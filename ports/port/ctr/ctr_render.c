/*
 * ctr_render.c  The frame's lists drawn by the PICA200 with citro3d, as rd_soft draws them
 *
 * Depth: the frame's 1/w range is cut into slices, each with its own part of the depth buffer, since
 * float24 can't hold sprites one bit apart. Palettes: fragment lighting looks the colour up, the texel
 * read as a normal and the palette in the RR, RG, RB and FR tables. The offset colour is a constant
 * per triangle.
 */

#include <math.h>
#include <stddef.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/iosupport.h>
#include <3ds.h>
#include <citro3d.h>

#include "rd.h"
#include "rd_list.h"
#include "ctr_render.h"
#include "ctr_front.h"
#include "ctr_render_shbin.h"
#include "prof.h"
#include "early_load.h"
#include "ctr_audio.h"
#include "../nl/nl_port.h"

void host_log(const char* fmt, ...);
void* host_try_malloc(size_t bytes);

#define VIEW_W 800                  /* the top screen at twice its size */
#define VIEW_H 480
#define GAME_X 80
#define MAX_VERTS 32768             /* the most a saved frame has used is about 14,300 */
#define MAX_BATCHES 8192
#define MAX_TEX 4096                /* a power of 2 */
#define CMDBUF_SIZE 0x180000
#define MAX_SLICES 16
#define LINEAR_KEEP (512u << 10)
#define SLICE_SPAN 2048.0f          /* a slice is at most this many times its closest two sprite depths */

#define DISPLAY_TRANSFER_FLAGS                                                                                   \
    (GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) |                            \
     GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8))

typedef struct {
    float x, y, d, w;
    float u, v;
    uint32_t c;                     /* ARGB, read as bytes B, G, R, A */
} GV;

/* triangles drawn with one state */
typedef struct {
    uint16_t state;
    uint16_t slice;
    uint32_t offset;
    int first, count;               /* vertices; count GPU_GROUP: the frame's GPU group first */
} BATCH;

#define GPU_GROUP (-1)

/* in cache lines: what a state's look reads, then what it copies, then the rest */
typedef struct {
    const RD_TEXTURE* key;
    uint32_t serial;
    uint32_t used;                  /* the frame last drawn in */
    union {
        uint32_t copied;            /* frame_no when copied into the frame's textures; 0 after it is made */
        uint32_t made;              /* with MVC2_TEX_COPY_SAME instead: tex's version, made_clock when it last changed */
    };
    uint16_t tw, th;                /* MVC2_TEX_CURRENT: the source's, when this was last made for it */
    int16_t lru_prev, lru_next;     /* the textures held, least recently drawn first */
    uint8_t tformat, tlevels;
    uint8_t ok;
    uint8_t pad;                    /* converted by pad_texels */
    uint8_t unpadded;               /* these texels also drawn where they aren't padded */
    uint8_t ahead;                  /* MVC2_TEX_AHEAD: made ahead, not drawn since */
    C3D_Tex tex __attribute__((aligned(32)));
    float su, sv;                   /* the source's part of it */
    int width, height;              /* powers of 2 */
    int levels;                     /* past the first, down to 8x8 */
    GPU_TEXCOLOR format;
    uint32_t changed;               /* frame_no when its texels last changed */
} GTEX;
_Static_assert(offsetof(GTEX, tex) == 32 && sizeof(GTEX) % 32 == 0, "a texture's parts on their own cache lines");

/* MVC2_TEX_LEAN=1: a state's texture looked up once, and in a build without the profile not timed one by one */
static int tex_lean;

/* fminf and fmaxf are calls here */
static inline float min_f(float a, float b) {
    return a < b ? a : b;
}

static inline float max_f(float a, float b) {
    return a > b ? a : b;
}

static inline double min_d(double a, double b) {
    return a < b ? a : b;
}

static inline double max_d(double a, double b) {
    return a > b ? a : b;
}

static C3D_RenderTarget* target;
static int target_w, target_h;      /* turned: 240x400, 240x800 wide, or 480x800 at full size */
static u32 output_flags;            /* the display transfer to the screen */
static int full_now;                /* 640x480 drawn, then halved down the screen, and across it unless wide */
static int wide_now = -1, wide_want = -1;  /* the top screen's 800 columns; -1: not on this model */
static int wide_at[16], wide_at_n, wide_at_i;  /* MVC2_WIDE_AT=frame,...: test runs, switched at each */
static int game_y0, game_h;         /* the game's columns on it */
static int stretch_now, stretch_want = -1;  /* ctr_render_set_stretch's, taken as the next frame begins */
static int stretch_at[16], stretch_at_n, stretch_at_i;  /* MVC2_STRETCH_AT=frame,...: test runs, switched at each */
static u32* pillars;                /* the bars' pictures as the target has them, not when stretched (ctr_art.c) */
static u32* pillars_made;           /* kept while stretched */
/* MVC2_PILLARS_ONCE=1: the pillars copied into the target once, and again after the HOME menu; a frame clears only
   the game's rows of the colour (its draws are scissored to them) */
static int pillars_once;
static int pillars_line;            /* MVC2_PILLARS_LINE=1 (on with PILLARS_ONCE): each bar copied as one line, not lines of 0 */
static volatile int pillars_new = 1;
static volatile int vram_lost;      /* after the HOME menu or sleep: VRAM's textures made again (MVC2_TEX_VRAM) */
static aptHookCookie apt_cookie;
static int apt_hooked;

static void apt_event(APT_HookType type, void* param) {
    (void)param;
    if (type == APTHOOK_ONRESTORE || type == APTHOOK_ONWAKEUP) {
        pillars_new = 1;
        vram_lost = 1;
    }
}
static DVLB_s* dvlb;
static shaderProgram_s prog;
static int u_proj;
static int u_tl, b_tl, b_tl_lit, b_tl_vtxcol, b_rcp_w;
static int rcp_w;                   /* MVC2_RCP_W: a vertex's w the shader's reciprocal of its 1/w, not divided here */
static C3D_AttrInfo cpu_attr, mesh_attr;
static u32 buf_base;                /* the attribute buffers' base, as BufInfo_Init sets it */
static C3D_Mtx proj;
static C3D_Mtx proj_corner;         /* proj without the half pixel of MVC2_TEXEL_CENTRE */
static int smooth_2d;               /* MVC2_SMOOTH_2D: the game's point-sampled textures, not palettes, drawn by
                                       proj_corner and filtered, each pixel the mean of its 2x2 texels */
static int proj_now;                /* the projection sent: proj_corner when 1 */
static int corner_ok;               /* in draw(), for the CPU's triangles, not the GPU's groups */
static C3D_LightEnv env;
static C3D_Light light;
static C3D_LightLut env_lut;

static GV* vbuf[2];                 /* each MAX_VERTS vertices, then MAX_VERTS indices */
static int indexed;                 /* drawn by index, a strip's shared corners made once; with MVC2_INDEXED=0
                                       each triangle has its own three vertices, and idx isn't used */
static GTEX gtex[MAX_TEX + 1];       /* the last only the least recently drawn list's ends */
static uint32_t frame_no;
static uint32_t fence;              /* memory drawn in this frame or after may still be read by the GPU */

/* tl_unif: the matrix, face, light and texture scale */
enum { TL_UNIFS = 7 };

/* MVC2_PREP_PACK=1: what group prep reads of a group's draw, state and slices, written as the group is added */
typedef struct {
    uint32_t face_argb;
    float su, sv;                   /* the state's */
    int32_t mesh;
    int32_t tw, th;                 /* the state's texture's size, when cand */
    uint16_t put;
    uint8_t cand;                   /* corner_wanted's answer */
    uint8_t over;                   /* not a translucent list's */
    uint8_t k0, k1;
    uint8_t faces;                  /* the group's */
} GPACK;
_Static_assert(sizeof(GPACK) == 32, "a record a cache line");

/* what a group's draw reads of its mesh and its draw */
typedef struct {
    uint8_t mesh_ok, kind, last_cull, faces;
    uint16_t nruns, ntri;
    u32 frag_words, frag_at;
    const u32* frag;
    int nv;
} GHOT;

/* a model group's draw in a slice, worked out before it is drawn */
typedef struct {
    C3D_FVec u[TL_UNIFS];           /* u[5] as the GPU has it, when keep5 */
    float depth_scale, depth_offset;    /* the depth map, with MVC2_TL_OVERLAP */
    float clip;                     /* the rows' largest sum over the mesh */
    uint8_t ok, corner, keep5;
    GHOT h;                         /* with MVC2_DRAW_HOT */
} __attribute__((aligned(32))) GPREP;

#define MAX_GPREP 1024

/* what build() makes and draw() reads */
typedef struct {
    GV* verts;                      /* linear memory: one of vbuf, two when the drawing has its own thread */
    uint16_t* idx;                  /* the triangles' vertices, three each */
    int nverts, nidx;
    int nbatches, nslices, nbg;
    int ngprep;
    double slice_lo[MAX_SLICES + 1];
    double slice_scale[MAX_SLICES], slice_base[MAX_SLICES];     /* depth in the slice = z * scale + base */
    RD_VTX bg_vtx[4];               /* top left, top right, bottom left, bottom right */
    uint8_t gpu_k0[RD_GPU_GROUPS], gpu_k1[RD_GPU_GROUPS];
    BATCH batches[MAX_BATCHES];
    GPREP gprep[MAX_GPREP];         /* a group's batch's offset is its place here, with MVC2_GROUP_PREP */
    GPACK gpack[MAX_GPREP] __attribute__((aligned(32)));     /* and here, with MVC2_PREP_PACK */
} LISTS;
static LISTS lists_mem;
static LISTS* L = &lists_mem;
static int group_prep;              /* a group's uniforms, corner and depth worked out with the lists; not with
                                       MVC2_GROUP_PREP=0 */
static int prep_pack;               /* MVC2_PREP_PACK=1, with MVC2_PREP_FAST2 and MVC2_GROUP_PREP */
static int draw_hot;                /* MVC2_DRAW_HOT=1, with MVC2_GROUP_PREP: what the draws read of the meshes made
                                       with the group's prep */
static inline int corner_wanted(const BATCH* b);
static void prep_groups(void);

/* group prep's routines together from a 4 KB boundary: their lines in the same cache sets whatever else moves */
#define PREP_HOT __attribute__((section(".text.prephot"), no_reorder))

static int max_idx = MAX_VERTS;     /* the build's room in idx, and in verts with the background's 9 after them */
static int early;                   /* serial: the lists built before the wait for the GPU; not with MVC2_EARLY_LISTS=0 */
static int early_short;             /* that build out of room, for vertices or a texture: built again after the wait */
static int early_sweep;             /* MVC2_EARLY_SWEEP, in a profile build: on and off by turns, 600 frames each */

/* what the build and the draws read of a frame, taken from the game at the handoff */
typedef struct {
    int16_t tex;                    /* in RFRAME.tex, -1 for none, or TEX_LATER */
    uint8_t pal, pal16;
    float su, sv;                   /* to the GPU's texture coordinates */
} STATE_TEX;

#define TEX_LATER (-2)                /* not made yet */

typedef struct {
    RD_FRAME* frame;
    STATE_TEX st[RD_MAX_STATES];
    C3D_Tex tex[MAX_TEX];           /* by gtex's index, as they were */
    uint32_t made[MAX_TEX];         /* MVC2_TEX_COPY_SAME: the version of each copied, 0 for none */
    uint32_t palette[1024], palette_serial;
    RD_FOG fog;
} RFRAME;

static RFRAME first_rframe;
static RFRAME* rframes[2] = { &first_rframe, NULL };
static RD_FRAME* rd_frames[2];
static RFRAME* rf;                  /* the frame being drawn, */
static RD_FRAME* fr;                /* and its lists */
static int pipe_on;
static int lists1;                  /* MVC2_LISTS_CORE1=1: the lists built on core 1, as much as it can */

/* the depth slices: slice k holds 1/w from slice_lo[k] to slice_lo[k + 1] */
#define MAX_KEYS (RD_POOL_TRIS + 2)
static uint32_t zkeys[MAX_KEYS], ztmp[MAX_KEYS];
static uint32_t zends[2][MAX_KEYS];   /* the triangles' nearest and farthest depths */
static int nends;
static RD_TRI bg_tri[2];            /* the background, when it sets the depth */
static int bg_state;
static int32_t slice_key[MAX_SLICES];       /* z_key of the least float at or above slice_lo */
static int keys_rise;                       /* slice_key never falls */
/* summed from the start, by one thread at a time */
static uint64_t ctr_render_ticks[CTR_TICKS];
#ifdef MVC2_PROF
#define TEX_MARK(v) uint64_t v = ctr_render_ticks[CTR_TICKS_TEX]   /* the textures' time since v, into a zone */
#define TEX_SINCE(zone, v) PROF_ADD((zone), ctr_render_ticks[CTR_TICKS_TEX] - (v))
#else
#define TEX_MARK(v) ((void)0)
#define TEX_SINCE(zone, v) ((void)0)
#endif
static uint32_t ctr_render_count[CTR_COUNTS];

/* MVC2_PERFZONES=1, in a profile build and without MVC2_PERFCTR: the event counters and cycles of the core that runs
   each of the lists' steps (with MVC2_LISTS_CORE1, core 1's apart), the draws and the library's models, two events at
   a time in turn, 60 frames each, and logged every 600 frames as a frame's */
enum { PFZ_LISTS, PFZ_SCAN, PFZ_SCAN_GROUPS, PFZ_SLICE_SORT, PFZ_ADD, PFZ_TRANS_SORT, PFZ_TRANS, PFZ_TEX, PFZ_GPREP,
       PFZ_DRAWS, PFZ_MODELS, PFZ_END, PFZ_NL_OBJ, PFZ_N };
#ifdef MVC2_PROF
static const char* const pfz_name[PFZ_N] = { "lists", "slice scan", "scan groups", "slice sort", "add",
                                             "trans sort", "trans add", "textures", "group prep", "draws",
                                             "model draws", "end", "nl models" };
static const PerfCounterEvent pfz_ev[5][2] = {
    { PERFCOUNTEREVT_CORE_DATA_CACHE_READ_MISS, PERFCOUNTEREVT_CORE_DATA_CACHE_LINE_EVICTION },
    { PERFCOUNTEREVT_CORE_DATA_CACHE_WRITE_MISS, PERFCOUNTEREVT_CORE_STALL_BY_DATA_HAZARD },
    { PERFCOUNTEREVT_CORE_MAIN_TLB_MISS, PERFCOUNTEREVT_CORE_DATA_MICRO_TLB_MISS },
    { PERFCOUNTEREVT_CORE_INST_EXECUTED, PERFCOUNTEREVT_CORE_INST_CACHE_MISS },
    { PERFCOUNTEREVT_CORE_STALL_BY_LSU_FULL, PERFCOUNTEREVT_CORE_STALL_BY_LACK_OF_INST },
};
static const char* const pfz_ev_name[5][2] = {
    { "D reads missed", "lines evicted" }, { "D writes missed", "data stalls" }, { "main TLB misses", "micro TLB misses" },
    { "instructions", "I misses" }, { "LSU full stalls", "instruction stalls" },
};
static int perf_zones, pfz_set = -1, pfz_set1 = -1;   /* the events on core 0 and on core 1 */
static Thread pfz_thread1;              /* the lists' thread on core 1 */
static uint64_t pfz_sum[2][PFZ_N][5][2], pfz_cycles[2][PFZ_N];
static uint32_t pfz_frames[5], pfz_all;

static void pfz_read(u32* v) {
    u64 x = 0;
    svcControlPerformanceCounter(&x, PERFCOUNTEROP_GET_VALUE, PERFCOUNTERREG_CORE_COUNT_REG_0, 0);
    v[0] = (u32)x;
    svcControlPerformanceCounter(&x, PERFCOUNTEROP_GET_VALUE, PERFCOUNTERREG_CORE_COUNT_REG_1, 0);
    v[1] = (u32)x;
    svcControlPerformanceCounter(&x, PERFCOUNTEROP_GET_VALUE, PERFCOUNTERREG_CORE_CYCLE_COUNTER, 0);
    v[2] = (u32)x;
}

static void pfz_events(int set) {
    u64 x;
    svcControlPerformanceCounter(&x, PERFCOUNTEROP_SET_EVENT, PERFCOUNTERREG_CORE_COUNT_REG_0, pfz_ev[set][0]);
    svcControlPerformanceCounter(&x, PERFCOUNTEROP_SET_EVENT, PERFCOUNTERREG_CORE_COUNT_REG_1, pfz_ev[set][1]);
}

static void pfz_add(int z, const u32* from) {
    u32 v[3];
    int c = pfz_thread1 != NULL && threadGetCurrent() == pfz_thread1, set = c ? pfz_set1 : pfz_set;
    if (set < 0) {
        return;     /* before the first lists */
    }
    pfz_read(v);
    pfz_sum[c][z][set][0] += v[0] - from[0];
    pfz_sum[c][z][set][1] += v[1] - from[1];
    pfz_cycles[c][z] += v[2] - from[2];
}

/* each frame, before the lists: the events for the next 60 frames, and the log every 600 */
static void pfz_frame(void) {
    int want = (int)(frame_no / 60 % 5), z, e, c;
    if (want != pfz_set) {
        pfz_events(want);
        pfz_set = want;
    }
    pfz_frames[pfz_set]++;
    if (++pfz_all < 600) {
        return;
    }
    for (c = 0; c < 2; c++) {
        for (z = 0; z < PFZ_N; z++) {
            char line[400];
            int n;
            if (c == 1 && pfz_cycles[c][z] == 0) {
                continue;
            }
            n = snprintf(line, sizeof line, "perf zones to frame %lu, a frame: %s%s %.0f cycles", (unsigned long)frame_no,
                         c ? "core 1 " : "", pfz_name[z], (double)pfz_cycles[c][z] / pfz_all);
            for (e = 0; e < 5 && n < (int)sizeof line; e++) {
                double f = pfz_frames[e] ? (double)pfz_frames[e] : 1.0;
                n += snprintf(line + n, sizeof line - n, ", %s %.0f, %s %.0f", pfz_ev_name[e][0], pfz_sum[c][z][e][0] / f,
                              pfz_ev_name[e][1], pfz_sum[c][z][e][1] / f);
            }
            host_log("%s", line);
        }
    }
    memset(pfz_sum, 0, sizeof pfz_sum);
    memset(pfz_cycles, 0, sizeof pfz_cycles);
    memset(pfz_frames, 0, sizeof pfz_frames);
    pfz_all = 0;
}
void prof_perf_models(int stop, uint32_t* v) {
    if (perf_zones) {
        if (stop) {
            pfz_add(PFZ_NL_OBJ, v);
        } else {
            pfz_read(v);
        }
    }
}

#define PFZ_START(v) u32 v[3] = { 0, 0, 0 }; if (perf_zones) pfz_read(v)
#define PFZ_AGAIN(v) do { if (perf_zones) pfz_read(v); } while (0)
#define PFZ_STOP(z, v) do { if (perf_zones) pfz_add((z), (v)); } while (0)
#else
#define PFZ_START(v) ((void)0)
#define PFZ_AGAIN(v) ((void)0)
#define PFZ_STOP(z, v) ((void)0)
#endif

#define MAX_PARTS 4096
#define MAX_ITEMS (RD_MAX_TRIS + MAX_PARTS)
_Static_assert(MAX_ITEMS <= 65536 && MAX_SLICES <= 16, "a translucent group's item and slices in one word");
static uint8_t tri_k0[MAX_ITEMS], tri_k1[MAX_ITEMS];    /* the slices a triangle reaches (k0 > k1: culled) */
/* each list's triangles' nearest and farthest 1/w, and whether they are culled or have no pixel, are in fr->info,
   made by make_slices, the first pass that reads the triangles */
static float bg_lo[2], bg_hi[2];
/* the GPU's groups: their nearest and farthest 1/w (0: not drawn), lists and slices */
static float gpu_lo[RD_GPU_GROUPS], gpu_hi[RD_GPU_GROUPS];
static float gpu_key[RD_GPU_GROUPS];   /* a translucent group's 1/w among its list's strips */
static uint8_t gpu_list[RD_GPU_GROUPS];
static int by_slice[MAX_ITEMS * 2];     /* slice k's from slice_at[k] to slice_at[k + 1] */
static int slice_at[MAX_SLICES + 1];

/* The translucent list as drawn, far first. An item's fields lie together: an Old 3DS stores to memory not in
   its cache a word at a time, and a record written whole is one run of words, not a word in each of many arrays */
typedef struct {
    const RD_TRI* tri;
    float lo, hi;           /* sorted by these; for a part, its polygon's */
    float tlo, thi;         /* its own 1/w */
    uint32_t key;           /* the middle's sort_bits */
    uint16_t run;           /* the items from this one made from one polygon, sharing its depths */
    uint8_t hidden;         /* one of the list's own, with no pixel */
} TITEM;

static TITEM titem[MAX_ITEMS];
static int ntrans, nparts;
static RD_TRI trans_part[MAX_PARTS];
static int trans_order[MAX_ITEMS];
static uint64_t group[MAX_ITEMS], group_tmp[MAX_ITEMS];    /* a group's key over its item, count and slices */
static uint32_t sprite_z[MAX_ITEMS], sprite_tmp[MAX_ITEMS];     /* the shallow ones' depths, sorted */

static uint8_t morton[64];          /* (y & 7) * 8 + (x & 7) -> place in the 8x8 tile */

/* the GPU's state, set only when it changes */

static struct {
    int valid;
    int func, zwrite, alpha_test, fog;
    int blend;                      /* src << 8 | dst, or -1 for none */
    int combine;
    uint32_t offset;
    const C3D_Tex* tex;
    int filter, wrap, mip_d;
    int corner;                     /* sampled at the game's pixels' corners (MVC2_SMOOTH_2D) */
    int lut_base, lut_n;
    uint32_t lut_serial;
    int lit;                        /* the lighting on (MVC2_LIGHT_PAL) */
} cur;

static int light_pal;               /* MVC2_LIGHT_PAL: the lighting on only for the palettes' draws, which read it */

static int raw_draws;               /* draws sent without citro3d, its words written here */
static int gpu_late_on;             /* a list's order-free GPU groups drawn together after it (MVC2_GPU_TL_LATE) */
static uint8_t gpu_late[RD_GPU_GROUPS];
static int last_dep[RD_LISTS];      /* each list's last triangle that must keep its place among the groups */
static int trans_strips;            /* the translucent list sorted a strip at a time, none cut */
static C3D_Tex* bind_tex;           /* the texture for the next draw, when it changes */
static C3D_TexEnv env_next[2];      /* the combiner's stages for the next draw, */
static int env_new;                 /* those changed by bit */

/* citro3d's effect block, as its update last sent it, with the changes made since: depth map, culling, the
   depth range, alpha, stencil and depth tests, the blending and the early depth test */
enum { FX_WORDS = 34, FX_ZBUF = 0, FX_ZSCALE = 4, FX_ZOFF = 6, FX_ALPHA = 8, FX_DEPTH = 12, FX_DEPTH2 = 14,
       FX_BLEND = 18, FX_MODE = 22 };
static const u32 fx_header[FX_WORDS / 2] = { 0x000F006D, 0x000F0040, 0x801F004D, 0, 0x803F0104, 0, 0, 0x00080126,
                                             0x000F0103, 0x000F0101, 0x000F0102, 0x00070100, 0x000F0130, 0x00010062,
                                             0x000F0118, 0x00010061, 0x0007006A };
static u32 fx[FX_WORDS];
static int fx_known;                /* fx is citro3d's: learnt this frame */
static int fx_new;                  /* an effect setting made since the block last went */
static int fx_wrong;                /* fx or fog found to differ from citro3d's: not sent directly again */

/* the same for the fog's three registers: its mode, the combiner buffer's colour and the fog's colour */
enum { FOG_WORDS = 6 };
static const u32 fog_header[FOG_WORDS / 2] = { 0x000700E0, 0x000F00FD, 0x000F00E1 };
static u32 fog[FOG_WORDS];
static int fog_known, fog_new;
static int fog_lut_new;             /* a fog table bound that citro3d sends with its next fogged draw */

static void fx_depth_test(int func, int mask) {
    C3D_DepthTest(true, (GPU_TESTFUNC)func, (GPU_WRITEMASK)mask);
    fx[FX_DEPTH] = 1 | ((u32)func << 4 & 0x70) | (u32)mask << 8;
    fx[FX_DEPTH2] = (u32)(0xAF02 >> (func * 2 & 14)) << 24 & 0x3000000;
    fx_new = 1;
}

static void fx_alpha_test(int on, int func, int ref) {
    C3D_AlphaTest(on, (GPU_TESTFUNC)func, ref);
    fx[FX_ALPHA] = (u32)on | ((u32)func << 4 & 0x70) | (u32)ref << 8;
    fx_new = 1;
}

static void fx_blend(GPU_BLENDFACTOR src, GPU_BLENDFACTOR dst) {
    C3D_AlphaBlend(GPU_BLEND_ADD, GPU_BLEND_ADD, src, dst, GPU_ONE, GPU_ZERO);
    fx[FX_BLEND] = GPU_BLEND_ADD | GPU_BLEND_ADD << 8 | (u32)src << 16 | (u32)dst << 20 | GPU_ONE << 24 | GPU_ZERO << 28;
    fx[FX_MODE] = (fx[FX_MODE] & ~0xFF00u) | 0x100;
    fx_new = 1;
}

/* MVC2_MAP_SAME=1: a depth map the same as citro3d's (it keeps them as float24) not set again, nor the effect sent */
static int map_same;

static void fx_depth_map(float scale, float offset) {
    u32 s = f32tof24(scale), o = f32tof24(offset);
    if (map_same && fx_known && fx[FX_ZBUF] == 1 && fx[FX_ZSCALE] == s && fx[FX_ZOFF] == o) {
        return;
    }
    C3D_DepthMap(true, scale, offset);
    fx[FX_ZBUF] = 1;
    fx[FX_ZSCALE] = s;
    fx[FX_ZOFF] = o;
    fx_new = 1;
}

/* fx keeps citro3d's effect as it is: the same culling is left alone */
static void fx_cull(GPU_CULLMODE mode) {
    if (fx[2] == ((u32)mode & 3)) {
        return;
    }
    C3D_CullFace(mode);
    fx[2] = (u32)mode & 3;
    fx_new = 1;
}

static void fog_mode(int on) {
    C3D_FogGasMode(on ? GPU_FOG : GPU_NO_FOG, GPU_PLAIN_DENSITY, false);
    fog[0] = (fog[0] & ~0x100FFu) | (on ? GPU_FOG : GPU_NO_FOG);
    fog_new = 1;
}

/* a block of n words at w, with its headers every other word (0 for none), learnt into kept and checked against it */
static int learn(const u32* w, u32 left, const u32* header, int n, u32* kept, int* known) {
    int i;
    if ((u32)n > left) {
        return 0;
    }
    for (i = 0; i < n / 2; i++) {
        if (header[i] != 0 && w[2 * i + 1] != header[i]) {
            return 0;
        }
    }
    if (*known && memcmp(kept, w, (size_t)n * 4) != 0 && !fx_wrong) {
        fx_wrong = 1;
        host_log("draws: the effect or fog kept here differs from citro3d's; they go through citro3d from now");
    }
    memcpy(kept, w, (size_t)n * 4);
    *known = !fx_wrong;
    return 1;
}

/* the effect and fog blocks in the words citro3d sent from at */
static void c3d_learn(u32 at) {
    u32 k = at;
    while (k + 2 <= gpuCmdBufOffset) {
        u32 n = (gpuCmdBuf[k + 1] >> 20 & 0xFF) + 1, end = gpuCmdBufOffset;
        if (gpuCmdBuf[k + 1] == fx_header[0]) {
            learn(gpuCmdBuf + k, end - k, fx_header, FX_WORDS, fx, &fx_known);
        }
        if (gpuCmdBuf[k + 1] == fog_header[0]) {
            learn(gpuCmdBuf + k, end - k, fog_header, FOG_WORDS, fog, &fog_known);
        }
        if (gpuCmdBuf[k + 1] == 0x07FF00E8) {     /* the table's 128 words */
            fog_lut_new = 0;
        }
        k += 2 + (n - 1) + ((n - 1) & 1);
    }
}

static const GPU_TESTFUNC depth_func[8] = { GPU_NEVER, GPU_LESS, GPU_EQUAL, GPU_LEQUAL,
                                            GPU_GREATER, GPU_NOTEQUAL, GPU_GEQUAL, GPU_ALWAYS };

static GPU_BLENDFACTOR blend_factor(int f, int is_src) {
    switch (f) {
    case RD_ZERO: return GPU_ZERO;
    case RD_ONE: return GPU_ONE;
    case RD_OTHER: return is_src ? GPU_DST_COLOR : GPU_SRC_COLOR;
    case RD_INV_OTHER: return is_src ? GPU_ONE_MINUS_DST_COLOR : GPU_ONE_MINUS_SRC_COLOR;
    case RD_SRC_ALPHA: return GPU_SRC_ALPHA;
    case RD_INV_SRC_ALPHA: return GPU_ONE_MINUS_SRC_ALPHA;
    case RD_DST_ALPHA: return GPU_DST_ALPHA;
    default: return GPU_ONE_MINUS_DST_ALPHA;
    }
}

static int pow2(int n) {
    int p = 8;
    while (p < n) {
        p <<= 1;
    }
    return p;
}

static int is_pal(int format) {
    return format == RD_PAL4 || format == RD_PAL8;
}

static GPU_TEXCOLOR gpu_format(int format) {
    switch (format) {
    case RD_PAL4:
    case RD_PAL8: return GPU_L8;
    case RD_RGB565: return GPU_RGB565;
    case RD_ARGB4444: return GPU_RGBA4;
    default: return GPU_RGBA5551;
    }
}

static uint32_t load32(const void* p) {
    uint32_t v;
    memcpy(&v, p, 4);
    return v;
}

/* RGBA4 and RGBA5551: the alpha moved to the bottom bits of both halves */
static inline __attribute__((always_inline)) uint32_t rot16(uint32_t v, int rot) {
    uint32_t low = rot == 4 ? 0x000F000Fu : 0x00010001u, top = low << (16 - rot);
    return rot == 0 ? v : (v & ~top) << rot | (v >> (16 - rot) & low);
}

/* In a tile, rows y and y + 1 (y even) take turns a pair of texels at a time: texels x and x + 1 of row y, then
   the same of row y + 1, at 0, 4, 16 and 20 texels past the rows' start for x = 0, 2, 4 and 6. Eight texels of
   each row at a time here, s and t the rows */
static inline __attribute__((always_inline)) void rows8(const uint8_t* s, const uint8_t* t, uint32_t* d) {
    uint32_t a0 = load32(s), a1 = load32(s + 4), b0 = load32(t), b1 = load32(t + 4);
    d[0] = (a0 & 0xFFFF) | b0 << 16;
    d[1] = a0 >> 16 | (b0 & 0xFFFF0000u);
    d[4] = (a1 & 0xFFFF) | b1 << 16;
    d[5] = a1 >> 16 | (b1 & 0xFFFF0000u);
}

static inline __attribute__((always_inline)) void rows16(const uint16_t* s, const uint16_t* t, uint32_t* d, int rot) {
    d[0] = rot16(load32(s), rot);
    d[1] = rot16(load32(t), rot);
    d[2] = rot16(load32(s + 2), rot);
    d[3] = rot16(load32(t + 2), rot);
    d[8] = rot16(load32(s + 4), rot);
    d[9] = rot16(load32(t + 4), rot);
    d[10] = rot16(load32(s + 6), rot);
    d[11] = rot16(load32(t + 6), rot);
}

/* two rows at a time, the last one alone when the height is odd */
static void level8(const uint8_t* src, int w, int h, uint8_t* dst, int tiles) {
    int x, y;
    for (y = 0; y < h; y += 2) {
        uint8_t* row = dst + (y >> 3) * tiles * 64 + morton[(y & 7) * 8];
        const uint8_t* s = src + y * w;
        if (y + 1 == h) {
            for (x = 0; x < w; x++) {
                row[(x >> 3) * 64 + morton[x & 7]] = s[x];
            }
            break;
        }
        for (x = 0; x + 8 <= w; x += 8) {
            rows8(s + x, s + w + x, (uint32_t*)(row + x * 8));
        }
        for (; x < w; x++) {
            row[(x >> 3) * 64 + morton[x & 7]] = s[x];
            row[(x >> 3) * 64 + morton[x & 7] + 2] = s[w + x];
        }
    }
}

static inline __attribute__((always_inline)) void level16(const uint16_t* src, int w, int h, uint16_t* dst, int tiles,
                                                          int rot) {
    int x, y;
    for (y = 0; y < h; y += 2) {
        uint16_t* row = dst + (y >> 3) * tiles * 64 + morton[(y & 7) * 8];
        const uint16_t* s = src + y * w;
        if (y + 1 == h) {
            for (x = 0; x < w; x++) {
                row[(x >> 3) * 64 + morton[x & 7]] = (uint16_t)rot16(s[x], rot);
            }
            break;
        }
        for (x = 0; x + 8 <= w; x += 8) {
            rows16(s + x, s + w + x, (uint32_t*)(row + x * 8), rot);
        }
        for (; x < w; x++) {
            row[(x >> 3) * 64 + morton[x & 7]] = (uint16_t)rot16(s[x], rot);
            row[(x >> 3) * 64 + morton[x & 7] + 2] = (uint16_t)rot16(s[w + x], rot);
        }
    }
}

static void convert_rows(const RD_TEXTURE* t, const GTEX* g, const void* texels, int w, int h, void* data, int tiles) {
    if (g->format == GPU_L8) {
        level8(texels, w, h, data, tiles);
    } else if (t->format == RD_ARGB4444) {
        level16(texels, w, h, data, tiles, 4);
    } else if (t->format == RD_RGB565) {
        level16(texels, w, h, data, tiles, 0);
    } else {
        level16(texels, w, h, data, tiles, 1);
    }
}

/* into the GPU's tiling: 8x8 tiles in rows, Morton order inside, the first row at v = 1 */
static void convert_level(const RD_TEXTURE* t, const GTEX* g, const void* texels, int w, int h, void* data, int gw) {
    convert_rows(t, g, texels, w, h, data, gw / 8);
}

/* a direct kernel call in the CIA, which may make it; else through the GPU service, a call to another process */
static int direct_flush;
static int cmd_self;                /* MVC2_CMD_FLUSH=self: the frame's commands flushed before C3D_FrameEnd */

/* the bottom screen's console, its writes watched so that it is flushed only after one */
static int console_every;            /* MVC2_CONSOLE_FLUSH=every: flushed each frame, written to or not */
static int console_written = 1;     /* set as each of the console's writes returns, cleared by the flush */
static devoptab_t console_tab;
static ssize_t (*console_write0)(struct _reent* r, void* fd, const char* p, size_t n);

static ssize_t console_write(struct _reent* r, void* fd, const char* p, size_t n) {
    ssize_t k = console_write0(r, fd, p, n);
    __atomic_store_n(&console_written, 1, __ATOMIC_RELEASE);
    return k;
}

/* the console's stdout and stderr, through console_write */
static void console_watch(void) {
    const devoptab_t* con = devoptab_list[STD_OUT];
    console_written = 1;
    if (con == &console_tab) {
        return;
    }
    if (con == NULL || con->write_r == NULL || con->name == NULL || strcmp(con->name, "con") != 0) {
        console_every = 1;
        return;
    }
    console_tab = *con;
    console_write0 = con->write_r;
    console_tab.write_r = console_write;
    if (devoptab_list[STD_ERR] == con) {
        devoptab_list[STD_ERR] = &console_tab;
    }
    devoptab_list[STD_OUT] = &console_tab;
}

/* a direct call that fails (a range not all mapped) is made again through the GPU service, that one only */
static void flush_one(const void* p, u32 size) {
    static unsigned failed;
    Result r;
    if (size == 0) {
        return;
    }
    if (direct_flush) {
        if (R_SUCCEEDED(r = svcFlushProcessDataCache(CUR_PROCESS_HANDLE, (u32)p, size))) {
            return;
        }
        if (failed++ < 4) {
            host_log("cache flushes: the direct call failed (%08lX, %lu bytes at %p), that one through the GPU service",
                     (unsigned long)r, (unsigned long)size, p);
        }
    }
    GSPGPU_FlushDataCache(p, size);
}

static void flush_range(const void* p, u32 size, int zone) {
    PROF_START(t0);
    flush_one(p, size);
    PROF_STOP(zone, t0);
}

/* Luma's svcFlushEntireDataCache (0x92); on the New 3DS it takes the L2 too */
static void flush_whole(void) {
    __asm__ volatile("svc 0x92" : : : "r0", "r1", "r2", "r3", "r12", "memory");
}

/* The textures converted since the last flush, flushed together before the GPU reads them: a call for each run
   of them closer than a call's cost, or with MVC2_TEX_FLUSH=whole the whole cache when that costs less. The costs
   are timed at start */
#define MAX_DIRTY 512

typedef struct {
    u32 lo, hi;
} RANGE;

static RANGE dirty[MAX_DIRTY];
static int ndirty;
static int flush_each;                      /* MVC2_TEX_FLUSH=each: each texture as it is converted */
static u32 cost_call, cost_kb, cost_whole;  /* ticks: a call, each KB of it, the whole cache (0: not used) */
static u32 merge_gap;                       /* bytes */
static int whole_always;                    /* MVC2_TEX_FLUSH=whole1: the whole cache, whatever the runs' cost */

static int range_cmp(const void* a, const void* b) {
    u32 x = ((const RANGE*)a)->lo, y = ((const RANGE*)b)->lo;
    return x < y ? -1 : x > y;
}

static void tex_flush(void) {
    int i, n = 0;
    u32 cost = 0;
    if (ndirty == 0) {
        return;
    }
    PROF_START(t0);
    qsort(dirty, (size_t)ndirty, sizeof *dirty, range_cmp);
    for (i = 1; i < ndirty; i++) {
        if (dirty[i].lo <= dirty[n].hi + merge_gap) {
            dirty[n].hi = dirty[i].hi > dirty[n].hi ? dirty[i].hi : dirty[n].hi;
        } else {
            dirty[++n] = dirty[i];
        }
    }
    n++;
    for (i = 0; i < n && cost_whole != 0 && cost <= cost_whole; i++) {
        cost += cost_call + (dirty[i].hi - dirty[i].lo) / 1024 * cost_kb;
    }
    if (cost_whole != 0 && (cost > cost_whole || whole_always)) {
        flush_whole();
        n = 1;
    } else {
        for (i = 0; i < n; i++) {
            flush_one((const void*)dirty[i].lo, dirty[i].hi - dirty[i].lo);
        }
    }
    ndirty = 0;
    PROF_COUNT(PZ_TEX_FLUSH, (uint32_t)n);
    PROF_STOP(PZ_TEX_FLUSH, t0);
}

static int in_vram(const void* p) {
    return (u32)p - OS_VRAM_VADDR < OS_VRAM_SIZE;
}

static int vram_noflush;            /* MVC2_TEX_VRAM_NOFLUSH=1, with MVC2_TEX_VRAM=2: VRAM's textures not flushed */

static void tex_written(const void* p, u32 size) {
    if (vram_noflush && in_vram(p)) {
        return;
    }
    if (flush_each) {
        flush_range(p, size, PZ_TEX_FLUSH);
        return;
    }
    if (ndirty == MAX_DIRTY) {
        tex_flush();
    }
    dirty[ndirty].lo = (u32)p;
    dirty[ndirty].hi = (u32)p + size;
    ndirty++;
}

/* a texture copy by the GPU of a, into b; 1 if b has only v */
static int copy_reads(u32* a, u32* b, u32 size, u32 v) {
    u32 i;
    C3D_SyncTextureCopy(a, GX_BUFFER_DIM(size / 16, 0), b, GX_BUFFER_DIM(size / 16, 0), size, GX_TRANSFER_RAW_COPY(1));
    svcInvalidateProcessDataCache(CUR_PROCESS_HANDLE, (u32)b, size);
    for (i = 0; i < size / 4 && b[i] == v; i++) {
    }
    return i == size / 4;
}

/* the whole-cache call used only where the GPU is seen to miss data left in the cache, and to read it after the call */
static const char* whole_probe(u32* a, u32* b, u32 size) {
    s64 luma;
    if (R_FAILED(svcGetSystemInfo(&luma, 0x10000, 0))) {
        return "not Luma";
    }
    memset(a, 1, size);
    flush_one(a, size);
    flush_one(b, size);
    memset(a, 2, size);
    if (copy_reads(a, b, size, 0x02020202u)) {
        return "the GPU reads the cache's data without a flush";
    }
    flush_one(a, size);
    memset(a, 3, size);
    flush_whole();
    if (!copy_reads(a, b, size, 0x03030303u)) {
        return "the call left data in the cache";
    }
    return NULL;
}

static void tex_flush_init(void) {
    enum { SPAN = 256 << 10, PIECE = 1024, PIECES = 16, PROBE = 4096 };
    const char* e = getenv("MVC2_TEX_FLUSH");
    const char* why = NULL;
    uint8_t* buf = linearAlloc(SPAN);
    u32 calls = 0, span, t;
    int i;
    flush_each = e != NULL && strcmp(e, "each") == 0;
    if (flush_each || buf == NULL) {
        merge_gap = 0;
        host_log("texture flushes: each as it is converted%s", flush_each ? " (MVC2_TEX_FLUSH=each)" : "");
        flush_each = 1;
        linearFree(buf);
        return;
    }
    for (i = 0; i < PIECES; i++) {
        memset(buf + i * (SPAN / PIECES), i, PIECE);
        t = (u32)svcGetSystemTick();
        flush_one(buf + i * (SPAN / PIECES), PIECE);
        calls += (u32)svcGetSystemTick() - t;
    }
    cost_call = calls / PIECES;
    memset(buf, 0, SPAN);
    t = (u32)svcGetSystemTick();
    flush_one(buf, SPAN);
    span = (u32)svcGetSystemTick() - t;
    cost_kb = span > cost_call ? (span - cost_call) / (SPAN / 1024 - 1) : 0;
    merge_gap = cost_kb != 0 && cost_call / cost_kb * 1024 < SPAN ? cost_call / cost_kb * 1024 : SPAN;
    if (e == NULL || (strcmp(e, "whole") != 0 && strcmp(e, "whole1") != 0)) {
        why = "only with MVC2_TEX_FLUSH=whole";
    } else if (!direct_flush) {
        why = "no direct calls";
    } else {
        why = whole_probe((u32*)buf, (u32*)(buf + PROBE), PROBE);
    }
    if (why == NULL) {
        memset(buf, 4, PIECE);
        t = (u32)svcGetSystemTick();
        flush_whole();
        cost_whole = (u32)svcGetSystemTick() - t;
        cost_whole = cost_whole != 0 ? cost_whole : 1;
        whole_always = strcmp(e, "whole1") == 0;
    }
    linearFree(buf);
    if (why != NULL) {
        host_log("texture flushes: together, a call %lu ticks and %lu a KB, runs joined across %lu KB; not the whole cache: %s",
                 (unsigned long)cost_call, (unsigned long)cost_kb, (unsigned long)(merge_gap / 1024), why);
    } else {
        host_log("texture flushes: together, a call %lu ticks and %lu a KB, runs joined across %lu KB; the whole cache %lu%s",
                 (unsigned long)cost_call, (unsigned long)cost_kb, (unsigned long)(merge_gap / 1024), (unsigned long)cost_whole,
                 whole_always ? ", always (MVC2_TEX_FLUSH=whole1)" : "");
    }
}

/* MVC2_DRAW_COPY=1: the draws' and group prep's copies in 16-byte pieces, which gcc makes ldm and stm; memcpy, which
   gcc calls for any copy over 64 bytes, is newlib's word loop */
static int draw_copy;

typedef struct {
    u32 w[4];
} WORDS4;

/* n bytes, a multiple of 4 known when compiled */
static inline __attribute__((always_inline)) void copy_fixed(void* restrict to, const void* restrict from, const size_t n) {
    WORDS4* a = to;
    const WORDS4* b = from;
    u32* x = (u32*)to + n / 16 * 4;
    const u32* y = (const u32*)from + n / 16 * 4;
    size_t i;
#pragma GCC unroll 16
    for (i = 0; i < n / 16; i++) {
        a[i] = b[i];
    }
#pragma GCC unroll 3
    for (i = 0; i < n % 16 / 4; i++) {
        x[i] = y[i];
    }
}

/* k pieces of 16 bytes, k from 0 to 7 */
static inline void copy_pieces(void* restrict to, const void* restrict from, int k) {
    WORDS4* a = to;
    const WORDS4* b = from;
    switch (k) {
    case 7:
        a[6] = b[6];
        /* fall through */
    case 6:
        a[5] = b[5];
        /* fall through */
    case 5:
        a[4] = b[4];
        /* fall through */
    case 4:
        a[3] = b[3];
        /* fall through */
    case 3:
        a[2] = b[2];
        /* fall through */
    case 2:
        a[1] = b[1];
        /* fall through */
    case 1:
        a[0] = b[0];
        /* fall through */
    default:
        break;
    }
}

#ifdef MVC2_PROF
/* for the log, on the game's thread: the address space's blocks as svcQueryMemory gives them (those in use), and
   the ticks of flushes of lines already clean, 4 KB (the least of four, and the most) and 64 KB */
void ctr_flush_probe(uint32_t out[5]) {
    static uint8_t buf[64 << 10] __attribute__((aligned(32)));
    static int written;
    MemInfo mi;
    PageInfo pi;
    u32 a = 0, t;
    int i;
    out[0] = out[1] = out[3] = 0;
    out[2] = 0xFFFFFFFFu;
    while (a < 0x40000000u && R_SUCCEEDED(svcQueryMemory(&mi, &pi, a)) && mi.size != 0) {
        out[0]++;
        out[1] += mi.state != MEMSTATE_FREE;
        a = mi.base_addr + mi.size;
    }
    if (!written) {
        memset(buf, 1, sizeof buf);
        flush_one(buf, sizeof buf);
        written = 1;
    }
    for (i = 0; i < 4; i++) {
        t = (u32)svcGetSystemTick();
        flush_one(buf + i * 4096, 4096);
        t = (u32)svcGetSystemTick() - t;
        out[2] = t < out[2] ? t : out[2];
        out[3] = t > out[3] ? t : out[3];
    }
    t = (u32)svcGetSystemTick();
    flush_one(buf, sizeof buf);
    out[4] = (u32)svcGetSystemTick() - t;
}
#endif

#ifdef MVC2_PROF
/* MVC2_STORE_PROBE=1: 88-byte items written back to back, a word at a time, by memcpy, by ldm and stm and by the
   faster memcpy (MVC2_MEMCPY_FAST's), into lines not in the cache, the same with the cache full of dirty lines, a
   stream past the cache's size, lines read first (all, or each item's just before it), lines cached and dirty, and
   each item read again just after it is written */
enum { SP_ITEM = 88, SP_SPAN = 8 << 10, SP_STREAM = 64 << 10, SP_FILL = 32 << 10, SP_RUNS = 8, SP_CASES = 7, SP_WAYS = 4 };

static u32 sp_src[SP_ITEM / 4];
static volatile u32 sp_len = SP_ITEM, sp_sink;

static void __attribute__((noinline)) sp_store(uint8_t* p, u32 bytes, int copy, int read_each, int read_after) {
    u32 n = bytes / SP_ITEM, len = sp_len, i, k;
    for (i = 0; i < n; i++, p += SP_ITEM) {
        if (read_each) {
            uintptr_t a;
            for (a = (uintptr_t)p & ~(uintptr_t)31; a < (uintptr_t)p + SP_ITEM; a += 32) {
                sp_sink = *(volatile const u32*)a;
            }
        }
        if (copy == 1) {
            __real_memcpy(p, sp_src, len);
        } else if (copy == 2) {
            copy_fixed(p, sp_src, SP_ITEM);
        } else if (copy == 3) {
            ctr_memcpy(p, sp_src, len);
        } else {
            volatile u32* w = (volatile u32*)p;
            for (k = 0; k < SP_ITEM / 4; k++) {
                w[k] = k;
            }
        }
        if (read_after) {
            uintptr_t a;
            for (a = (uintptr_t)p & ~(uintptr_t)31; a < (uintptr_t)p + SP_ITEM; a += 32) {
                sp_sink = *(volatile const u32*)a;
            }
        }
    }
}

static void sp_read(const uint8_t* p, u32 bytes) {
    u32 a;
    for (a = 0; a < bytes; a += 32) {
        sp_sink = *(volatile const u32*)(p + a);
    }
}

static int sp_cmp(const void* a, const void* b) {
    u32 x = *(const u32*)a, y = *(const u32*)b;
    return x < y ? -1 : x > y;
}

static void store_probe(void) {
    static const char* const what[SP_CASES] = { "untouched", "after dirty lines", "a 64 KB stream", "read first",
                                                "each item read just before", "cached and dirty",
                                                "each item read just after" };
    static const char* const how[SP_WAYS] = { "words", "memcpy", "ldm/stm", "fast memcpy" };
    uint8_t* base = linearAlloc(SP_STREAM + SP_FILL);
    u32 t[SP_RUNS], reads[SP_RUNS], t0;
    double ns[SP_WAYS][SP_CASES][2], rd = 0;
    int copy, c, k;
    if (base == NULL) {
        host_log("store probe: no linear memory");
        return;
    }
    for (copy = 0; copy < SP_WAYS; copy++) {
        for (c = 0; c < SP_CASES; c++) {
            u32 bytes = c == 2 ? SP_STREAM : SP_SPAN;
            for (k = 0; k < SP_RUNS; k++) {
                flush_one(base, bytes);
                if (c == 1) {
                    memset(base + SP_STREAM, k, SP_FILL);
                } else if (c == 3) {
                    t0 = (u32)svcGetSystemTick();
                    sp_read(base, bytes);
                    reads[k] = (u32)svcGetSystemTick() - t0;
                } else if (c == 5) {
                    sp_read(base, bytes);
                    sp_store(base, bytes, copy, 0, 0);
                }
                t0 = (u32)svcGetSystemTick();
                sp_store(base, bytes, copy, c == 4, c == 6);
                t[k] = (u32)svcGetSystemTick() - t0;
            }
            qsort(t, SP_RUNS, sizeof t[0], sp_cmp);
            ns[copy][c][0] = t[0] * 1e9 / SYSCLOCK_ARM11 / (bytes / SP_ITEM);
            ns[copy][c][1] = t[SP_RUNS / 2] * 1e9 / SYSCLOCK_ARM11 / (bytes / SP_ITEM);
            if (c == 3 && copy == 0) {
                qsort(reads, SP_RUNS, sizeof reads[0], sp_cmp);
                rd = reads[0] * 1e9 / SYSCLOCK_ARM11 / (SP_SPAN / 32);
            }
        }
    }
    linearFree(base);
    host_log("store probe: an 88-byte item in ns, the least and the median of %d, items back to back over %d KB "
             "(MVC2_STORE_PROBE=1); reading a line not in the cache %.0f ns", SP_RUNS, SP_SPAN >> 10, rd);
    for (copy = 0; copy < SP_WAYS; copy++) {
        char buf[400];
        int len = snprintf(buf, sizeof buf, "  %s:", how[copy]);
        for (c = 0; c < SP_CASES && len < (int)sizeof buf; c++) {
            len += snprintf(buf + len, sizeof buf - (size_t)len, "%s %s %.0f/%.0f", c ? "," : "", what[c],
                            ns[copy][c][0], ns[copy][c][1]);
        }
        host_log("%s", buf);
    }
}
#endif

/* A twiddled 8x8 block is 64 texels in a row, x in the index's odd bits and y in the even; a tile has them the
   other way round. Each 4 texels of the source, (x, y) to (x + 1, y + 1), are a word of the tile. A rectangle is
   squares of its shorter side, one after another along the longer */
static void convert_twiddled4(const uint8_t* src, int w, int h, uint8_t* data) {
    static const uint8_t place[16] = { 0, 2, 1, 3, 8, 10, 9, 11, 4, 6, 5, 7, 12, 14, 13, 15 };
    uint32_t* d = (uint32_t*)data;
    uint32_t n = (uint32_t)(w < h ? w : h), tiles = n / 8, square = n * n / 8;     /* a square's words */
    uint32_t x, y, sx, sy;
    /* sx and sy count up in the odd and the even bits, from 0 again at each square */
    for (y = 0, sy = 0; y < (uint32_t)h / 8; y++, sy = y % tiles ? ((sy | 0xAAAAAAAAu) + 1) & 0x55555555u : 0) {
        const uint32_t* row = (const uint32_t*)src + (w < h ? y / tiles * square : 0);
        for (x = 0, sx = 0; x < (uint32_t)w / 8; x++, sx = x % tiles ? ((sx | 0x55555555u) + 1) & 0xAAAAAAAAu : 0, d += 16) {
            const uint32_t* s = row + (w > h ? x / tiles * square : 0) + (sx | sy) * 8;
            int k;
#pragma GCC unroll 8
            for (k = 0; k < 8; k++) {
                uint32_t lo = s[k] & 0x0F0F0F0Fu, hi = s[k] >> 4 & 0x0F0F0F0Fu;
                d[place[2 * k]] = (lo & 0xFFFF) | hi << 16;
                d[place[2 * k + 1]] = lo >> 16 | (hi & 0xFFFF0000u);
            }
        }
    }
}

/* MVC2_SMOOTH_2D: a copy of t's texels with each clear one given its neighbours' colour, by their alpha, so that
   filtering across a shape's edge doesn't take the clear texels' colour into it */
#define PAD_MAX (256 * 128)         /* the font's size */
#define PAD_STILL 30                /* frames unchanged before a texture is padded again */
static uint16_t* pad_buf;

/* a texel's colour times its alpha in 16-bit lanes, and its alpha in the top one */
static uint64_t pad_lanes(uint16_t q, int sh, int bits, int mask) {
    uint64_t a = (uint64_t)(q >> sh);
    return ((uint64_t)(q & mask) | (uint64_t)(q >> bits & mask) << 16 | (uint64_t)(q >> 2 * bits & mask) << 32) * a | a << 48;
}

static uint64_t pad_inv[8 * 15 + 1];     /* 2^32 / n, rounded up: (s * inv[n]) >> 32 is s / n for any s here */

/* a clear texel's colour from v, its neighbours' sums, n of their alpha */
static uint16_t pad_colour(uint64_t v, uint32_t n, int bits) {
    uint16_t p = 0;
    int c;
    for (c = 0; c < 3; c++) {
        uint32_t sum = (uint32_t)(v >> (16 * c) & 0xFFFF) + n / 2;
        p |= (uint16_t)((uint32_t)((uint64_t)sum * pad_inv[n] >> 32) << (c * bits));
    }
    return p;
}

/* row y's texels with alpha, a bit each; none outside the texture */
static void pad_row_bits(const uint16_t* src, int w, int h, int y, int sh, uint32_t* m) {
    int x;
    memset(m, 0, (size_t)(w + 31) / 32 * sizeof *m);
    if (y < 0 || y >= h) {
        return;
    }
    for (x = 0; x < w; x++) {
        m[x >> 5] |= (uint32_t)(src[y * w + x] >> sh != 0) << (x & 31);
    }
}

/* MVC2_PAD_FAST (on): the texels copied, then only the clear ones beside one that isn't worked out */
static int pad_fast = 1;

static void pad_some(const uint16_t* src, int w, int h, int sh, int bits, int mask) {
    static uint32_t rows[3][1024 / 32];
    int words = (w + 31) / 32, y, i;
    memcpy(pad_buf, src, (size_t)w * h * sizeof *pad_buf);
    pad_row_bits(src, w, h, -1, sh, rows[2]);
    pad_row_bits(src, w, h, 0, sh, rows[0]);
    for (y = 0; y < h; y++) {
        const uint32_t *up = rows[(y + 2) % 3], *mid = rows[y % 3];
        uint32_t* down = rows[(y + 1) % 3];
        pad_row_bits(src, w, h, y + 1, sh, down);
        for (i = 0; i < words; i++) {
            uint32_t m = up[i] | mid[i] | down[i];
            uint32_t ml = i > 0 ? up[i - 1] | mid[i - 1] | down[i - 1] : 0;
            uint32_t mr = i + 1 < words ? up[i + 1] | mid[i + 1] | down[i + 1] : 0;
            uint32_t cand = (m | m << 1 | ml >> 31 | m >> 1 | mr << 31) & ~mid[i];
            while (cand != 0) {
                int x = i * 32 + __builtin_ctz(cand), dy, dx;
                uint64_t v = 0;
                cand &= cand - 1;
                if (x >= w) {
                    break;
                }
                for (dy = y > 0 ? -1 : 0; dy <= 1 && y + dy < h; dy++) {
                    for (dx = x > 0 ? -1 : 0; dx <= 1 && x + dx < w; dx++) {
                        v += pad_lanes(src[(y + dy) * w + x + dx], sh, bits, mask);
                    }
                }
                pad_buf[y * w + x] = pad_colour(v, (uint32_t)(v >> 48), bits);
            }
        }
    }
}

static const void* pad_texels(const RD_TEXTURE* t) {
    static uint64_t across[3][1024];     /* rows' sums of each texel with the ones either side */
    const uint16_t* src = t->texels;
    int x, y, w = t->width, h = t->height, sh = t->format == RD_ARGB4444 ? 12 : 15;
    int bits = t->format == RD_ARGB4444 ? 4 : 5, mask = (1 << bits) - 1;
    if (pad_buf == NULL && (pad_buf = host_try_malloc(PAD_MAX * sizeof *pad_buf)) == NULL) {
        return t->texels;
    }
    if (pad_inv[1] == 0) {
        for (x = 1; x <= 8 * 15; x++) {
            pad_inv[x] = ((1ull << 32) + (uint64_t)x - 1) / (uint64_t)x;
        }
    }
    if (pad_fast) {
        pad_some(src, w, h, sh, bits, mask);
        return pad_buf;
    }
    /* row y's sums in across[(y + 1) % 3], the rows outside the texture none */
    memset(across[0], 0, sizeof across[0]);
    for (y = 0; y <= h; y++) {
        uint64_t* row = across[(y + 1) % 3];
        if (y < h) {
            const uint16_t* s = src + y * w;
            uint64_t prev = 0, at = pad_lanes(s[0], sh, bits, mask);
            for (x = 0; x < w; x++) {
                uint64_t next = x + 1 < w ? pad_lanes(s[x + 1], sh, bits, mask) : 0;
                row[x] = prev + at + next;
                prev = at;
                at = next;
            }
        } else {
            memset(row, 0, (size_t)w * sizeof *row);
        }
        if (y == 0) {
            continue;
        }
        /* row y - 1, with the rows above and below it */
        {
            const uint64_t *up = across[(y - 1) % 3], *mid = across[y % 3], *down = row;
            const uint16_t* s = src + (y - 1) * w;
            uint16_t* d = pad_buf + (y - 1) * w;
            for (x = 0; x < w; x++) {
                uint16_t p = s[x];
                uint64_t v;
                uint32_t n;
                if (p >> sh != 0 || (n = (uint32_t)((v = up[x] + mid[x] + down[x]) >> 48)) == 0) {
                    d[x] = p;   /* not clear, or with no neighbour that isn't */
                    continue;
                }
                d[x] = pad_colour(v, n, bits);
            }
        }
    }
    return pad_buf;
}

/* MVC2_VQ_TILES (on): a VQ square that keeps only its source tiled straight from it, unless padded. A 2x2 block is a
   codebook entry, its texels (0, 0), (0, 1), (1, 0), (1, 1), and a tile's 16 blocks are 16 index bytes in a row,
   twiddled as a PAL4 square's texels are. Mip levels' indices come smallest first, from 1x1 */
static int vq_tiles = 1;
static int tex_current;             /* MVC2_TEX_CURRENT=1: a state's texture made and unchanged taken as it was kept */
static int tex_vram;                /* MVC2_TEX_VRAM=1: those of 128x128 texels or more in VRAM */
static int vram_direct;             /* MVC2_TEX_VRAM=2: converted straight into it, not through the staging ring */
static int vram_moves;              /* with it, those left in linear memory moved there (MVC2_TEX_VRAM_MOVE=0: not) */
static int tex_unpark;              /* MVC2_TEX_UNPARK=1: older ones in VRAM freed to make room for a move */

static void convert_vq(const RD_TEXTURE* t, GTEX* g) {
    static const uint8_t place[16] = { 0, 2, 1, 3, 8, 10, 9, 11, 4, 6, 5, 7, 12, 14, 13, 15 };
    static uint64_t book[256];
    const uint16_t* c = t->texels;
    int rot = t->format == RD_ARGB4444 ? 4 : t->format == RD_RGB565 ? 0 : 1, i, k;
    for (i = 0; i < 256; i++, c += 4) {
        book[i] = rot16(c[0] | (uint32_t)c[2] << 16, rot) | (uint64_t)rot16(c[1] | (uint32_t)c[3] << 16, rot) << 32;
    }
    for (k = 0; k <= g->levels; k++) {
        const uint8_t* index = (const uint8_t*)t->texels + 2048;
        uint64_t* d = k == 0 ? g->tex.data : C3D_Tex2DGetImagePtr(&g->tex, k, NULL);
        uint32_t n = (uint32_t)t->width >> k, tiles = n / 8, x, y, sx, sy, s;
        if (t->levels > 0) {
            for (index += 1, s = 2; s < n; s <<= 1) {
                index += (s / 2) * (s / 2);
            }
        }
        for (y = 0, sy = 0; y < tiles; y++, sy = ((sy | 0xAAAAAAAAu) + 1) & 0x55555555u) {
            for (x = 0, sx = 0; x < tiles; x++, sx = ((sx | 0x55555555u) + 1) & 0xAAAAAAAAu, d += 16) {
                const uint8_t* b = index + (sx | sy) * 16;
#pragma GCC unroll 16
                for (i = 0; i < 16; i++) {
                    d[place[i]] = book[b[i]];
                }
            }
        }
    }
}

/* all of a texture's levels: C3D_Tex's size is the first's */
static u32 tex_bytes(const C3D_Tex* t) {
    return C3D_TexCalcTotalSize(t->size, t->maxLevel);
}

static void convert(const RD_TEXTURE* t, GTEX* g) {
    int k;
    if (g->pad) {
        convert_level(t, g, pad_texels(t), t->width, t->height, g->tex.data, g->width);
        tex_written(g->tex.data, tex_bytes(&g->tex));
        return;
    }
    if (t->unpack != NULL) {
        convert_vq(t, g);
        tex_written(g->tex.data, tex_bytes(&g->tex));
        return;
    }
    if (t->twiddled != NULL && g->format == GPU_L8 && g->width == t->width && g->height == t->height && g->levels == 0) {
        convert_twiddled4(t->twiddled, t->width, t->height, g->tex.data);
        tex_written(g->tex.data, tex_bytes(&g->tex));
        return;
    }
    convert_level(t, g, t->texels, t->width, t->height, g->tex.data, g->width);
    for (k = 1; k <= g->levels; k++) {
        convert_level(t, g, t->mip[k], t->width >> k, t->height >> k, C3D_Tex2DGetImagePtr(&g->tex, k, NULL),
                      g->width >> k);
    }
    tex_written(g->tex.data, tex_bytes(&g->tex));
}

/* the library's mip levels, down to 8x8, when its size is the GPU's */
static int gpu_levels(const RD_TEXTURE* t, int w, int h) {
    int k = 0;
    if (t->levels > 0 && w == t->width && h == t->height) {
        while (k < t->levels && (w >> (k + 1)) >= 8 && (h >> (k + 1)) >= 8) {
            k++;
        }
    }
    return k;
}

/* the textures held, least recently drawn first, so none is looked for in the whole table */
#define LRU_END MAX_TEX
#define lru_prev(i) gtex[i].lru_prev
#define lru_next(i) gtex[i].lru_next

static void lru_clear(void) {
    lru_prev(LRU_END) = lru_next(LRU_END) = LRU_END;
}

static void lru_unlink(int i) {
    lru_next(lru_prev(i)) = lru_next(i);
    lru_prev(lru_next(i)) = lru_prev(i);
}

static void lru_append(int i) {
    lru_prev(i) = lru_prev(LRU_END);
    lru_next(i) = LRU_END;
    lru_next(lru_prev(LRU_END)) = (int16_t)i;
    lru_prev(LRU_END) = (int16_t)i;
}

static void tex_used(GTEX* g) {
    if (g->used != frame_no) {
        g->used = frame_no;
        lru_unlink((int)(g - gtex));
        lru_append((int)(g - gtex));
    }
}

static int busy(uint32_t drawn) {
    return drawn >= fence;
}

/* memory the GPU may still read, freed when it can't: at most one frame's textures */
static struct {
    C3D_Tex tex;
    uint32_t drawn;
} *retired;
static int nretired;

static void tex_free(C3D_Tex* t, uint32_t drawn) {
    if (busy(drawn) && retired != NULL) {
        retired[nretired].tex = *t;
        retired[nretired++].drawn = drawn;
    } else {
        C3D_TexDelete(t);
    }
}

static void retired_free(void) {
    int i, n = 0;
    for (i = 0; i < nretired; i++) {
        if (busy(retired[i].drawn)) {
            retired[n++] = retired[i];
        } else {
            C3D_TexDelete(&retired[i].tex);
        }
    }
    nretired = n;
}

/* MVC2_TEX_COPY_SAME=1: a texture copied into a frame's table only when changed since that table last had it */
static int copy_same;
static uint32_t made_clock;

static void tex_changed(GTEX* g) {
    if (copy_same) {
        g->made = ++made_clock;
    }
}

static void tex_drop(GTEX* g) {
    tex_changed(g);
    tex_free(&g->tex, g->used);
    g->ok = 0;
    lru_unlink((int)(g - gtex));
}

/* frees the least recently drawn in VRAM (vram 1) or linear memory; 0 when the GPU may still read it */
static int free_oldest(int vram) {
    int i;
    for (i = lru_next(LRU_END); i != LRU_END && !busy(gtex[i].used); i = lru_next(i)) {
        if (in_vram(gtex[i].tex.data) == vram) {
            tex_drop(&gtex[i]);
            ctr_render_count[CTR_COUNT_FREED]++;
            return 1;
        }
    }
    return 0;
}

/* the memory of one of the least recently drawn of the same size and format and in the same place, for g */
static int take_same(GTEX* g, int w, int h, int levels, GPU_TEXCOLOR f, int vram) {
    int i, n;
    for (i = lru_next(LRU_END), n = 0; i != LRU_END && n < 32 && !busy(gtex[i].used); i = lru_next(i), n++) {
        GTEX* old = &gtex[i];
        if (old->width == w && old->height == h && old->levels == levels && old->format == f &&
            in_vram(old->tex.data) == vram) {
            g->tex = old->tex;
            old->ok = 0;
            tex_changed(old);
            lru_unlink(i);
            ctr_render_count[CTR_COUNT_FREED]++;
            return 1;
        }
    }
    return 0;
}

/* MVC2_TEX_VRAM: a texture in VRAM is converted into a bank of the staging ring, one bank a frame, and copied by the
   GPU before the frame's list. A bank is used again once the GPU has done that frame's copies */
#define STAGE_COPIES 16             /* a frame's, beside its own few in citro3d's queue of 32 */
static u32 vram_keep = 64u << 10;  /* VRAM left free, MVC2_TEX_VRAM_KEEP in KB */
static uint8_t* staging_buf;
static u32 staging_size;              /* a bank's bytes */
static u32 staging_off[2], staging_frame[2];
static int staging_live[2];
static struct {
    void *src, *dst;
    u32 size;
    u32 dim, flags;                 /* MVC2_TEX_GX: tiled by the display transfer (dim not 0), else copied */
} staged[STAGE_COPIES];
static int nstaged;
static uint32_t staged_frame;
static uint32_t vram_textures, vram_moved, vram_no_room, vram_no_stage, vram_freed;
static uint32_t unpark_freed;

/* the frame's bank, -1 while the GPU may still copy from it or another frame's copies wait */
static int staging_bank(void) {
    int b = (int)(frame_no & 1);
    if (nstaged != 0 && staged_frame != frame_no) {
        return -1;
    }
    if (!staging_live[b] || staging_frame[b] != frame_no) {
        if (staging_live[b] && busy(staging_frame[b])) {
            return -1;
        }
        staging_live[b] = 1;
        staging_frame[b] = frame_no;
        staging_off[b] = 0;
    }
    return b;
}

static int staging_room(u32 size) {
    int b;
    if (nstaged == STAGE_COPIES || size > staging_size || size / 16 > 0xFFFF || (b = staging_bank()) < 0) {
        return 0;
    }
    return staging_off[b] + size <= staging_size;
}

static void* staging_take(u32 size) {
    int b = staging_bank();
    void* p = staging_buf + (u32)b * staging_size + staging_off[b];
    staging_off[b] += (size + 127) & ~127u;
    return p;
}

static void staging_add(void* src, void* dst, u32 size) {
    tex_written(src, size);
    staged[nstaged].src = src;
    staged[nstaged].dst = dst;
    staged[nstaged].size = size;
    staged[nstaged].dim = 0;
    nstaged++;
    staged_frame = frame_no;
}

/* the frame's copies into VRAM, queued before its list */
static void staging_send(void) {
    int i;
    PROF_START(t0);
    tex_flush();
    for (i = 0; i < nstaged; i++) {
        u32 dim = GX_BUFFER_DIM(staged[i].size / 16, 0);
        if (staged[i].dim != 0) {
            GX_DisplayTransfer(staged[i].src, staged[i].dim, staged[i].dst, staged[i].dim, staged[i].flags);
            continue;
        }
        GX_TextureCopy(staged[i].src, dim, staged[i].dst, dim, staged[i].size, GX_TRANSFER_RAW_COPY(1));
    }
    PROF_COUNT(PZ_TEX_STAGE, (uint32_t)nstaged);
    nstaged = 0;
    PROF_STOP(PZ_TEX_STAGE, t0);
}

/* VRAM for g, the least recently drawn there freed until it fits; 0 when it can't */
static int vram_new(GTEX* g, int w, int h, int levels, GPU_TEXCOLOR f, u32 size) {
    if (vramSpaceFree() < size + vram_keep && take_same(g, w, h, levels, f, 1)) {
        return 1;
    }
    for (;;) {
        if (vramSpaceFree() >= size + vram_keep &&
            C3D_TexInitWithParams(&g->tex, NULL, (C3D_TexInitParams){ (u16)w, (u16)h, (u8)levels, f, GPU_TEX_2D, true })) {
            return 1;
        }
        if (!free_oldest(1)) {
            return 0;
        }
        vram_freed++;
    }
}

/* VRAM's textures dropped, made again when next drawn */
static void vram_drop_all(void) {
    int i = lru_next(LRU_END), next;
    for (; i != LRU_END; i = next) {
        next = lru_next(i);
        if (in_vram(gtex[i].tex.data) && !(busy(gtex[i].used) && retired != NULL && nretired == MAX_TEX)) {
            tex_drop(&gtex[i]);
        }
    }
}

/* one of 128x128 texels or more left in linear memory, copied into VRAM by the GPU as it is drawn again, when VRAM
   has room and the frame a copy to spare; its old memory goes once the GPU is done with it. MVC2_TEX_UNPARK: the least
   recently drawn in VRAM freed until it fits, as for a new one, since VRAM's free room is often in pieces */
static void vram_move(GTEX* g) {
    C3D_Tex t;
    void* data;
    u32 size = tex_bytes(&g->tex);
    if (retired == NULL || nretired == MAX_TEX || nstaged == STAGE_COPIES || (nstaged != 0 && staged_frame != frame_no) ||
        size / 16 > 0xFFFF) {
        return;
    }
    while (vramSpaceFree() < size + vram_keep ||
           !C3D_TexInitWithParams(&t, NULL, (C3D_TexInitParams){ (u16)g->width, (u16)g->height, (u8)g->levels, g->format, GPU_TEX_2D, true })) {
        if (!tex_unpark || !free_oldest(1)) {
            return;
        }
        unpark_freed++;
    }
    data = t.data;
    t = g->tex;
    g->tex.data = data;
    tex_changed(g);
    staging_add(t.data, data, size);
    tex_free(&t, frame_no);
    vram_moved++;
}

/* at_exit: after the teardown, all of them dropped, only the counts left */
static void vram_log(int at_exit) {
    u32 bytes = 0, n = 0, left = 0;
    int i;
    for (i = lru_next(LRU_END); i != LRU_END; i = lru_next(i)) {
        if (in_vram(gtex[i].tex.data)) {
            bytes += tex_bytes(&gtex[i].tex);
            n++;
        } else if (gtex[i].width * gtex[i].height >= 128 * 128) {
            left += tex_bytes(&gtex[i].tex);
        }
    }
    host_log("textures in VRAM: %s%lu, %lu KB in %lu (%lu KB of VRAM free), %lu KB of 128x128 or more left in linear "
             "memory; %lu made there, %lu moved there, %lu made in linear memory for want of VRAM and %lu of the staging "
             "ring, %lu freed for room",
             at_exit ? "at exit after frame " : "frame ", (unsigned long)frame_no, (unsigned long)(bytes >> 10),
             (unsigned long)n, (unsigned long)(vramSpaceFree() >> 10),
             (unsigned long)(left >> 10), (unsigned long)vram_textures, (unsigned long)vram_moved,
             (unsigned long)vram_no_room, (unsigned long)vram_no_stage, (unsigned long)vram_freed);
    if (tex_unpark) {
        host_log("textures: %lu freed in VRAM to move one there (MVC2_TEX_UNPARK)", (unsigned long)unpark_freed);
    }
}

static GTEX* find_tex(const RD_TEXTURE* t) {
    uint32_t h = (uint32_t)(uintptr_t)t * 2654435761u >> 20, i;
    GTEX* free_slot = NULL;
    for (i = 0; i < MAX_TEX; i++) {
        GTEX* g = &gtex[(h + i) & (MAX_TEX - 1)];
        if (g->key == t) {
            return g;
        }
        if (g->key == NULL) {
            return free_slot != NULL ? free_slot : g;
        }
        if (!g->ok && free_slot == NULL) {
            free_slot = g;
        }
    }
    return free_slot;
}

/* Small textures' earlier texels, kept converted by serial: a sprite tile that comes back to an earlier cell
   swaps its texture with the kept one instead of converting again. MVC2_SMOOTH_2D's padded ones are kept at any size,
   under their own key: an effect's animation comes back to its frames */
#define KEPT_WAYS 4
#define KEPT_SETS 64
#define KEPT_MAX_TEXELS 1024

typedef struct {
    uint32_t serial, used;
    uint32_t drawn;                 /* the frame its memory was last drawn in */
    int width, height;
    GPU_TEXCOLOR format;
    int ok, pad;
    C3D_Tex tex;
} KEPT;

static KEPT kept[KEPT_SETS][KEPT_WAYS];
static uint32_t kept_clock;

static int keepable(const GTEX* g) {
    return g->levels == 0 && (g->width * g->height <= KEPT_MAX_TEXELS || g->pad) && !in_vram(g->tex.data);
}

static void swap_tex(C3D_Tex* a, C3D_Tex* b) {
    C3D_Tex c = *a;
    *a = *b;
    *b = c;
}

/* g's texture for serial when kept, its old texels kept in its place */
static int kept_swap(GTEX* g, uint32_t serial) {
    KEPT* set = kept[serial % KEPT_SETS];
    int i;
    for (i = 0; i < KEPT_WAYS; i++) {
        KEPT* k = &set[i];
        if (k->ok && k->serial == serial && k->pad == g->pad && k->width == g->width && k->height == g->height &&
            k->format == g->format) {
            swap_tex(&g->tex, &k->tex);
            k->serial = g->serial;
            k->used = ++kept_clock;
            k->drawn = g->used;
            return 1;
        }
    }
    return 0;
}

/* g's texels kept under its serial before they are converted over; g takes the memory of the one they replace,
   or a new texture. 0 when there is no memory for that, or the GPU may read all the set's */
static int kept_add(GTEX* g) {
    KEPT* set = kept[g->serial % KEPT_SETS];
    KEPT* k = NULL;
    C3D_Tex fresh;
    int i;
    for (i = 0; i < KEPT_WAYS; i++) {
        if (!set[i].ok) {
            k = &set[i];
            break;
        }
        if (!busy(set[i].drawn) && (k == NULL || set[i].used < k->used)) {
            k = &set[i];
        }
    }
    if (k == NULL) {
        return 0;
    }
    if (k->ok && (k->width != g->width || k->height != g->height || k->format != g->format)) {
        C3D_TexDelete(&k->tex);
        k->ok = 0;
    }
    if (!k->ok) {
        if (linearSpaceFree() < (u32)(g->width * g->height * 2) + LINEAR_KEEP ||
            !C3D_TexInitWithParams(&fresh, NULL, (C3D_TexInitParams){ (u16)g->width, (u16)g->height, 0, g->format, GPU_TEX_2D, false })) {
            return 0;
        }
        k->tex = fresh;
        k->ok = 1;
    }
    swap_tex(&g->tex, &k->tex);
    C3D_TexSetWrap(&g->tex, GPU_REPEAT, GPU_REPEAT);
    C3D_TexSetFilterMipmap(&g->tex, GPU_NEAREST);
    k->serial = g->serial;
    k->pad = g->pad;
    k->width = g->width;
    k->height = g->height;
    k->format = g->format;
    k->used = ++kept_clock;
    k->drawn = g->used;
    return 1;
}

/* the kept textures' memory back, for others */
static int kept_drop_all(void) {
    int i, j, n = 0;
    for (i = 0; i < KEPT_SETS; i++) {
        for (j = 0; j < KEPT_WAYS; j++) {
            if (kept[i][j].ok && !busy(kept[i][j].drawn)) {
                C3D_TexDelete(&kept[i][j].tex);
                kept[i][j].ok = 0;
                n++;
            }
        }
    }
    return n;
}

/* A texture larger than its texels: the tiles convert doesn't fill, cleared before it fills the rest. A tile row's
   tiles are in a run, and the rows below the texels' last whole one are one run. MVC2_TEX_CLEAR=all clears it all */
static int clear_all;

static void clear_outside(GTEX* g, int tw, int th) {
    uint8_t* d = g->tex.data;
    u32 tile = g->tex.size / ((u32)g->width * g->height / 64), row = tile * (u32)(g->width / 8);
    int y, fx = tw / 8, fy = th / 8;
    if (clear_all) {
        memset(d, 0, g->tex.size);
        return;
    }
    for (y = 0; y < fy; y++) {
        memset(d + y * row + fx * tile, 0, row - fx * tile);
    }
    memset(d + fy * row, 0, g->tex.size - fy * row);
}

/* the texels of one that keeps only its source, made as it is converted */
static void* unpack_buf;
static uint32_t unpack_size;

static int unpack_room(uint32_t bytes) {
    if (bytes > unpack_size) {
        free(unpack_buf);
        unpack_size = 0;
        if ((unpack_buf = host_try_malloc(bytes)) == NULL) {
            return 0;
        }
        unpack_size = bytes;
    }
    return 1;
}

/* MVC2_TEX_FROM_SERIAL=1: a texture to be converted copied instead from another holding the same serial's texels,
   made alike: the game puts a sprite tile in another cell of its cache from one frame to the next */
static int from_serial;
static int16_t serial_slot[1024];       /* the texture last made for a serial, by its hash */
static uint32_t serial_copies;

static int16_t* serial_at(uint32_t serial) {
    return &serial_slot[(serial * 2654435761u) >> 22];
}

/* another texture with t's serial, made as g is and in linear memory; NULL if none */
static const GTEX* same_serial(const GTEX* g, const RD_TEXTURE* t) {
    const GTEX* o = &gtex[*serial_at(t->serial)];
    if (o == g || !o->ok || o->serial != t->serial || o->tw != t->width || o->th != t->height ||
        o->tformat != t->format || o->tlevels != t->levels || o->width != g->width || o->height != g->height ||
        o->format != g->format || o->levels != g->levels || o->pad != g->pad || in_vram(o->tex.data)) {
        return NULL;
    }
    return o;
}

/* converted again if its texels changed; NULL if it can't be */
/* what MVC2_TEX_CURRENT reads back: g made for t as it is now */
static GTEX* made_for(GTEX* g, const RD_TEXTURE* t) {
    if (from_serial) {
        *serial_at(t->serial) = (int16_t)(g - gtex);
    }
    g->tw = t->width;
    g->th = t->height;
    g->tformat = t->format;
    g->tlevels = t->levels;
    g->su = (float)t->width / (float)g->width;
    g->sv = (float)t->height / (float)g->height;
    g->copied = 0;
    tex_changed(g);
    return g;
}

static int tex_ahead;                       /* MVC2_TEX_AHEAD */
static uint32_t ahead_made, ahead_drawn;

/* a texture made and unchanged, drawn: MVC2_TEX_VRAM's left in linear memory moved as they are first drawn in a frame */
static void tex_drawn(GTEX* g) {
    if (g->ahead) {
        g->ahead = 0;
        ahead_drawn++;
    }
    if (vram_moves && g->used != frame_no && g->width * g->height >= 128 * 128 && !in_vram(g->tex.data)) {
        vram_move(g);
    }
    tex_used(g);
}

/* MVC2_TEX_CURRENT: gpu_tex's answer for a texture made for t as it is now, which gpu_tex would only mark used;
   NULL when gpu_tex must look */
static GTEX* tex_current_at(GTEX* g, const RD_TEXTURE* t, int pad) {
    if (g == NULL || g->key != t || !g->ok || g->serial != t->serial || t->texels == NULL || t->width != g->tw ||
        t->height != g->th || t->format != g->tformat || t->levels != g->tlevels) {
        return NULL;
    }
    /* gpu_tex's choice of padding, for a texture made and unchanged */
    if (!pad) {
        g->unpadded = 1;
    } else if (g->unpadded || (!g->pad && frame_no - g->changed < PAD_STILL)) {
        pad = 0;
    }
    if (g->pad != pad) {
        return NULL;
    }
    tex_drawn(g);
    return g;
}

/* MVC2_TEX_GX=1: a texture of 64x64 texels or more, one level, its size the GPU's, made in VRAM: its texels written
   in rows to the staging ring, the alpha moved, and tiled by the GPU's display transfer as it copies them there. The
   transfer is tested against the tiling here at start, and not used if it differs */
static int tex_gx, gx_flip;
static uint32_t gx_textures;

static int gx_tiles(const RD_TEXTURE* t, const GTEX* g) {
    return tex_gx && !g->pad && g->levels == 0 && t->unpack == NULL && g->format != GPU_L8 && t->width == g->width &&
           t->height == g->height && g->width >= 64 && g->height >= 64;
}

static u32 gx_flags(GPU_TEXCOLOR f) {
    u32 x = f == GPU_RGB565 ? GX_TRANSFER_FMT_RGB565 : f == GPU_RGBA4 ? GX_TRANSFER_FMT_RGBA4 : GX_TRANSFER_FMT_RGB5A1;
    return GX_TRANSFER_FLIP_VERT(gx_flip) | GX_TRANSFER_OUT_TILED(1) | GX_TRANSFER_RAW_COPY(0) |
           GX_TRANSFER_IN_FORMAT(x) | GX_TRANSFER_OUT_FORMAT(x) | GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO);
}

/* t's texels in rows, as the GPU's formats have them */
static void gx_linear(const RD_TEXTURE* t, void* data) {
    const uint8_t* s = t->texels;
    uint32_t* d = data;
    u32 i, n = (u32)(t->width * t->height) / 2;
    if (t->format == RD_RGB565) {
        memcpy(d, s, n * 4);
    } else if (t->format == RD_ARGB4444) {
        for (i = 0; i < n; i++) {
            d[i] = rot16(load32(s + i * 4), 4);
        }
    } else {
        for (i = 0; i < n; i++) {
            d[i] = rot16(load32(s + i * 4), 1);
        }
    }
    gx_textures++;
}

/* the display transfer's tiling of a 128x64 pattern in each format against the CPU's, each way up; the way that
   matches in all three, else -1 */
static int gx_test(void) {
    enum { W = 128, H = 64 };
    static const GPU_TEXCOLOR fmts[3] = { GPU_RGB565, GPU_RGBA5551, GPU_RGBA4 };
    uint16_t *lin = linearAlloc(W * H * 2), *ref = malloc(W * H * 2), *out = vramAlloc(W * H * 2);
    uint32_t seed = 0x2545F491u;
    int flip, k, i, ok = -1;
    if (lin != NULL && ref != NULL && out != NULL) {
        for (i = 0; i < W * H; i++) {
            seed = seed * 1664525u + 1013904223u;
            lin[i] = (uint16_t)(seed >> 16);
        }
        level16(lin, W, H, ref, W / 8, 0);
        GSPGPU_FlushDataCache(lin, W * H * 2);
        for (flip = 0; flip < 2 && ok < 0; flip++) {
            gx_flip = flip;
            for (k = 0; k < 3; k++) {
                memset(out, 0, W * H * 2);
                C3D_SyncDisplayTransfer((u32*)lin, GX_BUFFER_DIM(W, H), (u32*)out, GX_BUFFER_DIM(W, H), gx_flags(fmts[k]));
                GSPGPU_InvalidateDataCache(out, W * H * 2);
                if (memcmp(out, ref, W * H * 2) != 0) {
                    break;
                }
            }
            if (k == 3) {
                ok = flip;
            }
        }
    }
    if (lin != NULL) {
        linearFree(lin);
    }
    free(ref);
    if (out != NULL) {
        vramFree(out);
    }
    return ok;
}

static GTEX* gpu_tex_at(GTEX* g, const RD_TEXTURE* t, int pad) {
    int w, h, levels, moved = 0, want = pad, gx_tiled = 0;
    void* vdata = NULL;
    u32 vsize = 0;
    GPU_TEXCOLOR f;
    if (g == NULL || t->texels == NULL || t->width <= 0 || t->height <= 0 || t->width > 1024 || t->height > 1024) {
        return NULL;
    }
    w = pow2(t->width);
    h = pow2(t->height);
    f = gpu_format(t->format);
    levels = gpu_levels(t, w, h);
    if (g->key == t && g->serial == t->serial && !pad) {
        g->unpadded = 1;
    }
    /* not padded while it changes, or when it is drawn unpadded too: the two would convert it by turns */
    if (pad && g->key == t && g->ok &&
        (g->serial == t->serial ? g->unpadded || (!g->pad && frame_no - g->changed < PAD_STILL)
                                : frame_no - g->changed < PAD_STILL)) {
        pad = 0;
    }
    if (g->key == t && g->ok && g->serial == t->serial && g->width == w && g->height == h && g->format == f &&
        g->levels == levels && g->pad == pad) {
        tex_drawn(g);
        return g;
    }
    if (t->unpack != NULL && (!vq_tiles || pad) && !unpack_room(t->unpacked)) {
        return NULL;
    }
    if (g->ok && (g->key != t || g->width != w || g->height != h || g->format != f || g->levels != levels ||
                  g->pad != pad)) {
        tex_drop(g);
    }
    if (g->key != t || g->serial != t->serial) {
        g->changed = frame_no;
        g->unpadded = !want;
    }
    g->key = t;
    if (g->ok && keepable(g)) {
        if (kept_swap(g, t->serial)) {
            g->serial = t->serial;
            tex_used(g);
            ctr_render_count[CTR_COUNT_KEPT]++;
            return made_for(g, t);
        }
        moved = kept_add(g);
    }
    if (g->ok && !moved && busy(g->used)) {
        tex_drop(g);    /* the frame in flight may read it: new memory */
    }
    if (g->ok && tex_vram && !vram_direct && in_vram(g->tex.data) && !staging_room(tex_bytes(&g->tex))) {
        tex_drop(g);    /* no room in the staging ring this frame: linear memory */
    }
    if (!g->ok) {
        u32 need = (u32)(w * h * 2 * (levels > 0 ? 4 : 3) / 3) + LINEAR_KEEP;     /* all of the linear heap but LINEAR_KEEP */
        int vram = 0;
        PROF_START(ta);
        if (tex_vram && w * h >= 128 * 128) {
            u32 size = C3D_TexCalcTotalSize((u32)(w * h * (f == GPU_L8 ? 1 : 2)), levels);
            if (!vram_direct && !staging_room(size)) {
                vram_no_stage++;
            } else if (!(vram = vram_new(g, w, h, levels, f, size))) {
                vram_no_room++;
            } else {
                vram_textures++;
            }
        }
        if (!vram && (linearSpaceFree() >= need || !take_same(g, w, h, levels, f, 0))) {
            while (linearSpaceFree() < need && free_oldest(0)) {
            }
            while (!C3D_TexInitWithParams(&g->tex, NULL, (C3D_TexInitParams){ (u16)w, (u16)h, (u8)levels, f, GPU_TEX_2D, false })) {
                if (!free_oldest(0) && !kept_drop_all()) {
                    PROF_STOP(PZ_TEX_ALLOC, ta);
                    early_short = 1;
                    return NULL;
                }
            }
        }
        g->ok = 1;
        g->used = frame_no;
        lru_append((int)(g - gtex));
        g->width = w;
        g->height = h;
        g->levels = levels;
        g->format = f;
        g->pad = pad;
        C3D_TexSetWrap(&g->tex, GPU_REPEAT, GPU_REPEAT);
        C3D_TexSetFilterMipmap(&g->tex, GPU_NEAREST);     /* as the PowerVR2 */
        if ((!vram || vram_direct) && (w != t->width || h != t->height)) {
            clear_outside(g, t->width, t->height);
        }
        PROF_STOP(PZ_TEX_ALLOC, ta);
    }
    if (tex_vram && !vram_direct && in_vram(g->tex.data)) {
        /* converted into the staging ring, all of it written there */
        vdata = g->tex.data;
        vsize = tex_bytes(&g->tex);
        g->tex.data = staging_take(vsize);
        if (w != t->width || h != t->height) {
            clear_outside(g, t->width, t->height);
        }
    }
    {
#ifdef MVC2_PROF
        const int timed = 1;
#else
        const int timed = !tex_lean;
#endif
        uint64_t t0 = timed ? svcGetSystemTick() : 0, dt;
        RD_TEXTURE u;
        const GTEX* o = from_serial ? same_serial(g, t) : NULL;
        PFZ_START(q0);
        if (o == NULL && vdata != NULL && gx_tiles(t, g)) {
            gx_linear(t, g->tex.data);
            gx_tiled = 1;
        } else if (o != NULL) {
            memcpy(g->tex.data, o->tex.data, tex_bytes(&g->tex));
            tex_written(g->tex.data, tex_bytes(&g->tex));
            serial_copies++;
        } else if (t->unpack != NULL && (!vq_tiles || pad)) {
            u = *t;
            u.texels = unpack_buf;
            t->unpack(t, &u);
            convert(&u, g);
        } else {
            convert(t, g);
        }
        PFZ_STOP(PFZ_TEX, q0);
        dt = timed ? svcGetSystemTick() - t0 : 0;
        ctr_render_ticks[CTR_TICKS_TEX] += dt;
        PROF_ADD(PZ_TEXTURES, dt);
        PROF_COUNT(PZ_TEXTURES, (uint32_t)(t->width * t->height));
        if (t->width * t->height >= 128 * 128) {
            PROF_ADD(PZ_TEX_BIG, dt);
            PROF_COUNT(PZ_TEX_BIG, (uint32_t)(t->width * t->height));
        }
        ctr_render_count[CTR_COUNT_MADE]++;
        ctr_render_count[CTR_COUNT_TEXELS] += (uint32_t)(t->width * t->height);
    }
    if (vdata != NULL) {
        staging_add(g->tex.data, vdata, vsize);
        if (gx_tiled) {
            staged[nstaged - 1].dim = GX_BUFFER_DIM(g->width, g->height);
            staged[nstaged - 1].flags = gx_flags(g->format);
        }
        g->tex.data = vdata;
    }
    g->ahead = 0;
    g->serial = t->serial;
    tex_used(g);
    return made_for(g, t);
}

/* palettes in the lighting's lookup tables */

static uint32_t lut12(uint32_t v) {
    return v << 4 | v >> 4;     /* 0..255 to 0..4095 */
}

/* texel c is entry base + (c ^ 0x80) */
static uint32_t lut_palettes;       /* lut_palette's calls */

static void lut_palette(int base, int n) {
    static const GPU_LIGHTLUTID ids[4] = { GPU_LUT_RR, GPU_LUT_RG, GPU_LUT_RB, GPU_LUT_FR };
    uint32_t data[4][256];
    int i, k;
    lut_palettes++;
    for (k = 0; k < n; k++) {
        uint32_t c = rf->palette[(base + k) & 1023];
        data[0][k] = lut12(c >> 16 & 255);
        data[1][k] = lut12(c >> 8 & 255);
        data[2][k] = lut12(c & 255);
        data[3][k] = lut12(c >> 24);
    }
    for (i = 0; i < 4; i++) {
        if (n == 256) {
            GPUCMD_AddWrite(GPUREG_LIGHTING_LUT_INDEX, (u32)ids[i] << 8);
            GPUCMD_AddWrites(GPUREG_LIGHTING_LUT_DATA0, data[i] + 128, 128);
            GPUCMD_AddWrite(GPUREG_LIGHTING_LUT_INDEX, (u32)ids[i] << 8 | 0x80);
            GPUCMD_AddWrites(GPUREG_LIGHTING_LUT_DATA0, data[i], 128);
        } else {
            GPUCMD_AddWrite(GPUREG_LIGHTING_LUT_INDEX, (u32)ids[i] << 8 | 0x80);
            GPUCMD_AddWrites(GPUREG_LIGHTING_LUT_DATA0, data[i], (u32)n);
        }
    }
}

static void light_setup(void) {
    static const C3D_Material mtl = {
        { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 }, { 0, 0, 0 },
    };
    static float zero[512];
    C3D_FVec pos = FVec4_New(1.0f, 0.0f, 0.0f, 0.0f);   /* L.N is the normal's x */
    LightLut_FromArray(&env_lut, zero);
    C3D_LightEnvInit(&env);
    C3D_LightEnvMaterial(&env, &mtl);
    C3D_LightEnvAmbient(&env, 0, 0, 0);
    C3D_LightEnvLut(&env, GPU_LUT_D0, GPU_LUTINPUT_NH, false, NULL);
    C3D_LightEnvLut(&env, GPU_LUT_D1, GPU_LUTINPUT_NH, false, NULL);
    C3D_LightEnvLut(&env, GPU_LUT_FR, GPU_LUTINPUT_LN, true, &env_lut);
    C3D_LightEnvLut(&env, GPU_LUT_RR, GPU_LUTINPUT_LN, true, &env_lut);
    C3D_LightEnvLut(&env, GPU_LUT_RG, GPU_LUTINPUT_LN, true, &env_lut);
    C3D_LightEnvLut(&env, GPU_LUT_RB, GPU_LUTINPUT_LN, true, &env_lut);
    C3D_LightEnvFresnel(&env, GPU_SEC_ALPHA_FRESNEL);
    C3D_LightEnvBumpMode(&env, GPU_BUMP_AS_BUMP);
    C3D_LightEnvBumpSel(&env, 0);
    C3D_LightEnvBumpNormalZ(&env, false);
    C3D_LightEnvClampHighlights(&env, false);
    C3D_LightInit(&light, &env);
    C3D_LightColor(&light, 0, 0, 0);
    C3D_LightSpecular0(&light, 0, 0, 0);
    C3D_LightSpecular1(&light, 1, 1, 1);
    C3D_LightPosition(&light, &pos);
    C3D_LightShadowEnable(&light, false);
    C3D_LightSpotEnable(&light, false);
    C3D_LightDistAttnEnable(&light, false);
    C3D_LightGeoFactor(&light, 0, false);
    C3D_LightGeoFactor(&light, 1, false);
    /* layer configuration 5, for the Fresnel table (the alpha) */
    env.conf.config[0] = (env.conf.config[0] & ~(0xFu << 4)) | (GPU_LIGHT_ENV_LAYER_CONFIG(5) << 4);
    env.flags |= C3DF_LightEnv_Dirty;
}

/* Models the GPU turns and lights (rd_gpu_tl): their meshes in a pool of linear memory, made and freed on the
   game's thread, whose memory goes back to the pool once no frame that may draw it is left */

static u32 mesh_pool_size = 512u << 10;    /* MVC2_MESH_POOL, in KB */
#define MAX_MESHES 1024
#define MESH_HOLES (MAX_MESHES + 1)        /* holes lie between meshes, so there are never more */

/* triangles drawn with one culling */
typedef struct {
    uint16_t first, count;
    uint8_t cull;           /* GPU_CULLMODE */
} MRUN;

typedef struct {
    u32 at, size;           /* in the pool */
    int kind, nruns;
    const void* v;
    const uint16_t* idx;
    const MRUN* run;
    float nscale;           /* the normals were made over this */
    uint8_t live;
    uint32_t freed;         /* not 0: the last frame that may draw it, after which it goes back */
    float vmax[3];          /* its largest x, y and z, unsigned */
    u32 v_at, idx_at;       /* its vertices and indices from the GPU's buffer base */
    int nv, ntri;           /* its vertices, and the triangles its runs draw */
    const u32* frag;        /* the GPU's words for its draws: its buffer, then each run's culling and draw */
    u32 frag_words;         /* those, without the jump back after them */
    u32 frag_at;            /* their physical address */
} GMESH;

/* group_corner's answer kept for a mesh with the same matrix and texture size, as the HUD's mostly are from frame to
   frame; by the mesh's id + 1, forgotten when it goes and again when the id is used again (with MVC2_LISTS_CORE1,
   core 1 may keep one for it after it goes) */
typedef struct {
    float m[16];
    int16_t mesh;
    uint16_t tw, th;
    uint8_t yes;
} CORNER_MEMO;
static CORNER_MEMO corner_memo[256];
static CORNER_MEMO corner_memo2[256];
static struct {
    int16_t mesh;
    uint16_t tw, th;
    float at;
} corner_at[MAX_MESHES];            /* with MVC2_PREP_FAST2, the mesh's texels' area in the corner test */  /* with MVC2_PREP_FAST, the one before it, for a mesh drawn in two places */

static u32* mesh_frag(u32* w, const GMESH* g);
static u32* buf_setup(u32* w, const void* v, int stride);
static int raw_bufs;                /* MVC2_RAW_BUFS=1: a mesh's draws after its whole buffer block, as citro3d binds it */
static int frag_call;
static int frag_inline;             /* MVC2_FRAG_INLINE=1: those words written again from the mesh, not copied */

static uint8_t* mesh_pool;
static RANGE holes[MESH_HOLES];     /* the pool's free parts, by place */
static int nholes;
static GMESH* gmesh;                /* MAX_MESHES of them, made with the pool */
static uint8_t mesh_held[MAX_MESHES];   /* gmesh[i].size != 0, for the first pass over the groups */
static uint16_t* mesh_gone;         /* those freed, waiting for the GPU */
static int ngone, mesh_next;
#define MESH_STRIDE 24              /* x, y, z, u, v; then a colour, or a normal's x, y, z as bytes from -1 to 1 */
#define MESH_FRAG_WORDS(runs) ((raw_bufs ? 44 : 4) + 32 * (runs))   /* with the jump back: a multiple of 16 bytes */

static int pool_take(u32 size, u32* at) {
    int i;
    for (i = 0; i < nholes; i++) {
        if (holes[i].hi - holes[i].lo >= size) {
            *at = holes[i].lo;
            holes[i].lo += size;
            if (holes[i].lo == holes[i].hi) {
                memmove(&holes[i], &holes[i + 1], (size_t)(nholes - i - 1) * sizeof *holes);
                nholes--;
            }
            return 1;
        }
    }
    return 0;
}

static void pool_give(u32 lo, u32 hi) {
    int i = 0;
    while (i < nholes && holes[i].lo < lo) {
        i++;
    }
    if (i > 0 && holes[i - 1].hi == lo) {
        holes[i - 1].hi = hi;
        if (i < nholes && holes[i].lo == hi) {
            holes[i - 1].hi = holes[i].hi;
            memmove(&holes[i], &holes[i + 1], (size_t)(nholes - i - 1) * sizeof *holes);
            nholes--;
        }
    } else if (i < nholes && holes[i].lo == hi) {
        holes[i].lo = lo;
    } else if (nholes < MESH_HOLES) {
        memmove(&holes[i + 1], &holes[i], (size_t)(nholes - i) * sizeof *holes);
        holes[i].lo = lo;
        holes[i].hi = hi;
        nholes++;
    }
}

/* the freed meshes no frame left may draw */
static void mesh_release(void) {
    int i, n = 0;
    for (i = 0; i < ngone; i++) {
        GMESH* g = &gmesh[mesh_gone[i]];
        if (busy(g->freed)) {
            mesh_gone[n++] = mesh_gone[i];
        } else {
            pool_give(g->at, g->at + g->size);
            g->freed = 0;
            g->size = 0;
            mesh_held[mesh_gone[i]] = 0;
        }
    }
    ngone = n;
}

static uint8_t gpu_cull(int cull) {
    /* the target's turn flips the game's y: a triangle of positive area on the game's screen is clockwise */
    return cull == 2 ? GPU_CULL_FRONT_CCW : cull == 3 ? GPU_CULL_BACK_CCW : GPU_CULL_NONE;
}

static u32 align16(u32 n) {
    return (n + 15) & ~15u;
}

/* -1 to 1 in 255 steps */
static uint8_t normal_byte(float n) {
    float b = n * 127.5f + 128.0f;
    return (uint8_t)(b < 0.0f ? 0 : b > 255.0f ? 255 : (int)b);
}

int rd_gpu_mesh_make(const NL_MESH* m) {
    u32 vbytes, ibytes, at, size;
    float nscale = 1.0f, vmax[3] = { 0.0f, 0.0f, 0.0f };
    int id, i, nruns = 0, nr = 0;
    uint8_t* p;
    uint16_t* idx;
    MRUN* run;
    if (!rd_gpu_tl || m->kind < RD_GPU_UNLIT || m->kind > RD_GPU_LIT || m->nv <= 0 || m->nv > 65536 || m->ntri <= 0 ||
        m->ntri > 65535 || m->nspan <= 0) {
        return -1;
    }
    for (i = 0; i < m->ntri * 3; i++) {
        if (m->idx[i] >= m->nv) {
            return -1;
        }
    }
    for (i = 0; i < m->nspan; i++) {
        if (m->span[i].first + m->span[i].count > m->ntri) {
            return -1;
        }
    }
    {
        /* the runs, as made below */
        int last = -1, end = -1;
        for (i = 0; i < m->nspan; i++) {
            const NL_MSPAN* s = &m->span[i];
            int cull = gpu_cull(s->cull);
            if (s->count != 0) {
                nr += cull != last || s->first != end;
                last = cull;
                end = s->first + s->count;
            }
        }
    }
    mesh_release();
    for (i = 0; i < MAX_MESHES && (gmesh[mesh_next].live || gmesh[mesh_next].size != 0); i++) {
        mesh_next = (mesh_next + 1) % MAX_MESHES;
    }
    vbytes = align16((u32)(m->nv * MESH_STRIDE));
    ibytes = align16((u32)m->ntri * 6);
    size = vbytes + ibytes + align16((u32)nr * sizeof(MRUN)) + align16((u32)MESH_FRAG_WORDS(nr) * 4);
    if (i == MAX_MESHES || !pool_take(size, &at)) {
        ctr_render_count[CTR_COUNT_MESH_FULL]++;
        return -1;
    }
    ctr_render_count[CTR_COUNT_MESHES]++;
    ctr_render_count[CTR_COUNT_MESH_BYTES] += size;
    id = mesh_next;
    if (corner_memo[id & 255].mesh == id + 1) {
        corner_memo[id & 255].mesh = 0;
    }
    if (corner_memo2[id & 255].mesh == id + 1) {
        corner_memo2[id & 255].mesh = 0;
    }
    corner_at[id].mesh = 0;
    p = mesh_pool + at;
    if (m->kind == RD_GPU_LIT) {
        for (i = 0; i < m->nv; i++) {
            nscale = max_f(nscale, max_f(fabsf(m->v[i].nx), max_f(fabsf(m->v[i].ny), fabsf(m->v[i].nz))));
        }
    }
    for (i = 0; i < m->nv; i++) {
        const NL_MVTX* s = &m->v[i];
        float* d = (float*)(p + i * MESH_STRIDE);
        vmax[0] = max_f(vmax[0], fabsf(s->x));
        vmax[1] = max_f(vmax[1], fabsf(s->y));
        vmax[2] = max_f(vmax[2], fabsf(s->z));
        uint8_t* c = (uint8_t*)&d[5];
        d[0] = s->x;
        d[1] = s->y;
        d[2] = s->z;
        d[3] = s->u;
        d[4] = s->v;
        if (m->kind == RD_GPU_VTXCOL) {
            memcpy(c, &s->argb, 4);
        } else {
            c[0] = normal_byte(s->nx / nscale);
            c[1] = normal_byte(s->ny / nscale);
            c[2] = normal_byte(s->nz / nscale);
            c[3] = 0;
        }
    }
    idx = (uint16_t*)(p + vbytes);
    memcpy(idx, m->idx, (size_t)m->ntri * 6);
    run = (MRUN*)(p + vbytes + ibytes);
    for (i = 0; i < m->nspan; i++) {
        const NL_MSPAN* s = &m->span[i];
        uint8_t cull = gpu_cull(s->cull);
        if (s->count == 0) {
            continue;
        }
        if (nruns > 0 && run[nruns - 1].cull == cull && run[nruns - 1].first + run[nruns - 1].count == s->first) {
            run[nruns - 1].count += s->count;
        } else {
            run[nruns].first = s->first;
            run[nruns].count = s->count;
            run[nruns].cull = cull;
            nruns++;
        }
    }
    flush_one(p, vbytes + ibytes);
    {
        GMESH* g = &gmesh[id];
        u32* frag = (u32*)(p + vbytes + ibytes + align16((u32)nr * sizeof(MRUN)));
        int ntri = 0;
        for (i = 0; i < nruns; i++) {
            ntri += run[i].count;
        }
        *g = (GMESH){ at, size, m->kind, nruns, p, idx, run, nscale, 1, 0, { vmax[0], vmax[1], vmax[2] },
                      osConvertVirtToPhys(p) - buf_base, osConvertVirtToPhys(idx) - buf_base, m->nv, ntri, frag, 0,
                      osConvertVirtToPhys(frag) };
        g->frag_words = (u32)(mesh_frag(frag, g) - frag);
        mesh_held[id] = 1;
        if (frag_call) {
            frag[g->frag_words] = 1;     /* back to the list, where its call said */
            frag[g->frag_words + 1] = GPUCMD_HEADER(0, 0xF, GPUREG_CMDBUF_JUMP0);
            flush_one(frag, (g->frag_words + 2) * 4);
        }
    }
    return id;
}

void rd_gpu_mesh_free(int id) {
    if (gmesh == NULL || id < 0 || id >= MAX_MESHES || !gmesh[id].live) {
        return;
    }
    gmesh[id].live = 0;
    gmesh[id].freed = frame_no + 1;     /* the frame being made */
    if (corner_memo[id & 255].mesh == id + 1) {
        corner_memo[id & 255].mesh = 0;
    }
    if (corner_memo2[id & 255].mesh == id + 1) {
        corner_memo2[id & 255].mesh = 0;
    }
    corner_at[id].mesh = 0;
    mesh_gone[ngone++] = (uint16_t)id;
}

void ctr_render_meshes(uint32_t* size, uint32_t* used) {
    *size = mesh_pool != NULL ? mesh_pool_size : 0;
    *used = mesh_pool != NULL ? mesh_pool_size - rd_gpu_mesh_room() : 0;
}

uint32_t rd_gpu_mesh_room(void) {
    u32 n = 0;
    int i;
    mesh_release();
    for (i = 0; i < nholes; i++) {
        n += holes[i].hi - holes[i].lo;
    }
    return n;
}

static uint32_t zbits(float z) {
    uint32_t b;
    memcpy(&b, &z, 4);
    return b;       /* a positive float's bits sort as it does */
}

static float zfloat(uint32_t b) {
    float z;
    memcpy(&z, &b, 4);
    return z;
}

/* a stable radix sort of n keys, with their values when val isn't NULL, 8 bits at a time: the four
   bytes counted in one pass, a byte all keys share skipped, the arrays swapped rather than copied */
static void radix_sort_pairs(uint32_t* key, int* val, uint32_t* ktmp, int* vtmp, int n) {
    static int count[4][256];
    uint32_t *k = key, *kt = ktmp, *ks;
    int *v = val, *vt = vtmp, *vs, i, d;
    if (n == 0) {
        return;
    }
    memset(count, 0, sizeof count);
    for (i = 0; i < n; i++) {
        uint32_t x = key[i];
        count[0][x & 255]++;
        count[1][x >> 8 & 255]++;
        count[2][x >> 16 & 255]++;
        count[3][x >> 24]++;
    }
    for (d = 0; d < 4; d++) {
        int shift = d * 8, sum = 0, *c = count[d];
        if (c[k[0] >> shift & 255] == n) {
            continue;
        }
        for (i = 0; i < 256; i++) {
            int t = c[i];
            c[i] = sum;
            sum += t;
        }
        if (v != NULL) {
            for (i = 0; i < n; i++) {
                int at = c[k[i] >> shift & 255]++;
                kt[at] = k[i];
                vt[at] = v[i];
            }
        } else {
            for (i = 0; i < n; i++) {
                kt[c[k[i] >> shift & 255]++] = k[i];
            }
        }
        ks = k, k = kt, kt = ks;
        vs = v, v = vt, vt = vs;
    }
    if (k != key) {
        memcpy(key, k, (size_t)n * sizeof *key);
        if (val != NULL) {
            memcpy(val, v, (size_t)n * sizeof *val);
        }
    }
}

/* a few keys by insertion, more by radix */
static void radix_sort(uint32_t* a, uint32_t* tmp, int n) {
    int i, j;
    if (n > 64) {
        radix_sort_pairs(a, NULL, tmp, NULL, n);
        return;
    }
    for (i = 1; i < n; i++) {
        uint32_t x = a[i];
        for (j = i; j > 0 && a[j - 1] > x; j--) {
            a[j] = a[j - 1];
        }
        a[j] = x;
    }
}

/* (key, value) words by their keys, from lo to hi, stable: by the bits the keys don't all share, in digits of at
   most 9 bits (the counts stay in the cache), a pass writing one array. Digits all keys share are skipped */
static void radix_sort_groups(uint64_t* a, uint64_t* tmp, int n, uint32_t lo, uint32_t hi) {
    static int count[4][512];
    int bits = 32 - __builtin_clz((hi - lo) | 1), passes = (bits + 8) / 9, w = (bits + passes - 1) / passes, i, d;
    uint32_t mask = (1u << w) - 1;
    uint64_t *src = a, *dst = tmp, *t;
    if (n < 2) {
        return;
    }
    memset(count, 0, sizeof count);
    for (i = 0; i < n; i++) {
        uint32_t x = (uint32_t)(a[i] >> 32) - lo;
        for (d = 0; d < passes; d++) {
            count[d][x >> (d * w) & mask]++;
        }
    }
    for (d = 0; d < passes; d++) {
        int shift = d * w, sum = 0, *c = count[d];
        if (c[((uint32_t)(src[0] >> 32) - lo) >> shift & mask] == n) {
            continue;
        }
        for (i = 0; i <= (int)mask; i++) {
            int k = c[i];
            c[i] = sum;
            sum += k;
        }
        for (i = 0; i < n; i++) {
            uint64_t v = src[i];
            dst[c[((uint32_t)(v >> 32) - lo) >> shift & mask]++] = v;
        }
        t = src, src = dst, dst = t;
    }
    if (src != a) {
        memcpy(a, src, (size_t)n * sizeof *a);
    }
}

static int sort_fit;

/* keys from lo to hi, by digits of their distance from lo: three, or with MVC2_SORT_FIT (on) as many as cost least,
   a pass over the keys weighed against one over a digit's counts. Digits all keys share are skipped */
static void radix_sort_range(uint32_t* a, uint32_t* tmp, int n, uint32_t lo, uint32_t hi) {
    static int count[4][1024];
    int bits = 32 - __builtin_clz((hi - lo) | 1), passes = 3, w, i, d;
    uint32_t mask, *src = a, *dst = tmp, *t;
    if (n <= 64 || bits > 30) {
        radix_sort(a, tmp, n);
        return;
    }
    if (sort_fit) {
        int p, best = INT32_MAX;
        for (p = 1; p <= 4; p++) {
            int pw = (bits + p - 1) / p, cost = p * (2 * n + (1 << pw));
            if (pw <= 10 && cost < best) {
                best = cost;
                passes = p;
            }
        }
    }
    w = (bits + passes - 1) / passes;
    mask = (1u << w) - 1;
    for (d = 0; d < passes; d++) {
        memset(count[d], 0, (mask + 1) * sizeof **count);
    }
    for (i = 0; i < n; i++) {
        uint32_t x = a[i] - lo;
        for (d = 0; d < passes; d++) {
            count[d][x >> (d * w) & mask]++;
        }
    }
    for (d = 0; d < passes; d++) {
        int shift = d * w, sum = 0, *c = count[d];
        if (c[(src[0] - lo) >> shift & mask] == n) {
            continue;
        }
        for (i = 0; i <= (int)mask; i++) {
            int k = c[i];
            c[i] = sum;
            sum += k;
        }
        for (i = 0; i < n; i++) {
            uint32_t x = src[i];
            dst[c[(x - lo) >> shift & mask]++] = x;
        }
        t = src, src = dst, dst = t;
    }
    if (src != a) {
        memcpy(a, src, (size_t)n * sizeof *a);
    }
}

/* the bits of the greatest float at or below z (> 0), or below it when strictly: the ends' keys, all positive
   floats, compare with z as these do */
static uint32_t key_below(double z, int strictly) {
    float f = (float)z;
    if ((double)f > z || (strictly && (double)f == z)) {
        f = nextafterf(f, 0.0f);
    }
    return zbits(f);
}

/* the first of the n sorted keys above key z */
static int above(const uint32_t* keys, int n, uint32_t z) {
    int lo = 0, hi = n;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (keys[mid] <= z) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

/* the depth in a..b that the fewest triangles cross */
static double best_cut(double a, double b) {
    uint32_t ka = key_below(a, 0), kb = key_below(b, 1);
    int ia = above(zends[0], nends, ka), ib = above(zends[1], nends, ka), best_n = nends + 1;
    double best = b;
    for (;;) {
        uint32_t e = ia < nends ? zends[0][ia] : UINT32_MAX, next = UINT32_MAX;
        if (ib < nends && zends[1][ib] < e) {
            e = zends[1][ib];
        }
        if (e > kb) {
            break;
        }
        while (ia < nends && zends[0][ia] <= e) {
            ia++;
        }
        while (ib < nends && zends[1][ib] <= e) {
            ib++;
        }
        if (ia < nends) {
            next = zends[0][ia];
        }
        if (ib < nends && zends[1][ib] < next) {
            next = zends[1][ib];
        }
        if (ia - ib < best_n) {
            double ez = zfloat(e), nz = next == UINT32_MAX ? 1e30 : zfloat(next);
            best_n = ia - ib;
            best = min_d(min_d(ez * (1.0 + 1.0 / 1048576), (ez + nz) * 0.5), b);
        }
    }
    kb = key_below(b, 0);
    ia = above(zends[0], nends, kb);
    ib = above(zends[1], nends, kb);
    return ia - ib <= best_n ? b : best;
}

static int culled(const RD_STATE* s, const RD_TRI* t) {
    const RD_VTX *a = &t->v[0], *b = &t->v[1], *c = &t->v[2];
    double area;
    if (t->stays || s->cull == 0) {
        return 0;
    }
    area = ((double)b->x - a->x) * ((double)c->y - a->y) - ((double)b->y - a->y) * ((double)c->x - a->x);
    return fabs(area) < RD_CULL_VAL || (s->cull == 2 && area < 0) || (s->cull == 3 && area > 0);
}

static int unseen(const RD_TRI* t) {
    return rd_unseen(&t->v[0], &t->v[1], &t->v[2]);
}

/* drawn in any order among others like it: tested against the depth and writing it, not blended. Slices take their
   own parts of the depth buffer, so their order doesn't matter either */
static int order_free(const RD_STATE* s) {
    return rd_order_free(s);
}

static int max_slices = MAX_SLICES;  /* MVC2_SLICES=n (2 to 16): fewer, wider slices */

/* the slices, far to near: each at most SLICE_SPAN of 1/w, closed early where sprites' depths are too close */
static void make_slices(void) {
    int list, i, n = 0, m = 0, behind = 0, track = fr->ngroups > 0 && gpu_late_on;
    nends = 0;
    float zmin = 1e30f, zmax = 0.0f, znear = 0.0f;
    double lo, top, ratio;
    PROF_START(p0);
    PFZ_START(q0);
    last_dep[0] = last_dep[1] = last_dep[2] = -1;
    for (list = 0; list <= RD_LISTS; list++) {
        int ntris = list < RD_LISTS ? fr->ntris[list] : L->nbg;
        const RD_TRI* tris = list < RD_LISTS ? fr->tris[list] : bg_tri;
        RD_TRI_INFO* inf = list < RD_LISTS ? fr->info[list] : NULL;
        for (i = 0; i < ntris; i++) {
            const RD_TRI* t = &tris[i];
            float a, b;
            int flat;
            {
                /* the triangle's info for the passes after this one, which read it instead */
                float z0 = t->v[0].z, z1 = t->v[1].z, z2 = t->v[2].z;
                a = min_f(z0, min_f(z1, z2));
                b = max_f(z0, max_f(z1, z2));
                flat = zbits(z0) == zbits(z1) && zbits(z0) == zbits(z2);
                if (inf != NULL) {
                    RD_TRI_INFO f;
                    f.lo = a;
                    f.hi = b;
                    f.state = t->state;
                    f.flags = (uint8_t)((culled(&fr->states[t->state], t) ? RD_TRI_CULLED : 0) |
                                        (list == RD_TRANS && unseen(t) ? RD_TRI_UNSEEN : 0) | (flat ? RD_TRI_FLAT_Z : 0) |
                                        (t->share != 0 ? RD_TRI_SHARES : 0));
                    f.pad = 0;
                    inf[i] = f;
                    if (track && !order_free(&fr->states[t->state])) {
                        last_dep[list] = i;
                    }
                } else {
                    bg_lo[i] = a;
                    bg_hi[i] = b;
                }
            }
            if (a > 0.0f) {
                /* then a and b are the least and greatest of those not NaN, z2 among them: the same z's have
                   the same bits */
                zmin = a < zmin ? a : zmin;
                zmax = b > zmax ? b : zmax;
                uint32_t key = zbits(a);
                if (flat && n < MAX_KEYS) {
                    if (n == 0 || zkeys[n - 1] != key) {    /* a sprite's triangles share theirs */
                        zkeys[n++] = key;
                    }
                } else if (nends < MAX_KEYS) {
                    zends[0][nends] = zbits(a);
                    zends[1][nends++] = zbits(b);
                }
            } else {
                int k;
                for (k = 0; k < 3; k++) {
                    float z = t->v[k].z;
                    if (z > 0.0f) {
                        zmin = z < zmin ? z : zmin;
                        zmax = z > zmax ? z : zmax;
                    }
                }
                if (nends < MAX_KEYS && b > 0.0f) {
                    behind = 1;
                    zends[0][nends] = zbits(1e-30f);
                    zends[1][nends++] = zbits(b);
                }
            }
        }
    }
    /* a group cut at the near plane ends there: the slices go no nearer for it */
    PROF_START(pg);
    PFZ_START(qg);
    for (i = 0; i < fr->ngroups; i++) {
        float wlo, whi, key, near_w, a, b;
        int held;
        if (rd_group_info) {
            const RD_GPU_INFO* q = &fr->ginfo[i];
            wlo = q->wlo;
            whi = q->whi;
            key = q->key;
            near_w = q->near_w;
            gpu_list[i] = q->list;
            held = gmesh != NULL && q->mesh >= 0 && q->mesh < MAX_MESHES && mesh_held[q->mesh];
        } else {
            const RD_GPU_DRAW* d = &fr->groups[i];
            const GMESH* g = gmesh != NULL && d->g.mesh >= 0 && d->g.mesh < MAX_MESHES ? &gmesh[d->g.mesh] : NULL;
            wlo = d->g.wlo;
            whi = d->g.whi;
            key = d->g.key;
            near_w = fr->puts[d->put].near_w;
            gpu_list[i] = fr->states[d->state].list;
            held = g != NULL && g->size != 0;
        }
        gpu_lo[i] = gpu_hi[i] = 0.0f;
        if (!held || !(whi > near_w) || !(wlo <= whi)) {
            continue;
        }
        a = 1.0f / whi;
        b = 1.0f / max_f(wlo, near_w);
        gpu_lo[i] = a;
        gpu_hi[i] = b;
        gpu_key[i] = key > 0.0f ? key : (a + b) * 0.5f;
        zmin = a < zmin ? a : zmin;
        if (wlo < near_w) {
            znear = b > znear ? b : znear;
        } else {
            zmax = b > zmax ? b : zmax;
        }
        if (nends < MAX_KEYS) {
            zends[0][nends] = zbits(a);
            zends[1][nends++] = zbits(b);
        }
    }
    /* late: order-free, with nothing after it in its list that isn't */
    {
        int dep_after[RD_LISTS] = { 0 };
        for (i = fr->ngroups - 1; i >= 0; i--) {
            int l = gpu_list[i], at, free;
            if (rd_group_info) {
                free = track && fr->ginfo[i].free;
                at = fr->ginfo[i].at;
            } else {
                free = track && order_free(&fr->states[fr->groups[i].state]);
                at = fr->groups[i].at;
            }
            gpu_late[i] = (uint8_t)(free && !dep_after[l] && at > last_dep[l]);
            dep_after[l] |= !free;
        }
    }
    PROF_STOP(PZ_SCAN_GROUPS, pg);
    PFZ_STOP(PFZ_SCAN_GROUPS, qg);
    top = max_f(zmax * 2.0f, znear);
    zmax = max_f(zmax, znear);
    L->nslices = 0;
    PROF_STOP(PZ_SLICE_SCAN, p0);
    PFZ_STOP(PFZ_SCAN, q0);
    PROF_COUNT(PZ_SLICE_SCAN, (uint32_t)(fr->ntris[RD_OPAQUE] + fr->ntris[RD_PUNCH] + fr->ntris[RD_TRANS] + L->nbg));
    PROF_COUNT(PZ_SCAN_GROUPS, (uint32_t)fr->ngroups);
    PROF_START(p1);
    PFZ_START(q1);
    if (zmax == 0.0f) {
        L->slice_lo[0] = 0.0f;
        L->slice_lo[1] = 1.0f;
        L->nslices = 1;
        return;
    }
    if (sort_fit) {
        radix_sort_range(zkeys, ztmp, n, zbits(zmin), zbits(zmax));
    } else {
        radix_sort(zkeys, ztmp, n);
    }
    /* the ends lie from zmin (1e-30 for one behind the eye) to zmax */
    radix_sort_range(zends[0], ztmp, nends, zbits(behind ? 1e-30f : zmin), zbits(zmax));
    radix_sort_range(zends[1], ztmp, nends, zbits(behind ? 1e-30f : zmin), zbits(zmax));
    for (i = 0; i < n; i++) {
        if (m == 0 || zkeys[i] != zkeys[m - 1]) {
            zkeys[m++] = zkeys[i];
        }
    }
    PROF_STOP(PZ_SLICE_SORT, p1);
    PFZ_STOP(PFZ_SLICE_SORT, q1);
    PROF_COUNT(PZ_SLICE_SORT, (uint32_t)(n + 2 * nends));     /* the keys sorted */
    PROF_START(p2);
    lo = zmin * 0.5f;
    ratio = pow(top / lo, 1.0 / (max_slices / 2));
    ratio = ratio < 2.0 ? 2.0 : ratio;
    i = 0;
    while (L->nslices < max_slices - 1) {
        double limit = lo * ratio < top ? lo * ratio : top, hi, gap = 1e30, prev = 0.0;
        int have = 0;
        while (i < m && zfloat(zkeys[i]) < limit) {
            double z = zfloat(zkeys[i]);
            if (have && (z - lo > SLICE_SPAN * min_d(gap, z - prev))) {
                break;
            }
            if (have) {
                gap = min_d(gap, z - prev);
            }
            prev = z;
            have = 1;
            i++;
        }
        hi = limit;
        if (have && ((i < m && zfloat(zkeys[i]) < limit) || hi - lo > SLICE_SPAN * gap)) {
            double next = i < m ? zfloat(zkeys[i]) : prev * 2.0;
            hi = prev + min_d((next - prev) * 0.5, prev * (1.0 / 1048576));
        } else if (hi < top) {
            hi = best_cut(lo * sqrt(ratio), hi);
            while (i > 0 && zfloat(zkeys[i - 1]) >= hi) {
                i--;
            }
        }
        if (fr->ngroups > 0 && hi < top && hi > top * (1.0 - 1.0 / 1048576)) {
            hi = top;   /* no sliver at the top: a group's matrix in one would overflow float24 */
        }
        L->slice_lo[L->nslices++] = lo;
        lo = hi;
        if (lo >= top) {
            break;
        }
    }
    if (lo < top) {
        L->slice_lo[L->nslices++] = lo;   /* the slices ran out */
    }
    L->slice_lo[L->nslices] = top;
    PROF_STOP(PZ_SLICE_CUT, p2);
    PROF_COUNT(PZ_SLICE_CUT, (uint32_t)L->nslices);
}

/* a slice's depth, z * scale + base; in single precision (MVC2_SINGLE, on) (z - lo) * scale + (base + lo * scale), the
   terms near 1 and the result good to a few float steps */
static int single;

typedef struct {
    double scale, base;
    float lo, fscale, fbase;
} DEPTH_MAP;

static DEPTH_MAP depth_map(int k) {
    DEPTH_MAP m;
    m.scale = L->slice_scale[k];
    m.base = L->slice_base[k];
    m.lo = (float)L->slice_lo[k];
    m.fscale = (float)m.scale;
    m.fbase = (float)(m.base + m.lo * m.scale);
    return m;
}

/* with its depth in the map, and c's colour (a flat triangle's last vertex) */
static inline __attribute__((always_inline)) uint16_t put_at(const RD_VTX* s, float su, float sv, const DEPTH_MAP* m,
                                                             const RD_VTX* c) {
    uint16_t at = (uint16_t)L->nverts;
    GV* v = &L->verts[L->nverts++];
    v->x = s->x + GAME_X;
    v->y = s->y;
    v->d = single ? (s->z - m->lo) * m->fscale + m->fbase : (float)(s->z * m->scale + m->base);
    v->w = rcp_w ? s->z : 1.0f / s->z;
    v->u = s->u * su;
    v->v = 1.0f - s->v * sv;
    v->c = (c != NULL ? c : s)->base;
    return at;
}

/* in slice k */
static uint16_t put_vertex(const RD_VTX* s, float su, float sv, int k, const RD_VTX* c) {
    DEPTH_MAP m = depth_map(k);
    return put_at(s, su, sv, &m, c);
}

static void use_buffer(GV* v) {
    L->verts = v;
    L->idx = (uint16_t*)(v + MAX_VERTS);
}

/* the last triangle put whole, while its batch goes on: the next in its strip shares two of its vertices */
static struct {
    const RD_TRI* t;
    uint16_t at[3];
    int end;
} prev = { NULL, { 0 }, -1 };

enum { CUT_Z, CUT_X, CUT_Y };

static double cut_key(const RD_VTX* v, int axis) {
    return axis == CUT_Z ? v->z : axis == CUT_X ? v->x : v->y;
}

/* x, y and 1/w are linear on the screen, the rest divided by w */
static RD_VTX cut_edge(const RD_VTX* a, const RD_VTX* b, int axis, double at) {
    RD_VTX r;
    double t, za, zb, z, wa, wb;
    int sh;
    if (a->z > b->z || (a->z == b->z && (a->x > b->x || (a->x == b->x && a->y > b->y)))) {
        const RD_VTX* c = a;    /* the same point from either side of an edge */
        a = b;
        b = c;
    }
    za = a->z;
    zb = b->z;
    t = (at - cut_key(a, axis)) / (cut_key(b, axis) - cut_key(a, axis));
    z = axis == CUT_Z ? at : za + t * (zb - za);
    r.x = (float)(axis == CUT_X ? at : a->x + t * (b->x - a->x));
    r.y = (float)(axis == CUT_Y ? at : a->y + t * (b->y - a->y));
    r.z = (float)z;
    wb = zb * t / z;
    wa = 1.0 - wb;
    r.u = (float)(a->u * wa + b->u * wb);
    r.v = (float)(a->v * wa + b->v * wb);
    r.base = a->base;
    if (a->base != b->base || !(fabs(wb) < 1e9)) {   /* each channel c * wa + c * wb rounds to c */
        r.base = 0;
        for (sh = 0; sh < 32; sh += 8) {
            double c = (a->base >> sh & 255) * wa + (b->base >> sh & 255) * wb + 0.5;
            r.base |= (uint32_t)(c < 0 ? 0 : c > 255 ? 255 : c) << sh;
        }
    }
    r.offset = a->offset;
    return r;
}

/* p cut to the side sign keeps (1 above, -1 below), into q; the corners left */
static int cut_poly(const RD_VTX* p, int n, RD_VTX* q, int axis, double at, int sign) {
    int i, m = 0;
    for (i = 0; i < n; i++) {
        const RD_VTX* a = &p[i];
        const RD_VTX* b = &p[i + 1 < n ? i + 1 : 0];
        int ina = (cut_key(a, axis) - at) * sign >= 0, inb = (cut_key(b, axis) - at) * sign >= 0;
        if (ina) {
            q[m++] = *a;
        }
        if (ina != inb) {
            q[m++] = cut_edge(a, b, axis, at);
        }
    }
    return m;
}

/* p cut into the parts below and above, each cut point worked out once */
static void split_poly(const RD_VTX* p, int n, RD_VTX* lo, int* nlo, RD_VTX* hi, int* nhi, int axis, double at) {
    int i, nl = 0, nh = 0;
    for (i = 0; i < n; i++) {
        const RD_VTX* a = &p[i];
        const RD_VTX* b = &p[i + 1 < n ? i + 1 : 0];
        double ka = cut_key(a, axis) - at, kb = cut_key(b, axis) - at;
        int la = ka <= 0, lb = kb <= 0, ha = ka >= 0, hb = kb >= 0, have = 0;
        RD_VTX c;
        if (la) {
            lo[nl++] = *a;
        }
        if (la != lb) {
            c = cut_edge(a, b, axis, at);
            have = 1;
            lo[nl++] = c;
        }
        if (ha) {
            hi[nh++] = *a;
        }
        if (ha != hb) {
            if (!have) {
                c = cut_edge(a, b, axis, at);
            }
            hi[nh++] = c;
        }
    }
    *nlo = nl;
    *nhi = nh;
}

/* the first vertex's, or a flat one's last */
static uint32_t tri_offset(const RD_STATE* s, const RD_TRI* t) {
    return s->offset ? t->v[s->flat ? 2 : 0].offset & 0xFFFFFF : 0;
}

/* past this the GPU's float24 can't place a corner: cut to GUARD pixels round the screen */
#define GUARD 2048
/* MVC2_FAR_CUT=N: past N + 640 instead, cut to N round the screen, so the GPU's clip and float24 see no big triangle */
static double guard = GUARD;
static uint32_t far_bits = 0x46800000u;
/* |f| above 16384 (a NaN's isn't), on the float's bits: each VFP compare stalls the ARM11 */
static int far_coord(float f) {
    uint32_t m = zbits(f) & 0x7FFFFFFFu;
    return m > far_bits && m <= 0x7F800000u;
}

static int far_off(const RD_VTX* v) {
    return far_coord(v->x) || far_coord(v->y);
}

/* a far triangle in front of the eye and wholly past one edge of the drawing's target dropped before it is cut, as it
   draws nothing; not with MVC2_FAR_REJECT=0 */
static int far_reject;

static int past_edge(const RD_TRI* t) {
    const RD_VTX *a = &t->v[0], *b = &t->v[1], *c = &t->v[2];
    return (a->x < -GAME_X && b->x < -GAME_X && c->x < -GAME_X) ||
           (a->x > RD_WIDTH + GAME_X && b->x > RD_WIDTH + GAME_X && c->x > RD_WIDTH + GAME_X) ||
           (a->y < 0.0f && b->y < 0.0f && c->y < 0.0f) || (a->y > RD_HEIGHT && b->y > RD_HEIGHT && c->y > RD_HEIGHT);
}

/* MVC2_SLICE_CUT: a triangle crossing a slice's ends cut there, not left to the GPU's clip. Its depth in the slice
   reaches (zhi - zlo) / (hi - lo), placed by float24 to 2^-16 of that, and the slice's clip has 2^-12 to spare: one
   more than 8 slices deep loses a band at the slice's ends */
static int slice_cut;
/* MVC2_SLICE_CUT=all: every one crossing them cut here, so that two slices' parts share their edge exactly; the GPU's
   two clips leave a row drawn twice or not at all, which an added triangle shows as a line */
static float cut_slices = 8.0f;

static int cut_deep(float zlo, float zhi, double lo, double hi) {
    return slice_cut && (zlo < lo || zhi > hi) && zhi - zlo > cut_slices * (hi - lo);
}

static uint32_t max_u(uint32_t a, uint32_t b) {
    return a > b ? a : b;
}

/* all of t's corners within 16384 on both axes: the greatest of the bits shifted past the sign, for fewer tests */
static int near_tri(const RD_TRI* t) {
    const RD_VTX *a = &t->v[0], *b = &t->v[1], *c = &t->v[2];
    uint32_t m = max_u(max_u(max_u(zbits(a->x) << 1, zbits(a->y) << 1), max_u(zbits(b->x) << 1, zbits(b->y) << 1)),
                       max_u(zbits(c->x) << 1, zbits(c->y) << 1));
    return m <= far_bits << 1;
}

/* state i's texture made and its copy kept for the draws */
static void make_state_tex(RFRAME* r, const RD_FRAME* f, int i) {
    const RD_TEXTURE* t = f->states[i].tex;
    STATE_TEX* s = &r->st[i];
    GTEX* g;
    memset(s, 0, sizeof *s);
    s->tex = -1;
    if (t == NULL) {
        return;
    }
    s->pal = (uint8_t)is_pal(t->format);
    s->pal16 = t->format == RD_PAL4;
    {
        /* where a clear texel's colour can't show: its alpha used to blend or test */
        const RD_STATE* rs = &f->states[i];
        int pad = smooth_2d && !rs->bilinear && rs->tex_alpha && t->levels == 0 &&
                  (t->format == RD_ARGB4444 || t->format == RD_ARGB1555) && t->width * t->height <= PAD_MAX &&
                  (rs->list == RD_PUNCH || (rs->list == RD_TRANS && rs->src == RD_SRC_ALPHA));
        GTEX* h = tex_lean ? find_tex(t) : NULL;
        g = !tex_current ? NULL : tex_current_at(tex_lean ? h : find_tex(t), t, pad);
        if (g == NULL) {
            g = gpu_tex_at(tex_lean ? h : find_tex(t), t, pad);
        }
    }
    if (g != NULL) {
        s->tex = (int16_t)(g - gtex);
        if (!tex_current) {
            s->su = (float)t->width / (float)g->width;
            s->sv = (float)t->height / (float)g->height;
            r->tex[s->tex] = g->tex;
        } else {
            /* the frame's copy once, unless it is made again after */
            s->su = g->su;
            s->sv = g->sv;
            if (!copy_same) {
                if (g->copied != frame_no) {
                    r->tex[s->tex] = g->tex;
                    g->copied = frame_no;
                }
            } else if (r->made[s->tex] != g->made) {
                r->tex[s->tex] = g->tex;
                r->made[s->tex] = g->made;
            }
        }
    }
}

/* the draw for t in slice k: the last, when it goes on with the same state; NULL when there are too many */
static inline __attribute__((always_inline)) BATCH* batch_for(int k, const RD_TRI* t, const RD_STATE* s) {
    BATCH* b = L->nbatches > 0 ? &L->batches[L->nbatches - 1] : NULL;
    uint32_t off = tri_offset(s, t);
    if (b == NULL || b->count < 0 || b->slice != k || b->offset != off || b->first + b->count != L->nidx ||
        (b->state != t->state && !rd_same_state(&fr->states[b->state], s))) {
        if (L->nbatches == MAX_BATCHES) {
            return NULL;
        }
        b = &L->batches[L->nbatches++];
        b->state = t->state;
        b->slice = (uint16_t)k;
        b->offset = off;
        b->first = L->nidx;
        b->count = 0;
    }
    return b;
}

/* t whole, by index: corners shared with the list's triangle before, when that was put just before in this batch
   and, for a flat state, has the same colour */
static inline __attribute__((always_inline)) void put_whole(BATCH* b, const RD_TRI* t, const RD_VTX* flat, float su,
                                                            float sv, const DEPTH_MAP* m) {
    const RD_VTX* v = t->v;
    int share = t->share != 0 && prev.t == t - 1 && prev.end == L->nidx && b->count > 0 &&
                (flat == NULL || flat->base == prev.t->v[2].base) ? t->share : 0;
    uint16_t a = share & 3 ? prev.at[(share & 3) - 1] : put_at(&v[0], su, sv, m, flat);
    uint16_t c = share >> 2 ? prev.at[(share >> 2) - 1] : put_at(&v[1], su, sv, m, flat);
    prev.t = t;
    prev.at[0] = L->idx[L->nidx++] = a;
    prev.at[1] = L->idx[L->nidx++] = c;
    prev.at[2] = L->idx[L->nidx++] = put_at(&v[2], su, sv, m, flat);
    prev.end = L->nidx;
    b->count += 3;
}

/* the part of t (its 1/w from zlo to zhi) in slice k: cut by the GPU, or here when it passes the eye */
static void add_tri(int k, const RD_TRI* t, float zlo, float zhi) {
    const RD_STATE* s = &fr->states[t->state];
    const STATE_TEX* st = &rf->st[t->state];
    double lo = L->slice_lo[k], hi = L->slice_lo[k + 1];
    RD_VTX p[9], q[9];
    const RD_VTX* v = t->v;
    BATCH* b;
    int n = 3, i;
    if (zhi < lo || zlo >= hi) {
        return;
    }
    if (far_off(&t->v[0]) || far_off(&t->v[1]) || far_off(&t->v[2])) {
        if (far_reject && zlo > 0.0f && past_edge(t)) {
            return;
        }
        n = cut_poly(t->v, 3, p, CUT_Z, lo, 1);
        n = cut_poly(p, n, q, CUT_Z, hi, -1);
        n = cut_poly(q, n, p, CUT_X, -guard, 1);
        n = cut_poly(p, n, q, CUT_X, RD_WIDTH + guard, -1);
        n = cut_poly(q, n, p, CUT_Y, -guard, 1);
        n = cut_poly(p, n, q, CUT_Y, RD_HEIGHT + guard, -1);
        v = q;
    } else if (((zlo < lo || zhi > hi) && !(zlo > 0.0f)) || cut_deep(zlo, zhi, lo, hi)) {
        n = cut_poly(t->v, 3, p, CUT_Z, lo, 1);
        n = cut_poly(p, n, q, CUT_Z, hi, -1);
        v = q;
    }
    if (n < 3) {
        return;
    }
    if (L->nidx + (n - 2) * 3 > max_idx) {
        early_short = 1;
        return;
    }
    if (st->tex == TEX_LATER) {
        make_state_tex(rf, fr, t->state);
    }
    if (s->tex != NULL && st->tex < 0) {
        return;
    }
    b = batch_for(k, t, s);
    if (b == NULL) {
        return;
    }
    {
        float su = st->su, sv = st->sv;
        const RD_VTX* flat = s->flat ? &t->v[2] : NULL;
        uint16_t a, c;
        if (!indexed) {
            for (i = 2; i < n; i++) {
                put_vertex(&v[0], su, sv, k, flat);
                put_vertex(&v[i - 1], su, sv, k, flat);
                put_vertex(&v[i], su, sv, k, flat);
                b->count += 3;
            }
            L->nidx = L->nverts;
            return;
        }
        if (v == t->v) {
            DEPTH_MAP m = depth_map(k);
            put_whole(b, t, flat, su, sv, &m);
            return;
        }
        /* a cut one as a fan */
        a = put_vertex(&v[0], su, sv, k, flat);
        c = put_vertex(&v[1], su, sv, k, flat);
        for (i = 2; i < n; i++) {
            L->idx[L->nidx++] = a;
            L->idx[L->nidx++] = c;
            L->idx[L->nidx++] = c = put_vertex(&v[i], su, sv, k, flat);
            b->count += 3;
        }
        prev.end = -1;
    }
}

/* add_tri for by_slice's triangles from..to of a list in slice k, by index: those wholly in front of the eye and
   near the screen, which need no cutting, put here */
/* a slice's triangles put one by one, those that need no cutting here: what is kept from one to the next */
typedef struct {
    int k;
    double lo, hi;
    float flo, fhi, deep;
    DEPTH_MAP m;
    const RD_STATE* s;
    BATCH* run;             /* the draw the last triangle here went in, with this state */
    float su, sv;
    int state, usable;
} FAST;

static inline __attribute__((always_inline)) void fast_begin(FAST* f, int k) {
    f->k = k;
    f->lo = L->slice_lo[k];
    f->hi = L->slice_lo[k + 1];
    f->flo = (float)f->lo;
    f->fhi = (float)f->hi;
    f->deep = cut_slices * (f->fhi - f->flo);
    f->m = depth_map(k);
    f->s = NULL;
    f->run = NULL;
    f->su = f->sv = 0.0f;
    f->state = -1;
    f->usable = 0;
}

static inline __attribute__((always_inline)) void fast_tri(FAST* f, const RD_TRI* t, float zlo, float zhi) {
    int k = f->k;
    BATCH* b;
    if (!(zlo > 0.0f) || (!near_tri(t) && (far_off(&t->v[0]) || far_off(&t->v[1]) || far_off(&t->v[2])))) {
        add_tri(k, t, zlo, zhi);
        f->run = NULL;
        return;
    }
    if (single ? zhi < f->flo || zlo >= f->fhi : zhi < f->lo || zlo >= f->hi) {
        return;
    }
    if (single ? slice_cut && (zlo < f->flo || zhi > f->fhi) && zhi - zlo > f->deep : cut_deep(zlo, zhi, f->lo, f->hi)) {
        add_tri(k, t, zlo, zhi);
        f->run = NULL;
        return;
    }
    if (L->nidx + 3 > max_idx) {
        early_short = 1;
        return;
    }
    if (t->state != f->state) {
        const STATE_TEX* st = &rf->st[t->state];
        if (st->tex == TEX_LATER) {
            make_state_tex(rf, fr, t->state);
        }
        f->s = &fr->states[t->state];
        f->usable = f->s->tex == NULL || st->tex >= 0;
        f->su = st->su;
        f->sv = st->sv;
        f->state = t->state;
        f->run = NULL;
    }
    if (!f->usable) {
        return;
    }
    /* batch_for gives the same while nothing else has been put and the offset stays */
    b = f->run != NULL && (!f->s->offset || tri_offset(f->s, t) == f->run->offset) ? f->run : batch_for(k, t, f->s);
    if (b == NULL) {
        return;
    }
    put_whole(b, t, f->s->flat ? &t->v[2] : NULL, f->su, f->sv, &f->m);
    f->run = b;
}

static void add_slice(int k, int list, const RD_TRI* tris, int from, int to) {
    FAST f;
    int i;
    fast_begin(&f, k);
    for (i = from; i < to; i++) {
        int j = by_slice[i];
        fast_tri(&f, &tris[j], fr->info[list][j].lo, fr->info[list][j].hi);
    }
}

/* MVC2_TL_OVERLAP: a group's depth comes from its w in float24, good to about 2^-16 of it, and a slice's clip has
   1/4096 of the slice to spare; narrower slices lose a band of the group at their ends. Each slice the group is
   drawn in clips TL_OVERLAP of the depth further out (an opaque group's band is then drawn twice, the same), and a
   slice narrower than TL_NARROW of its depth isn't drawn in but given to the next wider one, the farther first */
#define TL_OVERLAP (1.0 / 16384)
#define TL_NARROW (1.0 / 1024)
static int tl_overlap;
static int map_custom;              /* the depth map a group's own, not its slice's */

static int slice_wide(int k) {
    return L->slice_lo[k + 1] - L->slice_lo[k] >= TL_NARROW * L->slice_lo[k + 1];
}

static int tl_host(int g, int k, int over, double* lo, double* hi);

/* GPU group g in slice k */
static void add_gpu(int k, int g) {
    const RD_GPU_DRAW* d = &fr->groups[g];
    const STATE_TEX* st = &rf->st[d->state];
    BATCH* b;
    if (tl_overlap && !tl_host(g, k, 0, NULL, NULL)) {
        return;
    }
    if (st->tex == TEX_LATER) {
        make_state_tex(rf, fr, d->state);
    }
    if ((fr->states[d->state].tex != NULL && st->tex < 0) || L->nbatches == MAX_BATCHES ||
        (group_prep && L->ngprep == MAX_GPREP)) {
        return;
    }
    b = &L->batches[L->nbatches++];
    b->state = d->state;
    b->slice = (uint16_t)k;
    b->offset = group_prep ? (uint32_t)L->ngprep++ : 0;
    b->first = g;
    b->count = GPU_GROUP;
    if (prep_pack) {
        GPACK* q = &L->gpack[b->offset];
        const RD_STATE* s = &fr->states[d->state];
        q->face_argb = d->g.face_argb;
        q->su = st->su;
        q->sv = st->sv;
        q->mesh = d->g.mesh;
        q->put = d->put;
        q->cand = (uint8_t)corner_wanted(b);
        q->tw = q->cand ? s->tex->width : 0;
        q->th = q->cand ? s->tex->height : 0;
        q->over = s->list != RD_TRANS;
        q->k0 = L->gpu_k0[g];
        q->k1 = L->gpu_k1[g];
        q->faces = d->g.faces;
    }
}

/* the list's GPU groups in slice k after the triangles sent before them, from by_slice's from..to, those after them
   following */
static void add_groups(int k, int list, const RD_TRI* tris, int from, int to) {
    int g;
    for (g = 0; g < fr->ngroups; g++) {
        if (gpu_list[g] == list && L->gpu_k0[g] <= k && k <= L->gpu_k1[g] && !gpu_late[g]) {
            int at = fr->groups[g].at, i = from;
            while (i < to && by_slice[i] < at) {
                i++;
            }
            if (tris != NULL) {
                add_slice(k, list, tris, from, i);
            }
            from = i;
            add_gpu(k, g);
        }
    }
    if (tris != NULL) {
        add_slice(k, list, tris, from, to);
    }
}

/* a float as an int that compares as it does with the slices' (none negative): a NaN or a
   negative below 0, and -0 as 0 */
static int32_t z_key(float z) {
    uint32_t b = zbits(z);
    return b <= 0x7F800000u ? (int32_t)b : b == 0x80000000u ? 0 : -1;
}

static int slice_find(int32_t z) {
    int lo = 0, hi = L->nslices - 1;
    while (lo < hi) {
        int mid = (lo + hi + 1) / 2;
        if (slice_key[mid] <= z) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    return lo;
}

static int slice_of(float z) {
    return slice_find(z_key(z));
}

/* the background as two opaque triangles that set the depth (one at or behind the eye has none) */
static void background_tris(void) {
    static const float corner[4][2] = { { 0, 0 }, { RD_WIDTH, 0 }, { 0, RD_HEIGHT }, { RD_WIDTH, RD_HEIGHT } };
    const RD_VTX *a = &fr->background[0], *b = &fr->background[1], *c = &fr->background[2];
    double area = ((double)b->x - a->x) * ((double)c->y - a->y) - ((double)b->y - a->y) * ((double)c->x - a->x);
    RD_VTX v[4];
    int i, sh;
    L->nbg = 0;
    for (i = 0; i < 4; i++) {
        double w0 = 1, w1 = 0, w2 = 0, z;
        if (area != 0) {
            w1 = (((double)corner[i][0] - a->x) * ((double)c->y - a->y) - ((double)corner[i][1] - a->y) * ((double)c->x - a->x)) / area;
            w2 = (((double)b->x - a->x) * ((double)corner[i][1] - a->y) - ((double)b->y - a->y) * ((double)corner[i][0] - a->x)) / area;
            w0 = 1 - w1 - w2;
        }
        memset(&v[i], 0, sizeof v[i]);
        v[i].x = corner[i][0];
        v[i].y = corner[i][1];
        z = w0 * a->z + w1 * b->z + w2 * c->z;
        v[i].z = (float)z;
        for (sh = 0; sh < 32; sh += 8) {
            double k = w0 * (a->base >> sh & 255) + w1 * (b->base >> sh & 255) + w2 * (c->base >> sh & 255) + 0.5;
            v[i].base |= (uint32_t)(k < 0 ? 0 : k > 255 ? 255 : k) << sh;
        }
        L->bg_vtx[i] = v[i];
    }
    for (i = 0; i < 4; i++) {
        if (!(v[i].z > 0 && v[i].z < 1e30f)) {
            return;
        }
    }
    if (fr->nstates >= RD_MAX_STATES) {
        return;
    }
    bg_state = fr->nstates++;
    memset(&fr->states[bg_state], 0, sizeof fr->states[bg_state]);
    memset(&rf->st[bg_state], 0, sizeof rf->st[bg_state]);
    rf->st[bg_state].tex = -1;
    fr->states[bg_state].list = RD_OPAQUE;
    fr->states[bg_state].depth = RD_ALWAYS;
    fr->states[bg_state].zwrite = 1;
    fr->states[bg_state].src = RD_ONE;
    fr->states[bg_state].dst = RD_ZERO;
    fr->states[bg_state].fog = RD_FOG_NONE;
    bg_tri[0].state = bg_tri[1].state = (uint16_t)bg_state;
    bg_tri[0].share = bg_tri[1].share = 0;
    bg_tri[0].v[0] = v[0], bg_tri[0].v[1] = v[1], bg_tri[0].v[2] = v[2];
    bg_tri[1].v[0] = v[1], bg_tri[1].v[1] = v[3], bg_tri[1].v[2] = v[2];
    L->nbg = 2;
}

/* a float's bits, sorting as the float does */
static uint32_t sort_bits(float f) {
    uint32_t b;
    memcpy(&b, &f, 4);
    return b & 0x80000000u ? ~b : b | 0x80000000u;
}

static int above_z(const uint32_t* zs, int n, float z) {
    int lo = 0, hi = n;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (zfloat(zs[mid]) <= z) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

/* t, sorted by zlo to zhi (for a part, its polygon's), its own 1/w from tlo to thi */
static void trans_add(const RD_TRI* t, float zlo, float zhi, float tlo, float thi, int hidden) {
    if (ntrans < MAX_ITEMS) {
        titem[ntrans++] = (TITEM){ t, zlo, zhi, tlo, thi, sort_bits((zlo + zhi) * 0.5f), 1, (uint8_t)hidden };
    }
}

static void trans_add_poly(const RD_TRI* t, const RD_VTX* p, int n) {
    float zlo = p[0].z, zhi = p[0].z;
    int i, first = ntrans;
    for (i = 1; i < n; i++) {
        zlo = min_f(zlo, p[i].z);
        zhi = max_f(zhi, p[i].z);
    }
    for (i = 2; i < n && nparts < MAX_PARTS; i++) {     /* as count_parts */
        RD_TRI* r = &trans_part[nparts++];
        r->state = t->state;
        r->share = 0;
        r->v[0] = p[0];
        r->v[1] = p[i - 1];
        r->v[2] = p[i];
        trans_add(r, zlo, zhi, min_f(r->v[0].z, min_f(r->v[1].z, r->v[2].z)), max_f(r->v[0].z, max_f(r->v[1].z, r->v[2].z)), 0);
    }
    if (ntrans > first) {
        titem[first].run = (uint16_t)(ntrans - first);
    }
}

/* t cut at the shallow ones' depths a to b - 1 */
static void trans_cut(const RD_TRI* t, int a, int b) {
    RD_VTX buf[2][9], part[9];
    RD_VTX* rest = buf[0];
    int nr = 3, np, j;
    memcpy(rest, t->v, sizeof t->v);
    if (fr->states[t->state].flat) {
        rest[0].base = rest[1].base = rest[2].base;     /* a flat one's colours on every corner */
        rest[0].offset = rest[1].offset = rest[2].offset;
    }
    for (j = a; j < b && nr >= 3; j++) {
        RD_VTX* next = rest == buf[0] ? buf[1] : buf[0];
        split_poly(rest, nr, part, &np, next, &nr, CUT_Z, zfloat(sprite_z[j]));
        rest = next;
        if (np >= 3) {
            trans_add_poly(t, part, np);
        }
    }
    if (nr >= 3) {
        trans_add_poly(t, rest, nr);
    }
}

static void count_parts(int n) {
    int i;
    for (i = 2; i < n && nparts < MAX_PARTS; i++) {
        nparts++;
    }
}

/* the parts trans_cut(t, a, b) would make, counted from the corners' depths alone (a cut corner's is the
   cut's), so that a hidden triangle takes its share of MAX_PARTS */
static void trans_cut_count(const RD_TRI* t, int a, int b) {
    float buf[2][9];
    float* rest = buf[0];
    int nr = 3, j, i;
    for (i = 0; i < 3; i++) {
        rest[i] = t->v[i].z;
    }
    for (j = a; j < b && nr >= 3; j++) {
        float* next = rest == buf[0] ? buf[1] : buf[0];
        double at = zfloat(sprite_z[j]);
        int np = 0, nh = 0;
        for (i = 0; i < nr; i++) {
            double ka = rest[i] - at, kb = rest[i + 1 < nr ? i + 1 : 0] - at;
            int la = ka <= 0, lb = kb <= 0, ha = ka >= 0, hb = kb >= 0;
            np += la + (la != lb);
            if (ha) {
                next[nh++] = rest[i];
            }
            if (ha != hb) {
                next[nh++] = (float)at;
            }
        }
        rest = next;
        nr = nh;
        if (np >= 3) {
            count_parts(np);
        }
    }
    if (nr >= 3) {
        count_parts(nr);
    }
}

/* the translucent list far first by middle depth, stable; a deep triangle is cut at the shallow ones'
   (sprites') depths, so each part sorts right against them */
#define SHALLOW 1.25f       /* farthest over nearest */
#define TRANS_DEEP SHALLOW  /* a translucent GPU group deeper is left to the CPU's sort */
#define DEEP 4.0f
static void sort_trans(void) {
    int n = fr->ntris[RD_TRANS], i, nz = 0, m, deep = 0, nitems;
    PROF_START(p0);
    ntrans = nparts = 0;
    for (i = 0; i < n; i++) {
        const RD_TRI_INFO* f = &fr->info[RD_TRANS][i];
        float zlo = f->lo, zhi = f->hi;
        if (!(f->flags & RD_TRI_CULLED)) {
            deep |= zhi > zlo * DEEP;
            trans_add(&fr->tris[RD_TRANS][i], zlo, zhi, zlo, zhi, (f->flags & RD_TRI_UNSEEN) != 0);
        }
    }
    nitems = ntrans;
    PROF_STOP(PZ_TRANS_PREP, p0);
    PROF_START(p1);
    if (deep) {
        int nraw = 0;
        for (i = 0; i < nitems; i++) {
            if (titem[i].lo > 0.0f && titem[i].hi <= titem[i].lo * SHALLOW && nraw + 2 <= MAX_ITEMS) {
                uint32_t lo = zbits(titem[i].lo), hi = zbits(titem[i].hi);
                nraw += 2;
                if (nz == 0 || sprite_z[nz - 1] != lo) {    /* the same depth again drops out below anyway */
                    sprite_z[nz++] = lo;
                }
                if (hi != lo) {
                    sprite_z[nz++] = hi;
                }
            }
        }
        radix_sort(sprite_z, sprite_tmp, nz);
        for (i = m = 0; i < nz; i++) {
            if (m == 0 || zfloat(sprite_z[i]) > zfloat(sprite_z[m - 1]) * 1.001f) {
                sprite_z[m++] = sprite_z[i];
            }
        }
        nz = m;
        for (i = 0; i < nitems; i++) {
            float zlo = titem[i].lo, zhi = titem[i].hi;
            int a, b;
            if (zhi <= zlo * DEEP) {
                continue;
            }
            a = above_z(sprite_z, nz, zlo);
            b = above_z(sprite_z, nz, zhi);
            if (b > a && zfloat(sprite_z[b - 1]) == zhi) {
                b--;
            }
            if (a < b && nparts + 3 * (b - a + 1) <= MAX_PARTS) {
                if (titem[i].hidden) {
                    trans_cut_count(titem[i].tri, a, b);
                } else {
                    trans_cut(titem[i].tri, a, b);
                }
                titem[i].tri = NULL;
            }
        }
    }
    PROF_STOP(PZ_TRANS_CUTS, p1);
    PROF_START(p2);
    /* a polygon's parts sorted as one, and only those drawn: the same order, stable. Each sorts with its
       first item, its count, and its first and last slices beside its key, so none is looked up after */
    {
        uint32_t lo = UINT32_MAX, hi = 0;
        for (i = n = 0; i < ntrans; i += titem[i].run) {
            const TITEM* it = &titem[i];
            if (it->tri != NULL && !it->hidden) {
                uint32_t key = it->key;
                lo = key < lo ? key : lo;
                hi = key > hi ? key : hi;
                group[n++] = (uint64_t)key << 32 |
                             (uint32_t)(i | it->run << 24 | slice_of(it->lo) << 16 | slice_of(it->hi) << 20);
            }
        }
        radix_sort_groups(group, group_tmp, n, lo, hi);
    }
    PROF_STOP(PZ_TRANS_GROUP, p2);
    PROF_START(p3);
    for (i = m = 0; i < n; i++) {
        uint32_t g = (uint32_t)group[i];
        int j = g & 0xFFFF, end = j + (g >> 24);
        uint8_t k0 = (uint8_t)(g >> 16 & 15), k1 = (uint8_t)(g >> 20 & 15);
        for (; j < end; j++) {
            trans_order[m] = j;
            tri_k0[m] = k0;
            tri_k1[m] = k1;
            m++;
        }
    }
    ntrans = m;
    PROF_STOP(PZ_TRANS_ORDER, p3);
}

/* MVC2_STRIP_CUTS: a strip more than DEEP deep sorted a triangle at a time, and a triangle that deep cut at the
   shallow strips' depths, as sort_trans does; its parts in trans_part, from STRIP_PART in trans_order */
#define STRIP_PART RD_MAX_TRIS
static int strip_cuts;
static float part_lo[MAX_PARTS], part_hi[MAX_PARTS];
static uint8_t part_ks[MAX_PARTS];      /* a part's first and last slices */

/* a polygon of t's sorted as one at its middle depth, its parts each in their own slices */
static void strip_poly(const RD_TRI* t, const RD_VTX* p, int n, int* ng, uint32_t* lo, uint32_t* hi) {
    float zlo = p[0].z, zhi = p[0].z;
    int i, first = nparts;
    for (i = 1; i < n; i++) {
        zlo = min_f(zlo, p[i].z);
        zhi = max_f(zhi, p[i].z);
    }
    for (i = 2; i < n && nparts < MAX_PARTS; i++) {
        RD_TRI* r = &trans_part[nparts];
        r->state = t->state;
        r->share = 0;
        r->v[0] = p[0];
        r->v[1] = p[i - 1];
        r->v[2] = p[i];
        part_lo[nparts] = min_f(r->v[0].z, min_f(r->v[1].z, r->v[2].z));
        part_hi[nparts] = max_f(r->v[0].z, max_f(r->v[1].z, r->v[2].z));
        part_ks[nparts] = (uint8_t)(slice_of(part_lo[nparts]) | slice_of(part_hi[nparts]) << 4);
        nparts++;
    }
    if (nparts > first) {
        uint32_t key = sort_bits((zlo + zhi) * 0.5f);
        *lo = key < *lo ? key : *lo;
        *hi = key > *hi ? key : *hi;
        group[(*ng)++] = (uint64_t)key << 32 | (uint32_t)first | 0x400000u | (uint32_t)(nparts - first) << 24;
    }
}

/* t cut at the shallow strips' depths a to b - 1, as trans_cut */
static void strip_cut(const RD_TRI* t, int a, int b, int* ng, uint32_t* lo, uint32_t* hi) {
    RD_VTX buf[2][9], part[9];
    RD_VTX* rest = buf[0];
    int nr = 3, np, j;
    memcpy(rest, t->v, sizeof t->v);
    if (fr->states[t->state].flat) {
        rest[0].base = rest[1].base = rest[2].base;
        rest[0].offset = rest[1].offset = rest[2].offset;
    }
    for (j = a; j < b && nr >= 3; j++) {
        RD_VTX* next = rest == buf[0] ? buf[1] : buf[0];
        split_poly(rest, nr, part, &np, next, &nr, CUT_Z, zfloat(sprite_z[j]));
        rest = next;
        if (np >= 3) {
            strip_poly(t, part, np, ng, lo, hi);
        }
    }
    if (nr >= 3) {
        strip_poly(t, rest, nr, ng, lo, hi);
    }
}

/* the cheaper order: each strip or quad far first by its middle depth, stable, none cut but those of MVC2_STRIP_CUTS.
   trans_order is then the list's own triangles, a GPU group's -1 - its number, a part's STRIP_PART + its number */
static void sort_trans_strips(void) {
    const RD_TRI* tris = fr->tris[RD_TRANS];
    const RD_TRI_INFO* inf = fr->info[RD_TRANS];
    static uint8_t tri_ks[RD_MAX_TRIS];     /* a triangle's first and last slices */
    static uint32_t deep[RD_MAX_TRIS];      /* the deep strips: first triangle, count from bit 24 */
    int n = fr->ntris[RD_TRANS], i, j, m, ng = 0, k = 0, gi = 0, ndeep = 0, nz = 0;
    uint32_t lo = UINT32_MAX, hi = 0;
    PROF_START(p0);
    nparts = 0;
    for (i = 0; i <= n; i = j) {
        float zlo = 0.0f, zhi = 0.0f;
        int seen = 0;
        /* the translucent GPU groups sent before triangle i, whole at their keys: bit 23 set, no count */
        for (; gi < fr->ngroups && (gpu_list[gi] != RD_TRANS || fr->groups[gi].at <= i); gi++) {
            if (gpu_list[gi] == RD_TRANS && L->gpu_k0[gi] <= L->gpu_k1[gi]) {
                uint32_t key = sort_bits(gpu_key[gi]);
                lo = key < lo ? key : lo;
                hi = key > hi ? key : hi;
                group[ng++] = (uint64_t)key << 32 | (uint32_t)gi | 0x800000u;
                if (strip_cuts && gpu_key[gi] > 0.0f && nz < MAX_ITEMS) {
                    sprite_z[nz++] = zbits(gpu_key[gi]);
                }
            }
        }
        if (i == n) {
            break;
        }
        for (j = i; j < n && (j == i || ((inf[j].flags & RD_TRI_SHARES) && inf[j].state == inf[i].state && j - i < 255)); j++) {
            if (!(inf[j].flags & (RD_TRI_CULLED | RD_TRI_UNSEEN))) {
                int32_t a = z_key(inf[j].lo), b = z_key(inf[j].hi);
                zlo = seen ? min_f(zlo, inf[j].lo) : inf[j].lo;
                zhi = seen ? max_f(zhi, inf[j].hi) : inf[j].hi;
                seen = 1;
                /* its slices as tri_slices finds them */
                if (!keys_rise || a < slice_key[k] || (k < L->nslices - 1 && a >= slice_key[k + 1])) {
                    k = slice_find(a);
                }
                tri_ks[j] = (uint8_t)(k | (b >= a && (k == L->nslices - 1 || b < slice_key[k + 1]) ? k : slice_find(b)) << 4);
            }
        }
        if (seen && strip_cuts && !(zhi <= zlo * DEEP)) {
            deep[ndeep++] = (uint32_t)(i | (j - i) << 24);
        } else if (seen) {
            uint32_t key = sort_bits((zlo + zhi) * 0.5f);
            lo = key < lo ? key : lo;
            hi = key > hi ? key : hi;
            group[ng++] = (uint64_t)key << 32 | (uint32_t)(i | (j - i) << 24);
            if (strip_cuts && zlo > 0.0f && zhi <= zlo * SHALLOW && nz + 2 <= MAX_ITEMS) {
                sprite_z[nz++] = zbits(zlo);
                sprite_z[nz++] = zbits(zhi);
            }
        }
    }
    PROF_STOP(PZ_TRANS_PREP, p0);
    PROF_START(p1);
    if (ndeep > 0) {
        /* the shallow strips' depths, 0.1% apart */
        radix_sort(sprite_z, sprite_tmp, nz);
        for (i = m = 0; i < nz; i++) {
            if (m == 0 || zfloat(sprite_z[i]) > zfloat(sprite_z[m - 1]) * 1.001f) {
                sprite_z[m++] = sprite_z[i];
            }
        }
        nz = m;
        for (i = 0; i < ndeep; i++) {
            int end = (int)(deep[i] & 0xFFFF) + (int)(deep[i] >> 24);
            for (j = deep[i] & 0xFFFF; j < end; j++) {
                int a, b;
                if (inf[j].flags & (RD_TRI_CULLED | RD_TRI_UNSEEN)) {
                    continue;
                }
                a = above_z(sprite_z, nz, inf[j].lo);
                b = above_z(sprite_z, nz, inf[j].hi);
                if (b > a && zfloat(sprite_z[b - 1]) == inf[j].hi) {
                    b--;
                }
                if (!(inf[j].hi <= inf[j].lo * DEEP) && a < b && nparts + 3 * (b - a + 1) <= MAX_PARTS) {
                    strip_cut(&tris[j], a, b, &ng, &lo, &hi);
                } else {
                    uint32_t key = sort_bits((inf[j].lo + inf[j].hi) * 0.5f);
                    lo = key < lo ? key : lo;
                    hi = key > hi ? key : hi;
                    group[ng++] = (uint64_t)key << 32 | (uint32_t)(j | 1 << 24);
                }
            }
        }
    }
    PROF_STOP(PZ_TRANS_CUTS, p1);
    PROF_START(p2);
    radix_sort_groups(group, group_tmp, ng, lo, hi);
    PROF_STOP(PZ_TRANS_GROUP, p2);
    PROF_START(p3);
    for (i = m = 0; i < ng; i++) {
        uint32_t g = (uint32_t)group[i];
        int end = (int)(g & 0xFFFF) + (int)(g >> 24);
        if (g & 0x800000u) {
            if (m < MAX_ITEMS) {
                trans_order[m] = -1 - (int)(g & 0xFFFF);
                tri_k0[m] = L->gpu_k0[g & 0xFFFF];
                tri_k1[m] = L->gpu_k1[g & 0xFFFF];
                m++;
            }
            continue;
        }
        if (g & 0x400000u) {
            for (j = g & 0xFFFF; j < end && m < MAX_ITEMS; j++) {
                trans_order[m] = STRIP_PART + j;
                tri_k0[m] = part_ks[j] & 15;
                tri_k1[m] = part_ks[j] >> 4;
                m++;
            }
            continue;
        }
        for (j = g & 0xFFFF; j < end; j++) {
            if (!(inf[j].flags & (RD_TRI_CULLED | RD_TRI_UNSEEN))) {
                trans_order[m] = j;
                tri_k0[m] = tri_ks[j] & 15;
                tri_k1[m] = tri_ks[j] >> 4;
                m++;
            }
        }
    }
    ntrans = m;
    PROF_STOP(PZ_TRANS_ORDER, p3);
}

static void tri_slices(int list) {
    int i, k = 0;
    for (i = 0; i < fr->ntris[list]; i++) {
        const RD_TRI_INFO* f = &fr->info[list][i];
        if (f->flags & RD_TRI_CULLED) {
            tri_k0[i] = 1, tri_k1[i] = 0;
        } else {
            int32_t a = z_key(f->lo), b = z_key(f->hi);
            /* most start in the slice the one before did */
            if (!keys_rise || a < slice_key[k] || (k < L->nslices - 1 && a >= slice_key[k + 1])) {
                k = slice_find(a);
            }
            tri_k0[i] = (uint8_t)k;
            /* and end in the slice they start in */
            tri_k1[i] = (uint8_t)(b >= a && (k == L->nslices - 1 || b < slice_key[k + 1]) ? k : slice_find(b));
        }
    }
}

/* by_slice, in order within each; 0 when there are too many */
static int bucket(int n) {
    int count[MAX_SLICES + 1] = { 0 }, i, k, total = 0;
    for (i = 0; i < n; i++) {
        for (k = tri_k0[i]; k <= tri_k1[i]; k++) {
            count[k]++;
        }
        total += tri_k0[i] <= tri_k1[i] ? tri_k1[i] - tri_k0[i] + 1 : 0;
    }
    if (total > (int)(sizeof by_slice / sizeof *by_slice)) {
        return 0;
    }
    slice_at[0] = 0;
    for (k = 0; k < L->nslices; k++) {
        slice_at[k + 1] = slice_at[k] + count[k];
        count[k] = slice_at[k];
    }
    for (i = 0; i < n; i++) {
        for (k = tri_k0[i]; k <= tri_k1[i]; k++) {
            by_slice[count[k]++] = i;
        }
    }
    return 1;
}

/* the GPU's models have float24's depth, about 2^-16 of a slice off: the CPU's translucent triangles a little nearer,
   so that a decal on a model stays on it. The select screen's sphere has its frames 2^-13.5 of a slice behind its
   tiles, and the frames are left to the CPU when their group is deep. Sprites are at least 2^-11 apart (SLICE_SPAN) */
#define TRANS_BIAS (1.0 / 32768)
static double trans_bias = TRANS_BIAS;
static double base_kept[MAX_SLICES];

/* each list slice by slice, far first */
/* The lists are built in stages, in this order: the slices, the opaque and punch-through lists, the translucent
   sort, the translucent list, and the groups' working out */
enum { STAGE_SLICES, STAGE_ADD, STAGE_SORT, STAGE_TRANS, STAGE_PREP, STAGES };
_Static_assert(STAGES == 5, "a profile zone for each stage on core 1");

static void stage_slices(void) {
    int i, k;
    PROF_START(p0);
    L->nverts = L->nidx = 0;
    L->nbatches = 0;
    L->ngprep = 0;
    prev.end = -1;
    background_tris();
    make_slices();
    for (k = 0; k < L->nslices; k++) {
        double span = L->slice_lo[k + 1] - L->slice_lo[k];
        L->slice_scale[k] = (1.0 - 2.0 / 4096) / span;
        L->slice_base[k] = 1.0 / 4096 - L->slice_lo[k] * L->slice_scale[k];
        float from = (float)L->slice_lo[k];
        if ((double)from < L->slice_lo[k]) {
            from = nextafterf(from, INFINITY);
        }
        slice_key[k] = from == from ? z_key(from) : INT32_MAX;
    }
    keys_rise = 1;
    for (k = 1; k < L->nslices; k++) {
        keys_rise &= slice_key[k - 1] <= slice_key[k];
    }
    for (i = 0; i < fr->ngroups; i++) {
        if (gpu_lo[i] > 0.0f) {
            L->gpu_k0[i] = (uint8_t)slice_of(gpu_lo[i]);
            L->gpu_k1[i] = (uint8_t)slice_of(gpu_hi[i]);
        } else {
            L->gpu_k0[i] = 1;
            L->gpu_k1[i] = 0;
        }
    }
    PROF_STOP(PZ_LIST_SLICES, p0);
}

static void stage_add(void) {
    int list, i, k;
    for (list = 0; list < RD_TRANS; list++) {
        const RD_TRI* tris = fr->tris[list];
        int n = fr->ntris[list];
        int sorted;
        PROF_START(p1);
        tri_slices(list);
        sorted = bucket(n);
        PROF_STOP(PZ_LIST_BUCKET, p1);
        PROF_START(p2);
        PFZ_START(q2);
        TEX_MARK(x2);
        for (k = 0; k < L->nslices; k++) {
            for (i = 0; list == RD_OPAQUE && i < L->nbg; i++) {
                add_tri(k, &bg_tri[i], bg_lo[i], bg_hi[i]);
            }
            if (sorted && indexed) {
                if (fr->ngroups > 0) {
                    add_groups(k, list, tris, slice_at[k], slice_at[k + 1]);
                } else {
                    add_slice(k, list, tris, slice_at[k], slice_at[k + 1]);
                }
                continue;
            }
            if (sorted) {
                for (i = slice_at[k]; i < slice_at[k + 1]; i++) {
                    add_tri(k, &tris[by_slice[i]], fr->info[list][by_slice[i]].lo, fr->info[list][by_slice[i]].hi);
                }
                add_groups(k, list, NULL, 0, 0);
                continue;
            }
            for (i = 0; i < n; i++) {
                if (tri_k0[i] <= k && k <= tri_k1[i]) {
                    add_tri(k, &tris[i], fr->info[list][i].lo, fr->info[list][i].hi);
                }
            }
            add_groups(k, list, NULL, 0, 0);
        }
        /* the late groups together, each in its slices in turn */
        for (i = 0; i < fr->ngroups; i++) {
            if (gpu_late[i] && gpu_list[i] == list) {
                for (k = L->gpu_k0[i]; k <= L->gpu_k1[i]; k++) {
                    add_gpu(k, i);
                }
            }
        }
        PROF_STOP(PZ_LIST_ADD, p2);
        PFZ_STOP(PFZ_ADD, q2);
        TEX_SINCE(PZ_ADD_TEX, x2);
    }
}

static void stage_sort(void) {
    uint64_t t0, t1;
    PFZ_START(q0);
    t0 = svcGetSystemTick();
    if (trans_strips) {
        sort_trans_strips();
    } else {
        sort_trans();
    }
    t1 = svcGetSystemTick();
    ctr_render_ticks[CTR_TICKS_SORT] += t1 - t0;
    PROF_ADD(PZ_LIST_SORT, t1 - t0);
    PROF_COUNT(PZ_LIST_SORT, (uint32_t)nparts);    /* the parts cut */
    PFZ_STOP(PFZ_TRANS_SORT, q0);
}

static int trans_fast;              /* the translucent triangles that need no cutting put as add_slice does; not with
                                       MVC2_TRANS_FAST=0 */

static void stage_trans(void) {
    int i, k;
    uint64_t t1 = svcGetSystemTick();
    PFZ_START(q1);
    TEX_MARK(x1);
    if (fr->ngroups > 0) {
        memcpy(base_kept, L->slice_base, sizeof base_kept);
        for (k = 0; k < L->nslices; k++) {
            L->slice_base[k] += trans_bias;
        }
    }
    if (trans_strips && trans_fast && indexed) {
        const RD_TRI* tris = fr->tris[RD_TRANS];
        const RD_TRI_INFO* inf = fr->info[RD_TRANS];
        int sorted = bucket(ntrans);
        FAST f;
        for (k = 0; k < L->nslices; k++) {
            int from = sorted ? slice_at[k] : 0, to = sorted ? slice_at[k + 1] : ntrans;
            fast_begin(&f, k);
            for (i = from; i < to; i++) {
                int at = sorted ? by_slice[i] : i, j = trans_order[at];
                if (!sorted && !(tri_k0[at] <= k && k <= tri_k1[at])) {
                    continue;
                }
                if (j < 0) {
                    add_gpu(k, -1 - j);
                    f.run = NULL;
                } else if (j >= STRIP_PART) {
                    add_tri(k, &trans_part[j - STRIP_PART], part_lo[j - STRIP_PART], part_hi[j - STRIP_PART]);
                    f.run = NULL;
                } else {
                    fast_tri(&f, &tris[j], inf[j].lo, inf[j].hi);
                }
            }
        }
    } else if (trans_strips) {
        const RD_TRI* tris = fr->tris[RD_TRANS];
        const RD_TRI_INFO* inf = fr->info[RD_TRANS];
        int sorted = bucket(ntrans);
        for (k = 0; k < L->nslices; k++) {
            int from = sorted ? slice_at[k] : 0, to = sorted ? slice_at[k + 1] : ntrans;
            for (i = from; i < to; i++) {
                int at = sorted ? by_slice[i] : i, j = trans_order[at];
                if (!sorted && !(tri_k0[at] <= k && k <= tri_k1[at])) {
                    continue;
                }
                if (j < 0) {
                    add_gpu(k, -1 - j);
                } else if (j >= STRIP_PART) {
                    add_tri(k, &trans_part[j - STRIP_PART], part_lo[j - STRIP_PART], part_hi[j - STRIP_PART]);
                } else {
                    add_tri(k, &tris[j], inf[j].lo, inf[j].hi);
                }
            }
        }
    } else if (bucket(ntrans)) {
        for (k = 0; k < L->nslices; k++) {
            FAST f;
            fast_begin(&f, k);
            for (i = slice_at[k]; i < slice_at[k + 1]; i++) {
                int j = trans_order[by_slice[i]];
                if (trans_fast && indexed) {
                    fast_tri(&f, titem[j].tri, titem[j].tlo, titem[j].thi);
                } else {
                    add_tri(k, titem[j].tri, titem[j].tlo, titem[j].thi);
                }
            }
        }
    } else {
        for (k = 0; k < L->nslices; k++) {
            for (i = 0; i < ntrans; i++) {
                if (tri_k0[i] <= k && k <= tri_k1[i]) {
                    int j = trans_order[i];
                    add_tri(k, titem[j].tri, titem[j].tlo, titem[j].thi);
                }
            }
        }
    }
    if (fr->ngroups > 0) {
        memcpy(L->slice_base, base_kept, sizeof base_kept);
    }
    ctr_render_ticks[CTR_TICKS_TRANS] += svcGetSystemTick() - t1;
    PROF_ADD(PZ_LIST_TRANS, svcGetSystemTick() - t1);
    PROF_COUNT(PZ_LIST_TRANS, (uint32_t)ntrans);   /* the translucent triangles and groups sorted */
    TEX_SINCE(PZ_TRANS_TEX, x1);
    PFZ_STOP(PFZ_TRANS, q1);
}

static void build_stage(int stage) {
    switch (stage) {
    case STAGE_SLICES:
        stage_slices();
        break;
    case STAGE_ADD:
        stage_add();
        break;
    case STAGE_SORT:
        stage_sort();
        break;
    case STAGE_TRANS:
        stage_trans();
        break;
    default:
        if (group_prep) {
            prep_groups();
        }
        break;
    }
}

static void build(void) {
    int stage;
#ifdef MVC2_PROF
    if (perf_zones) {
        pfz_frame();
    }
#endif
    PFZ_START(q0);
    for (stage = 0; stage < STAGES; stage++) {
        build_stage(stage);
    }
    PFZ_STOP(PFZ_LISTS, q0);
}

/* the PowerVR2's shading instruction with the texel (t) and vertex colour (p) */
static void shading_stage(const RD_STATE* s, int pal) {
    GPU_TEVSRC t = pal ? GPU_FRAGMENT_SECONDARY_COLOR : GPU_TEXTURE0;
    C3D_TexEnv* e;
    e = &env_next[0];
    env_new |= 1;
    C3D_TexEnvInit(e);
    C3D_TexEnvColor(e, 0xFFFFFFFF);
    if (s->tex == NULL) {
        C3D_TexEnvSrc(e, C3D_RGB, GPU_PRIMARY_COLOR, 0, 0);
        C3D_TexEnvFunc(e, C3D_RGB, GPU_REPLACE);
        C3D_TexEnvSrc(e, C3D_Alpha, s->use_alpha ? GPU_PRIMARY_COLOR : GPU_CONSTANT, 0, 0);
        C3D_TexEnvFunc(e, C3D_Alpha, GPU_REPLACE);
    } else {
        GPU_TEVSRC ta = s->tex_alpha ? t : GPU_CONSTANT, pa = s->use_alpha ? GPU_PRIMARY_COLOR : GPU_CONSTANT;
        switch (s->shading) {
        case RD_DECAL:
            C3D_TexEnvSrc(e, C3D_RGB, t, 0, 0);
            C3D_TexEnvFunc(e, C3D_RGB, GPU_REPLACE);
            C3D_TexEnvSrc(e, C3D_Alpha, ta, 0, 0);
            C3D_TexEnvFunc(e, C3D_Alpha, GPU_REPLACE);
            break;
        case RD_MODULATE:
            C3D_TexEnvSrc(e, C3D_RGB, t, GPU_PRIMARY_COLOR, 0);
            C3D_TexEnvFunc(e, C3D_RGB, GPU_MODULATE);
            C3D_TexEnvSrc(e, C3D_Alpha, ta, 0, 0);
            C3D_TexEnvFunc(e, C3D_Alpha, GPU_REPLACE);
            break;
        case RD_DECAL_ALPHA:
            if (s->tex_alpha) {
                C3D_TexEnvSrc(e, C3D_RGB, t, GPU_PRIMARY_COLOR, t);
                C3D_TexEnvOpRgb(e, GPU_TEVOP_RGB_SRC_COLOR, GPU_TEVOP_RGB_SRC_COLOR, GPU_TEVOP_RGB_SRC_ALPHA);
                C3D_TexEnvFunc(e, C3D_RGB, GPU_INTERPOLATE);
            } else {
                C3D_TexEnvSrc(e, C3D_RGB, t, 0, 0);
                C3D_TexEnvFunc(e, C3D_RGB, GPU_REPLACE);
            }
            C3D_TexEnvSrc(e, C3D_Alpha, pa, 0, 0);
            C3D_TexEnvFunc(e, C3D_Alpha, GPU_REPLACE);
            break;
        default:
            C3D_TexEnvSrc(e, C3D_RGB, t, GPU_PRIMARY_COLOR, 0);
            C3D_TexEnvFunc(e, C3D_RGB, GPU_MODULATE);
            C3D_TexEnvSrc(e, C3D_Alpha, ta, pa, 0);
            C3D_TexEnvFunc(e, C3D_Alpha, GPU_MODULATE);
            break;
        }
    }
}

/* the shading, then the offset added; with dry set, only whether that changes anything */
static int combine(const RD_STATE* s, int pal, uint32_t offset, int dry) {
    int key = (s->tex != NULL) | pal << 1 | s->shading << 2 | s->use_alpha << 4 | s->tex_alpha << 5 | s->offset << 6;
    C3D_TexEnv* e;
    int stage1;
    if (cur.valid && cur.combine == key && (!s->offset || cur.offset == offset)) {
        return 0;
    }
    if (dry) {
        return 1;
    }
    /* stage 0 by the key, stage 1 adds the offset */
    stage1 = !cur.valid || (cur.combine >> 6 & 1) != s->offset || (s->offset && cur.offset != offset);
    cur.offset = offset;
    if (!cur.valid || cur.combine != key) {
        cur.combine = key;
        shading_stage(s, pal);
    }
    if (!stage1) {
        return 1;
    }
    e = &env_next[1];
    env_new |= 2;
    C3D_TexEnvInit(e);
    if (s->offset) {
        /* the constant is ABGR */
        C3D_TexEnvColor(e, 0xFF000000u | (offset & 0xFF) << 16 | (offset & 0xFF00) | (offset >> 16 & 0xFF));
        C3D_TexEnvSrc(e, C3D_RGB, GPU_PREVIOUS, GPU_CONSTANT, 0);
        C3D_TexEnvFunc(e, C3D_RGB, GPU_ADD);
        C3D_TexEnvSrc(e, C3D_Alpha, GPU_PREVIOUS, 0, 0);
        C3D_TexEnvFunc(e, C3D_Alpha, GPU_REPLACE);
    }
    return 1;
}

static C3D_FogLut fog_lut;
static int fog_on;          /* the table is not all 0 */

/* rd_soft.c's fog_value */
static float fog_value(float z) {
    float q = rf->fog.density * z, m, pos;
    uint32_t bits;
    int e, i;
    if (!(q >= 1.0f)) {
        q = 1.0f;
    } else if (q > 255.9999f) {
        q = 255.9999f;
    }
    memcpy(&bits, &q, 4);
    e = (int)(bits >> 23) - 126;
    bits = (bits & 0x007FFFFFu) | 0x3F000000u;
    memcpy(&m, &bits, 4);
    pos = (m * 2.0f - 1.0f) * 16.0f + (float)((e - 1) * 16);
    i = (int)pos;
    if (i >= 127) {
        return rf->fog.table[127];
    }
    return rf->fog.table[i] + (rf->fog.table[i + 1] - rf->fog.table[i]) * (pos - (float)i);
}

/* the GPU's table, by the depth buffer's value: through the slices back to 1/w and the game's table */
static void fog_setup(void) {
    float data[256], v[129];
    int i;
    fog_on = 0;
    for (i = 0; i < 128; i++) {
        fog_on |= rf->fog.table[i] != 0.0f;
    }
    if (!fog_on) {
        return;
    }
    for (i = 0; i <= 128; i++) {
        double depth = (double)i / 128 * L->nslices;
        int k = (int)depth < L->nslices ? (int)depth : L->nslices - 1;
        double z = (depth - k - L->slice_base[k]) / L->slice_scale[k];
        v[i] = 1.0f - fog_value((float)z);
    }
    for (i = 0; i < 128; i++) {
        data[i] = v[i];
        data[128 + i] = v[i + 1] - v[i];
    }
    FogLut_FromArray(&fog_lut, data);
    C3D_FogLutBind(&fog_lut);
    fog_lut_new = 1;
    C3D_FogColor((rf->fog.color & 0xFF) << 16 | (rf->fog.color & 0xFF00) | (rf->fog.color >> 16 & 0xFF));
}

/* s set on the GPU, st its texture when it has one; with dry set, only whether that changes anything */
static int apply(const RD_STATE* s, const STATE_TEX* st, uint32_t offset, int dry) {
    /* punch-through and translucent: not behind, and only punch-through writes the depth */
    int func = depth_func[s->list == RD_OPAQUE ? s->depth & 7 : RD_GEQUAL];
    int zwrite = s->list == RD_OPAQUE ? s->zwrite : s->list == RD_PUNCH;
    int blend = s->list == RD_TRANS ? s->src << 8 | s->dst : -1;
    int alpha_test = s->list == RD_PUNCH, pal = 0, corner = 0;
    if (!cur.valid || cur.func != func || cur.zwrite != zwrite) {
        if (dry) {
            return 1;
        }
        fx_depth_test(func, GPU_WRITE_RED | GPU_WRITE_GREEN | GPU_WRITE_BLUE | (zwrite ? GPU_WRITE_DEPTH : 0));
        cur.func = func;
        cur.zwrite = zwrite;
    }
    if (!cur.valid || cur.alpha_test != alpha_test) {
        if (dry) {
            return 1;
        }
        fx_alpha_test(alpha_test, GPU_GEQUAL, 128);
        cur.alpha_test = alpha_test;
    }
    if (!cur.valid || cur.blend != blend) {
        if (dry) {
            return 1;
        }
        if (blend < 0) {
            fx_blend(GPU_ONE, GPU_ZERO);
        } else {
            fx_blend(blend_factor(s->src, 1), blend_factor(s->dst, 0));
        }
        cur.blend = blend;
    }
    if (s->tex != NULL) {
        pal = st->pal;
        if (st->tex >= 0) {
            C3D_Tex* tex = &rf->tex[st->tex];
            int filter;
            corner = corner_ok && !s->bilinear && !pal;
            filter = (s->bilinear || corner) && !pal;
            if (cur.tex != tex || cur.filter != filter || cur.wrap != s->uv_mode || cur.mip_d != s->mip_d) {
                if (dry) {
                    return 1;
                }
                /* the D adjust, as rd_soft */
                static const float bias[16] = { 0, -4, -2, -1 };
                C3D_TexSetLodBias(tex, bias[s->mip_d & 15]);
                /* clamp (1 v, 2 u) and flip (4 v, 8 u) */
                C3D_TexSetWrap(tex, s->uv_mode & 2 ? GPU_CLAMP_TO_EDGE : s->uv_mode & 8 ? GPU_MIRRORED_REPEAT : GPU_REPEAT,
                               s->uv_mode & 1 ? GPU_CLAMP_TO_EDGE : s->uv_mode & 4 ? GPU_MIRRORED_REPEAT : GPU_REPEAT);
                C3D_TexSetFilter(tex, filter ? GPU_LINEAR : GPU_NEAREST, filter ? GPU_LINEAR : GPU_NEAREST);
                cur.wrap = s->uv_mode;
                cur.mip_d = s->mip_d;
                bind_tex = tex;
                cur.tex = tex;
                cur.filter = filter;
            }
        }
    }
    if (!cur.valid || cur.corner != corner) {
        if (dry) {
            return 1;
        }
        cur.corner = corner;
    }
    {
        /* FOG_TABLE2 is drawn as FOG_TABLE */
        int fog = fog_on && (s->fog == RD_FOG_TABLE || s->fog == RD_FOG_TABLE2);
        if (!cur.valid || cur.fog != fog) {
            if (dry) {
                return 1;
            }
            fog_mode(fog);
            cur.fog = fog;
        }
    }
    if (combine(s, pal, offset, dry) && dry) {
        return 1;
    }
    if (light_pal && (!cur.valid || cur.lit != pal)) {
        if (dry) {
            return 1;
        }
        GPUCMD_AddWrite(GPUREG_LIGHTING_ENABLE0, (u32)pal);
        GPUCMD_AddWrite(GPUREG_LIGHTING_ENABLE1, (u32)!pal);
        cur.lit = pal;
    }
    if (pal) {
        int n = st->pal16 ? 16 : 256;
        if (cur.lut_base != s->palette || cur.lut_n != n || cur.lut_serial != rf->palette_serial) {
            if (dry) {
                return 1;
            }
            lut_palette(s->palette, n);
            cur.lut_base = s->palette;
            cur.lut_n = n;
            cur.lut_serial = rf->palette_serial;
        }
    }
    cur.valid = 1;
    return 0;
}

#ifdef MVC2_PROF
/* MVC2_DRAW_CRC=1: a running hash of the draws' command words, logged every 600 frames, for exactness checks */
static int draw_crc;

static uint32_t crc_h = 2166136261u;
static int crc_skip;

/* the words from from to the list's end; with MVC2_LIST_SPLIT once a part, the words the split adds left out */
static void crc_add(const u32* from) {
    const u32 *p, *end = gpuCmdBuf + gpuCmdBufOffset;
    if (end < from) {
        crc_skip = 1;
        return;
    }
    for (p = from; p < end; p++) {
        crc_h = (crc_h ^ *p) * 16777619u;
    }
}

static void crc_frame(const u32* from) {
    static uint32_t frames, skipped;
    crc_add(from);
    skipped += (uint32_t)crc_skip;
    crc_skip = 0;
    if (++frames % 600 == 0) {
        host_log("draw crc: frames %lu, %08lx (%lu not hashed)", (unsigned long)frames, (unsigned long)crc_h,
                 (unsigned long)skipped);
    }
}
#endif

static const GV* bound_verts;
static u32 idx_offset;              /* idx's place for the GPU, from the buffers' base; 0 if it has none */

#define CMD(reg, mask, v) (w[0] = (v), w[1] = GPUCMD_HEADER(0, (mask), (reg)), w += 2)

static void c3d_restore(void);

/* the next draw's texture and combiner given to citro3d */
static void c3d_pending(void) {
    int i;
    c3d_restore();
    for (i = 0; i < 2; i++) {
        if (env_new >> i & 1) {
            *C3D_GetTexEnv(i) = env_next[i];
        }
    }
    if (bind_tex != NULL) {
        C3D_TexBind(0, bind_tex);
    }
    env_new = 0;
    bind_tex = NULL;
}

/* The GPU's commands for a draw that changes nothing citro3d keeps but the effect, the texture, the fog and the
   combiner, as citro3d sends them: the effect block, unit 0's texture as its update binds it, the fog, the stages,
   then C3D_DrawElements' own. 0 when they don't fit, or when citro3d would send a new fog table, for it to do */
enum { RAW_STATE_WORDS = FX_WORDS + 14 + FOG_WORDS + 12, RAW_TAIL_WORDS = 30 };

static int raw_ok(const C3D_Tex* t) {
    return !((t != NULL && (t->param >> 28 & 7) != GPU_TEX_2D) || (fx_new && !fx_known) || (fog_new && !fog_known) ||
             (fog_lut_new && (fog[0] & 7) != GPU_NO_FOG));
}

/* the changed state's words */
static u32* raw_state(u32* w, const C3D_Tex* t) {
    int i;
    if (fx_new) {
        if (draw_copy) {
            copy_fixed(w, fx, sizeof fx);
        } else {
            memcpy(w, fx, sizeof fx);
        }
        w += FX_WORDS;
    }
    if (t != NULL) {
        w[0] = t->border;
        w[1] = GPUCMD_HEADER(1u, 0xF, GPUREG_TEXUNIT0_BORDER_COLOR) | 4 << 20;   /* 5 registers from there */
        w[2] = t->dim;
        w[3] = t->param;
        w[4] = t->lodParam;
        w[5] = osConvertVirtToPhys(t->data) >> 3;
        w += 6;
        CMD(GPUREG_TEXUNIT0_TYPE, 0xF, t->fmt);
        CMD(GPUREG_TEXUNIT_CONFIG, 0xB, 0x11001);   /* unit 0 on, and its cache cleared below */
        CMD(GPUREG_TEXUNIT_CONFIG, 0x4, 0x10000);
        CMD(GPUREG_TEXUNIT0_SHADOW, 0xF, 1);
    }
    if (fog_new) {
        memcpy(w, fog, sizeof fog);
        w += FOG_WORDS;
    }
    for (i = 0; i < 2; i++) {
        if (env_new >> i & 1) {
            const u32* e = (const u32*)&env_next[i];
            w[0] = e[0];
            w[1] = GPUCMD_HEADER(1u, 0xF, GPUREG_TEXENV0_SOURCE + i * 8) | 4 << 20;
            w[2] = e[1];
            w[3] = e[2];
            w[4] = e[3];
            w[5] = e[4];
            w += 6;
        }
    }
    env_new = 0;
    bind_tex = NULL;
    fx_new = 0;
    fog_new = 0;
    return w;
}

/* C3D_DrawElements' own words */
static u32* raw_tail(u32* w, u32 index_at, u32 count) {
    CMD(GPUREG_PRIMITIVE_CONFIG, 0x2, GPU_GEOMETRY_PRIM);
    CMD(GPUREG_RESTART_PRIMITIVE, 0xF, 1);
    CMD(GPUREG_INDEXBUFFER_CONFIG, 0xF, index_at | (u32)C3D_UNSIGNED_SHORT << 31);
    CMD(GPUREG_NUMVERTICES, 0xF, count);
    CMD(GPUREG_VERTEX_OFFSET, 0xF, 0);
    CMD(GPUREG_GEOSTAGE_CONFIG, 0x2, 0x100);
    CMD(GPUREG_GEOSTAGE_CONFIG2, 0x2, 0x100);
    CMD(GPUREG_START_DRAW_FUNC0, 0x1, 0);
    CMD(GPUREG_DRAWELEMENTS, 0xF, 1);
    CMD(GPUREG_START_DRAW_FUNC0, 0x1, 1);
    CMD(GPUREG_GEOSTAGE_CONFIG, 0x2, 0);
    CMD(GPUREG_GEOSTAGE_CONFIG2, 0x2, 0);
    CMD(GPUREG_VTX_FUNC, 0xF, 1);
    CMD(GPUREG_PRIMITIVE_CONFIG, 0x8, 0);
    CMD(GPUREG_PRIMITIVE_CONFIG, 0x8, 0);
    return w;
}

static int raw_draw(const C3D_Tex* t, u32 index_at, int count) {
    u32* w = gpuCmdBuf + gpuCmdBufOffset;
    if (gpuCmdBufOffset + RAW_STATE_WORDS - (t != NULL ? 0 : 14) + RAW_TAIL_WORDS > gpuCmdBufSize || !raw_ok(t)) {
        return 0;
    }
    w = raw_tail(raw_state(w, t), index_at, (u32)count);
    gpuCmdBufOffset = (u32)(w - gpuCmdBuf);
    return 1;
}

/* a mesh's words: its vertices, then each run's culling and draw */
static u32* mesh_frag(u32* w, const GMESH* g) {
    int r;
    if (raw_bufs) {
        w = buf_setup(w, g->v, MESH_STRIDE);
        if (frag_call) {
            CMD(GPUREG_FACECULLING_CONFIG, 0x0, 0);     /* a write of nothing: the call's words stay 16-byte lines */
        }
    } else {
        CMD(GPUREG_ATTRIBBUFFER0_OFFSET, 0xF, g->v_at);
    }
    for (r = 0; r < g->nruns; r++) {
        CMD(GPUREG_FACECULLING_CONFIG, 0xF, (u32)g->run[r].cull & 3);
        w = raw_tail(w, g->idx_at + (u32)g->run[r].first * 6, (u32)g->run[r].count * 3);
    }
    return w;
}

static int tl_now;                  /* the GPU set for groups: their vertices, culling and uniforms */

/* MVC2_FRAG_CALL=1: a mesh's words called where they are, not copied into the list. The GPU jumps to them on its
   second buffer and back on its first, each part of the list 16-byte aligned and its size given exactly: a part's
   size is written when the jump that ends it is. The list's last part ends at the frame's end, in a jump to a tail of
   its own with citro3d's last words, so that no size is guessed. It hangs the real PICA (not Azahar): off */
#define FRAG_RESERVE 16             /* the words kept for the jump to the tail */
static u32* frag_size0;             /* the size of the part after the last call, to be written */
static u32* frag_ret;               /* that part's first word */
static u32* frag_tail;              /* framebuffer flush, invalidate, early depth clear, finalize */
static u32 frag_tail_at;
static u32* cmd_base;               /* the list's buffer and its physical address */
static u32 cmd_base_at;

static u32 cmd_phys(const u32* w) {
    if (cmd_base != gpuCmdBuf) {
        cmd_base = gpuCmdBuf;
        cmd_base_at = osConvertVirtToPhys(gpuCmdBuf);
    }
    return cmd_base_at + (u32)((const uint8_t*)w - (const uint8_t*)gpuCmdBuf);
}

/* a word that writes nothing, until the words after it end on a 16-byte line */
static u32* cmd_pad(u32* w, int after) {
    if ((cmd_phys(w) + (u32)after * 4) & 15) {
        CMD(GPUREG_FACECULLING_CONFIG, 0x0, 0);
    }
    return w;
}

/* the jump that ends the part under way to addr, size bytes, on buffer b; the part's size written */
static u32* cmd_jump(u32* w, int b, u32 addr, u32 size) {
    w = cmd_pad(w, 6);
    CMD(b ? GPUREG_CMDBUF_ADDR1 : GPUREG_CMDBUF_ADDR0, 0xF, addr >> 3);
    CMD(b ? GPUREG_CMDBUF_SIZE1 : GPUREG_CMDBUF_SIZE0, 0xF, size >> 3);
    CMD(b ? GPUREG_CMDBUF_JUMP1 : GPUREG_CMDBUF_JUMP0, 0xF, 1);
    if (frag_size0 != NULL) {
        *frag_size0 = (u32)(w - frag_ret) * 4 >> 3;
    }
    return w;
}

/* a mesh's words called, the GPU back at the next word */
static u32* frag_jump(u32* w, u32 frag_at, u32 frag_words) {
    u32* size0;
    if (frag_size0 == NULL) {
        gpuCmdBufSize -= FRAG_RESERVE;
    }
    w = cmd_pad(w, 10);
    CMD(GPUREG_CMDBUF_ADDR0, 0xF, cmd_phys(w + 10) >> 3);     /* the word after the jump */
    size0 = w;
    CMD(GPUREG_CMDBUF_SIZE0, 0xF, 0);
    w = cmd_jump(w, 1, frag_at, (frag_words + 2) * 4);
    frag_ret = w;
    frag_size0 = size0;
    return w;
}

/* citro3d's frame end: the list's last part ends in the jump to the tail */
static void frag_end(void* unused) {
    (void)unused;
    if (frag_size0 != NULL) {
        u32* w;
        gpuCmdBufSize += FRAG_RESERVE;
        w = cmd_jump(gpuCmdBuf + gpuCmdBufOffset, 0, frag_tail_at, 8 * 4);
        gpuCmdBufOffset = (u32)(w - gpuCmdBuf);
        frag_size0 = NULL;
    }
}

/* MVC2_GPU_WATCH=1: a hang logged with the last frame's model draws and command list, then the program
   stopped, instead of a frozen console */
typedef struct {
    u32 cmd;                /* its first word in the frame's command list */
    u32 v, idx, count;      /* vertices and indices from the buffers' base, and the indices drawn */
    u32 mesh_size;
    float scale, reach;     /* its slice's depth scale, and the largest clip coordinate it can make */
    u16 group, mesh;
    u8 kind, cull, raw, lo, hi, nruns, slice;
    u16 bools;
} WATCH_DRAW;
#define WATCH_DRAWS 1024
#define HANG_FILE "sdmc:/3ds/mvc2/gpuhang.bin"
#define HANG_TEXT "sdmc:/3ds/mvc2/gpuhang.txt"     /* added to, never cut */
static int gpu_watch;
static WATCH_DRAW* wdraw;
static int nwdraw;
static u32* frame_cmd;              /* the frame's command list, and its words up to C3D_FrameEnd's own */
static u32 frame_words;
static volatile u32 watch_beat;     /* frames drawn */
static const char* volatile watch_phase = "starting";

/* a line in the log and in HANG_TEXT */
static void hang_line(FILE* f, const char* fmt, ...) {
    char line[320];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(line, sizeof line, fmt, ap);
    va_end(ap);
    host_log("%s", line);
    if (f != NULL) {
        fprintf(f, "%s\n", line);
    }
}

static gxCmdQueue_s* c3d_queue;         /* citro3d's queue, as it binds it */

/* citro3d's queue: its commands' ids, sent and done */
static void gpu_queue(FILE* t, const char* head) {
    char line[160];
    int i, n = 0;
    line[0] = '\0';
    if (c3d_queue != NULL) {
        for (i = 0; i < c3d_queue->numEntries && i < 32 && n < (int)sizeof line - 4; i++) {
            n += snprintf(line + n, sizeof line - (size_t)n, " %lu", (unsigned long)(c3d_queue->entries[i].data[0] & 0xFF));
        }
        hang_line(t, "%scitro3d's queue %u entries (commands%s), %u sent, %u done", head, (unsigned)c3d_queue->numEntries,
                  line, (unsigned)c3d_queue->curEntry, (unsigned)c3d_queue->lastEntry);
    }
}

/* MVC2_WATCH_REGS=1: the GPU's registers for the fills, its busy flags, the transfer, its interrupts and its command
   list, read through the GPU service. Off: on a stuck PICA the read never returns, and the system's core with it */
static int watch_regs;

static void gpu_regs(FILE* t) {
    static const u32 reg_at[5] = { 0x400010, 0x400030, 0x400C00, 0x401000, 0x4018E0 };   /* from 0x1EB00000 */
    u32 regs[8];
    int i;
    if (!watch_regs) {
        return;
    }
    for (i = 0; i < 5; i++) {
        memset(regs, 0, sizeof regs);
        GSPGPU_ReadHWRegs(reg_at[i], regs, sizeof regs);
        hang_line(t, "  registers at %08lX: %08lX %08lX %08lX %08lX %08lX %08lX %08lX %08lX", (unsigned long)(0x1EB00000 + reg_at[i]),
                  (unsigned long)regs[0], (unsigned long)regs[1], (unsigned long)regs[2], (unsigned long)regs[3],
                  (unsigned long)regs[4], (unsigned long)regs[5], (unsigned long)regs[6], (unsigned long)regs[7]);
    }
}

/* what is written so far on the card */
static void hang_sync(FILE* t) {
    if (t != NULL) {
        fflush(t);
        fsync(fileno(t));
    }
}

/* the report on the card, then the exit; nothing here waits on the GPU or its service before the files are closed */
static void watch_hung(const char* why, int cpu_too) {
    static int once;
    FILE *f, *t;
    u32 words = frame_words;
    int i;
    if (ctr_ending || __atomic_exchange_n(&once, 1, __ATOMIC_SEQ_CST)) {
        for (;;) {
            svcSleepThread(1000000000LL);
        }
    }
    if (words > CMDBUF_SIZE / 4) {
        words = CMDBUF_SIZE / 4;
    }
    t = fopen(HANG_TEXT, "a");
    {
        time_t now = time(NULL);
        char when[32];
        strftime(when, sizeof when, "%Y-%m-%d %H:%M:%S", localtime(&now));
        hang_line(t, "hung at %s: %s; drawing was %s, the game's frame %lu", when, why, watch_phase, (unsigned long)frame_no);
        hang_sync(t);
    }
    hang_line(t, "  the last frame sent: %lu command words before C3D_FrameEnd's at %08lX (physical), %d model draws",
              (unsigned long)frame_words, (unsigned long)(frame_cmd != NULL ? osConvertVirtToPhys(frame_cmd) : 0), nwdraw);
    for (i = 0; i < nwdraw && i < WATCH_DRAWS; i++) {
        const WATCH_DRAW* w = &wdraw[i];
        hang_line(t, "  draw %d at word %lu: group %u mesh %u (kind %u, %u runs, %lu bytes), %s; vertices at %08lX, "
                     "indices at %08lX, %lu of them; cull %u, uniforms %u-%u, bools %04X; slice %u, scale %g, clip "
                     "coordinates up to %g", i, (unsigned long)w->cmd, w->group, w->mesh, w->kind, w->nruns,
                  (unsigned long)w->mesh_size, w->raw ? "raw" : "citro3d", (unsigned long)w->v, (unsigned long)w->idx,
                  (unsigned long)w->count, w->cull, w->lo, w->hi, w->bools, w->slice, w->scale, w->reach);
    }
    gpu_queue(t, "  ");
    if (t != NULL) {
        hang_sync(t);
        fclose(t);
    }
    /* the words C3D_FrameEnd adds come after, in the same buffer */
    if (frame_cmd != NULL && words > 0 && (f = fopen(HANG_FILE, "wb")) != NULL) {
        fwrite(frame_cmd, 4, words + 8 <= CMDBUF_SIZE / 4 ? words + 8 : words, f);
        hang_sync(f);
        fclose(f);
        host_log("  its command list in " HANG_FILE);
    }
    ctr_log_flush();
    if (watch_regs && (t = fopen(HANG_TEXT, "a")) != NULL) {
        gpu_regs(t);
        hang_sync(t);
        fclose(t);
    }
    ctr_hung("The GPU stopped. The log and gpuhang.txt are in sdmc:/3ds/mvc2.\n\nIf HOME doesn't come back after "
             "START, hold POWER.", cpu_too);
}

/* MVC2_WATCH_TEST=frame: the report and the exit at that frame, to try them */
static unsigned long watch_test;

/* in the HOME Menu or asleep: no frames, and not a stop */
static volatile int away_home, away_sleep;
static volatile u32 away_beat;      /* watch_beat when the program was last away */
static aptHookCookie away_hook;

static int away(void) {
    if ((away_home || away_sleep) && watch_beat - away_beat >= 3) {
        away_home = away_sleep = 0;     /* back without a hook's word: the game is drawing */
    }
    return away_home || away_sleep;
}

static void away_event(APT_HookType type, void* param) {
    static const char* const what[APTHOOK_COUNT] = { "the HOME Menu", "back from the HOME Menu", "asleep", "awake",
                                                     "exiting" };
    (void)param;
    if (type == APTHOOK_ONSUSPEND || type == APTHOOK_ONSLEEP) {
        away_beat = watch_beat;
        if (type == APTHOOK_ONSUSPEND) {
            away_home = 1;
        } else {
            away_sleep = 1;
        }
    } else if (type == APTHOOK_ONRESTORE) {
        away_home = 0;
    } else if (type == APTHOOK_ONWAKEUP) {
        away_sleep = 0;
    } else if (type == APTHOOK_ONEXIT) {
        ctr_ending = 1;
    }
    if ((unsigned)type < APTHOOK_COUNT) {
        time_t now = time(NULL);
        char when[16];
        strftime(when, sizeof when, "%H:%M:%S", localtime(&now));
        host_log("apt: %s at %s, the game's frame %lu", what[type], when, (unsigned long)frame_no);
    }
}

/* the GPU's last frame waited for at most 500 ms, not counting time away */
static void watched_begin(void) {
    uint64_t start = svcGetSystemTick();
    if (watch_test != 0 && frame_no >= watch_test && !ctr_ending) {
        watch_hung("a test (MVC2_WATCH_TEST)", 0);
    }
    while (!C3D_FrameBegin(C3D_FRAME_NONBLOCK)) {
        if (away() || ctr_ending) {
            start = svcGetSystemTick();
        } else if (svcGetSystemTick() - start > SYSCLOCK_ARM11 / 2) {
            watch_hung("the GPU hasn't finished a frame in 500 ms", 0);
        }
        svcSleepThread(100000);
    }
}

/* a screen of the front end holds the game: not a stop */
void ctr_render_alive(void) {
    watch_beat++;
}

/* any other stop: no frame drawn for 10 s, not counting time away; none once the program is ending */
static void watch_main(void* arg) {
    u32 beat = watch_beat;
    int still = 0;
    (void)arg;
    while (!ctr_ending) {
        svcSleepThread(250000000LL);
        if (ctr_ending) {
            break;
        } else if (watch_beat != beat || away()) {
            beat = watch_beat;
            still = 0;
        } else if (++still == 40) {
            watch_hung("no frame drawn for 10 s", 1);
        }
    }
}
static int c3d_needed;              /* the next draw through citro3d, which sends what was set for the CPU's again */

static void bind_verts(void) {
    C3D_BufInfo* buf = C3D_GetBufInfo();
    BufInfo_Init(buf);
    BufInfo_Add(buf, L->verts, sizeof(GV), 3, 0x210);
    bound_verts = L->verts;
}

/* MVC2_RAW_BACK=1: after a run of groups, the CPU's vertex setup and bools written with the next raw draw, and given
   to citro3d again only before it next draws itself (a run of groups starting first gives it the groups' own) */
static int raw_back, back_words, c3d_stale;
enum { SETUP_WORDS = 4 + 2 + 2 + 4 + 2 + 38, BACK_WORDS = SETUP_WORDS + 2 };
/* MVC2_RAW_FRONT=1: the first group of a run sent directly too, its vertex setup and all its uniforms written here */
static int raw_front;

static void c3d_restore(void) {
    if (c3d_stale) {
        C3D_SetAttrInfo(&cpu_attr);
        bind_verts();
        C3D_BoolUnifsDirty[GPU_VERTEX_SHADER] = true;
        c3d_stale = back_words = 0;
    }
}

/* n words to consecutive registers from the header's */
static u32* cmd_run(u32* w, u32 header, const u32* p, u32 n) {
    w[0] = p[0];
    w[1] = header | (n - 1) << 20;
    memcpy(w + 2, p + 1, (n - 1) * 4);
    w += n + 1;
    if ((n - 1) & 1) {
        *w++ = 0;
    }
    return w;
}

/* the words citro3d's buffer bind sends for the vertices at v */
static u32* buf_setup(u32* w, const void* v, int stride) {
    C3D_BufInfo buf;
    BufInfo_Init(&buf);
    BufInfo_Add(&buf, v, stride, 3, 0x210);
    CMD(GPUREG_ATTRIBBUFFERS_LOC, 0xF, buf.base_paddr >> 3);
    return cmd_run(w, GPUCMD_HEADER(1u, 0xF, GPUREG_ATTRIBBUFFER0_OFFSET), (const u32*)buf.buffers, 36);
}

/* the words citro3d's attribute and buffer binds send for attr and the vertices at v */
static u32* attr_setup(u32* w, const C3D_AttrInfo* attr, const void* v, int stride) {
    u32 perm[2] = { (u32)attr->permutation, (u32)(attr->permutation >> 32) };
    w = cmd_run(w, GPUCMD_HEADER(1u, 0xF, GPUREG_ATTRIBBUFFERS_FORMAT_LOW), attr->flags, 2);
    CMD(GPUREG_VSH_INPUTBUFFER_CONFIG, 0xB, 0xA0000000u | (u32)(attr->attrCount - 1));
    CMD(GPUREG_VSH_NUM_ATTR, 0xF, (u32)(attr->attrCount - 1));
    w = cmd_run(w, GPUCMD_HEADER(1u, 0xF, GPUREG_VSH_ATTRIBUTES_PERMUTATION_LOW), perm, 2);
    return buf_setup(w, v, stride);
}

/* the CPU's, then the bools */
static u32* back_setup(u32* w) {
    w = attr_setup(w, &cpu_attr, L->verts, sizeof(GV));
    CMD(GPUREG_VSH_BOOLUNIFORM, 0xF, 0x7FFF0000u | C3D_BoolUnifs[GPU_VERTEX_SHADER]);
    return w;
}

static void tl_off(void) {
    C3D_BoolUnifs[GPU_VERTEX_SHADER] &= (u16)~(1u << (b_tl - 0x68) | 1u << (b_tl_lit - 0x68) | 1u << (b_tl_vtxcol - 0x68));
    if (raw_back) {
        back_words = c3d_stale = 1;
    } else {
        C3D_SetAttrInfo(&cpu_attr);
        bind_verts();
        C3D_BoolUnifsDirty[GPU_VERTEX_SHADER] = true;
        c3d_needed = 1;
    }
    fx_cull(GPU_CULL_NONE);
    tl_now = 0;
    ctr_render_count[CTR_COUNT_TL_RUNS]++;
}

static void bind_mesh(const void* v) {
    C3D_BufInfo* buf = C3D_GetBufInfo();
    BufInfo_Init(buf);
    BufInfo_Add(buf, v, MESH_STRIDE, 3, 0x210);
    bound_verts = NULL;
}

/* the put's matrix to the clip space in slice k: proj times the slice's (x + GAME_X, y, depth, 1) times w, whose
   depth times w, z * scale + base times w, is scale + base * w. Row 2 is that depth's two terms instead, for the shader
   to take from w: as a row of the matrix, a narrow slice's terms times the model's coordinates overflow float24 */
static uint32_t draw_serial;        /* draw()s so far */
static uint32_t prep_serial;        /* the groups' working out so far: in build() with MVC2_GROUP_PREP, else in draw() */

/* MVC2_PREP_FAST2=1, with MVC2_PREP_FAST=1: more of the same: the corner memo compared inline and the texels' area
   kept for a mesh, a put's rows tested for range as they are made, the clip tests on the bits, row 2 made once a slice */
static int prep_fast2;
static uint8_t rows_in[RD_GPU_PUTS];    /* with MVC2_PREP_FAST2, the put's rows 0, 1 and 3 in the GPU's range */

/* the floats' range tests: |x| < 1e18 as its bits, the same for every float, NaN too */
static inline int in_range(float x) {
    u32 b;
    memcpy(&b, &x, 4);
    return (b & 0x7FFFFFFFu) < 0x5D5E0B6Bu;
}

static inline int vec_in_range(const C3D_FVec* v) {
    return in_range(v->x) & in_range(v->y) & in_range(v->z) & in_range(v->w);
}

/* fast2: MVC2_PREP_FAST2's, given as a constant */
static inline __attribute__((always_inline)) void group_matrix(C3D_FVec* out, int put, int k, double base, const int fast2) {
    static C3D_FVec rows[RD_GPU_PUTS][4];
    static uint32_t made[RD_GPU_PUTS];      /* prep_serial when its rows were made */
    static C3D_FVec row2[MAX_SLICES];       /* with MVC2_PREP_FAST2, the slice's, made in row2_made */
    static uint32_t row2_made[MAX_SLICES];
    const float* m = fr->puts[put].m;
    if (made[put] != prep_serial && single) {
        /* rows 0, 1 and 3, the same in every slice: proj's are 0 for the depth */
        int i, c;
        for (i = 0; i < 4; i += i == 1 ? 2 : 1) {
            const float px = proj.r[i].x, py = proj.r[i].y, pw = proj.r[i].w;
            float r[4];
            for (c = 0; c < 4; c++) {
                float w = m[4 * c];
                r[c] = px * (m[4 * c + 1] + GAME_X * w) + py * m[4 * c + 2] + pw * w;
            }
            rows[put][i] = FVec4_New(r[0], r[1], r[2], r[3]);
        }
        made[put] = prep_serial;
        rows_in[put] = (uint8_t)(fast2 && (vec_in_range(&rows[put][0]) & vec_in_range(&rows[put][1]) &
                                           vec_in_range(&rows[put][3])));
    } else if (made[put] != prep_serial) {
        double v[4][4], r[4];
        int i, j, c;
        for (c = 0; c < 4; c++) {
            double w = m[4 * c], x = m[4 * c + 1], y = m[4 * c + 2];
            v[0][c] = x + GAME_X * w;
            v[1][c] = y;
            v[2][c] = 0.0;
            v[3][c] = w;
        }
        for (i = 0; i < 4; i += i == 1 ? 2 : 1) {
            const double p[4] = { proj.r[i].x, proj.r[i].y, 0.0, proj.r[i].w };
            for (c = 0; c < 4; c++) {
                r[c] = 0.0;
                for (j = 0; j < 4; j++) {
                    r[c] += p[j] * v[j][c];
                }
            }
            rows[put][i] = FVec4_New((float)r[0], (float)r[1], (float)r[2], (float)r[3]);
        }
        made[put] = prep_serial;
        rows_in[put] = (uint8_t)(fast2 && (vec_in_range(&rows[put][0]) & vec_in_range(&rows[put][1]) &
                                           vec_in_range(&rows[put][3])));
    }
    out[0] = rows[put][0];
    out[1] = rows[put][1];
    out[3] = rows[put][3];
    /* proj's row 2 is (0, 0, -1, 0) */
    if (!fast2) {
        out[2] = FVec4_New((float)(proj.r[2].z * base), (float)(proj.r[2].z * L->slice_scale[k]), 0.0f, 0.0f);
        return;
    }
    if (row2_made[k] != prep_serial) {
        row2[k] = FVec4_New((float)(proj.r[2].z * base), (float)(proj.r[2].z * L->slice_scale[k]), 0.0f, 0.0f);
        row2_made[k] = prep_serial;
    }
    out[2] = row2[k];
}

static C3D_FVec tl_sent[TL_UNIFS];  /* as the GPU has them */

/* MVC2_PREP_FAST=1: group prep's same sums made with fewer steps: bytes over 255 from a table, the range tests on
   the floats' bits, a slice's depth map kept for the groups drawn in the same slices, a corner test's vertices worked
   out once each and its answer kept for a mesh's last two matrices */
static int prep_fast;
static float by255[256];            /* i / 255.0f */

static inline __attribute__((always_inline)) void group_unifs(const BATCH* b, const GMESH* m, C3D_FVec* u, const int fast, const int fast2) {
    const RD_GPU_DRAW* d = &fr->groups[b->first];
    const RD_GPU_GROUP* g = &d->g;
    group_matrix(u, d->put, b->slice, L->slice_base[b->slice], fast2);
    if (m->kind == RD_GPU_VTXCOL) {
        u[4] = FVec4_New(g->face[1] / 255.0f, g->face[2] / 255.0f, g->face[3] / 255.0f, g->face[0] / 255.0f);
    } else if (fast) {
        u32 c = g->face_argb;
        u[4] = m->kind == RD_GPU_LIT ? FVec4_New(g->face[1], g->face[2], g->face[3], by255[c >> 24])
                                     : FVec4_New(by255[c >> 16 & 255], by255[c >> 8 & 255], by255[c & 255], by255[c >> 24]);
    } else if (m->kind == RD_GPU_LIT) {
        u[4] = FVec4_New(g->face[1], g->face[2], g->face[3], (float)(g->face_argb >> 24) / 255.0f);
    } else {
        u[4] = FVec4_New((float)(g->face_argb >> 16 & 255) / 255.0f, (float)(g->face_argb >> 8 & 255) / 255.0f,
                         (float)(g->face_argb & 255) / 255.0f, (float)(g->face_argb >> 24) / 255.0f);
    }
    u[5] = m->kind == RD_GPU_LIT ? FVec4_New(g->l[0] * m->nscale, g->l[1] * m->nscale, g->l[2] * m->nscale, g->ambient)
                                 : FVec4_New(0.0f, 0.0f, 0.0f, 0.0f);   /* not sent */
    u[6] = FVec4_New(rf->st[b->state].su, -rf->st[b->state].sv, 0.0f, 0.0f);
}

/* group_unifs with MVC2_PREP_PACK: the same, the face's bytes, the put and the state's scale from the record */
static void unifs_packed(const BATCH* b, const GMESH* m, const GPACK* q, C3D_FVec* u) {
    u32 c = q->face_argb;
    group_matrix(u, q->put, b->slice, L->slice_base[b->slice], 1);
    if (m->kind == RD_GPU_VTXCOL) {
        const RD_GPU_GROUP* g = &fr->groups[b->first].g;
        u[4] = FVec4_New(g->face[1] / 255.0f, g->face[2] / 255.0f, g->face[3] / 255.0f, g->face[0] / 255.0f);
        u[5] = FVec4_New(0.0f, 0.0f, 0.0f, 0.0f);
    } else if (m->kind == RD_GPU_LIT) {
        const RD_GPU_GROUP* g = &fr->groups[b->first].g;
        u[4] = FVec4_New(g->face[1], g->face[2], g->face[3], by255[c >> 24]);
        u[5] = FVec4_New(g->l[0] * m->nscale, g->l[1] * m->nscale, g->l[2] * m->nscale, g->ambient);
    } else {
        u[4] = FVec4_New(by255[c >> 16 & 255], by255[c >> 8 & 255], by255[c & 255], by255[c >> 24]);
        u[5] = FVec4_New(0.0f, 0.0f, 0.0f, 0.0f);
    }
    u[6] = FVec4_New(q->su, -q->sv, 0.0f, 0.0f);
}

static int tl_frags;                /* a group's mesh drawn with the words made with it; not with MVC2_GPU_TL_FRAGS=0 */

/* MVC2_PROF_TL=1, in a profile build: a group's draw timed in its steps */
#ifdef MVC2_PROF
static int prof_tl;                 /* MVC2_PROF_TL=n: one group in n timed, its times and counts taken n times */
static int tl_on, tl_seq;           /* this group timed */
static uint32_t tl_mark(int zone, uint32_t t, int n) {
    uint32_t now = prof_now();
    uint32_t d = now - t;
    prof_add(zone, (d > prof_read_ticks ? d - prof_read_ticks : 0u) * (uint32_t)n);
    return now;
}
#define TL_NOW() (tl_on ? prof_now() : 0u)
#define TL_MARK(zone, t) ((t) = tl_on ? tl_mark((zone), (t), prof_tl) : 0u)
#define TL_NOW1() (prof_tl ? prof_now() : 0u)
#define TL_MARK1(zone, t) ((t) = prof_tl ? tl_mark((zone), (t), 1) : 0u)
#define TL_COUNT(zone, n) (tl_on ? prof_count((zone), (uint32_t)(n) * (uint32_t)prof_tl) : (void)0)
#else
#define TL_NOW() 0u
#define TL_MARK(zone, t) ((void)(t))
#define TL_NOW1() 0u
#define TL_MARK1(zone, t) ((void)(t))
#define TL_COUNT(zone, n) ((void)(n))
#endif

static int vec_same(const C3D_FVec* a, const C3D_FVec* b) {
    u32 x[4], y[4];
    memcpy(x, a, sizeof x);
    memcpy(y, b, sizeof y);
    return ((x[0] ^ y[0]) | (x[1] ^ y[1]) | (x[2] ^ y[2]) | (x[3] ^ y[3])) == 0;
}

static uint32_t group_seen[RD_GPU_GROUPS];  /* draw_serial when the group was first drawn */

#ifdef MVC2_PROF
/* MVC2_LINE_COUNT=1: in a frame of 16, the cache lines the model draws and the states loop read and write, by what
   holds them: each group's or state's own, and the frame's once, counted 16 times */
enum { LD_BATCH, LD_DRAW, LD_MESH, LD_RUN, LD_PREP, LD_STATE, LD_ST, LD_TEX, LD_SEEN, LD_FRAG, LD_CMD, LD_N };
enum { LS_SEEN, LS_STATE, LS_ST, LS_SRC, LS_FIND, LS_GTEX, LS_LRU, LS_COPY, LS_N };
enum { LN_MINE = 128, LN_ALL = 31 };

typedef struct {
    uint32_t *key, *when, mask;     /* a frame's lines: line << 5 | class, and line << 5 | LN_ALL */
    uint32_t serial, items, all, cmd_last;
    uint32_t touch[LD_N], once[LD_N];
    uint32_t mine[LN_MINE];         /* the item's */
    int nmine;
} LINES;

static int line_count;
static uint32_t ln_dkey[16384], ln_dwhen[16384], ln_skey[4096], ln_swhen[4096];
static LINES ln_draws = { ln_dkey, ln_dwhen, 16383 }, ln_states = { ln_skey, ln_swhen, 4095 };
static int ln_draws_on, ln_states_on;
static const C3D_Tex* ln_bound;     /* the texture a group's draw sends */

static int ln_new(LINES* s, uint32_t key) {
    uint32_t h = (key * 2654435761u >> 16) & s->mask;
    while (s->when[h] == s->serial && s->key[h] != key) {
        h = (h + 1) & s->mask;
    }
    if (s->when[h] == s->serial) {
        return 0;
    }
    s->when[h] = s->serial;
    s->key[h] = key;
    return 1;
}

static void ln_add(LINES* s, int c, const void* p, uint32_t bytes) {
    uint32_t a = (uint32_t)(uintptr_t)p >> 5, z = ((uint32_t)(uintptr_t)p + (bytes > 0 ? bytes : 1) - 1) >> 5;
    for (; a <= z; a++) {
        uint32_t key = a << 5 | (uint32_t)c;
        int j;
        for (j = 0; j < s->nmine && s->mine[j] != key; j++) {
        }
        if (j < s->nmine) {
            continue;
        }
        if (s->nmine < LN_MINE) {
            s->mine[s->nmine++] = key;
        }
        s->touch[c]++;
        if (ln_new(s, key)) {
            s->once[c]++;
            s->all += (uint32_t)ln_new(s, a << 5 | LN_ALL);
        }
    }
}

#define LN_F(s, c, x) ln_add((s), (c), &(x), sizeof(x))

static void ln_begin(LINES* s) {
    s->serial++;
    s->cmd_last = 0;
}

static void ln_item(LINES* s) {
    s->nmine = 0;
    s->items++;
}

static void ln_end(LINES* s, int zone, int n) {
    int c;
    for (c = 0; c < n; c++) {
        prof_count(zone + 2 * c, s->touch[c] * 16);
        prof_count(zone + 2 * c + 1, s->once[c] * 16);
        s->touch[c] = s->once[c] = 0;
    }
    prof_count(zone + 2 * n, s->all * 16);
    prof_count(zone + 2 * n + 1, s->items * 16);
    s->all = s->items = 0;
}

/* what a group's draw reads of its draw and mesh, without MVC2_DRAW_HOT */
static void mesh_lines(LINES* s, const RD_GPU_DRAW* d, const GMESH* m, int raw) {
    LN_F(s, LD_DRAW, d->g.mesh);
    LN_F(s, LD_DRAW, d->g.faces);
    LN_F(s, LD_MESH, m->size);
    LN_F(s, LD_MESH, m->kind);
    LN_F(s, LD_MESH, m->nruns);
    LN_F(s, LD_MESH, m->run);
    LN_F(s, LD_MESH, m->ntri);
    LN_F(s, LD_MESH, m->nv);
    if (raw) {
        LN_F(s, LD_MESH, m->frag);
        LN_F(s, LD_MESH, m->frag_words);
        LN_F(s, LD_MESH, m->frag_at);
        LN_F(s, LD_RUN, m->run[m->nruns - 1].cull);
    } else {
        LN_F(s, LD_MESH, m->v);
        LN_F(s, LD_MESH, m->idx);
        LN_F(s, LD_MESH, m->v_at);
        LN_F(s, LD_MESH, m->idx_at);
        ln_add(s, LD_RUN, m->run, (uint32_t)m->nruns * sizeof *m->run);
    }
}

/* a group's draw: the command words from..to written, its mesh's words copied when frag; with m NULL its mesh
   and draw read from p */
static void draw_lines(const BATCH* b, const RD_GPU_DRAW* d, const GMESH* m, const GPREP* p, const GHOT* h, u32 from,
                       u32 to, int frag, int raw) {
    LINES* s = &ln_draws;
    ln_item(s);
    ln_add(s, LD_BATCH, b, sizeof *b);
    if (m == NULL) {
        LN_F(s, LD_PREP, p->h);
    } else {
        mesh_lines(s, d, m, raw);
    }
    LN_F(s, LD_PREP, p->u);
    LN_F(s, LD_PREP, p->ok);
    LN_F(s, LD_PREP, p->corner);
    LN_F(s, LD_PREP, p->keep5);
    if (tl_overlap) {
        LN_F(s, LD_PREP, p->depth_scale);
        LN_F(s, LD_PREP, p->depth_offset);
    }
    LN_F(s, LD_STATE, fr->states[b->state]);
    LN_F(s, LD_ST, rf->st[b->state]);
    if (ln_bound != NULL) {
        ln_add(s, LD_TEX, ln_bound, sizeof *ln_bound);
    }
    LN_F(s, LD_SEEN, group_seen[b->first]);
    if (frag) {
        ln_add(s, LD_FRAG, h->frag, h->frag_words * 4);
    }
    if (to > from) {
        /* written in order: only a line shared with the group before is not new */
        uint32_t a = (uint32_t)(uintptr_t)(gpuCmdBuf + from) >> 5, z = ((uint32_t)(uintptr_t)(gpuCmdBuf + to) - 1) >> 5;
        uint32_t n = z - a + 1, fresh = n - (a == s->cmd_last);
        s->touch[LD_CMD] += n;
        s->once[LD_CMD] += fresh;
        s->all += fresh;
        s->cmd_last = z;
    }
}
#endif

/* a group's draw in a slice counted, with the groups, and the indices and mesh vertices the shader is given */
static void group_counts(const BATCH* b, u32 ntri, u32 nv) {
    ctr_render_count[CTR_COUNT_GROUPS]++;
    if (group_seen[b->first] != draw_serial) {
        group_seen[b->first] = draw_serial;
        ctr_render_count[CTR_COUNT_GROUPS_ONCE]++;
    }
    ctr_render_count[CTR_COUNT_TL_INDICES] += ntri * 3;
    ctr_render_count[CTR_COUNT_TL_VERTS] += nv;
}

/* a row's largest sum over a mesh */
static float row_reach(const C3D_FVec* r, const float* vmax) {
    return fabsf(r->x) * vmax[0] + fabsf(r->y) * vmax[1] + fabsf(r->z) * vmax[2] + fabsf(r->w);
}

/* group_corner's sums, each vertex's worked out once: the same sums in the same order */
static PREP_HOT int corner_fast(const BATCH* b, const GMESH* m, const float* pm, const RD_STATE* s, int j) {
    const RD_GPU_DRAW* d = &fr->groups[b->first];
    struct {
        int idx;
        float sx, sy, tu, tv;
    } seen[16];
    float as = 0.0f, at = 0.0f;
    int i, k, c;
    for (i = 0; i < 16; i++) {
        seen[i].idx = -1;
    }
    for (k = 0; k < m->ntri && k < 32; k++) {
        float sx[3], sy[3], tu[3], tv[3];
        for (i = 0; i < 3; i++) {
            int x = m->idx[k * 3 + i], h = x & 15;
            if (seen[h].idx != x) {
                const float* v = (const float*)((const uint8_t*)m->v + x * MESH_STRIDE);
                float p[4] = { v[0], v[1], v[2], 1.0f }, w = 0.0f, px = 0.0f, py = 0.0f;
                for (c = 0; c < 4; c++) {
                    w += pm[4 * c] * p[c];
                    px += pm[4 * c + 1] * p[c];
                    py += pm[4 * c + 2] * p[c];
                }
                seen[h].idx = x;
                seen[h].sx = px / w;
                seen[h].sy = py / w;
                seen[h].tu = v[3] * (float)s->tex->width;
                seen[h].tv = v[4] * (float)s->tex->height;
            }
            sx[i] = seen[h].sx;
            sy[i] = seen[h].sy;
            tu[i] = seen[h].tu;
            tv[i] = seen[h].tv;
        }
        as += fabsf((sx[1] - sx[0]) * (sy[2] - sy[0]) - (sx[2] - sx[0]) * (sy[1] - sy[0]));
        at += fabsf((tu[1] - tu[0]) * (tv[2] - tv[0]) - (tu[2] - tu[0]) * (tv[1] - tv[0]));
    }
    TL_COUNT(PZ_TL_CORNER, 1);     /* worked out again */
    corner_memo2[j] = corner_memo[j];
    memcpy(corner_memo[j].m, pm, sizeof corner_memo[j].m);
    corner_memo[j].mesh = (int16_t)(d->g.mesh + 1);
    corner_memo[j].tw = (uint16_t)s->tex->width;
    corner_memo[j].th = (uint16_t)s->tex->height;
    corner_memo[j].yes = as > at * 0.5f && as < at * 2.0f;
    return corner_memo[j].yes;
}

/* corner_fast's sums with the texels' area kept for the mesh and its texture's size: the same sums in the same order */
static int corner_fast2(const GMESH* m, const float* pm, int mesh, int tw, int th, int j) {
    struct {
        int idx;
        float sx, sy, tu, tv;
    } seen[16];
    int have = corner_at[mesh].mesh == mesh + 1 && corner_at[mesh].tw == tw && corner_at[mesh].th == th;
    float as = 0.0f, at = have ? corner_at[mesh].at : 0.0f;
    int i, k, c;
#ifdef MVC2_PROF
    int past = -1;
#endif
    for (i = 0; i < 16; i++) {
        seen[i].idx = -1;
    }
    for (k = 0; k < m->ntri && k < 32; k++) {
        float sx[3], sy[3], tu[3], tv[3];
        for (i = 0; i < 3; i++) {
            int x = m->idx[k * 3 + i], h = x & 15;
            if (seen[h].idx != x) {
                const float* v = (const float*)((const uint8_t*)m->v + x * MESH_STRIDE);
                float p[4] = { v[0], v[1], v[2], 1.0f }, w = 0.0f, px = 0.0f, py = 0.0f;
                for (c = 0; c < 4; c++) {
                    w += pm[4 * c] * p[c];
                    px += pm[4 * c + 1] * p[c];
                    py += pm[4 * c + 2] * p[c];
                }
                seen[h].idx = x;
                seen[h].sx = px / w;
                seen[h].sy = py / w;
                if (!have) {
                    seen[h].tu = v[3] * (float)tw;
                    seen[h].tv = v[4] * (float)th;
                }
            }
            sx[i] = seen[h].sx;
            sy[i] = seen[h].sy;
            tu[i] = seen[h].tu;
            tv[i] = seen[h].tv;
        }
        as += fabsf((sx[1] - sx[0]) * (sy[2] - sy[0]) - (sx[2] - sx[0]) * (sy[1] - sy[0]));
        if (!have) {
            at += fabsf((tu[1] - tu[0]) * (tv[2] - tv[0]) - (tu[2] - tu[0]) * (tv[1] - tv[0]));
        }
#ifdef MVC2_PROF
        if (have && past < 0 && !(as < at * 2.0f)) {
            past = k + 1;
        }
#endif
    }
    TL_COUNT(PZ_TL_CORNER, 1);     /* worked out again */
#ifdef MVC2_PROF
    TL_COUNT(PZ_TL_C_AT, have);
    TL_COUNT(PZ_TL_C_BIG, !(as < at * 2.0f));
    TL_COUNT(PZ_TL_C_PAST, past < 0 ? 0 : k - past);
#endif
    if (!have) {
        corner_at[mesh].mesh = (int16_t)(mesh + 1);
        corner_at[mesh].tw = (uint16_t)tw;
        corner_at[mesh].th = (uint16_t)th;
        corner_at[mesh].at = at;
    }
    corner_memo2[j] = corner_memo[j];
    memcpy(corner_memo[j].m, pm, sizeof corner_memo[j].m);
    corner_memo[j].mesh = (int16_t)(mesh + 1);
    corner_memo[j].tw = (uint16_t)tw;
    corner_memo[j].th = (uint16_t)th;
    corner_memo[j].yes = as > at * 0.5f && as < at * 2.0f;
    return corner_memo[j].yes;
}

/* the 16 floats' bits the same: memcmp's answer in fewer steps */
static inline int mat_same(const float* a, const float* b) {
    u32 x[16], y[16], d = 0;
    int i;
    memcpy(x, a, sizeof x);
    memcpy(y, b, sizeof y);
#pragma GCC unroll 16
    for (i = 0; i < 16; i++) {
        d |= x[i] ^ y[i];
    }
    return d == 0;
}

/* w > 256 on the float's bits: positive, above 256's and not a NaN's */
static inline int above_256(float w) {
    int32_t x;
    memcpy(&x, &w, 4);
    return x > 0x43800000 && x <= 0x7F800000;
}

/* with MVC2_PREP_FAST2, group_corner's tests made in prep_group, before the call */
static inline int corner_wanted(const BATCH* b) {
    const RD_GPU_DRAW* d = &fr->groups[b->first];
    return smooth_2d && fr->states[b->state].tex != NULL && !rf->st[b->state].pal && !above_256(d->g.whi) &&
           !(d->g.whi - d->g.wlo > d->g.whi / 256);
}

static int corner_tested(const GMESH* m, int put, int mesh, int tw, int th);

/* MVC2_SMOOTH_2D: a group of the HUD's, flat to the screen and nearer than w 256, with its texels about the game's
   pixels in size (its triangles' area on the screen half to twice theirs in the texture) */
static inline __attribute__((always_inline)) int group_corner(const BATCH* b, const GMESH* m, const int fast) {
    const RD_STATE* s = &fr->states[b->state];
    const RD_GPU_DRAW* d = &fr->groups[b->first];
    const float* pm = fr->puts[d->put].m;
    float as = 0.0f, at = 0.0f;
    int i, k, c, j = d->g.mesh & 255;
    if (!smooth_2d || s->tex == NULL || rf->st[b->state].pal || d->g.whi > 256.0f || d->g.whi - d->g.wlo > d->g.whi / 256) {
        return 0;
    }
    if (corner_memo[j].mesh == d->g.mesh + 1 && corner_memo[j].tw == s->tex->width && corner_memo[j].th == s->tex->height &&
        memcmp(corner_memo[j].m, pm, sizeof corner_memo[j].m) == 0) {
        return corner_memo[j].yes;
    }
    if (fast) {
        const CORNER_MEMO* o = &corner_memo2[j];
        if (o->mesh == d->g.mesh + 1 && o->tw == s->tex->width && o->th == s->tex->height &&
            memcmp(o->m, pm, sizeof o->m) == 0) {
            CORNER_MEMO t = corner_memo[j];
            corner_memo[j] = *o;
            corner_memo2[j] = t;
            return corner_memo[j].yes;
        }
        return corner_fast(b, m, pm, s, j);
    }
    for (k = 0; k < m->ntri && k < 32; k++) {
        float sx[3], sy[3], tu[3], tv[3];
        for (i = 0; i < 3; i++) {
            const float* v = (const float*)((const uint8_t*)m->v + m->idx[k * 3 + i] * MESH_STRIDE);
            float p[4] = { v[0], v[1], v[2], 1.0f }, w = 0.0f, x = 0.0f, y = 0.0f;
            for (c = 0; c < 4; c++) {
                w += pm[4 * c] * p[c];
                x += pm[4 * c + 1] * p[c];
                y += pm[4 * c + 2] * p[c];
            }
            sx[i] = x / w;
            sy[i] = y / w;
            tu[i] = v[3] * (float)s->tex->width;
            tv[i] = v[4] * (float)s->tex->height;
        }
        as += fabsf((sx[1] - sx[0]) * (sy[2] - sy[0]) - (sx[2] - sx[0]) * (sy[1] - sy[0]));
        at += fabsf((tu[1] - tu[0]) * (tv[2] - tv[0]) - (tu[2] - tu[0]) * (tv[1] - tv[0]));
    }
    TL_COUNT(PZ_TL_CORNER, 1);     /* worked out again */
    memcpy(corner_memo[j].m, pm, sizeof corner_memo[j].m);
    corner_memo[j].mesh = (int16_t)(d->g.mesh + 1);
    corner_memo[j].tw = (uint16_t)s->tex->width;
    corner_memo[j].th = (uint16_t)s->tex->height;
    corner_memo[j].yes = as > at * 0.5f && as < at * 2.0f;
    return corner_memo[j].yes;
}

/* A batch of a GPU group: its mesh in its runs. The first of a run of groups goes through citro3d, which sends their
   attributes and uniforms; after it, a group's buffer, the uniforms that change and its bools are written here and
   its draws sent as raw_draw sends them, citro3d's copies of the uniforms kept the same */
/* a two-pass group's culling: its back faces' pass drops those facing the viewer, its front faces' those facing away */
static uint8_t pass_cull(uint8_t cull, int faces) {
    return faces == 0 ? cull : faces == 1 ? GPU_CULL_FRONT_CCW : GPU_CULL_BACK_CCW;
}

/* a slice's depth map for the groups drawn in slices k0 to k1, by those and the overlap: kept for the frame */
typedef struct {
    uint32_t serial, key;
    C3D_FVec u2;
    float scale, offset;
} DEPTH_KEPT;
static DEPTH_KEPT depth_kept[64];

/* max_f of two floats not below +0 and not NaNs, on their bits */
static inline float max_bits(float a, float b) {
    u32 x, y;
    memcpy(&x, &a, 4);
    memcpy(&y, &b, 4);
    return x > y ? a : b;
}

/* b's uniforms, corner and depth; 0 when they are out of the GPU's range. fast and fast2: MVC2_PREP_FAST's and
   MVC2_PREP_FAST2's, given as constants */
static inline __attribute__((always_inline)) int prep_body(const BATCH* b, const GMESH* m, GPREP* p, C3D_FVec* u, uint32_t* tq, const int fast,
                                                    const int fast2) {
    static int bad_logged;
    const RD_GPU_DRAW* d = &fr->groups[b->first];
    const GPACK* q = fast2 && prep_pack ? &L->gpack[b->offset] : NULL;
    float clip[4];
    int r, keep5, corner, ok, put = q != NULL ? q->put : d->put;
    if (q != NULL) {
        unifs_packed(b, m, q, u);
    } else {
        group_unifs(b, m, u, fast, fast2);
    }
    keep5 = m->kind != RD_GPU_LIT;
    TL_MARK(PZ_TL_UNIFS, *tq);
    if (!fast2) {
        corner = group_corner(b, m, fast);
    } else if (q != NULL ? q->cand : corner_wanted(b)) {
        corner = q != NULL ? corner_tested(m, put, q->mesh, q->tw, q->th)
                           : corner_tested(m, put, d->g.mesh, fr->states[b->state].tex->width,
                                           fr->states[b->state].tex->height);
    } else {
        TL_COUNT(PZ_TL_C_REJECT, 1);
        corner = 0;
    }
    TL_MARK(PZ_TL_CORNER, *tq);
    for (r = 0; r < 2 && corner; r++) {
        /* proj_corner's half pixel, times the put's w */
        const float* pm = fr->puts[put].m;
        float dw = proj_corner.r[r].w - proj.r[r].w;
        u[r] = FVec4_New(u[r].x + dw * pm[0], u[r].y + dw * pm[4], u[r].z + dw * pm[8], u[r].w + dw * pm[12]);
    }
    if (tl_overlap) {
        /* the clip's 0 to 1 from depth lo to hi in the slice's own, and the depth buffer's values as the slice's */
        /* the same for the groups drawn one after another in a slice: kept from the last */
        static double last_lo, last_hi;
        static int last_k = -1;
        static uint32_t last_serial;
        static C3D_FVec last_u2;
        static float last_scale, last_offset;
        double lo, hi, scale = L->slice_scale[b->slice], base = L->slice_base[b->slice], da, db;
        int k = b->slice, over = q != NULL ? q->over : fr->states[b->state].list != RD_TRANS;
        uint32_t key = 0;
        DEPTH_KEPT* e = NULL;
        if (fast) {
            key = q != NULL ? (uint32_t)q->k0 | (uint32_t)q->k1 << 4 | (uint32_t)k << 8 | (uint32_t)over << 12
                            : (uint32_t)L->gpu_k0[b->first] | (uint32_t)L->gpu_k1[b->first] << 4 | (uint32_t)k << 8 |
                                  (uint32_t)over << 12;
            e = &depth_kept[(key ^ key >> 6) & 63];
        }
        if (fast && e->serial == prep_serial && e->key == key) {
            u[2] = e->u2;
            p->depth_scale = e->scale;
            p->depth_offset = e->offset;
            TL_MARK(PZ_TL_DEPTH, *tq);
            goto ranges;
        }
        /* a translucent band drawn twice would show: only the narrow slices given over */
        tl_host(b->first, k, over, &lo, &hi);
        if (k != last_k || lo != last_lo || hi != last_hi || prep_serial != last_serial) {
            da = min_d(0.0, lo * scale + base);
            db = max_d(1.0, hi * scale + base);
            last_u2 = FVec4_New((float)(proj.r[2].z * (base - da) / (db - da)), (float)(proj.r[2].z * scale / (db - da)), 0.0f, 0.0f);
            last_scale = (float)(-(db - da) / L->nslices);
            last_offset = (float)((k + da) / L->nslices);
            TL_COUNT(PZ_TL_DEPTH, 1);  /* worked out again */
            last_k = k;
            last_lo = lo;
            last_hi = hi;
            last_serial = prep_serial;
        }
        u[2] = last_u2;
        p->depth_scale = last_scale;
        p->depth_offset = last_offset;
        if (fast) {
            e->serial = prep_serial;
            e->key = key;
            e->u2 = last_u2;
            e->scale = last_scale;
            e->offset = last_offset;
        }
    }
    TL_MARK(PZ_TL_DEPTH, *tq);
ranges:
    /* the shader's sums kept in float24's range, at most about 1.8e19, at the mesh's farthest corner; u[5], when
       kept, was in range when it was sent */
    clip[0] = row_reach(&u[0], m->vmax);
    clip[1] = row_reach(&u[1], m->vmax);
    clip[3] = row_reach(&u[3], m->vmax);
    clip[2] = fabsf(u[2].x) * clip[3] + fabsf(u[2].y);
    if (fast2) {
        /* the put's rows 0, 1 and 3 tested as they were made, unless the corner moved 0 and 1; clip is not below +0,
           so its bits test as its value */
        unsigned known = rows_in[put] && !corner ? 0xBu : 0u;
        for (r = 0; r < TL_UNIFS && ((r == 5 && keep5) || (((known >> r & 1u) || vec_in_range(&u[r])) &&
                                                             (r >= 4 || in_range(clip[r]))));
             r++) {
        }
    } else if (fast) {
        for (r = 0; r < TL_UNIFS && ((r == 5 && keep5) || (vec_in_range(&u[r]) && !(r < 4 && !(clip[r] < 1e18f))));
             r++) {
        }
    } else {
        for (r = 0; r < TL_UNIFS && ((r == 5 && keep5) || (fabsf(u[r].x) < 1e18f && fabsf(u[r].y) < 1e18f &&
                                     fabsf(u[r].z) < 1e18f && fabsf(u[r].w) < 1e18f && !(r < 4 && !(clip[r] < 1e18f))));
             r++) {
        }
    }
    if (fast2 && r == TL_UNIFS) {
        /* all four in range, not below +0: their bits order as they do */
        p->clip = max_bits(max_bits(clip[0], clip[1]), max_bits(clip[2], clip[3]));
    } else {
        p->clip = max_f(max_f(clip[0], clip[1]), max_f(clip[2], clip[3]));
    }
    ok = r == TL_UNIFS;
    p->keep5 = (uint8_t)keep5;
    p->corner = (uint8_t)corner;
    p->ok = (uint8_t)ok;
    if (!ok && bad_logged++ < 8) {
        host_log("model group %d: uniform %d in slice %d out of the GPU's range: %g %g %g %g, its sums to %g (slice %g to %g)",
                 b->first, r, b->slice, u[r].x, u[r].y, u[r].z, u[r].w, r < 4 ? clip[r] : 0.0, L->slice_lo[b->slice],
                 L->slice_lo[b->slice + 1]);
    }
    TL_MARK(PZ_TL_UNIFS, *tq);
    return ok;
}

static PREP_HOT __attribute__((noinline)) int prep_fast1(const BATCH* b, const GMESH* m, GPREP* p, C3D_FVec* u, uint32_t* tq) {
    return prep_body(b, m, p, u, tq, 1, 0);
}

static PREP_HOT __attribute__((noinline)) int prep_plain(const BATCH* b, const GMESH* m, GPREP* p, C3D_FVec* u, uint32_t* tq) {
    return prep_body(b, m, p, u, tq, 0, 0);
}

static PREP_HOT __attribute__((noinline, aligned(4096))) int prep_fast12(const BATCH* b, const GMESH* m, GPREP* p, C3D_FVec* u, uint32_t* tq) {
    return prep_body(b, m, p, u, tq, 1, 1);
}

static PREP_HOT int prep_unifs(const BATCH* b, const GMESH* m, GPREP* p, C3D_FVec* u, uint32_t* tq) {
    return prep_fast2 ? prep_fast12(b, m, p, u, tq) : prep_fast ? prep_fast1(b, m, p, u, tq) : prep_plain(b, m, p, u, tq);
}

/* with MVC2_DRAW_COPY, the uniforms worked out on the stack and written to p once: p's lines aren't read */
static PREP_HOT int prep_group(const BATCH* b, const GMESH* m, GPREP* p, uint32_t* tq) {
    C3D_FVec u[TL_UNIFS];
    int ok;
    if (!draw_copy) {
        return prep_unifs(b, m, p, p->u, tq);
    }
    ok = prep_unifs(b, m, p, u, tq);
    copy_fixed(p->u, u, sizeof u);
    return ok;
}

/* a group's mesh, or NULL when it has none */
static const GMESH* mesh_of(int mesh) {
    const GMESH* m = &gmesh[mesh < 0 || mesh >= MAX_MESHES ? 0 : mesh];
    return mesh < 0 || mesh >= MAX_MESHES || m->size == 0 ? NULL : m;
}

static const GMESH* group_mesh(const RD_GPU_DRAW* d) {
    return mesh_of(d->g.mesh);
}

static PREP_HOT void hot_make(GHOT* h, const GMESH* m, int faces) {
    h->mesh_ok = m != NULL;
    if (m != NULL) {
        h->kind = (uint8_t)m->kind;
        h->last_cull = m->nruns > 0 ? m->run[m->nruns - 1].cull & 3 : 0;
        h->faces = (uint8_t)faces;
        h->nruns = (uint16_t)m->nruns;
        h->ntri = (uint16_t)m->ntri;
        h->frag_words = m->frag_words;
        h->frag_at = m->frag_at;
        h->frag = m->frag;
        h->nv = m->nv;
    }
}

/* MVC2_GROUP_EARLY=1: the next group's mesh words and prep read while this one is drawn, and the mesh of the
   one after that, so that their misses are taken before they are needed; =2 four lines at a time, overlapped */
static int group_early;

static void group_touch(const void* p, uint32_t bytes) {
    if (group_early == 2) {
        early_lines4(p, bytes);
    } else {
        early_lines(p, bytes);
    }
}

static void group_ahead(const BATCH* b) {
    const BATCH* end = L->batches + L->nbatches;
    if (b + 1 < end && b[1].count == GPU_GROUP) {
        const GMESH* m = group_mesh(&fr->groups[b[1].first]);
        if (m != NULL) {
            group_touch(m->frag, m->frag_words * 4);
        }
        if (group_prep) {
            group_touch(&L->gprep[b[1].offset], sizeof(GPREP));
        }
        if (b + 2 < end && b[2].count == GPU_GROUP) {
            const RD_GPU_DRAW* d = &fr->groups[b[2].first];
            group_touch(d, sizeof *d);
            if (d->g.mesh >= 0 && d->g.mesh < MAX_MESHES) {
                group_touch(&gmesh[d->g.mesh], sizeof(GMESH));
            }
        }
    }
}

/* MVC2_PREP_EARLY=1: the lines a group's prep reads first taken while the one before it is worked out, its draw's
   one group sooner, so that their misses overlap */
static int prep_early;

static void prep_ahead(int i) {
    int n = 0, j;
    for (j = i + 1; j < L->nbatches && j <= i + 8 && n < 2; j++) {
        const BATCH* b = &L->batches[j];
        if (b->count == GPU_GROUP) {
            const RD_GPU_DRAW* d = &fr->groups[b->first];
            if (n++ == 0) {
                int mesh = d->g.mesh >= 0 && d->g.mesh < MAX_MESHES ? d->g.mesh : 0;
                early_words4(&gmesh[mesh].vmax, fr->puts[d->put].m, &fr->states[b->state].tex, &rf->st[b->state]);
                early_words4(&gmesh[mesh].kind, &fr->puts[d->put].m[8], &L->gprep[b->offset], &L->gpu_k0[b->first]);
            } else {
                early_lines4(d, sizeof *d);
            }
        }
    }
}

#ifdef MVC2_PROF
/* MVC2_PREP_PACK's counts, made a frame in 16 and taken 16 times: its records' lines, and the lines group prep reads
   for the same without them */
static void pack_count(void) {
    static uint32_t line[2048], when[2048];
    uint32_t n = 0, touch = 0, once = 0;
    int i, k;
    for (i = 0; i < L->nbatches; i++) {
        const BATCH* b = &L->batches[i];
        if (b->count == GPU_GROUP) {
            const RD_GPU_DRAW* d = &fr->groups[b->first];
            const RD_STATE* s = &fr->states[b->state];
            const GPACK* q = &L->gpack[b->offset];
            const void* at[12];
            uint32_t mine[12];
            int na = 0, nm = 0;
            at[na++] = &d->g.mesh;
            at[na++] = &d->g.face_argb;
            at[na++] = &d->g.whi;
            at[na++] = &d->g.wlo;
            at[na++] = &d->put;
            at[na++] = &rf->st[b->state];
            at[na++] = (const uint8_t*)&rf->st[b->state + 1] - 1;
            at[na++] = &s->tex;
            at[na++] = &s->list;
            at[na++] = &L->gpu_k0[b->first];
            at[na++] = &L->gpu_k1[b->first];
            if (q->cand) {
                at[na++] = &s->tex->width;
            }
            for (k = 0; k < na; k++) {
                uint32_t a = (uint32_t)(uintptr_t)at[k] >> 5, h = (a * 2654435761u) >> 21, j;
                for (j = 0; j < (uint32_t)nm && mine[j] != a; j++) {
                }
                if (j < (uint32_t)nm) {
                    continue;
                }
                mine[nm++] = a;
                touch++;
                while (when[h] == prep_serial && line[h] != a) {
                    h = (h + 1) & 2047;
                }
                if (when[h] != prep_serial) {
                    when[h] = prep_serial;
                    line[h] = a;
                    once++;
                }
            }
            n++;
        }
    }
    prof_count(PZ_PACK_BYTES, n * (uint32_t)sizeof(GPACK) * 16);
    prof_count(PZ_PACK_LINES, (n * (uint32_t)sizeof(GPACK) + 31) / 32 * 16);
    prof_count(PZ_PACK_TOUCH, touch * 16);
    prof_count(PZ_PACK_DISTINCT, once * 16);
}
#endif

/* MVC2_GROUP_PREP: the lists' groups worked out in their order, as draw() will take them */
static PREP_HOT __attribute__((noinline)) void prep_groups(void) {
    int i;
    uint32_t tq = 0;
    PROF_START(p0);
    PFZ_START(q0);
    prep_serial++;
    for (i = 0; i < L->nbatches; i++) {
        const BATCH* b = &L->batches[i];
        if (b->count == GPU_GROUP) {
            const GMESH* m = prep_pack ? mesh_of(L->gpack[b->offset].mesh) : group_mesh(&fr->groups[b->first]);
            GPREP* p = &L->gprep[b->offset];
            if (prep_early) {
                prep_ahead(i);
            }
#ifdef MVC2_PROF
            tl_on = prof_tl > 0 && ++tl_seq % prof_tl == 0;
            tq = TL_NOW();
#endif
            if (draw_hot) {
                hot_make(&p->h, m, prep_pack ? L->gpack[b->offset].faces : fr->groups[b->first].g.faces);
            }
            if (m == NULL) {
                p->ok = 0;
            } else {
                prep_group(b, m, p, &tq);
            }
        }
    }
#ifdef MVC2_PROF
    tl_on = 0;
#endif
    PROF_STOP(PZ_LIST_GPREP, p0);
    PFZ_STOP(PFZ_GPREP, q0);
#ifdef MVC2_PROF
    if (prep_pack && prep_serial % 16 == 0) {
        pack_count();
    }
#endif
}

/* whether group g is drawn in slice k, and the 1/w it is drawn from and to there: with over set, TL_OVERLAP further */
static PREP_HOT int tl_host(int g, int k, int over, double* lo, double* hi) {
    int k0 = L->gpu_k0[g], k1 = L->gpu_k1[g], j, a = k, b = k, any = 0, best = k0;
    for (j = k0; j <= k1; j++) {
        any |= slice_wide(j);
        if (L->slice_lo[j + 1] - L->slice_lo[j] > L->slice_lo[best + 1] - L->slice_lo[best]) {
            best = j;
        }
    }
    if (!any) {
        if (k != best) {
            return 0;
        }
        a = k0;
        b = k1;
    } else {
        if (!slice_wide(k)) {
            return 0;
        }
        for (j = k0; j < k && !slice_wide(j); j++) {
        }
        if (j == k) {
            a = k0;     /* the narrow ones below, with no wider one under them */
        }
        while (b < k1 && !slice_wide(b + 1)) {
            b++;
        }
    }
    if (lo != NULL) {
        double span = over ? (L->slice_lo[k + 1] - L->slice_lo[k]) / 16 : 0.0;
        *lo = L->slice_lo[a] - min_d(TL_OVERLAP * L->slice_lo[a], span);
        *hi = L->slice_lo[b + 1] + min_d(TL_OVERLAP * L->slice_lo[b + 1], span);
    }
    return 1;
}

/* group_corner after its tests, with MVC2_PREP_FAST2: the group's put, mesh and texture size */
static PREP_HOT int corner_tested(const GMESH* m, int put, int mesh, int tw, int th) {
    const float* pm = fr->puts[put].m;
    int j = mesh & 255;
    const CORNER_MEMO* o = &corner_memo2[j];
    if (corner_memo[j].mesh == mesh + 1 && corner_memo[j].tw == tw && corner_memo[j].th == th &&
        mat_same(corner_memo[j].m, pm)) {
        TL_COUNT(PZ_TL_C_WAY1, 1);
        return corner_memo[j].yes;
    }
    if (o->mesh == mesh + 1 && o->tw == tw && o->th == th && mat_same(o->m, pm)) {
        CORNER_MEMO t = corner_memo[j];
        TL_COUNT(PZ_TL_C_WAY2, 1);
        corner_memo[j] = *o;
        corner_memo2[j] = t;
        return corner_memo[j].yes;
    }
    return corner_fast2(m, pm, mesh, tw, th, j);
}

/* with MVC2_DRAW_COPY, the raw draw's uniforms: those of p that differ from what the GPU has (all of them, with all)
   taken into tl_sent, u[5] kept when p says; lo to hi, the range taken */
static void sent_take(const GPREP* p, int all, int* plo, int* phi) {
    const C3D_FVec* u = p->u;
    int lo = 0, hi = TL_UNIFS, r;
    while (!all && lo < hi && ((lo == 5 && p->keep5) || vec_same(&u[lo], &tl_sent[lo]))) {
        lo++;
    }
    while (!all && hi > lo && ((hi - 1 == 5 && p->keep5) || vec_same(&u[hi - 1], &tl_sent[hi - 1]))) {
        hi--;
    }
    for (r = lo; r < hi; r++) {
        if (r != 5 || !p->keep5) {
            tl_sent[r] = u[r];
        }
    }
    *plo = lo;
    *phi = hi;
}

static void draw_group(const BATCH* b) {
    static int bad_logged;
    const RD_GPU_DRAW* d = &fr->groups[b->first];
    const GMESH* m = NULL;
    const GPREP* p = group_prep ? &L->gprep[b->offset] : NULL;
    const GHOT* h;
    GPREP here;
    GHOT mine;
    C3D_FVec u[TL_UNIFS];
    u32 lit = 1u << (b_tl_lit - 0x68), vtxcol = 1u << (b_tl_vtxcol - 0x68);
    u16 bools;
    int r, c3d = !tl_now || !raw_draws, lo = 0, hi = TL_UNIFS, front = 0;
    uint32_t tq, luts;
#ifdef MVC2_PROF
    u32 cmd0 = gpuCmdBufOffset;
    tl_on = prof_tl > 0 && ++tl_seq % prof_tl == 0;
#endif
    tq = TL_NOW();
    if (group_early) {
        group_ahead(b);
    }
    if (gpuCmdBufOffset + 64 > gpuCmdBufSize) {
        return;
    }
    if (draw_hot) {
        h = &p->h;
    } else {
        m = group_mesh(d);
        hot_make(&mine, m, d->g.faces);
        h = &mine;
    }
    if (!h->mesh_ok) {
        if (bad_logged++ < 8) {
            host_log("model group %d: mesh %d has none", b->first, d->g.mesh);
        }
        return;
    }
    bools = (u16)((C3D_BoolUnifs[GPU_VERTEX_SHADER] & ~(lit | vtxcol)) | 1u << (b_tl - 0x68) |
                  (h->kind == RD_GPU_LIT ? lit : 0) | (h->kind == RD_GPU_VTXCOL ? vtxcol : 0));
    if (!group_prep) {
        prep_group(b, m, &here, &tq);
        p = &here;
    }
    if (tl_overlap) {
        fx_depth_map(p->depth_scale, p->depth_offset);
        map_custom = 1;
    }
    if (!p->ok) {
        return;
    }
    if (!draw_copy) {
        memcpy(u, p->u, sizeof u);
        if (p->keep5) {
            u[5] = tl_sent[5];
        }
    }
    luts = lut_palettes;
    corner_ok = p->corner;
    apply(&fr->states[b->state], &rf->st[b->state], 0, 0);
    corner_ok = 0;
#ifdef MVC2_PROF
    ln_bound = bind_tex;
#endif
    TL_COUNT(PZ_TL_APPLY, lut_palettes - luts);    /* the palettes sent */
    TL_MARK(PZ_TL_APPLY, tq);
    if (c3d && raw_front && tl_frags && h->nruns > 0 && h->faces == 0 && !c3d_needed && raw_ok(bind_tex) &&
        gpuCmdBufOffset + SETUP_WORDS + 4 + (u32)TL_UNIFS * 4 + 2 + 2 + RAW_STATE_WORDS + h->frag_words <= gpuCmdBufSize) {
        /* as citro3d is given it below, but the setup written here; then all the uniforms and the bools */
        if (m == NULL) {
            m = group_mesh(d);
        }
        gpuCmdBufOffset = (u32)(attr_setup(gpuCmdBuf + gpuCmdBufOffset, &mesh_attr, m->v, MESH_STRIDE) - gpuCmdBuf);
        C3D_SetAttrInfo(&mesh_attr);
        bind_mesh(m->v);
        c3d_stale = back_words = 0;
        tl_now = 1;
        c3d = 0;
        front = 1;
    }
    if (!c3d && tl_frags && h->nruns > 0 && h->faces == 0 && raw_ok(bind_tex) &&
        gpuCmdBufOffset + 4 + (u32)TL_UNIFS * 4 + 2 + 2 + RAW_STATE_WORDS + h->frag_words <= gpuCmdBufSize) {
        /* the uniforms that change, the bools, the state, then the mesh's own words */
        u32 *w = gpuCmdBuf + gpuCmdBufOffset, *at;
        u32 last = h->last_cull;
        const C3D_FVec* v = draw_copy ? tl_sent : u;    /* what is sent */
        if (draw_copy) {
            sent_take(p, front, &lo, &hi);
        } else {
            while (!front && lo < hi && vec_same(&u[lo], &tl_sent[lo])) {
                lo++;
            }
            while (!front && hi > lo && vec_same(&u[hi - 1], &tl_sent[hi - 1])) {
                hi--;
            }
        }
        if (lo < hi) {
            const u32* p = (const u32*)&v[lo];
            u32 n = (u32)(hi - lo) * 4;
            CMD(GPUREG_VSH_FLOATUNIFORM_CONFIG, 0xF, 0x80000000u | (u32)(u_tl + lo));
            memcpy(w, p, 4);
            w[1] = GPUCMD_HEADER(0, 0xF, GPUREG_VSH_FLOATUNIFORM_DATA) | (n - 1) << 20;
            if (draw_copy) {
                w[2] = p[1];
                w[3] = p[2];
                w[4] = p[3];
                copy_pieces(w + 5, p + 4, hi - lo - 1);
                copy_pieces(&C3D_FVUnif[GPU_VERTEX_SHADER][u_tl + lo], &v[lo], hi - lo);
            } else {
                memcpy(w + 2, p + 1, (n - 1) * 4);
                memcpy(&C3D_FVUnif[GPU_VERTEX_SHADER][u_tl + lo], &u[lo], (size_t)(hi - lo) * sizeof *u);
            }
            w += n + 1;
            *w++ = 0;       /* n - 1 is odd: the next header on an even word */
        }
        if (front || bools != C3D_BoolUnifs[GPU_VERTEX_SHADER]) {
            CMD(GPUREG_VSH_BOOLUNIFORM, 0xF, 0x7FFF0000u | bools);
            C3D_BoolUnifs[GPU_VERTEX_SHADER] = bools;
        }
        w = raw_state(w, bind_tex);
        TL_COUNT(PZ_TL_WRITE, (uint32_t)(w - (gpuCmdBuf + gpuCmdBufOffset)));
        TL_MARK(PZ_TL_WRITE, tq);
        at = w;
        if (frag_call && (h->frag_at & 15) == 0) {
            w = frag_jump(w, h->frag_at, h->frag_words);
        } else if (frag_inline) {
            if (m == NULL) {
                m = group_mesh(d);
            }
            w = mesh_frag(w, m);
        } else {
            memcpy(w, h->frag, h->frag_words * 4);
            w += h->frag_words;
        }
        gpuCmdBufOffset = (u32)(w - gpuCmdBuf);
        if (!draw_copy) {
            memcpy(tl_sent, u, sizeof u);
        }
        /* citro3d's culling as the GPU's, for its next effect block */
        if (fx[2] != last) {
            C3D_CullFace((GPU_CULLMODE)last);
            fx[2] = last;
        }
        if (wdraw != NULL) {
            m = group_mesh(d);
            for (r = 0; r < m->nruns; r++, nwdraw++) {
                if (nwdraw < WATCH_DRAWS) {
                    wdraw[nwdraw] = (WATCH_DRAW){ (u32)(at + (raw_bufs ? 40 : 2) + 32 * r - frame_cmd), m->v_at,
                                                  m->idx_at + (u32)m->run[r].first * 6, (u32)m->run[r].count * 3,
                                                  m->size, (float)L->slice_scale[b->slice], p->clip,
                                                  (u16)b->first, (u16)d->g.mesh, (u8)m->kind, m->run[r].cull, 1,
                                                  (u8)lo, (u8)hi, (u8)m->nruns, (u8)b->slice, bools };
                }
            }
        }
        ctr_render_count[CTR_COUNT_TRIS] += h->ntri;
        ctr_render_count[CTR_COUNT_DRAWS] += h->nruns;
        group_counts(b, h->ntri, (u32)h->nv);
        TL_COUNT(PZ_TL_FRAG, h->frag_words);
        TL_MARK(PZ_TL_FRAG, tq);
#ifdef MVC2_PROF
        if (ln_draws_on) {
            draw_lines(b, d, draw_hot ? NULL : m, p, h, cmd0, gpuCmdBufOffset,
                       !(frag_call && (h->frag_at & 15) == 0) && !frag_inline, 1);
        }
#endif
        return;
    }
    if (m == NULL) {
        m = group_mesh(d);
    }
    if (draw_copy) {
        copy_fixed(u, p->u, sizeof u);
        if (p->keep5) {
            u[5] = tl_sent[5];
        }
    }
    if (c3d) {
        C3D_SetAttrInfo(&mesh_attr);
        bind_mesh(m->v);
        c3d_stale = back_words = 0;
        memcpy(C3D_FVUnifWritePtr(GPU_VERTEX_SHADER, u_tl, TL_UNIFS), u, sizeof u);
        C3D_BoolUnifs[GPU_VERTEX_SHADER] = bools;
        C3D_BoolUnifsDirty[GPU_VERTEX_SHADER] = true;
        tl_now = 1;
    } else {
        if (raw_bufs) {
            /* citro3d's buffer bind, its own record of the buffers left as it is */
            C3D_BufInfo buf;
            BufInfo_Init(&buf);
            BufInfo_Add(&buf, m->v, MESH_STRIDE, 3, 0x210);
            GPUCMD_AddWrite(GPUREG_ATTRIBBUFFERS_LOC, buf.base_paddr >> 3);
            GPUCMD_AddIncrementalWrites(GPUREG_ATTRIBBUFFER0_OFFSET, (u32*)buf.buffers, 36);
        } else {
            GPUCMD_AddWrite(GPUREG_ATTRIBBUFFER0_OFFSET, m->v_at);
        }
        while (lo < hi && memcmp(&u[lo], &tl_sent[lo], sizeof *u) == 0) {
            lo++;
        }
        while (hi > lo && memcmp(&u[hi - 1], &tl_sent[hi - 1], sizeof *u) == 0) {
            hi--;
        }
        if (lo < hi) {
            GPUCMD_AddWrite(GPUREG_VSH_FLOATUNIFORM_CONFIG, 0x80000000u | (u32)(u_tl + lo));
            GPUCMD_AddWrites(GPUREG_VSH_FLOATUNIFORM_DATA, (u32*)&u[lo], (u32)(hi - lo) * 4);
            memcpy(&C3D_FVUnif[GPU_VERTEX_SHADER][u_tl + lo], &u[lo], (size_t)(hi - lo) * sizeof *u);
        }
        if (bools != C3D_BoolUnifs[GPU_VERTEX_SHADER]) {
            GPUCMD_AddWrite(GPUREG_VSH_BOOLUNIFORM, 0x7FFF0000u | bools);
            C3D_BoolUnifs[GPU_VERTEX_SHADER] = bools;
        }
    }
    memcpy(tl_sent, u, sizeof u);
    for (r = 0; r < m->nruns; r++) {
        u32 at = gpuCmdBufOffset, first = (u32)m->run[r].first * 3, count = (u32)m->run[r].count * 3;
        u32 index_at = m->idx_at + first * 2;
        int raw;
        if (wdraw != NULL) {
            if (nwdraw < WATCH_DRAWS) {
                wdraw[nwdraw] = (WATCH_DRAW){ (u32)(gpuCmdBuf + gpuCmdBufOffset - frame_cmd), m->v_at,
                                              index_at, count, m->size, (float)L->slice_scale[b->slice], p->clip,
                                              (u16)b->first,
                                              (u16)d->g.mesh, (u8)m->kind, m->run[r].cull, 0, (u8)lo, (u8)hi, (u8)m->nruns,
                                              (u8)b->slice, bools };
            }
            nwdraw++;
        }
        fx_cull((GPU_CULLMODE)pass_cull(m->run[r].cull, d->g.faces));
        raw = !c3d && raw_draw(bind_tex, index_at, (int)count);
        if (wdraw != NULL && nwdraw <= WATCH_DRAWS) {
            wdraw[nwdraw - 1].raw = (u8)raw;
        }
        if (!raw) {
            if (!c3d) {
                bind_mesh(m->v);    /* citro3d's buffer as the one written here */
            }
            c3d_pending();
            C3D_DrawElements(GPU_TRIANGLES, (int)count, C3D_UNSIGNED_SHORT, m->idx + first);
            c3d_learn(at);
            fx_new = fog_new = 0;
            c3d = 0;
        }
        ctr_render_count[CTR_COUNT_TRIS] += m->run[r].count;
        ctr_render_count[CTR_COUNT_DRAWS]++;
    }
    group_counts(b, (u32)m->ntri, (u32)m->nv);
    TL_MARK(PZ_TL_C3D, tq);
#ifdef MVC2_PROF
    if (ln_draws_on) {
        draw_lines(b, d, m, p, h, cmd0, gpuCmdBufOffset, 0, 0);
    }
#endif
}

/* MVC2_LIST_SPLIT=p[,p...]: the frame's list sent in parts, split at the first draw past p% of the last frame's
   words (=1: 8,30), so that the GPU begins on the first while the rest is written. Not with the drawing thread,
   MVC2_FRAG_CALL or MVC2_GPU_PROBE */
enum { MAX_SPLITS = 4 };
static int nsplits;
static uint8_t split_pct[MAX_SPLITS];
static u32 split_words[MAX_SPLITS];     /* this frame's split points, in words from frame_cmd */
static int split_next;                  /* the next of them; nsplits when none is left */
static int splitting;                   /* this frame's list sent in parts */
static int list_running;                /* citro3d's queue started before C3D_FrameEnd */
static u32 last_words;                  /* the last frame's words */
static uint32_t split_parts, split_frames;
static u16 queue_most;                  /* the most entries citro3d's queue held at a C3D_FrameEnd */
static int nsent;                     /* the frame's parts sent before C3D_FrameEnd: words from frame_cmd, and sizes */
static u32 part_at[MAX_SPLITS + 1], part_words[MAX_SPLITS + 1];
#ifdef MVC2_PROF
static u32 gpu_free_at;                 /* when C3D_FrameBegin found the GPU done */
#endif

/* GX_BindQueue through the linker's --wrap (CMakeLists), for citro3d's queue */
void __real_GX_BindQueue(gxCmdQueue_s* queue);
void __wrap_GX_BindQueue(gxCmdQueue_s* queue) {
    if (queue != NULL) {
        c3d_queue = queue;
    }
    __real_GX_BindQueue(queue);
}

/* the list so far sent, and citro3d's queue started on it with the clear and copies before it. As citro3d's split,
   but with one FINALIZE (citro3d's queue counts an interrupt a command), and the framebuffer still flagged as drawn
   to, so that the frame's last part flushes it as an unsplit frame does */
static void list_send_part(void) {
    u32* buf;
    u32 size;
    if (gpuCmdBufOffset == 0) {
        return;
    }
    GPUCMD_AddWrite(GPUREG_FRAMEBUFFER_FLUSH, 1);
    GPUCMD_AddWrite(GPUREG_FRAMEBUFFER_INVALIDATE, 1);
    if ((gpuCmdBufOffset & 3) == 0) {
        GPUCMD_AddWrite(GPUREG_FRAMEBUFFER_INVALIDATE, 1);  /* the FINALIZE ending the part on 16 bytes */
    }
    GPUCMD_Split(&buf, &size);
    GX_ProcessCommandList(buf, size * 4, GX_CMDLIST_FLUSH);
    if (nsent < MAX_SPLITS + 1) {
        part_at[nsent] = (u32)(buf - frame_cmd);
        part_words[nsent++] = size;
    }
    split_parts++;
    if (!list_running) {
        list_running = 1;
        PROF_ADD(PZ_GPU_IDLE, prof_now() - gpu_free_at);
        gxCmdQueueRun(c3d_queue);
#ifdef MVC2_PROF
        gpu_split_sent();
#endif
    }
}

/* after C3D_FrameEnd: the frame's words for the next one's split points, the queue's use, a line every 600 frames */
static void split_log(void) {
    last_words = frame_words;
    if (c3d_queue != NULL && c3d_queue->numEntries > queue_most) {
        queue_most = c3d_queue->numEntries;
    }
    if (++split_frames % 600 == 0) {
        host_log("list split: %.2f parts a frame, citro3d's queue at most %u of its %u entries (MVC2_LIST_SPLIT)",
                 (double)(split_parts + split_frames) / split_frames, (unsigned)queue_most,
                 c3d_queue != NULL ? (unsigned)c3d_queue->maxEntries : 0u);
    }
}

/* the wait for the last frame's GPU work, reported once if it takes over a second: the frame's parts, the interrupts
   since its list began (MVC2_GPU_SPLIT), and citro3d's queue and the GPU's registers */
static void split_wait(void) {
    static int reported;
    int i;
    if (c3d_queue == NULL || reported || gxCmdQueueWait(c3d_queue, SYSCLOCK_ARM11)) {
        return;
    }
    reported = 1;
    host_log("list split: the GPU not done after 1 s, after frame %lu", (unsigned long)frame_no);
    for (i = 0; i < nsent; i++) {
        host_log("  part %d: %lu words at word %lu", i, (unsigned long)part_words[i], (unsigned long)part_at[i]);
    }
#ifdef MVC2_PROF
    {
        char line[160];
        gpu_split_events(line, sizeof line);
        host_log("  interrupts since the list began:%s", line);
    }
#endif
    gpu_queue(NULL, "  ");
    gpu_regs(NULL);
    ctr_log_flush();
}

/* the frame's split points, from the last frame's words */
static void split_begin(void) {
    int k;
    list_running = splitting = nsent = 0;
    split_next = nsplits;
    if (nsplits == 0 || pipe_on || last_words == 0) {
        return;
    }
    for (k = 0; k < nsplits; k++) {
        split_words[k] = last_words * split_pct[k] / 100;
    }
    split_next = 0;
    splitting = 1;
}

static void draw(void) {
    int i, j;
#ifdef MVC2_PROF
    const u32* crc_at = gpuCmdBuf + gpuCmdBufOffset;
#endif
    uint32_t models = 0, models_at = 0;     /* the groups in this run of them, and when it began */
    uint32_t back_at = 0;                   /* when the draw after them began */
    GV* bg;
    PFZ_START(qm);
    if (L->nverts + 9 > MAX_VERTS) {
        L->nverts = MAX_VERTS - 9;
    }
    /* the background when it has no depth, then a triangle of no size, so that citro3d sends its pending
       light tables before the palettes go in */
    bg = &L->verts[L->nverts];
    {
        GV a = { GAME_X, 0, 0.5f, 1, 0, 0, 0xFF000000u | L->bg_vtx[0].base };
        GV b = { GAME_X + RD_WIDTH, 0, 0.5f, 1, 0, 0, 0xFF000000u | L->bg_vtx[1].base };
        GV cc = { GAME_X, RD_HEIGHT, 0.5f, 1, 0, 0, 0xFF000000u | L->bg_vtx[2].base };
        GV d = { GAME_X + RD_WIDTH, RD_HEIGHT, 0.5f, 1, 0, 0, 0xFF000000u | L->bg_vtx[3].base };
        bg[0] = a;
        bg[1] = b;
        bg[2] = cc;
        bg[3] = b;
        bg[4] = d;
        bg[5] = cc;
        bg[6] = a;
        bg[7] = a;
        bg[8] = a;
    }
    if (L->verts != bound_verts) {
        bind_verts();
    }
    flush_range(L->verts, (u32)(L->nverts + 9) * sizeof(GV), PZ_VERT_FLUSH);
    if (indexed) {
        flush_range(L->idx, (u32)L->nidx * sizeof *L->idx, PZ_VERT_FLUSH);
    }
    fog_setup();
    {
        u32 pa = osConvertVirtToPhys(L->idx), base = C3D_GetBufInfo()->base_paddr;
        idx_offset = pa > base ? pa - base : 0;
    }
    bind_tex = NULL;
    env_new = 0;
    fx_known = fog_known = 0;
    nwdraw = 0;
    draw_serial++;
    if (!group_prep) {
        prep_serial++;
    }
#ifdef MVC2_PROF
    ln_draws_on = line_count && draw_serial % 16 == 0;
    if (ln_draws_on) {
        ln_begin(&ln_draws);
    }
#endif

    memset(&cur, 0, sizeof cur);
    cur.lut_base = -1;
    C3D_SetScissor(GPU_SCISSOR_NORMAL, 0, game_y0, target_w, game_y0 + game_h);
    {
        static const RD_STATE bgs = { .list = RD_OPAQUE, .depth = RD_ALWAYS, .src = RD_ONE, .dst = RD_ZERO,
                                      .shading = RD_DECAL, .fog = RD_FOG_NONE };
        u32 at = gpuCmdBufOffset;
        apply(&bgs, NULL, 0, 0);
        c3d_pending();
        if (L->nbg == 0) {
            C3D_DrawArrays(GPU_TRIANGLES, L->nverts, 6);
        }
        C3D_DrawArrays(GPU_TRIANGLES, L->nverts + 6, 3);
        c3d_learn(at);
        fx_new = fog_new = 0;
    }
    /* a batch that changes nothing on the GPU goes on with the draw before */
    for (i = 0; i < L->nbatches; i = j) {
        const BATCH* b = &L->batches[i];
        int count = b->count;
        if (split_next < nsplits && (u32)(gpuCmdBuf + gpuCmdBufOffset - frame_cmd) >= split_words[split_next]) {
#ifdef MVC2_PROF
            if (draw_crc) {
                crc_add(crc_at);
            }
#endif
            list_send_part();
            while (split_next < nsplits && (u32)(gpuCmdBuf + gpuCmdBufOffset - frame_cmd) >= split_words[split_next]) {
                split_next++;
            }
#ifdef MVC2_PROF
            crc_at = gpuCmdBuf + gpuCmdBufOffset;
#endif
        }
        if (i == 0 || b->slice != L->batches[i - 1].slice || (map_custom && count != GPU_GROUP)) {
            /* slice k in the k-th part of the depth buffer */
            fx_depth_map(-1.0f / (float)L->nslices, (float)b->slice / (float)L->nslices);
            map_custom = 0;
        }
        corner_ok = 0;
        if (count == GPU_GROUP) {
            if (models == 0) {
                models_at = PROF_NOW();
                PFZ_AGAIN(qm);
            }
            models++;
            draw_group(b);
            j = i + 1;
            continue;
        }
        if (models > 0) {
            back_at = TL_NOW1();
        }
        if (tl_now) {
            tl_off();
        }
        corner_ok = smooth_2d;
        apply(&fr->states[b->state], &rf->st[b->state], b->offset, 0);
        if (cur.corner != proj_now) {
            C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, u_proj, cur.corner ? &proj_corner : &proj);
            proj_now = cur.corner;
            c3d_needed = 1;
        }
        for (j = i + 1; j < L->nbatches && L->batches[j].slice == b->slice && L->batches[j].count >= 0 &&
                        L->batches[j].first == b->first + count &&
                        !apply(&fr->states[L->batches[j].state], &rf->st[L->batches[j].state], L->batches[j].offset, 1);
             j++) {
            count += L->batches[j].count;
        }
        if (back_words && indexed && raw_draws && !c3d_needed && idx_offset != 0 &&
            gpuCmdBufOffset + BACK_WORDS <= gpuCmdBufSize) {
            gpuCmdBufOffset = (u32)(back_setup(gpuCmdBuf + gpuCmdBufOffset) - gpuCmdBuf);
            back_words = 0;
        }
        if (!indexed || !raw_draws || c3d_needed || idx_offset == 0 ||
            !raw_draw(bind_tex, idx_offset + (u32)b->first * 2, count)) {
            u32 at = gpuCmdBufOffset;
            c3d_needed = 0;
            c3d_pending();
            if (indexed) {
                C3D_DrawElements(GPU_TRIANGLES, count, C3D_UNSIGNED_SHORT, L->idx + b->first);
            } else {
                C3D_DrawArrays(GPU_TRIANGLES, b->first, count);
            }
            c3d_learn(at);
            fx_new = fog_new = 0;
        }
        ctr_render_count[CTR_COUNT_TRIS] += (uint32_t)count / 3;
        ctr_render_count[CTR_COUNT_DRAWS]++;
        if (models > 0) {
            /* a run of groups timed with the draw after it, where citro3d sends the CPU's setup again */
            TL_MARK1(PZ_TL_BACK, back_at);
            PROF_ADD(PZ_DRAW_MODELS, PROF_NOW() - models_at);
            PFZ_STOP(PFZ_MODELS, qm);
            PROF_COUNT(PZ_DRAW_MODELS, models);
            models = 0;
        }
    }
    if (tl_now) {
        tl_off();
    }
    if (models > 0) {
        PROF_ADD(PZ_DRAW_MODELS, PROF_NOW() - models_at);
        PROF_COUNT(PZ_DRAW_MODELS, models);
        PFZ_STOP(PFZ_MODELS, qm);
    }
#ifdef MVC2_PROF
    if (draw_crc) {
        crc_frame(crc_at);
    }
    if (ln_draws_on) {
        ln_end(&ln_draws, PZ_LN_DRAWS, LD_N);
        ln_draws_on = 0;
    }
#endif
    (void)models_at;    /* read by the profile only */
    (void)back_at;
}

static void pipe_start(void);
static void lists1_start(void);
static void wide_take(void);

#ifdef MVC2_PROF
/* the GPU's time for a frame's list: from its C3D_FrameEnd to the end of the list, timed by the interrupt */
static u32 list_sent;
static volatile u32 list_done;

static void list_end(void* arg) {
    (void)arg;
    list_done = (u32)svcGetSystemTick();
    gpu_split_p3d();
}
#endif

/* the game's columns on the target: across all of it, or 640 game pixels' worth between the pillars */
static void layout(int stretch) {
    game_y0 = stretch ? 0 : GAME_X * target_h / VIEW_W;
    game_h = stretch ? target_h : RD_WIDTH * target_h / VIEW_W;
    stretch_now = stretch;
    /* stretched, the pixels' centres fall between the game's: sprites' tiles a fraction apart would show the gaps */
    rd_snap_x = stretch;
}

/* the pillars' pictures when the game is between them, and how they are put */
static void pillars_setup(void) {
    const char* e = getenv("MVC2_PILLARS_LINE");
    pillars = NULL;
    if (game_y0 > 0) {
        if (pillars_made == NULL) {
            pillars_made = ctr_art_pillars(target_w, game_y0);
        }
        pillars = pillars_made;
    }
    pillars_once = pillars != NULL && getenv("MVC2_PILLARS_ONCE") != NULL && strcmp(getenv("MVC2_PILLARS_ONCE"), "0") != 0;
    if (pillars_once && !apt_hooked) {
        aptHook(&apt_cookie, apt_event, NULL);
        apt_hooked = 1;
    }
    pillars_line = pillars != NULL && (e != NULL ? strcmp(e, "0") != 0 : pillars_once);
    pillars_new = 1;
}

/* the game's pixels on the turned target; depth d to z = -d. MVC2_TEXEL_CENTRE's part of a game pixel along, or 0 */
static int texel_centre;

static float make_proj(void) {
    float across = -1.0f;
    int i;
    Mtx_OrthoTilt(&proj, GAME_X - (float)game_y0 * RD_WIDTH / game_h, GAME_X + (float)(target_h - game_y0) * RD_WIDTH / game_h,
                  (float)VIEW_H, 0.0f, 0.0f, 1.0f, true);
    proj.r[2].x = 0.0f;
    proj.r[2].y = 0.0f;
    proj.r[2].z = -1.0f;
    proj.r[2].w = 0.0f;
    proj.r[3].x = 0.0f;
    proj.r[3].y = 0.0f;
    proj.r[3].z = 0.0f;
    proj.r[3].w = 1.0f;
    if (texel_centre && (game_h * 2 == RD_WIDTH || game_h == RD_WIDTH || game_h == target_h) && target_w * 2 == RD_HEIGHT) {
        /* across, half size, or stretched to 400: 1.6 game pixels a pixel, whose centres fall on 0.8, 0.4, 0, 0.6
           and 0.2 of a game pixel by turns; a tenth along, none is on a texel's edge (nor stretched to 800, 0.8 a
           pixel). At half size a pixel's centre is on the game's pixels' corners, where a tile's edge and its
           texels' edges are: half a game pixel along, it is on a texel's centre, as the PowerVR2's are. Wide, a
           column is a game pixel, its centre on a texel's already */
        across = game_h * 2 == RD_WIDTH ? 0.5f : game_h == RD_WIDTH ? 0.0f : 0.1f;
        proj_corner = proj;
        for (i = 0; i < 2; i++) {
            proj.r[i].w += across * proj.r[i].x + 0.5f * proj.r[i].y;
        }
    }
    return across;
}

void ctr_render_set_stretch(int stretch) {
    stretch_want = stretch != 0;
}

/* as a frame begins, when nothing is building the lists */
static void stretch_take(void) {
    wide_take();
    if (stretch_at_i < stretch_at_n && ctr_presented >= (unsigned)stretch_at[stretch_at_i]) {
        stretch_at_i++;
        stretch_want = !stretch_now;
    }
    if (stretch_want >= 0 && stretch_want != stretch_now) {
        layout(stretch_want);
        make_proj();
        pillars_setup();
        host_log("drawing: frame %u on, %s", ctr_presented + 1, stretch_now ? "stretched" : "between the pillars");
    }
    stretch_want = -1;
}

/* a test run's frames, NAME=frame,...: how many */
static int frames_at(const char* name, int at[16]) {
    const char* p = getenv(name);
    int n = 0;
    while (p != NULL && n < 16 && *p != '\0') {
        at[n++] = atoi(p);
        while (*p != '\0' && *p++ != ',') {
        }
    }
    return n;
}

/* the target's size and the transfer to the screen: wide, the rows alone halved at full size */
static void sizes(int wide) {
    target_w = full_now ? VIEW_H : VIEW_H / 2;
    target_h = full_now || wide ? VIEW_W : VIEW_W / 2;
    output_flags = DISPLAY_TRANSFER_FLAGS | GX_TRANSFER_SCALING(!full_now ? GX_TRANSFER_SCALE_NO
                                                                : wide    ? GX_TRANSFER_SCALE_X
                                                                          : GX_TRANSFER_SCALE_XY);
}

static C3D_RenderTarget* target_make(void) {
    return C3D_RenderTargetCreate(target_w, target_h, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
}

void ctr_render_set_wide(int wide) {
    wide_want = wide != 0;
}

int ctr_render_wide(void) {
    return wide_now;
}

int ctr_render_top_w(void) {
    return wide_now > 0 ? 800 : 400;
}

/* as a frame begins: the target made again at its new size once the GPU is done with the last, VRAM's textures
   dropped first when there is no room for it */
static void wide_take(void) {
    int was = wide_now, was_h = target_h;
    if (wide_now >= 0 && wide_at_i < wide_at_n && ctr_presented >= (unsigned)wide_at[wide_at_i]) {
        wide_at_i++;
        wide_want = !wide_now;
    }
    if (wide_now < 0 || wide_want < 0 || wide_want == wide_now) {
        wide_want = -1;
        return;
    }
    if (c3d_queue != NULL) {
        gxCmdQueueWait(c3d_queue, -1);
    }
    wide_now = wide_want;
    wide_want = -1;
    sizes(wide_now);
    if (target_h != was_h) {
        C3D_RenderTargetDelete(target);
        target = target_make();
        if (target == NULL && tex_vram) {
            uint32_t fence_was = fence;
            fence = UINT32_MAX;
            retired_free();
            vram_drop_all();
            kept_drop_all();
            fence = fence_was;
            target = target_make();
        }
        if (target == NULL) {
            wide_now = was;
            sizes(was);
            target = target_make();
            if (target == NULL) {
                ctr_stop("There is no memory for the top screen's picture.");
            }
        }
        if (pillars_made != NULL) {
            linearFree(pillars_made);
            pillars_made = NULL;
        }
    }
    C3D_RenderTargetSetOutput(target, GFX_TOP, GFX_LEFT, output_flags);
    gfxSetWide(wide_now);
    layout(stretch_now);
    make_proj();
    pillars_setup();
    host_log("drawing: frame %u on, %s", ctr_presented + 1,
             wide_now == was ? "not wide: no room for the target" : wide_now ? "wide" : "not wide");
}

int ctr_render_init(int full, int stretch, int wide) {
    C3D_AttrInfo* attr;
    C3D_BufInfo* buf;
    float across;
    int i;

    lru_clear();
    direct_flush = !envIsHomebrew();
    host_log("cache flushes: %s", direct_flush ? "direct" : "through the GPU service");
    console_every = getenv("MVC2_CONSOLE_FLUSH") != NULL && strcmp(getenv("MVC2_CONSOLE_FLUSH"), "every") == 0;
    if (!console_every) {
        console_watch();
    }
    host_log("bottom screen: %s", console_every ? "flushed each frame" : "flushed only when its text was written to");
    cmd_self = direct_flush && getenv("MVC2_CMD_FLUSH") != NULL && strcmp(getenv("MVC2_CMD_FLUSH"), "self") == 0;
    if (cmd_self) {
        host_log("commands: flushed here before the frame's end, the GPU service's flush finding them clean "
                 "(MVC2_CMD_FLUSH=self)");
    }
    if (!C3D_Init(CMDBUF_SIZE)) {
        return 0;
    }
    tex_flush_init();
    full_now = full != 0;
    wide_now = wide < 0 ? -1 : wide != 0;
    sizes(wide_now > 0);
    layout(stretch);
    target = target_make();
    if (target == NULL) {
        return 0;
    }
    gfxSetWide(wide_now > 0);
    host_log("top screen: %s", wide_now < 0 ? "400 columns (no wide mode on this model)"
                               : wide_now  ? "800 columns (wide), the game's 640 one to one"
                                           : "400 columns");
    pillars_setup();
    if (pillars_once) {
        host_log("drawing: the pillars copied once, a frame clearing only the game's rows (MVC2_PILLARS_ONCE)");
    }
    if (pillars_line) {
        host_log("drawing: each pillar copied as one line (MVC2_PILLARS_LINE)");
    }
    stretch_at_n = frames_at("MVC2_STRETCH_AT", stretch_at);
    if (stretch_at_n != 0) {
        host_log("drawing: stretched or not by turns from frames %s (MVC2_STRETCH_AT)", getenv("MVC2_STRETCH_AT"));
    }
    wide_at_n = frames_at("MVC2_WIDE_AT", wide_at);
    if (wide_at_n != 0) {
        host_log("drawing: wide or not by turns from frames %s (MVC2_WIDE_AT)", getenv("MVC2_WIDE_AT"));
    }
    C3D_RenderTargetSetOutput(target, GFX_TOP, GFX_LEFT, output_flags);
    vbuf[0] = linearAlloc((sizeof(GV) + sizeof *L->idx) * MAX_VERTS);
    if (vbuf[0] == NULL) {
        return 0;
    }
    use_buffer(vbuf[0]);
    host_log("vertices: %d a frame, %u KB", MAX_VERTS, (unsigned)((sizeof(GV) + sizeof *L->idx) * MAX_VERTS >> 10));
    indexed = getenv("MVC2_INDEXED") == NULL || strcmp(getenv("MVC2_INDEXED"), "0") != 0;
    host_log("triangles: %s", indexed ? "drawn by index" : "each with its own vertices (MVC2_INDEXED=0)");
    raw_draws = getenv("MVC2_RAW_DRAWS") == NULL || strcmp(getenv("MVC2_RAW_DRAWS"), "0") != 0;
    raw_back = raw_draws && getenv("MVC2_RAW_BACK") != NULL && strcmp(getenv("MVC2_RAW_BACK"), "0") != 0;
    raw_bufs = raw_draws && getenv("MVC2_RAW_BUFS") != NULL && strcmp(getenv("MVC2_RAW_BUFS"), "0") != 0;
    if (raw_bufs) {
        host_log("draws: each mesh's whole buffer block before its draws, as citro3d binds it (MVC2_RAW_BUFS)");
    }
    if (raw_back) {
        host_log("draws: the first after a run of groups sent directly, citro3d given the CPU's vertices again "
                 "only before it next draws (MVC2_RAW_BACK)");
    }
    raw_front = raw_draws && getenv("MVC2_RAW_FRONT") != NULL && strcmp(getenv("MVC2_RAW_FRONT"), "0") != 0;
    if (raw_front) {
        host_log("draws: the first model group of a run sent directly too, its setup and uniforms written here (MVC2_RAW_FRONT)");
    }
    map_same = getenv("MVC2_MAP_SAME") != NULL && strcmp(getenv("MVC2_MAP_SAME"), "0") != 0;
    if (map_same) {
        host_log("draws: a depth map the same as the last not sent again (MVC2_MAP_SAME)");
    }
    draw_copy = getenv("MVC2_DRAW_COPY") != NULL && strcmp(getenv("MVC2_DRAW_COPY"), "1") == 0;
    if (draw_copy) {
        host_log("draws: their uniforms and state and group prep's copied by ldm and stm, not memcpy (MVC2_DRAW_COPY=1)");
    }
    ctr_memcpy_fast = getenv("MVC2_MEMCPY_FAST") != NULL && strcmp(getenv("MVC2_MEMCPY_FAST"), "1") == 0;
    if (ctr_memcpy_fast) {
        host_log("memcpy: 32 bytes at a time by ldm and stm where it can (MVC2_MEMCPY_FAST=1)");
    }
#ifdef MVC2_PROF
    draw_crc = getenv("MVC2_DRAW_CRC") != NULL && strcmp(getenv("MVC2_DRAW_CRC"), "0") != 0;
    line_count = getenv("MVC2_LINE_COUNT") != NULL && strcmp(getenv("MVC2_LINE_COUNT"), "1") == 0;
    if (line_count) {
        host_log("profile: the model draws' and the states loop's cache lines counted, a frame in 16 (MVC2_LINE_COUNT=1)");
    }
    if (getenv("MVC2_STORE_PROBE") != NULL && strcmp(getenv("MVC2_STORE_PROBE"), "1") == 0) {
        store_probe();
    }
#endif
    host_log("draws: %s", raw_draws ? "those that change only the effect, the fog, the texture or the combiner sent directly"
                                    : "all through citro3d (MVC2_RAW_DRAWS=0)");
    light_pal = getenv("MVC2_LIGHT_PAL") != NULL && strcmp(getenv("MVC2_LIGHT_PAL"), "0") != 0;
    if (light_pal) {
        host_log("lighting: on only for the palettes' draws (MVC2_LIGHT_PAL)");
    }
#ifdef MVC2_PROF
    gspSetEventCallback(GSPGPU_EVENT_P3D, list_end, NULL, false);
    gpu_split_init();
#endif
    aptHook(&away_hook, away_event, NULL);
    watch_regs = getenv("MVC2_WATCH_REGS") != NULL && strcmp(getenv("MVC2_WATCH_REGS"), "0") != 0;
    if (getenv("MVC2_GPU_WATCH") != NULL && strcmp(getenv("MVC2_GPU_WATCH"), "0") != 0) {
        s32 prio = 0x30;
        svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);
        wdraw = malloc(sizeof *wdraw * WATCH_DRAWS);
        gpu_watch = 1;
        watch_test = getenv("MVC2_WATCH_TEST") != NULL ? strtoul(getenv("MVC2_WATCH_TEST"), NULL, 10) : 0;
        host_log("watch: a frame the GPU hasn't finished in 500 ms, or no frame for 10 s, logged with the last frame's "
                 "model draws and %s, then the program stops (MVC2_GPU_WATCH)%s", HANG_FILE,
                 threadCreate(watch_main, NULL, 32 << 10, prio > 0x19 ? prio - 2 : 0x18, 0, true) == NULL
                     ? "; no thread for the 10 s check" : "");
    }
    {
        const char* e = getenv("MVC2_TRANS_STRIPS");
        trans_strips = e == NULL || strcmp(e, "0") != 0;
        strip_cuts = getenv("MVC2_STRIP_CUTS") == NULL || strcmp(getenv("MVC2_STRIP_CUTS"), "0") != 0;
        host_log("translucent sort: %s", !trans_strips ? "a triangle at a time, deep ones cut"
                                         : strip_cuts ? "a strip at a time, a deep one's triangles cut at the shallow ones' depths"
                                                      : "a strip at a time, none cut (MVC2_STRIP_CUTS=0)");
    }
    dvlb = DVLB_ParseFile((u32*)ctr_render_shbin, ctr_render_shbin_size);
    shaderProgramInit(&prog);
    shaderProgramSetVsh(&prog, &dvlb->DVLE[0]);
    C3D_BindProgram(&prog);
    u_proj = shaderInstanceGetUniformLocation(prog.vertexShader, "projection");
    u_tl = shaderInstanceGetUniformLocation(prog.vertexShader, "tl_unif");
    b_tl = shaderInstanceGetUniformLocation(prog.vertexShader, "tl");
    b_tl_lit = shaderInstanceGetUniformLocation(prog.vertexShader, "tl_lit");
    b_tl_vtxcol = shaderInstanceGetUniformLocation(prog.vertexShader, "tl_vtxcol");
    b_rcp_w = shaderInstanceGetUniformLocation(prog.vertexShader, "rcp_w");

    texel_centre = getenv("MVC2_TEXEL_CENTRE") == NULL || strcmp(getenv("MVC2_TEXEL_CENTRE"), "0") != 0;
    across = make_proj();
    if (across >= 0.0f) {
        smooth_2d = getenv("MVC2_SMOOTH_2D") == NULL || strcmp(getenv("MVC2_SMOOTH_2D"), "0") != 0;
        host_log("drawing: %s game pixel along, the pixels' centres %s; not with MVC2_TEXEL_CENTRE=0",
                 across == 0.5f ? "half a" : across == 0.0f ? "half a game pixel down and no" : "a tenth of a",
                 across == 0.1f ? "off the texels' edges" : "on the texels'");
        host_log("drawing: %s", smooth_2d ? "the game's point-sampled textures, not palettes, filtered at the game's pixels' corners"
                                          : "the game's point-sampled textures point-sampled (MVC2_SMOOTH_2D=0)");
    }

    attr = C3D_GetAttrInfo();
    AttrInfo_Init(attr);
    AttrInfo_AddLoader(attr, 0, GPU_FLOAT, 4);
    AttrInfo_AddLoader(attr, 1, GPU_FLOAT, 2);
    AttrInfo_AddLoader(attr, 2, GPU_UNSIGNED_BYTE, 4);
    cpu_attr = *attr;
    buf = C3D_GetBufInfo();
    BufInfo_Init(buf);
    BufInfo_Add(buf, L->verts, sizeof(GV), 3, 0x210);
    bound_verts = L->verts;
    buf_base = buf->base_paddr;
    AttrInfo_Init(&mesh_attr);
    AttrInfo_AddLoader(&mesh_attr, 0, GPU_FLOAT, 3);
    AttrInfo_AddLoader(&mesh_attr, 1, GPU_FLOAT, 2);
    AttrInfo_AddLoader(&mesh_attr, 2, GPU_UNSIGNED_BYTE, 4);
    C3D_BoolUnifSet(GPU_VERTEX_SHADER, b_tl, false);
    rcp_w = getenv("MVC2_RCP_W") == NULL || strcmp(getenv("MVC2_RCP_W"), "0") != 0;
    C3D_BoolUnifSet(GPU_VERTEX_SHADER, b_rcp_w, rcp_w);
    host_log("vertices: %s", rcp_w ? "w the shader's reciprocal of 1/w" : "w divided here (MVC2_RCP_W=0)");
    clear_all = getenv("MVC2_TEX_CLEAR") != NULL && strcmp(getenv("MVC2_TEX_CLEAR"), "all") == 0;
    vq_tiles = getenv("MVC2_VQ_TILES") == NULL || strcmp(getenv("MVC2_VQ_TILES"), "0") != 0;
    host_log("VQ textures: %s", vq_tiles ? "tiled from their source" : "unpacked, then tiled (MVC2_VQ_TILES=0)");
    pad_fast = getenv("MVC2_PAD_FAST") == NULL || strcmp(getenv("MVC2_PAD_FAST"), "0") != 0;
    single = getenv("MVC2_SINGLE") == NULL || strcmp(getenv("MVC2_SINGLE"), "0") != 0;
    trans_fast = getenv("MVC2_TRANS_FAST") == NULL || strcmp(getenv("MVC2_TRANS_FAST"), "0") != 0;
    host_log("translucent triangles: %s", trans_fast ? "those that need no cutting put together, a slice at a time"
                                                     : "each put on its own (MVC2_TRANS_FAST=0)");
    if (getenv("MVC2_SLICES") != NULL) {
        max_slices = atoi(getenv("MVC2_SLICES"));
        max_slices = max_slices < 2 ? 2 : max_slices > MAX_SLICES ? MAX_SLICES : max_slices;
        host_log("slices: at most %d (MVC2_SLICES)", max_slices);
    }
    sort_fit = getenv("MVC2_SORT_FIT") == NULL || strcmp(getenv("MVC2_SORT_FIT"), "0") != 0;
    host_log("slices: %s", sort_fit ? "their depths sorted in as many digits as cost least"
                                    : "their depths sorted in three digits (MVC2_SORT_FIT=0)");
    group_prep = getenv("MVC2_GROUP_PREP") == NULL || strcmp(getenv("MVC2_GROUP_PREP"), "0") != 0;
    group_early = getenv("MVC2_GROUP_EARLY") != NULL ? atoi(getenv("MVC2_GROUP_EARLY")) : 0;
    group_early = group_early == 1 || group_early == 2 ? group_early : 0;
    if (group_early) {
        host_log("models: the next group's words read ahead of it%s (MVC2_GROUP_EARLY=%d)",
                 group_early == 2 ? ", four lines at a time" : "", group_early);
    }
    host_log("model groups: %s", group_prep ? "their uniforms and depths worked out with the lists"
                                            : "their uniforms and depths worked out as they are drawn (MVC2_GROUP_PREP=0)");
    host_log("vertex depths and model matrices: %s", single ? "in single precision" : "in double precision (MVC2_SINGLE=0)");
    host_log("texture memory: %s", clear_all ? "all cleared (MVC2_TEX_CLEAR=all)" : "only what the texels don't fill cleared");
    rd_gpu_tl = 0;
    if (getenv("MVC2_MESH_POOL") != NULL) {
        int kb = atoi(getenv("MVC2_MESH_POOL"));
        mesh_pool_size = (u32)(kb < 64 ? 64 : kb > 4096 ? 4096 : kb) << 10;
    }
    if (getenv("MVC2_GPU_TL") != NULL && strcmp(getenv("MVC2_GPU_TL"), "0") == 0) {
        host_log("models: turned and lit here (MVC2_GPU_TL=0)");
    } else if ((gmesh = calloc(MAX_MESHES, sizeof *gmesh + sizeof *mesh_gone)) == NULL ||
               (mesh_pool = linearAlloc(mesh_pool_size)) == NULL ||
               osConvertVirtToPhys(mesh_pool) + mesh_pool_size - buf_base > 0x0FFFFFFFu) {
        /* the buffers' offsets have 28 bits */
        host_log("models: turned and lit here, %s", gmesh == NULL || mesh_pool == NULL ? "no memory for the GPU's meshes"
                                                                                     : "the meshes out of the GPU's reach");
        free(gmesh);
        gmesh = NULL;
        if (mesh_pool != NULL) {
            linearFree(mesh_pool);
            mesh_pool = NULL;
        }
    } else {
        mesh_gone = (uint16_t*)(gmesh + MAX_MESHES);
        holes[0].lo = 0;
        holes[0].hi = mesh_pool_size;
        nholes = 1;
        rd_gpu_tl = 1;
        host_log("models: opaque and punch-through groups turned and lit by the GPU, %lu KB for their meshes%s",
                 (unsigned long)mesh_pool_size >> 10, getenv("MVC2_MESH_POOL") != NULL ? " (MVC2_MESH_POOL)" : "");
        /* for finding a fault: MVC2_GPU_TL_KINDS, some of unlit, vtxcol and lit; MVC2_GPU_TL_MAX, groups a frame */
        if (getenv("MVC2_GPU_TL_KINDS") != NULL) {
            static const char* const name[] = { [RD_GPU_UNLIT] = "unlit", [RD_GPU_VTXCOL] = "vtxcol", [RD_GPU_LIT] = "lit" };
            const char* k = getenv("MVC2_GPU_TL_KINDS");
            rd_gpu_tl_kinds = 0;
            while (*k != 0) {
                size_t n = strcspn(k, ",");
                for (i = 0; i < 3; i++) {
                    rd_gpu_tl_kinds |= (strlen(name[i]) == n && strncmp(k, name[i], n) == 0) << i;
                }
                k += n + (k[n] == ',');
            }
            host_log("models: only %s%s%s on the GPU (MVC2_GPU_TL_KINDS)", rd_gpu_tl_kinds >> RD_GPU_UNLIT & 1 ? "unlit " : "",
                     rd_gpu_tl_kinds >> RD_GPU_VTXCOL & 1 ? "vertex-coloured " : "", rd_gpu_tl_kinds >> RD_GPU_LIT & 1 ? "lit" : "");
        }
        gpu_late_on = getenv("MVC2_GPU_TL_LATE") == NULL || strcmp(getenv("MVC2_GPU_TL_LATE"), "0") != 0;
        host_log("models: %s", gpu_late_on ? "a list's order-free groups drawn together after it"
                                           : "each group in its place in its list (MVC2_GPU_TL_LATE=0)");
        tl_overlap = getenv("MVC2_TL_OVERLAP") == NULL || strcmp(getenv("MVC2_TL_OVERLAP"), "0") != 0;
        slice_cut = getenv("MVC2_SLICE_CUT") == NULL || strcmp(getenv("MVC2_SLICE_CUT"), "0") != 0;
        if (getenv("MVC2_SLICE_CUT") != NULL && strcmp(getenv("MVC2_SLICE_CUT"), "all") == 0) {
            cut_slices = 0.0f;
        }
        far_reject = getenv("MVC2_FAR_REJECT") == NULL || strcmp(getenv("MVC2_FAR_REJECT"), "0") != 0;
        host_log("triangles: %s", far_reject ? "far ones wholly past an edge of the target dropped before they are cut"
                                             : "far ones all cut, those past the target's edges too (MVC2_FAR_REJECT=0)");
        if (getenv("MVC2_FAR_CUT") != NULL && atoi(getenv("MVC2_FAR_CUT")) > 0) {
            guard = atoi(getenv("MVC2_FAR_CUT"));
            far_bits = zbits((float)(guard + RD_WIDTH));
            host_log("triangles: those with a corner past %g pixels cut here to %g round the screen (MVC2_FAR_CUT)",
                     guard + RD_WIDTH, guard);
        }
        if (getenv("MVC2_TRANS_BIAS") != NULL) {
            trans_bias = atof(getenv("MVC2_TRANS_BIAS")) / 32768;
        }
        host_log("translucent triangles: %g/32768 of a slice nearer than the models%s", trans_bias * 32768,
                 getenv("MVC2_TRANS_BIAS") != NULL ? " (MVC2_TRANS_BIAS)" : "");
        host_log("triangles: %s", !slice_cut           ? "each left to the GPU's clip at its slice's ends (MVC2_SLICE_CUT=0)"
                                  : cut_slices == 0.0f ? "each crossing a slice's end cut there (MVC2_SLICE_CUT=all)"
                                                       : "those many slices deep cut at their slice's ends");
        host_log("models: %s", tl_overlap ? "each slice's clip a little wider for them, the narrowest given to the next"
                                          : "each clipped to its slice (MVC2_TL_OVERLAP=0)");
        tl_frags = getenv("MVC2_GPU_TL_FRAGS") == NULL || strcmp(getenv("MVC2_GPU_TL_FRAGS"), "0") != 0;
        prep_fast = getenv("MVC2_PREP_FAST") != NULL && strcmp(getenv("MVC2_PREP_FAST"), "1") == 0;
        for (i = 0; i < 256; i++) {
            by255[i] = (float)(uint32_t)i / 255.0f;
        }
        if (prep_fast) {
            host_log("models: group prep's sums with fewer steps (MVC2_PREP_FAST=1)");
        }
        prep_fast2 = prep_fast && getenv("MVC2_PREP_FAST2") != NULL && strcmp(getenv("MVC2_PREP_FAST2"), "1") == 0;
        if (prep_fast2) {
            host_log("models: more of group prep's sums with fewer steps (MVC2_PREP_FAST2=1)");
        }
        prep_pack = prep_fast2 && group_prep && getenv("MVC2_PREP_PACK") != NULL && strcmp(getenv("MVC2_PREP_PACK"), "1") == 0;
        if (prep_pack) {
            host_log("models: group prep's inputs packed as the groups are added (MVC2_PREP_PACK=1)");
        }
        draw_hot = group_prep && getenv("MVC2_DRAW_HOT") != NULL && strcmp(getenv("MVC2_DRAW_HOT"), "1") == 0;
        if (draw_hot) {
            host_log("models: what a group's draw reads of its mesh kept with its prep (MVC2_DRAW_HOT=1)");
        }
        prep_early = getenv("MVC2_PREP_EARLY") != NULL && strcmp(getenv("MVC2_PREP_EARLY"), "1") == 0;
        if (prep_early) {
            host_log("models: group prep's next lines read ahead (MVC2_PREP_EARLY=1)");
        }
        frag_call = tl_frags && getenv("MVC2_FRAG_CALL") != NULL && strcmp(getenv("MVC2_FRAG_CALL"), "1") == 0;
        frag_tail = frag_call ? linearMemAlign(8 * 4, 16) : NULL;
        if (frag_tail != NULL) {
            u32* w = frag_tail;
            CMD(GPUREG_FRAMEBUFFER_FLUSH, 0xF, 1);
            CMD(GPUREG_FRAMEBUFFER_INVALIDATE, 0xF, 1);
            CMD(GPUREG_EARLYDEPTH_CLEAR, 0xF, 1);
            CMD(GPUREG_FINALIZE, 0xF, 0x12345678);
            flush_one(frag_tail, 8 * 4);
            frag_tail_at = osConvertVirtToPhys(frag_tail);
            C3D_FrameEndHook(frag_end, NULL);
        } else {
            frag_call = 0;
        }
#ifdef MVC2_PROF
        perf_zones = getenv("MVC2_PERFZONES") != NULL && atoi(getenv("MVC2_PERFZONES")) != 0;
        if (perf_zones && getenv("MVC2_PERFCTR") != NULL && atoi(getenv("MVC2_PERFCTR")) != 0) {
            host_log("perf zones: not with MVC2_PERFCTR, which sets core 0's events itself");
            perf_zones = 0;
        }
        if (perf_zones) {
            u64 x;
            u32 a[3], b[3];
            uint64_t t = svcGetSystemTick();
            if (envIsHomebrew() || R_FAILED(svcControlPerformanceCounter(&x, PERFCOUNTEROP_ENABLE, 0, 0))) {
                host_log("perf zones: the counters not to be had");
                perf_zones = 0;
            } else {
                pfz_read(a);
                while (svcGetSystemTick() - t < 100000u) {
                }
                pfz_read(b);
                perf_zones = a[2] != b[2];
                host_log("perf zones: %s", perf_zones ? "the lists' steps counted on the core that runs them (MVC2_PERFZONES)"
                                                      : "the cycle counter doesn't run (an emulator?), so off");
            }
        }
        prof_tl = getenv("MVC2_PROF_TL") != NULL ? atoi(getenv("MVC2_PROF_TL")) : 0;
        prof_tl = prof_tl > 0 ? prof_tl : 0;
        if (prof_tl == 1) {
            host_log("profile: the groups' draws timed in their steps (MVC2_PROF_TL), a tick read each");
        } else if (prof_tl > 1) {
            host_log("profile: one group's draw in %d timed in its steps (MVC2_PROF_TL=%d), its times and counts taken %d times",
                     prof_tl, prof_tl, prof_tl);
        }
#endif
        frag_inline = tl_frags && !frag_call && getenv("MVC2_FRAG_INLINE") != NULL && strcmp(getenv("MVC2_FRAG_INLINE"), "0") != 0;
        host_log("models: %s", frag_call ? "a mesh's draws called where they were made (MVC2_FRAG_CALL=1)"
                               : frag_inline ? "a mesh's draws written as they were made, from the mesh (MVC2_FRAG_INLINE)"
                               : tl_frags ? "a mesh's draws sent as the words made with it"
                                          : "a mesh's draws sent a run at a time (MVC2_GPU_TL_FRAGS=0)");
        {
            /* translucent groups need the strip sort, which places them */
            const char* e = getenv("MVC2_GPU_TL_TRANS");
            rd_gpu_tl_trans = trans_strips && (e == NULL || strcmp(e, "0") != 0);
            host_log("models: %s", rd_gpu_tl_trans ? "translucent groups too, each whole at its place in the translucent list"
                                   : trans_strips ? "translucent groups turned and lit here (MVC2_GPU_TL_TRANS)"
                                                  : "translucent groups turned and lit here, without the strip sort");
            if (rd_gpu_tl_trans) {
                const char* d = getenv("MVC2_GPU_TL_TRANS_DEEP");
                rd_gpu_tl_trans_deep = d != NULL ? (float)atof(d) : TRANS_DEEP;
                if (rd_gpu_tl_trans_deep > 0.0f) {
                    const char* t = getenv("MVC2_GPU_TL_TWO_PASS");
                    rd_gpu_tl_two_pass = t == NULL || strcmp(t, "0") != 0;
                    host_log("models: but those whose farthest w is over %.2f times their nearest turned and lit here%s%s",
                             rd_gpu_tl_trans_deep, d != NULL ? " (MVC2_GPU_TL_TRANS_DEEP)" : "",
                             rd_gpu_tl_two_pass ? ", unless two-sided: those drawn back faces first, then front"
                                                : " (two-sided ones too: MVC2_GPU_TL_TWO_PASS=0)");
                }
            }
        }
        if (getenv("MVC2_GPU_TL_LIT") != NULL && strcmp(getenv("MVC2_GPU_TL_LIT"), "0") == 0) {
            rd_gpu_tl_kinds &= ~(1 << RD_GPU_LIT);
            host_log("models: lit groups turned and lit here (MVC2_GPU_TL_LIT=0)");
        }
        if (getenv("MVC2_GPU_TL_MAX") != NULL) {
            int n = atoi(getenv("MVC2_GPU_TL_MAX"));
            rd_gpu_tl_max = n < 0 ? 0 : n > RD_GPU_GROUPS ? RD_GPU_GROUPS : n;
            host_log("models: at most %d groups a frame on the GPU (MVC2_GPU_TL_MAX)", rd_gpu_tl_max);
        }
        rd_gpu_tl_nearclip = getenv("MVC2_TL_NEARCLIP") != NULL && strcmp(getenv("MVC2_TL_NEARCLIP"), "0") != 0;
        if (rd_gpu_tl_nearclip) {
            host_log("models: near-clipped here (MVC2_TL_NEARCLIP)");
        }
    }

    C3D_CullFace(GPU_CULL_NONE);
    for (i = 0; i < 64; i++) {
        int x = i & 7, y = i >> 3, k, m = 0;
        for (k = 0; k < 3; k++) {
            m |= ((x >> k) & 1) << (2 * k) | ((y >> k) & 1) << (2 * k + 1);
        }
        morton[i] = (uint8_t)m;
    }
    light_setup();
    C3D_LightEnvBind(&env);
    lists1 = rd_frames[1] != NULL && getenv("MVC2_LISTS_CORE1") != NULL && strcmp(getenv("MVC2_LISTS_CORE1"), "0") != 0;
    if (lists1) {
        lists1_start();
    } else if (rd_frames[1] != NULL) {
        pipe_start();
    }
    /* MVC2_TEX_SEEN=1, with the drawing a frame behind: no texture made at the handoff for a state whose triangles
       are all translucent ones with no pixel on the screen, none of which is drawn */
    rd_seen = (pipe_on || lists1) && getenv("MVC2_TEX_SEEN") != NULL && strcmp(getenv("MVC2_TEX_SEEN"), "0") != 0;
    tex_current = getenv("MVC2_TEX_CURRENT") != NULL && strcmp(getenv("MVC2_TEX_CURRENT"), "0") != 0;
    if (tex_current) {
        host_log("textures: one made and unchanged taken as it was kept, and copied once a frame (MVC2_TEX_CURRENT)");
    }
    if (rd_seen) {
        host_log("textures: made at the handoff only for the states with something to draw (MVC2_TEX_SEEN)");
    }
    tex_lean = getenv("MVC2_TEX_LEAN") != NULL && strcmp(getenv("MVC2_TEX_LEAN"), "0") != 0;
    tex_ahead = getenv("MVC2_TEX_AHEAD") != NULL && strcmp(getenv("MVC2_TEX_AHEAD"), "0") != 0;
    if (tex_ahead) {
        host_log("textures: those just loaded made in a frame that draws nothing (MVC2_TEX_AHEAD)");
    }
    from_serial = getenv("MVC2_TEX_FROM_SERIAL") != NULL && strcmp(getenv("MVC2_TEX_FROM_SERIAL"), "0") != 0;
    if (from_serial) {
        host_log("textures: one to convert copied from another holding the same serial's texels (MVC2_TEX_FROM_SERIAL)");
    }
    copy_same = tex_current && getenv("MVC2_TEX_COPY_SAME") != NULL && strcmp(getenv("MVC2_TEX_COPY_SAME"), "0") != 0;
    if (copy_same) {
        host_log("textures: a frame's copy of one made only when it changed since that frame's table last had it "
                 "(MVC2_TEX_COPY_SAME)");
    }
    if (tex_lean) {
        host_log("textures: a state's looked up once, and not timed one by one outside the profile (MVC2_TEX_LEAN)");
    }
    /* MVC2_TEX_VRAM=1 (MVC2_TEX_STAGE: the staging ring's KB, two banks); =2 converted straight into VRAM
       (MVC2_TEX_VRAM_NOFLUSH=1: not flushed, for VRAM the CPU does not cache) */
    if (getenv("MVC2_TEX_VRAM") != NULL && strcmp(getenv("MVC2_TEX_VRAM"), "0") != 0) {
        const char* e = getenv("MVC2_TEX_STAGE");
        u32 kb = e != NULL && atoi(e) > 0 ? (u32)atoi(e) : 1024;
        staging_size = (kb << 10) / 2 & ~127u;
        if (getenv("MVC2_TEX_VRAM_KEEP") != NULL) {
            vram_keep = (u32)atoi(getenv("MVC2_TEX_VRAM_KEEP")) << 10;
        }
        if (pipe_on) {
            host_log("textures: none in VRAM, the drawing thread makes them beside the frame's copies (MVC2_TEX_VRAM)");
        } else if ((atoi(getenv("MVC2_TEX_VRAM")) != 2 || !ctr_vram_writable) &&
                   (staging_buf = linearAlloc(staging_size * 2)) == NULL) {
            host_log("textures: none in VRAM, no linear memory for the staging ring (MVC2_TEX_VRAM)");
        } else {
            tex_vram = 1;
            vram_direct = staging_buf == NULL;
            vram_noflush = vram_direct && getenv("MVC2_TEX_VRAM_NOFLUSH") != NULL &&
                           strcmp(getenv("MVC2_TEX_VRAM_NOFLUSH"), "0") != 0;
            vram_moves = getenv("MVC2_TEX_VRAM_MOVE") == NULL || strcmp(getenv("MVC2_TEX_VRAM_MOVE"), "0") != 0;
            tex_unpark = vram_moves && getenv("MVC2_TEX_UNPARK") != NULL && strcmp(getenv("MVC2_TEX_UNPARK"), "0") != 0;
            if (tex_unpark) {
                host_log("textures: the least recently drawn in VRAM freed to move one left in linear memory there "
                         "(MVC2_TEX_UNPARK)");
            }
            if (!apt_hooked) {
                aptHook(&apt_cookie, apt_event, NULL);
                apt_hooked = 1;
            }
            if (vram_direct) {
                host_log("textures: those of 128x128 texels or more in VRAM, %lu KB of it free and %lu KB kept so, "
                         "converted straight into it and %s; those left in linear memory %s (MVC2_TEX_VRAM=2)",
                         (unsigned long)(vramSpaceFree() >> 10), (unsigned long)(vram_keep >> 10),
                         vram_noflush ? "not flushed (MVC2_TEX_VRAM_NOFLUSH)" : "flushed",
                         vram_moves ? "moved there as they are drawn" : "left there");
            } else {
                host_log("textures: those of 128x128 texels or more in VRAM, %lu KB of it free and %lu KB kept so, "
                         "through a staging ring of 2 x %lu KB; those left in linear memory %s (MVC2_TEX_VRAM)",
                         (unsigned long)(vramSpaceFree() >> 10), (unsigned long)(vram_keep >> 10),
                         (unsigned long)(staging_size >> 10), vram_moves ? "moved there as they are drawn" : "left there");
            }
        }
    }
    if (tex_vram && !vram_direct && ctr_vram_writable && getenv("MVC2_TEX_GX") != NULL &&
        strcmp(getenv("MVC2_TEX_GX"), "0") != 0) {
        int flip = gx_test();
        tex_gx = flip >= 0;
        gx_flip = tex_gx ? flip : 0;
        if (tex_gx) {
            host_log("textures: those of 64x64 texels or more with one level, unpadded, tiled by the GPU as they are "
                     "copied into VRAM (MVC2_TEX_GX, %s)", flip ? "flipped" : "not flipped");
        } else {
            host_log("textures: tiled by the CPU, the GPU's tiling differs from it (MVC2_TEX_GX)");
        }
    }
    /* MVC2_GROUP_INFO=1: the GPU's groups' depths taken as they are added, for the first pass over them */
    rd_group_info = rd_gpu_tl && getenv("MVC2_GROUP_INFO") != NULL && strcmp(getenv("MVC2_GROUP_INFO"), "0") != 0;
    if (rd_group_info) {
        host_log("models: their groups' depths kept as they are added (MVC2_GROUP_INFO)");
    }
    if (!pipe_on && !lists1 && (getenv("MVC2_EARLY_LISTS") == NULL || strcmp(getenv("MVC2_EARLY_LISTS"), "0") != 0)) {
        /* textures the frame in flight may read go back after it */
        retired = malloc(sizeof *retired * MAX_TEX);
        early = retired != NULL;
#ifdef MVC2_PROF
        early_sweep = early && getenv("MVC2_EARLY_SWEEP") != NULL && strcmp(getenv("MVC2_EARLY_SWEEP"), "0") != 0;
#endif
    }
    if (!pipe_on && !lists1) {
        host_log("lists: %s", early ? "built while the GPU draws the frame before, in the vertices it doesn't read"
                                    : "built after the GPU has drawn the frame before (MVC2_EARLY_LISTS=0)");
    }
    nsplits = 0;
    last_words = 0;
    if (getenv("MVC2_LIST_SPLIT") != NULL && strcmp(getenv("MVC2_LIST_SPLIT"), "0") != 0) {
        const char* s = strcmp(getenv("MVC2_LIST_SPLIT"), "1") == 0 ? "8,30" : getenv("MVC2_LIST_SPLIT");
        const char* why = pipe_on ? "the drawing thread" : frag_call ? "MVC2_FRAG_CALL" : c3d_queue == NULL ? "no queue"
                                                                                                         : NULL;
#ifdef MVC2_PROF
        why = why == NULL && gpu_probe_on() ? "MVC2_GPU_PROBE" : why;
#endif
        while (why == NULL && *s != '\0' && nsplits < MAX_SPLITS) {
            int p = atoi(s);
            if (p > 0 && p < 100 && (nsplits == 0 || p > split_pct[nsplits - 1])) {
                split_pct[nsplits++] = (uint8_t)p;
            }
            s += strcspn(s, ",");
            s += *s == ',';
        }
        if (nsplits > 0) {
            char at[32];
            int k, n = 0;
            for (k = 0; k < nsplits; k++) {
                n += snprintf(at + n, sizeof at - (size_t)n, "%s%d%%", k > 0 ? ", " : "", split_pct[k]);
            }
            host_log("lists: sent in up to %d parts, split at the first draw past %s of the last frame's words "
                     "(MVC2_LIST_SPLIT)", nsplits + 1, at);
        } else {
            host_log("lists: sent whole, %s (MVC2_LIST_SPLIT)", why != NULL ? why : "no split point given");
        }
    }
    host_log("linear heap free %lu KB after the renderer's buffers", (unsigned long)linearSpaceFree() >> 10);
    return 1;
}

/* MVC2_DUMP=n: the batches of the game's frame n, in the log */
static int dump_wanted(void) {
    const char* want = getenv("MVC2_DUMP");
    return want != NULL && (unsigned)atoi(want) == ctr_presented;
}

static void dump(void) {
    int i;
    host_log("background z %.9g %.9g %.9g, corners %.9g %.9g %.9g %.9g, %d triangles", fr->background[0].z,
             fr->background[1].z, fr->background[2].z, L->bg_vtx[0].z, L->bg_vtx[1].z, L->bg_vtx[2].z, L->bg_vtx[3].z, L->nbg);
    for (i = 0; i <= L->nslices; i++) {
        host_log("slice %d from %.9g", i, L->slice_lo[i]);
    }
    for (i = 0; i < L->nbatches; i++) {
        const BATCH* b = &L->batches[i];
        const RD_STATE* s = &fr->states[b->state];
        const GV* v;
        if (b->count == GPU_GROUP) {
            const RD_GPU_GROUP* g = &fr->groups[b->first].g;
            host_log("batch %d: slice %d list %d GPU group %d: mesh %d kind %d w %.9g to %.9g; bil %d tex %dx%d f%d pal %d",
                     i, b->slice, s->list, b->first, g->mesh, g->kind, g->wlo, g->whi, s->bilinear,
                     s->tex ? s->tex->width : 0, s->tex ? s->tex->height : 0, s->tex ? s->tex->format : -1, s->palette);
            continue;
        }
        v = &L->verts[indexed ? L->idx[b->first] : b->first];
        host_log("batch %d: slice %d list %d depth %d zw %d blend %d/%d shade %d ua %d ta %d bil %d off %d(%06X) tex %dx%d f%d pal %d "
                 "tris %d at %.0f,%.0f c %08X", i, b->slice, s->list, s->depth, s->zwrite, s->src, s->dst, s->shading, s->use_alpha,
                 s->tex_alpha, s->bilinear, s->offset, b->offset, s->tex ? s->tex->width : 0, s->tex ? s->tex->height : 0,
                 s->tex ? s->tex->format : -1, s->palette, b->count / 3, v->x - GAME_X, v->y, v->c);
    }
}

/* the sums as the frames drawn to the end left them, and as the last ctr_render_stats saw them */
static uint64_t done_ticks[CTR_TICKS], seen_ticks[CTR_TICKS];
static uint32_t done_count[CTR_COUNTS], seen_count[CTR_COUNTS];

/* while the drawing thread waits */
static void stats_done(void) {
    memcpy(done_ticks, ctr_render_ticks, sizeof done_ticks);
    memcpy(done_count, ctr_render_count, sizeof done_count);
}

void ctr_render_stats(uint64_t ticks[CTR_TICKS], uint32_t counts[CTR_COUNTS]) {
    int i;
    for (i = 0; i < CTR_TICKS; i++) {
        ticks[i] = done_ticks[i] - seen_ticks[i];
        seen_ticks[i] = done_ticks[i];
    }
    for (i = 0; i < CTR_COUNTS; i++) {
        counts[i] = done_count[i] - seen_count[i];
        seen_count[i] = done_count[i];
    }
}

/* the bottom screen's text, which the CPU writes, flushed at the end of a frame it was written in; the GPU's own
   inputs are flushed as they are made, so the frame's end flushes only its commands, not all of linear memory.
   A flush reaches the cache of the core it runs on: the text is the game's thread's */
static void flush_console(void) {
    u16 w, h;
    u8* fb;
    if (!console_every && !__atomic_exchange_n(&console_written, 0, __ATOMIC_ACQ_REL)) {
        return;
    }
    fb = gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, &w, &h);
    if ((u32)fb - OS_VRAM_VADDR < OS_VRAM_SIZE) {
        /* MVC2_VRAM_FB: through the GPU service, as libctru's own flush, so that a failed direct call there
           leaves the direct flushes on */
        PROF_START(t0);
        GSPGPU_FlushDataCache(fb, (u32)w * h * gspGetBytesPerPixel(gfxGetScreenFormat(GFX_BOTTOM)));
        PROF_STOP(PZ_CONSOLE_FLUSH, t0);
        return;
    }
    flush_range(fb, (u32)w * h * gspGetBytesPerPixel(gfxGetScreenFormat(GFX_BOTTOM)), PZ_CONSOLE_FLUSH);
}

void ctr_render_note(void) {
    console_every = 1;
    flush_console();
}

#ifdef MVC2_PROF
/* the states loop's lines for state i, by the way it is about to go: find_tex's probes, and what the call changes
   read before it does */
typedef struct {
    int path, last;                 /* 0 not seen, 1 the last one's texture, 2 made */
    GTEX* g;
    uint32_t used, copied;
    int prev, next, tail;
    int pad, was_unpadded, was_pad;
} LN_STATE;

static void state_lines_pre(LN_STATE* q, const RD_FRAME* f, int i, int last) {
    LINES* s = &ln_states;
    const RD_TEXTURE* t = f->states[i].tex;
    uint32_t h, k;
    ln_item(s);
    q->g = NULL;
    q->last = last;
    if (rd_seen) {
        LN_F(s, LS_SEEN, f->seen[i]);
        if (!f->seen[i]) {
            q->path = 0;
            return;
        }
    }
    LN_F(s, LS_STATE, f->states[i].tex);
    if (last >= 0) {
        LN_F(s, LS_STATE, f->states[last].tex);
        if (t == f->states[last].tex) {
            q->path = 1;
            return;
        }
    }
    q->path = 2;
    if (t == NULL) {
        return;
    }
    LN_F(s, LS_STATE, f->states[i]);
    {
        const RD_STATE* rs = &f->states[i];
        q->pad = smooth_2d && !rs->bilinear && rs->tex_alpha && t->levels == 0 &&
                 (t->format == RD_ARGB4444 || t->format == RD_ARGB1555) && t->width * t->height <= PAD_MAX &&
                 (rs->list == RD_PUNCH || (rs->list == RD_TRANS && rs->src == RD_SRC_ALPHA));
    }
    LN_F(s, LS_SRC, t->width);
    LN_F(s, LS_SRC, t->height);
    LN_F(s, LS_SRC, t->format);
    LN_F(s, LS_SRC, t->texels);
    LN_F(s, LS_SRC, t->serial);
    LN_F(s, LS_SRC, t->levels);
    h = (uint32_t)(uintptr_t)t * 2654435761u >> 20;
    for (k = 0; k < MAX_TEX; k++) {
        const GTEX* g = &gtex[(h + k) & (MAX_TEX - 1)];
        LN_F(s, LS_FIND, g->key);
        if (g->key == t || g->key == NULL) {
            break;
        }
        LN_F(s, LS_FIND, g->ok);
    }
    q->g = find_tex(t);
    if (q->g != NULL) {
        int gi = (int)(q->g - gtex);
        q->used = q->g->used;
        q->copied = q->g->copied;
        q->was_unpadded = q->g->unpadded;
        q->was_pad = q->g->pad;
        q->prev = lru_prev(gi);
        q->next = lru_next(gi);
        q->tail = lru_prev(LRU_END);
    }
}

static void state_lines(const LN_STATE* q, const RFRAME* r, int i) {
    LINES* s = &ln_states;
    GTEX* g = q->g;
    int gi;
    LN_F(s, LS_ST, r->st[i]);
    if (q->path == 1) {
        LN_F(s, LS_ST, r->st[q->last]);
    }
    if (g == NULL || r->st[i].tex != g - gtex) {
        return;
    }
    gi = (int)(g - gtex);
    LN_F(s, LS_GTEX, g->key);
    LN_F(s, LS_GTEX, g->ok);
    LN_F(s, LS_GTEX, g->serial);
    LN_F(s, LS_GTEX, g->pad);
    LN_F(s, LS_GTEX, g->unpadded);
    if (q->pad && !q->was_unpadded && !q->was_pad) {
        LN_F(s, LS_GTEX, g->changed);
    }
    LN_F(s, LS_GTEX, g->used);
    if (tex_current) {
        LN_F(s, LS_GTEX, g->tw);
        LN_F(s, LS_GTEX, g->tlevels);
        LN_F(s, LS_GTEX, g->su);
        LN_F(s, LS_GTEX, g->copied);
    } else {
        LN_F(s, LS_GTEX, g->width);
        LN_F(s, LS_GTEX, g->height);
    }
    if (q->used != frame_no) {
        LN_F(s, LS_LRU, lru_prev(gi));
        LN_F(s, LS_LRU, lru_next(gi));
        LN_F(s, LS_LRU, lru_next(q->prev));
        LN_F(s, LS_LRU, lru_prev(q->next));
        LN_F(s, LS_LRU, lru_prev(LRU_END));
        LN_F(s, LS_LRU, lru_next(LRU_END));
        LN_F(s, LS_LRU, lru_next(q->tail));
    }
    if (!tex_current || (q->copied != frame_no && g->copied == frame_no)) {
        LN_F(s, LS_COPY, r->tex[gi]);
        LN_F(s, LS_COPY, g->tex);
    }
}
#endif

/* MVC2_TEX_AHEAD=1: in a frame that draws nothing, the textures of 128x128 texels or more that the game loaded since
   made as their first draw would make them, while there is room without freeing others: a fight's first frame then
   finds its stage's made in the frame of its load. Not VQ ones (only their source is kept), the palettes' (a byte a
   texel), nor those MVC2_SMOOTH_2D may pad when first drawn */
static uint32_t ahead_seen[NL_MAX_TEX];     /* each slot's serial when last looked at */

static void textures_ahead(void) {
    int k, short_was = early_short;
    for (k = 0; k < NL_MAX_TEX; k++) {
        const RD_TEXTURE* t = &nl_tex[k].tex;
        int w, h, levels;
        u32 size;
        GTEX* g;
        if (!nl_tex[k].loaded || t->serial == ahead_seen[k]) {
            continue;
        }
        ahead_seen[k] = t->serial;
        if (t->texels == NULL || t->width * t->height < 128 * 128 || t->width > 1024 || t->height > 1024 ||
            t->unpack != NULL || is_pal(t->format) ||
            (smooth_2d && t->levels == 0 && (t->format == RD_ARGB4444 || t->format == RD_ARGB1555) &&
             t->width * t->height <= PAD_MAX)) {
            continue;
        }
        w = pow2(t->width);
        h = pow2(t->height);
        levels = gpu_levels(t, w, h);
        size = C3D_TexCalcTotalSize((u32)(w * h * 2), levels);
        if (!(tex_vram && (vram_direct || staging_room(size)) && vramSpaceFree() >= size + vram_keep) &&
            linearSpaceFree() < (u32)(w * h * 2 * (levels > 0 ? 4 : 3) / 3) + 2 * LINEAR_KEEP) {
            continue;
        }
        g = find_tex(t);
        if (g == NULL || (g->key == t && g->ok && g->serial == t->serial)) {
            continue;
        }
        if (gpu_tex_at(g, t, 0) != NULL) {
            g->ahead = 1;
            ahead_made++;
        }
    }
    early_short = short_was;
}

/* on the game's thread: the frame's textures made, and the state the draws read copied from the game's.
   Without the drawing thread a state's texture is made when its first triangle is drawn */
static void take_frame(RFRAME* r, RD_FRAME* f) {
    static int told;
    int i;
    PROF_START(p0);
    frame_no++;
    fence = pipe_on || early || lists1 ? frame_no - 1 : frame_no;
    retired_free();
    if (tex_vram) {
        if (vram_lost) {
            vram_lost = 0;
            vram_drop_all();
        }
        if (frame_no % 600 == 0) {
            vram_log(0);
        }
    }
    if (from_serial && frame_no % 600 == 0) {
        host_log("textures: %.1f a frame copied from another of the same serial (MVC2_TEX_FROM_SERIAL)",
                 serial_copies / 600.0);
        serial_copies = 0;
    }
    ctr_render_count[CTR_COUNT_DROPPED] += (uint32_t)f->dropped;
    ctr_render_count[CTR_COUNT_MOVED] += (uint32_t)f->moved;
    if (f->dropped > 0 && !told) {
        host_log("frame %u: the pool's %d triangles all used, %d more not drawn (counted in the frame times from here)",
                 frame_no, RD_POOL_TRIS, f->dropped);
        told = 1;
    }
    if (!pipe_on && !lists1) {
        for (i = 0; i < f->nstates; i++) {
            r->st[i].tex = TEX_LATER;
        }
    } else {
        /* every state's texture, the frame's triangles not read here: a state comes with what is drawn in it */
        int last = -1;
        PROF_START(p1);
#ifdef MVC2_PROF
        LN_STATE q = { 0 };
        ln_states_on = line_count && frame_no % 16 == 0;
        if (ln_states_on) {
            ln_begin(&ln_states);
        }
#endif
        for (i = 0; i < f->nstates; i++) {
#ifdef MVC2_PROF
            if (ln_states_on) {
                state_lines_pre(&q, f, i, last);
            }
#endif
            if (rd_seen && !f->seen[i]) {
                memset(&r->st[i], 0, sizeof r->st[i]);
                r->st[i].tex = -1;
            } else if (last >= 0 && f->states[i].tex == f->states[last].tex) {
                r->st[i] = r->st[last];
            } else {
                make_state_tex(r, f, i);
                last = i;
            }
#ifdef MVC2_PROF
            if (ln_states_on) {
                state_lines(&q, r, i);
            }
#endif
        }
#ifdef MVC2_PROF
        if (ln_states_on) {
            ln_end(&ln_states, PZ_LN_STATES, LS_N);
        }
#endif
        PROF_STOP(PZ_STATES, p1);
        PROF_COUNT(PZ_STATES, (uint32_t)f->nstates);
    }
    if (r->palette_serial != rd_palette_serial) {
        memcpy(r->palette, rd_palette, sizeof r->palette);
        r->palette_serial = rd_palette_serial;
    }
    if (frame_no % 600 == 0 && gx_textures != 0) {
        host_log("textures: %lu tiled by the GPU (MVC2_TEX_GX)", (unsigned long)gx_textures);
    }
    if (tex_ahead) {
        if (f->nstates == 0) {
            textures_ahead();
        }
        if (frame_no % 600 == 0 && ahead_made != 0) {
            host_log("textures: %lu made ahead, %lu of them drawn since (MVC2_TEX_AHEAD)", (unsigned long)ahead_made,
                     (unsigned long)ahead_drawn);
        }
    }
    r->fog = rd_fog;
    r->frame = f;
    tex_flush();
    PROF_STOP(PZ_HANDOFF, p0);
}

/* MVC2_EARLY_LISTS, serial: the frame in flight reads verts from ring_v[0] to ring_v[1] and idx from ring_i[0] to
   ring_i[1]; the next frame's lists go in the larger part of each that is left */
static int ring_v[2], ring_i[2], early_states;

static void early_place(void) {
    int vafter = MAX_VERTS - ring_v[1], iafter = MAX_VERTS - ring_i[1];
    int v0 = vafter >= ring_v[0] ? ring_v[1] : 0, i0 = iafter >= ring_i[0] ? ring_i[1] : 0;
    int vroom = vafter >= ring_v[0] ? vafter : ring_v[0], iroom = iafter >= ring_i[0] ? iafter : ring_i[0];
    L->verts = vbuf[0] + v0;
    L->idx = (uint16_t*)(vbuf[0] + MAX_VERTS) + i0;
    max_idx = vroom - 9 < iroom ? vroom - 9 : iroom;
    if (max_idx < 0) {
        max_idx = 0;
    }
    early_short = 0;
    early_states = fr->nstates;
}

/* after the wait, when the lists didn't fit: built again as before, in all of the vertices, with the textures that
   found no room tried again */
static void early_again(void) {
    int i;
    fr->nstates = early_states;
    for (i = 0; i < fr->nstates; i++) {
        if (rf->st[i].tex == -1 && fr->states[i].tex != NULL) {
            rf->st[i].tex = TEX_LATER;
        }
    }
    retired_free();
    use_buffer(vbuf[0]);
    max_idx = MAX_VERTS;
    build();
    tex_flush();
    ctr_render_count[CTR_COUNT_EARLY_AGAIN]++;
}

static void early_keep(void) {
    ring_v[0] = (int)(L->verts - vbuf[0]);
    ring_v[1] = L->nverts + 9 > MAX_VERTS ? MAX_VERTS : ring_v[0] + L->nverts + 9;
    ring_i[0] = (int)(L->idx - (uint16_t*)(vbuf[0] + MAX_VERTS));
    ring_i[1] = ring_i[0] + L->nidx;
}

static uint64_t lists1_ticks;       /* core 0's part of the lists, its wait for core 1 in it */

/* with the drawing thread the lists are built before the wait for the GPU, into the other vertices */
static void render(void) {
    static int parity;
    uint64_t t0 = svcGetSystemTick(), t1, t2, t3, t4, lists = 0;
    watch_phase = "building the lists";
    if (early_sweep && early != ((ctr_presented / 600) % 2 == 0)) {
        early = !early;
        host_log("lists: %s from frame %u (MVC2_EARLY_SWEEP)", early ? "built early" : "built after the wait", ctr_presented + 1);
    }
    if (pipe_on) {
        use_buffer(vbuf[parity ^= 1]);
        build();
        lists = svcGetSystemTick() - t0;
    } else if (early) {
        take_frame(rframes[0], rd_frame);
        rf = rframes[0];
        fr = rd_frame;
        t0 = svcGetSystemTick();
        early_place();
        build();
        tex_flush();
        lists = svcGetSystemTick() - t0;
    }
    if (lists1) {
        lists = lists1_ticks;
    }
    t1 = svcGetSystemTick();
    /* the top screen's vertical blank, timed apart from the wait for the GPU; not C3D_FrameSync, which waits
       for both screens'. The event stays set from a blank that came since the last wait, so a late frame
       goes on at once */
    {
        PROF_START(pv);
        watch_phase = "waiting for the vertical blank";
        gspWaitForEvent(GSPGPU_EVENT_VBlank0, false);
        PROF_STOP(PZ_VBLANK, pv);
    }
    {
        PROF_START(pg);
        watch_phase = "waiting for the GPU";
        if (gpu_watch) {
            watched_begin();
        } else {
            if (nsplits != 0) {
                split_wait();
            }
            C3D_FrameBegin(0);
        }
        PROF_STOP(PZ_GPU_WAIT, pg);
#ifdef MVC2_PROF
        gpu_free_at = prof_now();
        gpu_split_begun(pg);
        if (list_sent != 0 && list_done != 0 && (int32_t)(list_done - list_sent) > 0) {
            PROF_ADD(PZ_GPU_LIST, list_done - list_sent);
        }
        list_sent = 0;
#endif
    }
    frame_cmd = gpuCmdBuf;
    split_begin();
    watch_phase = "building the lists";
    t2 = svcGetSystemTick();
    if (early || lists1) {
        fence = frame_no;
        if (early_short) {
            early_again();
        }
        early_keep();
    } else if (!pipe_on) {
        uint64_t tb;
        take_frame(rframes[0], rd_frame);
        rf = rframes[0];
        fr = rd_frame;
        use_buffer(vbuf[0]);
        max_idx = MAX_VERTS;
        tb = svcGetSystemTick();
        build();
        tex_flush();
        lists = svcGetSystemTick() - tb;
        early_keep();
    }
    t3 = svcGetSystemTick();
    PFZ_START(qd);
    PROF_START(pb);
    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, u_proj, &proj);
    proj_now = 0;
    if (pillars_once && !pillars_new) {
        /* C3D_RenderTargetClear's fill, the colour's from the game's first row to its last */
        u32 *c = (u32*)target->frameBuf.colorBuf + game_y0 * target_w, *d = (u32*)target->frameBuf.depthBuf;
        GX_MemoryFill(c, 0x000000FF, c + game_h * target_w, GX_FILL_TRIGGER | GX_FILL_32BIT_DEPTH, d, 0,
                      d + target_w * target_h, GX_FILL_TRIGGER | GX_FILL_32BIT_DEPTH);
    } else {
        C3D_RenderTargetClear(target, C3D_CLEAR_ALL, 0x000000FF, 0);
        if (pillars != NULL) {
            u32 bar = (u32)(game_y0 * target_w), dim = pillars_line ? GX_BUFFER_DIM(bar * 4 / 16, 0) : 0;
            pillars_new = 0;
            GX_TextureCopy(pillars, dim, (u32*)target->frameBuf.colorBuf, dim, bar * 4, GX_TRANSFER_RAW_COPY(1));
            GX_TextureCopy(pillars + bar, dim, (u32*)target->frameBuf.colorBuf + (game_y0 + game_h) * target_w, dim,
                           bar * 4, GX_TRANSFER_RAW_COPY(1));
        }
    }
    C3D_FrameDrawOn(target);
#ifdef MVC2_PROF
    {
        static int unlit;
        if (gpu_probe_nolight() != unlit) {
            unlit = !unlit;
            C3D_LightEnvBind(unlit ? NULL : &env);
        }
    }
#endif
    PROF_STOP(PZ_DRAW_BEGIN, pb);
    if (splitting) {
        /* what the first part reads, flushed or queued before it */
        tex_flush();
        if (nstaged != 0) {
            staging_send();
        }
#ifdef MVC2_PROF
        gpu_split_take();
#endif
    }
    watch_phase = "drawing";
    draw();
    PFZ_STOP(PFZ_DRAWS, qd);
    t4 = svcGetSystemTick();
    PFZ_START(qe);
    if (!pipe_on) {
        flush_console();
    }
    watch_phase = "ending the frame";
    frame_words = (u32)(gpuCmdBuf + gpuCmdBufOffset - frame_cmd);  /* C3D_FrameEnd clears gpuCmdBuf */
#ifdef MVC2_PROF
    list_done = 0;
    gpu_probe_frame(frame_cmd, frame_words);
    if (!splitting) {
        gpu_split_take();
    }
#endif
    if (cmd_self) {
        flush_range(frame_cmd, frame_words * 4, PZ_CMD_FLUSH);
    }
#ifdef MVC2_PROF
    reg_check_frame(frame_cmd, frame_words);
#endif
    if (nstaged != 0) {
        staging_send();
    }
    {
        PROF_START(pe);
        if (!list_running) {
            PROF_ADD(PZ_GPU_IDLE, pe - gpu_free_at);
        }
        C3D_FrameEnd(GX_CMDLIST_FLUSH);
        PROF_STOP(PZ_FRAME_END, pe);
        PROF_COUNT(PZ_FRAME_END, frame_words);
    }
    PFZ_STOP(PFZ_END, qe);
#ifdef MVC2_PROF
    list_sent = prof_now();
    if (!list_running) {
        gpu_split_sent();
    }
#endif
    if (nsplits != 0) {
        split_log();
    }
    watch_beat++;
    watch_phase = pipe_on ? "waiting for the game's next frame" : "in the game";
    ctr_render_ticks[CTR_TICKS_SYNC] += t2 - t1;
    ctr_render_ticks[CTR_TICKS_LISTS] += pipe_on ? lists : early || lists1 ? lists + t3 - t2 : t3 - t2;
    ctr_render_ticks[CTR_TICKS_DRAW] += t4 - t3;
    ctr_render_ticks[CTR_TICKS_END] += svcGetSystemTick() - t4;
    PROF_ADD(PZ_LISTS, early || lists1 ? lists + t3 - t2 : lists);
    PROF_ADD(PZ_DRAWS, svcGetSystemTick() - t3);
}

/* the last frame as 640x480 ARGB */
static int read_target(uint32_t* frame) {
    enum { ROWS = 40 };     /* the target's rows a copy, a multiple of the 8-row tiles */
    uint32_t* lin = linearAlloc((size_t)target_w * ROWS * 4);
    int x, y, r0;
    if (lin == NULL) {
        return 0;
    }
    for (r0 = game_y0; r0 < game_y0 + game_h; r0 += ROWS) {
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C3D_SyncDisplayTransfer((u32*)target->frameBuf.colorBuf + r0 * target_w, GX_BUFFER_DIM(target_w, ROWS), lin,
                                GX_BUFFER_DIM(target_w, ROWS),
                                GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) |
                                    GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) |
                                    GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO));
        C3D_FrameEnd(0);
        GSPGPU_InvalidateDataCache(lin, (u32)target_w * ROWS * 4);
        for (x = 0; x < RD_WIDTH; x++) {
            int row = game_y0 + x * game_h / RD_WIDTH - r0;
            if (row < 0 || row >= ROWS) {
                continue;
            }
            for (y = 0; y < RD_HEIGHT; y++) {
                uint32_t p = lin[row * target_w + (target_w - 1 - y * target_w / RD_HEIGHT)];     /* RGBA */
                frame[y * RD_WIDTH + x] = 0xFF000000u | p >> 8;
            }
        }
    }
    linearFree(lin);
    return 1;
}

/* the last frame as the top screen has it, 400x240 ARGB (800x240 wide): the display transfer to the screen made
   again */
static int read_screen(uint32_t* px) {
    int w = ctr_render_top_w(), x, y;
    uint8_t* lin = linearAlloc((size_t)240 * w * 3);
    if (lin == NULL) {
        return 0;
    }
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    /* the output's size given as the input's, as citro3d gives it: the transfer halves it */
    C3D_SyncDisplayTransfer((u32*)target->frameBuf.colorBuf, GX_BUFFER_DIM(target_w, target_h), (u32*)lin,
                            GX_BUFFER_DIM(target_w, target_h), output_flags);
    C3D_FrameEnd(0);
    GSPGPU_InvalidateDataCache(lin, (u32)(240 * w * 3));
    /* a column of the screen a row of 240, from its bottom, B G R */
    for (x = 0; x < w; x++) {
        for (y = 0; y < 240; y++) {
            const uint8_t* p = lin + (x * 240 + 239 - y) * 3;
            px[y * w + x] = 0xFF000000u | (u32)p[2] << 16 | (u32)p[1] << 8 | p[0];
        }
    }
    linearFree(lin);
    return 1;
}

/* The drawing thread, on the New 3DS's core 2: it alone uses citro3d after the start. The game's thread waits
   for it before each handoff, so that a frame's memory has one owner at a time. */
enum { WORK_NONE, WORK_FRAME, WORK_READ, WORK_QUIT };
static Thread pipe_thread;
static LightLock pipe_lock;
static CondVar pipe_go, pipe_done;  /* one each, as lists1_go and lists1_done: the drawing thread waits on go */
static int pipe_work, read_ok;
static RFRAME* pipe_rf;
static uint32_t* read_to;
static int read_whole;              /* read_screen's, not read_target's */
static aptHookCookie pipe_hook;
static const char* mode = "serial";

static void pipe_main(void* arg) {
    uint32_t fpscr = (uint32_t)(uintptr_t)arg;
    __asm__ volatile("vmsr fpscr, %0" : : "r"(fpscr));      /* the game's thread's rounding */
    for (;;) {
        int work;
        PROF_START(p0);
        LightLock_Lock(&pipe_lock);
        while (pipe_work == WORK_NONE) {
            CondVar_Wait(&pipe_go, &pipe_lock);
        }
        work = pipe_work;
        LightLock_Unlock(&pipe_lock);
        PROF_STOP(PZ_RENDER_IDLE, p0);
        if (work == WORK_QUIT) {
            break;
        }
        if (work == WORK_FRAME) {
            rf = pipe_rf;
            fr = rf->frame;
            render();
        } else {
            read_ok = read_whole ? read_screen(read_to) : read_target(read_to);
        }
        LightLock_Lock(&pipe_lock);
        pipe_work = WORK_NONE;
        CondVar_Broadcast(&pipe_done);
        LightLock_Unlock(&pipe_lock);
    }
}

static void pipe_idle(void) {
    LightLock_Lock(&pipe_lock);
    while (pipe_work != WORK_NONE) {
        CondVar_Wait(&pipe_done, &pipe_lock);
    }
    LightLock_Unlock(&pipe_lock);
}

static void pipe_wait(void) {
    PROF_START(p0);
    pipe_idle();
    PROF_STOP(PZ_PIPE_WAIT, p0);
}

static void pipe_post(int work) {
    LightLock_Lock(&pipe_lock);
    pipe_work = work;
    CondVar_Broadcast(&pipe_go);
    LightLock_Unlock(&pipe_lock);
}

/* before citro3d's own hook, which uses its state when the HOME Menu comes and goes */
static void pipe_apt(APT_HookType type, void* param) {
    (void)type;
    (void)param;
    pipe_idle();
}

static void pipe_free(const char* why) {
    if (vbuf[1] != NULL) {
        linearFree(vbuf[1]);
    }
    free(retired);
    free(rd_frames[1]);
    free(rframes[1]);
    vbuf[1] = NULL;
    retired = NULL;
    rd_frames[1] = NULL;
    rframes[1] = NULL;
    mode = why;
}

/* before the ROM cache takes the heap: a second frame, so that the drawing can run beside the game */
int ctr_render_reserve(void) {
    rd_frames[0] = rd_frame;
    rd_frames[1] = calloc(1, sizeof(RD_FRAME));     /* its lists' sizes read by rd_begin_frame */
    rframes[1] = malloc(sizeof(RFRAME));
    if (rd_frames[1] == NULL || rframes[1] == NULL) {
        pipe_free("serial: no memory for a second frame");
        return 0;
    }
    memset(rframes[1]->made, 0, sizeof rframes[1]->made);
    rframes[1]->palette_serial = rd_palette_serial - 1;
    return 1;
}

static void pipe_start(void) {
    s32 prio = 0x30;
    uint32_t fpscr;
    vbuf[1] = linearAlloc((sizeof(GV) + sizeof *L->idx) * MAX_VERTS);
    retired = malloc(sizeof *retired * MAX_TEX);
    if (vbuf[1] == NULL || retired == NULL) {
        pipe_free("serial: no memory for a second frame");
        return;
    }
    LightLock_Init(&pipe_lock);
    CondVar_Init(&pipe_go);
    CondVar_Init(&pipe_done);
    svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);
    __asm__ volatile("vmrs %0, fpscr" : "=r"(fpscr));
    pipe_on = 1;
    pipe_thread = threadCreate(pipe_main, (void*)(uintptr_t)fpscr, 64 << 10, prio - 1, 2, false);
    if (pipe_thread == NULL) {
        pipe_on = 0;
        pipe_free("serial: no thread on core 2");
        return;
    }
    aptHook(&pipe_hook, pipe_apt, NULL);
    mode = "pipelined: the lists and the GPU's commands on core 2, a frame behind the game";
}

/* MVC2_LISTS_CORE1=1, for the Old 3DS: a frame's lists built on core 1, beside the sound, while the game makes
   the next. Core 1 takes the stages in turn; when the game's frame is done, core 0 waits for the stage under way
   and builds the rest, then draws, a frame behind the game. Core 1 flushes the vertices it writes.
   Each side waits on its own CondVar: libctru's wakes only threads already in the kernel, and a second waiter
   that begins before the first gets there puts it back to sleep */
static Thread lists1_thread;
static LightLock lists1_lock;
static CondVar lists1_go, lists1_done;  /* core 1 waits on go, core 0 on done */
static int lists1_next = STAGES;    /* the next stage of the frame's lists */
static int lists1_busy, lists1_stop, lists1_quit, lists1_have;
static int lists1_stages = STAGES;  /* MVC2_LISTS1_STAGES=n: core 1 begins only the first n */
static int lists1_limit = STAGES;   /* this frame's: lists1_stages, or 0 while the sound is heavy */
/* MVC2_LISTS_SOUNDY=1 (or hi,lo,up,down): while the sound's own time on core 1 is long (the title's and the ranking
   screens' music, 8-10 ms a frame, against 3-4 in a fight), core 1 is left to it and core 0 builds the lists: after
   up frames at hi ms or more, smoothed (7, 30), and back after down frames under lo (5.5, 30) */
static float soundy_hi, soundy_lo, soundy_own;
static int soundy_up, soundy_down;
static int soundy_heavy, soundy_run, soundy_logged;
/* MVC2_LISTS1_YIELD=ms: a stage not begun on core 1 while the sound's frame is being made, when the sound would then
   end more than ms after it was handed over (its own time, the stages over it so far and this one's); the rest
   left to core 0, or to core 1 again when the sound's frame is done */
static uint32_t lists1_yield;
static int lists1_held, lists1_held_frame;
static float lists1_est[STAGES];    /* each stage's ticks on core 1, smoothed */
static volatile uint32_t lists1_seq, lists1_busy_ticks, lists1_stage_t0;   /* the stages' ticks, odd seq while changed */
static uint64_t lists1_cutoff;      /* MVC2_LISTS1_CUTOFF=ms: none begun on core 1 that long after the frame was handed
                                       over, the rest left to core 0 (0: no limit) */
static uint64_t lists1_given;       /* when it was */
static uint64_t lists1_begun;       /* when core 1 began the stage under way */
static RFRAME* lists1_rf;
static int lists1_join;             /* MVC2_FLUSH_JOIN=1: core 1 goes on to its next stage without letting go, and
                                       flushes what its stages wrote once, when it stops (else after each stage) */

uint32_t ctr_lists1_busy(void) {
    uint32_t q, b, t0;
    do {
        q = lists1_seq;
        __sync_synchronize();
        b = lists1_busy_ticks;
        t0 = lists1_stage_t0;
        __sync_synchronize();
    } while ((q & 1) || q != lists1_seq);
    return t0 != 0 ? b + ((uint32_t)svcGetSystemTick() - t0) : b;
}

static void busy_mark(uint32_t t0, uint32_t add) {
    lists1_seq++;
    __sync_synchronize();
    lists1_busy_ticks += add;
    lists1_stage_t0 = t0;
    __sync_synchronize();
    lists1_seq++;
}

/* MVC2_LISTS1_YIELD: stage held back for the sound; under lists1_lock */
static int lists1_hold(int stage) {
    uint32_t at_go, need;
    if (lists1_yield == 0 || stage >= lists1_limit || !ctr_audio_making(&at_go, &need)) {
        return 0;
    }
    lists1_held = need + (lists1_busy_ticks - at_go) + (uint32_t)lists1_est[stage] > lists1_yield;
    lists1_held_frame |= lists1_held;
    return lists1_held;
}

void ctr_lists1_sound_done(void) {
    if (lists1_held) {
        LightLock_Lock(&lists1_lock);
        lists1_held = 0;
        CondVar_Broadcast(&lists1_go);
        LightLock_Unlock(&lists1_lock);
    }
}

static void lists1_main(void* arg) {
    uint32_t fpscr = (uint32_t)(uintptr_t)arg;
    __asm__ volatile("vmsr fpscr, %0" : : "r"(fpscr));      /* the game's thread's rounding */
    for (;;) {
        int stage, v0, i0;
        LightLock_Lock(&lists1_lock);
        while (!lists1_quit && (lists1_stop || lists1_next >= lists1_limit ||
                                (lists1_cutoff != 0 && svcGetSystemTick() - lists1_given > lists1_cutoff) ||
                                lists1_hold(lists1_next))) {
            CondVar_Wait(&lists1_go, &lists1_lock);
        }
        if (lists1_quit) {
            LightLock_Unlock(&lists1_lock);
            break;
        }
        stage = lists1_next;
        lists1_busy = 1;
        lists1_begun = svcGetSystemTick();
        LightLock_Unlock(&lists1_lock);
#ifdef MVC2_PROF
        if (perf_zones && pfz_set1 != pfz_set) {
            pfz_events(pfz_set);    /* the counters are each core's own */
            pfz_set1 = pfz_set;
        }
#endif
        v0 = stage == STAGE_SLICES ? 0 : L->nverts;
        i0 = stage == STAGE_SLICES ? 0 : L->nidx;
        for (;;) {
            int go_on = 0;
            uint32_t s0 = (uint32_t)svcGetSystemTick() | 1, d;
            PROF_START(p0);
            busy_mark(s0, 0);
            build_stage(stage);
            d = (uint32_t)svcGetSystemTick() - s0;
            busy_mark(0, d);
            lists1_est[stage] += ((float)d - lists1_est[stage]) * 0.25f;
            if (lists1_join) {
                LightLock_Lock(&lists1_lock);
                go_on = !lists1_quit && !lists1_stop && stage + 1 < lists1_limit &&
                        (lists1_cutoff == 0 || svcGetSystemTick() - lists1_given <= lists1_cutoff) && !lists1_hold(stage + 1);
                if (go_on) {
                    lists1_next = stage + 1;
                    lists1_begun = svcGetSystemTick();
                }
                LightLock_Unlock(&lists1_lock);
            }
            if (!go_on) {
                if (L->nverts > v0) {
                    flush_one(&L->verts[v0], (u32)(L->nverts - v0) * sizeof(GV));
                }
                if (indexed && L->nidx > i0) {
                    flush_one(&L->idx[i0], (u32)(L->nidx - i0) * sizeof *L->idx);
                }
            }
            PROF_STOP(PZ_LISTS1, p0);
            PROF_STOP(PZ_LISTS1_STAGE + stage, p0);
            if (!go_on) {
                break;
            }
            stage++;
        }
        LightLock_Lock(&lists1_lock);
        lists1_next = stage + 1;
        lists1_busy = 0;
        CondVar_Broadcast(&lists1_done);
        LightLock_Unlock(&lists1_lock);
    }
}

static double lists1_ms(uint64_t since) {
    return (double)(svcGetSystemTick() - since) * 1000.0 / SYSCLOCK_ARM11;
}

/* the stages core 1 hasn't begun, here */
static void lists1_finish(void) {
    uint64_t t0 = svcGetSystemTick();
    int stage;
#ifdef MVC2_PROF
    if (perf_zones) {
        pfz_frame();
    }
#endif
    {
        PROF_START(p0);
        LightLock_Lock(&lists1_lock);
        lists1_stop = 1;
        while (lists1_busy) {
            if (CondVar_WaitTimeout(&lists1_done, &lists1_lock, 1000000000LL) && lists1_busy) {
                host_log("lists: frame %lu, core 0 has waited %.0f ms for core 1's stage %d, begun %.0f ms ago, the "
                         "frame handed over %.0f ms ago", (unsigned long)frame_no, lists1_ms(t0), lists1_next,
                         lists1_ms(lists1_begun), lists1_ms(lists1_given));
            }
        }
        stage = lists1_next;
        ctr_render_count[CTR_COUNT_HELD] += (uint32_t)lists1_held_frame;
        lists1_held_frame = 0;
        LightLock_Unlock(&lists1_lock);
        PROF_STOP(PZ_LISTS1_WAIT, p0);
    }
    ctr_render_count[CTR_COUNT_STAGES1] += (uint32_t)stage;
    for (; stage < STAGES; stage++) {
        build_stage(stage);
    }
    lists1_ticks = svcGetSystemTick() - t0;
}

/* MVC2_LISTS_SOUNDY's count of the frames the sound takes long or not */
static void soundy_check(void) {
    int many;
    soundy_own += (ctr_audio_own_ms() - soundy_own) * 0.125f;
    many = soundy_heavy ? soundy_own >= soundy_lo : soundy_own >= soundy_hi;
    soundy_run = many != soundy_heavy ? soundy_run + 1 : 0;
    if (soundy_run < (soundy_heavy ? soundy_down : soundy_up)) {
        return;
    }
    soundy_heavy = !soundy_heavy;
    soundy_run = 0;
    if (soundy_logged < 40) {
        soundy_logged++;
        host_log("lists: frame %lu, the sound %.1f ms a frame of its own: %s", (unsigned long)frame_no, soundy_own,
                 soundy_heavy ? "on core 0 while the sound is heavy" : "back on core 1");
    }
}

/* the frame drawn whose lists are ready, then the game's new one handed over and begun on core 1 */
static void lists1_frame(void) {
    RD_FRAME* next;
    if (lists1_have) {
        lists1_finish();
        render();
        stats_done();
        if (dump_wanted()) {
            dump();
        }
    }
    stretch_take();
    lists1_rf = rframes[lists1_rf == rframes[0]];
    take_frame(lists1_rf, rd_frame);
    rf = lists1_rf;
    fr = rd_frame;
    next = rd_frame == rd_frames[0] ? rd_frames[1] : rd_frames[0];
    memcpy(next->background, rd_frame->background, sizeof next->background);    /* it lasts from frame to frame */
    rd_frame = next;
    early_place();
    if (soundy_up != 0) {
        soundy_check();
    }
    LightLock_Lock(&lists1_lock);
    lists1_next = 0;
    lists1_stop = 0;
    lists1_limit = soundy_heavy ? 0 : lists1_stages;
    lists1_given = svcGetSystemTick();
    CondVar_Broadcast(&lists1_go);
    LightLock_Unlock(&lists1_lock);
    lists1_have = 1;
}

static void lists1_start(void) {
    s32 prio = 0x30;
    uint32_t fpscr;
    char cut[48] = "";
    /* MVC2_LISTS1_ABOVE=1: core 1's stages before the sound's frame, which waits for them; the first 2 unless
       MVC2_LISTS1_STAGES or MVC2_LISTS1_CUTOFF bounds them */
    int above = getenv("MVC2_LISTS1_ABOVE") != NULL && atoi(getenv("MVC2_LISTS1_ABOVE")) != 0;
    retired = malloc(sizeof *retired * MAX_TEX);
    if (retired == NULL) {
        lists1 = 0;
        pipe_free("serial: no memory for the lists on core 1");
        return;
    }
    if (getenv("MVC2_LISTS1_STAGES") != NULL) {
        lists1_stages = atoi(getenv("MVC2_LISTS1_STAGES"));
        lists1_stages = lists1_stages < 0 ? 0 : lists1_stages > STAGES ? STAGES : lists1_stages;
    }
    if (getenv("MVC2_LISTS1_CUTOFF") != NULL && atof(getenv("MVC2_LISTS1_CUTOFF")) > 0) {
        lists1_cutoff = (uint64_t)(atof(getenv("MVC2_LISTS1_CUTOFF")) * SYSCLOCK_ARM11 / 1000);
        snprintf(cut, sizeof cut, ", none begun %s ms after the handoff", getenv("MVC2_LISTS1_CUTOFF"));
    }
    if (above && getenv("MVC2_LISTS1_STAGES") == NULL && lists1_cutoff == 0) {
        lists1_stages = 2;
    }
    lists1_join = getenv("MVC2_FLUSH_JOIN") != NULL && atoi(getenv("MVC2_FLUSH_JOIN")) != 0;
    lists1_limit = lists1_stages;
    if (getenv("MVC2_LISTS_SOUNDY") != NULL && strcmp(getenv("MVC2_LISTS_SOUNDY"), "0") != 0) {
        soundy_hi = 7.0f, soundy_lo = 5.5f, soundy_up = 30, soundy_down = 30;
        if (strchr(getenv("MVC2_LISTS_SOUNDY"), ',') != NULL) {
            sscanf(getenv("MVC2_LISTS_SOUNDY"), "%f,%f,%d,%d", &soundy_hi, &soundy_lo, &soundy_up, &soundy_down);
        }
        soundy_up = soundy_up < 1 ? 1 : soundy_up;
        host_log("lists on core 1: left to the sound after %d frames at %.1f ms or more of its own, smoothed, back after "
                 "%d under %.1f (MVC2_LISTS_SOUNDY)", soundy_up, soundy_hi, soundy_down, soundy_lo);
    }
    if (getenv("MVC2_LISTS1_YIELD") != NULL && atof(getenv("MVC2_LISTS1_YIELD")) > 0) {
        lists1_yield = (uint32_t)(atof(getenv("MVC2_LISTS1_YIELD")) * SYSCLOCK_ARM11 / 1000);
        host_log("lists on core 1: a stage not begun while the sound's frame is being made if it would then end %s ms "
                 "or more after it was handed over (MVC2_LISTS1_YIELD)", getenv("MVC2_LISTS1_YIELD"));
    }
    LightLock_Init(&lists1_lock);
    CondVar_Init(&lists1_go);
    CondVar_Init(&lists1_done);
    svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);
    __asm__ volatile("vmrs %0, fpscr" : "=r"(fpscr));
    use_buffer(vbuf[0]);
    /* the sound's thread is at prio - 1 */
    lists1_thread = threadCreate(lists1_main, (void*)(uintptr_t)fpscr, 64 << 10,
                                 above ? prio - 2 : prio < 0x3F ? prio + 1 : prio, 1, false);
#ifdef MVC2_PROF
    pfz_thread1 = lists1_thread;
#endif
    if (lists1_thread == NULL) {
        lists1 = 0;
        free(retired);
        retired = NULL;
        pipe_free("serial: no thread on core 1");
        return;
    }
    mode = "split: the lists on core 1 beside the game's next frame, as many stages as it finishes, the drawing a frame "
           "behind the game";
    host_log("lists on core 1: the first %d of %d stages at most, %s the sound's priority%s", lists1_stages, STAGES,
             above ? "above" : "below", cut);
    if (lists1_join) {
        host_log("lists on core 1: their vertices flushed once, when core 1 stops (MVC2_FLUSH_JOIN)");
    }
}

const char* ctr_render_mode(void) {
    return mode;
}

void ctr_render_frame(void) {
    RD_FRAME* next;
    if (lists1) {
        lists1_frame();
        return;
    }
    if (!pipe_on) {
        stretch_take();
        render();
        stats_done();
        if (dump_wanted()) {
            dump();
        }
        return;
    }
    pipe_wait();
    stats_done();
    stretch_take();
    pipe_rf = rframes[pipe_rf == rframes[0]];
    take_frame(pipe_rf, rd_frame);
    flush_console();
    next = rd_frame == rd_frames[0] ? rd_frames[1] : rd_frames[0];
    memcpy(next->background, rd_frame->background, sizeof next->background);    /* it lasts from frame to frame */
    rd_frame = next;
    pipe_post(WORK_FRAME);
    if (dump_wanted()) {
        pipe_wait();
        dump();
    }
}

/* the last frame as 640x480 ARGB, or with screen set as the top screen's 400x240, for test runs */
int ctr_render_read(uint32_t* frame, int screen) {
    if (!pipe_on) {
        return screen ? read_screen(frame) : read_target(frame);
    }
    pipe_wait();
    read_to = frame;
    read_whole = screen;
    pipe_post(WORK_READ);
    pipe_wait();
    return read_ok;
}

/* host_restart: before (0), the frame in flight drawn and the GPU done with it, the textures and meshes made for the
   program going dropped (their keys and ids are in its memory), and the render layer's settings taken; after (1),
   those given back */
void ctr_render_restart(int after) {
    static struct {
        int tl, kinds, max, trans, two_pass, nearclip, seen, group_info;
        float trans_deep;
        uint32_t deep_left, two_passed, near_left, palette_serial;
    } k;
    uint32_t fence_was;
    int i, next;
    if (after) {
        rd_gpu_tl = k.tl;
        rd_gpu_tl_kinds = k.kinds;
        rd_gpu_tl_max = k.max;
        rd_gpu_tl_trans = k.trans;
        rd_gpu_tl_two_pass = k.two_pass;
        rd_gpu_tl_nearclip = k.nearclip;
        rd_seen = k.seen;
        rd_group_info = k.group_info;
        rd_gpu_tl_trans_deep = k.trans_deep;
        rd_gpu_tl_deep_left = k.deep_left;
        rd_gpu_tl_two_passed = k.two_passed;
        rd_gpu_tl_near_left = k.near_left;
        rd_palette_serial = k.palette_serial + 1;   /* each frame's palette copied again */
        return;
    }
    if (lists1 && lists1_have) {
        lists1_finish();
        render();
        stats_done();
        lists1_have = 0;
    }
    if (pipe_on) {
        pipe_wait();
    }
    if (c3d_queue != NULL) {
        gxCmdQueueWait(c3d_queue, -1);
    }
    fence_was = fence;
    fence = UINT32_MAX;
    retired_free();
    for (i = lru_next(LRU_END); i != LRU_END; i = next) {
        next = lru_next(i);
        tex_drop(&gtex[i]);
    }
    for (i = 0; i < MAX_TEX; i++) {
        gtex[i].key = NULL;
        gtex[i].serial = 0;
    }
    kept_drop_all();
    memset(ahead_seen, 0, sizeof ahead_seen);
    memset(serial_slot, 0, sizeof serial_slot);
    fence = fence_was;
    if (gmesh != NULL) {
        memset(gmesh, 0, MAX_MESHES * sizeof *gmesh);
        memset(mesh_held, 0, sizeof mesh_held);
        holes[0].lo = 0;
        holes[0].hi = mesh_pool_size;
        nholes = 1;
        ngone = mesh_next = 0;
    }
    memset(corner_memo, 0, sizeof corner_memo);
    memset(corner_memo2, 0, sizeof corner_memo2);
    memset(corner_at, 0, sizeof corner_at);
    k.tl = rd_gpu_tl;
    k.kinds = rd_gpu_tl_kinds;
    k.max = rd_gpu_tl_max;
    k.trans = rd_gpu_tl_trans;
    k.two_pass = rd_gpu_tl_two_pass;
    k.nearclip = rd_gpu_tl_nearclip;
    k.seen = rd_seen;
    k.group_info = rd_group_info;
    k.trans_deep = rd_gpu_tl_trans_deep;
    k.deep_left = rd_gpu_tl_deep_left;
    k.two_passed = rd_gpu_tl_two_passed;
    k.near_left = rd_gpu_tl_near_left;
    k.palette_serial = rd_palette_serial;
}

void ctr_render_exit(void) {
    int i;
    if (lists1) {
        LightLock_Lock(&lists1_lock);
        lists1_quit = 1;
        CondVar_Broadcast(&lists1_go);
        LightLock_Unlock(&lists1_lock);
        threadJoin(lists1_thread, U64_MAX);
        threadFree(lists1_thread);
        lists1 = 0;
        rd_frame = rd_frames[0];
    }
    if (pipe_on) {
        pipe_wait();
        pipe_post(WORK_QUIT);
        threadJoin(pipe_thread, U64_MAX);
        threadFree(pipe_thread);
        aptUnhook(&pipe_hook);
        pipe_on = 0;
        rd_frame = rd_frames[0];
    }
    fence = UINT32_MAX;
    retired_free();
    for (i = 0; i < MAX_TEX; i++) {
        if (gtex[i].ok) {
            C3D_TexDelete(&gtex[i].tex);
            gtex[i].ok = 0;
        }
    }
    kept_drop_all();
    lru_clear();
    if (mesh_pool != NULL) {
        linearFree(mesh_pool);
        mesh_pool = NULL;
    }
    free(gmesh);
    gmesh = NULL;
    memset(mesh_held, 0, sizeof mesh_held);
    ngone = nholes = 0;
    rd_gpu_tl = 0;
    linearFree(vbuf[0]);
    if (apt_hooked) {
        aptUnhook(&apt_cookie);
        apt_hooked = 0;
    }
    pillars_once = 0;
    if (tex_vram) {
        vram_log(1);
    }
    if (staging_buf != NULL) {
        linearFree(staging_buf);
        staging_buf = NULL;
    }
    tex_vram = vram_moves = vram_direct = vram_noflush = tex_unpark = 0;
    nstaged = 0;
    if (pillars_made != NULL) {
        linearFree(pillars_made);
        pillars_made = pillars = NULL;
    }
    pipe_free("serial");
    shaderProgramFree(&prog);
    DVLB_Free(dvlb);
    C3D_RenderTargetDelete(target);
    C3D_Fini();
}

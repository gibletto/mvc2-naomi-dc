/*
 * ctr_render.c  The frame's lists drawn by the PICA200 with citro3d, as rd_soft draws them
 *
 * Depth: the frame's 1/w range is cut into slices, each with its own part of the depth buffer, since
 * float24 can't hold sprites one bit apart. Palettes: fragment lighting looks the colour up, the texel
 * read as a normal and the palette in the RR, RG, RB and FR tables. The offset colour is a constant
 * per triangle.
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <3ds.h>
#include <citro3d.h>

#include "rd.h"
#include "rd_list.h"
#include "ctr_render.h"
#include "ctr_render_shbin.h"
#include "prof.h"

void host_log(const char* fmt, ...);

#define VIEW_W 800                  /* the top screen at twice its size */
#define VIEW_H 480
#define GAME_X 80
#define MAX_VERTS 65536
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
    int first, count;               /* vertices */
} BATCH;

typedef struct {
    const RD_TEXTURE* key;
    uint32_t serial;
    int width, height;              /* powers of 2 */
    int levels;                     /* past the first, down to 8x8 */
    GPU_TEXCOLOR format;
    uint32_t used;                  /* the frame last drawn in */
    int ok;
    C3D_Tex tex;
} GTEX;

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

static C3D_RenderTarget* target;
static int target_w, target_h;      /* turned: 240x400, or 480x800 at full size */
static int game_y0, game_h;         /* the game's columns on it */
static DVLB_s* dvlb;
static shaderProgram_s prog;
static int u_proj;
static C3D_Mtx proj;
static C3D_LightEnv env;
static C3D_Light light;
static C3D_LightLut env_lut;

static GV* verts;                   /* linear memory */
static int nverts;
static BATCH batches[MAX_BATCHES];
static int nbatches;
static GTEX gtex[MAX_TEX];
static uint32_t frame_no;

/* the depth slices: slice k holds 1/w from slice_lo[k] to slice_lo[k + 1] */
#define MAX_KEYS (RD_LISTS * RD_MAX_TRIS + 2)
static uint32_t zkeys[MAX_KEYS], ztmp[MAX_KEYS];
static uint32_t zends[2][MAX_KEYS];   /* the triangles' nearest and farthest depths */
static int nends;
static double slice_lo[MAX_SLICES + 1];
static RD_TRI bg_tri[2];            /* the background, when it sets the depth */
static RD_VTX bg_vtx[4];            /* top left, top right, bottom left, bottom right */
static int nbg, bg_state;
static double slice_scale[MAX_SLICES], slice_base[MAX_SLICES];     /* depth in the slice = z * scale + base */
#define MAX_PARTS 4096
#define MAX_ITEMS (RD_MAX_TRIS + MAX_PARTS)
static uint8_t tri_k0[MAX_ITEMS], tri_k1[MAX_ITEMS];    /* the slices a triangle reaches (k0 > k1: culled) */
static int by_slice[MAX_ITEMS * 2];     /* slice k's from slice_at[k] to slice_at[k + 1] */
static int slice_at[MAX_SLICES + 1];

/* each state's texture, and the scale to the GPU's texture coordinates */
static struct {
    uint32_t frame;
    GTEX* g;
    float su, sv;
} state_tex[RD_MAX_STATES];   /* doubles: a cut can fall between two neighbouring floats */
static int nslices;

/* the translucent list as drawn, far first */
static const RD_TRI* trans_item[MAX_ITEMS];
static int ntrans, nparts;
static RD_TRI trans_part[MAX_PARTS];
static float trans_lo[MAX_ITEMS], trans_hi[MAX_ITEMS];
static uint32_t trans_key[MAX_ITEMS], trans_key2[MAX_ITEMS];
static int trans_order[MAX_ITEMS], trans_tmp[MAX_ITEMS], trans_tmp2[MAX_ITEMS];
static uint32_t sprite_z[MAX_ITEMS];         /* the shallow ones' depths, sorted */

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
    int lut_base, lut_n;
    uint32_t lut_serial;
} cur;

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

/* into the GPU's tiling: 8x8 tiles in rows, Morton order inside, the first row at v = 1 */
static void convert_level(const RD_TEXTURE* t, const GTEX* g, const void* texels, int w, int h, void* data, int gw) {
    int x, y, tiles = gw / 8;
    /* in a tile's row, texels x and x + 1 (x even) lie side by side: at 0, 4, 16 and 20 past the row's start */
    static const uint8_t pair[4] = { 0, 4, 16, 20 };
    if (g->format == GPU_L8) {
        uint8_t* dst = data;
        const uint8_t* src = texels;
        for (y = 0; y < h; y++) {
            uint8_t* row = dst + (y >> 3) * tiles * 64 + morton[(y & 7) * 8];
            const uint8_t* s = src + y * w;
            for (x = 0; x + 8 <= w; x += 8, s += 8) {
                uint8_t* d = row + x * 8;
                int k;
                for (k = 0; k < 4; k++) {
                    *(uint16_t*)(d + pair[k]) = (uint16_t)(s[2 * k] | s[2 * k + 1] << 8);
                }
            }
            for (; x < w; x++) {
                row[(x >> 3) * 64 + morton[x & 7]] = src[y * w + x];
            }
        }
    } else {
        uint16_t* dst = data;
        const uint16_t* src = texels;
        /* RGBA4 and RGBA5551: the alpha moved to the bottom bits */
        int rot = t->format == RD_ARGB4444 ? 4 : t->format == RD_RGB565 ? 0 : 1;
        uint32_t low = rot == 4 ? 0x000F000Fu : 0x00010001u;     /* each half's bottom rot bits */
        for (y = 0; y < h; y++) {
            uint16_t* row = dst + (y >> 3) * tiles * 64 + morton[(y & 7) * 8];
            const uint16_t* s = src + y * w;
            for (x = 0; x + 8 <= w; x += 8, s += 8) {
                uint16_t* d = row + x * 8;
                int k;
                for (k = 0; k < 4; k++) {
                    uint32_t a = s[2 * k], b = s[2 * k + 1], v = a | b << 16;
                    if (rot != 0) {
                        v = (v << rot & ~low) | (v >> (16 - rot) & low);    /* both halves rotated */
                    }
                    *(uint32_t*)(d + pair[k]) = v;
                }
            }
            for (; x < w; x++) {
                uint32_t p = src[y * w + x];
                p = rot != 0 ? (p << rot | p >> (16 - rot)) & 0xFFFF : p;
                row[(x >> 3) * 64 + morton[x & 7]] = (uint16_t)p;
            }
        }
    }
}

static void convert(const RD_TEXTURE* t, GTEX* g) {
    int k;
    convert_level(t, g, t->texels, t->width, t->height, g->tex.data, g->width);
    for (k = 1; k <= g->levels; k++) {
        convert_level(t, g, t->mip[k], t->width >> k, t->height >> k, C3D_Tex2DGetImagePtr(&g->tex, k, NULL),
                      g->width >> k);
    }
    C3D_TexFlush(&g->tex);
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

/* frees the least recently drawn; 0 when none */
static int free_oldest(void) {
    int i, oldest = -1;
    for (i = 0; i < MAX_TEX; i++) {
        if (gtex[i].ok && gtex[i].used != frame_no && (oldest < 0 || gtex[i].used < gtex[oldest].used)) {
            oldest = i;
        }
    }
    if (oldest < 0) {
        return 0;
    }
    C3D_TexDelete(&gtex[oldest].tex);
    gtex[oldest].ok = 0;
    ctr_render_count[CTR_COUNT_FREED]++;
    return 1;
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

/* converted again if its texels changed; NULL if it can't be */
static GTEX* gpu_tex(const RD_TEXTURE* t) {
    GTEX* g = find_tex(t);
    int w, h, levels;
    GPU_TEXCOLOR f;
    if (g == NULL || t->texels == NULL || t->width <= 0 || t->height <= 0 || t->width > 1024 || t->height > 1024) {
        return NULL;
    }
    w = pow2(t->width);
    h = pow2(t->height);
    f = gpu_format(t->format);
    levels = gpu_levels(t, w, h);
    if (g->key == t && g->ok && g->serial == t->serial && g->width == w && g->height == h && g->format == f &&
        g->levels == levels) {
        g->used = frame_no;
        return g;
    }
    if (g->ok && (g->key != t || g->width != w || g->height != h || g->format != f || g->levels != levels)) {
        C3D_TexDelete(&g->tex);
        g->ok = 0;
    }
    g->key = t;
    if (!g->ok) {
        /* all of the linear heap but LINEAR_KEEP */
        while (linearSpaceFree() < (u32)(w * h * 2 * (levels > 0 ? 4 : 3) / 3) + LINEAR_KEEP && free_oldest()) {
        }
        while (!C3D_TexInitWithParams(&g->tex, NULL, (C3D_TexInitParams){ (u16)w, (u16)h, (u8)levels, f, GPU_TEX_2D, false })) {
            if (!free_oldest()) {
                return NULL;
            }
        }
        g->ok = 1;
        g->width = w;
        g->height = h;
        g->levels = levels;
        g->format = f;
        C3D_TexSetWrap(&g->tex, GPU_REPEAT, GPU_REPEAT);
        C3D_TexSetFilterMipmap(&g->tex, GPU_NEAREST);     /* as the PowerVR2 */
        if (w != t->width || h != t->height) {
            memset(g->tex.data, 0, g->tex.size);
        }
    }
    {
        uint64_t t0 = svcGetSystemTick(), dt;
        convert(t, g);
        dt = svcGetSystemTick() - t0;
        ctr_render_ticks[CTR_TICKS_TEX] += dt;
        PROF_ADD(PZ_TEXTURES, dt);
        ctr_render_count[CTR_COUNT_MADE]++;
        ctr_render_count[CTR_COUNT_TEXELS] += (uint32_t)(t->width * t->height);
    }
    g->serial = t->serial;
    g->used = frame_no;
    return g;
}

/* palettes in the lighting's lookup tables */

static uint32_t lut12(uint32_t v) {
    return v << 4 | v >> 4;     /* 0..255 to 0..4095 */
}

/* texel c is entry base + (c ^ 0x80) */
static void lut_palette(int base, int n) {
    static const GPU_LIGHTLUTID ids[4] = { GPU_LUT_RR, GPU_LUT_RG, GPU_LUT_RB, GPU_LUT_FR };
    uint32_t data[4][256];
    int i, k;
    for (k = 0; k < n; k++) {
        uint32_t c = rd_palette[(base + k) & 1023];
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

static void radix_sort(uint32_t* a, uint32_t* tmp, int n) {
    int shift, i;
    for (shift = 0; shift < 32; shift += 8) {
        int count[257] = { 0 };
        for (i = 0; i < n; i++) {
            count[(a[i] >> shift & 255) + 1]++;
        }
        for (i = 0; i < 256; i++) {
            count[i + 1] += count[i];
        }
        for (i = 0; i < n; i++) {
            tmp[count[a[i] >> shift & 255]++] = a[i];
        }
        memcpy(a, tmp, (size_t)n * 4);
    }
}

/* the first of the n sorted keys above z */
static int above(const uint32_t* keys, int n, double z) {
    int lo = 0, hi = n;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (zfloat(keys[mid]) <= z) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

/* the depth in a..b that the fewest triangles cross */
static double best_cut(double a, double b) {
    int ia = above(zends[0], nends, a), ib = above(zends[1], nends, a), best_n = nends + 1;
    double best = b;
    for (;;) {
        double e = 1e30, next = 1e30;
        if (ia < nends) {
            e = zfloat(zends[0][ia]);
        }
        if (ib < nends && zfloat(zends[1][ib]) < e) {
            e = zfloat(zends[1][ib]);
        }
        if (e >= b) {
            break;
        }
        while (ia < nends && zfloat(zends[0][ia]) <= e) {
            ia++;
        }
        while (ib < nends && zfloat(zends[1][ib]) <= e) {
            ib++;
        }
        if (ia < nends) {
            next = zfloat(zends[0][ia]);
        }
        if (ib < nends && zfloat(zends[1][ib]) < next) {
            next = zfloat(zends[1][ib]);
        }
        if (ia - ib < best_n) {
            best_n = ia - ib;
            best = min_d(min_d(e * (1.0 + 1.0 / 1048576), (e + next) * 0.5), b);
        }
    }
    ia = above(zends[0], nends, b);
    ib = above(zends[1], nends, b);
    return ia - ib <= best_n ? b : best;
}

/* the slices, far to near: each at most SLICE_SPAN of 1/w, closed early where sprites' depths are too close */
static void make_slices(void) {
    int list, i, n = 0, m = 0;
    nends = 0;
    float zmin = 1e30f, zmax = 0.0f;
    double lo, top, ratio;
    for (list = 0; list <= RD_LISTS; list++) {
        int ntris = list < RD_LISTS ? rd_frame.ntris[list] : nbg;
        for (i = 0; i < ntris; i++) {
            const RD_TRI* t = list < RD_LISTS ? &rd_frame.tris[list][i] : &bg_tri[i];
            int k;
            for (k = 0; k < 3; k++) {
                float z = t->v[k].z;
                if (z > 0.0f) {
                    zmin = z < zmin ? z : zmin;
                    zmax = z > zmax ? z : zmax;
                }
            }
            if (t->v[0].z > 0.0f && t->v[0].z == t->v[1].z && t->v[0].z == t->v[2].z && n < MAX_KEYS) {
                zkeys[n++] = zbits(t->v[0].z);
            } else if (nends < MAX_KEYS) {
                float a = min_f(t->v[0].z, min_f(t->v[1].z, t->v[2].z)), b = max_f(t->v[0].z, max_f(t->v[1].z, t->v[2].z));
                if (b > 0.0f) {
                    zends[0][nends] = zbits(a > 0.0f ? a : 1e-30f);
                    zends[1][nends++] = zbits(b);
                }
            }
        }
    }
    nslices = 0;
    if (zmax == 0.0f) {
        slice_lo[0] = 0.0f;
        slice_lo[1] = 1.0f;
        nslices = 1;
        return;
    }
    radix_sort(zkeys, ztmp, n);
    radix_sort(zends[0], ztmp, nends);
    radix_sort(zends[1], ztmp, nends);
    for (i = 0; i < n; i++) {
        if (m == 0 || zkeys[i] != zkeys[m - 1]) {
            zkeys[m++] = zkeys[i];
        }
    }
    lo = zmin * 0.5f;
    top = zmax * 2.0f;
    ratio = pow(top / lo, 1.0 / (MAX_SLICES / 2));
    ratio = ratio < 2.0 ? 2.0 : ratio;
    i = 0;
    while (nslices < MAX_SLICES - 1) {
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
        slice_lo[nslices++] = lo;
        lo = hi;
        if (lo >= top) {
            break;
        }
    }
    if (lo < top) {
        slice_lo[nslices++] = lo;   /* the slices ran out */
    }
    slice_lo[nslices] = top;
}

/* in slice k, with c's colour (a flat triangle's last vertex) */
static void put_vertex(const RD_VTX* s, float su, float sv, int k, const RD_VTX* c) {
    GV* v = &verts[nverts++];
    v->x = s->x + GAME_X;
    v->y = s->y;
    v->d = (float)(s->z * slice_scale[k] + slice_base[k]);
    v->w = 1.0f / s->z;
    v->u = s->u * su;
    v->v = 1.0f - s->v * sv;
    v->c = (c != NULL ? c : s)->base;
}

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
    r.base = 0;
    for (sh = 0; sh < 32; sh += 8) {
        double c = (a->base >> sh & 255) * wa + (b->base >> sh & 255) * wb + 0.5;
        r.base |= (uint32_t)(c < 0 ? 0 : c > 255 ? 255 : c) << sh;
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

/* FPU_CULL_VAL, as libnaomi sets it */
#define CULL_VAL 0.02

static int culled(const RD_STATE* s, const RD_TRI* t) {
    const RD_VTX *a = &t->v[0], *b = &t->v[1], *c = &t->v[2];
    double area = ((double)b->x - a->x) * ((double)c->y - a->y) - ((double)b->y - a->y) * ((double)c->x - a->x);
    return s->cull != 0 && (fabs(area) < CULL_VAL || (s->cull == 2 && area < 0) || (s->cull == 3 && area > 0));
}

/* past this the GPU's float24 can't place a corner: cut to GUARD pixels round the screen */
#define GUARD 2048
static int far_off(const RD_VTX* v) {
    return fabsf(v->x) > 16384.0f || fabsf(v->y) > 16384.0f;
}

/* the part of t in slice k: cut by the GPU, or here when it passes the eye */
static void add_tri(int k, const RD_TRI* t) {
    const RD_STATE* s = &rd_frame.states[t->state];
    float z0 = t->v[0].z, z1 = t->v[1].z, z2 = t->v[2].z;
    float zlo = min_f(z0, min_f(z1, z2)), zhi = max_f(z0, max_f(z1, z2));
    double lo = slice_lo[k], hi = slice_lo[k + 1];
    RD_VTX p[9], q[9];
    const RD_VTX* v = t->v;
    uint32_t off;
    BATCH* b = nbatches > 0 ? &batches[nbatches - 1] : NULL;
    GTEX* g = NULL;
    int n = 3, i;
    if (zhi < lo || zlo >= hi) {
        return;
    }
    if (far_off(&t->v[0]) || far_off(&t->v[1]) || far_off(&t->v[2])) {
        n = cut_poly(t->v, 3, p, CUT_Z, lo, 1);
        n = cut_poly(p, n, q, CUT_Z, hi, -1);
        n = cut_poly(q, n, p, CUT_X, -GUARD, 1);
        n = cut_poly(p, n, q, CUT_X, RD_WIDTH + GUARD, -1);
        n = cut_poly(q, n, p, CUT_Y, -GUARD, 1);
        n = cut_poly(p, n, q, CUT_Y, RD_HEIGHT + GUARD, -1);
        v = q;
    } else if ((zlo < lo || zhi > hi) && !(zlo > 0.0f)) {
        n = cut_poly(t->v, 3, p, CUT_Z, lo, 1);
        n = cut_poly(p, n, q, CUT_Z, hi, -1);
        v = q;
    }
    if (n < 3) {
        return;
    }
    if (nverts + (n - 2) * 3 > MAX_VERTS) {
        return;
    }
    if (state_tex[t->state].frame != frame_no) {
        g = s->tex != NULL ? gpu_tex(s->tex) : NULL;
        state_tex[t->state].frame = frame_no;
        state_tex[t->state].g = g;
        state_tex[t->state].su = g != NULL ? (float)s->tex->width / (float)g->width : 0.0f;
        state_tex[t->state].sv = g != NULL ? (float)s->tex->height / (float)g->height : 0.0f;
    }
    g = state_tex[t->state].g;
    if (s->tex != NULL && g == NULL) {
        return;
    }
    off = tri_offset(s, t);
    /* a new draw unless the last goes on with the same state */
    if (b == NULL || b->slice != k || b->offset != off || b->first + b->count != nverts ||
        (b->state != t->state && memcmp(&rd_frame.states[b->state], s, sizeof *s) != 0)) {
        if (nbatches == MAX_BATCHES) {
            return;
        }
        b = &batches[nbatches++];
        b->state = t->state;
        b->slice = (uint16_t)k;
        b->offset = off;
        b->first = nverts;
        b->count = 0;
    }
    for (i = 2; i < n; i++) {
        float su = state_tex[t->state].su, sv = state_tex[t->state].sv;
        const RD_VTX* flat = s->flat ? &t->v[2] : NULL;
        put_vertex(&v[0], su, sv, k, flat);
        put_vertex(&v[i - 1], su, sv, k, flat);
        put_vertex(&v[i], su, sv, k, flat);
        b->count += 3;
    }
}

static int slice_of(float z) {
    int lo = 0, hi = nslices - 1;
    while (lo < hi) {
        int mid = (lo + hi + 1) / 2;
        if (slice_lo[mid] <= z) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    return lo;
}

/* the background as two opaque triangles that set the depth (one at or behind the eye has none) */
static void background_tris(void) {
    static const float corner[4][2] = { { 0, 0 }, { RD_WIDTH, 0 }, { 0, RD_HEIGHT }, { RD_WIDTH, RD_HEIGHT } };
    const RD_VTX *a = &rd_frame.background[0], *b = &rd_frame.background[1], *c = &rd_frame.background[2];
    double area = ((double)b->x - a->x) * ((double)c->y - a->y) - ((double)b->y - a->y) * ((double)c->x - a->x);
    RD_VTX v[4];
    int i, sh;
    nbg = 0;
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
        bg_vtx[i] = v[i];
    }
    for (i = 0; i < 4; i++) {
        if (!(v[i].z > 0 && v[i].z < 1e30f)) {
            return;
        }
    }
    if (rd_frame.nstates >= RD_MAX_STATES) {
        return;
    }
    bg_state = rd_frame.nstates++;
    memset(&rd_frame.states[bg_state], 0, sizeof rd_frame.states[bg_state]);
    rd_frame.states[bg_state].list = RD_OPAQUE;
    rd_frame.states[bg_state].depth = RD_ALWAYS;
    rd_frame.states[bg_state].zwrite = 1;
    rd_frame.states[bg_state].src = RD_ONE;
    rd_frame.states[bg_state].dst = RD_ZERO;
    rd_frame.states[bg_state].fog = RD_FOG_NONE;
    bg_tri[0].state = bg_tri[1].state = (uint16_t)bg_state;
    bg_tri[0].v[0] = v[0], bg_tri[0].v[1] = v[1], bg_tri[0].v[2] = v[2];
    bg_tri[1].v[0] = v[1], bg_tri[1].v[1] = v[3], bg_tri[1].v[2] = v[2];
    nbg = 2;
}

/* a float's bits, sorting as the float does */
static uint32_t sort_bits(float f) {
    uint32_t b;
    memcpy(&b, &f, 4);
    return b & 0x80000000u ? ~b : b | 0x80000000u;
}

/* a stable radix sort, 8 bits at a time */
static void radix_sort_pairs(uint32_t* key, int* val, uint32_t* ktmp, int* vtmp, int n) {
    int shift, i;
    for (shift = 0; shift < 32; shift += 8) {
        int count[257] = { 0 };
        for (i = 0; i < n; i++) {
            count[(key[i] >> shift & 255) + 1]++;
        }
        if (count[(key[0] >> shift & 255) + 1] == n) {
            continue;
        }
        for (i = 0; i < 256; i++) {
            count[i + 1] += count[i];
        }
        for (i = 0; i < n; i++) {
            int at = count[key[i] >> shift & 255]++;
            ktmp[at] = key[i];
            vtmp[at] = val[i];
        }
        memcpy(key, ktmp, (size_t)n * sizeof *key);
        memcpy(val, vtmp, (size_t)n * sizeof *val);
    }
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

static void trans_add(const RD_TRI* t, float zlo, float zhi) {
    if (ntrans < MAX_ITEMS) {
        trans_item[ntrans] = t;
        trans_lo[ntrans] = zlo;
        trans_hi[ntrans] = zhi;
        trans_key[ntrans] = sort_bits((zlo + zhi) * 0.5f);
        ntrans++;
    }
}

static void trans_add_poly(const RD_TRI* t, const RD_VTX* p, int n) {
    float zlo = p[0].z, zhi = p[0].z;
    int i;
    for (i = 1; i < n; i++) {
        zlo = min_f(zlo, p[i].z);
        zhi = max_f(zhi, p[i].z);
    }
    for (i = 2; i < n && nparts < MAX_PARTS; i++) {
        RD_TRI* r = &trans_part[nparts++];
        r->state = t->state;
        r->v[0] = p[0];
        r->v[1] = p[i - 1];
        r->v[2] = p[i];
        trans_add(r, zlo, zhi);
    }
}

/* t cut at the shallow ones' depths a to b - 1 */
static void trans_cut(const RD_TRI* t, int a, int b) {
    RD_VTX buf[2][9], part[9];
    RD_VTX* rest = buf[0];
    int nr = 3, np, j;
    memcpy(rest, t->v, sizeof t->v);
    if (rd_frame.states[t->state].flat) {
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

/* the translucent list far first by middle depth, stable; a deep triangle is cut at the shallow ones'
   (sprites') depths, so each part sorts right against them */
#define SHALLOW 1.25f       /* farthest over nearest */
#define DEEP 4.0f
static void sort_trans(void) {
    int n = rd_frame.ntris[RD_TRANS], i, nz = 0, m, deep = 0;
    ntrans = nparts = 0;
    for (i = 0; i < n; i++) {
        const RD_TRI* t = &rd_frame.tris[RD_TRANS][i];
        float zlo = min_f(t->v[0].z, min_f(t->v[1].z, t->v[2].z)), zhi = max_f(t->v[0].z, max_f(t->v[1].z, t->v[2].z));
        if (!culled(&rd_frame.states[t->state], t)) {
            deep |= zhi > zlo * DEEP;
            trans_add(t, zlo, zhi);
        }
    }
    if (deep) {
        int nitems = ntrans;
        for (i = 0; i < nitems; i++) {
            if (trans_lo[i] > 0.0f && trans_hi[i] <= trans_lo[i] * SHALLOW && nz + 2 <= MAX_ITEMS) {
                sprite_z[nz++] = zbits(trans_lo[i]);
                sprite_z[nz++] = zbits(trans_hi[i]);
            }
        }
        radix_sort(sprite_z, trans_key2, nz);
        for (i = m = 0; i < nz; i++) {
            if (m == 0 || zfloat(sprite_z[i]) > zfloat(sprite_z[m - 1]) * 1.001f) {
                sprite_z[m++] = sprite_z[i];
            }
        }
        nz = m;
        for (i = 0; i < nitems; i++) {
            float zlo = trans_lo[i], zhi = trans_hi[i];
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
                trans_cut(trans_item[i], a, b);
                trans_item[i] = NULL;
            }
        }
    }
    n = ntrans;
    for (i = 0; i < n; i++) {
        trans_tmp[i] = i;
    }
    radix_sort_pairs(trans_key, trans_tmp, trans_key2, trans_tmp2, n);
    for (i = m = 0; i < n; i++) {
        int j = trans_tmp[i];
        if (trans_item[j] != NULL) {
            trans_order[m] = j;
            tri_k0[m] = (uint8_t)slice_of(trans_lo[j]);
            tri_k1[m] = (uint8_t)slice_of(trans_hi[j]);
            m++;
        }
    }
    ntrans = m;
}

static void tri_slices(int list) {
    int i;
    for (i = 0; i < rd_frame.ntris[list]; i++) {
        const RD_TRI* t = &rd_frame.tris[list][i];
        tri_k0[i] = (uint8_t)slice_of(min_f(t->v[0].z, min_f(t->v[1].z, t->v[2].z)));
        tri_k1[i] = (uint8_t)slice_of(max_f(t->v[0].z, max_f(t->v[1].z, t->v[2].z)));
        if (culled(&rd_frame.states[t->state], t)) {
            tri_k0[i] = 1, tri_k1[i] = 0;
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
    for (k = 0; k < nslices; k++) {
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

/* each list slice by slice, far first */
static void build(void) {
    int list, i, k;
    uint64_t t0, t1;
    nverts = 0;
    nbatches = 0;
    for (list = 0; list < RD_LISTS; list++) {
        static int told;
        if (rd_frame.ntris[list] == RD_MAX_TRIS && !told) {
            host_log("a list of more than %d triangles: the rest are not drawn", RD_MAX_TRIS);
            told = 1;
        }
    }
    background_tris();
    make_slices();
    for (k = 0; k < nslices; k++) {
        double span = slice_lo[k + 1] - slice_lo[k];
        slice_scale[k] = (1.0 - 2.0 / 4096) / span;
        slice_base[k] = 1.0 / 4096 - slice_lo[k] * slice_scale[k];
    }
    for (list = 0; list < RD_TRANS; list++) {
        const RD_TRI* tris = rd_frame.tris[list];
        int n = rd_frame.ntris[list];
        int sorted;
        tri_slices(list);
        sorted = bucket(n);
        for (k = 0; k < nslices; k++) {
            for (i = 0; list == RD_OPAQUE && i < nbg; i++) {
                add_tri(k, &bg_tri[i]);
            }
            if (sorted) {
                for (i = slice_at[k]; i < slice_at[k + 1]; i++) {
                    add_tri(k, &tris[by_slice[i]]);
                }
                continue;
            }
            for (i = 0; i < n; i++) {
                if (tri_k0[i] <= k && k <= tri_k1[i]) {
                    add_tri(k, &tris[i]);
                }
            }
        }
    }
    t0 = svcGetSystemTick();
    sort_trans();
    t1 = svcGetSystemTick();
    ctr_render_ticks[CTR_TICKS_SORT] += t1 - t0;
    if (bucket(ntrans)) {
        for (k = 0; k < nslices; k++) {
            for (i = slice_at[k]; i < slice_at[k + 1]; i++) {
                add_tri(k, trans_item[trans_order[by_slice[i]]]);
            }
        }
    } else {
        for (k = 0; k < nslices; k++) {
            for (i = 0; i < ntrans; i++) {
                if (tri_k0[i] <= k && k <= tri_k1[i]) {
                    add_tri(k, trans_item[trans_order[i]]);
                }
            }
        }
    }
    ctr_render_ticks[CTR_TICKS_TRANS] += svcGetSystemTick() - t1;
}

/* the PowerVR2's shading instruction with the texel (t) and vertex colour (p), then the offset added */
static void combine(const RD_STATE* s, int pal, uint32_t offset) {
    int key = (s->tex != NULL) | pal << 1 | s->shading << 2 | s->use_alpha << 4 | s->tex_alpha << 5 | s->offset << 6;
    GPU_TEVSRC t = pal ? GPU_FRAGMENT_SECONDARY_COLOR : GPU_TEXTURE0;
    C3D_TexEnv* e;
    if (cur.valid && cur.combine == key && (!s->offset || cur.offset == offset)) {
        return;
    }
    cur.combine = key;
    cur.offset = offset;
    e = C3D_GetTexEnv(0);
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
    e = C3D_GetTexEnv(1);
    C3D_TexEnvInit(e);
    if (s->offset) {
        /* the constant is ABGR */
        C3D_TexEnvColor(e, 0xFF000000u | (offset & 0xFF) << 16 | (offset & 0xFF00) | (offset >> 16 & 0xFF));
        C3D_TexEnvSrc(e, C3D_RGB, GPU_PREVIOUS, GPU_CONSTANT, 0);
        C3D_TexEnvFunc(e, C3D_RGB, GPU_ADD);
        C3D_TexEnvSrc(e, C3D_Alpha, GPU_PREVIOUS, 0, 0);
        C3D_TexEnvFunc(e, C3D_Alpha, GPU_REPLACE);
    }
}

static C3D_FogLut fog_lut;
static int fog_on;          /* the table is not all 0 */

/* rd_soft.c's fog_value */
static float fog_value(float z) {
    float q = rd_fog.density * z, m, pos;
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
        return rd_fog.table[127];
    }
    return rd_fog.table[i] + (rd_fog.table[i + 1] - rd_fog.table[i]) * (pos - (float)i);
}

/* the GPU's table, by the depth buffer's value: through the slices back to 1/w and the game's table */
static void fog_setup(void) {
    float data[256], v[129];
    int i;
    fog_on = 0;
    for (i = 0; i < 128; i++) {
        fog_on |= rd_fog.table[i] != 0.0f;
    }
    if (!fog_on) {
        return;
    }
    for (i = 0; i <= 128; i++) {
        double depth = (double)i / 128 * nslices;
        int k = (int)depth < nslices ? (int)depth : nslices - 1;
        double z = (depth - k - slice_base[k]) / slice_scale[k];
        v[i] = 1.0f - fog_value((float)z);
    }
    for (i = 0; i < 128; i++) {
        data[i] = v[i];
        data[128 + i] = v[i + 1] - v[i];
    }
    FogLut_FromArray(&fog_lut, data);
    C3D_FogLutBind(&fog_lut);
    C3D_FogColor((rd_fog.color & 0xFF) << 16 | (rd_fog.color & 0xFF00) | (rd_fog.color >> 16 & 0xFF));
}

static void apply(const RD_STATE* s, uint32_t offset) {
    /* punch-through and translucent: not behind, and only punch-through writes the depth */
    int func = depth_func[s->list == RD_OPAQUE ? s->depth & 7 : RD_GEQUAL];
    int zwrite = s->list == RD_OPAQUE ? s->zwrite : s->list == RD_PUNCH;
    int blend = s->list == RD_TRANS ? s->src << 8 | s->dst : -1;
    int alpha_test = s->list == RD_PUNCH, pal = 0;
    GTEX* g = NULL;
    if (!cur.valid || cur.func != func || cur.zwrite != zwrite) {
        C3D_DepthTest(true, (GPU_TESTFUNC)func,
                      (GPU_WRITEMASK)(GPU_WRITE_RED | GPU_WRITE_GREEN | GPU_WRITE_BLUE | (zwrite ? GPU_WRITE_DEPTH : 0)));
        cur.func = func;
        cur.zwrite = zwrite;
    }
    if (!cur.valid || cur.alpha_test != alpha_test) {
        C3D_AlphaTest(alpha_test, GPU_GEQUAL, 128);
        cur.alpha_test = alpha_test;
    }
    if (!cur.valid || cur.blend != blend) {
        if (blend < 0) {
            C3D_AlphaBlend(GPU_BLEND_ADD, GPU_BLEND_ADD, GPU_ONE, GPU_ZERO, GPU_ONE, GPU_ZERO);
        } else {
            C3D_AlphaBlend(GPU_BLEND_ADD, GPU_BLEND_ADD, blend_factor(s->src, 1), blend_factor(s->dst, 0), GPU_ONE, GPU_ZERO);
        }
        cur.blend = blend;
    }
    if (s->tex != NULL) {
        g = find_tex(s->tex);
        pal = is_pal(s->tex->format);
        if (g != NULL && g->ok) {
            int filter = s->bilinear && !pal;
            if (cur.tex != &g->tex || cur.filter != filter || cur.wrap != s->uv_mode || cur.mip_d != s->mip_d) {
                /* the D adjust, as rd_soft */
                static const float bias[16] = { 0, -4, -2, -1 };
                C3D_TexSetLodBias(&g->tex, bias[s->mip_d & 15]);
                /* clamp (1 v, 2 u) and flip (4 v, 8 u) */
                C3D_TexSetWrap(&g->tex, s->uv_mode & 2 ? GPU_CLAMP_TO_EDGE : s->uv_mode & 8 ? GPU_MIRRORED_REPEAT : GPU_REPEAT,
                               s->uv_mode & 1 ? GPU_CLAMP_TO_EDGE : s->uv_mode & 4 ? GPU_MIRRORED_REPEAT : GPU_REPEAT);
                C3D_TexSetFilter(&g->tex, filter ? GPU_LINEAR : GPU_NEAREST, filter ? GPU_LINEAR : GPU_NEAREST);
                cur.wrap = s->uv_mode;
                cur.mip_d = s->mip_d;
                C3D_TexBind(0, &g->tex);
                cur.tex = &g->tex;
                cur.filter = filter;
            }
        }
    }
    {
        /* FOG_TABLE2 is drawn as FOG_TABLE */
        int fog = fog_on && (s->fog == RD_FOG_TABLE || s->fog == RD_FOG_TABLE2);
        if (!cur.valid || cur.fog != fog) {
            C3D_FogGasMode(fog ? GPU_FOG : GPU_NO_FOG, GPU_PLAIN_DENSITY, false);
            cur.fog = fog;
        }
    }
    combine(s, pal, offset);
    cur.valid = 1;
    if (pal) {
        int n = s->tex->format == RD_PAL4 ? 16 : 256;
        if (cur.lut_base != s->palette || cur.lut_n != n || cur.lut_serial != rd_palette_serial) {
            lut_palette(s->palette, n);
            cur.lut_base = s->palette;
            cur.lut_n = n;
            cur.lut_serial = rd_palette_serial;
        }
    }
}

static void draw(void) {
    int i;
    GV* bg;
    if (nverts + 9 > MAX_VERTS) {
        nverts = MAX_VERTS - 9;
    }
    /* the background when it has no depth, then a triangle of no size, so that citro3d sends its pending
       light tables before the palettes go in */
    bg = &verts[nverts];
    {
        GV a = { GAME_X, 0, 0.5f, 1, 0, 0, 0xFF000000u | bg_vtx[0].base };
        GV b = { GAME_X + RD_WIDTH, 0, 0.5f, 1, 0, 0, 0xFF000000u | bg_vtx[1].base };
        GV cc = { GAME_X, RD_HEIGHT, 0.5f, 1, 0, 0, 0xFF000000u | bg_vtx[2].base };
        GV d = { GAME_X + RD_WIDTH, RD_HEIGHT, 0.5f, 1, 0, 0, 0xFF000000u | bg_vtx[3].base };
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
    GSPGPU_FlushDataCache(verts, (u32)(nverts + 9) * sizeof(GV));
    fog_setup();

    memset(&cur, 0, sizeof cur);
    cur.lut_base = -1;
    C3D_SetScissor(GPU_SCISSOR_NORMAL, 0, game_y0, target_w, game_y0 + game_h);
    {
        static const RD_STATE bgs = { .list = RD_OPAQUE, .depth = RD_ALWAYS, .src = RD_ONE, .dst = RD_ZERO,
                                      .shading = RD_DECAL, .fog = RD_FOG_NONE };
        apply(&bgs, 0);
        if (nbg == 0) {
            C3D_DrawArrays(GPU_TRIANGLES, nverts, 6);
        }
        C3D_DrawArrays(GPU_TRIANGLES, nverts + 6, 3);
    }
    for (i = 0; i < nbatches; i++) {
        if (i == 0 || batches[i].slice != batches[i - 1].slice) {
            /* slice k in the k-th part of the depth buffer */
            C3D_DepthMap(true, -1.0f / (float)nslices, (float)batches[i].slice / (float)nslices);
        }
        apply(&rd_frame.states[batches[i].state], batches[i].offset);
        C3D_DrawArrays(GPU_TRIANGLES, batches[i].first, batches[i].count);
        ctr_render_count[CTR_COUNT_TRIS] += (uint32_t)batches[i].count / 3;
        ctr_render_count[CTR_COUNT_DRAWS]++;
    }
}

int ctr_render_init(int full, int stretch) {
    C3D_AttrInfo* attr;
    C3D_BufInfo* buf;
    int i;

    if (!C3D_Init(CMDBUF_SIZE)) {
        return 0;
    }
    target_w = full ? VIEW_H : VIEW_H / 2;
    target_h = full ? VIEW_W : VIEW_W / 2;
    game_y0 = stretch ? 0 : GAME_X * target_h / VIEW_W;
    game_h = stretch ? target_h : RD_WIDTH * target_h / VIEW_W;
    target = C3D_RenderTargetCreate(target_w, target_h, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
    if (target == NULL) {
        return 0;
    }
    C3D_RenderTargetSetOutput(target, GFX_TOP, GFX_LEFT,
                              DISPLAY_TRANSFER_FLAGS | GX_TRANSFER_SCALING(full ? GX_TRANSFER_SCALE_XY : GX_TRANSFER_SCALE_NO));
    verts = linearAlloc(sizeof(GV) * MAX_VERTS);
    if (verts == NULL) {
        return 0;
    }
    dvlb = DVLB_ParseFile((u32*)ctr_render_shbin, ctr_render_shbin_size);
    shaderProgramInit(&prog);
    shaderProgramSetVsh(&prog, &dvlb->DVLE[0]);
    C3D_BindProgram(&prog);
    u_proj = shaderInstanceGetUniformLocation(prog.vertexShader, "projection");

    /* the game's pixels on the turned target; depth d to z = -d */
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

    attr = C3D_GetAttrInfo();
    AttrInfo_Init(attr);
    AttrInfo_AddLoader(attr, 0, GPU_FLOAT, 4);
    AttrInfo_AddLoader(attr, 1, GPU_FLOAT, 2);
    AttrInfo_AddLoader(attr, 2, GPU_UNSIGNED_BYTE, 4);
    buf = C3D_GetBufInfo();
    BufInfo_Init(buf);
    BufInfo_Add(buf, verts, sizeof(GV), 3, 0x210);

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
    return 1;
}

/* MVC2_DUMP=n: the batches of the game's frame n, in the log */
static void dump(void) {
    const char* want = getenv("MVC2_DUMP");
    int i;
    if (want == NULL || (unsigned)atoi(want) != ctr_presented) {
        return;
    }
    host_log("background z %.9g %.9g %.9g, corners %.9g %.9g %.9g %.9g, %d triangles", rd_frame.background[0].z,
             rd_frame.background[1].z, rd_frame.background[2].z, bg_vtx[0].z, bg_vtx[1].z, bg_vtx[2].z, bg_vtx[3].z, nbg);
    for (i = 0; i <= nslices; i++) {
        host_log("slice %d from %.9g", i, slice_lo[i]);
    }
    for (i = 0; i < nbatches; i++) {
        const BATCH* b = &batches[i];
        const RD_STATE* s = &rd_frame.states[b->state];
        const GV* v = &verts[b->first];
        host_log("batch %d: slice %d list %d depth %d zw %d blend %d/%d shade %d ua %d ta %d bil %d off %d(%06X) tex %dx%d f%d pal %d "
                 "tris %d at %.0f,%.0f c %08X", i, b->slice, s->list, s->depth, s->zwrite, s->src, s->dst, s->shading, s->use_alpha,
                 s->tex_alpha, s->bilinear, s->offset, b->offset, s->tex ? s->tex->width : 0, s->tex ? s->tex->height : 0,
                 s->tex ? s->tex->format : -1, s->palette, b->count / 3, v->x - GAME_X, v->y, v->c);
    }
}

uint64_t ctr_render_ticks[CTR_TICKS];
uint32_t ctr_render_count[CTR_COUNTS];

void ctr_render_frame(void) {
    static uint64_t last;
    uint64_t t0 = svcGetSystemTick(), t1, t2, t3;
    frame_no++;
    /* citro3d's frame sync would hold a slow frame to two vertical blanks */
    C3D_FrameBegin(t0 - last < SYSCLOCK_ARM11 / 60 ? C3D_FRAME_SYNCDRAW : 0);
    t1 = svcGetSystemTick();
    last = t1;
    build();
    dump();
    t2 = svcGetSystemTick();
    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, u_proj, &proj);
    C3D_RenderTargetClear(target, C3D_CLEAR_ALL, 0x000000FF, 0);
    C3D_FrameDrawOn(target);
    draw();
    t3 = svcGetSystemTick();
    C3D_FrameEnd(0);
    ctr_render_ticks[CTR_TICKS_SYNC] += t1 - t0;
    ctr_render_ticks[CTR_TICKS_LISTS] += t2 - t1;
    ctr_render_ticks[CTR_TICKS_DRAW] += t3 - t2;
    ctr_render_ticks[CTR_TICKS_END] += svcGetSystemTick() - t3;
    PROF_ADD(PZ_GPU_WAIT, t1 - t0);
    PROF_ADD(PZ_LISTS, t2 - t1);
    PROF_ADD(PZ_DRAWS, svcGetSystemTick() - t2);
}

/* the last frame as 640x480 ARGB, for test runs */
int ctr_render_read(uint32_t* frame) {
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

void ctr_render_exit(void) {
    int i;
    for (i = 0; i < MAX_TEX; i++) {
        if (gtex[i].ok) {
            C3D_TexDelete(&gtex[i].tex);
            gtex[i].ok = 0;
        }
    }
    linearFree(verts);
    shaderProgramFree(&prog);
    DVLB_Free(dvlb);
    C3D_RenderTargetDelete(target);
    C3D_Fini();
}

/*
 * rd.h  What the port draws: the PowerVR2's screen-space triangles, 1/w deep, and their states
 */

#ifndef RD_H
#define RD_H

#include <stdint.h>

#define RD_WIDTH 640
#define RD_HEIGHT 480

typedef struct {
    float x, y;         /* pixels */
    float z;            /* 1/w: bigger is nearer */
    float u, v;
    uint32_t base;      /* ARGB */
    uint32_t offset;    /* ARGB, added after texturing */
} RD_VTX;

enum { RD_OPAQUE, RD_PUNCH, RD_TRANS };
/* as the TSP numbers them; FOG_VERTEX is not done */
enum { RD_FOG_TABLE, RD_FOG_VERTEX, RD_FOG_NONE, RD_FOG_TABLE2 };
enum { RD_NEVER, RD_LESS, RD_EQUAL, RD_LEQUAL, RD_GREATER, RD_NOTEQUAL, RD_GEQUAL, RD_ALWAYS };
/* "other" is the destination for the source factor, and the source for the destination's */
enum { RD_ZERO, RD_ONE, RD_OTHER, RD_INV_OTHER, RD_SRC_ALPHA, RD_INV_SRC_ALPHA, RD_DST_ALPHA, RD_INV_DST_ALPHA };
enum { RD_DECAL, RD_MODULATE, RD_DECAL_ALPHA, RD_MODULATE_ALPHA };
enum { RD_ARGB1555, RD_RGB565, RD_ARGB4444, RD_PAL4 = 5, RD_PAL8 = 6 };
/* a model in the scene; the game's front (a model put without the camera's view, or a strip at screen places: the
   timer, gauges, words and menus); a sprite */
enum { RD_SCENE, RD_FRONT, RD_SPRITE };

/* untwiddled, a byte a texel for the palette formats */
typedef struct rd_texture {
    int width, height;
    int format;
    void* texels;
    uint32_t serial;    /* changes when its texels do */
    int levels;         /* mip[k], k from 1 to levels, is width >> k by height >> k */
    const void* mip[11];
    const void* twiddled;   /* on the 3DS, a PAL4 texture's source, which it converts from */
    /* on the 3DS, a VQ square whose texels aren't kept (texels is its source): unpack makes them, and its mip levels
       after, in the unpacked bytes at out->texels, and sets out's mip[] */
    void (*unpack)(const struct rd_texture* t, struct rd_texture* out);
    uint32_t unpacked;
    /* a PAL4 square drawn, point sampled, only from its corner this many texels on a side (0: all of it), its texels
       past that whatever they were: a front end may draw them as the corner's edge */
    int corner;
} RD_TEXTURE;

typedef struct {
    uint8_t list;
    uint8_t depth;
    uint8_t zwrite;
    uint8_t src, dst;
    uint8_t shading;
    uint8_t use_alpha;      /* 0: the vertex colour's alpha is taken as 1 */
    uint8_t tex_alpha;      /* 0: the texel's alpha is taken as 1 */
    uint8_t bilinear;
    uint8_t offset;
    uint8_t flat;           /* a triangle takes its last vertex's colours */
    uint8_t fog;
    uint8_t uv_mode;        /* the TSP's bits 15-18: 1 clamp v, 2 clamp u, 4 flip v, 8 flip u */
    uint8_t mip_d;          /* the TSP's D adjust */
    uint8_t cull;           /* 0 none, 1 tiny, 2 also counter-clockwise, 3 also clockwise */
    uint8_t origin;         /* what drew it (RD_SCENE...) */
    const RD_TEXTURE* tex;
    uint16_t palette;       /* the first entry the texels index */
} RD_STATE;

extern uint32_t rd_palette[1024];
extern uint32_t rd_palette_serial;
extern int rd_palette_mode;     /* the palette RAM's format: 0 ARGB1555, 1 RGB565, 2 ARGB4444 */

/* the fog registers; FOG_DENSITY is a 1.7 mantissa in bits 8-15 over a signed exponent */
void rd_fog_table(const float* table);
void rd_fog_color(uint32_t argb);
void rd_fog_density(uint32_t reg);

void rd_begin_frame(void);
/* set by a front end for a frame it won't show: its strips are dropped, and the library's models not made */
extern int rd_discard;
/* set by a front end whose pixels' centres aren't the game's columns' (the 3DS stretched): an upright quad's sides
   are moved to the game's pixel edges, so that it covers the columns it does at 640 wide and tiles the game puts a
   fraction apart meet as they do there */
extern int rd_snap_x;
/* three vertices whose plane covers the screen */
void rd_background_plane(const RD_VTX* v);
void rd_background(uint32_t argb);
void rd_strip(const RD_STATE* s, const RD_VTX* v, int n);
void rd_quad(const RD_STATE* s, const RD_VTX* v);              /* corners in order around it */

/* Models the GPU turns and lights: a group's vertices and triangles made once into a mesh, then drawn
   with a put's matrix (rows: w, x * w, y * w from the model's x, y, z, 1, the screen's x and y in pixels) */
typedef struct {
    float x, y, z, u, v;
    float nx, ny, nz;   /* lit */
    uint32_t argb;      /* vertex colours */
} NL_MVTX;

typedef struct {
    uint16_t first, count;  /* triangles */
    uint8_t cull, flat;     /* cull as RD_STATE's */
} NL_MSPAN;

enum { RD_GPU_UNLIT, RD_GPU_VTXCOL, RD_GPU_LIT };

typedef struct {
    int kind, nv, ntri, nspan;
    const NL_MVTX* v;
    const uint16_t* idx;    /* three a triangle */
    const NL_MSPAN* span;
    float lo[3], hi[3];
} NL_MESH;

typedef struct {
    int mesh, kind;
    float face[4];          /* ARGB, 0 to 1 */
    uint32_t face_argb;
    float l[3], ambient;    /* the light in the model's space */
    float wlo, whi;         /* the group's w, from its bounds */
    float key;              /* translucent: the 1/w it sorts at among its list's strips; not > 0: the middle of its bounds */
    float key_far, key_near;    /* a deep one drawn in two passes: its back faces' 1/w, then its front faces' */
    uint8_t two_sided;      /* a shell, none of its triangles culled, with faces seen facing each way: two passes */
    uint8_t faces;          /* the renderer's: 0 all, 1 the back faces' pass, 2 the front faces' */
} RD_GPU_GROUP;

/* set by a back end that draws them; MVC2_GPU_TL=0 clears it */
extern int rd_gpu_tl;
extern int rd_gpu_tl_kinds, rd_gpu_tl_max;   /* the kinds taken, by bit, and the most groups a frame */
extern int rd_gpu_tl_trans;                 /* translucent groups taken too */
extern float rd_gpu_tl_trans_deep;          /* but not one whose farthest w is more than this times its nearest (0: any) */
extern uint32_t rd_gpu_tl_deep_left;        /* those left to the caller for their depth, so far */
extern int rd_gpu_tl_two_pass;              /* a deep two-sided one taken all the same, its back faces then its front */
extern uint32_t rd_gpu_tl_two_passed;       /* those, so far */
extern int rd_gpu_tl_nearclip;              /* none taken that may reach the near plane, for the caller to clip */
extern uint32_t rd_gpu_tl_near_left;        /* those left to the caller, so far */
int rd_gpu_mesh_make(const NL_MESH* m);     /* its id, or -1 for no room */
void rd_gpu_mesh_free(int id);              /* kept while a frame drawing it may still be read */
uint32_t rd_gpu_mesh_room(void);            /* bytes */
int rd_gpu_put(const float m[16], float near_w);    /* its index, or -1 when there is no room */
int rd_gpu_group(int put, const RD_STATE* s, const RD_GPU_GROUP* g);     /* 0: not taken, for the caller to draw */
/* set by a back end that draws only some: 0 for one the caller is to draw */
extern int (*rd_gpu_take)(const RD_STATE* s, const RD_GPU_GROUP* g, const float m[16]);

#ifdef __PSP__
/* psp_render.c: the frame's 1/w range and flat 1/w values, taken as its triangles are added */
typedef struct {
    float lo, hi, last[RD_TRANS + 1];
    int has3d;
} RD_ZRANGE;
#define RD_ZCUT 64.0f              /* game pixels past the screen a triangle's corners may be with their 1/w taken */
extern RD_ZRANGE rd_zrange;
void rd_zrange_begin(void);
void rd_flat_z(float z);

static inline void rd_z_take(float z) {
    if (z > 0.0f && z < 1e30f) {
        rd_zrange.lo = z < rd_zrange.lo ? z : rd_zrange.lo;
        rd_zrange.hi = z > rd_zrange.hi ? z : rd_zrange.hi;
    }
}

void rd_tri_z_cut(const RD_VTX* a, const RD_VTX* b, const RD_VTX* c);

static inline void rd_tri_z(int list, const RD_VTX* a, const RD_VTX* b, const RD_VTX* c) {
    if (a->z == b->z && b->z == c->z) {
        if (a->z != rd_zrange.last[list]) {
            rd_zrange.last[list] = a->z;
            rd_flat_z(a->z);
        }
        return;
    }
    rd_zrange.has3d = 1;
    /* one far past the screen: only the 1/w its part on the screen can have */
    if (a->x < -RD_ZCUT || b->x < -RD_ZCUT || c->x < -RD_ZCUT || a->y < -RD_ZCUT || b->y < -RD_ZCUT || c->y < -RD_ZCUT ||
        a->x > RD_WIDTH + RD_ZCUT || b->x > RD_WIDTH + RD_ZCUT || c->x > RD_WIDTH + RD_ZCUT ||
        a->y > RD_HEIGHT + RD_ZCUT || b->y > RD_HEIGHT + RD_ZCUT || c->y > RD_HEIGHT + RD_ZCUT) {
        rd_tri_z_cut(a, b, c);
        return;
    }
    rd_z_take(a->z);
    rd_z_take(b->z);
    rd_z_take(c->z);
}
#endif

/* "exact" (or NULL), or words: fast, half, nofog, nosort */
void rd_quality(const char* words);

/* rd_soft.c: the frame into ARGB (ABGR when built with RD_ABGR) */
void rd_end_frame(uint32_t* frame, int pitch);
/* and into RGB565, field -1 every row, 0 or 1 only the even or odd rows; frame is only written, in order */
void rd_end_frame565(uint16_t* frame, int pitch, int field);

#endif

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

/* untwiddled, a byte a texel for the palette formats */
typedef struct {
    int width, height;
    int format;
    void* texels;
    uint32_t serial;    /* changes when its texels do */
    int levels;         /* mip[k], k from 1 to levels, is width >> k by height >> k */
    const void* mip[11];
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
    const RD_TEXTURE* tex;
    uint16_t palette;       /* the first entry the texels index */
} RD_STATE;

extern uint32_t rd_palette[1024];
extern uint32_t rd_palette_serial;

/* the fog registers; FOG_DENSITY is a 1.7 mantissa in bits 8-15 over a signed exponent */
void rd_fog_table(const float* table);
void rd_fog_color(uint32_t argb);
void rd_fog_density(uint32_t reg);

void rd_begin_frame(void);
/* three vertices whose plane covers the screen */
void rd_background_plane(const RD_VTX* v);
void rd_background(uint32_t argb);
void rd_strip(const RD_STATE* s, const RD_VTX* v, int n);
void rd_quad(const RD_STATE* s, const RD_VTX* v);              /* corners in order around it */

/* "exact" (or NULL), or words: fast, half, nofog, nosort */
void rd_quality(const char* words);

/* rd_soft.c: the frame into ARGB */
void rd_end_frame(uint32_t* frame, int pitch);

#endif

/*
 * nl_port.h  What the port's NAOMI library routines share
 */

#ifndef NL_PORT_H
#define NL_PORT_H

#include <stddef.h>
#include <stdint.h>

#include "rd.h"

/* a texture header, as the game's data holds them */
typedef struct {
    int16_t width, height;
    int32_t type;
    void* address;
    int32_t globl_index;
} NLthd;

typedef struct {
    RD_TEXTURE tex;
    int type;
    int loaded;
    int palette;        /* the bank a sprite draws with */
    size_t bytes;
} NL_TEX;

#define NL_MAX_TEX 0x1000

extern NL_TEX nl_tex[NL_MAX_TEX];
extern int nl_tex_offset;
extern int nl_palette_direct;

int nl_tex_load(NL_TEX* t, const NLthd* thd);
int nl_tex_reload(NL_TEX* t, const void* texels);
void nl_tex_free_all(void);

/* nlObjPutSetFadeColor's, 0 to 1 */
extern float nl_fade_r, nl_fade_g, nl_fade_b;

static inline uint16_t nl_palette_base(const NL_TEX* t, int bank) {
    return (uint16_t)(t->tex.format == RD_PAL8 ? (bank & 3) * 256 : (bank & 63) * 16);
}

static inline uint32_t nl_float_argb(float a, float r, float g, float b) {
#define NL_C(f) ((uint32_t)((f) <= 0.0f ? 0 : (f) >= 1.0f ? 255 : (int)((f) * 255.0f + 0.5f)))
    return NL_C(a) << 24 | NL_C(r) << 16 | NL_C(g) << 8 | NL_C(b);
#undef NL_C
}

/* nl_xmtr.c's current matrix, by rows, and the view matrix */
const float* nl_xmtrx(void);
const float* nl_view_matrix(void);

#endif

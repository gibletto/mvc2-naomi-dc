/*
 * nl_sprite.c  The NAOMI library's sprites (libspr): a common texture's rectangle, anchored, zoomed
 * and turned in screen space
 */

#include <math.h>

#include "nl_port.h"

typedef struct {
    int32_t sprno;
    float x, y, z;
    float zm_x, zm_y;
    float u0, v0, u1, v1;
    int32_t ang;
    float trnsl;
    int32_t listType;
    int32_t attr;
    int32_t base_color;
    int32_t offset_color;
    int32_t reserved[4];
} NLsprarg;

/* attr */
#define NLSPR_DISP_MASK     0x0000000F  /* bits 0-1 across (1 left, 2 centre, 3 right), 2-3 down */
#define NLSPR_FLIP_H        0x00000010
#define NLSPR_FLIP_V        0x00000020
#define NLSPR_DEPTH_SHIFT   8           /* 0 for greater */
#define NLSPR_FOG_EBL       0x00000800
#define NLSPR_BILINEAR      0x00001000
#define NLSPR_FADE          0x00002000
#define NLSPR_SUPERSAMPLE   0x00010000
#define NLSPR_COLORSET      0x00020000

/* listType */
#define NLSPR_LISTTYPE_OPAQUE 0

static int max_spr = NL_MAX_TEX;
static float screen_w = 640.0f, screen_h = 480.0f;

void nlInitSprite(int* work, int nbLoadSpr, int nbMaxSpr) {
    (void)work;
    (void)nbLoadSpr;
    max_spr = nbMaxSpr < NL_MAX_TEX ? nbMaxSpr : NL_MAX_TEX;
}

void nlSpriteSetImaginaryScreenSize(float w, float h) {
    screen_w = w;
    screen_h = h;
}

int nlSpriteLoadTextureOnce(const NLthd* thd, const int32_t* pal, int i) {
    int index = nl_tex_offset + i;
    NL_TEX* t;
    if (index < 0 || index >= NL_MAX_TEX) {
        return 1;
    }
    t = &nl_tex[index];
    if (!nl_tex_load(t, &thd[i])) {
        return 1;
    }
    t->palette = pal != NULL ? pal[i] : 0;
    return 0;
}

int nlSpriteLoadTexture(const NLthd* thd, const int32_t* pal) {
    int i, r = 0;
    for (i = 0; thd[i].type != -1; i++) {
        r |= nlSpriteLoadTextureOnce(thd, pal, i);
    }
    return r;
}

int nlSpriteFreeTexture(const NLthd* thd, const int32_t* pal) {
    int i;
    (void)pal;
    for (i = 0; thd[i].type != -1; i++) {
        if (nl_tex_offset + i < NL_MAX_TEX) {
            nl_tex[nl_tex_offset + i].loaded = 0;
        }
    }
    return 0;
}

int nlSpriteChangePalette(int index, int bank) {
    int old;
    if (index < 0 || index >= NL_MAX_TEX) {
        return 0;
    }
    old = nl_tex[index].palette;
    nl_tex[index].palette = bank;
    return old;
}

static int has_alpha(const NL_TEX* t) {
    switch (t->tex.format) {
    case RD_RGB565:
        return 0;
    default:
        return 1;   /* the palettes are 4444 */
    }
}

int nlSprPut(const NLsprarg* a) {
    static const float corner[4][2] = { { 1, 1 }, { 3, 1 }, { 3, 3 }, { 1, 3 } };
    const NL_TEX* t;
    RD_STATE s = { 0 };
    RD_VTX v[4];
    float sx = 640.0f / screen_w, sy = 480.0f / screen_h;
    float hw, hh, ah, av, c, sn, trnsl, u0 = a->u0, v0 = a->v0, u1 = a->u1, v1 = a->v1, z;
    uint32_t base, offset = 0;
    int i, translucent, depth;

    if (a->sprno >= max_spr) {
        return -1;
    }
    if (a->sprno < 0 || !nl_tex[a->sprno].loaded) {
        return 0;
    }
    t = &nl_tex[a->sprno];
    trnsl = a->trnsl < 0.0f ? 0.0f : a->trnsl > 1.0f ? 1.0f : a->trnsl;
    if (a->listType == NLSPR_LISTTYPE_OPAQUE) {
        translucent = 0;
    } else if (a->listType == 2 || a->listType == 4) {
        translucent = 1;
    } else {
        translucent = trnsl < 1.0f || has_alpha(t);
    }

    hw = t->tex.width * a->zm_x * 0.5f;
    hh = t->tex.height * a->zm_y * 0.5f;
    if (a->attr & NLSPR_DISP_MASK) {
        ah = (float)(a->attr & 3);
        av = (float)(a->attr >> 2 & 3);
    } else {
        ah = av = 1.0f;
    }
    c = cosf((a->ang & 0xffff) * (6.2831853f / 65536.0f));
    sn = sinf((a->ang & 0xffff) * (6.2831853f / 65536.0f));
    if (a->attr & NLSPR_FLIP_H) {
        u0 = 1.0f - u0;
        u1 = 1.0f - u1;
    }
    /* v runs up the texture: the game's cells are stored bottom row first */
    if (!(a->attr & NLSPR_FLIP_V)) {
        v0 = 1.0f - v0;
        v1 = 1.0f - v1;
    }

    base = nl_float_argb(trnsl, 1.0f, 1.0f, 1.0f);
    if (a->attr & NLSPR_FADE) {
        base = nl_float_argb(trnsl, nl_fade_r, nl_fade_g, nl_fade_b);
    } else if (a->attr & NLSPR_COLORSET) {
        base = (uint32_t)a->base_color;
    }
    if (a->attr & NLSPR_COLORSET) {
        offset = (uint32_t)a->offset_color;
    }
    z = a->z != 0.0f ? 1.0f / a->z : 0.0f;
    for (i = 0; i < 4; i++) {
        float dx = (corner[i][0] - ah) * hw, dy = (corner[i][1] - av) * hh;
        v[i].x = (a->x + dx * c + dy * sn) * sx;
        v[i].y = (a->y - dx * sn + dy * c) * sy;
        v[i].z = z;
        v[i].u = i == 0 || i == 3 ? u0 : u1;
        v[i].v = i < 2 ? v0 : v1;
        v[i].base = base;
        v[i].offset = offset;
    }

    depth = a->attr >> NLSPR_DEPTH_SHIFT & 7;
    s.list = translucent ? RD_TRANS : RD_OPAQUE;
    s.depth = (uint8_t)(translucent || depth == 0 ? RD_GREATER : depth);
    s.zwrite = !translucent;
    s.src = RD_SRC_ALPHA;
    s.dst = RD_INV_SRC_ALPHA;
    s.shading = RD_MODULATE_ALPHA;
    s.use_alpha = (uint8_t)translucent;
    s.tex_alpha = 1;
    s.bilinear = (a->attr & NLSPR_BILINEAR) != 0;
    s.offset = (a->attr & NLSPR_COLORSET) != 0;
    s.fog = (a->attr & NLSPR_FOG_EBL) ? RD_FOG_TABLE : RD_FOG_NONE;
    s.tex = &t->tex;
    s.palette = nl_palette_base(t, t->palette);
    rd_quad(&s, v);
    return 0;
}

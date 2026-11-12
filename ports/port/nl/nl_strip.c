/*
 * nl_strip.c  The NAOMI library's strips (libstrip), as src/lib/libstrip/nlstrip.c sends them: screen
 * space (or through XMTRX, undivided, with NL_DISP_USEMATRIX), 1/z deep
 */

#include "nl_port.h"

typedef struct {
    float x, y, z;
} NLV3;

typedef struct {
    float x, y, z;
    uint32_t col;
} NLVERTEX00;

typedef struct {
    float x, y, z;
    float u, v;
    uint32_t col;
    uint32_t ofs;
} NLVERTEX03;

void nlCalcPoint(const NLV3* s, NLV3* d);

/* nlDispStripX's modes: words a vertex, and the colour's word */
static const int strip_fmt[18][2] = {
    { 4, 3 }, { 7, 0 }, { 0, 0 }, { 7, 5 }, { 0, 0 }, { 13, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
    { 5, 3 }, { 0, 0 }, { 11, 5 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 9, 0 },
};

static int disp_coord;
static int disp_palette;

#define MAX_STRIP 1024

void nlDispStripInit(void) {
    disp_coord = 0;
    disp_palette = 0;
}

void nlDispStripCoordMode(int mode) {
    disp_coord = mode;
}

int nlDispStripLoadTexture(int index, const NLthd* thd) {
    if (index < 0 || index >= NL_MAX_TEX) {
        return 1;
    }
    return !nl_tex_load(&nl_tex[index], thd);
}

void nlDispStripFreeTexture(int index) {
    if (index >= 0 && index < NL_MAX_TEX) {
        nl_tex[index].loaded = 0;
    }
}

void nlDispStripSetPalette(int index) {
    disp_palette = index;
}

static void position(RD_VTX* d, const float* p) {
    if (disp_coord) {
        NLV3 o;
        nlCalcPoint((const NLV3*)p, &o);
        d->x = o.x;
        d->y = o.y;
        d->z = 1.0f / o.z;
    } else {
        d->x = p[0];
        d->y = p[1];
        d->z = 1.0f / p[2];
    }
}

static void state(RD_STATE* s, int translucent, const NL_TEX* t, int offset) {
    s->list = translucent ? RD_TRANS : RD_OPAQUE;
    s->depth = RD_GREATER;
    s->zwrite = !translucent;
    s->src = RD_SRC_ALPHA;
    s->dst = RD_INV_SRC_ALPHA;
    s->shading = RD_MODULATE_ALPHA;
    s->use_alpha = (uint8_t)translucent;
    s->tex_alpha = 1;
    s->offset = (uint8_t)offset;
    s->tex = t != NULL ? &t->tex : NULL;
    s->palette = t != NULL ? nl_palette_base(t, nl_palette_direct ? disp_palette : t->palette) : 0;
}

static void strip(int num, int mode, const uint32_t* vt, int translucent) {
    RD_STATE s = { 0 };
    RD_VTX v[MAX_STRIP];
    const int* fmt = strip_fmt[mode];
    int i;
    if (num > MAX_STRIP || fmt[0] == 0) {
        return;
    }
    state(&s, translucent, NULL, 0);
    for (i = 0; i < num; i++, vt += fmt[0]) {
        position(&v[i], (const float*)vt);
        v[i].u = v[i].v = 0.0f;
        v[i].base = vt[fmt[1]];
        v[i].offset = 0;
    }
    rd_strip(&s, v, num);
}

static void strip_tex(int num, const NLVERTEX03* vt, int txtnum, int translucent, int offset) {
    RD_STATE s = { 0 };
    RD_VTX v[MAX_STRIP];
    int i;
    if (num > MAX_STRIP || txtnum < 0 || txtnum >= NL_MAX_TEX || !nl_tex[txtnum].loaded) {
        return;
    }
    state(&s, translucent, &nl_tex[txtnum], offset);
    for (i = 0; i < num; i++, vt++) {
        position(&v[i], &vt->x);
        v[i].u = vt->u;
        v[i].v = vt->v;
        v[i].base = vt->col;
        v[i].offset = offset ? vt->ofs : 0;
    }
    rd_strip(&s, v, num);
}

void nlDispStrip(int num, const NLVERTEX00* vt) {
    strip(num, 0, (const uint32_t*)vt, 0);
}

void nlDispStripX(int num, int mode, const uint32_t* vt) {
    strip(num, mode, vt, 0);
}

void nlDispStripTrnsl(int num, const NLVERTEX00* vt) {
    strip(num, 0, (const uint32_t*)vt, 1);
}

void nlDispStripTrnslX(int num, int mode, const uint32_t* vt) {
    strip(num, mode, vt, 1);
}

void nlDispStripTex(int num, const NLVERTEX03* vt, int txtnum) {
    strip_tex(num, vt, txtnum, 0, 0);
}

void nlDispStripTexTrnsl(int num, const NLVERTEX03* vt, int txtnum) {
    strip_tex(num, vt, txtnum, 1, 0);
}

void nlDispStripTexOfs(int num, const NLVERTEX03* vt, int txtnum) {
    strip_tex(num, vt, txtnum, 0, 1);
}

void nlDispStripTexOfsTrnsl(int num, const NLVERTEX03* vt, int txtnum) {
    strip_tex(num, vt, txtnum, 1, 1);
}

/*
 * nl_obj.c  The NAOMI library's models (libnlobj: nlObjPut and its texture loads)
 *
 * A model: a 6-word header (word 0: draw it; word 1 bit 1: no lighting), then groups, a zero word last.
 * A group: the polygon header, a bounding sphere, texture number (32), lighting byte (36: -1 flat), ambient
 * (40), face and offset ARGB floats (44, 60), primitives' length (76), primitives (80). A primitive: a
 * header word (bit 3 triangles, else a strip; bit 6 Gouraud; bits 0-1 culling; bit 7 a strip going on), a
 * count, then 32-byte vertices (bit 0 of x set) or 8-byte ones pointing back to a position's record.
 * Drawn as nlobjasm.src does, with triangles cut at the near plane.
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "naomi_host.h"
#include "nl_port.h"

typedef struct {
    float x, y, z;
} NLV3;

/* nlproj.c's projection, in ftrv's order (nl_set_cnv_proj_pers) */
extern float nl_cnv_proj_pers[16];

static NLV3 light = { 1.0f, 0.0f, 0.0f };
static float global_ambience = 0.3f;
static int trnsl_blend = 0x68;      /* nlObjPutSetTrnslMode's form: SRC_ALPHA, INV_SRC_ALPHA */

/* nlproj.c's NLOBJPUTPARAM: libnlobj cuts at -znear (0x0c) against the matrix's fourth row, -w */
extern uint8_t nl_objput_param[];

static float near_w = 1e-3f;
#define MAX_VTX 4096

/* a vertex after the transform, before the divide */
typedef struct {
    float w, x, y;
    float u, v;
    float c[4];         /* ARGB, 0 to 1 */
} OVTX;

static OVTX ov[MAX_VTX];
static RD_VTX sv[MAX_VTX];          /* for those in front of the near plane */
static int obj_log;

static uint32_t word(const uint8_t* p) {
    uint32_t w;
    memcpy(&w, p, 4);
    return w;
}

static float flt(const uint8_t* p) {
    float f;
    memcpy(&f, p, 4);
    return f;
}

/* d = a x b, 4x4 by rows */
static void mul(float* d, const float* a, const float* b) {
    float t[16];
    int r, c;
    for (r = 0; r < 4; r++) {
        for (c = 0; c < 4; c++) {
            t[r * 4 + c] = a[r * 4] * b[c] + a[r * 4 + 1] * b[4 + c] + a[r * 4 + 2] * b[8 + c] + a[r * 4 + 3] * b[12 + c];
        }
    }
    memcpy(d, t, sizeof t);
}

/* the PowerVR2 state of a polygon header */

static const RD_TEXTURE* group_texture(uint32_t pcw, uint32_t tcw, int texno) {
    uint32_t index = tcw & 0x1FFFFF;
    if (!(pcw & 8) || texno < 0 || index >= NL_MAX_TEX || !nl_tex[index].loaded) {
        return NULL;
    }
    return &nl_tex[index].tex;
}

static void pvr_state(RD_STATE* s, uint32_t pcw, uint32_t isp, uint32_t tsp, uint32_t tcw, const RD_TEXTURE* tex) {
    int list = pcw >> 24 & 7;
    memset(s, 0, sizeof *s);
    s->list = list == 2 ? RD_TRANS : list == 4 ? RD_PUNCH : RD_OPAQUE;
    s->depth = (uint8_t)(isp >> 29);
    s->zwrite = !(isp >> 26 & 1);
    s->src = (uint8_t)(tsp >> 29);
    s->dst = (uint8_t)(tsp >> 26 & 7);
    s->use_alpha = tsp >> 20 & 1;
    s->tex_alpha = !(tsp >> 19 & 1);
    s->fog = tsp >> 22 & 3;
    s->bilinear = (tsp >> 13 & 3) != 0;
    s->shading = (uint8_t)(tsp >> 6 & 3);
    s->uv_mode = (uint8_t)(tsp >> 15 & 15);
    s->mip_d = (uint8_t)(tsp >> 8 & 15);
    s->offset = pcw >> 2 & 1;
    s->tex = tex;
    if (tex != NULL) {
        int fmt = tcw >> 27 & 7;
        s->palette = (uint16_t)(fmt == 5 ? (tcw >> 21 & 63) * 16 : fmt == 6 ? (tcw >> 25 & 3) * 256 : 0);
    }
}

typedef struct {
    float m[16];        /* model to screen, ftrv's order */
    NLV3 l;             /* the light, in the model's space */
    int lit;
    int vtxcol;         /* the vertices carry their colours */
    float ambient;
    float face[4], offs[4];     /* ARGB, 0 to 1 */
    RD_STATE state;
} GROUP;

/* the record at *pp: its position's record, and *pp moved past it */
static const uint8_t* record(const uint8_t** pp) {
    const uint8_t* p = *pp;
    if (word(p) & 1) {
        *pp = p + 32;
        return p;
    }
    *pp = p + 8;
    return p + 8 + (int32_t)word(p + 4);
}

static void transform(const GROUP* g, const uint8_t* r, OVTX* o) {
    float x = flt(r), y = flt(r + 4), z = flt(r + 8);
    const float* m = g->m;
    o->w = x * m[0] + y * m[4] + z * m[8] + m[12];
    o->x = x * m[1] + y * m[5] + z * m[9] + m[13];
    o->y = x * m[2] + y * m[6] + z * m[10] + m[14];
    o->u = flt(r + 24);
    o->v = flt(r + 28);
    if (g->vtxcol) {
        uint32_t c = word(r + 16);
        o->c[0] = g->face[0] * (c >> 24) / 255.0f;
        o->c[1] = g->face[1] * (c >> 16 & 255) / 255.0f;
        o->c[2] = g->face[2] * (c >> 8 & 255) / 255.0f;
        o->c[3] = g->face[3] * (c & 255) / 255.0f;
    } else {
        float i = 1.0f;
        if (g->lit) {
            float d = flt(r + 12) * g->l.x + flt(r + 16) * g->l.y + flt(r + 20) * g->l.z;
            i = g->ambient + (d > 0.0f ? d : 0.0f);
        }
        o->c[0] = g->face[0];
        o->c[1] = g->face[1] * i;
        o->c[2] = g->face[2] * i;
        o->c[3] = g->face[3] * i;
    }
}

static void screen(const OVTX* o, RD_VTX* d) {
    float rw = 1.0f / o->w;
    d->x = o->x * rw;
    d->y = o->y * rw;
    d->z = rw;
    d->u = o->u;
    d->v = o->v;
    d->base = nl_float_argb(o->c[0], o->c[1], o->c[2], o->c[3]);
    d->offset = 0;
}

static OVTX lerp(const OVTX* a, const OVTX* b) {
    float t = (near_w - a->w) / (b->w - a->w);
    OVTX o;
    int k;
    o.w = near_w;
    o.x = a->x + (b->x - a->x) * t;
    o.y = a->y + (b->y - a->y) * t;
    o.u = a->u + (b->u - a->u) * t;
    o.v = a->v + (b->v - a->v) * t;
    for (k = 0; k < 4; k++) {
        o.c[k] = a->c[k] + (b->c[k] - a->c[k]) * t;
    }
    return o;
}

/* one triangle, cut at the near plane (w = znear) */
static void triangle(const GROUP* g, const OVTX* a, const OVTX* b, const OVTX* c) {
    if (a->w >= near_w && b->w >= near_w && c->w >= near_w) {
        RD_VTX v[3];
        screen(a, &v[0]);
        screen(b, &v[1]);
        screen(c, &v[2]);
        rd_strip(&g->state, v, 3);
        return;
    }
    const OVTX* in[3] = { a, b, c };
    OVTX poly[4];
    RD_VTX v[4];
    int n = 0, k;
    for (k = 0; k < 3; k++) {
        const OVTX *p = in[k], *q = in[(k + 1) % 3];
        if (p->w >= near_w) {
            poly[n++] = *p;
        }
        if ((p->w >= near_w) != (q->w >= near_w)) {
            poly[n++] = lerp(p, q);
        }
    }
    if (n < 3) {
        return;
    }
    for (k = 0; k < n; k++) {
        screen(&poly[k], &v[k]);
    }
    if (n == 3) {
        rd_strip(&g->state, v, 3);
    } else {
        RD_VTX s[4] = { v[0], v[1], v[3], v[2] };
        rd_strip(&g->state, s, 4);
    }
}

/* a primitive at p (its header word read): the data after it */
static const uint8_t* primitive(GROUP* g, uint32_t head, const uint8_t* p) {
    int n = (int)word(p), k, front = 1;
    p += 4;
    g->state.cull = (uint8_t)(head & 3);
    g->state.flat = !(head & 64);
    if (head & 8) {
        while (n-- > 0) {
            for (k = 0; k < 3; k++) {
                transform(g, record(&p), &ov[k]);
            }
            triangle(g, &ov[0], &ov[1], &ov[2]);
        }
        return p;
    }
    if (n > MAX_VTX) {
        n = MAX_VTX;
    }
    for (k = 0; k < n; k++) {
        transform(g, record(&p), &ov[k]);
        front &= ov[k].w >= near_w;
    }
    if (front && n >= 3) {
        /* all in front: each vertex to the screen once */
        for (k = 0; k < n; k++) {
            screen(&ov[k], &sv[k]);
        }
        rd_strip(&g->state, sv, n);
        return p;
    }
    for (k = 2; k < n; k++) {
        if (k & 1) {
            triangle(g, &ov[k - 1], &ov[k - 2], &ov[k]);
        } else {
            triangle(g, &ov[k - 2], &ov[k - 1], &ov[k]);
        }
    }
    return p;
}

/* the light in the model's space: the transpose of the model matrix's turn */
static NLV3 model_light(const float* m) {
    NLV3 l;
    l.x = light.x * m[0] + light.y * m[1] + light.z * m[2];
    l.y = light.x * m[4] + light.y * m[5] + light.z * m[6];
    l.z = light.x * m[8] + light.y * m[9] + light.z * m[10];
    return l;
}

static void draw(const int* obj, const float* model, const float* toscreen, float trnsl) {
    const uint8_t* p = (const uint8_t*)obj + 24;
    GROUP g;
    if (obj == NULL || (obj[0] != 0 && obj[0] != 1)) {
        return;
    }
    mul(g.m, model, toscreen);
    g.l = model_light(model);
    while (word(p) != 0) {
        uint32_t pcw = word(p), isp = word(p + 4), tsp = word(p + 8), tcw = word(p + 12);
        int texno = (int32_t)word(p + 32), mode = (int8_t)p[36], k;
        const uint8_t *prim = p + 80, *end = prim + word(p + 76);
        if (trnsl < 1.0f) {
            pcw = (pcw | 0x02000000) & ~0x04000000u;
            tsp = ((tsp | 0x001000C0) & 0x03FFFFFF) | (uint32_t)((0xFC ^ trnsl_blend) & 0xFC) << 24;
        }
        pvr_state(&g.state, pcw, isp, tsp, tcw, group_texture(pcw, tcw, texno));
        if (obj_log) {
            host_log("  group pcw %08X isp %08X tsp %08X tcw %08X tex %d mode %d", pcw, isp, tsp, tcw, texno, mode);
        }
        g.lit = mode >= 0 && !(obj[1] & 2);
        g.vtxcol = mode == -3;
        g.ambient = flt(p + 40) * global_ambience;
        for (k = 0; k < 4; k++) {
            g.face[k] = flt(p + 44 + k * 4);
            g.offs[k] = flt(p + 60 + k * 4);
        }
        if ((pcw >> 4 & 3) != 2 || g.vtxcol) {
            g.face[0] = g.face[1] = g.face[2] = g.face[3] = 1.0f;
        }
        g.face[0] *= trnsl;
        g.face[1] *= nl_fade_r;
        g.face[2] *= nl_fade_g;
        g.face[3] *= nl_fade_b;
        while (prim < end) {
            uint32_t head = word(prim);
            if ((int32_t)head <= 0) {
                break;
            }
            prim = primitive(&g, head, prim + 4);
        }
        p = end;
    }
}

/* MVC2_OBJLOG=frame: the models drawn in that frame, and their groups' polygon headers */
static int obj_log_frame(int* logged) {
    const char* e = getenv("MVC2_OBJLOG");
    return e != NULL && host_frame() == (unsigned)atoi(e) && (*logged)++ < 200;
}

static void put(const int* obj, float trnsl, int use_view) {
    float to_screen[16], zneg;
    static int logged;

    memcpy(&zneg, nl_objput_param + 0x0c, 4);
    near_w = -zneg > 0.0f ? -zneg : 1e-3f;     /* before any projection is set */
    if (use_view) {
        mul(to_screen, nl_view_matrix(), nl_cnv_proj_pers);
    } else {
        memcpy(to_screen, nl_cnv_proj_pers, sizeof to_screen);
    }
    if (obj_log_frame(&logged)) {
        host_log("obj %p view %d trnsl %g", (const void*)obj, use_view, trnsl);
        obj_log = 1;
    }
    draw(obj, nl_xmtrx(), to_screen, trnsl);
    obj_log = 0;
}

void nlObjPut(int* obj) {
    put(obj, 1.0f, 0);
}

void nlObjPutUseViewMatrix(int* obj) {
    put(obj, 1.0f, 1);
}

void nlObjPutTrnsl(int* obj, float trs) {
    put(obj, trs, 0);
}

void nlObjPutUseViewMatrixTrnsl(int* obj, float trs) {
    put(obj, trs, 1);
}

/* nlobjcem.src: a flagged group's (head bit 8) texture coordinates from its normals turned by the
   matrix; lighting byte -2 left alone, below -2 normals as bytes (z, y, x at 12) */
void nlObjPutCheapEnvMap(int* obj) {
    const float* m = nl_xmtrx();
    uint8_t* p;
    if (obj == NULL || obj[0] < 0) {
        return;
    }
    for (p = (uint8_t*)obj + 24; word(p) != 0; p += 80 + word(p + 76)) {
        int mode = (int8_t)p[36];
        uint8_t *prim = p + 80, *end = prim + word(p + 76);
        if (mode == -2 || !(word(prim) & 0x100)) {
            continue;
        }
        while (prim < end && (int32_t)word(prim) > 0) {
            uint32_t head = word(prim);
            int n = (int)word(prim + 4) * (head & 8 ? 3 : 1);
            prim += 8;
            while (n-- > 0) {
                if (word(prim) & 1) {
                    float nx, ny, nz, u, v, scale = mode < -2 ? 1.0f / 256.0f : 0.5f;
                    uint32_t vw;
                    if (mode < -2) {
                        nz = (int8_t)prim[12], ny = (int8_t)prim[13], nx = (int8_t)prim[14];
                    } else {
                        nx = flt(prim + 12), ny = flt(prim + 16), nz = flt(prim + 20);
                    }
                    u = 0.5f + scale * (m[0] * nx + m[4] * ny + m[8] * nz);
                    v = 0.5f + scale * (m[1] * nx + m[5] * ny + m[9] * nz);
                    memcpy(&vw, &v, 4);
                    vw = (vw & ~1u) | (word(prim + 28) & 1);
                    memcpy(prim + 24, &u, 4);
                    memcpy(prim + 28, &vw, 4);
                    prim += 32;
                } else {
                    prim += 8;
                }
            }
        }
    }
}

void nlObjPutInit(void) {
}

void nlObjPutSetGlobalAmbience(float amb) {
    global_ambience = amb;
}

void nlObjPutSetLightDirectionV(const NLV3* v) {
    light = *v;
}

void nlObjPutSetTrnslMode(int src, int dst) {
    trnsl_blend = (~src & 7) << 5 | (~dst & 7) << 2;
}

void nlObjPutInitTexture(void) {
}

/* each group's texture into the common textures from the offset, its index kept in the group's texture word */
int nlObjPutLoadTexture(int* obj, const NLthd* thd) {
    uint8_t* p;
    if (obj == NULL || obj[0] < 0) {
        return 0;
    }
    for (p = (uint8_t*)obj + 24; word(p) != 0; p += 80 + word(p + 76)) {
        int texno = (int32_t)word(p + 32);
        uint32_t tcw = word(p + 12);
        if (texno >= 0) {
            int index = nl_tex_offset + texno;
            if (index >= 0 && index < NL_MAX_TEX) {
                nl_tex_load(&nl_tex[index], &thd[texno]);
                tcw = (tcw & ~0x1FFFFFu) | (uint32_t)index;
            }
        }
        if ((tcw >> 27 & 7) == 5 || (tcw >> 27 & 7) == 6) {
            tcw = (tcw & 0xF81FFFFF) | (word(p + 60) & 63) << 21;
        }
        memcpy(p + 12, &tcw, 4);
    }
    return 0;
}

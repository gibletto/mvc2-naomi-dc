/*
 * rd_soft.c  The frame drawn in software, as the PowerVR2 draws it. The translucent list is sorted
 * per pixel (auto-sort mode): fragments blended far to near, at one depth in the order sent.
 */

/* SSE, not the 387, whose wider registers round wherever the compiler spills them */
#if defined(__GNUC__) && defined(__i386__)
#pragma GCC target("sse2", "fpmath=sse")
#endif
/* and no fused multiply-add */
#if defined(__clang__)
#pragma STDC FP_CONTRACT OFF
#elif defined(__GNUC__)
#pragma GCC optimize("fp-contract=off")
#endif

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rd_sys.h"
/* SSE2 and NEON give the same pixels as the plain C (RD_PLAIN) */
#if defined(__SSE2__) && !defined(RD_PLAIN)
#define RD_SSE2 1
#include <emmintrin.h>
#elif (defined(__ARM_NEON) || defined(__ARM_NEON__)) && !defined(RD_PLAIN)
#include <arm_neon.h>
#define RD_NEON 1
#endif

#include "rd.h"
#include "rd_list.h"

#if defined(__GNUC__)
#define PIXEL_STEP static inline __attribute__((always_inline))
#else
#define PIXEL_STEP static inline
#endif

/* rd_q: half draws at half size, each pixel shown as four; fast takes the nearest texel */
static int scale = 1, draw_w = RD_WIDTH, draw_h = RD_HEIGHT, fast, nofog, nosort;

static float zbuf[RD_WIDTH * RD_HEIGHT];
static uint32_t cbuf[RD_WIDTH * RD_HEIGHT];     /* ARGB, the alpha kept for the blend factors */

/* the translucent fragments, a list a pixel */
typedef struct {
    float z;
    uint32_t color;
    int order;          /* in the list */
    int next;
} FRAG;

static int fhead[RD_WIDTH * RD_HEIGHT];

typedef struct WORKER WORKER;

typedef struct {
    int a, r, g, b;
} COLOR;

static COLOR unpack(uint32_t c) {
    COLOR k;
    k.a = (int)(c >> 24);
    k.r = (int)(c >> 16 & 255);
    k.g = (int)(c >> 8 & 255);
    k.b = (int)(c & 255);
    return k;
}

static uint32_t pack(COLOR c) {
    return (uint32_t)c.a << 24 | (uint32_t)c.r << 16 | (uint32_t)c.g << 8 | (uint32_t)c.b;
}

static int depth_pass(int mode, float z, float old) {
    switch (mode) {
    case RD_NEVER: return 0;
    case RD_LESS: return z < old;
    case RD_EQUAL: return z == old;
    case RD_LEQUAL: return z <= old;
    case RD_GREATER: return z > old;
    case RD_NOTEQUAL: return z != old;
    case RD_GEQUAL: return z >= old;
    default: return 1;
    }
}

/* clamped, mirrored or repeated, as the TSP says */
static int wrap(int x, int n, int clamp, int flip) {
    if (clamp) {
        return x < 0 ? 0 : x >= n ? n - 1 : x;
    }
    if (flip) {
        x &= 2 * n - 1;
        return x < n ? x : 2 * n - 1 - x;
    }
    return x & (n - 1);
}

/* (int)f, INT_MIN out of range as on x86, so every CPU draws the same pixels */
#if defined(__i386__) || defined(__x86_64__) || defined(_M_IX86) || defined(_M_X64)
#define to_int(f) ((int)(f))
#else
static int to_int(float f) {
    return f >= -2147483648.0f && f < 2147483648.0f ? (int)f : INT_MIN;
}
#endif

/* (int)floorf(f) without the call */
static int ifloor(float f) {
    int i = to_int(f);
    return f < (float)i && i != INT_MIN ? i - 1 : i;
}

static uint32_t argb16[3][65536];

static void make_tables(void) {
    uint32_t p;
    for (p = 0; p < 65536; p++) {
        argb16[RD_ARGB1555][p] = (p & 0x8000 ? 0xFF000000u : 0) | (p >> 10 & 31) * 255 / 31 << 16 |
                                 (p >> 5 & 31) * 255 / 31 << 8 | (p & 31) * 255 / 31;
        argb16[RD_RGB565][p] = 0xFF000000u | (p >> 11) * 255 / 31 << 16 | (p >> 5 & 63) * 255 / 63 << 8 | (p & 31) * 255 / 31;
        argb16[RD_ARGB4444][p] = (p >> 12) * 0x11000000u | (p >> 8 & 15) * 0x110000u | (p >> 4 & 15) * 0x1100u |
                                 (p & 15) * 0x11u;
    }
}

static uint32_t fetch(const RD_STATE* s, const void* d, int at) {
    if (s->tex->format == RD_PAL4 || s->tex->format == RD_PAL8) {
        return rd_palette[(s->palette + ((const uint8_t*)d)[at]) & 1023];
    }
    return argb16[s->tex->format][((const uint16_t*)d)[at]];
}

static uint32_t texel(const RD_STATE* s, int k, int x, int y) {
    const RD_TEXTURE* t = s->tex;
    int w = t->width >> k, h = t->height >> k;
    if (w < 1) w = 1;
    if (h < 1) h = 1;
    return fetch(s, k == 0 ? t->texels : t->mip[k],
                 wrap(y, h, s->uv_mode & 1, s->uv_mode & 4) * w + wrap(x, w, s->uv_mode & 2, s->uv_mode & 8));
}

/* the four texels last blended: the next pixel often wants the same */
typedef struct {
    const RD_STATE* s;
    int k, x, y;
    _Alignas(16) float c[4][4];     /* (x, y), (x + 1, y), (x, y + 1), (x + 1, y + 1); a r g b */
} TEXCACHE;

static void texel_floats(float* c, uint32_t q) {
#if defined(RD_SSE2)
    __m128i z = _mm_setzero_si128(), k = _mm_unpacklo_epi16(_mm_unpacklo_epi8(_mm_cvtsi32_si128((int)q), z), z);
    _mm_store_ps(c, _mm_cvtepi32_ps(_mm_shuffle_epi32(k, _MM_SHUFFLE(0, 1, 2, 3))));
#elif defined(RD_NEON)
    uint32x4_t k = vmovl_u16(vget_low_u16(vmovl_u8(vreinterpret_u8_u32(vdup_n_u32(q)))));
    k = vrev64q_u32(k);
    vst1q_f32(c, vcvtq_f32_u32(vcombine_u32(vget_high_u32(k), vget_low_u32(k))));
#else
    c[0] = (float)(int)(q >> 24), c[1] = (float)(int)(q >> 16 & 255);
    c[2] = (float)(int)(q >> 8 & 255), c[3] = (float)(int)(q & 255);
#endif
}

PIXEL_STEP COLOR sample(const RD_STATE* s, int k, float u, float v, TEXCACHE* tc) {
    const RD_TEXTURE* t = s->tex;
    const void* d = k == 0 ? t->texels : t->mip[k];
    int w = t->width >> k, h = t->height >> k;
    int cu = s->uv_mode & 2, cv = s->uv_mode & 1, fu_ = s->uv_mode & 8, fv_ = s->uv_mode & 4;
    float fu, fv;
    if (w < 1) w = 1;
    if (h < 1) h = 1;
    fu = u * w, fv = v * h;
    if (!s->bilinear || fast) {
        return unpack(fetch(s, d, wrap(ifloor(fv), h, cv, fv_) * w + wrap(ifloor(fu), w, cu, fu_)));
    } else {
        int x = ifloor(fu - 0.5f), y = ifloor(fv - 0.5f);
        float ax = fu - 0.5f - x, ay = fv - 0.5f - y;
        COLOR c;
        if (tc->s != s || tc->k != k || tc->x != x || tc->y != y) {
            int x0 = wrap(x, w, cu, fu_), x1 = wrap(x + 1, w, cu, fu_);
            int y0 = wrap(y, h, cv, fv_) * w, y1 = wrap(y + 1, h, cv, fv_) * w;
            if (tc->s == s && tc->k == k && tc->x + 1 == x && tc->y == y) {
                /* one texel on: the right two become the left two */
                memcpy(tc->c[0], tc->c[1], sizeof tc->c[0]);
                memcpy(tc->c[2], tc->c[3], sizeof tc->c[2]);
                texel_floats(tc->c[1], fetch(s, d, y0 + x1));
                texel_floats(tc->c[3], fetch(s, d, y1 + x1));
            } else {
                texel_floats(tc->c[0], fetch(s, d, y0 + x0));
                texel_floats(tc->c[1], fetch(s, d, y0 + x1));
                texel_floats(tc->c[2], fetch(s, d, y1 + x0));
                texel_floats(tc->c[3], fetch(s, d, y1 + x1));
            }
            tc->s = s, tc->k = k, tc->x = x, tc->y = y;
        }
#if defined(RD_SSE2)
        {
            __m128 bx = _mm_set1_ps(1 - ax), fx = _mm_set1_ps(ax), by = _mm_set1_ps(1 - ay), fy = _mm_set1_ps(ay);
            __m128 top = _mm_add_ps(_mm_mul_ps(_mm_load_ps(tc->c[0]), bx), _mm_mul_ps(_mm_load_ps(tc->c[1]), fx));
            __m128 bottom = _mm_add_ps(_mm_mul_ps(_mm_load_ps(tc->c[2]), bx), _mm_mul_ps(_mm_load_ps(tc->c[3]), fx));
            __m128i k = _mm_cvttps_epi32(_mm_add_ps(_mm_mul_ps(top, by), _mm_mul_ps(bottom, fy)));
            c.a = _mm_cvtsi128_si32(k);
            c.r = _mm_cvtsi128_si32(_mm_srli_si128(k, 4));
            c.g = _mm_cvtsi128_si32(_mm_srli_si128(k, 8));
            c.b = _mm_cvtsi128_si32(_mm_srli_si128(k, 12));
        }
#elif defined(RD_NEON)
        {
            /* with to_int's INT_MIN */
            float32x4_t bx = vdupq_n_f32(1 - ax), fx = vdupq_n_f32(ax), by = vdupq_n_f32(1 - ay), fy = vdupq_n_f32(ay);
            float32x4_t top = vaddq_f32(vmulq_f32(vld1q_f32(tc->c[0]), bx), vmulq_f32(vld1q_f32(tc->c[1]), fx));
            float32x4_t bottom = vaddq_f32(vmulq_f32(vld1q_f32(tc->c[2]), bx), vmulq_f32(vld1q_f32(tc->c[3]), fx));
            float32x4_t m = vaddq_f32(vmulq_f32(top, by), vmulq_f32(bottom, fy));
            int32x4_t k = vbslq_s32(vcaltq_f32(m, vdupq_n_f32(2147483648.0f)), vcvtq_s32_f32(m), vdupq_n_s32(INT_MIN));
            c.a = vgetq_lane_s32(k, 0);
            c.r = vgetq_lane_s32(k, 1);
            c.g = vgetq_lane_s32(k, 2);
            c.b = vgetq_lane_s32(k, 3);
        }
#else
#define MIX(j) to_int((tc->c[0][j] * (1 - ax) + tc->c[1][j] * ax) * (1 - ay) + (tc->c[2][j] * (1 - ax) + tc->c[3][j] * ax) * ay)
        c.a = MIX(0);
        c.r = MIX(1);
        c.g = MIX(2);
        c.b = MIX(3);
#undef MIX
#endif
        return c;
    }
}

/* the mip level nearest the larger texel step, moved by the D adjust */
static int mip_level(const RD_STATE* s, float dudx, float dvdx, float dudy, float dvdy) {
    static const float bias[16] = { 0, -4, -2, -1 };
    float w = (float)s->tex->width, h = (float)s->tex->height;
    float lx = dudx * dudx * w * w + dvdx * dvdx * h * h, ly = dudy * dudy * w * w + dvdy * dvdy * h * h;
    float l = lx > ly ? lx : ly, lod;
    uint32_t bits;
    int k;
    memcpy(&bits, &l, 4);
    if ((bits >> 23) - 1 < 254 && (bits & 0x7FFFFF) > 0x800 && (bits & 0x7FFFFF) < 0x7FF800) {
        /* log2 l well inside (e, e + 1): no log2f needed */
        int e = (int)(bits >> 23) - 127;
        k = ((e + 1) >> 1) + (int)bias[s->mip_d & 15];
    } else {
        lod = 0.5f * log2f(l) + bias[s->mip_d & 15];
        k = to_int(floorf(lod + 0.5f));
    }
    return k < 0 ? 0 : k > s->tex->levels ? s->tex->levels : k;
}

static int clamp255(int x) {
    return x < 0 ? 0 : x > 255 ? 255 : x;
}

PIXEL_STEP COLOR shade(const RD_STATE* s, COLOR base, COLOR offs, int level, float u, float v, TEXCACHE* tc) {
    COLOR c;
    if (!s->use_alpha) {
        base.a = 255;       /* the texture's alpha still counts */
    }
    c = base;
    if (s->tex != NULL) {
        COLOR t = sample(s, level, u, v, tc);
        if (!s->tex_alpha) {
            t.a = 255;
        }
        switch (s->shading) {
        case RD_DECAL:
            c = t;
            break;
        case RD_MODULATE:
            c.r = t.r * base.r / 255;
            c.g = t.g * base.g / 255;
            c.b = t.b * base.b / 255;
            c.a = t.a;
            break;
        case RD_DECAL_ALPHA:
            c.r = (t.r * t.a + base.r * (255 - t.a)) / 255;
            c.g = (t.g * t.a + base.g * (255 - t.a)) / 255;
            c.b = (t.b * t.a + base.b * (255 - t.a)) / 255;
            c.a = base.a;
            break;
        default:
            c.r = t.r * base.r / 255;
            c.g = t.g * base.g / 255;
            c.b = t.b * base.b / 255;
            c.a = t.a * base.a / 255;
            break;
        }
    }
    if (s->offset) {
        c.r = clamp255(c.r + offs.r);
        c.g = clamp255(c.g + offs.g);
        c.b = clamp255(c.b + offs.b);
    }
    return c;
}

/* the fog table at 1/w z: 16 entries a doubling of density * z, the two either side blended */
static float fog_value(float z) {
    float q = rd_fog.density * z, m, pos;
    uint32_t bits;
    int e, i;
    if (!(q >= 1.0f)) {
        q = 1.0f;
    } else if (q > 255.9999f) {
        q = 255.9999f;
    }
    memcpy(&bits, &q, 4);       /* q = m 2^e, m in [0.5, 1) */
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

static int fog_on;      /* the table is not all 0 */

static void fog(const RD_STATE* s, COLOR* c, float z) {
    float f = fog_value(z);
    COLOR k = unpack(rd_fog.color);
    if (s->fog == RD_FOG_TABLE2) {
        k.a = (int)(f * 255.0f + 0.5f);
        *c = k;
        return;
    }
    if (f == 0.0f) {
        return;
    }
    if (f == 1.0f) {
        c->r = k.r, c->g = k.g, c->b = k.b;
        return;
    }
    c->r += (int)((k.r - c->r) * f + (k.r > c->r ? 0.5f : -0.5f));
    c->g += (int)((k.g - c->g) * f + (k.g > c->g ? 0.5f : -0.5f));
    c->b += (int)((k.b - c->b) * f + (k.b > c->b ? 0.5f : -0.5f));
}

/* blend factor f, 0 to 255, for a r g b */
static void factors(int f, const int* other, int sa, int da, int* k) {
    int i;
    switch (f) {
    case RD_ZERO: k[0] = k[1] = k[2] = k[3] = 0; break;
    case RD_ONE: k[0] = k[1] = k[2] = k[3] = 255; break;
    case RD_OTHER: for (i = 0; i < 4; i++) k[i] = other[i]; break;
    case RD_INV_OTHER: for (i = 0; i < 4; i++) k[i] = 255 - other[i]; break;
    case RD_SRC_ALPHA: k[0] = k[1] = k[2] = k[3] = sa; break;
    case RD_INV_SRC_ALPHA: k[0] = k[1] = k[2] = k[3] = 255 - sa; break;
    case RD_DST_ALPHA: k[0] = k[1] = k[2] = k[3] = da; break;
    default: k[0] = k[1] = k[2] = k[3] = 255 - da; break;
    }
}

PIXEL_STEP uint32_t blend(const RD_STATE* s, COLOR c, uint32_t dst) {
    int sc[4], dc[4], fs[4], fd[4], i;
    uint32_t out = 0;
    if (s->src == RD_SRC_ALPHA && s->dst == RD_INV_SRC_ALPHA) {
        int a = c.a, na = 255 - a;
        return (uint32_t)((a * a + (int)(dst >> 24) * na + 127) / 255) << 24 |
               (uint32_t)((c.r * a + (int)(dst >> 16 & 255) * na + 127) / 255) << 16 |
               (uint32_t)((c.g * a + (int)(dst >> 8 & 255) * na + 127) / 255) << 8 |
               (uint32_t)((c.b * a + (int)(dst & 255) * na + 127) / 255);
    }
    if (s->src == RD_SRC_ALPHA && s->dst == RD_ONE) {
        int a = c.a;
        return (uint32_t)clamp255((int)(dst >> 24) + (a * a + 127) / 255) << 24 |
               (uint32_t)clamp255((int)(dst >> 16 & 255) + (c.r * a + 127) / 255) << 16 |
               (uint32_t)clamp255((int)(dst >> 8 & 255) + (c.g * a + 127) / 255) << 8 |
               (uint32_t)clamp255((int)(dst & 255) + (c.b * a + 127) / 255);
    }
    sc[0] = c.a, sc[1] = c.r, sc[2] = c.g, sc[3] = c.b;
    dc[0] = (int)(dst >> 24), dc[1] = (int)(dst >> 16 & 255), dc[2] = (int)(dst >> 8 & 255), dc[3] = (int)(dst & 255);
    factors(s->src, dc, c.a, dc[0], fs);
    factors(s->dst, sc, c.a, dc[0], fd);
    for (i = 0; i < 4; i++) {
        out = out << 8 | (uint32_t)clamp255((sc[i] * fs[i] + dc[i] * fd[i] + 127) / 255);
    }
    return out;
}

/* doubles: a vertex cut at the near plane can be far off the screen */
static double edge(const RD_VTX* a, const RD_VTX* b, double x, double y) {
    return ((double)b->x - a->x) * (y - a->y) - ((double)b->y - a->y) * (x - a->x);
}

/* the top-left rule: two triangles that share an edge draw each pixel on it once */
static int edge_owns(const RD_VTX* a, const RD_VTX* b, double sign) {
    double dx = -((double)b->y - a->y) * sign, dy = ((double)b->x - a->x) * sign;
    return dx > 0 || (dx == 0 && dy > 0);
}

/* rows in bands of 8, dealt out to the workers in turn: the same frame for any count */
#define BAND 8
#define MAX_WORKERS 32

struct WORKER {
    _Alignas(64) int index;     /* a cache line each */
    TEXCACHE tex;
    FRAG* frags;
    int nfrags, maxfrags;
    RD_SEM* go;
};

static WORKER workers[MAX_WORKERS];
static int nworkers;
static RD_SEM* done;
static int phase, ntotal;
static uint32_t* out_frame;
static int out_pitch;

static int first_row(const WORKER* w, int y) {
    int band = y / BAND, skip = ((w->index - band) % nworkers + nworkers) % nworkers;
    return skip == 0 ? y : (band + skip) * BAND;
}

static int next_row(const WORKER* w, int y) {
    return (y + 1) % BAND != 0 ? y + 1 : y + 1 + (nworkers - 1) * BAND;
}

static void add_frag(WORKER* w, int at, float z, uint32_t color, int order) {
    FRAG* f;
    if (w->nfrags == w->maxfrags) {
        int n = w->maxfrags ? w->maxfrags * 2 : 1 << 16;
        FRAG* p = realloc(w->frags, (size_t)n * sizeof *p);
        if (p == NULL) {
            return;
        }
        w->frags = p;
        w->maxfrags = n;
    }
    f = &w->frags[w->nfrags];
    f->z = z;
    f->color = color;
    f->order = order;
    f->next = fhead[at];
    fhead[at] = w->nfrags++;
}

#define PLANES 11

/* a triangle set up to draw */
typedef struct {
    const RD_STATE* s;
    const RD_TRI* t;
    RD_VTX v[3];            /* on the drawn frame */
    int list, order;
    int x0, x1, y0, y1;     /* y1 <= y0: not drawn */
    int own[3], depth, zwrite;
    int flat;               /* one colour all over */
    int level;              /* -1: chosen pixel by pixel */
    double area, sign;
    double gx[3], gy[3];    /* the barycentrics' steps */
    float zx, zy;
    float uz_x, uz_y, vz_x, vz_y;
    float col[3][8];        /* the colours over w */
    /* at + dx x + dy y: 1/w, u/w, v/w and the eight colour channels over w */
    double at[PLANES], dx[PLANES], dy[PLANES];
} SETUP;

static SETUP* setups;
static int maxsetups;

/* fmin and fmax without the calls (no NaNs here) */
static double dmin(double a, double b) {
    return a < b ? a : b;
}

static double dmax(double a, double b) {
    return a > b ? a : b;
}

/* FPU_CULL_VAL, as libnaomi sets it */
#define CULL_VAL 0.02

static void setup_tri(SETUP* p, int list, const RD_TRI* t, int order) {
    const RD_STATE* s = &rd_frame.states[t->state];
    const RD_VTX *a = &p->v[0], *b = &p->v[1], *c = &p->v[2];
    double area;
    COLOR k[6];
    int i;
    p->y0 = p->y1 = 0;
    for (i = 0; i < 3; i++) {
        p->v[i] = t->v[i];
        if (scale != 1) {
            p->v[i].x /= scale, p->v[i].y /= scale;
        }
    }
    area = edge(a, b, c->x, c->y);
    if (area == 0.0 || !(area == area) || (s->cull != 0 && fabs(area) < CULL_VAL) || (s->cull == 2 && area < 0) ||
        (s->cull == 3 && area > 0)) {
        return;
    }
    p->s = s;
    p->t = t;
    p->list = list;
    p->order = order;
    p->flat = s->flat || (a->base == b->base && a->base == c->base && a->offset == b->offset && a->offset == c->offset);
    p->area = area;
    p->sign = area > 0 ? 1.0 : -1.0;
    p->own[0] = edge_owns(b, c, p->sign);
    p->own[1] = edge_owns(c, a, p->sign);
    p->own[2] = edge_owns(a, b, p->sign);
    p->gx[0] = -((double)c->y - b->y) / area, p->gy[0] = ((double)c->x - b->x) / area;
    p->gx[1] = -((double)a->y - c->y) / area, p->gy[1] = ((double)a->x - c->x) / area;
    p->gx[2] = -((double)b->y - a->y) / area, p->gy[2] = ((double)b->x - a->x) / area;
    p->zx = (float)(p->gx[0] * a->z + p->gx[1] * b->z + p->gx[2] * c->z);
    p->zy = (float)(p->gy[0] * a->z + p->gy[1] * b->z + p->gy[2] * c->z);
    if (s->tex != NULL && s->tex->levels > 0) {
        const double *gx = p->gx, *gy = p->gy;
        p->uz_x = (float)(gx[0] * a->u * a->z + gx[1] * b->u * b->z + gx[2] * c->u * c->z);
        p->uz_y = (float)(gy[0] * a->u * a->z + gy[1] * b->u * b->z + gy[2] * c->u * c->z);
        p->vz_x = (float)(gx[0] * a->v * a->z + gx[1] * b->v * b->z + gx[2] * c->v * c->z);
        p->vz_y = (float)(gy[0] * a->v * a->z + gy[1] * b->v * b->z + gy[2] * c->v * c->z);
    }
    if (list == RD_OPAQUE) {
        p->depth = s->depth;
        p->zwrite = s->zwrite;
    } else {
        p->depth = RD_GEQUAL;      /* punch-through and auto-sorted translucent */
        p->zwrite = list == RD_PUNCH;
    }
    /* clamped before the casts: a vertex cut at the near plane can be past int's range */
    p->x0 = (int)dmin(dmax(floor(dmin(a->x, dmin(b->x, c->x))), 0.0), draw_w);
    p->x1 = (int)dmax(dmin(ceil(dmax(a->x, dmax(b->x, c->x))), draw_w), 0.0);
    p->y0 = (int)dmin(dmax(floor(dmin(a->y, dmin(b->y, c->y))), 0.0), draw_h);
    p->y1 = (int)dmax(dmin(ceil(dmax(a->y, dmax(b->y, c->y))), draw_h), 0.0);
    p->level = s->tex != NULL && s->tex->levels > 0 ? -1 : 0;
    for (i = 0; i < 3; i++) {
        float w = p->v[i].z;
        k[i] = unpack(p->v[i].base);
        k[i + 3] = unpack(p->v[i].offset);
        p->col[i][0] = k[i].a * w, p->col[i][1] = k[i].r * w, p->col[i][2] = k[i].g * w, p->col[i][3] = k[i].b * w;
        p->col[i][4] = k[i + 3].a * w, p->col[i][5] = k[i + 3].r * w, p->col[i][6] = k[i + 3].g * w;
        p->col[i][7] = k[i + 3].b * w;
    }
    {
        double w0 = edge(b, c, 0.5, 0.5) / area, w1 = edge(c, a, 0.5, 0.5) / area, w2 = edge(a, b, 0.5, 0.5) / area;
        double f[3][PLANES];
        int j;
        for (i = 0; i < 3; i++) {
            const RD_VTX* v = &p->v[i];
            f[i][0] = v->z;
            f[i][1] = (double)v->u * v->z;
            f[i][2] = (double)v->v * v->z;
            for (j = 0; j < 8; j++) {
                f[i][3 + j] = p->col[i][j];
            }
        }
        for (j = 0; j < PLANES; j++) {
            p->at[j] = w0 * f[0][j] + w1 * f[1][j] + w2 * f[2][j];
            p->dx[j] = p->gx[0] * f[0][j] + p->gx[1] * f[1][j] + p->gx[2] * f[2][j];
            p->dy[j] = p->gy[0] * f[0][j] + p->gy[1] * f[1][j] + p->gy[2] * f[2][j];
        }
    }
    if (fast && p->level < 0) {
        /* one level for the whole triangle, at its middle */
        double x = (p->x0 + p->x1) * 0.5, y = (p->y0 + p->y1) * 0.5;
        float z = (float)(p->at[0] + p->dx[0] * x + p->dy[0] * y), rz, u, v;
        p->level = 0;
        if (z > 0.0f) {
            rz = 1.0f / z;
            u = (float)((p->at[1] + p->dx[1] * x + p->dy[1] * y) * rz);
            v = (float)((p->at[2] + p->dx[2] * x + p->dy[2] * y) * rz);
            p->level = mip_level(s, (p->uz_x - u * p->zx) * rz, (p->vz_x - v * p->zx) * rz,
                                 (p->uz_y - u * p->zy) * rz, (p->vz_y - v * p->zy) * rz);
        }
    }
}

/* the exact test, with the top-left rule */
static int inside(const SETUP* p, int x, int y) {
    const RD_VTX *a = &p->v[0], *b = &p->v[1], *c = &p->v[2];
    double px = x + 0.5, py = y + 0.5;
    double e0 = edge(b, c, px, py) * p->sign, e1 = edge(c, a, px, py) * p->sign, e2 = edge(a, b, px, py) * p->sign;
    return !(e0 < 0 || e1 < 0 || e2 < 0 || (e0 == 0 && !p->own[0]) || (e1 == 0 && !p->own[1]) ||
             (e2 == 0 && !p->own[2]));
}

/* the columns of row py where edge a-b can be inside, a pixel or two wide of it */
static void edge_span(const RD_VTX* a, const RD_VTX* b, double sign, double py, double* lo, double* hi) {
    double d = ((double)b->y - a->y) * sign, e = ((double)b->x - a->x) * (py - a->y) * sign, x;
    if (d == 0) {
        if (e < 0) {
            *lo = 1e30, *hi = -1e30;
        }
        return;
    }
    x = e / d + a->x - 0.5;
    if (!(x == x)) {
        return;
    }
    if (d > 0) {
        *hi = dmin(*hi, x + 2);
    } else {
        *lo = dmax(*lo, x - 2);
    }
}

static void raster(const SETUP* p, WORKER* wk) {
    const RD_STATE* s = p->s;
    const RD_VTX *a = &p->v[0], *b = &p->v[1], *c = &p->v[2];
    double sign = p->sign;
    /* the planes the pixels read */
    int list = p->list, y, k, np = !p->flat ? (s->offset ? PLANES : 7) : s->tex != NULL ? 3 : 1;
    for (y = first_row(wk, p->y0); y < p->y1; y = next_row(wk, y)) {
        double py = y + 0.5, lo = p->x0, hi = p->x1, f[PLANES];
        int x, xs, xe;
        edge_span(b, c, sign, py, &lo, &hi);
        edge_span(c, a, sign, py, &lo, &hi);
        edge_span(a, b, sign, py, &lo, &hi);
        if (!(lo < hi)) {
            continue;
        }
        xs = lo <= p->x0 ? p->x0 : (int)lo;     /* floor: lo > x0 >= 0 */
        xe = hi >= p->x1 ? p->x1 : (int)hi + (hi > (int)hi);
        while (xs < xe && !inside(p, xs, y)) {
            xs++;
        }
        while (xe > xs && !inside(p, xe - 1, y)) {
            xe--;
        }
        for (k = 0; k < np; k++) {
            f[k] = p->at[k] + p->dx[k] * xs + p->dy[k] * y;
        }
        for (x = xs; x < xe; x++) {
            float z, rz, u = 0, v = 0, ch[8];
            int at = y * RD_WIDTH + x, level;
            COLOR base, offs, col;
            if (x > xs) {
                for (k = 0; k < np; k++) {
                    f[k] += p->dx[k];
                }
            }
            z = (float)f[0];
            if (!depth_pass(p->depth, z, zbuf[at]) || !(z > 0.0f)) {
                continue;
            }
            rz = 1.0f / z;
            if (s->tex != NULL) {
                u = (float)(f[1] * rz);
                v = (float)(f[2] * rz);
            }
            if (p->flat) {
                base = unpack(c->base);
                offs = unpack(c->offset);
                offs.a = 0;
            } else {
                for (k = 0; k < 4; k++) {
                    ch[k] = (float)(f[3 + k] * rz);
                }
                if (s->offset) {
                    for (k = 5; k < 8; k++) {
                        ch[k] = (float)(f[3 + k] * rz);
                    }
                }
                base.a = s->use_alpha ? clamp255(to_int(ch[0] + 0.5f)) : 255;
                base.r = clamp255(to_int(ch[1] + 0.5f));
                base.g = clamp255(to_int(ch[2] + 0.5f));
                base.b = clamp255(to_int(ch[3] + 0.5f));
                offs.a = 0;
                if (s->offset) {
                    offs.r = clamp255(to_int(ch[5] + 0.5f));
                    offs.g = clamp255(to_int(ch[6] + 0.5f));
                    offs.b = clamp255(to_int(ch[7] + 0.5f));
                } else {
                    offs.r = offs.g = offs.b = 0;
                }
            }
            level = p->level;
            if (level < 0) {
                level = mip_level(s, (p->uz_x - u * p->zx) * rz, (p->vz_x - v * p->zx) * rz,
                                  (p->uz_y - u * p->zy) * rz, (p->vz_y - v * p->zy) * rz);
            }
            col = shade(s, base, offs, level, u, v, &wk->tex);
            if ((s->fog == RD_FOG_TABLE && fog_on && !nofog) || s->fog == RD_FOG_TABLE2) {
                fog(s, &col, z);
            }
            if (list == RD_TRANS && !nosort) {
                add_frag(wk, at, z, pack(col), p->order);
                continue;
            }
            if (list == RD_PUNCH && col.a < 128) {
                continue;
            }
            cbuf[at] = list == RD_OPAQUE ? pack(col) : blend(s, col, cbuf[at]);
            if (p->zwrite) {
                zbuf[at] = z;
            }
        }
    }
}

#define MAX_LAYERS 256

/* MVC2_RDLOG=n: frame n's lists into rdlog.txt; MVC2_RDTEX adds the textures, MVC2_RDVTX=state
   a state's triangles, MVC2_RDPIX=x,y a pixel's translucent fragments */
static FILE* dbg_log;
static int dbg_pix = -1;

static void dump_texture(const RD_STATE* s, int state) {
    char name[32];
    FILE* p;
    int x, y, alpha;
    for (alpha = 0; alpha < 2; alpha++) {
        snprintf(name, sizeof name, alpha ? "tex_%d_a.pgm" : "tex_%d.ppm", state);
        if ((p = fopen(name, "wb")) == NULL) {
            continue;
        }
        fprintf(p, "P%d %d %d 255\n", alpha ? 5 : 6, s->tex->width, s->tex->height);
        for (y = 0; y < s->tex->height; y++) {
            for (x = 0; x < s->tex->width; x++) {
                uint32_t c = texel(s, 0, x, y);
                if (alpha) {
                    fputc((int)(c >> 24), p);
                } else {
                    fputc((int)(c >> 16 & 255), p), fputc((int)(c >> 8 & 255), p), fputc((int)(c & 255), p);
                }
            }
        }
        fclose(p);
    }
}

static int texel_bytes(const RD_TEXTURE* t) {
    return t->format == RD_PAL4 || t->format == RD_PAL8 ? 1 : 2;
}

/* MVC2_RDSAVE=n,m,...: those frames into rdframe_N.bin, for port/tools (rd_frame_file.c) */
static void save_frame(void) {
    static unsigned count;
    const char* e = getenv("MVC2_RDSAVE");
    const RD_TEXTURE** tex;
    int ntex = 0, i, j, k;
    FILE* f;
    char name[32];
    count++;
    while (e != NULL && (unsigned)atoi(e) != count) {
        e = strchr(e, ',');
        e = e != NULL ? e + 1 : NULL;
    }
    snprintf(name, sizeof name, "rdframe_%u.bin", count);
    if (e == NULL || (f = fopen(name, "wb")) == NULL) {
        return;
    }
    tex = malloc(sizeof *tex * (size_t)(rd_frame.nstates + 1));
    fwrite("RDF1", 1, 4, f);
    fwrite(rd_frame.ntris, sizeof(int), RD_LISTS, f);
    fwrite(&rd_frame.nstates, sizeof(int), 1, f);
    for (i = 0; i < RD_LISTS; i++) {
        fwrite(rd_frame.tris[i], sizeof(RD_TRI), (size_t)rd_frame.ntris[i], f);
    }
    fwrite(rd_frame.background, sizeof rd_frame.background, 1, f);
    fwrite(&rd_fog, sizeof rd_fog, 1, f);
    fwrite(rd_palette, sizeof rd_palette, 1, f);
    for (i = 0; i < rd_frame.nstates; i++) {
        const RD_STATE* s = &rd_frame.states[i];
        int n = -1;
        for (j = 0; s->tex != NULL && j < ntex; j++) {
            if (tex[j] == s->tex) {
                n = j;
            }
        }
        if (s->tex != NULL && n < 0 && tex != NULL) {
            tex[ntex] = s->tex;
            n = ntex++;
        }
        fwrite(s, sizeof *s, 1, f);
        fwrite(&n, sizeof n, 1, f);
    }
    fwrite(&ntex, sizeof ntex, 1, f);
    for (i = 0; i < ntex; i++) {
        const RD_TEXTURE* t = tex[i];
        fwrite(&t->width, sizeof(int), 1, f);
        fwrite(&t->height, sizeof(int), 1, f);
        fwrite(&t->format, sizeof(int), 1, f);
        fwrite(&t->levels, sizeof(int), 1, f);
        for (k = 0; k <= t->levels; k++) {
            int w = t->width >> k, h = t->height >> k;
            fwrite(k == 0 ? t->texels : t->mip[k], (size_t)texel_bytes(t), (size_t)(w < 1 ? 1 : w) * (h < 1 ? 1 : h), f);
        }
    }
    free(tex);
    fclose(f);
}

static void log_frame(void) {
    static unsigned count;
    const char* e = getenv("MVC2_RDLOG");
    int list, i, px, py;
    count++;
    dbg_pix = -1;
    if (e == NULL || (unsigned)atoi(e) != count || (dbg_log = fopen("rdlog.txt", "w")) == NULL) {
        return;
    }
    fprintf(dbg_log, "fog colour %08X density %g table", rd_fog.color, rd_fog.density);
    for (i = 0; i < 128; i++) {
        fprintf(dbg_log, " %.2f", rd_fog.table[i]);
    }
    fprintf(dbg_log, "\nbackground %08X %08X %08X z %g\n", rd_frame.background[0].base, rd_frame.background[1].base,
            rd_frame.background[2].base, rd_frame.background[0].z);
    e = getenv("MVC2_RDPIX");
    if (e != NULL && sscanf(e, "%d,%d", &px, &py) == 2) {
        dbg_pix = py * RD_WIDTH + px;
    }
    for (list = 0; list < RD_LISTS; list++) {
        for (i = 0; i < rd_frame.ntris[list];) {
            const RD_TRI* t = &rd_frame.tris[list][i];
            const RD_STATE* s = &rd_frame.states[t->state];
            float x0 = 1e9f, y0 = 1e9f, x1 = -1e9f, y1 = -1e9f, z0 = 1e9f, z1 = -1e9f;
            int j = i, k;
            for (; j < rd_frame.ntris[list] && rd_frame.tris[list][j].state == t->state; j++) {
                for (k = 0; k < 3; k++) {
                    const RD_VTX* v = &rd_frame.tris[list][j].v[k];
                    x0 = fminf(x0, v->x), x1 = fmaxf(x1, v->x), y0 = fminf(y0, v->y), y1 = fmaxf(y1, v->y);
                    z0 = fminf(z0, v->z), z1 = fmaxf(z1, v->z);
                }
            }
            fprintf(dbg_log, "L%d tri%d st%d n%d dep%d zw%d bl%d/%d sh%d ua%d ta%d bi%d of%d fl%d fog%d cull%d uv%d mipd%d pal%d tex%p %dx%d f%d lv%d | "
                             "%.0f,%.0f-%.0f,%.0f z %g..%g base %08X ofs %08X uv %.3f,%.3f\n",
                    list, i, t->state, j - i, s->depth, s->zwrite, s->src, s->dst, s->shading, s->use_alpha, s->tex_alpha,
                    s->bilinear, s->offset, s->flat, s->fog, s->cull, s->uv_mode, s->mip_d, s->palette, (const void*)s->tex,
                    s->tex ? s->tex->width : 0, s->tex ? s->tex->height : 0, s->tex ? s->tex->format : -1,
                    s->tex ? s->tex->levels : 0, x0, y0, x1, y1, z0, z1, t->v[0].base,
                    t->v[0].offset, t->v[0].u, t->v[0].v);
            if (s->tex != NULL && getenv("MVC2_RDTEX") != NULL) {
                dump_texture(s, t->state);
            }
            if (getenv("MVC2_RDVTX") != NULL && atoi(getenv("MVC2_RDVTX")) == t->state) {
                for (k = i; k < j; k++) {
                    const RD_VTX* v = rd_frame.tris[list][k].v;
                    fprintf(dbg_log, "  %.1f,%.1f %.3f,%.3f  %.1f,%.1f %.3f,%.3f  %.1f,%.1f %.3f,%.3f\n", v[0].x, v[0].y, v[0].u,
                            v[0].v, v[1].x, v[1].y, v[1].u, v[1].v, v[2].x, v[2].y, v[2].u, v[2].v);
                }
            }
            i = j;
        }
    }
}

/* row y's translucent fragments, far to near, into the frame */
static void resolve_row(const WORKER* w, int y) {
    int idx[MAX_LAYERS], at;
    for (at = y * RD_WIDTH; at < y * RD_WIDTH + draw_w; at++) {
        int n = 0, i, f;
        for (f = fhead[at]; f >= 0 && n < MAX_LAYERS; f = w->frags[f].next) {
            /* by depth, then order: the list runs newest first */
            const FRAG* p = &w->frags[f];
            for (i = n; i > 0; i--) {
                const FRAG* q = &w->frags[idx[i - 1]];
                if (q->z < p->z || (q->z == p->z && q->order < p->order)) {
                    break;
                }
                idx[i] = idx[i - 1];
            }
            idx[i] = f;
            n++;
        }
        for (i = 0; i < n; i++) {
            const FRAG* p = &w->frags[idx[i]];
            const RD_TRI* t = &rd_frame.tris[RD_TRANS][p->order];
            if (at == dbg_pix) {
                fprintf(dbg_log, "pixel: under %08X, z %g tri %d st %d colour %08X\n", cbuf[at], p->z, p->order, t->state,
                        p->color);
            }
            cbuf[at] = blend(&rd_frame.states[t->state], unpack(p->color), cbuf[at]);
        }
    }
}

static RD_VTX background[3];    /* on the drawn frame */

static void background_row(int y) {
    const RD_VTX *a = &background[0], *b = &background[1], *c = &background[2];
    double area = edge(a, b, c->x, c->y);
    COLOR ca = unpack(a->base), cb = unpack(b->base), cc = unpack(c->base), k;
    int x;
    if (a->base == b->base && a->base == c->base && a->z == b->z && a->z == c->z && isfinite(a->z) &&
        isfinite(a->x) && isfinite(a->y) && isfinite(b->x) && isfinite(b->y) && isfinite(c->x) && isfinite(c->y)) {
        for (x = 0; x < draw_w; x++) {
            cbuf[y * RD_WIDTH + x] = a->base;
            zbuf[y * RD_WIDTH + x] = a->z;
            fhead[y * RD_WIDTH + x] = -1;
        }
        return;
    }
    for (x = 0; x < draw_w; x++) {
        int at = y * RD_WIDTH + x;
        double w0 = 1, w1 = 0, w2 = 0;
        if (area != 0) {
            w0 = edge(b, c, x + 0.5, y + 0.5) / area;
            w1 = edge(c, a, x + 0.5, y + 0.5) / area;
            w2 = 1 - w0 - w1;
        }
        k.a = clamp255((int)(w0 * ca.a + w1 * cb.a + w2 * cc.a + 0.5));
        k.r = clamp255((int)(w0 * ca.r + w1 * cb.r + w2 * cc.r + 0.5));
        k.g = clamp255((int)(w0 * ca.g + w1 * cb.g + w2 * cc.g + 0.5));
        k.b = clamp255((int)(w0 * ca.b + w1 * cb.b + w2 * cc.b + 0.5));
        cbuf[at] = pack(k);
        zbuf[at] = (float)(w0 * a->z + w1 * b->z + w2 * c->z);
        fhead[at] = -1;
    }
}

/* phase 0: set up a share of the triangles; 1: draw, resolve and copy out the worker's rows */
static void work(WORKER* w) {
    int i, y, x;
    if (phase == 0) {
        int per = (ntotal + nworkers - 1) / nworkers, end = (w->index + 1) * per, base[RD_LISTS + 1], list = 0;
        base[0] = 0;
        for (i = 0; i < RD_LISTS; i++) {
            base[i + 1] = base[i] + rd_frame.ntris[i];
        }
        for (i = w->index * per; i < end && i < ntotal; i++) {
            while (i >= base[list + 1]) {
                list++;
            }
            setup_tri(&setups[i], list, &rd_frame.tris[list][i - base[list]], i - base[list]);
        }
        return;
    }
    w->nfrags = 0;
    w->tex.s = NULL;
    for (y = first_row(w, 0); y < draw_h; y = next_row(w, y)) {
        background_row(y);
    }
    for (i = 0; i < ntotal; i++) {
        if (setups[i].y1 > setups[i].y0) {
            raster(&setups[i], w);
        }
    }
    for (y = first_row(w, 0); y < draw_h; y = next_row(w, y)) {
        resolve_row(w, y);
        if (scale == 1) {
            for (x = 0; x < RD_WIDTH; x++) {
                out_frame[y * out_pitch + x] = 0xFF000000u | cbuf[y * RD_WIDTH + x];
            }
        } else {
            uint32_t* o = &out_frame[2 * y * out_pitch];
            for (x = 0; x < RD_WIDTH; x++) {
                o[x] = o[out_pitch + x] = 0xFF000000u | cbuf[y * RD_WIDTH + x / 2];
            }
        }
    }
}

static int worker_main(void* arg) {
    WORKER* w = arg;
    for (;;) {
        rd_sem_wait(w->go);
        work(w);
        rd_sem_post(done);
    }
    return 0;
}

static void take_quality(void) {
    fast = rd_q.fast;
    nofog = rd_q.nofog;
    nosort = rd_q.nosort;
    scale = rd_q.half ? 2 : 1;
    draw_w = RD_WIDTH / scale, draw_h = RD_HEIGHT / scale;
}

/* MVC2_RDTHREADS=n workers, by default one a CPU */
static void start_workers(void) {
    const char* e = getenv("MVC2_RDTHREADS");
    const char* q = getenv("MVC2_RDQUALITY");
    int i, n = e != NULL ? atoi(e) : rd_cpu_count();
    if (q != NULL) {
        rd_quality(q);
    }
    n = n < 1 ? 1 : n > MAX_WORKERS ? MAX_WORKERS : n;
    done = SDL_CreateSemaphore(0);
    nworkers = 1;
    workers[0].index = 0;
    for (i = 1; i < n && done != NULL; i++) {
        WORKER* w = &workers[i];
        w->index = i;
        w->go = SDL_CreateSemaphore(0);
        if (w->go == NULL || SDL_CreateThread(worker_main, "render", w) == NULL) {
            break;
        }
        nworkers++;
    }
}

static void run_phase(int p) {
    int i;
    phase = p;
    for (i = 1; i < nworkers; i++) {
        rd_sem_post(workers[i].go);
    }
    work(&workers[0]);
    for (i = 1; i < nworkers; i++) {
        rd_sem_wait(done);
    }
}

static double now_ms(void) {
    return SDL_GetPerformanceCounter() * 1000.0 / SDL_GetPerformanceFrequency();
}

/* MVC2_RDTIME: the frame times on stderr */
static void report_time(double ms) {
    static int on = -1, frames;
    static double total, most;
    if (on < 0) {
        on = getenv("MVC2_RDTIME") != NULL;
    }
    if (!on) {
        return;
    }
    total += ms;
    most = ms > most ? ms : most;
    if (++frames == 300) {
        fprintf(stderr, "render: %.2f ms a frame, at most %.2f (%d workers)\n", total / frames, most, nworkers);
        frames = 0;
        total = most = 0;
    }
}

void rd_end_frame(uint32_t* frame, int pitch) {
    double start = now_ms();
    int list, i;
    if (nworkers == 0) {
        make_tables();
        start_workers();
    }
    take_quality();
    log_frame();
    save_frame();
    ntotal = 0;
    for (list = 0; list < RD_LISTS; list++) {
        ntotal += rd_frame.ntris[list];
    }
    if (ntotal > maxsetups) {
        SETUP* p = realloc(setups, (size_t)ntotal * sizeof *p);
        if (p == NULL) {
            return;
        }
        setups = p;
        maxsetups = ntotal;
    }
    out_frame = frame;
    out_pitch = pitch;
    fog_on = 0;
    for (i = 0; i < 128; i++) {
        fog_on |= rd_fog.table[i] != 0.0f;
    }
    for (i = 0; i < 3; i++) {
        background[i] = rd_frame.background[i];
        if (scale != 1) {
            background[i].x /= scale, background[i].y /= scale;
        }
    }
    run_phase(0);
    run_phase(1);
    if (dbg_log != NULL) {
        fclose(dbg_log);
        dbg_log = NULL;
    }
    report_time(now_ms() - start);
}

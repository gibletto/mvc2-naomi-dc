/*
 * rd_gl1.c  The frame's lists drawn by OpenGL 1.4's fixed function, as rd_soft draws them
 *
 * Unit 0 shades with the texture (texture_env_combine), the next adds the offset colour, a constant a batch, and
 * the last mixes in the table fog: a ramp of the table's value over w, which a perspective-correct texture
 * coordinate equal to w reads. Depth: the frame's 1/w values laid out on the depth buffer, the 3D triangles'
 * range evenly and each flat 1/w outside it on a step of its own. A flat triangle goes in at its own pixels,
 * w 1; a 3D one as x * w, y * w, w. Translucent triangles are drawn far to near, in their order where level.
 */

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "rd.h"
#include "rd_list.h"
#include "rd_gl1.h"
#include "rd_gl1_tex.h"
#include "rd_sys.h"

#if defined(_WIN32) && !defined(_WIN64)
#define GLCALL __stdcall
#else
#define GLCALL
#endif

typedef unsigned int GLenum;
typedef unsigned int GLuint;
typedef int GLint;
typedef int GLsizei;
typedef float GLfloat;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef double GLclampd;

enum {
    GL_TRIANGLES = 0x0004, GL_CULL_FACE = 0x0B44, GL_LIGHTING = 0x0B50, GL_FOG = 0x0B60, GL_DEPTH_TEST = 0x0B71,
    GL_ALPHA_TEST = 0x0BC0, GL_DITHER = 0x0BD0, GL_BLEND = 0x0BE2, GL_SCISSOR_TEST = 0x0C11, GL_TEXTURE_2D = 0x0DE1,
    GL_PERSPECTIVE_CORRECTION_HINT = 0x0C50, GL_NICEST = 0x1102, GL_UNPACK_ALIGNMENT = 0x0CF5, GL_PACK_ALIGNMENT = 0x0D05,
    GL_NEVER = 0x0200, GL_GEQUAL = 0x0206, GL_ALWAYS = 0x0207,
    GL_SRC_COLOR = 0x0300, GL_ONE_MINUS_SRC_COLOR = 0x0301, GL_SRC_ALPHA = 0x0302, GL_ONE_MINUS_SRC_ALPHA = 0x0303,
    GL_DST_ALPHA = 0x0304, GL_ONE_MINUS_DST_ALPHA = 0x0305, GL_DST_COLOR = 0x0306, GL_ONE_MINUS_DST_COLOR = 0x0307,
    GL_DEPTH_BUFFER_BIT = 0x0100, GL_COLOR_BUFFER_BIT = 0x4000,
    GL_UNSIGNED_BYTE = 0x1401, GL_UNSIGNED_INT = 0x1405, GL_FLOAT = 0x1406, GL_RGB = 0x1907, GL_RGBA = 0x1908,
    GL_UNSIGNED_SHORT_5_6_5 = 0x8363, GL_UNSIGNED_SHORT_4_4_4_4_REV = 0x8365, GL_UNSIGNED_SHORT_1_5_5_5_REV = 0x8366,
    GL_TEXTURE_MAG_FILTER = 0x2800, GL_TEXTURE_MIN_FILTER = 0x2801, GL_TEXTURE_WRAP_S = 0x2802, GL_TEXTURE_WRAP_T = 0x2803,
    GL_NEAREST = 0x2600, GL_LINEAR = 0x2601, GL_NEAREST_MIPMAP_NEAREST = 0x2700, GL_LINEAR_MIPMAP_NEAREST = 0x2701,
    GL_REPEAT = 0x2901, GL_CLAMP_TO_EDGE = 0x812F, GL_MIRRORED_REPEAT = 0x8370, GL_TEXTURE_MAX_LEVEL = 0x813D,
    GL_TEXTURE_ENV = 0x2300, GL_TEXTURE_ENV_MODE = 0x2200, GL_TEXTURE_ENV_COLOR = 0x2201,
    GL_COMBINE = 0x8570, GL_COMBINE_RGB = 0x8571, GL_COMBINE_ALPHA = 0x8572,
    GL_SOURCE0_RGB = 0x8580, GL_SOURCE0_ALPHA = 0x8588, GL_OPERAND0_RGB = 0x8590, GL_OPERAND0_ALPHA = 0x8598,
    GL_REPLACE = 0x1E01, GL_MODULATE = 0x2100, GL_ADD = 0x0104, GL_INTERPOLATE = 0x8575,
    GL_TEXTURE = 0x1702, GL_CONSTANT = 0x8576, GL_PRIMARY_COLOR = 0x8577, GL_PREVIOUS = 0x8578,
    GL_TEXTURE_FILTER_CONTROL = 0x8500, GL_TEXTURE_LOD_BIAS = 0x8501, GL_TEXTURE0 = 0x84C0,
    GL_VERTEX_ARRAY = 0x8074, GL_COLOR_ARRAY = 0x8076, GL_TEXTURE_COORD_ARRAY = 0x8078,
    GL_MODELVIEW = 0x1700, GL_PROJECTION = 0x1701, GL_SMOOTH = 0x1D01,
    GL_MAX_TEXTURE_SIZE = 0x0D33, GL_ALPHA_BITS = 0x0D55, GL_DEPTH_BITS = 0x0D56, GL_MAX_TEXTURE_UNITS = 0x84E2,
    GL_VENDOR = 0x1F00, GL_RENDERER = 0x1F01, GL_VERSION = 0x1F02, GL_EXTENSIONS = 0x1F03,
    /* the Pinedra driver's GL_PND_ordered_translucency and GL_PND_window_vertices (driver 0.5.44) */
    GL_ORDERED_TRANSLUCENCY_PND = 0x1D100, GL_WINDOW_VERTICES_PND = 0x1D105, GL_LOWER_LEFT = 0x8CA1, GL_UPPER_LEFT = 0x8CA2,
    /* GL_EXT_paletted_texture */
    GL_COLOR_INDEX = 0x1900, GL_COLOR_INDEX4_EXT = 0x80E4, GL_COLOR_INDEX8_EXT = 0x80E5, GL_RGBA8 = 0x8058
};

#define GL_FUNCS(X) \
    X(void, Viewport, (GLint, GLint, GLsizei, GLsizei)) \
    X(void, ClearColor, (GLfloat, GLfloat, GLfloat, GLfloat)) \
    X(void, ClearDepth, (GLclampd)) \
    X(void, Clear, (GLbitfield)) \
    X(void, Enable, (GLenum)) \
    X(void, Disable, (GLenum)) \
    X(void, Hint, (GLenum, GLenum)) \
    X(void, DepthFunc, (GLenum)) \
    X(void, DepthMask, (GLboolean)) \
    X(void, BlendFunc, (GLenum, GLenum)) \
    X(void, AlphaFunc, (GLenum, GLfloat)) \
    X(void, ShadeModel, (GLenum)) \
    X(void, MatrixMode, (GLenum)) \
    X(void, LoadIdentity, (void)) \
    X(void, LoadMatrixf, (const GLfloat*)) \
    X(void, TexEnvi, (GLenum, GLenum, GLint)) \
    X(void, TexEnvf, (GLenum, GLenum, GLfloat)) \
    X(void, TexEnvfv, (GLenum, GLenum, const GLfloat*)) \
    X(void, GenTextures, (GLsizei, GLuint*)) \
    X(void, DeleteTextures, (GLsizei, const GLuint*)) \
    X(void, BindTexture, (GLenum, GLuint)) \
    X(void, TexImage2D, (GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const void*)) \
    X(void, TexSubImage2D, (GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, const void*)) \
    X(void, TexParameteri, (GLenum, GLenum, GLint)) \
    X(void, PixelStorei, (GLenum, GLint)) \
    X(void, ActiveTexture, (GLenum)) \
    X(void, ClientActiveTexture, (GLenum)) \
    X(void, EnableClientState, (GLenum)) \
    X(void, DisableClientState, (GLenum)) \
    X(void, VertexPointer, (GLint, GLenum, GLsizei, const void*)) \
    X(void, ColorPointer, (GLint, GLenum, GLsizei, const void*)) \
    X(void, TexCoordPointer, (GLint, GLenum, GLsizei, const void*)) \
    X(void, DrawArrays, (GLenum, GLint, GLsizei)) \
    X(void, DrawElements, (GLenum, GLsizei, GLenum, const void*)) \
    X(void, ReadPixels, (GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, void*)) \
    X(void, GetIntegerv, (GLenum, GLint*)) \
    X(const unsigned char*, GetString, (GLenum)) \
    X(GLenum, GetError, (void))

#define GL_PTR(ret, name, args) static ret(GLCALL* gl##name) args;
GL_FUNCS(GL_PTR)

RD_GL1_STATS rd_gl1_stats;

/* order: the translucent list given in its own order as window-space vertices, the driver ordering it as the PVR did
   (GL_PND_ordered_translucency, GL_PND_window_vertices); clut: a paletted texture sent as its indices and its colours,
   the driver making the texels on its own thread (GL_EXT_paletted_texture). Each where the driver has it */
static int order_want, clut_want, ord_on, ord_frame, ord_live, clut_on;
static void(GLCALL* glWindowOriginPND)(GLenum);
static void(GLCALL* glColorTableEXT)(GLenum, GLenum, GLsizei, GLenum, GLenum, const void*);

static int units;                   /* texture units: 3, offset on 1 and fog on 2; 2, both on 1, the offset lost */
static int fog_unit, offset_unit;
static int depth_bits;
static int lod_bias_ok;
static int tex16, upward, less, atlas, pool_on;
static float snap = 256.0f;         /* the GPU's grid, a pixel's parts; 0: corners sent as they are */
/* a translucent triangle whose farthest 1/w is past deep_k times its nearest is cut at the depths of the shallow ones
   (shallow_k) its box meets */
static float deep_k = 1.05f, shallow_k = 1.25f;
static uint32_t frame_no;
static RD_FRAME* fr;                /* the frame drawn, its palette (rd_gl1_pal) and its fog */
static const RD_FOG* frame_fog = &rd_fog;
static int out_w = RD_WIDTH, out_h = RD_HEIGHT;
static GLuint dummy_tex, fog_tex;

static void option(const char* words, const char* name, float* value) {
    const char* at = words != NULL ? strstr(words, name) : NULL;
    if (at != NULL) {
        *value = (float)atof(at + strlen(name));
    }
}

void rd_gl1_size(int width, int height) {
    out_w = width > 0 ? width : RD_WIDTH;
    out_h = height > 0 ? height : RD_HEIGHT;
}

void rd_gl1_options(const char* words) {
    tex16 = words != NULL && strstr(words, "tex16") != NULL;
    atlas = words != NULL && strstr(words, "atlas") != NULL;
    pool_on = words != NULL && strstr(words, "pool") != NULL;
    upward = words != NULL && strstr(words, "up") != NULL;
    less = words != NULL && strstr(words, "less") != NULL;
    order_want = words != NULL && strstr(words, "order") != NULL;
    clut_want = words != NULL && strstr(words, "clut") != NULL;
    option(words, "snap=", &snap);
    option(words, "deep=", &deep_k);
    option(words, "shallow=", &shallow_k);
}

/* ---- the GL's state, as last set ---- */

typedef struct {
    int on;                         /* GL_TEXTURE_2D */
    GLuint tex;
    int env;                        /* the combine set up, 0 none */
    GLint envv[15];                 /* its TEXTURE_ENV values, as set_combine sends them; 0 not known */
    uint32_t color;                 /* TEXTURE_ENV_COLOR, ARGB */
    float bias;
    int coords;                     /* its coordinate array */
} UNIT;

static struct {
    UNIT u[4];
    int active, client;
    int blend, src, dst;
    int depth_func, depth_mask, alpha_test;
} gs;

static void forget_binds(void) {
    int u;
    for (u = 0; u < 4; u++) {
        gs.u[u].tex = ~0u;
    }
}

static void active(int u) {
    if (gs.active != u) {
        gs.active = u;
        glActiveTexture(GL_TEXTURE0 + (GLenum)u);
    }
}

static void unit_on(int u, int on) {
    if (gs.u[u].on != on) {
        active(u);
        gs.u[u].on = on;
        (on ? glEnable : glDisable)(GL_TEXTURE_2D);
        glClientActiveTexture(GL_TEXTURE0 + (GLenum)u);
        (on ? glEnableClientState : glDisableClientState)(GL_TEXTURE_COORD_ARRAY);
    }
}

static void bind(int u, GLuint tex) {
    if (gs.u[u].tex != tex) {
        active(u);
        gs.u[u].tex = tex;
        glBindTexture(GL_TEXTURE_2D, tex);
    }
}

static void env_color(int u, uint32_t argb) {
    if (gs.u[u].color != argb) {
        float c[4];
        c[0] = (float)(argb >> 16 & 255) / 255.0f;
        c[1] = (float)(argb >> 8 & 255) / 255.0f;
        c[2] = (float)(argb & 255) / 255.0f;
        c[3] = (float)(argb >> 24) / 255.0f;
        active(u);
        gs.u[u].color = argb;
        glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, c);
    }
}

/* a combine: the function and three sources with their operands, for rgb then alpha */
typedef struct {
    GLenum rgb, rs[3], ro[3], alpha, as[3], ao[3];
} COMBINE;

/* only the values that change are sent: each glTexEnvi costs the driver's thread */
static void set_combine(int u, int key, const COMBINE* c) {
    GLenum name[15];
    GLint value[15];
    int i;
    if (gs.u[u].env == key) {
        return;
    }
    gs.u[u].env = key;
    name[0] = GL_TEXTURE_ENV_MODE, value[0] = GL_COMBINE;
    name[1] = GL_COMBINE_RGB, value[1] = (GLint)c->rgb;
    name[2] = GL_COMBINE_ALPHA, value[2] = (GLint)c->alpha;
    for (i = 0; i < 3; i++) {
        name[3 + 4 * i] = GL_SOURCE0_RGB + (GLenum)i, value[3 + 4 * i] = (GLint)c->rs[i];
        name[4 + 4 * i] = GL_OPERAND0_RGB + (GLenum)i, value[4 + 4 * i] = (GLint)c->ro[i];
        name[5 + 4 * i] = GL_SOURCE0_ALPHA + (GLenum)i, value[5 + 4 * i] = (GLint)c->as[i];
        name[6 + 4 * i] = GL_OPERAND0_ALPHA + (GLenum)i, value[6 + 4 * i] = (GLint)c->ao[i];
    }
    for (i = 0; i < 15; i++) {
        if (gs.u[u].envv[i] != value[i]) {
            active(u);
            gs.u[u].envv[i] = value[i];
            glTexEnvi(GL_TEXTURE_ENV, name[i], value[i]);
        }
    }
}

/* unit 0's shading: the TSP's instruction, the texel's alpha taken as 1 without tex_alpha (the constant's) */
static void shading_env(const RD_STATE* s) {
    COMBINE c;
    GLenum ta = s->tex_alpha ? GL_TEXTURE : GL_CONSTANT;
    int i;
    for (i = 0; i < 3; i++) {
        c.rs[i] = c.as[i] = GL_TEXTURE;
        c.ro[i] = GL_SRC_COLOR;
        c.ao[i] = GL_SRC_ALPHA;
    }
    switch (s->shading) {
    case RD_DECAL:
        c.rgb = GL_REPLACE;
        c.alpha = GL_REPLACE, c.as[0] = ta;
        break;
    case RD_MODULATE:
        c.rgb = GL_MODULATE, c.rs[1] = GL_PRIMARY_COLOR;
        c.alpha = GL_REPLACE, c.as[0] = ta;
        break;
    case RD_DECAL_ALPHA:
        if (s->tex_alpha) {
            c.rgb = GL_INTERPOLATE, c.rs[1] = GL_PRIMARY_COLOR, c.ro[2] = GL_SRC_ALPHA;
        } else {
            c.rgb = GL_REPLACE;
        }
        c.alpha = GL_REPLACE, c.as[0] = GL_PRIMARY_COLOR;
        break;
    default:
        c.rgb = GL_MODULATE, c.rs[1] = GL_PRIMARY_COLOR;
        c.alpha = GL_MODULATE, c.as[0] = ta, c.as[1] = GL_PRIMARY_COLOR;
        break;
    }
    set_combine(0, 1 + s->shading + 4 * s->tex_alpha, &c);
}

/* the offset added, the alpha kept */
static void offset_env(int u) {
    static const COMBINE c = { GL_ADD, { GL_PREVIOUS, GL_CONSTANT, GL_CONSTANT },
                               { GL_SRC_COLOR, GL_SRC_COLOR, GL_SRC_COLOR }, GL_REPLACE,
                               { GL_PREVIOUS, GL_PREVIOUS, GL_PREVIOUS }, { GL_SRC_ALPHA, GL_SRC_ALPHA, GL_SRC_ALPHA } };
    set_combine(u, 100, &c);
}

/* table fog: the fog colour mixed in by the ramp's value; table2: the fog colour, the value its alpha */
static void fog_env(int u, int table2) {
    static const COMBINE mix = { GL_INTERPOLATE, { GL_CONSTANT, GL_PREVIOUS, GL_TEXTURE },
                                 { GL_SRC_COLOR, GL_SRC_COLOR, GL_SRC_ALPHA }, GL_REPLACE,
                                 { GL_PREVIOUS, GL_PREVIOUS, GL_PREVIOUS }, { GL_SRC_ALPHA, GL_SRC_ALPHA, GL_SRC_ALPHA } };
    static const COMBINE two = { GL_REPLACE, { GL_CONSTANT, GL_CONSTANT, GL_CONSTANT },
                                 { GL_SRC_COLOR, GL_SRC_COLOR, GL_SRC_COLOR }, GL_REPLACE,
                                 { GL_TEXTURE, GL_TEXTURE, GL_TEXTURE }, { GL_SRC_ALPHA, GL_SRC_ALPHA, GL_SRC_ALPHA } };
    set_combine(u, table2 ? 201 : 200, table2 ? &two : &mix);
}

static void set_blend(int on, int src, int dst) {
    static const GLenum fs[8] = { 0, 1, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
                                  GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA };
    static const GLenum fd[8] = { 0, 1, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
                                  GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA };
    if (on != gs.blend) {
        gs.blend = on;
        (on ? glEnable : glDisable)(GL_BLEND);
    }
    if (on && (src != gs.src || dst != gs.dst)) {
        gs.src = src, gs.dst = dst;
        glBlendFunc(fs[src & 7], fd[dst & 7]);
    }
}

static void set_depth(int func, int mask) {
    static const int mirrored[8] = { RD_NEVER, RD_GREATER, RD_EQUAL, RD_GEQUAL, RD_LESS, RD_NOTEQUAL, RD_LEQUAL,
                                     RD_ALWAYS };
    func = less ? mirrored[func] : func;
    if (func != gs.depth_func) {
        gs.depth_func = func;
        glDepthFunc(GL_NEVER + (GLenum)func);
    }
    if (mask != gs.depth_mask) {
        gs.depth_mask = mask;
        glDepthMask((GLboolean)mask);
    }
}

static void set_alpha_test(int on) {
    if (on != gs.alpha_test) {
        gs.alpha_test = on;
        (on ? glEnable : glDisable)(GL_ALPHA_TEST);
    }
}

/* ---- textures ---- */

typedef struct {
    const RD_TEXTURE* t;
    uint32_t serial, pal_hash;      /* its texels' and, for the palette formats, its palette's */
    int key;                        /* its palette + 1 (0 not a palette format) and its sampling (sampler()) */
    uint32_t used;
    GLuint name;
    int w, h, levels, format;       /* as made */
    int form;                       /* a palette format's 16-bit form (rd_gl1_tex.h), -1 RGBA8 */
    GLint min, mag, wrap_s, wrap_t; /* as last set, 0 none yet */
    int own;                        /* name holds the contents serial and pal_hash say, not the pool */
    int pool;                       /* its contents' pool slot, -1 none */
    uint32_t pool_gen;              /* the slot's, when found there */
} GTEX;

#define EVICT_FRAMES 600

static GTEX* gtex;
static int ngtex, maxgtex;
static int* hash;                   /* gtex's indices, -1 empty; 2 * maxgtex */
static uint32_t* conv;              /* a level made RGBA8 */
static size_t conv_room;

static int is_pal(const RD_TEXTURE* t) {
    return t->format == RD_PAL4 || t->format == RD_PAL8;
}

static uint32_t argb_to_rgba(uint32_t c) {
    return (c & 0xFF00FF00u) | (c >> 16 & 255) | (c & 255) << 16;
}

static uint32_t pal_hash(const RD_TEXTURE* t, int palette) {
    int n = t->format == RD_PAL4 ? 16 : 256, i;
    uint32_t h = 2166136261u;
    for (i = 0; i < n; i++) {
        h = (h ^ rd_gl1_pal[(palette + i) & 1023]) * 16777619u;
    }
    return h;
}

static uint32_t hash_of(const RD_TEXTURE* t, int key) {
    return ((uint32_t)((uintptr_t)t >> 4) + (uint32_t)key * 40503u) * 2654435761u & (uint32_t)(2 * maxgtex - 1);
}

static void rehash(void) {
    int i;
    for (i = 0; i < 2 * maxgtex; i++) {
        hash[i] = -1;
    }
    for (i = 0; i < ngtex; i++) {
        uint32_t h = hash_of(gtex[i].t, gtex[i].key);
        while (hash[h] >= 0) {
            h = (h + 1) & (uint32_t)(2 * maxgtex - 1);
        }
        hash[h] = i;
    }
}

static GTEX* entry(const RD_TEXTURE* t, int key) {
    uint32_t h;
    GTEX* e;
    if (maxgtex > 0) {
        for (h = hash_of(t, key); hash[h] >= 0; h = (h + 1) & (uint32_t)(2 * maxgtex - 1)) {
            e = &gtex[hash[h]];
            if (e->t == t && e->key == key) {
                return e;
            }
        }
    }
    if (ngtex == maxgtex) {
        int n = maxgtex ? maxgtex * 2 : 512;
        GTEX* p = realloc(gtex, (size_t)n * sizeof *p);
        int* q = realloc(hash, (size_t)n * 2 * sizeof *q);
        if (p != NULL) {
            gtex = p;
        }
        if (q != NULL) {
            hash = q;
        }
        if (p == NULL || q == NULL) {
            return NULL;
        }
        maxgtex = n;
        rehash();
    }
    e = &gtex[ngtex];
    memset(e, 0, sizeof *e);
    e->pool = -1;
    e->t = t;
    e->key = key;
    for (h = hash_of(t, key); hash[h] >= 0; h = (h + 1) & (uint32_t)(2 * maxgtex - 1)) {
    }
    hash[h] = ngtex++;
    return e;
}

/* texture names from a stock made at the start, a let-go texture's name given back to it: Pinedra's glGenTextures
   waits for the GPU, a frame or more in play */
#define NAMES (RD_POOL + 512)
static GLuint names[NAMES];
static int n_names;

static GLuint new_name(void) {
    if (n_names == 0) {
        glGenTextures(NAMES / 4, names);
        n_names = NAMES / 4;
    }
    return names[--n_names];
}

/* its storage let go, the name kept */
static void free_name(GLuint name) {
    static const uint32_t texel;
    if (name == 0) {
        return;
    }
    if (n_names == NAMES) {
        glDeleteTextures(1, &name);
        return;
    }
    bind(0, name);
    active(0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &texel);
    names[n_names++] = name;
}

static void evict(void) {
    int i, n = 0;
    for (i = 0; i < ngtex; i++) {
        if (frame_no - gtex[i].used > EVICT_FRAMES) {
            free_name(gtex[i].name);
        } else {
            gtex[n++] = gtex[i];
        }
    }
    if (n != ngtex) {
        ngtex = n;
        rehash();
        forget_binds();
    }
}

static const void* level_texels(const RD_TEXTURE* t, int k) {
    return k == 0 ? t->texels : t->mip[k];
}

static const GLenum form_format[3] = { GL_RGBA, GL_RGB, GL_RGBA };
static const GLenum form_type[3] = { GL_UNSIGNED_SHORT_1_5_5_5_REV, GL_UNSIGNED_SHORT_5_6_5,
                                     GL_UNSIGNED_SHORT_4_4_4_4_REV };

/* level k's texels as GL takes them: format and type; a palette format's in form, -1 RGBA8 */
static const void* level_data(const RD_TEXTURE* t, int k, int palette, int form, int w, int h, GLenum* format,
                              GLenum* type) {
    size_t n = (size_t)w * h;
    const void* src = level_texels(t, k);
    if (n > conv_room) {
        uint32_t* p = realloc(conv, n * sizeof *p);
        if (p == NULL) {
            return NULL;
        }
        conv = p;
        conv_room = n;
    }
    if (tex16 && !is_pal(t)) {
        if (t->format == RD_RGB565) {
            *format = GL_RGB;
            *type = GL_UNSIGNED_SHORT_5_6_5;
            return src;
        }
        rd_gl1_swap16(src, (int)n, t->format, (uint16_t*)conv);
        *format = form_format[t->format];
        *type = form_type[t->format];
        return conv;
    }
    if (is_pal(t) && form >= 0) {
        rd_gl1_expand16(src, (int)n, t, palette, form, (uint16_t*)conv);
        *format = form_format[form];
        *type = form_type[form];
        return conv;
    }
    if (is_pal(t)) {
        rd_gl1_expand32(src, (int)n, t, palette, conv);
    } else {
        rd_gl1_rgba16(src, (int)n, t->format, conv);
    }
    *format = GL_RGBA;
    *type = GL_UNSIGNED_BYTE;
    return conv;
}

static uint32_t us_since(uint64_t t0) {
    return (uint32_t)((SDL_GetPerformanceCounter() - t0) * 1000000u / SDL_GetPerformanceFrequency());
}

/* t's GL texture for this palette, made or sent again as need be; 0 when it can't be had */
static GTEX pooled[RD_POOL];       /* the pool's slots' textures */

/* the texture of the pool's slot with the contents of t (in form), sent if they're new to it; NULL when the pool has
   no slot free */
static GTEX* pool_texture(GTEX* e, const RD_TEXTURE* t, int palette, int form, uint32_t ph) {
    int slot, r = rd_gl1_pool(t, palette, frame_no, &slot);
    GTEX* p;
    if (r < 0) {
        return NULL;
    }
    p = &pooled[slot];
    if (r == 1) {
        GLenum format, type;
        const void* d = level_data(t, 0, palette, form, t->width, t->height, &format, &type);
        if (d == NULL) {
            return NULL;
        }
        if (p->name == 0) {
            p->name = new_name();
        }
        bind(0, p->name);
        active(0);
        if (p->form != form || p->w == 0) {
            p->min = p->mag = p->wrap_s = p->wrap_t = 0;
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
            glTexImage2D(GL_TEXTURE_2D, 0, format == GL_RGB ? GL_RGB : GL_RGBA, t->width, t->height, 0, format, type, d);
        } else {
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, t->width, t->height, format, type, d);
        }
        p->w = t->width, p->h = t->height, p->levels = 0, p->format = t->format, p->form = form;
        rd_gl1_stats.upload_bytes += (uint32_t)(t->width * t->height * (form < 0 ? 4 : 2));
        rd_gl1_stats.uploads++;
    }
    e->pool = slot;
    e->pool_gen = rd_gl1_pool_gen(slot);
    e->serial = t->serial;
    e->pal_hash = ph;
    return p;
}

/* a texture is kept once for each way it is sampled: Pinedra's driver redoes one whose filter or wrap changes, at
   several ms a time */
static int sampler(const RD_STATE* s) {
    return s->bilinear | (s->uv_mode & 15) << 1;
}

static int entries_of(const RD_TEXTURE* t) {
    return t->format == RD_PAL4 ? 16 : 256;
}

/* clut: e as its own paletted texture, its colours sent at every use and its indices when they change */
static GTEX* clut_texture(GTEX* e, const RD_TEXTURE* t, int palette, uint32_t ph) {
    int n = entries_of(t), k, remake = e->name == 0 || e->form != -2 || e->w != t->width || e->h != t->height ||
                                    e->levels != t->levels;
    int same = !remake && e->own && e->serial == t->serial;
    uint32_t pt[256];
    for (k = 0; k < n; k++) {
        uint32_t c = rd_gl1_pal[(palette + k) & 1023];
        pt[k] = (c & 0xFF00FF00u) | (c >> 16 & 255) | (c & 255) << 16;
    }
    e->pool = -1;
    if (e->name == 0) {
        e->name = new_name();
    }
    bind(0, e->name);
    active(0);
    if (remake) {
        e->min = e->mag = e->wrap_s = e->wrap_t = 0;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, t->levels);
    }
    glColorTableEXT(GL_TEXTURE_2D, GL_RGBA8, n, GL_RGBA, GL_UNSIGNED_BYTE, pt);
    for (k = 0; !same && k <= t->levels; k++) {
        int w = t->width >> k, h = t->height >> k;
        w = w < 1 ? 1 : w;
        h = h < 1 ? 1 : h;
        if (remake) {
            glTexImage2D(GL_TEXTURE_2D, k, n == 16 ? GL_COLOR_INDEX4_EXT : GL_COLOR_INDEX8_EXT, w, h, 0, GL_COLOR_INDEX,
                         GL_UNSIGNED_BYTE, level_texels(t, k));
        } else {
            glTexSubImage2D(GL_TEXTURE_2D, k, 0, 0, w, h, GL_COLOR_INDEX, GL_UNSIGNED_BYTE, level_texels(t, k));
        }
        rd_gl1_stats.upload_bytes += (uint32_t)(w * h);
    }
    rd_gl1_stats.uploads += !same;
    e->serial = t->serial;
    e->pal_hash = ph;
    e->w = t->width, e->h = t->height, e->levels = t->levels, e->format = t->format, e->form = -2;
    e->own = 1;
    return e;
}

static GTEX* texture(const RD_TEXTURE* t, int palette, int sampling) {
    int pal = is_pal(t), k, remake, form;
    uint32_t ph;
    GTEX* e;
    if (t->texels == NULL || t->width < 1 || t->height < 1) {
        return NULL;
    }
    palette = pal ? palette & 1023 : -1;
    e = entry(t, (palette + 1) | sampling << 11);
    if (e == NULL) {
        return NULL;
    }
    e->used = frame_no;
    ph = pal ? pal_hash(t, palette) : 0;
    if (e->pool >= 0 && e->serial == t->serial && e->pal_hash == ph && rd_gl1_pool_gen(e->pool) == e->pool_gen) {
        rd_gl1_pool_use(e->pool, frame_no);
        return &pooled[e->pool];
    }
    if (e->name != 0 && e->own && e->serial == t->serial && e->pal_hash == ph) {
        return e;
    }
    form = pal && tex16 ? rd_gl1_pal_form(t, palette) : -1;
    if (clut_on && pal) {
        return clut_texture(e, t, palette, ph);
    }
    e->pool = -1;
    e->own = 0;
    if (pool_on && rd_gl1_pool_texture(t)) {
        GTEX* p = pool_texture(e, t, palette, form, ph);
        if (p != NULL) {
            return p;
        }
    }
    remake = e->name == 0 || e->w != t->width || e->h != t->height || e->levels != t->levels || e->format != t->format ||
             e->form != form;
    if (e->name == 0) {
        e->name = new_name();
    }
    bind(0, e->name);
    active(0);
    if (remake) {
        e->min = e->mag = e->wrap_s = e->wrap_t = 0;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, t->levels);
    }
    for (k = 0; k <= t->levels; k++) {
        int w = t->width >> k, h = t->height >> k;
        GLenum format, type;
        const void* d;
        w = w < 1 ? 1 : w;
        h = h < 1 ? 1 : h;
        d = level_data(t, k, palette, form, w, h, &format, &type);
        if (d == NULL) {
            return NULL;
        }
        if (remake) {
            glTexImage2D(GL_TEXTURE_2D, k, format == GL_RGB ? GL_RGB : GL_RGBA, w, h, 0, format, type, d);
        } else {
            glTexSubImage2D(GL_TEXTURE_2D, k, 0, 0, w, h, format, type, d);
        }
        rd_gl1_stats.upload_bytes += (uint32_t)(w * h * (type == GL_UNSIGNED_BYTE ? 4 : 2));
    }
    rd_gl1_stats.uploads++;
    e->serial = t->serial;
    e->pal_hash = ph;
    e->w = t->width, e->h = t->height, e->levels = t->levels, e->format = t->format, e->form = form;
    e->own = 1;
    return e;
}

static void tex_params(GTEX* e, const RD_STATE* s) {
    GLint mag = s->bilinear ? GL_LINEAR : GL_NEAREST;
    GLint min = e->levels > 0 ? (s->bilinear ? GL_LINEAR_MIPMAP_NEAREST : GL_NEAREST_MIPMAP_NEAREST) : mag;
    GLint ws = s->uv_mode & 2 ? GL_CLAMP_TO_EDGE : s->uv_mode & 8 ? GL_MIRRORED_REPEAT : GL_REPEAT;
    GLint wt = s->uv_mode & 1 ? GL_CLAMP_TO_EDGE : s->uv_mode & 4 ? GL_MIRRORED_REPEAT : GL_REPEAT;
    if (e->mag != mag) {
        e->mag = mag;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
    }
    if (e->min != min) {
        e->min = min;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
    }
    if (e->wrap_s != ws) {
        e->wrap_s = ws;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ws);
    }
    if (e->wrap_t != wt) {
        e->wrap_t = wt;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wt);
    }
}

/* ---- the sprites' tiles ---- */

static GTEX pages[RD_FORMS];        /* a form's atlas page, name 0 not made */
static int* st_at;                  /* each state's tile in its page (x + y * RD_ATLAS), -1 none */
static int* st_key;                 /* the first state drawn as it is: its batch's */
static uint8_t* st_form;
static int room_at, room_key, room_form;
static uint16_t tile_texels[RD_SLOT * RD_SLOT];

static int grow(void** p, int* room, int want, size_t size);

static GTEX* page(int form) {
    GTEX* e = &pages[form];
    if (e->name == 0) {
        glGenTextures(1, &e->name);
        bind(0, e->name);
        active(0);
        glTexImage2D(GL_TEXTURE_2D, 0, form == RD_F565 ? GL_RGB : GL_RGBA, RD_ATLAS, RD_ATLAS, 0, form_format[form],
                     form_type[form], NULL);
        e->w = e->h = RD_ATLAS;
        e->levels = 0;
        e->min = e->mag = e->wrap_s = e->wrap_t = 0;
    }
    return e;
}

/* the 15 bytes before origin: what a batch draws with but the texture */
static unsigned state_hash(const RD_STATE* s, int form) {
    const uint8_t* b = (const uint8_t*)s;
    unsigned h = 2166136261u ^ (unsigned)form;
    int i;
    for (i = 0; i < 15; i++) {
        h = (h ^ b[i]) * 16777619u;
    }
    return h;
}

/* each state's tile, sent if need be; the states that differ only by their tile drawn as one */
static void tiles(void) {
    static int first[512];          /* state + 1 */
    int n = fr->nstates, i;
    if (!grow((void**)&st_at, &room_at, n + 1, sizeof *st_at) || !grow((void**)&st_key, &room_key, n + 1, sizeof *st_key) ||
        !grow((void**)&st_form, &room_form, n + 1, 1)) {
        atlas = 0;
        return;
    }
    memset(first, 0, sizeof first);
    for (i = 0; i < n; i++) {
        const RD_STATE* s = &fr->states[i];
        int form, at, r;
        st_at[i] = -1;
        st_key[i] = i;
        if (!atlas || !rd_gl1_tile_state(s) || s->fog == RD_FOG_TABLE2 ||
            (form = rd_gl1_pal_form(s->tex, s->palette & 1023)) < 0) {
            continue;
        }
        r = rd_gl1_tile(s->tex, s->palette & 1023, form, frame_no, &at, tile_texels);
        if (r < 0) {
            continue;
        }
        if (r > 0) {
            GTEX* e = page(form);
            bind(0, e->name);
            active(0);
            glTexSubImage2D(GL_TEXTURE_2D, 0, at % RD_ATLAS - 1, at / RD_ATLAS - 1, RD_SLOT, RD_SLOT, form_format[form],
                            form_type[form], tile_texels);
            rd_gl1_stats.uploads++;
            rd_gl1_stats.upload_bytes += RD_SLOT * RD_SLOT * 2;
        }
        st_at[i] = at;
        st_form[i] = (uint8_t)form;
        {
            unsigned h = state_hash(s, form) & 511;
            for (; first[h] != 0; h = (h + 1) & 511) {
                const RD_STATE* o = &fr->states[first[h] - 1];
                if (st_form[first[h] - 1] == form && memcmp(o, s, 15) == 0) {
                    st_key[i] = first[h] - 1;
                    break;
                }
            }
            if (first[h] == 0) {
                first[h] = i + 1;
            }
        }
    }
}

/* ---- the frame's depth layout ---- */

#define SLOT_STEP 4                 /* depth units a flat 1/w outside the 3D range */

/* 1/w values are compared as their bits, which rise with them where they are positive: on the MiSTer's
   Cortex-A9 a float comparison stalls the pipeline */
#define USABLE_END 0x7149F2CAu      /* 1e30f's bits */

static float lo3, hi3;              /* the 3D triangles' 1/w */
static uint32_t lo3b, hi3b;         /* and their bits */
static double d_lo3, d_scale3;
static int scaled3;                 /* d_scale3 > 0 */      /* their depths: d_lo3 + (z - lo3) * d_scale3 */
static double z3_a, z3_b;          /* and the GPU's, depth_of(z) * 2 - 1, as z3_a + z * z3_b */
static uint32_t* flats;             /* the flat 1/w values outside it, sorted, each once (bits) */
static int nflats, maxflats, nbelow;
static double unit_d;               /* a depth unit */

static uint32_t zbits(float z) {
    uint32_t u;
    memcpy(&u, &z, sizeof u);
    return u;
}

static float zfloat(uint32_t u) {
    float z;
    memcpy(&z, &u, sizeof z);
    return z;
}

/* above 0 and below 1e30 */
static int usable_b(uint32_t u) {
    return u - 1u < USABLE_END - 1u;
}

static int usable(float z) {
    return usable_b(zbits(z));
}

static int bits_cmp(const void* a, const void* b) {
    uint32_t x = *(const uint32_t*)a, y = *(const uint32_t*)b;
    return x < y ? -1 : x > y;
}

static void add_flat(uint32_t z) {
    if (nflats == maxflats) {
        int n = maxflats ? maxflats * 2 : 1024;
        uint32_t* p = realloc(flats, (size_t)n * sizeof *p);
        if (p == NULL) {
            return;
        }
        flats = p, maxflats = n;
    }
    flats[nflats++] = z;
}

static int flat_tri(const RD_TRI* t) {
    uint32_t a = zbits(t->v[0].z);
    return a == zbits(t->v[1].z) && a == zbits(t->v[2].z);
}

/* the 3D triangles' range, the flat ones' 1/w (scan's) laid out in slots outside it */
static void layout_depth(uint32_t lo, uint32_t hi) {
    int i, n, has3d = hi != 0;
    double units_all = ldexp(1.0, depth_bits > 24 ? 24 : depth_bits) - 1.0, room;
    unit_d = 1.0 / units_all;
    lo3b = lo, hi3b = hi;
    lo3 = has3d ? zfloat(lo) : 1e30f, hi3 = zfloat(hi);
    for (i = 0; i < 3; i++) {
        uint32_t z = zbits(fr->background[i].z);
        if (usable_b(z)) {
            add_flat(z);
        }
    }
    for (i = n = 0; i < nflats && has3d; i++) {
        if (flats[i] < lo || flats[i] > hi) {
            flats[n++] = flats[i];
        }
    }
    nflats = has3d ? n : nflats;
    qsort(flats, (size_t)nflats, sizeof *flats, bits_cmp);
    for (i = n = 0; i < nflats; i++) {
        if (n == 0 || flats[i] != flats[n - 1]) {
            flats[n++] = flats[i];
        }
    }
    nflats = n;
    for (nbelow = 0; nbelow < nflats && has3d && flats[nbelow] < lo; nbelow++) {
    }
    if (!has3d) {
        nbelow = nflats;
    }
    /* below: slots 1 to nbelow; the 3D range; above: the rest, the last a step under 1 */
    room = units_all - (double)(nflats + 2) * SLOT_STEP;
    d_lo3 = (double)(nbelow + 1) * SLOT_STEP * unit_d;
    d_scale3 = has3d && hi > lo ? room * unit_d / ((double)hi3 - lo3) : 0.0;
    scaled3 = has3d && hi > lo;
    z3_b = (less ? -2.0 : 2.0) * d_scale3;
    z3_a = (less ? 1.0 - d_lo3 : d_lo3) * 2.0 - 1.0 - lo3 * z3_b;
}

/* a 1/w's depth, 0 to 1 */
static double depth_up(float z) {
    uint32_t u = zbits(z);
    int lo, hi;
    if (!usable_b(u)) {
        return z > 0.0f ? 1.0 : 0.0;
    }
    if (scaled3 && u >= lo3b && u <= hi3b) {
        return d_lo3 + ((double)z - lo3) * d_scale3;
    }
    lo = 0, hi = nflats - 1;
    while (lo <= hi) {
        int m = (lo + hi) / 2;
        if (flats[m] == u) {
            return (m < nbelow ? (double)((m + 1) * SLOT_STEP) : 1.0 / unit_d + (double)((m - nflats) * SLOT_STEP)) * unit_d;
        }
        if (flats[m] < u) {
            lo = m + 1;
        } else {
            hi = m - 1;
        }
    }
    /* not laid out (a 3D triangle's 1/w past the range): its nearest slot's side */
    return u < lo3b ? d_lo3 : d_lo3 + ((double)hi3 - lo3) * d_scale3;
}

/* less: nearer is less, the way Pinedra's core rejects hidden pixels early */
static double depth_of(float z) {
    return less ? 1.0 - depth_up(z) : depth_up(z);
}

/* ---- fog ---- */

#define RAMP 512

static int fog_on;
static float ramp_w0, ramp_k;       /* s = (w - w0) * k */
static uint8_t ramp[RAMP * 4], ramp_sent[RAMP * 4];
static uint16_t ramp_nz[RAMP + 1];  /* the ramp's non-zero entries before each */

/* rd_soft.c's: the fog table at 1/w z */
static float fog_value(float z) {
    float q = frame_fog->density * z, m, pos;
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
        return frame_fog->table[127];
    }
    return frame_fog->table[i] + (frame_fog->table[i + 1] - frame_fog->table[i]) * (pos - (float)i);
}

static uint8_t* st_fogged;
static int any_fogged;              /* a state fogged this frame */         /* each state's fogged(), this frame */
static int maxst;

static int grow(void** p, int* room, int want, size_t size);

static int fogged(const RD_STATE* s) {
    return (s->fog == RD_FOG_TABLE && fog_on && !rd_q.nofog) || s->fog == RD_FOG_TABLE2;
}

/* the ramp over the fogged triangles' w where the table's value changes with it */
/* the fogged triangles' 1/w range (scan's) */
static void fog_ramp(uint32_t lo, uint32_t hi) {
    float wlo, whi, d = frame_fog->density, w0, w1;
    int i;
    if (hi == 0) {
        return;
    }
    wlo = 1.0f / zfloat(hi), whi = 1.0f / zfloat(lo);
    w0 = d > 0.0f && wlo < d / 256.0f ? d / 256.0f : wlo;
    w1 = d > 0.0f && whi > d ? d : whi;
    if (!(w1 > w0)) {
        w1 = w0 * 1.0001f + 1e-6f;
    }
    ramp_w0 = w0;
    ramp_k = 1.0f / (w1 - w0);
    {
        static float made[2 + 128 + 1];
        float now[2 + 128 + 1];
        now[0] = w0, now[1] = w1, now[2] = frame_fog->density;
        memcpy(&now[3], frame_fog->table, sizeof frame_fog->table);
        if (memcmp(now, made, sizeof now) == 0) {
            return;
        }
        memcpy(made, now, sizeof now);
    }
    for (i = 0; i < RAMP; i++) {
        float w = w0 + ((float)i + 0.5f) * (w1 - w0) / RAMP;
        int f = (int)(fog_value(1.0f / w) * 255.0f + 0.5f);
        ramp[4 * i] = ramp[4 * i + 1] = ramp[4 * i + 2] = ramp[4 * i + 3] = (uint8_t)f;
    }
    for (i = 0; i < RAMP; i++) {
        ramp_nz[i + 1] = (uint16_t)(ramp_nz[i] + (ramp[4 * i] != 0));
    }
    if (memcmp(ramp, ramp_sent, sizeof ramp) != 0) {
        memcpy(ramp_sent, ramp, sizeof ramp);
        active(0);
        bind(0, fog_tex);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, RAMP, 1, GL_RGBA, GL_UNSIGNED_BYTE, ramp);
    }
}

/* the fog the ramp's linear filter gives from s0 to s1 is all 0: the fog unit would change nothing */
static int fog_none(float s0, float s1) {
    float a = s0 * RAMP - 0.5f, b = s1 * RAMP - 0.5f;
    int i, j;
    if (!(a > -2.0f && b < RAMP + 1.0f)) {
        return 0;
    }
    i = a < 0.0f ? 0 : (int)a;
    j = (b < 0.0f ? 0 : (int)b) + 1;
    j = j > RAMP - 1 ? RAMP - 1 : j;
    i = i > RAMP - 1 ? RAMP - 1 : i;
    return ramp_nz[j + 1] == ramp_nz[i];
}

/* ---- the frame's scan ---- */

/* all of a triangle's cache lines, as a pass reaches the triangles some way before it: the A9 fetches none ahead of
   its own */
static void prefetch_tri(const RD_TRI* t) {
    const char* p = (const char*)t;
    __builtin_prefetch(p);
    __builtin_prefetch(p + 32);
    __builtin_prefetch(p + 64);
}

/* one pass over the lists (on the MiSTer each costs its memory's time): the 3D triangles' 1/w range, the flat
   ones' values, the fogged ones' range */
static void scan(void) {
    uint32_t lo = 0xFFFFFFFFu, hi = 0, flo = 0xFFFFFFFFu, fhi = 0, last = 0;
    int l, i;
    fog_on = 0;
    for (i = 0; i < 128; i++) {
        fog_on |= frame_fog->table[i] != 0.0f;
    }
    if (!grow((void**)&st_fogged, &maxst, fr->nstates + 1, 1)) {
        return;
    }
    any_fogged = 0;
    for (i = 0; i < fr->nstates; i++) {
        st_fogged[i] = (uint8_t)fogged(&fr->states[i]);
        any_fogged |= st_fogged[i];
    }
    nflats = 0;
    for (l = 0; l < RD_LISTS; l++) {
        const RD_TRI* t = fr->tris[l];
        int n = fr->ntris[l];
        for (i = 0; i < n; i++, t++) {
            uint32_t a = zbits(t->v[0].z), b = zbits(t->v[1].z), c = zbits(t->v[2].z);
            uint32_t ua = usable_b(a), ub = usable_b(b), uc = usable_b(c);
            prefetch_tri(t + 8);
            if (a != b || b != c) {
                if (ua) {
                    lo = a < lo ? a : lo, hi = a > hi ? a : hi;
                }
                if (ub) {
                    lo = b < lo ? b : lo, hi = b > hi ? b : hi;
                }
                if (uc) {
                    lo = c < lo ? c : lo, hi = c > hi ? c : hi;
                }
            } else if (ua && a != last) {
                add_flat(a);
                last = a;
            }
            if (st_fogged[t->state]) {
                if (ua) {
                    flo = a < flo ? a : flo, fhi = a > fhi ? a : fhi;
                }
                if (ub) {
                    flo = b < flo ? b : flo, fhi = b > fhi ? b : fhi;
                }
                if (uc) {
                    flo = c < flo ? c : flo, fhi = c > fhi ? c : fhi;
                }
            }
        }
    }
    layout_depth(lo, hi);
    fog_ramp(flo, fhi);
}

/* ---- the vertices and batches ---- */

typedef struct {
    float x, y, z, w;
    float u, v;
    uint32_t rgba;
    float s, t;                     /* the fog ramp's: sent only in a frame with fog */
} GV;

typedef struct {
    int first, count, state, list;  /* indices */
    uint32_t offset;
    int fog;                        /* fogged(), and the fog not 0 on it */
} BATCH;

static GV* verts;
static int nverts, maxverts;
static int vstride = sizeof(GV);    /* a vertex's bytes in verts this frame: without fog, none for s and t */
#define VERT(i) ((GV*)((char*)verts + (size_t)(i) * (size_t)vstride))

/* a copy of one of the two sizes, not a call */
static void put_vertex(GV* d, const GV* g) {
    if (vstride == (int)sizeof(GV)) {
        memcpy(d, g, sizeof(GV));
    } else {
        memcpy(d, g, offsetof(GV, s));
    }
}
static GLuint* ix;
static int nix, maxix;
static BATCH* batches;
static int nbatches, maxbatches;
static int drawn, solid, late, sent_verts;      /* the batches drawn; the first after the opaque ones; the background drawn late */

static int grow(void** p, int* room, int want, size_t size) {
    if (want > *room) {
        int n = *room ? *room : 4096;
        void* q;
        while (n < want) {
            n *= 2;
        }
        q = realloc(*p, (size_t)n * size);
        if (q == NULL) {
            return 0;
        }
        *p = q, *room = n;
    }
    return 1;
}

static uint32_t tri_offset(const RD_STATE* s, const RD_TRI* t) {
    return s->offset ? t->v[s->flat ? 2 : 0].offset & 0xFFFFFFu : 0;
}

/* every corner past one side of the screen, by more than a pixel (on the bits: on the MiSTer's A9 a float comparison
   stalls) */
static int below_m1(uint32_t b) {
    return b - 0xBF800001u <= 0xFF800000u - 0xBF800001u;
}

static int above(uint32_t b, uint32_t c) {
    return b - (c + 1) <= 0x7F800000u - (c + 1);
}

static int unseen(const RD_TRI* t) {
    uint32_t x0 = zbits(t->v[0].x), x1 = zbits(t->v[1].x), x2 = zbits(t->v[2].x);
    uint32_t y0 = zbits(t->v[0].y), y1 = zbits(t->v[1].y), y2 = zbits(t->v[2].y);
    return (below_m1(x0) & below_m1(x1) & below_m1(x2)) | (below_m1(y0) & below_m1(y1) & below_m1(y2)) |
           (above(x0, 0x44204000u) & above(x1, 0x44204000u) & above(x2, 0x44204000u)) |
           (above(y0, 0x43F08000u) & above(y1, 0x43F08000u) & above(y2, 0x43F08000u));
}

/* rd.c's area test kept it, the same sum (its stays: at least the cull value across, facing the drawn way) and no
   corner so far out that the sum could be a NaN, which dropped() drops: dropped() would keep it */
static int kept(const RD_STATE* s, const RD_TRI* t) {
    uint32_t m = 0;
    int k;
    if (s->cull == 0 || !(t->stays & 1)) {
        return 0;
    }
    for (k = 0; k < 3; k++) {
        uint32_t x = zbits(t->v[k].x) & 0x7FFFFFFFu, y = zbits(t->v[k].y) & 0x7FFFFFFFu;
        m = x > m ? x : m;
        m = y > m ? y : m;
    }
    return m < 0x5E800000u;     /* 2^62: the products stay finite */
}

/* rd_soft's setup_tri's: no pixel, or culled by its state */
static int dropped(const RD_STATE* s, const RD_TRI* t) {
    const RD_VTX *a = &t->v[0], *b = &t->v[1], *c = &t->v[2];
    double area = ((double)b->x - a->x) * ((double)c->y - a->y) - ((double)b->y - a->y) * ((double)c->x - a->x);
    double mag = fabs(area);
    /* zero, NaN (which fails the comparison) or under the cull value; or facing the culled way. In the float
       registers: on the A9 a move to the core's waits for the sum */
    if (s->cull != 0 ? !(mag >= RD_CULL_VAL) : !(mag > 0.0)) {
        return 1;
    }
    return (s->cull == 2 && area < 0.0) || (s->cull == 3 && area > 0.0);
}

/* floor(x * snap + 0.5), without libm's call where it fits an int */
static double grid(float x) {
    double v = x * (double)snap + 0.5;
    int i;
    if (!(v > -1e9 && v < 1e9)) {
        return floor(v);
    }
    i = (int)v;
    return (double)(i - ((double)i > v));
}

/* A flat triangle's corners on the GPU's grid, its texture coordinates where its plane has them there: the
   GPU's own snap would move its corners and leave the coordinates, and a texel a point-sampled sprite shows
   at a pixel would change where its coordinate there is near a texel's edge */
/* x * snap a whole number, on x's bits (snap a power of two from 1 to 2^16) */
static int on_grid(float x, int snap_log2) {
    uint32_t u;
    int e;
    memcpy(&u, &x, sizeof u);
    e = (int)(u >> 23 & 255) - 127 + snap_log2;     /* x * snap's exponent */
    if ((u & 0x7FFFFFFFu) == 0 || e >= 23) {
        return (u & 0x7F800000u) != 0x7F800000u;
    }
    return e >= 0 && (u & ((1u << (23 - e)) - 1)) == 0;
}

static int snap_log2(void) {
    int k;
    for (k = 0; k <= 16; k++) {
        if (snap == (float)(1 << k)) {
            return k;
        }
    }
    return -1;
}

static void snapped(const RD_TRI* t, GV* g) {
    const RD_VTX *a = &t->v[0], *b = &t->v[1], *c = &t->v[2];
    static int sl = -2;
    if (sl == -2) {
        sl = snap_log2();
    }
    if (sl >= 0 && on_grid(a->x, sl) && on_grid(a->y, sl) && on_grid(b->x, sl) && on_grid(b->y, sl) &&
        on_grid(c->x, sl) && on_grid(c->y, sl)) {
        int k;
        for (k = 0; k < 3; k++) {
            g[k].x = t->v[k].x, g[k].y = t->v[k].y, g[k].u = t->v[k].u, g[k].v = t->v[k].v;
        }
        return;
    }
    double area = ((double)b->x - a->x) * ((double)c->y - a->y) - ((double)b->y - a->y) * ((double)c->x - a->x);
    double inv = 1.0 / area, step = 1.0 / snap;
    int k;
    for (k = 0; k < 3; k++) {
        double x = grid(t->v[k].x) * step, y = grid(t->v[k].y) * step;
        double w0 = (((double)c->x - b->x) * (y - b->y) - ((double)c->y - b->y) * (x - b->x)) * inv;
        double w1 = (((double)a->x - c->x) * (y - c->y) - ((double)a->y - c->y) * (x - c->x)) * inv;
        double w2 = 1.0 - w0 - w1;
        g[k].x = (float)x, g[k].y = (float)y;
        g[k].u = (float)(w0 * a->u + w1 * b->u + w2 * c->u);
        g[k].v = (float)(w0 * a->v + w1 * b->v + w2 * c->v);
    }
}

static const RD_TRI* last_tri;     /* the triangle put last, and its corners' indices */
static GLuint last_ix[3];

static int same_vertex(const GV* a, const GV* b) {
    const uint32_t *x = (const uint32_t*)a, *y = (const uint32_t*)b;
    uint32_t d = (x[0] ^ y[0]) | (x[1] ^ y[1]) | (x[2] ^ y[2]) | (x[3] ^ y[3]) | (x[4] ^ y[4]) | (x[5] ^ y[5]) |
                 (x[6] ^ y[6]);
    if (vstride == (int)sizeof(GV)) {
        d |= (x[7] ^ y[7]) | (x[8] ^ y[8]);
    }
    return d == 0;
}

/* g's index; a corner the list's triangle before has (its share) and that came out the same is sent once */
static GLuint vertex(const RD_TRI* t, int k, const GV* g) {
    int c = k < 2 && last_tri == t - 1 ? (t->share >> 2 * k & 3) - 1 : -1;
    if (c >= 0 && same_vertex(VERT(last_ix[c]), g)) {
        return last_ix[c];
    }
    put_vertex(VERT(nverts), g);
    return (GLuint)nverts++;
}

/* two states the GL draws alike: culling, flat colours and what drew them are the triangles' own, done here */
static int same_gl_state(const RD_STATE* a, const RD_STATE* b) {
    RD_STATE x = *a, y = *b;
    x.cull = y.cull = 0, x.flat = y.flat = 0, x.origin = y.origin = 0;
    return rd_same_state(&x, &y);
}

/* a 1/w in the 3D triangles' range, on its bits */
static int in_3d(uint32_t u) {
    return u - lo3b <= hi3b - lo3b;
}

/* a 3D corner's w, and its depth times w: out of line, where a corner is outside the 3D range */
static __attribute__((noinline, cold)) float depth_w(uint32_t u, float* z) {
    float w = 1.0f / zfloat(u);
    if (scaled3 && in_3d(u)) {
        *z = (float)((z3_a + zfloat(u) * z3_b) * w);
    } else {
        *z = (float)((depth_of(zfloat(u)) * 2.0 - 1.0) * w);
    }
    return w;
}

/* a 3D corner's all but its depth */
static void corner(GV* g, const RD_VTX* v, float w, uint32_t argb) {
    g->x = v->x * w, g->y = v->y * w, g->w = w;
    g->u = v->u, g->v = v->v;
    g->s = (w - ramp_w0) * ramp_k, g->t = 0.5f;
    g->rgba = argb_to_rgba(argb);
}

static void draw_batches(int upto);

/* order: a translucent triangle's corners in window space, x and y as they are, the window depth and 1/w for the
   driver's rhw (its order key); the depth and fog as put_tri makes them */
static void win_tri(const RD_STATE* s, const RD_TRI* t, int flat, GV* g) {
    int k;
    if (flat) {
        static uint32_t last_z = 0xFFFFFFFFu, last_frame;
        static float last_d, last_f;
        uint32_t zb = zbits(t->v[0].z);
        if (zb != last_z || last_frame != frame_no) {
            last_z = zb, last_frame = frame_no;
            last_d = (float)depth_of(t->v[0].z);
            last_f = (1.0f / t->v[0].z - ramp_w0) * ramp_k;
        }
        if (snap > 0.0f) {
            snapped(t, g);
        }
        for (k = 0; k < 3; k++, g++) {
            const RD_VTX* v = &t->v[k];
            uint32_t c = s->flat ? t->v[2].base : v->base;
            if (snap <= 0.0f) {
                g->x = v->x, g->y = v->y, g->u = v->u, g->v = v->v;
            }
            g->z = last_d, g->w = v->z;
            g->s = last_f, g->t = 0.5f;
            g->rgba = argb_to_rgba(s->use_alpha ? c : c | 0xFF000000u);
        }
        return;
    }
    for (k = 0; k < 3; k++, g++) {
        const RD_VTX* v = &t->v[k];
        uint32_t c = s->flat ? t->v[2].base : v->base;
        g->x = v->x, g->y = v->y, g->u = v->u, g->v = v->v, g->w = v->z;
        g->z = scaled3 && in_3d(zbits(v->z)) ? (float)((z3_a + v->z * z3_b) * 0.5 + 0.5) : (float)depth_of(v->z);
        if (any_fogged) {
            g->s = (1.0f / v->z - ramp_w0) * ramp_k, g->t = 0.5f;
        }
        g->rgba = argb_to_rgba(s->use_alpha ? c : c | 0xFF000000u);
    }
}

#define BATCH_TRIS 64             /* a batch's most: its vertices stay in the cache for the GL's copy */

/* check: dropped as rd_soft drops it (not a part of one cut, which was) */
static void put_tri(int list, const RD_TRI* t, int check) {
    const RD_STATE* s = &fr->states[t->state];
    int flat = flat_tri(t), k, fog;
    uint32_t off;
    GV tri[3], *g = tri;
    BATCH* b;
    if ((check && !kept(s, t) && dropped(s, t)) || unseen(t)) {
        return;
    }
    if (!usable(t->v[0].z) || !usable(t->v[1].z) || !usable(t->v[2].z)) {
        rd_gl1_stats.skipped++;
        return;
    }
    if (!grow((void**)&verts, &maxverts, nverts + 3, sizeof *verts) ||
        !grow((void**)&ix, &maxix, nix + 3, sizeof *ix)) {
        return;
    }
    if (ord_frame && list == RD_TRANS) {
        win_tri(s, t, flat, tri);
    } else if (flat) {
        /* one 1/w: one depth and one fog coordinate */
        static uint32_t last_z = 0xFFFFFFFFu, last_frame;
        static float last_d, last_f;
        uint32_t zb = zbits(t->v[0].z);
        float d, f;
        if (zb != last_z || last_frame != frame_no) {
            last_z = zb, last_frame = frame_no;
            last_d = (float)(depth_of(t->v[0].z) * 2.0 - 1.0);
            last_f = (1.0f / t->v[0].z - ramp_w0) * ramp_k;
        }
        d = last_d, f = last_f;
        if (snap > 0.0f) {
            snapped(t, g);
        }
        for (k = 0; k < 3; k++, g++) {
            const RD_VTX* v = &t->v[k];
            uint32_t c = s->flat ? t->v[2].base : v->base;
            if (snap <= 0.0f) {
                g->x = v->x, g->y = v->y, g->u = v->u, g->v = v->v;
            }
            g->z = d, g->w = 1.0f;
            g->s = f, g->t = 0.5f;
            g->rgba = argb_to_rgba(s->use_alpha ? c : c | 0xFF000000u);
        }
    } else {
        /* 1/w's bits tested before its divisions, the corners a step at a time so that their divisions and
           multiplies overlap: on the A9 a move from a float register waits for those before it */
        const RD_VTX* v = t->v;
        uint32_t u0 = zbits(v[0].z), u1 = zbits(v[1].z), u2 = zbits(v[2].z);
        uint32_t c0 = s->flat ? v[2].base : v[0].base, c1 = s->flat ? v[2].base : v[1].base, c2 = v[2].base;
        float w0, w1, w2;
        if (scaled3 && in_3d(u0) & in_3d(u1) & in_3d(u2)) {
            float z0 = zfloat(u0), z1 = zfloat(u1), z2 = zfloat(u2);
            w0 = 1.0f / z0, w1 = 1.0f / z1, w2 = 1.0f / z2;
            tri[0].z = (float)((z3_a + z0 * z3_b) * w0);
            tri[1].z = (float)((z3_a + z1 * z3_b) * w1);
            tri[2].z = (float)((z3_a + z2 * z3_b) * w2);
        } else {
            w0 = depth_w(u0, &tri[0].z), w1 = depth_w(u1, &tri[1].z), w2 = depth_w(u2, &tri[2].z);
        }
        corner(&tri[0], &v[0], w0, s->use_alpha ? c0 : c0 | 0xFF000000u);
        corner(&tri[1], &v[1], w1, s->use_alpha ? c1 : c1 | 0xFF000000u);
        corner(&tri[2], &v[2], w2, s->use_alpha ? c2 : c2 | 0xFF000000u);
    }
    if (atlas && st_at[t->state] >= 0) {
        float ax = (float)(st_at[t->state] % RD_ATLAS), ay = (float)(st_at[t->state] / RD_ATLAS);
        for (k = 0; k < 3; k++) {
            tri[k].u = (ax + tri[k].u * RD_TILE) * (1.0f / RD_ATLAS);
            tri[k].v = (ay + tri[k].v * RD_TILE) * (1.0f / RD_ATLAS);
        }
    }
    fog = st_fogged[t->state];
    if (fog && s->fog == RD_FOG_TABLE) {
        float s0 = tri[0].s, s1 = tri[0].s;
        for (k = 1; k < 3; k++) {
            s0 = tri[k].s < s0 ? tri[k].s : s0;
            s1 = tri[k].s > s1 ? tri[k].s : s1;
        }
        fog = !fog_none(s0, s1);
    }
    off = tri_offset(s, t);
    b = nbatches > 0 ? &batches[nbatches - 1] : NULL;
    if (b == NULL || b->count >= 3 * BATCH_TRIS || b->list != list || b->offset != off || b->fog != fog ||
        (atlas ? st_key[b->state] != st_key[t->state]
               : b->state != t->state && !same_gl_state(&fr->states[b->state], s))) {
        if (!grow((void**)&batches, &maxbatches, nbatches + 1, sizeof *batches)) {
            return;
        }
        b = &batches[nbatches++];
        b->first = nix;
        b->count = 0;
        b->state = t->state;
        b->list = list;
        b->offset = off;
        b->fog = fog;
        draw_batches(nbatches - 1);
    }
    for (k = 0; k < 3; k++) {
        ix[nix + k] = vertex(t, k, &tri[k]);
    }
    memcpy(last_ix, &ix[nix], sizeof last_ix);
    last_tri = t;
    nix += 3;
    b->count += 3;
    rd_gl1_stats.tris++;
}

/* The translucent list far to near, a triangle at a time by the middle of its 1/w, in order where level. A deep
   triangle is cut at the shallow ones' (sprites') depths, so that each part sorts right against them. */
#define MAX_CUTS 32         /* a triangle's; and parts past half the list's count and 256 aren't cut */

typedef struct {
    float key;
    int order;              /* the triangle's, times 256, and its part's */
    const RD_TRI* t;        /* NULL: parts[part], or nothing when part is -1 */
    int part;
} ITEM;

static ITEM* items;
static int nitems, maxitems;
static RD_TRI* parts;
static int nparts, maxparts;
static float* cuts;
static int maxcuts;

typedef struct {
    uint32_t key;           /* key_bits */
    uint32_t ref;           /* the triangle's place in the list, or PART and its part's */
} SORTED;

#define PART 0x80000000u

static SORTED *sorted, *sorted_tmp;
static int maxsorted, maxsorted_tmp;

static uint32_t key_bits(float key) {
    uint32_t u;
    memcpy(&u, &key, sizeof u);
    return u;               /* keys are 0 or more: their bits rise with them */
}

/* a radix sort's digits: 9 bits, 9, 9 and 5 */
static void count_digits(int (*count)[512], uint32_t u) {
    count[0][u & 511]++, count[1][u >> 9 & 511]++, count[2][u >> 18 & 511]++, count[3][u >> 27]++;
}

static void put_sorted(SORTED* a, int* n, int (*count)[512], float key, uint32_t ref) {
    uint32_t u = key_bits(key);
    a[*n].key = u, a[(*n)++].ref = ref;
    count_digits(count, u);
}

/* items[0, k) the triangles in order, the parts of the cut ones after them in their order: all in order, then by key
   (a stable radix sort, a digit a pass: few enough places to write to for the cache; a digit all share left out) */
static const SORTED* sort_items(int k, int* count_out) {
    static int count[4][512];
    const RD_TRI* tris = fr->tris[RD_TRANS];
    SORTED *a, *b, *t;
    int i, j = k, n = 0, p;
    if (!grow((void**)&sorted, &maxsorted, nitems, sizeof *sorted) ||
        !grow((void**)&sorted_tmp, &maxsorted_tmp, nitems, sizeof *sorted_tmp)) {
        return NULL;
    }
    a = sorted, b = sorted_tmp;
    memset(count, 0, sizeof count);
    for (i = 0; i < k; i++) {
        if (items[i].t != NULL) {
            put_sorted(a, &n, count, items[i].key, (uint32_t)(items[i].t - tris));
        } else {
            for (; j < nitems && items[j].order >> 8 == items[i].order >> 8; j++) {
                if (items[j].part >= 0) {
                    put_sorted(a, &n, count, items[j].key, PART | (uint32_t)items[j].part);
                }
            }
        }
    }
    for (p = 0; p < 4 && n > 0; p++) {
        int* c = count[p];
        int sum = 0, sh = 9 * p;
        uint32_t mask = p < 3 ? 511 : 31;
        if (c[a[0].key >> sh & mask] == n) {
            continue;
        }
        for (i = 0; i < 512; i++) {
            int x = c[i];
            c[i] = sum;
            sum += x;
        }
        for (i = 0; i < n; i++) {
            b[c[a[i].key >> sh & mask]++] = a[i];
        }
        t = a, a = b, b = t;
    }
    *count_out = n;
    return a;
}

static void add_item(float key, int order, const RD_TRI* t, int part) {
    if (grow((void**)&items, &maxitems, nitems + 1, sizeof *items)) {
        ITEM* e = &items[nitems++];
        e->key = key, e->order = order, e->t = t, e->part = part;
    }
}

/* x, y and 1/w are linear on the screen, the rest divided by w */
static RD_VTX cut_edge(const RD_VTX* a, const RD_VTX* b, double at) {
    RD_VTX r;
    double t, wa, wb;
    int sh;
    if (a->z > b->z || (a->z == b->z && (a->x > b->x || (a->x == b->x && a->y > b->y)))) {
        const RD_VTX* c = a;    /* the same point from either side of an edge */
        a = b;
        b = c;
    }
    t = (at - a->z) / ((double)b->z - a->z);
    r.x = (float)(a->x + t * ((double)b->x - a->x));
    r.y = (float)(a->y + t * ((double)b->y - a->y));
    r.z = (float)at;
    wb = b->z * t / at;
    wa = 1.0 - wb;
    r.u = (float)(a->u * wa + b->u * wb);
    r.v = (float)(a->v * wa + b->v * wb);
    r.base = r.offset = 0;
    for (sh = 0; sh < 32; sh += 8) {
        double c = (a->base >> sh & 255) * wa + (b->base >> sh & 255) * wb + 0.5;
        double o = (a->offset >> sh & 255) * wa + (b->offset >> sh & 255) * wb + 0.5;
        r.base |= (uint32_t)(c < 0 ? 0 : c > 255 ? 255 : c) << sh;
        r.offset |= (uint32_t)(o < 0 ? 0 : o > 255 ? 255 : o) << sh;
    }
    return r;
}

/* p cut at 1/w at into the parts below and above */
static void split_poly(const RD_VTX* p, int n, RD_VTX* lo, int* nlo, RD_VTX* hi, int* nhi, double at) {
    int i, nl = 0, nh = 0;
    for (i = 0; i < n; i++) {
        const RD_VTX* a = &p[i];
        const RD_VTX* b = &p[i + 1 < n ? i + 1 : 0];
        double ka = a->z - at, kb = b->z - at;
        int la = ka <= 0, lb = kb <= 0, ha = ka >= 0, hb = kb >= 0;
        RD_VTX c;
        if (la != lb || ha != hb) {
            c = cut_edge(a, b, at);
        }
        if (la) {
            lo[nl++] = *a;
        }
        if (la != lb) {
            lo[nl++] = c;
        }
        if (ha) {
            hi[nh++] = *a;
        }
        if (ha != hb) {
            hi[nh++] = c;
        }
    }
    *nlo = nl;
    *nhi = nh;
}

/* a polygon of triangle t's, a fan sorted as one at its middle depth */
static void add_poly(const RD_TRI* t, const RD_VTX* p, int n, int order) {
    float lo = p[0].z, hi = p[0].z;
    int i;
    for (i = 1; i < n; i++) {
        lo = p[i].z < lo ? p[i].z : lo;
        hi = p[i].z > hi ? p[i].z : hi;
    }
    for (i = 2; i < n; i++) {
        RD_TRI* r;
        if (!grow((void**)&parts, &maxparts, nparts + 1, sizeof *parts)) {
            return;
        }
        r = &parts[nparts];
        r->state = t->state;
        r->share = 0;
        r->stays = 1;
        r->v[0] = p[0], r->v[1] = p[i - 1], r->v[2] = p[i];
        add_item((lo + hi) * 0.5f, order + (i - 2), NULL, nparts++);
    }
}

/* its nearest and farthest 1/w; 0 when one isn't usable */
static int z_range(const RD_TRI* t, float* lo, float* hi) {
    uint32_t a = zbits(t->v[0].z), b = zbits(t->v[1].z), c = zbits(t->v[2].z), l, h;
    if (!usable_b(a) || !usable_b(b) || !usable_b(c)) {
        return 0;
    }
    l = a < b ? a : b, h = a > b ? a : b;
    l = c < l ? c : l, h = c > h ? c : h;
    *lo = zfloat(l), *hi = zfloat(h);
    return 1;
}

/* item i's triangle cut at cuts[a, b) */
static void cut_item(int i, int a, int b) {
    const RD_TRI* t = items[i].t;
    RD_VTX buf[2][12], part[12], *rest = buf[0];
    int j, nr = 3, np;
    memcpy(rest, t->v, sizeof t->v);
    if (fr->states[t->state].flat) {
        rest[0].base = rest[1].base = rest[2].base;     /* a flat one's colours on every corner */
        rest[0].offset = rest[1].offset = rest[2].offset;
    }
    for (j = a; j < b && nr >= 3; j++) {
        RD_VTX* next = rest == buf[0] ? buf[1] : buf[0];
        split_poly(rest, nr, part, &np, next, &nr, cuts[j]);
        rest = next;
        if (np >= 3) {
            add_poly(t, part, np, items[i].order + (j - a) * 2);
        }
    }
    if (nr >= 3) {
        add_poly(t, rest, nr, items[i].order + (b - a) * 2);
    }
    items[i].t = NULL;
    items[i].part = -1;
}

typedef struct {
    float x0, y0, x1, y1, lo, hi;
} SHAL;

static SHAL* shal;
static int maxshal;

static float min_f(float a, float b) {
    return b < a ? b : a;
}

static float max_f(float a, float b) {
    return b > a ? b : a;
}

static void bounds(const RD_TRI* t, SHAL* b) {
    const RD_VTX* v = t->v;
    b->x0 = min_f(v[0].x, min_f(v[1].x, v[2].x));
    b->x1 = max_f(v[0].x, max_f(v[1].x, v[2].x));
    b->y0 = min_f(v[0].y, min_f(v[1].y, v[2].y));
    b->y1 = max_f(v[0].y, max_f(v[1].y, v[2].y));
}

static SHAL* shal_tmp;
static int maxshal_tmp;

typedef struct {
    uint32_t k, i;          /* a key and where its record is */
} RKEY;

static RKEY *rkey, *rkey_tmp;
static int maxrkey, maxrkey_tmp;

static int rkeys(int n) {
    return grow((void**)&rkey, &maxrkey, n, sizeof *rkey) && grow((void**)&rkey_tmp, &maxrkey_tmp, n, sizeof *rkey_tmp);
}

/* rkey[0, n) by k, its least base: a stable radix sort, a digit a pass (few enough places to write to for the cache),
   as many as k's range needs, a digit all share left out */
static RKEY* sort_keys(int n, uint32_t base, uint32_t range) {
    static int count[4][512];
    RKEY *a = rkey, *b = rkey_tmp, *t;
    int i, p, np = 1;
    while (np < 4 && range >> 9 * np != 0) {
        np++;
    }
    memset(count, 0, sizeof count);
    for (i = 0; i < n; i++) {
        count_digits(count, a[i].k - base);
    }
    for (p = 0; p < np && n > 0; p++) {
        int* c = count[p];
        int sum = 0, sh = 9 * p;
        uint32_t mask = p < 3 ? 511 : 31;
        if (c[(a[0].k - base) >> sh & mask] == n) {
            continue;
        }
        for (i = 0; i < 512; i++) {
            int x = c[i];
            c[i] = sum;
            sum += x;
        }
        for (i = 0; i < n; i++) {
            b[c[(a[i].k - base) >> sh & mask]++] = a[i];
        }
        t = a, a = b, b = t;
    }
    return a;
}

/* shal[0, n)'s order by nearest, then farthest: by the nearest's bits, then those level there by the farthest's */
static const RKEY* sort_shal(int n) {
    const RKEY* r;
    RKEY* a = rkey;
    uint32_t lo = ~0u, hi = 0;
    int i, j, m;
    for (i = 0; i < n; i++) {
        uint32_t u = zbits(shal[i].lo);
        a[i].k = u, a[i].i = (uint32_t)i;
        lo = u < lo ? u : lo, hi = u > hi ? u : hi;
    }
    r = a = sort_keys(n, lo, hi - lo);
    /* those at one nearest by their farthest */
    for (i = 0; i < n; i = j) {
        for (j = i + 1; j < n && a[j].k == a[i].k; j++) {
        }
        for (m = i + 1; m < j; m++) {
            RKEY e = a[m];
            uint32_t h = zbits(shal[e.i].hi);
            int x;
            for (x = m; x > i && zbits(shal[a[x - 1].i].hi) > h; x--) {
                a[x] = a[x - 1];
            }
            a[x] = e;
        }
    }
    return r;
}

/* the shallow ones at one depth as one box (a character's sprites) */
static int merge_shallow(int n) {
    const RKEY* r;
    SHAL* t;
    int i, m = 0, room;
    if (!grow((void**)&shal_tmp, &maxshal_tmp, n, sizeof *shal_tmp) || !rkeys(n)) {
        return n;
    }
    r = sort_shal(n);
    for (i = 0; i < n; i++) {
        const SHAL* s = &shal[r[i].i];
        SHAL* g = &shal_tmp[m - 1];
        if (i + 8 < n) {
            __builtin_prefetch(&shal[r[i + 8].i]);
        }
        if (m > 0 && s->lo <= g->lo * 1.001f && fabsf(s->hi - g->hi) <= g->hi * 0.001f) {
            g->x0 = min_f(g->x0, s->x0), g->x1 = max_f(g->x1, s->x1);
            g->y0 = min_f(g->y0, s->y0), g->y1 = max_f(g->y1, s->y1);
            g->hi = max_f(g->hi, s->hi);
        } else {
            shal_tmp[m++] = *s;
        }
    }
    t = shal, shal = shal_tmp, shal_tmp = t;
    room = maxshal, maxshal = maxshal_tmp, maxshal_tmp = room;
    return m;
}

/* b joined to a when they are at the very same depths and no corner is a NaN: they would meet in merge_shallow
   anyway, side by side, and their box is the same either way */
static int same_shallow(SHAL* a, const SHAL* b) {
    if (zbits(a->lo) != zbits(b->lo) || zbits(a->hi) != zbits(b->hi) || a->x0 != a->x0 || a->x1 != a->x1 ||
        a->y0 != a->y0 || a->y1 != a->y1 || b->x0 != b->x0 || b->x1 != b->x1 || b->y0 != b->y0 || b->y1 != b->y1) {
        return 0;
    }
    a->x0 = min_f(a->x0, b->x0), a->x1 = max_f(a->x1, b->x1);
    a->y0 = min_f(a->y0, b->y0), a->y1 = max_f(a->y1, b->y1);
    return 1;
}

/* the first of the shallow ones (by their nearest) past z */
static int first_shal(int n, float z) {
    int lo = 0, hi = n;
    while (lo < hi) {
        int m = (lo + hi) / 2;
        if (shal[m].lo <= z) {
            lo = m + 1;
        } else {
            hi = m;
        }
    }
    return lo;
}

/* f's order as an unsigned number, -0 as 0; a NaN as end, which then fails the comparison it is in */
static uint32_t order_key(float f, uint32_t end) {
    uint32_t u;
    if (f != f) {
        return end;
    }
    f += 0.0f;
    memcpy(&u, &f, sizeof u);
    return u >> 31 ? ~u : u | 0x80000000u;
}

typedef struct {
    uint32_t x0, x1, y0, y1, lo;    /* a SHAL's, as order_key and zbits have them: no float compares */
} SHALK;

typedef struct {
    int item;
    float lo, hi;
    int live;                       /* not dropped */
    SHAL b;                         /* its box */
    uint32_t x0, x1, y0, y1;        /* b's, as order_key has them */
    int st, en;                     /* the shallow ones its depths reach */
    int at, nc;                     /* its cuts in swept[at, at + nc) */
} DEEP;

static SHALK* shalk;
static DEEP* deep;
static int maxshalk, maxdeep;

typedef struct {
    int deep;
    float cut;
} HIT;

static float* swept;
static HIT* hits;
static int* sweep_ord;
static uint32_t* sweep_box;
static int maxswept, maxhits, maxsweep_ord, maxsweep_box;

#define SWEEP_DEEP 512          /* deep ones past which the grid is quicker */
#define SWEEP_MAX (1 << 17)     /* cuts' room past which it is */

static int by_st(const void* a, const void* b) {
    return deep[*(const int*)a].st - deep[*(const int*)b].st;
}

/* Every live deep one's cuts at once, a shallow one at a time against the boxes of those whose depths reach it
   (side by side, for NEON), then each one's together: in the order deep_cuts's walk has them, to the same limit */
static int sweep_cuts(int ndeep, int n) {
    int i, j, a, nlive = 0, na = 0, next = 0, total = 0, end = 0, nhits = 0, *ord, *id;
    uint32_t *x0, *x1, *y0, *y1, *hit, ux0 = ~0u, ux1 = 0, uy0 = ~0u, uy1 = 0;
    for (i = 0; i < ndeep; i++) {
        DEEP* e = &deep[i];
        e->nc = 0;
        if (e->live && e->en > e->st) {
            int most = 2 * (e->en - e->st);
            total += most < 2 * n ? most : 2 * n;
            if (total > SWEEP_MAX) {
                return 0;
            }
        }
    }
    if (!grow((void**)&hits, &maxhits, total + 1, sizeof *hits) ||
        !grow((void**)&sweep_ord, &maxsweep_ord, 2 * ndeep + 1, sizeof *sweep_ord) ||
        !grow((void**)&sweep_box, &maxsweep_box, 5 * ndeep + 4, sizeof *sweep_box)) {
        return 0;
    }
    ord = sweep_ord, id = sweep_ord + ndeep;
    x0 = sweep_box, x1 = x0 + ndeep, y0 = x1 + ndeep, y1 = y0 + ndeep, hit = y1 + ndeep;
    for (i = 0; i < ndeep; i++) {
        if (deep[i].live && deep[i].en > deep[i].st) {
            ord[nlive++] = i;
        }
    }
    qsort(ord, (size_t)nlive, sizeof *ord, by_st);
    for (j = 0; next < nlive || na > 0; j++) {
        const SHALK* q;
        const SHAL* s;
        uint32_t qx0, qx1, qy0, qy1, any = 0;
        if (na == 0 && deep[ord[next]].st > j) {
            j = deep[ord[next]].st;
        }
        if (j >= end) {
            end = INT_MAX;
            ux0 = uy0 = ~0u, ux1 = uy1 = 0;
            for (a = na - 1; a >= 0; a--) {
                if (deep[id[a]].en <= j) {
                    na--;
                    id[a] = id[na], x0[a] = x0[na], x1[a] = x1[na], y0[a] = y0[na], y1[a] = y1[na];
                    continue;
                }
                if (deep[id[a]].en < end) {
                    end = deep[id[a]].en;
                }
                ux0 = x0[a] < ux0 ? x0[a] : ux0, ux1 = x1[a] > ux1 ? x1[a] : ux1;
                uy0 = y0[a] < uy0 ? y0[a] : uy0, uy1 = y1[a] > uy1 ? y1[a] : uy1;
            }
        }
        for (; next < nlive && deep[ord[next]].st <= j; next++) {
            const DEEP* e = &deep[ord[next]];
            id[na] = ord[next], x0[na] = e->x0, x1[na] = e->x1, y0[na] = e->y0, y1[na] = e->y1;
            na++;
            end = e->en < end ? e->en : end;
            ux0 = e->x0 < ux0 ? e->x0 : ux0, ux1 = e->x1 > ux1 ? e->x1 : ux1;
            uy0 = e->y0 < uy0 ? e->y0 : uy0, uy1 = e->y1 > uy1 ? e->y1 : uy1;
        }
        if (na == 0) {
            continue;
        }
        q = &shalk[j], s = &shal[j];
        qx0 = q->x0, qx1 = q->x1, qy0 = q->y0, qy1 = q->y1;
        if (qx1 < ux0 || qx0 > ux1 || qy1 < uy0 || qy0 > uy1) {
            continue;       /* it misses them all */
        }
        for (a = 0; a < na; a++) {
            uint32_t h = (qx1 >= x0[a]) & (qx0 <= x1[a]) & (qy1 >= y0[a]) & (qy0 <= y1[a]);
            hit[a] = h;
            any |= h;
        }
        if (!any) {
            continue;
        }
        for (a = na - 1; a >= 0; a--) {
            DEEP* e;
            if (!hit[a]) {
                continue;
            }
            e = &deep[id[a]];
            if (s->lo > e->lo && s->lo < e->hi) {
                hits[nhits].deep = id[a], hits[nhits++].cut = s->lo;
                e->nc++;
            }
            if (s->hi != s->lo && s->hi > e->lo && s->hi < e->hi) {
                hits[nhits].deep = id[a], hits[nhits++].cut = s->hi;
                e->nc++;
            }
            if (e->nc >= 2 * n - 1) {
                na--;
                id[a] = id[na], x0[a] = x0[na], x1[a] = x1[na], y0[a] = y0[na], y1[a] = y1[na];
            }
        }
    }
    if (!grow((void**)&swept, &maxswept, nhits + 1, sizeof *swept)) {
        return 0;
    }
    for (i = total = 0; i < ndeep; i++) {
        deep[i].at = total;
        total += deep[i].nc;
        deep[i].nc = 0;
    }
    for (i = 0; i < nhits; i++) {
        DEEP* e = &deep[hits[i].deep];
        swept[e->at + e->nc++] = hits[i].cut;
    }
    return 1;
}

/* The shallow ones by the cells of a grid on the screen their boxes reach, each cell's in their order: a deep one
   looks only in its own cells. A cell from a coordinate only rises with it, so two boxes that meet share a cell; a
   NaN, which fails every comparison, reaches the grid's end. */
#define GRID 16

static int* cell_list;
static int cell_start[GRID * GRID + 1], maxcell_list;
static uint32_t* stamp;
static uint32_t stamp_now;
static int maxstamp;

static int cell_of(float v, float scale, int nan) {
    float c;
    if (v != v) {
        return nan;
    }
    c = v * scale;
    return c < 1.0f ? 0 : c >= (float)(GRID - 1) ? GRID - 1 : (int)c;
}

static void cells(const SHAL* b, int* c) {
    const float sx = (float)GRID / RD_WIDTH, sy = (float)GRID / RD_HEIGHT;
    c[0] = cell_of(b->x0, sx, 0), c[1] = cell_of(b->x1, sx, GRID - 1);
    c[2] = cell_of(b->y0, sy, 0), c[3] = cell_of(b->y1, sy, GRID - 1);
}

static int grid_shallow(int nshal) {
    int j, x, y, c[4], total = 0;
    memset(cell_start, 0, sizeof cell_start);
    for (j = 0; j < nshal; j++) {
        cells(&shal[j], c);
        for (y = c[2]; y <= c[3]; y++) {
            for (x = c[0]; x <= c[1]; x++) {
                cell_start[y * GRID + x + 1]++;
            }
        }
    }
    for (j = 0; j < GRID * GRID; j++) {
        total += cell_start[j + 1];
        cell_start[j + 1] = total;
    }
    if (!grow((void**)&cell_list, &maxcell_list, total + 1, sizeof *cell_list) ||
        !grow((void**)&stamp, &maxstamp, nshal + 1, sizeof *stamp)) {
        return 0;
    }
    for (j = 0; j < nshal; j++) {
        cells(&shal[j], c);
        for (y = c[2]; y <= c[3]; y++) {
            for (x = c[0]; x <= c[1]; x++) {
                cell_list[cell_start[y * GRID + x]++] = j;
            }
        }
    }
    for (j = GRID * GRID; j > 0; j--) {
        cell_start[j] = cell_start[j - 1];
    }
    cell_start[0] = 0;
    memset(stamp, 0, (size_t)nshal * sizeof *stamp);
    stamp_now = 0;
    return 1;
}

/* the depths a deep one is cut at: those of the shallow ones in [st, en) its box meets, in no order; -1 when they
   reach the limit, where only their order says which are kept */
static int deep_cuts(const SHAL* b, float lo, float hi, int st, int en, int n, int grid) {
    uint32_t bx0 = order_key(b->x0, 0), bx1 = order_key(b->x1, ~0u), by0 = order_key(b->y0, 0), by1 = order_key(b->y1, ~0u);
    int nc = 0, x, y, c[4];
    if (!grid) {
        c[0] = c[2] = 0, c[1] = c[3] = 0;
    } else {
        cells(b, c);
        stamp_now++;
    }
    for (y = c[2]; y <= c[3]; y++) {
        for (x = c[0]; x <= c[1]; x++) {
            const int *p = grid ? &cell_list[cell_start[y * GRID + x]] : NULL, *e = grid ? &cell_list[cell_start[y * GRID + x + 1]] : NULL;
            int j = st;
            if (grid) {
                int l = 0, h = (int)(e - p);
                while (l < h) {
                    int m = (l + h) / 2;
                    if (p[m] < st) {
                        l = m + 1;
                    } else {
                        h = m;
                    }
                }
                p += l;
            }
            for (;;) {
                const SHALK* q;
                const SHAL* s;
                if (grid) {
                    if (p >= e || *p >= en) {
                        break;
                    }
                    j = *p++;
                    if (stamp[j] == stamp_now) {
                        continue;
                    }
                    stamp[j] = stamp_now;
                } else if (j >= en) {
                    break;
                }
                q = &shalk[j], s = &shal[j];
                if (!grid) {
                    j++;
                }
                if ((q->x1 < bx0) | (q->x0 > bx1) | (q->y1 < by0) | (q->y0 > by1)) {
                    continue;
                }
                if (s->lo > lo && s->lo < hi) {
                    cuts[nc++] = s->lo;
                }
                if (s->hi != s->lo && s->hi > lo && s->hi < hi) {
                    cuts[nc++] = s->hi;
                }
                if (nc >= 2 * n - 1) {
                    return grid ? -1 : nc;
                }
            }
        }
    }
    return nc;
}

/* e's box, and the shallow ones its depths reach */
static void deep_box(DEEP* e, int nshal) {
    const RD_TRI* t = items[e->item].t;
    uint32_t hb = zbits(e->hi);
    int h;
    e->live = !dropped(&fr->states[t->state], t);
    if (!e->live) {
        return;
    }
    bounds(t, &e->b);
    e->x0 = order_key(e->b.x0, 0), e->x1 = order_key(e->b.x1, ~0u);
    e->y0 = order_key(e->b.y0, 0), e->y1 = order_key(e->b.y1, ~0u);
    e->st = first_shal(nshal, e->lo / shallow_k);
    for (e->en = e->st, h = nshal; e->en < h;) {
        int mid = (e->en + h) / 2;
        if (shalk[mid].lo < hb) {
            e->en = mid + 1;
        } else {
            h = mid;
        }
    }
}

/* the driver's window vertices don't take a corner far past the screen (the attract's sky, millions of pixels off):
   a frame with a model's corner there is ordered here */
static int order_fits(void) {
    const RD_TRI* t = fr->tris[RD_TRANS];
    int n = fr->ntris[RD_TRANS], i, k;
    for (i = 0; i < n; i++) {
        prefetch_tri(&t[i + 8]);
        if (fr->states[t[i].state].origin != RD_SCENE) {
            continue;
        }
        for (k = 0; k < 3; k++) {
            if ((zbits(t[i].v[k].x) & 0x7FFFFFFFu) > 0x45800000u || (zbits(t[i].v[k].y) & 0x7FFFFFFFu) > 0x45800000u) {
                return 0;   /* past 4096 */
            }
        }
    }
    return 1;
}

static void put_trans(void) {
    const RD_TRI* tris = fr->tris[RD_TRANS];
    int n = fr->ntris[RD_TRANS], i, j, nshal = 0, ndeep = 0, k, grid, sweep, ahead;
    const SORTED* r;
    nitems = nparts = 0;
    if (ord_frame) {
        /* in the game's own order: the driver orders and cuts them */
        for (i = 0; i < n; i++) {
            prefetch_tri(&tris[i + 8]);
            put_tri(RD_TRANS, &tris[i], 1);
        }
        return;
    }
    if (!grow((void**)&cuts, &maxcuts, 2 * n + 1, sizeof *cuts) || !grow((void**)&shal, &maxshal, n + 1, sizeof *shal) ||
        !grow((void**)&deep, &maxdeep, n + 1, sizeof *deep)) {
        return;
    }
    for (i = 0; i < n; i++) {
        float lo, hi;
        prefetch_tri(&tris[i + 8]);
        if (!z_range(&tris[i], &lo, &hi)) {
            add_item(0.0f, i << 8, &tris[i], 0);
            continue;
        }
        if (hi <= lo * shallow_k) {
            bounds(&tris[i], &shal[nshal]);
            shal[nshal].lo = lo;
            shal[nshal].hi = hi;
            nshal += nshal == 0 || !same_shallow(&shal[nshal - 1], &shal[nshal]);
        }
        add_item((lo + hi) * 0.5f, i << 8, &tris[i], 0);
        if (hi > lo * deep_k && items[nitems - 1].t == &tris[i]) {
            deep[ndeep].item = nitems - 1, deep[ndeep].lo = lo, deep[ndeep++].hi = hi;
        }
    }
    k = nitems;
    nshal = ndeep == 0 ? 0 : nshal > 1 ? merge_shallow(nshal) : nshal;
    if (!grow((void**)&shalk, &maxshalk, nshal + 1, sizeof *shalk)) {
        nshal = 0;
    }
    for (j = 0; j < nshal; j++) {
        shalk[j].x0 = order_key(shal[j].x0, 0), shalk[j].x1 = order_key(shal[j].x1, ~0u);
        shalk[j].y0 = order_key(shal[j].y0, 0), shalk[j].y1 = order_key(shal[j].y1, ~0u);
        shalk[j].lo = zbits(shal[j].lo);
    }
    ahead = nshal > 0 && ndeep <= SWEEP_DEEP;
    for (i = 0; i < ndeep && ahead; i++) {
        deep_box(&deep[i], nshal);
    }
    sweep = ahead && sweep_cuts(ndeep, n);
    grid = nshal > 0 && !sweep && grid_shallow(nshal);
    /* each deep one cut at the depths of the shallow ones its box meets, while the parts last */
    for (i = 0; i < ndeep && nshal > 0 && nparts < n / 2 + 256; i++) {
        DEEP* e = &deep[i];
        int nc, m;
        if (!ahead) {
            deep_box(e, nshal);
        }
        if (!e->live) {
            continue;
        }
        if (sweep) {
            nc = e->nc;
            memcpy(cuts, &swept[e->at], (size_t)nc * sizeof *cuts);
        } else {
            nc = deep_cuts(&e->b, e->lo, e->hi, e->st, e->en, n, grid);
            if (nc < 0) {
                nc = deep_cuts(&e->b, e->lo, e->hi, e->st, e->en, n, 0);
            }
        }
        if (nc == 0) {
            continue;
        }
        for (j = 1; j < nc; j++) {
            float c = cuts[j];
            for (m = j; m > 0 && cuts[m - 1] > c; m--) {
                cuts[m] = cuts[m - 1];
            }
            cuts[m] = c;
        }
        for (j = m = 0; j < nc; j++) {
            if (m == 0 || cuts[j] > cuts[m - 1] * 1.001f) {
                cuts[m++] = cuts[j];
            }
        }
        if (m <= MAX_CUTS && nparts + 2 * m + 1 <= n / 2 + 256) {
            cut_item(e->item, 0, m);
        }
    }
    r = sort_items(k, &k);
    for (i = 0; r != NULL && i < k; i++) {
        uint32_t ref = r[i].ref;
        if (i + 6 < k && !(r[i + 6].ref & PART)) {
            prefetch_tri(&tris[r[i + 6].ref]);
        }
        if (i + 3 < k && !(r[i + 3].ref & PART)) {
            __builtin_prefetch(&fr->states[tris[r[i + 3].ref].state]);
        }
        if (ref & PART) {
            put_tri(RD_TRANS, &parts[ref & ~PART], 0);
        } else {
            put_tri(RD_TRANS, &tris[ref], 1);
        }
    }
}

/* ---- drawing ---- */

static void set_state(const BATCH* b) {
    const RD_STATE* s = &fr->states[b->state];
    int list = b->list, fog = b->fog, table2 = s->fog == RD_FOG_TABLE2, offset = b->offset != 0;
    GTEX* e = NULL;
    if (s->tex != NULL && !table2) {
        e = atlas && st_at[b->state] >= 0 ? &pages[st_form[b->state]] : texture(s->tex, s->palette, sampler(s));
    }
    if (e != NULL) {
        unit_on(0, 1);
        bind(0, e->name);
        active(0);
        tex_params(e, s);
        shading_env(s);
        if (lod_bias_ok && e->levels > 0) {
            static const float bias[16] = { 0, -4, -2, -1 };
            float k = bias[s->mip_d & 15];
            if (gs.u[0].bias != k) {
                gs.u[0].bias = k;
                glTexEnvf(GL_TEXTURE_FILTER_CONTROL, GL_TEXTURE_LOD_BIAS, k);
            }
        }
    } else {
        unit_on(0, 0);
    }
    if (offset && fog && offset_unit == fog_unit) {
        offset = 0;
        rd_gl1_stats.offset_lost++;
    }
    if (offset_unit != fog_unit) {
        unit_on(offset_unit, offset);
    }
    unit_on(fog_unit, fog || (offset && offset_unit == fog_unit));
    if (offset) {
        bind(offset_unit, dummy_tex);
        offset_env(offset_unit);
        env_color(offset_unit, 0xFF000000u | b->offset);
    }
    if (fog) {
        bind(fog_unit, fog_tex);
        fog_env(fog_unit, table2);
        env_color(fog_unit, frame_fog->color | 0xFF000000u);
    }
    set_blend(list != RD_OPAQUE && !(s->src == RD_ONE && s->dst == RD_ZERO), s->src, s->dst);
    set_alpha_test(list == RD_PUNCH);
    if (list == RD_OPAQUE) {
        set_depth(s->depth & 7, s->zwrite);
    } else {
        set_depth(RD_GEQUAL, list == RD_PUNCH);
    }
}

static void projection(void) {
    float m[16];
    memset(m, 0, sizeof m);
    m[0] = 2.0f / RD_WIDTH;
    m[5] = upward ? 2.0f / RD_HEIGHT : -2.0f / RD_HEIGHT;
    m[10] = 1.0f;
    m[12] = -1.0f;
    m[13] = upward ? -1.0f : 1.0f;
    m[15] = 1.0f;
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(m);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/* the background plane: a clear where it is level and of one colour */
static const float bg_cx[4] = { 0, RD_WIDTH, RD_WIDTH, 0 }, bg_cy[4] = { 0, 0, RD_HEIGHT, RD_HEIGHT };

/* the background drawn by a clear: one colour and depth */
static int bg_cleared(void) {
    const RD_VTX *a = &fr->background[0], *b = &fr->background[1], *c = &fr->background[2];
    double area = ((double)b->x - a->x) * ((double)c->y - a->y) - ((double)b->y - a->y) * ((double)c->x - a->x);
    return (a->base == b->base && a->base == c->base && a->z == b->z && a->z == c->z) || area == 0.0 || !(area == area);
}

/* the plane's 1/w at the screen's corner i */
static float bg_z(int i) {
    const RD_VTX *a = &fr->background[0], *b = &fr->background[1], *c = &fr->background[2];
    double area = ((double)b->x - a->x) * ((double)c->y - a->y) - ((double)b->y - a->y) * ((double)c->x - a->x);
    double px = bg_cx[i], py = bg_cy[i];
    double w0 = (((double)c->x - b->x) * (py - b->y) - ((double)c->y - b->y) * (px - b->x)) / area;
    double w1 = (((double)a->x - c->x) * (py - c->y) - ((double)a->y - c->y) * (px - c->x)) / area;
    return (float)(w0 * a->z + w1 * b->z + (1 - w0 - w1) * c->z);
}

/* late: drawn after the opaque lists, where it shows (late_ok's frames) */
static const GV* pointed;          /* verts, as the GL's arrays have it */
static void pointers(void);

static void background(int late) {
    const RD_VTX *a = &fr->background[0], *b = &fr->background[1], *c = &fr->background[2];
    double area = ((double)b->x - a->x) * ((double)c->y - a->y) - ((double)b->y - a->y) * ((double)c->x - a->x);
    const float *cx = bg_cx, *cy = bg_cy;
    static const int order[6] = { 0, 1, 2, 0, 2, 3 };
    uint32_t col[4];
    float z[4];
    int i, k;
    set_depth(late ? RD_GREATER : RD_ALWAYS, 1);
    if (bg_cleared()) {
        uint32_t k0 = a->base;
        glClearColor((float)(k0 >> 16 & 255) / 255.0f, (float)(k0 >> 8 & 255) / 255.0f, (float)(k0 & 255) / 255.0f,
                     (float)(k0 >> 24) / 255.0f);
        glClearDepth(depth_of(a->z));
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        return;
    }
    /* the plane's values at the screen's corners, from its three */
    for (i = 0; i < 4; i++) {
        double px = cx[i], py = cy[i];
        double w0 = (((double)c->x - b->x) * (py - b->y) - ((double)c->y - b->y) * (px - b->x)) / area;
        double w1 = (((double)a->x - c->x) * (py - c->y) - ((double)a->y - c->y) * (px - c->x)) / area;
        double w2 = 1 - w0 - w1;
        uint32_t out = 0;
        for (k = 24; k >= 0; k -= 8) {
            double v = w0 * (a->base >> k & 255) + w1 * (b->base >> k & 255) + w2 * (c->base >> k & 255) + 0.5;
            out |= (uint32_t)(v < 0 ? 0 : v > 255 ? 255 : (int)v) << k;
        }
        col[i] = out;
        z[i] = bg_z(i);
    }
    if (!grow((void**)&verts, &maxverts, nverts + 6, sizeof *verts)) {
        return;
    }
    for (i = 0; i < 6; i++) {
        GV g;
        k = order[i];
        memset(&g, 0, sizeof g);
        g.x = cx[k], g.y = cy[k], g.z = (float)(depth_of(z[k]) * 2.0 - 1.0), g.w = 1.0f;
        g.rgba = argb_to_rgba(col[k]);
        put_vertex(VERT(nverts + i), &g);
    }
    unit_on(0, 0);
    unit_on(offset_unit, 0);
    unit_on(fog_unit, 0);
    set_blend(0, 0, 0);
    set_alpha_test(0);
    if (!late) {
        glClear(GL_DEPTH_BUFFER_BIT);
    }
    if (pointed != verts) {
        pointers();
    }
    glDrawArrays(GL_TRIANGLES, nverts, 6);
    nverts += 6;
}

static void pointers(void) {
    int u;
    pointed = verts;
    glVertexPointer(4, GL_FLOAT, vstride, &verts[0].x);
    glColorPointer(4, GL_UNSIGNED_BYTE, vstride, &verts[0].rgba);
    for (u = 0; u < units && u < 3; u++) {
        glClientActiveTexture(GL_TEXTURE0 + (GLenum)u);
        /* without fog the fog's unit draws only the offset, from a texel of its own: any coordinates do */
        glTexCoordPointer(2, GL_FLOAT, vstride,
                          u == fog_unit && u != 0 && vstride == (int)sizeof(GV) ? &verts[0].s : &verts[0].u);
    }
}

/* less: the opaque list's runs of one state nearest first, each in order, where its depth test makes the order
   matter only between equal depths. 0 when it can't be (a triangle without such a test). */
static int near_first(const RD_TRI* t, int n) {
    static uint32_t* key[2];
    static int* run[2];
    static int room;
    static int count[2048];
    int i, j, sh, p = 0, nruns = 0;
    if (n > room) {
        for (i = 0; i < 2; i++) {
            uint32_t* k = realloc(key[i], (size_t)n * sizeof **key);
            int* x = k != NULL ? realloc(run[i], (size_t)n * sizeof **run) : NULL;
            if (k != NULL) {
                key[i] = k;
            }
            if (x == NULL) {
                return 0;
            }
            run[i] = x;
        }
        room = n;
    }
    for (i = 0; i < n; i = j) {
        const RD_STATE* s = &fr->states[t[i].state];
        uint32_t m = 0;
        if (!s->zwrite || ((s->depth & 7) != RD_GREATER && (s->depth & 7) != RD_GEQUAL)) {
            return 0;
        }
        for (j = i; j < n && t[j].state == t[i].state; j++) {
            uint32_t a = zbits(t[j].v[0].z), b = zbits(t[j].v[1].z), c = zbits(t[j].v[2].z);
            prefetch_tri(&t[j + 8]);
            a = a > b ? a : b;
            a = a > c ? a : c;
            m = a > m ? a : m;
        }
        key[0][nruns] = ~m;     /* the largest 1/w first */
        run[0][nruns++] = i;
    }
    for (sh = 0; sh < 32; sh += 11) {
        int sum = 0;
        memset(count, 0, sizeof count);
        for (i = 0; i < nruns; i++) {
            count[key[p][i] >> sh & 2047]++;
        }
        for (i = 0; i < 2048; i++) {
            int c = count[i];
            count[i] = sum;
            sum += c;
        }
        for (i = 0; i < nruns; i++) {
            int at = count[key[p][i] >> sh & 2047]++;
            key[!p][at] = key[p][i];
            run[!p][at] = run[p][i];
        }
        p = !p;
    }
    for (i = 0; i < nruns; i++) {
        for (j = run[p][i]; j < n && t[j].state == t[run[p][i]].state; j++) {
            prefetch_tri(&t[j + 8]);
            put_tri(RD_OPAQUE, &t[j], 1);
        }
    }
    return 1;
}

void rd_gl1_memory(RD_GL1_MEMORY* m) {
    int i;
    memset(m, 0, sizeof *m);
    for (i = 0; i < ngtex; i++) {
        const GTEX* e = &gtex[i];
        int pal = e->format == RD_PAL4 || e->format == RD_PAL8;
        size_t n = (size_t)e->w * e->h * (e->form >= 0 || (tex16 && !pal) ? 2 : 4);
        if (e->name != 0 && e->own) {
            m->textures++;
            m->texels += e->levels > 1 ? n * 4 / 3 : n;
        }
    }
    m->arrays = (size_t)maxgtex * (sizeof *gtex + 2 * sizeof *hash) + conv_room * sizeof *conv +
                (size_t)maxverts * sizeof *verts + (size_t)maxix * sizeof *ix + (size_t)maxbatches * sizeof *batches;
}

/* less: a background plane drawn after the opaque lists, so the GPU skips the hidden part of it, where that changes
   nothing: each of their triangles writes depth, passes in front, and is wholly in front of the plane */
static int late_ok(void) {
    double front = 0;
    int l, i, k;
    if (!less || bg_cleared()) {
        return 0;
    }
    for (i = 0; i < 4; i++) {
        double d = depth_up(bg_z(i));
        front = d > front ? d : front;
    }
    for (l = RD_OPAQUE; l < RD_TRANS; l++) {
        for (i = 0; i < fr->ntris[l]; i++) {
            const RD_TRI* t = &fr->tris[l][i];
            const RD_STATE* s = &fr->states[t->state];
            uint32_t m = zbits(t->v[0].z);
            prefetch_tri(t + 8);
            if (l == RD_OPAQUE && (!s->zwrite || ((s->depth & 7) != RD_GREATER && (s->depth & 7) != RD_GEQUAL))) {
                return 0;
            }
            for (k = 1; k < 3; k++) {
                uint32_t b = zbits(t->v[k].z);
                m = b < m ? b : m;
            }
            if (depth_up(zfloat(m)) <= front) {
                return 0;
            }
        }
    }
    return 1;
}

/* batches [drawn, upto), each drawn as the next is begun, while its corners are in the cache (Pinedra copies a draw's
   vertices as it is called); the background's late part before the first after the opaque ones */
static void draw_batches(int upto) {
    for (; drawn < upto; drawn++) {
        if (late && drawn == solid) {
            background(1);
        }
        if (ord_frame && drawn >= solid && !ord_live) {
            glEnable(GL_WINDOW_VERTICES_PND);
            glEnable(GL_ORDERED_TRANSLUCENCY_PND);
            ord_live = 1;
        }
        if (pointed != verts) {
            pointers();
        }
        set_state(&batches[drawn]);
        glDrawElements(GL_TRIANGLES, batches[drawn].count, GL_UNSIGNED_INT, &ix[batches[drawn].first]);
    }
    /* all but a new batch drawn: the GL has read their vertices and indices, so the next ones take their place, and
       the arrays stay in the cache */
    if (drawn == nbatches - 1 && batches[drawn].count == 0) {
        sent_verts += nverts;
        nverts = nix = 0;
        batches[drawn].first = 0;
        last_tri = NULL;
    }
}

void rd_gl1_draw_frame(RD_FRAME* f, const uint32_t* palette, const RD_FOG* fog_of) {
    uint64_t t0 = SDL_GetPerformanceCounter();
    int l, i;
    fr = f;
    rd_gl1_pal = palette;
    frame_fog = fog_of;
    frame_no++;
    memset(&rd_gl1_stats, 0, sizeof rd_gl1_stats);
    if (frame_no % 64 == 0) {
        evict();
    }
    glViewport(0, 0, out_w, out_h);
    projection();
    if (ord_on) {
        glWindowOriginPND(upward ? GL_LOWER_LEFT : GL_UPPER_LEFT);
    }
    any_fogged = 1;
    scan();
    vstride = any_fogged ? (int)sizeof(GV) : (int)offsetof(GV, s);
    tiles();
    nverts = nix = nbatches = drawn = sent_verts = 0;
    solid = INT_MAX;
    last_tri = NULL;
    if (!grow((void**)&verts, &maxverts, 6, sizeof *verts)) {
        return;
    }
    pointers();
    late = late_ok();
    if (late) {
        set_depth(RD_ALWAYS, 1);
        glClearDepth(depth_of(0.0f));
        glClear(GL_DEPTH_BUFFER_BIT);
    } else {
        background(0);
    }
    for (l = 0; l < RD_TRANS; l++) {
        if (less && l == RD_OPAQUE && near_first(fr->tris[l], fr->ntris[l])) {
            continue;
        }
        for (i = 0; i < fr->ntris[l]; i++) {
            prefetch_tri(&fr->tris[l][i + 8]);
            put_tri(l, &fr->tris[l][i], 1);
        }
    }
    solid = nbatches;
    ord_frame = ord_on && order_fits();
    put_trans();
    rd_gl1_stats.us_build = us_since(t0);
    t0 = SDL_GetPerformanceCounter();
    draw_batches(nbatches);
    if (ord_live) {
        glDisable(GL_ORDERED_TRANSLUCENCY_PND);
        glDisable(GL_WINDOW_VERTICES_PND);
        ord_live = 0;
    }
    if (late && solid == nbatches) {
        background(1);
    }
    rd_gl1_stats.us_draw = us_since(t0);
    rd_gl1_stats.batches = nbatches;
    rd_gl1_stats.verts = sent_verts + nverts;
    rd_gl1_stats.textures = ngtex;
}

void rd_gl1_end_frame(void) {
    rd_gl1_draw_frame(rd_frame, rd_palette, &rd_fog);
}

void rd_gl1_read(uint32_t* argb) {
    int y, x;
    if (out_w != RD_WIDTH || out_h != RD_HEIGHT) {
        memset(argb, 0, RD_WIDTH * RD_HEIGHT * sizeof *argb);
        return;
    }
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glReadPixels(0, 0, RD_WIDTH, RD_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, argb);
    for (y = 0; y < RD_HEIGHT / 2 && !upward; y++) {
        uint32_t* a = &argb[y * RD_WIDTH];
        uint32_t* b = &argb[(RD_HEIGHT - 1 - y) * RD_WIDTH];
        for (x = 0; x < RD_WIDTH; x++) {
            uint32_t t = a[x];
            a[x] = b[x];
            b[x] = t;
        }
    }
    for (x = 0; x < RD_WIDTH * RD_HEIGHT; x++) {
        argb[x] = argb_to_rgba(argb[x]);    /* the same swap back */
    }
}

static GLuint small_texture(int w, const uint8_t* rgba) {
    GLuint name;
    glGenTextures(1, &name);
    glBindTexture(GL_TEXTURE_2D, name);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);
    return name;
}

int rd_gl1_ordered(void) {
    return ord_on;
}

int rd_gl1_init(RD_GL_PROC proc) {
    static const uint8_t white[4] = { 255, 255, 255, 255 };
    static const float opaque[4] = { 0, 0, 0, 1 };
    int u, missing = 0, max_tex = 0, alpha_bits = 0;
#define GL_GET(ret, name, args)                                                                                    \
    gl##name = (ret(GLCALL*) args)proc("gl" #name);                                                                \
    if (gl##name == NULL) {                                                                                         \
        fprintf(stderr, "rd_gl1: no gl%s\n", #name);                                                               \
        missing++;                                                                                                  \
    }
    GL_FUNCS(GL_GET)
    if (glActiveTexture == NULL) {
        glActiveTexture = (void(GLCALL*)(GLenum))proc("glActiveTextureARB");
        glClientActiveTexture = (void(GLCALL*)(GLenum))proc("glClientActiveTextureARB");
        missing -= (glActiveTexture != NULL) + (glClientActiveTexture != NULL);
    }
    if (missing > 0) {
        return 0;
    }
    glGetIntegerv(GL_MAX_TEXTURE_UNITS, &units);
    glGetIntegerv(GL_DEPTH_BITS, &depth_bits);
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_tex);
    glGetIntegerv(GL_ALPHA_BITS, &alpha_bits);
    if (depth_bits < 16) {
        depth_bits = 16;
    }
    fprintf(stderr, "rd_gl1: %s, %s, %s; %d units, depth %d bits, alpha %d, textures to %d\n",
            (const char*)glGetString(GL_VENDOR), (const char*)glGetString(GL_RENDERER),
            (const char*)glGetString(GL_VERSION), units, depth_bits, alpha_bits, max_tex);
    if (units < 2) {
        return 0;
    }
    {
        const char* e = (const char*)glGetString(GL_EXTENSIONS);
        glWindowOriginPND = (void(GLCALL*)(GLenum))proc("glWindowOriginPND");
        glColorTableEXT = (void(GLCALL*)(GLenum, GLenum, GLsizei, GLenum, GLenum, const void*))proc("glColorTableEXT");
        ord_on = order_want && e != NULL && strstr(e, "GL_PND_ordered_translucency") != NULL &&
                 strstr(e, "GL_PND_window_vertices") != NULL && glWindowOriginPND != NULL;
        clut_on = clut_want && e != NULL && strstr(e, "GL_EXT_paletted_texture") != NULL && glColorTableEXT != NULL;
        if (order_want || clut_want) {
            fprintf(stderr, "rd_gl1: translucent order by the driver %s, palettes by the driver %s\n",
                    ord_on ? "on" : order_want ? "not there" : "off", clut_on ? "on" : clut_want ? "not there" : "off");
        }
    }
    offset_unit = 1;
    fog_unit = units >= 3 ? 2 : 1;
    while (glGetError() != 0) {
    }
    glTexEnvf(GL_TEXTURE_FILTER_CONTROL, GL_TEXTURE_LOD_BIAS, 0.0f);
    lod_bias_ok = glGetError() == 0;
    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    glDisable(GL_DITHER);
    glDisable(GL_SCISSOR_TEST);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glAlphaFunc(GL_GEQUAL, 128.0f / 255.0f);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    for (u = 0; u < units && u < 3; u++) {
        glClientActiveTexture(GL_TEXTURE0 + (GLenum)u);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    memset(&gs, 0, sizeof gs);
    gs.active = -1;
    gs.blend = gs.alpha_test = -1;
    gs.depth_func = gs.depth_mask = -1;
    for (u = 0; u < 4; u++) {
        gs.u[u].on = -1;
        gs.u[u].tex = ~0u;
        gs.u[u].color = 1;
    }
    active(0);
    glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, opaque);
    gs.u[0].color = 0xFF000000u;
    dummy_tex = small_texture(1, white);
    memset(ramp, 0, sizeof ramp);
    memset(ramp_sent, 0, sizeof ramp_sent);
    fog_tex = small_texture(RAMP, ramp);
    gs.u[0].tex = fog_tex;
    glGenTextures(NAMES, names);
    n_names = NAMES;
    return 1;
}

void rd_gl1_quit(void) {
    int i;
    for (i = 0; i < ngtex; i++) {
        glDeleteTextures(1, &gtex[i].name);
    }
    ngtex = 0;
    if (maxgtex > 0) {
        rehash();
    }
    glDeleteTextures(1, &dummy_tex);
    glDeleteTextures(1, &fog_tex);
    for (i = 0; i < RD_FORMS; i++) {
        if (pages[i].name != 0) {
            glDeleteTextures(1, &pages[i].name);
        }
    }
    memset(pages, 0, sizeof pages);
    rd_gl1_tiles_forget();
    for (i = 0; i < RD_POOL; i++) {
        if (pooled[i].name != 0) {
            glDeleteTextures(1, &pooled[i].name);
        }
    }
    memset(pooled, 0, sizeof pooled);
    rd_gl1_pool_forget();
    glDeleteTextures(n_names, names);
    n_names = 0;
}

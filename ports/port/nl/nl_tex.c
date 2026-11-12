/*
 * nl_tex.c  The NAOMI library's textures and palettes (nlcmntex, nlcmnpal, Kamui's texture loads)
 *
 * Textures are kept untwiddled, their palette bank chosen when drawn.
 */

#include <stdlib.h>
#include <string.h>

#include "naomi_host.h"
#include "nl_port.h"

NL_TEX nl_tex[NL_MAX_TEX];
int nl_tex_offset;

/* Kamui's texture types: the kind in bits 8-15, the texel format in 0-7 */

enum {
    KM_TWIDDLED = 1, KM_TWIDDLED_MM, KM_VQ, KM_VQ_MM, KM_PAL4, KM_PAL4_MM, KM_PAL8, KM_PAL8_MM,
    KM_RECTANGLE, KM_STRIDE = 0x0B, KM_TWIDDLED_RECTANGLE = 0x0D,
};

/* v's bits spread to the even bits */
static uint32_t spread(uint32_t v) {
    uint32_t i = 0, k;
    for (k = 0; v >> k; k++) {
        i |= (v >> k & 1) << (2 * k);
    }
    return i;
}

/* texel (x, y) of a twiddled texture is at col[x] + row[y] */
static uint32_t twiddle_col[1024], twiddle_row[1024];

/* an 8x8 block of a twiddled level is 64 texels in a row: texel (x + i, y + j) is AT(i, j) along */
#define SPREAD3(v) (((v) & 1) | ((v) & 2) << 1 | ((v) & 4) << 2)
#define AT(i, j) (SPREAD3(i) << 1 | SPREAD3(j))
#define ROW8(o, GET, j)                                                                                \
    ((o)[0] = GET(AT(0, j)), (o)[1] = GET(AT(1, j)), (o)[2] = GET(AT(2, j)), (o)[3] = GET(AT(3, j)),  \
     (o)[4] = GET(AT(4, j)), (o)[5] = GET(AT(5, j)), (o)[6] = GET(AT(6, j)), (o)[7] = GET(AT(7, j)))
#define BLOCK8(o, w, GET)                                                                              \
    (ROW8(o, GET, 0), ROW8((o) + (w), GET, 1), ROW8((o) + 2 * (w), GET, 2), ROW8((o) + 3 * (w), GET, 3), \
     ROW8((o) + 4 * (w), GET, 4), ROW8((o) + 5 * (w), GET, 5), ROW8((o) + 6 * (w), GET, 6),             \
     ROW8((o) + 7 * (w), GET, 7))
#define TEXEL(k) s[k]
#define NIBBLE(k) (uint8_t)(s[(k) >> 1] >> ((k) & 1) * 4 & 15)

static int blocks(int w, int h) {
    return w >= 8 && h >= 8 && (w & 7) == 0 && (h & 7) == 0;
}

static void twiddle_axes(uint32_t w, uint32_t h) {
    uint32_t n = w < h ? w : h, x, y;
    for (x = 0; x < w; x++) {
        twiddle_col[x] = (w > h ? x / n * n * n : 0) + (spread(x % n) << 1);
    }
    for (y = 0; y < h; y++) {
        twiddle_row[y] = (w > h ? 0 : y / n * n * n) + spread(y % n);
    }
}

/* the smaller levels come first, the 1x1 level after one texel of padding */
static uint32_t mip_offset(uint32_t n) {
    uint32_t t = 1, s;
    for (s = 1; s < n; s <<= 1) {
        t += s * s;
    }
    return t;
}

static int log_once(int type) {
    static int seen[64], n;
    int i;
    for (i = 0; i < n; i++) {
        if (seen[i] == type) {
            return 0;
        }
    }
    if (n < 64) {
        seen[n++] = type;
    }
    return 1;
}

/* 0 when the kind is not drawn yet */
static int unpack_level(int kind, int w, int h, const void* data, void* out) {
    const uint8_t* b = data;
    const uint16_t* p = data;
    const uint32_t* col = twiddle_col;
    int x, y;
    if (kind == KM_VQ || kind == KM_VQ_MM) {
        twiddle_axes((uint32_t)w / 2, (uint32_t)h / 2);
    } else if (kind != KM_RECTANGLE && kind != KM_STRIDE) {
        twiddle_axes((uint32_t)w, (uint32_t)h);
    }
    switch (kind) {
    case KM_PAL4:
    case KM_PAL4_MM:
        if (kind == KM_PAL4_MM) {
            b += (mip_offset((uint32_t)w) + 2) / 2;
        }
        if (blocks(w, h)) {
            for (y = 0; y < h; y += 8) {
                for (x = 0; x < w; x += 8) {
                    const uint8_t* s = b + ((col[x] + twiddle_row[y]) >> 1);
                    uint8_t* o = (uint8_t*)out + y * w + x;
                    BLOCK8(o, w, NIBBLE);
                }
            }
            return 1;
        }
        for (y = 0; y < h; y++) {
            uint32_t row = twiddle_row[y];
            for (x = 0; x < w; x++) {
                uint32_t i = col[x] + row;
                ((uint8_t*)out)[y * w + x] = (uint8_t)(b[i >> 1] >> ((i & 1) * 4) & 15);
            }
        }
        return 1;
    case KM_PAL8:
    case KM_PAL8_MM:
        if (kind == KM_PAL8_MM) {
            b += mip_offset((uint32_t)w) + 2;
        }
        if (blocks(w, h)) {
            for (y = 0; y < h; y += 8) {
                for (x = 0; x < w; x += 8) {
                    const uint8_t* s = b + col[x] + twiddle_row[y];
                    uint8_t* o = (uint8_t*)out + y * w + x;
                    BLOCK8(o, w, TEXEL);
                }
            }
            return 1;
        }
        for (y = 0; y < h; y++) {
            const uint8_t* row = b + twiddle_row[y];
            for (x = 0; x < w; x++) {
                ((uint8_t*)out)[y * w + x] = row[col[x]];
            }
        }
        return 1;
    case KM_TWIDDLED:
    case KM_TWIDDLED_MM:
    case KM_TWIDDLED_RECTANGLE:
        if (kind == KM_TWIDDLED_MM) {
            p += mip_offset((uint32_t)w);
        }
        if (blocks(w, h)) {
            for (y = 0; y < h; y += 8) {
                for (x = 0; x < w; x += 8) {
                    const uint16_t* s = p + col[x] + twiddle_row[y];
                    uint16_t* o = (uint16_t*)out + y * w + x;
                    BLOCK8(o, w, TEXEL);
                }
            }
            return 1;
        }
        for (y = 0; y < h; y++) {
            const uint16_t* row = p + twiddle_row[y];
            for (x = 0; x < w; x++) {
                ((uint16_t*)out)[y * w + x] = row[col[x]];
            }
        }
        return 1;
    case KM_VQ:
    case KM_VQ_MM: {
        /* a codebook of 256 2x2 blocks, then a byte a block, twiddled */
        const uint8_t* index = b + 2048;
        if (kind == KM_VQ_MM && w > 1) {
            uint32_t s;
            index += 1;
            for (s = 2; s < (uint32_t)w; s <<= 1) {
                index += (s / 2) * (s / 2);
            }
        }
        if (w == 1) {
            ((uint16_t*)out)[0] = p[index[0] * 4];
            return 1;
        }
        for (y = 0; y < h; y++) {
            const uint8_t* row = index + twiddle_row[y / 2];
            for (x = 0; x < w; x++) {
                ((uint16_t*)out)[y * w + x] = p[row[col[x / 2]] * 4 + ((x & 1) << 1 | (y & 1))];
            }
        }
        return 1;
    }
    case KM_RECTANGLE:
    case KM_STRIDE:
        memcpy(out, p, (size_t)w * h * 2);
        return 1;
    default:
        return 0;
    }
}

static int mipmapped(int kind) {
    return kind == KM_TWIDDLED_MM || kind == KM_VQ_MM || kind == KM_PAL4_MM || kind == KM_PAL8_MM;
}

static int unpack_texels(RD_TEXTURE* t, int type, const void* data) {
    int kind = type >> 8 & 0xff, k;
    t->format = kind >= KM_PAL4 && kind <= KM_PAL8_MM ? (kind <= KM_PAL4_MM ? RD_PAL4 : RD_PAL8) : type & 0xff;
    if (!unpack_level(kind, t->width, t->height, data, t->texels)) {
        if (log_once(type)) {
            host_log("texture type %04X is not drawn yet", type);
        }
        return 0;
    }
    for (k = 1; k <= t->levels; k++) {
        unpack_level(kind, t->width >> k, t->height >> k, data, (void*)t->mip[k]);
    }
    return 1;
}

static void set_mips(RD_TEXTURE* t, size_t texel) {
    size_t at = (size_t)t->width * t->height * texel;
    int k;
    for (k = 1; k <= t->levels; k++) {
        t->mip[k] = (uint8_t*)t->texels + at;
        at += (size_t)(t->width >> k) * (t->height >> k) * texel;
    }
}

/* a load that changes no texels keeps the serial */
static int unpack_again(NL_TEX* t, const void* src, size_t bytes, size_t texel) {
    void *old = t->tex.texels, *made = malloc(bytes);
    int ok;
    if (made == NULL) {
        ok = unpack_texels(&t->tex, t->type, src);      /* no room for both */
        t->tex.serial++;
        return ok;
    }
    t->tex.texels = made;
    set_mips(&t->tex, texel);
    ok = unpack_texels(&t->tex, t->type, src);
    if (ok && memcmp(old, made, bytes) == 0) {
        free(made);
        t->tex.texels = old;
        set_mips(&t->tex, texel);
        return 1;
    }
    free(old);
    t->bytes = bytes;
    t->tex.serial++;
    return ok;
}

static size_t texel_bytes(int type, int w, int h, size_t* texel) {
    int kind = type >> 8 & 0xff;
    size_t bytes;
    *texel = kind >= KM_PAL4 && kind <= KM_PAL8_MM ? 1 : 2;
    bytes = (size_t)w * h * *texel;
    return mipmapped(kind) && w == h ? bytes * 4 / 3 + *texel : bytes;
}

/* the texels are copied, so the game may reuse the memory */
int nl_tex_load(NL_TEX* t, const NLthd* thd) {
    int w = thd->width, h = thd->height, kind = thd->type >> 8 & 0xff, levels = 0;
    size_t texel, bytes;
    if (w <= 0 || h <= 0 || w > 1024 || h > 1024) {
        return 0;
    }
    if (mipmapped(kind) && w == h) {
        while ((w >> levels) > 1) {
            levels++;
        }
    }
    bytes = texel_bytes(thd->type, w, h, &texel);
    if (t->loaded && t->type == thd->type && t->tex.width == w && t->tex.height == h && t->tex.levels == levels) {
        t->loaded = unpack_again(t, thd->address, bytes, texel);
        return t->loaded;
    }
    if (t->tex.texels == NULL || t->bytes < bytes) {
        free(t->tex.texels);
        t->tex.texels = malloc(bytes);
        t->bytes = bytes;
        if (t->tex.texels == NULL) {
            return 0;
        }
    }
    t->tex.width = w;
    t->tex.height = h;
    t->tex.levels = levels;
    set_mips(&t->tex, texel);
    t->type = thd->type;
    t->loaded = unpack_texels(&t->tex, thd->type, thd->address);
    t->tex.serial++;
    return t->loaded;
}

int nl_tex_reload(NL_TEX* t, const void* texels) {
    size_t texel, bytes;
    if (!t->loaded) {
        return 0;
    }
    bytes = texel_bytes(t->type, t->tex.width, t->tex.height, &texel);
    unpack_again(t, texels, bytes, texel);
    return 1;
}

/* when the board stops */
void nl_tex_free_all(void) {
    int i;
    for (i = 0; i < NL_MAX_TEX; i++) {
        free(nl_tex[i].tex.texels);     /* the mip levels too */
        nl_tex[i].tex.texels = NULL;
        nl_tex[i].tex.levels = 0;
        nl_tex[i].bytes = 0;
        nl_tex[i].loaded = 0;
    }
}

void nlInitCommonTexture(int* work, int nbLoadTex, int nbMaxTex) {
    (void)work;
    (void)nbLoadTex;
    (void)nbMaxTex;
    nl_tex_offset = 0;
}

void nlSetCommonTextureOffset(int ofs) {
    nl_tex_offset = ofs;
}

void nlFreeCommonTextureAll(void) {
    int i;
    for (i = 0; i < NL_MAX_TEX; i++) {
        nl_tex[i].loaded = 0;
    }
}

int nlCommonTextureReloadA(int index, const void* texels) {
    if (index < 0 || index >= NL_MAX_TEX) {
        return 0;
    }
    return nl_tex_reload(&nl_tex[index], texels);
}

/* palettes */

static uint16_t pal_work[1024];
static int pal_dirty;
static int pal_mode = 2;    /* NL_PALETTE_4444 */
int nl_palette_direct;

void nlInitPalette(void* work, int n) {
    (void)work;
    (void)n;
    memset(pal_work, 0, sizeof pal_work);
    pal_dirty = 1;
}

void nlPaletteDirectMode(int on) {
    nl_palette_direct = on;
}

void nlSetPaletteMode(int mode) {
    pal_mode = mode;
    pal_dirty = 1;
}

void nlSetPaletteData(int no, const uint16_t* colours, int n) {
    if (no < 0 || no >= 1024) {
        return;
    }
    if (n > 1024 - no) {
        n = 1024 - no;
    }
    memcpy(&pal_work[no], colours, (size_t)n * 2);
    pal_dirty = 1;
}

/* sprite.c's name for it */
void func_0C1ED250(uint32_t no, const uint16_t* colours, int n) {
    nlSetPaletteData((int)no, colours, n);
}

static uint32_t argb(uint16_t c) {
    switch (pal_mode) {
    case 0:     /* 1555 */
        return (c & 0x8000 ? 0xFF000000u : 0) | (uint32_t)((c >> 10 & 31) * 255 / 31) << 16 |
               (uint32_t)((c >> 5 & 31) * 255 / 31) << 8 | (uint32_t)((c & 31) * 255 / 31);
    case 1:     /* 565 */
        return 0xFF000000u | (uint32_t)((c >> 11) * 255 / 31) << 16 | (uint32_t)((c >> 5 & 63) * 255 / 63) << 8 |
               (uint32_t)((c & 31) * 255 / 31);
    default:    /* 4444 */
        return (uint32_t)(c >> 12) * 0x11000000u | (uint32_t)(c >> 8 & 15) * 0x110000u | (uint32_t)(c >> 4 & 15) * 0x1100u |
               (uint32_t)(c & 15) * 0x11u;
    }
}

void nlLoadCorePaletteData(void) {
    int i;
    if (!pal_dirty) {
        return;
    }
    for (i = 0; i < 1024; i++) {
        rd_palette[i] = argb(pal_work[i]);
    }
    rd_palette_serial++;
    pal_dirty = 0;
}

void nlFreePaletteAll(void) {
}

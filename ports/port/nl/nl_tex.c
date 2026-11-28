/*
 * nl_tex.c  The NAOMI library's textures and palettes (nlcmntex, nlcmnpal, Kamui's texture loads)
 *
 * Textures are kept untwiddled, their palette bank chosen when drawn.
 */

#include <stdlib.h>
#include <string.h>

#include "host_cache.h"
#include "frame_log.h"
#include "naomi_host.h"
#include "nl_port.h"
#include "prof.h"

#if defined(__PSP__)
#define big_copy psp_copy       /* by DMA */
#else
#define big_copy memcpy
#endif

NL_TEX nl_tex[NL_MAX_TEX];
int nl_tex_offset;
int nl_tex_later;
static uint32_t serials;    /* each change of a texture's texels takes the next */
static uint32_t drawn;      /* frames given to the renderer */

/* Texels the game has freed, given back once RETIRE_FRAMES more frames have gone to the renderer: a frame in flight
   may still read them. */
#define RETIRE_FRAMES 3

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
    static uint32_t made_w, made_h;
    uint32_t n = w < h ? w : h, x, y;
    if (w == made_w && h == made_h) {
        return;
    }
    made_w = w;
    made_h = h;
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
        /* a block's texels in the codebook: (0, 0), (0, 1), (1, 0), (1, 1) */
        for (y = 0; y < h; y += 2) {
            const uint8_t* row = index + twiddle_row[y / 2];
            uint16_t* o = (uint16_t*)out + y * w;
            for (x = 0; x < w; x += 2) {
                const uint16_t* c = p + row[col[x / 2]] * 4;
                o[x] = c[0];
                o[x + 1] = c[2];
                o[x + w] = c[1];
                o[x + w + 1] = c[3];
            }
        }
        return 1;
    }
    case KM_RECTANGLE:
    case KM_STRIDE:
        big_copy(out, p, (size_t)w * h * 2);
        return 1;
    default:
        return 0;
    }
}

static int mipmapped(int kind) {
    return kind == KM_TWIDDLED_MM || kind == KM_VQ_MM || kind == KM_PAL4_MM || kind == KM_PAL8_MM;
}

static size_t source_size(int kind, int w, int h);

int nl_tex_sources;
int nl_tex_sync;
int nl_tex_fresh;

/* The 3DS tiles a PAL4 texture straight from its source, kept after the texels. A VQ square keeps only its source,
   unpacked when its GPU texture is made. Elsewhere, with nl_tex_sources, a PAL4 or VQ texture keeps only its source,
   unpacked when the renderer wants it. */
static size_t source_kept(int type, int w, int h) {
#ifdef __3DS__
    int kind = type >> 8 & 0xff;
    if (kind == KM_PAL4 && w >= 8 && h >= 8 && (w & (w - 1)) == 0 && (h & (h - 1)) == 0) {
        return (size_t)w * h / 2;
    }
#ifndef CTR_SOFT
    if ((kind == KM_VQ || kind == KM_VQ_MM) && w == h && w >= 8 && (w & (w - 1)) == 0) {
        return source_size(kind, w, h);
    }
#endif
#else
    int kind = type >> 8 & 0xff;
    if (nl_tex_sources && (kind == KM_PAL4 || kind == KM_PAL4_MM || kind == KM_VQ || kind == KM_VQ_MM)) {
        return source_size(kind, w, h);
    }
#endif
    return 0;
}

/* Without the software renderer nothing reads the texels of one drawn from its kept source, so they aren't
   made. Its source and texels are one to one: the same source is the same texels. */
static int texels_made(int type, int w, int h) {
#if defined(__3DS__) && defined(CTR_SOFT)
    (void)type, (void)w, (void)h;
    return 1;
#else
    return source_kept(type, w, h) == 0;
#endif
}

static void set_mips(RD_TEXTURE* t, size_t texel);
static size_t texel_bytes(int type, int w, int h, size_t* texel);

/* a VQ square's texels from its kept source, or with nl_tex_sources a PAL4 or VQ texture's (t is an NL_TEX's) */
static void unpack_kept(const RD_TEXTURE* t, RD_TEXTURE* out) {
    int kind = ((const NL_TEX*)t)->type >> 8 & 0xff, k;
    set_mips(out, t->format == RD_PAL4 ? 1 : 2);
    unpack_level(kind, t->width, t->height, t->texels, out->texels);
    for (k = 1; k <= t->levels; k++) {
        unpack_level(kind, t->width >> k, t->height >> k, t->texels, (void*)out->mip[k]);
    }
    out->unpack = NULL;
}

static int tex_format(int type) {
    int kind = type >> 8 & 0xff;
    return kind >= KM_PAL4 && kind <= KM_PAL8_MM ? (kind <= KM_PAL4_MM ? RD_PAL4 : RD_PAL8) : type & 0xff;
}

static int unpack_texels(RD_TEXTURE* t, int type, const void* data) {
    int kind = type >> 8 & 0xff, k;
    t->format = tex_format(type);
    t->unpack = NULL;
    if (!texels_made(type, t->width, t->height)) {
        if (kind == KM_VQ || kind == KM_VQ_MM || nl_tex_sources) {
            size_t texel;
            t->unpack = unpack_kept;
            t->unpacked = (uint32_t)texel_bytes(type, t->width, t->height, &texel);
        }
        return 1;
    }
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

/* the kept source after the texels, or in their place where they aren't made */
static size_t source_at(int type, int w, int h, size_t bytes) {
    return texels_made(type, w, h) ? bytes : 0;
}

/* of a corner tile, the source of its corner alone */
static void keep_source(NL_TEX* t, const void* src, size_t bytes) {
    size_t n = source_kept(t->type, t->tex.width, t->tex.height), c = (size_t)t->tex.corner * t->tex.corner / 2;
    void* at = n != 0 ? big_copy((uint8_t*)t->tex.texels + source_at(t->type, t->tex.width, t->tex.height, bytes), src,
                               c != 0 && c < n ? c : n)
                      : NULL;
    HOST_WRITTEN(HOST_W_KEEP, at != NULL ? (c != 0 && c < n ? c : n) : 0);
    t->tex.twiddled = (t->type >> 8 & 0xff) == KM_PAL4 ? at : NULL;
}

static void retire(void* texels);

/* the old texels kept while a frame in flight may read them */
static void let_go(void* texels) {
    if (nl_tex_fresh) {
        retire(texels);
    } else {
        free(texels);
    }
}

/* a load that changes no texels keeps the serial */
static int unpack_again(NL_TEX* t, const void* src, size_t bytes, size_t texel) {
    size_t kept = source_kept(t->type, t->tex.width, t->tex.height), room = bytes + kept;
    void *old = t->tex.texels, *made;
    int ok;
    if (!texels_made(t->type, t->tex.width, t->tex.height)) {
        if (memcmp(old, src, kept) != 0) {
            keep_source(t, src, bytes);
            t->tex.serial = ++serials;
        }
        return 1;
    }
    made = host_try_malloc(room);
    if (made == NULL) {
        ok = unpack_texels(&t->tex, t->type, src);      /* no room for both */
        keep_source(t, src, bytes);
        t->tex.serial = ++serials;
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
    let_go(old);
    t->bytes = room;
    keep_source(t, src, bytes);
    t->tex.serial = ++serials;
    return ok;
}

/* the bytes unpack_level reads for the whole texture, its mip levels too; 0 for a kind it doesn't draw */
static size_t source_size(int kind, int w, int h) {
    uint32_t s, n = 0;
    switch (kind) {
    case KM_PAL4: return (size_t)w * h / 2;
    case KM_PAL4_MM: return (mip_offset((uint32_t)w) + 2) / 2 + (size_t)w * h / 2;
    case KM_PAL8: return (size_t)w * h;
    case KM_PAL8_MM: return mip_offset((uint32_t)w) + 2 + (size_t)w * h;
    case KM_TWIDDLED:
    case KM_TWIDDLED_RECTANGLE:
    case KM_RECTANGLE:
    case KM_STRIDE: return (size_t)w * h * 2;
    case KM_TWIDDLED_MM: return (mip_offset((uint32_t)w) + (size_t)w * h) * 2;
    case KM_VQ: return 2048 + (size_t)(w / 2) * (h / 2);
    case KM_VQ_MM:
        if (w == 1) {
            return 2048 + 1;
        }
        for (s = 2, n = 1; s < (uint32_t)w; s <<= 1) {
            n += (s / 2) * (s / 2);
        }
        return 2048 + n + (size_t)(w / 2) * (h / 2);
    default: return 0;
    }
}

#define ROR16(v) ((v) >> 16 | (v) << 16)

#if defined(__PSP__)
#define ROR(v, k) ((v) >> (k) | (v) << (32 - (k)))

/* the PSP's: four lanes added, rotated and xored, no multiply until the end (a third faster there) */
static uint64_t source_hash64(const void* src, size_t n) {
    const uint32_t* p = src;
    const uint8_t* b8 = src;
    uint32_t a = (uint32_t)n ^ 0x2545F491u, b = 0x68E31DA4u, c = 0x1B873593u, d = 0xCC9E2D51u, w;
    size_t i = 0;
    if (((uintptr_t)src & 3) == 0) {
        for (; i + 4 <= n / 4; i += 4) {
            a = (a ^ p[i]) + ROR(a, 11);
            b = (b ^ p[i + 1]) + ROR(b, 7);
            c = (c ^ p[i + 2]) + ROR(c, 13);
            d = (d ^ p[i + 3]) + ROR(d, 19);
        }
        i *= 4;
    }
    for (; i + 4 <= n; i += 4) {
        memcpy(&w, b8 + i, 4);
        a = (a ^ w) + ROR(a, 11);
    }
    for (; i < n; i++) {
        a = (a ^ b8[i]) + ROR(a, 11);
    }
    a = (a ^ ROR(c, 16)) * 0x9E3779B1u;
    b = (b ^ ROR(d, 16)) * 0x85EBCA77u;
    a = (a ^ ROR(b, 16)) * 0xC2B2AE3Du;
    b = (b ^ ROR(a, 16)) * 0x27D4EB2Fu;
    return (uint64_t)(a ^ a >> 15) << 32 | (b ^ b >> 13);
}
#else
/* 64 bits of a texture's source, in two lanes of words: the same texels are not unpacked again */
static uint64_t source_hash64(const void* src, size_t n) {
    const uint32_t* p = src;
    const uint8_t* b8 = src;
    uint32_t a = (uint32_t)n ^ 0x2545F491u, b = 0x68E31DA4u, w;
    size_t i = 0;
    if (((uintptr_t)src & 3) == 0) {
        for (; i + 4 <= n / 4; i += 4) {
            a = (p[i] ^ ROR16(a)) * 0x9E3779B1u;
            b = (p[i + 1] ^ ROR16(b)) * 0x85EBCA77u;
            a = (p[i + 2] ^ ROR16(a)) * 0x9E3779B1u;
            b = (p[i + 3] ^ ROR16(b)) * 0x85EBCA77u;
        }
        i *= 4;
    }
    for (; i + 4 <= n; i += 4) {
        memcpy(&w, b8 + i, 4);
        a = (w ^ ROR16(a)) * 0x9E3779B1u;
    }
    for (; i < n; i++) {
        a = (b8[i] ^ ROR16(a)) * 0x9E3779B1u;
    }
    a = (a ^ ROR16(b)) * 0xC2B2AE3Du;
    b = (b ^ ROR16(a)) * 0x27D4EB2Fu;
    return (uint64_t)(a ^ a >> 15) << 32 | (b ^ b >> 13);
}
#endif

uint64_t nl_hash64(const void* src, size_t n) {
    return source_hash64(src, n);
}

static uint64_t loaded_hash[NL_MAX_TEX];    /* nl_tex[i]'s source when it was loaded, while hash_ok */
static uint8_t hash_ok[NL_MAX_TEX];

uint32_t host_fills;

/* MVC2_TEX_SKIP (on by default; 0 turns it off): a slot loaded again in the same frame from the same source, with no
   ROM or disc read since, is taken as the same without reading it again */
static struct {
    const void* src;
    uint32_t frame, fills, serial;
} last_load[NL_MAX_TEX];

static int tex_skip(void) {
    static int on = -1;
    if (on < 0) {
        const char* e = getenv("MVC2_TEX_SKIP");
        on = e == NULL || strcmp(e, "0") != 0;
    }
    return on;
}

static size_t texel_bytes(int type, int w, int h, size_t* texel) {
    int kind = type >> 8 & 0xff;
    size_t bytes;
    *texel = kind >= KM_PAL4 && kind <= KM_PAL8_MM ? 1 : 2;
    bytes = (size_t)w * h * *texel;
    return mipmapped(kind) && w == h ? bytes * 4 / 3 + *texel : bytes;
}

#ifdef __3DS__
/* MVC2_TEX_KEEP_FREED=1: a small texture the game frees kept as it was, with its serial, while the kept ones come to
   no more than KEEP_MOST bytes; loaded again from a source of the same hash it is back as it was, unpacked again
   from none (the game frees its sprites' textures at a screen change and loads the same ones after). Any other load
   into the slot lets go of it as the free would have */
#define KEEP_ONE 4096
#define KEEP_MOST (1u << 20)
static int keep_on = -1;
static uint8_t kept_freed[NL_MAX_TEX];
static size_t kept_bytes;
static uint32_t kept_back;

static int keep_freed(void) {
    if (keep_on < 0) {
        const char* e = getenv("MVC2_TEX_KEEP_FREED");
        keep_on = e != NULL && strcmp(e, "0") != 0;
        if (keep_on) {
            host_log("textures: small ones the game frees kept, for the same source loaded again (MVC2_TEX_KEEP_FREED)");
        }
    }
    return keep_on;
}

static void kept_let_go(NL_TEX* t) {
    kept_freed[t - nl_tex] = 0;
    kept_bytes -= t->bytes;
    hash_ok[t - nl_tex] = 0;
    retire(t->tex.texels);
    t->tex.texels = NULL;
    t->tex.levels = 0;
    t->bytes = 0;
}

static void kept_let_go_all(void) {
    int i;
    for (i = 0; i < NL_MAX_TEX; i++) {
        if (kept_freed[i]) {
            kept_let_go(&nl_tex[i]);
        }
    }
}
#endif

/* with nl_tex_later: a texture no frame in flight draws, whose texels can wait for its next draw */
static int may_wait(const NL_TEX* t, int type, int w, int h, size_t n) {
#ifdef __3DS__
    (void)t, (void)type, (void)w, (void)h, (void)n;
    return 0;       /* ctr_render reads loaded textures ahead of their draws */
#else
    static int on = -1;
    if (on < 0) {
        const char* e = getenv("MVC2_TEX_LATER");
        on = e != NULL ? atoi(e) != 0 : nl_tex_later;
    }
    return on && n != 0 && texels_made(type, w, h) && drawn - t->used >= RETIRE_FRAMES;
#endif
}

/* the source kept after the texels, for the first draw */
static void wait_source(NL_TEX* t, const void* src, size_t at, size_t n) {
    memcpy((uint8_t*)t->tex.texels + at, src, n);
    t->later = at;
}

static void unpack_waiting(NL_TEX* t) {
    uint64_t c0 = fl_now();
    const uint8_t* src = (const uint8_t*)t->tex.texels + t->later;
    t->later = 0;
    unpack_texels(&t->tex, t->type, src);
    fl.conv_ns += fl_now() - c0;
    fl.tex_conv++;
}

void nl_tex_use(NL_TEX* t) {
    t->used = drawn;
    if (t->later != 0) {
        unpack_waiting(t);
    }
}

/* the texels are copied, so the game may reuse the memory */
static int tex_load(NL_TEX* t, const NLthd* thd) {
    int w = thd->width, h = thd->height, kind = thd->type >> 8 & 0xff, levels = 0;
    size_t texel, bytes, room, n;
    uint64_t hash;
    int wait;
    if (w <= 0 || h <= 0 || w > 1024 || h > 1024) {
        return 0;
    }
    if (mipmapped(kind) && w == h) {
        while ((w >> levels) > 1) {
            levels++;
        }
    }
    if (t->tex.corner != 0) {
        t->tex.corner = 0;
        t->tex.serial = ++serials;
        hash_ok[t - nl_tex] = 0;
    }
    bytes = texel_bytes(thd->type, w, h, &texel);
    n = source_size(kind, w, h);
    if (t->loaded && t->type == thd->type && t->tex.width == w && t->tex.height == h && t->tex.levels == levels &&
        n != 0 && hash_ok[t - nl_tex] && tex_skip() && last_load[t - nl_tex].src == thd->address &&
        last_load[t - nl_tex].frame == host_frame() && last_load[t - nl_tex].fills == host_fills &&
        last_load[t - nl_tex].serial == t->tex.serial) {
        return 1;
    }
    last_load[t - nl_tex].src = thd->address;
    last_load[t - nl_tex].frame = host_frame();
    last_load[t - nl_tex].fills = host_fills;
    hash = n != 0 ? source_hash64(thd->address, n) : 0;
#ifdef __3DS__
    if (kept_freed[t - nl_tex]) {
        if (t->type == thd->type && t->tex.width == w && t->tex.height == h && t->tex.levels == levels && n != 0 &&
            loaded_hash[t - nl_tex] == hash) {
            kept_freed[t - nl_tex] = 0;
            kept_bytes -= t->bytes;
            kept_back++;
            t->loaded = 1;
            return 1;                                   /* the same source: the same texels and serial */
        }
        kept_let_go(t);
    }
#endif
    if (t->loaded && t->type == thd->type && t->tex.width == w && t->tex.height == h && t->tex.levels == levels) {
        if (n != 0 && hash_ok[t - nl_tex] && loaded_hash[t - nl_tex] == hash) {
            return 1;                                   /* the same source: the same texels and serial */
        }
        room = source_at(thd->type, w, h, bytes) + source_kept(thd->type, w, h);
        if (may_wait(t, thd->type, w, h, n) && t->bytes >= room + n) {
            wait_source(t, thd->address, room, n);
            keep_source(t, thd->address, bytes);
            t->tex.serial = ++serials;
            loaded_hash[t - nl_tex] = hash;
            hash_ok[t - nl_tex] = 1;
            return 1;
        }
        if (t->later != 0) {
            unpack_waiting(t);      /* unpack_again compares with the texels there */
        }
        { uint64_t c0 = fl_now(); t->loaded = unpack_again(t, thd->address, bytes, texel); fl.conv_ns += fl_now() - c0; fl.tex_conv++; }
        loaded_hash[t - nl_tex] = hash;
        hash_ok[t - nl_tex] = n != 0 && t->loaded;
        return t->loaded;
    }
    hash_ok[t - nl_tex] = 0;
    t->later = 0;
    room = source_at(thd->type, w, h, bytes) + source_kept(thd->type, w, h);
    wait = may_wait(t, thd->type, w, h, n);
    if (t->tex.texels == NULL || t->bytes < room + (wait ? n : 0) || nl_tex_fresh) {
        let_go(t->tex.texels);
        t->tex.texels = host_try_malloc(room + (wait ? n : 0));
#ifdef __3DS__
        if (t->tex.texels == NULL && kept_bytes != 0) {
            kept_let_go_all();
            t->tex.texels = host_try_malloc(room);
        }
#endif
        t->bytes = room + (wait ? n : 0);
        if (t->tex.texels == NULL) {
            static int told;
            if (told++ < 8) {
                host_log("textures: no memory for a %dx%d texture of type %04X, %lu bytes: not drawn", w, h, thd->type,
                         (unsigned long)room);
            }
            return 0;
        }
    }
    t->tex.width = w;
    t->tex.height = h;
    t->tex.levels = levels;
    set_mips(&t->tex, texel);
    t->type = thd->type;
    if (wait) {
        t->tex.format = tex_format(thd->type);
        t->tex.unpack = NULL;
        wait_source(t, thd->address, room, n);
        t->loaded = 1;
    } else {
        uint64_t c0 = fl_now();
        t->loaded = unpack_texels(&t->tex, thd->type, thd->address);
        fl.conv_ns += fl_now() - c0;
        fl.tex_conv++;
    }
    keep_source(t, thd->address, bytes);
    t->tex.serial = ++serials;
    loaded_hash[t - nl_tex] = hash;
    hash_ok[t - nl_tex] = n != 0 && t->loaded;
    return t->loaded;
}

int nl_tex_load(NL_TEX* t, const NLthd* thd) {
    PROF_START(t0);
    fl.tex_calls++;
    uint64_t f0 = fl_now();
    int r = tex_load(t, thd);
    fl.tex_ns += fl_now() - f0;
    last_load[t - nl_tex].serial = t->tex.serial;
    PROF_STOP(PZ_NL_TEX, t0);
    return r;
}

/* The sprites' tiles, reloaded every frame, come back to the same cells as they animate: recent ones are kept
   by their source's hash with their serial, and their texels where they are made. Only kinds whose texels and
   source are one to one are kept, so new source is new texels. */
#define CACHE_WAYS 4
#define TEXEL_SETS 64
#ifdef __PSP__
#define CACHE_SETS 1024     /* those kept as their sources, a serial each */
#else
#define CACHE_SETS TEXEL_SETS
#endif
#define CACHE_SOURCE 512
#define CACHE_TEXELS 1024

typedef struct {
    uint64_t hash;
    uint32_t serial, used;
    uint16_t type, width, height;
} CACHED;

static CACHED cache[CACHE_SETS][CACHE_WAYS];
static uint8_t (*cache_texels)[CACHE_WAYS][CACHE_TEXELS];
static uint32_t cache_clock;

static size_t source_bytes(int kind, int w, int h) {
    switch (kind) {
    case KM_PAL4: return (size_t)w * h / 2;
    case KM_PAL8: return (size_t)w * h;
    case KM_TWIDDLED:
    case KM_TWIDDLED_RECTANGLE:
    case KM_RECTANGLE: return (size_t)w * h * 2;
    default: return 0;
    }
}

/* the sources' hashes by their keys from nl_lz (MVC2_LZ_KEYS): a tile from a cell seen before not hashed again */
#define KEY_SETS 4096

typedef struct {
    uint64_t key, hash;
} KEYED;

static KEYED* keyed;

static uint64_t keyed_hash(const uint8_t* src, size_t n) {
    uint64_t k;
    KEYED* e;
    if (!nl_lz_key(src, n, &k) || (keyed == NULL && (keyed = host_try_calloc(KEY_SETS, sizeof *keyed)) == NULL)) {
        return source_hash64(src, n);
    }
    e = &keyed[(uint32_t)(k >> 32) & (KEY_SETS - 1)];
    if (e->key != k || e->hash == 0) {
        e->key = k;
        e->hash = source_hash64(src, n);
    }
    return e->hash;
}

/* with nl_tex_fresh, new memory for a reload's texels (levels none); 0 when there is none */
static int fresh_texels(NL_TEX* t) {
    void* p;
    if (!nl_tex_fresh) {
        return 1;
    }
    if ((p = host_try_malloc(t->bytes)) == NULL) {
        return 0;
    }
    retire(t->tex.texels);
    t->tex.texels = p;
    return 1;
}

/* 0 when the reload is not one the cache takes; the source's first `drawn` bytes alone tell one reload from another */
static int reload_cached(NL_TEX* t, const uint8_t* src, size_t bytes, size_t drawn) {
    int kind = t->type >> 8 & 0xff, i, made = texels_made(t->type, t->tex.width, t->tex.height);
    size_t n = source_bytes(kind, t->tex.width, t->tex.height), at = (size_t)(t - nl_tex);
    uint64_t h;
    unsigned sn;
    CACHED *set, *e = NULL;
    if (n == 0 || n > CACHE_SOURCE || (n & 3) != 0 || bytes > CACHE_TEXELS || t->tex.levels != 0) {
        return 0;
    }
    if (made && cache_texels == NULL && (cache_texels = host_try_calloc(TEXEL_SETS, sizeof *cache_texels)) == NULL) {
        return 0;
    }
    h = keyed_hash(src, drawn < n ? drawn : n);
    if (hash_ok[at] && loaded_hash[at] == h) {
        return 1;       /* the source it has */
    }
    sn = made ? h % TEXEL_SETS : h % CACHE_SETS;
    set = cache[sn];
    for (i = 0; i < CACHE_WAYS; i++) {
        CACHED* c = &set[i];
        if (c->serial != 0 && c->hash == h && c->type == t->type && c->width == t->tex.width && c->height == t->tex.height) {
            c->used = ++cache_clock;
            if (t->tex.serial != c->serial) {
                if (made) {
                    if (!fresh_texels(t)) {
                        return 0;
                    }
                    memcpy(t->tex.texels, cache_texels[sn][i], bytes);
                    HOST_WRITTEN(HOST_W_TEX, bytes);
                }
                keep_source(t, src, bytes);
                t->tex.serial = c->serial;
            }
            loaded_hash[at] = h;
            hash_ok[at] = 1;
            return 1;
        }
        if (e == NULL || c->used < e->used) {
            e = c;
        }
    }
    if (made) {
        uint8_t* out = cache_texels[sn][e - set];
        if (!fresh_texels(t) || !unpack_level(kind, t->tex.width, t->tex.height, src, out)) {
            return 0;
        }
        memcpy(t->tex.texels, out, bytes);
        HOST_WRITTEN(HOST_W_TEX, bytes);
    }
    keep_source(t, src, bytes);
    e->type = (uint16_t)t->type;
    e->width = (uint16_t)t->tex.width;
    e->height = (uint16_t)t->tex.height;
    e->hash = h;
    e->serial = t->tex.serial = ++serials;
    e->used = ++cache_clock;
    loaded_hash[at] = h;
    hash_ok[at] = 1;
    return 1;
}

/* side: the corner drawn, 0 all of it */
static int reload(NL_TEX* t, const void* texels, int side) {
    size_t texel, bytes, drawn = side != 0 ? (size_t)side * side / 2 : (size_t)-1;
    PROF_START(t0);
    if (!t->loaded) {
        return 0;
    }
    if (t->later != 0) {
        unpack_waiting(t);      /* a reload changes part of them */
    }
    if (t->tex.corner != side) {
        t->tex.corner = side;
        t->tex.serial = ++serials;
        hash_ok[t - nl_tex] = 0;
    }
    bytes = texel_bytes(t->type, t->tex.width, t->tex.height, &texel);
    if (!reload_cached(t, texels, bytes, drawn)) {
        uint64_t c0 = fl_now();
        hash_ok[t - nl_tex] = 0;
        unpack_again(t, texels, bytes, texel);
        fl.conv_ns += fl_now() - c0; fl.tex_conv++;
    }
    PROF_STOP(PZ_NL_TEX, t0);
    return 1;
}

int nl_tex_reload(NL_TEX* t, const void* texels) {
    return reload(t, texels, 0);
}

typedef struct {
    void* texels;
    uint32_t drawn;
} RETIRED;

static RETIRED* retired;
static int nretired, retired_room;

static void retire(void* texels) {
    if (texels == NULL) {
        return;
    }
    if (nretired == retired_room) {
        int room = retired_room != 0 ? retired_room * 2 : 512;
        RETIRED* r = host_try_malloc(sizeof *r * (size_t)room);
        if (r == NULL) {
            free(texels);       /* nothing points at them: the slot's pointer is cleared */
            return;
        }
        if (nretired != 0) {
            memcpy(r, retired, sizeof *r * (size_t)nretired);
        }
        free(retired);
        retired = r;
        retired_room = room;
    }
    retired[nretired].texels = texels;
    retired[nretired].drawn = drawn;
    nretired++;
}

static void retire_all(void) {
    int i;
    for (i = 0; i < nretired; i++) {
        free(retired[i].texels);
    }
    free(retired);
    retired = NULL;
    nretired = retired_room = 0;
}

void nl_tex_frame(void) {
    int i, n = 0;
    if (!rd_discard || nl_tex_sync) {
        drawn++;
    }
#ifdef __3DS__
    if (keep_on > 0 && host_frame() % 600 == 0) {
        host_log("textures: %lu freed ones loaded again from the same source, %lu KB kept (MVC2_TEX_KEEP_FREED)",
                 (unsigned long)kept_back, (unsigned long)(kept_bytes >> 10));
    }
#endif
    if (nretired == 0) {
        return;
    }
    for (i = 0; i < nretired; i++) {
        if (drawn - retired[i].drawn >= RETIRE_FRAMES) {
            free(retired[i].texels);
        } else {
            retired[n++] = retired[i];
        }
    }
    nretired = n;
    if (n == 0) {
        retire_all();
    }
}

void nl_tex_free(NL_TEX* t) {
#ifdef __3DS__
    if (t->loaded && hash_ok[t - nl_tex] && t->bytes <= KEEP_ONE && kept_bytes + t->bytes <= KEEP_MOST && keep_freed()) {
        kept_freed[t - nl_tex] = 1;
        kept_bytes += t->bytes;
        t->loaded = 0;
        return;
    }
    if (kept_freed[t - nl_tex]) {
        kept_let_go(t);
        return;
    }
#endif
    t->loaded = 0;
    t->later = 0;
    retire(t->tex.texels);
    t->tex.texels = NULL;
    t->tex.levels = 0;
    t->bytes = 0;
}

size_t nl_tex_held(void) {
    size_t n = 0;
    int i;
    for (i = 0; i < NL_MAX_TEX; i++) {
        n += nl_tex[i].bytes;
    }
    return n;
}

/* when the board stops */
void nl_tex_free_all(void) {
    int i;
#ifdef __3DS__
    if (kept_back != 0) {
        host_log("textures: %lu freed ones loaded again from the same source (MVC2_TEX_KEEP_FREED)",
                 (unsigned long)kept_back);
    }
    memset(kept_freed, 0, sizeof kept_freed);
    kept_bytes = 0;
    kept_back = 0;
#endif
    retire_all();
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
        nl_tex_free(&nl_tex[i]);
    }
}

int nlCommonTextureReloadA(int index, const void* texels) {
    if (index < 0 || index >= NL_MAX_TEX) {
        return 0;
    }
    return nl_tex_reload(&nl_tex[index], texels);
}

/* a PAL4 square whose sprite draws, point sampled, only the corner its first `drawn` bytes of source hold: the rest,
   never seen, doesn't make it a new texture */
int nlCommonTextureReloadDrawn(int index, const void* texels, int drawn) {
    NL_TEX* t;
    int side;
    if (index < 0 || index >= NL_MAX_TEX) {
        return 0;
    }
    t = &nl_tex[index];
    for (side = 8; side < t->tex.width && side * side / 2 != drawn; side *= 2) {
    }
    if ((t->type >> 8 & 0xff) != KM_PAL4 || t->tex.width != t->tex.height || side >= t->tex.width) {
        return nl_tex_reload(t, texels);
    }
    return reload(t, texels, side);
}

/* palettes */

static uint16_t pal_work[1024];
static int pal_dirty;
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
    rd_palette_mode = mode;
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
    switch (rd_palette_mode) {
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

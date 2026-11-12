/*
 * naomi_m1.c  The M1 cartridge's packed reads (nlaNRSDecodeRead)
 *
 * The scheme is as MAME's naomim1.cpp (BSD-3-Clause, copyright-holders: Olivier Galibert) works it out.
 */

#include <string.h>

#include "naomi_host.h"

#define M1_KEY 0x7c6e8bc1u      /* c18b6e7c, as the chip uses it */
#define DICT_SIZE 111
#define END_CODE 111

typedef struct {
    uint32_t pos;           /* the next word's ROM offset */
    uint32_t buf_at, buf_len;
    uint8_t buf[4096];      /* the stream, read ahead */
    uint64_t val;
    int bits;
} BITS;

static uint32_t next_word(BITS* b) {
    uint32_t a, c, d, e, left = naomi_rom_size() - b->pos;
    const uint8_t* w;
    if (b->pos + 4 > naomi_rom_size()) {
        return 0;
    }
    if (b->pos < b->buf_at || b->pos + 4 > b->buf_at + b->buf_len) {
        b->buf_at = b->pos;
        b->buf_len = left < sizeof b->buf ? left & ~3u : sizeof b->buf;
        if (!naomi_rom_read(b->buf_at, b->buf, b->buf_len)) {
            b->buf_len = 0;
            return 0;
        }
    }
    w = b->buf + (b->pos - b->buf_at);
    a = w[0];
    c = w[1];
    d = w[2];
    e = w[3];
    b->pos += 4;
    return M1_KEY ^ ((c ^ e) << 24 | (a ^ d) << 16 | c << 8 | a);
}

static uint32_t look(BITS* b, int n) {
    if (n > b->bits) {
        b->val = b->val << 32 | next_word(b);
        b->bits += 32;
    }
    return (uint32_t)(b->val >> (b->bits - n)) & ((1u << n) - 1);
}

static void skip(BITS* b, int n) {
    b->bits -= n;
}

static uint32_t get(BITS* b, int n) {
    uint32_t v = look(b, n);
    skip(b, n);
    return v;
}

/* past the stream's end, zeroes */
void naomi_m1_read(uint32_t src, uint8_t* dst, uint32_t size) {
    uint8_t dict[DICT_SIZE];
    BITS b;
    uint32_t n = 0;
    int i;

    if (src >= naomi_rom_size()) {
        host_log("packed read past the ROM's end: %08X", src);
        memset(dst, 0, size);
        return;
    }
    b.pos = src;
    b.buf_at = 0;
    b.buf_len = 0;
    b.val = 0;
    b.bits = 0;
    for (i = 0; i < DICT_SIZE; i++) {
        dict[i] = (uint8_t)get(&b, 8);
    }
    while (n < size) {
        int byte;
        switch (look(&b, 3)) {
        case 0:
        case 1:     /* 00 + 2: a dictionary byte 1-3, or 0 and a literal */
            skip(&b, 2);
            i = (int)get(&b, 2);
            byte = i ? dict[i] : (int)get(&b, 8);
            break;
        case 2:     /* 010 + 2 */
            skip(&b, 3);
            byte = dict[get(&b, 2) + 4];
            break;
        case 3:     /* 011 + 3 */
            skip(&b, 3);
            byte = dict[get(&b, 3) + 8];
            break;
        case 4:
        case 5:     /* 10 + 5 */
            skip(&b, 2);
            byte = dict[get(&b, 5) + 16];
            break;
        default:    /* 11 + 6, or the end */
            skip(&b, 2);
            i = (int)get(&b, 6) + 48;
            if (i == END_CODE) {
                memset(dst + n, 0, size - n);
                return;
            }
            byte = dict[i];
            break;
        }
        if ((dict[0] & 64) && n >= 2) {
            byte = dst[n - 2] - byte;
        }
        dst[n++] = (uint8_t)byte;
    }
}

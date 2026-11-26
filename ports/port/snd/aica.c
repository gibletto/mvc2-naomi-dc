/*
 * aica.c  The AICA, the NAOMI's and the Dreamcast's sound chip: 64 channels, the DSP, the timers and the interrupts.
 * Written from its register map and the SCSP's documented behaviour; it runs 16 samples at a time.
 */

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(__ARM_FEATURE_SAT) || defined(__ARM_FEATURE_DSP)
#include <arm_acle.h>
#endif

#include "naomi_host.h"
#include "aica.h"
#include "aica_dsp.h"
#include "arm7.h"
#include "prof.h"
#include "snd_drv.h"

#ifdef MVC2_PROF
static int game_side;               /* aica_run_cpu: the game's thread runs the CPU, the sound's waiting */
#define IO_COUNT() do { if (!game_side) PROF_COUNT(PZ_SND_IO, 1); } while (0)
#else
#define IO_COUNT() ((void)0)
#endif
#ifdef AICA_PERF
void (*aica_perf)(int at);
#define PERF(at) do { if (aica_perf) aica_perf(at); } while (0)
#else
#define PERF(at) ((void)0)
#endif

#define CHUNK 256                   /* the most samples mixed at once */
#define CPU_CYCLES_PER_SAMPLE 64    /* 2.8224 MHz: the ARM7's 45 MHz with the sound RAM's waits */
static int cpu_cycles = CPU_CYCLES_PER_SAMPLE;

static uint8_t* ram;
uint8_t* aica_ram;
uint32_t aica_ram_mask;

static uint8_t reg[0x8000];         /* as written; 0x800000-0x807FFF to the sound CPU */
static int cpu_reset = 1;
static int cpu_debt;                /* cycles the CPU ran past its share */
static int dsp_on = 1, filter_on = 1;
static int chunk = 16;              /* MVC2_SNDCHUNK: the samples mixed at once, the registers as they are */
static int half;                    /* MVC2_SNDHALF: the plain chunks' samples made at half the rate, each held */

static inline uint32_t reg16(uint32_t off) {
    return (uint32_t)reg[off] | (uint32_t)reg[off + 1] << 8;
}

/* ---- tables ---- */

/* the envelope rates' times, in ms, from silence to full (attack) and full to silence (the rest) */
static const float attack_ms[64] = {
    0, 0, 8100, 6900, 6000, 4800, 4000, 3400, 3000, 2400, 2000, 1700, 1500, 1200, 1000, 860, 760, 600, 500, 430,
    380, 300, 250, 220, 190, 150, 130, 110, 95, 76, 63, 55, 47, 38, 31, 27, 24, 19, 15, 13, 12, 9.4f, 7.9f, 6.8f,
    6.0f, 4.7f, 3.8f, 3.4f, 3.0f, 2.4f, 2.0f, 1.8f, 1.6f, 1.3f, 1.1f, 0.93f, 0.85f, 0.65f, 0.53f, 0.44f, 0.40f,
    0.35f, 0, 0,
};
static const float decay_ms[64] = {
    0, 0, 118200, 101300, 88600, 70900, 59100, 50700, 44300, 35500, 29600, 25300, 22200, 17700, 14800, 12700,
    11100, 8900, 7400, 6300, 5500, 4400, 3700, 3200, 2800, 2200, 1800, 1600, 1400, 1100, 920, 790, 690, 550, 460,
    390, 340, 270, 230, 200, 170, 140, 110, 98, 85, 68, 57, 49, 43, 34, 28, 25, 22, 18, 14, 12, 11, 8.5f, 7.1f,
    6.1f, 5.4f, 4.3f, 3.6f, 3.1f,
};
/* the LFO's frequencies, Hz */
static const float lfo_hz[32] = {
    0.17f, 0.19f, 0.23f, 0.27f, 0.34f, 0.39f, 0.45f, 0.55f, 0.68f, 0.78f, 0.92f, 1.10f, 1.39f, 1.60f, 1.87f, 2.27f,
    2.87f, 3.31f, 3.92f, 4.79f, 6.15f, 7.18f, 8.60f, 10.8f, 14.4f, 17.2f, 21.5f, 28.7f, 43.1f, 57.4f, 86.1f, 172.3f,
};
static const float plfo_cents[8] = { 0, 3.378f, 6.755f, 13.51f, 27.02f, 54.04f, 108.1f, 216.2f };
static const float alfo_db[8] = { 0, 0.4f, 0.8f, 1.5f, 3, 6, 12, 24 };

static uint32_t attack_step[64], decay_step[64];    /* envelope units (0x3FF full) << 16, a sample */
static uint32_t lfo_step[32];                       /* the phase (256 a period) << 16, a sample */
static int32_t att_gain[1024];                      /* 2^(-a/64), Q15: an attenuation of a/1024 of 96 dB */
static int32_t level_gain[16];                      /* a send level: 0 off, then 3 dB steps to 15 = 0 dB */
static int32_t pan_gain[16];                        /* the side a pan turns down: 3 dB a step, 15 off */

/* 16 bits, the width of the output and of the DSP's EFREG */
static int32_t out_level(int32_t s) {
    return s > 32767 ? 32767 : s < -32768 ? -32768 : s;
}
static int32_t plfo_tab[8][256];                    /* pitch: Q16 factors */
static int32_t alfo_tab[8][256];                    /* attenuation units */

static const int adpcm_scale[8] = { 230, 230, 230, 230, 307, 409, 512, 614 };

static void make_tables(void) {
    int i, j;
    for (i = 0; i < 64; i++) {
        attack_step[i] = attack_ms[i] > 0 ? (uint32_t)(1024.0 * 65536 / (attack_ms[i] * 44.1) + 0.5) : i < 2 ? 0 : 1024u << 16;
        decay_step[i] = decay_ms[i] > 0 ? (uint32_t)(1024.0 * 65536 / (decay_ms[i] * 44.1) + 0.5) : 0;
    }
    for (i = 0; i < 32; i++) {
        lfo_step[i] = (uint32_t)(lfo_hz[i] * 256.0 * 65536 / AICA_RATE + 0.5);
    }
    for (i = 0; i < 1024; i++) {
        att_gain[i] = (int32_t)(32768.0 * pow(2.0, -i / 64.0) + 0.5);
    }
    for (i = 0; i < 16; i++) {
        level_gain[i] = i == 0 ? 0 : (int32_t)(32768.0 * pow(2.0, -(15 - i) / 2.0) + 0.5);
        pan_gain[i] = i == 15 ? 0 : (int32_t)(32768.0 * pow(2.0, -i / 2.0) + 0.5);
    }
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 256; j++) {
            double w = (j - 128) / 128.0;
            plfo_tab[i][j] = (int32_t)(65536.0 * pow(2.0, plfo_cents[i] * w / 1200.0) + 0.5);
            alfo_tab[i][j] = (int32_t)(alfo_db[i] * (j / 255.0) / (96.0 / 1024) + 0.5);
        }
    }
}

/* ---- the channels ---- */

enum { EG_ATTACK, EG_DECAY1, EG_DECAY2, EG_RELEASE };

typedef struct {
    uint32_t pos;           /* the sample s0 is, from SA */
    uint32_t frac;          /* between s0 and s1, 16 bits */
    int32_t s0, s1;
    int32_t ad_prev, ad_step;       /* the ADPCM decoder after s1 */
    int32_t ad_loop_prev, ad_loop_step;
    int32_t eg;             /* attenuation << 16, 0 loudest */
    int32_t flv;            /* the filter's cutoff << 16 */
    int32_t y1, y2;
    uint32_t lfo;           /* phase << 16 */
    uint32_t noise;
    uint8_t ad_loop_saved;
    uint8_t eg_state;
    uint8_t feg_state;
    uint8_t lp;             /* passed the loop end since read */
} CHANNEL;                  /* two cache lines */

static _Alignas(32) CHANNEL ch[64];
static uint8_t ch_on[64];           /* sounding: keyed on, or releasing */

#define CR(c, r) reg16((uint32_t)(c) * 0x80 + (r))

static int oct_of(int c) {
    int oct = (CR(c, 0x18) >> 11) & 15;
    return oct >= 8 ? oct - 16 : oct;
}

static int eff_rate(int c, int r) {
    int krs = (CR(c, 0x14) >> 10) & 15, rate;
    if (r == 0) {
        return 0;
    }
    rate = 2 * r;
    if (krs != 15) {
        rate += oct_of(c) + 2 * krs + ((CR(c, 0x18) >> 9) & 1);
    }
    return rate < 0 ? 0 : rate > 63 ? 63 : rate;
}

/* the source of a channel's samples, read once a chunk */
typedef struct {
    const uint8_t* ram;
    uint32_t mask, sa, lsa, lea;
    int loop;
} SOURCE;

/* the sample at index i, when it comes after the one before (as the ADPCM decoder needs) */
static inline int32_t source_at(const SOURCE* src, int pcms, CHANNEL* v, uint32_t i, int looped) {
    int n;
    int32_t d, s;
    if (pcms == 0) {
        uint32_t a = (src->sa + i * 2) & src->mask & ~1u;
        return (int16_t)(src->ram[a] | src->ram[a + 1] << 8);
    }
    if (pcms == 1) {
        return (int8_t)src->ram[(src->sa + i) & src->mask] << 8;
    }
    if (i == src->lsa) {
        if (looped && pcms == 2 && v->ad_loop_saved) {
            v->ad_prev = v->ad_loop_prev;
            v->ad_step = v->ad_loop_step;
        } else if (!v->ad_loop_saved) {
            v->ad_loop_prev = v->ad_prev;
            v->ad_loop_step = v->ad_step;
            v->ad_loop_saved = 1;
        }
    }
    n = src->ram[(src->sa + (i >> 1)) & src->mask] >> ((i & 1) * 4) & 15;
    d = v->ad_step * ((n & 7) * 2 + 1) >> 3;
    d = d > 32767 ? 32767 : d;
    s = n & 8 ? v->ad_prev - d : v->ad_prev + d;
    s = s > 32767 ? 32767 : s < -32768 ? -32768 : s;
    v->ad_prev = s;
    v->ad_step = v->ad_step * adpcm_scale[n & 7] >> 8;
    v->ad_step = v->ad_step < 127 ? 127 : v->ad_step > 24576 ? 24576 : v->ad_step;
    return s;
}

static void source_of(int c, SOURCE* src) {
    src->ram = ram;
    src->mask = aica_ram_mask;
    src->sa = (CR(c, 0x00) & 0x7F) << 16 | CR(c, 0x04);
    src->lsa = CR(c, 0x08);
    src->lea = CR(c, 0x0C);
    src->loop = CR(c, 0x00) & 0x200;
}

static int32_t sample_at(int c, uint32_t i, int looped) {
    SOURCE src;
    source_of(c, &src);
    return source_at(&src, (CR(c, 0x00) >> 7) & 3, &ch[c], i, looped);
}

/* the sample after i, and whether the channel goes on: a loop goes back to LSA, else it ends at LEA */
static int next_index(int c, uint32_t i, uint32_t* next, int* looped) {
    uint32_t lea = CR(c, 0x0C);
    *looped = 0;
    if (i + 1 >= lea) {
        if (!(CR(c, 0x00) & 0x200)) {
            return 0;
        }
        *next = CR(c, 0x08);
        *looped = 1;
        return 1;
    }
    *next = i + 1;
    return 1;
}

/* a channel that has ended: its KYONB goes too, so the next KYONEX does not start it again. It reads back as
   released, silent and at address 0, which is how the driver tells that a slot is free again */
static void channel_stop(int c) {
    ch_on[c] = 0;
    ch[c].pos = 0;
    ch[c].frac = 0;
    ch[c].eg_state = ch[c].feg_state = EG_RELEASE;
    ch[c].eg = 0x3FF << 16;
    reg[c * 0x80 + 1] &= (uint8_t)~0x40;
}

static void key_on(int c) {
    CHANNEL* h = &ch[c];
    uint32_t nx;
    int looped;
    if (ch_on[c] && h->eg_state != EG_RELEASE) {
        return;
    }
    memset(h, 0, sizeof *h);
    ch_on[c] = 1;
    h->ad_step = 127;
    h->eg_state = EG_ATTACK;
    h->eg = 0x3FF << 16;
    if (eff_rate(c, CR(c, 0x10) & 31) >= 62) {
        h->eg = 0;
    }
    h->feg_state = EG_ATTACK;
    h->flv = (int32_t)(CR(c, 0x2C) & 0x1FFF) << 16;
    h->s0 = sample_at(c, 0, 0);
    h->s1 = next_index(c, 0, &nx, &looped) ? sample_at(c, nx, looped) : 0;
}

static void key_off(int c) {
    if (ch_on[c]) {
        ch[c].eg_state = EG_RELEASE;
        ch[c].feg_state = EG_RELEASE;
    }
}

static void key_execute(void) {
    int c;
    for (c = 0; c < 64; c++) {
        if (CR(c, 0x00) & 0x4000) {
            key_on(c);
        } else {
            key_off(c);
        }
    }
}

/* one sample's envelope: the attenuation (0 to 0x3FF), or -1 when the channel has gone quiet */
static int eg_step(int c, CHANNEL* h) {
    uint32_t r1 = CR(c, 0x10), r2 = CR(c, 0x14);
    switch (h->eg_state) {
    case EG_ATTACK:
        h->eg -= (int32_t)attack_step[eff_rate(c, r1 & 31)];
        if (h->eg <= 0) {
            h->eg = 0;
            if (!(r2 & 0x4000)) {
                h->eg_state = EG_DECAY1;
            }
        }
        break;
    case EG_DECAY1:
        h->eg += (int32_t)decay_step[eff_rate(c, (r1 >> 6) & 31)];
        if (h->eg >= (int32_t)(((r2 >> 5) & 31) << 5) << 16) {
            h->eg_state = EG_DECAY2;
        }
        break;
    case EG_DECAY2:
        h->eg += (int32_t)decay_step[eff_rate(c, (r1 >> 11) & 31)];
        break;
    default:
        h->eg += (int32_t)decay_step[eff_rate(c, r2 & 31)];
        break;
    }
    if (h->eg >= 0x3FF << 16) {
        h->eg = 0x3FF << 16;
        if (h->eg_state == EG_RELEASE) {
            return -1;
        }
    }
    return h->eg >> 16;
}

/* the filter's envelope: FLV0 to FLV1 (FAR), to FLV2 (FD1R), to FLV3 (FD2R); FLV4 on release (FRR) */
static void feg_step(int c, CHANNEL* h, int n) {
    static const int target_reg[4] = { 0x30, 0x34, 0x38, 0x3C };
    int rate, step;
    int32_t target = (int32_t)(CR(c, target_reg[h->feg_state]) & 0x1FFF) << 16;
    switch (h->feg_state) {
    case EG_ATTACK: rate = (CR(c, 0x40) >> 8) & 31; break;
    case EG_DECAY1: rate = CR(c, 0x40) & 31; break;
    case EG_DECAY2: rate = (CR(c, 0x44) >> 8) & 31; break;
    default: rate = CR(c, 0x44) & 31; break;
    }
    step = (int)(decay_step[eff_rate(c, rate)] * 8) * n;   /* 0x1FFF full, against the envelope's 0x3FF */
    if (h->flv < target) {
        h->flv = h->flv + step >= target ? target : h->flv + step;
    } else if (h->flv > target) {
        h->flv = h->flv - step <= target ? target : h->flv - step;
    }
    if (h->flv == target && h->feg_state < EG_DECAY2) {
        h->feg_state++;
    }
}

/* ---- the mix of one chunk ---- */

static int32_t mix_l[CHUNK], mix_r[CHUNK];
static int32_t mixs[CHUNK][16];     /* the DSP's inputs a sample, 20 bits */
static int32_t sends[CHUNK];        /* all the inputs' sum, 16 bits, for the reverb in the DSP's place */
static int sends_on, sent;          /* sends is wanted; something was sent in this chunk */
static int mixs_on = 1;             /* mixs is wanted: the DSP runs (or MVC2_MIXFAST=0) */
static int mix_fast = 1;            /* MVC2_MIXFAST: no work the output doesn't use */

/* the LFO's waves, 0 to 255: saw, square, triangle, noise */
static int lfo_wave(int ws, int ph, uint32_t noise) {
    switch (ws) {
    case 0: return ph;
    case 1: return ph < 128 ? 255 : 0;
    case 2: return ph < 128 ? ph * 2 : 511 - ph * 2;
    default: return (int)(noise >> 24);
    }
}

/* the envelope's change a sample in a state */
static int32_t eg_rate(int c, int state) {
    uint32_t r1 = CR(c, 0x10), r2 = CR(c, 0x14);
    switch (state) {
    case EG_ATTACK: return -(int32_t)attack_step[eff_rate(c, r1 & 31)];
    case EG_DECAY1: return (int32_t)decay_step[eff_rate(c, (r1 >> 6) & 31)];
    case EG_DECAY2: return (int32_t)decay_step[eff_rate(c, (r1 >> 11) & 31)];
    default: return (int32_t)decay_step[eff_rate(c, r2 & 31)];
    }
}

static inline int32_t clamp16(int32_t v) {
#ifdef __ARM_FEATURE_SAT
    return __ssat(v, 16);
#else
    return v > 32767 ? 32767 : v < -32768 ? -32768 : v;
#endif
}

/* the chunk's samples, before the envelope: source_at's work with the source and the decoder in locals; flat,
   from a source that doesn't wrap at the end of the RAM */
static inline __attribute__((always_inline)) void quick_samples(const SOURCE* src, int pcms, int flat, int halved,
                                                                CHANNEL* v, int32_t* smp, int n, uint32_t step) {
    const uint8_t* ram = flat ? src->ram + src->sa : src->ram;
    uint32_t mask = flat ? ~0u : src->mask, sa = flat ? 0 : src->sa, lsa = src->lsa, lea = src->lea;
    uint32_t frac = v->frac, pos = v->pos;
    int32_t s0 = v->s0, s1 = v->s1, prev = v->ad_prev, ad = v->ad_step;
    int loop = src->loop, i;
    if (halved) {
        step *= 2;
    }
    for (i = 0; i < n; i += 1 + halved) {
        smp[i] = s0 + ((s1 - s0) * (int32_t)(frac >> 1) >> 15);
        frac += step;
        while (frac >= 0x10000) {
            uint32_t at;
            int k, looped = 0;
            int32_t d;
            frac -= 0x10000;
            pos++;
            if (pos >= lea) {
                pos = lsa;
                v->lp = 1;
            }
            s0 = s1;
            at = pos + 1;
            if (at >= lea) {
                if (!loop) {
                    s1 = 0;
                    continue;
                }
                at = lsa;
                looped = 1;
            }
            if (pcms == 0) {
                uint32_t a = (sa + at * 2) & mask & ~1u;
                s1 = (int16_t)(ram[a] | ram[a + 1] << 8);
                continue;
            }
            if (pcms == 1) {
                s1 = (int8_t)ram[(sa + at) & mask] << 8;
                continue;
            }
            if (at == lsa) {
                if (looped && pcms == 2 && v->ad_loop_saved) {
                    prev = v->ad_loop_prev;
                    ad = v->ad_loop_step;
                } else if (!v->ad_loop_saved) {
                    v->ad_loop_prev = prev;
                    v->ad_loop_step = ad;
                    v->ad_loop_saved = 1;
                }
            }
            k = ram[(sa + (at >> 1)) & mask] >> ((at & 1) * 4) & 15;
            d = ad * ((k & 7) * 2 + 1) >> 3;
#ifdef __ARM_FEATURE_SAT
            d = __usat(d, 15);
#else
            d = d > 32767 ? 32767 : d;
#endif
            prev = clamp16(k & 8 ? prev - d : prev + d);
            s1 = prev;
            ad = ad * adpcm_scale[k & 7] >> 8;
            ad = ad < 127 ? 127 : ad > 24576 ? 24576 : ad;
        }
    }
    v->frac = frac;
    v->pos = pos;
    v->s0 = s0;
    v->s1 = s1;
    v->ad_prev = prev;
    v->ad_step = ad;
}

/* each kind its own copy, apart from the rest of the mix, so its state stays in registers */
#define QUICK(name, pcms, flat, halved)                                                                  \
    static __attribute__((noinline)) void name(const SOURCE* src, CHANNEL* v, int32_t* smp, int n,       \
                                               uint32_t step) {                                         \
        quick_samples(src, pcms, flat, halved, v, smp, n, step);                                         \
    }
QUICK(quick_pcm16, 0, 0, 0)
QUICK(quick_pcm8, 1, 0, 0)
QUICK(quick_adpcm, 2, 0, 0)
QUICK(quick_adpcm_long, 3, 0, 0)
QUICK(flat_pcm16, 0, 1, 0)
QUICK(flat_pcm8, 1, 1, 0)
QUICK(flat_adpcm, 2, 1, 0)
QUICK(flat_adpcm_long, 3, 1, 0)
QUICK(half_pcm16, 0, 0, 1)
QUICK(half_pcm8, 1, 0, 1)
QUICK(half_adpcm, 2, 0, 1)
QUICK(half_adpcm_long, 3, 0, 1)
QUICK(half_flat_pcm16, 0, 1, 1)
QUICK(half_flat_pcm8, 1, 1, 1)
QUICK(half_flat_adpcm, 2, 1, 1)
QUICK(half_flat_adpcm_long, 3, 1, 1)

/* a chunk that reaches no loop end or start: its source samples, s0 and s1 then one a step on, apart from the
   mix; quick_samples without the checks that can't pass */
static inline __attribute__((always_inline)) void plain_source(const SOURCE* src, int pcms, int flat, CHANNEL* v,
                                                               int32_t* buf, uint32_t m) {
    const uint8_t* ram = flat ? src->ram + src->sa : src->ram;
    uint32_t mask = flat ? ~0u : src->mask, sa = flat ? 0 : src->sa;
    uint32_t at = v->pos + 2, end = at + m;
    int32_t prev = v->ad_prev, ad = v->ad_step;
    buf[0] = v->s0;
    buf[1] = v->s1;
    buf += 2;
    for (; at != end; at++) {
        if (pcms == 0) {
            uint32_t a = (sa + at * 2) & mask & ~1u;
            *buf++ = (int16_t)(ram[a] | ram[a + 1] << 8);
        } else if (pcms == 1) {
            *buf++ = (int8_t)ram[(sa + at) & mask] << 8;
        } else {
            int k = ram[(sa + (at >> 1)) & mask] >> ((at & 1) * 4) & 15;
            int32_t d = ad * ((k & 7) * 2 + 1) >> 3;
#ifdef __ARM_FEATURE_SAT
            d = __usat(d, 15);
#else
            d = d > 32767 ? 32767 : d;
#endif
            prev = clamp16(k & 8 ? prev - d : prev + d);
            *buf++ = prev;
            ad = ad * adpcm_scale[k & 7] >> 8;
            ad = ad < 127 ? 127 : ad > 24576 ? 24576 : ad;
        }
    }
    v->ad_prev = prev;
    v->ad_step = ad;
}

/* MVC2_MIXSIMD: plain_source's ADPCM a byte at a time, its two samples in turn: the same samples */
static inline int32_t adpcm_next(int k, int32_t* prev, int32_t* ad) {
    int32_t d = *ad * ((k & 7) * 2 + 1) >> 3;
#ifdef __ARM_FEATURE_SAT
    d = __usat(d, 15);
#else
    d = d > 32767 ? 32767 : d;
#endif
    *prev = clamp16(k & 8 ? *prev - d : *prev + d);
    *ad = *ad * adpcm_scale[k & 7] >> 8;
    *ad = *ad < 127 ? 127 : *ad > 24576 ? 24576 : *ad;
    return *prev;
}

static inline __attribute__((always_inline)) void pair_adpcm(const SOURCE* src, int flat, CHANNEL* v, int32_t* buf,
                                                             uint32_t m) {
    const uint8_t* ram = flat ? src->ram + src->sa : src->ram;
    uint32_t mask = flat ? ~0u : src->mask, sa = flat ? 0 : src->sa;
    uint32_t at = v->pos + 2, end = at + m;
    int32_t prev = v->ad_prev, ad = v->ad_step;
    buf[0] = v->s0;
    buf[1] = v->s1;
    buf += 2;
    if (at != end && (at & 1)) {
        *buf++ = adpcm_next(ram[(sa + (at >> 1)) & mask] >> 4, &prev, &ad);
        at++;
    }
    for (; end - at >= 2; at += 2) {
        int b = ram[(sa + (at >> 1)) & mask];
        *buf++ = adpcm_next(b & 15, &prev, &ad);
        *buf++ = adpcm_next(b >> 4, &prev, &ad);
    }
    if (at != end) {
        *buf++ = adpcm_next(ram[(sa + (at >> 1)) & mask] & 15, &prev, &ad);
    }
    v->ad_prev = prev;
    v->ad_step = ad;
}

static __attribute__((noinline)) void pair_adpcm_wrap(const SOURCE* src, CHANNEL* v, int32_t* buf, uint32_t m) {
    pair_adpcm(src, 0, v, buf, m);
}

static __attribute__((noinline)) void pair_adpcm_flat(const SOURCE* src, CHANNEL* v, int32_t* buf, uint32_t m) {
    pair_adpcm(src, 1, v, buf, m);
}

#define PLAIN(name, pcms, flat)                                                                          \
    static __attribute__((noinline)) void name(const SOURCE* src, CHANNEL* v, int32_t* buf, uint32_t m) { \
        plain_source(src, pcms, flat, v, buf, m);                                                        \
    }
PLAIN(plain_pcm16, 0, 0)
PLAIN(plain_pcm8, 1, 0)
PLAIN(plain_adpcm, 2, 0)
PLAIN(flat_plain_pcm16, 0, 1)
PLAIN(flat_plain_pcm8, 1, 1)
PLAIN(flat_plain_adpcm, 2, 1)

/* the plain chunk's samples from its source samples, at frac (16 bits) a step on, mixed at one gain or each its own */
static inline __attribute__((always_inline)) void plain_mix(const int32_t* buf, uint32_t frac, uint32_t step, int n,
                                                            int32_t g, const int32_t* gain, int32_t gl, int32_t gr) {
    int i;
    for (i = 0; i < n; i++, frac += step) {
        const int32_t* b = &buf[frac >> 16];
        int32_t s = b[0] + ((b[1] - b[0]) * (int32_t)((frac & 0xFFFF) >> 1) >> 15);
        s = s * (gain ? gain[i] : g) >> 15;
        mix_l[i] += s * gl >> 15;
        mix_r[i] += s * gr >> 15;
    }
}

static __attribute__((noinline)) void plain_one(const int32_t* buf, uint32_t frac, uint32_t step, int n, int32_t g,
                                                int32_t gl, int32_t gr) {
    plain_mix(buf, frac, step, n, g, NULL, gl, gr);
}

static __attribute__((noinline)) void plain_each(const int32_t* buf, uint32_t frac, uint32_t step, int n,
                                                 const int32_t* gain, int32_t gl, int32_t gr) {
    plain_mix(buf, frac, step, n, 0, gain, gl, gr);
}

/* MVC2_MIXSIMD=1: the plain chunk's two gains made one before the mix, and its math in ARMv6's 32x16 multiplies:
   close to the exact mix, not the same; these give the same results where there are no such instructions */
static int mix_simd;

static inline int32_t mul_wb(int32_t a, int32_t b) {
#ifdef __ARM_FEATURE_DSP
    int32_t r;
    __asm__("smulwb %0, %1, %2" : "=r"(r) : "r"(a), "r"(b));
    return r;
#else
    return (int32_t)(((int64_t)a * (int16_t)b) >> 16);
#endif
}

static inline int32_t mul_wt(int32_t a, int32_t b) {
#ifdef __ARM_FEATURE_DSP
    int32_t r;
    __asm__("smulwt %0, %1, %2" : "=r"(r) : "r"(a), "r"(b));
    return r;
#else
    return (int32_t)(((int64_t)a * (int16_t)(b >> 16)) >> 16);
#endif
}

static inline int32_t mla_wb(int32_t a, int32_t b, int32_t acc) {
#ifdef __ARM_FEATURE_DSP
    return __smlawb(a, b, acc);
#else
    return acc + mul_wb(a, b);
#endif
}

/* the source sample at frac, doubled: b[0] + (b[1] - b[0]) * f / 32768 as plain_mix has it */
static inline int32_t simd_at(const int32_t* buf, uint32_t frac) {
    const int32_t* b = &buf[frac >> 16];
    return mla_wb((b[1] - b[0]) * 2, (int32_t)((frac & 0xFFFF) >> 1), b[0]) * 2;
}

static __attribute__((noinline)) void simd_one(const int32_t* buf, uint32_t frac, uint32_t step, int n, int32_t g,
                                               int32_t gl, int32_t gr) {
    int32_t l = g * gl >> 15, r = g * gr >> 15, i;
    uint32_t lr;
    l = l > 32767 ? 32767 : l;
    r = r > 32767 ? 32767 : r;
    lr = ((uint32_t)l & 0xFFFF) | (uint32_t)r << 16;
    for (i = 0; i < n; i++, frac += step) {
        int32_t s = simd_at(buf, frac);
        mix_l[i] += mul_wb(s, (int32_t)lr);
        mix_r[i] += mul_wt(s, (int32_t)lr);
    }
}

/* the same with each sample's own gain (an envelope moving) */
static __attribute__((noinline)) void simd_each(const int32_t* buf, uint32_t frac, uint32_t step, int n,
                                                const int32_t* gain, int32_t gl, int32_t gr) {
    uint32_t lr;
    int i;
    gl = gl > 32767 ? 32767 : gl;
    gr = gr > 32767 ? 32767 : gr;
    lr = ((uint32_t)gl & 0xFFFF) | (uint32_t)gr << 16;
    for (i = 0; i < n; i++, frac += step) {
        int32_t g = gain[i] > 32767 ? 32767 : gain[i];
        int32_t s = mul_wb(simd_at(buf, frac), g) * 2;
        mix_l[i] += mul_wb(s, (int32_t)lr);
        mix_r[i] += mul_wt(s, (int32_t)lr);
    }
}

/* MVC2_MIXSIMD=2: plain_one's own math in the 32x16 multiplies, so the same results: a gain of 0x8000 (one) is
   left out, the others are below it and fit the multiply's 16 bits */
static inline __attribute__((always_inline)) void exact_mix(const int32_t* buf, uint32_t frac, uint32_t step, int n,
                                                            int32_t g, int32_t gl, int32_t gr, int one_g,
                                                            int one_l, int one_r) {
    uint32_t lr = (one_l ? 0 : (uint32_t)gl & 0xFFFF) | (one_r ? 0 : (uint32_t)gr << 16);
    int i;
    for (i = 0; i < n; i++, frac += step) {
        int32_t s = simd_at(buf, frac), t = one_g ? s >> 1 : mul_wb(s, g);
        mix_l[i] += one_l ? t : mul_wb(t * 2, (int32_t)lr);
        mix_r[i] += one_r ? t : mul_wt(t * 2, (int32_t)lr);
    }
}

#define EXACT(k)                                                                                               static __attribute__((noinline)) void exact_##k(const int32_t* buf, uint32_t frac, uint32_t step, int n,                                                     int32_t g, int32_t gl, int32_t gr) {                           exact_mix(buf, frac, step, n, g, gl, gr, (k) & 1, (k) >> 1 & 1, (k) >> 2);                           }
EXACT(0)
EXACT(1)
EXACT(2)
EXACT(3)
EXACT(4)
EXACT(5)
EXACT(6)
EXACT(7)

static void (*const exact_kind[8])(const int32_t*, uint32_t, uint32_t, int, int32_t, int32_t, int32_t) = {
    exact_0, exact_1, exact_2, exact_3, exact_4, exact_5, exact_6, exact_7,
};

/* the exact_ copy for these gains, or NULL if one is past 0x8000 */
static int exact_one(const int32_t* buf, uint32_t frac, uint32_t step, int n, int32_t g, int32_t gl, int32_t gr) {
    if ((uint32_t)g > 0x8000 || (uint32_t)gl > 0x8000 || (uint32_t)gr > 0x8000) {
        return 0;
    }
    exact_kind[(g == 0x8000) | (gl == 0x8000) << 1 | (gr == 0x8000) << 2](buf, frac, step, n, g, gl, gr);
    return 1;
}

#define PLAIN_STEP 0x20000          /* the plain path's most source samples a sample */
static int32_t plain_buf[CHUNK * (PLAIN_STEP >> 16) + 2];

/* what a channel's registers say, made again after one of them is written */
typedef struct {
    SOURCE src;
    uint32_t step;
    int32_t gl, gr, gs, tl, qf, d1;
    int32_t eg_add[4];              /* the envelope's change a sample in each state */
    uint32_t r1c;
    uint8_t pcms, voff, lpoff, isel, plfos, alfos, plfows, alfows, hold;
    uint8_t flat;                   /* the source can't wrap at the end of the RAM */
} PARAMS;

static PARAMS par[64];
static uint8_t par_ok[64];

static const PARAMS* params(int c) {
    PARAMS* p = &par[c];
    uint32_t r18, r20, r24, r28, r2;
    int oct, disdl, dipan, i;
    uint32_t base;
    if (par_ok[c]) {
        return p;
    }
    r18 = CR(c, 0x18);
    r20 = CR(c, 0x20);
    r24 = CR(c, 0x24);
    r28 = CR(c, 0x28);
    r2 = CR(c, 0x14);
    p->r1c = CR(c, 0x1C);
    oct = oct_of(c);
    base = 0x400 | (r18 & 0x3FF);
    p->step = oct + 6 >= 0 ? base << (oct + 6) : base >> -(oct + 6);
    disdl = (r24 >> 8) & 15;
    dipan = r24 & 31;
    p->gl = p->gr = level_gain[disdl];
    if (dipan & 0x10) {
        p->gr = p->gr * pan_gain[dipan & 15] >> 15;
    } else {
        p->gl = p->gl * pan_gain[dipan & 15] >> 15;
    }
    p->gs = level_gain[(r20 >> 4) & 15];
    p->isel = r20 & 15;
    p->tl = (int32_t)((r28 >> 8) & 0xFF) << 2;
    p->voff = (r28 & 0x40) != 0;
    p->lpoff = (r28 & 0x20) != 0;
    p->qf = (int32_t)(r28 & 31) * 1024 / 32 * 7 / 8;    /* resonance, Q12 */
    p->plfos = (p->r1c >> 5) & 7;
    p->alfos = p->r1c & 7;
    p->plfows = (p->r1c >> 8) & 3;
    p->alfows = (p->r1c >> 3) & 3;
    p->pcms = (CR(c, 0x00) >> 7) & 3;
    p->hold = (r2 & 0x4000) != 0;
    p->d1 = (int32_t)(((r2 >> 5) & 31) << 5) << 16;
    for (i = 0; i < 4; i++) {
        p->eg_add[i] = eg_rate(c, i);
    }
    source_of(c, &p->src);
    {
        uint32_t last = p->src.lsa > p->src.lea ? p->src.lsa : p->src.lea;     /* no index read is past it */
        uint32_t end = p->pcms == 0 ? last * 2 + 1 : p->pcms == 1 ? last : last >> 1;
        p->flat = !(p->src.sa & 1) && p->src.sa + end <= p->src.mask;
    }
    par_ok[c] = 1;
    return p;
}

/* render_quick's chunk of one gain at half the rate, each sample held for two: the channel's state the same */
static int quick_half(int c, int n, const PARAMS* p, int32_t g) {
    CHANNEL* v = &ch[c];
    int32_t smp[CHUNK], gl = p->gl, gr = p->gr, gs = p->gs;
    int i;
    switch (p->pcms | p->flat << 2) {
    case 0: half_pcm16(&p->src, v, smp, n, p->step); break;
    case 1: half_pcm8(&p->src, v, smp, n, p->step); break;
    case 2: half_adpcm(&p->src, v, smp, n, p->step); break;
    case 3: half_adpcm_long(&p->src, v, smp, n, p->step); break;
    case 4: half_flat_pcm16(&p->src, v, smp, n, p->step); break;
    case 5: half_flat_pcm8(&p->src, v, smp, n, p->step); break;
    case 6: half_flat_adpcm(&p->src, v, smp, n, p->step); break;
    default: half_flat_adpcm_long(&p->src, v, smp, n, p->step); break;
    }
    for (i = 0; i < n; i += 2) {
        int32_t s = smp[i] * g >> 15, l = s * gl >> 15, r = s * gr >> 15;
        mix_l[i] += l;
        mix_l[i + 1] += l;
        mix_r[i] += r;
        mix_r[i + 1] += r;
        smp[i] = smp[i + 1] = s;
    }
    if (gs && (mixs_on || sends_on)) {
        int isel = p->isel;
        for (i = 0; i < n; i++) {
            if (mixs_on) {
                mixs[i][isel] += (smp[i] * gs >> 15) << 4;
            }
            if (sends_on) {
                sends[i] += smp[i] * gs >> 15;
            }
        }
    }
    return 1;
}

/* a chunk with no LFO and no filter in which the channel can't end: the envelope, the samples and the mix done
   apart, or 0 when the chunk is not like that */
static int render_quick(int c, int n, const PARAMS* p) {
    CHANNEL* v = &ch[c];
    int32_t add, gain[CHUNK], smp[CHUNK], gl = p->gl, gr = p->gr, gs = p->gs, g = -1;
    int i;
    if (v->eg_state == EG_ATTACK && p->hold) {
        return 0;   /* the attack may end at the loop's start */
    }
    if (!p->src.loop && v->pos + ((v->frac + n * p->step) >> 16) >= p->src.lea) {
        return 0;
    }
    add = p->eg_add[v->eg_state];
    if (v->eg_state == EG_RELEASE && (int64_t)v->eg + (int64_t)add * n >= 0x3FF << 16) {
        return 0;
    }

    if (add == 0 && (v->eg_state >= EG_DECAY2 || (v->eg_state == EG_DECAY1 ? v->eg < p->d1 : v->eg > 0))) {
        int att = (v->eg >> 16) + p->tl;
        g = p->voff ? 0x8000 : att >= 1024 ? 0 : att_gain[att];
    } else {
        for (i = 0; i < n; i++) {
            int att;
            v->eg += p->eg_add[v->eg_state];
            if (v->eg_state == EG_ATTACK) {
                if (v->eg <= 0) {
                    v->eg = 0;
                    v->eg_state = EG_DECAY1;
                }
            } else if (v->eg_state == EG_DECAY1 && v->eg >= p->d1) {
                v->eg_state = EG_DECAY2;
            }
            if (v->eg >= 0x3FF << 16) {
                v->eg = 0x3FF << 16;
            }
            att = (v->eg >> 16) + p->tl;
            gain[i] = p->voff ? 0x8000 : att >= 1024 ? 0 : att_gain[att];
        }
    }

    if (half && g >= 0 && !(n & 1)) {
        return quick_half(c, n, p, g);
    }
    if (mix_fast && !(gs && (mixs_on || sends_on)) && p->step <= PLAIN_STEP && v->frac < 0x10000) {
        uint32_t m = (v->frac + (uint32_t)n * p->step) >> 16;     /* the steps on in the source */
        uint32_t last = v->pos + m + 1;                         /* the last index read */
        if (last < p->src.lea && (p->pcms < 2 || p->src.lsa < v->pos + 2 || p->src.lsa > last)) {
            switch ((p->pcms < 2 ? p->pcms : 2) | p->flat << 2) {
            case 0: plain_pcm16(&p->src, v, plain_buf, m); break;
            case 1: plain_pcm8(&p->src, v, plain_buf, m); break;
            case 2: (mix_simd ? pair_adpcm_wrap : plain_adpcm)(&p->src, v, plain_buf, m); break;
            case 4: flat_plain_pcm16(&p->src, v, plain_buf, m); break;
            case 5: flat_plain_pcm8(&p->src, v, plain_buf, m); break;
            default: (mix_simd ? pair_adpcm_flat : flat_plain_adpcm)(&p->src, v, plain_buf, m); break;
            }
            if (g >= 0 && mix_simd == 1) {
                simd_one(plain_buf, v->frac, p->step, n, g, gl, gr);
            } else if (g >= 0) {
                if (mix_simd != 2 || !exact_one(plain_buf, v->frac, p->step, n, g, gl, gr)) {
                    plain_one(plain_buf, v->frac, p->step, n, g, gl, gr);
                }
            } else if (mix_simd == 1) {
                simd_each(plain_buf, v->frac, p->step, n, gain, gl, gr);
            } else {
                plain_each(plain_buf, v->frac, p->step, n, gain, gl, gr);
            }
            v->frac = (v->frac + (uint32_t)n * p->step) & 0xFFFF;
            v->pos += m;
            v->s0 = plain_buf[m];
            v->s1 = plain_buf[m + 1];
            return 1;
        }
    }
    switch (p->pcms | p->flat << 2) {
    case 0: quick_pcm16(&p->src, v, smp, n, p->step); break;
    case 1: quick_pcm8(&p->src, v, smp, n, p->step); break;
    case 2: quick_adpcm(&p->src, v, smp, n, p->step); break;
    case 3: quick_adpcm_long(&p->src, v, smp, n, p->step); break;
    case 4: flat_pcm16(&p->src, v, smp, n, p->step); break;
    case 5: flat_pcm8(&p->src, v, smp, n, p->step); break;
    case 6: flat_adpcm(&p->src, v, smp, n, p->step); break;
    default: flat_adpcm_long(&p->src, v, smp, n, p->step); break;
    }

    if (g >= 0) {
        for (i = 0; i < n; i++) {
            int32_t s = smp[i] * g >> 15;
            mix_l[i] += s * gl >> 15;
            mix_r[i] += s * gr >> 15;
            smp[i] = s;
        }
    } else {
        for (i = 0; i < n; i++) {
            int32_t s = smp[i] * gain[i] >> 15;
            mix_l[i] += s * gl >> 15;
            mix_r[i] += s * gr >> 15;
            smp[i] = s;
        }
    }
    if (gs && (mixs_on || sends_on)) {
        int isel = p->isel;
        for (i = 0; i < n; i++) {
            if (mixs_on) {
                mixs[i][isel] += (smp[i] * gs >> 15) << 4;
            }
            if (sends_on) {
                sends[i] += smp[i] * gs >> 15;
            }
        }
    }
    return 1;
}

static void render_channel(int c, int n) {
    CHANNEL* h = &ch[c];
    const PARAMS* p = params(c);
    uint32_t step = p->step, r1c = p->r1c;
    int32_t gl = p->gl, gr = p->gr, gs = p->gs, tl = p->tl, qf = p->qf;
    int voff = p->voff, lpoff = p->lpoff || !filter_on, isel = p->isel;
    int plfos = p->plfos, alfos = p->alfos, plfows = p->plfows, alfows = p->alfows;
    int i;
    sent |= gs != 0;
    if (r1c & 0x8000) {
        h->lfo = 0;
    }
    if (!plfos && !alfos && lpoff && render_quick(c, n, p)) {
        return;
    }
    if (!lpoff) {
        feg_step(c, h, n);
    }
    for (i = 0; i < n; i++) {
        int32_t s, att, g;
        uint32_t st = step;
        int lfo_ph = (h->lfo >> 16) & 255;
        att = eg_step(c, h);
        if (att < 0) {
            channel_stop(c);
            break;
        }
        if (plfos || alfos) {
            if (plfos) {
                st = (uint32_t)((uint64_t)st * (uint32_t)plfo_tab[plfos][lfo_wave(plfows, lfo_ph, h->noise)] >> 16);
            }
            if (alfos) {
                att += alfo_tab[alfos][lfo_wave(alfows, lfo_ph, h->noise)];
            }
            h->lfo += lfo_step[(r1c >> 10) & 31];
            h->noise = h->noise * 1103515245u + 12345u;
        }
        s = h->s0 + (int32_t)(((int64_t)(h->s1 - h->s0) * (int32_t)(h->frac >> 1)) >> 15);
        if (!lpoff) {
            /* two-pole low-pass: y = f x + (1 - f + q) y1 - q y2, f the cutoff */
            int32_t f = (h->flv >> 16) >> 1;    /* Q12 */
            int32_t y = (int32_t)(((int64_t)f * s + (int64_t)(4096 - f + qf) * h->y1 - (int64_t)qf * h->y2) >> 12);
            y = y > 32767 ? 32767 : y < -32768 ? -32768 : y;
            h->y2 = h->y1;
            h->y1 = y;
            s = y;
        }
        if (!voff) {
            att += tl;
            g = att >= 1024 ? 0 : att_gain[att];
            s = s * g >> 15;
        }
        mix_l[i] += s * gl >> 15;
        mix_r[i] += s * gr >> 15;
        if (gs && mixs_on) {
            mixs[i][isel] += (s * gs >> 15) << 4;
        }
        if (gs) {
            if (sends_on) {
                sends[i] += s * gs >> 15;
            }
        }
        h->frac += st;
        while (h->frac >= 0x10000) {
            uint32_t nx, nx2;
            int looped;
            h->frac -= 0x10000;
            if (!next_index(c, h->pos, &nx, &looped)) {
                channel_stop(c);
                h->lp = 1;
                break;
            }
            h->lp |= looped;
            h->pos = nx;
            h->s0 = h->s1;
            if (h->eg_state == EG_ATTACK && (CR(c, 0x14) & 0x4000) && h->pos >= CR(c, 0x08)) {
                h->eg_state = EG_DECAY1;
            }
            h->s1 = next_index(c, nx, &nx2, &looped) ? sample_at(c, nx2, looped) : 0;
        }
        if (!ch_on[c]) {
            break;
        }
    }
}

/* ---- the DSP ---- */

static DSP_REGS dsp;
static int dsp_last;                /* the steps up to the last that does something */
static int dsp_dirty = 1;
static int dsp_jit = 1;             /* MVC2_DSPJIT */
#ifdef AICA_DSP_JIT
static int dsp_made;                /* the program translated */
static unsigned dsp_makes, dsp_words;
static int dsp_fast;                /* the translated program runs this chunk */
static int dsp_table;               /* the program reads or writes the RAM as a table */
static uint16_t dsp_written;        /* the effect outputs it writes */
static struct {
    uint8_t e, right;
    int32_t lv, pan;
} dsp_outs[16];                     /* those written and not silent: the chunk's output mix */
static int dsp_outs_n;
#endif

static int32_t sx24(int32_t v) {
    return (int32_t)((uint32_t)v << 8) >> 8;
}

static uint16_t dsp_pack(int32_t val) {
    int sign = (val >> 23) & 1, e = 0;
    uint32_t t = (uint32_t)(val ^ (val << 1)) & 0xFFFFFF;
    while (e < 12 && !(t & 0x800000)) {
        t <<= 1;
        e++;
    }
    val = e < 12 ? (val << e) & 0x3FFFFF : val << 11;
    val >>= 11;
    return (uint16_t)((val & 0x7FF) | sign << 15 | e << 11);
}

static int32_t dsp_unpack(uint16_t v) {
    int sign = v >> 15 & 1, e = v >> 11 & 15;
    int32_t m = (int32_t)(v & 0x7FF) << 11;
    if (e > 11) {
        e = 11;
        m |= sign << 22;
    } else {
        m |= (sign ^ 1) << 22;
    }
    m |= sign << 23;
    return sx24(m) >> e;
}

static DSP_STEP prog[128];

static void dsp_scan(void) {
    int s;
    dsp_last = 0;
    for (s = 0; s < 128; s++) {
        uint32_t o = 0x3400 + s * 16, i0 = reg16(o), i1 = reg16(o + 4), i2 = reg16(o + 8), i3 = reg16(o + 12);
        DSP_STEP* p = &prog[s];
        if (i0 | i1 | i2 | i3) {
            dsp_last = s + 1;
        }
        p->tra = (i0 >> 9) & 0x7F;
        p->twa = (i0 >> 1) & 0x7F;
        p->ysel = (i1 >> 13) & 3;
        p->ira = (i1 >> 7) & 0x3F;
        p->iwa = (i1 >> 1) & 0x1F;
        p->ewa = (i2 >> 8) & 15;
        p->shift = (i2 >> 4) & 3;
        p->masa = (i3 >> 9) & 0x1F;
        p->f = (uint16_t)((i0 >> 8 & 1) * DF_TWT | (i1 >> 15 & 1) * DF_XSEL | (i1 >> 6 & 1) * DF_IWT |
                          (i2 >> 15 & 1) * DF_TABLE | (i2 >> 14 & 1) * DF_MWT | (i2 >> 13 & 1) * DF_MRD |
                          (i2 >> 12 & 1) * DF_EWT | (i2 >> 7 & 1) * DF_ADRL | (i2 >> 6 & 1) * DF_FRCL |
                          (i2 >> 3 & 1) * DF_YRL | (i2 >> 2 & 1) * DF_NEGB | (i2 >> 1 & 1) * DF_ZERO |
                          (i2 & 1) * DF_BSEL | (i3 >> 15 & 1) * DF_NOFL | (i3 >> 8 & 1) * DF_ADREB |
                          (i3 >> 7 & 1) * DF_NXADR);
        if (!(s & 1)) {
            p->f &= (uint16_t)~(DF_MRD | DF_MWT);       /* the memory is read and written on odd steps only */
        }
        p->coef = (int16_t)((int16_t)reg16(0x3000 + s * 4) >> 3);   /* each step has its own coefficient */
        p->madrs = (uint16_t)reg16(0x3200 + p->masa * 4);
    }
    dsp_dirty = 0;
#ifdef AICA_DSP_JIT
    dsp_made = 0;
    if (dsp_jit && dsp_last) {
        dsp_words = (unsigned)aica_dsp_jit_make(prog, dsp_last, &dsp);
        dsp_made = dsp_words > 0;
        memset(dsp.efreg, 0, sizeof dsp.efreg);     /* those the program doesn't write stay 0 */
        dsp_makes++;
        dsp_table = 0;
        dsp_written = 0;
        for (s = 0; s < dsp_last; s++) {
            dsp_table |= (prog[s].f & (DF_MRD | DF_MWT)) && (prog[s].f & DF_TABLE);
            if (prog[s].f & DF_EWT) {
                dsp_written |= (uint16_t)(1u << prog[s].ewa);
            }
        }
    }
#endif
}

#ifdef AICA_DSP_JIT
/* MVC2_DSPJIT=2: each sample made by both, from the same state; the C's kept */
static struct {
    uint32_t addr;
    uint16_t old, now;
} dsp_log[128];
static int dsp_log_n = -1;

static void dsp_logged(uint32_t addr) {
    if (dsp_log_n >= 0 && dsp_log_n < 128) {
        dsp_log[dsp_log_n].addr = addr;
        dsp_log[dsp_log_n++].old = (uint16_t)(ram[addr] | ram[addr + 1] << 8);
    }
}

static void dsp_sample(const int32_t* mix);

static void dsp_check(const int32_t* mix) {
    static DSP_REGS before, c;
    static unsigned long checked, wrong;
    int k, bad = 0;
    before = dsp;
    dsp_log_n = 0;
    dsp_sample(mix);
    c = dsp;
    for (k = 0; k < dsp_log_n; k++) {
        dsp_log[k].now = (uint16_t)(ram[dsp_log[k].addr] | ram[dsp_log[k].addr + 1] << 8);
    }
    for (k = dsp_log_n; k-- > 0;) {
        ram[dsp_log[k].addr] = (uint8_t)dsp_log[k].old;
        ram[dsp_log[k].addr + 1] = (uint8_t)(dsp_log[k].old >> 8);
    }
    dsp = before;
    aica_dsp_jit_run(&dsp, mix);
    if (memcmp(&dsp, &c, offsetof(DSP_REGS, ring)) != 0) {
        bad = 1;
    }
    for (k = 0; k < dsp_log_n; k++) {
        uint32_t a = dsp_log[k].addr;
        if ((uint16_t)(ram[a] | ram[a + 1] << 8) != dsp_log[k].now) {
            bad = 1;
        }
    }
    checked++;
    if (bad && wrong++ < 8) {
        int i;
        host_log("sound DSP check: sample %lu wrong (mdec %u)", checked, (unsigned)c.mdec);
        for (i = 0; i < (int)(offsetof(DSP_REGS, ring) / 4); i++) {
            if (((const int32_t*)&dsp)[i] != ((const int32_t*)&c)[i]) {
                host_log("  word %d: %08lx, C %08lx", i, (unsigned long)((const int32_t*)&dsp)[i],
                         (unsigned long)((const int32_t*)&c)[i]);
            }
        }
    }
    if (checked % 441000 == 0) {
        host_log("sound DSP check: %lu samples, %lu wrong", checked, wrong);
    }
    dsp = c;
    for (k = 0; k < dsp_log_n; k++) {
        ram[dsp_log[k].addr] = (uint8_t)dsp_log[k].now;
        ram[dsp_log[k].addr + 1] = (uint8_t)(dsp_log[k].now >> 8);
    }
    dsp_log_n = -1;
}
#endif

static void dsp_sample(const int32_t* mix) {
    uint32_t rbl = 8192u << ((reg16(0x2804) >> 13) & 3), rbp = reg16(0x2804) & 0xFFF;
    int32_t acc = 0, inputs = 0;
    int s;
    memset(dsp.efreg, 0, sizeof dsp.efreg);
    for (s = 0; s < dsp_last; s++) {
        const DSP_STEP* p = &prog[s];
        uint32_t f = p->f;
        int32_t b, x, y, shifted, t;
        if (p->ira <= 0x1F) {
            inputs = sx24(dsp.mems[p->ira]);
        } else if (p->ira <= 0x2F) {
            inputs = sx24(mix[p->ira - 0x20] << 4);
        } else {
            inputs = 0;
        }
        if (f & DF_IWT) {
            dsp.mems[p->iwa] = dsp.memval;
            if (p->ira == p->iwa) {
                inputs = dsp.memval;
            }
        }
        t = sx24(dsp.temp[(p->tra + dsp.mdec) & 0x7F]);
        b = f & DF_ZERO ? 0 : f & DF_BSEL ? acc : t;
        if ((f & (DF_NEGB | DF_ZERO)) == DF_NEGB) {
            b = -b;
        }
        x = f & DF_XSEL ? inputs : t;
        switch (p->ysel) {
        case 0: y = dsp.frc; break;
        case 1: y = p->coef; break;
        case 2: y = (dsp.y >> 11) & 0x1FFF; break;
        default: y = (dsp.y >> 4) & 0x0FFF; break;
        }
        if (f & DF_YRL) {
            dsp.y = inputs;
        }
        switch (p->shift) {
        case 0: shifted = acc > 0x7FFFFF ? 0x7FFFFF : acc < -0x800000 ? -0x800000 : acc; break;
        case 1: shifted = acc * 2; shifted = shifted > 0x7FFFFF ? 0x7FFFFF : shifted < -0x800000 ? -0x800000 : shifted; break;
        case 2: shifted = sx24(acc * 2); break;
        default: shifted = sx24(acc); break;
        }
        y = (int32_t)((uint32_t)y << 19) >> 19;
        acc = (int32_t)(((int64_t)x * y) >> 12) + b;
        if (f & DF_TWT) {
            dsp.temp[(p->twa + dsp.mdec) & 0x7F] = shifted;
        }
        if (f & DF_FRCL) {
            dsp.frc = p->shift == 3 ? shifted & 0xFFF : (shifted >> 11) & 0x1FFF;
        }
        if (f & (DF_MRD | DF_MWT)) {
            uint32_t addr = p->madrs;
            if (!(f & DF_TABLE)) {
                addr += dsp.mdec;
            }
            if (f & DF_ADREB) {
                addr += dsp.adrs & 0xFFF;
            }
            if (f & DF_NXADR) {
                addr++;
            }
            addr &= f & DF_TABLE ? 0xFFFF : rbl - 1;
            addr = ((addr + (rbp << 10)) * 2) & aica_ram_mask;
            if (f & DF_MRD) {
                uint16_t w = (uint16_t)(ram[addr] | ram[addr + 1] << 8);
                dsp.memval = f & DF_NOFL ? sx24((int32_t)(int16_t)w << 8) : dsp_unpack(w);
            }
            if (f & DF_MWT) {
                uint16_t w = f & DF_NOFL ? (uint16_t)(shifted >> 8) : dsp_pack(shifted);
#ifdef AICA_DSP_JIT
                dsp_logged(addr);
#endif
                ram[addr] = (uint8_t)w;
                ram[addr + 1] = (uint8_t)(w >> 8);
                arm7_code_written(addr, 2);
            }
        }
        if (f & DF_ADRL) {
            dsp.adrs = p->shift == 3 ? (shifted >> 12) & 0xFFF : inputs >> 16;
        }
        if (f & DF_EWT) {
            dsp.efreg[p->ewa] += shifted >> 8;
        }
    }
    dsp.mdec--;
}

#ifdef AICA_DSP_JIT
/* the chunk's ring and outputs for the translated program, or 0 where the ring wraps or the code is in it */
static int dsp_chunk(void) {
    uint32_t r = reg16(0x2804), rbl = 8192u << ((r >> 13) & 3), start = (r & 0xFFF) << 11;
    uint32_t span = (dsp_table && rbl < 0x10000 ? 0x10000 : rbl) * 2;
    int e;
    if (!dsp_made || start < arm7_code_limit() || start + span > aica_ram_mask + 1) {
        return 0;
    }
    dsp.ring = ram + start;
    dsp.rbl_mask = rbl - 1;
    dsp_outs_n = 0;
    for (e = 0; e < 16; e++) {
        uint32_t ef = reg16(0x2000 + e * 4);
        int32_t lv = level_gain[(ef >> 8) & 15];
        if ((dsp_written >> e & 1) && lv) {
            dsp_outs[dsp_outs_n].e = (uint8_t)e;
            dsp_outs[dsp_outs_n].right = (ef & 0x10) != 0;
            dsp_outs[dsp_outs_n].lv = lv;
            dsp_outs[dsp_outs_n++].pan = pan_gain[ef & 15];
        }
    }
    return 1;
}

/* a sample of the translated program, added to l and r as aica_render's loop adds the outputs */
static void dsp_fast_sample(const int32_t* mix, int32_t* l, int32_t* r) {
    int k;
    if (dsp_jit == 2) {
        dsp_check(mix);
    } else {
        aica_dsp_jit_run(&dsp, mix);
    }
    for (k = 0; k < dsp_outs_n; k++) {
        int32_t v = out_level(dsp.efreg[dsp_outs[k].e]), sl, sr;
        if (!v) {
            continue;
        }
        sl = sr = v * dsp_outs[k].lv >> 15;
        if (dsp_outs[k].right) {
            sr = sr * dsp_outs[k].pan >> 15;
        } else {
            sl = sl * dsp_outs[k].pan >> 15;
        }
        *l += sl;
        *r += sr;
    }
}
#endif

/* ---- the timers and the interrupts ---- */

static uint32_t scipd, mcipd, intreq;
static int timer_count[3], timer_sub[3];
static int timer_load[3] = { -1, -1, -1 };  /* written by the CPU in its run: set once the timers reach that time */
static int in_cpu;
static int timer_fast = 1;          /* MVC2_TIMERFAST=0: the timers counted a step at a time */

static void update_irq(void) {
    uint32_t p = reg16(0x289C) & scipd & 0x7FF;
    intreq = 0;
    if (p) {
        int i = 0;
        while (!(p >> i & 1)) {
            i++;
        }
        if (i > 7) {
            i = 7;
        }
        intreq = (reg16(0x28A8) >> i & 1) | (reg16(0x28AC) >> i & 1) << 1 | (reg16(0x28B0) >> i & 1) << 2;
    }
    arm7_set_fiq(intreq != 0);
}

static FILE* trace;                 /* MVC2_SNDTRACE: each register write, with the sample it came at */
static uint32_t trace_time;

static int timers_tick(int n) {
    int t, raised = 0;
    trace_time += (uint32_t)n;
    for (t = 0; t < 3; t++) {
        int shift = (reg16(0x2890 + t * 4) >> 8) & 7, prescale = 1 << shift;
        timer_sub[t] += n;
        if (timer_fast) {
            timer_count[t] += timer_sub[t] >> shift;   /* from at most 0xFF, each overflow back to 0 */
            timer_sub[t] &= prescale - 1;
            if (timer_count[t] > 0xFF) {
                timer_count[t] &= 0xFF;
                scipd |= 0x40u << t;
                mcipd |= 0x40u << t;
                raised = 1;
            }
            continue;
        }
        while (timer_sub[t] >= prescale) {
            timer_sub[t] -= prescale;
            if (++timer_count[t] > 0xFF) {
                timer_count[t] = 0;
                scipd |= 0x40u << t;
                mcipd |= 0x40u << t;
                raised = 1;
            }
        }
    }
    scipd |= 0x400;                 /* a sample has passed */
    if (raised || (reg16(0x289C) & 0x400)) {
        update_irq();
    }
    return raised;
}

/* the samples until a timer next overflows, up to n */
static int timers_next(int n) {
    int t;
    for (t = 0; t < 3; t++) {
        int prescale = 1 << ((reg16(0x2890 + t * 4) >> 8) & 7);
        int left = (0x100 - timer_count[t]) * prescale - timer_sub[t];
        n = left < n ? left : n;
    }
    return n > 0 ? n : 1;
}

/* the CPU for len samples, stopped at each timer's overflow. The handler runs at once, outside the samples' cycles:
   it reloads the tempo timer, so a tick lasts the timer's count whatever the CPU's rate. */
#define HANDLER_MAX 1024            /* cycles the handler may take at once */
static void run_cpu(int len) {
    int done = 0, cycles = cpu_debt, t;
    while (done < len) {
        int seg = timers_next(len - done), reached;
        if (!cpu_reset && cycles < (done + seg) * cpu_cycles) {
            in_cpu = 1;
            int ran = snd_drv_main((done + seg) * cpu_cycles - cycles, cpu_cycles);
            if (ran < 0) {
                ran = arm7_run((done + seg) * cpu_cycles - cycles);
            }
            cycles += ran;
            PROF_COUNT(PZ_SND_ARM, (uint32_t)ran);
            in_cpu = 0;
        }
        reached = cpu_reset ? done + seg : cycles / cpu_cycles;
        reached = reached < done + seg ? reached : done + seg;
        if (reached > done) {
            int raised = timers_tick(reached - done);
            done = reached;
            if (raised && !cpu_reset) {
                PROF_START(f);
                PERF(AICA_PERF_FIQ);
                if (!snd_drv_fiq()) {
                    arm7_run_fiq(HANDLER_MAX);
                }
                PERF(AICA_PERF_FIQ_END);
                PROF_STOP(PZ_SND_FIQ, f);
                PROF_COUNT(PZ_SND_FIQ, 1);
            }
        }
        for (t = 0; t < 3; t++) {
            if (timer_load[t] >= 0) {
                timer_count[t] = timer_load[t];
                timer_load[t] = -1;
            }
        }
    }
    cpu_debt = cpu_reset ? 0 : cycles - len * cpu_cycles;
}

/* ---- the registers ---- */

static uint32_t read_reg16(uint32_t off) {
    off &= 0x7FFE;
    if (off < 0x2000) {
        if ((off & 0x7F) == 0) {
            return reg16(off) & 0x7FFF;     /* KYONEX reads 0 */
        }
        return reg16(off);
    }
    switch (off) {
    case 0x2808: return 0x0900;             /* MIDI: nothing in, nothing out */
    case 0x2810: {
        int c = (reg16(0x280C) >> 8) & 63;
        CHANNEL* h = &ch[c];
        uint32_t v = (uint32_t)h->lp << 15 | (uint32_t)(h->eg_state & 3) << 13 | (uint32_t)(h->eg >> 16) << 3;
        if (reg16(0x280C) & 0x4000) {
            v = (v & 0xE000) | (uint32_t)(h->flv >> 16);
        }
        h->lp = 0;
        return v;
    }
    case 0x2814: return ch[(reg16(0x280C) >> 8) & 63].pos & 0xFFFF;
    case 0x2890: case 0x2894: case 0x2898:
        return (reg16(off) & 0x0700) | (uint32_t)timer_count[(off - 0x2890) / 4];
    case 0x28A0: return scipd;
    case 0x28B8: return mcipd;
    case 0x2D00: return intreq;
    default: return reg16(off);
    }
}

void (*aica_write_log)(uint32_t off, uint32_t v, uint32_t mask);
uint32_t aica_watch_gen;

static void write_reg16(uint32_t off, uint32_t v, uint32_t mask) {
    uint32_t old;
    off &= 0x7FFE;
    if (aica_write_log != NULL) {
        aica_write_log(off, v, mask);
    }
    if (trace != NULL) {
        uint32_t w[3] = { trace_time, off | mask << 16, v & mask };
        fwrite(w, sizeof w, 1, trace);
    }
    old = reg16(off);
    v = (old & ~mask) | (v & mask);
    if (off < 0x2000 ? (off & 0x7F) == 0 || (off & 0x7F) == 0x08 : off == 0x280C) {
        aica_watch_gen++;
    }
    if (off == 0x28A0) {                    /* SCIPD: only the SH4's interrupt can be raised */
        scipd |= v & 0x20 & mask;
        update_irq();
        return;
    }
    if (off == 0x28A4) {                    /* SCIRE: clears what it names */
        scipd &= ~(v & mask);
        update_irq();
        return;
    }
    if (off == 0x28B8) {
        mcipd |= v & 0x20 & mask;
        return;
    }
    if (off == 0x28BC) {
        mcipd &= ~(v & mask);
        return;
    }
    reg[off] = (uint8_t)v;
    reg[off + 1] = (uint8_t)(v >> 8);
    if (off < 0x2000) {
        par_ok[off >> 7] = 0;
        if ((off & 0x7F) == 0 && (v & 0x8000)) {
            reg[off + 1] &= 0x7F;
            key_execute();
        }
        return;
    }
    if (off >= 0x3000 && off < 0x3C00) {
        dsp_dirty = 1;
        return;
    }
    switch (off) {
    case 0x2890: case 0x2894: case 0x2898:
        if (!(mask & 0xFF)) {
            break;                          /* the prescale alone */
        }
        if (in_cpu) {
            timer_load[(off - 0x2890) / 4] = (int)(v & 0xFF);
            arm7_stop();
        } else {
            timer_count[(off - 0x2890) / 4] = (int)(v & 0xFF);
        }
        break;
    case 0x289C: case 0x28A8: case 0x28AC: case 0x28B0: case 0x2D04:
        update_irq();
        break;
    case 0x2C00:
        if ((v & 1) && !cpu_reset) {
            cpu_reset = 1;
        } else if (!(v & 1) && cpu_reset) {
            cpu_reset = 0;
            cpu_debt = 0;
            arm7_reset();
        }
        break;
    default:
        break;
    }
}

/* the sound CPU's side: the RAM in arm7.c, the registers from 0x800000 */
uint32_t aica_arm_read32(uint32_t a) {
    IO_COUNT();
    if ((a & 0xFFFF8000u) == 0x800000u) {
        return read_reg16(a & 0x7FFF);
    }
    return 0;
}

uint32_t aica_arm_read16(uint32_t a) {
    return aica_arm_read32(a & ~3u) >> (a & 2) * 8 & 0xFFFF;
}

uint32_t aica_arm_read8(uint32_t a) {
    IO_COUNT();
    if ((a & 0xFFFF8000u) == 0x800000u) {
        return read_reg16(a & 0x7FFE) >> (a & 1) * 8 & 0xFF;
    }
    return 0;
}

void aica_arm_write32(uint32_t a, uint32_t v) {
    IO_COUNT();
    if ((a & 0xFFFF8000u) == 0x800000u) {
        write_reg16(a & 0x7FFF, v, 0xFFFF);
    }
}

void aica_arm_write16(uint32_t a, uint32_t v) {
    IO_COUNT();
    if ((a & 0xFFFF8000u) == 0x800000u && !(a & 2)) {
        write_reg16(a & 0x7FFF, v, 0xFFFF);
    }
}

void aica_arm_write8(uint32_t a, uint32_t v) {
    IO_COUNT();
    if ((a & 0xFFFF8000u) == 0x800000u) {
        write_reg16(a & 0x7FFE, v << (a & 1) * 8, 0xFFu << (a & 1) * 8);
    }
}

/* what the sound CPU's writes to the registers can change, kept and put back around a check */
struct AICA_STATE {
    uint8_t reg[sizeof reg];
    CHANNEL ch[64];
    uint8_t ch_on[64], par_ok[64];
    uint32_t scipd, mcipd, intreq;
    int timer_count[3], timer_sub[3], timer_load[3];
    int dsp_dirty, cpu_reset, cpu_debt;
};

AICA_STATE* aica_state_new(void) {
    return malloc(sizeof(AICA_STATE));
}

void aica_state_save(AICA_STATE* s) {
    memcpy(s->reg, reg, sizeof reg);
    memcpy(s->ch, ch, sizeof ch);
    memcpy(s->ch_on, ch_on, sizeof ch_on);
    memcpy(s->par_ok, par_ok, sizeof par_ok);
    s->scipd = scipd;
    s->mcipd = mcipd;
    s->intreq = intreq;
    memcpy(s->timer_count, timer_count, sizeof timer_count);
    memcpy(s->timer_sub, timer_sub, sizeof timer_sub);
    memcpy(s->timer_load, timer_load, sizeof timer_load);
    s->dsp_dirty = dsp_dirty;
    s->cpu_reset = cpu_reset;
    s->cpu_debt = cpu_debt;
}

void aica_state_load(const AICA_STATE* s) {
    aica_watch_gen++;
    memcpy(reg, s->reg, sizeof reg);
    memcpy(ch, s->ch, sizeof ch);
    memcpy(ch_on, s->ch_on, sizeof ch_on);
    memcpy(par_ok, s->par_ok, sizeof par_ok);
    scipd = s->scipd;
    mcipd = s->mcipd;
    intreq = s->intreq;
    memcpy(timer_count, s->timer_count, sizeof timer_count);
    memcpy(timer_sub, s->timer_sub, sizeof timer_sub);
    memcpy(timer_load, s->timer_load, sizeof timer_load);
    dsp_dirty = s->dsp_dirty;
    cpu_reset = s->cpu_reset;
    cpu_debt = s->cpu_debt;
}

/* the SH4's side */
uint8_t* aica_sh4_ptr(uint32_t addr) {
    return ram + (addr & 0x00FFFFFFu & aica_ram_mask);
}

uint32_t aica_ram_size(void) {
    return ram != NULL ? aica_ram_mask + 1 : 0;
}

uint32_t aica_sh4_read32(uint32_t addr) {
    addr &= 0x00FFFFFFu;
    if (addr >= 0x800000u) {
        uint32_t v;
        memcpy(&v, ram + ((addr - 0x800000u) & aica_ram_mask & ~3u), 4);
        return v;
    }
    if ((addr & 0xFF8000u) == 0x700000u) {
        return read_reg16(addr & 0x7FFF);
    }
    return 0;
}

void aica_sh4_write32(uint32_t addr, uint32_t v) {
    addr &= 0x00FFFFFFu;
    if (addr >= 0x800000u) {
        memcpy(ram + ((addr - 0x800000u) & aica_ram_mask & ~3u), &v, 4);
        arm7_code_written((addr - 0x800000u) & aica_ram_mask & ~3u, 4);
    } else if ((addr & 0xFF8000u) == 0x700000u) {
        write_reg16(addr & 0x7FFF, v, 0xFFFF);
    }
}

/* ---- the whole ---- */

int aica_init(int ram_8mb) {
    static int tables;
    uint32_t size = ram_8mb ? 8u << 20 : 2u << 20;
    if (!tables) {
        make_tables();
        tables = 1;
    }
    /* the NAOMI's size for both, so that a switch either way finds it whole (a build with the Dreamcast's RAM alone
       has no switch) */
    /* with cache lines of its own, for a second CPU to make the sound (psp_audio.c) */
    if ((ram = host_keep_block((host_ram_size < SH4_RAM_SIZE ? size : 8u << 20) + 128)) == NULL) {
        return 0;
    }
    ram = (uint8_t*)(((uintptr_t)ram + 63) & ~(uintptr_t)63);
    aica_ram = ram;
    memset(ram, 0, size);
    memset(reg, 0, sizeof reg);
    memset(ch, 0, sizeof ch);
    memset(ch_on, 0, sizeof ch_on);
    memset(par_ok, 0, sizeof par_ok);
    memset(dsp.temp, 0, sizeof dsp.temp);
    memset(dsp.mems, 0, sizeof dsp.mems);
    dsp.frc = dsp.y = dsp.adrs = dsp.memval = 0;
    dsp.mdec = 0;
    dsp_dirty = 1;
    scipd = mcipd = intreq = 0;
    memset(timer_count, 0, sizeof timer_count);
    memset(timer_sub, 0, sizeof timer_sub);
    aica_ram_mask = size - 1;
    cpu_reset = 1;
    reg[0x2C00] = 1;
    arm7_reset();
    return 1;
}

void aica_free(void) {
    ram = aica_ram = NULL;      /* host_keep_block's */
    aica_ram_mask = 0;
}

int aica_tracing(void) {
    return trace != NULL;
}

int aica_cpu_running(void) {
    return !cpu_reset;
}

void aica_run_cpu(int cycles) {
    if (!cpu_reset) {
#ifdef MVC2_PROF
        game_side = 1;
#endif
        arm7_run_interpreted(cycles);   /* the translated code is for the sound's own thread */
#ifdef MVC2_PROF
        game_side = 0;
#endif
    }
}

void aica_set_dsp_jit(int mode) {
    dsp_jit = mode;
    dsp_dirty = 1;
}

void aica_set_dsp(int on) {
    dsp_on = on;
}

/* the sound CPU's cycles a sample: the driver's main loop only polls between ticks */
void aica_set_cpu_rate(int cycles) {
    cpu_cycles = cycles > 0 ? cycles : CPU_CYCLES_PER_SAMPLE;
}

void aica_set_filter(int on) {
    filter_on = on;
}

void aica_set_half(int on) {
    half = on;
}

void aica_set_mix_fast(int on) {
    mix_fast = on;
}

void aica_set_mix_simd(int on) {
    mix_simd = on;
}

void aica_set_timer_fast(int on) {
    timer_fast = on;
}

void aica_trace(const char* path) {
    trace = fopen(path, "wb");
}

void aica_set_chunk(int n) {
    chunk = n >= 1 && n <= CHUNK ? n : 16;
}

static unsigned last_active;

void aica_stats(AICA_STATS* s) {
    s->cpu_cycles = arm7_cycles();
    s->active_channels = last_active;
    s->dsp_steps = (unsigned)dsp_last;
}

/* the channels sounding and the DSP, for MVC2_SNDDBG */
void aica_dump(void (*log)(const char* fmt, ...)) {
    int c;
#ifdef AICA_DSP_JIT
    log("aica: dsp translated %d (%u times, %u words), running %d, %d outputs", dsp_made, dsp_makes, dsp_words,
        dsp_fast, dsp_outs_n);
#endif
    log("aica: mvol %u, dsp %d steps, rbp %X rbl %u, scieb %X scipd %X timers %X %X %X, pc %08X", reg16(0x2800) & 15,
        dsp_last, reg16(0x2804) & 0xFFF, (reg16(0x2804) >> 13) & 3, reg16(0x289C), scipd, reg16(0x2890),
        reg16(0x2894), reg16(0x2898), arm7_pc());
    {
        uint32_t rb = (reg16(0x2804) & 0xFFF) << 11, i, nz = 0, z6 = 0;
        for (i = 0; i < (8192u << ((reg16(0x2804) >> 13) & 3)) * 2; i += 2) {
            uint16_t w = (uint16_t)(ram[(rb + i) & aica_ram_mask] | ram[(rb + i + 1) & aica_ram_mask] << 8);
            nz += w != 0;
            z6 += (w & 0x7800) >= 0x5800;
        }
        log("  ring at %X: %u words not 0, %u with exponent >= 11; efreg %d %d %d %d", rb, nz, z6, dsp.efreg[0], dsp.efreg[1],
            dsp.efreg[2], dsp.efreg[3]);
        if (getenv("MVC2_DSPDUMP")) {
            for (i = 0; i < (uint32_t)dsp_last; i++) {
                uint32_t o = 0x3400 + i * 16;
                log("  step %3u: %04X %04X %04X %04X", i, reg16(o), reg16(o + 4), reg16(o + 8), reg16(o + 12));
            }
            for (i = 0; i < 64; i += 8) {
                log("  coef %2u: %04X %04X %04X %04X %04X %04X %04X %04X", i, reg16(0x3000 + i * 4),
                    reg16(0x3004 + i * 4), reg16(0x3008 + i * 4), reg16(0x300C + i * 4), reg16(0x3010 + i * 4),
                    reg16(0x3014 + i * 4), reg16(0x3018 + i * 4), reg16(0x301C + i * 4));
            }
            for (i = 0; i < 32; i += 8) {
                log("  madrs %2u: %04X %04X %04X %04X %04X %04X %04X %04X", i, reg16(0x3200 + i * 4),
                    reg16(0x3204 + i * 4), reg16(0x3208 + i * 4), reg16(0x320C + i * 4), reg16(0x3210 + i * 4),
                    reg16(0x3214 + i * 4), reg16(0x3218 + i * 4), reg16(0x321C + i * 4));
            }
        }
    }
    for (c = 0; c < 64; c++) {
        CHANNEL* h = &ch[c];
        if (!ch_on[c]) {
            continue;
        }
        log("  ch%02d pcms %u sa %06X lsa %04X lea %04X lp %u oct %d fns %03X tl %02X disdl %u pan %02X imxl %u isel %u "
            "eg %d/%03X ar %u d1 %u d2 %u rr %u dl %u pos %04X lpoff %u q %u flv %04X",
            c, (CR(c, 0) >> 7) & 3, (CR(c, 0) & 0x7F) << 16 | CR(c, 4), CR(c, 8), CR(c, 12), (CR(c, 0) >> 9) & 1,
            oct_of(c), CR(c, 0x18) & 0x3FF, CR(c, 0x28) >> 8, (CR(c, 0x24) >> 8) & 15, CR(c, 0x24) & 31,
            (CR(c, 0x20) >> 4) & 15, CR(c, 0x20) & 15, h->eg_state, h->eg >> 16, CR(c, 0x10) & 31,
            (CR(c, 0x10) >> 6) & 31, (CR(c, 0x10) >> 11) & 31, CR(c, 0x14) & 31, (CR(c, 0x14) >> 5) & 31, h->pos,
            (CR(c, 0x28) >> 5) & 1, CR(c, 0x28) & 31, h->flv >> 16);
    }
}

/* ---- a cheap reverb in the DSP's place, when it is off (MVC2_REVERB) ---- */

/* set to the arcade program's response, at half the rate; the Dreamcast loads no program, so it has none */

#define RV_SIZE 8192                /* one buffer for every delay, moving a word a step */
#define RV_HOLD (4 * 44100)         /* samples it runs after the last send */
enum {
    /* the delays, in steps of two samples */
    RV_PRE = 47, RV_AP1 = 71, RV_AP2 = 53, RV_AP3 = 189, RV_AP4 = 139,
    RV_C1 = 558, RV_C2 = 638, RV_C3 = 711, RV_C4 = 778, RV_OUTL = 112, RV_OUTR = 124,
    /* each one's newest word: after the one before, a word between */
    RV_E_PRE = RV_PRE + 1, RV_E_AP1 = RV_E_PRE + RV_AP1 + 1, RV_E_AP2 = RV_E_AP1 + RV_AP2 + 1,
    RV_E_AP3 = RV_E_AP2 + RV_AP3 + 1, RV_E_AP4 = RV_E_AP3 + RV_AP4 + 1, RV_E_C1 = RV_E_AP4 + RV_C1 + 1,
    RV_E_C2 = RV_E_C1 + RV_C2 + 1, RV_E_C3 = RV_E_C2 + RV_C3 + 1, RV_E_C4 = RV_E_C3 + RV_C4 + 1,
    RV_E_OUTL = RV_E_C4 + RV_OUTL + 1, RV_E_OUTR = RV_E_OUTL + RV_OUTR + 1
};
_Static_assert(RV_E_OUTR < RV_SIZE / 2, "the reverb's delays fit its buffer");
static int32_t rv_ring[RV_SIZE];
static int32_t rv_fb[4], rv_damp[4];
static int32_t rv_in[2], rv_l[2], rv_r[2];  /* the send's last odd and even samples; the last two steps' out */
static uint32_t rv_pos;
static int rv_on, rv_odd, rv_idle = RV_HOLD;

void aica_set_reverb(int on) {
    static const int len[4] = { RV_C1, RV_C2, RV_C3, RV_C4 };
    int k;
    rv_on = on;
    for (k = 0; k < 4; k++) {       /* the combs' gain for a decay of 1.5 s, Q12 */
        rv_fb[k] = (int32_t)(4096.0 * pow(10.0, -3.0 * len[k] / (1.5 * 22050)) + 0.5);
    }
}

/* an allpass of gain 5/8 whose delay ends at `end` from w; gives its output */
static inline int32_t rv_allpass(int32_t* w, int end, int len, int32_t s) {
    int32_t b = w[end - len];
    w[end] = s + (b >> 1) + (b >> 3);
    return b - s;
}

/* a comb of gain fb (Q12), a low-pass in its loop */
static inline int32_t rv_comb(int32_t* w, int end, int len, int32_t s, int32_t* damp, int32_t fb) {
    int32_t y = w[end - len];
    *damp = y + ((*damp - y) >> 4);
    w[end] = s + (*damp * fb >> 12);
    return y;
}

/* one step: the send (16 bits) in, the reverb's two sides out */
static void reverb_step(int32_t in, int32_t* l, int32_t* r) {
    int32_t* w = &rv_ring[rv_pos];
    int32_t d, s, c1, c2, c3, c4, sl, sr;
    in = in > 32767 ? 32767 : in < -32768 ? -32768 : in;     /* every sum below stays in 32 bits */
    d = w[RV_E_PRE - RV_PRE];
    w[RV_E_PRE] = in;
    s = rv_allpass(w, RV_E_AP1, RV_AP1, d);
    s = rv_allpass(w, RV_E_AP2, RV_AP2, s);
    s = rv_allpass(w, RV_E_AP3, RV_AP3, s);
    s = rv_allpass(w, RV_E_AP4, RV_AP4, s) >> 5;
    c1 = rv_comb(w, RV_E_C1, RV_C1, s, &rv_damp[0], rv_fb[0]);
    c2 = rv_comb(w, RV_E_C2, RV_C2, s, &rv_damp[1], rv_fb[1]);
    c3 = rv_comb(w, RV_E_C3, RV_C3, s, &rv_damp[2], rv_fb[2]);
    c4 = rv_comb(w, RV_E_C4, RV_C4, s, &rv_damp[3], rv_fb[3]);
    sl = rv_allpass(w, RV_E_OUTL, RV_OUTL, (c1 - c2 + c3 - c4) >> 2);   /* the sides */
    sr = rv_allpass(w, RV_E_OUTR, RV_OUTR, (c1 + c2 - c3 - c4) >> 2);
    d = (d >> 1) + (d >> 3);                                /* the echo, 5/8 */
    *l = d + sl + (sl >> 2) + (sl >> 3);                    /* the tail, 11/8 */
    *r = d + sr + (sr >> 2) + (sr >> 3);
    if (++rv_pos + RV_E_OUTR >= RV_SIZE) {
        memmove(rv_ring, &rv_ring[rv_pos], (RV_E_OUTR + 1) * sizeof rv_ring[0]);
        rv_pos = 0;
    }
}

/* the chunk's sends in, the reverb added to mix_l and mix_r */
static void reverb(int len) {
    int i;
    for (i = 0; i < len; i++) {
        int32_t in = sends[i];
        if (!rv_odd) {
            rv_in[1] = in;
            mix_l[i] += (rv_l[0] + rv_l[1]) >> 1;
            mix_r[i] += (rv_r[0] + rv_r[1]) >> 1;
        } else {
            mix_l[i] += rv_l[1];
            mix_r[i] += rv_r[1];
            rv_l[0] = rv_l[1];
            rv_r[0] = rv_r[1];
            reverb_step((rv_in[0] + 2 * rv_in[1] + in) >> 2, &rv_l[1], &rv_r[1]);
            rv_in[0] = in;
        }
        rv_odd ^= 1;
    }
}

void aica_render(int16_t* out, int n) {
    while (n > 0) {
        int len = n < chunk ? n : chunk, i, c, mvol = reg16(0x2800) & 15;
        unsigned active = 0;
        PROF_START(a);
        PERF(AICA_PERF_CHUNK);
        aica_watch_gen++;           /* the channels move on in each chunk's mix */
        run_cpu(len);
        PERF(AICA_PERF_ARM_END);
        PROF_STOP(PZ_SND_ARM, a);
        PROF_START(t);
        memset(mix_l, 0, chunk * sizeof mix_l[0]);
        memset(mix_r, 0, chunk * sizeof mix_r[0]);
        if (sent && mixs_on) {
            memset(mixs, 0, chunk * sizeof mixs[0]);
        }
        mixs_on = dsp_on || !mix_fast;
        sends_on = rv_on && !dsp_on && dsp_last;
        if (sends_on) {
            memset(sends, 0, chunk * sizeof sends[0]);
        }
        sent = 0;
        for (c = 0; c < 64; c++) {
            if (ch_on[c]) {
                render_channel(c, len);
                active++;
            }
        }
        last_active = active;
        PERF(AICA_PERF_MIX_END);
        PROF_STOP(PZ_SND_CHANNELS, t);
        PROF_COUNT(PZ_SND_CHANNELS, active);
        if (dsp_dirty) {
            dsp_scan();
        }
        if (sends_on && (sent || rv_idle < RV_HOLD)) {
            reverb(len);
            rv_idle = sent ? 0 : rv_idle + len;
            if (rv_idle >= RV_HOLD) {
                memset(rv_ring, 0, sizeof rv_ring);
                memset(rv_damp, 0, sizeof rv_damp);
                rv_l[0] = rv_l[1] = rv_r[0] = rv_r[1] = 0;
            }
        }
#ifdef AICA_DSP_JIT
        dsp_fast = dsp_on && dsp_last && dsp_chunk();
#endif
        for (i = 0; i < len; i++) {
            int32_t l = mix_l[i], r = mix_r[i], g;
#ifdef AICA_DSP_JIT
            if (dsp_fast) {
                dsp_fast_sample(mixs[i], &l, &r);
            } else
#endif
            if (dsp_on && dsp_last) {
                int e;
                dsp_sample(mixs[i]);
                for (e = 0; e < 16; e++) {
                    uint32_t ef = reg16(0x2000 + e * 4);
                    int32_t lv = level_gain[(ef >> 8) & 15], sl, sr;
                    if (!lv || !dsp.efreg[e]) {
                        continue;
                    }
                    sl = sr = out_level(dsp.efreg[e]) * lv >> 15;
                    if (ef & 0x10) {
                        sr = sr * pan_gain[ef & 15] >> 15;
                    } else {
                        sl = sl * pan_gain[ef & 15] >> 15;
                    }
                    l += sl;
                    r += sr;
                }
            }
            g = level_gain[mvol];
            out[i * 2] = (int16_t)out_level((int32_t)((int64_t)l * g >> 15));
            out[i * 2 + 1] = (int16_t)out_level((int32_t)((int64_t)r * g >> 15));
        }
        out += len * 2;
        n -= len;
    }
}

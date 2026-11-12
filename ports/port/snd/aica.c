/*
 * aica.c  The AICA, the NAOMI's and the Dreamcast's sound chip: 64 channels, the DSP, the timers and the interrupts.
 * Written from its register map and the SCSP's documented behaviour; it runs 16 samples at a time.
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "aica.h"
#include "arm7.h"
#include "prof.h"

#ifdef MVC2_PROF
static int game_side;               /* aica_run_cpu: the game's thread runs the CPU, the sound's waiting */
#define IO_COUNT() do { if (!game_side) PROF_COUNT(PZ_SND_IO, 1); } while (0)
#else
#define IO_COUNT() ((void)0)
#endif

#define CHUNK 16
#define CPU_CYCLES_PER_SAMPLE 64    /* 2.8224 MHz: the ARM7's 45 MHz with the sound RAM's waits */
static int cpu_cycles = CPU_CYCLES_PER_SAMPLE;

static uint8_t* ram;
uint8_t* aica_ram;
uint32_t aica_ram_mask;

static uint8_t reg[0x8000];         /* as written; 0x800000-0x807FFF to the sound CPU */
static int cpu_reset = 1;
static int cpu_debt;                /* cycles the CPU ran past its share */
static int dsp_on = 1, filter_on = 1;

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
    int on;                 /* sounding: keyed on, or releasing */
    uint32_t pos;           /* the sample s0 is, from SA */
    uint32_t frac;          /* between s0 and s1, 16 bits */
    int32_t s0, s1;
    int32_t ad_prev, ad_step;       /* the ADPCM decoder after s1 */
    int32_t ad_loop_prev, ad_loop_step;
    int ad_loop_saved;
    int eg_state;
    int32_t eg;             /* attenuation << 16, 0 loudest */
    int feg_state;
    int32_t flv;            /* the filter's cutoff << 16 */
    int32_t y1, y2;
    uint32_t lfo;           /* phase << 16 */
    uint32_t noise;
    int lp;                 /* passed the loop end since read */
} CHANNEL;

static CHANNEL ch[64];

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

static int32_t fetch(int c, uint32_t i) {
    uint32_t sa = (CR(c, 0x00) & 0x7F) << 16 | CR(c, 0x04);
    int pcms = (CR(c, 0x00) >> 7) & 3;
    if (pcms == 0) {
        uint32_t a = (sa + i * 2) & aica_ram_mask & ~1u;
        return (int16_t)(ram[a] | ram[a + 1] << 8);
    }
    return (int8_t)ram[(sa + i) & aica_ram_mask] << 8;
}

static int32_t adpcm_decode(int c, uint32_t i) {
    CHANNEL* h = &ch[c];
    uint32_t sa = (CR(c, 0x00) & 0x7F) << 16 | CR(c, 0x04);
    int n = ram[(sa + (i >> 1)) & aica_ram_mask] >> ((i & 1) * 4) & 15;
    int32_t d = h->ad_step * ((n & 7) * 2 + 1) >> 3, s;
    d = d > 32767 ? 32767 : d;      /* a big step's largest deltas are held to 16 bits */
    s = n & 8 ? h->ad_prev - d : h->ad_prev + d;
    s = s > 32767 ? 32767 : s < -32768 ? -32768 : s;
    h->ad_prev = s;
    h->ad_step = h->ad_step * adpcm_scale[n & 7] >> 8;
    h->ad_step = h->ad_step < 127 ? 127 : h->ad_step > 24576 ? 24576 : h->ad_step;
    return s;
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

/* the sample at index i, when it comes after the one before (as the ADPCM decoder needs) */
static int32_t sample_at(int c, uint32_t i, int looped) {
    CHANNEL* h = &ch[c];
    int pcms = (CR(c, 0x00) >> 7) & 3;
    if (pcms < 2) {
        return fetch(c, i);
    }
    if (i == CR(c, 0x08)) {
        if (looped && pcms == 2 && h->ad_loop_saved) {
            h->ad_prev = h->ad_loop_prev;
            h->ad_step = h->ad_loop_step;
        } else if (!h->ad_loop_saved) {
            h->ad_loop_prev = h->ad_prev;
            h->ad_loop_step = h->ad_step;
            h->ad_loop_saved = 1;
        }
    }
    return adpcm_decode(c, i);
}

/* a channel that has ended: its KYONB goes too, so the next KYONEX does not start it again */
static void channel_stop(int c) {
    ch[c].on = 0;
    reg[c * 0x80 + 1] &= (uint8_t)~0x40;
}

static void key_on(int c) {
    CHANNEL* h = &ch[c];
    uint32_t nx;
    int looped;
    if (h->on && h->eg_state != EG_RELEASE) {
        return;
    }
    memset(h, 0, sizeof *h);
    h->on = 1;
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
    if (ch[c].on) {
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

/* the LFO's waves, 0 to 255: saw, square, triangle, noise */
static int lfo_wave(int ws, int ph, uint32_t noise) {
    switch (ws) {
    case 0: return ph;
    case 1: return ph < 128 ? 255 : 0;
    case 2: return ph < 128 ? ph * 2 : 511 - ph * 2;
    default: return (int)(noise >> 24);
    }
}

static void render_channel(int c, int n) {
    CHANNEL* h = &ch[c];
    uint32_t r18 = CR(c, 0x18), r1c = CR(c, 0x1C), r20 = CR(c, 0x20), r24 = CR(c, 0x24), r28 = CR(c, 0x28);
    int oct = oct_of(c);
    uint32_t base = 0x400 | (r18 & 0x3FF), step;
    int32_t gl, gr, gs, tl = (int32_t)((r28 >> 8) & 0xFF) << 2;
    int disdl = (r24 >> 8) & 15, dipan = r24 & 31, imxl = (r20 >> 4) & 15, isel = r20 & 15;
    int voff = r28 & 0x40, lpoff = (r28 & 0x20) || !filter_on, q = r28 & 31;
    int plfos = (r1c >> 5) & 7, alfos = r1c & 7, plfows = (r1c >> 8) & 3, alfows = (r1c >> 3) & 3;
    int32_t qf = q * 1024 / 32 * 7 / 8;     /* resonance, Q12 */
    int i;
    step = oct + 6 >= 0 ? base << (oct + 6) : base >> -(oct + 6);
    gl = gr = level_gain[disdl];
    if (dipan & 0x10) {
        gr = gr * pan_gain[dipan & 15] >> 15;
    } else {
        gl = gl * pan_gain[dipan & 15] >> 15;
    }
    gs = level_gain[imxl];
    sent |= gs != 0;
    if (r1c & 0x8000) {
        h->lfo = 0;
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
        if (gs) {
            mixs[i][isel] += (s * gs >> 15) << 4;
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
        if (!h->on) {
            break;
        }
    }
}

/* ---- the DSP ---- */

static int32_t temp[128], mems[32], efreg[16];
static int32_t frc_reg, y_reg, adrs_reg, memval;
static uint32_t mdec;
static int dsp_last;                /* the steps up to the last that does something */
static int dsp_dirty = 1;

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

/* a step of the program, decoded when the program changes */
typedef struct {
    uint8_t tra, twa, ira, iwa, ewa, masa, shift, ysel;
    uint16_t f;             /* the DF_ flags */
    int16_t coef;           /* its coefficient, 13 bits */
    uint16_t madrs;
} DSP_STEP;

enum {
    DF_TWT = 1, DF_XSEL = 2, DF_IWT = 4, DF_TABLE = 8, DF_MWT = 16, DF_MRD = 32, DF_EWT = 64, DF_ADRL = 128,
    DF_FRCL = 256, DF_YRL = 512, DF_NEGB = 1024, DF_ZERO = 2048, DF_BSEL = 4096, DF_NOFL = 8192, DF_ADREB = 16384,
    DF_NXADR = 32768
};

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
}

static void dsp_sample(const int32_t* mix) {
    uint32_t rbl = 8192u << ((reg16(0x2804) >> 13) & 3), rbp = reg16(0x2804) & 0xFFF;
    int32_t acc = 0, inputs = 0;
    int s;
    memset(efreg, 0, sizeof efreg);
    for (s = 0; s < dsp_last; s++) {
        const DSP_STEP* p = &prog[s];
        uint32_t f = p->f;
        int32_t b, x, y, shifted, t;
        if (p->ira <= 0x1F) {
            inputs = sx24(mems[p->ira]);
        } else if (p->ira <= 0x2F) {
            inputs = sx24(mix[p->ira - 0x20] << 4);
        } else {
            inputs = 0;
        }
        if (f & DF_IWT) {
            mems[p->iwa] = memval;
            if (p->ira == p->iwa) {
                inputs = memval;
            }
        }
        t = sx24(temp[(p->tra + mdec) & 0x7F]);
        b = f & DF_ZERO ? 0 : f & DF_BSEL ? acc : t;
        if ((f & (DF_NEGB | DF_ZERO)) == DF_NEGB) {
            b = -b;
        }
        x = f & DF_XSEL ? inputs : t;
        switch (p->ysel) {
        case 0: y = frc_reg; break;
        case 1: y = p->coef; break;
        case 2: y = (y_reg >> 11) & 0x1FFF; break;
        default: y = (y_reg >> 4) & 0x0FFF; break;
        }
        if (f & DF_YRL) {
            y_reg = inputs;
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
            temp[(p->twa + mdec) & 0x7F] = shifted;
        }
        if (f & DF_FRCL) {
            frc_reg = p->shift == 3 ? shifted & 0xFFF : (shifted >> 11) & 0x1FFF;
        }
        if (f & (DF_MRD | DF_MWT)) {
            uint32_t addr = p->madrs;
            if (!(f & DF_TABLE)) {
                addr += mdec;
            }
            if (f & DF_ADREB) {
                addr += adrs_reg & 0xFFF;
            }
            if (f & DF_NXADR) {
                addr++;
            }
            addr &= f & DF_TABLE ? 0xFFFF : rbl - 1;
            addr = ((addr + (rbp << 10)) * 2) & aica_ram_mask;
            if (f & DF_MRD) {
                uint16_t w = (uint16_t)(ram[addr] | ram[addr + 1] << 8);
                memval = f & DF_NOFL ? sx24((int32_t)(int16_t)w << 8) : dsp_unpack(w);
            }
            if (f & DF_MWT) {
                uint16_t w = f & DF_NOFL ? (uint16_t)(shifted >> 8) : dsp_pack(shifted);
                ram[addr] = (uint8_t)w;
                ram[addr + 1] = (uint8_t)(w >> 8);
            }
        }
        if (f & DF_ADRL) {
            adrs_reg = p->shift == 3 ? (shifted >> 12) & 0xFFF : inputs >> 16;
        }
        if (f & DF_EWT) {
            efreg[p->ewa] += shifted >> 8;
        }
    }
    mdec--;
}

/* ---- the timers and the interrupts ---- */

static uint32_t scipd, mcipd, intreq;
static int timer_count[3], timer_sub[3];
static int timer_load[3] = { -1, -1, -1 };  /* written by the CPU in its run: set once the timers reach that time */
static int in_cpu;

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

static int timers_tick(int n) {
    int t, raised = 0;
    for (t = 0; t < 3; t++) {
        int prescale = 1 << ((reg16(0x2890 + t * 4) >> 8) & 7);
        timer_sub[t] += n;
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
            int ran = arm7_run((done + seg) * cpu_cycles - cycles);
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
                arm7_run_fiq(HANDLER_MAX);
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

static void write_reg16(uint32_t off, uint32_t v, uint32_t mask) {
    uint32_t old;
    off &= 0x7FFE;
    old = reg16(off);
    v = (old & ~mask) | (v & mask);
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
    if (ram == NULL || aica_ram_mask + 1 != size) {
        aica_free();
        if ((ram = malloc(size)) == NULL) {
            return 0;
        }
        aica_ram = ram;
    }
    memset(ram, 0, size);
    memset(reg, 0, sizeof reg);
    memset(ch, 0, sizeof ch);
    memset(temp, 0, sizeof temp);
    memset(mems, 0, sizeof mems);
    frc_reg = y_reg = adrs_reg = memval = 0;
    mdec = 0;
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
    free(ram);
    ram = aica_ram = NULL;
    aica_ram_mask = 0;
}

int aica_cpu_running(void) {
    return !cpu_reset;
}

void aica_run_cpu(int cycles) {
    if (!cpu_reset) {
#ifdef MVC2_PROF
        game_side = 1;
#endif
        arm7_run(cycles);
#ifdef MVC2_PROF
        game_side = 0;
#endif
    }
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

static unsigned last_active;

void aica_stats(AICA_STATS* s) {
    s->cpu_cycles = arm7_cycles();
    s->active_channels = last_active;
    s->dsp_steps = (unsigned)dsp_last;
}

/* the channels sounding and the DSP, for MVC2_SNDDBG */
void aica_dump(void (*log)(const char* fmt, ...)) {
    int c;
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
        log("  ring at %X: %u words not 0, %u with exponent >= 11; efreg %d %d %d %d", rb, nz, z6, efreg[0], efreg[1],
            efreg[2], efreg[3]);
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
        if (!h->on) {
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

static int32_t out_level(int32_t s) {
    return s > 32767 ? 32767 : s < -32768 ? -32768 : s;
}

void aica_render(int16_t* out, int n) {
    while (n > 0) {
        int len = n < CHUNK ? n : CHUNK, i, c, mvol = reg16(0x2800) & 15;
        unsigned active = 0;
        PROF_START(a);
        run_cpu(len);
        PROF_STOP(PZ_SND_ARM, a);
        PROF_START(t);
        memset(mix_l, 0, sizeof mix_l);
        memset(mix_r, 0, sizeof mix_r);
        memset(mixs, 0, sizeof mixs);
        sends_on = rv_on && !dsp_on && dsp_last;
        if (sends_on) {
            memset(sends, 0, sizeof sends);
        }
        sent = 0;
        for (c = 0; c < 64; c++) {
            if (ch[c].on) {
                render_channel(c, len);
                active++;
            }
        }
        last_active = active;
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
        for (i = 0; i < len; i++) {
            int32_t l = mix_l[i], r = mix_r[i], g;
            if (dsp_on && dsp_last) {
                int e;
                dsp_sample(mixs[i]);
                for (e = 0; e < 16; e++) {
                    uint32_t ef = reg16(0x2000 + e * 4);
                    int32_t lv = level_gain[(ef >> 8) & 15], sl, sr;
                    if (!lv || !efreg[e]) {
                        continue;
                    }
                    sl = sr = efreg[e] * lv >> 15;
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
            out[i * 2] = (int16_t)out_level(l * g >> 15);
            out[i * 2 + 1] = (int16_t)out_level(r * g >> 15);
        }
        out += len * 2;
        n -= len;
    }
}

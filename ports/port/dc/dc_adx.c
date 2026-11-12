/*
 * dc_adx.c  CRI's ADX player, the Dreamcast's music from the disc: one player, decoded a frame at a time on the
 * game's thread and mixed after the AICA. The game only waits on it, so what it reads back changes no frame.
 */

#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "adx.h"
#include "naomi_host.h"
#include "shinobi.h"
#include "snd.h"

#define ADX_SECTORS 16
#define ADX_BUF (ADX_SECTORS * GDD_SCT_SIZE)
#define ADX_BLOCK 18            /* a channel's 32 samples: a 16-bit scale and 16 bytes of 4-bit deltas */

Sint32 gdFsSeek(GDFS gd, Sint32 offset, Sint32 origin);    /* dc_gd.c */

struct _adx_talk {
    Sint32 vol;                 /* 0.1 dB steps, 0 the file's own level */
    Sint32 stat;
    Sint32 pause;
    GDFS gd;
    uint32_t size;
    int ch;
    uint32_t rate;
    uint32_t total;             /* samples a channel */
    int loop;
    uint32_t loop_start, loop_end;
    uint32_t data;              /* the first block */
    int32_t coef1, coef2;
    int32_t hist[2][2];
    int16_t dec[2][32];
    uint32_t sample;            /* the next sample to decode */
    int dec_left;
    uint32_t step, frac;        /* the file's rate against the output's, 16.16 */
    int16_t out[2];
    uint8_t buf[ADX_BUF];
    uint32_t buf_off, buf_len;  /* the file's bytes in buf */
};

static struct _adx_talk player;

static uint32_t be16(const uint8_t* p) {
    return (uint32_t)p[0] << 8 | p[1];
}

static uint32_t be32(const uint8_t* p) {
    return be16(p) << 16 | be16(p + 2);
}

/* the file's bytes at off (n of them, n at most a sector), read in when they are not in the buffer */
static const uint8_t* bytes_at(ADXT p, uint32_t off, uint32_t n) {
    if (off < p->buf_off || off + n > p->buf_off + p->buf_len) {
        uint32_t sct = off / GDD_SCT_SIZE;
        gdFsSeek(p->gd, (Sint32)sct, 0);
        gdFsRead(p->gd, ADX_SECTORS, p->buf);
        p->buf_off = sct * GDD_SCT_SIZE;
        p->buf_len = ADX_BUF;
    }
    return p->buf + (off - p->buf_off);
}

static void stop(ADXT p) {
    if (p->gd != NULL) {
        gdFsClose(p->gd);
        p->gd = NULL;
    }
    p->stat = ADXT_STAT_STOP;
}

/* decodes the 32 samples from sample on (a multiple of 32) */
static void decode_block(ADXT p) {
    uint32_t block = p->sample / 32;
    const uint8_t* b = bytes_at(p, p->data + block * ADX_BLOCK * (uint32_t)p->ch, ADX_BLOCK * (uint32_t)p->ch);
    int c, i;
    for (c = 0; c < p->ch; c++, b += ADX_BLOCK) {
        int32_t scale = (int32_t)(be16(b) & 0x1FFF) + 1;
        int32_t h1 = p->hist[c][0], h2 = p->hist[c][1];
        for (i = 0; i < 32; i++) {
            int n = b[2 + i / 2] >> (i & 1 ? 0 : 4) & 15;
            int32_t s = (n >= 8 ? n - 16 : n) * scale + (p->coef1 * h1 >> 12) + (p->coef2 * h2 >> 12);
            s = s > 32767 ? 32767 : s < -32768 ? -32768 : s;
            p->dec[c][i] = (int16_t)s;
            h2 = h1;
            h1 = s;
        }
        p->hist[c][0] = h1;
        p->hist[c][1] = h2;
    }
    p->dec_left = 32 - (int)(p->sample & 31);
}

/* the next sample of each channel; 0 when the file has ended */
static int next_sample(ADXT p) {
    int i;
    if (p->loop && p->sample >= p->loop_end) {
        p->sample = p->loop_start;
        p->dec_left = 0;
    }
    if (p->sample >= p->total) {
        stop(p);
        p->stat = ADXT_STAT_PLAYEND;
        return 0;
    }
    if (p->dec_left == 0) {
        uint32_t want = p->sample;
        p->sample &= ~31u;
        decode_block(p);
        p->sample = want;
    }
    i = (int)(p->sample & 31);
    p->out[0] = p->dec[0][i];
    p->out[1] = p->dec[p->ch - 1][i];
    p->sample++;
    p->dec_left--;
    return 1;
}

/* the frame's music, into buf (silence) */
static void adx_mix(int16_t* buf, int n) {
    ADXT p = &player;
    int32_t g;
    int i, c;
    if (p->stat != ADXT_STAT_PLAYING || p->pause) {
        return;
    }
    g = (int32_t)(32768.0 * pow(10.0, p->vol / 200.0));
    for (i = 0; i < n; i++) {
        p->frac += p->step;
        while (p->frac >= 0x10000) {
            p->frac -= 0x10000;
            if (!next_sample(p)) {
                return;
            }
        }
        for (c = 0; c < 2; c++) {
            int32_t s = p->out[c] * g >> 15;
            buf[i * 2 + c] = (int16_t)(s > 32767 ? 32767 : s < -32768 ? -32768 : s);
        }
    }
}

/* reads the header: the format, the predictor's coefficients from the cutoff, the loop */
static int open_header(ADXT p) {
    const uint8_t* h = bytes_at(p, 0, GDD_SCT_SIZE);
    double z, a, b, c;
    if (be16(h) != 0x8000 || h[4] != 3 || h[5] != ADX_BLOCK || h[6] != 4 || h[7] < 1 || h[7] > 2) {
        return 0;
    }
    p->data = be16(h + 2) + 4;
    p->ch = h[7];
    p->rate = be32(h + 8);
    p->total = be32(h + 12);
    z = cos(2.0 * M_PI * be16(h + 16) / p->rate);
    a = M_SQRT2 - z;
    b = M_SQRT2 - 1.0;
    c = (a - sqrt((a + b) * (a - b))) / b;
    p->coef1 = (int32_t)floor(c * 8192.0);
    p->coef2 = (int32_t)floor(c * c * -4096.0);
    p->loop = h[0x12] == 3 && be32(h + 0x18) != 0;
    p->loop_start = be32(h + 0x1C);
    p->loop_end = be32(h + 0x24);
    if (p->loop && (p->loop_end > p->total || p->loop_start >= p->loop_end)) {
        p->loop = 0;
    }
    p->step = (uint32_t)(((uint64_t)p->rate << 16) / SND_RATE);
    return 1;
}

void ADXT_Init(void) {
    snd_set_mixer(adx_mix);
}

void ADXT_Finish(void) {
    stop(&player);
}

ADXT ADXT_Create(Sint32 maxnch, void* work, Sint32 worksize) {
    (void)maxnch;
    (void)work;
    (void)worksize;
    return &player;
}

void ADXT_SetAutoRcvr(ADXT adxt, Sint32 rmode) {
    (void)adxt;
    (void)rmode;
}

void ADXT_ExecServer(void) {
}

Sint32 ADXT_GetStat(ADXT adxt) {
    return adxt != NULL ? adxt->stat : ADXT_STAT_STOP;
}

Sint32 ADXT_IsIbufSafety(ADXT adxt) {
    (void)adxt;
    return 1;
}

void ADXT_Pause(ADXT adxt, Sint32 sw) {
    if (adxt != NULL) {
        adxt->pause = sw;
    }
}

void ADXT_StartFname(ADXT adxt, char* fname) {
    if (adxt == NULL) {
        return;
    }
    stop(adxt);
    if (!snd_enabled() || (adxt->gd = gdFsOpen(fname, NULL)) == NULL) {
        return;
    }
    adxt->buf_off = adxt->buf_len = 0;
    if (!open_header(adxt)) {
        host_log("adx: %s is not a 4-bit ADX file", fname);
        stop(adxt);
        return;
    }
    memset(adxt->hist, 0, sizeof adxt->hist);
    adxt->sample = 0;
    adxt->dec_left = 0;
    adxt->frac = 0;
    adxt->out[0] = adxt->out[1] = 0;
    adxt->pause = 0;
    adxt->stat = ADXT_STAT_PLAYING;
    if (getenv("MVC2_SNDLOG")) {
        host_log("adx: %s, %u Hz, %d channels, %u samples, loop %d %u-%u (frame %u)", fname, adxt->rate, adxt->ch,
                 adxt->total, adxt->loop, adxt->loop_start, adxt->loop_end, host_frame());
    }
}

void ADXT_Stop(ADXT adxt) {
    if (adxt != NULL) {
        if (adxt->stat == ADXT_STAT_PLAYING && getenv("MVC2_SNDLOG")) {
            host_log("adx: stopped at sample %u (frame %u)", adxt->sample, host_frame());
        }
        stop(adxt);
    }
}

void ADXT_SetOutVol(ADXT adxt, Sint32 vol) {
    if (adxt != NULL) {
        adxt->vol = vol;
    }
}

Sint32 ADXT_GetOutVol(ADXT adxt) {
    return adxt != NULL ? adxt->vol : 0;
}

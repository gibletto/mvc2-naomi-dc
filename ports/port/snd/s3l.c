/*
 * s3l.c  The port's s3lib: Sega's NAOMI stream player, which feeds libsnd's streams from the ROM.
 * The board's ROM DMA into the sound RAM becomes a copy that is done at once, so its waits are gone.
 */

#include <stdint.h>
#include <string.h>

#include "snd.h"

/* the head of an "SPSD" stream, as libsnd's nlsnd.h has it */
typedef struct SNDSTREAMHD {
    char magic[4];      /* "SPSD" */
    uint8_t unk_04[3];
    uint8_t unk_07;
    uint8_t format;     /* 08: 0 is 16 bits, 1 is 8 bits and 2 or 3 is 4 bits */
    char chs;           /* 09: 0 is 1 channel, 1 is 2 and 2 is 4 */
    uint8_t unk_0a[2];
    uint32_t size;      /* 0C: the bytes of data after the head, all channels */
    uint8_t unk_10[6];
    uint16_t unk_16;
    uint8_t unk_18[6];
    uint8_t unk_1e;
    uint8_t unk_1f;
    uint16_t unk_20;
    uint16_t octfns;    /* 22 */
    uint8_t unk_24[6];
    uint16_t freq;      /* 2A */
    uint8_t unk_2c[2];
    uint16_t unk_2e;    /* 2E: bytes at the end of each channel's data that aren't played */
    struct {
        uint8_t gain;
        uint8_t vol;
        uint8_t pan;
        uint8_t fx;
    } ch[4];            /* 30 */
} SNDSTREAMHD;

/* libsnd (nlsnd.c) */
extern int32_t nlsndSeqRequest(uint32_t request);
extern int32_t nlsndStreamInit(int16_t mono, int16_t stereo, int16_t mode, int16_t count);
extern void nlsndStreamSetHdInfo(int16_t stream, const SNDSTREAMHD* hd);
extern uint32_t nlsndStreamGetBufInfo(int16_t stream, int16_t side, int32_t* size);
extern uint32_t nlsndStreamGetPlayPos(int16_t stream);
extern int16_t nlsndStreamOpen(int16_t type);
extern void nlsndStreamStart(int16_t stream);
extern void nlsndStreamStop(int16_t stream);
extern void nlsndStreamGenerateHdInfo(SNDSTREAMHD* hd);
extern void nlsndStreamSetHdOctFns(SNDSTREAMHD* hd, int16_t oct, uint16_t fns);
extern void nlsndStreamSetHdChs(SNDSTREAMHD* hd, int16_t chs);
extern void nlsndStreamSetHdVol(uint8_t* hd, int16_t ch, int32_t vol);
extern void nlsndStreamSetHdPan(uint8_t* hd, int16_t ch, int32_t pan);

/* the host: the sound RAM at an SH4 address (snd.h), and the cartridge's ROM */
extern int naomi_rom_read(uint32_t offset, void* dst, uint32_t size);

#define S3L_PORTS   8
#define S3L_HEAD    64          /* the head before a stream's data */
#define S3L_BUF     0x4000      /* a channel's buffer in the sound RAM, as libsnd must give it */
#define S3L_HALF    0x2000      /* half of it: what is read at a time, and a stereo file's block */

/* a port's states: stopped, filling before the start (0-3), and playing (0x10-0x13), a half refilled at a time */
#define S3L_STOP    (-1)
#define S3L_PLAY    0x10

/* a port: one stream of one or two channels, and where it is in the ROM */
typedef struct S3LPORT {
    SNDSTREAMHD hd;
    int16_t state;
    int16_t chs;            /* 1 mono, 2 stereo */
    int16_t stream;         /* its libsnd stream */
    int16_t word_samples;   /* samples in 32 bits */
    uint32_t buf[2][2];     /* each channel's buffer halves, SH4 addresses in the sound RAM */
    uint32_t wrap;          /* the samples in a whole buffer, where the play position wraps */
    uint32_t half_shift;    /* the play position's bit that says which half is playing */
    uint32_t end;           /* the samples to play */
    uint32_t base;          /* the samples played before the buffer's last wrap */
    int32_t pos;            /* the play position in the buffer */
    int32_t last_pos;       /* and as it was a frame before */
    uint32_t start;         /* the ROM address of the data */
    uint32_t read;          /* the ROM address read next */
    int32_t loop;           /* played again from the start at its end */
    uint32_t size;
    uint32_t left;          /* a channel's bytes still to read (stereo) */
} S3LPORT;

/* the bits in a sample, and the shift to a half of the buffer, for each format */
static const uint32_t s3l_bits[4] = { 16, 8, 4, 4 };
static const uint32_t s3l_shift[4] = { 12, 13, 14, 14 };

static int32_t s3l_stereo[4];       /* the ports opened stereo, and mono */
static int32_t s3l_mono[8];
static S3LPORT s3l_port[S3L_PORTS];

/* a channel's fx byte: its send and its level */
static uint8_t _FX(int send, int level)
{
    return (uint8_t)(((send & 15) << 4) | (level & 15));
}

/* each port's next half buffer, read ahead on the game's thread, as s3lIntr may run on the sound's */
static uint8_t s3l_next[S3L_PORTS][S3L_HALF];
static uint32_t s3l_next_rom[S3L_PORTS];
static uint8_t s3l_next_ok[S3L_PORTS];

/* a half buffer of the ROM from rom (the board's DMA reads from 32 bytes) */
static void s3l_fetch(uint32_t rom, uint8_t* dst)
{
    if (!naomi_rom_read(rom & ~31u, dst, S3L_HALF)) {
        memset(dst, 0, S3L_HALF);
    }
}

void s3l_stage(void)
{
    int i;

    for (i = 0; i < S3L_PORTS; i++) {
        if (s3l_port[i].state >= 0 && (!s3l_next_ok[i] || s3l_next_rom[i] != s3l_port[i].read)) {
            s3l_fetch(s3l_port[i].read, s3l_next[i]);
            s3l_next_rom[i] = s3l_port[i].read;
            s3l_next_ok[i] = 1;
        }
    }
}

/* Reads a port's next half buffer from the ROM into the sound RAM at dst. */
static void s3l_read(const S3LPORT* port, uint32_t dst)
{
    int i = (int)(port - s3l_port);
    uint8_t* p = snd_sh4_ptr(dst);

    if (p == NULL) {
        return;
    }
    if (s3l_next_ok[i] && s3l_next_rom[i] == port->read) {
        memcpy(p, s3l_next[i], S3L_HALF);
    } else {
        s3l_fetch(port->read, p);
    }
}

/* Reads a stream's head from the ROM. */
void s3lGetHeader(uint32_t adr, SNDSTREAMHD* hd)
{
    snd_sync();
    if (!naomi_rom_read(adr, hd, S3L_HEAD)) {
        memset(hd, 0, S3L_HEAD);
    }
}

/* Opens the ports, mono ones first: each gets a stream and a default head. Gives back
   (stereo ports << 8) | mono ports, or -1 when libsnd's buffers aren't S3L_BUF. */
int s3lInit(int mono, int stereo, int quad)
{
    S3LPORT* p;
    int32_t size0;
    int32_t size1;
    int n_mono;
    int n_stereo;
    int i;

    snd_sync();
    (void)quad;
    nlsndStreamInit(mono, stereo, 0, 0);
    for (p = s3l_port; p < s3l_port + S3L_PORTS; p++) {
        p->state = S3L_STOP;
    }
    for (i = 0; i < 4; i++) {
        s3l_stereo[i] = -1;
    }
    for (i = 0; i < 8; i++) {
        s3l_mono[i] = -1;
    }

    i = 0;
    n_mono = 0;
    for (; n_mono < mono; n_mono++, i++) {
        p = &s3l_port[i];
        p->chs = 1;
        p->stream = nlsndStreamOpen(p->chs);
        nlsndStreamGenerateHdInfo(&p->hd);
        nlsndStreamSetHdChs(&p->hd, p->chs);
        nlsndStreamSetHdOctFns(&p->hd, 0, 0);
        nlsndStreamSetHdVol((uint8_t*)&p->hd, 0, 127);
        nlsndStreamSetHdPan((uint8_t*)&p->hd, 0, 0);
        p->buf[0][0] = nlsndStreamGetBufInfo(p->stream, 0, &size0);
        if (size0 != S3L_BUF) {
            return -1;
        }
        p->buf[0][1] = p->buf[0][0] + S3L_HALF;
        s3l_mono[n_mono] = i;
    }

    n_stereo = 0;
    for (; n_stereo < stereo; n_stereo++, i++) {
        p = &s3l_port[i];
        p->chs = 2;
        p->stream = nlsndStreamOpen(p->chs);
        nlsndStreamGenerateHdInfo(&p->hd);
        nlsndStreamSetHdChs(&p->hd, p->chs);
        nlsndStreamSetHdOctFns(&p->hd, 0, 0);
        nlsndStreamSetHdVol((uint8_t*)&p->hd, 0, 127);
        nlsndStreamSetHdVol((uint8_t*)&p->hd, 1, 127);
        nlsndStreamSetHdPan((uint8_t*)&p->hd, 0, 31);
        nlsndStreamSetHdPan((uint8_t*)&p->hd, 1, 15);
        p->buf[0][0] = nlsndStreamGetBufInfo(p->stream, 0, &size0);
        p->buf[1][0] = nlsndStreamGetBufInfo(p->stream, 1, &size1);
        if (size0 != S3L_BUF || size1 != S3L_BUF) {
            return -1;
        }
        p->buf[0][1] = p->buf[0][0] + S3L_HALF;
        p->buf[1][1] = p->buf[1][0] + S3L_HALF;
        s3l_stereo[n_stereo] = i;
    }
    return (n_stereo << 8) | n_mono;
}

/* Sets a stereo port to the stream at adr in the ROM, stopping what it played. Its data is
   interleaved, a half buffer of each channel at a time. */
int s3lSetStereoPort(int port, uint32_t adr, int loop)
{
    S3LPORT* p;
    uint32_t bits;

    snd_sync();
    if (port == -1) {
        return -1;
    }
    p = &s3l_port[port];
    s3lGetHeader(adr, &p->hd);
    adr += S3L_HEAD;
    if (p->state >= 0) {
        nlsndStreamStop(p->stream);
    }
    nlsndStreamSetHdChs(&p->hd, 2);
    nlsndStreamSetHdInfo(p->stream, &p->hd);
    bits = s3l_bits[p->hd.format];
    p->wrap = S3L_BUF * 8 / bits;
    p->half_shift = s3l_shift[p->hd.format];
    p->word_samples = (int16_t)(32 / bits);
    p->size = p->hd.size;
    p->left = p->hd.size >> 1;
    p->start = adr;
    p->read = adr;
    p->loop = loop;
    p->end = (p->hd.size - p->hd.unk_2e * 2) * 8 / bits >> 1;
    p->state = S3L_STOP;
    return 0;
}

/* Sets a mono port to the stream at adr in the ROM, stopping what it played. */
int s3lSetMonoPort(int port, uint32_t adr, int loop)
{
    S3LPORT* p;
    uint32_t bits;

    snd_sync();
    if (port == -1) {
        return -1;
    }
    p = &s3l_port[port];
    s3lGetHeader(adr, &p->hd);
    adr += S3L_HEAD;
    if (p->state >= 0) {
        nlsndStreamStop(p->stream);
    }
    nlsndStreamSetHdChs(&p->hd, 1);
    nlsndStreamSetHdInfo(p->stream, &p->hd);
    bits = s3l_bits[p->hd.format];
    p->wrap = S3L_BUF * 8 / bits;
    p->half_shift = s3l_shift[p->hd.format];
    p->word_samples = (int16_t)(32 / bits);
    p->size = p->hd.size;
    p->left = p->hd.size;
    p->start = adr;
    p->read = adr;
    p->loop = loop;
    p->end = (p->hd.size - p->hd.unk_2e) * 8 / bits;
    p->state = S3L_STOP;
    return 0;
}

/* Sends a playing stream a request to the driver. req is written command << 16 | value, with
   a channel's 0x800; the driver wants the command in the low byte and the value from bit 16. */
int32_t s3lReq(int stream, uint32_t req)
{
    req |= (uint32_t)stream << 12;
    return nlsndSeqRequest(((req & 0xff0000) >> 16) | ((req & 0xff) << 16) | (req & 0xff00));
}

/* Sets the volume, 0-127: in the head while the port is stopped, by request while it plays. */
static void s3l_set_volume(int port, int vol)
{
    S3LPORT* p = &s3l_port[port];

    if (vol < 0) {
        vol = 0;
    } else if (vol > 127) {
        vol = 127;
    }
    if (p->state < 0) {
        p->hd.ch[0].vol = (uint8_t)vol;
        if (p->chs > 1) {
            p->hd.ch[1].vol = (uint8_t)vol;
        }
        nlsndStreamSetHdInfo(p->stream, &p->hd);
    } else {
        s3lReq(p->stream, 0xa20000 | vol);
        if (p->chs > 1) {
            s3lReq(p->stream, 0xa20800 | vol);
        }
    }
}

int s3lSetVolume(int port, int vol)
{
    if (port == -1) {
        return -1;
    }
    if (!snd_defer(s3l_set_volume, port, vol)) {
        snd_sync();
        s3l_set_volume(port, vol);
    }
    return 0;
}

/* Sets the volume bias, -64 to 63, of a playing port. */
int s3lSetVolumeBias(int port, int bias)
{
    S3LPORT* p;

    snd_sync();
    if (port == -1) {
        return -1;
    }
    p = &s3l_port[port];
    if (p->state >= 0) {
        if (bias < -64) {
            bias = -64;
        } else if (bias > 63) {
            bias = 63;
        }
        s3lReq(p->stream, 0xa10c00 | (bias + 64));
    }
    return 0;
}

/* Sets the pan, -60 to 60 in steps of 4. A mono channel gets 1-15 for one side, 17-31 for the
   other and 0 for the middle; a stereo port keeps one channel at its edge (15 or 31) and moves
   the other in from its own. */
int s3lSetPanpot(int port, int pan)
{
    S3LPORT* p;
    int pan0;
    int pan1;

    snd_sync();
    if (port == -1) {
        return -1;
    }
    p = &s3l_port[port];
    pan /= 4;
    if (pan < -15) {
        pan = -15;
    } else if (pan > 15) {
        pan = 15;
    }
    if (p->chs < 2) {
        if (pan < 0) {
            pan0 = -pan;
        } else if (pan > 0) {
            pan0 = pan + 16;
        } else {
            pan0 = 0;
        }
        if (p->state < 0) {
            p->hd.ch[0].pan = (uint8_t)pan0;
            nlsndStreamSetHdInfo(p->stream, &p->hd);
        } else {
            s3lReq(p->stream, 0xa20100 | pan0);
        }
    } else {
        if (pan < 0) {
            pan0 = -pan;
            pan1 = 31;
        } else if (pan > 0) {
            pan0 = 15;
            pan1 = pan + 16;
        } else {
            pan0 = 15;
            pan1 = 31;
        }
        if (p->state < 0) {
            p->hd.ch[0].pan = (uint8_t)pan0;
            p->hd.ch[1].pan = (uint8_t)pan1;
            nlsndStreamSetHdInfo(p->stream, &p->hd);
        } else {
            s3lReq(p->stream, 0xa20100 | pan0);
            s3lReq(p->stream, 0xa20900 | pan1);
        }
    }
    return 0;
}

/* Transposes a playing port, by -64 to 63. */
int s3lSetTranspose(int port, int transp)
{
    S3LPORT* p;
    int v;

    snd_sync();
    if (port == -1) {
        return -1;
    }
    p = &s3l_port[port];
    if (p->state >= 0) {
        v = transp + 64;
        if (v < 0) {
            v = 0;
        } else if (v > 127) {
            v = 127;
        }
        s3lReq(p->stream, 0xa10800 | v);
    }
    return 0;
}

/* Detunes a playing port, by -64 to 63. */
int s3lSetDetune(int port, int detune)
{
    S3LPORT* p;
    int v;

    snd_sync();
    if (port == -1) {
        return -1;
    }
    p = &s3l_port[port];
    if (p->state >= 0) {
        v = detune + 64;
        if (v < 0) {
            v = 0;
        } else if (v > 127) {
            v = 127;
        }
        s3lReq(p->stream, 0xa10900 | v);
    }
    return 0;
}

/* Sets the fx send and level, 0-15, of both channels (the head's second even for mono). */
int s3lSetFx(int port, int fxch, int fxlv)
{
    S3LPORT* p;

    snd_sync();
    if (port == -1) {
        return -1;
    }
    p = &s3l_port[port];
    if (fxch < 0) {
        fxch = 0;
    } else if (fxch > 15) {
        fxch = 15;
    }
    if (fxlv < 0) {
        fxlv = 0;
    } else if (fxlv > 15) {
        fxlv = 15;
    }
    if (p->state < 0) {
        p->hd.ch[0].fx = _FX(fxch, fxlv);
        p->hd.ch[1].fx = _FX(fxch, fxlv);
        nlsndStreamSetHdInfo(p->stream, &p->hd);
    } else {
        s3lReq(p->stream, 0xa20300 | fxch);
        s3lReq(p->stream, 0xa20200 | fxlv);
        if (p->chs > 1) {
            s3lReq(p->stream, 0xa20b00 | fxch);
            s3lReq(p->stream, 0xa20a00 | fxlv);
        }
    }
    return 0;
}

/* Sets the fx send and level, 0-15, of one channel. */
int s3lSetFxCh(int port, int ch, int fxch, int fxlv)
{
    S3LPORT* p;
    uint32_t chreq;

    snd_sync();
    if (port == -1) {
        return -1;
    }
    p = &s3l_port[port];
    if (fxch < 0) {
        fxch = 0;
    } else if (fxch > 15) {
        fxch = 15;
    }
    if (fxlv < 0) {
        fxlv = 0;
    } else if (fxlv > 15) {
        fxlv = 15;
    }
    if (p->state < 0) {
        p->hd.ch[ch].fx = _FX(fxch, fxlv);
        nlsndStreamSetHdInfo(p->stream, &p->hd);
    } else {
        chreq = (uint32_t)ch << 11;
        s3lReq(p->stream, (0xa20300 + chreq) | fxch);
        s3lReq(p->stream, (0xa20200 + chreq) | fxlv);
    }
    return 0;
}

/* Sets the filter of a playing port: its cutoff, 0-127, and q, 0-31. */
int s3lSetFilter(int port, int cutf, int q)
{
    S3LPORT* p;

    snd_sync();
    if (port == -1) {
        return -1;
    }
    p = &s3l_port[port];
    if (cutf < 0) {
        cutf = 0;
    } else if (cutf > 127) {
        cutf = 127;
    }
    if (q < 0) {
        q = 0;
    } else if (q > 31) {
        q = 31;
    }
    if (p->state >= 0) {
        s3lReq(p->stream, 0xa10a00 | cutf);
        s3lReq(p->stream, 0xa10b00 | q);
    }
    return 0;
}

/* Starts a port from the start of its data; s3lIntr fills the buffers and starts the stream. */
int s3lStartPort(int port)
{
    S3LPORT* p;

    snd_sync();
    if (port == -1) {
        return -1;
    }
    p = &s3l_port[port];
    p->state = 0;
    p->read = p->start;
    p->base = 0;
    p->pos = 0;
    return 0;
}

int s3lStopPort(int port)
{
    S3LPORT* p;

    snd_sync();
    if (port == -1) {
        return -1;
    }
    p = &s3l_port[port];
    p->state = S3L_STOP;
    nlsndStreamStop(p->stream);
    return 0;
}

/* The bytes to step over in the ROM for a stereo half: a whole one, or what is left of the
   channel's data at its end. */
static uint32_t s3l_step(const S3LPORT* p)
{
    return p->left >= S3L_HALF ? S3L_HALF : p->left;
}

/* once a frame: each playing port followed, stopped or looped at its end, and the half just played refilled;
   with the sound on another thread, run there after the frame before (snd_defer) */
static void s3l_intr(int a, int b)
{
    S3LPORT* p;
    uint32_t step;

    (void)a;
    (void)b;
    for (p = s3l_port; p < s3l_port + S3L_PORTS; p++) {
        if (p->state < 0) {
            continue;
        }
        if ((p->state & 0xf0) == S3L_PLAY) {
            p->pos = (int32_t)nlsndStreamGetPlayPos(p->stream);
            if (p->last_pos > p->pos) {
                p->base += p->wrap;
            }
            p->last_pos = p->pos;
            if (p->base + p->pos >= p->end) {
                nlsndStreamStop(p->stream);
                p->read = p->start;
                p->base = 0;
                p->left = p->size / p->chs;
                p->state = p->loop ? 0 : S3L_STOP;
                continue;
            }
        }

        if (p->chs == 2) {
            switch (p->state) {
            case 0:     /* the first halves, left then right, then start */
                s3l_read(p, p->buf[0][0]);
                p->read += S3L_HALF;
                p->state = 1;
                break;
            case 1:
                s3l_read(p, p->buf[1][0]);
                p->read += S3L_HALF;
                p->left -= S3L_HALF;
                p->state = 2;
                nlsndStreamStart(p->stream);
                break;
            case 2:     /* the second halves */
                s3l_read(p, p->buf[0][1]);
                p->read += S3L_HALF;
                p->state = 3;
                break;
            case 3:
                s3l_read(p, p->buf[1][1]);
                p->read += S3L_HALF;
                p->left -= S3L_HALF;
                p->state = S3L_PLAY;
                p->last_pos = 0;
                break;
            case S3L_PLAY:  /* the second half playing: refill the first */
                if ((p->pos >> p->half_shift & 1) == 1) {
                    s3l_read(p, p->buf[0][0]);
                    p->read += s3l_step(p);
                    p->state = S3L_PLAY + 1;
                }
                break;
            case S3L_PLAY + 1:
                s3l_read(p, p->buf[1][0]);
                step = s3l_step(p);
                p->read += step;
                p->left -= step;
                p->state = S3L_PLAY + 2;
                break;
            case S3L_PLAY + 2:  /* the first half playing: refill the second */
                if ((p->pos >> p->half_shift & 1) == 0) {
                    s3l_read(p, p->buf[0][1]);
                    p->read += s3l_step(p);
                    p->state = S3L_PLAY + 3;
                }
                break;
            case S3L_PLAY + 3:
                s3l_read(p, p->buf[1][1]);
                step = s3l_step(p);
                p->read += step;
                p->left -= step;
                p->state = S3L_PLAY;
                p->last_pos = 0;
                break;
            }
        } else {
            switch (p->state) {
            case 0:     /* the first half, then start, then the second */
                s3l_read(p, p->buf[0][0]);
                p->read += S3L_HALF;
                p->state = 1;
                p->last_pos = 0;
                nlsndStreamStart(p->stream);
                break;
            case 1:
                s3l_read(p, p->buf[0][1]);
                p->read += S3L_HALF;
                p->state = S3L_PLAY;
                break;
            case S3L_PLAY:
                if ((p->pos >> p->half_shift & 1) == 1) {
                    s3l_read(p, p->buf[0][0]);
                    p->read += S3L_HALF;
                    p->state = S3L_PLAY + 1;
                }
                break;
            case S3L_PLAY + 1:
                if ((p->pos >> p->half_shift & 1) == 0) {
                    s3l_read(p, p->buf[0][1]);
                    p->read += S3L_HALF;
                    p->state = S3L_PLAY;
                }
                break;
            }
        }
    }
}

void s3lIntr(void)
{
    if (!snd_defer(s3l_intr, 0, 0)) {
        snd_sync();
        s3l_intr(0, 0);
    }
}

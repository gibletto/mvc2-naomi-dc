/*
 * snd_nl.c  The NAOMI library's sound calls (libsnd, nlsnd*) over the emulated AICA. Where the library waits on the
 * driver, the sound CPU runs until it answers, with no time passing; the game gets the same back with sound off.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "aica.h"
#include "arm7.h"
#include "naomi_host.h"
#include "prof.h"
#include "snd.h"

#define NLSND_RAM 0xA0800000u
#define NLSND_G2_BASE 0x5F800000u
#define SND_PORTS 8
#define SND_STREAMS 8

/* the mailbox words, at the bottom of the sound RAM */
#define MB_RAM_MODE 0x50
#define MB_READY 0x5C
#define MB_PORT_ID 0x60
#define MB_MEM_SIZE 0x88
#define MB_STREAM_MEM 0x8C
#define MB_MOVE_FROM 0x90
#define MB_MOVE_TO 0x94
#define MB_MOVE_SIZE 0x98
#define MB_STREAM_MONO 0xA0
#define MB_STREAM_STEREO 0xA4
#define MB_STREAM_UNIT 0xA8
#define MB_LOAD_GO 0xB0
#define MB_LOAD_ACK 0xB4
#define MB_STREAM_BOX 0x100     /* one a stream, 0x60 bytes: its channels' registers, buffers, play position, head */
#define MB_BOX_SIZE 0x60
#define MB_RING 0x400
#define MB_RING_END 0x500

#define SND_REQ_VALID 0x80
#define SND_REQ_STATUS 0x2000a0
#define SND_REQ_PACK 0x18a0
#define SND_ERR_TIMEOUT -256
#define SND_DRIVER_MIN 0x6e0

/* the sound CPU's time a wait may take: a block move of all the RAM is a few seconds of it */
#define WAIT_CYCLES (64 * AICA_RATE * 8)

typedef struct {
    int32_t id;         /* -1 when free */
    uint32_t start;     /* SH4 address */
    int32_t size;
} SLOT;

typedef struct {
    int8_t state;       /* -1 none, 0 free, 1 open */
    int8_t playing;
    int16_t bits;
    uint32_t buf[2];    /* SH4 addresses */
    uint32_t freq;
    uint32_t box;       /* its mailbox block, from the bottom of the RAM */
} STREAM;

static int sound_on = -1;
static int inited;
static uint32_t ring;           /* the next ring word, from the bottom of the RAM */
static uint32_t base;           /* where the packs start (SH4 address) */
static uint32_t ram_size;
static uint32_t mem_size;       /* the room for packs */
static uint32_t stream_mem;
static uint32_t stream_start;
static int32_t used;
static int32_t stream_size;
static SLOT slot[SND_PORTS];
static STREAM stream[SND_STREAMS];

const char nlsnd_version[] = "libsnd0.93-19990920";

static int snd_log;      /* MVC2_SNDLOG: the requests, in the log */

int snd_enabled(void) {
    if (sound_on < 0) {
        const char* e = getenv("MVC2_SOUND");
        /* on unless MVC2_SOUND=0; a test run (MVC2_FRAMES) makes none unless it is saved (MVC2_WAV) */
        sound_on = e != NULL ? atoi(e) != 0 : getenv("MVC2_FRAMES") == NULL || getenv("MVC2_WAV") != NULL;
        snd_log = getenv("MVC2_SNDLOG") != NULL;
    }
    return sound_on;
}

uint8_t* snd_sh4_ptr(uint32_t sh4_addr) {
    snd_sync();
    return aica_sh4_ptr(sh4_addr);
}

static uint32_t mb(uint32_t off) {
    uint32_t v;
    memcpy(&v, aica_ram + off, 4);
    return v;
}

static void mb_set(uint32_t off, uint32_t v) {
    memcpy(aica_ram + off, &v, 4);
}

static uint32_t word_at(uint32_t off) {
    return mb(off & aica_ram_mask & ~3u);
}

/* runs the sound CPU until the mailbox word at off is (or, with want_zero, is not) zero */
static int wait_for(uint32_t off, int want_zero) {
    int n;
    if (!snd_enabled() || !aica_cpu_running()) {
        return 0;
    }
    for (n = 0; n < WAIT_CYCLES; n += 2048) {
        if ((mb(off) == 0) == want_zero) {
            return 0;
        }
        aica_run_cpu(2048);
    }
    host_log("sound: the driver did not answer at %X", off);
    return SND_ERR_TIMEOUT;
}

/* ---- requests ---- */

/* the request into the driver's ring, or -1 when the ring is full */
static int seq_put(uint32_t request) {
    if (mb(ring) != 0) {
        return -1;
    }
    if (snd_log) {
        host_log("sound: request %08X (frame %u)", request, host_frame());
    }
    mb_set(ring, request);
    ring += 4;
    if (ring == MB_RING_END) {
        ring = MB_RING;
    }
    return 0;
}

static void seq_work(int request, int unused) {
    (void)unused;
    seq_put((uint32_t)request);
}

/* the game never looks at the result, so it can be put in after the frame being made */
int32_t nlsndSeqRequest(uint32_t request) {
    if (!(request & SND_REQ_VALID)) {
        return -2;
    }
    if (!snd_enabled() || !inited) {
        return 0;
    }
    if (!snd_defer(seq_work, (int)request, 0)) {
        snd_sync();
        seq_put(request);
    }
    return 0;
}

/* a request the library itself must get through: the ring is waited on while full */
static int request_now(uint32_t request) {
    int n;
    if (!snd_enabled() || !inited) {
        return 0;
    }
    for (n = 0; n < WAIT_CYCLES; n += 2048) {
        if (seq_put(request) == 0) {
            return 0;
        }
        aica_run_cpu(2048);
    }
    host_log("sound: the driver's requests stay full");
    return SND_ERR_TIMEOUT;
}

/* the driver moves a block of its RAM (from, to and the size, as SH4 addresses) */
static int block_move(uint32_t from, uint32_t to, uint32_t size) {
    mb_set(MB_MOVE_FROM, from + NLSND_G2_BASE);
    mb_set(MB_MOVE_TO, to + NLSND_G2_BASE);
    mb_set(MB_MOVE_SIZE, size);
    if (request_now(SND_REQ_STATUS) != 0) {
        return SND_ERR_TIMEOUT;
    }
    return wait_for(MB_MOVE_SIZE, 1);
}

/* ---- the driver and the packs ---- */

/* starts the sound CPU with its program, in the RAM's mode (0 for 2 MB, 0x200 for 8 MB) */
int32_t nlsndInit(const void* driver, int32_t mode) {
    const uint8_t* d = driver;
    uint32_t size, i, v;
    snd_sync();
    memcpy(&v, d + 0xC4, 4);
    if (v < SND_DRIVER_MIN) {
        return -1;
    }
    if (mode != 0 && mode != 0x200) {
        return -2;
    }
    ram_size = mode ? 0x800000 : 0x200000;
    memcpy(&size, d + 0x54, 4);
    if (!aica_init(mode != 0)) {
        host_log("sound: no memory for the sound RAM");
        return -1;
    }
    if (getenv("MVC2_DSP")) {
        aica_set_dsp(atoi(getenv("MVC2_DSP")));
    }
    if (getenv("MVC2_REVERB")) {
        aica_set_reverb(atoi(getenv("MVC2_REVERB")));
    }
    if (getenv("MVC2_ARMRATE")) {
        aica_set_cpu_rate(atoi(getenv("MVC2_ARMRATE")));
    }
    if (getenv("MVC2_FILTER")) {
        aica_set_filter(atoi(getenv("MVC2_FILTER")));
    }
    aica_sh4_write32(0x00702800, (uint32_t)mode);
    memcpy(aica_ram, d, size & ~3u);
    mb_set(MB_RAM_MODE, (uint32_t)mode);
    for (i = 0; i < SND_PORTS; i++) {
        mb_set(MB_PORT_ID + i * 4, 0xFFFFFFFFu);
        slot[i].id = -1;
        slot[i].start = 0;
        slot[i].size = 0;
    }
    for (i = 0; i < SND_STREAMS; i++) {
        stream[i].state = -1;
        stream[i].playing = 0;
        stream[i].buf[0] = stream[i].buf[1] = 0xFFFFFFFFu;
    }
    memcpy(&v, d + 0x84, 4);
    base = NLSND_RAM + v;
    memcpy(&v, d + 0x80, 4);
    stream_start = 0x10 + v;
    ring = MB_RING;
    used = 0;
    inited = 1;
    /* what the driver gives the packs (it writes this to MEM_SIZE): the RAM above its own work, less 16 KB */
    mem_size = ram_size - (base - NLSND_RAM) - 0x4000;
    if (snd_enabled()) {
        aica_sh4_write32(0x00702C00, 0);        /* out of reset */
        wait_for(MB_READY, 0);
        aica_sh4_write32(0x0070289C, aica_sh4_read32(0x0070289C) & 0x7F7);
        if (mb(MB_MEM_SIZE) != mem_size) {
            host_log("sound: the driver gives the packs %X, not %X", mb(MB_MEM_SIZE), mem_size);
        }
    }
    return 0;
}

static int32_t pack_check(const uint8_t* pack, int32_t port) {
    int32_t size;
    if (port < 0 || port >= SND_PORTS) {
        return -1;
    }
    if (memcmp(pack, "DTPK", 4) != 0) {
        return -2;
    }
    if (slot[port].id != -1) {
        return -3;
    }
    memcpy(&size, pack + 8, 4);
    if (size + used > (int32_t)mem_size) {
        return -4;
    }
    return 0;
}

/* puts a pack into its port's slot after the packs before it; the ones behind move up */
static uint32_t pack_place(int32_t port, int32_t id, int32_t size, int32_t* after) {
    int i;
    uint32_t start = 0;
    int32_t behind = 0;
    used += size;
    slot[port].id = id;
    slot[port].size = size;
    mb_set(MB_PORT_ID + port * 4, (uint32_t)id);
    for (i = port - 1; i >= 0; i--) {
        if (slot[i].id != -1) {
            start = slot[i].start + slot[i].size;
            break;
        }
    }
    if (start == 0) {
        start = base;
    }
    slot[port].start = start;
    for (i = port + 1; i < SND_PORTS; i++) {
        if (slot[i].id != -1) {
            behind += slot[i].size;
            slot[i].start += size;
        }
    }
    *after = behind;
    return start;
}

int32_t nlsndPackLoad(const void* pack, int32_t port) {
    const uint8_t* p = pack;
    int32_t error, after, id, size;
    uint32_t start;
    snd_sync();
    if ((error = pack_check(p, port)) != 0) {
        return error;
    }
    memcpy(&id, p + 4, 4);
    memcpy(&size, p + 8, 4);
    start = pack_place(port, id, size, &after);
    if (!snd_enabled()) {
        return 0;
    }
    if (request_now(SND_REQ_PACK) != 0 || wait_for(MB_LOAD_ACK, 0) != 0) {
        return SND_ERR_TIMEOUT;
    }
    if (after && block_move(start, start + size, (uint32_t)after) != 0) {
        return SND_ERR_TIMEOUT;
    }
    memcpy(aica_sh4_ptr(start), p, (uint32_t)size & ~3u);
    mb_set(MB_LOAD_GO, 1);
    error = wait_for(MB_LOAD_ACK, 1);
    mb_set(MB_LOAD_GO, 0);
    return error;
}

/* a load by slices on the board; here it is done at once, and the status finds nothing going on */
int32_t nlsndPackLoad_Open(const void* pack, int32_t port, int32_t chunk) {
    int32_t error = pack_check(pack, port);
    snd_sync();
    (void)chunk;
    if (error != 0) {
        return error;
    }
    nlsndPackLoad(pack, port);
    return 0;
}

int32_t nlsndPackLoadStatus(void) {
    return -1;
}

static int32_t free_check(int32_t port) {
    if (port < 0 || port >= SND_PORTS) {
        return -1;
    }
    if (slot[port].id == -1) {
        return -2;
    }
    return 0;
}

int32_t nlsndPackFree(int32_t port) {
    int32_t error, size, behind = 0;
    uint32_t first = 0;
    int i;
    snd_sync();
    if ((error = free_check(port)) != 0) {
        return error;
    }
    size = slot[port].size;
    used -= size;
    slot[port].id = -1;
    mb_set(MB_PORT_ID + port * 4, 0xFFFFFFFFu);
    for (i = port + 1; i < SND_PORTS; i++) {
        if (slot[i].id != -1) {
            if (first == 0) {
                first = slot[i].start;
            }
            behind += slot[i].size;
            slot[i].start -= size;
        }
    }
    if (!snd_enabled()) {
        return 0;
    }
    if (request_now(SND_REQ_PACK) != 0 || wait_for(MB_LOAD_ACK, 0) != 0) {
        return SND_ERR_TIMEOUT;
    }
    if (first && block_move(first, first - size, (uint32_t)behind) != 0) {
        return SND_ERR_TIMEOUT;
    }
    mb_set(MB_LOAD_GO, 1);
    error = wait_for(MB_LOAD_ACK, 1);
    mb_set(MB_LOAD_GO, 0);
    return error;
}

int32_t nlsndPackFreeBG(int32_t port) {
    int32_t error = free_check(port);
    snd_sync();
    if (error != 0) {
        return error;
    }
    nlsndPackFree(port);
    return 0;
}

int32_t nlsndRemainMem(void) {
    return (int32_t)mem_size - used;
}

const char* nlsndVersion(void) {
    return nlsnd_version;
}

/* ---- the controls: a command for a pack, its parameter where the command wants it ---- */

int32_t nlsndSystemControl(uint16_t pack, uint32_t command, int32_t param) {
    uint32_t request = ((pack & 15u) << 24) + command;
    if (command & 0xff00) {
        switch (command) {
        case 0x01a0: case 0x04a0: case 0x09a0: case 0x0aa0: case 0x10a0:
            request += (uint32_t)(param & 127) << 16;
            break;
        case 0x05a0: case 0x06a0: case 0x07a0: case 0x11a0:
            request += (uint32_t)((param + 64) & 127) << 16;
            break;
        case 0x1ba0:
            request += (uint32_t)(param & 15) << 16;
            break;
        case 0x30a0: case 0x28a0: case 0x29a0:
            request += (uint32_t)((param - 1) & 15) << 16;
            break;
        case 0x02a0: case 0x03a0:
            break;
        default:
            return -2;
        }
    }
    return nlsndSeqRequest(request);
}

int32_t nlsndControlChange(uint32_t pack, uint32_t midich, uint32_t command, int32_t param) {
    uint32_t request = ((pack & 15) << 24) + ((midich - 1) & 15) + command;
    switch (command) {
    case 0x01b0: case 0x02b0: case 0x07b0: case 0x27b0:
        request += (uint32_t)(param & 127) << 16;
        break;
    case 0x0ab0: case 0x2ab0:
        request += (uint32_t)((param + 64) & 127) << 16;
        break;
    default:
        return -2;
    }
    return nlsndSeqRequest(request);
}

/* ---- the streams ---- */

int32_t nlsndStreamInit(int16_t mono, int16_t stereo, int16_t mode, int16_t count) {
    int32_t unit = 0;
    uint32_t mem = 0, box = MB_STREAM_BOX, start;
    int16_t i, j;
    snd_sync();
    if (mode) {
        return -2;
    }
    count = (int16_t)(mono + stereo * 2);
    if (count > SND_STREAMS) {
        return -1;
    }
    for (j = 0; j < SND_STREAMS; j++) {
        if (stream[j].state == 1) {
            return -3;
        }
    }
    mb_set(MB_STREAM_MONO, (uint32_t)mono);
    mb_set(MB_STREAM_STEREO, (uint32_t)stereo);
    stream_mem = snd_enabled() ? mb(MB_STREAM_MEM) : 0;
    if (count) {
        stream_size = (int32_t)((stream_mem - (uint32_t)count * 64) / (uint32_t)count & 0x7fffffe0u);
        if (stream_size > 0x4000) {
            stream_size = 0x4000;
        }
        unit = stream_size + 64;
        start = snd_enabled() ? word_at(stream_start) : 0;
        if (start & 31) {
            start = (start & ~31u) + 32;
        }
        mem = ram_size - (uint32_t)unit - start;
    } else {
        stream_size = 0;
    }
    mb_set(MB_STREAM_UNIT, (uint32_t)unit);
    for (i = 0; i < mono; i++) {
        stream[i].state = stream[i].playing = 0;
        stream[i].buf[0] = NLSND_RAM + mem;
        mb_set(box + 0x0C, mem);
        mem -= (uint32_t)unit;
        stream[i].buf[1] = 0;
        mb_set(box + 0x10, 0);
        stream[i].box = box;
        box += MB_BOX_SIZE;
    }
    for (; i < mono + stereo; i++) {
        stream[i].state = stream[i].playing = 0;
        stream[i].buf[1] = NLSND_RAM + mem;
        mb_set(box + 0x10, mem);
        mem -= (uint32_t)unit;
        stream[i].buf[0] = NLSND_RAM + mem;
        mb_set(box + 0x0C, mem);
        mem -= (uint32_t)unit;
        stream[i].box = box;
        box += MB_BOX_SIZE;
    }
    for (; i < SND_STREAMS; i++) {
        stream[i].state = -1;
        stream[i].playing = 0;
        stream[i].buf[0] = stream[i].buf[1] = 0xFFFFFFFFu;
    }
    nlsndSeqRequest(0xa1);
    return 0;
}

void nlsndStreamSetHdInfo(int16_t s, const void* hd) {
    const uint8_t* h = hd;
    uint16_t freq;
    snd_sync();
    if (stream[s].state != 1 || stream[s].playing) {
        return;
    }
    memcpy(aica_ram + stream[s].box + 0x20, h, 64);
    memcpy(&freq, h + 0x2A, 2);
    stream[s].freq = freq;
    switch ((int8_t)h[8]) {
    case 0: stream[s].bits = 16; break;
    case 1: stream[s].bits = 8; break;
    case 2: case 3: stream[s].bits = 4; break;
    default: break;
    }
}

uint32_t nlsndStreamGetBufInfo(int16_t s, int16_t side, int32_t* size) {
    if (stream[s].state != 1) {
        return 0;
    }
    *size = stream_size;
    return stream[s].buf[side];
}

int32_t nlsndStreamGetFreq(int16_t s) {
    return stream[s].state == 1 ? (int32_t)stream[s].freq : 0;
}

int32_t nlsndStreamGetBitSize(int16_t s) {
    return stream[s].state == 1 ? stream[s].bits : 0;
}

uint32_t nlsndStreamGetPlayPos(int16_t s) {
    snd_sync();
    if (stream[s].state != 1 || !stream[s].playing) {
        return 0;
    }
    return mb(stream[s].box + 0x14);
}

int16_t nlsndStreamOpen(int16_t type) {
    int16_t i;
    snd_sync();
    if (type != 1 && type != 2) {
        return -1;
    }
    for (i = 0; i < SND_STREAMS; i++) {
        if (stream[i].state == 0 && (type == 1 ? stream[i].buf[1] == 0 : stream[i].buf[1] != 0)) {
            stream[i].state = 1;
            return i;
        }
    }
    return -1;
}

void nlsndStreamClose(int16_t s) {
    snd_sync();
    if (stream[s].state == 1 && !stream[s].playing) {
        stream[s].state = 0;
    }
}

void nlsndStreamStart(int16_t s) {
    snd_sync();
    if (stream[s].state == 1 && !stream[s].playing) {
        nlsndSeqRequest(((uint32_t)(int16_t)s << 12) + 0x1a1);
        stream[s].playing = 1;
    }
}

/* stops a stream: its channels keyed off at once (as the library writes their registers), then the request */
void nlsndStreamStop(int16_t s) {
    uint32_t box, chreg, v;
    snd_sync();
    if (stream[s].state != 1 || !stream[s].playing) {
        return;
    }
    box = stream[s].box;
    if (snd_enabled() && inited) {
        chreg = mb(box + 4);
        if (chreg) {
            v = aica_sh4_read32(chreg) & 0x7FF;
            aica_sh4_write32(chreg, v);
            if ((mb(box + 0x20 + 8) & 0x300) == 0x100 && mb(box + 8)) {
                uint32_t v2 = aica_sh4_read32(mb(box + 8)) & 0x7FF;
                aica_sh4_write32(mb(box + 8), v2);
            }
            aica_sh4_write32(chreg, v | 0x8000);
        }
    }
    nlsndSeqRequest(((uint32_t)(int16_t)s << 12) + 0x2a1);
    stream[s].playing = 0;
}

void nlsndStreamGenerateHdInfo(void* hd) {
    static const uint8_t def[64] = {
        'S', 'P', 'S', 'D', 0x00, 0x01, 0x00, 0x04, 0x01, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00,
        0xf7, 0x1f, 0xf7, 0x1f, 0xf7, 0x1f, 0xf7, 0x1f, 0xf7, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x04, 0x01,
        0x00, 0x00, 0x00, 0x78, 0xdf, 0x07, 0x1f, 0x3c, 0x00, 0x00, 0x22, 0x56, 0x00, 0x00, 0x00, 0x00,
        0x0f, 0x7f, 0x00, 0x00, 0x0f, 0x7f, 0x00, 0x00, 0x0f, 0x7f, 0x00, 0x00, 0x0f, 0x7f, 0x00, 0x00,
    };
    memcpy(hd, def, 64);
}

void nlsndStreamSetHdOctFns(void* hd, int16_t oct, uint16_t fns) {
    uint16_t v = (uint16_t)(oct << 11 | fns);
    memcpy((uint8_t*)hd + 0x22, &v, 2);
}

void nlsndStreamSetHdChs(void* hd, int16_t chs) {
    uint8_t* p = (uint8_t*)hd + 9;
    switch (chs) {
    case 1: *p &= 0x80; break;
    case 2: *p = (uint8_t)((*p & 0x80) | 1); break;
    case 4: *p = (uint8_t)((*p & 0x80) | 2); break;
    default: break;
    }
}

void nlsndStreamSetHdVol(uint8_t* hd, int16_t c, int32_t vol) {
    hd[c * 4 + 49] = (uint8_t)vol;
}

void nlsndStreamSetHdPan(uint8_t* hd, int16_t c, int32_t pan) {
    hd[c * 4 + 50] = (uint8_t)pan;
}

/* ---- each frame ---- */

#define FRAME_US 16683u         /* the board's frame, as the port's timer counts it */

static int16_t frame_buf[2 * 1024];
static int16_t mix_buf[2 * 1024];   /* the mixer's part of the next frame */
static uint32_t frac;
static int prepared;                /* mix_buf holds the next frame's */
static SND_MIXER mixer;
static void (*sync_fn)(void);
static int (*defer_fn)(SND_WORK work, int a, int b);
static clock_t render_time;     /* for MVC2_SNDDBG: the time the AICA takes */
static uint32_t render_frames;

void snd_set_mixer(SND_MIXER fn) {
    mixer = fn;
}

void snd_set_sync(void (*fn)(void)) {
    sync_fn = fn;
}

void snd_sync(void) {
    if (sync_fn != NULL) {
        sync_fn();
    }
}

void snd_set_defer(int (*fn)(SND_WORK work, int a, int b)) {
    defer_fn = fn;
}

int snd_defer(SND_WORK work, int a, int b) {
    return defer_fn != NULL && defer_fn(work, a, b);
}

/* the next frame's samples, without counting them yet */
static uint32_t next_count(void) {
    return (frac + AICA_RATE * FRAME_US) / 1000000u;
}

void snd_prepare(void) {
    uint32_t n = next_count();
    PROF_START(t);
    memset(mix_buf, 0, n * 4);
    if (snd_enabled()) {
        if (mixer != NULL) {
            mixer(mix_buf, (int)n);
        }
        s3l_stage();
    }
    prepared = 1;
    PROF_STOP(PZ_SND_PREPARE, t);
}

/* at a restart, before the statics are put back: aica_init's pointer is among them */
void snd_free(void) {
    snd_sync();
    aica_free();
}

void snd_close(void) {
    if (render_frames && getenv("MVC2_SNDDBG")) {
        host_log("sound: %.3f ms a frame over %u frames", (double)render_time * 1000 / CLOCKS_PER_SEC / render_frames,
                 render_frames);
    }
}

int snd_frame(const int16_t** out) {
    uint32_t n, i;
    if (!prepared) {
        snd_prepare();
    }
    prepared = 0;
    frac += AICA_RATE * FRAME_US;
    n = frac / 1000000u;
    frac -= n * 1000000u;
    if (!snd_enabled()) {
        memset(frame_buf, 0, n * 4);
    } else {
        if (inited) {
            static int dbg = -1;
            if (dbg < 0) {
                dbg = getenv("MVC2_SNDDBG") ? atoi(getenv("MVC2_SNDDBG")) : 0;
            }
            if (dbg > 0 && host_frame() % (unsigned)dbg == 0) {
                AICA_STATS st;
                aica_stats(&st);
                host_log("frame %u: %llu ARM cycles", host_frame(), (unsigned long long)st.cpu_cycles);
                aica_dump(host_log);
            }
            {
                clock_t t0 = clock();
                aica_render(frame_buf, (int)n);
                render_time += clock() - t0;
                render_frames++;
            }
        } else {
            memset(frame_buf, 0, n * 4);
        }
        for (i = 0; i < n * 2; i++) {
            int32_t v = frame_buf[i] + mix_buf[i];
            frame_buf[i] = (int16_t)(v > 32767 ? 32767 : v < -32768 ? -32768 : v);
        }
    }
    *out = frame_buf;
    return (int)n;
}

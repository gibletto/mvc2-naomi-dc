/*
 * main_3ds.c  The 3DS front end: memory, the frame loop, input, the screens
 *
 * Main RAM is mapped at NAOMI_RAM_BASE, the top of the heap area, before libctru makes its heaps.
 */

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include <3ds.h>

#include "frame_log.h"
#include "naomi_host.h"
#include "naomi_input.h"
#include "rd.h"
#include "ctr_front.h"
#include "ctr_render.h"
#include "ctr_audio.h"
#include "snd.h"
#include "aica.h"
#include "prof.h"

uint8_t* aica_sh4_ptr(uint32_t addr);       /* port/snd/aica.h */
uint32_t aica_ram_size(void);

#define HOME CTR_HOME
#define RAM_SIZE NAOMI_RAM_SIZE
#define HEAP_MAX (NAOMI_RAM_BASE - OS_HEAP_AREA_BEGIN)
/* On an Old 3DS a 3dsx has 64 MB and the CIA 80: the program about 13, main RAM 32, linear 8, and the heap
   the rest, the ROM cache getting what is left past CACHE_RESERVE */
#define LINEAR_SMALL (8u << 20)
#define LINEAR_LARGE (16u << 20)
#define HEAP_RESERVE (11u << 20)
#define DC_RAM_SIZE (16u << 20)
#ifdef MVC2_PROF
#define CACHE_RESERVE (HEAP_RESERVE + (21u << 19) - (64u << 10))  /* a profile build's larger program left room */
#else
#define CACHE_RESERVE (HEAP_RESERVE + (21u << 19))  /* and the arcade's sound RAM, and 2.5 MB: hand play takes about 1.3 of it, at least 1.5 kept free */
#endif

extern char* fake_heap_start;
extern char* fake_heap_end;
extern u32 __ctru_heap, __ctru_heap_size, __ctru_linear_heap, __ctru_linear_heap_size;

int ctr_ram_mapped;
/* weak, for a front end's setting */
int host_unlock __attribute__((weak));
static u32 mem_total;

/* libctru's, with main RAM taken first */
void __system_allocateHeaps(void) {
    ResourceLimitType commit = RESLIMIT_COMMIT;
    Handle reslimit = 0;
    s64 limit = 0, used = 0;
    u32 tmp = 0, remaining, heap, linear;

    if (R_FAILED(svcGetResourceLimit(&reslimit, CUR_PROCESS_HANDLE))) {
        svcBreak(USERBREAK_PANIC);
    }
    svcGetResourceLimitLimitValues(&limit, reslimit, &commit, 1);
    svcGetResourceLimitCurrentValues(&used, reslimit, &commit, 1);
    svcCloseHandle(reslimit);
    remaining = (u32)(limit - used) & ~0xFFFu;
#ifdef OLD_3DS_TEST
    {
        /* no more than an Old 3DS has */
        s64 old = (s64)(envIsHomebrew() ? 64 : 80) << 20;
        if (limit > old) {
            remaining = (u32)(old - used) & ~0xFFFu;
        }
    }
#endif
    mem_total = remaining;
    linear = remaining >= RAM_SIZE + 2 * LINEAR_LARGE + HEAP_RESERVE ? LINEAR_LARGE : LINEAR_SMALL;
    if (remaining >= RAM_SIZE + linear + HEAP_RESERVE &&
        R_SUCCEEDED(svcControlMemory(&tmp, NAOMI_RAM_BASE, 0, RAM_SIZE, MEMOP_ALLOC, MEMPERM_READ | MEMPERM_WRITE))) {
        ctr_ram_mapped = 1;
        remaining -= RAM_SIZE;
    } else if (remaining >= DC_RAM_SIZE + linear + HEAP_RESERVE &&
               R_SUCCEEDED(svcControlMemory(&tmp, NAOMI_RAM_BASE, 0, DC_RAM_SIZE, MEMOP_ALLOC, MEMPERM_READ | MEMPERM_WRITE))) {
        /* the Dreamcast's 16 MB alone, and its 2 MB of sound RAM: an Old 3DS's 3dsx plays it */
        ctr_ram_mapped = 1;
        host_ram_size = DC_RAM_SIZE;
        remaining -= DC_RAM_SIZE;
    }
    heap = remaining - linear;
    if (heap > HEAP_MAX) {
        heap = HEAP_MAX;
    }
    __ctru_heap_size = heap;
    __ctru_linear_heap_size = remaining - heap;
    __ctru_heap = OS_HEAP_AREA_BEGIN;
    if (R_FAILED(svcControlMemory(&tmp, __ctru_heap, 0, __ctru_heap_size, MEMOP_ALLOC, MEMPERM_READ | MEMPERM_WRITE)) ||
        R_FAILED(svcControlMemory(&__ctru_linear_heap, 0, 0, __ctru_linear_heap_size, MEMOP_ALLOC_LINEAR,
                                  MEMPERM_READ | MEMPERM_WRITE))) {
        svcBreak(USERBREAK_PANIC);
    }
    mappableInit(OS_MAP_AREA_BEGIN, OS_MAP_AREA_END);
    fake_heap_start = (char*)__ctru_heap;
    fake_heap_end = fake_heap_start + __ctru_heap_size;
}

static PrintConsole console;

static void wait_start(void) {
    while (aptMainLoop()) {
        hidScanInput();
        if (hidKeysDown() & KEY_START) {
            break;
        }
        gspWaitForVBlank();
    }
}

void ctr_tell(const char* title, const char* text) {
    consoleClear();
    printf("%s\n\n%s\n\nPress START.\n", title, text);
    wait_start();
}

static HOST_INPUT input;
static unsigned presented;
unsigned ctr_presented;     /* frames shown, for MVC2_DUMP */
static int switch_req, quit_req, select_used, restart_req, dump_req;

const HOST_INPUT* host_input(void) {
    return &input;
}

static int coin_req;

/* MVC2_PRESS=n:key,..., as on the PC: player 1 holds the key for 5 frames from frame n; coin and switch happen at n */
static uint32_t pressed(void) {
    static const struct {
        const char* name;
        uint32_t bit;
    } keys[] = {
        { "start", JVS_START }, { "up", JVS_UP }, { "down", JVS_DOWN }, { "left", JVS_LEFT }, { "right", JVS_RIGHT },
        { "b1", JVS_B1 }, { "b2", JVS_B2 }, { "b3", JVS_B3 }, { "b4", JVS_B4 }, { "b5", JVS_B5 }, { "b6", JVS_B6 },
    };
    enum { MAX_PRESSES = 1024, SWITCH = 1u << 31, COIN = 1u << 30 };
    static struct {
        unsigned at;
        uint32_t bits;      /* a key's, or SWITCH or COIN */
    } list[MAX_PRESSES];
    static int n = -1;
    uint32_t bits = 0;
    int k;
    if (n < 0) {        /* read once: a test's list runs to hundreds of presses */
        const char* p = getenv("MVC2_PRESS");
        n = 0;
        while (p != NULL && *p != 0 && n < MAX_PRESSES) {
            char name[16];
            unsigned at;
            size_t i;
            if (sscanf(p, "%u:%15[a-z0-9]", &at, name) == 2) {
                list[n].at = at;
                list[n].bits = strcmp(name, "switch") == 0 ? SWITCH : strcmp(name, "coin") == 0 ? COIN : 0;
                for (i = 0; i < sizeof keys / sizeof keys[0]; i++) {
                    if (strcmp(name, keys[i].name) == 0) {
                        list[n].bits = keys[i].bit;
                    }
                }
                n++;
            }
            p = strchr(p, ',');
            p = p != NULL ? p + 1 : NULL;
        }
    }
    for (k = 0; k < n; k++) {
        if (list[k].bits == SWITCH || list[k].bits == COIN) {
            if (presented == list[k].at) {
                *(list[k].bits == SWITCH ? &switch_req : &coin_req) = 1;
            }
        } else if (presented >= list[k].at && presented < list[k].at + 5) {
            bits |= list[k].bits;
        }
    }
    return bits;
}

static void read_input(void) {
    static const struct {
        u32 key;
        uint32_t bit;
    } keymap[] = {
        { KEY_UP, JVS_UP }, { KEY_DOWN, JVS_DOWN }, { KEY_LEFT, JVS_LEFT }, { KEY_RIGHT, JVS_RIGHT },
        { KEY_START, JVS_START },
    };
    u32 held = hidKeysHeld(), down = hidKeysDown();
    size_t i;
    input.player[0] = pressed();
    input.player[1] = 0;
    input.coin = 0;
    input.test = 0;
    if (coin_req) {
        input.coin = 1;
        coin_req = 0;
    }
    if (held & KEY_SELECT) {
        if (down & KEY_START) {
            switch_req = 1;
        }
        if (down & KEY_B) {
            quit_req = 1;
        }
        if (down & KEY_Y) {
            dump_req = 1;
        }
        if (held & (KEY_START | KEY_B | KEY_Y)) {
            select_used = 1;
        }
        held &= ~(u32)(KEY_START | KEY_B | KEY_Y);
    } else if (hidKeysUp() & KEY_SELECT) {
        input.coin = !select_used;      /* SELECT let go on its own */
        select_used = 0;
    }
    for (i = 0; i < sizeof keymap / sizeof keymap[0]; i++) {
        if (held & keymap[i].key) {
            input.player[0] |= keymap[i].bit;
        }
    }
    input.player[0] |= ctr_options_keys(held);
}

/* MVC2_REC=file: each of the game's frames' input written there, and the saves and settings as they were at the
   start beside it (file.eeprom, .backup, .cfg, .vmu); MVC2_PLAY=file plays it again, from those, and ends with it */
enum { REC_SWITCH = 1, REC_QUIT = 2, REC_DUMP = 4, REC_RESTART = 8 };
typedef struct {
    uint32_t player;
    uint8_t coin, test, flags, unused;
} REC_FRAME;
static FILE *rec_f, *play_f;

/* a second's frames at a time written and flushed by a thread below the game's priority, so that a crash loses at
   most that and the SD card never holds up a frame */
#define REC_BATCH 60
static REC_FRAME rec_buf[2][REC_BATCH];
static unsigned rec_n, rec_half;
static volatile int rec_pending = -1, rec_quit;
static LightEvent rec_wake;
static Thread rec_thread;

static void rec_writer(void* arg) {
    (void)arg;
    for (;;) {
        LightEvent_Wait(&rec_wake);
        if (rec_pending >= 0) {
            fwrite(rec_buf[rec_pending], sizeof(REC_FRAME), REC_BATCH, rec_f);
            fflush(rec_f);
            __sync_synchronize();
            rec_pending = -1;
        }
        if (rec_quit) {
            return;
        }
    }
}

static void rec_put(const REC_FRAME* r) {
    if (rec_thread == NULL) {
        fwrite(r, sizeof *r, 1, rec_f);
        return;
    }
    rec_buf[rec_half][rec_n++] = *r;
    if (rec_n == REC_BATCH) {
        while (rec_pending >= 0) {
            svcSleepThread(1000000);
        }
        rec_pending = (int)rec_half;
        LightEvent_Signal(&rec_wake);
        rec_half ^= 1;
        rec_n = 0;
    }
}

static void rec_close(void) {
    if (rec_thread != NULL) {
        rec_quit = 1;
        LightEvent_Signal(&rec_wake);
        threadJoin(rec_thread, U64_MAX);
        threadFree(rec_thread);
        rec_thread = NULL;
        fwrite(rec_buf[rec_half], sizeof(REC_FRAME), rec_n, rec_f);
    }
    fclose(rec_f);
    rec_f = NULL;
}

static void copy_file(const char* from, const char* to) {
    FILE *in = fopen(from, "rb"), *out;
    char buf[4096];
    size_t n;
    if (in == NULL) {
        return;
    }
    if ((out = fopen(to, "wb")) != NULL) {
        while ((n = fread(buf, 1, sizeof buf, in)) > 0) {
            fwrite(buf, 1, n, out);
        }
        fclose(out);
    }
    fclose(in);
}

static void rec_open(void) {
    const char* rec = getenv("MVC2_REC");
    const char* play = getenv("MVC2_PLAY");
    char to[1100];
    if (play != NULL) {
        play_f = fopen(play, "rb");
        host_log(play_f != NULL ? "input: played from %s (MVC2_PLAY)" : "input: no %s to play (MVC2_PLAY)", play);
    } else if (rec != NULL) {
        snprintf(to, sizeof to, "%s.eeprom", rec);
        copy_file(host_save_path("eeprom.bin"), to);
        snprintf(to, sizeof to, "%s.backup", rec);
        copy_file(host_save_path("backup.bin"), to);
        snprintf(to, sizeof to, "%s.cfg", rec);
        copy_file(host_save_path("mvc2.cfg"), to);
        snprintf(to, sizeof to, "%s.vmu", rec);
        copy_file(host_save_path("vmu_save_A1.bin"), to);
        rec_f = fopen(rec, "wb");
        if (rec_f != NULL) {
            s32 prio = 0x30;
            svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);
            LightEvent_Init(&rec_wake, RESET_ONESHOT);
            rec_thread = threadCreate(rec_writer, NULL, 8 << 10, prio + 2 <= 0x3F ? prio + 2 : 0x3F, 0, false);
        }
        host_log(rec_f != NULL ? "input: recorded to %s, the saves beside it (MVC2_REC)" : "input: %s not written (MVC2_REC)",
                 rec);
    }
}

static void rec_frame(void) {
    REC_FRAME r;
    if (play_f != NULL) {
        if (fread(&r, sizeof r, 1, play_f) != 1) {
            quit_req = 1;
            return;
        }
        input.player[0] = r.player;
        input.coin = r.coin;
        input.test = r.test;
        switch_req = (r.flags & REC_SWITCH) != 0;
        quit_req = (r.flags & REC_QUIT) != 0;
        dump_req = (r.flags & REC_DUMP) != 0;
        restart_req = (r.flags & REC_RESTART) != 0;
        if (restart_req) {
            setenv("MVC2_UNLOCK", "1", 1);      /* the options' one restart is UNLOCK's */
        }
    } else if (rec_f != NULL) {
        r.player = input.player[0];
        r.coin = (uint8_t)input.coin;
        r.test = (uint8_t)input.test;
        r.flags = (uint8_t)((switch_req ? REC_SWITCH : 0) | (quit_req ? REC_QUIT : 0) | (dump_req ? REC_DUMP : 0) |
                             (restart_req ? REC_RESTART : 0));
        r.unused = 0;
        rec_put(&r);
    }
}

static uint32_t* frame;      /* a shot's, or rd_soft's */
#ifdef CTR_SOFT
static int soft;            /* MVC2_SOFT, in a CTR_SOFT build */
#else
#define soft 0
#endif

static int get_frame(void) {
    if (frame == NULL) {
        frame = host_try_malloc(RD_WIDTH * RD_HEIGHT * sizeof *frame);
    }
    return frame != NULL;
}

/* MVC2_SHOT=n,m,... */
static int want_shot(unsigned frame) {
    const char* list = getenv("MVC2_SHOT");
    char want[16];
    if (list == NULL) {
        return 0;
    }
    snprintf(want, sizeof want, "%u", frame);
    for (;;) {
        size_t n = strcspn(list, ",");
        if (n == strlen(want) && strncmp(list, want, n) == 0) {
            return 1;
        }
        if (list[n] == 0) {
            return 0;
        }
        list += n + 1;
    }
}

static void put32(uint8_t* p, uint32_t v) {
    p[0] = (uint8_t)v;
    p[1] = (uint8_t)(v >> 8);
    p[2] = (uint8_t)(v >> 16);
    p[3] = (uint8_t)(v >> 24);
}

static void save_bmp(const char* name, const uint32_t* px, int w, int h) {
    uint8_t hdr[54] = { 'B', 'M' };
    FILE* f = fopen(name, "wb");
    int y;
    if (f == NULL) {
        return;
    }
    put32(hdr + 2, 54 + (uint32_t)(w * h * 4));
    put32(hdr + 10, 54);
    put32(hdr + 14, 40);
    put32(hdr + 18, (uint32_t)w);
    put32(hdr + 22, (uint32_t)h);
    hdr[26] = 1;
    hdr[28] = 32;
    fwrite(hdr, 1, sizeof hdr, f);
    for (y = h - 1; y >= 0; y--) {
        fwrite(px + y * w, 4, (size_t)w, f);
    }
    fclose(f);
}

#ifdef CTR_SOFT
/* rd_soft's frame halved, in the middle of the top screen */
static void show_soft(void) {
    u16 fw, fh;
    uint8_t* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, &fw, &fh);
    int x, y;
    memset(fb, 0, (size_t)fw * fh * 3);
    for (x = 0; x < 320; x++) {
        uint8_t* col = fb + (size_t)(x + 40) * fw * 3;
        for (y = 0; y < 240; y++) {
            const uint32_t* s = frame + y * 2 * RD_WIDTH + x * 2;
            uint32_t a = s[0], b = s[1], c = s[RD_WIDTH], d = s[RD_WIDTH + 1];
            uint8_t* p = col + (size_t)(239 - y) * 3;
            p[0] = (uint8_t)(((a & 0xFF) + (b & 0xFF) + (c & 0xFF) + (d & 0xFF)) >> 2);
            p[1] = (uint8_t)(((a >> 8 & 0xFF) + (b >> 8 & 0xFF) + (c >> 8 & 0xFF) + (d >> 8 & 0xFF)) >> 2);
            p[2] = (uint8_t)(((a >> 16 & 0xFF) + (b >> 16 & 0xFF) + (c >> 16 & 0xFF) + (d >> 16 & 0xFF)) >> 2);
        }
    }
    gfxFlushBuffers();
    gfxSwapBuffers();
}
#endif

/* frame times in the log every STATS_FRAMES */
#define STATS_FRAMES 600
static uint64_t t_end, t_window, ticks_game, ticks_draw, ticks_wait;
static unsigned drawn;

static double ms(uint64_t ticks) {
    return (double)ticks * 1000.0 / SYSCLOCK_ARM11 / STATS_FRAMES;
}

static uint32_t linear_least = 0xFFFFFFFFu, window_least = 0xFFFFFFFFu;
static uint32_t heap_window_least = 0xFFFFFFFFu;     /* the heap past malloc's top, the room for a new block */

/* MVC2_FRAMELOG=FILE: a line a frame, kept in memory and written at exit */
typedef struct {
    uint32_t frame;
    float game, draw, wait, gap, gd_ms, sync_ms, conv_ms, tex_ms, lz_ms, open_ms, end_ms, adx_ms, fopen_ms, seek_ms, fclose_ms;
    uint32_t gd_kb, sync, req, open, conv, gd_wait, discard;
} FL_REC;
static FL_REC* fl_recs;
static uint32_t fl_n, fl_max;

static void fl_record(uint64_t t0, uint64_t t1, uint64_t t2, uint64_t prev_end, uint64_t prev_t2) {
    static int on = -1;
    FL_REC* r;
    if (on < 0) {
        on = getenv("MVC2_FRAMELOG") != NULL;
        fl_max = 20000;
        fl_recs = on ? malloc(fl_max * sizeof *fl_recs) : NULL;
        on = fl_recs != NULL;
    }
    if (!on || fl_n >= fl_max) {
        memset(&fl, 0, sizeof fl);
        return;
    }
    r = &fl_recs[fl_n++];
    r->frame = presented;
    r->game = prev_end ? (float)((double)(t0 - prev_end) * 1000.0 / SYSCLOCK_ARM11) : 0;
    r->draw = (float)((double)(t1 - t0) * 1000.0 / SYSCLOCK_ARM11);
    r->wait = (float)((double)(t2 - t1) * 1000.0 / SYSCLOCK_ARM11);
    r->gap = prev_t2 ? (float)((double)(t2 - prev_t2) * 1000.0 / SYSCLOCK_ARM11) : 0;
    r->gd_ms = (float)(fl.gd_ns / 1e6);
    r->sync_ms = (float)(fl.sync_ns / 1e6);
    r->conv_ms = (float)(fl.conv_ns / 1e6);
    r->tex_ms = (float)(fl.tex_ns / 1e6);
    r->lz_ms = (float)(fl.lz_ns / 1e6);
    r->open_ms = (float)(fl.open_ns / 1e6);
    r->end_ms = (float)(fl.end_ns / 1e6);
    r->adx_ms = (float)(fl.adx_ns / 1e6);
    r->fopen_ms = (float)(fl.fopen_ns / 1e6);
    r->seek_ms = (float)(fl.seek_ns / 1e6);
    r->fclose_ms = (float)(fl.fclose_ns / 1e6);
    r->gd_kb = fl.gd_bytes >> 10;
    r->sync = fl.gd_sync;
    r->req = fl.gd_req;
    r->open = fl.gd_open;
    r->conv = fl.tex_conv;
    r->gd_wait = fl.gd_wait;
    r->discard = rd_discard;
    memset(&fl, 0, sizeof fl);
}

/* n, or v to hundredths, and a space, at p: without printf's floats, slow on an Old 3DS */
static char* fl_int(char* p, unsigned long n) {
    char d[12];
    int k = 0;
    do {
        d[k++] = (char)('0' + n % 10);
        n /= 10;
    } while (n != 0);
    while (k > 0) {
        *p++ = d[--k];
    }
    *p++ = ' ';
    return p;
}

static char* fl_ms(char* p, float v) {
    unsigned long c = v > 0 ? (unsigned long)(v * 100.0f + 0.5f) : 0;
    p = fl_int(p, c / 100);
    p[-1] = '.';
    *p++ = (char)('0' + c / 10 % 10);
    *p++ = (char)('0' + c % 10);
    *p++ = ' ';
    return p;
}

static void fl_write(void) {
    FILE* f;
    uint32_t i;
    if (fl_recs == NULL || (f = fopen(getenv("MVC2_FRAMELOG"), "w")) == NULL) {
        return;
    }
    setvbuf(f, NULL, _IOFBF, 256 << 10);    /* the SD card written in few calls */
    fprintf(f, "frame game draw wait gap gd_ms sync_ms gd_kb sync req open conv conv_ms gd_wait discard tex_ms lz_ms open_ms end_ms adx_ms fopen_ms seek_ms fclose_ms\n");
    for (i = 0; i < fl_n; i++) {
        FL_REC* r = &fl_recs[i];
        char line[320], *p = line;
        p = fl_int(p, r->frame);
        p = fl_ms(p, r->game);
        p = fl_ms(p, r->draw);
        p = fl_ms(p, r->wait);
        p = fl_ms(p, r->gap);
        p = fl_ms(p, r->gd_ms);
        p = fl_ms(p, r->sync_ms);
        p = fl_int(p, r->gd_kb);
        p = fl_int(p, r->sync);
        p = fl_int(p, r->req);
        p = fl_int(p, r->open);
        p = fl_int(p, r->conv);
        p = fl_ms(p, r->conv_ms);
        p = fl_int(p, r->gd_wait);
        p = fl_int(p, r->discard);
        p = fl_ms(p, r->tex_ms);
        p = fl_ms(p, r->lz_ms);
        p = fl_ms(p, r->open_ms);
        p = fl_ms(p, r->end_ms);
        p = fl_ms(p, r->adx_ms);
        p = fl_ms(p, r->fopen_ms);
        p = fl_ms(p, r->seek_ms);
        p = fl_ms(p, r->fclose_ms);
        p[-1] = '\n';
        fwrite(line, 1, (size_t)(p - line), f);
    }
    fclose(f);
}

static void stats(uint64_t t0, uint64_t t1, uint64_t t2) {
    static uint64_t fl_prev_t2;
    uint32_t lin = linearSpaceFree(), heap = (uint32_t)(fake_heap_end - (char*)sbrk(0));
    linear_least = lin < linear_least ? lin : linear_least;
    window_least = lin < window_least ? lin : window_least;
    heap_window_least = heap < heap_window_least ? heap : heap_window_least;
    if (t_end != 0) {
        PROF_ADD(PZ_GAME, t0 - t_end);
        ticks_game += t0 - t_end;
        ticks_draw += t1 - t0;
        ticks_wait += t2 - t1;
    }
    fl_record(t0, t1, t2, t_end, fl_prev_t2);
    fl_prev_t2 = t2;
    t_end = t2;
    if (t_window == 0) {
        t_window = t2;
    } else if (presented % STATS_FRAMES == 0) {
        uint64_t r[CTR_TICKS];
        uint32_t c[CTR_COUNTS];
        ctr_render_stats(r, c);
        host_log("frames %u-%u: %.1f a second, %u drawn; game %.2f ms, drawing %.2f ms (GPU sync %.2f, lists %.2f of "
                 "which textures %.2f, translucent sort %.2f and list %.2f; draws %.2f, end %.2f), vblank wait %.2f ms", presented - STATS_FRAMES + 1, presented,
                 STATS_FRAMES * (double)SYSCLOCK_ARM11 / (double)(t2 - t_window), drawn, ms(ticks_game), ms(ticks_draw),
                 ms(r[CTR_TICKS_SYNC]), ms(r[CTR_TICKS_LISTS]), ms(r[CTR_TICKS_TEX]), ms(r[CTR_TICKS_SORT]), ms(r[CTR_TICKS_TRANS]), ms(r[CTR_TICKS_DRAW]),
                 ms(r[CTR_TICKS_END]), ms(ticks_wait));
        host_log("  a frame drawn: %.0f triangles in %.0f draws; %.1f textures made (%.0f texels), %.1f taken back as kept, %.1f let go for room",
                 (double)c[CTR_COUNT_TRIS] / (drawn ? drawn : 1), (double)c[CTR_COUNT_DRAWS] / (drawn ? drawn : 1),
                 (double)c[CTR_COUNT_MADE] / (drawn ? drawn : 1), (double)c[CTR_COUNT_TEXELS] / (drawn ? drawn : 1),
                 (double)c[CTR_COUNT_KEPT] / (drawn ? drawn : 1), (double)c[CTR_COUNT_FREED] / (drawn ? drawn : 1));
        host_log("  heap free %lu KB (least %lu KB); the triangle pool's parts laid out again %lu times, %lu triangles not drawn",
                 (unsigned long)heap >> 10, (unsigned long)heap_window_least >> 10, (unsigned long)c[CTR_COUNT_MOVED],
                 (unsigned long)c[CTR_COUNT_DROPPED]);
        heap_window_least = 0xFFFFFFFFu;
        {
            static uint32_t deep_seen;
            uint32_t size, used;
            ctr_render_meshes(&size, &used);
            host_log("  %.1f model groups drawn by the GPU, a slice at a time (of %.1f groups), in %.1f runs, %.0f indices of "
                     "%.0f mesh vertices; their meshes %lu KB of %lu, %.2f made a frame (%.1f KB), %.2f refused for room; "
                     "linear heap free %lu KB (least %lu KB); lists built again after the wait in %lu frames; %.1f translucent "
                     "groups turned here for their depth",
                     (double)c[CTR_COUNT_GROUPS] / (drawn ? drawn : 1),
                     (double)c[CTR_COUNT_GROUPS_ONCE] / (drawn ? drawn : 1), (double)c[CTR_COUNT_TL_RUNS] / (drawn ? drawn : 1),
                     (double)c[CTR_COUNT_TL_INDICES] / (drawn ? drawn : 1), (double)c[CTR_COUNT_TL_VERTS] / (drawn ? drawn : 1),
                     (unsigned long)used >> 10, (unsigned long)size >> 10, (double)c[CTR_COUNT_MESHES] / (drawn ? drawn : 1),
                     (double)c[CTR_COUNT_MESH_BYTES] / 1024.0 / (drawn ? drawn : 1),
                     (double)c[CTR_COUNT_MESH_FULL] / (drawn ? drawn : 1), (unsigned long)lin >> 10,
                     (unsigned long)window_least >> 10, (unsigned long)c[CTR_COUNT_EARLY_AGAIN],
                     (double)(rd_gpu_tl_deep_left - deep_seen) / (drawn ? drawn : 1));
            deep_seen = rd_gpu_tl_deep_left;
            if (rd_gpu_tl_nearclip) {
                static uint32_t near_seen;
                host_log("  %.2f groups a frame near-clipped here (MVC2_TL_NEARCLIP)", (double)(rd_gpu_tl_near_left - near_seen) / (drawn ? drawn : 1));
                near_seen = rd_gpu_tl_near_left;
            }
            window_least = 0xFFFFFFFFu;
            if (c[CTR_COUNT_STAGES1] > 0) {
                host_log("  lists: %.2f of their 5 stages a frame on core 1, stages held back for the sound in %lu frames",
                         (double)c[CTR_COUNT_STAGES1] / (drawn ? drawn : 1), (unsigned long)c[CTR_COUNT_HELD]);
            }
        }
#ifdef MVC2_PROF
        {
            uint32_t p[5];
            ctr_flush_probe(p);
            host_log("  memory: %lu blocks, %lu in use; flushes of clean lines: 4 KB %lu to %lu ticks, 64 KB %lu",
                     (unsigned long)p[0], (unsigned long)p[1], (unsigned long)p[2], (unsigned long)p[3], (unsigned long)p[4]);
        }
#endif
        t_window = t2;
        ticks_game = ticks_draw = ticks_wait = 0;
        drawn = 0;
    }
}

/* with MVC2_SKIP set, drawing skipped when more than a frame behind, up to MAX_SKIP running; else every frame drawn */
#define MAX_SKIP 3
static int skip_frame(uint64_t now) {
    static uint64_t due;
    static int skipped, skip = -1;
    const uint64_t frame = (uint64_t)(SYSCLOCK_ARM11 / 59.831);
    if (skip < 0) {
        skip = getenv("MVC2_SKIP") != NULL;
    }
    due += frame;
    if (due + 30 * frame < now || due > now + frame) {
        due = now;      /* half a second behind, or the start */
    }
    if (now > due + frame && skipped < MAX_SKIP && skip) {
        skipped++;
        return 1;
    }
    skipped = 0;
    return 0;
}

/* the next frame skipped or not, before the game makes it: when it will end more than a frame late, by
   this frame's end and its game time again (skipped, its models aren't made) */
static void decide_next(uint64_t t0) {
    static uint64_t t_end;
    uint64_t now = svcGetSystemTick();
    rd_discard = !soft && !want_shot(presented + 1) && skip_frame(now + (t_end != 0 ? t0 - t_end : 0));
    t_end = now;
}

/* shot_n.bmp, and with MVC2_SOFT_SHOT in a CTR_SOFT build rd_soft's soft_n.bmp */
void host_present(void) {
    char name[32];
    int shot;
    uint64_t t0 = svcGetSystemTick(), t1;
    PROF_PLACE(PP_PRESENT);
    ctr_presented = ++presented;
    ctr_rom_poll();
    shot = want_shot(presented);
    if (rd_discard) {
        rd_begin_frame();
        stats(t0, t0, t0);
        decide_next(t0);
        PROF_FRAME();
        return;
    }
    drawn++;
#ifdef CTR_SOFT
    if ((soft || (shot && getenv("MVC2_SOFT_SHOT") != NULL)) && get_frame()) {
        rd_end_frame(frame, RD_WIDTH);
        if (shot) {
            snprintf(name, sizeof name, soft ? "shot_%u.bmp" : "soft_%u.bmp", presented);
            save_bmp(name, frame, RD_WIDTH, RD_HEIGHT);
        }
    }
    if (soft) {
        show_soft();
    } else
#endif
    {
        ctr_render_frame();
        /* MVC2_SHOT_SCREEN=1: the top screen's 400x240 (800x240 wide), as the display transfer makes it */
        int screen = getenv("MVC2_SHOT_SCREEN") != NULL && strcmp(getenv("MVC2_SHOT_SCREEN"), "0") != 0;
        if (shot && get_frame() && ctr_render_read(frame, screen)) {
            snprintf(name, sizeof name, "shot_%u.bmp", presented);
            save_bmp(name, frame, screen ? ctr_render_top_w() : RD_WIDTH, screen ? 240 : RD_HEIGHT);
        }
    }
    rd_begin_frame();
    t1 = svcGetSystemTick();
    if (soft) {
        gspWaitForVBlank();
    }
    stats(t0, t1, svcGetSystemTick());
    PROF_ADD(PZ_PRESENT, t1 - t0);
    decide_next(t0);
    PROF_FRAME();
}

/* MVC2_RAMMAP: the megabytes of each RAM the game has written */
static void ram_map(const char* name, const uint8_t* base, uint32_t size) {
    char map[(32u << 20 >> 18) + 1];
    uint32_t page, top = 0;
    size = size < 32u << 20 ? size : 32u << 20;
    for (page = 0; page < size >> 12; page++) {
        const uint32_t* w = (const uint32_t*)(base + (page << 12));
        int i, used = 0;
        for (i = 0; i < 1024 && !used; i++) {
            used = w[i] != 0;
        }
        if ((page & 63) == 0) {
            map[page >> 6] = '.';
        }
        if (used) {
            map[page >> 6] = '#';
            top = (page + 1) << 12;
        }
    }
    map[size >> 18] = 0;
    host_log("%s written (256 KB a mark): %s; up to +0x%lX", name, map, (unsigned long)top);
}

/* scale = stretch (or MVC2_STRETCH=1): the game across the top screen's 400 */
static int stretched(void) {
    const char* e = getenv("MVC2_STRETCH");
    if (e != NULL) {
        return strcmp(e, "0") != 0;
    }
    return config_get("scale") != NULL && strcmp(config_get("scale"), "stretch") == 0;
}

/* render = exact (or MVC2_RDQUALITY): 640x480 halved; else 320x240 */
static int full_size(void) {
    const char* q = getenv("MVC2_RDQUALITY") != NULL ? getenv("MVC2_RDQUALITY") : config_get("render");
    return q != NULL && strcmp(q, "exact") == 0;
}

/* wide = on or off (or MVC2_WIDE): the top screen's 800 columns, on by default on a New 3DS; -1 on the original
   2DS, which has no wide mode */
static int wide(int n3ds) {
    const char* e = getenv("MVC2_WIDE") != NULL ? getenv("MVC2_WIDE") : config_get("wide");
    u8 model = CFG_MODEL_3DS;
    if (R_SUCCEEDED(cfguInit())) {
        CFGU_GetSystemModel(&model);
        cfguExit();
    }
    if (model == CFG_MODEL_2DS) {
        return -1;
    }
    return e == NULL ? n3ds : strcmp(e, "1") == 0 || strcmp(e, "on") == 0;
}

/* env.txt's NAME=value lines, for test runs; "o3ds:NAME=value" only on an Old 3DS (or the Old 3DS test build),
   "n3ds:NAME=value" only on a New 3DS */
static void read_env(int old) {
    static char line[16 << 10];     /* a press list runs to kilobytes */
    FILE* f = fopen("env.txt", "r");
    while (f != NULL && fgets(line, sizeof line, f) != NULL) {
        char* name = line;
        char* eq = strchr(line, '=');
        int skip = 0;
        if (line[0] == '#' || eq == NULL) {
            continue;
        }
        if (strncmp(line, "o3ds:", 5) == 0 || strncmp(line, "n3ds:", 5) == 0) {
            skip = (line[0] == 'o') != old;
            name += 5;
        }
        *eq = 0;
        eq[1 + strcspn(eq + 1, "\r\n")] = 0;
        host_log("env: %s%s=%.48s%s%s", name == line ? "" : line[0] == 'o' ? "(o3ds) " : "(n3ds) ", name, eq + 1,
                 strlen(eq + 1) > 48 ? "..." : "", skip ? ", skipped on this model" : "");
        if (!skip) {
            setenv(name, eq + 1, 1);
        }
    }
    if (f != NULL) {
        fclose(f);
    }
}

/* the switches set by default on every model, from the console runs; env.txt's own value (=0: off) is kept */
static void set_defaults(void) {
    static const char* const on[][2] = { { "MVC2_LISTS_CORE1", "1" }, { "MVC2_LISTS1_ABOVE", "1" },
                                         { "MVC2_TEX_SEEN", "1" }, { "MVC2_GROUP_INFO", "1" },
                                         { "MVC2_LZ_CACHE", "1" }, { "MVC2_TEX_CURRENT", "1" },
                                         { "MVC2_RAW_BACK", "1" }, { "MVC2_LISTS1_STAGES", "5" },
                                         { "MVC2_MIXSIMD", "2" }, { "MVC2_PILLARS_ONCE", "1" },
                                         { "MVC2_PILLARS_LINE", "1" }, { "MVC2_MAP_SAME", "1" },
                                         { "MVC2_FRAG_INLINE", "1" }, { "MVC2_RAW_FRONT", "1" },
                                         { "MVC2_VRAM_FB", "1" }, { "MVC2_PREP_FAST", "1" },
                                         { "MVC2_PREP_FAST2", "1" }, { "MVC2_TEX_VRAM", "1" },
                                         { "MVC2_GPU_SPLIT", "1" }, { "MVC2_DRAW_COPY", "1" },
                                         { "MVC2_MEMCPY_FAST", "1" }, { "MVC2_SNDROOM", "1" },
                                         { "MVC2_ROM_ROLL", "1" }, { "MVC2_TEX_FROM_SERIAL", "1" },
                                         { "MVC2_LOAD_KB", "48" }, { "MVC2_ADX_AHEAD", "1" },
                                         { "MVC2_TEX_LEAN", "1" }, { "MVC2_SNDNATIVE", "3" },
                                         { "MVC2_ARMSMALL", "2" }, { "MVC2_GPU_WATCH", "1" },
                                         { "MVC2_TL_NEARCLIP", "1" }, { "MVC2_FAR_CUT", "256" },
                                         { "MVC2_SLICE_CUT", "all" }, { "MVC2_RAW_BUFS", "1" },
                                         { "MVC2_TEX_UNPARK", "1" }, { "MVC2_LISTS1_YIELD", "14" },
                                         { "MVC2_GD_ASYNC", "1" }, { "MVC2_GD_AHEAD", "1" },
                                         { "MVC2_GD_NOBUF", "1" }, { "MVC2_ADX_THREAD", "1" },
                                         { "MVC2_TEX_AHEAD", "1" }, { "MVC2_GD_NEXT", "1" },
                                         { "MVC2_TEX_KEEP_FREED", "1" }, { "MVC2_GD_STEP_KB", "256" },
                                         { "MVC2_GD_KEEP", "1" }, { "MVC2_GD_PROGS", "1" },
                                         { "MVC2_ROM_DIRECT", "1" }, { "MVC2_ROM_PRELOAD", "1" } };
    int i;
    for (i = 0; i < (int)(sizeof on / sizeof *on); i++) {
        if (getenv(on[i][0]) == NULL) {
            setenv(on[i][0], on[i][1], 0);
            host_log("default: %s=%s", on[i][0], on[i][1]);
        }
    }
}

volatile int ctr_ending;

/* the reason on the bottom screen until START, except in a test run (MVC2_FRAMES or MVC2_SHOT), which ends */
static int render_on;    /* ctr_render_init done, ctr_render_exit not yet */

static int fail(const char* text) {
    int test = getenv("MVC2_FRAMES") != NULL || getenv("MVC2_SHOT") != NULL;
    ctr_ending = 1;
    host_log("stopped: %s", text);
    if (!test) {
        ctr_tell("Marvel vs. Capcom 2", text);
    }
    /* citro3d stopped before the screens it draws to */
    if (render_on) {
        ctr_render_exit();
        render_on = 0;
    }
    if (test) {
        /* a test run ends here, whatever else is still running */
        ctr_log_flush();
        fflush(stderr);
        svcExitProcess();
    }
    gfxExit();
    return 1;
}

void ctr_stop(const char* text) {
    ctr_save_flush();
    ctr_log_flush();
    ctr_audio_exit();
    fail(text);
    exit(1);
}

/* MVC2_GPU_WATCH: the sound stopped, the log closed and the reason shown until START (or for a minute), then the GPU
   reset for the HOME Menu, which APT is told to bring back. citro3d is left as it is: stopping it waits for the GPU.
   When a thread may be stuck too, the sound is only turned down, as stopping it waits for its thread. The wait reads
   the buttons only: APT's HOME would need the GPU. No stdio or atexit work at the end: another thread may hold its
   locks. A test run ends at once, and so does one already ending, whose buttons, screens and GPU may be gone */
void ctr_hung(const char* text, int cpu_too) {
    int i;
    if (ctr_ending || hidSharedMem == NULL) {
        svcExitProcess();
    }
    ctr_log_flush();
    fflush(stderr);
    if (getenv("MVC2_FRAMES") != NULL || getenv("MVC2_SHOT") != NULL) {
        fclose(stderr);
        svcExitProcess();
    }
    if (cpu_too) {
        ctr_audio_quiet();
    } else {
        ctr_audio_exit();
    }
    host_log("stopped: the GPU hung");
    ctr_log_flush();
    fflush(stderr);
    fsync(fileno(stderr));
    consoleClear();
    printf("Marvel vs. Capcom 2\n\n%s\n\nPress START.\n", text);
    ctr_render_note();
    for (i = 0; i < 60 * 60; i++) {
        hidScanInput();
        if (hidKeysDown() & KEY_START) {
            break;
        }
        svcSleepThread(16666667LL);
    }
    GSPGPU_ResetGpuCore();
    aptExit();
    svcExitProcess();
}

/* VRAM keeps what the last program left there: the top screen's two buffers black until the game draws, and the
   bottom's text flushed with them */
/* a switch or restart puts the layers' data back as it began: what this front end set in them, kept through it */
static void restarting(int after) {
    static uint32_t cache_bytes;
#ifdef MVC2_PROF
    static void (*perf)(int at);
#endif
    if (render_on) {
        ctr_render_restart(after);
    }
    if (!after) {
        cache_bytes = naomi_rom_cache_bytes;
#ifdef MVC2_PROF
        perf = aica_perf;
#endif
    } else {
        naomi_rom_cache_bytes = cache_bytes;
#ifdef MVC2_PROF
        aica_perf = perf;
#endif
        ctr_rom_start();    /* the read-ahead as the ROM's reader */
    }
}

/* each step of the exit in the log, from the one before */
static uint64_t exit_tick;

static void exit_step(const char* what) {
    uint64_t now = svcGetSystemTick();
    host_log("exit: %s, %.1f ms", what, (double)(now - exit_tick) / (SYSCLOCK_ARM11 / 1000.0));
    exit_tick = now;
}

static void clear_top(void) {
    int i;
    for (i = 0; i < 2; i++) {
        u16 w, h;
        u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, &w, &h);
        memset(fb, 0, (size_t)w * h * gspGetBytesPerPixel(gfxGetScreenFormat(GFX_TOP)));
        gfxFlushBuffers();
        gfxScreenSwapBuffers(GFX_TOP, false);
        gspWaitForVBlank();
    }
}

int ctr_vram_writable;

/* whether the CPU may write VRAM: the CIA maps it so (its .rsf), the Homebrew Launcher's 3dsx process only to be
   read, where the bottom screen's text in VRAM stopped the program at its first character. MVC2_VRAM_RO=1 runs a
   CIA the 3dsx's way */
static int vram_writable(void) {
    MemInfo mi;
    PageInfo pi;
    if (getenv("MVC2_VRAM_RO") != NULL && strcmp(getenv("MVC2_VRAM_RO"), "0") != 0) {
        return 0;
    }
    return !envIsHomebrew() ||
           (R_SUCCEEDED(svcQueryMemory(&mi, &pi, OS_VRAM_VADDR)) && (mi.perm & MEMPERM_WRITE) != 0);
}

int main(int argc, char** argv) {
    long frames_left;
    bool n3ds = false;
    int vram_fb;
    const char* serial_why;
    char* args[3] = { argc > 0 ? argv[0] : "mvc2", NULL, NULL };

    gfxInitDefault();
    consoleInit(GFX_BOTTOM, &console);
    APT_CheckNew3DS(&n3ds);
#ifndef OLD_3DS_TEST
    if (n3ds) {
        osSetSpeedupEnable(true);
    }
#endif
    mkdir("sdmc:/3ds", 0777);
    mkdir(HOME, 0777);
    chdir(HOME);
    freopen(HOME "/" CTR_LOG, "w", stderr);
#ifdef OLD_3DS_TEST
    read_env(1);
#else
    read_env(!n3ds);
#endif
    set_defaults();
    ctr_log_start(stderr);
    host_log("memory %lu KB, heap %lu KB, linear %lu KB, main RAM %s", (unsigned long)(mem_total >> 10),
             (unsigned long)(__ctru_heap_size >> 10), (unsigned long)(__ctru_linear_heap_size >> 10),
             !ctr_ram_mapped ? "not mapped" : host_ram_size < RAM_SIZE ? "mapped (the Dreamcast's 16 MB)" : "mapped");
    /* MVC2_VRAM_FB=1: the screens' framebuffers in VRAM, libctru having no way to move the top's alone */
    vram_fb = getenv("MVC2_VRAM_FB") != NULL && strcmp(getenv("MVC2_VRAM_FB"), "0") != 0;
    ctr_vram_writable = vram_writable();
    if (!ctr_vram_writable) {
        host_log("VRAM: mapped here only to be read, so the screens' framebuffers and textures are written through "
                 "linear memory (MVC2_VRAM_FB and MVC2_TEX_VRAM=2 off)");
        vram_fb = 0;
    }
    if (vram_fb) {
        gfxExit();
        gfxInit(GSP_BGR8_OES, GSP_BGR8_OES, true);
        consoleInit(GFX_BOTTOM, &console);
        clear_top();
        host_log("framebuffers: in VRAM, %lu KB of it (MVC2_VRAM_FB)",
                 (unsigned long)(OS_VRAM_SIZE - vramSpaceFree()) >> 10);
    }
#ifdef OLD_3DS_TEST
    host_log("%s, 268 MHz (the test build has no speed-up)", n3ds ? "a New 3DS" : "an Old 3DS or 2DS");
#else
    host_log("%s, %s", n3ds ? "a New 3DS" : "an Old 3DS or 2DS", n3ds ? "804 MHz with the L2 cache" : "268 MHz");
#endif
    {
        uint32_t fpscr;
        /* round to nearest, flush denormals, default NaN, as the SH-4 */
        __asm__ volatile("vmrs %0, fpscr" : "=r"(fpscr));
        host_log("FPSCR %08lX at the start", (unsigned long)fpscr);
        fpscr = (fpscr & ~0x03C00000u) | 0x03000000u;
        __asm__ volatile("vmsr fpscr, %0" : : "r"(fpscr));
    }
    printf("Marvel vs. Capcom 2\n");
#ifdef OLD_3DS_TEST
    printf("Old 3DS test build\n");
    host_log("Old 3DS test build");
#endif
    /* core 1: 80% for the sound, or 30% (Azahar's most) for the read-ahead; set before a thread runs there */
    {
        u32 limit = 0;
        if (!snd_enabled() || R_FAILED(APT_SetAppCpuTimeLimit(80)) || R_FAILED(APT_GetAppCpuTimeLimit(&limit)) ||
            limit < 80) {
            APT_SetAppCpuTimeLimit(30);
        }
    }
#ifdef MVC2_PROF
    prof_start();       /* before the ROM cache is sized */
    ctr_xmtr_bench();
#endif
    if (getenv("MVC2_SHOT") != NULL) {
        get_frame();    /* before the ROM cache */
    }
#ifdef OLD_3DS_TEST
    serial_why = "serial: the Old 3DS test build";
#else
    /* Old 3DS: core 1 is the sound's; a New 3DS draws as an Old 3DS does unless MVC2_PIPE=1 */
    serial_why = !n3ds ? "serial: an Old 3DS"
                 : getenv("MVC2_PIPE") == NULL || strcmp(getenv("MVC2_PIPE"), "0") == 0 ? "serial: as an Old 3DS (MVC2_PIPE=1: core 2)"
                                                                                       : NULL;
    if (serial_why == NULL) {
        ctr_render_reserve();
    }
#endif
    {
        struct mallinfo mi = mallinfo();
        u32 free_heap = __ctru_heap_size - (u32)mi.arena;
        naomi_rom_cache_bytes = free_heap > CACHE_RESERVE + CTR_ROM_BUFFERS ? free_heap - CACHE_RESERVE - CTR_ROM_BUFFERS : 0;
#ifdef MVC2_PROF
        host_log("heap free at start-up %lu KB", (unsigned long)(free_heap >> 10));
#endif
        ctr_rom_start();
        host_log("ROM cache %lu KB", (unsigned long)(naomi_rom_cache_bytes >> 10));
    }
    /* the second frame for the lists on core 1 out of the reserve, the ROM cache as it is */
    if (serial_why != NULL && getenv("MVC2_LISTS_CORE1") != NULL && strcmp(getenv("MVC2_LISTS_CORE1"), "0") != 0 &&
        ctr_render_reserve()) {
        serial_why = NULL;
    }
    if (!naomi_map_ram()) {
        return fail("There is not enough memory for the game's RAM. On an Old 3DS, install the CIA, which has "
                    "more than the 3dsx.");
    }
#ifdef CTR_SOFT
    soft = getenv("MVC2_SOFT") != NULL;
#endif
    frames_left = getenv("MVC2_FRAMES") ? atol(getenv("MVC2_FRAMES")) : -1;
    host_divzero_install();
    /* MVC2_MODE=dc or arcade: the mode to start in, as a test (which else starts in the arcade) or a recording needs */
    if (getenv("MVC2_MODE") != NULL) {
        args[1] = strcmp(getenv("MVC2_MODE"), "dc") == 0 ? "--dc" : "--arcade";
    }
    if (!pick_game(args[1] != NULL ? 2 : 1, args)) {
        return fail("No game data.");
    }
#ifdef OLD_3DS_TEST
    n3ds = false;
#endif
    if (!soft && !ctr_render_init(full_size(), stretched(), wide(n3ds))) {
#ifdef CTR_SOFT
        host_log("no GPU renderer: drawing with rd_soft");
        soft = 1;
#else
        return fail("The GPU renderer could not start.");
#endif
    } else if (!soft) {
        render_on = 1;
        host_log("drawing: %s", serial_why != NULL ? serial_why : ctr_render_mode());
        if (vram_fb) {
            host_log("VRAM: %lu KB left with the render target", (unsigned long)vramSpaceFree() >> 10);
        }
    }
    rec_open();
    host_keep_start();
    host_restart_hook = restarting;
    rd_begin_frame();
    if (!start_game() || ctr_rom_error != NULL) {
        return fail(ctr_rom_error != NULL ? ctr_rom_error : "The game could not start: see " HOME "/" CTR_LOG ".");
    }
    ctr_audio_init();
    {
        u32 limit = 0;
        if (R_SUCCEEDED(APT_GetAppCpuTimeLimit(&limit))) {
            host_log("core 1: %lu%% of its time for the game", (unsigned long)limit);
        }
    }
    ctr_options_show();
#ifdef MVC2_PROF
    /* inside PZ_GAME, which runs from one present's end to the next's start */
    uint32_t t_loop = prof_now();
#define LOOP_MARK() (t_loop = prof_now())
#define LOOP_ZONE(zone) do { uint32_t t_ = prof_now(); prof_add((zone), t_ - t_loop); t_loop = t_; } while (0)
#else
#define LOOP_MARK() ((void)0)
#define LOOP_ZONE(zone) ((void)0)
#endif
    while (aptMainLoop() && !quit_req) {
        LOOP_ZONE(PZ_GAME_APT);
        hidScanInput();
        switch (ctr_options_input()) {
        case OPT_SWITCH:
            switch_req = 1;
            break;
        case OPT_RESTART:
            restart_req = 1;
            break;
        }
        read_input();
        rec_frame();
        if (dump_req) {
            dump_req = 0;
            ctr_audio_dump();
        }
        LOOP_ZONE(PZ_GAME_INPUT);
        if (switch_req || restart_req) {
            ctr_audio_idle();
            if (switch_req ? !switch_game() : (host_restart(), !start_game())) {
                break;
            }
            if (ctr_rom_error != NULL) {
                break;
            }
            switch_req = restart_req = 0;
            rd_begin_frame();
            ctr_options_show();
        }
        naomi_run_frame();
        if (ctr_alloc_error != NULL) {
            ctr_log_flush();
            fail(ctr_alloc_error);                      /* not ctr_stop: the sound's thread may be the one waiting */
            exit(1);
        }
        LOOP_MARK();
        ctr_audio_frame();
        LOOP_ZONE(PZ_GAME_AUDIO);
        if (frames_left > 0 && --frames_left == 0) {
            break;
        }
    }
    ctr_ending = 1;
    exit_tick = svcGetSystemTick();
    if (quit_req) {
        consoleClear();
        printf("Marvel vs. Capcom 2\n\nClosing...\n");
        ctr_render_note();
    }
    if (rec_f != NULL) {
        rec_close();
    }
    ctr_audio_exit();
    exit_step("the sound stopped");
    ctr_save_flush();
    exit_step("the saves written");
#ifdef MVC2_PROF
    prof_stop();
#endif
    host_stub_report();
    {
        struct mallinfo mi = mallinfo();
        host_log("heap in use %lu KB (most %lu KB of %lu), linear heap free %lu KB (least %lu KB)",
                 (unsigned long)mi.uordblks >> 10, (unsigned long)mi.arena >> 10, (unsigned long)__ctru_heap_size >> 10,
                 (unsigned long)linearSpaceFree() >> 10, (unsigned long)linear_least >> 10);
    }
    if (ctr_ram_mapped && getenv("MVC2_RAMMAP") != NULL) {
        ram_map("main RAM", (const uint8_t*)NAOMI_RAM_BASE, host_ram_size);
        ram_map("sound RAM", aica_sh4_ptr(0x00800000), aica_ram_size());
    }
    host_log("exit after %u frames", presented);
    exit_step("the reports");
    fl_write();
    exit_step("the frame log written");
    ctr_log_flush();
    exit_step("the log written");
    if (render_on) {
        ctr_render_exit();
        render_on = 0;
        clear_top();
        exit_step("drawing stopped");
    }
    if (ctr_rom_error != NULL) {
        return fail(ctr_rom_error);
    }
    gfxExit();
    exit_step("the screens closed");
    ctr_log_flush();
    return 0;
}

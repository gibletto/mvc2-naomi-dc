/*
 * main_3ds.c  The 3DS front end: memory, the frame loop, input, the screens
 *
 * Main RAM is mapped at 0x0C000000, the top of the heap area, before libctru makes its heaps.
 */

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include <3ds.h>

#include "naomi_host.h"
#include "naomi_input.h"
#include "rd.h"
#include "ctr_front.h"
#include "ctr_render.h"
#include "ctr_audio.h"
#include "snd.h"
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
#define CACHE_RESERVE (HEAP_RESERVE + (8u << 20))   /* and the arcade's sound RAM */

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

int ctr_ask(const char* text) {
    consoleClear();
    printf("%s\n\nA: arcade   B: Dreamcast   START: quit\n", text);
    while (aptMainLoop()) {
        u32 down;
        hidScanInput();
        down = hidKeysDown();
        if (down & KEY_A) {
            return 0;
        }
        if (down & KEY_B) {
            return 1;
        }
        if (down & KEY_START) {
            break;
        }
        gspWaitForVBlank();
    }
    return -1;
}

static HOST_INPUT input;
static unsigned presented;
unsigned ctr_presented;     /* frames shown, for MVC2_DUMP */
static int switch_req, quit_req, select_used;

const HOST_INPUT* host_input(void) {
    return &input;
}

/* MVC2_PRESS=n:key,..., as on the PC */
static uint32_t pressed(void) {
    static const struct {
        const char* name;
        uint32_t bit;
    } keys[] = {
        { "start", JVS_START }, { "up", JVS_UP }, { "down", JVS_DOWN }, { "left", JVS_LEFT }, { "right", JVS_RIGHT },
        { "b1", JVS_B1 }, { "b2", JVS_B2 }, { "b3", JVS_B3 }, { "b4", JVS_B4 }, { "b5", JVS_B5 }, { "b6", JVS_B6 },
    };
    const char* p = getenv("MVC2_PRESS");
    uint32_t bits = 0;
    while (p != NULL && *p != 0) {
        char name[16];
        unsigned at;
        size_t i;
        if (sscanf(p, "%u:%15[a-z0-9]", &at, name) == 2 && presented == at && strcmp(name, "switch") == 0) {
            switch_req = 1;
        }
        if (sscanf(p, "%u:%15[a-z0-9]", &at, name) == 2 && presented >= at && presented < at + 5) {
            for (i = 0; i < sizeof keys / sizeof keys[0]; i++) {
                if (strcmp(name, keys[i].name) == 0) {
                    bits |= keys[i].bit;
                }
            }
        }
        p = strchr(p, ',');
        p = p != NULL ? p + 1 : NULL;
    }
    return bits;
}

static void read_input(void) {
    static const struct {
        u32 key;
        uint32_t bit;
    } keymap[] = {
        { KEY_UP, JVS_UP }, { KEY_DOWN, JVS_DOWN }, { KEY_LEFT, JVS_LEFT }, { KEY_RIGHT, JVS_RIGHT },
        { KEY_Y, JVS_B1 }, { KEY_X, JVS_B2 }, { KEY_L, JVS_B3 }, { KEY_ZL, JVS_B3 },
        { KEY_B, JVS_B4 }, { KEY_A, JVS_B5 }, { KEY_R, JVS_B6 }, { KEY_ZR, JVS_B6 }, { KEY_START, JVS_START },
    };
    u32 held = hidKeysHeld(), down = hidKeysDown();
    size_t i;
    input.player[0] = pressed();
    input.player[1] = 0;
    input.coin = 0;
    input.test = 0;
    if (held & KEY_SELECT) {
        if (down & KEY_START) {
            switch_req = 1;
        }
        if (down & KEY_B) {
            quit_req = 1;
        }
        if (held & (KEY_START | KEY_L | KEY_B)) {
            select_used = 1;
        }
        input.test = (held & KEY_L) != 0;
        held &= ~(u32)(KEY_START | KEY_L | KEY_B);
    } else if (hidKeysUp() & KEY_SELECT) {
        input.coin = !select_used;      /* SELECT let go on its own */
        select_used = 0;
    }
    for (i = 0; i < sizeof keymap / sizeof keymap[0]; i++) {
        if (held & keymap[i].key) {
            input.player[0] |= keymap[i].bit;
        }
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
        frame = malloc(RD_WIDTH * RD_HEIGHT * sizeof *frame);
    }
    return frame != NULL;
}

/* MVC2_SHOT=n,m,... */
static int want_shot(void) {
    const char* list = getenv("MVC2_SHOT");
    char want[16];
    if (list == NULL) {
        return 0;
    }
    snprintf(want, sizeof want, "%u", presented);
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

static uint32_t linear_least = 0xFFFFFFFFu;

static void stats(uint64_t t0, uint64_t t1, uint64_t t2) {
    uint32_t lin = linearSpaceFree();
    linear_least = lin < linear_least ? lin : linear_least;
    if (t_end != 0) {
        PROF_ADD(PZ_GAME, t0 - t_end);
        ticks_game += t0 - t_end;
        ticks_draw += t1 - t0;
        ticks_wait += t2 - t1;
    }
    t_end = t2;
    if (t_window == 0) {
        t_window = t2;
    } else if (presented % STATS_FRAMES == 0) {
        uint64_t* r = ctr_render_ticks;
        host_log("frames %u-%u: %.1f a second, %u drawn; game %.2f ms, drawing %.2f ms (GPU sync %.2f, lists %.2f of "
                 "which textures %.2f, translucent sort %.2f and list %.2f; draws %.2f, end %.2f), vblank wait %.2f ms", presented - STATS_FRAMES + 1, presented,
                 STATS_FRAMES * (double)SYSCLOCK_ARM11 / (double)(t2 - t_window), drawn, ms(ticks_game), ms(ticks_draw),
                 ms(r[CTR_TICKS_SYNC]), ms(r[CTR_TICKS_LISTS]), ms(r[CTR_TICKS_TEX]), ms(r[CTR_TICKS_SORT]), ms(r[CTR_TICKS_TRANS]), ms(r[CTR_TICKS_DRAW]),
                 ms(r[CTR_TICKS_END]), ms(ticks_wait));
        host_log("  a frame drawn: %.0f triangles in %.0f draws; %.1f textures made (%.0f texels), %.1f let go for room",
                 (double)ctr_render_count[CTR_COUNT_TRIS] / (drawn ? drawn : 1), (double)ctr_render_count[CTR_COUNT_DRAWS] / (drawn ? drawn : 1),
                 (double)ctr_render_count[CTR_COUNT_MADE] / (drawn ? drawn : 1), (double)ctr_render_count[CTR_COUNT_TEXELS] / (drawn ? drawn : 1),
                 (double)ctr_render_count[CTR_COUNT_FREED] / (drawn ? drawn : 1));
        memset(ctr_render_count, 0, sizeof ctr_render_count);
        t_window = t2;
        ticks_game = ticks_draw = ticks_wait = 0;
        drawn = 0;
        memset(ctr_render_ticks, 0, sizeof ctr_render_ticks);
    }
}

/* skip drawing when more than a frame behind, up to MAX_SKIP running (MVC2_NOSKIP: never) */
#define MAX_SKIP 3
static int skip_frame(uint64_t now) {
    static uint64_t due;
    static int skipped;
    const uint64_t frame = (uint64_t)(SYSCLOCK_ARM11 / 59.831);
    due += frame;
    if (due + 30 * frame < now || due > now + frame) {
        due = now;      /* half a second behind, or the start */
    }
    if (now > due + frame && skipped < MAX_SKIP && getenv("MVC2_NOSKIP") == NULL) {
        skipped++;
        return 1;
    }
    skipped = 0;
    return 0;
}

/* shot_n.bmp, and with MVC2_SOFT_SHOT in a CTR_SOFT build rd_soft's soft_n.bmp */
void host_present(void) {
    char name[32];
    int shot;
    uint64_t t0 = svcGetSystemTick(), t1;
    ctr_presented = ++presented;
    ctr_rom_poll();
    shot = want_shot();
    if (!soft && !shot && skip_frame(t0)) {
        rd_begin_frame();
        stats(t0, t0, t0);
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
        if (shot && get_frame() && ctr_render_read(frame)) {
            snprintf(name, sizeof name, "shot_%u.bmp", presented);
            save_bmp(name, frame, RD_WIDTH, RD_HEIGHT);
        }
    }
    rd_begin_frame();
    t1 = svcGetSystemTick();
    if (soft) {
        gspWaitForVBlank();
    }
    stats(t0, t1, svcGetSystemTick());
    PROF_ADD(PZ_PRESENT, t1 - t0);
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

/* render = exact (or MVC2_RDQUALITY): 640x480 halved; else 320x240 */
static int full_size(void) {
    const char* q = getenv("MVC2_RDQUALITY") != NULL ? getenv("MVC2_RDQUALITY") : config_get("render");
    return q != NULL && strcmp(q, "exact") == 0;
}

/* env.txt's NAME=value lines, for test runs */
static void read_env(void) {
    char line[512];
    FILE* f = fopen("env.txt", "r");
    while (f != NULL && fgets(line, sizeof line, f) != NULL) {
        char* eq = strchr(line, '=');
        if (line[0] == '#' || eq == NULL) {
            continue;
        }
        *eq = 0;
        eq[1 + strcspn(eq + 1, "\r\n")] = 0;
        setenv(line, eq + 1, 1);
    }
    if (f != NULL) {
        fclose(f);
    }
}

static int fail(const char* text) {
    if (getenv("MVC2_FRAMES") == NULL) {
        ctr_tell("Marvel vs. Capcom 2", text);
    }
    gfxExit();
    return 1;
}

int main(int argc, char** argv) {
    long frames_left;
    bool n3ds = false;
    char* args[2] = { argc > 0 ? argv[0] : "mvc2", NULL };

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
    read_env();
    host_log("memory %lu KB, heap %lu KB, linear %lu KB, main RAM %s", (unsigned long)(mem_total >> 10),
             (unsigned long)(__ctru_heap_size >> 10), (unsigned long)(__ctru_linear_heap_size >> 10),
             ctr_ram_mapped ? "mapped" : "not mapped");
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
#endif
    if (getenv("MVC2_SHOT") != NULL) {
        get_frame();    /* before the ROM cache */
    }
    {
        struct mallinfo mi = mallinfo();
        u32 free_heap = __ctru_heap_size - (u32)mi.arena;
        naomi_rom_cache_bytes = free_heap > CACHE_RESERVE + CTR_ROM_BUFFERS ? free_heap - CACHE_RESERVE - CTR_ROM_BUFFERS : 0;
        ctr_rom_start();
        host_log("ROM cache %lu KB", (unsigned long)(naomi_rom_cache_bytes >> 10));
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
    if (!pick_game(1, args)) {
        return fail("No game data.");
    }
    if (!soft && !ctr_render_init(full_size(), config_get("scale") != NULL && strcmp(config_get("scale"), "stretch") == 0)) {
#ifdef CTR_SOFT
        host_log("no GPU renderer: drawing with rd_soft");
        soft = 1;
#else
        return fail("The GPU renderer could not start.");
#endif
    }
    host_keep_start();
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
    consoleClear();
    printf("Marvel vs. Capcom 2\n\nSELECT coin, SELECT+START other version\nSELECT+L test, SELECT+B quit\n");
    while (aptMainLoop() && !quit_req) {
        hidScanInput();
        read_input();
        if (switch_req) {
            switch_req = 0;
            ctr_audio_idle();
            if (!switch_game() || ctr_rom_error != NULL) {
                break;
            }
            rd_begin_frame();
        }
        naomi_run_frame();
        ctr_audio_frame();
        if (frames_left > 0 && --frames_left == 0) {
            break;
        }
    }
    ctr_audio_exit();
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
        ram_map("main RAM", (const uint8_t*)NAOMI_RAM_BASE, RAM_SIZE);
        ram_map("sound RAM", aica_sh4_ptr(0x00800000), aica_ram_size());
    }
    host_log("exit after %u frames", presented);
    if (!soft) {
        ctr_render_exit();
    }
    if (ctr_rom_error != NULL) {
        return fail(ctr_rom_error);
    }
    gfxExit();
    return 0;
}

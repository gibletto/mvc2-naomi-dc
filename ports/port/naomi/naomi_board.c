/*
 * naomi_board.c  The NAOMI board as the game sees it: main RAM at 0x0C000000 (the game keeps fixed
 * addresses there), the program image, the game as a coroutine that yields in nlWaitVSync, interrupts
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#elif defined(__linux__)
#include <errno.h>
#include <sys/mman.h>
#ifndef MAP_FIXED_NOREPLACE
#define MAP_FIXED_NOREPLACE 0x100000    /* an older kernel takes it as a hint, checked below */
#endif
#endif

#include "naomi_host.h"
#include "naomi_input.h"
#include "coro.h"
#include "nl_port.h"
#include "snd.h"

typedef int (*INTR_FUNC)(void);

static const HOST_PROGRAM* program;
static coro* host_coro;
static coro* game_coro;
static INTR_FUNC intr[8];
static int render_done;
static unsigned frames;
static void (*stop_fn)(void);
static void (*frame_fn)(void);

void host_log(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fputc('\n', stderr);
    fflush(stderr);
}

/* a float returned to a caller expecting an int stays on the x87 stack, which then fills */
static void x87_check(void) {
#if defined(__i386__)
    static int reported;
    unsigned short env[14];
    int i, n = 0;
    __asm__ volatile("fnstenv %0" : "=m"(env));
    __asm__ volatile("fldenv %0" : : "m"(env));
    for (i = 0; i < 8; i++) {
        n += (env[4] >> (i * 2) & 3) != 3;
    }
    if (n != 0) {
        if (reported++ < 10) {
            host_log("x87 stack holds %d at frame %u: a float returned to a caller that takes an int?", n, frames);
        }
        __asm__ volatile("fninit");
    }
#endif
}

static const char* stub_name[512];
static unsigned stub_calls[512];
static int nstubs;

void host_stub(const char* name) {
    int i;
    for (i = 0; i < nstubs; i++) {
        if (stub_name[i] == name) {
            stub_calls[i]++;
            return;
        }
    }
    if (nstubs < 512) {
        stub_name[nstubs] = name;
        stub_calls[nstubs++] = 1;
    }
    host_log("stub: %s (frame %u)", name, frames);
}

void host_stub_report(void) {
    int i;
    for (i = 0; i < nstubs; i++) {
        host_log("calls: %8u %s", stub_calls[i], stub_name[i]);
    }
}

/* first thing in main(), before SDL or a GPU driver takes the addresses */
int naomi_map_ram(void) {
#ifdef _WIN32
    void* p = VirtualAlloc((void*)(uintptr_t)NAOMI_RAM_BASE, NAOMI_RAM_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    return p == (void*)(uintptr_t)NAOMI_RAM_BASE;
#elif defined(__3DS__)
    extern int ctr_ram_mapped;      /* mapped before libctru's heaps (port/ctr/main_3ds.c) */
    return ctr_ram_mapped;
#elif defined(__linux__)
    void* want = (void*)(uintptr_t)NAOMI_RAM_BASE;
    void* p = mmap(want, NAOMI_RAM_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED_NOREPLACE, -1, 0);
    if (p == MAP_FAILED) {
        host_log("main RAM: %s at %08X", strerror(errno), NAOMI_RAM_BASE);
        return 0;
    }
    if (p != want) {
        host_log("main RAM: the kernel gave %p, not %08X", p, NAOMI_RAM_BASE);
        munmap(p, NAOMI_RAM_SIZE);
        return 0;
    }
    return 1;
#else
    return 0;
#endif
}

/* as the boot ROM loads it, from the cartridge header's table */
static int load_program(void) {
    static uint8_t hdr[0x500];
    int i;
    if (!naomi_rom_read(0, hdr, sizeof hdr) || memcmp(hdr, "NAOMI", 5) != 0) {
        host_log("not a NAOMI cartridge");
        return 0;
    }
    for (i = 0; i < 8; i++) {
        uint32_t src, dst, size;
        memcpy(&src, hdr + 0x360 + i * 12, 4);
        memcpy(&dst, hdr + 0x364 + i * 12, 4);
        memcpy(&size, hdr + 0x368 + i * 12, 4);
        if (src == 0xFFFFFFFF) {
            break;
        }
        if (dst < NAOMI_RAM_BASE || dst + size > NAOMI_RAM_BASE + NAOMI_RAM_SIZE ||
            !naomi_rom_read(src, (void*)(uintptr_t)dst, size)) {
            return 0;
        }
    }
    return 1;
}

/* the data stays where it loaded; its words that point at the C's routines or data are moved */
static void init_data(void) {
    const HOST_RELOC* r;
    for (r = program->reloc; r->at != NULL; r++) {
        uint32_t v = (uint32_t)(uintptr_t)r->to;
        memcpy(r->at, &v, 4);
    }
}

static void game_entry(void* arg) {
    (void)arg;
    program->main();
    host_log("the game's main() returned");
    for (;;) {
        coro_switch(host_coro);
    }
}

void board_start(const HOST_PROGRAM* prog) {
    program = prog;
    init_data();
    host_coro = coro_self();
    game_coro = coro_create(game_entry, NULL, 1 << 20);
}

void board_at_stop(void (*fn)(void)) {
    stop_fn = fn;
}

void board_at_frame(void (*fn)(void)) {
    frame_fn = fn;
}

/* between frames: the game, waiting in nlWaitVSync, is dropped there */
void board_stop(void) {
    if (stop_fn != NULL) {
        stop_fn();
    }
    frame_fn = NULL;
    if (game_coro != NULL) {
        coro_delete(game_coro);
        game_coro = NULL;
    }
    naomi_rom_close();
    nl_tex_free_all();
    snd_free();
    memset((void*)(uintptr_t)NAOMI_RAM_BASE, 0, NAOMI_RAM_SIZE);
}

int naomi_init(const HOST_PROGRAM* prog, const char* rom_path) {
    if (!naomi_rom_open(rom_path) || !load_program()) {
        return 0;
    }
    board_start(prog);
    return 1;
}

void naomi_run_frame(void) {
    frames++;
    render_done = 0;
    nla_frame_input();
    if (frame_fn != NULL) {
        frame_fn();
    }
    if (intr[1] != NULL) {
        intr[1]();                  /* vertical blank in */
    }
    coro_switch(game_coro);
}

unsigned host_frame(void) {
    return frames;
}

/* the render started at the last nlWaitVSync ends at the game's first poll */
void host_poll(void) {
    if (!render_done) {
        render_done = 1;
        if (intr[0] != NULL) {
            intr[0]();
        }
    }
}

void nlWaitVSync(void) {
    x87_check();
    host_present();
    coro_switch(host_coro);
}

void nlInitInterrupt(void) {
}

void nlSetIntrFunction(int no, INTR_FUNC f) {
    if (no >= 0 && no < 8) {
        intr[no] = f;
    }
}

/* Shinobi's timer, in microseconds: it follows the frames and moves on a little each read, so a run
   is the same every time */
static unsigned frame_polls, polls_frame;

uint32_t syTmrGetCount(void) {
    host_poll();
    if (polls_frame != frames) {
        polls_frame = frames;
        frame_polls = 0;
    }
    if (frame_polls < 16000) {
        frame_polls++;
    }
    return frames * 16683u + frame_polls;
}

uint32_t syTmrDiffCount(uint32_t start, uint32_t end) {
    return end - start;
}

uint32_t syTmrCountToMicro(uint32_t count) {
    return count;
}

/* SHC's interrupt mask built-ins */
static int imask;

int host_get_imask(void) {
    return imask;
}

void host_set_imask(int mask) {
    imask = mask;
}

void cache_inv(void* p, int size) {
    (void)p;
    (void)size;
}

void cache_purge(void* p, int size) {
    (void)p;
    (void)size;
}

int nlaReadROMDMA(uint32_t offset, char* dst, uint32_t size, int wait) {
    (void)wait;
    if (getenv("MVC2_TRACE_ROM")) {
        host_log("rom %08X -> %08X %X", offset, (uint32_t)(uintptr_t)dst, size);
    }
    if (!naomi_rom_read(offset, dst, size)) {
        host_log("ROM read past the end: %08X + %X", offset, size);
        return -1;
    }
    return 0;
}

/* the cartridge unpacks it on the way */
void nlaNRSDecodeRead(uint32_t offset, char* dst, uint32_t size, int wait) {
    (void)wait;
    if (getenv("MVC2_TRACE_ROM")) {
        host_log("packed %08X -> %08X %X", offset, (uint32_t)(uintptr_t)dst, size);
    }
    naomi_m1_read(offset, (uint8_t*)dst, size);
}

int nlaCheckWaitROMDMA(void) {
    return 1;
}

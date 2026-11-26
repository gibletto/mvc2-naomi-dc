/*
 * main_mister.c  The MiSTer front end: the game on the DE10-Nano's Cortex-A9, drawn by the Pinedra GPU core's OpenGL
 * (rd_gl1), its sound to /dev/MrAudio; each part's time a frame in the log
 */

#define _GNU_SOURCE     /* RUSAGE_THREAD */
#include <errno.h>
#include <execinfo.h>
#include <malloc.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ucontext.h>
#include <unistd.h>
#include <sys/resource.h>

#include "mister_front.h"
#include "naomi_host.h"
#include "naomi_input.h"
#include "rd.h"
#include "../nl/nl_port.h"
#include "rd_gl1.h"
#include "frame_log.h"
#include "../sdl/hunt.h"
#include "snd_drv.h"
#include "test_mode.h"
#include "menu_font.h"

int fl_timing;

void snd_close(void);

static HOST_INPUT input;
static unsigned presented;
static int test_run;
static int coin_req;

const HOST_INPUT* host_input(void) {
    return &input;
}

uint64_t mister_us(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (uint64_t)t.tv_sec * 1000000u + (uint64_t)t.tv_nsec / 1000u;
}

/* MVC2_PRESS=n:key,...: player 1 holds the key for 5 frames from frame n (p2key: player 2); coin at n. Read once. */
static struct {
    unsigned at;
    int side;                       /* 0, 1, or -1 for the coin */
    uint32_t bit;
} *presses;
static int npresses = -1;

static void read_presses(void) {
    static const struct {
        const char* name;
        uint32_t bit;
    } keys[] = {
        { "start", JVS_START }, { "up", JVS_UP }, { "down", JVS_DOWN }, { "left", JVS_LEFT }, { "right", JVS_RIGHT },
        { "b1", JVS_B1 }, { "b2", JVS_B2 }, { "b3", JVS_B3 }, { "b4", JVS_B4 }, { "b5", JVS_B5 }, { "b6", JVS_B6 },
    };
    const char* p = getenv("MVC2_PRESS");
    int room = 0;
    npresses = 0;
    while (p != NULL && *p != 0) {
        char name[16];
        unsigned at;
        size_t i;
        if (sscanf(p, "%u:%15[a-z0-9]", &at, name) == 2) {
            int side = name[0] == 'p' && name[1] == '2';
            const char* key = side ? name + 2 : name;
            uint32_t bit = 0;
            for (i = 0; i < sizeof keys / sizeof keys[0]; i++) {
                if (strcmp(key, keys[i].name) == 0) {
                    bit = keys[i].bit;
                }
            }
            if (strcmp(name, "coin") == 0) {
                side = -1;
            }
            if (npresses == room) {
                void* q = realloc(presses, (size_t)(room = room * 2 + 64) * sizeof *presses);
                if (q == NULL) {
                    return;
                }
                presses = q;
            }
            presses[npresses].at = at, presses[npresses].side = side, presses[npresses].bit = bit;
            npresses++;
        }
        p = strchr(p, ',');
        p = p != NULL ? p + 1 : NULL;
    }
}

static uint32_t pressed(int side) {
    uint32_t bits = 0;
    int i;
    if (npresses < 0) {
        read_presses();
    }
    for (i = 0; i < npresses; i++) {
        if (presses[i].side < 0 && presented == presses[i].at) {
            coin_req = 1;
        }
        if (presses[i].side == side && presented >= presses[i].at && presented < presses[i].at + 5) {
            bits |= presses[i].bit;
        }
    }
    return bits;
}

static int test_open, test_held;
static void pace(void);

/* MVC2_HUNT=script: both players' input from a script (port/sdl/hunt.c), a replay the wrapper starts or a test run's */
static int hunting, hunt_done;

/* the Test button opens the port's test menu (the arcade's has no boot ROM here to go to) */
static void read_input(void) {
    if (hunting) {
        HOST_INPUT pads;
        if (!test_run) {
            mister_input_read(&pads);   /* the menu button still quits */
        }
        hunt_done = hunt_frame(&input, presented);
        input.test = 0;
        if (hunt_done && !test_run) {
            hunting = 0;    /* the replay over: the pads play on */
        }
        return;
    }
    input.player[0] = pressed(0);
    input.player[1] = pressed(1);
    input.coin = coin_req;
    input.test = 0;
    coin_req = 0;
    if (!test_run) {
        mister_input_read(&input);
    }
    if (input.test && !test_held && strstr(game_title(), "Dreamcast") == NULL) {
        test_mode_open("A", "B");
        test_open = 1;
    }
    test_held = input.test;
    input.test = 0;
}

/* the test menu's frame: its 40 by 30 characters on black, the pads' new presses its keys (the Test button
   again as B); 1 when it closed with new settings */
static int test_frame(void) {
    static uint32_t shown[RD_WIDTH * RD_HEIGHT], last;
    HOST_INPUT in;
    const TEST_SCREEN* s;
    uint32_t now, down;
    int keys, r, x, y, i;
    memset(&in, 0, sizeof in);
    mister_input_read(&in);
    now = in.player[0] | in.player[1];
    down = test_held ? 0 : now & ~last;
    keys = (down & JVS_UP ? TM_UP : 0) | (down & JVS_DOWN ? TM_DOWN : 0) | (down & JVS_LEFT ? TM_LEFT : 0) |
           (down & JVS_RIGHT ? TM_RIGHT : 0) | (down & (JVS_B5 | JVS_START) ? TM_OK : 0) | (down & JVS_B4 ? TM_BACK : 0) |
           (in.test && !test_held ? TM_BACK : 0);
    last = now;
    test_held = in.test;
    r = test_mode_keys(keys);
    if (r != TM_OPEN) {
        test_open = 0;
        last = 0;
        return r == TM_RESTART;
    }
    s = test_mode_screen();
    for (i = 0; i < RD_WIDTH * RD_HEIGHT; i++) {
        shown[i] = 0xFF000000u;
    }
    for (y = 0; y < TM_ROWS; y++) {
        for (x = 0; x < TM_COLS; x++) {
            char c[2] = { s->ch[y][x], 0 };
            if (c[0] != ' ') {
                menu_text(shown, RD_WIDTH, RD_HEIGHT, x * 16 + 3, y * 16 + 1, c, 0xFF000000u | s->color[y][x], 2);
            }
        }
    }
    mister_gl_show(shown);
    pace();
    return 0;
}

static uint32_t frame[RD_WIDTH * RD_HEIGHT];

static void put32(unsigned char* p, uint32_t v) {
    p[0] = v & 0xFF, p[1] = v >> 8 & 0xFF, p[2] = v >> 16 & 0xFF, p[3] = v >> 24;
}

/* the PC's BMP (BITMAPV4HEADER), so shots compare with its */
static void save_bmp(const char* name, const uint32_t* argb, int w, int h) {
    unsigned char hd[122];
    FILE* f = fopen(name, "wb");
    int y;
    if (f == NULL) {
        return;
    }
    memset(hd, 0, sizeof hd);
    hd[0] = 'B', hd[1] = 'M';
    put32(hd + 2, sizeof hd + (uint32_t)(w * h * 4));
    put32(hd + 10, sizeof hd);
    put32(hd + 14, 108);
    put32(hd + 18, (uint32_t)w);
    put32(hd + 22, (uint32_t)h);
    hd[26] = 1, hd[28] = 32, hd[30] = 3;
    put32(hd + 34, (uint32_t)(w * h * 4));
    put32(hd + 54, 0x00FF0000), put32(hd + 58, 0x0000FF00), put32(hd + 62, 0x000000FF), put32(hd + 66, 0xFF000000);
    put32(hd + 70, 0x57696E20);
    fwrite(hd, sizeof hd, 1, f);
    for (y = h - 1; y >= 0; y--) {
        fwrite(&argb[(size_t)y * w], (size_t)w * 4, 1, f);
    }
    fclose(f);
}

/* MVC2_SHOT=n,m,...: frames saved as shot_n.bmp */
static int shot_wanted(unsigned n) {
    const char* list = getenv("MVC2_SHOT");
    char want[16];
    if (list == NULL) {
        return 0;
    }
    snprintf(want, sizeof want, "%u", n);
    for (;;) {
        size_t len = strcspn(list, ",");
        if (len == strlen(want) && strncmp(list, want, len) == 0) {
            return 1;
        }
        if (list[len] == 0) {
            return 0;
        }
        list += len + 1;
    }
}

static void screenshot(const uint32_t* px, const char* kind) {
    char name[40];
    if (shot_wanted(presented)) {
        snprintf(name, sizeof name, "shot_%u%s.bmp", presented, kind);
        save_bmp(name, px, RD_WIDTH, RD_HEIGHT);
    }
}

/* both boards: a 27 MHz pixel clock, 858 by 525 */
static double frame_rate = 27000000.0 / (858.0 * 525.0);
static uint64_t next_frame;

static void pace_init(void) {
    const char* fps = getenv("MVC2_FPS");
    if (fps != NULL) {
        frame_rate = atof(fps);
    } else if (test_run) {
        frame_rate = 0;
    }
}

/* by the clock, unless the swap waited for the core's vblank */
static void pace(void) {
    uint64_t now, step;
    if (frame_rate <= 0 || mister_gl_vsynced()) {
        return;
    }
    step = (uint64_t)(1000000.0 / frame_rate);
    now = mister_us();
    if (next_frame == 0 || now > next_frame + step * 4) {
        next_frame = now;
    }
    if (now < next_frame) {
        struct timespec t;
        uint64_t at = next_frame;
        t.tv_sec = (time_t)(at / 1000000u), t.tv_nsec = (long)(at % 1000000u) * 1000;
        while (clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL) == EINTR) {
        }
    }
    next_frame += step;
}

/* each part's time a frame, logged every MVC2_STATS frames (in a run of MVC2_FRAMES, 300 unless set; else none) */
typedef struct {
    uint64_t sum, most, last;
} SPAN;

enum { T_GAME, T_CPU, T_DRAW, T_OUT, T_SOUND, T_MAKE, T_WAIT, T_FRAME, T_SPANS };
/* cpu: the game's time on its CPU (the rest of game it waited for one); sound: the game's thread's part; make: the
   sound's own thread's; wait: the game's for it */
static const char* const span_names[T_SPANS] = { "game", "cpu", "draw", "out", "sound", "make", "wait", "frame" };
static SPAN spans[T_SPANS];
static unsigned stat_frames, stat_every;
static uint64_t present_us, present_cpu, stat_start;
static long stat_switches;

static uint64_t cpu_us(void) {
    struct timespec t;
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &t);
    return (uint64_t)t.tv_sec * 1000000u + (uint64_t)t.tv_nsec / 1000u;
}

/* the game's thread taken off its CPU for another */
static long switches(void) {
    struct rusage r;
    getrusage(RUSAGE_THREAD, &r);
    return r.ru_nivcsw;
}

/* the process's page faults so far: the major ones read a page from the card */
static void faults(long* major, long* minor) {
    struct rusage r;
    getrusage(RUSAGE_SELF, &r);
    *major = r.ru_majflt;
    *minor = r.ru_minflt;
}

static long draw_major;             /* the last frame's major faults while it was drawn */

static void span_add(int i, uint64_t us) {
    spans[i].sum += us;
    spans[i].most = us > spans[i].most ? us : spans[i].most;
    spans[i].last = us;
}

/* the process's resident memory in KB, or (anon) the part no file backs: what the kernel can't take back */
static unsigned rss_kb(int anon) {
    unsigned long size = 0, resident = 0, shared = 0;
    FILE* f = fopen("/proc/self/statm", "r");
    if (f != NULL) {
        if (fscanf(f, "%lu %lu %lu", &size, &resident, &shared) != 3) {
            resident = shared = 0;
        }
        fclose(f);
    }
    return (unsigned)((anon ? resident - shared : resident) * (unsigned long)sysconf(_SC_PAGESIZE) / 1024);
}

/* MVC2_FRAMELOG=file: each frame's parts in microseconds, the sound queue and the memory, for the MiSTer gate */
static FILE* frame_file;

static void frame_log(void) {
    FILE* f;
    static int tried;
    static unsigned rss, anon, heap;
    int i;
    if (!tried) {
        const char* path = getenv("MVC2_FRAMELOG");
        tried = 1;
        frame_file = path != NULL ? fopen(path, "w") : NULL;
        if (frame_file != NULL) {
            setvbuf(frame_file, NULL, _IOFBF, 1 << 20);   /* written at the end: the card is mounted sync */
            f = frame_file;
            fprintf(f, "n");
            for (i = 0; i < T_SPANS; i++) {
                fprintf(f, " %s", span_names[i]);
            }
            fprintf(f, " queued dry dropped rss_kb anon_kb heap_kb\n");
        }
    }
    f = frame_file;
    if (f == NULL) {
        return;
    }
    if (presented % 60 == 0 || rss == 0) {
        struct mallinfo m = mallinfo();
        rss = rss_kb(0);
        anon = rss_kb(1);
        heap = (unsigned)(m.uordblks + m.hblkhd) / 1024;   /* malloc's blocks in use, the heap's and mapped */
    }
    fprintf(f, "%u", presented);
    for (i = 0; i < T_SPANS; i++) {
        fprintf(f, " %u", (unsigned)spans[i].last);
    }
    fprintf(f, " %u %u %u %u %u %u\n", mister_audio_queued, mister_audio_dry, mister_audio_dropped, rss, anon, heap);
}

static RD_GL1_STATS gl_sum;     /* the frames' since the last log */

/* what holds the memory: malloc's (its main arena), the ROM cache, the textures, rd_gl1's */
static void memory_line(void) {
    struct mallinfo m = mallinfo();
    char gl[96];
    RD_GL1_MEMORY g;
    mister_gl_memory(&g);
    snprintf(gl, sizeof gl, ", GL %d textures %.1f MB, arrays %.1f MB", g.textures, g.texels / 1048576.0,
             g.arrays / 1048576.0);
    host_log("memory: %u MB (%u anonymous); malloc arena %.1f in use %.1f free %.1f mapped %.1f MB; ROM cache %.1f MB, "
             "textures %.1f MB%s", rss_kb(0) / 1024, rss_kb(1) / 1024, m.arena / 1048576.0, m.uordblks / 1048576.0,
             m.fordblks / 1048576.0, m.hblkhd / 1048576.0, naomi_rom_held() / 1048576.0, nl_tex_held() / 1048576.0,
             gl);
}

/* a frame over 50 ms, as it happens: its parts, its page faults, its GL uploads, and with MVC2_LOAD_TIMES its
   texture loads, unpacks and decompression */
static void slow_frame(uint64_t took, long major, long minor) {
    const RD_GL1_STATS* g = mister_gl_stats();
    FRAME_LOG f = fl;
    memset(&fl, 0, sizeof fl);
    if (took < 50000 || stat_every == 0) {
        return;
    }
    if (fl_timing) {
        host_log("slow frame %u: loads %u (%u unpacked) %.1f ms, unpacks %.1f ms, lz %.1f ms, disc %.1f ms", presented,
                 f.tex_calls, f.tex_conv, f.tex_ns / 1e6, f.conv_ns / 1e6, f.lz_ns / 1e6, f.gd_ns / 1e6);
    }
    host_log("slow frame %u: %.1f ms: game %.1f cpu %.1f draw %.1f out %.1f sound %.1f make %.1f wait %.1f; "
             "faults %ld major (%ld drawing), %ld minor; gl %d uploads %u KB, build %.1f draw %.1f ms",
             presented, took / 1000.0, spans[T_GAME].last / 1000.0, spans[T_CPU].last / 1000.0,
             spans[T_DRAW].last / 1000.0, spans[T_OUT].last / 1000.0, spans[T_SOUND].last / 1000.0,
             spans[T_MAKE].last / 1000.0, spans[T_WAIT].last / 1000.0, major, draw_major, minor, g->uploads,
             g->upload_bytes / 1024, g->us_build / 1000.0, g->us_draw / 1000.0);
}

/* the time between one frame's measured part and the next's, over 50 ms: whether the game's thread ran (its CPU
   time), waited (its voluntary switches: a read, a lock), was put off its CPU for another, or read pages from the card */
typedef struct {
    uint64_t at, cpu;
    long waits, preempted, major;
} GAP;

static void gap_mark(GAP* g) {
    struct rusage r;
    long minor;
    g->at = mister_us();
    g->cpu = cpu_us();
    getrusage(RUSAGE_THREAD, &r);
    g->waits = r.ru_nvcsw;
    g->preempted = r.ru_nivcsw;
    faults(&g->major, &minor);
}

static void gap_check(const GAP* from) {
    GAP now;
    if (from->at == 0 || stat_every == 0) {
        return;
    }
    gap_mark(&now);
    if (now.at - from->at >= 50000) {
        host_log("stall before frame %u: %.1f ms between frames: the game's thread on its CPU %.1f ms, %ld waits, put off "
                 "its CPU %ld times, %ld pages read", presented + 1, (now.at - from->at) / 1000.0,
                 (now.cpu - from->cpu) / 1000.0, now.waits - from->waits, now.preempted - from->preempted,
                 now.major - from->major);
    }
}

static void stats_frame(void) {
    char line[600];
    int i, n;
    uint64_t now = mister_us();
    if (stat_every == 0) {
        memset(&gl_sum, 0, sizeof gl_sum);
        memset(spans, 0, sizeof spans);
        return;
    }
    if (stat_start == 0) {
        stat_start = now;
    }
    if (++stat_frames < stat_every) {
        return;
    }
    n = snprintf(line, sizeof line, "frames %u-%u:", presented - stat_frames, presented);
    for (i = 0; i < T_SPANS; i++) {
        n += snprintf(line + n, sizeof line - n, " %s %.2f (%.2f)", span_names[i],
                      spans[i].sum / 1000.0 / stat_frames, spans[i].most / 1000.0);
    }
    snprintf(line + n, sizeof line - n, " ms, %.2f fps, sound queued %u, taken off its CPU %.1f times a frame, %u MB",
             stat_frames * 1e6 / (double)(now - stat_start), mister_audio_queued,
             (double)(switches() - stat_switches) / stat_frames, rss_kb(0) / 1024);
    stat_switches = switches();
    host_log("%s", line);
    host_log("gl: build %.2f draw %.2f ms, %.1f uploads %.0f batches %.0f tris %.0f verts a frame",
             gl_sum.us_build / 1000.0 / stat_frames, gl_sum.us_draw / 1000.0 / stat_frames,
             (double)gl_sum.uploads / stat_frames,
             (double)gl_sum.batches / stat_frames, (double)gl_sum.tris / stat_frames, (double)gl_sum.verts / stat_frames);
    memset(&gl_sum, 0, sizeof gl_sum);
    memory_line();
    memset(spans, 0, sizeof spans);
    stat_frames = 0;
    stat_start = now;
}

static void gl_sum_add(const RD_GL1_STATS* g) {
    gl_sum.us_build += g->us_build, gl_sum.us_draw += g->us_draw;
    gl_sum.uploads += g->uploads;
    gl_sum.batches += g->batches, gl_sum.tris += g->tris;
    gl_sum.verts += g->verts;
}

/* piped: frame n's shot read once the next is drawn, on the drawing thread */
static void gl_shot(unsigned n) {
    static unsigned pending;
    static uint32_t px[RD_WIDTH * RD_HEIGHT];
    char name[40];
    if (pending != 0) {
        rd_gl1_read(px);
        snprintf(name, sizeof name, "shot_%u.bmp", pending);
        save_bmp(name, px, RD_WIDTH, RD_HEIGHT);
        pending = 0;
    }
    if (shot_wanted(n)) {
        pending = n;
    }
}

/* the frame handed to the drawing thread (its shot made by rd_soft here first), the last one's stats taken, then
   the next vblank waited for */
static void present_piped(uint64_t start) {
    uint64_t handed;
    presented++;
    if (shot_wanted(presented)) {
        rd_end_frame(frame, RD_WIDTH);
        screenshot(frame, "_soft");
    }
    mister_gl_hand(presented);
    gl_sum_add(mister_gl_stats());
    handed = mister_us();
    span_add(T_DRAW, mister_gl_draw_us());
    mister_gl_next();
    span_add(T_OUT, mister_us() - handed);
    present_us += mister_us() - start;
}

/* the lists drawn by the core's GPU; a shot drawn by rd_soft too, and read back a frame later (the driver reads the
   frame shown before, the new one being drawn yet) */
static void present_gl(uint64_t start) {
    static unsigned pending;
    uint64_t drawn;
    char name[40];
    long major, minor, after;
    faults(&major, &minor);
    mister_gl_draw();
    faults(&after, &minor);
    draw_major = after - major;
    gl_sum_add(&rd_gl1_stats);
    presented++;
    if (pending != 0) {
        rd_gl1_read(frame);
        snprintf(name, sizeof name, "shot_%u.bmp", pending);
        save_bmp(name, frame, RD_WIDTH, RD_HEIGHT);
        pending = 0;
    }
    if (shot_wanted(presented)) {
        rd_end_frame(frame, RD_WIDTH);
        screenshot(frame, "_soft");
        pending = presented;
    }
    rd_begin_frame();
    drawn = mister_us();
    span_add(T_DRAW, drawn - start);
    mister_gl_swap();
    span_add(T_OUT, mister_us() - drawn);
    pace();
    present_us += mister_us() - start;
}

/* MVC2_STATELOG=file: a hash of each frame's program data, and every 60th frame's main RAM too, to show that a setting
   changes nothing the game does */
static void state_log(void) {
    static FILE* f;
    static int tried;
    const HOST_PROGRAM* const programs[] = {
#ifdef MVC2_ARCADE
        &arc_program,
#endif
        &dc_program
    };
    uint32_t h[4] = { 0, 0, 0, 0 };
    size_t i, k;
    if (!tried) {
        tried = 1;
        f = getenv("MVC2_STATELOG") != NULL ? fopen(getenv("MVC2_STATELOG"), "w") : NULL;
    }
    if (f == NULL) {
        return;
    }
    for (k = (presented + 1) % 60 == 0 ? 0 : 1; k < 1 + 2 * sizeof programs / sizeof programs[0]; k++) {
        const uint32_t *w, *end;
        if (k == 0) {
            w = HOST_RAM(SH4_RAM_BASE), end = w + host_ram_size / 4;
        } else {
            const HOST_SPAN* sp = &programs[(k - 1) / 2]->span;
            w = (const uint32_t*)(k & 1 ? sp->data : sp->bss);
            end = (const uint32_t*)(k & 1 ? sp->data_end : sp->bss_end);
        }
        for (; w + 4 <= end; w += 4) {
            for (i = 0; i < 4; i++) {
                h[i] = (h[i] ^ w[i]) * 0x01000193u;
            }
        }
    }
    fprintf(f, "%u %08x%08x%08x%08x\n", presented + 1, h[0], h[1], h[2], h[3]);
}

void host_present(void) {
    uint64_t start = mister_us(), cpu = cpu_us();
    state_log();
    mister_prof_phase = 1;
    if (mister_gl_piped()) {
        present_piped(start);
    } else {
        present_gl(start);
    }
    present_cpu += cpu_us() - cpu;
    mister_prof_phase = 0;
}

int mister_pin(int cpu) {
    cpu_set_t set;
    if (getenv("MVC2_PIN") == NULL || atoi(getenv("MVC2_PIN")) == 0) {
        return 0;
    }
    CPU_ZERO(&set);
    CPU_SET(cpu, &set);
    if (sched_setaffinity(0, sizeof set, &set) != 0) {
        host_log("pin: CPU %d: %s", cpu, strerror(errno));
        return 0;
    }
    return 1;
}

void* host_try_malloc(size_t bytes) {
    return malloc(bytes);
}

void* host_try_calloc(size_t n, size_t size) {
    return calloc(n, size);
}

void host_out_of_memory(const char* what, size_t bytes) {
    host_log("stopped: out of memory: %s of %lu bytes failed", what, (unsigned long)bytes);
    mister_save_flush();
    exit(1);
}

/* the switches set by default, env.txt's own value kept: the sound CPU's driver as C and the mixer's 32x16
   multiplies (the same samples), as on the 3DS and the PSP; the arcade's sound costs 9.7 ms a frame so, 11 ms not.
   And the Dreamcast's files read ahead on the disc's reader (dc_gd.c), the same bytes in the same frames */
static void set_defaults(void) {
    static const char* const on[][2] = { { "MVC2_SNDNATIVE", "3" }, { "MVC2_MIXSIMD", "2" },   { "MVC2_GD_AHEAD", "1" },
                                         { "MVC2_GD_NEXT", "1" },     { "MVC2_GD_KEEP", "1" }, { "MVC2_GD_PROGS", "1" } };
    size_t i;
    for (i = 0; i < sizeof on / sizeof *on; i++) {
        if (getenv(on[i][0]) == NULL) {
            setenv(on[i][0], on[i][1], 0);
            if (stat_every != 0) {
                host_log("default: %s=%s", on[i][0], on[i][1]);
            }
        }
    }
}

/* SIGTERM (the wrapper's restart) and SIGINT: the run ends at the frame's end, as a quit does */
static volatile sig_atomic_t stop_req;

static void on_stop(int sig) {
    (void)sig;
    stop_req = 1;
}

/* an abort or a fault: the thread's stack to stderr (the run's log), by address, for addr2line on the build's
   unstripped binary; then the signal as it would have been */
extern char __executable_start[], etext[];

static void say(const char* s, int n) {
    if (n > 0 && write(2, s, (size_t)n) < 0) {
        return;
    }
}

static void on_fatal(int sig, siginfo_t* info, void* context) {
    const uintptr_t* sp = (const uintptr_t*)((const ucontext_t*)context)->uc_mcontext.arm_sp;
    void* at[64];
    char text[64];
    int n = backtrace(at, 64), i;
    (void)info;
    mister_log_rescue();
    say(text, snprintf(text, sizeof text, "fatal signal %d, its stack:\n", sig));
    backtrace_symbols_fd(at, n, 2);
    /* the unwinder stops at abort(): the words on the stack that point into the program, the callers among them */
    say(text, snprintf(text, sizeof text, "words on its stack that point into the program:\n"));
    for (i = 0; i < 2048; i++) {
        uintptr_t w = sp[i];
        if (w >= (uintptr_t)__executable_start && w < (uintptr_t)etext) {
            say(text, snprintf(text, sizeof text, "  sp+%d %08lx\n", i * 4, (unsigned long)w));
        }
    }
    signal(sig, SIG_DFL);
    raise(sig);
}

static void fatal_install(void) {
    static const int sigs[] = { SIGABRT, SIGSEGV, SIGBUS, SIGILL };
    struct sigaction sa;
    void* at[4];
    size_t i;
    backtrace(at, 4);   /* libgcc loaded now, not in the handler with the heap broken */
    memset(&sa, 0, sizeof sa);
    sa.sa_sigaction = on_fatal;
    sa.sa_flags = SA_SIGINFO;
    for (i = 0; i < sizeof sigs / sizeof *sigs; i++) {
        sigaction(sigs[i], &sa, NULL);
    }
}

int main(int argc, char** argv) {
    long frames_left = getenv("MVC2_FRAMES") ? atol(getenv("MVC2_FRAMES")) : -1;
    uint64_t frame_start = 0;
    test_run = frames_left >= 0;
    hunting = hunt_init(!test_run);
    atexit(mister_log_flush);
    stat_every = test_run ? 300 : 0;
    if (getenv("MVC2_STATS") != NULL) {
        stat_every = (unsigned)atoi(getenv("MVC2_STATS"));
    }
    snd_drv_quiet(stat_every == 0);
    host_stubs_quiet = stat_every == 0;
    set_defaults();
    if (!naomi_map_ram()) {
        fprintf(stderr, "could not map main RAM at %08X\n", NAOMI_RAM_BASE);
        return 1;
    }
    host_divzero_install();
    if (getenv("MVC2_NOSTACK") == NULL) {
        fatal_install();
    }
    signal(SIGTERM, on_stop);
    signal(SIGINT, on_stop);
    pace_init();
    /* 10 + PICK_ when the data is missing or unreadable: the wrapper says so on the OSD */
    if (!pick_game(argc, argv)) {
        return pick_why() != PICK_QUIT ? 10 + pick_why() : 1;
    }
    rd_quality(config_get("render"));
    fl_timing = getenv("MVC2_LOAD_TIMES") != NULL && atoi(getenv("MVC2_LOAD_TIMES")) != 0;
    nl_tex_later = 1;
    if (!mister_gl_init()) {
        fprintf(stderr, "no Pinedra GPU core to draw on\n");
        return 1;
    }
    mister_audio_open();
    if (mister_gl_piped()) {
        mister_gl_drawn = gl_shot;
    } else {
        mister_pin(0);
    }      /* after the GL driver's own threads were made */
    mister_prof_start();
    host_keep_start();
    rd_begin_frame();
    if (!start_game()) {
        return pick_why() != PICK_QUIT ? 10 + pick_why() : 1;
    }
    GAP gap = { 0 };
    for (;;) {
        uint64_t start, presenting, sounded, cpu, cpu_presenting;
        uint32_t waited;
        long major, minor, major_end, minor_end;
        if (test_open) {
            if (test_frame()) {
                mister_audio_idle();
                mister_save_flush();
                host_restart();
                if (!start_game()) {
                    return pick_why() != PICK_QUIT ? 10 + pick_why() : 1;
                }
                rd_begin_frame();
            }
            if (mister_input_quit() || stop_req) {
                break;
            }
            frame_start = 0;
            gap.at = 0;     /* the menu's frames are no stall */
            continue;
        }
        waited = mister_audio_wait_us;
        faults(&major, &minor);
        draw_major = 0;
        read_input();
        gap_check(&gap);
        start = mister_us(), presenting = present_us;
        cpu = cpu_us(), cpu_presenting = present_cpu;
        naomi_run_frame();
        sounded = mister_us();
        span_add(T_CPU, cpu_us() - cpu - (present_cpu - cpu_presenting));
        mister_prof_phase = 2;
        mister_audio_frame();
        mister_prof_phase = 0;
        span_add(T_SOUND, mister_us() - sounded);
        span_add(T_MAKE, mister_audio_make_us);
        span_add(T_WAIT, mister_audio_wait_us - waited);
        span_add(T_GAME, sounded - start - (present_us - presenting));
        if (frame_start != 0) {
            span_add(T_FRAME, start - frame_start);
        }
        frame_start = start;
        faults(&major_end, &minor_end);
        gap_mark(&gap);
        slow_frame(mister_us() - start, major_end - major, minor_end - minor);
        frame_log();
        stats_frame();
        if ((frames_left > 0 && --frames_left == 0) || (hunt_done && test_run) || mister_input_quit() || stop_req) {
            break;
        }
    }
    if (frame_file != NULL) {
        fclose(frame_file);
    }
    mister_save_flush();
    mister_prof_stop();
    snd_close();
    mister_audio_close();
    mister_gl_exit();
    if (stat_every != 0) {
        host_stub_report();
    }
    return 0;
}

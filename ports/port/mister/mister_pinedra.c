/*
 * mister_pinedra.c  The picture on the Pinedra GPU core: the frame's lists drawn by its OpenGL 1.4 (rd_gl1), from the
 * driver's libGL.so.1, loaded at run time; shown at the core's vblank.
 *
 * MVC2_PIPE=1 (off by default): the lists drawn on a thread of their own, which opens the GL, while the game makes
 * the next frame, begun as late as lets it be handed on when the drawing is free; the game and the drawing on CPU 0
 * (MVC2_PIPE_CPU moves the game), the driver's thread and the sound on CPU 1. Only when frames are shown changes: the
 * game's frames, their input and their sound are the same either way. MVC2_PIPELOG=file (a measuring aid, through
 * /dev/mem) logs each swap and the core's vblanks, finished frames and scanned-out buffer as they change.
 */

#define _GNU_SOURCE
#include <dirent.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/mman.h>

#include "mister_front.h"
#include "naomi_host.h"
#include "rd.h"
#include "rd_gl1.h"
#include "../nl/nl_port.h"

#define PND_LIB "/media/fat/pinedra/lib/libGL.so.1"

/* pnd_gl_get_stats' first word (driver 0.4.1): the swaps its thread has done */
#define PND_DONE 0

static void* lib;
static int (*pnd_open)(int width, int height);
static void (*pnd_swap_interval)(int interval);
static void (*pnd_swap)(void);
static void (*pnd_close)(void);
static void* (*pnd_get_proc)(const char* name);
static const void* (*pnd_get_stats)(void);
static const volatile uint32_t* counters;
static int opened, vsynced;

static void* gl_proc(const char* name) {
    void* p = pnd_get_proc(name);
    return p != NULL ? p : dlsym(lib, name);
}

static void close_gl(void) {
    if (opened) {
        rd_gl1_quit();
        pnd_close();
        opened = 0;
    }
    if (lib != NULL) {
        dlclose(lib);
        lib = NULL;
    }
}

static int open_gl(void) {
    const char* path = getenv("MVC2_LIBGL");
    const char* vsync = getenv("MVC2_VSYNC");
    const char* size = getenv("MVC2_PND_SIZE");     /* the CRT's visible area, WxH, from the wrapper */
    int w = RD_WIDTH, h = RD_HEIGHT;
    lib = dlopen(path != NULL ? path : PND_LIB, RTLD_NOW | RTLD_GLOBAL);
    if (lib == NULL) {
        host_log("gl: %s", dlerror());
        return 0;
    }
    *(void**)&pnd_open = dlsym(lib, "pnd_gl_open");
    *(void**)&pnd_swap_interval = dlsym(lib, "pnd_gl_swap_interval");
    *(void**)&pnd_swap = dlsym(lib, "pnd_gl_swap");
    *(void**)&pnd_close = dlsym(lib, "pnd_gl_close");
    *(void**)&pnd_get_proc = dlsym(lib, "pnd_gl_get_proc");
    *(void**)&pnd_get_stats = dlsym(lib, "pnd_gl_get_stats");
    if (pnd_open == NULL || pnd_swap == NULL || pnd_close == NULL || pnd_get_proc == NULL) {
        host_log("gl: not the Pinedra library");
        close_gl();
        return 0;
    }
    if (size != NULL && (sscanf(size, "%dx%d", &w, &h) != 2 || w < 64 || h < 48 || w > RD_WIDTH || h > RD_HEIGHT)) {
        w = RD_WIDTH, h = RD_HEIGHT;
    }
    if (!pnd_open(w, h)) {
        host_log("gl: no %dx%d context", w, h);
        close_gl();
        return 0;
    }
    opened = 1;
    vsynced = pnd_swap_interval != NULL && (vsync == NULL || atoi(vsync) != 0);
    if (pnd_swap_interval != NULL) {
        pnd_swap_interval(vsynced);
    }
    counters = pnd_get_stats != NULL ? pnd_get_stats() : NULL;
    /* RGBA8 (the driver's 16-bit path costs its thread more), sprite tiles pooled, the opaque list nearest first; and
       where the driver has them, the translucent list ordered by the driver and the palettes expanded by it, both
       on its own thread on the second core (Pinedra release 20261123, driver 0.5.44, for the order) */
    rd_gl1_options(getenv("MVC2_GL1") != NULL ? getenv("MVC2_GL1") : "less pool order clut");
    rd_gl1_size(w, h);
    if (!rd_gl1_init(gl_proc)) {
        close_gl();
        return 0;
    }
    return 1;
}

static void pin(int cpu) {
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(cpu, &set);
    if (pthread_setaffinity_np(pthread_self(), sizeof set, &set) != 0) {
        host_log("gl: no CPU %d for the thread", cpu);
    }
}

/* ---- the core's registers, read for MVC2_PIPELOG alone ---- */

/* core 0.12.0's count of frames finished, its vblank count (16 bits) and the frame buffer it scans out, changed at a
   vblank: a measure of what reaches the screen when, which nothing else here reads */
#define PND_REGS 0x31001000u
#define PND_FINISHED 0x0000
#define PND_VBLANKS 0x1008
#define PND_SHOWN 0x100C

static const volatile uint32_t* regs;

static int regs_open(void) {
    int fd = open("/dev/mem", O_RDONLY | O_SYNC | O_CLOEXEC);
    void* p;
    if (fd < 0) {
        return 0;
    }
    p = mmap(NULL, 8192, PROT_READ, MAP_SHARED, fd, PND_REGS);
    close(fd);
    if (p == MAP_FAILED) {
        return 0;
    }
    regs = p;
    return 1;
}

static uint32_t reg(unsigned at) {
    return regs != NULL ? regs[at / 4] : 0;
}

static FILE* pipe_log;              /* MVC2_PIPELOG=file: each swap, and the core's counts as they change */
static pthread_t sampler;
static volatile int sampling;

static void* sample_main(void* arg) {
    uint32_t v = ~0u, d = ~0u, fe = ~0u, sh = ~0u;
    cpu_set_t set;
    (void)arg;
    CPU_ZERO(&set);
    CPU_SET(1, &set);
    pthread_setaffinity_np(pthread_self(), sizeof set, &set);
    while (sampling) {
        uint32_t v1 = reg(PND_VBLANKS) & 0xFFFF, d1 = counters != NULL ? counters[PND_DONE] : 0;
        uint32_t fe1 = reg(PND_FINISHED), sh1 = reg(PND_SHOWN);
        if (v1 != v || d1 != d || fe1 != fe || sh1 != sh) {
            v = v1, d = d1, fe = fe1, sh = sh1;
            fprintf(pipe_log, "S %llu %u %u %u %x\n", (unsigned long long)mister_us(), v, d, fe, sh);
        }
        usleep(150);
    }
    return NULL;
}

static void log_open(void) {
    const char* log = getenv("MVC2_PIPELOG");
    if (log != NULL && regs_open() && (pipe_log = fopen(log, "w")) != NULL) {
        sampling = 1;
        if (pthread_create(&sampler, NULL, sample_main, NULL) != 0) {
            sampling = 0;
        }
    }
}

/* a swap: its frame, the count of swaps, the vblank its game frame began at and the one at the swap, its times */
static void log_swap(unsigned frame, unsigned swaps, uint32_t began, uint32_t v0, uint64_t t0) {
    fprintf(pipe_log, "F %u %u %u %u %llu %llu %u %u\n", frame, swaps, began, v0, (unsigned long long)t0,
            (unsigned long long)mister_us(), reg(PND_VBLANKS) & 0xFFFF, counters != NULL ? counters[PND_DONE] : 0);
}

static void log_close(void) {
    if (sampling) {
        sampling = 0;
        pthread_join(sampler, NULL);
    }
    if (pipe_log != NULL) {
        fclose(pipe_log);
        pipe_log = NULL;
    }
}

/* ---- the pipe ---- */

static int piped;
static pthread_t thread;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t changed = PTHREAD_COND_INITIALIZER;
static int state;                   /* the thread's open: 0 not yet, 1 done, -1 failed */
static int has_work, reading, quit;
static struct {
    RD_FRAME* f;
    unsigned frame;
    uint32_t began;                 /* the vblank its game frame began at, for the log */
} work;
static RD_FRAME* frames[2];
static uint64_t took_at, cycle_us;  /* when the drawing last took a frame, and the time since the one before */
static uint64_t game_from;          /* when the game frame began */
static uint32_t game_took[8], game_n;
static uint32_t began;

/* what the drawing reads but the game changes, as it was at the hand-off */
static uint32_t palette[1024], palette_serial;
static int have_palette;
static RD_FOG fog;
static RD_TEXTURE texs[NL_MAX_TEX]; /* a state's texture, at one place a texture whatever the frame */
static unsigned texs_frame[NL_MAX_TEX];

static RD_GL1_STATS last_stats;
static RD_GL1_MEMORY last_memory;
static uint32_t last_draw_us;
void (*mister_gl_drawn)(unsigned frame);

static void* render_main(void* arg) {
    unsigned swaps = 0;
    (void)arg;
    pin(0);
    pthread_setname_np(pthread_self(), "mvc2-gl");
    {
        int ok = open_gl();
        pthread_mutex_lock(&lock);
        state = ok ? 1 : -1;
        pthread_cond_broadcast(&changed);
        pthread_mutex_unlock(&lock);
        if (!ok) {
            return NULL;
        }
    }
    for (;;) {
        RD_FRAME* f;
        unsigned frame;
        uint32_t at, v0;
        uint64_t t0;
        pthread_mutex_lock(&lock);
        while (!has_work && !quit) {
            pthread_cond_wait(&changed, &lock);
        }
        if (!has_work) {
            pthread_mutex_unlock(&lock);
            break;
        }
        f = work.f, frame = work.frame, at = work.began;
        has_work = 0;
        reading = 1;
        t0 = mister_us();
        cycle_us = took_at != 0 ? t0 - took_at : 0;
        took_at = t0;
        pthread_mutex_unlock(&lock);
        rd_gl1_draw_frame(f, palette, &fog);
        pthread_mutex_lock(&lock);
        last_stats = rd_gl1_stats;
        last_draw_us = (uint32_t)(mister_us() - t0);
        if (frame % 60 == 0) {
            rd_gl1_memory(&last_memory);
        }
        reading = 0;
        pthread_cond_broadcast(&changed);
        pthread_mutex_unlock(&lock);
        if (mister_gl_drawn != NULL) {
            mister_gl_drawn(frame);
        }
        v0 = reg(PND_VBLANKS) & 0xFFFF;
        t0 = mister_us();
        pnd_swap();
        swaps++;
        if (pipe_log != NULL) {
            log_swap(frame, swaps, at, v0, t0);
        }
    }
    close_gl();
    return NULL;
}

/* where each thread is let run, once */
static void log_threads(void) {
    char line[512] = "", path[300], name[32], cpus[64], buf[256];
    int n = 0;
    struct dirent* e;
    DIR* d = opendir("/proc/self/task");
    if (d == NULL) {
        return;
    }
    while ((e = readdir(d)) != NULL && n < (int)sizeof line - 1) {
        FILE* f;
        if (e->d_name[0] == '.') {
            continue;
        }
        snprintf(path, sizeof path, "/proc/self/task/%s/status", e->d_name);
        if ((f = fopen(path, "r")) == NULL) {
            continue;
        }
        name[0] = cpus[0] = 0;
        while (fgets(buf, sizeof buf, f) != NULL) {
            sscanf(buf, "Name: %31s", name);
            sscanf(buf, "Cpus_allowed_list: %63s", cpus);
        }
        fclose(f);
        n += snprintf(line + n, sizeof line - (size_t)n, " %s:%s", name, cpus);
    }
    closedir(d);
    host_log("threads (name:CPUs):%s", line);
}

static int open_piped(void) {
    /* the driver's own run-ahead held to a frame: the frame drawn next is the one shown next */
    setenv("PND_FRAMES_AHEAD", "1", 0);
    setenv("MVC2_SNDCPU", "1", 0);     /* the sound beside the game and the driver: CPU 0 draws */
    frames[0] = rd_frame;
    frames[1] = calloc(1, sizeof *frames[1]);
    if (frames[1] == NULL || pthread_create(&thread, NULL, render_main, NULL) != 0) {
        free(frames[1]);
        return -1;
    }
    pthread_mutex_lock(&lock);
    while (state == 0) {
        pthread_cond_wait(&changed, &lock);
    }
    pthread_mutex_unlock(&lock);
    if (state < 0) {
        pthread_join(thread, NULL);
        free(frames[1]);
        return 0;
    }
    piped = 1;
    nl_tex_fresh = 1;
    pin(getenv("MVC2_PIPE_CPU") != NULL ? atoi(getenv("MVC2_PIPE_CPU")) : 0);   /* CPU 1 is the driver's */
    log_open();
    host_log("gl: drawn on a thread of its own while the game makes the next frame");
    return 1;
}

int mister_gl_init(void) {
    const char* p = getenv("MVC2_PIPE");
    if (p != NULL && atoi(p) != 0) {
        int r = open_piped();
        if (r >= 0) {
            return r;
        }
    }
    if (!open_gl()) {
        return 0;
    }
    log_open();
    return 1;
}

int mister_gl_piped(void) {
    return piped;
}

int mister_gl_ordered(void) {
    return !piped && rd_gl1_ordered();
}

/* the frame's states' textures, the palette and the fog as they are now, for the drawing to read */
static void take(RD_FRAME* f, unsigned frame) {
    int i;
    for (i = 0; i < f->nstates; i++) {
        RD_STATE* s = &f->states[i];
        size_t k;
        if (s->tex == NULL) {
            continue;
        }
        k = (size_t)((const NL_TEX*)s->tex - nl_tex);
        if (k >= NL_MAX_TEX) {
            continue;
        }
        if (texs_frame[k] != frame) {
            texs[k] = nl_tex[k].tex;
            texs_frame[k] = frame;
        }
        s->tex = &texs[k];
    }
    if (!have_palette || palette_serial != rd_palette_serial) {
        memcpy(palette, rd_palette, sizeof palette);
        palette_serial = rd_palette_serial;
        have_palette = 1;
    }
    fog = rd_fog;
}

void mister_gl_hand(unsigned frame) {
    RD_FRAME* f = rd_frame;
    if (game_from != 0) {
        game_took[game_n++ % 8] = (uint32_t)(mister_us() - game_from);
    }
    if (frame == 120) {
        log_threads();      /* the sound's thread among them by now */
    }
    pthread_mutex_lock(&lock);
    while (has_work || reading) {
        pthread_cond_wait(&changed, &lock);
    }
    pthread_mutex_unlock(&lock);
    take(f, frame);
    pthread_mutex_lock(&lock);
    work.f = f, work.frame = frame, work.began = began;
    has_work = 1;
    pthread_cond_broadcast(&changed);
    pthread_mutex_unlock(&lock);
    rd_frame = f == frames[0] ? frames[1] : frames[0];
    memcpy(rd_frame->background, f->background, sizeof rd_frame->background);   /* it lasts from frame to frame */
    rd_begin_frame();
}

/* the next game frame begun as late as it can be and still be handed on when the drawing can take it, so its input is
   read as late as it can be: the drawing is paced by the driver's swaps, and the game by the drawing */
void mister_gl_next(void) {
    uint64_t now = mister_us(), ready = 0, most = 0, start;
    struct timespec t;
    unsigned i;
    pthread_mutex_lock(&lock);
    if (has_work) {
        ready = took_at + cycle_us + last_draw_us;
    } else if (reading) {
        ready = took_at + last_draw_us;
    }
    pthread_mutex_unlock(&lock);
    for (i = 0; i < 8 && i < game_n; i++) {
        most = game_took[i] > most ? game_took[i] : most;
    }
    start = ready - most - 1500;
    if (ready != 0 && cycle_us != 0 && start > now && start < now + 50000) {
        t.tv_sec = (time_t)(start / 1000000u), t.tv_nsec = (long)(start % 1000000u) * 1000;
        while (clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL) == EINTR) {
        }
    }
    game_from = mister_us();
    began = reg(PND_VBLANKS) & 0xFFFF;
}

const RD_GL1_STATS* mister_gl_stats(void) {
    return piped ? &last_stats : &rd_gl1_stats;
}

uint32_t mister_gl_draw_us(void) {
    return last_draw_us;
}

void mister_gl_memory(RD_GL1_MEMORY* m) {
    if (piped) {
        pthread_mutex_lock(&lock);
        *m = last_memory;
        pthread_mutex_unlock(&lock);
    } else {
        rd_gl1_memory(m);
    }
}

void mister_gl_draw(void) {
    rd_gl1_end_frame();
}

void mister_gl_swap(void) {
    static unsigned n;
    uint32_t v0 = reg(PND_VBLANKS) & 0xFFFF;
    uint64_t t0 = mister_us();
    pnd_swap();
    if (pipe_log != NULL) {
        n++;
        log_swap(n, n, began, v0, t0);
        began = reg(PND_VBLANKS) & 0xFFFF;     /* the next game frame begins as this returns */
    }
}

int mister_gl_vsynced(void) {
    return (opened || piped) && vsynced;
}

void mister_gl_exit(void) {
    if (piped) {
        pthread_mutex_lock(&lock);
        quit = 1;
        pthread_cond_broadcast(&changed);
        pthread_mutex_unlock(&lock);
        pthread_join(thread, NULL);
        log_close();
        piped = 0;
        return;
    }
    log_close();
    close_gl();
}

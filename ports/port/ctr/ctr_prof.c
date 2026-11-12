/* ctr_prof.c  The zone timers on the 3DS: each frame's totals in a ring, summaries in prof.log, written by a thread */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <3ds.h>

#include "prof.h"
#include "ctr_front.h"

#define WINDOW 600              /* frames a summary covers */
#define RING 64                 /* frames held for prof.bin (MVC2_PROFBIN=1) */
#define HIST 500                /* frame times, in half milliseconds */
#define WORST 4
#define TEXT_SIZE 8192

static const char* const names[PZ_COUNT] = {
    [PZ_FRAME] = "frame", [PZ_GAME] = "game", [PZ_PRESENT] = "present", [PZ_LISTS] = "lists",
    [PZ_TEXTURES] = "textures", [PZ_DRAWS] = "draws", [PZ_GPU_WAIT] = "gpu wait", [PZ_VBLANK] = "vblank",
    [PZ_ROM_MISS] = "rom miss", [PZ_ROM_POLL] = "rom poll", [PZ_SND_PREPARE] = "snd prepare",
    [PZ_SND_SYNC] = "snd sync", [PZ_SND_FRAME] = "snd frame", [PZ_SND_ARM] = "snd arm", [PZ_SND_FIQ] = "snd fiq",
    [PZ_SND_IO] = "snd io", [PZ_SND_CHANNELS] = "snd channels", [PZ_SND_MIX] = "snd mix",
    [PZ_SND_DEFER] = "snd defer", [PZ_SND_OUT] = "snd out", [PZ_SND_IDLE] = "snd idle",
    [PZ_SND_SYNC_MID] = "snd sync mid",
    [PZ_ROM_THREAD] = "rom thread",
};

/* running totals, each written by its zone's thread only; a frame's share is the difference */
typedef struct {
    volatile uint32_t ticks, calls, items, pad;
} Zone;
static Zone zones[PZ_COUNT] __attribute__((aligned(32)));

typedef struct {
    uint32_t frame;
    uint32_t ticks[PZ_COUNT], calls[PZ_COUNT], items[PZ_COUNT];
} Rec;

static Rec seen, worst[WORST];
static uint64_t win_ticks[PZ_COUNT], win_calls[PZ_COUNT], win_items[PZ_COUNT];
static uint32_t win_max[PZ_COUNT];
static uint16_t hist[HIST];
static uint32_t frames, last_tick, win_start;

static Rec ring[RING];
static volatile uint32_t ring_head, ring_tail;

static FILE *log_file, *bin_file;
static char text[TEXT_SIZE];
static volatile int text_len, quit;
static uint32_t missed;
static LightEvent wake;
static Thread writer;

uint32_t prof_now(void) {
    return (uint32_t)svcGetSystemTick();
}

void prof_add(int zone, uint32_t ticks) {
    zones[zone].ticks += ticks;
    zones[zone].calls++;
}

void prof_count(int zone, uint32_t n) {
    zones[zone].items += n;
}

static double ms(uint64_t ticks) {
    return (double)ticks * 1000.0 / SYSCLOCK_ARM11;
}

static double percentile(int pct) {
    uint32_t want = (frames - win_start) * pct / 100, sum = 0;
    int i;
    for (i = 0; i < HIST; i++) {
        sum += hist[i];
        if (sum > want)
            break;
    }
    return i * 0.5;
}

static int put(int n, const char* fmt, ...) __attribute__((format(printf, 2, 3)));
static int put(int n, const char* fmt, ...) {
    va_list ap;
    if (n >= TEXT_SIZE)
        return n;
    va_start(ap, fmt);
    n += vsnprintf(text + n, TEXT_SIZE - n, fmt, ap);
    va_end(ap);
    return n < TEXT_SIZE ? n : TEXT_SIZE;
}

static const char* name(int i, char* buf) {
    if (names[i] != NULL)
        return names[i];
    sprintf(buf, "zone %d", i);
    return buf;
}

static void write_out(void);
#ifdef MVC2_PROF_FUNCS
static void write_places(FILE* f);
#endif

static void summary(void) {
    uint32_t n = frames - win_start;
    char buf[16];
    int i, w, len = 0;
    if (n == 0 || log_file == NULL || win_ticks[PZ_FRAME] == 0)
        return;
    if (text_len != 0) {
        missed++;
        return;
    }
    len = put(len, "frames %lu-%lu: %.1f a second; frame p50 %.1f p90 %.1f p99 %.1f max %.1f ms\n",
              (unsigned long)win_start + 1, (unsigned long)frames, n * 1000.0 / ms(win_ticks[PZ_FRAME]),
              percentile(50), percentile(90), percentile(99), ms(win_max[PZ_FRAME]));
    len = put(len, "  %-14s %9s %9s %9s %9s\n", "zone", "ms/frame", "max ms", "calls", "items");
    for (i = 0; i < PZ_COUNT; i++) {
        if (win_calls[i] == 0 && win_items[i] == 0)
            continue;
        len = put(len, "  %-14s %9.2f %9.2f %9.1f %9.1f\n", name(i, buf), ms(win_ticks[i]) / n, ms(win_max[i]),
                  (double)win_calls[i] / n, (double)win_items[i] / n);
    }
    for (w = 0; w < WORST && worst[w].frame != 0; w++) {
        len = put(len, "  worst %lu: %.1f ms:", (unsigned long)worst[w].frame, ms(worst[w].ticks[PZ_FRAME]));
        for (i = 1; i < PZ_COUNT; i++)
            if (worst[w].ticks[i] * 50u >= worst[w].ticks[PZ_FRAME])     /* 2% of the frame or more */
                len = put(len, " %s %.1f (%lu)", name(i, buf), ms(worst[w].ticks[i]), (unsigned long)worst[w].calls[i]);
        len = put(len, "\n");
    }
    if (missed != 0)
        len = put(len, "  (%lu summaries not written: the SD card was busy)\n", (unsigned long)missed);
    __sync_synchronize();
    text_len = len;
    if (writer != NULL)
        LightEvent_Signal(&wake);
    else
        write_out();
}

void prof_frame(void) {
    uint32_t now = prof_now(), next;
    Rec r;
    int i, w;
    if (last_tick != 0)
        prof_add(PZ_FRAME, now - last_tick);
    last_tick = now;
    r.frame = ++frames;
    for (i = 0; i < PZ_COUNT; i++) {
        uint32_t t = zones[i].ticks, c = zones[i].calls, it = zones[i].items;
        r.ticks[i] = t - seen.ticks[i];
        r.calls[i] = c - seen.calls[i];
        r.items[i] = it - seen.items[i];
        seen.ticks[i] = t;
        seen.calls[i] = c;
        seen.items[i] = it;
        win_ticks[i] += r.ticks[i];
        win_calls[i] += r.calls[i];
        win_items[i] += r.items[i];
        if (r.ticks[i] > win_max[i])
            win_max[i] = r.ticks[i];
    }
    if (r.calls[PZ_FRAME] != 0) {
        uint32_t b = (uint32_t)(ms(r.ticks[PZ_FRAME]) * 2.0);
        hist[b < HIST ? b : HIST - 1]++;
        for (w = 0; w < WORST; w++) {
            if (worst[w].frame == 0 || r.ticks[PZ_FRAME] > worst[w].ticks[PZ_FRAME]) {
                memmove(&worst[w + 1], &worst[w], (WORST - 1 - w) * sizeof(Rec));
                worst[w] = r;
                break;
            }
        }
    }
    if (bin_file != NULL) {
        next = (ring_head + 1) % RING;
        if (next != ring_tail) {        /* full: the frame is left out */
            ring[ring_head] = r;
            __sync_synchronize();
            ring_head = next;
        }
        if (frames % 16 == 0)
            LightEvent_Signal(&wake);
    }
    if (frames % WINDOW == 0) {
        summary();
        memset(win_ticks, 0, sizeof win_ticks);
        memset(win_calls, 0, sizeof win_calls);
        memset(win_items, 0, sizeof win_items);
        memset(win_max, 0, sizeof win_max);
        memset(hist, 0, sizeof hist);
        memset(worst, 0, sizeof worst);
        win_start = frames;
    }
}

static void write_out(void) {
    int len = text_len;
    if (len != 0) {
        fwrite(text, 1, len, log_file);
#ifdef MVC2_PROF_FUNCS
        write_places(log_file);
#endif
        fflush(log_file);
        __sync_synchronize();
        text_len = 0;
    }
    if (bin_file != NULL) {
        uint32_t head = ring_head, tail = ring_tail;
        if (head < tail) {
            fwrite(&ring[tail], sizeof(Rec), RING - tail, bin_file);
            tail = 0;
        }
        if (tail < head)
            fwrite(&ring[tail], sizeof(Rec), head - tail, bin_file);
        __sync_synchronize();
        ring_tail = head;
    }
}

/* the SD card's writes on a thread of their own: it only waits for them, so the game runs meanwhile */
static void writer_main(void* arg) {
    (void)arg;
    while (!quit) {
        LightEvent_Wait(&wake);
        write_out();
    }
    write_out();
}

/* the calibration: a loop of known length on each core, the tick's own cost, and the memory's latency */
static uint32_t spin(uint32_t n) {
    uint32_t t0 = prof_now();
    __asm__ volatile("1: subs %0, %0, #1\n\tbne 1b" : "+r"(n) : : "cc");
    return prof_now() - t0;
}

static volatile uint32_t core1_ticks;
static void spin_core1(void* arg) {
    core1_ticks = spin((uint32_t)arg);
}

static double chase(uint32_t size, uint32_t steps) {
    uint32_t lines = size / 32, i, seed = 12345, t0;
    uint32_t* buf = malloc(size);
    void** p;
    if (buf == NULL)
        return 0.0;
    for (i = 0; i < lines; i++)
        buf[i * 8] = i;
    for (i = lines - 1; i > 0; i--) {   /* one cycle through every line, in a random order */
        uint32_t j, t;
        seed = seed * 1664525u + 1013904223u;
        j = (seed >> 8) % i;
        t = buf[i * 8];
        buf[i * 8] = buf[j * 8];
        buf[j * 8] = t;
    }
    for (i = 0; i < lines; i++)
        buf[i * 8] = (uint32_t)&buf[buf[i * 8] * 8];
    p = (void**)buf;
    for (i = 0; i < lines; i++)
        p = *p;
    t0 = prof_now();
    for (i = 0; i < steps; i++)
        p = *p;
    t0 = prof_now() - t0;
    __asm__ volatile("" : : "r"(p));
    free(buf);
    return ms(t0) * 1e6 / steps;
}

static void calibrate(void) {
    const uint32_t loops = 1u << 22;
    uint32_t t0, t1, i;
    Thread th;
    t0 = spin(loops);
    th = threadCreate(spin_core1, (void*)loops, 4096, 0x18, 1, false);
    if (th != NULL) {
        threadJoin(th, U64_MAX);
        threadFree(th);
    }
    fprintf(log_file, "calibration: a loop of %lu on core 0 %.2f ms (%.2f ticks a pass)", (unsigned long)loops,
            ms(t0), (double)t0 / loops);
    if (th != NULL)
        fprintf(log_file, ", on core 1 %.2f ms: core 1 gives %.0f%%\n", ms(core1_ticks), 100.0 * t0 / core1_ticks);
    else
        fprintf(log_file, ", no thread on core 1\n");
    t1 = prof_now();
    for (i = 0; i < 10000; i++)
        (void)prof_now();
    fprintf(log_file, "calibration: a tick read %.2f us\n", ms(prof_now() - t1) * 1000.0 / 10000);
    fprintf(log_file, "calibration: memory, ns a read: 16 KB %.1f, 64 KB %.1f, 256 KB %.1f, 2 MB %.1f\n",
            chase(16 << 10, 1 << 20), chase(64 << 10, 1 << 20), chase(256 << 10, 1 << 20), chase(2 << 20, 1 << 20));
    fflush(log_file);
}

#ifdef MVC2_PROF_FUNCS
/* with -finstrument-functions: the game thread's place, a function entered or the caller returned to, sampled
   every millisecond; the addresses are named on the PC from the ELF */
#define SLOTS 8192u
static volatile uint32_t place;
static void* game_tls;
static uint32_t slot_pc[SLOTS], slot_n[SLOTS], samples, dropped, used;
static Thread sampler;

__attribute__((no_instrument_function)) void __cyg_profile_func_enter(void* fn, void* site) {
    (void)site;
    if (getThreadLocalStorage() == game_tls)
        place = (uint32_t)fn;
}

__attribute__((no_instrument_function)) void __cyg_profile_func_exit(void* fn, void* site) {
    (void)fn;
    if (getThreadLocalStorage() == game_tls)
        place = (uint32_t)site;
}

static void sampler_main(void* arg) {
    (void)arg;
    while (!quit) {
        uint32_t pc = place, h = ((pc >> 1) * 2654435761u) >> 19;
        svcSleepThread(1000000);
        if (pc == 0)
            continue;
        while (slot_pc[h] != pc && slot_pc[h] != 0)
            h = (h + 1) & (SLOTS - 1);
        if (slot_pc[h] == 0) {
            if (used >= SLOTS * 3 / 4) {
                dropped++;
                continue;
            }
            slot_pc[h] = pc;
            used++;
        }
        slot_n[h]++;
        samples++;
    }
}

/* the busiest places so far, on the writer's thread */
static void write_places(FILE* f) {
    static uint8_t taken[SLOTS];
    uint32_t total = samples, k, i;
    memset(taken, 0, sizeof taken);
    fprintf(f, "places: %lu samples, %lu places, %lu not kept\n", (unsigned long)total, (unsigned long)used,
            (unsigned long)dropped);
    for (k = 0; k < 60 && total != 0; k++) {
        uint32_t best = SLOTS;
        for (i = 0; i < SLOTS; i++)
            if (!taken[i] && slot_n[i] != 0 && (best == SLOTS || slot_n[i] > slot_n[best]))
                best = i;
        if (best == SLOTS)
            break;
        taken[best] = 1;
        fprintf(f, "  %08lx %6lu %5.1f%%\n", (unsigned long)slot_pc[best], (unsigned long)slot_n[best],
                100.0 * slot_n[best] / total);
    }
}
#endif

void prof_start(void) {
    s32 prio = 0x30;
    const char* bin = getenv("MVC2_PROFBIN");
    log_file = fopen(CTR_HOME "/" CTR_PROF ".log", "w");
    if (log_file == NULL)
        return;
    calibrate();
    if (bin != NULL && bin[0] == '1') {
        bin_file = fopen(CTR_HOME "/" CTR_PROF ".bin", "wb");
        if (bin_file != NULL) {
            uint32_t head[3] = {0x3150564Du, PZ_COUNT, SYSCLOCK_ARM11};    /* "MVP1" */
            fwrite(head, sizeof head, 1, bin_file);
        }
    }
    LightEvent_Init(&wake, RESET_ONESHOT);
    svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);
    writer = threadCreate(writer_main, NULL, 16 << 10, prio > 0x18 ? prio - 1 : prio, 0, false);
    if (writer == NULL)
        fprintf(log_file, "no writer thread: summaries are written by the game\n");
#ifdef MVC2_PROF_FUNCS
    game_tls = getThreadLocalStorage();
    sampler = threadCreate(sampler_main, NULL, 4096, prio > 0x19 ? prio - 2 : prio, 0, false);
#endif
}

void prof_stop(void) {
    summary();
    if (writer != NULL) {
        quit = 1;
        LightEvent_Signal(&wake);
        threadJoin(writer, U64_MAX);
        threadFree(writer);
        writer = NULL;
    } else if (log_file != NULL) {
        write_out();
    }
#ifdef MVC2_PROF_FUNCS
    if (sampler != NULL) {
        threadJoin(sampler, U64_MAX);
        threadFree(sampler);
        sampler = NULL;
    }
#endif
    if (bin_file != NULL)
        fclose(bin_file);
    if (log_file != NULL)
        fclose(log_file);
    bin_file = log_file = NULL;
}

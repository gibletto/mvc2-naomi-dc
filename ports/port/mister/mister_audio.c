/*
 * mister_audio.c  The sound to /dev/MrAudio, the MiSTer's ring in DDR3 that the core's ALSA mixer plays at 48 kHz:
 * each frame's 44.1 kHz samples resampled, stretched by up to 0.5% to keep about three frames queued by the
 * FPGA's own count (a read of the device says how many bytes it has left).
 *
 * Each frame's sound is made on a thread of its own (the second core) while the game makes the next frame
 * (MVC2_SNDTHREAD=0: on the game's thread): a sound call from the game first waits for it, and the calls the game
 * makes at a frame's start are kept and run after it, in their order, as on the PSP.
 */

#define _GNU_SOURCE
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/syscall.h>
#include <time.h>
#include <unistd.h>

#include "mister_front.h"
#include "naomi_host.h"
#include "snd.h"

#define OUT_RATE 48000
#define TARGET 3.0                  /* frames kept queued */
#define FRAME_BYTES (OUT_RATE * 4 * snd_base_us() / 1e6)    /* a frame's, at the game's rate */

static int dev = -1;
static int64_t pos;                 /* in 2^-32 of an input sample */
static int16_t in_last[2];
static double level = -1;
static int dry, dropped, played;
static FILE* wav;                   /* MVC2_WAV: the frames' sound before the stretch */
static uint32_t wav_samples;
uint32_t mister_audio_queued, mister_audio_dry, mister_audio_dropped;
uint32_t mister_audio_make_us, mister_audio_wait_us;

static int threaded;
static pthread_t maker;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t changed = PTHREAD_COND_INITIALIZER;
static int go, busy, quit;
static struct {
    SND_WORK work;
    int a, b;
} pending[32];                      /* the game's calls while a frame is being made, run after it */
static int pending_n;

static void put32(uint8_t* p, uint32_t v) {
    p[0] = (uint8_t)v, p[1] = (uint8_t)(v >> 8), p[2] = (uint8_t)(v >> 16), p[3] = (uint8_t)(v >> 24);
}

static void wav_header(void) {
    uint8_t h[44] = { 'R', 'I', 'F', 'F', 0, 0, 0, 0, 'W', 'A', 'V', 'E', 'f', 'm', 't', ' ', 16, 0, 0, 0, 1, 0, 2, 0,
                      0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 16, 0, 'd', 'a', 't', 'a' };
    put32(h + 4, 36 + wav_samples * 4);
    put32(h + 24, SND_RATE);
    put32(h + 28, SND_RATE * 4);
    put32(h + 40, wav_samples * 4);
    fseek(wav, 0, SEEK_SET);
    fwrite(h, 1, 44, wav);
    fseek(wav, 0, SEEK_END);
}

static unsigned read_at;    /* the core's read pointer */

/* the bytes the core has still to play, read from a new open as Pinedra's SDL does */
static int queued(void) {
    char text[128];
    unsigned rptr, wptr, len;
    ssize_t n;
    int fd = open("/dev/MrAudio", O_RDONLY | O_CLOEXEC);
    if (fd < 0) {
        return -1;
    }
    n = read(fd, text, sizeof text - 1);
    close(fd);
    if (n <= 0) {
        return -1;
    }
    text[n] = 0;
    if (sscanf(text, "rptr: %u, wptr: %u, len: %u", &rptr, &wptr, &len) != 3) {
        return -1;
    }
    read_at = rptr;
    return (int)len;
}

static void put(const int16_t* s, int n) {
    const char* p = (const char*)s;
    size_t left = (size_t)n * 4;
    while (left > 0) {
        ssize_t w = write(dev, p, left);
        if (w <= 0) {
            return;
        }
        p += w, left -= (size_t)w;
    }
}

static void queue(const int16_t* in, int n, double stretch) {
    static int16_t out[2 * 2048];
    int64_t step = (int64_t)(4294967296.0 * SND_RATE / (OUT_RATE * (1.0 + stretch)));
    int o = 0;
    while (o < 2048) {
        int i = (int)(pos >> 32), c;
        int32_t f = (int32_t)((uint32_t)pos >> 17);
        if (i >= n) {
            break;
        }
        for (c = 0; c < 2; c++) {
            int32_t a = i == 0 ? in_last[c] : in[(i - 1) * 2 + c], b = in[i * 2 + c];
            out[o * 2 + c] = (int16_t)(a + (((b - a) * f + 16384) >> 15));
        }
        o++;
        pos += step;
    }
    pos -= (int64_t)n << 32;
    in_last[0] = in[(n - 1) * 2];
    in_last[1] = in[(n - 1) * 2 + 1];
    put(out, o);
}

/* a frame's samples saved and played */
static void publish(const int16_t* s, int n) {
    static const int16_t quiet[2 * 2048];
    static unsigned last = ~0u;
    double frames, stretch;
    int q, stuck;
    if (n > 0 && wav != NULL) {
        fwrite(s, 4, (size_t)n, wav);
        wav_samples += (uint32_t)n;
    }
    if (dev < 0 || n <= 0 || (q = queued()) < 0) {
        return;
    }
    mister_audio_queued = (uint32_t)q;
    /* a queue the core doesn't read isn't dropped: a core just loaded starts at the next write it sees */
    stuck = read_at == last;
    last = read_at;
    frames = q / FRAME_BYTES;
    if (frames < 0.25) {
        int fill = (int)((TARGET - 1) * OUT_RATE * snd_base_us() / 1e6);
        put(quiet, fill < 2048 ? fill : 2048);
        level = TARGET - 1;
        dry++;
        mister_audio_dry++;
    } else if (frames > 4 * TARGET && !stuck) {
        level = TARGET;
        dropped++;
        mister_audio_dropped++;
        return;
    }
    level = level < 0 ? frames : level + (frames - level) * 0.05;
    stretch = (TARGET - level) * 0.0025;
    stretch = stretch > 0.005 ? 0.005 : stretch < -0.005 ? -0.005 : stretch;
    queue(s, n, stretch);
    played++;
}

/* the thread's own CPU time, so a core shared with another process doesn't count */
static uint64_t cpu_us(void) {
    struct timespec t;
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &t);
    return (uint64_t)t.tv_sec * 1000000u + (uint64_t)t.tv_nsec / 1000u;
}

static void make(void) {
    const int16_t* s;
    uint64_t t0 = cpu_us();
    int n = snd_frame(&s);
    mister_audio_make_us = (uint32_t)(cpu_us() - t0);
    publish(s, n);
}

static void run_pending(void) {
    int i;
    pthread_mutex_lock(&lock);
    for (i = 0; i < pending_n; i++) {
        pthread_mutex_unlock(&lock);
        pending[i].work(pending[i].a, pending[i].b);
        pthread_mutex_lock(&lock);
    }
    pending_n = 0;
    pthread_mutex_unlock(&lock);
}

static void* make_thread(void* arg) {
    /* MVC2_SNDCPU=N's CPU, MVC2_PIN's, else any: not the game thread's alone, which Pinedra's driver pins to one */
    const char* on = getenv("MVC2_SNDCPU");
    cpu_set_t set;
    long c, n = sysconf(_SC_NPROCESSORS_ONLN);
    (void)arg;
    const char* nice = getenv("MVC2_SNDNICE");
    pthread_setname_np(pthread_self(), "SDLAudio");    /* the name Pinedra's PND_AUDIO_CORE places */
    /* ahead of MiSTer's main program on CPU1, as Pinedra's driver thread is (nice -10): behind it, a frame's 3 ms of
       sound took a whole frame or more while the driver was busy, and the game waited for it */
    setpriority(PRIO_PROCESS, (id_t)syscall(SYS_gettid), nice != NULL ? atoi(nice) : -10);
    CPU_ZERO(&set);
    if (on != NULL) {
        CPU_SET(atoi(on), &set);
        pthread_setaffinity_np(pthread_self(), sizeof set, &set);
    } else if (!mister_pin(1)) {
        for (c = 0; c < n && c < CPU_SETSIZE; c++) {
            CPU_SET(c, &set);
        }
        pthread_setaffinity_np(pthread_self(), sizeof set, &set);
    }
    for (;;) {
        pthread_mutex_lock(&lock);
        while (!go && !quit) {
            pthread_cond_wait(&changed, &lock);
        }
        go = 0;
        pthread_mutex_unlock(&lock);
        if (quit) {
            break;
        }
        make();
        run_pending();
        pthread_mutex_lock(&lock);
        busy = 0;
        pthread_cond_broadcast(&changed);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

/* the frame being made finished, and the calls kept meanwhile run (not from the maker's own calls) */
static void settle(void) {
    uint64_t t0;
    if (!threaded || pthread_equal(pthread_self(), maker)) {
        return;
    }
    pthread_mutex_lock(&lock);
    if (busy) {
        t0 = mister_us();
        while (busy) {
            pthread_cond_wait(&changed, &lock);
        }
        mister_audio_wait_us += (uint32_t)(mister_us() - t0);
    }
    pthread_mutex_unlock(&lock);
}

static int defer(SND_WORK work, int a, int b) {
    int taken = 0;
    pthread_mutex_lock(&lock);
    if (busy && pending_n < 32 && !pthread_equal(pthread_self(), maker)) {
        pending[pending_n].work = work;
        pending[pending_n].a = a;
        pending[pending_n].b = b;
        pending_n++;
        taken = 1;
    }
    pthread_mutex_unlock(&lock);
    return taken;
}

void mister_audio_open(void) {
    const char* w = getenv("MVC2_WAV");
    const char* t = getenv("MVC2_SNDTHREAD");
    if (w != NULL && (wav = fopen(w, "wb")) != NULL) {
        wav_header();
    }
    /* MVC2_MRAUDIO=0: the sound made but not played */
    if (snd_enabled() && (getenv("MVC2_MRAUDIO") == NULL || atoi(getenv("MVC2_MRAUDIO")) != 0)) {
        dev = open("/dev/MrAudio", O_WRONLY | O_CLOEXEC);
        if (dev < 0) {
            host_log("audio: /dev/MrAudio can't be opened");
        } else {
            static const int16_t quiet[2 * 512];
            put(quiet, 512);    /* a period of silence first: what a core just loaded starts from */
        }
    }
    /* the sound made on a thread of its own, which the launcher's PND_AUDIO_CORE=1 keeps off the game's CPU0 (by its
       name; MVC2_SNDTHREAD=0: on the game's thread) */
    if (snd_enabled() && (t == NULL || atoi(t) != 0) &&
        pthread_create(&maker, NULL, make_thread, NULL) == 0) {
        threaded = 1;
    }
    host_log("audio: %s, made %s", dev >= 0 ? "to /dev/MrAudio" : "not played",
             threaded ? "on a thread of its own" : "on the game's thread");
}

static int by_size(const void* a, const void* b) {
    uint32_t x = *(const uint32_t*)a, y = *(const uint32_t*)b;
    return x < y ? -1 : x > y;
}

/* the game's frames a second: paced by the vblank, the refresh the driver's vsync follows (HDMI's, unless MiSTer.ini's
   vsync_adjust makes it the analog output's, mvc2.cfg's rate), measured as the middle half of 64 frames' lengths;
   else the rate */
static double frame_rate(uint64_t gap) {
    static uint32_t gaps[64], shown_us;
    static int n;
    if (!mister_gl_vsynced() || wav != NULL) {
        return mister_rate();
    }
    if (gap > 0 && gap < 100000) {
        gaps[n++] = (uint32_t)gap;
    }
    if (n == 64) {
        uint32_t sum = 0, mid;
        int i;
        qsort(gaps, 64, sizeof *gaps, by_size);
        for (i = 16; i < 48; i++) {
            sum += gaps[i];
        }
        mid = sum / 32;
        shown_us = mid > 1000000 / 70 && mid < 1000000 / 50 ? mid : shown_us;
        n = 0;
    }
    return shown_us != 0 ? 1e6 / shown_us : mister_rate();
}

/* a frame's sound as long as the frame took, up to two, while the game runs under 60 fps: the music keeps its time
   and the queue doesn't run dry (as on the 3DS and the PSP) */
static void frame_length(void) {
    static uint64_t last;
    static int64_t owed;
    static uint32_t more;
    uint64_t now = mister_us();
    int64_t base, room;
    snd_set_rate(frame_rate(last != 0 ? now - last : 0));   /* each frame: a restart puts the sound's statics back */
    base = snd_base_us();
    room = base;
    if (wav != NULL) {
        return;     /* a saved run's sound is the same every run (as on the PSP) */
    }
    if (last != 0) {
        /* no more than the sound's thread makes in 70% of the frame's time: more would hold the game up */
        double cost = (double)mister_audio_make_us / (base + more);
        if (cost > 0) {
            room = (int64_t)(0.7 * (double)(now - last) / cost) - base;
        }
        owed += (int64_t)(now - last) - (base + more);
        owed = owed < -(int64_t)base ? -(int64_t)base : owed > 4 * base ? 4 * base : owed;
        if (level > TARGET + 1.5 && owed > 0) {
            owed = 0;
        }
    }
    last = now;
    room = room < 0 ? 0 : room < base ? room : base;
    more = owed <= 0 ? 0 : owed < room ? (uint32_t)owed : (uint32_t)room;
    snd_set_frame_us(base + more);
}

void mister_audio_frame(void) {
    settle();
    frame_length();
    snd_prepare();
    snd_set_sync(settle);
    snd_set_defer(defer);
    if (!threaded) {
        make();
        return;
    }
    pthread_mutex_lock(&lock);
    busy = go = 1;
    pthread_cond_broadcast(&changed);
    pthread_mutex_unlock(&lock);
}

void mister_audio_idle(void) {
    settle();
}

void mister_audio_close(void) {
    settle();
    snd_set_sync(NULL);
    snd_set_defer(NULL);
    if (threaded) {
        pthread_mutex_lock(&lock);
        quit = 1;
        pthread_cond_broadcast(&changed);
        pthread_mutex_unlock(&lock);
        pthread_join(maker, NULL);
        threaded = 0;
    }
    if (wav != NULL) {
        wav_header();
        fclose(wav);
        wav = NULL;
    }
    if (dev >= 0) {
        host_log("audio: %d frames played, %d ran dry, %d dropped", played, dry, dropped);
        close(dev);
        dev = -1;
    }
}

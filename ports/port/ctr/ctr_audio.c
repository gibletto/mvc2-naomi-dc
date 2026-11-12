/*
 * ctr_audio.c  The 3DS's sound out: each frame's sound made on the second core beside the next frame, so it is the
 * PC's sample for sample, and queued to NDSP about three frames ahead, as on the PC.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <3ds.h>

#include "ctr_audio.h"
#include "naomi_host.h"
#include "snd.h"
#include "prof.h"

#define CHANNEL 0
#define BUFS 8                      /* frames NDSP can hold */
#define BUF_SAMPLES 1024
#define TARGET 3.0                  /* frames kept queued */
#define FRAME_SAMPLES (SND_RATE / 59.94)
#define REPORT 600

static int on, threaded;
static Thread thread;
static LightEvent go, done;         /* go: a frame to make; done: the thread is not making one */
static LightLock lock;              /* done and pending, between the two threads */
static struct {
    SND_WORK work;
    int a, b;
} pending[32];                      /* calls' work for the thread, after the frame */
static int pending_n;
static volatile int quit;
static ndspWaveBuf wave[BUFS];
static int16_t* pcm;                /* the buffers' samples, in linear memory */
static int next_buf;
static double level = -1;           /* the queue, smoothed, in frames */
static unsigned frames, last_dry;

/* for the log, over each REPORT frames */
static u64 make_ticks, wait_ticks;
static int dry, dropped;
static double lo = 99, hi;

static double ms(u64 ticks) {
    return ticks * 1000.0 / SYSCLOCK_ARM11;
}

static double queued_frames(void) {
    double n = 0;
    int i;
    for (i = 0; i < BUFS; i++) {
        if (wave[i].status == NDSP_WBUF_QUEUED || wave[i].status == NDSP_WBUF_PLAYING) {
            n += wave[i].nsamples;
        }
    }
    if (n > 0) {
        n -= ndspChnGetSamplePos(CHANNEL);
    }
    return n / FRAME_SAMPLES;
}

static void put(const int16_t* s, int n) {
    ndspWaveBuf* w = &wave[next_buf];
    if (w->status == NDSP_WBUF_QUEUED || w->status == NDSP_WBUF_PLAYING) {
        dropped++;
        return;
    }
    n = n < BUF_SAMPLES ? n : BUF_SAMPLES;
    if (s != NULL) {
        memcpy(w->data_pcm16, s, (size_t)n * 4);
    } else {
        memset(w->data_pcm16, 0, (size_t)n * 4);
    }
    w->nsamples = (u32)n;
    DSP_FlushDataCache(w->data_pcm16, (u32)n * 4);
    ndspChnWaveBufAdd(CHANNEL, w);
    next_buf = (next_buf + 1) % BUFS;
}

/* the frame's samples to NDSP, at a rate that holds the queue near TARGET */
static void output(const int16_t* s, int n) {
    double q = queued_frames(), stretch;
    if (q < 0.25) {
        /* ran dry: after a stall, silence ahead to fill the queue again; when the game keeps falling behind,
           only the frame (more silence would only cut its sound up) */
        if (frames - last_dry > 120) {
            put(NULL, (int)FRAME_SAMPLES);
            put(NULL, (int)FRAME_SAMPLES);
        }
        last_dry = frames;
        q = level = queued_frames();
        dry++;
    } else if (q > BUFS - 2) {
        level = TARGET;
        dropped++;
        return;
    }
    lo = q < lo ? q : lo;
    hi = q > hi ? q : hi;
    level = level < 0 ? q : level + (q - level) * 0.05;
    stretch = (TARGET - level) * 0.0025;
    stretch = stretch > 0.005 ? 0.005 : stretch < -0.005 ? -0.005 : stretch;
    ndspChnSetRate(CHANNEL, (float)(SND_RATE * (1.0 + stretch)));
    put(s, n);
}

static void make(void) {
    const int16_t* s;
    u64 t0 = svcGetSystemTick();
    PROF_START(f);
    int n = snd_frame(&s);
    PROF_STOP(PZ_SND_FRAME, f);
    make_ticks += svcGetSystemTick() - t0;
    if (n > 0) {
        PROF_START(t);
        output(s, n);
        PROF_STOP(PZ_SND_OUT, t);
    }
}

static void sound_thread(void* arg) {
    int i;
    (void)arg;
    for (;;) {
        PROF_START(idle);
        LightEvent_Wait(&go);
        PROF_STOP(PZ_SND_IDLE, idle);
        if (quit) {
            break;
        }
        make();
        PROF_START(t);
        LightLock_Lock(&lock);
        for (i = 0; i < pending_n; i++) {
            LightLock_Unlock(&lock);
            pending[i].work(pending[i].a, pending[i].b);
            LightLock_Lock(&lock);
        }
        PROF_COUNT(PZ_SND_DEFER, (uint32_t)pending_n);
        pending_n = 0;
        PROF_STOP(PZ_SND_DEFER, t);
        LightEvent_Signal(&done);
        LightLock_Unlock(&lock);
    }
}

static void wait_done(int zone) {
    u64 t0;
    if (threadGetCurrent() == thread || LightEvent_TryWait(&done)) {
        return;     /* the thread's own calls (pending work) wait for nothing */
    }
    t0 = svcGetSystemTick();
    PROF_START(t);
    LightEvent_Wait(&done);
    PROF_STOP(zone, t);
    wait_ticks += svcGetSystemTick() - t0;
    PROF_COUNT(zone, 1);
}

static void wait_mid(void) {
    wait_done(PZ_SND_SYNC_MID);
}

static int defer(SND_WORK work, int a, int b) {
    int taken = 0;
    LightLock_Lock(&lock);
    if (!LightEvent_TryWait(&done) && pending_n < 32) {
        pending[pending_n].work = work;
        pending[pending_n].a = a;
        pending[pending_n].b = b;
        pending_n++;
        taken = 1;
    }
    LightLock_Unlock(&lock);
    return taken;
}

static void report(void) {
    if (++frames % REPORT != 0) {
        return;
    }
    host_log("audio: %.2f ms a frame (%.0f%% of one) on core %d, the game waited %.2f ms a frame; queue %.2f to %.2f "
             "frames, %d ran dry, %d dropped", ms(make_ticks) / REPORT, ms(make_ticks) / REPORT * 5.994,
             threaded ? 1 : 0, ms(wait_ticks) / REPORT, lo, hi, dry, dropped);
    make_ticks = wait_ticks = 0;
    dry = dropped = 0;
    lo = 99;
    hi = 0;
}

void ctr_audio_init(void) {
    float mix[12] = { 1.0f, 1.0f };
    s32 prio = 0x30;
    int core = getenv("MVC2_SNDCORE") != NULL ? atoi(getenv("MVC2_SNDCORE")) : 1;
    int i;
    if (on || !snd_enabled()) {
        return;
    }
    /* the whole model (MVC2_DSP=1, MVC2_ARMRATE=64) is too slow for an old 3DS; MVC2_REVERB=1 is the cheap
       reverb */
    setenv("MVC2_DSP", "0", 0);
    setenv("MVC2_ARMRATE", "16", 0);
    if (R_FAILED(ndspInit())) {
        host_log("audio: no NDSP (on a 3DS it needs sdmc:/3ds/dspfirm.cdc)");
        return;
    }
    pcm = linearAlloc(BUFS * BUF_SAMPLES * 4);
    if (pcm == NULL) {
        ndspExit();
        return;
    }
    ndspSetOutputMode(NDSP_OUTPUT_STEREO);
    ndspChnReset(CHANNEL);
    ndspChnSetInterp(CHANNEL, NDSP_INTERP_LINEAR);
    ndspChnSetRate(CHANNEL, SND_RATE);
    ndspChnSetFormat(CHANNEL, NDSP_FORMAT_STEREO_PCM16);
    ndspChnSetMix(CHANNEL, mix);
    memset(wave, 0, sizeof wave);
    for (i = 0; i < BUFS; i++) {
        wave[i].data_pcm16 = pcm + i * BUF_SAMPLES * 2;
        wave[i].status = NDSP_WBUF_DONE;
    }
    on = 1;
    LightLock_Init(&lock);
    LightEvent_Init(&go, RESET_ONESHOT);
    LightEvent_Init(&done, RESET_STICKY);
    LightEvent_Signal(&done);
    svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);
    if (core == 1) {
        APT_SetAppCpuTimeLimit(80);
        thread = threadCreate(sound_thread, NULL, 64 * 1024, prio - 1, 1, false);
    }
    threaded = thread != NULL;
    host_log("audio: NDSP, the sound made on %s", threaded ? "core 1" : "the game's core");
}

void ctr_audio_frame(void) {
    if (!on) {
        return;
    }
    if (threaded) {
        wait_done(PZ_SND_SYNC);
        snd_prepare();
        report();
        snd_set_sync(wait_mid);    /* again after a switch, which puts the sound's state back */
        snd_set_defer(defer);
        LightEvent_Clear(&done);
        LightEvent_Signal(&go);
    } else {
        make();
        report();
    }
}

void ctr_audio_idle(void) {
    if (threaded) {
        wait_done(PZ_SND_SYNC);
    }
}

void ctr_audio_exit(void) {
    if (!on) {
        return;
    }
    if (threaded) {
        wait_done(PZ_SND_SYNC);
        quit = 1;
        LightEvent_Signal(&go);
        threadJoin(thread, U64_MAX);
        threadFree(thread);
        thread = NULL;
        threaded = 0;
        snd_set_sync(NULL);
        snd_set_defer(NULL);
    }
    snd_close();
    ndspChnReset(CHANNEL);
    ndspExit();
    linearFree(pcm);
    pcm = NULL;
    on = 0;
}

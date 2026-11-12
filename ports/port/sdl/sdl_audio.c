/*
 * sdl_audio.c  The PC's sound out: each frame's samples queued to the device, about three frames ahead, stretched
 * by up to 0.5% to hold it there against the drift between the frames' clock and the device's.
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sdl_front.h"

#include "snd.h"

#define CHANNELS 2
#define TARGET 3.0                  /* frames kept queued */

#ifdef MVC2_SDL3
static SDL_AudioStream* dev;        /* converts to the device's own rate */
#else
static SDL_AudioDeviceID dev;
#endif
static int dev_rate;
static double pos;                  /* in the input, between in_last and the next */
static int16_t in_last[2];
static double level = -1;           /* the queue, smoothed, in frames */
static int dropped, dry, waited, played;  /* MVC2_SNDDBG: counted, and shown at the end */
static double lo = 99, hi;          /* the queue's least and most, in frames, before each frame's samples */
static FILE* wav;                   /* MVC2_WAV: every frame's sound, the same in a test run as when played */
static uint32_t wav_samples;

static void put32(uint8_t* p, uint32_t v) {
    p[0] = (uint8_t)v;
    p[1] = (uint8_t)(v >> 8);
    p[2] = (uint8_t)(v >> 16);
    p[3] = (uint8_t)(v >> 24);
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

static void wav_write(const int16_t* s, int n) {
    static int tried;
    if (!tried) {
        tried = 1;
        if (getenv("MVC2_WAV") != NULL && (wav = fopen(getenv("MVC2_WAV"), "wb")) != NULL) {
            wav_header();
        }
    }
    if (wav != NULL) {
        fwrite(s, 4, (size_t)n, wav);
        wav_samples += (uint32_t)n;
    }
}

void sdl_audio_open(void) {
    SDL_AudioSpec want;
    if (!snd_enabled() || !SDL_SUCCEEDED(SDL_InitSubSystem(SDL_INIT_AUDIO))) {
        return;
    }
    memset(&want, 0, sizeof want);
    want.freq = SND_RATE;
    want.channels = CHANNELS;
#ifdef MVC2_SDL3
    want.format = SDL_AUDIO_S16;
    SDL_SetHint(SDL_HINT_AUDIO_DEVICE_SAMPLE_FRAMES, "512");
    dev = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &want, NULL, NULL);
    if (dev == NULL) {
        return;
    }
    dev_rate = SND_RATE;
    SDL_ResumeAudioStreamDevice(dev);
#else
    SDL_AudioSpec have;
    want.format = AUDIO_S16SYS;
    want.samples = 512;
    dev = SDL_OpenAudioDevice(NULL, 0, &want, &have, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);
    if (dev == 0) {
        return;
    }
    dev_rate = have.freq;
    SDL_PauseAudioDevice(dev, 0);
#endif
}

void sdl_audio_close(void) {
    if (wav != NULL) {
        wav_header();
        fclose(wav);
        wav = NULL;
    }
    if (dev) {
        if (getenv("MVC2_SNDDBG") != NULL) {
            fprintf(stderr, "audio: %d frames played at %d Hz, %d dropped, %d ran dry, %d waited, queue %.2f to %.2f "
                    "frames (%.2f at the end)\n", played, dev_rate, dropped, dry, waited, lo, hi, level);
        }
#ifdef MVC2_SDL3
        SDL_DestroyAudioStream(dev);
        dev = NULL;
#else
        SDL_CloseAudioDevice(dev);
        dev = 0;
#endif
    }
}

static void put(const int16_t* s, int n) {
#ifdef MVC2_SDL3
    SDL_PutAudioStreamData(dev, s, n * CHANNELS * 2);
#else
    SDL_QueueAudio(dev, s, (Uint32)(n * CHANNELS * 2));
#endif
}

static double queued_frames(void) {
#ifdef MVC2_SDL3
    double bytes = SDL_GetAudioStreamQueued(dev);
#else
    double bytes = SDL_GetQueuedAudioSize(dev);
#endif
    return bytes / (CHANNELS * 2.0) / (dev_rate / 59.94);
}

/* the frame's samples, at the device's rate times (1 + stretch) */
static void queue(const int16_t* in, int n, double stretch) {
    static int16_t out[2 * 4096];
    double step = (double)SND_RATE / (dev_rate * (1.0 + stretch));
    int o = 0;
    while (o < 4096) {
        int i = (int)pos;
        double f = pos - i;
        int16_t a[2], b[2];
        int c;
        if (i >= n) {
            break;
        }
        for (c = 0; c < CHANNELS; c++) {
            a[c] = i == 0 ? in_last[c] : in[(i - 1) * CHANNELS + c];
            b[c] = in[i * CHANNELS + c];
            out[o * CHANNELS + c] = (int16_t)lrint(a[c] + (b[c] - a[c]) * f);
        }
        o++;
        pos += step;
    }
    pos -= n;
    in_last[0] = in[(n - 1) * CHANNELS];
    in_last[1] = in[(n - 1) * CHANNELS + 1];
    put(out, o);
}

/* frames run faster than 59.94 (MVC2_FPS) wait for the device while the queue is 2.5 frames past its mark, but
   only until the frame has taken a frame's time, so a frame never waits twice */
static double backstop(double frames) {
    static Uint64 last;
    Uint64 freq = SDL_GetPerformanceFrequency(), now = SDL_GetPerformanceCounter();
    Uint64 until = last + (Uint64)(freq / 59.94);
    if (last != 0 && now < until && frames > TARGET + 2.5) {
        waited++;
        while (now < until && (frames = queued_frames()) > TARGET + 2.5) {
            SDL_Delay(1);
            now = SDL_GetPerformanceCounter();
        }
    }
    last = now;
    return frames;
}

void sdl_audio_frame(void) {
    static const int16_t quiet[2 * 4096];
    const int16_t* s;
    int n = snd_frame(&s);
    double frames, stretch;
    if (n > 0 && snd_enabled()) {
        wav_write(s, n);
    }
    if (!dev || n <= 0) {
        return;
    }
    frames = backstop(queued_frames());
    if (frames < 0.25) {
        /* ran dry: silence ahead to fill the queue again, then this frame */
        int q = (int)((TARGET - 1) * dev_rate / 59.94);
        put(quiet, q < 4096 ? q : 4096);
        frames = level = queued_frames();
        if (dry++ > 0 && getenv("MVC2_SNDDBG") != NULL) {
            fprintf(stderr, "audio: ran dry after %d frames\n", played);
        }
    } else if (frames > 4 * TARGET) {
        level = TARGET;
        dropped++;
        return;     /* far too far ahead: this one is dropped */
    }
    if (played >= 120) {    /* past the start */
        lo = frames < lo ? frames : lo;
        hi = frames > hi ? frames : hi;
    }
    level = level < 0 ? frames : level + (frames - level) * 0.05;
    stretch = (TARGET - level) * 0.0025;
    stretch = stretch > 0.005 ? 0.005 : stretch < -0.005 ? -0.005 : stretch;
    queue(s, n, stretch);
    played++;
}

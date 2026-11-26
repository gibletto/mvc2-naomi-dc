/*
 * main_sdl.c  The PC front end on SDL3 (with the GPU renderer) or SDL2: the window, the frame loop, input
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "frame_log.h"
#include "sdl_front.h"
#ifdef MVC2_SDL3
#include <SDL3/SDL_main.h>
#endif

#include "naomi_host.h"
#include "naomi_input.h"
#include "rd.h"
#ifdef MVC2_SDL3
#include "rd_gpu.h"
#include "gl_sdl.h"
#include "rd_gl1.h"
#endif
#include "input_sdl.h"
#include "menu_sdl.h"
#include "test_mode.h"
#include "hunt.h"
#ifdef _WIN32
#include "console_win32.h"
#include "display_win32.h"
void config_read(void);             /* start.c */
#endif

void sdl_audio_open(void);         /* sdl_audio.c */
void sdl_audio_frame(void);
void sdl_audio_close(void);
void snd_close(void);

static SDL_Window* window;
static SDL_Renderer* renderer;
static SDL_Texture* screen;

#ifdef MVC2_SDL3
static SDL_GPUDevice* gpu;
static SDL_Texture* gpu_screen;
static int glr;             /* the OpenGL renderer (gl_sdl.c), where SDL_GPU has neither Vulkan nor Direct3D 12 */
#define gl_uploads() (glr ? rd_gl1_stats.uploads : 0)
#define gl_upload_kb() (glr ? rd_gl1_stats.upload_bytes >> 10 : 0)
#else
#define gpu 0
#define glr 0
#define gl_uploads() 0
#define gl_upload_kb() 0u
#endif
static HOST_INPUT input;

const HOST_INPUT* host_input(void) {
    return &input;
}

static unsigned presented;
static int switch_req;      /* F9 */
static int coin_req;
static int test_run;        /* MVC2_FRAMES: the press list is the only input */
static int console_read = 1;    /* stderr outlives the game: a file, a pipe or the terminal it was started from */

/* MVC2_PRESS=n:key,...: player 1 holds the key for 5 frames from frame n (p2key: player 2); coin and switch
   happen at n */
static uint32_t pressed(int side) {
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
        if (sscanf(p, "%u:%15[a-z0-9]", &at, name) == 2 && presented == at && strcmp(name, "coin") == 0) {
            coin_req = 1;
        }
        if (sscanf(p, "%u:%15[a-z0-9]", &at, name) == 2 && presented >= at && presented < at + 5) {
            const char* key = side == 0 ? name : name[0] == 'p' && name[1] == '2' ? name + 2 : "";
            for (i = 0; i < sizeof keys / sizeof keys[0]; i++) {
                if (strcmp(key, keys[i].name) == 0) {
                    bits |= keys[i].bit;
                }
            }
        }
        p = strchr(p, ',');
        p = p != NULL ? p + 1 : NULL;
    }
    return bits;
}

static int hunting, hunt_done;

static void read_input(void) {
    if (hunting) {
        hunt_done = hunt_frame(&input, presented);
        input.test = 0;
        return;
    }
    input.player[0] = pressed(0);
    input.player[1] = pressed(1);
    input.coin = 0;
    input.test = 0;
    if (!test_run) {
        input_read(&input);
        if (key_held(SDL_SCANCODE_9)) {
            input.player[0] |= JVS_SERVICE;
        }
    }
    if (coin_req) {
        input.coin = 1;
        coin_req = 0;
    }
}

static uint32_t frame[RD_WIDTH * RD_HEIGHT];

static void put32(unsigned char* p, uint32_t v) {
    p[0] = v & 0xFF, p[1] = v >> 8 & 0xFF, p[2] = v >> 16 & 0xFF, p[3] = v >> 24;
}

/* SDL2's BMP header (BITMAPV4HEADER), so shots match whichever SDL is used */
static void save_bmp(const char* name, const uint32_t* argb, int w, int h) {
    unsigned char hd[122];
    uint32_t* row = malloc((size_t)w * 4);
    FILE* f = row != NULL ? fopen(name, "wb") : NULL;
    int y, x;
    if (f == NULL) {
        free(row);
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
    put32(hd + 70, 0x57696E20);             /* "Win " */
    fwrite(hd, sizeof hd, 1, f);
    for (y = h - 1; y >= 0; y--) {
        for (x = 0; x < w; x++) {
            put32((unsigned char*)&row[x], argb[(size_t)y * w + x]);
        }
        fwrite(row, (size_t)w * 4, 1, f);
    }
    fclose(f);
    free(row);
}

/* the GPU's frame read back, at most once a frame */
#ifdef MVC2_SDL3
static int frame_read;
static char window_shot[32];    /* a shot show() takes from the window this frame */
#endif

static void read_frame(void) {
#ifdef MVC2_SDL3
    if (gpu && !frame_read) {
        frame_read = rd_gpu_read(frame);
    }
    if (glr && !frame_read) {
        gl_read(renderer, frame);
        frame_read = 1;
    }
#endif
}

/* MVC2_SHOT=n,m,...: frames saved as shot_n.bmp */
static void screenshot(void) {
    const char* list = getenv("MVC2_SHOT");
    char want[16], name[32];
    if (list == NULL) {
        return;
    }
    snprintf(want, sizeof want, "%u", presented);
    for (;;) {
        size_t n = strcspn(list, ",");
        if (n == strlen(want) && strncmp(list, want, n) == 0) {
            break;
        }
        if (list[n] == 0) {
            return;
        }
        list += n + 1;
    }
    snprintf(name, sizeof name, "shot_%u.bmp", presented);
#ifdef MVC2_SDL3
    /* MVC2_SHOTVIEW=1: the picture as the window shows it */
    if (gpu && getenv("MVC2_SHOTVIEW") != NULL) {
        int w, h;
        uint32_t* big;
        rd_gpu_frame_size(&w, &h);
        big = malloc((size_t)w * h * 4);
        if (big != NULL && rd_gpu_read_view(big)) {
            save_bmp(name, big, w, h);
        }
        free(big);
        return;
    }
    if (gpu) {
        snprintf(window_shot, sizeof window_shot, "%s", name);
        return;
    }
#endif
    read_frame();
    save_bmp(name, frame, RD_WIDTH, RD_HEIGHT);
}

/* both boards: a 27 MHz pixel clock, 858 by 525 */
#define BOARD_RATE (27000000.0 / (858.0 * 525.0))
static double frame_rate = BOARD_RATE;
static double next_at;      /* the next frame's time, in performance counter ticks */
static double sync_step;    /* exclusive full screen's (sync_judge): ticks a frame, 0 for frame_rate's */
static int sync_keep;       /* after a stall, the frames kept on the display's refreshes */
#ifdef MVC2_SDL3
static void sync_snap(void);
#endif

static void pace_init(void) {
    const char* fps = getenv("MVC2_FPS");
    if (fps != NULL) {
        frame_rate = atof(fps);
    } else if (getenv("MVC2_FRAMES") != NULL) {
        frame_rate = 0;
    }
}

/* sleep most of the way, then spin the last 2 ms */
static void pace(void) {
    double freq = (double)SDL_GetPerformanceFrequency(), step, now;
    if (frame_rate <= 0) {
        return;
    }
    step = sync_step > 0 ? sync_step : freq / frame_rate;
    now = (double)SDL_GetPerformanceCounter();
    if (next_at == 0 || (now > next_at + step * 4 && !sync_keep)) {
        next_at = now;      /* after a stall, don't catch up */
    } else if (now > next_at + step * 4) {
        next_at += floor((now - next_at) / step) * step;
    }
#ifdef MVC2_SDL3
    sync_snap();
#endif
    while (now < next_at) {
        double left_ms = (next_at - now) * 1000 / freq;
        if (left_ms > 2) {
            SDL_Delay((Uint32)(left_ms - 2));
        }
        now = (double)SDL_GetPerformanceCounter();
    }
    next_at += step;
}

#ifdef MVC2_SDL3
/* The timer paces every renderer; vsync only times each present to a refresh. A display slower than the game's
   59.94 Hz (a 50 Hz TV) would hold it back, so there vsync is off: from the display's mode, or measured, when the
   frames come late with it on and in time without (a GL driver waits in the next frame's drawing, not the present) */
enum { VSYNC_TRUSTED, VSYNC_TRIED_OFF, VSYNC_KEPT_OFF, VSYNC_NOT_THE_CAUSE };
static int vsync_on = -1, vsync_trial;
static Uint64 watch_last, watch_with;
static Uint64 watch_gaps[120];
static int watch_frames;

/* Exclusive full screen at the board's rate, with vsync. A display with variable refresh (VRR: G-Sync, FreeSync) on
   follows the timer's 59.94 Hz, each frame shown as it comes. One without shows each frame for a whole number N of
   its refreshes, and against the timer one frame in every few hundred for one more (at 240 Hz, every 3 s): there the
   frames are locked to N refreshes (60.02 Hz at 240.08, 60.00 at 60), each present 2 ms before its refresh by the
   compositor's clock, the sound stretched to match (sdl_audio.c). Locked frames on a VRR display come unevenly (the
   clock means nothing there), so they are only for a display known to be without. Which: mvc2.cfg's vrr = on or off,
   else NVIDIA's driver, asked after a second of frames (it says so only for a program presenting), else not known,
   and the timer stays. The presents themselves can't tell: SDL's swapchains queue them, not waiting a refresh each */
enum { SYNC_TIMER, SYNC_ASKING, SYNC_VRR, SYNC_LOCKED };
static int sync_mode, sync_n, sync_frames;
static float sync_hz;
static double sync_lead;    /* ticks before the refresh */

static int vrr_wanted(void) {
    const char* v = config_get("vrr");
    return v != NULL && strcmp(v, "on") == 0 ? 1 : v != NULL && strcmp(v, "off") == 0 ? 0 : -1;
}

/* the last refresh's time and the period, in performance counter ticks */
static int sync_clock(Uint64* last, Uint64* period) {
#ifdef _WIN32
    return display_vblank(last, period);
#else
    (void)last, (void)period;
    return 0;
#endif
}

static void sync_say(const char* text) {
    static char said[240];
    if (strcmp(said, text) != 0) {
        snprintf(said, sizeof said, "%s", text);
        host_log("%s", text);
    }
}

/* vrr: 1 on, 0 off, -1 not known; how: who says */
static void sync_set(int vrr, const char* how) {
    double freq = (double)SDL_GetPerformanceFrequency();
    char text[240];
    Uint64 last, period;
    sync_mode = SYNC_TIMER, sync_step = 0, sync_keep = 0;
    if (vrr == 1) {
        sync_mode = SYNC_VRR;
        snprintf(text, sizeof text, "pacing: VRR on (%s): the timer at %.2f Hz, the display (%.2f Hz at most) following",
                 how, frame_rate, sync_hz);
    } else if (vrr < 0) {
        snprintf(text, sizeof text, "pacing: VRR not known (%s): the timer at %.2f Hz (vrr = off locks the frames to the "
                 "refreshes, for a display without)", how, frame_rate);
    } else if (!sync_clock(&last, &period) || fabs(period * sync_hz / freq - 1) > 0.005) {
        snprintf(text, sizeof text, "pacing: no VRR (%s), and no clock of the refreshes: the timer at %.2f Hz", how,
                 frame_rate);
    } else {
        double now = (double)SDL_GetPerformanceCounter();
        sync_mode = SYNC_LOCKED, sync_keep = 1;
        sync_step = sync_n * (double)period;
        sync_lead = freq * 0.002 < period / 2.0 ? freq * 0.002 : period / 2.0;
        next_at = (double)last + ceil((now - (double)last) / (double)period) * (double)period - sync_lead;
        snprintf(text, sizeof text, "pacing: no VRR (%s): locked to %d refreshes of %.2f Hz a frame, %.3f Hz, each "
                 "present %.1f ms before its refresh; the sound stretched to match", how, sync_n,
                 freq / (double)period, freq / sync_step, sync_lead * 1000 / freq);
    }
    sync_say(text);
}

static void sync_judge(void) {
    const SDL_DisplayMode* m = window != NULL ? SDL_GetCurrentDisplayMode(SDL_GetDisplayForWindow(window)) : NULL;
    int vrr = vrr_wanted();
    sync_hz = m != NULL ? m->refresh_rate : 0;
    sync_mode = SYNC_TIMER, sync_step = 0, sync_keep = 0;
    if (frame_rate <= 0 || fabs(frame_rate - BOARD_RATE) > 0.001 || vsync_on != 1 || renderer == NULL ||
        fullscreen_kind() != FULL_EXCLUSIVE || sync_hz <= 0 || (sync_n = (int)(sync_hz / BOARD_RATE + 0.5)) < 1) {
        char text[100];
        snprintf(text, sizeof text, "pacing: the timer at %.2f Hz", frame_rate);
        sync_say(text);
        return;
    }
    if (vrr >= 0) {
        sync_set(vrr, "mvc2.cfg's vrr");
        return;
    }
    sync_mode = SYNC_ASKING;    /* the timer meanwhile */
    sync_frames = 0;
}

/* after each present: NVIDIA's driver asked, a second into exclusive full screen */
static void sync_presented(void) {
    if (sync_mode != SYNC_ASKING || ++sync_frames < 60) {
        return;
    }
#ifdef _WIN32
    {
        int vrr = display_vrr(SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WIN32_HWND_POINTER,
                                                     NULL));
        sync_set(vrr, vrr >= 0 ? "NVIDIA's driver" : "no NVIDIA driver to ask");
    }
#else
    sync_set(-1, "nothing to ask");
#endif
}

/* the frame's time on the refreshes' clock: the refresh nearest it, less the lead */
static void sync_snap(void) {
    Uint64 last, period;
    if (sync_mode == SYNC_LOCKED && sync_clock(&last, &period)) {
        double k = floor((next_at + sync_lead - (double)last) / (double)period + 0.5);
        next_at = (double)last + k * (double)period - sync_lead;
    }
}

static void set_vsync(void) {
    const SDL_DisplayMode* m = SDL_GetCurrentDisplayMode(SDL_GetDisplayForWindow(window));
    float hz = m != NULL ? m->refresh_rate : 0;
    int off = vsync_trial == VSYNC_TRIED_OFF || vsync_trial == VSYNC_KEPT_OFF;
    int on = !off && !(frame_rate > 0 && hz > 0 && hz < frame_rate - 1);
    if (on != vsync_on) {
        if (!SDL_SetRenderVSync(renderer, on)) {
            host_log("pacing: vsync not set (%s)", SDL_GetError());
        }
        vsync_on = on;
        watch_frames = 0, watch_last = 0;
        host_log("pacing: a timer at %.2f Hz, vsync %s (the display at %.2f Hz%s)", frame_rate, on ? "on" : "off", hz,
                 vsync_trial == VSYNC_TRIED_OFF   ? "; frames late with it on: tried off"
                 : vsync_trial == VSYNC_KEPT_OFF  ? "; in time only with it off"
                 : vsync_trial == VSYNC_NOT_THE_CAUSE ? "; as late with it off" : "");
    }
    sync_judge();
}

static int by_size(const void* a, const void* b) {
    Uint64 x = *(const Uint64*)a, y = *(const Uint64*)b;
    return x < y ? -1 : x > y;
}

/* each frame: the middle interval of 120, late by 3% with vsync on, tries it off for 120 more; kept off when that
   is in time, and on again, not tried again, when not */
static void watch_vsync(void) {
    Uint64 now = SDL_GetPerformanceCounter(), step, mid;
    sync_presented();
    if (frame_rate <= 0 || vsync_trial == VSYNC_KEPT_OFF || vsync_trial == VSYNC_NOT_THE_CAUSE ||
        sync_mode == SYNC_LOCKED) {
        watch_frames = 0, watch_last = 0;
        return;
    }
    if (watch_last != 0) {
        watch_gaps[watch_frames++] = now - watch_last;
    }
    watch_last = now;
    if (watch_frames < 120) {
        return;
    }
    watch_frames = 0;
    qsort(watch_gaps, 120, sizeof *watch_gaps, by_size);
    mid = watch_gaps[60];
    step = (Uint64)(SDL_GetPerformanceFrequency() / frame_rate);
    if (vsync_trial == VSYNC_TRUSTED && vsync_on == 1 && mid > step * 103 / 100) {
        watch_with = mid;
        vsync_trial = VSYNC_TRIED_OFF;
        set_vsync();
    } else if (vsync_trial == VSYNC_TRIED_OFF) {
        vsync_trial = mid < watch_with * 97 / 100 ? VSYNC_KEPT_OFF : VSYNC_NOT_THE_CAUSE;
        if (vsync_trial == VSYNC_KEPT_OFF) {
            host_log("pacing: vsync kept off, the frames in time without it");
        }
        set_vsync();
    }
}
#endif

static int scaling;     /* 0 aspect, 1 integer, 2 stretch */
static int scale_given; /* by mvc2.cfg or F10, else set_scaling's choice */
static void set_scaling(void);

#ifdef MVC2_SDL3
static SDL_GPUTexture* gpu_wrapped;     /* gpu_screen's */
static int gpu_wrapped_w, gpu_wrapped_h;
static int gpu_one_to_one;  /* the frame shown pixel for pixel, in the middle of the window */

/* the GPU's picture settings from mvc2.cfg: resolution (auto or 1 to 12, 4.2 say), aa (1 to 4), sprites, hud */
static float view_res;
static int view_aa = 1, view_sprites = RD_SPRITES_SHARP, view_hud = RD_HUD_ARCADE, view_grid;
static const char* const sprite_names[] = { "arcade", "sharp", "pixel", "smooth", "texel" };
static const char* const hud_names[] = { "sharp", "smooth", "arcade" };

void view_settings(void) {
    const char* r = config_get("resolution");
    const char* a = config_get("aa");
    const char* sp = config_get("sprites");
    const char* hud = config_get("hud");
    const char* grid = config_get("texel_grid");
    int i;
    view_res = r != NULL && atof(r) >= 1 ? (float)atof(r) : 0;
    view_aa = a != NULL && atoi(a) > 0 ? atoi(a) : 1;
    view_aa = view_aa > 4 ? 4 : view_aa;
    view_sprites = RD_SPRITES_SHARP;
    for (i = 0; i < (int)(sizeof sprite_names / sizeof sprite_names[0]); i++) {
        if (sp != NULL && strcmp(sp, sprite_names[i]) == 0) {
            view_sprites = i;
        }
    }
    view_grid = grid == NULL || strcmp(grid, "object") != 0;
    view_hud = RD_HUD_ARCADE;
    for (i = 0; i < (int)(sizeof hud_names / sizeof hud_names[0]); i++) {
        if (hud != NULL && strcmp(hud, hud_names[i]) == 0) {
            view_hud = i;
        }
    }
    if (renderer != NULL) {
        set_scaling();
    }
}

/* the game's picture's size in the window, in pixels */
static void view_size(int* w, int* h) {
    SDL_FRect r;
    *w = *h = 0;
    if (SDL_GetRenderLogicalPresentationRect(renderer, &r) && r.w >= 1 && r.h >= 1) {
        *w = (int)(r.w + 0.5f), *h = (int)(r.h + 0.5f);
    }
}

/* rd_gpu_view's resolution and aa, and the size the frame is shown at (w by h); 1 when shown pixel for pixel */
static int view_args(float* res, int* aa, int* w, int* h) {
    view_size(w, h);
    *res = view_res, *aa = view_aa;
    if (view_sprites == RD_SPRITES_TEXEL && scaling == 1) {
        /* texel sprites in whole pixels: 2688 by 2016 when it fits, 7 by 9 to a character's pixel, else the largest
           whole multiple of 640 by 480; each shown pixel for pixel */
        int ow, oh, fw, fh;
        SDL_GetRenderOutputSize(renderer, &ow, &oh);
        if (*res <= 0) {
            int k = oh / RD_HEIGHT < ow / RD_WIDTH ? oh / RD_HEIGHT : ow / RD_WIDTH;
            *res = ow >= 2688 && oh >= 2016 ? 4.2f : k < 1 ? 1 : (float)k;
        }
        fh = 3 * (int)(RD_HEIGHT * *res / 3 + 0.5f), fw = fh / 3 * 4;
        *res *= view_aa < 1 ? 1 : view_aa, *aa = 1;
        if (fw <= ow && fh <= oh) {
            *w = fw, *h = fh;
            return 1;
        }
    }
    return 0;
}

/* the OpenGL renderer's: resolution times the game's 640 by 480, or (auto) enough for h; no aa */
static void gl_size(int h, int* fw, int* fh) {
    float k = view_res > 0 ? view_res : h <= 0 ? 1 : (float)((h + RD_HEIGHT - 1) / RD_HEIGHT);
    k = k < 1 ? 1 : k > RD_GPU_MAX_SCALE ? RD_GPU_MAX_SCALE : k;
    *fh = 3 * (int)(RD_HEIGHT * k / 3 + 0.5f), *fw = *fh / 3 * 4;
}

void view_drawn(int* fw, int* fh) {
    float res;
    int aa, w, h;
    if (glr) {
        view_size(&w, &h);
        gl_size(h, fw, fh);
        return;
    }
    view_args(&res, &aa, &w, &h);
    rd_gpu_view_size(res, aa, h, view_sprites, fw, fh);
}

/* the frame drawn by OpenGL, shown nearest when at the picture's size and filtered when not */
static SDL_Texture* gl_picture(void) {
    SDL_Texture* t;
    int w, h, fw, fh;
    view_size(&w, &h);
    gl_size(h, &fw, &fh);
    t = gl_frame(renderer, fw, fh);
    if (t == NULL) {
        return screen;
    }
    SDL_SetTextureScaleMode(t, fh == h || h <= 0 ? SDL_SCALEMODE_NEAREST : SDL_SCALEMODE_LINEAR);
    return t;
}

static SDL_Texture* gpu_frame(void) {
    SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(gpu);
    SDL_GPUTexture* t;
    float res;
    int aa, w, h;
    if (cmd == NULL) {
        return gpu_screen;
    }
    gpu_one_to_one = view_args(&res, &aa, &w, &h);
    rd_gpu_view(res, aa, w, h, view_sprites);
    rd_gpu_hud(view_hud);
    rd_gpu_texel_grid(view_grid);
    t = rd_gpu_end_frame(cmd);
    rd_gpu_submit(cmd);
    rd_gpu_frame_size(&w, &h);
    if (gpu_screen != NULL && (t != gpu_wrapped || w != gpu_wrapped_w || h != gpu_wrapped_h)) {
        SDL_DestroyTexture(gpu_screen);
        gpu_screen = NULL;
    }
    if (gpu_screen == NULL && t != NULL) {
        SDL_PropertiesID props = SDL_CreateProperties();
        SDL_SetNumberProperty(props, SDL_PROP_TEXTURE_CREATE_FORMAT_NUMBER, SDL_PIXELFORMAT_ABGR8888);
        SDL_SetNumberProperty(props, SDL_PROP_TEXTURE_CREATE_ACCESS_NUMBER, SDL_TEXTUREACCESS_STATIC);
        SDL_SetNumberProperty(props, SDL_PROP_TEXTURE_CREATE_WIDTH_NUMBER, w);
        SDL_SetNumberProperty(props, SDL_PROP_TEXTURE_CREATE_HEIGHT_NUMBER, h);
        SDL_SetPointerProperty(props, SDL_PROP_TEXTURE_CREATE_GPU_TEXTURE_POINTER, t);
        gpu_screen = SDL_CreateTextureWithProperties(renderer, props);
        SDL_DestroyProperties(props);
        if (gpu_screen != NULL) {
            SDL_SetTextureScaleMode(gpu_screen, SDL_SCALEMODE_NEAREST);
            SDL_SetTextureBlendMode(gpu_screen, SDL_BLENDMODE_NONE);    /* the frame's alpha isn't shown */
        }
        gpu_wrapped = t, gpu_wrapped_w = w, gpu_wrapped_h = h;
    }
    return gpu_screen;
}
#endif

/* F3: the game's and the drawing's time a frame, and the frame rate, averaged over half a second */
enum { TIMES_W = 240, TIMES_H = 12 };
static int times_on;
static SDL_Texture* times_tex;
static Uint64 game_ticks, draw_ticks, present_ticks, times_start;
static unsigned times_frames;

static void times_update(void) {
    static uint32_t text[TIMES_W * TIMES_H];
    char line[64];
    double freq = (double)SDL_GetPerformanceFrequency();
    Uint64 now = SDL_GetPerformanceCounter();
    int i;
    if (times_start == 0) {
        game_ticks = draw_ticks = 0, times_frames = 0, times_start = now;
        return;
    }
    if (now - times_start < freq / 2) {
        return;
    }
    snprintf(line, sizeof line, "GAME %5.1f MS  DRAW %5.1f MS  %5.1f FPS", game_ticks * 1000.0 / freq / times_frames,
             draw_ticks * 1000.0 / freq / times_frames, times_frames * freq / (now - times_start));
    game_ticks = draw_ticks = 0, times_frames = 0, times_start = now;
    if (times_tex == NULL) {
        times_tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, TIMES_W, TIMES_H);
        if (times_tex == NULL) {
            return;
        }
        SDL_SetTextureBlendMode(times_tex, SDL_BLENDMODE_BLEND);
#ifdef MVC2_SDL3
        SDL_SetTextureScaleMode(times_tex, SDL_SCALEMODE_NEAREST);
#endif
    }
    for (i = 0; i < TIMES_W * TIMES_H; i++) {
        text[i] = 0xA0000000u;
    }
    menu_text(text, TIMES_W, TIMES_H, 3, 2, line, 0xFFFFFFFFu, 1);
    SDL_UpdateTexture(times_tex, NULL, text, TIMES_W * 4);
}

#ifdef MVC2_SDL3
/* the game's picture as the window shows it, at r in the window's pixels, each of the 640 by 480 the average of the
   window's pixels whose centres are in it, saved as window_shot */
static void shot_window(const SDL_FRect* r) {
    static uint32_t shot[RD_WIDTH * RD_HEIGHT];
    SDL_Surface *s, *c = NULL;
    double sx = r->w / RD_WIDTH, sy = r->h / RD_HEIGHT;
    int x, y, i, j;
    SDL_SetRenderLogicalPresentation(renderer, 0, 0, SDL_LOGICAL_PRESENTATION_DISABLED);
    s = SDL_RenderReadPixels(renderer, NULL);
    set_scaling();
    if (s != NULL) {
        c = SDL_ConvertSurface(s, SDL_PIXELFORMAT_ARGB8888);
        SDL_DestroySurface(s);
    }
    if (c == NULL) {
        host_log("shot %s: %s", window_shot, SDL_GetError());
        return;
    }
    for (y = 0; y < RD_HEIGHT; y++) {
        int y0 = (int)ceil(r->y + y * sy - 0.5), y1 = (int)ceil(r->y + (y + 1) * sy - 0.5);
        y0 = y0 < 0 ? 0 : y0, y1 = y1 > c->h ? c->h : y1;
        for (x = 0; x < RD_WIDTH; x++) {
            int x0 = (int)ceil(r->x + x * sx - 0.5), x1 = (int)ceil(r->x + (x + 1) * sx - 0.5);
            uint32_t sum[3] = { 0, 0, 0 }, n;
            x0 = x0 < 0 ? 0 : x0, x1 = x1 > c->w ? c->w : x1;
            n = (uint32_t)((x1 - x0) * (y1 - y0));
            for (j = y0; j < y1; j++) {
                const uint32_t* p = (const uint32_t*)((const uint8_t*)c->pixels + (size_t)j * c->pitch);
                for (i = x0; i < x1; i++) {
                    sum[0] += p[i] >> 16 & 255, sum[1] += p[i] >> 8 & 255, sum[2] += p[i] & 255;
                }
            }
            /* the window shows no alpha */
            shot[y * RD_WIDTH + x] = n == 0 ? 0xFF000000u : 0xFF000000u | (sum[0] + n / 2) / n << 16 |
                                                                (sum[1] + n / 2) / n << 8 | (sum[2] + n / 2) / n;
        }
    }
    SDL_DestroySurface(c);
    save_bmp(window_shot, shot, RD_WIDTH, RD_HEIGHT);
}
#endif

static void show(SDL_Texture* t) {
#ifdef MVC2_SDL3
    SDL_FRect r = { 0, 0, 0, 0 };
#endif
    SDL_RenderClear(renderer);
#ifdef MVC2_SDL3
    if (gpu && gpu_one_to_one && t != NULL) {
        int ow, oh;
        SDL_SetRenderLogicalPresentation(renderer, 0, 0, SDL_LOGICAL_PRESENTATION_DISABLED);
        SDL_GetRenderOutputSize(renderer, &ow, &oh);
        r.w = (float)t->w, r.h = (float)t->h, r.x = (float)((ow - t->w) / 2), r.y = (float)((oh - t->h) / 2);
        SDL_RenderTexture(renderer, t, NULL, &r);
        set_scaling();
    } else {
        SDL_GetRenderLogicalPresentationRect(renderer, &r);
        SDL_RenderTexture(renderer, t, NULL, NULL);
    }
#else
    SDL_RenderTexture(renderer, t, NULL, NULL);
#endif
    if (times_on && times_tex != NULL) {
        render_at(renderer, times_tex, 4, 4, TIMES_W, TIMES_H);
    }
#ifdef MVC2_SDL3
    if (window_shot[0] != 0) {
        shot_window(&r);
        window_shot[0] = 0;
    }
#endif
    pace();
    SDL_RenderPresent(renderer);
#ifdef MVC2_SDL3
    watch_vsync();
#endif
}

static Uint64 fl_draw;     /* MVC2_FRAMELOG */
static uint32_t fl_hash;

void host_present(void) {
    SDL_Texture* t = screen;
    Uint64 start = SDL_GetPerformanceCounter();
#ifdef MVC2_SDL3
    if (gpu) {
        t = gpu_frame();
        frame_read = 0;
    } else if (glr) {
        t = gl_picture();
        frame_read = 0;
    } else
#endif
    {
        rd_end_frame(frame, RD_WIDTH);
    }
    rd_begin_frame();
    presented++;
    screenshot();
    if (!gpu && !glr) {
        SDL_UpdateTexture(screen, NULL, frame, RD_WIDTH * 4);
    }
    if (getenv("MVC2_FRAMELOG")) {
        uint32_t h = 2166136261u;
        int i;
        for (i = 0; i < RD_WIDTH * RD_HEIGHT; i += 7) {
            h = (h ^ frame[i]) * 16777619u;
        }
        fl_hash = h;
    }
    draw_ticks += SDL_GetPerformanceCounter() - start;
    fl_draw = SDL_GetPerformanceCounter() - start;
    times_frames++;
    if (times_on) {
        times_update();
    }
    show(t);
    present_ticks += SDL_GetPerformanceCounter() - start;
}

#ifdef MVC2_SDL3
#define FULLSCREEN SDL_WINDOW_FULLSCREEN    /* at the desktop's size */
#else
#define FULLSCREEN SDL_WINDOW_FULLSCREEN_DESKTOP
#endif

static const char* const scalings[] = { "aspect", "integer", "stretch" };
static int keep_settings;

static void set_scaling(void) {
    int w, h, small;
#ifdef MVC2_SDL3
    static const SDL_RendererLogicalPresentation mode[] = { SDL_LOGICAL_PRESENTATION_LETTERBOX,
                                                           SDL_LOGICAL_PRESENTATION_INTEGER_SCALE,
                                                           SDL_LOGICAL_PRESENTATION_STRETCH };
    SDL_GetCurrentRenderOutputSize(renderer, &w, &h);
    if (!scale_given) {
        /* texel sprites full screen: 2688 by 2016 shown pixel for pixel where it fits */
        scaling = view_sprites == RD_SPRITES_TEXEL && (SDL_GetWindowFlags(window) & FULLSCREEN) && w >= 2688 && h >= 2016;
    }
    small = w < RD_WIDTH || h < RD_HEIGHT;
    SDL_SetRenderLogicalPresentation(renderer, RD_WIDTH, RD_HEIGHT, mode[scaling == 1 && small ? 0 : scaling]);
#else
    SDL_GetRendererOutputSize(renderer, &w, &h);
    small = w < RD_WIDTH || h < RD_HEIGHT;
    SDL_RenderSetLogicalSize(renderer, scaling == 2 ? 0 : RD_WIDTH, scaling == 2 ? 0 : RD_HEIGHT);
    SDL_RenderSetIntegerScale(renderer, scaling == 1 && !small);
#endif
}

/* the largest whole multiple of 640 by 480 in nine tenths of the desktop */
static void first_size(int* w, int* h) {
    SDL_Rect r;
    int k = 2;
#ifdef MVC2_SDL3
    if (SDL_GetDisplayUsableBounds(SDL_GetPrimaryDisplay(), &r)) {
#else
    if (SDL_GetDisplayUsableBounds(0, &r) == 0) {
#endif
        k = r.w * 9 / 10 / RD_WIDTH < r.h * 9 / 10 / RD_HEIGHT ? r.w * 9 / 10 / RD_WIDTH : r.h * 9 / 10 / RD_HEIGHT;
        k = k < 1 ? 1 : k;
    }
    *w = RD_WIDTH * k, *h = RD_HEIGHT * k;
}

#ifdef MVC2_SDL3
/* SDL_GPU's renderer; when a step fails, nothing is kept */
static int open_gpu(void) {
    /* Vulkan first: Direct3D 12 makes each new texture and buffer on its own, and a hyper combo's can stall a
       frame by 20 ms or more, enough to run the sound dry */
    static const char* const drivers[] = { "vulkan", NULL };
    int i;
    for (i = 0; i < 2; i++) {
        gpu = SDL_CreateGPUDevice(RD_GPU_SHADER_FORMATS | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_SPIRV |
                                      SDL_GPU_SHADERFORMAT_MSL, false, drivers[i]);
        if (gpu != NULL) {
            if (rd_gpu_init(gpu)) {
                renderer = SDL_CreateGPURenderer(gpu, window);
                if (renderer != NULL) {
                    host_log("renderer: gpu (%s, %s)", SDL_GetGPUDeviceDriver(gpu), SDL_GetCurrentVideoDriver());
                    return 1;
                }
            }
            rd_gpu_quit();
            SDL_DestroyGPUDevice(gpu);
            gpu = NULL;
        }
    }
    host_log("renderer: no gpu (%s)", SDL_GetError());
    return 0;
}

/* the OpenGL renderer (gl_sdl.c) */
static int open_gl(void) {
    renderer = gl_open(window);
    glr = renderer != NULL;
    if (glr) {
        host_log("renderer: gl (%s, %s)", gl_name(), SDL_GetCurrentVideoDriver());
    } else {
        host_log("renderer: no gl (%s)", SDL_GetError());
    }
    return glr;
}

/* rd_gpu.c's state starts again with the other program */
static void gpu_restart(int stop) {
    if (!gpu) {
        return;
    }
    if (stop) {
        SDL_DestroyTexture(gpu_screen);
        gpu_screen = NULL;
        rd_gpu_quit();
    } else if (!rd_gpu_init(gpu)) {
        host_log("rd_gpu: could not start again");
    }
}
#else
#define gpu_restart(stop)
#endif

/* want: RENDERER_GPU tries SDL_GPU, then OpenGL; RENDERER_GL OpenGL; and where those fail, or RENDERER_SOFT, rd_soft
   draws, shown by SDL's own renderer */
static int make_renderer(int want) {
#ifdef MVC2_SDL3
    int up = want == RENDERER_GPU && open_gpu();
    up = up || (want != RENDERER_SOFT && open_gl());
    if (!up) {
        host_log("renderer: soft%s", want == RENDERER_SOFT ? " (as asked)" : "");
    }
    if (renderer == NULL) {
        renderer = SDL_CreateRenderer(window, NULL);
    }
    if (renderer == NULL) {
        return 0;
    }
    vsync_on = -1;
    set_vsync();
#else
    (void)want;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        return 0;
    }
#endif
    screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, RD_WIDTH, RD_HEIGHT);
    SDL_SetTextureBlendMode(screen, SDL_BLENDMODE_NONE);
#ifdef MVC2_SDL3
    SDL_SetTextureScaleMode(screen, SDL_SCALEMODE_NEAREST);     /* SDL3's default is linear */
#endif
    set_scaling();
    return screen != NULL;
}

int renderer_gpu(void) {
    return gpu != 0;
}

int renderer_kind(void) {
    return gpu ? RENDERER_GPU : glr ? RENDERER_GL : RENDERER_SOFT;
}

/* the renderer closed, with what it holds */
static void close_renderer(void) {
#ifdef MVC2_SDL3
    SDL_Texture** own[] = { &times_tex, &screen, &gpu_screen };
    size_t i;
    for (i = 0; i < sizeof own / sizeof own[0]; i++) {
        if (*own[i] != NULL) {
            SDL_DestroyTexture(*own[i]);
            *own[i] = NULL;
        }
    }
    if (glr) {
        gl_close(renderer);
        glr = 0;
    }
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    if (gpu != NULL) {
        rd_gpu_quit();
        SDL_DestroyGPUDevice(gpu);
        gpu = NULL;
    }
#endif
}

#if defined(MVC2_SDL3) && defined(_WIN32)
/* Windows gives Vulkan and Direct3D no swapchain in a window that OpenGL has drawn in after one of theirs (access
   denied), so the renderer after OpenGL gets a new window, as SDL gives OpenGL itself: the same place, size and
   full screen */
static void window_again(void) {
    SDL_Window* old = window;
    SDL_Window* made;
    int kind = fullscreen_kind(), x, y, w, h;
    if (kind != FULL_OFF) {
        SDL_SetWindowFullscreen(old, false);
        SDL_SyncWindow(old);
    }
    SDL_GetWindowPosition(old, &x, &y);
    SDL_GetWindowSize(old, &w, &h);
    made = SDL_CreateWindow(SDL_GetWindowTitle(old), w, h, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN |
                                                               (SDL_GetWindowFlags(old) & SDL_WINDOW_MAXIMIZED));
    if (made != NULL) {
        SDL_SetWindowMinimumSize(made, RD_WIDTH / 2, RD_HEIGHT / 2);
        SDL_SetWindowPosition(made, x, y);
        SDL_DestroyWindow(old);
        window = made;
    }
    if (kind != FULL_OFF) {
        fullscreen_set(kind);
    }
    SDL_ShowWindow(window);
    SDL_SyncWindow(window);
}
#else
#define window_again()
#endif

int renderer_set(int want) {
#ifdef MVC2_SDL3
    int was_gl = glr;
    if (want == renderer_kind()) {
        return want;
    }
    read_frame();
    close_renderer();
    if (was_gl) {
        window_again();
    }
    if (!make_renderer(want)) {
        host_log("no renderer: %s", SDL_GetError());
    }
    frame_read = 1;     /* frame holds the last picture */
    return renderer_kind();
#else
    (void)want;
    return RENDERER_SOFT;
#endif
}

/* full screen: FULL_DESKTOP a window over the whole display at the desktop's mode, FULL_EXCLUSIVE a display mode of
   its own (SDL3's; SDL restores the desktop's when the window goes, loses the focus or is minimized); full_kind is
   the one Alt+Enter goes to */
static int full_kind = FULL_DESKTOP;

static int full_named(const char* v) {
    return v == NULL                                                       ? -1
           : strcmp(v, "off") == 0 || strcmp(v, "no") == 0                 ? FULL_OFF
           : strcmp(v, "desktop") == 0 || strcmp(v, "yes") == 0            ? FULL_DESKTOP
           : strcmp(v, "exclusive") == 0                                   ? FULL_EXCLUSIVE
                                                                           : -1;
}

int fullscreen_kind(void) {
    return !(SDL_GetWindowFlags(window) & FULLSCREEN) ? FULL_OFF
#ifdef MVC2_SDL3
           : SDL_GetWindowFullscreenMode(window) != NULL ? FULL_EXCLUSIVE
#endif
                                                         : FULL_DESKTOP;
}

#ifdef MVC2_SDL3
/* the exclusive mode: the desktop's size, at the most refreshes a frame of the game's 59.94 Hz that a whole number
   divides (240 or 120 Hz, nearest a multiple: 59.94 before 60), or at 60 alone with mvc2.cfg's refresh = 60; the
   desktop's own mode when none is */
static int exclusive_mode(SDL_DisplayMode* out) {
    SDL_DisplayID d = SDL_GetDisplayForWindow(window);
    const SDL_DisplayMode* desk = SDL_GetDesktopDisplayMode(d);
    const char* r = config_get("refresh");
    int only60 = r != NULL && strcmp(r, "60") == 0, n = 0, i, best_k = 0;
    float best_off = 1;
    SDL_DisplayMode** modes;
    if (desk == NULL) {
        return 0;
    }
    *out = *desk;
    modes = SDL_GetFullscreenDisplayModes(d, &n);
    for (i = 0; modes != NULL && i < n; i++) {
        const SDL_DisplayMode* m = modes[i];
        float k = m->refresh_rate / (float)(27000000.0 / (858.0 * 525.0));
        int whole = (int)(k + 0.5f);
        float off = fabsf(k - (float)whole) / (float)(whole > 0 ? whole : 1);
        if (m->w != desk->w || m->h != desk->h || whole < 1 || off > 0.005f || (only60 && whole != 1)) {
            continue;
        }
        if (whole > best_k || (whole == best_k && off < best_off)) {
            *out = *m, best_k = whole, best_off = off;
        }
    }
    SDL_free(modes);
    return 1;
}
#endif

static const char* const full_names[] = { "off", "desktop", "exclusive" };

/* kind (FULL_OFF, _DESKTOP or _EXCLUSIVE), kept in mvc2.cfg unless a test run's: fullscreen the state, and
   fullscreen_kind the kind last asked for, which Alt+Enter goes to from a window, at the next start too */
void fullscreen_set(int kind) {
    int asked = kind;
#ifdef MVC2_SDL3
    SDL_DisplayMode m;
    if (kind == FULL_EXCLUSIVE && strcmp(SDL_GetCurrentVideoDriver(), "wayland") == 0) {
        host_log("fullscreen: Wayland has no exclusive mode; the desktop's");
        kind = FULL_DESKTOP;
    }
    if (kind == FULL_EXCLUSIVE && exclusive_mode(&m)) {
        if (!SDL_SetWindowFullscreenMode(window, &m)) {
            host_log("fullscreen: no exclusive mode (%s); the desktop's", SDL_GetError());
            kind = FULL_DESKTOP;
        } else {
            host_log("fullscreen: exclusive, %d by %d at %.2f Hz", m.w, m.h, m.refresh_rate);
        }
    }
    if (kind != FULL_EXCLUSIVE) {
        SDL_SetWindowFullscreenMode(window, NULL);
    }
    SDL_SetWindowFullscreen(window, kind != FULL_OFF);
#else
    SDL_SetWindowFullscreen(window, kind != FULL_OFF ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
#endif
    full_kind = asked != FULL_OFF ? asked : full_kind;
#ifdef MVC2_SDL3
    sync_judge();
#endif
    if (keep_settings) {
        const char* was = config_get("fullscreen_kind");
        config_set("fullscreen", full_names[asked]);     /* as asked: exclusive again where it can be */
        if (asked != FULL_OFF && (was == NULL || strcmp(was, full_names[asked]) != 0)) {
            config_set("fullscreen_kind", full_names[asked]);
        }
    }
}

/* the desktop's mode back however the game ends: at exit, and (where the system doesn't, as X11) on a crash */
static void full_restore(void) {
    if (window != NULL && SDL_WasInit(SDL_INIT_VIDEO) && (SDL_GetWindowFlags(window) & FULLSCREEN)) {
        SDL_SetWindowFullscreen(window, 0);
    }
}

#ifndef _WIN32
#include <signal.h>
static void full_crash(int sig) {
    full_restore();
    signal(sig, SIG_DFL);
    raise(sig);
}
#endif

/* sizes in pixels, so a whole multiple stays sharp at any display scaling */
static int open_window(void) {
    const char* size = config_get("window");
    const char* full = config_get("fullscreen");
    const char* scale = config_get("scale");
    const char* r;
    int w, h, i, kind;
    keep_settings = getenv("MVC2_FRAMES") == NULL;
    if (!keep_settings) {
        full = NULL;
    } else if (full == NULL && getenv("GAMESCOPE_WAYLAND_DISPLAY") != NULL) {
        /* Steam's Game Mode scales a window up to the screen: full screen unless mvc2.cfg says */
        full = "desktop";
    }
    full = getenv("MVC2_FULLSCREEN") != NULL ? getenv("MVC2_FULLSCREEN") : full;
    kind = full_named(full) < 0 ? FULL_OFF : full_named(full);
    full_kind = full_named(config_get("fullscreen_kind")) > FULL_OFF ? full_named(config_get("fullscreen_kind"))
                                                                     : full_kind;
    if (size == NULL || sscanf(size, "%dx%d", &w, &h) != 2) {
        first_size(&w, &h);
    }
    w = w < RD_WIDTH / 2 ? RD_WIDTH / 2 : w;
    h = h < RD_HEIGHT / 2 ? RD_HEIGHT / 2 : h;
#ifdef MVC2_SDL3
    /* hidden until full screen is set, so a window doesn't show first */
    window = SDL_CreateWindow(game_title(), w, h, SDL_WINDOW_RESIZABLE | (kind != FULL_OFF ? SDL_WINDOW_HIDDEN : 0));
    if (window == NULL) {
        return 0;
    }
    SDL_SetWindowMinimumSize(window, RD_WIDTH / 2, RD_HEIGHT / 2);
    if (kind != FULL_OFF) {
        fullscreen_set(kind);
        SDL_ShowWindow(window);
        SDL_SyncWindow(window);
    }
#else
    window = SDL_CreateWindow(game_title(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h,
                              SDL_WINDOW_RESIZABLE | (kind != FULL_OFF ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0));
    if (window == NULL) {
        return 0;
    }
    SDL_SetWindowMinimumSize(window, RD_WIDTH / 2, RD_HEIGHT / 2);
#endif
    full_kind = kind != FULL_OFF ? kind : full_kind;
    atexit(full_restore);
#ifndef _WIN32
    signal(SIGSEGV, full_crash);
    signal(SIGABRT, full_crash);
    signal(SIGBUS, full_crash);
    signal(SIGFPE, full_crash);
#endif
    for (i = 0; i < 3; i++) {
        if (scale != NULL && strcmp(scale, scalings[i]) == 0) {
            scaling = i, scale_given = 1;
        }
    }
    /* the GPU (or OpenGL) unless mvc2.cfg says soft or gl; a test run draws by rd_soft.c's rules unless it says gpu
       or gl */
    r = config_get("renderer");
    return make_renderer(r == NULL ? (test_run ? RENDERER_SOFT : RENDERER_GPU)
                         : strcmp(r, "soft") == 0 ? RENDERER_SOFT
                         : strcmp(r, "gl") == 0   ? RENDERER_GL
                                                  : RENDERER_GPU);
}

/* 1 when the window took the event */
static int window_key(const SDL_Event* e) {
    if (window_resized(e)) {
        set_scaling();
#ifdef MVC2_SDL3
        if (e->type == SDL_EVENT_WINDOW_ENTER_FULLSCREEN || e->type == SDL_EVENT_WINDOW_LEAVE_FULLSCREEN) {
            sync_judge();
        }
#endif
        return 0;
    }
#ifdef MVC2_SDL3
    if (e->type == SDL_EVENT_WINDOW_DISPLAY_CHANGED || e->type == SDL_EVENT_DISPLAY_CURRENT_MODE_CHANGED) {
        vsync_trial = VSYNC_TRUSTED;   /* another display, or another mode: judged again */
        set_vsync();
        return 0;
    }
#endif
    if (e->type != SDL_EVENT_KEY_DOWN || e->key.repeat) {
        return 0;
    }
    if (key_sym(e) == SDLK_F11 || (key_sym(e) == SDLK_RETURN && key_alt(e))) {
        fullscreen_set(fullscreen_kind() == FULL_OFF ? full_kind : FULL_OFF);
        return 1;
    }
    if (key_sym(e) == SDLK_F3) {
        times_on = !times_on;
        times_start = 0;
        return 1;
    }
    if (key_sym(e) == SDLK_F10) {
        scaling = (scaling + 1) % 3, scale_given = 1;
        set_scaling();
        if (keep_settings) {
            config_set("scale", scalings[scaling]);
        }
        return 1;
    }
    return 0;
}

static void save_window(void) {
    char size[32];
    const char* was = config_get("window");
    int w, h;
    if (!keep_settings || (SDL_GetWindowFlags(window) & (FULLSCREEN | SDL_WINDOW_MAXIMIZED))) {
        return;
    }
    SDL_GetWindowSize(window, &w, &h);
    snprintf(size, sizeof size, "%dx%d", w, h);
    if (was == NULL || strcmp(was, size) != 0) {
        config_set("window", size);
    }
}

/* the arcade's test menu (F2, the arcade only): its 40 by 30 characters, 16 pixels each, on black */
static int test_open, restart_req;

static int test_key(const SDL_Event* e) {
    static int axis[2];
    if (e->type == SDL_EVENT_KEY_DOWN) {
        SDL_Scancode k = key_scancode(e);
        int first = !e->key.repeat;
        return k == SDL_SCANCODE_UP ? TM_UP : k == SDL_SCANCODE_DOWN ? TM_DOWN : k == SDL_SCANCODE_LEFT ? TM_LEFT
               : k == SDL_SCANCODE_RIGHT ? TM_RIGHT
               : (k == SDL_SCANCODE_RETURN || k == SDL_SCANCODE_KP_ENTER) && first ? TM_OK
               : (k == SDL_SCANCODE_ESCAPE || k == SDL_SCANCODE_F2) && first ? TM_BACK : 0;
    }
    if (e->type == SDL_EVENT_GAMEPAD_BUTTON_DOWN) {
        int b = e->gbutton.button;
        return b == SDL_GAMEPAD_BUTTON_DPAD_UP ? TM_UP : b == SDL_GAMEPAD_BUTTON_DPAD_DOWN ? TM_DOWN
               : b == SDL_GAMEPAD_BUTTON_DPAD_LEFT ? TM_LEFT : b == SDL_GAMEPAD_BUTTON_DPAD_RIGHT ? TM_RIGHT
               : b == SDL_GAMEPAD_BUTTON_SOUTH ? TM_OK : b == SDL_GAMEPAD_BUTTON_EAST ? TM_BACK : 0;
    }
    if (e->type == SDL_EVENT_GAMEPAD_AXIS_MOTION &&
        (e->gaxis.axis == SDL_GAMEPAD_AXIS_LEFTX || e->gaxis.axis == SDL_GAMEPAD_AXIS_LEFTY)) {
        int y = e->gaxis.axis == SDL_GAMEPAD_AXIS_LEFTY, way = e->gaxis.value > 24000 ? 1 : e->gaxis.value < -24000 ? -1 : 0;
        int moved = way != 0 && way != axis[y];
        axis[y] = way;
        return !moved ? 0 : y ? (way < 0 ? TM_UP : TM_DOWN) : (way < 0 ? TM_LEFT : TM_RIGHT);
    }
    return 0;
}

static void present_test(void) {
    static uint32_t shown[RD_WIDTH * RD_HEIGHT];
    const TEST_SCREEN* s = test_mode_screen();
    int i, x, y;
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
    SDL_UpdateTexture(screen, NULL, shown, RD_WIDTH * 4);
    show(screen);
}

static void present_menu(void) {
    static uint32_t shown[RD_WIDTH * RD_HEIGHT];
    read_frame();
    memcpy(shown, frame, sizeof shown);
    menu_draw(shown, RD_WIDTH, RD_HEIGHT);
    SDL_UpdateTexture(screen, NULL, shown, RD_WIDTH * 4);
    show(screen);
}

/* broken at spaces into lines of cols characters; the y below it */
static int draw_wrapped(int x, int y, int cols, const char* text, uint32_t color) {
    char line[128];
    while (*text != 0) {
        int n = (int)strcspn(text, "\n"), cut = n;
        if (n > cols) {
            for (cut = cols; cut > 0 && text[cut] != ' '; cut--) {
            }
            cut = cut > 0 ? cut : cols;
        }
        snprintf(line, sizeof line, "%.*s", cut, text);
        menu_text(frame, RD_WIDTH, RD_HEIGHT, x, y, line, color, 1);
        y += 12;
        text += cut;
        text += *text == ' ' || *text == '\n';
    }
    return y;
}

/* what data is missing, until Enter finds it (1) or Escape quits (0) */
static int missing_screen(int argc, char** argv) {
    if (pick_message()[0] == 0 || getenv("MVC2_FRAMES") != NULL) {
        return 0;
    }
    if (!open_window()) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Marvel vs. Capcom 2", pick_message(), NULL);
        return 0;
    }
    SDL_SetWindowTitle(window, "Marvel vs. Capcom 2");
    for (;;) {
        SDL_Event e;
        int i, y;
        for (i = 0; i < RD_WIDTH * RD_HEIGHT; i++) {
            frame[i] = 0xFF101830u;
        }
        y = draw_wrapped(16, 16, 100, "MARVEL VS. CAPCOM 2", 0xFFFFE040u);
        y = draw_wrapped(16, y + 12, 100, pick_message(), 0xFFFFFFFFu);
        draw_wrapped(16, y + 12, 100, "ENTER: LOOK AGAIN    ESCAPE: QUIT", 0xFFA0A0A0u);
        SDL_UpdateTexture(screen, NULL, frame, RD_WIDTH * 4);
        show(screen);
        while (SDL_PollEvent(&e)) {
            if (window_key(&e)) {
                continue;
            }
            if (e.type == SDL_EVENT_QUIT || (e.type == SDL_EVENT_KEY_DOWN && key_sym(&e) == SDLK_ESCAPE)) {
                return 0;
            }
            if (e.type == SDL_EVENT_KEY_DOWN && (key_sym(&e) == SDLK_RETURN || key_sym(&e) == SDLK_KP_ENTER)) {
                if (pick_game(argc, argv)) {
                    SDL_SetWindowTitle(window, game_title());
                    return 1;
                }
                if (pick_message()[0] == 0) {
                    return 0;
                }
            }
        }
    }
}

void* host_try_malloc(size_t bytes) {
    return malloc(bytes);
}

void* host_try_calloc(size_t n, size_t size) {
    return calloc(n, size);
}

/* why the game stops: on stderr, and in a box where nobody would read that */
static void stop_box(const char* text) {
    fprintf(stderr, "%s\n", text);
    if (!console_read && !test_run) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Marvel vs. Capcom 2", text, window);
    }
}

/* a start that failed, unless the player was already told why */
static void not_started(void) {
    if (pick_why() == PICK_BAD_DISC) {
        return;
    }
    stop_box(console_read || test_run ? "The game could not start."
                                      : "The game could not start. With console = yes in mvc2.cfg, the console says why.");
}

void host_out_of_memory(const char* what, size_t bytes) {
    char text[200];
    snprintf(text, sizeof text, "Out of memory: %s of %lu bytes failed.", what, (unsigned long)bytes);
    host_log("stopped: %s", text);
    sdl_save_flush();
    if (getenv("MVC2_FRAMES") == NULL) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Marvel vs. Capcom 2", text, NULL);
    }
    exit(1);
}

int main(int argc, char** argv) {
    int quit = 0;
    long frames_left = getenv("MVC2_FRAMES") ? atol(getenv("MVC2_FRAMES")) : -1;

    test_run = frames_left >= 0;
#ifdef _WIN32
    console_read = console_start();
#endif
    hunting = test_run && hunt_init();

    if (!naomi_map_ram()) {
        char text[100];
        snprintf(text, sizeof text, "Could not map the game's main RAM at %08X.", NAOMI_RAM_BASE);
        stop_box(text);
        return 1;
    }
    host_divzero_install();
    pace_init();
#ifdef MVC2_SDL3
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | (test_run ? 0 : SDL_INIT_GAMEPAD))) {
#else
    SDL_SetHint(SDL_HINT_WINDOWS_DPI_AWARENESS, "permonitorv2");
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | (test_run ? 0 : SDL_INIT_GAMECONTROLLER)) != 0) {
#endif
        char text[300];
        snprintf(text, sizeof text, "SDL_Init: %s", SDL_GetError());
        stop_box(text);
        return 1;
    }
#ifdef _WIN32
    config_read();
    if (config_get("console") != NULL && strcmp(config_get("console"), "yes") == 0) {
        console_open();     /* closed with the game: a stop still shows its box */
    }
#endif
    if (!pick_game(argc, argv) && !missing_screen(argc, argv)) {
        return 1;
    }
    rd_quality(config_get("render"));   /* MVC2_RDQUALITY overrides it */
#ifdef MVC2_SDL3
    view_settings();
#endif
    input_init();
    if (window == NULL && !open_window()) {
        char text[300];
        snprintf(text, sizeof text, "No window: %s", SDL_GetError());
        stop_box(text);
        return 1;
    }
    if (frames_left < 0 || getenv("MVC2_FL_AUDIO")) {
        sdl_audio_open();
    }
    host_keep_start();
    rd_begin_frame();
    if (!start_game()) {
        not_started();
        return 1;
    }
    while (!quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (test_run) {
                quit |= e.type == SDL_EVENT_QUIT;
                continue;
            }
            input_event(&e);
            if (window_key(&e)) {
                continue;
            }
            if (test_open && e.type != SDL_EVENT_QUIT) {
                int r = test_mode_keys(test_key(&e));
                if (r != TM_OPEN) {
                    test_open = 0;
                    restart_req = r == TM_RESTART;
                }
                continue;
            }
            if (e.type == SDL_EVENT_KEY_DOWN && key_scancode(&e) == SDL_SCANCODE_F2 && !e.key.repeat &&
                !menu_active() && strstr(game_title(), "Dreamcast") == NULL) {
                test_mode_open("ENTER", "ESC");
                test_open = 1;
                continue;
            }
            if (menu_event(&e)) {
                continue;
            }
            if (e.type == SDL_EVENT_QUIT || (e.type == SDL_EVENT_KEY_DOWN && key_sym(&e) == SDLK_ESCAPE)) {
                quit = 1;
            }
            if (e.type == SDL_EVENT_KEY_DOWN && key_sym(&e) == SDLK_F9 && !e.key.repeat) {
                switch_req = 1;
            }
        }
        if (test_open) {
            present_test();
            continue;
        }
        if (menu_active()) {
            present_menu();
            continue;
        }
        read_input();
        if (switch_req || restart_req) {
            int other = switch_req;
            switch_req = restart_req = 0;
            gpu_restart(1);
            sdl_save_flush();
            if (other ? !switch_game() : (host_restart(), !start_game())) {
                not_started();
                quit = 1;
                continue;
            }
            gpu_restart(0);
            rd_quality(config_get("render"));   /* reset by the switch */
            SDL_SetWindowTitle(window, game_title());
            rd_begin_frame();
        }
        {
            Uint64 start = SDL_GetPerformanceCounter(), presenting = present_ticks;
            naomi_run_frame();
            sdl_audio_frame();
            game_ticks += SDL_GetPerformanceCounter() - start - (present_ticks - presenting);
            if (getenv("MVC2_FRAMELOG")) {
                static FILE* f;
                static uint32_t last_hash;
                double ms = 1000.0 / (double)SDL_GetPerformanceFrequency();
                if (f == NULL) {
                    f = fopen(getenv("MVC2_FRAMELOG"), "w");
                    fprintf(f, "frame game_ms draw_ms gd_ms gd_kb sync req open tex_calls conv conv_ms same hash wall_ms gl_up gl_kb\n");
                }
                fprintf(f, "%u %.3f %.3f %.3f %u %u %u %u %u %u %.3f %d %08X %.3f %d %u\n", presented,
                        (double)(SDL_GetPerformanceCounter() - start - (present_ticks - presenting)) * ms,
                        (double)fl_draw * ms, fl.gd_ns / 1e6, fl.gd_bytes >> 10, fl.gd_sync, fl.gd_req, fl.gd_open,
                        fl.tex_calls, fl.tex_conv, fl.conv_ns / 1e6, fl_hash == last_hash, fl_hash, (double)SDL_GetPerformanceCounter() * ms,
                        gl_uploads(), gl_upload_kb());
                last_hash = fl_hash;
                memset(&fl, 0, sizeof fl);
            }
        }
        if ((frames_left > 0 && --frames_left == 0) || hunt_done) {
            quit = 1;
        }
    }
    save_window();
    sdl_save_flush();
    snd_close();
    sdl_audio_close();
    host_stub_report();
    if (gpu || glr) {
        close_renderer();
    }
    SDL_Quit();
    return 0;
}

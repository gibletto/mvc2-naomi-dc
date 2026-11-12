/*
 * main_sdl.c  The PC front end on SDL3 (with the GPU renderer) or SDL2: the window, the frame loop, input
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sdl_front.h"
#ifdef MVC2_SDL3
#include <SDL3/SDL_main.h>
#endif

#include "naomi_host.h"
#include "naomi_input.h"
#include "rd.h"
#ifdef MVC2_SDL3
#include "rd_gpu.h"
#endif
#include "input_sdl.h"
#include "menu_sdl.h"

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
#else
#define gpu 0
#endif
static HOST_INPUT input;

const HOST_INPUT* host_input(void) {
    return &input;
}

static unsigned presented;
static int switch_req;      /* F9 */
static int coin_req;

/* MVC2_PRESS=n:key,...: player 1 holds the key for 5 frames from frame n; coin and switch happen at n */
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
        if (sscanf(p, "%u:%15[a-z0-9]", &at, name) == 2 && presented == at && strcmp(name, "coin") == 0) {
            coin_req = 1;
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
    input.player[0] = pressed();
    input.player[1] = 0;
    input_read(&input);
    if (coin_req) {
        input.coin = 1;
        coin_req = 0;
    }
    if (key_held(SDL_SCANCODE_9)) {
        input.player[0] |= JVS_SERVICE;
    }
    input.test = key_held(SDL_SCANCODE_F2);
}

static uint32_t frame[RD_WIDTH * RD_HEIGHT];

static void put32(unsigned char* p, uint32_t v) {
    p[0] = v & 0xFF, p[1] = v >> 8 & 0xFF, p[2] = v >> 16 & 0xFF, p[3] = v >> 24;
}

/* SDL2's BMP header (BITMAPV4HEADER), so shots match whichever SDL is used */
static void save_bmp(const char* name, const uint32_t* argb) {
    unsigned char h[122];
    uint32_t row[RD_WIDTH];
    FILE* f = fopen(name, "wb");
    int y, x;
    if (f == NULL) {
        return;
    }
    memset(h, 0, sizeof h);
    h[0] = 'B', h[1] = 'M';
    put32(h + 2, sizeof h + RD_WIDTH * RD_HEIGHT * 4);
    put32(h + 10, sizeof h);
    put32(h + 14, 108);
    put32(h + 18, RD_WIDTH);
    put32(h + 22, RD_HEIGHT);
    h[26] = 1, h[28] = 32, h[30] = 3;
    put32(h + 34, RD_WIDTH * RD_HEIGHT * 4);
    put32(h + 54, 0x00FF0000), put32(h + 58, 0x0000FF00), put32(h + 62, 0x000000FF), put32(h + 66, 0xFF000000);
    put32(h + 70, 0x57696E20);              /* "Win " */
    fwrite(h, sizeof h, 1, f);
    for (y = RD_HEIGHT - 1; y >= 0; y--) {
        for (x = 0; x < RD_WIDTH; x++) {
            put32((unsigned char*)&row[x], argb[y * RD_WIDTH + x]);
        }
        fwrite(row, sizeof row, 1, f);
    }
    fclose(f);
}

/* the GPU's frame read back, at most once a frame */
#ifdef MVC2_SDL3
static int frame_read;
#endif

static void read_frame(void) {
#ifdef MVC2_SDL3
    if (gpu && !frame_read) {
        frame_read = rd_gpu_read(frame);
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
    read_frame();
    save_bmp(name, frame);
}

/* both boards: a 27 MHz pixel clock, 858 by 525 */
static double frame_rate = 27000000.0 / (858.0 * 525.0);
static Uint64 next_frame;

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
    Uint64 now, freq = SDL_GetPerformanceFrequency(), step;
    if (frame_rate <= 0) {
        return;
    }
    step = (Uint64)(freq / frame_rate);
    now = SDL_GetPerformanceCounter();
    if (next_frame == 0 || now > next_frame + step * 4) {
        next_frame = now;   /* after a stall, don't catch up */
    }
    while (now < next_frame) {
        Uint64 left_ms = (next_frame - now) * 1000 / freq;
        if (left_ms > 2) {
            SDL_Delay((Uint32)(left_ms - 2));
        }
        now = SDL_GetPerformanceCounter();
    }
    next_frame += step;
}

#ifdef MVC2_SDL3
static SDL_Texture* gpu_frame(void) {
    SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(gpu);
    SDL_GPUTexture* t;
    if (cmd == NULL) {
        return gpu_screen;
    }
    t = rd_gpu_end_frame(cmd);
    SDL_SubmitGPUCommandBuffer(cmd);
    if (gpu_screen == NULL && t != NULL) {
        SDL_PropertiesID props = SDL_CreateProperties();
        SDL_SetNumberProperty(props, SDL_PROP_TEXTURE_CREATE_FORMAT_NUMBER, SDL_PIXELFORMAT_ABGR8888);
        SDL_SetNumberProperty(props, SDL_PROP_TEXTURE_CREATE_ACCESS_NUMBER, SDL_TEXTUREACCESS_STATIC);
        SDL_SetNumberProperty(props, SDL_PROP_TEXTURE_CREATE_WIDTH_NUMBER, RD_WIDTH);
        SDL_SetNumberProperty(props, SDL_PROP_TEXTURE_CREATE_HEIGHT_NUMBER, RD_HEIGHT);
        SDL_SetPointerProperty(props, SDL_PROP_TEXTURE_CREATE_GPU_TEXTURE_POINTER, t);
        gpu_screen = SDL_CreateTextureWithProperties(renderer, props);
        SDL_DestroyProperties(props);
        if (gpu_screen != NULL) {
            SDL_SetTextureScaleMode(gpu_screen, SDL_SCALEMODE_NEAREST);
        }
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

static void show(SDL_Texture* t) {
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, t, NULL, NULL);
    if (times_on && times_tex != NULL) {
        render_at(renderer, times_tex, 4, 4, TIMES_W, TIMES_H);
    }
    pace();
    SDL_RenderPresent(renderer);
}

void host_present(void) {
    SDL_Texture* t = screen;
    Uint64 start = SDL_GetPerformanceCounter();
#ifdef MVC2_SDL3
    if (gpu) {
        t = gpu_frame();
        frame_read = 0;
    } else
#endif
    {
        rd_end_frame(frame, RD_WIDTH);
    }
    rd_begin_frame();
    presented++;
    screenshot();
    if (!gpu) {
        SDL_UpdateTexture(screen, NULL, frame, RD_WIDTH * 4);
    }
    draw_ticks += SDL_GetPerformanceCounter() - start;
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
static int scaling;
static int keep_settings;

static void set_scaling(void) {
    int w, h, small;
#ifdef MVC2_SDL3
    static const SDL_RendererLogicalPresentation mode[] = { SDL_LOGICAL_PRESENTATION_LETTERBOX,
                                                           SDL_LOGICAL_PRESENTATION_INTEGER_SCALE,
                                                           SDL_LOGICAL_PRESENTATION_STRETCH };
    SDL_GetCurrentRenderOutputSize(renderer, &w, &h);
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
/* when a step fails, nothing is kept and the software renderer draws */
static void open_gpu(void) {
    gpu = SDL_CreateGPUDevice(RD_GPU_SHADER_FORMATS | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_SPIRV |
                                  SDL_GPU_SHADERFORMAT_MSL, false, NULL);
    if (gpu != NULL && rd_gpu_init(gpu)) {
        renderer = SDL_CreateGPURenderer(gpu, window);
        if (renderer != NULL) {
            host_log("renderer: gpu (%s)", SDL_GetGPUDeviceDriver(gpu));
            return;
        }
        rd_gpu_quit();
    }
    host_log("renderer: no gpu (%s), soft", SDL_GetError());
    if (gpu != NULL) {
        SDL_DestroyGPUDevice(gpu);
        gpu = NULL;
    }
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

static int make_renderer(int want_gpu) {
#ifdef MVC2_SDL3
    if (want_gpu) {
        open_gpu();
    }
    if (renderer == NULL) {
        renderer = SDL_CreateRenderer(window, NULL);
    }
    if (renderer == NULL) {
        return 0;
    }
    SDL_SetRenderVSync(renderer, 1);
#else
    (void)want_gpu;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        return 0;
    }
#endif
    screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, RD_WIDTH, RD_HEIGHT);
#ifdef MVC2_SDL3
    SDL_SetTextureScaleMode(screen, SDL_SCALEMODE_NEAREST);     /* SDL3's default is linear */
#endif
    set_scaling();
    return screen != NULL;
}

int renderer_gpu(void) {
    return gpu != 0;
}

int renderer_set_gpu(int on) {
#ifdef MVC2_SDL3
    SDL_Texture** own[] = { &times_tex, &screen, &gpu_screen };
    size_t i;
    if ((gpu != NULL) == on) {
        return on;
    }
    read_frame();
    for (i = 0; i < sizeof own / sizeof own[0]; i++) {
        if (*own[i] != NULL) {
            SDL_DestroyTexture(*own[i]);
            *own[i] = NULL;
        }
    }
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    if (gpu != NULL) {
        rd_gpu_quit();
        SDL_DestroyGPUDevice(gpu);
        gpu = NULL;
    }
    if (!make_renderer(on)) {
        host_log("no renderer: %s", SDL_GetError());
    }
    frame_read = 1;     /* frame holds the last picture */
    return gpu != NULL;
#else
    (void)on;
    return 0;
#endif
}

/* sizes in pixels, so a whole multiple stays sharp at any display scaling */
static int open_window(void) {
    const char* size = config_get("window");
    const char* full = config_get("fullscreen");
    const char* scale = config_get("scale");
    int w, h, i;
    keep_settings = getenv("MVC2_FRAMES") == NULL;
    if (!keep_settings) {
        full = NULL;
    }
    if (size == NULL || sscanf(size, "%dx%d", &w, &h) != 2) {
        first_size(&w, &h);
    }
    w = w < RD_WIDTH / 2 ? RD_WIDTH / 2 : w;
    h = h < RD_HEIGHT / 2 ? RD_HEIGHT / 2 : h;
#ifdef MVC2_SDL3
    window = SDL_CreateWindow(game_title(), w, h,
                              SDL_WINDOW_RESIZABLE | (full != NULL && strcmp(full, "yes") == 0 ? SDL_WINDOW_FULLSCREEN : 0));
    if (window == NULL) {
        return 0;
    }
    SDL_SetWindowMinimumSize(window, RD_WIDTH / 2, RD_HEIGHT / 2);
#else
    window = SDL_CreateWindow(game_title(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h,
                              SDL_WINDOW_RESIZABLE |
                                  (full != NULL && strcmp(full, "yes") == 0 ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0));
    if (window == NULL) {
        return 0;
    }
    SDL_SetWindowMinimumSize(window, RD_WIDTH / 2, RD_HEIGHT / 2);
#endif
    for (i = 0; i < 3; i++) {
        if (scale != NULL && strcmp(scale, scalings[i]) == 0) {
            scaling = i;
        }
    }
    return make_renderer(config_get("renderer") != NULL && strcmp(config_get("renderer"), "gpu") == 0);
}

/* 1 when the window took the event */
static int window_key(const SDL_Event* e) {
    if (window_resized(e)) {
        set_scaling();
        return 0;
    }
    if (e->type != SDL_EVENT_KEY_DOWN || e->key.repeat) {
        return 0;
    }
    if (key_sym(e) == SDLK_F11 || (key_sym(e) == SDLK_RETURN && key_alt(e))) {
        int full = !(SDL_GetWindowFlags(window) & FULLSCREEN);
#ifdef MVC2_SDL3
        SDL_SetWindowFullscreen(window, full);
#else
        SDL_SetWindowFullscreen(window, full ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
#endif
        if (keep_settings) {
            config_set("fullscreen", full ? "yes" : "no");
        }
        return 1;
    }
    if (key_sym(e) == SDLK_F3) {
        times_on = !times_on;
        times_start = 0;
        return 1;
    }
    if (key_sym(e) == SDLK_F10) {
        scaling = (scaling + 1) % 3;
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

int main(int argc, char** argv) {
    int quit = 0;
    long frames_left = getenv("MVC2_FRAMES") ? atol(getenv("MVC2_FRAMES")) : -1;

    if (!naomi_map_ram()) {
        fprintf(stderr, "could not map main RAM at %08X\n", NAOMI_RAM_BASE);
        return 1;
    }
    host_divzero_install();
    pace_init();
#ifdef MVC2_SDL3
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD)) {
#else
    SDL_SetHint(SDL_HINT_WINDOWS_DPI_AWARENESS, "permonitorv2");
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER) != 0) {
#endif
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
    if (!pick_game(argc, argv) && !missing_screen(argc, argv)) {
        return 1;
    }
    rd_quality(config_get("render"));   /* MVC2_RDQUALITY overrides it */
    input_init();
    if (window == NULL && !open_window()) {
        fprintf(stderr, "SDL: %s\n", SDL_GetError());
        return 1;
    }
    if (frames_left < 0) {
        sdl_audio_open();
    }
    host_keep_start();
    rd_begin_frame();
    if (!start_game()) {
        return 1;
    }
    while (!quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            input_event(&e);
            if (window_key(&e) || menu_event(&e)) {
                continue;
            }
            if (e.type == SDL_EVENT_QUIT || (e.type == SDL_EVENT_KEY_DOWN && key_sym(&e) == SDLK_ESCAPE)) {
                quit = 1;
            }
            if (e.type == SDL_EVENT_KEY_DOWN && key_sym(&e) == SDLK_F9 && !e.key.repeat) {
                switch_req = 1;
            }
        }
        if (menu_active()) {
            present_menu();
            continue;
        }
        read_input();
        if (switch_req) {
            switch_req = 0;
            gpu_restart(1);
            if (!switch_game()) {
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
        }
        if (frames_left > 0 && --frames_left == 0) {
            quit = 1;
        }
    }
    save_window();
    snd_close();
    sdl_audio_close();
    host_stub_report();
#ifdef MVC2_SDL3
    if (gpu) {
        SDL_DestroyRenderer(renderer);
        rd_gpu_quit();
        SDL_DestroyGPUDevice(gpu);
    }
#endif
    SDL_Quit();
    return 0;
}

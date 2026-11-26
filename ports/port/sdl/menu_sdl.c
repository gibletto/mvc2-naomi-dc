/*
 * menu_sdl.c  The controls screen (F1): each player's keys and buttons, the renderer, full screen and exit, over the paused game
 */

#include <stdio.h>
#include <string.h>

#include "sdl_front.h"

#include "naomi_host.h"
#include "rd.h"
#include "input_sdl.h"
#include "menu_sdl.h"
#include "menu_font.h"

enum { SCALE = 2, CW = 6 * SCALE, CH = 19, LEFT = 32, TOP = 16 };     /* 24 lines */

#define WHITE 0xFFFFFFFFu
#define YELLOW 0xFFFFE040u
#define GREY 0xFFA0A0A0u

static uint32_t* dst;
static int dst_w, dst_h;

static void draw_text(int col, int line, const char* s, uint32_t color) {
    int x = LEFT + col * CW;
    for (; *s != 0 && x + CW <= dst_w - LEFT / 2; s++, x += CW) {
        char c[2] = { *s, 0 };
        menu_text(dst, dst_w, dst_h, x, TOP + line * CH, c, color, SCALE);
    }
}

static const char* const labels[ACTIONS] = {
    "UP", "DOWN", "LEFT", "RIGHT", "LIGHT PUNCH", "HEAVY PUNCH", "ASSIST 1",
    "LIGHT KICK", "HEAVY KICK", "ASSIST 2", "START", "COIN",
};

enum { ROW_RENDERER = ACTIONS, ROW_PICTURE, ROW_SPRITES, ROW_HUD, ROW_FULL, ROW_DEFAULTS, ROW_DONE, ROW_EXIT, ROWS };

static int row_line(int a) {
    return 2 + a;
}

static const char* const pictures[] = { "exact", "fast", "lite+fast", "fast+half" };
enum { PICTURES = sizeof pictures / sizeof pictures[0] };

static const char* picture(void) {
    return config_get("render") != NULL ? config_get("render") : "exact";
}

/* the GPU's: its resolution and aa */
static const struct {
    const char *name, *resolution, *aa;
} gpu_pictures[] = {
    { "auto", "auto", "1" }, { "auto, aa 2x", "auto", "2" }, { "auto, aa 3x", "auto", "3" }, { "auto, aa 4x", "auto", "4" },
    { "4.2x", "4.2", "1" }, { "1x, the arcade's", "1", "1" },
};
enum { GPU_PICTURES = sizeof gpu_pictures / sizeof gpu_pictures[0] };

static int gpu_picture(void) {
    const char* r = config_get("resolution") != NULL ? config_get("resolution") : "auto";
    const char* a = config_get("aa") != NULL ? config_get("aa") : "1";
    int i;
    for (i = 0; i < GPU_PICTURES; i++) {
        if (strcmp(r, gpu_pictures[i].resolution) == 0 && strcmp(a, gpu_pictures[i].aa) == 0) {
            return i;
        }
    }
    return -1;
}

/* texel's choices take its grid too */
static const char* const sprite_modes[] = { "sharp", "texel", "texel, own grids", "smooth", "pixel", "arcade" };
enum { SPRITE_MODES = sizeof sprite_modes / sizeof sprite_modes[0], OBJECT_GRID = 2 };

static int sprites(void) {
    const char* s = config_get("sprites") != NULL ? config_get("sprites") : "sharp";
    const char* g = config_get("texel_grid");
    int i;
    for (i = 0; i < SPRITE_MODES; i++) {
        if (strcmp(s, sprite_modes[i]) == 0) {
            return i == 1 && g != NULL && strcmp(g, "object") == 0 ? OBJECT_GRID : i;
        }
    }
    return 0;
}

static void change_sprites(int way) {
    int now = (sprites() + way + SPRITE_MODES) % SPRITE_MODES;
    config_set("sprites", now == OBJECT_GRID ? "texel" : sprite_modes[now]);
    config_set("texel_grid", now == OBJECT_GRID ? "object" : "shared");
#ifdef MVC2_SDL3
    view_settings();
#endif
}

static const char* const hud_modes[] = { "arcade", "smooth", "sharp" };
enum { HUD_MODES = sizeof hud_modes / sizeof hud_modes[0] };

static const char* hud(void) {
    return config_get("hud") != NULL ? config_get("hud") : "arcade";
}

static void change_hud(int way) {
    int i, now = 0;
    for (i = 0; i < HUD_MODES; i++) {
        if (strcmp(hud(), hud_modes[i]) == 0) {
            now = i;
        }
    }
    config_set("hud", hud_modes[(now + way + HUD_MODES) % HUD_MODES]);
#ifdef MVC2_SDL3
    view_settings();
#endif
}

static void change_picture(int way) {
    int i, now = -1;
#ifdef MVC2_SDL3
    if (renderer_kind() != RENDERER_SOFT) {
        now = gpu_picture();
        now = now < 0 ? 0 : (now + way + GPU_PICTURES) % GPU_PICTURES;
        config_set("resolution", gpu_pictures[now].resolution);
        config_set("aa", gpu_pictures[now].aa);
        view_settings();
        return;
    }
#endif
    for (i = 0; i < PICTURES; i++) {
        if (strcmp(picture(), pictures[i]) == 0) {
            now = i;
        }
    }
    now = now < 0 ? 0 : (now + way + PICTURES) % PICTURES;
    config_set("render", pictures[now]);
    rd_quality(pictures[now]);
}

static const char* renderer_note;

/* GPU, GL and SOFT in turn (dir 1 or -1); the GPU falls to OpenGL and OpenGL to SOFT where they can't start */
static void change_renderer(int dir) {
#ifdef MVC2_SDL3
    int want = (renderer_kind() + (dir < 0 ? 1 : 2)) % 3;
    int now = renderer_set(want);
    config_set("renderer", now == RENDERER_SOFT ? "soft" : want == RENDERER_GL ? "gl" : "gpu");
    renderer_note = now == want           ? NULL
                    : now == RENDERER_GL  ? "NO VULKAN OR D3D12"
                    : want == RENDERER_GL ? "NO OPENGL"
                                          : "NO GPU OR OPENGL";
#else
    (void)dir;
    renderer_note = "THE SDL3 BUILD'S";
#endif
}

static const char* setting(int a, char* buf, size_t size) {
    size_t i;
    if (a == ROW_RENDERER) {
        static const char* const kinds[] = { "SOFT", "GL", "GPU" };
        snprintf(buf, size, "%s%s%s", kinds[renderer_kind()], renderer_note != NULL ? "  " : "",
                 renderer_note != NULL ? renderer_note : "");
    } else if (a == ROW_SPRITES) {
        snprintf(buf, size, "%s%s", sprite_modes[sprites()], renderer_gpu() ? "" : "  (GPU ONLY)");
    } else if (a == ROW_HUD) {
        snprintf(buf, size, "%s%s", hud(), renderer_gpu() ? "" : "  (GPU ONLY)");
    } else if (a == ROW_FULL) {
        static const char* const kinds[] = { "OFF", "DESKTOP", "EXCLUSIVE" };
        snprintf(buf, size, "%s", kinds[fullscreen_kind()]);
    } else if (renderer_kind() != RENDERER_SOFT) {
        /* OpenGL: the resolution, not the aa */
        int i = gpu_picture(), w = 0, h = 0;
        int aa_off = renderer_kind() == RENDERER_GL && i >= 0 && strcmp(gpu_pictures[i].aa, "1") != 0;
#ifdef MVC2_SDL3
        view_drawn(&w, &h);
#endif
        snprintf(buf, size, "%s: %d by %d%s", i >= 0 ? gpu_pictures[i].name : "mvc2.cfg's", w, h,
                 aa_off ? "  (AA GPU ONLY)" : "");
    } else {
        snprintf(buf, size, "%s", picture());
    }
    for (i = 0; buf[i] != 0; i++) {
        buf[i] = buf[i] >= 'a' && buf[i] <= 'z' ? buf[i] - 'a' + 'A' : buf[i];
    }
    return buf;
}

static int open_, player, row, waiting;
static int nav_axis[2];     /* so a push moves once */

int menu_active(void) {
    return open_;
}

/* a controller's buttons by their usual letters */
static const char* shown(const BINDING* b, char* buf, size_t size) {
    static const char* const buttons[SDL_GAMEPAD_BUTTON_COUNT] = {
        "A", "B", "X", "Y", "BACK", "GUIDE", "START", "L STICK", "R STICK", "LB", "RB",
        "D-UP", "D-DOWN", "D-LEFT", "D-RIGHT",
    };
    static const char* const axes[SDL_GAMEPAD_AXIS_COUNT][2] = {
        { "STICK LEFT", "STICK RIGHT" }, { "STICK UP", "STICK DOWN" },
        { "R STICK LEFT", "R STICK RIGHT" }, { "R STICK UP", "R STICK DOWN" }, { "LT", "LT" }, { "RT", "RT" },
    };
    if (b->type == BIND_KEY) {
        snprintf(buf, size, "%s", SDL_GetScancodeName(b->code));
    } else if (b->type == BIND_BUTTON && b->code < SDL_GAMEPAD_BUTTON_COUNT && buttons[b->code] != NULL) {
        snprintf(buf, size, "PAD %s", buttons[b->code]);
    } else if (b->type == BIND_AXIS && b->code < SDL_GAMEPAD_AXIS_COUNT) {
        snprintf(buf, size, "PAD %s", axes[b->code][b->dir > 0]);
    } else {
        binding_name(b, buf, size);
    }
    return buf;
}

void menu_draw(uint32_t* frame, int w, int h) {
    char line[200], one[60];
    SDL_Gamepad* pad = input_pad(player);
    int i, a, n;
    dst = frame, dst_w = w, dst_h = h;
    for (i = 0; i < w * h; i++) {
        frame[i] = 0xFF000000u | ((frame[i] >> 2) & 0x3F3F3Fu);
    }
    snprintf(line, sizeof line, "CONTROLS     < PLAYER %d >", player + 1);
    draw_text(0, 0, line, WHITE);
    draw_text(0, 1, pad != NULL ? SDL_GetGamepadName(pad) : "NO CONTROLLER", GREY);
    for (a = 0; a < ROWS; a++) {
        uint32_t color = a == row ? YELLOW : WHITE;
        if (a == ROW_DEFAULTS) {
            draw_text(0, row_line(a), "SET TO DEFAULT", color);
            continue;
        }
        if (a == ROW_DONE || a == ROW_EXIT) {
            draw_text(0, row_line(a), a == ROW_DONE ? "DONE" : "EXIT THE GAME", color);
            continue;
        }
        if (a >= ROW_RENDERER && a <= ROW_FULL) {
            draw_text(0, row_line(a),
                      a == ROW_RENDERER ? "RENDERER" : a == ROW_PICTURE ? "PICTURE" : a == ROW_SPRITES ? "SPRITES"
                      : a == ROW_HUD ? "HUD" : "FULL SCREEN", color);
            snprintf(line, sizeof line, "< %s >", setting(a, one, sizeof one));
            draw_text(13, row_line(a), line, color);
            continue;
        }
        draw_text(0, row_line(a), labels[a], color);
        if (waiting && a == row) {
            draw_text(13, row_line(a), "PRESS A KEY OR BUTTON (ESC: NO CHANGE)", YELLOW);
            continue;
        }
        n = 0;
        line[0] = 0;
        for (i = 0; i < MAX_BINDINGS && bindings[player][a][i].type != BIND_NONE; i++) {
            n += snprintf(line + n, sizeof line - n, "%s%s", i > 0 ? ", " : "",
                          shown(&bindings[player][a][i], one, sizeof one));
        }
        draw_text(13, row_line(a), n > 0 ? line : "-", color);
    }
    draw_text(0, 22, "UP/DOWN CHOOSE   LEFT/RIGHT PLAYER OR SETTING", GREY);
    draw_text(0, 23, "ENTER/A SET   DEL/X DEFAULT   ESC/B DONE", GREY);
}

static void close_menu(void) {
    open_ = 0;
    input_save(0);
    input_save(1);
}

static void take(const BINDING* b) {
    input_set(player, row, b);
    waiting = 0;
}

static void act(int up, int down, int left, int right, int enter, int reset, int back) {
    int a;
    if (up) {
        row = (row + ROWS - 1) % ROWS;
    } else if (down) {
        row = (row + 1) % ROWS;
    } else if ((left || right) && row == ROW_PICTURE) {
        change_picture(right ? 1 : -1);
    } else if ((left || right || enter) && row == ROW_SPRITES) {
        change_sprites(left ? -1 : 1);
    } else if ((left || right || enter) && row == ROW_HUD) {
        change_hud(left ? -1 : 1);
    } else if ((left || right || enter) && row == ROW_RENDERER) {
        change_renderer(left ? -1 : 1);
    } else if ((left || right || enter) && row == ROW_FULL) {
        fullscreen_set((fullscreen_kind() + (left ? 2 : 1)) % 3);     /* off, desktop, exclusive */
    } else if (enter && row == ROW_PICTURE) {
        change_picture(1);
    } else if (left || right) {
        player ^= 1;
    } else if (back) {
        close_menu();
    } else if (enter && row == ROW_DONE) {
        close_menu();
    } else if (enter && row == ROW_EXIT) {
        SDL_Event quit = { SDL_EVENT_QUIT };
        close_menu();
        SDL_PushEvent(&quit);
    } else if (enter && row == ROW_DEFAULTS) {
        for (a = 0; a < ACTIONS; a++) {
            input_defaults(player, a);
        }
    } else if (enter) {
        waiting = 1;
    } else if (reset && row < ACTIONS) {
        input_defaults(player, row);
    }
}

/* 1 when it took the event */
int menu_event(const SDL_Event* e) {
    if (!open_) {
        int pl;
        if (e->type == SDL_EVENT_KEY_DOWN && key_scancode(e) == SDL_SCANCODE_F1 && !e->key.repeat) {
            open_ = 1, player = 0, row = 0, waiting = 0;
            return 1;
        }
        if (e->type == SDL_EVENT_GAMEPAD_BUTTON_DOWN && (pl = input_pad_player(e->gbutton.which)) >= 0 &&
            input_menu_combo(pl)) {
            open_ = 1, player = pl, row = 0, waiting = 0;
            return 1;
        }
        return 0;
    }
    if (waiting) {
        BINDING b = { BIND_NONE, 0, 0 };
        if (e->type == SDL_EVENT_KEY_DOWN && !e->key.repeat) {
            if (key_scancode(e) == SDL_SCANCODE_ESCAPE) {
                waiting = 0;
                return 1;
            }
            b.type = BIND_KEY, b.code = key_scancode(e);
        } else if (e->type == SDL_EVENT_GAMEPAD_BUTTON_DOWN) {
            b.type = BIND_BUTTON, b.code = e->gbutton.button;
        } else if (e->type == SDL_EVENT_GAMEPAD_AXIS_MOTION && (e->gaxis.value > 24000 || e->gaxis.value < -24000)) {
            b.type = BIND_AXIS, b.code = e->gaxis.axis, b.dir = e->gaxis.value < 0 ? -1 : 1;
        }
        if (b.type != BIND_NONE) {
            take(&b);
        }
        return e->type != SDL_EVENT_QUIT;
    }
    if (e->type == SDL_EVENT_KEY_DOWN) {
        SDL_Scancode k = key_scancode(e);
        act(k == SDL_SCANCODE_UP, k == SDL_SCANCODE_DOWN, k == SDL_SCANCODE_LEFT, k == SDL_SCANCODE_RIGHT,
            (k == SDL_SCANCODE_RETURN || k == SDL_SCANCODE_KP_ENTER) && !e->key.repeat,
            k == SDL_SCANCODE_DELETE || k == SDL_SCANCODE_BACKSPACE,
            (k == SDL_SCANCODE_ESCAPE || k == SDL_SCANCODE_F1) && !e->key.repeat);
    } else if (e->type == SDL_EVENT_GAMEPAD_BUTTON_DOWN) {
        int b = e->gbutton.button;
        act(b == SDL_GAMEPAD_BUTTON_DPAD_UP, b == SDL_GAMEPAD_BUTTON_DPAD_DOWN,
            b == SDL_GAMEPAD_BUTTON_DPAD_LEFT, b == SDL_GAMEPAD_BUTTON_DPAD_RIGHT, b == SDL_GAMEPAD_BUTTON_SOUTH,
            b == SDL_GAMEPAD_BUTTON_WEST, b == SDL_GAMEPAD_BUTTON_EAST);
    } else if (e->type == SDL_EVENT_GAMEPAD_AXIS_MOTION &&
               (e->gaxis.axis == SDL_GAMEPAD_AXIS_LEFTX || e->gaxis.axis == SDL_GAMEPAD_AXIS_LEFTY)) {
        int way = e->gaxis.value > 24000 ? 1 : e->gaxis.value < -24000 ? -1 : 0;
        int* last = &nav_axis[e->gaxis.axis == SDL_GAMEPAD_AXIS_LEFTY];
        if (way != *last && way != 0) {
            int y = e->gaxis.axis == SDL_GAMEPAD_AXIS_LEFTY;
            act(y && way < 0, y && way > 0, !y && way < 0, !y && way > 0, 0, 0, 0);
        }
        *last = way;
    }
    return e->type != SDL_EVENT_QUIT;
}

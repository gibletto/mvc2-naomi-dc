/*
 * menu_sdl.c  The controls screen (F1): each player's keys and buttons, and the renderer, over the paused game
 */

#include <stdio.h>
#include <string.h>

#include "sdl_front.h"

#include "naomi_host.h"
#include "rd.h"
#include "input_sdl.h"
#include "menu_sdl.h"

/* 5 by 7: rows from the top, bit 4 the left column */
static const struct {
    char c;
    unsigned char row[7];
} font[] = {
    { '0', { 0x0E, 0x11, 0x13, 0x15, 0x19, 0x11, 0x0E } }, { '1', { 0x04, 0x0C, 0x04, 0x04, 0x04, 0x04, 0x0E } },
    { '2', { 0x0E, 0x11, 0x01, 0x02, 0x04, 0x08, 0x1F } }, { '3', { 0x1F, 0x02, 0x04, 0x02, 0x01, 0x11, 0x0E } },
    { '4', { 0x02, 0x06, 0x0A, 0x12, 0x1F, 0x02, 0x02 } }, { '5', { 0x1F, 0x10, 0x1E, 0x01, 0x01, 0x11, 0x0E } },
    { '6', { 0x06, 0x08, 0x10, 0x1E, 0x11, 0x11, 0x0E } }, { '7', { 0x1F, 0x01, 0x02, 0x04, 0x08, 0x08, 0x08 } },
    { '8', { 0x0E, 0x11, 0x11, 0x0E, 0x11, 0x11, 0x0E } }, { '9', { 0x0E, 0x11, 0x11, 0x0F, 0x01, 0x02, 0x0C } },
    { 'A', { 0x0E, 0x11, 0x11, 0x11, 0x1F, 0x11, 0x11 } }, { 'B', { 0x1E, 0x11, 0x11, 0x1E, 0x11, 0x11, 0x1E } },
    { 'C', { 0x0E, 0x11, 0x10, 0x10, 0x10, 0x11, 0x0E } }, { 'D', { 0x1C, 0x12, 0x11, 0x11, 0x11, 0x12, 0x1C } },
    { 'E', { 0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x1F } }, { 'F', { 0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x10 } },
    { 'G', { 0x0E, 0x11, 0x10, 0x17, 0x11, 0x11, 0x0F } }, { 'H', { 0x11, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11 } },
    { 'I', { 0x0E, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0E } }, { 'J', { 0x07, 0x02, 0x02, 0x02, 0x02, 0x12, 0x0C } },
    { 'K', { 0x11, 0x12, 0x14, 0x18, 0x14, 0x12, 0x11 } }, { 'L', { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1F } },
    { 'M', { 0x11, 0x1B, 0x15, 0x15, 0x11, 0x11, 0x11 } }, { 'N', { 0x11, 0x11, 0x19, 0x15, 0x13, 0x11, 0x11 } },
    { 'O', { 0x0E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E } }, { 'P', { 0x1E, 0x11, 0x11, 0x1E, 0x10, 0x10, 0x10 } },
    { 'Q', { 0x0E, 0x11, 0x11, 0x11, 0x15, 0x12, 0x0D } }, { 'R', { 0x1E, 0x11, 0x11, 0x1E, 0x14, 0x12, 0x11 } },
    { 'S', { 0x0F, 0x10, 0x10, 0x0E, 0x01, 0x01, 0x1E } }, { 'T', { 0x1F, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04 } },
    { 'U', { 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E } }, { 'V', { 0x11, 0x11, 0x11, 0x11, 0x11, 0x0A, 0x04 } },
    { 'W', { 0x11, 0x11, 0x11, 0x15, 0x15, 0x15, 0x0A } }, { 'X', { 0x11, 0x11, 0x0A, 0x04, 0x0A, 0x11, 0x11 } },
    { 'Y', { 0x11, 0x11, 0x11, 0x0A, 0x04, 0x04, 0x04 } }, { 'Z', { 0x1F, 0x01, 0x02, 0x04, 0x08, 0x10, 0x1F } },
    { '-', { 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00 } }, { '+', { 0x00, 0x04, 0x04, 0x1F, 0x04, 0x04, 0x00 } },
    { ':', { 0x00, 0x0C, 0x0C, 0x00, 0x0C, 0x0C, 0x00 } }, { '.', { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C } },
    { ',', { 0x00, 0x00, 0x00, 0x00, 0x0C, 0x04, 0x08 } }, { '/', { 0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x00 } },
    { '(', { 0x02, 0x04, 0x08, 0x08, 0x08, 0x04, 0x02 } }, { ')', { 0x08, 0x04, 0x02, 0x02, 0x02, 0x04, 0x08 } },
    { '<', { 0x02, 0x04, 0x08, 0x10, 0x08, 0x04, 0x02 } }, { '>', { 0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08 } },
    { '?', { 0x0E, 0x11, 0x01, 0x02, 0x04, 0x00, 0x04 } }, { '!', { 0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x04 } },
    { '=', { 0x00, 0x00, 0x1F, 0x00, 0x1F, 0x00, 0x00 } }, { '_', { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F } },
    { '[', { 0x0E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x0E } }, { ']', { 0x0E, 0x02, 0x02, 0x02, 0x02, 0x02, 0x0E } },
    { '\'', { 0x0C, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00 } }, { ';', { 0x00, 0x0C, 0x0C, 0x00, 0x0C, 0x04, 0x08 } },
    { '*', { 0x00, 0x04, 0x15, 0x0E, 0x15, 0x04, 0x00 } }, { '#', { 0x0A, 0x0A, 0x1F, 0x0A, 0x1F, 0x0A, 0x0A } },
    { '\\', { 0x00, 0x10, 0x08, 0x04, 0x02, 0x01, 0x00 } }, { '"', { 0x0A, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00 } },
};

enum { SCALE = 2, CW = 6 * SCALE, CH = 10 * SCALE, LEFT = 32, TOP = 24 };

#define WHITE 0xFFFFFFFFu
#define YELLOW 0xFFFFE040u
#define GREY 0xFFA0A0A0u

static uint32_t* dst;
static int dst_w, dst_h;

static void draw_char(int x, int y, char c, uint32_t color, int scale) {
    size_t i;
    int r, b, sy, sx;
    if (c >= 'a' && c <= 'z') {
        c -= 'a' - 'A';
    }
    for (i = 0; i < sizeof font / sizeof font[0] && font[i].c != c; i++) {
    }
    if (i == sizeof font / sizeof font[0]) {
        return;
    }
    for (r = 0; r < 7; r++) {
        for (b = 0; b < 5; b++) {
            if (!(font[i].row[r] & (0x10 >> b))) {
                continue;
            }
            for (sy = 0; sy < scale; sy++) {
                for (sx = 0; sx < scale; sx++) {
                    int px = x + b * scale + sx, py = y + r * scale + sy;
                    if (px >= 0 && px < dst_w && py >= 0 && py < dst_h) {
                        dst[py * dst_w + px] = color;
                    }
                }
            }
        }
    }
}

static void draw_text(int col, int line, const char* s, uint32_t color) {
    int x = LEFT + col * CW;
    for (; *s != 0 && x + CW <= dst_w - LEFT / 2; s++, x += CW) {
        draw_char(x, TOP + line * CH, *s, color, SCALE);
    }
}

/* each glyph 6 by 10 pixels, times scale */
void menu_text(uint32_t* buf, int w, int h, int x, int y, const char* s, uint32_t color, int scale) {
    dst = buf, dst_w = w, dst_h = h;
    for (; *s != 0; s++, x += 6 * scale) {
        draw_char(x, y, *s, color, scale);
    }
}

static const char* const labels[ACTIONS] = {
    "UP", "DOWN", "LEFT", "RIGHT", "LIGHT PUNCH", "HEAVY PUNCH", "ASSIST 1",
    "LIGHT KICK", "HEAVY KICK", "ASSIST 2", "START", "COIN",
};

enum { ROW_RENDERER = ACTIONS, ROW_PICTURE, ROW_DEFAULTS, ROW_DONE, ROWS };

static int row_line(int a) {
    return 3 + a + (a >= ROW_RENDERER);
}

static const char* const pictures[] = { "exact", "fast", "fast+half" };
enum { PICTURES = sizeof pictures / sizeof pictures[0] };

static const char* picture(void) {
    return config_get("render") != NULL ? config_get("render") : "exact";
}

static void change_picture(int way) {
    int i, now = -1;
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

static void change_renderer(void) {
#ifdef MVC2_SDL3
    int want = !renderer_gpu();
    int now = renderer_set_gpu(want);
    config_set("renderer", now ? "gpu" : "soft");
    renderer_note = now == want ? NULL : "THE GPU COULD NOT START";
#else
    renderer_note = "THE SDL3 BUILD'S";
#endif
}

static const char* setting(int a, char* buf, size_t size) {
    size_t i;
    if (a == ROW_RENDERER) {
        snprintf(buf, size, "%s%s%s", renderer_gpu() ? "GPU" : "SOFT", renderer_note != NULL ? "  " : "",
                 renderer_note != NULL ? renderer_note : "");
    } else {
        snprintf(buf, size, "%s%s", picture(), renderer_gpu() ? "  (SOFT ONLY)" : "");
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
            draw_text(0, row_line(a), "ALL BACK AS AT FIRST", color);
            continue;
        }
        if (a == ROW_DONE) {
            draw_text(0, row_line(a), "DONE", color);
            continue;
        }
        if (a == ROW_RENDERER || a == ROW_PICTURE) {
            draw_text(0, row_line(a), a == ROW_RENDERER ? "RENDERER" : "PICTURE", color);
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
    draw_text(0, 21, "UP/DOWN CHOOSE   LEFT/RIGHT PLAYER OR SETTING", GREY);
    draw_text(0, 22, "ENTER/A SET   DEL/X AS AT FIRST   ESC/B DONE", GREY);
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
    } else if ((left || right || enter) && row == ROW_RENDERER) {
        change_renderer();
    } else if (enter && row == ROW_PICTURE) {
        change_picture(1);
    } else if (left || right) {
        player ^= 1;
    } else if (back) {
        close_menu();
    } else if (enter && row == ROW_DONE) {
        close_menu();
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

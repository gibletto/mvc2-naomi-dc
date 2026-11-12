/*
 * input_sdl.c  The players' controls: the keyboard and up to two game controllers, kept in mvc2.cfg
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sdl_front.h"

#include "naomi_host.h"
#include "naomi_input.h"
#include "input_sdl.h"

const INPUT_ACTION input_actions[ACTIONS] = {
    { "up", JVS_UP },      { "down", JVS_DOWN }, { "left", JVS_LEFT }, { "right", JVS_RIGHT },
    { "b1", JVS_B1 },      { "b2", JVS_B2 },     { "b3", JVS_B3 },     { "b4", JVS_B4 },
    { "b5", JVS_B5 },      { "b6", JVS_B6 },     { "start", JVS_START }, { "coin", 0 },
};

BINDING bindings[2][ACTIONS][MAX_BINDINGS];

static SDL_Gamepad* pads[2];

#define KEY(k) { BIND_KEY, SDL_SCANCODE_##k, 0 }
#define BUTTON(b) { BIND_BUTTON, SDL_GAMEPAD_BUTTON_##b, 0 }
#define AXIS(a, dir) { BIND_AXIS, SDL_GAMEPAD_AXIS_##a, dir }

/* shoulders and triggers last, so an arcade stick's two rows match the cabinet's */
static const BINDING pad_defaults[ACTIONS][4] = {
    { BUTTON(DPAD_UP), AXIS(LEFTY, -1) },
    { BUTTON(DPAD_DOWN), AXIS(LEFTY, 1) },
    { BUTTON(DPAD_LEFT), AXIS(LEFTX, -1) },
    { BUTTON(DPAD_RIGHT), AXIS(LEFTX, 1) },
    { BUTTON(WEST) },
    { BUTTON(NORTH) },
    { BUTTON(RIGHT_SHOULDER), BUTTON(LEFT_SHOULDER) },
    { BUTTON(SOUTH) },
    { BUTTON(EAST) },
    { AXIS(RIGHT_TRIGGER, 1), AXIS(LEFT_TRIGGER, 1) },
    { BUTTON(START) },
    { BUTTON(BACK) },
};

static const BINDING key_defaults[2][ACTIONS] = {
    { KEY(UP), KEY(DOWN), KEY(LEFT), KEY(RIGHT), KEY(A), KEY(S), KEY(D), KEY(Z), KEY(X), KEY(C), KEY(1), KEY(5) },
    { KEY(I), KEY(K), KEY(J), KEY(L), KEY(R), KEY(T), KEY(Y), KEY(F), KEY(G), KEY(H), KEY(2), { BIND_NONE } },
};

void input_defaults(int pl, int action) {
    int i, n = 0;
    memset(bindings[pl][action], 0, sizeof bindings[pl][action]);
    if (key_defaults[pl][action].type != BIND_NONE) {
        bindings[pl][action][n++] = key_defaults[pl][action];
    }
    for (i = 0; i < 4 && pad_defaults[action][i].type != BIND_NONE && n < MAX_BINDINGS; i++) {
        bindings[pl][action][n++] = pad_defaults[action][i];
    }
}

void input_init(void) {
    int pl, a;
    for (pl = 0; pl < 2; pl++) {
        for (a = 0; a < ACTIONS; a++) {
            input_defaults(pl, a);
        }
    }
    /* layouts SDL lacks */
    SDL_AddGamepadMappingsFromFile(host_save_path("gamecontrollerdb.txt"));
    input_load();
}

/* key:NAME, pad:BUTTON, or pad:AXIS+ or - */
const char* binding_name(const BINDING* b, char* buf, size_t size) {
    switch (b->type) {
    case BIND_KEY:
        snprintf(buf, size, "key:%s", SDL_GetScancodeName(b->code));
        break;
    case BIND_BUTTON:
        snprintf(buf, size, "pad:%s", SDL_GetGamepadStringForButton(b->code));
        break;
    case BIND_AXIS:
        snprintf(buf, size, "pad:%s%c", SDL_GetGamepadStringForAxis(b->code), b->dir < 0 ? '-' : '+');
        break;
    default:
        snprintf(buf, size, "none");
    }
    return buf;
}

static int binding_parse(const char* s, size_t n, BINDING* b) {
    char name[64];
    if (n < 5 || n - 4 >= sizeof name) {
        return 0;
    }
    memcpy(name, s + 4, n - 4);
    name[n - 4] = 0;
    if (strncmp(s, "key:", 4) == 0) {
        b->type = BIND_KEY;
        b->code = SDL_GetScancodeFromName(name);
        b->dir = 0;
        return b->code != SDL_SCANCODE_UNKNOWN;
    }
    if (strncmp(s, "pad:", 4) != 0) {
        return 0;
    }
    if (name[n - 5] == '+' || name[n - 5] == '-') {
        b->type = BIND_AXIS;
        b->dir = name[n - 5] == '-' ? -1 : 1;
        name[n - 5] = 0;
        b->code = SDL_GetGamepadAxisFromString(name);
        return b->code != SDL_GAMEPAD_AXIS_INVALID;
    }
    b->type = BIND_BUTTON;
    b->code = SDL_GetGamepadButtonFromString(name);
    b->dir = 0;
    return b->code != SDL_GAMEPAD_BUTTON_INVALID;
}

static void setting_name(int pl, int action, char* buf, size_t size) {
    snprintf(buf, size, "p%d %s", pl + 1, input_actions[action].name);
}

void input_load(void) {
    int pl, a;
    for (pl = 0; pl < 2; pl++) {
        for (a = 0; a < ACTIONS; a++) {
            char key[32];
            const char* v;
            int n = 0;
            setting_name(pl, a, key, sizeof key);
            v = config_get(key);
            if (v == NULL) {
                continue;
            }
            memset(bindings[pl][a], 0, sizeof bindings[pl][a]);
            /* split at ", " before key: or pad:, since a key's name can hold either */
            while (*v != 0 && n < MAX_BINDINGS) {
                const char* end = v;
                while ((end = strstr(end + 1, ", ")) != NULL && strncmp(end + 2, "key:", 4) != 0 &&
                       strncmp(end + 2, "pad:", 4) != 0) {
                }
                if (end == NULL) {
                    end = v + strlen(v);
                }
                if (binding_parse(v, end - v, &bindings[pl][a][n])) {
                    n++;
                } else {
                    memset(&bindings[pl][a][n], 0, sizeof bindings[pl][a][n]);
                }
                v = *end != 0 ? end + 2 : end;
            }
        }
    }
}

/* those changed, and those mvc2.cfg has already */
void input_save(int pl) {
    int a, i;
    for (a = 0; a < ACTIONS; a++) {
        char key[32], value[MAX_BINDINGS * 40], one[40];
        BINDING now[MAX_BINDINGS];
        size_t len = 0;
        setting_name(pl, a, key, sizeof key);
        memcpy(now, bindings[pl][a], sizeof now);
        input_defaults(pl, a);
        i = memcmp(now, bindings[pl][a], sizeof now) == 0;
        memcpy(bindings[pl][a], now, sizeof now);
        if (i && config_get(key) == NULL) {
            continue;
        }
        value[0] = 0;
        for (i = 0; i < MAX_BINDINGS && bindings[pl][a][i].type != BIND_NONE; i++) {
            len += snprintf(value + len, sizeof value - len, "%s%s", i > 0 ? ", " : "",
                            binding_name(&bindings[pl][a][i], one, sizeof one));
        }
        config_set(key, i > 0 ? value : "none");
    }
}

/* a key replaces the action's keys, a button or axis its controller's */
void input_set(int pl, int action, const BINDING* b) {
    int a, i, j;
    int pad = b->type != BIND_KEY;
    for (a = 0; a < ACTIONS; a++) {
        for (i = j = 0; i < MAX_BINDINGS && bindings[pl][a][i].type != BIND_NONE; i++) {
            const BINDING* o = &bindings[pl][a][i];
            int same = o->type == b->type && o->code == b->code && o->dir == b->dir;
            if (!same && !(a == action && (o->type != BIND_KEY) == pad)) {
                bindings[pl][a][j++] = *o;
            }
        }
        for (; j < MAX_BINDINGS; j++) {
            memset(&bindings[pl][a][j], 0, sizeof bindings[pl][a][j]);
        }
    }
    for (i = 0; i < MAX_BINDINGS - 1 && bindings[pl][action][i].type != BIND_NONE; i++) {
    }
    bindings[pl][action][i] = *b;
}

void input_event(const SDL_Event* e) {
    int i;
    if (e->type == SDL_EVENT_GAMEPAD_ADDED) {
        SDL_JoystickID id = pad_added_id(e);
        for (i = 0; i < 2; i++) {
            if (pads[i] != NULL && SDL_GetGamepadID(pads[i]) == id) {
                return;
            }
        }
        for (i = 0; i < 2 && pads[i] != NULL; i++) {
        }
        if (i < 2) {
            pads[i] = pad_open_added(e);
            if (pads[i] != NULL) {
                fprintf(stderr, "player %d: %s\n", i + 1, SDL_GetGamepadName(pads[i]));
            }
        }
    } else if (e->type == SDL_EVENT_GAMEPAD_REMOVED) {
        for (i = 0; i < 2; i++) {
            if (pads[i] != NULL && SDL_GetGamepadID(pads[i]) == e->gdevice.which) {
                fprintf(stderr, "player %d: controller gone\n", i + 1);
                SDL_CloseGamepad(pads[i]);
                pads[i] = NULL;
            }
        }
    }
}

SDL_Gamepad* input_pad(int pl) {
    return pads[pl];
}

int input_pad_player(SDL_JoystickID id) {
    int i;
    for (i = 0; i < 2; i++) {
        if (pads[i] != NULL && SDL_GetGamepadID(pads[i]) == id) {
            return i;
        }
    }
    return -1;
}

int input_menu_combo(int pl) {
    return pads[pl] != NULL && SDL_GetGamepadButton(pads[pl], SDL_GAMEPAD_BUTTON_BACK) &&
           SDL_GetGamepadButton(pads[pl], SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER);
}

int input_held(int pl, const BINDING* b) {
    switch (b->type) {
    case BIND_KEY:
        return key_held(b->code);
    case BIND_BUTTON:
        return pads[pl] != NULL && SDL_GetGamepadButton(pads[pl], b->code);
    case BIND_AXIS:
        return pads[pl] != NULL && SDL_GetGamepadAxis(pads[pl], b->code) * b->dir > 16384;
    }
    return 0;
}

void input_read(HOST_INPUT* in) {
    static int coin_held;
    int pl, a, i, coin = 0;
    for (pl = 0; pl < 2; pl++) {
        for (a = 0; a < ACTIONS; a++) {
            for (i = 0; i < MAX_BINDINGS && bindings[pl][a][i].type != BIND_NONE; i++) {
                if (input_held(pl, &bindings[pl][a][i])) {
                    if (input_actions[a].bit != 0) {
                        in->player[pl] |= input_actions[a].bit;
                    } else {
                        coin = 1;
                    }
                }
            }
        }
    }
    for (pl = 0; pl < 2; pl++) {
        if (input_menu_combo(pl)) {
            coin_held = 1;          /* not from the controls screen's buttons */
        }
    }
    in->coin = coin && !coin_held;
    coin_held = coin;
}

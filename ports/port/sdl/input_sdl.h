/*
 * input_sdl.h  The players' controls (input_sdl.c)
 */

#ifndef INPUT_SDL_H
#define INPUT_SDL_H

#include "sdl_front.h"

#include "naomi_input.h"

enum { ACTIONS = 12, MAX_BINDINGS = 6 };

typedef struct {
    const char* name;
    uint32_t bit;           /* 0 for the coin */
} INPUT_ACTION;

enum { BIND_NONE, BIND_KEY, BIND_BUTTON, BIND_AXIS };

typedef struct {
    int type;
    int code;
    int dir;                /* an axis: -1 or 1 */
} BINDING;

extern const INPUT_ACTION input_actions[ACTIONS];
extern BINDING bindings[2][ACTIONS][MAX_BINDINGS];     /* ended by BIND_NONE */

void input_init(void);
void input_defaults(int pl, int action);
void input_event(const SDL_Event* e);
SDL_Gamepad* input_pad(int pl);
int input_pad_player(SDL_JoystickID id);
int input_menu_combo(int pl);
int input_held(int pl, const BINDING* b);
void input_read(HOST_INPUT* in);
void input_load(void);
void input_save(int pl);
void input_set(int pl, int action, const BINDING* b);
const char* binding_name(const BINDING* b, char* buf, size_t size);

#endif

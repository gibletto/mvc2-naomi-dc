/*
 * sdl_front.h  SDL3 for the front end, or with MVC2_SDL3 not defined, SDL2 under SDL3's names
 */

#ifndef SDL_FRONT_H
#define SDL_FRONT_H

#ifdef MVC2_SDL3

#include <SDL3/SDL.h>

#define SDL_SUCCEEDED(x) (x)

static inline SDL_Scancode key_scancode(const SDL_Event* e) {
    return e->key.scancode;
}

static inline SDL_Keycode key_sym(const SDL_Event* e) {
    return e->key.key;
}

static inline int key_alt(const SDL_Event* e) {
    return (e->key.mod & SDL_KMOD_ALT) != 0;
}

static inline int window_resized(const SDL_Event* e) {
    return e->type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED;
}

static inline SDL_JoystickID pad_added_id(const SDL_Event* e) {
    return e->gdevice.which;
}

static inline SDL_Gamepad* pad_open_added(const SDL_Event* e) {
    return SDL_OpenGamepad(e->gdevice.which);
}

#else

#include <SDL.h>

#define SDL_SUCCEEDED(x) ((x) == 0)

#define SDL_EVENT_QUIT SDL_QUIT
#define SDL_EVENT_KEY_DOWN SDL_KEYDOWN
#define SDL_EVENT_GAMEPAD_ADDED SDL_CONTROLLERDEVICEADDED
#define SDL_EVENT_GAMEPAD_REMOVED SDL_CONTROLLERDEVICEREMOVED
#define SDL_EVENT_GAMEPAD_BUTTON_DOWN SDL_CONTROLLERBUTTONDOWN
#define SDL_EVENT_GAMEPAD_AXIS_MOTION SDL_CONTROLLERAXISMOTION
#define gdevice cdevice
#define gbutton cbutton
#define gaxis caxis

#define SDL_Gamepad SDL_GameController
#define SDL_CloseGamepad SDL_GameControllerClose
#define SDL_GetGamepadName SDL_GameControllerName
#define SDL_GetGamepadButton SDL_GameControllerGetButton
#define SDL_GetGamepadAxis SDL_GameControllerGetAxis
#define SDL_AddGamepadMappingsFromFile SDL_GameControllerAddMappingsFromFile
#define SDL_GetGamepadStringForButton SDL_GameControllerGetStringForButton
#define SDL_GetGamepadStringForAxis SDL_GameControllerGetStringForAxis
#define SDL_GetGamepadButtonFromString SDL_GameControllerGetButtonFromString
#define SDL_GetGamepadAxisFromString SDL_GameControllerGetAxisFromString

#define SDL_GAMEPAD_BUTTON_INVALID SDL_CONTROLLER_BUTTON_INVALID
#define SDL_GAMEPAD_BUTTON_SOUTH SDL_CONTROLLER_BUTTON_A
#define SDL_GAMEPAD_BUTTON_EAST SDL_CONTROLLER_BUTTON_B
#define SDL_GAMEPAD_BUTTON_WEST SDL_CONTROLLER_BUTTON_X
#define SDL_GAMEPAD_BUTTON_NORTH SDL_CONTROLLER_BUTTON_Y
#define SDL_GAMEPAD_BUTTON_BACK SDL_CONTROLLER_BUTTON_BACK
#define SDL_GAMEPAD_BUTTON_START SDL_CONTROLLER_BUTTON_START
#define SDL_GAMEPAD_BUTTON_LEFT_SHOULDER SDL_CONTROLLER_BUTTON_LEFTSHOULDER
#define SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER SDL_CONTROLLER_BUTTON_RIGHTSHOULDER
#define SDL_GAMEPAD_BUTTON_DPAD_UP SDL_CONTROLLER_BUTTON_DPAD_UP
#define SDL_GAMEPAD_BUTTON_DPAD_DOWN SDL_CONTROLLER_BUTTON_DPAD_DOWN
#define SDL_GAMEPAD_BUTTON_DPAD_LEFT SDL_CONTROLLER_BUTTON_DPAD_LEFT
#define SDL_GAMEPAD_BUTTON_DPAD_RIGHT SDL_CONTROLLER_BUTTON_DPAD_RIGHT
#define SDL_GAMEPAD_BUTTON_COUNT SDL_CONTROLLER_BUTTON_MAX
#define SDL_GAMEPAD_AXIS_INVALID SDL_CONTROLLER_AXIS_INVALID
#define SDL_GAMEPAD_AXIS_LEFTX SDL_CONTROLLER_AXIS_LEFTX
#define SDL_GAMEPAD_AXIS_LEFTY SDL_CONTROLLER_AXIS_LEFTY
#define SDL_GAMEPAD_AXIS_LEFT_TRIGGER SDL_CONTROLLER_AXIS_TRIGGERLEFT
#define SDL_GAMEPAD_AXIS_RIGHT_TRIGGER SDL_CONTROLLER_AXIS_TRIGGERRIGHT
#define SDL_GAMEPAD_AXIS_COUNT SDL_CONTROLLER_AXIS_MAX

#define SDL_RenderTexture SDL_RenderCopy

static inline SDL_JoystickID SDL_GetGamepadID(SDL_GameController* pad) {
    return SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(pad));
}

static inline SDL_Scancode key_scancode(const SDL_Event* e) {
    return e->key.keysym.scancode;
}

static inline SDL_Keycode key_sym(const SDL_Event* e) {
    return e->key.keysym.sym;
}

static inline int key_alt(const SDL_Event* e) {
    return (e->key.keysym.mod & KMOD_ALT) != 0;
}

static inline int window_resized(const SDL_Event* e) {
    return e->type == SDL_WINDOWEVENT && e->window.event == SDL_WINDOWEVENT_SIZE_CHANGED;
}

/* SDL2's event has the controller's device index, not its id */
static inline SDL_JoystickID pad_added_id(const SDL_Event* e) {
    return SDL_JoystickGetDeviceInstanceID(e->cdevice.which);
}

static inline SDL_GameController* pad_open_added(const SDL_Event* e) {
    return SDL_GameControllerOpen(e->cdevice.which);
}

#endif

/* in the renderer's logical coordinates */
static inline void render_at(SDL_Renderer* r, SDL_Texture* t, int x, int y, int w, int h) {
#ifdef MVC2_SDL3
    SDL_FRect d = { (float)x, (float)y, (float)w, (float)h };
#else
    SDL_Rect d = { x, y, w, h };
#endif
    SDL_RenderTexture(r, t, NULL, &d);
}

static inline int key_held(SDL_Scancode k) {
    return SDL_GetKeyboardState(NULL)[k] != 0;
}

#endif

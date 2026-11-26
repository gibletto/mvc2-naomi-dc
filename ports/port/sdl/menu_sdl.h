/*
 * menu_sdl.h  The controls screen (menu_sdl.c)
 */

#ifndef MENU_SDL_H
#define MENU_SDL_H

#include <stdint.h>

#include "sdl_front.h"
#include "menu_font.h"

int menu_active(void);
int menu_event(const SDL_Event* e);         /* 1 when it took the event */
enum { RENDERER_SOFT, RENDERER_GL, RENDERER_GPU };
int renderer_gpu(void);                     /* main_sdl.c */
int renderer_kind(void);                    /* RENDERER_SOFT, _GL or _GPU */
int renderer_set(int want);                 /* RENDERER_GPU: SDL_GPU, else OpenGL, else soft; what started */
void view_settings(void);                   /* the GPU's picture, from mvc2.cfg */
void view_drawn(int* fw, int* fh);          /* the size the GPU draws it at, in this window */
enum { FULL_OFF, FULL_DESKTOP, FULL_EXCLUSIVE };
int fullscreen_kind(void);                  /* FULL_OFF, _DESKTOP or _EXCLUSIVE */
void fullscreen_set(int kind);              /* exclusive where it can be (not on Wayland), else the desktop's */
void menu_draw(uint32_t* frame, int w, int h);

#endif

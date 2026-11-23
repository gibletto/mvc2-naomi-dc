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
int renderer_gpu(void);                     /* main_sdl.c */
int renderer_set_gpu(int on);
void view_settings(void);                   /* the GPU's picture, from mvc2.cfg */
void view_drawn(int* fw, int* fh);          /* the size the GPU draws it at, in this window */
void menu_draw(uint32_t* frame, int w, int h);

#endif

/*
 * menu_sdl.h  The controls screen (menu_sdl.c)
 */

#ifndef MENU_SDL_H
#define MENU_SDL_H

#include <stdint.h>

#include "sdl_front.h"

int menu_active(void);
int menu_event(const SDL_Event* e);         /* 1 when it took the event */
int renderer_gpu(void);                     /* main_sdl.c */
int renderer_set_gpu(int on);
void menu_text(uint32_t* buf, int w, int h, int x, int y, const char* s, uint32_t color, int scale);
void menu_draw(uint32_t* frame, int w, int h);

#endif

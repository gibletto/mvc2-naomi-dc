/*
 * menu_font.h  The front ends' 5 by 7 font (menu_font.c)
 */

#ifndef MENU_FONT_H
#define MENU_FONT_H

#include <stdint.h>

void menu_text(uint32_t* buf, int w, int h, int x, int y, const char* s, uint32_t color, int scale);

#endif

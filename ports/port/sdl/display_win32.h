/*
 * display_win32.h  What Windows says of the display a window is on (display_win32.c)
 */

#ifndef DISPLAY_WIN32_H
#define DISPLAY_WIN32_H

#include <stdint.h>

/* 1 when NVIDIA's driver has the display's variable refresh (G-Sync, G-Sync Compatible) on for this program, 0 off,
   -1 not known (no NVIDIA driver, or it doesn't say). It says so only once the program presents to the display */
int display_vrr(void* hwnd);
/* the last refresh's time and the refresh period, in QueryPerformanceCounter's ticks (SDL_GetPerformanceCounter's
   on Windows), from the compositor's clock: 0 when it has none */
int display_vblank(uint64_t* last, uint64_t* period);

#endif

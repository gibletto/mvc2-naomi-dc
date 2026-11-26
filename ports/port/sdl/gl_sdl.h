/*
 * gl_sdl.h  The PC's OpenGL renderer (SDL3): rd_gl1 in SDL's OpenGL renderer's context, for a GPU with no Vulkan or
 * Direct3D 12
 */

#ifndef GL_SDL_H
#define GL_SDL_H

#include <stdint.h>

#include <SDL3/SDL.h>

/* SDL's OpenGL renderer on window with rd_gl1 started in it; NULL, and why in SDL_GetError, when either can't */
SDL_Renderer* gl_open(SDL_Window* window);
/* the frame's lists drawn at w by h (the game's 640 by 480 when the GL can't make that size): the texture that shows
   them, NULL when there is none */
SDL_Texture* gl_frame(SDL_Renderer* renderer, int w, int h);
/* the last frame at the game's size, as ARGB */
void gl_read(SDL_Renderer* renderer, uint32_t* argb);
/* before renderer is destroyed */
void gl_close(SDL_Renderer* renderer);
const char* gl_name(void);          /* the GL's vendor, renderer and version */

#endif

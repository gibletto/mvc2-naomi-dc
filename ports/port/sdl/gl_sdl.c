/*
 * gl_sdl.c  The PC's OpenGL renderer (SDL3), for a GPU with no Vulkan or Direct3D 12: the frame's lists drawn by
 * OpenGL's fixed function (rd_gl1, as on the MiSTer) in the context of SDL's OpenGL renderer, into a framebuffer object
 * that SDL then shows as a texture, so that the scaling, the borders and the overlays are SDL's as with the others
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gl_sdl.h"
#include "rd.h"
#include "rd_gl1.h"

#define GL_TEXTURE_2D 0x0DE1
#define GL_VENDOR 0x1F00

#if defined(_WIN32) && !defined(_WIN64)
#define GLCALL __stdcall
#else
#define GLCALL
#endif

static SDL_GLContext context;
static SDL_Window* win;
static SDL_Texture* shown;          /* the framebuffer object's texture, as SDL's */
static unsigned shown_name;
static int shown_w, shown_h;
static char name[160];

static void* proc(const char* fn) {
    return (void*)SDL_GL_GetProcAddress(fn);
}

static void current(SDL_Renderer* renderer) {
    SDL_FlushRenderer(renderer);    /* SDL's draws so far made, and what it knows of the GL's state forgotten */
    if (SDL_GL_GetCurrentContext() != context) {
        SDL_GL_MakeCurrent(win, context);
    }
}

/* the framebuffer object made w by h if it isn't, and SDL's texture of it with it: SDL sends the texture's level
   again as it wraps it, so before the frame is drawn into it. Bound, as rd_gl1_offscreen leaves it; 0 when not made */
static unsigned target(SDL_Renderer* renderer, int w, int h) {
    unsigned tex = rd_gl1_offscreen(w, h);
    if (tex != 0 && (shown == NULL || tex != shown_name || w != shown_w || h != shown_h)) {
        SDL_PropertiesID props = SDL_CreateProperties();
        rd_gl1_release();
        if (shown != NULL) {
            SDL_DestroyTexture(shown);
        }
        SDL_SetNumberProperty(props, SDL_PROP_TEXTURE_CREATE_FORMAT_NUMBER, SDL_PIXELFORMAT_ABGR8888);
        SDL_SetNumberProperty(props, SDL_PROP_TEXTURE_CREATE_ACCESS_NUMBER, SDL_TEXTUREACCESS_STATIC);
        SDL_SetNumberProperty(props, SDL_PROP_TEXTURE_CREATE_WIDTH_NUMBER, w);
        SDL_SetNumberProperty(props, SDL_PROP_TEXTURE_CREATE_HEIGHT_NUMBER, h);
        SDL_SetNumberProperty(props, SDL_PROP_TEXTURE_CREATE_OPENGL_TEXTURE_NUMBER, tex);
        shown = SDL_CreateTextureWithProperties(renderer, props);
        SDL_DestroyProperties(props);
        /* a 2D texture, as rd_gl1's is: SDL takes a rectangle one where the GL has no other of any size */
        if (shown != NULL && SDL_GetNumberProperty(SDL_GetTextureProperties(shown),
                                                   SDL_PROP_TEXTURE_OPENGL_TEXTURE_TARGET_NUMBER, 0) != GL_TEXTURE_2D) {
            SDL_DestroyTexture(shown);
            shown = NULL;
        }
        if (shown == NULL) {
            return 0;
        }
        SDL_SetTextureBlendMode(shown, SDL_BLENDMODE_NONE);    /* the frame's alpha isn't shown */
        shown_name = tex, shown_w = w, shown_h = h;
        current(renderer);
        tex = rd_gl1_offscreen(w, h);
    }
    return tex;
}

SDL_Renderer* gl_open(SDL_Window* window) {
    const char* words = getenv("MVC2_GL1");
    SDL_Renderer* renderer = SDL_CreateRenderer(window, "opengl");
    const unsigned char*(GLCALL * get_string)(unsigned);
    if (renderer == NULL) {
        return NULL;
    }
    win = window;
    context = SDL_GL_GetCurrentContext();
    get_string = (const unsigned char*(GLCALL*)(unsigned))proc("glGetString");
    if (context != NULL && get_string != NULL) {
        snprintf(name, sizeof name, "%s, %s, %s", (const char*)get_string(GL_VENDOR),
                 (const char*)get_string(GL_VENDOR + 1), (const char*)get_string(GL_VENDOR + 2));
    }
    /* drawn upward, as SDL takes a texture's rows; less: the opaque lists nearest first, so the GPU skips what they
       hide; pool: the small textures drawn again and again (the font's cells) kept; tex16: 16-bit textures sent as
       they are */
    rd_gl1_options(words != NULL ? words : "up less pool tex16");
    if (context == NULL || !rd_gl1_init(proc)) {
        SDL_DestroyRenderer(renderer);
        SDL_SetError("rd_gl1 could not start on %s (the GL's calls or texture units fall short)", name);
        return NULL;
    }
    if (target(renderer, RD_WIDTH, RD_HEIGHT) == 0) {
        rd_gl1_quit();
        SDL_DestroyRenderer(renderer);
        SDL_SetError("no framebuffer object or texture of 640 by 480 on %s", name);
        return NULL;
    }
    rd_gl1_release();
    return renderer;
}

SDL_Texture* gl_frame(SDL_Renderer* renderer, int w, int h) {
    unsigned tex;
    current(renderer);
    tex = target(renderer, w, h);
    if (tex == 0 && (w != RD_WIDTH || h != RD_HEIGHT)) {
        tex = target(renderer, RD_WIDTH, RD_HEIGHT);
    }
    if (tex != 0) {
        rd_gl1_end_frame();
    }
    rd_gl1_release();
    return tex != 0 ? shown : NULL;
}

void gl_read(SDL_Renderer* renderer, uint32_t* argb) {
    current(renderer);
    rd_gl1_read(argb);
}

void gl_close(SDL_Renderer* renderer) {
    current(renderer);
    if (shown != NULL) {
        SDL_DestroyTexture(shown);
        shown = NULL;
    }
    rd_gl1_quit();
    context = NULL;
}

const char* gl_name(void) {
    return name;
}

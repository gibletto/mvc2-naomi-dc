/*
 * rd_gl1.h  The frame drawn with OpenGL 1.4's fixed function (the MiSTer's Pinedra GPU)
 */

#ifndef RD_GL1_H
#define RD_GL1_H

#include <stdint.h>

#include "rd_list.h"

/* a GL entry point by name: dlsym on the MiSTer, SDL_GL_GetProcAddress on the PC */
typedef void* (*RD_GL_PROC)(const char* name);

/* 0 when an entry point is missing; the context is current */
int rd_gl1_init(RD_GL_PROC proc);
/* the frame's lists into the bound framebuffer at 0, 0, RD_WIDTH by RD_HEIGHT, or the size set */
void rd_gl1_end_frame(void);
/* f with this palette and fog: a frame the game has handed on, drawn on another thread while it fills the next */
void rd_gl1_draw_frame(RD_FRAME* f, const uint32_t* palette, const RD_FOG* fog);
void rd_gl1_size(int width, int height);
/* what it drew, as ARGB rows from the top (at RD_WIDTH by RD_HEIGHT only) */
void rd_gl1_read(uint32_t* argb);
/* a picture made on the CPU, RD_WIDTH by RD_HEIGHT ARGB rows from the top, drawn over the whole frame */
void rd_gl1_show(const uint32_t* argb);
void rd_gl1_quit(void);

/* MVC2_GL1 words, also set by a front end: tex16 (16-bit textures as the GPU takes them, not RGBA8), up (drawn
   bottom row first), less (depth tests the other way, the opaque list nearest first), snap=N (the GPU's grid,
   N a pixel), deep=K, shallow=K (the translucent list's cuts) */
void rd_gl1_options(const char* words);
int rd_gl1_ordered(void);           /* the driver orders the translucent list */

typedef struct {
    int batches, tris, verts, uploads, textures, skipped, offset_lost;
    uint32_t upload_bytes;
    uint32_t us_build, us_draw;     /* the lists put in order, drawing as it goes, and the last draws: a clock read
                                       a frame, not a draw or an upload (on the MiSTer each is a system call) */
} RD_GL1_STATS;

extern RD_GL1_STATS rd_gl1_stats;   /* the last frame's */

typedef struct {
    int textures;                   /* the GL textures held for the lists' (not the pool's) */
    size_t texels;                  /* their bytes as sent, mips too */
    size_t arrays;                  /* the cache's and the build's own arrays */
} RD_GL1_MEMORY;

void rd_gl1_memory(RD_GL1_MEMORY* m);

#endif

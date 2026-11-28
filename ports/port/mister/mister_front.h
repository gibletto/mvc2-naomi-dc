/*
 * mister_front.h  The MiSTer front end: the picture on the Pinedra GPU core (mister_pinedra.c), the sound to
 * /dev/MrAudio (mister_audio.c), the clock
 */

#ifndef MISTER_FRONT_H
#define MISTER_FRONT_H

#include <stdint.h>

#include "naomi_input.h"

uint64_t mister_us(void);                   /* CLOCK_MONOTONIC */

/* the lists drawn by the Pinedra core's OpenGL */
int mister_gl_init(void);                   /* 0 without the core's library or context */
void mister_gl_draw(void);                  /* the frame's lists, before rd_begin_frame */
void mister_gl_show(const uint32_t* argb);   /* a whole RD_WIDTH x RD_HEIGHT picture, piped or not */
void mister_gl_swap(void);                  /* shown from the next vblank, waited for when vsynced */
int mister_gl_vsynced(void);
void mister_gl_exit(void);
#include "rd_gl1.h"
/* piped (MVC2_PIPE): the frame handed to the drawing thread, rd_frame then the next's; the game's next frame from the
   next vblank; the last frame drawn's stats and time, and the GL's memory, as of the hand-off */
int mister_gl_piped(void);
void mister_gl_hand(unsigned frame);
void mister_gl_next(void);
const RD_GL1_STATS* mister_gl_stats(void);
uint32_t mister_gl_draw_us(void);
void mister_gl_memory(RD_GL1_MEMORY* m);
extern void (*mister_gl_drawn)(unsigned frame);     /* on the drawing thread, once a frame is drawn */

/* the sound: 44.1 kHz from the AICA, stretched a little to keep about three frames queued, out at 48 kHz, made on
   a thread of its own */
void mister_audio_open(void);
void mister_audio_frame(void);              /* after each frame: its sound begun */
void mister_audio_idle(void);               /* until the frame's sound is made */
void mister_audio_close(void);
extern uint32_t mister_audio_queued;        /* bytes the FPGA had left at the last frame */
extern uint32_t mister_audio_dry, mister_audio_dropped;     /* frames it ran dry at, frames dropped */
extern uint32_t mister_audio_make_us;       /* the last frame's sound's making, in its thread's CPU time */
extern uint32_t mister_audio_wait_us;       /* the game's waits for it, from the start */

/* the keyboard's and pads' controls added in (mister_input.c); 1 once the player has asked to quit */
void mister_input_read(HOST_INPUT* in);
int mister_input_quit(void);
/* the first start's unpack of name, on the wrapper's OSD */
void mister_unpacking(const char* name, uint32_t done_kb, uint32_t total_kb);

/* MVC2_PROF=file: the program counter sampled (mister_prof.c) */
void mister_prof_start(void);
/* MVC2_PIN=1: the calling thread kept on that CPU (the game on 0, the sound's maker on 1) */
int mister_pin(int cpu);    /* 1: pinned */
void mister_on_cpu1(void);  /* the calling thread on CPU1 alone */
double mister_rate(void);   /* the game's frames a second (mvc2.cfg's rate) */
/* what the main thread is doing, for the samples: 0 the game, 1 presenting, 2 the sound's frame; others' are 3 */
enum { MISTER_PHASES = 4 };
extern volatile int mister_prof_phase;
void mister_prof_stop(void);

#endif

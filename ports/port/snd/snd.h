/*
 * snd.h  The sound, as the front ends see it
 */

#ifndef SND_H
#define SND_H

#include <stdint.h>
#include <stdio.h>

#define SND_RATE 44100
#define SND_FRAME_US 16683          /* the game's frame at 59.94 Hz, as the board's timer counts it */

int snd_enabled(void);                      /* MVC2_SOUND=0 turns it off */
int snd_frame(const int16_t** out);         /* a frame's stereo samples (about 736), made once a frame */
void snd_close(void);                       /* at the end: MVC2_SNDDBG's timing */
void snd_free(void);                        /* at a restart: the sound RAM given back */

/* a frame made on another thread: snd_prepare after the game's frame, then snd_frame there; every sound call
   first calls snd_sync, and fn waits for that frame */
void snd_prepare(void);
void snd_set_frame_us(uint32_t us);         /* before snd_prepare: a longer frame, when the game falls behind */
void snd_set_rate(double fps);              /* the game's frames a second: each frame's sound 1/fps s long */
uint32_t snd_base_us(void);                 /* that frame, in us */
int snd_frame_extra(const int16_t** out);   /* a frame between the game's (no mixer); 0 if it can't be made */
void snd_set_sync(void (*fn)(void));
void snd_sync(void);

/* a call from the game into the sound, from snd_sync at its start to its end at the block's end, so that a front
   end can make frames between the game's while none is under way */
#define SND_CALL() int snd_call_ __attribute__((cleanup(snd_call_end), unused)) = snd_call_begin()
void snd_set_calls(void (*begin)(void), void (*end)(void));
int snd_call_begin(void);
void snd_call_end(int* call);

/* the game's calls at a frame's start (s3lIntr, s3lSetVolume), run on that thread after the frame it is making;
   fn gives 0 when the caller must run them */
typedef void (*SND_WORK)(int a, int b);
void snd_set_defer(int (*fn)(SND_WORK work, int a, int b));
int snd_defer(SND_WORK work, int a, int b);
void s3l_stage(void);                       /* s3l.c, for snd_prepare: the streams' next data read ahead */

/* what is mixed after the AICA (the Dreamcast's ADX music): made by snd_prepare, on the game's thread */
typedef void (*SND_MIXER)(int16_t* buf, int n);
void snd_set_mixer(SND_MIXER fn);
#ifdef __3DS__
/* its part of a frame between the game's, on the sound thread; 0 when it can't be made, and then the frame isn't */
typedef int (*SND_EXTRA_MIXER)(int16_t* buf, int n);
void snd_set_extra_mixer(SND_EXTRA_MIXER fn);
/* and its part of the game's frames, on the sound thread: replacing what snd_prepare mixed into buf, or not */
void snd_set_late_mixer(SND_MIXER fn);
#endif

uint8_t* snd_sh4_ptr(uint32_t sh4_addr);
void snd_sh4_written(uint32_t sh4_addr, uint32_t size);     /* after a write through snd_sh4_ptr */

#endif

/*
 * snd.h  The sound, as the front ends see it
 */

#ifndef SND_H
#define SND_H

#include <stdint.h>
#include <stdio.h>

#define SND_RATE 44100

int snd_enabled(void);                      /* MVC2_SOUND=0 turns it off */
int snd_frame(const int16_t** out);         /* a frame's stereo samples (about 736), made once a frame */
void snd_close(void);                       /* at the end: MVC2_SNDDBG's timing */
void snd_free(void);                        /* at a restart: the sound RAM given back */

/* a frame made on another thread: snd_prepare after the game's frame, then snd_frame there; every sound call
   first calls snd_sync, and fn waits for that frame */
void snd_prepare(void);
void snd_set_sync(void (*fn)(void));
void snd_sync(void);

/* the game's calls at a frame's start (s3lIntr, s3lSetVolume), run on that thread after the frame it is making;
   fn gives 0 when the caller must run them */
typedef void (*SND_WORK)(int a, int b);
void snd_set_defer(int (*fn)(SND_WORK work, int a, int b));
int snd_defer(SND_WORK work, int a, int b);
void s3l_stage(void);                       /* s3l.c, for snd_prepare: the streams' next data read ahead */

/* what is mixed after the AICA (the Dreamcast's ADX music): made by snd_prepare, on the game's thread */
typedef void (*SND_MIXER)(int16_t* buf, int n);
void snd_set_mixer(SND_MIXER fn);

uint8_t* snd_sh4_ptr(uint32_t sh4_addr);

#endif

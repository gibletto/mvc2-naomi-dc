/* prof.h  Zone timers for profiling builds (-DMVC2_PROF); nothing at all otherwise */

#ifndef PROF_H
#define PROF_H

#include <stdint.h>

/* each zone is added to by one thread only */
enum {
    PZ_FRAME,                                   /* a whole frame, timed by prof_frame */
    PZ_GAME, PZ_PRESENT, PZ_LISTS, PZ_TEXTURES, PZ_DRAWS, PZ_GPU_WAIT, PZ_VBLANK,
    PZ_ROM_MISS, PZ_ROM_POLL, PZ_SND_PREPARE, PZ_SND_SYNC,
    PZ_SND_FRAME = 16, PZ_SND_ARM, PZ_SND_FIQ, PZ_SND_IO, PZ_SND_CHANNELS, PZ_SND_MIX, PZ_SND_DEFER,
    PZ_SND_OUT, PZ_SND_IDLE,
    PZ_SND_SYNC_MID = 25,                       /* the game's waits inside its frame; PZ_SND_SYNC is the frame's end */
    PZ_ROM_THREAD = 28,
    PZ_COUNT = 32
};

#ifdef MVC2_PROF
uint32_t prof_now(void);                    /* the system tick, 268 MHz */
void prof_add(int zone, uint32_t ticks);    /* a call's time */
void prof_count(int zone, uint32_t n);      /* items counted with the zone, untimed */
void prof_frame(void);                      /* the end of a frame, on the game's thread */
void prof_start(void);                      /* first thing: the calibration, and the writer */
void prof_stop(void);
#define PROF_START(v) uint32_t v = prof_now()
#define PROF_STOP(zone, v) prof_add((zone), prof_now() - (v))
#define PROF_ADD(zone, ticks) prof_add((zone), (uint32_t)(ticks))
#define PROF_COUNT(zone, n) prof_count((zone), (n))
#define PROF_FRAME() prof_frame()
#else
#define PROF_START(v) ((void)0)
#define PROF_STOP(zone, v) ((void)0)
#define PROF_ADD(zone, ticks) ((void)0)
#define PROF_COUNT(zone, n) ((void)0)
#define PROF_FRAME() ((void)0)
#endif

#endif

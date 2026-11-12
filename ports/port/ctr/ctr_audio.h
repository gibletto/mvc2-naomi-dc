/*
 * ctr_audio.h  The 3DS's sound out (ctr_audio.c), for main_3ds.c
 */

#ifndef CTR_AUDIO_H
#define CTR_AUDIO_H

void ctr_audio_init(void);      /* after start_game: NDSP, and the sound's thread on the second core */
void ctr_audio_frame(void);     /* after each naomi_run_frame: the frame's sound is made beside the next frame */
void ctr_audio_idle(void);      /* before switch_game: waits until the thread is done with the sound's state */
void ctr_audio_exit(void);      /* at the end */

#endif

/* ctr_render.h  The 3DS's renderer: the frame's lists drawn with citro3d */

#ifndef CTR_RENDER_H
#define CTR_RENDER_H

#include <stdint.h>

int ctr_render_init(int full, int stretch);   /* full: 640x480 drawn, halved (else 320x240); 0: no GPU */
void ctr_render_frame(void);
void ctr_render_exit(void);
extern unsigned ctr_presented;
int ctr_render_read(uint32_t* frame);  /* 640x480 ARGB, for test runs */

/* system ticks, summed until cleared */
enum { CTR_TICKS_SYNC, CTR_TICKS_LISTS, CTR_TICKS_TEX, CTR_TICKS_SORT, CTR_TICKS_TRANS, CTR_TICKS_DRAW, CTR_TICKS_END, CTR_TICKS };
extern uint64_t ctr_render_ticks[CTR_TICKS];
/* counts since cleared */
enum { CTR_COUNT_MADE, CTR_COUNT_TEXELS, CTR_COUNT_FREED, CTR_COUNT_TRIS, CTR_COUNT_DRAWS, CTR_COUNTS };
extern uint32_t ctr_render_count[CTR_COUNTS];

#endif
/* pl_char_set and ef_char_set2 as some older modules declared them: the animation number passed as an
   int, so the caller extends it (extu.b) before the call. pl_char.c takes bytes. */
#ifndef PL_CHAR_OLD_H
#define PL_CHAR_OLD_H

extern void pl_char_set( PLAYER_WORK *pl_w, Uint8 group, int no );
extern void ef_char_set2( PLAYER_WORK *ef_w, Uint8 group, int no, int frame );

#ifdef PL_CHAR_SET2_BYTE
/* pl_char_set2 in a module that defines PL_CHAR_SET2_BYTE before player.h, with the frame as a byte:
   the caller works it out in eight bits and does not extend it. (PL_CHAR_SET2_OLD, all ints, is
   player.h's own switch.) */
extern void pl_char_set2( PLAYER_WORK *pl_w, Uint8 group, Sint8 no, Uint8 frame );
#endif

#endif

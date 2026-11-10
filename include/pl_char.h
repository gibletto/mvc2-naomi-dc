/* pl_char_set and ef_char_set2: set a work's animation, its group and number as bytes, as pl_char.c
   defines them. */
#ifndef PL_CHAR_H
#define PL_CHAR_H

extern void pl_char_set( PLAYER_WORK *pl_w, Uint8 group, Uint8 no );
extern void ef_char_set2( PLAYER_WORK *ef_w, Uint8 group, Uint8 no, int frame );

#endif

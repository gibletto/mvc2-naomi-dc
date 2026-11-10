/* The characters' animation data (data/char_XX.c): its records, and the rows of its tables. */
#ifndef CHAR_TBL_H
#define CHAR_TBL_H

#include "types.h"

/* a sprite's sizes and turn, by an animation frame's zoom number (cg_zoom) */
typedef struct SPR_PRM {
	Uint16	zm_x;		/* 000: the width, in thousandths */
	Uint16	zm_y;		/* 002: the height */
	Uint16	ang;		/* 004: added to the angle */
	Uint16	ofs_x;		/* 006: the centre it turns about, from the position (to disp_ofs_x, which is signed) */
	Uint16	ofs_y;		/* 008: the same, up and down (disp_ofs_y) */
	Uint16	flip;		/* 00a: flips it */
	Uint16	unk_0c[2];
} SPR_PRM;

/* the names Capcom's tables gave a frame's flags */
#define END_OF_DATA	0x80	/* end: the animation's last frame (the program's CG_END) */
#define ATK	0x80	/* atck: an attack, to its last frame that can hit (CG_ATCK_ON) */
#define ATE	0x40	/* atck: the attack's frames after that */
#define CSE	0x80	/* se: a sound effect, not a voice */

/* four bytes of a frame, as the word that holds them */
#define CG_BYTES( a, b, c, d )	( ( ( a ) & 0xff ) | ( ( b ) & 0xff ) << 8 | ( ( c ) & 0xff ) << 16 | ( Sint32 )( ( d ) & 0xff ) << 24 )

/* a player's frame (CHAR_DATA, 0x14 bytes): the number and the code for the program, its length in
   game frames, END_OF_DATA on the last one, the sprite, the draw priority, the shadow, the attack's
   phase, a number for the program, the sound, the SPR_PRM record, two more codes, the hold position
   and the frame's boxes (frame_box_XX) */
#define CG_FRAME( prm, type, ctr, end, spr, prio, shadow, atck, flag1, se, zoom, unk, flag2, rival, box ) \
	CG_BYTES( prm, type, ctr, end ), ( spr ), CG_BYTES( prio, shadow, atck, flag1 ), CG_BYTES( se, zoom, unk, flag2 ), \
	( ( rival ) & 0xffff ) | ( Sint32 )( box ) << 16

/* an effect's frame: the first eight bytes of a player's */
#define EF_FRAME( prm, type, ctr, end, spr )	CG_BYTES( prm, type, ctr, end ), ( spr )

/* after a last frame: the frame the animation goes on from */
#define CG_NEXT( to )	( Sint32 )( to )

#endif

/* The player work and the routines every character module uses. */
#ifndef PLAYER_H
#define PLAYER_H

#include "types.h"
#include "key.h"
#include "fighter.h"
#include "com.h"

/* a speed or point in the plane */
typedef struct VEC2 {
	Float	x, y;
} VEC2;

/* a point or offset in the stage */
typedef struct VEC3 {
	Float	x, y, z;
} VEC3;

/* a point before its divide by w */
typedef struct VEC4 {
	Float	x, y, z, w;
} VEC4;

/* a model's alpha and fade colour, the 16 bytes at PLAYER_WORK's 074 that effects copy whole */
typedef struct FADE_COL {
	Float	trnsl, r, g, b;
} FADE_COL;

/* a texture's header, as Kamui's */
typedef struct NLthd {
	Sint16	width, height;
	Sint32	type;
	void	*address;
	Sint32	globl_index;
} NLthd;

/* a file of models: their objects, and the textures they share */
typedef struct OBJ_FILE {
	int	**obj;		/* 000: the objects */
	Sint32	cnt;		/* 004: how many */
	NLthd	*thd;		/* 008: their textures, ended by one of width 0 */
} OBJ_FILE;

/* a 4x4 matrix, laid out as Kamui's */
typedef struct MATRIX {
	Float	m[4][4];
} MATRIX;

#include "nl.h"

/* a distance in the dots of Capcom's older boards (384 across, 224 down), as this screen has it */
#define DOT_X( n )	( ( n ) * 1.6666666f )
#define DOT_Y( n )	( ( n ) * 2.142857f )

/* one byte of a 16-bit field, read on its own */
#define HI_BYTE( w )	( ( (Sint8 *)&( w ) )[1] )
#define LO_BYTE( w )	( ( (Sint8 *)&( w ) )[0] )

/* the switches of one side (sw_work), or of the cabinet */
typedef struct SW_WORK {
	Uint16	now;		/* 000: held */
	Uint16	old;		/* 002: held the frame before */
	Uint16	on;		/* 004: newly pressed */
	Uint16	off;		/* 006: newly released */
	Uint16	trg_r;		/* 008: a pad's right trigger */
	Uint16	trg_l;		/* 00a: and its left */
	Sint16	stick_x;	/* 00c: a pad's stick */
	Sint16	stick_y;
	Uint16	rep;		/* 010: held keys given again while they repeat (key_repeat) */
	Uint16	rep_cnt;	/* 012: the frames they have been held */
} SW_WORK;

extern void *memset( void *s, int c, unsigned long n );

/* fabs() on a Float, done by the fabs instruction */
#define fabs( x )	_builtin_fabsf( x )

#include "pl_data.h"	/* the records of a character's data */
#include "pl_work.h"	/* PLAYER_WORK and the values of its fields */
#include "pl_def.h"	/* the player's constants */
#include "system.h"	/* SYSTEM_WORK, the ranking, the memory cards, files and sound */
#include "scr.h"	/* the screen: the camera, light, fog, fade, flash and text */
#include "vs_sys.h"	/* the fight */
#include "hit_work.h"	/* the works, their lists and the hit check */
#include "em_play.h"	/* the computer player */
#include "effect.h"	/* the effects and shots, by character */
#include "pl_sub.h"	/* the routines the character modules share */
#include "work.h"

#endif

/* Morrigan's effect 02 (character 03's pls02): the bats that fly round her. Eight of them are put
   out together; each follows one of her modes (lw->mode, set from what she does) and flies a
   table of steps round her, or flits off and back at random.
   Modes 4 to 13 send them out from her, round her, or up and back down on her. */
#include "player.h"
#include "ef_char.h"
#include "pl_char.h"
#include "pl03_def.h"

/* the effect's own work, from obj_mtx */
typedef struct PLS03_02_STEP {
	Sint16	x;		/* where it is from the middle of the loop, in 1/256 */
	Sint16	y;
	Sint16	prio;
	Sint16	unk_06;
} PLS03_02_STEP;

typedef struct PLS03_02_LOCAL_W {
	Sint8	mode;		/* 000: how it moves, from Morrigan's act (0 is none) */
	Sint8	old_mode;	/* 001: the mode it last started with */
	Uint8	unk_02;
	Sint8	flip;		/* 003: the loop is turned over */
	Sint16	idx;		/* 004: which step */
	Uint8	unk_06[2];
	PLS03_02_STEP	*tbl;	/* 008: the loop it flies */
	Sint16	add;		/* 00c: what it steps on by */
	Sint16	mask;		/* 00e: where the loop wraps */
	Float	dec_x;		/* 010: what the flight loses each frame, or the place it homes on */
	Float	dec_y;		/* 014 */
	Float	x;		/* 018: its place from Morrigan */
	Float	y;		/* 01c */
	Uint16	char_no;	/* 020: the animation she was in when it began */
} PLS03_02_LOCAL_W;
#define PLS03_02_LW( w )	( ( PLS03_02_LOCAL_W * )&( w )->obj_mtx )

/* where a bat sits beside Morrigan, in 1/256 */
typedef struct PLS03_02_OFS {
	Sint16	x;
	Sint16	y;
} PLS03_02_OFS;

/* one bat's loop: where it starts from Morrigan, its first step and the loop it flies */
typedef struct PLS03_02_LOOP {
	Sint16	x;
	Sint16	y;
	Sint16	idx;
	Sint16	unk_06;
	PLS03_02_STEP	*tbl;
	Sint16	add;
	Sint16	mask;
} PLS03_02_LOOP;

/* a flight: the speeds and accelerations it starts with, in 1/65536 */
typedef struct PLS03_02_FLY {
	Sint32	spd_x;
	Sint32	spd_y;
	Sint32	acc_x;
	Sint32	dec_x;
	Sint32	acc_y;
	Sint32	dec_y;
} PLS03_02_FLY;

/* where a bat sits for one of her frames, in 1/256, and whether it is turned over */
typedef struct PLS03_02_POS {
	Sint16	x;
	Sint16	y;
	Sint8	flip;
	Sint8	prio;
} PLS03_02_POS;

typedef struct PLS03_02_SPOT {
	Sint16	x;
	Sint16	y;
	Sint16	prio;
} PLS03_02_SPOT;

/* the same while she floats (mode 7), where the order is a byte */
typedef struct PLS03_02_FLOAT_SPOT {
	Sint16	x;
	Sint16	y;
	Sint8	prio;
} PLS03_02_FLOAT_SPOT;

typedef struct PLS03_02_DRIFT {
	Sint16	x;
	Sint16	y;
	Sint32	spd_x;
	Sint32	spd_y;
	Sint8	prio;
	Uint8	unk_0d[3];
} PLS03_02_DRIFT;

static PLS03_02_STEP pls03_02_tbl0[32] = {
	{ 0x0, 0x2, 0x2, 0x0 }, { 0x3, 0x2, 0x2, 0x0 }, { 0x8, 0x2, 0x2, 0x0 }, { 0xc, 0x1, 0x2, 0x0 },
	{ 0x10, 0x1, 0x2, 0x0 }, { 0x13, 0x1, 0x2, 0x0 }, { 0x15, 0x1, 0x2, 0x0 }, { 0x17, 0x1, 0x2, 0x0 },
	{ 0x18, 0x0, -0x2, 0x0 }, { 0x18, 0x1, -0x2, 0x0 }, { 0x17, 0x1, -0x2, 0x0 },
	{ 0x15, 0x1, -0x2, 0x0 }, { 0x12, 0x1, -0x2, 0x0 }, { 0xf, 0x1, -0x2, 0x0 },
	{ 0xa, 0x1, -0x2, 0x0 }, { 0x6, 0x2, -0x2, 0x0 }, { 0x1, 0x2, -0x2, 0x0 }, { -0x3, 0x2, -0x2, 0x0 },
	{ -0x7, 0x2, -0x2, 0x0 }, { -0xc, 0x1, -0x2, 0x0 }, { -0x10, 0x1, -0x2, 0x0 },
	{ -0x13, 0x1, -0x2, 0x0 }, { -0x16, 0x1, -0x2, 0x0 }, { -0x17, 0x1, -0x2, 0x0 },
	{ -0x18, 0x0, 0x2, 0x0 }, { -0x18, 0x1, 0x2, 0x0 }, { -0x17, 0x1, 0x2, 0x0 },
	{ -0x15, 0x1, 0x2, 0x0 }, { -0x12, 0x1, 0x2, 0x0 }, { -0xf, 0x1, 0x2, 0x0 },
	{ -0xb, 0x1, 0x2, 0x0 }, { -0x7, 0x2, 0x2, 0x0 },
} ;

static PLS03_02_STEP pls03_02_tbl1[128] = {
	{ 0x0, 0x2, -0x2, 0x0 }, { 0x0, 0x2, -0x2, 0x0 }, { 0x0, 0x2, -0x2, 0x0 }, { 0x0, 0x2, -0x2, 0x0 },
	{ 0x0, 0x2, -0x2, 0x0 }, { 0x1, 0x2, -0x2, 0x0 }, { 0x1, 0x2, -0x2, 0x0 }, { 0x2, 0x2, -0x2, 0x0 },
	{ 0x2, 0x2, -0x2, 0x0 }, { 0x3, 0x2, -0x2, 0x0 }, { 0x3, 0x1, -0x2, 0x0 }, { 0x4, 0x1, -0x2, 0x0 },
	{ 0x4, 0x1, -0x2, 0x0 }, { 0x4, 0x1, -0x2, 0x0 }, { 0x5, 0x1, -0x2, 0x0 }, { 0x5, 0x1, -0x2, 0x0 },
	{ 0x6, 0x1, -0x2, 0x0 }, { 0x6, 0x1, -0x2, 0x0 }, { 0x6, 0x1, -0x2, 0x0 }, { 0x8, 0x1, -0x2, 0x0 },
	{ 0x8, 0x1, -0x2, 0x0 }, { 0x8, 0x1, -0x2, 0x0 }, { 0x8, 0x1, -0x2, 0x0 }, { 0x9, 0x1, -0x2, 0x0 },
	{ 0x9, 0x1, -0x2, 0x0 }, { 0x9, 0x1, -0x2, 0x0 }, { 0x9, 0x0, -0x2, 0x0 }, { 0x9, 0x0, -0x2, 0x0 },
	{ 0x9, 0x0, -0x2, 0x0 }, { 0x9, 0x0, -0x2, 0x0 }, { 0x9, 0x0, -0x2, 0x0 }, { 0x9, 0x0, -0x2, 0x0 },
	{ 0xa, 0x0, -0x2, 0x0 }, { 0x9, 0x0, -0x2, 0x0 }, { 0x9, 0x0, -0x2, 0x0 }, { 0x9, 0x0, -0x2, 0x0 },
	{ 0x9, 0x0, -0x2, 0x0 }, { 0x9, 0x0, -0x2, 0x0 }, { 0x9, 0x0, -0x2, 0x0 }, { 0x9, -0x1, -0x2, 0x0 },
	{ 0x9, -0x1, -0x2, 0x0 }, { 0x9, -0x1, -0x2, 0x0 }, { 0x8, -0x1, -0x2, 0x0 },
	{ 0x8, -0x1, -0x2, 0x0 }, { 0x8, -0x1, -0x2, 0x0 }, { 0x8, -0x1, -0x2, 0x0 },
	{ 0x7, -0x2, -0x2, 0x0 }, { 0x7, -0x2, -0x2, 0x0 }, { 0x7, -0x2, -0x2, 0x0 },
	{ 0x6, -0x2, -0x2, 0x0 }, { 0x6, -0x2, -0x2, 0x0 }, { 0x5, -0x2, -0x2, 0x0 },
	{ 0x5, -0x2, -0x2, 0x0 }, { 0x5, -0x2, -0x2, 0x0 }, { 0x4, -0x2, -0x2, 0x0 },
	{ 0x4, -0x3, -0x2, 0x0 }, { 0x3, -0x3, -0x2, 0x0 }, { 0x3, -0x3, -0x2, 0x0 },
	{ 0x2, -0x3, -0x2, 0x0 }, { 0x2, -0x3, -0x2, 0x0 }, { 0x1, -0x3, -0x2, 0x0 },
	{ 0x1, -0x3, -0x2, 0x0 }, { 0x0, -0x3, -0x2, 0x0 }, { 0x0, -0x3, -0x2, 0x0 },
	{ 0x0, -0x3, -0x2, 0x0 }, { 0x0, -0x3, -0x2, 0x0 }, { 0x0, -0x3, -0x2, 0x0 },
	{ -0x1, -0x3, -0x2, 0x0 }, { -0x1, -0x3, -0x2, 0x0 }, { -0x2, -0x3, -0x2, 0x0 },
	{ -0x2, -0x3, -0x2, 0x0 }, { -0x3, -0x3, -0x2, 0x0 }, { -0x3, -0x3, -0x2, 0x0 },
	{ -0x4, -0x3, -0x2, 0x0 }, { -0x4, -0x2, -0x2, 0x0 }, { -0x5, -0x2, -0x2, 0x0 },
	{ -0x5, -0x2, -0x2, 0x0 }, { -0x5, -0x2, -0x2, 0x0 }, { -0x6, -0x2, -0x2, 0x0 },
	{ -0x6, -0x2, -0x2, 0x0 }, { -0x7, -0x2, -0x2, 0x0 }, { -0x7, -0x2, -0x2, 0x0 },
	{ -0x7, -0x2, -0x2, 0x0 }, { -0x8, -0x1, -0x2, 0x0 }, { -0x8, -0x1, -0x2, 0x0 },
	{ -0x8, -0x1, -0x2, 0x0 }, { -0x8, -0x1, -0x2, 0x0 }, { -0x9, -0x1, -0x2, 0x0 },
	{ -0x9, -0x1, -0x2, 0x0 }, { -0x9, -0x1, -0x2, 0x0 }, { -0x9, 0x0, -0x2, 0x0 },
	{ -0x9, 0x0, -0x2, 0x0 }, { -0x9, 0x0, -0x2, 0x0 }, { -0x9, 0x0, -0x2, 0x0 },
	{ -0x9, 0x0, -0x2, 0x0 }, { -0x9, 0x0, -0x2, 0x0 }, { -0xa, 0x0, -0x2, 0x0 },
	{ -0x9, 0x0, -0x2, 0x0 }, { -0x9, 0x0, -0x2, 0x0 }, { -0x9, 0x0, -0x2, 0x0 },
	{ -0x9, 0x0, -0x2, 0x0 }, { -0x9, 0x0, -0x2, 0x0 }, { -0x9, 0x0, -0x2, 0x0 },
	{ -0x9, 0x1, -0x2, 0x0 }, { -0x9, 0x1, -0x2, 0x0 }, { -0x9, 0x1, -0x2, 0x0 },
	{ -0x8, 0x1, -0x2, 0x0 }, { -0x8, 0x1, -0x2, 0x0 }, { -0x8, 0x1, -0x2, 0x0 },
	{ -0x8, 0x1, -0x2, 0x0 }, { -0x6, 0x1, -0x2, 0x0 }, { -0x6, 0x1, -0x2, 0x0 },
	{ -0x6, 0x1, -0x2, 0x0 }, { -0x5, 0x1, -0x2, 0x0 }, { -0x5, 0x1, -0x2, 0x0 },
	{ -0x4, 0x1, -0x2, 0x0 }, { -0x4, 0x1, -0x2, 0x0 }, { -0x4, 0x1, -0x2, 0x0 },
	{ -0x3, 0x1, -0x2, 0x0 }, { -0x3, 0x2, -0x2, 0x0 }, { -0x2, 0x2, -0x2, 0x0 },
	{ -0x2, 0x2, -0x2, 0x0 }, { -0x1, 0x2, -0x2, 0x0 }, { -0x1, 0x2, -0x2, 0x0 },
	{ 0x0, 0x2, -0x2, 0x0 }, { 0x0, 0x2, -0x2, 0x0 }, { 0x0, 0x2, -0x2, 0x0 }, { 0x0, 0x2, -0x2, 0x0 },
} ;

static PLS03_02_STEP pls03_02_tbl2[128] = {
	{ 0x0, 0x1, 0x2, 0x0 }, { 0x0, 0x1, 0x2, 0x0 }, { 0x0, 0x1, 0x2, 0x0 }, { 0x0, 0x1, 0x2, 0x0 },
	{ 0x0, 0x1, 0x2, 0x0 }, { 0x0, 0x1, 0x2, 0x0 }, { 0x1, 0x1, 0x2, 0x0 }, { 0x1, 0x1, 0x2, 0x0 },
	{ 0x1, 0x1, 0x2, 0x0 }, { 0x1, 0x1, 0x2, 0x0 }, { 0x2, 0x1, 0x2, 0x0 }, { 0x2, 0x1, 0x2, 0x0 },
	{ 0x3, 0x1, 0x2, 0x0 }, { 0x4, 0x1, 0x2, 0x0 }, { 0x4, 0x1, 0x2, 0x0 }, { 0x4, 0x1, 0x2, 0x0 },
	{ 0x4, 0x1, 0x2, 0x0 }, { 0x5, 0x1, 0x2, 0x0 }, { 0x5, 0x1, 0x2, 0x0 }, { 0x5, 0x1, 0x2, 0x0 },
	{ 0x5, 0x1, 0x2, 0x0 }, { 0x6, 0x1, 0x2, 0x0 }, { 0x6, 0x1, 0x2, 0x0 }, { 0x6, 0x0, 0x2, 0x0 },
	{ 0x6, 0x0, 0x2, 0x0 }, { 0x6, 0x0, 0x2, 0x0 }, { 0x6, 0x0, 0x2, 0x0 }, { 0x6, 0x0, 0x2, 0x0 },
	{ 0x6, 0x0, 0x2, 0x0 }, { 0x6, 0x0, 0x2, 0x0 }, { 0x6, 0x0, 0x2, 0x0 }, { 0x6, 0x0, 0x2, 0x0 },
	{ 0x7, 0x0, 0x2, 0x0 }, { 0x6, 0x0, 0x2, 0x0 }, { 0x6, 0x0, 0x2, 0x0 }, { 0x6, 0x0, 0x2, 0x0 },
	{ 0x6, 0x0, 0x2, 0x0 }, { 0x6, 0x0, 0x2, 0x0 }, { 0x6, 0x0, 0x2, 0x0 }, { 0x6, 0x0, 0x2, 0x0 },
	{ 0x6, 0x0, 0x2, 0x0 }, { 0x6, 0x0, 0x2, 0x0 }, { 0x6, -0x1, 0x2, 0x0 }, { 0x6, -0x1, 0x2, 0x0 },
	{ 0x5, -0x1, 0x2, 0x0 }, { 0x5, -0x1, 0x2, 0x0 }, { 0x5, -0x1, 0x2, 0x0 }, { 0x5, -0x1, 0x2, 0x0 },
	{ 0x4, -0x1, 0x2, 0x0 }, { 0x4, -0x1, 0x2, 0x0 }, { 0x4, -0x1, 0x2, 0x0 }, { 0x4, -0x1, 0x2, 0x0 },
	{ 0x3, -0x1, 0x2, 0x0 }, { 0x3, -0x2, 0x2, 0x0 }, { 0x3, -0x2, 0x2, 0x0 }, { 0x2, -0x2, 0x2, 0x0 },
	{ 0x2, -0x2, 0x2, 0x0 }, { 0x2, -0x2, 0x2, 0x0 }, { 0x2, -0x2, 0x2, 0x0 }, { 0x1, -0x2, 0x2, 0x0 },
	{ 0x1, -0x2, 0x2, 0x0 }, { 0x1, -0x2, 0x2, 0x0 }, { 0x0, -0x2, 0x2, 0x0 }, { 0x0, -0x2, 0x2, 0x0 },
	{ 0x0, -0x2, 0x2, 0x0 }, { 0x0, -0x2, 0x2, 0x0 }, { 0x0, -0x2, 0x2, 0x0 }, { -0x1, -0x2, 0x2, 0x0 },
	{ -0x1, -0x2, 0x2, 0x0 }, { -0x1, -0x2, 0x2, 0x0 }, { -0x2, -0x2, 0x2, 0x0 },
	{ -0x2, -0x2, 0x2, 0x0 }, { -0x2, -0x2, 0x2, 0x0 }, { -0x2, -0x2, 0x2, 0x0 },
	{ -0x3, -0x2, 0x2, 0x0 }, { -0x3, -0x2, 0x2, 0x0 }, { -0x3, -0x1, 0x2, 0x0 },
	{ -0x4, -0x1, 0x2, 0x0 }, { -0x4, -0x1, 0x2, 0x0 }, { -0x4, -0x1, 0x2, 0x0 },
	{ -0x4, -0x1, 0x2, 0x0 }, { -0x5, -0x1, 0x2, 0x0 }, { -0x5, -0x1, 0x2, 0x0 },
	{ -0x5, -0x1, 0x2, 0x0 }, { -0x5, -0x1, 0x2, 0x0 }, { -0x6, -0x1, 0x2, 0x0 },
	{ -0x6, -0x1, 0x2, 0x0 }, { -0x6, 0x0, 0x2, 0x0 }, { -0x6, 0x0, 0x2, 0x0 }, { -0x6, 0x0, 0x2, 0x0 },
	{ -0x6, 0x0, 0x2, 0x0 }, { -0x6, 0x0, 0x2, 0x0 }, { -0x6, 0x0, 0x2, 0x0 }, { -0x6, 0x0, 0x2, 0x0 },
	{ -0x6, 0x0, 0x2, 0x0 }, { -0x6, 0x0, 0x2, 0x0 }, { -0x7, 0x0, 0x2, 0x0 }, { -0x6, 0x0, 0x2, 0x0 },
	{ -0x6, 0x0, 0x2, 0x0 }, { -0x6, 0x0, 0x2, 0x0 }, { -0x6, 0x0, 0x2, 0x0 }, { -0x6, 0x0, 0x2, 0x0 },
	{ -0x6, 0x0, 0x2, 0x0 }, { -0x6, 0x0, 0x2, 0x0 }, { -0x6, 0x0, 0x2, 0x0 }, { -0x6, 0x0, 0x2, 0x0 },
	{ -0x6, 0x1, 0x2, 0x0 }, { -0x6, 0x1, 0x2, 0x0 }, { -0x5, 0x1, 0x2, 0x0 }, { -0x5, 0x1, 0x2, 0x0 },
	{ -0x5, 0x1, 0x2, 0x0 }, { -0x5, 0x1, 0x2, 0x0 }, { -0x4, 0x1, 0x2, 0x0 }, { -0x4, 0x1, 0x2, 0x0 },
	{ -0x4, 0x1, 0x2, 0x0 }, { -0x4, 0x1, 0x2, 0x0 }, { -0x3, 0x1, 0x2, 0x0 }, { -0x2, 0x1, 0x2, 0x0 },
	{ -0x2, 0x1, 0x2, 0x0 }, { -0x1, 0x1, 0x2, 0x0 }, { -0x1, 0x1, 0x2, 0x0 }, { -0x1, 0x1, 0x2, 0x0 },
	{ -0x1, 0x1, 0x2, 0x0 }, { 0x0, 0x1, 0x2, 0x0 }, { 0x0, 0x1, 0x2, 0x0 }, { 0x0, 0x1, 0x2, 0x0 },
	{ 0x0, 0x1, 0x2, 0x0 }, { 0x0, 0x1, 0x2, 0x0 },
} ;

static PLS03_02_FLY pls03_02_appear_fly_tbl[8] = {
	{ -39321, 135441, -270882, -17476, -71070, -2330 },
	{ -50244, 117964, -252823, -16347, -71070, -2330 },
	{ -52428, 98304, -216705, -13981, -71070, -2330 },
	{ -52428, 76458, -44418, -1456, -71070, -2330 },
	{ -13107, 174762, -361176, -23301, 451470, 29127 },
	{ 10922, 176947, 180588, 11650, 406323, 26214 },
	{ -2184, 41506, 45147, 2912, -45147, -2912 },
	{ 6553, 67720, -45147, -2912, -27088, -1747 },
} ;

static PLS03_02_POS pls03_02_pos[9][4] = {
	{ { -28, 28, 1, -2 }, { -13, 35, 1, -2 }, { -66, 10, 1, -2 }, { -2, 12, 1, -2 } },
	{ { -13, 1, 0, -2 }, { -58, 35, 0, -2 }, { -36, 6, 0, -2 }, { 2, 33, 0, -2 } },
	{ { -15, 24, 0, -2 }, { -49, 43, 0, -2 }, { -25, 59, 0, -2 }, { -26, 35, 0, -2 } },
	{ { -11, 26, 0, -2 }, { -13, 34, 0, -2 }, { -19, 41, 0, -2 }, { 5, 48, 0, -2 } },
	{ { -13, 26, 0, -2 }, { -17, 36, 0, -2 }, { -17, 46, 0, -2 }, { 0, 72, 0, -2 } },
	{ { -16388, 23, 0, -2 }, { -16387, 28, 0, -2 }, { -16420, 6, 0, -2 }, { -16385, 9, 0, -2 } },
	{ { -32748, 18, 0, -2 }, { -32761, 21, 0, -2 }, { 32762, 2, 0, -2 }, { -32768, 6, 0, -2 } },
	{ { 16428, 13, 0, -2 }, { 16401, 14, 0, -2 }, { 16408, -2, 0, -2 }, { 16385, 3, 0, -2 } },
	{ { 68, 8, 0, -2 }, { 27, 7, 0, -2 }, { 54, -6, 0, -2 }, { 2, 0, 0, -2 } },
} ;

static PLS03_02_FLY pls03_02_win_fly_tbl[8] = {
	{ 61166, 52428, 44418, 1456, -71070, -2330 },
	{ 58015, -15291, 44418, 1456, -71070, -2330 },
	{ 110318, 26214, 44418, 1456, -71070, -2330 },
	{ 91750, 8738, 44418, 1456, -71070, -2330 },
	{ -42598, 69905, 44418, 1456, -71070, -2330 },
	{ -93934, 34952, -71070, -2330, 44418, 1456 },
	{ -67720, -7645, -71070, -2330, 44418, 1456 },
	{ -70821, 52428, -71070, -2330, 44418, 1456 },
} ;

static PLS03_02_FLOAT_SPOT pls03_02_float_spot[8] = {
	{ -25, 23, 2 }, { -8, 28, 2 }, { -12, 22, 2 }, { -33, 22, 2 },
	{ -22, 19, 2 }, { 2, 21, 2 }, { -32, 33, 2 }, { -2, 45, 2 },
} ;

static PLS03_02_DRIFT pls03_02_drift_tbl[2] = {
	{ -9, 82, -8192, 6144, -2, 0, 0, 0 },
	{ 7, 81, 6144, 8192, 2, 0, 0, 0 },
} ;

static PLS03_02_SPOT pls03_02_soul_spot[4][9] = {
	{ { 48, -80, -2 }, { 48, -80, -2 }, { 72, -64, -2 }, { -56, -88, 2 }, { 40, -40, -2 }, { 40, -40, -2 }, { 40, -40, 2 }, { 40, -40, 2 }, { 0, 0, 0 } },
	{ { 16, -96, -2 }, { 32, -96, 2 }, { 32, -104, -2 }, { 56, 0, -2 }, { 16, 0, -2 }, { 56, 0, -2 }, { 32, 0, -2 }, { 32, 0, -2 }, { 0, 0, 0 } },
	{ { 0, 0, 0 }, { 0, 0, 0 }, { -16, -80, 2 }, { 56, -8, 2 }, { -8, -8, 2 }, { 56, -8, 2 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } },
	{ { 0, 0, 0 }, { 0, 0, 0 }, { -24, -48, -2 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } },
} ;

static PLS03_02_SPOT pls03_02_jsoul_spot[4][5] = {
	{ { 0, 0, 0 }, { 23, -84, 2 }, { 17, -84, -2 }, { -47, -52, -2 }, { -47, -52, -2 } },
	{ { 0, 0, 0 }, { 0, 0, 0 }, { -31, -92, -1 }, { -39, -60, -2 }, { 0, 0, 0 } },
	{ { 0, 0, 0 }, { 0, 0, 0 }, { -47, -68, -1 }, { -39, -52, -2 }, { 0, 0, 0 } },
	{ { 0, 0, 0 }, { 0, 0, 0 }, { -87, -84, 2 }, { 0, 0, 0 }, { 0, 0, 0 } },
} ;

static PLS03_02_FLY pls03_02_silhouette_fly_tbl[4] = {
	{ 61166, 52428, 44418, 1456, -71070, -2330 },
	{ 110318, 26214, 44418, 1456, -71070, -2330 },
	{ -42598, 69905, 44418, 1456, -71070, -2330 },
	{ -67720, -7645, -71070, -2330, 44418, 1456 },
} ;

Sint8 pls03_02_mode_get( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
void pls03_02_follow( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
void pls03_02_nop1( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
void pls03_02_char_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
void pls03_02_rise( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
void pls03_02_wait_a( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
void pls03_02_wait_b( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
void pls03_02_time_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
void pls03_02_nop2( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static int pls03_02_check_a( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static int pls03_02_check_b( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

static void pls03_02_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls03_02_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	int i ;

	for( i = 0 ; i < 8 ; i ++ ){
		if( ( ew = work_get( 0, W_PLS_ST, LIST_TOP ) ) ){
			PLS03_02_LOCAL_W *lw = PLS03_02_LW( ew ) ;

			ew->work_func = pls03_02_main ;
			ew->master_w = pl_w ;
			ew->pl_type = pl_w->pl_type ;
			ew->sub_type = i ;
			ew->ef_no = 0x302 ;
			lw->mode = 4 ;
		}
	}
	return ew ;
}

static void pls03_02_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_02_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_02_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_02_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls03_02_main( PLAYER_WORK *ew )
{
	static void ( *pls03_02_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls03_02_init, pls03_02_move, pls03_02_end, pls03_02_free,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->disp_flag = 0 ;
	ew->prio = pl_w->prio ;
	pls03_02_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls03_02_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, pl_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = pl_w->pl_no ;
	ew->pl_type = pl_w->pl_type ;
	ew->scale.x = pl_w->scale.x ;
	ew->scale.y = pl_w->scale.y ;
	ew->atck_level = pl_w->atck_level ;
	ew->work_no = pl_w->work_no ;
	ew->col_cnt = pl_w->col_cnt ;
	ew->scale = pl_w->scale ;
	ew->prio = pl_w->prio ;
	ef_char_set( ew, 27, 8 ) ;
	pls03_02_move( ew, pl_w ) ;
}

static void pls03_02_none( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_02_stand( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_02_walk( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_02_stop( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_02_appear( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_02_crouch( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_02_getup( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_02_float( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_02_win( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_02_drift( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_02_soul( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_02_jsoul( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_02_silhouette( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_02_mode13( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void pls03_02_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls03_02_mode_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls03_02_none, pls03_02_stand, pls03_02_walk, pls03_02_stop,
		pls03_02_appear, pls03_02_crouch, pls03_02_getup, pls03_02_float,
		pls03_02_win, pls03_02_drift, pls03_02_soul, pls03_02_jsoul,
		pls03_02_silhouette, pls03_02_mode13,
	} ;
	PLS03_02_LOCAL_W *lw = PLS03_02_LW( ew ) ;

	if( !pl_w->disp_flag ){
		ew->disp_flag = 0 ;
		return ;
	}
	if( lw->mode != lw->old_mode ){
		ew->routine_1 = ew->routine_2 = ew->routine_3 = 0 ;
		lw->old_mode = lw->mode ;
		return ;
	}
	pls03_02_mode_jmp[lw->mode & 15]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls03_02_none( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pls03_02_mode_get( ew, pl_w ) ;
}

/*==============================================================*/
/*			The bat is set out again: it rises from the			*/
/*			ground at a random place and time					*/
/*==============================================================*/
static void pls03_02_bat_a( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	pls03_02_nop1( ew, pl_w ) ;
	pls03_02_char_set( ew, pl_w ) ;
	pls03_02_rise( ew, pl_w ) ;
	pls03_02_wait_a( ew, pl_w ) ;
	pls03_02_time_set( ew, pl_w ) ;
}

/*==============================================================*/
static void pls03_02_bat_b( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	pls03_02_nop1( ew, pl_w ) ;
	pls03_02_char_set( ew, pl_w ) ;
	pls03_02_rise( ew, pl_w ) ;
	pls03_02_wait_b( ew, pl_w ) ;
	pls03_02_time_set( ew, pl_w ) ;
}

/*==============================================================*/
static void pls03_02_stand_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_1 ++ ;
	if( ew->sub_type < 4 ) pls03_02_bat_a( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls03_02_stand_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pls03_02_mode_get( ew, pl_w ) ) return ;
	if( ew->sub_type >= 4 ) return ;
	if( !ew->timer0 ){
		if( !-- ew->timer1 ) pls03_02_bat_a( ew, pl_w ) ;
		else {
			ef_char_move( ew ) ;
			ew->pos.x = pl_w->pos.x + ew->spd.x ;
			ew->pos.y += ew->spd.y ;
			pls03_02_follow( ew, pl_w ) ;
		}
	} else ew->timer0 -- ;
}

/*==============================================================*/
static void pls03_02_stand( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls03_02_stand_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls03_02_stand_start, pls03_02_stand_move,
	} ;

	PLAYER_WORK *w = ew ;

	pls03_02_stand_jmp[w->routine_1]( w, pl_w ) ;
}

/*==============================================================*/
static void pls03_02_walk_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static PLS03_02_LOOP pls03_02_loop[4] = {
		{ -22, 77, 0, 0, pls03_02_tbl1, 1, 127 },
		{ 18, 74, 64, 0, pls03_02_tbl2, 1, 127 },
		{ 0, 64, 0, 0, pls03_02_tbl0, 1, 31 },
		{ 0, 24, 12, 0, pls03_02_tbl0, -1, 31 },
	} ;
	PLS03_02_LOCAL_W *lw = PLS03_02_LW( ew ) ;

	ew->routine_1 ++ ;
	if( ew->sub_type >= 4 ) return ;
	pls03_02_nop1( ew, pl_w ) ;
	ew->disp_dir = pl_w->disp_dir ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	lw->x = ( pls03_02_loop[ew->sub_type].x << 8 ) * 1.6666666f / 256.0f ;
	lw->y = ( pls03_02_loop[ew->sub_type].y << 8 ) * 2.142857f / 256.0f ;
	lw->idx = pls03_02_loop[ew->sub_type].idx ;
	lw->tbl = pls03_02_loop[ew->sub_type].tbl ;
	lw->add = pls03_02_loop[ew->sub_type].add ;
	lw->mask = pls03_02_loop[ew->sub_type].mask ;
	if( !ew->disp_dir ) lw->x = -lw->x ;
	ew->pos.x += lw->x ;
	ew->pos.y += lw->y ;
	ef_char_set( ew, 27, 8 ) ;
}

/*--------------------------------------------------------------*/
static void pls03_02_walk_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS03_02_LOCAL_W *lw = PLS03_02_LW( ew ) ;
	PLS03_02_STEP *st ;
	Sint16 x, y ;

	if( pls03_02_mode_get( ew, pl_w ) ){
		if( pl_w->routine_1 != PL_NORM || pl_w->act_st != N_STAND ) return ;
		lw->mode = 3 ;
		ew->routine_1 = 0 ;
		pls03_02_stop( ew, pl_w ) ;
		return ;
	}
	if( ew->sub_type >= 4 ) return ;
	st = lw->tbl ;
	st += lw->idx ;
	x = st->x ;
	y = st->y ;
	if( !y ){
		if( ew->pos.y > pl_w->ground_y + 120.0f ) lw->flip = -1 ;
		if( ew->pos.y < pl_w->ground_y + DOT_Y( 24.0f ) ) lw->flip = 0 ;
	}
	if( lw->flip ) y = -y ;
	if( !ew->disp_dir ) x = -x ;
	ew->pos.x = pl_w->pos.x + ( x << 8 ) * 1.6666666f / 256.0f + lw->x ;
	if( ew->sub_type < 2 ) ew->pos.y = pl_w->pos.y + ( y << 8 ) * 2.142857f / 256.0f + lw->y ;
	else ew->pos.y += ( y << 8 ) * 2.142857f / 256.0f ;
	ew->sub_prio = st->prio ;
	lw->idx += lw->add ;
	lw->idx &= lw->mask ;
	ef_char_move( ew ) ;
	pls03_02_follow( ew, pl_w ) ;
}

/*==============================================================*/
static void pls03_02_walk( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls03_02_walk_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls03_02_walk_start, pls03_02_walk_move,
	} ;

	PLAYER_WORK *w = ew ;

	pls03_02_walk_jmp[w->routine_1]( w, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls03_02_stop_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS03_02_LOCAL_W *lw = PLS03_02_LW( ew ) ;

	if( pl_w->routine_1 != PL_NORM || pl_w->act_st != N_STAND || !HI_BYTE( pl_w->cg_rival ) ){
		lw->mode = 1 ;
		pls03_02_bat_a( ew, pl_w ) ;
		return ;
	}
	if( ew->sub_type >= 4 ) return ;
	ef_char_move( ew ) ;
	pls03_02_follow( ew, pl_w ) ;
}

/*==============================================================*/
static void pls03_02_stop_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static PLS03_02_OFS pls03_02_ofs[4] = {
	{ 0x1000, 0x2800 }, { 0x1400, 0x2000 }, { 0x1800, 0x1800 }, { 0x1c00, 0x1000 },
	} ;

	ew->routine_1 ++ ;
	if( ew->sub_type >= 4 ) return ;
	pls03_02_nop1( ew, pl_w ) ;
	ew->disp_dir = pl_w->disp_dir ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->sub_prio = 2 ;
	if( ew->disp_dir ) ew->pos.x -= pls03_02_ofs[ew->sub_type].x * 1.6666666f / 256.0f ;
	else ew->pos.x += pls03_02_ofs[ew->sub_type].x * 1.6666666f / 256.0f ;
	ew->pos.y += pls03_02_ofs[ew->sub_type].y * 2.142857f / 256.0f ;
	ef_char_set( ew, 27, 8 ) ;
	pls03_02_stop_move( ew, pl_w ) ;
}

/*==============================================================*/
static void pls03_02_stop( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls03_02_stop_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls03_02_stop_start, pls03_02_stop_move,
	} ;

	PLAYER_WORK *w = ew ;

	pls03_02_stop_jmp[w->routine_1]( w, pl_w ) ;
}

/*==============================================================*/
/*			Mode 4, her appearance: they fly out from her and	*/
/*			wait until it is over								*/
/*==============================================================*/
static void pls03_02_appear_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Sint16 pls03_02_ofs[8][3] = {
	{ 1024, -2048, -2 }, { 2560, -1024, -2 }, { 4608, -1280, -2 }, { 4864, 256, -2 },
	{ 0, 0, 2 }, { 4096, -2560, 2 }, { 1024, -2304, 2 }, { -2304, -3072, 2 },
	} ;

	ew->routine_1 ++ ;
	pls03_02_nop1( ew, pl_w ) ;
	ew->disp_dir = pl_w->disp_dir ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	if( ew->disp_dir ) ew->pos.x += pls03_02_ofs[ew->sub_type][0] * 1.6666666f / 256.0f ;
	else ew->pos.x -= pls03_02_ofs[ew->sub_type][0] * 1.6666666f / 256.0f ;
	ew->pos.y += pls03_02_ofs[ew->sub_type][1] * 2.142857f / 256.0f ;
	ew->sub_prio = ( Sint8 )pls03_02_ofs[ew->sub_type] ;
	ef_char_set2( ew, 27, 8, nlRand() & 3 ) ;
}

/*==============================================================*/
/*			One frame of a flight: speed and acceleration		*/
/*==============================================================*/
static void pls03_02_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS03_02_LOCAL_W *lw = PLS03_02_LW( ew ) ;

	ew->acc.x -= lw->dec_x ;
	ew->pos.x += ew->spd.x + ew->acc.x ;
	ew->acc.y -= lw->dec_y ;
	ew->pos.y += ew->spd.y + ew->acc.y ;
}

/*--------------------------------------------------------------*/
static void pls03_02_appear_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type ){
		ef_char_move( ew ) ;
		pls03_02_follow( ew, pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls03_02_appear_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !( ew->timer0 -- ) ) ew->routine_2 ++ ;
	pls03_02_fall( ew, pl_w ) ;
	ef_char_move( ew ) ;
	pls03_02_follow( ew, pl_w ) ;
}

/*==============================================================*/
static void pls03_02_appear_ready( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS03_02_LOCAL_W *lw = PLS03_02_LW( ew ) ;
	Sint32 spd_x, acc_x, dec_x ;

	if( !pl_w->cg_type ){
		ef_char_move( ew ) ;
		pls03_02_follow( ew, pl_w ) ;
		return ;
	}
	ew->routine_2 ++ ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	spd_x = pls03_02_appear_fly_tbl[ew->sub_type].spd_x ;
	ew->spd.y = pls03_02_appear_fly_tbl[ew->sub_type].spd_y * 2.142857f / 65536.0f ;
	acc_x = pls03_02_appear_fly_tbl[ew->sub_type].acc_x ;
	dec_x = pls03_02_appear_fly_tbl[ew->sub_type].dec_x ;
	ew->acc.y = pls03_02_appear_fly_tbl[ew->sub_type].acc_y * 2.142857f / 65536.0f ;
	lw->dec_y = pls03_02_appear_fly_tbl[ew->sub_type].dec_y * 2.142857f / 65536.0f ;
	if( !ew->disp_dir ){
		spd_x = -spd_x ;
		acc_x = -acc_x ;
		dec_x = -dec_x ;
	}
	ew->spd.x = spd_x * 1.6666666f / 65536.0f ;
	ew->acc.x = acc_x * 1.6666666f / 65536.0f ;
	lw->dec_x = dec_x * 1.6666666f / 65536.0f ;
	ew->timer0 = 30 ;
	pls03_02_appear_fly( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls03_02_appear_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls03_02_appear_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls03_02_appear_ready, pls03_02_appear_fly, pls03_02_appear_wait,
	} ;
	PLS03_02_LOCAL_W *lw = PLS03_02_LW( ew ) ;

	if( pl_w->char_no.b.grp != Char_Appear ){
		ew->routine_1 = ew->routine_2 = ew->routine_3 = 0 ;
		lw->mode = 1 ;
		pls03_02_char_set( ew, pl_w ) ;
		return ;
	}
	pls03_02_appear_move_jmp[ew->routine_2]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls03_02_appear( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls03_02_appear_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls03_02_appear_start, pls03_02_appear_move,
	} ;

	PLAYER_WORK *w = ew ;

	pls03_02_appear_jmp[w->routine_1]( w, pl_w ) ;
}

/*==============================================================*/
/*			Mode 5, crouching: they rise again from the			*/
/*			ground, sooner than when she stands					*/
/*==============================================================*/
static void pls03_02_crouch_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_1 ++ ;
	if( ew->sub_type < 4 ) pls03_02_bat_b( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls03_02_crouch_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pls03_02_mode_get( ew, pl_w ) ) return ;
	if( ew->sub_type >= 4 ) return ;
	if( ew->timer0 ){
		ew->timer0 -- ;
		return ;
	}
	if( !-- ew->timer1 ){
		pls03_02_bat_b( ew, pl_w ) ;
		return ;
	}
	ef_char_move( ew ) ;
	ew->pos.x = pl_w->pos.x + ew->spd.x ;
	ew->pos.y += ew->spd.y ;
	pls03_02_follow( ew, pl_w ) ;
}

/*==============================================================*/
static void pls03_02_crouch( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls03_02_crouch_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls03_02_crouch_start, pls03_02_crouch_move,
	} ;

	PLAYER_WORK *w = ew ;

	pls03_02_crouch_jmp[w->routine_1]( w, pl_w ) ;
}

/*==============================================================*/
/*			Mode 6, getting up: they sit about her where her	*/
/*			animation's frames say								*/
/*==============================================================*/
static void pls03_02_getup_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_1 ++ ;
	if( ew->sub_type >= 4 ) return ;
	pls03_02_nop1( ew, pl_w ) ;
	ef_char_set2( ew, 27, 8, nlRand() & 3 ) ;
}

/*--------------------------------------------------------------*/
static void pls03_02_getup_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float x, px, py ;

	if( pls03_02_mode_get( ew, pl_w ) ) return ;
	if( ew->sub_type >= 4 ) return ;
	if( pl_w->cg_type == 0x80 ) return ;
	px = pl_w->pos.x ;
	py = pl_w->pos.y ;
	x = ( pls03_02_pos[pl_w->cg_type][ew->sub_type].x << 8 ) * 1.6666666f / 256.0f ;
	if( !pl_w->disp_dir ) x = -x ;
	px += x ;
	ew->pos.x = px ;
	ew->pos.y = py + ( pls03_02_pos[pl_w->cg_type][ew->sub_type].y << 8 ) * 2.142857f / 256.0f ;
	ew->disp_dir = pls03_02_pos[pl_w->cg_type][ew->sub_type].flip ^ pl_w->disp_dir ;
	ew->sub_prio = pls03_02_pos[pl_w->cg_type][ew->sub_type].prio ;
	ef_char_move( ew ) ;
	pls03_02_follow( ew, pl_w ) ;
}

/*==============================================================*/
static void pls03_02_getup( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls03_02_getup_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls03_02_getup_start, pls03_02_getup_move,
	} ;

	pls03_02_getup_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
/*			Mode 7, floating: they sit at their places by her	*/
/*==============================================================*/
static void pls03_02_float_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_1 ++ ;
	if( ew->sub_type >= 4 ) return ;
	pls03_02_nop1( ew, pl_w ) ;
	ew->disp_dir = pl_w->disp_dir ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->spd.x = ( pls03_02_float_spot[ew->sub_type].x << 8 ) * 1.6666666f / 256.0f ;
	ew->spd.y = ( pls03_02_float_spot[ew->sub_type].y << 8 ) * 2.142857f / 256.0f ;
	ew->sub_prio = pls03_02_float_spot[ew->sub_type].prio ;
	if( !ew->disp_dir ) ew->spd.x = -ew->spd.x ;
	ef_char_set2( ew, 27, 8, nlRand() & 3 ) ;
}

/*--------------------------------------------------------------*/
static void pls03_02_float_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw03 = ( PL03_LOCAL_W * )pl_w->tlocal ;
	PLS03_02_LOCAL_W *lw = PLS03_02_LW( ew ) ;

	if( pl_w->routine_1 != PL_NORM || lw->mode != lw03->bat_mode ){
		lw->mode = 0 ;
		ew->routine_1 = 0 ;
		return ;
	}
	if( ew->sub_type >= 4 ) return ;
	if( !pl_w->cg_flag1 ) return ;
	ew->disp_dir = pl_w->disp_dir ;
	ew->pos.x = pl_w->pos.x + ew->spd.x ;
	ew->pos.y = pl_w->pos.y + ew->spd.y ;
	ef_char_move( ew ) ;
	pls03_02_follow( ew, pl_w ) ;
}

/*==============================================================*/
static void pls03_02_float( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls03_02_float_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls03_02_float_start, pls03_02_float_move,
	} ;

	pls03_02_float_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
/*			Mode 8, a win pose: they fly, then home in on her	*/
/*==============================================================*/
static void pls03_02_win_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS03_02_LOCAL_W *lw = PLS03_02_LW( ew ) ;
	Float spd_x, acc_x, dec_x ;

	ew->routine_1 ++ ;
	pls03_02_nop1( ew, pl_w ) ;
	ew->disp_dir = pl_w->disp_dir ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y + 120.0f ;
	spd_x = pls03_02_win_fly_tbl[ew->sub_type].spd_x ;
	ew->spd.y = pls03_02_win_fly_tbl[ew->sub_type].spd_y * 2.142857f / 65536.0f ;
	acc_x = pls03_02_win_fly_tbl[ew->sub_type].acc_x ;
	dec_x = pls03_02_win_fly_tbl[ew->sub_type].dec_x ;
	ew->acc.y = pls03_02_win_fly_tbl[ew->sub_type].acc_y * 2.142857f / 65536.0f ;
	lw->dec_y = pls03_02_win_fly_tbl[ew->sub_type].dec_y * 2.142857f / 65536.0f ;
	if( !ew->disp_dir ){
		spd_x = -spd_x ;
		acc_x = -acc_x ;
		dec_x = -dec_x ;
	}
	ew->spd.x = spd_x * 1.6666666f / 65536.0f ;
	ew->acc.x = acc_x * 1.6666666f / 65536.0f ;
	lw->dec_x = dec_x * 1.6666666f / 65536.0f ;
	ew->timer0 = 60 ;
	ef_char_set2( ew, 27, 8, nlRand() & 3 ) ;
}

/*--------------------------------------------------------------*/
static void pls03_02_win_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS03_02_LOCAL_W *lw = PLS03_02_LW( ew ) ;

	if( ew->timer0 -- ){
		pls03_02_fall( ew, pl_w ) ;
	} else {
		ew->routine_1 ++ ;
		lw->x = ew->pos.x - pl_w->pos.x ;
		lw->y = ew->pos.y - pl_w->pos.y ;
		lw->dec_x = ew->pos.x ;
		lw->dec_y = ew->pos.y ;
		ew->spd.x = 1.6666666f ;
		ew->acc.x = -0.104166664f ;
		ew->spd.y = -DOT_Y( 0.75f ) ;
		ew->acc.y = 0.0669642836f ;
		if( ew->sub_type & 2 ){
			ew->spd.x = -ew->spd.x ;
			ew->acc.x = -ew->acc.x ;
		}
		if( ew->sub_type & 1 ){
			ew->spd.y = -ew->spd.y ;
			ew->acc.y = -ew->acc.y ;
		}
	}
	ef_char_move( ew ) ;
	pls03_02_follow( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls03_02_win_home( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS03_02_LOCAL_W *lw = PLS03_02_LW( ew ) ;

	lw->dec_x = pl_w->pos.x + lw->x ;
	lw->dec_y = pl_w->pos.y + lw->y ;
	if( ew->spd.y > 0.0f ){
		if( lw->dec_y - ew->pos.y < 0.0f && ew->acc.y > 0.0f ) ew->acc.y = -ew->acc.y ;
	} else {
		if( lw->dec_y - ew->pos.y > 0.0f && ew->acc.y < 0.0f ) ew->acc.y = -ew->acc.y ;
	}
	if( ew->spd.x < 0.0f ){
		if( lw->dec_x - ew->pos.x > 0.0f && ew->acc.x < 0.0f ) ew->acc.x = -ew->acc.x ;
		if( lw->dec_x - ew->pos.x < 0.0f && ew->acc.x > 0.0f ) ew->acc.x = -ew->acc.x ;
	} else {
		if( lw->dec_x - ew->pos.x < 0.0f && ew->acc.x > 0.0f ) ew->acc.x = -ew->acc.x ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	ef_char_move( ew ) ;
	pls03_02_follow( ew, pl_w ) ;
}

/*==============================================================*/
static void pls03_02_win( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls03_02_win_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls03_02_win_start, pls03_02_win_fly, pls03_02_win_home,
	} ;
	PLS03_02_LOCAL_W *lw = PLS03_02_LW( ew ) ;
	PL03_LOCAL_W *lw03 = ( PL03_LOCAL_W * )pl_w->tlocal ;

	if( pl_w->act_st != N_WIN ){
		lw03->bat_mode = 0 ;
		lw->mode = 0 ;
	}
	pls03_02_win_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
/*			Mode 9, time over or a loss: two drift up from her	*/
/*==============================================================*/
static void pls03_02_drift_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float x, spd_x ;

	ew->routine_1 ++ ;
	if( ew->sub_type >= 2 ) return ;
	pls03_02_nop1( ew, pl_w ) ;
	ew->disp_dir = pl_w->disp_dir ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	x = ( pls03_02_drift_tbl[ew->sub_type].x << 8 ) * 1.6666666f / 256.0f ;
	ew->pos.y += ( pls03_02_drift_tbl[ew->type].y << 8 ) * 2.142857f / 256.0f ;
	spd_x = pls03_02_drift_tbl[ew->sub_type].spd_x * 1.6666666f / 65536.0f ;
	ew->spd.y = pls03_02_drift_tbl[ew->sub_type].spd_y * 2.142857f / 65536.0f ;
	ew->sub_prio = pls03_02_drift_tbl[ew->sub_type].prio ;
	if( !ew->disp_dir ){
		x = -x ;
		spd_x = -spd_x ;
	}
	ew->acc.x = ew->acc.y = 0.0f ;
	ew->pos.x = ew->pos.x + x ;
	ew->spd.x = spd_x ;
	ef_char_set2( ew, 27, 8, nlRand() & 3 ) ;
}

/*--------------------------------------------------------------*/
static void pls03_02_drift_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ew->sub_type >= 2 ) return ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	ef_char_move( ew ) ;
	pls03_02_follow( ew, pl_w ) ;
}

/*==============================================================*/
static void pls03_02_drift( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls03_02_drift_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls03_02_drift_start, pls03_02_drift_move,
	} ;
	PLS03_02_LOCAL_W *lw = PLS03_02_LW( ew ) ;
	PL03_LOCAL_W *lw03 = ( PL03_LOCAL_W * )pl_w->tlocal ;

	if( pl_w->act_st != N_WIN ){
		lw03->bat_mode = 0 ;
		lw->mode = 0 ;
	}
	pls03_02_drift_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
/*			Mode 10, the Soul Fist: they sit where her frames	*/
/*			put them											*/
/*==============================================================*/
static void pls03_02_soul_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS03_02_LOCAL_W *lw = PLS03_02_LW( ew ) ;

	ew->routine_1 ++ ;
	if( ew->sub_type < 4 ){
		lw->char_no = pl_w->char_no.w ;
		pls03_02_nop2( ew, pl_w ) ;
		ef_char_set( ew, 27, ew->sub_type + 11 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls03_02_soul_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS03_02_LOCAL_W *lw = PLS03_02_LW( ew ) ;
	PL03_LOCAL_W *lw03 = ( PL03_LOCAL_W * )pl_w->tlocal ;

	if( pl_w->routine_1 != PL_NORM || lw->char_no != pl_w->char_no.w ){
		lw->mode = 0 ;
		lw03->bat_mode = 0 ;
		ew->routine_1 = 0 ;
		return ;
	}
	if( ew->sub_type >= 4 ) return ;
	ef_char_set2( ew, 27, ew->sub_type + 11, HI_BYTE( pl_w->cg_rival ) ) ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->disp_dir = 0 ;
	ew->spd.x = ( pls03_02_soul_spot[ew->sub_type][HI_BYTE( pl_w->cg_rival )].x << 8 ) * 1.6666666f / 256.0f ;
	ew->spd.y = ( pls03_02_soul_spot[ew->sub_type][HI_BYTE( pl_w->cg_rival )].y << 8 ) * 2.142857f / 256.0f ;
	ew->sub_prio = pls03_02_soul_spot[ew->sub_type][HI_BYTE( pl_w->cg_rival )].prio ;
	if( pl_w->disp_dir ){
		ew->spd.x = -ew->spd.x ;
		ew->disp_dir = ew->disp_dir ^ 1 ;
	}
	ew->pos.x += ew->spd.x ;
	ew->pos.y -= ew->spd.y ;
	pls03_02_follow( ew, pl_w ) ;
}

/*==============================================================*/
static void pls03_02_soul( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls03_02_soul_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls03_02_soul_start, pls03_02_soul_move,
	} ;
	PLS03_02_LOCAL_W *lw = PLS03_02_LW( ew ) ;
	PL03_LOCAL_W *lw03 = ( PL03_LOCAL_W * )pl_w->tlocal ;

	if( pls03_02_check_a( ew, pl_w ) ){
		lw03->bat_mode = 0 ;
		lw->mode = 0 ;
	}
	pls03_02_soul_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
/*			Mode 11, the Soul Fist in the air: the same			*/
/*==============================================================*/
static void pls03_02_jsoul_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS03_02_LOCAL_W *lw = PLS03_02_LW( ew ) ;

	ew->routine_1 ++ ;
	if( ew->sub_type < 4 ){
		lw->char_no = pl_w->char_no.w ;
		pls03_02_nop2( ew, pl_w ) ;
		ef_char_set( ew, 27, ew->sub_type + 15 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls03_02_jsoul_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS03_02_LOCAL_W *lw = PLS03_02_LW( ew ) ;
	PL03_LOCAL_W *lw03 = ( PL03_LOCAL_W * )pl_w->tlocal ;

	if( pl_w->routine_1 != PL_NORM || lw->char_no != pl_w->char_no.w ){
		lw->mode = 0 ;
		lw03->bat_mode = 0 ;
		ew->routine_1 = 0 ;
		return ;
	}
	if( ew->sub_type >= 4 ) return ;
	ef_char_set2( ew, 27, ew->sub_type + 15, HI_BYTE( pl_w->cg_rival ) ) ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->disp_dir = 0 ;
	ew->spd.x = ( pls03_02_jsoul_spot[ew->sub_type][HI_BYTE( pl_w->cg_rival )].x << 8 ) * 1.6666666f / 256.0f ;
	ew->spd.y = ( pls03_02_jsoul_spot[ew->sub_type][HI_BYTE( pl_w->cg_rival )].y << 8 ) * 2.142857f / 256.0f ;
	ew->sub_prio = pls03_02_jsoul_spot[ew->sub_type][HI_BYTE( pl_w->cg_rival )].prio ;
	if( pl_w->disp_dir ){
		ew->spd.x = -ew->spd.x ;
		ew->disp_dir = ew->disp_dir ^ 1 ;
	}
	ew->pos.x += ew->spd.x ;
	ew->pos.y -= ew->spd.y ;
	pls03_02_follow( ew, pl_w ) ;
}

/*==============================================================*/
static void pls03_02_jsoul( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls03_02_jsoul_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls03_02_jsoul_start, pls03_02_jsoul_move,
	} ;
	PLS03_02_LOCAL_W *lw = PLS03_02_LW( ew ) ;
	PL03_LOCAL_W *lw03 = ( PL03_LOCAL_W * )pl_w->tlocal ;

	if( pls03_02_check_a( ew, pl_w ) ){
		lw03->bat_mode = 0 ;
		lw->mode = 0 ;
	}
	pls03_02_jsoul_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
/*			Mode 12, Silhouette Blade: the first shows on her	*/
/*			in a colour; the other four fly out and home in		*/
/*==============================================================*/
static void pls03_02_silhouette_first_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Sint32 pls03_02_col[6] = {
	126, 127, 128, 129, 130, 131,
	} ;

	ew->routine_1 ++ ;
	ew->sub_prio = -2 ;
	ef_color_set( pl_w, 2, pls03_02_col[pl_w->pl_color], 1 ) ;
	pls03_02_nop1( ew, pl_w ) ;
	ef_char_set( ew, 27, 19 ) ;
}

/*--------------------------------------------------------------*/
static void pls03_02_silhouette_first_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS03_02_LOCAL_W *lw = PLS03_02_LW( ew ) ;
	PL03_LOCAL_W *lw03 = ( PL03_LOCAL_W * )pl_w->tlocal ;

	if( !pl_w->routine_1 && pl_w->act_st == N_SUPER && pl_w->pl_sp_type == 9 ){
		if( !HI_BYTE( pl_w->cg_rival ) ) return ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		ew->disp_dir = pl_w->disp_dir ;
		ef_char_move( ew ) ;
		pls03_02_follow( ew, pl_w ) ;
		return ;
	}
	lw03->bat_mode = 0 ;
	lw->mode = 0 ;
}

/*==============================================================*/
static void pls03_02_silhouette_first( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls03_02_silhouette_first_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls03_02_silhouette_first_start, pls03_02_silhouette_first_move,
	} ;

	pls03_02_silhouette_first_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls03_02_silhouette_rest_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS03_02_LOCAL_W *lw = PLS03_02_LW( ew ) ;

	ew->routine_1 ++ ;
	pls03_02_nop1( ew, pl_w ) ;
	ew->disp_dir = pl_w->disp_dir ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y + 120.0f ;
	if( ew->sub_type > 4 ) return ;
	ew->spd.x = pls03_02_silhouette_fly_tbl[ew->sub_type - 1].spd_x * 1.6666666f / 65536.0f ;
	ew->spd.y = pls03_02_silhouette_fly_tbl[ew->sub_type - 1].spd_y * 2.142857f / 65536.0f ;
	ew->acc.x = pls03_02_silhouette_fly_tbl[ew->sub_type - 1].acc_x * 1.6666666f / 65536.0f ;
	lw->dec_x = pls03_02_silhouette_fly_tbl[ew->sub_type - 1].dec_x * 1.6666666f / 65536.0f ;
	ew->acc.y = pls03_02_silhouette_fly_tbl[ew->sub_type - 1].acc_y * 2.142857f / 65536.0f ;
	lw->dec_y = pls03_02_silhouette_fly_tbl[ew->sub_type - 1].dec_y * 2.142857f / 65536.0f ;
	if( !ew->disp_dir ){
		ew->spd.x = -ew->spd.x ;
		ew->acc.x = -ew->acc.x ;
		lw->dec_x = -lw->dec_x ;
	}
	ew->sub_prio = -2 ;
	ew->timer0 = 60 ;
	ef_char_set2( ew, 27, 8, nlRand() & 3 ) ;
}

/*==============================================================*/
static void pls03_02_silhouette_rest_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS03_02_LOCAL_W *lw = PLS03_02_LW( ew ) ;

	if( ew->timer0 -- ){
		pls03_02_fall( ew, pl_w ) ;
	} else {
		ew->routine_2 ++ ;
		lw->x = ew->pos.x - pl_w->pos.x ;
		lw->y = ew->pos.y - pl_w->pos.y ;
		lw->dec_x = ew->pos.x ;
		lw->dec_y = ew->pos.y ;
		ew->spd.x = 1.6666666f ;
		ew->acc.x = -0.104166664f ;
		ew->spd.y = -DOT_Y( 0.75f ) ;
		ew->acc.y = 0.0669642836f ;
		if( ew->sub_type & 2 ){
			ew->spd.x = -ew->spd.x ;
			ew->acc.x = -ew->acc.x ;
		}
		if( ew->sub_type & 1 ){
			ew->spd.y = -ew->spd.y ;
			ew->acc.y = -ew->acc.y ;
		}
	}
	ef_char_move( ew ) ;
	pls03_02_follow( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls03_02_silhouette_rest_home( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS03_02_LOCAL_W *lw = PLS03_02_LW( ew ) ;

	lw->dec_x = lw->x + pl_w->pos.x ;
	lw->dec_y = lw->y + pl_w->pos.y ;
	if( ew->spd.y > 0.0f ){
		if( lw->dec_y - ew->pos.y > 0.0f && ew->acc.y < 0.0f ) ew->acc.y = -ew->acc.y ;
		if( lw->dec_y - ew->pos.y < 0.0f && ew->acc.y > 0.0f ) ew->acc.y = -ew->acc.y ;
	} else {
		if( lw->dec_y - ew->pos.y < 0.0f && ew->acc.y > 0.0f ) ew->acc.y = -ew->acc.y ;
	}
	if( ew->spd.x < 0.0f ){
		if( lw->dec_x - ew->pos.x > 0.0f && ew->acc.x < 0.0f ) ew->acc.x = -ew->acc.x ;
		if( lw->dec_x - ew->pos.x < 0.0f && ew->acc.x > 0.0f ) ew->acc.x = -ew->acc.x ;
	} else {
		if( lw->dec_x - ew->pos.x < 0.0f && ew->acc.x > 0.0f ) ew->acc.x = -ew->acc.x ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	ef_char_move( ew ) ;
	pls03_02_follow( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls03_02_silhouette_rest_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls03_02_silhouette_rest_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls03_02_silhouette_rest_fly, pls03_02_silhouette_rest_home,
	} ;
	PLS03_02_LOCAL_W *lw = PLS03_02_LW( ew ) ;

	if( pl_w->routine_1 || pl_w->act_st != N_SUPER || pl_w->pl_sp_type != 9 ){
		lw->mode = 0 ;
		return ;
	}
	if( ew->sub_type > 4 ) return ;
	pls03_02_silhouette_rest_move_jmp[ew->routine_2]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls03_02_silhouette_rest( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls03_02_silhouette_rest_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls03_02_silhouette_rest_start, pls03_02_silhouette_rest_move,
	} ;

	pls03_02_silhouette_rest_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls03_02_silhouette( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS03_02_LOCAL_W *lw = PLS03_02_LW( ew ) ;
	PL03_LOCAL_W *lw03 = ( PL03_LOCAL_W * )pl_w->tlocal ;

	if( pls03_02_check_b( ew, pl_w ) ){
		lw03->bat_mode = 0 ;
		lw->mode = 0 ;
		return ;
	}
	if( ew->sub_type > 0 ) pls03_02_silhouette_rest( ew, pl_w ) ;
	else pls03_02_silhouette_first( ew, pl_w ) ;
}

/*==============================================================*/
static void pls03_02_mode13( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pls03_02_mode_get( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls03_02_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls03_02_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/****************************************************************/
Sint8 pls03_02_mode_get( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Sint8 pls03_02_mode_tbl[36] = {
	0x1, 0x2, 0x0, 0x0, 0x0, 0x0, 0x5, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x6, 0x0, 0x0,
	0x0, -0x1, -0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, -0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
	} ;
	PL03_LOCAL_W *lw03 = ( PL03_LOCAL_W * )pl_w->tlocal ;
	PLS03_02_LOCAL_W *lw = PLS03_02_LW( ew ) ;
	Sint8 mode ;

	if( pl_w->routine_1 != PL_NORM ){
		lw->mode = 0 ;
		return -1 ;
	}
	mode = pls03_02_mode_tbl[pl_w->act_st] ;
	if( mode < 0 ) mode = lw03->bat_mode ;
	lw->mode = mode ;
	return lw->mode - lw->old_mode ;
}

/****************************************************************/
void pls03_02_follow( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw03 = ( PL03_LOCAL_W * )pl_w->tlocal ;
	PLS03_02_LOCAL_W *lw = PLS03_02_LW( ew ) ;

	if( pl_w->be_flag && pl_w->disp_flag ) ew->disp_flag = 1 ;
	else {
		lw03->bat_mode = 0 ;
		lw->mode = 0 ;
	}
}

/****************************************************************/
void pls03_02_nop1( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
}

/****************************************************************/
void pls03_02_nop2( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
}

/****************************************************************/
void pls03_02_char_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ef_char_set( ew, 27, ( nlRand() & 3 ) + 4 ) ;
}

/****************************************************************/
void pls03_02_rise( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Sint16 spd_x ;
	static Sint16 pls03_02_spd_y[4] = {
	0x100, 0xd0, 0xb0, 0x80,
	} ;

	ew->acc.x = ew->acc.y = 0.0f ;
	spd_x = 0xe000 + ( nlRand() & 0x3f00 ) ;
	ew->spd.x = spd_x * 1.6666666f / 256.0f ;
	ew->pos.y = pl_w->pos.y - DOT_Y( 32.0f ) ;
	if( nlRand() & 1 ) ew->sub_prio = 2 ;
	else ew->sub_prio = -2 ;
	ew->spd.y = pls03_02_spd_y[nlRand() & 3] * 2.142857f / 256.0f ;
}

/****************************************************************/
void pls03_02_wait_a( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Sint16 pls03_02_wait[16] = {
	0x68, 0x70, 0x78, 0x80, 0x68, 0x70, 0x78, 0x80, 0x68, 0x70, 0x78, 0x80, 0x68, 0x70, 0x78, 0x80,
	} ;

	ew->timer1 = pls03_02_wait[nlRand() & 15] ;
}

/****************************************************************/
void pls03_02_wait_b( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Sint16 pls03_02_wait[16] = {
	0x28, 0x30, 0x38, 0x40, 0x28, 0x30, 0x38, 0x40, 0x28, 0x30, 0x38, 0x40, 0x28, 0x30, 0x38, 0x40,
	} ;

	ew->timer1 = pls03_02_wait[nlRand() & 15] ;
}

/****************************************************************/
void pls03_02_time_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->timer0 = ( nlRand() & 7 ) + 7 ;
}

/*==============================================================*/
static int pls03_02_check_a( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw03 = ( PL03_LOCAL_W * )pl_w->tlocal ;
	PLS03_02_LOCAL_W *lw = PLS03_02_LW( ew ) ;

	if( pl_w->act_st == N_SPECIAL && lw03->bat_mode && lw03->bat_mode != lw->mode ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static int pls03_02_check_b( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw03 = ( PL03_LOCAL_W * )pl_w->tlocal ;
	PLS03_02_LOCAL_W *lw = PLS03_02_LW( ew ) ;

	if( pl_w->act_st == N_SUPER && lw03->bat_mode && lw03->bat_mode != lw->mode ) return 1 ;
	return 0 ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls03_02.c" End of File	*/

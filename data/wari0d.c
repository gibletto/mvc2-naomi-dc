/* Hulk (character 0d): the computer's lists for cutting in.
   What it does when something happens in the fight. PLAYER_WORK's cpu_adrs[2] points here; the
   event checks of em_play.c start the modes.

   wari0d_tbl        a record (CPU_DATA) for each mode from 2 on: the offsets of its tables.
   wari0d_rate_MM    the chance, out of 32, that mode MM is started, by the level of play.
   wari0d_sel_MM     32 picks; one is taken at random.
   wari0d_cmd_MM     the mode's lists: a pick is a number in this table.
   wari0d_cmd_MM_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name
                    a list of lib0d.c. */
#include "player.h"
#include "cpu_cmd.h"

extern const Uint8 *const wari0d_cmd_02[], *const wari0d_cmd_03[], *const wari0d_cmd_05[],
	*const wari0d_cmd_06[], *const wari0d_cmd_07[], *const wari0d_cmd_08[], *const wari0d_cmd_09[],
	*const wari0d_cmd_10[], *const wari0d_cmd_11[], *const wari0d_cmd_12[];
extern const Uint8 wari0d_rate_02[], wari0d_sel_02[], wari0d_rate_03[], wari0d_rate_04[],
	wari0d_sel_03[], wari0d_rate_05[], wari0d_sel_05[], wari0d_rate_06[], wari0d_sel_06[],
	wari0d_rate_07[], wari0d_sel_07[], wari0d_rate_08[], wari0d_sel_08[], wari0d_rate_09[],
	wari0d_sel_09[], wari0d_rate_10[], wari0d_sel_10[], wari0d_rate_11[], wari0d_sel_11[],
	wari0d_rate_12[], wari0d_sel_12[], wari0d_rate_13[], wari0d_cmd_02_00[], wari0d_cmd_02_01[],
	wari0d_cmd_02_02[], wari0d_cmd_02_03[], wari0d_cmd_03_00[], wari0d_cmd_05_00[],
	wari0d_cmd_05_01[], wari0d_cmd_05_02[], wari0d_cmd_05_03[], wari0d_cmd_06_00[],
	wari0d_cmd_07_00[], wari0d_cmd_07_01[], wari0d_cmd_07_02[], wari0d_cmd_07_03[],
	wari0d_cmd_08_00[], wari0d_cmd_09_00[], wari0d_cmd_09_01[], wari0d_cmd_09_02[],
	wari0d_cmd_10_00[], wari0d_cmd_10_01[], wari0d_cmd_10_02[], wari0d_cmd_11_00[],
	wari0d_cmd_11_01[], wari0d_cmd_11_02[], wari0d_cmd_12_00[], wari0d_cmd_12_01[],
	wari0d_cmd_12_02[], wari0d_cmd_12_03[];

const CPU_DATA wari0d_tbl[] = {
	{ ( Sint32 )wari0d_rate_02, ( Sint32 )wari0d_sel_02, ( Sint32 )wari0d_cmd_02 },	/* 2: a throw would catch the opponent */
	{ ( Sint32 )wari0d_rate_03, ( Sint32 )wari0d_sel_03, ( Sint32 )wari0d_cmd_03 },	/* 3: it guards a hit (the chance is the program's own) */
	{ ( Sint32 )wari0d_rate_04, ( Sint32 )wari0d_sel_03, ( Sint32 )wari0d_cmd_03 },	/* 4: never started: a guard in the air is mode 3 */
	{ ( Sint32 )wari0d_rate_05, ( Sint32 )wari0d_sel_05, ( Sint32 )wari0d_cmd_05 },	/* 5: after it has guarded on the ground */
	{ ( Sint32 )wari0d_rate_06, ( Sint32 )wari0d_sel_06, ( Sint32 )wari0d_cmd_06 },	/* 6: after it has guarded in the air */
	{ ( Sint32 )wari0d_rate_07, ( Sint32 )wari0d_sel_07, ( Sint32 )wari0d_cmd_07 },	/* 7: after it has been hit */
	{ ( Sint32 )wari0d_rate_08, ( Sint32 )wari0d_sel_08, ( Sint32 )wari0d_cmd_08 },	/* 8: never started */
	{ ( Sint32 )wari0d_rate_09, ( Sint32 )wari0d_sel_09, ( Sint32 )wari0d_cmd_09 },	/* 9: after it has got up */
	{ ( Sint32 )wari0d_rate_10, ( Sint32 )wari0d_sel_10, ( Sint32 )wari0d_cmd_10 },	/* 10: the opponent dashes */
	{ ( Sint32 )wari0d_rate_11, ( Sint32 )wari0d_sel_11, ( Sint32 )wari0d_cmd_11 },	/* 11: the opponent flies or super jumps */
	{ ( Sint32 )wari0d_rate_12, ( Sint32 )wari0d_sel_12, ( Sint32 )wari0d_cmd_12 },	/* 12: the opponent jumps */
	{ ( Sint32 )wari0d_rate_13, 0, 0 },	/* 13: an attack is coming: the chance alone, the lists are yok0d.c's */
};

const Uint8 wari0d_rate_02[] = {
	  9,   9,  10,  10,  11,  11,  12,  12,  13,  13,  14,  14,  15,  15,  16,  16,
	 17,  18,  19,  20,  21,  22,  23,  24,  24,  24,  24,  24,  24,  24,  24,  24,
};

const Uint8 wari0d_sel_02[] = {
	  0,   0,   0,   0,   0,   0,   0,   2,   0,   0,   0,   0,   0,   0,   0,   2,
	  1,   1,   1,   1,   1,   1,   3,   3,   1,   1,   1,   1,   1,   1,   3,   3,
};

const Uint8 *const wari0d_cmd_02[] = {
	wari0d_cmd_02_00, wari0d_cmd_02_01, wari0d_cmd_02_02, wari0d_cmd_02_03,
};

const Uint8 wari0d_rate_03[] = {
	  1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   3,   3,   3,
};

const Uint8 wari0d_rate_04[] = {
	 16,  16,  17,  17,  18,  19,  20,  21,  22,  23,  24,  25,  25,  25,  25,  25,
	 25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,
};

const Uint8 wari0d_sel_03[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const wari0d_cmd_03[] = {
	wari0d_cmd_03_00,
};

const Uint8 wari0d_rate_05[] = {
	 19,  19,  20,  20,  21,  21,  22,  22,  23,  23,  24,  24,  25,  25,  26,  26,
	 27,  27,  28,  28,  29,  29,  30,  30,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari0d_sel_05[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   3,   3,   3,   3,
};

const Uint8 *const wari0d_cmd_05[] = {
	wari0d_cmd_05_00, wari0d_cmd_05_01, wari0d_cmd_05_02, wari0d_cmd_05_03,
};

const Uint8 wari0d_rate_06[] = {
	  4,   4,   4,   8,  12,  16,  18,  20,  22,  24,  26,  28,  30,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari0d_sel_06[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const wari0d_cmd_06[] = {
	wari0d_cmd_06_00,
};

const Uint8 wari0d_rate_07[] = {
	  4,   4,   5,   5,   6,   6,   7,   7,   8,   8,   9,   9,  10,  10,  11,  11,
	 12,  12,  12,  12,  13,  13,  13,  13,  14,  14,  14,  14,  15,  15,  15,  15,
};

const Uint8 wari0d_sel_07[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   3,   3,   3,   3,
};

const Uint8 *const wari0d_cmd_07[] = {
	wari0d_cmd_07_00, wari0d_cmd_07_01, wari0d_cmd_07_02, wari0d_cmd_07_03,
};

const Uint8 wari0d_rate_08[] = {
	  4,   4,   4,   8,  12,  16,  18,  20,  22,  24,  26,  28,  30,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari0d_sel_08[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const wari0d_cmd_08[] = {
	wari0d_cmd_08_00,
};

const Uint8 wari0d_rate_09[] = {
	 24,  25,  26,  27,  28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari0d_sel_09[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,
};

const Uint8 *const wari0d_cmd_09[] = {
	wari0d_cmd_09_00, wari0d_cmd_09_01, wari0d_cmd_09_02,
};

const Uint8 wari0d_rate_10[] = {
	  1,   2,   3,   4,   5,   6,   7,   7,   7,   8,   8,   8,   9,   9,   9,  10,
	 10,  10,  11,  12,  13,  14,  15,  16,  16,  16,  16,  16,  16,  16,  16,  16,
};

const Uint8 wari0d_sel_10[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,
};

const Uint8 *const wari0d_cmd_10[] = {
	wari0d_cmd_10_00, wari0d_cmd_10_01, wari0d_cmd_10_02,
};

const Uint8 wari0d_rate_11[] = {
	  0,   0,   1,   1,   2,   2,   3,   3,   4,   4,   5,   5,   6,   6,   7,   7,
	  8,   8,   9,   9,  10,  10,  11,  11,  12,  11,  12,  12,  13,  13,  14,  14,
};

const Uint8 wari0d_sel_11[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,
};

const Uint8 *const wari0d_cmd_11[] = {
	wari0d_cmd_11_00, wari0d_cmd_11_01, wari0d_cmd_11_02,
};

const Uint8 wari0d_rate_12[] = {
	  0,   0,   1,   1,   2,   2,   3,   3,   4,   4,   6,   6,   8,   8,  10,  10,
	 16,  16,  16,  16,  18,  18,  18,  18,  20,  20,  22,  24,  26,  28,  30,  30,
};

const Uint8 wari0d_sel_12[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  1,   1,   1,   1,   2,   2,   2,   2,   3,   3,   3,   3,   3,   3,   3,   3,
};

const Uint8 *const wari0d_cmd_12[] = {
	wari0d_cmd_12_00, wari0d_cmd_12_01, wari0d_cmd_12_02, wari0d_cmd_12_03,
};

const Uint8 wari0d_rate_13[] = {
	  8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,
	 24,  25,  26,  27,  28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari0d_cmd_02_00[] = {
	LOCK_SET,
	STAND,
	CMD_JUMP( 41 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 wari0d_cmd_02_01[] = {
	LOCK_SET,
	IF_ATCK_COME,
		GUARD( 30 ),
	CMD_ELSE,
		IF_LEVEL( 24 ),
			IF_RND( 30 ),
				WAIT2( 2 ),
				JUMP_RND( 8, 9, 6, 8 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 16 ),
				IF_RND( 20 ),
					WAIT2( 4 ),
					JUMP_RND( 7, 7, 8, 10 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 8 ),
					IF_RND( 16 ),
						WAIT2( 8 ),
						JUMP_RND( 7, 7, 10, 10 ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_LEVEL( 4 ),
						IF_RND( 4 ),
							WAIT2( 14 ),
							CMD_JUMP( 10 ),
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 wari0d_cmd_02_02[] = {
	LOCK_SET,
	GUARD( 30 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 wari0d_cmd_02_03[] = {
	LOCK_SET,
	WAIT( 30 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 wari0d_cmd_03_00[] = {
	CMD_JUMP( 40 ),
	CMD_EXIT,
};

const Uint8 wari0d_cmd_05_00[] = {
	IF_DIST( 96, 0, 96, 0 ),
		IF_DIST( 48, 0, 48, 0 ),
			IF_LEVEL( 24 ),
				IF_AREA2( 16, 0, 64, 0 ),
					CMD_JUMP( 4 ),
				CMD_ELSE,
					CMD_JUMP( 9 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 16 ),
					CMD_JUMP( 8 ),
				CMD_ELSE,
					IF_LEVEL( 8 ),
						CMD_JUMP( 7 ),
					CMD_ELSE,
						CMD_JUMP( 10 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 24 ),
				DASH_F( 128 ),
				CMD_JUMP( 9 ),
			CMD_ELSE,
				IF_LEVEL( 16 ),
					DASH_F( 128 ),
					CMD_JUMP( 8 ),
				CMD_ELSE,
					IF_LEVEL( 8 ),
						WALK_F( 128 ),
						CMD_JUMP( 7 ),
					CMD_ELSE,
						WALK_F( 128 ),
						CMD_JUMP( 10 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 24 ),
			CMD_JUMP( 19 ),
			CMD_JUMP( 9 ),
		CMD_ELSE,
			IF_LEVEL( 16 ),
				CMD_JUMP( 16 ),
				CMD_JUMP( 8 ),
			CMD_ELSE,
				IF_LEVEL( 8 ),
					CMD_JUMP( 0 ),
				CMD_ELSE,
					CMD_JUMP( 0 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari0d_cmd_05_01[] = {
	IF_LEVEL( 24 ),
		IF_RND( 16 ),
			SJUMP( DIR_BACK ),
		CMD_ELSE,
			SJUMP( DIR_FORWARD ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 16 ),
			WAIT2( 6 ),
			DASH_B( 128 ),
		CMD_ELSE,
			IF_LEVEL( 8 ),
				WAIT2( 14 ),
				IF_RND( 16 ),
					JUMP( DIR_BACK ),
				CMD_ELSE,
					JUMP( DIR_FORWARD ),
				CMD_ENDIF,
			CMD_ELSE,
				WALK_B( 96 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari0d_cmd_05_02[] = {
	CMD_JUMP( 40 ),
	CMD_EXIT,
};

const Uint8 wari0d_cmd_05_03[] = {
	LOCK_SET,
	WAIT( 30 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 wari0d_cmd_06_00[] = {
	CMD_JUMP( 40 ),
	CMD_EXIT,
};

const Uint8 wari0d_cmd_07_00[] = {
	IF_DIST( 96, 0, 96, 0 ),
		IF_DIST( 48, 0, 48, 0 ),
			IF_LEVEL( 24 ),
				CMD_JUMP( 9 ),
			CMD_ELSE,
				IF_LEVEL( 16 ),
					CMD_JUMP( 8 ),
				CMD_ELSE,
					IF_LEVEL( 8 ),
						CMD_JUMP( 7 ),
					CMD_ELSE,
						CMD_JUMP( 10 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 24 ),
				DASH_F( 128 ),
				CMD_JUMP( 9 ),
			CMD_ELSE,
				IF_LEVEL( 16 ),
					DASH_F( 128 ),
					CMD_JUMP( 8 ),
				CMD_ELSE,
					IF_LEVEL( 8 ),
						WALK_F( 128 ),
						CMD_JUMP( 7 ),
					CMD_ELSE,
						WALK_F( 128 ),
						CMD_JUMP( 10 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 24 ),
			CMD_JUMP( 19 ),
			CMD_JUMP( 9 ),
		CMD_ELSE,
			IF_LEVEL( 16 ),
				CMD_JUMP( 16 ),
				CMD_JUMP( 8 ),
			CMD_ELSE,
				IF_LEVEL( 8 ),
					CMD_JUMP( 0 ),
				CMD_ELSE,
					CMD_JUMP( 0 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari0d_cmd_07_01[] = {
	IF_LEVEL( 24 ),
		IF_RND( 16 ),
			SJUMP( DIR_BACK ),
		CMD_ELSE,
			SJUMP( DIR_FORWARD ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 16 ),
			WAIT2( 6 ),
			DASH_B( 128 ),
		CMD_ELSE,
			IF_LEVEL( 8 ),
				WAIT2( 14 ),
				IF_RND( 16 ),
					JUMP( DIR_BACK ),
				CMD_ELSE,
					JUMP( DIR_FORWARD ),
				CMD_ENDIF,
			CMD_ELSE,
				WALK_B( 96 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari0d_cmd_07_02[] = {
	CMD_JUMP( 40 ),
	CMD_EXIT,
};

const Uint8 wari0d_cmd_07_03[] = {
	LOCK_SET,
	WAIT( 30 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 wari0d_cmd_08_00[] = {
	CMD_JUMP( 40 ),
	CMD_EXIT,
};

const Uint8 wari0d_cmd_09_00[] = {
	CMD_JUMP( 41 ),
	CMD_EXIT,
};

const Uint8 wari0d_cmd_09_01[] = {
	CMD_JUMP( 40 ),
	CMD_EXIT,
};

const Uint8 wari0d_cmd_09_02[] = {
	LOCK_SET,
	WAIT( 30 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 wari0d_cmd_10_00[] = {
	CMD_JUMP( 40 ),
	CMD_EXIT,
};

const Uint8 wari0d_cmd_10_01[] = {
	LOCK_SET,
	TIMER2_SET( 60 ),
	IF_RND( 20 ),
		TIMER_SET( 60 ),
		IF_EM_ATCK( ATCK_CROUCH_HK ),
			CMD_JUMP( 24 ),
			CROUCH,
			KICK( LV_HEAVY ),
		CMD_ELSE,
			GUARD( 60 ),
		CMD_ENDIF,
	CMD_ELSE,
		WAIT( 30 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 wari0d_cmd_10_02[] = {
	IF_DIST( 96, 0, 96, 0 ),
		IF_DIST( 48, 0, 48, 0 ),
			IF_LEVEL( 24 ),
				JUMP_RND( 3, 4, 5, 6 ),
			CMD_ELSE,
				IF_LEVEL( 16 ),
					CMD_JUMP( 7 ),
				CMD_ELSE,
					IF_LEVEL( 8 ),
						WAIT2( 4 ),
						PUNCH( LV_HEAVY ),
					CMD_ELSE,
						WAIT2( 10 ),
						CMD_JUMP( 10 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 24 ),
				DASH_F( 96 ),
				JUMP_RND( 3, 4, 5, 6 ),
			CMD_ELSE,
				IF_LEVEL( 16 ),
					JUMP_RND( 0, 0, 1, 1 ),
				CMD_ELSE,
					IF_LEVEL( 8 ),
						WAIT2( 4 ),
						DASH_F( 96 ),
						WAIT2( 4 ),
						PUNCH( LV_HEAVY ),
					CMD_ELSE,
						WALK_F( 96 ),
						CMD_JUMP( 10 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 24 ),
			DASH_F( 96 ),
			JUMP_RND( 19, 19, 19, 57 ),
			CMD_JUMP( 22 ),
		CMD_ELSE,
			IF_LEVEL( 16 ),
				DASH_F( 96 ),
				JUMP_RND( 19, 57, 57, 16 ),
				CMD_JUMP( 22 ),
			CMD_ELSE,
				IF_LEVEL( 8 ),
					DASH_F( 96 ),
					JUMP_RND( 57, 16, 16, 16 ),
					WAIT2( 10 ),
					CMD_JUMP( 22 ),
				CMD_ELSE,
					DASH_F( 96 ),
					JUMP( DIR_FORWARD ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari0d_cmd_11_00[] = {
	LOCK_SET,
	IF_LEVEL( 24 ),
		SJUMP( DIR_FORWARD ),
		SJUMP_FLAG_SET,
			WAIT( 30 ),
			PUNCH( LV_HEAVY ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 16 ),
			SJUMP( DIR_FORWARD ),
			SJUMP_FLAG_SET,
				WAIT( 30 ),
				PUNCH( LV_LIGHT ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 8 ),
				SJUMP( DIR_FORWARD ),
				SJUMP_FLAG_SET,
					WAIT( 30 ),
					PUNCH( LV_LIGHT ),
					JUMP_FLAG_CLR,
				CMD_ENDIF,
			CMD_ELSE,
				SJUMP( DIR_FORWARD ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 wari0d_cmd_11_01[] = {
	LOCK_SET,
	IF_LEVEL( 24 ),
		STAND,
		TIMER_SET( 0xb4 ),
		IF_DIST( 48, 240, 96, 480 ),
			JUMP_RND( 4, 6, 6, 6 ),
		CMD_ELSE,
			CMD_JUMP( 4 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 16 ),
			STAND,
			TIMER_SET( 0xb4 ),
			IF_DIST( 48, 240, 96, 480 ),
				JUMP_RND( 2, 2, 4, 6 ),
			CMD_ELSE,
				CMD_JUMP( 2 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 8 ),
				CMD_JUMP( 40 ),
			CMD_ELSE,
				CMD_JUMP( 40 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 wari0d_cmd_11_02[] = {
	LOCK_SET,
	WAIT( 30 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 wari0d_cmd_12_00[] = {
	WAIT2( 4 ),
	IF_DIST( 96, 0, 96, 0 ),
		IF_DIST( 48, 0, 48, 0 ),
			IF_LEVEL( 24 ),
				JUMP( DIR_UP ),
				CMD_JUMP( 47 ),
			CMD_ELSE,
				IF_LEVEL( 16 ),
					JUMP( DIR_UP ),
					CMD_JUMP( 47 ),
				CMD_ELSE,
					IF_LEVEL( 8 ),
						WAIT2( 4 ),
						JUMP( DIR_UP ),
						CMD_JUMP( 48 ),
					CMD_ELSE,
						WAIT2( 8 ),
						JUMP( DIR_UP ),
						CMD_JUMP( 48 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 24 ),
				JUMP( DIR_FORWARD ),
				CMD_JUMP( 47 ),
			CMD_ELSE,
				IF_LEVEL( 16 ),
					JUMP( DIR_FORWARD ),
					CMD_JUMP( 47 ),
				CMD_ELSE,
					IF_LEVEL( 8 ),
						WAIT2( 4 ),
						JUMP( DIR_FORWARD ),
						CMD_JUMP( 48 ),
					CMD_ELSE,
						WAIT2( 8 ),
						JUMP( DIR_FORWARD ),
						CMD_JUMP( 48 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 24 ),
			DASH_F( 48 ),
			JUMP( DIR_FORWARD ),
			CMD_JUMP( 47 ),
		CMD_ELSE,
			IF_LEVEL( 16 ),
				WAIT2( 4 ),
				DASH_F( 48 ),
				JUMP( DIR_FORWARD ),
				CMD_JUMP( 47 ),
			CMD_ELSE,
				IF_LEVEL( 8 ),
					WAIT2( 6 ),
					DASH_F( 48 ),
					JUMP( DIR_FORWARD ),
					CMD_JUMP( 48 ),
				CMD_ELSE,
					WAIT2( 14 ),
					DASH_F( 48 ),
					JUMP( DIR_FORWARD ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari0d_cmd_12_01[] = {
	WAIT2( 4 ),
	IF_DIST( 96, 0, 96, 0 ),
		IF_DIST( 48, 0, 48, 0 ),
			IF_LEVEL( 24 ),
				JUMP_RND( 4, 4, 6, 6 ),
			CMD_ELSE,
				IF_LEVEL( 16 ),
					STAND,
					PUNCH( LV_HEAVY ),
				CMD_ELSE,
					IF_LEVEL( 8 ),
						WAIT2( 10 ),
						STAND,
						PUNCH( LV_HEAVY ),
					CMD_ELSE,
						WAIT2( 14 ),
						STAND,
						PUNCH( LV_HEAVY ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 24 ),
				JUMP_RND( 4, 4, 6, 6 ),
			CMD_ELSE,
				IF_LEVEL( 16 ),
					CMD_JUMP( 0 ),
				CMD_ELSE,
					IF_LEVEL( 8 ),
						STAND,
						WAIT2( 10 ),
						PUNCH( LV_HEAVY ),
					CMD_ELSE,
						STAND,
						WAIT2( 14 ),
						PUNCH( LV_HEAVY ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 24 ),
			CMD_JUMP( 5 ),
		CMD_ELSE,
			IF_LEVEL( 16 ),
				CMD_JUMP( 1 ),
			CMD_ELSE,
				IF_LEVEL( 8 ),
					WAIT2( 10 ),
					CMD_JUMP( 0 ),
				CMD_ELSE,
					WAIT2( 14 ),
					CMD_JUMP( 0 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari0d_cmd_12_02[] = {
	CMD_JUMP( 40 ),
	CMD_EXIT,
};

const Uint8 wari0d_cmd_12_03[] = {
	LOCK_SET,
	WAIT( 60 ),
	LOCK_CLR,
	CMD_EXIT,
};

/* Abyss (third form) (character 1a): the computer's lists for cutting in.
   What it does when something happens in the fight. PLAYER_WORK's cpu_adrs[2] points here; the
   event checks of em_play.c start the modes.

   wari1a_tbl        a record (CPU_DATA) for each mode from 2 on: the offsets of its tables.
   wari1a_rate_MM    the chance, out of 32, that mode MM is started, by the level of play.
   wari1a_sel_MM     32 picks; one is taken at random.
   wari1a_cmd_MM     the mode's lists: a pick is a number in this table.
   wari1a_cmd_MM_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name
                    a list of lib1a.c.
   wari1a_unused_N   bytes that nothing in the file reaches. */
#include "player.h"
#include "pl1a_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const wari1a_cmd_02[], *const wari1a_cmd_03[], *const wari1a_cmd_05[],
	*const wari1a_cmd_06[], *const wari1a_cmd_07[], *const wari1a_cmd_08[], *const wari1a_cmd_09[],
	*const wari1a_cmd_10[], *const wari1a_cmd_11[], *const wari1a_cmd_12[];
extern const Uint8 wari1a_rate_02[], wari1a_sel_02[], wari1a_rate_03[], wari1a_rate_04[],
	wari1a_sel_03[], wari1a_rate_05[], wari1a_sel_05[], wari1a_rate_06[], wari1a_sel_06[],
	wari1a_rate_07[], wari1a_sel_07[], wari1a_rate_08[], wari1a_sel_08[], wari1a_rate_09[],
	wari1a_sel_09[], wari1a_unused_0[], wari1a_rate_10[], wari1a_sel_10[], wari1a_rate_11[],
	wari1a_sel_11[], wari1a_rate_12[], wari1a_sel_12[], wari1a_rate_13[], wari1a_cmd_02_00[],
	wari1a_cmd_02_01[], wari1a_cmd_02_02[], wari1a_cmd_03_00[], wari1a_cmd_05_00[],
	wari1a_cmd_05_01[], wari1a_cmd_05_02[], wari1a_cmd_05_03[], wari1a_cmd_06_00[],
	wari1a_cmd_07_00[], wari1a_cmd_07_01[], wari1a_cmd_07_02[], wari1a_cmd_08_00[],
	wari1a_cmd_09_00[], wari1a_cmd_09_01[], wari1a_cmd_09_02[], wari1a_cmd_10_00[],
	wari1a_cmd_10_01[], wari1a_cmd_10_02[], wari1a_cmd_11_00[], wari1a_cmd_11_01[],
	wari1a_cmd_11_02[], wari1a_cmd_12_00[], wari1a_cmd_12_01[], wari1a_cmd_12_02[],
	wari1a_cmd_12_03[];

const CPU_DATA wari1a_tbl[] = {
	{ ( Sint32 )wari1a_rate_02, ( Sint32 )wari1a_sel_02, ( Sint32 )wari1a_cmd_02 },	/* 2: a throw would catch the opponent */
	{ ( Sint32 )wari1a_rate_03, ( Sint32 )wari1a_sel_03, ( Sint32 )wari1a_cmd_03 },	/* 3: it guards a hit (the chance is the program's own) */
	{ ( Sint32 )wari1a_rate_04, ( Sint32 )wari1a_sel_03, ( Sint32 )wari1a_cmd_03 },	/* 4: never started: a guard in the air is mode 3 */
	{ ( Sint32 )wari1a_rate_05, ( Sint32 )wari1a_sel_05, ( Sint32 )wari1a_cmd_05 },	/* 5: after it has guarded on the ground */
	{ ( Sint32 )wari1a_rate_06, ( Sint32 )wari1a_sel_06, ( Sint32 )wari1a_cmd_06 },	/* 6: after it has guarded in the air */
	{ ( Sint32 )wari1a_rate_07, ( Sint32 )wari1a_sel_07, ( Sint32 )wari1a_cmd_07 },	/* 7: after it has been hit */
	{ ( Sint32 )wari1a_rate_08, ( Sint32 )wari1a_sel_08, ( Sint32 )wari1a_cmd_08 },	/* 8: never started */
	{ ( Sint32 )wari1a_rate_09, ( Sint32 )wari1a_sel_09, ( Sint32 )wari1a_cmd_09 },	/* 9: after it has got up */
	{ ( Sint32 )wari1a_rate_10, ( Sint32 )wari1a_sel_10, ( Sint32 )wari1a_cmd_10 },	/* 10: the opponent dashes */
	{ ( Sint32 )wari1a_rate_11, ( Sint32 )wari1a_sel_11, ( Sint32 )wari1a_cmd_11 },	/* 11: the opponent flies or super jumps */
	{ ( Sint32 )wari1a_rate_12, ( Sint32 )wari1a_sel_12, ( Sint32 )wari1a_cmd_12 },	/* 12: the opponent jumps */
	{ ( Sint32 )wari1a_rate_13, 0, 0 },	/* 13: an attack is coming: the chance alone, the lists are yok1a.c's */
};

const Uint8 wari1a_rate_02[] = {
	  9,   9,  10,  10,  11,  11,  12,  12,  13,  13,  14,  14,  15,  15,  16,  16,
	 17,  18,  19,  20,  21,  22,  23,  24,  24,  24,  24,  24,  24,  24,  24,  24,
};

const Uint8 wari1a_sel_02[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  1,   1,   2,   2,   2,   2,   2,   2,   1,   1,   1,   1,   1,   1,   1,   1,
};

const Uint8 *const wari1a_cmd_02[] = {
	wari1a_cmd_02_00, wari1a_cmd_02_01, wari1a_cmd_02_02,
};

const Uint8 wari1a_rate_03[] = {
	  1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   3,   3,   3,
};

const Uint8 wari1a_rate_04[] = {
	 16,  16,  17,  17,  18,  19,  20,  21,  22,  23,  24,  25,  25,  25,  25,  25,
	 25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,
};

const Uint8 wari1a_sel_03[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const wari1a_cmd_03[] = {
	wari1a_cmd_03_00,
};

const Uint8 wari1a_rate_05[] = {
	 19,  19,  20,  20,  21,  21,  22,  22,  23,  23,  24,  24,  25,  25,  26,  26,
	 27,  27,  28,  28,  29,  29,  30,  30,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari1a_sel_05[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   2,   2,   2,   2,   2,   2,   2,
	  0,   0,   1,   1,   1,   1,   1,   1,   0,   0,   1,   1,   1,   1,   1,   1,
};

const Uint8 *const wari1a_cmd_05[] = {
	wari1a_cmd_05_00, wari1a_cmd_05_01, wari1a_cmd_05_02, wari1a_cmd_05_03,
};

const Uint8 wari1a_rate_06[] = {
	  4,   4,   4,   8,  12,  16,  18,  20,  22,  24,  26,  28,  30,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari1a_sel_06[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const wari1a_cmd_06[] = {
	wari1a_cmd_06_00,
};

const Uint8 wari1a_rate_07[] = {
	  4,   4,   5,   5,   6,   6,   7,   7,   8,   8,   9,   9,  10,  10,  11,  11,
	 12,  12,  12,  12,  13,  13,  13,  13,  14,  14,  14,  14,  15,  15,  15,  15,
};

const Uint8 wari1a_sel_07[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
};

const Uint8 *const wari1a_cmd_07[] = {
	wari1a_cmd_07_00, wari1a_cmd_07_01, wari1a_cmd_07_02,
};

const Uint8 wari1a_rate_08[] = {
	  4,   4,   4,   8,  12,  16,  18,  20,  22,  24,  26,  28,  30,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari1a_sel_08[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const wari1a_cmd_08[] = {
	wari1a_cmd_08_00,
};

const Uint8 wari1a_rate_09[] = {
	 24,  25,  26,  27,  28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari1a_sel_09[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   1,   1,   1,   1,
};

const Uint8 *const wari1a_cmd_09[] = {
	wari1a_cmd_09_00, wari1a_cmd_09_01, wari1a_cmd_09_02,
};

const Uint8 wari1a_unused_0[] = {
	 20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,
	 20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,
	  0,   0,   0,   0,   0,   0,   0,   0,   2,   2,   2,   2,   0,   0,   0,   0,
	  2,   2,   2,   2,   2,   2,   2,   2,   1,   1,   1,   1,   1,   1,   1,   1,
	255,   6,   0,   0,  11,   7,   0,   0,  14,   7,   0,   0,  31,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  35,   7,   0,   0,
};

const Uint8 wari1a_rate_10[] = {
	  1,   2,   3,   4,   5,   6,   7,   7,   7,   8,   8,   8,   9,   9,   9,  10,
	 10,  10,  11,  12,  13,  14,  15,  16,  16,  16,  16,  16,  16,  16,  16,  16,
};

const Uint8 wari1a_sel_10[] = {
	  0,   0,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,
};

const Uint8 *const wari1a_cmd_10[] = {
	wari1a_cmd_10_00, wari1a_cmd_10_01, wari1a_cmd_10_02,
};

const Uint8 wari1a_rate_11[] = {
	  0,   0,   1,   1,   2,   2,   3,   3,   4,   4,   5,   5,   6,   6,   7,   7,
	  8,   8,   9,   9,  10,  10,  11,  11,  12,  11,  12,  12,  13,  13,  14,  14,
};

const Uint8 wari1a_sel_11[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,
};

const Uint8 *const wari1a_cmd_11[] = {
	wari1a_cmd_11_00, wari1a_cmd_11_01, wari1a_cmd_11_02,
};

const Uint8 wari1a_rate_12[] = {
	  0,   0,   0,   0,   1,   1,   2,   2,   3,   3,   4,   4,   5,   5,   6,   6,
	  6,   6,   6,   7,   7,   7,   8,   8,   8,   9,   9,  10,  10,  11,  11,  12,
};

const Uint8 wari1a_sel_12[] = {
	  0,   0,   0,   0,   0,   3,   3,   3,   0,   0,   0,   0,   0,   3,   3,   3,
	  1,   1,   1,   1,   1,   3,   3,   3,   2,   2,   2,   2,   2,   3,   3,   3,
};

const Uint8 *const wari1a_cmd_12[] = {
	wari1a_cmd_12_00, wari1a_cmd_12_01, wari1a_cmd_12_02, wari1a_cmd_12_03,
};

const Uint8 wari1a_rate_13[] = {
	  8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,
	 24,  25,  26,  27,  28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	  1,   2,   3,   3,   4,   4,   5,   5,   6,   6,   6,   6,   6,   6,   6,   6,
	  7,   7,   7,   7,   7,   7,   7,   7,   8,   8,   8,   8,   8,   8,   8,   8,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,
	  3,   3,   3,   3,   3,   3,   3,   3,   4,   4,   4,   4,   5,   5,   5,   5,
	  4,   4,   4,   4,   6,   6,   6,   6,   8,   8,   8,   8,  10,  10,  10,  10,
	 12,  12,  12,  12,  14,  14,  14,  14,  16,  16,  16,  16,  16,  16,  16,  16,
};

const Uint8 wari1a_cmd_02_00[] = {
	STAND,
	IF_CATCH,
		IF_LEVEL( 24 ),
			IF_RND( 16 ),
				WAIT2( 2 ),
				NAG_VFLIP_ADJ( 0, LEVER_DOWN ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 21 ),
				IF_RND( 14 ),
					WAIT2( 4 ),
					NAG_VFLIP_ADJ( 0, LEVER_DOWN ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 13 ),
					IF_RND( 12 ),
						WAIT2( 6 ),
						NAG_VFLIP_ADJ( 0, LEVER_DOWN ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_LEVEL( 7 ),
						IF_RND( 10 ),
							WAIT2( 8 ),
							NAG_VFLIP_ADJ( 0, LEVER_DOWN ),
						CMD_ENDIF,
					CMD_ELSE,
						IF_RND( 4 ),
							WAIT2( 10 ),
							NAG_VFLIP_ADJ( 0, LEVER_DOWN ),
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari1a_cmd_02_01[] = {
	STAND,
	IF_LEVEL( 24 ),
		IF_RND( 28 ),
			CMD_JUMP( 30 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 21 ),
			IF_RND( 24 ),
				WAIT2( 2 ),
				CMD_JUMP( 30 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 13 ),
				IF_RND( 20 ),
					WAIT2( 2 ),
					CMD_JUMP( 30 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 7 ),
					IF_RND( 16 ),
						WAIT2( 4 ),
						CMD_JUMP( 30 ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_RND( 12 ),
						WAIT2( 6 ),
						CMD_JUMP( 30 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 wari1a_cmd_02_02[] = {
	STAND,
	IF_LEVEL( 24 ),
		IF_RND( 28 ),
			JUMP_LEVEL( 7, 8, 8, 16, 8, 24, 26 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 21 ),
			IF_RND( 20 ),
				WAIT2( 2 ),
				JUMP_LEVEL( 7, 8, 8, 16, 24, 24, 26 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 13 ),
				IF_RND( 14 ),
					WAIT2( 2 ),
					JUMP_LEVEL( 7, 8, 8, 16, 8, 24, 26 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 7 ),
					IF_RND( 8 ),
						WAIT2( 4 ),
						JUMP_LEVEL( 7, 8, 8, 16, 8, 24, 26 ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_RND( 1 ),
						WAIT2( 6 ),
						JUMP_LEVEL( 7, 8, 8, 16, 8, 24, 26 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 wari1a_cmd_03_00[] = {
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	TIMER2_SET( 60 ),
	CMD_EXIT,
};

const Uint8 wari1a_cmd_05_00[] = {
	STAND,
	IF_LEVEL( 24 ),
		IF_RND( 24 ),
			CMD_JUMP( 30 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 21 ),
			IF_RND( 22 ),
				WAIT2( 2 ),
				CMD_JUMP( 30 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 13 ),
				IF_RND( 20 ),
					WAIT2( 2 ),
					CMD_JUMP( 30 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 57 ),
					IF_RND( 18 ),
						WAIT2( 4 ),
						CMD_JUMP( 30 ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_RND( 16 ),
						WAIT2( 6 ),
						CMD_JUMP( 30 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 wari1a_cmd_05_01[] = {
	LOCK_SET,
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	IF_LEVEL( 3 ),
		IF_LEVEL( 8 ),
		CMD_ENDIF,
		GUARD( 2 ),
	CMD_ENDIF,
	IF_CATCH,
		NAG_VFLIP_ADJ( 0, LEVER_FORWARD ),
	CMD_ELSE,
		JUMP_LEVEL( 48, 7, 49, 13, 50, 21, 51 ),
		JUMP( DIR_UP ),
		JUMP_FLAG_SET,
			WAIT( 20 ),
			IF_LEVEL( 8 ),
				IF_RND( 24 ),
					JUMP_LEVEL( 38, 8, 39, 16, 40, 29, 41 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_RND( 10 ),
					JUMP_LEVEL( 38, 8, 39, 16, 40, 29, 41 ),
				CMD_ENDIF,
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	STAND,
	CMD_EXIT,
};

const Uint8 wari1a_cmd_05_02[] = {
	STAND,
	IF_LEVEL( 24 ),
		IF_RND( 28 ),
			JUMP_LEVEL( 7, 8, 8, 16, 8, 24, 26 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 21 ),
			IF_RND( 20 ),
				WAIT2( 2 ),
				JUMP_LEVEL( 7, 8, 8, 16, 24, 24, 26 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 13 ),
				IF_RND( 14 ),
					WAIT2( 2 ),
					JUMP_LEVEL( 7, 8, 8, 16, 8, 24, 26 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 7 ),
					IF_RND( 8 ),
						WAIT2( 4 ),
						JUMP_LEVEL( 7, 8, 8, 16, 8, 24, 26 ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_RND( 1 ),
						WAIT2( 6 ),
						JUMP_LEVEL( 7, 8, 8, 16, 8, 24, 26 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 wari1a_cmd_05_03[] = {
	JUMP_RET,
};

const Uint8 wari1a_cmd_06_00[] = {
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 wari1a_cmd_07_00[] = {
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT2( 2 ),
	TIMER2_SET( 60 ),
	CMD_EXIT,
};

const Uint8 wari1a_cmd_07_01[] = {
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	TIMER2_SET( 60 ),
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 wari1a_cmd_07_02[] = {
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT2( 2 ),
	GUARD( 30 ),
	CMD_EXIT,
};

const Uint8 wari1a_cmd_08_00[] = {
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	GUARD( 30 ),
	CMD_EXIT,
};

const Uint8 wari1a_cmd_09_00[] = {
	LOCK_SET,
	STAND,
	IF_LEVEL( 5 ),
		IF_LEVEL( 22 ),
			IF_RND( 16 ),
				IF_CATCH,
					NAG_VFLIP_ADJ( 0, LEVER_DOWN ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 8 ),
				IF_RND( 10 ),
					IF_CATCH,
						WAIT2( 2 ),
						NAG_VFLIP_ADJ( 0, LEVER_DOWN ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ELSE,
				IF_RND( 2 ),
					IF_CATCH,
						WAIT2( 4 ),
						NAG_VFLIP_ADJ( 0, LEVER_DOWN ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 wari1a_cmd_09_01[] = {
	IF_LEVEL( 16 ),
		GUARD( 30 ),
	CMD_ELSE,
		IF_RND( 16 ),
			GUARD2( 30 ),
		CMD_ELSE,
			GUARD( 30 ),
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	CMD_EXIT,
};

const Uint8 wari1a_cmd_09_02[] = {
	LOCK_SET,
	CROUCH,
	IF_ATCK_COME,
		GUARD( 30 ),
	CMD_ELSE,
		IF_EM_ATCK( ATCK_CROUCH_LK ),
			IF_LEVEL( 24 ),
				CANCEL_SET2,
					KICK( LV_LIGHT ),
					CMD_JUMP( 7 ),
					CANCEL_CLR,
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 16 ),
					IF_EM_ATCK( ATCK_STAND_LK ),
						CMD_JUMP( 4 ),
					CMD_ELSE,
						CROUCH,
						CANCEL_SET,
							KICK( LV_LIGHT ),
							KICK( LV_LIGHT ),
							CANCEL_CLR,
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ELSE,
					CROUCH,
					CANCEL_SET,
						KICK( LV_LIGHT ),
						CROUCH,
						KICK( LV_LIGHT ),
						CANCEL_CLR,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			GUARD( 30 ),
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	LOCK_CLR,
	WAIT2( 8 ),
	CMD_EXIT,
	STAND,
	WAIT2( 12 ),
	JUMP_LEVEL( 8, 8, 8, 15, 8, 24, 26 ),
	CMD_EXIT,
	WAIT2( 2 ),
	CMD_EXIT,
	STAND,
	LOCK_SET,
	WALK_B( 32 ),
	WAIT( 20 ),
	STAND,
	WAIT2( 8 ),
	SP_CMD( pl1a_sp_throw, 0, PK_PUNCH, LV_LIGHT, 0 ),
	STAND,
	LOCK_CLR,
	WAIT2( 14 ),
	CMD_EXIT,
	WAIT2( 6 ),
	CMD_EXIT,
};

const Uint8 wari1a_cmd_10_00[] = {
	CMD_JUMP( 59 ),
	CMD_EXIT,
};

const Uint8 wari1a_cmd_10_01[] = {
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 wari1a_cmd_10_02[] = {
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 wari1a_cmd_11_00[] = {
	STAND,
	WAIT2( 8 ),
	IF_LEVEL( 24 ),
		SJUMP( DIR_FORWARD ),
		SJUMP_FLAG_SET,
			JUMP_LEVEL( 42, 12, 43, 19, 44, 29, 45 ),
			TIMER_SET( 120 ),
			IF_CATCH,
				IF_LEVEL( 24 ),
				CMD_ELSE,
					WAIT2( 4 ),
				CMD_ENDIF,
				NAG_VFLIP_ADJ( 1, LEVER_FORWARD ),
				IF_LEVEL( 26 ),
					IF_GAUGE( 2 ),
						SP_CMD( pl1a_sp_blast, 0, PK_PUNCH, LV_LIGHT, 0 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
		STAND,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari1a_cmd_11_01[] = {
	STAND,
	IF_LEVEL( 31 ),
		IF_RND( 16 ),
			TIMER2_SET( 120 ),
		CMD_ELSE,
			TIMER2_SET( 60 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_RND( 8 ),
			TIMER2_SET( 30 ),
		CMD_ENDIF,
	CMD_ENDIF,
	SJUMP( DIR_FORWARD ),
	SJUMP_FLAG_SET,
		JUMP_LEVEL( 31, 8, 32, 29, 33, 30, 34 ),
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
};

const Uint8 wari1a_cmd_11_02[] = {
	WAIT2( 14 ),
	CMD_EXIT,
};

const Uint8 wari1a_cmd_12_00[] = {
	STAND,
	IF_LEVEL( 19 ),
		JUMP( DIR_FORWARD ),
		JUMP_FLAG_SET,
			JUMP_LEVEL( 48, 7, 49, 13, 50, 21, 51 ),
			JUMP_LEVEL( 31, 8, 32, 29, 33, 31, 34 ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
		STAND,
	CMD_ENDIF,
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 wari1a_cmd_12_01[] = {
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 wari1a_cmd_12_02[] = {
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 wari1a_cmd_12_03[] = {
	WAIT2( 2 ),
	CMD_EXIT,
};

/* Abyss (first form) (character 18): the computer's lists for cutting in.
   What it does when something happens in the fight. PLAYER_WORK's cpu_adrs[2] points here; the
   event checks of em_play.c start the modes.

   wari18_tbl        a record (CPU_DATA) for each mode from 2 on: the offsets of its tables.
   wari18_rate_MM    the chance, out of 32, that mode MM is started, by the level of play.
   wari18_sel_MM     32 picks; one is taken at random.
   wari18_cmd_MM     the mode's lists: a pick is a number in this table.
   wari18_cmd_MM_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name
                    a list of lib18.c.
   wari18_unused_N   bytes that nothing in the file reaches. */
#include "player.h"
#include "cpu_cmd.h"

extern const Uint8 *const wari18_cmd_02[], *const wari18_cmd_03[], *const wari18_cmd_05[],
	*const wari18_cmd_06[], *const wari18_cmd_07[], *const wari18_cmd_08[], *const wari18_cmd_09[],
	*const wari18_cmd_10[], *const wari18_cmd_11[], *const wari18_cmd_12[];
extern const Uint8 wari18_rate_02[], wari18_sel_02[], wari18_rate_03[], wari18_rate_04[],
	wari18_sel_03[], wari18_rate_05[], wari18_sel_05[], wari18_rate_06[], wari18_sel_06[],
	wari18_rate_07[], wari18_sel_07[], wari18_rate_08[], wari18_sel_08[], wari18_rate_09[],
	wari18_sel_09[], wari18_unused_0[], wari18_rate_10[], wari18_sel_10[], wari18_rate_11[],
	wari18_sel_11[], wari18_rate_12[], wari18_sel_12[], wari18_rate_13[], wari18_cmd_02_00[],
	wari18_cmd_02_01[], wari18_cmd_02_02[], wari18_cmd_03_00[], wari18_cmd_05_00[],
	wari18_cmd_06_00[], wari18_cmd_07_00[], wari18_cmd_07_01[], wari18_cmd_07_02[],
	wari18_cmd_08_00[], wari18_cmd_09_00[], wari18_cmd_09_01[], wari18_cmd_09_02[],
	wari18_cmd_10_00[], wari18_cmd_10_01[], wari18_cmd_10_02[], wari18_cmd_11_00[],
	wari18_cmd_11_01[], wari18_cmd_11_02[], wari18_cmd_12_00[], wari18_cmd_12_01[],
	wari18_cmd_12_02[], wari18_cmd_12_03[];

const CPU_DATA wari18_tbl[] = {
	{ ( Sint32 )wari18_rate_02, ( Sint32 )wari18_sel_02, ( Sint32 )wari18_cmd_02 },	/* 2: a throw would catch the opponent */
	{ ( Sint32 )wari18_rate_03, ( Sint32 )wari18_sel_03, ( Sint32 )wari18_cmd_03 },	/* 3: it guards a hit (the chance is the program's own) */
	{ ( Sint32 )wari18_rate_04, ( Sint32 )wari18_sel_03, ( Sint32 )wari18_cmd_03 },	/* 4: never started: a guard in the air is mode 3 */
	{ ( Sint32 )wari18_rate_05, ( Sint32 )wari18_sel_05, ( Sint32 )wari18_cmd_05 },	/* 5: after it has guarded on the ground */
	{ ( Sint32 )wari18_rate_06, ( Sint32 )wari18_sel_06, ( Sint32 )wari18_cmd_06 },	/* 6: after it has guarded in the air */
	{ ( Sint32 )wari18_rate_07, ( Sint32 )wari18_sel_07, ( Sint32 )wari18_cmd_07 },	/* 7: after it has been hit */
	{ ( Sint32 )wari18_rate_08, ( Sint32 )wari18_sel_08, ( Sint32 )wari18_cmd_08 },	/* 8: never started */
	{ ( Sint32 )wari18_rate_09, ( Sint32 )wari18_sel_09, ( Sint32 )wari18_cmd_09 },	/* 9: after it has got up */
	{ ( Sint32 )wari18_rate_10, ( Sint32 )wari18_sel_10, ( Sint32 )wari18_cmd_10 },	/* 10: the opponent dashes */
	{ ( Sint32 )wari18_rate_11, ( Sint32 )wari18_sel_11, ( Sint32 )wari18_cmd_11 },	/* 11: the opponent flies or super jumps */
	{ ( Sint32 )wari18_rate_12, ( Sint32 )wari18_sel_12, ( Sint32 )wari18_cmd_12 },	/* 12: the opponent jumps */
	{ ( Sint32 )wari18_rate_13, 0, 0 },	/* 13: an attack is coming: the chance alone, the lists are yok18.c's */
};

const Uint8 wari18_rate_02[] = {
	  9,   9,  10,  10,  11,  11,  12,  12,  13,  13,  14,  14,  15,  15,  16,  16,
	 17,  18,  19,  20,  21,  22,  23,  24,  24,  24,  24,  24,  24,  24,  24,  24,
};

const Uint8 wari18_sel_02[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  1,   1,   2,   2,   2,   2,   2,   2,   1,   1,   1,   1,   1,   1,   1,   1,
};

const Uint8 *const wari18_cmd_02[] = {
	wari18_cmd_02_00, wari18_cmd_02_01, wari18_cmd_02_02,
};

const Uint8 wari18_rate_03[] = {
	  1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   3,   3,   3,
};

const Uint8 wari18_rate_04[] = {
	 16,  16,  17,  17,  18,  19,  20,  21,  22,  23,  24,  25,  25,  25,  25,  25,
	 25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,
};

const Uint8 wari18_sel_03[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const wari18_cmd_03[] = {
	wari18_cmd_03_00,
};

const Uint8 wari18_rate_05[] = {
	 19,  19,  20,  20,  21,  21,  22,  22,  23,  23,  24,  24,  25,  25,  26,  26,
	 27,  27,  28,  28,  29,  29,  30,  30,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari18_sel_05[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const wari18_cmd_05[] = {
	wari18_cmd_05_00,
};

const Uint8 wari18_rate_06[] = {
	  4,   4,   4,   8,  12,  16,  18,  20,  22,  24,  26,  28,  30,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari18_sel_06[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const wari18_cmd_06[] = {
	wari18_cmd_06_00,
};

const Uint8 wari18_rate_07[] = {
	  4,   4,   5,   5,   6,   6,   7,   7,   8,   8,   9,   9,  10,  10,  11,  11,
	 12,  12,  12,  12,  13,  13,  13,  13,  14,  14,  14,  14,  15,  15,  15,  15,
};

const Uint8 wari18_sel_07[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
};

const Uint8 *const wari18_cmd_07[] = {
	wari18_cmd_07_00, wari18_cmd_07_01, wari18_cmd_07_02,
};

const Uint8 wari18_rate_08[] = {
	  4,   4,   4,   8,  12,  16,  18,  20,  22,  24,  26,  28,  30,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari18_sel_08[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const wari18_cmd_08[] = {
	wari18_cmd_08_00,
};

const Uint8 wari18_rate_09[] = {
	 24,  25,  26,  27,  28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari18_sel_09[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   1,   1,   1,   1,
};

const Uint8 *const wari18_cmd_09[] = {
	wari18_cmd_09_00, wari18_cmd_09_01, wari18_cmd_09_02,
};

const Uint8 wari18_unused_0[] = {
	 20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,
	 20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,
	  0,   0,   0,   0,   0,   0,   0,   0,   2,   2,   2,   2,   0,   0,   0,   0,
	  2,   2,   2,   2,   2,   2,   2,   2,   1,   1,   1,   1,   1,   1,   1,   1,
	 47,   5,   0,   0,  59,   5,   0,   0,  62,   5,   0,   0,  31,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  65,   5,   0,   0,
};

const Uint8 wari18_rate_10[] = {
	  1,   2,   3,   4,   5,   6,   7,   7,   7,   8,   8,   8,   9,   9,   9,  10,
	 10,  10,  11,  12,  13,  14,  15,  16,  16,  16,  16,  16,  16,  16,  16,  16,
};

const Uint8 wari18_sel_10[] = {
	  0,   0,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,
};

const Uint8 *const wari18_cmd_10[] = {
	wari18_cmd_10_00, wari18_cmd_10_01, wari18_cmd_10_02,
};

const Uint8 wari18_rate_11[] = {
	  0,   0,   1,   1,   2,   2,   3,   3,   4,   4,   5,   5,   6,   6,   7,   7,
	  8,   8,   9,   9,  10,  10,  11,  11,  12,  11,  12,  12,  13,  13,  14,  14,
};

const Uint8 wari18_sel_11[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,
};

const Uint8 *const wari18_cmd_11[] = {
	wari18_cmd_11_00, wari18_cmd_11_01, wari18_cmd_11_02,
};

const Uint8 wari18_rate_12[] = {
	  0,   0,   0,   0,   1,   1,   2,   2,   3,   3,   4,   4,   5,   5,   6,   6,
	  6,   6,   6,   7,   7,   7,   8,   8,   8,   9,   9,  10,  10,  11,  11,  12,
};

const Uint8 wari18_sel_12[] = {
	  0,   0,   0,   0,   0,   3,   3,   3,   0,   0,   0,   0,   0,   3,   3,   3,
	  1,   1,   1,   1,   1,   3,   3,   3,   2,   2,   2,   2,   2,   3,   3,   3,
};

const Uint8 *const wari18_cmd_12[] = {
	wari18_cmd_12_00, wari18_cmd_12_01, wari18_cmd_12_02, wari18_cmd_12_03,
};

const Uint8 wari18_rate_13[] = {
	  8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,
	 24,  25,  26,  27,  28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	  1,   2,   3,   3,   4,   4,   5,   5,   6,   6,   6,   6,   6,   6,   6,   6,
	  7,   7,   7,   7,   7,   7,   7,   7,   8,   8,   8,   8,   8,   8,   8,   8,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,
	  3,   3,   3,   3,   3,   3,   3,   3,   4,   4,   4,   4,   5,   5,   5,   5,
	  4,   4,   4,   4,   6,   6,   6,   6,   8,   8,   8,   8,  10,  10,  10,  10,
	 12,  12,  12,  12,  14,  14,  14,  14,  16,  16,  16,  16,  16,  16,  16,  16,
};

const Uint8 wari18_cmd_02_00[] = {
	CMD_EXIT,
};

const Uint8 wari18_cmd_02_01[] = {
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

const Uint8 wari18_cmd_02_02[] = {
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

const Uint8 wari18_cmd_03_00[] = {
	CMD_EXIT,
};

const Uint8 wari18_cmd_05_00[] = {
	CMD_EXIT,
};

const Uint8 wari18_cmd_06_00[] = {
	CMD_EXIT,
};

const Uint8 wari18_cmd_07_00[] = {
	CMD_EXIT,
};

const Uint8 wari18_cmd_07_01[] = {
	CMD_EXIT,
};

const Uint8 wari18_cmd_07_02[] = {
	CMD_EXIT,
};

const Uint8 wari18_cmd_08_00[] = {
	CMD_EXIT,
};

const Uint8 wari18_cmd_09_00[] = {
	CMD_EXIT,
};

const Uint8 wari18_cmd_09_01[] = {
	CMD_EXIT,
};

const Uint8 wari18_cmd_09_02[] = {
	CMD_EXIT,
	STAND,
	WAIT2( 12 ),
	JUMP_LEVEL( 8, 8, 8, 15, 8, 24, 26 ),
	CMD_EXIT,
	WAIT2( 2 ),
	CMD_EXIT,
	WAIT2( 14 ),
	CMD_EXIT,
	WAIT2( 6 ),
	CMD_EXIT,
};

const Uint8 wari18_cmd_10_00[] = {
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 wari18_cmd_10_01[] = {
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 wari18_cmd_10_02[] = {
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 wari18_cmd_11_00[] = {
	WAIT2( 14 ),
	CMD_EXIT,
};

const Uint8 wari18_cmd_11_01[] = {
	WAIT2( 14 ),
	CMD_EXIT,
};

const Uint8 wari18_cmd_11_02[] = {
	WAIT2( 14 ),
	CMD_EXIT,
};

const Uint8 wari18_cmd_12_00[] = {
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 wari18_cmd_12_01[] = {
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 wari18_cmd_12_02[] = {
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 wari18_cmd_12_03[] = {
	WAIT2( 2 ),
	CMD_EXIT,
};

/* Shuma Gorath (character 2d): the computer's lists for cutting in.
   What it does when something happens in the fight. PLAYER_WORK's cpu_adrs[2] points here; the
   event checks of em_play.c start the modes.

   wari2d_tbl        a record (CPU_DATA) for each mode from 2 on: the offsets of its tables.
   wari2d_rate_MM    the chance, out of 32, that mode MM is started, by the level of play.
   wari2d_sel_MM     32 picks; one is taken at random.
   wari2d_cmd_MM     the mode's lists: a pick is a number in this table.
   wari2d_cmd_MM_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name
                    a list of lib2d.c.
   wari2d_unused_N   bytes that nothing in the file reaches. */
#include "player.h"
#include "pl2d_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const wari2d_cmd_02[], *const wari2d_cmd_03[], *const wari2d_cmd_05[],
	*const wari2d_cmd_06[], *const wari2d_cmd_07[], *const wari2d_cmd_08[], *const wari2d_cmd_09[],
	*const wari2d_cmd_10[], *const wari2d_cmd_11[], *const wari2d_cmd_12[];
extern const Uint8 wari2d_rate_02[], wari2d_sel_02[], wari2d_rate_03[], wari2d_rate_04[],
	wari2d_sel_03[], wari2d_rate_05[], wari2d_sel_05[], wari2d_rate_06[], wari2d_sel_06[],
	wari2d_rate_07[], wari2d_sel_07[], wari2d_rate_08[], wari2d_sel_08[], wari2d_rate_09[],
	wari2d_sel_09[], wari2d_unused_0[], wari2d_rate_10[], wari2d_sel_10[], wari2d_rate_11[],
	wari2d_sel_11[], wari2d_rate_12[], wari2d_sel_12[], wari2d_rate_13[], wari2d_cmd_02_00[],
	wari2d_cmd_02_01[], wari2d_cmd_02_02[], wari2d_cmd_02_03[], wari2d_cmd_03_00[],
	wari2d_cmd_05_00[], wari2d_cmd_05_01[], wari2d_cmd_05_02[], wari2d_cmd_05_03[],
	wari2d_cmd_06_00[], wari2d_cmd_07_00[], wari2d_cmd_07_01[], wari2d_cmd_07_02[],
	wari2d_cmd_08_00[], wari2d_cmd_09_00[], wari2d_cmd_09_01[], wari2d_cmd_09_02[],
	wari2d_cmd_10_00[], wari2d_cmd_10_01[], wari2d_cmd_10_02[], wari2d_cmd_11_00[],
	wari2d_cmd_11_01[], wari2d_cmd_11_02[], wari2d_cmd_12_00[], wari2d_cmd_12_01[],
	wari2d_cmd_12_02[];

const CPU_DATA wari2d_tbl[] = {
	{ ( Sint32 )wari2d_rate_02, ( Sint32 )wari2d_sel_02, ( Sint32 )wari2d_cmd_02 },	/* 2: a throw would catch the opponent */
	{ ( Sint32 )wari2d_rate_03, ( Sint32 )wari2d_sel_03, ( Sint32 )wari2d_cmd_03 },	/* 3: it guards a hit (the chance is the program's own) */
	{ ( Sint32 )wari2d_rate_04, ( Sint32 )wari2d_sel_03, ( Sint32 )wari2d_cmd_03 },	/* 4: never started: a guard in the air is mode 3 */
	{ ( Sint32 )wari2d_rate_05, ( Sint32 )wari2d_sel_05, ( Sint32 )wari2d_cmd_05 },	/* 5: after it has guarded on the ground */
	{ ( Sint32 )wari2d_rate_06, ( Sint32 )wari2d_sel_06, ( Sint32 )wari2d_cmd_06 },	/* 6: after it has guarded in the air */
	{ ( Sint32 )wari2d_rate_07, ( Sint32 )wari2d_sel_07, ( Sint32 )wari2d_cmd_07 },	/* 7: after it has been hit */
	{ ( Sint32 )wari2d_rate_08, ( Sint32 )wari2d_sel_08, ( Sint32 )wari2d_cmd_08 },	/* 8: never started */
	{ ( Sint32 )wari2d_rate_09, ( Sint32 )wari2d_sel_09, ( Sint32 )wari2d_cmd_09 },	/* 9: after it has got up */
	{ ( Sint32 )wari2d_rate_10, ( Sint32 )wari2d_sel_10, ( Sint32 )wari2d_cmd_10 },	/* 10: the opponent dashes */
	{ ( Sint32 )wari2d_rate_11, ( Sint32 )wari2d_sel_11, ( Sint32 )wari2d_cmd_11 },	/* 11: the opponent flies or super jumps */
	{ ( Sint32 )wari2d_rate_12, ( Sint32 )wari2d_sel_12, ( Sint32 )wari2d_cmd_12 },	/* 12: the opponent jumps */
	{ ( Sint32 )wari2d_rate_13, 0, 0 },	/* 13: an attack is coming: the chance alone, the lists are yok2d.c's */
};

const Uint8 wari2d_rate_02[] = {
	  9,   9,  10,  10,  11,  11,  12,  12,  13,  13,  14,  14,  15,  15,  16,  16,
	 17,  18,  19,  20,  21,  22,  23,  24,  24,  24,  24,  24,  24,  24,  24,  24,
};

const Uint8 wari2d_sel_02[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   3,   3,   3,   0,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,
};

const Uint8 *const wari2d_cmd_02[] = {
	wari2d_cmd_02_00, wari2d_cmd_02_01, wari2d_cmd_02_02, wari2d_cmd_02_03,
};

const Uint8 wari2d_rate_03[] = {
	  1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   3,   3,   3,
};

const Uint8 wari2d_rate_04[] = {
	 16,  16,  17,  17,  18,  19,  20,  21,  22,  23,  24,  25,  25,  25,  25,  25,
	 25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,
};

const Uint8 wari2d_sel_03[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const wari2d_cmd_03[] = {
	wari2d_cmd_03_00,
};

const Uint8 wari2d_rate_05[] = {
	 19,  19,  20,  20,  21,  21,  22,  22,  23,  23,  24,  24,  25,  25,  26,  26,
	 27,  27,  28,  28,  29,  29,  30,  30,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari2d_sel_05[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   3,   3,   3,
	  1,   1,   1,   1,   2,   2,   2,   2,   1,   1,   1,   1,   1,   1,   1,   1,
};

const Uint8 *const wari2d_cmd_05[] = {
	wari2d_cmd_05_00, wari2d_cmd_05_01, wari2d_cmd_05_02, wari2d_cmd_05_03,
};

const Uint8 wari2d_rate_06[] = {
	  4,   4,   4,   8,  12,  16,  18,  20,  22,  24,  26,  28,  30,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari2d_sel_06[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const wari2d_cmd_06[] = {
	wari2d_cmd_06_00,
};

const Uint8 wari2d_rate_07[] = {
	  6,   6,   7,   7,   8,   8,   9,   9,  10,  10,  11,  11,  12,  12,  13,  13,
	 14,  14,  14,  14,  15,  15,  15,  15,  16,  16,  16,  16,  17,  17,  17,  17,
};

const Uint8 wari2d_sel_07[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
};

const Uint8 *const wari2d_cmd_07[] = {
	wari2d_cmd_07_00, wari2d_cmd_07_01, wari2d_cmd_07_02,
};

const Uint8 wari2d_rate_08[] = {
	 28,  29,  30,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari2d_sel_08[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const wari2d_cmd_08[] = {
	wari2d_cmd_08_00,
};

const Uint8 wari2d_rate_09[] = {
	 24,  25,  26,  27,  28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari2d_sel_09[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   1,   1,   1,   1,
};

const Uint8 *const wari2d_cmd_09[] = {
	wari2d_cmd_09_00, wari2d_cmd_09_01, wari2d_cmd_09_02,
};

const Uint8 wari2d_unused_0[] = {
	 20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,
	 20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,
	  0,   0,   0,   0,   0,   0,   0,   0,   2,   2,   2,   2,   0,   0,   0,   0,
	  2,   2,   2,   2,   2,   2,   2,   2,   1,   1,   1,   1,   1,   1,   1,   1,
	  4,   7,   0,   0,  18,   7,   0,   0,  30,   7,   0,   0,  31,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  51,   7,   0,   0,
};

const Uint8 wari2d_rate_10[] = {
	  1,   2,   3,   4,   5,   6,   7,   7,   7,   8,   8,   8,   9,   9,   9,  10,
	 10,  10,  11,  12,  13,  14,  15,  16,  16,  16,  16,  16,  16,  16,  16,  16,
};

const Uint8 wari2d_sel_10[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,
};

const Uint8 *const wari2d_cmd_10[] = {
	wari2d_cmd_10_00, wari2d_cmd_10_01, wari2d_cmd_10_02,
};

const Uint8 wari2d_rate_11[] = {
	  0,   0,   1,   1,   2,   2,   3,   3,   4,   4,   5,   5,   6,   6,   7,   7,
	  8,   8,   9,   9,  10,  10,  11,  11,  12,  13,  14,  15,  16,  17,  18,  19,
};

const Uint8 wari2d_sel_11[] = {
	  0,   0,   0,   0,   2,   2,   2,   2,   1,   1,   1,   1,   1,   2,   2,   2,
	  1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
};

const Uint8 *const wari2d_cmd_11[] = {
	wari2d_cmd_11_00, wari2d_cmd_11_01, wari2d_cmd_11_02,
};

const Uint8 wari2d_rate_12[] = {
	  0,   0,   0,   0,   1,   1,   2,   2,   3,   3,   4,   4,   5,   5,   6,   6,
	  6,   6,   6,   7,   7,   7,   8,   8,   8,   9,   9,  10,  10,  11,  11,  12,
};

const Uint8 wari2d_sel_12[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,
	  2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
};

const Uint8 *const wari2d_cmd_12[] = {
	wari2d_cmd_12_00, wari2d_cmd_12_01, wari2d_cmd_12_02,
};

const Uint8 wari2d_rate_13[] = {
	 28,  29,  30,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	  1,   2,   3,   3,   4,   4,   5,   5,   6,   6,   6,   6,   6,   6,   6,   6,
	  7,   7,   7,   7,   7,   7,   7,   7,   8,   8,   8,   8,   8,   8,   8,   8,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,
	  3,   3,   3,   3,   3,   3,   3,   3,   4,   4,   4,   4,   5,   5,   5,   5,
	  4,   4,   4,   4,   6,   6,   6,   6,   8,   8,   8,   8,  10,  10,  10,  10,
	 12,  12,  12,  12,  14,  14,  14,  14,  16,  16,  16,  16,  16,  16,  16,  16,
};

const Uint8 wari2d_cmd_02_00[] = {
	STAND,
	CMD_JUMP( 70 ),
	IF_CATCH,
		IF_LEVEL( 2 ),
			JUMP_LEVEL( 53, 7, 54, 13, 55, 21, 56 ),
			IF_RND( 16 ),
				WAIT2( 2 ),
				NAG_VFLIP_ADJ( 0, LEVER_BACK ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_RND( 4 ),
				WAIT2( 10 ),
				NAG_VFLIP_ADJ( 0, LEVER_BACK ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari2d_cmd_02_01[] = {
	STAND,
	CMD_JUMP( 70 ),
	IF_LEVEL( 24 ),
		IF_RND( 28 ),
			CMD_JUMP( 32 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 21 ),
			IF_RND( 24 ),
				WAIT2( 2 ),
				CMD_JUMP( 32 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 13 ),
				IF_RND( 20 ),
					WAIT2( 2 ),
					CMD_JUMP( 32 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 7 ),
					IF_RND( 16 ),
						WAIT2( 4 ),
						CMD_JUMP( 32 ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_RND( 12 ),
						WAIT2( 6 ),
						CMD_JUMP( 32 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 wari2d_cmd_02_02[] = {
	STAND,
	CMD_JUMP( 70 ),
	IF_LEVEL( 24 ),
		IF_RND( 28 ),
			CMD_JUMP( 2 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 21 ),
			IF_RND( 20 ),
				WAIT2( 2 ),
				CMD_JUMP( 2 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 13 ),
				IF_RND( 14 ),
					WAIT2( 2 ),
					CMD_JUMP( 2 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 7 ),
					IF_RND( 8 ),
						WAIT2( 4 ),
						CMD_JUMP( 2 ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_RND( 1 ),
						WAIT2( 6 ),
						CMD_JUMP( 2 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 wari2d_cmd_02_03[] = {
	STAND,
	CMD_JUMP( 70 ),
	IF_LEVEL( 24 ),
		IF_RND( 28 ),
			SP_CMD( pl2d_sp_devital, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 21 ),
			IF_RND( 20 ),
				WAIT2( 2 ),
				SP_CMD( pl2d_sp_devital, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 13 ),
				IF_RND( 14 ),
					WAIT2( 2 ),
					SP_CMD( pl2d_sp_devital, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 7 ),
					IF_RND( 8 ),
						WAIT2( 4 ),
						SP_CMD( pl2d_sp_devital, 0, PK_PUNCH, LV_LIGHT, 0 ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_RND( 1 ),
						WAIT2( 6 ),
						SP_CMD( pl2d_sp_devital, 0, PK_PUNCH, LV_LIGHT, 0 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 wari2d_cmd_03_00[] = {
	CMD_JUMP( 70 ),
	GUARD_CONT( 16 ),
	CMD_EXIT,
};

const Uint8 wari2d_cmd_05_00[] = {
	LOCK_SET,
	CMD_JUMP( 70 ),
	IF_LEVEL( 3 ),
		IF_LEVEL( 8 ),
			CMD_NOP,
		CMD_ELSE,
			GUARD( 1 ),
		CMD_ENDIF,
		IF_CATCH,
			NAG_VFLIP_ADJ( 1, LEVER_BACK ),
		CMD_ELSE,
			JUMP( DIR_UP ),
			JUMP_FLAG_SET,
				JUMP_LEVEL( 48, 7, 49, 13, 50, 21, 51 ),
				IF_LEVEL( 18 ),
					IF_RND( 24 ),
						CMD_JUMP( 24 ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_RND( 16 ),
						CMD_JUMP( 24 ),
					CMD_ENDIF,
				CMD_ENDIF,
				JUMP_FLAG_CLR,
			CMD_ENDIF,
			STAND,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 wari2d_cmd_05_01[] = {
	CMD_JUMP( 70 ),
	STAND,
	TIMER2_SET( 4 ),
	IF_LEVEL( 24 ),
		IF_RND( 24 ),
			CMD_JUMP( 32 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 21 ),
			IF_RND( 22 ),
				WAIT2( 2 ),
				CMD_JUMP( 32 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 13 ),
				IF_RND( 20 ),
					WAIT2( 2 ),
					CMD_JUMP( 3 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 7 ),
					IF_RND( 18 ),
						WAIT2( 4 ),
						CMD_JUMP( 32 ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_RND( 14 ),
						WAIT2( 6 ),
						CMD_JUMP( 32 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 wari2d_cmd_05_02[] = {
	CMD_JUMP( 70 ),
	STAND,
	TIMER2_SET( 4 ),
	IF_LEVEL( 24 ),
		IF_RND( 16 ),
			JUMP_LEVEL( 33, 0, 34, 0, 35, 0, 2 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 21 ),
			IF_RND( 12 ),
				WAIT2( 2 ),
				JUMP_LEVEL( 33, 0, 34, 0, 35, 0, 2 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 13 ),
				IF_RND( 12 ),
					WAIT2( 2 ),
					JUMP_LEVEL( 33, 0, 34, 0, 35, 0, 2 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 7 ),
					IF_RND( 8 ),
						WAIT2( 4 ),
						JUMP_LEVEL( 33, 0, 34, 0, 35, 0, 2 ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_RND( 2 ),
						WAIT2( 6 ),
						JUMP_LEVEL( 33, 0, 34, 0, 35, 0, 2 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 wari2d_cmd_05_03[] = {
	CMD_JUMP( 70 ),
	STAND,
	IF_DIST( 40, 16, 40, 0 ),
		IF_LEVEL( 24 ),
			IF_RND( 28 ),
				SP_CMD( pl2d_sp_devital, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 21 ),
				IF_RND( 20 ),
					WAIT2( 2 ),
					SP_CMD( pl2d_sp_devital, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 13 ),
					IF_RND( 14 ),
						WAIT2( 2 ),
						SP_CMD( pl2d_sp_devital, 0, PK_PUNCH, LV_LIGHT, 0 ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_LEVEL( 7 ),
						IF_RND( 8 ),
							WAIT2( 4 ),
							SP_CMD( pl2d_sp_devital, 0, PK_PUNCH, LV_LIGHT, 0 ),
						CMD_ENDIF,
					CMD_ELSE,
						IF_RND( 1 ),
							WAIT2( 6 ),
							SP_CMD( pl2d_sp_devital, 0, PK_PUNCH, LV_LIGHT, 0 ),
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		DASH_B( 192 ),
	CMD_ENDIF,
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 wari2d_cmd_06_00[] = {
	CMD_JUMP( 72 ),
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 wari2d_cmd_07_00[] = {
	CMD_JUMP( 70 ),
	GUARD( 30 ),
	CMD_EXIT,
};

const Uint8 wari2d_cmd_07_01[] = {
	CMD_JUMP( 70 ),
	GUARD( 30 ),
	CMD_EXIT,
};

const Uint8 wari2d_cmd_07_02[] = {
	CMD_JUMP( 70 ),
	GUARD( 30 ),
	CMD_EXIT,
};

const Uint8 wari2d_cmd_08_00[] = {
	CMD_JUMP( 72 ),
	GUARD( 30 ),
	CMD_EXIT,
};

const Uint8 wari2d_cmd_09_00[] = {
	LOCK_SET,
	CMD_JUMP( 70 ),
	STAND,
	IF_LEVEL( 5 ),
		JUMP_LEVEL( 53, 16, 54, 22, 55, 28, 56 ),
	CMD_ENDIF,
	STAND,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 wari2d_cmd_09_01[] = {
	CMD_JUMP( 70 ),
	IF_LEVEL( 6 ),
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

const Uint8 wari2d_cmd_09_02[] = {
	LOCK_SET,
	CMD_JUMP( 70 ),
	VCOUNTER_SET( 6 ),
	CROUCH,
	IF_ATCK_COME,
		GUARD( 30 ),
	CMD_ELSE,
		IF_EM_ATCK( ATCK_CROUCH_LK ),
			IF_LEVEL( 8 ),
				CMD_JUMP( 2 ),
			CMD_ELSE,
				CANCEL_SET,
					CROUCH,
					PUNCH( LV_LIGHT ),
					KICK( LV_LIGHT ),
					CANCEL_CLR,
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
	LOCK_SET,
	CMD_JUMP( 70 ),
	STAND,
	DASH_EM( 16 ),
	IF_CATCH,
		NAG_VFLIP_ADJ2( 1, LEVER_BACK ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
	LOCK_SET,
	CMD_JUMP( 70 ),
	STAND,
	WAIT2( 4 ),
	CMD_JUMP( 10 ),
	WAIT2( 6 ),
	LOCK_CLR,
	CMD_EXIT,
	LOCK_SET,
	CMD_JUMP( 70 ),
	STAND,
	WALK_B( 32 ),
	STAND,
	WAIT2( 4 ),
	SP_CMD( 4, 0, PK_PUNCH, LV_LIGHT, 0 ),
	STAND,
	WAIT2( 14 ),
	LOCK_CLR,
	CMD_EXIT,
	CMD_JUMP( 70 ),
	WAIT2( 6 ),
	CMD_EXIT,
};

const Uint8 wari2d_cmd_10_00[] = {
	CMD_JUMP( 70 ),
	GUARD( 30 ),
	CMD_EXIT,
};

const Uint8 wari2d_cmd_10_01[] = {
	CMD_JUMP( 70 ),
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 wari2d_cmd_10_02[] = {
	CMD_JUMP( 70 ),
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 wari2d_cmd_11_00[] = {
	CMD_JUMP( 71 ),
	STAND,
	WAIT2( 8 ),
	IF_LEVEL( 24 ),
		SJUMP( DIR_FORWARD ),
		SJUMP_FLAG_SET,
			JUMP_LEVEL( 42, 7, 43, 13, 44, 21, 45 ),
			TIMER_SET( 100 ),
			IF_HEIGHT( 1, 288 ),
				IF_CATCH,
					IF_LEVEL( 24 ),
						CMD_NOP,
					CMD_ELSE,
						WAIT2( 4 ),
					CMD_ENDIF,
					NAG_VFLIP_ADJ2( 0, LEVER_BACK ),
				CMD_ENDIF,
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
		STAND,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari2d_cmd_11_01[] = {
	CMD_JUMP( 71 ),
	STAND,
	WAIT2( 6 ),
	IF_LEVEL( 18 ),
		SJUMP( DIR_FORWARD ),
		SJUMP_FLAG_SET,
			JUMP_LEVEL( 42, 7, 43, 13, 44, 21, 45 ),
			WAIT( 30 ),
			CMD_JUMP( 11 ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 wari2d_cmd_11_02[] = {
	CMD_JUMP( 71 ),
	CROUCH,
	WAIT( 10 ),
	TIMER_SET( 120 ),
	IF_EM_ATCK( ATCK_CROUCH_HK ),
		KICK( LV_HEAVY ),
	CMD_ENDIF,
	CROUCH,
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 wari2d_cmd_12_00[] = {
	CMD_JUMP( 71 ),
	CROUCH,
	WAIT2( 14 ),
	TIMER_SET( 60 ),
	IF_EM_ATCK( ATCK_CROUCH_HK ),
		KICK( LV_HEAVY ),
	CMD_ENDIF,
	CROUCH,
	CMD_EXIT,
};

const Uint8 wari2d_cmd_12_01[] = {
	CMD_JUMP( 71 ),
	CROUCH,
	WAIT2( 4 ),
	IF_LEVEL( 31 ),
		IF_AREA( 48, 48, 64, 64 ),
			SP_CMD( pl2d_sp_smash, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CROUCH,
	CMD_EXIT,
};

const Uint8 wari2d_cmd_12_02[] = {
	CMD_JUMP( 71 ),
	STAND,
	WAIT2( 6 ),
	JUMP_LEVEL( 48, 7, 49, 13, 50, 21, 51 ),
	CMD_JUMP( 0 ),
	STAND,
	WAIT2( 2 ),
	CMD_EXIT,
};

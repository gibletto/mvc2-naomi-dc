/* Spiderman (character 0c): the computer's lists for cutting in.
   What it does when something happens in the fight. PLAYER_WORK's cpu_adrs[2] points here; the
   event checks of em_play.c start the modes.

   wari0c_tbl        a record (CPU_DATA) for each mode from 2 on: the offsets of its tables.
   wari0c_rate_MM    the chance, out of 32, that mode MM is started, by the level of play.
   wari0c_sel_MM     32 picks; one is taken at random.
   wari0c_cmd_MM     the mode's lists: a pick is a number in this table.
   wari0c_cmd_MM_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name
                    a list of lib0c.c. */
#include "player.h"
#include "pl0c_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const wari0c_cmd_02[], *const wari0c_cmd_03[], *const wari0c_cmd_05[],
	*const wari0c_cmd_06[], *const wari0c_cmd_07[], *const wari0c_cmd_08[], *const wari0c_cmd_09[],
	*const wari0c_cmd_10[], *const wari0c_cmd_11[], *const wari0c_cmd_12[];
extern const Uint8 wari0c_rate_02[], wari0c_sel_02[], wari0c_rate_03[], wari0c_rate_04[],
	wari0c_sel_03[], wari0c_rate_05[], wari0c_sel_05[], wari0c_rate_06[], wari0c_sel_06[],
	wari0c_rate_07[], wari0c_sel_07[], wari0c_rate_08[], wari0c_sel_08[], wari0c_rate_09[],
	wari0c_sel_09[], wari0c_rate_10[], wari0c_sel_10[], wari0c_rate_11[], wari0c_sel_11[],
	wari0c_rate_12[], wari0c_sel_12[], wari0c_rate_13[], wari0c_cmd_02_00[], wari0c_cmd_02_01[],
	wari0c_cmd_02_02[], wari0c_cmd_03_00[], wari0c_cmd_05_00[], wari0c_cmd_05_01[],
	wari0c_cmd_05_02[], wari0c_cmd_05_03[], wari0c_cmd_06_00[], wari0c_cmd_07_00[],
	wari0c_cmd_07_01[], wari0c_cmd_07_02[], wari0c_cmd_08_00[], wari0c_cmd_09_00[],
	wari0c_cmd_09_01[], wari0c_cmd_09_02[], wari0c_cmd_10_00[], wari0c_cmd_10_01[],
	wari0c_cmd_10_02[], wari0c_cmd_11_00[], wari0c_cmd_11_01[], wari0c_cmd_11_02[],
	wari0c_cmd_12_00[], wari0c_cmd_12_01[], wari0c_cmd_12_02[], wari0c_cmd_12_03[];

const CPU_DATA wari0c_tbl[] = {
	{ ( Sint32 )wari0c_rate_02, ( Sint32 )wari0c_sel_02, ( Sint32 )wari0c_cmd_02 },	/* 2: a throw would catch the opponent */
	{ ( Sint32 )wari0c_rate_03, ( Sint32 )wari0c_sel_03, ( Sint32 )wari0c_cmd_03 },	/* 3: it guards a hit (the chance is the program's own) */
	{ ( Sint32 )wari0c_rate_04, ( Sint32 )wari0c_sel_03, ( Sint32 )wari0c_cmd_03 },	/* 4: never started: a guard in the air is mode 3 */
	{ ( Sint32 )wari0c_rate_05, ( Sint32 )wari0c_sel_05, ( Sint32 )wari0c_cmd_05 },	/* 5: after it has guarded on the ground */
	{ ( Sint32 )wari0c_rate_06, ( Sint32 )wari0c_sel_06, ( Sint32 )wari0c_cmd_06 },	/* 6: after it has guarded in the air */
	{ ( Sint32 )wari0c_rate_07, ( Sint32 )wari0c_sel_07, ( Sint32 )wari0c_cmd_07 },	/* 7: after it has been hit */
	{ ( Sint32 )wari0c_rate_08, ( Sint32 )wari0c_sel_08, ( Sint32 )wari0c_cmd_08 },	/* 8: never started */
	{ ( Sint32 )wari0c_rate_09, ( Sint32 )wari0c_sel_09, ( Sint32 )wari0c_cmd_09 },	/* 9: after it has got up */
	{ ( Sint32 )wari0c_rate_10, ( Sint32 )wari0c_sel_10, ( Sint32 )wari0c_cmd_10 },	/* 10: the opponent dashes */
	{ ( Sint32 )wari0c_rate_11, ( Sint32 )wari0c_sel_11, ( Sint32 )wari0c_cmd_11 },	/* 11: the opponent flies or super jumps */
	{ ( Sint32 )wari0c_rate_12, ( Sint32 )wari0c_sel_12, ( Sint32 )wari0c_cmd_12 },	/* 12: the opponent jumps */
	{ ( Sint32 )wari0c_rate_13, 0, 0 },	/* 13: an attack is coming: the chance alone, the lists are yok0c.c's */
};

const Uint8 wari0c_rate_02[] = {
	  9,   9,  10,  10,  11,  11,  12,  12,  13,  13,  14,  14,  15,  15,  16,  16,
	 17,  18,  19,  20,  21,  22,  23,  24,  24,  24,  24,  24,  24,  24,  24,  24,
};

const Uint8 wari0c_sel_02[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,
};

const Uint8 *const wari0c_cmd_02[] = {
	wari0c_cmd_02_00, wari0c_cmd_02_01, wari0c_cmd_02_02,
};

const Uint8 wari0c_rate_03[] = {
	  1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   3,   3,   3,
};

const Uint8 wari0c_rate_04[] = {
	 16,  16,  17,  17,  18,  19,  20,  21,  22,  23,  24,  25,  25,  25,  25,  25,
	 25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,
};

const Uint8 wari0c_sel_03[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const wari0c_cmd_03[] = {
	wari0c_cmd_03_00,
};

const Uint8 wari0c_rate_05[] = {
	 19,  19,  20,  20,  21,  21,  22,  22,  23,  23,  24,  24,  25,  25,  26,  26,
	 27,  27,  28,  28,  29,  29,  30,  30,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari0c_sel_05[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   3,   3,   3,   3,
};

const Uint8 *const wari0c_cmd_05[] = {
	wari0c_cmd_05_00, wari0c_cmd_05_01, wari0c_cmd_05_02, wari0c_cmd_05_03,
};

const Uint8 wari0c_rate_06[] = {
	  4,   4,   4,   8,  12,  16,  18,  20,  22,  24,  26,  28,  30,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari0c_sel_06[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const wari0c_cmd_06[] = {
	wari0c_cmd_06_00,
};

const Uint8 wari0c_rate_07[] = {
	 12,  12,  12,  12,  12,  12,  13,  13,  13,  13,  13,  13,  14,  14,  14,  14,
	 15,  15,  15,  15,  16,  16,  16,  16,  17,  17,  17,  17,  18,  18,  18,  18,
};

const Uint8 wari0c_sel_07[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
};

const Uint8 *const wari0c_cmd_07[] = {
	wari0c_cmd_07_00, wari0c_cmd_07_01, wari0c_cmd_07_02,
};

const Uint8 wari0c_rate_08[] = {
	  4,   4,   4,   8,  12,  16,  18,  20,  22,  24,  26,  28,  30,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari0c_sel_08[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const wari0c_cmd_08[] = {
	wari0c_cmd_08_00,
};

const Uint8 wari0c_rate_09[] = {
	 24,  25,  26,  27,  28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari0c_sel_09[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,
	  1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,
};

const Uint8 *const wari0c_cmd_09[] = {
	wari0c_cmd_09_00, wari0c_cmd_09_01, wari0c_cmd_09_02, wari0c_cmd_09_02,
};

const Uint8 wari0c_rate_10[] = {
	  5,   6,   7,   8,   9,  10,  11,  12,  12,  13,  13,  14,  14,  15,  15,  15,
	 16,  16,  16,  17,  17,  18,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,
};

const Uint8 wari0c_sel_10[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,
};

const Uint8 *const wari0c_cmd_10[] = {
	wari0c_cmd_10_00, wari0c_cmd_10_01, wari0c_cmd_10_02,
};

const Uint8 wari0c_rate_11[] = {
	  0,   0,   1,   1,   2,   2,   3,   3,   4,   4,   5,   5,   6,   6,   7,   7,
	  8,   8,   9,   9,  10,  10,  11,  11,  12,  11,  12,  12,  13,  13,  14,  14,
};

const Uint8 wari0c_sel_11[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,
};

const Uint8 *const wari0c_cmd_11[] = {
	wari0c_cmd_11_00, wari0c_cmd_11_01, wari0c_cmd_11_02,
};

const Uint8 wari0c_rate_12[] = {
	  8,   8,   9,   9,  10,  10,  11,  11,  12,  12,  13,  13,  14,  14,  15,  15,
	 16,  16,  16,  17,  17,  17,  18,  18,  18,  19,  19,  20,  20,  21,  21,  22,
};

const Uint8 wari0c_sel_12[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
	  1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,
};

const Uint8 *const wari0c_cmd_12[] = {
	wari0c_cmd_12_00, wari0c_cmd_12_01, wari0c_cmd_12_02, wari0c_cmd_12_03,
};

const Uint8 wari0c_rate_13[] = {
	 10,  10,  11,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,
	 24,  25,  26,  27,  28,  29,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	  1,   2,   3,   3,   4,   4,   5,   5,   6,   6,   6,   6,   6,   6,   6,   6,
	  7,   7,   7,   7,   7,   7,   7,   7,   8,   8,   8,   8,   8,   8,   8,   8,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,
	  3,   3,   3,   3,   3,   3,   3,   3,   4,   4,   4,   4,   5,   5,   5,   5,
	  4,   4,   4,   4,   6,   6,   6,   6,   8,   8,   8,   8,  10,  10,  10,  10,
	 12,  12,  12,  12,  14,  14,  14,  14,  16,  16,  16,  16,  16,  16,  16,  16,
};

const Uint8 wari0c_cmd_02_00[] = {
	STAND,
	IF_LEVEL( 26 ),
		CMD_JUMP( 31 ),
	CMD_ELSE,
		IF_CATCH,
			JUMP_LEVEL( 30, 7, 29, 14, 28, 21, 27 ),
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT2( 6 ),
	CMD_EXIT,
};

const Uint8 wari0c_cmd_02_01[] = {
	STAND,
	IF_LEVEL( 26 ),
		CMD_JUMP( 24 ),
	CMD_ELSE,
		JUMP_LEVEL( 32, 7, 33, 14, 34, 21, 35 ),
	CMD_ENDIF,
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 wari0c_cmd_02_02[] = {
	STAND,
	IF_LEVEL( 24 ),
		CMD_JUMP( 22 ),
	CMD_ELSE,
		IF_RND( 20 ),
			WAIT2( 4 ),
			JUMP_LEVEL( 23, 4, 24, 16, 25, 24, 26 ),
		CMD_ELSE,
			WAIT2( 4 ),
			JUMP_LEVEL( 37, 4, 38, 16, 39, 24, 40 ),
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT2( 4 ),
	CMD_EXIT,
};

const Uint8 wari0c_cmd_03_00[] = {
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 6 ),
		CMD_ELSE,
			GUARD_BACK_SET( 3 ),
		CMD_ENDIF,
	CMD_ENDIF,
	GUARD_CONT( 16 ),
	CMD_EXIT,
};

const Uint8 wari0c_cmd_05_00[] = {
	STAND,
	IF_DIST( 32, 8, 64, 8 ),
		IF_LEVEL( 26 ),
			CMD_JUMP( 24 ),
		CMD_ELSE,
			JUMP_LEVEL( 32, 7, 33, 14, 34, 21, 35 ),
		CMD_ENDIF,
	CMD_ELSE,
		JUMP_LEVEL( 48, 7, 49, 13, 50, 21, 51 ),
	CMD_ENDIF,
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 wari0c_cmd_05_01[] = {
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_SET,
	IF_LEVEL( 3 ),
		IF_CATCH,
			NAG_VFLIP_ADJ( 0, LEVER_FORWARD ),
		CMD_ELSE,
			IF_DIST( 32, 8, 64, 8 ),
				JUMP_LEVEL( 32, 7, 33, 14, 34, 21, 35 ),
			CMD_ELSE,
				JUMP_LEVEL( 48, 7, 49, 13, 50, 21, 51 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	WAIT2( 4 ),
	CMD_EXIT,
};

const Uint8 wari0c_cmd_05_02[] = {
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_SET,
	IF_LEVEL( 3 ),
		IF_CATCH,
			NAG_VFLIP_ADJ( 0, LEVER_FORWARD ),
		CMD_ELSE,
			JUMP_LEVEL( 48, 7, 49, 13, 50, 21, 51 ),
			JUMP( DIR_UP ),
			JUMP_FLAG_SET,
				WAIT( 16 ),
				IF_LEVEL( 16 ),
					IF_RND( 18 ),
						CMD_JUMP( 10 ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_RND( 12 ),
						CMD_JUMP( 9 ),
					CMD_ENDIF,
				CMD_ENDIF,
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	WAIT2( 4 ),
	CMD_EXIT,
};

const Uint8 wari0c_cmd_05_03[] = {
	STAND,
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	IF_DIST( 32, 8, 64, 8 ),
		JUMP_LEVEL( 19, 8, 20, 16, 21, 24, 22 ),
	CMD_ELSE,
		JUMP_LEVEL( 48, 7, 49, 13, 50, 21, 51 ),
	CMD_ENDIF,
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 wari0c_cmd_06_00[] = {
	JUMP_LEVEL( 42, 7, 43, 13, 44, 21, 45 ),
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 wari0c_cmd_07_00[] = {
	WAIT2( 6 ),
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

const Uint8 wari0c_cmd_07_01[] = {
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

const Uint8 wari0c_cmd_07_02[] = {
	IF_LEVEL( 20 ),
		IF_RND( 20 ),
			SP_CMD( pl0c_sp_sting, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari0c_cmd_08_00[] = {
	IF_RND( 16 ),
		GUARD( 60 ),
	CMD_ELSE,
		GUARD( 30 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari0c_cmd_09_00[] = {
	LOCK_SET,
	STAND,
	IF_CATCH,
		IF_LEVEL( 3 ),
			JUMP_LEVEL( 30, 5, 29, 7, 28, 9, 27 ),
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 wari0c_cmd_09_01[] = {
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
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

const Uint8 wari0c_cmd_09_02[] = {
	LOCK_SET,
	CROUCH,
	IF_ATCK_COME,
		GUARD( 30 ),
	CMD_ELSE,
		IF_EM_ATCK( ATCK_STAND_LP ),
			IF_LEVEL( 26 ),
				JUMP_LEVEL( 60, 28, 61, 29, 62, 30, 63 ),
			CMD_ELSE,
				IF_LEVEL( 16 ),
					JUMP_LEVEL( 39, 18, 40, 21, 60, 23, 61 ),
				CMD_ELSE,
					JUMP_LEVEL( 37, 18, 38, 21, 39, 23, 40 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			JUMP( DIR_BACK ),
			JUMP_FLAG_SET,
				JUMP_LEVEL( 48, 5, 49, 16, 50, 22, 51 ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	LOCK_CLR,
	WAIT2( 8 ),
	CMD_EXIT,
	LOCK_SET,
	CROUCH,
	IF_ATCK_COME,
		GUARD( 30 ),
	CMD_ELSE,
		TIMER_SET( 1 ),
		IF_AREA( 32, 32, 64, 32 ),
			IF_LEVEL( 12 ),
				SP_CMD( pl0c_sp_sting, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				KICK( LV_HEAVY ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	WAIT2( 6 ),
	CMD_EXIT,
	STAND,
	LOCK_SET,
	WALK_B( 48 ),
	WAIT( 16 ),
	JUMP_RND( 56, 57, 58, 59 ),
	LOCK_CLR,
	WAIT2( 4 ),
	CMD_EXIT,
	STAND,
	WAIT2( 2 ),
	IF_RND( 20 ),
		SP_CMD( pl0c_sp_maximum, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ELSE,
		SP_CMD( pl0c_sp_crawler, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ENDIF,
	WAIT2( 14 ),
	CMD_EXIT,
	STAND,
	WAIT2( 6 ),
	JUMP_LEVEL( 2, 8, 3, 15, 62, 24, 63 ),
	CMD_EXIT,
	WAIT2( 6 ),
	CMD_EXIT,
};

const Uint8 wari0c_cmd_10_00[] = {
	WAIT2( 2 ),
	IF_RND( 18 ),
		JUMP_LEVEL( 48, 5, 49, 16, 50, 22, 51 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari0c_cmd_10_01[] = {
	IF_EM_ATCK( ATCK_CROUCH_HK ),
		CROUCH,
		KICK( LV_HEAVY ),
	CMD_ELSE,
		IF_RND( 20 ),
			JUMP_LEVEL( 48, 5, 49, 16, 50, 22, 51 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari0c_cmd_10_02[] = {
	TIMER_SET( 16 ),
	IF_DIST( 48, 16, 80, 96 ),
		SP_CMD( pl0c_sp_sting, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ELSE,
		IF_RND( 8 ),
			JUMP_LEVEL( 48, 5, 49, 16, 50, 22, 51 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari0c_cmd_11_00[] = {
	STAND,
	WAIT2( 6 ),
	JUMP_LEVEL( 42, 12, 43, 19, 44, 29, 45 ),
	IF_RND( 18 ),
		SJUMP( DIR_UP ),
		SJUMP_FLAG_SET,
			CMD_JUMP( 46 ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		IF_RND( 16 ),
			SJUMP( DIR_FORWARD ),
			SJUMP_FLAG_SET,
				CMD_JUMP( 46 ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			SJUMP( DIR_BACK ),
			SJUMP_FLAG_SET,
				CMD_JUMP( 46 ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	WAIT2( 10 ),
	CMD_EXIT,
};

const Uint8 wari0c_cmd_11_01[] = {
	STAND,
	WAIT2( 6 ),
	IF_RND( 18 ),
		SJUMP( DIR_UP ),
		SJUMP_FLAG_SET,
			CMD_JUMP( 47 ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		IF_RND( 16 ),
			SJUMP( DIR_FORWARD ),
			SJUMP_FLAG_SET,
				CMD_JUMP( 47 ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			SJUMP( DIR_BACK ),
			SJUMP_FLAG_SET,
				CMD_JUMP( 47 ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	WAIT2( 10 ),
	CMD_EXIT,
};

const Uint8 wari0c_cmd_11_02[] = {
	STAND,
	CMD_JUMP( 41 ),
	DASH_EM( 8 ),
	TIMER_SET( 2 ),
	IF_RND( 16 ),
		IF_DIST( 36, 80, 48, 208 ),
			SP_CMD( pl0c_sp_webthrow, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ELSE,
			SP_CMD( pl0c_sp_webthrow, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ENDIF,
	CMD_ELSE,
		DASH_B( 192 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 6 ),
	CMD_EXIT,
};

const Uint8 wari0c_cmd_12_00[] = {
	STAND,
	JUMP_LEVEL( 48, 7, 49, 13, 50, 21, 51 ),
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 wari0c_cmd_12_01[] = {
	STAND,
	IF_RND( 16 ),
		JUMP_LEVEL( 48, 7, 49, 13, 50, 21, 51 ),
	CMD_ELSE,
		JUMP_LEVEL( 14, 7, 15, 13, 16, 21, 17 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari0c_cmd_12_02[] = {
	STAND,
	IF_RND( 16 ),
		JUMP_LEVEL( 48, 7, 49, 13, 50, 21, 51 ),
	CMD_ELSE,
		JUMP_LEVEL( 14, 7, 15, 13, 16, 21, 17 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari0c_cmd_12_03[] = {
	STAND,
	IF_ATCK_COME,
		SP_CMD( pl0c_sp_sting, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ELSE,
		TIMER2_SET( 60 ),
	CMD_ENDIF,
	CMD_EXIT,
};

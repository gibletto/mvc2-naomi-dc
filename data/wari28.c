/* Gambit (character 28): the computer's lists for cutting in.
   What it does when something happens in the fight. PLAYER_WORK's cpu_adrs[2] points here; the
   event checks of em_play.c start the modes.

   wari28_tbl        a record (CPU_DATA) for each mode from 2 on: the offsets of its tables.
   wari28_rate_MM    the chance, out of 32, that mode MM is started, by the level of play.
   wari28_sel_MM     32 picks; one is taken at random.
   wari28_cmd_MM     the mode's lists: a pick is a number in this table.
   wari28_cmd_MM_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name
                    a list of lib28.c. */
#include "player.h"
#include "pl28_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const wari28_cmd_02[], *const wari28_cmd_03[], *const wari28_cmd_05[],
	*const wari28_cmd_06[], *const wari28_cmd_07[], *const wari28_cmd_08[], *const wari28_cmd_09[],
	*const wari28_cmd_10[], *const wari28_cmd_11[], *const wari28_cmd_12[];
extern const Uint8 wari28_rate_02[], wari28_sel_02[], wari28_rate_03[], wari28_rate_04[],
	wari28_sel_03[], wari28_rate_05[], wari28_sel_05[], wari28_rate_06[], wari28_sel_06[],
	wari28_rate_07[], wari28_sel_07[], wari28_rate_08[], wari28_sel_08[], wari28_rate_09[],
	wari28_sel_09[], wari28_rate_10[], wari28_sel_10[], wari28_rate_11[], wari28_sel_11[],
	wari28_rate_12[], wari28_sel_12[], wari28_rate_13[], wari28_cmd_02_00[], wari28_cmd_02_01[],
	wari28_cmd_02_02[], wari28_cmd_02_03[], wari28_cmd_03_00[], wari28_cmd_05_00[],
	wari28_cmd_05_01[], wari28_cmd_05_02[], wari28_cmd_06_00[], wari28_cmd_07_00[],
	wari28_cmd_07_01[], wari28_cmd_07_02[], wari28_cmd_08_00[], wari28_cmd_09_00[],
	wari28_cmd_09_01[], wari28_cmd_09_02[], wari28_cmd_10_00[], wari28_cmd_10_01[],
	wari28_cmd_10_02[], wari28_cmd_10_03[], wari28_cmd_11_00[], wari28_cmd_11_01[],
	wari28_cmd_11_02[], wari28_cmd_12_00[], wari28_cmd_12_01[], wari28_cmd_12_02[],
	wari28_cmd_12_03[], wari28_cmd_12_04[];

const CPU_DATA wari28_tbl[] = {
	{ ( Sint32 )wari28_rate_02, ( Sint32 )wari28_sel_02, ( Sint32 )wari28_cmd_02 },	/* 2: a throw would catch the opponent */
	{ ( Sint32 )wari28_rate_03, ( Sint32 )wari28_sel_03, ( Sint32 )wari28_cmd_03 },	/* 3: it guards a hit (the chance is the program's own) */
	{ ( Sint32 )wari28_rate_04, ( Sint32 )wari28_sel_03, ( Sint32 )wari28_cmd_03 },	/* 4: never started: a guard in the air is mode 3 */
	{ ( Sint32 )wari28_rate_05, ( Sint32 )wari28_sel_05, ( Sint32 )wari28_cmd_05 },	/* 5: after it has guarded on the ground */
	{ ( Sint32 )wari28_rate_06, ( Sint32 )wari28_sel_06, ( Sint32 )wari28_cmd_06 },	/* 6: after it has guarded in the air */
	{ ( Sint32 )wari28_rate_07, ( Sint32 )wari28_sel_07, ( Sint32 )wari28_cmd_07 },	/* 7: after it has been hit */
	{ ( Sint32 )wari28_rate_08, ( Sint32 )wari28_sel_08, ( Sint32 )wari28_cmd_08 },	/* 8: never started */
	{ ( Sint32 )wari28_rate_09, ( Sint32 )wari28_sel_09, ( Sint32 )wari28_cmd_09 },	/* 9: after it has got up */
	{ ( Sint32 )wari28_rate_10, ( Sint32 )wari28_sel_10, ( Sint32 )wari28_cmd_10 },	/* 10: the opponent dashes */
	{ ( Sint32 )wari28_rate_11, ( Sint32 )wari28_sel_11, ( Sint32 )wari28_cmd_11 },	/* 11: the opponent flies or super jumps */
	{ ( Sint32 )wari28_rate_12, ( Sint32 )wari28_sel_12, ( Sint32 )wari28_cmd_12 },	/* 12: the opponent jumps */
	{ ( Sint32 )wari28_rate_13, 0, 0 },	/* 13: an attack is coming: the chance alone, the lists are yok28.c's */
};

const Uint8 wari28_rate_02[] = {
	  4,   5,   6,   7,   9,  11,  11,  13,  13,  15,  15,  17,  17,  19,  19,  21,
	 21,  23,  23,  24,  24,  26,  26,  28,  28,  30,  30,  31,  31,  31,  31,  31,
};

const Uint8 wari28_sel_02[] = {
	  0,   0,   0,   1,   1,   1,   2,   2,   0,   0,   0,   1,   1,   1,   2,   2,
	  0,   0,   0,   1,   1,   1,   2,   3,   0,   0,   0,   1,   1,   1,   2,   3,
};

const Uint8 *const wari28_cmd_02[] = {
	wari28_cmd_02_00, wari28_cmd_02_01, wari28_cmd_02_02, wari28_cmd_02_03,
};

const Uint8 wari28_rate_03[] = {
	  1,   1,   1,   1,   1,   2,   3,   4,   4,   4,   5,   5,   6,   6,   7,   7,
	  8,   8,  10,  10,  11,  11,  12,  12,  14,  14,  15,  15,  15,  16,  16,  16,
};

const Uint8 wari28_rate_04[] = {
	 12,  12,  13,  13,  14,  15,  16,  17,  18,  19,  20,  21,  21,  21,  21,  21,
	 21,  21,  21,  21,  21,  21,  21,  21,  21,  21,  21,  21,  21,  21,  21,  21,
};

const Uint8 wari28_sel_03[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const wari28_cmd_03[] = {
	wari28_cmd_03_00,
};

const Uint8 wari28_rate_05[] = {
	  4,   4,   5,   5,   6,   6,   7,   7,   8,   8,   9,   9,  10,  10,  11,  11,
	 12,  12,  13,  13,  14,  14,  15,  15,  16,  16,  17,  17,  18,  19,  20,  21,
};

const Uint8 wari28_sel_05[] = {
	  0,   0,   0,   1,   1,   1,   2,   2,   0,   0,   0,   1,   1,   1,   2,   2,
	  0,   0,   0,   1,   1,   1,   2,   2,   0,   0,   0,   1,   1,   1,   2,   2,
};

const Uint8 *const wari28_cmd_05[] = {
	wari28_cmd_05_00, wari28_cmd_05_01, wari28_cmd_05_02,
};

const Uint8 wari28_rate_06[] = {
	  4,   4,   4,   8,  12,  16,  18,  20,  22,  24,  26,  28,  30,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari28_sel_06[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const wari28_cmd_06[] = {
	wari28_cmd_06_00,
};

const Uint8 wari28_rate_07[] = {
	 12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,
	 12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  16,  16,
};

const Uint8 wari28_sel_07[] = {
	  0,   0,   0,   1,   1,   1,   2,   2,   0,   0,   0,   1,   1,   1,   2,   2,
	  0,   0,   0,   1,   1,   1,   2,   2,   0,   0,   0,   1,   1,   1,   2,   2,
};

const Uint8 *const wari28_cmd_07[] = {
	wari28_cmd_07_00, wari28_cmd_07_01, wari28_cmd_07_02,
};

const Uint8 wari28_rate_08[] = {
	  4,   4,   4,   8,  12,  16,  18,  20,  22,  24,  26,  28,  30,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari28_sel_08[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const wari28_cmd_08[] = {
	wari28_cmd_08_00,
};

const Uint8 wari28_rate_09[] = {
	 24,  24,  24,  25,  25,  25,  26,  26,  26,  27,  27,  28,  28,  29,  29,  30,
	 30,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari28_sel_09[] = {
	  0,   0,   0,   0,   1,   1,   1,   1,   0,   0,   0,   1,   1,   1,   1,   1,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,
};

const Uint8 *const wari28_cmd_09[] = {
	wari28_cmd_09_00, wari28_cmd_09_01, wari28_cmd_09_02,
};

const Uint8 wari28_rate_10[] = {
	  5,   6,   7,   8,   8,   8,   8,   8,   9,  10,  11,  12,  12,  12,  12,  12,
	 13,  13,  13,  13,  14,  14,  14,  14,  16,  18,  20,  22,  24,  26,  28,  30,
};

const Uint8 wari28_sel_10[] = {
	  0,   0,   0,   0,   0,   0,   3,   3,   0,   0,   0,   0,   0,   0,   0,   3,
	  1,   1,   1,   1,   1,   1,   1,   3,   2,   2,   2,   2,   2,   2,   2,   3,
};

const Uint8 *const wari28_cmd_10[] = {
	wari28_cmd_10_00, wari28_cmd_10_01, wari28_cmd_10_02, wari28_cmd_10_03,
};

const Uint8 wari28_rate_11[] = {
	  8,   8,   8,   9,   9,   9,  10,  10,  10,  11,  11,  11,  12,  12,  12,  13,
	 13,  13,  14,  14,  14,  15,  15,  15,  16,  16,  16,  17,  17,  17,  18,  18,
};

const Uint8 wari28_sel_11[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   0,   0,   0,   0,
	  1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
};

const Uint8 *const wari28_cmd_11[] = {
	wari28_cmd_11_00, wari28_cmd_11_01, wari28_cmd_11_02,
};

const Uint8 wari28_rate_12[] = {
	  8,   8,   8,   9,   9,   9,  10,  10,  10,  11,  11,  11,  12,  12,  12,  13,
	 13,  13,  14,  14,  14,  15,  15,  15,  16,  16,  16,  17,  17,  17,  18,  18,
};

const Uint8 wari28_sel_12[] = {
	  0,   0,   0,   0,   3,   3,   4,   4,   0,   0,   0,   0,   3,   3,   4,   4,
	  1,   1,   1,   1,   3,   3,   4,   4,   2,   2,   2,   2,   3,   3,   4,   4,
};

const Uint8 *const wari28_cmd_12[] = {
	wari28_cmd_12_00, wari28_cmd_12_01, wari28_cmd_12_02, wari28_cmd_12_03, wari28_cmd_12_04,
};

const Uint8 wari28_rate_13[] = {
	  8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,
	 24,  25,  26,  27,  28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	  2,   2,   2,   2,   2,   2,   2,   2,   4,   4,   4,   6,   6,   8,   8,   8,
	 10,  10,  10,  11,  11,  11,  12,  12,  12,  13,  13,  13,  14,  14,  14,  16,
	  2,   2,   2,   2,   2,   2,   2,   2,   4,   4,   4,   4,   4,   4,   4,   4,
	  6,   6,   6,   6,   6,   6,   6,   6,   8,   8,   8,  10,  10,  10,  12,  12,
	  3,   3,   4,   4,   5,   5,   6,   6,   7,   7,   8,   8,   9,   9,  10,  10,
	 11,  11,  12,  12,  13,  13,  14,  14,  15,  15,  15,  15,  18,  18,  18,  20,
};

const Uint8 wari28_cmd_02_00[] = {
	IF_LEVEL( 30 ),
		IF_RND( 20 ),
			CMD_JUMP( 36 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 23 ),
			IF_RND( 16 ),
				CMD_JUMP( 36 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 12 ),
				IF_RND( 12 ),
					CMD_JUMP( 36 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 8 ),
					IF_RND( 8 ),
						CMD_JUMP( 36 ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_RND( 4 ),
						CMD_JUMP( 36 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari28_cmd_02_01[] = {
	IF_LEVEL( 30 ),
		IF_RND( 24 ),
			CMD_JUMP( 37 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 23 ),
			IF_RND( 18 ),
				CMD_JUMP( 37 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 16 ),
				IF_RND( 12 ),
					CMD_JUMP( 37 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 10 ),
					IF_RND( 10 ),
						CMD_JUMP( 37 ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_LEVEL( 5 ),
						IF_RND( 8 ),
							CMD_JUMP( 37 ),
						CMD_ENDIF,
					CMD_ELSE,
						IF_RND( 4 ),
							CMD_JUMP( 37 ),
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari28_cmd_02_02[] = {
	IF_LEVEL( 31 ),
		IF_RND( 10 ),
			SP_CMD( pl28_sp_slash2, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			IF_RND( 16 ),
				SP_CMD( pl28_sp_slash, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 27 ),
			IF_RND( 8 ),
				SP_CMD( pl28_sp_slash2, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				IF_RND( 10 ),
					SP_CMD( pl28_sp_slash, 0, PK_PUNCH, LV_HEAVY, 0 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 12 ),
				IF_RND( 8 ),
					SP_CMD( pl28_sp_slash2, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ELSE,
					IF_RND( 10 ),
						SP_CMD( pl28_sp_slash, 0, PK_PUNCH, LV_HEAVY, 0 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 8 ),
					IF_RND( 8 ),
						SP_CMD( pl28_sp_slash2, 0, PK_PUNCH, LV_LIGHT, 0 ),
					CMD_ELSE,
						IF_RND( 8 ),
							SP_CMD( pl28_sp_slash, 0, PK_PUNCH, LV_HEAVY, 0 ),
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ELSE,
					IF_RND( 4 ),
						SP_CMD( pl28_sp_slash2, 0, PK_PUNCH, LV_LIGHT, 0 ),
					CMD_ELSE,
						IF_RND( 4 ),
							SP_CMD( pl28_sp_slash, 0, PK_PUNCH, LV_HEAVY, 0 ),
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari28_cmd_02_03[] = {
	IF_RND( 26 ),
		IF_EM_ATCK( ATCK_CROUCH_HK ),
			CMD_JUMP( 5 ),
		CMD_ELSE,
			WAIT( 8 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari28_cmd_03_00[] = {
	CMD_JUMP( 38 ),
	CMD_JUMP( 39 ),
	WAIT( 3 ),
	STAND,
	CMD_EXIT,
};

const Uint8 wari28_cmd_05_00[] = {
	CMD_JUMP( 38 ),
	CMD_JUMP( 39 ),
	WAIT( 3 ),
	STAND,
	CMD_EXIT,
};

const Uint8 wari28_cmd_05_01[] = {
	CMD_JUMP( 38 ),
	CMD_JUMP( 40 ),
	WAIT( 3 ),
	STAND,
	CMD_EXIT,
};

const Uint8 wari28_cmd_05_02[] = {
	CMD_JUMP( 38 ),
	IF_LEVEL( 28 ),
		IF_RND( 4 ),
			SP_CMD( pl28_sp_slash2, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			CMD_JUMP( 39 ),
		CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 39 ),
	CMD_ENDIF,
	WAIT( 3 ),
	CMD_EXIT,
};

const Uint8 wari28_cmd_06_00[] = {
	CMD_JUMP( 38 ),
	TIMER2_SET( 60 ),
	WAIT( 3 ),
	STAND,
	CMD_EXIT,
};

const Uint8 wari28_cmd_07_00[] = {
	CMD_JUMP( 38 ),
	VCOUNTER_SET( 8 ),
	CMD_JUMP( 39 ),
	WAIT( 3 ),
	STAND,
	CMD_EXIT,
};

const Uint8 wari28_cmd_07_01[] = {
	CMD_JUMP( 38 ),
	VCOUNTER_SET( 8 ),
	IF_RND( 16 ),
		IF_HEIGHT( 0, 48 ),
			WAIT( 10 ),
			SP_CMD( pl28_sp_slash2, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			CMD_JUMP( 39 ),
		CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 40 ),
	CMD_ENDIF,
	WAIT( 3 ),
	CMD_EXIT,
};

const Uint8 wari28_cmd_07_02[] = {
	CMD_JUMP( 38 ),
	VCOUNTER_SET( 8 ),
	CMD_JUMP( 41 ),
	WAIT( 3 ),
	STAND,
	CMD_EXIT,
};

const Uint8 wari28_cmd_08_00[] = {
	CMD_JUMP( 38 ),
	CMD_JUMP( 40 ),
	WAIT( 3 ),
	CMD_EXIT,
};

const Uint8 wari28_cmd_09_00[] = {
	CMD_JUMP( 38 ),
	CMD_JUMP( 39 ),
	WAIT( 3 ),
	CMD_EXIT,
};

const Uint8 wari28_cmd_09_01[] = {
	WAIT2( 8 ),
	IF_ATCK_COME,
		CMD_JUMP( 39 ),
	CMD_ELSE,
		JUMP( DIR_BACK ),
		CMD_JUMP( 52 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari28_cmd_09_02[] = {
	WAIT( 4 ),
	CMD_JUMP( 28 ),
	STAND,
	WAIT( 4 ),
	CMD_EXIT,
};

const Uint8 wari28_cmd_10_00[] = {
	VCOUNTER_SET( 8 ),
	CMD_JUMP( 39 ),
	CMD_EXIT,
};

const Uint8 wari28_cmd_10_01[] = {
	JUMP( DIR_UP ),
	JUMP_FLAG_SET,
		TIMER_SET( 40 ),
		IF_HEIGHT( 1, 116 ),
			KICK( LV_HEAVY ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari28_cmd_10_02[] = {
	VCOUNTER_SET( 8 ),
	JUMP( DIR_BACK ),
	JUMP_FLAG_SET,
		TIMER_SET( 40 ),
		IF_HEIGHT( 1, 116 ),
			KICK( LV_HEAVY ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	CMD_JUMP( 39 ),
	WAIT( 3 ),
	CMD_EXIT,
};

const Uint8 wari28_cmd_10_03[] = {
	WAIT( 8 ),
	IF_RND( 16 ),
		SP_CMD( pl28_sp_slash, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ELSE,
		SP_CMD( pl28_sp_slash, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari28_cmd_11_00[] = {
	WAIT( 20 ),
	VCOUNTER_SET( 8 ),
	WAIT2( 14 ),
	SJUMP( DIR_UP ),
	SJUMP_FLAG_SET,
		JUMP_LEVEL( 53, 7, 54, 13, 55, 21, 56 ),
		TIMER_SET( 80 ),
		CMD_JUMP( 36 ),
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari28_cmd_11_01[] = {
	VCOUNTER_SET( 8 ),
	WAIT( 20 ),
	IF_LEVEL( 26 ),
		SJUMP( DIR_FORWARD ),
		SJUMP_FLAG_SET,
			JUMP_LEVEL( 53, 7, 54, 13, 55, 21, 56 ),
			TIMER_SET( 120 ),
			IF_EM_ATCK( ATCK_JUMP_LK ),
				CANCEL_SET2,
					PUNCH( LV_LIGHT ),
					KICK( LV_LIGHT ),
					PUNCH( LV_LIGHT ),
					KICK( LV_LIGHT ),
					CANCEL_CLR,
				CMD_ENDIF,
				SP_CMD( pl28_sp_jcard, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		SP_CMD( pl28_sp_jcard, 0, PK_PUNCH, LV_LIGHT, 0 ),
		IF_RND( 16 ),
			SP_CMD( pl28_sp_jcard, 0, PK_PUNCH, LV_LIGHT, 0 ),
			IF_RND( 16 ),
				SP_CMD( pl28_sp_jcard, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari28_cmd_11_02[] = {
	IF_LEVEL( 16 ),
		TIMER_SET( 80 ),
		IF_EM_ATCK( pl28_at_card_h ),
			SP_CMD( pl28_sp_slash2, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ELSE,
		SP_CMD( pl28_sp_trick, 0, PK_PUNCH, LV_LIGHT, 0 ),
		WAIT2( 6 ),
		SP_CMD( pl28_sp_trick, 0, PK_PUNCH, LV_LIGHT, 0 ),
		WAIT2( 6 ),
		SP_CMD( pl28_sp_trick, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari28_cmd_12_00[] = {
	VCOUNTER_SET( 8 ),
	WAIT( 10 ),
	WAIT2( 14 ),
	SJUMP( DIR_UP ),
	SJUMP_FLAG_SET,
		JUMP_LEVEL( 53, 7, 54, 13, 55, 21, 56 ),
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	CMD_EXIT,
};

const Uint8 wari28_cmd_12_01[] = {
	VCOUNTER_SET( 8 ),
	WAIT2( 14 ),
	SJUMP( DIR_FORWARD ),
	SJUMP_FLAG_SET,
		JUMP_LEVEL( 53, 7, 54, 13, 55, 21, 56 ),
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari28_cmd_12_02[] = {
	VCOUNTER_SET( 8 ),
	WAIT2( 12 ),
	DASH_B( 256 ),
	STAND,
	CMD_JUMP( 41 ),
	CMD_EXIT,
};

const Uint8 wari28_cmd_12_03[] = {
	CROUCH,
	WAIT( 50 ),
	SP_CMD( pl28_sp_escape, 0, PK_PUNCH, LV_HEAVY, LEVER_BACK ),
	CMD_EXIT,
};

const Uint8 wari28_cmd_12_04[] = {
	CROUCH,
	WAIT( 20 ),
	TIMER_SET( 80 ),
	IF_DIST( 48, 64, 64, 64 ),
		IF_RND( 24 ),
			CANCEL_SET2,
				PUNCH( LV_HEAVY ),
				SJUMP3( DIR_UP ),
				CANCEL_CLR,
			CMD_ENDIF,
			SJUMP_FLAG_SET,
				WAIT( 16 ),
				JUMP_LEVEL( 30, 4, 31, 12, 32, 22, 33 ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			CANCEL_SET,
				STAND,
				PUNCH( LV_LIGHT ),
				PUNCH( LV_HEAVY ),
				IF_RND( 10 ),
					SP_CMD( pl28_sp_card, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ENDIF,
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_RND( 16 ),
			SP_CMD( pl28_sp_slash2, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			SP_CMD( pl28_sp_slash, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

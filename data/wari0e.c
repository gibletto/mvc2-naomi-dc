/* Venom (character 0e): the computer's lists for cutting in.
   What it does when something happens in the fight. PLAYER_WORK's cpu_adrs[2] points here; the
   event checks of em_play.c start the modes.

   wari0e_tbl        a record (CPU_DATA) for each mode from 2 on: the offsets of its tables.
   wari0e_rate_MM    the chance, out of 32, that mode MM is started, by the level of play.
   wari0e_sel_MM     32 picks; one is taken at random.
   wari0e_cmd_MM     the mode's lists: a pick is a number in this table.
   wari0e_cmd_MM_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name
                    a list of lib0e.c. */
#include "player.h"
#include "pl0e_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const wari0e_cmd_02[], *const wari0e_cmd_03[], *const wari0e_cmd_05[],
	*const wari0e_cmd_06[], *const wari0e_cmd_07[], *const wari0e_cmd_08[], *const wari0e_cmd_09[],
	*const wari0e_cmd_10[], *const wari0e_cmd_11[], *const wari0e_cmd_12[];
extern const Uint8 wari0e_rate_02[], wari0e_sel_02[], wari0e_rate_03[], wari0e_rate_04[],
	wari0e_sel_03[], wari0e_rate_05[], wari0e_sel_05[], wari0e_rate_06[], wari0e_sel_06[],
	wari0e_rate_07[], wari0e_sel_07[], wari0e_rate_08[], wari0e_sel_08[], wari0e_rate_09[],
	wari0e_sel_09[], wari0e_rate_10[], wari0e_sel_10[], wari0e_rate_11[], wari0e_sel_11[],
	wari0e_rate_12[], wari0e_sel_12[], wari0e_rate_13[], wari0e_cmd_02_00[], wari0e_cmd_02_01[],
	wari0e_cmd_02_02[], wari0e_cmd_03_00[], wari0e_cmd_05_00[], wari0e_cmd_05_01[],
	wari0e_cmd_05_02[], wari0e_cmd_05_03[], wari0e_cmd_06_00[], wari0e_cmd_06_01[],
	wari0e_cmd_07_00[], wari0e_cmd_07_01[], wari0e_cmd_07_02[], wari0e_cmd_08_00[],
	wari0e_cmd_09_00[], wari0e_cmd_09_01[], wari0e_cmd_09_02[], wari0e_cmd_10_00[],
	wari0e_cmd_10_01[], wari0e_cmd_10_02[], wari0e_cmd_10_03[], wari0e_cmd_11_00[],
	wari0e_cmd_11_01[], wari0e_cmd_11_02[], wari0e_cmd_11_03[], wari0e_cmd_12_00[],
	wari0e_cmd_12_01[], wari0e_cmd_12_02[], wari0e_cmd_12_03[], wari0e_cmd_12_04[],
	wari0e_cmd_12_05[];

const CPU_DATA wari0e_tbl[] = {
	{ ( Sint32 )wari0e_rate_02, ( Sint32 )wari0e_sel_02, ( Sint32 )wari0e_cmd_02 },	/* 2: a throw would catch the opponent */
	{ ( Sint32 )wari0e_rate_03, ( Sint32 )wari0e_sel_03, ( Sint32 )wari0e_cmd_03 },	/* 3: it guards a hit (the chance is the program's own) */
	{ ( Sint32 )wari0e_rate_04, ( Sint32 )wari0e_sel_03, ( Sint32 )wari0e_cmd_03 },	/* 4: never started: a guard in the air is mode 3 */
	{ ( Sint32 )wari0e_rate_05, ( Sint32 )wari0e_sel_05, ( Sint32 )wari0e_cmd_05 },	/* 5: after it has guarded on the ground */
	{ ( Sint32 )wari0e_rate_06, ( Sint32 )wari0e_sel_06, ( Sint32 )wari0e_cmd_06 },	/* 6: after it has guarded in the air */
	{ ( Sint32 )wari0e_rate_07, ( Sint32 )wari0e_sel_07, ( Sint32 )wari0e_cmd_07 },	/* 7: after it has been hit */
	{ ( Sint32 )wari0e_rate_08, ( Sint32 )wari0e_sel_08, ( Sint32 )wari0e_cmd_08 },	/* 8: never started */
	{ ( Sint32 )wari0e_rate_09, ( Sint32 )wari0e_sel_09, ( Sint32 )wari0e_cmd_09 },	/* 9: after it has got up */
	{ ( Sint32 )wari0e_rate_10, ( Sint32 )wari0e_sel_10, ( Sint32 )wari0e_cmd_10 },	/* 10: the opponent dashes */
	{ ( Sint32 )wari0e_rate_11, ( Sint32 )wari0e_sel_11, ( Sint32 )wari0e_cmd_11 },	/* 11: the opponent flies or super jumps */
	{ ( Sint32 )wari0e_rate_12, ( Sint32 )wari0e_sel_12, ( Sint32 )wari0e_cmd_12 },	/* 12: the opponent jumps */
	{ ( Sint32 )wari0e_rate_13, 0, 0 },	/* 13: an attack is coming: the chance alone, the lists are yok0e.c's */
};

const Uint8 wari0e_rate_02[] = {
	  5,   6,   7,   8,   9,  10,  12,  14,  17,  20,  23,  25,  27,  25,  23,  21,
	 21,  23,  23,  24,  24,  26,  26,  28,  28,  30,  30,  31,  31,  31,  31,  31,
};

const Uint8 wari0e_sel_02[] = {
	  0,   0,   0,   0,   0,   0,   0,   2,   0,   0,   0,   0,   0,   0,   2,   2,
	  1,   1,   1,   1,   1,   1,   1,   2,   1,   1,   1,   1,   1,   1,   2,   2,
};

const Uint8 *const wari0e_cmd_02[] = {
	wari0e_cmd_02_00, wari0e_cmd_02_01, wari0e_cmd_02_02,
};

const Uint8 wari0e_rate_03[] = {
	  1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   3,   3,   3,
};

const Uint8 wari0e_rate_04[] = {
	 16,  16,  17,  17,  18,  19,  20,  21,  22,  23,  24,  25,  25,  25,  25,  25,
	 25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,
};

const Uint8 wari0e_sel_03[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const wari0e_cmd_03[] = {
	wari0e_cmd_03_00,
};

const Uint8 wari0e_rate_05[] = {
	  4,   4,   4,   4,   8,   8,  10,  10,  12,  12,  14,  14,  16,  16,  16,  16,
	 16,  16,  16,  16,  16,  16,  16,  16,  18,  18,  20,  20,  20,  22,  22,  24,
};

const Uint8 wari0e_sel_05[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,
	  2,   2,   2,   2,   2,   2,   2,   2,   1,   1,   2,   2,   3,   3,   3,   3,
};

const Uint8 *const wari0e_cmd_05[] = {
	wari0e_cmd_05_00, wari0e_cmd_05_01, wari0e_cmd_05_02, wari0e_cmd_05_03,
};

const Uint8 wari0e_rate_06[] = {
	  4,   4,   4,   8,  12,  16,  18,  20,  22,  24,  26,  28,  30,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari0e_sel_06[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,
};

const Uint8 *const wari0e_cmd_06[] = {
	wari0e_cmd_06_00, wari0e_cmd_06_01,
};

const Uint8 wari0e_rate_07[] = {
	  4,   4,   4,   8,  12,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,
	 16,  16,  16,  17,  17,  17,  17,  17,  18,  18,  18,  18,  18,  18,  20,  20,
};

const Uint8 wari0e_sel_07[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,
	  2,   2,   2,   2,   2,   2,   2,   2,   1,   1,   2,   2,   0,   0,   0,   0,
};

const Uint8 *const wari0e_cmd_07[] = {
	wari0e_cmd_07_00, wari0e_cmd_07_01, wari0e_cmd_07_02,
};

const Uint8 wari0e_rate_08[] = {
	  4,   4,   4,   8,  12,  16,  18,  20,  22,  24,  26,  28,  30,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari0e_sel_08[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const wari0e_cmd_08[] = {
	wari0e_cmd_08_00,
};

const Uint8 wari0e_rate_09[] = {
	  6,   6,   8,   8,   8,  10,  12,  14,  16,  18,  20,  22,  24,  25,  26,  27,
	 28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari0e_sel_09[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,
};

const Uint8 *const wari0e_cmd_09[] = {
	wari0e_cmd_09_00, wari0e_cmd_09_01, wari0e_cmd_09_02,
};

const Uint8 wari0e_rate_10[] = {
	  4,   5,   6,   7,   8,   9,  10,  12,  14,  16,  18,  20,  21,  21,  20,  20,
	 19,  19,  20,  20,  21,  21,  22,  22,  23,  23,  24,  24,  25,  26,  27,  28,
};

const Uint8 wari0e_sel_10[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,
	  2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   3,   3,   3,
};

const Uint8 *const wari0e_cmd_10[] = {
	wari0e_cmd_10_00, wari0e_cmd_10_01, wari0e_cmd_10_02, wari0e_cmd_10_03,
};

const Uint8 wari0e_rate_11[] = {
	 16,  16,  16,  17,  17,  17,  18,  18,  18,  19,  19,  19,  20,  20,  20,  21,
	 21,  21,  22,  22,  22,  23,  23,  23,  24,  24,  24,  25,  25,  25,  26,  26,
};

const Uint8 wari0e_sel_11[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   2,
	  2,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,   3,   3,   3,   3,
};

const Uint8 *const wari0e_cmd_11[] = {
	wari0e_cmd_11_00, wari0e_cmd_11_01, wari0e_cmd_11_02, wari0e_cmd_11_03,
};

const Uint8 wari0e_rate_12[] = {
	 16,  16,  17,  17,  18,  18,  19,  19,  20,  21,  22,  23,  24,  23,  22,  21,
	 20,  21,  22,  23,  23,  24,  24,  25,  25,  26,  26,  27,  27,  28,  28,  29,
};

const Uint8 wari0e_sel_12[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
	  1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   2,   3,   4,   5,
};

const Uint8 *const wari0e_cmd_12[] = {
	wari0e_cmd_12_00, wari0e_cmd_12_01, wari0e_cmd_12_02, wari0e_cmd_12_03, wari0e_cmd_12_04, wari0e_cmd_12_05,
};

const Uint8 wari0e_rate_13[] = {
	  8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,
	 24,  25,  26,  27,  28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	  1,   2,   3,   4,   5,   6,   7,   8,   9,   9,   9,   9,  10,  10,  10,  10,
	 10,  11,  11,  11,  11,  11,  11,  11,  12,  12,  12,  12,  12,  12,  12,  12,
	  1,   2,   3,   4,   5,   6,   7,   8,   9,   9,   9,   9,  10,  10,  10,  10,
	 10,  11,  11,  11,  11,  11,  11,  11,  12,  12,  12,  12,  12,  12,  12,  12,
	  4,   4,   5,   5,   5,   6,   7,   8,   9,   9,   9,   9,  10,  10,  10,  10,
	 10,  11,  11,  11,  11,  11,  11,  11,  12,  12,  12,  12,  12,  12,  12,  12,
};

const Uint8 wari0e_cmd_02_00[] = {
	IF_CATCH,
		IF_RND( 8 ),
			NAG_VFLIP_ADJ( 1, LEVER_FORWARD ),
			CMD_JUMP( 13 ),
		CMD_ELSE,
			CMD_JUMP( 11 ),
		CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 11 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari0e_cmd_02_01[] = {
	IF_CATCH,
		IF_RND( 8 ),
			NAG_VFLIP_ADJ2( 1, LEVER_FORWARD ),
			IF_LEVEL( 24 ),
				CMD_JUMP( 12 ),
			CMD_ELSE,
				IF_LEVEL( 16 ),
					IF_RND( 16 ),
						CMD_JUMP( 12 ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_LEVEL( 4 ),
						IF_RND( 8 ),
							CMD_JUMP( 12 ),
						CMD_ENDIF,
					CMD_ELSE,
						DASH_B( 192 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			CMD_JUMP( 11 ),
		CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 11 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari0e_cmd_02_02[] = {
	WAIT2( 8 ),
	IF_RND( 20 ),
		CMD_JUMP( 14 ),
	CMD_ELSE,
		IF_RND( 24 ),
			IF_GAUGE( 1 ),
				SP_CMD( pl0e_sp_bite, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 wari0e_cmd_03_00[] = {
	IF_RND( 20 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	GUARD_CONT( 16 ),
	CMD_EXIT,
};

const Uint8 wari0e_cmd_05_00[] = {
	IF_ATCK_COME,
		IF_RND( 24 ),
			GUARD_CONT( 16 ),
		CMD_ENDIF,
	CMD_ELSE,
		DASH_B( 192 ),
		WAIT( 16 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari0e_cmd_05_01[] = {
	LOCK_SET,
	IF_CATCH,
		IF_RND( 8 ),
			NAG_VFLIP_ADJ( 1, LEVER_FORWARD ),
			CMD_JUMP( 13 ),
		CMD_ELSE,
			CMD_JUMP( 11 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_RND( 16 ),
			CMD_JUMP( 11 ),
		CMD_ELSE,
			CMD_JUMP( 23 ),
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 wari0e_cmd_05_02[] = {
	IF_EM_ATCK( ATCK_STAND_MP ),
		CANCEL_SET2,
			STAND,
			PUNCH( LV_LIGHT ),
			SJUMP3( DIR_FORWARD ),
			CANCEL_CLR,
		CMD_ENDIF,
		SJUMP_FLAG_SET,
			CMD_JUMP( 6 ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		CROUCH,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari0e_cmd_05_03[] = {
	IF_RND( 16 ),
		CMD_JUMP( 14 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari0e_cmd_06_00[] = {
	CMD_JUMP( 18 ),
	CMD_EXIT,
};

const Uint8 wari0e_cmd_06_01[] = {
	IF_LEVEL( 24 ),
		IF_RND( 8 ),
			SP_CMD( pl0e_sp_fang, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			CMD_JUMP( 18 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 16 ),
			IF_RND( 8 ),
				KICK( LV_HEAVY ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari0e_cmd_07_00[] = {
	WAIT2( 8 ),
	IF_RND( 20 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_JUMP( 29 ),
	CMD_EXIT,
};

const Uint8 wari0e_cmd_07_01[] = {
	IF_RND( 20 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	TIMER2_SET( 60 ),
	CMD_EXIT,
};

const Uint8 wari0e_cmd_07_02[] = {
	IF_LEVEL( 24 ),
		IF_RND( 8 ),
			SP_CMD( pl0e_sp_fang, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			CMD_JUMP( 18 ),
		CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 29 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari0e_cmd_08_00[] = {
	CMD_JUMP( 18 ),
	CMD_EXIT,
};

const Uint8 wari0e_cmd_09_00[] = {
	LOCK_SET,
	CMD_JUMP( 23 ),
	CMD_JUMP( 23 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 wari0e_cmd_09_01[] = {
	LOCK_SET,
	IF_CATCH,
		IF_RND( 8 ),
			NAG_VFLIP_ADJ2( 1, LEVER_FORWARD ),
			IF_LEVEL( 24 ),
				CMD_JUMP( 12 ),
			CMD_ELSE,
				IF_LEVEL( 16 ),
					IF_RND( 12 ),
						CMD_JUMP( 12 ),
					CMD_ENDIF,
					DASH_B( 192 ),
				CMD_ELSE,
					DASH_B( 192 ),
				CMD_ENDIF,
				WAIT( 16 ),
			CMD_ENDIF,
		CMD_ELSE,
			CMD_JUMP( 23 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_RND( 8 ),
			CMD_JUMP( 11 ),
		CMD_ELSE,
			CMD_JUMP( 23 ),
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 wari0e_cmd_09_02[] = {
	IF_DIST( 64, 48, 64, 0 ),
		CANCEL_SET,
			PUNCH( LV_LIGHT ),
			PUNCH( LV_LIGHT ),
			IF_LEVEL( 20 ),
				IF_RND( 12 ),
					SP_CMD( pl0e_sp_webthrow, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 4 ),
					KICK( LV_LIGHT ),
				CMD_ENDIF,
			CMD_ENDIF,
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		IF_DIST( 112, 48, 112, 0 ),
			CANCEL_SET2,
				CROUCH,
				PUNCH( LV_HEAVY ),
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			DASH_B( 192 ),
			WAIT( 16 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari0e_cmd_10_00[] = {
	CMD_JUMP( 14 ),
	TIMER_SET( 2 ),
	IF_DIST( 64, 48, 64, 0 ),
		CANCEL_SET2,
			IF_LEVEL( 24 ),
				CROUCH,
				KICK( LV_HEAVY ),
			CMD_ELSE,
				IF_LEVEL( 16 ),
					CROUCH,
					PUNCH( LV_HEAVY ),
				CMD_ELSE,
					IF_LEVEL( 4 ),
						STAND,
						KICK( LV_LIGHT ),
					CMD_ELSE,
						PUNCH( LV_LIGHT ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		TIMER_SET( 2 ),
		IF_DIST( 104, 48, 104, 0 ),
			IF_RND( 12 ),
				STAND,
				PUNCH( LV_HEAVY ),
			CMD_ELSE,
				IF_RND( 12 ),
					CROUCH,
					PUNCH( LV_HEAVY ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			TIMER_SET( 3 ),
			IF_DIST( 160, 48, 160, 0 ),
				IF_RND( 12 ),
					SP_CMD( pl0e_sp_rush, 0, PK_PUNCH, LV_HEAVY, 0 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari0e_cmd_10_01[] = {
	STAND,
	JUMP( DIR_UP ),
	JUMP_FLAG_SET,
		TIMER_SET( 30 ),
		IF_HEIGHT( 0xff, 64 ),
			TIMER_SET( 1 ),
			IF_DIST( 64, 48, 64, 0 ),
				JUMP_FLAG_CLR,
				SP_CMD( pl0e_sp_fang, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				IF_DIST( 104, 48, 104, 0 ),
					JUMP_FLAG_CLR,
					SP_CMD( pl0e_sp_fang, 0, PK_PUNCH, LV_HEAVY, 0 ),
				CMD_ELSE,
					IF_DIST( 160, 48, 160, 0 ),
						JUMP_FLAG_CLR,
						SP_CMD( pl0e_sp_fang, 0, PK_PUNCH, LV_HEAVY, 0 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	WAIT2( 4 ),
	CMD_EXIT,
};

const Uint8 wari0e_cmd_10_02[] = {
	CMD_JUMP( 18 ),
	CMD_EXIT,
};

const Uint8 wari0e_cmd_10_03[] = {
	WAIT2( 8 ),
	CMD_JUMP( 14 ),
	CMD_JUMP( 23 ),
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 wari0e_cmd_11_00[] = {
	WAIT2( 6 ),
	DASH_F( 192 ),
	WAIT2( 4 ),
	IF_LEVEL( 16 ),
		IF_RND( 24 ),
			TIMER_SET( 10 ),
			IF_DIST( 48, 48, 112, 176 ),
				SP_CMD( pl0e_sp_fang, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari0e_cmd_11_01[] = {
	WAIT2( 6 ),
	CROUCH,
	WAIT2( 6 ),
	TIMER_SET( 8 ),
	IF_DIST( 48, 48, 224, 224 ),
		SP_CMD( pl0e_sp_rush, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ELSE,
		TIMER_SET( 8 ),
		IF_DIST( 96, 80, 320, 16 ),
			SP_CMD( pl0e_sp_rush, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ELSE,
			IF_RND( 22 ),
				CMD_JUMP( 23 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari0e_cmd_11_02[] = {
	WAIT2( 8 ),
	SJUMP( DIR_BACK ),
	SJUMP_FLAG_SET,
		TIMER_SET( 40 ),
		IF_HEIGHT( 0xff, 96 ),
			JUMP_FLAG_CLR,
			TIMER_SET( 2 ),
			IF_DIST( 64, 16, 112, 0 ),
				SP_CMD( pl0e_sp_fang, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				TIMER_SET( 2 ),
				IF_DIST( 104, 16, 152, 0 ),
					SP_CMD( pl0e_sp_fang, 0, PK_PUNCH, LV_HEAVY, 0 ),
				CMD_ELSE,
					TIMER_SET( 2 ),
					IF_DIST( 160, 16, 208, 0 ),
						SP_CMD( pl0e_sp_fang, 0, PK_PUNCH, LV_HEAVY, 0 ),
					CMD_ELSE,
						IF_RND( 22 ),
							CMD_JUMP( 18 ),
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	WAIT2( 4 ),
	CMD_EXIT,
};

const Uint8 wari0e_cmd_11_03[] = {
	WAIT2( 8 ),
	SJUMP( DIR_FORWARD ),
	SJUMP_FLAG_SET,
		TIMER_SET( 80 ),
		IF_DIST( 48, 48, 80, 96 ),
			CANCEL_SET2,
				CMD_JUMP( 6 ),
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			IF_RND( 16 ),
				FLY( LEVER_FORWARD, 192 ),
				IF_RND( 16 ),
					CMD_JUMP( 18 ),
				CMD_ELSE,
					IF_LEVEL( 12 ),
						KICK( LV_HEAVY ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	WAIT2( 4 ),
	CMD_EXIT,
};

const Uint8 wari0e_cmd_12_00[] = {
	CMD_JUMP( 23 ),
	CMD_EXIT,
};

const Uint8 wari0e_cmd_12_01[] = {
	STAND,
	WAIT( 16 ),
	TIMER_SET( 3 ),
	IF_DIST( 48, 32, 64, 128 ),
		IF_LEVEL( 16 ),
			IF_RND( 3 ),
				SP_CMD( pl0e_sp_fang, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				KICK( LV_HEAVY ),
			CMD_ENDIF,
		CMD_ELSE,
			KICK( LV_HEAVY ),
		CMD_ENDIF,
	CMD_ELSE,
		TIMER_SET( 3 ),
		IF_DIST( 48, 32, 176, 128 ),
			CROUCH,
			PUNCH( LV_HEAVY ),
		CMD_ELSE,
			TIMER_SET( 3 ),
			IF_DIST( 48, 32, 96, 160 ),
				CANCEL_SET2,
					IF_LEVEL( 4 ),
						PUNCH( LV_LIGHT ),
						SJUMP3( DIR_FORWARD ),
						SJUMP_FLAG_SET,
							CMD_JUMP( 6 ),
							JUMP_FLAG_CLR,
						CMD_ENDIF,
					CMD_ELSE,
						PUNCH( LV_LIGHT ),
						DASH_B( 192 ),
						WAIT( 16 ),
					CMD_ENDIF,
					CANCEL_CLR,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari0e_cmd_12_02[] = {
	WAIT( 10 ),
	WAIT2( 6 ),
	DASH_F( 192 ),
	WAIT2( 6 ),
	TIMER_SET( 3 ),
	IF_DIST( 48, 32, 96, 160 ),
		CANCEL_SET2,
			IF_LEVEL( 4 ),
				PUNCH( LV_LIGHT ),
				SJUMP3( DIR_FORWARD ),
				SJUMP_FLAG_SET,
					CMD_JUMP( 6 ),
					JUMP_FLAG_CLR,
				CMD_ENDIF,
			CMD_ELSE,
				PUNCH( LV_LIGHT ),
			CMD_ENDIF,
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	WAIT2( 14 ),
	CMD_EXIT,
};

const Uint8 wari0e_cmd_12_03[] = {
	WAIT( 10 ),
	WAIT2( 4 ),
	JUMP( DIR_BACK ),
	JUMP_FLAG_SET,
		TIMER_SET( 30 ),
		IF_HEIGHT( 0xff, 32 ),
			TIMER_SET( 50 ),
			IF_DIST( 48, 32, 80, 48 ),
				IF_RND( 8 ),
					JUMP_FLAG_CLR,
					SP_CMD( pl0e_sp_fang, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ELSE,
					PUNCH( LV_HEAVY ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	WAIT2( 4 ),
	CMD_EXIT,
};

const Uint8 wari0e_cmd_12_04[] = {
	WAIT2( 4 ),
	SJUMP( DIR_BACK ),
	SJUMP_FLAG_SET,
		TIMER_SET( 40 ),
		IF_HEIGHT( 0xff, 128 ),
			JUMP_FLAG_CLR,
			FLY( LEVER_FORWARD, 128 ),
			IF_RND( 16 ),
				SP_CMD( pl0e_sp_fang, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				IF_RND( 16 ),
					SP_CMD( pl0e_sp_fang, 0, PK_PUNCH, LV_HEAVY, 0 ),
				CMD_ELSE,
					SP_CMD( pl0e_sp_fang, 0, PK_PUNCH, LV_HEAVY, 0 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	WAIT2( 4 ),
	CMD_EXIT,
};

const Uint8 wari0e_cmd_12_05[] = {
	WAIT2( 8 ),
	CMD_JUMP( 14 ),
	WAIT2( 2 ),
	CMD_EXIT,
};

/* Sabretooth (character 2b): the computer's lists for cutting in.
   What it does when something happens in the fight. PLAYER_WORK's cpu_adrs[2] points here; the
   event checks of em_play.c start the modes.

   wari2b_tbl        a record (CPU_DATA) for each mode from 2 on: the offsets of its tables.
   wari2b_rate_MM    the chance, out of 32, that mode MM is started, by the level of play.
   wari2b_sel_MM     32 picks; one is taken at random.
   wari2b_cmd_MM     the mode's lists: a pick is a number in this table.
   wari2b_cmd_MM_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name
                    a list of lib2b.c. */
#include "player.h"
#include "pl2b_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const wari2b_cmd_02[], *const wari2b_cmd_03[], *const wari2b_cmd_05[],
	*const wari2b_cmd_06[], *const wari2b_cmd_07[], *const wari2b_cmd_08[], *const wari2b_cmd_09[],
	*const wari2b_cmd_10[], *const wari2b_cmd_11[], *const wari2b_cmd_12[];
extern const Uint8 wari2b_rate_02[], wari2b_sel_02[], wari2b_rate_03[], wari2b_rate_04[],
	wari2b_sel_03[], wari2b_rate_05[], wari2b_sel_05[], wari2b_rate_06[], wari2b_sel_06[],
	wari2b_rate_07[], wari2b_sel_07[], wari2b_rate_08[], wari2b_sel_08[], wari2b_rate_09[],
	wari2b_sel_09[], wari2b_rate_10[], wari2b_sel_10[], wari2b_rate_11[], wari2b_sel_11[],
	wari2b_rate_12[], wari2b_sel_12[], wari2b_rate_13[], wari2b_cmd_02_00[], wari2b_cmd_02_01[],
	wari2b_cmd_02_02[], wari2b_cmd_02_03[], wari2b_cmd_03_00[], wari2b_cmd_05_00[],
	wari2b_cmd_05_01[], wari2b_cmd_05_02[], wari2b_cmd_05_03[], wari2b_cmd_06_00[],
	wari2b_cmd_06_01[], wari2b_cmd_06_02[], wari2b_cmd_07_00[], wari2b_cmd_07_01[],
	wari2b_cmd_07_02[], wari2b_cmd_08_00[], wari2b_cmd_08_01[], wari2b_cmd_09_00[],
	wari2b_cmd_09_01[], wari2b_cmd_09_02[], wari2b_cmd_10_00[], wari2b_cmd_10_01[],
	wari2b_cmd_10_02[], wari2b_cmd_11_00[], wari2b_cmd_11_01[], wari2b_cmd_11_02[],
	wari2b_cmd_12_00[], wari2b_cmd_12_01[], wari2b_cmd_12_02[], wari2b_cmd_12_03[];

const CPU_DATA wari2b_tbl[] = {
	{ ( Sint32 )wari2b_rate_02, ( Sint32 )wari2b_sel_02, ( Sint32 )wari2b_cmd_02 },	/* 2: a throw would catch the opponent */
	{ ( Sint32 )wari2b_rate_03, ( Sint32 )wari2b_sel_03, ( Sint32 )wari2b_cmd_03 },	/* 3: it guards a hit (the chance is the program's own) */
	{ ( Sint32 )wari2b_rate_04, ( Sint32 )wari2b_sel_03, ( Sint32 )wari2b_cmd_03 },	/* 4: never started: a guard in the air is mode 3 */
	{ ( Sint32 )wari2b_rate_05, ( Sint32 )wari2b_sel_05, ( Sint32 )wari2b_cmd_05 },	/* 5: after it has guarded on the ground */
	{ ( Sint32 )wari2b_rate_06, ( Sint32 )wari2b_sel_06, ( Sint32 )wari2b_cmd_06 },	/* 6: after it has guarded in the air */
	{ ( Sint32 )wari2b_rate_07, ( Sint32 )wari2b_sel_07, ( Sint32 )wari2b_cmd_07 },	/* 7: after it has been hit */
	{ ( Sint32 )wari2b_rate_08, ( Sint32 )wari2b_sel_08, ( Sint32 )wari2b_cmd_08 },	/* 8: never started */
	{ ( Sint32 )wari2b_rate_09, ( Sint32 )wari2b_sel_09, ( Sint32 )wari2b_cmd_09 },	/* 9: after it has got up */
	{ ( Sint32 )wari2b_rate_10, ( Sint32 )wari2b_sel_10, ( Sint32 )wari2b_cmd_10 },	/* 10: the opponent dashes */
	{ ( Sint32 )wari2b_rate_11, ( Sint32 )wari2b_sel_11, ( Sint32 )wari2b_cmd_11 },	/* 11: the opponent flies or super jumps */
	{ ( Sint32 )wari2b_rate_12, ( Sint32 )wari2b_sel_12, ( Sint32 )wari2b_cmd_12 },	/* 12: the opponent jumps */
	{ ( Sint32 )wari2b_rate_13, 0, 0 },	/* 13: an attack is coming: the chance alone, the lists are yok2b.c's */
};

const Uint8 wari2b_rate_02[] = {
	  4,   5,   6,   7,   9,  11,  11,  13,  13,  15,  15,  17,  17,  19,  19,  21,
	 21,  23,  23,  24,  24,  26,  26,  28,  28,  30,  30,  31,  31,  31,  31,  31,
};

const Uint8 wari2b_sel_02[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   3,   3,   3,   0,   1,
};

const Uint8 *const wari2b_cmd_02[] = {
	wari2b_cmd_02_00, wari2b_cmd_02_01, wari2b_cmd_02_02, wari2b_cmd_02_03,
};

const Uint8 wari2b_rate_03[] = {
	  3,   5,   7,   9,  10,  11,  12,  12,  12,  13,  13,  13,  14,  14,  14,  15,
	 15,  15,  16,  16,  16,  17,  17,  17,  18,  18,  19,  19,  20,  21,  22,  23,
};

const Uint8 wari2b_rate_04[] = {
	 16,  16,  17,  17,  18,  19,  20,  21,  22,  23,  24,  25,  25,  25,  25,  25,
	 25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,
};

const Uint8 wari2b_sel_03[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const wari2b_cmd_03[] = {
	wari2b_cmd_03_00,
};

const Uint8 wari2b_rate_05[] = {
	 10,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  22,  22,  23,  23,
	 24,  24,  25,  25,  26,  26,  27,  27,  28,  28,  29,  29,  30,  30,  31,  31,
};

const Uint8 wari2b_sel_05[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   2,   2,   2,   2,   2,   2,   2,   2,
	  3,   3,   3,   3,   3,   3,   3,   3,   1,   1,   1,   1,   1,   1,   1,   1,
};

const Uint8 *const wari2b_cmd_05[] = {
	wari2b_cmd_05_00, wari2b_cmd_05_01, wari2b_cmd_05_02, wari2b_cmd_05_03,
};

const Uint8 wari2b_rate_06[] = {
	  4,   4,   4,   8,  12,  16,  18,  20,  22,  24,  26,  28,  30,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari2b_sel_06[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  2,   2,   2,   2,   2,   2,   2,   2,   1,   1,   1,   1,   1,   1,   1,   1,
};

const Uint8 *const wari2b_cmd_06[] = {
	wari2b_cmd_06_00, wari2b_cmd_06_01, wari2b_cmd_06_02,
};

const Uint8 wari2b_rate_07[] = {
	  4,   4,   4,   8,  12,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,
	 16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,
};

const Uint8 wari2b_sel_07[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
};

const Uint8 *const wari2b_cmd_07[] = {
	wari2b_cmd_07_00, wari2b_cmd_07_01, wari2b_cmd_07_02,
};

const Uint8 wari2b_rate_08[] = {
	  4,   4,   4,   8,  12,  16,  18,  20,  22,  24,  26,  28,  30,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari2b_sel_08[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
};

const Uint8 *const wari2b_cmd_08[] = {
	wari2b_cmd_08_00, wari2b_cmd_08_01,
};

const Uint8 wari2b_rate_09[] = {
	 20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  31,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari2b_sel_09[] = {
	  0,   0,   0,   0,   0,   0,   2,   2,   0,   0,   0,   0,   0,   0,   2,   2,
	  1,   1,   1,   1,   1,   1,   2,   2,   1,   1,   1,   1,   1,   1,   2,   2,
};

const Uint8 *const wari2b_cmd_09[] = {
	wari2b_cmd_09_00, wari2b_cmd_09_01, wari2b_cmd_09_02,
};

const Uint8 wari2b_rate_10[] = {
	  1,   2,   3,   4,   5,   6,   7,   7,   7,   8,   8,   8,   9,   9,   9,  10,
	 10,  10,  11,  12,  13,  14,  15,  16,  16,  16,  16,  16,  16,  16,  16,  16,
};

const Uint8 wari2b_sel_10[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,
};

const Uint8 *const wari2b_cmd_10[] = {
	wari2b_cmd_10_00, wari2b_cmd_10_01, wari2b_cmd_10_02,
};

const Uint8 wari2b_rate_11[] = {
	  8,   9,  10,  10,  11,  11,  12,  12,  13,  13,  13,  14,  14,  14,  15,  15,
	 15,  16,  16,  16,  16,  17,  17,  17,  17,  18,  18,  18,  18,  18,  19,  19,
};

const Uint8 wari2b_sel_11[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,
};

const Uint8 *const wari2b_cmd_11[] = {
	wari2b_cmd_11_00, wari2b_cmd_11_01, wari2b_cmd_11_02,
};

const Uint8 wari2b_rate_12[] = {
	  8,   8,   8,  12,  12,  12,  13,  13,  13,  14,  14,  14,  15,  15,  15,  16,
	 16,  16,  16,  17,  17,  17,  17,  18,  18,  18,  19,  19,  19,  19,  19,  19,
};

const Uint8 wari2b_sel_12[] = {
	  0,   0,   1,   1,   2,   3,   0,   1,   0,   0,   1,   1,   2,   3,   0,   1,
	  0,   0,   1,   1,   3,   3,   0,   1,   0,   0,   1,   1,   3,   3,   0,   1,
};

const Uint8 *const wari2b_cmd_12[] = {
	wari2b_cmd_12_00, wari2b_cmd_12_01, wari2b_cmd_12_02, wari2b_cmd_12_03,
};

const Uint8 wari2b_rate_13[] = {
	  8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,
	 24,  25,  26,  27,  28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	  1,   1,   1,   1,   2,   2,   2,   2,   3,   3,   3,   3,   4,   4,   4,   4,
	  5,   5,   5,   5,   6,   6,   6,   6,   7,   7,   7,   7,   8,   8,   8,   8,
	  1,   1,   1,   1,   2,   2,   2,   2,   3,   3,   3,   3,   4,   4,   4,   4,
	  5,   5,   5,   5,   6,   6,   6,   6,   7,   7,   7,   7,   8,   8,   8,   8,
	  3,   3,   4,   4,   5,   5,   6,   6,   7,   7,   8,   8,   9,   9,  10,  10,
	 11,  11,  12,  12,  13,  13,  14,  14,  15,  15,  15,  15,  18,  18,  18,  20,
};

const Uint8 wari2b_cmd_02_00[] = {
	LOCK_SET,
	WAIT2( 6 ),
	WALK_EM( 8 ),
	CMD_JUMP( 16 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 wari2b_cmd_02_01[] = {
	LOCK_SET,
	IF_CATCH,
		CMD_JUMP( 55 ),
	CMD_ELSE,
		CMD_JUMP( 50 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 wari2b_cmd_02_02[] = {
	CMD_JUMP( 0 ),
	GUARD( 60 ),
	CMD_EXIT,
};

const Uint8 wari2b_cmd_02_03[] = {
	GUARD( 120 ),
	CMD_EXIT,
};

const Uint8 wari2b_cmd_03_00[] = {
	GUARD_CONT( 16 ),
	CMD_EXIT,
};

const Uint8 wari2b_cmd_05_00[] = {
	GUARD( 30 ),
	IF_CATCH,
		CMD_JUMP( 55 ),
	CMD_ELSE,
		CMD_JUMP( 50 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari2b_cmd_05_01[] = {
	TIMER2_SET( 20 ),
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT2( 4 ),
	CMD_EXIT,
};

const Uint8 wari2b_cmd_05_02[] = {
	TIMER2_SET( 20 ),
	IF_RND( 12 ),
		IF_RND( 16 ),
			VCOUNTER_SET( 4 ),
		CMD_ELSE,
			VCOUNTER_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT2( 4 ),
	CMD_EXIT,
};

const Uint8 wari2b_cmd_05_03[] = {
	LOCK_SET,
	IF_LEVEL( 20 ),
		IF_RND( 20 ),
			IF_ATCK_COME,
				SP_CMD( pl2b_sp_berserk, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 16 ),
			IF_RND( 16 ),
				IF_ATCK_COME,
					SP_CMD( pl2b_sp_berserk, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 12 ),
				IF_RND( 12 ),
					IF_ATCK_COME,
						SP_CMD( pl2b_sp_berserk, 0, PK_PUNCH, LV_LIGHT, 0 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ELSE,
				GUARD( 30 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 wari2b_cmd_06_00[] = {
	CMD_JUMP( 0 ),
	GUARD( 30 ),
	CMD_EXIT,
};

const Uint8 wari2b_cmd_06_01[] = {
	CMD_JUMP( 0 ),
	CANCEL_SET2,
		PUNCH( LV_LIGHT ),
		PUNCH( LV_HEAVY ),
		CANCEL_CLR,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari2b_cmd_06_02[] = {
	IF_RND( 16 ),
		TIMER_SET( 5 ),
		IF_CATCH,
			NAG_VFLIP_ADJ( 1, LEVER_FORWARD ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari2b_cmd_07_00[] = {
	GUARD( 30 ),
	IF_CATCH,
		CMD_JUMP( 55 ),
	CMD_ELSE,
		CMD_JUMP( 50 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari2b_cmd_07_01[] = {
	IF_LEVEL( 24 ),
		IF_RND( 20 ),
			GUARD_CONT( 16 ),
		CMD_ELSE,
			GUARD( 30 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_RND( 18 ),
			GUARD( 60 ),
		CMD_ELSE,
			GUARD( 30 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari2b_cmd_07_02[] = {
	CMD_JUMP( 22 ),
	CMD_EXIT,
};

const Uint8 wari2b_cmd_08_00[] = {
	CMD_JUMP( 0 ),
	CANCEL_SET2,
		PUNCH( LV_LIGHT ),
		PUNCH( LV_HEAVY ),
		CANCEL_CLR,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari2b_cmd_08_01[] = {
	IF_LEVEL( 24 ),
		IF_RND( 20 ),
			GUARD_CONT( 16 ),
		CMD_ELSE,
			GUARD( 30 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_RND( 18 ),
			GUARD( 60 ),
		CMD_ELSE,
			GUARD( 30 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari2b_cmd_09_00[] = {
	GUARD( 30 ),
	IF_CATCH,
		CMD_JUMP( 55 ),
	CMD_ELSE,
		CMD_JUMP( 50 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari2b_cmd_09_01[] = {
	LOCK_SET,
	TIMER2_SET( 40 ),
	CROUCH,
	TIMER_SET( 30 ),
	IF_EM_ATCK( 48 ),
		CMD_JUMP( 0 ),
		SP_CMD( pl2b_sp_berserk, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ELSE,
		CMD_JUMP( 0 ),
		PUNCH( LV_HEAVY ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 wari2b_cmd_09_02[] = {
	CMD_JUMP( 0 ),
	IF_RND( 20 ),
		GUARD( 60 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari2b_cmd_10_00[] = {
	CMD_JUMP( 0 ),
	IF_RND( 20 ),
		TIMER2_SET( 60 ),
	CMD_ENDIF,
	WAIT2( 14 ),
	CMD_EXIT,
};

const Uint8 wari2b_cmd_10_01[] = {
	LOCK_SET,
	TIMER2_SET( 60 ),
	IF_RND( 20 ),
		TIMER_SET( 60 ),
		IF_EM_ATCK( ATCK_CROUCH_HK ),
			CMD_JUMP( 0 ),
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

const Uint8 wari2b_cmd_10_02[] = {
	LOCK_SET,
	IF_RND( 20 ),
		TIMER2_SET( 40 ),
	CMD_ENDIF,
	CROUCH,
	TIMER_SET( 120 ),
	IF_EM_ATCK( 48 ),
		CMD_JUMP( 0 ),
		SP_CMD( pl2b_sp_berserk, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 wari2b_cmd_11_00[] = {
	DASH_EM( 80 ),
	TIMER_SET( 120 ),
	IF_DIST( 80, 64, 0, 160 ),
		SP_CMD( pl2b_sp_berserk, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ELSE,
		WAIT2( 14 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari2b_cmd_11_01[] = {
	DASH_B( 256 ),
	IF_RND( 16 ),
		SP_CMD( pl2b_sp_berserk, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ELSE,
		SP_CMD( pl2b_sp_berserk, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari2b_cmd_11_02[] = {
	DASH_EM( 0 ),
	SJUMP( DIR_UP ),
	SJUMP_FLAG_SET,
		TIMER_SET( 0xa0 ),
		IF_EM_ATCK( ATCK_JUMP_LP ),
			CMD_JUMP( 36 ),
		CMD_ELSE,
			CMD_JUMP( 47 ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari2b_cmd_12_00[] = {
	IF_DIST( 80, 0, 80, 0 ),
		JUMP( DIR_UP ),
		JUMP_FLAG_SET,
			TIMER_SET( 20 ),
			IF_EM_ATCK( ATCK_JUMP_HK ),
				WAIT2( 4 ),
				CMD_JUMP( 19 ),
			CMD_ELSE,
				WAIT2( 4 ),
				KICK( LV_HEAVY ),
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		IF_DIST( 104, 0, 104, 0 ),
			JUMP( DIR_FORWARD ),
			JUMP_FLAG_SET,
				TIMER_SET( 20 ),
				IF_EM_ATCK( ATCK_JUMP_HK ),
					WAIT2( 4 ),
					CMD_JUMP( 36 ),
				CMD_ELSE,
					WAIT2( 4 ),
					CMD_JUMP( 47 ),
				CMD_ENDIF,
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari2b_cmd_12_01[] = {
	CROUCH,
	GUARD( 30 ),
	CMD_JUMP( 56 ),
	CMD_EXIT,
};

const Uint8 wari2b_cmd_12_02[] = {
	WAIT( 60 ),
	CMD_EXIT,
};

const Uint8 wari2b_cmd_12_03[] = {
	CMD_JUMP( 0 ),
	GUARD( 90 ),
	CMD_EXIT,
};

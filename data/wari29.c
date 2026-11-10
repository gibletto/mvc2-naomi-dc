/* Juggernaut (character 29): the computer's lists for cutting in.
   What it does when something happens in the fight. PLAYER_WORK's cpu_adrs[2] points here; the
   event checks of em_play.c start the modes.

   wari29_tbl        a record (CPU_DATA) for each mode from 2 on: the offsets of its tables.
   wari29_rate_MM    the chance, out of 32, that mode MM is started, by the level of play.
   wari29_sel_MM     32 picks; one is taken at random.
   wari29_cmd_MM     the mode's lists: a pick is a number in this table.
   wari29_cmd_MM_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name
                    a list of lib29.c.
   wari29_unused_N   bytes that nothing in the file reaches. */
#include "player.h"
#include "pl29_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const wari29_cmd_02[], *const wari29_cmd_03[], *const wari29_cmd_05[],
	*const wari29_cmd_06[], *const wari29_cmd_07[], *const wari29_cmd_08[], *const wari29_cmd_09[],
	*const wari29_cmd_10[], *const wari29_cmd_11[], *const wari29_cmd_12[];
extern const Uint8 wari29_rate_02[], wari29_sel_02[], wari29_rate_03[], wari29_rate_04[],
	wari29_sel_03[], wari29_rate_05[], wari29_sel_05[], wari29_rate_06[], wari29_sel_06[],
	wari29_rate_07[], wari29_sel_07[], wari29_rate_08[], wari29_sel_08[], wari29_rate_09[],
	wari29_sel_09[], wari29_unused_0[], wari29_rate_10[], wari29_sel_10[], wari29_rate_11[],
	wari29_sel_11[], wari29_rate_12[], wari29_sel_12[], wari29_rate_13[], wari29_cmd_02_00[],
	wari29_cmd_02_01[], wari29_cmd_02_02[], wari29_cmd_02_03[], wari29_cmd_03_00[],
	wari29_cmd_05_00[], wari29_cmd_05_01[], wari29_cmd_05_02[], wari29_cmd_05_03[],
	wari29_cmd_06_00[], wari29_cmd_07_00[], wari29_cmd_07_01[], wari29_cmd_07_02[],
	wari29_cmd_08_00[], wari29_cmd_09_00[], wari29_cmd_09_01[], wari29_cmd_09_02[],
	wari29_cmd_10_00[], wari29_cmd_10_01[], wari29_cmd_10_02[], wari29_cmd_11_00[],
	wari29_cmd_11_01[], wari29_cmd_11_02[], wari29_cmd_11_03[], wari29_cmd_12_00[],
	wari29_cmd_12_01[], wari29_cmd_12_02[], wari29_cmd_12_03[], wari29_cmd_12_04[];

const CPU_DATA wari29_tbl[] = {
	{ ( Sint32 )wari29_rate_02, ( Sint32 )wari29_sel_02, ( Sint32 )wari29_cmd_02 },	/* 2: a throw would catch the opponent */
	{ ( Sint32 )wari29_rate_03, ( Sint32 )wari29_sel_03, ( Sint32 )wari29_cmd_03 },	/* 3: it guards a hit (the chance is the program's own) */
	{ ( Sint32 )wari29_rate_04, ( Sint32 )wari29_sel_03, ( Sint32 )wari29_cmd_03 },	/* 4: never started: a guard in the air is mode 3 */
	{ ( Sint32 )wari29_rate_05, ( Sint32 )wari29_sel_05, ( Sint32 )wari29_cmd_05 },	/* 5: after it has guarded on the ground */
	{ ( Sint32 )wari29_rate_06, ( Sint32 )wari29_sel_06, ( Sint32 )wari29_cmd_06 },	/* 6: after it has guarded in the air */
	{ ( Sint32 )wari29_rate_07, ( Sint32 )wari29_sel_07, ( Sint32 )wari29_cmd_07 },	/* 7: after it has been hit */
	{ ( Sint32 )wari29_rate_08, ( Sint32 )wari29_sel_08, ( Sint32 )wari29_cmd_08 },	/* 8: never started */
	{ ( Sint32 )wari29_rate_09, ( Sint32 )wari29_sel_09, ( Sint32 )wari29_cmd_09 },	/* 9: after it has got up */
	{ ( Sint32 )wari29_rate_10, ( Sint32 )wari29_sel_10, ( Sint32 )wari29_cmd_10 },	/* 10: the opponent dashes */
	{ ( Sint32 )wari29_rate_11, ( Sint32 )wari29_sel_11, ( Sint32 )wari29_cmd_11 },	/* 11: the opponent flies or super jumps */
	{ ( Sint32 )wari29_rate_12, ( Sint32 )wari29_sel_12, ( Sint32 )wari29_cmd_12 },	/* 12: the opponent jumps */
	{ ( Sint32 )wari29_rate_13, 0, 0 },	/* 13: an attack is coming: the chance alone, the lists are yok29.c's */
};

const Uint8 wari29_rate_02[] = {
	  4,   5,   6,   7,   9,  11,  11,  13,  13,  15,  15,  17,  17,  19,  19,  21,
	 21,  23,  23,  24,  24,  26,  26,  28,  28,  30,  30,  31,  31,  31,  31,  31,
};

const Uint8 wari29_sel_02[] = {
	  3,   3,   3,   3,   3,   3,   3,   3,   1,   1,   1,   1,   0,   0,   0,   0,
	  0,   0,   3,   3,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
};

const Uint8 *const wari29_cmd_02[] = {
	wari29_cmd_02_00, wari29_cmd_02_01, wari29_cmd_02_02, wari29_cmd_02_03,
};

const Uint8 wari29_rate_03[] = {
	  0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,
	  2,   2,   2,   2,   3,   3,   3,   3,   3,   3,   3,   3,   4,   4,   4,   4,
};

const Uint8 wari29_rate_04[] = {
	 16,  16,  17,  17,  18,  19,  20,  21,  22,  23,  24,  25,  25,  25,  25,  25,
	 25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,
};

const Uint8 wari29_sel_03[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const wari29_cmd_03[] = {
	wari29_cmd_03_00,
};

const Uint8 wari29_rate_05[] = {
	  4,   4,   4,   8,  12,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,
	 16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,
};

const Uint8 wari29_sel_05[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   3,   3,   3,   3,
};

const Uint8 *const wari29_cmd_05[] = {
	wari29_cmd_05_00, wari29_cmd_05_01, wari29_cmd_05_02, wari29_cmd_05_03,
};

const Uint8 wari29_rate_06[] = {
	  4,   4,   4,   8,  12,  16,  18,  20,  22,  24,  26,  28,  30,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari29_sel_06[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const wari29_cmd_06[] = {
	wari29_cmd_06_00,
};

const Uint8 wari29_rate_07[] = {
	  4,   4,   4,   8,  12,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,
	 16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,
};

const Uint8 wari29_sel_07[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,
	  1,   1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,
};

const Uint8 *const wari29_cmd_07[] = {
	wari29_cmd_07_00, wari29_cmd_07_01, wari29_cmd_07_02,
};

const Uint8 wari29_rate_08[] = {
	  4,   4,   4,   8,  12,  16,  18,  20,  22,  24,  26,  28,  30,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari29_sel_08[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const wari29_cmd_08[] = {
	wari29_cmd_08_00,
};

const Uint8 wari29_rate_09[] = {
	  6,   6,   8,   8,   8,  10,  12,  14,  16,  18,  20,  22,  24,  25,  26,  27,
	 28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari29_sel_09[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   1,   1,   1,   0,   0,   0,   0,   0,   2,   2,   2,
};

const Uint8 *const wari29_cmd_09[] = {
	wari29_cmd_09_00, wari29_cmd_09_01, wari29_cmd_09_02,
};

const Uint8 wari29_unused_0[] = {
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	  0,   0,   0,   0,   0,   0,   0,   0,   2,   2,   2,   2,   0,   0,   0,   0,
	  2,   2,   2,   2,   2,   2,   2,   2,   1,   1,   1,   1,   1,   1,   1,   1,
	219,   5,   0,   0, 239,   5,   0,   0,   5,   6,   0,   0,  31,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  25,   6,   0,   0,
};

const Uint8 wari29_rate_10[] = {
	  1,   2,   3,   4,   5,   6,   7,   7,   7,   8,   8,   8,   9,   9,   9,  10,
	 10,  10,  11,  12,  13,  14,  15,  16,  16,  16,  16,  16,  16,  16,  16,  16,
};

const Uint8 wari29_sel_10[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,
};

const Uint8 *const wari29_cmd_10[] = {
	wari29_cmd_10_00, wari29_cmd_10_01, wari29_cmd_10_02,
};

const Uint8 wari29_rate_11[] = {
	 16,  16,  16,  17,  17,  17,  18,  18,  18,  19,  19,  19,  20,  20,  20,  21,
	 21,  21,  22,  22,  22,  23,  23,  23,  24,  24,  24,  25,  25,  25,  26,  26,
};

const Uint8 wari29_sel_11[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   3,   3,   3,   3,   3,   3,   3,   3,
	  1,   1,   1,   1,   1,   0,   0,   0,   2,   2,   2,   2,   2,   3,   3,   3,
};

const Uint8 *const wari29_cmd_11[] = {
	wari29_cmd_11_00, wari29_cmd_11_01, wari29_cmd_11_02, wari29_cmd_11_03,
};

const Uint8 wari29_rate_12[] = {
	 16,  16,  16,  17,  17,  17,  18,  18,  18,  19,  19,  19,  20,  20,  20,  21,
	 21,  21,  22,  22,  22,  23,  23,  23,  24,  24,  24,  25,  25,  25,  26,  26,
};

const Uint8 wari29_sel_12[] = {
	  0,   0,   0,   0,   0,   0,   2,   2,   1,   1,   1,   1,   1,   1,   3,   3,
	  4,   4,   4,   4,   3,   3,   3,   3,   4,   4,   4,   4,   4,   4,   4,   4,
};

const Uint8 *const wari29_cmd_12[] = {
	wari29_cmd_12_00, wari29_cmd_12_01, wari29_cmd_12_02, wari29_cmd_12_03, wari29_cmd_12_04,
};

const Uint8 wari29_rate_13[] = {
	  8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,
	 24,  25,  26,  27,  28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	  1,   2,   3,   4,   5,   6,   7,   8,   9,   9,   9,   9,  10,  10,  10,  10,
	 10,  11,  11,  11,  11,  11,  11,  11,  12,  12,  12,  12,  12,  12,  12,  12,
	  1,   2,   3,   4,   5,   6,   7,   8,   9,   9,   9,   9,  10,  10,  10,  10,
	 10,  11,  11,  11,  11,  11,  11,  11,  12,  12,  12,  12,  12,  12,  12,  12,
	  4,   4,   5,   5,   5,   6,   7,   8,   9,   9,   9,   9,  10,  10,  10,  10,
	 10,  11,  11,  11,  11,  11,  11,  11,  12,  12,  12,  12,  12,  12,  12,  12,
};

const Uint8 wari29_cmd_02_00[] = {
	CMD_JUMP( 15 ),
	IF_CATCH,
		NAG_VFLIP_ADJ( 1, LEVER_FORWARD ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari29_cmd_02_01[] = {
	CMD_JUMP( 15 ),
	IF_CATCH,
		NAG_VFLIP_ADJ( 1, LEVER_BACK ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari29_cmd_02_02[] = {
	CMD_JUMP( 15 ),
	CMD_EXIT,
};

const Uint8 wari29_cmd_02_03[] = {
	CMD_JUMP( 15 ),
	IF_LEVEL( 20 ),
		JUMP_RND( 1, 6, 7, 11 ),
	CMD_ELSE,
		LOCK_SET,
		IF_EM_ATCK( ATCK_CROUCH_LK ),
			CANCEL_SET2,
				WAIT2( 8 ),
				CROUCH,
				KICK( LV_LIGHT ),
				WAIT2( 8 ),
				KICK( LV_LIGHT ),
				WAIT2( 8 ),
				SP_CMD( pl29_sp_press, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			SP_CMD( pl29_sp_quake, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ENDIF,
		LOCK_CLR,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari29_cmd_03_00[] = {
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	GUARD_CONT( 16 ),
	CMD_EXIT,
};

const Uint8 wari29_cmd_05_00[] = {
	CMD_JUMP( 15 ),
	IF_LEVEL( 24 ),
		JUMP_RND( 2, 4, 7, 6 ),
	CMD_ELSE,
		IF_LEVEL( 16 ),
			JUMP_RND( 0, 3, 11, 5 ),
		CMD_ELSE,
			CANCEL_SET,
				CROUCH,
				KICK( LV_LIGHT ),
				KICK( LV_LIGHT ),
				SP_CMD( pl29_sp_quake, 0, PK_PUNCH, LV_HEAVY, 0 ),
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari29_cmd_05_01[] = {
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_JUMP( 15 ),
	GUARD( 60 ),
	CMD_EXIT,
};

const Uint8 wari29_cmd_05_02[] = {
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_JUMP( 24 ),
	CMD_EXIT,
};

const Uint8 wari29_cmd_05_03[] = {
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_JUMP( 15 ),
	IF_LEVEL( 16 ),
		VCOUNTER_SET( 3 ),
	CMD_ELSE,
		GUARD( 30 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari29_cmd_06_00[] = {
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_JUMP( 15 ),
	TIMER2_SET( 60 ),
	CMD_EXIT,
};

const Uint8 wari29_cmd_07_00[] = {
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_JUMP( 15 ),
	JUMP_RND( 4, 6, 8, 11 ),
	CMD_EXIT,
};

const Uint8 wari29_cmd_07_01[] = {
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_JUMP( 15 ),
	GUARD( 60 ),
	CMD_EXIT,
};

const Uint8 wari29_cmd_07_02[] = {
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_JUMP( 24 ),
	CMD_EXIT,
};

const Uint8 wari29_cmd_08_00[] = {
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_JUMP( 15 ),
	TIMER2_SET( 80 ),
	CMD_EXIT,
};

const Uint8 wari29_cmd_09_00[] = {
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_JUMP( 15 ),
	GUARD( 60 ),
	CMD_EXIT,
};

const Uint8 wari29_cmd_09_01[] = {
	CMD_JUMP( 15 ),
	IF_LEVEL( 24 ),
		JUMP_RND( 5, 7, 9, 11 ),
	CMD_ELSE,
		WAIT2( 8 ),
		SP_CMD( pl29_sp_quake, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari29_cmd_09_02[] = {
	CMD_JUMP( 15 ),
	IF_LEVEL( 16 ),
		JUMP_RND( 3, 1, 12, 5 ),
	CMD_ELSE,
		CMD_JUMP( 13 ),
	CMD_ENDIF,
	CMD_EXIT,
	CMD_JUMP( 15 ),
	STAND,
	DASH_ATCK( ATCK_STAND_HP ),
		CANCEL_SET2,
			WAIT2( 8 ),
			PUNCH( LV_HEAVY ),
			SP_CMD( pl29_sp_punch, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
	CMD_JUMP( 15 ),
	IF_RND( 8 ),
		SP_CMD( pl29_sp_chouhatsu, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ELSE,
		STAND,
		WAIT( 1 ),
		SP_CMD( pl29_sp_powerup, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ENDIF,
	CMD_EXIT,
	CMD_JUMP( 15 ),
	IF_LEVEL( 24 ),
		JUMP_RND( 5, 7, 9, 11 ),
	CMD_ELSE,
		WAIT2( 8 ),
		SP_CMD( pl29_sp_quake, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ENDIF,
	CMD_EXIT,
	CMD_JUMP( 15 ),
	CMD_EXIT,
};

const Uint8 wari29_cmd_10_00[] = {
	CMD_JUMP( 15 ),
	IF_LEVEL( 24 ),
		JUMP_RND( 5, 7, 9, 11 ),
	CMD_ELSE,
		WAIT2( 8 ),
		SP_CMD( pl29_sp_quake, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari29_cmd_10_01[] = {
	CMD_JUMP( 15 ),
	JUMP( DIR_UP ),
	JUMP_FLAG_SET,
		TIMER_SET( 90 ),
		IF_EM_ATCK( ATCK_JUMP_MK ),
			WAIT2( 8 ),
			KICK( LV_LIGHT ),
		CMD_ELSE,
			GUARD( 60 ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari29_cmd_10_02[] = {
	CMD_JUMP( 15 ),
	IF_LEVEL( 16 ),
		JUMP_RND( 3, 1, 12, 5 ),
	CMD_ELSE,
		CMD_JUMP( 13 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari29_cmd_11_00[] = {
	CMD_JUMP( 15 ),
	IF_LEVEL( 16 ),
		JUMP_RND( 2, 2, 14, 2 ),
	CMD_ELSE,
		JUMP_RND( 1, 14, 2, 2 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari29_cmd_11_01[] = {
	CMD_JUMP( 15 ),
	SJUMP( DIR_UP ),
	SJUMP_FLAG_SET,
		TIMER_SET( 90 ),
		IF_EM_ATCK( ATCK_JUMP_HP ),
			WAIT2( 8 ),
			PUNCH( LV_HEAVY ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari29_cmd_11_02[] = {
	CMD_JUMP( 15 ),
	STAND,
	WAIT( 1 ),
	SP_CMD( pl29_sp_powerup, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_EXIT,
};

const Uint8 wari29_cmd_11_03[] = {
	IF_LEVEL( 16 ),
		CMD_JUMP( 15 ),
		JUMP_RND( 1, 1, 1, 14 ),
	CMD_ELSE,
		IF_RND( 16 ),
			CMD_JUMP( 15 ),
			JUMP_RND( 1, 1, 1, 14 ),
		CMD_ELSE,
			CMD_JUMP( 15 ),
			GUARD( 60 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari29_cmd_12_00[] = {
	CMD_JUMP( 15 ),
	IF_LEVEL( 24 ),
		JUMP_RND( 2, 2, 14, 2 ),
	CMD_ELSE,
		JUMP_RND( 1, 14, 2, 2 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari29_cmd_12_01[] = {
	CMD_JUMP( 15 ),
	IF_LEVEL( 16 ),
		JUMP_RND( 14, 2, 1, 2 ),
	CMD_ELSE,
		TIMER_SET( 90 ),
		CROUCH,
		IF_EM_ATCK( ATCK_CROUCH_HP ),
			WAIT2( 8 ),
			PUNCH( LV_HEAVY ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari29_cmd_12_02[] = {
	CMD_JUMP( 15 ),
	IF_LEVEL( 16 ),
		JUMP_RND( 1, 11, 8, 9 ),
	CMD_ELSE,
		WAIT2( 8 ),
		SP_CMD( pl29_sp_quake, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari29_cmd_12_03[] = {
	CMD_JUMP( 15 ),
	IF_LEVEL( 16 ),
		JUMP_RND( 1, 11, 8, 9 ),
	CMD_ELSE,
		CMD_JUMP( 13 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari29_cmd_12_04[] = {
	IF_LEVEL( 16 ),
		CMD_JUMP( 15 ),
		JUMP_RND( 1, 1, 1, 14 ),
	CMD_ELSE,
		IF_RND( 16 ),
			CMD_JUMP( 15 ),
			JUMP_RND( 1, 1, 1, 14 ),
		CMD_ELSE,
			CMD_JUMP( 15 ),
			GUARD( 60 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

/* Omega Red (character 30): the computer's lists for cutting in.
   What it does when something happens in the fight. PLAYER_WORK's cpu_adrs[2] points here; the
   event checks of em_play.c start the modes.

   wari30_tbl        a record (CPU_DATA) for each mode from 2 on: the offsets of its tables.
   wari30_rate_MM    the chance, out of 32, that mode MM is started, by the level of play.
   wari30_sel_MM     32 picks; one is taken at random.
   wari30_cmd_MM     the mode's lists: a pick is a number in this table.
   wari30_cmd_MM_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name
                    a list of lib30.c.
   wari30_unused_N   bytes that nothing in the file reaches. */
#include "player.h"
#include "pl30_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const wari30_cmd_02[], *const wari30_cmd_03[], *const wari30_cmd_05[],
	*const wari30_cmd_06[], *const wari30_cmd_07[], *const wari30_cmd_08[], *const wari30_cmd_09[],
	*const wari30_cmd_10[], *const wari30_cmd_11[], *const wari30_cmd_12[];
extern const Uint8 wari30_rate_02[], wari30_sel_02[], wari30_rate_03[], wari30_rate_04[],
	wari30_sel_03[], wari30_rate_05[], wari30_sel_05[], wari30_rate_06[], wari30_sel_06[],
	wari30_rate_07[], wari30_sel_07[], wari30_rate_08[], wari30_sel_08[], wari30_rate_09[],
	wari30_sel_09[], wari30_unused_0[], wari30_rate_10[], wari30_sel_10[], wari30_rate_11[],
	wari30_sel_11[], wari30_rate_12[], wari30_sel_12[], wari30_rate_13[], wari30_cmd_02_00[],
	wari30_cmd_02_01[], wari30_cmd_02_02[], wari30_cmd_02_03[], wari30_cmd_03_00[],
	wari30_cmd_05_00[], wari30_cmd_05_01[], wari30_cmd_05_02[], wari30_cmd_05_03[],
	wari30_cmd_06_00[], wari30_cmd_07_00[], wari30_cmd_07_01[], wari30_cmd_07_02[],
	wari30_cmd_08_00[], wari30_cmd_09_00[], wari30_cmd_09_01[], wari30_cmd_09_02[],
	wari30_cmd_10_00[], wari30_cmd_10_01[], wari30_cmd_10_02[], wari30_cmd_11_00[],
	wari30_cmd_11_01[], wari30_cmd_11_02[], wari30_cmd_11_03[], wari30_cmd_12_00[],
	wari30_cmd_12_01[], wari30_cmd_12_02[], wari30_cmd_12_03[], wari30_cmd_12_04[];

const CPU_DATA wari30_tbl[] = {
	{ ( Sint32 )wari30_rate_02, ( Sint32 )wari30_sel_02, ( Sint32 )wari30_cmd_02 },	/* 2: a throw would catch the opponent */
	{ ( Sint32 )wari30_rate_03, ( Sint32 )wari30_sel_03, ( Sint32 )wari30_cmd_03 },	/* 3: it guards a hit (the chance is the program's own) */
	{ ( Sint32 )wari30_rate_04, ( Sint32 )wari30_sel_03, ( Sint32 )wari30_cmd_03 },	/* 4: never started: a guard in the air is mode 3 */
	{ ( Sint32 )wari30_rate_05, ( Sint32 )wari30_sel_05, ( Sint32 )wari30_cmd_05 },	/* 5: after it has guarded on the ground */
	{ ( Sint32 )wari30_rate_06, ( Sint32 )wari30_sel_06, ( Sint32 )wari30_cmd_06 },	/* 6: after it has guarded in the air */
	{ ( Sint32 )wari30_rate_07, ( Sint32 )wari30_sel_07, ( Sint32 )wari30_cmd_07 },	/* 7: after it has been hit */
	{ ( Sint32 )wari30_rate_08, ( Sint32 )wari30_sel_08, ( Sint32 )wari30_cmd_08 },	/* 8: never started */
	{ ( Sint32 )wari30_rate_09, ( Sint32 )wari30_sel_09, ( Sint32 )wari30_cmd_09 },	/* 9: after it has got up */
	{ ( Sint32 )wari30_rate_10, ( Sint32 )wari30_sel_10, ( Sint32 )wari30_cmd_10 },	/* 10: the opponent dashes */
	{ ( Sint32 )wari30_rate_11, ( Sint32 )wari30_sel_11, ( Sint32 )wari30_cmd_11 },	/* 11: the opponent flies or super jumps */
	{ ( Sint32 )wari30_rate_12, ( Sint32 )wari30_sel_12, ( Sint32 )wari30_cmd_12 },	/* 12: the opponent jumps */
	{ ( Sint32 )wari30_rate_13, 0, 0 },	/* 13: an attack is coming: the chance alone, the lists are yok30.c's */
};

const Uint8 wari30_rate_02[] = {
	  4,   5,   6,   7,   9,  11,  11,  13,  13,  15,  15,  17,  17,  19,  19,  21,
	 21,  23,  23,  24,  24,  26,  26,  28,  28,  30,  30,  31,  31,  31,  31,  31,
};

const Uint8 wari30_sel_02[] = {
	  3,   3,   3,   3,   3,   3,   3,   3,   1,   1,   1,   1,   0,   0,   0,   0,
	  0,   0,   3,   3,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
};

const Uint8 *const wari30_cmd_02[] = {
	wari30_cmd_02_00, wari30_cmd_02_01, wari30_cmd_02_02, wari30_cmd_02_03,
};

const Uint8 wari30_rate_03[] = {
	  0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,
	  2,   2,   2,   2,   3,   3,   3,   3,   3,   3,   3,   3,   4,   4,   4,   4,
};

const Uint8 wari30_rate_04[] = {
	 16,  16,  17,  17,  18,  19,  20,  21,  22,  23,  24,  25,  25,  25,  25,  25,
	 25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,
};

const Uint8 wari30_sel_03[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const wari30_cmd_03[] = {
	wari30_cmd_03_00,
};

const Uint8 wari30_rate_05[] = {
	  4,   4,   4,   8,  12,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,
	 16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,
};

const Uint8 wari30_sel_05[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   3,   3,   3,   3,
};

const Uint8 *const wari30_cmd_05[] = {
	wari30_cmd_05_00, wari30_cmd_05_01, wari30_cmd_05_02, wari30_cmd_05_03,
};

const Uint8 wari30_rate_06[] = {
	  4,   4,   4,   8,  12,  16,  18,  20,  22,  24,  26,  28,  30,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari30_sel_06[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const wari30_cmd_06[] = {
	wari30_cmd_06_00,
};

const Uint8 wari30_rate_07[] = {
	  4,   4,   4,   8,  12,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,
	 16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,
};

const Uint8 wari30_sel_07[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,
	  1,   1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,
};

const Uint8 *const wari30_cmd_07[] = {
	wari30_cmd_07_00, wari30_cmd_07_01, wari30_cmd_07_02,
};

const Uint8 wari30_rate_08[] = {
	  4,   4,   4,   8,  12,  16,  18,  20,  22,  24,  26,  28,  30,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari30_sel_08[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const wari30_cmd_08[] = {
	wari30_cmd_08_00,
};

const Uint8 wari30_rate_09[] = {
	  6,   6,   8,   8,   8,  10,  12,  14,  16,  18,  20,  22,  24,  25,  26,  27,
	 28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari30_sel_09[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   1,   1,   1,   0,   0,   0,   0,   0,   2,   2,   2,
};

const Uint8 *const wari30_cmd_09[] = {
	wari30_cmd_09_00, wari30_cmd_09_01, wari30_cmd_09_02,
};

const Uint8 wari30_unused_0[] = {
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	  0,   0,   0,   0,   0,   0,   0,   0,   2,   2,   2,   2,   0,   0,   0,   0,
	  2,   2,   2,   2,   2,   2,   2,   2,   1,   1,   1,   1,   1,   1,   1,   1,
	  9,   6,   0,   0,  32,   6,   0,   0,  37,   6,   0,   0,  31,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  42,   6,   0,   0,
};

const Uint8 wari30_rate_10[] = {
	  1,   2,   3,   4,   5,   6,   7,   7,   7,   8,   8,   8,   9,   9,   9,  10,
	 10,  10,  11,  12,  13,  14,  15,  16,  16,  16,  16,  16,  16,  16,  16,  16,
};

const Uint8 wari30_sel_10[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,
};

const Uint8 *const wari30_cmd_10[] = {
	wari30_cmd_10_00, wari30_cmd_10_01, wari30_cmd_10_02,
};

const Uint8 wari30_rate_11[] = {
	 16,  16,  16,  17,  17,  17,  18,  18,  18,  19,  19,  19,  20,  20,  20,  21,
	 21,  21,  22,  22,  22,  23,  23,  23,  24,  24,  24,  25,  25,  25,  26,  26,
};

const Uint8 wari30_sel_11[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   3,   3,
};

const Uint8 *const wari30_cmd_11[] = {
	wari30_cmd_11_00, wari30_cmd_11_01, wari30_cmd_11_02, wari30_cmd_11_03,
};

const Uint8 wari30_rate_12[] = {
	 16,  16,  16,  17,  17,  17,  18,  18,  18,  19,  19,  19,  20,  20,  20,  21,
	 21,  21,  22,  22,  22,  23,  23,  23,  24,  24,  24,  25,  25,  25,  26,  26,
};

const Uint8 wari30_sel_12[] = {
	  0,   0,   0,   0,   0,   0,   2,   2,   1,   1,   1,   1,   1,   1,   3,   3,
	  4,   4,   4,   4,   3,   3,   3,   3,   4,   4,   4,   4,   4,   4,   4,   4,
};

const Uint8 *const wari30_cmd_12[] = {
	wari30_cmd_12_00, wari30_cmd_12_01, wari30_cmd_12_02, wari30_cmd_12_03, wari30_cmd_12_04,
};

const Uint8 wari30_rate_13[] = {
	  8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,
	 24,  25,  26,  27,  28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	  1,   2,   3,   4,   5,   6,   7,   8,   9,   9,   9,   9,  10,  10,  10,  10,
	 10,  11,  11,  11,  11,  11,  11,  11,  12,  12,  12,  12,  12,  12,  12,  12,
	  1,   2,   3,   4,   5,   6,   7,   8,   9,   9,   9,   9,  10,  10,  10,  10,
	 10,  11,  11,  11,  11,  11,  11,  11,  12,  12,  12,  12,  12,  12,  12,  12,
	  4,   4,   5,   5,   5,   6,   7,   8,   9,   9,   9,   9,  10,  10,  10,  10,
	 10,  11,  11,  11,  11,  11,  11,  11,  12,  12,  12,  12,  12,  12,  12,  12,
};

const Uint8 wari30_cmd_02_00[] = {
	CMD_JUMP( 21 ),
	IF_CATCH,
		IF_RND( 16 ),
			NAG_VFLIP_ADJ( 0, LEVER_FORWARD ),
		CMD_ELSE,
			NAG_VFLIP_ADJ( 0, LEVER_BACK ),
		CMD_ENDIF,
		IF_LEVEL( 18 ),
			WAIT2( 8 ),
			CMD_JUMP( 15 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari30_cmd_02_01[] = {
	CMD_JUMP( 21 ),
	IF_CATCH,
		IF_RND( 16 ),
			NAG_VFLIP_ADJ( 1, LEVER_FORWARD ),
		CMD_ELSE,
			NAG_VFLIP_ADJ( 1, LEVER_BACK ),
		CMD_ENDIF,
		IF_LEVEL( 18 ),
			WAIT2( 8 ),
			CMD_JUMP( 15 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari30_cmd_02_02[] = {
	CMD_JUMP( 21 ),
	CANCEL_SET,
		CROUCH,
		PUNCH( LV_LIGHT ),
		KICK( LV_LIGHT ),
		KICK( LV_HEAVY ),
		CANCEL_CLR,
	CMD_ENDIF,
	IF_LEVEL( 16 ),
		IF_GAUGE( 2 ),
			WAIT2( 6 ),
			SP_V_COMBI,
		CMD_ELSE,
			IF_GAUGE( 1 ),
				SP_CMD( pl30_sp_destroyer, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				CMD_JUMP( 3 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 2 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari30_cmd_02_03[] = {
	CMD_JUMP( 21 ),
	IF_LEVEL( 20 ),
		JUMP_RND( 7, 8, 12, 13 ),
	CMD_ELSE,
		CANCEL_SET2,
			LOCK_SET,
			IF_EM_ATCK( ATCK_CROUCH_LK ),
				WAIT2( 8 ),
				CROUCH,
				KICK( LV_LIGHT ),
				WAIT2( 8 ),
				KICK( LV_LIGHT ),
				WAIT2( 8 ),
				CMD_JUMP( 6 ),
			CMD_ELSE,
				CMD_JUMP( 4 ),
			CMD_ENDIF,
			LOCK_CLR,
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari30_cmd_03_00[] = {
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

const Uint8 wari30_cmd_05_00[] = {
	CMD_JUMP( 21 ),
	IF_LEVEL( 24 ),
		JUMP_RND( 2, 3, 4, 6 ),
	CMD_ELSE,
		IF_LEVEL( 16 ),
			JUMP_RND( 6, 8, 12, 11 ),
		CMD_ELSE,
			CANCEL_SET,
				CROUCH,
				KICK( LV_LIGHT ),
				SP_CMD( pl30_sp_strike, 0, PK_PUNCH, LV_HEAVY, 0 ),
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari30_cmd_05_01[] = {
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_JUMP( 21 ),
	GUARD( 60 ),
	CMD_EXIT,
};

const Uint8 wari30_cmd_05_02[] = {
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

const Uint8 wari30_cmd_05_03[] = {
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_JUMP( 21 ),
	IF_LEVEL( 16 ),
		VCOUNTER_SET( 3 ),
	CMD_ELSE,
		GUARD( 30 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari30_cmd_06_00[] = {
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_JUMP( 21 ),
	TIMER2_SET( 60 ),
	CMD_EXIT,
};

const Uint8 wari30_cmd_07_00[] = {
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_JUMP( 21 ),
	JUMP_RND( 5, 6, 10, 15 ),
	CMD_EXIT,
};

const Uint8 wari30_cmd_07_01[] = {
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_JUMP( 21 ),
	GUARD( 60 ),
	CMD_EXIT,
};

const Uint8 wari30_cmd_07_02[] = {
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

const Uint8 wari30_cmd_08_00[] = {
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_JUMP( 21 ),
	TIMER2_SET( 80 ),
	CMD_EXIT,
};

const Uint8 wari30_cmd_09_00[] = {
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_JUMP( 21 ),
	GUARD( 60 ),
	CMD_EXIT,
};

const Uint8 wari30_cmd_09_01[] = {
	CMD_JUMP( 21 ),
	IF_LEVEL( 24 ),
		JUMP_RND( 7, 8, 15, 5 ),
	CMD_ELSE,
		WAIT2( 8 ),
		CMD_JUMP( 6 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari30_cmd_09_02[] = {
	CMD_JUMP( 21 ),
	IF_LEVEL( 16 ),
		CMD_JUMP( 0 ),
	CMD_ELSE,
		CMD_JUMP( 13 ),
	CMD_ENDIF,
	CMD_EXIT,
	STAND,
	WAIT2( 8 ),
	IF_LEVEL( 18 ),
		CMD_JUMP( 26 ),
	CMD_ELSE,
		IF_LEVEL( 10 ),
			CMD_JUMP( 2 ),
		CMD_ELSE,
			IF_LEVEL( 8 ),
				CMD_JUMP( 7 ),
			CMD_ELSE,
				PARTNER_CALL,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
	WAIT2( 6 ),
	CMD_JUMP( 0 ),
	CMD_EXIT,
	WAIT2( 8 ),
	CMD_JUMP( 4 ),
	CMD_EXIT,
	CMD_JUMP( 0 ),
	CMD_EXIT,
};

const Uint8 wari30_cmd_10_00[] = {
	CMD_JUMP( 21 ),
	IF_LEVEL( 24 ),
		JUMP_RND( 12, 13, 14, 26 ),
	CMD_ELSE,
		WAIT2( 8 ),
		CROUCH,
		CMD_JUMP( 15 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari30_cmd_10_01[] = {
	CMD_JUMP( 21 ),
	JUMP( DIR_UP ),
	JUMP_FLAG_SET,
		TIMER_SET( 120 ),
		IF_HEIGHT( 0xff, 50 ),
			CMD_JUMP( 36 ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari30_cmd_10_02[] = {
	CMD_JUMP( 21 ),
	IF_LEVEL( 16 ),
		JUMP_RND( 8, 7, 11, 4 ),
	CMD_ELSE,
		CMD_JUMP( 15 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari30_cmd_11_00[] = {
	STAND,
	CMD_JUMP( 21 ),
	IF_EM_JUMP( DIR_FORWARD ),
		SJUMP( DIR_BACK ),
	CMD_ELSE,
		SJUMP( DIR_FORWARD ),
	CMD_ENDIF,
	SJUMP_FLAG_SET,
		WAIT2( 8 ),
		TIMER_SET( 120 ),
		IF_DIST( 64, 64, 0, 64 ),
			PUNCH( LV_HEAVY ),
			WAIT( 30 ),
			CMD_JUMP( 20 ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari30_cmd_11_01[] = {
	CMD_JUMP( 21 ),
	IF_RND( 16 ),
		JUMP( DIR_UP ),
		JUMP_FLAG_SET,
			WAIT( 30 ),
			CMD_JUMP( 32 ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 33 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari30_cmd_11_02[] = {
	CMD_JUMP( 21 ),
	CMD_JUMP( 6 ),
	CMD_EXIT,
};

const Uint8 wari30_cmd_11_03[] = {
	CMD_JUMP( 21 ),
	IF_LEVEL( 18 ),
		CMD_JUMP( 11 ),
	CMD_ELSE,
		CMD_JUMP( 26 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari30_cmd_12_00[] = {
	CMD_JUMP( 21 ),
	IF_EM_JUMP( DIR_FORWARD ),
		CMD_JUMP( 28 ),
	CMD_ELSE,
		IF_EM_JUMP( DIR_UP ),
			CMD_JUMP( 29 ),
		CMD_ELSE,
			IF_LEVEL( 18 ),
				PARTNER_CALL,
			CMD_ENDIF,
			CMD_JUMP( 38 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari30_cmd_12_01[] = {
	CMD_JUMP( 21 ),
	IF_LEVEL( 16 ),
		JUMP_RND( 29, 28, 25, 24 ),
	CMD_ELSE,
		TIMER_SET( 90 ),
		CROUCH,
		IF_EM_ATCK( ATCK_CROUCH_MP ),
			WAIT2( 8 ),
			PUNCH( LV_LIGHT ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari30_cmd_12_02[] = {
	CMD_JUMP( 21 ),
	IF_LEVEL( 16 ),
		JUMP_RND( 1, 11, 8, 9 ),
	CMD_ELSE,
		WAIT2( 8 ),
		CMD_JUMP( 6 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari30_cmd_12_03[] = {
	CMD_JUMP( 21 ),
	IF_LEVEL( 16 ),
		JUMP_RND( 11, 10, 12, 15 ),
	CMD_ELSE,
		CMD_JUMP( 33 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari30_cmd_12_04[] = {
	IF_LEVEL( 16 ),
		CMD_JUMP( 21 ),
		JUMP( DIR_FORWARD ),
		JUMP_FLAG_SET,
			TIMER_SET( 60 ),
			IF_HEIGHT( 0xff, 50 ),
				PUNCH( LV_HEAVY ),
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		TIMER_SET( 120 ),
		IF_EM_STANCE( STANCE_STAND ),
			JUMP_RND( 2, 7, 13, 8 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

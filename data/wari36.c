/* Thanos (character 36): the computer's lists for cutting in.
   What it does when something happens in the fight. PLAYER_WORK's cpu_adrs[2] points here; the
   event checks of em_play.c start the modes.

   wari36_tbl        a record (CPU_DATA) for each mode from 2 on: the offsets of its tables.
   wari36_rate_MM    the chance, out of 32, that mode MM is started, by the level of play.
   wari36_sel_MM     32 picks; one is taken at random.
   wari36_cmd_MM     the mode's lists: a pick is a number in this table.
   wari36_cmd_MM_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name
                    a list of lib36.c.
   wari36_unused_N   bytes that nothing in the file reaches. */
#include "player.h"
#include "pl36_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const wari36_cmd_02[], *const wari36_cmd_03[], *const wari36_cmd_05[],
	*const wari36_cmd_06[], *const wari36_cmd_07[], *const wari36_cmd_08[], *const wari36_cmd_09[],
	*const wari36_cmd_10[], *const wari36_cmd_11[], *const wari36_cmd_12[];
extern const Uint8 wari36_rate_02[], wari36_sel_02[], wari36_rate_03[], wari36_rate_04[],
	wari36_sel_03[], wari36_rate_05[], wari36_sel_05[], wari36_rate_06[], wari36_sel_06[],
	wari36_rate_07[], wari36_sel_07[], wari36_rate_08[], wari36_sel_08[], wari36_rate_09[],
	wari36_sel_09[], wari36_unused_0[], wari36_rate_10[], wari36_sel_10[], wari36_rate_11[],
	wari36_sel_11[], wari36_rate_12[], wari36_sel_12[], wari36_rate_13[], wari36_cmd_02_00[],
	wari36_cmd_02_01[], wari36_cmd_02_02[], wari36_cmd_02_03[], wari36_cmd_03_00[],
	wari36_cmd_05_00[], wari36_cmd_05_01[], wari36_cmd_05_02[], wari36_cmd_06_00[],
	wari36_cmd_07_00[], wari36_cmd_07_01[], wari36_cmd_07_02[], wari36_cmd_08_00[],
	wari36_cmd_09_00[], wari36_cmd_09_01[], wari36_cmd_09_02[], wari36_cmd_10_00[],
	wari36_cmd_10_01[], wari36_cmd_10_02[], wari36_cmd_11_00[], wari36_cmd_11_01[],
	wari36_cmd_11_02[], wari36_cmd_11_03[], wari36_cmd_12_00[], wari36_cmd_12_01[],
	wari36_cmd_12_02[], wari36_cmd_12_03[];

const CPU_DATA wari36_tbl[] = {
	{ ( Sint32 )wari36_rate_02, ( Sint32 )wari36_sel_02, ( Sint32 )wari36_cmd_02 },	/* 2: a throw would catch the opponent */
	{ ( Sint32 )wari36_rate_03, ( Sint32 )wari36_sel_03, ( Sint32 )wari36_cmd_03 },	/* 3: it guards a hit (the chance is the program's own) */
	{ ( Sint32 )wari36_rate_04, ( Sint32 )wari36_sel_03, ( Sint32 )wari36_cmd_03 },	/* 4: never started: a guard in the air is mode 3 */
	{ ( Sint32 )wari36_rate_05, ( Sint32 )wari36_sel_05, ( Sint32 )wari36_cmd_05 },	/* 5: after it has guarded on the ground */
	{ ( Sint32 )wari36_rate_06, ( Sint32 )wari36_sel_06, ( Sint32 )wari36_cmd_06 },	/* 6: after it has guarded in the air */
	{ ( Sint32 )wari36_rate_07, ( Sint32 )wari36_sel_07, ( Sint32 )wari36_cmd_07 },	/* 7: after it has been hit */
	{ ( Sint32 )wari36_rate_08, ( Sint32 )wari36_sel_08, ( Sint32 )wari36_cmd_08 },	/* 8: never started */
	{ ( Sint32 )wari36_rate_09, ( Sint32 )wari36_sel_09, ( Sint32 )wari36_cmd_09 },	/* 9: after it has got up */
	{ ( Sint32 )wari36_rate_10, ( Sint32 )wari36_sel_10, ( Sint32 )wari36_cmd_10 },	/* 10: the opponent dashes */
	{ ( Sint32 )wari36_rate_11, ( Sint32 )wari36_sel_11, ( Sint32 )wari36_cmd_11 },	/* 11: the opponent flies or super jumps */
	{ ( Sint32 )wari36_rate_12, ( Sint32 )wari36_sel_12, ( Sint32 )wari36_cmd_12 },	/* 12: the opponent jumps */
	{ ( Sint32 )wari36_rate_13, 0, 0 },	/* 13: an attack is coming: the chance alone, the lists are yok36.c's */
};

const Uint8 wari36_rate_02[] = {
	  4,   5,   6,   7,   9,  11,  11,  13,  13,  15,  15,  17,  17,  19,  19,  21,
	 21,  23,  23,  24,  24,  26,  26,  28,  28,  30,  30,  31,  31,  31,  31,  31,
};

const Uint8 wari36_sel_02[] = {
	  0,   0,   0,   0,   0,   0,   0,   3,   1,   1,   1,   1,   1,   1,   1,   3,
	  2,   2,   2,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,
};

const Uint8 *const wari36_cmd_02[] = {
	wari36_cmd_02_00, wari36_cmd_02_01, wari36_cmd_02_02, wari36_cmd_02_03,
};

const Uint8 wari36_rate_03[] = {
	  2,   2,   2,   2,   3,   3,   3,   3,   4,   4,   4,   4,   5,   5,   5,   5,
	  6,   6,   7,   7,   8,   8,   9,   9,  10,  10,  11,  11,  12,  13,  14,  15,
};

const Uint8 wari36_rate_04[] = {
	 16,  16,  17,  17,  18,  19,  20,  21,  22,  23,  24,  25,  25,  25,  25,  25,
	 25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,
};

const Uint8 wari36_sel_03[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const wari36_cmd_03[] = {
	wari36_cmd_03_00,
};

const Uint8 wari36_rate_05[] = {
	  4,   4,   4,   8,  12,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,
	 16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,
};

const Uint8 wari36_sel_05[] = {
	  0,   0,   0,   0,   0,   0,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
	  2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   1,   1,   1,   1,
};

const Uint8 *const wari36_cmd_05[] = {
	wari36_cmd_05_00, wari36_cmd_05_01, wari36_cmd_05_02,
};

const Uint8 wari36_rate_06[] = {
	  4,   4,   4,   8,  12,  16,  18,  20,  22,  24,  26,  28,  30,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari36_sel_06[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const wari36_cmd_06[] = {
	wari36_cmd_06_00,
};

const Uint8 wari36_rate_07[] = {
	  4,   4,   4,   8,  12,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,
	 16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,
};

const Uint8 wari36_sel_07[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
};

const Uint8 *const wari36_cmd_07[] = {
	wari36_cmd_07_00, wari36_cmd_07_01, wari36_cmd_07_02,
};

const Uint8 wari36_rate_08[] = {
	  4,   4,   4,   8,  12,  16,  18,  20,  22,  24,  26,  28,  30,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari36_sel_08[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const wari36_cmd_08[] = {
	wari36_cmd_08_00,
};

const Uint8 wari36_rate_09[] = {
	  6,   6,   8,   8,   8,  10,  12,  14,  16,  18,  20,  22,  24,  25,  26,  27,
	 28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari36_sel_09[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,
};

const Uint8 *const wari36_cmd_09[] = {
	wari36_cmd_09_00, wari36_cmd_09_01, wari36_cmd_09_02,
};

const Uint8 wari36_unused_0[] = {
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  2,   2,   2,   2,   2,   2,   2,   2,   1,   1,   1,   1,   1,   1,   1,   1,
	  4,   5,   0,   0,  19,   5,   0,   0,  34,   5,   0,   0,  31,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  49,   5,   0,   0,
};

const Uint8 wari36_rate_10[] = {
	  1,   2,   3,   4,   5,   6,   7,   7,   7,   8,   8,   8,   9,   9,   9,  10,
	 10,  10,  11,  12,  13,  14,  15,  16,  16,  16,  16,  16,  16,  16,  16,  16,
};

const Uint8 wari36_sel_10[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,
};

const Uint8 *const wari36_cmd_10[] = {
	wari36_cmd_10_00, wari36_cmd_10_01, wari36_cmd_10_02,
};

const Uint8 wari36_rate_11[] = {
	  4,   4,   4,   6,   6,   6,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
	  8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
};

const Uint8 wari36_sel_11[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   3,   3,   3,   3,   3,   3,   3,   3,
	  1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,
};

const Uint8 *const wari36_cmd_11[] = {
	wari36_cmd_11_00, wari36_cmd_11_01, wari36_cmd_11_02, wari36_cmd_11_03,
};

const Uint8 wari36_rate_12[] = {
	  4,   4,   4,   6,   6,   6,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
	  8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
};

const Uint8 wari36_sel_12[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   3,   3,   3,
	  1,   1,   1,   1,   1,   3,   3,   3,   2,   2,   2,   2,   2,   3,   3,   3,
};

const Uint8 *const wari36_cmd_12[] = {
	wari36_cmd_12_00, wari36_cmd_12_01, wari36_cmd_12_02, wari36_cmd_12_03,
};

const Uint8 wari36_rate_13[] = {
	 20,  20,  20,  20,  22,  24,  26,  28,  30,  30,  30,  31,  31,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	  1,   2,   3,   4,   5,   6,   7,   8,   9,   9,   9,   9,  10,  10,  10,  10,
	 11,  11,  12,  12,  13,  13,  14,  14,  15,  15,  16,  16,  17,  17,  18,  18,
	  1,   1,   1,   1,   2,   2,   2,   2,   3,   3,   4,   4,   5,   5,   6,   6,
	  7,   7,   8,   8,   9,   9,  10,  10,  11,  11,  12,  12,  13,  13,  14,  14,
	  4,   4,   4,   4,   5,   6,   7,   8,   9,   9,   9,   9,  10,  10,  10,  10,
	 11,  11,  11,  11,  12,  12,  12,  12,  13,  13,  13,  13,  14,  14,  14,  14,
};

const Uint8 wari36_cmd_02_00[] = {
	WAIT2( 8 ),
	IF_CATCH,
		NAG_VFLIP_ADJ( 0, LEVER_FORWARD ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari36_cmd_02_01[] = {
	WAIT2( 8 ),
	IF_CATCH,
		NAG_VFLIP_ADJ( 1, LEVER_FORWARD ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari36_cmd_02_02[] = {
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 wari36_cmd_02_03[] = {
	IF_RND( 16 ),
		JUMP_RND( 5, 6, 2, 4 ),
	CMD_ELSE,
		JUMP_RND( 3, 7, 8, 9 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari36_cmd_03_00[] = {
	GUARD_CONT( 16 ),
	CMD_EXIT,
};

const Uint8 wari36_cmd_05_00[] = {
	IF_LEVEL( 16 ),
		VCOUNTER_SET( 3 ),
	CMD_ELSE,
		GUARD( 30 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari36_cmd_05_01[] = {
	GUARD( 30 ),
	CMD_EXIT,
};

const Uint8 wari36_cmd_05_02[] = {
	IF_RND( 16 ),
		JUMP_RND( 5, 6, 2, 4 ),
	CMD_ELSE,
		JUMP_RND( 3, 7, 8, 9 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari36_cmd_06_00[] = {
	TIMER2_SET( 60 ),
	CMD_EXIT,
};

const Uint8 wari36_cmd_07_00[] = {
	GUARD( 60 ),
	CMD_EXIT,
};

const Uint8 wari36_cmd_07_01[] = {
	GUARD( 30 ),
	CMD_EXIT,
};

const Uint8 wari36_cmd_07_02[] = {
	TIMER2_SET( 30 ),
	CMD_EXIT,
};

const Uint8 wari36_cmd_08_00[] = {
	TIMER2_SET( 60 ),
	CMD_EXIT,
};

const Uint8 wari36_cmd_09_00[] = {
	GUARD( 45 ),
	CMD_EXIT,
};

const Uint8 wari36_cmd_09_01[] = {
	GUARD( 30 ),
	CMD_EXIT,
};

const Uint8 wari36_cmd_09_02[] = {
	WAIT2( 2 ),
	CMD_EXIT,
	IF_RND( 16 ),
		JUMP_RND( 5, 6, 2, 4 ),
	CMD_ELSE,
		JUMP_RND( 3, 7, 8, 9 ),
	CMD_ENDIF,
	CMD_EXIT,
	IF_RND( 16 ),
		CMD_JUMP( 0 ),
	CMD_ELSE,
		SP_CMD( pl36_sp_chouhatsu, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_JUMP( 0 ),
	CMD_ENDIF,
	CMD_EXIT,
	IF_RND( 16 ),
		CMD_JUMP( 1 ),
	CMD_ELSE,
		SP_CMD( pl36_sp_chouhatsu, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_JUMP( 1 ),
	CMD_ENDIF,
	CMD_EXIT,
	WAIT( 1 ),
	CMD_EXIT,
};

const Uint8 wari36_cmd_10_00[] = {
	IF_RND( 16 ),
		JUMP_RND( 5, 6, 2, 4 ),
	CMD_ELSE,
		JUMP_RND( 3, 7, 8, 9 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari36_cmd_10_01[] = {
	IF_GAUGE( 1 ),
		CMD_JUMP( 1 ),
	CMD_ELSE,
		WAIT2( 8 ),
		SP_CMD( pl36_sp_sphere, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari36_cmd_10_02[] = {
	IF_GAUGE( 1 ),
		CMD_JUMP( 1 ),
	CMD_ELSE,
		WAIT2( 8 ),
		SP_CMD( pl36_sp_titan, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari36_cmd_11_00[] = {
	SJUMP( DIR_UP ),
	SJUMP_FLAG_SET,
		TIMER_SET( 120 ),
		IF_AREA( 128, 32, 128, 0 ),
			WAIT2( 8 ),
			SP_CMD( pl36_sp_titan, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ELSE,
			GUARD( 30 ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari36_cmd_11_01[] = {
	IF_GAUGE( 1 ),
		CMD_JUMP( 0 ),
	CMD_ELSE,
		TIMER_SET( 120 ),
		IF_DIST( 80, 64, 0, 64 ),
			WAIT2( 8 ),
			STAND,
			NAG_VFLIP_ADJ4( LV_HEAVY, LEVER_BACK ),
		CMD_ELSE,
			GUARD( 30 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari36_cmd_11_02[] = {
	IF_RND( 16 ),
		CMD_JUMP( 19 ),
		CMD_JUMP( 3 ),
	CMD_ELSE,
		SJUMP( DIR_UP ),
		SJUMP_FLAG_SET,
			TIMER_SET( 120 ),
			IF_EM_ATCK( ATCK_JUMP_HP ),
				WAIT2( 8 ),
				PUNCH( LV_HEAVY ),
			CMD_ELSE,
				GUARD( 30 ),
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari36_cmd_11_03[] = {
	IF_RND( 24 ),
		WAIT2( 8 ),
	CMD_ELSE,
		CMD_JUMP( 18 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari36_cmd_12_00[] = {
	IF_GAUGE( 1 ),
		CMD_JUMP( 0 ),
	CMD_ELSE,
		TIMER_SET( 120 ),
		IF_DIST( 80, 64, 0, 64 ),
			WAIT2( 8 ),
			STAND,
			NAG_VFLIP_ADJ4( LV_HEAVY, LEVER_BACK ),
		CMD_ELSE,
			GUARD( 30 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari36_cmd_12_01[] = {
	IF_RND( 16 ),
		CMD_JUMP( 19 ),
		CMD_JUMP( 3 ),
	CMD_ELSE,
		JUMP( DIR_UP ),
		JUMP_FLAG_SET,
			TIMER_SET( 120 ),
			IF_EM_ATCK( ATCK_JUMP_HP ),
				WAIT2( 8 ),
				PUNCH( LV_HEAVY ),
			CMD_ELSE,
				GUARD( 30 ),
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari36_cmd_12_02[] = {
	JUMP( DIR_UP ),
	JUMP_FLAG_SET,
		TIMER_SET( 120 ),
		IF_AREA( 128, 32, 128, 0 ),
			WAIT2( 8 ),
			SP_CMD( pl36_sp_titan, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ELSE,
			GUARD( 30 ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari36_cmd_12_03[] = {
	IF_RND( 28 ),
		JUMP_RND( 4, 4, 18, 18 ),
	CMD_ELSE,
		WAIT2( 8 ),
	CMD_ENDIF,
	CMD_EXIT,
};

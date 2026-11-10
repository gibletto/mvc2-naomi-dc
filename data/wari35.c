/* Blackheart (character 35): the computer's lists for cutting in.
   What it does when something happens in the fight. PLAYER_WORK's cpu_adrs[2] points here; the
   event checks of em_play.c start the modes.

   wari35_tbl        a record (CPU_DATA) for each mode from 2 on: the offsets of its tables.
   wari35_rate_MM    the chance, out of 32, that mode MM is started, by the level of play.
   wari35_sel_MM     32 picks; one is taken at random.
   wari35_cmd_MM     the mode's lists: a pick is a number in this table.
   wari35_cmd_MM_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name
                    a list of lib35.c.
   wari35_unused_N   bytes that nothing in the file reaches. */
#include "player.h"
#include "pl35_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const wari35_cmd_02[], *const wari35_cmd_03[], *const wari35_cmd_05[],
	*const wari35_cmd_06[], *const wari35_cmd_07[], *const wari35_cmd_08[], *const wari35_cmd_09[],
	*const wari35_cmd_10[], *const wari35_cmd_11[], *const wari35_cmd_12[];
extern const Uint8 wari35_rate_02[], wari35_sel_02[], wari35_rate_03[], wari35_rate_04[],
	wari35_sel_03[], wari35_rate_05[], wari35_sel_05[], wari35_rate_06[], wari35_sel_06[],
	wari35_rate_07[], wari35_sel_07[], wari35_rate_08[], wari35_sel_08[], wari35_rate_09[],
	wari35_sel_09[], wari35_unused_0[], wari35_rate_10[], wari35_sel_10[], wari35_rate_11[],
	wari35_sel_11[], wari35_rate_12[], wari35_sel_12[], wari35_rate_13[], wari35_cmd_02_00[],
	wari35_cmd_02_01[], wari35_cmd_02_02[], wari35_cmd_03_00[], wari35_cmd_05_00[],
	wari35_cmd_05_01[], wari35_cmd_05_02[], wari35_cmd_06_00[], wari35_cmd_07_00[],
	wari35_cmd_07_01[], wari35_cmd_07_02[], wari35_cmd_08_00[], wari35_cmd_09_00[],
	wari35_cmd_09_01[], wari35_cmd_09_02[], wari35_cmd_10_00[], wari35_cmd_10_01[],
	wari35_cmd_10_02[], wari35_cmd_11_00[], wari35_cmd_11_01[], wari35_cmd_11_02[],
	wari35_cmd_12_00[], wari35_cmd_12_01[], wari35_cmd_12_02[], wari35_cmd_12_03[];

const CPU_DATA wari35_tbl[] = {
	{ ( Sint32 )wari35_rate_02, ( Sint32 )wari35_sel_02, ( Sint32 )wari35_cmd_02 },	/* 2: a throw would catch the opponent */
	{ ( Sint32 )wari35_rate_03, ( Sint32 )wari35_sel_03, ( Sint32 )wari35_cmd_03 },	/* 3: it guards a hit (the chance is the program's own) */
	{ ( Sint32 )wari35_rate_04, ( Sint32 )wari35_sel_03, ( Sint32 )wari35_cmd_03 },	/* 4: never started: a guard in the air is mode 3 */
	{ ( Sint32 )wari35_rate_05, ( Sint32 )wari35_sel_05, ( Sint32 )wari35_cmd_05 },	/* 5: after it has guarded on the ground */
	{ ( Sint32 )wari35_rate_06, ( Sint32 )wari35_sel_06, ( Sint32 )wari35_cmd_06 },	/* 6: after it has guarded in the air */
	{ ( Sint32 )wari35_rate_07, ( Sint32 )wari35_sel_07, ( Sint32 )wari35_cmd_07 },	/* 7: after it has been hit */
	{ ( Sint32 )wari35_rate_08, ( Sint32 )wari35_sel_08, ( Sint32 )wari35_cmd_08 },	/* 8: never started */
	{ ( Sint32 )wari35_rate_09, ( Sint32 )wari35_sel_09, ( Sint32 )wari35_cmd_09 },	/* 9: after it has got up */
	{ ( Sint32 )wari35_rate_10, ( Sint32 )wari35_sel_10, ( Sint32 )wari35_cmd_10 },	/* 10: the opponent dashes */
	{ ( Sint32 )wari35_rate_11, ( Sint32 )wari35_sel_11, ( Sint32 )wari35_cmd_11 },	/* 11: the opponent flies or super jumps */
	{ ( Sint32 )wari35_rate_12, ( Sint32 )wari35_sel_12, ( Sint32 )wari35_cmd_12 },	/* 12: the opponent jumps */
	{ ( Sint32 )wari35_rate_13, 0, 0 },	/* 13: an attack is coming: the chance alone, the lists are yok35.c's */
};

const Uint8 wari35_rate_02[] = {
	  4,   5,   6,   7,   9,  11,  11,  13,  13,  15,  15,  17,  17,  19,  19,  21,
	 21,  23,  23,  24,  24,  26,  26,  28,  28,  30,  30,  31,  31,  31,  31,  31,
};

const Uint8 wari35_sel_02[] = {
	  0,   0,   0,   1,   1,   1,   2,   2,   0,   0,   0,   1,   1,   1,   2,   2,
	  0,   0,   0,   1,   1,   2,   2,   2,   0,   0,   0,   1,   1,   2,   2,   2,
};

const Uint8 *const wari35_cmd_02[] = {
	wari35_cmd_02_00, wari35_cmd_02_01, wari35_cmd_02_02,
};

const Uint8 wari35_rate_03[] = {
	  1,   2,   3,   4,   4,   4,   4,   4,   4,   4,   6,   6,   6,   6,   8,   8,
	  8,   8,   8,  10,  10,  10,  12,  12,  12,  12,  12,  12,  12,  12,  12,  18,
};

const Uint8 wari35_rate_04[] = {
	 12,  12,  13,  13,  14,  15,  16,  17,  18,  19,  22,  21,  21,  21,  21,  21,
	 21,  21,  21,  21,  21,  21,  21,  21,  21,  21,  21,  21,  21,  21,  21,  21,
};

const Uint8 wari35_sel_03[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const wari35_cmd_03[] = {
	wari35_cmd_03_00,
};

const Uint8 wari35_rate_05[] = {
	 12,  14,  16,  20,  21,  21,  22,  22,  23,  23,  24,  24,  25,  25,  26,  26,
	 27,  27,  28,  28,  29,  29,  30,  30,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari35_sel_05[] = {
	  0,   0,   0,   1,   1,   1,   2,   2,   0,   0,   0,   1,   1,   1,   2,   2,
	  0,   0,   0,   1,   1,   1,   2,   2,   0,   0,   0,   1,   1,   1,   2,   2,
};

const Uint8 *const wari35_cmd_05[] = {
	wari35_cmd_05_00, wari35_cmd_05_01, wari35_cmd_05_02,
};

const Uint8 wari35_rate_06[] = {
	  4,   4,   4,   8,  12,  16,  18,  20,  22,  24,  26,  28,  30,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari35_sel_06[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const wari35_cmd_06[] = {
	wari35_cmd_06_00,
};

const Uint8 wari35_rate_07[] = {
	 12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,
	 12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  14,  14,  14,  16,
};

const Uint8 wari35_sel_07[] = {
	  0,   0,   0,   1,   1,   1,   2,   2,   0,   0,   0,   1,   1,   1,   2,   2,
	  0,   0,   0,   1,   1,   1,   2,   2,   0,   0,   0,   1,   1,   1,   2,   2,
};

const Uint8 *const wari35_cmd_07[] = {
	wari35_cmd_07_00, wari35_cmd_07_01, wari35_cmd_07_02,
};

const Uint8 wari35_rate_08[] = {
	  4,   4,   4,   8,  12,  16,  18,  20,  22,  24,  26,  28,  30,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari35_sel_08[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const wari35_cmd_08[] = {
	wari35_cmd_08_00,
};

const Uint8 wari35_rate_09[] = {
	 24,  25,  26,  26,  26,  27,  27,  27,  28,  28,  29,  29,  30,  30,  31,  31,
	 28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari35_sel_09[] = {
	  0,   0,   0,   0,   1,   1,   1,   2,   0,   0,   0,   0,   1,   1,   1,   2,
	  0,   0,   0,   0,   1,   1,   1,   2,   0,   0,   0,   0,   1,   1,   1,   2,
};

const Uint8 *const wari35_cmd_09[] = {
	wari35_cmd_09_00, wari35_cmd_09_01, wari35_cmd_09_02,
};

const Uint8 wari35_unused_0[] = {
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	  0,   0,   1,   1,   2,   2,   3,   3,   0,   0,   1,   1,   2,   2,   3,   3,
	  0,   0,   1,   1,   2,   2,   3,   3,   0,   0,   1,   1,   2,   2,   3,   3,
	 65,   6,   0,   0,  68,   6,   0,   0,  81,   6,   0,   0, 128,   6,   0,   0,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	137,   6,   0,   0,
};

const Uint8 wari35_rate_10[] = {
	  3,   4,   5,   6,   7,   8,   9,  10,  11,  11,  11,  11,  11,  11,  11,  11,
	 12,  12,  13,  13,  14,  14,  16,  16,  18,  18,  20,  20,  22,  22,  24,  24,
};

const Uint8 wari35_sel_10[] = {
	  0,   0,   0,   1,   1,   1,   2,   2,   0,   0,   0,   1,   1,   1,   2,   2,
	  0,   0,   0,   1,   1,   1,   2,   2,   0,   0,   0,   1,   1,   1,   2,   2,
};

const Uint8 *const wari35_cmd_10[] = {
	wari35_cmd_10_00, wari35_cmd_10_01, wari35_cmd_10_02,
};

const Uint8 wari35_rate_11[] = {
	  8,   8,   8,   9,   9,   9,  10,  10,  10,  11,  11,  11,  12,  12,  12,  13,
	 13,  13,  14,  14,  14,  16,  16,  16,  18,  18,  18,  19,  19,  19,  20,  20,
};

const Uint8 wari35_sel_11[] = {
	  0,   0,   0,   1,   1,   1,   2,   2,   0,   0,   0,   1,   1,   1,   2,   2,
	  0,   0,   0,   1,   1,   1,   2,   2,   0,   0,   0,   1,   1,   1,   2,   2,
};

const Uint8 *const wari35_cmd_11[] = {
	wari35_cmd_11_00, wari35_cmd_11_01, wari35_cmd_11_02,
};

const Uint8 wari35_rate_12[] = {
	  6,   6,   7,   7,   8,   8,   8,   9,   9,   9,  10,  10,  10,  11,  11,  11,
	 12,  12,  12,  13,  13,  13,  14,  14,  14,  15,  15,  15,  16,  16,  16,  18,
};

const Uint8 wari35_sel_12[] = {
	  0,   0,   0,   1,   1,   1,   2,   2,   0,   0,   0,   1,   1,   1,   2,   2,
	  0,   0,   0,   1,   1,   1,   2,   2,   0,   0,   0,   1,   1,   1,   2,   2,
};

const Uint8 *const wari35_cmd_12[] = {
	wari35_cmd_12_00, wari35_cmd_12_01, wari35_cmd_12_02, wari35_cmd_12_03,
};

const Uint8 wari35_rate_13[] = {
	 28,  28,  30,  30,  30,  30,  30,  30,  31,  31,  31,  31,  31,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	  4,   4,   4,   4,   6,   6,   6,   6,   8,   8,   8,   8,  10,  10,  10,  10,
	 12,  12,  12,  12,  14,  14,  14,  14,  16,  16,  16,  16,  18,  18,  18,  18,
	  2,   2,   2,   2,   3,   3,   3,   3,   4,   4,   4,   4,   5,   5,   5,   5,
	  6,   6,   6,   6,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,  10,  10,
	  2,   2,   2,   2,   4,   4,   4,   4,   6,   6,   6,   8,   8,   8,   8,  10,
	 10,  10,  12,  12,  12,  14,  14,  14,  16,  16,  18,  18,  20,  20,  22,  24,
};

const Uint8 wari35_cmd_02_00[] = {
	IF_LEVEL( 29 ),
		IF_CATCH,
			IF_RND( 24 ),
				IF_RND( 16 ),
					NAG_VFLIP_ADJ2( 1, LEVER_FORWARD ),
				CMD_ELSE,
					NAG_VFLIP_ADJ( 1, LEVER_FORWARD ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			PUNCH( LV_LIGHT ),
			PUNCH( LV_LIGHT ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 22 ),
			IF_CATCH,
				IF_RND( 20 ),
					NAG_VFLIP_ADJ2( 1, LEVER_FORWARD ),
				CMD_ELSE,
					NAG_VFLIP_ADJ( 1, LEVER_FORWARD ),
				CMD_ENDIF,
			CMD_ELSE,
				PUNCH( LV_LIGHT ),
				PUNCH( LV_LIGHT ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 14 ),
				IF_CATCH,
					IF_RND( 16 ),
						IF_RND( 16 ),
							NAG_VFLIP_ADJ2( 1, LEVER_FORWARD ),
						CMD_ELSE,
							NAG_VFLIP_ADJ( 1, LEVER_FORWARD ),
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ELSE,
					PUNCH( LV_LIGHT ),
					PUNCH( LV_LIGHT ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 8 ),
					IF_CATCH,
						IF_RND( 12 ),
							IF_RND( 16 ),
								NAG_VFLIP_ADJ2( 1, LEVER_FORWARD ),
							CMD_ELSE,
								NAG_VFLIP_ADJ( 1, LEVER_FORWARD ),
							CMD_ENDIF,
						CMD_ENDIF,
					CMD_ELSE,
						PUNCH( LV_LIGHT ),
						PUNCH( LV_LIGHT ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_CATCH,
						IF_RND( 8 ),
							IF_RND( 16 ),
								NAG_VFLIP_ADJ2( 1, LEVER_FORWARD ),
							CMD_ELSE,
								NAG_VFLIP_ADJ( 1, LEVER_FORWARD ),
							CMD_ENDIF,
						CMD_ENDIF,
					CMD_ELSE,
						PUNCH( LV_LIGHT ),
						PUNCH( LV_LIGHT ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari35_cmd_02_01[] = {
	IF_LEVEL( 30 ),
		IF_RND( 26 ),
			PUNCH( LV_LIGHT ),
			PUNCH( LV_LIGHT ),
			CROUCH,
			PUNCH( LV_HEAVY ),
			STAND,
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 18 ),
			IF_RND( 20 ),
				PUNCH( LV_LIGHT ),
				PUNCH( LV_LIGHT ),
				CROUCH,
				PUNCH( LV_HEAVY ),
				STAND,
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 14 ),
				IF_RND( 16 ),
					PUNCH( LV_LIGHT ),
					PUNCH( LV_LIGHT ),
					CROUCH,
					PUNCH( LV_HEAVY ),
					STAND,
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 8 ),
					IF_RND( 12 ),
						PUNCH( LV_LIGHT ),
						PUNCH( LV_LIGHT ),
						CROUCH,
						PUNCH( LV_HEAVY ),
						STAND,
					CMD_ENDIF,
				CMD_ELSE,
					IF_RND( 8 ),
						PUNCH( LV_LIGHT ),
						PUNCH( LV_LIGHT ),
						CROUCH,
						PUNCH( LV_HEAVY ),
						STAND,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari35_cmd_02_02[] = {
	IF_LEVEL( 28 ),
		IF_RND( 24 ),
			CROUCH,
			KICK( LV_LIGHT ),
			KICK( LV_LIGHT ),
			KICK( LV_LIGHT ),
			PUNCH( LV_HEAVY ),
			STAND,
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 24 ),
			IF_RND( 20 ),
				CROUCH,
				KICK( LV_LIGHT ),
				KICK( LV_LIGHT ),
				KICK( LV_LIGHT ),
				PUNCH( LV_HEAVY ),
				STAND,
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 12 ),
				IF_RND( 16 ),
					CROUCH,
					KICK( LV_LIGHT ),
					KICK( LV_LIGHT ),
					KICK( LV_LIGHT ),
					PUNCH( LV_HEAVY ),
					STAND,
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 8 ),
					IF_RND( 12 ),
						CROUCH,
						KICK( LV_LIGHT ),
						KICK( LV_LIGHT ),
						KICK( LV_LIGHT ),
						PUNCH( LV_HEAVY ),
						STAND,
					CMD_ENDIF,
				CMD_ELSE,
					IF_RND( 8 ),
						CROUCH,
						KICK( LV_LIGHT ),
						KICK( LV_LIGHT ),
						KICK( LV_LIGHT ),
						PUNCH( LV_HEAVY ),
						STAND,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari35_cmd_03_00[] = {
	GUARD_CONT( 16 ),
	CMD_EXIT,
};

const Uint8 wari35_cmd_05_00[] = {
	GUARD( 30 ),
	CMD_EXIT,
};

const Uint8 wari35_cmd_05_01[] = {
	GUARD( 60 ),
	CMD_EXIT,
};

const Uint8 wari35_cmd_05_02[] = {
	TIMER2_SET( 60 ),
	CMD_EXIT,
};

const Uint8 wari35_cmd_06_00[] = {
	TIMER2_SET( 60 ),
	CMD_EXIT,
};

const Uint8 wari35_cmd_07_00[] = {
	VCOUNTER_SET( 6 ),
	GUARD( 30 ),
	CMD_EXIT,
};

const Uint8 wari35_cmd_07_01[] = {
	VCOUNTER_SET( 6 ),
	GUARD( 60 ),
	CMD_EXIT,
};

const Uint8 wari35_cmd_07_02[] = {
	VCOUNTER_SET( 6 ),
	TIMER2_SET( 60 ),
	CMD_EXIT,
};

const Uint8 wari35_cmd_08_00[] = {
	TIMER2_SET( 60 ),
	CMD_EXIT,
};

const Uint8 wari35_cmd_09_00[] = {
	GUARD( 30 ),
	CMD_EXIT,
};

const Uint8 wari35_cmd_09_01[] = {
	IF_LEVEL( 30 ),
		GUARD( 30 ),
	CMD_ELSE,
		IF_LEVEL( 26 ),
			IF_RND( 28 ),
				GUARD( 30 ),
			CMD_ELSE,
				CMD_JUMP( 17 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 17 ),
				IF_RND( 24 ),
					GUARD( 30 ),
				CMD_ELSE,
					CMD_JUMP( 17 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 10 ),
					IF_RND( 18 ),
						GUARD( 30 ),
					CMD_ELSE,
						CMD_JUMP( 17 ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_LEVEL( 2 ),
						IF_RND( 12 ),
							GUARD( 30 ),
						CMD_ELSE,
							CMD_JUMP( 17 ),
						CMD_ENDIF,
					CMD_ELSE,
						IF_RND( 8 ),
							GUARD( 30 ),
						CMD_ELSE,
							CMD_JUMP( 17 ),
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari35_cmd_09_02[] = {
	IF_LEVEL( 28 ),
		IF_DIST( 40, 32, 32, 40 ),
			CANCEL_SET2,
				STAND,
				PUNCH( LV_LIGHT ),
				IF_GAUGE( 2 ),
					WAIT2( 2 ),
					SP_CMD( pl35_sp_heart, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ELSE,
					SP_CMD( pl35_sp_inferno, 0, PK_PUNCH, LV_HEAVY, 0 ),
				CMD_ENDIF,
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			DASH_B( 384 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
	CMD_JUMP( 12 ),
	CMD_EXIT,
	SP_CMD( pl35_sp_chouhatsu, 0, PK_PUNCH, LV_LIGHT, 0 ),
	SP_CMD( pl35_sp_thunder, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_EXIT,
	IF_GAUGE( 1 ),
		IF_DIST( 128, 96, 0, 0 ),
			SP_CMD( pl35_sp_heart, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			SP_CMD( pl35_sp_judgement, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_DIST( 128, 192, 64, 0 ),
			KICK( LV_HEAVY ),
			WAIT( 40 ),
			KICK( LV_HEAVY ),
		CMD_ELSE,
			PUNCH( LV_HEAVY ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
	WAIT( 10 ),
	SP_CMD( pl35_sp_chouhatsu, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_EXIT,
	CMD_EXIT,
};

const Uint8 wari35_cmd_10_00[] = {
	VCOUNTER_SET( 6 ),
	TIMER2_SET( 60 ),
	JUMP( DIR_UP ),
	JUMP_FLAG_SET,
		TIMER_SET( 60 ),
		IF_HEIGHT( 0xff, 72 ),
			FLY( LEVER_FORWARD, 512 ),
		CMD_ELSE,
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ELSE,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari35_cmd_10_01[] = {
	VCOUNTER_SET( 6 ),
	IF_DIST( 0, 0, 96, 0 ),
		DASH_F( 512 ),
	CMD_ELSE,
		TIMER2_SET( 60 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari35_cmd_10_02[] = {
	VCOUNTER_SET( 6 ),
	GUARD( 30 ),
	CMD_EXIT,
};

const Uint8 wari35_cmd_11_00[] = {
	VCOUNTER_SET( 6 ),
	WAIT2( 14 ),
	IF_RND( 24 ),
		SJUMP( DIR_UP ),
		SJUMP_FLAG_SET,
			TIMER2_SET( 80 ),
			TIMER_SET( 80 ),
			IF_CATCH,
				IF_RND( 24 ),
					IF_RND( 16 ),
						NAG_VFLIP_ADJ( 1, LEVER_FORWARD ),
					CMD_ELSE,
						NAG_VFLIP_ADJ2( 1, LEVER_FORWARD ),
					CMD_ENDIF,
				CMD_ELSE,
					WAIT( 10 ),
				CMD_ENDIF,
			CMD_ELSE,
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ELSE,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari35_cmd_11_01[] = {
	WAIT2( 14 ),
	DASH_F( 512 ),
	WAIT2( 14 ),
	DASH_F( 256 ),
	CMD_EXIT,
};

const Uint8 wari35_cmd_11_02[] = {
	IF_RND( 16 ),
		CMD_JUMP( 12 ),
	CMD_ELSE,
		CMD_JUMP( 13 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari35_cmd_12_00[] = {
	VCOUNTER_SET( 6 ),
	WAIT2( 14 ),
	WAIT2( 12 ),
	JUMP( DIR_BACK ),
	JUMP_FLAG_SET,
		TIMER2_SET( 60 ),
		WAIT( 20 ),
		TIMER_SET( 80 ),
		IF_CATCH,
			NAG_VFLIP_ADJ( 1, LEVER_FORWARD ),
		CMD_ELSE,
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ELSE,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari35_cmd_12_01[] = {
	VCOUNTER_SET( 6 ),
	WAIT( 10 ),
	WAIT2( 14 ),
	WAIT2( 12 ),
	JUMP( DIR_UP ),
	JUMP_FLAG_SET,
		TIMER2_SET( 60 ),
		WAIT( 20 ),
		TIMER_SET( 80 ),
		IF_CATCH,
			NAG_VFLIP_ADJ( 1, LEVER_FORWARD ),
		CMD_ELSE,
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ELSE,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari35_cmd_12_02[] = {
	IF_RND( 16 ),
		DASH_F( 512 ),
	CMD_ELSE,
		DASH_B( 512 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari35_cmd_12_03[] = {
	IF_LEVEL( 24 ),
		CMD_JUMP( 12 ),
	CMD_ELSE,
		IF_RND( 4 ),
			CMD_JUMP( 12 ),
		CMD_ELSE,
			TIMER_SET( 40 ),
			WALK_DIST( 192 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

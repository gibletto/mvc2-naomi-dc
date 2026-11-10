/* Charlie (character 21): the computer's lists for cutting in.
   What it does when something happens in the fight. PLAYER_WORK's cpu_adrs[2] points here; the
   event checks of em_play.c start the modes.

   wari21_tbl        a record (CPU_DATA) for each mode from 2 on: the offsets of its tables.
   wari21_rate_MM    the chance, out of 32, that mode MM is started, by the level of play.
   wari21_sel_MM     32 picks; one is taken at random.
   wari21_cmd_MM     the mode's lists: a pick is a number in this table.
   wari21_cmd_MM_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name
                    a list of lib21.c.
   wari21_unused_N   bytes that nothing in the file reaches. */
#include "player.h"
#include "pl21_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const wari21_cmd_02[], *const wari21_cmd_03[], *const wari21_cmd_05[],
	*const wari21_cmd_06[], *const wari21_cmd_07[], *const wari21_cmd_08[], *const wari21_cmd_09[],
	*const wari21_cmd_10[], *const wari21_cmd_11[], *const wari21_cmd_12[];
extern const Uint8 wari21_rate_02[], wari21_sel_02[], wari21_rate_03[], wari21_rate_04[],
	wari21_sel_03[], wari21_rate_05[], wari21_sel_05[], wari21_rate_06[], wari21_sel_06[],
	wari21_rate_07[], wari21_sel_07[], wari21_rate_08[], wari21_sel_08[], wari21_rate_09[],
	wari21_sel_09[], wari21_unused_0[], wari21_rate_10[], wari21_sel_10[], wari21_rate_11[],
	wari21_sel_11[], wari21_rate_12[], wari21_sel_12[], wari21_rate_13[], wari21_cmd_02_00[],
	wari21_cmd_02_01[], wari21_cmd_02_02[], wari21_cmd_03_00[], wari21_cmd_05_00[],
	wari21_cmd_05_01[], wari21_cmd_05_02[], wari21_cmd_06_00[], wari21_cmd_07_00[],
	wari21_cmd_07_01[], wari21_cmd_07_02[], wari21_cmd_08_00[], wari21_cmd_09_00[],
	wari21_cmd_09_01[], wari21_cmd_09_02[], wari21_cmd_10_00[], wari21_cmd_10_01[],
	wari21_cmd_10_02[], wari21_cmd_11_00[], wari21_cmd_11_01[], wari21_cmd_11_02[],
	wari21_cmd_12_00[], wari21_cmd_12_01[], wari21_cmd_12_02[], wari21_cmd_12_03[];

const CPU_DATA wari21_tbl[] = {
	{ ( Sint32 )wari21_rate_02, ( Sint32 )wari21_sel_02, ( Sint32 )wari21_cmd_02 },	/* 2: a throw would catch the opponent */
	{ ( Sint32 )wari21_rate_03, ( Sint32 )wari21_sel_03, ( Sint32 )wari21_cmd_03 },	/* 3: it guards a hit (the chance is the program's own) */
	{ ( Sint32 )wari21_rate_04, ( Sint32 )wari21_sel_03, ( Sint32 )wari21_cmd_03 },	/* 4: never started: a guard in the air is mode 3 */
	{ ( Sint32 )wari21_rate_05, ( Sint32 )wari21_sel_05, ( Sint32 )wari21_cmd_05 },	/* 5: after it has guarded on the ground */
	{ ( Sint32 )wari21_rate_06, ( Sint32 )wari21_sel_06, ( Sint32 )wari21_cmd_06 },	/* 6: after it has guarded in the air */
	{ ( Sint32 )wari21_rate_07, ( Sint32 )wari21_sel_07, ( Sint32 )wari21_cmd_07 },	/* 7: after it has been hit */
	{ ( Sint32 )wari21_rate_08, ( Sint32 )wari21_sel_08, ( Sint32 )wari21_cmd_08 },	/* 8: never started */
	{ ( Sint32 )wari21_rate_09, ( Sint32 )wari21_sel_09, ( Sint32 )wari21_cmd_09 },	/* 9: after it has got up */
	{ ( Sint32 )wari21_rate_10, ( Sint32 )wari21_sel_10, ( Sint32 )wari21_cmd_10 },	/* 10: the opponent dashes */
	{ ( Sint32 )wari21_rate_11, ( Sint32 )wari21_sel_11, ( Sint32 )wari21_cmd_11 },	/* 11: the opponent flies or super jumps */
	{ ( Sint32 )wari21_rate_12, ( Sint32 )wari21_sel_12, ( Sint32 )wari21_cmd_12 },	/* 12: the opponent jumps */
	{ ( Sint32 )wari21_rate_13, 0, 0 },	/* 13: an attack is coming: the chance alone, the lists are yok21.c's */
};

const Uint8 wari21_rate_02[] = {
	  4,   5,   6,   7,   9,  11,  11,  13,  13,  15,  15,  17,  17,  19,  19,  21,
	 21,  23,  23,  24,  24,  26,  26,  28,  28,  30,  30,  31,  31,  31,  31,  31,
};

const Uint8 wari21_sel_02[] = {
	  0,   0,   0,   1,   1,   1,   2,   2,   0,   0,   0,   1,   1,   1,   2,   2,
	  0,   0,   0,   1,   1,   1,   2,   2,   0,   0,   0,   1,   1,   1,   2,   2,
};

const Uint8 *const wari21_cmd_02[] = {
	wari21_cmd_02_00, wari21_cmd_02_01, wari21_cmd_02_02,
};

const Uint8 wari21_rate_03[] = {
	  1,   1,   1,   1,   1,   2,   3,   4,   4,   4,   5,   5,   6,   6,   7,   7,
	  8,   8,  10,  10,  11,  11,  12,  12,  14,  14,  15,  15,  15,  16,  16,  16,
};

const Uint8 wari21_rate_04[] = {
	 12,  12,  13,  13,  14,  15,  16,  17,  18,  19,  20,  21,  21,  21,  21,  21,
	 21,  21,  21,  21,  21,  21,  21,  21,  21,  21,  21,  21,  21,  21,  21,  21,
};

const Uint8 wari21_sel_03[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const wari21_cmd_03[] = {
	wari21_cmd_03_00,
};

const Uint8 wari21_rate_05[] = {
	 12,  14,  16,  20,  21,  21,  22,  22,  23,  23,  24,  24,  25,  25,  26,  26,
	 27,  27,  28,  28,  29,  29,  30,  30,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari21_sel_05[] = {
	  0,   0,   0,   1,   1,   1,   2,   2,   0,   0,   0,   1,   1,   1,   2,   2,
	  0,   0,   0,   1,   1,   1,   2,   2,   0,   0,   0,   1,   1,   1,   2,   2,
};

const Uint8 *const wari21_cmd_05[] = {
	wari21_cmd_05_00, wari21_cmd_05_01, wari21_cmd_05_02,
};

const Uint8 wari21_rate_06[] = {
	  4,   4,   4,   8,  12,  16,  18,  20,  22,  24,  26,  28,  30,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari21_sel_06[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const wari21_cmd_06[] = {
	wari21_cmd_06_00,
};

const Uint8 wari21_rate_07[] = {
	 12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,
	 12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  16,  16,
};

const Uint8 wari21_sel_07[] = {
	  0,   0,   0,   1,   1,   1,   2,   2,   0,   0,   0,   1,   1,   1,   2,   2,
	  0,   0,   0,   1,   1,   1,   2,   2,   0,   0,   0,   1,   1,   1,   2,   2,
};

const Uint8 *const wari21_cmd_07[] = {
	wari21_cmd_07_00, wari21_cmd_07_01, wari21_cmd_07_02,
};

const Uint8 wari21_rate_08[] = {
	  4,   4,   4,   8,  12,  16,  18,  20,  22,  24,  26,  28,  30,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari21_sel_08[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const wari21_cmd_08[] = {
	wari21_cmd_08_00,
};

const Uint8 wari21_rate_09[] = {
	 24,  24,  24,  25,  25,  25,  26,  26,  26,  27,  27,  28,  28,  29,  29,  30,
	 30,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 wari21_sel_09[] = {
	  0,   0,   0,   0,   1,   1,   1,   1,   0,   0,   0,   1,   1,   1,   1,   1,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,
};

const Uint8 *const wari21_cmd_09[] = {
	wari21_cmd_09_00, wari21_cmd_09_01, wari21_cmd_09_02,
};

const Uint8 wari21_unused_0[] = {
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	  0,   0,   1,   1,   2,   2,   3,   3,   0,   0,   1,   1,   2,   2,   3,   3,
	  0,   0,   1,   1,   2,   2,   3,   3,   0,   0,   1,   1,   2,   2,   3,   3,
	241,   7,   0,   0,   3,   8,   0,   0,  45,   8,   0,   0, 105,   8,   0,   0,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	114,   8,   0,   0,
};

const Uint8 wari21_rate_10[] = {
	  5,   6,   7,   8,   8,   8,   8,   8,   9,  10,  11,  12,  12,  12,  12,  12,
	 13,  13,  13,  13,  14,  14,  14,  14,  16,  18,  20,  22,  24,  26,  28,  30,
};

const Uint8 wari21_sel_10[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,
};

const Uint8 *const wari21_cmd_10[] = {
	wari21_cmd_10_00, wari21_cmd_10_01, wari21_cmd_10_02,
};

const Uint8 wari21_rate_11[] = {
	  8,   8,   8,   9,   9,   9,  10,  10,  10,  11,  11,  11,  12,  12,  12,  13,
	 13,  13,  14,  14,  14,  15,  15,  15,  16,  16,  16,  17,  17,  17,  18,  18,
};

const Uint8 wari21_sel_11[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   0,   0,   0,   0,
	  1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
};

const Uint8 *const wari21_cmd_11[] = {
	wari21_cmd_11_00, wari21_cmd_11_01, wari21_cmd_11_02,
};

const Uint8 wari21_rate_12[] = {
	  8,   8,   8,   9,   9,   9,  10,  10,  10,  11,  11,  11,  12,  12,  12,  13,
	 13,  13,  14,  14,  14,  15,  15,  15,  16,  16,  16,  17,  17,  17,  18,  18,
};

const Uint8 wari21_sel_12[] = {
	  0,   0,   0,   0,   0,   3,   3,   3,   0,   0,   0,   0,   0,   3,   3,   3,
	  1,   1,   1,   1,   1,   3,   3,   3,   2,   2,   2,   2,   2,   3,   3,   3,
};

const Uint8 *const wari21_cmd_12[] = {
	wari21_cmd_12_00, wari21_cmd_12_01, wari21_cmd_12_02, wari21_cmd_12_03,
};

const Uint8 wari21_rate_13[] = {
	  8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,
	 24,  25,  26,  27,  28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	  2,   2,   2,   2,   2,   2,   2,   2,   4,   4,   4,   6,   6,   8,   8,   8,
	 10,  10,  10,  11,  11,  11,  12,  12,  12,  13,  13,  13,  14,  14,  14,  16,
	  2,   2,   2,   2,   2,   2,   2,   2,   4,   4,   4,   4,   4,   4,   4,   4,
	  6,   6,   6,   6,   6,   6,   6,   6,   8,   8,   8,  10,  10,  10,  12,  12,
	  3,   3,   4,   4,   5,   5,   6,   6,   7,   7,   8,   8,   9,   9,  10,  10,
	 11,  11,  12,  12,  13,  13,  14,  14,  15,  15,  15,  15,  18,  18,  18,  20,
};

const Uint8 wari21_cmd_02_00[] = {
	IF_LEVEL( 30 ),
		IF_RND( 24 ),
			IF_CATCH,
				IF_RND( 16 ),
					WAIT2( 6 ),
					NAG_VFLIP_ADJ( 1, LEVER_FORWARD ),
				CMD_ELSE,
					NAG_VFLIP_ADJ2( 1, LEVER_FORWARD ),
				CMD_ENDIF,
			CMD_ELSE,
				CANCEL_SET2,
					CROUCH,
					PUNCH( LV_LIGHT ),
					STAND,
					PUNCH( LV_HEAVY ),
					CANCEL_CLR,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 23 ),
			IF_RND( 16 ),
				IF_CATCH,
					IF_RND( 16 ),
						WAIT2( 6 ),
						NAG_VFLIP_ADJ( 1, LEVER_FORWARD ),
					CMD_ELSE,
						NAG_VFLIP_ADJ2( 1, LEVER_FORWARD ),
					CMD_ENDIF,
				CMD_ELSE,
					CANCEL_SET2,
						CROUCH,
						PUNCH( LV_LIGHT ),
						STAND,
						PUNCH( LV_HEAVY ),
						CANCEL_CLR,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 12 ),
				IF_RND( 12 ),
					IF_CATCH,
						IF_RND( 16 ),
							WAIT2( 6 ),
							NAG_VFLIP_ADJ( 1, LEVER_FORWARD ),
						CMD_ELSE,
							NAG_VFLIP_ADJ2( 1, LEVER_FORWARD ),
						CMD_ENDIF,
					CMD_ELSE,
						CANCEL_SET2,
							CROUCH,
							PUNCH( LV_LIGHT ),
							STAND,
							PUNCH( LV_HEAVY ),
							CANCEL_CLR,
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 8 ),
					IF_RND( 8 ),
						IF_CATCH,
							IF_RND( 16 ),
								WAIT2( 6 ),
								NAG_VFLIP_ADJ( 1, LEVER_FORWARD ),
							CMD_ELSE,
								NAG_VFLIP_ADJ2( 1, LEVER_FORWARD ),
							CMD_ENDIF,
						CMD_ELSE,
							CANCEL_SET2,
								CROUCH,
								PUNCH( LV_LIGHT ),
								STAND,
								PUNCH( LV_HEAVY ),
								CANCEL_CLR,
							CMD_ENDIF,
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ELSE,
					IF_RND( 4 ),
						IF_CATCH,
							IF_RND( 16 ),
								WAIT2( 6 ),
								NAG_VFLIP_ADJ( 1, LEVER_FORWARD ),
							CMD_ELSE,
								NAG_VFLIP_ADJ2( 1, LEVER_FORWARD ),
							CMD_ENDIF,
						CMD_ELSE,
							CANCEL_SET2,
								CROUCH,
								PUNCH( LV_LIGHT ),
								STAND,
								PUNCH( LV_HEAVY ),
								CANCEL_CLR,
							CMD_ENDIF,
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari21_cmd_02_01[] = {
	IF_LEVEL( 30 ),
		IF_RND( 24 ),
			IF_RND( 16 ),
				CANCEL_SET2,
					CROUCH,
					PUNCH( LV_LIGHT ),
					PUNCH( LV_LIGHT ),
					STAND,
					PUNCH( LV_HEAVY ),
					CANCEL_CLR,
				CMD_ELSE,
					CROUCH,
					KICK( LV_HEAVY ),
					STAND,
					CANCEL_CLR,
				CMD_ENDIF,
			CMD_ELSE,
				IF_RND( 16 ),
					CANCEL_SET,
						CROUCH,
						KICK( LV_LIGHT ),
						STAND,
						PUNCH( LV_LIGHT ),
						PUNCH( LV_HEAVY ),
						CANCEL_CLR,
					CMD_ELSE,
						CROUCH,
						PUNCH( LV_HEAVY ),
						STAND,
						CANCEL_CLR,
					CMD_ENDIF,
				CMD_ELSE,
					NAG_VFLIP_ADJ( 1, LEVER_FORWARD ),
				CMD_ENDIF,
			CMD_ENDIF,
			WAIT( 20 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 23 ),
			IF_RND( 18 ),
				IF_RND( 16 ),
					CANCEL_SET2,
						CROUCH,
						PUNCH( LV_LIGHT ),
						PUNCH( LV_LIGHT ),
						STAND,
						PUNCH( LV_HEAVY ),
						CANCEL_CLR,
					CMD_ELSE,
						CROUCH,
						KICK( LV_HEAVY ),
						STAND,
						CANCEL_CLR,
					CMD_ENDIF,
				CMD_ELSE,
					IF_RND( 16 ),
						CANCEL_SET,
							CROUCH,
							KICK( LV_LIGHT ),
							STAND,
							PUNCH( LV_LIGHT ),
							KICK( LV_LIGHT ),
							CANCEL_CLR,
						CMD_ELSE,
							CROUCH,
							PUNCH( LV_HEAVY ),
							STAND,
							CANCEL_CLR,
						CMD_ENDIF,
					CMD_ELSE,
						NAG_VFLIP_ADJ( 1, LEVER_FORWARD ),
					CMD_ENDIF,
				CMD_ENDIF,
				WAIT( 20 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 16 ),
				IF_RND( 12 ),
					IF_RND( 16 ),
						CANCEL_SET2,
							CROUCH,
							PUNCH( LV_LIGHT ),
							PUNCH( LV_LIGHT ),
							STAND,
							PUNCH( LV_HEAVY ),
							CANCEL_CLR,
						CMD_ELSE,
							CROUCH,
							KICK( LV_HEAVY ),
							STAND,
							CANCEL_CLR,
						CMD_ENDIF,
					CMD_ELSE,
						IF_RND( 16 ),
							CANCEL_SET,
								CROUCH,
								KICK( LV_LIGHT ),
								STAND,
								KICK( LV_LIGHT ),
								CANCEL_CLR,
							CMD_ELSE,
								CROUCH,
								PUNCH( LV_HEAVY ),
								STAND,
								CANCEL_CLR,
							CMD_ENDIF,
						CMD_ELSE,
							NAG_VFLIP_ADJ( 1, LEVER_FORWARD ),
						CMD_ENDIF,
					CMD_ENDIF,
					WAIT( 20 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 10 ),
					IF_RND( 10 ),
						IF_RND( 16 ),
							CANCEL_SET2,
								CROUCH,
								PUNCH( LV_LIGHT ),
								PUNCH( LV_LIGHT ),
								STAND,
								PUNCH( LV_HEAVY ),
								CANCEL_CLR,
							CMD_ELSE,
								CROUCH,
								KICK( LV_HEAVY ),
								STAND,
								CANCEL_CLR,
							CMD_ENDIF,
						CMD_ELSE,
							IF_RND( 16 ),
								CANCEL_SET,
									CROUCH,
									KICK( LV_LIGHT ),
									STAND,
									PUNCH( LV_LIGHT ),
									PUNCH( LV_HEAVY ),
									CANCEL_CLR,
								CMD_ELSE,
									CROUCH,
									PUNCH( LV_HEAVY ),
									STAND,
									CANCEL_CLR,
								CMD_ENDIF,
							CMD_ELSE,
								NAG_VFLIP_ADJ( 1, LEVER_FORWARD ),
							CMD_ENDIF,
						CMD_ENDIF,
						WAIT( 20 ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_LEVEL( 5 ),
						IF_RND( 8 ),
							IF_RND( 16 ),
								CANCEL_SET2,
									CROUCH,
									PUNCH( LV_LIGHT ),
									PUNCH( LV_LIGHT ),
									STAND,
									PUNCH( LV_HEAVY ),
									CANCEL_CLR,
								CMD_ELSE,
									CROUCH,
									KICK( LV_HEAVY ),
									STAND,
									CANCEL_CLR,
								CMD_ENDIF,
							CMD_ELSE,
								IF_RND( 16 ),
									CANCEL_SET,
										CROUCH,
										KICK( LV_LIGHT ),
										STAND,
										KICK( LV_LIGHT ),
										KICK( LV_HEAVY ),
										CANCEL_CLR,
									CMD_ELSE,
										CROUCH,
										PUNCH( LV_HEAVY ),
										STAND,
										CANCEL_CLR,
									CMD_ENDIF,
								CMD_ELSE,
									NAG_VFLIP_ADJ( 1, LEVER_FORWARD ),
								CMD_ENDIF,
							CMD_ENDIF,
							WAIT( 20 ),
						CMD_ENDIF,
					CMD_ELSE,
						IF_RND( 4 ),
							IF_RND( 16 ),
								CANCEL_SET2,
									CROUCH,
									PUNCH( LV_LIGHT ),
									PUNCH( LV_LIGHT ),
									STAND,
									PUNCH( LV_HEAVY ),
									CANCEL_CLR,
								CMD_ELSE,
									CROUCH,
									KICK( LV_HEAVY ),
									STAND,
									CANCEL_CLR,
								CMD_ENDIF,
							CMD_ELSE,
								IF_RND( 16 ),
									CANCEL_SET,
										CROUCH,
										KICK( LV_LIGHT ),
										KICK( LV_LIGHT ),
										KICK( LV_HEAVY ),
										STAND,
										CANCEL_CLR,
									CMD_ELSE,
										CROUCH,
										PUNCH( LV_HEAVY ),
										STAND,
										CANCEL_CLR,
									CMD_ENDIF,
								CMD_ELSE,
									NAG_VFLIP_ADJ( 1, LEVER_FORWARD ),
								CMD_ENDIF,
							CMD_ENDIF,
							WAIT( 20 ),
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari21_cmd_02_02[] = {
	IF_LEVEL( 31 ),
		IF_RND( 10 ),
			SP_CMD( pl21_sp_sonic, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 27 ),
			IF_RND( 8 ),
				SP_CMD( pl21_sp_sonic, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 12 ),
				IF_RND( 8 ),
					SP_CMD( pl21_sp_sonic, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 8 ),
					IF_RND( 8 ),
						SP_CMD( pl21_sp_sonic, 0, PK_PUNCH, LV_LIGHT, 0 ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_RND( 4 ),
						SP_CMD( pl21_sp_sonic, 0, PK_PUNCH, LV_LIGHT, 0 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari21_cmd_03_00[] = {
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

const Uint8 wari21_cmd_05_00[] = {
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

const Uint8 wari21_cmd_05_01[] = {
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	GUARD( 60 ),
	CMD_EXIT,
};

const Uint8 wari21_cmd_05_02[] = {
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	IF_LEVEL( 28 ),
		IF_RND( 4 ),
			SP_CMD( pl21_sp_sonic, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			GUARD( 30 ),
		CMD_ENDIF,
	CMD_ELSE,
		GUARD( 30 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari21_cmd_06_00[] = {
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	TIMER2_SET( 60 ),
	CMD_EXIT,
};

const Uint8 wari21_cmd_07_00[] = {
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	VCOUNTER_SET( 8 ),
	GUARD( 30 ),
	CMD_EXIT,
};

const Uint8 wari21_cmd_07_01[] = {
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	VCOUNTER_SET( 8 ),
	IF_RND( 16 ),
		IF_HEIGHT( 0, 48 ),
			WAIT( 10 ),
			SP_CMD( pl21_sp_smr, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ELSE,
			GUARD( 30 ),
		CMD_ENDIF,
	CMD_ELSE,
		TIMER2_SET( 80 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari21_cmd_07_02[] = {
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	VCOUNTER_SET( 8 ),
	TIMER2_SET( 80 ),
	CMD_EXIT,
};

const Uint8 wari21_cmd_08_00[] = {
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	VCOUNTER_SET( 8 ),
	IF_RND( 16 ),
		TIMER_SET( 120 ),
		IF_HEIGHT( 0, 48 ),
			WAIT( 10 ),
			SP_CMD( pl21_sp_smr, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ELSE,
			GUARD( 30 ),
		CMD_ENDIF,
	CMD_ELSE,
		TIMER2_SET( 80 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari21_cmd_09_00[] = {
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	GUARD( 30 ),
	CMD_EXIT,
	GUARD( 30 ),
	CMD_EXIT,
	GUARD( 30 ),
	CMD_EXIT,
};

const Uint8 wari21_cmd_09_01[] = {
	IF_LEVEL( 30 ),
		GUARD( 30 ),
	CMD_ELSE,
		IF_LEVEL( 26 ),
			IF_RND( 28 ),
				GUARD( 30 ),
			CMD_ELSE,
				CMD_JUMP( 19 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 17 ),
				IF_RND( 24 ),
					GUARD( 30 ),
				CMD_ELSE,
					CMD_JUMP( 19 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 10 ),
					IF_RND( 18 ),
						GUARD( 30 ),
					CMD_ELSE,
						CMD_JUMP( 19 ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_LEVEL( 2 ),
						IF_RND( 12 ),
							GUARD( 30 ),
						CMD_ELSE,
							CMD_JUMP( 19 ),
						CMD_ENDIF,
					CMD_ELSE,
						IF_RND( 8 ),
							GUARD( 30 ),
						CMD_ELSE,
							CMD_JUMP( 19 ),
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari21_cmd_09_02[] = {
	IF_EM_ATCK( pl21_at_sonic ),
		SP_CMD( pl21_sp_sonic, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ELSE,
		IF_EM_ATCK( 49 ),
			SP_CMD( pl21_sp_sonic, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ELSE,
			IF_EM_ATCK( 50 ),
				SP_CMD( pl21_sp_sonic, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ELSE,
				GUARD( 30 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
	IF_RND( 8 ),
		SP_CMD( pl21_sp_break, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ENDIF,
	IF_RND( 16 ),
		CMD_JUMP( 0 ),
	CMD_ELSE,
		CMD_JUMP( 17 ),
	CMD_ENDIF,
	CMD_EXIT,
	IF_AREA( 160, 160, 0, 0 ),
		TIMER_SET( 60 ),
		WALK_DIST( 64 ),
		CANCEL_SET2,
			CROUCH,
			KICK( LV_LIGHT ),
			KICK( LV_LIGHT ),
			STAND,
			KICK( LV_HEAVY ),
			SP_CMD( pl21_sp_sonic, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CANCEL_CLR,
		CMD_ELSE,
			CROUCH,
			KICK( LV_HEAVY ),
			STAND,
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 17 ),
	CMD_ENDIF,
	CMD_EXIT,
	IF_GAUGE( 1 ),
		WALK_F( 48 ),
		WAIT( 10 ),
		SP_CMD( pl21_sp_blitz, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ELSE,
		JUMP( DIR_FORWARD ),
		CANCEL_SET2,
			JUMP_FLAG_SET,
				CMD_NONE( 41 ),
				TIMER_SET( 120 ),
				IF_HEIGHT( 1, 112 ),
					IF_EM_ATCK( ATCK_JUMP_LP ),
						PUNCH( LV_LIGHT ),
						PUNCH( LV_LIGHT ),
						KICK( LV_LIGHT ),
						PUNCH( LV_HEAVY ),
					CMD_ELSE,
					CMD_ENDIF,
				CMD_ELSE,
				CMD_ENDIF,
				JUMP_FLAG_CLR,
			CMD_ELSE,
				JUMP_FLAG_CLR,
			CMD_ENDIF,
			IF_EM_ATCK( ATCK_CROUCH_LP ),
				CROUCH,
				PUNCH( LV_LIGHT ),
				PUNCH( LV_LIGHT ),
				KICK( LV_HEAVY ),
			CMD_ELSE,
			CMD_ENDIF,
			CANCEL_CLR,
		CMD_ELSE,
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
	WAIT( 10 ),
	SP_CMD( pl21_sp_break, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_EXIT,
	CMD_EXIT,
};

const Uint8 wari21_cmd_10_00[] = {
	VCOUNTER_SET( 8 ),
	GUARD( 30 ),
	CMD_EXIT,
};

const Uint8 wari21_cmd_10_01[] = {
	JUMP( DIR_UP ),
	CMD_EXIT,
};

const Uint8 wari21_cmd_10_02[] = {
	VCOUNTER_SET( 8 ),
	JUMP( DIR_BACK ),
	TIMER2_SET( 80 ),
	CMD_EXIT,
};

const Uint8 wari21_cmd_11_00[] = {
	WAIT( 20 ),
	VCOUNTER_SET( 8 ),
	WAIT2( 14 ),
	SJUMP( DIR_UP ),
	SJUMP_FLAG_SET,
		CMD_NONE( 41 ),
		TIMER2_SET( 90 ),
		TIMER_SET( 80 ),
		IF_CATCH,
			IF_RND( 16 ),
				IF_RND( 16 ),
					NAG_VFLIP_ADJ( 0, LEVER_FORWARD ),
				CMD_ELSE,
					NAG_VFLIP_ADJ( 1, LEVER_FORWARD ),
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
	CMD_EXIT,
};

const Uint8 wari21_cmd_11_01[] = {
	VCOUNTER_SET( 8 ),
	WAIT( 20 ),
	IF_LEVEL( 26 ),
		SJUMP( DIR_FORWARD ),
		SJUMP_FLAG_SET,
			CMD_NONE( 41 ),
			TIMER2_SET( 90 ),
			TIMER_SET( 120 ),
			IF_EM_ATCK( ATCK_JUMP_LK ),
				CANCEL_SET2,
					PUNCH( LV_LIGHT ),
					KICK( LV_LIGHT ),
					PUNCH( LV_LIGHT ),
					KICK( LV_LIGHT ),
					PUNCH( LV_HEAVY ),
					KICK( LV_HEAVY ),
					CANCEL_CLR,
				CMD_ELSE,
					SP_CMD( pl21_sp_justice, 0, PK_PUNCH, LV_LIGHT, 0 ),
					CANCEL_CLR,
				CMD_ENDIF,
			CMD_ELSE,
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ELSE,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		WAIT( 5 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari21_cmd_11_02[] = {
	IF_LEVEL( 16 ),
		TIMER_SET( 80 ),
		IF_EM_ATCK( 50 ),
		CMD_ELSE,
		CMD_ENDIF,
	CMD_ELSE,
		WAIT( 5 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari21_cmd_12_00[] = {
	VCOUNTER_SET( 8 ),
	WAIT( 10 ),
	WAIT2( 14 ),
	SJUMP( DIR_UP ),
	SJUMP_FLAG_SET,
		CMD_NONE( 41 ),
		TIMER2_SET( 90 ),
		WAIT( 60 ),
		JUMP_FLAG_CLR,
	CMD_ELSE,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari21_cmd_12_01[] = {
	VCOUNTER_SET( 8 ),
	WAIT2( 14 ),
	SJUMP( DIR_FORWARD ),
	SJUMP_FLAG_SET,
		CMD_NONE( 41 ),
		TIMER2_SET( 90 ),
		JUMP_FLAG_CLR,
	CMD_ELSE,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 wari21_cmd_12_02[] = {
	VCOUNTER_SET( 8 ),
	WAIT2( 12 ),
	DASH_B( 128 ),
	STAND,
	TIMER2_SET( 60 ),
	CMD_EXIT,
};

const Uint8 wari21_cmd_12_03[] = {
	TIMER_SET( 5 ),
	IF_EM_ATCK( 50 ),
		SP_CMD( pl21_sp_sonic, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ELSE,
		IF_EM_ATCK( ATCK_STAND_MP ),
			PUNCH( LV_LIGHT ),
		CMD_ELSE,
			IF_EM_ATCK( ATCK_STAND_HK ),
				KICK( LV_HEAVY ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

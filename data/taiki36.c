/* Thanos (character 36): the computer's waiting lists.
   What it does while nothing calls for more. PLAYER_WORK's cpu_adrs[0] points here, and em_cmd_sel
   picks a list from it.

   taiki36_tbl     one record (CPU_DATA): the offsets of the three tables.
   taiki36_rate    the chance, out of 32, that a list's CMD_NEXT goes over to an attack list
                   (atck36.c), by the level of play.
   taiki36_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   taiki36_cmd     the lists: a pick is a number in this table.
   taiki36_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                   list of lib36.c. */
#include "player.h"
#include "cpu_cmd.h"

extern const Uint8 *const taiki36_sel[], *const taiki36_cmd[];
extern const Uint8 taiki36_rate[], taiki36_sel_0[], taiki36_sel_1[], taiki36_sel_2[],
	taiki36_sel_3[], taiki36_sel_4[], taiki36_sel_5[], taiki36_sel_6[], taiki36_sel_7[],
	taiki36_sel_8[], taiki36_cmd_00[], taiki36_cmd_01[], taiki36_cmd_18[], taiki36_cmd_12[],
	taiki36_cmd_13[], taiki36_cmd_14[], taiki36_cmd_15[], taiki36_cmd_02[], taiki36_cmd_03[],
	taiki36_cmd_04[], taiki36_cmd_16[], taiki36_cmd_17[], taiki36_cmd_19[], taiki36_cmd_05[],
	taiki36_cmd_06[], taiki36_cmd_07[], taiki36_cmd_08[], taiki36_cmd_09[], taiki36_cmd_10[],
	taiki36_cmd_11[];

const CPU_DATA taiki36_tbl[] = {
	{ ( Sint32 )taiki36_rate, ( Sint32 )taiki36_sel, ( Sint32 )taiki36_cmd },
};

const Uint8 taiki36_rate[] = {
	 13,  14,  15,  16,  16,  17,  17,  18,  18,  19,  20,  20,  22,  23,  24,  25,
	 26,  27,  28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 *const taiki36_sel[] = {
	taiki36_sel_0, taiki36_sel_1, taiki36_sel_2, taiki36_sel_3, taiki36_sel_4, taiki36_sel_5,
	taiki36_sel_6, taiki36_sel_7, taiki36_sel_8,
};

const Uint8 taiki36_sel_0[] = {
	  2,   2,  14,  14,  14,  14,  14,  14,   4,  12,  12,  12,   4,  13,  13,  13,
	  2,   2,   2,   2,  15,  15,  15,  15,   2,   2,   2,   2,   2,   2,   2,   2,
};

const Uint8 taiki36_sel_1[] = {
	  2,  14,  14,  14,   2,   2,  15,  15,   4,  12,  12,  12,   4,  13,  13,  13,
	  4,   4,   4,   4,  16,  16,  16,  16,   2,   2,   2,   2,   2,   2,   2,   2,
};

const Uint8 taiki36_sel_2[] = {
	 17,  17,   5,   5,   6,   6,   7,   7,   2,   2,   2,   2,   3,   3,   3,   3,
	  2,  14,   2,  15,  12,  12,  13,  13,   4,   4,   4,   4,  16,  16,  16,  16,
};

const Uint8 taiki36_sel_3[] = {
	 17,  17,  17,  17,   6,   6,   6,   6,   2,   2,   2,   2,   3,   3,   3,   3,
	  4,   4,   4,   4,   4,   4,   4,   4,   2,  14,   2,  15,   2,  12,   2,  13,
};

const Uint8 taiki36_sel_4[] = {
	 17,  17,  17,  17,   6,   6,   6,   6,   2,   2,   2,   2,   3,   3,   3,   3,
	  4,   4,   4,   4,   4,   4,   4,   4,  17,  17,   6,   6,   2,   3,   4,   4,
};

const Uint8 taiki36_sel_5[] = {
	  2,   2,   2,   2,   3,   3,   3,   3,   4,   4,   4,   4,  16,  16,  16,  16,
	 17,  17,   5,   5,   6,   6,   7,   7,   8,   8,   8,   8,   8,   8,   8,   8,
};

const Uint8 taiki36_sel_6[] = {
	  8,   8,   8,   8,   8,   8,   8,   8,   9,   9,   9,   9,   9,   9,   9,   9,
	  2,   2,   2,   2,   3,   3,   3,   3,  17,  17,   5,   6,   7,   4,  16,  16,
};

const Uint8 taiki36_sel_7[] = {
	  9,   9,   9,   9,   9,   9,   9,   9,  11,  11,  11,  11,  11,  11,  11,  11,
	  3,   3,   3,   3,   3,   3,   3,   3,   8,   8,   8,   8,   8,   8,   8,   8,
};

const Uint8 taiki36_sel_8[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	 18,  18,  18,  18,  18,  18,  18,  18,  18,  18,  18,  18,  18,  18,  18,  18,
};

const Uint8 *const taiki36_cmd[] = {
	taiki36_cmd_00, taiki36_cmd_01, taiki36_cmd_02, taiki36_cmd_03, taiki36_cmd_04, taiki36_cmd_05,
	taiki36_cmd_06, taiki36_cmd_07, taiki36_cmd_08, taiki36_cmd_09, taiki36_cmd_10, taiki36_cmd_11,
	taiki36_cmd_12, taiki36_cmd_13, taiki36_cmd_14, taiki36_cmd_15, taiki36_cmd_16, taiki36_cmd_17,
	taiki36_cmd_18, taiki36_cmd_19,
};

const Uint8 taiki36_cmd_00[] = {
	WAIT( 16 ),
	CMD_RESET,
};

const Uint8 taiki36_cmd_01[] = {
	WALK_EM( 64 ),
	CMD_RESET,
};

const Uint8 taiki36_cmd_18[] = {
	WALK_B( 96 ),
	CMD_RESET,
};

const Uint8 taiki36_cmd_12[] = {
	STAND,
	WALK_RND( 0 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki36_cmd_13[] = {
	STAND,
	WALK_B( 96 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki36_cmd_14[] = {
	STAND,
	WAIT( 10 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki36_cmd_15[] = {
	STAND,
	WAIT( 30 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki36_cmd_02[] = {
	STAND,
	WAIT2( 12 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki36_cmd_03[] = {
	CROUCH,
	WAIT2( 12 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki36_cmd_04[] = {
	STAND,
	WAIT( 1 ),
	WALK_RND( 0 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki36_cmd_16[] = {
	STAND,
	WAIT( 1 ),
	WALK_DIST( 160 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki36_cmd_17[] = {
	STAND,
	WAIT( 1 ),
	DASH_F( 128 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki36_cmd_19[] = {
	STAND,
	WAIT( 1 ),
	DASH_B( 288 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki36_cmd_05[] = {
	STAND,
	WAIT( 1 ),
	TIMER_SET( 0 ),
	JUMP( DIR_UP ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki36_cmd_06[] = {
	STAND,
	WAIT( 1 ),
	TIMER_SET( 0 ),
	JUMP( DIR_FORWARD ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki36_cmd_07[] = {
	STAND,
	WAIT( 1 ),
	TIMER_SET( 0 ),
	JUMP( DIR_BACK ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki36_cmd_08[] = {
	WAIT( 1 ),
	TIMER_SET( 0 ),
	IF_DIST( 64, 32, 64, 32 ),
		CMD_NEXT,
	CMD_ELSE,
		GUARD( 30 ),
	CMD_ENDIF,
	STAND,
	WALK_RND( 0 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki36_cmd_09[] = {
	WAIT( 1 ),
	CROUCH,
	WAIT( 1 ),
	TIMER_SET( 0 ),
	WAIT( 1 ),
	IF_DIST( 64, 32, 64, 32 ),
		WAIT( 1 ),
		CMD_NEXT,
	CMD_ELSE,
		WAIT( 2 ),
		TIMER2_SET( 20 ),
		WAIT( 2 ),
	CMD_ENDIF,
	WAIT( 1 ),
	TIMER_SET( 30 ),
	WALK_RND( 0 ),
	WAIT( 1 ),
	CMD_NEXT,
	WAIT( 1 ),
	CMD_RESET,
};

const Uint8 taiki36_cmd_10[] = {
	WAIT( 1 ),
	TIMER_SET( 0 ),
	IF_DIST( 64, 32, 64, 32 ),
		CMD_NEXT,
	CMD_ELSE,
		GUARD( 30 ),
	CMD_ENDIF,
	STAND,
	DASH_F( 128 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki36_cmd_11[] = {
	WAIT( 1 ),
	TIMER_SET( 0 ),
	WAIT( 1 ),
	IF_DIST( 64, 32, 64, 32 ),
		WAIT( 1 ),
		CMD_NEXT,
	CMD_ELSE,
		WAIT( 1 ),
		TIMER2_SET( 30 ),
		WAIT( 1 ),
	CMD_ENDIF,
	STAND,
	WAIT( 1 ),
	JUMP( DIR_BACK ),
	JUMP_FLAG_SET,
		TIMER2_SET( 30 ),
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	WAIT( 1 ),
	CMD_NEXT,
	CMD_RESET,
};

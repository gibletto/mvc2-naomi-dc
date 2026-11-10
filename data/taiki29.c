/* Juggernaut (character 29): the computer's waiting lists.
   What it does while nothing calls for more. PLAYER_WORK's cpu_adrs[0] points here, and em_cmd_sel
   picks a list from it.

   taiki29_tbl     one record (CPU_DATA): the offsets of the three tables.
   taiki29_rate    the chance, out of 32, that a list's CMD_NEXT goes over to an attack list
                   (atck29.c), by the level of play.
   taiki29_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   taiki29_cmd     the lists: a pick is a number in this table.
   taiki29_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                   list of lib29.c. */
#include "player.h"
#include "cpu_cmd.h"

extern const Uint8 *const taiki29_sel[], *const taiki29_cmd[];
extern const Uint8 taiki29_rate[], taiki29_sel_0[], taiki29_sel_1[], taiki29_sel_2[],
	taiki29_sel_3[], taiki29_sel_4[], taiki29_sel_5[], taiki29_sel_6[], taiki29_sel_7[],
	taiki29_sel_8[], taiki29_cmd_00[], taiki29_cmd_01[], taiki29_cmd_18[], taiki29_cmd_12[],
	taiki29_cmd_13[], taiki29_cmd_14[], taiki29_cmd_15[], taiki29_cmd_02[], taiki29_cmd_03[],
	taiki29_cmd_04[], taiki29_cmd_16[], taiki29_cmd_17[], taiki29_cmd_19[], taiki29_cmd_05[],
	taiki29_cmd_06[], taiki29_cmd_07[], taiki29_cmd_08[], taiki29_cmd_09[], taiki29_cmd_10[],
	taiki29_cmd_11[];

const CPU_DATA taiki29_tbl[] = {
	{ ( Sint32 )taiki29_rate, ( Sint32 )taiki29_sel, ( Sint32 )taiki29_cmd },
};

const Uint8 taiki29_rate[] = {
	 13,  14,  15,  16,  16,  17,  17,  18,  18,  19,  20,  20,  22,  23,  24,  25,
	 26,  27,  28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 *const taiki29_sel[] = {
	taiki29_sel_0, taiki29_sel_1, taiki29_sel_2, taiki29_sel_3, taiki29_sel_4, taiki29_sel_5,
	taiki29_sel_6, taiki29_sel_7, taiki29_sel_8,
};

const Uint8 taiki29_sel_0[] = {
	  2,   2,  14,  14,  14,  14,  14,  14,   2,   2,   2,   2,  15,  15,  15,  15,
	 12,  12,  12,  12,  12,  13,  13,  13,   2,   2,   2,   2,   2,   2,   2,   2,
};

const Uint8 taiki29_sel_1[] = {
	 14,  14,  14,  14,  14,  14,  15,  15,  15,  15,  15,  15,  12,  12,  12,  12,
	 12,  13,  13,  13,   2,   2,   2,   2,   2,   2,   2,   2,   4,   4,   4,   4,
};

const Uint8 taiki29_sel_2[] = {
	 14,  14,  14,  15,  15,  15,  12,  12,  12,  12,  13,  13,  16,  16,  16,  16,
	  2,   2,   2,   2,   2,   2,   2,   2,   4,   4,   4,   4,   4,   4,   4,   4,
};

const Uint8 taiki29_sel_3[] = {
	 14,  14,  15,  15,  12,  12,  12,  13,   4,   4,   4,   4,   4,   4,   4,   4,
	  2,   2,   2,   2,   2,   2,  16,  16,  16,  16,  17,  17,  17,  19,   5,   6,
};

const Uint8 taiki29_sel_4[] = {
	 14,  15,  15,  12,  12,  13,   2,   2,   2,   2,   2,   2,   4,   4,   4,   4,
	  4,   4,   4,   4,  16,  16,  16,  16,   5,   6,   7,   8,   9,  17,  17,  17,
};

const Uint8 taiki29_sel_5[] = {
	 14,  15,  12,  13,  16,  16,  16,  16,   2,   2,   4,   4,   4,   4,   4,   4,
	 17,  17,  17,  17,   8,   8,   8,   8,  10,  10,  10,  10,   9,   9,  11,  11,
};

const Uint8 taiki29_sel_6[] = {
	  4,   4,   4,   4,   8,   8,   8,   8,   8,   8,  17,  17,  17,  17,  10,  10,
	 10,  10,  10,  10,  16,  16,  16,   2,   2,   2,  19,   5,   6,   7,  11,  11,
};

const Uint8 taiki29_sel_7[] = {
	  4,   4,   4,   4,   8,   8,   8,   8,   8,   8,  17,  17,  17,  17,  10,  10,
	 10,  10,  10,  10,  10,   4,   8,  17,   2,  19,   5,   6,   7,  11,  11,  11,
};

const Uint8 taiki29_sel_8[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	 18,  18,  18,  18,  18,  18,  18,  18,  18,  18,   1,   1,   1,   1,   1,   1,
};

const Uint8 *const taiki29_cmd[] = {
	taiki29_cmd_00, taiki29_cmd_01, taiki29_cmd_02, taiki29_cmd_03, taiki29_cmd_04, taiki29_cmd_05,
	taiki29_cmd_06, taiki29_cmd_07, taiki29_cmd_08, taiki29_cmd_09, taiki29_cmd_10, taiki29_cmd_11,
	taiki29_cmd_12, taiki29_cmd_13, taiki29_cmd_14, taiki29_cmd_15, taiki29_cmd_16, taiki29_cmd_17,
	taiki29_cmd_18, taiki29_cmd_19,
};

const Uint8 taiki29_cmd_00[] = {
	WAIT( 16 ),
	CMD_RESET,
};

const Uint8 taiki29_cmd_01[] = {
	WALK_EM( 64 ),
	CMD_RESET,
};

const Uint8 taiki29_cmd_18[] = {
	WALK_B( 96 ),
	CMD_RESET,
};

const Uint8 taiki29_cmd_12[] = {
	STAND,
	WALK_RND( 0 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki29_cmd_13[] = {
	STAND,
	WALK_B( 96 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki29_cmd_14[] = {
	STAND,
	WAIT( 30 ),
	CMD_RESET,
};

const Uint8 taiki29_cmd_15[] = {
	STAND,
	WAIT( 90 ),
	CMD_RESET,
};

const Uint8 taiki29_cmd_02[] = {
	STAND,
	WAIT2( 12 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki29_cmd_03[] = {
	CROUCH,
	WAIT2( 12 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki29_cmd_04[] = {
	STAND,
	WAIT( 1 ),
	WALK_RND( 0 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki29_cmd_16[] = {
	STAND,
	WAIT( 1 ),
	WALK_DIST( 160 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki29_cmd_17[] = {
	STAND,
	WAIT( 1 ),
	DASH_F( 128 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki29_cmd_19[] = {
	STAND,
	WAIT( 1 ),
	DASH_B( 288 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki29_cmd_05[] = {
	STAND,
	WAIT( 1 ),
	TIMER_SET( 0 ),
	JUMP( DIR_UP ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki29_cmd_06[] = {
	STAND,
	WAIT( 1 ),
	TIMER_SET( 0 ),
	JUMP( DIR_FORWARD ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki29_cmd_07[] = {
	STAND,
	WAIT( 1 ),
	TIMER_SET( 0 ),
	JUMP( DIR_BACK ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki29_cmd_08[] = {
	WAIT( 1 ),
	TIMER_SET( 0 ),
	IF_DIST( 64, 0, 64, 0 ),
		CMD_NEXT,
	CMD_ENDIF,
	STAND,
	WALK_RND( 0 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki29_cmd_09[] = {
	WAIT( 1 ),
	CROUCH,
	TIMER_SET( 0 ),
	IF_DIST( 64, 0, 64, 0 ),
		CMD_NEXT,
	CMD_ENDIF,
	WALK_RND( 0 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki29_cmd_10[] = {
	WAIT( 1 ),
	TIMER_SET( 0 ),
	IF_DIST( 64, 0, 64, 0 ),
		CMD_NEXT,
	CMD_ENDIF,
	STAND,
	DASH_F( 128 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki29_cmd_11[] = {
	WAIT( 1 ),
	TIMER_SET( 0 ),
	IF_DIST( 64, 0, 64, 0 ),
		CMD_NEXT,
	CMD_ENDIF,
	STAND,
	DASH_B( 288 ),
	CMD_NEXT,
	CMD_RESET,
};

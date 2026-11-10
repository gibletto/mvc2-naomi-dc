/* Strider Hiryu (character 05): the computer's waiting lists.
   What it does while nothing calls for more. PLAYER_WORK's cpu_adrs[0] points here, and em_cmd_sel
   picks a list from it.

   taiki05_tbl     one record (CPU_DATA): the offsets of the three tables.
   taiki05_rate    the chance, out of 32, that a list's CMD_NEXT goes over to an attack list
                   (atck05.c), by the level of play.
   taiki05_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   taiki05_cmd     the lists: a pick is a number in this table.
   taiki05_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                   list of lib05.c. */
#include "player.h"
#include "cpu_cmd.h"

extern const Uint8 *const taiki05_sel[], *const taiki05_cmd[];
extern const Uint8 taiki05_rate[], taiki05_sel_0[], taiki05_sel_1[], taiki05_sel_2[],
	taiki05_sel_3[], taiki05_sel_4[], taiki05_sel_5[], taiki05_sel_6[], taiki05_sel_7[],
	taiki05_sel_8[], taiki05_cmd_00[], taiki05_cmd_01[], taiki05_cmd_02[], taiki05_cmd_03[],
	taiki05_cmd_04[], taiki05_cmd_05[], taiki05_cmd_06[], taiki05_cmd_07[], taiki05_cmd_08[],
	taiki05_cmd_09[], taiki05_cmd_10[], taiki05_cmd_11[], taiki05_cmd_12[], taiki05_cmd_13[],
	taiki05_cmd_14[], taiki05_cmd_15[], taiki05_cmd_16[], taiki05_cmd_17[];

const CPU_DATA taiki05_tbl[] = {
	{ ( Sint32 )taiki05_rate, ( Sint32 )taiki05_sel, ( Sint32 )taiki05_cmd },
};

const Uint8 taiki05_rate[] = {
	 13,  14,  15,  16,  16,  17,  17,  18,  18,  19,  20,  20,  22,  23,  24,  25,
	 26,  27,  28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 *const taiki05_sel[] = {
	taiki05_sel_0, taiki05_sel_1, taiki05_sel_2, taiki05_sel_3, taiki05_sel_4, taiki05_sel_5,
	taiki05_sel_6, taiki05_sel_7, taiki05_sel_8,
};

const Uint8 taiki05_sel_0[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
	  3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,
};

const Uint8 taiki05_sel_1[] = {
	  3,   2,   2,   2,   2,   2,   2,   2,   3,   2,   2,   2,   2,   2,   2,   2,
	  3,   6,   6,   6,   6,   6,   6,   6,   3,   6,   6,   6,   6,   6,   6,   6,
};

const Uint8 taiki05_sel_2[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
	  6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
};

const Uint8 taiki05_sel_3[] = {
	  6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
	  4,   4,   4,   4,   4,   4,   4,   4,   5,   5,   5,   5,   5,   5,   5,   5,
};

const Uint8 taiki05_sel_4[] = {
	  5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,
	  6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
};

const Uint8 taiki05_sel_5[] = {
	  6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
	  8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
};

const Uint8 taiki05_sel_6[] = {
	 10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,
	  8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
};

const Uint8 taiki05_sel_7[] = {
	  8,  10,  10,  10,  10,  10,  10,  10,   8,  10,  10,  10,  10,  10,  10,  10,
	  8,  10,  10,  10,  10,  10,  10,  10,   8,  10,  10,  10,  10,  10,  10,  10,
};

const Uint8 taiki05_sel_8[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,
};

const Uint8 *const taiki05_cmd[] = {
	taiki05_cmd_00, taiki05_cmd_01, taiki05_cmd_02, taiki05_cmd_03, taiki05_cmd_04, taiki05_cmd_05,
	taiki05_cmd_06, taiki05_cmd_07, taiki05_cmd_08, taiki05_cmd_09, taiki05_cmd_10, taiki05_cmd_11,
	taiki05_cmd_12, taiki05_cmd_13, taiki05_cmd_14, taiki05_cmd_15, taiki05_cmd_16, taiki05_cmd_17,
};

const Uint8 taiki05_cmd_00[] = {
	WAIT( 30 ),
	CMD_RESET,
};

const Uint8 taiki05_cmd_01[] = {
	WALK_B( 8 ),
	WAIT2( 14 ),
	CMD_RESET,
};

const Uint8 taiki05_cmd_02[] = {
	STAND,
	WAIT2( 14 ),
	WAIT( 20 ),
	WALK_DIST( 144 ),
	STAND,
	WAIT2( 14 ),
	CMD_NEXT,
	WAIT( 20 ),
	WALK_B( 64 ),
	STAND,
	WAIT2( 14 ),
	STAND,
	WAIT( 20 ),
	CMD_NEXT,
	STAND,
	CMD_RESET,
};

const Uint8 taiki05_cmd_03[] = {
	STAND,
	WAIT( 40 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_RESET,
};

const Uint8 taiki05_cmd_04[] = {
	STAND,
	WAIT2( 8 ),
	JUMP( DIR_BACK ),
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT( 20 ),
	DASH_B( 512 ),
	STAND,
	WAIT( 20 ),
	JUMP( DIR_FORWARD ),
	STAND,
	WAIT2( 4 ),
	CMD_NEXT,
	WAIT2( 8 ),
	JUMP( DIR_BACK ),
	STAND,
	WAIT( 10 ),
	CMD_NEXT,
	WAIT2( 8 ),
	WALK_EM( 96 ),
	STAND,
	CMD_NEXT,
	STAND,
	WAIT2( 8 ),
	JUMP( DIR_UP ),
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT2( 8 ),
	STAND,
	CMD_RESET,
};

const Uint8 taiki05_cmd_05[] = {
	CMD_NEXT,
	STAND,
	WAIT2( 14 ),
	CMD_NEXT,
	WALK_B( 16 ),
	JUMP( DIR_BACK ),
	STAND,
	WAIT( 10 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_NEXT,
	JUMP( DIR_BACK ),
	STAND,
	WAIT( 10 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_NEXT,
	STAND,
	CMD_RESET,
};

const Uint8 taiki05_cmd_06[] = {
	STAND,
	CMD_NEXT,
	WAIT( 10 ),
	CMD_NEXT,
	TIMER_SET( 60 ),
	WALK_DIST( 144 ),
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_NEXT,
	WALK_B( 64 ),
	CMD_NEXT,
	STAND,
	WAIT( 10 ),
	CMD_NEXT,
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	WALK_DIST( 144 ),
	STAND,
	CMD_NEXT,
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	WALK_DIST( 112 ),
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	STAND,
	WAIT( 10 ),
	CMD_NEXT,
	WALK_B( 32 ),
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki05_cmd_07[] = {
	CMD_RESET,
};

const Uint8 taiki05_cmd_08[] = {
	STAND,
	DASH_B( 256 ),
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_NEXT,
	WALK_EM( 128 ),
	STAND,
	WAIT2( 8 ),
	DASH_F( 256 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_NEXT,
	WALK_DIST( 144 ),
	STAND,
	CMD_NEXT,
	STAND,
	WAIT2( 8 ),
	JUMP( DIR_BACK ),
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_NEXT,
	DASH_F( 256 ),
	STAND,
	CMD_NEXT,
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	WALK_DIST( 144 ),
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_NEXT,
	DASH_B( 512 ),
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_NEXT,
	STAND,
	CMD_RESET,
};

const Uint8 taiki05_cmd_09[] = {
	CMD_RESET,
};

const Uint8 taiki05_cmd_10[] = {
	CMD_NEXT,
	WAIT2( 4 ),
	CMD_NEXT,
	WAIT2( 4 ),
	CMD_NEXT,
	WAIT2( 4 ),
	CMD_NEXT,
	WAIT2( 4 ),
	CMD_NEXT,
	WAIT2( 4 ),
	CMD_NEXT,
	WAIT2( 4 ),
	CMD_NEXT,
	WAIT2( 4 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki05_cmd_11[] = {
	CMD_RESET,
};

const Uint8 taiki05_cmd_12[] = {
	CMD_RESET,
};

const Uint8 taiki05_cmd_13[] = {
	CMD_RESET,
};

const Uint8 taiki05_cmd_14[] = {
	CMD_RESET,
};

const Uint8 taiki05_cmd_15[] = {
	CMD_RESET,
};

const Uint8 taiki05_cmd_16[] = {
	CMD_RESET,
};

const Uint8 taiki05_cmd_17[] = {
	CMD_RESET,
};

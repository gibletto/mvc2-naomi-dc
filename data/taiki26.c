/* M. Bison (character 26): the computer's waiting lists.
   What it does while nothing calls for more. PLAYER_WORK's cpu_adrs[0] points here, and em_cmd_sel
   picks a list from it.

   taiki26_tbl     one record (CPU_DATA): the offsets of the three tables.
   taiki26_rate    the chance, out of 32, that a list's CMD_NEXT goes over to an attack list
                   (atck26.c), by the level of play.
   taiki26_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   taiki26_cmd     the lists: a pick is a number in this table.
   taiki26_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                   list of lib26.c. */
#include "player.h"
#include "cpu_cmd.h"

extern const Uint8 *const taiki26_sel[], *const taiki26_cmd[];
extern const Uint8 taiki26_rate[], taiki26_sel_0[], taiki26_sel_1[], taiki26_sel_2[],
	taiki26_sel_3[], taiki26_sel_4[], taiki26_sel_5[], taiki26_sel_6[], taiki26_sel_7[],
	taiki26_sel_8[], taiki26_cmd_00[], taiki26_cmd_01[], taiki26_cmd_02[], taiki26_cmd_03[],
	taiki26_cmd_04[], taiki26_cmd_05[], taiki26_cmd_06[], taiki26_cmd_07[], taiki26_cmd_08[],
	taiki26_cmd_09[], taiki26_cmd_10[], taiki26_cmd_11[], taiki26_cmd_12[], taiki26_cmd_13[],
	taiki26_cmd_14[], taiki26_cmd_15[], taiki26_cmd_16[], taiki26_cmd_17[];

const CPU_DATA taiki26_tbl[] = {
	{ ( Sint32 )taiki26_rate, ( Sint32 )taiki26_sel, ( Sint32 )taiki26_cmd },
};

const Uint8 taiki26_rate[] = {
	 13,  14,  15,  16,  16,  17,  17,  18,  18,  19,  20,  20,  22,  23,  24,  25,
	 26,  27,  28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 *const taiki26_sel[] = {
	taiki26_sel_0, taiki26_sel_1, taiki26_sel_2, taiki26_sel_3, taiki26_sel_4, taiki26_sel_5,
	taiki26_sel_6, taiki26_sel_7, taiki26_sel_8,
};

const Uint8 taiki26_sel_0[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
	  3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,
};

const Uint8 taiki26_sel_1[] = {
	  3,   2,   2,   2,   2,   2,   2,   2,   3,   2,   2,   2,   2,   2,   2,   2,
	  3,   6,   6,   6,   6,   6,   6,   6,   3,   6,   6,   6,   6,   6,   6,   6,
};

const Uint8 taiki26_sel_2[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
	  6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
};

const Uint8 taiki26_sel_3[] = {
	  6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
	  4,   4,   4,   4,   4,   4,   4,   4,   5,   5,   5,   5,   5,   5,   5,   5,
};

const Uint8 taiki26_sel_4[] = {
	  5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,
	  6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
};

const Uint8 taiki26_sel_5[] = {
	  6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
	  8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
};

const Uint8 taiki26_sel_6[] = {
	 10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,
	  8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
};

const Uint8 taiki26_sel_7[] = {
	  8,  10,  10,  10,  10,  10,  10,  10,   8,  10,  10,  10,  10,  10,  10,  10,
	  8,  10,  10,  10,  10,  10,  10,  10,   8,  10,  10,  10,  10,  10,  10,  10,
};

const Uint8 taiki26_sel_8[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,
};

const Uint8 *const taiki26_cmd[] = {
	taiki26_cmd_00, taiki26_cmd_01, taiki26_cmd_02, taiki26_cmd_03, taiki26_cmd_04, taiki26_cmd_05,
	taiki26_cmd_06, taiki26_cmd_07, taiki26_cmd_08, taiki26_cmd_09, taiki26_cmd_10, taiki26_cmd_11,
	taiki26_cmd_12, taiki26_cmd_13, taiki26_cmd_14, taiki26_cmd_15, taiki26_cmd_16, taiki26_cmd_17,
};

const Uint8 taiki26_cmd_00[] = {
	WAIT( 30 ),
	CMD_RESET,
};

const Uint8 taiki26_cmd_01[] = {
	WALK_B( 8 ),
	WAIT2( 14 ),
	CMD_RESET,
};

const Uint8 taiki26_cmd_02[] = {
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

const Uint8 taiki26_cmd_03[] = {
	STAND,
	WAIT( 40 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_RESET,
};

const Uint8 taiki26_cmd_04[] = {
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

const Uint8 taiki26_cmd_05[] = {
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

const Uint8 taiki26_cmd_06[] = {
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

const Uint8 taiki26_cmd_07[] = {
	CMD_RESET,
};

const Uint8 taiki26_cmd_08[] = {
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

const Uint8 taiki26_cmd_09[] = {
	CMD_RESET,
};

const Uint8 taiki26_cmd_10[] = {
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

const Uint8 taiki26_cmd_11[] = {
	CMD_RESET,
};

const Uint8 taiki26_cmd_12[] = {
	CMD_RESET,
};

const Uint8 taiki26_cmd_13[] = {
	CMD_RESET,
};

const Uint8 taiki26_cmd_14[] = {
	CMD_RESET,
};

const Uint8 taiki26_cmd_15[] = {
	CMD_RESET,
};

const Uint8 taiki26_cmd_16[] = {
	CMD_RESET,
};

const Uint8 taiki26_cmd_17[] = {
	CMD_RESET,
};

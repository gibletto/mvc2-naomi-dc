/* Megaman (character 1c): the computer's waiting lists.
   What it does while nothing calls for more. PLAYER_WORK's cpu_adrs[0] points here, and em_cmd_sel
   picks a list from it.

   taiki1c_tbl     one record (CPU_DATA): the offsets of the three tables.
   taiki1c_rate    the chance, out of 32, that a list's CMD_NEXT goes over to an attack list
                   (atck1c.c), by the level of play.
   taiki1c_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   taiki1c_cmd     the lists: a pick is a number in this table.
   taiki1c_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                   list of lib1c.c. */
#include "player.h"
#include "cpu_cmd.h"

extern const Uint8 *const taiki1c_sel[], *const taiki1c_cmd[];
extern const Uint8 taiki1c_rate[], taiki1c_sel_0[], taiki1c_sel_1[], taiki1c_sel_2[],
	taiki1c_sel_3[], taiki1c_sel_4[], taiki1c_sel_5[], taiki1c_sel_6[], taiki1c_sel_7[],
	taiki1c_sel_8[], taiki1c_cmd_00[], taiki1c_cmd_01[], taiki1c_cmd_02[], taiki1c_cmd_03[],
	taiki1c_cmd_04[], taiki1c_cmd_05[], taiki1c_cmd_06[], taiki1c_cmd_07[], taiki1c_cmd_08[],
	taiki1c_cmd_09[], taiki1c_cmd_10[], taiki1c_cmd_11[], taiki1c_cmd_12[], taiki1c_cmd_13[],
	taiki1c_cmd_14[], taiki1c_cmd_15[], taiki1c_cmd_16[], taiki1c_cmd_17[];

const CPU_DATA taiki1c_tbl[] = {
	{ ( Sint32 )taiki1c_rate, ( Sint32 )taiki1c_sel, ( Sint32 )taiki1c_cmd },
};

const Uint8 taiki1c_rate[] = {
	 13,  14,  15,  16,  16,  17,  17,  18,  18,  19,  20,  20,  22,  23,  24,  25,
	 26,  27,  28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 *const taiki1c_sel[] = {
	taiki1c_sel_0, taiki1c_sel_1, taiki1c_sel_2, taiki1c_sel_3, taiki1c_sel_4, taiki1c_sel_5,
	taiki1c_sel_6, taiki1c_sel_7, taiki1c_sel_8,
};

const Uint8 taiki1c_sel_0[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
	  3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,
};

const Uint8 taiki1c_sel_1[] = {
	  3,   2,   2,   2,   2,   2,   2,   2,   3,   2,   2,   2,   2,   2,   2,   2,
	  3,   6,   6,   6,   6,   6,   6,   6,   3,   6,   6,   6,   6,   6,   6,   6,
};

const Uint8 taiki1c_sel_2[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
	  6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
};

const Uint8 taiki1c_sel_3[] = {
	  6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
	  4,   4,   4,   4,   4,   4,   4,   4,   5,   5,   5,   5,   5,   5,   5,   5,
};

const Uint8 taiki1c_sel_4[] = {
	  5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,
	  6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
};

const Uint8 taiki1c_sel_5[] = {
	  6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
	  8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
};

const Uint8 taiki1c_sel_6[] = {
	 10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,
	  8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
};

const Uint8 taiki1c_sel_7[] = {
	  8,  10,  10,  10,  10,  10,  10,  10,   8,  10,  10,  10,  10,  10,  10,  10,
	  8,  10,  10,  10,  10,  10,  10,  10,   8,  10,  10,  10,  10,  10,  10,  10,
};

const Uint8 taiki1c_sel_8[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
};

const Uint8 *const taiki1c_cmd[] = {
	taiki1c_cmd_00, taiki1c_cmd_01, taiki1c_cmd_02, taiki1c_cmd_03, taiki1c_cmd_04, taiki1c_cmd_05,
	taiki1c_cmd_06, taiki1c_cmd_07, taiki1c_cmd_08, taiki1c_cmd_09, taiki1c_cmd_10, taiki1c_cmd_11,
	taiki1c_cmd_12, taiki1c_cmd_13, taiki1c_cmd_14, taiki1c_cmd_15, taiki1c_cmd_16, taiki1c_cmd_17,
};

const Uint8 taiki1c_cmd_00[] = {
	WAIT( 30 ),
	CMD_RESET,
};

const Uint8 taiki1c_cmd_01[] = {
	WALK_B( 8 ),
	WAIT2( 14 ),
	CMD_RESET,
};

const Uint8 taiki1c_cmd_02[] = {
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

const Uint8 taiki1c_cmd_03[] = {
	STAND,
	WAIT( 40 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_RESET,
};

const Uint8 taiki1c_cmd_04[] = {
	STAND,
	WAIT2( 8 ),
	JUMP( DIR_BACK ),
	CMD_NONE( 41 ),
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT( 20 ),
	WALK_B( 64 ),
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
	JUMP( DIR_FORWARD ),
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT2( 8 ),
	STAND,
	CMD_RESET,
};

const Uint8 taiki1c_cmd_05[] = {
	CMD_NEXT,
	STAND,
	WAIT2( 14 ),
	CMD_NEXT,
	WALK_B( 16 ),
	JUMP( DIR_BACK ),
	CMD_NONE( 41 ),
	STAND,
	WAIT( 10 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_NEXT,
	JUMP( DIR_BACK ),
	CMD_NONE( 41 ),
	STAND,
	WAIT( 10 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_NEXT,
	STAND,
	CMD_RESET,
};

const Uint8 taiki1c_cmd_06[] = {
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

const Uint8 taiki1c_cmd_07[] = {
	CMD_RESET,
};

const Uint8 taiki1c_cmd_08[] = {
	STAND,
	WALK_B( 32 ),
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_NEXT,
	WALK_EM( 128 ),
	STAND,
	WAIT2( 8 ),
	WALK_F( 16 ),
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
	CMD_NONE( 41 ),
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
	WALK_EM( 96 ),
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
	WALK_EM( 128 ),
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_NEXT,
	STAND,
	CMD_RESET,
};

const Uint8 taiki1c_cmd_09[] = {
	CMD_RESET,
};

const Uint8 taiki1c_cmd_10[] = {
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

const Uint8 taiki1c_cmd_11[] = {
	CMD_RESET,
};

const Uint8 taiki1c_cmd_12[] = {
	CMD_RESET,
};

const Uint8 taiki1c_cmd_13[] = {
	CMD_RESET,
};

const Uint8 taiki1c_cmd_14[] = {
	CMD_RESET,
};

const Uint8 taiki1c_cmd_15[] = {
	CMD_RESET,
};

const Uint8 taiki1c_cmd_16[] = {
	CMD_RESET,
};

const Uint8 taiki1c_cmd_17[] = {
	CMD_RESET,
};

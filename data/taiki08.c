/* Psylocke (character 08): the computer's waiting lists.
   What it does while nothing calls for more. PLAYER_WORK's cpu_adrs[0] points here, and em_cmd_sel
   picks a list from it.

   taiki08_tbl     one record (CPU_DATA): the offsets of the three tables.
   taiki08_rate    the chance, out of 32, that a list's CMD_NEXT goes over to an attack list
                   (atck08.c), by the level of play.
   taiki08_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   taiki08_cmd     the lists: a pick is a number in this table.
   taiki08_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                   list of lib08.c. */
#include "player.h"
#include "cpu_cmd.h"

extern const Uint8 *const taiki08_sel[], *const taiki08_cmd[];
extern const Uint8 taiki08_rate[], taiki08_sel_0[], taiki08_sel_1[], taiki08_sel_2[],
	taiki08_sel_3[], taiki08_sel_4[], taiki08_sel_5[], taiki08_sel_6[], taiki08_sel_7[],
	taiki08_sel_8[], taiki08_cmd_00[], taiki08_cmd_01[], taiki08_cmd_02[], taiki08_cmd_03[],
	taiki08_cmd_04[], taiki08_cmd_05[], taiki08_cmd_06[], taiki08_cmd_07[], taiki08_cmd_08[],
	taiki08_cmd_09[], taiki08_cmd_10[], taiki08_cmd_11[], taiki08_cmd_12[], taiki08_cmd_13[],
	taiki08_cmd_14[], taiki08_cmd_15[], taiki08_cmd_16[], taiki08_cmd_17[];

const CPU_DATA taiki08_tbl[] = {
	{ ( Sint32 )taiki08_rate, ( Sint32 )taiki08_sel, ( Sint32 )taiki08_cmd },
};

const Uint8 taiki08_rate[] = {
	 13,  14,  15,  16,  16,  17,  17,  18,  18,  19,  20,  20,  22,  23,  24,  25,
	 26,  27,  28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 *const taiki08_sel[] = {
	taiki08_sel_0, taiki08_sel_1, taiki08_sel_2, taiki08_sel_3, taiki08_sel_4, taiki08_sel_5,
	taiki08_sel_6, taiki08_sel_7, taiki08_sel_8,
};

const Uint8 taiki08_sel_0[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
	  3,   3,   3,   3,   3,   3,   3,   3,   2,   2,   2,   2,   2,   2,   2,   2,
};

const Uint8 taiki08_sel_1[] = {
	  3,   2,   2,   2,   2,   2,   2,   2,   3,   2,   2,   2,   2,   2,   2,   2,
	  3,   6,   6,   6,   6,   6,   6,   6,   3,   6,   6,   6,   6,   6,   6,   6,
};

const Uint8 taiki08_sel_2[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
	  6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
};

const Uint8 taiki08_sel_3[] = {
	  6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
	  4,   4,   4,   4,   4,   4,   4,   4,   5,   5,   5,   5,   5,   5,   5,   5,
};

const Uint8 taiki08_sel_4[] = {
	  5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,
	  6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
};

const Uint8 taiki08_sel_5[] = {
	  6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
	  8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
};

const Uint8 taiki08_sel_6[] = {
	 10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,
	  8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
};

const Uint8 taiki08_sel_7[] = {
	  8,  10,  10,  10,  10,  10,  10,  10,   8,  10,  10,  10,  10,  10,  10,  10,
	  8,  10,  10,  10,  10,  10,  10,  10,   8,  10,  10,  10,  10,  10,  10,  10,
};

const Uint8 taiki08_sel_8[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,
};

const Uint8 *const taiki08_cmd[] = {
	taiki08_cmd_00, taiki08_cmd_01, taiki08_cmd_02, taiki08_cmd_03, taiki08_cmd_04, taiki08_cmd_05,
	taiki08_cmd_06, taiki08_cmd_07, taiki08_cmd_08, taiki08_cmd_09, taiki08_cmd_10, taiki08_cmd_11,
	taiki08_cmd_12, taiki08_cmd_13, taiki08_cmd_14, taiki08_cmd_15, taiki08_cmd_16, taiki08_cmd_17,
};

const Uint8 taiki08_cmd_00[] = {
	WAIT( 10 ),
	WALK_B( 32 ),
	WAIT2( 14 ),
	WALK_B( 16 ),
	CMD_RESET,
};

const Uint8 taiki08_cmd_01[] = {
	WALK_F( 16 ),
	WAIT( 10 ),
	WALK_B( 16 ),
	WAIT2( 14 ),
	CMD_RESET,
};

const Uint8 taiki08_cmd_02[] = {
	STAND,
	WAIT2( 14 ),
	WAIT( 20 ),
	WALK_EM( 64 ),
	STAND,
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

const Uint8 taiki08_cmd_03[] = {
	STAND,
	WAIT( 40 ),
	CMD_NEXT,
	WAIT2( 14 ),
	CMD_RESET,
};

const Uint8 taiki08_cmd_04[] = {
	CROUCH,
	WAIT2( 14 ),
	JUMP( DIR_BACK ),
	STAND,
	WAIT2( 14 ),
	CMD_NEXT,
	WAIT( 20 ),
	DASH_B( 512 ),
	STAND,
	WAIT( 20 ),
	JUMP( DIR_FORWARD ),
	WAIT( 20 ),
	AIR_JUMP( DIR_FORWARD ),
	STAND,
	CMD_NEXT,
	WAIT2( 14 ),
	JUMP( DIR_BACK ),
	CROUCH,
	WAIT( 20 ),
	CMD_NEXT,
	WAIT2( 14 ),
	WALK_DIST( 80 ),
	STAND,
	CMD_NEXT,
	STAND,
	WAIT2( 14 ),
	JUMP( DIR_UP ),
	STAND,
	WAIT2( 14 ),
	CMD_NEXT,
	WAIT2( 14 ),
	STAND,
	CMD_RESET,
};

const Uint8 taiki08_cmd_05[] = {
	CMD_NEXT,
	STAND,
	WAIT2( 14 ),
	CMD_NEXT,
	WALK_B( 16 ),
	JUMP( DIR_BACK ),
	CROUCH,
	WAIT( 10 ),
	CMD_NEXT,
	WAIT2( 14 ),
	CMD_NEXT,
	JUMP( DIR_BACK ),
	STAND,
	WAIT( 10 ),
	CMD_NEXT,
	WAIT2( 14 ),
	CMD_NEXT,
	STAND,
	CMD_RESET,
};

const Uint8 taiki08_cmd_06[] = {
	STAND,
	CMD_NEXT,
	WAIT( 10 ),
	CMD_NEXT,
	WALK_EM( 80 ),
	CROUCH,
	CMD_NEXT,
	WAIT2( 14 ),
	CMD_NEXT,
	WALK_B( 96 ),
	CMD_NEXT,
	WAIT( 10 ),
	CMD_NEXT,
	STAND,
	WAIT2( 14 ),
	CMD_NEXT,
	WAIT( 10 ),
	CMD_NEXT,
	WALK_EM( 64 ),
	STAND,
	CMD_NEXT,
	CROUCH,
	WAIT2( 14 ),
	CMD_NEXT,
	WALK_EM( 96 ),
	STAND,
	CMD_NEXT,
	STAND,
	WAIT2( 14 ),
	CMD_NEXT,
	WALK_B( 48 ),
	CROUCH,
	WAIT( 10 ),
	CMD_NEXT,
	WAIT2( 14 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki08_cmd_07[] = {
	CMD_RESET,
};

const Uint8 taiki08_cmd_08[] = {
	STAND,
	DASH_B( 256 ),
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_NEXT,
	WALK_EM( 80 ),
	CMD_NEXT,
	CROUCH,
	WAIT2( 8 ),
	DASH_EM( 64 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_NEXT,
	WALK_DIST( 80 ),
	CMD_NEXT,
	STAND,
	WAIT2( 8 ),
	JUMP( DIR_BACK ),
	CROUCH,
	WAIT( 10 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_NEXT,
	CROUCH,
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT2( 14 ),
	CMD_NEXT,
	DASH_EM( 80 ),
	STAND,
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_NEXT,
	WALK_DIST( 96 ),
	STAND,
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_NEXT,
	DASH_B( 800 ),
	CROUCH,
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_NEXT,
	STAND,
	CMD_RESET,
};

const Uint8 taiki08_cmd_09[] = {
	CMD_RESET,
};

const Uint8 taiki08_cmd_10[] = {
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

const Uint8 taiki08_cmd_11[] = {
	CMD_RESET,
};

const Uint8 taiki08_cmd_12[] = {
	CMD_RESET,
};

const Uint8 taiki08_cmd_13[] = {
	CMD_RESET,
};

const Uint8 taiki08_cmd_14[] = {
	CMD_RESET,
};

const Uint8 taiki08_cmd_15[] = {
	CMD_RESET,
};

const Uint8 taiki08_cmd_16[] = {
	CMD_RESET,
};

const Uint8 taiki08_cmd_17[] = {
	CMD_RESET,
};

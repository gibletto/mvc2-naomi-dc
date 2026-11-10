/* Storm (character 2a): the computer's waiting lists.
   What it does while nothing calls for more. PLAYER_WORK's cpu_adrs[0] points here, and em_cmd_sel
   picks a list from it.

   taiki2a_tbl     one record (CPU_DATA): the offsets of the three tables.
   taiki2a_rate    the chance, out of 32, that a list's CMD_NEXT goes over to an attack list
                   (atck2a.c), by the level of play.
   taiki2a_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   taiki2a_cmd     the lists: a pick is a number in this table.
   taiki2a_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                   list of lib2a.c. */
#include "player.h"
#include "cpu_cmd.h"

extern const Uint8 *const taiki2a_sel[], *const taiki2a_cmd[];
extern const Uint8 taiki2a_rate[], taiki2a_sel_0[], taiki2a_sel_1[], taiki2a_sel_2[],
	taiki2a_sel_3[], taiki2a_sel_4[], taiki2a_sel_5[], taiki2a_sel_6[], taiki2a_sel_7[],
	taiki2a_sel_8[], taiki2a_cmd_00[], taiki2a_cmd_01[], taiki2a_cmd_02[], taiki2a_cmd_03[],
	taiki2a_cmd_04[], taiki2a_cmd_05[], taiki2a_cmd_06[], taiki2a_cmd_07[], taiki2a_cmd_08[],
	taiki2a_cmd_09[], taiki2a_cmd_10[], taiki2a_cmd_11[], taiki2a_cmd_12[], taiki2a_cmd_13[],
	taiki2a_cmd_14[], taiki2a_cmd_15[], taiki2a_cmd_16[], taiki2a_cmd_17[];

const CPU_DATA taiki2a_tbl[] = {
	{ ( Sint32 )taiki2a_rate, ( Sint32 )taiki2a_sel, ( Sint32 )taiki2a_cmd },
};

const Uint8 taiki2a_rate[] = {
	 13,  14,  15,  16,  16,  17,  17,  18,  18,  19,  20,  20,  22,  23,  24,  25,
	 26,  27,  28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 *const taiki2a_sel[] = {
	taiki2a_sel_0, taiki2a_sel_1, taiki2a_sel_2, taiki2a_sel_3, taiki2a_sel_4, taiki2a_sel_5,
	taiki2a_sel_6, taiki2a_sel_7, taiki2a_sel_8,
};

const Uint8 taiki2a_sel_0[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
	  3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,
};

const Uint8 taiki2a_sel_1[] = {
	  3,   2,   2,   2,   2,   2,   2,   2,   3,   2,   2,   2,   2,   2,   2,   2,
	  3,   6,   6,   6,   6,   6,   6,   6,   3,   6,   6,   6,   6,   6,   6,   6,
};

const Uint8 taiki2a_sel_2[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
	  6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
};

const Uint8 taiki2a_sel_3[] = {
	  3,   6,   6,   6,   6,   6,   6,   6,   3,   6,   6,   6,   6,   6,   6,   6,
	  3,   4,   4,   4,   4,   4,   4,   4,   3,   5,   5,   5,   5,   5,   5,   5,
};

const Uint8 taiki2a_sel_4[] = {
	  3,   5,   5,   5,   5,   5,   5,   5,   3,   5,   5,   5,   5,   5,   5,   5,
	  3,   6,   6,   6,   6,   6,   6,   6,   3,   6,   6,   6,   6,   6,   6,   6,
};

const Uint8 taiki2a_sel_5[] = {
	  3,   6,   6,   6,   6,   6,   6,   6,   3,   6,   6,   6,   6,   6,   6,   6,
	  3,   8,   8,   8,   8,   8,   8,   8,   3,   8,   8,   8,   8,   8,   8,   8,
};

const Uint8 taiki2a_sel_6[] = {
	  3,  10,  10,  10,  10,  10,  10,  10,   3,  10,  10,  10,  10,  10,  10,  10,
	  3,   8,   8,   8,   8,   8,   8,   8,   3,   8,   8,   8,   8,   8,   8,   8,
};

const Uint8 taiki2a_sel_7[] = {
	  3,  10,  10,  10,  10,  10,  10,  10,   3,  10,  10,  10,  10,  10,  10,  10,
	  3,  10,  10,  10,  10,  10,  10,  10,   3,  10,  10,  10,  10,  10,  10,  10,
};

const Uint8 taiki2a_sel_8[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,
};

const Uint8 *const taiki2a_cmd[] = {
	taiki2a_cmd_00, taiki2a_cmd_01, taiki2a_cmd_02, taiki2a_cmd_03, taiki2a_cmd_04, taiki2a_cmd_05,
	taiki2a_cmd_06, taiki2a_cmd_07, taiki2a_cmd_08, taiki2a_cmd_09, taiki2a_cmd_10, taiki2a_cmd_11,
	taiki2a_cmd_12, taiki2a_cmd_13, taiki2a_cmd_14, taiki2a_cmd_15, taiki2a_cmd_16, taiki2a_cmd_17,
};

const Uint8 taiki2a_cmd_00[] = {
	WAIT( 10 ),
	WALK_RND( 4 ),
	CMD_RESET,
};

const Uint8 taiki2a_cmd_01[] = {
	WALK_F( 64 ),
	WAIT2( 14 ),
	CMD_RESET,
};

const Uint8 taiki2a_cmd_02[] = {
	STAND,
	WAIT2( 14 ),
	WAIT( 20 ),
	TIMER_SET( 30 ),
	WALK_EM( 64 ),
	STAND,
	WAIT2( 14 ),
	CMD_NEXT,
	WAIT( 20 ),
	TIMER_SET( 30 ),
	WALK_B( 48 ),
	STAND,
	WAIT2( 14 ),
	STAND,
	WAIT( 20 ),
	CMD_NEXT,
	STAND,
	CMD_RESET,
};

const Uint8 taiki2a_cmd_03[] = {
	STAND,
	WAIT2( 6 ),
	CMD_NEXT,
	WAIT( 1 ),
	WALK_B( 32 ),
	CMD_NEXT,
	WAIT( 1 ),
	WALK_B( 32 ),
	CMD_NEXT,
	WAIT( 1 ),
	WALK_B( 32 ),
	CMD_NEXT,
	WAIT( 1 ),
	DASH_B( 256 ),
	WAIT( 1 ),
	CMD_NEXT,
	CROUCH,
	WAIT( 10 ),
	CMD_NEXT,
	WAIT( 10 ),
	CMD_NEXT,
	WAIT( 10 ),
	CMD_NEXT,
	WAIT( 10 ),
	CMD_NEXT,
	WAIT( 10 ),
	CMD_NEXT,
	WAIT( 10 ),
	CMD_NEXT,
	STAND,
	DASH_F( 256 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki2a_cmd_04[] = {
	CROUCH,
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
	CROUCH,
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

const Uint8 taiki2a_cmd_05[] = {
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

const Uint8 taiki2a_cmd_06[] = {
	STAND,
	CMD_NEXT,
	WAIT( 10 ),
	CMD_NEXT,
	WALK_EM( 80 ),
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_NEXT,
	WALK_B( 16 ),
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
	WALK_EM( 96 ),
	STAND,
	CMD_NEXT,
	CROUCH,
	WAIT2( 8 ),
	CMD_NEXT,
	WALK_EM( 80 ),
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	CROUCH,
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

const Uint8 taiki2a_cmd_07[] = {
	CMD_RESET,
};

const Uint8 taiki2a_cmd_08[] = {
	STAND,
	DASH_B( 256 ),
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_NEXT,
	WALK_EM( 96 ),
	CROUCH,
	WAIT2( 8 ),
	DASH_EM( 64 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_NEXT,
	WALK_DIST( 80 ),
	STAND,
	CMD_NEXT,
	CROUCH,
	WAIT2( 8 ),
	JUMP( DIR_BACK ),
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	CROUCH,
	WAIT2( 8 ),
	CMD_NEXT,
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_NEXT,
	DASH_EM( 80 ),
	STAND,
	CMD_NEXT,
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	WALK_DIST( 96 ),
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_NEXT,
	DASH_B( 512 ),
	CROUCH,
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_NEXT,
	STAND,
	CMD_RESET,
};

const Uint8 taiki2a_cmd_09[] = {
	CMD_RESET,
};

const Uint8 taiki2a_cmd_10[] = {
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

const Uint8 taiki2a_cmd_11[] = {
	CMD_RESET,
};

const Uint8 taiki2a_cmd_12[] = {
	CMD_RESET,
};

const Uint8 taiki2a_cmd_13[] = {
	CMD_RESET,
};

const Uint8 taiki2a_cmd_14[] = {
	CMD_RESET,
};

const Uint8 taiki2a_cmd_15[] = {
	CMD_RESET,
};

const Uint8 taiki2a_cmd_16[] = {
	CMD_RESET,
};

const Uint8 taiki2a_cmd_17[] = {
	CMD_RESET,
};

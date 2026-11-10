/* Spiderman (character 0c): the computer's waiting lists.
   What it does while nothing calls for more. PLAYER_WORK's cpu_adrs[0] points here, and em_cmd_sel
   picks a list from it.

   taiki0c_tbl     one record (CPU_DATA): the offsets of the three tables.
   taiki0c_rate    the chance, out of 32, that a list's CMD_NEXT goes over to an attack list
                   (atck0c.c), by the level of play.
   taiki0c_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   taiki0c_cmd     the lists: a pick is a number in this table.
   taiki0c_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                   list of lib0c.c. */
#include "player.h"
#include "cpu_cmd.h"

extern const Uint8 *const taiki0c_sel[], *const taiki0c_cmd[];
extern const Uint8 taiki0c_rate[], taiki0c_sel_0[], taiki0c_sel_1[], taiki0c_sel_2[],
	taiki0c_sel_3[], taiki0c_sel_4[], taiki0c_sel_5[], taiki0c_sel_6[], taiki0c_sel_7[],
	taiki0c_sel_8[], taiki0c_cmd_00[], taiki0c_cmd_01[], taiki0c_cmd_02[], taiki0c_cmd_03[],
	taiki0c_cmd_04[], taiki0c_cmd_05[], taiki0c_cmd_06[], taiki0c_cmd_07[], taiki0c_cmd_08[],
	taiki0c_cmd_09[], taiki0c_cmd_10[], taiki0c_cmd_11[], taiki0c_cmd_12[], taiki0c_cmd_13[],
	taiki0c_cmd_14[], taiki0c_cmd_15[], taiki0c_cmd_16[], taiki0c_cmd_17[];

const CPU_DATA taiki0c_tbl[] = {
	{ ( Sint32 )taiki0c_rate, ( Sint32 )taiki0c_sel, ( Sint32 )taiki0c_cmd },
};

const Uint8 taiki0c_rate[] = {
	 13,  14,  15,  16,  16,  17,  17,  18,  18,  19,  20,  20,  22,  23,  24,  25,
	 26,  27,  28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 *const taiki0c_sel[] = {
	taiki0c_sel_0, taiki0c_sel_1, taiki0c_sel_2, taiki0c_sel_3, taiki0c_sel_4, taiki0c_sel_5,
	taiki0c_sel_6, taiki0c_sel_7, taiki0c_sel_8,
};

const Uint8 taiki0c_sel_0[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  2,   2,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,   3,   3,   3,
};

const Uint8 taiki0c_sel_1[] = {
	  3,   3,   3,   3,   2,   2,   2,   2,   3,   3,   4,   4,   4,   4,   4,   4,
	  4,   4,   4,   5,   5,   5,   5,   5,   4,   4,   5,   5,   5,   5,   5,   5,
};

const Uint8 taiki0c_sel_2[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
	  6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
};

const Uint8 taiki0c_sel_3[] = {
	  6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
	  4,   4,   4,   4,   4,   4,   4,   4,   5,   5,   5,   5,   5,   5,   5,   5,
};

const Uint8 taiki0c_sel_4[] = {
	  5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,
	  6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
};

const Uint8 taiki0c_sel_5[] = {
	  6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
	  8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
};

const Uint8 taiki0c_sel_6[] = {
	 10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,
	  8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
};

const Uint8 taiki0c_sel_7[] = {
	  8,  10,  10,  10,  10,  10,  10,  10,   8,  10,  10,  10,  10,  10,  10,  10,
	  8,  10,  10,  10,  10,  10,  10,  10,   8,  10,  10,  10,  10,  10,  10,  10,
};

const Uint8 taiki0c_sel_8[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
};

const Uint8 *const taiki0c_cmd[] = {
	taiki0c_cmd_00, taiki0c_cmd_01, taiki0c_cmd_02, taiki0c_cmd_03, taiki0c_cmd_04, taiki0c_cmd_05,
	taiki0c_cmd_06, taiki0c_cmd_07, taiki0c_cmd_08, taiki0c_cmd_09, taiki0c_cmd_10, taiki0c_cmd_11,
	taiki0c_cmd_12, taiki0c_cmd_13, taiki0c_cmd_14, taiki0c_cmd_15, taiki0c_cmd_16, taiki0c_cmd_17,
};

const Uint8 taiki0c_cmd_00[] = {
	WAIT( 48 ),
	CMD_RESET,
};

const Uint8 taiki0c_cmd_01[] = {
	WALK_B( 8 ),
	WAIT2( 4 ),
	WALK_F( 16 ),
	WAIT( 16 ),
	CMD_RESET,
};

const Uint8 taiki0c_cmd_02[] = {
	STAND,
	WAIT( 16 ),
	JUMP( DIR_FORWARD ),
	WAIT2( 6 ),
	DASH_B( 256 ),
	WAIT( 16 ),
	CMD_NEXT,
	WAIT2( 10 ),
	JUMP( DIR_BACK ),
	WAIT2( 6 ),
	CROUCH,
	WAIT2( 6 ),
	WALK_F( 48 ),
	CMD_NEXT,
	WAIT2( 10 ),
	STAND,
	JUMP( DIR_BACK ),
	STAND,
	WAIT2( 6 ),
	CMD_NEXT,
	WAIT2( 6 ),
	CROUCH,
	WAIT2( 6 ),
	JUMP( DIR_FORWARD ),
	WAIT2( 6 ),
	WALK_B( 80 ),
	WAIT2( 4 ),
	CMD_NEXT,
	WAIT2( 10 ),
	CROUCH,
	WAIT2( 10 ),
	CMD_RESET,
};

const Uint8 taiki0c_cmd_03[] = {
	STAND,
	WAIT2( 6 ),
	DASH_B( 256 ),
	WAIT( 16 ),
	CROUCH,
	WAIT2( 8 ),
	CMD_NEXT,
	JUMP( DIR_FORWARD ),
	WAIT2( 8 ),
	WALK_B( 80 ),
	STAND,
	WAIT2( 4 ),
	CMD_NEXT,
	CROUCH,
	WAIT2( 10 ),
	STAND,
	WALK_F( 80 ),
	STAND,
	WAIT2( 6 ),
	CMD_NEXT,
	WAIT2( 4 ),
	JUMP( DIR_BACK ),
	STAND,
	JUMP( DIR_BACK ),
	CROUCH,
	WAIT2( 10 ),
	STAND,
	CMD_NEXT,
	WAIT2( 10 ),
	STAND,
	WAIT2( 4 ),
	CMD_RESET,
};

const Uint8 taiki0c_cmd_04[] = {
	CROUCH,
	WAIT2( 8 ),
	JUMP( DIR_BACK ),
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT( 16 ),
	CMD_NEXT,
	STAND,
	WAIT2( 8 ),
	JUMP( DIR_FORWARD ),
	STAND,
	WAIT2( 12 ),
	CMD_NEXT,
	WAIT( 16 ),
	JUMP( DIR_BACK ),
	CROUCH,
	WAIT( 16 ),
	CMD_NEXT,
	WAIT2( 6 ),
	WALK_EM( 160 ),
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	STAND,
	WAIT( 16 ),
	JUMP( DIR_UP ),
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT( 16 ),
	STAND,
	CMD_RESET,
};

const Uint8 taiki0c_cmd_05[] = {
	CMD_NEXT,
	STAND,
	WAIT( 16 ),
	CMD_NEXT,
	WALK_B( 16 ),
	JUMP( DIR_BACK ),
	STAND,
	WAIT( 16 ),
	CMD_NEXT,
	WAIT( 16 ),
	CMD_NEXT,
	WAIT( 16 ),
	CMD_NEXT,
	WAIT( 16 ),
	CMD_NEXT,
	JUMP( DIR_BACK ),
	STAND,
	JUMP( DIR_BACK ),
	STAND,
	WAIT2( 10 ),
	CROUCH,
	WAIT( 16 ),
	CMD_RESET,
};

const Uint8 taiki0c_cmd_06[] = {
	STAND,
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_NEXT,
	WALK_EM( 80 ),
	STAND,
	WAIT( 16 ),
	CMD_NEXT,
	WAIT( 16 ),
	CMD_NEXT,
	WALK_B( 16 ),
	WAIT( 16 ),
	CMD_NEXT,
	STAND,
	WAIT( 16 ),
	CMD_NEXT,
	STAND,
	WAIT( 16 ),
	CMD_NEXT,
	STAND,
	WAIT( 16 ),
	CMD_NEXT,
	WALK_EM( 96 ),
	STAND,
	WAIT( 16 ),
	CMD_NEXT,
	CROUCH,
	WAIT( 16 ),
	CMD_NEXT,
	WALK_EM( 80 ),
	STAND,
	WAIT( 16 ),
	CMD_NEXT,
	CROUCH,
	WAIT( 16 ),
	CMD_NEXT,
	WALK_B( 32 ),
	STAND,
	WAIT( 16 ),
	CMD_NEXT,
	STAND,
	WAIT( 16 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki0c_cmd_07[] = {
	CMD_RESET,
};

const Uint8 taiki0c_cmd_08[] = {
	STAND,
	DASH_B( 256 ),
	STAND,
	WAIT( 16 ),
	CMD_NEXT,
	WAIT( 16 ),
	CMD_NEXT,
	WALK_EM( 96 ),
	CROUCH,
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT( 16 ),
	CMD_NEXT,
	WAIT( 16 ),
	CMD_NEXT,
	WALK_DIST( 80 ),
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	CROUCH,
	WAIT2( 8 ),
	JUMP( DIR_BACK ),
	STAND,
	WAIT( 16 ),
	CMD_NEXT,
	CROUCH,
	WAIT( 16 ),
	CMD_NEXT,
	STAND,
	WAIT( 16 ),
	CMD_NEXT,
	WAIT( 16 ),
	CMD_NEXT,
	STAND,
	WAIT( 16 ),
	CMD_NEXT,
	STAND,
	WAIT( 16 ),
	CMD_NEXT,
	WALK_DIST( 96 ),
	STAND,
	WAIT( 16 ),
	CMD_NEXT,
	WAIT( 16 ),
	CMD_NEXT,
	WAIT2( 12 ),
	DASH_B( 512 ),
	CROUCH,
	WAIT( 16 ),
	CMD_NEXT,
	WAIT( 16 ),
	CMD_NEXT,
	STAND,
	CMD_RESET,
};

const Uint8 taiki0c_cmd_09[] = {
	CMD_RESET,
};

const Uint8 taiki0c_cmd_10[] = {
	CMD_NEXT,
	WAIT( 16 ),
	CMD_NEXT,
	WAIT( 16 ),
	CMD_NEXT,
	WAIT( 16 ),
	CMD_NEXT,
	WAIT( 16 ),
	CMD_NEXT,
	WAIT( 16 ),
	CMD_NEXT,
	WAIT( 16 ),
	CMD_NEXT,
	WAIT( 16 ),
	CMD_NEXT,
	WAIT( 16 ),
	CMD_RESET,
};

const Uint8 taiki0c_cmd_11[] = {
	CMD_RESET,
};

const Uint8 taiki0c_cmd_12[] = {
	CMD_RESET,
};

const Uint8 taiki0c_cmd_13[] = {
	CMD_RESET,
};

const Uint8 taiki0c_cmd_14[] = {
	CMD_RESET,
};

const Uint8 taiki0c_cmd_15[] = {
	CMD_RESET,
};

const Uint8 taiki0c_cmd_16[] = {
	CMD_RESET,
};

const Uint8 taiki0c_cmd_17[] = {
	CMD_RESET,
};

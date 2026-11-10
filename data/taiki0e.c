/* Venom (character 0e): the computer's waiting lists.
   What it does while nothing calls for more. PLAYER_WORK's cpu_adrs[0] points here, and em_cmd_sel
   picks a list from it.

   taiki0e_tbl     one record (CPU_DATA): the offsets of the three tables.
   taiki0e_rate    the chance, out of 32, that a list's CMD_NEXT goes over to an attack list
                   (atck0e.c), by the level of play.
   taiki0e_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   taiki0e_cmd     the lists: a pick is a number in this table.
   taiki0e_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                   list of lib0e.c. */
#include "player.h"
#include "cpu_cmd.h"

extern const Uint8 *const taiki0e_sel[], *const taiki0e_cmd[];
extern const Uint8 taiki0e_rate[], taiki0e_sel_0[], taiki0e_sel_1[], taiki0e_sel_2[],
	taiki0e_sel_3[], taiki0e_sel_4[], taiki0e_sel_5[], taiki0e_sel_6[], taiki0e_sel_7[],
	taiki0e_sel_8[], taiki0e_cmd_00[], taiki0e_cmd_01[], taiki0e_cmd_02[], taiki0e_cmd_03[],
	taiki0e_cmd_04[], taiki0e_cmd_05[], taiki0e_cmd_06[], taiki0e_cmd_07[], taiki0e_cmd_08[],
	taiki0e_cmd_09[], taiki0e_cmd_10[], taiki0e_cmd_11[], taiki0e_cmd_12[], taiki0e_cmd_13[],
	taiki0e_cmd_14[], taiki0e_cmd_15[];

const CPU_DATA taiki0e_tbl[] = {
	{ ( Sint32 )taiki0e_rate, ( Sint32 )taiki0e_sel, ( Sint32 )taiki0e_cmd },
};

const Uint8 taiki0e_rate[] = {
	 13,  14,  15,  16,  16,  17,  17,  18,  18,  19,  20,  20,  22,  23,  24,  25,
	 26,  27,  28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 *const taiki0e_sel[] = {
	taiki0e_sel_0, taiki0e_sel_1, taiki0e_sel_2, taiki0e_sel_3, taiki0e_sel_4, taiki0e_sel_5,
	taiki0e_sel_6, taiki0e_sel_7, taiki0e_sel_8,
};

const Uint8 taiki0e_sel_0[] = {
	  3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,
	  4,   4,   4,   4,   4,   4,   4,   4,   5,   5,   5,   5,   5,   6,   6,   6,
};

const Uint8 taiki0e_sel_1[] = {
	  3,   3,   3,   3,   4,   4,   4,   4,   3,   3,   3,   3,   5,   5,   5,   5,
	  6,   6,   6,   6,   7,   7,   7,   7,   8,   8,   8,   8,   9,   9,   9,   9,
};

const Uint8 taiki0e_sel_2[] = {
	  3,   3,   3,   3,   4,   4,   4,   4,   5,   5,   5,   5,   6,   6,   6,   6,
	  7,   7,   7,   7,   8,   8,   8,   8,   9,   9,   9,   9,  10,  10,  10,  10,
};

const Uint8 taiki0e_sel_3[] = {
	  3,   3,   4,   4,   5,   5,   6,   6,   7,   7,   7,   7,   7,   7,   7,   7,
	  8,   8,   8,   9,   9,   9,   9,  12,  10,  10,  10,  11,  11,  11,  12,  12,
};

const Uint8 taiki0e_sel_4[] = {
	  3,   3,   4,   4,   5,   5,   6,   6,   7,   7,   7,   7,   8,   8,   8,   8,
	  9,   9,   9,   9,  10,  10,  10,  10,  11,  11,  11,  11,  12,  12,  12,  12,
};

const Uint8 taiki0e_sel_5[] = {
	  3,   3,   3,   3,   4,   4,   4,   4,   5,   5,   5,   5,   6,   6,   6,   6,
	  7,   7,   7,   7,   8,   8,   9,   9,  10,  10,  11,  11,  12,  13,  14,  15,
};

const Uint8 taiki0e_sel_6[] = {
	  3,   3,   3,   3,   4,   4,   4,   4,   3,   3,   3,   3,   5,   5,   5,   5,
	  6,   6,   6,   6,   7,   7,  15,  15,   8,   8,  13,  13,   9,   9,  14,  14,
};

const Uint8 taiki0e_sel_7[] = {
	  3,   3,   3,   3,   4,   4,   4,   4,   3,   3,   3,   3,   5,   5,   5,   5,
	  6,   6,   6,   6,   7,   7,   7,   7,  12,  12,  13,  13,  14,  14,  15,  15,
};

const Uint8 taiki0e_sel_8[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,
	  2,   2,   2,   2,   2,   2,   2,   2,   0,   0,   1,   1,   1,   1,   2,   2,
};

const Uint8 *const taiki0e_cmd[] = {
	taiki0e_cmd_00, taiki0e_cmd_01, taiki0e_cmd_02, taiki0e_cmd_03, taiki0e_cmd_04, taiki0e_cmd_05,
	taiki0e_cmd_06, taiki0e_cmd_07, taiki0e_cmd_08, taiki0e_cmd_09, taiki0e_cmd_10, taiki0e_cmd_11,
	taiki0e_cmd_12, taiki0e_cmd_13, taiki0e_cmd_14, taiki0e_cmd_15,
};

const Uint8 taiki0e_cmd_00[] = {
	WAIT( 16 ),
	CMD_RESET,
};

const Uint8 taiki0e_cmd_01[] = {
	WALK_F( 64 ),
	CMD_RESET,
};

const Uint8 taiki0e_cmd_02[] = {
	WALK_B( 96 ),
	CMD_RESET,
};

const Uint8 taiki0e_cmd_03[] = {
	WAIT2( 4 ),
	STAND,
	WALK_F( 64 ),
	CMD_NEXT,
	WAIT2( 4 ),
	STAND,
	WALK_DIST( 192 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki0e_cmd_04[] = {
	WAIT2( 4 ),
	STAND,
	DASH_F( 256 ),
	WAIT( 16 ),
	CMD_NEXT,
	WAIT2( 4 ),
	STAND,
	WALK_DIST( 192 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki0e_cmd_05[] = {
	WAIT2( 4 ),
	STAND,
	JUMP( DIR_FORWARD ),
	CMD_NEXT,
	WAIT2( 4 ),
	STAND,
	WALK_DIST( 192 ),
	WAIT2( 2 ),
	WALK_B( 48 ),
	WAIT2( 2 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki0e_cmd_06[] = {
	WAIT2( 4 ),
	STAND,
	CMD_NEXT,
	WAIT2( 4 ),
	STAND,
	SJUMP( DIR_FORWARD ),
	STAND,
	WAIT2( 4 ),
	WALK_DIST( 192 ),
	WAIT2( 2 ),
	WALK_B( 48 ),
	JUMP( DIR_FORWARD ),
	STAND,
	WAIT2( 2 ),
	CMD_NEXT,
	WAIT2( 2 ),
	CMD_RESET,
};

const Uint8 taiki0e_cmd_07[] = {
	WAIT2( 4 ),
	STAND,
	WALK_B( 64 ),
	CMD_NEXT,
	WAIT2( 4 ),
	STAND,
	WALK_DIST( 192 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki0e_cmd_08[] = {
	WAIT2( 4 ),
	STAND,
	DASH_B( 256 ),
	WAIT( 16 ),
	CMD_NEXT,
	WAIT2( 4 ),
	STAND,
	WALK_DIST( 192 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki0e_cmd_09[] = {
	WAIT2( 4 ),
	STAND,
	JUMP( DIR_BACK ),
	CMD_NEXT,
	WAIT2( 4 ),
	STAND,
	WALK_DIST( 192 ),
	WAIT2( 2 ),
	CMD_NEXT,
	WAIT2( 4 ),
	WALK_B( 48 ),
	WAIT2( 2 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki0e_cmd_10[] = {
	WAIT2( 4 ),
	STAND,
	CMD_NEXT,
	WAIT2( 4 ),
	STAND,
	WAIT2( 2 ),
	SJUMP( DIR_BACK ),
	WAIT2( 8 ),
	WALK_DIST( 192 ),
	WAIT2( 2 ),
	CMD_NEXT,
	WAIT2( 4 ),
	WALK_B( 48 ),
	SJUMP( DIR_BACK ),
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT2( 2 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki0e_cmd_11[] = {
	WAIT2( 2 ),
	CROUCH,
	WAIT( 48 ),
	CMD_NEXT,
	WAIT2( 6 ),
	STAND,
	WALK_B( 96 ),
	CROUCH,
	WAIT( 48 ),
	CMD_NEXT,
	WAIT2( 2 ),
	WALK_B( 48 ),
	WAIT2( 2 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki0e_cmd_12[] = {
	WAIT2( 2 ),
	CROUCH,
	WAIT( 48 ),
	CMD_NEXT,
	WAIT2( 6 ),
	STAND,
	JUMP( DIR_BACK ),
	WAIT2( 2 ),
	JUMP( DIR_BACK ),
	WAIT2( 2 ),
	CROUCH,
	WAIT( 48 ),
	CMD_NEXT,
	WAIT2( 2 ),
	WALK_B( 48 ),
	WAIT2( 2 ),
	CMD_NEXT,
	WAIT2( 2 ),
	SJUMP( DIR_BACK ),
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT2( 2 ),
	CMD_RESET,
};

const Uint8 taiki0e_cmd_13[] = {
	WAIT2( 2 ),
	STAND,
	CMD_NEXT,
	WAIT2( 2 ),
	STAND,
	WALK_F( 48 ),
	WAIT2( 2 ),
	CMD_NEXT,
	WAIT2( 2 ),
	STAND,
	WALK_B( 48 ),
	WAIT2( 2 ),
	CMD_NEXT,
	WAIT2( 2 ),
	STAND,
	WALK_DIST( 144 ),
	WAIT2( 2 ),
	CMD_NEXT,
	WAIT2( 2 ),
	CMD_RESET,
};

const Uint8 taiki0e_cmd_14[] = {
	WAIT2( 2 ),
	STAND,
	CMD_NEXT,
	WAIT2( 2 ),
	STAND,
	WALK_DIST( 128 ),
	WAIT2( 2 ),
	CMD_NEXT,
	WAIT2( 2 ),
	STAND,
	DASH_F( 256 ),
	WAIT( 16 ),
	CMD_NEXT,
	WAIT2( 2 ),
	STAND,
	JUMP( DIR_FORWARD ),
	WAIT2( 2 ),
	CMD_NEXT,
	WAIT2( 2 ),
	CMD_RESET,
};

const Uint8 taiki0e_cmd_15[] = {
	WAIT2( 2 ),
	STAND,
	CMD_NEXT,
	WAIT2( 2 ),
	JUMP( DIR_FORWARD ),
	WAIT2( 2 ),
	JUMP( DIR_BACK ),
	WAIT2( 2 ),
	CMD_NEXT,
	WAIT2( 2 ),
	DASH_F( 256 ),
	WAIT( 16 ),
	CMD_NEXT,
	WAIT2( 2 ),
	WALK_B( 96 ),
	WAIT2( 2 ),
	WALK_DIST( 64 ),
	WAIT2( 2 ),
	CMD_NEXT,
	WAIT2( 2 ),
	CMD_RESET,
};

/* Anakaris (character 04): the computer's waiting lists.
   What it does while nothing calls for more. PLAYER_WORK's cpu_adrs[0] points here, and em_cmd_sel
   picks a list from it.

   taiki04_tbl     one record (CPU_DATA): the offsets of the three tables.
   taiki04_rate    the chance, out of 32, that a list's CMD_NEXT goes over to an attack list
                   (atck04.c), by the level of play.
   taiki04_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   taiki04_cmd     the lists: a pick is a number in this table.
   taiki04_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                   list of lib04.c. */
#include "player.h"
#include "cpu_cmd.h"

extern const Uint8 *const taiki04_sel[], *const taiki04_cmd[];
extern const Uint8 taiki04_rate[], taiki04_sel_0[], taiki04_sel_1[], taiki04_sel_2[],
	taiki04_sel_3[], taiki04_sel_4[], taiki04_sel_5[], taiki04_sel_6[], taiki04_sel_7[],
	taiki04_sel_8[], taiki04_cmd_00[], taiki04_cmd_01[], taiki04_cmd_02[], taiki04_cmd_03[],
	taiki04_cmd_04[], taiki04_cmd_05[], taiki04_cmd_06[], taiki04_cmd_07[], taiki04_cmd_08[],
	taiki04_cmd_09[], taiki04_cmd_10[], taiki04_cmd_11[], taiki04_cmd_12[], taiki04_cmd_13[],
	taiki04_cmd_14[], taiki04_cmd_15[], taiki04_cmd_16[], taiki04_cmd_17[];

const CPU_DATA taiki04_tbl[] = {
	{ ( Sint32 )taiki04_rate, ( Sint32 )taiki04_sel, ( Sint32 )taiki04_cmd },
};

const Uint8 taiki04_rate[] = {
	 13,  14,  15,  16,  16,  17,  17,  18,  18,  19,  20,  20,  22,  23,  24,  25,
	 26,  27,  28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 *const taiki04_sel[] = {
	taiki04_sel_0, taiki04_sel_1, taiki04_sel_2, taiki04_sel_3, taiki04_sel_4, taiki04_sel_5,
	taiki04_sel_6, taiki04_sel_7, taiki04_sel_8,
};

const Uint8 taiki04_sel_0[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
	  3,   3,   3,   3,   3,   3,   3,  10,   3,   3,   3,   3,   3,   3,   3,  10,
};

const Uint8 taiki04_sel_1[] = {
	  2,   2,   2,   2,   2,   2,   2,  10,   2,   2,   2,   2,   2,   2,   2,  10,
	  3,   6,   6,   6,   6,   6,   6,  10,   3,   6,   6,   6,   6,   6,   6,  10,
};

const Uint8 taiki04_sel_2[] = {
	  2,   2,   2,   2,   2,   2,   2,  10,   2,   2,   2,   2,   2,   2,   2,  10,
	  6,   6,   6,   6,   6,   6,  10,  10,   6,   6,   6,   6,   6,   6,  10,  10,
};

const Uint8 taiki04_sel_3[] = {
	  2,   2,   2,   2,   2,   2,  10,  10,   2,   2,   2,   2,   2,   2,  10,  10,
	  6,   6,   6,   6,   6,   6,  10,  10,   5,   5,   5,   5,   5,   5,  10,  10,
};

const Uint8 taiki04_sel_4[] = {
	  2,   2,   2,   2,   2,   2,  10,  10,   2,   2,   2,   2,   2,   2,  10,  10,
	  5,   5,   5,   5,   5,   5,  10,  10,  11,  11,  11,  11,  11,  11,  11,  11,
};

const Uint8 taiki04_sel_5[] = {
	  6,   6,   6,   6,   6,  10,  10,  10,   6,   6,   6,   6,   6,  10,  10,  10,
	 11,  11,  11,  11,  11,  10,  10,  10,  11,  11,  11,  11,  11,  10,  10,  10,
};

const Uint8 taiki04_sel_6[] = {
	 10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,
	 11,  11,  11,  11,  11,  11,  11,  11,   8,   8,   8,   8,   8,   8,   8,   8,
};

const Uint8 taiki04_sel_7[] = {
	 11,  11,  11,  11,  11,  11,  11,  11,  10,  10,  10,  10,  10,  10,  10,  10,
	 10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,
};

const Uint8 taiki04_sel_8[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,
};

const Uint8 *const taiki04_cmd[] = {
	taiki04_cmd_00, taiki04_cmd_01, taiki04_cmd_02, taiki04_cmd_03, taiki04_cmd_04, taiki04_cmd_05,
	taiki04_cmd_06, taiki04_cmd_07, taiki04_cmd_08, taiki04_cmd_09, taiki04_cmd_10, taiki04_cmd_11,
	taiki04_cmd_12, taiki04_cmd_13, taiki04_cmd_14, taiki04_cmd_15, taiki04_cmd_16, taiki04_cmd_17,
};

const Uint8 taiki04_cmd_00[] = {
	WAIT( 30 ),
	CMD_RESET,
};

const Uint8 taiki04_cmd_01[] = {
	WALK_B( 8 ),
	WAIT2( 14 ),
	CMD_RESET,
};

const Uint8 taiki04_cmd_02[] = {
	CMD_NEXT,
	STAND,
	WALK_DIST( 96 ),
	STAND,
	WAIT2( 14 ),
	CMD_NEXT,
	STAND,
	CMD_RESET,
};

const Uint8 taiki04_cmd_03[] = {
	STAND,
	WAIT( 60 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_RESET,
};

const Uint8 taiki04_cmd_04[] = {
	CMD_NEXT,
	STAND,
	WAIT2( 8 ),
	JUMP( DIR_BACK ),
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT( 20 ),
	WALK_DIST( 64 ),
	CMD_NEXT,
	STAND,
	WAIT( 20 ),
	JUMP( DIR_FORWARD ),
	STAND,
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT2( 8 ),
	STAND,
	CMD_RESET,
};

const Uint8 taiki04_cmd_05[] = {
	CMD_NEXT,
	STAND,
	WAIT2( 14 ),
	CMD_NEXT,
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

const Uint8 taiki04_cmd_06[] = {
	STAND,
	CMD_NEXT,
	WAIT( 10 ),
	CMD_NEXT,
	WALK_DIST( 64 ),
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT2( 8 ),
	WALK_DIST( 64 ),
	WAIT2( 8 ),
	CMD_NEXT,
	STAND,
	WALK_DIST( 64 ),
	WAIT( 10 ),
	CMD_NEXT,
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	STAND,
	CMD_RESET,
};

const Uint8 taiki04_cmd_07[] = {
	CMD_RESET,
};

const Uint8 taiki04_cmd_08[] = {
	CMD_NEXT,
	STAND,
	DASH_F( 64 ),
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_NEXT,
	WALK_DIST( 128 ),
	STAND,
	WAIT2( 8 ),
	DASH_EM( 96 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_NEXT,
	WALK_DIST( 64 ),
	STAND,
	CMD_NEXT,
	STAND,
	WAIT2( 8 ),
	JUMP( DIR_BACK ),
	STAND,
	CMD_NEXT,
	STAND,
	CMD_RESET,
};

const Uint8 taiki04_cmd_09[] = {
	CMD_RESET,
};

const Uint8 taiki04_cmd_10[] = {
	CMD_NEXT,
	STAND,
	DASH_EM( 64 ),
	CMD_NEXT,
	STAND,
	WAIT2( 4 ),
	DASH_EM( 64 ),
	CMD_NEXT,
	STAND,
	WAIT2( 4 ),
	DASH_EM( 64 ),
	CMD_NEXT,
	STAND,
	WAIT2( 4 ),
	CMD_RESET,
};

const Uint8 taiki04_cmd_11[] = {
	CMD_NEXT,
	STAND,
	IF_DIST( 64, 240, 32, 128 ),
		WALK_F( 64 ),
	CMD_ELSE,
		IF_DIST( 64, 240, 160, 128 ),
			WALK_F( 64 ),
		CMD_ELSE,
			WAIT2( 4 ),
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	CMD_NEXT,
	STAND,
	CMD_RESET,
};

const Uint8 taiki04_cmd_12[] = {
	CMD_RESET,
};

const Uint8 taiki04_cmd_13[] = {
	CMD_RESET,
};

const Uint8 taiki04_cmd_14[] = {
	CMD_RESET,
};

const Uint8 taiki04_cmd_15[] = {
	CMD_RESET,
};

const Uint8 taiki04_cmd_16[] = {
	CMD_RESET,
};

const Uint8 taiki04_cmd_17[] = {
	CMD_RESET,
};

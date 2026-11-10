/* Colossus (character 32): the computer's waiting lists.
   What it does while nothing calls for more. PLAYER_WORK's cpu_adrs[0] points here, and em_cmd_sel
   picks a list from it.

   taiki32_tbl     one record (CPU_DATA): the offsets of the three tables.
   taiki32_rate    the chance, out of 32, that a list's CMD_NEXT goes over to an attack list
                   (atck32.c), by the level of play.
   taiki32_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   taiki32_cmd     the lists: a pick is a number in this table.
   taiki32_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                   list of lib32.c. */
#include "player.h"
#include "cpu_cmd.h"

extern const Uint8 *const taiki32_sel[], *const taiki32_cmd[];
extern const Uint8 taiki32_rate[], taiki32_sel_0[], taiki32_sel_1[], taiki32_sel_2[],
	taiki32_sel_3[], taiki32_sel_4[], taiki32_sel_5[], taiki32_sel_6[], taiki32_sel_7[],
	taiki32_sel_8[], taiki32_cmd_00[], taiki32_cmd_01[], taiki32_cmd_02[], taiki32_cmd_03[],
	taiki32_cmd_04[], taiki32_cmd_05[], taiki32_cmd_06[], taiki32_cmd_07[], taiki32_cmd_08[],
	taiki32_cmd_09[], taiki32_cmd_10[], taiki32_cmd_11[], taiki32_cmd_12[], taiki32_cmd_13[],
	taiki32_cmd_14[], taiki32_cmd_15[], taiki32_cmd_16[], taiki32_cmd_17[];

const CPU_DATA taiki32_tbl[] = {
	{ ( Sint32 )taiki32_rate, ( Sint32 )taiki32_sel, ( Sint32 )taiki32_cmd },
};

const Uint8 taiki32_rate[] = {
	 13,  14,  15,  16,  16,  17,  17,  18,  18,  19,  20,  20,  22,  23,  24,  25,
	 26,  27,  28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 *const taiki32_sel[] = {
	taiki32_sel_0, taiki32_sel_1, taiki32_sel_2, taiki32_sel_3, taiki32_sel_4, taiki32_sel_5,
	taiki32_sel_6, taiki32_sel_7, taiki32_sel_8,
};

const Uint8 taiki32_sel_0[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
	  3,   3,   1,   3,   1,   3,   1,   3,   3,   3,   1,   3,   1,   3,   1,   3,
};

const Uint8 taiki32_sel_1[] = {
	  3,   2,   2,   2,   2,   2,   2,   2,   3,   2,   2,   2,   2,   2,   2,   2,
	  1,   1,   6,   6,   6,   6,   6,   6,   1,   1,   6,   6,   6,   6,   6,   6,
};

const Uint8 taiki32_sel_2[] = {
	  2,   2,   2,   2,   2,   2,   1,   2,   2,   2,   2,   2,   2,   2,   1,   2,
	  6,   1,   6,   6,   6,   6,   6,   6,   6,   1,   6,   6,   6,   6,   6,   6,
};

const Uint8 taiki32_sel_3[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,   6,   6,   6,   6,   6,   6,   6,   6,
	  6,   6,   6,   1,   6,   6,   6,   6,   5,   5,   5,   1,   5,   5,   5,   5,
};

const Uint8 taiki32_sel_4[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,  11,  11,  11,  11,  11,  11,  11,  11,
	  5,   1,   5,   5,   5,   5,   5,   5,   5,   1,   5,   5,  10,   5,   5,   5,
};

const Uint8 taiki32_sel_5[] = {
	  6,   6,   1,  10,   1,   6,   6,   6,   6,   6,   1,   6,   1,   6,   6,   6,
	 11,  11,  11,   8,  10,  11,  11,  11,  11,  11,  11,   8,  11,  11,  11,  11,
};

const Uint8 taiki32_sel_6[] = {
	 10,  10,  10,  10,  10,  10,  10,  10,  11,  11,  11,  11,  10,  11,  11,  11,
	 11,  11,  11,  11,  10,  11,  11,  11,   8,   8,   8,   8,  10,   8,   8,   8,
};

const Uint8 taiki32_sel_7[] = {
	 11,  11,   1,  11,  10,  11,  11,  11,  11,  11,  10,  11,  10,  11,  11,  11,
	  8,   8,  10,  10,  10,  10,  10,  10,   8,   8,  10,  10,  10,  10,  10,  10,
};

const Uint8 taiki32_sel_8[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,
};

const Uint8 *const taiki32_cmd[] = {
	taiki32_cmd_00, taiki32_cmd_01, taiki32_cmd_02, taiki32_cmd_03, taiki32_cmd_04, taiki32_cmd_05,
	taiki32_cmd_06, taiki32_cmd_07, taiki32_cmd_08, taiki32_cmd_09, taiki32_cmd_10, taiki32_cmd_11,
	taiki32_cmd_12, taiki32_cmd_13, taiki32_cmd_14, taiki32_cmd_15, taiki32_cmd_16, taiki32_cmd_17,
};

const Uint8 taiki32_cmd_00[] = {
	WAIT( 30 ),
	CMD_RESET,
};

const Uint8 taiki32_cmd_01[] = {
	WALK_F( 8 ),
	WAIT2( 14 ),
	CMD_RESET,
};

const Uint8 taiki32_cmd_02[] = {
	STAND,
	IF_DIST( 64, 240, 32, 128 ),
		WALK_B( 64 ),
	CMD_ELSE,
		IF_DIST( 64, 240, 160, 128 ),
			WALK_F( 64 ),
		CMD_ELSE,
			WAIT( 20 ),
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	WAIT2( 14 ),
	CMD_NEXT,
	WAIT2( 2 ),
	CMD_NEXT,
	STAND,
	CMD_RESET,
};

const Uint8 taiki32_cmd_03[] = {
	STAND,
	WAIT( 60 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_RESET,
};

const Uint8 taiki32_cmd_04[] = {
	STAND,
	WAIT2( 8 ),
	JUMP( DIR_BACK ),
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT( 20 ),
	DASH_B( 256 ),
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
	JUMP( DIR_FORWARD ),
	CMD_NEXT,
	STAND,
	WAIT2( 8 ),
	DASH_F( 96 ),
	CMD_NEXT,
	WAIT2( 8 ),
	STAND,
	CMD_RESET,
};

const Uint8 taiki32_cmd_05[] = {
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

const Uint8 taiki32_cmd_06[] = {
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
	WAIT2( 4 ),
	CMD_NEXT,
	STAND,
	CMD_RESET,
};

const Uint8 taiki32_cmd_07[] = {
	CMD_RESET,
};

const Uint8 taiki32_cmd_08[] = {
	STAND,
	DASH_B( 64 ),
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_NEXT,
	WALK_EM( 128 ),
	CMD_NEXT,
	STAND,
	WAIT2( 8 ),
	DASH_EM( 96 ),
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
	STAND,
	CMD_NEXT,
	STAND,
	CMD_RESET,
};

const Uint8 taiki32_cmd_09[] = {
	CMD_RESET,
};

const Uint8 taiki32_cmd_10[] = {
	STAND,
	CMD_NEXT,
	STAND,
	WAIT2( 4 ),
	CMD_NEXT,
	STAND,
	WAIT2( 4 ),
	CMD_NEXT,
	STAND,
	WAIT2( 4 ),
	CMD_RESET,
};

const Uint8 taiki32_cmd_11[] = {
	STAND,
	IF_DIST( 64, 240, 32, 128 ),
		WALK_B( 64 ),
	CMD_ELSE,
		IF_DIST( 64, 240, 160, 128 ),
			WALK_F( 32 ),
		CMD_ELSE,
			WAIT2( 4 ),
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	CMD_NEXT,
	STAND,
	CMD_RESET,
};

const Uint8 taiki32_cmd_12[] = {
	CMD_RESET,
};

const Uint8 taiki32_cmd_13[] = {
	CMD_RESET,
};

const Uint8 taiki32_cmd_14[] = {
	CMD_RESET,
};

const Uint8 taiki32_cmd_15[] = {
	CMD_RESET,
};

const Uint8 taiki32_cmd_16[] = {
	CMD_RESET,
};

const Uint8 taiki32_cmd_17[] = {
	CMD_RESET,
};

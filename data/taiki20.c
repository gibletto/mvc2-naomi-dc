/* Felicia (character 20): the computer's waiting lists.
   What it does while nothing calls for more. PLAYER_WORK's cpu_adrs[0] points here, and em_cmd_sel
   picks a list from it.

   taiki20_tbl     one record (CPU_DATA): the offsets of the three tables.
   taiki20_rate    the chance, out of 32, that a list's CMD_NEXT goes over to an attack list
                   (atck20.c), by the level of play.
   taiki20_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   taiki20_cmd     the lists: a pick is a number in this table.
   taiki20_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                   list of lib20.c. */
#include "player.h"
#include "cpu_cmd.h"

extern const Uint8 *const taiki20_sel[], *const taiki20_cmd[];
extern const Uint8 taiki20_rate[], taiki20_sel_0[], taiki20_sel_1[], taiki20_sel_2[],
	taiki20_sel_3[], taiki20_sel_4[], taiki20_sel_5[], taiki20_sel_6[], taiki20_sel_7[],
	taiki20_sel_8[], taiki20_cmd_00[], taiki20_cmd_01[], taiki20_cmd_02[], taiki20_cmd_03[],
	taiki20_cmd_04[], taiki20_cmd_05[], taiki20_cmd_06[], taiki20_cmd_07[], taiki20_cmd_08[],
	taiki20_cmd_09[], taiki20_cmd_10[], taiki20_cmd_11[], taiki20_cmd_12[], taiki20_cmd_13[],
	taiki20_cmd_14[], taiki20_cmd_15[], taiki20_cmd_16[], taiki20_cmd_17[];

const CPU_DATA taiki20_tbl[] = {
	{ ( Sint32 )taiki20_rate, ( Sint32 )taiki20_sel, ( Sint32 )taiki20_cmd },
};

const Uint8 taiki20_rate[] = {
	 13,  14,  15,  16,  16,  17,  17,  18,  18,  19,  20,  20,  22,  23,  24,  25,
	 26,  27,  28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 *const taiki20_sel[] = {
	taiki20_sel_0, taiki20_sel_1, taiki20_sel_2, taiki20_sel_3, taiki20_sel_4, taiki20_sel_5,
	taiki20_sel_6, taiki20_sel_7, taiki20_sel_8,
};

const Uint8 taiki20_sel_0[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
	  3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,
};

const Uint8 taiki20_sel_1[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
	  3,   6,   6,   6,   6,   6,   6,   6,   3,   6,   6,   6,   6,   6,   6,   6,
};

const Uint8 taiki20_sel_2[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
	  6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
};

const Uint8 taiki20_sel_3[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
	  6,   6,   6,   6,   6,   6,   6,   6,   5,   5,   5,   5,   5,   5,   5,   5,
};

const Uint8 taiki20_sel_4[] = {
	  2,   2,   2,   2,   2,   2,   2,  10,   2,   2,   2,   2,   2,   2,   2,  10,
	  5,   5,   5,   5,   5,   5,   5,  10,  11,  11,  11,  11,  11,  11,  11,  11,
};

const Uint8 taiki20_sel_5[] = {
	  6,   6,   6,   6,   6,   6,  10,  10,   6,   6,   6,   6,   6,   6,  10,  10,
	 11,  11,  11,  11,  11,  11,  10,  10,  11,  11,  11,  11,  11,  11,  10,  10,
};

const Uint8 taiki20_sel_6[] = {
	 10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,
	 11,  11,  11,  11,  11,  11,  11,  11,   8,   8,   8,   8,   8,   8,   8,   8,
};

const Uint8 taiki20_sel_7[] = {
	 11,  11,  11,  11,  11,  11,  11,  11,  10,  10,  10,  10,  10,  10,  10,  10,
	 10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,
};

const Uint8 taiki20_sel_8[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,
};

const Uint8 *const taiki20_cmd[] = {
	taiki20_cmd_00, taiki20_cmd_01, taiki20_cmd_02, taiki20_cmd_03, taiki20_cmd_04, taiki20_cmd_05,
	taiki20_cmd_06, taiki20_cmd_07, taiki20_cmd_08, taiki20_cmd_09, taiki20_cmd_10, taiki20_cmd_11,
	taiki20_cmd_12, taiki20_cmd_13, taiki20_cmd_14, taiki20_cmd_15, taiki20_cmd_16, taiki20_cmd_17,
};

const Uint8 taiki20_cmd_00[] = {
	WAIT( 30 ),
	CMD_RESET,
};

const Uint8 taiki20_cmd_01[] = {
	WALK_F( 8 ),
	WAIT2( 14 ),
	CMD_RESET,
};

const Uint8 taiki20_cmd_02[] = {
	CMD_NEXT,
	STAND,
	WALK_DIST( 64 ),
	STAND,
	WAIT2( 14 ),
	CMD_NEXT,
	STAND,
	CMD_RESET,
};

const Uint8 taiki20_cmd_03[] = {
	STAND,
	WAIT( 40 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_RESET,
};

const Uint8 taiki20_cmd_04[] = {
	CMD_NEXT,
	STAND,
	WAIT2( 8 ),
	JUMP( DIR_FORWARD ),
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

const Uint8 taiki20_cmd_05[] = {
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

const Uint8 taiki20_cmd_06[] = {
	STAND,
	CMD_NEXT,
	WAIT( 10 ),
	CMD_NEXT,
	WALK_DIST( 48 ),
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT2( 8 ),
	WALK_DIST( 48 ),
	WAIT2( 8 ),
	CMD_NEXT,
	STAND,
	WALK_DIST( 48 ),
	WAIT( 10 ),
	CMD_NEXT,
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	STAND,
	CMD_RESET,
};

const Uint8 taiki20_cmd_07[] = {
	CMD_RESET,
};

const Uint8 taiki20_cmd_08[] = {
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

const Uint8 taiki20_cmd_09[] = {
	CMD_RESET,
};

const Uint8 taiki20_cmd_10[] = {
	CMD_NEXT,
	WAIT( 30 ),
	STAND,
	DASH_EM( 64 ),
	CMD_NEXT,
	STAND,
	WAIT2( 4 ),
	DASH_EM( 48 ),
	CMD_NEXT,
	STAND,
	WAIT2( 4 ),
	DASH_EM( 96 ),
	CMD_NEXT,
	STAND,
	WAIT( 30 ),
	WAIT2( 4 ),
	CMD_RESET,
};

const Uint8 taiki20_cmd_11[] = {
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

const Uint8 taiki20_cmd_12[] = {
	CMD_RESET,
};

const Uint8 taiki20_cmd_13[] = {
	CMD_RESET,
};

const Uint8 taiki20_cmd_14[] = {
	CMD_RESET,
};

const Uint8 taiki20_cmd_15[] = {
	CMD_RESET,
};

const Uint8 taiki20_cmd_16[] = {
	CMD_RESET,
};

const Uint8 taiki20_cmd_17[] = {
	CMD_RESET,
};

/* Marrow (character 16): the computer's waiting lists.
   What it does while nothing calls for more. PLAYER_WORK's cpu_adrs[0] points here, and em_cmd_sel
   picks a list from it.

   taiki16_tbl     one record (CPU_DATA): the offsets of the three tables.
   taiki16_rate    the chance, out of 32, that a list's CMD_NEXT goes over to an attack list
                   (atck16.c), by the level of play.
   taiki16_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   taiki16_cmd     the lists: a pick is a number in this table.
   taiki16_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                   list of lib16.c. */
#include "player.h"
#include "cpu_cmd.h"

extern const Uint8 *const taiki16_sel[], *const taiki16_cmd[];
extern const Uint8 taiki16_rate[], taiki16_sel_0[], taiki16_sel_1[], taiki16_sel_2[],
	taiki16_sel_3[], taiki16_sel_4[], taiki16_sel_5[], taiki16_sel_6[], taiki16_sel_7[],
	taiki16_sel_8[], taiki16_cmd_00[], taiki16_cmd_01[], taiki16_cmd_02[], taiki16_cmd_03[],
	taiki16_cmd_04[], taiki16_cmd_05[], taiki16_cmd_06[], taiki16_cmd_07[], taiki16_cmd_08[],
	taiki16_cmd_09[], taiki16_cmd_10[], taiki16_cmd_11[], taiki16_cmd_12[], taiki16_cmd_13[],
	taiki16_cmd_14[], taiki16_cmd_15[], taiki16_cmd_16[], taiki16_cmd_17[];

const CPU_DATA taiki16_tbl[] = {
	{ ( Sint32 )taiki16_rate, ( Sint32 )taiki16_sel, ( Sint32 )taiki16_cmd },
};

const Uint8 taiki16_rate[] = {
	 13,  14,  15,  16,  16,  17,  17,  18,  18,  19,  20,  20,  22,  23,  24,  25,
	 26,  27,  28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 *const taiki16_sel[] = {
	taiki16_sel_0, taiki16_sel_1, taiki16_sel_2, taiki16_sel_3, taiki16_sel_4, taiki16_sel_5,
	taiki16_sel_6, taiki16_sel_7, taiki16_sel_8,
};

const Uint8 taiki16_sel_0[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
	  3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,
};

const Uint8 taiki16_sel_1[] = {
	  3,   2,   2,   2,   2,   2,   2,   2,   3,   2,   2,   2,   2,   2,   2,   2,
	  3,   6,   6,   6,   6,   6,   6,   6,   3,   6,   6,   6,   6,   6,   6,   6,
};

const Uint8 taiki16_sel_2[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
	  6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
};

const Uint8 taiki16_sel_3[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,   6,   6,   6,   6,   6,   6,   6,   6,
	  6,   6,   6,   6,   6,   6,   6,   6,   5,   5,   5,   5,   5,   5,   5,   5,
};

const Uint8 taiki16_sel_4[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,  11,  11,  11,  11,  11,  11,  11,  11,
	  5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,
};

const Uint8 taiki16_sel_5[] = {
	  6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
	 11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,
};

const Uint8 taiki16_sel_6[] = {
	 10,  10,  10,  10,  10,  10,  12,  12,  11,  11,  11,  11,  11,  11,  12,  12,
	 11,  11,  11,  11,  11,  11,  12,  12,   8,   8,   8,   8,   8,   8,  12,  12,
};

const Uint8 taiki16_sel_7[] = {
	 11,  11,  11,  11,  11,  11,  12,  12,  11,  11,  11,  11,  11,  11,  12,  12,
	  8,   8,  10,  10,  10,  10,  12,  12,   8,   8,  10,  10,  10,  10,  12,  12,
};

const Uint8 taiki16_sel_8[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,
};

const Uint8 *const taiki16_cmd[] = {
	taiki16_cmd_00, taiki16_cmd_01, taiki16_cmd_02, taiki16_cmd_03, taiki16_cmd_04, taiki16_cmd_05,
	taiki16_cmd_06, taiki16_cmd_07, taiki16_cmd_08, taiki16_cmd_09, taiki16_cmd_10, taiki16_cmd_11,
	taiki16_cmd_12, taiki16_cmd_13, taiki16_cmd_14, taiki16_cmd_15, taiki16_cmd_16, taiki16_cmd_17,
};

const Uint8 taiki16_cmd_00[] = {
	WAIT( 30 ),
	CMD_RESET,
};

const Uint8 taiki16_cmd_01[] = {
	WALK_B( 8 ),
	WAIT2( 14 ),
	CMD_RESET,
};

const Uint8 taiki16_cmd_02[] = {
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
	STAND,
	CMD_RESET,
};

const Uint8 taiki16_cmd_03[] = {
	STAND,
	WAIT( 60 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_RESET,
};

const Uint8 taiki16_cmd_04[] = {
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

const Uint8 taiki16_cmd_05[] = {
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

const Uint8 taiki16_cmd_06[] = {
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
	CMD_RESET,
};

const Uint8 taiki16_cmd_07[] = {
	CMD_RESET,
};

const Uint8 taiki16_cmd_08[] = {
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
	JUMP( DIR_BACK ),
	STAND,
	CMD_NEXT,
	STAND,
	CMD_RESET,
};

const Uint8 taiki16_cmd_09[] = {
	CMD_RESET,
};

const Uint8 taiki16_cmd_10[] = {
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

const Uint8 taiki16_cmd_11[] = {
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

const Uint8 taiki16_cmd_12[] = {
	STAND,
	DASH_F( 64 ),
	CMD_NEXT,
	STAND,
	WAIT2( 8 ),
	DASH_B( 64 ),
	CMD_NEXT,
	STAND,
	WAIT2( 8 ),
	DASH_F( 160 ),
	CMD_NEXT,
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki16_cmd_13[] = {
	CMD_RESET,
};

const Uint8 taiki16_cmd_14[] = {
	CMD_RESET,
};

const Uint8 taiki16_cmd_15[] = {
	CMD_RESET,
};

const Uint8 taiki16_cmd_16[] = {
	CMD_RESET,
};

const Uint8 taiki16_cmd_17[] = {
	CMD_RESET,
};

/* Hayato (character 12): the computer's waiting lists.
   What it does while nothing calls for more. PLAYER_WORK's cpu_adrs[0] points here, and em_cmd_sel
   picks a list from it.

   taiki12_tbl     one record (CPU_DATA): the offsets of the three tables.
   taiki12_rate    the chance, out of 32, that a list's CMD_NEXT goes over to an attack list
                   (atck12.c), by the level of play.
   taiki12_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   taiki12_cmd     the lists: a pick is a number in this table.
   taiki12_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                   list of lib12.c. */
#include "player.h"
#include "cpu_cmd.h"

extern const Uint8 *const taiki12_sel[], *const taiki12_cmd[];
extern const Uint8 taiki12_rate[], taiki12_sel_0[], taiki12_sel_1[], taiki12_sel_2[],
	taiki12_sel_3[], taiki12_sel_4[], taiki12_sel_5[], taiki12_sel_6[], taiki12_sel_7[],
	taiki12_sel_8[], taiki12_cmd_00[], taiki12_cmd_01[], taiki12_cmd_02[], taiki12_cmd_03[],
	taiki12_cmd_04[], taiki12_cmd_05[], taiki12_cmd_06[], taiki12_cmd_07[], taiki12_cmd_08[],
	taiki12_cmd_09[], taiki12_cmd_10[], taiki12_cmd_11[], taiki12_cmd_12[], taiki12_cmd_13[],
	taiki12_cmd_14[], taiki12_cmd_15[], taiki12_cmd_16[], taiki12_cmd_17[];

const CPU_DATA taiki12_tbl[] = {
	{ ( Sint32 )taiki12_rate, ( Sint32 )taiki12_sel, ( Sint32 )taiki12_cmd },
};

const Uint8 taiki12_rate[] = {
	 13,  14,  15,  16,  16,  17,  17,  18,  18,  19,  20,  20,  22,  23,  24,  25,
	 26,  27,  28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 *const taiki12_sel[] = {
	taiki12_sel_0, taiki12_sel_1, taiki12_sel_2, taiki12_sel_3, taiki12_sel_4, taiki12_sel_5,
	taiki12_sel_6, taiki12_sel_7, taiki12_sel_8,
};

const Uint8 taiki12_sel_0[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
	  3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,
};

const Uint8 taiki12_sel_1[] = {
	  3,   2,   2,   2,   2,   2,   2,   2,   3,   2,   2,   2,   2,   2,   2,   2,
	  3,   6,   6,   6,   6,   6,   6,   6,   3,   6,   6,   6,   6,   6,   6,   6,
};

const Uint8 taiki12_sel_2[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
	  6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
};

const Uint8 taiki12_sel_3[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,   6,   6,   6,   6,   6,   6,   6,   6,
	  6,   6,   6,   6,   6,   6,   6,   6,   5,   5,   5,   5,   5,   5,   5,   5,
};

const Uint8 taiki12_sel_4[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,  11,  11,  11,  11,  11,  11,  11,  11,
	  5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,
};

const Uint8 taiki12_sel_5[] = {
	  6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
	 11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,
};

const Uint8 taiki12_sel_6[] = {
	 10,  10,  10,  10,  10,  10,  10,  10,  11,  11,  11,  11,  11,  11,  11,  11,
	 11,  11,  11,  11,  11,  11,  11,  11,   8,   8,   8,   8,   8,   8,   8,   8,
};

const Uint8 taiki12_sel_7[] = {
	 11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,
	  8,   8,  10,  10,  10,  10,  10,  10,   8,   8,  10,  10,  10,  10,  10,  10,
};

const Uint8 taiki12_sel_8[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
};

const Uint8 *const taiki12_cmd[] = {
	taiki12_cmd_00, taiki12_cmd_01, taiki12_cmd_02, taiki12_cmd_03, taiki12_cmd_04, taiki12_cmd_05,
	taiki12_cmd_06, taiki12_cmd_07, taiki12_cmd_08, taiki12_cmd_09, taiki12_cmd_10, taiki12_cmd_11,
	taiki12_cmd_12, taiki12_cmd_13, taiki12_cmd_14, taiki12_cmd_15, taiki12_cmd_16, taiki12_cmd_17,
};

const Uint8 taiki12_cmd_00[] = {
	STAND,
	WAIT( 30 ),
	CMD_RESET,
};

const Uint8 taiki12_cmd_01[] = {
	WALK_F( 16 ),
	WAIT2( 14 ),
	CMD_RESET,
};

const Uint8 taiki12_cmd_02[] = {
	STAND,
	WALK_DIST( 256 ),
	CMD_NEXT,
	STAND,
	CMD_NEXT,
	WAIT( 20 ),
	CMD_NEXT,
	STAND,
	CMD_RESET,
};

const Uint8 taiki12_cmd_03[] = {
	STAND,
	WAIT( 60 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_RESET,
};

const Uint8 taiki12_cmd_04[] = {
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

const Uint8 taiki12_cmd_05[] = {
	STAND,
	IF_DIST( 40, 0, 40, 0 ),
		DASH_EM( 192 ),
		STAND,
		WAIT( 30 ),
		CMD_NEXT,
		WAIT2( 14 ),
	CMD_ELSE,
		IF_DIST( 72, 0, 72, 0 ),
			WALK_DIST( 208 ),
			STAND,
			WAIT( 30 ),
			CMD_NEXT,
			WAIT2( 14 ),
		CMD_ELSE,
			WALK_DIST( 64 ),
			STAND,
			WAIT( 30 ),
			CMD_NEXT,
			WAIT2( 14 ),
		CMD_ENDIF,
		WAIT2( 14 ),
		CMD_NEXT,
	CMD_ENDIF,
	WAIT2( 14 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki12_cmd_06[] = {
	STAND,
	IF_DIST( 128, 0, 128, 0 ),
		WALK_DIST( 128 ),
	CMD_ELSE,
		DASH_EM( 96 ),
		STAND,
		CMD_NEXT,
		WAIT( 10 ),
		CMD_NEXT,
	CMD_ENDIF,
	CMD_NEXT,
	STAND,
	CMD_RESET,
};

const Uint8 taiki12_cmd_07[] = {
	CMD_RESET,
};

const Uint8 taiki12_cmd_08[] = {
	STAND,
	DASH_EM( 256 ),
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
	JUMP( DIR_FORWARD ),
	STAND,
	CMD_NEXT,
	STAND,
	CMD_RESET,
};

const Uint8 taiki12_cmd_09[] = {
	CMD_RESET,
};

const Uint8 taiki12_cmd_10[] = {
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

const Uint8 taiki12_cmd_11[] = {
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

const Uint8 taiki12_cmd_12[] = {
	CMD_RESET,
};

const Uint8 taiki12_cmd_13[] = {
	CMD_RESET,
};

const Uint8 taiki12_cmd_14[] = {
	CMD_RESET,
};

const Uint8 taiki12_cmd_15[] = {
	CMD_RESET,
};

const Uint8 taiki12_cmd_16[] = {
	CMD_RESET,
};

const Uint8 taiki12_cmd_17[] = {
	CMD_RESET,
};

/* Abyss (second form) (character 19): the computer's waiting lists.
   What it does while nothing calls for more. PLAYER_WORK's cpu_adrs[0] points here, and em_cmd_sel
   picks a list from it.

   taiki19_tbl     one record (CPU_DATA): the offsets of the three tables.
   taiki19_rate    the chance, out of 32, that a list's CMD_NEXT goes over to an attack list
                   (atck19.c), by the level of play.
   taiki19_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   taiki19_cmd     the lists: a pick is a number in this table.
   taiki19_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                   list of lib19.c. */
#include "player.h"
#include "cpu_cmd.h"

extern const Uint8 *const taiki19_sel[], *const taiki19_cmd[];
extern const Uint8 taiki19_rate[], taiki19_sel_0[], taiki19_sel_1[], taiki19_sel_2[],
	taiki19_sel_3[], taiki19_sel_4[], taiki19_sel_5[], taiki19_sel_6[], taiki19_sel_7[],
	taiki19_sel_8[], taiki19_cmd_00[], taiki19_cmd_01[], taiki19_cmd_02[], taiki19_cmd_03[],
	taiki19_cmd_04[], taiki19_cmd_05[], taiki19_cmd_06[], taiki19_cmd_07[], taiki19_cmd_08[],
	taiki19_cmd_09[], taiki19_cmd_10[], taiki19_cmd_11[], taiki19_cmd_12[], taiki19_cmd_13[],
	taiki19_cmd_14[], taiki19_cmd_15[], taiki19_cmd_16[], taiki19_cmd_17[];

const CPU_DATA taiki19_tbl[] = {
	{ ( Sint32 )taiki19_rate, ( Sint32 )taiki19_sel, ( Sint32 )taiki19_cmd },
};

const Uint8 taiki19_rate[] = {
	 13,  14,  15,  16,  16,  17,  17,  18,  18,  19,  20,  20,  22,  23,  24,  25,
	 26,  27,  28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 *const taiki19_sel[] = {
	taiki19_sel_0, taiki19_sel_1, taiki19_sel_2, taiki19_sel_3, taiki19_sel_4, taiki19_sel_5,
	taiki19_sel_6, taiki19_sel_7, taiki19_sel_8,
};

const Uint8 taiki19_sel_0[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
	  3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,
};

const Uint8 taiki19_sel_1[] = {
	  3,   2,   2,   2,   2,   2,   2,   2,   3,   2,   2,   2,   2,   2,   2,   2,
	  3,   6,   6,   6,   6,   6,   6,   6,   3,   6,   6,   6,   6,   6,   6,   6,
};

const Uint8 taiki19_sel_2[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
	  6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
};

const Uint8 taiki19_sel_3[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,   6,   6,   6,   6,   6,   6,   6,   6,
	  6,   6,   6,   6,   6,   6,   6,   6,   5,   5,   5,   5,   5,   5,   5,   5,
};

const Uint8 taiki19_sel_4[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,  11,  11,  11,  11,  11,  11,  11,  11,
	  5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,
};

const Uint8 taiki19_sel_5[] = {
	  6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
	 11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,
};

const Uint8 taiki19_sel_6[] = {
	 10,  10,  10,  10,  10,  10,  10,  10,  11,  11,  11,  11,  11,  11,  11,  11,
	 11,  11,  11,  11,  11,  11,  11,  11,   8,   8,   8,   8,   8,   8,   8,   8,
};

const Uint8 taiki19_sel_7[] = {
	 11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,
	  8,   8,  10,  10,  10,  10,  10,  10,   8,   8,  10,  10,  10,  10,  10,  10,
};

const Uint8 taiki19_sel_8[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const taiki19_cmd[] = {
	taiki19_cmd_00, taiki19_cmd_01, taiki19_cmd_02, taiki19_cmd_03, taiki19_cmd_04, taiki19_cmd_05,
	taiki19_cmd_06, taiki19_cmd_07, taiki19_cmd_08, taiki19_cmd_09, taiki19_cmd_10, taiki19_cmd_11,
	taiki19_cmd_12, taiki19_cmd_13, taiki19_cmd_14, taiki19_cmd_15, taiki19_cmd_16, taiki19_cmd_17,
};

const Uint8 taiki19_cmd_00[] = {
	WAIT( 30 ),
	CMD_RESET,
};

const Uint8 taiki19_cmd_01[] = {
	CMD_RESET,
};

const Uint8 taiki19_cmd_02[] = {
	STAND,
	WALK_B( 64 ),
	WAIT( 20 ),
	STAND,
	WAIT2( 14 ),
	CMD_NEXT,
	STAND,
	CMD_RESET,
};

const Uint8 taiki19_cmd_03[] = {
	STAND,
	WAIT( 60 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_RESET,
};

const Uint8 taiki19_cmd_04[] = {
	STAND,
	WAIT2( 8 ),
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT( 20 ),
	STAND,
	WAIT( 20 ),
	STAND,
	WAIT2( 4 ),
	CMD_NEXT,
	WAIT2( 8 ),
	STAND,
	WAIT( 10 ),
	CMD_NEXT,
	WAIT2( 8 ),
	WALK_EM( 96 ),
	STAND,
	CMD_NEXT,
	STAND,
	WAIT2( 8 ),
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT2( 8 ),
	STAND,
	CMD_RESET,
};

const Uint8 taiki19_cmd_05[] = {
	CMD_NEXT,
	STAND,
	WAIT2( 14 ),
	CMD_NEXT,
	WALK_B( 16 ),
	STAND,
	WAIT( 10 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_NEXT,
	STAND,
	WAIT( 10 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_NEXT,
	STAND,
	CMD_RESET,
};

const Uint8 taiki19_cmd_06[] = {
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

const Uint8 taiki19_cmd_07[] = {
	CMD_RESET,
};

const Uint8 taiki19_cmd_08[] = {
	STAND,
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

const Uint8 taiki19_cmd_09[] = {
	CMD_RESET,
};

const Uint8 taiki19_cmd_10[] = {
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

const Uint8 taiki19_cmd_11[] = {
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

const Uint8 taiki19_cmd_12[] = {
	CMD_RESET,
};

const Uint8 taiki19_cmd_13[] = {
	CMD_RESET,
};

const Uint8 taiki19_cmd_14[] = {
	CMD_RESET,
};

const Uint8 taiki19_cmd_15[] = {
	CMD_RESET,
};

const Uint8 taiki19_cmd_16[] = {
	CMD_RESET,
};

const Uint8 taiki19_cmd_17[] = {
	CMD_RESET,
};

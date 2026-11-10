/* Shuma Gorath (character 2d): the computer's waiting lists.
   What it does while nothing calls for more. PLAYER_WORK's cpu_adrs[0] points here, and em_cmd_sel
   picks a list from it.

   taiki2d_tbl     one record (CPU_DATA): the offsets of the three tables.
   taiki2d_rate    the chance, out of 32, that a list's CMD_NEXT goes over to an attack list
                   (atck2d.c), by the level of play.
   taiki2d_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   taiki2d_cmd     the lists: a pick is a number in this table.
   taiki2d_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                   list of lib2d.c. */
#include "player.h"
#include "cpu_cmd.h"

extern const Uint8 *const taiki2d_sel[], *const taiki2d_cmd[];
extern const Uint8 taiki2d_rate[], taiki2d_sel_0[], taiki2d_sel_1[], taiki2d_sel_2[],
	taiki2d_sel_3[], taiki2d_sel_4[], taiki2d_sel_5[], taiki2d_sel_6[], taiki2d_sel_7[],
	taiki2d_sel_8[], taiki2d_cmd_00[], taiki2d_cmd_01[], taiki2d_cmd_02[], taiki2d_cmd_03[],
	taiki2d_cmd_04[], taiki2d_cmd_05[], taiki2d_cmd_06[], taiki2d_cmd_07[], taiki2d_cmd_08[],
	taiki2d_cmd_09[], taiki2d_cmd_10[], taiki2d_cmd_11[], taiki2d_cmd_12[], taiki2d_cmd_13[],
	taiki2d_cmd_14[], taiki2d_cmd_15[], taiki2d_cmd_16[], taiki2d_cmd_17[];

const CPU_DATA taiki2d_tbl[] = {
	{ ( Sint32 )taiki2d_rate, ( Sint32 )taiki2d_sel, ( Sint32 )taiki2d_cmd },
};

const Uint8 taiki2d_rate[] = {
	 13,  14,  15,  16,  16,  17,  17,  18,  18,  19,  20,  20,  22,  23,  24,  25,
	 26,  27,  28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 *const taiki2d_sel[] = {
	taiki2d_sel_0, taiki2d_sel_1, taiki2d_sel_2, taiki2d_sel_3, taiki2d_sel_4, taiki2d_sel_5,
	taiki2d_sel_6, taiki2d_sel_7, taiki2d_sel_8,
};

const Uint8 taiki2d_sel_0[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
	  3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,
};

const Uint8 taiki2d_sel_1[] = {
	  3,   2,   2,   2,   2,   2,   2,   2,   3,   2,   2,   2,   2,   2,   2,   2,
	  3,   6,   6,   6,   6,   6,   6,   6,   3,   6,   6,   6,   6,   6,   6,   6,
};

const Uint8 taiki2d_sel_2[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
	  6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
};

const Uint8 taiki2d_sel_3[] = {
	  6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
	  4,   4,   4,   4,   4,   4,   4,   4,   5,   5,   5,   5,   5,   5,   5,   5,
};

const Uint8 taiki2d_sel_4[] = {
	  5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,
	  6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
};

const Uint8 taiki2d_sel_5[] = {
	  6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
	  8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
};

const Uint8 taiki2d_sel_6[] = {
	 10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,
	  8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
};

const Uint8 taiki2d_sel_7[] = {
	  8,  10,  10,  10,  10,  10,  10,  10,   8,  10,  10,  10,  10,  10,  10,  10,
	  8,  10,  10,  10,  10,  10,  10,  10,   8,  10,  10,  10,  10,  10,  10,  10,
};

const Uint8 taiki2d_sel_8[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,
};

const Uint8 *const taiki2d_cmd[] = {
	taiki2d_cmd_00, taiki2d_cmd_01, taiki2d_cmd_02, taiki2d_cmd_03, taiki2d_cmd_04, taiki2d_cmd_05,
	taiki2d_cmd_06, taiki2d_cmd_07, taiki2d_cmd_08, taiki2d_cmd_09, taiki2d_cmd_10, taiki2d_cmd_11,
	taiki2d_cmd_12, taiki2d_cmd_13, taiki2d_cmd_14, taiki2d_cmd_15, taiki2d_cmd_16, taiki2d_cmd_17,
};

const Uint8 taiki2d_cmd_00[] = {
	WAIT( 10 ),
	WALK_B( 16 ),
	CMD_RESET,
};

const Uint8 taiki2d_cmd_01[] = {
	WALK_F( 8 ),
	WAIT2( 14 ),
	WALK_F( 8 ),
	WAIT2( 14 ),
	CMD_RESET,
};

const Uint8 taiki2d_cmd_02[] = {
	STAND,
	WAIT2( 14 ),
	WAIT2( 14 ),
	WALK_EM( 64 ),
	STAND,
	CMD_NEXT,
	WAIT2( 14 ),
	WALK_B( 16 ),
	STAND,
	WAIT2( 14 ),
	STAND,
	WAIT2( 14 ),
	CMD_NEXT,
	STAND,
	CMD_RESET,
};

const Uint8 taiki2d_cmd_03[] = {
	STAND,
	WAIT( 30 ),
	CMD_NEXT,
	WAIT2( 6 ),
	WALK_F( 16 ),
	STAND,
	CMD_NEXT,
	WAIT2( 6 ),
	CMD_RESET,
};

const Uint8 taiki2d_cmd_04[] = {
	STAND,
	IF_RND( 16 ),
		STAND,
		CMD_NEXT,
		WAIT2( 8 ),
		IF_DIST( 128, 240, 0, 0 ),
		CMD_ELSE,
			WALK_F( 32 ),
		CMD_ENDIF,
		CMD_NEXT,
		WAIT2( 8 ),
	CMD_ELSE,
		STAND,
		CMD_NEXT,
		WAIT2( 8 ),
		IF_DIST( 144, 240, 0, 0 ),
		CMD_ELSE,
			WALK_F( 32 ),
		CMD_ENDIF,
		CMD_NEXT,
		WAIT2( 8 ),
	CMD_ENDIF,
	IF_RND( 16 ),
		STAND,
		CMD_NEXT,
		WAIT2( 8 ),
		IF_DIST( 112, 240, 0, 0 ),
		CMD_ELSE,
			WALK_F( 32 ),
		CMD_ENDIF,
		CMD_NEXT,
		WAIT2( 8 ),
	CMD_ELSE,
		STAND,
		CMD_NEXT,
		WAIT2( 8 ),
		IF_DIST( 160, 240, 0, 0 ),
		CMD_ELSE,
			WALK_F( 32 ),
		CMD_ENDIF,
		CMD_NEXT,
		WAIT2( 8 ),
	CMD_ENDIF,
	CMD_RESET,
};

const Uint8 taiki2d_cmd_05[] = {
	CMD_NEXT,
	STAND,
	WAIT2( 4 ),
	CMD_NEXT,
	JUMP( DIR_BACK ),
	STAND,
	WAIT2( 6 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki2d_cmd_06[] = {
	STAND,
	CMD_NEXT,
	WAIT2( 14 ),
	CMD_NEXT,
	WALK_F( 32 ),
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_NEXT,
	WALK_F( 16 ),
	CMD_NEXT,
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	WALK_F( 32 ),
	STAND,
	CMD_NEXT,
	CROUCH,
	WAIT2( 8 ),
	CMD_NEXT,
	WALK_F( 16 ),
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	CROUCH,
	WAIT2( 14 ),
	CMD_NEXT,
	WALK_F( 32 ),
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki2d_cmd_07[] = {
	CMD_RESET,
};

const Uint8 taiki2d_cmd_08[] = {
	STAND,
	DASH_EM( 128 ),
	STAND,
	CMD_NEXT,
	WAIT2( 8 ),
	IF_DIST( 128, 240, 0, 0 ),
	CMD_ELSE,
		WALK_F( 32 ),
	CMD_ENDIF,
	CMD_NEXT,
	WAIT2( 8 ),
	DASH_EM( 144 ),
	STAND,
	CMD_NEXT,
	WAIT2( 8 ),
	IF_DIST( 144, 240, 0, 0 ),
	CMD_ELSE,
		WALK_F( 32 ),
	CMD_ENDIF,
	CMD_NEXT,
	WAIT2( 8 ),
	STAND,
	DASH_EM( 112 ),
	STAND,
	CMD_NEXT,
	WAIT2( 8 ),
	IF_DIST( 112, 240, 0, 0 ),
	CMD_ELSE,
		WALK_F( 32 ),
	CMD_ENDIF,
	CMD_NEXT,
	WAIT2( 8 ),
	STAND,
	DASH_EM( 160 ),
	STAND,
	CMD_NEXT,
	WAIT2( 8 ),
	IF_DIST( 160, 240, 0, 0 ),
	CMD_ELSE,
		WALK_F( 32 ),
	CMD_ENDIF,
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_RESET,
};

const Uint8 taiki2d_cmd_09[] = {
	CMD_RESET,
};

const Uint8 taiki2d_cmd_10[] = {
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

const Uint8 taiki2d_cmd_11[] = {
	CMD_RESET,
};

const Uint8 taiki2d_cmd_12[] = {
	CMD_RESET,
};

const Uint8 taiki2d_cmd_13[] = {
	CMD_RESET,
};

const Uint8 taiki2d_cmd_14[] = {
	CMD_RESET,
};

const Uint8 taiki2d_cmd_15[] = {
	CMD_RESET,
};

const Uint8 taiki2d_cmd_16[] = {
	CMD_RESET,
};

const Uint8 taiki2d_cmd_17[] = {
	CMD_RESET,
};

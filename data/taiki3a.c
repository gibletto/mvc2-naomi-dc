/* Servbot (character 3a): the computer's waiting lists.
   What it does while nothing calls for more. PLAYER_WORK's cpu_adrs[0] points here, and em_cmd_sel
   picks a list from it.

   taiki3a_tbl     one record (CPU_DATA): the offsets of the three tables.
   taiki3a_rate    the chance, out of 32, that a list's CMD_NEXT goes over to an attack list
                   (atck3a.c), by the level of play.
   taiki3a_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   taiki3a_cmd     the lists: a pick is a number in this table.
   taiki3a_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                   list of lib3a.c. */
#include "player.h"
#include "cpu_cmd.h"

extern const Uint8 *const taiki3a_sel[], *const taiki3a_cmd[];
extern const Uint8 taiki3a_rate[], taiki3a_sel_0[], taiki3a_sel_1[], taiki3a_sel_2[],
	taiki3a_sel_3[], taiki3a_sel_4[], taiki3a_sel_5[], taiki3a_sel_6[], taiki3a_sel_7[],
	taiki3a_sel_8[], taiki3a_cmd_00[], taiki3a_cmd_01[], taiki3a_cmd_02[], taiki3a_cmd_03[],
	taiki3a_cmd_04[], taiki3a_cmd_05[], taiki3a_cmd_06[], taiki3a_cmd_07[], taiki3a_cmd_08[],
	taiki3a_cmd_09[], taiki3a_cmd_10[];

const CPU_DATA taiki3a_tbl[] = {
	{ ( Sint32 )taiki3a_rate, ( Sint32 )taiki3a_sel, ( Sint32 )taiki3a_cmd },
};

const Uint8 taiki3a_rate[] = {
	 13,  14,  15,  16,  16,  17,  17,  18,  18,  19,  20,  20,  22,  23,  24,  25,
	 26,  27,  28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 *const taiki3a_sel[] = {
	taiki3a_sel_0, taiki3a_sel_1, taiki3a_sel_2, taiki3a_sel_3, taiki3a_sel_4, taiki3a_sel_5,
	taiki3a_sel_6, taiki3a_sel_7, taiki3a_sel_8,
};

const Uint8 taiki3a_sel_0[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
	  3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   4,   5,
};

const Uint8 taiki3a_sel_1[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
	  2,   2,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,   3,   4,   5,
};

const Uint8 taiki3a_sel_2[] = {
	  3,   3,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
	  4,   4,   4,   4,   4,   4,   4,   4,   5,   5,   5,   5,   5,   5,   5,   5,
};

const Uint8 taiki3a_sel_3[] = {
	  2,   2,   2,   2,   3,   3,   3,   3,   4,   4,   4,   4,   4,   4,   4,   4,
	  4,   4,   4,   4,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,
};

const Uint8 taiki3a_sel_4[] = {
	  4,   4,   4,   4,   4,   4,   4,   4,   5,   5,   5,   5,   5,   5,   5,   5,
	  6,   6,   6,   6,   6,   6,   6,   6,   7,   7,   7,   7,   7,   7,   7,   7,
};

const Uint8 taiki3a_sel_5[] = {
	  6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   7,   7,   7,   7,
	  7,   7,   7,   7,   7,   7,   7,   7,   8,   8,   8,   8,   8,   8,   8,   8,
};

const Uint8 taiki3a_sel_6[] = {
	  8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
	  8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
};

const Uint8 taiki3a_sel_7[] = {
	  9,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,
	  9,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,
};

const Uint8 taiki3a_sel_8[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
};

const Uint8 *const taiki3a_cmd[] = {
	taiki3a_cmd_00, taiki3a_cmd_01, taiki3a_cmd_02, taiki3a_cmd_03, taiki3a_cmd_04, taiki3a_cmd_05,
	taiki3a_cmd_06, taiki3a_cmd_07, taiki3a_cmd_08, taiki3a_cmd_09, taiki3a_cmd_10, taiki3a_cmd_10,
	taiki3a_cmd_10, taiki3a_cmd_10, taiki3a_cmd_10, taiki3a_cmd_10, taiki3a_cmd_10, taiki3a_cmd_10,
};

const Uint8 taiki3a_cmd_00[] = {
	WAIT( 64 ),
	CMD_RESET,
};

const Uint8 taiki3a_cmd_01[] = {
	WALK_B( 64 ),
	CMD_RESET,
};

const Uint8 taiki3a_cmd_02[] = {
	STAND,
	IF_DIST( 192, 255, 96, 128 ),
		WALK_B( 128 ),
	CMD_ELSE,
		WALK_F( 128 ),
	CMD_ENDIF,
	CROUCH,
	WAIT( 120 ),
	STAND,
	IF_DIST( 192, 255, 96, 128 ),
		WALK_B( 128 ),
	CMD_ELSE,
		WALK_F( 128 ),
	CMD_ENDIF,
	CROUCH,
	WAIT( 120 ),
	WAIT2( 14 ),
	CMD_NEXT,
	STAND,
	CMD_RESET,
};

const Uint8 taiki3a_cmd_03[] = {
	STAND,
	WAIT( 120 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_RESET,
};

const Uint8 taiki3a_cmd_04[] = {
	STAND,
	WALK_F( 128 ),
	IF_DIST( 64, 255, 32, 128 ),
		JUMP( DIR_BACK ),
	CMD_ELSE,
		CROUCH,
		WAIT( 60 ),
		WALK_F( 128 ),
		IF_DIST( 64, 255, 32, 128 ),
			JUMP( DIR_BACK ),
		CMD_ENDIF,
	CMD_ENDIF,
	CROUCH,
	WAIT( 60 ),
	WAIT2( 14 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki3a_cmd_05[] = {
	STAND,
	WALK_F( 128 ),
	IF_DIST( 64, 255, 32, 128 ),
		DASH_B( 255 ),
	CMD_ELSE,
		CROUCH,
		WAIT( 60 ),
		WALK_F( 128 ),
		IF_DIST( 64, 255, 32, 128 ),
			DASH_B( 255 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CROUCH,
	WAIT( 60 ),
	WAIT2( 14 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki3a_cmd_06[] = {
	STAND,
	WAIT2( 8 ),
	IF_DIST( 64, 255, 32, 128 ),
		WALK_B( 128 ),
	CMD_ELSE,
		IF_DIST( 64, 255, 96, 128 ),
			WALK_B( 64 ),
		CMD_ELSE,
			WALK_EM( 192 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT,
	CMD_JUMP( 0 ),
	CMD_RESET,
};

const Uint8 taiki3a_cmd_07[] = {
	STAND,
	WAIT2( 8 ),
	IF_DIST( 64, 255, 32, 128 ),
		DASH_B( 128 ),
	CMD_ELSE,
		IF_DIST( 64, 255, 96, 128 ),
			DASH_B( 64 ),
		CMD_ELSE,
			DASH_EM( 192 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT,
	CMD_JUMP( 0 ),
	CMD_RESET,
};

const Uint8 taiki3a_cmd_08[] = {
	STAND,
	DASH_EM( 64 ),
	CMD_NEXT,
	DASH_EM( 64 ),
	CMD_NEXT,
	DASH_EM( 64 ),
	CMD_NEXT,
	DASH_EM( 64 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki3a_cmd_09[] = {
	STAND,
	WAIT( 1 ),
	CMD_NEXT,
	WAIT( 1 ),
	CMD_NEXT,
	WAIT( 1 ),
	CMD_NEXT,
	WAIT( 1 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki3a_cmd_10[] = {
	CMD_RESET,
};

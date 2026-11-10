/* Sabretooth (character 2b): the computer's waiting lists.
   What it does while nothing calls for more. PLAYER_WORK's cpu_adrs[0] points here, and em_cmd_sel
   picks a list from it.

   taiki2b_tbl     one record (CPU_DATA): the offsets of the three tables.
   taiki2b_rate    the chance, out of 32, that a list's CMD_NEXT goes over to an attack list
                   (atck2b.c), by the level of play.
   taiki2b_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   taiki2b_cmd     the lists: a pick is a number in this table.
   taiki2b_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                   list of lib2b.c. */
#include "player.h"
#include "cpu_cmd.h"

extern const Uint8 *const taiki2b_sel[], *const taiki2b_cmd[];
extern const Uint8 taiki2b_rate[], taiki2b_sel_0[], taiki2b_sel_1[], taiki2b_sel_2[],
	taiki2b_sel_3[], taiki2b_sel_4[], taiki2b_sel_5[], taiki2b_sel_6[], taiki2b_sel_7[],
	taiki2b_sel_8[], taiki2b_cmd_00[], taiki2b_cmd_01[], taiki2b_cmd_02[], taiki2b_cmd_03[],
	taiki2b_cmd_04[], taiki2b_cmd_05[], taiki2b_cmd_06[], taiki2b_cmd_07[], taiki2b_cmd_08[],
	taiki2b_cmd_09[], taiki2b_cmd_10[], taiki2b_cmd_11[], taiki2b_cmd_12[], taiki2b_cmd_13[],
	taiki2b_cmd_14[], taiki2b_cmd_15[], taiki2b_cmd_16[], taiki2b_cmd_17[], taiki2b_cmd_18[];

const CPU_DATA taiki2b_tbl[] = {
	{ ( Sint32 )taiki2b_rate, ( Sint32 )taiki2b_sel, ( Sint32 )taiki2b_cmd },
};

const Uint8 taiki2b_rate[] = {
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
	 13,  14,  15,  16,  16,  17,  17,  18,  18,  19,  20,  20,  22,  23,  24,  25,
	 26,  27,  28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 *const taiki2b_sel[] = {
	taiki2b_sel_0, taiki2b_sel_1, taiki2b_sel_2, taiki2b_sel_3, taiki2b_sel_4, taiki2b_sel_5,
	taiki2b_sel_6, taiki2b_sel_7, taiki2b_sel_8,
};

const Uint8 taiki2b_sel_0[] = {
	  3,   3,   3,   3,   3,   3,   3,   3,  15,  15,  15,  15,  15,  15,  15,  15,
	 14,  14,  14,  14,  14,  14,  14,  14,  18,  18,  18,  18,  18,  18,  18,  18,
};

const Uint8 taiki2b_sel_1[] = {
	  3,   3,   3,   3,   3,   3,   3,   3,   5,   5,   5,   5,   8,   8,   8,   8,
	  5,   5,   5,   5,   9,   9,   9,   9,  10,  10,  10,  10,  10,  10,  10,  10,
};

const Uint8 taiki2b_sel_2[] = {
	  3,   3,   3,   3,   3,   3,   3,   3,   5,   5,   5,   5,   8,   8,   8,   8,
	  5,   5,   5,   5,   9,   9,   9,   9,  10,  10,  10,  10,  10,  10,  10,  10,
};

const Uint8 taiki2b_sel_3[] = {
	  4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
	  5,   5,   5,   5,   8,   8,   8,   8,   5,   5,   5,   5,   7,   7,   7,   7,
};

const Uint8 taiki2b_sel_4[] = {
	 11,  11,  11,  11,  11,  11,  11,  11,   4,   4,   4,   4,   4,   4,   4,   4,
	  5,   5,   5,   5,   8,   8,   8,   8,   5,   5,   5,   5,  12,  12,  12,  12,
};

const Uint8 taiki2b_sel_5[] = {
	  4,   4,   4,   4,   4,   4,   4,   4,   5,   5,   5,   5,  12,  12,  12,  12,
	 10,  10,  10,  10,  10,  10,  10,  10,  11,  11,  11,  11,  11,  11,  11,  11,
};

const Uint8 taiki2b_sel_6[] = {
	  4,   4,   4,   4,   4,   4,   4,   4,   5,   5,   5,   5,   8,   8,   8,   8,
	  5,   5,   5,   5,  12,  12,  12,  12,  11,  11,  11,  11,  11,  11,  11,  11,
};

const Uint8 taiki2b_sel_7[] = {
	 17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,
	 17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,
};

const Uint8 taiki2b_sel_8[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,
};

const Uint8 *const taiki2b_cmd[] = {
	taiki2b_cmd_00, taiki2b_cmd_01, taiki2b_cmd_02, taiki2b_cmd_03, taiki2b_cmd_04, taiki2b_cmd_05,
	taiki2b_cmd_06, taiki2b_cmd_07, taiki2b_cmd_08, taiki2b_cmd_09, taiki2b_cmd_10, taiki2b_cmd_11,
	taiki2b_cmd_12, taiki2b_cmd_13, taiki2b_cmd_14, taiki2b_cmd_15, taiki2b_cmd_16, taiki2b_cmd_17,
	taiki2b_cmd_18,
};

const Uint8 taiki2b_cmd_00[] = {
	WAIT( 16 ),
	CMD_RESET,
};

const Uint8 taiki2b_cmd_01[] = {
	WALK_EM( 64 ),
	STAND,
	CMD_RESET,
};

const Uint8 taiki2b_cmd_02[] = {
	WALK_B( 64 ),
	STAND,
	CMD_RESET,
};

const Uint8 taiki2b_cmd_03[] = {
	STAND,
	IF_DIST( 40, 0, 40, 0 ),
		DASH_B( 192 ),
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

const Uint8 taiki2b_cmd_04[] = {
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

const Uint8 taiki2b_cmd_05[] = {
	STAND,
	IF_DIST( 72, 0, 72, 0 ),
		CMD_NEXT,
		WAIT( 10 ),
		CMD_NEXT,
	CMD_ELSE,
		WAIT( 10 ),
		JUMP( DIR_FORWARD ),
		STAND,
		CMD_NEXT,
		IF_DIST( 72, 0, 72, 0 ),
			CMD_NEXT,
		CMD_ELSE,
			WAIT( 10 ),
			JUMP( DIR_FORWARD ),
			STAND,
			CMD_NEXT,
			IF_DIST( 72, 0, 72, 0 ),
				CMD_NEXT,
			CMD_ELSE,
				WAIT( 10 ),
				JUMP( DIR_FORWARD ),
				STAND,
				CMD_NEXT,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	CMD_RESET,
};

const Uint8 taiki2b_cmd_06[] = {
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

const Uint8 taiki2b_cmd_07[] = {
	STAND,
	IF_DIST( 48, 0, 48, 0 ),
		WALK_DIST( 128 ),
	CMD_ELSE,
		WALK_F( 96 ),
		STAND,
		CMD_NEXT,
		WAIT( 10 ),
		CMD_NEXT,
		WALK_B( 96 ),
		STAND,
		CMD_NEXT,
		WAIT( 10 ),
		CMD_NEXT,
	CMD_ENDIF,
	CMD_NEXT,
	STAND,
	CMD_RESET,
};

const Uint8 taiki2b_cmd_08[] = {
	CROUCH,
	WAIT2( 14 ),
	CMD_NEXT,
	WAIT( 10 ),
	CMD_NEXT,
	WAIT( 10 ),
	CMD_NEXT,
	WAIT( 10 ),
	CMD_NEXT,
	WAIT( 10 ),
	CMD_RESET,
};

const Uint8 taiki2b_cmd_09[] = {
	STAND,
	IF_DIST( 80, 0, 80, 0 ),
		DASH_B( 256 ),
	CMD_ELSE,
		WALK_F( 64 ),
		CROUCH,
		CMD_NEXT,
		WAIT( 10 ),
		CMD_NEXT,
		WAIT( 10 ),
		CMD_NEXT,
		WAIT( 10 ),
		STAND,
		CMD_NEXT,
		WAIT( 10 ),
		CMD_NEXT,
		WAIT( 10 ),
		CMD_NEXT,
		WAIT( 10 ),
	CMD_ENDIF,
	WAIT2( 14 ),
	CMD_RESET,
};

const Uint8 taiki2b_cmd_10[] = {
	STAND,
	IF_DIST( 48, 0, 48, 0 ),
		DASH_B( 256 ),
		CMD_NEXT,
	CMD_ELSE,
		WALK_EM( 96 ),
		CMD_NEXT,
	CMD_ENDIF,
	CMD_RESET,
};

const Uint8 taiki2b_cmd_11[] = {
	STAND,
	CMD_NEXT,
	IF_DIST( 64, 0, 64, 0 ),
		WALK_DIST( 128 ),
	CMD_ELSE,
		CMD_NEXT,
		IF_RND( 16 ),
			WALK_EM( 96 ),
			STAND,
			CMD_NEXT,
			WAIT( 10 ),
			CMD_NEXT,
		CMD_ELSE,
			WALK_EM( 256 ),
			STAND,
			CMD_NEXT,
			WAIT( 10 ),
			CMD_NEXT,
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT2( 14 ),
	STAND,
	CMD_RESET,
};

const Uint8 taiki2b_cmd_12[] = {
	STAND,
	IF_DIST( 48, 0, 48, 0 ),
		DASH_B( 256 ),
		STAND,
		CMD_NEXT,
	CMD_ENDIF,
	WALK_DIST( 256 ),
	STAND,
	CMD_NEXT,
	WAIT( 10 ),
	CMD_NEXT,
	STAND,
	CMD_RESET,
};

const Uint8 taiki2b_cmd_13[] = {
	STAND,
	IF_DIST( 128, 0, 128, 0 ),
		WALK_ATCK( ATCK_CROUCH_LK ),
		CMD_ENDIF,
	CMD_ELSE,
		WALK_F( 64 ),
		CROUCH,
		CMD_NEXT,
		WAIT( 20 ),
		CMD_NEXT,
		WAIT( 20 ),
		STAND,
		CMD_NEXT,
		WAIT( 20 ),
		CMD_NEXT,
		WAIT( 20 ),
	CMD_ENDIF,
	STAND,
	CMD_RESET,
};

const Uint8 taiki2b_cmd_14[] = {
	IF_DIST( 32, 0, 32, 0 ),
		JUMP( DIR_BACK ),
		STAND,
		CMD_NEXT,
	CMD_ELSE,
		IF_DIST( 88, 0, 88, 0 ),
			IF_RND( 16 ),
				JUMP( DIR_UP ),
				CMD_NEXT,
				STAND,
				CMD_NEXT,
			CMD_ELSE,
				JUMP( DIR_BACK ),
				CMD_NEXT,
				STAND,
				CMD_NEXT,
			CMD_ENDIF,
		CMD_ELSE,
			JUMP( DIR_UP ),
			CMD_NEXT,
			STAND,
			CMD_NEXT,
		CMD_ENDIF,
		CMD_NEXT,
	CMD_ENDIF,
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki2b_cmd_15[] = {
	IF_DIST( 32, 0, 32, 0 ),
		DASH_B( 192 ),
		STAND,
		WAIT( 30 ),
		CMD_NEXT,
		WAIT2( 14 ),
		CMD_NEXT,
	CMD_ELSE,
		IF_DIST( 72, 0, 72, 0 ),
			WALK_DIST( 208 ),
			STAND,
			WAIT( 30 ),
			CMD_NEXT,
			WAIT2( 14 ),
		CMD_ELSE,
			DASH_EM( 48 ),
			STAND,
			WAIT( 30 ),
			CMD_NEXT,
			WAIT2( 14 ),
		CMD_ENDIF,
		CMD_NEXT,
	CMD_ENDIF,
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki2b_cmd_16[] = {
	WAIT( 1 ),
	STAND,
	IF_AREA( 48, 0, 48, 0 ),
		WAIT( 1 ),
		CMD_NEXT,
	CMD_ELSE,
		JUMP( DIR_UP ),
		STAND,
		WAIT( 1 ),
		CMD_NEXT,
		WAIT( 10 ),
		CMD_NEXT,
		JUMP( DIR_UP ),
		STAND,
		WAIT( 1 ),
		CMD_NEXT,
	CMD_ENDIF,
	CMD_RESET,
};

const Uint8 taiki2b_cmd_17[] = {
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

const Uint8 taiki2b_cmd_18[] = {
	IF_DIST( 96, 0, 96, 0 ),
		IF_DIST( 72, 0, 72, 0 ),
			WALK_DIST( 64 ),
			STAND,
			WAIT( 30 ),
			CMD_NEXT,
			WAIT2( 14 ),
		CMD_ELSE,
			WALK_DIST( 208 ),
			STAND,
			WAIT( 30 ),
			CMD_NEXT,
			WAIT2( 14 ),
			CMD_NEXT,
		CMD_ENDIF,
		WAIT2( 14 ),
		CMD_NEXT,
	CMD_ELSE,
		DASH_EM( 64 ),
		STAND,
		WAIT( 30 ),
		CMD_NEXT,
		WAIT2( 14 ),
		CMD_NEXT,
	CMD_ENDIF,
	CMD_RESET,
};

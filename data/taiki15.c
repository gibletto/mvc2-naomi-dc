/* Amingo (character 15): the computer's waiting lists.
   What it does while nothing calls for more. PLAYER_WORK's cpu_adrs[0] points here, and em_cmd_sel
   picks a list from it.

   taiki15_tbl     one record (CPU_DATA): the offsets of the three tables.
   taiki15_rate    the chance, out of 32, that a list's CMD_NEXT goes over to an attack list
                   (atck15.c), by the level of play.
   taiki15_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   taiki15_cmd     the lists: a pick is a number in this table.
   taiki15_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                   list of lib15.c. */
#include "player.h"
#include "pl15_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const taiki15_sel[], *const taiki15_cmd[];
extern const Uint8 taiki15_rate[], taiki15_sel_0[], taiki15_sel_1[], taiki15_sel_2[],
	taiki15_sel_3[], taiki15_sel_4[], taiki15_sel_5[], taiki15_sel_6[], taiki15_sel_7[],
	taiki15_sel_8[], taiki15_cmd_00[], taiki15_cmd_01[], taiki15_cmd_02[], taiki15_cmd_03[],
	taiki15_cmd_04[], taiki15_cmd_05[], taiki15_cmd_06[], taiki15_cmd_07[], taiki15_cmd_08[],
	taiki15_cmd_09[], taiki15_cmd_10[], taiki15_cmd_11[], taiki15_cmd_12[], taiki15_cmd_13[],
	taiki15_cmd_14[], taiki15_cmd_15[], taiki15_cmd_16[], taiki15_cmd_17[];

const CPU_DATA taiki15_tbl[] = {
	{ ( Sint32 )taiki15_rate, ( Sint32 )taiki15_sel, ( Sint32 )taiki15_cmd },
};

const Uint8 taiki15_rate[] = {
	 13,  14,  15,  16,  16,  17,  17,  18,  18,  19,  20,  20,  22,  23,  24,  25,
	 26,  27,  28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 *const taiki15_sel[] = {
	taiki15_sel_0, taiki15_sel_1, taiki15_sel_2, taiki15_sel_3, taiki15_sel_4, taiki15_sel_5,
	taiki15_sel_6, taiki15_sel_7, taiki15_sel_8,
};

const Uint8 taiki15_sel_0[] = {
	  2,   1,   2,   2,   1,   2,   2,   2,   2,   1,   2,   2,   4,   2,   2,   2,
	  3,   1,   3,   3,   1,   3,   3,   3,   3,   1,   3,   3,   1,   3,   3,   3,
};

const Uint8 taiki15_sel_1[] = {
	  3,   2,   2,   2,   2,   2,   4,   2,   4,   2,   2,   2,   2,   2,   4,   2,
	  3,   6,   6,   4,   6,   6,   6,   6,   3,   6,   6,   4,   6,   6,   6,   6,
};

const Uint8 taiki15_sel_2[] = {
	  2,   4,   2,   2,   1,   2,   2,   2,   2,   2,   2,   2,   1,   2,   2,   2,
	  6,   6,   6,   6,   4,   6,   6,   1,   6,   4,   6,   6,   4,   6,   6,   1,
};

const Uint8 taiki15_sel_3[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,   6,   6,   1,   8,   6,   6,   6,   6,
	  6,   6,   6,   1,   6,   6,   6,   6,   5,   5,   5,   5,   5,   5,   5,   5,
};

const Uint8 taiki15_sel_4[] = {
	  2,   2,   2,   2,   4,   2,   2,   2,  11,  11,  11,  11,  11,  11,  11,  11,
	  5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   8,   5,   5,   1,   5,   5,
};

const Uint8 taiki15_sel_5[] = {
	  6,   6,   6,   6,   4,   6,   6,   6,   6,   6,   6,   6,   4,   6,   6,   6,
	 11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,
};

const Uint8 taiki15_sel_6[] = {
	 10,  10,  10,  10,  10,  10,  10,  10,  11,  11,  11,  11,  11,  11,  11,  11,
	 11,  11,  11,  10,  11,  11,  11,  11,   8,   8,   4,   8,   8,   8,   8,   8,
};

const Uint8 taiki15_sel_7[] = {
	 11,  11,  11,  11,   1,  11,  11,  11,  11,  11,  11,  11,   1,  11,  11,  11,
	  8,   8,  10,  10,  10,  10,  10,  10,   8,   8,  10,  10,  10,  10,  10,  10,
};

const Uint8 taiki15_sel_8[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,
};

const Uint8 *const taiki15_cmd[] = {
	taiki15_cmd_00, taiki15_cmd_01, taiki15_cmd_02, taiki15_cmd_03, taiki15_cmd_04, taiki15_cmd_05,
	taiki15_cmd_06, taiki15_cmd_07, taiki15_cmd_08, taiki15_cmd_09, taiki15_cmd_10, taiki15_cmd_11,
	taiki15_cmd_12, taiki15_cmd_13, taiki15_cmd_14, taiki15_cmd_15, taiki15_cmd_16, taiki15_cmd_17,
};

const Uint8 taiki15_cmd_00[] = {
	WAIT( 30 ),
	CMD_RESET,
};

const Uint8 taiki15_cmd_01[] = {
	WAIT2( 14 ),
	WALK_F( 16 ),
	CMD_RESET,
};

const Uint8 taiki15_cmd_02[] = {
	STAND,
	IF_DIST( 64, 240, 32, 128 ),
		WALK_B( 64 ),
	CMD_ELSE,
		IF_DIST( 64, 240, 144, 128 ),
			WALK_F( 80 ),
		CMD_ELSE,
			SP_CMD( pl15_sp_daichi_catch, 0, PK_PUNCH, LV_HEAVY, 0 ),
			WAIT( 20 ),
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	WAIT2( 14 ),
	CMD_NEXT,
	STAND,
	CMD_RESET,
};

const Uint8 taiki15_cmd_03[] = {
	STAND,
	WAIT( 60 ),
	CMD_NEXT,
	WAIT2( 6 ),
	CMD_RESET,
};

const Uint8 taiki15_cmd_04[] = {
	STAND,
	WAIT2( 8 ),
	JUMP( DIR_BACK ),
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT( 20 ),
	DASH_B( 384 ),
	SP_CMD( pl15_sp_daichi_catch, 0, PK_PUNCH, LV_HEAVY, 0 ),
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
	SP_CMD( pl15_sp_daichi_catch, 0, PK_PUNCH, LV_LIGHT, 0 ),
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

const Uint8 taiki15_cmd_05[] = {
	CMD_NEXT,
	STAND,
	WAIT2( 14 ),
	CMD_NEXT,
	WALK_B( 16 ),
	JUMP( DIR_BACK ),
	STAND,
	WAIT( 10 ),
	SP_CMD( pl15_sp_daichi_catch, 0, PK_PUNCH, LV_LIGHT, 0 ),
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

const Uint8 taiki15_cmd_06[] = {
	STAND,
	CMD_NEXT,
	WAIT( 10 ),
	CMD_NEXT,
	TIMER_SET( 60 ),
	SP_CMD( pl15_sp_daichi_catch, 0, PK_PUNCH, LV_LIGHT, 0 ),
	WALK_DIST( 160 ),
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	WAIT2( 8 ),
	CMD_NEXT,
	WALK_B( 64 ),
	CMD_NEXT,
	STAND,
	WAIT( 10 ),
	SP_CMD( pl15_sp_daichi_catch, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_NEXT,
	STAND,
	WAIT2( 8 ),
	CMD_NEXT,
	STAND,
	CMD_RESET,
};

const Uint8 taiki15_cmd_07[] = {
	CMD_RESET,
};

const Uint8 taiki15_cmd_08[] = {
	STAND,
	SP_CMD( pl15_sp_daichi_catch, 0, PK_PUNCH, LV_HEAVY, 0 ),
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
	SP_CMD( pl15_sp_daichi_catch, 0, PK_PUNCH, LV_LIGHT, 0 ),
	JUMP( DIR_BACK ),
	STAND,
	CMD_NEXT,
	STAND,
	CMD_RESET,
};

const Uint8 taiki15_cmd_09[] = {
	CMD_RESET,
};

const Uint8 taiki15_cmd_10[] = {
	STAND,
	CMD_NEXT,
	STAND,
	WAIT2( 4 ),
	CMD_NEXT,
	STAND,
	WAIT2( 2 ),
	CMD_NEXT,
	CMD_RESET,
};

const Uint8 taiki15_cmd_11[] = {
	STAND,
	IF_DIST( 64, 240, 32, 128 ),
		WALK_B( 64 ),
	CMD_ELSE,
		IF_DIST( 64, 240, 160, 128 ),
			SP_CMD( pl15_sp_daichi_catch, 0, PK_PUNCH, LV_LIGHT, 0 ),
			WALK_F( 48 ),
		CMD_ELSE,
			WAIT2( 4 ),
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	CMD_NEXT,
	STAND,
	CMD_RESET,
};

const Uint8 taiki15_cmd_12[] = {
	CMD_RESET,
};

const Uint8 taiki15_cmd_13[] = {
	CMD_RESET,
};

const Uint8 taiki15_cmd_14[] = {
	CMD_RESET,
};

const Uint8 taiki15_cmd_15[] = {
	CMD_RESET,
};

const Uint8 taiki15_cmd_16[] = {
	CMD_RESET,
};

const Uint8 taiki15_cmd_17[] = {
	CMD_RESET,
};

/* Sabretooth (character 2b): the computer's attack lists.
   What it does once a waiting list's CMD_NEXT has sent it to attack. PLAYER_WORK's cpu_adrs[1]
   points here.

   atck2b_tbl     one record (CPU_DATA): the offsets of the three tables.
   atck2b_rate    the chance, out of 32, that a list's CMD_NEXT2 starts another attack list, by
                  the level of play.
   atck2b_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   atck2b_cmd     the lists: a pick is a number in this table.
   atck2b_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                  list of lib2b.c. */
#include "player.h"
#include "pl2b_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const atck2b_sel[], *const atck2b_cmd[];
extern const Uint8 atck2b_rate[], atck2b_sel_0[], atck2b_sel_1[], atck2b_sel_2[], atck2b_sel_3[],
	atck2b_sel_4[], atck2b_sel_5[], atck2b_sel_6[], atck2b_sel_7[], atck2b_cmd_00[], atck2b_cmd_01[],
	atck2b_cmd_02[], atck2b_cmd_03[], atck2b_cmd_04[], atck2b_cmd_05[], atck2b_cmd_06[],
	atck2b_cmd_07[], atck2b_cmd_08[], atck2b_cmd_09[], atck2b_cmd_10[], atck2b_cmd_11[],
	atck2b_cmd_12[], atck2b_cmd_13[], atck2b_cmd_14[], atck2b_cmd_15[];

const CPU_DATA atck2b_tbl[] = {
	{ ( Sint32 )atck2b_rate, ( Sint32 )atck2b_sel, ( Sint32 )atck2b_cmd },
};

const Uint8 atck2b_rate[] = {
	 28,  28,  29,  29,  30,  30,  31,  31,  28,  28,  29,  29,  30,  30,  31,  31,
	 28,  28,  29,  29,  30,  30,  31,  31,  28,  28,  29,  29,  30,  30,  31,  31,
};

const Uint8 *const atck2b_sel[] = {
	atck2b_sel_0, atck2b_sel_1, atck2b_sel_2, atck2b_sel_3, atck2b_sel_4, atck2b_sel_5,
	atck2b_sel_6, atck2b_sel_7,
};

const Uint8 atck2b_sel_0[] = {
	  0,   0,   0,   0,   1,   1,   1,   2,   0,   0,   0,   0,   1,   1,   1,   2,
	  0,   0,   0,   0,   1,   1,   1,   2,   0,   0,   0,   0,   1,   1,   1,   2,
};

const Uint8 atck2b_sel_1[] = {
	  0,   0,   1,   1,   1,   2,   3,   4,   0,   0,   1,   1,   1,   2,   3,   4,
	  0,   0,   1,   1,   1,   2,   3,   4,   0,   0,   1,   1,   1,   2,   3,   4,
};

const Uint8 atck2b_sel_2[] = {
	  0,   1,   1,   2,   2,   3,   3,   4,   0,   1,   1,   2,   2,   3,   3,   4,
	  0,   1,   1,   2,   2,   3,   3,   4,   0,   1,   1,   2,   2,   3,   3,   4,
};

const Uint8 atck2b_sel_3[] = {
	  0,   2,   2,   3,   3,   4,   5,   6,   0,   2,   2,   3,   3,   4,   5,   6,
	  1,   2,   2,   3,   3,   4,   5,   6,   1,   2,   2,   3,   3,   4,   5,   6,
};

const Uint8 atck2b_sel_4[] = {
	  0,   2,   3,   4,   5,   6,   7,   8,   0,   2,   3,   4,   5,   6,   7,   8,
	  1,   2,   3,   4,   5,   6,   7,   8,   1,   2,   3,   4,   5,   6,   7,   8,
};

const Uint8 atck2b_sel_5[] = {
	  2,   3,   4,   6,   8,   8,   9,   9,   2,   3,   4,   6,   8,   8,   9,   9,
	  2,   3,   5,   7,   8,   8,   9,   9,   2,   3,   5,   7,   8,   8,   9,   9,
};

const Uint8 atck2b_sel_6[] = {
	  0,   2,   5,   7,   8,   9,  10,  12,   1,   3,   5,   7,   8,   9,  10,  13,
	  2,   4,   6,   8,   9,   9,  11,  14,   3,   4,   6,   8,   9,   9,  11,  15,
};

const Uint8 atck2b_sel_7[] = {
	  0,   4,   8,   8,   9,   9,  10,  12,   1,   5,   8,   8,   9,   9,  10,  13,
	  2,   6,   8,   8,   9,   9,  11,  14,   3,   7,   8,   8,   9,   9,  11,  15,
};

const Uint8 *const atck2b_cmd[] = {
	atck2b_cmd_00, atck2b_cmd_01, atck2b_cmd_02, atck2b_cmd_03, atck2b_cmd_04, atck2b_cmd_05,
	atck2b_cmd_06, atck2b_cmd_07, atck2b_cmd_08, atck2b_cmd_09, atck2b_cmd_10, atck2b_cmd_11,
	atck2b_cmd_12, atck2b_cmd_13, atck2b_cmd_14, atck2b_cmd_15,
};

const Uint8 atck2b_cmd_00[] = {
	STAND,
	CMD_JUMP( 0 ),
	CMD_JUMP( 1 ),
	IF_DIST( 80, 32, 80, 32 ),
		WALK_EM( 48 ),
		LOCK_SET,
		CMD_JUMP( 15 ),
		LOCK_CLR,
	CMD_ELSE,
		IF_RND( 8 ),
			SP_CMD( pl2b_sp_clawx, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			WALK_DIST( 192 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck2b_cmd_01[] = {
	STAND,
	CMD_JUMP( 0 ),
	CMD_JUMP( 1 ),
	IF_DIST( 80, 32, 80, 32 ),
		TIMER_SET( 120 ),
		WALK_ATCK( ATCK_STAND_HP ),
			IF_RND( 12 ),
				DASH_EM( 8 ),
			CMD_ELSE,
				WALK_EM( 16 ),
			CMD_ENDIF,
			LOCK_SET,
			IF_RND( 20 ),
				CMD_JUMP( 17 ),
			CMD_ELSE,
				CMD_JUMP( 15 ),
			CMD_ENDIF,
			LOCK_CLR,
		CMD_ELSE,
			CMD_JUMP( 0 ),
			DASH_B( 192 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_RND( 8 ),
			SP_CMD( pl2b_sp_clawx, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			WALK_DIST( 192 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck2b_cmd_02[] = {
	STAND,
	CMD_JUMP( 0 ),
	CMD_JUMP( 1 ),
	IF_DIST( 80, 32, 80, 32 ),
		DASH_EM( 8 ),
		LOCK_SET,
		CMD_JUMP( 15 ),
		LOCK_CLR,
		CMD_JUMP( 0 ),
		DASH_B( 192 ),
	CMD_ELSE,
		DASH_EM( 80 ),
		LOCK_SET,
		CMD_JUMP( 17 ),
		LOCK_CLR,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck2b_cmd_03[] = {
	STAND,
	CMD_JUMP( 0 ),
	CMD_JUMP( 1 ),
	IF_DIST( 80, 32, 80, 32 ),
		DASH_EM( 8 ),
		LOCK_SET,
		CMD_JUMP( 15 ),
		LOCK_CLR,
		CMD_JUMP( 0 ),
		DASH_B( 192 ),
	CMD_ELSE,
		DASH_EM( 64 ),
		IF_LEVEL( 16 ),
			IF_RND( 16 ),
				CMD_JUMP( 18 ),
			CMD_ELSE,
				IF_RND( 16 ),
					CMD_JUMP( 17 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			IF_RND( 16 ),
				CMD_JUMP( 17 ),
			CMD_ELSE,
				CMD_JUMP( 0 ),
				DASH_B( 192 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck2b_cmd_04[] = {
	STAND,
	CMD_JUMP( 0 ),
	CMD_JUMP( 1 ),
	WALK_EM( 128 ),
	JUMP( DIR_FORWARD ),
	JUMP_FLAG_SET,
		TIMER_SET( 120 ),
		IF_HEIGHT( 0xff, 80 ),
			CMD_JUMP( 19 ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	IF_EM_ATCK( ATCK_STAND_LP ),
		IF_RND( 16 ),
			CMD_JUMP( 15 ),
		CMD_ELSE,
			CMD_JUMP( 17 ),
		CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 0 ),
		DASH_B( 192 ),
	CMD_ENDIF,
	CMD_JUMP( 2 ),
	CMD_EXIT,
};

const Uint8 atck2b_cmd_05[] = {
	STAND,
	CMD_JUMP( 0 ),
	CMD_JUMP( 1 ),
	WALK_DIST( 128 ),
	JUMP( DIR_FORWARD ),
	JUMP_FLAG_SET,
		TIMER_SET( 120 ),
		IF_HEIGHT( 0xff, 96 ),
			CMD_JUMP( 19 ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	IF_RND( 20 ),
		DASH_EM( 8 ),
		CMD_JUMP( 15 ),
	CMD_ELSE,
		CMD_JUMP( 0 ),
		DASH_B( 192 ),
		TIMER_SET( 20 ),
		IF_DIST( 80, 32, 80, 32 ),
			CMD_JUMP( 17 ),
		CMD_ELSE,
			WALK_DIST( 160 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_JUMP( 2 ),
	CMD_EXIT,
};

const Uint8 atck2b_cmd_06[] = {
	STAND,
	CMD_JUMP( 0 ),
	CMD_JUMP( 1 ),
	WALK_DIST( 256 ),
	TIMER_SET( 10 ),
	IF_DIST( 80, 32, 80, 32 ),
		CMD_JUMP( 17 ),
		WALK_DIST( 256 ),
	CMD_ENDIF,
	CMD_JUMP( 0 ),
	DASH_EM( 192 ),
	JUMP( DIR_FORWARD ),
	JUMP_FLAG_SET,
		TIMER_SET( 120 ),
		IF_HEIGHT( 0xff, 96 ),
			CMD_JUMP( 19 ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	IF_RND( 20 ),
		DASH_EM( 8 ),
		CMD_JUMP( 15 ),
	CMD_ELSE,
		DASH_B( 192 ),
		CMD_JUMP( 0 ),
		IF_DIST( 80, 32, 80, 32 ),
			CMD_JUMP( 17 ),
		CMD_ELSE,
			WALK_DIST( 160 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_JUMP( 2 ),
	CMD_EXIT,
};

const Uint8 atck2b_cmd_07[] = {
	CMD_JUMP( 0 ),
	CMD_JUMP( 1 ),
	JUMP_RND( 20, 21, 22, 23 ),
	JUMP_RND( 20, 21, 22, 23 ),
	JUMP_RND( 20, 21, 22, 23 ),
	CMD_JUMP( 2 ),
	CMD_EXIT,
};

const Uint8 atck2b_cmd_08[] = {
	CMD_JUMP( 0 ),
	CMD_JUMP( 1 ),
	DASH_EM( 8 ),
	CMD_JUMP( 41 ),
	SJUMP_FLAG_SET,
		WAIT( 4 ),
		TIMER_SET( 120 ),
		IF_EM_ATCK( ATCK_JUMP_LP ),
			CMD_JUMP( 36 ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	CMD_EXIT,
};

const Uint8 atck2b_cmd_09[] = {
	CMD_JUMP( 0 ),
	CMD_JUMP( 1 ),
	IF_RND( 16 ),
		WALK_DIST( 144 ),
	CMD_ELSE,
		DASH_EM( 256 ),
	CMD_ENDIF,
	JUMP( DIR_FORWARD ),
	JUMP_FLAG_SET,
		TIMER_SET( 120 ),
		IF_HEIGHT( 0xff, 96 ),
			CMD_JUMP( 19 ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	DASH_EM( 8 ),
	CMD_JUMP( 41 ),
	SJUMP_FLAG_SET,
		WAIT( 4 ),
		TIMER_SET( 120 ),
		IF_EM_ATCK( ATCK_JUMP_LP ),
			CMD_JUMP( 36 ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	CMD_JUMP( 2 ),
	CMD_EXIT,
};

const Uint8 atck2b_cmd_10[] = {
	CMD_JUMP( 0 ),
	CMD_JUMP( 1 ),
	DASH_EM( 8 ),
	CMD_JUMP( 46 ),
	CMD_EXIT,
};

const Uint8 atck2b_cmd_11[] = {
	STAND,
	CMD_JUMP( 0 ),
	CMD_JUMP( 1 ),
	WALK_DIST( 128 ),
	JUMP( DIR_FORWARD ),
	JUMP_FLAG_SET,
		TIMER_SET( 120 ),
		IF_HEIGHT( 0xff, 96 ),
			CMD_JUMP( 19 ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	IF_RND( 20 ),
		DASH_EM( 8 ),
		CMD_JUMP( 46 ),
	CMD_ELSE,
		CMD_JUMP( 0 ),
		DASH_B( 192 ),
		TIMER_SET( 20 ),
		IF_DIST( 80, 32, 80, 32 ),
			CMD_JUMP( 17 ),
		CMD_ELSE,
			WALK_DIST( 160 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_JUMP( 2 ),
	CMD_EXIT,
};

const Uint8 atck2b_cmd_12[] = {
	STAND,
	SJUMP( DIR_FORWARD ),
	SJUMP_FLAG_SET,
		TIMER_SET( 120 ),
		IF_HEIGHT( 1, 128 ),
			CMD_JUMP( 47 ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	CMD_JUMP( 2 ),
	CMD_EXIT,
};

const Uint8 atck2b_cmd_13[] = {
	STAND,
	IF_RND( 10 ),
		DASH_EM( 8 ),
		JUMP_RND( 5, 6, 7, 8 ),
		CMD_JUMP( 0 ),
	CMD_ENDIF,
	DASH_B( 256 ),
	IF_GAUGE( 3 ),
		SP_V_COMBI,
	CMD_ENDIF,
	CMD_JUMP( 2 ),
	CMD_EXIT,
};

const Uint8 atck2b_cmd_14[] = {
	STAND,
	CMD_JUMP( 0 ),
	CMD_JUMP( 1 ),
	DASH_EM( 8 ),
	LOCK_SET,
	JUMP_RND( 3, 3, 4, 4 ),
	LOCK_CLR,
	CMD_JUMP( 0 ),
	DASH_B( 192 ),
	CMD_EXIT,
};

const Uint8 atck2b_cmd_15[] = {
	STAND,
	WALK_DIST( 128 ),
	CMD_JUMP( 0 ),
	DASH_EM( 128 ),
	CMD_JUMP( 37 ),
	SJUMP_FLAG_SET,
		WAIT( 4 ),
		TIMER_SET( 120 ),
		IF_EM_ATCK( ATCK_JUMP_LP ),
			CMD_JUMP( 24 ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	WAIT2( 6 ),
	DASH_B( 256 ),
	CMD_EXIT,
};

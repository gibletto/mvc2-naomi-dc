/* Omega Red (character 30): the computer's attack lists.
   What it does once a waiting list's CMD_NEXT has sent it to attack. PLAYER_WORK's cpu_adrs[1]
   points here.

   atck30_tbl     one record (CPU_DATA): the offsets of the three tables.
   atck30_rate    the chance, out of 32, that a list's CMD_NEXT2 starts another attack list, by
                  the level of play.
   atck30_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   atck30_cmd     the lists: a pick is a number in this table.
   atck30_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                  list of lib30.c. */
#include "player.h"
#include "pl30_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const atck30_sel[], *const atck30_cmd[];
extern const Uint8 atck30_rate[], atck30_sel_0[], atck30_sel_1[], atck30_sel_2[], atck30_sel_3[],
	atck30_sel_4[], atck30_sel_5[], atck30_sel_6[], atck30_sel_7[], atck30_cmd_00[], atck30_cmd_01[],
	atck30_cmd_02[], atck30_cmd_03[], atck30_cmd_04[], atck30_cmd_05[], atck30_cmd_06[],
	atck30_cmd_07[], atck30_cmd_08[], atck30_cmd_09[], atck30_cmd_10[], atck30_cmd_11[],
	atck30_cmd_12[], atck30_cmd_13[], atck30_cmd_14[], atck30_cmd_17[];

const CPU_DATA atck30_tbl[] = {
	{ ( Sint32 )atck30_rate, ( Sint32 )atck30_sel, ( Sint32 )atck30_cmd },
};

const Uint8 atck30_rate[] = {
	 11,  12,  13,  14,  14,  15,  15,  16,  16,  17,  18,  19,  20,  21,  22,  23,
	 24,  25,  26,  27,  28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 *const atck30_sel[] = {
	atck30_sel_0, atck30_sel_1, atck30_sel_2, atck30_sel_3, atck30_sel_4, atck30_sel_5,
	atck30_sel_6, atck30_sel_7,
};

const Uint8 atck30_sel_0[] = {
	  0,   0,   0,   0,   1,   1,   1,   1,   2,   2,   2,   2,   3,   3,   3,   3,
	  4,   4,   4,   4,   5,   5,   5,   5,   6,   6,   6,   6,   7,   7,   7,   7,
};

const Uint8 atck30_sel_1[] = {
	  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,   0,   1,
	  2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,   0,   1,   2,   3,
};

const Uint8 atck30_sel_2[] = {
	  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,   0,   1,
	  2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,   0,   1,   2,   3,
};

const Uint8 atck30_sel_3[] = {
	  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,   0,   1,
	  2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,   0,   1,   2,   3,
};

const Uint8 atck30_sel_4[] = {
	  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,   0,   1,
	  2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,   0,   1,   2,   3,
};

const Uint8 atck30_sel_5[] = {
	  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,   0,   1,
	  2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,   0,   1,   2,   3,
};

const Uint8 atck30_sel_6[] = {
	  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,   0,   1,
	  2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,   0,   1,   2,   3,
};

const Uint8 atck30_sel_7[] = {
	  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,   0,   1,
	  2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,   0,   1,   2,   3,
};

const Uint8 *const atck30_cmd[] = {
	atck30_cmd_00, atck30_cmd_01, atck30_cmd_02, atck30_cmd_03, atck30_cmd_04, atck30_cmd_05,
	atck30_cmd_06, atck30_cmd_07, atck30_cmd_08, atck30_cmd_09, atck30_cmd_10, atck30_cmd_11,
	atck30_cmd_12, atck30_cmd_13, atck30_cmd_14, atck30_cmd_14, atck30_cmd_14, atck30_cmd_17,
	atck30_cmd_17, atck30_cmd_17, atck30_cmd_17, atck30_cmd_17, atck30_cmd_17, atck30_cmd_17,
	atck30_cmd_17, atck30_cmd_17, atck30_cmd_17, atck30_cmd_17, atck30_cmd_17, atck30_cmd_17,
	atck30_cmd_17, atck30_cmd_17,
};

const Uint8 atck30_cmd_00[] = {
	WAIT2( 6 ),
	JUMP( DIR_BACK ),
	JUMP_FLAG_SET,
		IF_RND( 16 ),
			WAIT2( 12 ),
			CMD_JUMP( 1 ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	CMD_JUMP( 5 ),
	IF_RND( 16 ),
		WAIT2( 8 ),
		CMD_JUMP( 5 ),
	CMD_ELSE,
		JUMP( DIR_FORWARD ),
		CMD_JUMP( 9 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck30_cmd_01[] = {
	WAIT2( 6 ),
	DASH_F( 240 ),
	WAIT2( 6 ),
	IF_RND( 16 ),
		IF_EM_ATCK( ATCK_STAND_LK ),
			CANCEL_SET2,
				STAND,
				KICK( LV_LIGHT ),
				KICK( LV_LIGHT ),
				KICK( LV_HEAVY ),
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_EM_ATCK( ATCK_STAND_LK ),
			CANCEL_SET2,
				CROUCH,
				KICK( LV_LIGHT ),
				KICK( LV_LIGHT ),
				CMD_JUMP( 15 ),
				IF_RND( 16 ),
					CMD_JUMP( 6 ),
				CMD_ENDIF,
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	IF_RND( 10 ),
		CMD_JUMP( 53 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck30_cmd_02[] = {
	WAIT2( 6 ),
	WALK_EM( 128 ),
	IF_EM_ATCK( ATCK_STAND_HP ),
		PUNCH( LV_HEAVY ),
		IF_RND( 14 ),
			CMD_JUMP( 7 ),
		CMD_ELSE,
			IF_RND( 8 ),
				CMD_JUMP( 8 ),
			CMD_ELSE,
				CMD_JUMP( 16 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 11 ),
	CMD_ENDIF,
	CMD_JUMP( 53 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck30_cmd_03[] = {
	WAIT2( 6 ),
	SJUMP( DIR_UP ),
	SJUMP_FLAG_SET,
		TIMER_SET( 60 ),
		IF_HEIGHT( 1, 192 ),
			IF_RND( 16 ),
				FLY( LEVER_FORWARD, 240 ),
			CMD_ENDIF,
			CMD_JUMP( 1 ),
			WAIT2( 6 ),
			CMD_JUMP( 34 ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck30_cmd_04[] = {
	STAND,
	WAIT2( 6 ),
	CMD_JUMP( 15 ),
	IF_RND( 24 ),
		CMD_JUMP( 14 ),
	CMD_ENDIF,
	IF_RND( 8 ),
		CMD_JUMP( 24 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck30_cmd_05[] = {
	STAND,
	WALK_B( 128 ),
	IF_EM_ATCK( ATCK_STAND_HK ),
		WAIT2( 8 ),
		KICK( LV_HEAVY ),
	CMD_ELSE,
		IF_RND( 16 ),
			WAIT2( 8 ),
			JUMP( DIR_FORWARD ),
			JUMP_FLAG_SET,
				WAIT( 30 ),
				CMD_JUMP( 32 ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			WAIT2( 8 ),
			CMD_JUMP( 53 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck30_cmd_06[] = {
	STAND,
	DASH_B( 240 ),
	WAIT2( 10 ),
	CMD_JUMP( 33 ),
	CMD_JUMP( 35 ),
	IF_LEVEL( 10 ),
		WAIT2( 6 ),
		CMD_JUMP( 33 ),
		CMD_JUMP( 2 ),
	CMD_ELSE,
		CMD_JUMP( 11 ),
	CMD_ENDIF,
	IF_LEVEL( 16 ),
		WAIT2( 6 ),
		SP_CMD( pl30_sp_destroyer, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ENDIF,
	IF_RND( 8 ),
		CMD_JUMP( 24 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck30_cmd_07[] = {
	STAND,
	TIMER_SET( 120 ),
	WALK_DIST( 192 ),
	WAIT2( 6 ),
	IF_LEVEL( 8 ),
		JUMP( DIR_FORWARD ),
		JUMP_FLAG_SET,
			TIMER_SET( 60 ),
			IF_HEIGHT( 0xff, 64 ),
				CMD_JUMP( 32 ),
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
		STAND,
		IF_RND( 16 ),
			JUMP( DIR_FORWARD ),
			JUMP_FLAG_SET,
				TIMER_SET( 60 ),
				IF_HEIGHT( 0xff, 64 ),
					CMD_JUMP( 32 ),
				CMD_ENDIF,
				JUMP_FLAG_CLR,
			CMD_ENDIF,
			STAND,
		CMD_ENDIF,
		STAND,
		SJUMP( DIR_FORWARD ),
		SJUMP_FLAG_SET,
			IF_EM_STANCE( STANCE_AIR ),
				TIMER_SET( 120 ),
				IF_HEIGHT( 0xff, 192 ),
					PUNCH( LV_HEAVY ),
				CMD_ENDIF,
			CMD_ELSE,
				TIMER_SET( 120 ),
				IF_HEIGHT( 1, 192 ),
					PUNCH( LV_HEAVY ),
				CMD_ENDIF,
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
		STAND,
	CMD_ELSE,
		TIMER_SET( 120 ),
		WALK_DIST( 160 ),
		WAIT2( 6 ),
		JUMP( DIR_FORWARD ),
		JUMP_FLAG_SET,
			TIMER_SET( 60 ),
			IF_HEIGHT( 0xff, 64 ),
				PUNCH( LV_HEAVY ),
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	IF_RND( 16 ),
		CMD_JUMP( 24 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck30_cmd_08[] = {
	STAND,
	WAIT2( 6 ),
	CANCEL_SET2,
		CMD_JUMP( 37 ),
		IF_RND( 16 ),
			WAIT( 10 ),
			SP_CMD( pl30_sp_strike, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ELSE,
			SJUMP( DIR_FORWARD ),
			SJUMP_FLAG_SET,
				TIMER_SET( 0x80 ),
				IF_HEIGHT( 0xff, 128 ),
					PUNCH( LV_HEAVY ),
				CMD_ENDIF,
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
		CANCEL_CLR,
	CMD_ENDIF,
	IF_RND( 16 ),
		CMD_JUMP( 24 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck30_cmd_09[] = {
	STAND,
	DASH_ATCK( ATCK_CROUCH_LP ),
		CROUCH,
		PUNCH( LV_LIGHT ),
		KICK( LV_LIGHT ),
		DASH_B( 240 ),
		JUMP( DIR_FORWARD ),
		JUMP_FLAG_SET,
			WAIT( 16 ),
			CMD_JUMP( 32 ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		IF_RND( 16 ),
			JUMP( DIR_FORWARD ),
			JUMP_FLAG_SET,
				WAIT( 4 ),
				PUNCH( LV_HEAVY ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			CMD_JUMP( 6 ),
			IF_LEVEL( 10 ),
				SP_CMD( pl30_sp_destroyer, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_JUMP( 53 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck30_cmd_10[] = {
	STAND,
	JUMP( DIR_BACK ),
	JUMP_FLAG_SET,
		WAIT( 8 ),
		PUNCH( LV_HEAVY ),
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	IF_DIST( 48, 0, 0, 0 ),
		JUMP( DIR_BACK ),
		JUMP_FLAG_SET,
			WAIT( 8 ),
			PUNCH( LV_HEAVY ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
		STAND,
		IF_RND( 16 ),
			CMD_JUMP( 53 ),
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP( DIR_UP ),
	JUMP_FLAG_SET,
		TIMER_SET( 96 ),
		IF_HEIGHT( 0xff, 50 ),
			FLY( LEVER_FORWARD, 80 ),
			PUNCH( LV_HEAVY ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	JUMP_LEVEL( 10, 16, 2, 22, 7, 26, 3 ),
	CMD_JUMP( 53 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck30_cmd_11[] = {
	WAIT2( 6 ),
	IF_NG( 1 ),
		CMD_JUMP( 53 ),
		IF_RND( 16 ),
			CMD_JUMP( 14 ),
		CMD_ELSE,
			JUMP( DIR_FORWARD ),
			JUMP_FLAG_SET,
				TIMER_SET( 60 ),
				IF_HEIGHT( 0xff, 64 ),
					CMD_JUMP( 32 ),
				CMD_ENDIF,
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 29 ),
	CMD_ENDIF,
	STAND,
	IF_LEVEL( 16 ),
		CMD_JUMP( 3 ),
	CMD_ELSE,
		CMD_JUMP( 2 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck30_cmd_12[] = {
	WAIT2( 6 ),
	IF_NG( 1 ),
		CMD_JUMP( 53 ),
		IF_RND( 16 ),
			JUMP( DIR_UP ),
			LOCK_SET,
			JUMP_FLAG_SET,
				TIMER_SET( 120 ),
				IF_HEIGHT( 1, 30 ),
					SP_CMD( pl30_sp_jdestroyer, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ENDIF,
				JUMP_FLAG_CLR,
			CMD_ENDIF,
			LOCK_CLR,
		CMD_ELSE,
			WAIT2( 6 ),
			SP_CMD( pl30_sp_destroyer, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 21 ),
		CMD_JUMP( 3 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck30_cmd_13[] = {
	IF_NG( 1 ),
		CMD_JUMP( 53 ),
	CMD_ELSE,
		IF_RND( 16 ),
			JUMP( DIR_UP ),
			LOCK_SET,
			JUMP_FLAG_SET,
				TIMER_SET( 120 ),
				IF_HEIGHT( 1, 30 ),
					SP_CMD( pl30_sp_jdestroyer, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ENDIF,
				JUMP_FLAG_CLR,
			CMD_ENDIF,
			LOCK_CLR,
		CMD_ELSE,
			CMD_JUMP( 4 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck30_cmd_14[] = {
	STAND,
	CMD_JUMP( 54 ),
	CMD_NEXT2,
	CMD_EXIT,
	STAND,
	SJUMP( DIR_UP ),
	SJUMP_FLAG_SET,
		TIMER_SET( 0xc8 ),
		IF_HEIGHT( 0xff, 192 ),
			CMD_JUMP( 1 ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck30_cmd_17[] = {
	CMD_EXIT,
};

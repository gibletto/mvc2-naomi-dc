/* Sentinel (character 34): the computer's attack lists.
   What it does once a waiting list's CMD_NEXT has sent it to attack. PLAYER_WORK's cpu_adrs[1]
   points here.

   atck34_tbl     one record (CPU_DATA): the offsets of the three tables.
   atck34_rate    the chance, out of 32, that a list's CMD_NEXT2 starts another attack list, by
                  the level of play.
   atck34_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   atck34_cmd     the lists: a pick is a number in this table.
   atck34_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                  list of lib34.c. */
#include "player.h"
#include "pl34_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const atck34_sel[], *const atck34_cmd[];
extern const Uint8 atck34_rate[], atck34_sel_0[], atck34_sel_1[], atck34_sel_2[], atck34_sel_3[],
	atck34_sel_4[], atck34_sel_5[], atck34_sel_6[], atck34_sel_7[], atck34_cmd_00[], atck34_cmd_01[],
	atck34_cmd_02[], atck34_cmd_03[], atck34_cmd_04[], atck34_cmd_05[], atck34_cmd_06[],
	atck34_cmd_07[], atck34_cmd_08[], atck34_cmd_09[], atck34_cmd_10[], atck34_cmd_11[],
	atck34_cmd_12[], atck34_cmd_13[], atck34_cmd_14[], atck34_cmd_15[], atck34_cmd_16[],
	atck34_cmd_17[], atck34_cmd_18[], atck34_cmd_19[], atck34_cmd_20[], atck34_cmd_21[],
	atck34_cmd_22[], atck34_cmd_23[], atck34_cmd_24[], atck34_cmd_25[], atck34_cmd_26[],
	atck34_cmd_27[], atck34_cmd_28[], atck34_cmd_29[], atck34_cmd_30[], atck34_cmd_31[];

const CPU_DATA atck34_tbl[] = {
	{ ( Sint32 )atck34_rate, ( Sint32 )atck34_sel, ( Sint32 )atck34_cmd },
};

const Uint8 atck34_rate[] = {
	 10,  10,  13,  13,  15,  15,  17,  17,  18,  19,  20,  21,  22,  23,  24,  25,
	 25,  26,  27,  27,  27,  27,  27,  27,  28,  28,  29,  29,  30,  30,  31,  31,
};

const Uint8 *const atck34_sel[] = {
	atck34_sel_0, atck34_sel_1, atck34_sel_2, atck34_sel_3, atck34_sel_4, atck34_sel_5,
	atck34_sel_6, atck34_sel_7,
};

const Uint8 atck34_sel_0[] = {
	  0,   0,   0,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,
	  7,   7,   7,   7,   7,   7,   7,   7,   6,   6,   7,   7,   7,   7,   7,   7,
};

const Uint8 atck34_sel_1[] = {
	  3,   3,   3,   3,   1,   1,   3,   4,   3,   3,   3,   3,   1,   1,   3,   4,
	  6,   6,   6,   6,   1,   1,   3,   4,   0,   0,   0,   0,   1,   1,   3,   4,
};

const Uint8 atck34_sel_2[] = {
	  7,   7,   1,   1,   2,   6,   3,   3,   7,   7,   1,   1,   2,   6,   3,   3,
	  7,   7,   1,   1,   2,   6,   3,   3,   7,   7,  28,   1,   2,   6,   3,   3,
};

const Uint8 atck34_sel_3[] = {
	 12,   9,   1,   1,   2,   6,   3,   8,  12,   9,   1,   1,   2,   6,   3,   8,
	 12,   9,   1,   1,   2,   6,   3,   8,  12,   9,  13,  13,   2,   6,   3,   8,
};

const Uint8 atck34_sel_4[] = {
	  9,   9,   1,   1,   3,   2,   8,   8,   9,   9,   1,   1,   3,   6,   8,   8,
	  0,   0,  13,  13,   1,  12,   8,  10,  28,   0,  13,  13,   1,  12,   8,  10,
};

const Uint8 atck34_sel_5[] = {
	  9,   9,   1,   1,   3,   2,   8,   8,   9,   9,   1,  13,   3,   6,   8,   8,
	  0,   0,  13,  13,   1,  12,   8,  10,  28,  28,  13,  13,   1,  12,   8,  10,
};

const Uint8 atck34_sel_6[] = {
	  9,   9,   1,  13,   3,   2,   8,   8,   9,   9,  13,  13,   3,   6,   8,   8,
	  0,   0,  13,  13,   1,  12,   8,  10,  28,  28,  28,  13,   1,  12,   8,  10,
};

const Uint8 atck34_sel_7[] = {
	  9,   9,  13,  13,   3,   2,   8,   8,   9,   9,  13,  13,   3,   6,   8,   8,
	  9,   9,  13,  13,   1,  12,   8,  10,  12,  28,  28,  28,  12,  28,  28,  10,
};

const Uint8 *const atck34_cmd[] = {
	atck34_cmd_00, atck34_cmd_01, atck34_cmd_02, atck34_cmd_03, atck34_cmd_04, atck34_cmd_05,
	atck34_cmd_06, atck34_cmd_07, atck34_cmd_08, atck34_cmd_09, atck34_cmd_10, atck34_cmd_11,
	atck34_cmd_12, atck34_cmd_13, atck34_cmd_14, atck34_cmd_15, atck34_cmd_16, atck34_cmd_17,
	atck34_cmd_18, atck34_cmd_19, atck34_cmd_20, atck34_cmd_21, atck34_cmd_22, atck34_cmd_23,
	atck34_cmd_24, atck34_cmd_25, atck34_cmd_26, atck34_cmd_27, atck34_cmd_28, atck34_cmd_29,
	atck34_cmd_30, atck34_cmd_31,
};

const Uint8 atck34_cmd_00[] = {
	STAND,
	WALK_B( 64 ),
	TIMER_SET( 60 ),
	IF_DIST( 64, 32, 64, 0 ),
		SP_CMD( pl34_sp_rocket2, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ELSE,
		IF_LEVEL( 16 ),
			SP_CMD( pl34_sp_force, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ELSE,
			WAIT2( 8 ),
			SP_CMD( pl34_sp_force, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck34_cmd_01[] = {
	STAND,
	WAIT2( 8 ),
	CMD_JUMP( 14 ),
	JUMP_FLAG_SET,
		CMD_JUMP( 20 ),
		TIMER_SET( 60 ),
		IF_HEIGHT( 0, 48 ),
			CMD_JUMP( 9 ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	WAIT2( 8 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck34_cmd_02[] = {
	STAND,
	WAIT2( 4 ),
	IF_RND( 10 ),
		PARTNER_CALL,
	CMD_ENDIF,
	DASH_B( 1024 ),
	WAIT2( 4 ),
	SP_CMD( pl34_sp_force, 0, PK_PUNCH, LV_LIGHT, 0 ),
	TIMER_SET( 10 ),
	IF_EM_ATCK( ATCK_CROUCH_LP ),
		CROUCH,
		PUNCH( LV_LIGHT ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck34_cmd_03[] = {
	STAND,
	CMD_JUMP( 0 ),
	STAND,
	WAIT2( 2 ),
	TIMER_SET( 60 ),
	WALK_ATCK( ATCK_STAND_LP ),
		WAIT2( 2 ),
		IF_EM_ATCK( ATCK_STAND_LP ),
			CANCEL_SET2,
				PUNCH( LV_LIGHT ),
				PUNCH( LV_LIGHT ),
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			WAIT2( 2 ),
			CROUCH,
			PUNCH( LV_HEAVY ),
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	WAIT2( 4 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck34_cmd_04[] = {
	STAND,
	WAIT2( 14 ),
	STAND,
	IF_AREA( 96, 48, 224, 144 ),
		IF_LEVEL( 24 ),
			IF_RND( 10 ),
				SP_CMD( pl34_sp_force, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ELSE,
				SP_CMD( pl34_sp_force, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_RND( 10 ),
				SP_CMD( pl34_sp_force, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 24 ),
			SP_CMD_SET( pl34_sp_hforce, 0, PK_PUNCH, LV_LIGHT, 0, 1, 30 ),
			SP_CMD( pl34_sp_rocket, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			SP_CMD( pl34_sp_rocket, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck34_cmd_05[] = {
	STAND,
	WAIT2( 4 ),
	TIMER_SET( 60 ),
	DASH_ATCK( ATCK_CROUCH_LK ),
		CROUCH,
		IF_LEVEL( 16 ),
			CANCEL_SET,
				PUNCH( LV_LIGHT ),
				IF_EM_ATCK( ATCK_CROUCH_MP ),
					STAND,
					KICK( LV_HEAVY ),
					WAIT( 4 ),
					SJUMP3( DIR_UP ),
					CMD_JUMP( 4 ),
				CMD_ELSE,
					IF_EM_ATCK( ATCK_CROUCH_HK ),
						CROUCH,
						KICK( LV_HEAVY ),
					CMD_ENDIF,
				CMD_ENDIF,
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			KICK( LV_HEAVY ),
		CMD_ENDIF,
	CMD_ELSE,
		PUNCH( LV_LIGHT ),
	CMD_ENDIF,
	STAND,
	CMD_EXIT,
};

const Uint8 atck34_cmd_06[] = {
	STAND,
	WAIT2( 4 ),
	CMD_JUMP( 0 ),
	IF_AREA( 240, 16, 256, 96 ),
		SP_CMD( pl34_sp_rocket, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck34_cmd_07[] = {
	STAND,
	WAIT2( 4 ),
	SP_CMD( pl34_sp_force, 0, PK_PUNCH, LV_HEAVY, 0 ),
	STAND,
	CMD_EXIT,
};

const Uint8 atck34_cmd_08[] = {
	STAND,
	IF_GAUGE( 1 ),
		IF_RND( 16 ),
			WAIT2( 2 ),
			IF_AREA( 48, 64, 96, 64 ),
				SP_CMD( pl34_sp_plasma, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				SP_CMD( pl34_sp_hforce, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		PARTNER_CALL,
		JUMP( DIR_BACK ),
		WAIT( 30 ),
		SP_CMD( pl34_sp_rocket, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck34_cmd_09[] = {
	STAND,
	TIMER_SET( 60 ),
	DASH_EM( 128 ),
	STAND,
	JUMP( DIR_FORWARD ),
	JUMP_FLAG_SET,
		CMD_JUMP( 20 ),
		TIMER_SET( 30 ),
		IF_AREA( 64, 32, 128, 0 ),
			KICK( LV_LIGHT ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	IF_RND( 16 ),
		IF_EM_ATCK( ATCK_CROUCH_LK ),
			CROUCH,
			CANCEL_SET2,
				KICK( LV_LIGHT ),
				KICK( LV_LIGHT ),
				WAIT( 15 ),
				SP_CMD( pl34_sp_rocket, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			DASH_B( 256 ),
			WAIT2( 2 ),
			SP_CMD( pl34_sp_rocket, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ELSE,
		SP_CMD( pl34_sp_rocket, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 atck34_cmd_10[] = {
	STAND,
	WAIT2( 2 ),
	IF_RND( 16 ),
		IF_GAUGE( 2 ),
			SP_V_COMBI,
			WAIT2( 2 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_GAUGE( 1 ),
			SP_CMD( pl34_sp_plasma, 0, PK_PUNCH, LV_LIGHT, 0 ),
			WAIT2( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	WALK_B( 16 ),
	STAND,
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 atck34_cmd_11[] = {
	STAND,
	WAIT2( 2 ),
	CMD_JUMP( 14 ),
	JUMP_FLAG_SET,
		TIMER_SET( 100 ),
		IF_HEIGHT( 0xff, 80 ),
			IF_EM_ATCK( ATCK_JUMP_HP ),
				CANCEL_SET2,
					PUNCH( LV_LIGHT ),
					PUNCH( LV_HEAVY ),
					CANCEL_CLR,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	CANCEL_SET2,
		KICK( LV_HEAVY ),
		WAIT( 4 ),
		SJUMP3( DIR_FORWARD ),
		CMD_JUMP( 4 ),
		CANCEL_CLR,
	CMD_ENDIF,
	DASH_B( 1024 ),
	WAIT2( 14 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck34_cmd_12[] = {
	STAND,
	WAIT2( 2 ),
	CMD_JUMP( 14 ),
	JUMP_FLAG_SET,
		TIMER_SET( 100 ),
		IF_HEIGHT( 0xff, 80 ),
			IF_EM_ATCK( ATCK_JUMP_HP ),
				CANCEL_SET2,
					PUNCH( LV_LIGHT ),
					PUNCH( LV_HEAVY ),
					SP_CMD( pl34_sp_fly, 0, PK_PUNCH, LV_LIGHT, 0 ),
					CANCEL_CLR,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	FLY_FLAG_SET,
		CANCEL_SET2,
			KICK( LV_LIGHT ),
			PUNCH( LV_HEAVY ),
			SP_CMD( pl34_sp_rocket, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CANCEL_CLR,
		CMD_ENDIF,
		WALK_POS( 192, 0 ),
		SP_CMD( pl34_sp_fly, 0, PK_PUNCH, LV_LIGHT, 0 ),
		FLY_FLAG_CLR,
	CMD_ELSE,
		DASH_B( 1024 ),
		WAIT2( 14 ),
		FLY_FLAG_CLR,
	CMD_HIENDIF,
	STAND,
	CMD_EXIT,
};

const Uint8 atck34_cmd_13[] = {
	STAND,
	WAIT2( 6 ),
	CMD_JUMP( 0 ),
	IF_DIST( 64, 48, 96, 96 ),
		IF_RND( 10 ),
			STAND,
			JUMP( DIR_FORWARD ),
			JUMP_FLAG_SET,
				JUMP_LEVEL( 48, 7, 49, 13, 50, 21, 51 ),
				JUMP_LEVEL( 38, 8, 38, 29, 39, 31, 40 ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
			STAND,
			WAIT2( 4 ),
		CMD_ELSE,
			IF_RND( 20 ),
				DASH_EM( 64 ),
				SP_CMD( pl34_sp_force, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
		WAIT2( 2 ),
	CMD_ENDIF,
	IF_RND( 16 ),
		CMD_JUMP( 12 ),
	CMD_ELSE,
		CMD_JUMP( 13 ),
	CMD_ENDIF,
	IF_LEVEL( 10 ),
		JUMP_FLAG_SET,
			CMD_JUMP( 20 ),
			IF_RND( 16 ),
				IF_RND( 12 ),
					JUMP_LEVEL( 38, 13, 39, 21, 40, 31, 41 ),
				CMD_ELSE,
					JUMP_LEVEL( 38, 20, 39, 26, 40, 31, 41 ),
				CMD_ENDIF,
			CMD_ENDIF,
			CMD_JUMP( 35 ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		JUMP_FLAG_SET,
			CMD_JUMP( 20 ),
			CMD_JUMP( 35 ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	CMD_JUMP( 7 ),
	WAIT2( 4 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck34_cmd_14[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck34_cmd_15[] = {
	CMD_EXIT,
};

const Uint8 atck34_cmd_16[] = {
	CMD_EXIT,
};

const Uint8 atck34_cmd_17[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck34_cmd_18[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck34_cmd_19[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck34_cmd_20[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck34_cmd_21[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck34_cmd_22[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck34_cmd_23[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck34_cmd_24[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck34_cmd_25[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck34_cmd_26[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck34_cmd_27[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck34_cmd_28[] = {
	DASH_F( 1024 ),
	JUMP( DIR_FORWARD ),
	WAIT( 2 ),
	SP_CMD( pl34_sp_fly, 0, PK_PUNCH, LV_LIGHT, 0 ),
	FLY_FLAG_SET,
		CANCEL_SET2,
			KICK( LV_LIGHT ),
			PUNCH( LV_HEAVY ),
			CANCEL_CLR,
		CMD_ENDIF,
		WALK_POS_Y( 192, 80 ),
		FLY_FLAG_CLR,
	CMD_ELSE,
		WAIT2( 14 ),
		DASH_B( 1024 ),
		SP_CMD( pl34_sp_rocket, 0, PK_PUNCH, LV_LIGHT, 0 ),
		FLY_FLAG_CLR,
	CMD_HIENDIF,
	STAND,
	WAIT2( 2 ),
	WALK_F( 16 ),
	CMD_EXIT,
};

const Uint8 atck34_cmd_29[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck34_cmd_30[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck34_cmd_31[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

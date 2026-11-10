/* Amingo (character 15): the computer's attack lists.
   What it does once a waiting list's CMD_NEXT has sent it to attack. PLAYER_WORK's cpu_adrs[1]
   points here.

   atck15_tbl     one record (CPU_DATA): the offsets of the three tables.
   atck15_rate    the chance, out of 32, that a list's CMD_NEXT2 starts another attack list, by
                  the level of play.
   atck15_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   atck15_cmd     the lists: a pick is a number in this table.
   atck15_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                  list of lib15.c. */
#include "player.h"
#include "pl15_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const atck15_sel[], *const atck15_cmd[];
extern const Uint8 atck15_rate[], atck15_sel_0[], atck15_sel_1[], atck15_sel_2[], atck15_sel_3[],
	atck15_sel_4[], atck15_sel_5[], atck15_sel_6[], atck15_sel_7[], atck15_cmd_00[], atck15_cmd_01[],
	atck15_cmd_02[], atck15_cmd_03[], atck15_cmd_04[], atck15_cmd_05[], atck15_cmd_06[],
	atck15_cmd_07[], atck15_cmd_08[], atck15_cmd_09[], atck15_cmd_10[], atck15_cmd_11[],
	atck15_cmd_12[], atck15_cmd_13[], atck15_cmd_14[], atck15_cmd_15[], atck15_cmd_16[],
	atck15_cmd_17[], atck15_cmd_18[], atck15_cmd_19[], atck15_cmd_20[], atck15_cmd_21[],
	atck15_cmd_22[], atck15_cmd_23[], atck15_cmd_24[], atck15_cmd_25[], atck15_cmd_26[],
	atck15_cmd_27[], atck15_cmd_28[], atck15_cmd_29[], atck15_cmd_30[], atck15_cmd_31[];

const CPU_DATA atck15_tbl[] = {
	{ ( Sint32 )atck15_rate, ( Sint32 )atck15_sel, ( Sint32 )atck15_cmd },
};

const Uint8 atck15_rate[] = {
	 31,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,
	 23,  24,  25,  26,  27,  27,  28,  28,  31,  10,  21,  22,  24,  26,  16,  30,
};

const Uint8 *const atck15_sel[] = {
	atck15_sel_0, atck15_sel_1, atck15_sel_2, atck15_sel_3, atck15_sel_4, atck15_sel_5,
	atck15_sel_6, atck15_sel_7,
};

const Uint8 atck15_sel_0[] = {
	 16,   0,   0,   0,   1,   2,   3,   3,   0,   0,   5,  16,   1,   2,  14,   3,
	 14,  20,   0,  15,   1,   2,   3,   3,   6,   6,   7,   7,   1,   2,   3,   5,
};

const Uint8 atck15_sel_1[] = {
	 17,   0,   0,   0,   1,   1,   3,  14,   0,   0,  14,   0,   1,   1,   3,   4,
	  0,  14,   0,   0,   1,   1,  14,   4,   5,   0,  14,   0,   5,  16,   3,   4,
};

const Uint8 atck15_sel_2[] = {
	 18,   7,   1,  14,   2,   6,   3,   3,  14,   7,   1,   1,   2,   6,   3,   3,
	  7,   7,   1,   1,   5,   6,   3,  14,   7,   7,  14,   1,   2,   6,   3,   3,
};

const Uint8 atck15_sel_3[] = {
	 12,  11,   1,   1,   2,   6,   3,   8,  12,  14,   1,   1,   2,   6,   3,   8,
	 12,  11,  16,   5,   2,   6,   3,   9,   5,   5,  13,  13,   2,  14,   3,  18,
};

const Uint8 atck15_sel_4[] = {
	 11,  11,   0,   1,   3,   2,   8,   8,  11,  14,   1,  18,   3,   6,  14,   8,
	 11,  16,  13,  13,   5,  12,   9,  10,  14,  11,  13,  14,   5,  12,   9,  10,
};

const Uint8 atck15_sel_5[] = {
	 16,  14,   1,   1,   3,   2,  18,  14,  17,  11,  16,  13,   3,   6,   8,   8,
	 18,  11,  14,  16,   5,  12,   9,  10,  19,  11,  13,  14,   5,  12,   9,  10,
};

const Uint8 atck15_sel_6[] = {
	 16,  14,   1,  12,   3,   2,   8,  14,  16,  11,  13,   1,   3,   6,   8,   8,
	 17,  11,  17,  19,   5,  12,   9,  10,  18,  14,  19,  13,   5,  12,  19,  10,
};

const Uint8 atck15_sel_7[] = {
	 16,  14,  13,  13,   3,   2,  19,   8,  16,  11,  14,  12,   3,   6,   1,   8,
	 17,  11,  19,  12,   5,  12,  19,  10,  18,  14,   1,   5,   5,  19,   9,  10,
};

const Uint8 *const atck15_cmd[] = {
	atck15_cmd_00, atck15_cmd_01, atck15_cmd_02, atck15_cmd_03, atck15_cmd_04, atck15_cmd_05,
	atck15_cmd_06, atck15_cmd_07, atck15_cmd_08, atck15_cmd_09, atck15_cmd_10, atck15_cmd_11,
	atck15_cmd_12, atck15_cmd_13, atck15_cmd_14, atck15_cmd_15, atck15_cmd_16, atck15_cmd_17,
	atck15_cmd_18, atck15_cmd_19, atck15_cmd_20, atck15_cmd_21, atck15_cmd_22, atck15_cmd_23,
	atck15_cmd_24, atck15_cmd_25, atck15_cmd_26, atck15_cmd_27, atck15_cmd_28, atck15_cmd_29,
	atck15_cmd_30, atck15_cmd_31,
};

const Uint8 atck15_cmd_00[] = {
	WAIT2( 8 ),
	WALK_B( 256 ),
	WAIT2( 4 ),
	SP_CMD( pl15_sp_kaze, 0, PK_PUNCH, LV_HEAVY, 0 ),
	WAIT2( 6 ),
	SP_CMD( pl15_sp_daichi, 0, PK_PUNCH, LV_LIGHT, 0 ),
	IF_LEVEL( 24 ),
		IF_GAUGE( 2 ),
			IF_RND( 16 ),
				SP_CMD( pl15_sp_daichi_catch, 0, PK_PUNCH, LV_LIGHT, 0 ),
				SP_CMD( pl15_sp_okite, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				SP_CMD( pl15_sp_daichi, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ENDIF,
		CMD_ELSE,
			CMD_JUMP( 14 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_RND( 8 ),
			CMD_JUMP( 14 ),
		CMD_ELSE,
			SP_CMD( pl15_sp_kaze, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ENDIF,
		WAIT2( 8 ),
		SP_CMD( pl15_sp_daichi, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ENDIF,
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck15_cmd_01[] = {
	STAND,
	WAIT2( 4 ),
	DASH_ATCK( ATCK_CROUCH_MP ),
		IF_LEVEL( 16 ),
			IF_EM_ATCK( ATCK_CROUCH_LP ),
				CROUCH,
				CANCEL_SET2,
					PUNCH( LV_LIGHT ),
					PUNCH( LV_LIGHT ),
					SJUMP3( DIR_FORWARD ),
					CANCEL_CLR,
				CMD_ENDIF,
				WAIT( 2 ),
				SJUMP_FLAG_SET,
					CMD_JUMP( 4 ),
					JUMP_FLAG_CLR,
				CMD_ENDIF,
			CMD_ELSE,
				IF_RND( 16 ),
					SP_CMD( pl15_sp_daichi, 0, PK_PUNCH, LV_HEAVY, 0 ),
				CMD_ELSE,
					SP_CMD( pl15_sp_daichi_catch, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			CMD_JUMP( 8 ),
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck15_cmd_02[] = {
	STAND,
	TIMER_SET( 60 ),
	WALK_DIST( 64 ),
	CMD_JUMP( 11 ),
	STAND,
	WAIT2( 4 ),
	SP_CMD( pl15_sp_daichi_catch, 0, PK_PUNCH, LV_LIGHT, 0 ),
	IF_LEVEL( 16 ),
		IF_RND( 16 ),
			CMD_JUMP( 8 ),
		CMD_ELSE,
			CMD_JUMP( 5 ),
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck15_cmd_03[] = {
	STAND,
	CMD_JUMP( 0 ),
	WAIT2( 4 ),
	CMD_JUMP( 6 ),
	TIMER_SET( 90 ),
	WALK_DIST( 8 ),
	CROUCH,
	WAIT2( 2 ),
	IF_LEVEL( 16 ),
		IF_GAUGE( 1 ),
			CANCEL_SET2,
				KICK( LV_HEAVY ),
				SP_CMD( pl15_sp_taiyou, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			KICK( LV_HEAVY ),
		CMD_ENDIF,
	CMD_ELSE,
		KICK( LV_HEAVY ),
	CMD_ENDIF,
	WAIT2( 4 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck15_cmd_04[] = {
	STAND,
	IF_LEVEL( 16 ),
		WAIT2( 8 ),
	CMD_ELSE,
		WAIT2( 14 ),
	CMD_ENDIF,
	TIMER_SET( 60 ),
	IF_EM_ATCK( ATCK_CROUCH_HP ),
		SP_CMD( pl15_sp_midori, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ELSE,
		SP_CMD( pl15_sp_daichi, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ENDIF,
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck15_cmd_05[] = {
	STAND,
	DASH_EM( 16 ),
	IF_LEVEL( 22 ),
		IF_EM_ATCK( ATCK_CROUCH_LK ),
			CROUCH,
			CANCEL_SET2,
				KICK( LV_LIGHT ),
				KICK( LV_LIGHT ),
				SJUMP3( DIR_FORWARD ),
				CANCEL_CLR,
			CMD_ENDIF,
			SJUMP_FLAG_SET,
				CMD_JUMP( 4 ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			CROUCH,
			KICK( LV_LIGHT ),
			KICK( LV_HEAVY ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_RND( 16 ),
			IF_EM_ATCK( ATCK_CROUCH_LK ),
				CANCEL_SET2,
					CROUCH,
					KICK( LV_LIGHT ),
					KICK( LV_LIGHT ),
					SJUMP3( DIR_FORWARD ),
					CANCEL_CLR,
				CMD_ENDIF,
				WAIT( 1 ),
				SJUMP_FLAG_SET,
					TIMER_SET( 120 ),
					IF_EM_ATCK( ATCK_JUMP_LP ),
						PUNCH( LV_LIGHT ),
						PUNCH( LV_HEAVY ),
					CMD_ENDIF,
					JUMP_FLAG_CLR,
				CMD_ENDIF,
			CMD_ELSE,
				IF_EM_ATCK( ATCK_CROUCH_HK ),
					CROUCH,
					KICK( LV_HEAVY ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			PUNCH( LV_HEAVY ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_JUMP( 0 ),
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck15_cmd_06[] = {
	STAND,
	CMD_JUMP( 0 ),
	SP_CMD( pl15_sp_kaze, 0, PK_PUNCH, LV_HEAVY, 0 ),
	STAND,
	IF_RND( 16 ),
		SP_CMD( pl15_sp_midori, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ENDIF,
	IF_LEVEL( 16 ),
		IF_RND( 16 ),
			SP_CMD( pl15_sp_midori, 0, PK_PUNCH, LV_HEAVY, 0 ),
			SP_CMD( pl15_sp_kaze, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck15_cmd_07[] = {
	STAND,
	SP_CMD( pl15_sp_daichi_catch, 0, PK_PUNCH, LV_LIGHT, 0 ),
	WAIT2( 2 ),
	SP_CMD( pl15_sp_midori, 0, PK_PUNCH, LV_HEAVY, 0 ),
	STAND,
	WAIT2( 2 ),
	IF_EM_ATCK( ATCK_CROUCH_HK ),
		CROUCH,
		KICK( LV_HEAVY ),
	CMD_ENDIF,
	IF_LEVEL( 16 ),
		IF_RND( 24 ),
			CMD_JUMP( 26 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_RND( 16 ),
			CMD_JUMP( 8 ),
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck15_cmd_08[] = {
	STAND,
	IF_GAUGE( 1 ),
		IF_RND( 16 ),
			WAIT2( 2 ),
			IF_AREA( 240, 48, 128, 16 ),
				SP_CMD( pl15_sp_daichi_catch, 0, PK_PUNCH, LV_LIGHT, 0 ),
				WALK_B( 32 ),
				SP_CMD( pl15_sp_okite, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ELSE,
			CMD_JUMP( 54 ),
		CMD_ENDIF,
	CMD_ELSE,
		STAND,
		WAIT2( 2 ),
		SP_CMD( pl15_sp_daichi_catch, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ENDIF,
	CMD_JUMP( 0 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck15_cmd_09[] = {
	STAND,
	IF_GAUGE( 1 ),
		IF_RND( 16 ),
			IF_EM_ATCK( 81 ),
				SP_CMD( pl15_sp_okite, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		STAND,
		WAIT2( 2 ),
		SP_CMD( pl15_sp_daichi, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ENDIF,
	CMD_JUMP( 0 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck15_cmd_10[] = {
	STAND,
	WAIT2( 2 ),
	IF_RND( 16 ),
		IF_GAUGE( 2 ),
			IF_AREA( 64, 240, 80, 16 ),
				SP_V_COMBI,
			CMD_ENDIF,
			WAIT2( 2 ),
		CMD_ELSE,
			PARTNER_CALL,
		CMD_ENDIF,
	CMD_ELSE,
		WAIT( 1 ),
		CMD_JUMP( 0 ),
	CMD_ENDIF,
	STAND,
	CMD_JUMP( 0 ),
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck15_cmd_11[] = {
	STAND,
	DASH_B( 96 ),
	STAND,
	IF_AREA( 128, 64, 128, 16 ),
		SP_CMD( pl15_sp_daichi, 0, PK_PUNCH, LV_HEAVY, 0 ),
		STAND,
		CMD_JUMP( 24 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	IF_RND( 16 ),
		WAIT2( 2 ),
		IF_AREA( 128, 240, 64, 0 ),
			IF_LEVEL( 22 ),
				IF_GAUGE( 2 ),
					IF_RND( 10 ),
						SP_CMD( pl15_sp_taiyou, 0, PK_PUNCH, LV_LIGHT, 0 ),
					CMD_ENDIF,
				CMD_ELSE,
					SP_CMD( pl15_sp_daichi_catch, 0, PK_PUNCH, LV_LIGHT, 0 ),
					CMD_JUMP( 24 ),
				CMD_ENDIF,
			CMD_ELSE,
				SP_CMD( pl15_sp_okite, 0, PK_PUNCH, LV_LIGHT, 0 ),
				STAND,
				CMD_JUMP( 24 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_JUMP( 0 ),
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck15_cmd_12[] = {
	STAND,
	CMD_JUMP( 14 ),
	JUMP( DIR_FORWARD ),
	WAIT( 4 ),
	JUMP_FLAG_SET,
		CMD_JUMP( 20 ),
		WAIT( 6 ),
		SP_CMD( pl15_sp_kaze, 0, PK_PUNCH, LV_HEAVY, 0 ),
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	WAIT2( 2 ),
	CMD_JUMP( 21 ),
	CMD_JUMP( 0 ),
	STAND,
	IF_LEVEL( 10 ),
		IF_RND( 16 ),
			CMD_JUMP( 26 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck15_cmd_13[] = {
	STAND,
	LOCK_SET,
	CMD_JUMP( 0 ),
	CMD_JUMP( 25 ),
	JUMP_FLAG_SET,
		JUMP_LEVEL( 38, 8, 38, 29, 39, 31, 40 ),
		CMD_JUMP( 20 ),
		WAIT( 10 ),
		IF_LEVEL( 10 ),
			IF_RND( 16 ),
				IF_RND( 12 ),
					JUMP_LEVEL( 38, 13, 39, 21, 40, 31, 41 ),
				CMD_ELSE,
					JUMP_LEVEL( 38, 20, 39, 26, 40, 31, 41 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
		TIMER_SET( 60 ),
		IF_HEIGHT( 1, 128 ),
			IF_RND( 12 ),
				JUMP_LEVEL( 35, 8, 35, 22, 9, 31, 9 ),
			CMD_ELSE,
				JUMP_LEVEL( 35, 8, 35, 22, 35, 24, 9 ),
			CMD_ENDIF,
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	JUMP_LEVEL( 7, 8, 7, 15, 7, 24, 7 ),
	LOCK_CLR,
	WAIT2( 4 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck15_cmd_14[] = {
	IF_LEVEL( 16 ),
		JUMP_RND( 7, 10, 11, 17 ),
	CMD_ENDIF,
	IF_RND( 16 ),
		SP_CMD( pl15_sp_kaze, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ELSE,
		IF_RND( 16 ),
			SP_CMD( pl15_sp_daichi, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ELSE,
			SP_CMD( pl15_sp_daichi, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck15_cmd_15[] = {
	CMD_JUMP( 14 ),
	CMD_JUMP( 58 ),
	CMD_JUMP( 0 ),
	CMD_EXIT,
};

const Uint8 atck15_cmd_16[] = {
	CMD_JUMP( 54 ),
	CMD_JUMP( 0 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck15_cmd_17[] = {
	CMD_JUMP( 56 ),
	CMD_JUMP( 0 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck15_cmd_18[] = {
	CMD_JUMP( 53 ),
	CMD_JUMP( 0 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck15_cmd_19[] = {
	DASH_EM( 8 ),
	CROUCH,
	CANCEL_SET2,
		PUNCH( LV_LIGHT ),
		PUNCH( LV_LIGHT ),
		KICK( LV_HEAVY ),
		SP_CMD_SET( pl15_sp_taiyou, 0, PK_PUNCH, LV_LIGHT, 0, 1, 20 ),
		SP_CMD( pl15_sp_midori, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CANCEL_CLR,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck15_cmd_20[] = {
	DASH_B( 256 ),
	CMD_JUMP( 23 ),
	CMD_JUMP( 0 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck15_cmd_21[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck15_cmd_22[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck15_cmd_23[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck15_cmd_24[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck15_cmd_25[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck15_cmd_26[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck15_cmd_27[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck15_cmd_28[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck15_cmd_29[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck15_cmd_30[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck15_cmd_31[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

/* Tron Bonne (character 10): the computer's attack lists.
   What it does once a waiting list's CMD_NEXT has sent it to attack. PLAYER_WORK's cpu_adrs[1]
   points here.

   atck10_tbl     one record (CPU_DATA): the offsets of the three tables.
   atck10_rate    the chance, out of 32, that a list's CMD_NEXT2 starts another attack list, by
                  the level of play.
   atck10_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   atck10_cmd     the lists: a pick is a number in this table.
   atck10_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                  list of lib10.c. */
#include "player.h"
#include "pl10_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const atck10_sel[], *const atck10_cmd[];
extern const Uint8 atck10_rate[], atck10_sel_0[], atck10_sel_1[], atck10_sel_2[], atck10_sel_3[],
	atck10_sel_4[], atck10_sel_5[], atck10_sel_6[], atck10_sel_7[], atck10_cmd_00[], atck10_cmd_01[],
	atck10_cmd_02[], atck10_cmd_03[], atck10_cmd_04[], atck10_cmd_05[], atck10_cmd_06[],
	atck10_cmd_07[], atck10_cmd_08[], atck10_cmd_09[], atck10_cmd_10[], atck10_cmd_11[],
	atck10_cmd_12[], atck10_cmd_13[], atck10_cmd_14[], atck10_cmd_15[], atck10_cmd_16[],
	atck10_cmd_17[], atck10_cmd_18[], atck10_cmd_19[], atck10_cmd_20[], atck10_cmd_21[],
	atck10_cmd_22[], atck10_cmd_23[], atck10_cmd_24[], atck10_cmd_25[], atck10_cmd_26[],
	atck10_cmd_27[], atck10_cmd_28[], atck10_cmd_29[], atck10_cmd_30[], atck10_cmd_31[];

const CPU_DATA atck10_tbl[] = {
	{ ( Sint32 )atck10_rate, ( Sint32 )atck10_sel, ( Sint32 )atck10_cmd },
};

const Uint8 atck10_rate[] = {
	  8,   8,   8,   8,   9,   9,  10,  10,  11,  11,  12,  12,  13,  13,  14,  14,
	 15,  15,  16,  16,  17,  17,  18,  18,  19,  20,  21,  22,  24,  26,  28,  30,
};

const Uint8 *const atck10_sel[] = {
	atck10_sel_0, atck10_sel_1, atck10_sel_2, atck10_sel_3, atck10_sel_4, atck10_sel_5,
	atck10_sel_6, atck10_sel_7,
};

const Uint8 atck10_sel_0[] = {
	 16,   0,   0,   0,   1,   2,   3,   3,   0,   0,   0,   0,   1,   2,   3,   3,
	  0,   0,   0,   0,   1,   2,   3,   3,   6,   6,   7,   7,   1,   2,   3,  14,
};

const Uint8 atck10_sel_1[] = {
	 17,   0,   0,   0,   1,   1,   3,   4,   0,   0,   0,   0,   1,   1,   3,   4,
	  0,   0,   0,   0,   1,   1,   3,   4,   0,   0,  14,  14,   1,   1,   3,   4,
};

const Uint8 atck10_sel_2[] = {
	 18,   7,   1,   1,   2,   6,   3,   3,   7,   7,   1,   1,   2,   6,   3,   3,
	  7,   7,   1,   1,   5,   6,   3,   3,   7,   7,  14,  14,   2,   6,   3,   3,
};

const Uint8 atck10_sel_3[] = {
	 12,  11,   1,   1,   2,   6,   3,   8,  12,  11,   1,   1,   2,   6,   3,   8,
	 12,  11,   1,   1,   2,   6,   3,   9,   5,   5,  13,  13,  14,  14,  15,   9,
};

const Uint8 atck10_sel_4[] = {
	 11,  11,   1,   1,   3,   2,   8,   8,  11,  11,  14,  14,   3,   6,   8,  15,
	 11,  11,  13,  13,   5,  12,   9,  10,  11,  11,  13,  13,   5,  12,   9,  10,
};

const Uint8 atck10_sel_5[] = {
	 16,  11,   1,   1,   3,   2,   8,   8,  17,  11,  14,  14,  14,   6,   8,  15,
	 18,  11,  13,  13,   5,  12,   9,  10,  11,  11,  13,  13,   5,  12,   9,  10,
};

const Uint8 atck10_sel_6[] = {
	 16,  11,  13,   1,   3,   2,   8,   8,  16,  11,  13,   1,   3,   6,   8,   8,
	 17,  11,  13,   1,   5,  12,   9,  10,  18,  14,  14,  14,   5,  12,   9,  10,
};

const Uint8 atck10_sel_7[] = {
	 16,  11,  13,  13,   3,   2,   8,   8,  16,  11,  13,  13,   3,   6,   8,   8,
	 17,  11,  13,   5,   5,  12,   9,  10,  18,  14,  14,  14,  14,  12,   9,  10,
};

const Uint8 *const atck10_cmd[] = {
	atck10_cmd_00, atck10_cmd_01, atck10_cmd_02, atck10_cmd_03, atck10_cmd_04, atck10_cmd_05,
	atck10_cmd_06, atck10_cmd_07, atck10_cmd_08, atck10_cmd_09, atck10_cmd_10, atck10_cmd_11,
	atck10_cmd_12, atck10_cmd_13, atck10_cmd_14, atck10_cmd_15, atck10_cmd_16, atck10_cmd_17,
	atck10_cmd_18, atck10_cmd_19, atck10_cmd_20, atck10_cmd_21, atck10_cmd_22, atck10_cmd_23,
	atck10_cmd_24, atck10_cmd_25, atck10_cmd_26, atck10_cmd_27, atck10_cmd_28, atck10_cmd_29,
	atck10_cmd_30, atck10_cmd_31,
};

const Uint8 atck10_cmd_00[] = {
	STAND,
	WAIT2( 10 ),
	WALK_B( 512 ),
	WAIT2( 8 ),
	SP_CMD( pl10_sp_kobun, 0, PK_PUNCH, LV_HEAVY, 0 ),
	WAIT2( 14 ),
	SP_CMD( pl10_sp_beacon, 0, PK_PUNCH, LV_LIGHT, 0 ),
	IF_LEVEL( 24 ),
		IF_GAUGE( 3 ),
			IF_RND( 2 ),
				SP_CMD( pl10_sp_king, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				SP_CMD( pl10_sp_strike, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ENDIF,
		CMD_ELSE,
			SP_CMD( pl10_sp_strike, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ENDIF,
	CMD_ELSE,
		WAIT2( 14 ),
		SP_CMD( pl10_sp_strike, 0, PK_PUNCH, LV_HEAVY, 0 ),
		WAIT2( 14 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck10_cmd_01[] = {
	STAND,
	WAIT2( 8 ),
	CMD_JUMP( 12 ),
	JUMP( DIR_FORWARD ),
	JUMP_FLAG_SET,
		CMD_JUMP( 20 ),
		TIMER_SET( 90 ),
		IF_HEIGHT( 0xff, 112 ),
			CMD_JUMP( 9 ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	IF_LEVEL( 24 ),
		IF_EM_ATCK( ATCK_CROUCH_HP ),
			WAIT2( 4 ),
			CANCEL_SET2,
				CROUCH,
				PUNCH( LV_HEAVY ),
				SJUMP3( DIR_FORWARD ),
				SJUMP_FLAG_SET,
					CMD_JUMP( 4 ),
					JUMP_FLAG_CLR,
				CMD_ENDIF,
				CANCEL_CLR,
			CMD_ELSE,
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			CMD_JUMP( 8 ),
		CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 7 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 8 ),
	CMD_EXIT,
};

const Uint8 atck10_cmd_02[] = {
	STAND,
	WAIT2( 8 ),
	TIMER_SET( 60 ),
	WALK_EM( 64 ),
	CMD_JUMP( 11 ),
	STAND,
	WAIT2( 8 ),
	CMD_EXIT,
};

const Uint8 atck10_cmd_03[] = {
	STAND,
	CMD_JUMP( 0 ),
	WAIT2( 8 ),
	TIMER_SET( 60 ),
	WALK_ATCK( ATCK_CROUCH_HK ),
		CROUCH,
		WAIT2( 2 ),
		KICK( LV_HEAVY ),
	CMD_ENDIF,
	WAIT2( 8 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck10_cmd_04[] = {
	STAND,
	WAIT2( 14 ),
	TIMER_SET( 60 ),
	IF_EM_ATCK( ATCK_CROUCH_HP ),
		SP_CMD( pl10_sp_beacon, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ELSE,
		SP_CMD( pl10_sp_strike, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck10_cmd_05[] = {
	STAND,
	WAIT2( 4 ),
	DASH_ATCK( ATCK_CROUCH_LK ),
		CROUCH,
		CANCEL_SET2,
			KICK( LV_LIGHT ),
			IF_LEVEL( 24 ),
				IF_EM_ATCK( ATCK_CROUCH_HP ),
					CROUCH,
					PUNCH( LV_HEAVY ),
					SJUMP3( DIR_UP ),
					WAIT( 2 ),
					SJUMP_FLAG_SET,
						CMD_JUMP( 4 ),
						JUMP_FLAG_CLR,
					CMD_ENDIF,
				CMD_ELSE,
					IF_EM_ATCK( ATCK_CROUCH_HK ),
						CROUCH,
						KICK( LV_HEAVY ),
					CMD_ELSE,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ELSE,
				KICK( LV_HEAVY ),
			CMD_ENDIF,
			CANCEL_CLR,
		CMD_ELSE,
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		IF_RND( 16 ),
			CMD_JUMP( 54 ),
		CMD_ELSE,
			CMD_JUMP( 59 ),
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck10_cmd_06[] = {
	STAND,
	WAIT2( 4 ),
	CMD_JUMP( 0 ),
	IF_AREA( 64, 64, 128, 16 ),
		KICK( LV_HEAVY ),
		SP_CMD( pl10_sp_strike, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck10_cmd_07[] = {
	STAND,
	WAIT2( 4 ),
	SP_CMD( pl10_sp_kobun, 0, PK_PUNCH, LV_LIGHT, 0 ),
	STAND,
	WAIT2( 2 ),
	WALK_ATCK( ATCK_CROUCH_HK ),
		CROUCH,
		WAIT2( 2 ),
		KICK( LV_HEAVY ),
	CMD_ENDIF,
	STAND,
	CMD_EXIT,
};

const Uint8 atck10_cmd_08[] = {
	STAND,
	IF_GAUGE( 1 ),
		IF_RND( 16 ),
			WAIT2( 2 ),
			IF_AREA( 128, 64, 0, 64 ),
				SP_CMD( pl10_sp_lunch, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ELSE,
			CMD_JUMP( 54 ),
		CMD_ENDIF,
	CMD_ELSE,
		STAND,
		WAIT2( 2 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck10_cmd_09[] = {
	STAND,
	IF_GAUGE( 1 ),
		IF_RND( 16 ),
			IF_EM_ATCK( ATCK_STAND_LP ),
				SP_CMD( pl10_sp_king, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		STAND,
		WAIT2( 2 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck10_cmd_10[] = {
	STAND,
	WAIT2( 2 ),
	IF_RND( 16 ),
		IF_GAUGE( 2 ),
			IF_AREA( 128, 48, 128, 16 ),
				DASH_B( 64 ),
				STAND,
				SP_V_COMBI,
			CMD_ELSE,
				CMD_JUMP( 0 ),
			CMD_ENDIF,
			STAND,
			WAIT2( 2 ),
		CMD_ELSE,
			PARTNER_CALL,
		CMD_ENDIF,
	CMD_ELSE,
		WAIT( 1 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck10_cmd_11[] = {
	STAND,
	DASH_B( 96 ),
	STAND,
	WAIT2( 2 ),
	IF_AREA( 128, 64, 192, 16 ),
		SP_CMD( pl10_sp_beacon, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_JUMP( 24 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	IF_RND( 16 ),
		IF_RND( 10 ),
			DASH_B( 96 ),
			STAND,
		CMD_ENDIF,
		WAIT2( 2 ),
		IF_AREA( 128, 64, 192, 16 ),
			IF_LEVEL( 24 ),
				IF_GAUGE( 2 ),
					IF_RND( 10 ),
						SP_CMD( pl10_sp_king, 0, PK_PUNCH, LV_LIGHT, 0 ),
					CMD_ENDIF,
				CMD_ELSE,
					CMD_JUMP( 24 ),
					SP_CMD( pl10_sp_strike, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ENDIF,
			CMD_ELSE,
				CMD_JUMP( 24 ),
				SP_CMD( pl10_sp_strike, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck10_cmd_12[] = {
	STAND,
	WAIT2( 2 ),
	JUMP( DIR_FORWARD ),
	JUMP_FLAG_SET,
		TIMER_SET( 60 ),
		IF_HEIGHT( 0, 96 ),
			CMD_JUMP( 13 ),
			CMD_JUMP( 20 ),
			WAIT( 6 ),
			CMD_JUMP( 37 ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	WAIT2( 8 ),
	CANCEL_SET,
		IF_EM_ATCK( ATCK_STAND_LP ),
			STAND,
			PUNCH( LV_LIGHT ),
			IF_EM_ATCK( ATCK_STAND_MP ),
				STAND,
				PUNCH( LV_LIGHT ),
				IF_LEVEL( 24 ),
					WAIT( 10 ),
					SP_CMD( pl10_sp_strike, 0, PK_PUNCH, LV_HEAVY, 0 ),
				CMD_ENDIF,
			CMD_ELSE,
				CMD_JUMP( 21 ),
			CMD_ENDIF,
		CMD_ELSE,
			CMD_JUMP( 21 ),
		CMD_ENDIF,
		CANCEL_CLR,
	CMD_ENDIF,
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck10_cmd_13[] = {
	STAND,
	LOCK_SET,
	WAIT2( 6 ),
	CMD_JUMP( 0 ),
	CMD_JUMP( 25 ),
	CMD_JUMP( 61 ),
	JUMP_FLAG_SET,
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
		IF_HEIGHT( 1, 96 ),
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

const Uint8 atck10_cmd_14[] = {
	PARTNER_CALL,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck10_cmd_15[] = {
	STAND,
	WAIT2( 8 ),
	IF_GAUGE( 3 ),
		IF_EM_ATCK( pl10_at_snapback ),
			PARTNER_CHANGE,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck10_cmd_16[] = {
	CMD_JUMP( 54 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck10_cmd_17[] = {
	CMD_JUMP( 56 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck10_cmd_18[] = {
	CMD_JUMP( 53 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck10_cmd_19[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck10_cmd_20[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck10_cmd_21[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck10_cmd_22[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck10_cmd_23[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck10_cmd_24[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck10_cmd_25[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck10_cmd_26[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck10_cmd_27[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck10_cmd_28[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck10_cmd_29[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck10_cmd_30[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck10_cmd_31[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

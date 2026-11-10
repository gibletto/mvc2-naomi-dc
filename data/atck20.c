/* Felicia (character 20): the computer's attack lists.
   What it does once a waiting list's CMD_NEXT has sent it to attack. PLAYER_WORK's cpu_adrs[1]
   points here.

   atck20_tbl     one record (CPU_DATA): the offsets of the three tables.
   atck20_rate    the chance, out of 32, that a list's CMD_NEXT2 starts another attack list, by
                  the level of play.
   atck20_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   atck20_cmd     the lists: a pick is a number in this table.
   atck20_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                  list of lib20.c. */
#include "player.h"
#include "pl20_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const atck20_sel[], *const atck20_cmd[];
extern const Uint8 atck20_rate[], atck20_sel_0[], atck20_sel_1[], atck20_sel_2[], atck20_sel_3[],
	atck20_sel_4[], atck20_sel_5[], atck20_sel_6[], atck20_sel_7[], atck20_cmd_00[], atck20_cmd_01[],
	atck20_cmd_02[], atck20_cmd_03[], atck20_cmd_04[], atck20_cmd_05[], atck20_cmd_06[],
	atck20_cmd_07[], atck20_cmd_08[], atck20_cmd_09[], atck20_cmd_10[], atck20_cmd_11[],
	atck20_cmd_12[], atck20_cmd_13[], atck20_cmd_14[], atck20_cmd_15[], atck20_cmd_16[],
	atck20_cmd_17[], atck20_cmd_18[], atck20_cmd_19[], atck20_cmd_20[], atck20_cmd_21[],
	atck20_cmd_22[], atck20_cmd_23[], atck20_cmd_24[], atck20_cmd_25[], atck20_cmd_26[],
	atck20_cmd_27[], atck20_cmd_28[], atck20_cmd_29[], atck20_cmd_30[], atck20_cmd_31[];

const CPU_DATA atck20_tbl[] = {
	{ ( Sint32 )atck20_rate, ( Sint32 )atck20_sel, ( Sint32 )atck20_cmd },
};

const Uint8 atck20_rate[] = {
	  8,   8,   8,   8,   9,   9,  10,  10,  11,  11,  12,  12,  13,  13,  14,  14,
	 15,  15,  16,  16,  17,  17,  18,  18,  19,  20,  21,  22,  24,  26,  28,  30,
};

const Uint8 *const atck20_sel[] = {
	atck20_sel_0, atck20_sel_1, atck20_sel_2, atck20_sel_3, atck20_sel_4, atck20_sel_5,
	atck20_sel_6, atck20_sel_7,
};

const Uint8 atck20_sel_0[] = {
	  0,   0,   1,   1,   2,   2,   3,   3,   0,   0,   1,   1,   2,   2,   3,   3,
	  0,   0,   1,   1,   2,   2,   3,   3,   0,   0,   1,   1,   2,   2,   3,   3,
};

const Uint8 atck20_sel_1[] = {
	  0,   4,   4,   5,   5,   6,   6,   7,   0,   4,   4,   5,   5,   6,   6,   7,
	  0,   4,   4,   5,   5,   6,   6,   7,   0,   4,   4,   5,   5,   6,   6,   7,
};

const Uint8 atck20_sel_2[] = {
	 18,   7,   1,   1,   2,   6,   3,   3,   7,   7,   1,   1,   2,   6,   3,   3,
	  7,   7,   1,   1,   5,   6,   3,   3,   7,   7,   1,   1,   2,   6,   3,   3,
};

const Uint8 atck20_sel_3[] = {
	 12,  11,   1,   1,   2,   6,   3,   8,  12,  11,   1,   1,   2,   6,   3,   8,
	 12,  11,   1,   1,   2,   6,   3,   9,   5,   5,  13,  13,   2,   6,   3,   9,
};

const Uint8 atck20_sel_4[] = {
	 11,  11,   1,   1,   3,   2,   8,   8,  11,  11,   1,   1,   3,   6,   8,   8,
	 11,  11,  13,  13,   5,  12,   9,  10,  11,  11,  13,  13,   5,  12,   9,  10,
};

const Uint8 atck20_sel_5[] = {
	 15,  11,   1,   1,   3,   2,   8,   8,  15,  11,   1,  13,   3,   6,   8,   8,
	 15,  11,  13,  13,   5,  12,   9,  10,  11,  11,  13,  13,   5,  12,   9,  10,
};

const Uint8 atck20_sel_6[] = {
	 16,  11,   1,  20,  21,   2,   8,   8,  16,  11,  13,   1,  21,   6,   8,   8,
	 17,  11,  13,   1,   5,  12,   9,  10,  18,  11,  13,  20,   5,  12,   9,  10,
};

const Uint8 atck20_sel_7[] = {
	 16,  11,  13,  20,  21,   2,   8,   8,  16,  11,  13,  20,  21,   6,   8,   8,
	 17,  11,  13,  20,  21,  12,   9,  10,  18,  11,  13,  20,  21,  12,   9,  10,
};

const Uint8 *const atck20_cmd[] = {
	atck20_cmd_00, atck20_cmd_01, atck20_cmd_02, atck20_cmd_03, atck20_cmd_04, atck20_cmd_05,
	atck20_cmd_06, atck20_cmd_07, atck20_cmd_08, atck20_cmd_09, atck20_cmd_10, atck20_cmd_11,
	atck20_cmd_12, atck20_cmd_13, atck20_cmd_14, atck20_cmd_15, atck20_cmd_16, atck20_cmd_17,
	atck20_cmd_18, atck20_cmd_19, atck20_cmd_20, atck20_cmd_21, atck20_cmd_22, atck20_cmd_23,
	atck20_cmd_24, atck20_cmd_25, atck20_cmd_26, atck20_cmd_27, atck20_cmd_28, atck20_cmd_29,
	atck20_cmd_30, atck20_cmd_31,
};

const Uint8 atck20_cmd_00[] = {
	STAND,
	SP_CMD( pl20_sp_spike, 0, PK_PUNCH, LV_HEAVY, 0 ),
	WAIT2( 10 ),
	DASH_F( 64 ),
	WAIT2( 8 ),
	IF_RND( 8 ),
		PARTNER_CALL,
		SP_CMD( pl20_sp_delta, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ELSE,
		SP_CMD( pl20_sp_sand, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ENDIF,
	WAIT2( 14 ),
	IF_LEVEL( 24 ),
		IF_GAUGE( 3 ),
			IF_RND( 4 ),
				SP_CMD( pl20_sp_ssand, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		WAIT2( 14 ),
		PUNCH( LV_LIGHT ),
		PUNCH( LV_LIGHT ),
		SP_CMD( pl20_sp_sand, 0, PK_PUNCH, LV_LIGHT, 0 ),
		WAIT2( 14 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck20_cmd_01[] = {
	STAND,
	WAIT2( 8 ),
	CMD_JUMP( 12 ),
	JUMP_FLAG_SET,
		CMD_JUMP( 20 ),
		TIMER_SET( 60 ),
		IF_HEIGHT( 1, 128 ),
			CMD_JUMP( 9 ),
		CMD_ELSE,
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	IF_LEVEL( 24 ),
		IF_EM_ATCK( ATCK_STAND_HK ),
			WAIT2( 4 ),
			CANCEL_SET2,
				STAND,
				KICK( LV_HEAVY ),
				SJUMP3( DIR_UP ),
				CANCEL_CLR,
			CMD_ENDIF,
			SJUMP_FLAG_SET,
				CMD_JUMP( 4 ),
				JUMP_FLAG_CLR,
			CMD_ELSE,
				JUMP_FLAG_CLR,
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

const Uint8 atck20_cmd_02[] = {
	STAND,
	WAIT2( 8 ),
	TIMER_SET( 60 ),
	WALK_EM( 64 ),
	CMD_JUMP( 11 ),
	STAND,
	WAIT2( 8 ),
	CMD_EXIT,
};

const Uint8 atck20_cmd_03[] = {
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

const Uint8 atck20_cmd_04[] = {
	STAND,
	WAIT2( 14 ),
	TIMER_SET( 60 ),
	IF_EM_ATCK( ATCK_CROUCH_HK ),
		PARTNER_CALL,
		WAIT2( 4 ),
		SP_CMD( pl20_sp_spike, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ELSE,
		PARTNER_CALL,
		WAIT2( 4 ),
		SP_CMD( pl20_sp_spike, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ENDIF,
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck20_cmd_05[] = {
	STAND,
	WAIT2( 4 ),
	PARTNER_CALL,
	DASH_ATCK( ATCK_CROUCH_LK ),
		CROUCH,
		CANCEL_SET2,
			KICK( LV_LIGHT ),
			IF_LEVEL( 24 ),
				IF_EM_ATCK( ATCK_STAND_HK ),
					STAND,
					KICK( LV_HEAVY ),
					SJUMP3( DIR_UP ),
					WAIT( 2 ),
					SJUMP_FLAG_SET,
						CMD_JUMP( 4 ),
						JUMP_FLAG_CLR,
					CMD_ENDIF,
				CMD_ELSE,
					IF_EM_ATCK( ATCK_CROUCH_HK ),
						CROUCH,
						KICK( LV_LIGHT ),
						KICK( LV_HEAVY ),
						SP_CMD( pl20_sp_sand, 0, PK_PUNCH, LV_LIGHT, 0 ),
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

const Uint8 atck20_cmd_06[] = {
	STAND,
	WAIT2( 4 ),
	CMD_JUMP( 0 ),
	IF_AREA( 64, 64, 16, 16 ),
		WAIT2( 4 ),
		PARTNER_CALL,
		SP_CMD( pl20_sp_delta, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck20_cmd_07[] = {
	STAND,
	PARTNER_CALL,
	WAIT2( 4 ),
	SP_CMD( pl20_sp_delta, 0, PK_PUNCH, LV_HEAVY, 0 ),
	STAND,
	WAIT2( 2 ),
	IF_EM_ATCK( ATCK_CROUCH_HK ),
		CROUCH,
		KICK( LV_HEAVY ),
	CMD_ENDIF,
	STAND,
	CMD_EXIT,
};

const Uint8 atck20_cmd_08[] = {
	STAND,
	IF_GAUGE( 3 ),
		IF_RND( 16 ),
			WAIT2( 2 ),
			IF_AREA( 240, 48, 128, 16 ),
				SP_CMD( pl20_sp_ssand, 0, PK_PUNCH, LV_LIGHT, 0 ),
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

const Uint8 atck20_cmd_09[] = {
	STAND,
	IF_GAUGE( 2 ),
		IF_RND( 16 ),
			PARTNER_CALL,
			DASH_F( 64 ),
			SP_CMD( pl20_sp_delta, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ELSE,
		STAND,
		WAIT2( 2 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck20_cmd_10[] = {
	STAND,
	WAIT2( 2 ),
	IF_RND( 16 ),
		IF_GAUGE( 3 ),
			IF_AREA( 240, 48, 128, 16 ),
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
			SP_CMD( pl20_sp_delta, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ENDIF,
	CMD_ELSE,
		WAIT( 1 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck20_cmd_11[] = {
	STAND,
	DASH_EM( 64 ),
	STAND,
	WAIT2( 2 ),
	IF_AREA( 128, 64, 128, 16 ),
		PARTNER_CALL,
		SP_CMD( pl20_sp_delta, 0, PK_PUNCH, LV_HEAVY, 0 ),
		STAND,
		CMD_JUMP( 24 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	IF_RND( 16 ),
		IF_RND( 10 ),
			PARTNER_CALL,
			DASH_EM( 64 ),
			STAND,
		CMD_ENDIF,
		WAIT2( 2 ),
		IF_AREA( 128, 64, 128, 16 ),
			IF_LEVEL( 24 ),
				IF_GAUGE( 3 ),
					IF_RND( 10 ),
						SP_CMD( pl20_sp_ssand, 0, PK_PUNCH, LV_LIGHT, 0 ),
					CMD_ENDIF,
				CMD_ELSE,
					SP_CMD( pl20_sp_rolling, 0, PK_PUNCH, LV_HEAVY, 0 ),
					CMD_JUMP( 24 ),
				CMD_ENDIF,
			CMD_ELSE,
				SP_CMD( pl20_sp_rolling, 0, PK_PUNCH, LV_LIGHT, 0 ),
				STAND,
				CMD_JUMP( 24 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck20_cmd_12[] = {
	STAND,
	WAIT2( 2 ),
	PARTNER_CALL,
	JUMP( DIR_FORWARD ),
	WAIT( 4 ),
	JUMP_FLAG_SET,
		CMD_JUMP( 20 ),
		WAIT( 6 ),
		SP_CMD( pl20_sp_sand, 0, PK_PUNCH, LV_LIGHT, 0 ),
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	WAIT2( 8 ),
	CMD_JUMP( 21 ),
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck20_cmd_13[] = {
	STAND,
	LOCK_SET,
	WAIT2( 6 ),
	CMD_JUMP( 0 ),
	CMD_JUMP( 25 ),
	JUMP_FLAG_SET,
		CMD_JUMP( 20 ),
		WAIT( 10 ),
		IF_LEVEL( 10 ),
			IF_RND( 16 ),
				IF_RND( 12 ),
					JUMP_LEVEL( 38, 12, 39, 20, 40, 24, 41 ),
				CMD_ELSE,
					JUMP_LEVEL( 38, 14, 39, 22, 40, 26, 41 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
		TIMER_SET( 60 ),
		IF_HEIGHT( 1, 128 ),
			IF_RND( 12 ),
				JUMP_LEVEL( 35, 8, 35, 16, 9, 24, 9 ),
			CMD_ELSE,
				JUMP_LEVEL( 35, 10, 35, 18, 35, 26, 9 ),
			CMD_ENDIF,
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	CMD_JUMP( 7 ),
	LOCK_CLR,
	WAIT2( 4 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck20_cmd_14[] = {
	CMD_JUMP( 56 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck20_cmd_15[] = {
	CMD_JUMP( 57 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck20_cmd_16[] = {
	CMD_JUMP( 53 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck20_cmd_17[] = {
	CMD_JUMP( 54 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck20_cmd_18[] = {
	CMD_JUMP( 55 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck20_cmd_19[] = {
	CMD_JUMP( 56 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck20_cmd_20[] = {
	CMD_JUMP( 5 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck20_cmd_21[] = {
	CMD_JUMP( 6 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck20_cmd_22[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck20_cmd_23[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck20_cmd_24[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck20_cmd_25[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck20_cmd_26[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck20_cmd_27[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck20_cmd_28[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck20_cmd_29[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck20_cmd_30[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck20_cmd_31[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

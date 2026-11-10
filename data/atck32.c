/* Colossus (character 32): the computer's attack lists.
   What it does once a waiting list's CMD_NEXT has sent it to attack. PLAYER_WORK's cpu_adrs[1]
   points here.

   atck32_tbl     one record (CPU_DATA): the offsets of the three tables.
   atck32_rate    the chance, out of 32, that a list's CMD_NEXT2 starts another attack list, by
                  the level of play.
   atck32_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   atck32_cmd     the lists: a pick is a number in this table.
   atck32_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                  list of lib32.c. */
#include "player.h"
#include "pl32_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const atck32_sel[], *const atck32_cmd[];
extern const Uint8 atck32_rate[], atck32_sel_0[], atck32_sel_1[], atck32_sel_2[], atck32_sel_3[],
	atck32_sel_4[], atck32_sel_5[], atck32_sel_6[], atck32_sel_7[], atck32_cmd_00[], atck32_cmd_01[],
	atck32_cmd_02[], atck32_cmd_03[], atck32_cmd_04[], atck32_cmd_05[], atck32_cmd_06[],
	atck32_cmd_07[], atck32_cmd_08[], atck32_cmd_09[], atck32_cmd_10[], atck32_cmd_11[],
	atck32_cmd_12[], atck32_cmd_13[], atck32_cmd_14[], atck32_cmd_15[], atck32_cmd_16[],
	atck32_cmd_17[], atck32_cmd_18[], atck32_cmd_19[], atck32_cmd_20[], atck32_cmd_21[],
	atck32_cmd_22[], atck32_cmd_23[], atck32_cmd_24[], atck32_cmd_25[], atck32_cmd_26[],
	atck32_cmd_27[], atck32_cmd_28[], atck32_cmd_29[], atck32_cmd_30[], atck32_cmd_31[];

const CPU_DATA atck32_tbl[] = {
	{ ( Sint32 )atck32_rate, ( Sint32 )atck32_sel, ( Sint32 )atck32_cmd },
};

const Uint8 atck32_rate[] = {
	  8,   8,   8,   8,   9,   9,  10,  10,  21,  21,  22,  22,  23,  23,  24,  24,
	 15,  15,  16,  16,  17,  17,  18,  18,  19,  20,  21,  22,  24,  26,  28,  30,
};

const Uint8 *const atck32_sel[] = {
	atck32_sel_0, atck32_sel_1, atck32_sel_2, atck32_sel_3, atck32_sel_4, atck32_sel_5,
	atck32_sel_6, atck32_sel_7,
};

const Uint8 atck32_sel_0[] = {
	  5,   0,   4,   0,   1,   2,   3,  15,   0,   0,  20,   0,   1,   2,  20,   3,
	 20,  15,   4,  19,  20,  14,   3,   3,   6,   6,   7,   7,   1,  14,  15,   5,
};

const Uint8 atck32_sel_1[] = {
	 17,   2,   0,  14,   1,   1,   3,   4,   0,   2,   0,  15,   6,   1,   3,  20,
	  0,  20,  19,   0,   1,  15,   3,   4,   6,   7,   7,   0,   1,   1,   3,   5,
};

const Uint8 atck32_sel_2[] = {
	 18,   7,   1,   1,   2,   6,   3,   3,   7,   7,   1,  15,   2,  15,  19,  20,
	  7,  15,  21,   1,   5,   6,   4,   4,   7,   7,  20,   3,   2,   6,   5,   5,
};

const Uint8 atck32_sel_3[] = {
	 12,  11,   1,   1,   2,   6,   3,   8,  12,  11,  14,   1,   2,  15,  20,  20,
	 12,   7,  21,   4,  19,  15,   5,   9,   5,   5,  20,  13,   5,   6,   3,   9,
};

const Uint8 atck32_sel_4[] = {
	 11,  11,   1,   1,   3,   2,   8,  21,  11,  20,   1,   1,  19,   6,   5,   8,
	 11,  12,  15,  13,   5,  12,   9,  10,   2,   7,  13,  20,   5,  12,   9,  10,
};

const Uint8 atck32_sel_5[] = {
	 16,  11,   1,   1,   3,   2,   8,   8,  17,   5,  20,  14,  20,   6,   5,   8,
	 18,   5,  13,  13,   5,  12,   9,  10,  11,  11,  15,  19,   5,  12,   9,  10,
};

const Uint8 atck32_sel_6[] = {
	 16,  11,   1,  19,   3,   2,   8,   8,  12,   5,  13,   1,  20,   6,  11,   1,
	 17,  21,  15,   5,  21,  12,   9,  10,  18,  11,  13,  21,  20,  14,   9,  10,
};

const Uint8 atck32_sel_7[] = {
	 16,  11,  13,  14,   3,   2,   8,   5,  12,  12,  19,  13,   1,   6,  20,  10,
	 17,   1,   1,  21,   5,  12,   9,  10,  18,  11,  15,   5,  20,  12,   9,  21,
};

const Uint8 *const atck32_cmd[] = {
	atck32_cmd_00, atck32_cmd_01, atck32_cmd_02, atck32_cmd_03, atck32_cmd_04, atck32_cmd_05,
	atck32_cmd_06, atck32_cmd_07, atck32_cmd_08, atck32_cmd_09, atck32_cmd_10, atck32_cmd_11,
	atck32_cmd_12, atck32_cmd_13, atck32_cmd_14, atck32_cmd_15, atck32_cmd_16, atck32_cmd_17,
	atck32_cmd_18, atck32_cmd_19, atck32_cmd_20, atck32_cmd_21, atck32_cmd_22, atck32_cmd_23,
	atck32_cmd_24, atck32_cmd_25, atck32_cmd_26, atck32_cmd_27, atck32_cmd_28, atck32_cmd_29,
	atck32_cmd_30, atck32_cmd_31,
};

const Uint8 atck32_cmd_00[] = {
	STAND,
	LOCK_SET,
	WAIT2( 10 ),
	SP_CMD( pl32_sp_swing, 0, 2, LV_LIGHT, 0 ),
	WAIT2( 8 ),
	SP_CMD( pl32_sp_tackle, 0, PK_PUNCH, LV_HEAVY, 0 ),
	IF_RND( 16 ),
		IF_GAUGE( 1 ),
			SP_CMD( pl32_sp_armor, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	IF_EM_ATCK( pl32_at_swing ),
		SP_CMD( pl32_sp_swing, 0, 2, LV_HEAVY, 0 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 atck32_cmd_01[] = {
	STAND,
	CMD_JUMP( 14 ),
	JUMP_FLAG_SET,
		CMD_JUMP( 20 ),
		TIMER_SET( 60 ),
		IF_HEIGHT( 1, 128 ),
			CMD_JUMP( 9 ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	CMD_JUMP( 5 ),
	STAND,
	WAIT2( 4 ),
	CMD_EXIT,
};

const Uint8 atck32_cmd_02[] = {
	STAND,
	TIMER_SET( 60 ),
	CMD_JUMP( 11 ),
	IF_LEVEL( 8 ),
		CMD_JUMP( 8 ),
	CMD_ELSE,
		CMD_JUMP( 6 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 4 ),
	CMD_EXIT,
};

const Uint8 atck32_cmd_03[] = {
	STAND,
	CMD_JUMP( 0 ),
	TIMER_SET( 60 ),
	WALK_ATCK( ATCK_CROUCH_HK ),
		CROUCH,
		WAIT2( 2 ),
		KICK( LV_HEAVY ),
	CMD_ENDIF,
	WAIT2( 8 ),
	CMD_JUMP( 6 ),
	WAIT2( 4 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck32_cmd_04[] = {
	STAND,
	IF_LEVEL( 8 ),
		CMD_JUMP( 5 ),
	CMD_ENDIF,
	SP_CMD( pl32_sp_tackle, 0, PK_PUNCH, LV_HEAVY, 0 ),
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck32_cmd_05[] = {
	STAND,
	WAIT2( 4 ),
	DASH_EM( 16 ),
	IF_LEVEL( 24 ),
		IF_EM_ATCK( ATCK_STAND_HK ),
			CANCEL_SET2,
				KICK( LV_HEAVY ),
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
			IF_EM_ATCK( ATCK_STAND_HK ),
				CANCEL_SET2,
					KICK( LV_HEAVY ),
					SJUMP3( DIR_FORWARD ),
					CANCEL_CLR,
				CMD_ENDIF,
				SJUMP_FLAG_SET,
					TIMER_SET( 120 ),
					IF_EM_ATCK( ATCK_JUMP_LP ),
						CANCEL_SET2,
							PUNCH( LV_LIGHT ),
							PUNCH( LV_HEAVY ),
							CANCEL_CLR,
						CMD_ENDIF,
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
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck32_cmd_06[] = {
	STAND,
	CMD_JUMP( 0 ),
	IF_AREA( 64, 64, 16, 16 ),
		SP_CMD( pl32_sp_swing, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck32_cmd_07[] = {
	STAND,
	IF_LEVEL( 8 ),
		IF_GAUGE( 1 ),
			SP_CMD( pl32_sp_armor, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	IF_EM_ATCK( ATCK_STAND_HP ),
		PUNCH( LV_HEAVY ),
	CMD_ENDIF,
	WAIT2( 4 ),
	STAND,
	CMD_EXIT,
};

const Uint8 atck32_cmd_08[] = {
	STAND,
	IF_GAUGE( 1 ),
		IF_RND( 16 ),
			WAIT2( 2 ),
			IF_AREA( 240, 48, 128, 16 ),
				SP_CMD( pl32_sp_dive, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ELSE,
			CMD_JUMP( 54 ),
		CMD_ENDIF,
	CMD_ELSE,
	CMD_ENDIF,
	CMD_JUMP( 0 ),
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck32_cmd_09[] = {
	STAND,
	IF_GAUGE( 1 ),
		IF_RND( 16 ),
			SP_CMD( pl32_sp_armor, 0, PK_PUNCH, LV_LIGHT, 0 ),
			SP_CMD( pl32_sp_tackle, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			CMD_JUMP( 27 ),
		CMD_ENDIF,
	CMD_ELSE,
		STAND,
		WAIT2( 2 ),
	CMD_ENDIF,
	CMD_JUMP( 0 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck32_cmd_10[] = {
	STAND,
	WAIT2( 2 ),
	IF_RND( 16 ),
		IF_GAUGE( 2 ),
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
		CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 0 ),
		WAIT( 1 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck32_cmd_11[] = {
	STAND,
	DASH_B( 96 ),
	STAND,
	IF_AREA( 128, 64, 128, 16 ),
		STAND,
		CMD_JUMP( 24 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	IF_RND( 16 ),
		IF_RND( 10 ),
			DASH_F( 96 ),
			STAND,
		CMD_ENDIF,
		IF_AREA( 128, 64, 128, 16 ),
			IF_LEVEL( 24 ),
				IF_GAUGE( 1 ),
					IF_RND( 10 ),
						SP_CMD( pl32_sp_armor, 0, PK_PUNCH, LV_LIGHT, 0 ),
						SP_CMD( pl32_sp_tackle, 0, PK_PUNCH, LV_LIGHT, 0 ),
					CMD_ENDIF,
				CMD_ELSE,
					SP_CMD( pl32_sp_tackle, 0, PK_PUNCH, LV_HEAVY, 0 ),
					CMD_JUMP( 24 ),
				CMD_ENDIF,
			CMD_ELSE,
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

const Uint8 atck32_cmd_12[] = {
	STAND,
	CMD_JUMP( 14 ),
	JUMP( DIR_FORWARD ),
	WAIT( 4 ),
	JUMP_FLAG_SET,
		CMD_JUMP( 20 ),
		WAIT( 6 ),
		SP_CMD( pl32_sp_tackle, 0, PK_PUNCH, LV_HEAVY, 0 ),
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	WAIT2( 4 ),
	CMD_JUMP( 21 ),
	CMD_JUMP( 0 ),
	IF_LEVEL( 16 ),
		WALK_EM( 48 ),
		IF_EM_ATCK( ATCK_STAND_LK ),
			CANCEL_SET2,
				KICK( LV_LIGHT ),
				KICK( LV_LIGHT ),
				SP_CMD( pl32_sp_tackle, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck32_cmd_13[] = {
	STAND,
	LOCK_SET,
	CMD_JUMP( 0 ),
	CMD_JUMP( 25 ),
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

const Uint8 atck32_cmd_14[] = {
	WALK_F( 64 ),
	PUNCH( LV_HEAVY ),
	WALK_F( 80 ),
	PUNCH( LV_HEAVY ),
	WAIT2( 4 ),
	IF_RND( 16 ),
		SP_CMD( pl32_sp_tackle, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ELSE,
		CMD_JUMP( 28 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck32_cmd_15[] = {
	LOCK_SET,
	CMD_JUMP( 23 ),
	IF_LEVEL( 10 ),
		IF_GAUGE( 1 ),
			SP_CMD( pl32_sp_armor, 0, PK_PUNCH, LV_LIGHT, 0 ),
			SP_CMD( pl32_sp_tackle, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	WAIT2( 4 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck32_cmd_16[] = {
	CMD_JUMP( 54 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck32_cmd_17[] = {
	CMD_JUMP( 56 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck32_cmd_18[] = {
	CMD_JUMP( 53 ),
	CMD_JUMP( 28 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck32_cmd_19[] = {
	DASH_EM( 16 ),
	IF_EM_ATCK( ATCK_STAND_HK ),
		CANCEL_SET2,
			KICK( LV_HEAVY ),
			SP_CMD( pl32_sp_tackle, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	WAIT2( 4 ),
	CMD_JUMP( 5 ),
	STAND,
	CMD_JUMP( 28 ),
	STAND,
	CMD_EXIT,
};

const Uint8 atck32_cmd_20[] = {
	STAND,
	JUMP( DIR_FORWARD ),
	SP_CMD( pl32_sp_tackle, 0, PK_PUNCH, LV_HEAVY, 0 ),
	STAND,
	WAIT2( 2 ),
	IF_LEVEL( 10 ),
		IF_RND( 16 ),
			JUMP( DIR_FORWARD ),
			JUMP_FLAG_SET,
				IF_RND( 16 ),
					TIMER_SET( 90 ),
					IF_EM_ATCK( ATCK_JUMP_HP ),
						PUNCH( LV_HEAVY ),
					CMD_ENDIF,
				CMD_ELSE,
					TIMER_SET( 90 ),
					IF_EM_ATCK( pl32_at_jfwd_hp ),
						NAG_VFLIP_ADJ3( LV_HEAVY, LEVER_FORWARD ),
					CMD_ENDIF,
				CMD_ENDIF,
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
		IF_EM_ATCK( ATCK_CROUCH_HP ),
			CROUCH,
			PUNCH( LV_HEAVY ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck32_cmd_21[] = {
	STAND,
	WALK_DIST( 48 ),
	IF_EM_ATCK( ATCK_CROUCH_LK ),
		CANCEL_SET2,
			CROUCH,
			KICK( LV_LIGHT ),
			KICK( LV_LIGHT ),
			SP_CMD_SET( pl32_sp_dive, 0, PK_PUNCH, LV_LIGHT, 0, 1, 30 ),
			SP_CMD( pl32_sp_tackle, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_JUMP( 6 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck32_cmd_22[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck32_cmd_23[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck32_cmd_24[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck32_cmd_25[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck32_cmd_26[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck32_cmd_27[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck32_cmd_28[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck32_cmd_29[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck32_cmd_30[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck32_cmd_31[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

/* B.B. Hood (character 1f): the computer's attack lists.
   What it does once a waiting list's CMD_NEXT has sent it to attack. PLAYER_WORK's cpu_adrs[1]
   points here.

   atck1f_tbl     one record (CPU_DATA): the offsets of the three tables.
   atck1f_rate    the chance, out of 32, that a list's CMD_NEXT2 starts another attack list, by
                  the level of play.
   atck1f_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   atck1f_cmd     the lists: a pick is a number in this table.
   atck1f_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                  list of lib1f.c. */
#include "player.h"
#include "pl1f_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const atck1f_sel[], *const atck1f_cmd[];
extern const Uint8 atck1f_rate[], atck1f_sel_0[], atck1f_sel_1[], atck1f_sel_2[], atck1f_sel_3[],
	atck1f_sel_4[], atck1f_sel_5[], atck1f_sel_6[], atck1f_sel_7[], atck1f_cmd_00[], atck1f_cmd_01[],
	atck1f_cmd_02[], atck1f_cmd_03[], atck1f_cmd_04[], atck1f_cmd_05[], atck1f_cmd_06[],
	atck1f_cmd_07[], atck1f_cmd_08[], atck1f_cmd_09[], atck1f_cmd_10[], atck1f_cmd_11[],
	atck1f_cmd_12[], atck1f_cmd_13[], atck1f_cmd_14[], atck1f_cmd_15[], atck1f_cmd_16[],
	atck1f_cmd_18[], atck1f_cmd_17[], atck1f_cmd_19[], atck1f_cmd_20[], atck1f_cmd_21[],
	atck1f_cmd_22[], atck1f_cmd_23[], atck1f_cmd_24[], atck1f_cmd_25[], atck1f_cmd_26[],
	atck1f_cmd_27[], atck1f_cmd_28[], atck1f_cmd_29[], atck1f_cmd_30[], atck1f_cmd_31[];

const CPU_DATA atck1f_tbl[] = {
	{ ( Sint32 )atck1f_rate, ( Sint32 )atck1f_sel, ( Sint32 )atck1f_cmd },
};

const Uint8 atck1f_rate[] = {
	  8,   8,   8,   8,   9,   9,  10,  10,  11,  11,  12,  12,  13,  13,  14,  14,
	 15,  15,  16,  16,  17,  17,  18,  18,  19,  20,  21,  22,  24,  26,  28,  30,
};

const Uint8 *const atck1f_sel[] = {
	atck1f_sel_0, atck1f_sel_1, atck1f_sel_2, atck1f_sel_3, atck1f_sel_4, atck1f_sel_5,
	atck1f_sel_6, atck1f_sel_7,
};

const Uint8 atck1f_sel_0[] = {
	 16,   0,   5,   0,   1,   2,   3,   3,  10,   0,   5,   0,   1,   2,   3,   3,
	 11,   0,   5,   0,   1,   2,   3,   3,   6,   6,   7,   7,   1,   2,   3,   3,
};

const Uint8 atck1f_sel_1[] = {
	 17,   0,   5,   0,   1,   1,   3,   4,  10,   0,   5,   0,   1,   1,   3,   4,
	 11,   0,   5,   0,   1,   1,   3,   4,  12,   0,   5,   0,   1,   1,   3,   4,
};

const Uint8 atck1f_sel_2[] = {
	 18,   7,   1,   1,   2,   6,   3,   3,   7,   7,   1,   1,   2,   6,   3,   3,
	  7,   7,   1,   1,   5,   5,   5,   5,   7,   7,   1,   1,   2,   5,   3,   3,
};

const Uint8 atck1f_sel_3[] = {
	 12,  11,   1,   1,   2,   6,   3,   8,  12,  11,   1,   1,   2,   6,   3,   8,
	 12,  11,   1,   1,   2,   5,   3,   9,   5,   5,  13,  13,   2,   5,   3,   9,
};

const Uint8 atck1f_sel_4[] = {
	 11,  11,   1,   1,   3,   2,   8,   8,  11,  11,   1,   1,   3,   6,   8,   8,
	 11,  11,  13,  13,   5,  12,   9,  10,  11,  11,  13,  13,   5,  12,   9,  10,
};

const Uint8 atck1f_sel_5[] = {
	 16,   5,   1,   1,   3,   2,   8,   8,  17,   5,   1,  13,   3,   6,   8,   8,
	 18,  11,  13,  13,   5,  12,   9,  10,  11,  11,  13,  13,   5,  12,   9,  10,
};

const Uint8 atck1f_sel_6[] = {
	 16,   5,   1,   5,   3,   2,   8,   7,  16,  11,  13,   1,   3,   6,   8,   7,
	 17,  11,  13,   1,   5,  12,   9,  10,  18,  11,  13,   5,   5,  12,   9,  10,
};

const Uint8 atck1f_sel_7[] = {
	 16,  11,  13,  10,   3,   2,   8,   7,  16,  11,  13,  10,   3,   6,   8,   7,
	 17,   5,   5,   5,   5,  12,   9,  10,  18,   5,   5,   5,   5,  12,   9,  10,
};

const Uint8 *const atck1f_cmd[] = {
	atck1f_cmd_00, atck1f_cmd_01, atck1f_cmd_02, atck1f_cmd_03, atck1f_cmd_04, atck1f_cmd_05,
	atck1f_cmd_06, atck1f_cmd_07, atck1f_cmd_08, atck1f_cmd_09, atck1f_cmd_10, atck1f_cmd_11,
	atck1f_cmd_12, atck1f_cmd_13, atck1f_cmd_14, atck1f_cmd_15, atck1f_cmd_16, atck1f_cmd_17,
	atck1f_cmd_18, atck1f_cmd_19, atck1f_cmd_20, atck1f_cmd_21, atck1f_cmd_22, atck1f_cmd_23,
	atck1f_cmd_24, atck1f_cmd_25, atck1f_cmd_26, atck1f_cmd_27, atck1f_cmd_28, atck1f_cmd_29,
	atck1f_cmd_30, atck1f_cmd_31,
};

const Uint8 atck1f_cmd_00[] = {
	STAND,
	WAIT2( 8 ),
	CMD_JUMP( 5 ),
	WAIT2( 8 ),
	CMD_JUMP( 17 ),
	WAIT2( 8 ),
	CMD_EXIT,
};

const Uint8 atck1f_cmd_01[] = {
	WAIT2( 8 ),
	CMD_JUMP( 14 ),
	JUMP_FLAG_SET,
		CMD_JUMP( 20 ),
		CMD_JUMP( 5 ),
		TIMER_SET( 120 ),
		IF_HEIGHT( 0xff, 80 ),
			CMD_JUMP( 20 ),
			TIMER_SET( 60 ),
			IF_AREA( 64, 128, 64, 16 ),
				SP_CMD( pl1f_sp_cheer_k, 0, PK_PUNCH, LV_HEAVY, 0 ),
				WAIT2( 4 ),
				AIR_JUMP( DIR_BACK ),
			CMD_ELSE,
				CMD_JUMP( 66 ),
			CMD_ENDIF,
			WAIT2( 10 ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	TIMER_SET( 60 ),
	IF_DIST( 64, 8, 192, 1 ),
		NAG_VFLIP_ADJ3( LV_HEAVY, LEVER_FORWARD ),
	CMD_ENDIF,
	STAND,
	WAIT2( 8 ),
	CMD_EXIT,
};

const Uint8 atck1f_cmd_02[] = {
	STAND,
	WAIT2( 8 ),
	CMD_JUMP( 5 ),
	WAIT2( 8 ),
	TIMER_SET( 60 ),
	DASH_ATCK( ATCK_STAND_LP ),
		JUMP_RND( 6, 7, 8, 9 ),
		SJUMP_FLAG_SET,
			CMD_JUMP( 37 ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 17 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 8 ),
	CMD_EXIT,
};

const Uint8 atck1f_cmd_03[] = {
	STAND,
	WAIT2( 8 ),
	TIMER_SET( 60 ),
	WALK_EM( 48 ),
	CMD_JUMP( 72 ),
	IF_LEVEL( 16 ),
		IF_RND( 16 ),
			CMD_JUMP( 11 ),
		CMD_ELSE,
			CMD_JUMP( 5 ),
			DASH_ATCK( ATCK_STAND_LP ),
				JUMP_RND( 6, 7, 8, 9 ),
				SJUMP_FLAG_SET,
					CMD_JUMP( 37 ),
					JUMP_FLAG_CLR,
				CMD_ENDIF,
			CMD_ELSE,
				CMD_JUMP( 24 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 11 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 8 ),
	CMD_EXIT,
};

const Uint8 atck1f_cmd_04[] = {
	STAND,
	WAIT2( 10 ),
	TIMER_SET( 60 ),
	IF_DIST( 64, 32, 192, 32 ),
		TIMER_SET( 60 ),
		IF_EM_ATCK( ATCK_STAND_HP ),
			SP_CMD( pl1f_sp_missile_j, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			SP_CMD( pl1f_sp_shyness, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 60 ),
		IF_RND( 16 ),
			JUMP_LEVEL( 61, 8, 63, 16, 9, 24, 64 ),
		CMD_ELSE,
			JUMP_LEVEL( 62, 8, 7, 16, 64, 24, 8 ),
		CMD_ENDIF,
		IF_LEVEL( 24 ),
			SJUMP_FLAG_SET,
				CMD_JUMP( 37 ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	WAIT2( 8 ),
	CMD_JUMP( 0 ),
	CMD_EXIT,
};

const Uint8 atck1f_cmd_05[] = {
	STAND,
	WAIT2( 4 ),
	DASH_ATCK( ATCK_STAND_LK ),
		CMD_JUMP( 8 ),
		SJUMP_FLAG_SET,
			CMD_JUMP( 37 ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 5 ),
		JUMP( DIR_BACK ),
		JUMP_FLAG_SET,
			WAIT( 24 ),
			AIR_JUMP( DIR_FORWARD ),
			TIMER_SET( 30 ),
			IF_AREA( 96, 48, 48, 8 ),
				SP_CMD( pl1f_sp_cheer_k, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ELSE,
				TIMER_SET( 30 ),
				IF_AREA( 32, 32, 1, -16 ),
					KICK( LV_HEAVY ),
				CMD_ENDIF,
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	WAIT2( 10 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1f_cmd_06[] = {
	STAND,
	WAIT2( 4 ),
	CMD_JUMP( 0 ),
	TIMER_SET( 60 ),
	IF_AREA( 64, 64, 16, 16 ),
		IF_LEVEL( 24 ),
			DASH_ATCK( ATCK_STAND_LP ),
				IF_RND( 20 ),
					CANCEL_SET2,
						PUNCH( LV_LIGHT ),
						PUNCH( LV_LIGHT ),
						PARTNER_CHANGE,
						CANCEL_CLR,
					CMD_ENDIF,
				CMD_ELSE,
					PUNCH( LV_LIGHT ),
					PUNCH( LV_LIGHT ),
					SP_CMD( pl1f_sp_shyness, 0, PK_PUNCH, LV_HEAVY, 0 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 16 ),
				CMD_JUMP( 5 ),
				TIMER_SET( 60 ),
				DASH_ATCK( ATCK_STAND_LP ),
					CMD_JUMP( 6 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_RND( 20 ),
					PARTNER_CHANGE,
				CMD_ELSE,
					SP_CMD( pl1f_sp_shyness, 0, PK_PUNCH, LV_HEAVY, 0 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		WAIT2( 10 ),
		IF_GAUGE( 3 ),
			SP_CMD( pl1f_sp_apple, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			CROUCH,
			WAIT2( 8 ),
			SP_CMD( pl1f_sp_missile, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	CMD_JUMP( 0 ),
	CMD_EXIT,
};

const Uint8 atck1f_cmd_07[] = {
	STAND,
	WAIT2( 4 ),
	CMD_JUMP( 5 ),
	WALK_B( 128 ),
	CROUCH,
	WAIT2( 0 ),
	TIMER_SET( 60 ),
	IF_AREA( 64, 48, 112, 16 ),
		SP_CMD( pl1f_sp_happy, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_JUMP( 24 ),
	CMD_ELSE,
		SP_CMD( pl1f_sp_happy, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_JUMP( 14 ),
		JUMP_FLAG_SET,
			CMD_JUMP( 20 ),
			TIMER_SET( 120 ),
			IF_HEIGHT( 0xff, 80 ),
				IF_DIST( 32, 128, 32, 1 ),
					CMD_JUMP( 33 ),
				CMD_ENDIF,
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
		IF_DIST( 64, 64, 0, 0 ),
			DASH_F( 24 ),
			CMD_JUMP( 8 ),
			SJUMP_FLAG_SET,
				CMD_JUMP( 37 ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			IF_DIST( 64, 64, 128, 0 ),
				IF_RND( 12 ),
					DASH_F( 64 ),
					NAG_VFLIP_ADJ3( LV_HEAVY, LEVER_FORWARD ),
				CMD_ELSE,
					IF_RND( 16 ),
						NAG_VFLIP_ADJ3( LV_HEAVY, LEVER_FORWARD ),
					CMD_ELSE,
						CMD_JUMP( 5 ),
						DASH_B( 128 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT2( 8 ),
	CMD_EXIT,
};

const Uint8 atck1f_cmd_08[] = {
	STAND,
	WAIT2( 10 ),
	IF_GAUGE( 3 ),
		CMD_JUMP( 5 ),
		IF_RND( 16 ),
			WAIT2( 2 ),
			TIMER_SET( 60 ),
			IF_AREA( 64, 48, 192, 16 ),
				SP_CMD( pl1f_sp_hunting, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ELSE,
			CMD_JUMP( 54 ),
		CMD_ENDIF,
	CMD_ELSE,
		STAND,
		WAIT2( 2 ),
		DASH_ATCK( ATCK_STAND_LP ),
			CMD_JUMP( 14 ),
			JUMP_FLAG_SET,
				CMD_JUMP( 5 ),
				CMD_JUMP( 20 ),
				TIMER_SET( 60 ),
				IF_HEIGHT( 1, 128 ),
					CMD_JUMP( 33 ),
				CMD_ENDIF,
				JUMP_FLAG_CLR,
			CMD_ENDIF,
			CMD_JUMP( 8 ),
			SJUMP_FLAG_SET,
				JUMP_RND( 25, 26, 27, 28 ),
				JUMP_FLAG_CLR,
			CMD_ELSE,
				CMD_JUMP( 5 ),
				WALK_B( 512 ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT2( 4 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1f_cmd_09[] = {
	STAND,
	WAIT2( 10 ),
	IF_GAUGE( 3 ),
		IF_RND( 16 ),
			IF_EM_ATCK( pl1f_at_apple ),
				SP_CMD( pl1f_sp_apple, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		STAND,
		WAIT2( 2 ),
		DASH_ATCK( ATCK_STAND_HP ),
			CMD_JUMP( 5 ),
			CMD_JUMP( 14 ),
			JUMP_FLAG_SET,
				CMD_JUMP( 20 ),
				TIMER_SET( 60 ),
				IF_HEIGHT( 1, 128 ),
					CMD_JUMP( 33 ),
				CMD_ENDIF,
				JUMP_FLAG_CLR,
			CMD_ENDIF,
			CMD_JUMP( 8 ),
			SJUMP_FLAG_SET,
				CMD_JUMP( 37 ),
				JUMP_FLAG_CLR,
			CMD_ELSE,
				CMD_JUMP( 5 ),
				WALK_B( 192 ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT2( 4 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1f_cmd_10[] = {
	STAND,
	WAIT2( 10 ),
	CMD_JUMP( 14 ),
	LOCK_SET,
	CMD_JUMP( 5 ),
	JUMP_FLAG_SET,
		CMD_JUMP( 20 ),
		TIMER_SET( 120 ),
		IF_HEIGHT( 0xff, 80 ),
			IF_DIST( 32, 128, 32, 1 ),
				CMD_JUMP( 33 ),
			CMD_ELSE,
				IF_RND( 12 ),
					CMD_JUMP( 20 ),
					AIR_JUMP( DIR_FORWARD ),
				CMD_ELSE,
					CMD_JUMP( 33 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	JUMP_FLAG_SET,
		TIMER_SET( 120 ),
		IF_HEIGHT( 0xff, 64 ),
			IF_DIST( 32, 128, 32, 1 ),
				CMD_JUMP( 33 ),
			CMD_ENDIF,
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	LOCK_CLR,
	IF_DIST( 64, 64, 0, 0 ),
		DASH_F( 24 ),
		CMD_JUMP( 8 ),
		SJUMP_FLAG_SET,
			CMD_JUMP( 37 ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		IF_DIST( 64, 64, 128, 0 ),
			CMD_JUMP( 5 ),
			DASH_B( 128 ),
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT2( 10 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1f_cmd_11[] = {
	STAND,
	WAIT2( 10 ),
	CMD_JUMP( 5 ),
	DASH_B( 96 ),
	STAND,
	WAIT2( 2 ),
	TIMER_SET( 60 ),
	IF_AREA( 128, 64, 128, 16 ),
		SP_CMD( pl1f_sp_happy, 0, PK_PUNCH, LV_HEAVY, 0 ),
		STAND,
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
		TIMER_SET( 60 ),
		IF_AREA( 128, 64, 128, 16 ),
			IF_LEVEL( 24 ),
				IF_GAUGE( 2 ),
					IF_RND( 10 ),
						SP_CMD( pl1f_sp_apple, 0, PK_PUNCH, LV_LIGHT, 0 ),
					CMD_ENDIF,
				CMD_ELSE,
					CROUCH,
					WAIT2( 14 ),
					SP_CMD( pl1f_sp_happy, 0, PK_PUNCH, LV_HEAVY, 0 ),
					CMD_JUMP( 24 ),
				CMD_ENDIF,
			CMD_ELSE,
				SP_CMD( pl1f_sp_missile, 0, PK_PUNCH, LV_HEAVY, 0 ),
				STAND,
				CMD_JUMP( 24 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 20 ),
		CMD_JUMP( 38 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	CMD_JUMP( 0 ),
	CMD_EXIT,
};

const Uint8 atck1f_cmd_12[] = {
	WAIT2( 10 ),
	CMD_JUMP( 5 ),
	IF_RND( 16 ),
		DASH_B( 128 ),
		CMD_JUMP( 17 ),
		CMD_JUMP( 60 ),
		CMD_JUMP( 8 ),
		SJUMP_FLAG_SET,
			CMD_JUMP( 37 ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 60 ),
		CMD_JUMP( 55 ),
	CMD_ENDIF,
	CMD_JUMP( 0 ),
	CMD_EXIT,
};

const Uint8 atck1f_cmd_13[] = {
	STAND,
	WAIT2( 10 ),
	CMD_JUMP( 5 ),
	CMD_JUMP( 20 ),
	DASH_B( 192 ),
	WAIT2( 2 ),
	CMD_JUMP( 10 ),
	CMD_JUMP( 17 ),
	CMD_JUMP( 60 ),
	CMD_JUMP( 8 ),
	SJUMP_FLAG_SET,
		CMD_JUMP( 37 ),
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	CMD_NEXT2,
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 atck1f_cmd_14[] = {
	WAIT2( 10 ),
	CMD_JUMP( 20 ),
	TIMER_SET( 60 ),
	IF_DIST( 64, 8, 64, 1 ),
		CMD_JUMP( 5 ),
		CMD_JUMP( 60 ),
		CMD_JUMP( 56 ),
	CMD_ELSE,
		CMD_JUMP( 54 ),
		CMD_JUMP( 5 ),
	CMD_ENDIF,
	WAIT2( 10 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1f_cmd_15[] = {
	WAIT2( 10 ),
	CMD_JUMP( 20 ),
	CMD_JUMP( 5 ),
	CMD_JUMP( 60 ),
	CMD_JUMP( 57 ),
	CMD_JUMP( 0 ),
	WAIT2( 10 ),
	CMD_EXIT,
};

const Uint8 atck1f_cmd_16[] = {
	WAIT2( 10 ),
	CMD_JUMP( 14 ),
	CMD_JUMP( 20 ),
	CMD_JUMP( 58 ),
	CMD_JUMP( 0 ),
	CMD_EXIT,
};

const Uint8 atck1f_cmd_18[] = {
	WAIT2( 10 ),
	CMD_JUMP( 60 ),
};

const Uint8 atck1f_cmd_17[] = {
	WAIT2( 10 ),
	CMD_JUMP( 20 ),
	CMD_JUMP( 59 ),
	CMD_JUMP( 0 ),
	WAIT2( 10 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1f_cmd_19[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1f_cmd_20[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1f_cmd_21[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1f_cmd_22[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1f_cmd_23[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1f_cmd_24[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1f_cmd_25[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1f_cmd_26[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1f_cmd_27[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1f_cmd_28[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1f_cmd_29[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1f_cmd_30[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1f_cmd_31[] = {
	CMD_EXIT,
};

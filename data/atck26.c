/* M. Bison (character 26): the computer's attack lists.
   What it does once a waiting list's CMD_NEXT has sent it to attack. PLAYER_WORK's cpu_adrs[1]
   points here.

   atck26_tbl     one record (CPU_DATA): the offsets of the three tables.
   atck26_rate    the chance, out of 32, that a list's CMD_NEXT2 starts another attack list, by
                  the level of play.
   atck26_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   atck26_cmd     the lists: a pick is a number in this table.
   atck26_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                  list of lib26.c. */
#include "player.h"
#include "pl26_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const atck26_sel[], *const atck26_cmd[];
extern const Uint8 atck26_rate[], atck26_sel_0[], atck26_sel_1[], atck26_sel_2[], atck26_sel_3[],
	atck26_sel_4[], atck26_sel_5[], atck26_sel_6[], atck26_sel_7[], atck26_cmd_00[], atck26_cmd_01[],
	atck26_cmd_02[], atck26_cmd_03[], atck26_cmd_04[], atck26_cmd_05[], atck26_cmd_06[],
	atck26_cmd_07[], atck26_cmd_08[], atck26_cmd_09[], atck26_cmd_10[], atck26_cmd_11[],
	atck26_cmd_12[], atck26_cmd_13[], atck26_cmd_14[], atck26_cmd_15[], atck26_cmd_16[],
	atck26_cmd_17[], atck26_cmd_18[], atck26_cmd_19[], atck26_cmd_20[], atck26_cmd_21[],
	atck26_cmd_22[], atck26_cmd_23[], atck26_cmd_24[], atck26_cmd_25[], atck26_cmd_26[],
	atck26_cmd_27[], atck26_cmd_28[], atck26_cmd_29[], atck26_cmd_30[], atck26_cmd_31[];

const CPU_DATA atck26_tbl[] = {
	{ ( Sint32 )atck26_rate, ( Sint32 )atck26_sel, ( Sint32 )atck26_cmd },
};

const Uint8 atck26_rate[] = {
	  8,   8,   8,   8,   9,   9,  10,  10,  11,  11,  12,  12,  13,  13,  14,  14,
	 15,  15,  16,  16,  17,  17,  18,  18,  19,  20,  21,  22,  24,  26,  28,  30,
};

const Uint8 *const atck26_sel[] = {
	atck26_sel_0, atck26_sel_1, atck26_sel_2, atck26_sel_3, atck26_sel_4, atck26_sel_5,
	atck26_sel_6, atck26_sel_7,
};

const Uint8 atck26_sel_0[] = {
	  0,   0,   0,   0,   0,   0,   3,   3,   0,   0,   0,   0,   0,   0,   3,   3,
	  6,   6,   6,   6,   6,   6,   3,   3,  11,   6,   6,   6,   6,   6,   3,   3,
};

const Uint8 atck26_sel_1[] = {
	  0,   0,   0,   0,   1,   1,   3,   4,   6,   6,   6,   6,   1,   1,   3,   4,
	  6,   6,  15,  15,   1,   1,   3,   4,  11,   0,   0,  16,   1,   1,   3,   4,
};

const Uint8 atck26_sel_2[] = {
	  6,   7,   1,   1,   2,   6,   3,   3,   7,   7,   1,   1,   2,   6,   3,   3,
	  7,   7,   1,   1,   2,   6,   3,   3,  11,  16,  15,   1,   2,   6,  15,  16,
};

const Uint8 atck26_sel_3[] = {
	  7,   7,   1,   1,  28,   6,   3,   8,  12,  14,   1,   1,  28,   6,   3,   8,
	 12,  11,  17,  17,   2,  17,  15,   9,  12,  11,  13,  15,   2,  17,  15,   9,
};

const Uint8 atck26_sel_4[] = {
	  7,   7,  16,   1,  28,   2,   8,  17,  14,  14,  16,   1,  28,   6,   8,  17,
	 11,  11,  13,  13,   5,  12,   9,  10,  16,  16,  15,  15,   5,  12,   9,  10,
};

const Uint8 atck26_sel_5[] = {
	  7,   7,   1,  16,  28,   2,   8,  17,  14,  14,   1,  16,  28,   6,   8,  17,
	 11,  11,  15,  13,   5,  12,   9,  10,  16,  16,  15,  13,   5,  12,   9,  10,
};

const Uint8 atck26_sel_6[] = {
	  7,   7,   1,  13,  28,   2,   8,   8,  14,  14,  28,  13,  28,   6,  16,  16,
	 11,  11,  15,  16,   5,  12,   9,  10,  17,  17,  15,  16,   5,  12,   9,  10,
};

const Uint8 atck26_sel_7[] = {
	  7,   7,  13,  28,  28,   2,  17,   8,  14,  14,  13,  28,  28,   6,  16,   8,
	 11,  11,  15,  13,   5,  12,   9,  10,  16,  17,  15,  16,   5,  12,   9,  10,
};

const Uint8 *const atck26_cmd[] = {
	atck26_cmd_00, atck26_cmd_01, atck26_cmd_02, atck26_cmd_03, atck26_cmd_04, atck26_cmd_05,
	atck26_cmd_06, atck26_cmd_07, atck26_cmd_08, atck26_cmd_09, atck26_cmd_10, atck26_cmd_11,
	atck26_cmd_12, atck26_cmd_13, atck26_cmd_14, atck26_cmd_15, atck26_cmd_16, atck26_cmd_17,
	atck26_cmd_18, atck26_cmd_19, atck26_cmd_20, atck26_cmd_21, atck26_cmd_22, atck26_cmd_23,
	atck26_cmd_24, atck26_cmd_25, atck26_cmd_26, atck26_cmd_27, atck26_cmd_28, atck26_cmd_29,
	atck26_cmd_30, atck26_cmd_31,
};

const Uint8 atck26_cmd_00[] = {
	STAND,
	WAIT2( 10 ),
	IF_RND( 18 ),
		SP_CMD( pl26_sp_head, 0, PK_KICK, LV_LIGHT, 0 ),
	CMD_ELSE,
		SP_CMD( pl26_sp_head, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ENDIF,
	IF_AREA( 64, 64, 32, 0 ),
		CROUCH,
		WAIT2( 2 ),
		KICK( LV_HEAVY ),
		STAND,
		WAIT2( 4 ),
	CMD_ENDIF,
	STAND,
	CMD_EXIT,
};

const Uint8 atck26_cmd_01[] = {
	STAND,
	WAIT2( 8 ),
	CMD_JUMP( 14 ),
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
		IF_EM_ATCK( ATCK_STAND_HP ),
			WAIT2( 4 ),
			CANCEL_SET2,
				STAND,
				PUNCH( LV_HEAVY ),
				SJUMP3( DIR_UP ),
				SJUMP_FLAG_SET,
					CMD_JUMP( 4 ),
					JUMP_FLAG_CLR,
				CMD_ELSE,
					JUMP_FLAG_CLR,
				CMD_ENDIF,
				CANCEL_CLR,
			CMD_ELSE,
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			CMD_JUMP( 7 ),
		CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 7 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 8 ),
	CMD_EXIT,
};

const Uint8 atck26_cmd_02[] = {
	STAND,
	WAIT2( 8 ),
	TIMER_SET( 60 ),
	WALK_EM( 128 ),
	CMD_JUMP( 11 ),
	STAND,
	WAIT2( 8 ),
	CMD_EXIT,
};

const Uint8 atck26_cmd_03[] = {
	STAND,
	CMD_JUMP( 0 ),
	WAIT2( 8 ),
	TIMER_SET( 60 ),
	WALK_DIST( 128 ),
	IF_AREA( 64, 64, 32, 0 ),
		CROUCH,
		WAIT2( 2 ),
		KICK( LV_HEAVY ),
		STAND,
		WAIT2( 4 ),
	CMD_ENDIF,
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck26_cmd_04[] = {
	STAND,
	WAIT2( 4 ),
	CMD_JUMP( 24 ),
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck26_cmd_05[] = {
	STAND,
	WAIT2( 4 ),
	DASH_EM( 32 ),
	CROUCH,
	CANCEL_SET2,
		KICK( LV_LIGHT ),
		IF_LEVEL( 24 ),
			IF_EM_ATCK( ATCK_STAND_HP ),
				STAND,
				PUNCH( LV_HEAVY ),
				SJUMP3( DIR_UP ),
				WAIT( 2 ),
				SJUMP_FLAG_SET,
					CMD_JUMP( 4 ),
					JUMP_FLAG_CLR,
				CMD_ENDIF,
			CMD_ELSE,
				IF_AREA( 64, 64, 32, 0 ),
					CROUCH,
					WAIT2( 2 ),
					KICK( LV_HEAVY ),
					STAND,
					WAIT2( 4 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			KICK( LV_HEAVY ),
		CMD_ENDIF,
		CANCEL_CLR,
	CMD_ELSE,
		PUNCH( LV_LIGHT ),
		CANCEL_CLR,
	CMD_ENDIF,
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck26_cmd_06[] = {
	STAND,
	WAIT2( 4 ),
	CMD_JUMP( 0 ),
	TIMER_SET( 60 ),
	WALK_EM( 208 ),
	IF_AREA( 48, 64, 32, 16 ),
		WAIT2( 4 ),
		SP_CMD( pl26_sp_knee, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ELSE,
		IF_AREA( 80, 64, 64, 16 ),
			WAIT2( 4 ),
			SP_CMD( pl26_sp_knee, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ELSE,
			IF_AREA( 112, 64, 96, 16 ),
				WAIT2( 4 ),
				SP_CMD( pl26_sp_knee, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck26_cmd_07[] = {
	STAND,
	WAIT2( 4 ),
	CMD_JUMP( 17 ),
	STAND,
	CMD_EXIT,
};

const Uint8 atck26_cmd_08[] = {
	STAND,
	IF_GAUGE( 1 ),
		IF_RND( 16 ),
			WAIT2( 2 ),
			IF_AREA( 240, 48, 128, 16 ),
				CMD_JUMP( 10 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		STAND,
		WAIT2( 2 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck26_cmd_09[] = {
	STAND,
	IF_GAUGE( 1 ),
		IF_RND( 16 ),
			IF_AREA( 240, 48, 128, 16 ),
				CMD_JUMP( 21 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		STAND,
		WAIT2( 2 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck26_cmd_10[] = {
	STAND,
	WAIT2( 2 ),
	IF_RND( 16 ),
		IF_GAUGE( 2 ),
			IF_AREA( 240, 48, 128, 16 ),
				DASH_B( 64 ),
				STAND,
				SP_V_COMBI,
			CMD_ENDIF,
			STAND,
			WAIT2( 2 ),
		CMD_ENDIF,
	CMD_ELSE,
		WAIT( 1 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck26_cmd_11[] = {
	STAND,
	WAIT2( 2 ),
	IF_DIST( 64, 240, 64, 0 ),
		JUMP( DIR_BACK ),
		JUMP_FLAG_SET,
			CMD_JUMP( 20 ),
			TIMER_SET( 60 ),
			IF_HEIGHT( 1, 128 ),
				SP_CMD( pl26_sp_knee, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		JUMP( DIR_UP ),
		JUMP_FLAG_SET,
			CMD_JUMP( 20 ),
			TIMER_SET( 60 ),
			IF_HEIGHT( 1, 128 ),
				SP_CMD( pl26_sp_knee, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck26_cmd_12[] = {
	STAND,
	WAIT2( 2 ),
	SJUMP( DIR_UP ),
	SJUMP_FLAG_SET,
		CMD_JUMP( 20 ),
		WAIT( 50 ),
		CMD_JUMP( 17 ),
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck26_cmd_13[] = {
	STAND,
	LOCK_SET,
	WAIT2( 6 ),
	CMD_JUMP( 0 ),
	CMD_JUMP( 25 ),
	CMD_DUMMY,
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

const Uint8 atck26_cmd_14[] = {
	STAND,
	WAIT2( 2 ),
	DASH_B( 512 ),
	IF_DIST( 96, 240, 0, 0 ),
	CMD_ELSE,
		IF_RND( 12 ),
			SP_CMD( pl26_sp_field, 0, PK_PUNCH, LV_HEAVY, 0 ),
			IF_RND( 22 ),
				SP_CMD( pl26_sp_field, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				SP_CMD( pl26_sp_field, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ENDIF,
		CMD_ELSE,
		CMD_ENDIF,
		STAND,
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck26_cmd_15[] = {
	STAND,
	CMD_JUMP( 0 ),
	LOCK_SET,
	IF_LEVEL( 7 ),
		WAIT2( 4 ),
		SP_CMD( pl26_sp_fly, 0, PK_PUNCH, LV_LIGHT, 0 ),
		FLY_FLAG_SET,
			IF_RND( 10 ),
				TIMER_SET( 60 ),
				WALK_POS_Y( 224, 128 ),
				WAIT2( 4 ),
				SP_CMD( pl26_sp_knee, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ELSE,
				TIMER_SET( 40 ),
				WALK_POS_Y( 16, 32 ),
				CANCEL_SET2,
					IF_EM_ATCK( ATCK_JUMP_MK ),
						KICK( LV_LIGHT ),
					CMD_ENDIF,
					IF_EM_ATCK( ATCK_JUMP_HK ),
						KICK( LV_HEAVY ),
					CMD_ENDIF,
					IF_LEVEL( 13 ),
						IF_RND( 20 ),
							SP_CMD( pl26_sp_knee, 0, PK_PUNCH, LV_HEAVY, 0 ),
						CMD_ENDIF,
					CMD_ENDIF,
					CANCEL_CLR,
				CMD_ELSE,
					SP_CMD( pl26_sp_head, 0, PK_KICK, LV_LIGHT, 0 ),
					CANCEL_CLR,
				CMD_ENDIF,
			CMD_ENDIF,
			FLY_FLAG_CLR,
		CMD_HIENDIF,
		STAND,
		WAIT2( 2 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck26_cmd_16[] = {
	DASH_ATCK( ATCK_CROUCH_LP ),
		IF_EM_ATCK( ATCK_CROUCH_LP ),
			IF_RND( 16 ),
				CANCEL_SET,
					CROUCH,
					PUNCH( LV_LIGHT ),
					KICK( LV_LIGHT ),
					KICK( LV_HEAVY ),
					CANCEL_CLR,
				CMD_ENDIF,
			CMD_ELSE,
				CMD_JUMP( 54 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck26_cmd_17[] = {
	IF_DIST( 64, 8, 32, 0 ),
		IF_RND( 24 ),
			CMD_JUMP( 54 ),
		CMD_ELSE,
			CROUCH,
			KICK( LV_HEAVY ),
		CMD_ENDIF,
	CMD_ELSE,
		DASH_EM( 64 ),
		CMD_JUMP( 54 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck26_cmd_18[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck26_cmd_19[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck26_cmd_20[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck26_cmd_21[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck26_cmd_22[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck26_cmd_23[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck26_cmd_24[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck26_cmd_25[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck26_cmd_26[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck26_cmd_27[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck26_cmd_28[] = {
	CMD_JUMP( 55 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck26_cmd_29[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck26_cmd_30[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck26_cmd_31[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

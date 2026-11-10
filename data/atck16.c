/* Marrow (character 16): the computer's attack lists.
   What it does once a waiting list's CMD_NEXT has sent it to attack. PLAYER_WORK's cpu_adrs[1]
   points here.

   atck16_tbl     one record (CPU_DATA): the offsets of the three tables.
   atck16_rate    the chance, out of 32, that a list's CMD_NEXT2 starts another attack list, by
                  the level of play.
   atck16_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   atck16_cmd     the lists: a pick is a number in this table.
   atck16_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                  list of lib16.c. */
#include "player.h"
#include "pl16_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const atck16_sel[], *const atck16_cmd[];
extern const Uint8 atck16_rate[], atck16_sel_0[], atck16_sel_1[], atck16_sel_2[], atck16_sel_3[],
	atck16_sel_4[], atck16_sel_5[], atck16_sel_6[], atck16_sel_7[], atck16_cmd_00[], atck16_cmd_01[],
	atck16_cmd_02[], atck16_cmd_03[], atck16_cmd_04[], atck16_cmd_05[], atck16_cmd_06[],
	atck16_cmd_07[], atck16_cmd_08[], atck16_cmd_09[], atck16_cmd_10[], atck16_cmd_11[],
	atck16_cmd_12[], atck16_cmd_13[], atck16_cmd_14[], atck16_cmd_15[], atck16_cmd_16[],
	atck16_cmd_17[], atck16_cmd_18[], atck16_cmd_19[], atck16_cmd_20[], atck16_cmd_21[],
	atck16_cmd_22[], atck16_cmd_23[], atck16_cmd_24[], atck16_cmd_25[], atck16_cmd_26[],
	atck16_cmd_27[], atck16_cmd_28[], atck16_cmd_31[], atck16_cmd_30[], atck16_cmd_29[];

const CPU_DATA atck16_tbl[] = {
	{ ( Sint32 )atck16_rate, ( Sint32 )atck16_sel, ( Sint32 )atck16_cmd },
};

const Uint8 atck16_rate[] = {
	  8,   8,   8,   8,   9,   9,  10,  10,  11,  11,  12,  12,  13,  13,  14,  14,
	 15,  15,  16,  16,  17,  17,  18,  18,  19,  20,  21,  22,  24,  26,  28,  30,
};

const Uint8 *const atck16_sel[] = {
	atck16_sel_0, atck16_sel_1, atck16_sel_2, atck16_sel_3, atck16_sel_4, atck16_sel_5,
	atck16_sel_6, atck16_sel_7,
};

const Uint8 atck16_sel_0[] = {
	  0,   0,   0,   0,   1,   1,   1,   1,   0,   0,   0,   0,   1,   1,   3,   3,
	  2,   2,   0,   0,   1,   1,   3,   3,   4,   4,   5,   5,   1,   1,   3,   3,
};

const Uint8 atck16_sel_1[] = {
	  0,   0,   0,   0,   0,   1,   1,   1,   0,   4,   4,   5,   1,   1,   1,   1,
	  6,   0,  15,  15,   3,   3,   3,   3,   6,   6,   0,   7,   7,   7,   3,   3,
};

const Uint8 atck16_sel_2[] = {
	  0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   6,   6,   6,   6,   3,   3,
	  3,   3,   5,   5,  14,  14,  14,  14,  15,  15,  15,  19,  21,  22,  23,  24,
};

const Uint8 atck16_sel_3[] = {
	  0,   0,   0,   0,   1,   1,   1,   1,   2,   3,   3,   3,   6,   6,   7,   7,
	 20,  20,  20,  18,  18,  18,  21,  21,  19,  19,  19,  22,  23,  24,  10,  10,
};

const Uint8 atck16_sel_4[] = {
	  0,   0,   0,   1,   1,   1,   3,   3,   3,   7,   7,   8,   8,   8,   4,   4,
	 14,  14,  14,  15,  15,  15,  21,  21,  22,  22,  23,  23,  24,  24,  10,  10,
};

const Uint8 atck16_sel_5[] = {
	  1,   4,   4,  22,   6,   6,  24,  15,   1,   7,   7,   8,  14,  14,   8,  15,
	 18,  18,  19,  19,  20,  20,  21,  21,  22,  22,  23,  23,  24,  24,  10,  10,
};

const Uint8 atck16_sel_6[] = {
	 14,  14,  15,  15,   2,   2,   3,   3,   3,  20,  20,  18,  18,   8,  21,  21,
	 19,   8,  22,  22,   4,  23,  23,  23,   7,  24,  24,  24,  10,  10,  15,  15,
};

const Uint8 atck16_sel_7[] = {
	  2,   2,  21,  20,  20,   3,   8,   8,  14,  14,  14,  19,  19,   3,   3,   3,
	 22,  22,  22,  22,  23,  23,  23,  23,  24,  24,  24,  24,  10,  10,  21,  21,
};

const Uint8 *const atck16_cmd[] = {
	atck16_cmd_00, atck16_cmd_01, atck16_cmd_02, atck16_cmd_03, atck16_cmd_04, atck16_cmd_05,
	atck16_cmd_06, atck16_cmd_07, atck16_cmd_08, atck16_cmd_09, atck16_cmd_10, atck16_cmd_11,
	atck16_cmd_12, atck16_cmd_13, atck16_cmd_14, atck16_cmd_15, atck16_cmd_16, atck16_cmd_17,
	atck16_cmd_18, atck16_cmd_19, atck16_cmd_20, atck16_cmd_21, atck16_cmd_22, atck16_cmd_23,
	atck16_cmd_24, atck16_cmd_25, atck16_cmd_26, atck16_cmd_27, atck16_cmd_28, atck16_cmd_29,
	atck16_cmd_30, atck16_cmd_31,
};

const Uint8 atck16_cmd_00[] = {
	IF_LEVEL( 16 ),
		CMD_JUMP( 58 ),
	CMD_ELSE,
		IF_LEVEL( 8 ),
		CMD_ELSE,
			WALK_F( 96 ),
			WALK_B( 128 ),
			WALK_F( 96 ),
			CMD_JUMP( 26 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck16_cmd_01[] = {
	IF_LEVEL( 18 ),
		STAND,
		WAIT2( 6 ),
		DASH_ATCK( ATCK_CROUCH_LK ),
			CROUCH,
			CANCEL_SET,
				KICK( LV_LIGHT ),
				IF_EM_ATCK( ATCK_CROUCH_HP ),
					CROUCH,
					PUNCH( LV_HEAVY ),
					SJUMP3( DIR_FORWARD ),
					WAIT( 2 ),
					CMD_JUMP( 4 ),
				CMD_ENDIF,
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		JUMP( DIR_BACK ),
		WAIT2( 4 ),
		WALK_F( 192 ),
		CMD_JUMP( 27 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck16_cmd_02[] = {
	CMD_JUMP( 10 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck16_cmd_03[] = {
	STAND,
	CMD_JUMP( 0 ),
	WAIT2( 12 ),
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

const Uint8 atck16_cmd_04[] = {
	CMD_JUMP( 37 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck16_cmd_05[] = {
	IF_LEVEL( 8 ),
	CMD_ELSE,
		JUMP( DIR_FORWARD ),
		WALK_B( 48 ),
		WALK_F( 80 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 4 ),
	CMD_JUMP( 0 ),
	CMD_JUMP( 24 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck16_cmd_06[] = {
	IF_LEVEL( 8 ),
	CMD_ELSE,
		JUMP( DIR_UP ),
		WALK_B( 192 ),
		JUMP( DIR_FORWARD ),
	CMD_ENDIF,
	STAND,
	WAIT2( 4 ),
	CMD_JUMP( 0 ),
	IF_AREA( 64, 64, 16, 16 ),
		WAIT2( 4 ),
		SP_CMD( pl16_sp_bone, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck16_cmd_07[] = {
	CMD_JUMP( 68 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck16_cmd_08[] = {
	IF_DIST( 64, 8, 32, 0 ),
		IF_RND( 24 ),
			CMD_JUMP( 69 ),
		CMD_ELSE,
			CROUCH,
			KICK( LV_HEAVY ),
		CMD_ENDIF,
	CMD_ELSE,
		DASH_EM( 64 ),
		CMD_JUMP( 69 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck16_cmd_09[] = {
	STAND,
	IF_GAUGE( 1 ),
		IF_RND( 12 ),
			IF_AREA( 96, 16, 96, 16 ),
				SP_CMD( pl16_sp_stinger, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		STAND,
		WAIT2( 2 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck16_cmd_10[] = {
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
		WAIT( 1 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck16_cmd_11[] = {
	STAND,
	DASH_B( 96 ),
	STAND,
	WAIT2( 2 ),
	IF_AREA( 128, 64, 128, 16 ),
		SP_CMD( pl16_sp_bone, 0, PK_PUNCH, LV_HEAVY, 0 ),
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
		IF_AREA( 128, 64, 128, 16 ),
			IF_LEVEL( 24 ),
				IF_GAUGE( 2 ),
					IF_RND( 10 ),
						SP_CMD( pl16_sp_burst, 0, PK_PUNCH, LV_LIGHT, 0 ),
					CMD_ENDIF,
				CMD_ELSE,
					SP_CMD( pl16_sp_bone, 0, PK_PUNCH, LV_HEAVY, 0 ),
					CMD_JUMP( 24 ),
				CMD_ENDIF,
			CMD_ELSE,
				SP_CMD( pl16_sp_bone, 0, PK_PUNCH, LV_HEAVY, 0 ),
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

const Uint8 atck16_cmd_12[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck16_cmd_13[] = {
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

const Uint8 atck16_cmd_14[] = {
	IF_LEVEL( 16 ),
		CMD_JUMP( 60 ),
	CMD_ELSE,
		IF_LEVEL( 8 ),
			CMD_JUMP( 5 ),
		CMD_ELSE,
			CMD_JUMP( 30 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck16_cmd_15[] = {
	CMD_JUMP( 58 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck16_cmd_16[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck16_cmd_17[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck16_cmd_18[] = {
	CMD_JUMP( 53 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck16_cmd_19[] = {
	IF_LEVEL( 20 ),
		CMD_JUMP( 8 ),
	CMD_ELSE,
		IF_LEVEL( 8 ),
			IF_RND( 12 ),
				CMD_JUMP( 62 ),
			CMD_ELSE,
				CMD_JUMP( 63 ),
			CMD_ENDIF,
		CMD_ELSE,
			CMD_JUMP( 7 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck16_cmd_20[] = {
	IF_LEVEL( 8 ),
		CMD_JUMP( 6 ),
	CMD_ELSE,
		CMD_JUMP( 37 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck16_cmd_21[] = {
	CMD_JUMP( 61 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck16_cmd_22[] = {
	IF_LEVEL( 8 ),
		IF_AREA( 80, 1, 80, 1 ),
			CMD_JUMP( 66 ),
		CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 5 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck16_cmd_23[] = {
	IF_LEVEL( 22 ),
		DASH_ATCK( ATCK_CROUCH_LK ),
			CROUCH,
			CANCEL_SET,
				KICK( LV_LIGHT ),
				IF_EM_ATCK( ATCK_CROUCH_HP ),
					CROUCH,
					PUNCH( LV_HEAVY ),
					SJUMP3( DIR_FORWARD ),
					WAIT( 14 ),
					TIMER_SET( 0xa0 ),
					IF_EM_ATCK( ATCK_JUMP_LP ),
						CANCEL_SET,
							PUNCH( LV_LIGHT ),
							KICK( LV_LIGHT ),
							WAIT( 1 ),
							PUNCH( LV_LIGHT ),
							KICK( LV_HEAVY ),
							CANCEL_CLR,
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ELSE,
					IF_EM_ATCK( ATCK_CROUCH_HK ),
						CROUCH,
						KICK( LV_HEAVY ),
					CMD_ELSE,
					CMD_ENDIF,
				CMD_ENDIF,
				CANCEL_CLR,
			CMD_ELSE,
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ELSE,
		CMD_ENDIF,
		STAND,
	CMD_ELSE,
		IF_LEVEL( 6 ),
			CMD_JUMP( 67 ),
		CMD_ELSE,
			CMD_JUMP( 27 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck16_cmd_24[] = {
	IF_LEVEL( 6 ),
		STAND,
		WAIT2( 6 ),
		DASH_ATCK( ATCK_CROUCH_LK ),
			CROUCH,
			CANCEL_SET,
				KICK( LV_LIGHT ),
				IF_EM_ATCK( ATCK_CROUCH_HP ),
					CROUCH,
					PUNCH( LV_HEAVY ),
					SJUMP3( DIR_FORWARD ),
					WAIT( 2 ),
					CMD_JUMP( 4 ),
				CMD_ELSE,
					IF_EM_ATCK( ATCK_CROUCH_HK ),
						CROUCH,
						KICK( LV_HEAVY ),
					CMD_ELSE,
					CMD_ENDIF,
				CMD_ENDIF,
				CANCEL_CLR,
			CMD_ELSE,
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ELSE,
		CMD_ENDIF,
		STAND,
	CMD_ELSE,
		STAND,
		WAIT2( 4 ),
		WALK_EM( 8 ),
		CROUCH,
		CANCEL_SET,
			IF_EM_ATCK( ATCK_CROUCH_HP ),
				CROUCH,
				PUNCH( LV_HEAVY ),
				SJUMP3( DIR_FORWARD ),
				WAIT( 4 ),
				CMD_JUMP( 4 ),
			CMD_ELSE,
				IF_EM_ATCK( ATCK_CROUCH_HK ),
					CROUCH,
					KICK( LV_HEAVY ),
				CMD_ENDIF,
			CMD_ENDIF,
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck16_cmd_25[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck16_cmd_26[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck16_cmd_27[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck16_cmd_28[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck16_cmd_31[] = {
	CMD_JUMP( 58 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck16_cmd_30[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck16_cmd_29[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

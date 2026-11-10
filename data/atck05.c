/* Strider Hiryu (character 05): the computer's attack lists.
   What it does once a waiting list's CMD_NEXT has sent it to attack. PLAYER_WORK's cpu_adrs[1]
   points here.

   atck05_tbl     one record (CPU_DATA): the offsets of the three tables.
   atck05_rate    the chance, out of 32, that a list's CMD_NEXT2 starts another attack list, by
                  the level of play.
   atck05_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   atck05_cmd     the lists: a pick is a number in this table.
   atck05_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                  list of lib05.c. */
#include "player.h"
#include "pl05_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const atck05_sel[], *const atck05_cmd[];
extern const Uint8 atck05_rate[], atck05_sel_0[], atck05_sel_1[], atck05_sel_2[], atck05_sel_3[],
	atck05_sel_4[], atck05_sel_5[], atck05_sel_6[], atck05_sel_7[], atck05_cmd_00[], atck05_cmd_01[],
	atck05_cmd_02[], atck05_cmd_03[], atck05_cmd_04[], atck05_cmd_05[], atck05_cmd_06[],
	atck05_cmd_07[], atck05_cmd_08[], atck05_cmd_09[], atck05_cmd_10[], atck05_cmd_11[],
	atck05_cmd_12[], atck05_cmd_13[], atck05_cmd_14[], atck05_cmd_15[];

const CPU_DATA atck05_tbl[] = {
	{ ( Sint32 )atck05_rate, ( Sint32 )atck05_sel, ( Sint32 )atck05_cmd },
};

const Uint8 atck05_rate[] = {
	 13,  14,  15,  16,  16,  17,  17,  18,  18,  19,  20,  20,  22,  23,  24,  25,
	 26,  27,  28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 *const atck05_sel[] = {
	atck05_sel_0, atck05_sel_1, atck05_sel_2, atck05_sel_3, atck05_sel_4, atck05_sel_5,
	atck05_sel_6, atck05_sel_7,
};

const Uint8 atck05_sel_0[] = {
	  5,   5,   5,   5,   4,   4,   4,   4,   5,   5,   5,   5,   7,   7,   7,   4,
	  4,   4,   4,   4,   1,   1,   1,   1,   1,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 atck05_sel_1[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,   6,   6,   6,   6,   7,   7,   7,   7,
	  6,   6,   6,   6,   7,   7,   7,  10,   3,   3,   3,   3,   3,   3,   3,   3,
};

const Uint8 atck05_sel_2[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,
	  2,   2,   2,   4,   4,   4,   4,   4,   3,   3,   3,   3,   3,   3,   3,  10,
};

const Uint8 atck05_sel_3[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
	  0,   0,   0,   1,   1,   1,   5,   5,   5,   5,   5,   3,   3,   3,   3,  10,
};

const Uint8 atck05_sel_4[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
	  3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,  10,
};

const Uint8 atck05_sel_5[] = {
	  3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,
	  6,   6,   6,   6,   6,   6,   6,   6,   1,   1,   1,   1,   1,   1,   1,  10,
};

const Uint8 atck05_sel_6[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,   3,   3,   3,
	  6,   6,   6,   6,   6,   6,   6,   6,   1,   1,   1,   1,   0,   0,   0,  10,
};

const Uint8 atck05_sel_7[] = {
	  2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,
	  3,   3,   3,   3,   3,   3,   3,   3,   6,   6,   6,   6,   1,   1,   1,  10,
};

const Uint8 *const atck05_cmd[] = {
	atck05_cmd_00, atck05_cmd_01, atck05_cmd_02, atck05_cmd_03, atck05_cmd_04, atck05_cmd_05,
	atck05_cmd_06, atck05_cmd_07, atck05_cmd_08, atck05_cmd_09, atck05_cmd_10, atck05_cmd_11,
	atck05_cmd_12, atck05_cmd_13, atck05_cmd_14, atck05_cmd_15,
};

const Uint8 atck05_cmd_00[] = {
	STAND,
	WAIT2( 10 ),
	IF_RND( 16 ),
		SP_CMD( pl05_sp_gram, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ELSE,
		SP_CMD( pl05_sp_lgram, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ENDIF,
	IF_AREA( 64, 64, 32, 0 ),
		IF_RND( 20 ),
			CROUCH,
			WAIT2( 2 ),
			NAG_VFLIP_ADJ4( LV_HEAVY, LEVER_DOWN | LEVER_FORWARD ),
			STAND,
			WAIT2( 4 ),
		CMD_ELSE,
			CMD_JUMP( 56 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 22 ),
			IF_RND( 16 ),
				SP_CMD( pl05_sp_formation_a, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				SP_CMD( pl05_sp_formation_c, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_GAUGE( 2 ),
				SP_CMD( pl05_sp_ouroboros, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				SP_CMD( pl05_sp_formation_b, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck05_cmd_01[] = {
	STAND,
	CMD_JUMP( 57 ),
	CMD_JUMP( 54 ),
	CMD_JUMP( 55 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck05_cmd_02[] = {
	STAND,
	IF_DIST( 80, 1, 40, 1 ),
		DASH_ATCK( ATCK_STAND_LP ),
			IF_LEVEL( 24 ),
				CANCEL_SET,
					PUNCH( LV_LIGHT ),
					KICK( LV_LIGHT ),
					PUNCH( LV_LIGHT ),
					KICK( LV_LIGHT ),
					PUNCH( LV_HEAVY ),
					KICK( LV_HEAVY ),
					SP_CMD( pl05_sp_murakumo, 0, PK_PUNCH, LV_LIGHT, 0 ),
					CANCEL_CLR,
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 16 ),
					CANCEL_SET,
						PUNCH( LV_LIGHT ),
						KICK( LV_LIGHT ),
						PUNCH( LV_LIGHT ),
						KICK( LV_HEAVY ),
						SP_CMD( pl05_sp_murakumo, 0, PK_PUNCH, LV_LIGHT, 0 ),
						CANCEL_CLR,
					CMD_ENDIF,
				CMD_ELSE,
					CANCEL_SET,
						PUNCH( LV_LIGHT ),
						PUNCH( LV_LIGHT ),
						PUNCH( LV_HEAVY ),
						SP_CMD( pl05_sp_murakumo, 0, PK_PUNCH, LV_LIGHT, 0 ),
						CANCEL_CLR,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			WALK_B( 64 ),
		CMD_ENDIF,
	CMD_ELSE,
	CMD_ENDIF,
	STAND,
	WAIT2( 8 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck05_cmd_03[] = {
	STAND,
	CMD_JUMP( 0 ),
	CMD_JUMP( 14 ),
	JUMP_FLAG_SET,
		CMD_JUMP( 20 ),
		TIMER_SET( 60 ),
		IF_HEIGHT( 1, 96 ),
			TIMER_SET( 120 ),
			IF_AREA( 48, 24, 32, 1 ),
				IF_LEVEL( 24 ),
					KICK( LV_LIGHT ),
				CMD_ELSE,
					CANCEL_SET,
						KICK( LV_LIGHT ),
						KICK( LV_LIGHT ),
						CANCEL_CLR,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	IF_RND( 24 ),
		CMD_JUMP( 9 ),
	CMD_ELSE,
		CMD_JUMP( 56 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck05_cmd_04[] = {
	CROUCH,
	TIMER_SET( 15 ),
	IF_DIST( 88, 144, 32, 48 ),
		CROUCH,
		PUNCH( LV_HEAVY ),
	CMD_ELSE,
		IF_DIST( 64, 112, 32, 48 ),
			STAND,
			PUNCH( LV_HEAVY ),
		CMD_ELSE,
			DASH_B( 80 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck05_cmd_05[] = {
	STAND,
	IF_DIST( 224, 48, 48, 48 ),
		SP_CMD( pl05_sp_gram, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ELSE,
		IF_DIST( 224, 128, 48, 48 ),
			JUMP( DIR_UP ),
			JUMP_FLAG_SET,
				WAIT( 8 ),
				SP_CMD( pl05_sp_jgram, 0, PK_PUNCH, LV_LIGHT, 0 ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			DASH_F( 64 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck05_cmd_06[] = {
	STAND,
	JUMP_LEVEL( 48, 7, 49, 13, 50, 21, 51 ),
	CMD_JUMP( 57 ),
	IF_RND( 8 ),
		JUMP( DIR_FORWARD ),
		WAIT( 10 ),
		CMD_JUMP( 17 ),
	CMD_ELSE,
		IF_RND( 16 ),
			JUMP( DIR_UP ),
			WAIT( 10 ),
			CMD_JUMP( 17 ),
		CMD_ELSE,
			JUMP( DIR_FORWARD ),
			WAIT( 10 ),
			CMD_JUMP( 17 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck05_cmd_07[] = {
	STAND,
	IF_DIST( 64, 64, 64, 64 ),
		JUMP_LEVEL( 48, 7, 49, 13, 50, 21, 51 ),
		JUMP( DIR_UP ),
		WAIT( 16 ),
		JUMP_FLAG_SET,
			CMD_JUMP( 17 ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 57 ),
		IF_RND( 10 ),
			SP_CMD( pl05_sp_vajra, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			IF_RND( 16 ),
				SP_CMD( pl05_sp_vajra, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ELSE,
				SP_CMD( pl05_sp_vajra, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck05_cmd_08[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck05_cmd_09[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck05_cmd_10[] = {
	STAND,
	WAIT2( 2 ),
	IF_RND( 16 ),
		IF_GAUGE( 3 ),
			IF_AREA( 240, 80, 128, 32 ),
				DASH_B( 64 ),
				STAND,
				SP_V_COMBI,
			CMD_ENDIF,
			STAND,
			WAIT2( 2 ),
		CMD_ELSE,
			CMD_JUMP( 0 ),
		CMD_ENDIF,
	CMD_ELSE,
		WAIT( 1 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck05_cmd_11[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck05_cmd_12[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck05_cmd_13[] = {
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

const Uint8 atck05_cmd_14[] = {
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck05_cmd_15[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

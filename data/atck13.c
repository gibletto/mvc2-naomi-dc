/* Ruby Heart (character 13): the computer's attack lists.
   What it does once a waiting list's CMD_NEXT has sent it to attack. PLAYER_WORK's cpu_adrs[1]
   points here.

   atck13_tbl     one record (CPU_DATA): the offsets of the three tables.
   atck13_rate    the chance, out of 32, that a list's CMD_NEXT2 starts another attack list, by
                  the level of play.
   atck13_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   atck13_cmd     the lists: a pick is a number in this table.
   atck13_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                  list of lib13.c. */
#include "player.h"
#include "pl13_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const atck13_sel[], *const atck13_cmd[];
extern const Uint8 atck13_rate[], atck13_sel_0[], atck13_sel_1[], atck13_sel_2[], atck13_sel_3[],
	atck13_sel_4[], atck13_sel_5[], atck13_sel_6[], atck13_sel_7[], atck13_cmd_00[], atck13_cmd_01[],
	atck13_cmd_02[], atck13_cmd_03[], atck13_cmd_04[], atck13_cmd_05[], atck13_cmd_06[],
	atck13_cmd_07[], atck13_cmd_08[], atck13_cmd_09[], atck13_cmd_10[], atck13_cmd_11[],
	atck13_cmd_12[], atck13_cmd_13[], atck13_cmd_14[], atck13_cmd_15[], atck13_cmd_16[],
	atck13_cmd_17[], atck13_cmd_18[], atck13_cmd_19[], atck13_cmd_20[], atck13_cmd_21[],
	atck13_cmd_24[], atck13_cmd_28[];

const CPU_DATA atck13_tbl[] = {
	{ ( Sint32 )atck13_rate, ( Sint32 )atck13_sel, ( Sint32 )atck13_cmd },
};

const Uint8 atck13_rate[] = {
	  8,   8,   8,   8,   9,   9,  10,  10,  11,  11,  12,  12,  13,  13,  14,  14,
	 15,  15,  16,  16,  17,  17,  18,  18,  19,  20,  21,  22,  24,  26,  28,  30,
};

const Uint8 *const atck13_sel[] = {
	atck13_sel_0, atck13_sel_1, atck13_sel_2, atck13_sel_3, atck13_sel_4, atck13_sel_5,
	atck13_sel_6, atck13_sel_7,
};

const Uint8 atck13_sel_0[] = {
	 16,   0,   0,   0,   1,   2,   3,   3,   0,  20,   0,   0,   1,   2,   3,   3,
	  0,   0,   6,   0,   1,   2,   3,   3,   4,   4,   7,   7,   1,   2,  14,   3,
};

const Uint8 atck13_sel_1[] = {
	 17,   0,   2,   0,  20,   1,   3,   4,   0,   0,   2,   0,   1,   1,   3,   4,
	  0,   0,   6,  20,   1,   1,   3,   4,   0,  14,   6,   0,   1,   1,   3,   4,
};

const Uint8 atck13_sel_2[] = {
	 18,   7,   1,   1,   2,  20,   3,   0,   7,   7,   4,   1,   2,   6,   3,   3,
	  7,   4,   1,   1,   5,   6,   4,   3,  20,   4,   1,   1,   2,   6,   4,   3,
};

const Uint8 atck13_sel_3[] = {
	 12,  11,   1,   1,   2,  20,   3,   8,  12,  11,   1,   7,  14,   6,   3,   8,
	 12,  11,   1,   1,   2,   6,   4,   9,   5,   5,  13,  13,   2,   6,   3,   9,
};

const Uint8 atck13_sel_4[] = {
	 11,  11,   1,   1,   3,   2,   8,   8,  11,  11,   1,   1,  20,   6,  15,   7,
	 14,  11,  12,  13,   5,  12,   9,  10,  14,  11,   6,  13,   5,  12,   9,  10,
};

const Uint8 atck13_sel_5[] = {
	 16,  11,   0,   1,   3,   2,   8,   8,  17,  20,   1,  13,   3,   6,   1,   8,
	 18,  11,  13,  13,   5,  12,   9,  10,  11,   4,  13,  19,   5,  12,   9,  10,
};

const Uint8 atck13_sel_6[] = {
	 16,  11,   1,  12,   3,   2,   8,   8,  16,  11,  13,   1,   3,   6,  20,  15,
	 17,  12,  19,   1,   5,  12,  15,  10,  18,   6,   5,  13,   5,  12,   9,  10,
};

const Uint8 atck13_sel_7[] = {
	 16,  12,  13,  12,   3,   2,   8,   8,  20,   1,  13,  12,  19,   6,  15,   8,
	 17,  11,   5,   5,   5,  12,   5,  10,  18,   1,   9,  15,   5,  12,   9,  10,
};

const Uint8 *const atck13_cmd[] = {
	atck13_cmd_00, atck13_cmd_01, atck13_cmd_02, atck13_cmd_03, atck13_cmd_04, atck13_cmd_05,
	atck13_cmd_06, atck13_cmd_07, atck13_cmd_08, atck13_cmd_09, atck13_cmd_10, atck13_cmd_11,
	atck13_cmd_12, atck13_cmd_13, atck13_cmd_14, atck13_cmd_15, atck13_cmd_16, atck13_cmd_17,
	atck13_cmd_18, atck13_cmd_19, atck13_cmd_20, atck13_cmd_21, atck13_cmd_21, atck13_cmd_21,
	atck13_cmd_24, atck13_cmd_24, atck13_cmd_24, atck13_cmd_24, atck13_cmd_28, atck13_cmd_28,
	atck13_cmd_28, atck13_cmd_28,
};

const Uint8 atck13_cmd_00[] = {
	STAND,
	WAIT2( 10 ),
	WALK_B( 512 ),
	WAIT2( 8 ),
	SP_CMD( pl13_sp_sublimation, 0, PK_PUNCH, LV_HEAVY, 0 ),
	WAIT2( 14 ),
	DASH_F( 64 ),
	SP_CMD( pl13_sp_fantome, 0, PK_PUNCH, LV_HEAVY, 0 ),
	IF_LEVEL( 16 ),
		IF_GAUGE( 3 ),
			IF_RND( 2 ),
				SP_CMD( pl13_sp_paretonnre, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		DASH_EM( 48 ),
		PUNCH( LV_LIGHT ),
		PUNCH( LV_LIGHT ),
		IF_RND( 8 ),
			SP_CMD( pl13_sp_schwarz, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ENDIF,
		DASH_B( 64 ),
		WAIT2( 14 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck13_cmd_01[] = {
	STAND,
	WAIT2( 8 ),
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
	IF_LEVEL( 24 ),
		IF_EM_ATCK( ATCK_CROUCH_HP ),
			WAIT2( 4 ),
			CANCEL_SET2,
				CROUCH,
				KICK( LV_HEAVY ),
				SJUMP3( DIR_UP ),
				CANCEL_CLR,
			CMD_ENDIF,
			SJUMP_FLAG_SET,
				CMD_JUMP( 4 ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			SP_CMD( pl13_sp_rafale, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 8 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 8 ),
	CMD_EXIT,
};

const Uint8 atck13_cmd_02[] = {
	STAND,
	WAIT2( 8 ),
	TIMER_SET( 60 ),
	CMD_JUMP( 11 ),
	STAND,
	CMD_JUMP( 0 ),
	WALK_F( 64 ),
	CMD_JUMP( 5 ),
	WAIT2( 8 ),
	CMD_EXIT,
};

const Uint8 atck13_cmd_03[] = {
	STAND,
	CMD_JUMP( 0 ),
	WAIT2( 8 ),
	CMD_JUMP( 5 ),
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

const Uint8 atck13_cmd_04[] = {
	STAND,
	WAIT2( 14 ),
	DASH_F( 48 ),
	TIMER_SET( 60 ),
	IF_EM_ATCK( ATCK_CROUCH_LP ),
		CANCEL_SET2,
			PUNCH( LV_LIGHT ),
			PUNCH( LV_LIGHT ),
			PUNCH( LV_HEAVY ),
			CANCEL_CLR,
		CMD_ENDIF,
		IF_LEVEL( 16 ),
			SP_CMD( pl13_sp_sublimation, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck13_cmd_05[] = {
	STAND,
	WAIT2( 4 ),
	DASH_EM( 16 ),
	IF_LEVEL( 8 ),
		CROUCH,
		KICK( LV_LIGHT ),
		IF_EM_ATCK( ATCK_CROUCH_HP ),
			CANCEL_SET2,
				PUNCH( LV_HEAVY ),
				SJUMP3( DIR_UP ),
				CANCEL_CLR,
			CMD_ENDIF,
			WAIT( 2 ),
			SJUMP_FLAG_SET,
				CMD_JUMP( 4 ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			IF_EM_ATCK( ATCK_CROUCH_HK ),
				CROUCH,
				KICK( LV_HEAVY ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		CROUCH,
		IF_EM_ATCK( ATCK_CROUCH_LK ),
			KICK( LV_LIGHT ),
			KICK( LV_HEAVY ),
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck13_cmd_06[] = {
	STAND,
	WAIT2( 4 ),
	CMD_JUMP( 0 ),
	SP_CMD( pl13_sp_rafale, 0, PK_PUNCH, LV_LIGHT, 0 ),
	WALK_F( 64 ),
	IF_RND( 16 ),
		SP_CMD( pl13_sp_sublimation, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ENDIF,
	WALK_B( 48 ),
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck13_cmd_07[] = {
	STAND,
	WAIT2( 4 ),
	DASH_B( 48 ),
	SP_CMD( pl13_sp_sublimation, 0, PK_PUNCH, LV_HEAVY, 0 ),
	WAIT2( 10 ),
	DASH_F( 80 ),
	SP_CMD( pl13_sp_fantome, 0, PK_PUNCH, LV_LIGHT, 0 ),
	WAIT2( 10 ),
	STAND,
	CMD_EXIT,
};

const Uint8 atck13_cmd_08[] = {
	STAND,
	IF_GAUGE( 1 ),
		IF_RND( 16 ),
			WAIT2( 2 ),
			IF_AREA( 240, 48, 128, 16 ),
				SP_CMD( pl13_sp_magie, 0, PK_KICK, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ELSE,
			SP_CMD( pl13_sp_schwarz, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ELSE,
		STAND,
		WAIT2( 2 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck13_cmd_09[] = {
	STAND,
	IF_GAUGE( 1 ),
		IF_RND( 16 ),
			SP_CMD( pl13_sp_paretonnre, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_ELSE,
		STAND,
		WAIT2( 2 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck13_cmd_10[] = {
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

const Uint8 atck13_cmd_11[] = {
	STAND,
	DASH_B( 96 ),
	STAND,
	WAIT2( 2 ),
	IF_AREA( 128, 64, 128, 16 ),
		SP_CMD( pl13_sp_rafale, 0, PK_PUNCH, LV_LIGHT, 0 ),
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
		WAIT2( 2 ),
		IF_AREA( 128, 64, 128, 16 ),
			IF_LEVEL( 16 ),
				IF_GAUGE( 2 ),
					IF_RND( 10 ),
						SP_CMD( pl13_sp_flammeile, 0, PK_PUNCH, LV_LIGHT, 0 ),
					CMD_ENDIF,
				CMD_ELSE,
					SP_CMD( pl13_sp_rafale, 0, PK_PUNCH, LV_LIGHT, 0 ),
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

const Uint8 atck13_cmd_12[] = {
	STAND,
	WAIT2( 2 ),
	CMD_JUMP( 14 ),
	WAIT( 4 ),
	WAIT2( 8 ),
	CMD_JUMP( 21 ),
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck13_cmd_13[] = {
	STAND,
	LOCK_SET,
	WAIT2( 6 ),
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

const Uint8 atck13_cmd_14[] = {
	DASH_B( 96 ),
	SP_CMD( pl13_sp_fantome, 0, PK_PUNCH, LV_HEAVY, 0 ),
	JUMP( DIR_FORWARD ),
	STAND,
	SP_CMD( pl13_sp_sublimation, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck13_cmd_15[] = {
	STAND,
	CMD_JUMP( 26 ),
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck13_cmd_16[] = {
	CMD_JUMP( 54 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck13_cmd_17[] = {
	CMD_JUMP( 56 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck13_cmd_18[] = {
	CMD_JUMP( 53 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck13_cmd_19[] = {
	DASH_B( 256 ),
	CMD_JUMP( 23 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck13_cmd_20[] = {
	WALK_F( 64 ),
	TIMER_SET( 60 ),
	IF_EM_ATCK( ATCK_STAND_HK ),
		IF_LEVEL( 10 ),
			CANCEL_SET2,
				KICK( LV_LIGHT ),
				KICK( LV_LIGHT ),
				KICK( LV_HEAVY ),
				SJUMP3( DIR_FORWARD ),
				CANCEL_CLR,
			CMD_ENDIF,
			SJUMP_FLAG_SET,
				CMD_JUMP( 4 ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			IF_EM_ATCK( ATCK_CROUCH_HK ),
				CROUCH,
				KICK( LV_HEAVY ),
				SP_CMD( pl13_sp_fantome, 0, PK_PUNCH, LV_LIGHT, 0 ),
				JUMP( DIR_BACK ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck13_cmd_21[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck13_cmd_24[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck13_cmd_28[] = {
	CMD_EXIT,
};

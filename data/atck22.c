/* Sakura (character 22): the computer's attack lists.
   What it does once a waiting list's CMD_NEXT has sent it to attack. PLAYER_WORK's cpu_adrs[1]
   points here.

   atck22_tbl     one record (CPU_DATA): the offsets of the three tables.
   atck22_rate    the chance, out of 32, that a list's CMD_NEXT2 starts another attack list, by
                  the level of play.
   atck22_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   atck22_cmd     the lists: a pick is a number in this table.
   atck22_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                  list of lib22.c. */
#include "player.h"
#include "pl22_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const atck22_sel[], *const atck22_cmd[];
extern const Uint8 atck22_rate[], atck22_sel_0[], atck22_sel_1[], atck22_sel_2[], atck22_sel_3[],
	atck22_sel_4[], atck22_sel_5[], atck22_sel_6[], atck22_sel_7[], atck22_cmd_00[], atck22_cmd_01[],
	atck22_cmd_02[], atck22_cmd_03[], atck22_cmd_04[], atck22_cmd_05[], atck22_cmd_06[],
	atck22_cmd_07[], atck22_cmd_08[], atck22_cmd_09[], atck22_cmd_10[], atck22_cmd_11[],
	atck22_cmd_12[], atck22_cmd_13[], atck22_cmd_14[], atck22_cmd_15[], atck22_cmd_16[],
	atck22_cmd_17[], atck22_cmd_18[], atck22_cmd_19[], atck22_cmd_20[], atck22_cmd_21[],
	atck22_cmd_22[], atck22_cmd_23[], atck22_cmd_24[], atck22_cmd_25[], atck22_cmd_26[],
	atck22_cmd_27[], atck22_cmd_28[], atck22_cmd_29[], atck22_cmd_30[], atck22_cmd_31[];

const CPU_DATA atck22_tbl[] = {
	{ ( Sint32 )atck22_rate, ( Sint32 )atck22_sel, ( Sint32 )atck22_cmd },
};

const Uint8 atck22_rate[] = {
	  8,   8,   8,   8,   9,   9,  10,  10,  11,  11,  12,  12,  13,  13,  14,  14,
	 15,  15,  16,  16,  17,  17,  18,  18,  19,  20,  21,  22,  24,  26,  28,  30,
};

const Uint8 *const atck22_sel[] = {
	atck22_sel_0, atck22_sel_1, atck22_sel_2, atck22_sel_3, atck22_sel_4, atck22_sel_5,
	atck22_sel_6, atck22_sel_7,
};

const Uint8 atck22_sel_0[] = {
	  0,   0,   0,   5,   4,   4,   4,   4,   1,   1,   1,   5,   1,   1,   1,   1,
	  3,   3,   3,   5,   3,   3,   3,   3,  14,  14,  14,   5,  14,  14,  14,  14,
};

const Uint8 atck22_sel_1[] = {
	  0,   0,   0,   5,   1,   1,   3,   4,   0,   0,   0,   5,   1,   1,   3,   4,
	  0,   0,   0,   5,   1,   1,   3,   4,   0,   0,   0,   5,   1,   1,   3,   4,
};

const Uint8 atck22_sel_2[] = {
	  7,   7,   1,   1,   2,   6,   3,   3,   7,   7,   1,   1,   2,   6,   3,   3,
	  7,   7,   1,   1,   2,   6,   3,   3,   7,   7,   1,   1,   2,   6,   3,   3,
};

const Uint8 atck22_sel_3[] = {
	 12,  11,   1,   1,   2,   6,   3,   8,  12,  11,   1,   1,   2,   6,   3,   8,
	 12,  11,   1,   1,   2,   6,   3,   9,  12,  11,  13,  13,   2,   6,   3,   9,
};

const Uint8 atck22_sel_4[] = {
	 11,  11,   1,   1,   3,   2,   8,   8,  11,  11,   1,   1,   3,   6,   8,   8,
	 11,   5,  13,  13,   5,  12,   9,  10,  11,   5,  13,  13,   5,  12,   9,  10,
};

const Uint8 atck22_sel_5[] = {
	 11,  11,   1,   1,   3,   2,   8,   8,  11,  11,   1,  13,   3,   6,   8,   8,
	  5,   5,  13,  13,   5,  12,   9,  10,   5,   5,  13,  13,   5,  12,   9,  10,
};

const Uint8 atck22_sel_6[] = {
	 11,  11,   1,  13,   3,   2,   8,   8,  11,  11,  13,  13,   3,   6,   8,   8,
	  5,   5,  13,  13,   5,  12,   9,  10,   5,   5,  13,  13,   5,  12,   9,  10,
};

const Uint8 atck22_sel_7[] = {
	 11,  11,  13,  13,   3,   2,   8,   8,  11,  11,  13,  13,   3,   6,   8,   8,
	 11,   5,  13,  13,   5,  12,   9,  10,  11,   5,  13,  13,   5,  12,   9,  10,
};

const Uint8 *const atck22_cmd[] = {
	atck22_cmd_00, atck22_cmd_01, atck22_cmd_02, atck22_cmd_03, atck22_cmd_04, atck22_cmd_05,
	atck22_cmd_06, atck22_cmd_07, atck22_cmd_08, atck22_cmd_09, atck22_cmd_10, atck22_cmd_11,
	atck22_cmd_12, atck22_cmd_13, atck22_cmd_14, atck22_cmd_15, atck22_cmd_16, atck22_cmd_17,
	atck22_cmd_18, atck22_cmd_19, atck22_cmd_20, atck22_cmd_21, atck22_cmd_22, atck22_cmd_23,
	atck22_cmd_24, atck22_cmd_25, atck22_cmd_26, atck22_cmd_27, atck22_cmd_28, atck22_cmd_29,
	atck22_cmd_30, atck22_cmd_31,
};

const Uint8 atck22_cmd_00[] = {
	STAND,
	WAIT2( 10 ),
	CMD_JUMP( 75 ),
	WALK_B( 1024 ),
	WAIT2( 8 ),
	IF_DIST( 48, 32, 272, 32 ),
		SP_CMD( pl22_sp_shouou, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ELSE,
		IF_DIST( 48, 32, 176, 32 ),
			SP_CMD( pl22_sp_shouou, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ELSE,
			SP_CMD( pl22_sp_shouou, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT2( 14 ),
	CMD_EXIT,
};

const Uint8 atck22_cmd_01[] = {
	STAND,
	CMD_JUMP( 75 ),
	WAIT2( 8 ),
	CMD_JUMP( 14 ),
	JUMP_FLAG_SET,
		CMD_JUMP( 20 ),
		TIMER_SET( 60 ),
		IF_HEIGHT( 1, 96 ),
			KICK( LV_HEAVY ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	CMD_JUMP( 77 ),
	WAIT2( 4 ),
	IF_EM_ATCK( ATCK_CROUCH_MK ),
		CANCEL_SET,
			CROUCH,
			KICK( LV_LIGHT ),
			SP_CMD( pl22_sp_shouou, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	WAIT2( 8 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck22_cmd_02[] = {
	STAND,
	CMD_JUMP( 75 ),
	WAIT2( 8 ),
	TIMER_SET( 120 ),
	WALK_ATCK( ATCK_CROUCH_MK ),
		CROUCH,
		WAIT2( 8 ),
		CANCEL_SET2,
			KICK( LV_LIGHT ),
			SP_CMD( pl22_sp_shunpu, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CANCEL_CLR,
		CMD_ENDIF,
		STAND,
	CMD_ENDIF,
	WAIT2( 8 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck22_cmd_03[] = {
	STAND,
	WAIT2( 8 ),
	TIMER_SET( 60 ),
	WALK_ATCK( ATCK_CROUCH_HK ),
		CANCEL_SET,
			CROUCH,
			WAIT2( 2 ),
			CMD_JUMP( 75 ),
			KICK( LV_HEAVY ),
			CROUCH,
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_JUMP( 0 ),
	WAIT2( 8 ),
	CMD_EXIT,
};

const Uint8 atck22_cmd_04[] = {
	STAND,
	CMD_JUMP( 75 ),
	WAIT2( 14 ),
	TIMER_SET( 60 ),
	IF_DIST( 48, 32, 272, 32 ),
		SP_CMD( pl22_sp_shouou, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ELSE,
		SP_CMD( pl22_sp_shouou, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ENDIF,
	STAND,
	CMD_EXIT,
};

const Uint8 atck22_cmd_05[] = {
	STAND,
	CMD_JUMP( 75 ),
	WAIT2( 4 ),
	DASH_ATCK( ATCK_CROUCH_HP ),
		IF_EM_ATCK( ATCK_CROUCH_HP ),
			DASH_F( 32 ),
			CROUCH,
			CANCEL_SET2,
				PUNCH( LV_LIGHT ),
				PUNCH( LV_HEAVY ),
				SJUMP3( DIR_FORWARD ),
				CANCEL_CLR,
			CMD_ENDIF,
			SJUMP_FLAG_SET,
				WAIT( 4 ),
				CMD_JUMP( 4 ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			CANCEL_SET,
				KICK( LV_LIGHT ),
				KICK( LV_HEAVY ),
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck22_cmd_06[] = {
	STAND,
	CMD_JUMP( 75 ),
	JUMP( DIR_UP ),
	WAIT2( 14 ),
	SP_CMD( pl22_sp_jhadou, 0, PK_PUNCH, LV_HEAVY, 0 ),
	STAND,
	WAIT2( 2 ),
	CMD_JUMP( 77 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck22_cmd_07[] = {
	STAND,
	WAIT2( 4 ),
	DASH_ATCK( ATCK_STAND_LK ),
		CANCEL_SET,
			KICK( LV_LIGHT ),
			CROUCH,
			KICK( LV_LIGHT ),
			KICK( LV_HEAVY ),
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	CMD_EXIT,
};

const Uint8 atck22_cmd_08[] = {
	STAND,
	IF_GAUGE( 1 ),
		WAIT2( 2 ),
		IF_AREA( 64, 64, 160, 160 ),
			IF_CHAR( 0 ),
				SP_CMD( pl22_sp_shinkuu, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				SP_CMD( pl22_sp_dshinkuu, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck22_cmd_09[] = {
	STAND,
	IF_GAUGE( 1 ),
		WAIT2( 2 ),
		IF_AREA( 96, 32, 96, 32 ),
			SP_CMD( pl22_sp_midare, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck22_cmd_10[] = {
	STAND,
	IF_GAUGE( 1 ),
		IF_AREA( 32, 48, 64, 48 ),
			SP_CMD( pl22_sp_haru, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck22_cmd_11[] = {
	STAND,
	WAIT2( 2 ),
	IF_RND( 16 ),
		IF_GAUGE( 2 ),
			IF_AREA( 80, 80, 160, 160 ),
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
	CMD_EXIT,
};

const Uint8 atck22_cmd_12[] = {
	STAND,
	CMD_JUMP( 75 ),
	WAIT2( 4 ),
	IF_AREA( 48, 32, 272, 32 ),
		SP_CMD( pl22_sp_shunpu, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ELSE,
		IF_AREA( 48, 32, 176, 32 ),
			SP_CMD( pl22_sp_shunpu, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ELSE,
			SP_CMD( pl22_sp_shunpu, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_JUMP( 0 ),
	CMD_JUMP( 77 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck22_cmd_13[] = {
	STAND,
	LOCK_SET,
	WAIT2( 6 ),
	CMD_JUMP( 0 ),
	CMD_JUMP( 25 ),
	JUMP_EM( 12, 12, 12, 12, 13, 13, 13, 13 ),
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

const Uint8 atck22_cmd_14[] = {
	STAND,
	CMD_JUMP( 77 ),
	IF_AREA( 48, 32, 272, 32 ),
		DASH_F( 32 ),
		JUMP( DIR_FORWARD ),
		JUMP_FLAG_SET,
			WAIT( 24 ),
			SP_CMD( pl22_sp_jhadou, 0, PK_PUNCH, LV_HEAVY, 0 ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		IF_AREA( 48, 32, 176, 32 ),
			JUMP( DIR_FORWARD ),
			JUMP_FLAG_SET,
				CMD_JUMP( 0 ),
				WAIT( 24 ),
				SP_CMD( pl22_sp_jhadou, 0, PK_PUNCH, LV_HEAVY, 0 ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			JUMP( DIR_BACK ),
			JUMP_FLAG_SET,
				CMD_JUMP( 0 ),
				WAIT( 24 ),
				SP_CMD( pl22_sp_jhadou, 0, PK_PUNCH, LV_HEAVY, 0 ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck22_cmd_15[] = {
	CMD_EXIT,
};

const Uint8 atck22_cmd_16[] = {
	CMD_EXIT,
};

const Uint8 atck22_cmd_17[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck22_cmd_18[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck22_cmd_19[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck22_cmd_20[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck22_cmd_21[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck22_cmd_22[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck22_cmd_23[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck22_cmd_24[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck22_cmd_25[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck22_cmd_26[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck22_cmd_27[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck22_cmd_28[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck22_cmd_29[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck22_cmd_30[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck22_cmd_31[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

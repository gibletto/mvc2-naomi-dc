/* Dan (character 23): the computer's attack lists.
   What it does once a waiting list's CMD_NEXT has sent it to attack. PLAYER_WORK's cpu_adrs[1]
   points here.

   atck23_tbl     one record (CPU_DATA): the offsets of the three tables.
   atck23_rate    the chance, out of 32, that a list's CMD_NEXT2 starts another attack list, by
                  the level of play.
   atck23_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   atck23_cmd     the lists: a pick is a number in this table.
   atck23_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                  list of lib23.c. */
#include "player.h"
#include "pl23_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const atck23_sel[], *const atck23_cmd[];
extern const Uint8 atck23_rate[], atck23_sel_0[], atck23_sel_1[], atck23_sel_2[], atck23_sel_3[],
	atck23_sel_4[], atck23_sel_5[], atck23_sel_6[], atck23_sel_7[], atck23_cmd_00[], atck23_cmd_01[],
	atck23_cmd_02[], atck23_cmd_03[], atck23_cmd_04[], atck23_cmd_05[], atck23_cmd_06[],
	atck23_cmd_07[], atck23_cmd_08[], atck23_cmd_09[], atck23_cmd_10[], atck23_cmd_11[],
	atck23_cmd_12[], atck23_cmd_13[], atck23_cmd_14[], atck23_cmd_15[], atck23_cmd_16[],
	atck23_cmd_17[], atck23_cmd_18[], atck23_cmd_19[], atck23_cmd_20[], atck23_cmd_21[],
	atck23_cmd_22[], atck23_cmd_23[], atck23_cmd_24[], atck23_cmd_25[], atck23_cmd_26[],
	atck23_cmd_27[], atck23_cmd_28[], atck23_cmd_29[], atck23_cmd_30[], atck23_cmd_31[];

const CPU_DATA atck23_tbl[] = {
	{ ( Sint32 )atck23_rate, ( Sint32 )atck23_sel, ( Sint32 )atck23_cmd },
};

const Uint8 atck23_rate[] = {
	  8,   8,   8,   8,   9,   9,  10,  10,  11,  11,  12,  12,  13,  13,  14,  14,
	 15,  15,  16,  16,  17,  17,  18,  18,  19,  20,  21,  22,  24,  26,  28,  30,
};

const Uint8 *const atck23_sel[] = {
	atck23_sel_0, atck23_sel_1, atck23_sel_2, atck23_sel_3, atck23_sel_4, atck23_sel_5,
	atck23_sel_6, atck23_sel_7,
};

const Uint8 atck23_sel_0[] = {
	  1,   0,   0,   0,   0,   0,   3,   3,   1,   0,   0,   0,   0,   0,   3,   3,
	  1,   4,   0,   0,   0,   0,   3,   3,   1,   4,   0,   6,   0,   0,   3,   3,
};

const Uint8 atck23_sel_1[] = {
	  0,   0,   0,   0,   1,   1,   3,   4,   0,   0,   0,   0,   1,   1,   3,   4,
	  4,   0,   0,   0,   1,   1,   3,   4,   4,   0,   0,   6,   1,   1,   3,   4,
};

const Uint8 atck23_sel_2[] = {
	  7,   7,   1,   1,   2,   6,   3,   3,   7,   7,   1,   1,   2,   6,   3,   3,
	  7,   7,   1,   1,   2,   6,   3,   3,   7,   7,   1,   1,   2,   6,   3,   3,
};

const Uint8 atck23_sel_3[] = {
	 12,  11,   1,   1,   2,   6,   3,   8,  12,  11,   1,   1,   2,   6,   3,   8,
	 12,  11,   1,   1,   2,   6,   3,   9,  12,  11,  13,  13,   2,   6,   3,   9,
};

const Uint8 atck23_sel_4[] = {
	 11,  11,   1,   1,   3,   2,   8,   8,  11,  11,   1,   1,   3,   6,   8,   8,
	 11,  11,  13,  13,   5,  12,   9,  10,  11,  11,  13,  13,   5,  12,   9,  10,
};

const Uint8 atck23_sel_5[] = {
	 11,  11,   1,   1,   3,   2,   8,   8,  11,  11,   1,  13,   3,   6,   8,   8,
	 11,  11,  13,  13,   5,  12,   9,  10,  11,  11,  13,  13,   5,  12,   9,  10,
};

const Uint8 atck23_sel_6[] = {
	  5,  11,   1,  13,   3,   2,   8,   8,  11,  11,  13,  13,   3,   6,   8,   8,
	 11,  11,  13,  13,   5,  12,   9,  10,  11,  11,  13,  13,   5,  12,   9,  10,
};

const Uint8 atck23_sel_7[] = {
	 11,  11,  13,  13,   3,   2,   8,   8,  11,   5,  13,  13,   3,   6,   8,   8,
	 11,  11,  13,  13,   5,  12,   9,  10,   5,  11,  13,  13,   5,  12,   9,  10,
};

const Uint8 *const atck23_cmd[] = {
	atck23_cmd_00, atck23_cmd_01, atck23_cmd_02, atck23_cmd_03, atck23_cmd_04, atck23_cmd_05,
	atck23_cmd_06, atck23_cmd_07, atck23_cmd_08, atck23_cmd_09, atck23_cmd_10, atck23_cmd_11,
	atck23_cmd_12, atck23_cmd_13, atck23_cmd_14, atck23_cmd_15, atck23_cmd_16, atck23_cmd_17,
	atck23_cmd_18, atck23_cmd_19, atck23_cmd_20, atck23_cmd_21, atck23_cmd_22, atck23_cmd_23,
	atck23_cmd_24, atck23_cmd_25, atck23_cmd_26, atck23_cmd_27, atck23_cmd_28, atck23_cmd_29,
	atck23_cmd_30, atck23_cmd_31,
};

const Uint8 atck23_cmd_00[] = {
	STAND,
	WAIT2( 10 ),
	WALK_B( 1024 ),
	WAIT2( 8 ),
	SP_CMD( pl23_sp_sign, 0, PK_PUNCH, LV_HEAVY, 0 ),
	WAIT2( 14 ),
	CMD_EXIT,
};

const Uint8 atck23_cmd_01[] = {
	STAND,
	WAIT2( 8 ),
	CMD_JUMP( 14 ),
	JUMP_FLAG_SET,
		CMD_JUMP( 20 ),
		TIMER_SET( 60 ),
		IF_HEIGHT( 1, 96 ),
			CMD_JUMP( 9 ),
		CMD_ELSE,
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
				CANCEL_CLR,
			CMD_ENDIF,
			SJUMP_FLAG_SET,
				CMD_JUMP( 4 ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			CROUCH,
			KICK( LV_HEAVY ),
		CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 7 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 8 ),
	CMD_EXIT,
};

const Uint8 atck23_cmd_02[] = {
	STAND,
	WAIT2( 8 ),
	TIMER_SET( 60 ),
	WALK_ATCK( ATCK_CROUCH_MK ),
		CROUCH,
		WAIT2( 8 ),
		CANCEL_SET2,
			KICK( LV_LIGHT ),
			SP_CMD( pl23_sp_danku, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CANCEL_CLR,
		CMD_ENDIF,
		STAND,
	CMD_ENDIF,
	WAIT2( 8 ),
	CMD_EXIT,
};

const Uint8 atck23_cmd_03[] = {
	STAND,
	CMD_JUMP( 0 ),
	WAIT2( 8 ),
	TIMER_SET( 60 ),
	WALK_ATCK( ATCK_CROUCH_HK ),
		CROUCH,
		WAIT2( 2 ),
		KICK( LV_HEAVY ),
		CROUCH,
	CMD_ENDIF,
	WAIT2( 8 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck23_cmd_04[] = {
	STAND,
	WAIT2( 14 ),
	TIMER_SET( 60 ),
	IF_EM_ATCK( ATCK_CROUCH_HP ),
		SP_CMD( pl23_sp_kouryuu, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ELSE,
		SP_CMD( pl23_sp_kouryuu, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ENDIF,
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck23_cmd_05[] = {
	STAND,
	WAIT2( 4 ),
	DASH_EM( 8 ),
	CROUCH,
	IF_LEVEL( 24 ),
		CANCEL_SET2,
			NAG_VFLIP_ADJ3( LV_HEAVY, LEVER_DOWN | LEVER_FORWARD ),
			SJUMP3( DIR_FORWARD ),
			CANCEL_CLR,
		CMD_ENDIF,
		SJUMP_FLAG_SET,
			CMD_JUMP( 4 ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		SP_CMD( pl23_sp_danku, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ENDIF,
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck23_cmd_06[] = {
	STAND,
	WAIT2( 4 ),
	CMD_JUMP( 0 ),
	IF_AREA( 64, 64, 16, 16 ),
		WAIT2( 4 ),
		SP_CMD( pl23_sp_gadou, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck23_cmd_07[] = {
	STAND,
	WAIT2( 4 ),
	SP_CMD( pl23_sp_danku, 0, PK_PUNCH, LV_HEAVY, 0 ),
	STAND,
	WAIT2( 2 ),
	IF_EM_ATCK( ATCK_CROUCH_HK ),
		CROUCH,
		KICK( LV_HEAVY ),
	CMD_ENDIF,
	STAND,
	CMD_EXIT,
};

const Uint8 atck23_cmd_08[] = {
	STAND,
	IF_GAUGE( 1 ),
		IF_RND( 16 ),
			WAIT2( 2 ),
			IF_AREA( 96, 48, 64, 16 ),
				SP_CMD( pl23_sp_shinkuu, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		STAND,
		WAIT2( 2 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck23_cmd_09[] = {
	STAND,
	IF_GAUGE( 1 ),
		IF_RND( 16 ),
			IF_EM_ATCK( pl23_at_rekka ),
				SP_CMD( pl23_sp_rekka, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		STAND,
		WAIT2( 2 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck23_cmd_10[] = {
	STAND,
	WAIT2( 2 ),
	IF_RND( 16 ),
		IF_GAUGE( 3 ),
			IF_AREA( 96, 48, 64, 16 ),
				DASH_B( 64 ),
				STAND,
				SP_V_COMBI,
			CMD_ENDIF,
			STAND,
			WAIT2( 2 ),
		CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 54 ),
		WAIT( 1 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck23_cmd_11[] = {
	STAND,
	IF_GAUGE( 1 ),
		IF_RND( 16 ),
			IF_EM_ATCK( pl23_at_buraiken ),
				SP_CMD( pl23_sp_buraiken, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		STAND,
		WAIT2( 2 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck23_cmd_12[] = {
	STAND,
	WAIT2( 4 ),
	CMD_JUMP( 0 ),
	IF_AREA( 240, 64, 128, 16 ),
		WAIT2( 4 ),
		IF_LEVEL( 24 ),
			SP_CMD( pl23_sp_sign, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			IF_LEVEL( 8 ),
				SP_CMD( pl23_sp_sign, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ELSE,
				SP_CMD( pl23_sp_sign, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck23_cmd_13[] = {
	STAND,
	LOCK_SET,
	WAIT2( 6 ),
	CMD_JUMP( 0 ),
	CMD_JUMP( 25 ),
	JUMP_FLAG_SET,
		JUMP_LEVEL( 38, 8, 38, 29, 39, 31, 40 ),
		JUMP_FLAG_CLR,
	CMD_ENDIF,
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

const Uint8 atck23_cmd_14[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck23_cmd_15[] = {
	CMD_EXIT,
};

const Uint8 atck23_cmd_16[] = {
	CMD_EXIT,
};

const Uint8 atck23_cmd_17[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck23_cmd_18[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck23_cmd_19[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck23_cmd_20[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck23_cmd_21[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck23_cmd_22[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck23_cmd_23[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck23_cmd_24[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck23_cmd_25[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck23_cmd_26[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck23_cmd_27[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck23_cmd_28[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck23_cmd_29[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck23_cmd_30[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck23_cmd_31[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

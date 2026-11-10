/* Akuma (character 1e): the computer's attack lists.
   What it does once a waiting list's CMD_NEXT has sent it to attack. PLAYER_WORK's cpu_adrs[1]
   points here.

   atck1e_tbl     one record (CPU_DATA): the offsets of the three tables.
   atck1e_rate    the chance, out of 32, that a list's CMD_NEXT2 starts another attack list, by
                  the level of play.
   atck1e_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   atck1e_cmd     the lists: a pick is a number in this table.
   atck1e_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                  list of lib1e.c. */
#include "player.h"
#include "pl1e_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const atck1e_sel[], *const atck1e_cmd[];
extern const Uint8 atck1e_rate[], atck1e_sel_0[], atck1e_sel_1[], atck1e_sel_2[], atck1e_sel_3[],
	atck1e_sel_4[], atck1e_sel_5[], atck1e_sel_6[], atck1e_sel_7[], atck1e_cmd_00[], atck1e_cmd_01[],
	atck1e_cmd_02[], atck1e_cmd_03[], atck1e_cmd_04[], atck1e_cmd_05[], atck1e_cmd_06[],
	atck1e_cmd_07[], atck1e_cmd_08[], atck1e_cmd_09[], atck1e_cmd_10[], atck1e_cmd_11[],
	atck1e_cmd_12[], atck1e_cmd_13[], atck1e_cmd_14[], atck1e_cmd_15[], atck1e_cmd_16[],
	atck1e_cmd_17[], atck1e_cmd_18[], atck1e_cmd_19[], atck1e_cmd_20[], atck1e_cmd_21[],
	atck1e_cmd_22[], atck1e_cmd_23[], atck1e_cmd_24[], atck1e_cmd_25[], atck1e_cmd_26[],
	atck1e_cmd_27[], atck1e_cmd_28[], atck1e_cmd_29[], atck1e_cmd_30[], atck1e_cmd_31[];

const CPU_DATA atck1e_tbl[] = {
	{ ( Sint32 )atck1e_rate, ( Sint32 )atck1e_sel, ( Sint32 )atck1e_cmd },
};

const Uint8 atck1e_rate[] = {
	 10,  10,  13,  13,  15,  15,  17,  17,  18,  19,  20,  21,  22,  23,  24,  25,
	 25,  26,  27,  27,  27,  27,  27,  27,  28,  28,  29,  29,  30,  30,  31,  31,
};

const Uint8 *const atck1e_sel[] = {
	atck1e_sel_0, atck1e_sel_1, atck1e_sel_2, atck1e_sel_3, atck1e_sel_4, atck1e_sel_5,
	atck1e_sel_6, atck1e_sel_7,
};

const Uint8 atck1e_sel_0[] = {
	  0,   0,   0,   0,   0,   0,   3,   3,   0,   0,   0,   0,   0,   0,   3,   3,
	  0,   0,   0,   0,  15,  15,   3,   9,  15,  15,  15,  15,  15,  15,   3,  16,
};

const Uint8 atck1e_sel_1[] = {
	  0,   0,   0,   0,   1,   1,   3,   4,   0,   0,   0,   0,   1,   1,   3,   4,
	 14,  14,  14,  14,   1,   1,   3,   9,  15,  15,  15,  15,   1,   1,   3,  16,
};

const Uint8 atck1e_sel_2[] = {
	  7,   7,   1,   1,  14,   6,   3,   3,   7,   7,   1,   1,  14,   6,   3,   3,
	 15,  15,   1,   1,  14,   6,   3,   9,  15,  15,  17,  17,   2,   6,   3,  16,
};

const Uint8 atck1e_sel_3[] = {
	 12,  11,   1,   1,  14,   6,   3,   8,  12,  11,   1,   1,  14,   6,   3,  16,
	 12,  11,   1,   1,   2,   6,  15,  16,  12,  11,  13,  13,   2,   6,  15,   9,
};

const Uint8 atck1e_sel_4[] = {
	 15,  15,  17,  17,  14,   2,   8,   8,  11,  11,   1,   1,  14,   6,  16,  16,
	 11,  11,  13,  13,   5,  12,   9,  16,  11,  11,  13,  13,   5,  12,   9,  10,
};

const Uint8 atck1e_sel_5[] = {
	 15,  15,  17,  17,  14,   2,   8,   8,  11,  11,  13,  13,  14,   6,  16,  16,
	 11,  11,  13,  13,   5,  12,   9,  16,  11,  18,  13,  13,   5,  12,   9,  10,
};

const Uint8 atck1e_sel_6[] = {
	 15,  15,   1,  13,  14,   2,   8,   8,  11,  11,  13,  13,  14,   6,  16,  16,
	 11,  11,  13,  13,   5,  12,   9,  16,  11,  18,  17,  17,   5,  12,   9,  10,
};

const Uint8 atck1e_sel_7[] = {
	 15,  15,  13,  13,  14,   2,   8,   8,  11,  11,  13,  13,  14,   6,  16,  16,
	 11,  11,  13,  13,   5,  12,   9,  16,  11,  18,  17,  17,   5,  12,   9,  10,
};

const Uint8 *const atck1e_cmd[] = {
	atck1e_cmd_00, atck1e_cmd_01, atck1e_cmd_02, atck1e_cmd_03, atck1e_cmd_04, atck1e_cmd_05,
	atck1e_cmd_06, atck1e_cmd_07, atck1e_cmd_08, atck1e_cmd_09, atck1e_cmd_10, atck1e_cmd_11,
	atck1e_cmd_12, atck1e_cmd_13, atck1e_cmd_14, atck1e_cmd_15, atck1e_cmd_16, atck1e_cmd_17,
	atck1e_cmd_18, atck1e_cmd_19, atck1e_cmd_20, atck1e_cmd_21, atck1e_cmd_22, atck1e_cmd_23,
	atck1e_cmd_24, atck1e_cmd_25, atck1e_cmd_26, atck1e_cmd_27, atck1e_cmd_28, atck1e_cmd_29,
	atck1e_cmd_30, atck1e_cmd_31,
};

const Uint8 atck1e_cmd_00[] = {
	STAND,
	WAIT2( 10 ),
	WALK_B( 512 ),
	WAIT2( 8 ),
	SP_CMD( pl1e_sp_hadou, 0, PK_PUNCH, LV_LIGHT, 0 ),
	IF_LEVEL( 24 ),
		IF_GAUGE( 3 ),
			IF_RND( 6 ),
				IF_LEVEL( 28 ),
					WAIT2( 8 ),
					SP_CMD_SET( pl1e_sp_gou_hadou, 0, PK_PUNCH, LV_LIGHT, 0, 1, 20 ),
					SP_CMD( pl1e_sp_hadou, 0, PK_PUNCH, LV_HEAVY, 0 ),
				CMD_ELSE,
					WAIT2( 8 ),
					SP_CMD( pl1e_sp_tenma, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ENDIF,
			CMD_ELSE,
				WAIT2( 14 ),
				SP_CMD( pl1e_sp_hadou, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ENDIF,
		CMD_ELSE,
			WAIT2( 14 ),
			SP_CMD( pl1e_sp_hadou, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ENDIF,
	CMD_ELSE,
		WAIT2( 14 ),
		SP_CMD( pl1e_sp_hadou, 0, PK_PUNCH, LV_HEAVY, 0 ),
		WAIT2( 14 ),
		SP_CMD( pl1e_sp_hadou, 0, PK_PUNCH, LV_HEAVY, 0 ),
		WAIT2( 14 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck1e_cmd_01[] = {
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
				PUNCH( LV_HEAVY ),
				SJUMP3( DIR_UP ),
				CANCEL_CLR,
			CMD_ENDIF,
			SJUMP_FLAG_SET,
				CMD_JUMP( 4 ),
				JUMP_FLAG_CLR,
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

const Uint8 atck1e_cmd_02[] = {
	LOCK_SET,
	STAND,
	WAIT2( 8 ),
	TIMER_SET( 60 ),
	WALK_EM( 64 ),
	CMD_JUMP( 11 ),
	STAND,
	WAIT2( 8 ),
	LOCK_CLR,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1e_cmd_03[] = {
	LOCK_SET,
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
	LOCK_CLR,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1e_cmd_04[] = {
	STAND,
	LOCK_SET,
	WAIT2( 14 ),
	TIMER_SET( 60 ),
	IF_EM_ATCK( ATCK_CROUCH_HP ),
		SP_CMD( pl1e_sp_shouryuu, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ELSE,
		SP_CMD( pl1e_sp_hadou, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ENDIF,
	STAND,
	LOCK_CLR,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1e_cmd_05[] = {
	STAND,
	WAIT2( 4 ),
	DASH_ATCK( ATCK_CROUCH_LK ),
		CROUCH,
		IF_LEVEL( 24 ),
			IF_EM_ATCK( ATCK_CROUCH_HP ),
				CROUCH,
				CANCEL_SET2,
					KICK( LV_LIGHT ),
					PUNCH( LV_HEAVY ),
					CANCEL_CLR,
				CMD_ENDIF,
				SJUMP3( DIR_FORWARD ),
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
					PARTNER_CALL,
					SP_CMD( pl1e_sp_hadou, 0, PK_PUNCH, LV_HEAVY, 0 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 16 ),
				CANCEL_SET2,
					KICK( LV_LIGHT ),
					STAND,
					PUNCH( LV_HEAVY ),
					SP_CMD( pl1e_sp_hadou, 0, PK_PUNCH, LV_LIGHT, 0 ),
					CANCEL_CLR,
				CMD_ENDIF,
			CMD_ELSE,
				KICK( LV_HEAVY ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		PARTNER_CALL,
	CMD_ENDIF,
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1e_cmd_06[] = {
	STAND,
	WAIT2( 4 ),
	CMD_JUMP( 0 ),
	IF_AREA( 64, 64, 48, 32 ),
		WAIT2( 4 ),
		SP_CMD( pl1e_sp_hadou, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1e_cmd_07[] = {
	STAND,
	WAIT2( 4 ),
	SP_CMD( pl1e_sp_tatsumaki, 0, PK_PUNCH, LV_HEAVY, 0 ),
	STAND,
	WAIT2( 2 ),
	IF_EM_ATCK( ATCK_CROUCH_HK ),
		CROUCH,
		KICK( LV_HEAVY ),
	CMD_ENDIF,
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1e_cmd_08[] = {
	STAND,
	LOCK_SET,
	IF_GAUGE( 2 ),
		IF_RND( 16 ),
			WAIT2( 2 ),
			IF_AREA( 240, 48, 128, 48 ),
				SP_CMD( pl1e_sp_gou_hadou, 0, PK_PUNCH, LV_LIGHT, 0 ),
				IF_RND( 16 ),
					CMD_JUMP( 10 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		STAND,
		WAIT2( 2 ),
		PARTNER_CALL,
		JUMP( DIR_BACK ),
		WAIT( 16 ),
		JUMP_FLAG_SET,
			SP_CMD( pl1e_sp_zankuu, 0, PK_PUNCH, LV_HEAVY, 0 ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1e_cmd_09[] = {
	STAND,
	LOCK_SET,
	IF_GAUGE( 1 ),
		IF_RND( 16 ),
			IF_EM_ATCK( ATCK_CROUCH_HK ),
				CMD_JUMP( 21 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1e_cmd_10[] = {
	STAND,
	LOCK_SET,
	WAIT2( 2 ),
	IF_RND( 16 ),
		IF_GAUGE( 5 ),
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
	LOCK_CLR,
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1e_cmd_11[] = {
	STAND,
	LOCK_SET,
	DASH_B( 96 ),
	STAND,
	WAIT2( 2 ),
	IF_AREA( 128, 64, 128, 16 ),
		SP_CMD( pl1e_sp_hadou, 0, PK_PUNCH, LV_HEAVY, 0 ),
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
			SP_CMD( pl1e_sp_hadou, 0, PK_PUNCH, LV_HEAVY, 0 ),
			STAND,
			CMD_JUMP( 24 ),
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1e_cmd_12[] = {
	LOCK_SET,
	STAND,
	WAIT2( 2 ),
	CMD_JUMP( 14 ),
	JUMP_FLAG_SET,
		CMD_JUMP( 20 ),
		WAIT( 6 ),
		SP_CMD( pl1e_sp_jtatsumaki, 0, PK_PUNCH, LV_HEAVY, 0 ),
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	LOCK_CLR,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1e_cmd_13[] = {
	STAND,
	LOCK_SET,
	WAIT2( 6 ),
	CMD_JUMP( 0 ),
	CMD_JUMP( 25 ),
	CMD_JUMP( 14 ),
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

const Uint8 atck1e_cmd_14[] = {
	STAND,
	WAIT2( 2 ),
	IF_EM_STANCE( STANCE_AIR ),
	CMD_ELSE,
		IF_DIST( 32, 240, 48, 0 ),
			JUMP( DIR_BACK ),
			JUMP_FLAG_SET,
				CMD_JUMP( 20 ),
				TIMER_SET( 60 ),
				IF_HEIGHT( 1, 128 ),
					SP_CMD( pl1e_sp_kuujin, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ENDIF,
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			IF_DIST( 64, 240, 64, 0 ),
				JUMP( DIR_UP ),
				JUMP_FLAG_SET,
					CMD_JUMP( 20 ),
					TIMER_SET( 60 ),
					IF_HEIGHT( 1, 128 ),
						SP_CMD( pl1e_sp_kuujin, 0, PK_PUNCH, LV_LIGHT, 0 ),
					CMD_ENDIF,
					JUMP_FLAG_CLR,
				CMD_ENDIF,
			CMD_ELSE,
				JUMP( DIR_FORWARD ),
				JUMP_FLAG_SET,
					CMD_JUMP( 20 ),
					TIMER_SET( 60 ),
					IF_HEIGHT( 1, 128 ),
						SP_CMD( pl1e_sp_kuujin, 0, PK_PUNCH, LV_LIGHT, 0 ),
					CMD_ENDIF,
					JUMP_FLAG_CLR,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1e_cmd_15[] = {
	STAND,
	WAIT2( 2 ),
	IF_DIST( 64, 240, 64, 0 ),
		JUMP( DIR_BACK ),
		JUMP_FLAG_SET,
			CMD_JUMP( 20 ),
			TIMER_SET( 60 ),
			IF_HEIGHT( 1, 128 ),
				SP_CMD( pl1e_sp_zankuu, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		JUMP( DIR_UP ),
		JUMP_FLAG_SET,
			CMD_JUMP( 20 ),
			TIMER_SET( 60 ),
			IF_HEIGHT( 1, 128 ),
				SP_CMD( pl1e_sp_zankuu, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1e_cmd_16[] = {
	STAND,
	WAIT2( 2 ),
	CMD_JUMP( 37 ),
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1e_cmd_17[] = {
	STAND,
	WAIT2( 2 ),
	IF_DIST( 240, 64, 128, 0 ),
		SP_CMD( pl1e_sp_ashura, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ELSE,
		SP_CMD( pl1e_sp_ashura_b, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ENDIF,
	STAND,
	CMD_EXIT,
};

const Uint8 atck1e_cmd_18[] = {
	STAND,
	LOCK_SET,
	IF_GAUGE( 3 ),
		IF_RND( 18 ),
			WAIT2( 2 ),
			IF_AREA( 64, 24, 64, 24 ),
				SP_CMD( pl1e_sp_shungoku, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				PARTNER_CALL,
				SJUMP( DIR_BACK ),
				WAIT( 24 ),
				SP_CMD( pl1e_sp_zankuu, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_DIST( 64, 32, 80, 32 ),
				PARTNER_CALL,
				WAIT2( 2 ),
				JUMP( DIR_FORWARD ),
				WAIT( 32 ),
				SP_CMD( pl1e_sp_tenma, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				PARTNER_CALL,
				DASH_B( 128 ),
				IF_RND( 12 ),
					SP_CMD( pl1e_sp_gou_hadou, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ELSE,
					SP_CMD( pl1e_sp_hadou, 0, PK_PUNCH, LV_HEAVY, 0 ),
					IF_RND( 8 ),
						SP_CMD( pl1e_sp_hadou, 0, PK_PUNCH, LV_HEAVY, 0 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		DASH_ATCK( ATCK_CROUCH_LK ),
			IF_EM_ATCK( ATCK_CROUCH_LK ),
				CROUCH,
				KICK( LV_LIGHT ),
				IF_RND( 16 ),
					SP_CMD( pl1e_sp_tatsumaki, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ELSE,
					SP_CMD( pl1e_sp_shouryuu, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ENDIF,
			CMD_ELSE,
				SP_CMD( pl1e_sp_tatsumaki, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	STAND,
	CMD_EXIT,
};

const Uint8 atck1e_cmd_19[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1e_cmd_20[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1e_cmd_21[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1e_cmd_22[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1e_cmd_23[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1e_cmd_24[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1e_cmd_25[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1e_cmd_26[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1e_cmd_27[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1e_cmd_28[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1e_cmd_29[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1e_cmd_30[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1e_cmd_31[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

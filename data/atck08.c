/* Psylocke (character 08): the computer's attack lists.
   What it does once a waiting list's CMD_NEXT has sent it to attack. PLAYER_WORK's cpu_adrs[1]
   points here.

   atck08_tbl     one record (CPU_DATA): the offsets of the three tables.
   atck08_rate    the chance, out of 32, that a list's CMD_NEXT2 starts another attack list, by
                  the level of play.
   atck08_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   atck08_cmd     the lists: a pick is a number in this table.
   atck08_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                  list of lib08.c. */
#include "player.h"
#include "pl08_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const atck08_sel[], *const atck08_cmd[];
extern const Uint8 atck08_rate[], atck08_sel_0[], atck08_sel_1[], atck08_sel_2[], atck08_sel_3[],
	atck08_sel_4[], atck08_sel_5[], atck08_sel_6[], atck08_sel_7[], atck08_cmd_00[], atck08_cmd_01[],
	atck08_cmd_02[], atck08_cmd_03[], atck08_cmd_04[], atck08_cmd_05[], atck08_cmd_06[],
	atck08_cmd_07[], atck08_cmd_08[], atck08_cmd_09[], atck08_cmd_10[], atck08_cmd_11[],
	atck08_cmd_12[], atck08_cmd_13[], atck08_cmd_14[], atck08_cmd_15[], atck08_cmd_16[],
	atck08_cmd_17[], atck08_cmd_18[], atck08_cmd_19[], atck08_cmd_20[], atck08_cmd_21[],
	atck08_cmd_22[], atck08_cmd_25[], atck08_cmd_26[], atck08_cmd_29[], atck08_cmd_30[],
	atck08_cmd_31[];

const CPU_DATA atck08_tbl[] = {
	{ ( Sint32 )atck08_rate, ( Sint32 )atck08_sel, ( Sint32 )atck08_cmd },
};

const Uint8 atck08_rate[] = {
	 14,  15,  16,  17,  18,  19,  20,  21,  22,  22,  23,  23,  24,  24,  25,  25,
	 26,  26,  27,  27,  28,  28,  29,  29,  30,  30,  30,  30,  31,  31,  31,  31,
};

const Uint8 *const atck08_sel[] = {
	atck08_sel_0, atck08_sel_1, atck08_sel_2, atck08_sel_3, atck08_sel_4, atck08_sel_5,
	atck08_sel_6, atck08_sel_7,
};

const Uint8 atck08_sel_0[] = {
	  0,   0,   5,   0,   0,   0,   0,   0,  31,   6,   0,   0,  21,   0,  11,   0,
	  1,   1,   1,   1,   1,   1,   1,   1,  11,   8,   0,   3,   3,   3,   3,   3,
};

const Uint8 atck08_sel_1[] = {
	  0,   0,   1,   1,   2,   2,   4,   4,   5,   5,   5,   5,   6,   6,   6,   6,
	  7,   7,   7,   7,   8,   7,   7,   7,  11,  31,   7,   7,   7,  19,  19,   9,
};

const Uint8 atck08_sel_2[] = {
	  5,   5,   5,   7,   7,   4,   4,   8,   9,   5,   0,   6,   6,   6,  19,  19,
	  5,   6,   6,  10,  10,  10,  19,  19,  31,  11,  11,  14,  14,  14,  15,   7,
};

const Uint8 atck08_sel_3[] = {
	  1,   5,   7,  11,  11,  11,  19,  19,   4,   4,   1,   6,   6,   6,  19,  19,
	 10,  10,  19,   5,   5,   5,   7,   7,  31,  11,   7,   9,   9,   5,  16,  16,
};

const Uint8 atck08_sel_4[] = {
	  4,   4,   5,   5,   7,  10,  19,  19,   6,   8,   6,   6,  28,  10,  17,  19,
	 11,  20,  14,  14,  14,  14,  17,  28,  31,  11,  31,  16,  16,  19,   5,   9,
};

const Uint8 atck08_sel_5[] = {
	  4,   4,   5,   6,   7,  11,  10,  28,  28,  20,   6,   8,  14,  24,  17,  19,
	 20,   6,  28,  14,  29,  14,  14,  19,  31,  25,  13,  31,  11,  25,  16,   9,
};

const Uint8 atck08_sel_6[] = {
	  4,   4,   5,   6,   7,  31,  11,  28,  14,  20,   6,   8,  14,  24,  17,  19,
	 20,  28,  28,  14,  29,  14,  14,   8,  31,  25,  13,  31,  11,  25,  16,   9,
};

const Uint8 atck08_sel_7[] = {
	  4,   4,   5,   6,   7,  31,  11,  31,   8,  20,  28,  28,  14,  24,  17,  19,
	 30,  28,   8,  14,  14,  14,  14,  29,  31,  25,  13,  31,  11,  25,  16,   9,
};

const Uint8 *const atck08_cmd[] = {
	atck08_cmd_00, atck08_cmd_01, atck08_cmd_02, atck08_cmd_03, atck08_cmd_04, atck08_cmd_05,
	atck08_cmd_06, atck08_cmd_07, atck08_cmd_08, atck08_cmd_09, atck08_cmd_10, atck08_cmd_11,
	atck08_cmd_12, atck08_cmd_13, atck08_cmd_14, atck08_cmd_15, atck08_cmd_16, atck08_cmd_17,
	atck08_cmd_18, atck08_cmd_19, atck08_cmd_20, atck08_cmd_21, atck08_cmd_22, atck08_cmd_22,
	atck08_cmd_22, atck08_cmd_25, atck08_cmd_26, atck08_cmd_26, atck08_cmd_26, atck08_cmd_29,
	atck08_cmd_30, atck08_cmd_31,
};

const Uint8 atck08_cmd_00[] = {
	LOCK_SET,
	STAND,
	DASH_B( 1024 ),
	WAIT2( 4 ),
	SP_CMD( pl08_sp_blast, 0, PK_PUNCH, LV_HEAVY, 0 ),
	WAIT( 30 ),
	WAIT2( 14 ),
	SP_CMD( pl08_sp_blast, 0, PK_PUNCH, LV_HEAVY, 0 ),
	IF_LEVEL( 27 ),
	CMD_ELSE,
		WAIT( 30 ),
		WAIT2( 14 ),
		SP_CMD( pl08_sp_blast, 0, PK_PUNCH, LV_HEAVY, 0 ),
		STAND,
	CMD_ENDIF,
	LOCK_CLR,
	WAIT2( 14 ),
	IF_RND( 1 ),
		SP_CMD( pl08_sp_chouhatsu, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ENDIF,
	STAND,
	CMD_EXIT,
};

const Uint8 atck08_cmd_01[] = {
	LOCK_SET,
	STAND,
	WALK_ATCK( ATCK_CROUCH_HK ),
		CROUCH,
		WAIT2( 4 ),
		KICK( LV_HEAVY ),
	CMD_ENDIF,
	DASH_B( 96 ),
	STAND,
	WAIT2( 4 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 atck08_cmd_02[] = {
	STAND,
	WAIT2( 4 ),
	JUMP( DIR_FORWARD ),
	JUMP_FLAG_SET,
		WAIT( 38 ),
		TIMER_SET( 60 ),
		IF_AREA( 48, 32, 16, -16 ),
			KICK( LV_HEAVY ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	WAIT2( 14 ),
	JUMP( DIR_FORWARD ),
	JUMP_FLAG_SET,
		WAIT( 38 ),
		TIMER_SET( 60 ),
		IF_AREA( 48, 32, 16, -16 ),
			KICK( LV_HEAVY ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	WAIT2( 14 ),
	JUMP( DIR_FORWARD ),
	JUMP_FLAG_SET,
		WAIT( 38 ),
		TIMER_SET( 60 ),
		IF_AREA( 48, 32, 16, -16 ),
			KICK( LV_HEAVY ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	WAIT2( 14 ),
	CMD_EXIT,
};

const Uint8 atck08_cmd_03[] = {
	STAND,
	WAIT2( 10 ),
	IF_LEVEL( 8 ),
	CMD_ELSE,
		SP_CMD( pl08_sp_blast, 0, PK_PUNCH, LV_HEAVY, 0 ),
		STAND,
		WAIT2( 4 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck08_cmd_04[] = {
	LOCK_SET,
	JUMP( DIR_BACK ),
	STAND,
	WAIT2( 4 ),
	CMD_JUMP( 16 ),
	STAND,
	DASH_B( 128 ),
	STAND,
	WAIT2( 4 ),
	LOCK_CLR,
	CMD_JUMP( 59 ),
	CMD_EXIT,
};

const Uint8 atck08_cmd_05[] = {
	LOCK_SET,
	STAND,
	WALK_EM( 96 ),
	NAG_VFLIP_ADJ4( LV_HEAVY, LEVER_FORWARD ),
	IF_LEVEL( 16 ),
		IF_RND( 16 ),
			IF_LEVEL( 24 ),
				SP_CMD_SET( pl08_sp_butterfly, 0, PK_PUNCH, LV_LIGHT, 0, 1, 20 ),
				SP_CMD( pl08_sp_spin, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				SP_CMD( pl08_sp_spin, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ENDIF,
		CMD_ELSE,
			SP_CMD( pl08_sp_blast, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	WAIT2( 4 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 atck08_cmd_06[] = {
	LOCK_SET,
	STAND,
	WAIT2( 6 ),
	CMD_JUMP( 32 ),
	JUMP_FLAG_SET,
		WAIT( 30 ),
		TIMER_SET( 60 ),
		IF_AREA( 32, 16, 16, -8 ),
			KICK( LV_HEAVY ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	WAIT2( 8 ),
	IF_EM_ATCK( ATCK_CROUCH_LK ),
		CANCEL_SET2,
			STAND,
			KICK( LV_LIGHT ),
			WAIT2( 10 ),
			SP_CMD( pl08_sp_blast, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		IF_EM_ATCK( ATCK_STAND_HP ),
			STAND,
			PUNCH( LV_HEAVY ),
		CMD_ELSE,
			IF_EM_ATCK( ATCK_CROUCH_HK ),
				CROUCH,
				KICK( LV_HEAVY ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT2( 6 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 atck08_cmd_07[] = {
	LOCK_SET,
	STAND,
	WAIT2( 2 ),
	IF_LEVEL( 13 ),
		IF_DIST( 64, 8, 128, 1 ),
			JUMP( DIR_UP ),
			JUMP_FLAG_SET,
				JUMP_LEVEL( 48, 7, 49, 13, 50, 21, 51 ),
				WAIT( 35 ),
				SP_CMD( pl08_sp_blast, 0, PK_PUNCH, LV_LIGHT, 0 ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		JUMP( DIR_UP ),
		JUMP_FLAG_SET,
			JUMP_LEVEL( 48, 7, 49, 13, 50, 21, 51 ),
			WAIT( 35 ),
			SP_CMD( pl08_sp_blast, 0, PK_PUNCH, LV_LIGHT, 0 ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	LOCK_CLR,
	WAIT2( 4 ),
	CMD_EXIT,
};

const Uint8 atck08_cmd_08[] = {
	LOCK_SET,
	DASH_ATCK( 64 ),
	CMD_ENDIF,
	CROUCH,
	WAIT2( 4 ),
	IF_LEVEL( 16 ),
		SP_CMD( pl08_sp_spin, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ELSE,
		SP_CMD( pl08_sp_spin, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ENDIF,
	STAND,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 atck08_cmd_09[] = {
	LOCK_SET,
	WAIT2( 4 ),
	IF_GAUGE( 1 ),
		WALK_B( 96 ),
		STAND,
		WAIT( 60 ),
		SP_CMD( pl08_sp_thrust, 0, PK_PUNCH, LV_LIGHT, LEVER_FORWARD ),
	CMD_ENDIF,
	WAIT2( 4 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 atck08_cmd_10[] = {
	LOCK_SET,
	STAND,
	WAIT2( 2 ),
	CMD_JUMP( 2 ),
	STAND,
	WAIT2( 8 ),
	LOCK_CLR,
	CMD_NEXT2,
};

const Uint8 atck08_cmd_11[] = {
	IF_LEVEL( 6 ),
		WAIT2( 6 ),
		WALK_EM( 8 ),
		STAND,
		IF_LEVEL( 30 ),
			IF_CATCH,
				JUMP_LEVEL( 19, 6, 20, 20, 21, 28, 22 ),
				NAG_VFLIP_ADJ( 1, LEVER_BACK ),
			CMD_ELSE,
				WALK_F( 16 ),
				JUMP_LEVEL( 19, 6, 20, 20, 21, 28, 22 ),
				NAG_VFLIP_ADJ( 1, LEVER_BACK ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_CATCH,
				JUMP_LEVEL( 19, 6, 20, 20, 21, 28, 22 ),
				NAG_VFLIP_ADJ( 1, LEVER_BACK ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	WAIT2( 6 ),
	CMD_NEXT2,
};

const Uint8 atck08_cmd_12[] = {
	STAND,
	CMD_NEXT2,
};

const Uint8 atck08_cmd_13[] = {
	LOCK_SET,
	IF_LEVEL( 30 ),
		IF_GAUGE( 1 ),
			WALK_DIST( 112 ),
			STAND,
			WAIT2( 6 ),
			NAG_VFLIP_ADJ4( LV_HEAVY, LEVER_FORWARD ),
			WAIT2( 2 ),
			STAND,
			WAIT2( 8 ),
			SP_CMD( pl08_sp_maelstrom, 0, PK_PUNCH, LV_LIGHT, 0 ),
			WAIT2( 14 ),
		CMD_ENDIF,
		WAIT2( 8 ),
	CMD_ELSE,
		WALK_DIST( 96 ),
		STAND,
		WAIT2( 6 ),
		NAG_VFLIP_ADJ4( LV_HEAVY, LEVER_FORWARD ),
		STAND,
		WAIT2( 8 ),
		IF_RND( 16 ),
			CROUCH,
			KICK( LV_HEAVY ),
		CMD_ELSE,
			JUMP_LEVEL( 30, 10, 29, 17, 29, 26, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 atck08_cmd_14[] = {
	LOCK_SET,
	STAND,
	WAIT2( 4 ),
	DASH_EM( 32 ),
	JUMP_LEVEL( 30, 22, 29, 26, 29, 31, 0 ),
	STAND,
	WAIT2( 4 ),
	CMD_JUMP( 0 ),
	CMD_JUMP( 59 ),
	LOCK_CLR,
	CMD_NEXT2,
};

const Uint8 atck08_cmd_15[] = {
	CMD_NEXT2,
};

const Uint8 atck08_cmd_16[] = {
	STAND,
	WAIT2( 4 ),
	IF_GAUGE( 2 ),
		WALK_B( 64 ),
		WAIT2( 6 ),
		JUMP( DIR_BACK ),
		WAIT( 25 ),
		SP_CMD( pl08_sp_thrust, 0, PK_PUNCH, LV_LIGHT, LEVER_DOWN | LEVER_FORWARD ),
		WAIT2( 4 ),
		SP_CMD( pl08_sp_butterfly, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 4 ),
	CMD_EXIT,
};

const Uint8 atck08_cmd_17[] = {
	STAND,
	WAIT2( 2 ),
	SP_CMD( pl08_sp_ninjitsu, 0, PK_KICK, LV_LIGHT, 0 ),
	STAND,
	JUMP_LEVEL( 30, 16, 29, 24, 29, 31, 0 ),
	STAND,
	WAIT2( 4 ),
	CMD_NEXT2,
};

const Uint8 atck08_cmd_18[] = {
	CMD_NEXT2,
};

const Uint8 atck08_cmd_19[] = {
	STAND,
	WAIT2( 6 ),
	SP_CMD( pl08_sp_blast, 0, PK_PUNCH, LV_HEAVY, 0 ),
	STAND,
	WAIT2( 4 ),
	CMD_NEXT2,
};

const Uint8 atck08_cmd_20[] = {
	CROUCH,
	WAIT2( 6 ),
	TIMER_SET( 20 ),
	IF_DIST( 64, 64, 104, 160 ),
		JUMP( DIR_FORWARD ),
		CMD_JUMP( 10 ),
		STAND,
	CMD_ENDIF,
	WAIT2( 4 ),
	CMD_NEXT2,
};

const Uint8 atck08_cmd_21[] = {
	CMD_JUMP( 63 ),
	CMD_NEXT2,
};

const Uint8 atck08_cmd_22[] = {
	STAND,
	WAIT2( 4 ),
	IF_GAUGE( 1 ),
		WAIT2( 8 ),
		JUMP_LEVEL( 48, 7, 49, 13, 50, 21, 51 ),
		IF_RND( 16 ),
			TIMER2_SET( 120 ),
		CMD_ELSE,
			TIMER2_SET( 30 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_RND( 12 ),
			TIMER2_SET( 20 ),
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP( DIR_FORWARD ),
	JUMP_FLAG_SET,
		WAIT( 40 ),
		SP_CMD( pl08_sp_butterfly, 0, PK_PUNCH, LV_LIGHT, 0 ),
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	CMD_ENDIF,
	WAIT2( 6 ),
	STAND,
	CMD_NEXT2,
};

const Uint8 atck08_cmd_25[] = {
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
	CMD_JUMP( 59 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck08_cmd_26[] = {
	LOCK_SET,
	STAND,
	WAIT2( 6 ),
	IF_DIST( 64, 64, 96, 96 ),
		JUMP_RND( 37, 37, 37, 5 ),
		JUMP_FLAG_SET,
			JUMP_LEVEL( 24, 16, 24, 20, 23, 31, 6 ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 32 ),
		JUMP_FLAG_SET,
			JUMP_LEVEL( 48, 7, 49, 13, 50, 21, 51 ),
			WAIT( 30 ),
			IF_RND( 10 ),
				JUMP_LEVEL( 27, 18, 26, 26, 25, 31, 15 ),
			CMD_ELSE,
				JUMP_LEVEL( 27, 7, 26, 13, 25, 21, 58 ),
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
		STAND,
		JUMP_LEVEL( 30, 4, 29, 13, 29, 29, 0 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 6 ),
	LOCK_CLR,
};

const Uint8 atck08_cmd_29[] = {
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
		CMD_ELSE,
			PARTNER_CALL,
		CMD_ENDIF,
	CMD_ELSE,
		WAIT( 1 ),
	CMD_ENDIF,
	STAND,
	CMD_JUMP( 59 ),
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck08_cmd_30[] = {
	STAND,
	IF_LEVEL( 20 ),
		IF_RND( 24 ),
			WAIT2( 8 ),
			CMD_JUMP( 31 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 13 ),
			IF_RND( 12 ),
				WAIT2( 8 ),
				CMD_JUMP( 31 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_RND( 6 ),
				WAIT2( 8 ),
				CMD_JUMP( 31 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
};

const Uint8 atck08_cmd_31[] = {
	LOCK_SET,
	STAND,
	IF_LEVEL( 18 ),
		WAIT2( 4 ),
		CMD_JUMP( 32 ),
		JUMP_FLAG_SET,
			JUMP_LEVEL( 27, 8, 26, 16, 25, 29, 15 ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
		STAND,
		JUMP_LEVEL( 28, 12, 28, 24, 13, 31, 13 ),
		SJUMP_FLAG_SET,
			IF_RND( 2 ),
				JUMP_LEVEL( 24, 26, 24, 29, 23, 31, 6 ),
			CMD_ELSE,
				JUMP_LEVEL( 40, 26, 39, 29, 24, 31, 24 ),
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
		STAND,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_JUMP( 59 ),
	WAIT2( 6 ),
	CMD_NEXT2,
};

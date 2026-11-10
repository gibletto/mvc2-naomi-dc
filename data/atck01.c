/* Zangief (character 01): the computer's attack lists.
   What it does once a waiting list's CMD_NEXT has sent it to attack. PLAYER_WORK's cpu_adrs[1]
   points here.

   atck01_tbl     one record (CPU_DATA): the offsets of the three tables.
   atck01_rate    the chance, out of 32, that a list's CMD_NEXT2 starts another attack list, by
                  the level of play.
   atck01_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   atck01_cmd     the lists: a pick is a number in this table.
   atck01_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                  list of lib01.c. */
#include "player.h"
#include "pl01_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const atck01_sel[], *const atck01_cmd[];
extern const Uint8 atck01_rate[], atck01_sel_0[], atck01_sel_1[], atck01_sel_2[], atck01_sel_3[],
	atck01_sel_4[], atck01_sel_5[], atck01_sel_6[], atck01_sel_7[], atck01_cmd_00[], atck01_cmd_01[],
	atck01_cmd_02[], atck01_cmd_03[], atck01_cmd_04[], atck01_cmd_05[], atck01_cmd_06[],
	atck01_cmd_07[], atck01_cmd_08[], atck01_cmd_09[], atck01_cmd_10[], atck01_cmd_11[],
	atck01_cmd_12[], atck01_cmd_13[], atck01_cmd_14[], atck01_cmd_15[], atck01_cmd_16[],
	atck01_cmd_17[], atck01_cmd_18[], atck01_cmd_19[], atck01_cmd_20[], atck01_cmd_21[],
	atck01_cmd_22[], atck01_cmd_23[], atck01_cmd_24[], atck01_cmd_25[], atck01_cmd_26[],
	atck01_cmd_27[], atck01_cmd_28[], atck01_cmd_29[], atck01_cmd_30[], atck01_cmd_31[];

const CPU_DATA atck01_tbl[] = {
	{ ( Sint32 )atck01_rate, ( Sint32 )atck01_sel, ( Sint32 )atck01_cmd },
};

const Uint8 atck01_rate[] = {
	 10,  10,  13,  13,  15,  15,  17,  17,  18,  19,  20,  21,  22,  23,  24,  25,
	 25,  26,  27,  27,  27,  27,  27,  27,  28,  28,  29,  29,  30,  30,  31,  31,
};

const Uint8 *const atck01_sel[] = {
	atck01_sel_0, atck01_sel_1, atck01_sel_2, atck01_sel_3, atck01_sel_4, atck01_sel_5,
	atck01_sel_6, atck01_sel_7,
};

const Uint8 atck01_sel_0[] = {
	  3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,
	  7,   7,   7,   7,   7,   7,   7,   7,   6,   6,   7,   7,   7,   7,   7,   7,
};

const Uint8 atck01_sel_1[] = {
	  3,   3,   3,   3,   1,   1,   3,   4,   3,   3,   3,   3,   1,   1,   3,   4,
	  6,   6,   6,   6,   1,   1,   3,   4,   0,   0,   0,   0,   1,   1,   3,   4,
};

const Uint8 atck01_sel_2[] = {
	  7,   7,   1,   1,   2,   6,   3,   3,   7,   7,   1,   1,   2,   6,   3,   3,
	  7,   7,   1,   1,   2,   6,   3,   3,   7,   7,  28,   1,   2,   6,   3,   3,
};

const Uint8 atck01_sel_3[] = {
	 12,   9,   1,   1,   2,   6,   3,   8,  12,   9,   1,   1,   2,   6,   3,   8,
	 12,   9,   1,   1,   2,   6,   3,   8,  12,   9,  13,  13,   2,   6,   3,   8,
};

const Uint8 atck01_sel_4[] = {
	  9,   9,   1,   1,   3,   2,   8,   8,   9,   9,   1,   1,   3,   6,   8,   8,
	  5,   0,  13,  13,   1,  12,   8,  10,  28,   0,  13,  13,   1,  12,   8,  10,
};

const Uint8 atck01_sel_5[] = {
	  9,   9,   1,   1,   3,   2,   8,   8,   9,   9,   1,  13,   3,   6,   8,   8,
	  5,  11,  13,  13,   1,  12,   8,  10,  28,  28,  13,  13,   1,  12,   8,  10,
};

const Uint8 atck01_sel_6[] = {
	  9,   9,   1,  13,   3,   2,   8,   8,   9,   9,  13,  13,   3,   6,   8,   8,
	 11,  11,  13,  13,   1,  12,   8,  10,  28,  28,  28,  13,   1,  12,   8,  10,
};

const Uint8 atck01_sel_7[] = {
	  5,   5,  13,  13,   2,   2,   8,   8,   9,   9,  13,  13,   5,   6,   8,   8,
	  9,   9,  13,  13,   1,  12,   8,  10,  12,  28,  28,  28,  12,  28,  28,  10,
};

const Uint8 *const atck01_cmd[] = {
	atck01_cmd_00, atck01_cmd_01, atck01_cmd_02, atck01_cmd_03, atck01_cmd_04, atck01_cmd_05,
	atck01_cmd_06, atck01_cmd_07, atck01_cmd_08, atck01_cmd_09, atck01_cmd_10, atck01_cmd_11,
	atck01_cmd_12, atck01_cmd_13, atck01_cmd_14, atck01_cmd_15, atck01_cmd_16, atck01_cmd_17,
	atck01_cmd_18, atck01_cmd_19, atck01_cmd_20, atck01_cmd_21, atck01_cmd_22, atck01_cmd_23,
	atck01_cmd_24, atck01_cmd_25, atck01_cmd_26, atck01_cmd_27, atck01_cmd_28, atck01_cmd_29,
	atck01_cmd_30, atck01_cmd_31,
};

const Uint8 atck01_cmd_00[] = {
	STAND,
	WAIT2( 2 ),
	TIMER_SET( 60 ),
	WALK_EM( 80 ),
	STAND,
	TIMER_SET( 0xb4 ),
	IF_DIST( 64, 48, 32, 0 ),
		IF_LEVEL( 24 ),
			WAIT2( 2 ),
		CMD_ELSE,
			WAIT2( 8 ),
		CMD_ENDIF,
		STAND,
		SP_CMD( pl01_sp_screw, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ELSE,
		WALK_F( 16 ),
		IF_LEVEL( 16 ),
			WAIT2( 2 ),
			STAND,
			SP_CMD( pl01_sp_screw, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ELSE,
			WAIT2( 8 ),
			STAND,
			SP_CMD( pl01_sp_screw, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck01_cmd_01[] = {
	STAND,
	WAIT2( 8 ),
	CMD_JUMP( 14 ),
	JUMP_FLAG_SET,
		CMD_JUMP( 20 ),
		TIMER_SET( 60 ),
		IF_HEIGHT( 0, 64 ),
			CMD_JUMP( 9 ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	WAIT2( 8 ),
	IF_LEVEL( 24 ),
		IF_RND( 20 ),
			SP_CMD( pl01_sp_powerbomb, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 16 ),
			IF_RND( 20 ),
				SP_CMD( pl01_sp_powerbomb, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck01_cmd_02[] = {
	STAND,
	WAIT2( 4 ),
	IF_AREA( 240, 48, 144, 16 ),
		SP_CMD( pl01_sp_powerbomb, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ENDIF,
	IF_LEVEL( 24 ),
		IF_GAUGE( 3 ),
			IF_RND( 18 ),
				TIMER_SET( 10 ),
				IF_AREA( 48, 48, 48, 48 ),
					SP_CMD( pl01_sp_uab, 0, PK_PUNCH, LV_LIGHT, 0 ),
					SP_CMD( pl01_sp_blizzard, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ELSE,
					SP_CMD( pl01_sp_fab, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			IF_GAUGE( 1 ),
				SP_CMD( pl01_sp_fab, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 16 ),
			IF_RND( 10 ),
				IF_GAUGE( 1 ),
					SP_CMD( pl01_sp_fab, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 atck01_cmd_03[] = {
	STAND,
	CMD_JUMP( 0 ),
	STAND,
	WAIT2( 2 ),
	TIMER_SET( 60 ),
	WALK_ATCK( ATCK_STAND_HP ),
		STAND,
		WAIT2( 2 ),
		IF_EM_ATCK( ATCK_STAND_HP ),
			STAND,
			PUNCH( LV_HEAVY ),
		CMD_ELSE,
			WAIT2( 2 ),
			IF_EM_ATCK( ATCK_CROUCH_HP ),
				CROUCH,
				PUNCH( LV_HEAVY ),
				WAIT2( 2 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	WAIT2( 4 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck01_cmd_04[] = {
	STAND,
	WAIT2( 14 ),
	TIMER_SET( 40 ),
	IF_EM_ATCK( pl01_at_qlariat ),
		SP_CMD( pl01_sp_qlariat, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ENDIF,
	STAND,
	IF_AREA( 80, 48, 96, 32 ),
		IF_LEVEL( 24 ),
			IF_RND( 10 ),
				SP_CMD( pl01_sp_uab, 0, PK_PUNCH, LV_LIGHT, 0 ),
				SP_CMD( pl01_sp_blizzard, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				SP_CMD( pl01_sp_fab, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 12 ),
				IF_RND( 10 ),
					SP_CMD( pl01_sp_fab, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck01_cmd_05[] = {
	STAND,
	WAIT2( 4 ),
	TIMER_SET( 60 ),
	DASH_ATCK( ATCK_CROUCH_LK ),
		CROUCH,
		IF_LEVEL( 24 ),
			CANCEL_SET,
				PUNCH( LV_LIGHT ),
				IF_EM_ATCK( ATCK_CROUCH_MP ),
					CROUCH,
					PUNCH( LV_LIGHT ),
					SJUMP3( DIR_UP ),
					WAIT( 2 ),
					CMD_JUMP( 4 ),
				CMD_ELSE,
					IF_EM_ATCK( ATCK_CROUCH_HK ),
						CROUCH,
						KICK( LV_HEAVY ),
					CMD_ENDIF,
				CMD_ENDIF,
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			KICK( LV_HEAVY ),
		CMD_ENDIF,
	CMD_ELSE,
		PUNCH( LV_LIGHT ),
	CMD_ENDIF,
	STAND,
	CMD_EXIT,
};

const Uint8 atck01_cmd_06[] = {
	STAND,
	WAIT2( 4 ),
	CMD_JUMP( 0 ),
	LOCK_SET,
	IF_AREA( 64, 64, 16, 16 ),
		WAIT2( 4 ),
		SP_CMD( pl01_sp_banish, 0, PK_PUNCH, LV_HEAVY, 0 ),
		SP_CMD( pl01_sp_vodka, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ENDIF,
	LOCK_CLR,
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck01_cmd_07[] = {
	STAND,
	WAIT2( 4 ),
	SP_CMD( pl01_sp_lariat, 0, PK_PUNCH, LV_LIGHT, LEVER_FORWARD ),
	STAND,
	CMD_EXIT,
};

const Uint8 atck01_cmd_08[] = {
	STAND,
	LOCK_SET,
	IF_GAUGE( 1 ),
		IF_RND( 16 ),
			WAIT2( 2 ),
			IF_AREA( 128, 48, 128, 16 ),
				SP_CMD( pl01_sp_fab, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		STAND,
		WAIT2( 2 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck01_cmd_09[] = {
	STAND,
	LOCK_SET,
	TIMER_SET( 60 ),
	DASH_EM( 128 ),
	STAND,
	JUMP( DIR_FORWARD ),
	JUMP_FLAG_SET,
		CMD_JUMP( 20 ),
		TIMER_SET( 30 ),
		IF_AREA( 64, 16, 80, 0 ),
			KICK( LV_HEAVY ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	IF_RND( 16 ),
		IF_DIST( 96, 48, 48, 0 ),
			SP_CMD( pl01_sp_screw, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ELSE,
			TIMER_SET( 30 ),
			WALK_F( 16 ),
			IF_DIST( 96, 48, 48, 0 ),
				SP_CMD( pl01_sp_screw, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_AREA( 112, 48, 96, 16 ),
			SP_CMD( pl01_sp_powerbomb, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	LOCK_CLR,
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 atck01_cmd_10[] = {
	STAND,
	WAIT2( 2 ),
	IF_RND( 16 ),
		IF_GAUGE( 2 ),
			TIMER_SET( 10 ),
			IF_AREA( 80, 32, 48, 32 ),
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

const Uint8 atck01_cmd_11[] = {
	STAND,
	WAIT2( 2 ),
	IF_AREA( 96, 64, 112, 16 ),
		SP_CMD( pl01_sp_powerbomb, 0, PK_PUNCH, LV_HEAVY, 0 ),
		STAND,
		CMD_JUMP( 24 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	IF_RND( 16 ),
		WAIT2( 2 ),
		STAND,
		CMD_JUMP( 24 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck01_cmd_12[] = {
	STAND,
	WAIT2( 2 ),
	CMD_JUMP( 14 ),
	JUMP_FLAG_SET,
		CMD_JUMP( 20 ),
		WAIT( 6 ),
		SP_CMD( pl01_sp_lariat, 0, PK_PUNCH, LV_LIGHT, LEVER_FORWARD ),
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	CMD_EXIT,
};

const Uint8 atck01_cmd_13[] = {
	STAND,
	WAIT2( 6 ),
	CMD_JUMP( 0 ),
	IF_DIST( 64, 48, 96, 96 ),
		IF_RND( 10 ),
			STAND,
			JUMP( DIR_FORWARD ),
			JUMP_FLAG_SET,
				JUMP_LEVEL( 48, 7, 49, 13, 50, 21, 51 ),
				JUMP_LEVEL( 38, 8, 38, 29, 39, 31, 40 ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
			STAND,
			WAIT2( 4 ),
		CMD_ELSE,
			IF_RND( 20 ),
				DASH_EM( 64 ),
				SP_CMD( pl01_sp_qlariat, 0, PK_PUNCH, LV_LIGHT, LEVER_FORWARD ),
			CMD_ENDIF,
		CMD_ENDIF,
		WAIT2( 2 ),
	CMD_ENDIF,
	IF_RND( 16 ),
		CMD_JUMP( 12 ),
	CMD_ELSE,
		CMD_JUMP( 13 ),
	CMD_ENDIF,
	IF_LEVEL( 10 ),
		JUMP_FLAG_SET,
			CMD_JUMP( 20 ),
			IF_RND( 16 ),
				IF_RND( 12 ),
					JUMP_LEVEL( 38, 13, 39, 21, 40, 31, 41 ),
				CMD_ELSE,
					JUMP_LEVEL( 38, 20, 39, 26, 40, 31, 41 ),
				CMD_ENDIF,
			CMD_ENDIF,
			CMD_JUMP( 35 ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		JUMP_FLAG_SET,
			CMD_JUMP( 20 ),
			CMD_JUMP( 35 ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	CMD_JUMP( 7 ),
	WAIT2( 4 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck01_cmd_14[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck01_cmd_15[] = {
	CMD_EXIT,
};

const Uint8 atck01_cmd_16[] = {
	CMD_EXIT,
};

const Uint8 atck01_cmd_17[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck01_cmd_18[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck01_cmd_19[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck01_cmd_20[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck01_cmd_21[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck01_cmd_22[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck01_cmd_23[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck01_cmd_24[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck01_cmd_25[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck01_cmd_26[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck01_cmd_27[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck01_cmd_28[] = {
	WALK_EM( 144 ),
	JUMP( DIR_FORWARD ),
	JUMP_FLAG_SET,
		WAIT( 30 ),
		NAG_VFLIP_ADJ4( LV_LIGHT, LEVER_DOWN ),
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	IF_LEVEL( 31 ),
		CANCEL_SET2,
			PUNCH( LV_LIGHT ),
			SP_CMD( pl01_sp_blizzard, 0, PK_PUNCH, LV_LIGHT, 0 ),
			SP_CMD( pl01_sp_uab, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 16 ),
			IF_RND( 10 ),
				CROUCH,
				CANCEL_SET2,
					PUNCH( LV_LIGHT ),
					PUNCH( LV_LIGHT ),
					SJUMP3( DIR_UP ),
					CANCEL_CLR,
				CMD_ENDIF,
				SJUMP_FLAG_SET,
					CANCEL_SET2,
						PUNCH( LV_LIGHT ),
						KICK( LV_LIGHT ),
						SP_CMD( pl01_sp_qlariat, 0, PK_PUNCH, LV_LIGHT, LEVER_BACK ),
						PUNCH( LV_HEAVY ),
						CANCEL_CLR,
					CMD_ENDIF,
					JUMP_FLAG_CLR,
				CMD_ENDIF,
				JUMP_EM( 53, 54, 55, 56, 57, 58, 59, 60 ),
				SP_CMD( pl01_sp_fab, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				SP_CMD( pl01_sp_lariat, 0, PK_PUNCH, LV_LIGHT, LEVER_FORWARD ),
			CMD_ENDIF,
		CMD_ELSE,
			SP_CMD( pl01_sp_fab, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck01_cmd_29[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck01_cmd_30[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck01_cmd_31[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

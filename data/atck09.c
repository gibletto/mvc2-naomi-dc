/* Iceman (character 09): the computer's attack lists.
   What it does once a waiting list's CMD_NEXT has sent it to attack. PLAYER_WORK's cpu_adrs[1]
   points here.

   atck09_tbl     one record (CPU_DATA): the offsets of the three tables.
   atck09_rate    the chance, out of 32, that a list's CMD_NEXT2 starts another attack list, by
                  the level of play.
   atck09_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   atck09_cmd     the lists: a pick is a number in this table.
   atck09_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                  list of lib09.c. */
#include "player.h"
#include "pl09_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const atck09_sel[], *const atck09_cmd[];
extern const Uint8 atck09_rate[], atck09_sel_0[], atck09_sel_1[], atck09_sel_2[], atck09_sel_3[],
	atck09_sel_4[], atck09_sel_5[], atck09_sel_6[], atck09_sel_7[], atck09_cmd_00[], atck09_cmd_01[],
	atck09_cmd_02[], atck09_cmd_03[], atck09_cmd_04[], atck09_cmd_05[], atck09_cmd_06[],
	atck09_cmd_07[], atck09_cmd_08[], atck09_cmd_09[], atck09_cmd_10[], atck09_cmd_11[],
	atck09_cmd_12[], atck09_cmd_13[], atck09_cmd_14[], atck09_cmd_15[], atck09_cmd_16[],
	atck09_cmd_17[], atck09_cmd_18[], atck09_cmd_19[], atck09_cmd_20[], atck09_cmd_21[],
	atck09_cmd_22[], atck09_cmd_23[], atck09_cmd_24[], atck09_cmd_25[], atck09_cmd_26[],
	atck09_cmd_27[], atck09_cmd_28[], atck09_cmd_29[], atck09_cmd_30[], atck09_cmd_31[];

const CPU_DATA atck09_tbl[] = {
	{ ( Sint32 )atck09_rate, ( Sint32 )atck09_sel, ( Sint32 )atck09_cmd },
};

const Uint8 atck09_rate[] = {
	  8,   8,   8,   8,   9,   9,  10,  10,  11,  11,  12,  12,  13,  13,  14,  14,
	 15,  15,  16,  16,  17,  17,  18,  18,  19,  20,  21,  22,  24,  26,  28,  30,
};

const Uint8 *const atck09_sel[] = {
	atck09_sel_0, atck09_sel_1, atck09_sel_2, atck09_sel_3, atck09_sel_4, atck09_sel_5,
	atck09_sel_6, atck09_sel_7,
};

const Uint8 atck09_sel_0[] = {
	 16,   0,   0,   0,   1,   2,   3,   3,   0,   0,   0,   0,   1,   2,   3,   3,
	  0,   0,   0,   0,   1,   2,   3,   3,   6,   6,   7,   7,   1,   2,   3,   3,
};

const Uint8 atck09_sel_1[] = {
	 17,   0,   0,   0,   1,   1,   3,   4,   0,   0,   0,   0,   1,   1,   3,   4,
	  0,   0,   0,   0,   1,   1,   3,   4,   0,   0,   0,   0,   1,   1,   3,   4,
};

const Uint8 atck09_sel_2[] = {
	 18,   7,   1,   1,   2,   6,   3,   3,   7,   7,   1,   1,   2,   6,   3,   3,
	  7,   7,   1,   1,   5,   6,   3,   3,   7,   7,   1,   1,   2,   6,   3,   3,
};

const Uint8 atck09_sel_3[] = {
	 12,  11,   1,   1,   2,   6,   3,   8,  12,  11,   1,   1,   2,   6,   3,   8,
	 12,  11,   1,   1,   2,   6,   3,   9,   5,   5,  13,  13,   2,   6,   3,   9,
};

const Uint8 atck09_sel_4[] = {
	 11,  11,   1,   1,   3,   2,   8,   8,  11,  11,   1,   1,   3,   6,   8,   8,
	 11,  11,  13,  13,   5,  12,   9,  10,  11,  11,  13,  13,   5,  12,   9,  10,
};

const Uint8 atck09_sel_5[] = {
	 16,  11,   1,   1,   3,   2,   8,   8,  17,  11,   1,  13,   3,   6,   8,   8,
	 18,  11,  13,  13,   5,  12,   9,  10,  11,  11,  13,  13,   5,  12,   9,  10,
};

const Uint8 atck09_sel_6[] = {
	 16,  11,   1,  13,   3,   2,   8,   8,  16,  11,  13,   1,   3,   6,   8,   8,
	 17,  11,  13,   1,   5,  12,   9,  10,  18,  11,  13,  13,   5,  12,   9,  10,
};

const Uint8 atck09_sel_7[] = {
	 16,  11,  13,  13,   3,   2,   8,   8,  16,  11,  13,  13,   3,   6,   8,   8,
	 17,  11,  13,   5,   5,  12,   9,  10,  18,  11,  13,   5,   5,  12,   9,  10,
};

const Uint8 *const atck09_cmd[] = {
	atck09_cmd_00, atck09_cmd_01, atck09_cmd_02, atck09_cmd_03, atck09_cmd_04, atck09_cmd_05,
	atck09_cmd_06, atck09_cmd_07, atck09_cmd_08, atck09_cmd_09, atck09_cmd_10, atck09_cmd_11,
	atck09_cmd_12, atck09_cmd_13, atck09_cmd_14, atck09_cmd_15, atck09_cmd_16, atck09_cmd_17,
	atck09_cmd_18, atck09_cmd_19, atck09_cmd_20, atck09_cmd_21, atck09_cmd_22, atck09_cmd_23,
	atck09_cmd_24, atck09_cmd_25, atck09_cmd_26, atck09_cmd_27, atck09_cmd_28, atck09_cmd_29,
	atck09_cmd_30, atck09_cmd_31,
};

const Uint8 atck09_cmd_00[] = {
	STAND,
	WAIT2( 10 ),
	WALK_B( 512 ),
	WAIT2( 8 ),
	SP_CMD( pl09_sp_beam, 0, PK_PUNCH, LV_LIGHT, 0 ),
	WAIT2( 14 ),
	SP_CMD( pl09_sp_beam, 0, PK_PUNCH, LV_LIGHT, 0 ),
	IF_RND( 12 ),
		IF_LEVEL( 20 ),
			IF_GAUGE( 3 ),
				SP_CMD( pl09_sp_beam, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ELSE,
				SP_CMD( pl09_sp_arctic, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ELSE,
			SP_CMD( pl09_sp_beam, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck09_cmd_01[] = {
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
		IF_EM_ATCK( ATCK_CROUCH_HK ),
			WAIT2( 4 ),
			CANCEL_SET2,
				CROUCH,
				KICK( LV_HEAVY ),
				SJUMP3( DIR_UP ),
				SJUMP_FLAG_SET,
					CMD_JUMP( 4 ),
					JUMP_FLAG_CLR,
				CMD_ENDIF,
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			CMD_JUMP( 8 ),
		CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 7 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 8 ),
	CMD_EXIT,
};

const Uint8 atck09_cmd_02[] = {
	STAND,
	WAIT2( 8 ),
	CMD_JUMP( 11 ),
	STAND,
	WAIT2( 8 ),
	CMD_EXIT,
};

const Uint8 atck09_cmd_03[] = {
	STAND,
	CMD_JUMP( 0 ),
	WAIT2( 8 ),
	TIMER_SET( 60 ),
	WALK_ATCK( ATCK_CROUCH_HK ),
		IF_LEVEL( 16 ),
			IF_RND( 6 ),
				JUMP( DIR_FORWARD ),
				CMD_JUMP( 20 ),
				PARTNER_CALL,
			CMD_ELSE,
				STAND,
				WAIT2( 2 ),
				PUNCH( LV_HEAVY ),
				SP_CMD( pl09_sp_beam, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ELSE,
			STAND,
			WAIT2( 2 ),
			PUNCH( LV_HEAVY ),
		CMD_ENDIF,
	CMD_ELSE,
		SJUMP( DIR_BACK ),
		SJUMP_FLAG_SET,
			IF_RND( 6 ),
				WAIT( 80 ),
				IF_RND( 16 ),
					SP_CMD( pl09_sp_avalanche, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ELSE,
					SP_CMD( pl09_sp_avalanche, 0, PK_PUNCH, LV_HEAVY, 0 ),
				CMD_ENDIF,
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT2( 8 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck09_cmd_04[] = {
	STAND,
	WAIT2( 14 ),
	TIMER_SET( 60 ),
	IF_DIST( 48, 96, 48, 96 ),
		IF_GAUGE( 3 ),
			SP_CMD( pl09_sp_arctic, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			SP_CMD( pl09_sp_avalanche, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ELSE,
		DASH_B( 160 ),
		SP_CMD( pl09_sp_beam, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ENDIF,
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck09_cmd_05[] = {
	STAND,
	WAIT2( 4 ),
	DASH_ATCK( ATCK_CROUCH_HK ),
		IF_LEVEL( 16 ),
			CANCEL_SET2,
				CROUCH,
				KICK( LV_HEAVY ),
				SJUMP3( DIR_FORWARD ),
				CANCEL_CLR,
			CMD_ENDIF,
			WAIT( 2 ),
			SJUMP_FLAG_SET,
				CMD_JUMP( 4 ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 8 ),
				CANCEL_SET2,
					KICK( LV_LIGHT ),
					PUNCH( LV_HEAVY ),
					SP_CMD( pl09_sp_beam, 0, PK_PUNCH, LV_LIGHT, 0 ),
					CANCEL_CLR,
				CMD_ENDIF,
			CMD_ENDIF,
			IF_RND( 4 ),
				JUMP( DIR_BACK ),
				WAIT( 2 ),
				JUMP_FLAG_SET,
					PARTNER_CALL,
					JUMP_FLAG_CLR,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck09_cmd_06[] = {
	STAND,
	WAIT2( 4 ),
	CMD_JUMP( 0 ),
	IF_AREA( 64, 64, 128, 128 ),
		WAIT2( 4 ),
		SP_CMD( pl09_sp_beam, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ELSE,
		IF_AREA( 96, 64, 64, 128 ),
			WAIT2( 4 ),
			SP_CMD( pl09_sp_beam, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck09_cmd_07[] = {
	STAND,
	WAIT2( 4 ),
	TIMER_SET( 60 ),
	DASH_ATCK( ATCK_CROUCH_LK ),
		WAIT2( 4 ),
		IF_LEVEL( 16 ),
			CANCEL_SET,
				CROUCH,
				KICK( LV_LIGHT ),
				KICK( LV_LIGHT ),
				PUNCH( LV_HEAVY ),
				CANCEL_CLR,
			CMD_ENDIF,
			JUMP( DIR_FORWARD ),
			JUMP_FLAG_SET,
				WAIT( 3 ),
				PARTNER_CALL,
				WAIT( 12 ),
				SP_CMD( pl09_sp_jbeam, 0, PK_PUNCH, LV_HEAVY, 0 ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			CANCEL_SET,
				KICK( LV_LIGHT ),
				CROUCH,
				PUNCH( LV_HEAVY ),
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 18 ),
			IF_GAUGE( 5 ),
				IF_RND( 4 ),
					SP_V_COMBI,
				CMD_ELSE,
					IF_RND( 4 ),
						WAIT2( 4 ),
						PARTNER_CALL,
						SP_CMD( pl09_sp_arctic, 0, PK_PUNCH, LV_LIGHT, 0 ),
					CMD_ELSE,
						PARTNER_CALL,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ELSE,
				PARTNER_CALL,
				DASH_B( 128 ),
				SP_CMD( pl09_sp_beam, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ELSE,
			SP_CMD( pl09_sp_beam, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	CMD_EXIT,
};

const Uint8 atck09_cmd_08[] = {
	STAND,
	IF_GAUGE( 1 ),
		SP_CMD_SET( pl09_sp_arctic, 0, PK_PUNCH, LV_LIGHT, 0, 1, 20 ),
		SP_CMD( pl09_sp_avalanche, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ELSE,
		SP_CMD( pl09_sp_avalanche, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck09_cmd_09[] = {
	STAND,
	IF_GAUGE( 1 ),
		IF_RND( 16 ),
			SP_CMD( pl09_sp_arctic, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			IF_DIST( 64, 64, 64, 64 ),
				STAND,
				CANCEL_SET2,
					KICK( LV_HEAVY ),
					SP_CMD( pl09_sp_beam, 0, PK_PUNCH, LV_HEAVY, 0 ),
					CANCEL_CLR,
				CMD_ENDIF,
			CMD_ELSE,
				PARTNER_CALL,
				DASH_B( 128 ),
				SP_CMD( pl09_sp_avalanche, 0, PK_PUNCH, LV_LIGHT, 0 ),
				SP_CMD( pl09_sp_beam, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_DIST( 64, 64, 64, 64 ),
			STAND,
			CANCEL_SET2,
				KICK( LV_HEAVY ),
				SP_CMD( pl09_sp_beam, 0, PK_PUNCH, LV_HEAVY, 0 ),
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			PARTNER_CALL,
			DASH_B( 128 ),
			SP_CMD( pl09_sp_avalanche, 0, PK_PUNCH, LV_LIGHT, 0 ),
			SP_CMD( pl09_sp_beam, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck09_cmd_10[] = {
	STAND,
	WAIT2( 2 ),
	IF_RND( 16 ),
		IF_GAUGE( 3 ),
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

const Uint8 atck09_cmd_11[] = {
	STAND,
	DASH_B( 96 ),
	STAND,
	WAIT2( 2 ),
	IF_AREA( 128, 64, 128, 64 ),
		SP_CMD( pl09_sp_beam, 0, PK_PUNCH, LV_LIGHT, 0 ),
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
						SP_CMD( pl09_sp_arctic, 0, PK_PUNCH, LV_LIGHT, 0 ),
					CMD_ENDIF,
				CMD_ELSE,
					SP_CMD( pl09_sp_beam, 0, PK_PUNCH, LV_LIGHT, 0 ),
					CMD_JUMP( 24 ),
				CMD_ENDIF,
			CMD_ELSE,
				SP_CMD( pl09_sp_beam, 0, PK_PUNCH, LV_LIGHT, 0 ),
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

const Uint8 atck09_cmd_12[] = {
	STAND,
	WAIT2( 2 ),
	CMD_JUMP( 14 ),
	JUMP( DIR_FORWARD ),
	WAIT( 4 ),
	JUMP_FLAG_SET,
		CMD_JUMP( 20 ),
		WAIT( 6 ),
		SP_CMD( pl09_sp_jbeam, 0, PK_PUNCH, LV_HEAVY, 0 ),
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	WAIT2( 8 ),
	CMD_JUMP( 21 ),
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck09_cmd_13[] = {
	STAND,
	LOCK_SET,
	WAIT2( 6 ),
	CMD_JUMP( 0 ),
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

const Uint8 atck09_cmd_14[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck09_cmd_15[] = {
	CMD_EXIT,
};

const Uint8 atck09_cmd_16[] = {
	CMD_JUMP( 54 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck09_cmd_17[] = {
	CMD_JUMP( 56 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck09_cmd_18[] = {
	CMD_JUMP( 53 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck09_cmd_19[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck09_cmd_20[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck09_cmd_21[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck09_cmd_22[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck09_cmd_23[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck09_cmd_24[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck09_cmd_25[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck09_cmd_26[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck09_cmd_27[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck09_cmd_28[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck09_cmd_29[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck09_cmd_30[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck09_cmd_31[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

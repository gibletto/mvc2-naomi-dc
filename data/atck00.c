/* Ryu (character 00): the computer's attack lists.
   What it does once a waiting list's CMD_NEXT has sent it to attack. PLAYER_WORK's cpu_adrs[1]
   points here.

   atck00_tbl     one record (CPU_DATA): the offsets of the three tables.
   atck00_rate    the chance, out of 32, that a list's CMD_NEXT2 starts another attack list, by
                  the level of play.
   atck00_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   atck00_cmd     the lists: a pick is a number in this table.
   atck00_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                  list of lib00.c. */
#include "player.h"
#include "pl00_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const atck00_sel[], *const atck00_cmd[];
extern const Uint8 atck00_rate[], atck00_sel_0[], atck00_sel_1[], atck00_sel_2[], atck00_sel_3[],
	atck00_sel_4[], atck00_sel_5[], atck00_sel_6[], atck00_sel_7[], atck00_cmd_00[], atck00_cmd_01[],
	atck00_cmd_02[], atck00_cmd_03[], atck00_cmd_04[], atck00_cmd_05[], atck00_cmd_06[],
	atck00_cmd_07[], atck00_cmd_08[], atck00_cmd_09[], atck00_cmd_10[], atck00_cmd_11[],
	atck00_cmd_12[], atck00_cmd_13[], atck00_cmd_14[], atck00_cmd_15[], atck00_cmd_16[],
	atck00_cmd_17[], atck00_cmd_18[], atck00_cmd_19[], atck00_cmd_20[], atck00_cmd_21[],
	atck00_cmd_22[], atck00_cmd_23[], atck00_cmd_24[], atck00_cmd_25[], atck00_cmd_26[],
	atck00_cmd_27[], atck00_cmd_28[], atck00_cmd_29[], atck00_cmd_30[], atck00_cmd_31[];

const CPU_DATA atck00_tbl[] = {
	{ ( Sint32 )atck00_rate, ( Sint32 )atck00_sel, ( Sint32 )atck00_cmd },
};

const Uint8 atck00_rate[] = {
	  8,   8,   8,   8,   9,   9,  10,  10,  11,  11,  12,  12,  13,  13,  14,  14,
	 15,  15,  16,  16,  17,  17,  18,  18,  19,  20,  21,  22,  24,  26,  28,  30,
};

const Uint8 *const atck00_sel[] = {
	atck00_sel_0, atck00_sel_1, atck00_sel_2, atck00_sel_3, atck00_sel_4, atck00_sel_5,
	atck00_sel_6, atck00_sel_7,
};

const Uint8 atck00_sel_0[] = {
	 16,   0,   0,   0,   1,   2,   3,   3,   0,   0,   0,   0,   1,   2,   3,   3,
	  0,   0,   0,   0,   1,   2,   3,   3,   6,   6,   7,   7,   1,   2,   3,   3,
};

const Uint8 atck00_sel_1[] = {
	 17,   0,   0,   0,   1,   1,   3,   4,   0,   0,   0,   0,   1,   1,   3,   4,
	  0,   0,   0,   0,   1,   1,   3,   4,   0,   0,   0,   0,   1,   1,   3,   4,
};

const Uint8 atck00_sel_2[] = {
	 18,   7,   1,   1,   2,   6,   3,   3,   7,   7,   1,   1,   2,   6,   3,   3,
	  7,   7,   1,   1,   5,   6,   3,   3,   7,   7,   1,   1,   2,   6,   3,   3,
};

const Uint8 atck00_sel_3[] = {
	 12,  11,   1,   1,   2,   6,   3,   8,  12,  11,   1,   1,   2,   6,   3,   8,
	 12,  11,   1,   1,   2,   6,   3,   9,   5,   5,  13,  13,   2,   6,   3,   9,
};

const Uint8 atck00_sel_4[] = {
	 11,  11,   1,   1,   3,   2,   8,   8,  11,  11,   1,   1,   3,   6,   8,   8,
	 11,  11,  13,  13,   5,  12,   9,  10,  11,  11,  13,  13,   5,  12,   9,  10,
};

const Uint8 atck00_sel_5[] = {
	 16,  11,   1,   1,   3,   2,   8,   8,  17,  11,   1,  13,   3,   6,   8,   8,
	 18,  11,  13,  13,   5,  12,   9,  10,  11,  11,  13,  13,   5,  12,   9,  10,
};

const Uint8 atck00_sel_6[] = {
	 16,  11,   1,  13,   3,   2,   8,   8,  16,  11,  13,   1,   3,   6,   8,   8,
	 17,  11,  13,   1,   5,  12,   9,  10,  18,  11,  13,  13,   5,  12,   9,  10,
};

const Uint8 atck00_sel_7[] = {
	 16,  11,  13,  13,   3,   2,   8,   8,  16,  11,  13,  13,   3,   6,   8,   8,
	 17,  11,  13,   5,   5,  12,   9,  10,  18,  11,  13,   5,   5,  12,   9,  10,
};

const Uint8 *const atck00_cmd[] = {
	atck00_cmd_00, atck00_cmd_01, atck00_cmd_02, atck00_cmd_03, atck00_cmd_04, atck00_cmd_05,
	atck00_cmd_06, atck00_cmd_07, atck00_cmd_08, atck00_cmd_09, atck00_cmd_10, atck00_cmd_11,
	atck00_cmd_12, atck00_cmd_13, atck00_cmd_14, atck00_cmd_15, atck00_cmd_16, atck00_cmd_17,
	atck00_cmd_18, atck00_cmd_19, atck00_cmd_20, atck00_cmd_21, atck00_cmd_22, atck00_cmd_23,
	atck00_cmd_24, atck00_cmd_25, atck00_cmd_26, atck00_cmd_27, atck00_cmd_28, atck00_cmd_29,
	atck00_cmd_30, atck00_cmd_31,
};

const Uint8 atck00_cmd_00[] = {
	STAND,
	WAIT2( 10 ),
	WALK_B( 512 ),
	WAIT2( 8 ),
	SP_CMD( pl00_sp_hadou, 0, PK_PUNCH, LV_LIGHT, 0 ),
	IF_LEVEL( 24 ),
		IF_GAUGE( 3 ),
			IF_RND( 4 ),
				IF_LEVEL( 28 ),
					WAIT2( 8 ),
					SP_CMD_SET( pl00_sp_shinkuu, 0, PK_PUNCH, LV_LIGHT, 0, 1, 20 ),
					SP_CMD( pl00_sp_hadou, 0, PK_PUNCH, LV_HEAVY, 0 ),
				CMD_ELSE,
					WAIT2( 8 ),
					SP_CMD( pl00_sp_shinkuu, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ENDIF,
			CMD_ELSE,
				WAIT2( 14 ),
				SP_CMD( pl00_sp_hadou, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ENDIF,
		CMD_ELSE,
			WAIT2( 14 ),
			SP_CMD( pl00_sp_hadou, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ENDIF,
	CMD_ELSE,
		WAIT2( 14 ),
		SP_CMD( pl00_sp_hadou, 0, PK_PUNCH, LV_HEAVY, 0 ),
		WAIT2( 14 ),
		SP_CMD( pl00_sp_hadou, 0, PK_PUNCH, LV_HEAVY, 0 ),
		WAIT2( 14 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck00_cmd_01[] = {
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
			CMD_JUMP( 8 ),
		CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 7 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 8 ),
	CMD_EXIT,
};

const Uint8 atck00_cmd_02[] = {
	STAND,
	WAIT2( 8 ),
	TIMER_SET( 60 ),
	WALK_EM( 64 ),
	CMD_JUMP( 11 ),
	STAND,
	WAIT2( 8 ),
	CMD_EXIT,
};

const Uint8 atck00_cmd_03[] = {
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
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck00_cmd_04[] = {
	STAND,
	WAIT2( 14 ),
	TIMER_SET( 60 ),
	IF_EM_ATCK( ATCK_CROUCH_HP ),
		SP_CMD( pl00_sp_shouryu, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ELSE,
		SP_CMD( pl00_sp_hadou, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ENDIF,
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck00_cmd_05[] = {
	STAND,
	WAIT2( 4 ),
	DASH_ATCK( ATCK_CROUCH_LK ),
		CROUCH,
		IF_LEVEL( 24 ),
			CANCEL_SET2,
				KICK( LV_LIGHT ),
				PUNCH( LV_HEAVY ),
				SJUMP3( DIR_FORWARD ),
				WAIT( 4 ),
				CANCEL_CLR,
			CMD_ENDIF,
			SJUMP_FLAG_SET,
				CMD_JUMP( 4 ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 16 ),
				CANCEL_SET2,
					KICK( LV_LIGHT ),
					STAND,
					PUNCH( LV_HEAVY ),
					SP_CMD( pl00_sp_hadou, 0, PK_PUNCH, LV_LIGHT, 0 ),
					CANCEL_CLR,
				CMD_ENDIF,
			CMD_ELSE,
				KICK( LV_HEAVY ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_RND( 16 ),
			CMD_JUMP( 54 ),
		CMD_ELSE,
			CMD_JUMP( 59 ),
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck00_cmd_06[] = {
	STAND,
	WAIT2( 4 ),
	CMD_JUMP( 0 ),
	IF_AREA( 64, 64, 48, 32 ),
		WAIT2( 4 ),
		SP_CMD( pl00_sp_hadou, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck00_cmd_07[] = {
	STAND,
	WAIT2( 4 ),
	SP_CMD( pl00_sp_tatsu, 0, PK_PUNCH, LV_HEAVY, 0 ),
	STAND,
	WAIT2( 2 ),
	IF_EM_ATCK( ATCK_CROUCH_HK ),
		CROUCH,
		KICK( LV_HEAVY ),
	CMD_ENDIF,
	STAND,
	CMD_EXIT,
};

const Uint8 atck00_cmd_08[] = {
	STAND,
	IF_GAUGE( 1 ),
		IF_RND( 16 ),
			WAIT2( 2 ),
			IF_AREA( 240, 48, 128, 48 ),
				IF_RND( 8 ),
					PARTNER_CALL,
				CMD_ENDIF,
				IF_LEVEL( 26 ),
					SP_CMD_SET( pl00_sp_shinkuu, 0, PK_PUNCH, LV_LIGHT, 0, 1, 20 ),
					SP_CMD( pl00_sp_hadou, 0, PK_PUNCH, LV_HEAVY, 0 ),
				CMD_ELSE,
					SP_CMD( pl00_sp_shinkuu, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			CMD_JUMP( 54 ),
		CMD_ENDIF,
	CMD_ELSE,
		STAND,
		WAIT2( 2 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck00_cmd_09[] = {
	STAND,
	IF_GAUGE( 1 ),
		IF_RND( 16 ),
			IF_EM_ATCK( pl00_at_shinkuu_tatsu ),
				SP_CMD( pl00_sp_shinkuu_tatsu, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_RND( 8 ),
			SP_CMD( pl00_sp_tatsu, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			STAND,
			WAIT2( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck00_cmd_10[] = {
	STAND,
	WAIT2( 2 ),
	IF_RND( 16 ),
		IF_GAUGE( 5 ),
			IF_AREA( 160, 48, 160, 48 ),
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
			JUMP( DIR_BACK ),
			WAIT( 8 ),
			JUMP_FLAG_SET,
				SP_CMD( pl00_sp_jhadou, 0, PK_PUNCH, LV_HEAVY, 0 ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		WAIT( 1 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck00_cmd_11[] = {
	STAND,
	DASH_B( 96 ),
	STAND,
	WAIT2( 2 ),
	IF_AREA( 128, 64, 128, 32 ),
		SP_CMD( pl00_sp_hadou, 0, PK_PUNCH, LV_HEAVY, 0 ),
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
		IF_AREA( 128, 64, 128, 32 ),
			IF_LEVEL( 24 ),
				IF_GAUGE( 2 ),
					IF_RND( 10 ),
						SP_CMD( pl00_sp_shinkuu, 0, PK_PUNCH, LV_LIGHT, 0 ),
					CMD_ENDIF,
				CMD_ELSE,
					SP_CMD( pl00_sp_hadou, 0, PK_PUNCH, LV_HEAVY, 0 ),
					CMD_JUMP( 24 ),
				CMD_ENDIF,
			CMD_ELSE,
				SP_CMD( pl00_sp_hadou, 0, PK_PUNCH, LV_HEAVY, 0 ),
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

const Uint8 atck00_cmd_12[] = {
	STAND,
	WAIT2( 2 ),
	CMD_JUMP( 14 ),
	JUMP( DIR_FORWARD ),
	WAIT( 4 ),
	JUMP_FLAG_SET,
		CMD_JUMP( 20 ),
		WAIT( 6 ),
		SP_CMD( pl00_sp_jtatsu, 0, PK_PUNCH, LV_HEAVY, 0 ),
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	WAIT2( 8 ),
	CMD_JUMP( 21 ),
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck00_cmd_13[] = {
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

const Uint8 atck00_cmd_14[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck00_cmd_15[] = {
	CMD_EXIT,
};

const Uint8 atck00_cmd_16[] = {
	CMD_JUMP( 54 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck00_cmd_17[] = {
	CMD_JUMP( 56 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck00_cmd_18[] = {
	CMD_JUMP( 53 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck00_cmd_19[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck00_cmd_20[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck00_cmd_21[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck00_cmd_22[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck00_cmd_23[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck00_cmd_24[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck00_cmd_25[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck00_cmd_26[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck00_cmd_27[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck00_cmd_28[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck00_cmd_29[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck00_cmd_30[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck00_cmd_31[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

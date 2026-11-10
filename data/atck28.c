/* Gambit (character 28): the computer's attack lists.
   What it does once a waiting list's CMD_NEXT has sent it to attack. PLAYER_WORK's cpu_adrs[1]
   points here.

   atck28_tbl     one record (CPU_DATA): the offsets of the three tables.
   atck28_rate    the chance, out of 32, that a list's CMD_NEXT2 starts another attack list, by
                  the level of play.
   atck28_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   atck28_cmd     the lists: a pick is a number in this table.
   atck28_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                  list of lib28.c. */
#include "player.h"
#include "pl28_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const atck28_sel[], *const atck28_cmd[];
extern const Uint8 atck28_rate[], atck28_sel_0[], atck28_sel_1[], atck28_sel_2[], atck28_sel_3[],
	atck28_sel_4[], atck28_sel_5[], atck28_sel_6[], atck28_sel_7[], atck28_cmd_00[], atck28_cmd_01[],
	atck28_cmd_02[], atck28_cmd_03[], atck28_cmd_04[], atck28_cmd_05[], atck28_cmd_06[],
	atck28_cmd_07[], atck28_cmd_08[], atck28_cmd_09[], atck28_cmd_10[], atck28_cmd_11[],
	atck28_cmd_12[], atck28_cmd_13[], atck28_cmd_14[], atck28_cmd_15[], atck28_cmd_16[],
	atck28_cmd_17[], atck28_cmd_18[], atck28_cmd_19[], atck28_cmd_20[], atck28_cmd_21[],
	atck28_cmd_22[], atck28_cmd_23[], atck28_cmd_24[], atck28_cmd_25[], atck28_cmd_26[],
	atck28_cmd_27[], atck28_cmd_28[], atck28_cmd_29[], atck28_cmd_30[], atck28_cmd_31[];

const CPU_DATA atck28_tbl[] = {
	{ ( Sint32 )atck28_rate, ( Sint32 )atck28_sel, ( Sint32 )atck28_cmd },
};

const Uint8 atck28_rate[] = {
	  9,   9,  11,  11,  14,  14,  15,  15,  18,  19,  20,  21,  22,  23,  24,  25,
	 25,  26,  27,  27,  27,  27,  27,  27,  28,  28,  29,  29,  30,  30,  31,  31,
};

const Uint8 *const atck28_sel[] = {
	atck28_sel_0, atck28_sel_1, atck28_sel_2, atck28_sel_3, atck28_sel_4, atck28_sel_5,
	atck28_sel_6, atck28_sel_7,
};

const Uint8 atck28_sel_0[] = {
	  0,   0,   7,   7,   7,   2,   2,  16,   0,   0,   0,   1,   1,   2,   2,  16,
	  0,   0,   1,   1,   1,   2,   2,  14,  17,  17,  17,  17,  18,  18,  18,  14,
};

const Uint8 atck28_sel_1[] = {
	  2,   2,   7,   7,   7,   4,   4,  16,   2,   2,   2,   3,   3,   4,   4,  16,
	  2,   2,   3,   3,   3,   4,   4,  14,  17,  17,  17,  17,  18,  18,  18,  14,
};

const Uint8 atck28_sel_2[] = {
	  4,   4,   7,   7,   5,   7,   6,  16,   4,   4,   4,   5,   5,   6,   6,  16,
	  4,   4,   5,   5,   5,   6,   6,  14,  17,  17,  17,  17,  18,  18,  18,  14,
};

const Uint8 atck28_sel_3[] = {
	  6,   6,   6,   7,   7,   8,   8,   8,   6,   6,   6,   7,   7,   8,   8,   8,
	  6,   6,   7,   7,   7,   8,   8,  14,  17,  17,  18,  18,  18,  16,  16,  16,
};

const Uint8 atck28_sel_4[] = {
	  7,   7,   7,   9,  10,  10,  14,  14,   8,   8,   9,   9,  10,  10,  14,  14,
	  8,   8,   9,   9,  10,  10,  14,  14,  17,  17,  18,  18,  18,  18,  17,  17,
};

const Uint8 atck28_sel_5[] = {
	  7,   7,   7,  11,  11,  12,  12,  14,  10,  10,  10,  11,  11,  12,  12,  14,
	 10,  10,  11,  11,  11,  12,  12,  14,  16,  16,  16,  16,  18,  18,  18,  18,
};

const Uint8 atck28_sel_6[] = {
	  7,   7,   7,  13,  13,  15,  15,  14,  12,  12,  12,  13,  13,  15,  15,  14,
	 17,  17,  17,  17,  13,  15,  19,  19,  17,  18,  18,  18,  18,  16,  16,  16,
};

const Uint8 atck28_sel_7[] = {
	  7,   7,   7,  13,  15,  16,  15,  14,  10,  11,  12,  13,  15,  16,  15,  14,
	 10,  11,  12,  13,  15,  16,  15,  14,  18,  18,  18,  17,  17,  16,  20,  20,
};

const Uint8 *const atck28_cmd[] = {
	atck28_cmd_00, atck28_cmd_01, atck28_cmd_02, atck28_cmd_03, atck28_cmd_04, atck28_cmd_05,
	atck28_cmd_06, atck28_cmd_07, atck28_cmd_08, atck28_cmd_09, atck28_cmd_10, atck28_cmd_11,
	atck28_cmd_12, atck28_cmd_13, atck28_cmd_14, atck28_cmd_15, atck28_cmd_16, atck28_cmd_17,
	atck28_cmd_18, atck28_cmd_19, atck28_cmd_20, atck28_cmd_21, atck28_cmd_22, atck28_cmd_23,
	atck28_cmd_24, atck28_cmd_25, atck28_cmd_26, atck28_cmd_27, atck28_cmd_28, atck28_cmd_29,
	atck28_cmd_30, atck28_cmd_31,
};

const Uint8 atck28_cmd_00[] = {
	IF_RND( 11 ),
		WAIT( 5 ),
		WAIT2( 6 ),
		CMD_JUMP( 10 ),
	CMD_ELSE,
		IF_RND( 16 ),
			WAIT( 5 ),
			WAIT2( 6 ),
			CMD_JUMP( 11 ),
		CMD_ELSE,
			WAIT( 5 ),
			WAIT2( 6 ),
			CMD_JUMP( 12 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck28_cmd_01[] = {
	IF_RND( 11 ),
		WAIT( 5 ),
		WAIT2( 6 ),
		CMD_JUMP( 7 ),
	CMD_ELSE,
		IF_RND( 16 ),
			WAIT( 5 ),
			WAIT2( 6 ),
			LOCK_SET,
			CMD_JUMP( 8 ),
			LOCK_CLR,
		CMD_ELSE,
			WAIT( 5 ),
			WAIT2( 6 ),
			LOCK_SET,
			CMD_JUMP( 9 ),
			LOCK_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck28_cmd_02[] = {
	WAIT( 5 ),
	WAIT2( 6 ),
	CMD_JUMP( 13 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck28_cmd_03[] = {
	WAIT( 5 ),
	WAIT2( 6 ),
	LOCK_SET,
	JUMP_RND( 21, 21, 21, 34 ),
	LOCK_CLR,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck28_cmd_04[] = {
	IF_RND( 11 ),
		WAIT( 3 ),
		WAIT2( 4 ),
		CMD_JUMP( 10 ),
	CMD_ELSE,
		IF_RND( 16 ),
			WAIT( 3 ),
			WAIT2( 4 ),
			CMD_JUMP( 11 ),
		CMD_ELSE,
			WAIT( 3 ),
			WAIT2( 4 ),
			CMD_JUMP( 12 ),
		CMD_ENDIF,
	CMD_ENDIF,
	IF_RND( 12 ),
		CMD_JUMP( 0 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck28_cmd_05[] = {
	IF_RND( 11 ),
		WAIT( 4 ),
		WAIT2( 4 ),
		CMD_JUMP( 7 ),
	CMD_ELSE,
		IF_RND( 16 ),
			WAIT( 4 ),
			WAIT2( 4 ),
			LOCK_SET,
			CMD_JUMP( 8 ),
			LOCK_CLR,
		CMD_ELSE,
			WAIT( 4 ),
			WAIT2( 4 ),
			LOCK_SET,
			CMD_JUMP( 9 ),
			LOCK_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck28_cmd_06[] = {
	IF_RND( 11 ),
		WAIT( 4 ),
		WAIT2( 4 ),
		CMD_JUMP( 13 ),
	CMD_ELSE,
		IF_RND( 16 ),
			WAIT( 4 ),
			WAIT2( 4 ),
			CMD_JUMP( 24 ),
		CMD_ELSE,
			WAIT( 4 ),
			WAIT2( 4 ),
			CMD_JUMP( 25 ),
		CMD_ENDIF,
	CMD_ENDIF,
	IF_LEVEL( 16 ),
		IF_RND( 16 ),
			WAIT( 10 ),
			WAIT2( 6 ),
			SP_CMD( pl28_sp_card, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			IF_RND( 16 ),
				WAIT( 10 ),
				WAIT2( 6 ),
				SP_CMD( pl28_sp_slash2, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				CMD_JUMP( 42 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_RND( 8 ),
			WAIT( 10 ),
			WAIT2( 8 ),
			IF_RND( 16 ),
				WAIT( 20 ),
				SP_CMD( pl28_sp_card, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ELSE,
				SP_CMD( pl28_sp_slash2, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck28_cmd_07[] = {
	STAND,
	IF_RND( 14 ),
		IF_AREA( 32, 0, 32, 0 ),
			KICK( LV_LIGHT ),
			IF_RND( 10 ),
				KICK( LV_HEAVY ),
			CMD_ENDIF,
		CMD_ELSE,
			CMD_JUMP( 21 ),
		CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 21 ),
		IF_RND( 8 ),
			CMD_JUMP( 21 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck28_cmd_08[] = {
	STAND,
	IF_RND( 10 ),
		CMD_JUMP( 4 ),
	CMD_ELSE,
		IF_RND( 11 ),
			WAIT( 4 ),
			WAIT2( 4 ),
			CMD_JUMP( 10 ),
		CMD_ELSE,
			IF_RND( 16 ),
				WAIT( 4 ),
				WAIT2( 4 ),
				CMD_JUMP( 11 ),
			CMD_ELSE,
				WAIT( 4 ),
				WAIT2( 4 ),
				LOCK_SET,
				CMD_JUMP( 8 ),
				LOCK_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck28_cmd_09[] = {
	IF_RND( 11 ),
		WAIT( 4 ),
		WAIT2( 4 ),
		CMD_JUMP( 13 ),
	CMD_ELSE,
		IF_RND( 16 ),
			WAIT( 4 ),
			WAIT2( 4 ),
			CMD_JUMP( 24 ),
		CMD_ELSE,
			WAIT( 4 ),
			WAIT2( 4 ),
			CMD_JUMP( 25 ),
		CMD_ENDIF,
	CMD_ENDIF,
	IF_LEVEL( 16 ),
		IF_RND( 16 ),
			WAIT( 10 ),
			WAIT2( 6 ),
			SP_CMD( pl28_sp_card, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			IF_RND( 16 ),
				WAIT( 10 ),
				WAIT2( 6 ),
				SP_CMD( pl28_sp_slash, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				CMD_JUMP( 42 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_RND( 8 ),
			WAIT( 10 ),
			WAIT2( 8 ),
			IF_RND( 16 ),
				CMD_JUMP( 42 ),
			CMD_ELSE,
				SP_V_COMBI,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck28_cmd_10[] = {
	STAND,
	IF_RND( 8 ),
		IF_AREA( 32, 0, 32, 0 ),
			IF_RND( 10 ),
				CROUCH,
				TIMER2_SET( 5 ),
				WAIT( 30 ),
				SP_CMD( pl28_sp_escape, 0, PK_PUNCH, LV_HEAVY, LEVER_BACK ),
			CMD_ELSE,
				CROUCH,
				TIMER2_SET( 5 ),
				WAIT( 30 ),
				SP_CMD( pl28_sp_escape, 0, PK_PUNCH, LV_HEAVY, LEVER_FORWARD ),
			CMD_ENDIF,
		CMD_ELSE,
			CMD_JUMP( 21 ),
		CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 21 ),
		IF_RND( 8 ),
			CMD_JUMP( 21 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck28_cmd_11[] = {
	IF_RND( 20 ),
		CMD_JUMP( 17 ),
	CMD_ELSE,
		CMD_JUMP( 25 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck28_cmd_12[] = {
	STAND,
	IF_RND( 10 ),
		CMD_JUMP( 4 ),
	CMD_ELSE,
		IF_RND( 11 ),
			WAIT( 4 ),
			WAIT2( 4 ),
			CMD_JUMP( 10 ),
		CMD_ELSE,
			IF_RND( 16 ),
				WAIT( 4 ),
				WAIT2( 4 ),
				CMD_JUMP( 11 ),
			CMD_ELSE,
				WAIT( 4 ),
				WAIT2( 4 ),
				LOCK_SET,
				CMD_JUMP( 8 ),
				LOCK_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck28_cmd_13[] = {
	STAND,
	IF_RND( 10 ),
		CMD_JUMP( 21 ),
	CMD_ELSE,
		IF_RND( 11 ),
			WAIT( 4 ),
			WAIT2( 4 ),
			CMD_JUMP( 25 ),
			IF_RND( 10 ),
				CMD_JUMP( 0 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_RND( 16 ),
				WAIT( 4 ),
				WAIT2( 4 ),
				CMD_JUMP( 26 ),
				IF_RND( 10 ),
					CMD_JUMP( 0 ),
				CMD_ENDIF,
			CMD_ELSE,
				WAIT( 4 ),
				WAIT2( 4 ),
				CMD_JUMP( 24 ),
				IF_RND( 10 ),
					CMD_JUMP( 0 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck28_cmd_14[] = {
	CMD_JUMP( 13 ),
	CMD_JUMP( 0 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck28_cmd_15[] = {
	IF_RND( 10 ),
		CMD_JUMP( 7 ),
	CMD_ELSE,
		IF_RND( 16 ),
			CMD_JUMP( 9 ),
		CMD_ELSE,
			CMD_JUMP( 27 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck28_cmd_16[] = {
	STAND,
	IF_EM_STANCE( STANCE_AIR ),
		CMD_JUMP( 21 ),
	CMD_ELSE,
		IF_AREA( 48, 0, 48, 0 ),
			IF_RND( 16 ),
				CROUCH,
				KICK( LV_HEAVY ),
				STAND,
			CMD_ELSE,
				CROUCH,
				KICK( LV_HEAVY ),
				STAND,
				WALK_B( 64 ),
				WAIT( 20 ),
				IF_EM_STANCE( STANCE_STAND ),
					SP_CMD( pl28_sp_card, 0, PK_PUNCH, LV_HEAVY, 0 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			TIMER_SET( 30 ),
			WALK_ATCK( ATCK_CROUCH_HK ),
				CROUCH,
				KICK( LV_HEAVY ),
				STAND,
			CMD_ELSE,
				JUMP( DIR_BACK ),
				STAND,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	IF_RND( 10 ),
		CMD_JUMP( 0 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck28_cmd_17[] = {
	IF_RND( 16 ),
		CMD_JUMP( 28 ),
	CMD_ELSE,
		CMD_JUMP( 29 ),
	CMD_ENDIF,
	IF_RND( 24 ),
		CMD_JUMP( 29 ),
	CMD_ELSE,
		CMD_JUMP( 13 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck28_cmd_18[] = {
	JUMP_RND( 8, 21, 27, 28 ),
	CMD_JUMP( 0 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck28_cmd_19[] = {
	STAND,
	IF_EM_ATCK( ATCK_CROUCH_HK ),
		CMD_JUMP( 5 ),
	CMD_ELSE,
		LOCK_SET,
		DASH_EM( 16 ),
		CMD_JUMP( 5 ),
		LOCK_CLR,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck28_cmd_20[] = {
	STAND,
	LOCK_SET,
	DASH_EM( 16 ),
	IF_EM_ATCK( ATCK_STAND_LK ),
		KICK( LV_LIGHT ),
		WALK_F( 28 ),
		KICK( LV_LIGHT ),
		WALK_F( 32 ),
		IF_CATCH,
			NAG_VFLIP_ADJ( 1, LEVER_FORWARD ),
			WAIT( 5 ),
			CROUCH,
			IF_EM_ATCK( ATCK_CROUCH_MK ),
				IF_RND( 16 ),
					CANCEL_SET2,
						KICK( LV_LIGHT ),
						SP_CMD( pl28_sp_slash, 0, PK_PUNCH, LV_LIGHT, 0 ),
						CANCEL_CLR,
					CMD_ENDIF,
				CMD_ELSE,
					CANCEL_SET2,
						KICK( LV_LIGHT ),
						SP_CMD( pl28_sp_slash, 0, PK_PUNCH, LV_HEAVY, 0 ),
						CANCEL_CLR,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			WAIT( 8 ),
			DASH_B( 256 ),
			WAIT( 16 ),
		CMD_ENDIF,
	CMD_ELSE,
		KICK( LV_LIGHT ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck28_cmd_21[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck28_cmd_22[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck28_cmd_23[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck28_cmd_24[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck28_cmd_25[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck28_cmd_26[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck28_cmd_27[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck28_cmd_28[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck28_cmd_29[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck28_cmd_30[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck28_cmd_31[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

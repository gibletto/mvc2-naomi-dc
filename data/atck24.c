/* Cammy (character 24): the computer's attack lists.
   What it does once a waiting list's CMD_NEXT has sent it to attack. PLAYER_WORK's cpu_adrs[1]
   points here.

   atck24_tbl     one record (CPU_DATA): the offsets of the three tables.
   atck24_rate    the chance, out of 32, that a list's CMD_NEXT2 starts another attack list, by
                  the level of play.
   atck24_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   atck24_cmd     the lists: a pick is a number in this table.
   atck24_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                  list of lib24.c. */
#include "player.h"
#include "pl24_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const atck24_sel[], *const atck24_cmd[];
extern const Uint8 atck24_rate[], atck24_sel_0[], atck24_sel_1[], atck24_sel_2[], atck24_sel_3[],
	atck24_sel_4[], atck24_sel_5[], atck24_sel_6[], atck24_sel_7[], atck24_cmd_00[], atck24_cmd_01[],
	atck24_cmd_02[], atck24_cmd_03[], atck24_cmd_04[], atck24_cmd_05[], atck24_cmd_06[],
	atck24_cmd_07[], atck24_cmd_08[], atck24_cmd_09[], atck24_cmd_10[], atck24_cmd_11[],
	atck24_cmd_12[], atck24_cmd_13[], atck24_cmd_14[], atck24_cmd_15[], atck24_cmd_16[],
	atck24_cmd_17[], atck24_cmd_18[], atck24_cmd_19[], atck24_cmd_20[], atck24_cmd_21[],
	atck24_cmd_22[], atck24_cmd_23[], atck24_cmd_24[], atck24_cmd_25[], atck24_cmd_26[],
	atck24_cmd_27[], atck24_cmd_28[], atck24_cmd_29[], atck24_cmd_30[], atck24_cmd_31[];

const CPU_DATA atck24_tbl[] = {
	{ ( Sint32 )atck24_rate, ( Sint32 )atck24_sel, ( Sint32 )atck24_cmd },
};

const Uint8 atck24_rate[] = {
	 10,  10,  13,  13,  15,  15,  17,  17,  18,  19,  20,  21,  22,  23,  24,  25,
	 25,  26,  27,  27,  27,  27,  27,  27,  28,  28,  29,  29,  30,  30,  31,  31,
};

const Uint8 *const atck24_sel[] = {
	atck24_sel_0, atck24_sel_1, atck24_sel_2, atck24_sel_3, atck24_sel_4, atck24_sel_5,
	atck24_sel_6, atck24_sel_7,
};

const Uint8 atck24_sel_0[] = {
	 17,   0,   0,   1,   1,   2,   2,  16,  17,   0,   0,   1,   1,   2,   2,  16,
	 17,   0,   1,   1,   1,   2,   2,  14,  17,   0,   1,   1,   1,   2,   2,  14,
};

const Uint8 atck24_sel_1[] = {
	 17,   2,   2,   3,  17,  17,   4,  16,  17,   2,   2,   3,   3,   4,   4,  16,
	 17,   2,   3,   3,   3,   4,   4,  14,   5,   5,   5,   3,   3,   4,   4,  14,
};

const Uint8 atck24_sel_2[] = {
	 17,   4,   4,  17,   5,  17,   6,  16,  17,   4,   4,  17,   5,  17,   6,  16,
	 17,   4,   5,   5,   5,   6,   6,  14,  17,   4,   5,   5,   5,   6,   6,  14,
};

const Uint8 atck24_sel_3[] = {
	 17,   6,   6,   7,   7,   8,   8,   5,  17,   6,   6,   7,  17,  17,   8,   5,
	 17,   5,   5,   7,  17,   8,   8,  14,  16,  16,  16,  16,  16,  16,  16,  16,
};

const Uint8 atck24_sel_4[] = {
	 17,   8,   9,   9,  10,  10,  14,  14,  17,   8,   9,   9,  10,  10,  14,  14,
	 17,   8,   5,   5,   5,  17,  17,  14,  16,  16,  16,  16,  16,  10,  14,  14,
};

const Uint8 atck24_sel_5[] = {
	 17,  10,  10,  11,  11,  12,  12,  14,  17,  10,  10,  11,  11,  12,  12,  14,
	 17,   5,   5,   5,   5,  17,  17,  14,  16,  16,  16,  16,  16,  16,  16,  16,
};

const Uint8 atck24_sel_6[] = {
	  5,   5,   5,   5,   5,  15,  15,  14,  12,  12,  12,  13,  13,  15,  15,  14,
	 12,  12,  13,  13,  13,  15,  14,  14,  17,  12,  17,  13,  17,  17,  17,  17,
};

const Uint8 atck24_sel_7[] = {
	  5,   5,   5,  13,  15,  16,  15,  14,  10,  11,  12,  13,  15,  16,  15,  14,
	 10,  11,  12,  13,  15,  16,  15,  14,  17,  17,  17,  17,  17,  17,  17,  17,
};

const Uint8 *const atck24_cmd[] = {
	atck24_cmd_00, atck24_cmd_01, atck24_cmd_02, atck24_cmd_03, atck24_cmd_04, atck24_cmd_05,
	atck24_cmd_06, atck24_cmd_07, atck24_cmd_08, atck24_cmd_09, atck24_cmd_10, atck24_cmd_11,
	atck24_cmd_12, atck24_cmd_13, atck24_cmd_14, atck24_cmd_15, atck24_cmd_16, atck24_cmd_17,
	atck24_cmd_18, atck24_cmd_19, atck24_cmd_20, atck24_cmd_21, atck24_cmd_22, atck24_cmd_23,
	atck24_cmd_24, atck24_cmd_25, atck24_cmd_26, atck24_cmd_27, atck24_cmd_28, atck24_cmd_29,
	atck24_cmd_30, atck24_cmd_31,
};

const Uint8 atck24_cmd_00[] = {
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
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck24_cmd_01[] = {
	IF_RND( 11 ),
		WAIT( 5 ),
		WAIT2( 6 ),
		CMD_JUMP( 7 ),
	CMD_ELSE,
		IF_RND( 16 ),
			WAIT( 5 ),
			WAIT2( 6 ),
			CMD_JUMP( 8 ),
		CMD_ELSE,
			WAIT( 5 ),
			WAIT2( 6 ),
			CMD_JUMP( 9 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck24_cmd_02[] = {
	WAIT( 5 ),
	WAIT2( 6 ),
	CMD_JUMP( 13 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck24_cmd_03[] = {
	WAIT( 5 ),
	WAIT2( 6 ),
	CMD_JUMP( 21 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck24_cmd_04[] = {
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
			CMD_JUMP( 12 ),
		CMD_ENDIF,
	CMD_ENDIF,
	IF_RND( 8 ),
		CMD_JUMP( 0 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck24_cmd_05[] = {
	IF_RND( 11 ),
		WAIT( 4 ),
		WAIT2( 4 ),
		CMD_JUMP( 7 ),
	CMD_ELSE,
		IF_RND( 16 ),
			WAIT( 4 ),
			WAIT2( 4 ),
			CMD_JUMP( 8 ),
		CMD_ELSE,
			WAIT( 4 ),
			WAIT2( 4 ),
			CMD_JUMP( 9 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck24_cmd_06[] = {
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
			SP_CMD( pl24_sp_smasher, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			IF_RND( 16 ),
				WAIT( 10 ),
				WAIT2( 6 ),
				JUMP( DIR_BACK ),
				JUMP_FLAG_SET,
					SP_CMD( pl24_sp_killerbee, 0, PK_PUNCH, LV_LIGHT, 0 ),
					JUMP_FLAG_CLR,
				CMD_ENDIF,
			CMD_ELSE,
				SP_CMD( pl24_sp_killerbee, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_RND( 8 ),
			WAIT( 10 ),
			WAIT2( 8 ),
			IF_RND( 16 ),
				SP_CMD( pl24_sp_smasher, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				SP_CMD( pl24_sp_killerbee, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ELSE,
			WAIT2( 8 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck24_cmd_07[] = {
	STAND,
	IF_RND( 14 ),
		IF_AREA( 32, 0, 32, 0 ),
			SP_CMD( pl24_sp_revenge, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			CMD_JUMP( 21 ),
		CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 21 ),
		IF_RND( 8 ),
			CMD_JUMP( 21 ),
		CMD_ELSE,
			WAIT2( 4 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck24_cmd_08[] = {
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
				CMD_JUMP( 12 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck24_cmd_09[] = {
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
			SP_CMD( pl24_sp_smasher, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			IF_RND( 16 ),
				WAIT( 10 ),
				WAIT2( 6 ),
				JUMP( DIR_BACK ),
				JUMP_FLAG_SET,
					SP_CMD( pl24_sp_killerbee, 0, PK_PUNCH, LV_LIGHT, 0 ),
					JUMP_FLAG_CLR,
				CMD_ENDIF,
			CMD_ELSE,
				SP_CMD( pl24_sp_killerbee, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_RND( 8 ),
			WAIT( 10 ),
			WAIT2( 8 ),
			IF_RND( 16 ),
				SP_CMD( pl24_sp_smasher, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				SP_CMD( pl24_sp_killerbee, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ELSE,
			WAIT2( 4 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck24_cmd_10[] = {
	STAND,
	IF_RND( 8 ),
		IF_AREA( 32, 0, 32, 0 ),
			SP_CMD( pl24_sp_revenge, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			CMD_JUMP( 21 ),
		CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 21 ),
		IF_RND( 8 ),
			CMD_JUMP( 21 ),
		CMD_ELSE,
			WAIT2( 4 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck24_cmd_11[] = {
	IF_RND( 20 ),
		CMD_JUMP( 17 ),
	CMD_ELSE,
		CMD_JUMP( 25 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck24_cmd_12[] = {
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
				CMD_JUMP( 12 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck24_cmd_13[] = {
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

const Uint8 atck24_cmd_14[] = {
	CMD_JUMP( 13 ),
	CMD_JUMP( 0 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck24_cmd_15[] = {
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

const Uint8 atck24_cmd_16[] = {
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
				CANCEL_SET,
					CROUCH,
					KICK( LV_HEAVY ),
					SP_CMD( pl24_sp_arrow, 0, PK_PUNCH, LV_HEAVY, 0 ),
					CANCEL_CLR,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			TIMER_SET( 30 ),
			WALK_ATCK( ATCK_CROUCH_HK ),
				CROUCH,
				KICK( LV_HEAVY ),
				STAND,
				WAIT( 10 ),
				SP_CMD( pl24_sp_knuckle, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ELSE,
				JUMP( DIR_BACK ),
				JUMP_FLAG_SET,
					WAIT( 40 ),
					AIR_JUMP( DIR_UP ),
					WAIT( 40 ),
					IF_RND( 16 ),
						SP_CMD( pl24_sp_strike, 0, PK_PUNCH, LV_HEAVY, 0 ),
					CMD_ELSE,
						SP_CMD( pl24_sp_drill, 0, PK_PUNCH, LV_HEAVY, 0 ),
					CMD_ENDIF,
					JUMP_FLAG_CLR,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck24_cmd_17[] = {
	CMD_JUMP( 28 ),
	IF_VITAL( 80 ),
		IF_EM_ATCK( ATCK_STAND_LK ),
			CANCEL_SET2,
				PUNCH( LV_LIGHT ),
				SP_CMD( pl24_sp_knuckle, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			WAIT2( 4 ),
		CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 0 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck24_cmd_18[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck24_cmd_19[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck24_cmd_20[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck24_cmd_21[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck24_cmd_22[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck24_cmd_23[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck24_cmd_24[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck24_cmd_25[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck24_cmd_26[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck24_cmd_27[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck24_cmd_28[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck24_cmd_29[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck24_cmd_30[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck24_cmd_31[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

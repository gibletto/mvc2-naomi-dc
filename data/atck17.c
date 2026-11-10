/* Cable (character 17): the computer's attack lists.
   What it does once a waiting list's CMD_NEXT has sent it to attack. PLAYER_WORK's cpu_adrs[1]
   points here.

   atck17_tbl     one record (CPU_DATA): the offsets of the three tables.
   atck17_rate    the chance, out of 32, that a list's CMD_NEXT2 starts another attack list, by
                  the level of play.
   atck17_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   atck17_cmd     the lists: a pick is a number in this table.
   atck17_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                  list of lib17.c. */
#include "player.h"
#include "pl17_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const atck17_sel[], *const atck17_cmd[];
extern const Uint8 atck17_rate[], atck17_sel_0[], atck17_sel_1[], atck17_sel_2[], atck17_sel_3[],
	atck17_sel_4[], atck17_sel_5[], atck17_sel_6[], atck17_sel_7[], atck17_cmd_00[], atck17_cmd_01[],
	atck17_cmd_02[], atck17_cmd_03[], atck17_cmd_04[], atck17_cmd_05[], atck17_cmd_06[],
	atck17_cmd_07[], atck17_cmd_08[], atck17_cmd_09[], atck17_cmd_10[], atck17_cmd_11[],
	atck17_cmd_12[], atck17_cmd_13[], atck17_cmd_14[], atck17_cmd_15[], atck17_cmd_16[],
	atck17_cmd_17[], atck17_cmd_18[], atck17_cmd_19[], atck17_cmd_20[], atck17_cmd_21[],
	atck17_cmd_22[], atck17_cmd_23[], atck17_cmd_24[], atck17_cmd_25[], atck17_cmd_26[],
	atck17_cmd_27[], atck17_cmd_28[], atck17_cmd_29[], atck17_cmd_30[], atck17_cmd_31[];

const CPU_DATA atck17_tbl[] = {
	{ ( Sint32 )atck17_rate, ( Sint32 )atck17_sel, ( Sint32 )atck17_cmd },
};

const Uint8 atck17_rate[] = {
	  8,   8,   8,   8,   9,   9,  10,  10,  11,  11,  12,  12,  13,  13,  14,  14,
	 15,  15,  16,  16,  17,  17,  18,  18,  19,  20,  21,  22,  24,  26,  28,  30,
};

const Uint8 *const atck17_sel[] = {
	atck17_sel_0, atck17_sel_1, atck17_sel_2, atck17_sel_3, atck17_sel_4, atck17_sel_5,
	atck17_sel_6, atck17_sel_7,
};

const Uint8 atck17_sel_0[] = {
	  0,   1,   2,  24,   0,  27,   2,   3,   0,   1,  26,   3,  24,   1,   2,   3,
	  0,  24,   2,   3,   0,   1,  26,   3,  25,   1,   2,  24,  25,  27,   2,   3,
};

const Uint8 atck17_sel_1[] = {
	  0,   1,  26,   3,   0,  24,   2,   3,  26,  27,   2,  24,   0,  27,   2,   3,
	  0,   1,  25,   3,  24,   1,  25,   3,   5,   6,   2,   4,  25,   7,  24,   5,
};

const Uint8 atck17_sel_2[] = {
	  0,   1,   2,   3,  24,  26,   2,   3,   0,  26,   2,   3,  27,   1,   2,   3,
	 24,   6,  27,   4,  24,   7,  26,   5,  25,   8,   9,   4,   5,   8,   9,   5,
};

const Uint8 atck17_sel_3[] = {
	  0,   1,   2,   3,   0,   1,  27,   3,  24,   6,   2,   4,  27,   7,  26,   5,
	  5,  26,   9,   4,  25,   8,   9,   5,  25,  10,  11,  12,  24,  10,  11,   9,
};

const Uint8 atck17_sel_4[] = {
	  0,   6,   2,   4,  24,   7,   2,   5,  21,   8,   6,   4,  26,  27,   9,   5,
	  5,  10,  11,  26,  27,  10,  26,   9,  25,  13,  14,  12,  23,  13,  14,  12,
};

const Uint8 atck17_sel_5[] = {
	  0,   2,   9,   4,  24,   8,   9,   5,   5,   6,  11,  12,  26,  10,  11,  27,
	 25,  13,  14,  27,  21,  13,  14,  10,  22,  15,  16,  14,  23,  15,  16,  13,
};

const Uint8 atck17_sel_6[] = {
	  0,  10,  11,   2,  24,  10,  11,   9,  21,  13,  14,  12,   5,  13,   6,  27,
	 25,  15,  16,  14,  25,  27,  16,  13,  22,  17,  26,  18,  23,  17,  26,  15,
};

const Uint8 atck17_sel_7[] = {
	  0,  13,  14,   2,  24,  13,  14,  12,  24,  15,  16,  14,   5,  15,  27,  13,
	 21,  17,   6,  18,  21,  17,  18,  15,  23,  19,  26,  27,  22,  26,  20,  17,
};

const Uint8 *const atck17_cmd[] = {
	atck17_cmd_00, atck17_cmd_01, atck17_cmd_02, atck17_cmd_03, atck17_cmd_04, atck17_cmd_05,
	atck17_cmd_06, atck17_cmd_07, atck17_cmd_08, atck17_cmd_09, atck17_cmd_10, atck17_cmd_11,
	atck17_cmd_12, atck17_cmd_13, atck17_cmd_14, atck17_cmd_15, atck17_cmd_16, atck17_cmd_17,
	atck17_cmd_18, atck17_cmd_19, atck17_cmd_20, atck17_cmd_21, atck17_cmd_22, atck17_cmd_23,
	atck17_cmd_24, atck17_cmd_25, atck17_cmd_26, atck17_cmd_27, atck17_cmd_28, atck17_cmd_29,
	atck17_cmd_30, atck17_cmd_31,
};

const Uint8 atck17_cmd_00[] = {
	STAND,
	CMD_JUMP( 68 ),
	WAIT2( 10 ),
	WALK_B( 512 ),
	CMD_JUMP( 61 ),
	WAIT2( 8 ),
	CMD_JUMP( 60 ),
	CMD_JUMP( 0 ),
	CMD_EXIT,
};

const Uint8 atck17_cmd_01[] = {
	STAND,
	CMD_JUMP( 68 ),
	CMD_JUMP( 61 ),
	WAIT2( 10 ),
	CMD_JUMP( 65 ),
	CMD_JUMP( 0 ),
	CMD_EXIT,
};

const Uint8 atck17_cmd_02[] = {
	STAND,
	CMD_JUMP( 68 ),
	WAIT2( 8 ),
	TIMER_SET( 60 ),
	WALK_EM( 64 ),
	CMD_JUMP( 61 ),
	WAIT2( 10 ),
	CMD_JUMP( 11 ),
	STAND,
	WAIT2( 8 ),
	CMD_JUMP( 0 ),
	CMD_EXIT,
};

const Uint8 atck17_cmd_03[] = {
	STAND,
	CMD_JUMP( 68 ),
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
	CMD_JUMP( 0 ),
	CMD_EXIT,
};

const Uint8 atck17_cmd_04[] = {
	STAND,
	CMD_JUMP( 68 ),
	WAIT2( 14 ),
	TIMER_SET( 60 ),
	CMD_JUMP( 61 ),
	IF_EM_ATCK( ATCK_CROUCH_HP ),
		SP_CMD( pl17_sp_psimitar, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ELSE,
		SP_CMD( pl17_sp_psimitar, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ENDIF,
	STAND,
	CMD_NEXT2,
	CMD_JUMP( 0 ),
	CMD_EXIT,
};

const Uint8 atck17_cmd_05[] = {
	STAND,
	WAIT2( 4 ),
	CMD_JUMP( 68 ),
	IF_RND( 16 ),
		CMD_JUMP( 59 ),
	CMD_ELSE,
		CMD_JUMP( 60 ),
	CMD_ENDIF,
	STAND,
	CMD_JUMP( 0 ),
	CMD_EXIT,
};

const Uint8 atck17_cmd_06[] = {
	STAND,
	CMD_JUMP( 68 ),
	WAIT2( 4 ),
	CMD_JUMP( 0 ),
	CMD_JUMP( 17 ),
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_JUMP( 0 ),
	CMD_EXIT,
};

const Uint8 atck17_cmd_07[] = {
	STAND,
	CMD_JUMP( 68 ),
	WAIT2( 4 ),
	CMD_JUMP( 62 ),
	STAND,
	CMD_JUMP( 0 ),
	CMD_EXIT,
};

const Uint8 atck17_cmd_08[] = {
	STAND,
	CMD_JUMP( 68 ),
	CMD_JUMP( 61 ),
	CMD_JUMP( 29 ),
	STAND,
	CMD_JUMP( 0 ),
	CMD_EXIT,
};

const Uint8 atck17_cmd_09[] = {
	STAND,
	CMD_JUMP( 68 ),
	CMD_JUMP( 61 ),
	CMD_JUMP( 23 ),
	CMD_JUMP( 0 ),
	CMD_EXIT,
};

const Uint8 atck17_cmd_10[] = {
	STAND,
	CMD_JUMP( 68 ),
	CMD_JUMP( 61 ),
	CMD_JUMP( 10 ),
	CMD_JUMP( 0 ),
	CMD_EXIT,
};

const Uint8 atck17_cmd_11[] = {
	STAND,
	CMD_JUMP( 68 ),
	CMD_JUMP( 61 ),
	CMD_JUMP( 28 ),
	CMD_JUMP( 0 ),
	CMD_EXIT,
};

const Uint8 atck17_cmd_12[] = {
	STAND,
	CMD_JUMP( 68 ),
	CMD_JUMP( 61 ),
	CMD_JUMP( 64 ),
	CMD_JUMP( 0 ),
	CMD_EXIT,
};

const Uint8 atck17_cmd_13[] = {
	STAND,
	CMD_JUMP( 68 ),
	JUMP( DIR_BACK ),
	CMD_JUMP( 61 ),
	IF_RND( 16 ),
		CMD_JUMP( 39 ),
	CMD_ELSE,
		CMD_JUMP( 40 ),
	CMD_ENDIF,
	CMD_JUMP( 0 ),
	CMD_EXIT,
};

const Uint8 atck17_cmd_14[] = {
	STAND,
	CMD_JUMP( 68 ),
	CMD_JUMP( 63 ),
	CMD_JUMP( 0 ),
	CMD_EXIT,
};

const Uint8 atck17_cmd_15[] = {
	STAND,
	CMD_JUMP( 68 ),
	CMD_JUMP( 66 ),
	CMD_JUMP( 4 ),
	CMD_JUMP( 0 ),
	CMD_EXIT,
};

const Uint8 atck17_cmd_16[] = {
	STAND,
	CMD_JUMP( 68 ),
	CMD_JUMP( 66 ),
	CMD_JUMP( 5 ),
	CMD_JUMP( 0 ),
	CMD_EXIT,
};

const Uint8 atck17_cmd_17[] = {
	STAND,
	CMD_JUMP( 68 ),
	CMD_JUMP( 66 ),
	CMD_JUMP( 6 ),
	CMD_JUMP( 0 ),
	CMD_EXIT,
};

const Uint8 atck17_cmd_18[] = {
	STAND,
	CMD_JUMP( 68 ),
	CMD_JUMP( 7 ),
	CMD_JUMP( 0 ),
	CMD_EXIT,
};

const Uint8 atck17_cmd_19[] = {
	STAND,
	CMD_JUMP( 68 ),
	CMD_JUMP( 8 ),
	CMD_JUMP( 0 ),
	CMD_EXIT,
};

const Uint8 atck17_cmd_20[] = {
	STAND,
	CMD_JUMP( 68 ),
	CMD_JUMP( 9 ),
	CMD_JUMP( 0 ),
	CMD_EXIT,
};

const Uint8 atck17_cmd_21[] = {
	STAND,
	CMD_JUMP( 68 ),
	CMD_JUMP( 54 ),
	CMD_JUMP( 0 ),
	CMD_EXIT,
};

const Uint8 atck17_cmd_22[] = {
	STAND,
	CMD_JUMP( 68 ),
	CMD_JUMP( 55 ),
	CMD_JUMP( 0 ),
	CMD_EXIT,
};

const Uint8 atck17_cmd_23[] = {
	STAND,
	CMD_JUMP( 68 ),
	CMD_JUMP( 55 ),
	CMD_JUMP( 0 ),
	CMD_EXIT,
};

const Uint8 atck17_cmd_24[] = {
	STAND,
	CMD_JUMP( 68 ),
	IF_RND( 16 ),
		SP_CMD( pl17_sp_crackdown, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ELSE,
		IF_RND( 16 ),
			SP_CMD( pl17_sp_electrap, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			SP_CMD( pl17_sp_viper, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_JUMP( 0 ),
	CMD_EXIT,
};

const Uint8 atck17_cmd_25[] = {
	STAND,
	CMD_JUMP( 68 ),
	CMD_JUMP( 13 ),
	CMD_JUMP( 58 ),
	JUMP_LEVEL( 59, 8, 7, 16, 8, 24, 9 ),
	CMD_JUMP( 0 ),
	CMD_EXIT,
};

const Uint8 atck17_cmd_26[] = {
	STAND,
	CMD_JUMP( 68 ),
	WAIT2( 8 ),
	CMD_JUMP( 61 ),
	WAIT2( 8 ),
	CMD_JUMP( 67 ),
	WAIT2( 8 ),
	CMD_JUMP( 0 ),
	CMD_EXIT,
};

const Uint8 atck17_cmd_27[] = {
	STAND,
	WAIT2( 8 ),
	CMD_JUMP( 68 ),
	CMD_JUMP( 53 ),
	CMD_JUMP( 0 ),
	CMD_EXIT,
};

const Uint8 atck17_cmd_28[] = {
	CMD_EXIT,
};

const Uint8 atck17_cmd_29[] = {
	CMD_EXIT,
};

const Uint8 atck17_cmd_30[] = {
	CMD_EXIT,
};

const Uint8 atck17_cmd_31[] = {
	CMD_EXIT,
};

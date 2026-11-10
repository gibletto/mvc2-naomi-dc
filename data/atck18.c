/* Abyss (first form) (character 18): the computer's attack lists.
   What it does once a waiting list's CMD_NEXT has sent it to attack. PLAYER_WORK's cpu_adrs[1]
   points here.

   atck18_tbl     one record (CPU_DATA): the offsets of the three tables.
   atck18_rate    the chance, out of 32, that a list's CMD_NEXT2 starts another attack list, by
                  the level of play.
   atck18_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   atck18_cmd     the lists: a pick is a number in this table.
   atck18_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                  list of lib18.c. */
#include "player.h"
#include "pl18_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const atck18_sel[], *const atck18_cmd[];
extern const Uint8 atck18_rate[], atck18_sel_0[], atck18_sel_1[], atck18_sel_2[], atck18_sel_3[],
	atck18_sel_4[], atck18_sel_5[], atck18_sel_6[], atck18_sel_7[], atck18_cmd_00[], atck18_cmd_01[],
	atck18_cmd_02[], atck18_cmd_03[], atck18_cmd_04[], atck18_cmd_05[], atck18_cmd_06[],
	atck18_cmd_07[], atck18_cmd_08[], atck18_cmd_09[], atck18_cmd_10[], atck18_cmd_11[],
	atck18_cmd_12[], atck18_cmd_13[], atck18_cmd_14[], atck18_cmd_15[], atck18_cmd_16[],
	atck18_cmd_17[], atck18_cmd_18[], atck18_cmd_19[], atck18_cmd_20[], atck18_cmd_21[],
	atck18_cmd_22[], atck18_cmd_23[], atck18_cmd_24[], atck18_cmd_25[], atck18_cmd_26[],
	atck18_cmd_27[], atck18_cmd_28[], atck18_cmd_29[], atck18_cmd_30[], atck18_cmd_31[];

const CPU_DATA atck18_tbl[] = {
	{ ( Sint32 )atck18_rate, ( Sint32 )atck18_sel, ( Sint32 )atck18_cmd },
};

const Uint8 atck18_rate[] = {
	  8,   8,   8,   8,   9,   9,  10,  10,  11,  11,  12,  12,  13,  13,  14,  14,
	 15,  15,  16,  16,  17,  17,  18,  18,  19,  20,  21,  22,  24,  26,  28,  30,
};

const Uint8 *const atck18_sel[] = {
	atck18_sel_0, atck18_sel_1, atck18_sel_2, atck18_sel_3, atck18_sel_4, atck18_sel_5,
	atck18_sel_6, atck18_sel_7,
};

const Uint8 atck18_sel_0[] = {
	 17,   0,   0,   0,   1,   2,   3,   3,   0,   0,   0,   0,   1,   2,   3,   3,
	  0,   0,   0,   0,   1,   2,   3,   3,   6,   6,   7,   7,   1,   2,   3,   3,
};

const Uint8 atck18_sel_1[] = {
	 17,   0,   0,   0,   1,   1,   3,   4,   0,   0,   0,   0,   1,   1,   3,   4,
	  0,   0,   0,   0,   1,   1,   3,   4,   0,   0,   0,   0,   1,   1,   3,   4,
};

const Uint8 atck18_sel_2[] = {
	 17,   7,   1,   1,   2,   6,   3,   3,   7,   7,   1,   1,   2,   6,   3,   3,
	  7,   7,   1,   1,   4,   6,   3,   3,   7,   7,   1,   1,   2,   6,   3,   3,
};

const Uint8 atck18_sel_3[] = {
	 12,  11,   1,   1,   2,   6,   3,   8,  12,  11,   1,   1,   2,   6,   3,   8,
	 12,  11,   1,   1,   2,   6,   3,   9,   4,   4,  13,  13,   2,   6,   3,   9,
};

const Uint8 atck18_sel_4[] = {
	 11,  11,   1,   1,   3,   2,   8,   8,  11,  11,   1,   1,   3,   6,   8,   8,
	 11,  11,  13,  13,   4,  12,   9,   9,  11,  11,  13,  13,   4,  12,   9,   9,
};

const Uint8 atck18_sel_5[] = {
	 17,  11,   1,   1,   3,   2,   8,   8,  17,  11,   1,  13,   3,   6,   8,   8,
	 17,  11,  13,  13,   4,  12,   9,   9,  11,  11,  13,  13,   4,  12,   9,   9,
};

const Uint8 atck18_sel_6[] = {
	 17,  11,   1,  13,   3,   2,   8,   8,  17,  11,  13,   1,   3,   6,   8,   8,
	 17,  11,  13,   1,   4,  12,   9,   9,  17,  11,  13,  13,   4,  12,   9,   9,
};

const Uint8 atck18_sel_7[] = {
	 17,  11,  13,  13,   3,   2,   8,   8,  17,  11,  13,  13,   3,   6,   8,   8,
	 17,  11,  13,   4,   4,  12,   9,   9,  17,  11,  13,   4,   4,  12,   9,   9,
};

const Uint8 *const atck18_cmd[] = {
	atck18_cmd_00, atck18_cmd_01, atck18_cmd_02, atck18_cmd_03, atck18_cmd_04, atck18_cmd_05,
	atck18_cmd_06, atck18_cmd_07, atck18_cmd_08, atck18_cmd_09, atck18_cmd_10, atck18_cmd_11,
	atck18_cmd_12, atck18_cmd_13, atck18_cmd_14, atck18_cmd_15, atck18_cmd_16, atck18_cmd_17,
	atck18_cmd_18, atck18_cmd_19, atck18_cmd_20, atck18_cmd_21, atck18_cmd_22, atck18_cmd_23,
	atck18_cmd_24, atck18_cmd_25, atck18_cmd_26, atck18_cmd_27, atck18_cmd_28, atck18_cmd_29,
	atck18_cmd_30, atck18_cmd_31,
};

const Uint8 atck18_cmd_00[] = {
	STAND,
	WAIT2( 8 ),
	SP_CMD( pl18_sp_punch, 0, PK_PUNCH, LV_LIGHT, 0 ),
	WAIT2( 14 ),
	SP_CMD( pl18_sp_punch, 0, PK_PUNCH, LV_HEAVY, 0 ),
	IF_LEVEL( 24 ),
		IF_GAUGE( 3 ),
			IF_RND( 2 ),
				SP_CMD( pl18_sp_pillar, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				SP_CMD( pl18_sp_thunder, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		WAIT2( 14 ),
		SP_CMD( pl18_sp_rush, 0, PK_PUNCH, LV_LIGHT, 0 ),
		WAIT2( 14 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck18_cmd_01[] = {
	STAND,
	WAIT2( 8 ),
	CMD_JUMP( 14 ),
	STAND,
	IF_LEVEL( 24 ),
		CMD_JUMP( 8 ),
	CMD_ELSE,
		CMD_JUMP( 7 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 8 ),
	CMD_EXIT,
};

const Uint8 atck18_cmd_02[] = {
	STAND,
	WAIT2( 8 ),
	WALK_EM( 64 ),
	CMD_JUMP( 10 ),
	STAND,
	WAIT2( 8 ),
	CMD_EXIT,
};

const Uint8 atck18_cmd_03[] = {
	STAND,
	WAIT2( 8 ),
	SP_CMD( pl18_sp_rush, 0, PK_PUNCH, LV_LIGHT, 0 ),
	WAIT2( 8 ),
	CMD_EXIT,
};

const Uint8 atck18_cmd_04[] = {
	STAND,
	WAIT2( 14 ),
	TIMER_SET( 60 ),
	IF_EM_ATCK( ATCK_CROUCH_HP ),
		SP_CMD( pl18_sp_sweep, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ELSE,
		SP_CMD( pl18_sp_punch, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ENDIF,
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck18_cmd_05[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck18_cmd_06[] = {
	STAND,
	WAIT2( 4 ),
	IF_AREA( 64, 64, 16, 16 ),
		SP_CMD( pl18_sp_rush, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ENDIF,
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck18_cmd_07[] = {
	STAND,
	WAIT2( 4 ),
	SP_CMD( pl18_sp_ball, 0, PK_PUNCH, LV_LIGHT, 0 ),
	STAND,
	WAIT2( 2 ),
	SP_CMD( pl18_sp_rush, 0, PK_PUNCH, LV_LIGHT, 0 ),
	STAND,
	CMD_EXIT,
};

const Uint8 atck18_cmd_08[] = {
	STAND,
	IF_GAUGE( 1 ),
		IF_RND( 16 ),
			WAIT2( 2 ),
			IF_AREA( 240, 48, 128, 16 ),
				SP_CMD( pl18_sp_sweep, 0, PK_PUNCH, LV_LIGHT, 0 ),
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

const Uint8 atck18_cmd_09[] = {
	STAND,
	IF_GAUGE( 1 ),
		IF_RND( 16 ),
			SP_CMD( pl18_sp_pillar, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			SP_CMD( pl18_sp_thunder, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ELSE,
		STAND,
		WAIT2( 2 ),
		SP_CMD( pl18_sp_rush, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck18_cmd_10[] = {
	CMD_EXIT,
};

const Uint8 atck18_cmd_11[] = {
	STAND,
	WAIT2( 2 ),
	IF_AREA( 128, 64, 128, 16 ),
		SP_CMD( pl18_sp_punch, 0, PK_PUNCH, LV_HEAVY, 0 ),
		STAND,
		CMD_JUMP( 24 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	IF_RND( 16 ),
		WAIT2( 2 ),
		IF_AREA( 128, 64, 128, 16 ),
			IF_LEVEL( 24 ),
				IF_GAUGE( 2 ),
					IF_RND( 10 ),
						SP_CMD( pl18_sp_sweep, 0, PK_PUNCH, LV_LIGHT, 0 ),
					CMD_ENDIF,
				CMD_ELSE,
					SP_CMD( pl18_sp_punch, 0, PK_PUNCH, LV_HEAVY, 0 ),
					CMD_JUMP( 24 ),
				CMD_ENDIF,
			CMD_ELSE,
				SP_CMD( pl18_sp_punch, 0, PK_PUNCH, LV_HEAVY, 0 ),
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

const Uint8 atck18_cmd_12[] = {
	STAND,
	WAIT2( 2 ),
	SP_CMD( pl18_sp_pillar, 0, PK_PUNCH, LV_LIGHT, 0 ),
	WAIT2( 8 ),
	CMD_JUMP( 21 ),
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck18_cmd_13[] = {
	STAND,
	LOCK_SET,
	WAIT2( 6 ),
	CMD_JUMP( 0 ),
	CMD_JUMP( 25 ),
	CMD_DUMMY,
	STAND,
	JUMP_LEVEL( 7, 8, 7, 15, 7, 24, 7 ),
	LOCK_CLR,
	WAIT2( 4 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck18_cmd_14[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck18_cmd_15[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck18_cmd_16[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck18_cmd_17[] = {
	CMD_JUMP( 56 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck18_cmd_18[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck18_cmd_19[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck18_cmd_20[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck18_cmd_21[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck18_cmd_22[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck18_cmd_23[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck18_cmd_24[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck18_cmd_25[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck18_cmd_26[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck18_cmd_27[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck18_cmd_28[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck18_cmd_29[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck18_cmd_30[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck18_cmd_31[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

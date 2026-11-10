/* Abyss (second form) (character 19): the computer's attack lists.
   What it does once a waiting list's CMD_NEXT has sent it to attack. PLAYER_WORK's cpu_adrs[1]
   points here.

   atck19_tbl     one record (CPU_DATA): the offsets of the three tables.
   atck19_rate    the chance, out of 32, that a list's CMD_NEXT2 starts another attack list, by
                  the level of play.
   atck19_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   atck19_cmd     the lists: a pick is a number in this table.
   atck19_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                  list of lib19.c. */
#include "player.h"
#include "pl19_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const atck19_sel[], *const atck19_cmd[];
extern const Uint8 atck19_rate[], atck19_sel_0[], atck19_sel_1[], atck19_sel_2[], atck19_sel_3[],
	atck19_sel_4[], atck19_sel_5[], atck19_sel_6[], atck19_sel_7[], atck19_cmd_00[], atck19_cmd_01[],
	atck19_cmd_02[], atck19_cmd_03[], atck19_cmd_04[], atck19_cmd_05[], atck19_cmd_06[],
	atck19_cmd_07[], atck19_cmd_08[], atck19_cmd_09[], atck19_cmd_10[], atck19_cmd_11[],
	atck19_cmd_12[], atck19_cmd_13[], atck19_cmd_14[], atck19_cmd_15[], atck19_cmd_16[],
	atck19_cmd_17[], atck19_cmd_18[], atck19_cmd_19[], atck19_cmd_20[], atck19_cmd_21[],
	atck19_cmd_22[], atck19_cmd_23[], atck19_cmd_24[], atck19_cmd_25[], atck19_cmd_26[],
	atck19_cmd_27[], atck19_cmd_28[], atck19_cmd_29[], atck19_cmd_30[], atck19_cmd_31[];

const CPU_DATA atck19_tbl[] = {
	{ ( Sint32 )atck19_rate, ( Sint32 )atck19_sel, ( Sint32 )atck19_cmd },
};

const Uint8 atck19_rate[] = {
	  8,   8,   8,   8,   9,   9,  10,  10,  11,  11,  12,  12,  13,  13,  14,  14,
	 15,  15,  16,  16,  17,  17,  18,  18,  19,  20,  21,  22,  24,  26,  28,  30,
};

const Uint8 *const atck19_sel[] = {
	atck19_sel_0, atck19_sel_1, atck19_sel_2, atck19_sel_3, atck19_sel_4, atck19_sel_5,
	atck19_sel_6, atck19_sel_7,
};

const Uint8 atck19_sel_0[] = {
	 16,   0,   0,   0,   1,   2,   3,   3,   0,   0,   0,   0,   1,   2,   3,   3,
	  0,   0,   0,   0,   1,   2,   3,   3,   6,   6,   7,   7,   1,   2,   3,   3,
};

const Uint8 atck19_sel_1[] = {
	 17,   0,   0,   0,   1,   1,   3,   4,   0,   0,   0,   0,   1,   1,   3,   4,
	  0,   0,   0,   0,   1,   1,   3,   4,   0,   0,   0,   0,   1,   1,   3,   4,
};

const Uint8 atck19_sel_2[] = {
	 18,   7,   1,   1,   2,   6,   3,   3,   7,   7,   1,   1,   2,   6,   3,   3,
	  7,   7,   1,   1,   5,   6,   3,   3,   7,   7,   1,   1,   2,   6,   3,   3,
};

const Uint8 atck19_sel_3[] = {
	 12,  11,   1,   1,   2,   6,   3,   8,  12,  11,   1,   1,   2,   6,   3,   8,
	 12,  11,   1,   1,   2,   6,   3,   9,   5,   5,  13,  13,   2,   6,   3,   9,
};

const Uint8 atck19_sel_4[] = {
	 11,  11,   1,   1,   3,   2,   8,   8,  11,  11,   1,   1,   3,   6,   8,   8,
	 11,  11,  13,  13,   5,  12,   9,  10,  11,  11,  13,  13,   5,  12,   9,  10,
};

const Uint8 atck19_sel_5[] = {
	 16,  11,   1,   1,   3,   2,   8,   8,  17,  11,   1,  13,   3,   6,   8,   8,
	 18,  11,  13,  13,   5,  12,   9,  10,  11,  11,  13,  13,   5,  12,   9,  10,
};

const Uint8 atck19_sel_6[] = {
	 16,  11,   1,  13,   3,   2,   8,   8,  16,  11,  13,   1,   3,   6,   8,   8,
	 17,  11,  13,   1,   5,  12,   9,  10,  18,  11,  13,  13,   5,  12,   9,  10,
};

const Uint8 atck19_sel_7[] = {
	 16,  11,  13,  13,   3,   2,   8,   8,  16,  11,  13,  13,   3,   6,   8,   8,
	 17,  11,  13,   5,   5,  12,   9,  10,  18,  11,  13,   5,   5,  12,   9,  10,
};

const Uint8 *const atck19_cmd[] = {
	atck19_cmd_00, atck19_cmd_01, atck19_cmd_02, atck19_cmd_03, atck19_cmd_04, atck19_cmd_05,
	atck19_cmd_06, atck19_cmd_07, atck19_cmd_08, atck19_cmd_09, atck19_cmd_10, atck19_cmd_11,
	atck19_cmd_12, atck19_cmd_13, atck19_cmd_14, atck19_cmd_15, atck19_cmd_16, atck19_cmd_17,
	atck19_cmd_18, atck19_cmd_19, atck19_cmd_20, atck19_cmd_21, atck19_cmd_22, atck19_cmd_23,
	atck19_cmd_24, atck19_cmd_25, atck19_cmd_26, atck19_cmd_27, atck19_cmd_28, atck19_cmd_29,
	atck19_cmd_30, atck19_cmd_31,
};

const Uint8 atck19_cmd_00[] = {
	STAND,
	WAIT2( 8 ),
	SP_CMD( pl19_sp_blast, 0, PK_PUNCH, LV_LIGHT, 0 ),
	WAIT2( 14 ),
	SP_CMD( pl19_sp_beam, 0, PK_PUNCH, LV_LIGHT, 0 ),
	IF_LEVEL( 24 ),
		IF_RND( 2 ),
			SP_CMD( pl19_sp_bubble, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ELSE,
		WAIT2( 14 ),
		SP_CMD( pl19_sp_beam, 0, PK_PUNCH, LV_HEAVY, 0 ),
		WAIT2( 14 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck19_cmd_01[] = {
	STAND,
	WAIT2( 8 ),
	SP_CMD( pl19_sp_blast, 0, PK_PUNCH, LV_LIGHT, 0 ),
	WAIT2( 8 ),
	SP_CMD( pl19_sp_blast, 0, PK_PUNCH, LV_LIGHT, 0 ),
	WAIT2( 8 ),
	SP_CMD( pl19_sp_beam, 0, PK_PUNCH, LV_HEAVY, 0 ),
	WAIT2( 14 ),
	CMD_EXIT,
};

const Uint8 atck19_cmd_02[] = {
	STAND,
	WAIT2( 8 ),
	WALK_EM( 128 ),
	WAIT2( 8 ),
	SP_CMD( pl19_sp_blast, 0, PK_PUNCH, LV_LIGHT, 0 ),
	WAIT2( 0 ),
	SP_CMD( pl19_sp_storm, 0, PK_PUNCH, LV_LIGHT, 0 ),
	WAIT2( 8 ),
	CMD_EXIT,
};

const Uint8 atck19_cmd_03[] = {
	STAND,
	WAIT2( 8 ),
	SP_CMD( pl19_sp_blast, 0, PK_PUNCH, LV_LIGHT, 0 ),
	WAIT2( 14 ),
	CMD_EXIT,
};

const Uint8 atck19_cmd_04[] = {
	STAND,
	WAIT2( 14 ),
	TIMER_SET( 60 ),
	IF_EM_ATCK( ATCK_CROUCH_HP ),
		SP_CMD( pl19_sp_storm, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ELSE,
		SP_CMD( pl19_sp_bubble, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ENDIF,
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck19_cmd_05[] = {
	STAND,
	WAIT2( 4 ),
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck19_cmd_06[] = {
	STAND,
	WAIT2( 4 ),
	CMD_JUMP( 0 ),
	IF_AREA( 64, 64, 16, 16 ),
		WAIT2( 4 ),
		SP_CMD( pl19_sp_blast, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck19_cmd_07[] = {
	STAND,
	WAIT2( 4 ),
	SP_CMD( pl19_sp_blast, 0, PK_PUNCH, LV_LIGHT, 0 ),
	STAND,
	WAIT2( 2 ),
	STAND,
	CMD_EXIT,
};

const Uint8 atck19_cmd_08[] = {
	STAND,
	IF_GAUGE( 1 ),
		IF_RND( 16 ),
			WAIT2( 2 ),
			IF_AREA( 240, 48, 128, 16 ),
				SP_CMD( pl19_sp_storm, 0, PK_PUNCH, LV_LIGHT, 0 ),
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

const Uint8 atck19_cmd_09[] = {
	STAND,
	IF_GAUGE( 1 ),
		IF_RND( 16 ),
			IF_EM_ATCK( 81 ),
				SP_CMD( pl19_sp_storm, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		STAND,
		WAIT2( 2 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck19_cmd_10[] = {
	CMD_EXIT,
};

const Uint8 atck19_cmd_11[] = {
	STAND,
	WAIT2( 2 ),
	IF_AREA( 128, 64, 128, 16 ),
		SP_CMD( pl19_sp_bubble, 0, PK_PUNCH, LV_LIGHT, 0 ),
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
						SP_CMD( pl19_sp_storm, 0, PK_PUNCH, LV_LIGHT, 0 ),
					CMD_ENDIF,
				CMD_ELSE,
					SP_CMD( pl19_sp_beam, 0, PK_PUNCH, LV_HEAVY, 0 ),
					CMD_JUMP( 24 ),
				CMD_ENDIF,
			CMD_ELSE,
				SP_CMD( pl19_sp_beam, 0, PK_PUNCH, LV_LIGHT, 0 ),
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

const Uint8 atck19_cmd_12[] = {
	STAND,
	WAIT2( 2 ),
	CMD_JUMP( 14 ),
	CMD_JUMP( 20 ),
	WAIT( 6 ),
	SP_CMD( pl19_sp_blast, 0, PK_PUNCH, LV_LIGHT, 0 ),
	WAIT2( 8 ),
	CMD_JUMP( 21 ),
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck19_cmd_13[] = {
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

const Uint8 atck19_cmd_14[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck19_cmd_15[] = {
	CMD_EXIT,
};

const Uint8 atck19_cmd_16[] = {
	CMD_JUMP( 54 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck19_cmd_17[] = {
	CMD_JUMP( 56 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck19_cmd_18[] = {
	CMD_JUMP( 53 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck19_cmd_19[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck19_cmd_20[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck19_cmd_21[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck19_cmd_22[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck19_cmd_23[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck19_cmd_24[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck19_cmd_25[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck19_cmd_26[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck19_cmd_27[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck19_cmd_28[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck19_cmd_29[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck19_cmd_30[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck19_cmd_31[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

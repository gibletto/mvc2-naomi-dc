/* Cyclops (character 06): the computer's attack lists.
   What it does once a waiting list's CMD_NEXT has sent it to attack. PLAYER_WORK's cpu_adrs[1]
   points here.

   atck06_tbl     one record (CPU_DATA): the offsets of the three tables.
   atck06_rate    the chance, out of 32, that a list's CMD_NEXT2 starts another attack list, by
                  the level of play.
   atck06_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   atck06_cmd     the lists: a pick is a number in this table.
   atck06_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                  list of lib06.c. */
#include "player.h"
#include "pl06_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const atck06_sel[], *const atck06_cmd[];
extern const Uint8 atck06_rate[], atck06_sel_0[], atck06_sel_1[], atck06_sel_2[], atck06_sel_3[],
	atck06_sel_4[], atck06_sel_5[], atck06_sel_6[], atck06_sel_7[], atck06_cmd_00[], atck06_cmd_01[],
	atck06_cmd_02[], atck06_cmd_03[], atck06_cmd_04[], atck06_cmd_05[], atck06_cmd_06[],
	atck06_cmd_07[], atck06_cmd_08[], atck06_cmd_09[], atck06_cmd_10[], atck06_cmd_11[],
	atck06_cmd_12[], atck06_cmd_13[], atck06_cmd_14[], atck06_cmd_15[], atck06_cmd_16[],
	atck06_cmd_17[], atck06_cmd_18[], atck06_cmd_19[], atck06_cmd_20[], atck06_cmd_21[],
	atck06_cmd_22[], atck06_cmd_23[], atck06_cmd_24[], atck06_cmd_25[], atck06_cmd_26[],
	atck06_cmd_27[], atck06_cmd_28[], atck06_cmd_29[], atck06_cmd_30[], atck06_cmd_31[];

const CPU_DATA atck06_tbl[] = {
	{ ( Sint32 )atck06_rate, ( Sint32 )atck06_sel, ( Sint32 )atck06_cmd },
};

const Uint8 atck06_rate[] = {
	  9,   9,  11,  11,  14,  14,  15,  15,  18,  19,  20,  21,  22,  23,  24,  25,
	 25,  26,  27,  27,  27,  27,  27,  27,  28,  28,  29,  29,  30,  30,  31,  31,
};

const Uint8 *const atck06_sel[] = {
	atck06_sel_0, atck06_sel_1, atck06_sel_2, atck06_sel_3, atck06_sel_4, atck06_sel_5,
	atck06_sel_6, atck06_sel_7,
};

const Uint8 atck06_sel_0[] = {
	  0,   0,   0,   1,   1,   2,   2,  16,   0,   0,   0,   1,   1,   2,   2,  16,
	  0,   0,   1,   1,   1,   2,   2,  14,  16,  16,  16,  16,   7,   7,   7,  14,
};

const Uint8 atck06_sel_1[] = {
	  2,   2,   2,   3,   3,   4,   4,  16,   2,   2,   2,   3,   3,   4,   4,  16,
	  2,   2,   3,   3,   3,   4,   4,  14,  16,  16,  16,  16,   7,   7,   7,  14,
};

const Uint8 atck06_sel_2[] = {
	  4,   4,   4,   5,   5,   6,   6,  16,   4,   4,   4,   5,   5,   6,   6,  16,
	  4,   4,   5,   5,   5,   6,   6,  14,  16,  16,  16,  16,   7,   7,   7,  14,
};

const Uint8 atck06_sel_3[] = {
	  6,   6,   6,   7,   7,   8,   8,   8,   6,   6,   6,   7,   7,   8,   8,   8,
	  6,   6,   7,   7,   7,   8,   8,  14,  16,  16,  16,  16,  16,  16,  16,  16,
};

const Uint8 atck06_sel_4[] = {
	  8,   8,   9,   9,  10,  10,  14,  14,   8,   8,   9,   9,  10,  10,  14,  14,
	  8,   8,   9,   9,  10,  10,  14,  14,  16,  16,  16,  16,   7,   7,  14,  14,
};

const Uint8 atck06_sel_5[] = {
	  7,  10,  10,  11,  11,  12,  12,  14,  10,  10,  10,  11,  11,  12,  12,  14,
	 10,  10,  11,  11,  11,  12,  12,  14,  16,  16,  16,  16,   7,   7,  16,  16,
};

const Uint8 atck06_sel_6[] = {
	  7,   7,   7,   7,   7,   7,  15,  14,  12,  12,  12,  13,  13,  15,  15,  14,
	 12,  12,  13,  13,  13,  15,  14,  14,  16,  16,  16,  16,  16,  16,  14,  14,
};

const Uint8 atck06_sel_7[] = {
	  7,   7,   7,   7,   7,  16,  15,  14,  10,  11,  12,  13,  15,  16,  15,  14,
	 10,  11,  12,  13,  15,  16,  15,  14,  10,  11,  12,  13,  15,  16,  15,  14,
};

const Uint8 *const atck06_cmd[] = {
	atck06_cmd_00, atck06_cmd_01, atck06_cmd_02, atck06_cmd_03, atck06_cmd_04, atck06_cmd_05,
	atck06_cmd_06, atck06_cmd_07, atck06_cmd_08, atck06_cmd_09, atck06_cmd_10, atck06_cmd_11,
	atck06_cmd_12, atck06_cmd_13, atck06_cmd_14, atck06_cmd_15, atck06_cmd_16, atck06_cmd_17,
	atck06_cmd_18, atck06_cmd_19, atck06_cmd_20, atck06_cmd_21, atck06_cmd_22, atck06_cmd_23,
	atck06_cmd_24, atck06_cmd_25, atck06_cmd_26, atck06_cmd_27, atck06_cmd_28, atck06_cmd_29,
	atck06_cmd_30, atck06_cmd_31,
};

const Uint8 atck06_cmd_00[] = {
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

const Uint8 atck06_cmd_01[] = {
	IF_RND( 11 ),
		WAIT( 5 ),
		WAIT2( 6 ),
		CMD_JUMP( 7 ),
	CMD_ELSE,
		IF_RND( 16 ),
			WAIT( 5 ),
			WAIT2( 6 ),
			CANCEL_SET,
				CMD_JUMP( 8 ),
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			WAIT( 5 ),
			WAIT2( 6 ),
			CMD_JUMP( 9 ),
		CMD_ENDIF,
	CMD_ENDIF,
	IF_RND( 16 ),
		CMD_JUMP( 29 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck06_cmd_02[] = {
	WAIT( 5 ),
	WAIT2( 6 ),
	CMD_JUMP( 13 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck06_cmd_03[] = {
	WAIT( 5 ),
	WAIT2( 6 ),
	CMD_JUMP( 21 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck06_cmd_04[] = {
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
	IF_RND( 12 ),
		CMD_JUMP( 0 ),
		CMD_JUMP( 29 ),
	CMD_ENDIF,
	IF_RND( 16 ),
		CMD_JUMP( 29 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck06_cmd_05[] = {
	IF_RND( 11 ),
		WAIT( 4 ),
		WAIT2( 4 ),
		CMD_JUMP( 7 ),
	CMD_ELSE,
		IF_RND( 16 ),
			WAIT( 4 ),
			WAIT2( 4 ),
			CANCEL_SET2,
				CMD_JUMP( 8 ),
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			WAIT( 4 ),
			WAIT2( 4 ),
			CMD_JUMP( 9 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck06_cmd_06[] = {
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
			SP_CMD( pl06_sp_super, 0, PK_PUNCH, LV_LIGHT, 0 ),
			IF_RND( 24 ),
				SP_CMD( pl06_sp_super, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ELSE,
			WAIT( 10 ),
			WAIT2( 6 ),
			SP_CMD( pl06_sp_mega, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_RND( 8 ),
			WAIT( 10 ),
			WAIT2( 8 ),
			IF_RND( 16 ),
				WAIT( 20 ),
				SP_CMD( pl06_sp_blast, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ELSE,
				SP_CMD( pl06_sp_mega, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ELSE,
		CMD_ENDIF,
	CMD_ENDIF,
	IF_RND( 16 ),
		CMD_JUMP( 29 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck06_cmd_07[] = {
	STAND,
	IF_RND( 14 ),
		IF_AREA( 32, 0, 32, 0 ),
			CANCEL_SET2,
				CMD_JUMP( 8 ),
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 21 ),
	CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 21 ),
		IF_RND( 8 ),
			CMD_JUMP( 21 ),
		CMD_ELSE,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck06_cmd_08[] = {
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
				CANCEL_SET,
					CMD_JUMP( 8 ),
					CANCEL_CLR,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck06_cmd_09[] = {
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
			SP_CMD( pl06_sp_mega, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			IF_RND( 16 ),
				WAIT( 10 ),
				WAIT2( 6 ),
				SP_CMD( pl06_sp_super, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				SP_CMD( pl06_sp_mega, 0, PK_PUNCH, LV_LIGHT, 0 ),
				SP_CMD( pl06_sp_mega, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_RND( 8 ),
			WAIT( 10 ),
			WAIT2( 8 ),
			IF_RND( 16 ),
				WAIT2( 8 ),
				SP_CHANGE,
			CMD_ELSE,
				WAIT( 10 ),
				SP_V_COMBI,
			CMD_ENDIF,
		CMD_ELSE,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	IF_RND( 16 ),
		CMD_JUMP( 29 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck06_cmd_10[] = {
	STAND,
	CMD_JUMP( 21 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck06_cmd_11[] = {
	IF_RND( 20 ),
		CMD_JUMP( 17 ),
	CMD_ELSE,
		CMD_JUMP( 25 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck06_cmd_12[] = {
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
				CANCEL_SET2,
					CMD_JUMP( 8 ),
					CANCEL_CLR,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck06_cmd_13[] = {
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
				CMD_JUMP( 29 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_RND( 16 ),
				WAIT( 4 ),
				WAIT2( 4 ),
				CMD_JUMP( 26 ),
				IF_RND( 10 ),
					CMD_JUMP( 0 ),
					CMD_JUMP( 29 ),
				CMD_ENDIF,
			CMD_ELSE,
				WAIT( 4 ),
				WAIT2( 4 ),
				CMD_JUMP( 24 ),
				IF_RND( 10 ),
					CMD_JUMP( 0 ),
					CMD_JUMP( 29 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck06_cmd_14[] = {
	CMD_JUMP( 13 ),
	CMD_JUMP( 0 ),
	CMD_JUMP( 29 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck06_cmd_15[] = {
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
	IF_RND( 16 ),
		CMD_JUMP( 29 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck06_cmd_16[] = {
	IF_RND( 11 ),
		CMD_JUMP( 28 ),
	CMD_ELSE,
		IF_RND( 16 ),
			CMD_JUMP( 26 ),
		CMD_ELSE,
			CMD_JUMP( 25 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck06_cmd_17[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck06_cmd_18[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck06_cmd_19[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck06_cmd_20[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck06_cmd_21[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck06_cmd_22[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck06_cmd_23[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck06_cmd_24[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck06_cmd_25[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck06_cmd_26[] = {
	CMD_JUMP( 30 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck06_cmd_27[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck06_cmd_28[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck06_cmd_29[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck06_cmd_30[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck06_cmd_31[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

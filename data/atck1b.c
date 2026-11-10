/* Chun Li (character 1b): the computer's attack lists.
   What it does once a waiting list's CMD_NEXT has sent it to attack. PLAYER_WORK's cpu_adrs[1]
   points here.

   atck1b_tbl     one record (CPU_DATA): the offsets of the three tables.
   atck1b_rate    the chance, out of 32, that a list's CMD_NEXT2 starts another attack list, by
                  the level of play.
   atck1b_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   atck1b_cmd     the lists: a pick is a number in this table.
   atck1b_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                  list of lib1b.c.
   atck1b_unused_N  bytes that nothing in the file reaches. */
#include "player.h"
#include "pl1b_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const atck1b_sel[], *const atck1b_cmd[];
extern const Uint8 atck1b_rate[], atck1b_unused_0[], atck1b_sel_0[], atck1b_sel_1[], atck1b_sel_2[],
	atck1b_sel_3[], atck1b_sel_4[], atck1b_sel_5[], atck1b_sel_6[], atck1b_sel_7[], atck1b_cmd_00[],
	atck1b_cmd_01[], atck1b_cmd_02[], atck1b_cmd_03[], atck1b_cmd_04[], atck1b_cmd_05[],
	atck1b_cmd_06[], atck1b_cmd_07[], atck1b_cmd_08[], atck1b_cmd_09[], atck1b_cmd_10[],
	atck1b_cmd_11[], atck1b_cmd_12[], atck1b_cmd_13[], atck1b_cmd_14[], atck1b_cmd_15[],
	atck1b_cmd_16[], atck1b_cmd_17[], atck1b_cmd_18[], atck1b_cmd_19[], atck1b_cmd_20[],
	atck1b_cmd_21[], atck1b_cmd_22[], atck1b_cmd_23[], atck1b_cmd_24[], atck1b_cmd_25[],
	atck1b_cmd_26[], atck1b_cmd_27[], atck1b_cmd_28[], atck1b_cmd_29[], atck1b_cmd_30[],
	atck1b_cmd_31[];

const CPU_DATA atck1b_tbl[] = {
	{ ( Sint32 )atck1b_rate, ( Sint32 )atck1b_sel, ( Sint32 )atck1b_cmd },
};

const Uint8 atck1b_rate[] = {
	 10,  10,  13,  13,  15,  15,  17,  17,  18,  19,  20,  21,  22,  23,  24,  25,
	 25,  26,  27,  27,  27,  27,  27,  27,  28,  28,  29,  29,  30,  30,  31,  31,
};

const Uint8 *const atck1b_sel[] = {
	atck1b_sel_0, atck1b_sel_1, atck1b_sel_2, atck1b_sel_3, atck1b_sel_4, atck1b_sel_5,
	atck1b_sel_6, atck1b_sel_7,
};

const Uint8 atck1b_unused_0[] = {
	  1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
	  1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
};

const Uint8 atck1b_sel_0[] = {
	  0,   0,   0,   1,   1,   2,   2,  16,   0,   0,   0,   1,   1,   2,   2,  16,
	  0,   0,   1,   1,   1,   2,   2,  14,  18,  18,  17,  17,   1,  17,  17,  14,
};

const Uint8 atck1b_sel_1[] = {
	  2,   2,   2,   3,   3,   4,   4,  16,   2,   2,   2,   3,   3,   4,   4,  16,
	  2,   2,   3,   3,   3,   4,   4,  14,   2,  18,   3,  17,  17,  18,  18,  14,
};

const Uint8 atck1b_sel_2[] = {
	  4,   4,   4,   5,   5,   6,   6,  16,   4,   4,   4,   5,   5,   6,   6,  16,
	  4,   4,   5,   5,   5,   6,   6,  14,  18,  18,  18,  17,  17,  17,  17,  14,
};

const Uint8 atck1b_sel_3[] = {
	  6,   6,   6,   7,   7,   8,   8,   8,   6,   6,   6,   7,   7,   8,   8,   8,
	 18,  18,   7,  17,  17,  17,  17,  14,  18,  18,  18,  16,  16,  16,  16,  16,
};

const Uint8 atck1b_sel_4[] = {
	  8,   8,   9,   9,  10,  10,  14,  14,   8,   8,   9,   9,  10,  10,  14,  14,
	  8,  18,  18,   9,  18,  10,  18,  14,  17,  17,  17,  18,  18,  18,  18,  14,
};

const Uint8 atck1b_sel_5[] = {
	 10,  10,  10,  11,  11,  12,  12,  14,  10,  10,  10,  11,  11,  12,  12,  14,
	 17,  17,  17,  18,  18,  18,  18,  14,  16,  16,  16,  16,  16,  16,  18,  18,
};

const Uint8 atck1b_sel_6[] = {
	 12,  12,  12,  13,  13,  15,  15,  14,  12,  12,  12,  13,  13,  15,  15,  14,
	 12,  12,  13,  18,  13,  15,  14,  18,  17,  17,  17,  17,  18,  18,  18,  18,
};

const Uint8 atck1b_sel_7[] = {
	 10,  11,  12,  13,  15,  16,  15,  14,  10,  11,  12,  13,  15,  16,  15,  18,
	 18,  18,  18,  18,  15,  16,  15,  14,  18,  17,  17,  17,  18,  18,  18,  18,
};

const Uint8 *const atck1b_cmd[] = {
	atck1b_cmd_00, atck1b_cmd_01, atck1b_cmd_02, atck1b_cmd_03, atck1b_cmd_04, atck1b_cmd_05,
	atck1b_cmd_06, atck1b_cmd_07, atck1b_cmd_08, atck1b_cmd_09, atck1b_cmd_10, atck1b_cmd_11,
	atck1b_cmd_12, atck1b_cmd_13, atck1b_cmd_14, atck1b_cmd_15, atck1b_cmd_16, atck1b_cmd_17,
	atck1b_cmd_18, atck1b_cmd_19, atck1b_cmd_20, atck1b_cmd_21, atck1b_cmd_22, atck1b_cmd_23,
	atck1b_cmd_24, atck1b_cmd_25, atck1b_cmd_26, atck1b_cmd_27, atck1b_cmd_28, atck1b_cmd_29,
	atck1b_cmd_30, atck1b_cmd_31,
};

const Uint8 atck1b_cmd_00[] = {
	CMD_JUMP( 55 ),
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

const Uint8 atck1b_cmd_01[] = {
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

const Uint8 atck1b_cmd_02[] = {
	WAIT( 5 ),
	CMD_JUMP( 55 ),
	WAIT2( 6 ),
	CMD_JUMP( 13 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1b_cmd_03[] = {
	WAIT( 5 ),
	CMD_JUMP( 55 ),
	WAIT2( 6 ),
	CMD_JUMP( 21 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1b_cmd_04[] = {
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

const Uint8 atck1b_cmd_05[] = {
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
			CMD_JUMP( 55 ),
			CMD_JUMP( 24 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1b_cmd_06[] = {
	CMD_JUMP( 55 ),
	IF_RND( 11 ),
		WAIT( 4 ),
		WAIT2( 4 ),
		CMD_JUMP( 13 ),
	CMD_ELSE,
		IF_RND( 16 ),
			WAIT( 4 ),
			WAIT2( 4 ),
			CMD_JUMP( 55 ),
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
			SP_CMD( pl1b_sp_kikoushou, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			IF_RND( 16 ),
				WAIT2( 6 ),
				IF_EM_STANCE( STANCE_AIR ),
					TIMER_SET( 30 ),
					IF_AREA( 128, 0, 0, 0 ),
						SP_CMD( pl1b_sp_hazan, 0, PK_PUNCH, LV_LIGHT, 0 ),
					CMD_ELSE,
					CMD_ENDIF,
				CMD_ELSE,
					SP_CMD( pl1b_sp_senretsu, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ENDIF,
			CMD_ELSE,
				SP_CMD( pl1b_sp_kikoushou, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_RND( 8 ),
			WAIT( 10 ),
			WAIT2( 8 ),
			IF_RND( 16 ),
				SP_CMD( pl1b_sp_kikoushou, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				SP_CMD( pl1b_sp_senretsu, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ELSE,
			WALK_F( 64 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1b_cmd_07[] = {
	STAND,
	CMD_JUMP( 55 ),
	IF_RND( 14 ),
		IF_AREA( 32, 0, 32, 0 ),
			CANCEL_SET,
				KICK( LV_LIGHT ),
				KICK( LV_LIGHT ),
				SP_CMD( pl1b_sp_hyakuretsu, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CANCEL_CLR,
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

const Uint8 atck1b_cmd_08[] = {
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

const Uint8 atck1b_cmd_09[] = {
	IF_RND( 11 ),
		WAIT( 4 ),
		WAIT2( 4 ),
		CMD_JUMP( 13 ),
	CMD_ELSE,
		IF_RND( 16 ),
			WAIT( 4 ),
			WAIT2( 4 ),
			CMD_JUMP( 55 ),
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
			TIMER_SET( 10 ),
			IF_AREA( 128, 0, 0, 0 ),
				IF_EM_STANCE( STANCE_AIR ),
					SP_CMD( pl1b_sp_hazan, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ELSE,
					SP_CMD( pl1b_sp_kikoushou, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ENDIF,
			CMD_ELSE,
			CMD_ENDIF,
		CMD_ELSE,
			TIMER_SET( 10 ),
			IF_AREA( 128, 0, 0, 0 ),
				IF_EM_STANCE( STANCE_AIR ),
					SP_CMD( pl1b_sp_hazan, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ELSE,
					SP_CMD( pl1b_sp_senretsu, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ENDIF,
			CMD_ELSE,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1b_cmd_10[] = {
	STAND,
	IF_RND( 14 ),
		IF_AREA( 32, 0, 32, 0 ),
			CANCEL_SET,
				KICK( LV_LIGHT ),
				KICK( LV_LIGHT ),
				SP_CMD( pl1b_sp_hyakuretsu, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CANCEL_CLR,
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

const Uint8 atck1b_cmd_11[] = {
	STAND,
	IF_RND( 10 ),
		CMD_JUMP( 12 ),
	CMD_ELSE,
		IF_RND( 11 ),
			WAIT( 4 ),
			WAIT2( 4 ),
			CMD_JUMP( 26 ),
		CMD_ELSE,
			IF_RND( 16 ),
				WAIT( 4 ),
				WAIT2( 4 ),
				CMD_JUMP( 27 ),
			CMD_ELSE,
				WAIT( 4 ),
				WAIT2( 4 ),
				CMD_JUMP( 28 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
};

const Uint8 atck1b_cmd_12[] = {
	CMD_JUMP( 55 ),
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
				CMD_JUMP( 28 ),
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

const Uint8 atck1b_cmd_13[] = {
	CMD_JUMP( 55 ),
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
				CMD_JUMP( 55 ),
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

const Uint8 atck1b_cmd_14[] = {
	CMD_JUMP( 13 ),
	CMD_JUMP( 0 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1b_cmd_15[] = {
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

const Uint8 atck1b_cmd_16[] = {
	CMD_JUMP( 55 ),
	STAND,
	IF_EM_STANCE( STANCE_AIR ),
		CMD_JUMP( 21 ),
	CMD_ELSE,
		IF_AREA( 48, 0, 48, 0 ),
			IF_RND( 16 ),
				CROUCH,
				KICK( LV_LIGHT ),
				STAND,
			CMD_ELSE,
				CANCEL_SET,
					CROUCH,
					KICK( LV_LIGHT ),
					STAND,
					IF_RND( 16 ),
						SP_CMD( pl1b_sp_tenshou, 0, PK_PUNCH, LV_HEAVY, 0 ),
					CMD_ELSE,
						SP_CMD( pl1b_sp_senen, 0, PK_PUNCH, LV_HEAVY, 0 ),
					CMD_ENDIF,
					CANCEL_CLR,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			TIMER_SET( 30 ),
			WALK_ATCK( ATCK_STAND_MK ),
				KICK( LV_LIGHT ),
				WAIT( 10 ),
				SP_CMD( pl1b_sp_senen, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ELSE,
				JUMP( DIR_BACK ),
				JUMP_FLAG_SET,
					WAIT( 40 ),
					AIR_JUMP( DIR_UP ),
					WAIT( 40 ),
					TIMER_SET( 0xb4 ),
					IF_EM_ATCK( ATCK_JUMP_LK ),
						CANCEL_SET2,
							KICK( LV_LIGHT ),
							SP_CMD( pl1b_sp_hyakuretsu, 0, PK_PUNCH, LV_HEAVY, 0 ),
							CANCEL_CLR,
						CMD_ENDIF,
					CMD_ELSE,
					CMD_ENDIF,
					JUMP_FLAG_CLR,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1b_cmd_17[] = {
	CMD_JUMP( 55 ),
	CMD_JUMP( 28 ),
	IF_RND( 16 ),
		CMD_JUMP( 28 ),
	CMD_ELSE,
		CMD_JUMP( 12 ),
	CMD_ENDIF,
	IF_RND( 4 ),
		WAIT( 10 ),
		SP_V_COMBI,
	CMD_ELSE,
		CMD_JUMP( 0 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1b_cmd_18[] = {
	IF_RND( 24 ),
		CMD_JUMP( 31 ),
	CMD_ELSE,
		IF_RND( 16 ),
			CMD_JUMP( 30 ),
		CMD_ELSE,
			CMD_JUMP( 29 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1b_cmd_19[] = {
	CMD_JUMP( 55 ),
	STAND,
	CMD_JUMP( 20 ),
	JUMP( DIR_FORWARD ),
	TIMER_SET( 0xc8 ),
	IF_DIST( 80, 24, 96, 1 ),
		IF_GAUGE( 2 ),
			SP_CMD( pl1b_sp_shichisei, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1b_cmd_20[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1b_cmd_21[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1b_cmd_22[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1b_cmd_23[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1b_cmd_24[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1b_cmd_25[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1b_cmd_26[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1b_cmd_27[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1b_cmd_28[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1b_cmd_29[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1b_cmd_30[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck1b_cmd_31[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

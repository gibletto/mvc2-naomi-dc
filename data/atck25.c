/* Dhalsim (character 25): the computer's attack lists.
   What it does once a waiting list's CMD_NEXT has sent it to attack. PLAYER_WORK's cpu_adrs[1]
   points here.

   atck25_tbl     one record (CPU_DATA): the offsets of the three tables.
   atck25_rate    the chance, out of 32, that a list's CMD_NEXT2 starts another attack list, by
                  the level of play.
   atck25_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   atck25_cmd     the lists: a pick is a number in this table.
   atck25_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                  list of lib25.c. */
#include "player.h"
#include "pl25_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const atck25_sel[], *const atck25_cmd[];
extern const Uint8 atck25_rate[], atck25_sel_0[], atck25_sel_1[], atck25_sel_2[], atck25_sel_3[],
	atck25_sel_4[], atck25_sel_5[], atck25_sel_6[], atck25_sel_7[], atck25_cmd_00[], atck25_cmd_01[],
	atck25_cmd_02[], atck25_cmd_03[], atck25_cmd_04[], atck25_cmd_05[], atck25_cmd_06[],
	atck25_cmd_07[], atck25_cmd_08[], atck25_cmd_09[], atck25_cmd_10[], atck25_cmd_11[],
	atck25_cmd_12[], atck25_cmd_13[], atck25_cmd_14[], atck25_cmd_15[], atck25_cmd_16[],
	atck25_cmd_17[], atck25_cmd_18[], atck25_cmd_19[], atck25_cmd_20[], atck25_cmd_21[],
	atck25_cmd_22[], atck25_cmd_23[], atck25_cmd_24[], atck25_cmd_25[], atck25_cmd_26[],
	atck25_cmd_27[], atck25_cmd_28[], atck25_cmd_29[], atck25_cmd_30[], atck25_cmd_31[];

const CPU_DATA atck25_tbl[] = {
	{ ( Sint32 )atck25_rate, ( Sint32 )atck25_sel, ( Sint32 )atck25_cmd },
};

const Uint8 atck25_rate[] = {
	 10,  10,  13,  13,  15,  15,  17,  17,  18,  19,  20,  21,  22,  23,  24,  25,
	 25,  26,  27,  27,  27,  27,  27,  27,  28,  28,  29,  29,  30,  30,  31,  31,
};

const Uint8 *const atck25_sel[] = {
	atck25_sel_0, atck25_sel_1, atck25_sel_2, atck25_sel_3, atck25_sel_4, atck25_sel_5,
	atck25_sel_6, atck25_sel_7,
};

const Uint8 atck25_sel_0[] = {
	  0,   0,   1,   2,   3,   4,   4,   5,   0,   0,   1,   2,   3,   4,   4,   5,
	  8,   8,   8,   8,   8,   8,   8,   5,   0,   1,   1,   2,   3,   4,   4,   5,
};

const Uint8 atck25_sel_1[] = {
	  0,   0,   1,   2,   3,   4,   4,   5,   0,   0,   1,   2,   3,   4,   4,   5,
	  0,   8,   8,   8,   8,   8,   8,   7,   0,   1,   1,   2,   3,   7,   7,   7,
};

const Uint8 atck25_sel_2[] = {
	  0,   1,   1,   2,   3,   4,   5,   6,   0,   1,   1,   2,   3,   4,   5,   6,
	  0,   1,   1,   7,   7,   7,   7,   6,   8,   8,   8,   8,   8,   9,   9,   9,
};

const Uint8 atck25_sel_3[] = {
	  0,   1,   2,   2,   3,   3,   7,   6,   0,   1,   2,   2,   3,   3,   4,   6,
	  0,   7,   2,   7,   3,   7,   5,   9,   8,   8,   8,   8,   8,   9,   9,   9,
};

const Uint8 atck25_sel_4[] = {
	  0,   1,   2,   2,   3,   3,   4,   6,   0,   1,   2,   2,   3,   3,   4,   6,
	  8,   8,   8,   8,   8,   9,   8,   8,   7,   7,   7,   7,   7,   9,   9,   9,
};

const Uint8 atck25_sel_5[] = {
	  0,   1,   2,   2,   3,   3,   7,   6,   0,   1,   2,   2,   3,   3,   4,   6,
	  8,   8,   8,   8,   8,   9,   8,   6,   7,   7,   7,   7,   7,   9,   9,   9,
};

const Uint8 atck25_sel_6[] = {
	  0,   1,   2,   2,   3,   3,   7,   6,   0,   1,   2,   2,   3,   3,   9,   6,
	  0,   1,   2,   2,   3,   3,   9,   6,   8,   8,   8,   8,   8,   9,   9,   8,
};

const Uint8 atck25_sel_7[] = {
	  0,   0,   1,   2,   2,   6,   6,   7,   0,   0,   1,   2,   2,   6,   6,   7,
	  0,   0,   1,   2,   2,   9,   9,   7,   8,   8,   8,   8,   8,   9,   9,   8,
};

const Uint8 *const atck25_cmd[] = {
	atck25_cmd_00, atck25_cmd_01, atck25_cmd_02, atck25_cmd_03, atck25_cmd_04, atck25_cmd_05,
	atck25_cmd_06, atck25_cmd_07, atck25_cmd_08, atck25_cmd_09, atck25_cmd_10, atck25_cmd_11,
	atck25_cmd_12, atck25_cmd_13, atck25_cmd_14, atck25_cmd_15, atck25_cmd_16, atck25_cmd_17,
	atck25_cmd_18, atck25_cmd_19, atck25_cmd_20, atck25_cmd_21, atck25_cmd_22, atck25_cmd_23,
	atck25_cmd_24, atck25_cmd_25, atck25_cmd_26, atck25_cmd_27, atck25_cmd_28, atck25_cmd_29,
	atck25_cmd_30, atck25_cmd_31,
};

const Uint8 atck25_cmd_00[] = {
	JUMP_RND( 4, 7, 8, 11 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck25_cmd_01[] = {
	JUMP_RND( 9, 21, 27, 31 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck25_cmd_02[] = {
	IF_GAUGE( 3 ),
		CMD_JUMP( 21 ),
	CMD_ELSE,
		IF_RND( 10 ),
			CMD_JUMP( 10 ),
		CMD_ELSE,
			IF_RND( 16 ),
				CMD_JUMP( 9 ),
			CMD_ELSE,
				SP_CHANGE,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck25_cmd_03[] = {
	IF_AREA( 32, 0, 0, 0 ),
		PUNCH( LV_LIGHT ),
		CMD_JUMP( 5 ),
	CMD_ELSE,
		IF_RND( 16 ),
			SP_CMD( pl25_sp_fire, 0, PK_PUNCH, LV_HEAVY, 0 ),
			SP_CMD( pl25_sp_fire, 0, PK_PUNCH, LV_LIGHT, 0 ),
			SP_CMD( pl25_sp_fire, 0, PK_PUNCH, LV_LIGHT, 0 ),
			IF_RND( 20 ),
				SP_CMD( pl25_sp_fire, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				SP_CMD( pl25_sp_frei, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ENDIF,
		CMD_ELSE,
			DASH_B( 512 ),
			SP_CMD( pl25_sp_fire, 0, PK_PUNCH, LV_LIGHT, 0 ),
			SP_CMD( pl25_sp_fire, 0, PK_PUNCH, LV_LIGHT, 0 ),
			IF_RND( 20 ),
				SP_CMD( pl25_sp_fire, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				SP_CMD( pl25_sp_frei, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck25_cmd_04[] = {
	TIMER_SET( 120 ),
	IF_AREA2( 0, 0, 0, 0 ),
		TIMER2_SET( 80 ),
	CMD_ELSE,
		IF_RND( 8 ),
			KICK( LV_LIGHT ),
		CMD_ELSE,
			IF_RND( 11 ),
				PUNCH( LV_LIGHT ),
			CMD_ELSE,
				IF_RND( 16 ),
					PUNCH( LV_HEAVY ),
				CMD_ELSE,
					KICK( LV_HEAVY ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck25_cmd_05[] = {
	JUMP( DIR_FORWARD ),
	JUMP_FLAG_SET,
		TIMER_SET( 40 ),
		IF_AREA2( 0, 0, 0, 0 ),
			TIMER2_SET( 80 ),
		CMD_ELSE,
			TIMER_SET( 120 ),
			IF_HEIGHT( 1, 80 ),
				IF_RND( 8 ),
					KICK( LV_LIGHT ),
				CMD_ELSE,
					IF_RND( 11 ),
						PUNCH( LV_LIGHT ),
					CMD_ELSE,
						IF_RND( 16 ),
							PUNCH( LV_HEAVY ),
						CMD_ELSE,
							KICK( LV_HEAVY ),
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	IF_RND( 10 ),
		CMD_JUMP( 21 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck25_cmd_06[] = {
	IF_RND( 16 ),
		CMD_JUMP( 9 ),
	CMD_ELSE,
		CMD_JUMP( 31 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck25_cmd_07[] = {
	CMD_JUMP( 0 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck25_cmd_08[] = {
	LOCK_SET,
	CMD_JUMP( 12 ),
	LOCK_CLR,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck25_cmd_09[] = {
	IF_RND( 8 ),
		JUMP( DIR_BACK ),
		STAND,
	CMD_ENDIF,
	JUMP( DIR_UP ),
	JUMP_FLAG_SET,
		WAIT( 40 ),
		WAIT2( 14 ),
		WAIT2( 6 ),
		IF_AREA( 24, 0, 24, 0 ),
			NAG_VFLIP_ADJ4( LV_HEAVY, LEVER_DOWN | LEVER_FORWARD ),
		CMD_ELSE,
			IF_AREA( 48, 0, 48, 0 ),
				NAG_VFLIP_ADJ4( LV_LIGHT, LEVER_DOWN | LEVER_FORWARD ),
			CMD_ELSE,
				IF_RND( 16 ),
					NAG_VFLIP_ADJ4( LV_LIGHT, LEVER_DOWN | LEVER_FORWARD ),
				CMD_ELSE,
					NAG_VFLIP_ADJ3( LV_HEAVY, LEVER_DOWN | LEVER_FORWARD ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck25_cmd_10[] = {
	CMD_EXIT,
};

const Uint8 atck25_cmd_11[] = {
	CMD_EXIT,
};

const Uint8 atck25_cmd_12[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck25_cmd_13[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck25_cmd_14[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck25_cmd_15[] = {
	CMD_EXIT,
};

const Uint8 atck25_cmd_16[] = {
	CMD_EXIT,
};

const Uint8 atck25_cmd_17[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck25_cmd_18[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck25_cmd_19[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck25_cmd_20[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck25_cmd_21[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck25_cmd_22[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck25_cmd_23[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck25_cmd_24[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck25_cmd_25[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck25_cmd_26[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck25_cmd_27[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck25_cmd_28[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck25_cmd_29[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck25_cmd_30[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck25_cmd_31[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

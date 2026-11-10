/* Guile (character 02): the computer's attack lists.
   What it does once a waiting list's CMD_NEXT has sent it to attack. PLAYER_WORK's cpu_adrs[1]
   points here.

   atck02_tbl     one record (CPU_DATA): the offsets of the three tables.
   atck02_rate    the chance, out of 32, that a list's CMD_NEXT2 starts another attack list, by
                  the level of play.
   atck02_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   atck02_cmd     the lists: a pick is a number in this table.
   atck02_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                  list of lib02.c. */
#include "player.h"
#include "pl02_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const atck02_sel[], *const atck02_cmd[];
extern const Uint8 atck02_rate[], atck02_sel_0[], atck02_sel_1[], atck02_sel_2[], atck02_sel_3[],
	atck02_sel_4[], atck02_sel_5[], atck02_sel_6[], atck02_sel_7[], atck02_cmd_00[], atck02_cmd_01[],
	atck02_cmd_02[], atck02_cmd_03[], atck02_cmd_04[], atck02_cmd_05[], atck02_cmd_06[],
	atck02_cmd_07[], atck02_cmd_08[], atck02_cmd_09[], atck02_cmd_10[], atck02_cmd_11[],
	atck02_cmd_12[], atck02_cmd_13[], atck02_cmd_14[], atck02_cmd_15[], atck02_cmd_16[],
	atck02_cmd_17[], atck02_cmd_18[], atck02_cmd_19[], atck02_cmd_20[], atck02_cmd_21[],
	atck02_cmd_22[], atck02_cmd_23[], atck02_cmd_24[], atck02_cmd_25[], atck02_cmd_26[],
	atck02_cmd_27[], atck02_cmd_28[], atck02_cmd_29[], atck02_cmd_30[], atck02_cmd_31[];

const CPU_DATA atck02_tbl[] = {
	{ ( Sint32 )atck02_rate, ( Sint32 )atck02_sel, ( Sint32 )atck02_cmd },
};

const Uint8 atck02_rate[] = {
	 10,  11,  12,  13,  14,  15,  16,  16,  18,  19,  20,  21,  22,  23,  24,  25,
	 25,  26,  27,  27,  27,  27,  27,  27,  28,  28,  29,  29,  30,  30,  31,  31,
};

const Uint8 *const atck02_sel[] = {
	atck02_sel_0, atck02_sel_1, atck02_sel_2, atck02_sel_3, atck02_sel_4, atck02_sel_5,
	atck02_sel_6, atck02_sel_7,
};

const Uint8 atck02_sel_0[] = {
	  0,   0,   0,   2,   3,   3,   4,   4,   0,   0,   0,   2,   3,   3,   4,   4,
	  1,   1,   1,   2,   3,   3,  12,  12,   1,   1,   1,   2,   3,   3,   8,  13,
};

const Uint8 atck02_sel_1[] = {
	  0,   0,   0,   2,   3,   3,  12,   8,   0,   0,   1,   2,   3,   3,  12,  13,
	  0,   1,   1,   5,   5,   4,   4,  10,   1,   1,   1,   6,   6,   4,   4,  10,
};

const Uint8 atck02_sel_2[] = {
	  0,   0,   2,   2,   4,   4,   3,   3,   0,   0,   2,   2,   4,   4,   8,   8,
	  1,   1,   2,   2,   5,   5,  12,  12,   1,   1,   2,   2,   6,   6,  12,  12,
};

const Uint8 atck02_sel_3[] = {
	  0,   0,   4,   2,   6,   6,  10,  10,   0,   0,   4,   2,   6,   6,  12,  12,
	  1,   1,   5,   2,   6,   6,  12,  12,   1,   1,   5,   2,   6,   6,   8,  13,
};

const Uint8 atck02_sel_4[] = {
	  0,   0,   4,   2,   5,   5,  10,  10,   0,   0,   4,   2,   5,   5,   8,   8,
	  1,   1,   6,   2,   5,   5,  12,  14,   1,   1,   6,   2,   5,   5,  12,  14,
};

const Uint8 atck02_sel_5[] = {
	  0,   5,   5,   5,   6,  12,  10,  10,   0,   5,   5,   5,   6,  12,   3,   3,
	  4,   4,   6,   6,   6,  12,   8,   8,   4,   4,   2,   2,   2,   2,  13,  13,
};

const Uint8 atck02_sel_6[] = {
	  0,   0,   5,   5,   5,   5,  12,   8,   4,   4,   5,   5,   5,   6,  12,   8,
	  2,   2,   6,   6,   6,  12,  12,  13,   2,   2,   6,   6,   6,  12,  12,  13,
};

const Uint8 atck02_sel_7[] = {
	  5,   5,   5,   5,   2,  12,  12,  13,   5,   5,   5,   5,   2,  12,  12,  13,
	  6,   6,   6,   6,   6,   0,   8,   8,   6,   6,   6,   6,   6,  10,   4,   4,
};

const Uint8 *const atck02_cmd[] = {
	atck02_cmd_00, atck02_cmd_01, atck02_cmd_02, atck02_cmd_03, atck02_cmd_04, atck02_cmd_05,
	atck02_cmd_06, atck02_cmd_07, atck02_cmd_08, atck02_cmd_09, atck02_cmd_10, atck02_cmd_11,
	atck02_cmd_12, atck02_cmd_13, atck02_cmd_14, atck02_cmd_15, atck02_cmd_16, atck02_cmd_17,
	atck02_cmd_18, atck02_cmd_19, atck02_cmd_20, atck02_cmd_21, atck02_cmd_22, atck02_cmd_23,
	atck02_cmd_24, atck02_cmd_25, atck02_cmd_26, atck02_cmd_27, atck02_cmd_28, atck02_cmd_29,
	atck02_cmd_30, atck02_cmd_31,
};

const Uint8 atck02_cmd_00[] = {
	IF_RND( 16 ),
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

const Uint8 atck02_cmd_01[] = {
	WAIT2( 8 ),
	STAND,
	IF_DIST( 32, 4, 80, 0 ),
		IF_RND( 16 ),
			NAG_VFLIP_ADJ4( LV_HEAVY, LEVER_BACK ),
		CMD_ELSE,
			NAG_VFLIP_ADJ4( LV_LIGHT, LEVER_FORWARD ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_DIST( 96, 4, 128, 0 ),
			CROUCH,
			WAIT( 32 ),
			SP_CMD( pl02_sp_sonic, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT2( 8 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck02_cmd_02[] = {
	CROUCH,
	WAIT( 32 ),
	TIMER_SET( 60 ),
	IF_DIST( 64, 128, 56, 32 ),
		IF_LEVEL( 16 ),
			SP_CMD( pl02_sp_smr, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			PUNCH( LV_HEAVY ),
		CMD_ENDIF,
	CMD_ELSE,
		SP_CMD( pl02_sp_sonic, 0, PK_PUNCH, LV_HEAVY, 0 ),
		NAG_VFLIP_ADJ4( LV_LIGHT, LEVER_BACK ),
		WAIT2( 2 ),
		IF_EM_ATCK( ATCK_STAND_LK ),
			IF_RND( 16 ),
				CANCEL_SET,
					KICK( LV_LIGHT ),
					WAIT2( 8 ),
					CROUCH,
					KICK( LV_LIGHT ),
					WAIT2( 8 ),
					PUNCH( LV_HEAVY ),
					CANCEL_CLR,
				CMD_ENDIF,
			CMD_ELSE,
				CANCEL_SET2,
					KICK( LV_LIGHT ),
					WAIT2( 8 ),
					CROUCH,
					PUNCH( LV_LIGHT ),
					WAIT2( 8 ),
					KICK( LV_HEAVY ),
					CANCEL_CLR,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck02_cmd_03[] = {
	STAND,
	WALK_B( 96 ),
	SP_CMD( pl02_sp_sonic, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CROUCH,
	WAIT( 8 ),
	IF_RND( 16 ),
		TIMER_SET( 20 ),
		IF_DIST( 64, 128, 32, 64 ),
			SP_CMD( pl02_sp_smr, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_JUMP( 23 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck02_cmd_04[] = {
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
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck02_cmd_05[] = {
	STAND,
	WAIT2( 4 ),
	IF_LEVEL( 24 ),
		IF_RND( 16 ),
			PARTNER_CALL,
		CMD_ENDIF,
	CMD_ELSE,
		IF_RND( 10 ),
			PARTNER_CALL,
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT2( 10 ),
	DASH_ATCK( ATCK_CROUCH_LK ),
		CMD_JUMP( 53 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck02_cmd_06[] = {
	STAND,
	IF_LEVEL( 24 ),
		IF_RND( 12 ),
			PARTNER_CALL,
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 16 ),
			IF_RND( 8 ),
				PARTNER_CALL,
			CMD_ENDIF,
		CMD_ELSE,
			IF_RND( 4 ),
				PARTNER_CALL,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_JUMP( 5 ),
	JUMP_FLAG_SET,
		TIMER_SET( 60 ),
		IF_HEIGHT( 0xff, 96 ),
			TIMER_SET( 40 ),
			IF_EM_ATCK( ATCK_JUMP_LK ),
				CANCEL_SET,
					KICK( LV_LIGHT ),
					KICK( LV_LIGHT ),
					CANCEL_CLR,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	CMD_JUMP( 53 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck02_cmd_07[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck02_cmd_08[] = {
	IF_DIST( 64, 8, 32, 0 ),
		IF_RND( 24 ),
			CMD_JUMP( 22 ),
		CMD_ELSE,
			CROUCH,
			KICK( LV_HEAVY ),
		CMD_ENDIF,
	CMD_ELSE,
		DASH_EM( 64 ),
		CMD_JUMP( 22 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck02_cmd_09[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck02_cmd_10[] = {
	STAND,
	IF_RND( 8 ),
		IF_AREA( 32, 0, 32, 0 ),
			IF_RND( 10 ),
				NAG_VFLIP_ADJ3( LV_HEAVY, LEVER_FORWARD ),
			CMD_ELSE,
				CANCEL_SET,
					PUNCH( LV_LIGHT ),
					NAG_VFLIP_ADJ3( LV_HEAVY, LEVER_FORWARD ),
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

const Uint8 atck02_cmd_11[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck02_cmd_12[] = {
	STAND,
	IF_AREA( 64, 1, 64, 0 ),
		WALK_B( 16 ),
		NAG_VFLIP_ADJ4( LV_HEAVY, LEVER_BACK ),
		CROUCH,
		IF_LEVEL( 16 ),
			IF_RND( 16 ),
				PARTNER_CALL,
			CMD_ENDIF,
		CMD_ELSE,
			IF_RND( 8 ),
				PARTNER_CALL,
			CMD_ENDIF,
		CMD_ENDIF,
		WAIT2( 4 ),
		IF_LEVEL( 24 ),
			IF_AREA( 32, 1, 32, 0 ),
				IF_RND( 24 ),
					CMD_JUMP( 27 ),
				CMD_ELSE,
					CMD_JUMP( 28 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_RND( 24 ),
					DASH_EM( 8 ),
					IF_EM_ATCK( ATCK_CROUCH_LP ),
						CROUCH,
						IF_RND( 16 ),
							CANCEL_SET2,
								PUNCH( LV_LIGHT ),
								PUNCH( LV_LIGHT ),
								PUNCH( LV_HEAVY ),
								SJUMP3( DIR_UP ),
								WAIT( 1 ),
								CANCEL_CLR,
							CMD_ENDIF,
							SJUMP_FLAG_SET,
								CMD_JUMP( 4 ),
								JUMP_FLAG_CLR,
							CMD_ENDIF,
						CMD_ELSE,
							CANCEL_SET2,
								PUNCH( LV_LIGHT ),
								KICK( LV_LIGHT ),
								KICK( LV_HEAVY ),
								CANCEL_CLR,
							CMD_ENDIF,
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 16 ),
				IF_RND( 12 ),
					CMD_JUMP( 27 ),
				CMD_ELSE,
					CMD_JUMP( 28 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_RND( 18 ),
					CMD_JUMP( 28 ),
				CMD_ELSE,
					CMD_JUMP( 29 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck02_cmd_13[] = {
	CROUCH,
	WAIT2( 4 ),
	IF_RND( 16 ),
		TIMER_SET( 20 ),
		IF_AREA( 64, 96, 32, 32 ),
			IF_GAUGE( 3 ),
				STAND,
				SP_V_COMBI,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		WAIT2( 2 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck02_cmd_14[] = {
	CMD_JUMP( 13 ),
	CMD_JUMP( 0 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck02_cmd_15[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck02_cmd_16[] = {
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
					STAND,
					WALK_B( 64 ),
					WAIT( 20 ),
					IF_EM_STANCE( STANCE_STAND ),
						SP_CMD( pl02_sp_sonic, 0, PK_PUNCH, LV_HEAVY, 0 ),
					CMD_ELSE,
					CMD_ENDIF,
					CANCEL_CLR,
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
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	IF_RND( 10 ),
		CMD_JUMP( 0 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck02_cmd_17[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck02_cmd_18[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck02_cmd_19[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck02_cmd_20[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck02_cmd_21[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck02_cmd_22[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck02_cmd_23[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck02_cmd_24[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck02_cmd_25[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck02_cmd_26[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck02_cmd_27[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck02_cmd_28[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck02_cmd_29[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck02_cmd_30[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck02_cmd_31[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

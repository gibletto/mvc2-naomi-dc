/* Jill (character 11): the computer's attack lists.
   What it does once a waiting list's CMD_NEXT has sent it to attack. PLAYER_WORK's cpu_adrs[1]
   points here.

   atck11_tbl     one record (CPU_DATA): the offsets of the three tables.
   atck11_rate    the chance, out of 32, that a list's CMD_NEXT2 starts another attack list, by
                  the level of play.
   atck11_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   atck11_cmd     the lists: a pick is a number in this table.
   atck11_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                  list of lib11.c. */
#include "player.h"
#include "pl11_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const atck11_sel[], *const atck11_cmd[];
extern const Uint8 atck11_rate[], atck11_sel_0[], atck11_sel_1[], atck11_sel_2[], atck11_sel_3[],
	atck11_sel_4[], atck11_sel_5[], atck11_sel_6[], atck11_sel_7[], atck11_cmd_00[], atck11_cmd_01[],
	atck11_cmd_02[], atck11_cmd_03[], atck11_cmd_04[], atck11_cmd_05[], atck11_cmd_06[],
	atck11_cmd_07[], atck11_cmd_08[], atck11_cmd_09[], atck11_cmd_10[], atck11_cmd_11[],
	atck11_cmd_12[], atck11_cmd_13[], atck11_cmd_14[], atck11_cmd_15[], atck11_cmd_16[],
	atck11_cmd_17[], atck11_cmd_18[], atck11_cmd_19[], atck11_cmd_20[], atck11_cmd_21[],
	atck11_cmd_22[], atck11_cmd_23[], atck11_cmd_24[], atck11_cmd_25[], atck11_cmd_26[],
	atck11_cmd_27[], atck11_cmd_28[], atck11_cmd_29[], atck11_cmd_30[], atck11_cmd_31[];

const CPU_DATA atck11_tbl[] = {
	{ ( Sint32 )atck11_rate, ( Sint32 )atck11_sel, ( Sint32 )atck11_cmd },
};

const Uint8 atck11_rate[] = {
	 11,  11,  11,  11,  12,  12,  13,  13,  14,  14,  15,  15,  16,  16,  17,  17,
	 18,  18,  19,  19,  20,  20,  21,  21,  22,  23,  24,  25,  27,  29,  30,  31,
};

const Uint8 *const atck11_sel[] = {
	atck11_sel_0, atck11_sel_1, atck11_sel_2, atck11_sel_3, atck11_sel_4, atck11_sel_5,
	atck11_sel_6, atck11_sel_7,
};

const Uint8 atck11_sel_0[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   3,   3,   3,   5,   7,   1,   1,   6,
	  3,   3,   9,   5,   7,   1,   1,   6,   3,   9,   9,   5,   7,   2,   2,  10,
};

const Uint8 atck11_sel_1[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   7,   7,   7,   3,   1,   2,   4,   9,
	  7,   7,   7,   3,   1,   2,   4,   9,   7,   7,   7,   3,   1,   2,   6,  10,
};

const Uint8 atck11_sel_2[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   8,   8,   8,   8,   8,   8,   8,   8,
	  9,   9,   7,   7,   1,   2,   4,   6,   9,   9,   7,   7,   1,   2,   4,  10,
};

const Uint8 atck11_sel_3[] = {
	  0,   0,   0,   0,   0,   0,   8,   8,   1,   1,   7,   7,   2,   4,   6,   6,
	  1,   1,   7,   7,   3,   3,   5,   5,   1,   9,   9,   7,   3,   3,   5,  10,
};

const Uint8 atck11_sel_4[] = {
	  0,   0,   0,   0,   0,   0,   9,   9,   2,   2,   4,   4,   3,   3,   7,   7,
	  2,   2,   4,   4,   3,   3,   5,   5,   2,   9,   9,   4,   1,   1,   5,  10,
};

const Uint8 atck11_sel_5[] = {
	  0,   1,   1,   1,   1,   1,   5,   5,   0,   2,   2,   2,   4,   4,   3,   3,
	  0,   2,   2,   2,   4,   4,   8,   8,   0,   7,   7,   7,   7,   7,  10,  10,
};

const Uint8 atck11_sel_6[] = {
	  1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   3,   3,   4,   4,   4,
	  7,   7,   7,   7,   3,   3,   6,   6,   0,   0,   8,   8,   5,   9,  10,  10,
};

const Uint8 atck11_sel_7[] = {
	  1,   1,   2,   2,   4,   7,   7,   7,   1,   1,   2,   2,   4,   5,   8,   8,
	  1,   1,   2,   2,   4,   0,  10,  10,   3,   3,   3,   3,   4,   0,   6,   6,
};

const Uint8 *const atck11_cmd[] = {
	atck11_cmd_00, atck11_cmd_01, atck11_cmd_02, atck11_cmd_03, atck11_cmd_04, atck11_cmd_05,
	atck11_cmd_06, atck11_cmd_07, atck11_cmd_08, atck11_cmd_09, atck11_cmd_10, atck11_cmd_11,
	atck11_cmd_12, atck11_cmd_13, atck11_cmd_14, atck11_cmd_15, atck11_cmd_16, atck11_cmd_17,
	atck11_cmd_18, atck11_cmd_19, atck11_cmd_20, atck11_cmd_21, atck11_cmd_22, atck11_cmd_23,
	atck11_cmd_24, atck11_cmd_25, atck11_cmd_26, atck11_cmd_27, atck11_cmd_28, atck11_cmd_29,
	atck11_cmd_30, atck11_cmd_31,
};

const Uint8 atck11_cmd_00[] = {
	STAND,
	WAIT2( 8 ),
	IF_LEVEL( 12 ),
		DASH_B( 192 ),
		WAIT( 20 ),
	CMD_ENDIF,
	SP_CMD( pl11_sp_cerberus, 0, PK_PUNCH, LV_LIGHT, 0 ),
	WAIT( 8 ),
	IF_LEVEL( 20 ),
		SP_CMD( pl11_sp_crow, 0, PK_PUNCH, LV_LIGHT, 0 ),
		WAIT2( 8 ),
	CMD_ELSE,
		JUMP( DIR_FORWARD ),
		STAND,
		WAIT( 10 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck11_cmd_01[] = {
	STAND,
	CMD_JUMP( 5 ),
	CMD_JUMP( 14 ),
	JUMP_FLAG_SET,
		CMD_JUMP( 20 ),
		TIMER_SET( 60 ),
		IF_HEIGHT( 0xff, 152 ),
			TIMER_SET( 40 ),
			IF_EM_ATCK( ATCK_JUMP_LP ),
				CANCEL_SET,
					PUNCH( LV_LIGHT ),
					PUNCH( LV_LIGHT ),
					CANCEL_CLR,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	CMD_JUMP( 10 ),
	WAIT2( 12 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck11_cmd_02[] = {
	STAND,
	IF_DIST( 64, 8, 128, 1 ),
		CMD_JUMP( 8 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 8 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck11_cmd_03[] = {
	STAND,
	CMD_JUMP( 0 ),
	WAIT2( 8 ),
	IF_LEVEL( 24 ),
		WALK_DIST( 128 ),
		CROUCH,
		IF_GAUGE( 1 ),
			CANCEL_SET2,
				KICK( LV_HEAVY ),
				SP_CMD( pl11_sp_hstars, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			KICK( LV_HEAVY ),
		CMD_ENDIF,
	CMD_ELSE,
		WALK_ATCK( ATCK_CROUCH_HK ),
			CROUCH,
			KICK( LV_HEAVY ),
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT2( 8 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck11_cmd_04[] = {
	STAND,
	WAIT2( 8 ),
	IF_DIST( 128, 1, 64, 1 ),
		DASH_EM( 6 ),
		CROUCH,
		CANCEL_SET2,
			PUNCH( LV_LIGHT ),
			PUNCH( LV_HEAVY ),
			WAIT( 1 ),
			SJUMP3( DIR_FORWARD ),
			CANCEL_CLR,
		CMD_ENDIF,
		SJUMP_FLAG_SET,
			CMD_JUMP( 4 ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	WAIT2( 8 ),
	CMD_EXIT,
};

const Uint8 atck11_cmd_05[] = {
	WAIT2( 8 ),
	IF_AREA( 64, 16, 48, 0 ),
		IF_EM_ATCK( ATCK_CROUCH_HK ),
			CROUCH,
			KICK( LV_HEAVY ),
		CMD_ENDIF,
	CMD_ELSE,
		SP_CMD( pl11_sp_cerberus, 0, PK_PUNCH, LV_LIGHT, 0 ),
		WAIT2( 4 ),
		TIMER_SET( 40 ),
		IF_EM_STANCE( STANCE_AIR ),
			IF_RND( 20 ),
				SP_CMD( pl11_sp_grenade, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				SP_CMD( pl11_sp_grenade, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT2( 8 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck11_cmd_06[] = {
	IF_DIST( 64, 8, 48, 0 ),
		IF_RND( 24 ),
			CMD_JUMP( 6 ),
		CMD_ELSE,
			IF_EM_ATCK( ATCK_CROUCH_HK ),
				CROUCH,
				KICK( LV_HEAVY ),
			CMD_ELSE,
				SP_CMD( pl11_sp_cerberus, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT2( 8 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck11_cmd_07[] = {
	STAND,
	CMD_JUMP( 5 ),
	WAIT2( 6 ),
	IF_RND( 16 ),
		SP_CMD( pl11_sp_zombie, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ELSE,
		SP_CMD( pl11_sp_zombie, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ENDIF,
	WAIT2( 8 ),
	CMD_JUMP( 14 ),
	WAIT2( 4 ),
	CMD_JUMP( 10 ),
	WAIT2( 8 ),
	CMD_EXIT,
};

const Uint8 atck11_cmd_08[] = {
	STAND,
	IF_RND( 12 ),
		PARTNER_CALL,
	CMD_ENDIF,
	WAIT2( 8 ),
	IF_LEVEL( 16 ),
		DASH_B( 128 ),
		WAIT( 10 ),
	CMD_ENDIF,
	IF_RND( 16 ),
		SP_CMD( pl11_sp_zombie, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ELSE,
		SP_CMD( pl11_sp_zombie, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ENDIF,
	WAIT2( 8 ),
	SP_CMD( pl11_sp_crow, 0, PK_PUNCH, LV_LIGHT, 0 ),
	WAIT2( 8 ),
	SP_CMD( pl11_sp_cerberus, 0, PK_PUNCH, LV_LIGHT, 0 ),
	IF_GAUGE( 1 ),
		IF_RND( 4 ),
			WALK_EM( 96 ),
			SP_CMD( pl11_sp_tyrant, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT2( 8 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck11_cmd_09[] = {
	STAND,
	WALK_B( 96 ),
	SP_CMD( pl11_sp_zombie, 0, PK_PUNCH, LV_HEAVY, 0 ),
	WAIT2( 12 ),
	SP_CMD( pl11_sp_grenade, 0, PK_PUNCH, LV_LIGHT, 0 ),
	WAIT2( 8 ),
	SP_CMD( pl11_sp_chouhatsu, 0, PK_PUNCH, LV_LIGHT, 0 ),
	WAIT2( 12 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck11_cmd_10[] = {
	IF_RND( 16 ),
		TIMER_SET( 20 ),
		IF_AREA( 96, 96, 64, 32 ),
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

const Uint8 atck11_cmd_11[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck11_cmd_12[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck11_cmd_13[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck11_cmd_14[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck11_cmd_15[] = {
	CMD_EXIT,
};

const Uint8 atck11_cmd_16[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck11_cmd_17[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck11_cmd_18[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck11_cmd_19[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck11_cmd_20[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck11_cmd_21[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck11_cmd_22[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck11_cmd_23[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck11_cmd_24[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck11_cmd_25[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck11_cmd_26[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck11_cmd_27[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck11_cmd_28[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck11_cmd_29[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck11_cmd_30[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck11_cmd_31[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

/* Sonson (character 14): the computer's attack lists.
   What it does once a waiting list's CMD_NEXT has sent it to attack. PLAYER_WORK's cpu_adrs[1]
   points here.

   atck14_tbl     one record (CPU_DATA): the offsets of the three tables.
   atck14_rate    the chance, out of 32, that a list's CMD_NEXT2 starts another attack list, by
                  the level of play.
   atck14_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   atck14_cmd     the lists: a pick is a number in this table.
   atck14_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                  list of lib14.c. */
#include "player.h"
#include "pl14_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const atck14_sel[], *const atck14_cmd[];
extern const Uint8 atck14_rate[], atck14_sel_0[], atck14_sel_1[], atck14_sel_2[], atck14_sel_3[],
	atck14_sel_4[], atck14_sel_5[], atck14_sel_6[], atck14_sel_7[], atck14_cmd_00[], atck14_cmd_01[],
	atck14_cmd_02[], atck14_cmd_03[], atck14_cmd_04[], atck14_cmd_05[], atck14_cmd_06[],
	atck14_cmd_07[], atck14_cmd_08[], atck14_cmd_09[], atck14_cmd_10[], atck14_cmd_11[],
	atck14_cmd_12[], atck14_cmd_13[], atck14_cmd_14[], atck14_cmd_15[], atck14_cmd_16[],
	atck14_cmd_17[], atck14_cmd_18[], atck14_cmd_19[], atck14_cmd_20[], atck14_cmd_21[],
	atck14_cmd_22[], atck14_cmd_23[], atck14_cmd_24[], atck14_cmd_25[], atck14_cmd_26[],
	atck14_cmd_27[], atck14_cmd_28[], atck14_cmd_29[], atck14_cmd_30[], atck14_cmd_31[];

const CPU_DATA atck14_tbl[] = {
	{ ( Sint32 )atck14_rate, ( Sint32 )atck14_sel, ( Sint32 )atck14_cmd },
};

const Uint8 atck14_rate[] = {
	  8,   8,   8,   8,   9,   9,  10,  10,  11,  11,  12,  12,  13,  13,  14,  14,
	 15,  15,  16,  16,  17,  17,  18,  18,  19,  20,  21,  22,  24,  26,  28,  30,
};

const Uint8 *const atck14_sel[] = {
	atck14_sel_0, atck14_sel_1, atck14_sel_2, atck14_sel_3, atck14_sel_4, atck14_sel_5,
	atck14_sel_6, atck14_sel_7,
};

const Uint8 atck14_sel_0[] = {
	 16,   0,   0,   0,   1,   2,   3,   3,   0,   0,   0,   0,   1,   2,   3,   3,
	  0,   0,   0,   0,   1,   2,   3,   3,   6,   6,   7,   7,   1,   2,   3,   3,
};

const Uint8 atck14_sel_1[] = {
	 17,   0,   0,   0,   1,   1,   3,   4,   0,   0,   0,   0,   1,   1,   3,   4,
	  0,   0,   0,   0,   1,   1,   3,   4,   0,   0,   0,   0,   1,   1,   3,   4,
};

const Uint8 atck14_sel_2[] = {
	 18,   7,   1,   1,   2,   6,   3,   3,   7,   7,   1,   1,   2,   6,   3,   3,
	  7,   7,   1,   1,   5,   6,   3,   3,   7,   7,   1,   1,   2,   6,   3,   3,
};

const Uint8 atck14_sel_3[] = {
	 12,  11,   1,   1,   2,   6,   3,   8,  12,  11,   1,   1,   2,   6,   3,   8,
	 12,  11,   1,   1,   2,   6,   3,   9,   5,   5,  13,  13,   2,   6,   3,   9,
};

const Uint8 atck14_sel_4[] = {
	 11,  11,   1,   1,   3,   2,   8,   8,  11,  11,   1,   1,   3,   6,   8,   8,
	 11,  11,  13,  13,   5,  12,   9,  10,  11,  11,  13,  13,   5,  12,   9,  10,
};

const Uint8 atck14_sel_5[] = {
	 16,  11,   1,   1,   3,   2,   8,   8,  17,  11,   1,  13,   3,   6,   8,   8,
	 18,  11,  13,  13,   5,  12,   9,  10,  11,  11,  13,  13,   5,  12,   9,  10,
};

const Uint8 atck14_sel_6[] = {
	 16,  11,   1,  13,   3,   2,   8,   8,  16,  11,  13,   1,   3,   6,   8,   8,
	 17,  11,  13,   1,   5,  12,   9,  10,  18,  11,  13,  13,   5,  12,   9,  10,
};

const Uint8 atck14_sel_7[] = {
	 16,  11,  13,  13,   3,   2,   8,   8,  16,  11,  13,  13,   3,   6,   8,   8,
	 17,  11,  13,   5,   5,  12,   9,  10,  18,  11,  13,   5,   5,  12,   9,  10,
};

const Uint8 *const atck14_cmd[] = {
	atck14_cmd_00, atck14_cmd_01, atck14_cmd_02, atck14_cmd_03, atck14_cmd_04, atck14_cmd_05,
	atck14_cmd_06, atck14_cmd_07, atck14_cmd_08, atck14_cmd_09, atck14_cmd_10, atck14_cmd_11,
	atck14_cmd_12, atck14_cmd_13, atck14_cmd_14, atck14_cmd_15, atck14_cmd_16, atck14_cmd_17,
	atck14_cmd_18, atck14_cmd_19, atck14_cmd_20, atck14_cmd_21, atck14_cmd_22, atck14_cmd_23,
	atck14_cmd_24, atck14_cmd_25, atck14_cmd_26, atck14_cmd_27, atck14_cmd_28, atck14_cmd_29,
	atck14_cmd_30, atck14_cmd_31,
};

const Uint8 atck14_cmd_00[] = {
	STAND,
	WAIT2( 10 ),
	WALK_B( 160 ),
	WAIT2( 8 ),
	NAG_VFLIP_ADJ3( LV_HEAVY, LEVER_FORWARD ),
	WAIT2( 14 ),
	SP_CMD( pl14_sp_shienbu, 0, PK_PUNCH, LV_HEAVY, 0 ),
	IF_LEVEL( 24 ),
		IF_GAUGE( 1 ),
			IF_RND( 2 ),
				SP_CMD( pl14_sp_pow, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		WAIT2( 14 ),
		SP_CMD( pl14_sp_shienbu, 0, PK_PUNCH, LV_HEAVY, 0 ),
		WAIT2( 14 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck14_cmd_01[] = {
	STAND,
	WAIT2( 8 ),
	CMD_JUMP( 14 ),
	JUMP_FLAG_SET,
		CMD_JUMP( 20 ),
		TIMER_SET( 60 ),
		IF_HEIGHT( 1, 128 ),
			CMD_JUMP( 9 ),
		CMD_ELSE,
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	IF_LEVEL( 24 ),
		IF_EM_ATCK( pl14_at_upper ),
			WAIT2( 4 ),
			CROUCH,
			NAG_VFLIP_ADJ3( LV_HEAVY, LEVER_DOWN | LEVER_FORWARD ),
			SJUMP3( DIR_FORWARD ),
			WAIT( 2 ),
			SJUMP_FLAG_SET,
				CMD_JUMP( 4 ),
				JUMP_FLAG_CLR,
			CMD_ELSE,
				JUMP_FLAG_CLR,
			CMD_ENDIF,
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 8 ),
	CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 7 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 8 ),
	CMD_EXIT,
};

const Uint8 atck14_cmd_02[] = {
	STAND,
	WAIT2( 8 ),
	TIMER_SET( 60 ),
	WALK_EM( 64 ),
	CMD_JUMP( 11 ),
	STAND,
	WAIT2( 8 ),
	CMD_EXIT,
};

const Uint8 atck14_cmd_03[] = {
	STAND,
	CMD_JUMP( 0 ),
	WAIT2( 8 ),
	TIMER_SET( 60 ),
	WALK_ATCK( ATCK_CROUCH_HK ),
		CROUCH,
		WAIT2( 2 ),
		PUNCH( LV_HEAVY ),
	CMD_ENDIF,
	WAIT2( 8 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck14_cmd_04[] = {
	STAND,
	WAIT2( 14 ),
	TIMER_SET( 60 ),
	IF_EM_ATCK( pl14_at_seiten ),
		SP_CMD( pl14_sp_seiten, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ELSE,
		SP_CMD( pl14_sp_wall, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ENDIF,
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck14_cmd_05[] = {
	STAND,
	WAIT2( 4 ),
	DASH_ATCK( ATCK_CROUCH_LK ),
		CROUCH,
		KICK( LV_LIGHT ),
		IF_LEVEL( 16 ),
			IF_EM_ATCK( ATCK_CROUCH_HK ),
				CROUCH,
				NAG_VFLIP_ADJ3( LV_HEAVY, LEVER_DOWN | LEVER_FORWARD ),
				SJUMP3( DIR_FORWARD ),
				SJUMP_FLAG_SET,
					CMD_JUMP( 4 ),
					JUMP_FLAG_CLR,
				CMD_ENDIF,
			CMD_ELSE,
				IF_EM_ATCK( ATCK_CROUCH_HK ),
					CROUCH,
					PUNCH( LV_HEAVY ),
				CMD_ELSE,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			PUNCH( LV_HEAVY ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_RND( 16 ),
			CMD_JUMP( 54 ),
		CMD_ELSE,
			CMD_JUMP( 59 ),
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck14_cmd_06[] = {
	STAND,
	WAIT2( 4 ),
	CMD_JUMP( 0 ),
	WAIT2( 4 ),
	SP_CMD( pl14_sp_shienbu, 0, PK_PUNCH, LV_HEAVY, 0 ),
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck14_cmd_07[] = {
	STAND,
	WAIT2( 4 ),
	SP_CMD( pl14_sp_seiten, 0, PK_PUNCH, LV_HEAVY, 0 ),
	STAND,
	WAIT2( 2 ),
	IF_EM_ATCK( ATCK_CROUCH_HK ),
		CROUCH,
		PUNCH( LV_HEAVY ),
	CMD_ENDIF,
	STAND,
	CMD_EXIT,
};

const Uint8 atck14_cmd_08[] = {
	STAND,
	IF_GAUGE( 1 ),
		IF_RND( 16 ),
			WAIT2( 2 ),
			SP_CMD( pl14_sp_tenchi, 0, PK_PUNCH, LV_LIGHT, 0 ),
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

const Uint8 atck14_cmd_09[] = {
	STAND,
	IF_GAUGE( 1 ),
		IF_RND( 16 ),
			IF_EM_ATCK( pl14_at_enou ),
				SP_CMD( pl14_sp_tenchi, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		STAND,
		WAIT2( 2 ),
	CMD_ENDIF,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck14_cmd_10[] = {
	STAND,
	WAIT2( 2 ),
	IF_RND( 16 ),
		IF_GAUGE( 3 ),
			STAND,
			SP_V_COMBI,
			STAND,
			WAIT2( 2 ),
		CMD_ELSE,
			IF_GAUGE( 1 ),
				STAND,
				SP_CMD( pl14_sp_enou, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				CMD_JUMP( 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		WAIT( 1 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck14_cmd_11[] = {
	STAND,
	DASH_B( 96 ),
	STAND,
	PARTNER_CALL,
	WAIT2( 2 ),
	SP_CMD( pl14_sp_shienbu, 0, PK_PUNCH, LV_HEAVY, 0 ),
	STAND,
	CMD_JUMP( 24 ),
	STAND,
	WAIT2( 2 ),
	IF_RND( 16 ),
		IF_RND( 10 ),
			DASH_B( 96 ),
			STAND,
		CMD_ENDIF,
		WAIT2( 2 ),
		IF_LEVEL( 24 ),
			IF_GAUGE( 1 ),
				IF_RND( 10 ),
					SP_CMD( pl14_sp_tenchi, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ENDIF,
			CMD_ELSE,
				SP_CMD( pl14_sp_shienbu, 0, PK_PUNCH, LV_HEAVY, 0 ),
				CMD_JUMP( 24 ),
			CMD_ENDIF,
		CMD_ELSE,
			SP_CMD( pl14_sp_shienbu, 0, PK_PUNCH, LV_HEAVY, 0 ),
			STAND,
			CMD_JUMP( 24 ),
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck14_cmd_12[] = {
	STAND,
	WAIT2( 2 ),
	CMD_JUMP( 14 ),
	JUMP( DIR_FORWARD ),
	WAIT( 4 ),
	JUMP_FLAG_SET,
		CMD_JUMP( 20 ),
		WAIT( 6 ),
		CANCEL_SET2,
			PUNCH( LV_LIGHT ),
			PUNCH( LV_LIGHT ),
			CANCEL_CLR,
		CMD_ENDIF,
		SP_CMD( pl14_sp_fuusetsu, 0, PK_PUNCH, LV_HEAVY, 0 ),
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	WAIT2( 8 ),
	CMD_JUMP( 21 ),
	STAND,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck14_cmd_13[] = {
	STAND,
	LOCK_SET,
	WAIT2( 6 ),
	CMD_JUMP( 0 ),
	CMD_JUMP( 25 ),
	CMD_DUMMY,
	JUMP_FLAG_SET,
		CMD_JUMP( 20 ),
		WAIT( 10 ),
		IF_LEVEL( 10 ),
			IF_RND( 16 ),
				IF_RND( 12 ),
					JUMP_LEVEL( 38, 13, 39, 21, 40, 31, 41 ),
				CMD_ELSE,
					JUMP_LEVEL( 38, 20, 39, 26, 40, 31, 41 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
		TIMER_SET( 60 ),
		IF_HEIGHT( 1, 128 ),
			IF_RND( 12 ),
				JUMP_LEVEL( 35, 8, 35, 22, 9, 31, 9 ),
			CMD_ELSE,
				JUMP_LEVEL( 35, 8, 35, 22, 35, 24, 9 ),
			CMD_ENDIF,
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	JUMP_LEVEL( 7, 8, 7, 15, 7, 24, 7 ),
	LOCK_CLR,
	WAIT2( 4 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck14_cmd_14[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck14_cmd_15[] = {
	CMD_EXIT,
};

const Uint8 atck14_cmd_16[] = {
	CMD_JUMP( 54 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck14_cmd_17[] = {
	CMD_JUMP( 56 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck14_cmd_18[] = {
	CMD_JUMP( 53 ),
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck14_cmd_19[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck14_cmd_20[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck14_cmd_21[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck14_cmd_22[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck14_cmd_23[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck14_cmd_24[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck14_cmd_25[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck14_cmd_26[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck14_cmd_27[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck14_cmd_28[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck14_cmd_29[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck14_cmd_30[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck14_cmd_31[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

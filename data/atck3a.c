/* Servbot (character 3a): the computer's attack lists.
   What it does once a waiting list's CMD_NEXT has sent it to attack. PLAYER_WORK's cpu_adrs[1]
   points here.

   atck3a_tbl     one record (CPU_DATA): the offsets of the three tables.
   atck3a_rate    the chance, out of 32, that a list's CMD_NEXT2 starts another attack list, by
                  the level of play.
   atck3a_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   atck3a_cmd     the lists: a pick is a number in this table.
   atck3a_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                  list of lib3a.c.
   atck3a_unused_N  bytes that nothing in the file reaches. */
#include "player.h"
#include "pl3a_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const atck3a_sel[], *const atck3a_cmd[];
extern const Uint8 atck3a_rate[], atck3a_sel_0[], atck3a_sel_3[], atck3a_unused_0[], atck3a_cmd_00[],
	atck3a_cmd_01[], atck3a_cmd_02[], atck3a_cmd_03[], atck3a_cmd_04[], atck3a_cmd_05[],
	atck3a_cmd_06[], atck3a_cmd_07[], atck3a_cmd_08[], atck3a_cmd_09[], atck3a_cmd_10[],
	atck3a_cmd_11[], atck3a_cmd_12[], atck3a_cmd_13[], atck3a_cmd_14[], atck3a_cmd_15[],
	atck3a_cmd_16[], atck3a_cmd_17[], atck3a_cmd_18[], atck3a_cmd_19[], atck3a_cmd_20[],
	atck3a_cmd_21[], atck3a_cmd_22[], atck3a_cmd_23[], atck3a_cmd_24[], atck3a_cmd_25[],
	atck3a_cmd_26[], atck3a_cmd_27[], atck3a_cmd_28[], atck3a_cmd_29[], atck3a_cmd_30[],
	atck3a_cmd_31[];

const CPU_DATA atck3a_tbl[] = {
	{ ( Sint32 )atck3a_rate, ( Sint32 )atck3a_sel, ( Sint32 )atck3a_cmd },
};

const Uint8 atck3a_rate[] = {
	  8,   8,   8,   8,   9,   9,  10,  10,  11,  11,  12,  12,  13,  13,  14,  14,
	 15,  15,  16,  16,  17,  17,  18,  18,  19,  20,  21,  22,  24,  26,  28,  30,
};

const Uint8 *const atck3a_sel[] = {
	atck3a_sel_0, atck3a_sel_0, atck3a_sel_0, atck3a_sel_3, atck3a_sel_3, atck3a_sel_3,
	atck3a_sel_3, atck3a_sel_3,
};

const Uint8 atck3a_sel_0[] = {
	  1,   2,   3,   4,   5,   6,   7,   8,   9,   1,   2,   3,   4,   5,   6,   7,
	  8,   9,   1,   2,   3,   4,   5,   6,   7,   8,   9,   1,   2,   3,   4,  31,
};

const Uint8 atck3a_sel_3[] = {
	  1,   2,   3,   4,   5,   6,   7,   8,   9,   1,   2,   3,   4,   5,   6,   7,
	  8,   9,   1,   2,   3,   4,   5,   6,   7,   8,   9,   1,   2,   3,   4,   5,
};

const Uint8 *const atck3a_cmd[] = {
	atck3a_cmd_00, atck3a_cmd_01, atck3a_cmd_02, atck3a_cmd_03, atck3a_cmd_04, atck3a_cmd_05,
	atck3a_cmd_06, atck3a_cmd_07, atck3a_cmd_08, atck3a_cmd_09, atck3a_cmd_10, atck3a_cmd_11,
	atck3a_cmd_12, atck3a_cmd_13, atck3a_cmd_14, atck3a_cmd_15, atck3a_cmd_16, atck3a_cmd_17,
	atck3a_cmd_18, atck3a_cmd_19, atck3a_cmd_20, atck3a_cmd_21, atck3a_cmd_22, atck3a_cmd_23,
	atck3a_cmd_24, atck3a_cmd_25, atck3a_cmd_26, atck3a_cmd_27, atck3a_cmd_28, atck3a_cmd_29,
	atck3a_cmd_30, atck3a_cmd_31,
};

const Uint8 atck3a_unused_0[] = {
	 15,  64,   8,
};

const Uint8 atck3a_cmd_00[] = {
	JUMP_RND( 63, 64, 63, 64 ),
	CMD_EXIT,
};

const Uint8 atck3a_cmd_01[] = {
	DASH_EM( 16 ),
	IF_LEVEL( 16 ),
		IF_EM_ATCK( ATCK_CROUCH_LK ),
			CROUCH,
			CANCEL_SET2,
				KICK( LV_LIGHT ),
				KICK( LV_LIGHT ),
				KICK( LV_HEAVY ),
				SP_CMD( pl3a_sp_fire, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			SP_CMD( pl3a_sp_fire, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_EM_ATCK( ATCK_CROUCH_LK ),
			CROUCH,
			CANCEL_SET2,
				KICK( LV_LIGHT ),
				KICK( LV_LIGHT ),
				KICK( LV_HEAVY ),
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			SP_CMD( pl3a_sp_fire, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck3a_cmd_02[] = {
	STAND,
	DASH_B( 240 ),
	WAIT( 30 ),
	SP_CMD( pl3a_sp_engo, 0, PK_PUNCH, LV_LIGHT, 0 ),
	SP_CMD( pl3a_sp_engo, 0, PK_PUNCH, LV_LIGHT, 0 ),
	IF_LEVEL( 16 ),
		IF_RND( 16 ),
			SP_CMD( pl3a_sp_engo_k, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ELSE,
			SP_CMD( pl3a_sp_engo, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ELSE,
		SP_CMD( pl3a_sp_engo, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck3a_cmd_03[] = {
	SP_CMD( pl3a_sp_engo, 0, PK_PUNCH, LV_HEAVY, 0 ),
	DASH_EM( 16 ),
	DASH_F( 16 ),
	IF_LEVEL( 4 ),
		JUMP_LEVEL( 65, 8, 66, 16, 67, 24, 67 ),
	CMD_ELSE,
		IF_EM_ATCK( ATCK_CROUCH_LK ),
			CROUCH,
			CANCEL_SET2,
				KICK( LV_LIGHT ),
				KICK( LV_LIGHT ),
				KICK( LV_HEAVY ),
				SP_CMD( pl3a_sp_fire, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck3a_cmd_04[] = {
	STAND,
	WAIT2( 14 ),
	DASH_EM( 16 ),
	PARTNER_CALL,
	IF_LEVEL( 24 ),
		JUMP( DIR_FORWARD ),
	CMD_ELSE,
		JUMP( DIR_BACK ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck3a_cmd_05[] = {
	STAND,
	DASH_ATCK( ATCK_STAND_LP ),
		WAIT2( 8 ),
		IF_LEVEL( 24 ),
			CROUCH,
			CANCEL_SET2,
				KICK( LV_HEAVY ),
				SP_CMD( pl3a_sp_fire, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			SP_CMD( pl3a_sp_fire, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck3a_cmd_06[] = {
	STAND,
	WAIT2( 8 ),
	DASH_ATCK( ATCK_STAND_LP ),
		WAIT2( 8 ),
		SP_CMD( pl3a_sp_bull, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck3a_cmd_07[] = {
	WAIT2( 8 ),
	IF_LEVEL( 24 ),
		JUMP_EM( 68, 68, 68, 68, 73, 73, 73, 73 ),
	CMD_ELSE,
		IF_LEVEL( 16 ),
			CMD_JUMP( 61 ),
		CMD_ELSE,
			IF_LEVEL( 8 ),
				CMD_JUMP( 60 ),
			CMD_ELSE,
				CMD_JUMP( 72 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck3a_cmd_08[] = {
	CMD_JUMP( 71 ),
	CMD_EXIT,
};

const Uint8 atck3a_cmd_09[] = {
	STAND,
	IF_GAUGE( 1 ),
		SP_V_COMBI,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck3a_cmd_10[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck3a_cmd_11[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck3a_cmd_12[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck3a_cmd_13[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck3a_cmd_14[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck3a_cmd_15[] = {
	CMD_EXIT,
};

const Uint8 atck3a_cmd_16[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck3a_cmd_17[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck3a_cmd_18[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck3a_cmd_19[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck3a_cmd_20[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck3a_cmd_21[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck3a_cmd_22[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck3a_cmd_23[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck3a_cmd_24[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck3a_cmd_25[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck3a_cmd_26[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck3a_cmd_27[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck3a_cmd_28[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck3a_cmd_29[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck3a_cmd_30[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck3a_cmd_31[] = {
	IF_GAUGE( 1 ),
		IF_RND( 4 ),
			IF_EM_ATCK( ATCK_CROUCH_HK ),
				PARTNER_CHANGE,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

/* Juggernaut (character 29): the computer's attack lists.
   What it does once a waiting list's CMD_NEXT has sent it to attack. PLAYER_WORK's cpu_adrs[1]
   points here.

   atck29_tbl     one record (CPU_DATA): the offsets of the three tables.
   atck29_rate    the chance, out of 32, that a list's CMD_NEXT2 starts another attack list, by
                  the level of play.
   atck29_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   atck29_cmd     the lists: a pick is a number in this table.
   atck29_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                  list of lib29.c. */
#include "player.h"
#include "pl29_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const atck29_sel[], *const atck29_cmd[];
extern const Uint8 atck29_rate[], atck29_sel_0[], atck29_sel_1[], atck29_sel_2[], atck29_sel_3[],
	atck29_sel_4[], atck29_sel_5[], atck29_sel_6[], atck29_sel_7[], atck29_cmd_00[], atck29_cmd_01[],
	atck29_cmd_02[], atck29_cmd_03[], atck29_cmd_04[], atck29_cmd_05[], atck29_cmd_06[],
	atck29_cmd_07[], atck29_cmd_08[], atck29_cmd_09[];

const CPU_DATA atck29_tbl[] = {
	{ ( Sint32 )atck29_rate, ( Sint32 )atck29_sel, ( Sint32 )atck29_cmd },
};

const Uint8 atck29_rate[] = {
	 11,  12,  13,  14,  14,  15,  15,  16,  16,  17,  18,  19,  20,  21,  22,  23,
	 24,  25,  26,  27,  28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 *const atck29_sel[] = {
	atck29_sel_0, atck29_sel_1, atck29_sel_2, atck29_sel_3, atck29_sel_4, atck29_sel_5,
	atck29_sel_6, atck29_sel_7,
};

const Uint8 atck29_sel_0[] = {
	  1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   8,   7,   2,   2,   2,   2,
	  2,   2,   2,   2,   3,   3,   3,   3,   0,   0,   0,   4,   4,   4,   5,   6,
};

const Uint8 atck29_sel_1[] = {
	  1,   1,   1,   1,   1,   1,   5,   5,   2,   2,   2,   2,   2,   2,   3,   3,
	  3,   3,   4,   4,   4,   4,   4,   0,   0,   0,   0,   0,   5,   6,   7,   8,
};

const Uint8 atck29_sel_2[] = {
	  1,   1,   1,   1,   5,   5,   2,   2,   2,   2,   3,   3,   3,   3,   4,   4,
	  4,   4,   4,   4,   0,   0,   0,   0,   0,   0,   5,   5,   8,   8,   6,   7,
};

const Uint8 atck29_sel_3[] = {
	  1,   1,   1,   8,   3,   3,   3,   8,   2,   2,   2,   8,   4,   4,   4,   4,
	  4,   4,   0,   0,   0,   0,   0,   0,   5,   5,   5,   5,   6,   6,   7,   7,
};

const Uint8 atck29_sel_4[] = {
	  1,   1,   1,   8,   2,   2,   8,   3,   3,   8,   0,   0,   0,   0,   0,   8,
	  4,   4,   4,   4,   4,   8,   5,   5,   5,   5,   5,   5,   6,   6,   7,   7,
};

const Uint8 atck29_sel_5[] = {
	  1,   1,   8,   8,   2,   2,   3,   3,   4,   4,   4,   4,   4,   8,   0,   0,
	  0,   0,   0,   8,   5,   5,   5,   5,   5,   5,   5,   6,   6,   6,   7,   7,
};

const Uint8 atck29_sel_6[] = {
	  0,   0,   0,   0,   0,   0,   8,   8,   1,   1,   1,   2,   2,   3,   5,   5,
	  4,   4,   4,   4,   4,   4,   8,   8,   5,   5,   8,   8,   6,   6,   7,   7,
};

const Uint8 atck29_sel_7[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   2,   2,   3,   3,
	  4,   4,   4,   4,   8,   8,   8,   8,   5,   5,   5,   6,   6,   6,   7,   7,
};

const Uint8 *const atck29_cmd[] = {
	atck29_cmd_00, atck29_cmd_01, atck29_cmd_02, atck29_cmd_03, atck29_cmd_04, atck29_cmd_05,
	atck29_cmd_06, atck29_cmd_07, atck29_cmd_08, atck29_cmd_09, atck29_cmd_09, atck29_cmd_09,
	atck29_cmd_09, atck29_cmd_09, atck29_cmd_09, atck29_cmd_09, atck29_cmd_09, atck29_cmd_09,
	atck29_cmd_09, atck29_cmd_09, atck29_cmd_09, atck29_cmd_09, atck29_cmd_09, atck29_cmd_09,
	atck29_cmd_09, atck29_cmd_09, atck29_cmd_09, atck29_cmd_09, atck29_cmd_09, atck29_cmd_09,
	atck29_cmd_09, atck29_cmd_09,
};

const Uint8 atck29_cmd_00[] = {
	LOCK_SET,
	CMD_JUMP( 0 ),
	CMD_JUMP( 58 ),
	IF_LEVEL( 16 ),
		IF_LEVEL( 24 ),
			JUMP_RND( 7, 9, 5, 10 ),
		CMD_ELSE,
			JUMP_RND( 5, 12, 9, 7 ),
		CMD_ENDIF,
	CMD_ELSE,
		JUMP_RND( 12, 12, 7, 9 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 atck29_cmd_01[] = {
	LOCK_SET,
	IF_LEVEL( 24 ),
		JUMP_RND( 4, 8, 6, 9 ),
	CMD_ELSE,
		IF_EM_ATCK( ATCK_STAND_LP ),
			CANCEL_SET,
				STAND,
				WAIT2( 8 ),
				PUNCH( LV_LIGHT ),
				CMD_JUMP( 58 ),
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			JUMP_RND( 3, 5, 1, 12 ),
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck29_cmd_02[] = {
	LOCK_SET,
	IF_LEVEL( 24 ),
		CMD_JUMP( 58 ),
		JUMP_RND( 5, 7, 9, 10 ),
	CMD_ELSE,
		IF_EM_ATCK( ATCK_CROUCH_MK ),
			CANCEL_SET2,
				CROUCH,
				WAIT2( 8 ),
				KICK( LV_LIGHT ),
				CMD_JUMP( 58 ),
				CMD_JUMP( 1 ),
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			JUMP_RND( 1, 3, 6, 5 ),
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck29_cmd_03[] = {
	LOCK_SET,
	WALK_ATCK( pl29_at_fwd_hp ),
		STAND,
		WAIT2( 8 ),
		NAG_VFLIP_ADJ3( LV_HEAVY, LEVER_FORWARD ),
	CMD_ELSE,
		GUARD( 30 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck29_cmd_04[] = {
	LOCK_SET,
	CMD_JUMP( 0 ),
	IF_LEVEL( 24 ),
		JUMP_RND( 4, 8, 11, 6 ),
	CMD_ELSE,
		IF_LEVEL( 16 ),
			JUMP_RND( 3, 4, 8, 13 ),
		CMD_ELSE,
			JUMP_RND( 13, 13, 4, 8 ),
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck29_cmd_05[] = {
	LOCK_SET,
	CMD_JUMP( 0 ),
	IF_GAUGE( 1 ),
		CMD_JUMP( 11 ),
	CMD_ELSE,
		IF_LEVEL( 20 ),
			JUMP_RND( 4, 6, 8, 9 ),
		CMD_ELSE,
			JUMP( DIR_FORWARD ),
			WAIT2( 12 ),
			CMD_JUMP( 58 ),
			JUMP_FLAG_SET,
				GUARD( 30 ),
				TIMER_SET( 90 ),
				IF_EM_ATCK( ATCK_JUMP_MP ),
					WAIT2( 12 ),
					KICK( LV_LIGHT ),
				CMD_ENDIF,
				JUMP_FLAG_CLR,
			CMD_ENDIF,
			WAIT2( 8 ),
			CROUCH,
			KICK( LV_LIGHT ),
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck29_cmd_06[] = {
	LOCK_SET,
	CMD_JUMP( 0 ),
	IF_LEVEL( 10 ),
		JUMP_RND( 4, 6, 7, 10 ),
	CMD_ELSE,
		STAND,
		DASH_ATCK( ATCK_STAND_HP ),
			CANCEL_SET2,
				WAIT2( 8 ),
				STAND,
				PUNCH( LV_HEAVY ),
				CMD_JUMP( 58 ),
				SP_CMD( pl29_sp_punch, 0, PK_PUNCH, LV_HEAVY, 0 ),
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck29_cmd_07[] = {
	LOCK_SET,
	CMD_JUMP( 0 ),
	STAND,
	WAIT( 1 ),
	TIMER_SET( 60 ),
	IF_AREA( 64, 32, 64, 32 ),
		JUMP_RND( 5, 7, 9, 12 ),
	CMD_ELSE,
		SP_CMD( pl29_sp_powerup, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck29_cmd_08[] = {
	LOCK_SET,
	CMD_JUMP( 0 ),
	STAND,
	WAIT2( 8 ),
	TIMER_SET( 0 ),
	IF_DIST( 48, 48, 48, 48 ),
		SP_CMD( pl29_sp_quake, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ELSE,
		IF_DIST( 48, 48, 96, 48 ),
			SP_CMD( pl29_sp_quake, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ELSE,
			SP_CMD( pl29_sp_quake, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	IF_LEVEL( 10 ),
		JUMP_RND( 1, 3, 13, 5 ),
	CMD_ELSE,
		JUMP_RND( 1, 12, 13, 13 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck29_cmd_09[] = {
	CMD_EXIT,
};

/* Thanos (character 36): the computer's attack lists.
   What it does once a waiting list's CMD_NEXT has sent it to attack. PLAYER_WORK's cpu_adrs[1]
   points here.

   atck36_tbl     one record (CPU_DATA): the offsets of the three tables.
   atck36_rate    the chance, out of 32, that a list's CMD_NEXT2 starts another attack list, by
                  the level of play.
   atck36_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   atck36_cmd     the lists: a pick is a number in this table.
   atck36_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                  list of lib36.c. */
#include "player.h"
#include "pl36_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const atck36_sel[], *const atck36_cmd[];
extern const Uint8 atck36_rate[], atck36_sel_0[], atck36_sel_1[], atck36_sel_2[], atck36_sel_3[],
	atck36_sel_4[], atck36_sel_5[], atck36_sel_6[], atck36_sel_7[], atck36_cmd_00[], atck36_cmd_01[],
	atck36_cmd_02[], atck36_cmd_03[], atck36_cmd_04[], atck36_cmd_05[], atck36_cmd_06[],
	atck36_cmd_07[], atck36_cmd_08[], atck36_cmd_09[], atck36_cmd_10[], atck36_cmd_31[];

const CPU_DATA atck36_tbl[] = {
	{ ( Sint32 )atck36_rate, ( Sint32 )atck36_sel, ( Sint32 )atck36_cmd },
};

const Uint8 atck36_rate[] = {
	 11,  12,  13,  14,  14,  15,  15,  16,  16,  17,  18,  19,  20,  21,  22,  23,
	 24,  25,  26,  27,  28,  29,  30,  31,  31,  31,  31,  31,  31,  31,  31,  31,
};

const Uint8 *const atck36_sel[] = {
	atck36_sel_0, atck36_sel_1, atck36_sel_2, atck36_sel_3, atck36_sel_4, atck36_sel_5,
	atck36_sel_6, atck36_sel_7,
};

const Uint8 atck36_sel_0[] = {
	  1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   8,   7,   2,   2,   2,   2,
	  2,   2,   2,   2,   3,   3,   3,  31,   0,   0,   0,   4,   4,   4,   5,   6,
};

const Uint8 atck36_sel_1[] = {
	  1,   1,   1,   1,   1,   1,   5,   5,   2,   2,   2,   2,   2,   2,   3,   3,
	  3,   3,   4,   4,   4,   4,   4,  31,   0,   0,   0,   0,   5,   6,   7,   8,
};

const Uint8 atck36_sel_2[] = {
	  1,   1,   1,   1,   5,   5,   2,   2,   2,   2,   3,   3,   3,   3,   4,   4,
	  4,   4,   4,   4,   0,   0,   0,  31,   0,   0,   5,   5,   8,   8,   6,   7,
};

const Uint8 atck36_sel_3[] = {
	  1,   1,   1,   8,   3,   3,   3,   8,   2,   2,   2,   8,   4,   4,   4,   4,
	  4,   4,   0,   0,   0,   0,   0,   0,   5,   5,   5,   5,   6,   6,   7,   7,
};

const Uint8 atck36_sel_4[] = {
	  1,   1,   1,   8,   2,   2,   8,   3,   3,   8,   0,   0,   0,   0,   0,   8,
	  4,   4,   4,   4,   4,   8,   5,   5,   5,   9,   9,   9,   6,   6,   7,   7,
};

const Uint8 atck36_sel_5[] = {
	  1,   1,   8,   8,   2,   2,   3,   3,   4,   4,   4,   4,   4,   8,   0,   0,
	  9,   9,   9,   8,   5,   9,   9,   9,   5,   5,   5,   6,   6,   6,   7,   9,
};

const Uint8 atck36_sel_6[] = {
	  0,   0,   0,   0,   9,   9,   8,   8,   1,   1,   9,   2,   2,   3,   5,   9,
	  4,   4,   4,   9,   9,   9,   8,   8,   5,   5,   8,   8,   6,   6,   7,   7,
};

const Uint8 atck36_sel_7[] = {
	  0,   0,   0,   0,   9,   9,   9,   9,   1,   1,   9,   9,   2,   9,   3,   9,
	  4,   4,   9,   9,   8,   8,   9,   9,   5,   5,   9,   6,   6,   9,   7,   9,
};

const Uint8 *const atck36_cmd[] = {
	atck36_cmd_00, atck36_cmd_01, atck36_cmd_02, atck36_cmd_03, atck36_cmd_04, atck36_cmd_05,
	atck36_cmd_06, atck36_cmd_07, atck36_cmd_08, atck36_cmd_09, atck36_cmd_10, atck36_cmd_10,
	atck36_cmd_10, atck36_cmd_10, atck36_cmd_10, atck36_cmd_10, atck36_cmd_10, atck36_cmd_10,
	atck36_cmd_10, atck36_cmd_10, atck36_cmd_10, atck36_cmd_10, atck36_cmd_10, atck36_cmd_10,
	atck36_cmd_10, atck36_cmd_10, atck36_cmd_10, atck36_cmd_10, atck36_cmd_10, atck36_cmd_10,
	atck36_cmd_10, atck36_cmd_31,
};

const Uint8 atck36_cmd_00[] = {
	IF_LEVEL( 24 ),
		CMD_JUMP( 0 ),
	CMD_ENDIF,
	IF_RND( 16 ),
		STAND,
		WAIT2( 8 ),
		TIMER_SET( 30 ),
		IF_DIST( 48, 48, 48, 48 ),
			IF_LEVEL( 16 ),
				JUMP_RND( 2, 5, 9, 1 ),
			CMD_ELSE,
				JUMP_RND( 11, 11, 20, 20 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_DIST( 48, 48, 96, 48 ),
				SP_CMD( pl36_sp_sphere, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				IF_DIST( 48, 48, 144, 48 ),
					SP_CMD( pl36_sp_sphere, 0, PK_PUNCH, LV_HEAVY, 0 ),
				CMD_ELSE,
					SP_CMD( pl36_sp_sphere, 0, PK_PUNCH, LV_HEAVY, 0 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 11 ),
	CMD_ENDIF,
	CMD_NEXT2,
};

const Uint8 atck36_cmd_01[] = {
	IF_LEVEL( 16 ),
		CMD_JUMP( 0 ),
	CMD_ENDIF,
	IF_LEVEL( 28 ),
		JUMP_RND( 2, 6, 8, 11 ),
	CMD_ELSE,
		IF_LEVEL( 20 ),
			JUMP_RND( 11, 11, 11, 10 ),
		CMD_ELSE,
			WAIT2( 8 ),
			SP_CMD( pl36_sp_sphere, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
};

const Uint8 atck36_cmd_02[] = {
	IF_LEVEL( 16 ),
		CMD_JUMP( 0 ),
	CMD_ENDIF,
	IF_LEVEL( 28 ),
		JUMP_RND( 3, 6, 8, 10 ),
	CMD_ELSE,
		IF_LEVEL( 20 ),
			JUMP_RND( 10, 10, 10, 11 ),
		CMD_ELSE,
			WAIT2( 8 ),
			SP_CMD( pl36_sp_titan, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
};

const Uint8 atck36_cmd_03[] = {
	IF_LEVEL( 16 ),
		CMD_JUMP( 0 ),
	CMD_ENDIF,
	IF_DIST( 96, 48, 96, 48 ),
		STAND,
		WAIT2( 8 ),
		NAG_VFLIP_ADJ( 1, LEVER_FORWARD ),
	CMD_ELSE,
		IF_LEVEL( 16 ),
			JUMP_RND( 4, 4, 4, 9 ),
		CMD_ELSE,
			JUMP_RND( 11, 11, 11, 4 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
};

const Uint8 atck36_cmd_04[] = {
	IF_LEVEL( 24 ),
		CMD_JUMP( 0 ),
	CMD_ENDIF,
	IF_LEVEL( 28 ),
		JUMP_RND( 7, 7, 4, 20 ),
	CMD_ELSE,
		IF_LEVEL( 20 ),
			JUMP_RND( 20, 20, 7, 9 ),
		CMD_ELSE,
			JUMP_RND( 20, 20, 20, 7 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
};

const Uint8 atck36_cmd_05[] = {
	LOCK_SET,
	IF_GAUGE( 1 ),
		CMD_JUMP( 1 ),
	CMD_ELSE,
		IF_LEVEL( 24 ),
			JUMP_RND( 3, 5, 8, 9 ),
		CMD_ELSE,
			IF_LEVEL( 16 ),
				JUMP_RND( 4, 4, 21, 21 ),
			CMD_ELSE,
				IF_LEVEL( 8 ),
					JUMP_RND( 21, 21, 21, 4 ),
				CMD_ELSE,
					JUMP_RND( 11, 20, 10, 21 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_NEXT2,
};

const Uint8 atck36_cmd_06[] = {
	LOCK_SET,
	IF_GAUGE( 1 ),
		CMD_JUMP( 1 ),
	CMD_ELSE,
		IF_LEVEL( 24 ),
			JUMP_RND( 2, 5, 5, 7 ),
		CMD_ELSE,
			IF_LEVEL( 16 ),
				JUMP_RND( 5, 2, 22, 7 ),
			CMD_ELSE,
				IF_LEVEL( 8 ),
					JUMP_RND( 22, 22, 2, 3 ),
				CMD_ELSE,
					JUMP_RND( 11, 11, 20, 20 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
};

const Uint8 atck36_cmd_07[] = {
	IF_LEVEL( 24 ),
		IF_RND( 8 ),
			STAND,
			WAIT( 1 ),
			TIMER_SET( 10 ),
			IF_AREA( 64, 32, 64, 32 ),
				JUMP_RND( 2, 4, 5, 6 ),
			CMD_ELSE,
				IF_GAUGE( 2 ),
					JUMP_RND( 12, 13, 14, 15 ),
				CMD_ELSE,
					JUMP_RND( 2, 6, 1, 1 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
};

const Uint8 atck36_cmd_08[] = {
	LOCK_SET,
	IF_LEVEL( 16 ),
		CMD_JUMP( 0 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 8 ),
	TIMER_SET( 30 ),
	IF_DIST( 32, 48, 32, 48 ),
		JUMP_RND( 2, 4, 5, 6 ),
	CMD_ELSE,
		IF_DIST( 32, 48, 64, 48 ),
			SP_CMD( pl36_sp_sphere, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			IF_DIST( 32, 48, 128, 48 ),
				SP_CMD( pl36_sp_sphere, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ELSE,
				SP_CMD( pl36_sp_sphere, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	IF_RND( 8 ),
		WAIT2( 8 ),
		SP_CMD( pl36_sp_titan, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ELSE,
		JUMP_RND( 2, 5, 7, 9 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_NEXT2,
};

const Uint8 atck36_cmd_09[] = {
	STAND,
	IF_LEVEL( 24 ),
		IF_RND( 16 ),
			DASH_EM( 32 ),
			IF_EM_ATCK( ATCK_CROUCH_LP ),
				CANCEL_SET,
					CROUCH,
					KICK( LV_LIGHT ),
					STAND,
					PUNCH( LV_LIGHT ),
					KICK( LV_HEAVY ),
					CANCEL_CLR,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
		WAIT2( 8 ),
		PARTNER_CALL,
		DASH_F( 16 ),
		JUMP( DIR_FORWARD ),
		JUMP_FLAG_SET,
			TIMER_SET( 30 ),
			IF_EM_ATCK( ATCK_JUMP_LK ),
				CANCEL_SET2,
					KICK( LV_LIGHT ),
					PUNCH( LV_LIGHT ),
					CANCEL_CLR,
				CMD_ENDIF,
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
		IF_RND( 16 ),
			DASH_F( 16 ),
			JUMP( DIR_FORWARD ),
			JUMP_FLAG_SET,
				TIMER_SET( 30 ),
				IF_EM_ATCK( ATCK_JUMP_LK ),
					CANCEL_SET2,
						KICK( LV_LIGHT ),
						PUNCH( LV_LIGHT ),
						CANCEL_CLR,
					CMD_ENDIF,
				CMD_ENDIF,
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			DASH_F( 16 ),
			SJUMP( DIR_FORWARD ),
			SJUMP_FLAG_SET,
				WAIT( 20 ),
				NAG_VFLIP_ADJ4( LV_HEAVY, LEVER_DOWN ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
		DASH_EM( 16 ),
		IF_EM_ATCK( ATCK_CROUCH_LK ),
			DASH_F( 16 ),
			CANCEL_SET,
				CROUCH,
				KICK( LV_LIGHT ),
				PUNCH( LV_LIGHT ),
				PUNCH( LV_HEAVY ),
				SJUMP3( DIR_UP ),
				CANCEL_CLR,
			CMD_ENDIF,
			SJUMP_FLAG_SET,
				CMD_JUMP( 25 ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 16 ),
			IF_RND( 16 ),
				STAND,
				SP_CMD( pl36_sp_sphere, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
			WAIT2( 10 ),
			PARTNER_CALL,
			DASH_EM( 16 ),
			IF_RND( 16 ),
				SJUMP( DIR_FORWARD ),
				SJUMP_FLAG_SET,
					WAIT( 30 ),
					NAG_VFLIP_ADJ4( LV_HEAVY, LEVER_DOWN ),
					JUMP_FLAG_CLR,
				CMD_ENDIF,
			CMD_ENDIF,
			DASH_EM( 16 ),
			IF_EM_ATCK( ATCK_CROUCH_LK ),
				DASH_F( 16 ),
				IF_GAUGE( 1 ),
					CANCEL_SET,
						CROUCH,
						KICK( LV_LIGHT ),
						KICK( LV_LIGHT ),
						SP_CMD( pl36_sp_power, 0, PK_PUNCH, LV_LIGHT, 0 ),
						CANCEL_CLR,
					CMD_ENDIF,
				CMD_ELSE,
					CANCEL_SET,
						CROUCH,
						KICK( LV_LIGHT ),
						KICK( LV_LIGHT ),
						CANCEL_CLR,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			IF_RND( 16 ),
				STAND,
				SP_CMD( pl36_sp_sphere, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
			WAIT2( 12 ),
			PARTNER_CALL,
			DASH_EM( 16 ),
			IF_RND( 16 ),
				SJUMP( DIR_BACK ),
				SJUMP_FLAG_SET,
					WAIT( 30 ),
					NAG_VFLIP_ADJ4( LV_HEAVY, LEVER_DOWN ),
					JUMP_FLAG_CLR,
				CMD_ENDIF,
			CMD_ENDIF,
			DASH_EM( 16 ),
			IF_EM_ATCK( ATCK_CROUCH_LK ),
				DASH_F( 16 ),
				CANCEL_SET,
					CROUCH,
					KICK( LV_LIGHT ),
					KICK( LV_LIGHT ),
					CANCEL_CLR,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_NEXT2,
};

const Uint8 atck36_cmd_10[] = {
	CMD_EXIT,
};

const Uint8 atck36_cmd_31[] = {
	IF_GAUGE( 1 ),
		IF_RND( 4 ),
			IF_EM_ATCK( ATCK_STAND_HK ),
				PARTNER_CHANGE,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

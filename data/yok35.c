/* Blackheart (character 35): the computer's answers.
   What it does about an attack that is coming. PLAYER_WORK's cpu_answer_adrs points here;
   em_answer_sel picks a list from it.

   yok35_tbl        a record (CPU_DATA) for each answer. The row of the attack in its owner's hit
                    table (hit_dtXX.c) names the answer; YK_GD030 and YK_GD060 are the first two.
                    A record is the offsets of its sel (twice: no rate is read) and its cmd.
   yok35_sel_AA     32 picks; one is taken at random.
   yok35_cmd_AA     the answer's lists: a pick is a number in this table.
   yok35_cmd_AA_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name
                    a list of lib35.c. */
#include "player.h"
#include "pl35_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const yok35_cmd_00[], *const yok35_cmd_01[], *const yok35_cmd_02[],
	*const yok35_cmd_03[], *const yok35_cmd_04[], *const yok35_cmd_05[], *const yok35_cmd_06[],
	*const yok35_cmd_07[], *const yok35_cmd_08[];
extern const Uint8 yok35_sel_00[], yok35_sel_01[], yok35_sel_02[], yok35_sel_03[], yok35_sel_04[],
	yok35_sel_05[], yok35_sel_06[], yok35_sel_07[], yok35_sel_08[], yok35_cmd_00_00[],
	yok35_cmd_00_01[], yok35_cmd_00_02[], yok35_cmd_00_03[], yok35_cmd_00_04[], yok35_cmd_00_05[],
	yok35_cmd_00_06[], yok35_cmd_00_07[], yok35_cmd_00_08[], yok35_cmd_00_09[], yok35_cmd_01_00[],
	yok35_cmd_01_01[], yok35_cmd_01_02[], yok35_cmd_01_03[], yok35_cmd_01_04[], yok35_cmd_01_05[],
	yok35_cmd_01_06[], yok35_cmd_01_07[], yok35_cmd_01_08[], yok35_cmd_01_09[], yok35_cmd_02_00[],
	yok35_cmd_02_01[], yok35_cmd_02_02[], yok35_cmd_02_03[], yok35_cmd_02_04[], yok35_cmd_02_05[],
	yok35_cmd_02_06[], yok35_cmd_02_07[], yok35_cmd_02_08[], yok35_cmd_02_09[], yok35_cmd_03_00[],
	yok35_cmd_03_01[], yok35_cmd_03_02[], yok35_cmd_03_03[], yok35_cmd_03_04[], yok35_cmd_03_05[],
	yok35_cmd_03_06[], yok35_cmd_03_07[], yok35_cmd_03_08[], yok35_cmd_03_09[], yok35_cmd_04_00[],
	yok35_cmd_04_01[], yok35_cmd_04_02[], yok35_cmd_04_03[], yok35_cmd_04_04[], yok35_cmd_04_05[],
	yok35_cmd_04_06[], yok35_cmd_04_07[], yok35_cmd_04_08[], yok35_cmd_04_09[], yok35_cmd_05_00[],
	yok35_cmd_05_01[], yok35_cmd_05_02[], yok35_cmd_05_03[], yok35_cmd_05_04[], yok35_cmd_05_05[],
	yok35_cmd_05_06[], yok35_cmd_05_07[], yok35_cmd_05_08[], yok35_cmd_05_09[], yok35_cmd_06_00[],
	yok35_cmd_06_01[], yok35_cmd_06_02[], yok35_cmd_06_03[], yok35_cmd_06_04[], yok35_cmd_06_05[],
	yok35_cmd_06_06[], yok35_cmd_06_07[], yok35_cmd_06_08[], yok35_cmd_06_09[], yok35_cmd_07_00[],
	yok35_cmd_07_01[], yok35_cmd_07_02[], yok35_cmd_07_03[], yok35_cmd_07_04[], yok35_cmd_07_05[],
	yok35_cmd_07_06[], yok35_cmd_07_07[], yok35_cmd_07_08[], yok35_cmd_07_09[], yok35_cmd_08_00[],
	yok35_cmd_08_01[], yok35_cmd_08_02[], yok35_cmd_08_03[], yok35_cmd_08_04[], yok35_cmd_08_05[],
	yok35_cmd_08_06[], yok35_cmd_08_07[], yok35_cmd_08_08[], yok35_cmd_08_09[];

const CPU_DATA yok35_tbl[] = {
	{ ( Sint32 )yok35_sel_00, ( Sint32 )yok35_sel_00, ( Sint32 )yok35_cmd_00 },	/* YK_GD030 */
	{ ( Sint32 )yok35_sel_01, ( Sint32 )yok35_sel_01, ( Sint32 )yok35_cmd_01 },	/* YK_GD060 */
	{ ( Sint32 )yok35_sel_02, ( Sint32 )yok35_sel_02, ( Sint32 )yok35_cmd_02 },	/* 2 */
	{ ( Sint32 )yok35_sel_03, ( Sint32 )yok35_sel_03, ( Sint32 )yok35_cmd_03 },	/* 3 */
	{ ( Sint32 )yok35_sel_04, ( Sint32 )yok35_sel_04, ( Sint32 )yok35_cmd_04 },	/* 4 */
	{ ( Sint32 )yok35_sel_05, ( Sint32 )yok35_sel_05, ( Sint32 )yok35_cmd_05 },	/* 5 */
	{ ( Sint32 )yok35_sel_06, ( Sint32 )yok35_sel_06, ( Sint32 )yok35_cmd_06 },	/* 6 */
	{ ( Sint32 )yok35_sel_07, ( Sint32 )yok35_sel_07, ( Sint32 )yok35_cmd_07 },	/* 7 */
	{ ( Sint32 )yok35_sel_08, ( Sint32 )yok35_sel_08, ( Sint32 )yok35_cmd_08 },	/* 8 */
};

const Uint8 yok35_sel_00[] = {
	  0,   0,   0,   1,   2,   2,   3,   1,   1,   0,   0,   1,   2,   2,   3,   2,
	  2,   0,   1,   1,   2,   3,   3,   4,   3,   0,   1,   1,   2,   3,   3,   4,
};

const Uint8 *const yok35_cmd_00[] = {
	yok35_cmd_00_00, yok35_cmd_00_01, yok35_cmd_00_02, yok35_cmd_00_03, yok35_cmd_00_04, yok35_cmd_00_05,
	yok35_cmd_00_06, yok35_cmd_00_07, yok35_cmd_00_08, yok35_cmd_00_09,
};

const Uint8 yok35_sel_01[] = {
	  0,   0,   1,   2,   3,   3,   4,   4,   0,   0,   1,   2,   3,   3,   4,   4,
	  0,   1,   1,   2,   3,   4,   4,   4,   0,   1,   1,   2,   3,   4,   4,   4,
};

const Uint8 *const yok35_cmd_01[] = {
	yok35_cmd_01_00, yok35_cmd_01_01, yok35_cmd_01_02, yok35_cmd_01_03, yok35_cmd_01_04, yok35_cmd_01_05,
	yok35_cmd_01_06, yok35_cmd_01_07, yok35_cmd_01_08, yok35_cmd_01_09,
};

const Uint8 yok35_sel_02[] = {
	  0,   1,   2,   3,   4,   5,   4,   1,   0,   1,   2,   3,   4,   5,   4,   1,
	  0,   1,   2,   3,   4,   5,   4,   1,   0,   1,   2,   3,   4,   5,   4,   4,
};

const Uint8 *const yok35_cmd_02[] = {
	yok35_cmd_02_00, yok35_cmd_02_01, yok35_cmd_02_02, yok35_cmd_02_03, yok35_cmd_02_04, yok35_cmd_02_05,
	yok35_cmd_02_06, yok35_cmd_02_07, yok35_cmd_02_08, yok35_cmd_02_09,
};

const Uint8 yok35_sel_03[] = {
	  0,   1,   2,   3,   4,   5,   0,   6,   0,   1,   2,   3,   4,   5,   0,   6,
	  0,   1,   2,   3,   4,   5,   0,   6,   0,   1,   2,   3,   4,   5,   1,   6,
};

const Uint8 *const yok35_cmd_03[] = {
	yok35_cmd_03_00, yok35_cmd_03_01, yok35_cmd_03_02, yok35_cmd_03_03, yok35_cmd_03_04, yok35_cmd_03_05,
	yok35_cmd_03_06, yok35_cmd_03_07, yok35_cmd_03_08, yok35_cmd_03_09,
};

const Uint8 yok35_sel_04[] = {
	  0,   1,   2,   3,   4,   5,   6,   7,   0,   1,   2,   3,   4,   5,   6,   7,
	  0,   1,   2,   3,   4,   5,   6,   7,   0,   1,   2,   3,   4,   5,   6,   7,
};

const Uint8 *const yok35_cmd_04[] = {
	yok35_cmd_04_00, yok35_cmd_04_01, yok35_cmd_04_02, yok35_cmd_04_03, yok35_cmd_04_04, yok35_cmd_04_05,
	yok35_cmd_04_06, yok35_cmd_04_07, yok35_cmd_04_08, yok35_cmd_04_09,
};

const Uint8 yok35_sel_05[] = {
	  0,   1,   2,   3,   4,   5,   6,   0,   0,   1,   2,   3,   4,   5,   6,   1,
	  0,   1,   2,   3,   4,   5,   6,   2,   0,   1,   2,   3,   4,   5,   6,   3,
};

const Uint8 *const yok35_cmd_05[] = {
	yok35_cmd_05_00, yok35_cmd_05_01, yok35_cmd_05_02, yok35_cmd_05_03, yok35_cmd_05_04, yok35_cmd_05_05,
	yok35_cmd_05_06, yok35_cmd_05_07, yok35_cmd_05_08, yok35_cmd_05_09,
};

const Uint8 yok35_sel_06[] = {
	  0,   1,   2,   3,   4,   5,   0,   4,   0,   1,   2,   3,   4,   5,   0,   4,
	  0,   1,   2,   3,   4,   5,   0,   4,   0,   1,   2,   3,   4,   5,   0,   4,
};

const Uint8 *const yok35_cmd_06[] = {
	yok35_cmd_06_00, yok35_cmd_06_01, yok35_cmd_06_02, yok35_cmd_06_03, yok35_cmd_06_04, yok35_cmd_06_05,
	yok35_cmd_06_06, yok35_cmd_06_07, yok35_cmd_06_08, yok35_cmd_06_09,
};

const Uint8 yok35_sel_07[] = {
	  0,   1,   2,   3,   4,   5,   3,   4,   0,   1,   2,   3,   4,   5,   3,   4,
	  0,   1,   2,   3,   4,   5,   3,   4,   0,   1,   2,   3,   4,   5,   3,   4,
};

const Uint8 *const yok35_cmd_07[] = {
	yok35_cmd_07_00, yok35_cmd_07_01, yok35_cmd_07_02, yok35_cmd_07_03, yok35_cmd_07_04, yok35_cmd_07_05,
	yok35_cmd_07_06, yok35_cmd_07_07, yok35_cmd_07_08, yok35_cmd_07_09,
};

const Uint8 yok35_sel_08[] = {
	  0,   0,   1,   1,   2,   2,   3,   3,   0,   0,   1,   1,   2,   2,   3,   3,
	  0,   0,   1,   1,   2,   2,   3,   3,   0,   0,   1,   1,   2,   2,   3,   3,
};

const Uint8 *const yok35_cmd_08[] = {
	yok35_cmd_08_00, yok35_cmd_08_01, yok35_cmd_08_02, yok35_cmd_08_03, yok35_cmd_08_04, yok35_cmd_08_05,
	yok35_cmd_08_06, yok35_cmd_08_07, yok35_cmd_08_08, yok35_cmd_08_09,
};

const Uint8 yok35_cmd_00_00[] = {
	LOCK_SET,
	VCOUNTER_SET( 8 ),
	IF_ATCK_COME,
		CMD_JUMP( 17 ),
	CMD_ELSE,
		WAIT2( 4 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_00_01[] = {
	LOCK_SET,
	VCOUNTER_SET( 8 ),
	IF_ATCK_COME,
		CMD_JUMP( 19 ),
	CMD_ELSE,
		DASH_B( 128 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_00_02[] = {
	LOCK_SET,
	VCOUNTER_SET( 8 ),
	IF_ATCK_COME,
		GUARD2( 30 ),
	CMD_ELSE,
		IF_RND( 16 ),
			WAIT2( 2 ),
		CMD_ELSE,
			DASH_F( 512 ),
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_00_03[] = {
	LOCK_SET,
	VCOUNTER_SET( 8 ),
	IF_ATCK_COME,
		CMD_JUMP( 17 ),
	CMD_ELSE,
		IF_RND( 8 ),
			DASH_F( 512 ),
			CMD_JUMP( 1 ),
			CMD_JUMP( 0 ),
			CMD_JUMP( 18 ),
		CMD_ELSE,
			DASH_B( 512 ),
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_00_04[] = {
	LOCK_SET,
	VCOUNTER_SET( 8 ),
	IF_RND( 16 ),
		WAIT( 20 ),
	CMD_ELSE,
		WAIT( 4 ),
		DASH_F( 512 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_00_05[] = {
	CMD_EXIT,
};

const Uint8 yok35_cmd_00_06[] = {
	CMD_EXIT,
};

const Uint8 yok35_cmd_00_07[] = {
	CMD_EXIT,
};

const Uint8 yok35_cmd_00_08[] = {
	CMD_EXIT,
};

const Uint8 yok35_cmd_00_09[] = {
	CMD_EXIT,
};

const Uint8 yok35_cmd_01_00[] = {
	LOCK_SET,
	WAIT2( 4 ),
	SJUMP( DIR_UP ),
	SJUMP_FLAG_SET,
		CMD_JUMP( 18 ),
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_01_01[] = {
	LOCK_SET,
	WAIT2( 4 ),
	SJUMP( DIR_BACK ),
	SJUMP_FLAG_SET,
		CMD_JUMP( 18 ),
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_01_02[] = {
	LOCK_SET,
	WAIT2( 4 ),
	SJUMP( DIR_FORWARD ),
	CMD_JUMP( 18 ),
	JUMP_FLAG_CLR,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_01_03[] = {
	LOCK_SET,
	CMD_JUMP( 18 ),
	JUMP( DIR_FORWARD ),
	JUMP_FLAG_SET,
		TIMER_SET( 100 ),
		IF_HEIGHT( 0xff, 76 ),
			FLY( LEVER_FORWARD, 512 ),
		CMD_ELSE,
			GUARD( 30 ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ELSE,
		GUARD( 30 ),
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_01_04[] = {
	LOCK_SET,
	CMD_JUMP( 27 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_01_05[] = {
	CMD_EXIT,
};

const Uint8 yok35_cmd_01_06[] = {
	CMD_EXIT,
};

const Uint8 yok35_cmd_01_07[] = {
	CMD_EXIT,
};

const Uint8 yok35_cmd_01_08[] = {
	CMD_EXIT,
};

const Uint8 yok35_cmd_01_09[] = {
	CMD_EXIT,
};

const Uint8 yok35_cmd_02_00[] = {
	LOCK_SET,
	CROUCH,
	CMD_JUMP( 17 ),
	STAND,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_02_01[] = {
	LOCK_SET,
	IF_RND( 16 ),
		CMD_JUMP( 19 ),
	CMD_ELSE,
		IF_AREA( 160, 80, 0, 0 ),
			JUMP( DIR_FORWARD ),
			JUMP_FLAG_SET,
				TIMER_SET( 0xc8 ),
				IF_HEIGHT( 0, 80 ),
					IF_RND( 16 ),
						PUNCH( LV_HEAVY ),
					CMD_ELSE,
						PUNCH( LV_LIGHT ),
					CMD_ENDIF,
				CMD_ENDIF,
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			SJUMP( DIR_FORWARD ),
			SJUMP_FLAG_SET,
				TIMER_SET( 100 ),
				IF_HEIGHT( 0xff, 128 ),
					KICK( LV_HEAVY ),
				CMD_ENDIF,
				CMD_JUMP( 18 ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_02_02[] = {
	WAIT2( 4 ),
	LOCK_SET,
	SJUMP( DIR_UP ),
	SJUMP_FLAG_SET,
		CMD_JUMP( 18 ),
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_02_03[] = {
	LOCK_SET,
	SJUMP( DIR_FORWARD ),
	SJUMP_FLAG_SET,
		CMD_JUMP( 18 ),
		TIMER_SET( 0xc8 ),
		IF_HEIGHT( 1, 128 ),
		CMD_ENDIF,
		TIMER_SET( 50 ),
		IF_EM_ATCK( ATCK_JUMP_MP ),
			PUNCH( LV_LIGHT ),
		CMD_ELSE,
			IF_RND( 16 ),
				FLY( LEVER_FORWARD, 512 ),
			CMD_ELSE,
				PUNCH( LV_HEAVY ),
			CMD_ENDIF,
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_02_04[] = {
	LOCK_SET,
	CMD_JUMP( 27 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_02_05[] = {
	LOCK_SET,
	IF_AREA2( 0, 0, 0, 0 ),
		GUARD( 120 ),
		IF_AREA2( 0, 0, 0, 0 ),
			GUARD( 120 ),
		CMD_ELSE,
			IF_EM_ATCK( ATCK_STAND_MP ),
				PUNCH( LV_LIGHT ),
			CMD_ELSE,
				DASH_F( 512 ),
				IF_EM_ATCK( ATCK_CROUCH_LP ),
					CROUCH,
					PUNCH( LV_LIGHT ),
					PUNCH( LV_LIGHT ),
					PUNCH( LV_LIGHT ),
					STAND,
					WAIT( 8 ),
					WAIT2( 14 ),
				CMD_ELSE,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		TIMER_SET( 60 ),
		WALK_ATCK( ATCK_STAND_LK ),
			KICK( LV_LIGHT ),
		CMD_ELSE,
			KICK( LV_HEAVY ),
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	WAIT( 10 ),
	WAIT2( 14 ),
	CMD_EXIT,
};

const Uint8 yok35_cmd_02_06[] = {
	CMD_EXIT,
};

const Uint8 yok35_cmd_02_07[] = {
	CMD_EXIT,
};

const Uint8 yok35_cmd_02_08[] = {
	CMD_EXIT,
};

const Uint8 yok35_cmd_02_09[] = {
	CMD_EXIT,
};

const Uint8 yok35_cmd_03_00[] = {
	LOCK_SET,
	IF_RND( 16 ),
		WAIT( 20 ),
	CMD_ELSE,
		WAIT2( 4 ),
		IF_AREA2( 96, 160, 0, 0 ),
			GUARD2( 30 ),
		CMD_ELSE,
			WAIT2( 6 ),
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_03_01[] = {
	LOCK_SET,
	WAIT2( 4 ),
	IF_AREA2( 128, 128, 0, 0 ),
		GUARD2( 30 ),
	CMD_ELSE,
		WAIT2( 0 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_03_02[] = {
	LOCK_SET,
	WAIT2( 4 ),
	IF_DIST( 64, 96, 0, 256 ),
		CMD_JUMP( 10 ),
	CMD_ELSE,
		TIMER_SET( 20 ),
		IF_AREA2( 96, 160, 0, 0 ),
			GUARD2( 100 ),
		CMD_ELSE,
			IF_RND( 16 ),
				DASH_B( 128 ),
			CMD_ELSE,
				IF_RND( 16 ),
					SJUMP( DIR_FORWARD ),
					JUMP_FLAG_SET,
						TIMER_SET( 30 ),
						IF_EM_ATCK( ATCK_JUMP_MP ),
							PUNCH( LV_LIGHT ),
						CMD_ELSE,
							WAIT2( 14 ),
							PUNCH( LV_HEAVY ),
						CMD_ENDIF,
						JUMP_FLAG_CLR,
					CMD_ELSE,
						JUMP_FLAG_CLR,
					CMD_ENDIF,
				CMD_ELSE,
					SJUMP( DIR_UP ),
					SJUMP_FLAG_SET,
						TIMER_SET( 0xc8 ),
						IF_EM_ATCK( ATCK_JUMP_MP ),
							PUNCH( LV_LIGHT ),
						CMD_ELSE,
						CMD_ENDIF,
						JUMP_FLAG_CLR,
					CMD_ELSE,
						JUMP_FLAG_CLR,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_03_03[] = {
	LOCK_SET,
	WAIT2( 4 ),
	SJUMP( DIR_BACK ),
	SJUMP_FLAG_SET,
		CMD_JUMP( 18 ),
		WAIT( 120 ),
		WAIT2( 14 ),
		FLY( LEVER_BACK, 512 ),
		JUMP_FLAG_CLR,
	CMD_ELSE,
		GUARD2( 30 ),
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_03_04[] = {
	LOCK_SET,
	WAIT2( 2 ),
	DASH_F( 512 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_03_05[] = {
	LOCK_SET,
	WAIT2( 6 ),
	IF_AREA2( 0, 0, 0, 0 ),
		GUARD2( 30 ),
		IF_AREA2( 0, 0, 0, 0 ),
			GUARD2( 30 ),
		CMD_ELSE,
			IF_EM_ATCK( ATCK_CROUCH_MP ),
				CROUCH,
				PUNCH( LV_LIGHT ),
				STAND,
			CMD_ELSE,
				SP_CMD( pl35_sp_thunder, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		JUMP( DIR_FORWARD ),
		JUMP_FLAG_SET,
			TIMER2_SET( 120 ),
			TIMER_SET( 120 ),
			IF_HEIGHT( 0, 72 ),
				IF_EM_ATCK( ATCK_JUMP_MP ),
					PUNCH( LV_LIGHT ),
				CMD_ELSE,
					IF_EM_ATCK( ATCK_JUMP_LK ),
						KICK( LV_LIGHT ),
					CMD_ELSE,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ELSE,
				TIMER2_SET( 100 ),
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ELSE,
			GUARD2( 30 ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT( 10 ),
	WAIT2( 14 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_03_06[] = {
	LOCK_SET,
	GUARD2( 30 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_03_07[] = {
	CMD_EXIT,
};

const Uint8 yok35_cmd_03_08[] = {
	CMD_EXIT,
};

const Uint8 yok35_cmd_03_09[] = {
	CMD_EXIT,
};

const Uint8 yok35_cmd_04_00[] = {
	LOCK_SET,
	VCOUNTER_SET( 8 ),
	IF_AREA( 160, 128, 0, 0 ),
		CMD_JUMP( 17 ),
	CMD_ELSE,
		TIMER_SET( 60 ),
		IF_AREA2( 88, 128, 0, 0 ),
			JUMP( DIR_FORWARD ),
			JUMP_FLAG_SET,
				CMD_JUMP( 20 ),
				CMD_JUMP( 25 ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			IF_RND( 16 ),
				DASH_F( 512 ),
			CMD_ELSE,
				GUARD( 30 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_04_01[] = {
	LOCK_SET,
	VCOUNTER_SET( 8 ),
	IF_AREA2( 160, 128, 0, 0 ),
		WAIT2( 4 ),
		TIMER2_SET( 0xb4 ),
	CMD_ELSE,
		DASH_F( 256 ),
		GUARD( 30 ),
		IF_RND( 16 ),
			CMD_JUMP( 1 ),
			CMD_JUMP( 2 ),
		CMD_ELSE,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_04_02[] = {
	LOCK_SET,
	VCOUNTER_SET( 8 ),
	WAIT2( 0 ),
	IF_AREA2( 160, 0, 0, 0 ),
		IF_AREA2( 96, 0, 0, 0 ),
			DASH_F( 256 ),
			IF_RND( 8 ),
				CMD_JUMP( 1 ),
				CMD_JUMP( 2 ),
			CMD_ENDIF,
		CMD_ELSE,
			JUMP( DIR_FORWARD ),
			JUMP_FLAG_SET,
				TIMER2_SET( 60 ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		PUNCH( LV_LIGHT ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_04_03[] = {
	LOCK_SET,
	VCOUNTER_SET( 4 ),
	CMD_JUMP( 19 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_04_04[] = {
	LOCK_SET,
	VCOUNTER_SET( 8 ),
	WAIT2( 4 ),
	DASH_F( 512 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_04_05[] = {
	LOCK_SET,
	VCOUNTER_SET( 8 ),
	WAIT2( 4 ),
	IF_AREA2( 0, 0, 0, 0 ),
		WAIT2( 4 ),
		GUARD( 120 ),
		IF_AREA2( 0, 0, 0, 0 ),
			WAIT2( 4 ),
			GUARD( 120 ),
		CMD_ELSE,
			IF_EM_ATCK( ATCK_CROUCH_LK ),
				CROUCH,
				KICK( LV_LIGHT ),
				KICK( LV_LIGHT ),
				STAND,
			CMD_ELSE,
				IF_RND( 16 ),
					PUNCH( LV_HEAVY ),
				CMD_ELSE,
					KICK( LV_HEAVY ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_RND( 16 ),
			CMD_JUMP( 12 ),
		CMD_ELSE,
			CROUCH,
			PUNCH( LV_HEAVY ),
			STAND,
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT( 10 ),
	WAIT2( 14 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_04_06[] = {
	LOCK_SET,
	JUMP( DIR_FORWARD ),
	JUMP_FLAG_SET,
		CMD_JUMP( 18 ),
		IF_AREA( 160, 0, 0, 0 ),
			TIMER_SET( 100 ),
			IF_HEIGHT( 0, 64 ),
				PUNCH( LV_LIGHT ),
			CMD_ENDIF,
		CMD_ELSE,
			TIMER_SET( 100 ),
			IF_HEIGHT( 0, 64 ),
				PUNCH( LV_HEAVY ),
			CMD_ENDIF,
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_04_07[] = {
	LOCK_SET,
	SJUMP( DIR_FORWARD ),
	SJUMP_FLAG_SET,
		TIMER_SET( 0xf0 ),
		IF_HEIGHT( 0xff, 64 ),
			PUNCH( LV_HEAVY ),
		CMD_ELSE,
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_04_08[] = {
	CMD_EXIT,
};

const Uint8 yok35_cmd_04_09[] = {
	CMD_EXIT,
};

const Uint8 yok35_cmd_05_00[] = {
	LOCK_SET,
	VCOUNTER_SET( 8 ),
	TIMER2_SET( 120 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_05_01[] = {
	LOCK_SET,
	VCOUNTER_SET( 8 ),
	GUARD( 30 ),
	IF_EM_ATCK( ATCK_STAND_MK ),
		KICK( LV_LIGHT ),
		WAIT( 10 ),
	CMD_ELSE,
		TIMER2_SET( 80 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_05_02[] = {
	LOCK_SET,
	VCOUNTER_SET( 8 ),
	WAIT2( 4 ),
	IF_EM_ATCK( ATCK_STAND_MP ),
		PUNCH( LV_LIGHT ),
	CMD_ELSE,
		DASH_B( 180 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_05_03[] = {
	LOCK_SET,
	VCOUNTER_SET( 8 ),
	IF_EM_ATCK( pl35_at_thunder_counter ),
		SP_CMD( pl35_sp_thunder, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ELSE,
		TIMER2_SET( 0xb4 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_05_04[] = {
	LOCK_SET,
	VCOUNTER_SET( 8 ),
	TIMER2_SET( 80 ),
	JUMP( DIR_BACK ),
	JUMP_FLAG_SET,
		TIMER_SET( 120 ),
		IF_HEIGHT( 0xff, 71 ),
			IF_EM_ATCK( ATCK_JUMP_MP ),
				PUNCH( LV_LIGHT ),
			CMD_ELSE,
				PUNCH( LV_HEAVY ),
			CMD_ENDIF,
		CMD_ELSE,
			FLY( LEVER_BACK, 512 ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ELSE,
		GUARD( 30 ),
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_05_05[] = {
	LOCK_SET,
	VCOUNTER_SET( 8 ),
	WAIT2( 4 ),
	DASH_F( 512 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_05_06[] = {
	LOCK_SET,
	TIMER_SET( 16 ),
	IF_ATCK_COME,
		GUARD( 30 ),
	CMD_ELSE,
		CMD_JUMP( 10 ),
		IF_LEVEL( 24 ),
			CMD_JUMP( 4 ),
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_05_07[] = {
	CMD_EXIT,
};

const Uint8 yok35_cmd_05_08[] = {
	CMD_EXIT,
};

const Uint8 yok35_cmd_05_09[] = {
	CMD_EXIT,
};

const Uint8 yok35_cmd_06_00[] = {
	LOCK_SET,
	JUMP( DIR_BACK ),
	JUMP_FLAG_SET,
		TIMER_SET( 120 ),
		IF_HEIGHT( 1, 72 ),
			PUNCH( LV_HEAVY ),
		CMD_ELSE,
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ELSE,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	CROUCH,
	KICK( LV_HEAVY ),
	STAND,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_06_01[] = {
	LOCK_SET,
	DASH_B( 512 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_06_02[] = {
	LOCK_SET,
	DASH_F( 512 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_06_03[] = {
	LOCK_SET,
	KICK( LV_LIGHT ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_06_04[] = {
	LOCK_SET,
	CROUCH,
	PUNCH( LV_LIGHT ),
	PUNCH( LV_LIGHT ),
	STAND,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_06_05[] = {
	LOCK_SET,
	SJUMP( DIR_BACK ),
	SJUMP_FLAG_SET,
		TIMER_SET( 0xc8 ),
		IF_EM_ATCK( ATCK_JUMP_MP ),
			PUNCH( LV_LIGHT ),
		CMD_ELSE,
			KICK( LV_HEAVY ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ELSE,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	IF_EM_ATCK( ATCK_CROUCH_HP ),
		CROUCH,
		PUNCH( LV_HEAVY ),
		STAND,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_06_06[] = {
	CMD_EXIT,
};

const Uint8 yok35_cmd_06_07[] = {
	CMD_EXIT,
};

const Uint8 yok35_cmd_06_08[] = {
	CMD_EXIT,
};

const Uint8 yok35_cmd_06_09[] = {
	CMD_EXIT,
};

const Uint8 yok35_cmd_07_00[] = {
	LOCK_SET,
	CMD_JUMP( 17 ),
	IF_CATCH,
		NAG_VFLIP_ADJ( 0, LEVER_FORWARD ),
	CMD_ELSE,
		GUARD( 30 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_07_01[] = {
	LOCK_SET,
	VCOUNTER_SET( 8 ),
	IF_DIST( 128, 80, 0, 0 ),
		GUARD( 30 ),
		CMD_JUMP( 19 ),
	CMD_ELSE,
		CMD_JUMP( 19 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_07_02[] = {
	LOCK_SET,
	IF_DIST( 128, 80, 0, 0 ),
		GUARD( 30 ),
		TIMER_SET( 10 ),
		IF_AREA2( 0, 0, 0, 0 ),
			CMD_JUMP( 19 ),
			TIMER_SET( 10 ),
			IF_AREA2( 0, 0, 0, 0 ),
				CMD_JUMP( 19 ),
				TIMER_SET( 10 ),
				IF_AREA2( 0, 0, 0, 0 ),
					CMD_JUMP( 19 ),
				CMD_ELSE,
					IF_LEVEL( 28 ),
						CMD_JUMP( 12 ),
					CMD_ELSE,
						IF_LEVEL( 22 ),
							CMD_JUMP( 10 ),
						CMD_ELSE,
							IF_LEVEL( 13 ),
								PUNCH( LV_HEAVY ),
							CMD_ELSE,
								IF_LEVEL( 6 ),
									CROUCH,
									PUNCH( LV_LIGHT ),
									STAND,
								CMD_ELSE,
									KICK( LV_HEAVY ),
								CMD_ENDIF,
							CMD_ENDIF,
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 28 ),
					CMD_JUMP( 12 ),
				CMD_ELSE,
					IF_LEVEL( 22 ),
						CMD_JUMP( 10 ),
					CMD_ELSE,
						IF_LEVEL( 13 ),
							PUNCH( LV_HEAVY ),
						CMD_ELSE,
							IF_LEVEL( 6 ),
								CROUCH,
								PUNCH( LV_LIGHT ),
								STAND,
							CMD_ELSE,
								KICK( LV_HEAVY ),
							CMD_ENDIF,
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 28 ),
				CMD_JUMP( 12 ),
			CMD_ELSE,
				IF_LEVEL( 22 ),
					CMD_JUMP( 10 ),
				CMD_ELSE,
					IF_LEVEL( 13 ),
						PUNCH( LV_HEAVY ),
					CMD_ELSE,
						IF_LEVEL( 6 ),
							CROUCH,
							PUNCH( LV_LIGHT ),
							STAND,
						CMD_ELSE,
							KICK( LV_HEAVY ),
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_AREA2( 0, 0, 0, 0 ),
			CMD_JUMP( 19 ),
			TIMER_SET( 10 ),
			IF_AREA2( 0, 0, 0, 0 ),
				CMD_JUMP( 19 ),
			CMD_ELSE,
				IF_LEVEL( 28 ),
					CMD_JUMP( 12 ),
				CMD_ELSE,
					IF_LEVEL( 22 ),
						CMD_JUMP( 10 ),
					CMD_ELSE,
						IF_LEVEL( 13 ),
							PUNCH( LV_HEAVY ),
						CMD_ELSE,
							IF_LEVEL( 6 ),
								CROUCH,
								PUNCH( LV_LIGHT ),
								STAND,
							CMD_ELSE,
								KICK( LV_HEAVY ),
							CMD_ENDIF,
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 28 ),
				CMD_JUMP( 12 ),
			CMD_ELSE,
				IF_LEVEL( 22 ),
					CMD_JUMP( 10 ),
				CMD_ELSE,
					IF_LEVEL( 13 ),
						PUNCH( LV_HEAVY ),
					CMD_ELSE,
						IF_LEVEL( 6 ),
							CROUCH,
							PUNCH( LV_LIGHT ),
							STAND,
						CMD_ELSE,
							KICK( LV_HEAVY ),
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 28 ),
			CMD_JUMP( 12 ),
		CMD_ELSE,
			IF_LEVEL( 22 ),
				CMD_JUMP( 10 ),
			CMD_ELSE,
				IF_LEVEL( 13 ),
					PUNCH( LV_HEAVY ),
				CMD_ELSE,
					IF_LEVEL( 6 ),
						CROUCH,
						PUNCH( LV_LIGHT ),
						STAND,
					CMD_ELSE,
						KICK( LV_HEAVY ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_07_03[] = {
	LOCK_SET,
	IF_AREA( 96, 128, 0, 0 ),
		CMD_JUMP( 17 ),
	CMD_ELSE,
		TIMER_SET( 20 ),
		IF_AREA( 160, 128, 0, 0 ),
			WAIT2( 12 ),
			IF_LEVEL( 28 ),
				IF_RND( 4 ),
					DASH_F( 512 ),
				CMD_ELSE,
					DASH_B( 512 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 22 ),
					IF_RND( 8 ),
						DASH_F( 512 ),
					CMD_ELSE,
						DASH_B( 512 ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_LEVEL( 16 ),
						IF_RND( 12 ),
							DASH_F( 512 ),
						CMD_ELSE,
							DASH_B( 512 ),
						CMD_ENDIF,
					CMD_ELSE,
						IF_LEVEL( 12 ),
							IF_RND( 16 ),
								DASH_F( 512 ),
							CMD_ELSE,
								DASH_B( 512 ),
							CMD_ENDIF,
						CMD_ELSE,
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_07_04[] = {
	LOCK_SET,
	IF_AREA( 96, 128, 0, 0 ),
		CMD_JUMP( 17 ),
	CMD_ELSE,
		TIMER_SET( 20 ),
		IF_AREA( 160, 128, 0, 0 ),
			WAIT2( 12 ),
			IF_LEVEL( 28 ),
				IF_RND( 16 ),
					SJUMP( DIR_UP ),
				CMD_ELSE,
					SJUMP( DIR_BACK ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 22 ),
					IF_RND( 16 ),
						SJUMP( DIR_UP ),
					CMD_ELSE,
						JUMP( DIR_BACK ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_LEVEL( 16 ),
						IF_RND( 18 ),
							JUMP( DIR_BACK ),
						CMD_ELSE,
							GUARD2( 120 ),
						CMD_ENDIF,
					CMD_ELSE,
						IF_LEVEL( 12 ),
							IF_RND( 16 ),
								SJUMP( DIR_BACK ),
							CMD_ELSE,
								DASH_B( 512 ),
							CMD_ENDIF,
						CMD_ELSE,
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_07_05[] = {
	LOCK_SET,
	CROUCH,
	TIMER_SET( 20 ),
	IF_AREA2( 0, 0, 0, 0 ),
		CMD_JUMP( 17 ),
	CMD_ELSE,
		IF_EM_ATCK( ATCK_CROUCH_LP ),
			PUNCH( LV_LIGHT ),
			PUNCH( LV_LIGHT ),
		CMD_ELSE,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_07_06[] = {
	CMD_EXIT,
};

const Uint8 yok35_cmd_07_07[] = {
	CMD_EXIT,
};

const Uint8 yok35_cmd_07_08[] = {
	CMD_EXIT,
};

const Uint8 yok35_cmd_07_09[] = {
	CMD_EXIT,
};

const Uint8 yok35_cmd_08_00[] = {
	LOCK_SET,
	WAIT2( 4 ),
	SJUMP( DIR_FORWARD ),
	SJUMP_FLAG_SET,
		CMD_JUMP( 18 ),
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_08_01[] = {
	LOCK_SET,
	WAIT2( 4 ),
	SJUMP( DIR_UP ),
	SJUMP_FLAG_SET,
		TIMER_SET( 0xc8 ),
		IF_HEIGHT( 0xff, 304 ),
			WAIT2( 14 ),
			FLY( LEVER_BACK, 512 ),
			CMD_JUMP( 18 ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_08_02[] = {
	LOCK_SET,
	WAIT2( 4 ),
	IF_RND( 15 ),
		DASH_F( 512 ),
		IF_LEVEL( 8 ),
			IF_EM_ATCK( ATCK_STAND_MP ),
				PUNCH( LV_LIGHT ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		SJUMP( DIR_UP ),
		SJUMP_FLAG_SET,
			TIMER_SET( 0xc8 ),
			IF_HEIGHT( 0xff, 512 ),
				TIMER_SET( 0xc8 ),
				IF_EM_ATCK( ATCK_JUMP_MP ),
					PUNCH( LV_LIGHT ),
				CMD_ENDIF,
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_08_03[] = {
	LOCK_SET,
	JUMP( DIR_FORWARD ),
	JUMP_FLAG_SET,
		TIMER_SET( 0xc8 ),
		IF_HEIGHT( 0, 72 ),
			IF_RND( 18 ),
				PUNCH( LV_LIGHT ),
			CMD_ELSE,
				IF_RND( 16 ),
					PUNCH( LV_HEAVY ),
				CMD_ELSE,
					KICK( LV_HEAVY ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok35_cmd_08_04[] = {
	CMD_EXIT,
};

const Uint8 yok35_cmd_08_05[] = {
	CMD_EXIT,
};

const Uint8 yok35_cmd_08_06[] = {
	CMD_EXIT,
};

const Uint8 yok35_cmd_08_07[] = {
	CMD_EXIT,
};

const Uint8 yok35_cmd_08_08[] = {
	CMD_EXIT,
};

const Uint8 yok35_cmd_08_09[] = {
	CMD_EXIT,
};

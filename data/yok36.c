/* Thanos (character 36): the computer's answers.
   What it does about an attack that is coming. PLAYER_WORK's cpu_answer_adrs points here;
   em_answer_sel picks a list from it.

   yok36_tbl        a record (CPU_DATA) for each answer. The row of the attack in its owner's hit
                    table (hit_dtXX.c) names the answer; YK_GD030 and YK_GD060 are the first two.
                    A record is the offsets of its sel (twice: no rate is read) and its cmd.
   yok36_sel_AA     32 picks; one is taken at random.
   yok36_cmd_AA     the answer's lists: a pick is a number in this table.
   yok36_cmd_AA_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name
                    a list of lib36.c. */
#include "player.h"
#include "pl36_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const yok36_cmd_00[], *const yok36_cmd_01[], *const yok36_cmd_02[],
	*const yok36_cmd_03[], *const yok36_cmd_04[], *const yok36_cmd_05[], *const yok36_cmd_06[],
	*const yok36_cmd_07[], *const yok36_cmd_08[];
extern const Uint8 yok36_sel_00[], yok36_sel_01[], yok36_sel_02[], yok36_sel_03[], yok36_sel_04[],
	yok36_sel_05[], yok36_sel_06[], yok36_sel_07[], yok36_sel_08[], yok36_cmd_00_00[],
	yok36_cmd_00_01[], yok36_cmd_00_02[], yok36_cmd_00_03[], yok36_cmd_00_04[], yok36_cmd_00_05[],
	yok36_cmd_00_06[], yok36_cmd_00_07[], yok36_cmd_00_08[], yok36_cmd_00_09[], yok36_cmd_01_00[],
	yok36_cmd_01_01[], yok36_cmd_01_02[], yok36_cmd_01_03[], yok36_cmd_01_04[], yok36_cmd_01_05[],
	yok36_cmd_01_06[], yok36_cmd_01_07[], yok36_cmd_01_08[], yok36_cmd_01_09[], yok36_cmd_02_00[],
	yok36_cmd_02_01[], yok36_cmd_02_02[], yok36_cmd_02_03[], yok36_cmd_02_04[], yok36_cmd_02_05[],
	yok36_cmd_02_06[], yok36_cmd_02_07[], yok36_cmd_02_08[], yok36_cmd_02_09[], yok36_cmd_03_00[],
	yok36_cmd_03_01[], yok36_cmd_03_02[], yok36_cmd_03_03[], yok36_cmd_03_04[], yok36_cmd_03_05[],
	yok36_cmd_03_06[], yok36_cmd_03_07[], yok36_cmd_03_08[], yok36_cmd_03_09[], yok36_cmd_04_00[],
	yok36_cmd_04_01[], yok36_cmd_04_02[], yok36_cmd_04_03[], yok36_cmd_04_04[], yok36_cmd_04_05[],
	yok36_cmd_04_06[], yok36_cmd_04_07[], yok36_cmd_04_08[], yok36_cmd_04_09[];

const CPU_DATA yok36_tbl[] = {
	{ ( Sint32 )yok36_sel_00, ( Sint32 )yok36_sel_00, ( Sint32 )yok36_cmd_00 },	/* YK_GD030 */
	{ ( Sint32 )yok36_sel_01, ( Sint32 )yok36_sel_01, ( Sint32 )yok36_cmd_01 },	/* YK_GD060 */
	{ ( Sint32 )yok36_sel_02, ( Sint32 )yok36_sel_02, ( Sint32 )yok36_cmd_02 },	/* 2 */
	{ ( Sint32 )yok36_sel_03, ( Sint32 )yok36_sel_03, ( Sint32 )yok36_cmd_03 },	/* 3 */
	{ ( Sint32 )yok36_sel_04, ( Sint32 )yok36_sel_04, ( Sint32 )yok36_cmd_04 },	/* 4 */
	{ ( Sint32 )yok36_sel_05, ( Sint32 )yok36_sel_05, ( Sint32 )yok36_cmd_05 },	/* 5 */
	{ ( Sint32 )yok36_sel_06, ( Sint32 )yok36_sel_06, ( Sint32 )yok36_cmd_06 },	/* 6 */
	{ ( Sint32 )yok36_sel_07, ( Sint32 )yok36_sel_07, ( Sint32 )yok36_cmd_07 },	/* 7 */
	{ ( Sint32 )yok36_sel_08, ( Sint32 )yok36_sel_08, ( Sint32 )yok36_cmd_08 },	/* 8 */
};

const Uint8 yok36_sel_00[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,
	  2,   2,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   0,   1,   1,   2,
};

const Uint8 *const yok36_cmd_00[] = {
	yok36_cmd_00_00, yok36_cmd_00_01, yok36_cmd_00_02, yok36_cmd_00_03, yok36_cmd_00_04, yok36_cmd_00_05,
	yok36_cmd_00_06, yok36_cmd_00_07, yok36_cmd_00_08, yok36_cmd_00_09,
};

const Uint8 yok36_sel_01[] = {
	  3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   1,   1,   1,   1,
	  2,   2,   2,   2,   2,   2,   2,   2,   0,   0,   0,   0,   0,   1,   3,   3,
};

const Uint8 *const yok36_cmd_01[] = {
	yok36_cmd_01_00, yok36_cmd_01_01, yok36_cmd_01_02, yok36_cmd_01_03, yok36_cmd_01_04, yok36_cmd_01_05,
	yok36_cmd_01_06, yok36_cmd_01_07, yok36_cmd_01_08, yok36_cmd_01_09,
};

const Uint8 yok36_sel_02[] = {
	  0,   0,   0,   0,   4,   4,   4,   4,   1,   1,   1,   1,   2,   2,   0,   0,
	  3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   4,   4,   4,   4,
};

const Uint8 *const yok36_cmd_02[] = {
	yok36_cmd_02_00, yok36_cmd_02_01, yok36_cmd_02_02, yok36_cmd_02_03, yok36_cmd_02_04, yok36_cmd_02_05,
	yok36_cmd_02_06, yok36_cmd_02_07, yok36_cmd_02_08, yok36_cmd_02_09,
};

const Uint8 yok36_sel_03[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   2,   2,   2,   2,
	  3,   3,   3,   3,   0,   0,   0,   0,   3,   3,   3,   3,   3,   3,   3,   3,
};

const Uint8 *const yok36_cmd_03[] = {
	yok36_cmd_03_00, yok36_cmd_03_01, yok36_cmd_03_02, yok36_cmd_03_03, yok36_cmd_03_04, yok36_cmd_03_05,
	yok36_cmd_03_06, yok36_cmd_03_07, yok36_cmd_03_08, yok36_cmd_03_09,
};

const Uint8 yok36_sel_04[] = {
	  7,   7,   7,   7,   3,   3,   0,   0,   7,   7,   7,   7,   3,   3,   0,   0,
	  7,   7,   7,   7,   3,   3,   0,   0,   7,   7,   7,   7,   3,   3,   0,   0,
};

const Uint8 *const yok36_cmd_04[] = {
	yok36_cmd_04_00, yok36_cmd_04_01, yok36_cmd_04_02, yok36_cmd_04_03, yok36_cmd_04_04, yok36_cmd_04_05,
	yok36_cmd_04_06, yok36_cmd_04_07, yok36_cmd_04_08, yok36_cmd_04_09,
};

const Uint8 yok36_sel_05[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const yok36_cmd_05[] = {
	yok36_cmd_04_09,
};

const Uint8 yok36_sel_06[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const yok36_cmd_06[] = {
	yok36_cmd_04_09,
};

const Uint8 yok36_sel_07[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const yok36_cmd_07[] = {
	yok36_cmd_04_09,
};

const Uint8 yok36_sel_08[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const yok36_cmd_08[] = {
	yok36_cmd_04_09,
};

const Uint8 yok36_cmd_00_00[] = {
	CMD_JUMP( 19 ),
	IF_ATCK_COME,
		GUARD( 30 ),
	CMD_ELSE,
		IF_RND( 8 ),
			JUMP_RND( 11, 6, 0, 1 ),
		CMD_ELSE,
			WAIT2( 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 yok36_cmd_00_01[] = {
	CMD_JUMP( 19 ),
	IF_ATCK_COME,
		CMD_JUMP( 23 ),
	CMD_ELSE,
		IF_RND( 8 ),
			JUMP_RND( 11, 6, 0, 1 ),
		CMD_ELSE,
			CMD_JUMP( 23 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 yok36_cmd_00_02[] = {
	CMD_JUMP( 19 ),
	IF_ATCK_COME,
		GUARD2( 30 ),
	CMD_ELSE,
		IF_RND( 8 ),
			JUMP_RND( 11, 6, 0, 1 ),
		CMD_ELSE,
			WAIT2( 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 yok36_cmd_00_03[] = {
	CMD_JUMP( 19 ),
	TIMER2_SET( 120 ),
	DASH_B( 240 ),
	CMD_EXIT,
};

const Uint8 yok36_cmd_00_04[] = {
	CMD_EXIT,
};

const Uint8 yok36_cmd_00_05[] = {
	CMD_EXIT,
};

const Uint8 yok36_cmd_00_06[] = {
	CMD_EXIT,
};

const Uint8 yok36_cmd_00_07[] = {
	CMD_EXIT,
};

const Uint8 yok36_cmd_00_08[] = {
	CMD_EXIT,
};

const Uint8 yok36_cmd_00_09[] = {
	CMD_EXIT,
};

const Uint8 yok36_cmd_01_00[] = {
	CMD_JUMP( 19 ),
	SJUMP( DIR_FORWARD ),
	SJUMP_FLAG_SET,
		TIMER2_SET( 0xb4 ),
		WAIT( 0xb4 ),
		JUMP_FLAG_CLR,
	CMD_ELSE,
		GUARD( 30 ),
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 yok36_cmd_01_01[] = {
	CMD_JUMP( 19 ),
	TIMER_SET( 60 ),
	IF_AREA2( 64, 64, 64, 64 ),
		JUMP( DIR_FORWARD ),
		JUMP_FLAG_SET,
			TIMER2_SET( 60 ),
			WAIT( 60 ),
			JUMP_FLAG_CLR,
		CMD_ELSE,
			GUARD( 60 ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 yok36_cmd_01_02[] = {
	CMD_JUMP( 19 ),
	GUARD( 90 ),
	CMD_EXIT,
};

const Uint8 yok36_cmd_01_03[] = {
	CMD_JUMP( 19 ),
	IF_AREA2( 32, 32, 32, 32 ),
		WAIT2( 12 ),
		GUARD( 60 ),
	CMD_ELSE,
		JUMP( DIR_FORWARD ),
		JUMP_FLAG_SET,
			IF_AREA( 64, 64, 64, -32 ),
				NAG_VFLIP_ADJ4( LV_HEAVY, LEVER_DOWN ),
			CMD_ELSE,
				GUARD( 30 ),
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 yok36_cmd_01_04[] = {
	CMD_EXIT,
};

const Uint8 yok36_cmd_01_05[] = {
	CMD_EXIT,
};

const Uint8 yok36_cmd_01_06[] = {
	CMD_EXIT,
};

const Uint8 yok36_cmd_01_07[] = {
	CMD_EXIT,
};

const Uint8 yok36_cmd_01_08[] = {
	CMD_EXIT,
};

const Uint8 yok36_cmd_01_09[] = {
	CMD_EXIT,
};

const Uint8 yok36_cmd_02_00[] = {
	CMD_JUMP( 19 ),
	JUMP( DIR_FORWARD ),
	JUMP_FLAG_SET,
		TIMER2_SET( 0xb4 ),
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	TIMER2_SET( 1 ),
	CMD_EXIT,
};

const Uint8 yok36_cmd_02_01[] = {
	CMD_JUMP( 19 ),
	GUARD( 60 ),
	CMD_EXIT,
};

const Uint8 yok36_cmd_02_02[] = {
	CMD_JUMP( 19 ),
	GUARD( 120 ),
	CMD_EXIT,
};

const Uint8 yok36_cmd_02_03[] = {
	CMD_JUMP( 19 ),
	JUMP( DIR_FORWARD ),
	JUMP_FLAG_SET,
		TIMER2_SET( 0xb4 ),
		TIMER_SET( 60 ),
		IF_EM_ATCK( ATCK_JUMP_LP ),
			CANCEL_SET2,
				PUNCH( LV_LIGHT ),
				PUNCH( LV_HEAVY ),
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	TIMER2_SET( 1 ),
	CMD_EXIT,
};

const Uint8 yok36_cmd_02_04[] = {
	CMD_JUMP( 19 ),
	SJUMP( DIR_FORWARD ),
	SJUMP_FLAG_SET,
		TIMER2_SET( 0xb4 ),
		WAIT( 0xb4 ),
		JUMP_FLAG_CLR,
	CMD_ELSE,
		GUARD( 30 ),
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 yok36_cmd_02_05[] = {
	CMD_EXIT,
};

const Uint8 yok36_cmd_02_06[] = {
	CMD_EXIT,
};

const Uint8 yok36_cmd_02_07[] = {
	CMD_EXIT,
};

const Uint8 yok36_cmd_02_08[] = {
	CMD_EXIT,
};

const Uint8 yok36_cmd_02_09[] = {
	CMD_EXIT,
};

const Uint8 yok36_cmd_03_00[] = {
	CMD_JUMP( 19 ),
	IF_ATCK_COME,
		GUARD( 30 ),
	CMD_ELSE,
		IF_RND( 16 ),
			JUMP_RND( 11, 6, 0, 1 ),
		CMD_ELSE,
			WAIT2( 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 yok36_cmd_03_01[] = {
	CMD_JUMP( 19 ),
	IF_ATCK_COME,
		GUARD( 60 ),
	CMD_ELSE,
		IF_RND( 16 ),
			JUMP_RND( 11, 6, 0, 1 ),
		CMD_ELSE,
			WAIT2( 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 yok36_cmd_03_02[] = {
	CMD_JUMP( 19 ),
	IF_ATCK_COME,
		GUARD( 120 ),
	CMD_ELSE,
		IF_RND( 16 ),
			JUMP_RND( 11, 6, 0, 1 ),
		CMD_ELSE,
			WAIT2( 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 yok36_cmd_03_03[] = {
	CMD_JUMP( 19 ),
	SP_CMD( pl36_sp_sphere, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_EXIT,
};

const Uint8 yok36_cmd_03_04[] = {
	CMD_EXIT,
};

const Uint8 yok36_cmd_03_05[] = {
	CMD_EXIT,
};

const Uint8 yok36_cmd_03_06[] = {
	CMD_EXIT,
};

const Uint8 yok36_cmd_03_07[] = {
	CMD_EXIT,
};

const Uint8 yok36_cmd_03_08[] = {
	CMD_EXIT,
};

const Uint8 yok36_cmd_03_09[] = {
	CMD_EXIT,
};

const Uint8 yok36_cmd_04_00[] = {
	TIMER_SET( 30 ),
	IF_AREA2( 128, 128, 0, 0 ),
		CMD_JUMP( 19 ),
		GUARD2( 30 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 yok36_cmd_04_01[] = {
	CMD_JUMP( 19 ),
	CMD_JUMP( 23 ),
	CMD_EXIT,
};

const Uint8 yok36_cmd_04_02[] = {
	TIMER_SET( 30 ),
	IF_AREA2( 128, 128, 0, 0 ),
		CMD_JUMP( 19 ),
		GUARD2( 120 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 yok36_cmd_04_03[] = {
	CMD_JUMP( 19 ),
	TIMER_SET( 20 ),
	IF_AREA2( 32, 64, 32, 64 ),
		GUARD2( 30 ),
	CMD_ELSE,
		WAIT2( 12 ),
		SP_CMD( pl36_sp_titan, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 yok36_cmd_04_04[] = {
	CMD_JUMP( 19 ),
	TIMER_SET( 20 ),
	IF_AREA2( 32, 64, 32, 64 ),
		GUARD2( 30 ),
	CMD_ELSE,
		WAIT2( 12 ),
		JUMP_RND( 0, 1, 0, 1 ),
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 yok36_cmd_04_05[] = {
	CMD_JUMP( 19 ),
	SJUMP( DIR_UP ),
	SJUMP_FLAG_SET,
		TIMER_SET( 120 ),
		IF_DIST( 128, 32, 128, 0 ),
			WAIT2( 8 ),
			SP_CMD( pl36_sp_titan, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ELSE,
			CMD_JUMP( 23 ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 yok36_cmd_04_06[] = {
	CMD_JUMP( 19 ),
	JUMP( DIR_UP ),
	JUMP_FLAG_SET,
		TIMER_SET( 120 ),
		IF_DIST( 128, 32, 128, 0 ),
			WAIT2( 8 ),
			SP_CMD( pl36_sp_titan, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ELSE,
			GUARD2( 30 ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 yok36_cmd_04_07[] = {
	LOCK_SET,
	CMD_JUMP( 24 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok36_cmd_04_08[] = {
	CMD_EXIT,
};

const Uint8 yok36_cmd_04_09[] = {
	CMD_EXIT,
};

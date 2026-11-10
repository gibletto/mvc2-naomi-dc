/* Spiderman (character 0c): the computer's answers.
   What it does about an attack that is coming. PLAYER_WORK's cpu_answer_adrs points here;
   em_answer_sel picks a list from it.

   yok0c_tbl        a record (CPU_DATA) for each answer. The row of the attack in its owner's hit
                    table (hit_dtXX.c) names the answer; YK_GD030 and YK_GD060 are the first two.
                    A record is the offsets of its sel (twice: no rate is read) and its cmd.
   yok0c_sel_AA     32 picks; one is taken at random.
   yok0c_cmd_AA     the answer's lists: a pick is a number in this table.
   yok0c_cmd_AA_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name
                    a list of lib0c.c. */
#include "player.h"
#include "pl0c_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const yok0c_cmd_00[], *const yok0c_cmd_01[], *const yok0c_cmd_02[],
	*const yok0c_cmd_03[], *const yok0c_cmd_04[], *const yok0c_cmd_05[], *const yok0c_cmd_06[],
	*const yok0c_cmd_07[], *const yok0c_cmd_08[];
extern const Uint8 yok0c_sel_00[], yok0c_sel_01[], yok0c_sel_02[], yok0c_sel_03[], yok0c_sel_04[],
	yok0c_sel_05[], yok0c_sel_06[], yok0c_sel_07[], yok0c_sel_08[], yok0c_cmd_00_00[],
	yok0c_cmd_00_01[], yok0c_cmd_00_02[], yok0c_cmd_00_03[], yok0c_cmd_00_04[], yok0c_cmd_00_05[],
	yok0c_cmd_01_00[], yok0c_cmd_01_01[], yok0c_cmd_01_02[], yok0c_cmd_01_03[], yok0c_cmd_01_04[],
	yok0c_cmd_01_05[], yok0c_cmd_02_00[], yok0c_cmd_02_01[], yok0c_cmd_02_02[], yok0c_cmd_02_03[],
	yok0c_cmd_02_04[], yok0c_cmd_02_05[], yok0c_cmd_03_00[], yok0c_cmd_03_01[], yok0c_cmd_03_02[],
	yok0c_cmd_03_03[], yok0c_cmd_03_04[], yok0c_cmd_03_05[], yok0c_cmd_04_00[], yok0c_cmd_04_01[],
	yok0c_cmd_04_02[], yok0c_cmd_04_03[], yok0c_cmd_04_04[], yok0c_cmd_04_05[], yok0c_cmd_05_00[],
	yok0c_cmd_05_01[], yok0c_cmd_05_02[], yok0c_cmd_05_03[], yok0c_cmd_06_00[], yok0c_cmd_06_01[],
	yok0c_cmd_06_02[], yok0c_cmd_06_03[], yok0c_cmd_06_04[], yok0c_cmd_06_05[], yok0c_cmd_07_00[],
	yok0c_cmd_07_01[], yok0c_cmd_07_02[], yok0c_cmd_07_03[], yok0c_cmd_07_04[], yok0c_cmd_08_00[],
	yok0c_cmd_08_01[], yok0c_cmd_08_02[];

const CPU_DATA yok0c_tbl[] = {
	{ ( Sint32 )yok0c_sel_00, ( Sint32 )yok0c_sel_00, ( Sint32 )yok0c_cmd_00 },	/* YK_GD030 */
	{ ( Sint32 )yok0c_sel_01, ( Sint32 )yok0c_sel_01, ( Sint32 )yok0c_cmd_01 },	/* YK_GD060 */
	{ ( Sint32 )yok0c_sel_02, ( Sint32 )yok0c_sel_02, ( Sint32 )yok0c_cmd_02 },	/* 2 */
	{ ( Sint32 )yok0c_sel_03, ( Sint32 )yok0c_sel_03, ( Sint32 )yok0c_cmd_03 },	/* 3 */
	{ ( Sint32 )yok0c_sel_04, ( Sint32 )yok0c_sel_04, ( Sint32 )yok0c_cmd_04 },	/* 4 */
	{ ( Sint32 )yok0c_sel_05, ( Sint32 )yok0c_sel_05, ( Sint32 )yok0c_cmd_05 },	/* 5 */
	{ ( Sint32 )yok0c_sel_06, ( Sint32 )yok0c_sel_06, ( Sint32 )yok0c_cmd_06 },	/* 6 */
	{ ( Sint32 )yok0c_sel_07, ( Sint32 )yok0c_sel_07, ( Sint32 )yok0c_cmd_07 },	/* 7 */
	{ ( Sint32 )yok0c_sel_08, ( Sint32 )yok0c_sel_08, ( Sint32 )yok0c_cmd_08 },	/* 8 */
};

const Uint8 yok0c_sel_00[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
	  1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   3,   4,
};

const Uint8 *const yok0c_cmd_00[] = {
	yok0c_cmd_00_00, yok0c_cmd_00_01, yok0c_cmd_00_02, yok0c_cmd_00_03, yok0c_cmd_00_04, yok0c_cmd_00_05,
};

const Uint8 yok0c_sel_01[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,
	  1,   1,   2,   2,   2,   2,   2,   3,   3,   3,   4,   4,   4,   4,   5,   5,
};

const Uint8 *const yok0c_cmd_01[] = {
	yok0c_cmd_01_00, yok0c_cmd_01_01, yok0c_cmd_01_02, yok0c_cmd_01_03, yok0c_cmd_01_04, yok0c_cmd_01_05,
};

const Uint8 yok0c_sel_02[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   2,
	  2,   2,   2,   2,   2,   2,   2,   2,   3,   3,   4,   4,   4,   5,   5,   5,
};

const Uint8 *const yok0c_cmd_02[] = {
	yok0c_cmd_02_00, yok0c_cmd_02_01, yok0c_cmd_02_02, yok0c_cmd_02_03, yok0c_cmd_02_04, yok0c_cmd_02_05,
};

const Uint8 yok0c_sel_03[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   1,   1,   1,   1,   1,   1,   2,   2,   3,   3,   4,   4,   5,   5,
};

const Uint8 *const yok0c_cmd_03[] = {
	yok0c_cmd_03_00, yok0c_cmd_03_01, yok0c_cmd_03_02, yok0c_cmd_03_03, yok0c_cmd_03_04, yok0c_cmd_03_05,
};

const Uint8 yok0c_sel_04[] = {
	  0,   0,   0,   0,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
	  3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   4,   4,   4,   5,   5,   5,
};

const Uint8 *const yok0c_cmd_04[] = {
	yok0c_cmd_04_00, yok0c_cmd_04_01, yok0c_cmd_04_02, yok0c_cmd_04_03, yok0c_cmd_04_04, yok0c_cmd_04_05,
};

const Uint8 yok0c_sel_05[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2,
	  1,   1,   1,   1,   1,   1,   1,   2,   1,   1,   1,   1,   1,   1,   1,   2,
};

const Uint8 *const yok0c_cmd_05[] = {
	yok0c_cmd_05_00, yok0c_cmd_05_01, yok0c_cmd_05_02, yok0c_cmd_05_03, yok0c_cmd_05_03, yok0c_cmd_05_03,
};

const Uint8 yok0c_sel_06[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,
	  2,   2,   2,   3,   3,   3,   3,   3,   3,   4,   4,   4,   4,   5,   5,   5,
};

const Uint8 *const yok0c_cmd_06[] = {
	yok0c_cmd_06_00, yok0c_cmd_06_01, yok0c_cmd_06_02, yok0c_cmd_06_03, yok0c_cmd_06_04, yok0c_cmd_06_05,
};

const Uint8 yok0c_sel_07[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   2,   2,   3,   3,
};

const Uint8 *const yok0c_cmd_07[] = {
	yok0c_cmd_07_00, yok0c_cmd_07_01, yok0c_cmd_07_02, yok0c_cmd_07_03, yok0c_cmd_07_04, yok0c_cmd_07_04,
};

const Uint8 yok0c_sel_08[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,
};

const Uint8 *const yok0c_cmd_08[] = {
	yok0c_cmd_08_00, yok0c_cmd_08_01, yok0c_cmd_08_02, yok0c_cmd_08_02, yok0c_cmd_08_02, yok0c_cmd_08_02,
};

const Uint8 yok0c_cmd_00_00[] = {
	LOCK_SET,
	IF_AREA2( 96, 80, 0, 48 ),
		JUMP_LEVEL( 48, 7, 49, 13, 50, 21, 51 ),
	CMD_ELSE,
		WAIT2( 2 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0c_cmd_00_01[] = {
	LOCK_SET,
	IF_ATCK_COME,
		IF_LEVEL( 1 ),
			IF_RND( 24 ),
				GUARD( 30 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 21 ),
				IF_RND( 22 ),
					GUARD( 30 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 13 ),
					IF_RND( 20 ),
						GUARD( 30 ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_LEVEL( 7 ),
						IF_RND( 18 ),
							GUARD( 30 ),
						CMD_ENDIF,
					CMD_ELSE,
						IF_RND( 12 ),
							GUARD( 30 ),
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		WAIT( 10 ),
		WAIT2( 6 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0c_cmd_00_02[] = {
	LOCK_SET,
	IF_AREA2( 64, 80, 0, 48 ),
		JUMP( DIR_UP ),
		JUMP_FLAG_SET,
			JUMP_LEVEL( 48, 7, 49, 13, 50, 21, 51 ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		WAIT( 10 ),
		WAIT2( 6 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0c_cmd_00_03[] = {
	LOCK_SET,
	IF_LEVEL( 24 ),
		IF_RND( 24 ),
			CMD_JUMP( 41 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 21 ),
			IF_RND( 22 ),
				CMD_JUMP( 41 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 13 ),
				IF_RND( 20 ),
					CMD_JUMP( 41 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 7 ),
					IF_RND( 18 ),
						CMD_JUMP( 41 ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_RND( 12 ),
						CMD_JUMP( 41 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0c_cmd_00_04[] = {
	LOCK_SET,
	IF_AREA2( 144, 80, 0, 48 ),
		SJUMP( DIR_FORWARD ),
		SJUMP_FLAG_SET,
			JUMP_LEVEL( 42, 8, 43, 16, 44, 24, 45 ),
			JUMP_LEVEL( 70, 8, 71, 16, 72, 24, 73 ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0c_cmd_00_05[] = {
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 yok0c_cmd_01_00[] = {
	LOCK_SET,
	IF_AREA2( 144, 208, 0, 0 ),
		CMD_JUMP( 87 ),
	CMD_ELSE,
		WAIT2( 2 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0c_cmd_01_01[] = {
	LOCK_SET,
	IF_AREA2( 88, 144, 0, 0 ),
		JUMP( DIR_UP ),
		JUMP_FLAG_SET,
			JUMP_LEVEL( 48, 7, 49, 13, 50, 21, 51 ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		WAIT2( 2 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0c_cmd_01_02[] = {
	LOCK_SET,
	IF_ATCK_COME,
		CMD_JUMP( 87 ),
	CMD_ELSE,
		WAIT2( 2 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0c_cmd_01_03[] = {
	LOCK_SET,
	IF_LEVEL( 8 ),
		IF_RND( 20 ),
			IF_AREA2( 96, 80, 64, 48 ),
				DASH_B( 192 ),
				WAIT( 16 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 yok0c_cmd_01_04[] = {
	LOCK_SET,
	CMD_JUMP( 41 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0c_cmd_01_05[] = {
	LOCK_SET,
	SJUMP( DIR_FORWARD ),
	SJUMP_FLAG_SET,
		JUMP_LEVEL( 42, 8, 43, 16, 44, 24, 45 ),
		JUMP_LEVEL( 70, 8, 71, 16, 72, 24, 73 ),
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0c_cmd_02_00[] = {
	LOCK_SET,
	IF_AREA2( 144, 208, 0, 0 ),
		CMD_JUMP( 88 ),
	CMD_ELSE,
		WAIT2( 2 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0c_cmd_02_01[] = {
	LOCK_SET,
	IF_AREA2( 88, 144, 0, 0 ),
		JUMP( DIR_UP ),
		JUMP_FLAG_SET,
			JUMP_LEVEL( 48, 7, 49, 13, 50, 21, 51 ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		WAIT2( 2 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0c_cmd_02_02[] = {
	VCOUNTER_SET( 4 ),
	LOCK_SET,
	IF_ATCK_COME,
		CMD_JUMP( 88 ),
	CMD_ELSE,
		WAIT2( 2 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0c_cmd_02_03[] = {
	LOCK_SET,
	IF_LEVEL( 8 ),
		IF_RND( 20 ),
			IF_AREA2( 160, 240, 0, 0 ),
				DASH_B( 128 ),
				WAIT( 16 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 yok0c_cmd_02_04[] = {
	LOCK_SET,
	CMD_JUMP( 41 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0c_cmd_02_05[] = {
	LOCK_SET,
	SJUMP( DIR_FORWARD ),
	SJUMP_FLAG_SET,
		JUMP_LEVEL( 42, 8, 43, 16, 44, 24, 45 ),
		JUMP_LEVEL( 70, 8, 71, 16, 72, 24, 73 ),
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0c_cmd_03_00[] = {
	LOCK_SET,
	IF_AREA2( 80, 112, 0, 0 ),
		IF_LEVEL( 16 ),
			IF_RND( 12 ),
				GUARD( 30 ),
			CMD_ENDIF,
		CMD_ELSE,
			WAIT( 30 ),
		CMD_ENDIF,
	CMD_ELSE,
		WAIT( 30 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0c_cmd_03_01[] = {
	LOCK_SET,
	IF_AREA2( 80, 112, 0, 0 ),
		GUARD( 30 ),
	CMD_ELSE,
		WAIT2( 2 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0c_cmd_03_02[] = {
	LOCK_SET,
	IF_AREA2( 56, 64, 48, 64 ),
		JUMP( DIR_UP ),
		JUMP_FLAG_SET,
			JUMP_LEVEL( 4, 8, 5, 16, 6, 24, 7 ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0c_cmd_03_03[] = {
	LOCK_SET,
	IF_AREA2( 160, 96, 0, 64 ),
		GUARD( 60 ),
	CMD_ENDIF,
	LOCK_CLR,
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 yok0c_cmd_03_04[] = {
	LOCK_SET,
	IF_AREA2( 160, 96, 0, 64 ),
		GUARD( 120 ),
	CMD_ENDIF,
	LOCK_CLR,
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 yok0c_cmd_03_05[] = {
	LOCK_SET,
	IF_AREA2( 144, 64, 0, 48 ),
		TIMER2_SET( 30 ),
		SJUMP( DIR_FORWARD ),
		JUMP_LEVEL( 42, 8, 43, 16, 44, 24, 45 ),
		JUMP_LEVEL( 70, 8, 71, 16, 72, 24, 73 ),
	CMD_ENDIF,
	STAND,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0c_cmd_04_00[] = {
	LOCK_SET,
	IF_AREA2( 80, 72, 0, 48 ),
		GUARD( 30 ),
	CMD_ELSE,
		WAIT2( 2 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0c_cmd_04_01[] = {
	LOCK_SET,
	TIMER_SET( 8 ),
	IF_AREA2( 112, 80, 48, 48 ),
		IF_LEVEL( 31 ),
			GUARD( 30 ),
			CMD_JUMP( 82 ),
		CMD_ELSE,
			IF_LEVEL( 16 ),
				IF_RND( 29 ),
					GUARD( 30 ),
					CMD_JUMP( 82 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_RND( 27 ),
					GUARD( 30 ),
					CMD_JUMP( 82 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		WAIT2( 2 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0c_cmd_04_02[] = {
	LOCK_SET,
	TIMER_SET( 30 ),
	IF_AREA2( 176, 48, 160, 48 ),
		IF_AREA2( 48, 48, 32, 48 ),
			GUARD( 60 ),
		CMD_ELSE,
			TIMER_SET( 40 ),
			IF_AREA2( 48, 48, 32, 48 ),
				JUMP( DIR_UP ),
				JUMP_FLAG_SET,
					JUMP_LEVEL( 70, 8, 71, 16, 72, 24, 73 ),
					JUMP_FLAG_CLR,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	STAND,
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 yok0c_cmd_04_03[] = {
	LOCK_SET,
	TIMER_SET( 30 ),
	IF_AREA2( 176, 48, 160, 48 ),
		IF_AREA2( 48, 48, 32, 48 ),
			GUARD( 60 ),
		CMD_ELSE,
			TIMER_SET( 40 ),
			IF_AREA2( 64, 48, 48, 48 ),
				JUMP( DIR_FORWARD ),
				JUMP_FLAG_SET,
					JUMP_LEVEL( 4, 8, 5, 16, 6, 24, 7 ),
					JUMP_FLAG_CLR,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	LOCK_CLR,
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 yok0c_cmd_04_04[] = {
	LOCK_SET,
	CMD_JUMP( 47 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0c_cmd_04_05[] = {
	LOCK_SET,
	IF_AREA2( 176, 48, 160, 48 ),
		IF_AREA2( 80, 48, 64, 48 ),
			SJUMP( DIR_FORWARD ),
			SJUMP_FLAG_SET,
				JUMP_LEVEL( 70, 8, 71, 16, 72, 24, 73 ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0c_cmd_05_00[] = {
	LOCK_SET,
	IF_AREA2( 32, 56, 80, 72 ),
		CMD_JUMP( 87 ),
	CMD_ELSE,
		IF_RND( 12 ),
			DASH_B( 192 ),
			WAIT( 16 ),
			STAND,
		CMD_ELSE,
			JUMP( DIR_BACK ),
			JUMP_FLAG_SET,
				JUMP_LEVEL( 4, 8, 5, 16, 6, 24, 7 ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
			WAIT2( 4 ),
			STAND,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0c_cmd_05_01[] = {
	LOCK_SET,
	IF_AREA2( 32, 56, 80, 72 ),
		CMD_JUMP( 87 ),
	CMD_ELSE,
		DASH_EM( 48 ),
		IF_EM_ATCK( ATCK_STAND_MP ),
			PUNCH( LV_LIGHT ),
			IF_LEVEL( 3 ),
				IF_RND( 16 ),
					JUMP_LEVEL( 64, 8, 65, 16, 66, 24, 67 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT2( 2 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0c_cmd_05_02[] = {
	LOCK_SET,
	IF_AREA2( 32, 56, 80, 72 ),
		CMD_JUMP( 87 ),
	CMD_ELSE,
		IF_LEVEL( 2 ),
			IF_RND( 3 ),
				IF_GAUGE( 1 ),
					SP_CMD( pl0c_sp_maximum, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ENDIF,
			CMD_ELSE,
				SJUMP( DIR_UP ),
				SJUMP_FLAG_SET,
					JUMP_LEVEL( 42, 8, 43, 16, 44, 24, 45 ),
					JUMP_LEVEL( 70, 8, 71, 16, 72, 24, 73 ),
					JUMP_FLAG_CLR,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			SJUMP( DIR_UP ),
		CMD_ENDIF,
		STAND,
		WAIT2( 4 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0c_cmd_05_03[] = {
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 yok0c_cmd_06_00[] = {
	LOCK_SET,
	IF_AREA2( 96, 64, 64, 64 ),
		CMD_JUMP( 87 ),
	CMD_ELSE,
		WAIT2( 2 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0c_cmd_06_01[] = {
	LOCK_SET,
	CROUCH,
	TIMER_SET( 60 ),
	IF_ATCK_COME,
		GUARD( 30 ),
	CMD_ELSE,
		IF_LEVEL( 18 ),
			KICK( LV_HEAVY ),
			STAND,
		CMD_ELSE,
			WAIT2( 6 ),
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0c_cmd_06_02[] = {
	LOCK_SET,
	TIMER_SET( 30 ),
	IF_AREA2( 80, 64, 48, 64 ),
		JUMP( DIR_UP ),
		JUMP_FLAG_SET,
			JUMP_LEVEL( 48, 7, 49, 13, 50, 21, 51 ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		WAIT2( 2 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0c_cmd_06_03[] = {
	LOCK_SET,
	IF_LEVEL( 1 ),
		IF_RND( 28 ),
			CMD_JUMP( 82 ),
			WAIT2( 4 ),
			IF_LEVEL( 5 ),
				IF_RND( 8 ),
					JUMP_LEVEL( 19, 8, 20, 16, 21, 24, 22 ),
				CMD_ELSE,
					JUMP_LEVEL( 23, 8, 24, 16, 25, 24, 26 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_RND( 10 ),
					CANCEL_SET2,
						PUNCH( LV_LIGHT ),
						SP_CMD( pl0c_sp_sting, 0, PK_PUNCH, LV_LIGHT, 0 ),
						CANCEL_CLR,
					CMD_ENDIF,
				CMD_ELSE,
					CMD_JUMP( 39 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		WAIT2( 2 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0c_cmd_06_04[] = {
	LOCK_SET,
	IF_AREA2( 80, 48, 64, 48 ),
		SJUMP( DIR_FORWARD ),
		SJUMP_FLAG_SET,
			JUMP_LEVEL( 42, 8, 43, 16, 44, 24, 45 ),
			JUMP_LEVEL( 70, 8, 71, 16, 72, 24, 73 ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0c_cmd_06_05[] = {
	LOCK_SET,
	TIMER_SET( 10 ),
	IF_AREA2( 128, 48, 64, 48 ),
		IF_AREA2( 48, 48, 32, 48 ),
			GUARD( 30 ),
		CMD_ELSE,
			JUMP( DIR_FORWARD ),
			JUMP_FLAG_SET,
				JUMP_LEVEL( 4, 8, 5, 16, 6, 24, 7 ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
			STAND,
			WAIT2( 4 ),
			DASH_B( 192 ),
		CMD_ENDIF,
	CMD_ELSE,
		JUMP( DIR_BACK ),
		STAND,
		WAIT2( 4 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0c_cmd_07_00[] = {
	LOCK_SET,
	IF_AREA2( 64, 64, 16, 64 ),
		IF_LEVEL( 2 ),
			IF_RND( 12 ),
				GUARD( 60 ),
			CMD_ENDIF,
		CMD_ELSE,
			WAIT( 16 ),
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0c_cmd_07_01[] = {
	LOCK_SET,
	IF_RND( 16 ),
		IF_AREA2( 64, 64, 16, 64 ),
			GUARD( 30 ),
		CMD_ENDIF,
		TIMER_SET( 48 ),
		IF_EM_ATCK( ATCK_CROUCH_HK ),
			CROUCH,
			KICK( LV_HEAVY ),
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0c_cmd_07_02[] = {
	LOCK_SET,
	IF_RND( 16 ),
		IF_AREA2( 64, 64, 16, 64 ),
			GUARD( 30 ),
		CMD_ENDIF,
		TIMER_SET( 60 ),
		IF_EM_STANCE( STANCE_STAND ),
			SP_CMD( pl0c_sp_sting, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0c_cmd_07_03[] = {
	LOCK_SET,
	IF_RND( 26 ),
		GUARD_BACK_SET( 1 ),
		IF_AREA2( 64, 64, 16, 64 ),
			GUARD( 30 ),
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0c_cmd_07_04[] = {
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 yok0c_cmd_08_00[] = {
	LOCK_SET,
	IF_EM_STANCE( STANCE_AIR ),
		GUARD( 30 ),
		SJUMP( DIR_FORWARD ),
		SJUMP_FLAG_SET,
			JUMP_LEVEL( 42, 8, 43, 16, 44, 24, 45 ),
			JUMP_LEVEL( 70, 8, 71, 16, 72, 24, 73 ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
		STAND,
		WAIT2( 4 ),
	CMD_ELSE,
		GUARD( 30 ),
		TIMER_SET( 1 ),
		IF_DIST( 144, 32, 80, 16 ),
			TIMER_SET( 1 ),
			IF_DIST( 80, 32, 32, 16 ),
				IF_LEVEL( 2 ),
					IF_CATCH,
						NAG_VFLIP_ADJ( 0, LEVER_BACK ),
					CMD_ELSE,
						IF_RND( 10 ),
							JUMP_LEVEL( 19, 8, 20, 16, 21, 24, 22 ),
						CMD_ELSE,
							JUMP_LEVEL( 23, 8, 24, 16, 25, 24, 26 ),
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ELSE,
					WAIT2( 2 ),
				CMD_ENDIF,
			CMD_ELSE,
				WAIT2( 2 ),
				CMD_DUMMY,
				JUMP( DIR_FORWARD ),
				JUMP_FLAG_SET,
					JUMP_LEVEL( 4, 8, 5, 16, 6, 24, 7 ),
					JUMP_FLAG_CLR,
				CMD_ENDIF,
				JUMP_LEVEL( 19, 8, 20, 16, 21, 24, 22 ),
				WAIT2( 4 ),
				DASH_B( 192 ),
			CMD_ENDIF,
		CMD_ELSE,
			WAIT2( 2 ),
			SJUMP( DIR_FORWARD ),
			SJUMP_FLAG_SET,
				JUMP_LEVEL( 42, 8, 43, 16, 44, 24, 45 ),
				JUMP_LEVEL( 70, 8, 71, 16, 72, 24, 73 ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0c_cmd_08_01[] = {
	LOCK_SET,
	IF_EM_STANCE( STANCE_AIR ),
		IF_LEVEL( 2 ),
			IF_DIST( 144, 32, 96, 0 ),
				IF_RND( 16 ),
					GUARD( 40 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_DIST( 144, 32, 96, 64 ),
					IF_DIST( 80, 32, 32, 64 ),
						DASH_EM( 72 ),
						PUNCH( LV_LIGHT ),
						IF_LEVEL( 3 ),
							IF_RND( 16 ),
								WAIT( 2 ),
								SJUMP3( DIR_UP ),
								JUMP_LEVEL( 65, 5, 66, 12, 68, 22, 69 ),
							CMD_ENDIF,
						CMD_ELSE,
							WAIT2( 4 ),
							JUMP( DIR_BACK ),
							JUMP_LEVEL( 48, 8, 49, 16, 50, 24, 51 ),
							STAND,
						CMD_ENDIF,
					CMD_ELSE,
						WAIT2( 4 ),
						SP_CMD( pl0c_sp_webball, 0, PK_PUNCH, LV_LIGHT, 0 ),
					CMD_ENDIF,
				CMD_ELSE,
					DASH_F( 192 ),
					WAIT( 16 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			IF_RND( 10 ),
				GUARD_BACK_SET( 1 ),
				GUARD( 40 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		GUARD2( 30 ),
		IF_DIST( 144, 32, 96, 0 ),
			IF_DIST( 112, 32, 64, 0 ),
				IF_EM_ATCK( ATCK_STAND_LP ),
					JUMP_LEVEL( 33, 8, 34, 16, 35, 24, 36 ),
				CMD_ELSE,
					IF_RND( 12 ),
						JUMP_LEVEL( 23, 8, 24, 16, 25, 24, 26 ),
					CMD_ELSE,
						JUMP_LEVEL( 23, 8, 24, 16, 25, 24, 26 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ELSE,
				CMD_DUMMY,
				JUMP( DIR_FORWARD ),
				JUMP_FLAG_SET,
					JUMP_LEVEL( 4, 8, 5, 16, 6, 24, 7 ),
					JUMP_FLAG_CLR,
				CMD_ENDIF,
				JUMP_LEVEL( 19, 8, 20, 16, 21, 24, 22 ),
				WAIT2( 4 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_RND( 16 ),
				SJUMP( DIR_UP ),
				SJUMP_FLAG_SET,
					JUMP_LEVEL( 42, 8, 43, 16, 44, 24, 45 ),
					JUMP_LEVEL( 70, 8, 71, 16, 72, 24, 73 ),
					JUMP_FLAG_CLR,
				CMD_ENDIF,
			CMD_ELSE,
				WAIT2( 2 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0c_cmd_08_02[] = {
	WAIT2( 2 ),
	CMD_EXIT,
};

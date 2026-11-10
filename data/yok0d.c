/* Hulk (character 0d): the computer's answers.
   What it does about an attack that is coming. PLAYER_WORK's cpu_answer_adrs points here;
   em_answer_sel picks a list from it.

   yok0d_tbl        a record (CPU_DATA) for each answer. The row of the attack in its owner's hit
                    table (hit_dtXX.c) names the answer; YK_GD030 and YK_GD060 are the first two.
                    A record is the offsets of its sel (twice: no rate is read) and its cmd.
   yok0d_sel_AA     32 picks; one is taken at random.
   yok0d_cmd_AA     the answer's lists: a pick is a number in this table.
   yok0d_cmd_AA_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name
                    a list of lib0d.c. */
#include "player.h"
#include "cpu_cmd.h"

extern const Uint8 *const yok0d_cmd_00[], *const yok0d_cmd_01[], *const yok0d_cmd_02[],
	*const yok0d_cmd_03[], *const yok0d_cmd_04[], *const yok0d_cmd_05[], *const yok0d_cmd_06[],
	*const yok0d_cmd_07[], *const yok0d_cmd_08[];
extern const Uint8 yok0d_sel_00[], yok0d_sel_01[], yok0d_sel_02[], yok0d_sel_03[], yok0d_sel_04[],
	yok0d_sel_05[], yok0d_sel_06[], yok0d_sel_07[], yok0d_sel_08[], yok0d_cmd_00_00[],
	yok0d_cmd_00_01[], yok0d_cmd_00_02[], yok0d_cmd_01_00[], yok0d_cmd_01_01[], yok0d_cmd_01_02[],
	yok0d_cmd_02_00[], yok0d_cmd_02_01[], yok0d_cmd_02_02[], yok0d_cmd_03_00[], yok0d_cmd_03_01[],
	yok0d_cmd_03_02[], yok0d_cmd_04_00[], yok0d_cmd_04_01[], yok0d_cmd_04_02[], yok0d_cmd_04_03[],
	yok0d_cmd_04_04[], yok0d_cmd_04_05[], yok0d_cmd_05_00[], yok0d_cmd_05_01[], yok0d_cmd_05_02[],
	yok0d_cmd_06_00[], yok0d_cmd_06_01[], yok0d_cmd_06_02[], yok0d_cmd_06_03[], yok0d_cmd_06_04[],
	yok0d_cmd_06_05[], yok0d_cmd_07_00[], yok0d_cmd_07_01[], yok0d_cmd_07_02[], yok0d_cmd_07_03[],
	yok0d_cmd_07_04[], yok0d_cmd_08_00[], yok0d_cmd_08_01[], yok0d_cmd_08_02[], yok0d_cmd_08_03[],
	yok0d_cmd_08_04[], yok0d_cmd_08_05[];

const CPU_DATA yok0d_tbl[] = {
	{ ( Sint32 )yok0d_sel_00, ( Sint32 )yok0d_sel_00, ( Sint32 )yok0d_cmd_00 },	/* YK_GD030 */
	{ ( Sint32 )yok0d_sel_01, ( Sint32 )yok0d_sel_01, ( Sint32 )yok0d_cmd_01 },	/* YK_GD060 */
	{ ( Sint32 )yok0d_sel_02, ( Sint32 )yok0d_sel_02, ( Sint32 )yok0d_cmd_02 },	/* 2 */
	{ ( Sint32 )yok0d_sel_03, ( Sint32 )yok0d_sel_03, ( Sint32 )yok0d_cmd_03 },	/* 3 */
	{ ( Sint32 )yok0d_sel_04, ( Sint32 )yok0d_sel_04, ( Sint32 )yok0d_cmd_04 },	/* 4 */
	{ ( Sint32 )yok0d_sel_05, ( Sint32 )yok0d_sel_05, ( Sint32 )yok0d_cmd_05 },	/* 5 */
	{ ( Sint32 )yok0d_sel_06, ( Sint32 )yok0d_sel_06, ( Sint32 )yok0d_cmd_06 },	/* 6 */
	{ ( Sint32 )yok0d_sel_07, ( Sint32 )yok0d_sel_07, ( Sint32 )yok0d_cmd_07 },	/* 7 */
	{ ( Sint32 )yok0d_sel_08, ( Sint32 )yok0d_sel_08, ( Sint32 )yok0d_cmd_08 },	/* 8 */
};

const Uint8 yok0d_sel_00[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   2,   2,   2,   2,
};

const Uint8 *const yok0d_cmd_00[] = {
	yok0d_cmd_00_00, yok0d_cmd_00_01, yok0d_cmd_00_02,
};

const Uint8 yok0d_sel_01[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   2,   2,   2,   2,
};

const Uint8 *const yok0d_cmd_01[] = {
	yok0d_cmd_01_00, yok0d_cmd_01_01, yok0d_cmd_01_02,
};

const Uint8 yok0d_sel_02[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   2,   2,   2,   2,
};

const Uint8 *const yok0d_cmd_02[] = {
	yok0d_cmd_02_00, yok0d_cmd_02_01, yok0d_cmd_02_02,
};

const Uint8 yok0d_sel_03[] = {
	  0,   0,   0,   0,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,
	  2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
};

const Uint8 *const yok0d_cmd_03[] = {
	yok0d_cmd_03_00, yok0d_cmd_03_01, yok0d_cmd_03_02,
};

const Uint8 yok0d_sel_04[] = {
	  0,   0,   0,   0,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,
	  3,   3,   3,   3,   3,   3,   3,   3,   5,   5,   5,   5,   4,   4,   4,   4,
};

const Uint8 *const yok0d_cmd_04[] = {
	yok0d_cmd_04_00, yok0d_cmd_04_01, yok0d_cmd_04_02, yok0d_cmd_04_03, yok0d_cmd_04_04, yok0d_cmd_04_05,
};

const Uint8 yok0d_sel_05[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  2,   2,   2,   2,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,
};

const Uint8 *const yok0d_cmd_05[] = {
	yok0d_cmd_05_00, yok0d_cmd_05_01, yok0d_cmd_05_02,
};

const Uint8 yok0d_sel_06[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,
	  2,   2,   2,   2,   3,   3,   3,   3,   4,   4,   4,   4,   5,   5,   5,   5,
};

const Uint8 *const yok0d_cmd_06[] = {
	yok0d_cmd_06_00, yok0d_cmd_06_01, yok0d_cmd_06_02, yok0d_cmd_06_03, yok0d_cmd_06_04, yok0d_cmd_06_05,
};

const Uint8 yok0d_sel_07[] = {
	  0,   0,   0,   0,   1,   1,   1,   1,   2,   2,   2,   2,   3,   3,   3,   3,
	  4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
};

const Uint8 *const yok0d_cmd_07[] = {
	yok0d_cmd_07_00, yok0d_cmd_07_01, yok0d_cmd_07_02, yok0d_cmd_07_03, yok0d_cmd_07_04,
};

const Uint8 yok0d_sel_08[] = {
	  0,   0,   0,   0,   1,   1,   1,   1,   2,   2,   2,   2,   3,   3,   3,   3,
	  4,   4,   4,   4,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,
};

const Uint8 *const yok0d_cmd_08[] = {
	yok0d_cmd_08_00, yok0d_cmd_08_01, yok0d_cmd_08_02, yok0d_cmd_08_03, yok0d_cmd_08_04, yok0d_cmd_08_05,
};

const Uint8 yok0d_cmd_00_00[] = {
	LOCK_SET,
	IF_AREA2( 96, 208, 64, 0 ),
		CMD_JUMP( 40 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0d_cmd_00_01[] = {
	LOCK_SET,
	GUARD( 30 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0d_cmd_00_02[] = {
	LOCK_SET,
	WAIT( 15 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0d_cmd_01_00[] = {
	LOCK_SET,
	IF_AREA2( 96, 208, 64, 0 ),
		CMD_JUMP( 49 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0d_cmd_01_01[] = {
	LOCK_SET,
	GUARD( 60 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0d_cmd_01_02[] = {
	LOCK_SET,
	WAIT( 30 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0d_cmd_02_00[] = {
	LOCK_SET,
	IF_AREA2( 96, 208, 64, 0 ),
		CMD_JUMP( 50 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0d_cmd_02_01[] = {
	LOCK_SET,
	GUARD( 120 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0d_cmd_02_02[] = {
	LOCK_SET,
	WAIT( 60 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0d_cmd_03_00[] = {
	LOCK_SET,
	CMD_JUMP( 50 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0d_cmd_03_01[] = {
	LOCK_SET,
	GUARD( 120 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0d_cmd_03_02[] = {
	LOCK_SET,
	WAIT( 60 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0d_cmd_04_00[] = {
	LOCK_SET,
	CMD_JUMP( 51 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0d_cmd_04_01[] = {
	LOCK_SET,
	IF_AREA2( 160, 208, 0, 0 ),
		IF_LEVEL( 31 ),
			IF_RND( 31 ),
				GUARD( 30 ),
				IF_AREA2( 144, 144, 0, 0 ),
					GUARD( 30 ),
					IF_AREA2( 144, 144, 0, 0 ),
						GUARD( 30 ),
						IF_AREA2( 144, 144, 0, 0 ),
							GUARD( 30 ),
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 16 ),
				IF_RND( 29 ),
					GUARD( 30 ),
					IF_AREA2( 144, 144, 0, 0 ),
						GUARD( 30 ),
						IF_AREA2( 144, 144, 0, 0 ),
							GUARD( 30 ),
							IF_AREA2( 144, 144, 0, 0 ),
								GUARD( 30 ),
							CMD_ENDIF,
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ELSE,
				IF_RND( 27 ),
					GUARD( 30 ),
					IF_AREA2( 144, 144, 0, 0 ),
						GUARD( 30 ),
						IF_AREA2( 144, 144, 0, 0 ),
							GUARD( 30 ),
							IF_AREA2( 144, 144, 0, 0 ),
								GUARD( 30 ),
							CMD_ENDIF,
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		WAIT2( 2 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0d_cmd_04_02[] = {
	LOCK_SET,
	TIMER_SET( 40 ),
	IF_AREA2( 240, 112, 48, 0 ),
		IF_AREA2( 144, 90, 0, 0 ),
			GUARD( 60 ),
		CMD_ELSE,
			TIMER_SET( 60 ),
			IF_AREA2( 56, 144, 0, 0 ),
				JUMP( DIR_UP ),
				CMD_JUMP( 48 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
	CMD_ENDIF,
	STAND,
	LOCK_CLR,
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 yok0d_cmd_04_03[] = {
	LOCK_SET,
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	TIMER_SET( 40 ),
	IF_AREA2( 240, 112, 48, 0 ),
		IF_AREA2( 144, 144, 0, 0 ),
			GUARD( 60 ),
		CMD_ELSE,
			TIMER_SET( 60 ),
			IF_AREA2( 192, 112, 0, 0 ),
				JUMP( DIR_FORWARD ),
				CMD_JUMP( 48 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
	CMD_ENDIF,
	STAND,
	LOCK_CLR,
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 yok0d_cmd_04_04[] = {
	LOCK_SET,
	IF_LEVEL( 24 ),
		IF_RND( 20 ),
			CMD_JUMP( 46 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 21 ),
			IF_RND( 19 ),
				CMD_JUMP( 46 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 13 ),
				IF_RND( 18 ),
					CMD_JUMP( 46 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 7 ),
					IF_RND( 17 ),
						CMD_JUMP( 46 ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_RND( 16 ),
						CMD_JUMP( 46 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0d_cmd_04_05[] = {
	LOCK_SET,
	IF_DIST( 96, 0, 96, 0 ),
		IF_DIST( 48, 0, 48, 0 ),
			IF_LEVEL( 24 ),
				CMD_JUMP( 5 ),
			CMD_ELSE,
				IF_LEVEL( 16 ),
					CMD_JUMP( 2 ),
				CMD_ELSE,
					IF_LEVEL( 8 ),
						CMD_JUMP( 2 ),
					CMD_ELSE,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 24 ),
				CMD_JUMP( 5 ),
			CMD_ELSE,
				IF_LEVEL( 16 ),
					CMD_JUMP( 1 ),
				CMD_ELSE,
					IF_LEVEL( 8 ),
						CMD_JUMP( 1 ),
					CMD_ELSE,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 24 ),
			CMD_JUMP( 5 ),
		CMD_ELSE,
			IF_LEVEL( 16 ),
				CMD_JUMP( 0 ),
			CMD_ELSE,
				IF_LEVEL( 8 ),
					CMD_JUMP( 0 ),
				CMD_ELSE,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0d_cmd_05_00[] = {
	LOCK_SET,
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 2 ),
		CMD_ELSE,
			GUARD_BACK_SET( 1 ),
		CMD_ENDIF,
	CMD_ENDIF,
	IF_AREA2( 96, 144, 16, 0 ),
		GUARD2( 60 ),
	CMD_ELSE,
		IF_RND( 20 ),
			DASH_EM( 32 ),
			IF_LEVEL( 9 ),
				IF_RND( 24 ),
					JUMP_LEVEL( 0, 8, 0, 29, 2, 31, 1 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			JUMP( DIR_UP ),
			JUMP_FLAG_SET,
				IF_LEVEL( 18 ),
					CMD_JUMP( 40 ),
					IF_RND( 16 ),
						JUMP_LEVEL( 48, 8, 48, 22, 48, 31, 48 ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_RND( 26 ),
						JUMP_LEVEL( 48, 8, 48, 22, 48, 24, 48 ),
					CMD_ENDIF,
				CMD_ENDIF,
				JUMP_FLAG_CLR,
			CMD_ENDIF,
			STAND,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0d_cmd_05_01[] = {
	LOCK_SET,
	CMD_JUMP( 51 ),
	IF_DIST( 96, 0, 96, 0 ),
		IF_DIST( 48, 0, 48, 0 ),
			IF_LEVEL( 24 ),
				CMD_JUMP( 5 ),
			CMD_ELSE,
				IF_LEVEL( 16 ),
					CMD_JUMP( 2 ),
				CMD_ELSE,
					IF_LEVEL( 8 ),
						CMD_JUMP( 2 ),
					CMD_ELSE,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 24 ),
				CMD_JUMP( 5 ),
			CMD_ELSE,
				IF_LEVEL( 16 ),
					CMD_JUMP( 1 ),
				CMD_ELSE,
					IF_LEVEL( 8 ),
						CMD_JUMP( 1 ),
					CMD_ELSE,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 24 ),
			CMD_JUMP( 5 ),
		CMD_ELSE,
			IF_LEVEL( 16 ),
				CMD_JUMP( 0 ),
			CMD_ELSE,
				IF_LEVEL( 8 ),
					CMD_JUMP( 0 ),
				CMD_ELSE,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0d_cmd_05_02[] = {
	LOCK_SET,
	IF_LEVEL( 24 ),
		IF_RND( 17 ),
			CMD_JUMP( 46 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 21 ),
			IF_RND( 16 ),
				CMD_JUMP( 46 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 13 ),
				IF_RND( 15 ),
					CMD_JUMP( 46 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 7 ),
					IF_RND( 14 ),
						CMD_JUMP( 46 ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_RND( 13 ),
						CMD_JUMP( 46 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0d_cmd_06_00[] = {
	LOCK_SET,
	IF_AREA2( 96, 208, 64, 0 ),
		CMD_JUMP( 50 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0d_cmd_06_01[] = {
	LOCK_SET,
	CMD_JUMP( 51 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0d_cmd_06_02[] = {
	LOCK_SET,
	TIMER_SET( 20 ),
	IF_AREA2( 112, 128, 0, 0 ),
		IF_LEVEL( 24 ),
			IF_RND( 12 ),
				JUMP( DIR_UP ),
				CMD_JUMP( 48 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 20 ),
				IF_RND( 10 ),
					JUMP( DIR_UP ),
					CMD_JUMP( 52 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 16 ),
					IF_RND( 8 ),
						JUMP( DIR_UP ),
						CMD_JUMP( 52 ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_LEVEL( 8 ),
						IF_RND( 6 ),
							JUMP( DIR_UP ),
							CMD_JUMP( 52 ),
						CMD_ENDIF,
					CMD_ELSE,
						IF_RND( 2 ),
							JUMP( DIR_UP ),
							CMD_JUMP( 52 ),
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		WAIT2( 2 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0d_cmd_06_03[] = {
	LOCK_SET,
	TIMER_SET( 20 ),
	IF_AREA2( 112, 128, 0, 0 ),
		IF_LEVEL( 24 ),
			IF_RND( 17 ),
				CMD_JUMP( 46 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 20 ),
				IF_RND( 16 ),
					CMD_JUMP( 46 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 16 ),
					IF_RND( 15 ),
						CMD_JUMP( 46 ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_LEVEL( 8 ),
						IF_RND( 14 ),
							CMD_JUMP( 46 ),
						CMD_ENDIF,
					CMD_ELSE,
						IF_RND( 13 ),
							CMD_JUMP( 46 ),
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		WAIT2( 2 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0d_cmd_06_04[] = {
	LOCK_SET,
	TIMER_SET( 10 ),
	IF_AREA2( 192, 112, 0, 0 ),
		IF_AREA2( 64, 112, 0, 0 ),
			GUARD( 30 ),
		CMD_ELSE,
			JUMP( DIR_FORWARD ),
			CMD_JUMP( 48 ),
			WAIT( 25 ),
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	LOCK_CLR,
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 yok0d_cmd_06_05[] = {
	CMD_EXIT,
};

const Uint8 yok0d_cmd_07_00[] = {
	LOCK_SET,
	IF_AREA2( 96, 208, 64, 0 ),
		CMD_JUMP( 40 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0d_cmd_07_01[] = {
	LOCK_SET,
	GUARD( 30 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0d_cmd_07_02[] = {
	LOCK_SET,
	WAIT( 15 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0d_cmd_07_03[] = {
	LOCK_SET,
	CMD_JUMP( 51 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0d_cmd_07_04[] = {
	LOCK_SET,
	CMD_JUMP( 51 ),
	IF_LEVEL( 24 ),
		JUMP_RND( 4, 5, 5, 2 ),
	CMD_ELSE,
		IF_LEVEL( 16 ),
			JUMP_RND( 1, 2, 2, 4 ),
		CMD_ELSE,
			IF_LEVEL( 8 ),
				IF_DIST( 128, 128, 0, 64 ),
					CMD_JUMP( 7 ),
				CMD_ELSE,
					JUMP_RND( 1, 2, 0, 0 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_DIST( 128, 128, 0, 64 ),
					CMD_JUMP( 10 ),
				CMD_ELSE,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0d_cmd_08_00[] = {
	LOCK_SET,
	IF_AREA2( 240, 144, 48, 0 ),
		CMD_JUMP( 49 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0d_cmd_08_01[] = {
	LOCK_SET,
	TIMER_SET( 20 ),
	IF_AREA2( 384, 0, 192, 0 ),
		IF_LEVEL( 24 ),
			IF_RND( 12 ),
				JUMP( DIR_UP ),
				CMD_JUMP( 48 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 20 ),
				IF_RND( 10 ),
					JUMP( DIR_UP ),
					CMD_JUMP( 52 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 16 ),
					IF_RND( 8 ),
						JUMP( DIR_UP ),
						CMD_JUMP( 52 ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_LEVEL( 8 ),
						IF_RND( 6 ),
							JUMP( DIR_UP ),
							CMD_JUMP( 52 ),
						CMD_ENDIF,
					CMD_ELSE,
						IF_RND( 2 ),
							JUMP( DIR_UP ),
							CMD_JUMP( 52 ),
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		WAIT2( 2 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0d_cmd_08_02[] = {
	LOCK_SET,
	GUARD( 120 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0d_cmd_08_03[] = {
	LOCK_SET,
	TIMER_SET( 10 ),
	IF_AREA2( 384, 0, 192, 0 ),
		IF_AREA2( 144, 144, 0, 0 ),
			GUARD( 30 ),
		CMD_ELSE,
			JUMP( DIR_FORWARD ),
			CMD_JUMP( 48 ),
			WAIT( 25 ),
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	LOCK_CLR,
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 yok0d_cmd_08_04[] = {
	LOCK_SET,
	CMD_JUMP( 49 ),
	IF_LEVEL( 24 ),
		JUMP_RND( 4, 6, 6, 2 ),
	CMD_ELSE,
		IF_LEVEL( 16 ),
			JUMP_RND( 1, 2, 2, 4 ),
		CMD_ELSE,
			IF_LEVEL( 8 ),
				IF_DIST( 128, 128, 0, 64 ),
					CMD_JUMP( 7 ),
				CMD_ELSE,
					JUMP_RND( 1, 2, 0, 0 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_DIST( 128, 128, 0, 64 ),
					CMD_JUMP( 10 ),
				CMD_ELSE,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok0d_cmd_08_05[] = {
	LOCK_SET,
	TIMER_SET( 20 ),
	IF_AREA2( 384, 0, 192, 0 ),
		IF_LEVEL( 24 ),
			IF_RND( 17 ),
				CMD_JUMP( 46 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 20 ),
				IF_RND( 16 ),
					CMD_JUMP( 46 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 16 ),
					IF_RND( 15 ),
						CMD_JUMP( 46 ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_LEVEL( 8 ),
						IF_RND( 14 ),
							CMD_JUMP( 46 ),
						CMD_ENDIF,
					CMD_ELSE,
						IF_RND( 13 ),
							CMD_JUMP( 46 ),
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		WAIT2( 2 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

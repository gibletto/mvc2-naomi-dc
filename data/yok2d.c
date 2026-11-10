/* Shuma Gorath (character 2d): the computer's answers.
   What it does about an attack that is coming. PLAYER_WORK's cpu_answer_adrs points here;
   em_answer_sel picks a list from it.

   yok2d_tbl        a record (CPU_DATA) for each answer. The row of the attack in its owner's hit
                    table (hit_dtXX.c) names the answer; YK_GD030 and YK_GD060 are the first two.
                    A record is the offsets of its sel (twice: no rate is read) and its cmd.
   yok2d_sel_AA     32 picks; one is taken at random.
   yok2d_cmd_AA     the answer's lists: a pick is a number in this table.
   yok2d_cmd_AA_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name
                    a list of lib2d.c. */
#include "player.h"
#include "pl2d_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const yok2d_cmd_00[], *const yok2d_cmd_01[], *const yok2d_cmd_02[],
	*const yok2d_cmd_03[], *const yok2d_cmd_04[], *const yok2d_cmd_05[], *const yok2d_cmd_06[],
	*const yok2d_cmd_07[], *const yok2d_cmd_08[];
extern const Uint8 yok2d_sel_00[], yok2d_sel_01[], yok2d_sel_02[], yok2d_sel_03[], yok2d_sel_04[],
	yok2d_sel_05[], yok2d_sel_06[], yok2d_sel_07[], yok2d_sel_08[], yok2d_cmd_00_00[],
	yok2d_cmd_00_01[], yok2d_cmd_00_02[], yok2d_cmd_00_03[], yok2d_cmd_00_04[], yok2d_cmd_01_00[],
	yok2d_cmd_01_01[], yok2d_cmd_01_02[], yok2d_cmd_01_03[], yok2d_cmd_01_04[], yok2d_cmd_01_05[],
	yok2d_cmd_02_00[], yok2d_cmd_02_01[], yok2d_cmd_02_02[], yok2d_cmd_02_03[], yok2d_cmd_02_04[],
	yok2d_cmd_02_05[], yok2d_cmd_03_00[], yok2d_cmd_03_01[], yok2d_cmd_03_02[], yok2d_cmd_03_03[],
	yok2d_cmd_03_04[], yok2d_cmd_03_05[], yok2d_cmd_04_00[], yok2d_cmd_04_01[], yok2d_cmd_04_02[],
	yok2d_cmd_04_03[], yok2d_cmd_04_04[], yok2d_cmd_04_05[], yok2d_cmd_05_00[], yok2d_cmd_05_01[],
	yok2d_cmd_05_02[], yok2d_cmd_06_00[], yok2d_cmd_06_01[], yok2d_cmd_06_02[], yok2d_cmd_06_03[],
	yok2d_cmd_06_04[], yok2d_cmd_06_05[], yok2d_cmd_07_00[], yok2d_cmd_07_01[], yok2d_cmd_07_02[],
	yok2d_cmd_07_03[], yok2d_cmd_08_00[], yok2d_cmd_08_01[];

const CPU_DATA yok2d_tbl[] = {
	{ ( Sint32 )yok2d_sel_00, ( Sint32 )yok2d_sel_00, ( Sint32 )yok2d_cmd_00 },	/* YK_GD030 */
	{ ( Sint32 )yok2d_sel_01, ( Sint32 )yok2d_sel_01, ( Sint32 )yok2d_cmd_01 },	/* YK_GD060 */
	{ ( Sint32 )yok2d_sel_02, ( Sint32 )yok2d_sel_02, ( Sint32 )yok2d_cmd_02 },	/* 2 */
	{ ( Sint32 )yok2d_sel_03, ( Sint32 )yok2d_sel_03, ( Sint32 )yok2d_cmd_03 },	/* 3 */
	{ ( Sint32 )yok2d_sel_04, ( Sint32 )yok2d_sel_04, ( Sint32 )yok2d_cmd_04 },	/* 4 */
	{ ( Sint32 )yok2d_sel_05, ( Sint32 )yok2d_sel_05, ( Sint32 )yok2d_cmd_05 },	/* 5 */
	{ ( Sint32 )yok2d_sel_06, ( Sint32 )yok2d_sel_06, ( Sint32 )yok2d_cmd_06 },	/* 6 */
	{ ( Sint32 )yok2d_sel_07, ( Sint32 )yok2d_sel_07, ( Sint32 )yok2d_cmd_07 },	/* 7 */
	{ ( Sint32 )yok2d_sel_08, ( Sint32 )yok2d_sel_08, ( Sint32 )yok2d_cmd_08 },	/* 8 */
};

const Uint8 yok2d_sel_00[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
	  1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   3,   4,
};

const Uint8 *const yok2d_cmd_00[] = {
	yok2d_cmd_00_00, yok2d_cmd_00_01, yok2d_cmd_00_02, yok2d_cmd_00_03, yok2d_cmd_00_04,
};

const Uint8 yok2d_sel_01[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,
	  1,   1,   2,   2,   2,   2,   2,   3,   3,   3,   4,   4,   4,   4,   5,   5,
};

const Uint8 *const yok2d_cmd_01[] = {
	yok2d_cmd_01_00, yok2d_cmd_01_01, yok2d_cmd_01_02, yok2d_cmd_01_03, yok2d_cmd_01_04, yok2d_cmd_01_05,
};

const Uint8 yok2d_sel_02[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   2,
	  2,   2,   2,   2,   2,   2,   2,   2,   3,   3,   4,   4,   4,   5,   5,   5,
};

const Uint8 *const yok2d_cmd_02[] = {
	yok2d_cmd_02_00, yok2d_cmd_02_01, yok2d_cmd_02_02, yok2d_cmd_02_03, yok2d_cmd_02_04, yok2d_cmd_02_05,
};

const Uint8 yok2d_sel_03[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   1,   1,   1,   1,   1,   1,   2,   2,   3,   3,   4,   4,   5,   5,
};

const Uint8 *const yok2d_cmd_03[] = {
	yok2d_cmd_03_00, yok2d_cmd_03_01, yok2d_cmd_03_02, yok2d_cmd_03_03, yok2d_cmd_03_04, yok2d_cmd_03_05,
};

const Uint8 yok2d_sel_04[] = {
	  0,   0,   0,   0,   1,   1,   2,   2,   2,   2,   2,   2,   2,   3,   3,   3,
	  3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   4,   4,   4,   5,   5,   5,
};

const Uint8 *const yok2d_cmd_04[] = {
	yok2d_cmd_04_00, yok2d_cmd_04_01, yok2d_cmd_04_02, yok2d_cmd_04_03, yok2d_cmd_04_04, yok2d_cmd_04_05,
};

const Uint8 yok2d_sel_05[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2,
	  1,   1,   1,   1,   1,   1,   1,   2,   1,   1,   1,   1,   1,   1,   1,   2,
};

const Uint8 *const yok2d_cmd_05[] = {
	yok2d_cmd_05_00, yok2d_cmd_05_01, yok2d_cmd_05_02,
};

const Uint8 yok2d_sel_06[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,
	  2,   2,   2,   3,   3,   3,   3,   3,   3,   4,   4,   4,   4,   5,   5,   5,
};

const Uint8 *const yok2d_cmd_06[] = {
	yok2d_cmd_06_00, yok2d_cmd_06_01, yok2d_cmd_06_02, yok2d_cmd_06_03, yok2d_cmd_06_04, yok2d_cmd_06_05,
};

const Uint8 yok2d_sel_07[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   2,   2,   3,   3,
};

const Uint8 *const yok2d_cmd_07[] = {
	yok2d_cmd_07_00, yok2d_cmd_07_01, yok2d_cmd_07_02, yok2d_cmd_07_03,
};

const Uint8 yok2d_sel_08[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,
};

const Uint8 *const yok2d_cmd_08[] = {
	yok2d_cmd_08_00, yok2d_cmd_08_01,
};

const Uint8 yok2d_cmd_00_00[] = {
	LOCK_SET,
	IF_AREA2( 96, 80, 0, 48 ),
		GUARD( 30 ),
	CMD_ELSE,
		WAIT2( 2 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2d_cmd_00_01[] = {
	LOCK_SET,
	IF_ATCK_COME,
		IF_LEVEL( 24 ),
			IF_RND( 26 ),
				GUARD( 30 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 21 ),
				IF_RND( 24 ),
					GUARD( 30 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 13 ),
					IF_RND( 22 ),
						GUARD( 30 ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_LEVEL( 7 ),
						IF_RND( 20 ),
							GUARD( 30 ),
						CMD_ENDIF,
					CMD_ELSE,
						IF_RND( 16 ),
							GUARD( 30 ),
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

const Uint8 yok2d_cmd_00_02[] = {
	LOCK_SET,
	IF_AREA2( 64, 80, 0, 48 ),
		JUMP( DIR_UP ),
		JUMP_FLAG_SET,
			IF_CHAR( 1 ),
				CMD_JUMP( 72 ),
			CMD_ELSE,
				JUMP_LEVEL( 48, 7, 49, 13, 50, 21, 51 ),
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		WAIT2( 2 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2d_cmd_00_03[] = {
	LOCK_SET,
	CMD_JUMP( 39 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2d_cmd_00_04[] = {
	LOCK_SET,
	IF_AREA2( 144, 80, 0, 48 ),
		SJUMP( DIR_FORWARD ),
		SJUMP_FLAG_SET,
			CMD_JUMP( 73 ),
			JUMP_LEVEL( 42, 8, 43, 16, 44, 24, 45 ),
			CMD_JUMP( 31 ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2d_cmd_01_00[] = {
	LOCK_SET,
	IF_AREA2( 144, 208, 0, 0 ),
		CMD_JUMP( 67 ),
	CMD_ELSE,
		WAIT2( 2 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2d_cmd_01_01[] = {
	LOCK_SET,
	IF_AREA2( 88, 144, 0, 0 ),
		JUMP( DIR_UP ),
		JUMP_FLAG_SET,
			IF_CHAR( 1 ),
				CMD_JUMP( 72 ),
			CMD_ELSE,
				JUMP_LEVEL( 48, 7, 49, 13, 50, 21, 51 ),
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		WAIT2( 2 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2d_cmd_01_02[] = {
	LOCK_SET,
	IF_ATCK_COME,
		CMD_JUMP( 67 ),
	CMD_ELSE,
		WAIT2( 2 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2d_cmd_01_03[] = {
	LOCK_SET,
	IF_LEVEL( 8 ),
		IF_RND( 20 ),
			IF_AREA2( 96, 80, 64, 48 ),
				DASH_B( 224 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 yok2d_cmd_01_04[] = {
	LOCK_SET,
	CMD_JUMP( 70 ),
	CMD_JUMP( 39 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2d_cmd_01_05[] = {
	LOCK_SET,
	SJUMP( DIR_FORWARD ),
	SJUMP_FLAG_SET,
		IF_CHAR( 1 ),
			CMD_JUMP( 73 ),
		CMD_ELSE,
			JUMP_LEVEL( 42, 8, 43, 16, 44, 24, 45 ),
			CMD_JUMP( 31 ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2d_cmd_02_00[] = {
	LOCK_SET,
	IF_AREA2( 144, 208, 0, 0 ),
		CMD_JUMP( 68 ),
	CMD_ELSE,
		WAIT2( 2 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2d_cmd_02_01[] = {
	LOCK_SET,
	IF_AREA2( 88, 144, 0, 0 ),
		JUMP( DIR_UP ),
		JUMP_FLAG_SET,
			IF_CHAR( 1 ),
				CMD_JUMP( 72 ),
			CMD_ELSE,
				JUMP_LEVEL( 48, 7, 49, 13, 50, 21, 51 ),
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		WAIT2( 2 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2d_cmd_02_02[] = {
	VCOUNTER_SET( 6 ),
	LOCK_SET,
	IF_ATCK_COME,
		CMD_JUMP( 67 ),
	CMD_ELSE,
		WAIT2( 2 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2d_cmd_02_03[] = {
	LOCK_SET,
	IF_LEVEL( 8 ),
		IF_RND( 20 ),
			IF_AREA2( 160, 240, 0, 0 ),
				DASH_B( 224 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 yok2d_cmd_02_04[] = {
	LOCK_SET,
	CMD_JUMP( 70 ),
	CMD_JUMP( 27 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2d_cmd_02_05[] = {
	LOCK_SET,
	SJUMP( DIR_FORWARD ),
	SJUMP_FLAG_SET,
		IF_CHAR( 1 ),
			CMD_JUMP( 73 ),
		CMD_ELSE,
			JUMP_LEVEL( 42, 8, 43, 16, 44, 24, 45 ),
			CMD_JUMP( 31 ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2d_cmd_03_00[] = {
	LOCK_SET,
	IF_AREA2( 80, 112, 0, 0 ),
		IF_LEVEL( 16 ),
			IF_RND( 4 ),
				GUARD( 120 ),
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

const Uint8 yok2d_cmd_03_01[] = {
	LOCK_SET,
	IF_AREA2( 80, 112, 0, 0 ),
		GUARD( 30 ),
	CMD_ELSE,
		WAIT2( 2 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2d_cmd_03_02[] = {
	LOCK_SET,
	IF_AREA2( 56, 64, 48, 64 ),
		JUMP( DIR_UP ),
		JUMP_FLAG_SET,
			IF_CHAR( 1 ),
				CMD_JUMP( 72 ),
			CMD_ELSE,
				TIMER_SET( 40 ),
				IF_HEIGHT( 1, 96 ),
					TIMER_SET( 10 ),
					IF_AREA( 48, 16, 48, 32 ),
						PUNCH( LV_LIGHT ),
						KICK( LV_LIGHT ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2d_cmd_03_03[] = {
	LOCK_SET,
	IF_AREA2( 160, 96, 0, 64 ),
		GUARD( 60 ),
	CMD_ENDIF,
	LOCK_CLR,
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 yok2d_cmd_03_04[] = {
	LOCK_SET,
	IF_AREA2( 160, 96, 0, 64 ),
		GUARD( 120 ),
	CMD_ENDIF,
	LOCK_CLR,
	WAIT2( 2 ),
	CMD_EXIT,
};

const Uint8 yok2d_cmd_03_05[] = {
	LOCK_SET,
	IF_AREA2( 144, 64, 0, 48 ),
		TIMER2_SET( 30 ),
		SJUMP( DIR_FORWARD ),
		JUMP_FLAG_SET,
			IF_CHAR( 1 ),
				CMD_JUMP( 73 ),
			CMD_ELSE,
				JUMP_LEVEL( 42, 8, 43, 16, 44, 24, 45 ),
				CMD_JUMP( 31 ),
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2d_cmd_04_00[] = {
	LOCK_SET,
	IF_AREA2( 80, 72, 0, 48 ),
		GUARD( 30 ),
	CMD_ELSE,
		WAIT2( 2 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2d_cmd_04_01[] = {
	LOCK_SET,
	TIMER_SET( 8 ),
	IF_AREA2( 112, 80, 48, 48 ),
		IF_LEVEL( 31 ),
			GUARD( 30 ),
			CMD_JUMP( 69 ),
		CMD_ELSE,
			IF_LEVEL( 16 ),
				IF_RND( 29 ),
					GUARD( 30 ),
					CMD_JUMP( 69 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_RND( 27 ),
					GUARD( 30 ),
					CMD_JUMP( 69 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		WAIT2( 2 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2d_cmd_04_02[] = {
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
					IF_CHAR( 1 ),
						CMD_JUMP( 72 ),
					CMD_ELSE,
						TIMER_SET( 40 ),
						IF_HEIGHT( 1, 96 ),
							TIMER_SET( 10 ),
							IF_AREA( 48, 16, 48, 32 ),
								PUNCH( LV_LIGHT ),
								KICK( LV_LIGHT ),
							CMD_ENDIF,
						CMD_ENDIF,
					CMD_ENDIF,
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

const Uint8 yok2d_cmd_04_03[] = {
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
					IF_CHAR( 1 ),
						CMD_JUMP( 72 ),
					CMD_ELSE,
						TIMER_SET( 40 ),
						IF_HEIGHT( 1, 96 ),
							TIMER_SET( 10 ),
							IF_AREA( 48, 16, 48, 32 ),
								PUNCH( LV_LIGHT ),
								KICK( LV_LIGHT ),
							CMD_ENDIF,
						CMD_ENDIF,
					CMD_ENDIF,
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

const Uint8 yok2d_cmd_04_04[] = {
	LOCK_SET,
	CMD_JUMP( 70 ),
	CMD_JUMP( 27 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2d_cmd_04_05[] = {
	LOCK_SET,
	IF_AREA2( 176, 48, 160, 48 ),
		IF_AREA2( 80, 48, 64, 48 ),
			SJUMP( DIR_FORWARD ),
			SJUMP_FLAG_SET,
				IF_CHAR( 1 ),
					CMD_JUMP( 73 ),
				CMD_ELSE,
					CMD_JUMP( 31 ),
				CMD_ENDIF,
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2d_cmd_05_00[] = {
	LOCK_SET,
	IF_AREA2( 32, 56, 80, 72 ),
		CMD_JUMP( 67 ),
	CMD_ELSE,
		CMD_JUMP( 72 ),
		IF_RND( 12 ),
			DASH_B( 192 ),
			WAIT2( 4 ),
			STAND,
		CMD_ELSE,
			JUMP( DIR_BACK ),
			JUMP_FLAG_SET,
				IF_CHAR( 1 ),
					CMD_JUMP( 72 ),
				CMD_ELSE,
					TIMER_SET( 40 ),
					IF_HEIGHT( 1, 96 ),
						TIMER_SET( 10 ),
						IF_AREA( 48, 16, 48, 32 ),
							PUNCH( LV_LIGHT ),
							KICK( LV_LIGHT ),
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
				JUMP_FLAG_CLR,
			CMD_ENDIF,
			WAIT2( 4 ),
			STAND,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2d_cmd_05_01[] = {
	LOCK_SET,
	IF_AREA2( 32, 56, 80, 72 ),
		CMD_JUMP( 67 ),
	CMD_ELSE,
		CMD_JUMP( 70 ),
		DASH_EM( 48 ),
		IF_EM_ATCK( ATCK_STAND_MP ),
			PUNCH( LV_LIGHT ),
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT2( 2 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2d_cmd_05_02[] = {
	LOCK_SET,
	IF_AREA2( 32, 56, 80, 72 ),
		CMD_JUMP( 67 ),
	CMD_ELSE,
		IF_LEVEL( 2 ),
			SJUMP( DIR_UP ),
			SJUMP_FLAG_SET,
				IF_CHAR( 1 ),
					CMD_JUMP( 72 ),
				CMD_ELSE,
					JUMP_LEVEL( 42, 8, 43, 16, 44, 24, 45 ),
					CMD_JUMP( 31 ),
				CMD_ENDIF,
				JUMP_FLAG_CLR,
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

const Uint8 yok2d_cmd_06_00[] = {
	LOCK_SET,
	IF_AREA2( 96, 64, 64, 64 ),
		CMD_JUMP( 67 ),
	CMD_ELSE,
		WAIT2( 2 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2d_cmd_06_01[] = {
	LOCK_SET,
	CROUCH,
	TIMER_SET( 40 ),
	IF_ATCK_COME,
		GUARD( 30 ),
	CMD_ELSE,
		IF_LEVEL( 18 ),
			CMD_NOP,
		CMD_ELSE,
			WAIT2( 6 ),
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2d_cmd_06_02[] = {
	LOCK_SET,
	TIMER_SET( 20 ),
	IF_AREA2( 80, 64, 48, 64 ),
		JUMP( DIR_UP ),
		JUMP_FLAG_SET,
			IF_CHAR( 1 ),
				CMD_JUMP( 72 ),
			CMD_ELSE,
				JUMP_LEVEL( 48, 7, 49, 13, 50, 21, 51 ),
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		WAIT2( 2 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2d_cmd_06_03[] = {
	LOCK_SET,
	CMD_JUMP( 72 ),
	CMD_JUMP( 27 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2d_cmd_06_04[] = {
	LOCK_SET,
	IF_AREA2( 80, 48, 64, 48 ),
		SJUMP( DIR_FORWARD ),
		SJUMP_FLAG_SET,
			IF_CHAR( 1 ),
				CMD_JUMP( 72 ),
			CMD_ELSE,
				JUMP_LEVEL( 42, 8, 43, 16, 44, 24, 45 ),
				CMD_JUMP( 31 ),
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2d_cmd_06_05[] = {
	LOCK_SET,
	TIMER_SET( 10 ),
	IF_AREA2( 128, 48, 64, 48 ),
		IF_AREA2( 48, 48, 32, 48 ),
			GUARD( 30 ),
		CMD_ELSE,
			JUMP( DIR_FORWARD ),
			JUMP_FLAG_SET,
				IF_CHAR( 1 ),
					CMD_JUMP( 72 ),
				CMD_ELSE,
					TIMER_SET( 40 ),
					IF_HEIGHT( 1, 96 ),
						TIMER_SET( 10 ),
						IF_AREA( 48, 16, 48, 32 ),
							PUNCH( LV_LIGHT ),
							KICK( LV_LIGHT ),
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
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

const Uint8 yok2d_cmd_07_00[] = {
	LOCK_SET,
	IF_AREA2( 64, 64, 16, 64 ),
		IF_LEVEL( 2 ),
			IF_RND( 4 ),
				GUARD( 60 ),
			CMD_ENDIF,
		CMD_ELSE,
			WAIT( 16 ),
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2d_cmd_07_01[] = {
	LOCK_SET,
	IF_RND( 16 ),
		IF_AREA2( 64, 64, 16, 64 ),
			GUARD( 30 ),
		CMD_ENDIF,
		CMD_JUMP( 70 ),
		TIMER_SET( 48 ),
		IF_EM_ATCK( ATCK_CROUCH_HK ),
			CROUCH,
			KICK( LV_HEAVY ),
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2d_cmd_07_02[] = {
	LOCK_SET,
	IF_RND( 16 ),
		IF_AREA2( 64, 64, 16, 64 ),
			GUARD( 30 ),
		CMD_ENDIF,
		CMD_JUMP( 70 ),
		TIMER_SET( 60 ),
		IF_EM_STANCE( STANCE_STAND ),
			SP_CMD( pl2d_sp_smash, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2d_cmd_07_03[] = {
	LOCK_SET,
	IF_RND( 26 ),
		GUARD_BACK_SET( 1 ),
		IF_AREA2( 64, 64, 16, 64 ),
			GUARD( 30 ),
		CMD_ENDIF,
		CMD_JUMP( 70 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2d_cmd_08_00[] = {
	LOCK_SET,
	GUARD( 50 ),
	CMD_JUMP( 70 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2d_cmd_08_01[] = {
	LOCK_SET,
	GUARD( 50 ),
	CMD_JUMP( 70 ),
	LOCK_CLR,
	CMD_EXIT,
};

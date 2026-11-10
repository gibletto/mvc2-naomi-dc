/* War Machine (character 2e): the computer's answers.
   What it does about an attack that is coming. PLAYER_WORK's cpu_answer_adrs points here;
   em_answer_sel picks a list from it.

   yok2e_tbl        a record (CPU_DATA) for each answer. The row of the attack in its owner's hit
                    table (hit_dtXX.c) names the answer; YK_GD030 and YK_GD060 are the first two.
                    A record is the offsets of its sel (twice: no rate is read) and its cmd.
   yok2e_sel_AA     32 picks; one is taken at random.
   yok2e_cmd_AA     the answer's lists: a pick is a number in this table.
   yok2e_cmd_AA_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name
                    a list of lib2e.c.
   yok2e_unused_N   bytes that nothing in the file reaches. */
#include "player.h"
#include "pl2e_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const yok2e_cmd_00[], *const yok2e_cmd_01[], *const yok2e_cmd_02[],
	*const yok2e_cmd_03[], *const yok2e_cmd_04[], *const yok2e_cmd_05[], *const yok2e_cmd_06[],
	*const yok2e_cmd_07[], *const yok2e_cmd_08[];
extern const Uint8 yok2e_sel_00[], yok2e_sel_01[], yok2e_sel_02[], yok2e_sel_03[], yok2e_sel_04[],
	yok2e_sel_05[], yok2e_sel_06[], yok2e_sel_07[], yok2e_sel_08[], yok2e_unused_0[],
	yok2e_cmd_00_00[], yok2e_cmd_00_01[], yok2e_cmd_00_02[], yok2e_cmd_00_03[], yok2e_cmd_00_04[],
	yok2e_cmd_00_05[], yok2e_cmd_00_06[], yok2e_cmd_00_07[], yok2e_cmd_00_08[], yok2e_cmd_00_09[],
	yok2e_cmd_01_00[], yok2e_cmd_01_01[], yok2e_cmd_01_02[], yok2e_cmd_01_03[], yok2e_cmd_01_04[],
	yok2e_cmd_01_05[], yok2e_cmd_01_06[], yok2e_cmd_01_07[], yok2e_cmd_01_08[], yok2e_cmd_01_09[],
	yok2e_cmd_02_00[], yok2e_cmd_02_01[], yok2e_cmd_02_02[], yok2e_cmd_02_03[], yok2e_cmd_02_04[],
	yok2e_cmd_02_05[], yok2e_cmd_02_06[], yok2e_cmd_02_07[], yok2e_cmd_02_08[], yok2e_cmd_02_09[],
	yok2e_cmd_03_00[], yok2e_cmd_03_01[], yok2e_cmd_03_02[], yok2e_cmd_03_03[], yok2e_cmd_03_04[],
	yok2e_cmd_03_05[], yok2e_cmd_03_06[], yok2e_cmd_03_07[], yok2e_cmd_03_08[], yok2e_cmd_03_09[],
	yok2e_cmd_04_00[], yok2e_cmd_04_01[], yok2e_cmd_04_02[], yok2e_cmd_04_03[], yok2e_cmd_04_04[],
	yok2e_cmd_04_05[], yok2e_cmd_04_06[], yok2e_cmd_04_07[], yok2e_cmd_04_08[], yok2e_cmd_04_09[],
	yok2e_cmd_05_00[], yok2e_cmd_05_01[], yok2e_cmd_05_02[], yok2e_cmd_05_03[], yok2e_cmd_05_04[],
	yok2e_cmd_05_05[], yok2e_cmd_05_06[], yok2e_cmd_05_07[], yok2e_cmd_05_08[], yok2e_cmd_05_09[],
	yok2e_cmd_06_00[], yok2e_cmd_06_01[], yok2e_cmd_06_02[], yok2e_cmd_06_03[], yok2e_cmd_06_04[],
	yok2e_cmd_06_05[], yok2e_cmd_06_06[], yok2e_cmd_06_07[], yok2e_cmd_06_08[], yok2e_cmd_06_09[],
	yok2e_cmd_07_00[], yok2e_cmd_07_01[], yok2e_cmd_07_02[], yok2e_cmd_07_03[], yok2e_cmd_07_04[],
	yok2e_cmd_07_05[], yok2e_cmd_07_06[], yok2e_cmd_07_07[], yok2e_cmd_07_08[], yok2e_cmd_07_09[],
	yok2e_cmd_08_00[], yok2e_cmd_08_01[], yok2e_cmd_08_02[], yok2e_cmd_08_03[], yok2e_cmd_08_04[],
	yok2e_cmd_08_05[], yok2e_cmd_08_06[], yok2e_cmd_08_07[], yok2e_cmd_08_08[], yok2e_cmd_08_09[];

const CPU_DATA yok2e_tbl[] = {
	{ ( Sint32 )yok2e_sel_00, ( Sint32 )yok2e_sel_00, ( Sint32 )yok2e_cmd_00 },	/* YK_GD030 */
	{ ( Sint32 )yok2e_sel_01, ( Sint32 )yok2e_sel_01, ( Sint32 )yok2e_cmd_01 },	/* YK_GD060 */
	{ ( Sint32 )yok2e_sel_02, ( Sint32 )yok2e_sel_02, ( Sint32 )yok2e_cmd_02 },	/* 2 */
	{ ( Sint32 )yok2e_sel_03, ( Sint32 )yok2e_sel_03, ( Sint32 )yok2e_cmd_03 },	/* 3 */
	{ ( Sint32 )yok2e_sel_04, ( Sint32 )yok2e_sel_04, ( Sint32 )yok2e_cmd_04 },	/* 4 */
	{ ( Sint32 )yok2e_sel_05, ( Sint32 )yok2e_sel_05, ( Sint32 )yok2e_cmd_05 },	/* 5 */
	{ ( Sint32 )yok2e_sel_06, ( Sint32 )yok2e_sel_06, ( Sint32 )yok2e_cmd_06 },	/* 6 */
	{ ( Sint32 )yok2e_sel_07, ( Sint32 )yok2e_sel_07, ( Sint32 )yok2e_cmd_07 },	/* 7 */
	{ ( Sint32 )yok2e_sel_08, ( Sint32 )yok2e_sel_08, ( Sint32 )yok2e_cmd_08 },	/* 8 */
};

const Uint8 yok2e_sel_00[] = {
	  0,   0,   1,   1,   2,   2,   3,   3,   0,   0,   1,   1,   2,   2,   3,   3,
	  0,   0,   1,   1,   2,   2,   3,   4,   0,   0,   1,   1,   2,   2,   3,   4,
};

const Uint8 *const yok2e_cmd_00[] = {
	yok2e_cmd_00_00, yok2e_cmd_00_01, yok2e_cmd_00_02, yok2e_cmd_00_03, yok2e_cmd_00_04, yok2e_cmd_00_05,
	yok2e_cmd_00_06, yok2e_cmd_00_07, yok2e_cmd_00_08, yok2e_cmd_00_09,
};

const Uint8 yok2e_sel_01[] = {
	  0,   1,   2,   3,   4,   5,   5,   5,   0,   1,   2,   3,   4,   5,   5,   5,
	  0,   1,   2,   3,   4,   5,   5,   5,   0,   1,   2,   3,   4,   5,   5,   5,
};

const Uint8 *const yok2e_cmd_01[] = {
	yok2e_cmd_01_00, yok2e_cmd_01_01, yok2e_cmd_01_02, yok2e_cmd_01_03, yok2e_cmd_01_04, yok2e_cmd_01_05,
	yok2e_cmd_01_06, yok2e_cmd_01_07, yok2e_cmd_01_08, yok2e_cmd_01_09,
};

const Uint8 yok2e_sel_02[] = {
	  0,   1,   2,   3,   4,   5,   0,   2,   0,   1,   2,   3,   4,   5,   0,   2,
	  0,   1,   2,   3,   1,   5,   1,   3,   0,   1,   2,   3,   1,   5,   1,   3,
};

const Uint8 *const yok2e_cmd_02[] = {
	yok2e_cmd_02_00, yok2e_cmd_02_01, yok2e_cmd_02_02, yok2e_cmd_02_03, yok2e_cmd_02_04, yok2e_cmd_02_05,
	yok2e_cmd_02_06, yok2e_cmd_02_07, yok2e_cmd_02_08, yok2e_cmd_02_09,
};

const Uint8 yok2e_sel_03[] = {
	  0,   1,   2,   3,   3,   3,   4,   5,   0,   1,   2,   3,   3,   3,   4,   5,
	  0,   1,   2,   3,   3,   3,   4,   5,   0,   1,   2,   3,   3,   3,   4,   5,
};

const Uint8 *const yok2e_cmd_03[] = {
	yok2e_cmd_03_00, yok2e_cmd_03_01, yok2e_cmd_03_02, yok2e_cmd_03_03, yok2e_cmd_03_04, yok2e_cmd_03_05,
	yok2e_cmd_03_06, yok2e_cmd_03_07, yok2e_cmd_03_08, yok2e_cmd_03_09,
};

const Uint8 yok2e_sel_04[] = {
	  0,   1,   2,   3,   4,   5,   6,   2,   0,   1,   2,   3,   4,   5,   6,   2,
	  0,   1,   2,   3,   4,   5,   6,   3,   0,   1,   2,   3,   4,   5,   6,   3,
};

const Uint8 *const yok2e_cmd_04[] = {
	yok2e_cmd_04_00, yok2e_cmd_04_01, yok2e_cmd_04_02, yok2e_cmd_04_03, yok2e_cmd_04_04, yok2e_cmd_04_05,
	yok2e_cmd_04_06, yok2e_cmd_04_07, yok2e_cmd_04_08, yok2e_cmd_04_09,
};

const Uint8 yok2e_sel_05[] = {
	  0,   0,   1,   2,   2,   3,   0,   4,   0,   0,   1,   2,   2,   3,   1,   4,
	  0,   1,   1,   2,   3,   3,   2,   4,   0,   1,   1,   2,   3,   3,   3,   4,
};

const Uint8 *const yok2e_cmd_05[] = {
	yok2e_cmd_05_00, yok2e_cmd_05_01, yok2e_cmd_05_02, yok2e_cmd_05_03, yok2e_cmd_05_04, yok2e_cmd_05_05,
	yok2e_cmd_05_06, yok2e_cmd_05_07, yok2e_cmd_05_08, yok2e_cmd_05_09,
};

const Uint8 yok2e_sel_06[] = {
	  0,   1,   2,   3,   4,   0,   2,   4,   0,   1,   2,   3,   4,   0,   2,   4,
	  0,   1,   2,   3,   4,   0,   2,   4,   0,   1,   2,   3,   4,   0,   2,   4,
};

const Uint8 *const yok2e_cmd_06[] = {
	yok2e_cmd_06_00, yok2e_cmd_06_01, yok2e_cmd_06_02, yok2e_cmd_06_03, yok2e_cmd_06_04, yok2e_cmd_06_05,
	yok2e_cmd_06_06, yok2e_cmd_06_07, yok2e_cmd_06_08, yok2e_cmd_06_09,
};

const Uint8 yok2e_sel_07[] = {
	  0,   0,   1,   1,   2,   2,   3,   3,   0,   0,   1,   1,   2,   2,   3,   3,
	  0,   0,   1,   1,   2,   2,   3,   3,   0,   0,   1,   1,   2,   2,   3,   3,
};

const Uint8 *const yok2e_cmd_07[] = {
	yok2e_cmd_07_00, yok2e_cmd_07_01, yok2e_cmd_07_02, yok2e_cmd_07_03, yok2e_cmd_07_04, yok2e_cmd_07_05,
	yok2e_cmd_07_06, yok2e_cmd_07_07, yok2e_cmd_07_08, yok2e_cmd_07_09,
};

const Uint8 yok2e_sel_08[] = {
	  0,   1,   2,   3,   4,   5,   0,   2,   0,   1,   2,   3,   4,   5,   0,   2,
	  0,   1,   2,   3,   4,   5,   1,   3,   0,   1,   2,   3,   4,   5,   1,   3,
};

const Uint8 *const yok2e_cmd_08[] = {
	yok2e_cmd_08_00, yok2e_cmd_08_01, yok2e_cmd_08_02, yok2e_cmd_08_03, yok2e_cmd_08_04, yok2e_cmd_08_05,
	yok2e_cmd_08_06, yok2e_cmd_08_07, yok2e_cmd_08_08, yok2e_cmd_08_09,
};

const Uint8 yok2e_unused_0[] = {
	  0,   0,   1,   1,   2,   2,   3,   0,   0,   0,   1,   1,   2,   2,   3,   0,
	  0,   0,   1,   1,   2,   3,   3,   0,   0,   0,   1,   1,   2,   3,   3,   0,
	248,  11,   0,   0,  35,  12,   0,   0,  90,  12,   0,   0,  95,  12,   0,   0,
	106,  12,   0,   0, 109,  12,   0,   0, 112,  12,   0,   0, 113,  12,   0,   0,
	114,  12,   0,   0, 115,  12,   0,   0,   0,   1,   2,   3,   4,   5,   0,   2,
	  0,   1,   2,   3,   4,   5,   0,   2,   0,   1,   2,   3,   4,   5,   1,   3,
	  0,   1,   2,   3,   4,   5,   1,   3, 116,  12,   0,   0, 119,  12,   0,   0,
	122,  12,   0,   0, 125,  12,   0,   0, 128,  12,   0,   0, 131,  12,   0,   0,
	134,  12,   0,   0, 135,  12,   0,   0, 136,  12,   0,   0, 137,  12,   0,   0,
	  0,   1,   2,   3,   4,   5,   0,   2,   0,   1,   2,   3,   4,   5,   0,   2,
	  0,   1,   2,   3,   4,   5,   1,   3,   0,   1,   2,   3,   4,   5,   1,   3,
	138,  12,   0,   0, 141,  12,   0,   0, 144,  12,   0,   0, 147,  12,   0,   0,
	150,  12,   0,   0, 153,  12,   0,   0, 156,  12,   0,   0, 157,  12,   0,   0,
	158,  12,   0,   0, 159,  12,   0,   0,   0,   1,   2,   3,   4,   5,   0,   2,
	  0,   1,   2,   3,   4,   5,   0,   2,   0,   1,   2,   3,   4,   5,   1,   3,
	  0,   1,   2,   3,   4,   5,   1,   3, 160,  12,   0,   0, 163,  12,   0,   0,
	166,  12,   0,   0, 169,  12,   0,   0, 172,  12,   0,   0, 175,  12,   0,   0,
	178,  12,   0,   0, 179,  12,   0,   0, 180,  12,   0,   0, 181,  12,   0,   0,
	  0,   1,   2,   3,   4,   5,   0,   2,   0,   1,   2,   3,   4,   5,   0,   2,
	  0,   1,   2,   3,   4,   5,   1,   3,   0,   1,   2,   3,   4,   5,   1,   3,
	182,  12,   0,   0, 185,  12,   0,   0, 188,  12,   0,   0, 191,  12,   0,   0,
	194,  12,   0,   0, 197,  12,   0,   0, 200,  12,   0,   0, 201,  12,   0,   0,
	202,  12,   0,   0, 203,  12,   0,   0,   0,   1,   2,   3,   4,   5,   0,   2,
	  0,   1,   2,   3,   4,   5,   0,   2,   0,   1,   2,   3,   4,   5,   1,   3,
	  0,   1,   2,   3,   4,   5,   1,   3, 204,  12,   0,   0, 207,  12,   0,   0,
	210,  12,   0,   0, 213,  12,   0,   0, 216,  12,   0,   0, 219,  12,   0,   0,
	222,  12,   0,   0, 223,  12,   0,   0, 224,  12,   0,   0, 225,  12,   0,   0,
	  0,   1,   2,   3,   4,   5,   0,   2,   0,   1,   2,   3,   4,   5,   0,   2,
	  0,   1,   2,   3,   4,   5,   1,   3,   0,   1,   2,   3,   4,   5,   1,   3,
	226,  12,   0,   0, 229,  12,   0,   0, 232,  12,   0,   0, 235,  12,   0,   0,
	238,  12,   0,   0, 241,  12,   0,   0, 244,  12,   0,   0, 245,  12,   0,   0,
	246,  12,   0,   0, 247,  12,   0,   0,
};

const Uint8 yok2e_cmd_00_00[] = {
	LOCK_SET,
	VCOUNTER_SET( 9 ),
	WAIT2( 6 ),
	IF_ATCK_COME,
		CMD_JUMP( 11 ),
	CMD_ELSE,
		WAIT2( 4 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_00_01[] = {
	LOCK_SET,
	VCOUNTER_SET( 9 ),
	IF_ATCK_COME,
		CMD_JUMP( 11 ),
	CMD_ELSE,
		JUMP( DIR_UP ),
		JUMP_FLAG_SET,
			CMD_JUMP( 12 ),
			TIMER_SET( 40 ),
			IF_DIST( 176, 160, 0, 0 ),
				TIMER_SET( 40 ),
				IF_HEIGHT( 1, 128 ),
					NAG_VFLIP_ADJ4( LV_LIGHT, LEVER_DOWN ),
				CMD_ELSE,
				CMD_ENDIF,
			CMD_ELSE,
				IF_EM_ATCK( pl2e_at_jdown_hp ),
					NAG_VFLIP_ADJ3( LV_HEAVY, LEVER_DOWN ),
				CMD_ELSE,
				CMD_ENDIF,
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ELSE,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
		IF_EM_ATCK( ATCK_CROUCH_MK ),
			CROUCH,
			IF_RND( 16 ),
				KICK( LV_LIGHT ),
			CMD_ELSE,
				KICK( LV_HEAVY ),
			CMD_ENDIF,
			STAND,
		CMD_ELSE,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_00_02[] = {
	LOCK_SET,
	VCOUNTER_SET( 9 ),
	WAIT2( 6 ),
	IF_ATCK_COME,
		GUARD2( 30 ),
	CMD_ELSE,
		IF_RND( 2 ),
			PUNCH( LV_HEAVY ),
		CMD_ELSE,
			WAIT2( 4 ),
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_00_03[] = {
	LOCK_SET,
	VCOUNTER_SET( 9 ),
	IF_DIST( 104, 128, 0, 0 ),
		CMD_JUMP( 11 ),
	CMD_ELSE,
		IF_ATCK_COME,
			CMD_JUMP( 11 ),
		CMD_ELSE,
			DASH_B( 512 ),
			IF_LEVEL( 16 ),
				IF_RND( 12 ),
					CROUCH,
					PUNCH( LV_HEAVY ),
					STAND,
				CMD_ENDIF,
			CMD_ELSE,
				IF_RND( 2 ),
					CROUCH,
					PUNCH( LV_HEAVY ),
					STAND,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_00_04[] = {
	LOCK_SET,
	VCOUNTER_SET( 9 ),
	WAIT2( 6 ),
	IF_RND( 16 ),
		WAIT( 20 ),
	CMD_ELSE,
		WALK_B( 64 ),
		WAIT( 10 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_00_05[] = {
	CMD_EXIT,
};

const Uint8 yok2e_cmd_00_06[] = {
	CMD_EXIT,
};

const Uint8 yok2e_cmd_00_07[] = {
	CMD_EXIT,
};

const Uint8 yok2e_cmd_00_08[] = {
	CMD_EXIT,
};

const Uint8 yok2e_cmd_00_09[] = {
	CMD_EXIT,
};

const Uint8 yok2e_cmd_01_00[] = {
	LOCK_SET,
	WAIT2( 6 ),
	SJUMP( DIR_UP ),
	SJUMP_FLAG_SET,
		WAIT( 60 ),
		FLY( LEVER_FORWARD, 512 ),
		JUMP_FLAG_CLR,
	CMD_ELSE,
		GUARD( 30 ),
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_01_01[] = {
	LOCK_SET,
	WAIT2( 6 ),
	JUMP( DIR_UP ),
	JUMP_FLAG_SET,
		WAIT( 60 ),
		FLY( LEVER_UP | LEVER_BACK, 512 ),
		JUMP_FLAG_CLR,
	CMD_ELSE,
		GUARD( 30 ),
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_01_02[] = {
	LOCK_SET,
	WAIT2( 6 ),
	SJUMP( DIR_FORWARD ),
	SJUMP_FLAG_SET,
		TIMER_SET( 0xc8 ),
		IF_HEIGHT( 0xff, 288 ),
			NAG_VFLIP_ADJ4( LV_LIGHT, LEVER_DOWN ),
		CMD_ELSE,
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ELSE,
		GUARD( 30 ),
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_01_03[] = {
	LOCK_SET,
	WAIT2( 6 ),
	IF_DIST( 32, 160, 0, 0 ),
		CROUCH,
		KICK( LV_LIGHT ),
		STAND,
	CMD_ELSE,
		WAIT( 10 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_01_04[] = {
	LOCK_SET,
	WAIT( 20 ),
	JUMP( DIR_UP ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_01_05[] = {
	LOCK_SET,
	IF_AREA2( 64, 128, 0, 0 ),
		TIMER2_SET( 0xc8 ),
	CMD_ELSE,
		SJUMP( DIR_UP ),
		SJUMP_FLAG_SET,
			CMD_JUMP( 12 ),
			IF_RND( 16 ),
				TIMER_SET( 0xc8 ),
				IF_HEIGHT( 0, 304 ),
					WAIT2( 6 ),
					NAG_VFLIP_ADJ4( LV_LIGHT, LEVER_DOWN ),
				CMD_ELSE,
				CMD_ENDIF,
			CMD_ELSE,
				TIMER_SET( 0xc8 ),
				IF_HEIGHT( 0, 304 ),
					CMD_JUMP( 12 ),
				CMD_ENDIF,
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ELSE,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
		IF_RND( 16 ),
			IF_EM_ATCK( ATCK_STAND_LP ),
				CMD_JUMP( 14 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_RND( 16 ),
				CMD_JUMP( 15 ),
			CMD_ELSE,
				TIMER2_SET( 0xc8 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_01_06[] = {
	CMD_EXIT,
};

const Uint8 yok2e_cmd_01_07[] = {
	CMD_EXIT,
};

const Uint8 yok2e_cmd_01_08[] = {
	CMD_EXIT,
};

const Uint8 yok2e_cmd_01_09[] = {
	CMD_EXIT,
};

const Uint8 yok2e_cmd_02_00[] = {
	LOCK_SET,
	WAIT2( 6 ),
	CMD_JUMP( 12 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_02_01[] = {
	LOCK_SET,
	CMD_JUMP( 17 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_02_02[] = {
	LOCK_SET,
	WAIT2( 6 ),
	GUARD2( 120 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_02_03[] = {
	LOCK_SET,
	WAIT2( 6 ),
	CMD_JUMP( 12 ),
	SP_CMD( pl2e_sp_cannon, 0, PK_PUNCH, LV_LIGHT, 0 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_02_04[] = {
	WAIT( 10 ),
	WAIT2( 14 ),
	CMD_EXIT,
};

const Uint8 yok2e_cmd_02_05[] = {
	LOCK_SET,
	WAIT2( 6 ),
	IF_AREA2( 0, 0, 0, 0 ),
		GUARD( 120 ),
		IF_AREA2( 0, 0, 0, 0 ),
			WAIT2( 6 ),
			GUARD( 120 ),
		CMD_ELSE,
			IF_EM_ATCK( ATCK_CROUCH_LP ),
				CROUCH,
				PUNCH( LV_LIGHT ),
				KICK( LV_LIGHT ),
				STAND,
			CMD_ELSE,
				IF_RND( 16 ),
					SP_CMD( pl2e_sp_cannon, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ELSE,
					CROUCH,
					KICK( LV_HEAVY ),
					STAND,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 30 ),
			IF_RND( 28 ),
				IF_DIST( 160, 160, 32, 32 ),
					SP_CMD( pl2e_sp_repulsor, 0, PK_PUNCH, LV_HEAVY, 0 ),
				CMD_ELSE,
					GUARD( 30 ),
				CMD_ENDIF,
			CMD_ELSE,
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 24 ),
				IF_RND( 22 ),
					IF_DIST( 160, 160, 32, 32 ),
						SP_CMD( pl2e_sp_repulsor, 0, PK_PUNCH, LV_HEAVY, 0 ),
					CMD_ELSE,
						GUARD( 30 ),
					CMD_ENDIF,
				CMD_ELSE,
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 16 ),
					IF_RND( 16 ),
						IF_DIST( 160, 160, 32, 32 ),
							SP_CMD( pl2e_sp_repulsor, 0, PK_PUNCH, LV_HEAVY, 0 ),
						CMD_ELSE,
							GUARD( 30 ),
						CMD_ENDIF,
					CMD_ELSE,
					CMD_ENDIF,
				CMD_ELSE,
					IF_LEVEL( 8 ),
						IF_RND( 8 ),
							IF_DIST( 160, 160, 32, 32 ),
								SP_CMD( pl2e_sp_repulsor, 0, PK_PUNCH, LV_HEAVY, 0 ),
							CMD_ELSE,
								GUARD( 30 ),
							CMD_ENDIF,
						CMD_ELSE,
						CMD_ENDIF,
					CMD_ELSE,
						IF_RND( 2 ),
							IF_DIST( 160, 160, 32, 32 ),
								SP_CMD( pl2e_sp_repulsor, 0, PK_PUNCH, LV_HEAVY, 0 ),
							CMD_ELSE,
								GUARD( 30 ),
							CMD_ENDIF,
						CMD_ELSE,
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT( 10 ),
	WAIT2( 14 ),
	CMD_EXIT,
};

const Uint8 yok2e_cmd_02_06[] = {
	CMD_EXIT,
};

const Uint8 yok2e_cmd_02_07[] = {
	CMD_EXIT,
};

const Uint8 yok2e_cmd_02_08[] = {
	CMD_EXIT,
};

const Uint8 yok2e_cmd_02_09[] = {
	CMD_EXIT,
};

const Uint8 yok2e_cmd_03_00[] = {
	LOCK_SET,
	WAIT2( 6 ),
	GUARD2( 60 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_03_01[] = {
	LOCK_SET,
	WAIT2( 6 ),
	GUARD2( 120 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_03_02[] = {
	LOCK_SET,
	IF_DIST( 64, 0, 0, 32 ),
		IF_EM_ATCK( 51 ),
			WAIT2( 4 ),
			SP_CMD( pl2e_sp_repulsor, 0, PK_PUNCH, LV_HEAVY, 0 ),
			WAIT2( 4 ),
		CMD_ELSE,
			IF_DIST( 64, 96, 0, 256 ),
				IF_RND( 16 ),
					CMD_JUMP( 3 ),
				CMD_ELSE,
					DASH_F( 128 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_RND( 16 ),
					SJUMP( DIR_FORWARD ),
					SJUMP_FLAG_SET,
						TIMER_SET( 0xb4 ),
						IF_EM_ATCK( pl2e_at_jup_hk ),
							NAG_VFLIP_ADJ4( LV_HEAVY, LEVER_UP ),
						CMD_ELSE,
							IF_EM_ATCK( pl2e_at_jup_hp ),
								NAG_VFLIP_ADJ3( LV_HEAVY, LEVER_UP ),
							CMD_ELSE,
								FLY( LEVER_UP, 384 ),
								IF_RND( 16 ),
									NAG_VFLIP_ADJ4( LV_LIGHT, LEVER_DOWN ),
								CMD_ELSE,
									WAIT2( 8 ),
								CMD_ENDIF,
							CMD_ENDIF,
						CMD_ENDIF,
						JUMP_FLAG_CLR,
					CMD_ELSE,
						JUMP_FLAG_CLR,
					CMD_ENDIF,
				CMD_ELSE,
					WAIT2( 8 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		TIMER_SET( 20 ),
		IF_AREA2( 128, 128, 0, 0 ),
			GUARD2( 30 ),
		CMD_ELSE,
			IF_RND( 16 ),
				DASH_F( 128 ),
			CMD_ELSE,
				SJUMP( DIR_FORWARD ),
				TIMER_SET( 0xc8 ),
				IF_EM_ATCK( ATCK_JUMP_HP ),
					PUNCH( LV_HEAVY ),
				CMD_ELSE,
					IF_EM_ATCK( pl2e_at_jdown_hp ),
						NAG_VFLIP_ADJ3( LV_HEAVY, LEVER_DOWN ),
					CMD_ELSE,
						NAG_VFLIP_ADJ4( LV_LIGHT, LEVER_DOWN ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_03_03[] = {
	LOCK_SET,
	WAIT2( 10 ),
	IF_AREA2( 128, 128, 0, 0 ),
		GUARD2( 120 ),
	CMD_ELSE,
		WAIT2( 4 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_03_04[] = {
	LOCK_SET,
	WAIT( 20 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_03_05[] = {
	LOCK_SET,
	WAIT2( 6 ),
	IF_AREA2( 0, 0, 0, 0 ),
		GUARD2( 120 ),
		IF_AREA2( 0, 0, 0, 0 ),
			WAIT2( 12 ),
			GUARD2( 120 ),
		CMD_ELSE,
			IF_EM_ATCK( pl2e_at_crouch_hp_2 ),
				SP_CMD( pl2e_sp_cannon, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ELSE,
				SP_CMD( pl2e_sp_repulsor, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_DIST( 160, 160, 32, 32 ),
			CROUCH,
			KICK( LV_HEAVY ),
			STAND,
		CMD_ELSE,
			GUARD2( 60 ),
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT( 10 ),
	WAIT2( 14 ),
	CMD_EXIT,
};

const Uint8 yok2e_cmd_03_06[] = {
	CMD_EXIT,
};

const Uint8 yok2e_cmd_03_07[] = {
	CMD_EXIT,
};

const Uint8 yok2e_cmd_03_08[] = {
	CMD_EXIT,
};

const Uint8 yok2e_cmd_03_09[] = {
	CMD_EXIT,
};

const Uint8 yok2e_cmd_04_00[] = {
	LOCK_SET,
	VCOUNTER_SET( 9 ),
	TIMER_SET( 20 ),
	IF_AREA2( 128, 128, 0, 0 ),
		CMD_JUMP( 12 ),
	CMD_ELSE,
		IF_RND( 16 ),
			CMD_JUMP( 12 ),
		CMD_ELSE,
			SJUMP( DIR_FORWARD ),
			CMD_JUMP( 13 ),
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_04_01[] = {
	LOCK_SET,
	VCOUNTER_SET( 9 ),
	WAIT2( 2 ),
	IF_AREA2( 128, 128, 0, 0 ),
		CMD_JUMP( 12 ),
	CMD_ELSE,
		IF_RND( 12 ),
			SP_CMD( pl2e_sp_cannon, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			JUMP( DIR_UP ),
			JUMP_FLAG_SET,
				TIMER_SET( 0xc8 ),
				CMD_JUMP( 13 ),
				IF_HEIGHT( 0, 112 ),
					FLY( LEVER_UP | LEVER_FORWARD, 128 ),
					TIMER_SET( 20 ),
					IF_EM_ATCK( ATCK_JUMP_HK ),
						KICK( LV_HEAVY ),
					CMD_ELSE,
						IF_EM_ATCK( pl2e_at_jdown_hp ),
							NAG_VFLIP_ADJ3( LV_HEAVY, LEVER_DOWN | LEVER_FORWARD ),
						CMD_ELSE,
							IF_EM_ATCK( pl2e_at_jdown_hk ),
								NAG_VFLIP_ADJ4( LV_LIGHT, LEVER_DOWN ),
							CMD_ELSE,
							CMD_ENDIF,
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_04_02[] = {
	LOCK_SET,
	VCOUNTER_SET( 9 ),
	WAIT2( 2 ),
	GUARD( 120 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_04_03[] = {
	LOCK_SET,
	VCOUNTER_SET( 9 ),
	JUMP( DIR_FORWARD ),
	JUMP_FLAG_SET,
		IF_HEIGHT( 0, 104 ),
			FLY( LEVER_FORWARD, 512 ),
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ELSE,
		GUARD( 120 ),
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	IF_LEVEL( 16 ),
		CMD_JUMP( 2 ),
	CMD_ELSE,
		IF_RND( 8 ),
			CMD_JUMP( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_04_04[] = {
	LOCK_SET,
	VCOUNTER_SET( 9 ),
	WAIT2( 2 ),
	JUMP( DIR_FORWARD ),
	JUMP_FLAG_SET,
		TIMER_SET( 120 ),
		IF_HEIGHT( 0xff, 112 ),
			IF_LEVEL( 8 ),
				SP_CMD( pl2e_sp_bomb, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				IF_AREA( 64, 0, 64, 0 ),
					NAG_VFLIP_ADJ4( LV_LIGHT, LEVER_DOWN ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	IF_LEVEL( 16 ),
		CMD_JUMP( 2 ),
	CMD_ELSE,
		IF_RND( 8 ),
			CMD_JUMP( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_04_05[] = {
	LOCK_SET,
	VCOUNTER_SET( 9 ),
	WAIT2( 2 ),
	IF_AREA2( 0, 0, 0, 0 ),
		GUARD( 120 ),
		IF_AREA2( 0, 0, 0, 0 ),
			WAIT2( 12 ),
			GUARD( 120 ),
		CMD_ELSE,
			CMD_JUMP( 13 ),
			IF_EM_ATCK( ATCK_CROUCH_MK ),
				CROUCH,
				KICK( LV_LIGHT ),
				KICK( LV_HEAVY ),
				STAND,
			CMD_ELSE,
				IF_RND( 16 ),
					CROUCH,
					PUNCH( LV_HEAVY ),
					STAND,
				CMD_ELSE,
					SP_CMD( pl2e_sp_bomb, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		CMD_JUMP( 13 ),
		DASH_ATCK( ATCK_CROUCH_LP ),
			CANCEL_SET2,
				CROUCH,
				PUNCH( LV_LIGHT ),
				PUNCH( LV_LIGHT ),
				STAND,
				SP_CMD( pl2e_sp_repulsor, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CANCEL_CLR,
			CMD_ELSE,
				GUARD( 30 ),
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			GUARD( 30 ),
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT( 10 ),
	WAIT2( 14 ),
	CMD_EXIT,
};

const Uint8 yok2e_cmd_04_06[] = {
	IF_DIST( 240, 48, 0, 0 ),
		TIMER_SET( 30 ),
		IF_AREA2( 128, 128, 0, 0 ),
			GUARD( 30 ),
		CMD_ELSE,
			SP_CMD( pl2e_sp_cannon, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_DIST( 32, 48, 272, 0 ),
			SJUMP( DIR_FORWARD ),
			SJUMP_FLAG_SET,
				IF_LEVEL( 24 ),
					TIMER_SET( 100 ),
					IF_HEIGHT( 0, 256 ),
						NAG_VFLIP_ADJ4( LV_LIGHT, LEVER_DOWN ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_RND( 16 ),
						CMD_JUMP( 12 ),
					CMD_ENDIF,
				CMD_ENDIF,
				JUMP_FLAG_CLR,
			CMD_ELSE,
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			IF_AREA2( 0, 0, 0, 0 ),
				GUARD( 30 ),
			CMD_ELSE,
				TIMER_SET( 10 ),
				IF_AREA2( 0, 0, 0, 0 ),
					GUARD( 30 ),
				CMD_ELSE,
					JUMP( DIR_FORWARD ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_04_07[] = {
	CMD_EXIT,
};

const Uint8 yok2e_cmd_04_08[] = {
	CMD_EXIT,
};

const Uint8 yok2e_cmd_04_09[] = {
	CMD_EXIT,
};

const Uint8 yok2e_cmd_05_00[] = {
	LOCK_SET,
	VCOUNTER_SET( 9 ),
	IF_LEVEL( 27 ),
		TIMER_SET( 5 ),
		IF_EM_ATCK( ATCK_STAND_HK ),
			KICK( LV_HEAVY ),
		CMD_ELSE,
			CMD_JUMP( 12 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 20 ),
			IF_RND( 24 ),
				TIMER_SET( 5 ),
				IF_EM_ATCK( ATCK_STAND_HK ),
					KICK( LV_HEAVY ),
				CMD_ELSE,
					CMD_JUMP( 12 ),
				CMD_ENDIF,
			CMD_ELSE,
				WAIT2( 12 ),
				GUARD( 30 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 12 ),
				IF_RND( 16 ),
					TIMER_SET( 5 ),
					IF_EM_ATCK( ATCK_STAND_HK ),
						KICK( LV_HEAVY ),
					CMD_ELSE,
						TIMER2_SET( 80 ),
					CMD_ENDIF,
				CMD_ELSE,
					WAIT2( 14 ),
					GUARD( 30 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 6 ),
					IF_RND( 8 ),
						TIMER_SET( 5 ),
						IF_EM_ATCK( ATCK_STAND_HK ),
							KICK( LV_HEAVY ),
						CMD_ELSE,
							TIMER2_SET( 80 ),
						CMD_ENDIF,
					CMD_ELSE,
						WAIT( 10 ),
						GUARD( 30 ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_RND( 16 ),
						WAIT2( 14 ),
						GUARD( 30 ),
					CMD_ELSE,
						WALK_B( 64 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_05_01[] = {
	LOCK_SET,
	VCOUNTER_SET( 9 ),
	IF_LEVEL( 27 ),
		TIMER_SET( 5 ),
		TIMER_SET( 5 ),
		IF_EM_ATCK( 51 ),
			IF_RND( 16 ),
				SP_CMD( pl2e_sp_repulsor, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ELSE,
				SP_CMD( pl2e_sp_repulsor, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ENDIF,
		CMD_ELSE,
			CMD_JUMP( 12 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 20 ),
			IF_RND( 24 ),
				TIMER_SET( 5 ),
				IF_EM_ATCK( 51 ),
					IF_RND( 16 ),
						SP_CMD( pl2e_sp_repulsor, 0, PK_PUNCH, LV_HEAVY, 0 ),
					CMD_ELSE,
						SP_CMD( pl2e_sp_repulsor, 0, PK_PUNCH, LV_HEAVY, 0 ),
					CMD_ENDIF,
				CMD_ELSE,
					CMD_JUMP( 12 ),
				CMD_ENDIF,
			CMD_ELSE,
				WAIT2( 12 ),
				GUARD( 30 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 12 ),
				IF_RND( 16 ),
					TIMER_SET( 5 ),
					IF_EM_ATCK( 51 ),
						IF_RND( 16 ),
							SP_CMD( pl2e_sp_repulsor, 0, PK_PUNCH, LV_HEAVY, 0 ),
						CMD_ELSE,
							SP_CMD( pl2e_sp_repulsor, 0, PK_PUNCH, LV_HEAVY, 0 ),
						CMD_ENDIF,
					CMD_ELSE,
						CMD_JUMP( 12 ),
					CMD_ENDIF,
				CMD_ELSE,
					WAIT2( 14 ),
					GUARD( 30 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 6 ),
					IF_RND( 8 ),
						TIMER_SET( 5 ),
						IF_EM_ATCK( 51 ),
							IF_RND( 16 ),
								SP_CMD( pl2e_sp_repulsor, 0, PK_PUNCH, LV_HEAVY, 0 ),
							CMD_ELSE,
								SP_CMD( pl2e_sp_repulsor, 0, PK_PUNCH, LV_HEAVY, 0 ),
							CMD_ENDIF,
						CMD_ELSE,
							CMD_JUMP( 12 ),
						CMD_ENDIF,
					CMD_ELSE,
						WAIT( 10 ),
						GUARD( 30 ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_RND( 16 ),
						WAIT2( 14 ),
						GUARD( 30 ),
					CMD_ELSE,
						WALK_B( 64 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_05_02[] = {
	LOCK_SET,
	VCOUNTER_SET( 9 ),
	TIMER_SET( 5 ),
	IF_EM_ATCK( ATCK_CROUCH_MP ),
		CROUCH,
		PUNCH( LV_LIGHT ),
		STAND,
	CMD_ELSE,
		TIMER2_SET( 80 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_05_03[] = {
	LOCK_SET,
	VCOUNTER_SET( 9 ),
	CMD_JUMP( 12 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_05_04[] = {
	LOCK_SET,
	DASH_B( 96 ),
	IF_RND( 10 ),
		PUNCH( LV_HEAVY ),
	CMD_ELSE,
		GUARD( 30 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_05_05[] = {
	CMD_EXIT,
};

const Uint8 yok2e_cmd_05_06[] = {
	CMD_EXIT,
};

const Uint8 yok2e_cmd_05_07[] = {
	CMD_EXIT,
};

const Uint8 yok2e_cmd_05_08[] = {
	CMD_EXIT,
};

const Uint8 yok2e_cmd_05_09[] = {
	CMD_EXIT,
};

const Uint8 yok2e_cmd_06_00[] = {
	LOCK_SET,
	JUMP( DIR_BACK ),
	LOCK_CLR,
	GUARD( 120 ),
	CMD_EXIT,
};

const Uint8 yok2e_cmd_06_01[] = {
	LOCK_SET,
	IF_EM_ATCK( ATCK_STAND_LK ),
		KICK( LV_LIGHT ),
	CMD_ELSE,
		DASH_B( 128 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_06_02[] = {
	LOCK_SET,
	IF_RND( 16 ),
		DASH_B( 128 ),
	CMD_ELSE,
		WALK_B( 64 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_06_03[] = {
	LOCK_SET,
	JUMP( DIR_UP ),
	JUMP_FLAG_SET,
		TIMER_SET( 120 ),
		IF_HEIGHT( 0, 144 ),
			FLY( LEVER_BACK, 256 ),
		CMD_ELSE,
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ELSE,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_06_04[] = {
	LOCK_SET,
	SJUMP( DIR_BACK ),
	SJUMP_FLAG_SET,
		TIMER_SET( 0xc8 ),
		IF_HEIGHT( 0, 288 ),
			FLY( LEVER_UP, 512 ),
			NAG_VFLIP_ADJ4( LV_LIGHT, LEVER_DOWN ),
		CMD_ELSE,
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ELSE,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_06_05[] = {
	CMD_EXIT,
};

const Uint8 yok2e_cmd_06_06[] = {
	CMD_EXIT,
};

const Uint8 yok2e_cmd_06_07[] = {
	CMD_EXIT,
};

const Uint8 yok2e_cmd_06_08[] = {
	CMD_EXIT,
};

const Uint8 yok2e_cmd_06_09[] = {
	CMD_EXIT,
};

const Uint8 yok2e_cmd_07_00[] = {
	LOCK_SET,
	TIMER_SET( 20 ),
	IF_AREA2( 208, 128, 0, 0 ),
		CMD_JUMP( 13 ),
	CMD_ELSE,
		IF_RND( 12 ),
			WAIT( 1 ),
		CMD_ELSE,
			SJUMP( DIR_UP ),
			SJUMP_FLAG_SET,
				CMD_JUMP( 12 ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
			CMD_JUMP( 13 ),
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_07_01[] = {
	LOCK_SET,
	GUARD( 30 ),
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_07_02[] = {
	LOCK_SET,
	TIMER_SET( 10 ),
	IF_AREA2( 0, 0, 0, 0 ),
		CMD_JUMP( 12 ),
		TIMER_SET( 5 ),
		IF_AREA2( 0, 0, 0, 0 ),
			CMD_JUMP( 12 ),
			TIMER_SET( 3 ),
			IF_AREA2( 0, 0, 0, 0 ),
				CMD_JUMP( 12 ),
			CMD_ELSE,
				IF_LEVEL( 28 ),
					CMD_JUMP( 4 ),
				CMD_ELSE,
					IF_LEVEL( 22 ),
						CMD_JUMP( 1 ),
					CMD_ELSE,
						IF_LEVEL( 12 ),
							CMD_JUMP( 6 ),
						CMD_ELSE,
							IF_LEVEL( 8 ),
								WAIT2( 2 ),
								CMD_JUMP( 6 ),
							CMD_ELSE,
								IF_RND( 16 ),
									PUNCH( LV_HEAVY ),
								CMD_ENDIF,
							CMD_ENDIF,
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 28 ),
				CMD_JUMP( 4 ),
			CMD_ELSE,
				IF_LEVEL( 22 ),
					CMD_JUMP( 1 ),
				CMD_ELSE,
					IF_LEVEL( 12 ),
						CMD_JUMP( 6 ),
					CMD_ELSE,
						IF_LEVEL( 8 ),
							WAIT2( 2 ),
							CMD_JUMP( 6 ),
						CMD_ELSE,
							IF_RND( 16 ),
								PUNCH( LV_HEAVY ),
							CMD_ENDIF,
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 28 ),
			CMD_JUMP( 4 ),
		CMD_ELSE,
			IF_LEVEL( 22 ),
				CMD_JUMP( 1 ),
			CMD_ELSE,
				IF_LEVEL( 12 ),
					CMD_JUMP( 6 ),
				CMD_ELSE,
					IF_LEVEL( 8 ),
						WAIT2( 2 ),
						CMD_JUMP( 6 ),
					CMD_ELSE,
						IF_RND( 16 ),
							PUNCH( LV_HEAVY ),
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_07_03[] = {
	LOCK_SET,
	IF_AREA( 240, 0, 0, 0 ),
		CMD_JUMP( 11 ),
	CMD_ELSE,
		CMD_JUMP( 6 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_07_04[] = {
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_07_05[] = {
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_07_06[] = {
	CMD_EXIT,
};

const Uint8 yok2e_cmd_07_07[] = {
	CMD_EXIT,
};

const Uint8 yok2e_cmd_07_08[] = {
	CMD_EXIT,
};

const Uint8 yok2e_cmd_07_09[] = {
	CMD_EXIT,
};

const Uint8 yok2e_cmd_08_00[] = {
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_08_01[] = {
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_08_02[] = {
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_08_03[] = {
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_08_04[] = {
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_08_05[] = {
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
};

const Uint8 yok2e_cmd_08_06[] = {
	CMD_EXIT,
};

const Uint8 yok2e_cmd_08_07[] = {
	CMD_EXIT,
};

const Uint8 yok2e_cmd_08_08[] = {
	CMD_EXIT,
};

const Uint8 yok2e_cmd_08_09[] = {
	CMD_EXIT,
	LOCK_SET,
	SJUMP( DIR_BACK ),
	SJUMP_FLAG_SET,
		TIMER_SET( 0xc8 ),
		IF_HEIGHT( 0, 304 ),
			NAG_VFLIP_ADJ4( LV_LIGHT, LEVER_DOWN ),
		CMD_ELSE,
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ELSE,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	IF_EM_ATCK( ATCK_STAND_LP ),
		CANCEL_SET,
			PUNCH( LV_LIGHT ),
			PUNCH( LV_LIGHT ),
			CANCEL_CLR,
		CMD_ELSE,
			GUARD( 30 ),
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		IF_EM_ATCK( ATCK_CROUCH_HK ),
			CROUCH,
			KICK( LV_HEAVY ),
			STAND,
		CMD_ENDIF,
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
	LOCK_SET,
	SJUMP( DIR_UP ),
	SJUMP_FLAG_SET,
		TIMER_SET( 100 ),
		IF_HEIGHT( 0, 304 ),
			SP_CMD( pl2e_sp_bomb, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
		CANCEL_SET2,
			NAG_VFLIP_ADJ4( LV_LIGHT, LEVER_DOWN ),
			IF_EM_ATCK( ATCK_JUMP_HK ),
				KICK( LV_HEAVY ),
			CMD_ENDIF,
			CANCEL_CLR,
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ELSE,
		GUARD( 30 ),
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	IF_AREA( 192, 128, 0, 0 ),
		KICK( LV_LIGHT ),
	CMD_ELSE,
		SP_CMD( pl2e_sp_repulsor, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ENDIF,
	LOCK_CLR,
	CMD_EXIT,
	LOCK_SET,
	GUARD( 30 ),
	LOCK_CLR,
	CMD_EXIT,
	LOCK_SET,
	GUARD( 30 ),
	SP_CMD( pl2e_sp_repulsor, 0, PK_PUNCH, LV_LIGHT, 0 ),
	LOCK_CLR,
	CMD_EXIT,
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
	CMD_EXIT,
	CMD_EXIT,
	CMD_EXIT,
	CMD_EXIT,
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
	CMD_EXIT,
	CMD_EXIT,
	CMD_EXIT,
	CMD_EXIT,
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
	CMD_EXIT,
	CMD_EXIT,
	CMD_EXIT,
	CMD_EXIT,
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
	CMD_EXIT,
	CMD_EXIT,
	CMD_EXIT,
	CMD_EXIT,
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
	CMD_EXIT,
	CMD_EXIT,
	CMD_EXIT,
	CMD_EXIT,
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
	CMD_EXIT,
	CMD_EXIT,
	CMD_EXIT,
	CMD_EXIT,
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
	LOCK_SET,
	LOCK_CLR,
	CMD_EXIT,
	CMD_EXIT,
	CMD_EXIT,
	CMD_EXIT,
	CMD_EXIT,
};

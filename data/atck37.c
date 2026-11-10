/* Jin (character 37): the computer's attack lists.
   What it does once a waiting list's CMD_NEXT has sent it to attack. PLAYER_WORK's cpu_adrs[1]
   points here.

   atck37_tbl     one record (CPU_DATA): the offsets of the three tables.
   atck37_rate    the chance, out of 32, that a list's CMD_NEXT2 starts another attack list, by
                  the level of play.
   atck37_sel     for each of the computer's levels (cpu_lv), 32 picks; one is taken at random.
   atck37_cmd     the lists: a pick is a number in this table.
   atck37_cmd_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name a
                  list of lib37.c. */
#include "player.h"
#include "cpu_cmd.h"

extern const Uint8 *const atck37_sel[], *const atck37_cmd[];
extern const Uint8 atck37_rate[], atck37_sel_0[], atck37_sel_1[], atck37_sel_2[], atck37_sel_3[],
	atck37_sel_4[], atck37_sel_5[], atck37_sel_6[], atck37_sel_7[], atck37_cmd_00[], atck37_cmd_04[],
	atck37_cmd_08[], atck37_cmd_12[], atck37_cmd_16[], atck37_cmd_25[], atck37_cmd_26[],
	atck37_cmd_27[], atck37_cmd_28[], atck37_cmd_29[], atck37_cmd_30[], atck37_cmd_31[];

const CPU_DATA atck37_tbl[] = {
	{ ( Sint32 )atck37_rate, ( Sint32 )atck37_sel, ( Sint32 )atck37_cmd },
};

const Uint8 atck37_rate[] = {
	 10,  10,  13,  13,  15,  15,  17,  17,  18,  19,  20,  21,  22,  23,  24,  25,
	 25,  26,  27,  27,  27,  27,  27,  27,  28,  28,  29,  29,  30,  30,  31,  31,
};

const Uint8 *const atck37_sel[] = {
	atck37_sel_0, atck37_sel_1, atck37_sel_2, atck37_sel_3, atck37_sel_4, atck37_sel_5,
	atck37_sel_6, atck37_sel_7,
};

const Uint8 atck37_sel_0[] = {
	 12,  12,  12,  12,  20,  20,  20,  20,  12,  12,  12,  12,  12,  12,  12,  12,
	 21,  21,  21,  21,  21,  21,  21,  21,  13,  14,  15,  16,  17,  18,  19,  20,
};

const Uint8 atck37_sel_1[] = {
	 12,  12,  12,  12,  20,  20,  20,  20,  12,  12,  12,  12,  12,  12,  12,  12,
	 21,  21,  21,  21,  21,  21,  21,  21,  13,  14,  15,  16,  17,  18,  19,  20,
};

const Uint8 atck37_sel_2[] = {
	 12,  12,  12,  12,  20,  20,  20,  20,  21,  21,  21,  13,  13,  13,  14,  14,
	 21,  21,  21,  15,  15,  15,  14,  14,  13,  14,  15,  16,  17,  18,  19,  20,
};

const Uint8 atck37_sel_3[] = {
	 12,  12,  12,  12,  20,  20,  20,  20,  21,  21,  21,  13,  13,  13,  14,  14,
	 21,  21,  21,  15,  15,  15,  14,  14,  13,  14,  15,  16,  17,  18,  19,  20,
};

const Uint8 atck37_sel_4[] = {
	 12,  12,  12,  12,  20,  20,  20,  20,  21,  21,  21,  21,  16,  16,  17,  17,
	 21,  21,  21,  21,  18,  18,  19,  19,  13,  14,  15,  16,  17,  18,  19,  20,
};

const Uint8 atck37_sel_5[] = {
	 12,  12,  12,  12,  20,  20,  20,  20,  21,  21,  21,  21,  16,  16,  17,  17,
	 21,  21,  21,  21,  18,  18,  19,  19,  13,  14,  15,  16,  17,  18,  19,  20,
};

const Uint8 atck37_sel_6[] = {
	 12,  12,  12,  12,  20,  20,  20,  20,  21,  21,  21,  21,  16,  16,  17,  17,
	 21,  21,  21,  21,  18,  18,  19,  19,  13,  14,  15,  16,  17,  18,  19,  20,
};

const Uint8 atck37_sel_7[] = {
	 12,  12,  12,  12,  20,  20,  20,  20,  21,  21,  21,  21,  16,  16,  17,  17,
	 21,  21,  21,  21,  18,  18,  19,  19,  13,  14,  15,  16,  17,  18,  19,  20,
};

const Uint8 *const atck37_cmd[] = {
	atck37_cmd_00, atck37_cmd_00, atck37_cmd_00, atck37_cmd_00, atck37_cmd_04, atck37_cmd_04,
	atck37_cmd_04, atck37_cmd_04, atck37_cmd_08, atck37_cmd_08, atck37_cmd_08, atck37_cmd_08,
	atck37_cmd_12, atck37_cmd_12, atck37_cmd_12, atck37_cmd_12, atck37_cmd_16, atck37_cmd_16,
	atck37_cmd_16, atck37_cmd_16, atck37_cmd_16, atck37_cmd_16, atck37_cmd_16, atck37_cmd_16,
	atck37_cmd_16, atck37_cmd_25, atck37_cmd_26, atck37_cmd_27, atck37_cmd_28, atck37_cmd_29,
	atck37_cmd_30, atck37_cmd_31,
};

const Uint8 atck37_cmd_00[] = {
	CMD_JUMP( 11 ),
	CMD_JUMP( 39 ),
	CMD_JUMP( 37 ),
	IF_DIST( 96, 0, 96, 0 ),
		IF_DIST( 48, 0, 48, 0 ),
			IF_LEVEL( 24 ),
				CMD_JUMP( 9 ),
			CMD_ELSE,
				IF_LEVEL( 16 ),
					CMD_JUMP( 8 ),
				CMD_ELSE,
					IF_LEVEL( 8 ),
						CMD_JUMP( 7 ),
					CMD_ELSE,
						CMD_JUMP( 10 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 24 ),
				DASH_F( 128 ),
				CMD_JUMP( 9 ),
			CMD_ELSE,
				IF_LEVEL( 16 ),
					DASH_F( 128 ),
					CMD_JUMP( 8 ),
				CMD_ELSE,
					IF_LEVEL( 8 ),
						WALK_F( 128 ),
						CMD_JUMP( 7 ),
					CMD_ELSE,
						WALK_F( 128 ),
						CMD_JUMP( 10 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 24 ),
			DASH_F( 288 ),
			CMD_JUMP( 9 ),
		CMD_ELSE,
			IF_LEVEL( 16 ),
				DASH_F( 288 ),
				CMD_JUMP( 8 ),
			CMD_ELSE,
				IF_LEVEL( 8 ),
					WALK_F( 288 ),
					CMD_JUMP( 7 ),
				CMD_ELSE,
					WALK_F( 288 ),
					CMD_JUMP( 10 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck37_cmd_04[] = {
	CMD_JUMP( 11 ),
	CMD_JUMP( 39 ),
	CMD_JUMP( 37 ),
	IF_DIST( 96, 0, 96, 0 ),
		IF_DIST( 48, 0, 48, 0 ),
			IF_LEVEL( 24 ),
				JUMP_RND( 19, 19, 19, 57 ),
				CMD_JUMP( 9 ),
			CMD_ELSE,
				IF_LEVEL( 16 ),
					JUMP_RND( 19, 57, 57, 16 ),
					CMD_JUMP( 8 ),
				CMD_ELSE,
					IF_LEVEL( 8 ),
						JUMP_RND( 57, 57, 16, 16 ),
						CMD_JUMP( 7 ),
					CMD_ELSE,
						CMD_JUMP( 16 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 24 ),
				JUMP_RND( 19, 19, 19, 57 ),
				CMD_JUMP( 9 ),
			CMD_ELSE,
				IF_LEVEL( 16 ),
					JUMP_RND( 19, 57, 57, 16 ),
					CMD_JUMP( 8 ),
				CMD_ELSE,
					IF_LEVEL( 8 ),
						JUMP_RND( 57, 57, 16, 16 ),
						CMD_JUMP( 7 ),
					CMD_ELSE,
						CMD_JUMP( 16 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 24 ),
			JUMP_RND( 19, 19, 19, 57 ),
			CMD_JUMP( 9 ),
		CMD_ELSE,
			IF_LEVEL( 16 ),
				JUMP_RND( 19, 57, 57, 16 ),
				CMD_JUMP( 8 ),
			CMD_ELSE,
				IF_LEVEL( 8 ),
					JUMP_RND( 57, 57, 16, 16 ),
					CMD_JUMP( 7 ),
				CMD_ELSE,
					CMD_JUMP( 16 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck37_cmd_08[] = {
	CMD_JUMP( 11 ),
	CMD_JUMP( 39 ),
	CMD_JUMP( 37 ),
	IF_DIST( 96, 0, 96, 0 ),
		IF_DIST( 48, 0, 48, 0 ),
			IF_LEVEL( 24 ),
				JUMP_RND( 4, 4, 6, 6 ),
			CMD_ELSE,
				IF_LEVEL( 16 ),
					JUMP_RND( 1, 3, 3, 6 ),
				CMD_ELSE,
					IF_LEVEL( 8 ),
						JUMP_RND( 1, 1, 3, 3 ),
					CMD_ELSE,
						CMD_JUMP( 1 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 24 ),
				JUMP_RND( 4, 4, 5, 5 ),
			CMD_ELSE,
				IF_LEVEL( 16 ),
					JUMP_RND( 1, 55, 55, 5 ),
				CMD_ELSE,
					IF_LEVEL( 8 ),
						JUMP_RND( 1, 1, 55, 55 ),
					CMD_ELSE,
						CMD_JUMP( 55 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 24 ),
			JUMP_RND( 4, 4, 5, 5 ),
		CMD_ELSE,
			IF_LEVEL( 16 ),
				JUMP_RND( 56, 56, 56, 4 ),
			CMD_ELSE,
				IF_LEVEL( 8 ),
					JUMP_RND( 1, 1, 56, 56 ),
				CMD_ELSE,
					CMD_JUMP( 56 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck37_cmd_12[] = {
	CMD_JUMP( 11 ),
	CMD_JUMP( 39 ),
	CMD_JUMP( 37 ),
	IF_DIST( 104, 0, 104, 0 ),
		IF_DIST( 69, 0, 69, 0 ),
			IF_DIST( 40, 0, 40, 0 ),
				IF_LEVEL( 24 ),
					CMD_JUMP( 23 ),
				CMD_ELSE,
					IF_LEVEL( 16 ),
						CMD_JUMP( 22 ),
					CMD_ELSE,
						IF_LEVEL( 8 ),
							WAIT2( 10 ),
							CMD_JUMP( 22 ),
						CMD_ELSE,
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 24 ),
					DASH_F( 48 ),
					CMD_JUMP( 23 ),
				CMD_ELSE,
					IF_LEVEL( 16 ),
						DASH_F( 48 ),
						CMD_JUMP( 22 ),
					CMD_ELSE,
						IF_LEVEL( 8 ),
							CMD_JUMP( 16 ),
							CMD_JUMP( 22 ),
						CMD_ELSE,
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 24 ),
				DASH_F( 8 ),
				JUMP_RND( 19, 19, 19, 57 ),
				CMD_JUMP( 22 ),
			CMD_ELSE,
				IF_LEVEL( 16 ),
					DASH_F( 8 ),
					JUMP_RND( 19, 57, 57, 16 ),
					CMD_JUMP( 22 ),
				CMD_ELSE,
					IF_LEVEL( 8 ),
						DASH_F( 8 ),
						JUMP_RND( 57, 16, 16, 16 ),
						WAIT2( 10 ),
						CMD_JUMP( 22 ),
					CMD_ELSE,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 24 ),
			DASH_F( 96 ),
			JUMP_RND( 19, 19, 19, 57 ),
			CMD_JUMP( 22 ),
		CMD_ELSE,
			IF_LEVEL( 16 ),
				DASH_F( 96 ),
				JUMP_RND( 19, 57, 57, 16 ),
				CMD_JUMP( 22 ),
			CMD_ELSE,
				IF_LEVEL( 8 ),
					DASH_F( 96 ),
					JUMP_RND( 57, 16, 16, 16 ),
					WAIT2( 10 ),
					CMD_JUMP( 22 ),
				CMD_ELSE,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_EXIT,
};

const Uint8 atck37_cmd_16[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck37_cmd_25[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck37_cmd_26[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck37_cmd_27[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck37_cmd_28[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck37_cmd_29[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck37_cmd_30[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

const Uint8 atck37_cmd_31[] = {
	CMD_NEXT2,
	CMD_EXIT,
};

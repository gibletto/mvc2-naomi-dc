/* Abyss (first form) (character 18): the computer's shared lists.
   The lists that the others go to with CMD_JUMP (or JUMP_RND, JUMP_EM, JUMP_LEVEL, JUMP_DIST)
   and come back from with JUMP_RET. PLAYER_WORK's cpu_ex_adrs points here.

   lib18_tbl     the lists, by number.
   lib18_cmd_NN  a list, in the commands of cpu_cmd.h. A jump in one names another of them. */
#include "player.h"
#include "pl18_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const lib18_tbl[];
extern const Uint8 lib18_cmd_00[], lib18_cmd_01[], lib18_cmd_02[], lib18_cmd_03[], lib18_cmd_04[],
	lib18_cmd_05[], lib18_cmd_06[], lib18_cmd_07[], lib18_cmd_08[], lib18_cmd_09[], lib18_cmd_10[],
	lib18_cmd_11[], lib18_cmd_12[], lib18_cmd_13[], lib18_cmd_14[], lib18_cmd_15[], lib18_cmd_16[],
	lib18_cmd_17[], lib18_cmd_18[], lib18_cmd_19[], lib18_cmd_20[], lib18_cmd_21[], lib18_cmd_22[],
	lib18_cmd_23[], lib18_cmd_24[], lib18_cmd_25[], lib18_cmd_26[], lib18_cmd_27[], lib18_cmd_28[],
	lib18_cmd_29[], lib18_cmd_30[], lib18_cmd_31[], lib18_cmd_32[], lib18_cmd_33[], lib18_cmd_34[],
	lib18_cmd_35[], lib18_cmd_36[], lib18_cmd_37[], lib18_cmd_38[], lib18_cmd_39[], lib18_cmd_40[],
	lib18_cmd_41[], lib18_cmd_42[], lib18_cmd_43[], lib18_cmd_44[], lib18_cmd_45[], lib18_cmd_46[],
	lib18_cmd_47[], lib18_cmd_48[], lib18_cmd_49[], lib18_cmd_50[], lib18_cmd_51[], lib18_cmd_52[],
	lib18_cmd_53[], lib18_cmd_54[], lib18_cmd_55[], lib18_cmd_56[], lib18_cmd_57[], lib18_cmd_58[],
	lib18_cmd_59[], lib18_cmd_60[], lib18_cmd_61[];

const Uint8 *const lib18_tbl[] = {
	lib18_cmd_00, lib18_cmd_01, lib18_cmd_02, lib18_cmd_03, lib18_cmd_04, lib18_cmd_05,
	lib18_cmd_06, lib18_cmd_07, lib18_cmd_08, lib18_cmd_09, lib18_cmd_10, lib18_cmd_11,
	lib18_cmd_12, lib18_cmd_13, lib18_cmd_14, lib18_cmd_15, lib18_cmd_16, lib18_cmd_17,
	lib18_cmd_18, lib18_cmd_19, lib18_cmd_20, lib18_cmd_21, lib18_cmd_22, lib18_cmd_23,
	lib18_cmd_24, lib18_cmd_25, lib18_cmd_26, lib18_cmd_27, lib18_cmd_28, lib18_cmd_29,
	lib18_cmd_30, lib18_cmd_31, lib18_cmd_32, lib18_cmd_33, lib18_cmd_34, lib18_cmd_35,
	lib18_cmd_36, lib18_cmd_37, lib18_cmd_38, lib18_cmd_39, lib18_cmd_40, lib18_cmd_41,
	lib18_cmd_42, lib18_cmd_43, lib18_cmd_44, lib18_cmd_45, lib18_cmd_46, lib18_cmd_47,
	lib18_cmd_48, lib18_cmd_49, lib18_cmd_50, lib18_cmd_51, lib18_cmd_52, lib18_cmd_53,
	lib18_cmd_54, lib18_cmd_55, lib18_cmd_56, lib18_cmd_57, lib18_cmd_58, lib18_cmd_59,
	lib18_cmd_60, lib18_cmd_61, 0,
};

const Uint8 lib18_cmd_00[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_01[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_02[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_03[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_04[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_05[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_06[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_07[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_08[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_09[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_10[] = {
	STAND,
	IF_GAUGE( 1 ),
		IF_LEVEL( 28 ),
			IF_RND( 20 ),
				SP_CMD( pl18_sp_thunder, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 20 ),
				IF_RND( 14 ),
					SP_CMD( pl18_sp_thunder, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 14 ),
					IF_LEVEL( 6 ),
						IF_RND( 8 ),
							SP_CMD( pl18_sp_thunder, 0, PK_PUNCH, LV_LIGHT, 0 ),
						CMD_ENDIF,
					CMD_ELSE,
						IF_RND( 4 ),
							SP_CMD( pl18_sp_thunder, 0, PK_PUNCH, LV_LIGHT, 0 ),
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
		WAIT2( 6 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	JUMP_RET,
};

const Uint8 lib18_cmd_11[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_12[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_13[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_14[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_15[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_16[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_17[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_18[] = {
	IF_LEVEL( 31 ),
		IF_RND( 30 ),
			WAIT( 1 ),
		CMD_ELSE,
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 30 ),
			IF_RND( 28 ),
				WAIT( 1 ),
			CMD_ELSE,
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 29 ),
				IF_RND( 26 ),
					WAIT( 1 ),
				CMD_ELSE,
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 28 ),
					IF_RND( 25 ),
						WAIT( 1 ),
					CMD_ELSE,
					CMD_ENDIF,
				CMD_ELSE,
					IF_LEVEL( 27 ),
						IF_RND( 24 ),
							WAIT( 1 ),
						CMD_ELSE,
						CMD_ENDIF,
					CMD_ELSE,
						IF_LEVEL( 26 ),
							IF_RND( 23 ),
								WAIT( 1 ),
							CMD_ELSE,
							CMD_ENDIF,
						CMD_ELSE,
							IF_LEVEL( 25 ),
								IF_RND( 22 ),
									WAIT( 1 ),
								CMD_ELSE,
								CMD_ENDIF,
							CMD_ELSE,
								IF_LEVEL( 24 ),
									IF_RND( 21 ),
										WAIT( 1 ),
									CMD_ELSE,
									CMD_ENDIF,
								CMD_ELSE,
								CMD_ENDIF,
							CMD_ENDIF,
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib18_cmd_19[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_20[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_21[] = {
	STAND,
	IF_GAUGE( 1 ),
		IF_LEVEL( 28 ),
			IF_RND( 20 ),
				SP_CMD( pl18_sp_ball, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 20 ),
				IF_RND( 14 ),
					SP_CMD( pl18_sp_ball, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 14 ),
					IF_LEVEL( 8 ),
						IF_RND( 4 ),
							SP_CMD( pl18_sp_ball, 0, PK_PUNCH, LV_LIGHT, 0 ),
						CMD_ENDIF,
					CMD_ELSE,
						IF_RND( 4 ),
							SP_CMD( pl18_sp_ball, 0, PK_PUNCH, LV_LIGHT, 0 ),
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
		WAIT2( 6 ),
	CMD_ENDIF,
	STAND,
	WAIT2( 2 ),
	JUMP_RET,
};

const Uint8 lib18_cmd_22[] = {
	TIMER_SET( 5 ),
	IF_AREA2( 0, 0, 0, 0 ),
		IF_AREA( 256, 0, 0, 0 ),
			CMD_JUMP( 19 ),
			TIMER_SET( 5 ),
			IF_AREA2( 0, 0, 0, 0 ),
				CMD_JUMP( 19 ),
			CMD_ELSE,
				IF_LEVEL( 28 ),
					IF_AREA( 80, 128, 0, 0 ),
						CMD_JUMP( 21 ),
					CMD_ELSE,
					CMD_ENDIF,
				CMD_ELSE,
					IF_LEVEL( 22 ),
						IF_RND( 24 ),
							IF_AREA( 80, 128, 0, 0 ),
								CMD_JUMP( 21 ),
							CMD_ELSE,
							CMD_ENDIF,
						CMD_ELSE,
						CMD_ENDIF,
					CMD_ELSE,
						IF_LEVEL( 12 ),
							IF_RND( 16 ),
								IF_AREA( 80, 128, 0, 0 ),
									CMD_JUMP( 21 ),
								CMD_ELSE,
								CMD_ENDIF,
							CMD_ELSE,
							CMD_ENDIF,
						CMD_ELSE,
							IF_LEVEL( 8 ),
								IF_RND( 8 ),
									IF_AREA( 80, 128, 0, 0 ),
										CMD_JUMP( 21 ),
									CMD_ELSE,
									CMD_ENDIF,
								CMD_ELSE,
								CMD_ENDIF,
							CMD_ELSE,
							CMD_ENDIF,
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		WAIT( 1 ),
	CMD_ENDIF,
	CMD_ELSE,
		TIMER_SET( 5 ),
		IF_AREA2( 0, 0, 0, 0 ),
			CMD_JUMP( 19 ),
		CMD_ELSE,
			IF_LEVEL( 28 ),
				IF_AREA( 80, 128, 0, 0 ),
					CMD_JUMP( 21 ),
				CMD_ELSE,
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 22 ),
					IF_RND( 24 ),
						IF_AREA( 80, 128, 0, 0 ),
							CMD_JUMP( 21 ),
						CMD_ELSE,
						CMD_ENDIF,
					CMD_ELSE,
					CMD_ENDIF,
				CMD_ELSE,
					IF_LEVEL( 12 ),
						IF_RND( 16 ),
							IF_AREA( 80, 128, 0, 0 ),
								CMD_JUMP( 21 ),
							CMD_ELSE,
							CMD_ENDIF,
						CMD_ELSE,
						CMD_ENDIF,
					CMD_ELSE,
						IF_LEVEL( 8 ),
							IF_RND( 8 ),
								IF_AREA( 80, 128, 0, 0 ),
									CMD_JUMP( 21 ),
								CMD_ELSE,
								CMD_ENDIF,
							CMD_ELSE,
							CMD_ENDIF,
						CMD_ELSE,
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib18_cmd_23[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_24[] = {
	STAND,
	TIMER_SET( 60 ),
	IF_AREA( 32, 48, 32, 80 ),
		SP_CMD( pl18_sp_ball, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib18_cmd_25[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_26[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_27[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_28[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_29[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_30[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_31[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_32[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_33[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_34[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_35[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_36[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_37[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_38[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_39[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_40[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_41[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_42[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_43[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_44[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_45[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_46[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_47[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_48[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_49[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_50[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_51[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_52[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_53[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_54[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_55[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_56[] = {
	STAND,
	WAIT2( 8 ),
	IF_LEVEL( 24 ),
		IF_AREA( 128, 64, 0, 64 ),
			IF_GAUGE( 1 ),
				SP_CMD( pl18_sp_thunder, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				SP_CMD( pl18_sp_sweep, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_GAUGE( 1 ),
				SP_CMD( pl18_sp_pillar, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				SP_CMD( pl18_sp_rush, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_AREA( 128, 64, 0, 64 ),
			SP_CMD( pl18_sp_sweep, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			SP_CMD( pl18_sp_rush, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib18_cmd_57[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_58[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_59[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_60[] = {
	JUMP_RET,
};

const Uint8 lib18_cmd_61[] = {
	JUMP_RET,
};

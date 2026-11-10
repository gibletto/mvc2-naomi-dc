/* Sabretooth (character 2b): the computer's shared lists.
   The lists that the others go to with CMD_JUMP (or JUMP_RND, JUMP_EM, JUMP_LEVEL, JUMP_DIST)
   and come back from with JUMP_RET. PLAYER_WORK's cpu_ex_adrs points here.

   lib2b_tbl     the lists, by number.
   lib2b_cmd_NN  a list, in the commands of cpu_cmd.h. A jump in one names another of them. */
#include "player.h"
#include "pl2b_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const lib2b_tbl[];
extern const Uint8 lib2b_cmd_00[], lib2b_cmd_01[], lib2b_cmd_02[], lib2b_cmd_03[], lib2b_cmd_04[],
	lib2b_cmd_05[], lib2b_cmd_06[], lib2b_cmd_07[], lib2b_cmd_08[], lib2b_cmd_09[], lib2b_cmd_10[],
	lib2b_cmd_11[], lib2b_cmd_12[], lib2b_cmd_13[], lib2b_cmd_14[], lib2b_cmd_15[], lib2b_cmd_16[],
	lib2b_cmd_17[], lib2b_cmd_18[], lib2b_cmd_19[], lib2b_cmd_20[], lib2b_cmd_21[], lib2b_cmd_22[],
	lib2b_cmd_23[], lib2b_cmd_24[], lib2b_cmd_25[], lib2b_cmd_26[], lib2b_cmd_27[], lib2b_cmd_28[],
	lib2b_cmd_29[], lib2b_cmd_30[], lib2b_cmd_31[], lib2b_cmd_32[], lib2b_cmd_33[], lib2b_cmd_34[],
	lib2b_cmd_35[], lib2b_cmd_36[], lib2b_cmd_37[], lib2b_cmd_38[], lib2b_cmd_39[], lib2b_cmd_40[],
	lib2b_cmd_41[], lib2b_cmd_42[], lib2b_cmd_43[], lib2b_cmd_44[], lib2b_cmd_45[], lib2b_cmd_46[],
	lib2b_cmd_47[], lib2b_cmd_48[], lib2b_cmd_49[], lib2b_cmd_50[], lib2b_cmd_51[], lib2b_cmd_52[],
	lib2b_cmd_53[], lib2b_cmd_54[], lib2b_cmd_55[], lib2b_cmd_56[], lib2b_cmd_57[], lib2b_cmd_58[],
	lib2b_cmd_59[], lib2b_cmd_60[], lib2b_cmd_61[];

const Uint8 *const lib2b_tbl[] = {
	lib2b_cmd_00, lib2b_cmd_01, lib2b_cmd_02, lib2b_cmd_03, lib2b_cmd_04, lib2b_cmd_05,
	lib2b_cmd_06, lib2b_cmd_07, lib2b_cmd_08, lib2b_cmd_09, lib2b_cmd_10, lib2b_cmd_11,
	lib2b_cmd_12, lib2b_cmd_13, lib2b_cmd_14, lib2b_cmd_15, lib2b_cmd_16, lib2b_cmd_17,
	lib2b_cmd_18, lib2b_cmd_19, lib2b_cmd_20, lib2b_cmd_21, lib2b_cmd_22, lib2b_cmd_23,
	lib2b_cmd_24, lib2b_cmd_25, lib2b_cmd_26, lib2b_cmd_27, lib2b_cmd_28, lib2b_cmd_29,
	lib2b_cmd_30, lib2b_cmd_31, lib2b_cmd_32, lib2b_cmd_33, lib2b_cmd_34, lib2b_cmd_35,
	lib2b_cmd_36, lib2b_cmd_37, lib2b_cmd_38, lib2b_cmd_39, lib2b_cmd_40, lib2b_cmd_41,
	lib2b_cmd_42, lib2b_cmd_43, lib2b_cmd_44, lib2b_cmd_45, lib2b_cmd_46, lib2b_cmd_47,
	lib2b_cmd_48, lib2b_cmd_49, lib2b_cmd_50, lib2b_cmd_51, lib2b_cmd_52, lib2b_cmd_53,
	lib2b_cmd_54, lib2b_cmd_55, lib2b_cmd_56, lib2b_cmd_57, lib2b_cmd_58, lib2b_cmd_59,
	lib2b_cmd_60, lib2b_cmd_61, 0,
};

const Uint8 lib2b_cmd_00[] = {
	IF_LEVEL( 24 ),
		WAIT2( 0 ),
	CMD_ELSE,
		IF_LEVEL( 20 ),
			WAIT2( 4 ),
		CMD_ELSE,
			IF_LEVEL( 16 ),
				WAIT2( 8 ),
			CMD_ELSE,
				WAIT2( 12 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_01[] = {
	IF_LEVEL( 24 ),
		IF_RND( 24 ),
			PARTNER_CALL,
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 20 ),
			IF_RND( 20 ),
				PARTNER_CALL,
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 16 ),
				IF_RND( 16 ),
					PARTNER_CALL,
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 12 ),
					IF_RND( 12 ),
						PARTNER_CALL,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_02[] = {
	IF_RND( 10 ),
		IF_LEVEL( 24 ),
			IF_RND( 8 ),
				IF_VITAL( 60 ),
					SP_CHANGE,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 20 ),
				IF_RND( 8 ),
					IF_VITAL( 80 ),
						SP_CHANGE,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 16 ),
					IF_RND( 8 ),
						IF_VITAL( 100 ),
							SP_CHANGE,
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ELSE,
					IF_LEVEL( 12 ),
						IF_RND( 8 ),
							IF_VITAL( 120 ),
								SP_CHANGE,
							CMD_ENDIF,
						CMD_ENDIF,
					CMD_ELSE,
						IF_RND( 8 ),
							IF_VITAL( 140 ),
								SP_CHANGE,
							CMD_ENDIF,
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_03[] = {
	IF_RND( 20 ),
		CANCEL_SET,
			PUNCH( LV_LIGHT ),
			PUNCH( LV_LIGHT ),
			KICK( LV_HEAVY ),
			SP_CMD( pl2b_sp_berserk, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		CANCEL_SET2,
			PUNCH( LV_LIGHT ),
			PUNCH( LV_LIGHT ),
			KICK( LV_HEAVY ),
			SP_CMD( pl2b_sp_berserk, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_04[] = {
	IF_RND( 20 ),
		CANCEL_SET,
			KICK( LV_LIGHT ),
			KICK( LV_LIGHT ),
			KICK( LV_HEAVY ),
			SP_CMD( pl2b_sp_berserk, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		CANCEL_SET2,
			KICK( LV_LIGHT ),
			KICK( LV_LIGHT ),
			KICK( LV_HEAVY ),
			SP_CMD( pl2b_sp_berserk, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_05[] = {
	IF_RND( 20 ),
		CANCEL_SET,
			PUNCH( LV_LIGHT ),
			PUNCH( LV_LIGHT ),
			PUNCH( LV_HEAVY ),
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		CANCEL_SET2,
			PUNCH( LV_LIGHT ),
			PUNCH( LV_LIGHT ),
			PUNCH( LV_HEAVY ),
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_06[] = {
	IF_RND( 20 ),
		CANCEL_SET,
			KICK( LV_LIGHT ),
			KICK( LV_LIGHT ),
			KICK( LV_HEAVY ),
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		CANCEL_SET2,
			KICK( LV_LIGHT ),
			KICK( LV_LIGHT ),
			KICK( LV_HEAVY ),
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_07[] = {
	IF_RND( 20 ),
		CANCEL_SET,
			PUNCH( LV_LIGHT ),
			PUNCH( LV_HEAVY ),
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		CANCEL_SET2,
			PUNCH( LV_LIGHT ),
			PUNCH( LV_HEAVY ),
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_08[] = {
	IF_RND( 16 ),
		CANCEL_SET,
			KICK( LV_LIGHT ),
			KICK( LV_HEAVY ),
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		CANCEL_SET2,
			KICK( LV_LIGHT ),
			KICK( LV_HEAVY ),
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_09[] = {
	IF_RND( 16 ),
		PUNCH( LV_HEAVY ),
	CMD_ELSE,
		PUNCH( LV_HEAVY ),
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_10[] = {
	IF_RND( 16 ),
		KICK( LV_HEAVY ),
	CMD_ELSE,
		KICK( LV_HEAVY ),
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_11[] = {
	JUMP_LEVEL( 9, 12, 7, 16, 5, 22, 3 ),
	JUMP_RET,
};

const Uint8 lib2b_cmd_12[] = {
	JUMP_LEVEL( 10, 12, 8, 16, 6, 22, 4 ),
	JUMP_RET,
};

const Uint8 lib2b_cmd_13[] = {
	JUMP_LEVEL( 9, 12, 7, 16, 6, 22, 4 ),
	JUMP_RET,
};

const Uint8 lib2b_cmd_14[] = {
	JUMP_LEVEL( 10, 12, 8, 16, 5, 22, 3 ),
	JUMP_RET,
};

const Uint8 lib2b_cmd_15[] = {
	IF_RND( 12 ),
		CROUCH,
	CMD_ENDIF,
	JUMP_RND( 11, 12, 13, 14 ),
	STAND,
	JUMP_RET,
};

const Uint8 lib2b_cmd_16[] = {
	JUMP_RND( 11, 12, 13, 14 ),
	JUMP_RET,
};

const Uint8 lib2b_cmd_17[] = {
	TIMER_SET( 3 ),
	IF_AREA( 32, 96, 160, 96 ),
		SP_CMD( pl2b_sp_birdie, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ELSE,
		TIMER_SET( 3 ),
		IF_AREA( 48, 96, 288, 96 ),
			SP_CMD( pl2b_sp_birdie, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ELSE,
			TIMER_SET( 3 ),
			IF_AREA( 0, 48, 0, 128 ),
				SP_CMD( pl2b_sp_birdie, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_18[] = {
	CANCEL_SET,
		CROUCH,
		KICK( LV_HEAVY ),
		WAIT( 10 ),
		SP_CMD( pl2b_sp_berserk, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CANCEL_CLR,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_19[] = {
	IF_LEVEL( 18 ),
		IF_RND( 16 ),
			PUNCH( LV_HEAVY ),
		CMD_ELSE,
			KICK( LV_HEAVY ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_RND( 16 ),
			PUNCH( LV_LIGHT ),
		CMD_ELSE,
			KICK( LV_LIGHT ),
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_20[] = {
	STAND,
	IF_RND( 18 ),
		JUMP( DIR_FORWARD ),
		JUMP_FLAG_SET,
			TIMER_SET( 120 ),
			IF_HEIGHT( 0xff, 96 ),
				PUNCH( LV_HEAVY ),
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		JUMP( DIR_FORWARD ),
		JUMP_FLAG_SET,
			TIMER_SET( 120 ),
			IF_HEIGHT( 0xff, 96 ),
				KICK( LV_HEAVY ),
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_21[] = {
	STAND,
	IF_RND( 16 ),
		SP_CMD( pl2b_sp_fang, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ELSE,
		SP_CMD( pl2b_sp_fang, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ENDIF,
	WAIT2( 4 ),
	JUMP_RND( 17, 47, 17, 47 ),
	JUMP_RET,
};

const Uint8 lib2b_cmd_22[] = {
	IF_GAUGE( 1 ),
		IF_RND( 10 ),
			SP_CMD( pl2b_sp_clawx, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			IF_RND( 10 ),
				SP_CMD( pl2b_sp_hbirdie, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				IF_RND( 10 ),
					SP_CMD( pl2b_sp_weapon, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_23[] = {
	IF_RND( 24 ),
		JUMP( DIR_BACK ),
		JUMP_FLAG_SET,
			TIMER_SET( 120 ),
			IF_HEIGHT( 1, 80 ),
				PARTNER_CALL,
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
		STAND,
	CMD_ELSE,
		IF_GAUGE( 3 ),
			SP_V_COMBI,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_24[] = {
	CANCEL_SET,
		PUNCH( LV_LIGHT ),
		KICK( LV_LIGHT ),
		PUNCH( LV_LIGHT ),
		KICK( LV_LIGHT ),
		PUNCH( LV_HEAVY ),
		CANCEL_CLR,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_25[] = {
	CANCEL_SET,
		PUNCH( LV_LIGHT ),
		KICK( LV_LIGHT ),
		PUNCH( LV_LIGHT ),
		KICK( LV_LIGHT ),
		KICK( LV_HEAVY ),
		CANCEL_CLR,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_26[] = {
	CANCEL_SET,
		PUNCH( LV_LIGHT ),
		PUNCH( LV_LIGHT ),
		KICK( LV_LIGHT ),
		PUNCH( LV_HEAVY ),
		CANCEL_CLR,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_27[] = {
	CANCEL_SET,
		KICK( LV_LIGHT ),
		PUNCH( LV_LIGHT ),
		KICK( LV_LIGHT ),
		KICK( LV_HEAVY ),
		CANCEL_CLR,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_28[] = {
	CANCEL_SET,
		PUNCH( LV_LIGHT ),
		PUNCH( LV_LIGHT ),
		PUNCH( LV_HEAVY ),
		CANCEL_CLR,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_29[] = {
	CANCEL_SET,
		KICK( LV_LIGHT ),
		KICK( LV_LIGHT ),
		KICK( LV_HEAVY ),
		CANCEL_CLR,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_30[] = {
	CANCEL_SET,
		PUNCH( LV_LIGHT ),
		KICK( LV_LIGHT ),
		PUNCH( LV_HEAVY ),
		CANCEL_CLR,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_31[] = {
	CANCEL_SET,
		KICK( LV_LIGHT ),
		KICK( LV_HEAVY ),
		CANCEL_CLR,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_32[] = {
	JUMP_LEVEL( 30, 12, 28, 18, 26, 22, 24 ),
	JUMP_RET,
};

const Uint8 lib2b_cmd_33[] = {
	JUMP_LEVEL( 31, 12, 29, 18, 27, 22, 25 ),
	JUMP_RET,
};

const Uint8 lib2b_cmd_34[] = {
	JUMP_LEVEL( 30, 12, 28, 18, 27, 22, 25 ),
	JUMP_RET,
};

const Uint8 lib2b_cmd_35[] = {
	JUMP_LEVEL( 31, 12, 29, 18, 26, 22, 24 ),
	JUMP_RET,
};

const Uint8 lib2b_cmd_36[] = {
	JUMP_RND( 32, 33, 34, 35 ),
	JUMP_RET,
};

const Uint8 lib2b_cmd_37[] = {
	CANCEL_SET,
		CROUCH,
		KICK( LV_LIGHT ),
		KICK( LV_LIGHT ),
		PUNCH( LV_HEAVY ),
		SJUMP3( DIR_FORWARD ),
		CANCEL_CLR,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_38[] = {
	CANCEL_SET,
		KICK( LV_LIGHT ),
		KICK( LV_LIGHT ),
		PUNCH( LV_HEAVY ),
		SJUMP3( DIR_FORWARD ),
		CANCEL_CLR,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_39[] = {
	CANCEL_SET,
		CROUCH,
		PUNCH( LV_LIGHT ),
		PUNCH( LV_LIGHT ),
		STAND,
		PUNCH( LV_HEAVY ),
		SJUMP3( DIR_FORWARD ),
		CANCEL_CLR,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_40[] = {
	CANCEL_SET,
		CROUCH,
		KICK( LV_LIGHT ),
		KICK( LV_HEAVY ),
		SJUMP3( DIR_FORWARD ),
		CANCEL_CLR,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_41[] = {
	IF_RND( 16 ),
		JUMP_LEVEL( 40, 12, 39, 20, 38, 24, 37 ),
	CMD_ELSE,
		JUMP_LEVEL( 40, 8, 39, 16, 38, 21, 37 ),
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_42[] = {
	CANCEL_SET,
		PUNCH( LV_LIGHT ),
		PUNCH( LV_LIGHT ),
		SP_CMD_SET( pl2b_sp_clawx, 0, PK_PUNCH, LV_LIGHT, 0, 1, 18 ),
		SP_CMD( pl2b_sp_berserk, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CANCEL_CLR,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_43[] = {
	CANCEL_SET,
		KICK( LV_LIGHT ),
		KICK( LV_LIGHT ),
		SP_CMD_SET( pl2b_sp_hbirdie, 0, PK_PUNCH, LV_LIGHT, 0, 1, 24 ),
		SP_CMD( pl2b_sp_berserk, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CANCEL_CLR,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_44[] = {
	CANCEL_SET,
		PUNCH( LV_LIGHT ),
		PUNCH( LV_LIGHT ),
		SP_CMD_SET( pl2b_sp_weapon, 0, PK_PUNCH, LV_LIGHT, 0, 1, 20 ),
		SP_CMD( pl2b_sp_berserk, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CANCEL_CLR,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_45[] = {
	CANCEL_SET,
		KICK( LV_LIGHT ),
		KICK( LV_LIGHT ),
		SP_CMD_SET( pl2b_sp_hbirdie, 0, PK_PUNCH, LV_LIGHT, 0, 1, 30 ),
		SP_CMD( pl2b_sp_fang, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CANCEL_CLR,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_46[] = {
	IF_GAUGE( 1 ),
		JUMP_RND( 42, 43, 44, 45 ),
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_47[] = {
	TIMER_SET( 3 ),
	IF_AREA( 112, 64, 96, 64 ),
		SP_CMD( pl2b_sp_berserk, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ELSE,
		TIMER_SET( 3 ),
		IF_AREA( 0, 64, 0, 64 ),
			SP_CMD( pl2b_sp_berserk, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_48[] = {
	IF_RND( 16 ),
		NAG_VFLIP_ADJ( 1, LEVER_FORWARD ),
	CMD_ELSE,
		NAG_VFLIP_ADJ( 1, LEVER_BACK ),
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_49[] = {
	IF_RND( 16 ),
		NAG_VFLIP_ADJ2( 1, LEVER_FORWARD ),
	CMD_ELSE,
		NAG_VFLIP_ADJ2( 1, LEVER_BACK ),
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_50[] = {
	PUNCH( LV_LIGHT ),
	PUNCH( LV_LIGHT ),
	PUNCH( LV_LIGHT ),
	IF_LEVEL( 16 ),
		IF_RND( 18 ),
			CMD_JUMP( 18 ),
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_51[] = {
	IF_RND( 24 ),
		JUMP_RND( 48, 49, 48, 49 ),
	CMD_ELSE,
		CMD_JUMP( 50 ),
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_52[] = {
	IF_RND( 20 ),
		JUMP_RND( 48, 49, 48, 49 ),
	CMD_ELSE,
		CMD_JUMP( 50 ),
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_53[] = {
	IF_RND( 16 ),
		JUMP_RND( 48, 49, 48, 49 ),
	CMD_ELSE,
		CMD_JUMP( 50 ),
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_54[] = {
	IF_RND( 12 ),
		JUMP_RND( 48, 49, 48, 49 ),
	CMD_ELSE,
		CMD_JUMP( 50 ),
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_55[] = {
	JUMP_LEVEL( 54, 16, 53, 20, 52, 24, 51 ),
	JUMP_RET,
};

const Uint8 lib2b_cmd_56[] = {
	TIMER_SET( 120 ),
	IF_AREA( 64, 64, 112, 128 ),
		IF_RND( 20 ),
			SP_CMD( pl2b_sp_berserk, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			SP_CMD( pl2b_sp_berserk, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_57[] = {
	IF_LEVEL( 24 ),
		IF_RND( 24 ),
			GUARD( 30 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 20 ),
			IF_RND( 21 ),
				GUARD( 30 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 16 ),
				IF_RND( 18 ),
					GUARD( 30 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 12 ),
					IF_RND( 15 ),
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
	JUMP_RET,
};

const Uint8 lib2b_cmd_58[] = {
	IF_LEVEL( 24 ),
		IF_RND( 24 ),
			GUARD( 60 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 20 ),
			IF_RND( 21 ),
				GUARD( 60 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 16 ),
				IF_RND( 18 ),
					GUARD( 60 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 12 ),
					IF_RND( 15 ),
						GUARD( 60 ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_RND( 12 ),
						GUARD( 60 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_59[] = {
	IF_LEVEL( 24 ),
		IF_RND( 24 ),
			GUARD( 120 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 20 ),
			IF_RND( 21 ),
				GUARD( 120 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 16 ),
				IF_RND( 18 ),
					GUARD( 120 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 12 ),
					IF_RND( 15 ),
						GUARD( 120 ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_RND( 12 ),
						GUARD( 120 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_60[] = {
	IF_RND( 16 ),
		IF_RND( 16 ),
			GUARD_BACK_SET( 4 ),
		CMD_ELSE,
			GUARD_BACK_SET( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2b_cmd_61[] = {
	IF_CATCH,
		CMD_JUMP( 55 ),
	CMD_ELSE,
		IF_DIST( 48, 48, 0, 48 ),
			WALK_F( 8 ),
		CMD_ELSE,
			WALK_F( 16 ),
		CMD_ENDIF,
		IF_CATCH,
			CMD_JUMP( 55 ),
		CMD_ELSE,
			IF_DIST( 48, 48, 0, 48 ),
				WALK_F( 8 ),
			CMD_ELSE,
				WALK_F( 16 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

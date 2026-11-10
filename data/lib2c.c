/* Magneto (character 2c): the computer's shared lists.
   The lists that the others go to with CMD_JUMP (or JUMP_RND, JUMP_EM, JUMP_LEVEL, JUMP_DIST)
   and come back from with JUMP_RET. PLAYER_WORK's cpu_ex_adrs points here.

   lib2c_tbl     the lists, by number.
   lib2c_cmd_NN  a list, in the commands of cpu_cmd.h. A jump in one names another of them. */
#include "player.h"
#include "pl2c_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const lib2c_tbl[];
extern const Uint8 lib2c_cmd_00[], lib2c_cmd_01[], lib2c_cmd_02[], lib2c_cmd_03[], lib2c_cmd_04[],
	lib2c_cmd_05[], lib2c_cmd_06[], lib2c_cmd_07[], lib2c_cmd_08[], lib2c_cmd_09[], lib2c_cmd_10[],
	lib2c_cmd_11[], lib2c_cmd_12[], lib2c_cmd_13[], lib2c_cmd_14[], lib2c_cmd_15[], lib2c_cmd_16[],
	lib2c_cmd_17[], lib2c_cmd_18[], lib2c_cmd_19[], lib2c_cmd_20[], lib2c_cmd_21[], lib2c_cmd_22[],
	lib2c_cmd_23[], lib2c_cmd_24[], lib2c_cmd_25[], lib2c_cmd_26[], lib2c_cmd_27[], lib2c_cmd_28[],
	lib2c_cmd_29[], lib2c_cmd_30[], lib2c_cmd_31[], lib2c_cmd_32[], lib2c_cmd_33[], lib2c_cmd_34[],
	lib2c_cmd_35[], lib2c_cmd_36[], lib2c_cmd_37[], lib2c_cmd_38[], lib2c_cmd_39[], lib2c_cmd_40[],
	lib2c_cmd_41[], lib2c_cmd_42[], lib2c_cmd_43[], lib2c_cmd_44[], lib2c_cmd_45[], lib2c_cmd_46[],
	lib2c_cmd_47[], lib2c_cmd_48[], lib2c_cmd_49[], lib2c_cmd_50[], lib2c_cmd_51[], lib2c_cmd_52[],
	lib2c_cmd_53[], lib2c_cmd_54[], lib2c_cmd_55[], lib2c_cmd_56[], lib2c_cmd_57[], lib2c_cmd_58[],
	lib2c_cmd_59[], lib2c_cmd_60[], lib2c_cmd_61[], lib2c_cmd_62[], lib2c_cmd_63[], lib2c_cmd_64[],
	lib2c_cmd_65[], lib2c_cmd_66[], lib2c_cmd_67[], lib2c_cmd_68[], lib2c_cmd_69[], lib2c_cmd_70[],
	lib2c_cmd_71[], lib2c_cmd_72[], lib2c_cmd_73[], lib2c_cmd_74[], lib2c_cmd_75[], lib2c_cmd_76[];

const Uint8 *const lib2c_tbl[] = {
	lib2c_cmd_00, lib2c_cmd_01, lib2c_cmd_02, lib2c_cmd_03, lib2c_cmd_04, lib2c_cmd_05,
	lib2c_cmd_06, lib2c_cmd_07, lib2c_cmd_08, lib2c_cmd_09, lib2c_cmd_10, lib2c_cmd_11,
	lib2c_cmd_12, lib2c_cmd_13, lib2c_cmd_14, lib2c_cmd_15, lib2c_cmd_16, lib2c_cmd_17,
	lib2c_cmd_18, lib2c_cmd_19, lib2c_cmd_20, lib2c_cmd_21, lib2c_cmd_22, lib2c_cmd_23,
	lib2c_cmd_24, lib2c_cmd_25, lib2c_cmd_26, lib2c_cmd_27, lib2c_cmd_28, lib2c_cmd_29,
	lib2c_cmd_30, lib2c_cmd_31, lib2c_cmd_32, lib2c_cmd_33, lib2c_cmd_34, lib2c_cmd_35,
	lib2c_cmd_36, lib2c_cmd_37, lib2c_cmd_38, lib2c_cmd_39, lib2c_cmd_40, lib2c_cmd_41,
	lib2c_cmd_42, lib2c_cmd_43, lib2c_cmd_44, lib2c_cmd_45, lib2c_cmd_46, lib2c_cmd_47,
	lib2c_cmd_48, lib2c_cmd_49, lib2c_cmd_50, lib2c_cmd_51, lib2c_cmd_52, lib2c_cmd_53,
	lib2c_cmd_54, lib2c_cmd_55, lib2c_cmd_56, lib2c_cmd_57, lib2c_cmd_58, lib2c_cmd_59,
	lib2c_cmd_60, lib2c_cmd_61, lib2c_cmd_62, lib2c_cmd_63, lib2c_cmd_64, lib2c_cmd_65,
	lib2c_cmd_66, lib2c_cmd_67, lib2c_cmd_68, lib2c_cmd_69, lib2c_cmd_70, lib2c_cmd_71,
	lib2c_cmd_72, lib2c_cmd_73, lib2c_cmd_74, lib2c_cmd_75, lib2c_cmd_76, 0,
};

const Uint8 lib2c_cmd_00[] = {
	TIMER_SET( 120 ),
	IF_AREA( 24, 16, 16, 0 ),
		CANCEL_SET2,
			NAG_VFLIP_ADJ4( LV_LIGHT, LEVER_DOWN ),
			KICK( LV_LIGHT ),
			KICK( LV_HEAVY ),
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_01[] = {
	IF_LEVEL( 16 ),
		DASH_EM( 48 ),
		CANCEL_SET,
			KICK( LV_LIGHT ),
			KICK( LV_LIGHT ),
			SP_CMD( pl2c_sp_disrupt, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CANCEL_CLR,
		CMD_ENDIF,
		STAND,
		IF_LEVEL( 24 ),
			IF_RND( 16 ),
				WAIT2( 6 ),
				JUMP( DIR_UP ),
				JUMP_FLAG_SET,
					WAIT( 30 ),
					SP_CMD( pl2c_sp_jblast, 0, PK_PUNCH, LV_HEAVY, 0 ),
					JUMP_FLAG_CLR,
				CMD_ENDIF,
				STAND,
				DASH_B( 512 ),
			CMD_ELSE,
				CMD_JUMP( 6 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 8 ),
			IF_RND( 20 ),
				DASH_EM( 64 ),
				STAND,
				JUMP_EM( 13, 13, 13, 13, 12, 12, 12, 12 ),
			CMD_ELSE,
				DASH_EM( 80 ),
				STAND,
				KICK( LV_HEAVY ),
			CMD_ENDIF,
			DASH_B( 512 ),
		CMD_ELSE,
			DASH_EM( 32 ),
			CROUCH,
			PUNCH( LV_HEAVY ),
			CROUCH,
			DASH_B( 256 ),
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	JUMP_RET,
};

const Uint8 lib2c_cmd_02[] = {
	WAIT2( 2 ),
	JUMP_RET,
};

const Uint8 lib2c_cmd_03[] = {
	IF_RND( 20 ),
		DASH_EM( 64 ),
		STAND,
		JUMP_EM( 13, 13, 13, 13, 12, 12, 12, 12 ),
	CMD_ELSE,
		DASH_EM( 48 ),
		STAND,
		KICK( LV_HEAVY ),
	CMD_ENDIF,
	IF_RND( 10 ),
		WAIT2( 2 ),
		JUMP( DIR_BACK ),
		JUMP_FLAG_SET,
			IF_RND( 12 ),
				JUMP_LEVEL( 48, 7, 49, 13, 50, 21, 51 ),
			CMD_ENDIF,
			IF_RND( 26 ),
				WAIT( 30 ),
				SP_CMD( pl2c_sp_jblast, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ELSE,
	CMD_ENDIF,
	STAND,
	JUMP_RET,
};

const Uint8 lib2c_cmd_04[] = {
	DASH_EM( 48 ),
	CANCEL_SET,
		KICK( LV_LIGHT ),
		KICK( LV_LIGHT ),
		WAIT2( 4 ),
		SP_CMD( pl2c_sp_disrupt, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CANCEL_CLR,
	CMD_ENDIF,
	STAND,
	IF_RND( 10 ),
		WAIT2( 4 ),
	CMD_ENDIF,
	IF_RND( 20 ),
		JUMP( DIR_BACK ),
		JUMP_FLAG_SET,
			IF_RND( 12 ),
				JUMP_LEVEL( 48, 7, 49, 13, 50, 21, 51 ),
			CMD_ENDIF,
			WAIT( 30 ),
			SP_CMD( pl2c_sp_jblast, 0, PK_PUNCH, LV_HEAVY, 0 ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	JUMP_RET,
};

const Uint8 lib2c_cmd_05[] = {
	JUMP( DIR_FORWARD ),
	JUMP_FLAG_SET,
		WAIT( 30 ),
		TIMER_SET( 60 ),
		IF_AREA( 24, 16, 16, 0 ),
			IF_LEVEL( 10 ),
				CANCEL_SET2,
					NAG_VFLIP_ADJ4( LV_LIGHT, LEVER_DOWN ),
					KICK( LV_LIGHT ),
					KICK( LV_HEAVY ),
					CANCEL_CLR,
				CMD_ENDIF,
			CMD_ELSE,
				CANCEL_SET2,
					NAG_VFLIP_ADJ4( LV_LIGHT, LEVER_DOWN ),
					KICK( LV_HEAVY ),
					CANCEL_CLR,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	CANCEL_SET,
		IF_EM_ATCK( ATCK_STAND_HK ),
			KICK( LV_HEAVY ),
			SP_CMD( pl2c_sp_disrupt, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ELSE,
			KICK( LV_LIGHT ),
			SP_CMD( pl2c_sp_disrupt, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ENDIF,
		CANCEL_CLR,
	CMD_ELSE,
		DASH_B( 200 ),
		CANCEL_CLR,
	CMD_ENDIF,
	STAND,
	JUMP_RET,
};

const Uint8 lib2c_cmd_06[] = {
	STAND,
	SP_CMD( pl2c_sp_grav, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CROUCH,
	TIMER_SET( 60 ),
	IF_EM_ATCK( ATCK_CROUCH_HP ),
		WAIT( 20 ),
		IF_RND( 10 ),
			CANCEL_SET2,
				CROUCH,
				PUNCH( LV_HEAVY ),
				SJUMP3( DIR_UP ),
				CANCEL_CLR,
			CMD_ENDIF,
			SJUMP_FLAG_SET,
				JUMP_LEVEL( 30, 8, 31, 29, 32, 31, 8 ),
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			STAND,
			IF_RND( 16 ),
				STAND,
				KICK( LV_HEAVY ),
			CMD_ELSE,
				STAND,
				PUNCH( LV_HEAVY ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	JUMP_RET,
};

const Uint8 lib2c_cmd_07[] = {
	STAND,
	IF_EM_ATCK( ATCK_CROUCH_HP ),
		CANCEL_SET2,
			CROUCH,
			PUNCH( LV_HEAVY ),
			SJUMP3( DIR_UP ),
			CANCEL_CLR,
		CMD_ENDIF,
		SJUMP_FLAG_SET,
			TIMER_SET( 60 ),
			IF_EM_ATCK( ATCK_JUMP_LP ),
				CANCEL_SET,
					PUNCH( LV_LIGHT ),
					KICK( LV_LIGHT ),
					PUNCH( LV_LIGHT ),
					KICK( LV_LIGHT ),
					PUNCH( LV_HEAVY ),
					KICK( LV_HEAVY ),
					SP_CMD( pl2c_sp_jblast, 0, PK_PUNCH, LV_HEAVY, 0 ),
					CANCEL_CLR,
				CMD_ENDIF,
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT2( 2 ),
	JUMP_RET,
};

const Uint8 lib2c_cmd_08[] = {
	TIMER_SET( 120 ),
	IF_EM_ATCK( ATCK_JUMP_LP ),
		CANCEL_SET,
			PUNCH( LV_LIGHT ),
			KICK( LV_LIGHT ),
			PUNCH( LV_LIGHT ),
			KICK( LV_LIGHT ),
			PUNCH( LV_HEAVY ),
			KICK( LV_HEAVY ),
			SP_CMD( pl2c_sp_jblast, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_09[] = {
	STAND,
	IF_RND( 12 ),
		DASH_EM( 96 ),
		STAND,
		WAIT2( 4 ),
		WALK_DIST( 96 ),
	CMD_ELSE,
		DASH_EM( 112 ),
		STAND,
		WAIT2( 4 ),
		WALK_DIST( 112 ),
	CMD_ENDIF,
	STAND,
	JUMP_RET,
};

const Uint8 lib2c_cmd_10[] = {
	STAND,
	IF_RND( 12 ),
		DASH_EM( 120 ),
		STAND,
		WAIT2( 4 ),
		WALK_DIST( 120 ),
	CMD_ELSE,
		DASH_EM( 144 ),
		STAND,
		WAIT2( 4 ),
		WALK_DIST( 144 ),
	CMD_ENDIF,
	STAND,
	JUMP_RET,
};

const Uint8 lib2c_cmd_11[] = {
	WAIT2( 2 ),
	JUMP_RET,
};

const Uint8 lib2c_cmd_12[] = {
	STAND,
	PUNCH( LV_HEAVY ),
	JUMP_RET,
};

const Uint8 lib2c_cmd_13[] = {
	STAND,
	KICK( LV_LIGHT ),
	JUMP_RET,
};

const Uint8 lib2c_cmd_14[] = {
	STAND,
	IF_DIST( 96, 6, 96, 1 ),
		JUMP_EM( 9, 9, 9, 9, 10, 10, 10, 10 ),
	CMD_ELSE,
		IF_DIST( 32, 6, 240, 1 ),
			JUMP_EM( 17, 17, 17, 17, 18, 18, 18, 18 ),
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_15[] = {
	STAND,
	IF_RND( 12 ),
		DASH_EM( 128 ),
		STAND,
		WAIT2( 4 ),
		WALK_DIST( 128 ),
		JUMP( DIR_FORWARD ),
	CMD_ELSE,
		DASH_EM( 152 ),
		STAND,
		WAIT2( 4 ),
		WALK_DIST( 152 ),
		JUMP( DIR_FORWARD ),
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_16[] = {
	STAND,
	IF_RND( 12 ),
		DASH_EM( 112 ),
		STAND,
		WAIT2( 4 ),
		WALK_DIST( 112 ),
		JUMP( DIR_FORWARD ),
	CMD_ELSE,
		DASH_EM( 144 ),
		STAND,
		WAIT2( 4 ),
		WALK_DIST( 144 ),
		JUMP( DIR_FORWARD ),
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_17[] = {
	STAND,
	WAIT2( 8 ),
	IF_RND( 12 ),
		DASH_F( 48 ),
		JUMP( DIR_FORWARD ),
	CMD_ELSE,
		DASH_F( 16 ),
		JUMP( DIR_FORWARD ),
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_18[] = {
	STAND,
	WAIT2( 8 ),
	IF_RND( 12 ),
		DASH_F( 64 ),
		JUMP( DIR_FORWARD ),
	CMD_ELSE,
		DASH_F( 32 ),
		JUMP( DIR_FORWARD ),
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_19[] = {
	IF_AREA( 48, 16, 48, 1 ),
		CROUCH,
		PUNCH( LV_LIGHT ),
	CMD_ELSE,
		IF_EM_ATCK( ATCK_STAND_MK ),
			STAND,
			KICK( LV_LIGHT ),
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_20[] = {
	IF_EM_ATCK( ATCK_CROUCH_LP ),
		CROUCH,
		CANCEL_SET,
			PUNCH( LV_LIGHT ),
			PUNCH( LV_LIGHT ),
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_21[] = {
	IF_EM_STANCE( STANCE_AIR ),
		IF_DIST( 64, 80, 48, 64 ),
			IF_RND( 20 ),
				STAND,
				JUMP( DIR_FORWARD ),
				JUMP_FLAG_SET,
					JUMP_LEVEL( 66, 8, 67, 13, 68, 21, 69 ),
					JUMP_FLAG_CLR,
				CMD_ENDIF,
				STAND,
				WAIT2( 4 ),
			CMD_ENDIF,
			WAIT2( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_22[] = {
	JUMP_LEVEL( 23, 6, 24, 20, 25, 28, 26 ),
	JUMP_RET,
};

const Uint8 lib2c_cmd_23[] = {
	IF_RND( 10 ),
		WAIT( 15 ),
	CMD_ELSE,
		IF_RND( 16 ),
			WAIT( 12 ),
		CMD_ELSE,
			WAIT( 10 ),
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_24[] = {
	IF_RND( 10 ),
		WAIT( 10 ),
	CMD_ELSE,
		IF_RND( 16 ),
			WAIT( 8 ),
		CMD_ELSE,
			WAIT( 6 ),
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_25[] = {
	IF_RND( 10 ),
		WAIT( 8 ),
	CMD_ELSE,
		IF_RND( 16 ),
			WAIT( 6 ),
		CMD_ELSE,
			WAIT( 4 ),
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_26[] = {
	IF_RND( 10 ),
	CMD_ELSE,
		IF_RND( 16 ),
			WAIT( 1 ),
		CMD_ELSE,
			WAIT( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_27[] = {
	CANCEL_SET,
		IF_EM_ATCK( ATCK_STAND_HK ),
			KICK( LV_HEAVY ),
			SP_CMD( pl2c_sp_disrupt, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ELSE,
			KICK( LV_LIGHT ),
			KICK( LV_LIGHT ),
			SP_CMD( pl2c_sp_disrupt, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ENDIF,
		CANCEL_CLR,
	CMD_ELSE,
		DASH_B( 200 ),
		CANCEL_CLR,
	CMD_ENDIF,
	STAND,
	JUMP_RET,
};

const Uint8 lib2c_cmd_28[] = {
	IF_EM_ATCK( ATCK_STAND_LP ),
		CANCEL_SET,
			STAND,
			PUNCH( LV_LIGHT ),
			KICK( LV_LIGHT ),
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		IF_EM_ATCK( ATCK_CROUCH_LP ),
			CANCEL_SET,
				CROUCH,
				PUNCH( LV_LIGHT ),
				KICK( LV_LIGHT ),
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	IF_LEVEL( 0 ),
		IF_EM_ATCK( 49 ),
			SP_CMD( pl2c_sp_disrupt, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	JUMP_RET,
};

const Uint8 lib2c_cmd_29[] = {
	IF_EM_ATCK( ATCK_STAND_LP ),
		STAND,
		CANCEL_SET,
			PUNCH( LV_LIGHT ),
			KICK( LV_LIGHT ),
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		IF_EM_ATCK( ATCK_CROUCH_LP ),
			CANCEL_SET,
				CROUCH,
				PUNCH( LV_LIGHT ),
				KICK( LV_LIGHT ),
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	IF_LEVEL( 30 ),
		IF_EM_ATCK( 49 ),
			SP_CMD( pl2c_sp_disrupt, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	STAND,
	JUMP_RET,
};

const Uint8 lib2c_cmd_30[] = {
	TIMER_SET( 120 ),
	IF_EM_ATCK( ATCK_JUMP_HK ),
		KICK( LV_HEAVY ),
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_31[] = {
	TIMER_SET( 120 ),
	IF_EM_ATCK( ATCK_JUMP_LP ),
		CANCEL_SET,
			PUNCH( LV_LIGHT ),
			SP_CMD( pl2c_sp_jblast, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_32[] = {
	TIMER_SET( 120 ),
	IF_EM_ATCK( ATCK_JUMP_LP ),
		CANCEL_SET,
			PUNCH( LV_LIGHT ),
			KICK( LV_LIGHT ),
			PUNCH( LV_HEAVY ),
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_33[] = {
	STAND,
	DASH_EM( 64 ),
	STAND,
	WAIT2( 6 ),
	JUMP_EM( 13, 13, 13, 13, 12, 12, 12, 12 ),
	STAND,
	WAIT2( 4 ),
	JUMP_RET,
};

const Uint8 lib2c_cmd_34[] = {
	IF_EM_ATCK( ATCK_STAND_HP ),
		STAND,
		PUNCH( LV_HEAVY ),
	CMD_ELSE,
		CROUCH,
		KICK( LV_HEAVY ),
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_35[] = {
	TIMER_SET( 120 ),
	IF_AREA( 32, 16, 16, 0 ),
		KICK( LV_HEAVY ),
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_36[] = {
	TIMER_SET( 120 ),
	IF_AREA( 24, 16, 16, 0 ),
		CANCEL_SET,
			PUNCH( LV_LIGHT ),
			KICK( LV_HEAVY ),
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_37[] = {
	STAND,
	SP_CMD( pl2c_sp_fly, 0, PK_PUNCH, LV_LIGHT, 0 ),
	FLY_FLAG_SET,
		WALK_POS( 288, 0xff ),
		SP_CMD( pl2c_sp_jblast, 0, PK_PUNCH, LV_HEAVY, 0 ),
		WAIT( 30 ),
		WALK_POS( 288, 0xff ),
		WAIT2( 8 ),
		SP_CMD( pl2c_sp_jblast, 0, PK_PUNCH, LV_HEAVY, 0 ),
		WAIT( 30 ),
		IF_LEVEL( 21 ),
			IF_RND( 16 ),
				WALK_POS( 288, 0xff ),
				WAIT2( 8 ),
				SP_CMD( pl2c_sp_jblast, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
		WAIT2( 8 ),
		SP_CMD( pl2c_sp_fly, 0, PK_PUNCH, LV_LIGHT, 0 ),
		FLY_FLAG_CLR,
	CMD_HIENDIF,
	STAND,
	JUMP_RET,
};

const Uint8 lib2c_cmd_38[] = {
	TIMER_SET( 20 ),
	IF_EM_ATCK( ATCK_JUMP_HK ),
		KICK( LV_HEAVY ),
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_39[] = {
	TIMER_SET( 20 ),
	IF_EM_ATCK( ATCK_JUMP_LP ),
		CANCEL_SET,
			PUNCH( LV_LIGHT ),
			SP_CMD( pl2c_sp_jblast, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_40[] = {
	TIMER_SET( 20 ),
	IF_EM_ATCK( ATCK_JUMP_LP ),
		CANCEL_SET,
			PUNCH( LV_LIGHT ),
			KICK( LV_LIGHT ),
			PUNCH( LV_HEAVY ),
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_41[] = {
	TIMER_SET( 20 ),
	IF_EM_ATCK( ATCK_JUMP_LP ),
		CANCEL_SET,
			PUNCH( LV_LIGHT ),
			KICK( LV_LIGHT ),
			PUNCH( LV_LIGHT ),
			KICK( LV_LIGHT ),
			PUNCH( LV_HEAVY ),
			KICK( LV_HEAVY ),
			SP_CMD( pl2c_sp_jblast, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_42[] = {
	IF_RND( 8 ),
		TIMER2_SET( 0xc8 ),
	CMD_ELSE,
		TIMER2_SET( 60 ),
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_43[] = {
	IF_RND( 10 ),
		TIMER2_SET( 0xc8 ),
	CMD_ELSE,
		TIMER2_SET( 60 ),
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_44[] = {
	IF_RND( 12 ),
		TIMER2_SET( 0xf0 ),
	CMD_ELSE,
		TIMER2_SET( 80 ),
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_45[] = {
	IF_RND( 16 ),
		TIMER2_SET( 0xf0 ),
	CMD_ELSE,
		TIMER2_SET( 80 ),
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_46[] = {
	IF_AREA2( 144, 144, 0, 0 ),
		GUARD( 60 ),
	CMD_ELSE,
		SJUMP( DIR_FORWARD ),
		SJUMP_FLAG_SET,
			JUMP_LEVEL( 42, 7, 43, 13, 44, 21, 45 ),
			WAIT( 30 ),
			JUMP_LEVEL( 30, 8, 35, 16, 36, 22, 0 ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
		STAND,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_47[] = {
	IF_LEVEL( 1 ),
		IF_RND( 28 ),
			TIMER_SET( 20 ),
			IF_AREA2( 152, 144, 0, 64 ),
				GUARD( 30 ),
				IF_DIST( 64, 8, 0, 64 ),
					IF_AREA2( 152, 144, 0, 64 ),
						GUARD( 30 ),
						IF_AREA2( 152, 144, 0, 64 ),
							GUARD( 30 ),
						CMD_ELSE,
							IF_LEVEL( 24 ),
								IF_RND( 16 ),
									DASH_EM( 80 ),
									JUMP_EM( 13, 13, 13, 13, 12, 12, 12, 12 ),
								CMD_ELSE,
									DASH_EM( 80 ),
									CROUCH,
									KICK( LV_HEAVY ),
								CMD_ENDIF,
							CMD_ELSE,
								IF_RND( 10 ),
									DASH_ATCK( ATCK_CROUCH_HK ),
										STAND,
										KICK( LV_LIGHT ),
										KICK( LV_LIGHT ),
									CMD_ENDIF,
								CMD_ELSE,
									DASH_EM( 32 ),
									CROUCH,
									PUNCH( LV_LIGHT ),
								CMD_ENDIF,
							CMD_ENDIF,
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ELSE,
					IF_LEVEL( 24 ),
						IF_RND( 16 ),
							DASH_EM( 80 ),
							JUMP_EM( 13, 13, 13, 13, 12, 12, 12, 12 ),
						CMD_ELSE,
							DASH_EM( 80 ),
							CROUCH,
							KICK( LV_HEAVY ),
						CMD_ENDIF,
					CMD_ELSE,
						IF_RND( 10 ),
							DASH_ATCK( ATCK_CROUCH_HK ),
								STAND,
								KICK( LV_LIGHT ),
							CMD_ENDIF,
						CMD_ELSE,
							DASH_EM( 32 ),
							CROUCH,
							PUNCH( LV_LIGHT ),
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ELSE,
				IF_AREA2( 152, 144, 0, 0 ),
					GUARD( 30 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			WAIT2( 2 ),
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_48[] = {
	IF_RND( 8 ),
		TIMER2_SET( 60 ),
	CMD_ELSE,
		TIMER2_SET( 20 ),
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_49[] = {
	IF_RND( 10 ),
		TIMER2_SET( 60 ),
	CMD_ELSE,
		TIMER2_SET( 20 ),
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_50[] = {
	IF_RND( 12 ),
		TIMER2_SET( 60 ),
	CMD_ELSE,
		TIMER2_SET( 20 ),
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_51[] = {
	IF_RND( 18 ),
		TIMER2_SET( 60 ),
	CMD_ELSE,
		TIMER2_SET( 20 ),
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_52[] = {
	JUMP_FLAG_SET,
		JUMP_LEVEL( 71, 7, 72, 13, 73, 21, 74 ),
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	JUMP_RET,
};

const Uint8 lib2c_cmd_53[] = {
	IF_LEVEL( 1 ),
		IF_RND( 28 ),
			TIMER_SET( 20 ),
			IF_AREA2( 96, 144, 56, 64 ),
				GUARD2( 30 ),
				IF_DIST( 64, 8, 0, 64 ),
					IF_AREA2( 96, 144, 56, 64 ),
						GUARD2( 30 ),
						IF_AREA2( 96, 144, 56, 64 ),
							GUARD2( 30 ),
						CMD_ELSE,
							IF_LEVEL( 24 ),
								IF_RND( 16 ),
									DASH_EM( 80 ),
									JUMP_EM( 13, 13, 13, 13, 12, 12, 12, 12 ),
								CMD_ELSE,
									DASH_EM( 80 ),
									CROUCH,
									KICK( LV_HEAVY ),
								CMD_ENDIF,
							CMD_ELSE,
								IF_RND( 10 ),
									DASH_ATCK( ATCK_CROUCH_HK ),
										STAND,
										KICK( LV_LIGHT ),
									CMD_ENDIF,
								CMD_ELSE,
									DASH_EM( 32 ),
									CROUCH,
									PUNCH( LV_LIGHT ),
								CMD_ENDIF,
							CMD_ENDIF,
						CMD_ENDIF,
					CMD_ELSE,
						IF_LEVEL( 24 ),
							IF_RND( 16 ),
								DASH_EM( 80 ),
								JUMP_EM( 13, 13, 13, 13, 12, 12, 12, 12 ),
							CMD_ELSE,
								DASH_EM( 80 ),
								CROUCH,
								KICK( LV_HEAVY ),
							CMD_ENDIF,
						CMD_ELSE,
							IF_RND( 10 ),
								DASH_ATCK( ATCK_CROUCH_HK ),
									STAND,
									KICK( LV_LIGHT ),
								CMD_ENDIF,
							CMD_ELSE,
								DASH_EM( 32 ),
								CROUCH,
								PUNCH( LV_LIGHT ),
							CMD_ENDIF,
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ELSE,
					IF_AREA2( 96, 144, 56, 0 ),
						GUARD2( 30 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ELSE,
				WAIT2( 2 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_54[] = {
	TIMER_SET( 120 ),
	IF_AREA( 128, 80, 0, 0 ),
		SP_CMD( pl2c_sp_shock, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_55[] = {
	WAIT( 1 ),
	JUMP_RET,
};

const Uint8 lib2c_cmd_56[] = {
	WAIT( 1 ),
	JUMP_RET,
};

const Uint8 lib2c_cmd_57[] = {
	WAIT( 1 ),
	JUMP_RET,
};

const Uint8 lib2c_cmd_58[] = {
	WAIT( 1 ),
	JUMP_RET,
};

const Uint8 lib2c_cmd_59[] = {
	WAIT( 1 ),
	JUMP_RET,
};

const Uint8 lib2c_cmd_60[] = {
	WAIT( 1 ),
	JUMP_RET,
};

const Uint8 lib2c_cmd_61[] = {
	WAIT( 1 ),
	JUMP_RET,
};

const Uint8 lib2c_cmd_62[] = {
	IF_LEVEL( 24 ),
		CMD_JUMP( 63 ),
	CMD_ELSE,
		IF_LEVEL( 16 ),
			CMD_JUMP( 64 ),
		CMD_ELSE,
			IF_LEVEL( 8 ),
				CMD_JUMP( 65 ),
			CMD_ELSE,
				IF_RND( 1 ),
					SP_CHANGE,
				CMD_ELSE,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
};

const Uint8 lib2c_cmd_63[] = {
	IF_RND( 8 ),
		IF_VITAL( 96 ),
			IF_EM_VITAL( 16 ),
				SP_CHANGE,
			CMD_ELSE,
				SP_V_COMBI,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	CMD_ENDIF,
	WAIT2( 2 ),
	JUMP_RET,
};

const Uint8 lib2c_cmd_64[] = {
	IF_RND( 8 ),
		IF_VITAL( 64 ),
			IF_EM_VITAL( 16 ),
				SP_CHANGE,
			CMD_ELSE,
				IF_RND( 8 ),
					SP_V_COMBI,
				CMD_ELSE,
					SP_CHANGE,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_65[] = {
	IF_RND( 8 ),
		IF_VITAL( 48 ),
			IF_EM_VITAL( 16 ),
				SP_CHANGE,
			CMD_ELSE,
				IF_RND( 4 ),
					SP_V_COMBI,
				CMD_ELSE,
					SP_CHANGE,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_66[] = {
	IF_RND( 8 ),
		TIMER2_SET( 60 ),
	CMD_ELSE,
		TIMER2_SET( 20 ),
	CMD_ENDIF,
	CMD_JUMP( 30 ),
	JUMP_RET,
};

const Uint8 lib2c_cmd_67[] = {
	IF_RND( 10 ),
		TIMER2_SET( 60 ),
	CMD_ELSE,
		TIMER2_SET( 20 ),
	CMD_ENDIF,
	CMD_JUMP( 31 ),
	JUMP_RET,
};

const Uint8 lib2c_cmd_68[] = {
	IF_RND( 12 ),
		TIMER2_SET( 60 ),
	CMD_ELSE,
		TIMER2_SET( 20 ),
	CMD_ENDIF,
	CMD_JUMP( 32 ),
	JUMP_RET,
};

const Uint8 lib2c_cmd_69[] = {
	IF_RND( 18 ),
		TIMER2_SET( 60 ),
	CMD_ELSE,
		TIMER2_SET( 20 ),
	CMD_ENDIF,
	CMD_JUMP( 8 ),
	JUMP_RET,
};

const Uint8 lib2c_cmd_70[] = {
	WAIT( 20 ),
	IF_LEVEL( 18 ),
		IF_RND( 24 ),
			JUMP_RND( 30, 54, 30, 54 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_RND( 16 ),
			JUMP_RND( 30, 54, 30, 54 ),
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_71[] = {
	IF_RND( 8 ),
		TIMER2_SET( 60 ),
	CMD_ELSE,
		TIMER2_SET( 20 ),
	CMD_ENDIF,
	CMD_JUMP( 70 ),
	JUMP_RET,
};

const Uint8 lib2c_cmd_72[] = {
	IF_RND( 10 ),
		TIMER2_SET( 60 ),
	CMD_ELSE,
		TIMER2_SET( 20 ),
	CMD_ENDIF,
	CMD_JUMP( 70 ),
	JUMP_RET,
};

const Uint8 lib2c_cmd_73[] = {
	IF_RND( 12 ),
		TIMER2_SET( 60 ),
	CMD_ELSE,
		TIMER2_SET( 20 ),
	CMD_ENDIF,
	CMD_JUMP( 70 ),
	JUMP_RET,
};

const Uint8 lib2c_cmd_74[] = {
	IF_RND( 18 ),
		TIMER2_SET( 60 ),
	CMD_ELSE,
		TIMER2_SET( 20 ),
	CMD_ENDIF,
	CMD_JUMP( 70 ),
	JUMP_RET,
};

const Uint8 lib2c_cmd_75[] = {
	IF_LEVEL( 24 ),
		IF_RND( 28 ),
			WAIT2( 2 ),
			PARTNER_CALL,
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 20 ),
			IF_RND( 24 ),
				WAIT2( 4 ),
				PARTNER_CALL,
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 16 ),
				IF_RND( 20 ),
					WAIT2( 6 ),
					PARTNER_CALL,
				CMD_ENDIF,
			CMD_ELSE,
				IF_RND( 12 ),
					WAIT2( 8 ),
					PARTNER_CALL,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib2c_cmd_76[] = {
	IF_LEVEL( 24 ),
		IF_RND( 28 ),
			SP_CHANGE,
		CMD_ENDIF,
	CMD_ELSE,
		IF_LEVEL( 20 ),
			IF_RND( 24 ),
				SP_CHANGE,
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 16 ),
				IF_RND( 20 ),
					SP_CHANGE,
				CMD_ENDIF,
			CMD_ELSE,
				IF_RND( 12 ),
					SP_CHANGE,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

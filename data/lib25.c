/* Dhalsim (character 25): the computer's shared lists.
   The lists that the others go to with CMD_JUMP (or JUMP_RND, JUMP_EM, JUMP_LEVEL, JUMP_DIST)
   and come back from with JUMP_RET. PLAYER_WORK's cpu_ex_adrs points here.

   lib25_tbl     the lists, by number.
   lib25_cmd_NN  a list, in the commands of cpu_cmd.h. A jump in one names another of them. */
#include "player.h"
#include "pl25_def.h"
#include "cpu_cmd.h"

extern const Uint8 *const lib25_tbl[];
extern const Uint8 lib25_cmd_00[], lib25_cmd_01[], lib25_cmd_02[], lib25_cmd_03[], lib25_cmd_04[],
	lib25_cmd_05[], lib25_cmd_06[], lib25_cmd_07[], lib25_cmd_08[], lib25_cmd_09[], lib25_cmd_10[],
	lib25_cmd_11[], lib25_cmd_12[], lib25_cmd_13[], lib25_cmd_14[], lib25_cmd_15[], lib25_cmd_16[],
	lib25_cmd_17[], lib25_cmd_18[], lib25_cmd_19[], lib25_cmd_20[], lib25_cmd_21[], lib25_cmd_22[],
	lib25_cmd_23[], lib25_cmd_24[], lib25_cmd_25[], lib25_cmd_26[], lib25_cmd_27[], lib25_cmd_28[],
	lib25_cmd_29[], lib25_cmd_30[], lib25_cmd_31[], lib25_cmd_32[], lib25_cmd_33[], lib25_cmd_34[],
	lib25_cmd_35[], lib25_cmd_36[], lib25_cmd_37[], lib25_cmd_38[], lib25_cmd_39[], lib25_cmd_40[],
	lib25_cmd_41[], lib25_cmd_42[], lib25_cmd_43[], lib25_cmd_44[], lib25_cmd_45[], lib25_cmd_46[],
	lib25_cmd_47[], lib25_cmd_48[], lib25_cmd_49[], lib25_cmd_50[], lib25_cmd_51[], lib25_cmd_52[],
	lib25_cmd_53[], lib25_cmd_54[], lib25_cmd_55[], lib25_cmd_56[], lib25_cmd_57[], lib25_cmd_58[],
	lib25_cmd_59[], lib25_cmd_60[], lib25_cmd_61[];

const Uint8 *const lib25_tbl[] = {
	lib25_cmd_00, lib25_cmd_01, lib25_cmd_02, lib25_cmd_03, lib25_cmd_04, lib25_cmd_05,
	lib25_cmd_06, lib25_cmd_07, lib25_cmd_08, lib25_cmd_09, lib25_cmd_10, lib25_cmd_11,
	lib25_cmd_12, lib25_cmd_13, lib25_cmd_14, lib25_cmd_15, lib25_cmd_16, lib25_cmd_17,
	lib25_cmd_18, lib25_cmd_19, lib25_cmd_20, lib25_cmd_21, lib25_cmd_22, lib25_cmd_23,
	lib25_cmd_24, lib25_cmd_25, lib25_cmd_26, lib25_cmd_27, lib25_cmd_28, lib25_cmd_29,
	lib25_cmd_30, lib25_cmd_31, lib25_cmd_32, lib25_cmd_33, lib25_cmd_34, lib25_cmd_35,
	lib25_cmd_36, lib25_cmd_37, lib25_cmd_38, lib25_cmd_39, lib25_cmd_40, lib25_cmd_41,
	lib25_cmd_42, lib25_cmd_43, lib25_cmd_44, lib25_cmd_45, lib25_cmd_46, lib25_cmd_47,
	lib25_cmd_48, lib25_cmd_49, lib25_cmd_50, lib25_cmd_51, lib25_cmd_52, lib25_cmd_53,
	lib25_cmd_54, lib25_cmd_55, lib25_cmd_56, lib25_cmd_57, lib25_cmd_58, lib25_cmd_59,
	lib25_cmd_60, lib25_cmd_61, 0,
};

const Uint8 lib25_cmd_00[] = {
	IF_LEVEL( 24 ),
		CMD_JUMP( 1 ),
	CMD_ELSE,
		IF_LEVEL( 16 ),
			CMD_JUMP( 2 ),
		CMD_ELSE,
			IF_LEVEL( 8 ),
				CMD_JUMP( 3 ),
			CMD_ELSE,
				IF_RND( 8 ),
					SP_CHANGE,
				CMD_ELSE,
					WAIT2( 4 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
};

const Uint8 lib25_cmd_01[] = {
	IF_RND( 10 ),
		IF_VITAL( 80 ),
			IF_EM_VITAL( 80 ),
				SP_V_COMBI,
			CMD_ELSE,
				IF_RND( 8 ),
					SP_CHANGE,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			PARTNER_CALL,
			CROUCH,
			KICK( LV_LIGHT ),
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT2( 2 ),
	JUMP_RET,
};

const Uint8 lib25_cmd_02[] = {
	IF_RND( 8 ),
		IF_VITAL( 64 ),
			IF_EM_VITAL( 64 ),
				SP_V_COMBI,
			CMD_ELSE,
				IF_RND( 4 ),
					SP_CHANGE,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			PARTNER_CALL,
			CROUCH,
			KICK( LV_LIGHT ),
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT2( 2 ),
	JUMP_RET,
};

const Uint8 lib25_cmd_03[] = {
	IF_RND( 6 ),
		IF_VITAL( 48 ),
			IF_EM_VITAL( 48 ),
				SP_V_COMBI,
			CMD_ELSE,
				IF_RND( 2 ),
					SP_CHANGE,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			PARTNER_CALL,
			CROUCH,
			KICK( LV_LIGHT ),
			KICK( LV_LIGHT ),
		CMD_ENDIF,
	CMD_ENDIF,
	WAIT2( 2 ),
	JUMP_RET,
};

const Uint8 lib25_cmd_04[] = {
	IF_DIST( 24, 0, 24, 0 ),
		IF_RND( 16 ),
			PUNCH( LV_HEAVY ),
		CMD_ELSE,
			CANCEL_SET,
				PUNCH( LV_LIGHT ),
				SP_CMD( pl25_sp_brast, 0, PK_PUNCH, LV_HEAVY, 0 ),
				CANCEL_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_DIST( 48, 0, 48, 0 ),
			JUMP( DIR_BACK ),
			JUMP_FLAG_SET,
				TIMER_SET( 80 ),
				IF_HEIGHT( 0xff, 96 ),
					NAG_VFLIP_ADJ3( LV_HEAVY, LEVER_DOWN | LEVER_FORWARD ),
				CMD_ENDIF,
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			SP_CMD( pl25_sp_fire, 0, PK_PUNCH, LV_LIGHT, 0 ),
			WAIT( 30 ),
			KICK( LV_HEAVY ),
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib25_cmd_05[] = {
	CANCEL_SET2,
		SJUMP3( DIR_UP ),
		SJUMP_FLAG_SET,
			TIMER_SET( 0xf0 ),
			IF_EM_ATCK( ATCK_JUMP_LP ),
				PUNCH( LV_LIGHT ),
				KICK( LV_LIGHT ),
				KICK( LV_LIGHT ),
				NAG_VFLIP_ADJ4( LV_LIGHT, LEVER_DOWN | LEVER_FORWARD ),
				IF_RND( 8 ),
					NAG_VFLIP_ADJ4( LV_HEAVY, LEVER_DOWN | LEVER_FORWARD ),
				CMD_ENDIF,
			CMD_ELSE,
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ELSE,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
		IF_AREA( 96, 48, 0, 48 ),
			CROUCH,
			NAG_VFLIP_ADJ4( LV_HEAVY, LEVER_DOWN ),
			STAND,
		CMD_ELSE,
		CMD_ENDIF,
		CANCEL_CLR,
	CMD_ELSE,
		WAIT2( 4 ),
		CANCEL_CLR,
	CMD_ENDIF,
	WAIT2( 8 ),
	JUMP_RET,
};

const Uint8 lib25_cmd_06[] = {
	CANCEL_SET2,
		SJUMP3( DIR_FORWARD ),
		SJUMP_FLAG_SET,
			TIMER_SET( 0xf0 ),
			IF_EM_ATCK( ATCK_JUMP_LP ),
				PUNCH( LV_LIGHT ),
				KICK( LV_LIGHT ),
				KICK( LV_LIGHT ),
				NAG_VFLIP_ADJ4( LV_LIGHT, LEVER_DOWN | LEVER_FORWARD ),
				IF_RND( 8 ),
					NAG_VFLIP_ADJ4( LV_HEAVY, LEVER_DOWN | LEVER_FORWARD ),
				CMD_ENDIF,
			CMD_ELSE,
			CMD_ENDIF,
			JUMP_FLAG_CLR,
		CMD_ELSE,
			JUMP_FLAG_CLR,
		CMD_ENDIF,
		IF_AREA( 48, 48, 48, 48 ),
			CROUCH,
			NAG_VFLIP_ADJ4( LV_HEAVY, LEVER_DOWN ),
			STAND,
		CMD_ELSE,
		CMD_ENDIF,
		CANCEL_CLR,
	CMD_ELSE,
		WAIT2( 4 ),
		CANCEL_CLR,
	CMD_ENDIF,
	WAIT2( 8 ),
	JUMP_RET,
};

const Uint8 lib25_cmd_07[] = {
	IF_DIST( 24, 0, 24, 0 ),
		IF_CATCH,
			NAG_VFLIP_ADJ2( 1, LEVER_FORWARD ),
		CMD_ELSE,
			JUMP( DIR_UP ),
			JUMP_FLAG_SET,
				TIMER_SET( 80 ),
				IF_HEIGHT( 0xff, 80 ),
					NAG_VFLIP_ADJ4( LV_HEAVY, LEVER_DOWN | LEVER_FORWARD ),
				CMD_ENDIF,
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_DIST( 48, 0, 48, 0 ),
			JUMP( DIR_UP ),
			JUMP_FLAG_SET,
				TIMER_SET( 80 ),
				IF_HEIGHT( 0xff, 96 ),
					NAG_VFLIP_ADJ4( LV_LIGHT, LEVER_DOWN | LEVER_FORWARD ),
				CMD_ENDIF,
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			IF_RND( 12 ),
				PUNCH( LV_HEAVY ),
			CMD_ELSE,
				CMD_JUMP( 9 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib25_cmd_08[] = {
	IF_DIST( 24, 0, 24, 0 ),
		IF_CATCH,
			NAG_VFLIP_ADJ2( 1, LEVER_FORWARD ),
		CMD_ELSE,
			SJUMP( DIR_UP ),
			SJUMP_FLAG_SET,
				TIMER_SET( 0xf0 ),
				IF_HEIGHT( 1, 80 ),
					NAG_VFLIP_ADJ4( LV_HEAVY, LEVER_DOWN | LEVER_FORWARD ),
				CMD_ENDIF,
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_DIST( 48, 0, 48, 0 ),
			SJUMP( DIR_UP ),
			SJUMP_FLAG_SET,
				TIMER_SET( 0xf0 ),
				IF_HEIGHT( 1, 96 ),
					NAG_VFLIP_ADJ4( LV_LIGHT, LEVER_DOWN | LEVER_FORWARD ),
				CMD_ENDIF,
				JUMP_FLAG_CLR,
			CMD_ENDIF,
		CMD_ELSE,
			IF_RND( 12 ),
				PUNCH( LV_HEAVY ),
			CMD_ELSE,
				CMD_JUMP( 9 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib25_cmd_09[] = {
	IF_EM_STANCE( STANCE_AIR ),
		IF_AREA( 64, 128, 96, 128 ),
			SP_CMD( pl25_sp_brast, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			PARTNER_CALL,
			DASH_B( 64 ),
			IF_GAUGE( 3 ),
				SP_CMD_SET( pl25_sp_inferno, 0, PK_PUNCH, LV_LIGHT, 0, 1, 30 ),
				SP_CMD( pl25_sp_fire, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				SP_CMD( pl25_sp_frei, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_AREA( 40, 0, 40, 0 ),
			CMD_JUMP( 10 ),
		CMD_ELSE,
			SP_CMD( pl25_sp_fire, 0, PK_PUNCH, LV_LIGHT, 0 ),
			IF_RND( 28 ),
				SP_CMD( pl25_sp_fire, 0, PK_PUNCH, LV_LIGHT, 0 ),
				IF_RND( 16 ),
					SP_CMD( pl25_sp_fire, 0, PK_PUNCH, LV_LIGHT, 0 ),
					IF_RND( 8 ),
						SP_CMD( pl25_sp_fire, 0, PK_PUNCH, LV_LIGHT, 0 ),
					CMD_ELSE,
						IF_RND( 8 ),
							SP_CMD( pl25_sp_frei, 0, PK_PUNCH, LV_LIGHT, 0 ),
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ELSE,
					IF_RND( 8 ),
						SP_CMD( pl25_sp_frei, 0, PK_PUNCH, LV_HEAVY, 0 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ELSE,
				IF_RND( 8 ),
					SP_CMD( pl25_sp_frei, 0, PK_PUNCH, LV_HEAVY, 0 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	IF_RND( 10 ),
		IF_GAUGE( 3 ),
			SP_CMD( pl25_sp_inferno, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ENDIF,
	IF_VITAL( 128 ),
		CMD_JUMP( 0 ),
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib25_cmd_10[] = {
	PARTNER_CALL,
	IF_RND( 8 ),
		SP_CMD( pl25_sp_teleport, 0, PK_PUNCH, LV_HEAVY, 0 ),
	CMD_ELSE,
		IF_RND( 10 ),
			SP_CMD( pl25_sp_teleport, 0, PK_PUNCH, 3, 0 ),
		CMD_ELSE,
			IF_RND( 16 ),
				SP_CMD( pl25_sp_teleport, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				SP_CMD( pl25_sp_teleport, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	IF_CATCH,
		CROUCH,
		NAG_VFLIP_ADJ2( 1, LEVER_FORWARD ),
		STAND,
	CMD_ELSE,
		CROUCH,
		KICK( LV_HEAVY ),
		WAIT( 1 ),
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib25_cmd_11[] = {
	IF_DIST( 24, 0, 24, 0 ),
		IF_CATCH,
			NAG_VFLIP_ADJ( 1, LEVER_FORWARD ),
		CMD_ELSE,
			KICK( LV_LIGHT ),
			KICK( LV_HEAVY ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_DIST( 48, 0, 48, 0 ),
			IF_RND( 10 ),
				CROUCH,
				NAG_VFLIP_ADJ4( LV_LIGHT, LEVER_FORWARD ),
				NAG_VFLIP_ADJ4( LV_LIGHT, LEVER_FORWARD ),
				NAG_VFLIP_ADJ4( LV_HEAVY, LEVER_FORWARD ),
				STAND,
			CMD_ELSE,
				IF_RND( 8 ),
					KICK( LV_LIGHT ),
				CMD_ELSE,
					IF_RND( 12 ),
						PUNCH( LV_LIGHT ),
					CMD_ELSE,
						IF_RND( 16 ),
							KICK( LV_HEAVY ),
						CMD_ELSE,
							PUNCH( LV_HEAVY ),
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			IF_RND( 16 ),
				JUMP( DIR_FORWARD ),
				JUMP_FLAG_SET,
					WAIT( 10 ),
					TIMER_SET( 120 ),
					IF_EM_ATCK( ATCK_JUMP_HP ),
						PUNCH( LV_HEAVY ),
					CMD_ENDIF,
					JUMP_FLAG_CLR,
				CMD_ENDIF,
			CMD_ELSE,
				CANCEL_SET2,
					PUNCH( LV_HEAVY ),
					SP_CMD( pl25_sp_frei, 0, PK_PUNCH, LV_HEAVY, 0 ),
					CANCEL_CLR,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib25_cmd_12[] = {
	IF_AREA( 32, 48, 160, 48 ),
		KICK( LV_LIGHT ),
		WALK_F( 16 ),
		IF_RND( 16 ),
			KICK( LV_LIGHT ),
			WALK_F( 16 ),
		CMD_ELSE,
			CANCEL_SET2,
				PUNCH( LV_LIGHT ),
				PUNCH( LV_LIGHT ),
				CANCEL_CLR,
			CMD_ENDIF,
			IF_GAUGE( 3 ),
				WAIT( 10 ),
				SP_CMD( pl25_sp_strike, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				SP_CMD( pl25_sp_brast, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ENDIF,
			WALK_F( 16 ),
		CMD_ENDIF,
		IF_RND( 16 ),
			KICK( LV_HEAVY ),
			WALK_F( 16 ),
		CMD_ELSE,
			PUNCH( LV_HEAVY ),
			WALK_F( 16 ),
		CMD_ENDIF,
	CMD_ELSE,
		IF_AREA( 20, 0, 20, 0 ),
			KICK( LV_LIGHT ),
			KICK( LV_LIGHT ),
			SP_CMD( pl25_sp_fire, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			TIMER_SET( 30 ),
			WALK_RND( 2 ),
			IF_RND( 16 ),
				PUNCH( LV_HEAVY ),
			CMD_ELSE,
				KICK( LV_HEAVY ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib25_cmd_13[] = {
	CANCEL_SET2,
		CROUCH,
		PUNCH( LV_LIGHT ),
		PUNCH( LV_LIGHT ),
		STAND,
		PUNCH( LV_HEAVY ),
		CANCEL_CLR,
	CMD_ELSE,
		CROUCH,
		KICK( LV_HEAVY ),
		STAND,
		CANCEL_CLR,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib25_cmd_14[] = {
	IF_RND( 16 ),
		CANCEL_SET,
			CROUCH,
			KICK( LV_LIGHT ),
			STAND,
			PUNCH( LV_LIGHT ),
			PUNCH( LV_HEAVY ),
			CANCEL_CLR,
		CMD_ELSE,
			CROUCH,
			PUNCH( LV_HEAVY ),
			STAND,
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ELSE,
		NAG_VFLIP_ADJ( 1, LEVER_FORWARD ),
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib25_cmd_15[] = {
	IF_CATCH,
		IF_RND( 16 ),
			WAIT2( 6 ),
			NAG_VFLIP_ADJ2( 1, LEVER_FORWARD ),
		CMD_ELSE,
			NAG_VFLIP_ADJ2( 1, LEVER_FORWARD ),
		CMD_ENDIF,
	CMD_ELSE,
		CANCEL_SET2,
			CROUCH,
			PUNCH( LV_LIGHT ),
			STAND,
			PUNCH( LV_HEAVY ),
			CANCEL_CLR,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib25_cmd_16[] = {
	JUMP_RET,
};

const Uint8 lib25_cmd_17[] = {
	TIMER_SET( 30 ),
	IF_RND( 10 ),
		IF_RND( 13 ),
			SP_CMD( pl25_sp_frei, 0, PK_PUNCH, LV_HEAVY, 0 ),
		CMD_ELSE,
			IF_RND( 16 ),
				SP_CMD( pl25_sp_frei, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ELSE,
				SP_CMD( pl25_sp_frei, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		TIMER_SET( 30 ),
		WALK_ATCK( pl25_at_brast_l ),
		CMD_ENDIF,
		IF_EM_ATCK( pl25_at_brast_l ),
			IF_RND( 13 ),
				SP_CMD( pl25_sp_brast, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ELSE,
				IF_RND( 16 ),
					SP_CMD( pl25_sp_brast, 0, PK_PUNCH, LV_HEAVY, 0 ),
				CMD_ELSE,
					SP_CMD( pl25_sp_brast, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ELSE,
			WALK_B( 64 ),
			IF_RND( 13 ),
				SP_CMD( pl25_sp_fire, 0, PK_PUNCH, LV_HEAVY, 0 ),
			CMD_ELSE,
				IF_RND( 16 ),
					SP_CMD( pl25_sp_fire, 0, PK_PUNCH, LV_HEAVY, 0 ),
				CMD_ELSE,
					SP_CMD( pl25_sp_fire, 0, PK_PUNCH, LV_LIGHT, 0 ),
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	IF_LEVEL( 12 ),
		IF_GAUGE( 3 ),
			SP_CMD( pl25_sp_inferno, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
	CMD_ELSE,
		WAIT2( 4 ),
	CMD_ENDIF,
	CMD_JUMP( 0 ),
	JUMP_RET,
};

const Uint8 lib25_cmd_18[] = {
	JUMP_RET,
};

const Uint8 lib25_cmd_19[] = {
	IF_LEVEL( 28 ),
		GUARD( 30 ),
	CMD_ELSE,
		IF_LEVEL( 22 ),
			IF_RND( 28 ),
				GUARD( 30 ),
			CMD_ELSE,
				WAIT2( 4 ),
				GUARD( 30 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 12 ),
				IF_RND( 18 ),
					GUARD( 30 ),
				CMD_ELSE,
					WAIT2( 6 ),
					GUARD( 30 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 4 ),
					IF_RND( 1 ),
						WAIT( 10 ),
					CMD_ELSE,
						IF_RND( 16 ),
							GUARD( 30 ),
						CMD_ELSE,
							WAIT2( 8 ),
							GUARD( 30 ),
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ELSE,
					IF_RND( 2 ),
						WAIT( 10 ),
					CMD_ELSE,
						IF_RND( 8 ),
							WAIT2( 14 ),
							GUARD( 30 ),
						CMD_ELSE,
							GUARD( 30 ),
						CMD_ENDIF,
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib25_cmd_20[] = {
	WAIT2( 6 ),
	IF_LEVEL( 28 ),
		TIMER2_SET( 0xc8 ),
	CMD_ELSE,
		IF_LEVEL( 24 ),
			IF_RND( 28 ),
				TIMER2_SET( 0xc8 ),
			CMD_ELSE,
				WAIT2( 10 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_LEVEL( 20 ),
				IF_RND( 18 ),
					TIMER2_SET( 0xc8 ),
				CMD_ELSE,
					WAIT2( 12 ),
				CMD_ENDIF,
			CMD_ELSE,
				IF_LEVEL( 16 ),
					IF_RND( 10 ),
						TIMER2_SET( 0xc8 ),
					CMD_ELSE,
						WAIT2( 12 ),
					CMD_ENDIF,
				CMD_ELSE,
					IF_RND( 6 ),
						TIMER2_SET( 0xc8 ),
					CMD_ELSE,
						WAIT2( 14 ),
					CMD_ENDIF,
				CMD_ENDIF,
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib25_cmd_21[] = {
	IF_RND( 16 ),
		PARTNER_CALL,
	CMD_ENDIF,
	WAIT2( 6 ),
	TIMER_SET( 5 ),
	IF_AREA( 48, 64, 48, 96 ),
		SP_CMD( pl25_sp_brast, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ELSE,
		WAIT2( 4 ),
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib25_cmd_22[] = {
	JUMP_RET,
};

const Uint8 lib25_cmd_23[] = {
	JUMP_RET,
};

const Uint8 lib25_cmd_24[] = {
	JUMP_RET,
};

const Uint8 lib25_cmd_25[] = {
	JUMP_RET,
};

const Uint8 lib25_cmd_26[] = {
	JUMP_RET,
};

const Uint8 lib25_cmd_27[] = {
	TIMER_SET( 20 ),
	IF_AREA( 48, 0, 48, 0 ),
		SP_CMD( pl25_sp_strike, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ELSE,
		SP_CMD( pl25_sp_inferno, 0, PK_PUNCH, LV_LIGHT, 0 ),
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib25_cmd_28[] = {
	JUMP_RET,
};

const Uint8 lib25_cmd_29[] = {
	JUMP_RET,
};

const Uint8 lib25_cmd_30[] = {
	JUMP_RET,
};

const Uint8 lib25_cmd_31[] = {
	TIMER_SET( 5 ),
	IF_AREA( 48, 0, 0, 0 ),
		CANCEL_SET,
			PUNCH( LV_HEAVY ),
			IF_RND( 16 ),
				SP_CMD( pl25_sp_frei, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				SP_CMD( pl25_sp_fire, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
			CANCEL_CLR,
		CMD_ENDIF,
		IF_EM_STANCE( STANCE_AIR ),
			SP_CMD( pl25_sp_brast, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			IF_RND( 16 ),
				SP_CMD( pl25_sp_frei, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ELSE,
				SP_CMD( pl25_sp_fire, 0, PK_PUNCH, LV_LIGHT, 0 ),
			CMD_ENDIF,
		CMD_ENDIF,
	CMD_ELSE,
		IF_RND( 16 ),
			SP_CMD( pl25_sp_frei, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			SP_CMD( pl25_sp_fire, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
		WAIT2( 14 ),
		IF_RND( 16 ),
			SP_CMD( pl25_sp_frei, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ELSE,
			SP_CMD( pl25_sp_fire, 0, PK_PUNCH, LV_LIGHT, 0 ),
		CMD_ENDIF,
		WAIT2( 14 ),
		STAND,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib25_cmd_32[] = {
	JUMP_RET,
};

const Uint8 lib25_cmd_33[] = {
	JUMP_RET,
};

const Uint8 lib25_cmd_34[] = {
	JUMP_RET,
};

const Uint8 lib25_cmd_35[] = {
	JUMP_RET,
};

const Uint8 lib25_cmd_36[] = {
	JUMP_RET,
};

const Uint8 lib25_cmd_37[] = {
	WAIT( 1 ),
	JUMP_RET,
};

const Uint8 lib25_cmd_38[] = {
	JUMP_RET,
};

const Uint8 lib25_cmd_39[] = {
	JUMP_RET,
};

const Uint8 lib25_cmd_40[] = {
	JUMP_RET,
};

const Uint8 lib25_cmd_41[] = {
	JUMP_RET,
};

const Uint8 lib25_cmd_42[] = {
	JUMP_RET,
};

const Uint8 lib25_cmd_43[] = {
	JUMP_RET,
};

const Uint8 lib25_cmd_44[] = {
	JUMP_RET,
};

const Uint8 lib25_cmd_45[] = {
	JUMP_RET,
};

const Uint8 lib25_cmd_46[] = {
	IF_AREA2( 144, 144, 0, 0 ),
		GUARD( 60 ),
	CMD_ELSE,
		SJUMP( DIR_FORWARD ),
		SJUMP_FLAG_SET,
			WAIT( 16 ),
			GUARD( 30 ),
			JUMP_FLAG_CLR,
		CMD_ENDIF,
		STAND,
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib25_cmd_47[] = {
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
									CMD_JUMP( 12 ),
								CMD_ELSE,
									CMD_JUMP( 10 ),
								CMD_ENDIF,
							CMD_ELSE,
								IF_RND( 10 ),
									CMD_JUMP( 12 ),
								CMD_ELSE,
									CMD_JUMP( 10 ),
								CMD_ENDIF,
							CMD_ENDIF,
						CMD_ENDIF,
					CMD_ELSE,
						IF_LEVEL( 24 ),
							CMD_JUMP( 12 ),
						CMD_ELSE,
							IF_RND( 10 ),
								DASH_ATCK( ATCK_CROUCH_HK ),
									STAND,
									NAG_VFLIP_ADJ4( LV_LIGHT, LEVER_DOWN | LEVER_FORWARD ),
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

const Uint8 lib25_cmd_48[] = {
	IF_RND( 8 ),
		TIMER2_SET( 60 ),
	CMD_ELSE,
		TIMER2_SET( 20 ),
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib25_cmd_49[] = {
	IF_RND( 10 ),
		TIMER2_SET( 60 ),
	CMD_ELSE,
		TIMER2_SET( 20 ),
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib25_cmd_50[] = {
	IF_RND( 12 ),
		TIMER2_SET( 60 ),
	CMD_ELSE,
		TIMER2_SET( 20 ),
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib25_cmd_51[] = {
	IF_RND( 18 ),
		TIMER2_SET( 60 ),
	CMD_ELSE,
		TIMER2_SET( 20 ),
	CMD_ENDIF,
	JUMP_RET,
};

const Uint8 lib25_cmd_52[] = {
	JUMP_FLAG_SET,
		JUMP_LEVEL( 48, 7, 49, 13, 50, 21, 51 ),
		WAIT( 20 ),
		IF_LEVEL( 18 ),
			IF_RND( 24 ),
				JUMP_RND( 40, 41, 4, 4 ),
			CMD_ENDIF,
		CMD_ELSE,
			IF_RND( 16 ),
				JUMP_RND( 38, 38, 39, 39 ),
			CMD_ENDIF,
		CMD_ENDIF,
		JUMP_FLAG_CLR,
	CMD_ENDIF,
	STAND,
	JUMP_RET,
};

const Uint8 lib25_cmd_53[] = {
	JUMP_RET,
};

const Uint8 lib25_cmd_54[] = {
	JUMP_RET,
};

const Uint8 lib25_cmd_55[] = {
	JUMP_RET,
};

const Uint8 lib25_cmd_56[] = {
	JUMP_RET,
};

const Uint8 lib25_cmd_57[] = {
	JUMP_RET,
};

const Uint8 lib25_cmd_58[] = {
	JUMP_RET,
};

const Uint8 lib25_cmd_59[] = {
	JUMP_RET,
};

const Uint8 lib25_cmd_60[] = {
	JUMP_RET,
};

const Uint8 lib25_cmd_61[] = {
	JUMP_RET,
};

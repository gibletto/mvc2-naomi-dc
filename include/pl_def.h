/* The player's constants: stances, states, acts, animation groups and common sounds, with the
   tests of a work's character. player.h includes it. */
#ifndef PL_DEF_H
#define PL_DEF_H

/* pl_stance */
#define STANCE_STAND	0
#define STANCE_CROUCH	1
#define STANCE_AIR		2
#define STANCE_DOWN		3	/* lying on the ground */

/* pl_color_set() colours */
#define PC_DEFAULT		0

/* the stick held away from the opponent, in pl_sw_now */
#define BACK			0x0400

/* the camera back to its usual mode */
#define SCR_EXCEP_SET	scr_excep_set( pl_w, 0, 0 )

/* a new attack: it can hit again. A list of statements, not a block */
#define HIT_RESET( pl )	\
	( pl )->atck_attr = 0 ;	\
	( pl )->hit_flag = 0 ;	\
	( pl )->unk_1c4 = 0 ;	\
	vs_sys_w->atck_cnt[( pl )->pl_no] ++

/* stop moving sideways */
#define RATE_CLR_X( pl )	{ ( pl )->spd.x = 0.0f ; ( pl )->acc.x = 0.0f ; }

/* tests a pair of flag words against a pair of masks */
#define FLAG_CK( f, m0, m1 )	( ( ( f )[0] & ( m0 ) ) | ( ( f )[1] & ( m1 ) ) )

/* a character's bit in pl_bit[0] and pl_bit[1], and tests of a work's character */
#define PLB0( n )	( ( n ) < 32 ? 1UL << ( ( n ) & 31 ) : 0 )
#define PLB1( n )	( ( n ) < 32 ? 0 : 1UL << ( ( n ) & 31 ) )
#define PL_CK( w, n )	FLAG_CK( ( w )->pl_bit, PLB0( n ), PLB1( n ) )
/* Abyss, in any form */
#define ABYSS_CK( w )	FLAG_CK( ( w )->pl_bit, PLB0( PL_ABYSS1 ) | PLB0( PL_ABYSS2 ) | PLB0( PL_ABYSS3 ), 0 )
/* the small ones, held lower in throws */
#define SMALL_CK( w )	FLAG_CK( ( w )->pl_bit, PLB0( PL_ROLL ), PLB1( PL_SERVBOT ) )
/* they jump off the walls */
#define WALL_JUMP_CK( w )	FLAG_CK( ( w )->pl_bit, PLB0( PL_STRIDER ) | PLB0( PL_WOLVERINE ) | PLB0( PL_SPIDERMAN ) | PLB0( PL_MARROW ) | PLB0( PL_CHUN_LI ) | PLB0( PL_MEGAMAN ), PLB1( PL_FELICIA ) | PLB1( PL_SABRETOOTH ) | PLB1( PL_BONE_WOLVERINE ) )
/* they jump in the air */
#define AIR_JUMP_CK( w )	FLAG_CK( ( w )->pl_bit, PLB0( PL_STRIDER ) | PLB0( PL_CYCLOPS ) | PLB0( PL_PSYLOCKE ) | PLB0( PL_CAPTAIN_AMERICA ) | PLB0( PL_MARROW ) | PLB0( PL_CHUN_LI ) | PLB0( PL_ROLL ) | PLB0( PL_BB_HOOD ), PLB1( PL_CAMMY ) )
/* twice */
#define AIR_JUMP2_CK( w )	FLAG_CK( ( w )->pl_bit, PLB0( PL_PSYLOCKE ) | PLB0( PL_CHUN_LI ), 0 )
/* they have a speed-up, which slows their gauge */
#define SPEED_UP_CK( w )	FLAG_CK( ( w )->pl_bit, PLB0( PL_WOLVERINE ) | PLB0( PL_ROGUE ), PLB1( PL_SPIRAL ) | PLB1( PL_BONE_WOLVERINE ) )
/* the metal bodies */
#define METAL_CK( w )	FLAG_CK( ( w )->pl_bit, PLB0( PL_DOOM ) | PLB0( PL_TRON ), PLB1( PL_WAR_MACHINE ) | PLB1( PL_IRON_MAN ) | PLB1( PL_SENTINEL ) )
/* their air dash takes any of eight ways */
#define DASH8_CK( w )	FLAG_CK( ( w )->pl_bit, 0, PLB1( PL_WAR_MACHINE ) | PLB1( PL_IRON_MAN ) )
/* theirs only forward or back */
#define DASH2_CK( w )	FLAG_CK( ( w )->pl_bit, PLB0( PL_SONSON ), PLB1( PL_BLACKHEART ) )
/* they start floating */
#define FLOAT_CK( w )	FLAG_CK( ( w )->pl_bit, 0, PLB1( PL_STORM ) | PLB1( PL_MAGNETO ) )

/* a player's routine_0 */
#define	PL_APPEAR		0	/* the entrance (pl_move_appear) */
#define	PL_FIGHT		1	/* the fight (pl_move_fight) */
/* and its routine_1 in the fight */
#define	PL_NORM			0
#define	PL_CATCH		1	/* throwing (pl_catch) */
#define	PL_CAUGHT		2	/* being thrown or held (pl_caught) */
#define	PL_DAMAGE		3	/* hit (pl_dm_move) */

/* act_st_set() acts */
#define	N_STAND			0	/* standing */
#define	N_WALK			1	/* walking */
#define	N_JUMP			2	/* a jump's take-off */
#define	N_JUMP_AIR		3	/* in a jump, or falling back into one */
#define	N_LAND			4	/* landing (pl_land_set) */
#define	N_CROUCH		5	/* crouching down */
#define	N_CROUCHING		6	/* crouching */
#define	N_STAND_UP		7	/* standing up from a crouch */
#define	N_TURN			8	/* turning round, standing */
#define	N_CROUCH_TURN	9	/* turning round, crouching */
#define	N_DASH			10	/* dashing forward */
#define	N_BACK_DASH		11	/* dashing back */
#define	N_DIZZY			12	/* stunned after a crumple, until mashed out or timed out */
#define	N_SJUMP			13	/* a super jump's take-off (pl_sjump_set) */
#define	N_SJUMP_AIR		14	/* in a super jump (pl_sjump_ret) */
#define	N_AIR_RECOVER	15	/* righting itself in the air after being knocked away */
#define	N_FALL			16	/* falling, from no speed */
#define	N_GET_UP		17	/* getting up off the floor, or rolling up */
#define	N_GUARD			18	/* guarding */
#define	N_AIR_GUARD		19	/* guarding in the air */
#define	N_NML_ATCK		20	/* a normal attack (pl_tbl[3]) */
#define	N_SPECIAL		21
#define	N_WIN			22	/* the win pose (pl_tbl[7]) */
#define	N_DOWN			23	/* lying on the floor, or knocked out */
#define	N_FLY			24	/* flying (fly_flag) */
#define	N_WALL_JUMP		25	/* jumping off the screen's edge (pl_wall_jump_ck) */
#define	N_AIR_DASH		26	/* dashing in the air (pl_air_dash_ck) */
#define	N_ESCAPE		27	/* breaking free of a throw */
#define	N_CHANGE		28	/* a team change: a tag, a counter or a partner coming in (change_type) */
#define	N_SUPER			29
#define	N_CATCH			30	/* throwing (pl_catch_set) */
#define	N_CAUGHT		31	/* being thrown */
#define	N_DAMAGE		32	/* hit, in the damage routine (routine_1 3) */
#define	N_CMD_ATCK		34	/* the down-forward attack (pl_cmd_atck_ck, pl_tbl[15]) */

#define	ATCK_H			1

/* pl_char_set() animation groups */
#define	Char_Stand		0	/* standing and walking */
#define	Char_Jump		1	/* jumping, falling and landing */
#define	Char_Dash		2	/* dashing forward (0) and back (1) */
#define	Char_Crouch		3	/* crouching down, crouching, standing up */
#define	Char_Turn		4	/* turning round, standing (0) or crouching (1) */
#define	Char_Guard		5	/* guarding */
#define	Char_Air_Guard	6	/* guarding in the air */
#define	Char_Stand_P	7	/* the standing punches */
#define	Char_Stand_K	8	/* the standing kicks */
#define	Char_Crouch_P	9	/* the crouching punches */
#define	Char_Crouch_K	10	/* the crouching kicks */
#define	Char_Jump_P		11	/* the jumping punches */
#define	Char_Jump_K		12	/* the jumping kicks */
#define	Char_Damage		13	/* hit, knocked down or away, and breaking free */
#define	Char_Dizzy		14	/* stunned (N_DIZZY) */
#define	Char_Catch		15	/* throws */
#define	Char_Caught		16	/* being thrown */
#define	Char_Get_Up		17	/* getting up off the floor */
#define	Char_Appear		18
#define	Char_Win		19
#define	Char_Extra		20	/* the launcher, t_kick, the taunt and command normals */
#define	Char_Special	21	/* the special moves */
#define	Char_Super		22	/* the hyper combos */
#define	Char_Effect		23	/* the effects' own animations */
#define	Char_Change		24	/* a team change (N_CHANGE) */
#define	Char_Fly		26	/* flying */

/* pl_se_req() common sounds */
#define SE_SWING_L	20	/* an attack's swing: light, medium, heavy */
#define SE_SWING_M	21
#define SE_SWING_H	22

#endif

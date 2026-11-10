/* Juggernaut (character 29): command checks, normal attacks, appearance, win and lose scenes,
   special moves and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl29_def.h"

/* the character's own work, in tlocal */
typedef struct PL29_LOCAL_W {
	Uint8	rush_col;		/* 00: the charge of sp_headcrush has set his colour */
	Sint8	flash;			/* 01: the charge's flash, 0 to 3 */
	Sint8	pw_timer;		/* 02: frames left of the power-up's pose, sp_powerup */
	Uint8	unk_03;
	Uint8	pw_on;			/* 04: the power-up is on */
	Uint8	pw_set;			/* 05: sp_powerup asks for it; the next move clears it */
	Sint8	pw_used;		/* 06: a powered hit or a throw has used it */
	Sint8	pw_glow;		/* 07: its glow, 0 to 7 */
	Uint8	unk_08[2];
	Sint16	rush_time;		/* 0a: frames the charge may still last */
	Sint8	rush_hits;		/* 0c: hits the charge has made */
} PL29_LOCAL_W;

/* the code reaches tlocal afresh each time rather than through a pointer */
#define LW	( ( PL29_LOCAL_W * )pl_w->tlocal )

void pl29_main( PLAYER_WORK *pl_w );
void pl29_com_ck( PLAYER_WORK *pl_w );
void pl29_atck_set( PLAYER_WORK *pl_w );
void pl29_atck( PLAYER_WORK *pl_w );
void pl29_dash( PLAYER_WORK *pl_w );
void pl29_noki( PLAYER_WORK *pl_w );
void pl29_appear( PLAYER_WORK *pl_w );
void pl29_win( PLAYER_WORK *pl_w );
void pl29_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl29_catch_ck( PLAYER_WORK *pl_w );
void pl29_catch_set( PLAYER_WORK *pl_w );
void pl29_catch( PLAYER_WORK *pl_w );
void pl29_hold( PLAYER_WORK *pl_w );
int pl29_delay_ck( PLAYER_WORK *pl_w );
void pl29_t_kick( PLAYER_WORK *pl_w );
void pl29_upper( PLAYER_WORK *pl_w );
void pl29_sp_unison( PLAYER_WORK *pl_w );
void pl29_sp_gattai( PLAYER_WORK *pl_w );
void pl29_sp_assist( PLAYER_WORK *pl_w );
void pl29_sp_zero( PLAYER_WORK *pl_w );

/* each attack's table, four bytes: punches then kicks, light to heavy; standing and crouching,
   jumping, super jumping */
static Sint8 nml_tbl[24] = {
	 1,  2,  1,  2,  -1, -1, -1, -1,  -1, -1, -1, -1,
	 1,  2,  1,  2,  -1, -1, -1, -1,  -1, -1, -1, -1,
} ;
static Sint8 j_tbl[24] = {
	 1,  2,  1,  2,  -1, -1, -1, -1,  -1, -1, -1, -1,
	 1,  2,  1,  2,  -1, -1, -1, -1,  -1, -1, -1, -1,
} ;
static Sint8 sj_tbl[24] = {
	 1,  2,  0,  2,  -1,  2,  1,  2,  -1, -1, -1,  2,
	 1,  2,  1,  2,  -1,  2, -1,  2,  -1, -1, -1, -1,
} ;

/* commands: time or steps, kind, the special move, then the stick and buttons */
static Uint16 com_quake[] = { 3, COM_GROUND, pl29_sp_quake, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_PUNCH } ;	/* Earthquake Punch */
static Uint16 com_headcrush[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl29_sp_headcrush, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Juggernaut Headcrush */
static Uint16 com_powerup[] = { 3, COM_GROUND | COM_ALL_SW, pl29_sp_powerup, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_PUNCH } ;	/* Cytorrak Power-up */
static Uint16 com_punch[] = { 5, COM_GROUND, pl29_sp_punch, 0, KEY_BACK, KEY_DOWN | KEY_BACK, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Juggernaut Punch */
static Uint16 com_press[] = { 5, 0, pl29_sp_press, 0, KEY_BACK, KEY_DOWN | KEY_BACK, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Juggernaut Body Press */
static Uint16 com_sp08[] = { 3, COM_GROUND | COM_ALL_SW, pl29_sp_08, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;

static void ( *pl29_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl29_main,
	pl29_com_ck,
	pl29_atck_set,
	pl29_atck,
	pl29_dash,
	pl29_noki,
	pl29_appear,
	pl29_win,
	pl29_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl29_catch_ck,	/* returns a value */
	pl29_catch_set,
	pl29_catch,
	pl29_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl29_delay_ck,
	pl29_t_kick,
	pl29_upper,
	pl29_sp_unison,
	pl29_sp_gattai,
	pl29_sp_assist,
	pl29_sp_zero,
} ;

/* not used */
static Float unk_spd[4] = { DOT_X( 4.0f ), -DOT_X( 4.0f ), DOT_X( 4.0f ), -DOT_X( 4.0f ) } ;

static Uint8 quake_ck( PLAYER_WORK *pl_w );
static Uint8 headcrush_ck( PLAYER_WORK *pl_w );
static Uint8 powerup_ck( PLAYER_WORK *pl_w );
static Uint8 punch_ck( PLAYER_WORK *pl_w );
static Uint8 press_ck( PLAYER_WORK *pl_w );
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl29_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl29_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl29_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( powerup_ck( pl_w ) ) return ;
	if( headcrush_ck( pl_w ) ) return ;
	if( quake_ck( pl_w ) ) return ;
	if( press_ck( pl_w ) ) return ;
	if( punch_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
static Uint8 quake_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_quake, &pl_w->com_w0 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl29_sp_quake ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 headcrush_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_headcrush, &pl_w->com_w1 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl29_sp_headcrush ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 powerup_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_powerup, &pl_w->com_w2 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl29_sp_powerup ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 punch_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_punch, &pl_w->com_w3 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w3 ) ;
	pl_gauge_add( pl_w, 10 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl29_sp_punch ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 press_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_press, &pl_w->com_w4 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w4 ) ;
	pl_gauge_add( pl_w, 10 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl29_sp_press ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 4 ) ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl29_sp_chouhatsu ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int snapback_ck( PLAYER_WORK *pl_w )
{
	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl29_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/****************************************************************/
/*			A hyper combo entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl29_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_headcrush, &pl_w->com_w1 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl29_sp_headcrush ;
	return 1 ;
}

/****************************************************************/
/*			The power-up of sp_powerup and its glow, while it		*/
/*			lasts: it ends with no vitality left, or after a	*/
/*			hit (pw_used) when it is not set again				*/
/****************************************************************/
void pl29_main( PLAYER_WORK *pl_w )
{
	if( LW->rush_col && pl_w->char_no.w != 0x1600 ){
		LW->rush_col = 0 ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
	}
	pl_w->armor_timer = 2 ;
	if( !LW->pw_on ){
		if( !LW->pw_set ) return ;
		LW->pw_on ++ ;
		pl_w->hit_flag = 0 ;
		LW->pw_glow = LW->pw_used = 0 ;
	}
	if( !LW->pw_on ) return ;
	if( !pl_w->vital_new ) goto off ;
	if( LW->pw_set ) goto on ;
	if( LW->pw_used ) goto off ;
	if( pl_w->cg_atck_flag & CG_ATCK_ALL ) goto on ;
off:
	LW->pw_on = LW->pw_set = 0 ;
	pl_w->atck_pow = 0 ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	return ;
on:
	if( pl_w->hit_flag ){
		pl_w->atck_pow = 0 ;
		LW->pw_used = -1 ;
	}
	LW->pw_glow ++ ;
	LW->pw_glow &= 7 ;
	ef_color_set( pl_w, 0, LW->pw_glow + pl_w->pl_color * 10 + 2, 1 ) ;
}

static void nml_atck( PLAYER_WORK *pl_w );
static void jump_atck_ck( PLAYER_WORK *pl_w );
static void jump_atck( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Normal attacks: nml_no is the strength and			*/
/*			atck_pk the punch or kick							*/
/*==============================================================*/
static void nml_atck( PLAYER_WORK *pl_w )
{
	Uint8 no ;

	pl_brake_set( pl_w ) ;
	pl_w->chain_tbl = &nml_tbl[pl_w->atck_pk * 12 + pl_w->nml_no * 4] ;
	pl_w->nml_level = pl_w->nml_no ;
	no = pl_w->pl_stance * 6 + pl_w->atck_pk * 3 + pl_w->nml_no ;
	if( ( pl_w->atck_lever & KEY_FORWARD ) && !pl_w->atck_pk && pl_w->nml_no == 2 && !pl_w->pl_stance ){
		pl_w->atck_no = pl29_at_fwd_hp ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, 3 ) ;
		pls29_00_set( pl_w, 1 ) ;
	} else {
		pl_w->atck_no = no ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, ( pl_w->pl_stance << 1 ) + pl_w->atck_pk + Char_Stand_P, pl_w->nml_no ) ;
	}
	pl_se_req( pl_w, pl_w->nml_no + 20 ) ;
}

/*==============================================================*/
/*			Jump attacks: the chain table for a normal or a		*/
/*			super jump											*/
/*==============================================================*/
static void jump_atck( PLAYER_WORK *pl_w )
{
	register Sint16 no = pl_w->atck_pk * 12 + pl_w->nml_no * 4 ;
	Sint8 *tbl ;

	if( !pl_w->sjump_flag ) tbl = &j_tbl[no] ;
	else tbl = &sj_tbl[no] ;
	pl_w->chain_tbl = tbl ;
	pl_w->nml_level = pl_w->nml_no ;
	pl_w->atck_no = pl_w->atck_pk * 3 + pl_w->nml_no + 12 ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, pl_w->atck_pk + Char_Jump_P, pl_w->nml_no ) ;
	pl_se_req( pl_w, pl_w->nml_no + 20 ) ;
	if( !pl_w->atck_pk ){
		if( pl_w->jump_atk_cnt & JATK_P_MASK ) pl_w->jump_atk_cnt -= 0x01 ;
	} else {
		if( pl_w->jump_atk_cnt & JATK_K_MASK ) pl_w->jump_atk_cnt -= 0x10 ;
	}
}

/*==============================================================*/
/*			In the air, only while that button's jump			*/
/*			attacks are left									*/
/*==============================================================*/
static void jump_atck_ck( PLAYER_WORK *pl_w )
{
	if( !pl_w->atck_pk ){
		if( pl_w->jump_atk_cnt & JATK_P_MASK ) jump_atck( pl_w ) ;
	} else {
		if( pl_w->jump_atk_cnt & JATK_K_MASK ) jump_atck( pl_w ) ;
	}
}

/****************************************************************/
void pl29_atck_set( PLAYER_WORK *pl_w )
{
	static void ( *atck_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_atck, jump_atck_ck, jump_atck, nml_atck,
	} ;

	LW->pw_set = 0 ;
	atck_set_jmp[pl_w->atck_type]( pl_w ) ;
}

/*==============================================================*/
/*			The crouching medium punch's second hit				*/
/*==============================================================*/
static void crouch_mp_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->pl_stance == STANCE_CROUCH && pl_w->nml_no == 1 && pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->atck_no = pl29_at_crouch_mp_2 ;
		HIT_RESET( pl_w ) ;
		scr_work.quake_level = 1 ;
		scr_work.quake_flag = 1 ;
	}
}

/****************************************************************/
void pl29_atck( PLAYER_WORK *pl_w )
{
	switch( pl_w->atck_type ){
	case 3:
		pl_smoke_set( pl_w ) ;
	case 0:
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		pl_brake_stop( pl_w ) ;
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		else if( !pl_w->atck_pk ) crouch_mp_ck( pl_w ) ;
		break ;
	case 2:
		pl_jump_hit_ck( pl_w ) ;
		pl_sjump_ctrl( pl_w ) ;
	case 1:
		pl_jump_move( pl_w ) ;
		pl_sjump_top_ck( pl_w ) ;
		if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
		if( pl_land_ck( pl_w ) ) pl_land_set( pl_w ) ;
		break ;
	}
}

/****************************************************************/
/*			The launcher										*/
/****************************************************************/
void pl29_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_w->atck_no = pl29_at_upper ;
		HIT_RESET( pl_w ) ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_gauge_add( pl_w, 5 ) ;
		pl_char_set( pl_w, Char_Extra, 6 ) ;
		pl_se_req( pl_w, SE_SWING_H ) ;
	}
	if( pl_w->atck_type == 3 ) pl_smoke_set( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_brake_stop( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void dash_init( PLAYER_WORK *pl_w );
static void dash_move( PLAYER_WORK *pl_w );
static void dash_stop( PLAYER_WORK *pl_w );
static void dash_end( PLAYER_WORK *pl_w );

/****************************************************************/
void pl29_dash( PLAYER_WORK *pl_w )
{
	static void ( *dash_jmp[])( PLAYER_WORK *pl_w ) = {
		dash_init, dash_move, dash_stop, dash_end,
	} ;

	dash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dash_init( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? 10.0f : -10.0f ;
	}
}

/*--------------------------------------------------------------*/
static void dash_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) <= 0 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 4.0f ) : -DOT_X( 4.0f ) ;
		pl_char_set( pl_w, Char_Dash, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void dash_stop( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void dash_end( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void noki_init( PLAYER_WORK *pl_w );
static void noki_jump( PLAYER_WORK *pl_w );
static void noki_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash, a hop								*/
/****************************************************************/
void pl29_noki( PLAYER_WORK *pl_w )
{
	static void ( *noki_jmp[])( PLAYER_WORK *pl_w ) = {
		noki_init, noki_jump, noki_land,
	} ;

	noki_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void noki_init( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->routine_2 ++ ;
		pl_w->spd.y = DOT_X( 0.5f ) ;
		pl_w->acc.y = -0.15625f ;
		pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 7.0f ) : DOT_X( 7.0f ) ;
		pl_w->acc.x = pl_w->pl_dir ? 0.104166664f : -0.104166664f ;
		noki_jump( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void noki_jump( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->ground_y < pl_w->pos.y ) return ;
	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 4.0f ) : DOT_X( 4.0f ) ;
	pl_char_set( pl_w, Char_Dash, 3 ) ;
}

/*--------------------------------------------------------------*/
static void noki_land( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void appear_init( PLAYER_WORK *pl_w );
static void appear_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl29_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_init,
		appear_move,
	} ;

	appear_jmp[ pl_w->routine_2 ]( pl_w ) ;
}

/*==============================================================*/
static void appear_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->disp_flag = 1 ;
	pl_w->timer0 = 60 ;
	pl_char_set( pl_w, Char_Appear, 0 ) ;
}

/*--------------------------------------------------------------*/
static void appear_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->timer0 ) pl_w->routine_1 ++ ;
	else pl_w->timer0 -- ;
}

/*==============================================================*/
/*			Back to neutral once the scene is over				*/
/*==============================================================*/
static void win_end_ck( PLAYER_WORK *pl_w )
{
	if( pl_win_end_ck( pl_w ) ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			Walks forward, then punches							*/
/*==============================================================*/
static void win_ko( PLAYER_WORK *pl_w )
{
	switch( pl_w->routine_3 ){
	case 0:
		pl_w->routine_3 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = -DOT_X( 2.0f ) ;
		if( pl_w->disp_dir ) pl_w->spd.x = -pl_w->spd.x ;
		pl_char_set( pl_w, Char_Stand, 2 ) ;
		break ;
	case 1:
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_char_move( pl_w ) ;
		if( vs_sys_work.push_bit ){
			pl_w->atck_no = ATCK_STAND_MK ;
			HIT_RESET( pl_w ) ;
			pl_w->routine_3 ++ ;
			pl_char_set( pl_w, Char_Stand_K, 1 ) ;
		}
		break ;
	case 2:
		if( pl_char_move( pl_w ) < 0 ){
			pl_w->routine_3 ++ ;
			pl_char_set( pl_w, Char_Win, 1 ) ;
		} else if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			scr_work.quake_level = 1 ;
			scr_work.quake_flag = 1 ;
		}
		break ;
	case 3:
		pl_char_move( pl_w ) ;
		break ;
	}
	win_end_ck( pl_w ) ;
}

/*==============================================================*/
static void win_pose( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_3 ){
		pl_w->timer0 = 60 ;
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Win, 6 ) ;
	} else {
		pl_char_move( pl_w ) ;
	}
	win_end_ck( pl_w ) ;
}

/*==============================================================*/
static void win_pose_ef( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_3 ){
		pl_w->timer0 = 60 ;
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Win, 7 ) ;
	} else {
		pl_char_move( pl_w ) ;
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pls29_00_set( pl_w, 7 ) ;
		}
	}
	win_end_ck( pl_w ) ;
}

/*==============================================================*/
static void win_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	win_end_ck( pl_w ) ;
}

/* the win poses for a KO, picked at random; the second four when the match is over too (vs_end_flag) */
static Uint8 win_type_tbl[8] = { 6, 7, 6, 7, 0, 1, 6, 7 } ;

/****************************************************************/
void pl29_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko, win_ko, win_move, win_move, win_move, win_move, win_pose, win_pose_ef,
	} ;
	int ofs ;

	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
		switch( pl_w->type ){
		case 0:
			if( PL_CK( pl_w->target_w, PL_ABYSS3 ) ) pl_w->type = 6 ;
			else {
				ofs = !vs_sys_work.vs_end_flag ? 0 : 4 ;
				pl_w->type = win_type_tbl[( Sys->game_timer0 & 3 ) + ofs] ;
			}
			break ;
		case 1:
			pl_w->type = 2 ;
			pl_char_set( pl_w, Char_Win, 2 ) ;
			break ;
		case 2:
		case 3:
			if( vs_sys_work.winner_id == pl_w->pl_no ){
				pl_w->type = 7 ;
				break ;
			}
			pl_w->type = 5 ;
			pl_char_set( pl_w, Char_Win, 5 ) ;
			break ;
		case 4:
			pl_w->type = 3 ;
			pl_char_set( pl_w, Char_Win, 5 ) ;
			break ;
		}
	} else {
		win_jmp[pl_w->type]( pl_w ) ;
	}
}

static void sp_quake( PLAYER_WORK *pl_w );
static void sp_headcrush( PLAYER_WORK *pl_w );
static void sp_powerup( PLAYER_WORK *pl_w );
static void sp_punch( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_press( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );

/****************************************************************/
void pl29_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *pl_sp_tbl[])( PLAYER_WORK *pl_w ) = {
		sp_quake,					/* 00 Earthquake Punch */
		sp_headcrush,					/* 01 Juggernaut Headcrush (hyper) */
		sp_powerup,					/* 02 Cytorrak Power-up */
		sp_punch,					/* 03 Juggernaut Punch */
		sp_chouhatsu,					/* 04 Taunt */
		pl_stand_ret,			/* 05 */
		pl_stand_ret,			/* 06 */
		sp_press,					/* 07 Juggernaut Body Press */
		pl_stand_ret,			/* 08 */
		sp_snapback,					/* 09 Snapback */
	} ;

	pl_sp_tbl[pl_w->pl_sp_type]( pl_w ) ;
}

static void quake_init( PLAYER_WORK *pl_w );
static void quake_move( PLAYER_WORK *pl_w );
static void quake_wait( PLAYER_WORK *pl_w );
static void quake_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A punch at the ground that throws out three			*/
/*			pieces, five at the higher strength					*/
/*==============================================================*/
static void sp_quake( PLAYER_WORK *pl_w )
{
	static void ( *quake_jmp[])( PLAYER_WORK *pl_w ) = {
		quake_init, quake_move, quake_wait, quake_end,
	} ;

	quake_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void quake_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	LW->pw_set = 0 ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_dir_set( pl_w ) ;
	pl_w->atck_no = pl29_at_quake ;
	HIT_RESET( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level << 1 ) ;
}

/*--------------------------------------------------------------*/
static void quake_move( PLAYER_WORK *pl_w )
{
	Sint8 i, n ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		scr_work.quake_level = 3 ;
		scr_work.quake_flag = 1 ;
		pl_w->timer0 = pl_w->atck_level ? 56 : 46 ;
		shl29_00_set( pl_w, 0, 0 ) ;
		n = pl_w->atck_level ? 5 : 3 ;
		for( i = 0 ; i < n ; i++ ) shl29_00_set( pl_w, 1, i ) ;
		pl_w->unk_27b = 0 ;
		pl_w->cancel_timer = 16 ;
	}
}

/*--------------------------------------------------------------*/
static void quake_wait( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !--pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void quake_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void headcrush_init( PLAYER_WORK *pl_w );
static void headcrush_start( PLAYER_WORK *pl_w );
static void headcrush_rush( PLAYER_WORK *pl_w );
static void headcrush_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo: a charge that hits again and			*/
/*			again; pressed buttons add hits						*/
/*==============================================================*/
static void sp_headcrush( PLAYER_WORK *pl_w )
{
	static void ( *headcrush_jmp[])( PLAYER_WORK *pl_w ) = {
		headcrush_init, headcrush_start, headcrush_rush, headcrush_end,
	} ;

	headcrush_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void headcrush_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->pos.y = pl_w->ground_y ;
	LW->pw_set = 0 ;
	LW->rush_col = 1 ;
	LW->flash = 0 ;
	LW->rush_time = 80 ;
	LW->rush_hits = 0 ;
	pl_w->timer1 = 0 ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->atck_no = pl29_at_headcrush ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 0 ) ;
}

/*--------------------------------------------------------------*/
static void headcrush_start( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type & 2 ){
		pl_w->cg_type = 0 ;
		ofs.x = 0.0f ;
		ofs.y = DOT_Y( 96.0f ) ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
		return ;
	}
	if( pl_w->cg_type & 1 ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->timer0 = 0 ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 8.0f ) : -DOT_X( 8.0f ) ;
		pl_w->acc.x = pl_w->pl_dir ? DOT_X( 0.25f ) : -DOT_X( 0.25f ) ;
		pls29_00_set( pl_w, 0 ) ;
	}
}

/* presses needed for each extra hit, and the flashes of the body */
static Uint8 headcrush_hit_tbl[5] = { 0, 10, 14, 18, 24 } ;
static Uint8 headcrush_flash_tbl[4] = { 0, 0, 0, 1 } ;

/*--------------------------------------------------------------*/
static void headcrush_rush( PLAYER_WORK *pl_w )
{
	Uint8 col, i, n, wall, side ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	LW->flash ++ ;
	LW->flash &= 3 ;
	if( ( col = headcrush_flash_tbl[LW->flash] ) ) ef_color_set( pl_w, 0, col + pl_w->pl_color * 10, 1 ) ;
	else pl_color_set( pl_w, PC_DEFAULT ) ;
	if( pl_w->hit_flag ){
		if( !pl_w->timer0 ){
			pl_w->timer0 = 8 ;
			pl_w->hit_flag = 0 ;
			LW->rush_hits ++ ;
		} else {
			pl_w->timer0 -- ;
		}
	}
	if( pl_w->hit_flag != ( HR_GUARD | HR_HIT ) ){	/* never 0x81: the field is signed, as in the arcade */
		if( !LW->rush_hits ){	/* a wall ahead before the first hit ends the rush */
			wall = pl_w->wall_flag ;
			if( wall ){
				side = pl_w->pl_dir ? 1 : 2 ;
				wall &= side ;
			}
			if( wall ) goto finish ;
		}
		if( sw_renda_ck( pl_w ) ){
			if( ++pl_w->timer1 & 0x100 ) pl_w->timer1 -- ;
		}
		for( i = 4, n = 3 ; i >= 0 ; i-- ){
			if( pl_w->timer1 >= headcrush_hit_tbl[i] ){
				n += i ;
				break ;
			}
		}
		if( LW->rush_hits < n && pl_w->target_w->vital_new && --LW->rush_time > 0 ) return ;
	}
finish :
	pl_w->delay_flag = 0 ;
	pl_w->delay_timer = 0 ;
	pl_w->hyper_bg = 0 ;
	pl_w->hyper_bg_tm = 0 ;
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl29_at_headcrush_last ;
	HIT_RESET( pl_w ) ;
	scr_work.quake_level = 3 ;
	scr_work.quake_flag = 1 ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_char_set( pl_w, Char_Super, 1 ) ;
}

/*--------------------------------------------------------------*/
static void headcrush_end( PLAYER_WORK *pl_w )
{
	if( pl_w->hit_flag & HR_GUARD ) pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_char_move( pl_w ) < 0 ){
		LW->rush_col = 0 ;
		pl_stand_ret( pl_w ) ;
	}
}

static void powerup_init( PLAYER_WORK *pl_w );
static void powerup_move( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A power-up kept until a hit uses it (pl29_main);	*/
/*			the pose lasts 50 frames							*/
/*==============================================================*/
static void sp_powerup( PLAYER_WORK *pl_w )
{
	static void ( *powerup_jmp[])( PLAYER_WORK *pl_w ) = {
		powerup_init, powerup_move,
	} ;

	powerup_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void powerup_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->pos.y = pl_w->ground_y ;
	LW->pw_timer = 50 ;
	LW->pw_set = 1 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->atck_pow = 48 ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 4 ) ;
}

/*--------------------------------------------------------------*/
static void powerup_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !--LW->pw_timer ) pl_stand_ret( pl_w ) ;
}

static void punch_init( PLAYER_WORK *pl_w );
static void punch_start( PLAYER_WORK *pl_w );
static void punch_dash( PLAYER_WORK *pl_w );
static void punch_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A punch with a dash forward at the higher			*/
/*			strength											*/
/*==============================================================*/
static void sp_punch( PLAYER_WORK *pl_w )
{
	static void ( *punch_jmp[])( PLAYER_WORK *pl_w ) = {
		punch_init, punch_start, punch_dash, punch_end,
	} ;

	punch_jmp[pl_w->routine_2]( pl_w ) ;
}

/* its speed for each strength */
static Float punch_spd[2] = { 0.0f, 20.0f } ;

/*==============================================================*/
static void punch_init( PLAYER_WORK *pl_w )
{
	LW->pw_set = 0 ;
	pl_w->routine_2 ++ ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_sp_ef_set( pl_w ) ;
	if( pl_w->team_type == 3 ){
		pl_w->atck_no = pl29_at_punch_vc ;
		HIT_RESET( pl_w ) ;
	} else {
		pl_w->atck_no = ( pl_w->atck_level << 1 ) + 48 ;
		HIT_RESET( pl_w ) ;
	}
	pl_char_set( pl_w, Char_Special, ( pl_w->atck_level << 1 ) + 5 ) ;
}

/*--------------------------------------------------------------*/
static void punch_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = punch_spd[pl_w->atck_level] ;
		if( !pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = 0.0f ;
	}
}

/*--------------------------------------------------------------*/
static void punch_dash( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pls29_00_set( pl_w, 2 ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Special, ( pl_w->atck_level << 1 ) + 8 ) ;
	}
}

/*--------------------------------------------------------------*/
static void punch_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	else if( pl_w->cg_prm ){
		pl_w->cg_prm = 0 ;
		scr_work.quake_level = 1 ;
		scr_work.quake_flag = 1 ;
	}
}

static void press_init( PLAYER_WORK *pl_w );
static void press_start( PLAYER_WORK *pl_w );
static void press_jump( PLAYER_WORK *pl_w );
static void press_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A jump that comes down on the opponent				*/
/*==============================================================*/
static void sp_press( PLAYER_WORK *pl_w )
{
	static void ( *press_jmp[])( PLAYER_WORK *pl_w ) = {
		press_init, press_start, press_jump, press_land,
	} ;

	press_jmp[pl_w->routine_2]( pl_w ) ;
}

/* its speeds for each strength, sideways then up; the third is not used */
static Float press_spd[3][2] = {
	{ DOT_X( 2.0f ), 30.0f },
	{ DOT_X( 5.0f ), DOT_Y( 18.0f ) },
	{ 0.0f, 0.0f },
} ;

/*==============================================================*/
static void press_init( PLAYER_WORK *pl_w )
{
	Uint8 no ;

	pl_w->routine_2 ++ ;
	LW->pw_set = 0 ;
	pl_dir_set( pl_w ) ;
	if( pl_w->pl_stance != STANCE_AIR ){
		pl_sp_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, ( pl_w->atck_level << 1 ) + 18 ) ;
	} else {
		pl_char_set2( pl_w, Char_Special, ( pl_w->atck_level << 1 ) + 18, 1 ) ;
	}
	no = pl_w->atck_level ? 61 : 59 ;
	pl_w->atck_no = no ;
	HIT_RESET( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void press_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_air_set( pl_w ) ;
		pl_w->spd.y = press_spd[pl_w->atck_level][1] ;
		pl_w->acc.y = -2.142857f ;
		pl_w->spd.x = press_spd[pl_w->atck_level][0] ;
		pl_w->acc.x = -0.008370535f ;
		if( !pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
	}
}

/*--------------------------------------------------------------*/
static void press_jump( PLAYER_WORK *pl_w )
{
	Sint8 no ;

	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->spd.y > 0.0f ) return ;
	if( pl_w->ground_y < pl_w->pos.y + DOT_Y( 32.0f ) ) return ;
	pl_w->routine_2 ++ ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	scr_work.quake_level = 3 ;
	scr_work.quake_flag = 1 ;
	pl_land_ef_set( pl_w ) ;
	if( pl_w->hit_flag ){
		pl_w->pl_stance = STANCE_CROUCH ;
		no = ( pl_w->atck_level << 1 ) + 21 ;
	} else {
		pl_w->pl_stance = STANCE_DOWN ;
		no = ( pl_w->atck_level << 1 ) + 24 ;
	}
	pl_char_set( pl_w, Char_Special, no ) ;
}

/*--------------------------------------------------------------*/
static void press_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			A 60 frame move that sets off an effect when the	*/
/*			animation says										*/
/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 60 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_dir_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 17 ) ;
	} else {
		pl_char_move( pl_w ) ;
		if( --pl_w->timer0 ){
			if( pl_w->cg_type ){
				pl_w->cg_type = 0 ;
				pls29_00_set( pl_w, 7 ) ;
			}
		} else {
			pl_stand_ret( pl_w ) ;
		}
	}
}

/*==============================================================*/
static void unused_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_char_set( pl_w, Char_Extra, 3 ) ;
}

/*==============================================================*/
static void unused_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/* not used */
static void ( *unused_jmp[])( PLAYER_WORK *pl_w ) = {
	unused_init, unused_move,
} ;

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_jump( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The partner's jump in								*/
/****************************************************************/
void pl29_t_kick( PLAYER_WORK *pl_w )
{
	static void ( *t_kick_jmp[])( PLAYER_WORK *pl_w ) = {
		t_kick_init, t_kick_jump, t_kick_land,
	} ;

	t_kick_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void t_kick_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->spd.x = 30.0f ;
	if( !pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 2.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->atck_no = pl29_at_t_kick ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 0 ) ;
}

/*--------------------------------------------------------------*/
static void t_kick_jump( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_land_ck( pl_w ) ){
		pl_land_ef_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Extra, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void t_kick_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_stop_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
/*			The snapback										*/
/*==============================================================*/
static void sp_snapback( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_dir_set( pl_w ) ;
		pl_sp_ef_set( pl_w ) ;
		pl_w->atck_no = pl29_at_snapback ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 32 ) ;
		pls29_00_set( pl_w, 1 ) ;
	} else if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
	} else if( pl_w->routine_2 == 1 && pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		ofs.x = 90.0f ;
		ofs.y = 180.0f ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
}

/****************************************************************/
PLAYER_WORK *pl29_catch_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = 0 ;

	if( ( pl_w->atck_lever & ( KEY_FORWARD | KEY_BACK ) ) && pl_w->atck_level ){
		if( pl_w->pl_stance == STANCE_AIR ){
			if( !pl_w->atck_pk ){
				if( ( em = pl_catch_ck( pl_w ) ) ) pl_w->catch_no = 2 ;
			}
		} else {
			if( ( em = pl_catch_ck( pl_w ) ) ){
				if( !pl_w->atck_pk ) pl_w->catch_no = 1 ;
				else pl_w->catch_no = 0 ;
			}
		}
	}
	return em ;
}

/****************************************************************/
/*			Start the throw that pl29_catch_ck chose			*/
/****************************************************************/
void pl29_catch_set( PLAYER_WORK *pl_w )
{
	scr_excep_set( pl_w, 5, 5 ) ;
	if( !( pl_w->atck_lever & KEY_FORWARD ) ){
		pl_w->disp_dir ^= 1 ;
		pl_w->pl_dir ^= 1 ;
	}
	pl_w->catch_w->pl_dir = pl_w->catch_w->disp_dir = pl_w->pl_dir ^ 1 ;
	pl_w->routine_2 = 0 ;
	LW->pw_used = -1 ;
	pl_char_set( pl_w, Char_Catch, pl_w->catch_no ) ;
}

/* where the opponent is held for each throw, x then y */
static Float catch_ofs[6] = {
	-DOT_X( 80.0f ), DOT_Y( 144.0f ),
	-DOT_X( 80.0f ), DOT_Y( 144.0f ),
	DOT_X( 22.0f ), DOT_Y( 176.0f ),
} ;

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void pl29_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->catch_w ;
	VEC3 ofs ;

	pl_w->catch_flag = 1 ;
	if( !pl_w->routine_2 ){
		ofs.x = catch_ofs[pl_w->catch_no * 2] ;
		ofs.y = catch_ofs[pl_w->catch_no * 2 + 1] ;
		nage_hit_set( pl_w, &ofs ) ;
		catch_gauge_add( pl_w ) ;
		pl_w->hit_stop = 10 ;
		LW->pw_set = 0 ;
		pl_w->routine_2 ++ ;
	}
	switch( pl_w->catch_no ){
	case 0:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		else if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			em->dm_hit_w = pl_w ;
			em->catch_type = 2 ;
			em->pl_stance = STANCE_AIR ;
			scr_excep_clr() ;
			em->atck_no = pl29_at_throw_k ;
			em->pl_dir = pl_w->pl_dir ^ 1 ;
			catch_se_req( pl_w->catch_w ) ;
		}
		break ;
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		else if( pl_w->cg_type ){
			if( pl_w->cg_type > 0 ){
				em->dm_hit_w = pl_w ;
				em->atck_no = pl29_at_throw_p_hit ;
				catch_damage( pl_w ) ;
			} else {
				em->dm_hit_w = pl_w ;
				em->catch_type = 1 ;
				em->pl_stance = STANCE_AIR ;
				scr_excep_clr() ;
				em->atck_no = pl29_at_throw_p ;
				em->pl_dir = pl_w->pl_dir ^ 1 ;
			}
			hit_se_req( em, 2 ) ;
			pl_w->cg_type = 0 ;
			ofs.x = -DOT_X( 103.0f ) ;
			if( pl_w->disp_dir ) ofs.x = -ofs.x ;
			ofs.x += pl_w->pos.x ;
			ofs.y = pl_w->pos.y + DOT_Y( 85.0f ) ;
			hitmark_set( &ofs, pl_w->pl_dir, 2, 0 ) ;
			pl_w->cg_type = 0 ;
		}
		break ;
	case 2:
		if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
		else if( pl_w->cg_type ){
			em->dm_hit_w = pl_w ;
			em->catch_type = 1 ;
			em->pl_stance = STANCE_AIR ;
			scr_excep_clr() ;
			em->atck_no = pl29_at_air_throw ;
			em->pl_dir = pl_w->pl_dir ^ 1 ;
		}
		break ;
	}
}

/****************************************************************/
/*			Holding the opponent								*/
/****************************************************************/
void pl29_hold( PLAYER_WORK *pl_w )
{
	catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void pl29_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl29_sp_headcrush ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl29_sp_headcrush ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl29_sp_headcrush ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl29_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl29_sp_headcrush ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl29_sp_headcrush ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl29_sp_headcrush ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl29_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl29_sp_quake ;
		pl_w->atck_level = 0 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl29_sp_punch ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl29_sp_press ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void pl29_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl29_sp_quake ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl29_sp_punch ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl29_sp_press ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl29.c" End of File	*/

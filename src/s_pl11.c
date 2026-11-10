/* Jill (character 11): command checks, normal attacks, appearance, win and lose poses, special
   moves and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl11_def.h"

void pl11_main( PLAYER_WORK *pl_w );
void pl11_com_ck( PLAYER_WORK *pl_w );
void pl11_atck_set( PLAYER_WORK *pl_w );
void pl11_atck( PLAYER_WORK *pl_w );
void pl11_dash( PLAYER_WORK *pl_w );
void pl11_noki( PLAYER_WORK *pl_w );
void pl11_appear( PLAYER_WORK *pl_w );
void pl11_win( PLAYER_WORK *pl_w );
void pl11_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl11_catch_ck( PLAYER_WORK *pl_w );
void pl11_catch_set( PLAYER_WORK *pl_w );
void pl11_catch( PLAYER_WORK *pl_w );
void pl11_hold( PLAYER_WORK *pl_w );
int pl11_delay_ck( PLAYER_WORK *pl_w );
void pl11_t_kick( PLAYER_WORK *pl_w );
void pl11_upper( PLAYER_WORK *pl_w );
void pl11_sp_unison( PLAYER_WORK *pl_w );
void pl11_sp_gattai( PLAYER_WORK *pl_w );
void pl11_sp_assist( PLAYER_WORK *pl_w );
void pl11_sp_zero( PLAYER_WORK *pl_w );
void pl11_tlocal_init( PLAYER_WORK *pl_w );

/* each attack's table: standing and crouching, jumping, super jumping */
static Sint8 lp_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 mp_tbl[4] = { -1,  2, -1,  2 } ;
static Sint8 hp_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 lk_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 mk_tbl[4] = { -1,  2, -1,  2 } ;
static Sint8 hk_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 j_lp_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 j_mp_tbl[4] = { -1,  2, -1,  2 } ;
static Sint8 j_hp_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 j_lk_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 j_mk_tbl[4] = { -1,  2, -1,  2 } ;
static Sint8 j_hk_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 sj_lp_tbl[4] = {  1,  2,  0,  2 } ;
static Sint8 sj_mp_tbl[4] = { -1,  2,  1,  2 } ;
static Sint8 sj_hp_tbl[4] = { -1, -1, -1,  2 } ;
static Sint8 sj_lk_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 sj_mk_tbl[4] = { -1,  2, -1,  2 } ;
static Sint8 sj_hk_tbl[4] = { -1, -1, -1, -1 } ;

/* commands: time or steps, kind, the special move, then the stick and buttons */
static Uint16 com_cerberus[] = { 3, COM_GROUND, pl11_sp_cerberus, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_LK } ;	/* Cerberus Escape */
static Uint16 com_crow[] = { 3, COM_GROUND, pl11_sp_crow, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_HK } ;	/* Crow Escape */
static Uint16 com_zombie[] = { 3, COM_GROUND, pl11_sp_zombie, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Zombie Escape */
static Uint16 com_stars[] = { 3, 0, pl11_sp_stars, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Charging Stars */
static Uint16 com_grenade[] = { 3, COM_GROUND, pl11_sp_grenade, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_PUNCH } ;	/* Grenade Launcher */
static Uint16 com_return[] = { 5, COM_GROUND, pl11_sp_return, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Return Fire */
static Uint16 com_hstars[] = { 3, COM_HYPER | COM_ALL_SW, pl11_sp_hstars, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Hyper Charging Stars (hyper) */
static Uint16 com_tyrant[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl11_sp_tyrant, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Tyrant Escape (hyper) */
static Uint16 com_tyrant_shot[] = { 3, COM_FREE, pl11_sp_tyrant, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Tyrant Escape's second command, which finishes it */
static Uint16 com_rocket[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl11_sp_rocket, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Rocket Launcher (hyper) */

static void ( *pl11_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl11_main,
	pl11_com_ck,
	pl11_atck_set,
	pl11_atck,
	pl11_dash,
	pl11_noki,
	pl11_appear,
	pl11_win,
	pl11_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl11_catch_ck,	/* returns a value */
	pl11_catch_set,
	pl11_catch,
	pl11_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl11_delay_ck,
	pl11_t_kick,
	pl11_upper,
	pl11_sp_unison,
	pl11_sp_gattai,
	pl11_sp_assist,
	pl11_sp_zero,
	0,
	0,
	0,
	pl11_tlocal_init,
} ;

static Uint8 cerberus_ck( PLAYER_WORK *pl_w );
static Uint8 crow_ck( PLAYER_WORK *pl_w );
static Uint8 zombie_ck( PLAYER_WORK *pl_w );
static Uint8 return_ck( PLAYER_WORK *pl_w );
static Uint8 stars_ck( PLAYER_WORK *pl_w );
static Uint8 grenade_ck( PLAYER_WORK *pl_w );
static Uint8 hstars_ck( PLAYER_WORK *pl_w );
static Uint8 tyrant_ck( PLAYER_WORK *pl_w );
static Uint8 rocket_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static int chouhatsu_ck( PLAYER_WORK *pl_w );
static int hstars_delay_ck( PLAYER_WORK *pl_w );
static int tyrant_delay_ck( PLAYER_WORK *pl_w );
static int rocket_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl11_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl11_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl11_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( return_ck( pl_w ) ) return ;
	if( grenade_ck( pl_w ) ) return ;
	if( hstars_ck( pl_w ) ) return ;
	if( tyrant_ck( pl_w ) ) return ;
	if( rocket_ck( pl_w ) ) return ;
	if( stars_ck( pl_w ) ) return ;
	if( zombie_ck( pl_w ) ) return ;
	if( cerberus_ck( pl_w ) ) return ;
	if( crow_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
/*			Not while the last one is still out					*/
/*==============================================================*/
static Uint8 cerberus_ck( PLAYER_WORK *pl_w )
{
	PL11_LOCAL_W *lw = ( PL11_LOCAL_W * )pl_w->tlocal ;

	if( !com_nml_ck( pl_w, com_cerberus, &pl_w->com_w0 ) ) return 0 ;
	if( lw->cerberus_shot_flag ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl11_sp_cerberus ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 crow_ck( PLAYER_WORK *pl_w )
{
	PL11_LOCAL_W *lw = ( PL11_LOCAL_W * )pl_w->tlocal ;

	if( !com_nml_ck( pl_w, com_crow, &pl_w->com_w1 ) ) return 0 ;
	if( lw->crow_shot_flag ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl11_sp_crow ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 zombie_ck( PLAYER_WORK *pl_w )
{
	PL11_LOCAL_W *lw = ( PL11_LOCAL_W * )pl_w->tlocal ;

	if( !com_nml_ck( pl_w, com_zombie, &pl_w->com_w2 ) ) return 0 ;
	if( lw->zombie_shot_flag ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl11_sp_zombie ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 return_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_return, &pl_w->com_w3 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w3 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl11_sp_return ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			Done at once from a light press, or charged			*/
/*			while the button is held (pl11_main)				*/
/*==============================================================*/
static Uint8 stars_ck( PLAYER_WORK *pl_w )
{
	PL11_LOCAL_W *lw = ( PL11_LOCAL_W * )pl_w->tlocal ;

	if( com_nml_ck( pl_w, com_stars, &pl_w->com_w4 ) ){
		if( pl_w->pl_stance == STANCE_AIR ){
			if( pl_w->jump_com_cnt ) return 0 ;
			else pl_w->jump_com_cnt ++ ;
		}
		com_sw_set( pl_w, &pl_w->com_w4 ) ;
		if( !pl_w->atck_level ){
			lw->charge_flag = 0 ;
			lw->charge_cnt = 0 ;
			pl_w->routine_1 = PL_NORM ;
			if( pl_w->pl_stance == STANCE_AIR ) pl_w->routine_2 = 1 ;
			else pl_w->routine_2 = 0 ;
			pl_w->routine_3 = 0 ;
			pl_w->pl_sp_type = pl11_sp_stars ;
			act_st_set( pl_w, N_SPECIAL ) ;
			return 1 ;
		}
		if( !lw->charge_flag ){
			lw->charge_flag = 1 ;
			lw->charge_cnt = 0 ;
			lw->charge_timer = 0 ;
			pls11_00_set( pl_w, 13, 0 ) ;
		}
	}
	return 0 ;
}

/*==============================================================*/
static Uint8 grenade_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_grenade, &pl_w->com_w5 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w5 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl11_sp_grenade ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 hstars_ck( PLAYER_WORK *pl_w )
{
	PL11_LOCAL_W *lw = ( PL11_LOCAL_W * )pl_w->tlocal ;

	if( !com_nml_ck( pl_w, com_hstars, &pl_w->com_w6 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR ){
		if( pl_w->jump_com_cnt && !pl_w->sjump_flag ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w6 ) ;
	lw->charge_flag = 0 ;
	pl_w->routine_1 = PL_NORM ;
	if( pl_w->pl_stance == STANCE_AIR ) pl_w->routine_2 = 1 ;
	else pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl11_sp_hstars ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 tyrant_ck( PLAYER_WORK *pl_w )
{
	PL11_LOCAL_W *lw = ( PL11_LOCAL_W * )pl_w->tlocal ;

	if( !com_nml_ck( pl_w, com_tyrant, &pl_w->com_w7 ) || !*pl_w->gauge_level || lw->tyrant_flag ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w7 ) ;
	lw->charge_flag = 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl11_sp_tyrant ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 rocket_ck( PLAYER_WORK *pl_w )
{
	PL11_LOCAL_W *lw = ( PL11_LOCAL_W * )pl_w->tlocal ;

	if( !com_nml_ck( pl_w, com_rocket, &pl_w->com_w8 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w8 ) ;
	lw->charge_flag = 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl11_sp_rocket ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static int snapback_ck( PLAYER_WORK *pl_w )
{
	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl11_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static int chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 10 ) ) return 0 ;
	pl_w->pl_sp_type = pl11_sp_chouhatsu ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl11_delay_ck( PLAYER_WORK *pl_w )
{
	if( hstars_delay_ck( pl_w ) || tyrant_delay_ck( pl_w ) || rocket_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static int hstars_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_hstars, &pl_w->com_w6 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR ) return 0 ;
	pl_w->delay_sp_type = 6 ;
	return 1 ;
}

/*==============================================================*/
static int tyrant_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_tyrant, &pl_w->com_w7 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 7 ;
	return 1 ;
}

/*==============================================================*/
static int rocket_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_rocket, &pl_w->com_w8 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 8 ;
	return 1 ;
}

/****************************************************************/
/*			A charge: it grows while the button is held, and	*/
/*			goes off when it is let go							*/
/****************************************************************/
void pl11_main( PLAYER_WORK *pl_w )
{
	PL11_LOCAL_W *lw = ( PL11_LOCAL_W * )pl_w->tlocal ;

	if( !lw->charge_flag ) return ;
	if( pl_w->pl_new_lvbt & KEY_HP ){
		if( lw->charge_timer ++ >= 100 ){
			lw->charge_timer = 0 ;
			if( lw->charge_cnt < 48 ) lw->charge_cnt ++ ;
		}
		return ;
	}
	lw->charge_flag = 0 ;
	if( sp_ok_ck( pl_w ) ){
		pl_w->routine_1 = PL_NORM ;
		if( pl_w->pl_stance == STANCE_AIR ) pl_w->routine_2 = 1 ;
		else pl_w->routine_2 = 0 ;
		pl_w->routine_3 = 0 ;
		pl_w->pl_sp_type = pl11_sp_stars ;
		pl_w->atck_level = 1 ;
		act_st_set( pl_w, N_SPECIAL ) ;
	}
}

static void nml_atck( PLAYER_WORK *pl_w );
static void stand_punch( PLAYER_WORK *pl_w );
static void crouch_punch( PLAYER_WORK *pl_w );
static void stand_kick( PLAYER_WORK *pl_w );
static void crouch_kick( PLAYER_WORK *pl_w );
static void jump_atck_ck( PLAYER_WORK *pl_w );
static void jump_atck( PLAYER_WORK *pl_w );
static void jump_punch( PLAYER_WORK *pl_w );
static void jump_kick( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Normal attacks										*/
/****************************************************************/
void pl11_atck_set( PLAYER_WORK *pl_w )
{
	static void ( *atck_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_atck, jump_atck_ck, jump_atck, nml_atck,
	} ;

	atck_set_jmp[pl_w->atck_type]( pl_w ) ;
}

/*==============================================================*/
static void nml_atck( PLAYER_WORK *pl_w )
{
	pl_brake_set( pl_w ) ;
	if( pl_w->atck_pk == 1 ){
		if( pl_w->pl_stance == STANCE_CROUCH ) crouch_kick( pl_w ) ;
		else stand_kick( pl_w ) ;
	} else {
		if( pl_w->pl_stance == STANCE_CROUCH ) crouch_punch( pl_w ) ;
		else stand_punch( pl_w ) ;
	}
}

/*==============================================================*/
static void stand_punch( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_STAND_LP ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_STAND_MP ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_STAND_HP ;
		pl_se_req( pl_w, SE_SWING_H ) ;
		pl_w->chain_tbl = hp_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Stand_P, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
static void crouch_punch( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_CROUCH_LP ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_CROUCH_MP ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_CROUCH_HP ;
		pl_se_req( pl_w, SE_SWING_H ) ;
		pl_w->chain_tbl = hp_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Crouch_P, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
static void stand_kick( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_STAND_LK ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		pl_w->chain_tbl = lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_STAND_MK ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_w->chain_tbl = mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_STAND_HK ;
		pl_se_req( pl_w, SE_SWING_H ) ;
		pl_w->chain_tbl = hk_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Stand_K, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
static void crouch_kick( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_CROUCH_LK ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		pl_w->chain_tbl = lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_CROUCH_MK ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_w->chain_tbl = mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_CROUCH_HK ;
		pl_se_req( pl_w, SE_SWING_H ) ;
		pl_w->chain_tbl = hk_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Crouch_K, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
/*			In the air, only while that button's jump			*/
/*			attacks are left									*/
/*==============================================================*/
static void jump_atck_ck( PLAYER_WORK *pl_w )
{
	if( ( pl_w->atck_pk == 0 && ( pl_w->jump_atk_cnt & JATK_P_MASK ) )
	 || ( pl_w->atck_pk != 0 && ( pl_w->jump_atk_cnt & JATK_K_MASK ) ) ) jump_atck( pl_w ) ;
}

/*==============================================================*/
static void jump_atck( PLAYER_WORK *pl_w )
{
	if( pl_w->atck_pk == 1 ) jump_kick( pl_w ) ;
	else jump_punch( pl_w ) ;
}

/*==============================================================*/
static void jump_punch( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_JUMP_LP ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_lp_tbl ;
		else pl_w->chain_tbl = sj_lp_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_JUMP_MP ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mp_tbl ;
		else pl_w->chain_tbl = sj_mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_JUMP_HP ;
		pl_se_req( pl_w, SE_SWING_H ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_hp_tbl ;
		else pl_w->chain_tbl = sj_hp_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Jump_P, pl_w->char_no.b.no ) ;
	if( pl_w->jump_atk_cnt & JATK_P_MASK ) pl_w->jump_atk_cnt -= 0x01 ;
}

/*==============================================================*/
static void jump_kick( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_JUMP_LK ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_lk_tbl ;
		else pl_w->chain_tbl = sj_lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_JUMP_MK ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mk_tbl ;
		else pl_w->chain_tbl = sj_mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_JUMP_HK ;
		pl_se_req( pl_w, SE_SWING_H ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_hk_tbl ;
		else pl_w->chain_tbl = sj_hk_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Jump_K, pl_w->char_no.b.no ) ;
	if( pl_w->jump_atk_cnt & JATK_K_MASK ) pl_w->jump_atk_cnt -= 0x10 ;
}

static void nml_move_dash( PLAYER_WORK *pl_w );
static void nml_move( PLAYER_WORK *pl_w );
static void stand_punch_move( PLAYER_WORK *pl_w );
static void crouch_punch_move( PLAYER_WORK *pl_w );
static void stand_kick_move( PLAYER_WORK *pl_w );
static void crouch_kick_move( PLAYER_WORK *pl_w );
static void slide_move( PLAYER_WORK *pl_w );
static void sjump_move( PLAYER_WORK *pl_w );
static void jump_move( PLAYER_WORK *pl_w );
static void jump_punch_move( PLAYER_WORK *pl_w );
static void jump_kick_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl11_atck( PLAYER_WORK *pl_w )
{
	static void ( *atck_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_move, jump_move, sjump_move, nml_move_dash,
	} ;

	atck_jmp[pl_w->atck_type]( pl_w ) ;
}

/*==============================================================*/
static void nml_move_dash( PLAYER_WORK *pl_w )
{
	pl_smoke_set( pl_w ) ;
	nml_move( pl_w ) ;
}

/*==============================================================*/
static void nml_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_brake_stop( pl_w ) ;
	if( pl_w->atck_pk == 1 ){
		if( pl_w->pl_stance == STANCE_CROUCH ) crouch_kick_move( pl_w ) ;
		else stand_kick_move( pl_w ) ;
	} else {
		if( pl_w->pl_stance == STANCE_CROUCH ) crouch_punch_move( pl_w ) ;
		else stand_punch_move( pl_w ) ;
	}
}

/*==============================================================*/
static void stand_punch_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
	case 1:
	case 2:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
/*			The heavy one steps forward on each hit				*/
/*==============================================================*/
static void crouch_punch_move( PLAYER_WORK *pl_w )
{
	Float step ;

	switch( pl_w->nml_no ){
	case 2:
		if( pl_char_move( pl_w ) < 0 ){
			pl_stand_ret( pl_w ) ;
			break ;
		}
		step = pl_w->cg_type ;
		if( step ){
			pl_w->cg_type = 0 ;
			step *= pl_w->pl_dir ? 1.6666666f : -1.6666666f ;
			pl_w->pos.x += step ;
		}
		break ;
	case 0:
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
static void stand_kick_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
	case 1:
	case 2:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
static void crouch_kick_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 2:
		slide_move( pl_w ) ;
		break ;
	case 0:
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
/*			The heavy crouching kick slides						*/
/*==============================================================*/
static void slide_move( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_char_move( pl_w ) ;
		if( pl_w->cg_type ){
			pl_w->routine_2 ++ ;
			pl_w->spd.x = pl_w->pl_dir ? DOT_X( 8.5f ) : -DOT_X( 8.5f ) ;
			pl_w->acc.x = pl_w->pl_dir ? -0.7291666f : 0.7291666f ;
		}
	} else {
		pl_smoke_set( pl_w ) ;
		if( pl_w->cg_type ) pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		if( pl_char_move( pl_w ) < 0 ){
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_stand_ret( pl_w ) ;
		}
	}
}

/*==============================================================*/
static void sjump_move( PLAYER_WORK *pl_w )
{
	pl_jump_hit_ck( pl_w ) ;
	pl_sjump_ctrl( pl_w ) ;
	jump_move( pl_w ) ;
}

/*==============================================================*/
static void jump_move( PLAYER_WORK *pl_w )
{
	pl_jump_move( pl_w ) ;
	pl_sjump_top_ck( pl_w ) ;
	if( pl_w->atck_pk == 1 ) jump_kick_move( pl_w ) ;
	else jump_punch_move( pl_w ) ;
	if( pl_land_ck( pl_w ) ) pl_land_set( pl_w ) ;
}

/*==============================================================*/
static void jump_punch_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/*==============================================================*/
static void jump_kick_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/****************************************************************/
/*			The launcher										*/
/****************************************************************/
void pl11_upper( PLAYER_WORK *pl_w )
{
	Float step ;

	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_char_set( pl_w, Char_Extra, 3 ) ;
		pl_w->atck_no = pl11_at_upper ;
		HIT_RESET( pl_w ) ;
		pl_se_req( pl_w, SE_SWING_H ) ;
		pl_gauge_add( pl_w, 5 ) ;
	}
	if( pl_w->atck_type == 3 ) pl_smoke_set( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_brake_stop( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	step = pl_w->cg_type ;
	if( step ){
		pl_w->cg_type = 0 ;
		step *= pl_w->pl_dir ? 1.6666666f : -1.6666666f ;
		pl_w->pos.x += step ;
	}
}

static void dash_init( PLAYER_WORK *pl_w );
static void dash_move( PLAYER_WORK *pl_w );
static void dash_stop( PLAYER_WORK *pl_w );
static void dash_end( PLAYER_WORK *pl_w );

/****************************************************************/
void pl11_dash( PLAYER_WORK *pl_w )
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
	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.x = pl_w->pl_dir ? DOT_X( 9.5f ) : -DOT_X( 9.5f ) ;
	pl_w->acc.x = pl_w->pl_dir ? -0.3125f : 0.3125f ;
	pl_w->timer0 = 14 ;
}

/*--------------------------------------------------------------*/
static void dash_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !--pl_w->timer0 ) pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void dash_stop( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 4.0f ) : -DOT_X( 4.0f ) ;
		pl_w->acc.x = pl_w->pl_dir ? -DOT_X( 0.125f ) : DOT_X( 0.125f ) ;
		pl_w->char_no.b.grp = 2 ;
		pl_w->char_no.b.no = 2 ;
		pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
	}
}

/*--------------------------------------------------------------*/
static void dash_end( PLAYER_WORK *pl_w )
{
	if( !pl_w->cg_type ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void noki_init( PLAYER_WORK *pl_w );
static void noki_move( PLAYER_WORK *pl_w );
static void noki_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash										*/
/****************************************************************/
void pl11_noki( PLAYER_WORK *pl_w )
{
	static void ( *noki_jmp[])( PLAYER_WORK *pl_w ) = {
		noki_init, noki_move, noki_end,
	} ;

	noki_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void noki_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_char_move( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 13.0f ) : DOT_X( 13.0f ) ;
	pl_w->acc.x = pl_w->pl_dir ? 0.104166664f : -0.104166664f ;
	pl_w->timer0 = 4 ;
}

/*--------------------------------------------------------------*/
static void noki_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( !--pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 8.0f ) : DOT_X( 8.0f ) ;
		pl_w->acc.x = pl_w->pl_dir ? DOT_X( 0.25f ) : -DOT_X( 0.25f ) ;
		pl_char_set( pl_w, Char_Dash, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void noki_end( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void appear_init( PLAYER_WORK *pl_w );
static void appear_move( PLAYER_WORK *pl_w );
static void win_time_ko( PLAYER_WORK *pl_w );
static void win_ko_init( PLAYER_WORK *pl_w );
static void win_ko_move( PLAYER_WORK *pl_w );
static void win_ko_shot( PLAYER_WORK *pl_w );
static void win_ko_move_e( PLAYER_WORK *pl_w );
static void lose_time_ko( PLAYER_WORK *pl_w );

/****************************************************************/
void pl11_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_init,
		appear_move,
	} ;

	appear_jmp[ pl_w->routine_2 ]( pl_w ) ;
}

/*==============================================================*/
/*			One of two entrances								*/
/*==============================================================*/
static void appear_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->disp_flag = 1 ;
	pl_w->timer1 = nlRand() & 1 ;
	pl_char_set( pl_w, Char_Appear, pl_w->timer1 ) ;
}

/*--------------------------------------------------------------*/
static void appear_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_1 ++ ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pls11_00_set( pl_w, 9, pl_w->timer1 == 0 ? 0 : 1 ) ;
	}
}

/****************************************************************/
void pl11_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_time_ko,
		lose_time_ko,
		win_time_ko,
		lose_time_ko,
		lose_time_ko,
	} ;

	if( pl_win_end_ck( pl_w ) ){
		pl_stand_ret( pl_w ) ;
	}else{
		win_jmp[ pl_w->type ]( pl_w ) ;
	}
}

/*==============================================================*/
static void win_time_ko( PLAYER_WORK *pl_w )
{
	static void ( *win_ko_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko_init,
		win_ko_move,
		win_ko_shot,
		win_ko_move_e,
	} ;

	win_ko_jmp[ pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			One of three poses, the last only while partners	*/
/*			are left											*/
/*==============================================================*/
static void win_ko_init( PLAYER_WORK *pl_w )
{
	static Uint8 win_tbl0[4] = { 0, 1, 2, 0 } ;
	static Uint8 win_tbl1[4] = { 0, 2, 0, 2 } ;

	if( vs_sys_work.vs_end_flag ) pl_w->timer0 = win_tbl0[Sys->game_timer0 & 3] ;
	else pl_w->timer0 = win_tbl1[Sys->game_timer0 & 3] ;
	if( pl_be_cnt( pl_w ) >= 2 ) pl_w->timer0 = 2 ;

	switch( pl_w -> timer0 ){
	case 0 :
		pl_w->routine_2 ++ ;
		pl_w->char_no.b.no = 0 ;
		break ;
	case 1 :
		pl_w->routine_2 += 2 ;
		pl_w->char_no.b.no = 1 ;
		ef_color_set( pl_w, 5, 0, 1 ) ;
		break ;
	case 2 :
		pl_w->routine_2 += 3 ;
		pl_w->char_no.b.no = 2 ;
		break ;
	}
	pl_char_set( pl_w, Char_Win, pl_w->char_no.b.no ) ;
}

/*--------------------------------------------------------------*/
static void win_ko_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_ko_shot( PLAYER_WORK *pl_w )
{
	pl_w->win_wait = 0xff ;
	pl_char_move( pl_w ) ;
	switch( pl_w->cg_type ){
	case 1:
		pl_w->cg_type = 0 ;
		pls11_00_set( pl_w, 10, 0 ) ;
		pl_se_req( pl_w, 73 ) ;
		break ;
	case 2:
		pl_w->cg_type = 0 ;
		pls11_00_set( pl_w, 11, 0 ) ;
		break ;
	}
}

/*--------------------------------------------------------------*/
static void win_ko_move_e( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void lose_time_ko( PLAYER_WORK *pl_w )
{
	if( !pl_w -> routine_2 ){
		pl_w -> routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, 3 ) ;
	}else{
		pl_char_move( pl_w ) ;
	}
}

static void sp_cerberus( PLAYER_WORK *pl_w );
static void sp_crow( PLAYER_WORK *pl_w );
static void sp_zombie( PLAYER_WORK *pl_w );
static void sp_return( PLAYER_WORK *pl_w );
static void sp_stars( PLAYER_WORK *pl_w );
static void sp_grenade( PLAYER_WORK *pl_w );
static void sp_hstars( PLAYER_WORK *pl_w );
static void sp_tyrant( PLAYER_WORK *pl_w );
static void sp_rocket( PLAYER_WORK *pl_w );
static void sp_heal( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );

/****************************************************************/
void pl11_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *pl_sp_tbl[])( PLAYER_WORK *pl_w ) = {
		sp_cerberus,					/* 00 Cerberus Escape */
		sp_crow,					/* 01 Crow Escape */
		sp_zombie,					/* 02 Zombie Escape */
		sp_return,					/* 03 Return Fire */
		sp_stars,					/* 04 Charging Stars */
		sp_grenade,					/* 05 Grenade Launcher */
		sp_hstars,					/* 06 Hyper Charging Stars (hyper) */
		sp_tyrant,					/* 07 Tyrant Escape (hyper) */
		sp_rocket,					/* 08 Rocket Launcher (hyper) */
		sp_heal,					/* 09 the heal for the partner on the point (assist) */
		sp_chouhatsu,					/* 0a the taunt */
		sp_snapback,					/* 0b the snapback */
	} ;

	pl_sp_tbl[ pl_w->pl_sp_type ]( pl_w ) ;
}

static void cerberus_init( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );
static void cerberus_wait( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );
static void cerberus_shot( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );
static void cerberus_hold( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );
static void cerberus_fall( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );
static void cerberus_end( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );

/*==============================================================*/
/*			A shot that stays out; a second press while it		*/
/*			does is a jump										*/
/*==============================================================*/
static void sp_cerberus( PLAYER_WORK *pl_w )
{
	static void ( *cerberus_jmp[])( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw ) = {
		cerberus_init,
		cerberus_wait,
		cerberus_shot,
		cerberus_hold,
		cerberus_fall,
		cerberus_end,
	} ;

	cerberus_jmp[ pl_w->routine_2 ]( pl_w, ( PL11_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void cerberus_init( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl11_at_cerberus ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 0 ) ;
}

/*--------------------------------------------------------------*/
static void cerberus_wait( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		lw->cerberus_near_flag = 0 ;
		if( !shl11_00_set( pl_w, 0 ) ){
			pl_stand_ret( pl_w ) ;
			return ;
		}
		pl_w->unk_27b = 0 ;
		pl_w->cancel_timer = 16 ;
	}
}

/*--------------------------------------------------------------*/
static void cerberus_shot( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( !lw->cerberus_shot_flag ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( lw->cerberus_near_flag ){
		pl_w->routine_2 += 2 ;
		pl_w->spd.y = 10.0f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_char_set( pl_w, Char_Special, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void cerberus_hold( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	if( !pl_w->cg_type ) pl_char_move( pl_w ) ;
	if( !--pl_w->timer0 ) pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void cerberus_fall( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
	if( !pl_w->cg_type && pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void cerberus_end( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void crow_init( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );
static void crow_wait( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );
static void crow_shot( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );
static void crow_hold( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );
static void crow_end( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );

/*==============================================================*/
/*			A second shot that stays out						*/
/*==============================================================*/
static void sp_crow( PLAYER_WORK *pl_w )
{
	static void ( *crow_jmp[])( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw ) = {
		crow_init,
		crow_wait,
		crow_shot,
		crow_hold,
		crow_end,
	} ;

	crow_jmp[ pl_w->routine_2 ]( pl_w, ( PL11_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void crow_init( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	if( pl_w->team_type == 3 ){
		pl_w->atck_no = pl11_at_crow_vc ;
		HIT_RESET( pl_w ) ;
	}else{
		pl_w->atck_no = pl11_at_crow ;
		HIT_RESET( pl_w ) ;
	}
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 4 ) ;
}

/*--------------------------------------------------------------*/
static void crow_wait( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		lw->crow_near_flag = 0 ;
		if( !shl11_00_set( pl_w, 1 ) ){
			pl_stand_ret( pl_w ) ;
			return ;
		}
		pl_w->unk_27b = 0 ;
		pl_w->cancel_timer = 16 ;
	}
}

/*--------------------------------------------------------------*/
static void crow_shot( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( !lw->crow_shot_flag ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( lw->crow_near_flag ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 32 ;
		pl_char_set( pl_w, Char_Special, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void crow_hold( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	if( !pl_w->cg_type ) pl_char_move( pl_w ) ;
	if( !--pl_w->timer0 ) pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void crow_end( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void zombie_init( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );
static void zombie_move( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );

/*==============================================================*/
/*			A shot thrown once									*/
/*==============================================================*/
static void sp_zombie( PLAYER_WORK *pl_w )
{
	static void ( *zombie_jmp[])( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw ) = {
		zombie_init,
		zombie_move,
	} ;

	zombie_jmp[ pl_w->routine_2 ]( pl_w, ( PL11_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void zombie_init( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	if( pl_w->atck_level ){
		pl_w->atck_no = pl11_at_zombie_h ;
		HIT_RESET( pl_w ) ;
	}else{
		pl_w->atck_no = pl11_at_zombie ;
		HIT_RESET( pl_w ) ;
	}
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_sp_ef_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_char_set( pl_w, Char_Special, 5 ) ;
}

/*--------------------------------------------------------------*/
static void zombie_move( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) >= 0 ){
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			if( pl_w->atck_level ) shl11_01_set( pl_w, 2 ) ;
			else shl11_01_set( pl_w, 0 ) ;
			pl_w->unk_27b = 0 ;
			pl_w->cancel_timer = 16 ;
		}
		return ;
	}
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_stand_ret( pl_w ) ;
}

static void return_init( PLAYER_WORK *pl_w );
static void return_wait( PLAYER_WORK *pl_w );
static void return_flash( PLAYER_WORK *pl_w );
static void return_atck( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A counter: flashes while it waits, and a hit		*/
/*			taken then throws the opponent						*/
/*==============================================================*/
static void sp_return( PLAYER_WORK *pl_w )
{
	static void ( *return_jmp[])( PLAYER_WORK *pl_w ) = {
		return_init,
		return_wait,
		return_flash,
		return_atck,
	} ;

	return_jmp[ pl_w->routine_2 ]( pl_w ) ;
}

/*==============================================================*/
static void return_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 26 ) ;
}

/*--------------------------------------------------------------*/
static void return_wait( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->dm_attr = ATR_COUNTER ;
	}
}

/*--------------------------------------------------------------*/
static void return_flash( PLAYER_WORK *pl_w )
{
	pl_color_set( pl_w, ( Sys->game_timer0 & 2 ) ? 8 : 0 ) ;
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->dm_attr = 0 ;
		pl_w->timer0 = 1 ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
	}else if( !( pl_w->dm_attr & ATR_COUNTER ) ){
		pl_w->routine_2 ++ ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_w->atck_no = pl11_at_counter ;
		HIT_RESET( pl_w ) ;
		pl_w->timer0 = 0 ;
		pl_char_set( pl_w, Char_Special, 6 ) ;
	}
}

/*--------------------------------------------------------------*/
static void return_atck( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( !pl_w->timer0 && pl_w->hit_flag ){
		pl_w->timer0 = 1 ;
		if( pl_sp_catch_ck( pl_w ) ){
			pl_w->routine_2 = 0 ;
			pl_w->catch_no = 0xc2 ;
			pl_catch_set( pl_w, pl_w->em_w ) ;
		}
	}
}

static void stars_ground( PLAYER_WORK *pl_w );
static void stars_air( PLAYER_WORK *pl_w );
static void stars_init( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );
static void stars_start( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );
static void stars_rush( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );
static void stars_end( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );
static void stars_hit_set( PLAYER_WORK *pl_w, Uint8 no );
static void stars_j_init( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );
static void stars_j_start( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );
static void stars_j_rush( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );
static void stars_j_fall( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );
static void stars_j_end( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );

/*==============================================================*/
/*			The charged rush, one more hit for each step of		*/
/*			the charge; on the ground or in the air				*/
/*==============================================================*/
static void sp_stars( PLAYER_WORK *pl_w )
{
	static void ( *stars_jmp[])( PLAYER_WORK *pl_w ) = {
		stars_ground,
		stars_air,
	} ;

	stars_jmp[ pl_w->routine_2 ]( pl_w ) ;
}

/*==============================================================*/
static void stars_ground( PLAYER_WORK *pl_w )
{
	static void ( *stars_ground_jmp[])( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw ) = {
		stars_init,
		stars_start,
		stars_rush,
		stars_end,
	} ;

	stars_ground_jmp[ pl_w->routine_3 ]( pl_w, ( PL11_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void stars_init( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	pl_w->routine_3 ++ ;
	stars_hit_set( pl_w, 0 ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 7 ) ;
}

/*--------------------------------------------------------------*/
static void stars_start( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		if( pl_w->atck_level ) pl_w->timer0 = 30 ;
		else pl_w->timer0 = 15 ;
		pl_w->timer1 = lw->charge_cnt + 1 ;
		pl_w->dir = 1 ;
		lw->stars_hit_cnt = 0 ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 8.0f ) : -DOT_X( 8.0f ) ;
		pls11_00_set( pl_w, 3, lw->charge_cnt ? 1 : 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void stars_rush( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !--pl_w->timer0 ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = pl_w->pl_dir ? 5.0f : -5.0f ;
		pl_w->acc.x = pl_w->pl_dir ? -0.104166664f : 0.104166664f ;
		stars_hit_set( pl_w, 1 ) ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 9 ) ;
		return ;
	}
	if( pl_w->hit_flag && !--pl_w->dir ){
		pl_w->dir = 1 ;
		pl_w->timer0 ++ ;
		lw->stars_hit_cnt ++ ;
		if( !--pl_w->timer1 ) pl_w->timer0 = 1 ;
		else if( lw->stars_hit_cnt > 1 ) stars_hit_set( pl_w, 2 ) ;
		else stars_hit_set( pl_w, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void stars_end( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
/*			The rush's hits, stronger for the stronger			*/
/*			version												*/
/*==============================================================*/
static void stars_hit_set( PLAYER_WORK *pl_w, Uint8 no )
{
	static Uint8 stars_hit_tbl[6] = { 0x38, 0x39, 0x4c, 0x50, 0x51, 0x52 } ;

	no += pl_w->team_type == 3 ? 3 : 0 ;
	pl_w->atck_no = stars_hit_tbl[no] ;
	HIT_RESET( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void stars_air( PLAYER_WORK *pl_w )
{
	static void ( *stars_air_jmp[])( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw ) = {
		stars_j_init,
		stars_j_start,
		stars_j_rush,
		stars_j_fall,
		stars_j_end,
	} ;

	stars_air_jmp[ pl_w->routine_3 ]( pl_w, ( PL11_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void stars_j_init( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	pl_w->routine_3 ++ ;
	pl_w->atck_no = pl11_at_stars_air ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 11 ) ;
}

/*--------------------------------------------------------------*/
static void stars_j_start( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		if( pl_w->atck_level ) pl_w->timer0 = 30 ;
		else pl_w->timer0 = 15 ;
		pl_w->timer1 = lw->charge_cnt + 1 ;
		pl_w->dir = 1 ;
		lw->stars_hit_cnt = 0 ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 8.0f ) : -DOT_X( 8.0f ) ;
		pls11_00_set( pl_w, 3, ( lw->charge_cnt ? 1 : 0 ) + 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void stars_j_rush( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !--pl_w->timer0 ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = pl_w->pl_dir ? 5.0f : -5.0f ;
		pl_w->acc.x = pl_w->pl_dir ? -0.052083332f : 0.052083332f ;
		pl_w->spd.y = -DOT_Y( 3.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_w->atck_no = pl11_at_stars_air_end ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 13 ) ;
		return ;
	}
	if( pl_w->hit_flag && !--pl_w->dir ){
		pl_w->dir = 1 ;
		pl_w->timer0 ++ ;
		lw->stars_hit_cnt ++ ;
		if( !--pl_w->timer1 ){
			pl_w->timer0 = 1 ;
		}else{
			if( lw->stars_hit_cnt > 1 ){
				pl_w->atck_no = pl11_at_stars_air_2 ;
				HIT_RESET( pl_w ) ;
			}else{
				pl_w->atck_no = pl11_at_stars_air ;
				HIT_RESET( pl_w ) ;
			}
		}
	}
}

/*--------------------------------------------------------------*/
static void stars_j_fall( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_3 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Jump, 3 ) ;
		pl_land_ef_set( pl_w ) ;
		return ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void stars_j_end( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void grenade_init( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );
static void grenade_move( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );
static void grenade_wait( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );
static void grenade_end( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );

/*==============================================================*/
/*			A move that goes on to a second part when its		*/
/*			effect starts										*/
/*==============================================================*/
static void sp_grenade( PLAYER_WORK *pl_w )
{
	static void ( *grenade_jmp[])( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw ) = {
		grenade_init,
		grenade_move,
		grenade_wait,
		grenade_end,
	} ;

	grenade_jmp[ pl_w->routine_2 ]( pl_w, ( PL11_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void grenade_init( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	if( pl_w->team_type == 3 ){
		pl_w->atck_no = pl11_at_grenade_vc ;
		HIT_RESET( pl_w ) ;
	}else{
		pl_w->atck_no = pl11_at_grenade ;
		HIT_RESET( pl_w ) ;
	}
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	if( shl11_02_set( pl_w, 0, 0 ) ) lw->grenade_end_flag = 0 ;
	else lw->grenade_end_flag = 1 ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 15 ) ;
}

/*--------------------------------------------------------------*/
static void grenade_move( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		grenade_wait( pl_w, lw ) ;
	}
}

/*--------------------------------------------------------------*/
static void grenade_wait( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( lw->grenade_end_flag ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 17 ) ;
	}
}

/*--------------------------------------------------------------*/
static void grenade_end( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void hstars_ground( PLAYER_WORK *pl_w );
static void hstars_air( PLAYER_WORK *pl_w );
static void hstars_init( PLAYER_WORK *pl_w );
static void hstars_wait( PLAYER_WORK *pl_w );
static void hstars_start( PLAYER_WORK *pl_w );
static void hstars_rush( PLAYER_WORK *pl_w );
static void hstars_end( PLAYER_WORK *pl_w );
static void hstars_j_init( PLAYER_WORK *pl_w );
static void hstars_j_wait( PLAYER_WORK *pl_w );
static void hstars_j_start( PLAYER_WORK *pl_w );
static void hstars_j_rush( PLAYER_WORK *pl_w );
static void hstars_j_fall( PLAYER_WORK *pl_w );
static void hstars_j_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo: a rush of nine hits, on the			*/
/*			ground or in the air								*/
/*==============================================================*/
static void sp_hstars( PLAYER_WORK *pl_w )
{
	static void ( *hstars_jmp[])( PLAYER_WORK *pl_w ) = {
		hstars_ground,
		hstars_air,
	} ;

	hstars_jmp[ pl_w->routine_2 ]( pl_w ) ;
}

/*==============================================================*/
static void hstars_ground( PLAYER_WORK *pl_w )
{
	static void ( *hstars_ground_jmp[])( PLAYER_WORK *pl_w ) = {
		hstars_init,
		hstars_wait,
		hstars_start,
		hstars_rush,
		hstars_end,
	} ;

	hstars_ground_jmp[ pl_w->routine_3 ]( pl_w ) ;
}

/*==============================================================*/
static void hstars_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_3 ++ ;
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 0 ) ;
}

/*--------------------------------------------------------------*/
static void hstars_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_voice_req( pl_w, 24 ) ;
		ofs.x = DOT_X( 34.0f ) ;
		ofs.y = DOT_Y( 66.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void hstars_start( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		pl_w->timer0 = 30 ;
		pl_w->timer1 = 9 ;
		pl_w->atck_no = pl11_at_hstars ;
		HIT_RESET( pl_w ) ;
		pl_w->dir = 1 ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 7.0f ) : -DOT_X( 7.0f ) ;
		pls11_00_set( pl_w, 5, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void hstars_rush( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !--pl_w->timer0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_3 ++ ;
		pl_w->spd.x = pl_w->pl_dir ? 5.0f : -5.0f ;
		pl_w->acc.x = pl_w->pl_dir ? -0.104166664f : 0.104166664f ;
		pl_w->atck_no = pl11_at_hstars_end ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 1 ) ;
		return ;
	}
	if( pl_w->hit_flag && !--pl_w->dir ){
		pl_w->dir = 1 ;
		if( !--pl_w->timer1 ){
			pl_w->timer0 = 1 ;
		}else{
			pl_w->atck_no = pl11_at_hstars ;
			HIT_RESET( pl_w ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void hstars_end( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void hstars_air( PLAYER_WORK *pl_w )
{
	static void ( *hstars_air_jmp[])( PLAYER_WORK *pl_w ) = {
		hstars_j_init,
		hstars_j_wait,
		hstars_j_start,
		hstars_j_rush,
		hstars_j_fall,
		hstars_j_end,
	} ;

	hstars_air_jmp[ pl_w->routine_3 ]( pl_w ) ;
}

/*==============================================================*/
static void hstars_j_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_3 ++ ;
	pl_dir_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_char_set( pl_w, Char_Super, 2 ) ;
}

/*--------------------------------------------------------------*/
static void hstars_j_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_voice_req( pl_w, 24 ) ;
		ofs.x = DOT_X( 26.0f ) ;
		ofs.y = DOT_Y( 76.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void hstars_j_start( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		pl_w->timer0 = 30 ;
		pl_w->timer1 = 9 ;
		pl_w->atck_no = pl11_at_hstars_air ;
		HIT_RESET( pl_w ) ;
		pl_w->dir = 1 ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 7.0f ) : -DOT_X( 7.0f ) ;
		pls11_00_set( pl_w, 5, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void hstars_j_rush( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !--pl_w->timer0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_3 ++ ;
		pl_w->spd.x = pl_w->pl_dir ? 5.0f : -5.0f ;
		pl_w->acc.x = pl_w->pl_dir ? -0.052083332f : 0.052083332f ;
		pl_w->spd.y = -DOT_Y( 3.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_w->atck_no = pl11_at_hstars_air_end ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 3 ) ;
		return ;
	}
	if( pl_w->hit_flag && !--pl_w->dir ){
		pl_w->dir = 1 ;
		if( !--pl_w->timer1 ){
			pl_w->timer0 = 1 ;
		}else{
			pl_w->atck_no = pl11_at_hstars_air ;
			HIT_RESET( pl_w ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void hstars_j_fall( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_3 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Jump, 3 ) ;
		pl_land_ef_set( pl_w ) ;
		return ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void hstars_j_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void rocket_init( PLAYER_WORK *pl_w );
static void rocket_wait( PLAYER_WORK *pl_w );
static void rocket_shot( PLAYER_WORK *pl_w );
static void rocket_shot_end( PLAYER_WORK *pl_w );
static void rocket_last_shot( PLAYER_WORK *pl_w );
static void rocket_jump( PLAYER_WORK *pl_w );
static void rocket_fall( PLAYER_WORK *pl_w );
static void rocket_land( PLAYER_WORK *pl_w );
static void rocket_hop( PLAYER_WORK *pl_w );
static void rocket_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo: seven shots, a last one, then a		*/
/*			jump back											*/
/*==============================================================*/
static void sp_rocket( PLAYER_WORK *pl_w )
{
	static void ( *rocket_jmp[])( PLAYER_WORK *pl_w ) = {
		rocket_init,
		rocket_wait,
		rocket_shot,
		rocket_shot_end,
		rocket_last_shot,
		rocket_jump,
		rocket_fall,
		rocket_land,
		rocket_hop,
		rocket_end,
	} ;

	rocket_jmp[ pl_w->routine_2 ]( pl_w ) ;
}

/*==============================================================*/
static void rocket_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl11_at_rocket ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 4 ) ;
}

/*--------------------------------------------------------------*/
static void rocket_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = -DOT_X( 62.0f ) ;
		ofs.y = DOT_Y( 78.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
		pl_w->timer0 = 7 ;
		pl_w->timer1 = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void rocket_shot( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shl11_02_set( pl_w, 1, pl_w->timer1 & 3 ) ;
		pls11_00_set( pl_w, 6, pl_w->timer1 & 3 ) ;
		pls11_00_set( pl_w, 15, pl_w->timer1 & 3 ) ;
		pl_w->timer1 ++ ;
		if( !--pl_w->timer0 ) pl_w->routine_2 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void rocket_shot_end( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Super, 5 ) ;
	}
}

/*--------------------------------------------------------------*/
static void rocket_last_shot( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		shl11_02_set( pl_w, 1, ( pl_w->timer1 & 3 ) + 0x80 ) ;
		pls11_00_set( pl_w, 6, pl_w->timer1 & 3 ) ;
		pl_w->timer1 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void rocket_jump( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->spd.x = pl_w->pl_dir ? -10.0f : 10.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 4.5f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pls11_00_set( pl_w, 14, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void rocket_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pls11_00_set( pl_w, 19, 0 ) ;
	}
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_DOWN ;
		pl_char_set( pl_w, Char_Super, 6 ) ;
	}
}

/*--------------------------------------------------------------*/
static void rocket_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 2.0f ) : DOT_X( 2.0f ) ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 3.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_char_set( pl_w, Char_Super, 7 ) ;
	}
}

/*--------------------------------------------------------------*/
static void rocket_hop( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_char_set( pl_w, Char_Super, 8 ) ;
	}
}

/*--------------------------------------------------------------*/
static void rocket_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void tyrant_init( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );
static void tyrant_wait( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );
static void tyrant_catch_ck( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );
static void tyrant_stop( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );
static void tyrant_step( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );
static void tyrant_slide( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );
static void tyrant_hold( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );
static void tyrant_hit_ck( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );
static void tyrant_com_ck( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );
static void tyrant_flash( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );
static void tyrant_shot( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );
static void tyrant_jump( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );
static void tyrant_fall( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );
static void tyrant_land( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );
static void tyrant_hop( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );
static void tyrant_end( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw );

/*==============================================================*/
/*			A hyper combo that needs a partner: it catches		*/
/*			the opponent, and a second command finishes it		*/
/*==============================================================*/
static void sp_tyrant( PLAYER_WORK *pl_w )
{
	static void ( *tyrant_jmp[])( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw ) = {
		tyrant_init,
		tyrant_wait,
		tyrant_catch_ck,
		tyrant_stop,
		tyrant_step,
		tyrant_slide,
		tyrant_hold,
		tyrant_hit_ck,
		tyrant_com_ck,
		tyrant_flash,
		tyrant_shot,
		tyrant_jump,
		tyrant_fall,
		tyrant_land,
		tyrant_hop,
		tyrant_end,
	} ;

	tyrant_jmp[ pl_w->routine_2 ]( pl_w, ( PL11_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void tyrant_init( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl11_at_tyrant ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	lw->tyrant_reach_st = 0 ;
	lw->tyrant_hold_st = 0 ;
	lw->tyrant_throw_flag = 0 ;
	pl_sp_ef_set( pl_w ) ;
	if( work_rest_cnt < 2 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
		return ;
	}
	shl11_03_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_char_set( pl_w, Char_Super, 9 ) ;
}

/*--------------------------------------------------------------*/
static void tyrant_wait( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = -65.0f ;
		ofs.y = 0.0f ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void tyrant_catch_ck( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( !pl_w->cg_type ) pl_char_move( pl_w ) ;
	if( lw->tyrant_reach_st < 0 ){
		pl_w->routine_2 = 15 ;
		return ;
	}
	if( lw->tyrant_reach_st ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 32 ;
		scr_excep_set( pl_w, 1, 1 ) ;
		pl_char_set( pl_w, Char_Super, 10 ) ;
	}
}

/*--------------------------------------------------------------*/
static void tyrant_stop( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( !--pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 2.5f ) : DOT_X( 2.5f ) ;
	}
}

/*--------------------------------------------------------------*/
static void tyrant_step( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void tyrant_slide( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( lw->tyrant_hold_st ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Super, 11 ) ;
	}
}

/*--------------------------------------------------------------*/
static void tyrant_hold( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ) pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void tyrant_hit_ck( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( lw->tyrant_hold_st < 0 ){
		pl_w->routine_2 = 15 ;
		pl_char_set( pl_w, Char_Super, 13 ) ;
	}
	if( lw->tyrant_hold_st >= 2 ){
		pl_w->routine_2 ++ ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void tyrant_com_ck( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	if( lw->tyrant_hold_st < 0 ){
		pl_w->routine_2 = 15 ;
		pl_char_set( pl_w, Char_Super, 13 ) ;
		return ;
	}
	if( ( ( pl_w->cpu_flag && pl_w->atck_pk ) || com_nml_ck( pl_w, com_tyrant_shot, &pl_w->com_w9 ) ) && *pl_w->gauge_level ){
		pl_w->routine_2 ++ ;
		pl_w->atck_no = pl11_at_tyrant_shot ;
		HIT_RESET( pl_w ) ;
		lw->tyrant_throw_flag = 1 ;
		pl_char_set( pl_w, Char_Super, 12 ) ;
		pl_sp_ef_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void tyrant_flash( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		ofs.x = -DOT_X( 10.0f ) ;
		ofs.y = DOT_Y( 55.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_flash( pl_w, &ofs, 1 ) ;
		pl_w->hyper_bg = vs_sys_w->hyper_cnt[pl_w->pl_no] ;
		vs_sys_w->hyper_bg_req = vs_sys_w->hyper_cnt[pl_w->pl_no] | ( pl_w->pl_no << 4 ) ;
		pl_w->hyper_bg_tm = 16 ;
	}
}

/*--------------------------------------------------------------*/
static void tyrant_shot( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		shl11_02_set( pl_w, 2, 0 ) ;
		pls11_00_set( pl_w, 6, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void tyrant_jump( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->spd.x = pl_w->pl_dir ? -10.0f : 10.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 4.5f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
	}
}

/*--------------------------------------------------------------*/
static void tyrant_fall( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	rocket_fall( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void tyrant_land( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	rocket_land( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void tyrant_hop( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	rocket_hop( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void tyrant_end( PLAYER_WORK *pl_w, PL11_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->pl_stance = STANCE_STAND ;
		SCR_EXCEP_SET ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void heal_init( PLAYER_WORK *pl_w );
static void heal_wait( PLAYER_WORK *pl_w );
static void heal_hit( PLAYER_WORK *pl_w );
static void heal_end( PLAYER_WORK *pl_w );
static void heal_miss( PLAYER_WORK *pl_w );
static Uint8 heal_hit_ck( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The heal: reaches for the partner on the point		*/
/*			and gives vital back (one of two, at random)		*/
/*==============================================================*/
static void sp_heal( PLAYER_WORK *pl_w )
{
	static void ( *heal_jmp[])( PLAYER_WORK *pl_w ) = {
		heal_init,
		heal_wait,
		heal_hit,
		heal_end,
		heal_miss,
	} ;

	heal_jmp[ pl_w->routine_2 ]( pl_w ) ;
}

/*==============================================================*/
static void heal_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	ef_color_set( pl_w, 4, 1, 1 ) ;
	pl_w->timer1 = Sys->game_timer0 & 1 ;
	pl_char_set( pl_w, Char_Special, pl_w->timer1 + 19 ) ;
}

/*--------------------------------------------------------------*/
static void heal_wait( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 120 ;
	}
}

/*--------------------------------------------------------------*/
static void heal_hit( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *mw = vs_sys_work.team_w[pl_w->pl_no][0] ;

	if( heal_hit_ck( pl_w ) ){
		if( mw->vital_new || !mw->team_no ){
			pl_w->routine_2 ++ ;
			pls11_00_set( pl_w, 8, pl_w->timer1 ) ;
			vital_add_dmg( mw, pl_w->timer1 ? 20 : 10 ) ;
			puff_ef_set( mw, 4 ) ;
			pl_char_set( pl_w, Char_Special, pl_w->timer1 + 21 ) ;
			return ;
		}
		pl_w->timer0 = 1 ;
	}
	if( --pl_w->timer0 ) return ;
	pl_w->routine_2 += 2 ;
	pl_char_set( pl_w, Char_Special, pl_w->timer1 + 23 ) ;
}

/*--------------------------------------------------------------*/
static void heal_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void heal_miss( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
/*			Does the attack reach the partner's body			*/
/*==============================================================*/
static Uint8 heal_hit_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *mw = vs_sys_work.team_w[pl_w->pl_no][0] ;
	FRAME_BOX *mb = mw->frame_box ;
	FRAME_BOX *pb = pl_w->frame_box ;

	if( box_ck( pl_w, mw, pb->atck_box2 & 0xff, mb->body_box0 )
	 || box_ck( pl_w, mw, pb->atck_box2 & 0xff, mb->body_box1 )
	 || box_ck( pl_w, mw, pb->atck_box2 & 0xff, mb->body_box2 )
	 || ( mb->body_box3 && box_ck( pl_w, mw, pb->atck_box2 & 0xff, mb->body_box3 ) ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
/*			A taunt												*/
/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Extra, 2 ) ;
	}else if( pl_char_move( pl_w ) < 0 ){
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void snapback_init( PLAYER_WORK *pl_w );
static void snapback_move( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The snapback: a flash on the mark					*/
/*==============================================================*/
static void sp_snapback( PLAYER_WORK *pl_w )
{
	static void ( *snapback_jmp[])( PLAYER_WORK *pl_w ) = {
		snapback_init,
		snapback_move,
	} ;

	snapback_jmp[ pl_w->routine_2 ]( pl_w ) ;
}

/*==============================================================*/
static void snapback_init( PLAYER_WORK *pl_w )
{
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
	pl_w->atck_no = pl11_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 25 ) ;
}

/*--------------------------------------------------------------*/
static void snapback_move( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type & 1 ){
		pl_w->cg_type = 0 ;
		ofs.x = 0.0f ;
		ofs.y = DOT_Y( 25.0f ) ;
		ofs.z = 0.0f ;
		pl_snap_flash( pl_w, &ofs ) ;
	}else if( pl_w->cg_type & 2 ){
		pl_w->cg_type = 0 ;
		pl_se_req( pl_w, SE_SWING_H ) ;
	}
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_move( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A kick that leaps off the wall						*/
/****************************************************************/
void pl11_t_kick( PLAYER_WORK *pl_w )
{
	static void ( *t_kick_jmp[])( PLAYER_WORK *pl_w ) = {
		t_kick_init,
		t_kick_move,
		t_kick_land,
	} ;

	t_kick_jmp[ pl_w->routine_2 ]( pl_w ) ;
}

/*==============================================================*/
static void t_kick_init( PLAYER_WORK *pl_w )
{
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->spd.x = 30.0f ;
	if( !pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 2.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->atck_no = pl11_at_t_kick ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 0 ) ;
}

/*--------------------------------------------------------------*/
static void t_kick_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_2 ++ ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void t_kick_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stop_stand_ret( pl_w ) ;
	}else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
	}
}

static PLAYER_WORK *nml_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *crouch_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *jump_catch_ck( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Can a throw start?									*/
/****************************************************************/
PLAYER_WORK *pl11_catch_ck( PLAYER_WORK *pl_w )
{
	static PLAYER_WORK *( *catch_ck_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_catch_ck,
		crouch_catch_ck,
		jump_catch_ck,
		nml_catch_ck,
	} ;

	return catch_ck_jmp[ pl_w->pl_stance ]( pl_w ) ;
}

/*==============================================================*/
static PLAYER_WORK *nml_catch_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( !( pl_w->dir = ( pl_w->atck_lever & ( KEY_FORWARD | KEY_BACK ) ) >> 10 ) ) return 0 ;
	if( pl_w->atck_pk == 0 && pl_w->atck_level == 1 ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 0 ;
			return em ;
		}
		return 0 ;
	}
	return 0 ;
}

/*==============================================================*/
static PLAYER_WORK *crouch_catch_ck( PLAYER_WORK *pl_w )
{
	return 0 ;
}

/*==============================================================*/
static PLAYER_WORK *jump_catch_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( !( pl_w->dir = ( pl_w->atck_lever & ( KEY_FORWARD | KEY_BACK ) ) >> 10 ) ) return 0 ;
	if( pl_w->atck_pk == 0 && pl_w->atck_level == 1 && pl_w->pos.y > DOT_Y( 64.0f ) ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 1 ;
			return em ;
		}
		return 0 ;
	}
	return 0 ;
}

static void nml_p_set( PLAYER_WORK *pl_w );
static void jump_p_set( PLAYER_WORK *pl_w );
static void sp_catch_set( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Start the throw that pl11_catch_ck chose, or the	*/
/*			one a special move caught with						*/
/****************************************************************/
void pl11_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_set,
		jump_p_set,
		sp_catch_set,
		sp_catch_set,
		sp_catch_set,
	} ;

	catch_set_jmp[ pl_w->catch_no & CATCH_NO_MASK ]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_set( PLAYER_WORK *pl_w )
{
	VEC3 pos ;

	scr_excep_set( pl_w, 6, 6 ) ;
	catch_gauge_add( pl_w ) ;
	pl_w->hit_stop = 10 ;
	pos.x = -90.0f ;
	pos.y = DOT_Y( 72.0f ) ;
	pos.z = 0.0f ;
	nage_hit_set( pl_w, &pos ) ;
	if( PL_CK( pl_w->catch_w, PL_SERVBOT ) ) pl_w->catch_w->pos.y = pl_w->ground_y + DOT_Y( 60.0f ) ;
	else if( PL_CK( pl_w->catch_w, PL_ROLL ) ) pl_w->catch_w->pos.y = pl_w->ground_y + DOT_Y( 20.0f ) ;
	pl_char_set( pl_w, Char_Catch, 0 ) ;
	pos = pl_w->pos ;
	catch_em_pos_set1( pl_w, pl_w->catch_w ) ;
	pl_w->pos = pos ;
}

/*==============================================================*/
static void jump_p_set( PLAYER_WORK *pl_w )
{
	VEC3 pos ;

	scr_excep_set( pl_w, 6, 6 ) ;
	catch_gauge_add( pl_w ) ;
	pl_w->hit_stop = 10 ;
	pos.x = -90.0f ;
	pos.y = DOT_Y( 72.0f ) ;
	pos.z = 0.0f ;
	nage_hit_set( pl_w, &pos ) ;
	pl_char_set( pl_w, Char_Catch, 0 ) ;
	pos = pl_w->pos ;
	catch_em_pos_set1( pl_w, pl_w->catch_w ) ;
	pl_w->pos = pos ;
}

/*==============================================================*/
static void sp_catch_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;
	VEC3 pos[2] ;

	scr_excep_set( pl_w, 5, 5 ) ;
	catch_gauge_add( pl_w ) ;
	pl_w->hit_stop = 10 ;
	ofs.x = -DOT_X( 76.0f ) ;
	ofs.y = DOT_Y( 62.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_char_set( pl_w, Char_Catch, 1 ) ;
	pos[0] = pl_w->pos ;
	pos[1] = pl_w->catch_w->pos ;
	catch_pos_set( pl_w, pl_w->catch_w ) ;
	pl_w->pos = pos[0] ;
	pl_w->catch_w->pos = pos[1] ;
}

static void nml_catch( PLAYER_WORK *pl_w );
static void sp_catch( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void pl11_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_catch,
		nml_catch,
		sp_catch,
		sp_catch,
		sp_catch,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[ pl_w->catch_no & CATCH_NO_MASK ]( pl_w ) ;
}

static void nml_catch_init( PLAYER_WORK *pl_w );
static void nml_catch_jump( PLAYER_WORK *pl_w );
static void nml_catch_throw( PLAYER_WORK *pl_w );
static void nml_catch_fall( PLAYER_WORK *pl_w );
static void nml_catch_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The normal throw: a jump that carries the			*/
/*			opponent, and a throw from the top					*/
/*==============================================================*/
static void nml_catch( PLAYER_WORK *pl_w )
{
	static void ( *nml_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_catch_init,
		nml_catch_jump,
		nml_catch_throw,
		nml_catch_fall,
		nml_catch_end,
	} ;

	nml_catch_jmp[ pl_w->routine_2 ]( pl_w ) ;
}

/*==============================================================*/
/*			The jump goes an eighth of the way to where the		*/
/*			hold puts her each frame							*/
/*==============================================================*/
static void nml_catch_init( PLAYER_WORK *pl_w )
{
	VEC3 pos[2] ;

	pl_w->routine_2 ++ ;
	pos[0] = pl_w->pos ;
	pos[1] = pl_w->pos ;
	catch_em_pos_set1( pl_w, pl_w->catch_w ) ;
	pos[0].x = pl_w->pos.x - pos[0].x ;
	pos[0].y = pl_w->pos.y - pos[0].y ;
	pl_w->pos = pos[1] ;
	pos[0].y /= 8.0f ;
	if( ( pl_w->catch_no & CATCH_NO_MASK ) != 1 && pos[0].y < 0.0f ) pos[0].y = 0.0f ;
	pl_w->spd.x = pos[0].x / 8.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = pos[0].y + 2.142857f ;
	pl_w->acc.y = -DOT_Y( 0.25f ) ;
}

/*--------------------------------------------------------------*/
static void nml_catch_jump( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void nml_catch_throw( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->pl_stance = STANCE_AIR ;
		SCR_EXCEP_SET ;
		em->atck_no = ( pl_w->catch_no & 1 ) ? 33 : 32 ;
		pl_w->spd.x = pl_w->pl_dir ? -5.0f : 5.0f ;
		pl_w->spd.y = DOT_Y( 6.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		ofs.x = -125.0f ;
		ofs.y = DOT_Y( 66.0f ) ;
		ofs.z = 0.0f ;
		hit_mark_set( pl_w, &ofs, 12 ) ;
		pl_se_req( pl_w, 5 ) ;
	}
}

/*--------------------------------------------------------------*/
static void nml_catch_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->spd.y > 0.0f ) return ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Jump, 5 ) ;
	}
}

/*--------------------------------------------------------------*/
static void nml_catch_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void sp_catch_init( PLAYER_WORK *pl_w );
static void sp_catch_pull( PLAYER_WORK *pl_w );
static void sp_catch_hit( PLAYER_WORK *pl_w );
static void sp_catch_wait( PLAYER_WORK *pl_w );
static void sp_catch_next( PLAYER_WORK *pl_w );
static void sp_catch_more( PLAYER_WORK *pl_w );
static void sp_catch_end( PLAYER_WORK *pl_w );
static void sp_catch_hit_set( PLAYER_WORK *pl_w );
static void sp_catch_release( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The counter's throw: hits while the opponent is		*/
/*			held, more while a button is pressed				*/
/*==============================================================*/
static void sp_catch( PLAYER_WORK *pl_w )
{
	static void ( *sp_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		sp_catch_init,
		sp_catch_pull,
		sp_catch_hit,
		sp_catch_wait,
		sp_catch_next,
		sp_catch_more,
		sp_catch_end,
	} ;

	sp_catch_jmp[ pl_w->routine_2 ]( pl_w ) ;
}

/*==============================================================*/
/*			The two close the gap between them over 16			*/
/*			frames												*/
/*==============================================================*/
static void sp_catch_init( PLAYER_WORK *pl_w )
{
	Float x[3] ;

	pl_w->routine_2 ++ ;
	x[0] = pl_w->pos.x ;
	x[1] = pl_w->catch_w->pos.x ;
	x[2] = pl_w->catch_w->pos.x - pl_w->pos.x ;
	catch_pos_set( pl_w, pl_w->catch_w ) ;
	x[2] = pl_w->catch_w->pos.x - pl_w->pos.x - x[2] ;
	x[2] /= 16.0f ;
	pl_w->pos.x = x[0] ;
	pl_w->catch_w->pos.x = x[1] ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->catch_w->spd.x = 0.0f ;
	pl_w->catch_w->spd.y = 0.0f ;
	pl_w->catch_w->acc.x = 0.0f ;
	pl_w->catch_w->acc.y = 0.0f ;
	pl_w->catch_w->spd.x = x[2] ;
	pl_w->spd.x = -x[2] ;
	pl_w->timer0 = 16 ;
}

/*--------------------------------------------------------------*/
static void sp_catch_pull( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->catch_w->wall_flag ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
	}else{
		pl_w->catch_w->pos.x += pl_w->catch_w->spd.x ;
		pl_w->catch_w->spd.x += pl_w->catch_w->acc.x ;
	}
	if( !--pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 2 ;
		pl_w->dir = 0 ;
		catch_pos_set( pl_w, pl_w->catch_w ) ;
		pl_char_set( pl_w, Char_Catch, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void sp_catch_hit( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->catch_w ;

	pl_char_move( pl_w ) ;
	if( !em->vital_new || vs_sys_work.vs_st >= VS_END_WAIT ){
		sp_catch_release( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		sp_catch_hit_set( pl_w ) ;
		if( !--pl_w->timer0 ){
			pl_w->routine_2 ++ ;
			pl_w->timer0 = 7 ;
			pl_w->timer1 = 12 ;
			pl_char_set( pl_w, Char_Catch, 11 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void sp_catch_wait( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->catch_w ;

	pl_char_move( pl_w ) ;
	if( !--pl_w->timer1 || !em->vital_new || vs_sys_work.vs_st >= VS_END_WAIT ) sp_catch_release( pl_w ) ;
	else{
		if( pl_w->cpu_flag ){
			if( !sw_renda_ck( pl_w ) ) return ;
		} else if( !( pl_w->pl_sw_now & KEY_PUNCH ) ) return ;
		pl_w->routine_2 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void sp_catch_next( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		if( --pl_w->timer0 ){
			pl_w->routine_2 ++ ;
			pl_char_set( pl_w, Char_Catch, 3 ) ;
		}else{
			pl_w->routine_2 = 6 ;
			pl_char_set( pl_w, Char_Catch, 5 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void sp_catch_more( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		sp_catch_hit_set( pl_w ) ;
		pl_w->routine_2 = 3 ;
		pl_w->timer1 = 12 ;
	}
}

/*--------------------------------------------------------------*/
static void sp_catch_end( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	if( pl_char_move( pl_w ) >= 0 ){
		switch( pl_w->cg_type ){
		case 1:
			pl_w->cg_type = 0 ;
			pls11_00_set( pl_w, 0, 0 ) ;
			pls11_00_set( pl_w, 2, 0 ) ;
			SCR_EXCEP_SET ;
			em = pl_w->catch_w ;
			em->dm_hit_w = pl_w ;
			em->catch_type = 1 ;
			em->atck_no = pl11_at_counter_end ;
			ofs.x = em->pos.x - pl_w->pos.x ;
			if( ofs.x > 0.0f ) ofs.x = -ofs.x ;
			ofs.y = em->size_top * 2.142857f / 1.8f ;
			ofs.z = 0.0f ;
			hit_mark_set( pl_w, &ofs, 1 ) ;
			combo_add( em, pl_w, 1 ) ;
			pl_se_req( pl_w, 7 ) ;
			break ;
		case 2:
			pl_w->cg_type = 0 ;
			pls11_00_set( pl_w, 1, 0 ) ;
			break ;
		}
	}else{
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
/*			One hit on the held opponent, with its spark at		*/
/*			the body's middle									*/
/*==============================================================*/
static void sp_catch_hit_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->catch_w ;
	VEC3 ofs ;

	pl_w->dir ++ ;
	pls11_00_set( pl_w, 0, 0 ) ;
	pls11_00_set( pl_w, 2, 0 ) ;
	em->atck_no = pl11_at_counter_hit ;
	catch_damage( pl_w ) ;
	ofs.x = em->pos.x - pl_w->pos.x ;
	if( ofs.x > 0.0f ) ofs.x = -ofs.x ;
	ofs.y = em->size_top * 2.142857f / 1.8f ;
	ofs.z = 0.0f ;
	hit_mark_set( pl_w, &ofs, 1 ) ;
	combo_add( em, pl_w, 1 ) ;
	pl_se_req( pl_w, 7 ) ;
	pl_char_set( em, Char_Damage, ( pl_w->dir & 1 ) ? 3 : 0 ) ;
}

/*==============================================================*/
/*			Let go when the opponent is out or the hits are		*/
/*			used up												*/
/*==============================================================*/
static void sp_catch_release( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->catch_w ;

	pl_w->routine_2 = 6 ;
	SCR_EXCEP_SET ;
	em->dm_hit_w = pl_w ;
	em->catch_type = 1 ;
	em->atck_no = pl11_at_counter_release ;
	pl_char_set( pl_w, Char_Catch, 4 ) ;
}

static void hold( PLAYER_WORK *pl_w );
static void sp_hold( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Holding the opponent								*/
/****************************************************************/
void pl11_hold( PLAYER_WORK *pl_w )
{
	static void ( *hold_jmp[])( PLAYER_WORK *pl_w ) = {
		hold,
		hold,
		sp_hold,
		sp_hold,
		sp_hold,
	} ;

	hold_jmp[ pl_w->catch_no & CATCH_NO_MASK ]( pl_w ) ;
}

/*==============================================================*/
static void hold( PLAYER_WORK *pl_w )
{
	if( pl_w->catch_w->routine_2 < 2 ) return ;
	catch_em_pos_set0( pl_w->catch_w, pl_w ) ;
}

/*==============================================================*/
static void sp_hold( PLAYER_WORK *pl_w )
{
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void pl11_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl11_sp_rocket ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl11_sp_hstars ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl11_sp_rocket ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl11_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl11_sp_rocket ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl11_sp_hstars ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl11_sp_rocket ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl11_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl11_sp_heal ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl11_sp_stars ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl11_sp_grenade ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void pl11_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl11_sp_crow ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl11_sp_stars ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl11_sp_grenade ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
/*			Clear the work, all but zombie_shot_flag				*/
/****************************************************************/
void pl11_tlocal_init( PLAYER_WORK *pl_w )
{
	PL11_LOCAL_W *lw = ( PL11_LOCAL_W * )pl_w->tlocal ;
	Uint8 keep = lw->zombie_shot_flag ;

	memset( lw, 0, sizeof( pl_w->tlocal ) ) ;
	lw->zombie_shot_flag = keep ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl11.c" End of File	*/

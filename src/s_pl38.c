/* Captain Commando (character 38): command checks, normal attacks, appearance, win poses, special
   moves and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl38_def.h"

void pl38_main( PLAYER_WORK *pl_w );
void pl38_com_ck( PLAYER_WORK *pl_w );
void pl38_atck_set( PLAYER_WORK *pl_w );
void pl38_atck( PLAYER_WORK *pl_w );
void pl38_dash( PLAYER_WORK *pl_w );
void pl38_noki( PLAYER_WORK *pl_w );
void pl38_appear( PLAYER_WORK *pl_w );
void pl38_win( PLAYER_WORK *pl_w );
void pl38_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl38_catch_ck( PLAYER_WORK *pl_w );
void pl38_catch_set( PLAYER_WORK *pl_w );
void pl38_catch( PLAYER_WORK *pl_w );
void pl38_hold( PLAYER_WORK *pl_w );
int pl38_delay_ck( PLAYER_WORK *pl_w );
void pl38_t_kick( PLAYER_WORK *pl_w );
void pl38_upper( PLAYER_WORK *pl_w );
void pl38_sp_unison( PLAYER_WORK *pl_w );
void pl38_sp_gattai( PLAYER_WORK *pl_w );
void pl38_sp_assist( PLAYER_WORK *pl_w );
void pl38_sp_zero( PLAYER_WORK *pl_w );
void pl38_char_rnd_set( PLAYER_WORK *pl_w );
void pl38_char_rnd_set_2( PLAYER_WORK *pl_w );
void pl38_char_rnd_set_3( PLAYER_WORK *pl_w );

/* each attack's table: four bytes for each button (punch, kick) and strength */
static Sint8 jump_tbl[24] = {
	  1,  2,  1,  2, -1,  2, -1,  2, -1, -1, -1, -1,
	  1,  2,  1,  2, -1,  2, -1,  2, -1, -1, -1, -1,
} ;
static Sint8 nml_tbl[24] = {
	  1,  2,  0,  2, -1,  2,  1,  2, -1, -1, -1,  2,
	  1,  2,  1,  2, -1,  2, -1,  2, -1, -1, -1, -1,
} ;
static Sint8 sjump_tbl[24] = {
	  1,  2,  0,  2, -1,  2,  1,  2, -1, -1, -1,  2,
	  1,  2,  1,  2, -1,  2, -1,  2, -1, -1, -1, -1,
} ;

/* commands: time or steps, kind, the special move, then the stick and buttons */
static Uint16 com_sword[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl38_sp_sword, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Captain Sword (hyper) */
static Uint16 com_fire[] = { 3, COM_GROUND, pl38_sp_fire, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Captain Fire */
static Uint16 com_jfire[] = { 3, COM_AIR, pl38_sp_jfire, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Captain Fire in the air */
static Uint16 com_corridor[] = { 3, COM_GROUND, pl38_sp_corridor, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Captain Corridor */
static Uint16 com_storm[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl38_sp_storm, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Captain Storm (hyper) */
static Uint16 com_strike[] = { 3, COM_GROUND, pl38_sp_strike, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Commando Strike */
static Uint16 com_strike_pk[] = { 3, COM_GROUND | COM_ALL_SW, pl38_sp_strike, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_HP | KEY_LK } ;	/* Commando Strike with HP and LK */
static Uint16 com_kick[] = { 3, COM_GROUND, pl38_sp_kick, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Captain Kick */

static void ( *pl38_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl38_main,
	pl38_com_ck,
	pl38_atck_set,
	pl38_atck,
	pl38_dash,
	pl38_noki,
	pl38_appear,
	pl38_win,
	pl38_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl38_catch_ck,	/* returns a value */
	pl38_catch_set,
	pl38_catch,
	pl38_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl38_delay_ck,
	pl38_t_kick,
	pl38_upper,
	pl38_sp_unison,
	pl38_sp_gattai,
	pl38_sp_assist,
	pl38_sp_zero,
	0,
	0,
	0,
	0,
	pl38_char_rnd_set,
	pl38_char_rnd_set_2,
	pl38_char_rnd_set_3,
} ;

static Uint8 sword_ck( PLAYER_WORK *pl_w );
static Uint8 fire_ck( PLAYER_WORK *pl_w );
static Uint8 jfire_ck( PLAYER_WORK *pl_w );
static Uint8 corridor_ck( PLAYER_WORK *pl_w );
static Uint8 storm_ck( PLAYER_WORK *pl_w );
static Uint8 strike_ck( PLAYER_WORK *pl_w );
static Uint8 strike_pk_ck( PLAYER_WORK *pl_w );
static Uint8 kick_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl38_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl38_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl38_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( storm_ck( pl_w ) ) return ;
	if( corridor_ck( pl_w ) ) return ;
	if( sword_ck( pl_w ) ) return ;
	if( strike_pk_ck( pl_w ) ) return ;
	if( strike_ck( pl_w ) ) return ;
	if( fire_ck( pl_w ) ) return ;
	if( jfire_ck( pl_w ) ) return ;
	if( kick_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
/*			Not again until shot_timer runs out					*/
/*==============================================================*/
static Uint8 sword_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_sword, &pl_w->com_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( PL38_LW( pl_w )->shot_timer ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl38_sp_sword ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 fire_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_fire, &pl_w->com_w0 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl38_sp_fire ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			In the air, once a jump unless super jumping		*/
/*==============================================================*/
static Uint8 jfire_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_jfire, &pl_w->com_w2 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl38_sp_jfire ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 corridor_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_corridor, &pl_w->com_w1 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl38_sp_corridor ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 storm_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_storm, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( PL38_LW( pl_w )->shot_timer ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w3 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl38_sp_storm ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
/*			The strength is the button's, or the attack			*/
/*			level for a partner									*/
/*==============================================================*/
static Uint8 strike_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_strike, &pl_w->com_w4 ) ) return 0 ;
	if( PL38_LW( pl_w )->shot_timer ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w4 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl38_sp_strike ;
	act_st_set( pl_w, N_SPECIAL ) ;
	if( !pl_w->cpu_flag ) PL38_LW( pl_w )->strike_level = pl_w->atck_level ;
	else PL38_LW( pl_w )->strike_level = pl_w->atck_pk ;
	return 1 ;
}

/*==============================================================*/
static Uint8 strike_pk_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_strike_pk, &pl_w->com_w6 ) ) return 0 ;
	if( PL38_LW( pl_w )->shot_timer ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w6 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl38_sp_strike ;
	act_st_set( pl_w, N_SPECIAL ) ;
	if( !pl_w->cpu_flag ) PL38_LW( pl_w )->strike_level = 2 ;
	else PL38_LW( pl_w )->strike_level = pl_w->atck_pk ;
	return 1 ;
}

/*==============================================================*/
static Uint8 kick_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_kick, &pl_w->com_w5 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w5 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl38_sp_kick ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int snapback_ck( PLAYER_WORK *pl_w )
{
	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl38_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 7 ) ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl38_sp_chouhatsu ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl38_delay_ck( PLAYER_WORK *pl_w )
{
	if( com_nml_ck( pl_w, com_sword, &pl_w->com_w ) && *pl_w->gauge_level ){
		pl_w->delay_sp_type = pl38_sp_sword ;
		return 1 ;
	}
	if( com_nml_ck( pl_w, com_storm, &pl_w->com_w3 ) && *pl_w->gauge_level ){
		pl_w->delay_sp_type = pl38_sp_storm ;
		return 1 ;
	}
	return 0 ;
}

/****************************************************************/
void pl38_main( PLAYER_WORK *pl_w )
{
	if( PL38_LW( pl_w )->shot_timer ) PL38_LW( pl_w )->shot_timer -- ;
}

/*==============================================================*/
/*			Normal attacks										*/
/*==============================================================*/
static void nml_atck( PLAYER_WORK *pl_w )
{
	pl_brake_set( pl_w ) ;
	pl_w->chain_tbl = &nml_tbl[pl_w->atck_pk * 12 + pl_w->nml_no * 4] ;
	pl_w->nml_level = pl_w->nml_no ;
	if( pl_w->atck_pk && pl_w->nml_no == 2 && !pl_w->pl_stance ){
		pl_w->atck_no = pl38_at_stand_hk ;
		HIT_RESET( pl_w ) ;
	} else {
		pl_w->atck_no = pl_w->pl_stance * 6 + pl_w->atck_pk * 3 + pl_w->nml_no ;
		HIT_RESET( pl_w ) ;
	}
	pl_char_set( pl_w, ( pl_w->pl_stance << 1 ) + pl_w->atck_pk + Char_Stand_P, pl_w->nml_no ) ;
	if( pl_w->atck_pk || pl_w->nml_no != 2 ) pl_se_req( pl_w, pl_w->nml_no + 20 ) ;
}

/*==============================================================*/
/*			In the air; a super jump has its own table			*/
/*==============================================================*/
static void jump_atck( PLAYER_WORK *pl_w )
{
	register Sint16 ofs ;
	Sint8 *tbl ;

	ofs = pl_w->atck_pk * 12 ;
	ofs += pl_w->nml_no * 4 ;
	if( !pl_w->sjump_flag ) tbl = jump_tbl + ofs ;
	else tbl = sjump_tbl + ofs ;
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
/*			In a jump only while that button's attacks are		*/
/*			left												*/
/*==============================================================*/
static void jump_atck_ck( PLAYER_WORK *pl_w )
{
	if( !pl_w->atck_pk ){
		if( pl_w->jump_atk_cnt & JATK_P_MASK ) jump_atck( pl_w ) ;
	} else if( pl_w->jump_atk_cnt & JATK_K_MASK ) jump_atck( pl_w ) ;
}

/****************************************************************/
void pl38_atck_set( PLAYER_WORK *pl_w )
{
	static void ( *atck_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_atck, jump_atck_ck, jump_atck, nml_atck,
	} ;

	atck_set_jmp[pl_w->atck_type]( pl_w ) ;
}

/*==============================================================*/
/*			The standing heavy punch steps forward				*/
/*==============================================================*/
static void punch_move( PLAYER_WORK *pl_w )
{
	Float step ;

	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	else if( pl_w->cg_prm ){
		pl_w->cg_prm = 0 ;
		step = -DOT_X( 32.0f ) ;
		if( pl_w->disp_dir ) step = DOT_X( 32.0f ) ;
		pl_w->pos.x += step ;
	}
}

/*==============================================================*/
/*			The standing heavy kick hops forward				*/
/*==============================================================*/
static void kick_move( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_char_move( pl_w ) ;
		if( !pl_w->cg_type ){
			pl_w->routine_2 ++ ;
			pl_w->spd.x = -DOT_X( 4.0f ) ;
			pl_w->acc.x = DOT_X( 0.25f ) ;
			pl_w->spd.y = DOT_Y( 4.0f ) ;
			pl_w->acc.y = -DOT_Y( 0.375f ) ;
			if( pl_w->pl_dir ){
				pl_w->spd.x = -pl_w->spd.x ;
				pl_w->acc.x = -pl_w->acc.x ;
			}
		}
	} else if( pl_w->routine_2 == 1 ){
		pl_char_move( pl_w ) ;
		if( pl_land_ck3( pl_w ) ){
			pl_w->pl_stance = STANCE_STAND ;
			pl_w->routine_2 ++ ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_char_set2( pl_w, Char_Stand_K, 2, 7 ) ;
			pl_land_ef_set( pl_w ) ;
		}
	} else {
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	}
}

/****************************************************************/
void pl38_atck( PLAYER_WORK *pl_w )
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
		if( !pl_w->pl_stance && pl_w->nml_no == 2 ){
			if( !pl_w->atck_pk ) punch_move( pl_w ) ;
			else kick_move( pl_w ) ;
		} else if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
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
void pl38_upper( PLAYER_WORK *pl_w )
{
	if( pl_w->atck_type == 3 ) pl_smoke_set( pl_w ) ;
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_brake_set( pl_w ) ;
		pl_w->atck_no = pl38_at_upper ;
		HIT_RESET( pl_w ) ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_gauge_add( pl_w, 5 ) ;
		pl_char_set( pl_w, Char_Extra, 27 ) ;
		pl_se_req( pl_w, SE_SWING_M ) ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_brake_stop( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void dash_init( PLAYER_WORK *pl_w );
static void dash_move( PLAYER_WORK *pl_w );
static void dash_slow( PLAYER_WORK *pl_w );
static void dash_end( PLAYER_WORK *pl_w );

/****************************************************************/
void pl38_dash( PLAYER_WORK *pl_w )
{
	static void ( *dash_jmp[])( PLAYER_WORK *pl_w ) = {
		dash_init, dash_move, dash_slow, dash_end,
	} ;

	dash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dash_init( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->timer0 = 10 ;
		pl_w->spd.x = -DOT_X( 9.5f ) ;
		pl_w->acc.x = 0.3125f ;
		if( pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
	}
}

/*--------------------------------------------------------------*/
static void dash_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 <= 0 ) pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void dash_slow( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) <= 0 ){
		pl_w->routine_2 ++ ;
		pl_w->acc.x = DOT_X( 0.375f ) ;
		if( pl_w->disp_dir ) pl_w->acc.x = -pl_w->acc.x ;
		pl_char_set( pl_w, Char_Dash, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void dash_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) >= 0 ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->spd.x * pl_w->acc.x >= 0.0f ) RATE_CLR_X( pl_w ) ;
	} else {
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void noki_init( PLAYER_WORK *pl_w );
static void noki_move( PLAYER_WORK *pl_w );
static void noki_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash										*/
/****************************************************************/
void pl38_noki( PLAYER_WORK *pl_w )
{
	static void ( *noki_jmp[])( PLAYER_WORK *pl_w ) = {
		noki_init, noki_move, noki_end,
	} ;

	noki_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void noki_init( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->timer0 = 10 ;
		pl_w->spd.x = DOT_X( 9.5f ) ;
		pl_w->acc.x = -0.3125f ;
		if( pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
	}
}

/*--------------------------------------------------------------*/
static void noki_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->acc.x = -DOT_X( 0.875f ) ;
		if( pl_w->disp_dir ) pl_w->acc.x = -pl_w->acc.x ;
		pl_char_set( pl_w, Char_Dash, 3 ) ;
	} else if( pl_w->cg_prm ) ;
}

/*--------------------------------------------------------------*/
static void noki_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) >= 0 ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->spd.x * pl_w->acc.x >= 0.0f ) RATE_CLR_X( pl_w ) ;
	} else {
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void appear_init( PLAYER_WORK *pl_w );
static void appear_wait( PLAYER_WORK *pl_w );
static void appear_move( PLAYER_WORK *pl_w );
static void appear_next( PLAYER_WORK *pl_w );
static void appear_end( PLAYER_WORK *pl_w );
static void appear_color( PLAYER_WORK *pl_w );

/****************************************************************/
void pl38_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_init, appear_wait, appear_move, appear_next, appear_end,
	} ;

	appear_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void appear_init( PLAYER_WORK *pl_w )
{
	Uint16 col ;
	Sint8 no ;

	pl_w->routine_2 ++ ;
	pl_w->disp_flag = 1 ;
	col = pl_w->pl_color * 2 + 6 ;
	no = 13 ;
	if( !pl_w->cpu_flag && !( pl_w->pad_sw.now & KEY_KICK ) ){
		col = pl_w->pl_color * 2 + 5 ;
		no = 0 ;
	}
	pl_char_set( pl_w, Char_Appear, no ) ;
	ef_color_set( pl_w, 2, col, 1 ) ;
}

/*--------------------------------------------------------------*/
static void appear_wait( PLAYER_WORK *pl_w )
{
	if( vs_sys_work.vs_st >= VS_APPEAR ){
		pl_char_move( pl_w ) ;
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pl_w->routine_2 ++ ;
			pls38_00_set( pl_w ) ;
			pl_w->timer0 = -1 ;
		} else if( pl_w->cg_flag1 ) pl_w->cg_flag1 = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void appear_move( PLAYER_WORK *pl_w )
{
	appear_color( pl_w ) ;
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void appear_next( PLAYER_WORK *pl_w )
{
	appear_color( pl_w ) ;
	pl_w->routine_2 ++ ;
	pl_char_set( pl_w, Char_Appear, 1 ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
}

/*--------------------------------------------------------------*/
static void appear_end( PLAYER_WORK *pl_w )
{
	appear_color( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_1 ++ ;
		pl_char_set( pl_w, Char_Stand, 0 ) ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
	}
}

/*==============================================================*/
/*			Cycles the palette through four colours				*/
/*==============================================================*/
static void appear_color( PLAYER_WORK *pl_w )
{
	pl_w->timer0 ++ ;
	pl_w->timer0 &= 3 ;
	ef_color_set( pl_w, 1, pl_w->timer0, 1 ) ;
}

/****************************************************************/
/*			After sp_storm or a normal win, only the first pose	*/
/****************************************************************/
void pl38_win( PLAYER_WORK *pl_w )
{
	Sint8 no ;
	int sp ;

	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		if( !pl_w->type || pl_w->type == 2 ){
			pl_w->routine_3 = 0 ;
			if( Rnd() & 1 ){
				sp = pl_w->pl_sp_type ;
				if( sp != pl38_sp_storm && sp ){
					if( Rnd() & 1 ){
						no = 2 ;
						pl_w->routine_3 = 2 ;
					} else no = 0 ;
				} else {
					no = 1 ;
					pl_w->routine_3 = 1 ;
				}
			} else {
				no = 1 ;
				pl_w->routine_3 = 1 ;
			}
			pl_char_set( pl_w, Char_Win, no ) ;
			if( pl_w->routine_3 == 1 ) Shl38_set( pl_w, 3, 0 ) ;
		} else {
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_char_set( pl_w, Char_Win, 3 ) ;
		}
	} else if( pl_win_end_ck( pl_w ) ) pl_stand_ret( pl_w ) ;
	else if( !pl_w->type || pl_w->type == 2 ){
		pl_char_move( pl_w ) ;
		if( !pl_w->routine_3 ){
			if( pl_w->cg_type ){
				pl_w->cg_type = 0 ;
				Shl38_01_set( pl_w, 0, 2 ) ;
			}
		} else if( pl_w->routine_3 == 2 ){
			if( pl_w->cg_type ){
				pl_w->cg_type = 0 ;
				Shl38_01_set( pl_w, 0, 4 ) ;
			}
		}
	} else pl_char_move( pl_w ) ;
}

static void sp_sword( PLAYER_WORK *pl_w );
static void sp_fire( PLAYER_WORK *pl_w );
static void sp_corridor( PLAYER_WORK *pl_w );
static void sp_storm( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_strike( PLAYER_WORK *pl_w );
static void sp_kick( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Special moves and hyper combos; the unused			*/
/*			numbers fall back on sp_sword							*/
/****************************************************************/
void pl38_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *pl_sp_tbl[])( PLAYER_WORK *pl_w ) = {
		sp_sword, sp_fire, sp_corridor, sp_sword, sp_fire, sp_sword, sp_storm, sp_chouhatsu,
		sp_strike, sp_sword, sp_sword, sp_sword, sp_kick, sp_snapback,
	} ;

	pl_sp_tbl[pl_w->pl_sp_type]( pl_w ) ;
}

/*==============================================================*/
/*			Stop, and stand on the ground						*/
/*==============================================================*/
static void sp_stand_set( PLAYER_WORK *pl_w )
{
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_dir_set( pl_w ) ;
}

static void sword_init( PLAYER_WORK *pl_w );
static void sword_wait( PLAYER_WORK *pl_w );
static void sword_shot( PLAYER_WORK *pl_w );
static void sword_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_sword( PLAYER_WORK *pl_w )
{
	static void ( *sword_jmp[])( PLAYER_WORK *pl_w ) = {
		sword_init, sword_wait, sword_shot, sword_end,
	} ;

	sword_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void sword_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	sp_stand_set( pl_w ) ;
	pl_w->atck_no = pl38_at_sword ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 0 ) ;
	pl_sp_ef_set( pl_w ) ;
	if( pl_w->team_type != 5 ) Shl38_01_set( pl_w, 0, 0 ) ;
}

/*--------------------------------------------------------------*/
static void sword_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = -10.0f ;
		ofs.y = DOT_Y( 132.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void sword_shot( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->dir = pl_w->cg_type ;
		if( !Shl38_02_set( pl_w ) || !pls38_01_sword_set( pl_w ) ) pl_stand_ret( pl_w ) ;
		else pl_voice_req( pl_w, 16 ) ;
	}
}

/*--------------------------------------------------------------*/
static void sword_end( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	else {
		if( pl_w->cg_type < 0 ){ }
		if( pl_w->cg_type != pl_w->dir ){
			pl_w->hit_flag = 0 ;
			pl_w->dir = pl_w->cg_type ;
		}
		if( pl_w->cg_flag1 & 2 ){
			pl_w->cg_flag1 &= ~2 ;
			Shl38_set( pl_w, 10, 0 ) ;
			pl_w->delay_flag = 0 ;
			pl_w->delay_timer = 0 ;
			pl_w->hyper_bg = 0 ;
			pl_w->hyper_bg_tm = 0 ;
		}
	}
}

static void fire_init( PLAYER_WORK *pl_w );
static void fire_move( PLAYER_WORK *pl_w );
static void fire_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			On the ground, or in the air as pl38_sp_jfire		*/
/*==============================================================*/
static void sp_fire( PLAYER_WORK *pl_w )
{
	static void ( *fire_jmp[])( PLAYER_WORK *pl_w ) = {
		fire_init, fire_move, fire_end,
	} ;

	if( pl_w->pl_stance == STANCE_AIR ){
		pl_jump_move( pl_w ) ;
		if( pl_w->pos.y < pl_w->ground_y ) pl_w->pos.y = pl_w->ground_y ;
	}
	fire_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void fire_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	if( pl_w->pl_stance == STANCE_AIR ){
		pl_dir_set( pl_w ) ;
		if( !pl_w->atck_level ){
			pl_w->atck_no = pl38_at_jfire_l ;
			HIT_RESET( pl_w ) ;
		} else {
			pl_w->atck_no = pl38_at_jfire_h ;
			HIT_RESET( pl_w ) ;
		}
		pl_w->spd.x /= 16.0f ;
		pl_w->spd.y /= 8.0f ;
		pl_w->acc.y /= 64.0f ;
		pl_w->acc.x = 0.0f ;
		pl_char_set( pl_w, Char_Special, ( pl_w->atck_level << 1 ) + 6 ) ;
	} else {
		sp_stand_set( pl_w ) ;
		if( !pl_w->atck_level ){
			pl_w->atck_no = pl38_at_fire_l ;
			HIT_RESET( pl_w ) ;
		} else {
			pl_w->atck_no = pl38_at_fire_h ;
			HIT_RESET( pl_w ) ;
		}
		pl_char_set( pl_w, Char_Special, pl_w->atck_level << 1 ) ;
		pl_sp_ef_set( pl_w ) ;
	}
	pl_gauge_add( pl_w, 5 ) ;
}

/*--------------------------------------------------------------*/
static void fire_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->unk_27b = 0 ;
		pl_w->cancel_timer = 16 ;
		if( pl_w->pl_stance == STANCE_AIR ) Shl38_set( pl_w, 4, 0 ) ;
		else Shl38_set( pl_w, 0, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void fire_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		if( pl_w->pl_stance == STANCE_AIR ) pl_jump_ret( pl_w ) ;
		else pl_stand_ret( pl_w ) ;
	}
}

static void corridor_init( PLAYER_WORK *pl_w );
static void corridor_move( PLAYER_WORK *pl_w );
static void corridor_flash( PLAYER_WORK *pl_w );
static void flash_ck( PLAYER_WORK *pl_w );
static void sp_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_corridor( PLAYER_WORK *pl_w )
{
	static void ( *corridor_jmp[])( PLAYER_WORK *pl_w ) = {
		corridor_init, corridor_move, corridor_flash, sp_end,
	} ;

	corridor_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void corridor_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	sp_stand_set( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	if( !pl_w->atck_level ){
		pl_w->atck_no = pl38_at_corridor_l ;
		HIT_RESET( pl_w ) ;
	} else {
		pl_w->atck_no = pl38_at_corridor_h ;
		HIT_RESET( pl_w ) ;
	}
	pl_char_set( pl_w, Char_Super, ( pl_w->atck_level << 1 ) + 6 ) ;
	pl_sp_ef_set( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void corridor_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		scr_work.quake_level = 1 ;
		scr_work.quake_flag = 1 ;
		pl_w->routine_2 ++ ;
		PL38_LW( pl_w )->wait_timer = PL38_LW( pl_w )->flash_wait = 1 ;
		PL38_LW( pl_w )->unk_02 = PL38_LW( pl_w )->hit_cnt = 0 ;
		Shl38_set( pl_w, 1, 0 ) ;
		Shl38_set( pl_w, 3, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void corridor_flash( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	flash_ck( pl_w ) ;
}

/*==============================================================*/
/*			Flashes every flash_wait frames until the			*/
/*			animation says to stop								*/
/*==============================================================*/
static void flash_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
	} else if( -- PL38_LW( pl_w )->wait_timer < 0 ){
		PL38_LW( pl_w )->wait_timer = PL38_LW( pl_w )->flash_wait ;
		PL38_LW( pl_w )->unk_02 ^= 1 ;
		if( !( PL38_LW( pl_w )->unk_02 & 1 ) ) pl_color_set( pl_w, PC_DEFAULT ) ;
		else ef_color_set( pl_w, 0, 4, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void sp_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void storm_init( PLAYER_WORK *pl_w );
static void storm_wait( PLAYER_WORK *pl_w );
static void storm_shot( PLAYER_WORK *pl_w );
static void storm_rush( PLAYER_WORK *pl_w );
static void storm_move( PLAYER_WORK *pl_w );
static void storm_jump( PLAYER_WORK *pl_w );
static void storm_fall( PLAYER_WORK *pl_w );
static void storm_land( PLAYER_WORK *pl_w );
static void storm_walk( PLAYER_WORK *pl_w );
static void storm_meet( PLAYER_WORK *pl_w );
static void storm_flash( PLAYER_WORK *pl_w );
static void storm_flash_keep( PLAYER_WORK *pl_w );
static void storm_last( PLAYER_WORK *pl_w );
static void storm_end( PLAYER_WORK *pl_w );
static void storm_hit( PLAYER_WORK *pl_w );
static PLAYER_WORK *partner_get( Uint8 pl_no, Uint8 no );

/*==============================================================*/
static void sp_storm( PLAYER_WORK *pl_w )
{
	static void ( *storm_jmp[])( PLAYER_WORK *pl_w ) = {
		storm_init, storm_wait, storm_shot, storm_rush, storm_move, storm_jump, storm_fall,
		storm_land, storm_walk, storm_meet, storm_flash, storm_flash_keep, storm_last, storm_end,
	} ;

	storm_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void storm_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	sp_stand_set( pl_w ) ;
	pl_w->atck_no = pl38_at_storm ;
	HIT_RESET( pl_w ) ;
	PL38_LW( pl_w )->rush_hit_flag = 0 ;
	pl_char_set( pl_w, Char_Super, 24 ) ;
	scr_excep_set( pl_w, 1, 0 ) ;
	PL38_LW( pl_w )->unk_02 = PL38_LW( pl_w )->hit_cnt = 0 ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_sp_ef_set( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void storm_wait( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_prm ){
		pl_w->cg_prm = 0 ;
		Shl38_01_set( pl_w, 0, 1 ) ;
		pl_w->routine_2 ++ ;
		PL38_LW( pl_w )->wait_timer = 16 ;
	}
}

/*--------------------------------------------------------------*/
static void storm_shot( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( -- PL38_LW( pl_w )->wait_timer < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = 0.0f ;
		ofs.y = DOT_Y( 80.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void storm_rush( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->routine_2 ++ ;
	pl_w->spd.x = -DOT_X( 16.0f ) ;
	pl_w->acc.x = DOT_X( 0.25f ) ;
	if( pl_w->disp_dir ){
		pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = -pl_w->acc.x ;
	}
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	PL38_LW( pl_w )->wait_timer = 12 ;
	pl_char_set( pl_w, Char_Super, 26 ) ;
}

/*==============================================================*/
/*			Until the rush hits or its time runs out			*/
/*==============================================================*/
static void storm_move( PLAYER_WORK *pl_w )
{
	int sx, sx2 ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	sx = pl_w->spd.x ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	sx2 = pl_w->spd.x ;
	if( ( sx ^ sx2 ) < 0 ) RATE_CLR_X( pl_w ) ;
	if( ( vs_sys_work.push_bit & ( 1 << pl_w->work_no ) ) || -- PL38_LW( pl_w )->wait_timer < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Super, 27 ) ;
	}
}

/*--------------------------------------------------------------*/
static void storm_jump( PLAYER_WORK *pl_w )
{
	int sx, sx2 ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	sx = pl_w->spd.x ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	sx2 = pl_w->spd.x ;
	if( ( sx ^ sx2 ) < 0 ) RATE_CLR_X( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.y = DOT_Y( 4.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
	}
	storm_hit( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void storm_fall( PLAYER_WORK *pl_w )
{
	Float sx ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	sx = pl_w->spd.x ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( sx * pl_w->spd.x < 0.0f ) RATE_CLR_X( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( pl_w->pos.y < pl_w->ground_y ){
		if( !PL38_LW( pl_w )->rush_hit_flag ){
			pl_w->delay_flag = 0 ;
			pl_w->delay_timer = 0 ;
			pl_w->hyper_bg = 0 ;
			pl_w->hyper_bg_tm = 0 ;
		}
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_land_ef_set( pl_w ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Jump, 3 ) ;
	}
	storm_hit( pl_w ) ;
}

/*==============================================================*/
/*			The partner in slot no of player pl_no's team,		*/
/*			if it is this character and on the screen			*/
/*==============================================================*/
static PLAYER_WORK *partner_get( Uint8 pl_no, Uint8 no )
{
	PLAYER_WORK *mw = vs_sys_work.team_w[pl_no][no] ;

	if( mw->be_flag && mw->routine_1 == PL_DAMAGE && mw->target_w->pl_type == PL_CAPTAIN_COMMANDO ) return mw ;
	return 0 ;
}

/*==============================================================*/
/*			Lands; with a partner of the same character on		*/
/*			the team, walks to him								*/
/*==============================================================*/
static void storm_land( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *mw ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		if( PL38_LW( pl_w )->rush_hit_flag ){
			pl_w->routine_2 ++ ;
			PL38_LW( pl_w )->rush_result = 0 ;
			if( mw = partner_get( pl_w->target_w->pl_no, 0 ) ) goto found ;
			if( mw = partner_get( pl_w->target_w->pl_no, 1 ) ) goto found ;
			if( mw = partner_get( pl_w->target_w->pl_no, 2 ) ) goto found ;
			goto stand ;
found :
			if( !mw->team_no ){
				pl_w->spd.x = ( mw->pos.x - pl_w->pos.x ) / 32.0f ;
				pl_w->acc.x = 0.0f ;
				pl_w->disp_dir = ( pl_w->spd.x < 0.0f ) ? 0 : 1 ;
				pl_w->pl_dir = pl_w->disp_dir ;
				PL38_LW( pl_w )->unk_02 = 31 ;
				PL38_LW( pl_w )->wait_timer = 57 ;
				pl_char_set( pl_w, Char_Stand, 2 ) ;
				return ;
			}
		}
	stand:
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_stand_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void storm_walk( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( -- PL38_LW( pl_w )->unk_02 < 0 ){
		pl_w->routine_2 ++ ;
		pl38_char_rnd_set_3( pl_w ) ;
	}
}

/*==============================================================*/
/*			Walks to the partner, then the throw				*/
/*==============================================================*/
static void storm_meet( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *mw ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( mw = partner_get( pl_w->target_w->pl_no, 0 ) ) goto found ;
	if( mw = partner_get( pl_w->target_w->pl_no, 1 ) ) goto found ;
	if( mw = partner_get( pl_w->target_w->pl_no, 2 ) ) goto found ;
	goto stand ;
found :
	if( mw->team_no ) goto stand ;
	pl_char_move( pl_w ) ;
	if( -- PL38_LW( pl_w )->wait_timer < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->atck_no = pl38_at_storm_meet ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 25 ) ;
	}
	return ;
stand :
	pl_w->delay_flag = 0 ;
	pl_w->delay_timer = 0 ;
	pl_w->hyper_bg = 0 ;
	pl_w->hyper_bg_tm = 0 ;
	pl_stand_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void storm_flash( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		scr_work.quake_level = 3 ;
		scr_work.quake_flag = 1 ;
		scr_excep_set( pl_w, 13, 7 ) ;
		pl_w->routine_2 ++ ;
		PL38_LW( pl_w )->wait_timer = PL38_LW( pl_w )->flash_wait = 1 ;
		PL38_LW( pl_w )->unk_02 = PL38_LW( pl_w )->hit_cnt = 0 ;
		Shl38_set( pl_w, 1, 0 ) ;
		Shl38_set( pl_w, 3, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void storm_flash_keep( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	flash_ck( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void storm_last( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		em = pl_w->em_w ;
		if( !em->work_kind ) em->ukemi_flag = 0 ;
		pl_char_set( pl_w, Char_Super, 29 ) ;
		Shl38_01_set( pl_w, 0, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void storm_end( PLAYER_WORK *pl_w )
{
	VEC3 pos ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_se_req( pl_w, 58 ) ;
		pl_stand_ret( pl_w ) ;
	} else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_se_req( pl_w, 74 ) ;
		pos.x = pl_w->pos.x ;
		pos.y = pl_w->pos.y + DOT_Y( 16.0f ) ;
		burst_set( pl_w, &pos, 3, 0x37a ) ;
		scr_excep_clr() ;
	}
}

/*==============================================================*/
/*			Counts the rush's hits; after five it stops			*/
/*==============================================================*/
static void storm_hit( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_w->hit_flag && !PL38_LW( pl_w )->unk_02 ){
		em = pl_w->em_w ;
		if( em->work_kind == W_PL && !em->armor_flag && !( pl_w->hit_flag & ( HR_GUARD | HR_ARMOR ) ) ){
			if( ++ PL38_LW( pl_w )->hit_cnt < 5 ){
				pl_w->atck_no = pl38_at_storm ;
				HIT_RESET( pl_w ) ;
				PL38_LW( pl_w )->unk_02 = 0 ;
			} else PL38_LW( pl_w )->unk_02 = -1 ;
			PL38_LW( pl_w )->rush_result = 1 ;
			PL38_LW( pl_w )->rush_hit_flag = -1 ;
			return ;
		}
		PL38_LW( pl_w )->rush_result = -1 ;
		PL38_LW( pl_w )->unk_02 = -1 ;
	}
}

/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		sp_stand_set( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, 2 ) ;
	} else if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		if( pl_w->cg_flag1 ) pl_color_set( pl_w, PC_DEFAULT ) ;
		else ef_color_set( pl_w, 0, pl_w->pl_color * 2 + 6, 1 ) ;
	}
}

/*==============================================================*/
static void sp_strike( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		sp_stand_set( pl_w ) ;
		pl_gauge_add( pl_w, 8 ) ;
		pl_w->atck_no = PL38_LW( pl_w )->strike_level + 66 ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Special, ( PL38_LW( pl_w )->strike_level << 1 ) + 12 ) ;
		pl_sp_ef_set( pl_w ) ;
	} else if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		Shl38_set( pl_w, PL38_LW( pl_w )->strike_level + 5, 0 ) ;
	}
}

static void kick_init( PLAYER_WORK *pl_w );
static void kick_wait( PLAYER_WORK *pl_w );
static void kick_rise( PLAYER_WORK *pl_w );
static void kick_fall( PLAYER_WORK *pl_w );
static void kick_land( PLAYER_WORK *pl_w );
static void spd_set( PLAYER_WORK *pl_w, float *spd );

/*==============================================================*/
static void sp_kick( PLAYER_WORK *pl_w )
{
	static void ( *kick_jmp[])( PLAYER_WORK *pl_w ) = {
		kick_init, kick_wait, kick_rise, kick_fall, kick_land, sp_end,
	} ;

	kick_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void kick_init( PLAYER_WORK *pl_w )
{
	int no ;

	pl_w->routine_2 ++ ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	sp_stand_set( pl_w ) ;
	pl_gauge_add( pl_w, 10 ) ;
	no = !pl_w->atck_level ? 81 : 83 ;
	pl_w->atck_no = no ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, ( pl_w->atck_level << 1 ) + 32 ) ;
	pl_sp_ef_set( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void kick_wait( PLAYER_WORK *pl_w )
{
	static float kick_spd[8] = {
		-DOT_X( 10.0f ), DOT_X( 0.5f ), DOT_Y( 1.5f ), -0.20089285f,
		-DOT_X( 10.0f ), DOT_X( 0.5f ), DOT_Y( 2.5f ), -0.20089285f,
	} ;
	int i ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		i = pl_w->atck_level * 4 ;
		spd_set( pl_w, &kick_spd[i] ) ;
	}
}

/*==============================================================*/
/*			Sets the speeds from a table, mirrored when			*/
/*			facing the other way								*/
/*==============================================================*/
static void spd_set( PLAYER_WORK *pl_w, float *spd )
{
	pl_w->spd.x = *spd++ ;
	pl_w->acc.x = *spd++ ;
	pl_w->spd.y = *spd++ ;
	pl_w->acc.y = *spd ;
	if( pl_w->disp_dir ){
		pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = -pl_w->acc.x ;
	}
	pl_w->pl_stance = STANCE_AIR ;
}

/*--------------------------------------------------------------*/
static void kick_rise( PLAYER_WORK *pl_w )
{
	static float kick_fall_spd[8] = {
		-DOT_X( 11.0f ), DOT_X( 0.25f ), DOT_Y( 1.0f ), -0.13392857f,
		-DOT_X( 14.0f ), DOT_X( 0.25f ), DOT_Y( 2.0f ), -0.20089285f,
	} ;
	int i, no ;

	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		no = !pl_w->atck_level ? 84 : 86 ;
		pl_w->atck_no = no ;
		HIT_RESET( pl_w ) ;
		i = pl_w->atck_level * 4 ;
		spd_set( pl_w, &kick_fall_spd[i] ) ;
	}
}

/*--------------------------------------------------------------*/
static void kick_fall( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_char_set( pl_w, Char_Jump, 10 ) ;
	} else {
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
}

/*--------------------------------------------------------------*/
static void kick_land( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, ( pl_w->atck_level << 1 ) + 35 ) ;
	}
}

static void snapback_init( PLAYER_WORK *pl_w );
static void snapback_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_snapback( PLAYER_WORK *pl_w )
{
	static void ( *snapback_jmp[])( PLAYER_WORK *pl_w ) = {
		snapback_init, snapback_move,
	} ;

	snapback_jmp[pl_w->routine_2]( pl_w ) ;
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
	pl_w->atck_no = pl38_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 38 ) ;
}

/*--------------------------------------------------------------*/
static void snapback_move( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;
	Float dx ;

	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	else {
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			ofs.x = 0.0f ;
			ofs.y = DOT_Y( 80.0f ) ;
			pl_snap_flash( pl_w, &ofs ) ;
		}
		if( pl_w->cg_prm ){
			pl_w->cg_prm = 0 ;
			dx = -DOT_X( 32.0f ) ;
			if( pl_w->disp_dir ) dx = DOT_X( 32.0f ) ;
			pl_w->pos.x += dx ;
		}
	}
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_move( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A kick that leaps off the wall						*/
/****************************************************************/
void pl38_t_kick( PLAYER_WORK *pl_w )
{
	static void ( *t_kick_jmp[])( PLAYER_WORK *pl_w ) = {
		t_kick_init, t_kick_move, t_kick_land,
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
	pl_w->spd.y = -DOT_Y( 0.125f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->atck_no = pl38_at_t_kick ;
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

/****************************************************************/
/*			The throws: on the ground one of three punch		*/
/*			throws at random or a kick throw, one in the air	*/
/****************************************************************/
PLAYER_WORK *pl38_catch_ck( PLAYER_WORK *pl_w )
{
	static Uint16 catch_no_tbl[4] = { 0, 4, 5, 5 } ;
	PLAYER_WORK *em ;
	int stance ;

	stance = pl_w->pl_stance ;
	if( ( !stance || stance == 2 )
	 && ( PL38_LW( pl_w )->catch_lever = pl_w->atck_lever ) & 0x0c00 && pl_w->atck_level ){
		if( !( em = pl_catch_ck( pl_w ) ) ) return 0 ;
		if( pl_w->atck_pk == 1 ){
			if( pl_w->pl_stance == STANCE_AIR ) em = 0 ;
			else pl_w->catch_no = 2 ;
		} else if( pl_w->pl_stance != STANCE_AIR ){
			if( !PL38_LW( pl_w )->shot_timer ){
				PL38_LW( pl_w )->catch_sel = Rnd() & 3 ;
				pl_w->catch_no = catch_no_tbl[PL38_LW( pl_w )->catch_sel] ;
			} else pl_w->catch_no = 2 ;
		} else pl_w->catch_no = 1 ;
	} else em = 0 ;
	return em ;
}

/****************************************************************/
/*			Start the throw that pl38_catch_ck chose			*/
/****************************************************************/
void pl38_catch_set( PLAYER_WORK *pl_w )
{
	static Uint16 catch_char_tbl[4] = { 0, 8, 9, 9 } ;
	static int catch_voice_tbl[4] = { 8, 7, 9, 9 } ;
	static Uint16 catch_eff_tbl[4] = { 0, 16, 17, 17 } ;
	PLAYER_WORK *em ;
	VEC3 ofs ;
	Uint16 eff ;
	int voice ;

	em = pl_w->catch_w ;
	catch_gauge_add( pl_w ) ;
	em->pl_dir = pl_w->pl_dir ^ 1 ;
	em->disp_dir = pl_w->pl_dir ;
	pl_w->hit_stop = 10 ;
	switch( pl_w->catch_no ){
	case 0:
	case 3:
	case 4:
	case 5:
		if( PL38_LW( pl_w )->catch_lever & 0x0400 ){
			pl_w->pl_dir ^= 1 ;
			pl_w->disp_dir = pl_w->pl_dir ;
			em->pl_dir ^= 1 ;
			em->disp_dir = em->pl_dir ;
		}
		ofs.x = -95.0f ;
		ofs.y = DOT_Y( 64.0f ) ;
		nage_hit_set( pl_w, &ofs ) ;
		pl_char_set( pl_w, Char_Catch, catch_char_tbl[PL38_LW( pl_w )->catch_sel] ) ;
		if( eff = catch_eff_tbl[PL38_LW( pl_w )->catch_sel] ) Shl38_set( pl_w, eff, 0 ) ;
		Shl38_set( pl_w, 15, 0 ) ;
		voice = catch_voice_tbl[PL38_LW( pl_w )->catch_sel] ;
		pl_voice_req( pl_w, voice ) ;
		break ;
	case 1:
		scr_excep_set( pl_w, 5, 5 ) ;
		if( !( PL38_LW( pl_w )->catch_lever & 0x0400 ) ){
			pl_w->pl_dir ^= 1 ;
			pl_w->disp_dir = pl_w->pl_dir ;
			em->pl_dir ^= 1 ;
			em->disp_dir = em->pl_dir ;
		}
		ofs.x = -95.0f ;
		ofs.y = DOT_Y( 64.0f ) ;
		nage_hit_set( pl_w, &ofs ) ;
		pl_char_set( pl_w, Char_Catch, 2 ) ;
		break ;
	case 2:
		if( !( PL38_LW( pl_w )->catch_lever & 0x0800 ) ){
			pl_w->pl_dir ^= 1 ;
			pl_w->disp_dir = pl_w->pl_dir ;
			em->pl_dir ^= 1 ;
			em->disp_dir = em->pl_dir ;
		}
		em->pl_dir = pl_w->pl_dir ^ 1 ;
		ofs.x = -95.0f ;
		ofs.y = DOT_Y( 64.0f ) ;
		nage_hit_set( pl_w, &ofs ) ;
		pl_char_set( pl_w, Char_Catch, 3 ) ;
		Shl38_set( pl_w, 3, 0 ) ;
		pl_w->timer0 = 8 ;
		hold_renda_set( pl_w->catch_w, 7 ) ;
		catch_renda_set( pl_w, 4 ) ;
		break ;
	}
}

static void catch_0( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void jump_p_catch( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void nml_k_catch( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void catch_4( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void catch_5( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void catch0_grab( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void catch0_toss( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void catch0_end( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void catch5_grab( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void catch5_toss( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void catch5_end( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void nml_k_catch_grab( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void nml_k_catch_hold( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void nml_k_catch_end( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void catch_eff_set( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void catch_ret( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void pl38_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w, PLAYER_WORK *em ) = {
		catch_0, jump_p_catch, nml_k_catch, catch_0, catch_4, catch_5,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no]( pl_w, pl_w->catch_w ) ;
}

/*==============================================================*/
static void catch_0( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	static void ( *catch0_jmp[])( PLAYER_WORK *pl_w, PLAYER_WORK *em ) = {
		catch0_grab, catch0_toss, catch0_end,
	} ;

	catch0_jmp[pl_w->routine_2]( pl_w, em ) ;
}

/*==============================================================*/
static void catch0_grab( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	VEC3 pos ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_flag1 ){
		pl_w->cg_flag1 = 0 ;
		pl_w->routine_2 ++ ;
		em->dm_hit_w = pl_w ;
		em->atck_no = pl38_at_throw_p ;
		em->catch_type = 18 ;
		pos.x = 90.0f ;
		if( pl_w->disp_dir ) ;	/* the facing is tested, but the offset is the same */
		pos.x += pl_w->pos.x ;
		pos.y = pl_w->pos.y + 285.0f ;
		hitmark_set( &pos, pl_w->pl_dir, 2, 0 ) ;
		hit_se_req( pl_w->catch_w, 0 ) ;
	}
}

/*==============================================================*/
/*			Once the opponent is high enough, throws him		*/
/*			down												*/
/*==============================================================*/
static void catch0_toss( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	if( em->routine_1 == PL_CAUGHT && em->dm_hit_w->pl_type == PL_CAPTAIN_COMMANDO && em->catch_type == 18 ){
		if( em->pos.y > DOT_Y( 160.0f ) ){
			pl_w->routine_2 ++ ;
			em->dm_hit_w = pl_w ;
			em->atck_no = pl38_at_throw_p_toss ;
			em->catch_type = 19 ;
			em->catch_no = 3 ;
			em->routine_2 = em->routine_3 = 0 ;
			em->hit_stop = 10 ;
			Shl38_set( pl_w, 18, 0 ) ;
		}
	} else catch_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void catch0_end( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	if( pl_char_move( pl_w ) < 0 ) catch_ret( pl_w ) ;
}

/*==============================================================*/
static void catch_4( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	} else if( pl_w->cg_flag1 ){
		pl_w->cg_flag1 = 0 ;
		em->dm_hit_w = pl_w ;
		em->atck_no = pl38_at_throw_p ;
		em->catch_type = 1 ;
		catch_eff_set( pl_w, em ) ;
	}
}

/*==============================================================*/
static void catch_eff_set( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	VEC3 pos ;
	Float dx ;

	dx = -90.0f ;
	if( pl_w->disp_dir ) dx = 90.0f ;
	pos.x = dx + em->pos.x ;
	pos.y = em->pos.y + 285.0f ;
	hitmark_set( &pos, pl_w->pl_dir, 2, 0 ) ;
	hit_se_req( pl_w->catch_w, 0 ) ;
}

/*==============================================================*/
static void catch_5( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	static void ( *catch5_jmp[])( PLAYER_WORK *pl_w, PLAYER_WORK *em ) = {
		catch5_grab, catch5_toss, catch5_end,
	} ;

	catch5_jmp[pl_w->routine_2]( pl_w, em ) ;
}

/*==============================================================*/
static void catch5_grab( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_flag1 ){
		pl_w->cg_flag1 = 0 ;
		pl_w->routine_2 ++ ;
		em->dm_hit_w = pl_w ;
		em->atck_no = pl38_at_throw_p ;
		em->catch_type = 18 ;
		catch_eff_set( pl_w, em ) ;
	}
}

/*--------------------------------------------------------------*/
static void catch5_toss( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	VEC3 pos ;

	if( em->routine_1 == PL_CAUGHT && em->dm_hit_w->pl_type == PL_CAPTAIN_COMMANDO && em->catch_type == 18
	 && em->pos.y > DOT_Y( 192.0f ) ){
		pl_w->routine_2 ++ ;
		em->dm_hit_w = pl_w ;
		em->atck_no = pl38_at_throw_p_toss2 ;
		em->catch_type = 9 ;
		em->catch_no = 3 ;
		em->routine_2 = em->routine_3 = 0 ;
		em->hit_stop = 10 ;
		pl_se_req( pl_w, 73 ) ;
		pos.x = em->pos.x ;
		pos.y = em->pos.y + DOT_Y( 16.0f ) ;
		burst_set( em, &pos, 1, 0xf9 ) ;
	}
}

/*--------------------------------------------------------------*/
static void catch5_end( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void jump_p_catch( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	if( !pl_w->routine_2 ){
		pl_char_move( pl_w ) ;
		if( pl_w->cg_flag1 ){
			pl_w->cg_flag1 = 0 ;
			pl_w->routine_2 ++ ;
			em->dm_hit_w = pl_w ;
			em->atck_no = pl38_at_air_throw ;
			em->catch_type = 1 ;
			em->disp_dir ^= 1 ;
		}
	} else if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		catch_ret( pl_w ) ;
	}
}

/*==============================================================*/
static void nml_k_catch( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	static void ( *nml_k_catch_jmp[])( PLAYER_WORK *pl_w, PLAYER_WORK *em ) = {
		nml_k_catch_grab, nml_k_catch_hold, nml_k_catch_end,
	} ;

	nml_k_catch_jmp[pl_w->routine_2]( pl_w, em ) ;
}

/*==============================================================*/
static void nml_k_catch_grab( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		em->dm_elem = 3 ;
		em->unk_228 = 3 ;
		dm00_04_set( em ) ;
	}
}

/*==============================================================*/
/*			Hits the opponent until the timer runs out			*/
/*==============================================================*/
static void nml_k_catch_hold( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	Sint8 no ;

	if( catch_renda_ck( pl_w ) ) pl_w->timer0 ++ ;
	if( hold_renda_ck( pl_w->catch_w ) ) pl_w->timer0 = 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_flag1 ){
		pl_w->cg_flag1 = 0 ;
		vital_add_dmg2( pl_w->catch_w, -1 ) ;
		if( -- pl_w->timer0 <= 0 ){
			pl_w->routine_2 ++ ;
			SCR_EXCEP_SET ;
			pl_w->atck_no = pl38_at_throw_k ;
			pl_w->hit_flag = pl_w->atck_attr = 0 ;
			pl_w->unk_1c4 = 0 ;
			vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
			pl_char_set( pl_w, Char_Catch, 4 ) ;
			pl_color_set( pl_w, PC_DEFAULT ) ;
			return ;
		}
	}
	no = pl_w->cg_type ;
	if( no ){
		pl_w->cg_type = 0 ;
		if( !( no -= 4 ) ) pl_color_set( pl_w, PC_DEFAULT ) ;
		else ef_color_set( pl_w, 0, 4, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void nml_k_catch_end( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	VEC3 pos ;
	Float dx ;

	if( pl_char_move( pl_w ) < 0 ) catch_ret( pl_w ) ;
	else {
		if( pl_w->cg_prm ){
			pl_w->cg_prm = 0 ;
			em->dm_hit_w = pl_w ;
			em->atck_no = pl38_at_throw_k ;
			em->catch_type = 10 ;
		}
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			dx = -85.0f ;
			if( pl_w->disp_dir ) dx = 85.0f ;
			pos.x = dx + pl_w->pos.x ;
			pos.y = pl_w->pos.y + 240.0f ;
			hitmark_set( &pos, pl_w->pl_dir, 2, 0 ) ;
		}
	}
}

/*==============================================================*/
static void catch_ret( PLAYER_WORK *pl_w )
{
	if( pl_w->pl_stance == STANCE_AIR ) pl_jump_ret( pl_w ) ;
	else pl_stand_ret( pl_w ) ;
}

/****************************************************************/
/*			Holding the opponent								*/
/****************************************************************/
void pl38_hold( PLAYER_WORK *pl_w )
{
	catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void pl38_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl38_sp_sword ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl38_sp_sword ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl38_sp_sword ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl38_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl38_sp_sword ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl38_sp_sword ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl38_sp_sword ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl38_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl38_sp_fire ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl38_sp_corridor ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl38_sp_kick ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void pl38_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl38_sp_fire ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl38_sp_corridor ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl38_sp_kick ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
/*			The next animation, one of four at random,			*/
/*			chosen by what the animation set					*/
/****************************************************************/
void pl38_char_rnd_set( PLAYER_WORK *pl_w )
{
	static Uint8 char_rnd_tbl[3][4] = {
		{ 13, 13, 14, 15 }, { 16, 16, 17, 18 }, { 19, 19, 20, 21 },
	} ;

	if( pl_w->cg_type ){
		pl_char_set( pl_w, Char_Stand, char_rnd_tbl[pl_w->cg_type - 1][Rnd() & 3] ) ;
	}
}

/****************************************************************/
void pl38_char_rnd_set_2( PLAYER_WORK *pl_w )
{
	static Uint8 char_rnd_tbl_2[2][4] = {
		{ 22, 22, 23, 24 }, { 25, 25, 26, 27 },
	} ;
	Sint8 no ;

	no = pl_w->jump_dir ;
	if( no < 0 ) no = 0 ;
	pl_char_set( pl_w, Char_Stand, char_rnd_tbl_2[no][Rnd() & 3] ) ;
}

/****************************************************************/
void pl38_char_rnd_set_3( PLAYER_WORK *pl_w )
{
	static Uint8 char_rnd_tbl_3[2][4] = {
		{ 16, 16, 17, 18 }, { 19, 19, 20, 21 },
	} ;
	Sint8 no ;

	no = pl_w->cg_type ;
	if( no ) no = 1 ;
	pl_char_set( pl_w, Char_Stand, char_rnd_tbl_3[no][Rnd() & 3] ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl38.c" End of File	*/

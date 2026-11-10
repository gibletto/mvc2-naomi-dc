/* Magneto (character 2c): command checks, normal attacks, flight, appearance, win poses, special
   moves and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl2c_def.h"

/* declared with int arguments, as its calls here are built; pls2c_01.c defines them Uint8 */
extern PLAYER_WORK *pls2c_01_set2( PLAYER_WORK *pl_w, int type, int kind ) ;

void pl2c_main( PLAYER_WORK *pl_w );
void pl2c_com_ck( PLAYER_WORK *pl_w );
void pl2c_atck_set( PLAYER_WORK *pl_w );
void pl2c_atck( PLAYER_WORK *pl_w );
void pl2c_dash( PLAYER_WORK *pl_w );
void pl2c_noki( PLAYER_WORK *pl_w );
void pl2c_appear( PLAYER_WORK *pl_w );
void pl2c_win( PLAYER_WORK *pl_w );
void pl2c_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl2c_catch_ck( PLAYER_WORK *pl_w );
void pl2c_catch_set( PLAYER_WORK *pl_w, PLAYER_WORK *em_w );
void pl2c_catch( PLAYER_WORK *pl_w );
void pl2c_hold( PLAYER_WORK *pl_w );
int pl2c_delay_ck( PLAYER_WORK *pl_w );
void pl2c_t_kick( PLAYER_WORK *pl_w );
void pl2c_upper( PLAYER_WORK *pl_w );
void pl2c_sp_unison( PLAYER_WORK *pl_w );
void pl2c_sp_gattai( PLAYER_WORK *pl_w );
void pl2c_sp_assist( PLAYER_WORK *pl_w );
void pl2c_sp_zero( PLAYER_WORK *pl_w );

/* each attack's table: four bytes for each button (punch, kick) and strength */
static Sint8 nml_tbl[24] = {
	  1,  2,  1,  2, -1, -1, -1, -1, -1, -1, -1, -1,
	  1,  2,  1,  2, -1, -1, -1, -1, -1, -1, -1, -1,
} ;
static Sint8 jump_tbl[24] = {
	  1,  2,  0,  2, -1,  2,  1,  2, -1, -1, -1,  2,
	  1,  2,  1,  2, -1,  2, -1,  2, -1, -1, -1, -1,
} ;

/* commands: time or steps, kind, the special move, then the stick and buttons */
static Uint16 com_disrupt[] = { 5, 0, pl2c_sp_disrupt, 0, KEY_BACK, KEY_DOWN | KEY_BACK, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* EM Disruptor */
static Uint16 com_grav[] = { 5, 0, pl2c_sp_grav, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Hyper Grav */
static Uint16 com_shock[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl2c_sp_shock, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Magnetic Shockwave */
static Uint16 com_tempest[] = { 3, COM_HYPER | COM_ALL_SW, pl2c_sp_tempest, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Magnetic Tempest */
static Uint16 com_fly[] = { 3, COM_ALL_SW, pl2c_sp_fly, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Flight */
static Uint16 com_field[] = { 5, COM_GROUND, pl2c_sp_field, 0, KEY_BACK, KEY_DOWN | KEY_BACK, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Magnetic Forcefield */
static Uint16 com_jblast[] = { 3, COM_AIR, pl2c_sp_jblast, 0, KEY_UP, KEY_UP | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Magnetic Blast */

/* the throws, by the stick: the first one's attack, then the second one's attack and animation */
static Uint16 catch_tbl0[16] = {
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x21, 0x20, 0x21, 0x20, 0x21, 0x20, 0x21, 0x20,
} ;
static Uint16 catch_tbl1[16] = {
	0x23, 0x23, 0x23, 0x23, 0x24, 0x24, 0x24, 0x24, 0x22, 0x23, 0x22, 0x23, 0x22, 0x23, 0x22, 0x23,
} ;
static Uint16 catch_tbl2[16] = {
	0x05, 0x05, 0x05, 0x05, 0x07, 0x07, 0x07, 0x07, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
} ;

static void ( *pl2c_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl2c_main,
	pl2c_com_ck,
	pl2c_atck_set,
	pl2c_atck,
	pl2c_dash,
	pl2c_noki,
	pl2c_appear,
	pl2c_win,
	pl2c_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl2c_catch_ck,	/* returns a value */
	( void ( * )( PLAYER_WORK *pl_w ) )pl2c_catch_set,	/* takes the opponent too */
	pl2c_catch,
	pl2c_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl2c_delay_ck,
	pl2c_t_kick,
	pl2c_upper,
	pl2c_sp_unison,
	pl2c_sp_gattai,
	pl2c_sp_assist,
	pl2c_sp_zero,
} ;

static int snapback_ck( PLAYER_WORK *pl_w );
static Uint8 shock_ck( PLAYER_WORK *pl_w );
static Uint8 tempest_ck( PLAYER_WORK *pl_w );
static Uint8 disrupt_ck( PLAYER_WORK *pl_w );
static Uint8 grav_ck( PLAYER_WORK *pl_w );
static Uint8 fly_ck( PLAYER_WORK *pl_w );
static Uint8 field_ck( PLAYER_WORK *pl_w );
static Uint8 jblast_ck( PLAYER_WORK *pl_w );
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w );
static void fly_end( PLAYER_WORK *pl_w );

/****************************************************************/
void pl2c_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl2c_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl2c_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( shock_ck( pl_w ) ) return ;
	if( tempest_ck( pl_w ) ) return ;
	if( jblast_ck( pl_w ) ) return ;
	if( field_ck( pl_w ) ) return ;
	if( disrupt_ck( pl_w ) ) return ;
	if( grav_ck( pl_w ) ) return ;
	if( fly_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	if( pl_air_dash_sw_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
static int snapback_ck( PLAYER_WORK *pl_w )
{
	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl2c_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 shock_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_shock, &pl_w->com_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w ) ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl2c_sp_shock ;
	pl_w->atck_type = 0 ;
	return 1 ;
}

/*==============================================================*/
/*			In the air, once a jump unless super jumping		*/
/*==============================================================*/
static Uint8 tempest_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_tempest, &pl_w->com_w0 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl2c_sp_tempest ;
	pl_w->atck_type = 0 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 disrupt_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_disrupt, &pl_w->com_w1 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl2c_sp_disrupt ;
	pl_w->atck_type = 0 ;
	return 1 ;
}

/*==============================================================*/
/*			Not while one is still out							*/
/*==============================================================*/
static Uint8 grav_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_grav, &pl_w->com_w2 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	if( PL2C_LW( pl_w )->magnet_flag ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl2c_sp_grav ;
	pl_w->atck_type = 0 ;
	return 1 ;
}

/*==============================================================*/
/*			Flight												*/
/*==============================================================*/
static Uint8 fly_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_fly, &pl_w->com_w3 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w3 ) ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl2c_sp_fly ;
	pl_w->atck_type = 0 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 field_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_field, &pl_w->com_w5 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w5 ) ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl2c_sp_field ;
	pl_w->atck_type = 0 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 jblast_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_jblast, &pl_w->com_w4 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w4 ) ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl2c_sp_jblast ;
	pl_w->atck_type = 0 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 6 ) ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl2c_sp_chouhatsu ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl2c_delay_ck( PLAYER_WORK *pl_w )
{
	if( com_nml_ck( pl_w, com_shock, &pl_w->com_w ) && *pl_w->gauge_level ){
		pl_w->delay_sp_type = pl2c_sp_shock ;
		return 1 ;
	}
	if( com_nml_ck( pl_w, com_tempest, &pl_w->com_w0 ) && *pl_w->gauge_level ){
		pl_w->delay_sp_type = pl2c_sp_tempest ;
		return 1 ;
	}
	return 0 ;
}

/****************************************************************/
/*			Counts down the flight								*/
/****************************************************************/
void pl2c_main( PLAYER_WORK *pl_w )
{
	if( PL2C_LW( pl_w )->catch_ng_flag ){
		pl_w->catch_ng_timer = 2 ;
		if( -- PL2C_LW( pl_w )->catch_ng_timer < 0 ){
			pl_w->catch_ng_timer = PL2C_LW( pl_w )->catch_ng_flag = 0 ;
			pl_w->dm_flag = HIT_PL ;
		}
	}
	if( pl_w->fly_flag && pl_w->routine_1 == PL_NORM && !-- PL2C_LW( pl_w )->fly_timer ){
		if( pl_w->act_st == N_SPECIAL || pl_w->act_st == N_SUPER ) PL2C_LW( pl_w )->fly_timer = 1 ;
		else fly_end( pl_w ) ;
	}
}

/*==============================================================*/
/*			Back to neutral after a move, in the air or on		*/
/*			the ground											*/
/*==============================================================*/
static void sp_ret( PLAYER_WORK *pl_w, Uint16 stance )
{
	if( stance == 2 ){
		if( pl_w->fly_flag ){
			pl_w->spd.x = pl_w->acc.x = pl_w->spd.y = 0.0f ;
			pl_w->acc.y = -DOT_Y( 0.375f ) ;
			pl_stand_ret( pl_w ) ;
		} else {
			if( !pl_w->spd.y ){
				pl_w->spd.x = pl_w->acc.x = pl_w->spd.y = 0.0f ;
				pl_w->acc.y = -DOT_Y( 0.375f ) ;
			}
			pl_jump_ret( pl_w ) ;
		}
	} else pl_stand_ret( pl_w ) ;
}

/* the sound for each attack */
static Uint8 nml_voice_tbl[12] = { 0, 0, 0, 20, 21, 22, 20, 0, 0, 20, 21, 22 } ;

/*==============================================================*/
/*			Normal attacks										*/
/*==============================================================*/
static void nml_atck( PLAYER_WORK *pl_w )
{
	Uint8 no ;
	Uint8 voice ;

	pl_brake_set( pl_w ) ;
	pl_w->chain_tbl = &nml_tbl[pl_w->atck_pk * 12 + pl_w->nml_no * 4] ;
	pl_w->nml_level = pl_w->nml_no ;
	no = pl_w->pl_stance * 6 + pl_w->atck_pk * 3 + pl_w->nml_no ;
	pl_w->atck_no = no ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, ( pl_w->pl_stance << 1 ) + pl_w->atck_pk + Char_Stand_P, pl_w->nml_no ) ;
	if( voice = nml_voice_tbl[no] ) pl_se_req( pl_w, voice ) ;
}

/*==============================================================*/
/*			In the air; the light kick with the stick down		*/
/*			is a dive											*/
/*==============================================================*/
static void jump_atck( PLAYER_WORK *pl_w )
{
	if( ( pl_w->atck_lever & KEY_DOWN ) && !pl_w->nml_no && pl_w->atck_pk ){
		pl_w->atck_no = pl2c_at_jdown_lk ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Jump_K, 3 ) ;
	} else {
		pl_w->atck_no = pl_w->atck_pk * 3 + pl_w->nml_no + 12 ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, pl_w->atck_pk + Char_Jump_P, pl_w->nml_no ) ;
	}
	pl_w->chain_tbl = &jump_tbl[pl_w->atck_pk * 12 + pl_w->nml_no * 4] ;
	pl_w->nml_level = pl_w->nml_no ;
	if( pl_w->atck_pk ) pl_se_req( pl_w, pl_w->nml_no + 20 ) ;
	if( !pl_w->atck_pk ){
		if( pl_w->jump_atk_cnt & JATK_P_MASK ) pl_w->jump_atk_cnt -= 0x01 ;
	} else {
		if( pl_w->jump_atk_cnt & JATK_K_MASK ) pl_w->jump_atk_cnt -= 0x10 ;
	}
}

/*==============================================================*/
/*			In a jump only while that button's attacks are		*/
/*			left, but freely while flying						*/
/*==============================================================*/
static void jump_atck_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->fly_flag ) jump_atck( pl_w ) ;
	else if( !pl_w->atck_pk ){
		if( pl_w->jump_atk_cnt & JATK_P_MASK ) jump_atck( pl_w ) ;
	} else if( pl_w->jump_atk_cnt & JATK_K_MASK ) jump_atck( pl_w ) ;
}

/****************************************************************/
void pl2c_atck_set( PLAYER_WORK *pl_w )
{
	static void ( *atck_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_atck, jump_atck_ck, jump_atck, nml_atck,
	} ;

	atck_set_jmp[pl_w->atck_type]( pl_w ) ;
}

/*==============================================================*/
/*			The medium punch fires a shot						*/
/*==============================================================*/
static void punch_move( PLAYER_WORK *pl_w )
{
	if( pl_w->nml_no == 1 ){
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		else if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			shl2c_00_set( pl_w, 1 ) ;
		}
	} else {
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
/*			The heavy kicks: crouching he slides, standing		*/
/*			it hits twice										*/
/*==============================================================*/
static void kick_move( PLAYER_WORK *pl_w )
{
	if( pl_w->nml_no == 2 ){
		if( pl_w->pl_stance == STANCE_CROUCH ){
			if( !pl_w->routine_2 ){
				pl_char_move( pl_w ) ;
				if( pl_w->cg_type ){
					pl_w->cg_type = 0 ;
					pl_w->routine_2 ++ ;
					pl_w->timer1 = 4 ;
					pl_w->spd.x = DOT_X( 5.0f ) ;
					pl_w->acc.x = -DOT_X( 0.125f ) ;
					if( !pl_w->disp_dir ){
						pl_w->spd.x = -pl_w->spd.x ;
						pl_w->acc.x = -pl_w->acc.x ;
					}
				}
			} else if( pl_w->routine_2 == 1 ){
				if( -- pl_w->timer1 < 0 ){
					pl_w->timer1 = 6 ;
					pls2c_01_set( pl_w, 0 ) ;
				}
				pl_char_move( pl_w ) ;
				pl_w->pos.x += pl_w->spd.x ;
				pl_w->spd.x += pl_w->acc.x ;
				if( pl_w->spd.x * pl_w->acc.x >= 0.0f ){
					pl_w->routine_2 ++ ;
					pl_char_set( pl_w, Char_Crouch_K, 3 ) ;
				}
			} else {
				if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
			}
		} else {
			if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
			else if( pl_w->cg_type ){
				pl_w->cg_type = 0 ;
				pl_se_req( pl_w, SE_SWING_H ) ;
				pl_w->atck_no = pl2c_at_stand_hk_2 ;
				HIT_RESET( pl_w ) ;
			}
		}
	} else {
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	}
}

/****************************************************************/
void pl2c_atck( PLAYER_WORK *pl_w )
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
		if( !pl_w->atck_pk ) punch_move( pl_w ) ;
		else kick_move( pl_w ) ;
		break ;
	case 2:
		if( !pl_w->fly_flag ){
			pl_jump_hit_ck( pl_w ) ;
			pl_sjump_ctrl( pl_w ) ;
		}
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
void pl2c_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_w->atck_no = pl2c_at_upper ;
		HIT_RESET( pl_w ) ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_gauge_add( pl_w, 5 ) ;
		pl_char_set( pl_w, Char_Extra, 7 ) ;
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
static void dash_end( PLAYER_WORK *pl_w );

/****************************************************************/
void pl2c_dash( PLAYER_WORK *pl_w )
{
	static void ( *dash_jmp[])( PLAYER_WORK *pl_w ) = {
		dash_init, dash_move, dash_end,
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
		pl_w->spd.x = pl_w->pl_dir ? 20.0f : -20.0f ;
		pl_w->timer1 = 4 ;
		pl_w->timer0 = 24 ;
	}
}

/*--------------------------------------------------------------*/
static void dash_move( PLAYER_WORK *pl_w )
{
	if( -- pl_w->timer1 < 0 ){
		pl_w->timer1 = 4 ;
		pls2c_01_set( pl_w, 0 ) ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Dash, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void dash_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void noki_init( PLAYER_WORK *pl_w );
static void noki_move( PLAYER_WORK *pl_w );
static void noki_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash										*/
/****************************************************************/
void pl2c_noki( PLAYER_WORK *pl_w )
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
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 13.0f ) : DOT_X( 13.0f ) ;
		pl_w->timer1 = 4 ;
		pl_w->timer0 = 24 ;
	}
}

/*--------------------------------------------------------------*/
static void noki_move( PLAYER_WORK *pl_w )
{
	if( -- pl_w->timer1 < 0 ){
		pl_w->timer1 = 4 ;
		pls2c_01_set( pl_w, 1 ) ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Dash, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void noki_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/****************************************************************/
void pl2c_appear( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->disp_flag = 1 ;
		pl_char_set( pl_w, Char_Stand, 0 ) ;
	}
	if( pl_w->routine_2 == 1 ){
		if( vs_sys_work.vs_st >= VS_APPEAR ){
			pl_w->routine_2 ++ ;
			pl_char_set( pl_w, Char_Appear, 0 ) ;
		}
	}
	if( pl_w->routine_2 == 2 && pl_char_move( pl_w ) < 0 ) pl_w->routine_1 ++ ;
}

/****************************************************************/
void pl2c_win( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		switch( pl_w->type ){
		case 0:
			if( Rnd() & 1 ) pl_char_set( pl_w, Char_Win, 0 ) ;
			else pl_char_set( pl_w, Char_Win, 1 ) ;
			break ;
		case 1:
		case 3:
		case 4:
			pl_char_set( pl_w, Char_Win, 3 ) ;
			break ;
		case 2:
			pl_char_set( pl_w, Char_Win, 2 ) ;
			break ;
		}
	}
	if( pl_win_end_ck( pl_w ) ) pl_stand_ret( pl_w ) ;
	else pl_char_move( pl_w ) ;
}

static void sp_disrupt( PLAYER_WORK *pl_w );
static void sp_grav( PLAYER_WORK *pl_w );
static void sp_shock( PLAYER_WORK *pl_w );
static void sp_tempest( PLAYER_WORK *pl_w );
static void sp_fly( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_jblast( PLAYER_WORK *pl_w );
static void sp_field( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );
static void sp_crouch_hp( PLAYER_WORK *pl_w );

/****************************************************************/
void pl2c_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *pl_sp_tbl[])( PLAYER_WORK *pl_w ) = {
		sp_disrupt, sp_grav, sp_shock, sp_tempest, sp_fly, pl_stand_ret, sp_chouhatsu, sp_jblast,
		sp_field, pl_stand_ret, pl_stand_ret, pl_stand_ret, sp_snapback, sp_crouch_hp,
	} ;

	pl_sp_tbl[pl_w->pl_sp_type]( pl_w ) ;
}

/*==============================================================*/
/*			A shot, on the ground or in the air					*/
/*==============================================================*/
static void sp_disrupt( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_gauge_add( pl_w, 5 ) ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
		if( pl_w->pl_stance != STANCE_AIR ){
			pl_sp_ef_set( pl_w ) ;
			pl_w->pl_stance = STANCE_STAND ;
		}
		pl_dir_set( pl_w ) ;
		pl_w->atck_no = ( pl_w->atck_level << 1 ) + 48 ;
		HIT_RESET( pl_w ) ;
		if( pl_w->atck_level ) pl_char_set( pl_w, Char_Special, 0 ) ;
		else pl_char_set( pl_w, Char_Special, 16 ) ;
		shl2c_00_set( pl_w, 2 ) ;
		pls2c_01_set2( pl_w, 5, 0 ) ;
		pls2c_01_set2( pl_w, 5, 1 ) ;
		pls2c_01_set2( pl_w, 5, 2 ) ;
		pl_w->spd.x /= 8.0f ;
		pl_w->acc.x /= 8.0f ;
		pl_w->spd.y /= 8.0f ;
		pl_w->acc.y /= 8.0f ;
		pl_w->routine_2 ++ ;
		pl_w->unk_27b = 0 ;
		pl_w->cancel_timer = 16 ;
	}
	if( pl_w->pl_stance == STANCE_AIR ){
		if( pl_w->ground_y <= pl_w->pos.y ){
			pl_w->pos.x += pl_w->spd.x ;
			pl_w->spd.x += pl_w->acc.x ;
			pl_w->pos.y += pl_w->spd.y ;
			pl_w->spd.y += pl_w->acc.y ;
		}
	} else pl_brake_stop( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ) sp_ret( pl_w, pl_w->pl_stance ) ;
}

static void grav_init( PLAYER_WORK *pl_w );
static void grav_move( PLAYER_WORK *pl_w );
static void grav_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_grav( PLAYER_WORK *pl_w )
{
	static void ( *grav_jmp[])( PLAYER_WORK *pl_w ) = {
		grav_init, grav_move, grav_end,
	} ;

	grav_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void grav_init( PLAYER_WORK *pl_w )
{
	PL2C_LOCAL_W *lw ;

	pl_w->atck_no = pl2c_at_grav ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 16 ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	lw = PL2C_LW( pl_w ) ;
	lw->hold_flag = lw->magnet_flag = 0 ;
	pl_char_set( pl_w, Char_Special, 1 ) ;
	if( pl_w->pl_stance == STANCE_AIR ){
		pl_w->spd.x /= 8.0f ;
		pl_w->acc.x /= 8.0f ;
		pl_w->spd.y /= 2.0f ;
		pl_w->acc.y /= 2.0f ;
	} else {
		pl_sp_ef_set( pl_w ) ;
		pl_w->pl_stance = STANCE_STAND ;
	}
	pl_dir_set( pl_w ) ;
	pl_w->routine_2 ++ ;
	grav_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void grav_move( PLAYER_WORK *pl_w )
{
	if( pl_w->pl_stance == STANCE_AIR ){
		pl_brake_stop( pl_w ) ;
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		pl_land_ck( pl_w ) ;
	}
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shl2c_03_set( pl_w ) ;
		pl_w->timer1 = 60 ;
		pl_w->routine_2 ++ ;
		pl_w->unk_27b = 0 ;
		pl_w->cancel_timer = 16 ;
	}
}

/*--------------------------------------------------------------*/
static void grav_end( PLAYER_WORK *pl_w )
{
	if( pl_w->pl_stance == STANCE_AIR ){
		pl_brake_stop( pl_w ) ;
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		pl_land_ck( pl_w ) ;
	}
	pl_char_move( pl_w ) ;
	if( PL2C_LW( pl_w )->hold_flag || -- pl_w->timer1 < 0 ) sp_ret( pl_w, pl_w->pl_stance ) ;
}

static void tempest_init( PLAYER_WORK *pl_w );
static void tempest_start( PLAYER_WORK *pl_w );
static void tempest_move( PLAYER_WORK *pl_w );
static void tempest_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo of shots, on the ground or in the		*/
/*			air													*/
/*==============================================================*/
static void sp_tempest( PLAYER_WORK *pl_w )
{
	static void ( *tempest_jmp[])( PLAYER_WORK *pl_w ) = {
		tempest_init, tempest_start, tempest_move, tempest_end,
	} ;

	tempest_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void tempest_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	if( pl_w->pl_stance != STANCE_AIR ){
		pl_sp_ef_set( pl_w ) ;
		pl_w->pl_stance = STANCE_STAND ;
	}
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->atck_no = pl2c_at_tempest ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 16 ) ;
}

/*--------------------------------------------------------------*/
static void tempest_start( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer1 = 96 ;
		pl_w->timer0 = 2 ;
		pl_char_set( pl_w, Char_Super, 17 ) ;
		pl_voice_req( pl_w, 24 ) ;
	} else if( pl_w->cg_type ){
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->cg_type = 0 ;
		ofs.x = -5.0f ;
		ofs.y = DOT_Y( 118.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void tempest_move( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( !-- pl_w->timer1 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Super, 18 ) ;
		return ;
	}
	if( !-- pl_w->timer0 ){
		pl_w->timer0 = 2 ;
		if( Rnd() & 0x7f ) shl2c_01_set( pl_w, 0 ) ;
		else shl2c_01_set( pl_w, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void tempest_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) sp_ret( pl_w, pl_w->pl_stance ) ;
}

static void shock_init( PLAYER_WORK *pl_w );
static void shock_start( PLAYER_WORK *pl_w );
static void shock_move( PLAYER_WORK *pl_w );
static void shock_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo on the ground							*/
/*==============================================================*/
static void sp_shock( PLAYER_WORK *pl_w )
{
	static void ( *shock_jmp[])( PLAYER_WORK *pl_w ) = {
		shock_init, shock_start, shock_move, shock_end,
	} ;

	shock_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void shock_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_sp_ef_set( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->atck_no = pl2c_at_shock ;
	HIT_RESET( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_char_set( pl_w, Char_Super, pl_w->pl_stance = STANCE_STAND ) ;
	pl_w->routine_2 ++ ;
	shock_start( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shock_start( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->timer0 = 24 ;
		pl_w->timer1 = 0 ;
		pl_char_set( pl_w, Char_Super, 1 ) ;
		pl_w->routine_2 ++ ;
		shl2c_02_set( pl_w, 0 ) ;
	} else if( pl_w->cg_type ){
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->cg_type = 0 ;
		ofs.x = -DOT_X( 25.0f ) ;
		ofs.y = DOT_Y( 101.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shock_move( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Super, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shock_end( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_stand_ret( pl_w ) ;
	}
}

static void fly_init( PLAYER_WORK *pl_w );
static void fly_move( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Flight, or the end of it when already flying		*/
/*==============================================================*/
static void sp_fly( PLAYER_WORK *pl_w )
{
	static void ( *fly_jmp[])( PLAYER_WORK *pl_w ) = {
		fly_init, fly_move,
	} ;

	fly_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void fly_init( PLAYER_WORK *pl_w )
{
	if( pl_w->fly_flag ){
		fly_end( pl_w ) ;
		return ;
	}
	pl_w->routine_2 ++ ;
	if( pl_w->pl_stance != STANCE_AIR ) pl_sp_ef_set( pl_w ) ;
	pl_w->fly_flag = 0xff ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->sjump_flag = 0 ;
	pl_w->spd.y = DOT_Y( 6.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.125f ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
	PL2C_LW( pl_w )->fly_timer = 600 ;
	pl_w->timer0 = 16 ;
	pl_char_set( pl_w, Char_Fly, 0 ) ;
	fly_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void fly_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			The flight ends: he falls							*/
/*==============================================================*/
static void fly_end( PLAYER_WORK *pl_w )
{
	pl_w->fly_flag = 0 ;
	pl_w->spd.x = pl_w->spd.y = pl_w->acc.x = 0.0f ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_jump_ret( pl_w ) ;
}

/*==============================================================*/
/*			A shot from the air									*/
/*==============================================================*/
static void sp_jblast( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_gauge_add( pl_w, 2 ) ;
		pl_dir_set( pl_w ) ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_w->atck_no = pl2c_at_jblast ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 18 ) ;
		pl_w->jump_atk_cnt = 0 ;
		pl_w->spd.x /= 8.0f ;
		pl_w->acc.x /= 8.0f ;
		pl_w->spd.y /= 8.0f ;
		pl_w->acc.y /= 8.0f ;
	}
	if( pl_w->pl_stance != STANCE_AIR ) pl_brake_stop( pl_w ) ;
	else if( pl_w->ground_y <= pl_w->pos.y ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
	if( pl_char_move( pl_w ) < 0 ) sp_ret( pl_w, 2 ) ;
	else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		PL2C_LW( pl_w )->shot_flag = 1 ;
		pl_w->unk_27b = 0 ;
		pl_w->cancel_timer = 16 ;
		shl2c_00_set( pl_w, 0 ) ;
	}
}

static void field_init( PLAYER_WORK *pl_w );
static void field_start( PLAYER_WORK *pl_w );
static void field_wait( PLAYER_WORK *pl_w );
static void field_end( PLAYER_WORK *pl_w );
static void field_catch( PLAYER_WORK *pl_w );
static void field_throw( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A grab: if it catches, he throws					*/
/*==============================================================*/
static void sp_field( PLAYER_WORK *pl_w )
{
	static void ( *field_jmp[])( PLAYER_WORK *pl_w ) = {
		field_init, field_start, field_wait, field_end, field_catch, field_throw,
	} ;

	field_jmp[pl_w->routine_2]( pl_w ) ;
	if( pl_w->cg_flag1 ){
		pl_w->atck_no = pl_w->cg_flag1 ;
		HIT_RESET( pl_w ) ;
		pl_w->cg_flag1 = 0 ;
	}
}

/*==============================================================*/
static void field_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_char_set( pl_w, Char_Special, 19 ) ;
}

/*--------------------------------------------------------------*/
static void field_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->dm_attr = ATR_COUNTER ;
		pl_w->timer0 = 16 ;
		if( !pl_w->atck_level ) pl_w->timer0 += 4 ;
	}
}

/*--------------------------------------------------------------*/
static void field_wait( PLAYER_WORK *pl_w )
{
	if( pl_w->dm_attr & ATR_COUNTER ){
		if( !-- pl_w->timer0 ){
			pl_w->routine_2 ++ ;
			pl_w->dm_attr = 0 ;
			pl_char_set( pl_w, Char_Special, 21 ) ;
		} else pl_char_move( pl_w ) ;
	} else {
		pl_w->routine_2 ++ ;
		pl_w->dm_attr = 0 ;
		if( pl_w->atck_level ){
			pl_w->routine_2 = 4 ;
			pl_w->catch_ng_timer = 2 ;
			if( pl_w->dm_hit_w ) pl_w->dm_hit_w->hit_stop = 16 ;
			pl_char_set( pl_w, Char_Super, 13 ) ;
			pl_sp_ef_set( pl_w ) ;
		} else pl_char_set( pl_w, Char_Special, 20 ) ;
	}
}

/*--------------------------------------------------------------*/
static void field_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void field_catch( PLAYER_WORK *pl_w )
{
	pl_w->catch_ng_timer = 2 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		shl2c_02_set2( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 14 ) ;
	}
}

/*--------------------------------------------------------------*/
static void field_throw( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 = 3 ;
		pl_w->timer0 = 20 ;
		pl_char_set( pl_w, Char_Super, 15 ) ;
	}
}

/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_char_set( pl_w, Char_Win, 1 ) ;
		pl_w->timer0 = 60 ;
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_crouch_hp( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->atck_no = pl2c_at_crouch_hp ;
		HIT_RESET( pl_w ) ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_dir_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 23 ) ;
	} else if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_jump( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The partner's jump in								*/
/****************************************************************/
void pl2c_t_kick( PLAYER_WORK *pl_w )
{
	static void ( *t_kick_jmp[])( PLAYER_WORK *pl_w ) = {
		t_kick_init, t_kick_jump, t_kick_land,
	} ;

	t_kick_jmp[pl_w->routine_2]( pl_w ) ;
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
	pl_w->atck_no = pl2c_at_t_kick ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 5 ) ;
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
		pl_char_set( pl_w, Char_Extra, 6 ) ;
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
	pl_w->atck_no = pl2c_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 22 ) ;
}

/*--------------------------------------------------------------*/
static void snapback_move( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		ofs.x = 0.0f ;
		ofs.y = DOT_Y( 80.0f ) ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
}

/****************************************************************/
/*			Can a throw start?									*/
/****************************************************************/
PLAYER_WORK *pl2c_catch_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( !PL2C_LW( pl_w )->magnet_flag && pl_w->pl_stance != STANCE_CROUCH && ( pl_w->atck_lever & ( KEY_FORWARD | KEY_BACK ) ) && pl_w->atck_level ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			if( !pl_w->atck_pk ) pl_w->catch_no = 0 ;
			else pl_w->catch_no = 1 ;
			PL2C_LW( pl_w )->catch_sw = pl_w->atck_lever ;
		}
		return em ;
	}
	return 0 ;
}

/****************************************************************/
/*			Start the throw that pl2c_catch_ck chose			*/
/****************************************************************/
void pl2c_catch_set( PLAYER_WORK *pl_w, PLAYER_WORK *em_w )
{
	VEC3 ofs ;
	Sint8 no ;

	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->hit_stop = 10 ;
	scr_excep_set( pl_w, 5, 5 ) ;
	if( PL2C_LW( pl_w )->catch_sw & 0x400 ){
		pl_w->disp_dir ^= 1 ;
		pl_w->pl_dir ^= 1 ;
	}
	em_w->pl_dir = em_w->disp_dir = pl_w->pl_dir ^ 1 ;
	if( !pl_w->catch_no ){
		no = 0 ;
		pls2c_02_set( pl_w ) ;
	} else {
		no = 2 ;
		pl_w->timer0 = 32 ;
		pl_w->routine_2 = 0 ;
	}
	pl_char_set( pl_w, Char_Catch, no ) ;
	ofs.x = -120.0f ;
	ofs.y = 270.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	catch_gauge_add( pl_w ) ;
}

/****************************************************************/
/*			A throw in progress: the first slams, the second	*/
/*			picks a strength from the stick						*/
/****************************************************************/
void pl2c_catch( PLAYER_WORK *pl_w )
{
	Sint16 no ;

	pl_w->catch_flag = 1 ;
	if( !pl_w->catch_no ){
		if( pl_char_move( pl_w ) < 0 ){
			scr_excep_clr() ;
			sp_ret( pl_w, pl_w->pl_stance ) ;
		} else {
			if( pl_w->cg_type == 1 ){
				pl_w->cg_type = 0 ;
				pls2c_01_set( pl_w, 3 ) ;
			}
			if( pl_w->cg_type == 2 ){
				pl_w->cg_type = 0 ;
				pl_w->spd.x = 0.0f ;
				pl_w->spd.y = 0.0f ;
				pl_w->acc.x = 0.0f ;
				pl_w->acc.y = 0.0f ;
				pl_w->catch_w->dm_hit_w = pl_w ;
				pl_w->catch_w->catch_type = 5 ;
				pl_w->catch_w->atck_no = catch_tbl0[( pl_w->pl_sw_new & KEY_LEVER ) >> 10] ;
				scr_excep_clr() ;
			}
		}
	} else if( !pl_w->routine_2 ){
		if( -- pl_w->timer0 < 0 ){
			pl_w->routine_2 ++ ;
			no = ( pl_w->pl_sw_new & KEY_LEVER ) >> 10 ;
			pl_w->atck_level = catch_tbl1[no] ;
			pl_char_set( pl_w, Char_Catch, catch_tbl2[no] ) ;
		}
		pl_char_move( pl_w ) ;
	} else if( pl_char_move( pl_w ) < 0 ){
		scr_excep_clr() ;
		sp_ret( pl_w, pl_w->pl_stance ) ;
	} else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->catch_w->dm_hit_w = pl_w ;
		pl_w->catch_w->catch_type = 1 ;
		pl_w->catch_w->atck_no = pl_w->atck_level ;
	}
}

/****************************************************************/
void pl2c_hold( PLAYER_WORK *pl_w )
{
	catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void pl2c_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl2c_sp_shock ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl2c_sp_tempest ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl2c_sp_tempest ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl2c_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl2c_sp_shock ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl2c_sp_tempest ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl2c_sp_tempest ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl2c_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl2c_sp_disrupt ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl2c_sp_grav ;
		pl_w->atck_level = 0 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl2c_sp_crouch_hp ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void pl2c_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl2c_sp_disrupt ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl2c_sp_grav ;
		pl_w->atck_level = 0 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl2c_sp_crouch_hp ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl2c.c" End of File	*/

/* Morrigan (character 03): command checks, normal attacks, appearance, win and lose poses, special
   moves and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl03_def.h"

void pl03_main( PLAYER_WORK *pl_w );
void pl03_com_ck( PLAYER_WORK *pl_w );
void pl03_atck_set( PLAYER_WORK *pl_w );
void pl03_atck( PLAYER_WORK *pl_w );
void pl03_dash( PLAYER_WORK *pl_w );
void pl03_noki( PLAYER_WORK *pl_w );
void pl03_appear( PLAYER_WORK *pl_w );
void pl03_win( PLAYER_WORK *pl_w );
void pl03_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl03_catch_ck( PLAYER_WORK *pl_w );
void pl03_catch_set( PLAYER_WORK *pl_w );
void pl03_catch( PLAYER_WORK *pl_w );
void pl03_hold( PLAYER_WORK *pl_w );
int pl03_delay_ck( PLAYER_WORK *pl_w );
void pl03_t_kick( PLAYER_WORK *pl_w );
void pl03_upper( PLAYER_WORK *pl_w );
void pl03_sp_unison( PLAYER_WORK *pl_w );
void pl03_sp_gattai( PLAYER_WORK *pl_w );
void pl03_sp_assist( PLAYER_WORK *pl_w );
void pl03_sp_zero( PLAYER_WORK *pl_w );
void pl03_air_dash( PLAYER_WORK *pl_w );
void pl03_tlocal_init( PLAYER_WORK *pl_w );

/* the palette for each of the six colours, used with ef_color_set() */
static Sint32 appear_color[6] = { 0x42, 0x4b, 0x54, 0x5d, 0x66, 0x6f } ;
static Sint32 sp_color[6] = { 0x30, 0x33, 0x36, 0x39, 0x3c, 0x3f } ;
static Sint32 win_color[6] = { 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d } ;

/* each attack's table: standing and crouching, jumping, super jumping */
static Sint8 lp_tbl[4] = {  1,  2,  0,  2 } ;
static Sint8 mp_tbl[4] = { -1,  2,  1,  2 } ;
static Sint8 hp_tbl[4] = { -1, -1, -1,  2 } ;
static Sint8 lk_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 mk_tbl[4] = { -1,  2, -1,  2 } ;
static Sint8 hk_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 j_lp_tbl[4] = {  1,  2,  0,  2 } ;
static Sint8 j_mp_tbl[4] = { -1,  2,  1,  2 } ;
static Sint8 j_hp_tbl[4] = { -1, -1, -1,  2 } ;
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
static Uint16 com_soul[] = { 3, COM_GROUND, pl03_sp_soul, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Soul Fist */
static Uint16 com_jsoul[] = { 3, COM_AIR, pl03_sp_soul, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Soul Fist in the air */
static Uint16 com_blade[] = { 3, COM_GROUND, pl03_sp_blade, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_PUNCH } ;	/* Shadow Blade */
static Uint16 com_jblade[] = { 3, COM_AIR, pl03_sp_blade, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_PUNCH } ;	/* Shadow Blade in the air */
static Uint16 com_vector[] = { 5, COM_GROUND, pl03_sp_vector, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Vector Drain */
static Uint16 com_shower[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl03_sp_shower, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Finishing Shower (hyper) */
static Uint16 com_silhouette[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl03_sp_silhouette, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_PUNCH } ;	/* Silhouette Blade (hyper) */
static Uint16 com_illusion[] = { 3, COM_HYPER | COM_ALL_SW, pl03_sp_illusion, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Darkness Illusion (hyper) */
static Uint16 com_shell[] = { 3, COM_AIR, pl03_sp_shell, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Shell Kick (in the air) */

static void ( *pl03_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl03_main,
	pl03_com_ck,
	pl03_atck_set,
	pl03_atck,
	pl03_dash,
	pl03_noki,
	pl03_appear,
	pl03_win,
	pl03_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl03_catch_ck,	/* returns a value */
	pl03_catch_set,
	pl03_catch,
	pl03_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl03_delay_ck,
	pl03_t_kick,
	pl03_upper,
	pl03_sp_unison,
	pl03_sp_gattai,
	pl03_sp_assist,
	pl03_sp_zero,
	0,
	pl03_air_dash,
	0,
	pl03_tlocal_init,
} ;

static Uint8 soul_ck( PLAYER_WORK *pl_w );
static Uint8 jsoul_ck( PLAYER_WORK *pl_w );
static Uint8 blade_ck( PLAYER_WORK *pl_w );
static Uint8 jblade_ck( PLAYER_WORK *pl_w );
static Uint8 vector_ck( PLAYER_WORK *pl_w );
static Uint8 shower_ck( PLAYER_WORK *pl_w );
static Uint8 silhouette_ck( PLAYER_WORK *pl_w );
static Uint8 illusion_ck( PLAYER_WORK *pl_w );
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static Uint8 shell_ck( PLAYER_WORK *pl_w );
static int shower_delay_ck( PLAYER_WORK *pl_w );
static int silhouette_delay_ck( PLAYER_WORK *pl_w );
static int illusion_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl03_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl03_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl03_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( silhouette_ck( pl_w ) ) return ;
	if( illusion_ck( pl_w ) ) return ;
	if( shower_ck( pl_w ) ) return ;
	if( jblade_ck( pl_w ) ) return ;
	if( blade_ck( pl_w ) ) return ;
	if( jsoul_ck( pl_w ) ) return ;
	if( soul_ck( pl_w ) ) return ;
	if( vector_ck( pl_w ) ) return ;
	if( shell_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	if( pl_air_dash_ck( pl_w, &pl_w->com_w9 ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
static Uint8 soul_ck( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	if( !com_nml_ck( pl_w, com_soul, &pl_w->com_w0 ) ) return 0 ;
	if( lw->shot_flag ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl03_sp_soul ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			In the air: once a jump unless super jumping,		*/
/*			and not too low										*/
/*==============================================================*/
static Uint8 jsoul_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_jsoul, &pl_w->com_w1 ) ) return 0 ;
	if( pl_w->ground_y + DOT_Y( 64.0f ) > pl_w->pos.y ) return 0 ;
	if( pl_w->jump_com_cnt && !pl_w->sjump_flag ) return 0 ;
	pl_w->jump_com_cnt = 1 ;
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 1 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl03_sp_soul ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 blade_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_blade, &pl_w->com_w2 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl03_sp_blade ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 jblade_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_jblade, &pl_w->com_w3 ) ) return 0 ;
	if( pl_w->ground_y + DOT_Y( 64.0f ) > pl_w->pos.y ) return 0 ;
	if( pl_w->jump_com_cnt && !pl_w->sjump_flag ) return 0 ;
	pl_w->jump_com_cnt = 1 ;
	com_sw_set( pl_w, &pl_w->com_w3 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 1 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl03_sp_blade ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 vector_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_vector, &pl_w->com_w4 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w4 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl03_sp_vector ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 shower_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_shower, &pl_w->com_w5 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl03_sp_shower ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 silhouette_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_silhouette, &pl_w->com_w6 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl03_sp_silhouette ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
/*			On the ground or, once a jump, in the air			*/
/*==============================================================*/
static Uint8 illusion_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_illusion, &pl_w->com_w7 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR ){
		if( pl_w->jump_com_cnt && !pl_w->sjump_flag ) return 0 ;
		pl_w->jump_com_cnt = 1 ;
		pl_w->type = 1 ;
	} else pl_w->type = 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl03_sp_illusion ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 8 ) ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl03_sp_chouhatsu ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int snapback_ck( PLAYER_WORK *pl_w )
{
	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl03_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 shell_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_shell, &pl_w->com_w8 ) ) return 0 ;
	if( pl_w->jump_com_cnt && !pl_w->sjump_flag ) return 0 ;
	pl_w->jump_com_cnt = 1 ;
	com_sw_set( pl_w, &pl_w->com_w8 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl03_sp_shell ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int shower_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_shower, &pl_w->com_w5 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl03_sp_shower ;
	return 1 ;
}

/*==============================================================*/
static int silhouette_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_silhouette, &pl_w->com_w6 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl03_sp_silhouette ;
	return 1 ;
}

/*==============================================================*/
static int illusion_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_illusion, &pl_w->com_w7 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->type = 0 ;
	pl_w->delay_sp_type = pl03_sp_illusion ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl03_delay_ck( PLAYER_WORK *pl_w )
{
	if( silhouette_delay_ck( pl_w ) || illusion_delay_ck( pl_w ) || shower_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/****************************************************************/
void pl03_main( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	if( !( pl_w->routine_1 == PL_NORM && pl_w->act_st == N_SPECIAL )
	 && !( pl_w->routine_1 == PL_NORM && pl_w->act_st == N_SUPER ) ){
		if( lw->col_timer && !-- lw->col_timer ) pl_color_set( pl_w, PC_DEFAULT ) ;
	}
	if( lw->dash_ef_flag ){
		if( pl_w->routine_1 == PL_NORM ){
			if( pl_w->act_st == N_SUPER && pl_w->pl_sp_type == pl03_sp_illusion ) return ;
			if( pl_w->act_st == N_DASH || pl_w->act_st == N_BACK_DASH || pl_w->act_st == N_AIR_DASH ) return ;
		}
		lw->dash_ef_flag = 0 ;
		pl_voice_req( pl_w, 43 ) ;
	}
	if( lw->dash_flag ){
		if( pl_w->routine_1 == PL_NORM && pl_w->act_st == N_AIR_DASH ) return ;
		lw->dash_flag = 0 ;
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
		pl_w->unk_2a2 = 1 ;
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
		if( pl_w->atck_lever & KEY_BACK ){
			pl_w->char_no.b.no = 3 ;
			pl_w->atck_no = pl03_at_dback_hp ;
		} else {
			pl_w->char_no.b.no = 2 ;
			pl_w->atck_no = ATCK_CROUCH_HP ;
		}
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
		if( pl_w->atck_lever & KEY_FORWARD ){
			pl_w->routine_2 ++ ;
			pl_w->routine_3 = 0 ;
			pl_w->char_no.b.no = 3 ;
			pl_w->atck_no = pl03_at_fwd_hk ;
		} else {
			pl_w->char_no.b.no = 2 ;
			pl_w->atck_no = ATCK_STAND_HK ;
		}
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
		if( pl_w->atck_lever & KEY_DOWN ){
			/* stops the jump */
			pl_w->routine_2 = 2 ;
			pl_w->routine_3 = 0 ;
			pl_w->sjump_flag = 0 ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_w->char_no.b.no = 5 ;
			pl_w->atck_no = pl03_at_jdown_hk ;
		} else {
			pl_w->char_no.b.no = 2 ;
			pl_w->atck_no = ATCK_JUMP_HK ;
		}
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

/*==============================================================*/
/*			An attack ends a flight: she starts to fall			*/
/*==============================================================*/
static void jump_atck( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	if( lw->dash_flag ){
		lw->dash_flag = 0 ;
		if( pl_w->spd.y > 0.0f ) pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
	}
	if( pl_w->atck_pk == 1 ) jump_kick( pl_w ) ;
	else jump_punch( pl_w ) ;
}

/*==============================================================*/
static void jump_atck_ck( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	if( lw->dash_flag ){
		lw->dash_flag = 0 ;
		if( pl_w->spd.y > 0.0f ) pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
	}
	if( ( pl_w->atck_pk == 0 && ( pl_w->jump_atk_cnt & JATK_P_MASK ) )
	 || ( pl_w->atck_pk != 0 && ( pl_w->jump_atk_cnt & JATK_K_MASK ) ) ) jump_atck( pl_w ) ;
}

/*==============================================================*/
static void nml_atck_dash( PLAYER_WORK *pl_w )
{
	nml_atck( pl_w ) ;
}

/****************************************************************/
/*			Normal attacks										*/
/****************************************************************/
void pl03_atck_set( PLAYER_WORK *pl_w )
{
	static void ( *atck_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_atck, jump_atck_ck, jump_atck, nml_atck_dash,
	} ;

	atck_set_jmp[pl_w->atck_type]( pl_w ) ;
}

/*==============================================================*/
static void stand_punch_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			The light one hits twice							*/
/*==============================================================*/
static void crouch_punch_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		if( pl_char_move( pl_w ) < 0 ){
			pl_stand_ret( pl_w ) ;
			return ;
		}
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pl_w->atck_no = pl03_at_crouch_lp_2 ;
			HIT_RESET( pl_w ) ;
		}
		break ;
	case 1:
	case 2:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
/*			The heavy kick with the stick forward: a hop		*/
/*==============================================================*/
static void stand_hk_start( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->spd.x = DOT_X( 2.0f ) ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 2.0f ) ;
		pl_w->acc.y = -0.3348214f ;
		if( !pl_w->disp_dir ) pl_w->spd.x = -pl_w->spd.x ;
		pl_char_set( pl_w, Char_Stand_K, 4 ) ;
	}
}

/*==============================================================*/
static void stand_hk_jump( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_land_ef_set( pl_w ) ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_char_set( pl_w, Char_Stand_K, 5 ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->atck_no = pl_w->cg_type ;
		HIT_RESET( pl_w ) ;
		pl_w->cg_type = 0 ;
	}
}

/*==============================================================*/
static void stand_hk_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void stand_kick_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	case 2:
		if( !pl_w->routine_2 ){
			if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
			break ;
		}
		switch( pl_w->routine_3 ){
		case 0:
			stand_hk_start( pl_w ) ;
			break ;
		case 1:
			stand_hk_jump( pl_w ) ;
			break ;
		case 2:
			stand_hk_land( pl_w ) ;
			break ;
		}
		break ;
	}
}

/*==============================================================*/
static void crouch_kick_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
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
static void jump_punch_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
	if( pl_land_ck( pl_w ) ) pl_land_set( pl_w ) ;
}

/*==============================================================*/
/*			The heavy kick with the stick down stops the		*/
/*			jump: she hangs, then dives							*/
/*==============================================================*/
static void jump_hk_hang( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_3 ){
		pl_w->routine_3 ++ ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y /= 2.0f ;
		pl_w->acc.y = -0.4017857f ;
	}
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void jump_hk_dive( PLAYER_WORK *pl_w )
{
	switch( pl_w->routine_3 ){
	case 0:
		pl_char_move( pl_w ) ;
		if( pl_w->cg_type & 1 ){
			pl_w->cg_type &= 0xfe ;
			pl_w->routine_3 ++ ;
			if( !pl_w->sjump_flag ) pl_w->spd.y = -DOT_Y( 12.0f ) ;
			else pl_w->spd.y = -DOT_Y( 15.0f ) ;
		}
		break ;
	case 1:
		pl_char_move( pl_w ) ;
		if( !pl_w->hit_flag ){
			if( pl_w->ground_y > pl_w->pos.y ){
				pl_w->routine_3 ++ ;
				pl_w->pos.y = pl_w->ground_y ;
				pl_w->pl_stance = STANCE_STAND ;
				pl_w->spd.x = 0.0f ;
				pl_w->spd.y = 0.0f ;
				pl_w->acc.x = 0.0f ;
				pl_w->acc.y = 0.0f ;
				pl_land_ef_set( pl_w ) ;
				pl_char_set( pl_w, Char_Jump, 3 ) ;
			}
		} else {
			/* it hit: she bounces off */
			pl_w->spd.x = DOT_X( 3.5f ) ;
			if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = 0.0f ;
			pl_w->spd.y = 15.0f ;
			pl_w->acc.y = -DOT_Y( 0.5f ) ;
			pl_w->jump_dir = 1 ;
			pl_jump_ret2( pl_w ) ;
		}
		break ;
	case 2:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
/*			The heavy kick hangs, then dives; the other			*/
/*			kicks fall and land									*/
/*==============================================================*/
static void jump_kick_move( PLAYER_WORK *pl_w )
{
	if( pl_w->nml_no == 2 ){
		switch( pl_w->routine_2 ){
		case 1:
			jump_hk_hang( pl_w ) ;
			break ;
		case 2:
			jump_hk_dive( pl_w ) ;
			return ;
		default:
			goto move ;
		}
		if( pl_land_ck( pl_w ) ) goto land ;
	}
	else {
move :
		if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
		if( !pl_land_ck( pl_w ) ) return ;
land :
		pl_land_set( pl_w ) ;
	}
}

/*==============================================================*/
static void jump_move( PLAYER_WORK *pl_w )
{
	pl_jump_move( pl_w ) ;
	pl_sjump_top_ck( pl_w ) ;
	if( pl_w->atck_pk == 1 ) jump_kick_move( pl_w ) ;
	else jump_punch_move( pl_w ) ;
}

/*==============================================================*/
static void sjump_move( PLAYER_WORK *pl_w )
{
	pl_jump_hit_ck( pl_w ) ;
	pl_sjump_ctrl( pl_w ) ;
	jump_move( pl_w ) ;
}

/*==============================================================*/
static void nml_move_dash( PLAYER_WORK *pl_w )
{
	pl_smoke_set( pl_w ) ;
	nml_move( pl_w ) ;
}

/****************************************************************/
/*			The normal attacks as they play out					*/
/****************************************************************/
void pl03_atck( PLAYER_WORK *pl_w )
{
	static void ( *atck_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_move, jump_move, sjump_move, nml_move_dash,
	} ;

	atck_jmp[pl_w->atck_type]( pl_w ) ;
}

/****************************************************************/
/*			The launcher										*/
/****************************************************************/
void pl03_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_gauge_add( pl_w, 5 ) ;
		if( pl_w->atck_pk == 0 ){
			pl_w->atck_no = pl03_at_upper ;
			pl_w->pl_stance = STANCE_CROUCH ;
			pl_char_set( pl_w, Char_Extra, 9 ) ;
			HIT_RESET( pl_w ) ;
			pl_se_req( pl_w, SE_SWING_H ) ;
		}
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
/*			The dash: she flies forward							*/
/****************************************************************/
void pl03_dash( PLAYER_WORK *pl_w )
{
	static void ( *dash_jmp[])( PLAYER_WORK *pl_w ) = {
		dash_init, dash_move, dash_end,
	} ;

	dash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dash_init( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->timer0 = 16 ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 8.0f ) : -DOT_X( 8.0f ) ;
		lw->dash_ef_flag = 1 ;
		pl_voice_req( pl_w, 31 ) ;
	}
}

/*--------------------------------------------------------------*/
static void dash_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !--pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->acc.x = pl_w->pl_dir ? -DOT_X( 0.5f ) : DOT_X( 0.5f ) ;
		pl_char_set( pl_w, Char_Dash, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void dash_end( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		lw->dash_ef_flag = 0 ;
		pl_voice_req( pl_w, 43 ) ;
		pl_stand_ret( pl_w ) ;
	}
}

static void air_dash_init( PLAYER_WORK *pl_w );
static void air_dash_start( PLAYER_WORK *pl_w );
static void air_dash_move( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The dash in the air, level, up or down as the		*/
/*			stick says											*/
/****************************************************************/
void pl03_air_dash( PLAYER_WORK *pl_w )
{
	static void ( *air_dash_jmp[])( PLAYER_WORK *pl_w ) = {
		air_dash_init, air_dash_start, air_dash_move,
	} ;

	air_dash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void air_dash_init( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	pl_w->routine_2 ++ ;
	pl_w->sp_ng_timer = 10 ;
	pl_w->atck_ng_timer = 10 ;
	lw->dash_flag = 1 ;
	lw->dash_sw = pl_w->atck_lever ;
	pl_char_set( pl_w, Char_Dash, 4 ) ;
}

/* speed and its change, in 1/65536ths: level, down, up */
static Sint32 air_dash_spd[3][4] = {
	{ -0x98000,       0,        0, -0x5000 },
	{ -0x40000, -0x2000,  -0xc8000,  0xa800 },
	{ -0x70000,  0x1000,   0xb8000, -0xb000 },
} ;

/*--------------------------------------------------------------*/
static void air_dash_start( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;
	Uint16 lever ;
	Uint16 no ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		lw->dash_ef_flag = 1 ;
		pl_voice_req( pl_w, 31 ) ;
		pl_w->timer0 = 32 ;
		lever = pl_w->pl_sw_new ;
		if( pl_w->cpu_flag ) lever = lw->dash_sw ;
		no = ( lever & 0x3000 ) / 0x1000 ;
		pl_w->spd.x = air_dash_spd[no][0] * 1.6666666f / 65536.0f ;
		pl_w->acc.x = air_dash_spd[no][1] * 1.6666666f / 65536.0f ;
		pl_w->spd.y = air_dash_spd[no][2] * 2.142857f / 65536.0f ;
		pl_w->acc.y = air_dash_spd[no][3] * 2.142857f / 65536.0f ;
		if( pl_w->disp_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		pl_char_set( pl_w, Char_Dash, no + 5 ) ;
	}
}

/*--------------------------------------------------------------*/
static void air_dash_move( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ) pl_w->pos.y = pl_w->ground_y ;
	if( !--pl_w->timer0 ){
		pl_w->acc.x = 0.0f ;
		lw->dash_ef_flag = 0 ;
		lw->dash_flag = 0 ;
		pl_voice_req( pl_w, 43 ) ;
		pl_jump_ret( pl_w ) ;
	}
}

static void noki_init( PLAYER_WORK *pl_w );
static void noki_move( PLAYER_WORK *pl_w );
static void noki_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash: she flies back						*/
/****************************************************************/
void pl03_noki( PLAYER_WORK *pl_w )
{
	static void ( *noki_jmp[])( PLAYER_WORK *pl_w ) = {
		noki_init, noki_move, noki_end,
	} ;

	noki_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void noki_init( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->routine_2 ++ ;
	pl_w->timer0 = 16 ;
	pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 7.0f ) : DOT_X( 7.0f ) ;
	lw->dash_ef_flag = 1 ;
	pl_voice_req( pl_w, 31 ) ;
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
		pl_w->acc.x = pl_w->pl_dir ? DOT_X( 0.5f ) : -DOT_X( 0.5f ) ;
		pl_char_set( pl_w, Char_Dash, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void noki_end( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		lw->dash_ef_flag = 0 ;
		pl_voice_req( pl_w, 43 ) ;
		pl_stand_ret( pl_w ) ;
	}
}

static void appear_init( PLAYER_WORK *pl_w );
static void appear_wait( PLAYER_WORK *pl_w );
static void appear_change( PLAYER_WORK *pl_w );
static void appear_end( PLAYER_WORK *pl_w );

/****************************************************************/
void pl03_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_init, appear_wait, appear_change, appear_end,
	} ;

	appear_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void appear_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->disp_flag = 1 ;
	pls03_03_set( pl_w, 0 ) ;
	ef_color_set( pl_w, 2, appear_color[pl_w->pl_color], 1 ) ;
	pl_char_set( pl_w, Char_Appear, 0 ) ;
}

/*--------------------------------------------------------------*/
static void appear_wait( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 32 ;
		pl_char_set( pl_w, Char_Appear, 1 ) ;
	}
}

/*==============================================================*/
/*			Her colours change as the animation says			*/
/*==============================================================*/
static void appear_change( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_prm ){
		pl_w->cg_prm = 0 ;
		ef_color_set( pl_w, 0, appear_color[pl_w->pl_color] + pl_w->cg_flag1, 1 ) ;
		if( !pl_w->timer0-- ){
			ef_color_set( pl_w, 0, appear_color[pl_w->pl_color] + 4, 1 ) ;
			pl_w->routine_2 ++ ;
			pl_char_set( pl_w, Char_Appear, 2 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void appear_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_1 ++ ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
		return ;
	}
	if( pl_w->cg_prm ){
		if( pl_w->cg_prm == 96 ) pl_color_set( pl_w, PC_DEFAULT ) ;
		else ef_color_set( pl_w, 0, appear_color[pl_w->pl_color] + pl_w->cg_prm, 1 ) ;
		pl_w->cg_prm = 0 ;
	}
}

static void win_init( PLAYER_WORK *pl_w );
static void win_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl03_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_init, win_move,
	} ;

	win_jmp[pl_w->routine_2]( pl_w ) ;
}

/* the KO win poses: the animation, then lw->bat_mode */
static Sint8 win_pose[16][2] = {
	{ 0, 7 }, { 0, 7 }, { 0, 7 }, { 0, 7 }, { 1, 8 }, { 1, 8 }, { 2, 8 }, { 2, 8 },
	{ 3, 8 }, { 3, 8 }, { 4, 8 }, { 4, 8 }, { 5, 8 }, { 5, 8 }, { 5, 8 }, { 5, 8 },
} ;

/*==============================================================*/
/*			A button held picks the pose, or else it is			*/
/*			random												*/
/*==============================================================*/
static void win_ko_init( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;
	Sint8 no ;

	if( pl_w->pl_new_lvbt & KEY_BUTTON ){
		if( pl_w->pl_new_lvbt & KEY_LP ) no = 0 ;
		else if( pl_w->pl_new_lvbt & KEY_HP ) no = 4 ;
		else if( pl_w->pl_new_lvbt & KEY_A1 ) no = 6 ;
		else if( pl_w->pl_new_lvbt & KEY_LK ) no = 8 ;
		else if( pl_w->pl_new_lvbt & KEY_HK ) no = 10 ;
		else if( vs_sys_work.vs_end_flag && pl_be_cnt( pl_w ) == 1 ) no = 12 ;
		else no = ( nlRand() + Sys->game_timer0 ) & 7 ;
	} else if( vs_sys_work.vs_end_flag && pl_be_cnt( pl_w ) == 1 ){
		no = ( nlRand() + Sys->game_timer0 ) & 15 ;
	} else no = ( nlRand() + Sys->game_timer0 ) & 7 ;
	pl_w->char_no.b.no = win_pose[no][0] ;
	pl_w->sub_type = win_pose[no][0] ;
	lw->bat_mode = win_pose[no][1] ;
	pl_char_set( pl_w, Char_Win, pl_w->char_no.b.no ) ;
	if( pl_w->sub_type == 5 ) pls03_03_set( pl_w, 2 ) ;
}

/*==============================================================*/
static void win_time( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	lw->bat_mode = 9 ;
	pl_char_set( pl_w, Char_Win, 7 ) ;
}

/*==============================================================*/
static void lose_time_ko( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	lw->bat_mode = 9 ;
	pl_char_set( pl_w, Char_Win, 8 ) ;
}

/*==============================================================*/
static void win_init( PLAYER_WORK *pl_w )
{
	static void ( *win_init_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko_init, lose_time_ko, win_time, lose_time_ko, lose_time_ko,
	} ;

	pl_w->routine_2 ++ ;
	pl_w->routine_3 = 0 ;
	win_init_jmp[pl_w->type]( pl_w ) ;
}

/*==============================================================*/
/*			Pose 0: she rises, then floats						*/
/*==============================================================*/
static void win_float_rise( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_flag1 ) pl_w->pos.y += DOT_Y( 0.5f ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->spd.y = DOT_Y( 0.5f ) ;
		pl_w->acc.y = -0.06696428f ;
	}
}

/*--------------------------------------------------------------*/
static void win_float_hold( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_flag1 ){
		if( pl_w->spd.y > DOT_Y( 0.5f ) ) pl_w->acc.y = -pl_w->acc.y ;
		if( pl_w->spd.y < -DOT_Y( 0.5f ) ) pl_w->acc.y = -pl_w->acc.y ;
	}
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void win_float_move( PLAYER_WORK *pl_w )
{
	static void ( *win_float_jmp[])( PLAYER_WORK *pl_w ) = {
		win_float_rise, win_float_hold,
	} ;
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	if( pl_win_end_ck( pl_w ) ){
		lw->bat_mode = 0 ;
		pl_stand_ret( pl_w ) ;
	} else {
		win_float_jmp[pl_w->routine_3]( pl_w ) ;
	}
}

/*==============================================================*/
static void win_pose_move( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	if( pl_win_end_ck( pl_w ) ){
		lw->bat_mode = 0 ;
		pl_stand_ret( pl_w ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
/*			Pose 5: her colours change							*/
/*==============================================================*/
static void win_change_move( PLAYER_WORK *pl_w )
{
	pl_w->win_wait = 0xff ;
	pl_win_end_ck( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ) ef_color_set( pl_w, 0, win_color[pl_w->pl_color], 1 ) ;
}

/*==============================================================*/
static void win_ko_move( PLAYER_WORK *pl_w )
{
	static void ( *win_ko_jmp[])( PLAYER_WORK *pl_w ) = {
		win_float_move, win_pose_move, win_pose_move, win_pose_move, win_pose_move, win_change_move,
	} ;

	win_ko_jmp[pl_w->sub_type]( pl_w ) ;
}

/*==============================================================*/
static void lose_move( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	if( pl_win_end_ck( pl_w ) ){
		lw->bat_mode = 0 ;
		pl_stand_ret( pl_w ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_move( PLAYER_WORK *pl_w )
{
	static void ( *win_move_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko_move, lose_move, lose_move, lose_move, lose_move,
	} ;

	win_move_jmp[pl_w->type]( pl_w ) ;
}

static void sp_soul( PLAYER_WORK *pl_w );
static void sp_blade( PLAYER_WORK *pl_w );
static void sp_vector( PLAYER_WORK *pl_w );
static void sp_illusion( PLAYER_WORK *pl_w );
static void sp_shower( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_silhouette( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );
static void sp_shell( PLAYER_WORK *pl_w );

/****************************************************************/
void pl03_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *pl_sp_tbl[])( PLAYER_WORK *pl_w ) = {
		sp_soul,				/* 00 */
		sp_soul,				/* 01 Soul Fist */
		sp_blade,				/* 02 Shadow Blade */
		sp_vector,					/* 03 Vector Drain */
		sp_soul,				/* 04 */
		sp_soul,				/* 05 */
		sp_illusion,					/* 06 Darkness Illusion (hyper) */
		sp_shower,					/* 07 Finishing Shower (hyper) */
		sp_chouhatsu,					/* 08 the taunt */
		sp_silhouette,					/* 09 Silhouette Blade (hyper) */
		sp_soul,				/* 0a */
		sp_soul,				/* 0b */
		sp_soul,				/* 0c */
		sp_soul,				/* 0d */
		sp_snapback,					/* 0e the snapback */
		sp_shell,					/* 0f Shell Kick (in the air) */
	} ;
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	lw->bat_mode = 0 ;
	pl_sp_tbl[pl_w->pl_sp_type]( pl_w ) ;
}

/*==============================================================*/
/*			A special move starts standing on the ground		*/
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

static void soul( PLAYER_WORK *pl_w );
static void soul_init( PLAYER_WORK *pl_w );
static void soul_move( PLAYER_WORK *pl_w );
static void jsoul( PLAYER_WORK *pl_w );
static void jsoul_init( PLAYER_WORK *pl_w );
static void jsoul_move( PLAYER_WORK *pl_w );
static void jsoul_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The soul fist, on the ground or in the air			*/
/*==============================================================*/
static void sp_soul( PLAYER_WORK *pl_w )
{
	static void ( *sp_soul_jmp[])( PLAYER_WORK *pl_w ) = {
		soul, jsoul,
	} ;

	sp_soul_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void soul( PLAYER_WORK *pl_w )
{
	static void ( *soul_jmp[])( PLAYER_WORK *pl_w ) = {
		soul_init, soul_move,
	} ;
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	lw->bat_mode = 10 ;
	soul_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void soul_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_w->atck_no = pl03_at_soul ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	sp_stand_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->char_no.b.no = pl_w->atck_level ;
	pl_char_set( pl_w, Char_Special, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
/*			The fist goes out when the animation says			*/
/*==============================================================*/
static void soul_move( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		if( shl03_00_set( pl_w, 0, 0 ) ){
			lw->shot_flag = 1 ;
			pl_voice_req( pl_w, 30 ) ;
		}
	}
	if( pl_char_move( pl_w ) < 0 ){
		lw->bat_mode = 0 ;
		pl_stand_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void jsoul( PLAYER_WORK *pl_w )
{
	static void ( *jsoul_jmp[])( PLAYER_WORK *pl_w ) = {
		jsoul_init, jsoul_move, jsoul_land,
	} ;
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	lw->bat_mode = 11 ;
	jsoul_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
/*			In the air she slows down while she throws it		*/
/*==============================================================*/
static void jsoul_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_w->atck_no = pl03_at_jsoul ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->spd.x /= 8.0f ;
	pl_w->acc.x /= 8.0f ;
	pl_w->spd.y /= 8.0f ;
	pl_w->acc.y /= 8.0f ;
	pl_w->char_no.b.no = pl_w->atck_level + 2 ;
	pl_char_set( pl_w, Char_Special, pl_w->char_no.b.no ) ;
}

/*--------------------------------------------------------------*/
static void jsoul_move( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shl03_00_set( pl_w, 1, 0 ) ;
		pl_voice_req( pl_w, 30 ) ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ) pl_w->pos.y = pl_w->ground_y ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		lw->bat_mode = 0 ;
		pl_w->spd.x = pl_w->pl_dir ? -1.6666666f : 1.6666666f ;
		pl_w->acc.x = pl_w->pl_dir ? 0.026041666f : -0.026041666f ;
		pl_w->spd.y = 7.5f ;
		pl_w->acc.y = -0.4017857f ;
		pl_char_set( pl_w, Char_Special, 4 ) ;
	}
}

/*--------------------------------------------------------------*/
static void jsoul_land( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;
	Sint8 land ;

	lw->bat_mode = 0 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	land = pl_char_move( pl_w ) ;
	if( land < 0 ) pl_jump_ret( pl_w ) ;
}

static void blade( PLAYER_WORK *pl_w );
static void blade_init( PLAYER_WORK *pl_w );
static void blade_move( PLAYER_WORK *pl_w );
static void blade_fall( PLAYER_WORK *pl_w );
static void blade_land( PLAYER_WORK *pl_w );
static void jblade( PLAYER_WORK *pl_w );
static void jblade_init( PLAYER_WORK *pl_w );
static void jblade_move( PLAYER_WORK *pl_w );
static void jblade_fall( PLAYER_WORK *pl_w );
static void jblade_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The shadow blade, on the ground or in the air		*/
/*==============================================================*/
static void sp_blade( PLAYER_WORK *pl_w )
{
	static void ( *sp_blade_jmp[])( PLAYER_WORK *pl_w ) = {
		blade, jblade,
	} ;

	sp_blade_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void blade( PLAYER_WORK *pl_w )
{
	static void ( *blade_jmp[])( PLAYER_WORK *pl_w ) = {
		blade_init, blade_move, blade_fall, blade_land,
	} ;

	blade_jmp[pl_w->routine_3]( pl_w ) ;
}

/* speed and its change for each strength */
static Float blade_spd[][4] = {
	{  7.5f, -DOT_X( 0.25f ), DOT_Y( 5.0f ), -0.73660713f },
	{ DOT_X( 11.0f ), -0.5729166f, DOT_Y( 13.0f ), -0.8705357f },
} ;

/*==============================================================*/
static void blade_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	if( pl_w->team_type == 3 ){
		pl_w->atck_no = pl03_at_blade_h ;
		HIT_RESET( pl_w ) ;
	} else {
		pl_w->atck_no = pl_w->atck_level + 48 ;
		HIT_RESET( pl_w ) ;
	}
	pl_gauge_add( pl_w, 5 ) ;
	sp_stand_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->spd.x = pl_w->pl_dir ? blade_spd[pl_w->atck_level][0] : -blade_spd[pl_w->atck_level][0] ;
	pl_w->acc.x = pl_w->pl_dir ? blade_spd[pl_w->atck_level][1] : -blade_spd[pl_w->atck_level][1] ;
	pl_w->spd.y = blade_spd[pl_w->atck_level][2] ;
	pl_w->acc.y = blade_spd[pl_w->atck_level][3] ;
	pl_w->char_no.b.no = pl_w->atck_level + 5 ;
	pl_char_set( pl_w, Char_Special, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
/*			She rises, hitting as the animation says			*/
/*==============================================================*/
static void blade_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = -DOT_Y( 2.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.5f ) ;
		pl_char_set( pl_w, Char_Special, 9 ) ;
	}
	if( pl_w->cg_type ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->pl_stance != STANCE_AIR ) pl_air_set( pl_w ) ;
	}
	if( pl_w->cg_flag1 ){
		/* both arms are the same in the arcade */
		if( pl_w->team_type == 3 ){
			pl_w->atck_no = pl_w->cg_flag1 + 49 ;
			HIT_RESET( pl_w ) ;
		} else {
			pl_w->atck_no = pl_w->cg_flag1 + 49 ;
			HIT_RESET( pl_w ) ;
		}
		pl_w->cg_flag1 = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void blade_fall( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_3 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 10 ) ;
	}
}

/*--------------------------------------------------------------*/
static void blade_land( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	if( pl_char_move( pl_w ) < 0 ){
		lw->bat_mode = 0 ;
		pl_stand_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void jblade( PLAYER_WORK *pl_w )
{
	static void ( *jblade_jmp[])( PLAYER_WORK *pl_w ) = {
		jblade_init, jblade_move, jblade_fall, jblade_land,
	} ;

	jblade_jmp[pl_w->routine_3]( pl_w ) ;
}

static Float jblade_spd[][4] = {
	{  7.5f, -DOT_X( 0.25f ), DOT_Y( 5.0f ), -0.73660713f },
	{ 10.0f, -DOT_X( 0.125f ), DOT_Y( 10.0f ), -DOT_Y( 0.375f ) },
} ;

/*==============================================================*/
static void jblade_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_w->atck_no = pl_w->atck_level + 52 ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.x = pl_w->pl_dir ? jblade_spd[pl_w->atck_level][0] : -jblade_spd[pl_w->atck_level][0] ;
	pl_w->acc.x = pl_w->pl_dir ? jblade_spd[pl_w->atck_level][1] : -jblade_spd[pl_w->atck_level][1] ;
	pl_w->spd.y = jblade_spd[pl_w->atck_level][2] ;
	pl_w->acc.y = jblade_spd[pl_w->atck_level][3] ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 7 ) ;
}

/*--------------------------------------------------------------*/
static void jblade_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = -DOT_Y( 2.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.5f ) ;
		pl_char_set( pl_w, Char_Special, 9 ) ;
	}
	if( pl_w->cg_type ){
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
}

/*--------------------------------------------------------------*/
static void jblade_fall( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_3 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 10 ) ;
	}
}

/*--------------------------------------------------------------*/
static void jblade_land( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	if( pl_char_move( pl_w ) < 0 ){
		lw->bat_mode = 0 ;
		pl_stand_ret( pl_w ) ;
	}
}

static void vector_init( PLAYER_WORK *pl_w );
static void vector_move( PLAYER_WORK *pl_w );
static void vector_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A flying grab										*/
/*==============================================================*/
static void sp_vector( PLAYER_WORK *pl_w )
{
	static void ( *vector_jmp[])( PLAYER_WORK *pl_w ) = {
		vector_init, vector_move, vector_end,
	} ;

	vector_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void vector_init( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	pl_w->routine_2 ++ ;
	pl_gauge_add( pl_w, 5 ) ;
	sp_stand_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 15 ) ;
	lw->dash_ef_flag = 1 ;
}

/*--------------------------------------------------------------*/
static void vector_move( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, 16 ) ;
		return ;
	}
	if( ( em = pl_catch_ck( pl_w ) ) ){
		scr_excep_set( pl_w, 5, 5 ) ;
		pl_w->catch_no = 0xc1 ;
		pl_catch_set( pl_w, em ) ;
	}
}

/*--------------------------------------------------------------*/
static void vector_end( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	if( pl_char_move( pl_w ) < 0 ){
		lw->bat_mode = 0 ;
		pl_stand_ret( pl_w ) ;
	}
}

static void shower_init( PLAYER_WORK *pl_w );
static void shower_wait( PLAYER_WORK *pl_w );
static void shower_pose( PLAYER_WORK *pl_w );
static void shower_shot( PLAYER_WORK *pl_w );
static void shower_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo: a rain of shots						*/
/*==============================================================*/
static void sp_shower( PLAYER_WORK *pl_w )
{
	static void ( *shower_jmp[])( PLAYER_WORK *pl_w ) = {
		shower_init, shower_wait, shower_pose, shower_shot, shower_end,
	} ;

	shower_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void shower_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl03_at_shower ;
	HIT_RESET( pl_w ) ;
	sp_stand_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->timer0 = 114 ;
	pls03_00_set( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 25 ) ;
	shower_wait( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shower_wait( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	lw->col_timer = 6 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->routine_2 ++ ;
		lw->renda_cnt = 10 ;
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
		pl_w->timer0 = 44 ;
		pls03_00_set2( pl_w ) ;
		ofs.x = -DOT_X( 8.0f ) ;
		ofs.y = DOT_Y( 52.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
		pl_char_set( pl_w, Char_Super, 26 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shower_pose( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	pl_w->stop_move_timer = 2 ;
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	lw->col_timer = 6 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_flag1 ) ef_color_set( pl_w, 2, sp_color[pl_w->pl_color] + pl_w->cg_prm, 1 ) ;
	if( pl_w->timer0 ){
		pl_w->timer0 -- ;
		return ;
	}
	pl_w->stop_move_flag = 0 ;
	pl_w->stop_move_timer = 0 ;
	if( !( vs_sys_work.game_stop & ( 1 << pl_w->pl_no ) ) ){
		pl_w->timer0 = 70 ;
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Super, 27 ) ;
	}
}

/*==============================================================*/
/*			The shots: each press of a button makes it last		*/
/*			longer												*/
/*==============================================================*/
static void shower_shot( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	lw->col_timer = 6 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type == 1 ){
		pl_w->cg_type = 0 ;
		shl03_01_set( pl_w ) ;
		pl_se_req( pl_w, 72 ) ;
	}
	if( !--pl_w->timer0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Super, 28 ) ;
		return ;
	}
	lw->gacha_flag = 0 ;
	if( !pl_w->cpu_flag ){
		if( lever_gacha_ck( pl_w ) ) lw->gacha_flag = -1 ;
		if( sw_renda_ck( pl_w ) && lw->renda_cnt ){
			lw->renda_cnt -- ;
			pl_w->timer0 ++ ;
		}
	}
}

/*--------------------------------------------------------------*/
static void shower_end( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	lw->col_timer = 6 ;
	if( pl_char_move( pl_w ) < 0 ){
		lw->bat_mode = 0 ;
		pl_stand_ret( pl_w ) ;
	}
}

static void chouhatsu_init( PLAYER_WORK *pl_w );
static void chouhatsu_move( PLAYER_WORK *pl_w );
static void chouhatsu_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			She rises, then drops back down						*/
/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w )
{
	static void ( *chouhatsu_jmp[])( PLAYER_WORK *pl_w ) = {
		chouhatsu_init, chouhatsu_move, chouhatsu_end,
	} ;

	chouhatsu_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void chouhatsu_init( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	lw->bat_mode = 7 ;
	pl_w->routine_2 ++ ;
	pls03_03_set( pl_w, 1 ) ;
	pl_w->timer0 = 60 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.y = 2.142857f ;
	pl_w->acc.y = -0.06696428f ;
	ef_color_set( pl_w, 0, win_color[pl_w->pl_color], 1 ) ;
	pl_char_set( pl_w, Char_Extra, 5 ) ;
}

/*--------------------------------------------------------------*/
static void chouhatsu_move( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	lw->bat_mode = 7 ;
	if( !--pl_w->timer0 ){
		lw->bat_mode = 0 ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_char_set( pl_w, Char_Jump, 9 ) ;
	} else {
		if( !pl_w->cg_prm ){
			pl_w->pos.y += pl_w->spd.y ;
			pl_w->spd.y += pl_w->acc.y ;
		}
		if( pl_w->spd.y < 0.0f ){
			pl_w->spd.y = 0.0f ;
			pl_w->acc.y = 0.0f ;
		}
		pl_char_move( pl_w ) ;
	}
}

/*==============================================================*/
static void chouhatsu_fall( PLAYER_WORK *pl_w )
{
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_3 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Jump, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void chouhatsu_land( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	if( pl_char_move( pl_w ) < 0 ){
		lw->bat_mode = 0 ;
		pl_stand_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void chouhatsu_end( PLAYER_WORK *pl_w )
{
	static void ( *chouhatsu_end_jmp[])( PLAYER_WORK *pl_w ) = {
		chouhatsu_fall, chouhatsu_land,
	} ;

	chouhatsu_end_jmp[pl_w->routine_3]( pl_w ) ;
}

static void silhouette_init( PLAYER_WORK *pl_w );
static void silhouette_wait( PLAYER_WORK *pl_w );
static void silhouette_move( PLAYER_WORK *pl_w );
static void silhouette_keep( PLAYER_WORK *pl_w );
static void silhouette_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo: she lasts until all its shots are	*/
/*			gone												*/
/*==============================================================*/
static void sp_silhouette( PLAYER_WORK *pl_w )
{
	static void ( *silhouette_jmp[])( PLAYER_WORK *pl_w ) = {
		silhouette_init, silhouette_wait, silhouette_move, silhouette_keep, silhouette_end,
	} ;
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	lw->bat_mode = 12 ;
	silhouette_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void silhouette_init( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	pl_w->routine_2 ++ ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->atck_no = pl03_at_silhouette ;
	HIT_RESET( pl_w ) ;
	sp_stand_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	lw->shot_cnt = 0 ;
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_char_set( pl_w, Char_Super, 41 ) ;
}

/*--------------------------------------------------------------*/
static void silhouette_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_char_move( pl_w ) ;
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	if( pl_w->cg_type ){
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->timer0 = 300 ;
		ofs.x = -DOT_X( 4.0f ) ;
		ofs.y = DOT_Y( 88.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void silhouette_move( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_char_set( pl_w, Char_Super, 42 ) ;
		return ;
	}
	if( pl_w->cg_prm ){
		pl_w->cg_prm = 0 ;
		if( shl03_02_set( pl_w ) ) lw->shot_cnt ++ ;
	}
}

/*--------------------------------------------------------------*/
static void silhouette_keep( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	lw->col_timer = 6 ;
	pl_char_move( pl_w ) ;
	if( !lw->shot_cnt ){
		pl_w->routine_2 ++ ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_char_set( pl_w, Char_Super, 43 ) ;
	}
}

/*--------------------------------------------------------------*/
static void silhouette_end( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	if( pl_char_move( pl_w ) < 0 ){
		lw->bat_mode = 0 ;
		pl_stand_ret( pl_w ) ;
	}
}

static void illusion( PLAYER_WORK *pl_w );
static void jillusion( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo: she dashes in and, if she catches	*/
/*			the opponent, hits them many times					*/
/*==============================================================*/
static void sp_illusion( PLAYER_WORK *pl_w )
{
	static void ( *sp_illusion_jmp[])( PLAYER_WORK *pl_w ) = {
		illusion, jillusion,
	} ;

	sp_illusion_jmp[pl_w->type]( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void illusion_wait( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->timer0 = 40 ;
		pl_w->spd.x = DOT_X( 8.0f ) ;
		pl_w->acc.x = -0.104166664f ;
		if( !pl_w->disp_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		pl_w->pl_stance = STANCE_AIR ;
		pl_char_set( pl_w, Char_Super, 6 ) ;
		lw->dash_ef_flag = 1 ;
		ofs.x = -DOT_X( 16.0f ) ;
		ofs.y = DOT_Y( 64.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*==============================================================*/
static void illusion_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_3 ++ ;
	sp_stand_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = pl03_at_illusion ;
	HIT_RESET( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_char_set( pl_w, Char_Super, 5 ) ;
	illusion_wait( pl_w ) ;
}

/*==============================================================*/
static void illusion_start( PLAYER_WORK *pl_w )
{
	static void ( *illusion_start_jmp[])( PLAYER_WORK *pl_w ) = {
		illusion_init, illusion_wait,
	} ;

	illusion_start_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void illusion_rush( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_w->hit_flag < 0 ){
		lw->dash_ef_flag = 0 ;
		pl_voice_req( pl_w, 43 ) ;
		if( !( pl_w->hit_flag & 0x7f ) && pl_sp_catch_ck( pl_w ) ){
			pl_w->routine_2 ++ ;
			pl_w->routine_3 = 0 ;
			scr_excep_set( pl_w, 5, 5 ) ;
			pl_w->pos.y = pl_w->ground_y ;
			lw->em_w = pl_w->em_w ;
			lw->double_flag = 0 ;
			lw->spark_no = 0 ;
			shl03_03_set( pl_w, 0 ) ;
			pl_char_set( pl_w, Char_Super, 15 ) ;
			pl_w->catch_no = 0xc5 ;
			pl_sp_catch_set( pl_w, pl_w->em_w ) ;
		} else {
			pl_w->pos.y = pl_w->ground_y ;
			pl_w->routine_3 = 1 ;
			pl_w->spd.x = DOT_X( 2.0f ) ;
			pl_w->acc.x = 0.0f ;
			pl_w->spd.y = DOT_Y( 8.0f ) ;
			pl_w->acc.y = -DOT_Y( 0.375f ) ;
			if( pl_w->disp_dir ){
				pl_w->spd.x = -pl_w->spd.x ;
				pl_w->acc.x = -pl_w->acc.x ;
			}
			pl_char_set( pl_w, Char_Super, 11 ) ;
		}
	} else if( !--pl_w->timer0 ){
		lw->dash_ef_flag = 0 ;
		pl_voice_req( pl_w, 43 ) ;
		pl_w->routine_2 = 3 ;
		pl_w->routine_3 = 0 ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x /= 2.0f ;
		pl_char_set( pl_w, Char_Super, 10 ) ;
	}
}

/*--------------------------------------------------------------*/
static void illusion_bound( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_2 = 3 ;
		pl_w->routine_3 = 0 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 12 ) ;
	}
}

/*--------------------------------------------------------------*/
static void illusion_dash( PLAYER_WORK *pl_w )
{
	static void ( *illusion_dash_jmp[])( PLAYER_WORK *pl_w ) = {
		illusion_rush, illusion_bound,
	} ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	illusion_dash_jmp[pl_w->routine_3]( pl_w ) ;
}

static Sint16 illusion_hit_ofs[6][4] = {
	{ -12288, 16384, -28672,  8192 },
	{ -12288,  8192, -28672, 16384 },
	{  -8192, 16384, -28672,  8192 },
	{      0,  8192, -16384, 16384 },
	{  -4096,  8192,  16384, 16384 },
	{      0, 16384,  28672,  8192 },
} ;

/*==============================================================*/
/*			Two hit sparks on the opponent, in one of six		*/
/*			pairs of places										*/
/*==============================================================*/
static void illusion_hit_ef( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;
	VEC3 ofs ;

	ofs.x = illusion_hit_ofs[lw->spark_no][0] * 1.6666666f / 256.0f ;
	ofs.y = illusion_hit_ofs[lw->spark_no][1] * 2.142857f / 256.0f ;
	catch_hit_mark_set( pl_w, &ofs ) ;
	ofs.x = illusion_hit_ofs[lw->spark_no][2] * 1.6666666f / 256.0f ;
	ofs.y = illusion_hit_ofs[lw->spark_no][3] * 2.142857f / 256.0f ;
	catch_hit_mark_set( pl_w, &ofs ) ;
	if( pl_w->cg_flag1 ){
		hit_se_req( pl_w->catch_w, pl_w->cg_flag1 + 0xff ) ;
		pl_w->cg_flag1 = 0 ;
	}
	switch( lw->spark_no ){
	case 0:
		lw->spark_no = 1 ;
		break ;
	case 1:
		lw->spark_no = 0 ;
		break ;
	case 2:
	case 3:
	case 4:
		lw->spark_no ++ ;
		break ;
	case 5:
		lw->spark_no = 0 ;
		break ;
	}
}

/*==============================================================*/
static void illusion_hold( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	if( lw->double_flag ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Super, 16 ) ;
		return ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void illusion_hits( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *em_w ;
	Uint8 no ;

	if( pl_char_move( pl_w ) < 0 ){
		lw->drop_flag = 0 ;
		pl_w->routine_3 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 16.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.25f ) ;
		pl_char_set( pl_w, Char_Super, 13 ) ;
		return ;
	}
	no = pl_w->cg_type ;
	if( no ){
		pl_w->cg_type = 0 ;
		if( no == 62 ){
			pls03_04_set( pl_w, 0 ) ;
			em_w = lw->em_w ;
			em_w->dm_hit_w = pl_w ;
			em_w->atck_no = no ;
			em_w->disp_dir = pl_w->disp_dir ;
			em_w->catch_type = 17 ;
			em_w->pl_stance = STANCE_AIR ;
			combo_hit( em_w, pl_w ) ;
		} else {
			em_w = lw->em_w ;
			em_w->dm_hit_w = pl_w ;
			em_w->atck_no = pl_w->atck_no = no ;
			catch_damage( pl_w ) ;
			combo_hit( em_w, pl_w ) ;
			catch_damage( pl_w ) ;
		}
		combo_hit( em_w, pl_w ) ;
		illusion_hit_ef( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void illusion_rise( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y > pl_w->ground_y + DOT_Y( 256.0f ) ){
		pl_w->pos.y = pl_w->ground_y + DOT_Y( 256.0f ) ;
		pl_w->routine_3 ++ ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void illusion_catch_up( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *em_w = lw->em_w ;

	if( pl_w->pos.y > em_w->pos.y ){
		pl_w->routine_3 ++ ;
		lw->drop_flag = -1 ;
		em_w->pos.y = pl_w->pos.y ;
		pl_char_set( pl_w, Char_Super, 17 ) ;
		pl_w->catch_no = 0xc5 ;
		pl_ef_catch_set( pl_w, em_w ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void illusion_air_hits( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *em_w = lw->em_w ;

	em_w->pos.y = pl_w->pos.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		if( pl_w->cg_type == 63 ){
			pl_w->routine_3 ++ ;
			pls03_04_set( pl_w, 2 ) ;
			em_w = pl_w->catch_w ;
			em_w->dm_hit_w = pl_w ;
			em_w->atck_no = pl03_at_illusion_air ;
			em_w->disp_dir = pl_w->disp_dir ;
			em_w->catch_type = 1 ;
			em_w->pl_stance = STANCE_AIR ;
			combo_hit( em_w, pl_w ) ;
			combo_hit( em_w, pl_w ) ;
			illusion_hit_ef( pl_w ) ;
			pl_char_set( pl_w, Char_Super, 18 ) ;
		} else {
			em_w = lw->em_w ;
			em_w->dm_hit_w = pl_w ;
			pl_w->atck_no = pl_w->cg_type ;
			em_w->atck_no = pl_w->cg_type ;
			catch_damage( pl_w ) ;
			combo_hit( em_w, pl_w ) ;
			catch_damage( pl_w ) ;
			combo_hit( em_w, pl_w ) ;
			pl_w->cg_type = 0 ;
		}
		illusion_hit_ef( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void illusion_finish( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		scr_excep_set( pl_w, 0, 0 ) ;
		pl_w->routine_3 ++ ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.x = DOT_X( 2.0f ) ;
		pl_w->spd.y = -DOT_Y( 2.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.5f ) ;
		if( pl_w->disp_dir ) pl_w->spd.x = -pl_w->spd.x ;
		pl_char_set( pl_w, Char_Super, 19 ) ;
	}
}

/*--------------------------------------------------------------*/
static void illusion_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_3 ++ ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 20 ) ;
	}
}

/*--------------------------------------------------------------*/
static void illusion_land( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	if( pl_char_move( pl_w ) < 0 ){
		lw->bat_mode = 0 ;
		pl_stand_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void illusion_hit( PLAYER_WORK *pl_w )
{
	static void ( *illusion_hit_jmp[])( PLAYER_WORK *pl_w ) = {
		illusion_hold, illusion_hits, illusion_rise, illusion_catch_up,
		illusion_air_hits, illusion_finish, illusion_fall, illusion_land,
	} ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	illusion_hit_jmp[pl_w->routine_3]( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void illusion_end( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;
	Float spd_x = pl_w->spd.x ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( ( pl_w->spd.x < 0.0f && spd_x > 0.0f ) || ( pl_w->spd.x > 0.0f && spd_x < 0.0f ) ){
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		lw->bat_mode = 0 ;
		pl_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
static void illusion( PLAYER_WORK *pl_w )
{
	static void ( *illusion_jmp[])( PLAYER_WORK *pl_w ) = {
		illusion_start, illusion_dash, illusion_hit, illusion_end,
	} ;

	illusion_jmp[pl_w->routine_2]( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void jillusion_wait( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->timer0 = 30 ;
		pl_w->spd.x = DOT_X( 8.0f ) ;
		pl_w->acc.x = -0.104166664f ;
		if( !pl_w->disp_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		pl_w->pl_stance = STANCE_AIR ;
		pl_char_set( pl_w, Char_Super, 7 ) ;
		lw->dash_ef_flag = 1 ;
		ofs.x = -DOT_X( 16.0f ) ;
		ofs.y = DOT_Y( 64.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*==============================================================*/
static void jillusion_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_3 ++ ;
	pl_dir_set( pl_w ) ;
	pl_w->atck_no = pl03_at_illusion ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 14 ) ;
	jillusion_wait( pl_w ) ;
}

/*==============================================================*/
static void jillusion_start( PLAYER_WORK *pl_w )
{
	static void ( *jillusion_start_jmp[])( PLAYER_WORK *pl_w ) = {
		jillusion_init, jillusion_wait,
	} ;

	jillusion_start_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void jillusion_rush( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *em_w ;

	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_w->hit_flag < 0 ){
		lw->dash_ef_flag = 0 ;
		pl_voice_req( pl_w, 43 ) ;
		if( !( pl_w->hit_flag & 0x7f ) ){
			em_w = pl_w->em_w ;
			if( pl_sp_catch_ck( pl_w ) ){
				em_w->pos.y = pl_w->pos.y ;
				pl_w->routine_2 ++ ;
				pl_w->routine_3 = 0 ;
				scr_excep_set( pl_w, 5, 5 ) ;
				lw->em_w = pl_w->em_w ;
				lw->double_flag = 0 ;
				lw->spark_no = 0 ;
				shl03_03_set( pl_w, 1 ) ;
				pl_char_set( pl_w, Char_Super, 21 ) ;
				pl_w->catch_no = 0xc5 ;
				pl_sp_catch_set( pl_w, pl_w->em_w ) ;
				return ;
			}
		}
		pl_w->routine_3 = 1 ;
		pl_w->spd.x = DOT_X( 2.0f ) ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 8.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		if( pl_w->disp_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		pl_char_set( pl_w, Char_Super, 11 ) ;
	} else if( !--pl_w->timer0 ){
		pl_w->routine_3 ++ ;
		lw->dash_ef_flag = 0 ;
		pl_voice_req( pl_w, 43 ) ;
		pl_w->spd.x /= 2.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = -2.142857f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_char_set( pl_w, Char_Jump, 9 ) ;
	}
}

/*--------------------------------------------------------------*/
static void jillusion_bound( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_2 = 3 ;
		pl_w->routine_3 = 0 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 12 ) ;
	}
}

/*--------------------------------------------------------------*/
static void jillusion_dash( PLAYER_WORK *pl_w )
{
	static void ( *jillusion_dash_jmp[])( PLAYER_WORK *pl_w ) = {
		jillusion_rush, jillusion_bound,
	} ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	jillusion_dash_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void jillusion_hold( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	pl_char_move( pl_w ) ;
	if( lw->double_flag ){
		pl_w->routine_3 ++ ;
		lw->release_flag = 0 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.y = DOT_Y( 0.25f ) ;
		pl_char_set( pl_w, Char_Super, 22 ) ;
	}
}

/*--------------------------------------------------------------*/
static void jillusion_hits( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *em_w ;
	Uint8 no ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 = pl_w->routine_3 + 1 ;
		pl_char_set( pl_w, Char_Super, 23 ) ;
		return ;
	}
	no = pl_w->cg_type ;
	pl_w->cg_type = 0 ;
	if( no ){
		if( no == 66 ){
			em_w = lw->em_w ;
			em_w->dm_hit_w = pl_w ;
			em_w->disp_dir = pl_w->disp_dir ;
			em_w->catch_type = 17 ;
			em_w->pl_stance = STANCE_AIR ;
			em_w->atck_no = no ;
			lw->release_flag = -1 ;
			scr_excep_set( pl_w, 12, 12 ) ;
			pls03_04_set( pl_w, 0 ) ;
			combo_hit( em_w, pl_w ) ;
			combo_hit( em_w, pl_w ) ;
			illusion_hit_ef( pl_w ) ;
			return ;
		}
		em_w = lw->em_w ;
		em_w->dm_hit_w = pl_w ;
		em_w->atck_no = pl_w->atck_no = no ;
		catch_damage( pl_w ) ;
		combo_hit( em_w, pl_w ) ;
		catch_damage( pl_w ) ;
		combo_hit( em_w, pl_w ) ;
		illusion_hit_ef( pl_w ) ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !lw->release_flag ){
		em_w = lw->em_w ;
		em_w->pos.y = pl_w->pos.y ;
	}
}

/*--------------------------------------------------------------*/
static void jillusion_turn( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *em_w ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		scr_excep_set( pl_w, 0, 0 ) ;
		pl_w->disp_dir = pl_w->disp_dir ^ 1 ;
		pl_char_set( pl_w, Char_Super, 24 ) ;
		return ;
	}
	if( HI_BYTE( pl_w->cg_rival ) ){
		HI_BYTE( pl_w->cg_rival ) = 0 ;
		pl_w->catch_no = 0xc5 ;
		pl_ef_catch_set( pl_w, lw->em_w ) ;
		em_w = lw->em_w ;
		combo_hit( em_w, pl_w ) ;
		combo_hit( em_w, pl_w ) ;
		lw->spark_no = 2 ;
	}
	if( pl_w->cg_type ){
		em_w = lw->em_w ;
		em_w->dm_hit_w = pl_w ;
		em_w->atck_no = pl_w->cg_type ;
		pl_w->atck_no = pl_w->cg_type ;
		pl_w->cg_type = 0 ;
		catch_damage( pl_w ) ;
		combo_hit( em_w, pl_w ) ;
		catch_damage( pl_w ) ;
		combo_hit( em_w, pl_w ) ;
		illusion_hit_ef( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void jillusion_finish( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *em_w ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_3 ++ ;
		pl_w->spd.x = DOT_X( 2.0f ) ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = -DOT_Y( 2.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.5f ) ;
		if( pl_w->disp_dir ) pl_w->spd.x = -pl_w->spd.x ;
		pl_char_set( pl_w, Char_Super, 19 ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pls03_04_set( pl_w, 2 ) ;
		em_w = lw->em_w ;
		em_w->dm_hit_w = pl_w ;
		em_w->disp_dir = pl_w->disp_dir ;
		em_w->catch_type = 1 ;
		em_w->pl_stance = STANCE_AIR ;
		em_w->atck_no = pl03_at_jillusion_end ;
		combo_hit( em_w, pl_w ) ;
		combo_hit( em_w, pl_w ) ;
		illusion_hit_ef( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void jillusion_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_3 ++ ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 20 ) ;
	}
}

/*--------------------------------------------------------------*/
static void jillusion_land( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	if( pl_char_move( pl_w ) < 0 ){
		lw->bat_mode = 0 ;
		pl_stand_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void jillusion_hit( PLAYER_WORK *pl_w )
{
	static void ( *jillusion_hit_jmp[])( PLAYER_WORK *pl_w ) = {
		jillusion_hold, jillusion_hits, jillusion_turn,
		jillusion_finish, jillusion_fall, jillusion_land,
	} ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	jillusion_hit_jmp[pl_w->routine_3]( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void jillusion_end( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;
	Float spd_x = pl_w->spd.x ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( ( pl_w->spd.x < 0.0f && spd_x > 0.0f ) || ( pl_w->spd.x > 0.0f && spd_x < 0.0f ) ){
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		lw->bat_mode = 0 ;
		pl_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
static void jillusion( PLAYER_WORK *pl_w )
{
	static void ( *jillusion_jmp[])( PLAYER_WORK *pl_w ) = {
		jillusion_start, jillusion_dash, jillusion_hit, jillusion_end,
	} ;

	jillusion_jmp[pl_w->routine_2]( pl_w ) ;
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
	pl_w->atck_no = pl03_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 17 ) ;
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
		ofs.x = -DOT_X( 32.0f ) ;
		ofs.y = DOT_Y( 64.0f ) ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
	if( pl_w->cg_flag1 ) pl_se_req( pl_w, SE_SWING_H ) ;
}

/*==============================================================*/
static void shell_init( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	lw->kick_cnt = 0 ;
	pl_w->routine_2 ++ ;
	pl_w->sjump_flag = 0 ;
	pl_w->char_no.b.no = 4 ;
	pl_w->atck_no = pl03_at_shell ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y /= 2.0f ;
	pl_w->acc.y = -0.4017857f ;
	pl_se_req( pl_w, SE_SWING_H ) ;
	if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_hk_tbl ;
	else pl_w->chain_tbl = sj_hk_tbl ;
	pl_w->nml_level = 2 ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Jump_K, pl_w->char_no.b.no ) ;
	if( pl_w->jump_atk_cnt & JATK_K_MASK ) pl_w->jump_atk_cnt -= 0x10 ;
}

/*==============================================================*/
/*			Each hit that lands lets her kick again, twice		*/
/*			at most												*/
/*==============================================================*/
static void shell_move( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	pl_char_move( pl_w ) ;
	pl_jump_move( pl_w ) ;
	pl_sjump_top_ck( pl_w ) ;
	if( pl_land_ck( pl_w ) ){
		pl_land_set( pl_w ) ;
		return ;
	}
	if( lw->kick_cnt < 2 ){
		if( pl_w->hit_flag ){
			if( pl_w->cg_type ){
				pl_w->cg_type = 0 ;
				lw->kick_cnt ++ ;
				pl_w->atck_no = lw->kick_cnt + 23 ;
				HIT_RESET( pl_w ) ;
			}
		}
	}
}

/*==============================================================*/
/*			A jump kick in the air that she can repeat			*/
/*==============================================================*/
static void sp_shell( PLAYER_WORK *pl_w )
{
	static void ( *shell_jmp[])( PLAYER_WORK *pl_w ) = {
		shell_init, shell_move,
	} ;

	shell_jmp[pl_w->routine_2]( pl_w ) ;
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_move( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A jump straight after a throw is escaped			*/
/****************************************************************/
void pl03_t_kick( PLAYER_WORK *pl_w )
{
	static void ( *t_kick_jmp[])( PLAYER_WORK *pl_w ) = {
		t_kick_init, t_kick_move, t_kick_land,
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
	pl_w->atck_no = pl03_at_t_kick ;
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
		pl_char_set( pl_w, Char_Extra, 1 ) ;
		pl_land_ef_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void t_kick_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stop_stand_ret( pl_w ) ;
	else if( pl_w->cg_type ) pl_w->cg_type = 0 ;
}

/****************************************************************/
/*			Can a throw start?									*/
/****************************************************************/
PLAYER_WORK *pl03_catch_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( !( pl_w->atck_lever & ( KEY_FORWARD | KEY_BACK ) ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_CROUCH ) return 0 ;
	if( !pl_w->atck_level ) return 0 ;
	if( pl_w->atck_pk ){
		if( pl_w->pl_stance == STANCE_AIR ) return 0 ;
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 3 ;
			return em ;
		}
		return 0 ;
	}
	if( pl_w->pl_stance == STANCE_AIR ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 2 ;
			return em ;
		}
		return 0 ;
	}
	if( ( em = pl_catch_ck( pl_w ) ) ){
		pl_w->catch_no = 0 ;
		return em ;
	}
	return 0 ;
}

static void nml_p_set( PLAYER_WORK *pl_w );
static void vector_set( PLAYER_WORK *pl_w );
static void jump_p_set( PLAYER_WORK *pl_w );
static void nml_k_set( PLAYER_WORK *pl_w );
static void catch_dummy( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Start the throw that pl03_catch_ck chose			*/
/****************************************************************/
void pl03_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_set, vector_set, jump_p_set, nml_k_set, catch_dummy, catch_dummy,
	} ;

	catch_set_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_w->atck_lever & KEY_BACK ){
		pl_w->pl_dir = pl_w->pl_dir ^ 1 ;
		pl_w->disp_dir = pl_w->disp_dir ^ 1 ;
	}
	pl_w->hit_stop = 10 ;
	pl_w->routine_2 = 0 ;
	scr_excep_set( pl_w, 5, 5 ) ;
	ofs.x = -DOT_X( 32.0f ) ;
	ofs.y = DOT_Y( 64.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 0 ) ;
}

/*==============================================================*/
static void vector_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->hit_stop = 10 ;
	pl_w->routine_2 = 0 ;
	ofs.x = -DOT_X( 32.0f ) ;
	ofs.y = DOT_Y( 64.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_char_set( pl_w, Char_Catch, 1 ) ;
}

/*==============================================================*/
static void jump_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->hit_stop = 10 ;
	pl_w->routine_2 = 0 ;
	ofs.x = -DOT_X( 32.0f ) ;
	ofs.y = DOT_Y( 64.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	scr_excep_set( pl_w, 5, 5 ) ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 8 ) ;
}

/*==============================================================*/
static void nml_k_set( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;
	VEC3 ofs ;

	if( pl_w->atck_lever & KEY_BACK ){
		pl_w->pl_dir = pl_w->pl_dir ^ 1 ;
		pl_w->disp_dir = pl_w->disp_dir ^ 1 ;
	}
	scr_excep_set( pl_w, 5, 5 ) ;
	pl_w->hit_stop = 10 ;
	pl_w->routine_2 = 0 ;
	ofs.x = -DOT_X( 32.0f ) ;
	ofs.y = DOT_Y( 64.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	lw->leave_flag = 0 ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 7 ) ;
}

/*==============================================================*/
static void catch_dummy( PLAYER_WORK *pl_w )
{
}

static void nml_p_catch( PLAYER_WORK *pl_w );
static void vector_catch( PLAYER_WORK *pl_w );
static void jump_p_catch( PLAYER_WORK *pl_w );
static void nml_k_catch( PLAYER_WORK *pl_w );
static void illusion_catch( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void pl03_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_catch, vector_catch, jump_p_catch, nml_k_catch, illusion_catch, illusion_catch,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->disp_dir = pl_w->disp_dir ^ 1 ;
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		em_w = pl_w->catch_w ;
		em_w->dm_hit_w = pl_w ;
		em_w->catch_type = 1 ;
		em_w->pl_stance = STANCE_AIR ;
		em_w->atck_no = pl03_at_throw_p ;
		em_w->pl_dir = pl_w->pl_dir ;
		scr_excep_set( pl_w, 0, 0 ) ;
		catch_se_req( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void vector_catch_land( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_land_ck( pl_w ) ) pl_land_set( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void vector_catch_leave( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 10.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 10.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.5f ) ;
		if( pl_w->disp_dir ) pl_w->spd.x = -pl_w->spd.x ;
		pl_char_set( pl_w, Char_Catch, 4 ) ;
		em_w = pl_w->catch_w ;
		em_w->dm_hit_w = pl_w ;
		em_w->catch_type = 2 ;
		em_w->atck_no = pl03_at_vector ;
		em_w->pl_dir = pl_w->pl_dir ^ 1 ;
		scr_excep_set( pl_w, 0, 0 ) ;
		catch_se_req( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void vector_catch_fall( PLAYER_WORK *pl_w )
{
	Float spd_x ;

	pl_char_move( pl_w ) ;
	spd_x = pl_w->spd.x ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( ( pl_w->spd.x > 0.0f && spd_x < 0.0f ) || ( pl_w->spd.x < 0.0f && spd_x > 0.0f ) ){
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
	}
	if( pl_w->cg_flag1 ){
		pl_w->cg_flag1 = 0 ;
		pl_se_req( pl_w, SE_SWING_H ) ;
	}
	if( lever_gacha_ck( pl_w ) ){
		if( !pl_w->cpu_flag ){
			if( pl_w->cg_type ){
				pl_w->cg_type = 0 ;
				pl_w->cg_ctr = 1 ;
			}
		} else if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			if( pl_w->cg_ctr != 1 ) pl_w->cg_ctr -- ;
		}
	}
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Catch, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void vector_catch_fly( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
	if( pl_w->spd.y < 0.0f ){
		pl_w->acc.x = pl_w->acc.y = 0.0f ;
		pl_w->spd.x = DOT_X( 0.5f ) ;
		pl_w->spd.y = DOT_Y( 0.5f ) ;
		if( !pl_w->disp_dir ) pl_w->spd.x = -pl_w->spd.x ;
	}
	if( !--pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = DOT_X( 4.0f ) ;
		pl_w->acc.x = -0.104166664f ;
		pl_w->spd.y = DOT_Y( 6.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.25f ) ;
		if( pl_w->disp_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		lw->dash_ef_flag = 0 ;
		pl_voice_req( pl_w, 43 ) ;
		pl_char_set( pl_w, Char_Catch, 2 ) ;
	}
}

/*==============================================================*/
static void vector_catch_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.x = DOT_X( 2.0f ) ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 8.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.125f ) ;
	if( !pl_w->disp_dir ) pl_w->spd.x = -pl_w->spd.x ;
	pl_w->timer0 = 90 ;
	vector_catch_fly( pl_w ) ;
}

/*==============================================================*/
/*			The flying grab of sp_vector							*/
/*==============================================================*/
static void vector_catch( PLAYER_WORK *pl_w )
{
	static void ( *vector_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		vector_catch_init, vector_catch_fly, vector_catch_fall, vector_catch_leave, vector_catch_land,
	} ;

	vector_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void jump_p_catch_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void jump_p_catch_land( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Jump, 5 ) ;
	}
}

/*--------------------------------------------------------------*/
static void jump_p_catch_throw( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		scr_excep_set( pl_w, 0, 0 ) ;
		em_w = pl_w->catch_w ;
		em_w->dm_hit_w = pl_w ;
		em_w->catch_type = 2 ;
		em_w->atck_no = pl03_at_air_throw_p ;
		pl_w->pl_dir = pl_w->disp_dir ;
		em_w->disp_dir = pl_w->pl_dir ^ 1 ;
		em_w->pl_dir = em_w->disp_dir ;
		pl_w->spd.x = 10.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 8.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.25f ) ;
		if( pl_w->disp_dir ) pl_w->spd.x = -pl_w->spd.x ;
		pl_char_set( pl_w, Char_Jump, 2 ) ;
		catch_se_req( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void jump_p_catch_drop( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_prm ){
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Catch, 9 ) ;
	}
}

/*==============================================================*/
static void jump_p_catch_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.y = -DOT_Y( 4.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.5f ) ;
	jump_p_catch_drop( pl_w ) ;
}

/*==============================================================*/
/*			The throw in the air: she drops with the			*/
/*			opponent											*/
/*==============================================================*/
static void jump_p_catch( PLAYER_WORK *pl_w )
{
	static void ( *jump_p_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		jump_p_catch_init, jump_p_catch_drop, jump_p_catch_throw, jump_p_catch_land,
		jump_p_catch_end,
	} ;

	jump_p_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void nml_k_catch_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		scr_excep_set( pl_w, 0, 0 ) ;
		pl_stand_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void nml_k_catch_miss( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_char_set( pl_w, Char_Extra, 2 ) ;
}

/*--------------------------------------------------------------*/
static void nml_k_catch_wait( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void nml_k_catch_start( PLAYER_WORK *pl_w )
{
	void *ew ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		return ;
	}
	if( pl_w->cg_flag1 ){
		pl_w->cg_flag1 = 0 ;
		if( ( ew = pls03_01_set( pl_w ) ) ) pl_catch_pass( pl_w, ew ) ;
		else {
			scr_excep_set( pl_w, 0, 0 ) ;
			pl_stand_ret( pl_w ) ;
		}
	}
}

/*==============================================================*/
static void nml_k_catch( PLAYER_WORK *pl_w )
{
	static void ( *nml_k_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_k_catch_start, nml_k_catch_wait, nml_k_catch_miss, nml_k_catch_end,
	} ;

	nml_k_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void illusion_catch( PLAYER_WORK *pl_w )
{
	pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void nml_p_hold( PLAYER_WORK *pl_w )
{
	catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/*==============================================================*/
static void jump_p_hold( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w = pl_w->catch_w ;

	if( HI_BYTE( em_w->cg_rival ) ){
		if( em_w->cg_prm ) catch_pos_set( pl_w->catch_w, pl_w ) ;
		else catch_em_pos_set0( pl_w->catch_w, pl_w ) ;
	}
}

/*==============================================================*/
static void nml_k_hold( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w = pl_w->catch_w ;

	if( !em_w->cg_prm ) catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/*==============================================================*/
static void hold_dummy( PLAYER_WORK *pl_w )
{
}

/*==============================================================*/
static void illusion_catch_hold( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w = pl_w->catch_w ;

	if( !em_w->cg_prm ) catch_pos_set( pl_w->catch_w, pl_w ) ;
	else catch_em_pos_set0( pl_w->catch_w, pl_w ) ;
}

/****************************************************************/
/*			The opponent's side of a throw						*/
/****************************************************************/
void pl03_hold( PLAYER_WORK *pl_w )
{
	static void ( *hold_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_hold, nml_p_hold, jump_p_hold, nml_k_hold, hold_dummy, illusion_catch_hold,
	} ;

	hold_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/****************************************************************/
void pl03_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl03_sp_silhouette ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl03_sp_shower ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl03_sp_silhouette ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl03_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl03_sp_silhouette ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl03_sp_shower ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl03_sp_silhouette ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl03_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl03_sp_blade ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl03_sp_soul ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl03_sp_soul ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void pl03_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl03_sp_blade ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl03_sp_soul ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl03_sp_blade ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
/*			Clear the work, all but bat_mode					*/
/****************************************************************/
void pl03_tlocal_init( PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;
	Uint8 keep = lw->bat_mode ;

	memset( pl_w->tlocal, 0, sizeof( pl_w->tlocal ) ) ;
	lw->bat_mode = keep ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl03.c" End of File	*/

/* SonSon (character 14): command checks, normal attacks, appearance, win and lose poses, special
   moves and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "burst_ef_old.h"
#include "pl14_def.h"

/* s_pl14 declared this check as an int, where pl_main.c reads a Uint8 */
extern int pl_nml_atck_ck( PLAYER_WORK *pl_w );

/* the routine returns a Uint8; this module called it as an int */
extern int pl_nml_atck_ck( PLAYER_WORK *pl_w );

void pl14_main( PLAYER_WORK *pl_w );
void pl14_com_ck( PLAYER_WORK *pl_w );
void pl14_atck_set( PLAYER_WORK *pl_w );
void pl14_atck( PLAYER_WORK *pl_w );
void pl14_dash( PLAYER_WORK *pl_w );
void pl14_noki( PLAYER_WORK *pl_w );
void pl14_appear( PLAYER_WORK *pl_w );
void pl14_win( PLAYER_WORK *pl_w );
void pl14_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl14_catch_ck( PLAYER_WORK *pl_w );
void pl14_catch_set( PLAYER_WORK *pl_w );
void pl14_catch( PLAYER_WORK *pl_w );
void pl14_hold( PLAYER_WORK *pl_w );
int pl14_delay_ck( PLAYER_WORK *pl_w );
void pl14_t_kick( PLAYER_WORK *pl_w );
void pl14_upper( PLAYER_WORK *pl_w );
void pl14_sp_unison( PLAYER_WORK *pl_w );
void pl14_sp_gattai( PLAYER_WORK *pl_w );
void pl14_sp_assist( PLAYER_WORK *pl_w );
void pl14_sp_zero( PLAYER_WORK *pl_w );
void pl14_air_dash( PLAYER_WORK *pl_w );
void pl14_tlocal_init( PLAYER_WORK *pl_w );

/* sp_pow: which effect to fire and ride, picked at random */
static Uint8 pow_ride_tbl[8] = { 24, 24, 24, 20, 20, 20, 16, 16 } ;

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
static Uint16 com_shienbu[] = { 3, 0, pl14_sp_shienbu, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Shienbu */
static Uint16 com_fuusetsu[] = { 4, 10, pl14_sp_fuusetsu, 0, KEY_PUNCH } ;	/* Fuusetsu Zan */
static Uint16 com_seiten[] = { 3, 0, pl14_sp_seiten, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_PUNCH } ;	/* Seiten Rengeki */
static Uint16 com_hofuku[] = { 5, COM_GROUND | COM_ATCK_NG, pl14_sp_hofuku, 0, KEY_BACK, KEY_DOWN | KEY_BACK, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Hofuku Zenshin */
static Uint16 com_tenchi[] = { 3, COM_HYPER | COM_ALL_SW, pl14_sp_tenchi, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Tenchi Tsuukan (hyper) */
static Uint16 com_hisago[] = { 5, COM_GROUND, pl14_sp_hisago, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Kingin no Hisago */
static Uint16 com_pow[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl14_sp_pow, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Pow (hyper) */
static Uint16 com_enou[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl14_sp_enou, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* En'Ou (hyper) */
static Uint16 com_wall[] = { 3, COM_GROUND, pl14_sp_wall, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Wall Walk */

static void ( *pl14_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl14_main,
	pl14_com_ck,
	pl14_atck_set,
	pl14_atck,
	pl14_dash,
	pl14_noki,
	pl14_appear,
	pl14_win,
	pl14_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl14_catch_ck,	/* returns a value */
	pl14_catch_set,
	pl14_catch,
	pl14_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl14_delay_ck,
	pl14_t_kick,
	pl14_upper,
	pl14_sp_unison,
	pl14_sp_gattai,
	pl14_sp_assist,
	pl14_sp_zero,
	0,
	pl14_air_dash,
	0,
	pl14_tlocal_init,
} ;

static Uint8 shienbu_ck( PLAYER_WORK *pl_w );
static Uint8 fuusetsu_ck( PLAYER_WORK *pl_w );
static Uint8 seiten_ck( PLAYER_WORK *pl_w );
static Uint8 hofuku_ck( PLAYER_WORK *pl_w );
static Uint8 tenchi_ck( PLAYER_WORK *pl_w );
static Uint8 hisago_ck( PLAYER_WORK *pl_w );
static Uint8 pow_ck( PLAYER_WORK *pl_w );
static Uint8 enou_ck( PLAYER_WORK *pl_w );
static Uint8 wall_ck( PLAYER_WORK *pl_w );
static int chouhatsu_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static int tenchi_delay_ck( PLAYER_WORK *pl_w );
static int pow_delay_ck( PLAYER_WORK *pl_w );
static int enou_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl14_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl14_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl14_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( tenchi_ck( pl_w ) ) return ;
	if( pow_ck( pl_w ) ) return ;
	if( enou_ck( pl_w ) ) return ;
	if( seiten_ck( pl_w ) ) return ;
	if( shienbu_ck( pl_w ) ) return ;
	if( fuusetsu_ck( pl_w ) ) return ;
	if( hofuku_ck( pl_w ) ) return ;
	if( hisago_ck( pl_w ) ) return ;
	if( wall_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( pl_air_dash_ck( pl_w, &pl_w->com_w6 ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
static Uint8 shienbu_ck( PLAYER_WORK *pl_w )
{
	PL14_LOCAL_W *lw ;

	if( !com_nml_ck( pl_w, com_shienbu, &pl_w->com_w ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	lw = ( PL14_LOCAL_W * )pl_w->tlocal ;
	if( lw->shot_cnt ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl14_sp_shienbu ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 fuusetsu_ck( PLAYER_WORK *pl_w )
{
	if( !com_renda_ck( pl_w, com_fuusetsu, &pl_w->com_w0 ) ) return 0 ;
	if( !sp_ok_ck( pl_w ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl14_sp_fuusetsu ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 seiten_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_seiten, &pl_w->com_w1 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl14_sp_seiten ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 hofuku_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_hofuku, &pl_w->com_w2 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl14_sp_hofuku ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 tenchi_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_tenchi, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w3 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl14_sp_tenchi ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 hisago_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_hisago, &pl_w->com_w4 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w4 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl14_sp_hisago ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 pow_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_pow, &pl_w->com_w5 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w5 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl14_sp_pow ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
/*			Has no return value at its end, as in the			*/
/*			arcade's code										*/
/*==============================================================*/
static Uint8 enou_ck( PLAYER_WORK *pl_w )
{
	PL14_LOCAL_W *lw ;

	if( !com_nml_ck( pl_w, com_enou, &pl_w->com_w7 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w7 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl14_sp_enou ;
	act_st_set( pl_w, N_SUPER ) ;
	lw = ( PL14_LOCAL_W * )pl_w->tlocal ;
	lw->monkey_timer = 240 ;
	if( pl_w->cpu_flag ) pl_w->pl_sp_type = pl14_sp_enou_auto ;
}

/*==============================================================*/
static Uint8 wall_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_wall, &pl_w->com_w8 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w8 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl14_sp_wall ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 8 ) ) return 0 ;
	pl_w->pl_sp_type = pl14_sp_chouhatsu ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static int snapback_ck( PLAYER_WORK *pl_w )
{
	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl14_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl14_delay_ck( PLAYER_WORK *pl_w )
{
	if( tenchi_delay_ck( pl_w ) || pow_delay_ck( pl_w ) || enou_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static int tenchi_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_tenchi, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 4 ;
	return 1 ;
}

/*==============================================================*/
static int pow_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_pow, &pl_w->com_w5 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 6 ;
	return 1 ;
}

/*==============================================================*/
static int enou_delay_ck( PLAYER_WORK *pl_w )
{
	PL14_LOCAL_W *lw ;

	if( !com_nml_ck( pl_w, com_enou, &pl_w->com_w6 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 7 ;
	lw = ( PL14_LOCAL_W * )pl_w->tlocal ;
	lw->monkey_timer = 240 ;
	return 1 ;
}

/****************************************************************/
void pl14_main( PLAYER_WORK *pl_w )
{
	if( pl_w->act_st == N_SUPER ){
		if( pl_w->pl_sp_type != pl14_sp_enou || pl_w->pl_sp_type != pl14_sp_enou_auto ){
			if( !pl_w->hit_stop ) pl_w->armor_flag = 0 ;
		}
		if( pl_w->pl_sp_type == pl14_sp_pow ) return ;
	} else {
		pl_w->armor_flag = 0 ;
		if( pl_w->act_st != N_CAUGHT || pl_w->catch_no != 0xc3 ){
			pl_w->scale.x = 1.0f ;
			pl_w->scale.y = 1.0f ;
			pl_w->rot[2] = 0 ;
		}
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
void pl14_atck_set( PLAYER_WORK *pl_w )
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
		pl_char_set( pl_w, Char_Stand_P, pl_w->char_no.b.no ) ;
		pls14_02_set( pl_w, 0, 2 ) ;
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
		pl_char_set( pl_w, Char_Stand_P, pl_w->char_no.b.no ) ;
		pls14_02_set( pl_w, 0, 2 ) ;
		break ;
	case 2:
		if( pl_w->atck_lever & KEY_FORWARD ){
			pl_w->char_no.b.no = 3 ;
			pl_w->atck_no = pl14_at_fwd_hp ;
			pl_char_set( pl_w, Char_Stand_P, pl_w->char_no.b.no ) ;
		} else {
			pl_w->char_no.b.no = 2 ;
			pl_w->atck_no = ATCK_STAND_HP ;
			pl_char_set( pl_w, Char_Stand_P, pl_w->char_no.b.no ) ;
			pls14_01_set( pl_w, 4, 0 ) ;
			pls14_01_set( pl_w, 5, 0 ) ;
		}
		pl_se_req( pl_w, SE_SWING_H ) ;
		pl_w->chain_tbl = hp_tbl ;
		pl_w->nml_level = 2 ;
		pls14_02_set( pl_w, 0, 2 ) ;
		break ;
	}
	HIT_RESET( pl_w ) ;
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
		pl_char_set( pl_w, Char_Stand_K, pl_w->char_no.b.no ) ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_STAND_MK ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_w->chain_tbl = mk_tbl ;
		pl_w->nml_level = 1 ;
		pl_char_set( pl_w, Char_Stand_K, pl_w->char_no.b.no ) ;
		pls14_02_set( pl_w, 0, 2 ) ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_STAND_HK ;
		pl_se_req( pl_w, SE_SWING_H ) ;
		pl_w->chain_tbl = hk_tbl ;
		pl_w->nml_level = 2 ;
		pl_char_set( pl_w, Char_Stand_K, pl_w->char_no.b.no ) ;
		break ;
	}
	HIT_RESET( pl_w ) ;
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
		pl_char_set( pl_w, Char_Jump_P, pl_w->char_no.b.no ) ;
		pls14_02_set( pl_w, 0, 2 ) ;
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
		pl_char_set( pl_w, Char_Jump_P, pl_w->char_no.b.no ) ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_JUMP_HP ;
		pl_se_req( pl_w, SE_SWING_H ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_hp_tbl ;
		else pl_w->chain_tbl = sj_hp_tbl ;
		pl_w->nml_level = 2 ;
		pl_char_set( pl_w, Char_Jump_P, pl_w->char_no.b.no ) ;
		break ;
	}
	HIT_RESET( pl_w ) ;
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
static void sjump_move( PLAYER_WORK *pl_w );
static void jump_move( PLAYER_WORK *pl_w );
static void jump_punch_move( PLAYER_WORK *pl_w );
static void jump_kick_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl14_atck( PLAYER_WORK *pl_w )
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
	case 2:
		if( pl_char_move( pl_w ) < 0 ){
			pl_stand_ret( pl_w ) ;
			return ;
		}
		if( pl_w->cg_type ){
			pl_w->pos.x += pl_w->pl_dir ? DOT_X( pl_w->cg_type ) : -DOT_X( pl_w->cg_type ) ;
			pl_w->cg_type = 0 ;
		}
		break ;
	case 1:
		if( pl_char_move( pl_w ) < 0 ){
			pl_stand_ret( pl_w ) ;
			return ;
		}
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pl_w->atck_no = pl14_at_mp_2 ;
			HIT_RESET( pl_w ) ;
		}
		break ;
	case 0:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
static void crouch_punch_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 2:
		if( pl_char_move( pl_w ) < 0 ){
			pl_stand_ret( pl_w ) ;
			return ;
		}
		if( pl_w->cg_type ){
			pl_w->pos.x += pl_w->pl_dir ? DOT_X( pl_w->cg_type ) : -DOT_X( pl_w->cg_type ) ;
			pl_w->cg_type = 0 ;
		}
		break ;
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	case 0:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
static void stand_kick_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 2:
		if( pl_char_move( pl_w ) < 0 ){
			pl_stand_ret( pl_w ) ;
			return ;
		}
		if( pl_w->cg_type ){
			pl_w->pos.x += pl_w->pl_dir ? DOT_X( pl_w->cg_type ) : -DOT_X( pl_w->cg_type ) ;
			pl_w->cg_type = 0 ;
		}
		if( pl_w->cg_flag1 ){
			pl_w->cg_flag1 = 0 ;
			pl_w->atck_no = ATCK_STAND_HK ;
			HIT_RESET( pl_w ) ;
		}
		break ;
	case 1:
		if( pl_char_move( pl_w ) < 0 ){
			pl_stand_ret( pl_w ) ;
			return ;
		}
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pl_w->atck_no = pl14_at_mk_2 ;
			HIT_RESET( pl_w ) ;
		}
		break ;
	case 0:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
static void crouch_kick_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 1:
	case 0:
	case 2:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
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
/*			The heavy punch hits again as the animation says	*/
/*==============================================================*/
static void jump_punch_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 2:
		if( pl_char_move( pl_w ) < 0 ){
			pl_jump_ret( pl_w ) ;
			return ;
		}
		if( pl_w->cg_flag1 ){
			if( pl_w->cg_flag1 == 1 ) pl_w->atck_no = ATCK_JUMP_HP ;
			HIT_RESET( pl_w ) ;
			if( pl_w->cg_flag1 == 2 ) pl_w->atck_no = pl14_at_jump_hp_2 ;
			HIT_RESET( pl_w ) ;
			pl_w->cg_flag1 = 0 ;
		}
		break ;
	case 0:
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
static void jump_kick_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/****************************************************************/
/*			The launcher										*/
/****************************************************************/
void pl14_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_w->atck_no = pl14_at_upper ;
		HIT_RESET( pl_w ) ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Extra, 3 ) ;
		pl_se_req( pl_w, SE_SWING_H ) ;
		pl_gauge_add( pl_w, 5 ) ;
		pls14_02_set( pl_w, 0, 2 ) ;
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
void pl14_dash( PLAYER_WORK *pl_w )
{
	static void ( *dash_jmp[])( PLAYER_WORK *pl_w ) = {
		dash_init, dash_move, dash_end,
	} ;

	dash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			A hop forward, set off as the animation says		*/
/*==============================================================*/
static void dash_init( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 9.5f ) : -DOT_X( 9.5f ) ;
		pl_w->pos.x += pl_w->pl_dir ? DOT_X( 16.0f ) : -DOT_X( 16.0f ) ;
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
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? 1.6666666f : -1.6666666f ;
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
void pl14_noki( PLAYER_WORK *pl_w )
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
		pl_w->routine_2 ++ ;
		pl_w->spd.x = DOT_X( 9.5f ) ;
		pl_w->acc.x = -0.3125f ;
		pl_w->spd.y = DOT_Y( 3.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.25f ) ;
		if( pl_w->disp_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		pl_w->timer0 = 16 ;
		noki_jump( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void noki_jump( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->ground_y >= pl_w->pos.y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = DOT_X( 2.5f ) ;
		pl_w->acc.x = -0.3255208f ;
		if( pl_w->disp_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		pl_char_set( pl_w, Char_Dash, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void noki_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void appear_init( PLAYER_WORK *pl_w );
static void appear_in( PLAYER_WORK *pl_w );
static void appear_out( PLAYER_WORK *pl_w );
static void appear_back( PLAYER_WORK *pl_w );
static void appear_jump( PLAYER_WORK *pl_w );
static void appear_fall( PLAYER_WORK *pl_w );
static void appear_land( PLAYER_WORK *pl_w );

/****************************************************************/
void pl14_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_init,
		appear_in,
		appear_out,
		appear_back,
		appear_jump,
		appear_fall,
		appear_land,
	} ;

	appear_jmp[ pl_w->routine_2 ]( pl_w ) ;
}

/*==============================================================*/
/*			Comes in from the side, with a second work			*/
/*			moving the other way								*/
/*==============================================================*/
static void appear_init( PLAYER_WORK *pl_w )
{
	PL14_LOCAL_W *lw = ( PL14_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *ew ;

	pl_w->routine_2 ++ ;
	pl_w->disp_flag = 1 ;
	lw->entry_x = pl_w->pos.x ;
	pl_w->pos.y += DOT_Y( 96.0f ) ;
	pl_w->spd.y = -DOT_Y( 4.0f ) ;
	pl_w->acc.y = 0.3348214f ;
	if( !pl_w->pl_no ){
		pl_w->disp_dir = 1 ;
		pl_w->pos.x -= 320.0f ;
		pl_w->spd.x = DOT_X( 8.0f ) ;
		pl_w->acc.x = 0.0f ;
	} else {
		pl_w->disp_dir = 0 ;
		pl_w->pos.x += 320.0f ;
		pl_w->spd.x = -DOT_X( 8.0f ) ;
		pl_w->acc.x = 0.0f ;
	}
	pl_char_set( pl_w, Char_Dash, 5 ) ;
	if( ew = pls14_01_set( pl_w, 15, 8 ) ){
		ew->spd.x = -pl_w->spd.x ;
		ew->acc.x = pl_w->acc.x ;
		ew->spd.y = pl_w->spd.y ;
		ew->acc.y = pl_w->acc.y ;
		ew->disp_dir = pl_w->disp_dir ;
	}
}

/*--------------------------------------------------------------*/
static void appear_in( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	pl_w->cg_type = 2 ;
	pl_w->cg_type = 0 ;
	if( scr_in_ck( pl_w ) ){
		pl_w->routine_2 ++ ;
		pl_voice_req( pl_w, 30 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_out( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	pl_w->cg_type = 2 ;
	pl_w->cg_type = 0 ;
	if( !scr_in_ck( pl_w ) ){
		pl_w->routine_2 ++ ;
		pl_w->spd.y = -DOT_Y( 6.0f ) ;
		pl_w->acc.y = 0.06696428f ;
		pl_voice_req( pl_w, 30 ) ;
		if( !pl_w->pl_no ){
			pl_w->disp_dir = 0 ;
			pl_w->spd.x = -20.0f ;
			pl_w->acc.x = 0.0f ;
		} else {
			pl_w->disp_dir = 1 ;
			pl_w->spd.x = 20.0f ;
			pl_w->acc.x = 0.0f ;
		}
	}
}

/*==============================================================*/
/*			Flies back to where the round starts				*/
/*==============================================================*/
static void appear_back( PLAYER_WORK *pl_w )
{
	PL14_LOCAL_W *lw = ( PL14_LOCAL_W * )pl_w->tlocal ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	pl_w->cg_type = 2 ;
	pl_w->cg_type = 0 ;
	if( !pl_w->pl_no ){
		if( pl_w->pos.x <= lw->entry_x ){
		} else return ;
	} else {
		if( pl_w->pos.x < lw->entry_x ) return ;
	}
	pl_w->routine_2 ++ ;
	pl_w->pos.x = lw->entry_x ;
	pl_w->pos.y += DOT_Y( 32.0f ) ;
	pl_char_set( pl_w, Char_Jump, 1 ) ;
}

/*--------------------------------------------------------------*/
static void appear_jump( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 5.0f ) ;
	pl_w->acc.y = -0.60267854f ;
}

/*--------------------------------------------------------------*/
static void appear_fall( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->pos.y > pl_w->ground_y ) return ;
	pl_w->routine_2 ++ ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_char_set( pl_w, Char_Jump, 3 ) ;
	pl_land_ef_set( pl_w ) ;
	pl_voice_req( pl_w, 10 ) ;
}

/*--------------------------------------------------------------*/
static void appear_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_w->routine_1 ++ ;
}

static void win_time_ko( PLAYER_WORK *pl_w );
static void win_ko_init( PLAYER_WORK *pl_w );
static void win_ko_move( PLAYER_WORK *pl_w );
static void win_ko_move_e( PLAYER_WORK *pl_w );
static void win_ko_wait( PLAYER_WORK *pl_w );
static void lose_time_ko( PLAYER_WORK *pl_w );
static void lose_time( PLAYER_WORK *pl_w );
static void win_time( PLAYER_WORK *pl_w );
static void draw_time( PLAYER_WORK *pl_w );

/****************************************************************/
void pl14_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_time_ko,
		lose_time_ko,
		win_time,
		lose_time,
		draw_time,
	} ;

	if( pl_win_end_ck( pl_w ) ){
		pl_color_set( pl_w, PC_DEFAULT ) ;
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
		win_ko_move_e,
		win_ko_wait,
	} ;

	win_ko_jmp[ pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void win_ko_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = 1 ;
	if( pl_be_cnt( pl_w ) >= 2 ){
		pl_char_set( pl_w, Char_Win, ( Rnd() & 1 ) ? 4 : 5 ) ;
		return ;
	}
	switch( Rnd() & 3 ){
	case 0:
		pl_char_set( pl_w, Char_Win, 0 ) ;
		pls14_01_set( pl_w, 19, 5 ) ;
		break ;
	case 1:
		pl_char_set( pl_w, Char_Win, 2 ) ;
		pls14_01_set( pl_w, 20, 10 ) ;
		break ;
	case 2:
		pl_char_set( pl_w, Char_Win, 4 ) ;
		break ;
	case 3:
		pl_char_set( pl_w, Char_Win, 5 ) ;
		break ;
	}
}

/*--------------------------------------------------------------*/
static void win_ko_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_ko_move_e( PLAYER_WORK *pl_w )
{
}

/*--------------------------------------------------------------*/
static void win_ko_wait( PLAYER_WORK *pl_w )
{
}

/*==============================================================*/
static void lose_time_ko( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, 7 ) ;
	}else{
		pl_char_move( pl_w ) ;
	}
}

/*==============================================================*/
static void lose_time( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, 8 ) ;
	}else{
		pl_char_move( pl_w ) ;
	}
}

/*==============================================================*/
static void win_time( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, 6 ) ;
	}else{
		pl_char_move( pl_w ) ;
	}
}

/*==============================================================*/
static void draw_time( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, 8 ) ;
	}else{
		pl_char_move( pl_w ) ;
	}
}

static void sp_shienbu( PLAYER_WORK *pl_w );
static void sp_fuusetsu( PLAYER_WORK *pl_w );
static void sp_seiten( PLAYER_WORK *pl_w );
static void sp_hofuku( PLAYER_WORK *pl_w );
static void sp_tenchi( PLAYER_WORK *pl_w );
static void sp_hisago( PLAYER_WORK *pl_w );
static void sp_pow( PLAYER_WORK *pl_w );
static void sp_enou( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_peach( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );
static void sp_wall( PLAYER_WORK *pl_w );
static void sp_enou_auto( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Special moves and hyper combos						*/
/****************************************************************/
void pl14_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *sp_jmp[])( PLAYER_WORK *pl_w ) = {
		sp_shienbu, sp_fuusetsu, sp_seiten, sp_hofuku, sp_tenchi, sp_hisago, sp_pow,
		sp_enou, sp_chouhatsu, sp_peach, sp_snapback, sp_wall, sp_enou_auto,
	} ;

	sp_jmp[pl_w->pl_sp_type]( pl_w ) ;
}

static void shienbu_init( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void shienbu_move( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );

/*==============================================================*/
static void sp_shienbu( PLAYER_WORK *pl_w )
{
	static void ( *shienbu_jmp[])( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw ) = {
		shienbu_init, shienbu_move,
	} ;

	if( pl_w->pl_stance == STANCE_AIR ){
		pl_jump_move( pl_w ) ;
		if( pl_w->pos.y <= pl_w->ground_y ) pl_w->pos.y = pl_w->ground_y ;
	}
	shienbu_jmp[pl_w->routine_2]( pl_w, ( PL14_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void shienbu_init( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->atck_no = pl14_at_shienbu ;
	HIT_RESET( pl_w ) ;
	if( pl_w->pl_stance == STANCE_AIR ){
		pl_w->spd.x /= 16.0f ;
		pl_w->spd.y /= 8.0f ;
		pl_w->acc.y /= 64.0f ;
		pl_w->acc.x = 0.0f ;
		pl_char_set( pl_w, Char_Special, 1 ) ;
	} else {
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_sp_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 0 ) ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
	}
}

/*--------------------------------------------------------------*/
static void shienbu_move( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		if( pl_w->pl_stance == STANCE_AIR ) pl_jump_ret( pl_w ) ;
		else pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pls14_01_set( pl_w, 12, 7 ) ;
		pls14_02_set( pl_w, 34, 3 ) ;
	}
}

static void fuusetsu_init( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void fuusetsu_move( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void fuusetsu_end( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );

/*==============================================================*/
static void sp_fuusetsu( PLAYER_WORK *pl_w )
{
	static void ( *fuusetsu_jmp[])( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw ) = {
		fuusetsu_init, fuusetsu_move, fuusetsu_end,
	} ;

	if( pl_w->pl_stance == STANCE_AIR ){
		pl_jump_move( pl_w ) ;
		if( pl_w->pos.y <= pl_w->ground_y ) pl_w->pos.y = pl_w->ground_y ;
	}
	fuusetsu_jmp[pl_w->routine_2]( pl_w, ( PL14_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void fuusetsu_init( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_w->timer0 = 24 ;
	pl_w->timer1 = 30 ;
	pl_dir_set( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->spd.x = pl_w->pl_dir ? DOT_X( 0.5f ) : -DOT_X( 0.5f ) ;
	pl_w->acc.x = 0.0f ;
	if( pl_w->pl_stance == STANCE_AIR ){
		pl_w->spd.x /= 16.0f ;
		pl_w->spd.y /= 8.0f ;
		pl_w->acc.y /= 64.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->atck_no = pl14_at_fuusetsu ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 4 ) ;
	} else {
		pl_w->pl_stance = STANCE_STAND ;
		pl_sp_ef_set( pl_w ) ;
		pl_w->atck_no = pl14_at_fuusetsu ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 2 ) ;
	}
	pl_color_set( pl_w, PC_DEFAULT ) ;
}

/*==============================================================*/
/*			Hits as the animation says; mashing the buttons		*/
/*			makes it last longer								*/
/*==============================================================*/
static void fuusetsu_move( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_flag1 ){
		pl_w->cg_flag1 = 0 ;
		pl_w->atck_no = pl14_at_fuusetsu ;
		HIT_RESET( pl_w ) ;
	}
	if( pl_w->timer1 > 0 ){
		pl_w->timer1 -- ;
		if( sw_renda_ck( pl_w ) ) pl_w->timer0 += 2 ;
	}
	if( -- pl_w->timer0 < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->char_no.b.no = 3 ;
		if( pl_w->pl_stance == STANCE_AIR ) pl_w->char_no.b.no = 5 ;
		pl_char_set( pl_w, Char_Special, pl_w->char_no.b.no ) ;
	}
}

/*--------------------------------------------------------------*/
static void fuusetsu_end( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->atck_no = pl_w->atck_level ;
	HIT_RESET( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		if( pl_w->pl_stance == STANCE_AIR ) pl_jump_ret( pl_w ) ;
		else pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ) pl_w->cg_type = 0 ;
}

static void seiten_init( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void seiten_move( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void seiten_jump( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );

/*==============================================================*/
static void sp_seiten( PLAYER_WORK *pl_w )
{
	static void ( *seiten_jmp[])( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw ) = {
		seiten_init, seiten_move, seiten_jump,
	} ;

	seiten_jmp[pl_w->routine_2]( pl_w, ( PL14_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void seiten_init( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	Float	spd ;

	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->timer0 = 0 ;
	pl_w->atck_no = pl14_at_seiten ;
	HIT_RESET( pl_w ) ;
	if( pl_w->pl_stance == STANCE_AIR ){
		pl_w->routine_2 ++ ;
		pl_w->char_no.b.no = pl_w->atck_level ? 27 : 11 ;
		pl_char_set( pl_w, Char_Special, pl_w->char_no.b.no ) ;
	} else {
		pl_air_set( pl_w ) ;
		pl_w->char_no.b.no = pl_w->atck_level ? 26 : 10 ;
		pl_char_set( pl_w, Char_Special, pl_w->char_no.b.no ) ;
		pl_sp_ef_set( pl_w ) ;
	}
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	if( !pl_w->atck_level ){
		pl_w->spd.y = DOT_Y( 9.5f ) ;
		pl_w->acc.y = -0.9040178f ;
		spd = -DOT_X( 2.0f ) ;
		if( pl_w->disp_dir ) spd = DOT_X( 2.0f ) ;
	} else {
		pl_w->spd.y = DOT_Y( 16.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.625f ) ;
		spd = -DOT_X( 2.75f ) ;
		if( pl_w->disp_dir ) spd = DOT_X( 2.75f ) ;
	}
	pl_w->spd.x = spd ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
}

/*--------------------------------------------------------------*/
static void seiten_move( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->pos.x += pl_w->pl_dir ? DOT_X( 52.0f ) : -DOT_X( 52.0f ) ;
	}
}

/*==============================================================*/
/*			The light version hits once more in the air, the	*/
/*			heavy one twice										*/
/*==============================================================*/
static void seiten_jump( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->timer0 != pl_w->atck_level + 1 && pl_w->hit_flag ){
		pl_w->timer0 ++ ;
		if( pl_w->timer0 < pl_w->atck_level + 2 ){
			pl_w->atck_no = pl14_at_seiten_2 ;
			HIT_RESET( pl_w ) ;
		}
	}
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_land_ef_set( pl_w ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void hofuku_init( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void hofuku_move( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void hofuku_end( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );

/*==============================================================*/
static void sp_hofuku( PLAYER_WORK *pl_w )
{
	static void ( *hofuku_jmp[])( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw ) = {
		hofuku_init, hofuku_move, hofuku_end,
	} ;

	hofuku_jmp[pl_w->routine_2]( pl_w, ( PL14_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void hofuku_init( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_w->timer0 = pl_w->atck_level ? 40 : 30 ;
	pl_dir_set( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->atck_no = pl_w->atck_level ;
	HIT_RESET( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_char_set( pl_w, Char_Special, 12 ) ;
	pl_w->spd.x = pl_w->disp_dir ? DOT_X( 4.25f ) : -DOT_X( 4.25f ) ;
	pl_w->sp_ng_timer = 10 ;
	pl_w->atck_ng_timer = 10 ;
}

/*--------------------------------------------------------------*/
static void hofuku_move( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	if( ( pl_w->pl_sw_new & KEY_ATCK ) && pl_nml_atck_ck( pl_w ) ) return ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_char_move( pl_w ) ;
	pl_w->push_ng_timer = 2 ;
	if( -- pl_w->timer0 > 0 ) return ;
	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_char_set( pl_w, Char_Special, 13 ) ;
}

/*--------------------------------------------------------------*/
static void hofuku_end( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	pl_w->push_ng_timer = 2 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_land_ef_set( pl_w ) ;
		pl_stand_ret( pl_w ) ;
	}
}

static void tenchi_init( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void tenchi_move( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void tenchi_end( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );

/*==============================================================*/
static void sp_tenchi( PLAYER_WORK *pl_w )
{
	static void ( *tenchi_jmp[])( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw ) = {
		tenchi_init, tenchi_move, tenchi_end,
	} ;

	if( pl_w->pl_stance == STANCE_AIR ){
		pl_jump_move( pl_w ) ;
		if( pl_w->pos.y < pl_w->ground_y ) pl_w->pos.y = pl_w->ground_y ;
	}
	tenchi_jmp[pl_w->routine_2]( pl_w, ( PL14_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void tenchi_init( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	pl_color_set( pl_w, PC_DEFAULT ) ;
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	if( pl_w->pl_stance == STANCE_AIR ){
		pl_w->spd.x /= 16.0f ;
		pl_w->spd.y /= 8.0f ;
		pl_w->acc.y /= 64.0f ;
		pl_w->acc.x = 0.0f ;
	}
	pl_dir_set( pl_w ) ;
	pl_w->atck_no = pl14_at_tenchi ;
	HIT_RESET( pl_w ) ;
	if( !pl_w->pl_stance ){
		pl_sp_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 0 ) ;
	} else pl_char_set( pl_w, Char_Super, 64 ) ;
}

/*--------------------------------------------------------------*/
static void tenchi_move( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	VEC3	ofs ;

	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	if( pl_char_move( pl_w ) < 0 ) return ;
	pl_w->routine_2 ++ ;
	pl_w->stop_move_flag = 0 ;
	pl_w->stop_move_timer = 0 ;
	/* ofs.y is added to without being set first, as in the arcade */
	if( pl_w->pl_stance == STANCE_AIR ){
		pl_char_set( pl_w, Char_Super, 65 ) ;
		ofs.x = 0.0f ;
		ofs.y += DOT_Y( 44.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	} else {
		pl_char_set( pl_w, Char_Super, 1 ) ;
		ofs.x = 0.0f ;
		ofs.y += DOT_Y( 24.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void tenchi_end( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		if( !pl_w->pl_stance ){
			pl_stand_ret( pl_w ) ;
			return ;
		}
		pl_jump_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shl14_01_set( pl_w, 0 ) ;
	}
}

static void hisago_init( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void hisago_move( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void hisago_end( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );

/*==============================================================*/
static void sp_hisago( PLAYER_WORK *pl_w )
{
	static void ( *hisago_jmp[])( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw ) = {
		hisago_init, hisago_move, hisago_end,
	} ;

	if( pl_w->pl_stance == STANCE_AIR ){
		pl_jump_move( pl_w ) ;
		if( pl_w->pos.y < pl_w->ground_y ) pl_w->pos.y = pl_w->ground_y ;
	}
	hisago_jmp[pl_w->routine_2]( pl_w, ( PL14_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void hisago_init( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->atck_no = pl14_at_hisago ;
	HIT_RESET( pl_w ) ;
	pl_w->timer0 = 60 ;
	pl_char_set( pl_w, Char_Catch, 4 ) ;
	pls14_01_set( pl_w, 10, 3 ) ;
	pl_sp_ef_set( pl_w ) ;
}

/*==============================================================*/
/*			A throw if it hits, else it runs out				*/
/*==============================================================*/
static void hisago_move( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( pl_w->hit_flag ){
		if( pl_sp_catch_ck( pl_w ) ){
			pl_w->catch_flag = 1 ;
			pl_w->catch_no = 0xc3 ;
			pl_catch_set( pl_w, pl_w->em_w ) ;
			return ;
		}
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, 17 ) ;
		pls14_02_set( pl_w, 35, 5 ) ;
	}
	if( -- pl_w->timer0 > 0 ) return ;
	pl_w->routine_2 ++ ;
	pl_char_set( pl_w, Char_Special, 17 ) ;
	pls14_02_set( pl_w, 35, 5 ) ;
}

/*--------------------------------------------------------------*/
static void hisago_end( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void pow_init( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void pow_shot( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void pow_wait( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void pow_ride_ck( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void pow_jump( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void pow_ride( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void pow_ride2( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void pow_ride3( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void pow_off( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void pow_slide( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void pow_hop( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void pow_fall( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void pow_end( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void pow_jump_on( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Fires an effect, jumps on it and rides it			*/
/*==============================================================*/
static void sp_pow( PLAYER_WORK *pl_w )
{
	static void ( *pow_jmp[])( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw ) = {
		pow_init, pow_shot, pow_wait, pow_ride_ck, pow_jump, pow_ride, pow_ride2,
		pow_ride3, pow_off, pow_slide, pow_hop, pow_fall, pow_end,
	} ;

	if( pl_w->pl_stance == STANCE_AIR ){
		pl_jump_move( pl_w ) ;
		if( pl_w->pos.y < pl_w->ground_y ) pl_w->pos.y = pl_w->ground_y ;
	}
	pl_w->prio = 8 ;
	pow_jmp[pl_w->routine_2]( pl_w, ( PL14_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void pow_init( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->rot_ofs_x = 0 ;
	pl_w->rot_ofs_y = 0 ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->atck_no = pl14_at_pow ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 5 ) ;
	pl_sp_ef_set( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pow_shot( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	VEC3	ofs ;

	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->type = pow_ride_tbl[Rnd() & 7] ;
		switch( pl_w->type ){
		case 24:
			pl_w->sub_type = 18 ;
			break ;
		case 20:
			pl_w->sub_type = 17 ;
			break ;
		case 16:
			pl_w->sub_type = 16 ;
			break ;
		default:
			pl_w->type = 24 ;
			pl_w->sub_type = 18 ;
			break ;
		}
		if( ( pl_w->ef_w = shl14_02_set( pl_w, pl_w->type ) ) == 0 ){
			pl_stand_ret( pl_w ) ;
			return ;
		}
		pl_w->routine_2 ++ ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		/* ofs.y is added to without being set first, as in the arcade */
		ofs.x = 0.0f ;
		ofs.y += DOT_Y( 59.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pow_wait( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 180 ;
	}
}

/*==============================================================*/
/*			Waits for the effect to be ready to ride, or		*/
/*			gone												*/
/*==============================================================*/
static void pow_ride_ck( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	PLAYER_WORK	*ew ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	ew = pl_w->ef_w ;
	if( !ew->disp_flag || -- pl_w->timer0 <= 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( ew->routine_0 == 1 && ew->routine_1 == 1 && ew->routine_2 ) pow_jump_on( pl_w ) ;
	else if( ew->routine_0 == 1 && ew->routine_1 == 2 ) pow_jump_on( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pow_jump( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( -- pl_w->timer0 > 0 ) return ;
	pl_w->routine_2 ++ ;
	pl_w->timer0 = 10 ;
	pl_char_set( pl_w, Char_Super, 8 ) ;
}

/*--------------------------------------------------------------*/
static void pow_ride( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( -- pl_w->timer0 > 0 ) return ;
	pl_w->routine_2 ++ ;
	pl_w->rot[2] = 0xe000 ;
	switch( pl_w->type ){
	case 24:
		pl_w->pos.y -= 0.0f ;
		pls14_01_set( pl_w, 18, 5 ) ;
		break ;
	case 20:
		pl_w->pos.y -= DOT_Y( 8.0f ) ;
		pls14_01_set( pl_w, 17, 5 ) ;
		break ;
	case 16:
		pl_w->pos.y -= DOT_Y( 4.0f ) ;
		pls14_01_set( pl_w, 16, 5 ) ;
		break ;
	}
	pl_char_set( pl_w, Char_Super, 9 ) ;
}

/*--------------------------------------------------------------*/
static void pow_ride2( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	PLAYER_WORK	*ew ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->rot[2] = 0xe000 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		ew = pl_w->ef_w ;
		pl_w->rot[2] = 0x4000 ;
		pl_w->pos.x = ew->pos.x + -DOT_X( 64.0f ) ;
		if( pl_w->disp_dir ) pl_w->pos.x = ew->pos.x + DOT_X( 64.0f ) ;
		ew->timer0 -- ;
		switch( pl_w->type ){
		case 24:
			pl_w->pos.y = ew->pos.y + DOT_Y( 72.0f ) ;
			pls14_01_set( pl_w, 18, 5 ) ;
			break ;
		case 20:
			pl_w->pos.y = ew->pos.y + DOT_Y( 64.0f ) ;
			pls14_01_set( pl_w, 17, 5 ) ;
			break ;
		case 16:
			pl_w->pos.y = ew->pos.y + DOT_Y( 72.0f ) ;
			pls14_01_set( pl_w, 16, 5 ) ;
			break ;
		}
	}
}

/*--------------------------------------------------------------*/
static void pow_ride3( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	PLAYER_WORK	*ew ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->rot[2] = 0x4000 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->rot[2] = 0 ;
		ew = pl_w->ef_w ;
		ew->timer0 -- ;
		pl_w->pos.x = ew->pos.x + 0.0f ;
		pl_w->pos.x += pl_w->disp_dir ? -DOT_X( 8.0f ) : DOT_X( 8.0f ) ;
		pl_w->pos.y = ew->pos.y + DOT_Y( 64.0f ) ;
		switch( pl_w->type ){
		case 24:
			pls14_01_set( pl_w, 18, 5 ) ;
			break ;
		case 20:
			pls14_01_set( pl_w, 17, 5 ) ;
			break ;
		case 16:
			pls14_01_set( pl_w, 16, 5 ) ;
			break ;
		}
	}
}

/*--------------------------------------------------------------*/
static void pow_off( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	PLAYER_WORK	*ew ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->spd.x = pl_w->disp_dir ? -DOT_Y( 3.75f ) : DOT_Y( 3.75f ) ;
		pl_char_set( pl_w, Char_Super, 10 ) ;
		ew = pl_w->ef_w ;
		ew->timer0 -- ;
		pl_w->pos.y = pl_w->ground_y ;
	}
}

/*--------------------------------------------------------------*/
static void pow_slide( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_char_set( pl_w, Char_Super, 11 ) ;
		pl_land_ef_set( pl_w ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
}

/*--------------------------------------------------------------*/
static void pow_hop( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.y = DOT_Y( 4.75f ) ;
		pl_w->acc.y = -0.4520089f ;
		pl_w->spd.x = pl_w->disp_dir ? -DOT_X( 2.5f ) : DOT_X( 2.5f ) ;
		pl_char_set( pl_w, Char_Jump, 2 ) ;
		return ;
	}
	if( pl_w->cg_type ){
		/* nothing is done here */
	}
}

/*--------------------------------------------------------------*/
static void pow_fall( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_land_ef_set( pl_w ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Super, 6 ) ;
		pl_w->timer0 = 20 ;
		SCR_EXCEP_SET ;
	}
}

/*--------------------------------------------------------------*/
static void pow_end( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
/*			A jump that lands on the effect in 46 frames		*/
/*==============================================================*/
static void pow_jump_on( PLAYER_WORK *pl_w )
{
	PLAYER_WORK	*ew ;
	Float	ofs ;

	ew = pl_w->ef_w ;
	pl_w->routine_2 ++ ;
	pl_char_set( pl_w, Char_Super, 7 ) ;
	pl_w->spd.y = DOT_Y( 9.5f ) ;
	pl_w->acc.y = -0.9040178f ;
	ofs = DOT_X( 32.0f ) ;
	if( pl_w->disp_dir ) ofs = -DOT_X( 32.0f ) ;
	pl_w->spd.x = ( ew->pos.x + ofs - pl_w->pos.x ) / 46.0f ;
	pl_w->spd.y += ( ew->pos.y - pl_w->pos.y + DOT_Y( 104.0f ) ) / 46.0f ;
	pl_w->timer0 = 46 ;
}

static void enou_grow( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void enou_stand( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void enou_rush( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void enou_rush_end( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void enou_shrink( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static Uint8 enou_atck_ck( PLAYER_WORK *pl_w );
static Uint8 enou_walk_ck( PLAYER_WORK *pl_w, Uint8 old );
static Uint8 enou_walk_on( PLAYER_WORK *pl_w, Uint8 old );
static void enou_stand_set( PLAYER_WORK *pl_w );
static Uint8 enou_turn_ck( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Grows into a giant monkey that the stick and		*/
/*			buttons move, for 240 frames						*/
/*==============================================================*/
static void sp_enou( PLAYER_WORK *pl_w )
{
	static void ( *enou_jmp[])( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw ) = {
		enou_grow, enou_stand, enou_rush, enou_rush_end, enou_shrink,
	} ;
	PL14_LOCAL_W	*lw = ( PL14_LOCAL_W * )pl_w->tlocal ;

	if( vs_sys_w->vs_st >= VS_END_WAIT && !flash_flag_get() ) lw->monkey_timer = 0 ;
	if( -- lw->monkey_timer <= 0 && pl_w->routine_2 != 4 ){
		pl_w->routine_2 = 4 ;
		pl_w->routine_3 = 0 ;
	}
	enou_jmp[pl_w->routine_2]( pl_w, ( PL14_LOCAL_W * )pl_w->tlocal ) ;
}

static void enou_grow_init( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void enou_grow_wait( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void enou_grow_up( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void enou_grow_end( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );

/*==============================================================*/
static void enou_grow( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	static void ( *enou_grow_jmp[])( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw ) = {
		enou_grow_init, enou_grow_wait, enou_grow_up, enou_grow_end,
	} ;

	enou_grow_jmp[pl_w->routine_3]( pl_w, ( PL14_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void enou_grow_init( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	VEC3	ofs ;

	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	scr_excep_set( pl_w, 13, 6 ) ;
	pl_w->routine_3 ++ ;
	lw->monkey_timer = 240 ;
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->atck_no = pl14_at_enou ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 54 ) ;
	lw->step_timer = 15 ;
	pl_sp_ef_set( pl_w ) ;
	pl_voice_req( pl_w, 20 ) ;
	pl_w->stop_move_flag = 0 ;
	pl_w->stop_move_timer = 0 ;
	/* ofs.y is added to without being set first, as in the arcade */
	ofs.x = 0.0f ;
	ofs.y += DOT_Y( 64.0f ) ;
	pl_hyper_start( pl_w, &ofs, 1 ) ;
}

/*--------------------------------------------------------------*/
static void enou_grow_wait( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		lw->monkey_scale_x = 0.5f ;
		lw->monkey_scale_y = 0.5f ;
		pl_w->scale.x = lw->monkey_scale_x ;
		pl_w->scale.y = lw->monkey_scale_y ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_char_set( pl_w, Char_Super, 63 ) ;
		if( !( vs_sys_work.team_hyper_flag & ( 1 << pl_w->pl_no ) ) ) pls14_02_set( pl_w, 0, 1 ) ;
		ef_color_set( pl_w, 0, pl_w->pl_color + 6, 1 ) ;
		ef_color_set( pl_w, 2, 12, 2 ) ;
		pl_voice_req( pl_w, 34 ) ;
		pl_se_req( pl_w, 53 ) ;
		pl_w->armor_flag |= ARMOR_HEAVY ;
		pl_w->nml_catch_ng_timer = 2 ;
		combo_reset( pl_w ) ;
	}
}

/*==============================================================*/
/*			Flashes while growing to full size					*/
/*==============================================================*/
static void enou_grow_up( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( !( Sys->game_timer0 & 1 ) ) ef_color_set( pl_w, 0, pl_w->pl_color + 6, 1 ) ;
	else pl_color_set( pl_w, 8 ) ;
	if( !( Sys->game_timer0 & 7 ) ){
		scr_work.quake_level = 2 ;
		scr_work.quake_flag = 1 ;
	}
	lw->monkey_scale_x += 0.02f ;
	lw->monkey_scale_y += 0.02f ;
	pl_w->scale.x = lw->monkey_scale_x ;
	pl_w->scale.y = lw->monkey_scale_y ;
	if( lw->monkey_scale_y > 1.0f ){
		lw->monkey_scale_x = 1.0f ;
		pl_w->scale.x = lw->monkey_scale_x ;
		pl_w->scale.y = lw->monkey_scale_y ;
		pl_w->routine_3 ++ ;
		lw->step_timer = 30 ;
		pl_char_set( pl_w, Char_Super, 55 ) ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
		ef_color_set( pl_w, 0, pl_w->pl_color + 6, 1 ) ;
		ef_color_set( pl_w, 2, 12, 2 ) ;
		pl_voice_req( pl_w, 37 ) ;
	}
	pl_w->armor_flag |= ARMOR_HEAVY ;
	pl_w->nml_catch_ng_timer = 2 ;
	combo_reset( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void enou_grow_end( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( -- lw->step_timer <= 0 ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		lw->monkey_timer = 240 ;
		Timer_Gauge_Set( pl_w, &lw->monkey_timer, 6 ) ;
		enou_stand_set( pl_w ) ;
	}
	pl_w->armor_flag |= ARMOR_HEAVY ;
	pl_w->nml_catch_ng_timer = 2 ;
	combo_reset( pl_w ) ;
}

static void enou_wait( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void enou_walk( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void enou_turn( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void enou_punch( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void enou_kick( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );

/*--------------------------------------------------------------*/
static void enou_stand( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	static void ( *enou_stand_jmp[])( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw ) = {
		enou_wait, enou_walk, enou_turn, enou_punch, enou_kick,
	} ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	enou_stand_jmp[pl_w->routine_3]( pl_w, ( PL14_LOCAL_W * )pl_w->tlocal ) ;
	pl_w->armor_flag |= ARMOR_HEAVY ;
	pl_w->nml_catch_ng_timer = 2 ;
	combo_reset( pl_w ) ;
}

/*==============================================================*/
static void enou_wait( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	if( enou_atck_ck( pl_w ) ) return ;
	if( enou_walk_ck( pl_w, 0 ) ) return ;
	if( enou_turn_ck( pl_w ) ) return ;
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void enou_walk( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	if( enou_atck_ck( pl_w ) ) return ;
	if( enou_turn_ck( pl_w ) ) return ;
	if( !enou_walk_on( pl_w, 0 ) ) enou_stand_set( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
	} else if( pl_w->cg_type == 1 ){
		pl_w->cg_type = 2 ;
		scr_work.quake_level = 1 ;
		scr_work.quake_flag = 1 ;
		pl_se_req( pl_w, 53 ) ;
	}
}

/*--------------------------------------------------------------*/
static void enou_turn( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		enou_stand_set( pl_w ) ;
		pl_dir_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void enou_punch( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		enou_stand_set( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type == 1 ){
		pl_w->cg_type = 0 ;
		pls14_01_set( pl_w, 22, 13 ) ;
		pls14_01_set( pl_w, 23, 13 ) ;
		pls14_01_set( pl_w, 24, 13 ) ;
	} else if( pl_w->cg_type == 2 ){
		pl_w->cg_type = 0 ;
		pl_w->pos.x += pl_w->disp_dir ? DOT_X( 16.0f ) : -DOT_X( 16.0f ) ;
	}
}

/*--------------------------------------------------------------*/
static void enou_kick( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		enou_stand_set( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type == 1 ){
		pl_w->cg_type = 0 ;
		pls14_01_set( pl_w, 32, 13 ) ;
		pls14_01_set( pl_w, 33, 13 ) ;
	} else if( pl_w->cg_type == 2 ){
		pl_w->cg_type = 0 ;
		pl_w->pos.x += pl_w->disp_dir ? -DOT_X( 40.0f ) : DOT_X( 40.0f ) ;
	}
}

static void enou_rush_init( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void enou_rush_mid( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void enou_rush_up( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void enou_rush_down( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void enou_rush_top( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void enou_rush_bottom( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );

/*==============================================================*/
/*			A run of blows, longer while a kick is held; the	*/
/*			stick aims it up or down							*/
/*==============================================================*/
static void enou_rush( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	static void ( *enou_rush_jmp[])( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw ) = {
		enou_rush_init, enou_rush_mid, enou_rush_up, enou_rush_down, enou_rush_top, enou_rush_bottom,
	} ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_w->pad_sw.now & KEY_KICK ){
		if( pl_w->timer1 ++ > 150 ){
			pl_w->timer1 = 10 ;
			pl_w->routine_2 ++ ;
			pl_w->routine_3 = 0 ;
			ef_color_set( pl_w, 0, pl_w->pl_color + 6, 1 ) ;
			return ;
		}
	} else if( pl_w->timer1 ++ > 60 ){
		pl_w->timer1 = 10 ;
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		ef_color_set( pl_w, 0, pl_w->pl_color + 6, 1 ) ;
		return ;
	}
	enou_rush_jmp[pl_w->routine_3]( pl_w, ( PL14_LOCAL_W * )pl_w->tlocal ) ;
	pl_w->armor_flag |= ARMOR_HEAVY ;
	pl_w->nml_catch_ng_timer = 2 ;
	combo_reset( pl_w ) ;
	if( !( Sys->game_timer0 & 1 ) ) ef_color_set( pl_w, 0, pl_w->pl_color * 2 + 20, 1 ) ;
	else ef_color_set( pl_w, 0, pl_w->pl_color * 2 + 21, 1 ) ;
	if( !( Sys->game_timer0 & 3 ) ) dust_set( pl_w, 0, 0, 3.8f, 0.4f ) ;
}

/*==============================================================*/
static void enou_rush_init( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		scr_work.quake_level = 2 ;
		scr_work.quake_flag = 1 ;
		pl_w->atck_no = pl14_at_enou_rush ;
		HIT_RESET( pl_w ) ;
		if( !( pl_w->pl_new_lvbt & ( KEY_UP | KEY_DOWN ) ) ){
			pl_char_set( pl_w, Char_Super, 41 ) ;
			shl14_04_set( pl_w, 0 ) ;
			return ;
		}
		if( pl_w->pl_new_lvbt & KEY_UP ){
			pl_w->routine_3 = 2 ;
			pl_char_set( pl_w, Char_Super, 40 ) ;
			shl14_04_set( pl_w, 0 ) ;
		} else {
			pl_w->routine_3 = 3 ;
			pl_char_set( pl_w, Char_Super, 42 ) ;
			shl14_04_set( pl_w, 0 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void enou_rush_mid( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		if( pl_w->pl_new_lvbt & ( KEY_UP | KEY_DOWN ) ){
			if( pl_w->pl_new_lvbt & KEY_UP ){
				pl_w->routine_3 = 2 ;
				pl_char_set( pl_w, Char_Super, 40 ) ;
			} else {
				pl_w->routine_3 = 3 ;
				pl_char_set( pl_w, Char_Super, 42 ) ;
			}
		}
	}
}

/*--------------------------------------------------------------*/
static void enou_rush_up( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		if( !( pl_w->pl_new_lvbt & ( KEY_UP | KEY_DOWN ) ) || ( pl_w->pl_new_lvbt & KEY_DOWN ) ){
			pl_w->routine_3 = 1 ;
			pl_char_set( pl_w, Char_Super, 41 ) ;
			return ;
		}
		if( pl_w->pl_new_lvbt & KEY_UP ){
			pl_w->routine_3 = 4 ;
			pl_char_set( pl_w, Char_Super, 38 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void enou_rush_down( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		if( !( pl_w->pl_new_lvbt & ( KEY_UP | KEY_DOWN ) ) || ( pl_w->pl_new_lvbt & KEY_UP ) ){
			pl_w->routine_3 = 1 ;
			pl_char_set( pl_w, Char_Super, 41 ) ;
		}
		if( pl_w->pl_new_lvbt & KEY_DOWN ){
			pl_w->routine_3 = 5 ;
			pl_char_set( pl_w, Char_Super, 44 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void enou_rush_top( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 && !( pl_w->pl_new_lvbt & KEY_UP ) ){
		pl_w->routine_3 = 2 ;
		pl_char_set( pl_w, Char_Super, 39 ) ;
	}
}

/*--------------------------------------------------------------*/
static void enou_rush_bottom( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 && !( pl_w->pl_new_lvbt & KEY_DOWN ) ){
		pl_w->routine_3 = 3 ;
		pl_char_set( pl_w, Char_Super, 43 ) ;
	}
}

static void enou_rush_wait( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void enou_rush_back( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );

/*--------------------------------------------------------------*/
static void enou_rush_end( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	static void ( *enou_rush_end_jmp[])( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw ) = {
		enou_rush_wait, enou_rush_back,
	} ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	enou_rush_end_jmp[pl_w->routine_3]( pl_w, ( PL14_LOCAL_W * )pl_w->tlocal ) ;
	pl_w->armor_flag |= ARMOR_HEAVY ;
	pl_w->nml_catch_ng_timer = 2 ;
	combo_reset( pl_w ) ;
}

/*==============================================================*/
static void enou_rush_wait( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer1 > 0 ) return ;
	pl_w->routine_3 ++ ;
	pl_char_set( pl_w, Char_Super, 37 ) ;
	scr_work.quake_level = 2 ;
	scr_work.quake_flag = 1 ;
}

/*--------------------------------------------------------------*/
static void enou_rush_back( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) enou_stand_set( pl_w ) ;
}

static void enou_shrink_init( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void enou_shrink_down( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void enou_shrink_end( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );

/*--------------------------------------------------------------*/
static void enou_shrink( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	static void ( *enou_shrink_jmp[])( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw ) = {
		enou_shrink_init, enou_shrink_down, enou_shrink_end,
	} ;

	enou_shrink_jmp[pl_w->routine_3]( pl_w, ( PL14_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void enou_shrink_init( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	pl_w->routine_3 ++ ;
	lw->monkey_scale_x -= 0.02f ;
	lw->monkey_scale_y -= 0.02f ;
	pl_w->scale.x = lw->monkey_scale_x ;
	pl_w->scale.y = lw->monkey_scale_y ;
	pl_char_set( pl_w, Char_Super, 67 ) ;
	if( !( vs_sys_work.team_hyper_flag & ( 1 << pl_w->pl_no ) ) ) pls14_02_set( pl_w, 1, 1 ) ;
	pl_voice_req( pl_w, 35 ) ;
	pl_se_req( pl_w, 58 ) ;
	pl_w->armor_flag |= ARMOR_HEAVY ;
	pl_w->nml_catch_ng_timer = 2 ;
	combo_reset( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void enou_shrink_down( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	lw->monkey_scale_x -= 0.02f ;
	lw->monkey_scale_y -= 0.02f ;
	pl_w->scale.x = lw->monkey_scale_x ;
	pl_w->scale.y = lw->monkey_scale_y ;
	if( !( Sys->game_timer0 & 1 ) ) ef_color_set( pl_w, 0, pl_w->pl_color + 6, 1 ) ;
	else pl_color_set( pl_w, 8 ) ;
	if( pl_w->scale.y < 0.5f ){
		lw->monkey_scale_x = 1.0f ;
		lw->monkey_scale_y = 1.0f ;
		pl_w->scale.x = lw->monkey_scale_x ;
		pl_w->scale.y = lw->monkey_scale_y ;
		pl_w->routine_3 ++ ;
		pl_dir_set( pl_w ) ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->armor_flag = 0 ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_char_set( pl_w, Char_Super, 61 ) ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		return ;
	}
	pl_w->armor_flag |= ARMOR_HEAVY ;
	pl_w->nml_catch_ng_timer = 2 ;
	combo_reset( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void enou_shrink_end( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			A punch, a kick or a run of blows, from the			*/
/*			buttons just pressed								*/
/*==============================================================*/
static Uint8 enou_atck_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->pad_sw.on & KEY_LP ){
		pl_w->routine_3 = 3 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->atck_no = pl14_at_enou_lp ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 32 ) ;
		scr_work.quake_level = 1 ;
		scr_work.quake_flag = 1 ;
		pl_voice_req( pl_w, 37 ) ;
		return 1 ;
	}
	if( pl_w->pad_sw.on & KEY_HP ){
		pl_w->routine_3 = 4 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->atck_no = pl14_at_enou_hp ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 52 ) ;
		scr_work.quake_level = 1 ;
		scr_work.quake_flag = 1 ;
		pl_voice_req( pl_w, 37 ) ;
		return 1 ;
	}
	if( !( pl_w->pad_sw.on & KEY_KICK ) ) return 0 ;
	pl_w->routine_2 = 2 ;
	pl_w->routine_3 = 0 ;
	pl_w->timer1 = 0 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_char_set( pl_w, Char_Super, 33 ) ;
	pl_voice_req( pl_w, 37 ) ;
	pl_se_req( pl_w, 76 ) ;
	return 1 ;
}

/*==============================================================*/
/*			Starts a walk if the stick is held to either		*/
/*			side												*/
/*==============================================================*/
static Uint8 enou_walk_ck( PLAYER_WORK *pl_w, Uint8 old )
{
	Uint16	sw = old ? pl_w->pl_now_lvbt : pl_w->pl_new_lvbt ;

	if( !( sw & ( KEY_FORWARD | KEY_BACK ) ) ) return 0 ;
	pl_w->routine_3 = 1 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->jump_dir = ( pl_w->pl_sw_new & BACK ) >> 10 ;
	pl_char_set( pl_w, Char_Super, pl_w->jump_dir + 29 ) ;
	if( !pl_w->disp_dir ){
		pl_w->spd.x = -DOT_X( 3.375f ) ;
		if( pl_w->jump_dir ) pl_w->spd.x = -pl_w->spd.x ;
	} else {
		pl_w->spd.x = DOT_X( 3.375f ) ;
		if( pl_w->jump_dir ) pl_w->spd.x = -pl_w->spd.x ;
	}
	return 1 ;
}

/*==============================================================*/
static Uint8 enou_walk_on( PLAYER_WORK *pl_w, Uint8 old )
{
	Uint16	sw = old ? pl_w->pl_now_lvbt : pl_w->pl_new_lvbt ;

	if( !( sw & ( KEY_FORWARD | KEY_BACK ) ) ) return 0 ;
	return 1 ;
}

/*==============================================================*/
static void enou_stand_set( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = 1 ;
	pl_w->routine_3 = 0 ;
	pl_char_set( pl_w, Char_Super, 28 ) ;
}

/*==============================================================*/
/*			Turns round when the opponent has gone behind		*/
/*==============================================================*/
static Uint8 enou_turn_ck( PLAYER_WORK *pl_w )
{
	Float	ofs ;

	ofs = -DOT_X( 14.0f ) ;
	if( pl_w->pl_dir ) ofs = DOT_X( 14.0f ) ;
	if( pl_w->pos.x <= pl_w->target_w->pos.x + ofs && pl_w->pl_dir ) return 0 ;
	if( pl_w->pos.x > pl_w->target_w->pos.x + ofs && !pl_w->pl_dir ) return 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->routine_2 = 1 ;
	pl_w->routine_3 = 2 ;
	pl_char_set( pl_w, Char_Super, 31 ) ;
	return 1 ;
}

static void wall_init( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void wall_run( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void wall_wall( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void wall_climb( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void wall_ceil( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void wall_cross( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void wall_wall2( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void wall_down( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void wall_land( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void wall_jump( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void wall_fall( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static Uint8 wall_off_ck( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Runs up the wall, across the top of the screen		*/
/*			and down the other side								*/
/*==============================================================*/
static void sp_wall( PLAYER_WORK *pl_w )
{
	static void ( *wall_jmp[])( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw ) = {
		wall_init, wall_run, wall_wall, wall_climb, wall_ceil, wall_cross,
		wall_wall2, wall_down, wall_land, wall_jump, wall_fall,
	} ;

	wall_jmp[pl_w->routine_2]( pl_w, ( PL14_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void wall_init( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_w->disp_dir = pl_w->disp_dir ^ 1 ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->atck_no = pl14_at_wall ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Stand, 2 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.x = pl_w->disp_dir ? 12.5f : -12.5f ;
	pl_w->spd.y = DOT_Y( 6.5f ) ;
	scr_excep_set( pl_w, 1, 7 ) ;
}

/*--------------------------------------------------------------*/
static void wall_run( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	scr_excep_set( pl_w, 1, 7 ) ;
	pl_w->push_ng_timer = 2 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_char_move( pl_w ) ;
	if( pl_w->disp_dir ){
		if( scr_work.scr_right + -DOT_X( 64.0f ) > pl_w->pos.x ) return ;
	} else if( pl_w->pos.x > scr_work.scr_left + DOT_X( 64.0f ) ) return ;
	pl_w->routine_2 ++ ;
	lw->wall_rot = 0 ;
	pl_w->timer0 = 20 ;
	pl_voice_req( pl_w, 3 ) ;
}

/*--------------------------------------------------------------*/
static void wall_wall( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	scr_excep_set( pl_w, 1, 7 ) ;
	lw->wall_rot -= 0x800 ;
	pl_w->rot[2] = lw->wall_rot ;
	pl_w->push_ng_timer = 2 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_char_move( pl_w ) ;
	if( pl_w->disp_dir ){
		if( scr_work.scr_right + -40.0f > pl_w->pos.x ) return ;
		pl_w->pos.x = scr_work.scr_right + -DOT_X( 2.0f ) ;
	} else {
		if( pl_w->pos.x > scr_work.scr_left + 40.0f ) return ;
		pl_w->pos.x = scr_work.scr_left + DOT_X( 2.0f ) ;
	}
	pl_w->pos.y += DOT_Y( 16.0f ) ;
	pl_w->routine_2 ++ ;
	pl_w->rot[2] = 0 ;
	pl_char_set( pl_w, Char_Special, 28 ) ;
	pl_air_set( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void wall_climb( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	scr_excep_set( pl_w, 1, 7 ) ;
	pl_w->push_ng_timer = 2 ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( wall_off_ck( pl_w ) ) return ;
	if( scr_work.scr_top + -DOT_Y( 64.0f ) > pl_w->pos.y ) return ;
	pl_w->routine_2 ++ ;
	lw->wall_rot = 0 ;
	pl_voice_req( pl_w, 3 ) ;
}

/*--------------------------------------------------------------*/
static void wall_ceil( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	scr_excep_set( pl_w, 1, 7 ) ;
	lw->wall_rot -= 0x800 ;
	pl_w->rot[2] = lw->wall_rot ;
	pl_w->push_ng_timer = 2 ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( wall_off_ck( pl_w ) ) return ;
	if( scr_work.scr_top + -DOT_Y( 24.0f ) > pl_w->pos.y ) return ;
	pl_w->pos.y = scr_work.scr_top + 0.0f ;
	if( pl_w->disp_dir ) pl_w->pos.x = scr_work.scr_right + -40.0f ;
	else pl_w->pos.x = scr_work.scr_left + 40.0f ;
	pl_w->routine_2 ++ ;
	pl_w->spd.x = -pl_w->spd.x ;
	pl_w->rot[2] = 0 ;
	pl_char_set( pl_w, Char_Special, 22 ) ;
}

/*--------------------------------------------------------------*/
static void wall_cross( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	scr_excep_set( pl_w, 1, 7 ) ;
	pl_w->push_ng_timer = 2 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_char_move( pl_w ) ;
	if( wall_off_ck( pl_w ) ) return ;
	if( pl_w->disp_dir ){
		if( pl_w->pos.x > scr_work.scr_left + DOT_X( 64.0f ) ) return ;
	} else if( scr_work.scr_right + -DOT_X( 64.0f ) > pl_w->pos.x ) return ;
	pl_w->routine_2 ++ ;
	lw->wall_rot = 0 ;
	pl_voice_req( pl_w, 3 ) ;
}

/*--------------------------------------------------------------*/
static void wall_wall2( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	scr_excep_set( pl_w, 1, 7 ) ;
	lw->wall_rot -= 0x800 ;
	pl_w->rot[2] = lw->wall_rot ;
	pl_w->push_ng_timer = 2 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_char_move( pl_w ) ;
	if( wall_off_ck( pl_w ) ) return ;
	if( pl_w->disp_dir ){
		if( pl_w->pos.x > scr_work.scr_left + 40.0f ) return ;
		pl_w->pos.x = scr_work.scr_left + DOT_X( 2.0f ) ;
	} else {
		if( scr_work.scr_right + -40.0f > pl_w->pos.x ) return ;
		pl_w->pos.x = scr_work.scr_right + -DOT_X( 2.0f ) ;
	}
	pl_w->routine_2 ++ ;
	pl_w->pos.y -= DOT_Y( 24.0f ) ;
	pl_w->spd.y = -pl_w->spd.y ;
	pl_w->wall_flag = 0 ;
	pl_w->rot[2] = 0 ;
	pl_char_set( pl_w, Char_Special, 29 ) ;
	lw->wall_rot = 0 ;
}

/*--------------------------------------------------------------*/
static void wall_down( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	scr_excep_set( pl_w, 1, 7 ) ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( wall_off_ck( pl_w ) ) return ;
	if( pl_w->pos.y > pl_w->ground_y + DOT_Y( 48.0f ) ) return ;
	pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void wall_land( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	scr_excep_set( pl_w, 1, 7 ) ;
	lw->wall_rot -= 0x800 ;
	pl_w->rot[2] = lw->wall_rot ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( wall_off_ck( pl_w ) ) return ;
	if( pl_w->pos.y > pl_w->ground_y + DOT_Y( 24.0f ) ) return ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	lw->wall_rot = 0 ;
	pl_w->rot[2] = 0 ;
	pl_voice_req( pl_w, 3 ) ;
	SCR_EXCEP_SET ;
	pl_stand_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void wall_jump( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ) pl_w->routine_2 ++ ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_land_ef_set( pl_w ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void wall_fall( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_land_ef_set( pl_w ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
/*			Jumps off the wall or dives at the opponent when	*/
/*			a button is pressed									*/
/*==============================================================*/
static Uint8 wall_off_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->cpu_flag ){
		if( -- pl_w->timer0 < 0 ){
			SCR_EXCEP_SET ;
			pl_w->rot[2] = 0 ;
			pl_w->routine_2 = 9 ;
			pl_w->pl_dir = 1 ;
			if( pl_w->pos.x > scr_work.scr_left + 320.0f ) pl_w->pl_dir = 0 ;
			pl_w->disp_dir = pl_w->pl_dir ;
			pl_char_set( pl_w, Char_Special, 25 ) ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_w->spd.y = -DOT_Y( 7.5f ) ;
			pl_w->spd.x = pl_w->pl_dir ? DOT_X( 12.5f ) : -DOT_X( 12.5f ) ;
			pl_se_req( pl_w, SE_SWING_M ) ;
			return 1 ;
		}
		return 0 ;
	}
	if( pl_w->pos.y <= pl_w->ground_y + DOT_Y( 64.0f ) ) return 0 ;
	if( pl_w->pad_sw.on & KEY_PUNCH ){
		SCR_EXCEP_SET ;
		pl_w->rot[2] = 0 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		if( pl_w->routine_2 == 5 ) pl_w->pos.y -= DOT_Y( 48.0f ) ;
		pl_dir_set( pl_w ) ;
		pl_w->sjump_flag = 2 ;
		pl_jump_ret( pl_w ) ;
		return 1 ;
	}
	if( pl_w->pad_sw.on & KEY_KICK ){
		SCR_EXCEP_SET ;
		pl_w->rot[2] = 0 ;
		pl_w->routine_2 = 9 ;
		pl_w->pl_dir = 1 ;
		if( pl_w->pos.x > scr_work.scr_left + 320.0f ) pl_w->pl_dir = 0 ;
		pl_w->disp_dir = pl_w->pl_dir ;
		pl_char_set( pl_w, Char_Special, 25 ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.y = -DOT_Y( 7.5f ) ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 12.5f ) : -DOT_X( 12.5f ) ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		return 1 ;
	}
	return 0 ;
}

/*==============================================================*/
/*			The taunt: its animation is in group 19, with		*/
/*			the win poses										*/
/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, 19, 9 ) ;
	} else if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void peach_init( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void peach_wait( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void peach_move( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void peach_hit( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void peach_end( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );

/*==============================================================*/
static void sp_peach( PLAYER_WORK *pl_w )
{
	static void ( *peach_jmp[])( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw ) = {
		peach_init, peach_wait, peach_move, peach_hit, peach_end,
	} ;

	if( pl_w->pl_stance == STANCE_AIR ){
		pl_jump_move( pl_w ) ;
		if( pl_w->pos.y < pl_w->ground_y ) pl_w->pos.y = pl_w->ground_y ;
	}
	peach_jmp[pl_w->routine_2]( pl_w, ( PL14_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void peach_init( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_w->timer0 = 120 ;
	pl_dir_set( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->atck_no = pl_w->atck_level ;
	HIT_RESET( pl_w ) ;
	pl_w->cg_type = 0 ;
	pl_char_set( pl_w, Char_Special, 18 ) ;
	if( ( pl_w->ef_w = shl14_03_set( pl_w, 0 ) ) == 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			Waits for the effect, for up to 120 frames			*/
/*==============================================================*/
static void peach_wait( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	PLAYER_WORK	*ew = pl_w->ef_w ;

	if( ew->routine_0 >= 2 ){
		pl_w->routine_2 = 3 ;
		pl_w->timer0 = 30 ;
		pl_se_req( pl_w, 42 ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 <= 0 ) pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void peach_move( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	pl_w->routine_2 ++ ;
	pl_char_set( pl_w, Char_Special, 19 ) ;
}

/*--------------------------------------------------------------*/
static void peach_hit( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type == 2 ){
		pl_w->cg_type = 0 ;
		vital_add_dmg( pl_w, 20 ) ;
		puff_ef_set( pl_w, 4 ) ;
		pl_voice_req( pl_w, 31 ) ;
		pl_se_req( pl_w, 42 ) ;
	}
}

/*--------------------------------------------------------------*/
static void peach_end( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 <= 0 ) pl_stand_ret( pl_w ) ;
}

static void snapback_init( PLAYER_WORK *pl_w );
static void snapback_move( PLAYER_WORK *pl_w );
static void snapback_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_snapback( PLAYER_WORK *pl_w )
{
	static void ( *snapback_jmp[])( PLAYER_WORK *pl_w ) = {
		snapback_init, snapback_move, snapback_end,
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
	pl_w->atck_no = pl14_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 23 ) ;
}

/*--------------------------------------------------------------*/
static void snapback_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) > 0 ) return ;
	pl_w->routine_2 ++ ;
	pl_char_set( pl_w, Char_Special, 24 ) ;
	pls14_01_set( pl_w, 4, 0 ) ;
	pls14_01_set( pl_w, 5, 0 ) ;
}

/*--------------------------------------------------------------*/
static void snapback_end( PLAYER_WORK *pl_w )
{
	VEC3	ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->pos.x += pl_w->pl_dir ? DOT_X( pl_w->cg_type ) : -DOT_X( pl_w->cg_type ) ;
		pl_w->cg_type = 0 ;
	}
	if( pl_w->cg_prm == 1 ){
		pl_w->cg_prm = 0 ;
		/* ofs.y is added to without being set first, as in the arcade */
		ofs.x = 0.0f ;
		ofs.y += DOT_Y( 48.0f ) ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
}

static void enou_auto_grow( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void enou_auto_start( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void enou_auto_walk( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void enou_auto_turn( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void enou_auto_punch( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void enou_auto_kick( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void enou_auto_rush( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void enou_auto_end( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static Uint8 enou_auto_atck_ck( PLAYER_WORK *pl_w );
static Uint8 enou_auto_walk_on( PLAYER_WORK *pl_w, Uint8 old );
static void enou_auto_stand_set( PLAYER_WORK *pl_w );
static Uint8 enou_auto_turn_ck( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			En'Ou when cpu_flag is set or as a team hyper:		*/
/*			the giant monkey mostly rushes						*/
/*==============================================================*/
static void sp_enou_auto( PLAYER_WORK *pl_w )
{
	static void ( *enou_auto_jmp[])( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw ) = {
		enou_auto_grow, enou_auto_start, enou_auto_walk, enou_auto_turn, enou_auto_punch, enou_auto_kick,
		enou_auto_rush, enou_auto_end, enou_shrink,
	} ;

	pl_w->hyper_bg_tm = 5 ;
	if( pl_w->routine_2 != 0 && pl_w->routine_2 != 8 ){
		pl_w->armor_flag |= ARMOR_HEAVY ;
		pl_w->nml_catch_ng_timer = 2 ;
		combo_reset( pl_w ) ;
	}
	enou_auto_jmp[pl_w->routine_2]( pl_w, ( PL14_LOCAL_W * )pl_w->tlocal ) ;
}

static void enou_auto_grow_end( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );

/*==============================================================*/
static void enou_auto_grow( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	static void ( *enou_auto_grow_jmp[])( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw ) = {
		enou_grow_init, enou_grow_wait, enou_grow_up, enou_auto_grow_end,
	} ;

	enou_auto_grow_jmp[pl_w->routine_3]( pl_w, ( PL14_LOCAL_W * )pl_w->tlocal ) ;
}

/*--------------------------------------------------------------*/
static void enou_auto_grow_end( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	if( !pl_w->cpu_flag || -- lw->step_timer <= 0 ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->armor_flag |= ARMOR_HEAVY ;
		pl_w->nml_catch_ng_timer = 2 ;
		combo_reset( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void enou_auto_start( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	pl_w->routine_2 = 6 ;
	pl_w->routine_3 = 0 ;
	lw->monkey_timer = 120 ;
	if( !( vs_sys_work.team_hyper_flag & ( 1 << pl_w->pl_no ) ) && pl_w->cpu_flag ){
		lw->monkey_timer = 150 ;
		Timer_Gauge_Set( pl_w, &lw->monkey_timer, 6 ) ;
	}
	pl_char_set( pl_w, Char_Super, 33 ) ;
	pl_se_req( pl_w, 76 ) ;
}

/*--------------------------------------------------------------*/
static void enou_auto_walk( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	if( enou_auto_atck_ck( pl_w ) ) return ;
	if( enou_auto_turn_ck( pl_w ) ) return ;
	if( !enou_auto_walk_on( pl_w, 0 ) ) enou_auto_stand_set( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
	} else if( pl_w->cg_type = 1 ){		/* an assignment, as in the arcade */
		pl_w->cg_type = 2 ;
		scr_work.quake_level = 1 ;
		scr_work.quake_flag = 1 ;
		pl_se_req( pl_w, 53 ) ;
	}
}

/*--------------------------------------------------------------*/
static void enou_auto_turn( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		enou_auto_stand_set( pl_w ) ;
		pl_dir_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void enou_auto_punch( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	if( pl_w->hit_flag || pl_w->cg_type == 3 ){
		pl_w->routine_2 = 5 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->atck_no = pl14_at_enou_lp ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 52 ) ;
		scr_work.quake_level = 1 ;
		scr_work.quake_flag = 1 ;
		return ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		enou_auto_stand_set( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type == 1 ){
		pl_w->cg_type = 0 ;
		pls14_01_set( pl_w, 22, 13 ) ;
		pls14_01_set( pl_w, 23, 13 ) ;
		pls14_01_set( pl_w, 24, 13 ) ;
	} else if( pl_w->cg_type == 2 ){
		pl_w->cg_type = 0 ;
		pl_w->pos.x += pl_w->disp_dir ? DOT_X( 16.0f ) : -DOT_X( 16.0f ) ;
	}
}

/*--------------------------------------------------------------*/
static void enou_auto_kick( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 = 8 ;
		pl_w->routine_3 = 0 ;
		return ;
	}
	if( pl_w->cg_type == 1 ){
		pl_w->cg_type = 0 ;
		pls14_01_set( pl_w, 32, 13 ) ;
		pls14_01_set( pl_w, 33, 13 ) ;
	} else if( pl_w->cg_type == 2 ){
		pl_w->cg_type = 0 ;
		pl_w->pos.x += pl_w->disp_dir ? -DOT_X( 40.0f ) : DOT_X( 40.0f ) ;
	}
}

static void enou_auto_rush_init( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void enou_auto_rush_mid( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void enou_auto_rush_down( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void enou_auto_rush_up( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void enou_auto_rush_bottom( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void enou_auto_rush_top( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );
static void enou_auto_rush_loop( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw );

/*==============================================================*/
/*			A set run of blows until the time is up				*/
/*==============================================================*/
static void enou_auto_rush( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	static void ( *enou_auto_rush_jmp[])( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw ) = {
		enou_auto_rush_init, enou_auto_rush_down, enou_auto_rush_bottom, enou_auto_rush_down, enou_auto_rush_mid,
		enou_auto_rush_up, enou_auto_rush_top, enou_auto_rush_up, enou_auto_rush_loop,
	} ;

	enou_auto_rush_jmp[pl_w->routine_3]( pl_w, ( PL14_LOCAL_W * )pl_w->tlocal ) ;
	if( vs_sys_w->vs_st >= VS_END_WAIT && !flash_flag_get() ) lw->monkey_timer = 0 ;
	if( -- lw->monkey_timer <= 0 ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		ef_color_set( pl_w, 0, pl_w->pl_color + 6, 1 ) ;
		pl_char_set( pl_w, Char_Super, 37 ) ;
		return ;
	}
	if( !( Sys->game_timer0 & 1 ) ) ef_color_set( pl_w, 0, pl_w->pl_color * 2 + 20, 1 ) ;
	else ef_color_set( pl_w, 0, pl_w->pl_color * 2 + 21, 1 ) ;
	if( !( Sys->game_timer0 & 3 ) ) dust_set( pl_w, 0, 0, 3.8f, 0.4f ) ;
}

/*==============================================================*/
static void enou_auto_rush_init( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		scr_work.quake_level = 2 ;
		scr_work.quake_flag = 1 ;
		pl_w->atck_no = pl14_at_enou_rush ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 41 ) ;
		shl14_04_set( pl_w, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void enou_auto_rush_mid( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Super, 41 ) ;
	}
}

/*--------------------------------------------------------------*/
static void enou_auto_rush_down( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Super, 43 ) ;
	}
}

/*--------------------------------------------------------------*/
static void enou_auto_rush_up( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Super, 39 ) ;
	}
}

/*--------------------------------------------------------------*/
static void enou_auto_rush_bottom( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Super, 44 ) ;
	}
}

/*--------------------------------------------------------------*/
static void enou_auto_rush_top( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Super, 38 ) ;
	}
}

/*--------------------------------------------------------------*/
static void enou_auto_rush_loop( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 = 1 ;
		pl_char_set( pl_w, Char_Super, 41 ) ;
	}
}

/*--------------------------------------------------------------*/
static void enou_auto_end( PLAYER_WORK *pl_w, PL14_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 = 8 ;
		pl_w->routine_3 = 0 ;
	}
}

/*==============================================================*/
/*			As enou_atck_ck, but it sets routine_3 for the		*/
/*			punch and kick										*/
/*==============================================================*/
static Uint8 enou_auto_atck_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->pad_sw.on & KEY_LP ){
		pl_w->routine_3 = 4 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->atck_no = pl14_at_enou_lp ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 32 ) ;
		scr_work.quake_level = 1 ;
		scr_work.quake_flag = 1 ;
		return 1 ;
	}
	if( pl_w->pad_sw.on & KEY_HP ){
		pl_w->routine_3 = 5 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->atck_no = pl14_at_enou_hp ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 52 ) ;
		scr_work.quake_level = 1 ;
		scr_work.quake_flag = 1 ;
		return 1 ;
	}
	if( !( pl_w->pad_sw.on & KEY_LK ) ) return 0 ;
	pl_w->routine_2 = 6 ;
	pl_w->routine_3 = 0 ;
	pl_w->timer1 = 0 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_char_set( pl_w, Char_Super, 33 ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 enou_auto_walk_on( PLAYER_WORK *pl_w, Uint8 old )
{
	Uint16	sw = old ? pl_w->pl_now_lvbt : pl_w->pl_new_lvbt ;

	if( !( sw & ( KEY_FORWARD | KEY_BACK ) ) ) return 0 ;
	return 1 ;
}

/*==============================================================*/
static void enou_auto_stand_set( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = 1 ;
	pl_w->routine_3 = 0 ;
	pl_char_set( pl_w, Char_Super, 28 ) ;
}

/*==============================================================*/
static Uint8 enou_auto_turn_ck( PLAYER_WORK *pl_w )
{
	Float	ofs ;

	ofs = -DOT_X( 14.0f ) ;
	if( pl_w->pl_dir ) ofs = DOT_X( 14.0f ) ;
	if( pl_w->pos.x <= pl_w->target_w->pos.x + ofs && pl_w->pl_dir ) return 0 ;
	if( pl_w->pos.x > pl_w->target_w->pos.x + ofs && !pl_w->pl_dir ) return 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->routine_2 = 3 ;
	pl_w->routine_3 = 0 ;
	pl_char_set( pl_w, Char_Super, 31 ) ;
	return 1 ;
}

static PLAYER_WORK *nml_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *crouch_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *jump_catch_ck( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Can a throw start?									*/
/****************************************************************/
PLAYER_WORK *pl14_catch_ck( PLAYER_WORK *pl_w )
{
	static PLAYER_WORK *( *catch_ck_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_catch_ck, crouch_catch_ck, jump_catch_ck, nml_catch_ck,
	} ;

	return catch_ck_jmp[pl_w->pl_stance]( pl_w ) ;
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
	if( pl_w->atck_pk == 1 && pl_w->atck_level == 1 ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 1 ;
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
	if( pl_w->atck_pk == 0 && pl_w->atck_level == 1 ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 2 ;
			return em ;
		}
		return 0 ;
	}
	return 0 ;
}

static void nml_p_set( PLAYER_WORK *pl_w );
static void nml_k_set( PLAYER_WORK *pl_w );
static void jump_p_set( PLAYER_WORK *pl_w );
static void sp_set( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Start the throw that pl14_catch_ck chose			*/
/****************************************************************/
void	pl14_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_set, nml_k_set, jump_p_set, sp_set,
	} ;

	catch_set_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	ofs.x = -DOT_X( 32.0f ) ;
	ofs.y = DOT_Y( 80.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	catch_gauge_add( pl_w ) ;
	scr_excep_set( pl_w, 13, 6 ) ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_char_set( pl_w, Char_Catch, 0 ) ;
}

/*==============================================================*/
static void nml_k_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_w->dir & 2 ){
		pl_w->pl_dir ^= 1 ;
		pl_w->disp_dir = pl_w->pl_dir ;
	}
	ofs.x = -DOT_X( 32.0f ) ;
	ofs.y = DOT_Y( 80.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	catch_gauge_add( pl_w ) ;
	scr_excep_set( pl_w, 13, 7 ) ;
	pl_char_set( pl_w, Char_Catch, 5 ) ;
	pl_voice_req( pl_w, 4 ) ;
}

/*==============================================================*/
static void jump_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	ofs.x = -DOT_X( 32.0f ) ;
	ofs.y = DOT_Y( 80.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	catch_gauge_add( pl_w ) ;
	scr_excep_set( pl_w, 13, 6 ) ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_char_set( pl_w, Char_Catch, 3 ) ;
	pl_voice_req( pl_w, 3 ) ;
}

/*==============================================================*/
static void sp_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	ofs.x = -DOT_X( 32.0f ) ;
	ofs.y = DOT_Y( 80.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	catch_gauge_add( pl_w ) ;
	scr_excep_set( pl_w, 5, 5 ) ;
}

static void p_catch( PLAYER_WORK *pl_w );
static void k_catch( PLAYER_WORK *pl_w );
static void sp_catch( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void	pl14_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		p_catch, k_catch, p_catch, sp_catch,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

static void p_catch_lift( PLAYER_WORK *pl_w );
static void p_catch_fall( PLAYER_WORK *pl_w );
static void p_catch_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The opponent is carried up and thrown down			*/
/*==============================================================*/
static void p_catch( PLAYER_WORK *pl_w )
{
	static void ( *p_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		p_catch_lift, p_catch_fall, p_catch_end,
	} ;

	p_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void p_catch_lift( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type == 1 ){
		pl_w->cg_type = 3 ;
		em = pl_w->catch_w ;
		em->atck_no = pl14_at_throw_p ;
		catch_damage( pl_w ) ;
		ofs.x = 0.0f ;
		ofs.y = 0.0f ;
		hit_mark_set( pl_w, &ofs, 1 ) ;
		pl_se_req( pl_w, 14 ) ;
	}
	if( pl_w->cg_type == 2 ){
		em = pl_w->catch_w ;
		pl_w->routine_2 ++ ;
		em->dm_hit_w = pl_w ;
		em->pl_dir = pl_w->pl_dir ^ 1 ;
		em->atck_no = pl14_at_throw_p_2 ;
		em->catch_type = 1 ;
		scr_excep_clr() ;
		pls14_02_set( pl_w, 31, 0 ) ;
		pl_w->pos.x += pl_w->disp_dir ? -DOT_X( 64.0f ) : DOT_X( 64.0f ) ;
		pl_w->pos.y += -DOT_Y( 72.0f ) ;
		pl_w->spd.y = DOT_Y( 5.5f ) ;
		pl_w->acc.y = -0.9040178f ;
	}
}

/*--------------------------------------------------------------*/
static void p_catch_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_land_ef_set( pl_w ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Catch, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void p_catch_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void k_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_flag1 ){
		pl_w->cg_flag1 = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->pl_dir = pl_w->pl_dir ;
		em->atck_no = pl14_at_throw_k ;
		em->catch_type = 16 ;
	}
}

static void sp_catch_init( PLAYER_WORK *pl_w );
static void sp_catch_spin( PLAYER_WORK *pl_w );
static void sp_catch_drop( PLAYER_WORK *pl_w );
static void sp_catch_shot( PLAYER_WORK *pl_w );
static void sp_catch_hit( PLAYER_WORK *pl_w );
static void sp_catch_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The opponent is pulled in, spun and shot away		*/
/*==============================================================*/
static void sp_catch( PLAYER_WORK *pl_w )
{
	static void ( *sp_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		sp_catch_init, sp_catch_spin, sp_catch_drop, sp_catch_shot,
		sp_catch_hit, sp_catch_end,
	} ;

	sp_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void sp_catch_init( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 pos ;

	pl_w->routine_2 ++ ;
	pl_char_move( pl_w ) ;
	pl_w->cg_prm = 0 ;
	pl_w->timer0 = 24 ;
	pl_w->catch_w = pl_w->em_w ;
	em = pl_w->catch_w ;
	pos = em->pos ;
	catch_pos_set( pl_w, em ) ;
	em->spd.x = ( em->pos.x - pos.x ) / 24.0f ;
	em->acc.x = 0.0f ;
	em->acc.y = -DOT_Y( 0.5f ) ;
	em->spd.y = ( em->pos.y - pos.y ) / 24.0f - em->acc.y * 24.0f / 2.0f ;
	em->pos = pos ;
	em->disp_ofs_x = 0 ;
	em->disp_ofs_y = 102 ;
}

/*--------------------------------------------------------------*/
static void sp_catch_spin( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	em = pl_w->catch_w ;
	em->rot[2] += 0x2000 ;
	em->scale.x -= 0.041666668f ;
	em->scale.y -= 0.041666668f ;
	pl_w->catch_w->pos.x += pl_w->catch_w->spd.x ;
	pl_w->catch_w->spd.x += pl_w->catch_w->acc.x ;
	pl_w->catch_w->pos.y += pl_w->catch_w->spd.y ;
	pl_w->catch_w->spd.y += pl_w->catch_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 <= 0 ){
		pl_w->routine_2 ++ ;
		em->disp_flag = 0 ;
		em->cg_shadow = 0xff ;
		pl_char_set( pl_w, Char_Special, 15 ) ;
	}
}

/*--------------------------------------------------------------*/
static void sp_catch_drop( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	em = pl_w->catch_w ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		if( pl_w->disp_dir ){
			if( pl_w->pos.x > scr_work.scr_right + -DOT_X( 128.0f ) ) pl_w->disp_dir = 0 ;
		} else {
			if( pl_w->pos.x < scr_work.scr_left + DOT_X( 128.0f ) ) pl_w->disp_dir = 1 ;
		}
		em->pos.x = pl_w->pos.x ;
		em->pos.x += pl_w->disp_dir ? DOT_X( 112.0f ) : -DOT_X( 112.0f ) ;
		pl_char_set( pl_w, Char_Special, 30 ) ;
	}
}

/*--------------------------------------------------------------*/
static void sp_catch_shot( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		if( !( pl_w->ef_w = pls14_01_set( pl_w, 9, 2 ) ) ){
			em = pl_w->catch_w ;
			em->disp_flag = 1 ;
			em->scale.x = 1.0f ;
			em->scale.y = 1.0f ;
			em->dm_hit_w = pl_w ;
			em->catch_type = 1 ;
			scr_excep_clr() ;
			pl_stand_ret( pl_w ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void sp_catch_hit( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ef, *em ;
	VEC3 pos ;

	pl_char_move( pl_w ) ;
	ef = pl_w->ef_w ;
	if( ef->cg_type ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, 16 ) ;
		em = pl_w->catch_w ;
		em->disp_flag = 1 ;
		em->scale.x = 1.0f ;
		em->scale.y = 1.0f ;
		em->atck_no = pl14_at_hisago_2 ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		scr_excep_y_clr() ;
		pl_voice_req( pl_w, 43 ) ;
		pos.x = pl_w->pos.x ;
		pos.x += pl_w->disp_dir ? DOT_X( 112.0f ) : -DOT_X( 112.0f ) ;
		pos.y = pl_w->pos.y ;
		pos.y += 80.0f ;
		burst_pos_set( &pos, 1 ) ;
		pl_se_req( pl_w, 73 ) ;
		scr_excep_set( pl_w, 13, 7 ) ;
	}
}

/*--------------------------------------------------------------*/
static void sp_catch_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		scr_excep_set( pl_w, 13, 0 ) ;
		pl_stand_ret( pl_w ) ;
	}
}

static void air_dash_init( PLAYER_WORK *pl_w );
static void air_dash_start( PLAYER_WORK *pl_w );
static void air_dash_move( PLAYER_WORK *pl_w );
static void air_dash_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A dash through the air								*/
/****************************************************************/
void	pl14_air_dash( PLAYER_WORK *pl_w )
{
	static void ( *air_dash_jmp[])( PLAYER_WORK *pl_w ) = {
		air_dash_init, air_dash_start, air_dash_move, air_dash_end,
	} ;

	air_dash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void air_dash_init( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ef ;
	register Sint8 no ;

	pl_w->routine_2 ++ ;
	pl_w->timer0 = 16 ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->spd.y = 0.0f ;
	pl_w->spd.x = -DOT_X( 9.5f ) ;
	pl_w->acc.x = 0.3125f ;
	no = 5 ;
	if( pl_w->disp_dir ){
		pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = -pl_w->acc.x ;
	}
	if( pl_w->dir & 1 ){
		no = 6 ;
		pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = -pl_w->acc.x ;
	}
	pl_char_set( pl_w, Char_Dash, no ) ;
	if( ( ef = pls14_01_set( pl_w, 15, 8 ) ) ){
		ef->spd.x = pl_w->spd.x ;
		ef->disp_dir = pl_w->disp_dir ;
		ef->dir = pl_w->dir ;
	}
	pl_voice_req( pl_w, 30 ) ;
	pl_voice_req( pl_w, 23 ) ;
	pl_w->sp_ng_timer = 10 ;
	pl_w->atck_ng_timer = 10 ;
}

/*--------------------------------------------------------------*/
static void air_dash_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ) pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void air_dash_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( -- pl_w->timer0 <= 0 ){
		pl_w->routine_2 ++ ;
		pl_w->acc.x = 0.0f ;
	}
}

/*--------------------------------------------------------------*/
static void air_dash_end( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_dir_set( pl_w ) ;
		pl_jump_ret( pl_w ) ;
	}
}

static void p_hold( PLAYER_WORK *pl_w );
static void k_hold( PLAYER_WORK *pl_w );
static void sp_hold( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Holding the opponent								*/
/****************************************************************/
void	pl14_hold( PLAYER_WORK *pl_w )
{
	static void ( *hold_jmp[])( PLAYER_WORK *pl_w ) = {
		p_hold, k_hold, p_hold, sp_hold,
	} ;

	hold_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void p_hold( PLAYER_WORK *pl_w )
{
	catch_em_pos_set0( pl_w->catch_w, pl_w ) ;
}

/*==============================================================*/
static void k_hold( PLAYER_WORK *pl_w )
{
	catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/*==============================================================*/
static void sp_hold( PLAYER_WORK *pl_w )
{
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void	pl14_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl14_sp_enou_auto ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl14_sp_enou_auto ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl14_sp_tenchi ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl14_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl14_sp_enou_auto ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl14_sp_enou_auto ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl14_sp_tenchi ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl14_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl14_sp_peach ;
		pl_w->atck_level = 0 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl14_sp_shienbu ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl14_sp_seiten ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void	pl14_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl14_sp_fuusetsu ;
		pl_w->atck_level = 0 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl14_sp_shienbu ;
		pl_w->atck_level = 0 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl14_sp_seiten ;
		pl_w->atck_level = 0 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_move( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A kick that leaps off the wall						*/
/****************************************************************/
void	pl14_t_kick( PLAYER_WORK *pl_w )
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
	pl_w->atck_no = pl14_at_t_kick ;
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
		pl_char_set( pl_w, Char_Extra, 2 ) ;
		pl_land_ef_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void t_kick_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_stop_stand_ret( pl_w ) ;
	} else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
	}
}

/****************************************************************/
/*			Clear SonSon's own work, all but shot_cnt			*/
/****************************************************************/
void	pl14_tlocal_init( PLAYER_WORK *pl_w )
{
	PL14_LOCAL_W *lw = ( PL14_LOCAL_W * )pl_w->tlocal ;
	Sint16 keep = lw->shot_cnt ;

	memset( &pl_w->tlocal, 0, sizeof( pl_w->tlocal ) ) ;
	lw->shot_cnt = keep ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl14.c" End of File	*/

/* Omega Red (character 30): command checks, normal attacks, appearance, win and lose poses, special
   moves and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl30_def.h"

void pl30_main( PLAYER_WORK *pl_w );
void pl30_com_ck( PLAYER_WORK *pl_w );
void pl30_atck_set( PLAYER_WORK *pl_w );
void pl30_atck( PLAYER_WORK *pl_w );
void pl30_dash( PLAYER_WORK *pl_w );
void pl30_noki( PLAYER_WORK *pl_w );
void pl30_appear( PLAYER_WORK *pl_w );
void pl30_win( PLAYER_WORK *pl_w );
void pl30_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl30_catch_ck( PLAYER_WORK *pl_w );
void pl30_catch_set( PLAYER_WORK *pl_w );
void pl30_catch( PLAYER_WORK *pl_w );
void pl30_hold( PLAYER_WORK *pl_w );
int pl30_delay_ck( PLAYER_WORK *pl_w );
void pl30_t_kick( PLAYER_WORK *pl_w );
void pl30_upper( PLAYER_WORK *pl_w );
void pl30_sp_unison( PLAYER_WORK *pl_w );
void pl30_sp_gattai( PLAYER_WORK *pl_w );
void pl30_sp_assist( PLAYER_WORK *pl_w );
void pl30_sp_zero( PLAYER_WORK *pl_w );
void pl30_air_dash( PLAYER_WORK *pl_w );

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
static Uint16 com_destroyer[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl30_sp_destroyer, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Omega Destroyer */
static Uint16 com_jdestroyer[] = { 3, COM_AIR | COM_HYPER | COM_ALL_SW, pl30_sp_jdestroyer, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Omega Destroyer in the air */
static Uint16 com_coil[] = { 3, 0, pl30_sp_coil, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Carbonadium Coil */
static Uint16 com_coil2[] = { 3, COM_ALL_SW, pl30_sp_coil2, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Carbonadium Coil with both punches */
static Uint16 com_jcoil[] = { 3, COM_AIR, pl30_sp_jcoil, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Kuuchuu Carbonadium Coil (air, kick) */
static Uint16 com_jcoil2[] = { 3, COM_AIR | COM_ALL_SW, pl30_sp_jcoil2, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Kuuchuu Carbonadium Coil, both kicks */
static Uint16 com_strike[] = { 3, COM_GROUND, pl30_sp_strike, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Omega Strike */
static Uint16 com_strike2[] = { 3, COM_GROUND | COM_ALL_SW, pl30_sp_strike2, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Omega Strike with both kicks */

static void ( *pl30_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl30_main,
	pl30_com_ck,
	pl30_atck_set,
	pl30_atck,
	pl30_dash,
	pl30_noki,
	pl30_appear,
	pl30_win,
	pl30_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl30_catch_ck,	/* returns a value */
	pl30_catch_set,
	pl30_catch,
	pl30_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl30_delay_ck,
	pl30_t_kick,
	pl30_upper,
	pl30_sp_unison,
	pl30_sp_gattai,
	pl30_sp_assist,
	pl30_sp_zero,
	0,
	pl30_air_dash,
} ;

static int destroyer_ck( PLAYER_WORK *pl_w );
static int coil_ck( PLAYER_WORK *pl_w );
static int coil2_ck( PLAYER_WORK *pl_w );
static int jcoil_ck( PLAYER_WORK *pl_w );
static int jcoil2_ck( PLAYER_WORK *pl_w );
static int strike_ck( PLAYER_WORK *pl_w );
static int strike2_ck( PLAYER_WORK *pl_w );
static int jdestroyer_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static int chouhatsu_ck( PLAYER_WORK *pl_w );
static int destroyer_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl30_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl30_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl30_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( destroyer_ck( pl_w ) ) return ;
	if( jdestroyer_ck( pl_w ) ) return ;
	if( coil2_ck( pl_w ) ) return ;
	if( coil_ck( pl_w ) ) return ;
	if( jcoil2_ck( pl_w ) ) return ;
	if( jcoil_ck( pl_w ) ) return ;
	if( strike2_ck( pl_w ) ) return ;
	if( strike_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( pl_air_dash_ck( pl_w, &pl_w->com_w3 ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
static int destroyer_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_destroyer, &pl_w->com_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl30_sp_destroyer ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
/*			The strong version on the ground, or in the air		*/
/*==============================================================*/
static int coil_ck( PLAYER_WORK *pl_w )
{
	Uint8 level ;

	if( !com_nml_ck( pl_w, com_coil, &pl_w->com_w0 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	if( pl_w->atck_level == 1 ) level = 1 ;
	else level = 2 ;
	if( pl_w->pl_stance == STANCE_AIR ) pl_w->pl_sp_type = pl30_sp_jcoil ;
	else pl_w->pl_sp_type = pl30_sp_coil ;
	pl_w->atck_level = level ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int coil2_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_coil2, &pl_w->com_w4 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	if( pl_w->pl_stance == STANCE_AIR ) pl_w->pl_sp_type = pl30_sp_jcoil ;
	else pl_w->pl_sp_type = pl30_sp_coil ;
	pl_w->atck_level = 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			In the air											*/
/*==============================================================*/
static int jcoil_ck( PLAYER_WORK *pl_w )
{
	Uint8 level ;

	if( !com_nml_ck( pl_w, com_jcoil, &pl_w->com_w1 ) ) return 0 ;
	if( !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	if( pl_w->atck_level == 1 ) level = 3 ;
	else level = 2 ;
	pl_w->atck_level = level ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl30_sp_jcoil ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int jcoil2_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_jcoil2, &pl_w->com_w6 ) ) return 0 ;
	if( !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	pl_w->atck_level = 4 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl30_sp_jcoil ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int strike_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_strike, &pl_w->com_w2 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	if( pl_w->atck_level == 1 ) pl_w->atck_level = 1 ;
	else pl_w->atck_level = 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl30_sp_strike ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int strike2_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_strike2, &pl_w->com_w7 ) ) return 0 ;
	pl_w->atck_level = 2 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl30_sp_strike ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int jdestroyer_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_jdestroyer, &pl_w->com_w5 ) || !*pl_w->gauge_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 9 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl30_sp_jdestroyer ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static int snapback_ck( PLAYER_WORK *pl_w )
{
	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl30_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static int chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 6 ) ) return 0 ;
	pl_w->pl_sp_type = pl30_sp_chouhatsu ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/****************************************************************/
/*			A hyper combo entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl30_delay_ck( PLAYER_WORK *pl_w )
{
	if( destroyer_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static int destroyer_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_destroyer, &pl_w->com_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl30_sp_destroyer ;
	return 1 ;
}

/****************************************************************/
void pl30_main( PLAYER_WORK *pl_w )
{
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
void pl30_atck_set( PLAYER_WORK *pl_w )
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
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_CROUCH_HP ;
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
		pl_w->chain_tbl = hk_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Stand_K, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
/*			The heavy one sweeps with the stick's way			*/
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
		pl_char_set( pl_w, Char_Crouch_K, pl_w->char_no.b.no ) ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_CROUCH_MK ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_w->chain_tbl = mk_tbl ;
		pl_w->nml_level = 1 ;
		pl_char_set( pl_w, Char_Crouch_K, pl_w->char_no.b.no ) ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_CROUCH_HK ;
		pl_w->chain_tbl = hk_tbl ;
		pl_w->nml_level = 2 ;
		pl_se_req( pl_w, SE_SWING_H ) ;
		pl_char_set( pl_w, Char_Crouch_K, pl_w->char_no.b.no ) ;
		shl30_02_set( pl_w, 0, ( pl_w->atck_lever & ( KEY_FORWARD | KEY_BACK ) ) >> 10 ) ;
		pl_w->timer0 = 0x100 ;
		break ;
	}
	HIT_RESET( pl_w ) ;
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
	Uint8 se ;

	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_JUMP_LK ;
		se = 20 ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_lk_tbl ;
		else pl_w->chain_tbl = sj_lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_JUMP_MK ;
		se = 21 ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mk_tbl ;
		else pl_w->chain_tbl = sj_mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_JUMP_HK ;
		se = 22 ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_hk_tbl ;
		else pl_w->chain_tbl = sj_hk_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	pl_se_req( pl_w, se ) ;
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
void pl30_atck( PLAYER_WORK *pl_w )
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
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			The heavy one hits again							*/
/*==============================================================*/
static void crouch_punch_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 2:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		if( pl_w->cg_flag1 ){
			pl_w->cg_flag1 = 0 ;
			pl_w->atck_no = ATCK_CROUCH_HP ;
			HIT_RESET( pl_w ) ;
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
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			The heavy one holds its last frame for a while		*/
/*==============================================================*/
static void crouch_kick_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	case 2:
		if( !pl_w->routine_2 ){
			pl_char_move( pl_w ) ;
			if( !pl_w->timer0 ){
				pl_w->routine_2 ++ ;
				pl_char_set( pl_w, Char_Extra, 1 ) ;
			} else pl_w->timer0 -- ;
		} else if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
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
static void jump_punch_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
	if( pl_w->nml_no == 2 && pl_w->cg_flag1 ){
		pl_w->cg_flag1 = 0 ;
		pl_w->atck_no = ATCK_JUMP_HP ;
		HIT_RESET( pl_w ) ;
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
void pl30_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Extra, 17 ) ;
		pl_w->atck_no = pl30_at_upper ;
		HIT_RESET( pl_w ) ;
		pl_gauge_add( pl_w, 5 ) ;
		pl_se_req( pl_w, SE_SWING_M ) ;
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
void pl30_dash( PLAYER_WORK *pl_w )
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
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = -DOT_X( 8.0f ) ;
		pl_w->acc.x = 0.3125f ;
		if( pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		pl_w->timer0 = 16 ;
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
	if( --pl_w->timer0 <= 0 ) pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void dash_end( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_brake_stop( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		RATE_CLR_X( pl_w ) ;
		pl_stand_ret( pl_w ) ;
	}
}

static void noki_init( PLAYER_WORK *pl_w );
static void noki_jump( PLAYER_WORK *pl_w );
static void noki_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash, a hop								*/
/****************************************************************/
void pl30_noki( PLAYER_WORK *pl_w )
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
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.y = DOT_Y( 4.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_w->spd.x = !pl_w->pl_dir ? 10.0f : -10.0f ;
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
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Dash, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void noki_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void appear_init( PLAYER_WORK *pl_w );
static void appear_wait( PLAYER_WORK *pl_w );
static void appear_move( PLAYER_WORK *pl_w );
static void appear_end( PLAYER_WORK *pl_w );
static void win_init( PLAYER_WORK *pl_w );
static void win_move( PLAYER_WORK *pl_w );
static void win_ko_init( PLAYER_WORK *pl_w );
static void lose_init( PLAYER_WORK *pl_w );
static void win_time_init( PLAYER_WORK *pl_w );
static void win_ko_move( PLAYER_WORK *pl_w );
static void lose_move( PLAYER_WORK *pl_w );
static void win_time_move( PLAYER_WORK *pl_w );
static void win_ko1_start( PLAYER_WORK *pl_w );
static void win_ko1_move( PLAYER_WORK *pl_w );
static void win_ko1_next( PLAYER_WORK *pl_w );

/****************************************************************/
void pl30_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_init,
		appear_wait,
		appear_move,
		appear_end,
	} ;

	appear_jmp[ pl_w->routine_2 ]( pl_w ) ;
}

/*==============================================================*/
static void appear_init( PLAYER_WORK *pl_w )
{
	pl_w -> routine_2 ++ ;
	pl_w -> disp_flag = 1 ;
	ef_color_set( pl_w, 0, pl_w->pl_color * 4, 1 ) ;
	ef_color_set( pl_w, 1, 24, 1 ) ;
	pl_char_set( pl_w, Char_Appear, 0 ) ;
}

/*==============================================================*/
/*			Wait for the round's start							*/
/*==============================================================*/
static void appear_wait( PLAYER_WORK *pl_w )
{
	if( vs_sys_w->vs_st >= VS_APPEAR ){
		pl_w -> routine_2 ++ ;
		shl30_02_set( pl_w, 2, 0 ) ;
		shl30_02_set( pl_w, 2, 1 ) ;
		pl_voice_req( pl_w, 31 ) ;
	}
}

/*==============================================================*/
/*			The colour the animation asks for					*/
/*==============================================================*/
static void appear_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type > 0 ){
		ef_color_set( pl_w, 0, pl_w->cg_type + pl_w->pl_color * 4, 1 ) ;
		pl_w->cg_type = 0 ;
	} else if( pl_w->cg_type < 0 ){
		pl_w -> routine_2 ++ ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w -> routine_1 ++ ;
	}
}

/****************************************************************/
void pl30_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_init,
		win_move,
	} ;

	win_jmp[ pl_w->routine_2 ]( pl_w ) ;
}

/*==============================================================*/
static void win_init( PLAYER_WORK *pl_w )
{
	static void ( *win_init_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko_init,
		lose_init,
		win_time_init,
		lose_init,
		lose_init,
	} ;

	pl_w -> routine_2 ++ ;
	win_init_jmp[ pl_w->type ]( pl_w ) ;
}

/*==============================================================*/
/*			Won by KO: one of two poses							*/
/*==============================================================*/
static void win_ko_init( PLAYER_WORK *pl_w )
{
	pl_w->timer0 = 0 ;
	pl_w->char_no.b.no = CH30_WIN_KO0 ;
	if( Sys->game_timer0 & 1 ){
		pl_w->timer0 = 1 ;
		pl_w->char_no.b.no = CH30_WIN_KO1 ;
	}
	pl_char_set( pl_w, Char_Win, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
static void win_time_init( PLAYER_WORK *pl_w )
{
	pl_char_set( pl_w, Char_Win, CH30_WIN_TIME ) ;
}

/*==============================================================*/
static void lose_init( PLAYER_WORK *pl_w )
{
	pl_char_set( pl_w, Char_Win, CH30_LOSE ) ;
}

/*--------------------------------------------------------------*/
static void win_move( PLAYER_WORK *pl_w )
{
	static void ( *win_move_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko_move,
		lose_move,
		win_time_move,
		lose_move,
		lose_move,
	} ;

	if( pl_win_end_ck( pl_w ) ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	win_move_jmp[ pl_w->type ]( pl_w ) ;
}

/*==============================================================*/
static void win_ko_move( PLAYER_WORK *pl_w )
{
	static void ( *win_ko1_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko1_start,
		win_ko1_move,
		win_ko1_next,
		win_ko1_move,
	} ;

	if( !pl_w->timer0 ){
		pl_char_move( pl_w ) ;
	}else{
		win_ko1_jmp[ pl_w->routine_3 ]( pl_w ) ;
	}
}

/*==============================================================*/
static void win_ko1_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_3 ++ ;
		shl30_02_set( pl_w, 7, 0 ) ;
		shl30_02_set( pl_w, 7, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void win_ko1_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_ko1_next( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_char_set( pl_w, Char_Win, 1 ) ;
}

/*==============================================================*/
static void win_time_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void lose_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shl30_02_set( pl_w, 9, 0 ) ;
	}
	if( pl_w->cg_flag1 ){
		pl_w->cg_flag1 = 0 ;
		shl30_02_set( pl_w, 9, 1 ) ;
	}
}

static void sp_destroyer( PLAYER_WORK *pl_w );
static void sp_coil( PLAYER_WORK *pl_w );
static void sp_jcoil( PLAYER_WORK *pl_w );
static void sp_strike( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );
static void sp_burrow( PLAYER_WORK *pl_w );
static void sp_jdestroyer( PLAYER_WORK *pl_w );
static void sp_counter_a( PLAYER_WORK *pl_w );
static void sp_counter_g( PLAYER_WORK *pl_w );

/****************************************************************/
void	pl30_sp_atck( PLAYER_WORK *pl_w )
{
	static void (*pl_sp_tbl[])( PLAYER_WORK *pl_w ) ={
		sp_destroyer,					/* 00 Omega Destroyer (hyper) */
		sp_coil,					/* 01 */
		sp_coil,					/* 02 Carbonadium Coil */
		sp_jcoil,					/* 03 Kuuchuu Carbonadium Coil (air, kick) */
		sp_strike,					/* 04 Omega Strike */
		sp_coil,					/* 05 */
		sp_chouhatsu,					/* 06 the taunt */
		sp_snapback,					/* 07 the snapback */
		sp_coil,					/* 08 */
		sp_burrow,					/* 09 Burrowing Coil (gamma assist) */
		sp_jdestroyer,					/* 0a Omega Destroyer in the air (hyper) */
		sp_counter_a,					/* 0b the alpha type's Variable Counter */
		sp_counter_g,					/* 0c the gamma type's Variable Counter */
		sp_coil,					/* 0d Carbonadium Coil with both punches */
		sp_jcoil,					/* 0e Kuuchuu Carbonadium Coil, both kicks */
		sp_strike,					/* 0f Omega Strike with both kicks */
	} ;

	pl_sp_tbl[ pl_w->pl_sp_type ]( pl_w ) ;
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

static void destroyer_init( PLAYER_WORK *pl_w );
static void destroyer_move( PLAYER_WORK *pl_w );
static void destroyer_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_destroyer( PLAYER_WORK *pl_w )
{
	static void ( *destroyer_jmp[])( PLAYER_WORK *pl_w ) = {
		destroyer_init, destroyer_move, destroyer_end,
	} ;

	destroyer_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void destroyer_init( PLAYER_WORK *pl_w )
{
	pl_w->hyper_bg_tm = 5 ;
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	sp_stand_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	/* from the Japanese disc on, the hyper starts with its own attack set, not the one before it */
#if BUILD_DATE >= 20000303
	pl_w->atck_no = pl30_at_destroyer ;
	HIT_RESET( pl_w ) ;
#endif
	pl_char_set( pl_w, Char_Super, 0 ) ;
}

/*--------------------------------------------------------------*/
static void destroyer_move( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->hyper_bg_tm = 5 ;
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->routine_2 ++ ;
		ofs.x = 0.0f ;
		ofs.y = DOT_Y( 80.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
		shl30_00_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void destroyer_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( ( pl_w->cg_atck_flag & CG_ATCK_ALL ) != CG_ATCK_ON ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
	} else {
		pl_w->hyper_bg_tm = 5 ;
		pl_w->delay_timer = 2 ;
		pl_w->hyper_bg_tm = 5 ;
	}
}

static void coil_init( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void coil_throw( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void coil_wait( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void coil_miss( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void coil_miss_end( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void coil_hold( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void coil_turn( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void coil_act( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw, Uint8 no );
static void coil_move( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void coil_end_set( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void coil_end( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void coil_pull( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );

/*==============================================================*/
/*			The coil on the ground: thrown out, then the		*/
/*			catch												*/
/*==============================================================*/
static void sp_coil( PLAYER_WORK *pl_w )
{
	static void ( *coil_jmp[])( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw ) = {
		coil_init, coil_throw, coil_wait, coil_miss, coil_miss_end, coil_hold,
		coil_move, coil_end_set, coil_end, coil_pull, coil_pull, coil_turn,
	} ;

	pl_w->atck_flag = 0 ;
	pl_w->hit_ng_timer = 3 ;
	pl_w->sp_ng_timer = 0 ;
	coil_jmp[pl_w->routine_2]( pl_w, ( PL30_LOCAL_W * )pl_w->tlocal ) ;
}

/* for each strength: the animation and the attack */
static Uint8 coil_no_tbl[3] = { 0, 2, 4 } ;
static Uint8 coil_atck_tbl[3] = { 0x33, 0x34, 0x35 } ;
static Uint8 coil_miss_tbl[3] = { 1, 3, 5 } ;
/* what the coil does, by the stick's way */
static Uint8 coil_lever_tbl[16] = { 4, 4, 4, 4, 6, 6, 12, 4, 0, 2, 10, 4, 4, 4, 4, 4 } ;
static Uint8 coil_act_tbl[7] = { 8, 10, 12, 14, 14, 16, 18 } ;
static Uint8 coil_end_tbl[7] = { 9, 11, 13, 15, 15, 17, 19 } ;

/*==============================================================*/
static void coil_init( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	sp_stand_set( pl_w ) ;
	pl_w->jump_dash_cnt = 0 ;
	lw->level = pl_w->atck_level ;
	pl_char_set( pl_w, Char_Special, coil_no_tbl[pl_w->atck_level] ) ;
	pl_w->atck_no = coil_atck_tbl[pl_w->atck_level] ;
	HIT_RESET( pl_w ) ;
	pl_w->pl_stop_now = 0 ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_sp_ef_set( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void coil_throw( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	pl_w->pl_stop_now |= pl_w->pl_sw_now & KEY_ATCK ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		if( !shl30_01_set( pl_w ) ){
			pl_w->routine_2 = 3 ;
			return ;
		}
		pl_w->routine_2 ++ ;
		pl_voice_req( pl_w, 32 ) ;
	}
}

/*--------------------------------------------------------------*/
static void coil_wait( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void coil_miss( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_w->dm_flag = -128 ;
	pl_w->catch_ng_timer = 0 ;
	pl_w->sp_ng_timer = 0 ;
	pl_char_set( pl_w, Char_Special, coil_miss_tbl[lw->level] ) ;
	coil_miss_end( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void coil_miss_end( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		scr_excep_clr() ;
		pl_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
/*			Caught: punch to pull them in, kick to throw		*/
/*			them												*/
/*==============================================================*/
static void coil_hold( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	if( pl_w->team_type == 8 ){
		coil_act( pl_w, lw, 0 ) ;
		return ;
	}
	if( pl_w->pl_sw_new & KEY_LEVER ){
		if( pl_w->pl_sw_now & KEY_PUNCH ) coil_act( pl_w, lw, 0 ) ;
		else if( pl_w->pl_sw_now & KEY_KICK ) coil_act( pl_w, lw, 1 ) ;
		else goto move ;
	} else if( pl_w->pl_sw_now & KEY_PUNCH ){
		pl_w->routine_2 = 9 ;
		pl_voice_req( pl_w, 23 ) ;
		coil_pull( pl_w, lw ) ;
	} else if( pl_w->pl_sw_now & KEY_KICK ){
		pl_w->routine_2 = 10 ;
		pl_voice_req( pl_w, 22 ) ;
		coil_pull( pl_w, lw ) ;
	} else goto move ;
	return ;
move:
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
/*			The stick and the way kept from the catch			*/
/*==============================================================*/
static void coil_turn( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	pl_w->pl_sw_new = lw->lever ;
	pl_w->pl_dir ^= lw->turn ;
	pl_w->disp_dir = pl_w->pl_dir ;
	coil_act( pl_w, lw, lw->type ) ;
}

/*==============================================================*/
static void coil_act( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw, Uint8 no )
{
	Uint8 act ;

	scr_excep_set( pl_w, 5, 5 ) ;
	act = coil_lever_tbl[( ( pl_w->pl_sw_new & KEY_LEVER ) >> 10 ) & 15] ;
	if( pl_w->team_type == 8 ) act = 10 ;
	if( no && act >= 10 ) no = 0 ;
	pl_w->type = no ;
	lw->level = act ;
	pl_char_set( pl_w, Char_Special, coil_act_tbl[act >> 1] ) ;
	pl_w->routine_2 = 6 ;
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void coil_move( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void coil_end_set( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	pl_w->dm_flag = -128 ;
	pl_w->catch_ng_timer = 0 ;
	pl_w->sp_ng_timer = 0 ;
	pl_char_set( pl_w, Char_Special, coil_end_tbl[lw->level / 2] ) ;
	pl_w->routine_2 ++ ;
	coil_end( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void coil_end( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		scr_excep_clr() ;
		if( lw->level < 10 ){
			pl_w->pl_dir ^= 1 ;
			pl_w->disp_dir = pl_w->pl_dir ;
		}
		pl_stand_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void coil_pull( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	if( pl_w->pl_sw_new & KEY_LEVER ){
		if( pl_w->pl_sw_now & KEY_PUNCH ) coil_act( pl_w, lw, 0 ) ;
		else if( pl_w->pl_sw_now & KEY_KICK ) coil_act( pl_w, lw, 1 ) ;
	}
	pl_char_move( pl_w ) ;
}

static void jcoil_init( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void jcoil_air_move( PLAYER_WORK *pl_w );
static void jcoil_throw( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void jcoil_wait( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void jcoil_miss( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void jcoil_miss_end( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void jcoil_hold( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void jcoil_turn( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void jcoil_act( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw, Uint8 no );
static void jcoil_move( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void jcoil_end_set( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void jcoil_end( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void jcoil_pull( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );

/*==============================================================*/
/*			The coil in the air									*/
/*==============================================================*/
static void sp_jcoil( PLAYER_WORK *pl_w )
{
	static void ( *jcoil_jmp[])( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw ) = {
		jcoil_init, jcoil_throw, jcoil_wait, jcoil_miss, jcoil_miss_end, jcoil_hold,
		jcoil_move, jcoil_end_set, jcoil_end, jcoil_pull, jcoil_pull, jcoil_turn,
	} ;

	pl_w->atck_flag = 0 ;
	pl_w->hit_ng_timer = 3 ;
	jcoil_jmp[pl_w->routine_2]( pl_w, ( PL30_LOCAL_W * )pl_w->tlocal ) ;
}

static Uint8 jcoil_no_tbl[5] = { 0x14, 0x16, 0x18, 0x1a, 0x1c } ;
static Uint8 jcoil_atck_tbl[5] = { 0x33, 0x34, 0x35, 0x36, 0x37 } ;
static Uint8 jcoil_miss_tbl[5] = { 0x15, 0x17, 0x19, 0x1b, 0x1d } ;
static Uint8 jcoil_lever_tbl[16] = { 4, 4, 4, 4, 8, 6, 12, 4, 0, 2, 10, 4, 4, 4, 4, 4 } ;
static Uint8 jcoil_act_tbl[7] = { 0x1e, 0x20, 0x22, 0x24, 0x36, 0x26, 0x28 } ;
static Uint8 jcoil_end_tbl[7] = { 0x1f, 0x21, 0x23, 0x25, 0x37, 0x27, 0x29 } ;

/*==============================================================*/
static void jcoil_init( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_w->jump_com_cnt ++ ;
	pl_w->jump_dash_cnt = 0 ;
	pl_dir_set( pl_w ) ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->spd.x = pl_w->spd.x / 8.0f ;
	pl_w->spd.y = pl_w->spd.y / 8.0f ;
	pl_w->acc.y = pl_w->acc.y / 16.0f ;
	lw->level = pl_w->atck_level ;
	pl_char_set( pl_w, Char_Special, jcoil_no_tbl[pl_w->atck_level] ) ;
	pl_w->atck_no = jcoil_atck_tbl[pl_w->atck_level] ;
	HIT_RESET( pl_w ) ;
	pl_w->pl_stop_now = 0 ;
	pl_gauge_add( pl_w, 5 ) ;
}

/*==============================================================*/
/*			Fall slowly, and stop on the ground					*/
/*==============================================================*/
static void jcoil_air_move( PLAYER_WORK *pl_w )
{
	pl_brake_stop( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
}

/*--------------------------------------------------------------*/
static void jcoil_throw( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	pl_w->pl_stop_now |= pl_w->pl_sw_now & KEY_ATCK ;
	jcoil_air_move( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		if( !shl30_01_set( pl_w ) ){
			pl_w->routine_2 = 3 ;
			return ;
		}
		pl_w->routine_2 ++ ;
		pl_voice_req( pl_w, 32 ) ;
	}
}

/*--------------------------------------------------------------*/
static void jcoil_wait( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	jcoil_air_move( pl_w ) ;
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void jcoil_miss( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_w->dm_flag = -128 ;
	pl_w->catch_ng_timer = 0 ;
	pl_char_set( pl_w, Char_Special, jcoil_miss_tbl[lw->level] ) ;
	jcoil_miss_end( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void jcoil_miss_end( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	jcoil_air_move( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		scr_excep_clr() ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_jump_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void jcoil_hold( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	if( pl_w->pl_sw_new & KEY_LEVER ){
		if( pl_w->pl_sw_now & KEY_PUNCH ) jcoil_act( pl_w, lw, 0 ) ;
		else if( pl_w->pl_sw_now & KEY_KICK ) jcoil_act( pl_w, lw, 1 ) ;
		else goto move ;
	} else if( pl_w->pl_sw_now & KEY_PUNCH ){
		pl_w->routine_2 = 9 ;
		pl_voice_req( pl_w, 23 ) ;
		jcoil_pull( pl_w, lw ) ;
	} else if( pl_w->pl_sw_now & KEY_KICK ){
		pl_w->routine_2 = 10 ;
		pl_voice_req( pl_w, 22 ) ;
		jcoil_pull( pl_w, lw ) ;
	} else goto move ;
	return ;
move:
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void jcoil_turn( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	pl_w->pl_sw_new = lw->lever ;
	pl_w->pl_dir ^= lw->turn ;
	pl_w->disp_dir = pl_w->pl_dir ;
	jcoil_act( pl_w, lw, lw->type ) ;
}

/*==============================================================*/
static void jcoil_act( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw, Uint8 no )
{
	Uint8 act ;

	scr_excep_set( pl_w, 5, 5 ) ;
	act = jcoil_lever_tbl[( ( pl_w->pl_sw_new & KEY_LEVER ) >> 10 ) & 15] ;
	if( no && act >= 10 ) no = 0 ;
	pl_w->type = no ;
	lw->level = act ;
	pl_char_set( pl_w, Char_Special, jcoil_act_tbl[act >> 1] ) ;
	pl_w->routine_2 = 6 ;
	pl_char_move( pl_w ) ;
	jcoil_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void jcoil_move( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void jcoil_end_set( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	pl_w->dm_flag = -128 ;
	pl_w->catch_ng_timer = 0 ;
	pl_char_set( pl_w, Char_Special, jcoil_end_tbl[lw->level / 2] ) ;
	pl_w->routine_2 ++ ;
	jcoil_end( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void jcoil_end( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		scr_excep_clr() ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		if( lw->level < 10 ){
			pl_w->pl_dir ^= 1 ;
			pl_w->disp_dir = pl_w->pl_dir ;
		}
		pl_jump_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void jcoil_pull( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	if( pl_w->pl_sw_new & KEY_LEVER ){
		if( pl_w->pl_sw_now & KEY_PUNCH ) jcoil_act( pl_w, lw, 0 ) ;
		else if( pl_w->pl_sw_now & KEY_KICK ) jcoil_act( pl_w, lw, 1 ) ;
	}
	pl_char_move( pl_w ) ;
}

static void strike_init( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void strike_throw( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void strike_hold( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void strike_ret_set( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void strike_ret( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void strike_pull_set( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void strike_pull( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void strike_back( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );

/*==============================================================*/
/*			The coil thrown to hook on and swing				*/
/*==============================================================*/
static void sp_strike( PLAYER_WORK *pl_w )
{
	static void ( *strike_jmp[])( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw ) = {
		strike_init, strike_throw, strike_hold, strike_ret_set,
		strike_ret, strike_pull_set, strike_pull, strike_back,
	} ;

	strike_jmp[pl_w->routine_2]( pl_w, ( PL30_LOCAL_W * )pl_w->tlocal ) ;
}

/* for each strength: the animation, the stick's way and the attack */
static Uint8 strike_tbl[3][3] = {
	{ 0x2c, 0xc0, 0x39 },
	{ 0x2b, 0xe0, 0x3a },
	{ 0x2a, 0x00, 0x3b },
} ;
static Uint8 strike_ret_tbl[3] = { 0x2f, 0x2e, 0x2d } ;
static Uint8 strike_pull_tbl[3] = { 0x32, 0x31, 0x30 } ;
static Uint8 strike_back_tbl[3] = { 0x35, 0x34, 0x33 } ;

/*==============================================================*/
static void strike_init( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	Uint8 atck ;

	pl_w->routine_2 ++ ;
	sp_stand_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_gauge_add( pl_w, 16 ) ;
	pl_w->sjump_flag = 0 ;
	pl_w->jump_com_cnt = 0 ;
	pl_w->jump_dash_cnt = 0 ;
	lw->pull_flag = 0 ;
	lw->ret_pos = pl_w->pos ;
	lw->level = pl_w->atck_level ;
	pl_char_set( pl_w, Char_Special, strike_tbl[lw->level][0] ) ;
	pl_w->dir = strike_tbl[lw->level][1] ;
	atck = strike_tbl[lw->level][2] ;
	if( pl_w->team_type == 3 ) atck = 70 ;
	pl_w->atck_no = atck ;
	HIT_RESET( pl_w ) ;
	strike_throw( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void strike_throw( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->push_ng_timer = 2 ;
		if( lw->level ) pl_air_set( pl_w ) ;
		shl30_03_set( pl_w ) ;
	}
}

/*==============================================================*/
/*			Let go with the stick's way, or swing in with a		*/
/*			kick												*/
/*==============================================================*/
static void strike_hold( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	pl_w->push_ng_timer = 2 ;
	if( pl_w->wall_flag ){
		if( ( pl_w->wall_flag & WALL_RIGHT ) == pl_w->pl_dir ) goto ret ;
	}
	if( pl_char_move( pl_w ) < 0 ) goto ret ;
	if( !pl_w->team_no && ( pl_w->pl_sw_now & KEY_KICK ) ){
		if( pl_w->pl_sw_new & KEY_DOWN ) goto ret ;
		if( pl_w->pl_sw_new & KEY_BACK ){
			pl_w->routine_2 = 5 ;
			return ;
		}
	}
	return ;
ret:
	pl_w->routine_2 ++ ;
	strike_ret_set( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void strike_ret_set( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_char_set( pl_w, Char_Special, strike_ret_tbl[lw->level] ) ;
	if( lw->level ){
		pl_w->spd.y = 0.13392857f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
	}
	strike_ret( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void strike_ret( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ){
		if( lw->level != 0 ){
			pl_w->jump_atk_cnt = JATK_ONE ;
			pl_sjump_ret( pl_w ) ;
		} else pl_stand_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void strike_pull_set( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_char_set( pl_w, Char_Special, strike_pull_tbl[lw->level] ) ;
	strike_pull( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void strike_pull( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	pl_w->push_ng_timer = 2 ;
	if( !lw->pull_flag ){
		pl_char_move( pl_w ) ;
		return ;
	}
	pl_w->routine_2 ++ ;
	pl_char_set( pl_w, Char_Special, strike_back_tbl[lw->level] ) ;
	pl_w->pos = lw->ret_pos ;
	strike_back( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void strike_back( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_stand_ret( pl_w ) ;
	}
}

static void jdestroyer_init( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void jdestroyer_throw( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void jdestroyer_wait( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void jdestroyer_miss( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void jdestroyer_miss_end( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void jdestroyer_hit( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void jdestroyer_hit_move( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void jdestroyer_end_set( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void jdestroyer_end( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void jdestroyer_dash_init( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void jdestroyer_dash_start( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void jdestroyer_dash( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void jdestroyer_dash_end( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void jdestroyer_dash_hit( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );
static void jdestroyer_none( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw );

/*==============================================================*/
/*			A hyper combo: the coils, after a dash				*/
/*==============================================================*/
static void sp_jdestroyer( PLAYER_WORK *pl_w )
{
	static void ( *jdestroyer_jmp[])( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw ) = {
		jdestroyer_init, jdestroyer_throw, jdestroyer_wait, jdestroyer_miss, jdestroyer_miss_end,
		jdestroyer_hit, jdestroyer_hit_move, jdestroyer_end_set, jdestroyer_end, jdestroyer_dash_init,
		jdestroyer_dash_start, jdestroyer_dash, jdestroyer_dash_hit, jdestroyer_dash_end, jdestroyer_none,
	} ;

	jdestroyer_jmp[pl_w->routine_2]( pl_w, ( PL30_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void jdestroyer_init( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->routine_2 ++ ;
	lw->level = 2 ;
	pl_w->atck_level = 2 ;
	pl_char_set( pl_w, Char_Special, 24 ) ;
}

/*--------------------------------------------------------------*/
static void jdestroyer_throw( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		if( !shl30_01_set( pl_w ) ){
			pl_w->routine_2 = 3 ;
			return ;
		}
		pl_voice_req( pl_w, 32 ) ;
	}
}

/*--------------------------------------------------------------*/
static void jdestroyer_wait( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void jdestroyer_miss( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	pl_w->delay_flag = 0 ;
	pl_w->delay_timer = 0 ;
	pl_w->hyper_bg = 0 ;
	pl_w->hyper_bg_tm = 0 ;
	pl_w->routine_2 ++ ;
	pl_w->dm_flag = -128 ;
	pl_w->catch_ng_timer = 0 ;
	pl_char_set( pl_w, Char_Special, 25 ) ;
	jdestroyer_miss_end( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void jdestroyer_miss_end( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		scr_excep_clr() ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_jump_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void jdestroyer_hit( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->atck_flag = 0 ;
	pl_w->hit_ng_timer = 3 ;
	scr_excep_set( pl_w, 1, 1 ) ;
	pl_w->type = 2 ;
	lw->level = 8 ;
	pl_char_set( pl_w, Char_Super, 4 ) ;
	pl_w->routine_2 = 6 ;
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void jdestroyer_hit_move( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->atck_flag = 0 ;
	pl_w->hit_ng_timer = 3 ;
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void jdestroyer_end_set( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	pl_w->delay_flag = 0 ;
	pl_w->delay_timer = 0 ;
	pl_w->hyper_bg = 0 ;
	pl_w->hyper_bg_tm = 0 ;
	pl_w->atck_flag = 0 ;
	pl_w->hit_ng_timer = 3 ;
	pl_w->dm_flag = -128 ;
	pl_w->catch_ng_timer = 0 ;
	pl_char_set( pl_w, Char_Super, 5 ) ;
	pl_w->routine_2 ++ ;
	jdestroyer_end( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void jdestroyer_end( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	pl_w->delay_flag = 0 ;
	pl_w->delay_timer = 0 ;
	pl_w->hyper_bg = 0 ;
	pl_w->hyper_bg_tm = 0 ;
	pl_w->atck_flag = 0 ;
	pl_w->hit_ng_timer = 3 ;
	if( pl_char_move( pl_w ) < 0 ){
		scr_excep_clr() ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_jump_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void jdestroyer_dash_init( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl30_at_jdestroyer ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 2 ) ;
}

/*--------------------------------------------------------------*/
static void jdestroyer_dash_start( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type & 1 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = -20.0f ;
		if( pl_w->disp_dir ) pl_w->spd.x = -pl_w->spd.x ;
		pl_w->timer0 = 16 ;
		ofs.x = 0.0f ;
		ofs.y = DOT_Y( 80.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*==============================================================*/
/*			Dash until the opponent is hit, then grab them		*/
/*==============================================================*/
static void jdestroyer_dash( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	PLAYER_WORK *em ;
	Float x ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( !pl_w->hit_flag ){
		if( --pl_w->timer0 < 0 ){
			pl_w->routine_2 = 13 ;
			pl_char_set( pl_w, Char_Super, 3 ) ;
		} else {
			pl_char_move( pl_w ) ;
			pl_w->pos.x += pl_w->spd.x ;
			pl_w->spd.x += pl_w->acc.x ;
			pl_w->pos.y += pl_w->spd.y ;
			pl_w->spd.y += pl_w->acc.y ;
		}
	} else if( !pl_sp_catch_ck( pl_w ) ){
		pl_w->routine_2 = 13 ;
		pl_w->spd.y = DOT_Y( 6.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_w->spd.x = 1.6666666f ;
		if( pl_w->disp_dir ) pl_w->spd.x = -pl_w->spd.x ;
		pl_char_set( pl_w, Char_Super, 3 ) ;
	} else {
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Super, 1 ) ;
		em = pl_w->em_w ;
		em->pos = pl_w->pos ;
		em->pos.y += DOT_Y( 8.0f ) ;
		x = -DOT_X( 64.0f ) ;
		if( pl_w->disp_dir ) x = DOT_X( 64.0f ) ;
		em->pos.x += x ;
		pl_w->spd = em->pos ;
	}
}

/*--------------------------------------------------------------*/
static void jdestroyer_dash_end( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		RATE_CLR_X( pl_w ) ;
		if( !pl_w->hit_flag ) pl_w->spd.y = 0.0f ;
		pl_jump_ret( pl_w ) ;
	} else if( pl_w->hit_flag ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
}

/*--------------------------------------------------------------*/
static void jdestroyer_dash_hit( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 = 0 ;
		jdestroyer_init( pl_w, lw ) ;
	} else if( pl_w->cg_flag1 ){
		pl_w->atck_no = pl_w->cg_flag1 ;
		HIT_RESET( pl_w ) ;
		pl_w->cg_flag1 = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void jdestroyer_none( PLAYER_WORK *pl_w, PL30_LOCAL_W *lw )
{
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
	sp_stand_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl30_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 63 ) ;
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
		ofs.x = 95.0f ;
		ofs.y = 60.0f ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
}

/*==============================================================*/
static void sp_burrow( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->atck_no = pl30_at_burrow ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 61 ) ;
		pl_w->timer0 = -1 ;
		shl30_02_set( pl_w, 1, 1 ) ;
	}
	if( !pl_w->routine_3 ){
		pl_char_move( pl_w ) ;
		if( !pl_w->timer0 ){
			pl_w->routine_3 ++ ;
			pl_char_set( pl_w, Char_Special, 62 ) ;
		} else pl_w->timer0 -- ;
	} else if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_counter_a( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, 65 ) ;
		pl_w->atck_no = pl30_at_counter_a ;
		HIT_RESET( pl_w ) ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_counter_g( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, 67 ) ;
		pl_w->atck_no = pl30_at_counter_g ;
		HIT_RESET( pl_w ) ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, 56 ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
	} else if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_move( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A kick that leaps off the wall						*/
/****************************************************************/
void	pl30_t_kick( PLAYER_WORK *pl_w )
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
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->spd.x = DOT_X( 20.0f ) ;
	if( !pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = -0.73660713f ;
	pl_w->atck_no = pl30_at_t_kick ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 2 ) ;
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
		pl_char_set( pl_w, Char_Extra, 3 ) ;
		pl_land_ef_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void t_kick_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stop_stand_ret( pl_w ) ;
	} else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
	}
}

static void air_dash_init( PLAYER_WORK *pl_w );
static void air_dash_start( PLAYER_WORK *pl_w );
static void air_dash_move( PLAYER_WORK *pl_w );
static void air_dash_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A dash in the air, cut short by the stick			*/
/****************************************************************/
void	pl30_air_dash( PLAYER_WORK *pl_w )
{
	static void ( *air_dash_jmp[])( PLAYER_WORK *pl_w ) = {
		air_dash_init, air_dash_start, air_dash_move, air_dash_end,
	} ;

	if( pl_w->pl_sw_now & KEY_BACK ){
		RATE_CLR_X( pl_w ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_jump_ret( pl_w ) ;
		return ;
	}
	air_dash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void air_dash_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.y = DOT_Y( 2.0f ) ;
	pl_w->acc.y = -0.4017857f ;
	pl_w->spd.x = DOT_X( 8.0f ) ;
	pl_w->acc.x = 0.0f ;
	if( !pl_w->disp_dir ) pl_w->spd.x = -pl_w->spd.x ;
	pl_char_set( pl_w, Char_Dash, 4 ) ;
	air_dash_start( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void air_dash_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ) pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void air_dash_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
	}
}

/*--------------------------------------------------------------*/
static void air_dash_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/****************************************************************/
/*			A throw: not crouching, the stick held to a			*/
/*			side, and not a kick in the air						*/
/****************************************************************/
PLAYER_WORK *pl30_catch_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_w->pl_stance == STANCE_CROUCH ) return 0 ;
	if( !( pl_w->atck_lever & ( KEY_FORWARD | KEY_BACK ) ) ) return 0 ;
	if( !pl_w->atck_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && pl_w->atck_pk == 1 ) return 0 ;
	if( !( em = pl_catch_ck( pl_w ) ) ) return em ;
	pl_w->dir = ( pl_w->atck_lever & KEY_LEVER ) >> 10 ;
	if( pl_w->pl_stance == STANCE_AIR ) pl_w->catch_no = 3 ;
	else pl_w->catch_no = !pl_w->atck_pk ? 2 : 1 ;
	return em ;
}

static void nml_k_set( PLAYER_WORK *pl_w );
static void nml_p_set( PLAYER_WORK *pl_w );
static void jump_set( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Start the throw that pl30_catch_ck chose			*/
/****************************************************************/
void	pl30_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_k_set, nml_k_set, nml_p_set, jump_set,
	} ;
	PLAYER_WORK *em ;

	catch_gauge_add( pl_w ) ;
	em = pl_w->catch_w ;
	em->disp_dir = em->pl_dir = pl_w->disp_dir ^ 1 ;
	pl_w->hit_stop = 10 ;
	catch_set_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_k_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	scr_excep_set( pl_w, 5, 5 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	ofs.x = -DOT_X( 128.0f ) ;
	ofs.y = DOT_Y( 64.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_char_set( pl_w, Char_Catch, 1 ) ;
}

/*==============================================================*/
static void nml_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	scr_excep_set( pl_w, 5, 5 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	ofs.x = -DOT_X( 128.0f ) ;
	ofs.y = DOT_Y( 64.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_char_set( pl_w, Char_Catch, 2 ) ;
}

/*==============================================================*/
static void jump_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	scr_excep_set( pl_w, 5, 5 ) ;
	pl_w->spd.y = DOT_Y( 4.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	if( pl_w->sjump_flag ){
		pl_w->spd.x /= 2.0f ;
		pl_w->acc.x /= 2.0f ;
	}
	ofs.x = -DOT_X( 64.0f ) ;
	ofs.y = DOT_Y( 80.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_char_set( pl_w, Char_Catch, 3 ) ;
}

static void catch_ret( PLAYER_WORK *pl_w );
static void nml_k_catch( PLAYER_WORK *pl_w );
static void nml_p_catch( PLAYER_WORK *pl_w );
static void jump_catch( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void	pl30_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_k_catch, nml_k_catch, nml_p_catch, jump_catch,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void catch_ret( PLAYER_WORK *pl_w )
{
	if( pl_w->pl_stance == STANCE_AIR ) pl_jump_ret( pl_w ) ;
	else pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			Swung over, and thrown the way the stick held		*/
/*==============================================================*/
static void nml_k_catch( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		catch_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->disp_dir ^= 1 ;
		pl_w->pl_dir = pl_w->disp_dir ;
		if( pl_w->dir & 1 ){
			pl_w->disp_dir ^= 1 ;
			pl_w->pl_dir = pl_w->disp_dir ;
			pl_w->catch_w->pl_dir ^= 1 ;
		}
	}
	if( pl_w->cg_flag1 ){
		pl_w->catch_w->catch_type = 1 ;
		pl_w->catch_w->atck_no = pl_w->cg_flag1 ;
		pl_w->catch_w->dm_hit_w = pl_w ;
		pl_w->cg_flag1 = 0 ;
		scr_excep_clr() ;
	}
}

/*==============================================================*/
static void nml_p_catch( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		catch_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		if( pl_w->dir & 1 ){
			pl_w->disp_dir ^= 1 ;
			pl_w->pl_dir = pl_w->disp_dir ;
		}
	}
	if( pl_w->cg_flag1 ){
		pl_w->catch_w->catch_type = 11 ;
		pl_w->catch_w->atck_no = pl_w->cg_flag1 ;
		pl_w->catch_w->dm_hit_w = pl_w ;
		pl_w->cg_flag1 = 0 ;
		scr_excep_clr() ;
		catch_se_req( pl_w->catch_w ) ;
	}
}

/*==============================================================*/
static void jump_catch( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_3 ){
		if( pl_w->cg_type > 0 ){
			pl_w->cg_type = 0 ;
			pl_w->disp_dir ^= 1 ;
			pl_w->pl_dir = pl_w->disp_dir ;
		}
		if( pl_w->cg_type >= 0 ) pl_char_move( pl_w ) ;
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->pos.y <= pl_w->ground_y ){
			pl_w->pos.y = pl_w->ground_y ;
			pl_w->routine_3 ++ ;
			pl_w->pl_stance = STANCE_STAND ;
		}
	} else {
		if( pl_char_move( pl_w ) < 0 ){
			catch_ret( pl_w ) ;
			return ;
		}
		if( pl_w->cg_flag1 ){
			pl_w->catch_w->catch_type = 11 ;
			pl_w->catch_w->atck_no = pl_w->cg_flag1 ;
			pl_w->catch_w->dm_hit_w = pl_w ;
			pl_w->cg_flag1 = 0 ;
			scr_excep_clr() ;
			catch_se_req( pl_w->catch_w ) ;
		}
	}
}

/****************************************************************/
/*			Holding the opponent, but not in the throws			*/
/*			marked 0xc0											*/
/****************************************************************/
void	pl30_hold( PLAYER_WORK *pl_w )
{
	if( pl_w->catch_no == 0xc0 || pl_w->catch_no == 0xc0 ) return ;
	catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void	pl30_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl30_sp_destroyer ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl30_sp_destroyer ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl30_sp_destroyer ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl30_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl30_sp_destroyer ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl30_sp_destroyer ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl30_sp_destroyer ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl30_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl30_sp_coil ;
		pl_w->atck_level = 2 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl30_sp_strike ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl30_sp_burrow ;
		pl_w->atck_level = 0 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void	pl30_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl30_sp_counter_a ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl30_sp_strike ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl30_sp_counter_g ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl30.c" End of File	*/

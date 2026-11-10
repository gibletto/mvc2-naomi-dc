/* War Machine (character 2e): command checks, normal attacks, flight, appearance, win poses,
   special moves and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl2e_def.h"

void pl2e_main( PLAYER_WORK *pl_w );
void pl2e_com_ck( PLAYER_WORK *pl_w );
void pl2e_atck_set( PLAYER_WORK *pl_w );
void pl2e_atck( PLAYER_WORK *pl_w );
void pl2e_dash( PLAYER_WORK *pl_w );
void pl2e_noki( PLAYER_WORK *pl_w );
void pl2e_appear( PLAYER_WORK *pl_w );
void pl2e_win( PLAYER_WORK *pl_w );
void pl2e_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl2e_catch_ck( PLAYER_WORK *pl_w );
void pl2e_catch_set( PLAYER_WORK *pl_w );
void pl2e_catch( PLAYER_WORK *pl_w );
void pl2e_hold( PLAYER_WORK *pl_w );
int pl2e_delay_ck( PLAYER_WORK *pl_w );
void pl2e_t_kick( PLAYER_WORK *pl_w );
void pl2e_upper( PLAYER_WORK *pl_w );
void pl2e_sp_unison( PLAYER_WORK *pl_w );
void pl2e_sp_gattai( PLAYER_WORK *pl_w );
void pl2e_sp_assist( PLAYER_WORK *pl_w );
void pl2e_sp_zero( PLAYER_WORK *pl_w );

/* each attack's table: standing and crouching, jumping, super jumping */
static Sint8 lp_tbl[4] = {  1,  2,  0,  2 } ;
static Sint8 mp_tbl[4] = { -1,  2,  1,  2 } ;
static Sint8 hp_tbl[4] = { -1, -1, -1,  2 } ;
static Sint8 lk_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 mk_tbl[4] = { -1,  2, -1,  2 } ;
static Sint8 hk_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 j_lp_tbl[4] = {  1,  2,  0,  2 } ;
static Sint8 j_mp_tbl[4] = { -1,  2,  1,  2 } ;
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
static Uint16 com_cannon[] = { 3, 0, pl2e_sp_cannon, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Shoulder Cannon */
static Uint16 com_ccannon[] = { 3, COM_GROUND, pl2e_sp_ccannon, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Shoulder Cannon, crouching */
static Uint16 com_repulsor[] = { 5, COM_GROUND, pl2e_sp_repulsor, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Repulsor Blast */
static Uint16 com_fly[] = { 3, COM_ALL_SW, pl2e_sp_fly, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Flight */
static Uint16 com_proton[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl2e_sp_proton, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Proton Cannon */
static Uint16 com_destroyer[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl2e_sp_destroyer, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* War Destroyer */

static void ( *pl2e_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl2e_main,
	pl2e_com_ck,
	pl2e_atck_set,
	pl2e_atck,
	pl2e_dash,
	pl2e_noki,
	pl2e_appear,
	pl2e_win,
	pl2e_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl2e_catch_ck,	/* returns a value */
	pl2e_catch_set,
	pl2e_catch,
	pl2e_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl2e_delay_ck,
	pl2e_t_kick,
	pl2e_upper,
	pl2e_sp_unison,
	pl2e_sp_gattai,
	pl2e_sp_assist,
	pl2e_sp_zero,
} ;

static Uint8 cannon_ck( PLAYER_WORK *pl_w );
static Uint8 ccannon_ck( PLAYER_WORK *pl_w );
static Uint8 repulsor_ck( PLAYER_WORK *pl_w );
static Uint8 bomb_ck( PLAYER_WORK *pl_w );
static Uint8 fly_ck( PLAYER_WORK *pl_w );
static Uint8 proton_ck( PLAYER_WORK *pl_w );
static Uint8 destroyer_ck( PLAYER_WORK *pl_w );
static int chouhatsu_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static int proton_delay_ck( PLAYER_WORK *pl_w );
static int destroyer_delay_ck( PLAYER_WORK *pl_w );
static void fly_end( PLAYER_WORK *pl_w );

/****************************************************************/
void pl2e_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl2e_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl2e_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( proton_ck( pl_w ) ) return ;
	if( destroyer_ck( pl_w ) ) return ;
	if( fly_ck( pl_w ) ) return ;
	if( cannon_ck( pl_w ) ) return ;
	if( ccannon_ck( pl_w ) ) return ;
	if( repulsor_ck( pl_w ) ) return ;
	if( bomb_ck( pl_w ) ) return ;
	if( pl_air_dash_sw_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
/*			Once a jump unless super jumping					*/
/*==============================================================*/
static Uint8 cannon_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_cannon, &pl_w->com_w0 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl2e_sp_cannon ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 ccannon_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_ccannon, &pl_w->com_w1 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl2e_sp_ccannon ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 repulsor_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_repulsor, &pl_w->com_w2 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl2e_sp_repulsor ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			Its strength comes from the stick: forward, back	*/
/*			or neither											*/
/*==============================================================*/
static Uint8 bomb_ck( PLAYER_WORK *pl_w )
{
	PL2E_LOCAL_W *lw = ( PL2E_LOCAL_W * )pl_w->tlocal ;
	Uint16 sw ;

	if( lw->bomb_cnt ) return 0 ;
	if( !sw_now_ck( pl_w, 0x0140, &sw, 3 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	if( !sp_ok_ck( pl_w ) ) return 0 ;
	if( pl_w->pl_sw_new & KEY_FORWARD ) pl_w->atck_level = 2 ;
	else if( pl_w->pl_sw_new & KEY_BACK ) pl_w->atck_level = 0 ;
	else pl_w->atck_level = 1 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl2e_sp_bomb ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 fly_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_fly, &pl_w->com_w3 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w3 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl2e_sp_fly ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 proton_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_proton, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl2e_sp_proton ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 destroyer_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_destroyer, &pl_w->com_w5 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl2e_sp_destroyer ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static int chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 4 ) ) return 0 ;
	pl_w->pl_sp_type = pl2e_sp_chouhatsu ;
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
	pl_w->pl_sp_type = pl2e_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/****************************************************************/
/*			A hyper combo entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl2e_delay_ck( PLAYER_WORK *pl_w )
{
	if( proton_delay_ck( pl_w ) ) return 1 ;
	if( destroyer_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static int proton_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_proton, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl2e_sp_proton ;
	return 1 ;
}

/*==============================================================*/
static int destroyer_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_destroyer, &pl_w->com_w5 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl2e_sp_destroyer ;
	return 1 ;
}

/****************************************************************/
/*			Counts down the flight								*/
/****************************************************************/
void pl2e_main( PLAYER_WORK *pl_w )
{
	PL2E_LOCAL_W *lw = ( PL2E_LOCAL_W * )pl_w->tlocal ;

	if( pl_w->fly_flag && pl_w->routine_1 == PL_NORM && !-- lw->fly_timer ){
		if( pl_w->act_st == N_SPECIAL ) lw->fly_timer = 1 ;
		else fly_end( pl_w ) ;
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
static void fly_atck_set( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Normal attacks										*/
/****************************************************************/
void pl2e_atck_set( PLAYER_WORK *pl_w )
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
/*			The heavy punch is held for 40 frames				*/
/*==============================================================*/
static void stand_punch( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_STAND_LP ;
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_STAND_MP ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_STAND_HP ;
		pl_w->chain_tbl = hp_tbl ;
		pl_w->nml_level = 2 ;
		pl_w->routine_2 = 0 ;
		pl_w->timer0 = 40 ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Stand_P, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
/*			The heavy punch is a beam							*/
/*==============================================================*/
static void crouch_punch( PLAYER_WORK *pl_w )
{
	PL2E_LOCAL_W *lw = ( PL2E_LOCAL_W * )pl_w->tlocal ;

	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_CROUCH_LP ;
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Crouch_P, pl_w->char_no.b.no ) ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_CROUCH_MP ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Crouch_P, pl_w->char_no.b.no ) ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_CROUCH_HP ;
		pl_w->chain_tbl = hp_tbl ;
		pl_w->nml_level = 2 ;
		pl_w->atck_no = pl2e_at_crouch_hp ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Crouch_P, pl_w->char_no.b.no ) ;
		pl_w->routine_3 = 0 ;
		lw->beam_end_flag = 0 ;
		pl_w->timer1 = pl_w->cg_type ;
		pl_w->cg_type = 0 ;
		break ;
	}
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
		pl_se_req( pl_w, SE_SWING_H ) ;
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
		pl_w->chain_tbl = mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_CROUCH_HK ;
		pl_w->chain_tbl = hk_tbl ;
		pl_w->nml_level = 2 ;
		pl_se_req( pl_w, SE_SWING_H ) ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Crouch_K, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
/*			In the air, only while that button's jump			*/
/*			attacks are left, but freely while flying			*/
/*==============================================================*/
static void jump_atck_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->fly_flag
	 || ( pl_w->atck_pk == 0 && ( pl_w->jump_atk_cnt & JATK_P_MASK ) )
	 || ( pl_w->atck_pk != 0 && ( pl_w->jump_atk_cnt & JATK_K_MASK ) ) ) jump_atck( pl_w ) ;
}

/*==============================================================*/
static void jump_atck( PLAYER_WORK *pl_w )
{
	fly_atck_set( pl_w ) ;
	if( pl_w->atck_pk == 1 ) jump_kick( pl_w ) ;
	else jump_punch( pl_w ) ;
}

/*==============================================================*/
/*			The heavy punch aims up or down with the stick		*/
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
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mp_tbl ;
		else pl_w->chain_tbl = sj_mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_hp_tbl ;
		else pl_w->chain_tbl = sj_hp_tbl ;
		pl_w->nml_level = 2 ;
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_JUMP_HP ;
		if( pl_w->atck_lever & KEY_UP ){
			pl_w->char_no.b.no = 6 ;
			pl_w->atck_no = pl2e_at_jup_hp ;
		}
		if( pl_w->atck_lever & KEY_DOWN ){
			pl_w->char_no.b.no = 7 ;
			pl_w->atck_no = pl2e_at_jdown_hp ;
		}
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Jump_P, pl_w->char_no.b.no ) ;
	if( pl_w->jump_atk_cnt & JATK_P_MASK ) pl_w->jump_atk_cnt -= 0x01 ;
}

/*==============================================================*/
/*			The heavy kick with the stick down, high enough,	*/
/*			is a dive											*/
/*==============================================================*/
static void jump_kick( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		pl_char_set( pl_w, Char_Jump_K, 0 ) ;
		pl_w->atck_no = ATCK_JUMP_LK ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_lk_tbl ;
		else pl_w->chain_tbl = sj_lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_char_set( pl_w, Char_Jump_K, 1 ) ;
		pl_w->atck_no = ATCK_JUMP_MK ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mk_tbl ;
		else pl_w->chain_tbl = sj_mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_hk_tbl ;
		else pl_w->chain_tbl = sj_hk_tbl ;
		pl_w->nml_level = 2 ;
		if( ( pl_w->atck_lever & KEY_DOWN ) && pl_w->pos.y > pl_w->ground_y + DOT_Y( 64.0f ) ){
			pl_w->fly_flag = 0 ;
			pl_w->jump_atk_cnt &= JATK_P_MASK ;
			pl_w->sjump_flag = 0 ;
			pl_char_set( pl_w, Char_Extra, 4 ) ;
			pl_w->atck_no = pl2e_at_jdown_hk ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_w->routine_2 = 1 ;
			pl_w->routine_3 = 0 ;
			break ;
		}
		pl_w->routine_2 = 0 ;
		pl_w->char_no.b.grp = 12 ;
		pl_se_req( pl_w, SE_SWING_H ) ;
		if( pl_w->atck_lever & KEY_UP ){
			pl_char_set( pl_w, Char_Jump_K, 6 ) ;
			pl_w->atck_no = pl2e_at_jup_hk ;
		} else {
			pl_char_set( pl_w, Char_Jump_K, 2 ) ;
			pl_w->atck_no = ATCK_JUMP_HK ;
		}
		break ;
	}
	HIT_RESET( pl_w ) ;
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
void pl2e_atck( PLAYER_WORK *pl_w )
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

static void stand_lp_move( PLAYER_WORK *pl_w );
static void stand_mp_move( PLAYER_WORK *pl_w );
static void stand_hp_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void stand_punch_move( PLAYER_WORK *pl_w )
{
	static void ( *stand_punch_jmp[])( PLAYER_WORK *pl_w ) = {
		stand_lp_move, stand_mp_move, stand_hp_move,
	} ;

	stand_punch_jmp[pl_w->nml_no]( pl_w ) ;
}

/*==============================================================*/
static void stand_lp_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void stand_mp_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			Fires a shot, then holds the pose until timer0		*/
/*			runs out											*/
/*==============================================================*/
static void stand_hp_shot( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shl2e_05_set( pl_w, 1, 0 ) ;
	}
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Extra, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void stand_hp_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void stand_hp_move( PLAYER_WORK *pl_w )
{
	static void ( *stand_hp_jmp[])( PLAYER_WORK *pl_w ) = {
		stand_hp_shot, stand_hp_end,
	} ;

	stand_hp_jmp[pl_w->routine_2]( pl_w ) ;
}

static void crouch_lp_move( PLAYER_WORK *pl_w );
static void crouch_mp_move( PLAYER_WORK *pl_w );
static void crouch_hp_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void crouch_punch_move( PLAYER_WORK *pl_w )
{
	static void ( *crouch_punch_jmp[])( PLAYER_WORK *pl_w ) = {
		crouch_lp_move, crouch_mp_move, crouch_hp_move,
	} ;

	crouch_punch_jmp[pl_w->nml_no]( pl_w ) ;
}

/*==============================================================*/
static void crouch_lp_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void crouch_mp_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			The crouching heavy punch: a flash, then a beam		*/
/*			held for the frames the animation gives				*/
/*==============================================================*/
static void crouch_hp_start( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		pl_w->timer0 = 10 ;
		pls2e_00_set( pl_w, 2, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void crouch_hp_wait( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_3 ++ ;
		shl2e_00_set( pl_w, 0, 2 ) ;
		pl_w->cancel_timer = 16 ;
		pl_w->unk_27b = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void crouch_hp_shot( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( !-- pl_w->timer1 ){
		pl_w->routine_3 ++ ;
		lw->beam_end_flag = 1 ;
	}
}

/*--------------------------------------------------------------*/
static void crouch_hp_end( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void crouch_hp_move( PLAYER_WORK *pl_w )
{
	static void ( *crouch_hp_jmp[])( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw ) = {
		crouch_hp_start, crouch_hp_wait, crouch_hp_shot, crouch_hp_end,
	} ;

	crouch_hp_jmp[pl_w->routine_3]( pl_w, ( PL2E_LOCAL_W * )pl_w->tlocal ) ;
}

static void stand_lk_move( PLAYER_WORK *pl_w );
static void stand_mk_move( PLAYER_WORK *pl_w );
static void stand_hk_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void stand_kick_move( PLAYER_WORK *pl_w )
{
	static void ( *stand_kick_jmp[])( PLAYER_WORK *pl_w ) = {
		stand_lk_move, stand_mk_move, stand_hk_move,
	} ;

	stand_kick_jmp[pl_w->nml_no]( pl_w ) ;
}

/*==============================================================*/
static void stand_lk_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void stand_mk_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shl2e_05_set( pl_w, 1, 2 ) ;
	}
}

/*==============================================================*/
static void stand_hk_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shl2e_05_set( pl_w, 1, 4 ) ;
	}
}

static void crouch_lk_move( PLAYER_WORK *pl_w );
static void crouch_mk_move( PLAYER_WORK *pl_w );
static void crouch_hk_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void crouch_kick_move( PLAYER_WORK *pl_w )
{
	static void ( *crouch_kick_jmp[])( PLAYER_WORK *pl_w ) = {
		crouch_lk_move, crouch_mk_move, crouch_hk_move,
	} ;

	crouch_kick_jmp[pl_w->nml_no]( pl_w ) ;
}

/*==============================================================*/
static void crouch_lk_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			Fires a shot, then slides							*/
/*==============================================================*/
static void crouch_mk_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type & 0x02 ){
		pl_w->cg_type &= 0xfd ;
		shl2e_05_set( pl_w, 1, 1 ) ;
	}
	if( pl_w->cg_type & 0x01 ){
		pl_w->cg_type &= 0xfe ;
		pl_w->spd.x = -DOT_X( 4.0f ) ;
		pl_w->acc.x = DOT_X( 0.25f ) ;
		if( pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
	}
}

/*==============================================================*/
static void crouch_hk_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shl2e_05_set( pl_w, 1, 5 ) ;
	}
}

/*==============================================================*/
static void sjump_move( PLAYER_WORK *pl_w )
{
	if( pl_w->fly_flag ){
		jump_move( pl_w ) ;
		return ;
	}
	pl_jump_hit_ck( pl_w ) ;
	pl_sjump_ctrl( pl_w ) ;
	jump_move( pl_w ) ;
}

/*==============================================================*/
static void jump_move( PLAYER_WORK *pl_w )
{
	if( pl_w->fly_flag && pl_air_dash_sw_ck( pl_w ) ) return ;
	pl_jump_move( pl_w ) ;
	pl_sjump_top_ck( pl_w ) ;
	if( pl_w->atck_pk == 1 ) jump_kick_move( pl_w ) ;
	else jump_punch_move( pl_w ) ;
	if( pl_land_ck( pl_w ) ) pl_land_set( pl_w ) ;
}

static void jump_lp_move( PLAYER_WORK *pl_w );
static void jump_mp_move( PLAYER_WORK *pl_w );
static void jump_hp_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void jump_punch_move( PLAYER_WORK *pl_w )
{
	static void ( *jump_punch_jmp[])( PLAYER_WORK *pl_w ) = {
		jump_lp_move, jump_mp_move, jump_hp_move,
	} ;

	jump_punch_jmp[pl_w->nml_no]( pl_w ) ;
}

/*==============================================================*/
static void jump_lp_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_jump_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shl2e_05_set( pl_w, 1, 6 ) ;
	}
}

/*==============================================================*/
static void jump_mp_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_jump_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shl2e_05_set( pl_w, 1, 3 ) ;
	}
}

/*==============================================================*/
static void jump_hp_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

static void jump_lk_move( PLAYER_WORK *pl_w );
static void jump_mk_move( PLAYER_WORK *pl_w );
static void jump_hk_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void jump_kick_move( PLAYER_WORK *pl_w )
{
	static void ( *jump_kick_jmp[])( PLAYER_WORK *pl_w ) = {
		jump_lk_move, jump_mk_move, jump_hk_move,
	} ;

	jump_kick_jmp[pl_w->nml_no]( pl_w ) ;
}

/*==============================================================*/
static void jump_lk_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/*==============================================================*/
static void jump_mk_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/*==============================================================*/
static void jump_hk_nml( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/*==============================================================*/
/*			The dive: a hop back, then down at an angle			*/
/*==============================================================*/
static void dive_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_3 ++ ;
		pl_w->spd.x = -DOT_X( 2.0f ) ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = -2.142857f ;
		pl_w->acc.y = -DOT_Y( 0.125f ) ;
		if( pl_w->disp_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
	}
}

/*--------------------------------------------------------------*/
static void dive_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->hit_flag ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Extra, 5 ) ;
		pl_w->spd.x = DOT_X( 4.0f ) ;
		pl_w->acc.x = -0.052083332f ;
		pl_w->spd.y = DOT_Y( 6.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		if( pl_w->disp_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
	}
}

/*--------------------------------------------------------------*/
static void dive_end( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void dive( PLAYER_WORK *pl_w )
{
	static void ( *dive_jmp[])( PLAYER_WORK *pl_w ) = {
		dive_start, dive_move, dive_end,
	} ;

	dive_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void jump_hk_move( PLAYER_WORK *pl_w )
{
	static void ( *jump_hk_jmp[])( PLAYER_WORK *pl_w ) = {
		jump_hk_nml, dive,
	} ;

	jump_hk_jmp[pl_w->routine_2]( pl_w ) ;
}

/****************************************************************/
/*			The launcher										*/
/****************************************************************/
void pl2e_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_gauge_add( pl_w, 5 ) ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Extra, 6 ) ;
		pl_w->atck_no = pl2e_at_upper ;
		HIT_RESET( pl_w ) ;
		pl_se_req( pl_w, SE_SWING_H ) ;
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
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shl2e_05_set( pl_w, 1, 4 ) ;
	}
}

static void dash_init( PLAYER_WORK *pl_w );
static void dash_move( PLAYER_WORK *pl_w );
static void dash_end( PLAYER_WORK *pl_w );

/****************************************************************/
void pl2e_dash( PLAYER_WORK *pl_w )
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
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 8.0f ) : -DOT_X( 8.0f ) ;
		pl_w->timer0 = 20 ;
	}
}

/*--------------------------------------------------------------*/
static void dash_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->acc.x = pl_w->pl_dir ? -DOT_X( 0.25f ) : DOT_X( 0.25f ) ;
		pl_char_set( pl_w, Char_Dash, 2 ) ;
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
static void noki_move( PLAYER_WORK *pl_w );
static void noki_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash										*/
/****************************************************************/
void pl2e_noki( PLAYER_WORK *pl_w )
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
		pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 8.0f ) : DOT_X( 8.0f ) ;
		pl_w->timer0 = 20 ;
	}
}

/*--------------------------------------------------------------*/
static void noki_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->acc.x = pl_w->pl_dir ? DOT_X( 0.25f ) : -DOT_X( 0.25f ) ;
		pl_char_set( pl_w, Char_Dash, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void noki_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			He flies down into the stage, lands and stands		*/
/*			up													*/
/*==============================================================*/
static void appear_init( PLAYER_WORK *pl_w )
{
	PL2E_LOCAL_W *lw = ( PL2E_LOCAL_W * )pl_w->tlocal ;

	pl_w->routine_2 ++ ;
	pl_w->disp_flag = 1 ;
	lw->land_flag = 0 ;
	pl_w->pos.y += 480.0f ;
	pl_w->spd.y = -DOT_Y( 4.0f ) ;
	pl_char_set( pl_w, Char_Appear, 0 ) ;
	pl_voice_req( pl_w, 34 ) ;
}

/*--------------------------------------------------------------*/
static void appear_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->ground_y >= pl_w->pos.y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.y = -DOT_Y( 2.0f ) ;
		pl_w->timer0 = 8 ;
	}
}

/*--------------------------------------------------------------*/
static void appear_land( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->timer0 = 8 ;
		pl_char_set( pl_w, Char_Appear, 1 ) ;
		pls2e_00_set( pl_w, 1, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_rise( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->timer0 = 8 ;
		pl_char_set( pl_w, Char_Appear, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_stand( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->timer0 = 10 ;
		pl_char_set( pl_w, Char_Appear, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_hop( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.y = DOT_Y( 8.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.5f ) ;
		pl_char_set( pl_w, Char_Appear, 9 ) ;
		pls2e_00_set( pl_w, 0, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_end( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->ground_y >= pl_w->pos.y ){
		fall_se_req( pl_w ) ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->routine_1 ++ ;
		pl_land_ef_set( pl_w ) ;
	}
}

/****************************************************************/
void pl2e_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_init, appear_fall, appear_land, appear_rise, appear_stand, appear_hop, appear_end,
	} ;

	appear_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void win_ko_init( PLAYER_WORK *pl_w )
{
	if( !( Rnd() & 1 ) ) pl_w->char_no.b.no = 0 ;
	else pl_w->char_no.b.no = 1 ;
	pl_char_set( pl_w, Char_Win, pl_w->char_no.b.no ) ;
	pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void win_ko_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
/*			One of two poses at random							*/
/*==============================================================*/
static void win_time_ko( PLAYER_WORK *pl_w )
{
	static void ( *win_time_ko_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko_init, win_ko_move,
	} ;

	win_time_ko_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void lose_time_ko( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, 2 ) ;
	} else pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void win_time( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, 0 ) ;
	} else pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void draw_time( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, 2 ) ;
	} else pl_char_move( pl_w ) ;
}

/****************************************************************/
void pl2e_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_time_ko, lose_time_ko, win_time, lose_time_ko, draw_time,
	} ;

	if( pl_win_end_ck( pl_w ) ) pl_stand_ret( pl_w ) ;
	else win_jmp[pl_w->type]( pl_w ) ;
}

/*==============================================================*/
/*			The partner's kick: a dash through the air			*/
/*==============================================================*/
static void t_kick_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->spd.x = -30.0f ;
	if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = -DOT_Y( 0.125f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->atck_no = pl2e_at_t_kick ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 1 ) ;
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
static void t_kick_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stop_stand_ret( pl_w ) ;
}

/****************************************************************/
void pl2e_t_kick( PLAYER_WORK *pl_w )
{
	static void ( *t_kick_jmp[])( PLAYER_WORK *pl_w ) = {
		t_kick_init, t_kick_move, t_kick_end,
	} ;

	t_kick_jmp[pl_w->routine_2]( pl_w ) ;
}

/* sp_cannon on the ground: three missiles, or five for the stronger buttons */
static void cannon_move( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw );

/*==============================================================*/
static void cannon_init( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	pl_w->routine_3 ++ ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	if( !pl_w->atck_level ) lw->shot_cnt = 3 ;
	else lw->shot_cnt = 5 ;
	pl_w->atck_no = pl2e_at_cannon ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level ) ;
	pl_w->timer1 = pl_w->cg_type ;
	pl_w->cg_type = 0 ;
	cannon_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void cannon_move( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		pl_w->timer0 = 0 ;
		pl_w->cancel_timer = 16 ;
		pl_w->unk_27b = 0 ;
	}
}

/*==============================================================*/
/*			One missile every four frames						*/
/*==============================================================*/
static void cannon_shot( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( lw->shot_cnt && !pl_w->timer0 -- ){
		pl_w->timer0 = 3 ;
		shl2e_05_set( pl_w, 2, 0 ) ;
		lw->shot_cnt -- ;
	}
	if( !-- pl_w->timer1 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void cannon_end( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void cannon_ground( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	static void ( *cannon_ground_jmp[])( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw ) = {
		cannon_init, cannon_move, cannon_shot, cannon_end,
	} ;

	cannon_ground_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/* sp_cannon in the air */
static void cannon_fall( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw );
static void cannon_air_move( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw );

/*==============================================================*/
static void cannon_air_init( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	pl_w->routine_3 ++ ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.y = -2.142857f ;
	if( !pl_w->atck_level ) lw->shot_cnt = 3 ;
	else lw->shot_cnt = 5 ;
	pl_w->atck_no = pl2e_at_cannon ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 4 ) ;
	pl_w->timer1 = pl_w->cg_type ;
	pl_w->cg_type = 0 ;
	cannon_air_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void cannon_air_move( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	cannon_fall( pl_w, lw ) ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		pl_w->timer0 = 0 ;
		pl_w->cancel_timer = 16 ;
		pl_w->unk_27b = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void cannon_air_shot( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	cannon_fall( pl_w, lw ) ;
	pl_char_move( pl_w ) ;
	if( lw->shot_cnt && !pl_w->timer0 -- ){
		pl_w->timer0 = 3 ;
		shl2e_05_set( pl_w, 2, 1 ) ;
		lw->shot_cnt -- ;
	}
	if( !-- pl_w->timer1 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 6 ) ;
	}
}

/*--------------------------------------------------------------*/
static void cannon_air_end( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	cannon_fall( pl_w, lw ) ;
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void cannon_air( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	static void ( *cannon_air_jmp[])( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw ) = {
		cannon_air_init, cannon_air_move, cannon_air_shot, cannon_air_end,
	} ;

	cannon_air_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*==============================================================*/
/*			Falls while not flying, but not through the			*/
/*			floor												*/
/*==============================================================*/
static void cannon_fall( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	if( !pl_w->fly_flag ){
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
	if( pl_w->ground_y > pl_w->pos.y ) pl_w->pos.y = pl_w->ground_y ;
}

/*==============================================================*/
static void sp_cannon( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	static void ( *cannon_jmp[])( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw ) = {
		cannon_ground, cannon_air,
	} ;

	if( pl_w->pl_stance == STANCE_AIR ) pl_w->routine_2 = 1 ;
	cannon_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/* sp_ccannon: the missiles, crouching */
static void ccannon_move( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw );

/*==============================================================*/
static void ccannon_init( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_CROUCH ;
	pl_w->pos.y = pl_w->ground_y ;
	if( !pl_w->atck_level ) lw->shot_cnt = 3 ;
	else lw->shot_cnt = 5 ;
	pl_w->atck_no = pl2e_at_cannon ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 8 ) ;
	pl_w->timer1 = pl_w->cg_type ;
	pl_w->cg_type = 0 ;
	ccannon_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void ccannon_move( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 0 ;
		pl_w->cancel_timer = 16 ;
		pl_w->unk_27b = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void ccannon_shot( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( lw->shot_cnt && !pl_w->timer0 -- ){
		pl_w->timer0 = 3 ;
		shl2e_05_set( pl_w, 2, 2 ) ;
		lw->shot_cnt -- ;
	}
	if( !-- pl_w->timer1 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 10 ) ;
	}
}

/*--------------------------------------------------------------*/
static void ccannon_end( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_ccannon( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	static void ( *ccannon_jmp[])( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw ) = {
		ccannon_init, ccannon_move, ccannon_shot, ccannon_end,
	} ;

	ccannon_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/* sp_repulsor: a burst of shots */
static void repulsor_move( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw );

/*==============================================================*/
static void repulsor_init( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->atck_no = pl2e_at_repulsor ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 12 ) ;
	pl_w->timer0 = 4 ;
	pl_w->timer1 = !pl_w->atck_level ? 8 : 12 ;
	pl_w->sub_type = 0 ;
	repulsor_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void repulsor_move( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, 14 ) ;
	}
}

/*--------------------------------------------------------------*/
static void repulsor_shot( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( !-- pl_w->timer1 ){
		shl2e_01_set( pl_w, 0, pl_w->sub_type ++ ) ;
		pl_w->timer1 = !pl_w->atck_level ? 8 : 12 ;
		if( !-- pl_w->timer0 ){
			pl_w->routine_2 ++ ;
			pl_w->timer0 = 40 ;
		}
	}
}

/*--------------------------------------------------------------*/
static void repulsor_wait( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, 15 ) ;
	}
}

/*--------------------------------------------------------------*/
static void repulsor_end( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_repulsor( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	static void ( *repulsor_jmp[])( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw ) = {
		repulsor_init, repulsor_move, repulsor_shot, repulsor_wait, repulsor_end,
	} ;

	repulsor_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/* sp_bomb on the ground */
static void bomb_move( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw );

/*==============================================================*/
static void bomb_init( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	pl_w->routine_3 ++ ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->atck_no = pl2e_at_bomb ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 16 ) ;
	bomb_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void bomb_move( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shl2e_02_set( pl_w, 0, 0 ) ;
		shl2e_02_set( pl_w, 0, 1 ) ;
	}
}

/*==============================================================*/
static void bomb_ground( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	static void ( *bomb_ground_jmp[])( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw ) = {
		bomb_init, bomb_move,
	} ;

	bomb_ground_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/* sp_bomb in the air */
static void bomb_air_move( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw );

/*==============================================================*/
static void bomb_air_init( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	pl_w->routine_3 ++ ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl2e_at_bomb ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 17 ) ;
	bomb_air_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void bomb_air_move( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_jump_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type & 2 ){
		pl_w->cg_type &= 0xfd ;
		pls2e_00_set( pl_w, 6, 0 ) ;
		pls2e_00_set( pl_w, 6, 1 ) ;
	}
	if( pl_w->cg_type & 1 ){
		pl_w->cg_type &= 0xfe ;
		shl2e_02_set( pl_w, 0, 0 ) ;
		shl2e_02_set( pl_w, 0, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void bomb_air( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	static void ( *bomb_air_jmp[])( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw ) = {
		bomb_air_init, bomb_air_move,
	} ;

	bomb_air_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*==============================================================*/
static void sp_bomb( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	static void ( *bomb_jmp[])( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw ) = {
		bomb_ground, bomb_air,
	} ;

	if( pl_w->pl_stance == STANCE_AIR ) pl_w->routine_2 = 1 ;
	bomb_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/* sp_fly: flight, which takes off or, while flying, ends it */
static void fly_move( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw );

/*==============================================================*/
static void fly_init( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	if( pl_w->fly_flag ){
		fly_end( pl_w ) ;
		return ;
	}
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	if( pl_w->pl_stance != STANCE_AIR ) pl_sp_ef_set( pl_w ) ;
	pl_w->fly_flag = 1 ;
	lw->fly_timer = 480 ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->sjump_flag = 0 ;
	pl_w->jump_com_cnt = 0 ;
	pl_w->jump_dash_cnt = 0 ;
	pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 6.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.125f ) ;
	pl_char_set( pl_w, Char_Fly, 0 ) ;
	fly_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void fly_move( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ){
		pls2e_00_set( pl_w, 3, 0 ) ;
		pls2e_00_set( pl_w, 7, 0 ) ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
static void fly_end( PLAYER_WORK *pl_w )
{
	pl_w->fly_flag = 0 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_jump_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_fly( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	static void ( *fly_jmp[])( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw ) = {
		fly_init, fly_move,
	} ;

	fly_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/*==============================================================*/
/*			An attack during take-off slows the flight, or		*/
/*			drops him when not flying							*/
/*==============================================================*/
static void fly_atck_set( PLAYER_WORK *pl_w )
{
	if( pl_w->char_no.b.grp == Char_Fly ){
		pl_dir_set( pl_w ) ;
		if( pl_w->fly_flag ){
			pl_w->spd.x /= 8.0f ;
			pl_w->acc.x /= 8.0f ;
			pl_w->spd.y /= 8.0f ;
			pl_w->acc.y /= 8.0f ;
		} else {
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_w->acc.y = -DOT_Y( 0.375f ) ;
		}
	}
}

/* sp_proton: a hyper combo, a beam held for two seconds */
static void proton_move( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw );

/*==============================================================*/
static void proton_init( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	lw->proton_end_flag = lw->ready_flag = 0 ;
	pl_w->atck_no = pl2e_at_proton ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 0 ) ;
	proton_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void proton_move( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		if( !shl2e_03_set( pl_w, 0, 0 ) ){
			pl_stand_ret( pl_w ) ;
			return ;
		}
	}
	if( lw->ready_flag ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Super, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void proton_shot( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type & 2 ){
		pl_w->cg_type &= 0xfd ;
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 120 ;
		pl_w->timer1 = 0 ;
		if( !shl2e_06_set( pl_w, 0, 0 ) ) pl_stand_ret( pl_w ) ;
		else {
			ofs.x = -DOT_X( 98.0f ) ;
			ofs.y = DOT_Y( 97.0f ) ;
			ofs.z = 0.0f ;
			pl_w->stop_move_flag = 0 ;
			pl_w->stop_move_timer = 0 ;
			pl_hyper_start( pl_w, &ofs, 1 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void proton_hold( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 10 ;
		lw->proton_end_flag = 1 ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void proton_wait( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Super, 2 ) ;
		shl2e_03_set( pl_w, 1, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void proton_end( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_proton( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	static void ( *proton_jmp[])( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw ) = {
		proton_init, proton_move, proton_shot, proton_hold, proton_wait, proton_end,
	} ;

	proton_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/* sp_destroyer: a hyper combo, a rain of shots from above */
static void destroyer_move( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw );
static void destroyer_fire( PLAYER_WORK *pl_w );

/*==============================================================*/
static void destroyer_init( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->atck_no = pl2e_at_destroyer ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 3 ) ;
	destroyer_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void destroyer_move( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->timer0 = 48 ;
		ofs.x = 0.0f ;
		ofs.y = DOT_Y( 74.0f ) ;
		ofs.z = 0.0f ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void destroyer_shot( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	int i ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	destroyer_fire( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_se_req( pl_w, 76 ) ;
		for( i = 0 ; i < 10 ; i++ ) pls2e_00_set( pl_w, 5, i ) ;
	}
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 64 ;
	}
}

/*--------------------------------------------------------------*/
static void destroyer_hold( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Super, 4 ) ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void destroyer_end( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			A shot every four frames, from each side in turn	*/
/*==============================================================*/
static void destroyer_fire( PLAYER_WORK *pl_w )
{
	int side ;

	if( !( pl_w->timer0 & 3 ) ){
		side = pl_w->timer0 >> 2 ;
		side &= 1 ;
		shl2e_05_set( pl_w, 0, side ) ;
	}
}

/*==============================================================*/
static void sp_destroyer( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	static void ( *destroyer_jmp[])( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw ) = {
		destroyer_init, destroyer_move, destroyer_shot, destroyer_hold, destroyer_end,
	} ;

	destroyer_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/* sp_chouhatsu */
static void chouhatsu_end( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw );

/*==============================================================*/
static void chouhatsu_init( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_char_set( pl_w, Char_Extra, 3 ) ;
	chouhatsu_end( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void chouhatsu_end( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	static void ( *chouhatsu_jmp[])( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw ) = {
		chouhatsu_init, chouhatsu_end,
	} ;

	chouhatsu_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/* sp_snapback: the snapback */
static void snapback_move( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw );

/*==============================================================*/
static void snapback_init( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
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
	pl_w->atck_no = pl2e_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 18 ) ;
	pl_w->timer0 = 40 ;
	snapback_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void snapback_move( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_char_move( pl_w ) ;
	if( !pl_w->timer0 -- ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, 19 ) ;
		return ;
	}
	if( pl_w->cg_type & 1 ){
		pl_w->cg_type &= 0xfe ;
		shl2e_05_set( pl_w, 1, 8 ) ;
	}
	if( pl_w->cg_type & 2 ){
		pl_w->cg_type &= 0xfd ;
		ofs.x = 0.0f ;
		ofs.y = DOT_Y( 59.0f ) ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
}

/*--------------------------------------------------------------*/
static void snapback_end( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_snapback( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	static void ( *snapback_jmp[])( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw ) = {
		snapback_init, snapback_move, snapback_end,
	} ;

	snapback_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/****************************************************************/
void pl2e_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *pl_sp_tbl[])( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw ) = {
		sp_repulsor, sp_proton, sp_cannon, sp_bomb, sp_chouhatsu, sp_fly, sp_destroyer, sp_cannon,
		sp_snapback, sp_cannon, sp_cannon, sp_cannon, sp_ccannon,
	} ;
	PL2E_LOCAL_W *lw = ( PL2E_LOCAL_W * )pl_w->tlocal ;

	pl_sp_tbl[pl_w->pl_sp_type]( pl_w, lw ) ;
}

/*==============================================================*/
static PLAYER_WORK *nml_catch_ck( PLAYER_WORK *pl_w )
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
	if( pl_w->atck_pk == 1 && pl_w->atck_level == 1 ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 0 ;
			return em ;
		}
		return 0 ;
	}
	return 0 ;
}

/*==============================================================*/
static PLAYER_WORK *jump_catch_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( !( pl_w->dir = ( pl_w->atck_lever & ( KEY_FORWARD | KEY_BACK ) ) >> 10 ) ) return 0 ;
	if( pl_w->atck_pk == 0 && pl_w->atck_level == 1 ){
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

/****************************************************************/
/*			Can a throw start?									*/
/****************************************************************/
PLAYER_WORK *pl2e_catch_ck( PLAYER_WORK *pl_w )
{
	static PLAYER_WORK *( *catch_ck_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_catch_ck, crouch_catch_ck, jump_catch_ck, crouch_catch_ck,
	} ;

	return catch_ck_jmp[pl_w->pl_stance]( pl_w ) ;
}

/*==============================================================*/
/*			The kick throw drags the opponent along the			*/
/*			ground												*/
/*==============================================================*/
static void nml_k_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_w->dir & 1 ){
		pl_w->pl_dir ^= 1 ;
		pl_w->disp_dir = pl_w->pl_dir ;
	}
	pl_w->hit_stop = 10 ;
	pl_char_set( pl_w, Char_Catch, 0 ) ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.x = -10.0f ;
	pl_w->acc.x = -DOT_X( 0.25f ) ;
	if( pl_w->disp_dir ){
		pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = -pl_w->acc.x ;
	}
	ofs.x = -DOT_X( 35.0f ) ;
	ofs.y = 135.0f ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	catch_gauge_add( pl_w ) ;
}

/*==============================================================*/
static void jump_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_w->dir & 1 ){
		pl_w->pl_dir ^= 1 ;
		pl_w->disp_dir = pl_w->pl_dir ;
	}
	pl_w->hit_stop = 10 ;
	pl_char_set( pl_w, Char_Catch, 3 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	ofs.x = -DOT_X( 35.0f ) ;
	ofs.y = 135.0f ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	catch_gauge_add( pl_w ) ;
}

/*==============================================================*/
static void nml_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_w->dir & 1 ){
		pl_w->pl_dir ^= 1 ;
		pl_w->disp_dir = pl_w->pl_dir ;
	}
	pl_w->hit_stop = 10 ;
	pl_char_set( pl_w, Char_Catch, 4 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	ofs.x = -DOT_X( 35.0f ) ;
	ofs.y = 135.0f ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	catch_gauge_add( pl_w ) ;
	pl_w->timer0 = 60 ;
}

/****************************************************************/
/*			Start the throw that pl2e_catch_ck chose			*/
/****************************************************************/
void pl2e_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_k_set, jump_p_set, nml_p_set,
	} ;

	catch_set_jmp[pl_w->catch_no]( pl_w ) ;
}

/*==============================================================*/
static void nml_k_drag( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	PLAYER_WORK *em = pl_w->catch_w ;

	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		if( pl_w->wall_flag || em->wall_flag ){
			pl_w->routine_2 ++ ;
			pl_char_set( pl_w, Char_Catch, 1 ) ;
			pl_w->timer0 = 56 ;
		}
	}
}

/*--------------------------------------------------------------*/
static void nml_k_throw( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	pl_char_move( pl_w ) ;
	if( !-- pl_w->timer0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type & 1 ){
		pl_w->cg_type &= 0xfe ;
		shl2e_05_set( pl_w, 1, 9 ) ;
	}
	if( pl_w->cg_type & 2 ){
		pl_w->cg_type &= 0xfd ;
		em = pl_w->catch_w ;
		em->catch_type = 1 ;
		em->pl_stance = STANCE_AIR ;
		em->atck_no = pl2e_at_throw_k ;
		pl_w->atck_no = pl2e_at_throw_k ;
		em->pl_dir = pl_w->pl_dir ;
		em->pl_dir ^= 1 ;
		scr_work.quake_level = 3 ;
		scr_work.quake_flag = 1 ;
	}
}

/*==============================================================*/
static void nml_k_catch( PLAYER_WORK *pl_w )
{
	static void ( *nml_k_catch_jmp[])( PLAYER_WORK *pl_w, PL2E_LOCAL_W *lw ) = {
		nml_k_drag, nml_k_throw,
	} ;

	nml_k_catch_jmp[pl_w->routine_2]( pl_w, ( PL2E_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void jump_p_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_jump_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type & 1 ){
		pl_w->cg_type &= 0xfe ;
		shl2e_05_set( pl_w, 1, 7 ) ;
	}
	if( pl_w->cg_type & 2 ){
		pl_w->cg_type &= 0xfd ;
		em = pl_w->catch_w ;
		em->catch_type = 1 ;
		em->pl_stance = STANCE_AIR ;
		em->atck_no = pl2e_at_air_throw ;
		pl_w->atck_no = pl2e_at_air_throw ;
		em->pl_dir = pl_w->pl_dir ;
		em->pl_dir ^= 1 ;
	}
}

/*==============================================================*/
static void nml_p_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_char_move( pl_w ) ;
	if( !-- pl_w->timer0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type & 1 ){
		pl_w->cg_type &= 0xfe ;
		shl2e_05_set( pl_w, 1, 9 ) ;
	}
	if( pl_w->cg_type & 2 ){
		pl_w->cg_type &= 0xfd ;
		em = pl_w->catch_w ;
		em->catch_type = 1 ;
		em->pl_stance = STANCE_AIR ;
		em->atck_no = pl2e_at_throw_p ;
		pl_w->atck_no = pl2e_at_throw_p ;
		em->pl_dir = pl_w->pl_dir ;
		em->pl_dir ^= 1 ;
	}
}

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void pl2e_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_k_catch, jump_p_catch, nml_p_catch,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no]( pl_w ) ;
}

/*==============================================================*/
static void nml_hold( PLAYER_WORK *pl_w )
{
	catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/****************************************************************/
/*			The opponent's side of each throw					*/
/****************************************************************/
void pl2e_hold( PLAYER_WORK *pl_w )
{
	static void ( *hold_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_hold, nml_hold, nml_hold,
	} ;

	hold_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/****************************************************************/
void pl2e_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl2e_sp_proton ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl2e_sp_destroyer ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl2e_sp_destroyer ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl2e_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl2e_sp_proton ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl2e_sp_destroyer ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl2e_sp_destroyer ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl2e_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl2e_sp_cannon ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl2e_sp_repulsor ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl2e_sp_bomb ;
		break ;
	}
	pl_w->atck_level = 0 ;
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void pl2e_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl2e_sp_cannon ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl2e_sp_repulsor ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl2e_sp_bomb ;
		break ;
	}
	pl_w->atck_level = 0 ;
	act_st_set( pl_w, N_SPECIAL ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl2e.c" End of File	*/

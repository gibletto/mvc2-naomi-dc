/* Sentinel (character 34): command checks, normal attacks, appearance, win and lose poses, special
   moves and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl34_def.h"

void pl34_main( PLAYER_WORK *pl_w );
void pl34_com_ck( PLAYER_WORK *pl_w );
void pl34_atck_set( PLAYER_WORK *pl_w );
void pl34_atck( PLAYER_WORK *pl_w );
void pl34_dash( PLAYER_WORK *pl_w );
void pl34_noki( PLAYER_WORK *pl_w );
void pl34_appear( PLAYER_WORK *pl_w );
void pl34_win( PLAYER_WORK *pl_w );
void pl34_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl34_catch_ck( PLAYER_WORK *pl_w );
void pl34_catch_set( PLAYER_WORK *pl_w );
void pl34_catch( PLAYER_WORK *pl_w );
void pl34_hold( PLAYER_WORK *pl_w );
int pl34_delay_ck( PLAYER_WORK *pl_w );
void pl34_t_kick( PLAYER_WORK *pl_w );
void pl34_upper( PLAYER_WORK *pl_w );
void pl34_sp_unison( PLAYER_WORK *pl_w );
void pl34_sp_gattai( PLAYER_WORK *pl_w );
void pl34_sp_assist( PLAYER_WORK *pl_w );
void pl34_sp_zero( PLAYER_WORK *pl_w );
void pl34_tlocal_init( PLAYER_WORK *pl_w );

/* each attack's table: standing and crouching, jumping, super jumping */
static Sint8 lp_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 mp_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 hp_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 lk_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 mk_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 hk_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 j_lp_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 j_mp_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 j_hp_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 j_lk_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 j_mk_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 j_hk_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 sj_lp_tbl[4] = {  1,  2,  0,  2 } ;
static Sint8 sj_mp_tbl[4] = { -1,  2,  1,  2 } ;
static Sint8 sj_hp_tbl[4] = { -1, -1, -1,  2 } ;
static Sint8 sj_lk_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 sj_mk_tbl[4] = { -1,  2, -1,  2 } ;
static Sint8 sj_hk_tbl[4] = { -1, -1, -1, -1 } ;

/* commands: time or steps, kind, the special move, then the stick and buttons */
static Uint16 com_plasma[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl34_sp_plasma, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Plasma Storm */
static Uint16 com_rocket2[] = { 3, COM_GROUND, pl34_sp_rocket2, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_PUNCH } ;	/* Rocket Punch */
static Uint16 com_j_rocket2[] = { 3, COM_AIR, pl34_sp_rocket2, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_PUNCH } ;	/* Rocket Punch */
static Uint16 com_rocket[] = { 3, COM_GROUND, pl34_sp_rocket, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Rocket Punch */
static Uint16 com_j_rocket[] = { 3, COM_AIR, pl34_sp_rocket, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Rocket Punch */
static Uint16 com_force[] = { 3, COM_GROUND, pl34_sp_force, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Sentinel Force */
static Uint16 com_fly[] = { 3, COM_ALL_SW, pl34_sp_fly, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Flight */
static Uint16 com_hforce[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl34_sp_hforce, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Hyper Sentinel Force */
static Uint16 com_hdrive[] = { 3, COM_AIR | COM_HYPER | COM_ALL_SW, pl34_sp_hdrive, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Hard Drive (hyper) */

static void ( *pl34_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl34_main,
	pl34_com_ck,
	pl34_atck_set,
	pl34_atck,
	pl34_dash,
	pl34_noki,
	pl34_appear,
	pl34_win,
	pl34_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl34_catch_ck,	/* returns a value */
	pl34_catch_set,
	pl34_catch,
	pl34_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl34_delay_ck,
	pl34_t_kick,
	pl34_upper,
	pl34_sp_unison,
	pl34_sp_gattai,
	pl34_sp_assist,
	pl34_sp_zero,
	0,
	0,
	0,
	pl34_tlocal_init,
} ;

static Uint8 hdrive_ck( PLAYER_WORK *pl_w );
static Uint8 plasma_ck( PLAYER_WORK *pl_w );
static Uint8 hforce_ck( PLAYER_WORK *pl_w );
static Uint8 rocket2_ck( PLAYER_WORK *pl_w );
static Uint8 j_rocket2_ck( PLAYER_WORK *pl_w );
static Uint8 rocket_ck( PLAYER_WORK *pl_w );
static Uint8 j_rocket_ck( PLAYER_WORK *pl_w );
static Uint8 force_ck( PLAYER_WORK *pl_w );
static Uint8 fly_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl34_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl34_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl34_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( hdrive_ck( pl_w ) ) return ;
	if( plasma_ck( pl_w ) ) return ;
	if( hforce_ck( pl_w ) ) return ;
	if( j_rocket2_ck( pl_w ) ) return ;
	if( rocket2_ck( pl_w ) ) return ;
	if( j_rocket_ck( pl_w ) ) return ;
	if( rocket_ck( pl_w ) ) return ;
	if( force_ck( pl_w ) ) return ;
	if( fly_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
/*			On the ground, or once a jump unless super			*/
/*			jumping												*/
/*==============================================================*/
static Uint8 hdrive_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_hdrive, &pl_w->com_w5 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl34_sp_hdrive ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 plasma_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_plasma, &pl_w->com_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl34_sp_plasma ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 hforce_ck( PLAYER_WORK *pl_w )
{
	PL34_LOCAL_W *lw = ( PL34_LOCAL_W * )pl_w->tlocal ;

	if( !com_nml_ck( pl_w, com_hforce, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( lw->drone_cnt ) return 0 ;
	lw->unk_06 = 0 ;
	lw->unk_07 = 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl34_sp_hforce ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
/*			The same move as sp_rocket, at its strongest			*/
/*==============================================================*/
static Uint8 rocket2_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_rocket2, &pl_w->com_w7 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w7 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl34_sp_rocket ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->atck_level = 2 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 j_rocket2_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_j_rocket2, &pl_w->com_w6 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w6 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 1 ;
	pl_w->pl_sp_type = pl34_sp_rocket ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->atck_level = 2 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 rocket_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_rocket, &pl_w->com_w0 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl34_sp_rocket ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 j_rocket_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_j_rocket, &pl_w->com_w1 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 1 ;
	pl_w->pl_sp_type = pl34_sp_rocket ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 force_ck( PLAYER_WORK *pl_w )
{
	PL34_LOCAL_W *lw = ( PL34_LOCAL_W * )pl_w->tlocal ;

	if( !com_nml_ck( pl_w, com_force, &pl_w->com_w2 ) ) return 0 ;
	if( lw->drone_cnt ) return 0 ;
	lw->unk_06 = 0 ;
	lw->unk_07 = 0 ;
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl34_sp_force ;
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
	pl_w->pl_sp_type = pl34_sp_fly ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int snapback_ck( PLAYER_WORK *pl_w )
{
	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl34_sp_snapback ;
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
	pl_w->pl_sp_type = pl34_sp_chouhatsu ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/*==============================================================*/
static int plasma_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_plasma, &pl_w->com_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 1 ;
	return 1 ;
}

/*==============================================================*/
static int hforce_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_hforce, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 6 ;
	return 1 ;
}

/****************************************************************/
int pl34_delay_ck( PLAYER_WORK *pl_w )
{
	if( plasma_delay_ck( pl_w ) || hforce_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

static void flight_end( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw );
static void kick_eff_set( PLAYER_WORK *pl_w, Uint8 no );

/****************************************************************/
void pl34_main( PLAYER_WORK *pl_w )
{
	PL34_LOCAL_W *lw = ( PL34_LOCAL_W * )pl_w->tlocal ;

	pl_w->armor_timer = 2 ;
	if( lw->kick_timer ){
		if( pl_w->char_no.w == lw->kick_char.w ){
			lw->kick_timer -- ;
			if( pl_w->cg_type ){
				kick_eff_set( pl_w, pl_w->cg_type - 1 ) ;
				pl_w->cg_type = 0 ;
			}
		} else {
			lw->kick_timer = 0 ;
		}
	}
	if( pl_w->fly_flag == 0 || pl_w->routine_1 ){
		if( !( pl_w->fly_flag && pl_w->routine_1 == PL_CATCH ) && lw->color_flag ){
			lw->color_flag = 0 ;
			pl_color_set( pl_w, PC_DEFAULT ) ;
		}
		return ;
	}
	if( !--lw->fly_timer ){
		if( pl_w->act_st == N_SPECIAL ) lw->fly_timer = 1 ;
		else flight_end( pl_w, lw ) ;
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
void pl34_atck_set( PLAYER_WORK *pl_w )
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
/*			The heavy kick leaves an effect for a while (see	*/
/*			pl34_main)											*/
/*==============================================================*/
static void stand_kick( PLAYER_WORK *pl_w )
{
	PL34_LOCAL_W *lw = ( PL34_LOCAL_W * )pl_w->tlocal ;

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
		pl_w->chain_tbl = mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_STAND_HK ;
		pl_w->chain_tbl = hk_tbl ;
		pl_se_req( pl_w, SE_SWING_H ) ;
		pl_w->nml_level = 2 ;
		lw->kick_char.b.grp = 8 ;
		lw->kick_char.b.no = pl_w->char_no.b.no ;
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
	if( pl_w->fly_flag
	 || ( pl_w->atck_pk == 0 && ( pl_w->jump_atk_cnt & JATK_P_MASK ) )
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
	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_JUMP_LK ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_lk_tbl ;
		else pl_w->chain_tbl = sj_lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_JUMP_MK ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mk_tbl ;
		else pl_w->chain_tbl = sj_mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_JUMP_HK ;
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
void pl34_atck( PLAYER_WORK *pl_w )
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
/*			The heavy punch fires a shot: wait for the			*/
/*			frame, then hold it for a while						*/
/*==============================================================*/
static void stand_punch_move( PLAYER_WORK *pl_w )
{
	PL34_LOCAL_W *lw = ( PL34_LOCAL_W * )pl_w->tlocal ;

	switch( pl_w->nml_no ){
	case 0:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pl_w->atck_no = pl34_at_stand_mp_2 ;
			HIT_RESET( pl_w ) ;
		}
		break ;
	case 2:
		switch( pl_w->routine_3 ){
		case 0:
			pl_w->routine_3 ++ ;
			lw->punch_cg = 0 ;
		case 1:
			if( pl_w->cg_type ){
				lw->punch_cg = pl_w->cg_type ;
				shl34_03_set( pl_w, 0 ) ;
				pl_voice_req( pl_w, 38 ) ;
				pl_w->routine_3 ++ ;
				pl_w->timer0 = 20 ;
			}
			pl_char_move( pl_w ) ;
			break ;
		case 2:
			if( !--pl_w->timer0 ){
				pl_w->routine_3 ++ ;
				lw->punch_cg = 0 ;
				pl_char_set( pl_w, Char_Stand_P, 3 ) ;
			}
			lw->punch_cg = pl_w->cg_type ;
			pl_char_move( pl_w ) ;
			break ;
		case 3:
			if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
			lw->punch_cg = pl_w->cg_type ;
			if( !lw->punch_cg ) lw->punch_cg = -1 ;
			break ;
		}
		break ;
	}
}

/*==============================================================*/
static void crouch_punch_move( PLAYER_WORK *pl_w )
{
	PL34_LOCAL_W *lw = ( PL34_LOCAL_W * )pl_w->tlocal ;

	switch( pl_w->nml_no ){
	case 0:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pl_w->atck_no = pl34_at_crouch_mp_2 ;
			HIT_RESET( pl_w ) ;
		}
		break ;
	case 2:
		switch( pl_w->routine_3 ){
		case 0:
			pl_w->routine_3 ++ ;
			lw->punch_cg = 0 ;
		case 1:
			if( pl_w->cg_type ){
				lw->punch_cg = pl_w->cg_type ;
				shl34_03_set( pl_w, 1 ) ;
				pl_voice_req( pl_w, 38 ) ;
				pl_w->routine_3 ++ ;
				pl_w->timer0 = 20 ;
			}
			pl_char_move( pl_w ) ;
			break ;
		case 2:
			if( !--pl_w->timer0 ){
				pl_w->routine_3 ++ ;
				lw->punch_cg = 0 ;
				pl_char_set( pl_w, Char_Crouch_P, 3 ) ;
			}
			lw->punch_cg = pl_w->cg_type ;
			pl_char_move( pl_w ) ;
			break ;
		case 3:
			if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
			lw->punch_cg = pl_w->cg_type ;
			if( !lw->punch_cg ) lw->punch_cg = -1 ;
			break ;
		}
		break ;
	}
}

/*==============================================================*/
static void stand_kick_move( PLAYER_WORK *pl_w )
{
	PL34_LOCAL_W *lw = ( PL34_LOCAL_W * )pl_w->tlocal ;

	switch( pl_w->nml_no ){
	case 0:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	case 2:
		lw->kick_timer = 8 ;
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
/*			The medium and heavy kicks hit again on the			*/
/*			frames the animation marks							*/
/*==============================================================*/
static void crouch_kick_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	case 1:
	case 2:
		if( pl_w->cg_type ){
			pl_w->atck_no = pl_w->cg_type ;
			HIT_RESET( pl_w ) ;
			pl_w->cg_type = 0 ;
		}
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
void pl34_upper( PLAYER_WORK *pl_w )
{
	PL34_LOCAL_W *lw = ( PL34_LOCAL_W * )pl_w->tlocal ;

	lw->kick_timer = 8 ;
	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_gauge_add( pl_w, 5 ) ;
		if( pl_w->atck_pk == 1 ){
			pl_w->atck_no = pl34_at_upper ;
			pl_w->pl_stance = STANCE_STAND ;
			pl_char_set( pl_w, Char_Extra, 0 ) ;
			HIT_RESET( pl_w ) ;
			lw->kick_char.w = pl_w->char_no.w ;
		}
	}
	if( pl_w->atck_type == 3 ) pl_smoke_set( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_brake_stop( pl_w ) ;
	if( pl_w->cg_flag1 ){
		pl_se_req( pl_w, SE_SWING_H ) ;
		pl_w->cg_flag1 = 0 ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void dash_init( PLAYER_WORK *pl_w );
static void dash_move( PLAYER_WORK *pl_w );
static void dash_end( PLAYER_WORK *pl_w );

/****************************************************************/
void pl34_dash( PLAYER_WORK *pl_w )
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
		pl_w->timer0 = 20 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 8.0f ) : -DOT_X( 8.0f ) ;
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
	if( !pl_w->timer0-- ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
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
void pl34_noki( PLAYER_WORK *pl_w )
{
	static void ( *noki_jmp[])( PLAYER_WORK *pl_w ) = {
		noki_init, noki_move, noki_end,
	} ;

	noki_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void noki_init( PLAYER_WORK *pl_w )
{
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->routine_2 ++ ;
	pl_w->timer0 = 20 ;
	pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 10.0f ) : DOT_X( 10.0f ) ;
	noki_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void noki_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( !pl_w->timer0-- ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Dash, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void noki_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void appear_init( PLAYER_WORK *pl_w );
static void appear_fall( PLAYER_WORK *pl_w );
static void appear_end( PLAYER_WORK *pl_w );

/****************************************************************/
void pl34_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_init, appear_fall, appear_end,
	} ;

	appear_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			Flies down from above the screen					*/
/*==============================================================*/
static void appear_init( PLAYER_WORK *pl_w )
{
	if( vs_sys_work.vs_st < VS_APPEAR ){
		pl_w->disp_flag = 0 ;
		return ;
	}
	pl_w->routine_2 ++ ;
	pl_w->disp_flag = 1 ;
	pl_w->pos.y += DOT_Y( 256.0f ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.y = -DOT_Y( 2.0f ) ;
	pl_voice_req( pl_w, 47 ) ;
	pl_char_set( pl_w, Char_Fly, 6 ) ;
}

/*--------------------------------------------------------------*/
static void appear_fall( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Jump, 3 ) ;
		pl_land_ef_set( pl_w ) ;
		pl_voice_req( pl_w, 31 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_w->routine_1 ++ ;
}

static void win_init( PLAYER_WORK *pl_w );
static void win_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl34_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_init, win_move,
	} ;

	win_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void win_pose( PLAYER_WORK *pl_w )
{
	pl_char_set( pl_w, Char_Win, CH34_WIN ) ;
}

/*==============================================================*/
static void lose_pose( PLAYER_WORK *pl_w )
{
	pl_char_set( pl_w, Char_Win, CH34_LOSE ) ;
}

/*==============================================================*/
static void win_init( PLAYER_WORK *pl_w )
{
	static void ( *win_pose_jmp[])( PLAYER_WORK *pl_w ) = {
		win_pose, lose_pose, win_pose, lose_pose, lose_pose,
	} ;

	pl_w->routine_2 ++ ;
	pl_w->routine_3 = 0 ;
	win_pose_jmp[pl_w->type]( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_move( PLAYER_WORK *pl_w )
{
	if( pl_win_end_ck( pl_w ) ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
}

static void sp_rocket( PLAYER_WORK *pl_w );
static void sp_plasma( PLAYER_WORK *pl_w );
static void sp_force( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );
static void sp_fly( PLAYER_WORK *pl_w );
static void sp_stand_hk( PLAYER_WORK *pl_w );
static void sp_hforce( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_hdrive( PLAYER_WORK *pl_w );

/****************************************************************/
void pl34_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *pl_sp_tbl[])( PLAYER_WORK *pl_w ) = {
		sp_rocket,					/* 00 Rocket Punch */
		sp_plasma,					/* 01 Plasma Storm (hyper) */
		sp_force,					/* 02 Sentinel Force */
		sp_snapback,					/* 03 the snapback */
		sp_fly,					/* 04 Flight */
		sp_stand_hk,					/* 05 standing heavy kick (beta assist and counter) */
		sp_hforce,					/* 06 Hyper Sentinel Force (hyper) */
		sp_chouhatsu,					/* 07 the taunt */
		sp_hdrive,					/* 08 Hard Drive (hyper) */
		sp_rocket,					/* 09 Rocket Punch at its strongest */
	} ;

	pl_sp_tbl[ pl_w->pl_sp_type ]( pl_w ) ;
}

static void rocket_init( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw );
static void rocket_move( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw );
static void rocket_wait( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw );
static void rocket_next( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw );
static void rocket_end( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw );

/*==============================================================*/
/*			Rocket Punch, on the ground or in the air			*/
/*			(routine_3 1)										*/
/*==============================================================*/
static void sp_rocket( PLAYER_WORK *pl_w )
{
	static void ( *rocket_jmp[])( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw ) = {
		rocket_init, rocket_move, rocket_wait, rocket_next, rocket_end,
	} ;
	PL34_LOCAL_W *lw = ( PL34_LOCAL_W * )pl_w->tlocal ;

	rocket_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/*==============================================================*/
static void rocket_init( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	if( !pl_w->routine_3 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_sp_ef_set( pl_w ) ;
		pl_w->atck_no = pl_w->atck_level ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 0 ) ;
		pls34_00_set( pl_w, pl_w->atck_level ) ;
	} else {
		pl_w->spd.x = pl_w->spd.x / 16.0f ;
		pl_w->acc.x = pl_w->acc.x / 16.0f ;
		pl_w->spd.y = pl_w->spd.y / 16.0f ;
		pl_w->acc.y = pl_w->acc.y / 16.0f ;
		pl_w->atck_no = pl_w->atck_level ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 14 ) ;
		pls34_00_set( pl_w, pl_w->atck_level + 3 ) ;
	}
	lw->fist_end_flag = 0 ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->atck_no = pl34_at_rocket ;
	HIT_RESET( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void rocket_move( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_prm ){
		pl_w->cg_prm = 0 ;
		if( !( pl_w->ef_w = shl34_00_set( pl_w, pl_w->routine_3 * 3 + pl_w->atck_level ) ) ) lw->fist_end_flag = 1 ;
		pl_w->routine_2 ++ ;
		pl_voice_req( pl_w, 20 ) ;
		pl_voice_req( pl_w, 37 ) ;
	}
}

/*--------------------------------------------------------------*/
static void rocket_wait( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw )
{
	if( lw->fist_end_flag ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, pl_w->routine_3 == 0 ? 1 : 15 ) ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ) pl_w->pos.y = pl_w->ground_y ;
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void rocket_next( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ) pl_w->pos.y = pl_w->ground_y ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_prm ){
		pl_w->cg_prm = 0 ;
		pl_w->routine_2 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void rocket_end( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ) pl_w->pos.y = pl_w->ground_y ;
	if( pl_char_move( pl_w ) < 0 ){
		if( !pl_w->routine_3 ){
			pl_stand_ret( pl_w ) ;
		} else {
			pl_w->acc.y = -DOT_Y( 0.375f ) ;
			pl_jump_ret( pl_w ) ;
		}
	}
}

static void plasma_init( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw );
static void plasma_wait( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw );
static void plasma_move( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw );
static void plasma_end( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw );

/*==============================================================*/
/*			A hyper combo										*/
/*==============================================================*/
static void sp_plasma( PLAYER_WORK *pl_w )
{
	static void ( *plasma_jmp[])( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw ) = {
		plasma_init, plasma_wait, plasma_move, plasma_end,
	} ;
	PL34_LOCAL_W *lw = ( PL34_LOCAL_W * )pl_w->tlocal ;

	plasma_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/*==============================================================*/
static void plasma_init( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	lw->unk_0b = 0 ;
	lw->unk_0c = 0 ;
	lw->unk_0d = 0 ;
	lw->unk_0e = 0 ;
	lw->unk_0f = 0 ;
	lw->unk_0a = 16 ;
	pl_dir_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = pl34_at_rocket2 ;
	HIT_RESET( pl_w ) ;
	pl_voice_req( pl_w, 22 ) ;
	pl_char_set( pl_w, Char_Super, 0 ) ;
}

/*--------------------------------------------------------------*/
static void plasma_wait( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	lw->unk_0a = 3 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Super, 1 ) ;
		pl_w->timer0 = 90 ;
		shl34_04_set( pl_w ) ;
		pl_voice_req( pl_w, 45 ) ;
		ofs.x = -200.0f ;
		ofs.y = 240.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void plasma_move( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw )
{
	lw->unk_0a = 3 ;
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( !sw_renda_ck( pl_w ) && !pl_w->timer0-- ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Super, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void plasma_end( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw )
{
	lw->unk_0a = 3 ;
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void force_init( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw );
static void force_move( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw );
static void force_end( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw );

/*==============================================================*/
static void sp_force( PLAYER_WORK *pl_w )
{
	static void ( *force_jmp[])( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw ) = {
		force_init, force_move, force_end,
	} ;
	PL34_LOCAL_W *lw = ( PL34_LOCAL_W * )pl_w->tlocal ;

	force_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/*==============================================================*/
static void force_init( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_sp_ef_set( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->atck_no = pl34_at_force ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 22 ) ;
	pl_w->timer0 = 72 ;
	force_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void force_move( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_voice_req( pl_w, 21 ) ;
		pl_voice_req( pl_w, 47 ) ;
		shl34_01_set( pl_w, pl_w->atck_level ) ;
		force_end( pl_w, lw ) ;
	}
}

/*--------------------------------------------------------------*/
static void force_end( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( !--pl_w->timer0 ) pl_stand_ret( pl_w ) ;
}

static void snapback_init( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw );
static void snapback_move( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw );

/*==============================================================*/
/*			A kick that leaves the same effect as the heavy		*/
/*			kick												*/
/*==============================================================*/
static void sp_snapback( PLAYER_WORK *pl_w )
{
	static void ( *snapback_jmp[])( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw ) = {
		snapback_init, snapback_move,
	} ;
	PL34_LOCAL_W *lw = ( PL34_LOCAL_W * )pl_w->tlocal ;

	snapback_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/*==============================================================*/
static void snapback_init( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = pl34_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 27 ) ;
	lw->kick_char.w = pl_w->char_no.w ;
}

/*--------------------------------------------------------------*/
static void snapback_move( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw )
{
	VEC3 ofs ;

	lw->kick_timer = 8 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_flag1 ){
		pl_w->cg_flag1 = 0 ;
		ofs.x = -80.0f ;
		ofs.y = DOT_Y( 64.0f ) ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
}

static void flight_init( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw );
static void flight_move( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw );
static void flight_end( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw );
static void flight_color( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw );

/*==============================================================*/
/*			Flight: sets fly_flag for 900 frames (see			*/
/*			pl34_main); done again, it ends						*/
/*==============================================================*/
static void sp_fly( PLAYER_WORK *pl_w )
{
	static void ( *flight_jmp[])( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw ) = {
		flight_init, flight_move,
	} ;
	PL34_LOCAL_W *lw = ( PL34_LOCAL_W * )pl_w->tlocal ;

	flight_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/*==============================================================*/
static void flight_init( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw )
{
	if( pl_w->fly_flag ){
		flight_end( pl_w, lw ) ;
		return ;
	}
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	flight_color( pl_w, lw ) ;
	pl_w->fly_flag = 1 ;
	lw->fly_timer = 900 ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->sjump_flag = 0 ;
	pl_w->jump_com_cnt = 0 ;
	pl_w->jump_dash_cnt = 0 ;
	pl_w->timer0 = 16 ;
	pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 3.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.25f ) ;
	pl_char_set( pl_w, Char_Fly, 0 ) ;
	flight_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void flight_move( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( !pl_w->timer0-- || ( pl_w->timer0 <= 8 && pl_w->pl_sw_now ) ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void flight_end( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw )
{
	pl_w->fly_flag = 0 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	if( lw->color_flag ){
		lw->color_flag = 0 ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
	}
	pl_jump_ret( pl_w ) ;
}

static void stand_hk_init( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw );
static void stand_hk_end( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw );

/*==============================================================*/
/*			The heavy kick as a special move					*/
/*==============================================================*/
static void sp_stand_hk( PLAYER_WORK *pl_w )
{
	static void ( *stand_hk_jmp[])( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw ) = {
		stand_hk_init, stand_hk_end,
	} ;
	PL34_LOCAL_W *lw = ( PL34_LOCAL_W * )pl_w->tlocal ;

	stand_hk_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/*==============================================================*/
static void stand_hk_init( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_w->char_no.b.no = 2 ;
	pl_gauge_add( pl_w, 2 ) ;
	if( pl_w->team_type == 3 ){
		pl_w->atck_no = pl34_at_upper ;
		HIT_RESET( pl_w ) ;
	} else {
		pl_w->atck_no = ATCK_STAND_HK ;
		HIT_RESET( pl_w ) ;
	}
	pl_char_set( pl_w, Char_Stand_K, pl_w->char_no.b.no ) ;
}

/*--------------------------------------------------------------*/
static void stand_hk_end( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void hforce_init( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw );
static void hforce_wait( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw );
static void hforce_fire( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw );
static void hforce_end( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw );

/*==============================================================*/
/*			A hyper combo: three shots, one every 30 frames		*/
/*==============================================================*/
static void sp_hforce( PLAYER_WORK *pl_w )
{
	static void ( *hforce_jmp[])( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw ) = {
		hforce_init, hforce_wait, hforce_fire, hforce_end,
	} ;
	PL34_LOCAL_W *lw = ( PL34_LOCAL_W * )pl_w->tlocal ;

	hforce_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/*==============================================================*/
static void hforce_init( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw )
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
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = pl34_at_hforce ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 3 ) ;
	hforce_wait( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void hforce_wait( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 1 ;
		pl_w->timer1 = 3 ;
		pl_voice_req( pl_w, 21 ) ;
		ofs.x = -DOT_X( 128.0f ) ;
		ofs.y = DOT_Y( 118.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
		hforce_fire( pl_w, lw ) ;
	}
}

/*--------------------------------------------------------------*/
static void hforce_fire( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( !--pl_w->timer0 ){
		if( shl34_01_set( pl_w, 2 ) ){
			pl_voice_req( pl_w, 47 ) ;
			if( !--pl_w->timer1 ){
				pl_w->delay_flag = 0 ;
				pl_w->delay_timer = 0 ;
				pl_w->hyper_bg = 0 ;
				pl_w->hyper_bg_tm = 0 ;
				pl_w->routine_2 ++ ;
				pl_char_set( pl_w, Char_Super, 4 ) ;
			}
		}
		pl_w->timer0 = 30 ;
	}
}

/*--------------------------------------------------------------*/
static void hforce_end( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void chouhatsu_init( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw );
static void chouhatsu_end( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw );

/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w )
{
	static void ( *chouhatsu_jmp[])( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw ) = {
		chouhatsu_init, chouhatsu_end,
	} ;
	PL34_LOCAL_W *lw = ( PL34_LOCAL_W * )pl_w->tlocal ;

	chouhatsu_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/*==============================================================*/
static void chouhatsu_init( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_char_set( pl_w, Char_Special, 30 ) ;
}

/*--------------------------------------------------------------*/
static void chouhatsu_end( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void hdrive_init( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw );
static void hdrive_wait( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw );
static void hdrive_rush( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw );
static void hdrive_fall( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw );
static void hdrive_end( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw );

/*==============================================================*/
/*			A hyper combo in the air: a rush that hits up to	*/
/*			nine times, then a fall								*/
/*==============================================================*/
static void sp_hdrive( PLAYER_WORK *pl_w )
{
	static void ( *hdrive_jmp[])( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw ) = {
		hdrive_init, hdrive_wait, hdrive_rush, hdrive_fall, hdrive_end,
	} ;
	PL34_LOCAL_W *lw = ( PL34_LOCAL_W * )pl_w->tlocal ;

	hdrive_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/*==============================================================*/
static void hdrive_init( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_dir_set( pl_w ) ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->atck_no = pl34_at_hdrive ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 5 ) ;
}

/*--------------------------------------------------------------*/
static void hdrive_wait( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 7.0f ) : -DOT_X( 7.0f ) ;
		pl_w->timer0 = 30 ;
		pl_w->timer1 = 9 ;
		ofs.x = -80.0f ;
		ofs.y = DOT_Y( 118.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
		pls34_01_set( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 6 ) ;
	}
}

/*--------------------------------------------------------------*/
static void hdrive_rush( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_w->cg_type ) pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->hit_flag && pl_w->timer1 && pl_w->cg_prm ){
		if( --pl_w->timer1 ){
			pl_w->atck_no = pl34_at_hdrive ;
			HIT_RESET( pl_w ) ;
		} else {
			pl_w->timer0 = 1 ;
		}
	}
	if( !pl_w->timer0-- ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->atck_no = pl34_at_hdrive_end ;
		HIT_RESET( pl_w ) ;
		pl_w->spd.x = pl_w->pl_dir ? 5.0f : -5.0f ;
		pl_w->acc.x = pl_w->pl_dir ? -0.052083332f : 0.052083332f ;
		pl_w->spd.y = -5.0f ;
		pl_w->acc.y = -DOT_X( 0.375f ) ;
		pl_char_set( pl_w, Char_Super, 7 ) ;
	}
}

/*--------------------------------------------------------------*/
static void hdrive_fall( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->pos.y < pl_w->ground_y ){
		flight_end( pl_w, lw ) ;
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Super, 8 ) ;
		pl_land_ef_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void hdrive_end( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}
static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_move( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The tag-in attack: he flies in and lands			*/
/****************************************************************/
void pl34_t_kick( PLAYER_WORK *pl_w )
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
	pl_w->acc.x = 0.0f ;
	if( !pl_w->pl_dir ){
		pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = -pl_w->acc.x ;
	}
	pl_w->spd.y = DOT_Y( 2.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->atck_no = pl34_at_t_kick ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 29 ) ;
}

/*--------------------------------------------------------------*/
static void t_kick_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Extra, 1 ) ;
		pl_land_ef_set( pl_w ) ;
		pl_voice_req( pl_w, 31 ) ;
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
/*			Can a throw start?									*/
/****************************************************************/
PLAYER_WORK *pl34_catch_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( !( pl_w->atck_lever & ( KEY_FORWARD | KEY_BACK ) ) ) return 0 ;
	if( pl_w->atck_pk ) return 0 ;
	if( !pl_w->atck_level ) return 0 ;
	if( ( em = pl_catch_ck( pl_w ) ) ){
		if( pl_w->pl_stance == STANCE_AIR ) pl_w->catch_no = 1 ;
		else if( pl_w->pl_stance == STANCE_STAND ) pl_w->catch_no = 0 ;
		else return 0 ;
		return em ;
	}
	return 0 ;
}

/*==============================================================*/
static void nml_catch_set( PLAYER_WORK *pl_w )
{
	PL34_LOCAL_W *lw = ( PL34_LOCAL_W * )pl_w->tlocal ;
	VEC3 ofs ;
	void *ew ;

	if( pl_w->atck_lever & KEY_BACK ){
		pl_w->pl_dir = pl_w->pl_dir ^ 1 ;
		pl_w->disp_dir = pl_w->disp_dir ^ 1 ;
	}
	pl_w->hit_stop = 10 ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	scr_excep_set( pl_w, 5, 5 ) ;
	ofs.x = -120.0f ;
	ofs.y = DOT_Y( 48.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	lw->fist_end_flag = 0 ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 0 ) ;
	pl_w->atck_level = 1 ;
	if( ( ew = pls34_00_set2( pl_w, 1 ) ) ) pl_catch_pass( pl_w, ew ) ;
	else pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void jump_catch_set( PLAYER_WORK *pl_w )
{
	PL34_LOCAL_W *lw = ( PL34_LOCAL_W * )pl_w->tlocal ;
	VEC3 ofs ;
	void *ew ;

	pl_w->hit_stop = 10 ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	scr_excep_set( pl_w, 5, 5 ) ;
	ofs.x = -120.0f ;
	ofs.y = DOT_Y( 48.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	lw->fist_end_flag = 0 ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 14 ) ;
	pl_w->atck_level = 1 ;
	if( ( ew = pls34_00_set2( pl_w, 4 ) ) ) pl_catch_pass( pl_w, ew ) ;
	else pl_jump_ret( pl_w ) ;
}

/****************************************************************/
/*			Start the throw that pl34_catch_ck chose			*/
/****************************************************************/
void pl34_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_catch_set, jump_catch_set,
	} ;

	catch_set_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_catch_fire( PLAYER_WORK *pl_w )
{
	PL34_LOCAL_W *lw = ( PL34_LOCAL_W * )pl_w->tlocal ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_prm ){
		pl_w->cg_prm = 0 ;
		if( !( pl_w->ef_w = shl34_00_set_1( pl_w, 1 ) ) ) lw->fist_end_flag = 1 ;
		else pl_catch_pass( pl_w, pl_w->ef_w ) ;
		pl_w->routine_2 ++ ;
		pl_voice_req( pl_w, 37 ) ;
	}
}

/*--------------------------------------------------------------*/
static void nml_catch_wait( PLAYER_WORK *pl_w )
{
	PL34_LOCAL_W *lw = ( PL34_LOCAL_W * )pl_w->tlocal ;

	if( lw->fist_end_flag ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, 1 ) ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void nml_catch_next( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_prm ){
		pl_w->cg_prm = 0 ;
		pl_w->routine_2 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void nml_catch_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		SCR_EXCEP_SET ;
		pl_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
static void nml_catch( PLAYER_WORK *pl_w )
{
	static void ( *nml_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_catch_fire, nml_catch_wait, nml_catch_next, nml_catch_end,
	} ;

	nml_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void jump_catch_fire( PLAYER_WORK *pl_w )
{
	PL34_LOCAL_W *lw = ( PL34_LOCAL_W * )pl_w->tlocal ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_prm ){
		pl_w->cg_prm = 0 ;
		if( !( pl_w->ef_w = shl34_00_set_1( pl_w, 4 ) ) ) lw->fist_end_flag = 1 ;
		else pl_catch_pass( pl_w, pl_w->ef_w ) ;
		pl_w->routine_2 ++ ;
		pl_voice_req( pl_w, 37 ) ;
	}
}

/*--------------------------------------------------------------*/
static void jump_catch_wait( PLAYER_WORK *pl_w )
{
	PL34_LOCAL_W *lw = ( PL34_LOCAL_W * )pl_w->tlocal ;

	if( lw->fist_end_flag ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, 15 ) ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void jump_catch_next( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_prm ){
		pl_w->cg_prm = 0 ;
		pl_w->routine_2 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void jump_catch_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		SCR_EXCEP_SET ;
		pl_jump_ret( pl_w ) ;
	}
}

/*==============================================================*/
static void jump_catch( PLAYER_WORK *pl_w )
{
	static void ( *jump_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		jump_catch_fire, jump_catch_wait, jump_catch_next, jump_catch_end,
	} ;

	jump_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void pl34_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_catch, jump_catch,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/****************************************************************/
void pl34_hold( PLAYER_WORK *pl_w )
{
}

/****************************************************************/
void pl34_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl34_sp_hforce ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl34_sp_hforce ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl34_sp_hforce ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl34_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl34_sp_hforce ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl34_sp_hforce ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl34_sp_hforce ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl34_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl34_sp_rocket ;
		pl_w->atck_level = 0 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl34_sp_stand_hk ;
		pl_w->atck_level = 0 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl34_sp_force ;
		pl_w->atck_level = 0 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void pl34_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl34_sp_rocket ;
		pl_w->atck_level = 0 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl34_sp_stand_hk ;
		pl_w->atck_level = 0 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl34_sp_force ;
		pl_w->atck_level = 0 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/*==============================================================*/
static void flight_color( PLAYER_WORK *pl_w, PL34_LOCAL_W *lw )
{
	lw->color_flag = 1 ;
	ef_color_set( pl_w, 3, 2, 1 ) ;
}

/****************************************************************/
/*			Clear the work, all but drone_cnt					*/
/****************************************************************/
void pl34_tlocal_init( PLAYER_WORK *pl_w )
{
	PL34_LOCAL_W *lw = ( PL34_LOCAL_W * )pl_w->tlocal ;
	Uint8 keep = lw->drone_cnt ;

	memset( pl_w->tlocal, 0, sizeof( pl_w->tlocal ) ) ;
	lw->drone_cnt = keep ;
}

/* where the heavy kick's effects appear, in 16.16 */
static Sint32 kick_eff_ofs[][2] = {
	{ 0x400000, 0x300000 },
	{ 0x780000, 0x540000 },
	{ 0x600000, 0x9e0000 },
} ;

/*==============================================================*/
static void kick_eff_set( PLAYER_WORK *pl_w, Uint8 no )
{
	VEC3 pos ;

	pos.x = pl_w->pos.x ;
	pos.y = pl_w->pos.y ;
	if( !pl_w->disp_dir ) pos.x -= kick_eff_ofs[no][0] * 1.6666666f / 65536.0f ;
	else pos.x += kick_eff_ofs[no][0] * 1.6666666f / 65536.0f ;
	pos.y += kick_eff_ofs[no][1] * 2.142857f / 65536.0f ;
	if( !no ) pl_se_req( pl_w, 73 ) ;
	burst_set( pl_w, &pos, 1, 0x409 ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl34.c" End of File	*/

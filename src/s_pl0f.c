/* Doctor Doom (character 0f): command checks, normal attacks, flight, appearance, win poses,
   special moves and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl0f_def.h"

void pl0f_main( PLAYER_WORK *pl_w );
void pl0f_com_ck( PLAYER_WORK *pl_w );
void pl0f_atck_set( PLAYER_WORK *pl_w );
void pl0f_atck( PLAYER_WORK *pl_w );
void pl0f_dash( PLAYER_WORK *pl_w );
void pl0f_noki( PLAYER_WORK *pl_w );
void pl0f_appear( PLAYER_WORK *pl_w );
void pl0f_win( PLAYER_WORK *pl_w );
void pl0f_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl0f_catch_ck( PLAYER_WORK *pl_w );
void pl0f_catch_set( PLAYER_WORK *pl_w );
void pl0f_catch( PLAYER_WORK *pl_w );
void pl0f_hold( PLAYER_WORK *pl_w );
int pl0f_delay_ck( PLAYER_WORK *pl_w );
void pl0f_t_kick( PLAYER_WORK *pl_w );
void pl0f_upper( PLAYER_WORK *pl_w );
void pl0f_sp_unison( PLAYER_WORK *pl_w );
void pl0f_sp_gattai( PLAYER_WORK *pl_w );
void pl0f_sp_assist( PLAYER_WORK *pl_w );
void pl0f_sp_zero( PLAYER_WORK *pl_w );

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
static Sint8 sj_hp_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 sj_lk_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 sj_mk_tbl[4] = { -1,  2, -1,  2 } ;
static Sint8 sj_hk_tbl[4] = { -1, -1, -1, -1 } ;

/* commands: time or steps, kind, the special move, then the stick and buttons */
static Uint16 com_plasma[] = { 3, 0, pl0f_sp_plasma, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Plasma Beam */
static Uint16 com_photon[] = { 5, 0, pl0f_sp_photon, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Photon Shot */
static Uint16 com_shield[] = { 5, COM_GROUND, pl0f_sp_shield, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Molecular Shield */
static Uint16 com_array[] = { 5, COM_HYPER | COM_ALL_SW, pl0f_sp_array, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Photon Array */
static Uint16 com_cage[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl0f_sp_cage, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Electric Cage */
static Uint16 com_flame[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl0f_sp_flame, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Sphere Flame */
static Uint16 com_fly[] = { 3, COM_ALL_SW, pl0f_sp_fly, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Flight */

static void ( *pl0f_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl0f_main,
	pl0f_com_ck,
	pl0f_atck_set,
	pl0f_atck,
	pl0f_dash,
	pl0f_noki,
	pl0f_appear,
	pl0f_win,
	pl0f_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl0f_catch_ck,	/* returns a value */
	pl0f_catch_set,
	pl0f_catch,
	pl0f_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl0f_delay_ck,
	pl0f_t_kick,
	pl0f_upper,
	pl0f_sp_unison,
	pl0f_sp_gattai,
	pl0f_sp_assist,
	pl0f_sp_zero,
} ;

static Uint8 plasma_ck( PLAYER_WORK *pl_w );
static Uint8 photon_ck( PLAYER_WORK *pl_w );
static Uint8 shield_ck( PLAYER_WORK *pl_w );
static Uint8 array_ck( PLAYER_WORK *pl_w );
static Uint8 cage_ck( PLAYER_WORK *pl_w );
static Uint8 flame_ck( PLAYER_WORK *pl_w );
static Uint8 fly_ck( PLAYER_WORK *pl_w );
static int chouhatsu_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static int array_delay_ck( PLAYER_WORK *pl_w );
static int cage_delay_ck( PLAYER_WORK *pl_w );
static int flame_delay_ck( PLAYER_WORK *pl_w );
static void fly_end( PLAYER_WORK *pl_w );

/****************************************************************/
void pl0f_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl0f_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl0f_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( array_ck( pl_w ) ) return ;
	if( cage_ck( pl_w ) ) return ;
	if( flame_ck( pl_w ) ) return ;
	if( fly_ck( pl_w ) ) return ;
	if( plasma_ck( pl_w ) ) return ;
	if( photon_ck( pl_w ) ) return ;
	if( shield_ck( pl_w ) ) return ;
	if( pl_air_dash_sw_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
static Uint8 plasma_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_plasma, &pl_w->com_w0 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0f_sp_plasma ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 photon_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_photon, &pl_w->com_w1 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0f_sp_photon ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 shield_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_shield, &pl_w->com_w2 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0f_sp_shield ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			The hyper combos									*/
/*==============================================================*/
static Uint8 array_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_array, &pl_w->com_w3 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0f_sp_array ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 cage_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_cage, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0f_sp_cage ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 flame_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_flame, &pl_w->com_w5 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0f_sp_flame ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
/*			Flight												*/
/*==============================================================*/
static Uint8 fly_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_fly, &pl_w->com_w6 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w6 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0f_sp_fly ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 6 ) ) return 0 ;
	pl_w->pl_sp_type = pl0f_sp_chouhatsu ;
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
	pl_w->pl_sp_type = pl0f_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl0f_delay_ck( PLAYER_WORK *pl_w )
{
	if( array_delay_ck( pl_w ) || cage_delay_ck( pl_w ) || flame_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static int array_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_array, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl0f_sp_array ;
	return 1 ;
}

/*==============================================================*/
static int cage_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_cage, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl0f_sp_cage ;
	return 1 ;
}

/*==============================================================*/
static int flame_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_flame, &pl_w->com_w5 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl0f_sp_flame ;
	return 1 ;
}

/****************************************************************/
/*			Counts down the flight; it ends when the time is	*/
/*			up, but not during a move							*/
/****************************************************************/
void pl0f_main( PLAYER_WORK *pl_w )
{
	PL0F_LOCAL_W *lw = PL0F_LW( pl_w ) ;

	if( pl_w->fly_flag && pl_w->routine_1 == PL_NORM && !-- lw->fly_timer ){
		if( pl_w->act_st == N_SPECIAL ) lw->fly_timer = 1 ;
		if( pl_w->act_st == N_SUPER ) lw->fly_timer = 1 ;
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
void pl0f_atck_set( PLAYER_WORK *pl_w )
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
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_CROUCH_MP ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		pl_se_req( pl_w, SE_SWING_M ) ;
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
		pl_se_req( pl_w, SE_SWING_M ) ;
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
/*			In the air, freely while flying, else only while	*/
/*			that button's jump attacks are left					*/
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
static void jump_punch( PLAYER_WORK *pl_w )
{
	PL0F_LOCAL_W *lw = PL0F_LW( pl_w ) ;

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
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_JUMP_HP ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_hp_tbl ;
		else pl_w->chain_tbl = sj_hp_tbl ;
		pl_w->nml_level = 2 ;
		pl_w->routine_2 = 2 ;
		lw->beam_stop_flag = 0 ;
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
		pl_w->fly_flag = 0 ;
		pl_w->sjump_flag = 0 ;
		pl_w->jump_atk_cnt &= JATK_P_MASK ;
		if( ( pl_w->atck_lever & KEY_DOWN ) && pl_w->pos.y > pl_w->ground_y + DOT_Y( 64.0f ) ){
			pl_char_set( pl_w, Char_Jump_K, 2 ) ;
			pl_w->atck_no = ATCK_JUMP_HK ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_w->routine_2 = 2 ;
			pl_w->routine_3 = 0 ;
		} else {
			pl_char_set( pl_w, Char_Extra, 3 ) ;
			pl_w->atck_no = pl0f_at_air_hk ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_w->routine_2 = 1 ;
			pl_w->routine_3 = 0 ;
		}
		break ;
	}
	HIT_RESET( pl_w ) ;
	if( pl_w->jump_atk_cnt & JATK_K_MASK ) pl_w->jump_atk_cnt -= 0x10 ;
}

static void nml_move( PLAYER_WORK *pl_w );
static void nml_move_dash( PLAYER_WORK *pl_w );
static void jump_move( PLAYER_WORK *pl_w );
static void sjump_move( PLAYER_WORK *pl_w );
static void stand_punch_move( PLAYER_WORK *pl_w );
static void crouch_punch_move( PLAYER_WORK *pl_w );
static void stand_kick_move( PLAYER_WORK *pl_w );
static void crouch_kick_move( PLAYER_WORK *pl_w );
static void jump_punch_move( PLAYER_WORK *pl_w );
static void jump_kick_move( PLAYER_WORK *pl_w );
static void hp_sub( PLAYER_WORK *pl_w );

/****************************************************************/
void pl0f_atck( PLAYER_WORK *pl_w )
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
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	case 2:
		hp_sub( pl_w ) ;
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
static void crouch_punch_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	case 2:
		hp_sub( pl_w ) ;
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
/*			The standing heavy kick hits a second time			*/
/*==============================================================*/
static void stand_kick_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	case 2:
		if( pl_char_move( pl_w ) < 0 ){
			pl_stand_ret( pl_w ) ;
			return ;
		}
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pl_w->atck_no = pl0f_at_stand_hk_2 ;
			HIT_RESET( pl_w ) ;
		}
		break ;
	}
}

/*==============================================================*/
static void crouch_kick_move( PLAYER_WORK *pl_w )
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

static void j_lp_move( PLAYER_WORK *pl_w );
static void j_mp_move( PLAYER_WORK *pl_w );
static void j_hp_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void jump_punch_move( PLAYER_WORK *pl_w )
{
	static void ( *jump_punch_jmp[])( PLAYER_WORK *pl_w ) = {
		j_lp_move, j_mp_move, j_hp_move,
	} ;

	jump_punch_jmp[pl_w->nml_no]( pl_w ) ;
}

/*==============================================================*/
static void j_lp_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/*==============================================================*/
static void j_mp_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/*==============================================================*/
/*			The heavy punch fires a shot, and marks when it		*/
/*			may be cancelled									*/
/*==============================================================*/
static void j_hp_move( PLAYER_WORK *pl_w )
{
	PL0F_LOCAL_W *lw = PL0F_LW( pl_w ) ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_jump_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type & 1 ){
		pl_w->cg_type &= 0xfe ;
		shl0f_04_set( pl_w, 0, 0 ) ;
	}
	if( pl_w->cg_type & 2 ){
		pl_w->cg_type &= 0xfd ;
		lw->beam_stop_flag = 1 ;
	}
}

static void j_lk_move( PLAYER_WORK *pl_w );
static void j_mk_move( PLAYER_WORK *pl_w );
static void j_hk_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void jump_kick_move( PLAYER_WORK *pl_w )
{
	static void ( *jump_kick_jmp[])( PLAYER_WORK *pl_w ) = {
		j_lk_move, j_mk_move, j_hk_move,
	} ;

	jump_kick_jmp[pl_w->nml_no]( pl_w ) ;
}

/*==============================================================*/
static void j_lk_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/*==============================================================*/
static void j_mk_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/*==============================================================*/
static void j_hk_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/*==============================================================*/
/*			The dive: down at an angle, then a bounce off		*/
/*			whatever it hits									*/
/*==============================================================*/
static void dive_init( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_3 ++ ;
		if( pl_w->routine_2 == 1 ){
			pl_w->spd.x = -DOT_X( 4.0f ) ;
			pl_w->acc.x = 0.052083332f ;
			pl_w->spd.y = -DOT_Y( 1.25f ) ;
			pl_w->acc.y = -0.03348214f ;
		} else {
			pl_w->spd.x = -DOT_X( 2.5f ) ;
			pl_w->acc.x = 0.026041666f ;
			pl_w->spd.y = -DOT_Y( 2.25f ) ;
			pl_w->acc.y = -0.06696428f ;
		}
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
	if( !pl_w->pl_dir ){
		if( pl_w->wall_flag == WALL_LEFT ){
			pl_jump_ret( pl_w ) ;
			return ;
		}
	} else if( pl_w->wall_flag == WALL_RIGHT ){
		pl_jump_ret( pl_w ) ;
		return ;
	}
	if( pl_w->hit_flag ){
		pl_w->routine_3 ++ ;
		pl_w->pos.y += DOT_Y( 16.0f ) ;
		if( pl_w->routine_2 == 1 ){
			pl_char_set( pl_w, Char_Extra, 4 ) ;
			pl_w->spd.x = DOT_X( 4.0f ) ;
			pl_w->acc.x = -DOT_X( 0.125f ) ;
			pl_w->spd.y = DOT_Y( 8.0f ) ;
			pl_w->acc.y = -DOT_Y( 0.375f ) ;
		} else {
			pl_char_set( pl_w, Char_Extra, 5 ) ;
			pl_w->spd.x = DOT_X( 2.0f ) ;
			pl_w->acc.x = -0.104166664f ;
			pl_w->spd.y = DOT_Y( 8.0f ) ;
			pl_w->acc.y = -DOT_Y( 0.375f ) ;
		}
		if( pl_w->disp_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
	}
}

/*--------------------------------------------------------------*/
static void dive_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void dive( PLAYER_WORK *pl_w )
{
	static void ( *dive_jmp[])( PLAYER_WORK *pl_w ) = {
		dive_init, dive_move, dive_end,
	} ;

	dive_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void j_hk_move( PLAYER_WORK *pl_w )
{
	static void ( *j_hk_jmp[])( PLAYER_WORK *pl_w ) = {
		j_hk_end, dive, dive,
	} ;

	j_hk_jmp[pl_w->routine_2]( pl_w ) ;
}

/****************************************************************/
/*			The launcher										*/
/****************************************************************/
void pl0f_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_gauge_add( pl_w, 5 ) ;
		if( !pl_w->atck_pk ){
			pl_w->pl_stance = STANCE_STAND ;
			pl_char_set( pl_w, Char_Extra, 6 ) ;
			pl_w->atck_no = pl0f_at_upper_p ;
			HIT_RESET( pl_w ) ;
			pl_se_req( pl_w, SE_SWING_H ) ;
		} else {
			pl_w->pl_stance = STANCE_STAND ;
			pl_char_set( pl_w, Char_Extra, 7 ) ;
			pl_w->atck_no = pl0f_at_upper_k ;
			HIT_RESET( pl_w ) ;
			pl_se_req( pl_w, SE_SWING_M ) ;
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
/*			The dash, a hop forward								*/
/****************************************************************/
void pl0f_dash( PLAYER_WORK *pl_w )
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
		pl_w->spd.x = -DOT_X( 10.0f ) ;
		pl_w->acc.x = 0.26041666f ;
		pl_w->spd.y = DOT_Y( 3.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.25f ) ;
		if( pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
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
	if( pl_w->ground_y >= pl_w->pos.y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.y = 0.0f ;
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
void pl0f_noki( PLAYER_WORK *pl_w )
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
		pl_w->spd.x = 20.0f ;
		pl_w->acc.x = -0.3125f ;
		pl_w->spd.y = DOT_Y( 3.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.25f ) ;
		if( pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
	}
}

/*--------------------------------------------------------------*/
static void noki_move( PLAYER_WORK *pl_w )
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
		pl_char_set( pl_w, Char_Dash, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void noki_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void appear_init( PLAYER_WORK *pl_w );
static void appear_wait( PLAYER_WORK *pl_w );
static void appear_end( PLAYER_WORK *pl_w );

/****************************************************************/
void pl0f_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_init, appear_wait, appear_end,
	} ;

	appear_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			One of two entrances, at random						*/
/*==============================================================*/
static void appear_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->disp_flag = 1 ;
	if( !( Rnd() & 1 ) ){
		pl_char_set( pl_w, Char_Appear, 0 ) ;
		pl_w->timer1 = 7 ;
	} else {
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Appear, 1 ) ;
		ef_color_set( pl_w, 0, pl_w->pl_color * 28, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_wait( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_1 ++ ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_1 ++ ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
	}
}

/* a table nothing reads */
static Uint8 win_tbl[8] = { 7, 7, 13, 12, 11, 10, 9, 8 } ;

/*==============================================================*/
/*			The KO win poses: a button held picks one,			*/
/*			against Abyss it is pose 1, and with a partner		*/
/*			left it is pose 2									*/
/*==============================================================*/
static void win_ko_init( PLAYER_WORK *pl_w )
{
	Uint8 no ;
	PLAYER_WORK *em ;

	no = Rnd() & 1 ;
	if( !pl_w->cpu_flag ){
		if( sw_work[pl_w->pl_no].now & KEY_LP ) no = 0 ;
		else if( sw_work[pl_w->pl_no].now & KEY_HP ) no = 1 ;
	}
	em = pl_w->target_w ;
	if( em->pl_type == PL_ABYSS1 || em->pl_type == PL_ABYSS2 || em->pl_type == PL_ABYSS3 ) no = 1 ;
	if( pl_be_cnt( pl_w ) >= 2 ) no = 2 ;
	switch( no ){
	case 0:
		pl_w->sub_type = 0 ;
		pl_char_set( pl_w, Char_Stand, 2 ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = -5.0f ;
		if( pl_w->disp_dir ) pl_w->spd.x = -pl_w->spd.x ;
		break ;
	case 1:
		pl_w->sub_type = 1 ;
		pl_w->disp_dir = 0 ;
		pl_char_set( pl_w, Char_Win, 1 ) ;
		ef_color_set( pl_w, 3, pl_w->pl_color * 28 + 3, 1 ) ;
		pls0f_00_set( pl_w, 0, 0 ) ;
		pls0f_00_set( pl_w, 0, 1 ) ;
		break ;
	default:
		pl_w->sub_type = 2 ;
		pl_char_set( pl_w, Char_Win, 0 ) ;
		pl_voice_req( pl_w, 16 ) ;
		break ;
	}
	pl_w->routine_2 ++ ;
	pl_w->routine_3 = 0 ;
}

/*==============================================================*/
/*			Pose 0: he walks a little way, then poses			*/
/*==============================================================*/
static void win0_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->routine_3 ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		if( pl_w->em_dist <= DOT_X( 64.0f ) ){
			pl_w->routine_3 ++ ;
			pl_char_set( pl_w, Char_Win, 0 ) ;
			pls0f_00_set( pl_w, 2, 0 ) ;
			pls0f_00_set( pl_w, 2, 1 ) ;
			pls0f_00_set( pl_w, 2, 2 ) ;
			pls0f_00_set( pl_w, 2, 3 ) ;
			pl_voice_req( pl_w, 16 ) ;
		}
	}
}

/*==============================================================*/
static void win1_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void win2_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_ko_move( PLAYER_WORK *pl_w )
{
	static void ( *win_ko_move_jmp[])( PLAYER_WORK *pl_w ) = {
		win0_move, win1_move, win2_move,
	} ;

	win_ko_move_jmp[pl_w->sub_type]( pl_w ) ;
}

/*==============================================================*/
static void win_ko( PLAYER_WORK *pl_w )
{
	static void ( *win_ko_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko_init, win_ko_move,
	} ;

	win_ko_jmp[pl_w->routine_2]( pl_w ) ;
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
void pl0f_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko, lose_time_ko, win_time, lose_time_ko, draw_time,
	} ;

	if( pl_win_end_ck( pl_w ) ){
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_stand_ret( pl_w ) ;
	} else win_jmp[pl_w->type]( pl_w ) ;
}

/*==============================================================*/
static void t_kick_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->spd.x = -30.0f ;
	if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = -DOT_Y( 0.125f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->atck_no = pl0f_at_t_kick ;
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
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Extra, 1 ) ;
		pl_land_ef_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void t_kick_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stop_stand_ret( pl_w ) ;
}

/****************************************************************/
/*			The partner's jump in								*/
/****************************************************************/
void pl0f_t_kick( PLAYER_WORK *pl_w )
{
	static void ( *t_kick_jmp[])( PLAYER_WORK *pl_w ) = {
		t_kick_init, t_kick_jump, t_kick_land,
	} ;

	t_kick_jmp[pl_w->routine_2]( pl_w ) ;
}

static void air_move( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw );
static void plasma_sub( PLAYER_WORK *pl_w );
static void photon_sub( PLAYER_WORK *pl_w );

/* a beam, on the ground or in the air */
static void plasma_gnd_init( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw );
static void plasma_gnd_move( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw );

/*==============================================================*/
static void plasma_gnd_init( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
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
	pl_w->atck_no = pl_w->atck_level + 50 ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 0 ) ;
	plasma_gnd_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void plasma_gnd_move( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	plasma_sub( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shl0f_03_set( pl_w, 0, 0 ) ;
		shl0f_03_set( pl_w, 1, 0 ) ;
		pl_w->cancel_timer = 16 ;
		pl_w->unk_27b = 0 ;
	}
}

/*==============================================================*/
static void plasma_gnd( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	static void ( *plasma_gnd_jmp[])( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw ) = {
		plasma_gnd_init, plasma_gnd_move,
	} ;

	plasma_gnd_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

static void plasma_air_init( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw );
static void plasma_air_move( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw );

/*==============================================================*/
static void plasma_air_init( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	pl_w->routine_3 ++ ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->spd.x /= 16.0f ;
	pl_w->spd.y /= 8.0f ;
	pl_w->acc.y /= 64.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->atck_no = pl_w->atck_level + 52 ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 1 ) ;
	pl_w->timer0 = 48 ;
	plasma_air_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void plasma_air_move( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	air_move( pl_w, lw ) ;
	pl_char_move( pl_w ) ;
	plasma_sub( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shl0f_03_set( pl_w, 0, 1 ) ;
		shl0f_03_set( pl_w, 1, 1 ) ;
		pl_w->cancel_timer = 16 ;
		pl_w->unk_27b = 0 ;
	}
	if( !-- pl_w->timer0 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Special, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void plasma_air_end( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	air_move( pl_w, lw ) ;
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void plasma_air( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	static void ( *plasma_air_jmp[])( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw ) = {
		plasma_air_init, plasma_air_move, plasma_air_end,
	} ;

	plasma_air_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*==============================================================*/
static void sp_plasma( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	static void ( *plasma_jmp[])( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw ) = {
		plasma_gnd, plasma_air,
	} ;

	if( pl_w->pl_stance == STANCE_AIR ) pl_w->routine_2 = 1 ;
	plasma_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/* a spread of shots, on the ground or in the air */
static void photon_gnd_init( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw );
static void photon_gnd_move( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw );

/*==============================================================*/
static void photon_gnd_init( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
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
	pl_w->atck_no = pl0f_at_photon_gnd ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 3 ) ;
	photon_gnd_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void photon_gnd_move( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	int i ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	photon_sub( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		for( i = 0 ; i < 10 ; i++ ) shl0f_02_set( pl_w, 0, i ) ;
	}
}

/*==============================================================*/
static void photon_gnd( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	static void ( *photon_gnd_jmp[])( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw ) = {
		photon_gnd_init, photon_gnd_move,
	} ;

	photon_gnd_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

static void photon_air_init( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw );
static void photon_air_move( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw );

/*==============================================================*/
static void photon_air_init( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	pl_w->routine_3 ++ ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->spd.x /= 16.0f ;
	pl_w->spd.y /= 8.0f ;
	pl_w->acc.y /= 64.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->atck_no = pl_w->atck_level + 48 ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 4 ) ;
	photon_air_move( pl_w, lw ) ;
}

/*==============================================================*/
/*			The first shots, then he starts to fall				*/
/*==============================================================*/
static void photon_air_move( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	int i ;

	air_move( pl_w, lw ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_jump_ret( pl_w ) ;
		return ;
	}
	photon_sub( pl_w ) ;
	if( pl_w->cg_type & 1 ){
		pl_w->cg_type &= 0xfe ;
		for( i = 10 ; i < 15 ; i++ ) shl0f_02_set( pl_w, 0, i ) ;
	}
	if( pl_w->cg_type & 2 ){
		pl_w->cg_type &= 0xfd ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
	}
}

/*--------------------------------------------------------------*/
static void photon_air( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	static void ( *photon_air_jmp[])( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw ) = {
		photon_air_init, photon_air_move,
	} ;

	photon_air_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*==============================================================*/
static void sp_photon( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	static void ( *photon_jmp[])( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw ) = {
		photon_gnd, photon_air,
	} ;

	if( pl_w->pl_stance == STANCE_AIR ) pl_w->routine_2 = 1 ;
	photon_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/* rocks gather round him, then fly off one by one */
static void shield_init( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw );
static void shield_wait( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw );

/*==============================================================*/
static void shield_init( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
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
	pl_w->atck_no = pl_w->atck_level + 54 ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 5 ) ;
	lw->shield_fly_flag = 0 ;
	shield_wait( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void shield_wait( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 36 ;
		pl_w->timer1 = 0 ;
		pl_voice_req( pl_w, 32 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shield_shot( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	if( !( pl_w->timer0 & 3 ) ){
		shl0f_05_set( pl_w, 0, pl_w->timer1 ) ;
		pl_w->timer1 ++ ;
	}
	pl_char_move( pl_w ) ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 90 ;
		lw->shield_fly_flag = 1 ;
	}
}

/*--------------------------------------------------------------*/
static void shield_hold( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	if( pl_w->timer0 == 60 ) pl_voice_req( pl_w, 33 ) ;
	pl_char_move( pl_w ) ;
	if( !-- pl_w->timer0 ) pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void shield_end( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_shield( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	static void ( *shield_jmp[])( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw ) = {
		shield_init, shield_wait, shield_shot, shield_hold, shield_end,
	} ;

	shield_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/* a hyper combo of shots, on the ground or in the air */
static void array_gnd_init( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw );
static void array_gnd_start( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw );

/*==============================================================*/
static void array_gnd_init( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_3 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->atck_no = pl0f_at_array_gnd ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 0 ) ;
	array_gnd_start( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void array_gnd_start( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_3 ++ ;
		pl_w->timer0 = 90 ;
		pl_w->timer1 = 0 ;
		lw->shot_cnt = 5 ;
		ofs.x = -DOT_X( 40.0f ) ;
		ofs.y = DOT_Y( 96.0f ) ;
		ofs.z = 0.0f ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*==============================================================*/
/*			A burst of shots every ten frames, five in all		*/
/*==============================================================*/
static void array_gnd_move( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	int i ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_w->timer0 == 90 ) pl_voice_req( pl_w, 21 ) ;
	pl_char_move( pl_w ) ;
	photon_sub( pl_w ) ;
	if( !pl_w->timer0 -- ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Super, 7 ) ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
	} else if( !pl_w->timer1 -- ){
		pl_w->timer1 = 10 ;
		if( lw->shot_cnt ){
			lw->shot_cnt -- ;
			for( i = 0 ; i < 10 ; i++ ) shl0f_02_set( pl_w, 1, i ) ;
			pl_voice_req( pl_w, 31 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void array_gnd_end( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void array_gnd( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	static void ( *array_gnd_jmp[])( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw ) = {
		array_gnd_init, array_gnd_start, array_gnd_move, array_gnd_end,
	} ;

	array_gnd_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

static void array_air_init( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw );
static void array_air_start( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw );

/*==============================================================*/
static void array_air_init( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_3 ++ ;
	pl_dir_set( pl_w ) ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl0f_at_array ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 1 ) ;
	array_air_start( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void array_air_start( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	air_move( pl_w, lw ) ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_3 ++ ;
		pl_w->timer0 = 90 ;
		pl_w->timer1 = 0 ;
		lw->shot_cnt = 5 ;
		ofs.x = -DOT_X( 26.0f ) ;
		ofs.y = DOT_Y( 44.0f ) ;
		ofs.z = 0.0f ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void array_air_move( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	int i ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_w->timer0 == 90 ) pl_voice_req( pl_w, 21 ) ;
	air_move( pl_w, lw ) ;
	pl_char_move( pl_w ) ;
	photon_sub( pl_w ) ;
	if( !pl_w->timer0 -- ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Super, 8 ) ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
	} else if( !pl_w->timer1 -- ){
		pl_w->timer1 = 10 ;
		if( lw->shot_cnt ){
			lw->shot_cnt -- ;
			for( i = 10 ; i < 15 ; i++ ) shl0f_02_set( pl_w, 1, i ) ;
			pl_voice_req( pl_w, 31 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void array_air_end( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void array_air( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	static void ( *array_air_jmp[])( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw ) = {
		array_air_init, array_air_start, array_air_move, array_air_end,
	} ;

	array_air_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*==============================================================*/
static void sp_array( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	static void ( *array_jmp[])( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw ) = {
		array_gnd, array_air,
	} ;

	if( pl_w->pl_stance == STANCE_AIR ) pl_w->routine_2 = 1 ;
	array_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/* a hyper combo on the ground */
static void cage_init( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw );
static void cage_start( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw );

/*==============================================================*/
static void cage_init( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
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
	lw->cage_end_flag = 0 ;
	lw->cage_hit_flag = 0 ;
	pl_w->atck_no = pl0f_at_cage ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 2 ) ;
	cage_start( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void cage_start( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		ofs.x = -75.0f ;
		ofs.y = DOT_Y( 85.0f ) ;
		ofs.z = 0.0f ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*==============================================================*/
/*			Waits for lw->cage_end_flag to be set				*/
/*==============================================================*/
static void cage_move( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		if( !shl0f_00_set( pl_w, 0, 0 ) ){
			pl_stand_ret( pl_w ) ;
			return ;
		}
	}
	if( !lw->cage_hit_flag && pl_w->hit_flag ){
		pl_char_set( pl_w, Char_Super, 3 ) ;
		lw->cage_hit_flag = 1 ;
	}
	if( lw->cage_end_flag ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 20 ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void cage_hold( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->timer0 -- ){
		pl_char_set( pl_w, Char_Super, 4 ) ;
		pl_w->routine_2 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void cage_end( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_cage( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	static void ( *cage_jmp[])( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw ) = {
		cage_init, cage_start, cage_move, cage_hold, cage_end,
	} ;

	cage_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/* a hyper combo on the ground, in two parts */
static void flame_init( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw );
static void flame_start( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw );

/*==============================================================*/
static void flame_init( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
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
	lw->flame_end_flag = 0 ;
	pl_w->atck_no = pl0f_at_flame_start ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 5 ) ;
	pl_w->timer0 = 48 ;
	flame_start( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void flame_start( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type & 1 ){
		pl_w->cg_type &= 0x7e ;
		pl_w->routine_2 ++ ;
		ofs.x = -DOT_X( 16.0f ) ;
		ofs.y = DOT_Y( 85.0f ) ;
		ofs.z = 0.0f ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void flame_move( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type & 2 ){
		pl_w->cg_type &= 0x7d ;
		shl0f_01_set( pl_w, 0, 0 ) ;
		pl_se_req( pl_w, 75 ) ;
	}
	if( lw->flame_end_flag ){
		pl_char_set( pl_w, Char_Super, 6 ) ;
		pl_w->routine_2 ++ ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void flame_end( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_flame( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	static void ( *flame_jmp[])( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw ) = {
		flame_init, flame_start, flame_move, flame_end,
	} ;

	flame_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

static void chouhatsu_init( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw );
static void chouhatsu_move( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw );

/*==============================================================*/
static void chouhatsu_init( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
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
	pl_char_set( pl_w, Char_Extra, 2 ) ;
	pl_voice_req( pl_w, 16 ) ;
	pl_w->timer0 = 64 ;
	chouhatsu_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void chouhatsu_move( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->timer0 -- ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	static void ( *chouhatsu_jmp[])( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw ) = {
		chouhatsu_init, chouhatsu_move,
	} ;

	chouhatsu_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

static void snapback_init( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw );
static void snapback_move( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw );

/*==============================================================*/
static void snapback_init( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
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
	pl_w->atck_no = pl0f_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 6 ) ;
	snapback_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void snapback_move( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		ofs.x = 55.0f ;
		ofs.y = DOT_Y( 74.0f ) ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
}

/*==============================================================*/
static void sp_snapback( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	static void ( *snapback_jmp[])( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw ) = {
		snapback_init, snapback_move,
	} ;

	snapback_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/* flight, which takes off or, while flying, ends it */
static void fly_move( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw );

/*==============================================================*/
static void fly_init( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
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
static void fly_move( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ){
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
static void sp_fly( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	static void ( *fly_jmp[])( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw ) = {
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

/****************************************************************/
void pl0f_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *pl_sp_tbl[])( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw ) = {
		sp_plasma, sp_photon, sp_shield, sp_array, sp_cage, sp_flame, sp_chouhatsu, sp_snapback,
		sp_fly,
	} ;
	PL0F_LOCAL_W *lw = PL0F_LW( pl_w ) ;

	pl_sp_tbl[pl_w->pl_sp_type]( pl_w, lw ) ;
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
	if( pl_w->atck_pk == 1 && pl_w->atck_level == 1 ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 3 ;
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
PLAYER_WORK *pl0f_catch_ck( PLAYER_WORK *pl_w )
{
	static PLAYER_WORK *( *catch_ck_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_catch_ck, crouch_catch_ck, jump_catch_ck, crouch_catch_ck,
	} ;

	return catch_ck_jmp[pl_w->pl_stance]( pl_w ) ;
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
	pl_char_set( pl_w, Char_Catch, 0 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	ofs.x = -DOT_X( 143.0f ) ;
	ofs.y = DOT_Y( 86.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	catch_gauge_add( pl_w ) ;
}

/*==============================================================*/
static void nml_k_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_w->dir & 1 ){
		pl_w->pl_dir ^= 1 ;
		pl_w->disp_dir = pl_w->pl_dir ;
	}
	pl_w->hit_stop = 10 ;
	pl_char_set( pl_w, Char_Catch, 2 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	ofs.x = -DOT_X( 143.0f ) ;
	ofs.y = DOT_Y( 86.0f ) ;
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
	pl_char_set( pl_w, Char_Catch, 4 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	ofs.x = -DOT_X( 143.0f ) ;
	ofs.y = DOT_Y( 69.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	catch_gauge_add( pl_w ) ;
}

/*==============================================================*/
static void jump_k_set( PLAYER_WORK *pl_w )
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
	ofs.x = -DOT_X( 143.0f ) ;
	ofs.y = DOT_Y( 69.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	catch_gauge_add( pl_w ) ;
}

/****************************************************************/
/*			Start the throw that pl0f_catch_ck chose			*/
/****************************************************************/
void pl0f_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_set, nml_k_set, jump_p_set, jump_k_set,
	} ;

	catch_set_jmp[pl_w->catch_no]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_throw( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 30 ;
		em = pl_w->catch_w ;
		em->catch_type = 1 ;
		em->pl_stance = STANCE_AIR ;
		em->atck_no = pl0f_at_throw_p ;
		pl_w->atck_no = pl0f_at_throw_p ;
		em->pl_dir = pl_w->pl_dir ;
		em->pl_dir ^= 1 ;
		pls0f_00_set( pl_w, 1, 0 ) ;
		pl_voice_req( pl_w, 34 ) ;
	}
}

/*--------------------------------------------------------------*/
static void nml_p_wait( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	plasma_sub( pl_w ) ;
	if( !pl_w->timer0 -- ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Catch, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void nml_p_end( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void nml_p_catch( PLAYER_WORK *pl_w )
{
	static void ( *nml_p_catch_jmp[])( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw ) = {
		nml_p_throw, nml_p_wait, nml_p_end,
	} ;

	nml_p_catch_jmp[pl_w->routine_2]( pl_w, PL0F_LW( pl_w ) ) ;
}

/*==============================================================*/
static void nml_k_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->catch_type = 11 ;
		em->pl_stance = STANCE_AIR ;
		em->atck_no = pl0f_at_throw_k ;
		pl_w->atck_no = pl0f_at_throw_k ;
		em->pl_dir = pl_w->pl_dir ;
		em->pl_dir ^= 1 ;
		catch_se_req( em ) ;
	}
}

/*==============================================================*/
static void jump_p_throw( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 30 ;
		em = pl_w->catch_w ;
		em->catch_type = 1 ;
		em->pl_stance = STANCE_AIR ;
		em->atck_no = pl0f_at_air_throw_p ;
		pl_w->atck_no = pl0f_at_air_throw_p ;
		em->pl_dir = pl_w->pl_dir ;
		em->pl_dir ^= 1 ;
		pls0f_00_set( pl_w, 1, 1 ) ;
		pl_voice_req( pl_w, 34 ) ;
	}
}

/*--------------------------------------------------------------*/
static void jump_p_wait( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	plasma_sub( pl_w ) ;
	if( !pl_w->timer0 -- ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Catch, 5 ) ;
	}
}

/*--------------------------------------------------------------*/
static void jump_p_end( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/*==============================================================*/
static void jump_p_catch( PLAYER_WORK *pl_w )
{
	static void ( *jump_p_catch_jmp[])( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw ) = {
		jump_p_throw, jump_p_wait, jump_p_end,
	} ;

	jump_p_catch_jmp[pl_w->routine_2]( pl_w, PL0F_LW( pl_w ) ) ;
}

/*==============================================================*/
static void jump_k_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_jump_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->catch_type = 1 ;
		em->pl_stance = STANCE_AIR ;
		em->atck_no = pl0f_at_air_throw_k ;
		pl_w->atck_no = pl0f_at_air_throw_k ;
		em->pl_dir = pl_w->pl_dir ;
		em->pl_dir ^= 1 ;
	}
}

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void pl0f_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_catch, nml_k_catch, jump_p_catch, jump_k_catch,
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
void pl0f_hold( PLAYER_WORK *pl_w )
{
	static void ( *hold_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_hold, nml_hold, nml_hold, nml_hold,
	} ;

	hold_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/* a table nothing reads */
static Uint8 unk_tbl[3][4] = {
	{ 17, 18, 16, 18 },
	{ 20, 21, 19, 21 },
	{ 23, 24, 22, 24 },
} ;

/****************************************************************/
void pl0f_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl0f_sp_array ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl0f_sp_flame ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl0f_sp_cage ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl0f_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl0f_sp_array ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl0f_sp_flame ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl0f_sp_cage ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl0f_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl0f_sp_photon ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl0f_sp_shield ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl0f_sp_plasma ;
		break ;
	}
	pl_w->atck_level = 0 ;
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void pl0f_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl0f_sp_photon ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl0f_sp_shield ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl0f_sp_plasma ;
		break ;
	}
	pl_w->atck_level = 0 ;
	act_st_set( pl_w, N_SPECIAL ) ;
}

/*==============================================================*/
/*			Falls and lands, unless flying						*/
/*==============================================================*/
static void air_move( PLAYER_WORK *pl_w, PL0F_LOCAL_W *lw )
{
	if( !pl_w->fly_flag ){
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
	if( pl_w->ground_y > pl_w->pos.y ) pl_w->pos.y = pl_w->ground_y ;
}

/*==============================================================*/
static void plasma_sub( PLAYER_WORK *pl_w )
{
}

/*==============================================================*/
static void photon_sub( PLAYER_WORK *pl_w )
{
}

/*==============================================================*/
static void hp_sub( PLAYER_WORK *pl_w )
{
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl0f.c" End of File	*/

/* Cammy (character 24): command checks, normal attacks, dashes, appearance, win and lose poses,
   special moves and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "o_sub1.h"
#include "pl24_def.h"

void pl24_main( PLAYER_WORK *pl_w );
void pl24_com_ck( PLAYER_WORK *pl_w );
void pl24_atck_set( PLAYER_WORK *pl_w );
void pl24_atck( PLAYER_WORK *pl_w );
void pl24_dash( PLAYER_WORK *pl_w );
void pl24_noki( PLAYER_WORK *pl_w );
void pl24_appear( PLAYER_WORK *pl_w );
void pl24_win( PLAYER_WORK *pl_w );
void pl24_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl24_catch_ck( PLAYER_WORK *pl_w );
void pl24_catch_set( PLAYER_WORK *pl_w );
void pl24_catch( PLAYER_WORK *pl_w );
void pl24_hold( PLAYER_WORK *pl_w );
int pl24_delay_ck( PLAYER_WORK *pl_w );
void pl24_t_kick( PLAYER_WORK *pl_w );
void pl24_upper( PLAYER_WORK *pl_w );
void pl24_sp_unison( PLAYER_WORK *pl_w );
void pl24_sp_gattai( PLAYER_WORK *pl_w );
void pl24_sp_assist( PLAYER_WORK *pl_w );
void pl24_sp_zero( PLAYER_WORK *pl_w );

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
static Uint16 com_spike[] = { 3, COM_GROUND, pl24_sp_spike, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_KICK } ;	/* Cannon Spike */
static Uint16 com_arrow[] = { 3, COM_GROUND, pl24_sp_arrow, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Spiral Arrow */
static Uint16 com_drill[] = { 3, COM_AIR, pl24_sp_drill, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Cannon Drill */
static Uint16 com_knuckle[] = { 3, COM_GROUND, pl24_sp_knuckle, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Axle Spin Knuckle */
static Uint16 com_hooligan[] = { 3, COM_GROUND, pl24_sp_hooligan, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Hooligan Combination */
static Uint16 com_revenge[] = { 5, COM_GROUND, pl24_sp_revenge, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Cannon Revenge */
static Uint16 com_strike[] = { 3, COM_AIR, pl24_sp_strike, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Cannon Strike */
static Uint16 com_smasher[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl24_sp_smasher, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Spin Drive Smasher (hyper) */
static Uint16 com_killerbee[] = { 3, COM_HYPER | COM_ALL_SW, pl24_sp_killerbee, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Killer Bee Assault (hyper) */
static Uint16 com_shaft[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl24_sp_shaft, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Reverse Shaft Breaker (hyper) */

static void ( *pl24_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl24_main,
	pl24_com_ck,
	pl24_atck_set,
	pl24_atck,
	pl24_dash,
	pl24_noki,
	pl24_appear,
	pl24_win,
	pl24_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl24_catch_ck,	/* returns a value */
	pl24_catch_set,
	pl24_catch,
	pl24_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl24_delay_ck,
	pl24_t_kick,
	pl24_upper,
	pl24_sp_unison,
	pl24_sp_gattai,
	pl24_sp_assist,
	pl24_sp_zero,
} ;

static Uint8 smasher_ck( PLAYER_WORK *pl_w );
static Uint8 killerbee_ck( PLAYER_WORK *pl_w );
static Uint8 shaft_ck( PLAYER_WORK *pl_w );
static Uint8 revenge_ck( PLAYER_WORK *pl_w );
static Uint8 spike_ck( PLAYER_WORK *pl_w );
static Uint8 arrow_ck( PLAYER_WORK *pl_w );
static Uint8 drill_ck( PLAYER_WORK *pl_w );
static Uint8 knuckle_ck( PLAYER_WORK *pl_w );
static Uint8 strike_ck( PLAYER_WORK *pl_w );
static Uint8 hooligan_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static int chouhatsu_ck( PLAYER_WORK *pl_w );
static int smasher_delay_ck( PLAYER_WORK *pl_w );
static int killerbee_delay_ck( PLAYER_WORK *pl_w );
static int shaft_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl24_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl24_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl24_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( smasher_ck( pl_w ) ) return ;
	if( killerbee_ck( pl_w ) ) return ;
	if( shaft_ck( pl_w ) ) return ;
	if( revenge_ck( pl_w ) ) return ;
	if( spike_ck( pl_w ) ) return ;
	if( arrow_ck( pl_w ) ) return ;
	if( drill_ck( pl_w ) ) return ;
	if( knuckle_ck( pl_w ) ) return ;
	if( strike_ck( pl_w ) ) return ;
	if( hooligan_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
static Uint8 spike_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_spike, &pl_w->com_w0 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl24_sp_spike ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 arrow_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_arrow, &pl_w->com_w1 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl24_sp_arrow ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			In the air, once a jump unless it is a super		*/
/*			jump												*/
/*==============================================================*/
static Uint8 drill_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_drill, &pl_w->com_w2 ) ) return 0 ;
	if( !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl24_sp_drill ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 knuckle_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_knuckle, &pl_w->com_w3 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w3 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl24_sp_knuckle ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 hooligan_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_hooligan, &pl_w->com_w4 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w4 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl24_sp_hooligan ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 revenge_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_revenge, &pl_w->com_w5 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w5 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl24_sp_revenge ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			In the air, once a jump unless it is a super		*/
/*			jump												*/
/*==============================================================*/
static Uint8 strike_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_strike, &pl_w->com_w6 ) ) return 0 ;
	if( !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w6 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl24_sp_strike ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			Hyper combos										*/
/*==============================================================*/
static Uint8 smasher_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_smasher, &pl_w->com_w7 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w7 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl24_sp_smasher ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 killerbee_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_killerbee, &pl_w->com_w8 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w8 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl24_sp_killerbee ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 shaft_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_shaft, &pl_w->com_w9 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w9 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl24_sp_shaft ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static int snapback_ck( PLAYER_WORK *pl_w )
{
	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl24_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static int chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 5 ) ) return 0 ;
	pl_w->pl_sp_type = pl24_sp_chouhatsu ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl24_delay_ck( PLAYER_WORK *pl_w )
{
	if( killerbee_delay_ck( pl_w ) || smasher_delay_ck( pl_w ) || shaft_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static int smasher_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_smasher, &pl_w->com_w7 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl24_sp_smasher ;
	return 1 ;
}

/*==============================================================*/
static int killerbee_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_killerbee, &pl_w->com_w8 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl24_sp_killerbee ;
	return 1 ;
}

/*==============================================================*/
static int shaft_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_shaft, &pl_w->com_w9 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl24_sp_shaft ;
	return 1 ;
}

/****************************************************************/
void pl24_main( PLAYER_WORK *pl_w )
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
void pl24_atck_set( PLAYER_WORK *pl_w )
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
		pl_w->char_no.b.no = 3 ;
		pl_w->atck_no = pl24_at_stand_lp ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->char_no.b.no = 4 ;
		pl_w->atck_no = pl24_at_stand_mp ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 5 ;
		pl_w->atck_no = pl24_at_stand_hp ;
		pl_se_req( pl_w, SE_SWING_M ) ;
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
/*			The heavy kick has a second version, with the		*/
/*			stick held back										*/
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
		pl_w->char_no.b.no = 4 ;
		pl_w->atck_no = pl24_at_stand_mk ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_w->chain_tbl = mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		if( pl_w->atck_lever & KEY_BACK ){
			pl_w->char_no.b.no = 5 ;
			pl_w->atck_no = pl24_at_back_hk ;
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
/*			Rising and falling use different animations			*/
/*==============================================================*/
static void jump_punch( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		if( pl_w->jump_dir < 0 ) pl_w->char_no.b.no = 0 ;
		else pl_w->char_no.b.no = 3 ;
		pl_w->atck_no = ATCK_JUMP_LP ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_lp_tbl ;
		else pl_w->chain_tbl = sj_lp_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		if( pl_w->jump_dir < 0 ) pl_w->char_no.b.no = 1 ;
		else pl_w->char_no.b.no = 4 ;
		pl_w->atck_no = ATCK_JUMP_MP ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mp_tbl ;
		else pl_w->chain_tbl = sj_mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		if( pl_w->jump_dir < 0 ) pl_w->char_no.b.no = 2 ;
		else pl_w->char_no.b.no = 5 ;
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
		if( pl_w->jump_dir < 0 ) pl_w->char_no.b.no = 0 ;
		else pl_w->char_no.b.no = 3 ;
		pl_w->atck_no = ATCK_JUMP_LK ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_lk_tbl ;
		else pl_w->chain_tbl = sj_lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		if( pl_w->jump_dir < 0 ) pl_w->char_no.b.no = 1 ;
		else pl_w->char_no.b.no = 4 ;
		pl_w->atck_no = ATCK_JUMP_MK ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mk_tbl ;
		else pl_w->chain_tbl = sj_mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		if( pl_w->jump_dir < 0 ) pl_w->char_no.b.no = 2 ;
		else pl_w->char_no.b.no = 5 ;
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
void pl24_atck( PLAYER_WORK *pl_w )
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
static void crouch_punch_move( PLAYER_WORK *pl_w )
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
void pl24_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Extra, 3 ) ;
		pl_w->atck_no = pl24_at_upper ;
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
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void dash_start( PLAYER_WORK *pl_w );
static void dash_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl24_dash( PLAYER_WORK *pl_w )
{
	static void ( *dash_jmp[])( PLAYER_WORK *pl_w ) = {
		dash_start, dash_move,
	} ;

	dash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dash_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 10.0f ) : -DOT_X( 10.0f ) ;
		pl_w->timer0 = 30 ;
	}
}

/*--------------------------------------------------------------*/
static void dash_move( PLAYER_WORK *pl_w )
{
	if( -- pl_w->timer0 > 0 ){
		pl_char_move( pl_w ) ;
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		return ;
	}
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_stand_ret( pl_w ) ;
}

static void noki_start( PLAYER_WORK *pl_w );
static void noki_move( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash										*/
/****************************************************************/
void pl24_noki( PLAYER_WORK *pl_w )
{
	static void ( *noki_jmp[])( PLAYER_WORK *pl_w ) = {
		noki_start, noki_move,
	} ;

	noki_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void noki_start( PLAYER_WORK *pl_w )
{
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->routine_2 ++ ;
	noki_move( pl_w ) ;
}

/*==============================================================*/
/*			Slow while the animation says so					*/
/*==============================================================*/
static void noki_move( PLAYER_WORK *pl_w )
{
	Float spd ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ) spd = DOT_X( 2.0f ) ;
	else spd = DOT_X( 8.0f ) ;
	pl_w->spd.x = pl_w->pl_dir ? -spd : spd ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
}

static void appear_init( PLAYER_WORK *pl_w );
static void appear_move( PLAYER_WORK *pl_w );
static void appear_color_set( PLAYER_WORK *pl_w );

/****************************************************************/
void pl24_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_init, appear_move,
	} ;

	appear_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void appear_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->disp_flag = 1 ;
	appear_color_set( pl_w ) ;
	pl_char_set( pl_w, Char_Appear, 0 ) ;
}

/*--------------------------------------------------------------*/
static void appear_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_w->routine_1 ++ ;
	else if( pl_w->cg_type < 0 ){
		pl_w->cg_type = 0 ;
		pls24_00_set( pl_w, 0 ) ;
	}
}

static void win_ko( PLAYER_WORK *pl_w );
static void win_ko_init( PLAYER_WORK *pl_w );
static void win_e_set( PLAYER_WORK *pl_w );
static void win_ko_move( PLAYER_WORK *pl_w );
static void win_ko_move_e( PLAYER_WORK *pl_w );
static void win_time( PLAYER_WORK *pl_w );
static void win_time_init( PLAYER_WORK *pl_w );
static void lose_time_ko( PLAYER_WORK *pl_w );
static void draw_time( PLAYER_WORK *pl_w );

/****************************************************************/
void pl24_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko, lose_time_ko, win_time, lose_time_ko, draw_time,
	} ;

	if( pl_win_end_ck( pl_w ) ) pl_stand_ret( pl_w ) ;
	else win_jmp[pl_w->type]( pl_w ) ;
}

/*==============================================================*/
static void win_ko( PLAYER_WORK *pl_w )
{
	static void ( *win_ko_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko_init, win_ko_move, win_ko_move_e,
	} ;

	win_ko_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			One of four poses; against Bison the				*/
/*			fourth has its own, and with Bison on				*/
/*			neither team she may turn to the stage's middle		*/
/*			and pose there										*/
/*==============================================================*/
static void win_ko_init( PLAYER_WORK *pl_w )
{
	Uint8 pl_no ;

	pl_w->routine_2 ++ ;
	pl_w->timer1 = 0 ;
	pl_w->timer0 = Rnd() & 3 ;
	if( pl_be_cnt( pl_w ) >= 2 ) pl_w->timer0 = 0 ;
	switch( pl_w->timer0 ){
	case 0:
		pl_char_set( pl_w, Char_Win, CH24_WIN_KO0 ) ;
		break ;
	case 1:
		pl_char_set( pl_w, Char_Win, CH24_WIN_KO1 ) ;
		break ;
	case 2:
		pl_char_set( pl_w, Char_Win, CH24_WIN_KO2 ) ;
		break ;
	case 3:
		if( pl_w->target_w->pl_type == PL_BISON ){
			pl_dir_set( pl_w ) ;
			pl_char_set( pl_w, Char_Win, CH24_WIN_KO_E ) ;
			break ;
		} else {
			pl_no = pl_w->pl_no ;
			if( vs_sys_work.team_w[pl_no][1]->pl_type != PL_BISON && vs_sys_work.team_w[pl_no][2]->pl_type != PL_BISON
			 && vs_sys_work.team_w[pl_no ^ 1][1]->pl_type != PL_BISON && vs_sys_work.team_w[pl_no ^ 1][2]->pl_type != PL_BISON ){
				if( vs_sys_work.vs_end_flag && pls24_00_set( pl_w, 1 ) ){
					pl_w->routine_2 = 2 ;
					win_e_set( pl_w ) ;
					return ;
				}
			}
			pl_char_set( pl_w, Char_Win, CH24_WIN_KO0 ) ;
		}
		break ;
	}
}

/*==============================================================*/
static void win_e_set( PLAYER_WORK *pl_w )
{
	PL24_LOCAL_W *lw = ( PL24_LOCAL_W * )pl_w->tlocal ;

	pl_w->timer1 = 60 ;
	lw->win_flag = 0 ;
	if( pl_w->pos.x >= scr_work.eye[0].x ) pl_w->pl_dir = 0 ;
	else pl_w->pl_dir = 1 ;
	pl_w->disp_dir = pl_w->pl_dir ;
	pl_char_set( pl_w, Char_Stand, 0 ) ;
}

/*--------------------------------------------------------------*/
static void win_ko_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_ko_move_e( PLAYER_WORK *pl_w )
{
	PL24_LOCAL_W *lw ;

	pl_w->win_wait = 0xff ;
	lw = ( PL24_LOCAL_W * )pl_w->tlocal ;
	if( pl_w->timer1 && !-- pl_w->timer1 && lw->win_flag ){
		pl_w->char_no.b.no = !pl_w->type ? CH24_WIN_KO_E : CH24_WIN_TIME_E ;
		pl_char_set( pl_w, Char_Win, pl_w->char_no.b.no ) ;
	} else pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void win_time( PLAYER_WORK *pl_w )
{
	static void ( *win_time_jmp[])( PLAYER_WORK *pl_w ) = {
		win_time_init, win_ko_move, win_ko_move_e,
	} ;

	win_time_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void win_time_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->timer1 = 0 ;
	if( pl_w->target_w->pl_type == PL_BISON ){
		pl_dir_set( pl_w ) ;
		pl_char_set( pl_w, Char_Win, CH24_WIN_TIME_E ) ;
	} else if( vs_sys_work.vs_end_flag && pls24_00_set( pl_w, 1 ) ){
		pl_w->routine_2 = 2 ;
		win_e_set( pl_w ) ;
	} else pl_char_set( pl_w, Char_Win, CH24_WIN_KO0 ) ;
}

/*==============================================================*/
static void lose_time_ko( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, CH24_LOSE ) ;
	} else pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void draw_time( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, CH24_TM_DRAW ) ;
	} else pl_char_move( pl_w ) ;
}

static void sp_spike( PLAYER_WORK *pl_w );
static void sp_arrow( PLAYER_WORK *pl_w );
static void sp_knuckle( PLAYER_WORK *pl_w );
static void sp_hooligan( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_revenge( PLAYER_WORK *pl_w );
static void sp_smasher( PLAYER_WORK *pl_w );
static void sp_strike( PLAYER_WORK *pl_w );
static void sp_drill( PLAYER_WORK *pl_w );
static void sp_killerbee( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );
static void sp_shaft( PLAYER_WORK *pl_w );

/****************************************************************/
void pl24_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *pl_sp_tbl[])( PLAYER_WORK *pl_w ) = {
		sp_spike, sp_arrow, sp_knuckle, sp_hooligan, sp_spike, sp_chouhatsu, sp_revenge, sp_smasher,
		sp_strike, sp_drill, sp_spike, sp_killerbee, sp_spike, sp_snapback, sp_shaft,
	} ;

	pl_sp_tbl[pl_w->pl_sp_type]( pl_w ) ;
}

static void spike_init( PLAYER_WORK *pl_w );
static void spike_start( PLAYER_WORK *pl_w );
static void spike_up( PLAYER_WORK *pl_w );
static void spike_fall( PLAYER_WORK *pl_w );
static void spike_land( PLAYER_WORK *pl_w );
static void spike_hit( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A rising kick										*/
/*==============================================================*/
static void sp_spike( PLAYER_WORK *pl_w )
{
	static void ( *spike_jmp[])( PLAYER_WORK *pl_w ) = {
		spike_init, spike_start, spike_up, spike_fall, spike_land,
	} ;

	spike_jmp[pl_w->routine_2]( pl_w ) ;
}

/* by atck_level: speed x and its change, speed y */
static Float spike_spd_tbl[2][3] = {
	{ 20.0f, -DOT_X( 1.25f ), DOT_Y( 11.0f ) },
	{ DOT_X( 20.0f ), -DOT_X( 1.25f ), DOT_Y( 20.0f ) },
} ;

/*==============================================================*/
static void spike_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl_w->atck_level ? 50 : 48 ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 10 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_voice_req( pl_w, 23 ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->char_no.b.no = pl_w->atck_level ? 2 : 0U ;
	pl_char_set( pl_w, Char_Special, pl_w->char_no.b.no ) ;
}

/*--------------------------------------------------------------*/
static void spike_start( PLAYER_WORK *pl_w )
{
	if( pl_w->hit_flag ){
		pl_w->routine_2 ++ ;
		spike_hit( pl_w ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_air_set( pl_w ) ;
		pl_w->spd.x = pl_w->pl_dir ? spike_spd_tbl[pl_w->atck_level][0] : -spike_spd_tbl[pl_w->atck_level][0] ;
		pl_w->acc.x = pl_w->pl_dir ? spike_spd_tbl[pl_w->atck_level][1] : -spike_spd_tbl[pl_w->atck_level][1] ;
		pl_w->spd.y = spike_spd_tbl[pl_w->atck_level][2] ;
		pl_w->acc.y = -DOT_Y( 0.625f ) ;
	}
}

/*--------------------------------------------------------------*/
static void spike_up( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( ( !pl_w->pl_dir && pl_w->spd.x > 0.0f ) || ( pl_w->pl_dir && pl_w->spd.x < 0.0f ) ){
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
	}
	if( pl_w->hit_flag ){
		spike_hit( pl_w ) ;
		return ;
	}
	if( pl_w->spd.y <= 0.0f ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_char_set( pl_w, Char_Special, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void spike_fall( PLAYER_WORK *pl_w )
{
	Float spd ;

	spd = pl_w->spd.x ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( ( spd > 0.0f && pl_w->spd.x < 0.0f ) || ( spd < 0.0f && pl_w->spd.x > 0.0f ) ){
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
	}
	pl_char_move( pl_w ) ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 5 ) ;
	}
}

/*--------------------------------------------------------------*/
static void spike_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
/*			The kick hit: she drops back						*/
/*==============================================================*/
static void spike_hit( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.x = pl_w->pl_dir ? -2.5f : 2.5f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 8.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_char_set( pl_w, Char_Special, 4 ) ;
}

static void arrow_init( PLAYER_WORK *pl_w );
static void arrow_start( PLAYER_WORK *pl_w );
static void arrow_move( PLAYER_WORK *pl_w );
static void arrow_stop( PLAYER_WORK *pl_w );
static void arrow_end( PLAYER_WORK *pl_w );
static void arrow_jump( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A low drilling kick along the ground				*/
/*==============================================================*/
static void sp_arrow( PLAYER_WORK *pl_w )
{
	static void ( *arrow_jmp[])( PLAYER_WORK *pl_w ) = {
		arrow_init, arrow_start, arrow_move, arrow_stop, arrow_end,
	} ;

	arrow_jmp[pl_w->routine_2]( pl_w ) ;
}

/* by atck_level: the slowing on landing */
static Float arrow_acc_tbl[2] = { -1.5625f, -2.5f } ;

/* by atck_level: speed x, and the fall */
static Float arrow_spd_tbl[2][2] = {
	{ DOT_X( 8.0f ), -DOT_Y( 0.25f ) },
	{ 20.0f, -0.13392857f },
} ;

/*==============================================================*/
static void arrow_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl_w->atck_level ? 53 : 51 ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 10 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_voice_req( pl_w, 22 ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 6 ) ;
}

/*--------------------------------------------------------------*/
static void arrow_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ) arrow_jump( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void arrow_move( PLAYER_WORK *pl_w )
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
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->acc.x = pl_w->pl_dir ? arrow_acc_tbl[pl_w->atck_level] : -arrow_acc_tbl[pl_w->atck_level] ;
		pl_char_set( pl_w, Char_Special, 7 ) ;
	}
}

/*--------------------------------------------------------------*/
static void arrow_stop( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( ( !pl_w->pl_dir && pl_w->spd.x < 0.0f ) || ( pl_w->pl_dir && pl_w->spd.x > 0.0f ) ) return ;
	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
}

/*--------------------------------------------------------------*/
static void arrow_end( PLAYER_WORK *pl_w )
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
/*			She leaves the ground for the kick					*/
/*==============================================================*/
static void arrow_jump( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->pos.y += DOT_Y( 16.0f ) ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->spd.x = pl_w->pl_dir ? arrow_spd_tbl[pl_w->atck_level][0] : -arrow_spd_tbl[pl_w->atck_level][0] ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = arrow_spd_tbl[pl_w->atck_level][1] ;
}

static void knuckle_init( PLAYER_WORK *pl_w );
static void knuckle_start( PLAYER_WORK *pl_w );
static void knuckle_jump( PLAYER_WORK *pl_w );
static void knuckle_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A short hop forward that hits again as the			*/
/*			animation says										*/
/*==============================================================*/
static void sp_knuckle( PLAYER_WORK *pl_w )
{
	static void ( *knuckle_jmp[])( PLAYER_WORK *pl_w ) = {
		knuckle_init, knuckle_start, knuckle_jump, knuckle_land,
	} ;

	knuckle_jmp[pl_w->routine_2]( pl_w ) ;
}

/* by atck_level: speed x, speed y */
static Float knuckle_spd_tbl[2][2] = {
	{ DOT_X( 4.0f ), DOT_Y( 2.0f ) },
	{ 10.0f, DOT_Y( 3.0f ) },
} ;

/*==============================================================*/
static void knuckle_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl_w->atck_level ? 56 : 54 ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 10 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->char_no.b.no = pl_w->atck_level ? 10 : 8 ;
	pl_char_set( pl_w, Char_Special, pl_w->char_no.b.no ) ;
}

/*--------------------------------------------------------------*/
static void knuckle_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_air_set( pl_w ) ;
		pl_w->spd.x = pl_w->pl_dir ? knuckle_spd_tbl[pl_w->atck_level][0] : -knuckle_spd_tbl[pl_w->atck_level][0] ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = knuckle_spd_tbl[pl_w->atck_level][1] ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
	}
}

/*--------------------------------------------------------------*/
static void knuckle_jump( PLAYER_WORK *pl_w )
{
	if( !pl_w->cg_type ) pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_ef_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void knuckle_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->atck_no = pl_w->atck_level ? 59 : 57 ;
		HIT_RESET( pl_w ) ;
	}
}

static void hooligan_init( PLAYER_WORK *pl_w );
static void hooligan_start( PLAYER_WORK *pl_w );
static void hooligan_jump( PLAYER_WORK *pl_w );
static void hooligan_stop( PLAYER_WORK *pl_w );
static void hooligan_end( PLAYER_WORK *pl_w );
static void hooligan_miss( PLAYER_WORK *pl_w );
static void hooligan_kick_start( PLAYER_WORK *pl_w );
static void hooligan_kick_fall( PLAYER_WORK *pl_w );
static void hooligan_kick_land( PLAYER_WORK *pl_w );
static void hooligan_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A jump that throws or kicks down from the top of	*/
/*			its arc												*/
/*==============================================================*/
static void sp_hooligan( PLAYER_WORK *pl_w )
{
	static void ( *hooligan_jmp[])( PLAYER_WORK *pl_w ) = {
		hooligan_init, hooligan_start, hooligan_jump, hooligan_stop, hooligan_end, hooligan_miss, hooligan_kick_start, hooligan_kick_fall,
		hooligan_kick_land,
	} ;

	hooligan_jmp[pl_w->routine_2]( pl_w ) ;
}

/* by atck_level: speed x, speed y */
static Float hooligan_spd_tbl[2][2] = {
	{ DOT_X( 2.0f ), DOT_Y( 9.0f ) },
	{ 10.0f, 15.0f },
} ;

/* by atck_level: the slowing after landing */
static Float hooligan_acc_tbl[2] = { -DOT_X( 0.125f ), -DOT_X( 0.375f ) } ;

/*==============================================================*/
static void hooligan_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl_w->atck_level ? 63 : 61 ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 10 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 11 ) ;
}

/*--------------------------------------------------------------*/
static void hooligan_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_air_set( pl_w ) ;
		pl_w->spd.x = pl_w->pl_dir ? hooligan_spd_tbl[pl_w->atck_level][0] : -hooligan_spd_tbl[pl_w->atck_level][0] ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = hooligan_spd_tbl[pl_w->atck_level][1] ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
	}
}

/*--------------------------------------------------------------*/
static void hooligan_jump( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	Float dy ;

	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	dy = DOT_Y( 48.0f ) ;
	if( !pl_w->routine_3 ){
		if( !pl_w->cg_type ){
			hooligan_land( pl_w ) ;
			return ;
		}
		pl_w->routine_3 ++ ;
		pl_w->pos.y += dy ;
	}
	em = pl_w->target_w ;
	if( em->pl_stance == STANCE_DOWN ){
		hooligan_land( pl_w ) ;
		return ;
	}
	if( pl_w->cpu_flag ){
		if( pl_w->atck_pk ){
			hooligan_land( pl_w ) ;
			return ;
		}
	} else if( pl_w->pl_sw_now & KEY_KICK ){
		pl_w->routine_2 = 6 ;
		pl_w->atck_no = pl24_at_hooligan_kick ;
		HIT_RESET( pl_w ) ;
		pl_w->pos.y -= DOT_Y( 40.0f ) ;
		if( pl_w->pos.y <= pl_w->ground_y ) pl_w->pos.y = pl_w->ground_y ;
		pl_char_set( pl_w, Char_Special, 29 ) ;
		return ;
	} else if( !( pl_w->pl_sw_now & KEY_PUNCH ) ){
		hooligan_land( pl_w ) ;
		return ;
	}
	if( ( em = pl_catch_ck2( pl_w ) ) ){
		if( em->pl_stance == STANCE_CROUCH ) pl_w->catch_no = 0xc4 ;
		else if( em->pl_stance == STANCE_AIR ) pl_w->catch_no = 0xc3 ;
		else if( pl_w->pos.y > em->size_top * em->scale.y * 2.142857f / 2.0f + em->pos.y ) pl_w->catch_no = 0xc3 ;
		else pl_w->catch_no = 0xc4 ;
		pl_catch_set( pl_w, em ) ;
		return ;
	}
	if( pl_w->cpu_flag ){
		hooligan_land( pl_w ) ;
		return ;
	}
	pl_w->routine_2 = 5 ;
	pl_w->pos.y -= dy ;
	pl_char_set( pl_w, Char_Special, 12 ) ;
}

/*==============================================================*/
static void hooligan_land( PLAYER_WORK *pl_w )
{
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->acc.x = pl_w->pl_dir ? hooligan_acc_tbl[pl_w->atck_level] : -hooligan_acc_tbl[pl_w->atck_level] ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 13 ) ;
	}
}

/*--------------------------------------------------------------*/
static void hooligan_stop( PLAYER_WORK *pl_w )
{
	if( !pl_w->cg_type ) pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( ( !pl_w->pl_dir && pl_w->spd.x < 0.0f ) || ( pl_w->pl_dir && pl_w->spd.x > 0.0f ) ) return ;
	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
}

/*--------------------------------------------------------------*/
static void hooligan_end( PLAYER_WORK *pl_w )
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
/*			Missed the throw: fall								*/
/*==============================================================*/
static void hooligan_miss( PLAYER_WORK *pl_w )
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
		pl_char_set( pl_w, Char_Jump, 5 ) ;
	}
}

/*==============================================================*/
/*			The kick down										*/
/*==============================================================*/
static void hooligan_kick_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = pl_w->pl_dir ? 10.0f : -10.0f ;
		pl_w->acc.x = pl_w->pl_dir ? -0.0f : 0.0f ;
		pl_w->spd.y = -DOT_Y( 9.0f ) ;
		pl_w->acc.y = 0.0f ;
	}
}

/*--------------------------------------------------------------*/
static void hooligan_kick_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( ( !pl_w->pl_dir && pl_w->spd.x > 0.0f ) || ( pl_w->pl_dir && pl_w->spd.x < 0.0f ) ){
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
	}
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 30 ) ;
	}
}

/*--------------------------------------------------------------*/
static void hooligan_kick_land( PLAYER_WORK *pl_w )
{
	spike_land( pl_w ) ;
}

static void revenge_init( PLAYER_WORK *pl_w );
static void revenge_start( PLAYER_WORK *pl_w );
static void revenge_counter( PLAYER_WORK *pl_w );
static void revenge_end( PLAYER_WORK *pl_w );
static void revenge_spike( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A flashing charge that ends in a kick, or at its	*/
/*			higher level in Cannon Spike's rise					*/
/*==============================================================*/
static void sp_revenge( PLAYER_WORK *pl_w )
{
	static void ( *revenge_jmp[])( PLAYER_WORK *pl_w ) = {
		revenge_init, revenge_start, revenge_counter, revenge_end, revenge_spike, spike_up, spike_fall, spike_land,
	} ;

	revenge_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void revenge_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_gauge_add( pl_w, 10 ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level ? 32 : 14 ) ;
}

/*--------------------------------------------------------------*/
static void revenge_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->dm_attr = ATR_COUNTER ;
		pl_w->timer0 = 1 ;
		pl_w->timer1 = 8 ;
	}
}

/*--------------------------------------------------------------*/
static void revenge_counter( PLAYER_WORK *pl_w )
{
	int col ;

	if( !-- pl_w->timer0 ){
		pl_w->timer0 = 1 ;
		col = pl_w->pl_color * 9 + pl_w->timer1 ;
		ef_color_set( pl_w, 0, col, 1 ) ;
		if( -- pl_w->timer1 < 0 ) pl_w->timer1 = 8 ;
	}
	pl_char_move( pl_w ) ;
	if( pl_w->dm_attr & ATR_COUNTER ){
		if( !pl_w->cg_type ){
			pl_w->routine_2 ++ ;
			pl_w->dm_attr = 0 ;
			pl_color_set( pl_w, PC_DEFAULT ) ;
		}
		return ;
	}
	pl_color_set( pl_w, PC_DEFAULT ) ;
	if( pl_w->atck_level ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_w->atck_no = pl24_at_revenge_spike ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 31 ) ;
	} else {
		pl_w->routine_2 += 2 ;
		pl_w->atck_no = pl24_at_revenge ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 23 ) ;
	}
}

/*--------------------------------------------------------------*/
static void revenge_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void revenge_spike( PLAYER_WORK *pl_w )
{
	if( pl_w->hit_flag ){
		pl_w->routine_2 ++ ;
		spike_hit( pl_w ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_air_set( pl_w ) ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 20.0f ) : -DOT_X( 20.0f ) ;
		pl_w->acc.x = pl_w->pl_dir ? -DOT_X( 1.25f ) : DOT_X( 1.25f ) ;
		pl_w->spd.y = DOT_Y( 20.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.625f ) ;
	}
}

static void smasher_init( PLAYER_WORK *pl_w );
static void smasher_wait( PLAYER_WORK *pl_w );
static void smasher_start( PLAYER_WORK *pl_w );
static void smasher_move( PLAYER_WORK *pl_w );
static void smasher_up( PLAYER_WORK *pl_w );
static void smasher_air( PLAYER_WORK *pl_w );
static void smasher_fall( PLAYER_WORK *pl_w );
static void smasher_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo: a dash, a hit that lifts, and a		*/
/*			rising kick											*/
/*==============================================================*/
static void sp_smasher( PLAYER_WORK *pl_w )
{
	static void ( *smasher_jmp[])( PLAYER_WORK *pl_w ) = {
		smasher_init, smasher_wait, smasher_start, smasher_move, smasher_up, smasher_air, smasher_fall, smasher_land,
	} ;

	smasher_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void smasher_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl24_at_smasher_1 ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_sp_ef_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_char_set( pl_w, Char_Special, 15 ) ;
}

/*--------------------------------------------------------------*/
static void smasher_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = -DOT_X( 5.0f ) ;
		ofs.y = DOT_Y( 20.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void smasher_start( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->pos.y += DOT_Y( 16.0f ) ;
		pl_w->spd.x = pl_w->pl_dir ? 20.0f : -20.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = -0.13392857f ;
	}
}

/*--------------------------------------------------------------*/
static void smasher_move( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_flag1 ){
		pl_w->cg_flag1 = 0 ;
		pl_w->atck_no = pl24_at_smasher_1 ;
		HIT_RESET( pl_w ) ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->atck_no = pl24_at_smasher_2 ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 16 ) ;
	}
}

/*--------------------------------------------------------------*/
static void smasher_up( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 20.0f ) : -DOT_X( 20.0f ) ;
		pl_w->acc.x = pl_w->pl_dir ? -DOT_X( 1.25f ) : DOT_X( 1.25f ) ;
		pl_w->spd.y = DOT_Y( 20.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.625f ) ;
	}
}

/*--------------------------------------------------------------*/
static void smasher_air( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_flag1 ){
		pl_w->atck_no = pl24_at_smasher_2 ;
		HIT_RESET( pl_w ) ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( ( !pl_w->pl_dir && pl_w->spd.x > 0.0f ) || ( pl_w->pl_dir && pl_w->spd.x < 0.0f ) ){
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
	}
	if( pl_w->spd.y <= 0.0f ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_char_set( pl_w, Char_Special, 17 ) ;
	}
}

/*--------------------------------------------------------------*/
static void smasher_fall( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( ( !pl_w->pl_dir && pl_w->spd.x > 0.0f ) || ( pl_w->pl_dir && pl_w->spd.x < 0.0f ) ){
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
	}
	pl_char_move( pl_w ) ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 18 ) ;
	}
}

/*--------------------------------------------------------------*/
static void smasher_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void strike_init( PLAYER_WORK *pl_w );
static void strike_start( PLAYER_WORK *pl_w );
static void strike_fall( PLAYER_WORK *pl_w );
static void strike_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A dive kick down from the air						*/
/*==============================================================*/
static void sp_strike( PLAYER_WORK *pl_w )
{
	static void ( *strike_jmp[])( PLAYER_WORK *pl_w ) = {
		strike_init, strike_start, strike_fall, strike_land,
	} ;

	strike_jmp[pl_w->routine_2]( pl_w ) ;
}

/* by atck_level: speed x, acceleration x, speed y, acceleration y */
static Float strike_spd_tbl[2][4] = {
	{ 10.0f, -0.0f, -DOT_Y( 9.0f ), 0.0f },
	{ DOT_X( 10.0f ), -0.0f, -DOT_Y( 12.0f ), 0.0f },
} ;

/*==============================================================*/
static void strike_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl_w->atck_level ? 73 : 71 ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 10 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->char_no.b.no = pl_w->atck_level ? 26 : 24 ;
	pl_char_set( pl_w, Char_Special, pl_w->char_no.b.no ) ;
}

/*--------------------------------------------------------------*/
static void strike_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = pl_w->pl_dir ? strike_spd_tbl[pl_w->atck_level][0] : -strike_spd_tbl[pl_w->atck_level][0] ;
		pl_w->acc.x = pl_w->pl_dir ? strike_spd_tbl[pl_w->atck_level][1] : -strike_spd_tbl[pl_w->atck_level][1] ;
		pl_w->spd.y = strike_spd_tbl[pl_w->atck_level][2] ;
		pl_w->acc.y = strike_spd_tbl[pl_w->atck_level][3] ;
	}
}

/*--------------------------------------------------------------*/
static void strike_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( ( !pl_w->pl_dir && pl_w->spd.x > 0.0f ) || ( pl_w->pl_dir && pl_w->spd.x < 0.0f ) ){
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
	}
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 27 ) ;
	}
}

/*--------------------------------------------------------------*/
static void strike_land( PLAYER_WORK *pl_w )
{
	spike_land( pl_w ) ;
}

static void drill_init( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw );
static void drill_start( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw );
static void drill_move( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw );
static void drill_fall( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw );
static void drill_land( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw );

/*==============================================================*/
/*			Cannon Drill: Spiral Arrow's kick in the air, down	*/
/*			to the height it started at, then a fall			*/
/*==============================================================*/
static void sp_drill( PLAYER_WORK *pl_w )
{
	static void ( *drill_jmp[])( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw ) = {
		drill_init, drill_start, drill_move, drill_fall, drill_land,
	} ;

	drill_jmp[pl_w->routine_2]( pl_w, ( PL24_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void drill_init( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl_w->atck_level ? 69 : 67 ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 10 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	lw->start_y = pl_w->pos.y ;
	pl_voice_req( pl_w, 22 ) ;
	pl_char_set( pl_w, Char_Special, 21 ) ;
}

/*--------------------------------------------------------------*/
static void drill_start( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ) arrow_jump( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void drill_move( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y > lw->start_y ){
		if( pl_w->pos.y > pl_w->ground_y ) return ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
	}
	pl_w->routine_2 ++ ;
	pl_w->spd.x /= 2.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_char_set( pl_w, Char_Special, 22 ) ;
}

/*--------------------------------------------------------------*/
static void drill_fall( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	pl_jump_move( pl_w ) ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Jump, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void drill_land( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void killerbee_init( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw );
static void killerbee_wait( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw );
static void killerbee_jump( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw );
static void killerbee_dive_init( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw );
static void killerbee_dive( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw );
static void killerbee_catch( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw );
static void killerbee_keep( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw );
static void killerbee_up( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw );
static void killerbee_drop( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw );
static void killerbee_throw( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw );
static void killerbee_hop( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw );
static void killerbee_end( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw );
static void killerbee_miss( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw );
static void killerbee_miss_end( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw );
static void killerbee_wall( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw );
static void killerbee_dir_ck( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw );
static void killerbee_atck_set( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw );
static void killerbee_fall( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw );
static Uint8 killerbee_ceil_ck( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw );
static Uint8 killerbee_floor_ck( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw );
static Uint8 killerbee_left_ck( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw );
static Uint8 killerbee_right_ck( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw );
static void killerbee_ofs_set( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw );

/*==============================================================*/
/*			A hyper combo: dives at the opponent from wall		*/
/*			to wall, then a throw								*/
/*==============================================================*/
static void sp_killerbee( PLAYER_WORK *pl_w )
{
	static void ( *killerbee_jmp[])( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw ) = {
		killerbee_init, killerbee_wait, killerbee_jump, killerbee_dive_init, killerbee_dive, killerbee_catch, killerbee_keep, killerbee_up,
		killerbee_drop, killerbee_throw, killerbee_hop, killerbee_end, killerbee_miss, killerbee_miss_end,
	} ;

	killerbee_jmp[pl_w->routine_2]( pl_w, ( PL24_LOCAL_W * )pl_w->tlocal ) ;
}

/* by direction: the animation and the attack */
static Sint16 killerbee_char_tbl[17][2] = {
	{ 3, 74 }, { 4, 75 }, { 4, 75 }, { 5, 76 }, { 5, 76 }, { 6, 77 }, { 6, 77 }, { 6, 77 },
	{ 7, 78 }, { 8, 79 }, { 8, 79 }, { 8, 79 }, { 9, 80 }, { 9, 80 }, { 10, 81 }, { 10, 81 },
	{ 11, 82 },
} ;

/*==============================================================*/
static void killerbee_init( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl24_at_killerbee ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	if( pl_w->pl_stance != STANCE_AIR ){
		pl_sp_ef_set( pl_w ) ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->pos.y = pl_w->ground_y ;
	}
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	lw->hit_cnt = 5 ;
	pl_w->timer0 = 32 ;
	pl_w->timer1 = 0 ;
	if( pl_w->pl_no ) scr_excep_set( pl_w, 1, 4 ) ;
	else scr_excep_set( pl_w, 1, 3 ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_char_set( pl_w, Char_Super, 12 ) ;
}

/*--------------------------------------------------------------*/
static void killerbee_wait( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw )
{
	Float x ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_ctr ++ ;
		x = pl_w->pl_dir ? scr_work.eye[0].x + -320.0f : scr_work.eye[0].x + 320.0f ;
		pl_w->spd.x = ( x - pl_w->pos.x ) / 32.0f ;
		x = scr_work.center_y + DOT_Y( 64.0f ) ;
		pl_w->spd.y = ( x - pl_w->pos.y ) / 32.0f ;
	}
}

/*--------------------------------------------------------------*/
static void killerbee_jump( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->spd.y < 0.0f ){
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
	if( -- pl_w->timer0 > 0 ){
		if( pl_w->timer0 == 28 ) scr_excep_set( pl_w, 1, 1 ) ;
		return ;
	}
	pl_w->routine_2 ++ ;
	pl_w->stop_move_flag = 0 ;
	pl_w->stop_move_timer = 0 ;
	ofs.x = DOT_X( 23.0f ) ;
	ofs.y = DOT_Y( 32.0f ) ;
	ofs.z = 0.0f ;
	pl_hyper_start( pl_w, &ofs, 1 ) ;
	pl_voice_req( pl_w, 11 ) ;
	pl_char_set( pl_w, Char_Super, 2 ) ;
}

/*--------------------------------------------------------------*/
static void killerbee_dive_init( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw )
{
	Uint8 no ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->routine_2 ++ ;
	pl_char_move( pl_w ) ;
	scr_excep_set( pl_w, 1, 8 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->dir = 0xff ;
	killerbee_dir_ck( pl_w, lw ) ;
	no = pl_w->pl_dir ? 5 : 21 ;
	pl_w->dir -= no ;
	if( ( Sint8 )pl_w->dir < 0 ) pl_w->dir = 0 ;
	else if( pl_w->dir > 6 ) pl_w->dir = 6 ;
	no += pl_w->dir ;
	if( !pl_w->cpu_flag ){
		if( pl_w->pl_new_lvbt & ( KEY_UP | KEY_DOWN ) ) no = pl_w->pl_dir ? 11 : 21 ;
	}
	pl_w->dir = no ;
	killerbee_atck_set( pl_w, lw ) ;
	pl_w->atck_no = pl24_at_killerbee_dive ;
	HIT_RESET( pl_w ) ;
	killerbee_dive( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void killerbee_dive( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->push_ng_timer = 1 ;
	if( !pl_w->routine_3 ){
		if( !pl_w->timer1 && pl_w->hit_flag ){
			if( !pl_sp_catch_ck( pl_w ) ){
				pl_w->delay_flag = 0 ;
				pl_w->delay_timer = 0 ;
				pl_w->hyper_bg = 0 ;
				pl_w->hyper_bg_tm = 0 ;
				killerbee_fall( pl_w, lw ) ;
				return ;
			}
			pl_w->timer1 = 1 ;
			lw->hit_cnt -- ;
		}
		if( !lw->hit_cnt ){
			pl_w->delay_flag = 0 ;
			pl_w->delay_timer = 0 ;
			pl_w->hyper_bg = 0 ;
			pl_w->hyper_bg_tm = 0 ;
			pl_w->routine_2 ++ ;
			pl_w->catch_no = 0xc6 ;
			pl_w->atck_no = pl24_at_killerbee_last ;
			HIT_RESET( pl_w ) ;
			pl_sp_catch_set( pl_w, pl_w->em_w ) ;
			pl_w->catch_w->pl_dir = pl_w->catch_w->disp_dir = pl_w->pl_dir ^ 1 ;
			pl_w->timer0 = 16 ;
			pl_w->spd.x = pl_w->pl_dir ? 1.6666666f : -1.6666666f ;
			pl_w->acc.x = 0.0f ;
			pl_w->spd.y = DOT_Y( 16.0f ) ;
			pl_w->acc.y = -DOT_Y( 0.375f ) ;
			ofs.x = -DOT_X( 26.0f ) ;
			ofs.y = 75.0f ;
			ofs.z = 0.0f ;
			nage_hit_set( pl_w, &ofs ) ;
			pl_w->hit_stop = 10 ;
			pl_char_set( pl_w, Char_Catch, 6 ) ;
			return ;
		}
		dir_move( pl_w, 0x960 ) ;
		switch( pl_w->dir >> 3 ){
		case 0:
			if( !killerbee_ceil_ck( pl_w, lw ) ) killerbee_right_ck( pl_w, lw ) ;
			break ;
		case 1:
			if( !killerbee_right_ck( pl_w, lw ) ) killerbee_floor_ck( pl_w, lw ) ;
			break ;
		case 2:
			if( !killerbee_floor_ck( pl_w, lw ) ) killerbee_left_ck( pl_w, lw ) ;
			break ;
		case 3:
			if( !killerbee_left_ck( pl_w, lw ) ) killerbee_ceil_ck( pl_w, lw ) ;
			break ;
		}
		if( !pl_w->routine_3 ) return ;
	}
	killerbee_wall( pl_w, lw ) ;
}

/*==============================================================*/
/*			On a wall: kept there as the screen moves, until	*/
/*			the next dive										*/
/*==============================================================*/
static void killerbee_wall( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw )
{
	pl_w->pos.x = scr_work.eye[0].x - lw->wall_x ;
	pl_w->pos.y = scr_work.center_y - lw->wall_y ;
	if( pl_w->pos.y <= pl_w->ground_y ) pl_w->pos.y = pl_w->ground_y ;
	if( pl_char_move( pl_w ) < 0 ){
		if( !pl_w->combo_cnt ){
			killerbee_fall( pl_w, lw ) ;
			return ;
		}
		killerbee_dir_ck( pl_w, lw ) ;
		pl_w->routine_3 = 0 ;
	}
}

/*==============================================================*/
/*			The next dive's direction: at the opponent, or		*/
/*			bounced off the wall when right above them			*/
/*==============================================================*/
static void killerbee_dir_ck( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw )
{
	Uint8 dir_tbl[32][4] = {
		{ 0x0c, 0x00, 0x00, 0x00 }, { 0x0f, 0x00, 0x1f, 0x00 }, { 0x0f, 0x00, 0x1f, 0x00 }, { 0x0f, 0x00, 0x1f, 0x00 },
		{ 0x0e, 0x00, 0x1e, 0x00 }, { 0x0e, 0x00, 0x1e, 0x00 }, { 0x0d, 0x00, 0x1e, 0x00 }, { 0x0d, 0x00, 0x1d, 0x00 },
		{ 0x00, 0x00, 0x1d, 0x00 }, { 0x00, 0x01, 0x13, 0x00 }, { 0x00, 0x01, 0x12, 0x00 }, { 0x00, 0x02, 0x12, 0x00 },
		{ 0x00, 0x02, 0x12, 0x00 }, { 0x00, 0x03, 0x12, 0x00 }, { 0x00, 0x03, 0x11, 0x00 }, { 0x00, 0x03, 0x11, 0x00 },
		{ 0x00, 0x1c, 0x00, 0x00 }, { 0x00, 0x1f, 0x00, 0x0f }, { 0x00, 0x1f, 0x00, 0x0f }, { 0x00, 0x1e, 0x00, 0x0e },
		{ 0x00, 0x1e, 0x00, 0x0e }, { 0x00, 0x1e, 0x00, 0x0e }, { 0x00, 0x1d, 0x00, 0x0d }, { 0x00, 0x1d, 0x00, 0x0d },
		{ 0x00, 0x00, 0x00, 0x0c }, { 0x13, 0x00, 0x00, 0x03 }, { 0x13, 0x00, 0x00, 0x03 }, { 0x13, 0x00, 0x00, 0x03 },
		{ 0x12, 0x00, 0x00, 0x02 }, { 0x12, 0x00, 0x00, 0x02 }, { 0x11, 0x00, 0x00, 0x01 }, { 0x11, 0x00, 0x00, 0x01 },
	} ;
	VEC3 pos ;
	Float dx, dy ;

	lw->last_dir = pl_w->dir ;
	if( pl_w->hit_flag && pl_w->ground_y + DOT_Y( 80.0f ) < pl_w->target_w->pos.y ){
		dx = fabs( pl_w->target_w->pos.x - pl_w->pos.x ) ;
		dy = fabs( pl_w->target_w->pos.y - pl_w->pos.y ) ;
		if( dx <= DOT_X( 208.0f ) && dy <= DOT_Y( 128.0f ) && pl_w->target_w->spd.y >= 0.0f ){
			pl_w->dir = dir_tbl[pl_w->dir][pl_w->routine_3 - 1] ;
			pl_w->dir += ( Rnd() & 3 ) - 1 ;
			pl_w->dir &= 31 ;
			killerbee_atck_set( pl_w, lw ) ;
			return ;
		}
	}
	pos = pl_w->target_w->pos ;
	if( pl_w->target_w->spd.y < 0.0f ) pos.y -= DOT_Y( 48.0f ) ;
	pl_w->dir = caldir_pos_256( &pl_w->pos, &pos ) ;
	pl_w->dir = pl_w->dir / 8 + ( pl_w->dir % 8 >= 4 ? 1 : 0 ) ;
	if( pl_w->dir == lw->last_dir ) pl_w->dir += 16 ;
	pl_w->dir &= 31 ;
	killerbee_atck_set( pl_w, lw ) ;
}

/*==============================================================*/
static void killerbee_atck_set( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw )
{
	Uint8 dir ;
	int no ;

	pl_w->timer1 = no = 0 ;
	dir = pl_w->dir ;
	if( dir > 16 ){
		pl_w->pl_dir = pl_w->disp_dir = 0 ;
		dir = ( dir ^ 31 ) + 1 ;
	} else {
		pl_w->pl_dir = pl_w->disp_dir = 1 ;
	}
	if( pl_w->target_w->pos.x <= scr_work.eye[0].x ) no = 9 ;
	if( pl_w->pl_dir ) no ^= 9 ;
	no += killerbee_char_tbl[dir][1] ;
	if( lw->hit_cnt ){
		if( lw->hit_cnt == 1 ) pl_w->atck_no = pl24_at_killerbee_last ;
		else pl_w->atck_no = no ;
		HIT_RESET( pl_w ) ;
	}
	pl_char_set( pl_w, Char_Super, killerbee_char_tbl[dir][0] ) ;
}

/*==============================================================*/
/*			The hit missed: fall away							*/
/*==============================================================*/
static void killerbee_fall( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw )
{
	pl_w->routine_2 = 12 ;
	pl_w->spd.x = pl_w->pl_dir ? -2.5f : 2.5f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 8.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_char_set( pl_w, Char_Super, 13 ) ;
}

/*==============================================================*/
static Uint8 killerbee_ceil_ck( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw )
{
	Float top ;

	if( pl_w->dir == 24 ) return 0 ;
	top = scr_work.center_y + 102.85715f ;
	if( pl_w->pos.y < top ) return 0 ;
	pl_w->routine_3 = 1 ;
	killerbee_ofs_set( pl_w, lw ) ;
	lw->wall_y = -DOT_Y( 64.0f ) ;
	pl_char_set( pl_w, Char_Super, 1 ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 killerbee_floor_ck( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw )
{
	Float bottom ;

	if( pl_w->dir == 8 ) return 0 ;
	bottom = scr_work.center_y + -240.0f ;
	if( pl_w->pos.y > pl_w->ground_y && pl_w->pos.y > bottom ) return 0 ;
	pl_w->routine_3 = 2 ;
	killerbee_ofs_set( pl_w, lw ) ;
	lw->wall_y = 240.0f ;
	pl_char_set( pl_w, Char_Super, 0 ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 killerbee_left_ck( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw )
{
	if( pl_w->dir == 16 || !( pl_w->wall_flag & WALL_LEFT ) ) return 0 ;
	pl_w->routine_3 = 4 ;
	killerbee_ofs_set( pl_w, lw ) ;
	lw->wall_x = 320.0f ;
	pl_w->pl_dir = pl_w->disp_dir = pl_w->pl_dir ^ 1 ;
	pl_char_set( pl_w, Char_Super, 2 ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 killerbee_right_ck( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw )
{
	if( !pl_w->dir || !( pl_w->wall_flag & WALL_RIGHT ) ) return 0 ;
	pl_w->routine_3 = 3 ;
	killerbee_ofs_set( pl_w, lw ) ;
	lw->wall_x = -320.0f ;
	pl_w->pl_dir = pl_w->disp_dir = pl_w->pl_dir ^ 1 ;
	pl_char_set( pl_w, Char_Super, 2 ) ;
	return 1 ;
}

/*==============================================================*/
static void killerbee_ofs_set( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw )
{
	lw->wall_x = scr_work.eye[0].x - pl_w->pos.x ;
	lw->wall_y = scr_work.center_y - pl_w->pos.y ;
}

/*==============================================================*/
/*			The throw: held, then thrown down					*/
/*==============================================================*/
static void killerbee_catch( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw )
{
	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	if( -- pl_w->timer1 < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer1 = 8 ;
	}
}

/*--------------------------------------------------------------*/
static void killerbee_keep( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw )
{
	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	if( !pl_w->cg_type ) pl_char_move( pl_w ) ;
	if( -- pl_w->timer1 < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->spd.y = DOT_Y( 4.0f ) ;
		pl_w->acc.y = -0.100446425f ;
		pl_w->timer1 = 24 ;
	}
}

/*--------------------------------------------------------------*/
static void killerbee_up( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw )
{
	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	if( !-- pl_w->timer1 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 2.0f ) : -DOT_X( 2.0f ) ;
		pl_w->spd.y = -DOT_Y( 8.0f ) ;
		pl_w->acc.y = -2.142857f ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
}

/*--------------------------------------------------------------*/
static void killerbee_drop( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw )
{
	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	if( !pl_w->cg_type ) pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		if( pl_w->cg_type ){
			pl_w->routine_2 ++ ;
			pl_w->cg_type = 0 ;
			catch_se_req( pl_w ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void killerbee_throw( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->pl_stance = STANCE_AIR ;
		SCR_EXCEP_SET ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 2 ;
		em->atck_no = pl24_at_killerbee_throw ;
		pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 4.0f ) : DOT_X( 4.0f ) ;
		pl_w->spd.y = DOT_Y( 4.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
	}
}

/*--------------------------------------------------------------*/
static void killerbee_hop( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw )
{
	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Jump, 5 ) ;
	}
}

/*--------------------------------------------------------------*/
static void killerbee_end( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw )
{
	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	if( pl_char_move( pl_w ) < 0 ){
		if( pl_w->pl_stance == STANCE_AIR ) pl_jump_ret( pl_w ) ;
		else pl_stand_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void killerbee_miss( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw )
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
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 14 ) ;
	}
}

/*--------------------------------------------------------------*/
static void killerbee_miss_end( PLAYER_WORK *pl_w, PL24_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_stand_ret( pl_w ) ;
	}
}

static void snapback_init( PLAYER_WORK *pl_w );
static void snapback_move( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A standing strike that throws out an effect on		*/
/*			its frame											*/
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
	pl_w->atck_no = pl24_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 28 ) ;
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
		ofs.x = 25.0f ;
		ofs.y = DOT_Y( 76.0f ) ;
		ofs.z = 0.0f ;
		pl_snap_flash( pl_w, &ofs ) ;
	} else if( pl_w->cg_type & 2 ){
		pl_w->cg_type = 0 ;
		pl_se_req( pl_w, SE_SWING_H ) ;
	}
}

/*==============================================================*/
/*			A taunt												*/
/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_char_set( pl_w, Char_Extra, 2 ) ;
	} else if( pl_char_move( pl_w ) < 0 ){
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void shaft_init( PLAYER_WORK *pl_w );
static void shaft_wait( PLAYER_WORK *pl_w );
static void shaft_start( PLAYER_WORK *pl_w );
static void shaft_up( PLAYER_WORK *pl_w );
static void shaft_top( PLAYER_WORK *pl_w );
static void shaft_fall( PLAYER_WORK *pl_w );
static void shaft_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo: a rising spin that hits again and	*/
/*			again												*/
/*==============================================================*/
static void sp_shaft( PLAYER_WORK *pl_w )
{
	static void ( *shaft_jmp[])( PLAYER_WORK *pl_w ) = {
		shaft_init, shaft_wait, shaft_start, shaft_up, shaft_top, shaft_fall, shaft_land,
	} ;

	shaft_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void shaft_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl24_at_shaft_1 ;
	HIT_RESET( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 15 ) ;
}

/*--------------------------------------------------------------*/
static void shaft_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = 30.0f ;
		ofs.y = 120.0f ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shaft_start( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_flag1 ){
		pl_w->cg_flag1 = 0 ;
		pl_w->atck_no = pl24_at_shaft_1 ;
		HIT_RESET( pl_w ) ;
		pl_w->atck_attr = ATR_BACK_POS ;
	}
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->spd.y = DOT_Y( 16.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
	}
}

/*--------------------------------------------------------------*/
static void shaft_up( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( sw_renda_ck( pl_w ) ) pl_w->cg_ctr = 1 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->spd.y <= DOT_Y( 2.0f ) ){
		pl_w->routine_2 ++ ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->acc.y = -0.06696428f ;
		pl_w->timer0 = 0 ;
		pl_w->atck_no = pl24_at_shaft_3 ;
		HIT_RESET( pl_w ) ;
		pl_w->atck_attr = 0 ;
		pl_char_set( pl_w, Char_Super, 16 ) ;
		return ;
	}
	if( pl_w->cg_flag1 ){
		pl_w->cg_flag1 = 0 ;
		pl_w->atck_no = pl24_at_shaft_2 ;
		HIT_RESET( pl_w ) ;
		pl_w->atck_attr = ATR_BACK_POS ;
	}
}

/*==============================================================*/
/*			The top: the animation runs at half speed			*/
/*==============================================================*/
static void shaft_top( PLAYER_WORK *pl_w )
{
	if( ( pl_w->timer0 = ( pl_w->timer0 + 1 ) & 1 ) ) pl_char_move( pl_w ) ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->spd.y > 2.142857f ) return ;
	pl_w->routine_2 ++ ;
	pl_w->acc.y = -0.73660713f ;
}

/*--------------------------------------------------------------*/
static void shaft_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 17 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shaft_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_move( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A kick that leaps off the wall						*/
/****************************************************************/
void	pl24_t_kick( PLAYER_WORK *pl_w )
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
	pl_w->pos.y += DOT_Y( 48.0f ) ;
	pl_w->spd.x = DOT_X( 16.0f ) ;
	if( !pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = -DOT_Y( 0.25f ) ;
	pl_w->atck_no = pl24_at_t_kick ;
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
	if( pl_char_move( pl_w ) < 0 ) pl_stop_stand_ret( pl_w ) ;
	else if( pl_w->cg_type ) pl_w->cg_type = 0 ;
}

static PLAYER_WORK *nml_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *crouch_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *jump_catch_ck( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Can a throw start?									*/
/****************************************************************/
PLAYER_WORK *pl24_catch_ck( PLAYER_WORK *pl_w )
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
		pl_w->dir ^= 3 ;
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 5 ;
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
		pl_w->dir ^= 3 ;
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 2 ;
			return em ;
		}
		return 0 ;
	}
	if( pl_w->atck_pk == 1 && pl_w->atck_level == 1 && pl_w->pos.y > DOT_Y( 64.0f ) ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 1 ;
			return em ;
		}
		return 0 ;
	}
	return 0 ;
}

static void nml_k_set( PLAYER_WORK *pl_w );
static void jump_k_set( PLAYER_WORK *pl_w );
static void jump_p_set( PLAYER_WORK *pl_w );
static void hooligan_h_set( PLAYER_WORK *pl_w );
static void hooligan_l_set( PLAYER_WORK *pl_w );
static void nml_p_set( PLAYER_WORK *pl_w );
static void killerbee_catch_set( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Start the throw that catch_no names					*/
/****************************************************************/
void	pl24_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_k_set, jump_k_set, jump_p_set, hooligan_h_set, hooligan_l_set, nml_p_set, killerbee_catch_set,
	} ;

	catch_set_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_k_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	catch_gauge_add( pl_w ) ;
	ofs.x = -80.0f ;
	ofs.y = DOT_Y( 86.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	pl_w->catch_w->pl_dir = pl_w->catch_w->disp_dir = pl_w->pl_dir ^ 1 ;
	if( !( pl_w->dir & 1 ) ){
		pl_w->pl_dir = pl_w->disp_dir = pl_w->pl_dir ^ 1 ;
		pl_w->catch_w->pl_dir = pl_w->catch_w->disp_dir = pl_w->pl_dir ^ 1 ;
	}
	pl_char_set( pl_w, Char_Catch, 0 ) ;
}

/*==============================================================*/
static void jump_k_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	catch_gauge_add( pl_w ) ;
	pl_w->catch_w->pl_dir = pl_w->catch_w->disp_dir = pl_w->pl_dir ^ 1 ;
	if( !( pl_w->dir & 1 ) ){
		pl_w->pl_dir = pl_w->disp_dir = pl_w->pl_dir ^ 1 ;
		pl_w->catch_w->pl_dir = pl_w->catch_w->disp_dir = pl_w->pl_dir ^ 1 ;
	}
	pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 2.0f ) : DOT_X( 2.0f ) ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = -DOT_Y( 2.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	ofs.x = -50.0f ;
	ofs.y = DOT_Y( 74.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	pl_char_set( pl_w, Char_Catch, 1 ) ;
}

/*==============================================================*/
static void jump_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	catch_gauge_add( pl_w ) ;
	pl_w->catch_w->pl_dir = pl_w->catch_w->disp_dir = pl_w->pl_dir ^ 1 ;
	if( pl_w->dir & 2 ){
		pl_w->pl_dir = pl_w->disp_dir = pl_w->pl_dir ^ 1 ;
		pl_w->catch_w->pl_dir = pl_w->catch_w->disp_dir = pl_w->pl_dir ^ 1 ;
	}
	pl_w->spd.y = 0.0f ;
	ofs.x = DOT_X( 7.0f ) ;
	ofs.y = DOT_Y( 107.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	pl_char_set( pl_w, Char_Catch, 2 ) ;
}

/*==============================================================*/
static void hooligan_h_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	catch_gauge_add( pl_w ) ;
	ofs.x = -80.0f ;
	ofs.y = DOT_Y( 86.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	pl_w->pl_dir = pl_w->disp_dir = pl_w->pl_dir ^ 1 ;
	pl_w->catch_w->pl_dir = pl_w->catch_w->disp_dir = pl_w->pl_dir ^ 1 ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_char_set( pl_w, Char_Catch, 3 ) ;
}

/*==============================================================*/
static void hooligan_l_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	catch_gauge_add( pl_w ) ;
	pl_w->catch_w->pl_dir = pl_w->catch_w->disp_dir = pl_w->pl_dir ^ 1 ;
	pl_w->spd.x = pl_w->pl_dir ? DOT_X( 4.0f ) : -DOT_X( 4.0f ) ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 4.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	ofs.x = -DOT_X( 26.0f ) ;
	ofs.y = 75.0f ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	pl_char_set( pl_w, Char_Catch, 4 ) ;
}

/*==============================================================*/
static void nml_p_set( PLAYER_WORK *pl_w )
{
	catch_gauge_add( pl_w ) ;
	pl_w->catch_w->pl_dir = pl_w->catch_w->disp_dir = pl_w->pl_dir ^ 1 ;
	if( pl_w->dir & 2 ){
		pl_w->pl_dir = pl_w->disp_dir = pl_w->pl_dir ^ 1 ;
		pl_w->catch_w->pl_dir = pl_w->catch_w->disp_dir = pl_w->pl_dir ^ 1 ;
	}
	pl_char_set( pl_w, Char_Catch, 5 ) ;
}

/*==============================================================*/
/*			The end of Killer Bee Assault						*/
/*==============================================================*/
static void killerbee_catch_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->catch_w->pl_dir = pl_w->catch_w->disp_dir = pl_w->pl_dir ^ 1 ;
	pl_w->timer0 = 16 ;
	pl_w->spd.x = pl_w->pl_dir ? 1.6666666f : -1.6666666f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 16.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	ofs.x = -DOT_X( 26.0f ) ;
	ofs.y = 75.0f ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_char_set( pl_w, Char_Catch, 6 ) ;
}

static void nml_k_catch( PLAYER_WORK *pl_w );
static void jump_k_catch( PLAYER_WORK *pl_w );
static void jump_p_catch( PLAYER_WORK *pl_w );
static void hooligan_h_catch( PLAYER_WORK *pl_w );
static void hooligan_l_catch( PLAYER_WORK *pl_w );
static void nml_p_catch( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void	pl24_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_k_catch, jump_k_catch, jump_p_catch, hooligan_h_catch, hooligan_l_catch, nml_p_catch, nml_k_catch,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

static void nml_k_catch_start( PLAYER_WORK *pl_w );
static void nml_k_catch_jump( PLAYER_WORK *pl_w );
static void nml_k_catch_throw( PLAYER_WORK *pl_w );
static void nml_k_catch_hop( PLAYER_WORK *pl_w );
static void nml_k_catch_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void nml_k_catch( PLAYER_WORK *pl_w )
{
	static void ( *nml_k_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_k_catch_start, nml_k_catch_jump, nml_k_catch_throw, nml_k_catch_hop, nml_k_catch_end,
	} ;

	nml_k_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void nml_k_catch_start( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_3 ){
		pl_w->routine_3 ++ ;
	} else {
		pl_char_move( pl_w ) ;
		if( pl_w->cg_type ){
			pl_w->routine_2 ++ ;
			scr_excep_set( pl_w, 5, 5 ) ;
			pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 2.0f ) : DOT_X( 2.0f ) ;
			pl_w->acc.x = 0.0f ;
			pl_w->spd.y = DOT_Y( 8.0f ) ;
			pl_w->acc.y = -DOT_Y( 0.75f ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void nml_k_catch_jump( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type ) pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->spd.y > 0.0f ) return ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		SCR_EXCEP_SET ;
	}
}

/*--------------------------------------------------------------*/
static void nml_k_catch_throw( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 3 ;
		em->atck_no = pl24_at_throw_k ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 2.0f ) : -DOT_X( 2.0f ) ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 4.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_dir_set( pl_w ) ;
		scr_work.quake_level = 2 ;
		scr_work.quake_flag = 1 ;
	}
}

/*--------------------------------------------------------------*/
static void nml_k_catch_hop( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
	}
}

/*--------------------------------------------------------------*/
static void nml_k_catch_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void jump_k_catch_fall( PLAYER_WORK *pl_w );
static void jump_k_catch_throw( PLAYER_WORK *pl_w );
static void jump_k_catch_hop( PLAYER_WORK *pl_w );
static void jump_k_catch_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void jump_k_catch( PLAYER_WORK *pl_w )
{
	static void ( *jump_k_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		jump_k_catch_fall, jump_k_catch_throw, jump_k_catch_hop, jump_k_catch_end,
	} ;

	jump_k_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void jump_k_catch_fall( PLAYER_WORK *pl_w )
{
	if( !pl_w->cg_type ) pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		if( pl_w->cg_type ){
			pl_w->routine_2 ++ ;
			pl_w->cg_type = 0 ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
		}
	}
}

/*--------------------------------------------------------------*/
static void jump_k_catch_throw( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 3 ;
		em->atck_no = pl24_at_air_throw_k ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 2.0f ) : -DOT_X( 2.0f ) ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 4.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_dir_set( pl_w ) ;
		scr_work.quake_level = 2 ;
		scr_work.quake_flag = 1 ;
	}
}

/*--------------------------------------------------------------*/
static void jump_k_catch_hop( PLAYER_WORK *pl_w )
{
	nml_k_catch_hop( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void jump_k_catch_end( PLAYER_WORK *pl_w )
{
	nml_k_catch_end( pl_w ) ;
}

/*==============================================================*/
static void jump_p_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
	}
	if( pl_w->routine_2 || pl_char_move( pl_w ) < 0 ){
		if( pl_w->pl_stance == STANCE_AIR ) pl_jump_ret( pl_w ) ;
		else pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->atck_no = pl24_at_air_throw_p ;
	}
}

static void hooligan_h_catch_start( PLAYER_WORK *pl_w );
static void hooligan_h_catch_jump( PLAYER_WORK *pl_w );
static void hooligan_h_catch_throw( PLAYER_WORK *pl_w );
static void hooligan_h_catch_hop( PLAYER_WORK *pl_w );
static void hooligan_h_catch_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void hooligan_h_catch( PLAYER_WORK *pl_w )
{
	static void ( *hooligan_h_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		hooligan_h_catch_start, hooligan_h_catch_jump, hooligan_h_catch_throw, hooligan_h_catch_hop, hooligan_h_catch_end,
	} ;

	hooligan_h_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void hooligan_h_catch_start( PLAYER_WORK *pl_w )
{
	nml_k_catch_start( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void hooligan_h_catch_jump( PLAYER_WORK *pl_w )
{
	nml_k_catch_jump( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void hooligan_h_catch_throw( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 3 ;
		em->atck_no = pl24_at_hooligan_throw_h ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 2.0f ) : -DOT_X( 2.0f ) ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 6.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_dir_set( pl_w ) ;
		scr_work.quake_level = 2 ;
		scr_work.quake_flag = 1 ;
	}
}

/*--------------------------------------------------------------*/
static void hooligan_h_catch_hop( PLAYER_WORK *pl_w )
{
	nml_k_catch_hop( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void hooligan_h_catch_end( PLAYER_WORK *pl_w )
{
	nml_k_catch_end( pl_w ) ;
}

static void hooligan_l_catch_fall( PLAYER_WORK *pl_w );
static void hooligan_l_catch_throw( PLAYER_WORK *pl_w );
static void hooligan_l_catch_hop( PLAYER_WORK *pl_w );
static void hooligan_l_catch_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void hooligan_l_catch( PLAYER_WORK *pl_w )
{
	static void ( *hooligan_l_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		hooligan_l_catch_fall, hooligan_l_catch_throw, hooligan_l_catch_hop, hooligan_l_catch_end,
	} ;

	hooligan_l_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void hooligan_l_catch_fall( PLAYER_WORK *pl_w )
{
	if( !pl_w->cg_type ) pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		if( pl_w->cg_type ){
			pl_w->routine_2 ++ ;
			pl_w->cg_type = 0 ;
			catch_se_req( pl_w ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void hooligan_l_catch_throw( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 2 ;
		em->atck_no = pl24_at_hooligan_throw_l ;
		pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 4.0f ) : DOT_X( 4.0f ) ;
		pl_w->spd.y = DOT_Y( 4.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
	}
}

/*--------------------------------------------------------------*/
static void hooligan_l_catch_hop( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_char_set( pl_w, Char_Jump, 5 ) ;
	}
}

/*--------------------------------------------------------------*/
static void hooligan_l_catch_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		if( pl_w->pl_stance == STANCE_AIR ) pl_jump_ret( pl_w ) ;
		else pl_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
static void nml_p_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_flag1 ){
		pl_w->cg_flag1 = 0 ;
		ofs.x = DOT_X( 32.0f ) ;
		ofs.y = 120.0f ;
		ofs.z = 0.0f ;
		nage_hit_set( pl_w, &ofs ) ;
		pl_w->hit_stop = 10 ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 2 ;
		em->atck_no = pl24_at_throw_p ;
		catch_se_req( pl_w ) ;
	}
}

static void nml_k_hold( PLAYER_WORK *pl_w );
static void hold( PLAYER_WORK *pl_w );
static void hooligan_h_hold( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Being thrown										*/
/****************************************************************/
void	pl24_hold( PLAYER_WORK *pl_w )
{
	static void ( *hold_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_k_hold, hold, hold, hooligan_h_hold, hold, hold, hold,
	} ;

	hold_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_k_hold( PLAYER_WORK *pl_w )
{
	switch( pl_w->catch_w->cg_flag1 ){
	case 1:
		catch_em_pos_set0( pl_w->catch_w, pl_w ) ;
		return ;
	case 2:
		if( pl_w->catch_w->pos.y <= pl_w->catch_w->ground_y ) pl_w->catch_w->pos.y = pl_w->catch_w->ground_y ;
		break ;
	}
	catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/*==============================================================*/
static void hold( PLAYER_WORK *pl_w )
{
	if( pl_w->catch_w->cg_flag1 ) catch_em_pos_set0( pl_w->catch_w, pl_w ) ;
	else catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/*==============================================================*/
static void hooligan_h_hold( PLAYER_WORK *pl_w )
{
	switch( pl_w->catch_w->cg_flag1 ){
	case 1:
		catch_em_pos_set0( pl_w->catch_w, pl_w ) ;
		return ;
	case 2:
		if( pl_w->catch_w->pos.y <= pl_w->catch_w->ground_y ) pl_w->catch_w->pos.y = pl_w->catch_w->ground_y ;
		break ;
	}
	catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void	pl24_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl24_sp_smasher ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl24_sp_smasher ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl24_sp_killerbee ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl24_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl24_sp_smasher ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl24_sp_smasher ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl24_sp_killerbee ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl24_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl24_sp_spike ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl24_sp_arrow ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl24_sp_knuckle ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void	pl24_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl24_sp_spike ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl24_sp_arrow ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl24_sp_knuckle ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/* the entrance's colours, one set for each of the six colours */
static Uint16 appear_color[6][16] = {
	{ 0x0000, 0xf111, 0xffda, 0xffb8, 0xfe96, 0xfc64, 0xf842, 0xf520,
	  0xff60, 0xfd40, 0xfc20, 0xf910, 0xf700, 0xff50, 0xfffb, 0xfff0 },
	{ 0x0000, 0xf111, 0xffda, 0xffb8, 0xfe96, 0xfc64, 0xf842, 0xf520,
	  0xf6cf, 0xf6ac, 0xf489, 0xf266, 0xf044, 0xff50, 0xfffb, 0xfff0 },
	{ 0x0000, 0xf111, 0xffda, 0xffa7, 0xfc75, 0xf753, 0xf433, 0xf111,
	  0xff60, 0xfd40, 0xfc20, 0xf910, 0xf700, 0xff50, 0xfffb, 0xfff0 },
	{ 0x0000, 0xf111, 0xffda, 0xffa7, 0xfc75, 0xf753, 0xf433, 0xf111,
	  0xf6cf, 0xf6ac, 0xf489, 0xf266, 0xf044, 0xf51f, 0xfffb, 0xfff0 },
	{ 0x0000, 0xf111, 0xffd9, 0xff95, 0xfd67, 0xfa46, 0xf624, 0xf523,
	  0xff60, 0xfd40, 0xfc20, 0xf910, 0xf700, 0xfe14, 0xfffb, 0xff60 },
	{ 0x0000, 0xf111, 0xfc63, 0xf943, 0xf623, 0xf413, 0xf303, 0xf104,
	  0xf6cf, 0xf6ac, 0xf489, 0xf266, 0xf044, 0xff30, 0xfd95, 0xff0a },
} ;

/*==============================================================*/
static void appear_color_set( PLAYER_WORK *pl_w )
{
	COL_WORK *ow = &col_work[pl_w->col_no] + 2 ;
	Uint16 *dst ;
	Uint16 *src ;
	int i ;

	ow->trans_flag = 1 ;
	src = appear_color[pl_w->pl_color] ;
	dst = ow->col ;
	for( i = 0 ; i < 16 ; i ++ ) *dst++ = *src++ ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl24.c" End of File	*/

/* Guile (character 02): command checks, normal attacks, appearance, win and lose poses, special
   moves and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl02_def.h"

/* speeds for a jump */
typedef struct JUMP_SPD {
	Float	spd_x, acc_x, spd_y, acc_y;
} JUMP_SPD;

void pl02_main( PLAYER_WORK *pl_w );
void pl02_com_ck( PLAYER_WORK *pl_w );
void pl02_atck_set( PLAYER_WORK *pl_w );
void pl02_atck( PLAYER_WORK *pl_w );
void pl02_dash( PLAYER_WORK *pl_w );
void pl02_noki( PLAYER_WORK *pl_w );
void pl02_appear( PLAYER_WORK *pl_w );
void pl02_win( PLAYER_WORK *pl_w );
void pl02_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl02_catch_ck( PLAYER_WORK *pl_w );
void pl02_catch_set( PLAYER_WORK *pl_w );
void pl02_catch( PLAYER_WORK *pl_w );
void pl02_hold( PLAYER_WORK *pl_w );
int pl02_delay_ck( PLAYER_WORK *pl_w );
void pl02_t_kick( PLAYER_WORK *pl_w );
void pl02_upper( PLAYER_WORK *pl_w );
void pl02_sp_unison( PLAYER_WORK *pl_w );
void pl02_sp_gattai( PLAYER_WORK *pl_w );
void pl02_sp_assist( PLAYER_WORK *pl_w );
void pl02_sp_zero( PLAYER_WORK *pl_w );

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
static Uint16 com_sonic[] = { 40, COM_GROUND, pl02_sp_sonic, 0, KEY_BACK, KEY_FORWARD, KEY_PUNCH } ;	/* Sonic Boom */
static Uint16 com_smr[] = { 40, COM_GROUND, pl02_sp_smr, 0, KEY_DOWN, KEY_UP, KEY_KICK } ;	/* Somersault Kick */
static Uint16 com_jsmr[] = { 2, COM_AIR, pl02_sp_jsmr, 0, KEY_DOWN, KEY_UP, KEY_KICK } ;	/* Somersault Kick in the air */
static Uint16 com_strike[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, 3, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Somersault Strike (hyper) */
static Uint16 com_hurricane[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, 4, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Sonic Hurricane (hyper) */
static Uint16 com_crossfire[] = { 3, COM_AIR | COM_HYPER | COM_ALL_SW, 7, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Crossfire Assault (hyper, in the air) */

/* the Somersault Kick's speeds for each strength */
static JUMP_SPD smr_spd[2] = {
	{ DOT_X( 2.5f ), 0.0f, DOT_Y( 9.0f ), -DOT_Y( 0.5f ) },
	{ 5.0f, 0.0f, DOT_Y( 13.0f ), -DOT_Y( 0.5f ) },
} ;

static void ( *pl02_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl02_main,
	pl02_com_ck,
	pl02_atck_set,
	pl02_atck,
	pl02_dash,
	pl02_noki,
	pl02_appear,
	pl02_win,
	pl02_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl02_catch_ck,	/* returns a value */
	pl02_catch_set,
	pl02_catch,
	pl02_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl02_delay_ck,
	pl02_t_kick,
	pl02_upper,
	pl02_sp_unison,
	pl02_sp_gattai,
	pl02_sp_assist,
	pl02_sp_zero,
} ;

static Uint8 sonic_ck( PLAYER_WORK *pl_w );
static Uint8 smr_ck( PLAYER_WORK *pl_w );
static Uint8 jsmr_ck( PLAYER_WORK *pl_w );
static Uint8 strike_ck( PLAYER_WORK *pl_w );
static Uint8 hurricane_ck( PLAYER_WORK *pl_w );
static Uint8 crossfire_ck( PLAYER_WORK *pl_w );
static int chouhatsu_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static int hurricane_delay_ck( PLAYER_WORK *pl_w );
static int crossfire_delay_ck( PLAYER_WORK *pl_w );
static int strike_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl02_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl02_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl02_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( sonic_ck( pl_w ) ) return ;
	if( crossfire_ck( pl_w ) ) return ;
	if( smr_ck( pl_w ) ) return ;
	if( jsmr_ck( pl_w ) ) return ;
	if( strike_ck( pl_w ) ) return ;
	if( hurricane_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
static Uint8 sonic_ck( PLAYER_WORK *pl_w )
{
	PL02_LOCAL_W *lw = ( PL02_LOCAL_W * )pl_w->tlocal ;

	if( !com_tame_ck( pl_w, com_sonic, &pl_w->com_w0 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	if( lw->shot_flag ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl02_sp_sonic ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 smr_ck( PLAYER_WORK *pl_w )
{
	if( !com_tame_ck( pl_w, com_smr, &pl_w->com_w1 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl02_sp_smr ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			The air somersault: once a jump, and not too low	*/
/*==============================================================*/
static Uint8 jsmr_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_jsmr, &pl_w->com_w2 ) ) return 0 ;
	if( pl_w->ground_y + 112.0f > pl_w->pos.y ) return 0 ;
	if( pl_w->jump_com_cnt ) return 0 ;
	pl_w->jump_com_cnt ++ ;
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl02_sp_jsmr ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 strike_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_strike, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w3 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl02_sp_strike ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 hurricane_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_hurricane, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w4 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl02_sp_hurricane ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 crossfire_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_crossfire, &pl_w->com_w5 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag && pl_w->jump_com_cnt ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w5 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl02_sp_crossfire ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static int chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 6 ) ) return 0 ;
	pl_w->pl_sp_type = pl02_sp_chouhatsu ;
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
	pl_w->pl_sp_type = pl02_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl02_delay_ck( PLAYER_WORK *pl_w )
{
	if( hurricane_delay_ck( pl_w ) || crossfire_delay_ck( pl_w ) || strike_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static int hurricane_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_hurricane, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 4 ;
	return 1 ;
}

/*==============================================================*/
static int crossfire_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_crossfire, &pl_w->com_w5 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 7 ;
	return 1 ;
}

/*==============================================================*/
static int strike_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_strike, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 3 ;
	return 1 ;
}

/****************************************************************/
void pl02_main( PLAYER_WORK *pl_w )
{
}

/* how close the other player must be for the close punch and kick */
static Float near_range[2][3] = {
	{ DOT_X( 64.0f ), DOT_X( 64.0f ), DOT_X( 64.0f ) },
	{ DOT_X( 64.0f ), DOT_X( 64.0f ), DOT_X( 64.0f ) },
} ;

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
void pl02_atck_set( PLAYER_WORK *pl_w )
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
		if( pl_far_ck( pl_w, near_range[0][0] ) ){
			pl_w->char_no.b.no = 3 ;
			pl_w->atck_no = pl02_at_far_lp ;
		} else {
			pl_w->char_no.b.no = 0 ;
			pl_w->atck_no = ATCK_STAND_LP ;
		}
		pl_se_req( pl_w, SE_SWING_L ) ;
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		pl_w->unk_2a2 = 1 ;
		break ;
	case 1:
		pl_w->char_no.b.no = 4 ;
		pl_w->atck_no = pl02_at_far_mp ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_STAND_HP ;
		if( pl_w->atck_lever == KEY_FORWARD ){
			pl_w->char_no.b.no = 6 ;
			pl_w->atck_no = pl02_at_fwd_hp ;
		} else if( pl_w->atck_lever == KEY_BACK ){
			pl_w->char_no.b.no = 5 ;
			pl_w->atck_no = pl02_at_back_hp ;
		}
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
		if( pl_far_ck( pl_w, near_range[1][0] ) ){
			pl_w->char_no.b.no = 3 ;
			pl_w->atck_no = pl02_at_far_lk ;
		} else {
			pl_w->char_no.b.no = 0 ;
			pl_w->atck_no = ATCK_STAND_LK ;
		}
		if( pl_w->atck_lever == KEY_BACK ){
			pl_w->char_no.b.no = 8 ;
			pl_w->nml_no = 0x61 ;
			pl_w->atck_no = pl02_at_back_lk ;
		}
		pl_se_req( pl_w, SE_SWING_L ) ;
		pl_w->chain_tbl = lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->char_no.b.no = 4 ;
		pl_w->atck_no = pl02_at_far_mk ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_w->chain_tbl = mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		if( pl_far_ck( pl_w, near_range[1][2] ) ){
			pl_w->char_no.b.no = 5 ;
			pl_w->atck_no = pl02_at_far_hk ;
		} else {
			pl_w->char_no.b.no = 2 ;
			pl_w->atck_no = ATCK_STAND_HK ;
		}
		if( pl_w->atck_lever == KEY_BACK ){
			pl_w->char_no.b.no = 6 ;
			pl_w->nml_no = 0x60 ;
			pl_w->atck_no = pl02_at_back_hk ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
		}
		if( pl_w->atck_lever == KEY_FORWARD ){
			pl_w->char_no.b.no = 7 ;
			pl_w->atck_no = pl02_at_fwd_hk ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
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
		if( pl_w->atck_lever & KEY_UP ){
			pl_w->char_no.b.no = 5 ;
			pl_w->atck_no = ATCK_JUMP_HP ;
		} else {
			pl_w->char_no.b.no = 2 ;
			pl_w->atck_no = ATCK_JUMP_HP ;
		}
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
		if( pl_w->atck_lever & KEY_UP ){
			pl_w->char_no.b.no = 0 ;
			pl_w->atck_no = ATCK_JUMP_LK ;
		} else {
			pl_w->char_no.b.no = 3 ;
			pl_w->atck_no = ATCK_JUMP_LK ;
		}
		pl_se_req( pl_w, SE_SWING_L ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_lk_tbl ;
		else pl_w->chain_tbl = sj_lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		if( pl_w->atck_lever & KEY_UP ){
			pl_w->char_no.b.no = 4 ;
			pl_w->atck_no = ATCK_JUMP_MK ;
		} else {
			pl_w->char_no.b.no = 1 ;
			pl_w->atck_no = ATCK_JUMP_MK ;
		}
		pl_se_req( pl_w, SE_SWING_M ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mk_tbl ;
		else pl_w->chain_tbl = sj_mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		if( pl_w->atck_lever & KEY_UP ){
			pl_w->char_no.b.no = 5 ;
			pl_w->atck_no = ATCK_JUMP_HK ;
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
static void lever_atck( PLAYER_WORK *pl_w );
static void lever_hk( PLAYER_WORK *pl_w );
static void lever_hk_start( PLAYER_WORK *pl_w );
static void lever_hk_move( PLAYER_WORK *pl_w );
static void lever_lk( PLAYER_WORK *pl_w );
static void lever_lk_start( PLAYER_WORK *pl_w );
static void lever_lk_jump( PLAYER_WORK *pl_w );
static void lever_lk_land( PLAYER_WORK *pl_w );

/****************************************************************/
void pl02_atck( PLAYER_WORK *pl_w )
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
	if( pl_w->nml_no == 0x60 ){
		pl_w->timer0 = 0 ;
		lever_atck( pl_w ) ;
		return ;
	}
	if( pl_w->nml_no == 0x61 ){
		pl_w->timer0 = 1 ;
		lever_atck( pl_w ) ;
		return ;
	}
	if( pl_w->atck_pk == 1 ){
		if( pl_w->pl_stance == STANCE_CROUCH ) crouch_kick_move( pl_w ) ;
		else stand_kick_move( pl_w ) ;
	} else {
		if( pl_w->pl_stance == STANCE_CROUCH ) crouch_punch_move( pl_w ) ;
		else stand_punch_move( pl_w ) ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_brake_stop( pl_w ) ;
}

/*==============================================================*/
static void stand_punch_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void crouch_punch_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void stand_kick_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			The heavy one hits twice							*/
/*==============================================================*/
static void crouch_kick_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	if( pl_w->nml_no == 2 && pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_se_req( pl_w, SE_SWING_H ) ;
		pl_w->atck_no = pl02_at_crouch_hk_2 ;
		HIT_RESET( pl_w ) ;
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

/*==============================================================*/
/*			The attacks done with the stick held				*/
/*==============================================================*/
static void lever_atck( PLAYER_WORK *pl_w )
{
	static void ( *lever_atck_jmp[])( PLAYER_WORK *pl_w ) = {
		lever_hk, lever_lk,
	} ;

	lever_atck_jmp[pl_w->timer0]( pl_w ) ;
}

/*==============================================================*/
static void lever_hk( PLAYER_WORK *pl_w )
{
	static void ( *lever_hk_jmp[])( PLAYER_WORK *pl_w ) = {
		lever_hk_start, lever_hk_move,
	} ;

	lever_hk_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void lever_hk_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->spd.x = DOT_X( 3.25f ) ;
		pl_w->acc.x = 0.0f ;
		if( !pl_w->pl_dir ){
			pl_w->spd.x = -DOT_X( 3.25f ) ;
			pl_w->acc.x = -0.0f ;
		}
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
}

/*--------------------------------------------------------------*/
static void lever_hk_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		RATE_CLR_X( pl_w ) ;
	}
}

/*==============================================================*/
static void lever_lk( PLAYER_WORK *pl_w )
{
	static void ( *lever_lk_jmp[])( PLAYER_WORK *pl_w ) = {
		lever_lk_start, lever_lk_jump, lever_lk_land,
	} ;

	lever_lk_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void lever_lk_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = DOT_X( 4.0f ) ;
		pl_w->acc.x = 0.0f ;
		if( !pl_w->pl_dir ){
			pl_w->spd.x = -DOT_X( 4.0f ) ;
			pl_w->acc.x = -0.0f ;
		}
		pl_w->spd.y = DOT_Y( 3.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
	}
}

/*--------------------------------------------------------------*/
static void lever_lk_jump( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Stand_K, 9 ) ;
	}
}

/*--------------------------------------------------------------*/
static void lever_lk_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/****************************************************************/
/*			The launcher										*/
/****************************************************************/
void pl02_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_w->atck_no = pl02_at_upper ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_char_set( pl_w, Char_Extra, 4 ) ;
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

static void dash_init( PLAYER_WORK *pl_w );
static void dash_move( PLAYER_WORK *pl_w );
static void dash_stop( PLAYER_WORK *pl_w );
static void dash_end( PLAYER_WORK *pl_w );

/****************************************************************/
void pl02_dash( PLAYER_WORK *pl_w )
{
	static void ( *dash_jmp[])( PLAYER_WORK *pl_w ) = {
		dash_init, dash_move, dash_stop, dash_end,
	} ;

	dash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dash_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.x = pl_w->pl_dir ? DOT_X( 9.5f ) : -DOT_X( 9.5f ) ;
	pl_w->acc.x = pl_w->pl_dir ? -DOT_X( 0.125f ) : DOT_X( 0.125f ) ;
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void dash_move( PLAYER_WORK *pl_w )
{
	if( !pl_w->cg_type ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Dash, 2 ) ;
		pl_w->acc.x = pl_w->pl_dir ? -DOT_X( 0.5f ) : DOT_X( 0.5f ) ;
	}
}

/*--------------------------------------------------------------*/
static void dash_stop( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		return ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void dash_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void noki_init( PLAYER_WORK *pl_w );
static void noki_jump( PLAYER_WORK *pl_w );
static void noki_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash, a hop								*/
/****************************************************************/
void pl02_noki( PLAYER_WORK *pl_w )
{
	static void ( *noki_jmp[])( PLAYER_WORK *pl_w ) = {
		noki_init, noki_jump, noki_land,
	} ;

	noki_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void noki_init( PLAYER_WORK *pl_w )
{
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 9.5f ) : DOT_X( 9.5f ) ;
		pl_w->acc.x = pl_w->pl_dir ? 0.3125f : -0.3125f ;
		pl_w->spd.y = DOT_Y( 3.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.25f ) ;
		noki_jump( pl_w ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void noki_jump( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Dash, 3 ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? -5.0f : 5.0f ;
		pl_w->acc.x = pl_w->pl_dir ? DOT_X( 0.125f ) : -DOT_X( 0.125f ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void noki_land( PLAYER_WORK *pl_w )
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
static void win_ko_move_e( PLAYER_WORK *pl_w );
static void lose_time_ko( PLAYER_WORK *pl_w );
static void draw_time( PLAYER_WORK *pl_w );

/****************************************************************/
void pl02_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_init,
		appear_move,
	} ;

	appear_jmp[ pl_w->routine_2 ]( pl_w ) ;
}

/*==============================================================*/
static void appear_init( PLAYER_WORK *pl_w )
{
	pl_w -> routine_2 ++ ;
	pl_w -> disp_flag = 1 ;
	pl_char_set( pl_w, Char_Appear, 0 ) ;
}

/*--------------------------------------------------------------*/
static void appear_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w -> routine_1 ++ ;
	}
}

/****************************************************************/
void pl02_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_time_ko,
		lose_time_ko,
		win_time_ko,
		lose_time_ko,
		draw_time,
	} ;

	pl_w -> pos.y = pl_w -> ground_y ;
	if( pl_win_end_ck( pl_w ) ){
		pl_stand_ret( pl_w ) ;
	}else{
		win_jmp[ pl_w->type ]( pl_w ) ;
	}
}

/*==============================================================*/
/*			KO win, time-out win								*/
/*==============================================================*/
static void win_time_ko( PLAYER_WORK *pl_w )
{
	static void ( *win_ko_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko_init,
		win_ko_move,
		win_ko_move_e,
	} ;

	win_ko_jmp[ pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void win_ko_init( PLAYER_WORK *pl_w )
{
	pl_w -> routine_2 ++ ;

	/***** Time-out win *****/
	if( pl_w->type == 2 ){
		pl_char_set( pl_w, Char_Win, CH02_WIN_KO4 ) ;
		return ;
	}

	/***** KO win: one of four poses *****/
	pl_w -> timer0 = Rnd() & 0x3 ;

	switch( pl_w -> timer0 ){
	case 0 :
		pl_char_set( pl_w, Char_Win, CH02_WIN_KO0 ) ;		/* win pose 1 */
		break ;
	case 1 :
		pl_char_set( pl_w, Char_Win, CH02_WIN_KO1 ) ;		/* win pose 2 */
		break ;
	case 2 :
		pl_char_set( pl_w, Char_Win, CH02_WIN_KO2 ) ;		/* win pose 3 */
		break ;
	case 3 :
		pl_char_set( pl_w, Char_Win, CH02_WIN_KO3 ) ;		/* win pose 4 */
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
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
/*			KO loss, time-out loss								*/
/*==============================================================*/
static void lose_time_ko( PLAYER_WORK *pl_w )
{
	if( !pl_w -> routine_2 ){
		pl_w -> routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, CH02_LOSE ) ;
	}else{
		pl_char_move( pl_w ) ;
	}
}

/*==============================================================*/
/*			Time-out draw										*/
/*==============================================================*/
static void draw_time( PLAYER_WORK *pl_w )
{
	if( !pl_w -> routine_2 ){
		pl_w -> routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, CH02_TM_DRAW ) ;
	}else{
		pl_char_move( pl_w ) ;
	}
}

static void sp_sonic( PLAYER_WORK *pl_w );
static void sp_smr( PLAYER_WORK *pl_w );
static void sp_jsmr( PLAYER_WORK *pl_w );
static void sp_strike( PLAYER_WORK *pl_w );
static void sp_hurricane( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_crossfire( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Special moves										*/
/****************************************************************/
void	pl02_sp_atck( PLAYER_WORK *pl_w )
{
	static void (*pl_sp_tbl[])( PLAYER_WORK *pl_w ) ={
		sp_sonic,				/* 00 Sonic Boom */
		sp_smr,					/* 01 Somersault Kick */
		sp_jsmr,				/* 02 Somersault Kick in the air */
		sp_strike,					/* 03 Somersault Strike (hyper) */
		sp_hurricane,					/* 04 Sonic Hurricane (hyper) */
		sp_snapback,					/* 05 the snapback */
		sp_chouhatsu,					/* 06 the taunt */
		sp_crossfire,					/* 07 Crossfire Assault (hyper, in the air) */
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

static void sonic_init( PLAYER_WORK *pl_w );
static void sonic_move( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Sonic Boom											*/
/*==============================================================*/
static void sp_sonic( PLAYER_WORK *pl_w )
{
	static void ( *sonic_jmp[])( PLAYER_WORK *pl_w ) = {
		sonic_init, sonic_move,
	} ;

	sonic_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void sonic_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	sp_stand_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = pl02_at_sonic ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_voice_req( pl_w, 20 ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level ) ;
	sonic_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void sonic_move( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type ){
		shl02_03_set( pl_w ) ;
		pl_voice_req( pl_w, 30 ) ;
		pl_w->cg_type = 0 ;
	}
	if( pl_w->cg_prm ){
		shl02_00_set( pl_w ) ;
		pl_w->cg_prm = 0 ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void smr_init( PLAYER_WORK *pl_w );
static void smr_start( PLAYER_WORK *pl_w );
static void smr_jump( PLAYER_WORK *pl_w );
static void smr_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Somersault Kick										*/
/*==============================================================*/
static void sp_smr( PLAYER_WORK *pl_w )
{
	static void ( *smr_jmp[])( PLAYER_WORK *pl_w ) = {
		smr_init, smr_start, smr_jump, smr_land,
	} ;

	smr_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void smr_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	sp_stand_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	if( pl_w->team_type == 3 ){
		pl_w->atck_no = pl02_at_vc_smr ;
		HIT_RESET( pl_w ) ;
	} else {
		pl_w->atck_no = pl_w->atck_level ? 0x32 : 0x33 ;
		HIT_RESET( pl_w ) ;
		pl_gauge_add( pl_w, 5 ) ;
	}
	pl_dir_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 2 ) ;
	smr_start( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void smr_start( PLAYER_WORK *pl_w )
{
	JUMP_SPD *spd ;

	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		spd = smr_spd ;
		spd += pl_w->atck_level ;
		pl_w->spd.x = spd->spd_x ;
		pl_w->acc.x = spd->acc_x ;
		pl_w->spd.y = spd->spd_y ;
		pl_w->acc.y = spd->acc_y ;
		if( !pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		pl_air_set( pl_w ) ;
		smr_jump( pl_w ) ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void smr_jump( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_prm ){
		pl_w->cg_prm = 0 ;
		shl02_02_set( pl_w ) ;
		shl02_06_set( pl_w, 0 ) ;
	}
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		RATE_CLR_X( pl_w ) ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 4 ) ;
		smr_land( pl_w ) ;
		return ;
	}
	if( pl_w->spd.y < 0.0f ){
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void smr_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void jsmr_init( PLAYER_WORK *pl_w );
static void jsmr_start( PLAYER_WORK *pl_w );
static void jsmr_jump( PLAYER_WORK *pl_w );
static void jsmr_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The Somersault Kick in the air						*/
/*==============================================================*/
static void sp_jsmr( PLAYER_WORK *pl_w )
{
	static void ( *jsmr_jmp[])( PLAYER_WORK *pl_w ) = {
		jsmr_init, jsmr_start, jsmr_jump, jsmr_land,
	} ;

	jsmr_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void jsmr_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl_w->atck_level ? 0x32 : 0x33 ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->timer0 = 0 ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 6 ) ;
	jsmr_start( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void jsmr_start( PLAYER_WORK *pl_w )
{
	JUMP_SPD *spd ;

	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		spd = smr_spd ;
		spd += pl_w->atck_level ;
		pl_w->spd.x = spd->spd_x ;
		pl_w->acc.x = spd->acc_x ;
		pl_w->spd.y = spd->spd_y ;
		pl_w->acc.y = spd->acc_y ;
		pl_w->acc.x += -0.052083332f ;
		if( !pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		jsmr_jump( pl_w ) ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void jsmr_jump( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_prm ){
		shl02_02_set( pl_w ) ;
		shl02_06_set( pl_w, 0 ) ;
		pl_w->cg_prm = 0 ;
	}
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		RATE_CLR_X( pl_w ) ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 4 ) ;
		jsmr_land( pl_w ) ;
		return ;
	}
	/* rising */
	if( pl_w->spd.y > 0.0f ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		pl_char_move( pl_w ) ;
	} else {
		if( pl_char_move( pl_w ) < 0 ){
			pl_jump_ret( pl_w ) ;
			return ;
		}
		if( pl_w->spd.x <= 0.0f ){
			RATE_CLR_X( pl_w ) ;
		}
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
}

/*--------------------------------------------------------------*/
static void jsmr_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void strike_init( PLAYER_WORK *pl_w );
static void strike_wait( PLAYER_WORK *pl_w );
static void strike_move( PLAYER_WORK *pl_w );
static void strike_up( PLAYER_WORK *pl_w );
static void strike_air( PLAYER_WORK *pl_w );
static void strike_fall( PLAYER_WORK *pl_w );
static void strike_land( PLAYER_WORK *pl_w );
static void strike_hit( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo: a rush with several hits, then a		*/
/*			kick up into the air								*/
/*==============================================================*/
static void sp_strike( PLAYER_WORK *pl_w )
{
	static void ( *strike_jmp[])( PLAYER_WORK *pl_w ) = {
		strike_init, strike_wait, strike_move, strike_move, strike_up, strike_air, strike_fall, strike_land,
	} ;

	strike_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void strike_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	sp_stand_set( pl_w ) ;
	pl_w->atck_no = pl02_at_strike ;
	HIT_RESET( pl_w ) ;
	pl_w->timer0 = 0 ;
	pl_w->spd.x = pl_w->pl_dir ? DOT_X( 5.0f ) : -DOT_X( 5.0f ) ;
	pl_sp_ef_set( pl_w ) ;
	shl02_02_set( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 1 ) ;
}

/*--------------------------------------------------------------*/
static void strike_wait( PLAYER_WORK *pl_w )
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
		ofs.x = -DOT_X( 40.0f ) ;
		ofs.y = DOT_Y( 27.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void strike_move( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) >= 0 ){
		strike_hit( pl_w ) ;
		if( HI_BYTE( pl_w->cg_rival ) & 1 ) pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->cg_flag1 ){
			pl_w->cg_flag1 = 0 ;
			shl02_06_set( pl_w, 1 ) ;
		}
		return ;
	}
	if( pl_w->routine_2 == 2 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 5.0f ) : -DOT_X( 5.0f ) ;
		pl_w->atck_no = pl02_at_strike ;
		HIT_RESET( pl_w ) ;
		pl_w->timer0 = 0 ;
		pl_sp_ef_set( pl_w ) ;
		shl02_02_set( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 2 ) ;
	} else {
		pl_w->routine_2 ++ ;
		pl_w->atck_no = pl02_at_strike_end ;
		HIT_RESET( pl_w ) ;
		pl_w->timer0 = 0 ;
		if( pl_w->pl_dir ){
			pl_w->spd.x = DOT_X( 16.0f ) ;
			pl_w->acc.x = -1.25f ;
		} else {
			pl_w->spd.x = -DOT_X( 16.0f ) ;
			pl_w->acc.x = 1.25f ;
		}
		pl_w->spd.y = DOT_Y( 16.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.5f ) ;
		shl02_06_set( pl_w, 1 ) ;
		pl_char_set( pl_w, Char_Super, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void strike_up( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	strike_hit( pl_w ) ;
	if( HI_BYTE( pl_w->cg_rival ) & 1 ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_sp_ef_set( pl_w ) ;
		shl02_06_set( pl_w, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void strike_air( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	/* stop once the rush has run out */
	if( ( pl_w->pl_dir && pl_w->spd.x < 0.0f ) || ( !pl_w->pl_dir && pl_w->spd.x > 0.0f ) ){
		RATE_CLR_X( pl_w ) ;
	}
	if( HI_BYTE( pl_w->cg_rival ) >= 0 ){
		pl_char_move( pl_w ) ;
		strike_hit( pl_w ) ;
	}
	if( pl_w->spd.y < 0.0f ){
		pl_w->routine_2 ++ ;
		RATE_CLR_X( pl_w ) ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_char_set( pl_w, Char_Super, 4 ) ;
	}
}

/*--------------------------------------------------------------*/
static void strike_fall( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( HI_BYTE( pl_w->cg_rival ) >= 0 ) pl_char_move( pl_w ) ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 5 ) ;
	}
}

/*--------------------------------------------------------------*/
static void strike_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			Each hit of the rush, up to three					*/
/*==============================================================*/
static void strike_hit( PLAYER_WORK *pl_w )
{
	static Uint8 strike_atck[] = { 0x35, 0x35, 0x35, 0x35, 0x36, 0x36, 0x36 } ;

	if( !pl_w->cg_type ) return ;
	if( pl_w->hit_flag ) pl_w->timer0 ++ ;
	if( pl_w->timer0 >= 3 ) return ;
	pl_w->atck_no = strike_atck[pl_w->cg_type >> 1] ;
	HIT_RESET( pl_w ) ;
}

static void hurricane_init( PLAYER_WORK *pl_w );
static void hurricane_wait( PLAYER_WORK *pl_w );
static void hurricane_move( PLAYER_WORK *pl_w );
static void hurricane_keep( PLAYER_WORK *pl_w );
static void hurricane_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo: the Sonic Hurricane, held for two	*/
/*			seconds												*/
/*==============================================================*/
static void sp_hurricane( PLAYER_WORK *pl_w )
{
	static void ( *hurricane_jmp[])( PLAYER_WORK *pl_w ) = {
		hurricane_init, hurricane_wait, hurricane_move, hurricane_keep, hurricane_end,
	} ;

	hurricane_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void hurricane_init( PLAYER_WORK *pl_w )
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
	sp_stand_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = pl02_at_hurricane ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_w->type = 120 ;
	pl_w->timer0 = 0 ;
	pl_char_set( pl_w, Char_Super, 0 ) ;
	hurricane_wait( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void hurricane_wait( PLAYER_WORK *pl_w )
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
		ofs = pl_w->pos ;
		ofs.x = DOT_X( 2.0f ) ;
		ofs.y = 150.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void hurricane_move( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->push_ng_timer = 2 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_char_set( pl_w, Char_Super, 9 ) ;
		pl_w->routine_2 ++ ;
		return ;
	}
	if( pl_w->cg_type ){
		shl02_01_set( pl_w, 0 ) ;
		pl_w->cg_type = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void hurricane_keep( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->push_ng_timer = 2 ;
	if( -- pl_w->type == 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Super, 10 ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void hurricane_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void snapback_init( PLAYER_WORK *pl_w );
static void snapback_move( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A special move that sets off an effect in front		*/
/*			of Guile											*/
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
	sp_stand_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = pl02_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 5 ) ;
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
		ofs.x = 40.0f ;
		ofs.y = DOT_Y( 80.0f ) ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
}

static void chouhatsu_init( PLAYER_WORK *pl_w );
static void chouhatsu_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A move done standing on the ground					*/
/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w )
{
	static void ( *chouhatsu_jmp[])( PLAYER_WORK *pl_w ) = {
		chouhatsu_init, chouhatsu_end,
	} ;

	chouhatsu_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void chouhatsu_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_char_set( pl_w, Char_Extra, 5 ) ;
	pl_voice_req( pl_w, 17 ) ;
}

/*--------------------------------------------------------------*/
static void chouhatsu_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void crossfire_init( PLAYER_WORK *pl_w, PL02_LOCAL_W *lw );
static void crossfire_wait( PLAYER_WORK *pl_w, PL02_LOCAL_W *lw );
static void crossfire_dash( PLAYER_WORK *pl_w, PL02_LOCAL_W *lw );
static void crossfire_catch( PLAYER_WORK *pl_w, PL02_LOCAL_W *lw );
static void crossfire_end( PLAYER_WORK *pl_w, PL02_LOCAL_W *lw );
static void crossfire_miss( PLAYER_WORK *pl_w, PL02_LOCAL_W *lw );
static void crossfire_miss_end( PLAYER_WORK *pl_w, PL02_LOCAL_W *lw );
static void crossfire_kick_init( PLAYER_WORK *pl_w, PL02_LOCAL_W *lw );
static void crossfire_kick_start( PLAYER_WORK *pl_w, PL02_LOCAL_W *lw );
static void crossfire_kick_jump( PLAYER_WORK *pl_w, PL02_LOCAL_W *lw );
static void crossfire_kick_land( PLAYER_WORK *pl_w, PL02_LOCAL_W *lw );

/*==============================================================*/
/*			A hyper combo: a dash that catches the opponent,	*/
/*			then a Somersault Kick								*/
/*==============================================================*/
static void sp_crossfire( PLAYER_WORK *pl_w )
{
	static void ( *crossfire_jmp[])( PLAYER_WORK *pl_w, PL02_LOCAL_W *lw ) = {
		crossfire_init, crossfire_wait, crossfire_dash, crossfire_catch, crossfire_end, crossfire_miss, crossfire_miss_end,
		crossfire_kick_init, crossfire_kick_start, crossfire_kick_jump, crossfire_kick_land,
	} ;

	crossfire_jmp[pl_w->routine_2]( pl_w, ( PL02_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void crossfire_init( PLAYER_WORK *pl_w, PL02_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl02_at_crossfire ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_AIR ;
	em = pl_w->target_w ;
	lw->em_type = em->pl_type ;
	pl_w->timer0 = 16 ;
	pl_w->sub_type = 0 ;
	pl_w->spd.x = pl_w->pl_dir ? 20.0f : -20.0f ;
	pl_char_set( pl_w, Char_Super, 6 ) ;
}

/*--------------------------------------------------------------*/
static void crossfire_wait( PLAYER_WORK *pl_w, PL02_LOCAL_W *lw )
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
		ofs.x = -DOT_X( 7.0f ) ;
		ofs.y = DOT_Y( 76.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void crossfire_dash( PLAYER_WORK *pl_w, PL02_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->hit_flag ){
		if( pl_sp_catch_ck( pl_w ) ){
			/* caught: lift the opponent */
			pl_w->routine_2 ++ ;
			em = pl_w->em_w ;
			em->pos.y = pl_w->pos.y ;
			if( PL_CK( em, PL_SERVBOT ) ) em->pos.y = pl_w->pos.y + 52.5f ;
			if( PL_CK( em, PL_ROLL ) ) em->pos.y = pl_w->pos.y + 52.5f ;
			em->pl_stance = STANCE_AIR ;
			pl_w->spd.x /= 16.0f ;
			pl_char_set( pl_w, Char_Super, 7 ) ;
		} else {
			pl_w->timer0 = 1 ;
			pl_w->routine_2 = 5 ;
			pl_w->delay_flag = 0 ;
			pl_w->delay_timer = 0 ;
			pl_w->hyper_bg = 0 ;
			pl_w->hyper_bg_tm = 0 ;
			pl_w->spd.x = DOT_X( 2.5f ) ;
			if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = 0.0f ;
			pl_w->spd.y = 15.0f ;
			pl_w->acc.y = -DOT_Y( 0.5f ) ;
			pl_w->jump_dir = 1 ;
			pl_char_set( pl_w, Char_Extra, 6 ) ;
		}
		return ;
	}
	if( -- pl_w->timer0 == 0 ){
		pl_w->routine_2 ++ ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_voice_req( pl_w, 43 ) ;
		pl_w->spd.x /= 2.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = -2.142857f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_char_set( pl_w, Char_Jump, 9 ) ;
		pl_w->sub_type = 1 ;
	}
}

/*--------------------------------------------------------------*/
static void crossfire_catch( PLAYER_WORK *pl_w, PL02_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	em = pl_w->target_w ;
	if( em->pl_type != lw->em_type ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_jump_ret( pl_w ) ;
		return ;
	}
	if( pl_w->sub_type == 1 ){
		pl_char_move( pl_w ) ;
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->pos.y < pl_w->ground_y ){
			pl_w->pos.y = pl_w->ground_y ;
			pl_w->routine_2 = 4 ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_w->pl_stance = STANCE_STAND ;
			pl_land_ef_set( pl_w ) ;
			pl_char_set( pl_w, Char_Jump, 3 ) ;
		}
	} else if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x /= 4.0f ;
		pl_w->spd.y = DOT_Y( 8.0f ) ;
		pl_w->acc.y = -2.142857f ;
		pl_w->routine_2 = 7 ;
	} else {
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		/* stay on the screen */
		if( pl_w->pos.x < scr_work.scr_left + -DOT_X( 16.0f ) ) pl_w->pos.x = scr_work.scr_left + -80.0f ;
		if( pl_w->pos.x > scr_work.scr_right + DOT_X( 16.0f ) ) pl_w->pos.x = scr_work.scr_left + 80.0f ;
		/* carry the opponent along */
		if( pl_w->sub_type == 1 ){
		} else if( pl_w->pl_dir ) em->pos.x = pl_w->pos.x + 100.0f ;
		else em->pos.x = pl_w->pos.x + -100.0f ;
		if( pl_w->cg_flag1 ){
			pl_w->atck_no = pl_w->cg_flag1 ;
			HIT_RESET( pl_w ) ;
			pl_w->cg_flag1 = 0 ;
		}
	}
}

/*--------------------------------------------------------------*/
static void crossfire_end( PLAYER_WORK *pl_w, PL02_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void crossfire_miss( PLAYER_WORK *pl_w, PL02_LOCAL_W *lw )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, 7 ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void crossfire_miss_end( PLAYER_WORK *pl_w, PL02_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void crossfire_kick_init( PLAYER_WORK *pl_w, PL02_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->atck_no = pl_w->atck_level ? 0x4e : 0x4e ;
	HIT_RESET( pl_w ) ;
	pl_w->timer0 = 0 ;
	pl_char_set( pl_w, Char_Special, 7 ) ;
	jsmr_start( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void crossfire_kick_start( PLAYER_WORK *pl_w, PL02_LOCAL_W *lw )
{
	JUMP_SPD *spd ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		spd = smr_spd ;
		spd += pl_w->atck_level ;
		pl_w->spd.x = spd->spd_x ;
		pl_w->acc.x = spd->acc_x ;
		pl_w->spd.y = spd->spd_y ;
		pl_w->acc.y = spd->acc_y ;
		if( !pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		jsmr_jump( pl_w ) ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void crossfire_kick_jump( PLAYER_WORK *pl_w, PL02_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_w->cg_prm ){
		shl02_02_set( pl_w ) ;
		shl02_06_set( pl_w, 2 ) ;
		pl_w->cg_prm = 0 ;
	}
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 4 ) ;
		jsmr_land( pl_w ) ;
		return ;
	}
	if( pl_w->spd.y < 0.0f ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		if( !pl_w->timer0 ){
			pl_char_set( pl_w, Char_Special, 8 ) ;
			pl_w->timer0 = 1 ;
		}
		RATE_CLR_X( pl_w ) ;
	} else {
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
	}
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void crossfire_kick_land( PLAYER_WORK *pl_w, PL02_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static PLAYER_WORK *nml_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *crouch_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *jump_catch_ck( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Can a throw start?									*/
/****************************************************************/
PLAYER_WORK *pl02_catch_ck( PLAYER_WORK *pl_w )
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

	if( !( pl_w->dir = ( pl_w->atck_lever & ( KEY_DOWN | KEY_FORWARD | KEY_BACK ) ) >> 10 ) ) return 0 ;
	if( pl_w->atck_pk == 0 && pl_w->atck_level == 1 && pl_w->pos.y > DOT_Y( 64.0f ) ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 2 ;
			return em ;
		}
		return 0 ;
	}
	if( pl_w->atck_pk == 1 && pl_w->atck_level == 1 && pl_w->pos.y > DOT_Y( 64.0f ) ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 3 ;
			return em ;
		}
		return 0 ;
	}
	return 0 ;
}

static void nml_p_set( PLAYER_WORK *pl_w );
static void nml_k_set( PLAYER_WORK *pl_w );
static void jump_p_set( PLAYER_WORK *pl_w );
static void jump_k_set( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Start the throw that pl02_catch_ck chose			*/
/****************************************************************/
void	pl02_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_set, nml_k_set, jump_p_set, jump_k_set,
	} ;

	catch_set_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( !( pl_w->dir & 1 ) ){
		pl_w->pl_dir ^= 1 ;
		pl_w->disp_dir = pl_w->pl_dir ;
	}
	scr_excep_set( pl_w, 5, 5 ) ;
	ofs.x = -DOT_X( 68.0f ) ;
	ofs.y = 195.0f ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 0 ) ;
}

/*==============================================================*/
static void nml_k_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( !( pl_w->dir & 2 ) ){
		pl_w->pl_dir ^= 1 ;
		pl_w->disp_dir = pl_w->pl_dir ;
	}
	scr_excep_set( pl_w, 5, 5 ) ;
	ofs.x = DOT_X( 20.0f ) ;
	ofs.y = 195.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 1 ) ;
}

/*==============================================================*/
static void jump_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( !( pl_w->dir & 2 ) ){
		pl_w->pl_dir ^= 1 ;
		pl_w->disp_dir = pl_w->pl_dir ;
	}
	scr_excep_set( pl_w, 5, 5 ) ;
	ofs.x = DOT_X( 52.0f ) ;
	ofs.y = 300.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 2 ) ;
}

/*==============================================================*/
static void jump_k_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( !( pl_w->dir & 2 ) ){
		pl_w->pl_dir ^= 1 ;
		pl_w->disp_dir = pl_w->pl_dir ;
	}
	scr_excep_set( pl_w, 5, 5 ) ;
	ofs.x = -80.0f ;
	ofs.y = DOT_Y( 86.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 3 ) ;
}

static void nml_p_catch( PLAYER_WORK *pl_w );
static void nml_k_catch( PLAYER_WORK *pl_w );
static void jump_p_catch( PLAYER_WORK *pl_w );
static void jump_k_catch( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void	pl02_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_catch, nml_k_catch, jump_p_catch, jump_k_catch,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->pl_stance = STANCE_STAND ;
		SCR_EXCEP_SET ;
		em->atck_no = pl02_at_throw_p ;
		em->pl_dir = pl_w->pl_dir ;
	}
}

/*==============================================================*/
static void nml_k_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 11 ;
		em->pl_stance = STANCE_STAND ;
		SCR_EXCEP_SET ;
		em->atck_no = pl02_at_throw_k ;
		em->pl_dir = pl_w->pl_dir ;
		catch_se_req( pl_w ) ;
	}
}

/*==============================================================*/
static void jump_p_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->pl_stance = STANCE_AIR ;
		scr_excep_clr() ;
		SCR_EXCEP_SET ;
		em->atck_no = pl02_at_air_throw_p ;
		em->pl_dir = pl_w->pl_dir ^ 1 ;
	}
}

/*==============================================================*/
static void jump_k_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	if( pl_w->routine_2 == 0 ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( !pl_w->cg_type ) pl_char_move( pl_w ) ;
		if( pl_w->pos.y <= pl_w->ground_y ){
			if( pl_w->routine_3 == 0 && pl_w->pos.y <= DOT_Y( 160.0f ) ) pl_w->routine_3 ++ ;
			pl_w->routine_2 ++ ;
			pl_char_set( pl_w, Char_Catch, 4 ) ;
			pl_w->pos.y = pl_w->ground_y ;
			pl_w->pl_stance = STANCE_STAND ;
			catch_se_req( pl_w ) ;
			pl_se_req( pl_w, 2 ) ;
		}
	} else {
		if( pl_char_move( pl_w ) >= 0 ){
			if( pl_w->cg_type < 0 ){
				pl_w->cg_type = 0 ;
				em = pl_w->catch_w ;
				em->atck_no = pl02_at_air_throw_k_hit ;
				catch_damage( pl_w ) ;
				ofs.x = -DOT_X( 32.0f ) ;
				ofs.y = DOT_Y( 32.0f ) ;
				ofs.z = 0.0f ;
				hit_mark_set( pl_w, &ofs, 1 ) ;
			} else if( pl_w->cg_type > 0 ){
				pl_w->cg_type = 0 ;
				em = pl_w->catch_w ;
				em->dm_hit_w = pl_w ;
				em->catch_type = 1 ;
				em->atck_no = pl02_at_air_throw_k ;
			}
			return ;
		}
		pl_stand_ret( pl_w ) ;
	}
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_move( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A kick that leaps off the wall						*/
/****************************************************************/
void	pl02_t_kick( PLAYER_WORK *pl_w )
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
	if( pl_w->pl_dir ) pl_w->spd.x = 30.0f ;
	else pl_w->spd.x = -30.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 2.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->atck_no = pl02_at_t_kick ;
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
	if( pl_char_move( pl_w ) < 0 ){
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_stop_stand_ret( pl_w ) ;
	} else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
	}
}

static void hold( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Holding the opponent								*/
/****************************************************************/
void	pl02_hold( PLAYER_WORK *pl_w )
{
	static void ( *hold_jmp[])( PLAYER_WORK *pl_w ) = {
		hold, hold, hold, hold,
	} ;

	hold_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void hold( PLAYER_WORK *pl_w )
{
	catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void	pl02_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl02_sp_strike ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl02_sp_hurricane ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl02_sp_hurricane ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl02_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl02_sp_strike ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl02_sp_hurricane ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl02_sp_hurricane ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl02_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl02_sp_smr ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl02_sp_sonic ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl02_sp_sonic ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void	pl02_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl02_sp_smr ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl02_sp_sonic ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl02_sp_smr ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl02.c" End of File	*/

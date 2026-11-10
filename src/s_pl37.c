/* Jin (character 37): command checks, normal attacks, appearance, win and lose poses, special
   moves and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl37_def.h"

void pl37_main( PLAYER_WORK *pl_w );
void pl37_com_ck( PLAYER_WORK *pl_w );
void pl37_atck_set( PLAYER_WORK *pl_w );
void pl37_atck( PLAYER_WORK *pl_w );
void pl37_dash( PLAYER_WORK *pl_w );
void pl37_noki( PLAYER_WORK *pl_w );
void pl37_appear( PLAYER_WORK *pl_w );
void pl37_win( PLAYER_WORK *pl_w );
void pl37_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl37_catch_ck( PLAYER_WORK *pl_w );
void pl37_catch_set( PLAYER_WORK *pl_w );
void pl37_catch( PLAYER_WORK *pl_w );
void pl37_hold( PLAYER_WORK *pl_w );
int pl37_delay_ck( PLAYER_WORK *pl_w );
void pl37_t_kick( PLAYER_WORK *pl_w );
void pl37_upper( PLAYER_WORK *pl_w );
void pl37_sp_unison( PLAYER_WORK *pl_w );
void pl37_sp_gattai( PLAYER_WORK *pl_w );
void pl37_sp_assist( PLAYER_WORK *pl_w );
void pl37_sp_zero( PLAYER_WORK *pl_w );
void pl37_tlocal_init( PLAYER_WORK *pl_w );

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
static Uint16 com_typhoon[] = { 40, COM_GROUND, pl37_sp_typhoon, 0, KEY_BACK, KEY_FORWARD, KEY_PUNCH } ;	/* Saotome Typhoon */
static Uint16 com_dynamite[] = { 40, COM_GROUND, pl37_sp_dynamite, 0, KEY_DOWN, KEY_UP, KEY_PUNCH } ;	/* Saotome Dynamite */
static Uint16 com_vulcan[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl37_sp_vulcan, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Blodia Vulcan (hyper) */
static Uint16 com_blodia[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl37_sp_blodia, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Blodia Punch (hyper) */
static Uint16 com_cyclone[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl37_sp_cyclone, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Saotome Cyclone (hyper) */
static Uint16 com_crush[] = { 5, COM_GROUND, pl37_sp_crush, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Saotome Crash */

static void ( *pl37_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl37_main,
	pl37_com_ck,
	pl37_atck_set,
	pl37_atck,
	pl37_dash,
	pl37_noki,
	pl37_appear,
	pl37_win,
	pl37_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl37_catch_ck,	/* returns a value */
	pl37_catch_set,
	pl37_catch,
	pl37_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl37_delay_ck,
	pl37_t_kick,
	pl37_upper,
	pl37_sp_unison,
	pl37_sp_gattai,
	pl37_sp_assist,
	pl37_sp_zero,
	0,
	0,
	0,
	pl37_tlocal_init,
} ;

static Uint8 typhoon_ck( PLAYER_WORK *pl_w );
static Uint8 dynamite_ck( PLAYER_WORK *pl_w );
static Uint8 vulcan_ck( PLAYER_WORK *pl_w );
static Uint8 blodia_ck( PLAYER_WORK *pl_w );
static Uint8 cyclone_ck( PLAYER_WORK *pl_w );
static Uint8 crush_ck( PLAYER_WORK *pl_w );
static int chouhatsu_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static Uint8 vulcan_delay_ck( PLAYER_WORK *pl_w );
static Uint8 blodia_delay_ck( PLAYER_WORK *pl_w );
static Uint8 cyclone_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl37_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl37_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl37_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( vulcan_ck( pl_w ) ) return ;
	if( blodia_ck( pl_w ) ) return ;
	if( cyclone_ck( pl_w ) ) return ;
	if( typhoon_ck( pl_w ) ) return ;
	if( dynamite_ck( pl_w ) ) return ;
	if( crush_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
/*			Charged back, then forward							*/
/*==============================================================*/
static Uint8 typhoon_ck( PLAYER_WORK *pl_w )
{
	if( !com_tame_ck( pl_w, com_typhoon, &pl_w->com_w ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl37_sp_typhoon ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			Charged down, then up								*/
/*==============================================================*/
static Uint8 dynamite_ck( PLAYER_WORK *pl_w )
{
	if( !com_tame_ck( pl_w, com_dynamite, &pl_w->com_w0 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl37_sp_dynamite ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 vulcan_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_vulcan, &pl_w->com_w1 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl37_sp_vulcan ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 blodia_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_blodia, &pl_w->com_w2 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl37_sp_blodia ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 cyclone_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_cyclone, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl37_sp_cyclone ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 crush_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_crush, &pl_w->com_w4 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w4 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl37_sp_crush ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 8 ) ) return 0 ;
	pl_w->pl_sp_type = pl37_sp_chouhatsu ;
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
	pl_w->pl_sp_type = pl37_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl37_delay_ck( PLAYER_WORK *pl_w )
{
	if( vulcan_delay_ck( pl_w ) || blodia_delay_ck( pl_w ) || cyclone_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static Uint8 vulcan_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_vulcan, &pl_w->com_w1 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl37_sp_vulcan ;
	return 1 ;
}

/*==============================================================*/
static Uint8 blodia_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_blodia, &pl_w->com_w2 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl37_sp_blodia ;
	return 1 ;
}

/*==============================================================*/
static Uint8 cyclone_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_cyclone, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl37_sp_cyclone ;
	return 1 ;
}

/****************************************************************/
/*			Once a round, low on vitality, a hyper combo of		*/
/*			its own												*/
/****************************************************************/
void pl37_main( PLAYER_WORK *pl_w )
{
	PL37_LOCAL_W *lw = ( PL37_LOCAL_W * )pl_w->tlocal ;

	if( lw->burning_end_flag || lw->burning_flag ) return ;
	if( pl_w->vital_new > 50 ) return ;
	if( vs_sys_work.ko_cnt[pl_w->pl_no] < 2 ) return ;
	if( !sp_ok_ck( pl_w ) ) return ;
	if( pl_w->pl_stance == STANCE_AIR ) return ;
	if( pl_w->pl_stance == STANCE_DOWN ) return ;
	if( vs_sys_work.vs_st == VS_END_WAIT || vs_sys_work.vs_st == VS_WIN ) return ;
	if( pl_w->team_no ) return ;
	lw->burning_flag = 1 ;
	lw->burning_col = 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl37_sp_shine ;
	act_st_set( pl_w, N_SUPER ) ;
	pls37_00_set2( pl_w, 16, 0 ) ;
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
void pl37_atck_set( PLAYER_WORK *pl_w )
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
		pl_se_req( pl_w, SE_SWING_H ) ;
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
		pl_se_req( pl_w, SE_SWING_H ) ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Crouch_P, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
/*			The heavy kick leaps forward						*/
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
		pl_w->chain_tbl = mk_tbl ;
		pl_w->nml_level = 1 ;
		pl_w->routine_2 = 0 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_STAND_HK ;
		pl_w->chain_tbl = hk_tbl ;
		pl_w->nml_level = 2 ;
		pl_se_req( pl_w, SE_SWING_H ) ;
		pl_w->spd.x = -6.25f ;
		pl_w->acc.x = 0.104166664f ;
		pl_w->spd.y = DOT_Y( 9.5f ) ;
		pl_w->acc.y = -1.0379463f ;
		if( pl_w->disp_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		pl_w->routine_2 = 0 ;
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
		pl_w->routine_2 = 0 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_CROUCH_HK ;
		pl_w->chain_tbl = hk_tbl ;
		pl_w->nml_level = 2 ;
		pl_w->routine_2 = 0 ;
		pl_voice_req( pl_w, 28 ) ;
		pl_se_req( pl_w, SE_SWING_H ) ;
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
/*			The heavy kick held down, high enough: a dive		*/
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
		pl_w->sjump_flag = 0 ;
		pl_w->jump_atk_cnt &= JATK_P_MASK ;
		if( ( pl_w->atck_lever & KEY_DOWN ) && pl_w->pos.y > pl_w->ground_y + DOT_Y( 64.0f ) ){
			pl_char_set( pl_w, Char_Extra, 2 ) ;
			pl_w->atck_no = pl37_at_jdown_hk ;
			pl_w->routine_2 = 1 ;
			pl_w->timer0 = 2 ;
			pl_w->timer1 = 2 ;
		} else {
			pl_char_set( pl_w, Char_Jump_K, 2 ) ;
			pl_w->atck_no = ATCK_JUMP_HK ;
			pl_w->routine_2 = 0 ;
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
static void stand_lk_move( PLAYER_WORK *pl_w );
static void stand_mk_start( PLAYER_WORK *pl_w );
static void stand_mk_move( PLAYER_WORK *pl_w );
static void stand_mk( PLAYER_WORK *pl_w );
static void stand_hk_start( PLAYER_WORK *pl_w );
static void stand_hk_move( PLAYER_WORK *pl_w );
static void stand_hk_land( PLAYER_WORK *pl_w );
static void stand_hk( PLAYER_WORK *pl_w );
static void crouch_kick_move( PLAYER_WORK *pl_w );
static void crouch_lk_move( PLAYER_WORK *pl_w );
static void crouch_mk_start( PLAYER_WORK *pl_w );
static void crouch_mk_move( PLAYER_WORK *pl_w );
static void crouch_mk( PLAYER_WORK *pl_w );
static void crouch_hk_start( PLAYER_WORK *pl_w );
static void crouch_hk_slide( PLAYER_WORK *pl_w );
static void crouch_hk_end( PLAYER_WORK *pl_w );
static void crouch_hk( PLAYER_WORK *pl_w );
static void sjump_move( PLAYER_WORK *pl_w );
static void jump_move( PLAYER_WORK *pl_w );
static void jump_punch_move( PLAYER_WORK *pl_w );
static void jump_kick_move( PLAYER_WORK *pl_w );
static void jump_lk_move( PLAYER_WORK *pl_w );
static void jump_mk_move( PLAYER_WORK *pl_w );
static void jump_hk_move( PLAYER_WORK *pl_w );
static void jump_hk_dive( PLAYER_WORK *pl_w );
static void jump_hk( PLAYER_WORK *pl_w );

/****************************************************************/
void pl37_atck( PLAYER_WORK *pl_w )
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
/*			The heavy punch steps in, and back when it ends		*/
/*==============================================================*/
static void stand_punch_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	case 2:
		pl_smoke_set( pl_w ) ;
		if( pl_char_move( pl_w ) < 0 ){
			if( !pl_w->disp_dir ) pl_w->pos.x += -DOT_X( 40.0f ) ;
			else pl_w->pos.x += DOT_X( 40.0f ) ;
			pl_stand_ret( pl_w ) ;
		} else {
			if( pl_w->cg_type & 1 ){
				pl_w->cg_type &= 0xfe ;
				pl_w->spd.x = -DOT_X( 10.0f ) ;
				pl_w->acc.x = 0.83335876f ;
				if( pl_w->disp_dir ){
					pl_w->spd.x = -pl_w->spd.x ;
					pl_w->acc.x = -pl_w->acc.x ;
				}
			}
			if( pl_w->cg_type & 2 ){
				pl_w->cg_type &= 0xfd ;
				pls37_00_set( pl_w, 9, 2 ) ;
			}
			if( pl_w->spd.x * pl_w->acc.x > 0.0f ) RATE_CLR_X( pl_w ) ;
		}
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
	static void ( *stand_kick_jmp[])( PLAYER_WORK *pl_w ) = {
		stand_lk_move, stand_mk, stand_hk,
	} ;

	stand_kick_jmp[pl_w->nml_no]( pl_w ) ;
}

/*==============================================================*/
/*			The animation moves him back and forth				*/
/*==============================================================*/
static void stand_lk_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		if( pl_w->disp_dir ) pl_w->cg_type = -pl_w->cg_type ;
		pl_w->pos.x -= pl_w->cg_type ;
		pl_w->cg_type = 0 ;
	}
}

/*==============================================================*/
/*			A hop												*/
/*==============================================================*/
static void stand_mk_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->spd.x = -DOT_X( 5.75f ) ;
		pl_w->acc.x = 0.104166664f ;
		pl_w->spd.y = DOT_Y( 5.5f ) ;
		pl_w->acc.y = -0.6361607f ;
		if( pl_w->disp_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
	}
}

/*--------------------------------------------------------------*/
static void stand_mk_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type & 2 ){
		pl_w->cg_type &= 0xfd ;
		pls37_00_set( pl_w, 9, 8 ) ;
	}
	if( pl_w->cg_type & 4 ){
		pl_w->cg_type &= 0xfb ;
		pls37_00_set( pl_w, 9, 9 ) ;
	}
	if( pl_w->cg_type & 8 ){
		pl_w->cg_type &= 0xf7 ;
		pl_w->atck_no = pl37_at_stand_mk_2 ;
		HIT_RESET( pl_w ) ;
	}
	if( pl_w->ground_y >= pl_w->pos.y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
static void stand_mk( PLAYER_WORK *pl_w )
{
	static void ( *stand_mk_jmp[])( PLAYER_WORK *pl_w ) = {
		stand_mk_start, stand_mk_move,
	} ;

	stand_mk_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			A jump forward and a stamp							*/
/*==============================================================*/
static void stand_hk_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = -15.0f ;
		pl_w->acc.x = 0.104166664f ;
		pl_w->spd.y = -DOT_Y( 4.5f ) ;
		pl_w->acc.y = -0.13392857f ;
		if( pl_w->disp_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
	}
}

/*--------------------------------------------------------------*/
static void stand_hk_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->ground_y >= pl_w->pos.y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->routine_2 ++ ;
		pl_w->spd.x = -DOT_X( 5.5f ) ;
		pl_w->acc.x = 0.2734375f ;
		if( pl_w->disp_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		pl_char_set( pl_w, Char_Stand_K, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void stand_hk_land( PLAYER_WORK *pl_w )
{
	pl_smoke_set( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
static void stand_hk( PLAYER_WORK *pl_w )
{
	static void ( *stand_hk_jmp[])( PLAYER_WORK *pl_w ) = {
		stand_hk_start, stand_hk_move, stand_hk_land,
	} ;

	stand_hk_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void crouch_kick_move( PLAYER_WORK *pl_w )
{
	static void ( *crouch_kick_jmp[])( PLAYER_WORK *pl_w ) = {
		crouch_lk_move, crouch_mk, crouch_hk,
	} ;

	crouch_kick_jmp[pl_w->nml_no]( pl_w ) ;
}

/*==============================================================*/
static void crouch_lk_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			A hop												*/
/*==============================================================*/
static void crouch_mk_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = -6.25f ;
		pl_w->acc.x = 0.104166664f ;
		pl_w->spd.y = DOT_Y( 7.5f ) ;
		pl_w->acc.y = -0.9040178f ;
		if( pl_w->disp_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
	}
}

/*--------------------------------------------------------------*/
static void crouch_mk_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type & 2 ){
		pl_w->cg_type &= 0xfd ;
		pls37_00_set( pl_w, 9, 0 ) ;
	}
	if( pl_w->cg_type & 4 ){
		pl_w->cg_type &= 0xfb ;
		pls37_00_set( pl_w, 9, 1 ) ;
	}
	if( pl_w->cg_type & 8 ){
		pl_w->cg_type &= 0xf7 ;
		pl_w->atck_no = pl37_at_crouch_mk_2 ;
		HIT_RESET( pl_w ) ;
	}
	if( pl_w->ground_y >= pl_w->pos.y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
static void crouch_mk( PLAYER_WORK *pl_w )
{
	static void ( *crouch_mk_jmp[])( PLAYER_WORK *pl_w ) = {
		crouch_mk_start, crouch_mk_move,
	} ;

	crouch_mk_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			A slide												*/
/*==============================================================*/
static void crouch_hk_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = -15.0f ;
		pl_w->acc.x = 0.4427083f ;
		if( pl_w->disp_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		pl_w->timer0 = 24 ;
	}
}

/*--------------------------------------------------------------*/
static void crouch_hk_slide( PLAYER_WORK *pl_w )
{
	pl_smoke_set( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( !pl_w->timer0 -- ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Crouch_K, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void crouch_hk_end( PLAYER_WORK *pl_w )
{
	pl_smoke_set( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
static void crouch_hk( PLAYER_WORK *pl_w )
{
	static void ( *crouch_hk_jmp[])( PLAYER_WORK *pl_w ) = {
		crouch_hk_start, crouch_hk_slide, crouch_hk_end,
	} ;

	crouch_hk_jmp[pl_w->routine_2]( pl_w ) ;
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
	static void ( *jump_kick_jmp[])( PLAYER_WORK *pl_w ) = {
		jump_lk_move, jump_mk_move, jump_hk,
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
	else if( pl_w->cg_type & 2 ){
		pl_w->cg_type &= 0xfd ;
		pls37_00_set( pl_w, 9, 5 ) ;
	}
}

/*==============================================================*/
static void jump_hk_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
	else {
		if( pl_w->cg_type & 2 ){
			pl_w->cg_type &= 0xfd ;
			pls37_00_set( pl_w, 9, 6 ) ;
		}
		if( pl_w->cg_type & 4 ){
			pl_w->cg_type &= 0xfb ;
			pls37_00_set( pl_w, 9, 7 ) ;
		}
		if( pl_w->cg_type & 8 ){
			pl_w->cg_type &= 0xf7 ;
			pl_w->atck_no = pl37_at_jump_hk_2 ;
			HIT_RESET( pl_w ) ;
		}
	}
}

/*==============================================================*/
/*			The dive: three hits on the way down				*/
/*==============================================================*/
static void jump_hk_dive( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type & 1 ){
		pl_w->cg_type &= 0xfe ;
		pl_w->spd.x = -DOT_X( 7.0f ) ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = -DOT_Y( 6.0f ) ;
		pl_w->acc.y = 0.0f ;
		if( pl_w->disp_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
	}
	if( pl_w->hit_flag && !pl_w->timer1 -- ){
		pl_w->timer1 = 2 ;
		if( pl_w->timer0 ){
			pl_w->timer0 -- ;
			pl_w->atck_no = pl37_at_jdown_hk ;
			HIT_RESET( pl_w ) ;
		}
	}
}

/*==============================================================*/
static void jump_hk( PLAYER_WORK *pl_w )
{
	static void ( *jump_hk_jmp[])( PLAYER_WORK *pl_w ) = {
		jump_hk_move, jump_hk_dive,
	} ;

	jump_hk_jmp[pl_w->routine_2]( pl_w ) ;
}

static void upper_init( PLAYER_WORK *pl_w );
static void upper_move( PLAYER_WORK *pl_w );
static void upper_k_start( PLAYER_WORK *pl_w );
static void upper_k_move( PLAYER_WORK *pl_w );
static void upper_kick( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The launcher: a punch standing, a kick crouching	*/
/*==============================================================*/
static void upper_init( PLAYER_WORK *pl_w )
{
	pl_brake_set( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	if( !pl_w->atck_pk ){
		pl_w->routine_2 = 1 ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Extra, 9 ) ;
		pl_w->atck_no = pl37_at_upper ;
		HIT_RESET( pl_w ) ;
		pl_se_req( pl_w, SE_SWING_M ) ;
	} else {
		pl_w->routine_2 = 2 ;
		pl_w->routine_3 = 0 ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_char_set( pl_w, Char_Extra, 10 ) ;
		pl_w->atck_no = pl37_at_upper_k ;
		HIT_RESET( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void upper_move( PLAYER_WORK *pl_w )
{
	if( pl_w->atck_type == 3 ) pl_smoke_set( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_brake_stop( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void upper_k_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = -6.25f ;
		pl_w->acc.x = 0.104166664f ;
		pl_w->spd.y = DOT_Y( 7.5f ) ;
		pl_w->acc.y = -0.9040178f ;
		if( pl_w->disp_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
	}
}

/*--------------------------------------------------------------*/
static void upper_k_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type & 2 ){
		pl_w->cg_type &= 0xfd ;
		pls37_00_set( pl_w, 9, 0 ) ;
	}
	if( pl_w->cg_type & 4 ){
		pl_w->cg_type &= 0xfb ;
		pls37_00_set( pl_w, 9, 1 ) ;
	}
	if( pl_w->cg_type & 8 ){
		pl_w->cg_type &= 0xf7 ;
		pl_w->atck_no = pl37_at_upper_k_2 ;
		HIT_RESET( pl_w ) ;
	}
	if( pl_w->ground_y >= pl_w->pos.y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void upper_kick( PLAYER_WORK *pl_w )
{
	static void ( *upper_kick_jmp[])( PLAYER_WORK *pl_w ) = {
		upper_k_start, upper_k_move,
	} ;

	if( pl_w->atck_type == 3 ) pl_smoke_set( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_brake_stop( pl_w ) ;
	upper_kick_jmp[pl_w->routine_3]( pl_w ) ;
}

/****************************************************************/
void pl37_upper( PLAYER_WORK *pl_w )
{
	static void ( *upper_jmp[])( PLAYER_WORK *pl_w ) = {
		upper_init, upper_move, upper_kick,
	} ;

	upper_jmp[pl_w->routine_2]( pl_w ) ;
}

static void dash_init( PLAYER_WORK *pl_w );
static void dash_move( PLAYER_WORK *pl_w );
static void dash_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The dash: a slide forward							*/
/****************************************************************/
void pl37_dash( PLAYER_WORK *pl_w )
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
		pl_w->spd.x = -DOT_X( 10.0f ) ;
		pl_w->acc.x = 0.33854166f ;
		if( pl_w->disp_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		pl_w->timer0 = 14 ;
		pl_voice_req( pl_w, 3 ) ;
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
		pl_w->spd.x = -DOT_X( 5.5f ) ;
		pl_w->acc.x = 0.2734375f ;
		if( pl_w->disp_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		pl_char_set( pl_w, Char_Dash, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void dash_end( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void noki_init( PLAYER_WORK *pl_w );
static void noki_move( PLAYER_WORK *pl_w );
static void noki_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash: a hop back							*/
/****************************************************************/
void pl37_noki( PLAYER_WORK *pl_w )
{
	static void ( *noki_jmp[])( PLAYER_WORK *pl_w ) = {
		noki_init, noki_move, noki_land,
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
		noki_move( pl_w ) ;
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
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !( pl_w->spd.x * pl_w->acc.x < 0.0f ) ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			The appearance: he drops in from above the			*/
/*			screen and lands where he stood						*/
/*==============================================================*/
static void appear_init( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	lw->land_x = pl_w->pos.x ;
	lw->appear_flag = 1 ;
	pl_w->disp_flag = 1 ;
	if( !pl_w->disp_dir ) pl_w->pos.x += DOT_X( 104.0f ) ;
	else pl_w->pos.x -= DOT_X( 104.0f ) ;
	pl_w->pos.y += 480.0f ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.x = -4.4791665f ;
	pl_w->spd.y = -DOT_Y( 2.5f ) ;
	if( pl_w->disp_dir ) pl_w->spd.x = -pl_w->spd.x ;
	pl_char_set( pl_w, Char_Appear, 0 ) ;
	pls37_00_set( pl_w, 0, 0 ) ;
}

/*--------------------------------------------------------------*/
static void appear_fall( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->ground_y + DOT_Y( 96.0f ) >= pl_w->pos.y ){
		pl_w->pos.y = pl_w->ground_y + DOT_Y( 96.0f ) ;
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 64 ;
	}
}

/*--------------------------------------------------------------*/
static void appear_wait( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.5739339f ;
		pl_w->acc.x = 0.0390625f ;
		pl_w->spd.y = DOT_Y( 7.625f ) ;
		pl_w->acc.y = -0.9040178f ;
		if( pl_w->disp_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		pl_char_set( pl_w, Char_Appear, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_jump( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type & 1 ) return ;
	if( pl_w->cg_type & 2 ){
		pl_w->cg_type &= 0xfd ;
		lw->appear_ef_flag = 1 ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->ground_y >= pl_w->pos.y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pos.x = lw->land_x ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Jump, 5 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_end( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		lw->appear_flag = 0 ;
		pl_w->routine_1 ++ ;
	}
}

/****************************************************************/
void pl37_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw ) = {
		appear_init, appear_fall, appear_wait, appear_jump, appear_end,
	} ;

	appear_jmp[pl_w->routine_2]( pl_w, ( PL37_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
/*			The KO win poses: a button held picks one, and		*/
/*			against Abyss it is pose 0 or 1						*/
/*==============================================================*/
static void win_ko_init( PLAYER_WORK *pl_w )
{
	Uint8 no ;
	PLAYER_WORK *em ;

	no = Rnd() & 3 ;
	if( !pl_w->cpu_flag ){
		if( sw_work[pl_w->pl_no].now & KEY_LP ) no = 0 ;
		else if( sw_work[pl_w->pl_no].now & KEY_HP ) no = 1 ;
		else if( sw_work[pl_w->pl_no].now & KEY_A1 ) no = 2 ;
	}
	em = pl_w->target_w ;
	if( em->pl_type == PL_ABYSS1 || em->pl_type == PL_ABYSS2 || em->pl_type == PL_ABYSS3 ) no = Rnd() & 1 ;
	switch( no ){
	case 0:
		pl_char_set( pl_w, Char_Win, 0 ) ;
		pl_w->sub_type = 0 ;
		pl_w->timer0 = 60 ;
		break ;
	case 1:
		pl_char_set( pl_w, Char_Win, 3 ) ;
		pl_w->sub_type = 1 ;
		break ;
	default:
		pl_char_set( pl_w, Char_Stand, 2 ) ;
		pl_w->sub_type = 2 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = -5.0f ;
		if( pl_w->disp_dir ) pl_w->spd.x = -pl_w->spd.x ;
		break ;
	}
	pl_w->routine_2 ++ ;
	pl_w->routine_3 = 0 ;
}

/*==============================================================*/
static void win0_move( PLAYER_WORK *pl_w )
{
	if( !pl_w->timer0 -- ){
		/* nothing is done when the time is up */
	}
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
/*			Pose 1: he jumps									*/
/*==============================================================*/
static void win1_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		pl_w->timer0 = 32 ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->sjump_cnt = 0 ;
		pl_w->sjump_flag = 0 ;
		pl_w->jump_com_cnt = 0 ;
		pl_w->jump_dash_cnt = 0 ;
		pl_w->jump_atk_cnt = JATK_ONE ;
		pl_w->air_jump_cnt = 0 ;
		pl_w->spd.x = -10.0f ;
		pl_w->acc.x = DOT_X( 0.125f ) ;
		pl_w->spd.y = DOT_Y( 4.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.125f ) ;
		if( pl_w->disp_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
	}
}

/*--------------------------------------------------------------*/
static void win1_jump( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !pl_w->timer0 -- ) pl_w->routine_3 ++ ;
}

/*--------------------------------------------------------------*/
static void win1_end( PLAYER_WORK *pl_w )
{
}

/*==============================================================*/
static void win1_move( PLAYER_WORK *pl_w )
{
	static void ( *win1_jmp[])( PLAYER_WORK *pl_w ) = {
		win1_start, win1_jump, win1_end,
	} ;

	win1_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
/*			Pose 2: he walks a little way, then poses			*/
/*==============================================================*/
static void win2_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->routine_3 ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		if( pl_w->em_dist <= DOT_X( 32.0f ) ){
			pl_w->routine_3 ++ ;
			pl_char_set( pl_w, Char_Win, 1 ) ;
		}
	}
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
void pl37_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko, lose_time_ko, win_time, lose_time_ko, draw_time,
	} ;

	if( pl_win_end_ck( pl_w ) ){
		if( pl_w->pl_stance == STANCE_AIR ) pl_jump_ret( pl_w ) ;
		else pl_stand_ret( pl_w ) ;
	} else win_jmp[pl_w->type]( pl_w ) ;
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_move( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/*==============================================================*/
static void t_kick_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->spd.x = -30.0f ;
	if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 0.125f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->atck_no = pl37_at_t_kick ;
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
		pls37_00_set( pl_w, 5, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void t_kick_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stop_stand_ret( pl_w ) ;
}

/****************************************************************/
/*			The throw's follow-up kick							*/
/****************************************************************/
void pl37_t_kick( PLAYER_WORK *pl_w )
{
	static void ( *t_kick_jmp[])( PLAYER_WORK *pl_w ) = {
		t_kick_init, t_kick_move, t_kick_land,
	} ;

	t_kick_jmp[pl_w->routine_2]( pl_w ) ;
}

static void sp_typhoon( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw );
static void sp_dynamite( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw );
static void sp_vulcan( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw );
static void sp_blodia( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw );
static void sp_cyclone( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw );
static void sp_crush( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw );
static void sp_shine( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw );
static void sp_chouhatsu( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw );
static void sp_snapback( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw );
static void sp_punch( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw );

/****************************************************************/
/*			Special moves										*/
/****************************************************************/
void pl37_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *pl_sp_tbl[])( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw ) = {
		sp_typhoon, sp_dynamite, sp_vulcan, sp_blodia, sp_cyclone, sp_crush, sp_typhoon, sp_shine, sp_chouhatsu, sp_snapback, sp_punch,
	} ;

	PL37_LOCAL_W *lw = ( PL37_LOCAL_W * )pl_w->tlocal ;

	pl_sp_tbl[pl_w->pl_sp_type]( pl_w, lw ) ;
}

/* sp_typhoon's speed and its change, in 1/65536ths, for each strength */
static Sint32 typhoon_spd_tbl[4] = {
	-0x50000, 0x1800,
	-0xb0000, 0x3000,
} ;

static void typhoon_wait( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw );

/*==============================================================*/
/*			Saotome Typhoon: a dash that hits again and again	*/
/*==============================================================*/
static void typhoon_init( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
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
	lw->hit_wait_flag = 0 ;
	lw->hit_timer = 0 ;
	lw->unk_02 = 0 ;
	lw->atck_alt = 0 ;
	pl_w->atck_no = pl_w->atck_level * 2 + 48 ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level ) ;
	pl_w->timer0 = pl_w->cg_type ;
	typhoon_wait( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void typhoon_wait( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 2 ) ;
		pl_w->timer0 = pl_w->cg_type ;
		pl_w->spd.x = typhoon_spd_tbl[pl_w->atck_level * 2] * 1.6666666f / 65536.0f ;
		pl_w->acc.x = typhoon_spd_tbl[pl_w->atck_level * 2 + 1] * 1.6666666f / 65536.0f ;
		if( pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
	}
}

/*==============================================================*/
/*			The hits alternate between two attacks				*/
/*==============================================================*/
static void typhoon_dash( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ) return ;
	if( pl_w->cg_prm ){
		pl_w->cg_prm = 0 ;
		lw->unk_02 = 1 ;
		pls37_00_set( pl_w, 7, 0 ) ;
	}
	if( lw->hit_timer && !-- lw->hit_timer ){
		lw->hit_wait_flag = 0 ;
		lw->hit_timer = 0 ;
		if( lw->atck_alt ^= 1 ){
			pl_w->atck_no = pl_w->atck_level * 2 + 48 ;
			HIT_RESET( pl_w ) ;
		} else {
			pl_w->atck_no = pl_w->atck_level * 2 + 78 ;
			HIT_RESET( pl_w ) ;
		}
	}
	if( pl_w->hit_flag && !lw->hit_wait_flag ){
		lw->hit_wait_flag = 1 ;
		lw->hit_timer = 8 ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( !pl_w->pl_dir ){
		if( pl_w->wall_flag == WALL_LEFT ) pl_w->timer0 = 0 ;
	} else if( pl_w->wall_flag == WALL_RIGHT ) pl_w->timer0 = 0 ;
	if( !pl_w->timer0 -- ){
		pl_w->routine_2 ++ ;
		lw->unk_02 = 0 ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 4 ) ;
		pls37_00_set( pl_w, 1, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void typhoon_end( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
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
static void sp_typhoon( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	static void ( *typhoon_jmp[])( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw ) = {
		typhoon_init, typhoon_wait, typhoon_dash, typhoon_end,
	} ;

	typhoon_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

static void dynamite_move( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw );
static void dynamite_flash( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw );
static void dynamite_color( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw );

/*==============================================================*/
/*			Saotome Dynamite: a rising attack and its sparks	*/
/*==============================================================*/
static void dynamite_init( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
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
	pl_w->atck_no = pl_w->atck_level * 2 + 51 ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 6 ) ;
	pl_w->timer0 = 16 ;
	dynamite_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void dynamite_move( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type & 1 ){
		pl_w->cg_type &= 0xfe ;
		dynamite_color( pl_w, lw ) ;
		pls37_00_set( pl_w, 2, 0 ) ;
	}
	if( pl_w->cg_type & 2 ){
		pl_w->cg_type &= 0xfd ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pls37_00_set( pl_w, 3, 0 ) ;
	}
	if( pl_w->cg_type & 0x10 ){
		pl_w->cg_type &= 0xef ;
		shl37_00_set( pl_w, 0, 0 ) ;
		shl37_00_set( pl_w, 0, 1 ) ;
		shl37_00_set( pl_w, 0, 2 ) ;
		shl37_00_set( pl_w, 0, 3 ) ;
		shl37_00_set( pl_w, 0, 4 ) ;
		shl37_00_set( pl_w, 0, 5 ) ;
		shl37_00_set( pl_w, 0, 6 ) ;
		shl37_00_set( pl_w, 0, 7 ) ;
		shl37_00_set( pl_w, 0, 8 ) ;
		shl37_00_set( pl_w, 0, 9 ) ;
	}
	if( pl_w->cg_type & 8 ){
		pl_w->cg_type &= 0xf7 ;
		lw->col_step = 0 ;
		lw->col_timer = 0 ;
		pls37_00_set( pl_w, 6, 0 ) ;
	}
	if( pl_w->cg_type & 4 ){
		dynamite_flash( pl_w, lw ) ;
		if( !-- pl_w->timer0 ){
			pl_w->routine_2 ++ ;
			pl_color_set( pl_w, PC_DEFAULT ) ;
			pl_char_set( pl_w, Char_Special, pl_w->atck_level + 8 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void dynamite_end( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_dynamite( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	static void ( *dynamite_jmp[])( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw ) = {
		dynamite_init, dynamite_move, dynamite_end,
	} ;

	dynamite_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/*==============================================================*/
/*			The colour brightens step by step, unless sp_shine		*/
/*			is on												*/
/*==============================================================*/
static void dynamite_flash( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	if( !lw->burning_flag && !lw->col_timer -- ){
		lw->col_timer = 4 ;
		ef_color_set( pl_w, 0, pl_w->pl_color * 6 + lw->col_step + 36, 1 ) ;
		if( lw->col_step < 4 ) lw->col_step ++ ;
	}
}

/*==============================================================*/
static void dynamite_color( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	if( !lw->burning_flag ) ef_color_set( pl_w, 0, pl_w->pl_color * 6 + 41, 1 ) ;
}

static void vulcan_start( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw );
static void vulcan_hit_ck( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw );
static void vulcan_stand_ck( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw );
static void vulcan_shot( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw );
static void vulcan_spark( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw );
static void vulcan_flame( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw );

/*==============================================================*/
/*			Blodia Vulcan: the hyper combo that rises with the	*/
/*			opponent											*/
/*==============================================================*/
static void vulcan_init( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
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
	lw->beam_flag = 0 ;
	lw->catch_flag = 0 ;
	lw->hit_wait_flag = lw->hit_timer = 0 ;
	lw->burn_flag = 0 ;
	lw->stand_hit_flag = 0 ;
	lw->flame_no = 0 ;
	pl_w->atck_no = pl37_at_vulcan ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 0 ) ;
	vulcan_start( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void vulcan_start( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type & 2 ){
		pl_w->cg_type &= 0xfd ;
		pls37_00_set2( pl_w, 13, 0 ) ;
	}
	if( pl_w->cg_type & 1 ){
		pl_w->routine_2 ++ ;
		ofs.x = -5.0f ;
		ofs.y = DOT_Y( 87.0f ) ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
		pl_w->ef_w = shl37_01_set( pl_w, 0, 0 ) ;
		if( !pl_w->ef_w ){
			pl_stand_ret( pl_w ) ;
			return ;
		}
		lw->beam_flag = 1 ;
		scr_excep_set( pl_w, 1, 1 ) ;
		pl_w->timer0 = 80 ;
	}
}

/*==============================================================*/
/*			Missed, or hit in the air: he rises with the		*/
/*			beam and lands										*/
/*==============================================================*/
static void vulcan_rise( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->timer0 -- ;
	vulcan_shot( pl_w, lw ) ;
	vulcan_stand_ck( pl_w, lw ) ;
	if( !lw->stand_hit_flag ) vulcan_spark( pl_w, lw ) ;
	else vulcan_flame( pl_w, lw ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		lw->rise_x = pl_w->pos.x ;
		pl_w->spd.y = DOT_Y( 8.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.125f ) ;
		pl_char_set( pl_w, Char_Super, 21 ) ;
	}
}

/*--------------------------------------------------------------*/
static void vulcan_up( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->timer0 -- ;
	vulcan_shot( pl_w, lw ) ;
	vulcan_stand_ck( pl_w, lw ) ;
	if( !lw->stand_hit_flag ) vulcan_spark( pl_w, lw ) ;
	else vulcan_flame( pl_w, lw ) ;
	if( !pl_w->cg_type ){
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Super, 22 ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
}

/*--------------------------------------------------------------*/
static void vulcan_ride( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	PLAYER_WORK *ef ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->timer0 -- ;
	vulcan_shot( pl_w, lw ) ;
	vulcan_stand_ck( pl_w, lw ) ;
	if( !lw->stand_hit_flag ) vulcan_spark( pl_w, lw ) ;
	else vulcan_flame( pl_w, lw ) ;
	ef = pl_w->ef_w ;
	pl_w->pos.x = ef->pos.x ;
	pl_w->pos.y = ef->pos.y ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void vulcan_fire( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	vulcan_shot( pl_w, lw ) ;
	vulcan_spark( pl_w, lw ) ;
	vulcan_hit_ck( pl_w, lw ) ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_3 ++ ;
		pl_w->disp_flag = 1 ;
		lw->beam_flag = 0 ;
		pl_char_set( pl_w, Char_Super, 25 ) ;
	}
}

/*--------------------------------------------------------------*/
static void vulcan_fall( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->pos.x = lw->rise_x ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_char_set( pl_w, Char_Super, 26 ) ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void vulcan_end( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	if( pl_w->cg_type ) pl_w->cg_type = 0 ;
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void vulcan_miss( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	static void ( *vulcan_miss_jmp[])( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw ) = {
		vulcan_rise, vulcan_up, vulcan_ride, vulcan_fire, vulcan_fall, vulcan_end,
	} ;

	vulcan_miss_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*==============================================================*/
/*			The opponent was caught: the beam burns him			*/
/*==============================================================*/
static void vulcan_burn_init( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->disp_flag = 0 ;
	lw->burn_flag = 1 ;
	shl37_01_set2( pl_w, 4, 0 ) ;
	shl37_01_set2( pl_w, 5, 0 ) ;
	shl37_01_set2( pl_w, 6, 0 ) ;
	shl37_01_set2( pl_w, 6, 1 ) ;
	shl37_01_set2( pl_w, 8, 0 ) ;
	shl37_01_set2( pl_w, 8, 1 ) ;
	ef_color_set( pl_w, 0, pl_w->pl_color * 3 + 72, 3 ) ;
	pl_w->timer0 = 96 ;
	pl_w->routine_3 ++ ;
}

/*--------------------------------------------------------------*/
static void vulcan_burn( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	vulcan_shot( pl_w, lw ) ;
	vulcan_flame( pl_w, lw ) ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_3 ++ ;
		lw->beam_flag = 0 ;
		lw->burn_flag = 0 ;
		pl_w->disp_flag = 1 ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_char_set( pl_w, Char_Super, 25 ) ;
	}
}

/*--------------------------------------------------------------*/
static void vulcan_burn_fall( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Super, 29 ) ;
		pl_w->pos.x = lw->rise_x ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void vulcan_burn_end( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	if( pl_w->cg_type ) pl_w->cg_type = 0 ;
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void vulcan_hit( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	static void ( *vulcan_hit_jmp[])( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw ) = {
		vulcan_burn_init, vulcan_burn, vulcan_burn_fall, vulcan_burn_end,
	} ;

	vulcan_hit_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void vulcan_move( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	static void ( *vulcan_move_jmp[])( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw ) = {
		vulcan_miss, vulcan_hit,
	} ;

	vulcan_move_jmp[lw->catch_flag]( pl_w, lw ) ;
}

/*==============================================================*/
static void sp_vulcan( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	static void ( *vulcan_jmp[])( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw ) = {
		vulcan_init, vulcan_start, vulcan_move,
	} ;

	vulcan_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/*==============================================================*/
/*			The opponent is caught if he is hit standing on		*/
/*			the ground											*/
/*==============================================================*/
static void vulcan_hit_ck( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	if( !pl_w->hit_flag ) return ;
	if( pl_w->hit_flag & HR_GUARD ) return ;
	if( pl_w->hit_flag & HR_ARMOR ) return ;
	em = pl_w->em_w ;
	if( em->work_kind != W_PL ) return ;
	if( em->team_no ) return ;
	if( ABYSS_CK( em ) ) return ;
	if( em->routine_1 != PL_DAMAGE ) return ;
	lw->catch_flag = 1 ;
	pl_w->routine_3 = 0 ;
	lw->stand_hit_flag = 1 ;
	pl_w->timer0 = 100 ;
}

/*==============================================================*/
static void vulcan_stand_ck( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	if( !pl_w->hit_flag ) return ;
	if( pl_w->hit_flag & HR_GUARD ) return ;
	if( pl_w->hit_flag & HR_ARMOR ) return ;
	em = pl_w->em_w ;
	if( em->work_kind != W_PL ) return ;
	if( em->team_no ) return ;
	if( ABYSS_CK( em ) ) return ;
	if( em->routine_1 != PL_DAMAGE ) return ;
	lw->stand_hit_flag = 1 ;
}

/*==============================================================*/
static void vulcan_shot( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	if( !( pl_w->timer0 & 1 ) ) shl37_01_set2( pl_w, 2, 0 ) ;
	if( !( pl_w->timer0 & 15 ) ) pl_voice_req( pl_w, 31 ) ;
}

/*==============================================================*/
static void vulcan_spark( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	if( !( pl_w->timer0 & 3 ) ) shl37_01_set2( pl_w, 3, 0 ) ;
}

/*==============================================================*/
static void vulcan_flame( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	if( !( pl_w->timer0 & 7 ) ){
		shl37_01_set2( pl_w, 7, lw->flame_no ) ;
		lw->flame_no ^= 1 ;
	}
}

static void blodia_start( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw );

/*==============================================================*/
/*			Blodia Punch: a hyper combo, a dash behind a beam	*/
/*==============================================================*/
static void blodia_init( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
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
	lw->shot_end_flag = 0 ;
	pl_w->atck_no = pl37_at_blodia ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 8 ) ;
	blodia_start( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void blodia_start( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type & 2 ) pl_w->cg_type &= 0xfd ;
	if( pl_w->cg_type & 1 ){
		pl_w->cg_type &= 0xfe ;
		ofs.x = DOT_X( 7.0f ) ;
		ofs.y = 165.0f ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
		if( !shl37_02_set( pl_w, 0, 0 ) ){
			scr_excep_set( pl_w, 0, 0 ) ;
			pl_stand_ret( pl_w ) ;
			return ;
		}
		scr_excep_set( pl_w, 1, 1 ) ;
		pl_w->timer0 = 4 ;
		pl_w->routine_2 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void blodia_wait( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->timer0 -- ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Super, 9 ) ;
	}
}

/*--------------------------------------------------------------*/
static void blodia_dash( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_w->cg_type & 1 ) pl_w->cg_type &= 0xfe ;
	if( pl_w->cg_type & 2 ) pl_w->cg_type &= 0xfd ;
	if( lw->shot_end_flag ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 12 ;
	}
}

/*--------------------------------------------------------------*/
static void blodia_hold( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->timer0 -- ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Super, 18 ) ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void blodia_end( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		if( !pl_w->disp_dir ) pl_w->pos.x -= DOT_X( 40.0f ) ;
		else pl_w->pos.x += DOT_X( 40.0f ) ;
		scr_excep_set( pl_w, 0, 0 ) ;
		pl_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
static void sp_blodia( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	static void ( *blodia_jmp[])( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw ) = {
		blodia_init, blodia_start, blodia_wait, blodia_dash, blodia_hold, blodia_end,
	} ;

	blodia_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

static void cyclone_start( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw );
static void cyclone_spin( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw );

/*==============================================================*/
/*			Saotome Cyclone: the hyper combo that spins			*/
/*			forward and catches the opponent					*/
/*==============================================================*/
static void cyclone_init( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
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
	lw->spin_end_flag = 0 ;
	lw->unk_02 = 0 ;
	pl_w->atck_no = pl37_at_cyclone ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 11 ) ;
	cyclone_start( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void cyclone_start( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_3 ++ ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = 80.0f ;
		ofs.y = 120.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void cyclone_wait( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Super, 12 ) ;
		pl_w->timer0 = pl_w->cg_type ;
	}
}

/*--------------------------------------------------------------*/
static void cyclone_spin_start( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_3 ++ ;
		pl_w->spd.x = -1.6666666f ;
		if( pl_w->disp_dir ) pl_w->spd.x = -pl_w->spd.x ;
		shl37_03_set( pl_w, 0, 0 ) ;
		shl37_03_set( pl_w, 0, 1 ) ;
		shl37_03_set( pl_w, 0, 2 ) ;
		shl37_03_set( pl_w, 1, 0 ) ;
		lw->unk_02 = 1 ;
		pl_w->timer1 = 16 ;
		lw->sway_dir = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void cyclone_spin_move( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	cyclone_spin( pl_w, lw ) ;
	if( !( pl_w->timer0 & 15 ) ){
		pl_w->atck_no = pl37_at_cyclone ;
		HIT_RESET( pl_w ) ;
	}
	if( !pl_w->timer0 -- ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Super, 13 ) ;
		lw->spin_end_flag = 1 ;
		lw->unk_02 = 0 ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		return ;
	}
	if( pl_sp_catch_ck( pl_w ) ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->catch_no = 4 ;
		pl_w->catch_no |= CATCH_ESC_NG ;
		pl_w->catch_no |= CATCH_ESC2_NG ;
		em = pl_w->em_w ;
		pl_sp_catch_set( pl_w, em ) ;
		em->routine_2 = 0 ;
		em->spd.x = em->spd.y = em->acc.x = em->acc.y = 0.0f ;
		lw->unk_15 = 0 ;
		pl_w->timer0 = 80 ;
	}
}

/*--------------------------------------------------------------*/
static void cyclone_end( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
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
static void cyclone_move( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	static void ( *cyclone_move_jmp[])( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw ) = {
		cyclone_init, cyclone_start, cyclone_wait, cyclone_spin_start, cyclone_spin_move, cyclone_end,
	} ;

	cyclone_move_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*==============================================================*/
/*			Caught: he spins with the opponent, then throws		*/
/*			him													*/
/*==============================================================*/
static void cyclone_hold( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	cyclone_spin( pl_w, lw ) ;
	if( !pl_w->timer0 -- ){
		pl_w->routine_3 ++ ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->atck_no = pl37_at_cyclone_throw ;
		pl_w->atck_no = pl37_at_cyclone_throw ;
		em->catch_type = 15 ;
		em->routine_2 = 0 ;
		pl_w->atck_pow = lw->unk_15 / 4 + 32 ;
		pl_char_set( pl_w, Char_Super, 30 ) ;
		pls37_00_set( pl_w, 1, 0 ) ;
		lw->spin_end_flag = 1 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void cyclone_throw( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void cyclone_catch( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	static void ( *cyclone_catch_jmp[])( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw ) = {
		cyclone_hold, cyclone_throw,
	} ;

	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	cyclone_catch_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*==============================================================*/
static void sp_cyclone( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	static void ( *cyclone_jmp[])( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw ) = {
		cyclone_move, cyclone_catch,
	} ;

	cyclone_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/*==============================================================*/
/*			He sways back and forth as he spins					*/
/*==============================================================*/
static void cyclone_spin( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	if( !pl_w->timer1 -- ){
		pl_w->timer1 = 16 ;
		lw->sway_dir ^= 1 ;
		pl_w->spd.x = !lw->sway_dir ? -0.104166664f : 0.104166664f ;
	}
}

static void crush_move( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw );

/*==============================================================*/
/*			Saotome Crash: a dash that grabs the opponent		*/
/*==============================================================*/
static void crush_init( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->spd.x = -DOT_X( 8.0f ) ;
	pl_w->acc.x = DOT_X( 0.375f ) ;
	if( pl_w->disp_dir ){
		pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = -pl_w->acc.x ;
	}
	pl_w->atck_no = pl37_at_crush ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 17 ) ;
	crush_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void crush_move( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type & 1 ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
	}
	if( pl_w->cg_type & 2 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->routine_2 ++ ;
		return ;
	}
	if( pl_sp_catch_ck( pl_w ) ){
		em = pl_w->em_w ;
		pl_w->catch_no = 2 ;
		pl_w->catch_no |= CATCH_ESC_NG ;
		pl_w->catch_no |= CATCH_ESC2_NG ;
		pl_w->routine_2 = pl_w->routine_3 = 0 ;
		em->spd.x = em->spd.y = em->acc.x = em->acc.y = 0.0f ;
		pl_catch_set( pl_w, em ) ;
	}
}

/*--------------------------------------------------------------*/
static void crush_end( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_crush( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	static void ( *crush_jmp[])( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw ) = {
		crush_init, crush_move, crush_end,
	} ;

	crush_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

static void shine_start( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw );

/*==============================================================*/
/*			Saotome Shine: the hyper combo that comes by		*/
/*			itself when his vitality is low						*/
/*==============================================================*/
static void shine_init( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
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
	pl_w->atck_pow = 40 ;
	pl_w->def_pow = 40 ;
	pl_w->atck_no = pl37_at_shine ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 3 ) ;
	shine_start( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void shine_start( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		ofs.x = DOT_X( 5.0f ) ;
		ofs.y = DOT_Y( 93.0f ) ;
		pl_hyper_flash( pl_w, &ofs, 0 ) ;
		pl_w->timer0 = 16 ;
		pl_voice_req( pl_w, 29 ) ;
		pl_se_req( pl_w, 80 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shine_end( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( !pl_w->timer0 -- ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_shine( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	static void ( *shine_jmp[])( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw ) = {
		shine_init, shine_start, shine_end,
	} ;

	shine_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/* sp_chouhatsu's colour for each level, four frames each */
static Uint8 chouhatsu_color_tbl[16] = {
	0, 0, 0, 0,
	1, 2, 1, 2,
	3, 4, 3, 4,
	5, 5, 5, 5,
} ;

static void chouhatsu_charge( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw );
static void chouhatsu_color( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw );

/*==============================================================*/
/*			The taunt: he charges up, a level for each run of	*/
/*			presses												*/
/*==============================================================*/
static void chouhatsu_init( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
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
	lw->charge_level = 0 ;
	lw->press_cnt = 0 ;
	pl_w->timer0 = 60 ;
	pl_w->timer1 = 0 ;
	pl_w->atck_no = pl37_at_chouhatsu ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 4 ) ;
	chouhatsu_charge( pl_w, lw ) ;
	pl_voice_req( pl_w, 45 ) ;
}

/*--------------------------------------------------------------*/
static void chouhatsu_charge( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	VEC3 ofs ;
	Uint8 no ;

	pl_dir_set( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->atck_no = pl37_at_chouhatsu ;
		HIT_RESET( pl_w ) ;
	}
	chouhatsu_color( pl_w, lw ) ;
	if( pl_w->pl_sw_now & KEY_ATCK ) lw->press_cnt ++ ;
	if( !pl_w->timer0 -- ){
		if( lw->press_cnt >= 8 ){
			if( lw->charge_level < 3 ){
				lw->charge_level ++ ;
				pl_char_set( pl_w, Char_Extra, lw->charge_level + 4 ) ;
				switch( lw->charge_level ){
				case 1:
					no = 4 ;
					break ;
				case 2:
					no = 6 ;
					break ;
				case 3:
					no = 20 ;
					break ;
				}
				pl_voice_req( pl_w, no ) ;
			}
			lw->press_cnt = 0 ;
			pl_w->timer0 = 60 ;
		} else pl_w->routine_2 ++ ;
	}
	if( lw->charge_level >= 3 ){
		ofs.x = DOT_X( 32.0f ) ;
		if( pl_w->disp_dir ) ofs.x = -ofs.x ;
		ofs.x += pl_w->pos.x ;
		ofs.y = pl_w->pos.y + DOT_Y( 80.0f ) ;
		ofs.z = 0.0f ;
		if( !( pl_w->timer0 & 15 ) ){
			one_spark_set( &ofs, pl_w->pl_dir ^ 1 ) ;
			pl_se_req( pl_w, 75 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void chouhatsu_release( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	chouhatsu_color( pl_w, lw ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Extra, 8 ) ;
		lw->col_step = 0 ;
		lw->col_timer = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void chouhatsu_end( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	dynamite_flash( pl_w, lw ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
static void chouhatsu_color( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	Uint8 col ;

	if( !lw->burning_flag && lw->charge_level ){
		pl_w->timer1 ++ ;
		col = chouhatsu_color_tbl[lw->charge_level * 4 + ( pl_w->timer1 & 3 )] ;
		ef_color_set( pl_w, 0, pl_w->pl_color * 6 + col + 90, 1 ) ;
	}
}

/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	static void ( *chouhatsu_jmp[])( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw ) = {
		chouhatsu_init, chouhatsu_charge, chouhatsu_release, chouhatsu_end,
	} ;

	chouhatsu_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

static void snapback_move( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw );

/*==============================================================*/
/*			The snapback, with its flash						*/
/*==============================================================*/
static void snapback_init( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
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
	pl_w->atck_no = pl37_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 10 ) ;
	snapback_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void snapback_move( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type & 1 ){
		pl_w->cg_type &= 0xfe ;
		ofs.x = DOT_X( 37.0f ) ;
		ofs.y = DOT_Y( 55.0f ) ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
}

/*==============================================================*/
static void sp_snapback( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	static void ( *snapback_jmp[])( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw ) = {
		snapback_init, snapback_move,
	} ;

	snapback_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

static void punch_end( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw );

/*==============================================================*/
/*			The standing medium punch (gamma assist				*/
/*			and counter)										*/
/*==============================================================*/
static void punch_init( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = ATCK_STAND_MP ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Stand_P, 1 ) ;
	pl_se_req( pl_w, SE_SWING_M ) ;
	punch_end( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void punch_end( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_punch( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	static void ( *punch_jmp[])( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw ) = {
		punch_init, punch_end,
	} ;

	punch_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/*==============================================================*/
/*			The stick toward or away from the opponent and a	*/
/*			heavy button: a kick throw, catch_no 0, or a		*/
/*			punch throw, catch_no 1. In the air only the		*/
/*			punch throw											*/
/*==============================================================*/
static PLAYER_WORK *nml_catch_ck( PLAYER_WORK *pl_w )
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
PLAYER_WORK *pl37_catch_ck( PLAYER_WORK *pl_w )
{
	static PLAYER_WORK *( *catch_ck_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_catch_ck, crouch_catch_ck, jump_catch_ck, crouch_catch_ck,
	} ;

	return catch_ck_jmp[pl_w->pl_stance]( pl_w ) ;
}

/*==============================================================*/
/*			The kick throw: he holds the opponent, who can		*/
/*			mash free											*/
/*==============================================================*/
static void nml_k_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_w->dir & 1 ){
		pl_w->pl_dir ^= 1 ;
		pl_w->disp_dir = pl_w->pl_dir ;
	}
	pl_w->hit_stop = 10 ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 0 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	ofs.x = -85.0f ;
	ofs.y = DOT_Y( 25.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	hold_renda_set( pl_w->catch_w, 16 ) ;
	catch_renda_set( pl_w, 1 ) ;
	pl_w->timer0 = 48 ;
	pl_char_move( pl_w ) ;
	pl_w->catch_w->pos.y = pl_w->catch_w->ground_y ;
}

/*==============================================================*/
/*			The punch throw: he runs the opponent to the		*/
/*			screen's edge										*/
/*==============================================================*/
static void nml_p_set( PLAYER_WORK *pl_w )
{
	PL37_LOCAL_W *lw = ( PL37_LOCAL_W * )pl_w->tlocal ;
	VEC3 ofs ;

	if( pl_w->dir & 1 ){
		pl_w->pl_dir ^= 1 ;
		pl_w->disp_dir = pl_w->pl_dir ;
	}
	pl_w->hit_stop = 10 ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 3 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	lw->edge_flag = 0 ;
	pl_w->spd.x = -15.0f ;
	pl_w->acc.x = -DOT_X( 0.125f ) ;
	if( pl_w->disp_dir ){
		pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = -pl_w->acc.x ;
	}
	ofs.x = -20.0f ;
	ofs.y = 180.0f ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
}

/*==============================================================*/
/*			Saotome Crash's grab								*/
/*==============================================================*/
static void crush_c_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->hit_stop = 10 ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 6 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	ofs.x = -DOT_X( 64.0f ) ;
	ofs.y = DOT_Y( 48.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	scr_excep_set( pl_w, 1, 1 ) ;
}

/*==============================================================*/
/*			The air throw: as the punch throw					*/
/*==============================================================*/
static void jump_p_set( PLAYER_WORK *pl_w )
{
	PL37_LOCAL_W *lw = ( PL37_LOCAL_W * )pl_w->tlocal ;
	VEC3 ofs ;

	if( pl_w->dir & 1 ){
		pl_w->pl_dir ^= 1 ;
		pl_w->disp_dir = pl_w->pl_dir ;
	}
	pl_w->hit_stop = 10 ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 18 ) ;
	lw->edge_flag = 0 ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.x = -15.0f ;
	pl_w->acc.x = -DOT_X( 0.125f ) ;
	if( pl_w->disp_dir ){
		pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = -pl_w->acc.x ;
	}
	ofs.x = -20.0f ;
	ofs.y = DOT_Y( 86.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
}

/*==============================================================*/
/*			Saotome Cyclone's catch: sp_cyclone does it all		*/
/*==============================================================*/
static void cyclone_c_set( PLAYER_WORK *pl_w )
{
}

/****************************************************************/
/*			Start the throw that pl37_catch_ck chose			*/
/****************************************************************/
void pl37_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_k_set, nml_p_set, crush_c_set, jump_p_set, cyclone_c_set,
	} ;

	catch_set_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
/*			The kick throw: mashing shakes him, and when the	*/
/*			time is up he throws								*/
/*==============================================================*/
static void nml_k_mash( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->catch_w ;
	VEC3 ofs ;

	pl_char_move( pl_w ) ;
	if( catch_renda_ck( pl_w ) ) pl_w->cg_ctr = 1 ;
	if( hold_renda_ck( pl_w->catch_w ) ) pl_w->timer0 = 0 ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		ofs.x = -DOT_X( 26.0f ) ;
		ofs.y = DOT_Y( 13.0f ) ;
		catch_hit_mark_set( pl_w, &ofs ) ;
		hit_se_req( em, 2 ) ;
		em->dm_hit_w = pl_w ;
		em->atck_no = pl37_at_throw_k ;
		pl_w->atck_no = pl37_at_throw_k ;
		em->pl_dir = pl_w->pl_dir ;
		em->pl_dir ^= 1 ;
		catch_damage( pl_w ) ;
		pl_w->hit_stop = 8 ;
	}
	if( !pl_w->timer0 -- ){
		em->dm_hit_w = pl_w ;
		em->atck_no = pl37_at_throw_k ;
		pl_w->atck_no = pl37_at_throw_k ;
		em->catch_type = 3 ;
		pl_w->spd.x = DOT_X( 4.75f ) ;
		pl_w->acc.x = -0.052083332f ;
		pl_w->spd.y = DOT_Y( 5.5f ) ;
		pl_w->acc.y = -0.6361607f ;
		if( pl_w->disp_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		pl_char_set( pl_w, Char_Catch, 1 ) ;
		pl_w->routine_2 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void nml_k_fly( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ) return ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->ground_y >= pl_w->pos.y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Catch, 2 ) ;
		pl_land_ef_set( pl_w ) ;
		pl_w->routine_2 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void nml_k_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void nml_k_catch( PLAYER_WORK *pl_w )
{
	static void ( *nml_k_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_k_mash, nml_k_fly, nml_k_end,
	} ;

	nml_k_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			The punch throw										*/
/*==============================================================*/
static void nml_p_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pls37_00_set( pl_w, 15, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void nml_p_run( PLAYER_WORK *pl_w )
{
	PL37_LOCAL_W *lw = ( PL37_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *em ;

	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( !pl_w->disp_dir ){
		if( scr_work.stage_left >= pl_w->pos.x ) lw->edge_flag = 1 ;
	} else {
		if( scr_work.stage_right <= pl_w->pos.x ) lw->edge_flag = 1 ;
	}
	if( lw->edge_flag ){
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->atck_no = pl37_at_throw_p ;
		pl_w->atck_no = pl37_at_throw_p ;
		em->catch_type = 1 ;
		em->pl_dir = pl_w->pl_dir ;
		em->pl_dir ^= 1 ;
		pl_w->hit_stop = 8 ;
		scr_work.quake_level = 3 ;
		scr_work.quake_flag = 1 ;
		pl_voice_req( pl_w, 30 ) ;
		pl_w->routine_2 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void nml_p_jump( PLAYER_WORK *pl_w )
{
	pl_w->spd.x = DOT_X( 5.75f ) ;
	pl_w->acc.x = -0.052083332f ;
	pl_w->spd.y = DOT_Y( 5.5f ) ;
	pl_w->acc.y = -0.6361607f ;
	if( pl_w->disp_dir ){
		pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = -pl_w->acc.x ;
	}
	pl_char_set( pl_w, Char_Catch, 4 ) ;
	pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void nml_p_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ) return ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->ground_y >= pl_w->pos.y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Catch, 5 ) ;
		pl_land_ef_set( pl_w ) ;
		pl_w->routine_2 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void nml_p_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void nml_p_catch( PLAYER_WORK *pl_w )
{
	static void ( *nml_p_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_start, nml_p_run, nml_p_jump, nml_p_fall, nml_p_end,
	} ;

	nml_p_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			Saotome Crash's grab: he dashes across with the		*/
/*			opponent, up the screen and down again, and			*/
/*			throws												*/
/*==============================================================*/
static void crush_c_start( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = -DOT_X( 16.0f ) ;
		if( pl_w->disp_dir ) pl_w->spd.x = -pl_w->spd.x ;
		scr_work.quake_level = 3 ;
		scr_work.quake_flag = 1 ;
		pls37_00_set( pl_w, 8, 0 ) ;
		pl_se_req( pl_w, 41 ) ;
	}
}

/*--------------------------------------------------------------*/
static void crush_c_run( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( !pl_w->disp_dir ){
		if( pl_w->wall_flag != WALL_LEFT ) return ;
	} else {
		if( pl_w->wall_flag != WALL_RIGHT ) return ;
	}
	pl_w->routine_2 ++ ;
	pl_w->wall_ng_timer = 3 ;
	pl_w->spd.y = 30.0f ;
	pl_char_set( pl_w, Char_Catch, 9 ) ;
	scr_work.quake_level = 3 ;
	scr_work.quake_flag = 1 ;
	pl_se_req( pl_w, 47 ) ;
	pls37_00_set( pl_w, 8, 1 ) ;
}

/*--------------------------------------------------------------*/
static void crush_c_up( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	pl_w->wall_ng_timer = 3 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( scr_work.scr_top <= pl_w->pos.y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y + 480.0f ;
		pl_char_set( pl_w, Char_Catch, 11 ) ;
		pl_w->spd.x = DOT_X( 16.0f ) ;
		if( pl_w->disp_dir ) pl_w->spd.x = -pl_w->spd.x ;
		scr_work.quake_level = 3 ;
		scr_work.quake_flag = 1 ;
		pl_se_req( pl_w, 47 ) ;
		pls37_00_set( pl_w, 8, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void crush_c_back( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_w->disp_dir ){
		if( pl_w->wall_flag != WALL_LEFT ) return ;
	} else {
		if( pl_w->wall_flag != WALL_RIGHT ) return ;
	}
	pl_w->routine_2 ++ ;
	pl_w->wall_ng_timer = 3 ;
	pl_w->spd.y = -30.0f ;
	pl_char_set( pl_w, Char_Catch, 13 ) ;
	scr_work.quake_level = 3 ;
	scr_work.quake_flag = 1 ;
	pl_se_req( pl_w, 47 ) ;
	pls37_00_set( pl_w, 8, 3 ) ;
}

/*--------------------------------------------------------------*/
static void crush_c_down( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	pl_w->wall_ng_timer = 3 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->ground_y >= pl_w->pos.y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_char_set( pl_w, Char_Catch, 7 ) ;
		pl_w->spd.x = -DOT_X( 16.0f ) ;
		if( pl_w->disp_dir ) pl_w->spd.x = -pl_w->spd.x ;
		scr_work.quake_level = 3 ;
		scr_work.quake_flag = 1 ;
		pl_se_req( pl_w, 47 ) ;
		pls37_00_set( pl_w, 8, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void crush_c_stop( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( !pl_w->disp_dir ){
		if( pl_w->wall_flag != WALL_LEFT ) return ;
	} else {
		if( pl_w->wall_flag != WALL_RIGHT ) return ;
	}
	pl_w->routine_2 ++ ;
	pl_char_set( pl_w, Char_Catch, 8 ) ;
	pl_se_req( pl_w, 47 ) ;
}

/*--------------------------------------------------------------*/
static void crush_c_throw( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	if( pl_char_move( pl_w ) < 0 ){
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->atck_no = pl37_at_crush_throw ;
		pl_w->atck_no = pl37_at_crush_throw ;
		em->catch_type = 1 ;
		scr_work.quake_level = 3 ;
		scr_work.quake_flag = 1 ;
		pl_se_req( pl_w, 48 ) ;
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Catch, 15 ) ;
		pl_w->spd.x = DOT_X( 5.75f ) ;
		pl_w->acc.x = -0.052083332f ;
		pl_w->spd.y = DOT_Y( 5.5f ) ;
		pl_w->acc.y = -0.6361607f ;
		if( pl_w->disp_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		SCR_EXCEP_SET ;
	}
}

/*--------------------------------------------------------------*/
static void crush_c_fall( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	pl_w->push_ng_timer = 1 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ) return ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->ground_y >= pl_w->pos.y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Catch, 16 ) ;
		pl_land_ef_set( pl_w ) ;
		pl_w->routine_2 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void crush_c_end( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void crush_c_catch( PLAYER_WORK *pl_w )
{
	static void ( *crush_c_jmp[])( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw ) = {
		crush_c_start, crush_c_run, crush_c_up, crush_c_back, crush_c_down, crush_c_stop,
		crush_c_throw, crush_c_fall, crush_c_end,
	} ;

	crush_c_jmp[pl_w->routine_2]( pl_w, ( PL37_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
/*			The air throw										*/
/*==============================================================*/
static void jump_p_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pls37_00_set( pl_w, 15, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void jump_p_run( PLAYER_WORK *pl_w )
{
	PL37_LOCAL_W *lw = ( PL37_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *em ;

	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( !pl_w->disp_dir ){
		if( scr_work.stage_left >= pl_w->pos.x ) lw->edge_flag = 1 ;
	} else {
		if( scr_work.stage_right <= pl_w->pos.x ) lw->edge_flag = 1 ;
	}
	if( lw->edge_flag ){
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->atck_no = pl37_at_air_throw ;
		pl_w->atck_no = pl37_at_air_throw ;
		em->catch_type = 1 ;
		em->pl_dir = pl_w->pl_dir ;
		em->pl_dir ^= 1 ;
		pl_w->hit_stop = 8 ;
		scr_work.quake_level = 3 ;
		scr_work.quake_flag = 1 ;
		pl_voice_req( pl_w, 30 ) ;
		pl_w->routine_2 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void jump_p_jump( PLAYER_WORK *pl_w )
{
	pl_w->spd.x = DOT_X( 5.75f ) ;
	pl_w->acc.x = -0.052083332f ;
	pl_w->spd.y = DOT_Y( 5.5f ) ;
	pl_w->acc.y = -0.6361607f ;
	if( pl_w->disp_dir ){
		pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = -pl_w->acc.x ;
	}
	pl_char_set( pl_w, Char_Catch, 19 ) ;
	pl_w->timer0 = 32 ;
	pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void jump_p_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !pl_w->timer0 -- ) pl_jump_ret( pl_w ) ;
}

/*==============================================================*/
static void jump_p_catch( PLAYER_WORK *pl_w )
{
	static void ( *jump_p_jmp[])( PLAYER_WORK *pl_w ) = {
		jump_p_start, jump_p_run, jump_p_jump, jump_p_fall,
	} ;

	jump_p_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void cyclone_c_catch( PLAYER_WORK *pl_w )
{
}

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void pl37_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_k_catch, nml_p_catch, crush_c_catch, jump_p_catch, cyclone_c_catch,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

static void cyclone_em_y_ck( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw );
static void cyclone_em_x_ck( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw );

/*==============================================================*/
/*			Saotome Cyclone's catch, run for the opponent:		*/
/*			he swings back and forth before Jin, then is		*/
/*			carried up. lw is Jin's work						*/
/*==============================================================*/
static void cyclone_em_init( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	pl_w->spd.x = -40.0f ;
	pl_w->acc.x = DOT_X( 8.0f ) ;
	pl_w->spd.y = DOT_Y( 2.0f ) ;
	pl_w->acc.y = 0.008370535f ;
	lw->swing_flag = 0 ;
	lw->swing_x = pl_w->pos.x ;
	lw->swing_spd = pl_w->spd.x ;
	lw->swing_timer = 32 ;
	pl_char_set( pl_w, Char_Damage, 28 ) ;
	pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void cyclone_em_swing( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	PLAYER_WORK *em = pl_w->catch_w ;

	pl_w->cg_ctr = 1 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	cyclone_em_y_ck( pl_w, lw ) ;
	cyclone_em_x_ck( pl_w, lw ) ;
	if( !lw->swing_timer -- ){
		lw->swing_timer = 32 ;
		pl_w->prio = em->prio + 1 ;
		pl_w->routine_2 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void cyclone_em_rise( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	PLAYER_WORK *em = pl_w->catch_w ;

	pl_w->cg_ctr = 1 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x = em->pos.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	cyclone_em_y_ck( pl_w, lw ) ;
	if( !lw->swing_timer -- ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.y = DOT_Y( 8.0f ) ;
		pl_w->pos.x = em->pos.x ;
		pl_w->pos.y -= DOT_Y( 16.0f ) ;
	}
}

/*--------------------------------------------------------------*/
static void cyclone_em_drop( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	PLAYER_WORK *em = pl_w->catch_w ;

	pl_char_move( pl_w ) ;
	pl_w->pos.x = em->pos.x ;
	cyclone_em_y_ck( pl_w, lw ) ;
}

/*==============================================================*/
/*			No higher than the top of the screen				*/
/*==============================================================*/
static void cyclone_em_y_ck( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	if( pl_w->pos.y > pl_w->ground_y + DOT_Y( 256.0f ) ) pl_w->pos.y = pl_w->ground_y + DOT_Y( 256.0f ) ;
}

/*==============================================================*/
/*			The swing turns where it began						*/
/*==============================================================*/
static void cyclone_em_x_ck( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw )
{
	if( !lw->swing_flag ){
		if( pl_w->pos.x > lw->swing_x ){
			pl_w->spd.x = -lw->swing_spd ;
			pl_w->acc.x = -pl_w->acc.x ;
			lw->swing_flag = 1 ;
		}
	} else if( pl_w->pos.x < lw->swing_x ){
		pl_w->spd.x = lw->swing_spd ;
		pl_w->acc.x = -pl_w->acc.x ;
		lw->swing_flag = 0 ;
	}
}

/*==============================================================*/
static void cyclone_c_hold( PLAYER_WORK *pl_w )
{
	static void ( *cyclone_em_jmp[])( PLAYER_WORK *pl_w, PL37_LOCAL_W *lw ) = {
		cyclone_em_init, cyclone_em_swing, cyclone_em_rise, cyclone_em_drop,
	} ;
	PLAYER_WORK *em = pl_w->catch_w ;
	PL37_LOCAL_W *lw = ( PL37_LOCAL_W * )em->tlocal ;

	cyclone_em_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/*==============================================================*/
static void nml_k_hold( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->catch_w ;

	catch_em_pos_set1( em, pl_w ) ;
}

/*==============================================================*/
static void hold( PLAYER_WORK *pl_w )
{
	catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/****************************************************************/
/*			Holding the opponent								*/
/****************************************************************/
void pl37_hold( PLAYER_WORK *pl_w )
{
	static void ( *hold_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_k_hold, hold, hold, hold, cyclone_c_hold,
	} ;

	hold_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void pl37_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl37_sp_cyclone ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl37_sp_blodia ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl37_sp_blodia ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl37_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl37_sp_cyclone ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl37_sp_blodia ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl37_sp_blodia ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl37_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl37_sp_typhoon ;
		pl_w->atck_level = 0 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl37_sp_dynamite ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl37_sp_punch ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void pl37_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl37_sp_typhoon ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl37_sp_dynamite ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl37_sp_punch ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
/*			A new round: all but burning_flag					*/
/****************************************************************/
void pl37_tlocal_init( PLAYER_WORK *pl_w )
{
	PL37_LOCAL_W *lw = ( PL37_LOCAL_W * )pl_w->tlocal ;
	Uint8 keep = lw->burning_flag ;

	memset( lw, 0, sizeof( pl_w->tlocal ) ) ;
	lw->burning_flag = keep ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl37.c" End of File	*/

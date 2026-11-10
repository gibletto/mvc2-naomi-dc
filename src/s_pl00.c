/* Ryu (character 00): command checks, normal attacks, dashes, appearance and win poses, special
   moves and hyper combos, throws, partner moves and the wall kick. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl00_def.h"

void pl00_main( PLAYER_WORK *pl_w );
void pl00_com_ck( PLAYER_WORK *pl_w );
void pl00_atck_set( PLAYER_WORK *pl_w );
void pl00_atck( PLAYER_WORK *pl_w );
void pl00_dash( PLAYER_WORK *pl_w );
void pl00_noki( PLAYER_WORK *pl_w );
void pl00_appear( PLAYER_WORK *pl_w );
void pl00_win( PLAYER_WORK *pl_w );
void pl00_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl00_catch_ck( PLAYER_WORK *pl_w );
void pl00_catch_set( PLAYER_WORK *pl_w );
void pl00_catch( PLAYER_WORK *pl_w );
void pl00_hold( PLAYER_WORK *pl_w );
int pl00_delay_ck( PLAYER_WORK *pl_w );
void pl00_t_kick( PLAYER_WORK *pl_w );
void pl00_upper( PLAYER_WORK *pl_w );
void pl00_sp_unison( PLAYER_WORK *pl_w );
void pl00_sp_gattai( PLAYER_WORK *pl_w );
void pl00_sp_assist( PLAYER_WORK *pl_w );
void pl00_sp_zero( PLAYER_WORK *pl_w );

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
static Uint16 com_hadou[] = { 3, COM_GROUND, pl00_sp_hadou, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Hadouken */
static Uint16 com_shouryu[] = { 3, COM_GROUND, pl00_sp_shouryu, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_PUNCH } ;	/* Shoryuken */
static Uint16 com_tatsu[] = { 3, COM_GROUND, pl00_sp_tatsu, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Tatsumaki Senpuukyaku */
static Uint16 com_jtatsu[] = { 3, COM_AIR, pl00_sp_jtatsu, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Tatsumaki Senpuukyaku in the air */
static Uint16 com_jhadou[] = { 3, COM_AIR, pl00_sp_jhadou, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Hadouken in the air */
static Uint16 com_shinkuu[] = { 3, COM_HYPER | COM_ALL_SW, pl00_sp_shinkuu, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Shinkuu Hadouken */
static Uint16 com_shinkuu_tatsu[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl00_sp_shinkuu_tatsu, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Shinkuu Tatsumaki Senpuukyaku */
static Uint16 com_shin_shouryu[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl00_sp_shin_shouryu, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_PUNCH } ;	/* Shin Shoryuken */

static void ( *pl00_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl00_main,
	pl00_com_ck,
	pl00_atck_set,
	pl00_atck,
	pl00_dash,
	pl00_noki,
	pl00_appear,
	pl00_win,
	pl00_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl00_catch_ck,	/* returns a value */
	pl00_catch_set,
	pl00_catch,
	pl00_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl00_delay_ck,
	pl00_t_kick,
	pl00_upper,
	pl00_sp_unison,
	pl00_sp_gattai,
	pl00_sp_assist,
	pl00_sp_zero,
} ;

static Uint8 hadou_ck( PLAYER_WORK *pl_w );
static Uint8 shouryu_ck( PLAYER_WORK *pl_w );
static Uint8 tatsu_ck( PLAYER_WORK *pl_w );
static Uint8 jtatsu_ck( PLAYER_WORK *pl_w );
static Uint8 jhadou_ck( PLAYER_WORK *pl_w );
static Uint8 shinkuu_ck( PLAYER_WORK *pl_w );
static Uint8 shinkuu_tatsu_ck( PLAYER_WORK *pl_w );
static Uint8 shin_shouryu_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static int chouhatsu_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl00_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl00_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl00_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( shin_shouryu_ck( pl_w ) ) return ;
	if( shinkuu_ck( pl_w ) ) return ;
	if( shinkuu_tatsu_ck( pl_w ) ) return ;
	if( shouryu_ck( pl_w ) ) return ;
	if( hadou_ck( pl_w ) ) return ;
	if( tatsu_ck( pl_w ) ) return ;
	if( jhadou_ck( pl_w ) ) return ;
	if( jtatsu_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
/*			On the ground; in a super jump, only while			*/
/*			shot_flag is clear									*/
/*==============================================================*/
static Uint8 hadou_ck( PLAYER_WORK *pl_w )
{
	PL00_LOCAL_W *lw = ( PL00_LOCAL_W * )pl_w->tlocal ;

	if( !com_nml_ck( pl_w, com_hadou, &pl_w->com_w ) ) return 0 ;
	if( !pl_w->sjump_flag && lw->shot_flag ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl00_sp_hadou ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 shouryu_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_shouryu, &pl_w->com_w0 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->pl_sp_type = pl00_sp_shouryu ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 tatsu_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_tatsu, &pl_w->com_w1 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->pl_sp_type = pl00_sp_tatsu ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
/*			In the air: once a jump, unless super jumping		*/
/*==============================================================*/
static Uint8 jtatsu_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_jtatsu, &pl_w->com_w2 ) ) return 0 ;
	if( pl_w->spd.y < 0.0f && pl_w->pos.y - pl_w->ground_y < DOT_Y( 48.0f ) ) return 1 ;
	if( !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->pl_sp_type = pl00_sp_jtatsu ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 jhadou_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_jhadou, &pl_w->com_w3 ) ) return 0 ;
	if( !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w3 ) ;
	pl_w->pl_sp_type = pl00_sp_jhadou ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 shinkuu_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_shinkuu, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl00_sp_shinkuu ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 shinkuu_tatsu_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_shinkuu_tatsu, &pl_w->com_w5 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl00_sp_shinkuu_tatsu ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
/*			Shin Shoryuken, a level 3 hyper combo				*/
/*==============================================================*/
static Uint8 shin_shouryu_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_shin_shouryu, &pl_w->com_w6 ) ) return 0 ;
	if( *pl_w->gauge_level < 3 ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl00_sp_shin_shouryu ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static int snapback_ck( PLAYER_WORK *pl_w )
{
	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl00_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static int chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 8 ) ) return 0 ;
	pl_w->pl_sp_type = pl00_sp_chouhatsu ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/****************************************************************/
void pl00_main( PLAYER_WORK *pl_w )
{
}

static void nml_atck( PLAYER_WORK *pl_w );
static void stand_punch( PLAYER_WORK *pl_w );
static void crouch_punch( PLAYER_WORK *pl_w );
static void stand_kick( PLAYER_WORK *pl_w );
static void crouch_kick( PLAYER_WORK *pl_w );
static void jump_atck( PLAYER_WORK *pl_w );
static void jump_punch( PLAYER_WORK *pl_w );
static void jump_kick( PLAYER_WORK *pl_w );
static void jump_atck_ck( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Normal attacks										*/
/****************************************************************/
void pl00_atck_set( PLAYER_WORK *pl_w )
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
/*			The heavy punch with the stick forward is a step	*/
/*			and a punch (routine_2 set)							*/
/*==============================================================*/
static void stand_punch( PLAYER_WORK *pl_w )
{
	pl_w->char_no.b.grp = 7 ;
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
		if( pl_w->atck_lever & KEY_FORWARD ){
			pl_w->char_no.b.grp = 20 ;
			pl_w->char_no.b.no = 0 ;
			pl_w->atck_no = pl00_at_fwd_hp_1 ;
			pl_w->routine_2 = 1 ;
		} else {
			pl_w->char_no.b.no = 2 ;
			pl_w->atck_no = ATCK_STAND_HP ;
			pl_se_req( pl_w, SE_SWING_H ) ;
			pl_w->routine_2 = 0 ;
		}
		pl_w->chain_tbl = hp_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
static void crouch_punch( PLAYER_WORK *pl_w )
{
	pl_w->char_no.b.grp = 7 ;
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
/*			The heavy kick with the stick forward is a			*/
/*			jumping kick (routine_2 set)						*/
/*==============================================================*/
static void stand_kick( PLAYER_WORK *pl_w )
{
	pl_w->char_no.b.grp = 8 ;
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
			pl_w->char_no.b.grp = 20 ;
			pl_w->char_no.b.no = 1 ;
			pl_w->atck_no = pl00_at_fwd_hk ;
			pl_w->routine_2 = 1 ;
			pl_w->routine_3 = 0 ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
		} else {
			pl_w->char_no.b.no = 2 ;
			pl_w->atck_no = ATCK_STAND_HK ;
			pl_se_req( pl_w, SE_SWING_H ) ;
			pl_w->routine_2 = 0 ;
		}
		pl_w->chain_tbl = hk_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
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
static void jump_atck( PLAYER_WORK *pl_w )
{
	if( pl_w->atck_pk == 1 ) jump_kick( pl_w ) ;
	else jump_punch( pl_w ) ;
}

/*==============================================================*/
static void jump_punch( PLAYER_WORK *pl_w )
{
	pl_w->char_no.b.grp = 11 ;
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
		pl_w->char_no.b.grp = 20 ;
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = pl00_at_jump_mp_1 ;
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
	pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
	if( pl_w->jump_atk_cnt & JATK_P_MASK ) pl_w->jump_atk_cnt -= 0x01 ;
}

/*==============================================================*/
static void jump_kick( PLAYER_WORK *pl_w )
{
	pl_w->char_no.b.grp = 12 ;
	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.no = 3 ;
		pl_w->atck_no = ATCK_JUMP_LK ;
		pl_w->char_no.b.grp = 20 ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_lk_tbl ;
		else pl_w->chain_tbl = sj_lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->char_no.b.grp = 20 ;
		pl_w->char_no.b.no = 4 ;
		pl_w->atck_no = ATCK_JUMP_MK ;
		if( pl_w->atck_lever & KEY_UP ){
			pl_w->char_no.b.grp = 12 ;
			pl_w->char_no.b.no = 1 ;
			pl_w->atck_no = pl00_at_jup_mk ;
		}
		pl_se_req( pl_w, SE_SWING_M ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mk_tbl ;
		else pl_w->chain_tbl = sj_mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 5 ;
		pl_w->atck_no = pl00_at_jump_hk_b ;
		if( pl_w->atck_lever & KEY_UP ){
			pl_w->char_no.b.no = 2 ;
			pl_w->atck_no = ATCK_JUMP_HK ;
		} else pl_w->char_no.b.grp = 20 ;
		pl_se_req( pl_w, SE_SWING_H ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_hk_tbl ;
		else pl_w->chain_tbl = sj_hk_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
	if( pl_w->jump_atk_cnt & JATK_K_MASK ) pl_w->jump_atk_cnt -= 0x10 ;
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
void pl00_atck( PLAYER_WORK *pl_w )
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
/*			The stepping punch hits again on the animation's	*/
/*			mark, and moves on another							*/
/*==============================================================*/
static void stand_punch_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	case 2:
		if( pl_char_move( pl_w ) < 0 ){
			pl_stand_ret( pl_w ) ;
			break ;
		}
		if( pl_w->routine_2 ){
			if( pl_w->cg_type ){
				pl_w->cg_type = 0 ;
				pl_w->atck_no = pl00_at_fwd_hp_2 ;
				HIT_RESET( pl_w ) ;
			}
			if( pl_w->cg_prm ) pl_w->pos.x += pl_w->pl_dir ? DOT_X( 2.0f ) : -DOT_X( 2.0f ) ;
		}
		break ;
	}
}

/*==============================================================*/
static void crouch_punch_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void stand_kick_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	case 1:
		if( pl_char_move( pl_w ) < 0 ){
			pl_stand_ret( pl_w ) ;
			return ;
		}
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pl_w->atck_no = pl00_at_stand_mk_2 ;
			pl_w->hit_flag = pl_w->atck_attr = 0 ;
			pl_w->unk_1c4 = 0 ;
			vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
		}
		break ;
	case 2:
		if( !pl_w->routine_2 ){
			if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
			break ;
		}
		switch( pl_w->routine_3 ){
		case 0:
			pl_char_move( pl_w ) ;
			if( pl_w->cg_type ){
				pl_w->routine_3 ++ ;
				pl_w->pl_stance = STANCE_AIR ;
				pl_w->spd.x = pl_w->pl_dir ? DOT_X( 4.25f ) : -DOT_X( 4.25f ) ;
				pl_w->acc.x = 0.0f ;
				pl_w->spd.y = DOT_Y( 4.0f ) ;
				pl_w->acc.y = -0.7533482f ;
			}
			break ;
		case 1:
			pl_char_move( pl_w ) ;
			if( pl_land_y_ck( pl_w, pl_w->pos.y ) ){
				pl_w->routine_3 ++ ;
				pl_w->pos.y = pl_w->ground_y ;
				pl_w->pl_stance = STANCE_STAND ;
				pl_w->spd.x = 0.0f ;
				pl_w->spd.y = 0.0f ;
				pl_w->acc.x = 0.0f ;
				pl_w->acc.y = 0.0f ;
				pl_land_ef_set( pl_w ) ;
				pl_char_set( pl_w, Char_Extra, 10 ) ;
			}
			break ;
		case 2:
			if( pl_char_move( pl_w ) < 0 ){
				pl_w->target_w->nml_catch_ng_timer = 5 ;
				pl_stand_ret( pl_w ) ;
			}
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
/*			The middle punch in the air hits twice				*/
/*==============================================================*/
static void jump_punch_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
	case 2:
		if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
		break ;
	case 1:
		if( pl_char_move( pl_w ) < 0 ){
			pl_jump_ret( pl_w ) ;
			return ;
		}
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pl_w->atck_no = pl00_at_jump_mp_2 ;
			HIT_RESET( pl_w ) ;
		}
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
void pl00_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_w->atck_no = pl00_at_upper ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_char_set( pl_w, Char_Extra, 11 ) ;
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
static void dash_start( PLAYER_WORK *pl_w );
static void dash_move( PLAYER_WORK *pl_w );
static void dash_stop( PLAYER_WORK *pl_w );

/****************************************************************/
void pl00_dash( PLAYER_WORK *pl_w )
{
	static void ( *dash_jmp[])( PLAYER_WORK *pl_w ) = {
		dash_init, dash_start, dash_move, dash_stop,
	} ;

	dash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dash_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	dash_start( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void dash_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 8.0f ) : -DOT_X( 8.0f ) ;
		pl_w->acc.x = pl_w->pl_dir ? -0.13020833f : 0.13020833f ;
		pl_w->timer0 = 24 ;
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
	if( -- pl_w->timer0 <= 0 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 2.5f ) : -DOT_X( 2.5f ) ;
		pl_w->acc.x = pl_w->pl_dir ? -0.33854166f : 0.33854166f ;
		pl_char_set( pl_w, Char_Dash, 2 ) ;
	}
}

/*==============================================================*/
/*			Slows down until the speed turns round				*/
/*==============================================================*/
static void dash_stop( PLAYER_WORK *pl_w )
{
	Float spd = pl_w->spd.x ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( spd * pl_w->spd.x < 0.0f ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
}

static void noki_init( PLAYER_WORK *pl_w );
static void noki_start( PLAYER_WORK *pl_w );
static void noki_move( PLAYER_WORK *pl_w );
static void noki_stop( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash										*/
/****************************************************************/
void pl00_noki( PLAYER_WORK *pl_w )
{
	static void ( *noki_jmp[])( PLAYER_WORK *pl_w ) = {
		noki_init, noki_start, noki_move, noki_stop,
	} ;

	noki_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void noki_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 11.0f ) : DOT_X( 11.0f ) ;
	pl_w->acc.x = pl_w->pl_dir ? 0.33854166f : -0.33854166f ;
	pl_w->timer0 = 20 ;
	noki_start( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void noki_start( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_char_move( pl_w ) ;
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
	if( -- pl_w->timer0 <= 0 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 2.5f ) : DOT_X( 2.5f ) ;
		pl_w->acc.x = pl_w->pl_dir ? 0.33854166f : -0.33854166f ;
		pl_char_set( pl_w, Char_Dash, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void noki_stop( PLAYER_WORK *pl_w )
{
	Float spd = pl_w->spd.x ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( spd * pl_w->spd.x < 0.0f ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
}

static void appear_init( PLAYER_WORK *pl_w );
static void appear_move( PLAYER_WORK *pl_w );
static void win_init( PLAYER_WORK *pl_w );
static void win_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl00_appear( PLAYER_WORK *pl_w )
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

/* the win poses for a KO, picked at random, and the pose for each other end of a round */
static Sint8 win_ko_tbl[8] = { 0, 1, 2, 3, 6, 1, 2, 6 } ;
static Sint8 win_tbl[5] = { 4, 4, 0, 4, 4 } ;

/****************************************************************/
void pl00_win( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ) win_init( pl_w ) ;
	else win_move( pl_w ) ;
}

/*==============================================================*/
static void win_init( PLAYER_WORK *pl_w )
{
	Uint8 no ;

	pl_w->routine_2 ++ ;
	no = win_tbl[pl_w->type] ;
	if( !pl_w->type ) no = win_ko_tbl[Rnd() & 7] ;
	pl_char_set( pl_w, Char_Win, no ) ;
}

/*==============================================================*/
static void win_move( PLAYER_WORK *pl_w )
{
	if( pl_win_end_ck( pl_w ) ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
}

static void sp_hadou( PLAYER_WORK *pl_w );
static void sp_shouryu( PLAYER_WORK *pl_w );
static void sp_tatsu( PLAYER_WORK *pl_w );
static void sp_jtatsu( PLAYER_WORK *pl_w );
static void sp_jhadou( PLAYER_WORK *pl_w );
static void sp_shinkuu( PLAYER_WORK *pl_w );
static void sp_shinkuu_tatsu( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_shin_shouryu( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );

/****************************************************************/
void	pl00_sp_atck( PLAYER_WORK *pl_w )
{
	static void (*pl_sp_tbl[])( PLAYER_WORK *pl_w ) ={
		sp_hadou,					/* 00 Hadouken */
		sp_shouryu,					/* 01 Shoryuken */
		sp_tatsu,					/* 02 Tatsumaki Senpuukyaku */
		sp_jtatsu,					/* 03 Tatsumaki Senpuukyaku in the air */
		sp_jhadou,					/* 04 Hadouken in the air */
		sp_shinkuu,					/* 05 Shinkuu Hadouken (hyper) */
		sp_shinkuu_tatsu,					/* 06 Shinkuu Tatsumaki Senpuukyaku (hyper) */
		sp_hadou,					/* 07 */
		sp_chouhatsu,					/* 08 the taunt */
		sp_hadou,					/* 09 */
		sp_hadou,					/* 0a */
		sp_shin_shouryu,					/* 0b Shin Shoryuken (level 3 hyper) */
		sp_hadou,					/* 0c */
		sp_snapback,					/* 0d the snapback */
	} ;

	pl_sp_tbl[ pl_w->pl_sp_type ]( pl_w ) ;
}

static void hadou_init( PLAYER_WORK *pl_w, PL00_LOCAL_W *lw );
static void hadou_move( PLAYER_WORK *pl_w, PL00_LOCAL_W *lw );

/*==============================================================*/
/*			A shot along the ground								*/
/*==============================================================*/
static void sp_hadou( PLAYER_WORK *pl_w )
{
	static void ( *hadou_jmp[])( PLAYER_WORK *pl_w, PL00_LOCAL_W *lw ) = {
		hadou_init, hadou_move,
	} ;

	hadou_jmp[pl_w->routine_2]( pl_w, ( PL00_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void hadou_init( PLAYER_WORK *pl_w, PL00_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->atck_no = pl_w->atck_level + 48 ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level ) ;
}

/*--------------------------------------------------------------*/
static void hadou_move( PLAYER_WORK *pl_w, PL00_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shl00_00_set( pl_w, 0 ) ;
	}
}

/*==============================================================*/
/*			Heavier once past the top of the jump				*/
/*==============================================================*/
static void shouryu_gravity( PLAYER_WORK *pl_w )
{
	Float old, now ;

	old = pl_w->spd.y ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	now = pl_w->spd.y ;
	if( old > 0.0f && now < 0.0f ) pl_w->acc.y = -DOT_Y( 0.75f ) ;
}

/*==============================================================*/
/*			A hit on the animation's mark, while the hits		*/
/*			left (hit_cnt, guard_cnt) last						*/
/*==============================================================*/
static void shouryu_hit( PLAYER_WORK *pl_w, PL00_LOCAL_W *lw )
{
	Sint8 no ;

	if( lw->hit_cnt > 0 && lw->guard_cnt > 0 && pl_w->cg_prm ){
		no = pl_w->cg_prm ;
		pl_w->cg_prm = 0 ;
		if( pl_w->hit_flag ){
			if( pl_w->hit_flag & HR_GUARD ){
				if( -- lw->guard_cnt <= 0 ) return ;
			} else {
				if( -- lw->hit_cnt <= 0 ) return ;
			}
		}
		pl_w->atck_no = no ;
		HIT_RESET( pl_w ) ;
	}
}

static void shouryu_init( PLAYER_WORK *pl_w, PL00_LOCAL_W *lw );
static void shouryu_start( PLAYER_WORK *pl_w, PL00_LOCAL_W *lw );
static void shouryu_jump( PLAYER_WORK *pl_w, PL00_LOCAL_W *lw );
static void shouryu_fall( PLAYER_WORK *pl_w, PL00_LOCAL_W *lw );
static void shouryu_land( PLAYER_WORK *pl_w, PL00_LOCAL_W *lw );

/* hits for each strength, counted apart by the side of hit_flag (hit_cnt, guard_cnt) */
static Uint8 shouryu_hits[4] = { 1, 1, 1, 1 } ;

/*==============================================================*/
/*			A rising uppercut									*/
/*==============================================================*/
static void sp_shouryu( PLAYER_WORK *pl_w )
{
	static void ( *shouryu_jmp[])( PLAYER_WORK *pl_w, PL00_LOCAL_W *lw ) = {
		shouryu_init, shouryu_start, shouryu_jump, shouryu_fall, shouryu_land,
	} ;

	shouryu_jmp[pl_w->routine_2]( pl_w, ( PL00_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void shouryu_init( PLAYER_WORK *pl_w, PL00_LOCAL_W *lw )
{
	Float spd_x ;

	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->routine_2 ++ ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_gauge_add( pl_w, 10 ) ;
	if( !pl_w->atck_level ){
		spd_x = 10.0f ;
		pl_w->spd.y = DOT_Y( 8.0f ) ;
	} else {
		spd_x = DOT_X( 10.0f ) ;
		pl_w->spd.y = DOT_Y( 16.0f ) ;
	}
	if( !pl_w->pl_dir ) spd_x = -spd_x ;
	pl_w->spd.x = spd_x ;
	pl_w->acc.x = pl_w->pl_dir ? -0.7291666f : 0.7291666f ;
	pl_w->acc.y = -DOT_Y( 0.5f ) ;
	pl_w->atck_no = pl_w->atck_level + 51 ;
	HIT_RESET( pl_w ) ;
	lw->hit_cnt = shouryu_hits[pl_w->atck_level * 2] ;
	lw->guard_cnt = shouryu_hits[pl_w->atck_level * 2 + 1] ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 2 ) ;
}

/*--------------------------------------------------------------*/
static void shouryu_start( PLAYER_WORK *pl_w, PL00_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	shouryu_hit( pl_w, lw ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_air_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void shouryu_jump( PLAYER_WORK *pl_w, PL00_LOCAL_W *lw )
{
	Float now, old ;

	shouryu_gravity( pl_w ) ;
	old = pl_w->spd.x ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	now = pl_w->spd.x ;
	if( now * old < 0.0f ) pl_w->routine_2 ++ ;
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shouryu_fall( PLAYER_WORK *pl_w, PL00_LOCAL_W *lw )
{
	shouryu_gravity( pl_w ) ;
	if( pl_w->pos.y > pl_w->ground_y ){
		if( pl_w->cg_type == 0 ) pl_char_move( pl_w ) ;
		return ;
	}
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_land_ef_set( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shouryu_land( PLAYER_WORK *pl_w, PL00_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			Another hit on each of the animation's marks		*/
/*==============================================================*/
static void tatsu_hit( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type > 0 ){
		pl_w->cg_type = 0 ;
		pl_w->atck_no = pl_w->atck_level + 57 ;
		HIT_RESET( pl_w ) ;
	}
}

static void tatsu_init( PLAYER_WORK *pl_w );
static void tatsu_rise( PLAYER_WORK *pl_w );
static void tatsu_spin( PLAYER_WORK *pl_w );
static void tatsu_fall( PLAYER_WORK *pl_w );
static void tatsu_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A spinning kick that travels forward				*/
/*==============================================================*/
static void sp_tatsu( PLAYER_WORK *pl_w )
{
	static void ( *tatsu_jmp[])( PLAYER_WORK *pl_w ) = {
		tatsu_init, tatsu_rise, tatsu_spin, tatsu_fall, tatsu_land,
	} ;

	tatsu_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void tatsu_init( PLAYER_WORK *pl_w )
{
	Float spd ;

	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_gauge_add( pl_w, 10 ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->atck_no = pl_w->atck_level + 57 ;
	HIT_RESET( pl_w ) ;
	pl_w->spd.y = DOT_Y( 4.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_air_set( pl_w ) ;
	spd = pl_w->atck_level ? 6.25f : DOT_X( 2.5f ) ;
	pl_w->spd.x = pl_w->pl_dir ? spd : -spd ;
	pl_w->acc.x = 0.0f ;
	pl_char_set( pl_w, Char_Special, 4 ) ;
}

/*--------------------------------------------------------------*/
static void tatsu_rise( PLAYER_WORK *pl_w )
{
	Float spd = pl_w->spd.y ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( spd * pl_w->spd.y > 0.0f ){
		pl_char_move( pl_w ) ;
		return ;
	}
	pl_w->routine_2 ++ ;
	pl_w->timer0 = pl_w->atck_level * 2 ;
	pl_char_set( pl_w, Char_Special, 6 ) ;
	tatsu_hit( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void tatsu_spin( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type < 0 ){
		pl_w->cg_type = 0 ;
		if( -- pl_w->timer0 < 0 ){
			pl_w->routine_2 ++ ;
			pl_char_set( pl_w, Char_Special, 5 ) ;
			return ;
		}
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_char_move( pl_w ) ;
	tatsu_hit( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void tatsu_fall( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Special, 7 ) ;
		pl_land_ef_set( pl_w ) ;
	} else {
		pl_char_move( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void tatsu_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void jtatsu_hit( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type > 0 ){
		pl_w->cg_type = 0 ;
		pl_w->atck_no = pl_w->atck_level + 60 ;
		HIT_RESET( pl_w ) ;
	}
}

static void jtatsu_init( PLAYER_WORK *pl_w );
static void jtatsu_spin( PLAYER_WORK *pl_w );
static void jtatsu_fall( PLAYER_WORK *pl_w );
static void jtatsu_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The spinning kick in the air						*/
/*==============================================================*/
static void sp_jtatsu( PLAYER_WORK *pl_w )
{
	static void ( *jtatsu_jmp[])( PLAYER_WORK *pl_w ) = {
		jtatsu_init, jtatsu_spin, jtatsu_fall, jtatsu_land,
	} ;

	jtatsu_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			Adds to the jump's speed, the way it is already		*/
/*			moving												*/
/*==============================================================*/
static void jtatsu_init( PLAYER_WORK *pl_w )
{
	Float spd ;

	pl_w->routine_2 ++ ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->atck_no = pl_w->atck_level + 60 ;
	HIT_RESET( pl_w ) ;
	spd = pl_w->atck_level ? DOT_X( 2.5f ) : 2.5f ;
	if( pl_w->pl_dir ){
		if( pl_w->spd.x < 0.0f ) spd = -spd ;
	} else {
		spd = -spd ;
		if( pl_w->spd.x > 0.0f ) spd = -spd ;
	}
	pl_w->spd.x += spd ;
	pl_w->acc.x = 0.0f ;
	pl_w->timer0 = pl_w->atck_level * 2 ;
	pl_char_set( pl_w, Char_Special, 8 ) ;
}

/*--------------------------------------------------------------*/
static void jtatsu_spin( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 = 3 ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Jump, 3 ) ;
		return ;
	}
	if( pl_w->cg_type < 0 ){
		pl_w->cg_type = 0 ;
		if( -- pl_w->timer0 < 0 ){
			pl_w->routine_2 ++ ;
			pl_char_set( pl_w, Char_Special, 10 ) ;
			return ;
		}
	}
	pl_char_move( pl_w ) ;
	jtatsu_hit( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void jtatsu_fall( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Jump, 3 ) ;
	} else if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void jtatsu_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			Moves, and stops on the floor						*/
/*==============================================================*/
static void jhadou_xy_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ) pl_w->pos.y = pl_w->ground_y ;
}

static void jhadou_init( PLAYER_WORK *pl_w, PL00_LOCAL_W *lw );
static void jhadou_move( PLAYER_WORK *pl_w, PL00_LOCAL_W *lw );
static void jhadou_end( PLAYER_WORK *pl_w, PL00_LOCAL_W *lw );

/*==============================================================*/
/*			The shot thrown from a jump							*/
/*==============================================================*/
static void sp_jhadou( PLAYER_WORK *pl_w )
{
	static void ( *jhadou_jmp[])( PLAYER_WORK *pl_w, PL00_LOCAL_W *lw ) = {
		jhadou_init, jhadou_move, jhadou_end,
	} ;

	jhadou_jmp[pl_w->routine_2]( pl_w, ( PL00_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
/*			The jump slows down while the shot is thrown		*/
/*==============================================================*/
static void jhadou_init( PLAYER_WORK *pl_w, PL00_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_gauge_add( pl_w, 2 ) ;
	pl_w->atck_no = pl_w->atck_level + 63 ;
	HIT_RESET( pl_w ) ;
	pl_w->spd.x /= 16.0f ;
	pl_w->spd.y /= 8.0f ;
	pl_w->acc.y /= 64.0f ;
	pl_w->acc.x = 0.0f ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 14 ) ;
}

/*--------------------------------------------------------------*/
static void jhadou_move( PLAYER_WORK *pl_w, PL00_LOCAL_W *lw )
{
	jhadou_xy_move( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		shl00_00_set( pl_w, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void jhadou_end( PLAYER_WORK *pl_w, PL00_LOCAL_W *lw )
{
	jhadou_xy_move( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_jump_ret( pl_w ) ;
	}
}

static void shinkuu_init( PLAYER_WORK *pl_w, PL00_LOCAL_W *lw );
static void shinkuu_wait( PLAYER_WORK *pl_w, PL00_LOCAL_W *lw );
static void shinkuu_shot( PLAYER_WORK *pl_w, PL00_LOCAL_W *lw );
static void shinkuu_beam( PLAYER_WORK *pl_w, PL00_LOCAL_W *lw );
static void shinkuu_end( PLAYER_WORK *pl_w, PL00_LOCAL_W *lw );

/*==============================================================*/
/*			A hyper combo: a beam, on the ground or in the		*/
/*			air, that lasts longer for pressed buttons			*/
/*==============================================================*/
static void sp_shinkuu( PLAYER_WORK *pl_w )
{
	static void ( *shinkuu_jmp[])( PLAYER_WORK *pl_w, PL00_LOCAL_W *lw ) = {
		shinkuu_init, shinkuu_wait, shinkuu_shot, shinkuu_beam, shinkuu_end,
	} ;

	shinkuu_jmp[pl_w->routine_2]( pl_w, ( PL00_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void shinkuu_init( PLAYER_WORK *pl_w, PL00_LOCAL_W *lw )
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
	if( pl_w->pl_stance != STANCE_AIR ){
		pl_dir_set( pl_w ) ;
		pl_sp_ef_set( pl_w ) ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
	}
	lw->renda_cnt = pl_w->cpu_flag ? 12 : 24 ;
	pl_w->timer0 = 64 ;
	pl_w->atck_no = pl00_at_shinkuu ;
	HIT_RESET( pl_w ) ;
	if( pl_w->pl_stance != STANCE_AIR ) pl_char_set( pl_w, Char_Super, 4 ) ;
	else pl_char_set( pl_w, Char_Super, 6 ) ;
}

/*--------------------------------------------------------------*/
static void shinkuu_wait( PLAYER_WORK *pl_w, PL00_LOCAL_W *lw )
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
		ofs.x = 40.0f ;
		ofs.y = DOT_Y( 64.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shinkuu_shot( PLAYER_WORK *pl_w, PL00_LOCAL_W *lw )
{
	Float x ;
	Float y ;
	int no ;
	Uint8 type ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		if( pl_w->pl_stance != STANCE_AIR ){
			x = 195.0f ;
			y = DOT_Y( 53.0f ) ;
			no = 0 ;
			type = 0 ;
		} else {
			x = DOT_X( 104.0f ) ;
			y = DOT_Y( 72.0f ) ;
			no = 0 ;
			type = 1 ;
		}
		pls00_01_set( pl_w, type ) ;
		if( shl00_01_set( pl_w, no, x, y ) == 0 ){ }
	}
}

/*--------------------------------------------------------------*/
static void shinkuu_beam( PLAYER_WORK *pl_w, PL00_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( lw->renda_cnt >= 0 && sw_renda_ck( pl_w ) ){
		lw->renda_cnt -- ;
		pl_w->timer0 ++ ;
	}
	if( -- pl_w->timer0 <= 0 ){
		pl_w->routine_2 ++ ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_char_set( pl_w, Char_Super, ( pl_w->pl_stance != STANCE_AIR ) ? 5 : 7 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shinkuu_end( PLAYER_WORK *pl_w, PL00_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		if( pl_w->pl_stance != STANCE_AIR ) pl_stand_ret( pl_w ) ;
		else pl_jump_ret( pl_w ) ;
	}
}

static void shinkuu_tatsu_init( PLAYER_WORK *pl_w );
static void shinkuu_tatsu_wait( PLAYER_WORK *pl_w );
static void shinkuu_tatsu_start( PLAYER_WORK *pl_w );
static void shinkuu_tatsu_rise( PLAYER_WORK *pl_w );
static void shinkuu_tatsu_spin( PLAYER_WORK *pl_w );
static void shinkuu_tatsu_spin_end( PLAYER_WORK *pl_w );
static void shinkuu_tatsu_fall( PLAYER_WORK *pl_w );
static void shinkuu_tatsu_land( PLAYER_WORK *pl_w );
static void shinkuu_tatsu_hit( PLAYER_WORK *pl_w );
static void shinkuu_tatsu_last_hit( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo: a rising uppercut with many hits		*/
/*==============================================================*/
static void sp_shinkuu_tatsu( PLAYER_WORK *pl_w )
{
	static void ( *shinkuu_tatsu_jmp[])( PLAYER_WORK *pl_w ) = {
		shinkuu_tatsu_init, shinkuu_tatsu_wait, shinkuu_tatsu_start, shinkuu_tatsu_rise,
		shinkuu_tatsu_spin, shinkuu_tatsu_spin_end, shinkuu_tatsu_fall, shinkuu_tatsu_land,
	} ;

	shinkuu_tatsu_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			A hit on each of the animation's marks				*/
/*==============================================================*/
static void shinkuu_tatsu_hit( PLAYER_WORK *pl_w )
{
	Sint8 no ;

	if( pl_w->cg_flag1 ){
		no = pl_w->cg_flag1 ;
		pl_w->cg_flag1 = 0 ;
		pl_w->atck_no = no ;
		HIT_RESET( pl_w ) ;
		pl_w->atck_attr = ATR_BACK_POS ;
	}
}

/*==============================================================*/
static void shinkuu_tatsu_last_hit( PLAYER_WORK *pl_w )
{
	Sint8 no ;

	if( pl_w->cg_flag1 ){
		no = pl_w->cg_flag1 ;
		pl_w->cg_flag1 = 0 ;
		pl_w->atck_no = no ;
		HIT_RESET( pl_w ) ;
	}
}

/*==============================================================*/
static void shinkuu_tatsu_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->atck_no = pl00_at_shinkuu_tatsu ;
	HIT_RESET( pl_w ) ;
	pl_w->atck_attr = ATR_BACK_POS ;
	pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 4.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.5f ) ;
	pl_char_set( pl_w, Char_Super, 0 ) ;
}

/*--------------------------------------------------------------*/
static void shinkuu_tatsu_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_prm ){
		pl_w->routine_2 ++ ;
		pl_w->cg_prm = 0 ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = 80.0f ;
		ofs.y = DOT_Y( 64.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shinkuu_tatsu_start( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		pl_air_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void shinkuu_tatsu_rise( PLAYER_WORK *pl_w )
{
	Float spd ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	spd = pl_w->spd.y ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( spd * pl_w->spd.y > 0.0f ){
		pl_char_move( pl_w ) ;
		return ;
	}
	pl_w->routine_2 ++ ;
	pl_w->timer0 = 8 ;
	pls00_02_set( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 1 ) ;
}

/*==============================================================*/
/*			Spins in the air, sinking a little, for the			*/
/*			timer's turns of the animation						*/
/*==============================================================*/
static void shinkuu_tatsu_spin( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		if( -- pl_w->timer0 <= 0 ){
			pl_w->routine_2 ++ ;
			pl_char_set( pl_w, Char_Super, 8 ) ;
		}
		return ;
	}
	pl_w->pos.y += -0.13392857f ;
	shinkuu_tatsu_hit( pl_w ) ;
}

/*==============================================================*/
static void shinkuu_tatsu_spin_end( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Super, 2 ) ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		return ;
	}
	pl_w->pos.y += -0.13392857f ;
	shinkuu_tatsu_last_hit( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shinkuu_tatsu_fall( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Super, 3 ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shinkuu_tatsu_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Extra, 6 ) ;
	} else if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void shin_shouryu_init( PLAYER_WORK *pl_w );
static void shin_shouryu_wait( PLAYER_WORK *pl_w );
static void shin_shouryu_dash( PLAYER_WORK *pl_w );
static void shin_shouryu_rush( PLAYER_WORK *pl_w );
static void shin_shouryu_rise( PLAYER_WORK *pl_w );
static void shin_shouryu_fall( PLAYER_WORK *pl_w );
static void shin_shouryu_land( PLAYER_WORK *pl_w );
static void shin_shouryu_hit( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Shin Shoryuken: a dash, on a hit the rush, then		*/
/*			the rising uppercut either way						*/
/*==============================================================*/
static void sp_shin_shouryu( PLAYER_WORK *pl_w )
{
	static void ( *shin_shouryu_jmp[])( PLAYER_WORK *pl_w ) = {
		shin_shouryu_init, shin_shouryu_wait, shin_shouryu_dash, shin_shouryu_rush, shin_shouryu_rise, shin_shouryu_fall, shin_shouryu_land,
	} ;

	shin_shouryu_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void shin_shouryu_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->timer0 = 0 ;
	pl_w->atck_no = pl00_at_shin_shouryu ;
	HIT_RESET( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_char_set( pl_w, Char_Super, 9 ) ;
	shin_shouryu_hit( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shin_shouryu_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	shin_shouryu_hit( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = -25.0f ;
		ofs.y = DOT_Y( 46.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 3 ) ;
	}
}

/* the dash: a jump at its end, or the rush if it hit an opponent who can be hit */
/*==============================================================*/
static void shin_shouryu_dash( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 = 4 ;
		pl_air_set( pl_w ) ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 2.0f ) : -DOT_X( 2.0f ) ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 16.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.5f ) ;
		pl_w->timer1 = 1 ;
		pl_char_set( pl_w, Char_Super, 13 ) ;
	} else {
		if( pl_w->cg_type ){ }
		if( pl_w->hit_flag && !( pl_w->hit_flag & HR_GUARD ) && !pl_w->em_w->armor_flag ){
			pl_w->routine_2 ++ ;
			pl_w->timer1 = 0 ;
			pl_char_set( pl_w, Char_Super, 10 ) ;
		}
	}
	shin_shouryu_hit( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shin_shouryu_rush( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_air_set( pl_w ) ;
		pl_w->pos.x += pl_w->pl_dir ? 40.0f : -40.0f ;
		pl_w->spd.x = pl_w->pl_dir ? 1.6666666f : -1.6666666f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 20.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.5f ) ;
	}
	if( pl_w->cg_prm ){
		/* nothing here in the arcade build, though the flag is still tested */
	}
	shin_shouryu_hit( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shin_shouryu_rise( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->spd.y < 0.0f ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		if( !pl_w->timer1 ) pl_char_set( pl_w, Char_Super, 11 ) ;
		else pl_char_set( pl_w, Char_Super, 14 ) ;
	}
	shin_shouryu_hit( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shin_shouryu_fall( PLAYER_WORK *pl_w )
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
		if( !pl_w->timer1 ) pl_char_set( pl_w, Char_Super, 12 ) ;
		else pl_char_set( pl_w, Char_Super, 15 ) ;
		pl_land_ef_set( pl_w ) ;
	}
	shin_shouryu_hit( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shin_shouryu_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	shin_shouryu_hit( pl_w ) ;
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
	pl_color_set( pl_w, 0 ) ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = pl00_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 23 ) ;
}

/*--------------------------------------------------------------*/
static void snapback_move( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;
	Float spd ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type < 0 ){
		pl_w->cg_type = 0 ;
		ofs.x = 25.0f ;
		ofs.y = 165.0f ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
	if( pl_w->cg_prm ){
		spd = pl_w->pl_dir ? DOT_X( 2.0f ) : -DOT_X( 2.0f ) ;
		pl_w->pos.x += spd ;
	}
}

/*==============================================================*/
/*			A hit on each of the animation's marks				*/
/*==============================================================*/
static void shin_shouryu_hit( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_flag1 ){
		pl_w->atck_no = pl_w->cg_flag1 ;
		HIT_RESET( pl_w ) ;
		pl_w->cg_flag1 = 0 ;
	}
}

static PLAYER_WORK *nml_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *crouch_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *jump_catch_ck( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Can a throw start?									*/
/****************************************************************/
PLAYER_WORK *pl00_catch_ck( PLAYER_WORK *pl_w )
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
static PLAYER_WORK *crouch_catch_ck( PLAYER_WORK *pl_w )
{
	return 0 ;
}

/*==============================================================*/
static PLAYER_WORK *jump_catch_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( !( pl_w->dir = ( pl_w->atck_lever & ( KEY_FORWARD | KEY_BACK ) ) >> 10 ) ) return 0 ;
	if( pl_w->atck_pk == 1 && pl_w->atck_level == 1 && pl_w->pos.y > DOT_Y( 64.0f ) ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 2 ;
			return em ;
		}
		return 0 ;
	}
	return 0 ;
}

static void nml_k_set( PLAYER_WORK *pl_w );
static void nml_p_set( PLAYER_WORK *pl_w );
static void jump_k_set( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Start the throw that pl00_catch_ck chose			*/
/****************************************************************/
void	pl00_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_k_set, nml_p_set, jump_k_set,
	} ;

	catch_set_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_k_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->pl_dir ^= 1 ;
	pl_w->disp_dir = pl_w->pl_dir ;
	if( !( pl_w->dir & 2 ) ){
		pl_w->pl_dir ^= 1 ;
		pl_w->disp_dir = pl_w->pl_dir ;
	}
	scr_excep_set( pl_w, 5, 5 ) ;
	ofs.x = -DOT_X( 50.0f ) ;
	ofs.y = DOT_Y( 74.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 0 ) ;
}

/*==============================================================*/
static void nml_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->pl_dir ^= 1 ;
	pl_w->disp_dir = pl_w->pl_dir ;
	if( !( pl_w->dir & 2 ) ){
		pl_w->pl_dir ^= 1 ;
		pl_w->disp_dir = pl_w->pl_dir ;
	}
	scr_excep_set( pl_w, 5, 5 ) ;
	ofs.x = -DOT_X( 50.0f ) ;
	ofs.y = DOT_Y( 74.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 1 ) ;
}

/*==============================================================*/
static void jump_k_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->pl_dir ^= 1 ;
	pl_w->disp_dir = pl_w->pl_dir ;
	if( !( pl_w->dir & 2 ) ){
		pl_w->pl_dir ^= 1 ;
		pl_w->disp_dir = pl_w->pl_dir ;
	}
	scr_excep_set( pl_w, 5, 5 ) ;
	ofs.x = -DOT_X( 50.0f ) ;
	ofs.y = DOT_Y( 74.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y /= 2.0f ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 2 ) ;
}

static void nml_k_catch( PLAYER_WORK *pl_w );
static void nml_p_catch( PLAYER_WORK *pl_w );
static void jump_k_catch( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void	pl00_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_k_catch, nml_p_catch, jump_k_catch,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_k_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->pl_stance = STANCE_AIR ;
		SCR_EXCEP_SET ;
		em->atck_no = pl00_at_throw_k ;
		em->pl_dir = pl_w->pl_dir ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			The throw over the shoulder: Ryu turns round		*/
/*			with it												*/
/*==============================================================*/
static void nml_p_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->pl_stance = STANCE_AIR ;
		SCR_EXCEP_SET ;
		em->atck_no = pl00_at_throw_p ;
		em->pl_dir = pl_w->pl_dir ;
		pl_w->pl_dir = pl_w->pl_dir ^ 1 ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		pl_dir_set( pl_w ) ;
		pl_stand_ret( pl_w ) ;
	}
}

/* the air throw: Ryu falls with the opponent to a little above the floor */
/*==============================================================*/
static void jump_k_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_w->routine_2 = 0 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y + DOT_X( 40.0f ) ) pl_w->pos.y = pl_w->ground_y + DOT_X( 40.0f ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->pl_stance = STANCE_AIR ;
		SCR_EXCEP_SET ;
		em->atck_no = pl00_at_air_throw ;
		em->pl_dir = pl_w->pl_dir ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static int shinkuu_delay_ck( PLAYER_WORK *pl_w );
static int shinkuu_tatsu_delay_ck( PLAYER_WORK *pl_w );
static int shin_shouryu_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl00_delay_ck( PLAYER_WORK *pl_w )
{
	if( shin_shouryu_delay_ck( pl_w ) || shinkuu_delay_ck( pl_w ) || shinkuu_tatsu_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static int shinkuu_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_shinkuu, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR ) return 0 ;
	pl_w->delay_sp_type = 5 ;
	return 1 ;
}

/*==============================================================*/
static int shinkuu_tatsu_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_shinkuu_tatsu, &pl_w->com_w5 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 6 ;
	return 1 ;
}

/*==============================================================*/
static int shin_shouryu_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_shin_shouryu, &pl_w->com_w6 ) ) return 0 ;
	if( *pl_w->gauge_level < 3 ) return 0 ;
	pl_w->delay_sp_type = 11 ;
	return 1 ;
}

static void hold( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Holding the opponent								*/
/****************************************************************/
void	pl00_hold( PLAYER_WORK *pl_w )
{
	static void ( *hold_jmp[])( PLAYER_WORK *pl_w ) = {
		hold, hold, hold,
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
void	pl00_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl00_sp_shinkuu ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl00_sp_shinkuu ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl00_sp_shinkuu_tatsu ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl00_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl00_sp_shinkuu ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl00_sp_shinkuu ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl00_sp_shinkuu_tatsu ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl00_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl00_sp_shouryu ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl00_sp_hadou ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl00_sp_tatsu ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void	pl00_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl00_sp_shouryu ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl00_sp_hadou ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl00_sp_tatsu ;
		pl_w->atck_level = 1 ;
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
void	pl00_t_kick( PLAYER_WORK *pl_w )
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
	pl_w->atck_no = pl00_at_t_kick ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 8 ) ;
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
		pl_char_set( pl_w, Char_Extra, 9 ) ;
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

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl00.c" End of File	*/

/* Ken (character 27): command checks, normal attacks, dashes, appearance and win poses, special
   moves and hyper combos, throws, partner moves and the wall kick. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl27_def.h"

void pl27_main( PLAYER_WORK *pl_w );
void pl27_com_ck( PLAYER_WORK *pl_w );
void pl27_atck_set( PLAYER_WORK *pl_w );
void pl27_atck( PLAYER_WORK *pl_w );
void pl27_dash( PLAYER_WORK *pl_w );
void pl27_noki( PLAYER_WORK *pl_w );
void pl27_appear( PLAYER_WORK *pl_w );
void pl27_win( PLAYER_WORK *pl_w );
void pl27_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl27_catch_ck( PLAYER_WORK *pl_w );
void pl27_catch_set( PLAYER_WORK *pl_w );
void pl27_catch( PLAYER_WORK *pl_w );
void pl27_hold( PLAYER_WORK *pl_w );
int pl27_delay_ck( PLAYER_WORK *pl_w );
void pl27_t_kick( PLAYER_WORK *pl_w );
void pl27_upper( PLAYER_WORK *pl_w );
void pl27_sp_unison( PLAYER_WORK *pl_w );
void pl27_sp_gattai( PLAYER_WORK *pl_w );
void pl27_sp_assist( PLAYER_WORK *pl_w );
void pl27_sp_zero( PLAYER_WORK *pl_w );

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
static Uint16 com_hadou[] = { 3, 0, pl27_sp_hadou, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Hadouken */
static Uint16 com_shouryuu[] = { 3, 0, pl27_sp_shouryuu, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_PUNCH } ;	/* Shouryuuken */
static Uint16 com_tatsumaki[] = { 3, COM_GROUND, pl27_sp_tatsumaki, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Tatsumaki Senpuukyaku */
static Uint16 com_jtatsumaki[] = { 3, COM_AIR, pl27_sp_jtatsumaki, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Tatsumaki Senpuukyaku in the air */
static Uint16 com_reppa[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl27_sp_reppa, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Shouryuu Reppa (hyper) */
static Uint16 com_shinryuu[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl27_sp_shinryuu, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Shinryuuken (hyper) */
static Uint16 com_shippu[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl27_sp_shippu, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Shippuu Jinraikyaku (hyper) */
static Uint16 com_zenpou[] = { 3, COM_GROUND, pl27_sp_zenpou, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Zenpou Tenshin (roll) */

static void ( *pl27_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl27_main,
	pl27_com_ck,
	pl27_atck_set,
	pl27_atck,
	pl27_dash,
	pl27_noki,
	pl27_appear,
	pl27_win,
	pl27_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl27_catch_ck,	/* returns a value */
	pl27_catch_set,
	pl27_catch,
	pl27_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl27_delay_ck,
	pl27_t_kick,
	pl27_upper,
	pl27_sp_unison,
	pl27_sp_gattai,
	pl27_sp_assist,
	pl27_sp_zero,
} ;

static Uint8 hadou_ck( PLAYER_WORK *pl_w );
static Uint8 shouryuu_ck( PLAYER_WORK *pl_w );
static Uint8 tatsumaki_ck( PLAYER_WORK *pl_w );
static Uint8 jtatsumaki_ck( PLAYER_WORK *pl_w );
static Uint8 zenpou_ck( PLAYER_WORK *pl_w );
static Uint8 reppa_ck( PLAYER_WORK *pl_w );
static Uint8 shinryuu_ck( PLAYER_WORK *pl_w );
static Uint8 shippu_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static int chouhatsu_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl27_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl27_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl27_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( reppa_ck( pl_w ) ) return ;
	if( shinryuu_ck( pl_w ) ) return ;
	if( shippu_ck( pl_w ) ) return ;
	if( shouryuu_ck( pl_w ) ) return ;
	if( hadou_ck( pl_w ) ) return ;
	if( tatsumaki_ck( pl_w ) ) return ;
	if( zenpou_ck( pl_w ) ) return ;
	if( jtatsumaki_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
/*			On the ground, or in the air once a jump; only		*/
/*			while shot_flag is clear							*/
/*==============================================================*/
static Uint8 hadou_ck( PLAYER_WORK *pl_w )
{
	PL27_LOCAL_W *lw = ( PL27_LOCAL_W * )pl_w->tlocal ;
	Uint8 sp ;

	if( !com_nml_ck( pl_w, com_hadou, &pl_w->com_w ) ) return 0 ;
	if( lw->shot_flag ) return 0 ;
	sp = pl27_sp_hadou ;
	if( pl_w->pl_stance == STANCE_AIR ){
		if( !pl_w->sjump_flag ){
			if( pl_w->jump_com_cnt ) return 0 ;
			pl_w->jump_com_cnt ++ ;
		}
		sp = pl27_sp_jhadou ;
	}
	com_sw_set( pl_w, &pl_w->com_w ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = sp ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 shouryuu_ck( PLAYER_WORK *pl_w )
{
	Uint8 sp ;

	if( !com_nml_ck( pl_w, com_shouryuu, &pl_w->com_w0 ) ) return 0 ;
	sp = pl27_sp_shouryuu ;
	if( pl_w->pl_stance == STANCE_AIR ){
		if( !pl_w->sjump_flag ){
			if( pl_w->jump_com_cnt ) return 0 ;
			pl_w->jump_com_cnt ++ ;
		}
		sp = pl27_sp_jshouryuu ;
	}
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->pl_sp_type = sp ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 tatsumaki_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_tatsumaki, &pl_w->com_w1 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->pl_sp_type = pl27_sp_tatsumaki ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
/*			In the air: once a jump, unless super jumping		*/
/*==============================================================*/
static Uint8 jtatsumaki_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_jtatsumaki, &pl_w->com_w2 ) ) return 0 ;
	if( pl_w->spd.y < 0.0f && pl_w->pos.y - pl_w->ground_y < DOT_Y( 48.0f ) ) return 1 ;
	if( !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->pl_sp_type = pl27_sp_jtatsumaki ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 zenpou_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_zenpou, &pl_w->com_w7 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w7 ) ;
	pl_w->pl_sp_type = pl27_sp_zenpou ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 reppa_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_reppa, &pl_w->com_w5 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl27_sp_reppa ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 shinryuu_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_shinryuu, &pl_w->com_w6 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl27_sp_shinryuu ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 shippu_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_shippu, &pl_w->com_w8 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl27_sp_shippu ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static int snapback_ck( PLAYER_WORK *pl_w )
{
	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl27_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static int chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 9 ) ) return 0 ;
	pl_w->pl_sp_type = pl27_sp_chouhatsu ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/****************************************************************/
void pl27_main( PLAYER_WORK *pl_w )
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
void pl27_atck_set( PLAYER_WORK *pl_w )
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
/*			The heavy kick with the stick forward is another	*/
/*			kick (routine_2 set)								*/
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
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_STAND_HK ;
		pl_w->routine_2 = 0 ;
		if( pl_w->atck_lever & KEY_FORWARD ){
			pl_w->routine_2 = 1 ;
			pl_w->char_no.b.grp = 20 ;
			pl_w->char_no.b.no = 0 ;
			pl_w->atck_no = pl27_at_fwd_hk_1 ;
		}
		pl_se_req( pl_w, SE_SWING_H ) ;
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
	pl_w->char_no.b.grp = 12 ;
	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = pl27_at_jump_lk2 ;
		pl_w->char_no.b.grp = 20 ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_lk_tbl ;
		else pl_w->chain_tbl = sj_lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->char_no.b.grp = 20 ;
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = pl27_at_jump_mk2 ;
		if( pl_w->atck_lever & KEY_UP ){
			pl_w->char_no.b.grp = 12 ;
			pl_w->char_no.b.no = 1 ;
			pl_w->atck_no = ATCK_JUMP_MK ;
		}
		pl_se_req( pl_w, SE_SWING_M ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mk_tbl ;
		else pl_w->chain_tbl = sj_mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 3 ;
		pl_w->atck_no = pl27_at_jump_hk2 ;
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
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
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
void pl27_atck( PLAYER_WORK *pl_w )
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
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			The light and middle kicks hit again on the			*/
/*			animation's mark; the forward kick also steps		*/
/*==============================================================*/
static void stand_kick_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
	case 1:
		if( pl_char_move( pl_w ) < 0 ){
			pl_stand_ret( pl_w ) ;
			return ;
		}
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pl_w->atck_no = pl27_at_stand_lk_2 ;
			HIT_RESET( pl_w ) ;
		}
		break ;
	case 2:
		if( pl_char_move( pl_w ) < 0 ){
			pl_stand_ret( pl_w ) ;
			break ;
		}
		if( pl_w->routine_2 ){
			if( pl_w->cg_type ){
				pl_w->cg_type = 0 ;
				pl_w->atck_no = pl27_at_fwd_hk_2 ;
				HIT_RESET( pl_w ) ;
			}
			if( pl_w->cg_prm ) pl_w->pos.x += pl_w->pl_dir ? DOT_X( 2.0f ) : -DOT_X( 2.0f ) ;
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
void pl27_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_w->atck_no = pl27_at_upper ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_char_set( pl_w, Char_Extra, 8 ) ;
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
static void dash_stop( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A hop forward										*/
/****************************************************************/
void pl27_dash( PLAYER_WORK *pl_w )
{
	static void ( *dash_jmp[])( PLAYER_WORK *pl_w ) = {
		dash_start, dash_move, dash_stop,
	} ;

	dash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dash_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 9.5f ) : -DOT_X( 9.5f ) ;
		pl_w->acc.x = pl_w->pl_dir ? -0.390625f : 0.390625f ;
		pl_w->spd.y = DOT_Y( 2.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.25f ) ;
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
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
}

/*==============================================================*/
/*			Slows down until the speed turns round				*/
/*==============================================================*/
static void dash_stop( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->acc.x * pl_w->spd.x > 0.0f ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	pl_smoke_set( pl_w ) ;
	pl_char_move( pl_w ) ;
}

static void noki_start( PLAYER_WORK *pl_w );
static void noki_move( PLAYER_WORK *pl_w );
static void noki_stop( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash										*/
/****************************************************************/
void pl27_noki( PLAYER_WORK *pl_w )
{
	static void ( *noki_jmp[])( PLAYER_WORK *pl_w ) = {
		noki_start, noki_move, noki_stop,
	} ;

	noki_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void noki_start( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 11.0f ) : DOT_X( 11.0f ) ;
	pl_w->acc.x = pl_w->pl_dir ? 0.33854166f : -0.33854166f ;
	pl_w->timer0 = 20 ;
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
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->acc.x * pl_w->spd.x > 0.0f ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	pl_smoke_set( pl_w ) ;
	pl_char_move( pl_w ) ;
}

static void appear_init( PLAYER_WORK *pl_w );
static void appear_move( PLAYER_WORK *pl_w );
static void win_init( PLAYER_WORK *pl_w );
static void win_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl27_appear( PLAYER_WORK *pl_w )
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

/* the win poses for a KO, picked at random with a voice, and the pose for each other end of a
   round */
static Sint8 win_ko_tbl[8] = { 0, 1, 2, 1, 5, 1, 2, 5 } ;
static Sint8 win_tbl[5] = { 3, 3, 0, 3, 3 } ;

/****************************************************************/
void pl27_win( PLAYER_WORK *pl_w )
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
	if( !pl_w->type ){
		no = win_ko_tbl[Rnd() & 7] ;
		pl_voice_req( pl_w, ( Rnd() & 1 ) + 15 ) ;
	}
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
static void sp_shouryuu( PLAYER_WORK *pl_w );
static void sp_tatsumaki( PLAYER_WORK *pl_w );
static void sp_jtatsumaki( PLAYER_WORK *pl_w );
static void sp_jhadou( PLAYER_WORK *pl_w );
static void sp_zenpou( PLAYER_WORK *pl_w );
static void sp_reppa( PLAYER_WORK *pl_w );
static void sp_shinryuu( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_jshouryuu( PLAYER_WORK *pl_w );
static void sp_shippu( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );

/****************************************************************/
void	pl27_sp_atck( PLAYER_WORK *pl_w )
{
	static void (*pl_sp_tbl[])( PLAYER_WORK *pl_w ) ={
		sp_hadou,					/* 00 Hadouken */
		sp_shouryuu,					/* 01 Shouryuuken */
		sp_tatsumaki,					/* 02 Tatsumaki Senpuukyaku */
		sp_jtatsumaki,					/* 03 Tatsumaki Senpuukyaku in the air */
		sp_jhadou,					/* 04 Hadouken in the air */
		sp_zenpou,					/* 05 Zenpou Tenshin (roll) */
		sp_reppa,					/* 06 Shouryuu Reppa (hyper) */
		sp_shinryuu,					/* 07 Shinryuuken (hyper) */
		sp_hadou,					/* 08 */
		sp_chouhatsu,					/* 09 Taunt */
		sp_hadou,					/* 0a */
		sp_jshouryuu,					/* 0b Shouryuuken in the air */
		sp_hadou,					/* 0c */
		sp_hadou,					/* 0d */
		sp_shippu,					/* 0e Shippuu Jinraikyaku (hyper) */
		sp_hadou,					/* 0f */
		sp_snapback,					/* 10 Snapback */
	} ;

	pl_sp_tbl[ pl_w->pl_sp_type ]( pl_w ) ;
}

static void hadou_init( PLAYER_WORK *pl_w, PL27_LOCAL_W *lw );
static void hadou_move( PLAYER_WORK *pl_w, PL27_LOCAL_W *lw );

/*==============================================================*/
/*			A shot along the ground								*/
/*==============================================================*/
static void sp_hadou( PLAYER_WORK *pl_w )
{
	static void ( *hadou_jmp[])( PLAYER_WORK *pl_w, PL27_LOCAL_W *lw ) = {
		hadou_init, hadou_move,
	} ;

	hadou_jmp[pl_w->routine_2]( pl_w, ( PL27_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void hadou_init( PLAYER_WORK *pl_w, PL27_LOCAL_W *lw )
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
static void hadou_move( PLAYER_WORK *pl_w, PL27_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shl27_00_set( pl_w, 0 ) ;
	}
}

/*==============================================================*/
/*			Heavier once past the top of the jump				*/
/*==============================================================*/
static void shouryuu_gravity( PLAYER_WORK *pl_w )
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
static void shouryuu_hit( PLAYER_WORK *pl_w, PL27_LOCAL_W *lw )
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

static void shouryuu_init( PLAYER_WORK *pl_w, PL27_LOCAL_W *lw );
static void shouryuu_start( PLAYER_WORK *pl_w, PL27_LOCAL_W *lw );
static void shouryuu_jump( PLAYER_WORK *pl_w, PL27_LOCAL_W *lw );
static void shouryuu_fall( PLAYER_WORK *pl_w, PL27_LOCAL_W *lw );
static void shouryuu_land( PLAYER_WORK *pl_w, PL27_LOCAL_W *lw );

/* hits for each strength, counted apart by the side of hit_flag (hit_cnt, guard_cnt) */
static Uint8 shouryuu_hits[2 * 2] = { 1, 1, 3, 3 } ;

/*==============================================================*/
/*			A rising uppercut									*/
/*==============================================================*/
static void sp_shouryuu( PLAYER_WORK *pl_w )
{
	static void ( *shouryuu_jmp[])( PLAYER_WORK *pl_w, PL27_LOCAL_W *lw ) = {
		shouryuu_init, shouryuu_start, shouryuu_jump, shouryuu_fall, shouryuu_land,
	} ;

	shouryuu_jmp[pl_w->routine_2]( pl_w, ( PL27_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void shouryuu_init( PLAYER_WORK *pl_w, PL27_LOCAL_W *lw )
{
	Float spd_x ;

	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->routine_2 ++ ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_gauge_add( pl_w, 10 ) ;
	if( !pl_w->atck_level ){
		spd_x = DOT_X( 8.0f ) ;
		pl_w->spd.y = DOT_Y( 8.0f ) ;
	} else {
		spd_x = 20.0f ;
		pl_w->spd.y = DOT_Y( 16.0f ) ;
	}
	if( !pl_w->pl_dir ) spd_x = -spd_x ;
	pl_w->spd.x = spd_x ;
	pl_w->acc.x = pl_w->pl_dir ? -0.7291666f : 0.7291666f ;
	pl_w->acc.y = -DOT_Y( 0.5f ) ;
	pl_w->atck_no = pl_w->atck_level + 51 ;
	HIT_RESET( pl_w ) ;
	lw->hit_cnt = shouryuu_hits[pl_w->atck_level * 2] ;
	lw->guard_cnt = shouryuu_hits[pl_w->atck_level * 2 + 1] ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 2 ) ;
}

/*--------------------------------------------------------------*/
static void shouryuu_start( PLAYER_WORK *pl_w, PL27_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	shouryuu_hit( pl_w, lw ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_air_set( pl_w ) ;
		pl_sp_ef_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void shouryuu_jump( PLAYER_WORK *pl_w, PL27_LOCAL_W *lw )
{
	Float now, old ;

	shouryuu_gravity( pl_w ) ;
	old = pl_w->spd.x ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	now = pl_w->spd.x ;
	if( now * old < 0.0f ) pl_w->routine_2 ++ ;
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shouryuu_fall( PLAYER_WORK *pl_w, PL27_LOCAL_W *lw )
{
	shouryuu_gravity( pl_w ) ;
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
static void shouryuu_land( PLAYER_WORK *pl_w, PL27_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			Another hit on each of the animation's marks		*/
/*==============================================================*/
static void tatsumaki_hit( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type > 0 ){
		pl_w->cg_type = 0 ;
		pl_w->atck_no = pl_w->atck_level + 58 ;
		HIT_RESET( pl_w ) ;
	}
}

static void tatsumaki_init( PLAYER_WORK *pl_w );
static void tatsumaki_rise( PLAYER_WORK *pl_w );
static void tatsumaki_spin( PLAYER_WORK *pl_w );
static void tatsumaki_spin_end( PLAYER_WORK *pl_w );
static void tatsumaki_fall( PLAYER_WORK *pl_w );
static void tatsumaki_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A spinning kick that travels forward				*/
/*==============================================================*/
static void sp_tatsumaki( PLAYER_WORK *pl_w )
{
	static void ( *tatsumaki_jmp[])( PLAYER_WORK *pl_w ) = {
		tatsumaki_init, tatsumaki_rise, tatsumaki_spin, tatsumaki_spin_end, tatsumaki_fall, tatsumaki_land,
	} ;

	tatsumaki_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void tatsumaki_init( PLAYER_WORK *pl_w )
{
	Float spd ;

	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_gauge_add( pl_w, 10 ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->atck_no = pl_w->atck_level + 58 ;
	HIT_RESET( pl_w ) ;
	pl_air_set( pl_w ) ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	spd = pl_w->atck_level ? DOT_X( 8.0f ) : DOT_X( 5.0f ) ;
	pl_w->spd.y = pl_w->atck_level ? DOT_Y( 4.0f ) : DOT_Y( 1.25f ) ;
	pl_w->spd.x = pl_w->pl_dir ? spd : -spd ;
	pl_char_set( pl_w, Char_Special, 4 ) ;
}

/*--------------------------------------------------------------*/
static void tatsumaki_rise( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 3 ;
		pl_char_set( pl_w, Char_Special, 6 ) ;
		tatsumaki_hit( pl_w ) ;
	}
}

/*==============================================================*/
/*			Spins three times; the last turn is another			*/
/*			attack when the spin has hit						*/
/*==============================================================*/
static void tatsumaki_spin( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->cg_prm ){
		pl_w->cg_prm = 0 ;
		if( -- pl_w->timer0 <= 0 ){
			pl_w->routine_2 ++ ;
			if( pl_w->hit_flag ){
				pl_w->atck_no = pl27_at_tatsumaki_last ;
				HIT_RESET( pl_w ) ;
				pl_char_set( pl_w, Char_Special, 25 ) ;
				return ;
			}
		}
	}
	pl_char_move( pl_w ) ;
	tatsumaki_hit( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void tatsumaki_spin_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = -2.142857f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_char_set( pl_w, Char_Special, 5 ) ;
	}
}

/*--------------------------------------------------------------*/
static void tatsumaki_fall( PLAYER_WORK *pl_w )
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
static void tatsumaki_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void jtatsumaki_hit( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type > 0 ){
		pl_w->cg_type = 0 ;
		pl_w->atck_no = pl_w->atck_level + 61 ;
		HIT_RESET( pl_w ) ;
	}
}

static void jtatsumaki_init( PLAYER_WORK *pl_w );
static void jtatsumaki_spin( PLAYER_WORK *pl_w );
static void jtatsumaki_fall( PLAYER_WORK *pl_w );
static void jtatsumaki_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The spinning kick in the air						*/
/*==============================================================*/
static void sp_jtatsumaki( PLAYER_WORK *pl_w )
{
	static void ( *jtatsumaki_jmp[])( PLAYER_WORK *pl_w ) = {
		jtatsumaki_init, jtatsumaki_spin, jtatsumaki_fall, jtatsumaki_land,
	} ;

	jtatsumaki_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			Adds to the jump's speed, the way it is already		*/
/*			moving												*/
/*==============================================================*/
static void jtatsumaki_init( PLAYER_WORK *pl_w )
{
	Float spd ;

	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->atck_no = pl_w->atck_level + 61 ;
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
	pl_w->timer0 = pl_w->atck_level * 2 + 1 ;
	pl_char_set( pl_w, Char_Special, 8 ) ;
}

/*--------------------------------------------------------------*/
static void jtatsumaki_spin( PLAYER_WORK *pl_w )
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
	jtatsumaki_hit( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void jtatsumaki_fall( PLAYER_WORK *pl_w )
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
static void jtatsumaki_land( PLAYER_WORK *pl_w )
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

static void jhadou_init( PLAYER_WORK *pl_w, PL27_LOCAL_W *lw );
static void jhadou_move( PLAYER_WORK *pl_w, PL27_LOCAL_W *lw );
static void jhadou_end( PLAYER_WORK *pl_w, PL27_LOCAL_W *lw );

/*==============================================================*/
/*			The shot thrown from a jump							*/
/*==============================================================*/
static void sp_jhadou( PLAYER_WORK *pl_w )
{
	static void ( *jhadou_jmp[])( PLAYER_WORK *pl_w, PL27_LOCAL_W *lw ) = {
		jhadou_init, jhadou_move, jhadou_end,
	} ;

	jhadou_jmp[pl_w->routine_2]( pl_w, ( PL27_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
/*			The jump slows down while the shot is thrown		*/
/*==============================================================*/
static void jhadou_init( PLAYER_WORK *pl_w, PL27_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_gauge_add( pl_w, 2 ) ;
	pl_w->atck_no = pl_w->atck_level + 64 ;
	HIT_RESET( pl_w ) ;
	pl_w->spd.x /= 16.0f ;
	pl_w->spd.y /= 8.0f ;
	pl_w->acc.y /= 64.0f ;
	pl_w->acc.x = 0.0f ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 14 ) ;
}

/*--------------------------------------------------------------*/
static void jhadou_move( PLAYER_WORK *pl_w, PL27_LOCAL_W *lw )
{
	jhadou_xy_move( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		shl27_00_set( pl_w, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void jhadou_end( PLAYER_WORK *pl_w, PL27_LOCAL_W *lw )
{
	jhadou_xy_move( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_jump_ret( pl_w ) ;
	}
}

static void zenpou_init( PLAYER_WORK *pl_w );
static void zenpou_start( PLAYER_WORK *pl_w );
static void zenpou_move( PLAYER_WORK *pl_w );
static void zenpou_stop( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hop forward, longer for the heavy button			*/
/*==============================================================*/
static void sp_zenpou( PLAYER_WORK *pl_w )
{
	static void ( *zenpou_jmp[])( PLAYER_WORK *pl_w ) = {
		zenpou_init, zenpou_start, zenpou_move, zenpou_stop,
	} ;

	zenpou_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void zenpou_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_char_set( pl_w, Char_Dash, 0 ) ;
	zenpou_start( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void zenpou_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->spd.y = DOT_Y( 2.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.25f ) ;
		if( pl_w->atck_level ){
			pl_w->spd.x = pl_w->pl_dir ? DOT_X( 9.5f ) : -DOT_X( 9.5f ) ;
			pl_w->acc.x = pl_w->pl_dir ? -0.390625f : 0.390625f ;
		} else {
			pl_w->spd.x = pl_w->pl_dir ? DOT_X( 9.5f ) : -DOT_X( 9.5f ) ;
			pl_w->acc.x = pl_w->pl_dir ? -0.5208333f : 0.5208333f ;
		}
	}
}

/*--------------------------------------------------------------*/
static void zenpou_move( PLAYER_WORK *pl_w )
{
	pl_w->push_ng_timer = 2 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->acc.x = pl_w->pl_dir ? -DOT_X( 0.5f ) : DOT_X( 0.5f ) ;
	}
}

/*==============================================================*/
/*			Slows down until the speed turns round				*/
/*==============================================================*/
static void zenpou_stop( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->acc.x * pl_w->spd.x > 0.0f ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	pl_smoke_set( pl_w ) ;
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
/*			Heavier once past the top of the jump				*/
/*==============================================================*/
static void reppa_gravity( PLAYER_WORK *pl_w )
{
	Float old ;

	old = pl_w->spd.y ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( old * pl_w->spd.y < 0.0f ) pl_w->acc.y = -1.2053571f ;
}

/*==============================================================*/
/*			A hit on each of the animation's marks				*/
/*==============================================================*/
static void reppa_hit( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_prm ){
		pl_w->atck_no = pl_w->cg_prm ;
		HIT_RESET( pl_w ) ;
		pl_w->cg_prm = 0 ;
	}
}

static void reppa_init( PLAYER_WORK *pl_w );
static void reppa_wait( PLAYER_WORK *pl_w );
static void reppa_dash( PLAYER_WORK *pl_w );
static void reppa_rise( PLAYER_WORK *pl_w );
static void reppa_top( PLAYER_WORK *pl_w );
static void reppa_fall( PLAYER_WORK *pl_w );
static void reppa_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo: a dash and a rising uppercut with	*/
/*			many hits											*/
/*==============================================================*/
static void sp_reppa( PLAYER_WORK *pl_w )
{
	static void ( *reppa_jmp[])( PLAYER_WORK *pl_w ) = {
		reppa_init, reppa_wait, reppa_dash, reppa_rise,
		reppa_top, reppa_fall, reppa_land,
	} ;

	reppa_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void reppa_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->atck_no = pl27_at_reppa ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 0 ) ;
}

/*--------------------------------------------------------------*/
static void reppa_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 10.0f ) : -DOT_X( 10.0f ) ;
		pl_w->acc.x = pl_w->pl_dir ? -DOT_X( 0.375f ) : DOT_X( 0.375f ) ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = -40.0f ;
		ofs.y = DOT_Y( 24.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*==============================================================*/
/*			acc.x is cleared whether or not the speed turned	*/
/*			round												*/
/*==============================================================*/
static void reppa_dash( PLAYER_WORK *pl_w )
{
	Float old ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	old = pl_w->spd.x ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_w->spd.x * old < 0.0f ) pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_char_move( pl_w ) ;
	reppa_hit( pl_w ) ;
	if( pl_w->cg_type == 2 ){
		pl_w->cg_type = 0 ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 10.0f ) : -DOT_X( 10.0f ) ;
		pl_w->acc.x = pl_w->pl_dir ? -DOT_X( 0.375f ) : DOT_X( 0.375f ) ;
	}
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = pl_w->pl_dir ? 20.0f : -20.0f ;
		pl_w->acc.x = pl_w->pl_dir ? -DOT_X( 0.375f ) : DOT_X( 0.375f ) ;
		pl_w->spd.y = DOT_Y( 16.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.5f ) ;
	}
}

/*--------------------------------------------------------------*/
static void reppa_rise( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_char_move( pl_w ) ;
	reppa_hit( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_air_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void reppa_top( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_w->acc.x * pl_w->spd.x > 0.0f ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
	}
	reppa_fall( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void reppa_fall( PLAYER_WORK *pl_w )
{
	reppa_hit( pl_w ) ;
	reppa_gravity( pl_w ) ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_land_ef_set( pl_w ) ;
		reppa_land( pl_w ) ;
	} else if( !pl_w->cg_type ) pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void reppa_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			Hits on the animation's marks; one high in the		*/
/*			air is a hit of its own								*/
/*==============================================================*/
static void shinryuu_hit( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_flag1 > 0 ){
		if( pl_w->pos.y >= pl_w->ground_y + 720.0f ){
			pl_w->atck_no = pl27_at_shinryuu_top ;
			HIT_RESET( pl_w ) ;
			pl_w->atck_attr = 0 ;
		} else {
			pl_w->atck_no = pl_w->cg_flag1 ;
			HIT_RESET( pl_w ) ;
			pl_w->atck_attr = ATR_BACK_POS ;
		}
		pl_w->cg_flag1 = 0 ;
	}
}

static void shinryuu_init( PLAYER_WORK *pl_w );
static void shinryuu_wait( PLAYER_WORK *pl_w );
static void shinryuu_start( PLAYER_WORK *pl_w );
static void shinryuu_rise( PLAYER_WORK *pl_w );
static void shinryuu_fall( PLAYER_WORK *pl_w );
static void shinryuu_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo: a spinning uppercut straight up		*/
/*==============================================================*/
static void sp_shinryuu( PLAYER_WORK *pl_w )
{
	static void ( *shinryuu_jmp[])( PLAYER_WORK *pl_w ) = {
		shinryuu_init, shinryuu_wait, shinryuu_start, shinryuu_rise, shinryuu_fall, shinryuu_land,
	} ;

	shinryuu_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void shinryuu_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->atck_no = pl27_at_shinryuu ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 1 ) ;
}

/*==============================================================*/
/*			The height above the floor for each of the			*/
/*			animation's marks									*/
/*==============================================================*/
static void shinryuu_wait( PLAYER_WORK *pl_w )
{
	static Float shinryuu_y_tbl[5] = { 0.0f, 45.0f, DOT_Y( 46.0f ), 75.0f, 0.0f } ;
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		if( pl_w->cg_type > 0 ){
			pl_w->pos.y = pl_w->ground_y - shinryuu_y_tbl[pl_w->cg_type] ;
			return ;
		}
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		shinryuu_hit( pl_w ) ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = -40.0f ;
		ofs.y = DOT_Y( 24.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shinryuu_start( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	shinryuu_hit( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_air_set( pl_w ) ;
		pls27_01_set( pl_w ) ;
		pl_w->spd.y = DOT_Y( 16.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
	}
}

/*--------------------------------------------------------------*/
static void shinryuu_rise( PLAYER_WORK *pl_w )
{
	Float old ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	old = pl_w->spd.y ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( old * pl_w->spd.y < 0.0f ){
		pl_w->routine_2 ++ ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_char_set( pl_w, Char_Super, 2 ) ;
		return ;
	}
	if( sw_renda_ck( pl_w ) ) pl_w->cg_ctr = 1 ;
	pl_char_move( pl_w ) ;
	shinryuu_hit( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shinryuu_fall( PLAYER_WORK *pl_w )
{
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_land_ef_set( pl_w ) ;
	} else if( !pl_w->cg_type ) pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shinryuu_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Extra, 4 ) ;
	} else if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void jshouryuu_init( PLAYER_WORK *pl_w );
static void jshouryuu_start( PLAYER_WORK *pl_w );
static void jshouryuu_rise( PLAYER_WORK *pl_w );
static void jshouryuu_fall( PLAYER_WORK *pl_w );
static void jshouryuu_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The rising uppercut from a jump						*/
/*==============================================================*/
static void sp_jshouryuu( PLAYER_WORK *pl_w )
{
	static void ( *jshouryuu_jmp[])( PLAYER_WORK *pl_w ) = {
		jshouryuu_init, jshouryuu_start, jshouryuu_rise, jshouryuu_fall, jshouryuu_land,
	} ;

	jshouryuu_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void jshouryuu_init( PLAYER_WORK *pl_w )
{
	Float spd ;

	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_gauge_add( pl_w, 10 ) ;
	spd = pl_w->atck_level ? DOT_X( 10.0f ) : DOT_X( 8.0f ) ;
	pl_w->spd.x = pl_w->pl_dir ? spd : -spd ;
	pl_w->acc.x = pl_w->pl_dir ? -DOT_X( 0.375f ) : DOT_X( 0.375f ) ;
	pl_w->spd.y = pl_w->atck_level ? DOT_Y( 16.0f ) : DOT_Y( 10.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.5f ) ;
	pl_w->atck_no = pl27_at_jshouryuu ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 21 ) ;
}

/*--------------------------------------------------------------*/
static void jshouryuu_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void jshouryuu_rise( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_prm ){
		pl_w->atck_no = pl_w->cg_prm ;
		HIT_RESET( pl_w ) ;
		pl_w->cg_prm = 0 ;
	}
	if( pl_w->spd.y < 0.0f ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = -DOT_Y( 0.75f ) ;
		pl_char_set( pl_w, Char_Special, 27 ) ;
	}
}

/*--------------------------------------------------------------*/
static void jshouryuu_fall( PLAYER_WORK *pl_w )
{
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 23 ) ;
		pl_land_ef_set( pl_w ) ;
	} else {
		pl_char_move( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void jshouryuu_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			A hit on each of the animation's marks				*/
/*==============================================================*/
static void shippu_hit( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_flag1 ){
		pl_w->atck_no = pl_w->cg_flag1 ;
		HIT_RESET( pl_w ) ;
		pl_w->cg_flag1 = 0 ;
	}
}

static void shippu_init( PLAYER_WORK *pl_w );
static void shippu_wait( PLAYER_WORK *pl_w );
static void shippu_kick( PLAYER_WORK *pl_w );
static void shippu_rush( PLAYER_WORK *pl_w );
static void shippu_end( PLAYER_WORK *pl_w );
static void shippu_fall( PLAYER_WORK *pl_w );
static void shippu_land( PLAYER_WORK *pl_w );
static void shippu_miss( PLAYER_WORK *pl_w );
static void shippu_mash( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo: a run of kicks, longer for the		*/
/*			buttons pressed meanwhile (timer0)					*/
/*==============================================================*/
static void sp_shippu( PLAYER_WORK *pl_w )
{
	static void ( *shippu_jmp[])( PLAYER_WORK *pl_w ) = {
		shippu_init, shippu_wait, shippu_kick, shippu_rush,
		shippu_end, shippu_fall, shippu_land, shippu_miss,
	} ;

	shippu_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void shippu_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->timer0 = 0 ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.x = pl_w->pl_dir ? 7.5f : -7.5f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->atck_no = pl27_at_shippu ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 5 ) ;
	shippu_hit( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shippu_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	shippu_hit( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = -40.0f ;
		ofs.y = DOT_Y( 24.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*==============================================================*/
/*			The first kick; a hit on the even side goes on		*/
/*			to the rush											*/
/*==============================================================*/
static void shippu_kick( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_w->hit_flag && !( pl_w->hit_flag & HR_GUARD ) ) pl_w->timer0 = 1 ;
	if( pl_char_move( pl_w ) < 0 ){
		if( !pl_w->timer0 ){
			pl_w->routine_2 = 7 ;
			pl_char_set( pl_w, Char_Super, 6 ) ;
			pl_w->delay_flag = 0 ;
			pl_w->delay_timer = 0 ;
			pl_w->hyper_bg = 0 ;
			pl_w->hyper_bg_tm = 0 ;
			return ;
		}
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 0 ;
		pl_w->type = 0 ;
		pl_w->timer1 = 4 ;
		pl_char_set( pl_w, Char_Super, 7 ) ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 4.0f ) : -DOT_X( 4.0f ) ;
		pl_w->spd.y = DOT_Y( 4.0f ) ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		return ;
	}
	if( pl_w->cg_prm ){
		pls27_02_set( pl_w, pl_w->cg_prm - 1 ) ;
		pl_w->cg_prm = 0 ;
	}
	pl_w->cg_type = 1 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	shippu_hit( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shippu_rush( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		if( -- pl_w->timer1 < 0 ){
			if( ( pl_w->timer1 = pl_w->timer0 ) ){
				pl_w->pos.x += pl_w->spd.x ;
				pl_w->spd.x += pl_w->acc.x ;
				pl_w->pos.y += pl_w->spd.y ;
				pl_w->spd.y += pl_w->acc.y ;
				pl_w->timer0 = 0 ;
				pl_w->spd.y = 0.0f ;
				pl_w->acc.y = 0.0f ;
				pl_w->spd.x = 0.0f ;
				pl_w->acc.x = 0.0f ;
				pl_char_set( pl_w, Char_Super, 12 ) ;
				return ;
			}
			pl_w->routine_2 ++ ;
			pl_char_set( pl_w, Char_Super, 8 ) ;
			shippu_hit( pl_w ) ;
			return ;
		}
	}
	if( pl_w->cg_prm && pl_w->timer1 ){
		pls27_02_set( pl_w, pl_w->cg_prm - 1 ) ;
		pl_w->cg_prm = 0 ;
	}
	shippu_mash( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	shippu_hit( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shippu_end( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->spd.y = -DOT_Y( 8.0f ) ;
		pl_char_set( pl_w, Char_Special, 5 ) ;
	}
	shippu_hit( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shippu_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Jump, 3 ) ;
		pl_land_ef_set( pl_w ) ;
	}
	shippu_hit( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shippu_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
		return ;
	}
	shippu_hit( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shippu_miss( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
		return ;
	}
	shippu_hit( pl_w ) ;
}

/*==============================================================*/
/*			While moving, each third press of a kick adds a		*/
/*			turn of the rush, up to six							*/
/*==============================================================*/
static void shippu_mash( PLAYER_WORK *pl_w )
{
	if( pl_w->spd.y && ( pl_w->pad_sw.on & KEY_KICK ) ){
		pl_w->type ++ ;
		if( pl_w->type > 2 ){
			pl_w->type = 0 ;
			if( ++ pl_w->timer0 > 6 ) pl_w->timer0 = 6 ;
		}
	}
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
	pl_w->atck_no = pl27_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 28 ) ;
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
		ofs.x = -DOT_X( 58.0f ) ;
		ofs.y = DOT_Y( 60.0f ) ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
	if( pl_w->cg_prm ){
		spd = pl_w->pl_dir ? DOT_X( 2.0f ) : -DOT_X( 2.0f ) ;
		pl_w->pos.x += spd ;
	}
}

static PLAYER_WORK *nml_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *crouch_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *jump_catch_ck( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Can a throw start?									*/
/****************************************************************/
PLAYER_WORK *pl27_catch_ck( PLAYER_WORK *pl_w )
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
	if( pl_w->atck_pk == 0 && pl_w->atck_level == 1 && pl_w->pos.y > DOT_Y( 64.0f ) ){
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
static void jump_k_set( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Start the throw that pl27_catch_ck chose			*/
/****************************************************************/
void	pl27_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_set, nml_k_set, jump_k_set,
	} ;

	catch_set_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
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
	pl_char_set( pl_w, Char_Catch, 0 ) ;
}

/*==============================================================*/
/*			The rolling throw: Ken turns round only when		*/
/*			dir's bit 1 is clear								*/
/*==============================================================*/
static void nml_k_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

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
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 3 ) ;
}

static void nml_p_catch( PLAYER_WORK *pl_w );
static void nml_k_catch( PLAYER_WORK *pl_w );
static void jump_k_catch( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void	pl27_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_catch, nml_k_catch, jump_k_catch,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
/*			Ken hops with the opponent and stops on the			*/
/*			floor												*/
/*==============================================================*/
static void nml_p_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_w->routine_2 == 0 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = pl_w->pl_dir ? -7.5f : 7.5f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 2.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.125f ) ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->pl_stance = STANCE_AIR ;
		SCR_EXCEP_SET ;
		em->atck_no = pl27_at_throw ;
		em->pl_dir = pl_w->pl_dir ;
	}
	if( !pl_w->cg_prm ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->pos.y <= pl_w->ground_y ){
			pl_w->spd.y = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_w->pos.y = pl_w->ground_y ;
		}
	}
}

static void nml_k_init( PLAYER_WORK *pl_w );
static void nml_k_roll( PLAYER_WORK *pl_w );
static void nml_k_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The rolling throw: the buttons add rolls until		*/
/*			the time runs out									*/
/*==============================================================*/
static void nml_k_catch( PLAYER_WORK *pl_w )
{
	static void ( *nml_k_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_k_init, nml_k_roll, nml_k_end,
	} ;

	nml_k_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			Let go of the opponent at the end of the rolls		*/
/*==============================================================*/
static void nml_k_release( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	em = pl_w->catch_w ;
	em->dm_hit_w = pl_w ;
	em->catch_type = 1 ;
	em->pl_stance = STANCE_AIR ;
	SCR_EXCEP_SET ;
	em->atck_no = pl27_at_throw_k ;
	em->pl_dir = pl_w->pl_dir ^ 1 ;
	pl_char_set( pl_w, Char_Catch, 2 ) ;
}

/*==============================================================*/
static void nml_k_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	hold_renda_set( pl_w->catch_w, 30 ) ;
	catch_renda_set( pl_w, 3 ) ;
	pl_w->timer0 = 64 ;
}

/*--------------------------------------------------------------*/
static void nml_k_roll( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 <= 0 ){
		VEC3 pos ;

		nml_k_release( pl_w ) ;
		pl_w->routine_2 ++ ;
		pos.y = pl_w->pos.y + DOT_Y( 57.0f ) ;
		if( pl_w->disp_dir ) pos.x = 95.0f ;
		else pos.x = -95.0f ;
		pos.x += pl_w->pos.x ;
		pos.z = pl_w->pos.z ;
		hitmark_set( &pos, pl_w->disp_dir, 1, 0 ) ;
		pl_se_req( pl_w, 0 ) ;
		return ;
	}
	if( catch_renda_ck( pl_w ) ) pl_w->cg_ctr = 1 ;
	if( hold_renda_ck( pl_w->catch_w ) ){
		nml_k_release( pl_w ) ;
		pl_w->routine_2 ++ ;
		return ;
	}
	if( pl_w->cg_type ){
		VEC3 pos ;

		pl_w->cg_type = 0 ;
		pl_w->catch_w->dm_hit_w = pl_w ;
		pl_w->catch_w->atck_no = pl27_at_throw_k_hit ;
		catch_damage( pl_w ) ;
		pos.y = pl_w->pos.y + DOT_Y( 57.0f ) ;
		if( pl_w->disp_dir ) pos.x = 95.0f ;
		else pos.x = -95.0f ;
		pos.x += pl_w->pos.x ;
		pos.z = pl_w->pos.z ;
		hitmark_set( &pos, pl_w->disp_dir, 1, 0 ) ;
		pl_se_req( pl_w, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void nml_k_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void jump_k_dust( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The air throw: Ken falls to the floor, then			*/
/*			slides while the opponent is thrown					*/
/*==============================================================*/
static void jump_k_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_w->routine_2 == 0 ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		pl_char_move( pl_w ) ;
		if( pl_w->pos.y <= pl_w->ground_y ){
			pl_w->routine_2 ++ ;
			pl_w->pos.y = pl_w->ground_y ;
			pl_w->spd.x = pl_w->pl_dir ? -10.0f : 10.0f ;
			pl_w->acc.y = 0.0f ;
			pl_char_set( pl_w, Char_Catch, 4 ) ;
			pl_w->pl_stance = STANCE_STAND ;
			pl_se_req( pl_w, 48 ) ;
			jump_k_dust( pl_w ) ;
		}
	} else {
		if( !pl_w->cg_prm ) pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		if( pl_char_move( pl_w ) < 0 ){
			pl_stand_ret( pl_w ) ;
			return ;
		}
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			em = pl_w->catch_w ;
			em->dm_hit_w = pl_w ;
			em->catch_type = 1 ;
			em->pl_stance = STANCE_AIR ;
			SCR_EXCEP_SET ;
			em->atck_no = pl27_at_throw ;
			em->pl_dir = pl_w->pl_dir ;
		}
	}
}

/*==============================================================*/
static void jump_k_dust( PLAYER_WORK *pl_w )
{
	VEC3 pos ;

	pos.x = pl_w->pos.x ;
	pos.x += 1.6666666f ;
	pos.x += pl_w->disp_dir ? -120.0f : 120.0f ;
	pos.y = pl_w->ground_y ;
	pos.z = pl_w->pos.z ;
	fall_se_req( pl_w ) ;
	land_ef_set( &pos, 0 ) ;
}

static int reppa_delay_ck( PLAYER_WORK *pl_w );
static int shinryuu_delay_ck( PLAYER_WORK *pl_w );
static int shippu_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl27_delay_ck( PLAYER_WORK *pl_w )
{
	if( reppa_delay_ck( pl_w ) || shinryuu_delay_ck( pl_w ) || shippu_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static int reppa_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_reppa, &pl_w->com_w5 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 6 ;
	return 1 ;
}

/*==============================================================*/
static int shinryuu_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_shinryuu, &pl_w->com_w6 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 7 ;
	return 1 ;
}

/*==============================================================*/
static int shippu_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_shippu, &pl_w->com_w8 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 14 ;
	return 1 ;
}

static void hold( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Holding the opponent								*/
/****************************************************************/
void	pl27_hold( PLAYER_WORK *pl_w )
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
void	pl27_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl27_sp_shinryuu ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl27_sp_reppa ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl27_sp_reppa ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl27_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl27_sp_shinryuu ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl27_sp_reppa ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl27_sp_reppa ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl27_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl27_sp_shouryuu ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl27_sp_hadou ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl27_sp_tatsumaki ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void	pl27_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl27_sp_shouryuu ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl27_sp_hadou ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl27_sp_tatsumaki ;
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
void	pl27_t_kick( PLAYER_WORK *pl_w )
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
	pl_w->atck_no = pl27_at_t_kick ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 6 ) ;
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
		pl_char_set( pl_w, Char_Extra, 7 ) ;
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
/* "s_pl27.c" End of File	*/

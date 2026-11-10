/* Amingo (character 15): command checks, normal attacks, appearance, win and lose poses, special
   moves and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl15_def.h"

void pl15_main( PLAYER_WORK *pl_w );
void pl15_com_ck( PLAYER_WORK *pl_w );
void pl15_atck_set( PLAYER_WORK *pl_w );
void pl15_atck( PLAYER_WORK *pl_w );
void pl15_dash( PLAYER_WORK *pl_w );
void pl15_noki( PLAYER_WORK *pl_w );
void pl15_appear( PLAYER_WORK *pl_w );
void pl15_win( PLAYER_WORK *pl_w );
void pl15_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl15_catch_ck( PLAYER_WORK *pl_w );
void pl15_catch_set( PLAYER_WORK *pl_w );
void pl15_catch( PLAYER_WORK *pl_w );
void pl15_hold( PLAYER_WORK *pl_w );
int pl15_delay_ck( PLAYER_WORK *pl_w );
void pl15_t_kick( PLAYER_WORK *pl_w );
void pl15_upper( PLAYER_WORK *pl_w );
void pl15_sp_unison( PLAYER_WORK *pl_w );
void pl15_sp_gattai( PLAYER_WORK *pl_w );
void pl15_sp_assist( PLAYER_WORK *pl_w );
void pl15_sp_zero( PLAYER_WORK *pl_w );
void pl15_tlocal_init( PLAYER_WORK *pl_w );

/* commands: time or steps, kind, the special move, then the stick and buttons */
static Uint16 com_daichi_catch[] = { 3, COM_GROUND, pl15_sp_daichi_catch, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Daichi no Kodomo (capture) */
static Uint16 com_daichi[] = { 3, COM_GROUND, pl15_sp_daichi, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Daichi no Kodomo (punch/kick) */
static Uint16 com_midori[] = { 3, COM_GROUND, pl15_sp_midori, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Midori no Kanki */
static Uint16 com_kaze[] = { 3, 0, pl15_sp_kaze, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Kaze no Sakebi */
static Uint16 com_taiyou[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl15_sp_taiyou, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Taiyou no Megumi (hyper) */
static Uint16 com_okite[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl15_sp_okite, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Shokubutsu no Okite (hyper) */

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

static void ( *pl15_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl15_main,
	pl15_com_ck,
	pl15_atck_set,
	pl15_atck,
	pl15_dash,
	pl15_noki,
	pl15_appear,
	pl15_win,
	pl15_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl15_catch_ck,	/* returns a value */
	pl15_catch_set,
	pl15_catch,
	pl15_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl15_delay_ck,
	pl15_t_kick,
	pl15_upper,
	pl15_sp_unison,
	pl15_sp_gattai,
	pl15_sp_assist,
	pl15_sp_zero,
	0,
	0,
	0,
	pl15_tlocal_init,
} ;

static Uint8 daichi_catch_ck( PLAYER_WORK *pl_w );
static Uint8 daichi_ck( PLAYER_WORK *pl_w );
static Uint8 midori_ck( PLAYER_WORK *pl_w );
static Uint8 kaze_ck( PLAYER_WORK *pl_w );
static Uint8 taiyou_ck( PLAYER_WORK *pl_w );
static Uint8 okite_ck( PLAYER_WORK *pl_w );
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static Uint8 okite_delay_ck( PLAYER_WORK *pl_w );
static Uint8 taiyou_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl15_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl15_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl15_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( taiyou_ck( pl_w ) ) return ;
	if( okite_ck( pl_w ) ) return ;
	if( midori_ck( pl_w ) ) return ;
	if( kaze_ck( pl_w ) ) return ;
	if( daichi_catch_ck( pl_w ) ) return ;
	if( daichi_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
/*			Only while fewer than two are out					*/
/*==============================================================*/
static Uint8 daichi_catch_ck( PLAYER_WORK *pl_w )
{
	PL15_LOCAL_W *lw = ( PL15_LOCAL_W * )pl_w->tlocal ;

	if( !com_nml_ck( pl_w, com_daichi_catch, &pl_w->com_w0 ) ) return 0 ;
	if( lw->shot_cnt >= 2 ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl15_sp_daichi_catch ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 daichi_ck( PLAYER_WORK *pl_w )
{
	PL15_LOCAL_W *lw = ( PL15_LOCAL_W * )pl_w->tlocal ;

	if( !com_nml_ck( pl_w, com_daichi, &pl_w->com_w5 ) ) return 0 ;
	if( lw->shot_cnt >= 2 ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w5 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl15_sp_daichi ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 midori_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_midori, &pl_w->com_w1 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl15_sp_midori ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			Once a jump in the air								*/
/*==============================================================*/
static Uint8 kaze_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_kaze, &pl_w->com_w2 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl15_sp_kaze ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 taiyou_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_taiyou, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl15_sp_taiyou ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 okite_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_okite, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl15_sp_okite ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl15_delay_ck( PLAYER_WORK *pl_w )
{
	if( okite_delay_ck( pl_w ) || taiyou_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static Uint8 taiyou_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_taiyou, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 3 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 okite_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_okite, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 5 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 6 ) ) return 0 ;
	pl_w->pl_sp_type = pl15_sp_chouhatsu ;
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
	pl_w->pl_sp_type = pl15_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/****************************************************************/
/*			During the hyper combo sp_taiyou, scale follows		*/
/*			pl_scale											*/
/****************************************************************/
void pl15_main( PLAYER_WORK *pl_w )
{
	if( pl_w->old_act_st == N_SUPER && pl_w->pl_sp_type == pl15_sp_taiyou ) pl_w->scale = pl_w->pl_scale ;
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
void pl15_atck_set( PLAYER_WORK *pl_w )
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
		pl_w->chain_tbl = hp_tbl ;
		pl_w->nml_level = 2 ;
		pl_se_req( pl_w, SE_SWING_H ) ;
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
		pl_se_req( pl_w, SE_SWING_M ) ;
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
		pl_se_req( pl_w, SE_SWING_H ) ;
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
void pl15_atck( PLAYER_WORK *pl_w )
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
/*			The heavy one hits again							*/
/*==============================================================*/
static void stand_punch_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 2:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		else if( pl_w->cg_flag1 ){
			pl_w->atck_no = ATCK_STAND_HP ;
			HIT_RESET( pl_w ) ;
			pl_w->cg_flag1 = 0 ;
		}
		break ;
	case 0:
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
static void crouch_punch_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->nml_no == 2 && pl_w->cg_flag1 ){
		pl_w->atck_no = ATCK_CROUCH_HP ;
		HIT_RESET( pl_w ) ;
		pl_w->cg_flag1 = 0 ;
	}
}

/*==============================================================*/
static void stand_kick_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
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
/*			The heavy one hits again							*/
/*==============================================================*/
static void jump_kick_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 2:
		if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
		else if( pl_w->cg_flag1 ){
			pl_w->atck_no = ATCK_JUMP_HK ;
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

/****************************************************************/
/*			The launcher										*/
/****************************************************************/
void pl15_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_CROUCH ;
		if( pl_w->atck_pk == 0 ){
			pl_char_set( pl_w, Char_Extra, 3 ) ;
			pl_w->atck_no = pl15_at_upper_p ;
			HIT_RESET( pl_w ) ;
		} else {
			pl_char_set( pl_w, Char_Extra, 4 ) ;
			pl_w->atck_no = pl15_at_upper_k ;
			HIT_RESET( pl_w ) ;
		}
		pl_se_req( pl_w, SE_SWING_M ) ;
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
static void dash_end( PLAYER_WORK *pl_w );

/****************************************************************/
void pl15_dash( PLAYER_WORK *pl_w )
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
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 7.0f ) : -DOT_X( 7.0f ) ;
		pl_w->timer0 = 16 ;
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
		pl_char_set( pl_w, Char_Dash, 2 ) ;
		pl_w->acc.x = pl_w->pl_dir ? -DOT_X( 0.5f ) : DOT_X( 0.5f ) ;
	}
}

/*--------------------------------------------------------------*/
static void dash_end( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void noki_init( PLAYER_WORK *pl_w );
static void noki_jump( PLAYER_WORK *pl_w );
static void noki_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash, a hop								*/
/****************************************************************/
void pl15_noki( PLAYER_WORK *pl_w )
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
		pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 5.0f ) : DOT_X( 5.0f ) ;
		pl_w->acc.x = pl_w->pl_dir ? -0.15625f : 0.15625f ;
		pl_w->spd.y = 7.5f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
	}
	noki_jump( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void noki_jump( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->ground_y > pl_w->pos.y ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Dash, 3 ) ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
}

/*--------------------------------------------------------------*/
static void noki_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void appear_init( PLAYER_WORK *pl_w );
static void appear_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl15_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_init, appear_move,
	} ;

	appear_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void appear_init( PLAYER_WORK *pl_w )
{
	PL15_LOCAL_W *lw = ( PL15_LOCAL_W * )pl_w->tlocal ;

	pl_w->routine_2 ++ ;
	pl_w->disp_flag = 1 ;
	pl_char_set( pl_w, Char_Appear, 0 ) ;
	lw->shot_cnt = 0 ;
}

/*--------------------------------------------------------------*/
static void appear_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_w->routine_1 ++ ;
}

static void win_init( PLAYER_WORK *pl_w );
static void win_move( PLAYER_WORK *pl_w );
static void win_ko( PLAYER_WORK *pl_w );
static void lose_set( PLAYER_WORK *pl_w );
static void win_time( PLAYER_WORK *pl_w );
static void win_pose_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl15_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_init, win_move,
	} ;

	win_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			The pose for how the round ended					*/
/*==============================================================*/
static void win_init( PLAYER_WORK *pl_w )
{
	static void ( *win_init_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko, lose_set, win_time, lose_set, lose_set,
	} ;

	pl_w->routine_2 ++ ;
	win_init_jmp[pl_w->type]( pl_w ) ;
}

/*==============================================================*/
/*			Won by KO: one of two poses							*/
/*==============================================================*/
static void win_ko( PLAYER_WORK *pl_w )
{
	int r ;

	pl_w->routine_3 ++ ;
	r = nlRand() ;
	if( r & 1 ) pl_char_set( pl_w, Char_Win, 0 ) ;
	else pl_char_set( pl_w, Char_Win, 1 ) ;
}

/*==============================================================*/
static void lose_set( PLAYER_WORK *pl_w )
{
	pl_char_set( pl_w, Char_Win, 2 ) ;
}

/*==============================================================*/
static void win_time( PLAYER_WORK *pl_w )
{
	pl_char_set( pl_w, Char_Win, 0 ) ;
}

/*--------------------------------------------------------------*/
static void win_move( PLAYER_WORK *pl_w )
{
	static void ( *win_move_jmp[])( PLAYER_WORK *pl_w ) = {
		win_pose_move, win_pose_move, win_pose_move, win_pose_move, win_pose_move,
	} ;

	if( pl_win_end_ck( pl_w ) ) pl_stand_ret( pl_w ) ;
	else win_move_jmp[pl_w->type]( pl_w ) ;
}

/*==============================================================*/
static void win_pose_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_move( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A kick that leaps off the wall						*/
/****************************************************************/
void pl15_t_kick( PLAYER_WORK *pl_w )
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
	pl_w->atck_no = pl15_at_t_kick ;
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

static void sp_daichi_catch( PLAYER_WORK *pl_w );
static void sp_midori( PLAYER_WORK *pl_w );
static void sp_kaze( PLAYER_WORK *pl_w );
static void sp_taiyou( PLAYER_WORK *pl_w );
static void sp_bite( PLAYER_WORK *pl_w );
static void sp_okite( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Special moves; pl15_sp_daichi is sp_daichi_catch	*/
/*			with the other button								*/
/****************************************************************/
void pl15_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *pl_sp_tbl[])( PLAYER_WORK *pl_w ) = {
		sp_daichi_catch, sp_midori, sp_kaze, sp_taiyou, sp_bite, sp_okite, sp_chouhatsu, sp_snapback, sp_daichi_catch,
	} ;

	pl_sp_tbl[pl_w->pl_sp_type]( pl_w ) ;
}

static void daichi_catch_init( PLAYER_WORK *pl_w );
static void daichi_catch_move( PLAYER_WORK *pl_w );
static void daichi_catch_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_daichi_catch( PLAYER_WORK *pl_w )
{
	static void ( *daichi_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		daichi_catch_init, daichi_catch_move, daichi_catch_end,
	} ;

	daichi_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void daichi_catch_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl_w->atck_pk + 0x30 ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level ) ;
}

/*==============================================================*/
/*			Sends one out when the animation says				*/
/*==============================================================*/
static void daichi_catch_move( PLAYER_WORK *pl_w )
{
	void *ew ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		if( pl_w->pl_sp_type == pl15_sp_daichi_catch ) ew = shl15_01_set( pl_w, 0 ) ;
		else ew = shl15_01_set( pl_w, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void daichi_catch_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void kaze_init( PLAYER_WORK *pl_w );
static void kaze_start( PLAYER_WORK *pl_w );
static void kaze_move( PLAYER_WORK *pl_w );
static void kaze_stop( PLAYER_WORK *pl_w );
static void kaze_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A rolling dash, on the ground or in the air			*/
/*==============================================================*/
static void sp_kaze( PLAYER_WORK *pl_w )
{
	static void ( *kaze_jmp[])( PLAYER_WORK *pl_w ) = {
		kaze_init, kaze_start, kaze_move, kaze_stop, kaze_end,
	} ;

	kaze_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void kaze_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	if( pl_w->pl_stance != STANCE_AIR ){
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_sp_ef_set( pl_w ) ;
	} else pl_w->pl_stance = STANCE_AIR ;
	pl_w->atck_no = pl_w->atck_level + 0x34 ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 6 ) ;
}

/*--------------------------------------------------------------*/
static void kaze_start( PLAYER_WORK *pl_w )
{
	PL15_LOCAL_W *lw ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		lw = ( PL15_LOCAL_W * )pl_w->tlocal ;
		if( pl_w->pl_stance == STANCE_AIR ) pl_w->push_ng_timer = 3 ;
		pl_w->cg_type = 0 ;
		lw->kaze_hit_cnt = 0 ;
		lw->kaze_end_flag = 0 ;
		pl_w->cg_ctr = 1 ;
		pl_char_move( pl_w ) ;
		pls15_01_set( pl_w, 1 ) ;
		pls15_01_set( pl_w, 3 ) ;
		pls15_01_set( pl_w, 4 ) ;
		pls15_01_set( pl_w, 5 ) ;
		pls15_01_set( pl_w, 6 ) ;
		pls15_01_set( pl_w, 7 ) ;
		if( pl_w->pl_stance == STANCE_STAND ) pls15_01_set( pl_w, 8 ) ;
		if( !pl_w->atck_level ){
			pl_w->timer0 = 38 ;
			pl_w->spd.x = -25.0f ;
			pl_w->acc.x = 1.25f ;
		} else {
			pl_w->timer0 = 50 ;
			pl_w->spd.x = -DOT_X( 20.0f ) ;
			pl_w->acc.x = DOT_X( 0.875f ) ;
		}
		if( pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
	}
}

/*==============================================================*/
/*			Slows to a stop; each new hit is counted			*/
/*==============================================================*/
static void kaze_move( PLAYER_WORK *pl_w )
{
	PL15_LOCAL_W *lw = ( PL15_LOCAL_W * )pl_w->tlocal ;

	if( pl_w->pl_stance == STANCE_AIR ) pl_w->push_ng_timer = 3 ;
	if( pl_w->hit_flag ){ if( !( pl_w->hit_flag & HR_HIT ) ) lw->kaze_hit_cnt ++ ; }
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_flag1 && pl_w->cg_flag1 < 0x80 ){
		pl_w->atck_no = pl_w->cg_flag1 + 0x33 ;
		HIT_RESET( pl_w ) ;
		pl_w->cg_flag1 = 0 ;
	}
	pl_w->timer0 -- ;
	if( pl_w->timer0 <= 0 || pl_w->spd.x * pl_w->acc.x > 0.0f ){
		pl_w->routine_2 ++ ;
		if( pl_w->pl_stance == STANCE_AIR ) pl_w->push_ng_timer = 0 ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 8 ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		lw->kaze_end_flag = 1 ;
	}
}

/*--------------------------------------------------------------*/
static void kaze_stop( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type == 9 ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		if( pl_w->pl_stance == STANCE_AIR ){
			RATE_CLR_X( pl_w ) ;
			pl_jump_ret( pl_w ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void kaze_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		RATE_CLR_X( pl_w ) ;
		pl_stand_ret( pl_w ) ;
	}
}

static void taiyou_init( PLAYER_WORK *pl_w );
static void taiyou_move( PLAYER_WORK *pl_w );
static void taiyou_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo; acc.x and acc.y hold how far it		*/
/*			has grown											*/
/*==============================================================*/
static void sp_taiyou( PLAYER_WORK *pl_w )
{
	static void ( *taiyou_jmp[])( PLAYER_WORK *pl_w ) = {
		taiyou_init, taiyou_move, taiyou_end,
	} ;

	taiyou_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void taiyou_init( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	switch( pl_w->routine_3 ){
	case 0:
		if( pl_w->team_type == 6 ){
			pl_w->stop_move_flag = 0xff ;
			pl_w->stop_move_timer = 16 ;
		}
		pl_w->routine_3 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->atck_no = pl15_at_taiyou ;
		HIT_RESET( pl_w ) ;
		pl_w->atck_attr = ATR_BACK_POS ;
		pl_dir_set( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 0 ) ;
		pl_sp_ef_set( pl_w ) ;
		break ;
	case 1:
		pl_w->delay_timer = 2 ;
		pl_w->hyper_bg_tm = 5 ;
		pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
		pl_char_move( pl_w ) ;
		if( pl_w->cg_type ){
			pl_w->stop_move_flag = 0 ;
			pl_w->stop_move_timer = 0 ;
			pl_w->routine_3 ++ ;
			pl_w->cg_type = 0 ;
			ofs.x = 0.0f ;
			ofs.y = DOT_Y( 48.0f ) ;
			ofs.z = 0.0f ;
			pl_hyper_start( pl_w, &ofs, 1 ) ;
		}
		break ;
	case 2:
		pl_w->push_ng_timer = 3 ;
		pl_w->delay_timer = 2 ;
		pl_w->hyper_bg_tm = 5 ;
		pl_char_move( pl_w ) ;
		if( pl_w->cg_type ){
			pl_w->routine_3 ++ ;
			pl_w->cg_type = 0 ;
			pl_w->cg_ctr = 2 ;
			pls15_02_set( pl_w ) ;
		}
		break ;
	case 3:
		pl_w->push_ng_timer = 3 ;
		pl_w->delay_timer = 2 ;
		pl_w->hyper_bg_tm = 5 ;
		if( pl_char_move( pl_w ) < 0 ){
			pl_w->routine_2 ++ ;
			pl_w->routine_3 = 0 ;
			pl_w->timer0 = 60 ;
			pl_w->timer1 = 40 ;
			pl_w->acc.x = 34.0f ;
			pl_w->acc.y = 0.0f ;
			pl_w->scale = pl_w->pl_scale ;
			pl_char_set( pl_w, Char_Super, 1 ) ;
		}
		break ;
	}
}

/*==============================================================*/
/*			Pressing buttons makes it last longer				*/
/*==============================================================*/
static void taiyou_move( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->push_ng_timer = 3 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_flag1 && pl_w->cg_flag1 < 0x80 ){
		pl_w->atck_no = pl_w->cg_flag1 + 0x38 ;
		HIT_RESET( pl_w ) ;
		pl_w->atck_attr = ATR_BACK_POS ;
		pl_w->cg_flag1 = 0 ;
	}
	if( pl_w->acc.y < pl_w->acc.x ) pl_w->acc.y += 1.0f ;
	pl_w->scale.x = pl_w->pl_scale.x + pl_w->acc.y / pl_w->acc.x * 1.0f ;
	pl_w->scale.y = pl_w->pl_scale.y + pl_w->acc.y / pl_w->acc.x * 1.0f ;
	if( pl_w->timer1 && sw_renda_ck( pl_w ) ){
		pl_w->timer1 -- ;
		pl_w->timer0 ++ ;
	}
	if( -- pl_w->timer0 < 0 ){
		pl_w->push_ng_timer = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->acc.x = pl_w->acc.y = 34.0f ;
		pl_char_set( pl_w, Char_Super, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void taiyou_end( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_w->acc.y != 0.0f ) pl_w->acc.y -= 1.0f ;
	pl_w->scale.x = pl_w->pl_scale.x + pl_w->acc.y / pl_w->acc.x * 1.0f ;
	pl_w->scale.y = pl_w->pl_scale.y + pl_w->acc.y / pl_w->acc.x * 1.0f ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->scale = pl_w->pl_scale ;
		pl_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
static void sp_bite( PLAYER_WORK *pl_w )
{
	void *ew ;

	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, 14 ) ;
	} else {
		if( pl_char_move( pl_w ) < 0 ){
			pl_stand_ret( pl_w ) ;
			return ;
		}
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			ew = shl15_00_set( pl_w, 0 ) ;
		}
	}
}

static void okite_init( PLAYER_WORK *pl_w );
static void okite_reach( PLAYER_WORK *pl_w );
static void okite_miss( PLAYER_WORK *pl_w );
static void okite_catch( PLAYER_WORK *pl_w );
static void okite_reach_start( PLAYER_WORK *pl_w );
static void okite_reach_wait( PLAYER_WORK *pl_w );
static void okite_catch_init( PLAYER_WORK *pl_w );
static void okite_catch_pull( PLAYER_WORK *pl_w );
static void okite_catch_fly( PLAYER_WORK *pl_w );
static void okite_catch_hit( PLAYER_WORK *pl_w );
static void okite_catch_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo that reaches out and, if it			*/
/*			catches, pulls the opponent in						*/
/*==============================================================*/
static void sp_okite( PLAYER_WORK *pl_w )
{
	static void ( *okite_jmp[])( PLAYER_WORK *pl_w ) = {
		okite_init, okite_reach, okite_miss, okite_catch,
	} ;

	okite_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void okite_init( PLAYER_WORK *pl_w )
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
	pl_w->atck_no = pl15_at_okite_reach ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 3 ) ;
}

/*--------------------------------------------------------------*/
static void okite_reach( PLAYER_WORK *pl_w )
{
	static void ( *okite_reach_jmp[])( PLAYER_WORK *pl_w ) = {
		okite_reach_start, okite_reach_wait,
	} ;

	okite_reach_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void okite_reach_start( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = -DOT_X( 16.0f ) ;
		ofs.y = DOT_Y( 80.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
	if( pl_w->cg_prm ){
		pl_w->routine_3 ++ ;
		pl_w->timer0 = 120 ;
		if( !( pl_w->ef_w = shl15_02_set( pl_w, 0 ) ) ){
			pl_w->routine_2 ++ ;
			pl_w->routine_3 = 0 ;
			pl_char_set( pl_w, Char_Super, 4 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void okite_reach_wait( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 < 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_char_set( pl_w, Char_Super, 4 ) ;
	}
}

/*--------------------------------------------------------------*/
static void okite_miss( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void okite_catch( PLAYER_WORK *pl_w )
{
	static void ( *okite_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		okite_catch_init, okite_catch_pull, okite_catch_fly, okite_catch_hit, okite_catch_end,
	} ;

	pl_w->catch_flag = 1 ;
	okite_catch_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void okite_catch_init( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->routine_3 ++ ;
	pl_char_set( pl_w, Char_Super, 5 ) ;
}

/*==============================================================*/
/*			The opponent is thrown up to land where they are	*/
/*			held												*/
/*==============================================================*/
static void okite_catch_pull( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w ;
	VEC3 pos ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_w->cg_prm ){
		pl_w->cg_prm = 0 ;
		pl_w->routine_3 ++ ;
		pl_w->timer0 = 24 ;
		pl_char_set( pl_w, Char_Catch, 3 ) ;
		em_w = pl_w->catch_w ;
		pos = em_w->pos ;
		catch_pos_set( pl_w, em_w ) ;
		em_w->spd.x = ( em_w->pos.x - pos.x ) / 24.0f ;
		em_w->acc.x = 0.0f ;
		em_w->acc.y = -DOT_Y( 0.5f ) ;
		em_w->spd.y = ( em_w->pos.y - pos.y ) / 24.0f - em_w->acc.y * 24.0f / 2.0f ;
		em_w->pos = pos ;
		pls15_00_set( pl_w, 2 ) ;
	} else pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void okite_catch_fly( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->catch_w->pos.x += pl_w->catch_w->spd.x ;
	pl_w->catch_w->spd.x += pl_w->catch_w->acc.x ;
	pl_w->catch_w->pos.y += pl_w->catch_w->spd.y ;
	pl_w->catch_w->spd.y += pl_w->catch_w->acc.y ;
	if( -- pl_w->timer0 <= 0 ){
		pl_w->routine_3 ++ ;
		scr_excep_set( pl_w, 5, 5 ) ;
	}
}

/*--------------------------------------------------------------*/
static void okite_catch_hit( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	em_w = pl_w->catch_w ;
	if( LO_BYTE( pl_w->cg_rival ) ){
		catch_pos_set( pl_w, em_w ) ;
		em_w->disp_flag = 1 ;
	} else em_w->disp_flag = 0 ;
	if( pl_w->cg_flag1 && pl_w->cg_flag1 < 0x80 ){
		pl_w->cg_flag1 = 0 ;
		pl_w->catch_w->dm_hit_w = pl_w ;
		pl_w->catch_w->atck_no = pl15_at_okite ;
		catch_damage( pl_w ) ;
	} else if( pl_w->cg_type ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_3 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		em_w = pl_w->catch_w ;
		em_w->catch_type = 1 ;
		em_w->atck_no = pl15_at_okite_end ;
		em_w->disp_flag = 1 ;
		scr_excep_clr() ;
	}
}

/*--------------------------------------------------------------*/
static void okite_catch_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void midori_init( PLAYER_WORK *pl_w );
static void midori_move( PLAYER_WORK *pl_w );
static void midori_end( PLAYER_WORK *pl_w );
static void midori_start( PLAYER_WORK *pl_w );
static void midori_rise( PLAYER_WORK *pl_w );
static void midori_fall( PLAYER_WORK *pl_w );
static void midori_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A leap that comes down on the opponent; mashing		*/
/*			makes it last longer								*/
/*==============================================================*/
static void sp_midori( PLAYER_WORK *pl_w )
{
	static void ( *midori_jmp[])( PLAYER_WORK *pl_w ) = {
		midori_init, midori_move, midori_end,
	} ;

	midori_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void midori_init( PLAYER_WORK *pl_w )
{
	PL15_LOCAL_W *lw = ( PL15_LOCAL_W * )pl_w->tlocal ;

	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->atck_no = pl_w->atck_level + 0x32 ;
	HIT_RESET( pl_w ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	lw->renda_cnt = 0 ;
	pl_char_set( pl_w, Char_Special, 10 ) ;
}

/*--------------------------------------------------------------*/
static void midori_move( PLAYER_WORK *pl_w )
{
	static void ( *midori_move_jmp[])( PLAYER_WORK *pl_w ) = {
		midori_start, midori_rise, midori_fall, midori_land,
	} ;

	midori_move_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
/*			Counts presses of the move's button, up to 5		*/
/*==============================================================*/
static void midori_renda( PLAYER_WORK *pl_w )
{
	PL15_LOCAL_W *lw = ( PL15_LOCAL_W * )pl_w->tlocal ;

	if( lw->renda_cnt >= 0 ){
		if( pl_w->pl_now_lvbt & KEY_PUNCH ){
			if( pl_w->atck_level ){
				if( pl_w->pl_now_lvbt & KEY_HP ) lw->renda_cnt ++ ;
			} else {
				if( pl_w->pl_now_lvbt & KEY_LP ) lw->renda_cnt ++ ;
			}
			if( lw->renda_cnt >= 5 ) lw->renda_cnt = 5 ;
		}
	}
}

/*==============================================================*/
/*			Jumps toward a point in front, with two effects		*/
/*==============================================================*/
static void midori_start( PLAYER_WORK *pl_w )
{
	PL15_LOCAL_W *lw = ( PL15_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *ef ;
	VEC3 ofs ;
	Float dx ;

	midori_renda( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ) return ;
	pl_w->routine_3 ++ ;
	pl_w->atck_no = pl_w->atck_level + 0x32 ;
	HIT_RESET( pl_w ) ;
	lw->midori_long_flag = 0 ;
	pl_w->push_ng_timer = 3 ;
	lw->midori_land_flag = 0 ;
	pl_w->pl_stance = STANCE_AIR ;
	if( pl_w->atck_level == 0 ){
		pl_char_set( pl_w, Char_Special, 12 ) ;
		pl_w->timer0 = 32 ;
		pl_w->timer1 = 27 ;
		pl_w->spd.y = DOT_Y( 8.0f ) ;
		ofs = pl_w->pos ;
		dx = DOT_X( 160.0f ) ;
		if( !pl_w->disp_dir ) dx = -dx ;
		ofs.x += dx ;
		pl_w->spd.z = ofs.x ;
	} else {
		pl_char_set( pl_w, Char_Special, 15 ) ;
		pl_w->timer0 = 24 ;
		pl_w->timer1 = 19 ;
		pl_w->spd.y = DOT_Y( 6.0f ) ;
		ofs = pl_w->pos ;
		dx = 400.0f ;
		if( !pl_w->disp_dir ) dx = -dx ;
		ofs.x += dx ;
		pl_w->spd.z = ofs.x ;
	}
	if( ef = pls15_00_set( pl_w, 0 ) ) ef->pos = pl_w->pos ;
	if( ef = pls15_00_set( pl_w, 1 ) ) ef->pos = ofs ;
	pl_w->pos.y += DOT_Y( 80.0f ) ;
	dx = 160.0f ;
	if( !pl_w->disp_dir ) dx = -dx ;
	pl_w->pos.x += dx ;
	ofs.y += 90.0f ;
	dx = -DOT_X( 16.0f ) ;
	if( !pl_w->disp_dir ) dx = -dx ;
	ofs.x += dx ;
	pl_w->spd.x = ( ofs.x - pl_w->pos.x ) / 32.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = ( ( ofs.y - pl_w->pos.y ) / 32.0f * 2.0f - pl_w->spd.y * 2.0f ) / 32.0f ;
}

/*--------------------------------------------------------------*/
static void midori_rise( PLAYER_WORK *pl_w )
{
	PL15_LOCAL_W *lw = ( PL15_LOCAL_W * )pl_w->tlocal ;

	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_w->push_ng_timer = 3 ;
	pl_w->timer0 -- ;
	if( -- pl_w->timer1 < 0 ){
		pl_w->routine_3 = 3 ;
		return ;
	}
	midori_renda( pl_w ) ;
	if( lw->renda_cnt >= 2 && !lw->midori_long_flag ){
		lw->midori_long_flag = 1 ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 18 ) ;
	} else if( pl_w->cg_flag1 ){
		pl_w->atck_no = pl_w->atck_level + 0x32 ;
		pl_w->hit_flag = pl_w->atck_attr = 0 ;
		pl_w->unk_1c4 = 0 ;
		vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
		pl_w->cg_flag1 = 0 ;
	}
	if( pl_w->spd.y < 0.0f ){
		pl_w->routine_3 = 2 ;
		pl_char_set( pl_w, Char_Special, 2 ) ;
		pl_w->atck_no = pl_w->atck_level + 0x42 ;
		pl_w->hit_flag = pl_w->atck_attr = 0 ;
		pl_w->unk_1c4 = 0 ;
		vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	}
}

/*--------------------------------------------------------------*/
static void midori_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_w->timer0 -- ;
	if( -- pl_w->timer1 < 0 ) pl_w->routine_3 ++ ;
}

/*--------------------------------------------------------------*/
static void midori_land( PLAYER_WORK *pl_w )
{
	PL15_LOCAL_W *lw ;

	pl_w->hit_ng_timer = 3 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( -- pl_w->timer0 < 0 ){
		pl_w->routine_2 ++ ;
		lw = ( PL15_LOCAL_W * )pl_w->tlocal ;
		pl_w->rot[2] = 0 ;
		pl_w->routine_3 = 0 ;
		lw->midori_land_flag = 1 ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->rot[2] = 0 ;
		pl_w->scale.x = 1.0f ;
		pl_w->scale.y = 1.0f ;
		pl_char_set2( pl_w, Char_Special, 11, 5 ) ;
		pl_land_ef_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void midori_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void chouhatsu_init( PLAYER_WORK *pl_w );
static void chouhatsu_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w )
{
	static void ( *chouhatsu_jmp[])( PLAYER_WORK *pl_w ) = {
		chouhatsu_init, chouhatsu_move,
	} ;

	chouhatsu_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void chouhatsu_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_char_set( pl_w, Char_Extra, 2 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
}

/*--------------------------------------------------------------*/
static void chouhatsu_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
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
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = pl15_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 17 ) ;
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
		ofs.x = -80.0f ;
		ofs.y = DOT_Y( 64.0f ) ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
}

static PLAYER_WORK *nml_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *crouch_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *jump_catch_ck( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Can a throw start?									*/
/****************************************************************/
PLAYER_WORK *pl15_catch_ck( PLAYER_WORK *pl_w )
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
	return 0 ;
}

/*==============================================================*/
static PLAYER_WORK *crouch_catch_ck( PLAYER_WORK *pl_w )
{
	return 0 ;
}

/*==============================================================*/
/*			The air throw is the same throw						*/
/*==============================================================*/
static PLAYER_WORK *jump_catch_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( !( pl_w->dir = ( pl_w->atck_lever & ( KEY_FORWARD | KEY_BACK ) ) >> 10 ) ) return 0 ;
	if( pl_w->atck_pk == 0 && pl_w->atck_level == 1 && pl_w->pos.y > DOT_Y( 64.0f ) ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 0 ;
			return em ;
		}
		return 0 ;
	}
	return 0 ;
}

static void nml_p_set( PLAYER_WORK *pl_w );
static void jump_p_set( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Start the throw that pl15_catch_ck chose			*/
/****************************************************************/
void	pl15_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_set, jump_p_set,
	} ;

	catch_set_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->hit_stop = 10 ;
	if( !( ( pl_w->dir << 10 ) & 0x800 ) ){
		pl_w->disp_dir ^= 1 ;
		pl_w->pl_dir ^= 1 ;
	}
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 0 ) ;
	ofs.x = -DOT_X( 50.0f ) ;
	ofs.y = DOT_Y( 74.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
}

/*==============================================================*/
static void jump_p_set( PLAYER_WORK *pl_w )
{
	pl_w->hit_stop = 10 ;
	if( !( ( pl_w->dir << 10 ) & 0x800 ) ){
		pl_w->disp_dir ^= 1 ;
		pl_w->pl_dir ^= 1 ;
	}
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 0 ) ;
}

static void nml_p_catch( PLAYER_WORK *pl_w );
static void jump_p_catch( PLAYER_WORK *pl_w );
static void nml_p_catch_move( PLAYER_WORK *pl_w );
static void nml_p_catch_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void	pl15_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_catch, jump_p_catch,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_catch( PLAYER_WORK *pl_w )
{
	static void ( *nml_p_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_catch_move, nml_p_catch_end,
	} ;

	nml_p_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_catch_move( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w ;
	VEC3 ofs ;

	if( pl_w->pl_stance == STANCE_AIR ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->pos.y < pl_w->ground_y ){
			pl_land_ef_set( pl_w ) ;
			pl_w->pos.y = pl_w->ground_y ;
			pl_w->pl_stance = STANCE_STAND ;
		}
	}
	pl_char_move( pl_w ) ;
	if( pl_w->cg_prm ){
		pl_w->cg_prm = 0 ;
		pl_w->cg_type = 0 ;
		pl_w->catch_w->dm_hit_w = pl_w ;
		pl_w->catch_w->atck_no = pl15_at_throw_p ;
		catch_damage( pl_w ) ;
		ofs.x = -DOT_X( 128.0f ) ;
		ofs.y = DOT_Y( 80.0f ) ;
		ofs.z = 0.0f ;
		hit_mark_set( pl_w, &ofs, 0 ) ;
		hit_se_req( pl_w->catch_w, 1 ) ;
		pl_se_req( pl_w, 11 ) ;
	} else if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		em_w = pl_w->catch_w ;
		em_w->dm_hit_w = pl_w ;
		em_w->catch_type = 1 ;
		em_w->atck_no = pl15_at_throw_p_2 ;
	}
}

/*--------------------------------------------------------------*/
static void nml_p_catch_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		if( pl_w->pl_stance == STANCE_AIR ) pl_jump_ret( pl_w ) ;
		else pl_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
static void jump_p_catch( PLAYER_WORK *pl_w )
{
}

static void hold( PLAYER_WORK *pl_w );
static void jump_hold( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Holding the opponent								*/
/****************************************************************/
void	pl15_hold( PLAYER_WORK *pl_w )
{
	static void ( *hold_jmp[])( PLAYER_WORK *pl_w ) = {
		hold, jump_hold,
	} ;

	hold_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void hold( PLAYER_WORK *pl_w )
{
	catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/*==============================================================*/
static void jump_hold( PLAYER_WORK *pl_w )
{
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void	pl15_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	pl_w->pl_sp_type = pl15_sp_taiyou ;
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl15_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	pl_w->pl_sp_type = pl15_sp_taiyou ;
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl15_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
	case 1:
		pl_w->pl_sp_type = pl15_sp_bite ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl15_sp_kaze ;
		break ;
	}
	pl_w->atck_level = 1 ;
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void	pl15_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
	case 2:
		pl_w->pl_sp_type = pl15_sp_midori ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl15_sp_kaze ;
		break ;
	}
	pl_w->atck_level = 1 ;
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
/*			Clear the work, all but shot_cnt					*/
/****************************************************************/
void pl15_tlocal_init( PLAYER_WORK *pl_w )
{
	PL15_LOCAL_W *lw = ( PL15_LOCAL_W * )pl_w->tlocal ;
	Sint8 keep = lw->shot_cnt ;

	memset( pl_w->tlocal, 0, sizeof( pl_w->tlocal ) ) ;
	lw->shot_cnt = keep ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl15.c" End of File	*/

/* Chun-Li (character 1b): command checks, normal attacks, appearance, win and lose poses, special
   moves and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl1b_def.h"

/* speeds for a jump */
typedef struct JUMP_SPD {
	Float	spd_x, acc_x, spd_y, acc_y;
} JUMP_SPD;

void pl1b_main( PLAYER_WORK *pl_w );
void pl1b_com_ck( PLAYER_WORK *pl_w );
void pl1b_atck_set( PLAYER_WORK *pl_w );
void pl1b_atck( PLAYER_WORK *pl_w );
void pl1b_dash( PLAYER_WORK *pl_w );
void pl1b_noki( PLAYER_WORK *pl_w );
void pl1b_appear( PLAYER_WORK *pl_w );
void pl1b_win( PLAYER_WORK *pl_w );
void pl1b_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl1b_catch_ck( PLAYER_WORK *pl_w );
void pl1b_catch_set( PLAYER_WORK *pl_w );
void pl1b_catch( PLAYER_WORK *pl_w );
void pl1b_hold( PLAYER_WORK *pl_w );
int pl1b_delay_ck( PLAYER_WORK *pl_w );
void pl1b_t_kick( PLAYER_WORK *pl_w );
void pl1b_upper( PLAYER_WORK *pl_w );
void pl1b_sp_unison( PLAYER_WORK *pl_w );
void pl1b_sp_gattai( PLAYER_WORK *pl_w );
void pl1b_sp_assist( PLAYER_WORK *pl_w );
void pl1b_sp_zero( PLAYER_WORK *pl_w );

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
static Uint16 com_kikou[] = { 5, COM_GROUND, pl1b_sp_kikou, 0, KEY_BACK, KEY_DOWN | KEY_BACK, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ; /* Kikou Ken */
static Uint16 com_tenshou[] = { 3, COM_GROUND, pl1b_sp_tenshou, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_KICK } ; /* Tenshoukyaku */
static Uint16 com_senen[] = { 5, COM_GROUND, pl1b_sp_senen, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ; /* Sen'en Shuu */
static Uint16 com_kikoushou[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl1b_sp_kikoushou, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ; /* Kikoushou */
static Uint16 com_senretsu[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl1b_sp_senretsu, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ; /* Senretsukyaku */
static Uint16 com_hazan[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl1b_sp_hazan, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_KICK } ; /* Hazan Tenshoukyaku */
static Uint16 com_shichisei[] = { 3, COM_AIR | COM_HYPER | COM_ALL_SW, pl1b_sp_shichisei, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ; /* Shichisei Ranka */
static Uint16 com_hyakuretsu[] = { 4, 10, pl1b_sp_hyakuretsu, 0, KEY_KICK } ; /* Hyakuretsukyaku */

static void ( *pl1b_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl1b_main,
	pl1b_com_ck,
	pl1b_atck_set,
	pl1b_atck,
	pl1b_dash,
	pl1b_noki,
	pl1b_appear,
	pl1b_win,
	pl1b_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl1b_catch_ck,	/* returns a value */
	pl1b_catch_set,
	pl1b_catch,
	pl1b_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl1b_delay_ck,
	pl1b_t_kick,
	pl1b_upper,
	pl1b_sp_unison,
	pl1b_sp_gattai,
	pl1b_sp_assist,
	pl1b_sp_zero,
} ;

static Uint8 kikou_ck( PLAYER_WORK *pl_w );
static Uint8 tenshou_ck( PLAYER_WORK *pl_w );
static Uint8 senen_ck( PLAYER_WORK *pl_w );
static Uint8 kikoushou_ck( PLAYER_WORK *pl_w );
static Uint8 senretsu_ck( PLAYER_WORK *pl_w );
static Uint8 hazan_ck( PLAYER_WORK *pl_w );
static Uint8 hyakuretsu_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static int chouhatsu_ck( PLAYER_WORK *pl_w );
static int hazan_delay_ck( PLAYER_WORK *pl_w );
static int senretsu_delay_ck( PLAYER_WORK *pl_w );
static int kikoushou_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl1b_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl1b_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl1b_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( hyakuretsu_ck( pl_w ) ) return ;
	if( hazan_ck( pl_w ) ) return ;
	if( senretsu_ck( pl_w ) ) return ;
	if( kikoushou_ck( pl_w ) ) return ;
	if( kikou_ck( pl_w ) ) return ;
	if( tenshou_ck( pl_w ) ) return ;
	if( senen_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	if( pl_air_dash_ck( pl_w, &pl_w->com_w12 ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
static Uint8 kikou_ck( PLAYER_WORK *pl_w )
{
	PL1B_LOCAL_W *lw = ( PL1B_LOCAL_W * )pl_w->tlocal ;

	if( !com_nml_ck( pl_w, com_kikou, &pl_w->com_w0 ) ) return 0 ;
	if( lw->shot_flag ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1b_sp_kikou ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 tenshou_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_tenshou, &pl_w->com_w1 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1b_sp_tenshou ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 senen_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_senen, &pl_w->com_w2 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1b_sp_senen ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 kikoushou_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_kikoushou, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w3 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1b_sp_kikoushou ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 senretsu_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_senretsu, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w4 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1b_sp_senretsu ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 hazan_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_hazan, &pl_w->com_w5 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w5 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1b_sp_hazan ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
/*			Once a jump in the air								*/
/*==============================================================*/
static Uint8 hyakuretsu_ck( PLAYER_WORK *pl_w )
{
	if( !com_renda_ck( pl_w, com_hyakuretsu, &pl_w->com_w7 ) ) return 0 ;
	if( !sp_ok_ck( pl_w ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w7 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1b_sp_hyakuretsu ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int snapback_ck( PLAYER_WORK *pl_w )
{
	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl1b_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static int chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 8 ) ) return 0 ;
	pl_w->pl_sp_type = pl1b_sp_chouhatsu ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl1b_delay_ck( PLAYER_WORK *pl_w )
{
	if( hazan_delay_ck( pl_w ) || senretsu_delay_ck( pl_w ) || kikoushou_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static int hazan_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_hazan, &pl_w->com_w5 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 5 ;
	return 1 ;
}

/*==============================================================*/
static int senretsu_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_senretsu, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 4 ;
	return 1 ;
}

/*==============================================================*/
static int kikoushou_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_kikoushou, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 3 ;
	return 1 ;
}

/****************************************************************/
void pl1b_main( PLAYER_WORK *pl_w )
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
void pl1b_atck_set( PLAYER_WORK *pl_w )
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
/*			Forward and the heavy punch starts a move of its	*/
/*			own													*/
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
		pl_w->chain_tbl = hp_tbl ;
		pl_w->nml_level = 2 ;
		if( pl_w->atck_lever & KEY_FORWARD ){
			pl_w->routine_2 = 1 ;
			pl_w->atck_no = pl1b_at_fwd_hp ;
			HIT_RESET( pl_w ) ;
			pl_char_set( pl_w, Char_Special, 21 ) ;
			shl1b_00_set( pl_w, 5, 0 ) ;
			return ;
		}
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_STAND_HP ;
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
		pl_se_req( pl_w, SE_SWING_H ) ;
		pl_w->chain_tbl = hp_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Crouch_P, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
/*			The heavy kick steps forward						*/
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
		pl_w->pos.x += pl_w->pl_dir ? DOT_X( 16.0f ) : -DOT_X( 16.0f ) ;
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_STAND_HK ;
		pl_se_req( pl_w, SE_SWING_H ) ;
		pl_w->chain_tbl = hk_tbl ;
		pl_w->nml_level = 2 ;
		shl1b_00_set( pl_w, 4, 0 ) ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Stand_K, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
/*			Back and the heavy kick is a flip					*/
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
		pl_w->chain_tbl = hk_tbl ;
		pl_w->nml_level = 2 ;
		if( !pl_w->cpu_flag && ( pl_w->atck_lever & KEY_BACK ) ){
			pl_w->routine_2 ++ ;
			pl_w->atck_no = pl1b_at_downback_hk ;
			HIT_RESET( pl_w ) ;
			pl_char_set( pl_w, Char_Crouch_K, 3 ) ;
			return ;
		}
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_CROUCH_HK ;
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
/*			Down and the heavy kick is a stomp					*/
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
		pl_w->atck_no = pl1b_at_jump_hk ;
		if( pl_w->atck_lever & KEY_DOWN ){
			pl_w->routine_2 ++ ;
			pl_w->char_no.b.no = 6 ;
			pl_w->atck_no = pl1b_at_jdown_hk ;
		} else if( pl_w->atck_lever & KEY_FORWARD ){
			pl_w->char_no.b.no = 5 ;
			pl_w->atck_no = pl1b_at_jfwd_hk ;
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
static void lever_atck0( PLAYER_WORK *pl_w );
static void lever_atck0_start( PLAYER_WORK *pl_w );
static void lever_atck0_hold( PLAYER_WORK *pl_w );
static void lever_atck0_set( PLAYER_WORK *pl_w );
static void lever_atck0_end( PLAYER_WORK *pl_w );
static void crouch_punch_move( PLAYER_WORK *pl_w );
static void stand_kick_move( PLAYER_WORK *pl_w );
static void crouch_kick_move( PLAYER_WORK *pl_w );
static void lever_atck1( PLAYER_WORK *pl_w );
static void lever_atck1_start( PLAYER_WORK *pl_w );
static void lever_atck1_jump( PLAYER_WORK *pl_w );
static void lever_atck1_land( PLAYER_WORK *pl_w );
static void sjump_move( PLAYER_WORK *pl_w );
static void jump_move( PLAYER_WORK *pl_w );
static void jump_punch_move( PLAYER_WORK *pl_w );
static void jump_kick_move( PLAYER_WORK *pl_w );
static void lever_atck2( PLAYER_WORK *pl_w );
static void lever_atck2_fall( PLAYER_WORK *pl_w );
static void lever_atck2_bound( PLAYER_WORK *pl_w );

/****************************************************************/
void pl1b_atck( PLAYER_WORK *pl_w )
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
		if( !pl_w->pl_stance ) stand_kick_move( pl_w ) ;
		else crouch_kick_move( pl_w ) ;
	} else {
		if( pl_w->pl_stance == STANCE_CROUCH ) crouch_punch_move( pl_w ) ;
		else stand_punch_move( pl_w ) ;
	}
}

/*==============================================================*/
static void stand_punch_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 2:
		if( pl_w->routine_2 ){
			lever_atck0( pl_w ) ;
			break ;
		}
	case 0:
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
static void lever_atck0( PLAYER_WORK *pl_w )
{
	static void ( *lever_atck0_jmp[])( PLAYER_WORK *pl_w ) = {
		lever_atck0_start, lever_atck0_hold, lever_atck0_set, lever_atck0_end,
	} ;

	lever_atck0_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void lever_atck0_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		pl_w->timer0 = 240 ;
		shl1b_00_set( pl_w, 6, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void lever_atck0_hold( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( --pl_w->timer0 < 0 ) pl_w->routine_3 ++ ;
}

/*--------------------------------------------------------------*/
static void lever_atck0_set( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_char_set( pl_w, Char_Special, 24 ) ;
}

/*--------------------------------------------------------------*/
static void lever_atck0_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
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
	case 2:
		if( pl_w->routine_2 ){
			lever_atck1( pl_w ) ;
			break ;
		}
	case 0:
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
static void lever_atck1( PLAYER_WORK *pl_w )
{
	static void ( *lever_atck1_jmp[])( PLAYER_WORK *pl_w ) = {
		lever_atck1_start, lever_atck1_jump, lever_atck1_land,
	} ;

	lever_atck1_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void lever_atck1_start( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->spd.x = pl_w->pl_dir ? DOT_X( 2.0f ) : -DOT_X( 2.0f ) ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 12.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.5f ) ;
	pl_w->timer1 = 0 ;
}

/*--------------------------------------------------------------*/
static void lever_atck1_jump( PLAYER_WORK *pl_w )
{
	if( pl_w->hit_flag && !pl_w->timer1 ) pl_w->timer1 = 1 ;
	pl_char_move( pl_w ) ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_3 ++ ;
		pl_w->sjump_flag = 0 ;
		pl_w->air_guard_cnt = 0 ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Crouch_K, 4 ) ;
	}
}

/*--------------------------------------------------------------*/
static void lever_atck1_land( PLAYER_WORK *pl_w )
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
/*			The heavy kick can hit twice						*/
/*==============================================================*/
static void jump_kick_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 2:
		if( pl_w->routine_2 ){
			lever_atck2( pl_w ) ;
			break ;
		}
		if( pl_char_move( pl_w ) < 0 ){
			pl_jump_ret( pl_w ) ;
			break ;
		}
		if( pl_w->cg_flag1 ){
			pl_w->atck_no = pl_w->cg_flag1 ;
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

/*==============================================================*/
static void lever_atck2( PLAYER_WORK *pl_w )
{
	static void ( *lever_atck2_jmp[])( PLAYER_WORK *pl_w ) = {
		lever_atck2_fall, lever_atck2_bound,
	} ;

	lever_atck2_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
/*			Bounce off what it hits, away from the stage's		*/
/*			ends												*/
/*==============================================================*/
static void lever_atck2_fall( PLAYER_WORK *pl_w )
{
	if( !pl_w->hit_flag ){
		if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
		return ;
	}
	pl_w->routine_3 ++ ;
	pl_w->sjump_flag = 0 ;
	{
		Float *cx = &scr_work.eye[0].x ;

		if( pl_w->pos.x < *cx + -160.0f ){
			if( pl_w->pl_dir ) pl_w->jump_dir = 0 ;
			else pl_w->jump_dir = 1 ;
		} else if( pl_w->pos.x > *cx + 160.0f ){
			if( pl_w->pl_dir ) pl_w->jump_dir = 1 ;
			else pl_w->jump_dir = 0 ;
		}
	}
	pl_jump_spd_set( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void lever_atck2_bound( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->jump_atk_cnt = JATK_ONE ;
		pl_w->jump_com_cnt = 0 ;
		act_st_set( pl_w, N_JUMP_AIR ) ;
		switch( pl_w->jump_dir ){
		case 0:
			pl_char_set( pl_w, Char_Jump, 14 ) ;
			break ;
		case 1:
			pl_char_set( pl_w, Char_Jump, 15 ) ;
			break ;
		case -1:
			pl_char_set( pl_w, Char_Jump, 13 ) ;
			break ;
		}
	}
}

/****************************************************************/
/*			The launcher										*/
/****************************************************************/
void pl1b_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_char_set( pl_w, Char_Extra, 13 ) ;
		pl_w->atck_no = pl1b_at_upper ;
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
void pl1b_dash( PLAYER_WORK *pl_w )
{
	static void ( *dash_jmp[])( PLAYER_WORK *pl_w ) = {
		dash_init, dash_move, dash_stop, dash_end,
	} ;

	dash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dash_init( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 16 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 9.5f ) : -DOT_X( 9.5f ) ;
		pl_w->acc.x = pl_w->pl_dir ? -0.3125f : 0.3125f ;
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
	if( --pl_w->timer0 == 0 ) pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void dash_stop( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ){
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
void pl1b_noki( PLAYER_WORK *pl_w )
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
		pl_w->spd.x = pl_w->pl_dir ? -10.0f : 10.0f ;
	}
}

/*--------------------------------------------------------------*/
static void noki_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) >= 0 ){
		if( !pl_w->cg_type ) pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	} else {
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
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
static void appear_move( PLAYER_WORK *pl_w );
static void win_time_ko( PLAYER_WORK *pl_w );
static void win_ko_init( PLAYER_WORK *pl_w );
static void win_ko_move( PLAYER_WORK *pl_w );
static void win_time( PLAYER_WORK *pl_w );
static void lose_time_ko( PLAYER_WORK *pl_w );

/****************************************************************/
void pl1b_appear( PLAYER_WORK *pl_w )
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
/*			KO, lose, time over win, time over loss, draw		*/
/****************************************************************/
void pl1b_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_time_ko,
		lose_time_ko,
		win_time,
		lose_time_ko,
		lose_time_ko,
	} ;

	if( pl_win_end_ck( pl_w ) ){
		pl_stand_ret( pl_w ) ;
	}else{
		win_jmp[ pl_w->type ]( pl_w ) ;
	}
}

/*==============================================================*/
static void win_time_ko( PLAYER_WORK *pl_w )
{
	static void ( *win_ko_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko_init,
		win_ko_move,
	} ;

	win_ko_jmp[ pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			One of four poses									*/
/*==============================================================*/
static void win_ko_init( PLAYER_WORK *pl_w )
{
	pl_w -> routine_2 ++ ;
	pl_w -> timer0 = Rnd() & 0x3 ;

	switch( pl_w -> timer0 ){
	case 0 :
		pl_char_set( pl_w, Char_Win, CH1B_WIN_KO0 ) ;
		break ;
	case 1 :
		pl_char_set( pl_w, Char_Win, CH1B_WIN_KO1 ) ;
		break ;
	case 2 :
		pl_char_set( pl_w, Char_Win, CH1B_WIN_KO2 ) ;
		break ;
	case 3 :
		pl_char_set( pl_w, Char_Win, CH1B_WIN_KO3 ) ;
		break ;
	}
}

/*--------------------------------------------------------------*/
static void win_ko_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void win_time( PLAYER_WORK *pl_w )
{
	if( !pl_w -> routine_2 ){
		pl_w -> routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, CH1B_WIN_TIME ) ;
	}else{
		pl_char_move( pl_w ) ;
	}
}

/*==============================================================*/
static void lose_time_ko( PLAYER_WORK *pl_w )
{
	if( !pl_w -> routine_2 ){
		pl_w -> routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, CH1B_LOSE ) ;
	}else{
		pl_char_move( pl_w ) ;
	}
}

static void sp_kikou( PLAYER_WORK *pl_w );
static void sp_tenshou( PLAYER_WORK *pl_w );
static void sp_senen( PLAYER_WORK *pl_w );
static void sp_kikoushou( PLAYER_WORK *pl_w );
static void sp_senretsu( PLAYER_WORK *pl_w );
static void sp_hazan( PLAYER_WORK *pl_w );
static void sp_hyakuretsu( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_shichisei( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );

/****************************************************************/
void	pl1b_sp_atck( PLAYER_WORK *pl_w )
{
	static void (*pl_sp_tbl[])( PLAYER_WORK *pl_w ) ={
		sp_kikou,				/* 00 Kikou Ken */
		sp_tenshou,				/* 01 Tenshoukyaku */
		sp_senen,					/* 02 Sen'en Shuu */
		sp_kikoushou,					/* 03 Kikoushou (hyper) */
		sp_senretsu,					/* 04 Senretsukyaku (hyper) */
		sp_hazan,					/* 05 Hazan Tenshoukyaku (hyper) */
		sp_hyakuretsu,			/* 06 Hyakuretsukyaku */
		sp_kikou,				/* 07 */
		sp_chouhatsu,					/* 08 taunt */
		sp_kikou,				/* 09 */
		sp_kikou,				/* 0a */
		sp_shichisei,					/* 0b Shichisei Ranka (air hyper) */
		sp_kikou,				/* 0c */
		sp_kikou,				/* 0d */
		sp_kikou,				/* 0e */
		sp_snapback,					/* 0f Snap Back */
	} ;

	pl_sp_tbl[ pl_w->pl_sp_type ]( pl_w ) ;
}

static void kikou_init( PLAYER_WORK *pl_w );
static void kikou_move( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Kikouken											*/
/*==============================================================*/
static void sp_kikou( PLAYER_WORK *pl_w )
{
	static void ( *kikou_jmp[])( PLAYER_WORK *pl_w ) = {
		kikou_init, kikou_move,
	} ;

	kikou_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void kikou_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl1b_at_kikou ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 0 ) ;
}

/*==============================================================*/
/*			The shot leaves on the animation's flag				*/
/*==============================================================*/
static void kikou_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( HI_BYTE( pl_w->cg_rival ) & 1 ){
		HI_BYTE( pl_w->cg_rival ) = 0 ;
		shl1b_00_set( pl_w, 0, 0 ) ;
	}
}

static void tenshou_init( PLAYER_WORK *pl_w );
static void tenshou_start( PLAYER_WORK *pl_w );
static void tenshou_air( PLAYER_WORK *pl_w );
static void tenshou_land( PLAYER_WORK *pl_w );
static void tenshou_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_tenshou( PLAYER_WORK *pl_w )
{
	static void ( *tenshou_jmp[])( PLAYER_WORK *pl_w ) = {
		tenshou_init, tenshou_start, tenshou_air, tenshou_land,
	} ;

	tenshou_jmp[pl_w->routine_2]( pl_w ) ;
}

/* Tenshoukyaku: its speeds for each strength, and the attack of each kick */
static JUMP_SPD tenshou_spd[2] = {
	{ 10.0f, -DOT_X( 0.375f ), DOT_Y( 10.0f ), -DOT_Y( 0.5f ) },
	{ DOT_X( 8.0f ), -DOT_X( 0.25f ), 30.0f, -DOT_Y( 0.5f ) },
} ;
static Sint32 tenshou_atck[6] = { 0x33, 0x34, 0x33, 0x34, 0x37, 0x38 } ;

/*==============================================================*/
static void tenshou_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl_w->atck_level ? 0x37 : 0x34 ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 12 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	if( Sys->voice_type ) pl_voice_req( pl_w, 20 ) ;
	else pl_voice_req( pl_w, 26 ) ;
	pl_w->spd.x = pl_w->pl_dir ? tenshou_spd[pl_w->atck_level].spd_x : -tenshou_spd[pl_w->atck_level].spd_x ;
	pl_w->acc.x = pl_w->pl_dir ? tenshou_spd[pl_w->atck_level].acc_x : -tenshou_spd[pl_w->atck_level].acc_x ;
	pl_w->spd.y = tenshou_spd[pl_w->atck_level].spd_y ;
	pl_w->acc.y = tenshou_spd[pl_w->atck_level].acc_y ;
	pl_sp_ef_set( pl_w ) ;
	shl1b_00_set( pl_w, 4, 2 ) ;
	pl_w->char_no.b.no = pl_w->atck_level ? 3 : 1 ;
	pl_char_set( pl_w, Char_Special, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
/*			Leaves the ground on the animation's flag			*/
/*==============================================================*/
static void tenshou_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( HI_BYTE( pl_w->cg_rival ) & 1 ){
		pl_w->routine_2 ++ ;
		pl_air_set( pl_w ) ;
		pl_w->pos.x += pl_w->pl_dir ? 40.0f : -40.0f ;
		tenshou_move( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void tenshou_air( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	tenshou_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void tenshou_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
/*			Rises without drifting back, a new attack for		*/
/*			each kick, until it lands							*/
/*==============================================================*/
static void tenshou_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( ( !pl_w->pl_dir && pl_w->spd.x > 0.0f ) || ( pl_w->pl_dir && pl_w->spd.x < 0.0f ) ) RATE_CLR_X( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->atck_no = tenshou_atck[( pl_w->cg_type >> 1 ) - 1] ;
		HIT_RESET( pl_w ) ;
		pl_w->cg_type = 0 ;
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
		pl_char_set( pl_w, Char_Special, 26 ) ;
	}
}

static void senen_init( PLAYER_WORK *pl_w );
static void senen_move( PLAYER_WORK *pl_w );
static void senen_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_senen( PLAYER_WORK *pl_w )
{
	static void ( *senen_jmp[])( PLAYER_WORK *pl_w ) = {
		senen_init, senen_move, senen_end,
	} ;

	senen_jmp[pl_w->routine_2]( pl_w ) ;
}

/* Sen'en Shuu: its speeds for each strength */
static Float senen_spd[2][2] = {
	{ DOT_X( 2.0f ), DOT_Y( 16.0f ) },
	{ DOT_X( 8.0f ), DOT_Y( 10.0f ) },
} ;

/*==============================================================*/
static void senen_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl_w->atck_level ? 0x3b : 0x39 ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 12 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->timer1 = 0 ;
	pl_w->spd.x = pl_w->pl_dir ? senen_spd[pl_w->atck_level][0] : -senen_spd[pl_w->atck_level][0] ;
	pl_w->spd.y = senen_spd[pl_w->atck_level][1] ;
	pl_w->acc.y = -2.142857f ;
	pl_sp_ef_set( pl_w ) ;
	shl1b_00_set( pl_w, 4, 1 ) ;
	pl_w->char_no.b.no = pl_w->atck_level ? 6 : 4 ;
	pl_char_set( pl_w, Char_Special, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
/*			Goes up while the animation's flag is positive,		*/
/*			then flies until it lands							*/
/*==============================================================*/
static void senen_move( PLAYER_WORK *pl_w )
{
	if( pl_w->hit_flag && !pl_w->timer1 ) pl_w->timer1 = 1 ;
	if( !pl_w->cg_flag1 ) pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ) return ;
	else if( pl_w->cg_type >= 0 ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		return ;
	}
	pl_air_set( pl_w ) ;
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
		pl_se_req( pl_w, 52 ) ;
	}
}

/*--------------------------------------------------------------*/
static void senen_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void kikoushou_init( PLAYER_WORK *pl_w, PL1B_LOCAL_W *lw );
static void kikoushou_start( PLAYER_WORK *pl_w, PL1B_LOCAL_W *lw );
static void kikoushou_shot( PLAYER_WORK *pl_w, PL1B_LOCAL_W *lw );
static void kikoushou_hold( PLAYER_WORK *pl_w, PL1B_LOCAL_W *lw );
static void kikoushou_end( PLAYER_WORK *pl_w, PL1B_LOCAL_W *lw );

/*==============================================================*/
/*			Kikoushou: a hyper combo, a shot held while the		*/
/*			kicks are pressed									*/
/*==============================================================*/
static void sp_kikoushou( PLAYER_WORK *pl_w )
{
	static void ( *kikoushou_jmp[])( PLAYER_WORK *pl_w, PL1B_LOCAL_W *lw ) = {
		kikoushou_init, kikoushou_start, kikoushou_shot, kikoushou_hold, kikoushou_end,
	} ;

	kikoushou_jmp[pl_w->routine_2]( pl_w, ( PL1B_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void kikoushou_init( PLAYER_WORK *pl_w, PL1B_LOCAL_W *lw )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl1b_at_kikoushou ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	shl1b_00_set( pl_w, 2, 0 ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 0 ) ;
}

/*--------------------------------------------------------------*/
static void kikoushou_start( PLAYER_WORK *pl_w, PL1B_LOCAL_W *lw )
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
		ofs.x = DOT_X( 41.0f ) ;
		ofs.y = DOT_Y( 71.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void kikoushou_shot( PLAYER_WORK *pl_w, PL1B_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( !( HI_BYTE( pl_w->cg_rival ) & 1 ) ) return ;
	pl_w->routine_2 ++ ;
	lw->kikoushou_shot_flag = 0 ;
	shl1b_00_set( pl_w, 11, 0 ) ;
	if( !( pl_w->ef_w = shl1b_00_set( pl_w, 1, 0 ) ) ){
		pl_w->routine_2 = 4 ;
		pl_char_set( pl_w, Char_Super, 4 ) ;
		return ;
	}
	lw->kikoushou_renda_cnt = 8 ;
	pl_char_set( pl_w, Char_Super, 3 ) ;
}

/*==============================================================*/
/*			Each other press of the buttons makes the shot		*/
/*			bigger												*/
/*==============================================================*/
static void kikoushou_hold( PLAYER_WORK *pl_w, PL1B_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( !lw->kikoushou_shot_flag ){
		pl_w->routine_2 ++ ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_char_set( pl_w, Char_Super, 4 ) ;
	}
	if( lw->kikoushou_renda_cnt && sw_renda_ck( pl_w ) ){
		if( !( --lw->kikoushou_renda_cnt & 1 ) ) pl_w->timer1 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void kikoushou_end( PLAYER_WORK *pl_w, PL1B_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void senretsu_init( PLAYER_WORK *pl_w, PL1B_LOCAL_W *lw );
static void senretsu_start( PLAYER_WORK *pl_w, PL1B_LOCAL_W *lw );
static void senretsu_kick( PLAYER_WORK *pl_w, PL1B_LOCAL_W *lw );
static void senretsu_stop( PLAYER_WORK *pl_w, PL1B_LOCAL_W *lw );
static void senretsu_end( PLAYER_WORK *pl_w, PL1B_LOCAL_W *lw );

/*==============================================================*/
/*			Senretsukyaku: a hyper combo of kicks, longer		*/
/*			while the buttons are pressed						*/
/*==============================================================*/
static void sp_senretsu( PLAYER_WORK *pl_w )
{
	static void ( *senretsu_jmp[])( PLAYER_WORK *pl_w, PL1B_LOCAL_W *lw ) = {
		senretsu_init, senretsu_start, senretsu_kick, senretsu_stop, senretsu_end,
	} ;

	senretsu_jmp[pl_w->routine_2]( pl_w, ( PL1B_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void senretsu_init( PLAYER_WORK *pl_w, PL1B_LOCAL_W *lw )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl1b_at_senretsu ;
	HIT_RESET( pl_w ) ;
	pl_w->atck_attr = ATR_DOWN_LIFT ;
	pl_dir_set( pl_w ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	if( pl_w->cpu_flag ) lw->senretsu_renda_cnt = 20 ;
	else lw->senretsu_renda_cnt = 60 ;
	pl_w->timer0 = 60 ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 5 ) ;
}

/*--------------------------------------------------------------*/
static void senretsu_start( PLAYER_WORK *pl_w, PL1B_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 8.0f ) : -DOT_X( 8.0f ) ;
		shl1b_00_set( pl_w, 4, 3 ) ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = -40.0f ;
		ofs.y = 90.0f ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void senretsu_kick( PLAYER_WORK *pl_w, PL1B_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ) pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->cg_flag1 ){
		pl_w->atck_no = pl_w->cg_flag1 ;
		HIT_RESET( pl_w ) ;
		pl_w->atck_attr = ATR_DOWN_LIFT ;
		pl_w->cg_flag1 = 0 ;
	}
	if( lw->senretsu_renda_cnt && sw_renda_ck( pl_w ) ){
		lw->senretsu_renda_cnt -- ;
		return ;
	}
	if( --pl_w->timer0 == 0 ){
		pl_w->routine_2 ++ ;
		pl_w->atck_no = pl1b_at_senretsu_end ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 7 ) ;
	}
}

/*--------------------------------------------------------------*/
static void senretsu_stop( PLAYER_WORK *pl_w, PL1B_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_char_set( pl_w, Char_Super, 8 ) ;
	}
}

/*--------------------------------------------------------------*/
static void senretsu_end( PLAYER_WORK *pl_w, PL1B_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void hazan_init( PLAYER_WORK *pl_w );
static void hazan_start( PLAYER_WORK *pl_w );
static void hazan_kick( PLAYER_WORK *pl_w );
static void hazan_air( PLAYER_WORK *pl_w );
static void hazan_end( PLAYER_WORK *pl_w );
static void hazan_hit( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Hazan Tenshoukyaku: a hyper combo that leaps up		*/
/*			kicking												*/
/*==============================================================*/
static void sp_hazan( PLAYER_WORK *pl_w )
{
	static void ( *hazan_jmp[])( PLAYER_WORK *pl_w ) = {
		hazan_init, hazan_start, hazan_kick, hazan_air, hazan_end,
	} ;

	hazan_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void hazan_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl1b_at_hazan ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.x = pl_w->pl_dir ? 10.0f : -10.0f ;
	pl_w->acc.x = pl_w->pl_dir ? -DOT_X( 0.25f ) : DOT_X( 0.25f ) ;
	pl_w->spd.y = DOT_Y( 11.0f ) ;
	pl_w->acc.y = -0.4017857f ;
	pl_sp_ef_set( pl_w ) ;
	shl1b_00_set( pl_w, 4, 2 ) ;
	pl_char_set( pl_w, Char_Super, 9 ) ;
}

/*--------------------------------------------------------------*/
static void hazan_start( PLAYER_WORK *pl_w )
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
		ofs.x = -DOT_X( 7.0f ) ;
		ofs.y = DOT_Y( 50.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void hazan_kick( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type & 1 ){
		pl_w->routine_2 ++ ;
		pl_w->pos.x += pl_w->pl_dir ? -40.0f : 40.0f ;
		pl_w->pl_stance = STANCE_AIR ;
	}
	hazan_hit( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void hazan_air( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_w->spd.y < 0.0f ) pl_w->acc.y = -DOT_Y( 0.5f ) ;
	else pl_w->timer0 = 0 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( ( !pl_w->pl_dir && pl_w->spd.x > 0.0f ) || ( pl_w->pl_dir && pl_w->spd.x < 0.0f ) ){
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
	}
	hazan_hit( pl_w ) ;
	if( pl_w->pos.y > pl_w->ground_y ) return ;
	pl_w->delay_flag = 0 ;
	pl_w->delay_timer = 0 ;
	pl_w->hyper_bg = 0 ;
	pl_w->hyper_bg_tm = 0 ;
	pl_w->routine_2 ++ ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_land_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 26 ) ;
}

/*--------------------------------------------------------------*/
static void hazan_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
/*			The animation asks for the next hit in its upper	*/
/*			bits												*/
/*==============================================================*/
static void hazan_hit( PLAYER_WORK *pl_w )
{
	static Sint32 hazan_atck[7] = { 0x47, 0x48, 0x47, 0x48, 0x4b, 0x4c, 0x63 } ;
	Sint8 hit ;

	hit = pl_w->cg_type ;
	hit &= ~1 ;
	if( !hit ) return ;
	pl_w->cg_type &= 1 ;
	pl_w->atck_no = hazan_atck[( hit >> 1 ) - 1] ;
	HIT_RESET( pl_w ) ;
}

static void hyakuretsu_init( PLAYER_WORK *pl_w, PL1B_LOCAL_W *lw );
static void hyakuretsu_kick( PLAYER_WORK *pl_w, PL1B_LOCAL_W *lw );
static void hyakuretsu_stop( PLAYER_WORK *pl_w, PL1B_LOCAL_W *lw );
static void hyakuretsu_end( PLAYER_WORK *pl_w, PL1B_LOCAL_W *lw );

/*==============================================================*/
/*			Hyakuretsukyaku, on the ground or in the air		*/
/*==============================================================*/
static void sp_hyakuretsu( PLAYER_WORK *pl_w )
{
	static void ( *hyakuretsu_jmp[])( PLAYER_WORK *pl_w, PL1B_LOCAL_W *lw ) = {
		hyakuretsu_init, hyakuretsu_kick, hyakuretsu_stop, hyakuretsu_end,
	} ;

	if( pl_w->pl_stance == STANCE_AIR ){
		pl_jump_move( pl_w ) ;
		if( pl_w->pos.y <= pl_w->ground_y ){
			pl_w->pos.y = pl_w->ground_y ;
			pl_w->pl_stance = STANCE_STAND ;
			pl_stand_ret( pl_w ) ;
			return ;
		}
	}
	hyakuretsu_jmp[pl_w->routine_2]( pl_w, ( PL1B_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void hyakuretsu_init( PLAYER_WORK *pl_w, PL1B_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl1b_at_hyakuretsu ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 7 ) ;
	pl_dir_set( pl_w ) ;
	lw->tap_timer = 15 ;
	if( pl_w->pl_stance == STANCE_AIR ){
		pl_w->spd.x /= 16.0f ;
		pl_w->spd.y /= 8.0f ;
		pl_w->acc.y /= 64.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->char_no.b.no = pl_w->atck_level ? 15 : 13 ;
	} else {
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->char_no.b.no = pl_w->atck_level ? 9 : 7 ;
		pl_sp_ef_set( pl_w ) ;
	}
	pl_char_set( pl_w, Char_Special, pl_w->char_no.b.no ) ;
	pl_w->timer0 = pl_w->cg_type ;
	pl_w->timer1 = 60 ;
	shl1b_00_set( pl_w, 4, 3 ) ;
}

/*==============================================================*/
/*			A new hit with each kick; tapping the buttons		*/
/*			keeps it going										*/
/*==============================================================*/
static void hyakuretsu_kick( PLAYER_WORK *pl_w, PL1B_LOCAL_W *lw )
{
	static Uint8 hyakuretsu_tap[2] = { 0, 6 } ;
	Uint8 level ;
	Uint8 atck ;
	Sint8 kick ;
	Sint8 save ;

	pl_char_move( pl_w ) ;
	if( pl_w->timer0 != pl_w->cg_type ){
		pl_w->timer0 = pl_w->cg_type ;
		if( pl_w->pl_stance == STANCE_AIR ) atck = pl_w->atck_level ? 0x55 : 0x53 ;
		else atck = pl_w->atck_level ? 0x4f : 0x4d ;
		pl_w->atck_no = atck ;
		HIT_RESET( pl_w ) ;
	}
	if( pl_w->cpu_flag ){
		if( pl_w->timer1 -- < 0 ) pl_w->routine_2 ++ ;
	} else if( !( pl_w->pl_sw_now & KEY_KICK ) ){
		if( -- lw->tap_timer < 0 ) pl_w->routine_2 ++ ;
	} else {
		level = ( pl_w->pl_sw_now & KEY_LK ) ? 0 : 1 ;
		if( lw->tap_timer <= hyakuretsu_tap[level] ) pl_w->routine_2 ++ ;
		else {
			lw->tap_timer = 15 ;
			if( pl_w->atck_level == level ) return ;
			pl_w->atck_level = level ;
			save = pl_w->cg_ctr ;
			kick = pl_w->cg_type ;
			if( pl_w->pl_stance == STANCE_AIR ) pl_w->char_no.b.no = pl_w->atck_level ? 15 : 13 ;
			else pl_w->char_no.b.no = pl_w->atck_level ? 9 : 7 ;
			pl_char_set( pl_w, Char_Special, pl_w->char_no.b.no ) ;
			shl1b_00_set( pl_w, 4, 3 ) ;
			while( pl_w->cg_type != kick ){
				pl_w->cg_ctr = 1 ;
				pl_char_move( pl_w ) ;
			}
			pl_w->cg_ctr = save ;
		}
	}
}

/*--------------------------------------------------------------*/
static void hyakuretsu_stop( PLAYER_WORK *pl_w, PL1B_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( HI_BYTE( pl_w->cg_rival ) & 1 ){
		pl_w->routine_2 ++ ;
		if( pl_w->pl_stance == STANCE_AIR ) pl_w->char_no.b.no = pl_w->atck_level ? 18 : 16 ;
		else pl_w->char_no.b.no = pl_w->atck_level ? 12 : 10 ;
		pl_char_set( pl_w, Char_Special, pl_w->char_no.b.no ) ;
	}
}

/*--------------------------------------------------------------*/
static void hyakuretsu_end( PLAYER_WORK *pl_w, PL1B_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		if( pl_w->pl_stance == STANCE_AIR ){
			pl_w->spd.y = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_jump_ret( pl_w ) ;
		} else {
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_stand_ret( pl_w ) ;
		}
	}
}

static void shichisei_init( PLAYER_WORK *pl_w );
static void shichisei_start( PLAYER_WORK *pl_w );
static void shichisei_dash( PLAYER_WORK *pl_w );
static void shichisei_catch( PLAYER_WORK *pl_w );
static void shichisei_throw( PLAYER_WORK *pl_w );
static void shichisei_fall( PLAYER_WORK *pl_w );
static void shichisei_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Shichisei Ranka: a dash that catches the			*/
/*			opponent and throws them							*/
/*==============================================================*/
static void sp_shichisei( PLAYER_WORK *pl_w )
{
	static void ( *shichisei_jmp[])( PLAYER_WORK *pl_w ) = {
		shichisei_init, shichisei_start, shichisei_dash, shichisei_catch, shichisei_throw, shichisei_fall, shichisei_end,
	} ;

	shichisei_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void shichisei_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl1b_at_shichisei ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_char_set( pl_w, Char_Super, 11 ) ;
}

/*--------------------------------------------------------------*/
/* the odd test of cg_type is the game's own */
static void shichisei_start( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( !( !pl_w->cg_type & 1 ) ){
		pl_w->routine_2 ++ ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->timer0 = 14 ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 14.0f ) : -DOT_X( 14.0f ) ;
		ofs.x = 10.0f ;
		ofs.y = DOT_Y( 88.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shichisei_dash( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->hit_flag ){
		if( !pl_sp_catch_ck( pl_w ) ){
			pl_w->routine_2 = 5 ;
			pl_w->sjump_flag = 0 ;
			pl_w->delay_flag = 0 ;
			pl_w->delay_timer = 0 ;
			pl_w->hyper_bg = 0 ;
			pl_w->hyper_bg_tm = 0 ;
			pl_w->air_jump_cnt = 2 ;
			pl_w->spd.x = pl_w->pl_dir ? 2.5f : -2.5f ;
			pl_w->acc.x = 0.0f ;
			pl_w->spd.y = DOT_Y( 8.0f ) ;
			pl_w->acc.y = -DOT_Y( 0.375f ) ;
			pl_char_set( pl_w, Char_Super, 13 ) ;
		} else {
			pl_w->routine_2 ++ ;
			pl_w->catch_no = 0xc2 ;
			pl_sp_catch_set( pl_w, pl_w->em_w ) ;
			shl1b_00_set( pl_w, 8, 0 ) ;
		}
	} else if( --pl_w->timer0 == 0 ){
		pl_w->jump_dir = 0 ;
		pl_jump_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void shichisei_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->catch_w ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->routine_2 ++ ;
	pl_w->timer0 = 300 ;
	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	scr_excep_set( pl_w, 1, 1 ) ;
	pl_w->pos.x = pl_w->pl_dir ? scr_work.eye[0].x + 0.0f : scr_work.eye[0].x + -DOT_X( 16.0f ) ;
	pl_w->pos.y = scr_work.center_y + -120.0f ;
	pl_char_set( pl_w, Char_Super, 14 ) ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.x = pl_w->pl_dir ? 0.104166664f : -0.104166664f ;
	pl_w->acc.x = 0.0f ;
	em->pl_dir = em->disp_dir = pl_w->pl_dir ^ 1 ;
	em->spd.y = 0.0f ;
	em->acc.y = 0.0f ;
	em->spd.x = pl_w->spd.x / 2.0f ;
	em->acc.x = 0.0f ;
	catch_pos_set( pl_w, em ) ;
}

/*--------------------------------------------------------------*/
static void shichisei_throw( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->catch_w ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	if( --pl_w->timer0 ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		em->pos.x += em->spd.x ;
		em->spd.x += em->acc.x ;
		em->pos.y += em->spd.y ;
		em->spd.y += em->acc.y ;
		return ;
	}
	pl_w->routine_2 ++ ;
	pl_w->delay_flag = 0 ;
	pl_w->delay_timer = 0 ;
	pl_w->hyper_bg = 0 ;
	pl_w->hyper_bg_tm = 0 ;
	em->dm_hit_w = pl_w ;
	em->catch_type = 1 ;
	em->atck_no = pl1b_at_shichisei_2 ;
	em->pl_dir = em->disp_dir = em->pl_dir ^ 1 ;
	SCR_EXCEP_SET ;
	pl_w->spd.x = pl_w->pl_dir ? DOT_X( 4.0f ) : -DOT_X( 4.0f ) ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_char_set( pl_w, Char_Jump, 10 ) ;
}

/*--------------------------------------------------------------*/
static void shichisei_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y > pl_w->ground_y ) return ;
	pl_w->routine_2 ++ ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_CROUCH ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_char_set( pl_w, Char_Super, 15 ) ;
}

/*--------------------------------------------------------------*/
static void shichisei_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void snapback_init( PLAYER_WORK *pl_w );
static void snapback_start( PLAYER_WORK *pl_w );
static void snapback_shot( PLAYER_WORK *pl_w );
static void snapback_hit( PLAYER_WORK *pl_w );
static void snapback_wait( PLAYER_WORK *pl_w );
static void snapback_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Snap Back											*/
/*==============================================================*/
static void sp_snapback( PLAYER_WORK *pl_w )
{
	static void ( *snapback_jmp[])( PLAYER_WORK *pl_w ) = {
		snapback_init, snapback_start, snapback_shot, snapback_hit, snapback_wait, snapback_end,
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
	pl_w->atck_no = pl1b_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 27 ) ;
}

/*--------------------------------------------------------------*/
static void snapback_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		shl1b_00_set( pl_w, 5, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void snapback_shot( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		ofs.x = DOT_X( 35.0f ) ;
		ofs.y = DOT_Y( 72.0f ) ;
		ofs.z = 0.0f ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
}

/*--------------------------------------------------------------*/
static void snapback_hit( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->timer0 = 22 ;
		shl1b_00_set( pl_w, 6, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void snapback_wait( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( --pl_w->timer0 == 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, 28 ) ;
	}
}

/*--------------------------------------------------------------*/
static void snapback_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			Taunt												*/
/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w )
{
	if( pl_w->routine_2 == 0 ){
		pl_w->routine_2 ++ ;
		pl_w->atck_no = pl1b_at_chouhatsu ;
		HIT_RESET( pl_w ) ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_char_set( pl_w, Char_Extra, 7 ) ;
	} else if( pl_char_move( pl_w ) < 0 ){
		pl_color_set( pl_w, 0 ) ;
		pl_stand_ret( pl_w ) ;
	}
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_move( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A kick that leaps off the wall						*/
/****************************************************************/
void	pl1b_t_kick( PLAYER_WORK *pl_w )
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
	pl_w->atck_no = pl1b_at_t_kick ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 5 ) ;
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
		pl_char_set( pl_w, Char_Extra, 6 ) ;
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
PLAYER_WORK *pl1b_catch_ck( PLAYER_WORK *pl_w )
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
static PLAYER_WORK *jump_catch_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( !( pl_w->dir = ( pl_w->atck_lever & ( KEY_FORWARD | KEY_BACK ) ) >> 10 ) ) return 0 ;
	if( pl_w->atck_pk == 0 && pl_w->atck_level == 1 && pl_w->pos.y > DOT_Y( 64.0f ) ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 1 ;
			return em ;
		}
		return 0 ;
	}
	return 0 ;
}

static void nml_set( PLAYER_WORK *pl_w );
static void jump_set( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Start the throw that pl1b_catch_ck chose			*/
/****************************************************************/
void	pl1b_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_set, jump_set,
	} ;
	PLAYER_WORK *em ;

	catch_gauge_add( pl_w ) ;
	em = pl_w->catch_w ;
	em->pl_dir = em->disp_dir = pl_w->pl_dir ^ 1 ;
	catch_set_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	if( pl_w->dir & 1 ){
		pl_w->pl_dir = pl_w->disp_dir = pl_w->pl_dir ^ 1 ;
		em = pl_w->catch_w ;
		em->pl_dir = em->disp_dir = em->pl_dir ^ 1 ;
	}
	ofs.x = DOT_X( 7.0f ) ;
	ofs.y = DOT_Y( 88.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	pl_char_set( pl_w, Char_Catch, 0 ) ;
}

/*==============================================================*/
static void jump_set( register PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;
	Float spd ;

	spd = 0.0f ;
	pl_w->acc.x = spd ;
	pl_w->spd.y = spd ;
	pl_w->acc.y = -DOT_Y( 0.25f ) ;
	if( pl_w->jump_dir >= 0 ){
		spd = pl_w->pl_dir ? DOT_X( 4.0f ) : -DOT_X( 4.0f ) ;
		if( pl_w->jump_dir ) spd = -spd ;
	}
	pl_w->spd.x = spd ;
	if( pl_w->dir & 1 ){
		pl_w->pl_dir = pl_w->disp_dir = pl_w->pl_dir ^ 1 ;
		em = pl_w->catch_w ;
		em->pl_dir = em->disp_dir = em->pl_dir ^ 1 ;
	}
	ofs.x = DOT_X( 7.0f ) ;
	ofs.y = DOT_Y( 88.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	pl_char_set( pl_w, Char_Catch, 1 ) ;
}

static void nml_catch( PLAYER_WORK *pl_w );
static void jump_catch( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void	pl1b_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_catch, jump_catch, nml_catch,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no]( pl_w ) ;
}

/*==============================================================*/
static void nml_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_char_move( pl_w ) >= 0 ){
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			em = pl_w->catch_w ;
			em->dm_hit_w = pl_w ;
			em->catch_type = 2 ;
			em->atck_no = pl1b_at_throw ;
			catch_se_req( pl_w ) ;
		}
		return ;
	}
	pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void jump_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_jump_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->atck_no = pl1b_at_air_throw ;
	}
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		if( !LO_BYTE( pl_w->cg_rival ) ) pl_stand_ret( pl_w ) ;
	}
}

/****************************************************************/
/*			Holding the opponent								*/
/****************************************************************/
void	pl1b_hold( PLAYER_WORK *pl_w )
{
	if( ( pl_w->catch_no & CATCH_NO_MASK ) != 2 ) catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void	pl1b_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl1b_sp_kikoushou ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl1b_sp_hazan ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl1b_sp_senretsu ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl1b_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl1b_sp_kikoushou ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl1b_sp_hazan ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl1b_sp_senretsu ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl1b_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl1b_sp_kikou ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl1b_sp_tenshou ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl1b_sp_senen ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void	pl1b_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl1b_sp_kikou ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl1b_sp_tenshou ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl1b_sp_senen ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl1b.c" End of File	*/

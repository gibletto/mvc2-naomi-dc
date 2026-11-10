/* Tron Bonne (character 10): command checks, normal attacks, appearance, win poses, special moves
   and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl10_def.h"

/* moves by the distance the animation gives, the way the player faces */
#define CG_MOVE_X( pl )	\
	if( ( pl )->cg_rival ){	\
		if( !( pl )->disp_dir ) ( pl )->pos.x += ( Sint16 )( pl )->cg_rival * 1.6666666f ;	\
		else ( pl )->pos.x -= ( Sint16 )( pl )->cg_rival * 1.6666666f ;	\
		( pl )->cg_rival = 0 ;	\
	}


void pl10_main( PLAYER_WORK *pl_w );
void pl10_com_ck( PLAYER_WORK *pl_w );
void pl10_atck_set( PLAYER_WORK *pl_w );
void pl10_atck( PLAYER_WORK *pl_w );
void pl10_dash( PLAYER_WORK *pl_w );
void pl10_noki( PLAYER_WORK *pl_w );
void pl10_appear( PLAYER_WORK *pl_w );
void pl10_win( PLAYER_WORK *pl_w );
void pl10_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl10_catch_ck( PLAYER_WORK *pl_w );
void pl10_catch_set( PLAYER_WORK *pl_w );
void pl10_catch( PLAYER_WORK *pl_w );
void pl10_hold( PLAYER_WORK *pl_w );
int pl10_delay_ck( PLAYER_WORK *pl_w );
void pl10_t_kick( PLAYER_WORK *pl_w );
void pl10_upper( PLAYER_WORK *pl_w );
void pl10_sp_unison( PLAYER_WORK *pl_w );
void pl10_sp_gattai( PLAYER_WORK *pl_w );
void pl10_sp_assist( PLAYER_WORK *pl_w );
void pl10_sp_zero( PLAYER_WORK *pl_w );
void pl10_air_dash( PLAYER_WORK *pl_w );
void pl10_tlocal_init( PLAYER_WORK *pl_w );

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
static Uint16 com_strike[] = { 3, 0, pl10_sp_strike, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Bonne Strike */
static Uint16 com_beacon[] = { 3, COM_GROUND, pl10_sp_beacon, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Beacon Bomb */
static Uint16 com_kobun[] = { 3, COM_GROUND, pl10_sp_kobun, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_PUNCH } ;	/* Kobun Launch */
static Uint16 com_king[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl10_sp_king, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* King Kobun (hyper) */
static Uint16 com_lunch[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl10_sp_lunch, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Lunch Rush (hyper) */

static void ( *pl10_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl10_main,
	pl10_com_ck,
	pl10_atck_set,
	pl10_atck,
	pl10_dash,
	pl10_noki,
	pl10_appear,
	pl10_win,
	pl10_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl10_catch_ck,	/* returns a value */
	pl10_catch_set,
	pl10_catch,
	pl10_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl10_delay_ck,
	pl10_t_kick,
	pl10_upper,
	pl10_sp_unison,
	pl10_sp_gattai,
	pl10_sp_assist,
	pl10_sp_zero,
	0,
	pl10_air_dash,
	0,
	pl10_tlocal_init,
} ;

static Uint8 strike_ck( PLAYER_WORK *pl_w );
static Uint8 beacon_ck( PLAYER_WORK *pl_w );
static Uint8 kobun_ck( PLAYER_WORK *pl_w );
static Uint8 king_ck( PLAYER_WORK *pl_w );
static Uint8 lunch_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w );
static int lunch_delay_ck( PLAYER_WORK *pl_w );
static int king_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl10_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl10_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl10_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( king_ck( pl_w ) ) return ;
	if( lunch_ck( pl_w ) ) return ;
	if( kobun_ck( pl_w ) ) return ;
	if( beacon_ck( pl_w ) ) return ;
	if( strike_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	pl_air_dash_ck( pl_w, &pl_w->com_w12 ) ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
/*			Once a jump unless super jumping					*/
/*==============================================================*/
static Uint8 strike_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_strike, &pl_w->com_w ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && pl_w->sjump_flag == 0 ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl10_sp_strike ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 beacon_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_beacon, &pl_w->com_w0 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl10_sp_beacon ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 kobun_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_kobun, &pl_w->com_w1 ) ) return 0 ;
	if( PL10_LW( pl_w )->throw_cnt ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl10_sp_kobun ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 king_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_king, &pl_w->com_w2 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl10_sp_king ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 lunch_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_lunch, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl10_sp_lunch ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static int snapback_ck( PLAYER_WORK *pl_w )
{
	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl10_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 3 ) ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl10_sp_chouhatsu ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl10_delay_ck( PLAYER_WORK *pl_w )
{
	if( lunch_delay_ck( pl_w ) || king_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static int lunch_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_lunch, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 5 ;
	return 1 ;
}

/*==============================================================*/
static int king_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_king, &pl_w->com_w2 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 4 ;
	return 1 ;
}

/****************************************************************/
void pl10_main( PLAYER_WORK *pl_w )
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
void pl10_atck_set( PLAYER_WORK *pl_w )
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
/*			The middle one starts without the sound				*/
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
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
}

/*==============================================================*/
static void jump_atck( PLAYER_WORK *pl_w )
{
	if( pl_w->atck_pk == 1 ) jump_kick( pl_w ) ;
	else jump_punch( pl_w ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
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
void pl10_atck( PLAYER_WORK *pl_w )
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
/*			The heavy and middle ones hit again					*/
/*==============================================================*/
static void stand_punch_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	switch( pl_w->nml_no ){
	case 2:
		if( !pl_w->cg_type ) break ;
		pl_w->cg_type = 0 ;
		pl_w->atck_no = ATCK_STAND_HP ;
		HIT_RESET( pl_w ) ;
		break ;
	case 1:
		if( !pl_w->cg_type ) break ;
		pl_w->cg_type = 0 ;
		pl_w->atck_no = ATCK_STAND_MP ;
		HIT_RESET( pl_w ) ;
		break ;
	case 0:
		break ;
	}
	CG_MOVE_X( pl_w ) ;
}

/*==============================================================*/
static void crouch_punch_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_stand_ret( pl_w ) ;
		return ;
	}
	switch( pl_w->nml_no ){
	case 0:
		break ;
	case 1:
		if( pl_w->routine_2 == 0 && pl_w->cg_type ){
			pl_w->routine_2 ++ ;
			pl_w->timer0 = 1 ;
			pl_w->timer1 = 0 ;
		}
		if( pl_w->routine_2 == 1 && pl_w->cg_type ){
			if( --pl_w->timer0 <= 0 ){
				shl10_00_set( pl_w, 0 ) ;
				shl10_00_set( pl_w, 1 ) ;
				pl_w->timer0 = 10 ;
			}
			if( ++pl_w->timer1 & 2 ) pl_color_set( pl_w, PC_DEFAULT ) ;
			else ef_color_set( pl_w, 3, 3, 1 ) ;
		}
		break ;
	case 2:
		if( pl_w->routine_2 == 0 && pl_w->cg_type ){
			pl_w->routine_2 ++ ;
			pls10_00_set( pl_w, 1 ) ;
		}
		break ;
	}
	CG_MOVE_X( pl_w ) ;
}

/*==============================================================*/
static void stand_kick_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 2:
		/* held for longer while the button is */
		if( !( pl_w->pl_new_lvbt & KEY_HK ) ) pl_w->routine_3 = 1 ;
		if( pl_w->routine_2 == 0 ){
			pl_char_move( pl_w ) ;
			if( pl_w->cg_type ){
				if( pl_w->routine_3 ) pl_w->routine_2 += 2 ;
				else {
					pl_w->routine_2 ++ ;
					pl_w->timer0 = 90 ;
				}
				return ;
			}
		}
		if( pl_w->routine_2 == 1 ){
			if( --pl_w->timer0 <= 0 || pl_w->routine_3 ) pl_w->routine_2 ++ ;
		}
		if( pl_w->routine_2 == 2 ){
			pl_char_move( pl_w ) ;
			if( pl_w->cg_type ){
				pl_w->routine_2 ++ ;
				pl_se_req( pl_w, 23 ) ;
				shl10_02_set( pl_w, 0 ) ;
			}
		} else if( pl_w->routine_2 == 3 ){
			if( pl_char_move( pl_w ) < 0 ){
				pl_stand_ret( pl_w ) ;
				return ;
			}
		}
		break ;
	case 1:
		if( pl_char_move( pl_w ) < 0 ){
			pl_stand_ret( pl_w ) ;
			return ;
		}
		if( pl_w->routine_2 == 0 && pl_w->cg_type ){
			pl_w->routine_2 ++ ;
			pls10_00_set( pl_w, 13 ) ;
		}
		if( pl_w->cg_prm ){
			pl_w->cg_prm = 0 ;
			pl_w->atck_no = ATCK_STAND_MK ;
			HIT_RESET( pl_w ) ;
		}
		break ;
	case 0:
		if( pl_char_move( pl_w ) < 0 ){
			pl_stand_ret( pl_w ) ;
			return ;
		}
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pl_w->atck_no = pl10_at_stand_lk_2 ;
			HIT_RESET( pl_w ) ;
		}
		break ;
	}
	CG_MOVE_X( pl_w ) ;
}

/*==============================================================*/
/*			The heavy one can be steered, and leaves dust		*/
/*==============================================================*/
static void crouch_kick_move( PLAYER_WORK *pl_w )
{
	VEC3 pos ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	switch( pl_w->nml_no ){
	case 0:
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pl_w->atck_no = ATCK_CROUCH_LK ;
			HIT_RESET( pl_w ) ;
		}
		break ;
	case 1:
		break ;
	default:
		break ;
	case 2:
		if( pl_w->cpu_flag == 0 ){
			if( pl_w->pl_sw_new & KEY_FORWARD ){
				if( !pl_w->pl_dir ) pl_w->pos.x += -5.0f ;
				else pl_w->pos.x -= -5.0f ;
			} else if( pl_w->pl_sw_new & KEY_BACK ){
				if( !pl_w->pl_dir ) pl_w->pos.x += DOT_X( 2.5f ) ;
				else pl_w->pos.x -= DOT_X( 2.5f ) ;
			}
		}
		if( !( Sys->game_timer0 & 0x0f ) ){
			pos = pl_w->pos ;
			if( !pl_w->disp_dir ) pos.x += 0.0f ;	/* the same either way */
			else pos.x += 0.0f ;
			pos.y += 0.0f ;
			pls10_05_burst_set( &pos, pl_w->disp_dir, 0 ) ;
		}
		break ;
	}
	CG_MOVE_X( pl_w ) ;
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
	if( pl_char_move( pl_w ) < 0 ){
		pl_jump_ret( pl_w ) ;
		return ;
	}
	switch( pl_w->nml_no ){
	case 0:
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pl_w->atck_no = ATCK_JUMP_LK ;
			HIT_RESET( pl_w ) ;
		}
		break ;
	case 1:
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pl_w->atck_no = ATCK_JUMP_MK ;
			HIT_RESET( pl_w ) ;
		}
		break ;
	case 2:
		break ;
	}
}

static void upper_init( PLAYER_WORK *pl_w );
static void upper_move( PLAYER_WORK *pl_w );
static void upper_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The launcher										*/
/****************************************************************/
void pl10_upper( PLAYER_WORK *pl_w )
{
	static void ( *upper_jmp[])( PLAYER_WORK *pl_w ) = {
		upper_init, upper_move, upper_end,
	} ;

	upper_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void upper_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_brake_set( pl_w ) ;
	pl_w->atck_no = pl10_at_upper ;
	pl_w->pl_stance = STANCE_CROUCH ;
	pl_char_set( pl_w, Char_Extra, 3 ) ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	upper_move( pl_w ) ;
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
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pls10_00_set( pl_w, 17 ) ;
	}
}

/*--------------------------------------------------------------*/
static void upper_end( PLAYER_WORK *pl_w )
{
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
void pl10_dash( PLAYER_WORK *pl_w )
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
		pl_w->spd.x = !pl_w->pl_dir ? -DOT_X( 8.0f ) : DOT_X( 8.0f ) ;
	}
}

/*--------------------------------------------------------------*/
static void dash_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( --pl_w->timer0 <= 0 ){
		pl_w->routine_2 ++ ;
		pl_w->acc.x = !pl_w->pl_dir ? 0.26041666f : -0.26041666f ;
		pl_char_set( pl_w, Char_Dash, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void dash_end( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		RATE_CLR_X( pl_w ) ;
	}
}

static void air_dash_init( PLAYER_WORK *pl_w );
static void air_dash_move( PLAYER_WORK *pl_w );
static void air_dash_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The dash in the air									*/
/****************************************************************/
void pl10_air_dash( PLAYER_WORK *pl_w )
{
	static void ( *air_dash_jmp[])( PLAYER_WORK *pl_w ) = {
		air_dash_init, air_dash_move, air_dash_end,
	} ;

	air_dash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void air_dash_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->jump_dash_cnt = 0 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_char_set( pl_w, Char_Dash, 4 ) ;
	pl_w->timer0 = pl_w->cg_prm ;
}

/*--------------------------------------------------------------*/
static void air_dash_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->spd.x = !pl_w->pl_dir ? -DOT_X( 8.0f ) : DOT_X( 8.0f ) ;
	}
	if( --pl_w->timer0 <= 0 ){
		pl_w->routine_2 ++ ;
		pl_w->acc.x = !pl_w->pl_dir ? 0.26041666f : -0.26041666f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_char_set( pl_w, Char_Dash, 5 ) ;
	}
}

/*--------------------------------------------------------------*/
static void air_dash_end( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		RATE_CLR_X( pl_w ) ;
	}
}

static void noki_init( PLAYER_WORK *pl_w );
static void noki_jump( PLAYER_WORK *pl_w );
static void noki_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash, a hop								*/
/****************************************************************/
void pl10_noki( PLAYER_WORK *pl_w )
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
		pl_w->timer0 = 20 ;
		pl_w->spd.x = !pl_w->pl_dir ? DOT_X( 8.0f ) : -DOT_X( 8.0f ) ;
		pl_w->spd.y = DOT_Y( 2.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.25f ) ;
	}
}

/*--------------------------------------------------------------*/
static void noki_jump( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->spd.y && pl_w->pos.y < pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
	if( --pl_w->timer0 <= 0 ){
		pl_w->routine_2 ++ ;
		pl_w->acc.x = !pl_w->pl_dir ? -0.26041666f : 0.26041666f ;
		pl_char_set( pl_w, Char_Dash, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void noki_land( PLAYER_WORK *pl_w )
{
	pl_smoke_set( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		RATE_CLR_X( pl_w ) ;
	}
}

static void appear_init( PLAYER_WORK *pl_w );
static void appear_move( PLAYER_WORK *pl_w );
static void appear_stop( PLAYER_WORK *pl_w );
static void appear_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			She drives in from the side of the screen			*/
/****************************************************************/
void pl10_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_init, appear_move, appear_stop, appear_end,
	} ;

	appear_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void appear_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->disp_flag = 1 ;
	pl_w->spd.z = pl_w->pos.x ;
	if( !( pl_w->work_no & 1 ) ){
		pl_w->pos.x -= DOT_X( 256.0f ) ;
		pl_w->spd.x = 10.0f ;
	} else {
		pl_w->pos.x += DOT_X( 256.0f ) ;
		pl_w->spd.x = -10.0f ;
	}
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->acc.x = 0.0f ;
	pl_w->timer0 = 30 ;
	pl_char_set( pl_w, Char_Appear, 0 ) ;
	appear_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void appear_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( --pl_w->timer0 <= 0 ){
		pl_w->routine_2 ++ ;
		pl_w->acc.x = !( pl_w->work_no & 1 ) ? -DOT_X( 0.25f ) : DOT_X( 0.25f ) ;
		pl_char_set( pl_w, Char_Appear, 1 ) ;
		pl_voice_req( pl_w, 10 ) ;
	}
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
/*			Until the brakes stop her where she should stand	*/
/*==============================================================*/
static void appear_stop( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_w->spd.x * pl_w->acc.x > 0.0f ){
		pl_w->routine_2 ++ ;
		pl_w->pos.x = pl_w->spd.z ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_char_set( pl_w, Char_Appear, 2 ) ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void appear_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_w->routine_1 ++ ;
}

static void win_init( PLAYER_WORK *pl_w );
static void win_main( PLAYER_WORK *pl_w );
static void win_time_ko( PLAYER_WORK *pl_w );
static void win_ko_init( PLAYER_WORK *pl_w );
static void win_ko_move( PLAYER_WORK *pl_w );
static void win_scene( PLAYER_WORK *pl_w );
static void scene_init( PLAYER_WORK *pl_w );
static void scene_walk( PLAYER_WORK *pl_w );
static void scene_stop( PLAYER_WORK *pl_w );
static void scene_move( PLAYER_WORK *pl_w );
static void scene_wait( PLAYER_WORK *pl_w );
static void scene_end( PLAYER_WORK *pl_w );
static void scene_keep( PLAYER_WORK *pl_w );
static void lose_time_ko( PLAYER_WORK *pl_w );
static void lose_init( PLAYER_WORK *pl_w );
static void lose_move( PLAYER_WORK *pl_w );
static void lose_end( PLAYER_WORK *pl_w );
static void win_type_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl10_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_init, win_main,
	} ;

	win_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void win_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	if( !pl_w->type ) win_type_ck( pl_w ) ;
	win_main( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_main( PLAYER_WORK *pl_w )
{
	static void ( *win_type_jmp[])( PLAYER_WORK *pl_w ) = {
		win_time_ko,
		lose_time_ko,
		win_time_ko,
		lose_time_ko,
		lose_time_ko,
		win_time_ko,
		win_time_ko,
		win_time_ko,
		win_scene,
		win_scene,
		win_scene,
	} ;

	if( pl_win_end_ck( pl_w ) ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	win_type_jmp[pl_w->type]( pl_w ) ;
}

/*==============================================================*/
static void win_time_ko( PLAYER_WORK *pl_w )
{
	static void ( *win_ko_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko_init, win_ko_move,
	} ;

	win_ko_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void win_ko_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_char_set( pl_w, Char_Win, 0 ) ;
	win_ko_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_ko_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

/* walks out from the edge of the stage */
#define SCENE_WALK_SET( pl )	{	\
	( pl )->routine_3 ++ ;	\
	( pl )->spd.x = !( pl )->disp_dir ? -DOT_X( 4.0f ) : DOT_X( 4.0f ) ;	\
	( pl )->acc.x = 0.0f ;	\
	( pl )->spd.y = 0.0f ;	\
	( pl )->acc.y = 0.0f ;	\
	pl_char_set( pl, Char_Stand, 2 ) ;	\
	scene_walk( pl ) ;	\
}

/*==============================================================*/
/*			The wins picked by a button: she walks out from		*/
/*			the edge of the stage first							*/
/*==============================================================*/
static void win_scene( PLAYER_WORK *pl_w )
{
	static void ( *scene_jmp[])( PLAYER_WORK *pl_w ) = {
		scene_init, scene_walk, scene_stop, scene_move, scene_wait, scene_end, scene_keep,
	} ;

	pl_w->push_ng_timer = 1 ;
	scene_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void scene_init( PLAYER_WORK *pl_w )
{
	pl_color_set( pl_w, PC_DEFAULT ) ;
	ef_color_set( pl_w, 5, 0, 1 ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	if( pl_w->disp_dir == 0 && pl_w->pos.x > scr_work.scr_right + -320.0f ){
		SCENE_WALK_SET( pl_w ) ;
	} else if( pl_w->disp_dir && pl_w->pos.x < scr_work.scr_left + 320.0f ){
		SCENE_WALK_SET( pl_w ) ;
	} else {
		pl_w->routine_3 = 2 ;
		scene_stop( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void scene_walk( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_w->disp_dir == 0 && pl_w->pos.x <= scr_work.scr_right + -320.0f ){
		pl_w->routine_3 ++ ;
		scene_stop( pl_w ) ;
	} else if( pl_w->disp_dir && pl_w->pos.x >= scr_work.scr_left + 320.0f ){
		pl_w->routine_3 ++ ;
		scene_stop( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void scene_stop( PLAYER_WORK *pl_w )
{
	pl_w->win_wait = 0xff ;
	pl_w->routine_3 = 3 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pls10_00_set( pl_w, 0 ) ;
	pl_char_set( pl_w, Char_Win, 3 ) ;
	scene_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void scene_move( PLAYER_WORK *pl_w )
{
	pl_w->win_wait = 0xff ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->timer0 = 60 ;
		pls10_04_set( pl_w, 0 ) ;
		pls10_04_set( pl_w, 1 ) ;
		pls10_04_set( pl_w, 2 ) ;
		pls10_04_set( pl_w, 3 ) ;
		pls10_04_set( pl_w, 4 ) ;
		pls10_04_set( pl_w, 5 ) ;
		pls10_04_set( pl_w, 6 ) ;
		pls10_04_set( pl_w, 7 ) ;
		pls10_04_set( pl_w, 8 ) ;
		pls10_04_set( pl_w, 9 ) ;
	}
}

/*--------------------------------------------------------------*/
static void scene_wait( PLAYER_WORK *pl_w )
{
	pl_w->win_wait = 0xff ;
	if( --pl_w->timer0 <= 0 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Win, 4 ) ;
	}
}

/*--------------------------------------------------------------*/
static void scene_end( PLAYER_WORK *pl_w )
{
	pl_w->win_wait = 0xff ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Damage, 31 ) ;
	}
}

/*--------------------------------------------------------------*/
static void scene_keep( PLAYER_WORK *pl_w )
{
	pl_w->win_wait = 0xff ;
}

/*==============================================================*/
static void lose_time_ko( PLAYER_WORK *pl_w )
{
	static void ( *lose_jmp[])( PLAYER_WORK *pl_w ) = {
		lose_init, lose_move, lose_end,
	} ;

	lose_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void lose_init( PLAYER_WORK *pl_w )
{
	VEC3 pos ;

	pl_w->routine_3 ++ ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	ef_color_set( pl_w, 5, 1, 1 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	if( !pl_w->disp_dir ) pos.x = pl_w->pos.x + 0.0f ;
	else pos.x = pl_w->pos.x + -0.0f ;
	pos.y = pl_w->pos.y + DOT_Y( 40.0f ) ;
	burst_set( pl_w, &pos, 1, 8 ) ;
	pl_se_req( pl_w, 74 ) ;
	pl_char_set( pl_w, Char_Win, 2 ) ;
	lose_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void lose_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		pls10_01_set( pl_w, 0 ) ;
		pls10_01_set( pl_w, 1 ) ;
		pls10_01_set( pl_w, 2 ) ;
		pls10_01_set( pl_w, 3 ) ;
		pls10_01_set( pl_w, 4 ) ;
		pls10_01_set( pl_w, 5 ) ;
		pls10_01_set( pl_w, 6 ) ;
		pls10_01_set( pl_w, 7 ) ;
		pls10_01_set( pl_w, 8 ) ;
		pls10_01_set( pl_w, 10 ) ;
		pls10_01_set( pl_w, 11 ) ;
	}
}

/*--------------------------------------------------------------*/
static void lose_end( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

/* the scene for a win picked by the button held, or at random */
static Sint16 win_type_tbl[16] = { 5, 5, 5, 6, 6, 7, 7, 7, 8, 8, 8, 8, 9, 10, 10, 10 } ;

/*==============================================================*/
static void win_type_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->pad_sw.now & KEY_BUTTON ){
		pl_w->type = 5 ;
		if( pl_w->pad_sw.now & KEY_HP ) pl_w->type = 6 ;
		if( pl_w->pad_sw.now & KEY_A1 ) pl_w->type = 7 ;
		if( pl_w->pad_sw.now & KEY_LK ) pl_w->type = 8 ;
		if( pl_w->pad_sw.now & KEY_HK ) pl_w->type = 9 ;
		if( pl_w->pad_sw.now & KEY_A2 ) pl_w->type = 10 ;
	} else pl_w->type = win_type_tbl[Rnd() & 0x0f] ;
	if( !vs_sys_work.vs_end_flag ){
		if( pl_w->type == 8 || pl_w->type == 9 || pl_w->type == 10 ) pl_w->type = 5 ;
	}
	if( pl_be_cnt( pl_w ) > 1 ) pl_w->type = 5 ;
}

static void sp_strike( PLAYER_WORK *pl_w );
static void sp_beacon( PLAYER_WORK *pl_w );
static void sp_kobun( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_king( PLAYER_WORK *pl_w );
static void sp_lunch( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );
static void sp_throw( PLAYER_WORK *pl_w );
static void sp_kick( PLAYER_WORK *pl_w );
static void sp_fire( PLAYER_WORK *pl_w );

/****************************************************************/
void pl10_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *pl_sp_tbl[])( PLAYER_WORK *pl_w ) = {
		sp_strike,					/* 00 Bonne Strike */
		sp_beacon,					/* 01 Beacon Bomb */
		sp_kobun,					/* 02 Kobun Launch */
		sp_chouhatsu,					/* 03 the taunt */
		sp_king,					/* 04 King Kobun (hyper) */
		sp_lunch,					/* 05 Lunch Rush (hyper) */
		sp_snapback,					/* 06 the snapback */
		sp_throw,					/* 07 a throw (assist) */
		sp_kick,					/* 08 a kick (assist) */
		sp_fire,					/* 09 Gustaff's fire (assist) */
	} ;

	pl_sp_tbl[pl_w->pl_sp_type]( pl_w ) ;
}

/* how long Bonne Strike's rush lasts, on the ground and in the air, for each strength */
static Sint16 strike_time_tbl[2][2] = { { 12, 24 }, { 12, 24 } } ;

static void strike_stand( PLAYER_WORK *pl_w );
static void strike_air( PLAYER_WORK *pl_w );
static void strike_init( PLAYER_WORK *pl_w );
static void strike_start( PLAYER_WORK *pl_w );
static void strike_accel( PLAYER_WORK *pl_w );
static void strike_rush( PLAYER_WORK *pl_w );
static void strike_stop( PLAYER_WORK *pl_w );
static void strike_air_init( PLAYER_WORK *pl_w );
static void strike_air_start( PLAYER_WORK *pl_w );
static void strike_air_accel( PLAYER_WORK *pl_w );
static void strike_air_rush( PLAYER_WORK *pl_w );
static void strike_air_stop( PLAYER_WORK *pl_w );
static void strike_air_move( PLAYER_WORK *pl_w );
static void strike_renda( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_strike( PLAYER_WORK *pl_w )
{
	static void ( *strike_jmp[])( PLAYER_WORK *pl_w ) = {
		strike_stand, strike_air,
	} ;

	strike_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void strike_stand( PLAYER_WORK *pl_w )
{
	static void ( *strike_stand_jmp[])( PLAYER_WORK *pl_w ) = {
		strike_init, strike_start, strike_accel, strike_rush, strike_stop,
	} ;

	strike_stand_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void strike_init( PLAYER_WORK *pl_w )
{
	PL10_LOCAL_W *lw = PL10_LW( pl_w ) ;

	if( pl_w->pl_stance == STANCE_AIR ){
		pl_w->routine_2 = 1 ;
		strike_air( pl_w ) ;
		return ;
	}
	pl_w->routine_3 ++ ;
	pl_w->timer1 = 1 ;
	lw->renda_cnt = 0 ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 0 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->atck_no = pl10_at_strike ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 3 ) ;
	pl_sp_ef_set( pl_w ) ;
	strike_start( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void strike_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = !pl_w->disp_dir ? -DOT_X( 2.0f ) : DOT_X( 2.0f ) ;
		pl_se_req( pl_w, 75 ) ;
		ef_color_set( pl_w, 5, 4, 1 ) ;
		pls10_00_set( pl_w, 14 ) ;
	}
	strike_renda( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void strike_accel( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( !pl_w->disp_dir && pl_w->spd.x <= -20.0f ){
		pl_w->routine_3 ++ ;
		pl_w->timer0 = strike_time_tbl[pl_w->routine_2][pl_w->atck_level] ;
		pl_w->spd.x = !pl_w->disp_dir ? -20.0f : 20.0f ;
		pl_w->acc.x = 0.0f ;
	} else if( pl_w->disp_dir && pl_w->spd.x >= 20.0f ){
		pl_w->routine_3 ++ ;
		pl_w->timer0 = strike_time_tbl[pl_w->routine_2][pl_w->atck_level] ;
		pl_w->spd.x = !pl_w->disp_dir ? -20.0f : 20.0f ;
		pl_w->acc.x = 0.0f ;
	}
	strike_renda( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void strike_rush( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_char_move( pl_w ) ;
	strike_renda( pl_w ) ;
	if( pl_w->timer0 == 2 ){
		pl_w->atck_no = pl10_at_strike_last ;
		HIT_RESET( pl_w ) ;
	}
	if( -- pl_w->timer0 <= 0 ){
		pl_w->routine_3 ++ ;
		pl_w->acc.x = !pl_w->disp_dir ? DOT_X( 2.0f ) : -DOT_X( 2.0f ) ;
		pl_char_set( pl_w, Char_Special, 4 ) ;
	}
}

/*--------------------------------------------------------------*/
static void strike_stop( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_w->spd.x == 0.0f || pl_w->spd.x * pl_w->acc.x > 0.0f ) RATE_CLR_X( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_color_set( pl_w, 0 ) ;
		pl_voice_req( pl_w, 43 ) ;
		pl_stand_ret( pl_w ) ;
	}
	CG_MOVE_X( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void strike_air( PLAYER_WORK *pl_w )
{
	static void ( *strike_air_jmp[])( PLAYER_WORK *pl_w ) = {
		strike_air_init, strike_air_start, strike_air_accel, strike_air_rush, strike_air_stop,
	} ;

	strike_air_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void strike_air_init( PLAYER_WORK *pl_w )
{
	PL10_LOCAL_W *lw = PL10_LW( pl_w ) ;

	pl_w->routine_3 ++ ;
	pl_w->timer1 = 1 ;
	lw->renda_cnt = 0 ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 0 ) ;
	pl_w->spd.x /= 8.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y /= 8.0f ;
	pl_w->acc.y /= 64.0f ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->atck_no = pl10_at_strike_air ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 5 ) ;
	strike_air_start( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void strike_air_start( PLAYER_WORK *pl_w )
{
	strike_air_move( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = !pl_w->disp_dir ? -DOT_X( 2.0f ) : DOT_X( 2.0f ) ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_se_req( pl_w, 75 ) ;
		ef_color_set( pl_w, 5, 4, 1 ) ;
		pls10_00_set( pl_w, 15 ) ;
	}
	strike_renda( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void strike_air_accel( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( !pl_w->disp_dir && pl_w->spd.x <= -20.0f ){
		pl_w->routine_3 ++ ;
		pl_w->timer0 = strike_time_tbl[pl_w->routine_2][pl_w->atck_level] ;
		pl_w->spd.x = !pl_w->disp_dir ? -20.0f : 20.0f ;
		pl_w->acc.x = 0.0f ;
	} else if( pl_w->disp_dir && pl_w->spd.x >= 20.0f ){
		pl_w->routine_3 ++ ;
		pl_w->timer0 = strike_time_tbl[pl_w->routine_2][pl_w->atck_level] ;
		pl_w->spd.x = !pl_w->disp_dir ? -20.0f : 20.0f ;
		pl_w->acc.x = 0.0f ;
	}
	strike_renda( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void strike_air_rush( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_char_move( pl_w ) ;
	strike_renda( pl_w ) ;
	if( pl_w->timer0 == 2 ){
		pl_w->atck_no = pl10_at_strike_air_last ;
		HIT_RESET( pl_w ) ;
	}
	if( -- pl_w->timer0 <= 0 ){
		pl_w->routine_3 ++ ;
		pl_w->acc.x = !pl_w->disp_dir ? DOT_X( 0.5f ) : -DOT_X( 0.5f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_char_set( pl_w, Char_Special, 6 ) ;
	}
}

/*--------------------------------------------------------------*/
static void strike_air_stop( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ) pl_w->pos.y = pl_w->ground_y ;
	if( pl_w->spd.x == 0.0f || pl_w->spd.x * pl_w->acc.x > 0.0f ) RATE_CLR_X( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_color_set( pl_w, 0 ) ;
		pl_voice_req( pl_w, 43 ) ;
		pl_jump_ret( pl_w ) ;
	}
}

/*==============================================================*/
static void strike_air_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->ground_y >= pl_w->pos.y ) pl_w->pos.y = pl_w->ground_y ;
}

/*==============================================================*/
/*			Pressing the buttons makes the rush hit sooner		*/
/*==============================================================*/
static void strike_renda( PLAYER_WORK *pl_w )
{
	PL10_LOCAL_W *lw ;

	if( -- pl_w->timer1 == 0 ) goto hit ;
	if( !sw_renda_ck( pl_w ) ) return ;
	lw = PL10_LW( pl_w ) ;
	lw->renda_cnt ++ ;
	if( lw->renda_cnt < 1 ) return ;
	lw->renda_cnt = 0 ;
	pl_w->timer1 -= 3 ;
	if( pl_w->timer1 > 0 ) return ;
hit:
	pl_w->timer1 = 4 ;
	if( pl_w->pl_stance == STANCE_STAND ){
		pl_w->atck_no = pl10_at_strike ;
		HIT_RESET( pl_w ) ;
	} else {
		pl_w->atck_no = pl10_at_strike_air ;
		HIT_RESET( pl_w ) ;
	}
}

static void beacon_init( PLAYER_WORK *pl_w );
static void beacon_wait( PLAYER_WORK *pl_w );
static void beacon_shot( PLAYER_WORK *pl_w );
static void beacon_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_beacon( PLAYER_WORK *pl_w )
{
	static void ( *beacon_jmp[])( PLAYER_WORK *pl_w ) = {
		beacon_init, beacon_wait, beacon_shot, beacon_end,
	} ;

	beacon_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void beacon_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 0 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->atck_no = pl10_at_beacon ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level ) ;
	pl_sp_ef_set( pl_w ) ;
	beacon_wait( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void beacon_wait( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_prm ){
		pl_w->routine_2 ++ ;
		if( !pl_w->atck_level ) pls10_00_set( pl_w, 4 ) ;
		else pls10_00_set( pl_w, 7 ) ;
	}
}

/*--------------------------------------------------------------*/
static void beacon_shot( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_voice_req( pl_w, 47 ) ;
		pls10_00_set( pl_w, !pl_w->atck_level ? 2 : 3 ) ;
		shl10_04_set( pl_w, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void beacon_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void kobun_init( PLAYER_WORK *pl_w );
static void kobun_move( PLAYER_WORK *pl_w );
static void kobun_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_kobun( PLAYER_WORK *pl_w )
{
	static void ( *kobun_jmp[])( PLAYER_WORK *pl_w ) = {
		kobun_init, kobun_move, kobun_end,
	} ;

	kobun_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void kobun_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 0 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->atck_no = pl10_at_kobun ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 2 ) ;
	pl_sp_ef_set( pl_w ) ;
	kobun_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void kobun_move( PLAYER_WORK *pl_w )
{
	VEC3 pos ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		shl10_01_set( pl_w, pl_w->atck_level ) ;
		pos = pl_w->pos ;
		pos.x += !pl_w->disp_dir ? -DOT_X( 80.0f ) : DOT_X( 80.0f ) ;
		pos.y += 240.0f ;
		pls10_05_burst_set( &pos, pl_w->disp_dir, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void kobun_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void king_init( PLAYER_WORK *pl_w );
static void king_wait( PLAYER_WORK *pl_w );
static void king_move( PLAYER_WORK *pl_w );
static void king_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo										*/
/*==============================================================*/
static void sp_king( PLAYER_WORK *pl_w )
{
	static void ( *king_jmp[])( PLAYER_WORK *pl_w ) = {
		king_init, king_wait, king_move, king_end,
	} ;

	king_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void king_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	PL10_LW( pl_w )->claw_hop_cnt = 0 ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 0 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->atck_no = pl10_at_king ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 0 ) ;
	pl_sp_ef_set( pl_w ) ;
	king_wait( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void king_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		scr_excep_set( pl_w, 13, !pl_w->pl_no ? 3 : 4 ) ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = 20.0f ;
		ofs.y = DOT_Y( 108.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void king_move( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		scr_excep_clr() ;
	}
}

/*--------------------------------------------------------------*/
static void king_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void lunch_init( PLAYER_WORK *pl_w );
static void lunch_wait( PLAYER_WORK *pl_w );
static void lunch_start( PLAYER_WORK *pl_w );
static void lunch_open( PLAYER_WORK *pl_w );
static void lunch_hit_wait( PLAYER_WORK *pl_w );
static void lunch_shot( PLAYER_WORK *pl_w );
static void lunch_end( PLAYER_WORK *pl_w );
static int lunch_shot_set( PLAYER_WORK *pl_w );
static int catch_hit_ck( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo that fires a run of shots				*/
/*==============================================================*/
static void sp_lunch( PLAYER_WORK *pl_w )
{
	static void ( *lunch_jmp[])( PLAYER_WORK *pl_w ) = {
		lunch_init, lunch_wait, lunch_start, lunch_open, lunch_hit_wait, lunch_shot, lunch_end,
	} ;

	lunch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void lunch_init( PLAYER_WORK *pl_w )
{
	PL10_LOCAL_W *lw = PL10_LW( pl_w ) ;

	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	PL10_LW( pl_w )->claw_hop_cnt = 0 ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 0 ) ;
	ef_color_set( pl_w, 5, 2, 1 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	lw->run_cnt = 0 ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->atck_no = pl10_at_lunch ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 1 ) ;
	pl_sp_ef_set( pl_w ) ;
	lunch_wait( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void lunch_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		scr_excep_set( pl_w, 13, !pl_w->pl_no ? 3 : 4 ) ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = -25.0f ;
		ofs.y = DOT_Y( 107.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void lunch_start( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_prm ){
		pl_w->routine_2 ++ ;
		pls10_00_lunch_set( pl_w, 10 ) ;
	}
}

/*--------------------------------------------------------------*/
static void lunch_open( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 0 ;
		pl_w->timer1 = 1 ;
		pl_voice_req( pl_w, 47 ) ;
		pls10_00_set( pl_w, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void lunch_hit_wait( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( catch_hit_ck( pl_w ) || pl_w->team_type == 4 || pl_w->team_type == 5 ){
		pl_w->routine_2 ++ ;
	} else if( pl_w->cg_type != 1 ){
		pl_w->routine_2 += 2 ;
		scr_excep_clr() ;
	}
}

/*--------------------------------------------------------------*/
static void lunch_shot( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_w->cg_type ) pl_char_move( pl_w ) ;
	if( lunch_shot_set( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		scr_excep_clr() ;
	}
}

/*--------------------------------------------------------------*/
static void lunch_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/* pairs of a shot and the frames to the next, ending at -1 */
static Sint8 lunch_shot_tbl[] = {
	 2, 2, 13, 2,  5, 2,  6, 2,  2, 2,  3, 2,  5, 2, 11, 2,
	 3, 2,  4, 2,  0, 2,  4, 2, 12, 2, 13, 2,  1, 2,  2, 2,
	 4, 2,  5, 2,  0, 2,  1, 2,  3, 2, 10, 2,  0, 2,  3, 2,
	 4, 2,  1, 2,  2, 2,  6, 2,  7, 2,  8, 2,  9, 2, 10, 2,
	 0, 2, 14, 2,  1, 2,  2, 2,  0, 2,  4, 2,  5, 2, 12, 2,
	-1, 0,
} ;

/*==============================================================*/
/*			Fires the shots due									*/
/*==============================================================*/
static int lunch_shot_set( PLAYER_WORK *pl_w )
{
	Sint8 no ;

	if( -- pl_w->timer1 ) return 0 ;
	do {
		no = lunch_shot_tbl[pl_w->timer0 * 2] ;
		if( no == -1 ) break ;
		shl10_03_set( pl_w, no, pl_w->timer0 ) ;
		pl_w->timer1 = lunch_shot_tbl[pl_w->timer0 * 2 + 1] ;
		pl_w->timer0 ++ ;
	} while( !pl_w->timer1 ) ;
	return no ;
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
	pl_w->atck_no = pl10_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 7 ) ;
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
		ofs.x = -DOT_X( 20.0f ) ;
		ofs.y = DOT_Y( 48.0f ) ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
	CG_MOVE_X( pl_w ) ;
}

static void chouhatsu_init( PLAYER_WORK *pl_w );
static void chouhatsu_end( PLAYER_WORK *pl_w );

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
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 0 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_char_set( pl_w, Char_Extra, 2 ) ;
	chouhatsu_end( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void chouhatsu_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_jump( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
void pl10_t_kick( PLAYER_WORK *pl_w )
{
	static void ( *t_kick_jmp[])( PLAYER_WORK *pl_w ) = {
		t_kick_init, t_kick_jump, t_kick_land,
	} ;

	t_kick_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void t_kick_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_color_set( pl_w, 0 ) ;
	if( !pl_w->disp_dir ) pl_w->spd.x = -30.0f ;
	else pl_w->spd.x = 30.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 0.125f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->atck_no = pl10_at_t_kick ;
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

static void throw_init( PLAYER_WORK *pl_w );
static void throw_catch( PLAYER_WORK *pl_w );
static void throw_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A throw done as a special move						*/
/*==============================================================*/
static void sp_throw( PLAYER_WORK *pl_w )
{
	static void ( *throw_jmp[])( PLAYER_WORK *pl_w ) = {
		throw_init, throw_catch, throw_end,
	} ;

	throw_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void throw_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 0 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_char_set( pl_w, Char_Catch, 2 ) ;
	throw_catch( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void throw_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 0xc2 ;
			pl_catch_set( pl_w, em ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void throw_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void kick_init( PLAYER_WORK *pl_w );
static void kick_move( PLAYER_WORK *pl_w );
static void kick_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_kick( PLAYER_WORK *pl_w )
{
	static void ( *kick_jmp[])( PLAYER_WORK *pl_w ) = {
		kick_init, kick_move, kick_end,
	} ;

	kick_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void kick_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 0 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->atck_no = pl10_at_kick ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 8 ) ;
	kick_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void kick_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pls10_00_set( pl_w, 16 ) ;
	}
}

/*--------------------------------------------------------------*/
static void kick_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void fire_init( PLAYER_WORK *pl_w );
static void fire_move( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Flashes while it fires a shot every ten frames		*/
/*==============================================================*/
static void sp_fire( PLAYER_WORK *pl_w )
{
	static void ( *fire_jmp[])( PLAYER_WORK *pl_w ) = {
		fire_init, fire_move,
	} ;

	fire_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void fire_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 0 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->timer0 = 1 ;
	pl_w->timer1 = 0 ;
	pl_w->atck_no = pl10_at_fire ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 9 ) ;
	fire_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void fire_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_color_set( pl_w, 0 ) ;
		pl_stand_ret( pl_w ) ;
	}
	if( pl_w->cg_type ){
		if( -- pl_w->timer0 <= 0 ){
			shl10_00_set( pl_w, 2 ) ;
			shl10_00_set( pl_w, 3 ) ;
			pl_w->timer0 = 10 ;
		}
		if( ++ pl_w->timer1 & 2 ) pl_color_set( pl_w, 0 ) ;
		else ef_color_set( pl_w, 3, 3, 1 ) ;
	}
}

static PLAYER_WORK *nml_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *crouch_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *jump_catch_ck( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Can a throw start?									*/
/****************************************************************/
PLAYER_WORK *pl10_catch_ck( PLAYER_WORK *pl_w )
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

	if( !( pl_w->dir = ( pl_w->atck_lever & ( KEY_DOWN | KEY_FORWARD | KEY_BACK ) ) >> 10 ) ) return 0 ;
	if( pl_w->atck_pk == 0 && pl_w->atck_level == 1 && pl_w->pos.y > DOT_Y( 64.0f ) ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 1 ;
			return em ;
		}
		return 0 ;
	}
	return 0 ;
}

static void nml_p_set( PLAYER_WORK *pl_w );
static void jump_p_set( PLAYER_WORK *pl_w );
static void throw_set( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Start the throw pl10_catch_ck or sp_throw chose		*/
/****************************************************************/
void pl10_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_set, jump_p_set, throw_set, nml_p_set,
	} ;

	catch_set_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_set( PLAYER_WORK *pl_w )
{
	if( pl_w->dir & 1 ){
		pl_w->pl_dir ^= 1 ;
		pl_w->disp_dir = pl_w->pl_dir ;
	}
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 0 ) ;
}

/*==============================================================*/
static void jump_p_set( PLAYER_WORK *pl_w )
{
	if( pl_w->dir & 1 ){
		pl_w->pl_dir ^= 1 ;
		pl_w->disp_dir = pl_w->pl_dir ;
	}
	pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y /= 2.0f ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 1 ) ;
}

/*==============================================================*/
static void throw_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	ofs.x = -DOT_X( 76.0f ) ;
	ofs.y = 90.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	pl_char_set( pl_w, Char_Catch, 3 ) ;
}

static void nml_p_catch( PLAYER_WORK *pl_w );
static void jump_p_catch( PLAYER_WORK *pl_w );
static void throw_throw( PLAYER_WORK *pl_w );
static void jump_p_throw( PLAYER_WORK *pl_w );
static void jump_p_fall( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void pl10_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_catch, jump_p_catch, throw_throw, nml_p_catch,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
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
	if( pl_w->cg_prm == 1 ){
		pl_w->cg_prm = 0 ;
		ofs.x = -DOT_X( 76.0f ) ;
		ofs.y = 90.0f ;
		nage_hit_set( pl_w, &ofs ) ;
		pl_w->hit_stop = 10 ;
	}
	if( pl_w->cg_type == 1 ){
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->pl_dir = pl_w->pl_dir ^ 1 ;
		em->pl_stance = STANCE_AIR ;
		em->atck_no = pl10_at_throw_p ;
		em->catch_type = 1 ;
		scr_excep_clr() ;
	}
}

/*==============================================================*/
static void jump_p_catch( PLAYER_WORK *pl_w )
{
	static void ( *jump_p_jmp[])( PLAYER_WORK *pl_w ) = {
		jump_p_throw, jump_p_fall,
	} ;

	jump_p_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void jump_p_throw( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	strike_air_move( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_prm == 1 ){
		pl_w->cg_prm = 0 ;
		ofs.x = -DOT_X( 61.0f ) ;
		ofs.y = DOT_Y( 58.0f ) ;
		nage_hit_set( pl_w, &ofs ) ;
		pl_w->hit_stop = 10 ;
	}
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->pl_stance = STANCE_AIR ;
		em->atck_no = pl10_at_air_throw ;
		em->pl_dir = pl_w->pl_dir ^ 1 ;
		if( !pl_w->cg_rival ) pl_jump_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void jump_p_fall( PLAYER_WORK *pl_w )
{
	strike_air_move( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/*==============================================================*/
static void throw_throw( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->pl_dir = pl_w->pl_dir ^ 1 ;
		em->pl_stance = STANCE_AIR ;
		em->atck_no = pl10_at_assist_throw ;
		em->catch_type = 1 ;
	}
}

/****************************************************************/
/*			Holding the opponent								*/
/****************************************************************/
void pl10_hold( PLAYER_WORK *pl_w )
{
	if( ( pl_w->catch_no & CATCH_NO_MASK ) != 3 ) catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void pl10_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl10_sp_lunch ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl10_sp_king ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl10_sp_king ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl10_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl10_sp_lunch ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl10_sp_king ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl10_sp_king ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl10_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl10_sp_throw ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl10_sp_kick ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl10_sp_fire ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void pl10_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl10_sp_strike ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl10_sp_kick ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl10_sp_fire ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void pl10_tlocal_init( PLAYER_WORK *pl_w )
{
	PL10_LOCAL_W *lw = PL10_LW( pl_w ) ;
	PLAYER_WORK *keep0 = lw->servbot_w ;
	Uint8 keep4 = lw->servbot_flag ;

	memset( pl_w->tlocal, 0, sizeof( pl_w->tlocal ) ) ;
	lw->servbot_w = keep0 ;
	lw->servbot_flag = keep4 ;
}

/*==============================================================*/
/*			Has Lunch Rush's beam caught the opponent?			*/
/*==============================================================*/
static int catch_hit_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( !( pl_w->hit_flag & HR_HIT ) ) return 0 ;
	if( pl_w->em_w->work_kind ) return 0 ;
	if( pl_w->hit_flag & ( HR_GUARD | HR_ARMOR ) ) return 0 ;
	if( pl_w != pl_w->em_w->dm_hit_w ) return 0 ;
	em = pl_w->em_w ;
	if( em->team_no ) return 0 ;
	if( em->pl_stance == STANCE_DOWN ) return 0 ;
	if( !em->vital_new ) return 0 ;
	if( em->routine_1 != PL_DAMAGE ) return 0 ;
	em = &pl_work[pl_w->work_no] ;
	if( em->routine_1 == PL_CATCH ) return 0 ;
	return 1 ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl10.c" End of File	*/

/* Dan (character 23): command checks, normal attacks, dashes, appearance, win and lose poses,
   special moves, taunts and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl23_def.h"

/* the character's own work, in tlocal */
typedef struct PL23_LOCAL_W {
	Uint8	buraiken_hit;	/* 0: sp_buraiken has landed its first hit */
	Uint8	kouryuu_cnt;	/* 1: counts the uppercuts; every fourth uses the other arm */
} PL23_LOCAL_W;

void pl23_main( PLAYER_WORK *pl_w );
void pl23_com_ck( PLAYER_WORK *pl_w );
void pl23_atck_set( PLAYER_WORK *pl_w );
void pl23_atck( PLAYER_WORK *pl_w );
void pl23_dash( PLAYER_WORK *pl_w );
void pl23_noki( PLAYER_WORK *pl_w );
void pl23_appear( PLAYER_WORK *pl_w );
void pl23_win( PLAYER_WORK *pl_w );
void pl23_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl23_catch_ck( PLAYER_WORK *pl_w );
void pl23_catch_set( PLAYER_WORK *pl_w );
void pl23_catch( PLAYER_WORK *pl_w );
void pl23_hold( PLAYER_WORK *pl_w );
int pl23_delay_ck( PLAYER_WORK *pl_w );
void pl23_t_kick( PLAYER_WORK *pl_w );
void pl23_upper( PLAYER_WORK *pl_w );
void pl23_sp_unison( PLAYER_WORK *pl_w );
void pl23_sp_gattai( PLAYER_WORK *pl_w );
void pl23_sp_assist( PLAYER_WORK *pl_w );
void pl23_sp_zero( PLAYER_WORK *pl_w );

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
static Uint16 com_gadou[] = { 3, COM_GROUND, pl23_sp_gadou, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Gadouken */
static Uint16 com_kouryuu[] = { 3, COM_GROUND, pl23_sp_kouryuu, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_PUNCH } ;	/* Kouryuuken */
static Uint16 com_danku[] = { 3, COM_GROUND, pl23_sp_danku, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Dankuukyaku */
static Uint16 com_jdanku[] = { 3, COM_AIR, pl23_sp_jdanku, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Dankuukyaku in the air */
static Uint16 com_shinkuu[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl23_sp_shinkuu, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Shinkuu Gadouken (hyper) */
static Uint16 com_rekka[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl23_sp_rekka, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Kouryuu Rekka (hyper) */
static Uint16 com_buraiken[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl23_sp_buraiken, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Hisshou Buraiken (hyper) */
static Uint16 com_zenten[] = { 3, COM_GROUND | COM_SW_NOW, pl23_sp_zenten, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_START } ;	/* Zenten Chouhatsu (rolling taunt) */
static Uint16 com_kouten[] = { 3, COM_GROUND | COM_SW_NOW, pl23_sp_kouten, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_START } ;	/* Kouten Chouhatsu (back roll taunt) */
static Uint16 com_densetsu[] = { 6, COM_GROUND | COM_HYPER | COM_SW_NOW, pl23_sp_densetsu, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_START } ;	/* Chouhatsu Densetsu (hyper taunt) */
static Uint16 com_jburaiken[] = { 3, COM_AIR | COM_HYPER | COM_ALL_SW, pl23_sp_jburaiken, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Hisshou Buraiken in the air (hyper, unused) */
static Uint16 com_sign[] = { 3, COM_GROUND, pl23_sp_sign, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Premium Sign */
static Uint16 com_otoko[] = { 4, COM_GROUND | COM_HYPER, pl23_sp_otoko, 0, KEY_HP, KEY_LK, KEY_BACK, KEY_LP, KEY_LP } ;	/* Otoko Michi (level 3 hyper) */

static void ( *pl23_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl23_main,
	pl23_com_ck,
	pl23_atck_set,
	pl23_atck,
	pl23_dash,
	pl23_noki,
	pl23_appear,
	pl23_win,
	pl23_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl23_catch_ck,	/* returns a value */
	pl23_catch_set,
	pl23_catch,
	pl23_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl23_delay_ck,
	pl23_t_kick,
	pl23_upper,
	pl23_sp_unison,
	pl23_sp_gattai,
	pl23_sp_assist,
	pl23_sp_zero,
} ;

static Uint8 otoko_ck( PLAYER_WORK *pl_w );
static Uint8 shinkuu_ck( PLAYER_WORK *pl_w );
static Uint8 rekka_ck( PLAYER_WORK *pl_w );
static Uint8 buraiken_ck( PLAYER_WORK *pl_w );
static Uint8 kouryuu_ck( PLAYER_WORK *pl_w );
static Uint8 gadou_ck( PLAYER_WORK *pl_w );
static Uint8 danku_ck( PLAYER_WORK *pl_w );
static Uint8 jdanku_ck( PLAYER_WORK *pl_w );
static Uint8 densetsu_ck( PLAYER_WORK *pl_w );
static Uint8 zenten_ck( PLAYER_WORK *pl_w );
static Uint8 kouten_ck( PLAYER_WORK *pl_w );
static Uint8 sign_ck( PLAYER_WORK *pl_w );
static Uint8 shinkuu_delay_ck( PLAYER_WORK *pl_w );
static Uint8 rekka_delay_ck( PLAYER_WORK *pl_w );
static Uint8 buraiken_delay_ck( PLAYER_WORK *pl_w );
static Uint8 otoko_delay_ck( PLAYER_WORK *pl_w );
static Uint8 densetsu_delay_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl23_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl23_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl23_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( otoko_ck( pl_w ) ) return ;
	if( shinkuu_ck( pl_w ) ) return ;
	if( rekka_ck( pl_w ) ) return ;
	if( buraiken_ck( pl_w ) ) return ;
	if( kouryuu_ck( pl_w ) ) return ;
	if( densetsu_ck( pl_w ) ) return ;
	if( zenten_ck( pl_w ) ) return ;
	if( kouten_ck( pl_w ) ) return ;
	if( gadou_ck( pl_w ) ) return ;
	if( danku_ck( pl_w ) ) return ;
	if( jdanku_ck( pl_w ) ) return ;
	if( sign_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( pl_chouhatsu_ck( pl_w, 8 ) ){
		pl_w->pl_sp_type = pl23_sp_chouhatsu ;
		pl_w->routine_1 = PL_NORM ;
		pl_w->routine_2 = pl_w->routine_3 = 0 ;
		act_st_set( pl_w, N_SPECIAL ) ;
		return ;
	}
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
/*			A level 3 hyper combo								*/
/*==============================================================*/
static Uint8 otoko_ck( PLAYER_WORK *pl_w )
{
	if( !com_seq_ck( pl_w, com_otoko, &pl_w->com_w8 ) ) return 0 ;
	if( *pl_w->gauge_level < 3 ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w8 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl23_sp_otoko ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 shinkuu_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_shinkuu, &pl_w->com_w2 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl23_sp_shinkuu ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 rekka_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_rekka, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl23_sp_rekka ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 buraiken_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_buraiken, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl23_sp_buraiken ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 kouryuu_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_kouryuu, &pl_w->com_w0 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl23_sp_kouryuu ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 gadou_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_gadou, &pl_w->com_w ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl23_sp_gadou ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 danku_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_danku, &pl_w->com_w1 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl23_sp_danku ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			In the air, once a jump								*/
/*==============================================================*/
static Uint8 jdanku_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_jdanku, &pl_w->com_w10 ) ) return 0 ;
	if( pl_w->jump_com_cnt ) return 0 ;
	pl_w->jump_com_cnt ++ ;
	com_sw_set( pl_w, &pl_w->com_w10 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl23_sp_jdanku ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 densetsu_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_densetsu, &pl_w->com_w7 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w7 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl23_sp_densetsu ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 zenten_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_zenten, &pl_w->com_w5 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w5 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl23_sp_zenten ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 kouten_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_kouten, &pl_w->com_w6 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w6 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl23_sp_kouten ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 sign_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_sign, &pl_w->com_w9 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w9 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl23_sp_sign ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl23_delay_ck( PLAYER_WORK *pl_w )
{
	if( shinkuu_delay_ck( pl_w ) || rekka_delay_ck( pl_w ) || buraiken_delay_ck( pl_w )
	 || otoko_delay_ck( pl_w ) || densetsu_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static Uint8 shinkuu_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_shinkuu, &pl_w->com_w2 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl23_sp_shinkuu ;
	return 1 ;
}

/*==============================================================*/
static Uint8 rekka_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_rekka, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl23_sp_rekka ;
	return 1 ;
}

/*==============================================================*/
static Uint8 buraiken_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_buraiken, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl23_sp_buraiken ;
	return 1 ;
}

/*==============================================================*/
static Uint8 otoko_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_seq_ck( pl_w, com_otoko, &pl_w->com_w8 ) ) return 0 ;
	if( *pl_w->gauge_level < 3 ) return 0 ;
	pl_w->delay_sp_type = pl23_sp_otoko ;
	return 1 ;
}

/*==============================================================*/
static Uint8 densetsu_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_densetsu, &pl_w->com_w7 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl23_sp_densetsu ;
	return 1 ;
}

/*==============================================================*/
static int snapback_ck( PLAYER_WORK *pl_w )
{
	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl23_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/****************************************************************/
void pl23_main( PLAYER_WORK *pl_w )
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
void pl23_atck_set( PLAYER_WORK *pl_w )
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
void pl23_atck( PLAYER_WORK *pl_w )
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
	case 2:
	case 0:
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
static void crouch_punch_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 2:
	case 0:
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
/*			The light and medium kicks can hit again, as the	*/
/*			animation says										*/
/*==============================================================*/
static void stand_kick_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 2:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	case 0:
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		else if( pl_w->cg_flag1 ){
			pl_w->atck_no = pl_w->cg_flag1 ;
			HIT_RESET( pl_w ) ;
			pl_w->cg_flag1 = 0 ;
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
/*			The crouching heavy kick launcher					*/
/****************************************************************/
void pl23_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_char_set( pl_w, Char_Extra, 2 ) ;
		pl_w->atck_no = pl23_at_upper ;
		HIT_RESET( pl_w ) ;
		pl_gauge_add( pl_w, 5 ) ;
		pl_se_req( pl_w, SE_SWING_H ) ;
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
static void dash_end( PLAYER_WORK *pl_w );

/****************************************************************/
void pl23_dash( PLAYER_WORK *pl_w )
{
	static void ( *dash_jmp[])( PLAYER_WORK *pl_w ) = {
		dash_start, dash_move, dash_stop, dash_end,
	} ;

	dash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dash_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->timer0 = 20 ;
		if( pl_w->pl_dir ){
			pl_w->spd.x = DOT_X( 6.5f ) ;
			pl_w->acc.x = -DOT_X( 0.125f ) ;
		} else {
			pl_w->spd.x = -DOT_X( 6.5f ) ;
			pl_w->acc.x = DOT_X( 0.125f ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void dash_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->timer0 -- ;
	if( pl_w->timer0 <= 0 ) pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void dash_stop( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = pl_w->pl_dir ? 2.5f : -2.5f ;
		pl_w->acc.x = pl_w->pl_dir ? -DOT_X( 0.125f ) : DOT_X( 0.125f ) ;
		pl_char_set( pl_w, Char_Dash, 2 ) ;
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

static void noki_start( PLAYER_WORK *pl_w );
static void noki_move( PLAYER_WORK *pl_w );
static void noki_stop( PLAYER_WORK *pl_w );
static void noki_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash										*/
/****************************************************************/
void pl23_noki( PLAYER_WORK *pl_w )
{
	static void ( *noki_jmp[])( PLAYER_WORK *pl_w ) = {
		noki_start, noki_move, noki_stop, noki_end,
	} ;

	noki_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void noki_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 16 ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? -12.5f : 12.5f ;
		pl_w->acc.x = pl_w->pl_dir ? 0.3125f : -0.3125f ;
		noki_move( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void noki_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	pl_w->timer0 -- ;
	if( pl_w->timer0 <= 0 ) pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void noki_stop( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = pl_w->pl_dir ? -2.5f : 2.5f ;
		pl_w->acc.x = pl_w->pl_dir ? DOT_X( 0.125f ) : -DOT_X( 0.125f ) ;
		pl_char_set( pl_w, Char_Dash, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void noki_end( PLAYER_WORK *pl_w )
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
static void appear_main( PLAYER_WORK *pl_w );
static void appear_walk( PLAYER_WORK *pl_w );
static void appear_walk_init( PLAYER_WORK *pl_w );
static void appear_walk_move( PLAYER_WORK *pl_w );
static void appear_walk_stop( PLAYER_WORK *pl_w );
static void appear_walk_pose( PLAYER_WORK *pl_w );
static void appear_drop( PLAYER_WORK *pl_w );
static void appear_drop_init( PLAYER_WORK *pl_w );
static void appear_drop_fall( PLAYER_WORK *pl_w );
static void appear_drop_land( PLAYER_WORK *pl_w );
static void appear_drop_down( PLAYER_WORK *pl_w );
static void appear_drop_getup( PLAYER_WORK *pl_w );

/****************************************************************/
void pl23_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_init, appear_main,
	} ;

	appear_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
/*			The second entrance when the button is held			*/
/*==============================================================*/
static void appear_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_w->sub_type = 0 ;
	if( !pl_w->cpu_flag ){
		if( sw_work[pl_w->pl_no].now & KEY_LP ) pl_w->sub_type = 1 ;
	}
}

/*--------------------------------------------------------------*/
static void appear_main( PLAYER_WORK *pl_w )
{
	if( pl_w->sub_type ) appear_drop( pl_w ) ;
	else appear_walk( pl_w ) ;
}

/*==============================================================*/
/*			Walks in											*/
/*==============================================================*/
static void appear_walk( PLAYER_WORK *pl_w )
{
	static void ( *appear_walk_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_walk_init, appear_walk_move, appear_walk_stop, appear_walk_pose,
	} ;

	appear_walk_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void appear_walk_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->disp_flag = 1 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_char_set( pl_w, Char_Appear, 0 ) ;
	if( pl_w->disp_dir ){
		pl_w->spd.x = DOT_X( 2.0f ) ;
		pl_w->pos.x += -DOT_X( 64.0f ) ;
	} else {
		pl_w->spd.x = -DOT_X( 2.0f ) ;
		pl_w->pos.x += DOT_X( 64.0f ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_walk_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void appear_walk_stop( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 60 ;
		pl_char_set( pl_w, Char_Appear, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_walk_pose( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->timer0 -- ;
	if( pl_w->timer0 <= 0 ){
		pl_w->routine_1 ++ ;
		pl_w->routine_2 = 0 ;
	}
}

/*==============================================================*/
/*			Falls in from above, crashes and gets up			*/
/*==============================================================*/
static void appear_drop( PLAYER_WORK *pl_w )
{
	static void ( *appear_drop_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_drop_init, appear_drop_fall, appear_drop_land, appear_drop_down, appear_drop_getup,
	} ;

	appear_drop_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			Group 23 holds the effects' animations				*/
/*			(Char_Effect)										*/
/*==============================================================*/
static void appear_drop_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->disp_flag = 1 ;
	if( pl_w->disp_dir ){
		pl_w->spd.x = 10.0f ;
		pl_w->acc.x = -0.104166664f ;
		pl_w->pos.x += -DOT_X( 76.0f ) ;
	} else {
		pl_w->spd.x = -10.0f ;
		pl_w->acc.x = 0.104166664f ;
		pl_w->pos.x += DOT_X( 76.0f ) ;
	}
	pl_w->spd.y = -DOT_Y( 4.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->pos.y += DOT_Y( 240.0f ) ;
	pl_char_set( pl_w, 23, 2 ) ;
}

/*--------------------------------------------------------------*/
static void appear_drop_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->ground_y > pl_w->pos.y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		if( pl_w->disp_dir ) pl_w->pos.x += -DOT_X( 64.0f ) ;
		else pl_w->pos.x += DOT_X( 64.0f ) ;
		scr_work.quake_level = 1 ;
		scr_work.quake_flag = 1 ;
		pl_se_req( pl_w, 73 ) ;
		pls23_00_set( pl_w, 1, 0 ) ;
		pls23_00_set( pl_w, 1, 1 ) ;
		scr_work.quake_level = 3 ;
		scr_work.quake_flag = 1 ;
		pl_char_set( pl_w, Char_Damage, 34 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_drop_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 120 ;
		pl_char_set( pl_w, Char_Damage, 31 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_drop_down( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->timer0 -- ;
	if( pl_w->timer0 <= 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Get_Up, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_drop_getup( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_1 ++ ;
		pl_w->routine_2 = 0 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
}

static void win_init( PLAYER_WORK *pl_w );
static void win_ko_init( PLAYER_WORK *pl_w );
static void win_time_init( PLAYER_WORK *pl_w );
static void lose_init( PLAYER_WORK *pl_w );
static void lose_ko_init( PLAYER_WORK *pl_w );
static void win_move( PLAYER_WORK *pl_w );
static void win_ko_move( PLAYER_WORK *pl_w );
static void lose_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl23_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_init, win_move,
	} ;

	win_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void win_init( PLAYER_WORK *pl_w )
{
	static void ( *win_init_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko_init, lose_ko_init, win_time_init, lose_init, lose_ko_init,
	} ;

	pl_w->routine_2 ++ ;
	win_init_jmp[pl_w->type]( pl_w ) ;
}

/*==============================================================*/
/*			Won by KO: one of two poses							*/
/*==============================================================*/
static void win_ko_init( PLAYER_WORK *pl_w )
{
	Uint8 no ;

	no = nlRand() ;
	no &= 1 ;
	pl_char_set( pl_w, Char_Win, no ) ;
}

/*==============================================================*/
static void win_time_init( PLAYER_WORK *pl_w )
{
	pl_char_set( pl_w, Char_Win, CH23_WIN_TIME ) ;
}

/*==============================================================*/
static void lose_init( PLAYER_WORK *pl_w )
{
	pl_char_set( pl_w, Char_Win, CH23_LOSE ) ;
}

/*==============================================================*/
/*			The same pose as lose_init							*/
/*==============================================================*/
static void lose_ko_init( PLAYER_WORK *pl_w )
{
	pl_char_set( pl_w, Char_Win, CH23_LOSE ) ;
}

/*--------------------------------------------------------------*/
static void win_move( PLAYER_WORK *pl_w )
{
	static void ( *win_move_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko_move, lose_move, lose_move, lose_move, lose_move,
	} ;

	if( pl_win_end_ck( pl_w ) ) pl_stand_ret( pl_w ) ;
	else win_move_jmp[pl_w->type]( pl_w ) ;
}

/*==============================================================*/
static void win_ko_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ) pl_w->cg_type = 0 ;
}

/*==============================================================*/
static void lose_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_move( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A kick that leaps off the wall						*/
/****************************************************************/
void pl23_t_kick( PLAYER_WORK *pl_w )
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
	pl_w->atck_no = pl23_at_t_kick ;
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

static void sp_gadou( PLAYER_WORK *pl_w );
static void sp_kouryuu( PLAYER_WORK *pl_w );
static void sp_danku( PLAYER_WORK *pl_w );
static void sp_shinkuu( PLAYER_WORK *pl_w );
static void sp_rekka( PLAYER_WORK *pl_w );
static void sp_buraiken( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_zenten( PLAYER_WORK *pl_w );
static void sp_kouten( PLAYER_WORK *pl_w );
static void sp_densetsu( PLAYER_WORK *pl_w );
static void sp_jburaiken( PLAYER_WORK *pl_w );
static void sp_jdanku( PLAYER_WORK *pl_w );
static void sp_sign( PLAYER_WORK *pl_w );
static void sp_otoko( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Special moves and hyper combos						*/
/****************************************************************/
void pl23_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *pl_sp_tbl[])( PLAYER_WORK *pl_w ) = {
		sp_gadou, sp_kouryuu, sp_danku, sp_shinkuu, sp_rekka, sp_buraiken, sp_gadou, sp_gadou,
		sp_chouhatsu, sp_zenten, sp_kouten, sp_densetsu, sp_jburaiken, sp_jdanku, sp_sign, sp_gadou,
		sp_otoko, sp_snapback,
	} ;

	pl_sp_tbl[pl_w->pl_sp_type]( pl_w ) ;
}

static void gadou_init( PLAYER_WORK *pl_w );
static void gadou_shot( PLAYER_WORK *pl_w );
static void gadou_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A shot along the ground								*/
/*==============================================================*/
static void sp_gadou( PLAYER_WORK *pl_w )
{
	static void ( *gadou_jmp[])( PLAYER_WORK *pl_w ) = {
		gadou_init, gadou_shot, gadou_end,
	} ;

	gadou_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void gadou_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_sp_ef_set( pl_w ) ;
	pl_gauge_add( pl_w, 8 ) ;
	if( pl_w->team_type == 3 ){
		pl_w->atck_no = pl23_at_gadou_h ;
		HIT_RESET( pl_w ) ;
	} else {
		pl_w->atck_no = pl_w->atck_level + 48 ;
		HIT_RESET( pl_w ) ;
	}
	pl_char_set( pl_w, Char_Special, 0 ) ;
}

/*--------------------------------------------------------------*/
static void gadou_shot( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		shl23_00_set( pl_w, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void gadou_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/* the uppercut's animations: for each strength, then the same with the other arm */
static const Uint8 kouryuu_char[4] = { 3, 5, 37, 39 } ;

static void kouryuu_init( PLAYER_WORK *pl_w );
static void kouryuu_start( PLAYER_WORK *pl_w );
static void kouryuu_gravity( PLAYER_WORK *pl_w );
static void kouryuu_jump( PLAYER_WORK *pl_w );
static void kouryuu_air( PLAYER_WORK *pl_w );
static void kouryuu_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The rising uppercut									*/
/*==============================================================*/
static void sp_kouryuu( PLAYER_WORK *pl_w )
{
	static void ( *kouryuu_jmp[])( PLAYER_WORK *pl_w ) = {
		kouryuu_init, kouryuu_start, kouryuu_jump, kouryuu_air, kouryuu_land,
	} ;

	kouryuu_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void kouryuu_init( PLAYER_WORK *pl_w )
{
	PL23_LOCAL_W *lw = ( PL23_LOCAL_W * )pl_w->tlocal ;
	Float acc = 0.5208333f ;

	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_gauge_add( pl_w, 13 ) ;
	pl_sp_ef_set( pl_w ) ;
	if( pl_w->atck_level == 0 ){
		pl_w->atck_no = pl23_at_kouryuu_l ;
		pl_w->hit_flag = pl_w->atck_attr = 0 ;
		pl_w->unk_1c4 = 0 ;
		vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
		pl_w->spd.x = -DOT_X( 2.0f ) ;
		pl_w->acc.x = acc ;
		pl_w->spd.y = DOT_Y( 8.0f ) ;
	} else {
		if( pl_w->team_type == 3 ){
			pl_w->atck_no = pl23_at_kouryuu_counter ;
			pl_w->hit_flag = pl_w->atck_attr = 0 ;
			pl_w->unk_1c4 = 0 ;
			vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
		} else {
			pl_w->atck_no = pl23_at_kouryuu_h ;
			pl_w->hit_flag = pl_w->atck_attr = 0 ;
			pl_w->unk_1c4 = 0 ;
			vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
		}
		pl_w->spd.x = -10.0f ;
		pl_w->acc.x = acc ;
		pl_w->spd.y = DOT_Y( 16.0f ) ;
	}
	pl_w->acc.y = -DOT_Y( 0.5f ) ;
	if( pl_w->disp_dir ){
		pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = -pl_w->acc.x ;
	}
	pl_char_set( pl_w, Char_Special, kouryuu_char[pl_w->atck_level + 2] ) ;
	lw->kouryuu_cnt ++ ;
	lw->kouryuu_cnt &= 3 ;
	if( lw->kouryuu_cnt ) pl_char_set( pl_w, Char_Special, kouryuu_char[pl_w->atck_level] ) ;
	else {
		pl_char_set( pl_w, Char_Special, kouryuu_char[pl_w->atck_level + 2] ) ;
		pl_color_set( pl_w, 8 ) ;
	}
}

/*--------------------------------------------------------------*/
static void kouryuu_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_air_set( pl_w ) ;
	}
}

/*==============================================================*/
/*			Heavier once past the top of the jump				*/
/*==============================================================*/
static void kouryuu_gravity( PLAYER_WORK *pl_w )
{
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->spd.y * pl_w->acc.y > 0.0f ) pl_w->acc.y = -DOT_Y( 0.75f ) ;
}

/*--------------------------------------------------------------*/
static void kouryuu_jump( PLAYER_WORK *pl_w )
{
	kouryuu_gravity( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_w->spd.x * pl_w->acc.x > 0.0f ) pl_w->routine_2 ++ ;
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void kouryuu_air( PLAYER_WORK *pl_w )
{
	kouryuu_gravity( pl_w ) ;
	if( pl_w->ground_y > pl_w->pos.y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_land_ef_set( pl_w ) ;
	} else if( !pl_w->cg_type ){
		pl_char_move( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void kouryuu_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

/* the flying kick's speeds for each strength: spd.x, acc.x, spd.y, acc.y */
static const Float danku_spd[2][4] = {
	{ -DOT_X( 4.0f ), 0.0f, DOT_Y( 6.0f ), -DOT_Y( 0.5f ) },
	{ -DOT_X( 5.0f ), 0.0f, 7.5f, -0.3348214f },
} ;

static void danku_init( PLAYER_WORK *pl_w );
static void danku_start( PLAYER_WORK *pl_w );
static void danku_jump( PLAYER_WORK *pl_w );
static void danku_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The flying kick										*/
/*==============================================================*/
static void sp_danku( PLAYER_WORK *pl_w )
{
	static void ( *danku_jmp[])( PLAYER_WORK *pl_w ) = {
		danku_init, danku_start, danku_jump, danku_land,
	} ;

	danku_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void danku_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_gauge_add( pl_w, 13 ) ;
	pl_char_set( pl_w, Char_Special, !pl_w->atck_level ? 6 : 8 ) ;
}

/*--------------------------------------------------------------*/
static void danku_start( PLAYER_WORK *pl_w )
{
	const Float *spd ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type >= 0 ){
		pl_w->routine_2 ++ ;
		pl_air_set( pl_w ) ;
		spd = danku_spd[pl_w->atck_level] ;
		pl_w->spd.x = *spd++ ;
		pl_w->acc.x = *spd++ ;
		pl_w->spd.y = *spd++ ;
		pl_w->acc.y = *spd ;
		if( pl_w->disp_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
	}
}

/*--------------------------------------------------------------*/
static void danku_jump( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_flag1 ){
		if( pl_w->team_type == 3 ){
			pl_w->atck_no = pl_w->cg_flag1 + 28 ;
			HIT_RESET( pl_w ) ;
		} else {
			pl_w->atck_no = pl_w->cg_flag1 ;
			HIT_RESET( pl_w ) ;
		}
		pl_w->cg_flag1 = 0 ;
	}
	if( pl_w->ground_y > pl_w->pos.y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_char_set( pl_w, Char_Special, !pl_w->atck_level ? 40 : 42 ) ;
		pl_land_ef_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void danku_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void jdanku_init( PLAYER_WORK *pl_w );
static void jdanku_start( PLAYER_WORK *pl_w );
static void jdanku_jump( PLAYER_WORK *pl_w );
static void jdanku_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The flying kick from a jump							*/
/*==============================================================*/
static void sp_jdanku( PLAYER_WORK *pl_w )
{
	static void ( *jdanku_jmp[])( PLAYER_WORK *pl_w ) = {
		jdanku_init, jdanku_start, jdanku_jump, jdanku_land,
	} ;

	jdanku_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void jdanku_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_gauge_add( pl_w, 13 ) ;
	if( !pl_w->atck_level ) pl_char_set( pl_w, Char_Special, 44 ) ;
	else pl_char_set( pl_w, Char_Special, 45 ) ;
	pl_w->pl_stance = STANCE_AIR ;
}

/*--------------------------------------------------------------*/
static void jdanku_start( PLAYER_WORK *pl_w )
{
	const Float *spd ;
	Sint8 rev ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type >= 0 ){
		pl_w->routine_2 ++ ;
		spd = danku_spd[pl_w->atck_level] ;
		if( pl_w->jump_dir < 0 ){
			if( pl_w->disp_dir ) rev = 1 ;
			else rev = 0 ;
		} else if( pl_w->spd.x > 0.0f ) rev = 1 ;
		else rev = 0 ;
		pl_w->spd.x = *spd++ ;
		pl_w->acc.x = *spd++ ;
		pl_w->spd.y = *spd++ ;
		pl_w->acc.y = *spd ;
		if( rev ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
	}
}

/*--------------------------------------------------------------*/
static void jdanku_jump( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->ground_y > pl_w->pos.y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_char_set( pl_w, Char_Special, !pl_w->atck_level ? 40 : 42 ) ;
		pl_land_ef_set( pl_w ) ;
	} else if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
	else if( pl_w->cg_flag1 ){
		pl_w->atck_no = pl_w->cg_flag1 ;
		HIT_RESET( pl_w ) ;
		pl_w->cg_flag1 = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void jdanku_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void shinkuu_init( PLAYER_WORK *pl_w );
static void shinkuu_wait( PLAYER_WORK *pl_w );
static void shinkuu_shot( PLAYER_WORK *pl_w );
static void shinkuu_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo that sends out an effect				*/
/*==============================================================*/
static void sp_shinkuu( PLAYER_WORK *pl_w )
{
	static void ( *shinkuu_jmp[])( PLAYER_WORK *pl_w ) = {
		shinkuu_init, shinkuu_wait, shinkuu_shot, shinkuu_end,
	} ;

	shinkuu_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void shinkuu_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_color_set( pl_w, 0 ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = pl23_at_shinkuu ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 0 ) ;
	pl_w->cg_ctr += 8 ;
}

/*--------------------------------------------------------------*/
static void shinkuu_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->cg_type = 0 ;
		ofs.x = DOT_X( 32.0f ) ;
		ofs.y = DOT_Y( 88.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shinkuu_shot( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		shl23_00_set( pl_w, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shinkuu_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_stand_ret( pl_w ) ;
	}
}

static void rekka_init( PLAYER_WORK *pl_w );
static void rekka_wait( PLAYER_WORK *pl_w );
static void rekka_move( PLAYER_WORK *pl_w );
static void rekka_up( PLAYER_WORK *pl_w );
static void rekka_air( PLAYER_WORK *pl_w );
static void rekka_fall( PLAYER_WORK *pl_w );
static void rekka_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo that rushes in and leaps up			*/
/*==============================================================*/
static void sp_rekka( PLAYER_WORK *pl_w )
{
	static void ( *rekka_jmp[])( PLAYER_WORK *pl_w ) = {
		rekka_init, rekka_wait, rekka_move, rekka_up, rekka_air, rekka_fall, rekka_land,
	} ;

	rekka_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void rekka_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_color_set( pl_w, 0 ) ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.x = pl_w->disp_dir ? 1.6666666f : -1.6666666f ;
	pl_w->atck_no = pl23_at_rekka ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 1 ) ;
}

/*--------------------------------------------------------------*/
static void rekka_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type < 0 ){
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		ofs.x = -DOT_X( 16.0f ) ;
		ofs.y = DOT_Y( 48.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void rekka_move( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->atck_no = pl23_at_rekka_2 ;
		HIT_RESET( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void rekka_up( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 16.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.5f ) ;
		pl_air_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void rekka_air( PLAYER_WORK *pl_w )
{
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->spd.y * pl_w->acc.y > 0.0f ){
		pl_w->routine_2 ++ ;
		pl_w->acc.y = -2.142857f ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void rekka_fall( PLAYER_WORK *pl_w )
{
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->ground_y > pl_w->pos.y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_land_ef_set( pl_w ) ;
	} else if( !pl_w->cg_type ) pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void rekka_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void buraiken_init( PLAYER_WORK *pl_w );
static void buraiken_wait( PLAYER_WORK *pl_w );
static void buraiken_move( PLAYER_WORK *pl_w );
static void buraiken_up( PLAYER_WORK *pl_w );
static void buraiken_jump( PLAYER_WORK *pl_w );
static void buraiken_air( PLAYER_WORK *pl_w );
static void buraiken_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo that rushes in, holds the opponent	*/
/*			and kicks up										*/
/*==============================================================*/
static void sp_buraiken( PLAYER_WORK *pl_w )
{
	static void ( *buraiken_jmp[])( PLAYER_WORK *pl_w ) = {
		buraiken_init, buraiken_wait, buraiken_move, buraiken_up, buraiken_jump, buraiken_air, buraiken_land,
	} ;

	buraiken_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void buraiken_init( PLAYER_WORK *pl_w )
{
	PL23_LOCAL_W *lw = ( PL23_LOCAL_W * )pl_w->tlocal ;

	pl_w->routine_2 ++ ;
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_color_set( pl_w, 0 ) ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	lw->buraiken_hit = 0 ;
	pl_w->atck_no = pl23_at_buraiken ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 2 ) ;
	pl_w->cg_ctr += 8 ;
}

/*--------------------------------------------------------------*/
static void buraiken_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = -DOT_X( 8.0f ) ;
		ofs.y = DOT_Y( 74.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*==============================================================*/
/*			On the first hit, puts the work at target_w on		*/
/*			the ground beside Dan								*/
/*==============================================================*/
static void buraiken_move( PLAYER_WORK *pl_w )
{
	PL23_LOCAL_W *lw = ( PL23_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *em ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( lw->buraiken_hit ) goto move ;
	if( !pl_w->hit_flag ) goto move ;
	em = pl_w->em_w ;
	if( em->work_kind || em->team_no ) goto move ;
	if( ABYSS_CK( em ) ) goto move ;
	lw->buraiken_hit = 1 ;
	pl_w->target_w->pl_stance = STANCE_STAND ;
	pl_w->target_w->pos.y = pl_w->ground_y ;
	pl_w->target_w->pos.x = pl_w->pos.x ;
	if( pl_w->disp_dir ) pl_w->target_w->pos.x -= -DOT_X( 64.0f ) ;
	else pl_w->target_w->pos.x -= DOT_X( 64.0f ) ;
move:
	if( pl_char_move( pl_w ) >= 0 ){
		if( pl_w->cg_flag1 ){
			pl_w->atck_no = pl_w->cg_flag1 ;
			HIT_RESET( pl_w ) ;
			pl_w->cg_flag1 = 0 ;
		}
	} else {
		pl_w->routine_2 ++ ;
		pl_w->spd.x = -DOT_X( 10.0f ) ;
		pl_w->acc.x = 0.7291666f ;
		pl_w->spd.y = DOT_Y( 16.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.5f ) ;
		if( pl_w->disp_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		pl_w->atck_no = pl23_at_buraiken_last ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void buraiken_up( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_air_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void buraiken_jump( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	kouryuu_gravity( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_w->spd.x * pl_w->acc.x > 0.0f ) pl_w->routine_2 ++ ;
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void buraiken_air( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	kouryuu_gravity( pl_w ) ;
	if( pl_w->ground_y > pl_w->pos.y ){
		pl_w->routine_2 ++ ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_land_ef_set( pl_w ) ;
	} else if( !pl_w->cg_type ) pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void buraiken_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

/* the taunt's pose and sound for each stance */
static const Uint8 chouhatsu_tbl[3][2] = {
	{ 14, 24 }, { 15, 24 }, { 16, 8 },
} ;

static void chouhatsu_init( PLAYER_WORK *pl_w );
static void chouhatsu_move( PLAYER_WORK *pl_w );
static void chouhatsu_air( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The taunt											*/
/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w )
{
	static void ( *chouhatsu_jmp[])( PLAYER_WORK *pl_w ) = {
		chouhatsu_init, chouhatsu_move, chouhatsu_air,
	} ;

	chouhatsu_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void chouhatsu_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->timer0 = 72 ;
	pl_dir_set( pl_w ) ;
	pl_gauge_add( pl_w, chouhatsu_tbl[pl_w->pl_stance][1] ) ;
	pl_char_set( pl_w, Char_Special, chouhatsu_tbl[pl_w->pl_stance][0] ) ;
	if( pl_w->pl_stance == STANCE_AIR ) pl_w->routine_2 ++ ;
	else {
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
}

/*--------------------------------------------------------------*/
static void chouhatsu_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->timer0 -- ;
	if( pl_w->timer0 <= 0 ) pl_stand_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void chouhatsu_air( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	pl_w->timer0 -- ;
	if( pl_w->timer0 <= 0 || pl_land_ck( pl_w ) ) pl_jump_ret( pl_w ) ;
}

static void zenten_init( PLAYER_WORK *pl_w );
static void roll_wait( PLAYER_WORK *pl_w );
static void roll_move( PLAYER_WORK *pl_w );
static void roll_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The rolling taunt forwards							*/
/*==============================================================*/
static void sp_zenten( PLAYER_WORK *pl_w )
{
	static void ( *zenten_jmp[])( PLAYER_WORK *pl_w ) = {
		zenten_init, roll_wait, roll_move, roll_end,
	} ;

	zenten_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void zenten_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_gauge_add( pl_w, 32 ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.x = pl_w->disp_dir ? DOT_X( 2.0f ) : -DOT_X( 2.0f ) ;
	pl_char_set( pl_w, Char_Special, 17 ) ;
}

/*--------------------------------------------------------------*/
static void roll_wait( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void roll_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->timer0 = 60 ;
	}
}

/*--------------------------------------------------------------*/
static void roll_end( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->timer0 -- ;
	if( pl_w->timer0 <= 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void kouten_init( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The rolling taunt backwards							*/
/*==============================================================*/
static void sp_kouten( PLAYER_WORK *pl_w )
{
	static void ( *kouten_jmp[])( PLAYER_WORK *pl_w ) = {
		kouten_init, roll_wait, roll_move, roll_end,
	} ;

	kouten_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void kouten_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_gauge_add( pl_w, 32 ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.x = pl_w->disp_dir ? -DOT_X( 2.0f ) : DOT_X( 2.0f ) ;
	pl_char_set( pl_w, Char_Special, 18 ) ;
}

static void densetsu_init( PLAYER_WORK *pl_w );
static void densetsu_wait( PLAYER_WORK *pl_w );
static void densetsu_roll_init( PLAYER_WORK *pl_w );
static void densetsu_roll_move( PLAYER_WORK *pl_w );
static void densetsu_crouch_init( PLAYER_WORK *pl_w );
static void densetsu_crouch_move( PLAYER_WORK *pl_w );
static void densetsu_pose_init( PLAYER_WORK *pl_w );
static void densetsu_pose_move( PLAYER_WORK *pl_w );
static void densetsu_sign_init( PLAYER_WORK *pl_w );
static void densetsu_sign_keep( PLAYER_WORK *pl_w );
static void densetsu_sign_end( PLAYER_WORK *pl_w );
static void densetsu_jump_init( PLAYER_WORK *pl_w );
static void densetsu_jump( PLAYER_WORK *pl_w );
static void densetsu_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The hyper taunt: a run of taunts, rolls and a		*/
/*			jump												*/
/*==============================================================*/
static void sp_densetsu( PLAYER_WORK *pl_w )
{
	static void ( *densetsu_jmp[])( PLAYER_WORK *pl_w ) = {
		densetsu_init, densetsu_wait, densetsu_roll_init, densetsu_roll_move, densetsu_crouch_init, densetsu_crouch_move, densetsu_roll_init, densetsu_roll_move,
		densetsu_pose_init, densetsu_pose_move, densetsu_roll_init, densetsu_roll_move, densetsu_sign_init, densetsu_sign_keep, densetsu_sign_end, densetsu_jump_init,
		densetsu_jump, densetsu_land,
	} ;

	densetsu_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void densetsu_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->pl_stance = STANCE_STAND ;
	pl_color_set( pl_w, 0 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.x = pl_w->disp_dir ? DOT_X( 2.0f ) : -DOT_X( 2.0f ) ;
	pl_char_set( pl_w, Char_Special, 19 ) ;
}

/*--------------------------------------------------------------*/
static void densetsu_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		ofs.x = -DOT_X( 16.0f ) ;
		ofs.y = DOT_Y( 87.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void densetsu_roll_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_set( pl_w, Char_Special, 20 ) ;
}

/*--------------------------------------------------------------*/
static void densetsu_roll_move( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_char_move( pl_w ) < 0 ) pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void densetsu_crouch_init( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->routine_2 ++ ;
	pl_char_set( pl_w, Char_Special, 21 ) ;
}

/*--------------------------------------------------------------*/
static void densetsu_crouch_move( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ) pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void densetsu_pose_init( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->routine_2 ++ ;
	pl_char_set( pl_w, Char_Special, 24 ) ;
}

/*--------------------------------------------------------------*/
static void densetsu_pose_move( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ) pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void densetsu_sign_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_set( pl_w, Char_Special, 34 ) ;
	pl_w->timer0 = 60 ;
}

/*--------------------------------------------------------------*/
static void densetsu_sign_keep( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	pl_w->timer0 -- ;
	if( pl_w->timer0 <= 0 ){
		pl_w->routine_2 ++ ;
		if( pl_w->disp_dir ) pl_char_set( pl_w, Char_Special, 36 ) ;
		else pl_char_set( pl_w, Char_Special, 35 ) ;
	}
}

/*--------------------------------------------------------------*/
static void densetsu_sign_end( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ) pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void densetsu_jump_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->delay_flag = 0 ;
	pl_w->delay_timer = 0 ;
	pl_w->hyper_bg = 0 ;
	pl_w->hyper_bg_tm = 0 ;
	pl_w->spd.x = -6.25f ;
	pl_w->acc.x = 0.0390625f ;
	pl_w->spd.y = DOT_Y( 9.625f ) ;
	pl_w->acc.y = -0.9040178f ;
	if( pl_w->disp_dir ){
		pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = -pl_w->acc.x ;
	}
	pl_char_set( pl_w, Char_Special, 22 ) ;
}

/*--------------------------------------------------------------*/
static void densetsu_jump( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, 23 ) ;
	}
}

/*--------------------------------------------------------------*/
static void densetsu_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void jburaiken_init( PLAYER_WORK *pl_w );
static void jburaiken_wait( PLAYER_WORK *pl_w );
static void jburaiken_hit( PLAYER_WORK *pl_w );
static void jburaiken_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo with a jump; no command starts it		*/
/*==============================================================*/
static void sp_jburaiken( PLAYER_WORK *pl_w )
{
	static void ( *jburaiken_jmp[])( PLAYER_WORK *pl_w ) = {
		jburaiken_init, jburaiken_wait, jburaiken_hit, jburaiken_end,
	} ;

	jburaiken_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void jburaiken_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = pl23_at_jburaiken ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 4 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
}

/*--------------------------------------------------------------*/
static void jburaiken_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_air_set( pl_w ) ;
		ofs.x = -DOT_X( 16.0f ) ;
		ofs.y = DOT_Y( 48.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
		pl_w->timer0 = 10 ;
	}
}

/*--------------------------------------------------------------*/
static void jburaiken_hit( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_prm ){
		pl_w->atck_no = pl_w->cg_prm ;
		HIT_RESET( pl_w ) ;
		pl_w->cg_prm = 0 ;
		pl_w->timer0 -- ;
		if( pl_w->timer0 <= 0 ){
			pl_w->routine_2 ++ ;
			pl_w->atck_no = pl23_at_jburaiken_last ;
			HIT_RESET( pl_w ) ;
			pl_char_set( pl_w, Char_Super, 5 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void jburaiken_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_jump_ret( pl_w ) ;
	}
}

static void sign_init( PLAYER_WORK *pl_w );
static void sign_keep( PLAYER_WORK *pl_w );
static void sign_throw( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Premium Sign: throws a piece that falls and blinks	*/
/*==============================================================*/
static void sp_sign( PLAYER_WORK *pl_w )
{
	static void ( *sign_jmp[])( PLAYER_WORK *pl_w ) = {
		sign_init, sign_keep, sign_throw,
	} ;

	sign_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void sign_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_dir_set( pl_w ) ;
	pl_w->atck_no = pl23_at_sign ;
	HIT_RESET( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	if( !pl_w->atck_level ){
		pl_char_set( pl_w, Char_Special, 25 ) ;
		pl_w->timer0 = 10 ;
	} else {
		pl_char_set( pl_w, Char_Special, 27 ) ;
		pl_w->timer0 = 60 ;
	}
}

/*--------------------------------------------------------------*/
static void sign_keep( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->timer0 -- ;
	if( pl_w->timer0 <= 0 ){
		pl_w->routine_2 ++ ;
		if( pl_w->atck_level == 0 ){
			if( pl_w->disp_dir ) pl_char_set( pl_w, Char_Special, 31 ) ;
			else pl_char_set( pl_w, Char_Special, 28 ) ;
		} else {
			if( pl_w->disp_dir ) pl_char_set( pl_w, Char_Special, 33 ) ;
			else pl_char_set( pl_w, Char_Special, 30 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void sign_throw( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	else if( pl_w->cg_type < 0 ){
		pl_w->cg_type = 0 ;
		/* both arms are the same in the arcade */
		if( pl_w->atck_level != 0 ){
			pl_w->atck_no = pl23_at_sign_hit ;
			HIT_RESET( pl_w ) ;
		} else {
			pl_w->atck_no = pl23_at_sign_hit ;
			HIT_RESET( pl_w ) ;
		}
		shl23_00_set( pl_w, 2 ) ;
	}
}

static void otoko_init( PLAYER_WORK *pl_w );
static void otoko_wait( PLAYER_WORK *pl_w );
static void otoko_move( PLAYER_WORK *pl_w );
static void otoko_miss( PLAYER_WORK *pl_w );
static void otoko_reach( PLAYER_WORK *pl_w );
static void otoko_hold( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The level 3 hyper combo: a dash that grabs and		*/
/*			explodes											*/
/*==============================================================*/
static void sp_otoko( PLAYER_WORK *pl_w )
{
	static void ( *otoko_jmp[])( PLAYER_WORK *pl_w ) = {
		otoko_init, otoko_wait, otoko_move, otoko_miss,
	} ;

	otoko_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void otoko_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_color_set( pl_w, 0 ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 6 ) ;
}

/*--------------------------------------------------------------*/
static void otoko_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type < 0 ){
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->cg_type = 0 ;
		ofs.x = -5.0f ;
		ofs.y = DOT_Y( 69.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 3 ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->disp_dir ? DOT_X( 2.0f ) : -DOT_X( 2.0f ) ;
	}
}

/*--------------------------------------------------------------*/
static void otoko_move( PLAYER_WORK *pl_w )
{
	static void ( *otoko_move_jmp[])( PLAYER_WORK *pl_w ) = {
		otoko_reach, otoko_hold,
	} ;

	otoko_move_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void otoko_reach( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->spd.x = pl_w->disp_dir ? -0.104166664f : 0.104166664f ;
		pl_char_set( pl_w, Char_Super, 7 ) ;
	} else if( !pl_w->cg_type ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		if( ( em = pl_catch_ck( pl_w ) ) && pl_w->target_w->pl_stance != STANCE_AIR ){
			pl_w->routine_3 ++ ;
			pl_w->catch_no = 0xc3 ;
			pl_sp_catch_set( pl_w, em ) ;
			pl_char_set( pl_w, Char_Catch, 3 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void otoko_hold( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->catch_flag = 1 ;
	pl_w->push_ng_timer = 2 ;
	pl_w->catch_ng_timer = 2 ;
	if( !pl_w->routine_3 ){
		pl_w->routine_3 ++ ;
		pl_char_move( pl_w ) ;
		if( pl_w->cg_type & 1 ){
			pl_w->cg_type ^= 1 ;
			pls23_00_set( pl_w, 2, 0 ) ;
		}
		if( pl_w->cg_type & 2 ){
			pl_w->cg_type ^= 2 ;
			pl_color_set( pl_w, 8 ) ;
		}
	} else if( pl_char_move( pl_w ) < 0 ){
		pl_w->target_w->dm_hit_w = pl_w ;
		pl_w->target_w->catch_type = 1 ;
		pl_w->target_w->pl_dir = pl_w->pl_dir ;
		pl_w->target_w->atck_no = pl23_at_otoko ;
		scr_excep_clr() ;
		pl_w->target_w->pl_dir ^= 1 ;
		pl_w->atck_no = pl23_at_otoko_self ;
		pl_w->catch_flag = 0 ;
		dm_ko_self( pl_w ) ;
		pl_color_set( pl_w, 0 ) ;
	} else if( pl_w->cg_type & 2 ){
		pl_w->cg_type ^= 2 ;
		burst_set( pl_w, &pl_w->pos, 1, 0x108 ) ;
		scr_work.quake_level = 3 ;
		scr_work.quake_flag = 1 ;
	}
}

/*--------------------------------------------------------------*/
static void otoko_miss( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void snapback_init( PLAYER_WORK *pl_w );
static void snapback_move( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The move snapback_ck starts, for a level of the			*/
/*			hyper gauge											*/
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
	pl_w->atck_no = pl23_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 43 ) ;
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
		ofs.x = -DOT_X( 64.0f ) ;
		ofs.y = DOT_Y( 72.0f ) ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
}

static PLAYER_WORK *nml_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *crouch_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *jump_catch_ck( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The throw the player can do now, and the			*/
/*			opponent it catches									*/
/****************************************************************/
PLAYER_WORK *pl23_catch_ck( PLAYER_WORK *pl_w )
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
			pl_w->catch_no = 2 ;
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

static void nml_p_set( PLAYER_WORK *pl_w );
static void jump_p_set( PLAYER_WORK *pl_w );
static void nml_k_set( PLAYER_WORK *pl_w );
static void otoko_set( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Start the throw that pl23_catch_ck chose			*/
/****************************************************************/
void	pl23_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_set, jump_p_set, nml_k_set, otoko_set,
	} ;

	catch_gauge_add( pl_w ) ;
	catch_set_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->hit_stop = 10 ;
	if( ( pl_w->dir << 10 ) & 0x0800 ){
		pl_w->disp_dir ^= 1 ;
		pl_w->pl_dir ^= 1 ;
	}
	ofs.x = -DOT_X( 32.0f ) ;
	ofs.y = DOT_Y( 80.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 0 ) ;
}

/*==============================================================*/
static void jump_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->hit_stop = 10 ;
	if( ( pl_w->dir << 10 ) & 0x0800 ){
		pl_w->disp_dir ^= 1 ;
		pl_w->pl_dir ^= 1 ;
	}
	ofs.x = DOT_X( 32.0f ) ;
	ofs.y = DOT_Y( 80.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	catch_gauge_add( pl_w ) ;
	pl_w->spd.y = DOT_Y( 4.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	if( pl_w->sjump_flag ){
		pl_w->spd.x /= 2.0f ;
		pl_w->acc.x /= 2.0f ;
	}
	pl_char_set( pl_w, Char_Catch, 1 ) ;
}

/*==============================================================*/
static void nml_k_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->hit_stop = 10 ;
	if( !( ( pl_w->dir << 10 ) & 0x0800 ) ){
		pl_w->disp_dir ^= 1 ;
		pl_w->pl_dir ^= 1 ;
	}
	ofs.x = -80.0f ;
	ofs.y = DOT_Y( 80.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 2 ) ;
}

/*==============================================================*/
/*			The grab of the level 3 hyper combo					*/
/*==============================================================*/
static void otoko_set( PLAYER_WORK *pl_w )
{
	pl_char_set( pl_w, Char_Catch, 3 ) ;
}

static void nml_p_catch( PLAYER_WORK *pl_w );
static void jump_p_catch( PLAYER_WORK *pl_w );
static void nml_k_catch( PLAYER_WORK *pl_w );
static void otoko_catch( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void	pl23_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_catch, jump_p_catch, nml_k_catch, otoko_catch,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_catch( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->target_w->dm_hit_w = pl_w ;
		pl_w->target_w->catch_type = 1 ;
		pl_w->target_w->pl_dir = pl_w->pl_dir ;
		pl_w->target_w->atck_no = pl23_at_throw_p ;
		scr_excep_clr() ;
	}
}

/*==============================================================*/
static void jump_p_catch( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_3 ){
		if( pl_w->cg_type >= 0 ) pl_char_move( pl_w ) ;
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->ground_y > pl_w->pos.y ){
			pl_w->routine_3 ++ ;
			pl_w->pos.y = pl_w->ground_y ;
			pl_w->cg_type = 0 ;
			pl_land_ef_set( pl_w ) ;
		}
	} else if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	} else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->target_w->dm_hit_w = pl_w ;
		pl_w->target_w->catch_type = 11 ;
		pl_w->target_w->pl_dir = pl_w->pl_dir ;
		pl_w->target_w->atck_no = pl23_at_air_throw ;
		scr_excep_clr() ;
		catch_se_req( pl_w ) ;
	}
}

/*==============================================================*/
static void nml_k_catch( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->target_w->dm_hit_w = pl_w ;
		pl_w->target_w->catch_type = 1 ;
		pl_w->target_w->atck_no = pl23_at_throw_k ;
		scr_excep_clr() ;
		pl_w->target_w->pl_dir = pl_w->pl_dir ;
		pl_w->target_w->pl_dir ^= 1 ;
		ofs.x = -75.0f ;
		ofs.y = 180.0f ;
		ofs.z = 0.0f ;
		catch_hit_mark_set( pl_w, &ofs ) ;
		pl_se_req( pl_w, 1 ) ;
	}
}

/*==============================================================*/
/*			Otoko Michi runs its own grab						*/
/*==============================================================*/
static void otoko_catch( PLAYER_WORK *pl_w )
{
}

/****************************************************************/
/*			Holding the opponent								*/
/****************************************************************/
void	pl23_hold( PLAYER_WORK *pl_w )
{
	catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void	pl23_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl23_sp_shinkuu ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl23_sp_rekka ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl23_sp_buraiken ;
		break ;
	}
	pl_w->atck_level = 1 ;
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl23_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl23_sp_shinkuu ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl23_sp_rekka ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl23_sp_buraiken ;
		break ;
	}
	pl_w->atck_level = 1 ;
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl23_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl23_sp_gadou ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl23_sp_kouryuu ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl23_sp_sign ;
		break ;
	}
	pl_w->atck_level = 1 ;
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void	pl23_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl23_sp_gadou ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl23_sp_kouryuu ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl23_sp_danku ;
		break ;
	}
	pl_w->atck_level = 1 ;
	act_st_set( pl_w, N_SPECIAL ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl23.c" End of File	*/

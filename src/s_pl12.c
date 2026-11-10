/* Hayato (character 12): command checks, normal attacks, appearance, win and lose poses, special
   moves and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl12_def.h"

void pl12_main( PLAYER_WORK *pl_w );
void pl12_com_ck( PLAYER_WORK *pl_w );
void pl12_atck_set( PLAYER_WORK *pl_w );
void pl12_atck( PLAYER_WORK *pl_w );
void pl12_dash( PLAYER_WORK *pl_w );
void pl12_noki( PLAYER_WORK *pl_w );
void pl12_appear( PLAYER_WORK *pl_w );
void pl12_win( PLAYER_WORK *pl_w );
void pl12_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl12_catch_ck( PLAYER_WORK *pl_w );
void pl12_catch_set( PLAYER_WORK *pl_w );
void pl12_catch( PLAYER_WORK *pl_w );
void pl12_hold( PLAYER_WORK *pl_w );
int pl12_delay_ck( PLAYER_WORK *pl_w );
void pl12_t_kick( PLAYER_WORK *pl_w );
void pl12_upper( PLAYER_WORK *pl_w );
void pl12_sp_unison( PLAYER_WORK *pl_w );
void pl12_sp_gattai( PLAYER_WORK *pl_w );
void pl12_sp_assist( PLAYER_WORK *pl_w );
void pl12_sp_zero( PLAYER_WORK *pl_w );
int pl12_free_ck( PLAYER_WORK *pl_w );

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
static Sint8 ex_tbl_00[4] = { -1, -1, -1, -1 } ;
static Sint8 ex_tbl_01[4] = { -1, -1, -1, -1 } ;
static Sint8 ex_tbl_02[4] = {  3, -1, -1, -1 } ;
static Sint8 ex_tbl_03[4] = {  4,  6, -1, -1 } ;
static Sint8 ex_tbl_04[4] = {  5, -1, -1,  3 } ;
static Sint8 ex_tbl_05[4] = { -1, -1, -1, -1 } ;
static Sint8 ex_tbl_06[4] = { -1, -1, -1,  3 } ;
static Sint8 ex_tbl_07[4] = {  7, -1, -1, -1 } ;
static Sint8 ex_tbl_08[4] = {  8, -1, -1, -1 } ;
static Sint8 ex_tbl_09[4] = { -1, -1, -1, -1 } ;
static Sint8 ex_tbl_0a[4] = {  9, -1, -1, -1 } ;
static Sint8 ex_tbl_0b[4] = { -1, 10, -1, -1 } ;
static Sint8 ex_tbl_0c[4] = { -1, 11,  4, -1 } ;
static Sint8 ex_tbl_0d[4] = { -1, 12, -1, -1 } ;
static Sint8 ex_tbl_0e[4] = { -1, 12, -1, -1 } ;
static Sint8 ex_tbl_0f[4] = { -1, -1, -1, -1 } ;

/* commands: time or steps, kind, the special move, then the stick and buttons */
static Uint16 com_shiden[] = { 3, COM_GROUND, pl12_sp_shiden, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Shiden */
static Uint16 com_guren[] = { 3, 0, pl12_sp_guren, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_PUNCH } ;	/* Guren */
static Uint16 com_byakko[] = { 2, COM_GROUND | COM_ALL_SW, 0, 0, KEY_BACK, KEY_FORWARD, KEY_PUNCH } ;	/* Byakkou Hou */
static Uint16 com_engetsu[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl12_sp_engetsu, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Engetsu (hyper) */
static Uint16 com_rasetsu[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl12_sp_rasetsu, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Rasetsu Zan (hyper) */
static Uint16 com_field[] = { 3, COM_HYPER | COM_ALL_SW, pl12_sp_field, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Plasma Field (hyper) */
static Uint16 com_black[] = { 4, COM_GROUND | COM_HYPER, pl12_sp_black, 0, KEY_LP, KEY_HP, KEY_BACK, KEY_LK, KEY_HK } ;	/* Black Hayato (hyper) */

static Float shiden_spd[4] = { -20.0f, 1.25f, -20.0f, DOT_X( 0.5f ) } ;
static Float shiden_next_spd[4] = { -DOT_X( 10.0f ), 1.25f, -20.0f, 1.25f } ;
static Float guren_spd[8] = { 0.0f, 0.0f, DOT_Y( 8.0f ), -DOT_Y( 0.5f ), -20.0f, DOT_X( 0.5f ), DOT_Y( 16.0f ), -DOT_Y( 0.5f ) } ;
static Float rasetsu_spd[14] = {
	-20.0f, 1.5625f, -DOT_X( 10.0f ), 1.25f, -DOT_X( 10.0f ), 1.25f, -DOT_X( 10.0f ), 1.25f,
	-DOT_X( 10.0f ), 1.25f, -DOT_X( 10.0f ), 1.25f, -20.0f, DOT_X( 0.625f ),
} ;

static void ( *pl12_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl12_main,
	pl12_com_ck,
	pl12_atck_set,
	pl12_atck,
	pl12_dash,
	pl12_noki,
	pl12_appear,
	pl12_win,
	pl12_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl12_catch_ck,	/* returns a value */
	pl12_catch_set,
	pl12_catch,
	pl12_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl12_delay_ck,
	pl12_t_kick,
	pl12_upper,
	pl12_sp_unison,
	pl12_sp_gattai,
	pl12_sp_assist,
	pl12_sp_zero,
	0,
	0,
	0,
	0,
	( void ( * )( PLAYER_WORK *pl_w ) )pl12_free_ck,
} ;

static Uint8 shiden_ck( PLAYER_WORK *pl_w );
static Uint8 guren_ck( PLAYER_WORK *pl_w );
static Uint8 byakko_ck( PLAYER_WORK *pl_w );
static Uint8 engetsu_ck( PLAYER_WORK *pl_w );
static Uint8 rasetsu_ck( PLAYER_WORK *pl_w );
static Uint8 field_ck( PLAYER_WORK *pl_w );
static Uint8 black_ck( PLAYER_WORK *pl_w );
static Uint8 engetsu_free_ck( PLAYER_WORK *pl_w );
static Uint8 rasetsu_free_ck( PLAYER_WORK *pl_w );
static Uint8 black_free_ck( PLAYER_WORK *pl_w );
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static int engetsu_delay_ck( PLAYER_WORK *pl_w );
static int rasetsu_delay_ck( PLAYER_WORK *pl_w );
static int field_delay_ck( PLAYER_WORK *pl_w );
static int black_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl12_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl12_tbl[i] ;
}

/*==============================================================*/
/*			One air special a jump, but any number in a			*/
/*			super jump											*/
/*==============================================================*/
static Uint8 jump_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->pl_stance != STANCE_AIR ) return 1 ;
	if( !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	return 1 ;
}

/****************************************************************/
/*			Commands; while the hyper combos are free they		*/
/*			cost no gauge										*/
/****************************************************************/
void pl12_com_ck( PLAYER_WORK *pl_w )
{
	PL12_LOCAL_W *lw = ( PL12_LOCAL_W * )pl_w->tlocal ;

	if( !lw->hyper_timer ){
		if( pl_v_combi_ck( pl_w ) ) return ;
		if( pl_assist_ck( pl_w ) ) return ;
		if( pl_change_ck( pl_w ) ) return ;
		if( pl_vcounter_ck( pl_w ) ) return ;
		if( black_ck( pl_w ) ) return ;
		if( field_ck( pl_w ) ) return ;
		if( engetsu_ck( pl_w ) ) return ;
		if( rasetsu_ck( pl_w ) ) return ;
		if( guren_ck( pl_w ) ) return ;
		if( shiden_ck( pl_w ) ) return ;
		if( byakko_ck( pl_w ) ) return ;
	} else {
		if( pl_assist_ck( pl_w ) ) return ;
		if( pl_vcounter_ck( pl_w ) ) return ;
		if( black_free_ck( pl_w ) ) return ;
		if( engetsu_free_ck( pl_w ) ) return ;
		if( rasetsu_free_ck( pl_w ) ) return ;
		if( guren_ck( pl_w ) ) return ;
		if( shiden_ck( pl_w ) ) return ;
		if( byakko_ck( pl_w ) ) return ;
	}
	if( snapback_ck( pl_w ) ) return ;
	if( pl_chouhatsu_ck( pl_w, 9 ) ){
		chouhatsu_ck( pl_w ) ;
		return ;
	}
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
static Uint8 shiden_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_shiden, &pl_w->com_w ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl12_sp_shiden ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 guren_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_guren, &pl_w->com_w0 ) ) return 0 ;
	if( !jump_com_ck( pl_w ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl12_sp_guren ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 byakko_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_byakko, &pl_w->com_w1 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl12_sp_byakko ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 engetsu_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_engetsu, &pl_w->com_w2 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl12_sp_engetsu ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 rasetsu_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_rasetsu, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w3 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl12_sp_rasetsu ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
/*			Not while it is already on							*/
/*==============================================================*/
static Uint8 field_ck( PLAYER_WORK *pl_w )
{
	PL12_LOCAL_W *lw = ( PL12_LOCAL_W * )pl_w->tlocal ;

	if( lw->hyper_timer > 0 ) return 0 ;
	if( !com_nml_ck( pl_w, com_field, &pl_w->com_w4 ) ) return 0 ;
	if( !jump_com_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w4 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl12_sp_field ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
/*			The level 3 hyper combo								*/
/*==============================================================*/
static Uint8 black_ck( PLAYER_WORK *pl_w )
{
	if( !com_seq_ck( pl_w, com_black, &pl_w->com_w5 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w5 ) ;
	if( *pl_w->gauge_level < 3 ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl12_sp_black ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 engetsu_free_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_engetsu, &pl_w->com_w2 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl12_sp_engetsu ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 rasetsu_free_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_rasetsu, &pl_w->com_w3 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w3 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl12_sp_rasetsu ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 black_free_ck( PLAYER_WORK *pl_w )
{
	if( !com_seq_ck( pl_w, com_black, &pl_w->com_w5 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w5 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl12_sp_black ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 9 ) ) return 0 ;
	pl_w->pl_sp_type = pl12_sp_chouhatsu ;
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
	pl_w->pl_sp_type = pl12_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl12_delay_ck( PLAYER_WORK *pl_w )
{
	if( engetsu_delay_ck( pl_w ) || rasetsu_delay_ck( pl_w ) || field_delay_ck( pl_w ) || black_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static int engetsu_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_engetsu, &pl_w->com_w2 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 3 ;
	return 1 ;
}

/*==============================================================*/
static int rasetsu_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_rasetsu, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 4 ;
	return 1 ;
}

/*==============================================================*/
static int field_delay_ck( PLAYER_WORK *pl_w )
{
	PL12_LOCAL_W *lw = ( PL12_LOCAL_W * )pl_w->tlocal ;

	if( lw->hyper_timer > 0 ) return 0 ;
	if( !com_nml_ck( pl_w, com_field, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 5 ;
	return 1 ;
}

/*==============================================================*/
static int black_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_seq_ck( pl_w, com_black, &pl_w->com_w5 ) ) return 0 ;
	if( *pl_w->gauge_level < 3 ) return 0 ;
	pl_w->delay_sp_type = 8 ;
	return 1 ;
}

static void parts_set( PLAYER_WORK *pl_w );
static void parts_half( PLAYER_WORK *pl_w, Uint8 no );
static void parts_down( PLAYER_WORK *pl_w, Uint8 no );
static void parts_up( PLAYER_WORK *pl_w, Uint8 no );
static void flash_ck( PLAYER_WORK *pl_w );
static void free_hyper_cnt( PLAYER_WORK *pl_w );

/****************************************************************/
void pl12_main( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_1 ) flash_ck( pl_w ) ;
	if( !( Sys->game_timer0 & 1 ) ) parts_set( pl_w ) ;
	free_hyper_cnt( pl_w ) ;
	pl12_free_ck( pl_w ) ;
}

/*==============================================================*/
/*			The sword's parts light up and fade as the			*/
/*			animation says										*/
/*==============================================================*/
static void parts_set( PLAYER_WORK *pl_w )
{
	COL_WORK *ow = &col_work[pl_w->col_no] + 1 ;

	if( HI_BYTE( pl_w->cg_rival ) == 1 ){
		ow->trans_flag = 1 ;
		ow->col[9] |= COL_ALPHA ;
		ow->col[10] |= COL_ALPHA ;
		ow->col[11] |= COL_ALPHA ;
		ow->col[12] |= COL_ALPHA ;
		ow->col[14] |= COL_ALPHA ;
	}
	if( HI_BYTE( pl_w->cg_rival ) == 2 ) parts_half( pl_w, 10 ) ;
	if( HI_BYTE( pl_w->cg_rival ) == 3 ) parts_half( pl_w, 11 ) ;
	if( HI_BYTE( pl_w->cg_rival ) == 4 ) parts_half( pl_w, 12 ) ;
	if( HI_BYTE( pl_w->cg_rival ) == 5 ) parts_half( pl_w, 10 ) ;
	if( HI_BYTE( pl_w->cg_rival ) == 6 ){
		parts_half( pl_w, 10 ) ;
		parts_half( pl_w, 12 ) ;
	}
	if( HI_BYTE( pl_w->cg_rival ) == 7 ) parts_half( pl_w, 12 ) ;
	if( HI_BYTE( pl_w->cg_rival ) == 8 ){
		parts_half( pl_w, 10 ) ;
		parts_half( pl_w, 12 ) ;
	}
	if( HI_BYTE( pl_w->cg_rival ) == 9 ){
		parts_half( pl_w, 9 ) ;
		parts_half( pl_w, 10 ) ;
	}
	if( HI_BYTE( pl_w->cg_rival ) == 10 ){
		parts_half( pl_w, 9 ) ;
		parts_up( pl_w, 10 ) ;
	}
	if( HI_BYTE( pl_w->cg_rival ) == 11 ){
		parts_half( pl_w, 9 ) ;
		parts_half( pl_w, 10 ) ;
	}
	if( HI_BYTE( pl_w->cg_rival ) == 12 ){
		parts_half( pl_w, 12 ) ;
		parts_half( pl_w, 14 ) ;
	}
	if( HI_BYTE( pl_w->cg_rival ) == 13 ) parts_half( pl_w, 9 ) ;
	if( HI_BYTE( pl_w->cg_rival ) == 14 ){
		parts_half( pl_w, 10 ) ;
		parts_half( pl_w, 12 ) ;
		parts_half( pl_w, 14 ) ;
	}
	if( HI_BYTE( pl_w->cg_rival ) == 15 ){
		parts_half( pl_w, 9 ) ;
		parts_half( pl_w, 12 ) ;
	}
	if( HI_BYTE( pl_w->cg_rival ) == 16 ){
		parts_down( pl_w, 10 ) ;
		parts_down( pl_w, 12 ) ;
	}
	if( HI_BYTE( pl_w->cg_rival ) == 50 ) parts_up( pl_w, 10 ) ;
	if( HI_BYTE( pl_w->cg_rival ) == 51 ){ }
	if( HI_BYTE( pl_w->cg_rival ) == 52 ) parts_up( pl_w, 12 ) ;
	if( HI_BYTE( pl_w->cg_rival ) == 53 ) parts_up( pl_w, 10 ) ;
	if( HI_BYTE( pl_w->cg_rival ) == 54 ){
		parts_up( pl_w, 10 ) ;
		parts_up( pl_w, 12 ) ;
	}
	if( HI_BYTE( pl_w->cg_rival ) == 55 ) parts_up( pl_w, 12 ) ;
	if( HI_BYTE( pl_w->cg_rival ) == 56 ){
		parts_up( pl_w, 10 ) ;
		parts_up( pl_w, 12 ) ;
	}
	if( HI_BYTE( pl_w->cg_rival ) == 57 ){
		parts_up( pl_w, 9 ) ;
		parts_up( pl_w, 10 ) ;
	}
	if( HI_BYTE( pl_w->cg_rival ) == 59 ){
		parts_up( pl_w, 9 ) ;
		parts_up( pl_w, 10 ) ;
	}
	if( HI_BYTE( pl_w->cg_rival ) == 60 ){
		parts_up( pl_w, 12 ) ;
		parts_up( pl_w, 14 ) ;
	}
	if( HI_BYTE( pl_w->cg_rival ) == 61 ) parts_up( pl_w, 9 ) ;
	if( HI_BYTE( pl_w->cg_rival ) == 62 ){
		parts_up( pl_w, 10 ) ;
		parts_up( pl_w, 12 ) ;
		parts_up( pl_w, 14 ) ;
	}
	if( HI_BYTE( pl_w->cg_rival ) == 63 ){
		parts_up( pl_w, 9 ) ;
		parts_up( pl_w, 12 ) ;
	}
}

/*==============================================================*/
static void parts_half( PLAYER_WORK *pl_w, Uint8 no )
{
	COL_WORK *ow = &col_work[pl_w->col_no] + 1 ;

	ow->trans_flag = 1 ;
	if( ow->col[no] <= 0x7000 ){
		ow->col[no] |= 0x6000 ;
		return ;
	}
	ow->col[no] -= 0x1000 ;
}

/*==============================================================*/
static void parts_down( PLAYER_WORK *pl_w, Uint8 no )
{
	COL_WORK *ow = &col_work[pl_w->col_no] + 1 ;

	ow->trans_flag = 1 ;
	if( ow->col[no] == 0 ) return ;
	ow->col[no] -= 0x1000 ;
	if( ow->col[no] > 0x1000 ){ }
}

/*==============================================================*/
static void parts_up( PLAYER_WORK *pl_w, Uint8 no )
{
	COL_WORK *ow = &col_work[pl_w->col_no] + 1 ;

	ow->trans_flag = 1 ;
	if( ow->col[no] >= 0xf000 ){
		ow->col[no] |= COL_ALPHA ;
		return ;
	}
	ow->col[no] += 0x1000 ;
}

/*==============================================================*/
/*			The body flashes during a normal attack				*/
/*==============================================================*/
static void flash_ck( PLAYER_WORK *pl_w )
{
	PL12_LOCAL_W *lw = ( PL12_LOCAL_W * )pl_w->tlocal ;
	Uint8 n ;

	if( pl_w->act_st != N_NML_ATCK ) lw->weak_flag = 0 ;
	if( pl_w->act_st == N_NML_ATCK && lw->flash_flag ){
		n = Sys->game_timer0 % 4 ;
		if( n >= 2 ){
			pl_color_trans( pl_w, 0, 6, 1 ) ;
			return ;
		}
		if( n >= 0 ){
			pl_color_trans( pl_w, 0, 0, 1 ) ;
			return ;
		}
	}
	lw->flash_flag = 0 ;
	pl_color_trans( pl_w, 0, 0, 1 ) ;
}

/*==============================================================*/
/*			The free hyper combos run out, and the body			*/
/*			flashes while they last								*/
/*==============================================================*/
static void free_hyper_cnt( PLAYER_WORK *pl_w )
{
	PL12_LOCAL_W *lw = ( PL12_LOCAL_W * )pl_w->tlocal ;
	Uint8 n ;

	if( !lw->hyper_timer ){
		lw->hyper_timer = 0 ;
		if( !lw->flash_flag ) pl_color_trans( pl_w, 0, 0, 1 ) ;
		return ;
	}
	if( vs_sys_work.vs_st >= VS_END_WAIT ){
		lw->hyper_timer = 0 ;
		if( !lw->flash_flag ) pl_color_trans( pl_w, 0, 0, 1 ) ;
		return ;
	}
	if( pl_w->team_no ){
		lw->hyper_timer = 0 ;
		if( !lw->flash_flag ) pl_color_trans( pl_w, 0, 0, 1 ) ;
		return ;
	}
	if( pl_w->vital_new <= 0 ){
		lw->hyper_timer = 0 ;
		if( !lw->flash_flag ) pl_color_trans( pl_w, 0, 0, 1 ) ;
		return ;
	}
	lw->hyper_timer -- ;
	pl_w->hyper_bg_tm = 5 ;
	if( lw->flash_flag ) return ;
	n = Sys->game_timer0 % 4 ;
	if( n >= 2 ){
		pl_color_trans( pl_w, 0, 6, 1 ) ;
		return ;
	}
	if( n >= 0 ){
		pl_color_trans( pl_w, 0, 7, 1 ) ;
		return ;
	}
	pl_color_trans( pl_w, 0, 0, 1 ) ;
}

/*==============================================================*/
static void sp_start_set( PLAYER_WORK *pl_w )
{
	PL12_LOCAL_W *lw = ( PL12_LOCAL_W * )pl_w->tlocal ;

	if( lw->hyper_timer > 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg_tm = 5 ;
	} else {
		pl_w->delay_timer = 2 ;
		pl_w->hyper_bg_tm = 5 ;
	}
}

/*==============================================================*/
static void sp_end_set( PLAYER_WORK *pl_w )
{
	PL12_LOCAL_W *lw = ( PL12_LOCAL_W * )pl_w->tlocal ;

	if( lw->hyper_timer > 0 ){
		pl_w->delay_timer = pl_w->delay_flag = 0 ;
		pl_w->hyper_bg_tm = 5 ;
	} else {
		pl_w->delay_timer = pl_w->delay_flag = 0 ;
		pl_w->hyper_bg_tm = pl_w->hyper_bg = 0 ;
	}
}

/****************************************************************/
/*			While the hyper combos are free						*/
/****************************************************************/
int pl12_free_ck( PLAYER_WORK *pl_w )
{
	PL12_LOCAL_W *lw = ( PL12_LOCAL_W * )pl_w->tlocal ;

	if( lw->hyper_timer > 0 ) return 1 ;
	return 0 ;
}

static void nml_atck( PLAYER_WORK *pl_w );
static void nml_atck_dash( PLAYER_WORK *pl_w );
static void stand_punch( PLAYER_WORK *pl_w );
static void stand_punch_dash( PLAYER_WORK *pl_w );
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
void pl12_atck_set( PLAYER_WORK *pl_w )
{
	static void ( *atck_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_atck, jump_atck_ck, jump_atck, nml_atck_dash,
	} ;

	atck_set_jmp[pl_w->atck_type]( pl_w ) ;
}

/*==============================================================*/
/*			The same, without the chained punches				*/
/*==============================================================*/
static void nml_atck_dash( PLAYER_WORK *pl_w )
{
	pl_brake_set( pl_w ) ;
	if( pl_w->atck_pk == 1 ){
		if( pl_w->pl_stance == STANCE_CROUCH ) crouch_kick( pl_w ) ;
		else stand_kick( pl_w ) ;
	} else {
		if( pl_w->pl_stance == STANCE_CROUCH ) crouch_punch( pl_w ) ;
		else stand_punch_dash( pl_w ) ;
	}
}

/*==============================================================*/
static void stand_punch_dash( PLAYER_WORK *pl_w )
{
	PL12_LOCAL_W *lw = ( PL12_LOCAL_W * )pl_w->tlocal ;

	switch( pl_w->nml_no ){
	case 0:
		lw->weak_flag = 1 ;
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_STAND_LP ;
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		lw->weak_flag = 1 ;
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_STAND_MP ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		if( pl_w->jump_dir ){
			pl_w->char_no.b.no = 2 ;
			pl_w->atck_no = ATCK_STAND_HP ;
			pl_w->chain_tbl = hp_tbl ;
			pl_w->nml_level = 2 ;
		} else {
			pl_w->spd.x = pl_w->pl_dir ? DOT_X( 9.5f ) : -DOT_X( 9.5f ) ;
			pl_w->acc.x = pl_w->pl_dir ? -0.104166664f : 0.104166664f ;
			pl_w->char_no.b.no = 3 ;
			pl_w->atck_no = pl12_at_dash_hp ;
			pl_w->chain_tbl = ex_tbl_01 ;
			pl_w->nml_level = 2 ;
			HIT_RESET( pl_w ) ;
			pl_char_set( pl_w, Char_Extra, pl_w->char_no.b.no ) ;
			return ;
		}
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Stand_P, pl_w->char_no.b.no ) ;
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
/*			From 3 on, nml_no is a chained attack				*/
/*==============================================================*/
static void stand_punch( PLAYER_WORK *pl_w )
{
	PL12_LOCAL_W *lw = ( PL12_LOCAL_W * )pl_w->tlocal ;

	lw->flash_flag = 0 ;
	if( pl_w->nml_no >= 3 ) lw->flash_flag = 1 ;
	switch( pl_w->nml_no ){
	case 0:
		lw->weak_flag = 1 ;
		if( pl_w->atck_lever & KEY_BACK ){
			lw->flash_flag = 1 ;
			pl_w->char_no.b.no = 5 ;
			pl_w->atck_no = pl12_at_back_lp ;
			pl_w->chain_tbl = ex_tbl_02 ;
			HIT_RESET( pl_w ) ;
			pl_char_set( pl_w, Char_Extra, pl_w->char_no.b.no ) ;
			pl_w->nml_level = 0 ;
			return ;
		}
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_STAND_LP ;
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		lw->weak_flag = 1 ;
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_STAND_MP ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_STAND_HP ;
		pl_w->chain_tbl = hp_tbl ;
		if( !lw->weak_flag ){
			if( pl_w->atck_lever & KEY_BACK ){
				lw->flash_flag = 1 ;
				pl_w->char_no.b.no = 13 ;
				pl_w->atck_no = pl12_at_back_hp ;
				pl_w->chain_tbl = ex_tbl_0a ;
				HIT_RESET( pl_w ) ;
				pl_char_set( pl_w, Char_Extra, pl_w->char_no.b.no ) ;
				pl_w->nml_level = 0 ;
				return ;
			}
		}
		pl_w->nml_level = 2 ;
		if( pl_w->atck_lever & KEY_FORWARD ){
			pl_w->char_no.b.no = 3 ;
			pl_w->atck_no = pl12_at_fwd_hp ;
			pl_w->chain_tbl = ex_tbl_00 ;
		}
		pl_w->nml_level = 2 ;
		break ;
	case 3:
		pl_w->char_no.b.no = 6 ;
		pl_w->atck_no = pl12_at_chain_06 ;
		pl_w->chain_tbl = ex_tbl_03 ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, pl_w->char_no.b.no ) ;
		pl_w->nml_level = 0 ;
		return ;
	case 4:
		pl_w->char_no.b.no = 7 ;
		pl_w->atck_no = pl12_at_chain_07 ;
		pl_w->chain_tbl = ex_tbl_04 ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, pl_w->char_no.b.no ) ;
		pl_w->nml_level = 0 ;
		return ;
	case 5:
		pl_w->char_no.b.no = 8 ;
		pl_w->atck_no = pl12_at_chain_08 ;
		pl_w->chain_tbl = ex_tbl_05 ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, pl_w->char_no.b.no ) ;
		pl_w->nml_level = 0 ;
		return ;
	case 6:
		pl_w->char_no.b.no = 9 ;
		pl_w->atck_no = pl12_at_chain_09 ;
		pl_w->chain_tbl = ex_tbl_06 ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, pl_w->char_no.b.no ) ;
		pl_w->nml_level = 0 ;
		return ;
	case 7:
		pl_w->char_no.b.no = 11 ;
		pl_w->atck_no = pl12_at_chain_11 ;
		pl_w->chain_tbl = ex_tbl_08 ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, pl_w->char_no.b.no ) ;
		pl_w->nml_level = 0 ;
		return ;
	case 8:
		pl_w->char_no.b.no = 12 ;
		pl_w->atck_no = pl12_at_chain_12 ;
		pl_w->chain_tbl = ex_tbl_09 ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, pl_w->char_no.b.no ) ;
		pl_w->nml_level = 0 ;
		return ;
	case 9:
		pl_w->char_no.b.no = 14 ;
		pl_w->atck_no = pl12_at_chain_14 ;
		pl_w->chain_tbl = ex_tbl_0b ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, pl_w->char_no.b.no ) ;
		pl_w->nml_level = 0 ;
		return ;
	case 10:
		pl_w->char_no.b.no = 15 ;
		pl_w->atck_no = pl12_at_chain_15 ;
		pl_w->chain_tbl = ex_tbl_0c ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, pl_w->char_no.b.no ) ;
		pl_w->nml_level = 0 ;
		return ;
	case 11:
		pl_w->char_no.b.no = 16 ;
		pl_w->atck_no = pl12_at_chain_16 ;
		pl_w->chain_tbl = ex_tbl_0d ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, pl_w->char_no.b.no ) ;
		pl_w->nml_level = 0 ;
		return ;
	case 12:
		pl_w->char_no.b.no = 18 ;
		pl_w->atck_no = pl12_at_chain_18 ;
		pl_w->chain_tbl = ex_tbl_0f ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, pl_w->char_no.b.no ) ;
		pl_w->nml_level = 0 ;
		return ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Stand_P, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
static void crouch_punch( PLAYER_WORK *pl_w )
{
	PL12_LOCAL_W *lw = ( PL12_LOCAL_W * )pl_w->tlocal ;

	lw->flash_flag = 0 ;
	if( pl_w->nml_no >= 3 ) lw->flash_flag = 1 ;
	switch( pl_w->nml_no ){
	case 0:
		lw->weak_flag = 1 ;
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_CROUCH_LP ;
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Crouch_P, pl_w->char_no.b.no ) ;
		return ;
	case 1:
		lw->weak_flag = 1 ;
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_CROUCH_MP ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Crouch_P, pl_w->char_no.b.no ) ;
		return ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_CROUCH_HP ;
		pl_w->chain_tbl = hp_tbl ;
		pl_w->nml_level = 2 ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Crouch_P, pl_w->char_no.b.no ) ;
		return ;
	case 3:
		pl_w->char_no.b.no = 6 ;
		pl_w->atck_no = pl12_at_chain_06 ;
		pl_w->chain_tbl = ex_tbl_03 ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, pl_w->char_no.b.no ) ;
		pl_w->nml_level = 0 ;
		return ;
	case 4:
		pl_w->char_no.b.no = 7 ;
		pl_w->atck_no = pl12_at_chain_07 ;
		pl_w->chain_tbl = ex_tbl_04 ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, pl_w->char_no.b.no ) ;
		pl_w->nml_level = 0 ;
		return ;
	case 5:
		pl_w->char_no.b.no = 8 ;
		pl_w->atck_no = pl12_at_chain_08 ;
		pl_w->chain_tbl = ex_tbl_05 ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, pl_w->char_no.b.no ) ;
		pl_w->nml_level = 0 ;
		return ;
	case 6:
		pl_w->char_no.b.no = 9 ;
		pl_w->atck_no = pl12_at_chain_09 ;
		pl_w->chain_tbl = ex_tbl_06 ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, pl_w->char_no.b.no ) ;
		pl_w->nml_level = 0 ;
		return ;
	case 7:
		pl_w->char_no.b.no = 11 ;
		pl_w->atck_no = pl12_at_chain_11 ;
		pl_w->chain_tbl = ex_tbl_08 ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, pl_w->char_no.b.no ) ;
		pl_w->nml_level = 0 ;
		return ;
	case 8:
		pl_w->char_no.b.no = 12 ;
		pl_w->atck_no = pl12_at_chain_12 ;
		pl_w->chain_tbl = ex_tbl_09 ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, pl_w->char_no.b.no ) ;
		pl_w->nml_level = 0 ;
		return ;
	case 9:
		pl_w->char_no.b.no = 14 ;
		pl_w->atck_no = pl12_at_chain_14 ;
		pl_w->chain_tbl = ex_tbl_0b ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, pl_w->char_no.b.no ) ;
		pl_w->nml_level = 0 ;
		return ;
	case 10:
		pl_w->char_no.b.no = 15 ;
		pl_w->atck_no = pl12_at_chain_15 ;
		pl_w->chain_tbl = ex_tbl_0c ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, pl_w->char_no.b.no ) ;
		pl_w->nml_level = 0 ;
		return ;
	case 11:
		pl_w->char_no.b.no = 16 ;
		pl_w->atck_no = pl12_at_chain_16 ;
		pl_w->chain_tbl = ex_tbl_0d ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, pl_w->char_no.b.no ) ;
		pl_w->nml_level = 0 ;
		return ;
	case 12:
		pl_w->char_no.b.no = 18 ;
		pl_w->atck_no = pl12_at_chain_18 ;
		pl_w->chain_tbl = ex_tbl_0f ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, pl_w->char_no.b.no ) ;
		pl_w->nml_level = 0 ;
		return ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Crouch_P, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
static void stand_kick( PLAYER_WORK *pl_w )
{
	PL12_LOCAL_W *lw = ( PL12_LOCAL_W * )pl_w->tlocal ;

	lw->flash_flag = 0 ;
	if( pl_w->nml_no >= 3 ) lw->flash_flag = 1 ;
	switch( pl_w->nml_no ){
	case 0:
		lw->weak_flag = 1 ;
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_STAND_LK ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		pl_w->chain_tbl = lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		lw->weak_flag = 1 ;
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
	case 3:
		pl_w->char_no.b.no = 10 ;
		pl_w->atck_no = pl12_at_chain_10 ;
		pl_w->chain_tbl = ex_tbl_07 ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, pl_w->char_no.b.no ) ;
		pl_w->nml_level = 0 ;
		return ;
	case 4:
		pl_w->char_no.b.no = 17 ;
		pl_w->atck_no = pl12_at_chain_17 ;
		pl_w->chain_tbl = ex_tbl_0e ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, pl_w->char_no.b.no ) ;
		pl_w->nml_level = 0 ;
		return ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Stand_K, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
static void crouch_kick( PLAYER_WORK *pl_w )
{
	PL12_LOCAL_W *lw = ( PL12_LOCAL_W * )pl_w->tlocal ;

	lw->flash_flag = 0 ;
	if( pl_w->nml_no >= 3 ) lw->flash_flag = 1 ;
	switch( pl_w->nml_no ){
	case 0:
		lw->weak_flag = 1 ;
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_CROUCH_LK ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		pl_w->chain_tbl = lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		lw->weak_flag = 1 ;
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
		break ;
	case 3:
		pl_w->char_no.b.no = 10 ;
		pl_w->atck_no = pl12_at_chain_10 ;
		pl_w->chain_tbl = ex_tbl_07 ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, pl_w->char_no.b.no ) ;
		pl_w->nml_level = 0 ;
		return ;
	case 4:
		pl_w->char_no.b.no = 17 ;
		pl_w->atck_no = pl12_at_chain_17 ;
		pl_w->chain_tbl = ex_tbl_0e ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, pl_w->char_no.b.no ) ;
		pl_w->nml_level = 0 ;
		return ;
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
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Jump_P, pl_w->char_no.b.no ) ;
		if( pl_w->jump_atk_cnt & JATK_P_MASK ) pl_w->jump_atk_cnt -= 0x01 ;
		return ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Jump_P, pl_w->char_no.b.no ) ;
	if( pl_w->jump_atk_cnt & JATK_P_MASK ) pl_w->jump_atk_cnt -= 0x01 ;
}

/*==============================================================*/
/*			The down heavy kick counts as a punch				*/
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
		if( pl_w->atck_lever & KEY_DOWN ){
			pl_w->char_no.b.no = 6 ;
			pl_w->atck_no = pl12_at_jdown_hk ;
		}
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_hk_tbl ;
		else pl_w->chain_tbl = sj_hk_tbl ;
		pl_w->nml_level = 2 ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Jump_K, pl_w->char_no.b.no ) ;
		if( pl_w->jump_atk_cnt & JATK_P_MASK ) pl_w->jump_atk_cnt -= 0x01 ;
		return ;
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
static void stand_punch_move_dash( PLAYER_WORK *pl_w );
static void sjump_move( PLAYER_WORK *pl_w );
static void jump_move( PLAYER_WORK *pl_w );
static void jump_punch_move( PLAYER_WORK *pl_w );
static void jump_kick_move( PLAYER_WORK *pl_w );
static void stand_hk_move( PLAYER_WORK *pl_w );
static void hop_atck( PLAYER_WORK *pl_w );
static void hop_init( PLAYER_WORK *pl_w );
static void hop_move( PLAYER_WORK *pl_w );
static void dive_atck( PLAYER_WORK *pl_w );
static void dive_init( PLAYER_WORK *pl_w );
static void dive_move( PLAYER_WORK *pl_w );
static void dive_bound( PLAYER_WORK *pl_w );
static void dive_move_e( PLAYER_WORK *pl_w );
static void rush_atck( PLAYER_WORK *pl_w );
static void rush_init( PLAYER_WORK *pl_w );
static void rush_move( PLAYER_WORK *pl_w );
static void rush_dash( PLAYER_WORK *pl_w );
static void step_move( PLAYER_WORK *pl_w );
static void chain07_atck( PLAYER_WORK *pl_w );
static void chain07_move( PLAYER_WORK *pl_w );
static void chain07_move_e( PLAYER_WORK *pl_w );
static void chain08_atck( PLAYER_WORK *pl_w );
static void chain08_init( PLAYER_WORK *pl_w );
static void chain08_move( PLAYER_WORK *pl_w );
static void chain08_land( PLAYER_WORK *pl_w );
static void chain08_move_e( PLAYER_WORK *pl_w );
static void chain12_atck( PLAYER_WORK *pl_w );
static void chain12_init( PLAYER_WORK *pl_w );
static void chain12_move( PLAYER_WORK *pl_w );
static void chain12_up( PLAYER_WORK *pl_w );
static void chain12_fall( PLAYER_WORK *pl_w );
static void chain12_move_e( PLAYER_WORK *pl_w );

/****************************************************************/
void pl12_atck( PLAYER_WORK *pl_w )
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
		else stand_punch_move_dash( pl_w ) ;
	}
}

/*==============================================================*/
static void nml_move( PLAYER_WORK *pl_w )
{
	if( pl_w->nml_no < 3 ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		pl_brake_stop( pl_w ) ;
	}
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
	case 3:
		step_move( pl_w ) ;
		return ;
	case 4:
		step_move( pl_w ) ;
		return ;
	case 5:
		step_move( pl_w ) ;
		return ;
	case 6:
		step_move( pl_w ) ;
		return ;
	case 7:
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		chain07_atck( pl_w ) ;
		return ;
	case 8:
		chain08_atck( pl_w ) ;
		return ;
	case 9:
		step_move( pl_w ) ;
		return ;
	case 10:
		step_move( pl_w ) ;
		return ;
	case 11:
		step_move( pl_w ) ;
		return ;
	case 12:
		chain12_atck( pl_w ) ;
		return ;
	case 2:
		if( pl_w->atck_lever & KEY_FORWARD ){
			hop_atck( pl_w ) ;
			return ;
		}
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
	case 3:
		step_move( pl_w ) ;
		return ;
	case 4:
		step_move( pl_w ) ;
		return ;
	case 5:
		step_move( pl_w ) ;
		return ;
	case 6:
		step_move( pl_w ) ;
		return ;
	case 7:
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		chain07_atck( pl_w ) ;
		return ;
	case 8:
		chain08_atck( pl_w ) ;
		return ;
	case 9:
		step_move( pl_w ) ;
		return ;
	case 10:
		step_move( pl_w ) ;
		return ;
	case 11:
		step_move( pl_w ) ;
		return ;
	case 12:
		chain12_atck( pl_w ) ;
		return ;
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
	case 3:
		step_move( pl_w ) ;
		return ;
	case 4:
		step_move( pl_w ) ;
		break ;
	case 2:
		stand_hk_move( pl_w ) ;
		break ;
	case 0:
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
static void crouch_kick_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 3:
		step_move( pl_w ) ;
		return ;
	case 4:
		step_move( pl_w ) ;
		return ;
	case 0:
	case 1:
	case 2:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
static void stand_punch_move_dash( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 2:
		rush_atck( pl_w ) ;
		return ;
	case 0:
	case 1:
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
	if( pl_w->atck_pk == 1 ){
		jump_kick_move( pl_w ) ;
		switch( pl_w->nml_no ){
		case 2:
			if( pl_w->atck_lever & KEY_DOWN ) return ;
			break ;
		case 0:
		case 1:
			break ;
		}
	} else jump_punch_move( pl_w ) ;
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
	switch( pl_w->nml_no ){
	case 2:
		if( pl_w->atck_lever & KEY_DOWN ) dive_atck( pl_w ) ;
		else if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
		break ;
	case 0:
	case 1:
	default:
		if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
/*			The heavy standing kick hits twice					*/
/*==============================================================*/
static void stand_hk_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	if( pl_w->cg_type == 1 ){
		pl_w->cg_type = 0 ;
		pl_w->atck_no = pl12_at_stand_hk_2 ;
		HIT_RESET( pl_w ) ;
	}
}

/*==============================================================*/
/*			Holding up on the heavy punch hops forward			*/
/*==============================================================*/
static void hop_atck( PLAYER_WORK *pl_w )
{
	static void ( *hop_jmp[])( PLAYER_WORK *pl_w ) = {
		hop_init, hop_move,
	} ;

	hop_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void hop_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->spd.x = pl_w->pl_dir ? DOT_Y( 2.0f ) : -DOT_Y( 2.0f ) ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 4.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.5f ) ;
}

/*--------------------------------------------------------------*/
static void hop_move( PLAYER_WORK *pl_w )
{
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_land_ef_set( pl_w ) ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pos.y = pl_w->ground_y ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			Holding down on the heavy jumping kick dives		*/
/*==============================================================*/
static void dive_atck( PLAYER_WORK *pl_w )
{
	static void ( *dive_jmp[])( PLAYER_WORK *pl_w ) = {
		dive_init, dive_move, dive_bound, dive_move_e,
	} ;

	dive_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void dive_init( PLAYER_WORK *pl_w )
{
	pl_w->sjump_flag = 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		return ;
	}
	pl_w->routine_3 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
}

/*--------------------------------------------------------------*/
static void dive_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->hit_flag ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = DOT_X( 2.5f ) ;
		if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = 15.0f ;
		pl_w->acc.y = -DOT_Y( 0.5f ) ;
		pl_w->jump_dir = 1 ;
		pl_char_set( pl_w, Char_Extra, 20 ) ;
		return ;
	}
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_3 = 3 ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, 21 ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void dive_bound( PLAYER_WORK *pl_w )
{
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_3 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, 21 ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void dive_move_e( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			The third heavy punch rushes in						*/
/*==============================================================*/
static void rush_atck( PLAYER_WORK *pl_w )
{
	static void ( *rush_jmp[])( PLAYER_WORK *pl_w ) = {
		rush_init, rush_move, rush_dash,
	} ;

	rush_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void rush_init( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w ;

	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	if( pl_w->hit_flag & HR_GUARD ) return ;
	else {
		if( pl_w->hit_flag ){
			em_w = pl_w->em_w ;
			if( !em_w->team_no ){
				pl_w->routine_3 ++ ;
				pl_w->push_ng_timer = 2 ;
				pl_w->timer0 = 10 ;
			}
		}
	}
}

/*--------------------------------------------------------------*/
static void rush_move( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type == 1 ){
		pl_w->cg_type = 0 ;
		pl_w->acc.x *= 4.0f ;
	}
	if( pl_w->cg_type == 2 ){
		pl_w->cg_type = 0 ;
		pl_w->acc.x *= 4.0f ;
	}
	if( pl_w->spd.x * pl_w->acc.x > 0.0f ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
	pl_w->push_ng_timer = 2 ;
	if( pl_w->timer0-- <= 0 ){
		if( pl_w->pl_now_lvbt & KEY_HP ){
			pl_w->routine_3 ++ ;
			pl_w->push_ng_timer = 2 ;
			pl_dir_set( pl_w ) ;
			pl_w->atck_no = pl12_at_rush_hp ;
			HIT_RESET( pl_w ) ;
			pl_char_set( pl_w, Char_Extra, 4 ) ;
		}
	}
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void rush_dash( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type == 1 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
	if( pl_w->spd.x * pl_w->acc.x > 0.0f ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
	if( pl_w->cg_type == 2 ){
		pl_w->cg_type = 0 ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 9.5f ) : -DOT_X( 9.5f ) ;
		pl_w->acc.x = pl_w->pl_dir ? -DOT_X( 0.375f ) : DOT_X( 0.375f ) ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			The chain attacks step forward as the animation		*/
/*			says												*/
/*==============================================================*/
static void step_move( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type ){
		pl_w->pos.x += pl_w->pl_dir ? DOT_X( pl_w->cg_type ) : -DOT_X( pl_w->cg_type ) ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void chain07_atck( PLAYER_WORK *pl_w )
{
	static void ( *chain07_jmp[])( PLAYER_WORK *pl_w ) = {
		chain07_move, chain07_move_e,
	} ;

	chain07_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void chain07_move( PLAYER_WORK *pl_w )
{
	if( pl_w->hit_flag ){
		pl_w->routine_3 ++ ;
		shl12_00_set2( pl_w, 3, 0, 0 ) ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void chain07_move_e( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void chain08_atck( PLAYER_WORK *pl_w )
{
	static void ( *chain08_jmp[])( PLAYER_WORK *pl_w ) = {
		chain08_init, chain08_move, chain08_land, chain08_move_e,
	} ;
	PLAYER_WORK *em ;

	em = pl_w->target_w ;
	chain08_jmp[pl_w->routine_3]( pl_w ) ;
	if( em->dm_no == 10 && em->dm_hit_w == pl_w ) em->ukemi_flag = 0 ;
}

/*==============================================================*/
static void chain08_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_w->spd.x = pl_w->pl_dir ? DOT_X( 2.0f ) : -DOT_X( 2.0f ) ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 16.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.75f ) ;
	pl_w->pl_stance = STANCE_AIR ;
	chain08_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void chain08_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_3 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_land_ef_set( pl_w ) ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void chain08_land( PLAYER_WORK *pl_w )
{
	if( pl_w->hit_flag ){
		pl_w->routine_3 ++ ;
		shl12_00_set2( pl_w, 5, 0, 0 ) ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void chain08_move_e( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void chain12_atck( PLAYER_WORK *pl_w )
{
	static void ( *chain12_jmp[])( PLAYER_WORK *pl_w ) = {
		chain12_init, chain12_move, chain12_up, chain12_fall, chain12_move_e,
	} ;

	chain12_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void chain12_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.y = DOT_Y( 8.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.5f ) ;
	pl_char_set( pl_w, Char_Extra, 18 ) ;
}

/*--------------------------------------------------------------*/
static void chain12_move( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type > 1 ){
		pl_w->pos.x += pl_w->pl_dir ? DOT_X( pl_w->cg_type ) : -DOT_X( pl_w->cg_type ) ;
	}
	if( pl_w->cg_type == 1 ){
		pl_w->routine_3 ++ ;
		pl_w->timer0 = 1 ;
		pl_air_set( pl_w ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void chain12_up( PLAYER_WORK *pl_w )
{
	if( pl_w->spd.y < 0.0f ) pl_w->routine_3 ++ ;
	else {
		if( !--pl_w->timer0 ){
			shl12_00_set( pl_w, 1 ) ;
			shl12_00_set( pl_w, 2 ) ;
			pl_w->timer0 = 6 ;
		}
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->cg_type ) return ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void chain12_fall( PLAYER_WORK *pl_w )
{
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_3 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_land_ef_set( pl_w ) ;
	} else {
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->cg_type ) return ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void chain12_move_e( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/****************************************************************/
/*			The launcher										*/
/****************************************************************/
void pl12_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		if( !pl_w->atck_pk ){
			pl_gauge_add( pl_w, 5 ) ;
			pl_w->atck_no = pl12_at_upper ;
			HIT_RESET( pl_w ) ;
			pl_w->pl_stance = STANCE_CROUCH ;
			pl_char_set( pl_w, Char_Extra, 19 ) ;
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
void pl12_dash( PLAYER_WORK *pl_w )
{
	static void ( *dash_jmp[])( PLAYER_WORK *pl_w ) = {
		dash_init, dash_move, dash_end,
	} ;

	dash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dash_init( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 9.5f ) : -DOT_X( 9.5f ) ;
		pl_w->acc.x = pl_w->pl_dir ? -DOT_X( 0.25f ) : DOT_X( 0.25f ) ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void dash_move( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_land_ef_set( pl_w ) ;
		pl_w->cg_type = 0 ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void dash_end( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void noki_init( PLAYER_WORK *pl_w );
static void noki_move( PLAYER_WORK *pl_w );
static void noki_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash										*/
/****************************************************************/
void pl12_noki( PLAYER_WORK *pl_w )
{
	static void ( *noki_jmp[])( PLAYER_WORK *pl_w ) = {
		noki_init, noki_move, noki_end,
	} ;

	noki_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void noki_init( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 9.5f ) : DOT_X( 9.5f ) ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void noki_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->acc.x = pl_w->pl_dir ? DOT_X( 0.25f ) : -DOT_X( 0.25f ) ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void noki_end( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void appear_init( PLAYER_WORK *pl_w );
static void appear_move( PLAYER_WORK *pl_w );
static void appear_a( PLAYER_WORK *pl_w );
static void appear_a_init( PLAYER_WORK *pl_w );
static void appear_a_move( PLAYER_WORK *pl_w );
static void appear_a_jump( PLAYER_WORK *pl_w );
static void appear_a_fall( PLAYER_WORK *pl_w );
static void appear_a_move_e( PLAYER_WORK *pl_w );
static void appear_b( PLAYER_WORK *pl_w );
static void appear_b_init( PLAYER_WORK *pl_w );
static void appear_b_move( PLAYER_WORK *pl_w );

/****************************************************************/
/*			One of two entrances, chosen at random				*/
/****************************************************************/
void pl12_appear( PLAYER_WORK *pl_w )
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
	pl_w->sub_type = Rnd() ;
	pl_w->sub_type &= 1 ;
	appear_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void appear_move( PLAYER_WORK *pl_w )
{
	static void ( *appear_move_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_a, appear_b,
	} ;

	appear_move_jmp[pl_w->sub_type]( pl_w ) ;
}

/*==============================================================*/
/*			He rides in from off the screen and jumps off		*/
/*==============================================================*/
static void appear_a( PLAYER_WORK *pl_w )
{
	static void ( *appear_a_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_a_init, appear_a_move, appear_a_jump, appear_a_fall, appear_a_move_e,
	} ;

	appear_a_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void appear_a_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_w->spd.x = pl_w->pos.x ;
	pl_w->pos.x += pl_w->disp_dir ? -324 : 324 ;
	pl_w->timer0 = 18 ;
	pl_w->pos.y = 72.0f ;
	pls12_00_set( pl_w, 2 ) ;
	pls12_00_set( pl_w, 3 ) ;
	pls12_00_set( pl_w, 4 ) ;
	pl_char_set( pl_w, Char_Appear, 0 ) ;
}

/*--------------------------------------------------------------*/
static void appear_a_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->disp_dir ? 18 : -18 ;
	pl_char_move( pl_w ) ;
	if( !pl_w->timer0-- ){
		pl_w->routine_3 ++ ;
		pl_w->spd.y = DOT_Y( 6.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_w->timer0 = 8 ;
		pl_char_set( pl_w, Char_Appear, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_a_jump( PLAYER_WORK *pl_w )
{
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( !pl_w->timer0-- ) pl_w->routine_3 ++ ;
}

/*--------------------------------------------------------------*/
static void appear_a_fall( PLAYER_WORK *pl_w )
{
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_3 ++ ;
		pl_land_ef_set( pl_w ) ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->pos.x = pl_w->spd.x ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_char_set( pl_w, Char_Appear, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_a_move_e( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type == 1 ){
		pl_w->cg_type = 0 ;
		pls12_00_set( pl_w, 5 ) ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_w->routine_1 ++ ;
}

/*==============================================================*/
static void appear_b( PLAYER_WORK *pl_w )
{
	static void ( *appear_b_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_b_init, appear_b_move,
	} ;

	appear_b_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void appear_b_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_char_set( pl_w, Char_Appear, 3 ) ;
}

/*--------------------------------------------------------------*/
static void appear_b_move( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type == 1 ){
		pl_w->cg_type = 0 ;
		pls12_00_set( pl_w, 5 ) ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_w->routine_1 ++ ;
}

static void win_init( PLAYER_WORK *pl_w );
static void win_move( PLAYER_WORK *pl_w );
static void win_time_ko( PLAYER_WORK *pl_w );
static void win_ko_init( PLAYER_WORK *pl_w );
static void win_ko_move( PLAYER_WORK *pl_w );
static void lose_time_ko( PLAYER_WORK *pl_w );
static void lose_ko_init( PLAYER_WORK *pl_w );
static void lose_ko_move( PLAYER_WORK *pl_w );
static void win_pose1( PLAYER_WORK *pl_w );
static void win_p1_init( PLAYER_WORK *pl_w );
static void win_p1_move( PLAYER_WORK *pl_w );
static void win_p1_move_e( PLAYER_WORK *pl_w );
static void win_pose2( PLAYER_WORK *pl_w );
static void win_p2_init( PLAYER_WORK *pl_w );
static void win_p2_move( PLAYER_WORK *pl_w );
static void win_p2_move_e( PLAYER_WORK *pl_w );
static void win_pose3( PLAYER_WORK *pl_w );
static void win_p3_init( PLAYER_WORK *pl_w );
static void win_p3_move( PLAYER_WORK *pl_w );
static void win_p3_keep( PLAYER_WORK *pl_w );
static void win_p3_run_init( PLAYER_WORK *pl_w );
static void win_p3_run( PLAYER_WORK *pl_w );
static void win_p3_ride( PLAYER_WORK *pl_w );
static void win_p3_move_e( PLAYER_WORK *pl_w );
static void win_type_set( PLAYER_WORK *pl_w );

/****************************************************************/
void pl12_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_init, win_move,
	} ;

	win_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void win_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	if( !pl_w->type ) win_type_set( pl_w ) ;
	win_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_move( PLAYER_WORK *pl_w )
{
	static void ( *win_move_jmp[])( PLAYER_WORK *pl_w ) = {
		win_time_ko,
		lose_time_ko,
		win_time_ko,
		lose_time_ko,
		lose_time_ko,
		win_time_ko,
		win_pose1,
		win_pose2,
		win_pose3,
		win_time_ko,
		win_time_ko,
	} ;

	if( pl_win_end_ck( pl_w ) ) pl_stand_ret( pl_w ) ;
	else win_move_jmp[pl_w->type]( pl_w ) ;
}

/*==============================================================*/
static void lose_time_ko( PLAYER_WORK *pl_w )
{
	static void ( *lose_ko_jmp[])( PLAYER_WORK *pl_w ) = {
		lose_ko_init, lose_ko_move,
	} ;

	lose_ko_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void lose_ko_init( PLAYER_WORK *pl_w )
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
	pl_char_set( pl_w, Char_Win, 6 ) ;
}

/*--------------------------------------------------------------*/
static void lose_ko_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
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
	pl_char_set( pl_w, Char_Win, 4 ) ;
}

/*--------------------------------------------------------------*/
static void win_ko_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void win_pose1( PLAYER_WORK *pl_w )
{
	static void ( *win_p1_jmp[])( PLAYER_WORK *pl_w ) = {
		win_p1_init, win_p1_move, win_p1_move_e,
	} ;

	win_p1_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void win_p1_init( PLAYER_WORK *pl_w )
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
	pls12_00_set( pl_w, 9 ) ;
	pls12_00_set( pl_w, 6 ) ;
	pl_char_set( pl_w, Char_Win, 4 ) ;
}

/*--------------------------------------------------------------*/
static void win_p1_move( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_end_flag < 0 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Win, 5 ) ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_p1_move_e( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void win_pose2( PLAYER_WORK *pl_w )
{
	static void ( *win_p2_jmp[])( PLAYER_WORK *pl_w ) = {
		win_p2_init, win_p2_move, win_p2_move_e,
	} ;

	win_p2_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void win_p2_init( PLAYER_WORK *pl_w )
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
	pls12_00_set( pl_w, 9 ) ;
	pls12_00_set( pl_w, 7 ) ;
	pl_char_set( pl_w, Char_Win, 4 ) ;
}

/*--------------------------------------------------------------*/
static void win_p2_move( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_end_flag < 0 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Win, 5 ) ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_p2_move_e( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
/*			He runs off the screen and rides back in			*/
/*==============================================================*/
static void win_pose3( PLAYER_WORK *pl_w )
{
	static void ( *win_p3_jmp[])( PLAYER_WORK *pl_w ) = {
		win_p3_init, win_p3_move, win_p3_keep, win_p3_run_init, win_p3_run, win_p3_ride,
		win_p3_move_e,
	} ;

	win_p3_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void win_p3_init( PLAYER_WORK *pl_w )
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
	pls12_00_set( pl_w, 9 ) ;
	pls12_00_set( pl_w, 8 ) ;
	pl_char_set( pl_w, Char_Win, 4 ) ;
}

/*--------------------------------------------------------------*/
static void win_p3_move( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_end_flag < 0 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Win, 5 ) ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_p3_keep( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_p3_run_init( PLAYER_WORK *pl_w )
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
	pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 16.0f ) : DOT_X( 16.0f ) ;
	pl_w->spd.y = DOT_Y( 4.0f ) ;
	pl_w->acc.y = 0.01674107f ;
	pl_w->timer0 = 90 ;
	pl_char_set( pl_w, Char_Win, 0 ) ;
}

/*--------------------------------------------------------------*/
static void win_p3_run( PLAYER_WORK *pl_w )
{
	if( !pl_w->timer0-- ){
		pl_w->routine_3 ++ ;
		pl_w->pos.x = pl_w->disp_dir ? scr_work.scr_left + -96.0f : scr_work.scr_right + 96.0f ;
		pls12_00_set( pl_w, 2 ) ;
		pls12_00_set( pl_w, 3 ) ;
		pls12_00_set( pl_w, 4 ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
		pl_w->timer0 = 120 ;
		pl_char_set( pl_w, Char_Win, 1 ) ;
		return ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_p3_ride( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->disp_dir ? 18 : -18 ;
	if( !pl_w->timer0-- ) pl_w->routine_3 ++ ;
}

/*--------------------------------------------------------------*/
static void win_p3_move_e( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
/*			The win pose: the button held, or one at random		*/
/*==============================================================*/
static void win_type_set( PLAYER_WORK *pl_w )
{
	static Sint16 win_type_tbl[16] = {
		5, 5, 5, 6, 6, 7, 7, 7, 8, 8, 8, 8, 9, 10, 10, 10,
	} ;

	if( pl_w->pl_new_lvbt & KEY_BUTTON ){
		if( pl_w->pl_new_lvbt & KEY_LP ) pl_w->type = 5 ;
		else if( pl_w->pl_new_lvbt & KEY_HP ) pl_w->type = 6 ;
		else if( pl_w->pl_new_lvbt & KEY_A1 ) pl_w->type = 7 ;
		else if( pl_w->pl_new_lvbt & KEY_LK ) pl_w->type = 8 ;
		else if( pl_w->pl_new_lvbt & KEY_HK ) pl_w->type = 9 ;
		else pl_w->type = 10 ;
	} else pl_w->type = win_type_tbl[Rnd() & 15] ;
	if( pl_be_cnt( pl_w ) > 1 ) pl_w->type = 5 ;
}

static void sp_shiden( PLAYER_WORK *pl_w );
static void sp_guren( PLAYER_WORK *pl_w );
static void sp_byakko( PLAYER_WORK *pl_w );
static void sp_engetsu( PLAYER_WORK *pl_w );
static void sp_rasetsu( PLAYER_WORK *pl_w );
static void sp_field( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );
static void sp_counter( PLAYER_WORK *pl_w );
static void sp_black( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );

/****************************************************************/
void pl12_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *pl_sp_tbl[])( PLAYER_WORK *pl_w ) = {
		sp_shiden,					/* 00 Shiden */
		sp_guren,					/* 01 Guren */
		sp_byakko,					/* 02 Byakkou Hou */
		sp_engetsu,					/* 03 Engetsu (hyper) */
		sp_rasetsu,					/* 04 Rasetsu Zan (hyper) */
		sp_field,					/* 05 Plasma Field (hyper) */
		sp_snapback,					/* 06 the snapback */
		sp_counter,					/* 07 Guren (variable counter) */
		sp_black,					/* 08 Black Hayato (hyper) */
		sp_chouhatsu,					/* 09 the taunt */
	} ;

	pl_sp_tbl[ pl_w->pl_sp_type ]( pl_w ) ;
}

static void shiden_init( PLAYER_WORK *pl_w );
static void shiden_move( PLAYER_WORK *pl_w );
static void shiden_dash( PLAYER_WORK *pl_w );
static void shiden_next( PLAYER_WORK *pl_w );
static void shiden_next_dash( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A two-stage slash: it dashes in, and dashes			*/
/*			again												*/
/*==============================================================*/
static void sp_shiden( PLAYER_WORK *pl_w )
{
	static void ( *shiden_jmp[])( PLAYER_WORK *pl_w ) = {
		shiden_init, shiden_move, shiden_dash, shiden_next, shiden_next_dash,
	} ;

	shiden_jmp[ pl_w->routine_2 ]( pl_w ) ;
}

/*==============================================================*/
static void shiden_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	if( pl_w->team_type == 3 ){
		pl_w->atck_no = pl12_at_shiden_vc ;
		HIT_RESET( pl_w ) ;
	}else{
		pl_w->atck_no = pl_w->atck_level + 48 ;
		HIT_RESET( pl_w ) ;
	}
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level ) ;
}

/*--------------------------------------------------------------*/
static void shiden_move( PLAYER_WORK *pl_w )
{
	Float *spd = shiden_spd ;

	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	spd += pl_w->atck_level * 2 ;
	pl_w->spd.x = pl_w->pl_dir ? -spd[0] : spd[0] ;
	pl_w->acc.x = pl_w->pl_dir ? -spd[1] : spd[1] ;
	pl_w->timer0 = 0 ;
	shiden_dash( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shiden_dash( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type == 1 ){
		pl_w->routine_2 ++ ;
		shiden_dash( pl_w ) ;
		return ;
	}
	if( !pl_w->team_no && pl_w->cg_type == 2 ){
		pls12_00_set( pl_w, 1 ) ;
		pl_w->timer0 ++ ;
		pl_w->cg_type = 0 ;
	}
	if( pl_w->spd.x * pl_w->acc.x < 0.0f ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
}

/*--------------------------------------------------------------*/
static void shiden_next( PLAYER_WORK *pl_w )
{
	Float *spd ;

	pl_w->routine_2 ++ ;
	if( pl_w->team_type == 3 ){
		pl_w->atck_no = pl12_at_shiden_vc_2 ;
		HIT_RESET( pl_w ) ;
	}else{
		pl_w->atck_no = pl_w->atck_level + 50 ;
		HIT_RESET( pl_w ) ;
	}
	spd = shiden_next_spd ;
	spd += pl_w->atck_level * 2 ;
	pl_w->spd.x = pl_w->pl_dir ? -spd[0] : spd[0] ;
	pl_w->acc.x = pl_w->pl_dir ? -spd[1] : spd[1] ;
	pl_sp_ef_set( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shiden_next_dash( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !pl_w->team_no && pl_w->cg_type == 2 ){
		pls12_00_set( pl_w, 1 ) ;
		pl_w->timer0 ++ ;
		pl_w->cg_type = 0 ;
	}
	if( pl_w->spd.x * pl_w->acc.x > 0.0f ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void guren_init( PLAYER_WORK *pl_w );
static void guren_move( PLAYER_WORK *pl_w );
static void guren_up( PLAYER_WORK *pl_w );
static void guren_fall( PLAYER_WORK *pl_w );
static void guren_move_e( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A rising slash that hits three times				*/
/*==============================================================*/
static void sp_guren( PLAYER_WORK *pl_w )
{
	static void ( *guren_jmp[])( PLAYER_WORK *pl_w ) = {
		guren_init, guren_move, guren_up, guren_fall, guren_move_e,
	} ;

	guren_jmp[ pl_w->routine_2 ]( pl_w ) ;
}

/*==============================================================*/
static void guren_init( PLAYER_WORK *pl_w )
{
	Float *spd ;

	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl_w->atck_level + 52 ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	spd = guren_spd ;
	spd += pl_w->atck_level * 4 ;
	pl_w->spd.x = pl_w->pl_dir ? -spd[0] : spd[0] ;
	pl_w->acc.x = pl_w->pl_dir ? -spd[1] : spd[1] ;
	pl_w->spd.y = spd[2] ;
	pl_w->acc.y = spd[3] ;
	if( pl_w->pl_stance != STANCE_AIR ){
		pl_w->pl_stance = STANCE_STAND ;
		pl_sp_ef_set( pl_w ) ;
	}
	pl_w->sub_no = 0 ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 2 ) ;
}

/*--------------------------------------------------------------*/
static void guren_move( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 1 ;
		pl_air_set( pl_w ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
	} else {
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void guren_up( PLAYER_WORK *pl_w )
{
	if( pl_w->spd.y < 0.0f ) pl_w->routine_2 ++ ;
	else {
		if( !--pl_w->timer0 ){
			if( pl_w->hit_flag ) pl_w->sub_no ++ ;
			if( pl_w->sub_no < 3 ){
				if( pl_w->sub_no == 2 ){
					pl_w->atck_no = pl_w->atck_level + 82 ;
					HIT_RESET( pl_w ) ;
				}else{
					pl_w->atck_no = pl_w->atck_level + 52 ;
					HIT_RESET( pl_w ) ;
				}
			}
			shl12_00_set( pl_w, 1 ) ;
			shl12_00_set( pl_w, 2 ) ;
			pl_w->timer0 = 6 ;
		}
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->cg_type ) return ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void guren_fall( PLAYER_WORK *pl_w )
{
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_land_ef_set( pl_w ) ;
	} else {
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->cg_type ) return ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void guren_move_e( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void byakko_init( PLAYER_WORK *pl_w );
static void byakko_move( PLAYER_WORK *pl_w );
static void byakko_move_e( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A slash that throws the opponent when it			*/
/*			connects											*/
/*==============================================================*/
static void sp_byakko( PLAYER_WORK *pl_w )
{
	static void ( *byakko_jmp[])( PLAYER_WORK *pl_w ) = {
		byakko_init, byakko_move, byakko_move_e,
	} ;

	byakko_jmp[ pl_w->routine_2 ]( pl_w ) ;
}

/*==============================================================*/
static void byakko_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl12_at_shiden ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 4 ) ;
}

/*--------------------------------------------------------------*/
static void byakko_move( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_end_flag < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, 5 ) ;
	}
	if( pl_w->cg_type ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 0xc3 ;
			pl_catch_set( pl_w, em ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void byakko_move_e( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void engetsu_init( PLAYER_WORK *pl_w );
static void engetsu_move( PLAYER_WORK *pl_w );
static void engetsu_keep( PLAYER_WORK *pl_w );
static void engetsu_move_e( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_engetsu( PLAYER_WORK *pl_w )
{
	static void ( *engetsu_jmp[])( PLAYER_WORK *pl_w ) = {
		engetsu_init, engetsu_move, engetsu_keep, engetsu_move_e,
	} ;

	engetsu_jmp[ pl_w->routine_2 ]( pl_w ) ;
}

/*==============================================================*/
static void engetsu_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl12_at_engetsu ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->sub_no = 0 ;
	pl_char_set( pl_w, Char_Super, 0 ) ;
}

/*--------------------------------------------------------------*/
static void engetsu_move( PLAYER_WORK *pl_w )
{
	PL12_LOCAL_W *lw = ( PL12_LOCAL_W * )pl_w->tlocal ;
	VEC3 ofs ;

	sp_start_set( pl_w ) ;
	if( pl_w->cg_type == 1 ){
		pl_w->cg_type = 0 ;
		ofs.x = 0.0f ;
		ofs.y = DOT_Y( 80.0f ) ;
		if( !lw->hyper_timer ) pl_hyper_start( pl_w, &ofs, 1 ) ;
		else pl_hyper_start( pl_w, &ofs, 0 ) ;
	}
	if( pl_w->cg_type == 2 ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		shl12_00_set3( pl_w, 7 ) ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void engetsu_keep( PLAYER_WORK *pl_w )
{
	sp_start_set( pl_w ) ;
	if( pl_w->cg_type == 3 ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void engetsu_move_e( PLAYER_WORK *pl_w )
{
	sp_end_set( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void rasetsu_init( PLAYER_WORK *pl_w );
static void rasetsu_move( PLAYER_WORK *pl_w );
static void rasetsu_slash( PLAYER_WORK *pl_w );
static void rasetsu_move_e( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A run of slashes, stepping forward on each			*/
/*==============================================================*/
static void sp_rasetsu( PLAYER_WORK *pl_w )
{
	static void ( *rasetsu_jmp[])( PLAYER_WORK *pl_w ) = {
		rasetsu_init, rasetsu_move, rasetsu_slash, rasetsu_move_e,
	} ;

	rasetsu_jmp[ pl_w->routine_2 ]( pl_w ) ;
}

/*==============================================================*/
static void rasetsu_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->atck_no = pl12_at_rasetsu ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 1 ) ;
}

/*--------------------------------------------------------------*/
static void rasetsu_move( PLAYER_WORK *pl_w )
{
	PL12_LOCAL_W *lw = ( PL12_LOCAL_W * )pl_w->tlocal ;
	VEC3 ofs ;

	sp_start_set( pl_w ) ;
	if( pl_w->cg_type == 1 ){
		pl_w->cg_type = 0 ;
		ofs.x = -40.0f ;
		ofs.y = DOT_Y( 64.0f ) ;
		if( !lw->hyper_timer ) pl_hyper_start( pl_w, &ofs, 1 ) ;
		else pl_hyper_start( pl_w, &ofs, 0 ) ;
	}
	if( pl_w->cg_type == 2 ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->timer0 = 2 ;
		pl_w->spd.x = rasetsu_spd[0] ;
		pl_w->acc.x = rasetsu_spd[1] ;
		pl_w->spd.x = pl_w->pl_dir ? -pl_w->spd.x : pl_w->spd.x ;
		pl_w->acc.x = pl_w->pl_dir ? -pl_w->acc.x : pl_w->acc.x ;
		pl_sp_ef_set( pl_w ) ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void rasetsu_slash( PLAYER_WORK *pl_w )
{
	Float *spd ;

	sp_start_set( pl_w ) ;
	if( pl_w->cg_type == 3 ){
		pl_w->routine_2 ++ ;
		rasetsu_move_e( pl_w ) ;
	}
	if( pl_w->cg_type == 2 ){
		pl_w->cg_type = 0 ;
		spd = rasetsu_spd ;
		spd += pl_w->timer0 ;
		pl_w->spd.x = spd[0] ;
		pl_w->acc.x = spd[1] ;
		pl_w->spd.x = pl_w->pl_dir ? -pl_w->spd.x : pl_w->spd.x ;
		pl_w->acc.x = pl_w->pl_dir ? -pl_w->acc.x : pl_w->acc.x ;
		pl_w->atck_no = pl_w->timer0 / 2 + 55 ;
		HIT_RESET( pl_w ) ;
		pl_w->timer0 += 2 ;
		pl_sp_ef_set( pl_w ) ;
	}
	if( pl_w->spd.x * pl_w->acc.x < 0.0f ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	} else {
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void rasetsu_move_e( PLAYER_WORK *pl_w )
{
	sp_end_set( pl_w ) ;
	if( pl_w->cg_type == 4 ){
		pl_w->cg_type = 0 ;
		shl12_00_set2( pl_w, 5, 0, 1 ) ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void field_init( PLAYER_WORK *pl_w );
static void field_move( PLAYER_WORK *pl_w );
static void field_keep( PLAYER_WORK *pl_w );
static void field_fall( PLAYER_WORK *pl_w );
static void field_move_e( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_field( PLAYER_WORK *pl_w )
{
	static void ( *field_jmp[])( PLAYER_WORK *pl_w ) = {
		field_init, field_move, field_keep, field_fall, field_move_e,
	} ;

	field_jmp[ pl_w->routine_2 ]( pl_w ) ;
}

/*==============================================================*/
static void field_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl12_at_field ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	if( pl_w->pl_stance != STANCE_AIR ){
		pl_w->pl_stance = STANCE_STAND ;
		pl_sp_ef_set( pl_w ) ;
	}
	pl_char_set( pl_w, Char_Super, 2 ) ;
	field_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void field_move( PLAYER_WORK *pl_w )
{
	PL12_LOCAL_W *lw ;
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_w->cg_type == 1 ){
		pl_w->cg_type = 0 ;
		lw = ( PL12_LOCAL_W * )pl_w->tlocal ;
		if( !lw->hyper_timer ){
			ofs.x = -DOT_X( 8.0f ) ;
			ofs.y = DOT_Y( 80.0f ) ;
			pl_hyper_start( pl_w, &ofs, 1 ) ;
		}
	}
	if( pl_w->cg_type == 2 ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		shl12_00_set( pl_w, 0 ) ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void field_keep( PLAYER_WORK *pl_w )
{
	sp_start_set( pl_w ) ;
	if( pl_w->pl_stance != STANCE_AIR ){
		if( pl_w->cg_end_flag < 0 ){
			pl_w->routine_2 = 4 ;
			pl_color_set( pl_w, PC_DEFAULT ) ;
			pl_char_set( pl_w, Char_Super, 3 ) ;
			return ;
		}
	} else {
		if( pl_w->cg_end_flag < 0 ){
			pl_w->routine_2 ++ ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_w->acc.y = -DOT_Y( 0.375f ) ;
			pl_color_set( pl_w, PC_DEFAULT ) ;
			pl_char_set( pl_w, Char_Super, 4 ) ;
			return ;
		}
	}
	pl_char_move( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	if( pl_w->cg_type == 3 ) pl_color_set( pl_w, 4 ) ;
}

/*--------------------------------------------------------------*/
static void field_fall( PLAYER_WORK *pl_w )
{
	sp_end_set( pl_w ) ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 5 ) ;
	}
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
}

/*--------------------------------------------------------------*/
static void field_move_e( PLAYER_WORK *pl_w )
{
	sp_end_set( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void black_init( PLAYER_WORK *pl_w );
static void black_move( PLAYER_WORK *pl_w );
static void black_catch( PLAYER_WORK *pl_w );
static void black_cut_init( PLAYER_WORK *pl_w );
static void black_cut( PLAYER_WORK *pl_w );
static void black_cut_end( PLAYER_WORK *pl_w );
static void black_turn( PLAYER_WORK *pl_w );
static void black_move_e( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A lunging grab that ends in a flurry of cuts		*/
/*==============================================================*/
static void sp_black( PLAYER_WORK *pl_w )
{
	static void ( *black_jmp[])( PLAYER_WORK *pl_w ) = {
		black_init, black_move, black_catch, black_cut_init,
		black_cut, black_cut_end, black_turn, black_move_e,
	} ;

	black_jmp[ pl_w->routine_2 ]( pl_w ) ;
}

/*==============================================================*/
static void black_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl12_at_black ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 6 ) ;
}

/*--------------------------------------------------------------*/
static void black_move( PLAYER_WORK *pl_w )
{
	PL12_LOCAL_W *lw = ( PL12_LOCAL_W * )pl_w->tlocal ;
	VEC3 ofs ;

	sp_start_set( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->cg_type == 1 ){
		pl_w->cg_type = 0 ;
		ofs.x = DOT_X( 8.0f ) ;
		ofs.y = 120.0f ;
		if( !lw->hyper_timer ) pl_hyper_start( pl_w, &ofs, 3 ) ;
		else pl_hyper_start( pl_w, &ofs, 0 ) ;
		pl_w->spd.x = pl_w->pl_dir ? 20.0f : -20.0f ;
		pl_w->acc.x = pl_w->pl_dir ? -DOT_X( 0.375f ) : DOT_X( 0.375f ) ;
	}
	if( pl_w->hit_flag && pl_sp_catch_ck( pl_w ) ){
		pl_w->routine_2 ++ ;
		pl_w->catch_no = 0xc4 ;
		pl_sp_catch_set( pl_w, pl_w->em_w ) ;
		pl_w->catch_flag = 1 ;
		pl_w->catch_ng_timer = 2 ;
		pl_w->push_ng_timer = 2 ;
		pl_char_set( pl_w, Char_Super, 7 ) ;
		black_catch( pl_w ) ;
		return ;
	}
	if( pl_w->cg_end_flag < 0 ){
		pl_w->routine_2 = 7 ;
		pl_char_set( pl_w, Char_Super, 10 ) ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void black_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	sp_start_set( pl_w ) ;
	em = pl_w->catch_w ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		ofs.x = 0.0f ;
		ofs.y = 120.0f ;
		hit_mark_set( em, &ofs, 13 ) ;
		pl_se_req( pl_w, 6 ) ;
		vital_add_dmg2( em, -1 ) ;
	}
	if( pl_w->cg_end_flag < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 100 ;
		flash_dark_long_set() ;
		return ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void black_cut_init( PLAYER_WORK *pl_w )
{
	sp_start_set( pl_w ) ;
	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	flash_work.timer = 2 ;
	pl_w->routine_2 ++ ;
	scr_excep_set( pl_w, 1, 13 ) ;
	pl_w->pos.x = ( scr_work.scr_left + scr_work.scr_right ) / 2.0f ;
	pl_w->pos.x += pl_w->pl_dir ? -DOT_X( 80.0f ) : DOT_X( 80.0f ) ;
}

/*==============================================================*/
/*			The throw's hits: sparks on the held player			*/
/*			while the timer runs								*/
/*==============================================================*/
static void black_cut( register PLAYER_WORK *pl_w )
{
	Uint8 no ;
	register PLAYER_WORK *em ;
	VEC3 ofs ;

	sp_start_set( pl_w ) ;
	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	flash_work.timer = 2 ;
	if( pl_w->timer0 -- == 0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 80 ;
		pl_char_set( pl_w, Char_Super, 8 ) ;
	}
	if( pl_w->timer0 % 9 == 1 ){
		em = pl_w->catch_w ;
		ofs.x = ( Float )( Rnd() & 63 ) * 1.6666666f ;
		ofs.y = ( Float )( Rnd() & 95 ) * 2.142857f ;
		no = ( Rnd() & 7 ) + 9 ;
		hit_mark_set1( em, &ofs, no ) ;
		pl_se_req( pl_w, Rnd() % 3 + 4 ) ;
		combo_add( em, pl_w, 1 ) ;
		vital_add_dmg2( em, -2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void black_cut_end( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	sp_start_set( pl_w ) ;
	em = pl_w->catch_w ;
	if( pl_w->timer0 == 10 ){
		ofs.x = 0.0f ;
		ofs.y = DOT_Y( 80.0f ) ;
		hit_mark_set( em, &ofs, 10 ) ;
		pl_se_req( pl_w, 6 ) ;
		vital_add_dmg2( em, -1 ) ;
		combo_add( em, pl_w, 1 ) ;
	}
	if( pl_w->timer0 == 5 ){
		ofs.x = 0.0f ;
		ofs.y = DOT_Y( 80.0f ) ;
		hit_mark_set( em, &ofs, 16 ) ;
		pl_se_req( pl_w, 6 ) ;
		vital_add_dmg2( em, -1 ) ;
		combo_add( em, pl_w, 1 ) ;
	}
	if( pl_w->timer0 -- == 0 ){
		pl_w->routine_2 ++ ;
		scr_excep_set( pl_w, 0, 13 ) ;
		pl_w->spd.x = pl_w->pl_dir ? -20.0f : 20.0f ;
		pl_w->acc.x = pl_w->pl_dir ? DOT_X( 0.5f ) : -DOT_X( 0.5f ) ;
		SCR_EXCEP_SET ;
		pl_w->cg_type = 0 ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->atck_no = pl12_at_black_end ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void black_turn( PLAYER_WORK *pl_w )
{
	sp_start_set( pl_w ) ;
	if( pl_w->cg_end_flag < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->disp_dir ^= 1 ;
		pl_char_set( pl_w, Char_Super, 10 ) ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void black_move_e( PLAYER_WORK *pl_w )
{
	sp_end_set( pl_w ) ;
	if( pl_w->spd.x * pl_w->acc.x < 0.0f ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	} else {
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void chouhatsu_init( PLAYER_WORK *pl_w );
static void chouhatsu_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w )
{
	static void ( *chouhatsu_jmp[])( PLAYER_WORK *pl_w ) = {
		chouhatsu_init, chouhatsu_move,
	} ;

	chouhatsu_jmp[ pl_w->routine_2 ]( pl_w ) ;
}

/*==============================================================*/
static void chouhatsu_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_dir_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 7 ) ;
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

	snapback_jmp[ pl_w->routine_2 ]( pl_w ) ;
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
	pl_w->atck_no = pl12_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 2 ) ;
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
		ofs.x = DOT_X( 8.0f ) ;
		ofs.y = DOT_Y( 104.0f ) ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_move( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A kick that leaps off the wall						*/
/****************************************************************/
void pl12_t_kick( PLAYER_WORK *pl_w )
{
	static void ( *t_kick_jmp[])( PLAYER_WORK *pl_w ) = {
		t_kick_init, t_kick_move, t_kick_land,
	} ;

	t_kick_jmp[ pl_w->routine_2 ]( pl_w ) ;
}

/*==============================================================*/
static void t_kick_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->spd.x = 30.0f ;
	if( !pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 2.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->atck_no = pl12_at_t_kick ;
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
		pl_land_ef_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Extra, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void t_kick_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void counter_init( PLAYER_WORK *pl_w );
static void counter_move( PLAYER_WORK *pl_w );
static void counter_up( PLAYER_WORK *pl_w );
static void counter_fall( PLAYER_WORK *pl_w );
static void counter_move_e( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A rising slash										*/
/*==============================================================*/
static void sp_counter( PLAYER_WORK *pl_w )
{
	static void ( *counter_jmp[])( PLAYER_WORK *pl_w ) = {
		counter_init, counter_move, counter_up, counter_fall, counter_move_e,
	} ;

	counter_jmp[ pl_w->routine_2 ]( pl_w ) ;
}

/*==============================================================*/
static void counter_init( PLAYER_WORK *pl_w )
{
	Float *spd ;

	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl12_at_guren_vc ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	spd = guren_spd ;
	spd += pl_w->atck_level * 4 ;
	pl_w->spd.x = pl_w->pl_dir ? -spd[0] : spd[0] ;
	pl_w->acc.x = pl_w->pl_dir ? -spd[1] : spd[1] ;
	pl_w->spd.y = spd[2] ;
	pl_w->acc.y = spd[3] ;
	pl_char_set( pl_w, Char_Special, 6 ) ;
}

/*--------------------------------------------------------------*/
static void counter_move( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 1 ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
	} else {
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void counter_up( PLAYER_WORK *pl_w )
{
	if( pl_w->spd.y < 0.0f ) pl_w->routine_2 ++ ;
	else {
		if( -- pl_w->timer0 == 0 ){
			shl12_00_set( pl_w, 1 ) ;
			shl12_00_set( pl_w, 2 ) ;
			pl_w->timer0 = 6 ;
		}
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->cg_type ) return ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void counter_fall( PLAYER_WORK *pl_w )
{
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_land_ef_set( pl_w ) ;
	} else {
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->cg_type ) return ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void counter_move_e( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static PLAYER_WORK *nml_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *crouch_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *jump_catch_ck( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Can a throw start?									*/
/****************************************************************/
PLAYER_WORK *pl12_catch_ck( PLAYER_WORK *pl_w )
{
	static PLAYER_WORK *( *catch_ck_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_catch_ck, crouch_catch_ck, jump_catch_ck, nml_catch_ck,
	} ;

	return catch_ck_jmp[ pl_w->pl_stance ]( pl_w ) ;
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
	if( pl_w->atck_pk == 1 && pl_w->atck_level == 1 ){
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
static void jump_p_set( PLAYER_WORK *pl_w );
static void jump_k_set( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Start the throw that pl12_catch_ck chose			*/
/****************************************************************/
void pl12_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_set, nml_k_set, jump_p_set, jump_k_set,
	} ;

	catch_set_jmp[ pl_w->catch_no & CATCH_NO_MASK ]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	if( pl_w->atck_lever & KEY_BACK ){
		pl_w->pl_dir = pl_w->pl_dir ^ 1 ;
		pl_w->disp_dir = pl_w->disp_dir ^ 1 ;
	}
	ofs.x = -DOT_X( 50.0f ) ;
	ofs.y = DOT_Y( 74.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	scr_excep_set( pl_w, 5, 5 ) ;
	catch_gauge_add( pl_w ) ;
	pls12_00_set( pl_w, 0 ) ;
	em = pl_w->catch_w ;
	pl_char_set( em, Char_Damage, 5 ) ;
	pl_char_set( pl_w, Char_Catch, 0 ) ;
}

/*==============================================================*/
static void nml_k_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_w->atck_lever & KEY_BACK ){
		pl_w->pl_dir = pl_w->pl_dir ^ 1 ;
		pl_w->disp_dir = pl_w->disp_dir ^ 1 ;
	}
	ofs.x = -DOT_X( 50.0f ) ;
	ofs.y = DOT_Y( 74.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	scr_excep_set( pl_w, 5, 5 ) ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 1 ) ;
}

/*==============================================================*/
static void jump_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_w->atck_lever & KEY_BACK ){
		pl_w->pl_dir = pl_w->pl_dir ^ 1 ;
		pl_w->disp_dir = pl_w->disp_dir ^ 1 ;
	}
	ofs.x = -DOT_X( 50.0f ) ;
	ofs.y = DOT_Y( 74.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	scr_excep_set( pl_w, 5, 5 ) ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 2 ) ;
}

/*==============================================================*/
static void jump_k_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	ofs.x = -DOT_X( 50.0f ) ;
	ofs.y = DOT_Y( 74.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	scr_excep_set( pl_w, 5, 5 ) ;
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
void pl12_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_catch, nml_k_catch, jump_p_catch, jump_k_catch,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[ pl_w->catch_no & CATCH_NO_MASK ]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	if( pl_char_move( pl_w ) >= 0 ){
		if( pl_w->cg_type == 6 ){
			SCR_EXCEP_SET ;
			ofs.x = -DOT_X( 80.0f ) ;
			ofs.y = DOT_Y( 64.0f ) ;
			hit_mark_set( pl_w, &ofs, 15 ) ;
			pl_se_req( pl_w, 6 ) ;
			em = pl_w->catch_w ;
			em->dm_hit_w = pl_w ;
			pl_w->pl_dir = pl_w->disp_dir ;
			em->disp_dir = pl_w->pl_dir ^ 1 ;
			em->pl_dir = em->disp_dir ;
			em->atck_no = pl12_at_throw_p ;
			em->catch_type = 1 ;
		}
		return ;
	}
	pl_stand_ret( pl_w ) ;
}

static void nk_move( PLAYER_WORK *pl_w );
static void nk_up( PLAYER_WORK *pl_w );
static void nk_fall( PLAYER_WORK *pl_w );
static void nk_land( PLAYER_WORK *pl_w );

/*==============================================================*/
static void nml_k_catch( PLAYER_WORK *pl_w )
{
	static void ( *nk_jmp[])( PLAYER_WORK *pl_w ) = {
		nk_move, nk_up, nk_fall, nk_land,
	} ;

	pl_w->catch_flag = 1 ;
	nk_jmp[ pl_w->routine_2 ]( pl_w ) ;
}

/*==============================================================*/
static void nk_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type != 2 ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_Y( 6.0f ) : -DOT_Y( 6.0f ) ;
		pl_w->acc.x = pl_w->pl_dir ? -DOT_Y( 0.125f ) : DOT_Y( 0.125f ) ;
		pl_w->spd.y = DOT_Y( 13.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.5f ) ;
	}
}

/*--------------------------------------------------------------*/
static void nk_up( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->spd.y < 0.0f ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->acc.y = -DOT_Y( 0.75f ) ;
	}
	if( pl_w->cg_type != 1 ) pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void nk_fall( PLAYER_WORK *pl_w )
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
		pl_w->spd.x = pl_w->pl_dir ? -DOT_Y( 6.0f ) : DOT_Y( 6.0f ) ;
		pl_w->pos.y = pl_w->ground_y ;
	}
}

/*--------------------------------------------------------------*/
static void nk_land( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_w->cg_type == 2 ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
	if( pl_w->cg_type == 1 ){
		SCR_EXCEP_SET ;
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->pl_dir = pl_w->pl_dir ;
		em->atck_no = pl12_at_throw_k ;
		em->catch_type = 1 ;
		scr_work.quake_level = 2 ;
		scr_work.quake_flag = 1 ;
		catch_se_req( em ) ;
		return ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void jp_move( PLAYER_WORK *pl_w );
static void jp_fall( PLAYER_WORK *pl_w );
static void jp_land( PLAYER_WORK *pl_w );

/*==============================================================*/
static void jump_p_catch( PLAYER_WORK *pl_w )
{
	static void ( *jp_jmp[])( PLAYER_WORK *pl_w ) = {
		jp_move, jp_fall, jp_land,
	} ;

	pl_w->catch_flag = 1 ;
	jp_jmp[ pl_w->routine_2 ]( pl_w ) ;
}

/*==============================================================*/
static void jp_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->acc.y = -DOT_Y( 0.75f ) ;
	}
}

/*--------------------------------------------------------------*/
static void jp_fall( PLAYER_WORK *pl_w )
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
		pl_w->spd.x = pl_w->pl_dir ? -DOT_Y( 6.0f ) : DOT_Y( 6.0f ) ;
		pl_w->pos.y = pl_w->ground_y ;
	}
}

/*--------------------------------------------------------------*/
static void jp_land( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	if( pl_w->cg_type != 2 ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
	if( pl_w->cg_type == 1 ){
		SCR_EXCEP_SET ;
		ofs.x = 0.0f ;
		ofs.y = 0.0f ;
		hit_mark_set1( pl_w, &ofs, 2 ) ;
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->pl_dir = pl_w->pl_dir ;
		em->atck_no = pl12_at_air_throw_p ;
		em->catch_type = 3 ;
		scr_work.quake_level = 2 ;
		scr_work.quake_flag = 1 ;
		catch_se_req( em ) ;
		return ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
static void jump_k_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	if( pl_char_move( pl_w ) >= 0 ){
		if( pl_w->cg_type ){
			ofs.x = -DOT_X( 128.0f ) ;
			ofs.y = DOT_Y( 88.0f ) ;
			hit_mark_set( pl_w, &ofs, 2 ) ;
			pl_se_req( pl_w, 3 ) ;
			SCR_EXCEP_SET ;
			pl_w->cg_type = 0 ;
			em = pl_w->catch_w ;
			em->dm_hit_w = pl_w ;
			em->pl_dir = pl_w->pl_dir ;
			em->atck_no = pl12_at_air_throw_k ;
			em->catch_type = 1 ;
		}
	} else {
		pl_w->pos.x += pl_w->disp_dir ? DOT_X( 64.0f ) : -DOT_X( 64.0f ) ;
		pl_stand_ret( pl_w ) ;
	}
}

static void hold( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Holding the opponent								*/
/****************************************************************/
void pl12_hold( PLAYER_WORK *pl_w )
{
	static void ( *hold_jmp[])( PLAYER_WORK *pl_w ) = {
		hold, hold, hold, hold, hold,
	} ;

	hold_jmp[ pl_w->catch_no & CATCH_NO_MASK ]( pl_w ) ;
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
void pl12_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl12_sp_rasetsu ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl12_sp_engetsu ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl12_sp_engetsu ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl12_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl12_sp_rasetsu ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl12_sp_engetsu ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl12_sp_engetsu ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl12_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl12_sp_shiden ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl12_sp_guren ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl12_sp_shiden ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void pl12_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl12_sp_shiden ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl12_sp_counter ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl12_sp_counter ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl12.c" End of File	*/

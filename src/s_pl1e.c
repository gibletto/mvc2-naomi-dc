/* Akuma (character 1e): command checks, normal attacks, dashes, appearance and win poses, special
   moves and hyper combos, throws, partner moves and the wall kick. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl1e_def.h"

void pl1e_main( PLAYER_WORK *pl_w );
void pl1e_com_ck( PLAYER_WORK *pl_w );
void pl1e_atck_set( PLAYER_WORK *pl_w );
void pl1e_atck( PLAYER_WORK *pl_w );
void pl1e_dash( PLAYER_WORK *pl_w );
void pl1e_noki( PLAYER_WORK *pl_w );
void pl1e_appear( PLAYER_WORK *pl_w );
void pl1e_win( PLAYER_WORK *pl_w );
void pl1e_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl1e_catch_ck( PLAYER_WORK *pl_w );
void pl1e_catch_set( PLAYER_WORK *pl_w );
void pl1e_catch( PLAYER_WORK *pl_w );
void pl1e_hold( PLAYER_WORK *pl_w );
int pl1e_delay_ck( PLAYER_WORK *pl_w );
void pl1e_t_kick( PLAYER_WORK *pl_w );
void pl1e_upper( PLAYER_WORK *pl_w );
void pl1e_sp_unison( PLAYER_WORK *pl_w );
void pl1e_sp_gattai( PLAYER_WORK *pl_w );
void pl1e_sp_assist( PLAYER_WORK *pl_w );
void pl1e_sp_zero( PLAYER_WORK *pl_w );

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
static Uint16 com_hadou[] = { 3, COM_GROUND, pl1e_sp_hadou, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ; /* Gou Hadouken */
static Uint16 com_shouryuu[] = { 3, COM_GROUND, pl1e_sp_shouryuu, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_PUNCH } ; /* Gou Shouryuuken */
static Uint16 com_tatsumaki[] = { 3, COM_GROUND, pl1e_sp_tatsumaki, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ; /* Tatsumaki Zankuukyaku */
static Uint16 com_jtatsumaki[] = { 3, COM_AIR, pl1e_sp_jtatsumaki, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ; /* Tatsumaki Zankuukyaku in the air */
static Uint16 com_zankuu[] = { 3, COM_AIR, pl1e_sp_zankuu, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ; /* Zankuu Hadouken */
static Uint16 com_ashura[] = { 3, COM_GROUND, pl1e_sp_ashura, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_ATCK } ; /* Ashura Senkuu forward */
static Uint16 com_ashura_b[] = { 3, COM_GROUND, pl1e_sp_ashura_b, 0, KEY_BACK, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_ATCK } ; /* Ashura Senkuu back */
static Uint16 com_kuujin[] = { 3, COM_AIR, pl1e_sp_kuujin, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ; /* Tenma Kuujinkyaku */
static Uint16 com_gou_hadou[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl1e_sp_gou_hadou, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ; /* Messatsu Gou Hadou */
static Uint16 com_gou_shouryuu[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl1e_sp_gou_shouryuu, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ; /* Messatsu Gou Shouryuu */
static Uint16 com_tenma[] = { 3, COM_AIR | COM_HYPER | COM_ALL_SW, pl1e_sp_tenma, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ; /* Tenma Gou Zankuu */
static Uint16 com_shungoku[] = { 4, COM_GROUND | COM_HYPER, pl1e_sp_shungoku, 0, KEY_LP, KEY_LP, KEY_FORWARD, KEY_LK, KEY_HP } ; /* Shun Goku Satsu */
static Uint16 com_catch[] = { 5, COM_GROUND, 0, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_HK } ; /* Jigoku Guruma */
static Uint16 com_gou_rasen[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl1e_sp_gou_rasen, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ; /* Messatsu Gou Rasen */

static void ( *pl1e_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl1e_main,
	pl1e_com_ck,
	pl1e_atck_set,
	pl1e_atck,
	pl1e_dash,
	pl1e_noki,
	pl1e_appear,
	pl1e_win,
	pl1e_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl1e_catch_ck,	/* returns a value */
	pl1e_catch_set,
	pl1e_catch,
	pl1e_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl1e_delay_ck,
	pl1e_t_kick,
	pl1e_upper,
	pl1e_sp_unison,
	pl1e_sp_gattai,
	pl1e_sp_assist,
	pl1e_sp_zero,
} ;

static Uint8 hadou_ck( PLAYER_WORK *pl_w );
static Uint8 shouryuu_ck( PLAYER_WORK *pl_w );
static Uint8 tatsumaki_ck( PLAYER_WORK *pl_w );
static Uint8 catch_com_ck( PLAYER_WORK *pl_w );
static Uint8 jtatsumaki_ck( PLAYER_WORK *pl_w );
static Uint8 zankuu_ck( PLAYER_WORK *pl_w );
static Uint8 ashura_ck( PLAYER_WORK *pl_w );
static Uint8 ashura_b_ck( PLAYER_WORK *pl_w );
static Uint8 kuujin_ck( PLAYER_WORK *pl_w );
static Uint8 gou_hadou_ck( PLAYER_WORK *pl_w );
static Uint8 gou_shouryuu_ck( PLAYER_WORK *pl_w );
static Uint8 tenma_ck( PLAYER_WORK *pl_w );
static Uint8 shungoku_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static int chouhatsu_ck( PLAYER_WORK *pl_w );
static Uint8 gou_rasen_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl1e_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl1e_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl1e_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( shungoku_ck( pl_w ) ) return ;
	if( ashura_ck( pl_w ) ) return ;
	if( ashura_b_ck( pl_w ) ) return ;
	if( gou_shouryuu_ck( pl_w ) ) return ;
	if( shouryuu_ck( pl_w ) ) return ;
	if( gou_hadou_ck( pl_w ) ) return ;
	if( tenma_ck( pl_w ) ) return ;
	if( catch_com_ck( pl_w ) ) return ;
	if( zankuu_ck( pl_w ) ) return ;
	if( hadou_ck( pl_w ) ) return ;
	if( tatsumaki_ck( pl_w ) ) return ;
	if( jtatsumaki_ck( pl_w ) ) return ;
	if( kuujin_ck( pl_w ) ) return ;
	if( gou_rasen_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
/*			On the ground, only while shot_flag is clear		*/
/*==============================================================*/
static Uint8 hadou_ck( PLAYER_WORK *pl_w )
{
	PL1E_LOCAL_W *lw = ( PL1E_LOCAL_W * )pl_w->tlocal ;

	if( !com_nml_ck( pl_w, com_hadou, &pl_w->com_w ) ) return 0 ;
	if( lw->shot_flag ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1e_sp_hadou ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 shouryuu_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_shouryuu, &pl_w->com_w0 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->pl_sp_type = pl1e_sp_shouryuu ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 tatsumaki_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_tatsumaki, &pl_w->com_w1 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->pl_sp_type = pl1e_sp_tatsumaki ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
/*			Jigoku Guruma, the command throw, when the			*/
/*			opponent is in reach								*/
/*==============================================================*/
static Uint8 catch_com_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( !com_nml_ck( pl_w, com_catch, &pl_w->com_w11 ) ) return 0 ;
	if( !( em = pl_catch_ck( pl_w ) ) ) return 0 ;
	pl_w->catch_no = 0xc4 ;
	pl_catch_set( pl_w, em ) ;
	return 1 ;
}

/*==============================================================*/
/*			In the air: once a jump, unless super jumping		*/
/*==============================================================*/
static Uint8 jtatsumaki_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_jtatsumaki, &pl_w->com_w2 ) ) return 0 ;
	if( pl_w->spd.y < 0.0f && pl_w->pos.y - pl_w->ground_y < DOT_Y( 48.0f ) ) return 0 ;
	if( !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->pl_sp_type = pl1e_sp_jtatsumaki ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 zankuu_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_zankuu, &pl_w->com_w3 ) ) return 0 ;
	if( !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w3 ) ;
	pl_w->pl_sp_type = pl1e_sp_zankuu ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
/*			The teleports: any punch or kick, and they drop		*/
/*			the hyper combos' commands							*/
/*==============================================================*/
static Uint8 ashura_ck( PLAYER_WORK *pl_w )
{
	Uint16 sw ;

	if( !com_nml_ck( pl_w, com_ashura, &pl_w->com_w5 ) ) return 0 ;
	if( !sw_on_ck( pl_w, 0x0300, &sw ) && !sw_on_ck( pl_w, 0x0060, &sw ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w5 ) ;
	memset( &pl_w->com_w4, 0, 8 ) ;
	memset( &pl_w->com_w9, 0, 8 ) ;
	memset( &pl_w->com_w12, 0, 8 ) ;
	pl_w->pl_sp_type = pl1e_sp_ashura ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 ashura_b_ck( PLAYER_WORK *pl_w )
{
	Uint16 sw ;

	if( !com_nml_ck( pl_w, com_ashura_b, &pl_w->com_w6 ) ) return 0 ;
	if( !sw_on_ck( pl_w, 0x0300, &sw ) && !sw_on_ck( pl_w, 0x0060, &sw ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w6 ) ;
	memset( &pl_w->com_w4, 0, 8 ) ;
	memset( &pl_w->com_w9, 0, 8 ) ;
	memset( &pl_w->com_w12, 0, 8 ) ;
	pl_w->pl_sp_type = pl1e_sp_ashura_b ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
/*			In the air, high enough above the floor				*/
/*==============================================================*/
static Uint8 kuujin_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_kuujin, &pl_w->com_w10 ) ) return 0 ;
	if( !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	if( pl_w->pos.y - pl_w->ground_y < DOT_Y( 48.0f ) ) return 0 ;
	if( pl_w->spd.y < 0.0f && pl_w->pos.y - pl_w->ground_y < DOT_Y( 48.0f ) ) return 1 ;
	com_sw_set( pl_w, &pl_w->com_w10 ) ;
	pl_w->pl_sp_type = pl1e_sp_kuujin ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 gou_hadou_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_gou_hadou, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1e_sp_gou_hadou ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 gou_shouryuu_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_gou_shouryuu, &pl_w->com_w9 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1e_sp_gou_shouryuu ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 tenma_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_tenma, &pl_w->com_w7 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1e_sp_tenma ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
/*			A level 3 hyper combo, done by a run of buttons		*/
/*==============================================================*/
static Uint8 shungoku_ck( PLAYER_WORK *pl_w )
{
	if( !com_seq_ck( pl_w, com_shungoku, &pl_w->com_w8 ) ) return 0 ;
	if( *pl_w->gauge_level < 3 ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1e_sp_shungoku ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static int snapback_ck( PLAYER_WORK *pl_w )
{
	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl1e_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static int chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 15 ) ) return 0 ;
	pl_w->pl_sp_type = pl1e_sp_chouhatsu ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 gou_rasen_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_gou_rasen, &pl_w->com_w12 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1e_sp_gou_rasen ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/****************************************************************/
void pl1e_main( PLAYER_WORK *pl_w )
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
void pl1e_atck_set( PLAYER_WORK *pl_w )
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
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_STAND_HP ;
		pl_se_req( pl_w, SE_SWING_H ) ;
		pl_w->routine_2 = 0 ;
		if( pl_w->atck_lever & KEY_FORWARD ){
			pl_w->routine_2 = 1 ;
			pl_w->char_no.b.grp = 20 ;
			pl_w->char_no.b.no = 0 ;
			pl_w->atck_no = pl1e_at_fwd_hp_1 ;
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
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_STAND_HK ;
		pl_se_req( pl_w, SE_SWING_H ) ;
		pl_w->routine_2 = 0 ;
		if( pl_w->atck_lever & KEY_FORWARD ){
			pl_w->routine_2 = 1 ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_w->char_no.b.grp = 20 ;
			pl_w->char_no.b.no = 1 ;
			pl_w->atck_no = pl1e_at_fwd_hk ;
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
		pl_w->char_no.b.no = 3 ;
		pl_w->atck_no = pl1e_at_jump_lk ;
		pl_w->char_no.b.grp = 20 ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_lk_tbl ;
		else pl_w->chain_tbl = sj_lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->char_no.b.grp = 20 ;
		pl_w->char_no.b.no = 4 ;
		pl_w->atck_no = pl1e_at_jump_mk ;
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
		pl_w->char_no.b.no = 5 ;
		pl_w->atck_no = pl1e_at_jump_hk ;
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
void pl1e_atck( PLAYER_WORK *pl_w )
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
				pl_w->atck_no = pl1e_at_fwd_hp_2 ;
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
			pl_w->atck_no = pl1e_at_stand_mk_2 ;
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
				pl_w->spd.x = pl_w->pl_dir ? DOT_X( 4.0f ) : -DOT_X( 4.0f ) ;
				pl_w->acc.x = 0.0f ;
				pl_w->spd.y = DOT_Y( 4.25f ) ;
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
void pl1e_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_w->atck_no = pl1e_at_upper ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_char_set( pl_w, Char_Extra, 12 ) ;
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
static void dash_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The dash: a hop forward								*/
/****************************************************************/
void pl1e_dash( PLAYER_WORK *pl_w )
{
	static void ( *dash_jmp[])( PLAYER_WORK *pl_w ) = {
		dash_start, dash_move, dash_end,
	} ;

	dash_jmp[pl_w->routine_2]( pl_w ) ;
	pl_smoke_set( pl_w ) ;
}

/*==============================================================*/
static void dash_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? 20.0f : -20.0f ;
		pl_w->acc.x = pl_w->pl_dir ? -DOT_X( 0.375f ) : DOT_X( 0.375f ) ;
	}
}

/*==============================================================*/
/*			Until the speed turns round, then down on the		*/
/*			floor												*/
/*==============================================================*/
static void dash_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->acc.x * pl_w->spd.x > 0.0f ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_char_set( pl_w, Char_Dash, 2 ) ;
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
static void noki_start( PLAYER_WORK *pl_w );
static void noki_move( PLAYER_WORK *pl_w );
static void noki_stop( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash										*/
/****************************************************************/
void pl1e_noki( PLAYER_WORK *pl_w )
{
	static void ( *noki_jmp[])( PLAYER_WORK *pl_w ) = {
		noki_init, noki_start, noki_move, noki_stop,
	} ;

	noki_jmp[pl_w->routine_2]( pl_w ) ;
	pl_smoke_set( pl_w ) ;
}

/*==============================================================*/
static void noki_init( PLAYER_WORK *pl_w )
{
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->routine_2 ++ ;
	pl_w->spd.x = pl_w->pl_dir ? -20.0f : 20.0f ;
	pl_w->acc.x = pl_w->pl_dir ? DOT_X( 0.375f ) : -DOT_X( 0.375f ) ;
	pl_char_set( pl_w, Char_Dash, 1 ) ;
}

/*--------------------------------------------------------------*/
static void noki_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
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
	if( pl_w->acc.x * pl_w->spd.x > 0.0f ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Dash, 3 ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void noki_stop( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void appear_init( PLAYER_WORK *pl_w );
static void appear_move( PLAYER_WORK *pl_w );
static void win_init( PLAYER_WORK *pl_w );
static void win_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl1e_appear( PLAYER_WORK *pl_w )
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

/*==============================================================*/
/*			An effect on the animation's mark					*/
/*==============================================================*/
static void appear_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w -> routine_1 ++ ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pls1e_03_set( pl_w, 0 ) ;
	}
}

/* the win poses for a KO, picked at random, and the pose for each other end of a round */
static Sint8 win_ko_tbl[4] = { 0, 1, 2, 1 } ;
static Sint8 win_tbl[5] = { 4, 4, 3, 4, 4 } ;

/****************************************************************/
void pl1e_win( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ) win_init( pl_w ) ;
	else win_move( pl_w ) ;
}

/*==============================================================*/
/*			The pose picked for a KO is kept in timer0			*/
/*==============================================================*/
static void win_init( PLAYER_WORK *pl_w )
{
	Uint8 no ;

	pl_w->routine_2 ++ ;
	no = win_tbl[pl_w->type] ;
	if( !pl_w->type ){
		pl_w->timer0 = Rnd() & 3 ;
		no = win_ko_tbl[pl_w->timer0] ;
	}
	pl_char_set( pl_w, Char_Win, no ) ;
}

/*==============================================================*/
/*			Two of the KO poses have an effect on the			*/
/*			animation's mark									*/
/*==============================================================*/
static void win_move( PLAYER_WORK *pl_w )
{
	int pose ;

	if( pl_win_end_ck( pl_w ) ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
	if( pl_w->type ) return ;
	pose = pl_w->timer0 ;
	if( ( pose == 1 || pose == 3 ) && pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pls1e_03_set( pl_w, 1 ) ;
	}
}

static void sp_hadou( PLAYER_WORK *pl_w );
static void sp_shouryuu( PLAYER_WORK *pl_w );
static void sp_tatsumaki( PLAYER_WORK *pl_w );
static void sp_jtatsumaki( PLAYER_WORK *pl_w );
static void sp_zankuu( PLAYER_WORK *pl_w );
static void sp_tenma( PLAYER_WORK *pl_w );
static void sp_ashura( PLAYER_WORK *pl_w );
static void sp_ashura_b( PLAYER_WORK *pl_w );
static void sp_shungoku( PLAYER_WORK *pl_w );
static void sp_gou_shouryuu( PLAYER_WORK *pl_w );
static void sp_gou_hadou( PLAYER_WORK *pl_w );
static void sp_kuujin( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );
static void sp_gou_rasen( PLAYER_WORK *pl_w );

/****************************************************************/
void	pl1e_sp_atck( PLAYER_WORK *pl_w )
{
	static void (*pl_sp_tbl[])( PLAYER_WORK *pl_w ) ={
		sp_hadou,					/* 00 Gou Hadouken */
		sp_shouryuu,					/* 01 Gou Shouryuuken */
		sp_tatsumaki,					/* 02 Tatsumaki Zankuukyaku */
		sp_jtatsumaki,					/* 03 Tatsumaki Zankuukyaku in the air */
		sp_zankuu,					/* 04 Zankuu Hadouken */
		sp_tenma,					/* 05 Tenma Gou Zankuu (air hyper) */
		sp_ashura,					/* 06 Ashura Senkuu forward */
		sp_ashura_b,					/* 07 Ashura Senkuu back */
		sp_shungoku,					/* 08 Shun Goku Satsu (hyper) */
		sp_hadou,					/* 09 */
		sp_hadou,					/* 0a */
		sp_gou_shouryuu,					/* 0b Messatsu Gou Shouryuu (hyper) */
		sp_hadou,					/* 0c */
		sp_gou_hadou,					/* 0d Messatsu Gou Hadou (hyper) */
		sp_kuujin,					/* 0e Tenma Kuujinkyaku */
		sp_chouhatsu,					/* 0f taunt */
		sp_snapback,					/* 10 Snap Back */
		sp_gou_rasen,					/* 11 Messatsu Gou Rasen (hyper) */
	} ;

	pl_sp_tbl[ pl_w->pl_sp_type ]( pl_w ) ;
}

/*==============================================================*/
/*			A hit set by the animation, numbered in cg_flag1	*/
/*==============================================================*/
static void cg_hit( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_flag1 ){
		pl_w->atck_no = pl_w->cg_flag1 ;
		HIT_RESET( pl_w ) ;
		pl_w->cg_flag1 = 0 ;
	}
}

static void hadou_init( PLAYER_WORK *pl_w, PL1E_LOCAL_W *lw );
static void hadou_move( PLAYER_WORK *pl_w, PL1E_LOCAL_W *lw );

/*==============================================================*/
/*			A shot along the ground								*/
/*==============================================================*/
static void sp_hadou( PLAYER_WORK *pl_w )
{
	static void ( *hadou_jmp[])( PLAYER_WORK *pl_w, PL1E_LOCAL_W *lw ) = {
		hadou_init, hadou_move,
	} ;

	hadou_jmp[pl_w->routine_2]( pl_w, ( PL1E_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void hadou_init( PLAYER_WORK *pl_w, PL1E_LOCAL_W *lw )
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
static void hadou_move( PLAYER_WORK *pl_w, PL1E_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shl1e_00_set( pl_w, 0 ) ;
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
static void shouryuu_hit( PLAYER_WORK *pl_w, PL1E_LOCAL_W *lw )
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

static void shouryuu_init( PLAYER_WORK *pl_w, PL1E_LOCAL_W *lw );
static void shouryuu_start( PLAYER_WORK *pl_w, PL1E_LOCAL_W *lw );
static void shouryuu_jump( PLAYER_WORK *pl_w, PL1E_LOCAL_W *lw );
static void shouryuu_fall( PLAYER_WORK *pl_w, PL1E_LOCAL_W *lw );
static void shouryuu_land( PLAYER_WORK *pl_w, PL1E_LOCAL_W *lw );

/* hits for each strength, counted apart by the side of hit_flag (hit_cnt, guard_cnt) */
static Uint8 shouryuu_hits[4] = { 1, 1, 3, 3 } ;

/*==============================================================*/
/*			A rising uppercut									*/
/*==============================================================*/
static void sp_shouryuu( PLAYER_WORK *pl_w )
{
	static void ( *shouryuu_jmp[])( PLAYER_WORK *pl_w, PL1E_LOCAL_W *lw ) = {
		shouryuu_init, shouryuu_start, shouryuu_jump, shouryuu_fall, shouryuu_land,
	} ;

	shouryuu_jmp[pl_w->routine_2]( pl_w, ( PL1E_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void shouryuu_init( PLAYER_WORK *pl_w, PL1E_LOCAL_W *lw )
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
	pl_w->acc.x = pl_w->pl_dir ? -DOT_X( 0.5f ) : DOT_X( 0.5f ) ;
	pl_w->acc.y = -DOT_Y( 0.5f ) ;
	pl_w->atck_no = pl_w->atck_level + 51 ;
	HIT_RESET( pl_w ) ;
	lw->hit_cnt = shouryuu_hits[pl_w->atck_level * 2] ;
	lw->guard_cnt = shouryuu_hits[pl_w->atck_level * 2 + 1] ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 2 ) ;
}

/*--------------------------------------------------------------*/
static void shouryuu_start( PLAYER_WORK *pl_w, PL1E_LOCAL_W *lw )
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
static void shouryuu_jump( PLAYER_WORK *pl_w, PL1E_LOCAL_W *lw )
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
static void shouryuu_fall( PLAYER_WORK *pl_w, PL1E_LOCAL_W *lw )
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
static void shouryuu_land( PLAYER_WORK *pl_w, PL1E_LOCAL_W *lw )
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
static void tatsumaki_fall( PLAYER_WORK *pl_w );
static void tatsumaki_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A spinning kick that travels forward				*/
/*==============================================================*/
static void sp_tatsumaki( PLAYER_WORK *pl_w )
{
	static void ( *tatsumaki_jmp[])( PLAYER_WORK *pl_w ) = {
		tatsumaki_init, tatsumaki_rise, tatsumaki_spin, tatsumaki_fall, tatsumaki_land,
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
	pl_gauge_add( pl_w, 8 ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->atck_no = pl_w->atck_level + 58 ;
	HIT_RESET( pl_w ) ;
	pl_w->spd.y = DOT_Y( 3.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_air_set( pl_w ) ;
	spd = pl_w->atck_level ? 6.25f : DOT_X( 2.5f ) ;
	pl_w->spd.x = pl_w->pl_dir ? spd : -spd ;
	pl_w->acc.x = 0.0f ;
	pl_char_set( pl_w, Char_Special, 4 ) ;
}

/*==============================================================*/
/*			Spins for one more turn (timer0) at each			*/
/*			strength											*/
/*==============================================================*/
static void tatsumaki_rise( PLAYER_WORK *pl_w )
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
	pls1e_04_set( pl_w, 0 ) ;
	pl_w->timer0 = pl_w->atck_level * 2 + 1 ;
	pl_char_set( pl_w, Char_Special, 6 ) ;
	tatsumaki_hit( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void tatsumaki_spin( PLAYER_WORK *pl_w )
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
	tatsumaki_hit( pl_w ) ;
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
	pls1e_04_set( pl_w, 0 ) ;
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
static void zankuu_xy_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ) pl_w->pos.y = pl_w->ground_y ;
}

static void zankuu_init( PLAYER_WORK *pl_w, PL1E_LOCAL_W *lw );
static void zankuu_move( PLAYER_WORK *pl_w, PL1E_LOCAL_W *lw );
static void zankuu_end( PLAYER_WORK *pl_w, PL1E_LOCAL_W *lw );

/*==============================================================*/
/*			The shot thrown from a jump							*/
/*==============================================================*/
static void sp_zankuu( PLAYER_WORK *pl_w )
{
	static void ( *zankuu_jmp[])( PLAYER_WORK *pl_w, PL1E_LOCAL_W *lw ) = {
		zankuu_init, zankuu_move, zankuu_end,
	} ;

	zankuu_jmp[pl_w->routine_2]( pl_w, ( PL1E_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
/*			The jump slows down while the shot is thrown		*/
/*==============================================================*/
static void zankuu_init( PLAYER_WORK *pl_w, PL1E_LOCAL_W *lw )
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
static void zankuu_move( PLAYER_WORK *pl_w, PL1E_LOCAL_W *lw )
{
	zankuu_xy_move( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		shl1e_00_set( pl_w, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void zankuu_end( PLAYER_WORK *pl_w, PL1E_LOCAL_W *lw )
{
	zankuu_xy_move( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_jump_ret( pl_w ) ;
	}
}

static void tenma_init( PLAYER_WORK *pl_w );
static void tenma_wait( PLAYER_WORK *pl_w );
static void tenma_shot( PLAYER_WORK *pl_w );
static void tenma_beam( PLAYER_WORK *pl_w );
static void tenma_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo: shots thrown down at a slant from	*/
/*			a jump												*/
/*==============================================================*/
static void sp_tenma( PLAYER_WORK *pl_w )
{
	static void ( *tenma_jmp[])( PLAYER_WORK *pl_w ) = {
		tenma_init, tenma_wait, tenma_shot, tenma_beam, tenma_end,
	} ;

	tenma_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			Moves, and stops on the floor						*/
/*==============================================================*/
static void tenma_xy_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
}

/*==============================================================*/
/*			The jump slows almost to a stop						*/
/*==============================================================*/
static void tenma_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_w->spd.x /= 16.0f ;
	pl_w->spd.y /= 16.0f ;
	pl_w->acc.y /= 64.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->atck_no = pl1e_at_tenma ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 3 ) ;
}

/*--------------------------------------------------------------*/
static void tenma_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = -DOT_X( 56.0f ) ;
		ofs.y = DOT_Y( 47.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*==============================================================*/
/*			The shots last timer1 frames, one every timer0		*/
/*			frames												*/
/*==============================================================*/
static void tenma_shot( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 6 ;
		pl_w->timer1 = 80 ;
		pl_char_set( pl_w, Char_Super, 4 ) ;
	}
}

/*--------------------------------------------------------------*/
static void tenma_beam( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	tenma_xy_move( pl_w ) ;
	if( -- pl_w->timer1 <= 0 ){
		pl_w->routine_2 ++ ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_char_set( pl_w, Char_Super, 5 ) ;
	} else if( -- pl_w->timer0 <= 0 ){
		pl_w->timer0 = 4 ;
		shl1e_02_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void tenma_end( PLAYER_WORK *pl_w )
{
	tenma_xy_move( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

static void ashura_init( PLAYER_WORK *pl_w );
static void ashura_move( PLAYER_WORK *pl_w );
static void ashura_stop( PLAYER_WORK *pl_w );
static void ashura_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A fast glide forward, leaving a trail				*/
/*==============================================================*/
static void sp_ashura( PLAYER_WORK *pl_w )
{
	static void ( *ashura_jmp[])( PLAYER_WORK *pl_w ) = {
		ashura_init, ashura_move, ashura_stop, ashura_end,
	} ;

	ashura_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			The trail behind the slide, every third frame		*/
/*==============================================================*/
static void ashura_ef_set( PLAYER_WORK *pl_w )
{
	if( -- pl_w->timer0 < 0 ){
		pl_w->timer0 = 2 ;
		pls1e_02_set( pl_w ) ;
	}
}

/*==============================================================*/
static void ashura_init( PLAYER_WORK *pl_w )
{
	Float spd, acc ;

	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_gauge_add( pl_w, 2 ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->timer0 = 0 ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	spd = pl_w->atck_pk ? DOT_X( 10.0f ) : 25.0f ;
	acc = pl_w->atck_pk ? -0.6770833f : -0.78125f ;
	pl_w->spd.x = pl_w->pl_dir ? spd : -spd ;
	pl_w->acc.x = pl_w->pl_dir ? acc : -acc ;
	pl_char_set( pl_w, Char_Special, 21 ) ;
}

/*==============================================================*/
/*			The animation moves the player cg_prm steps		*/
/*			through the slide									*/
/*==============================================================*/
static void ashura_move( PLAYER_WORK *pl_w )
{
	Float x ;

	pl_dir_set( pl_w ) ;
	ashura_ef_set( pl_w ) ;
	pl_w->push_ng_timer = 2 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_prm ){
		x = pl_w->cg_prm ;
		x *= 1.6666666f ;
		pl_w->cg_prm = 0 ;
		pl_w->pos.x += pl_w->pl_dir ? -x : x ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
	}
}

/*==============================================================*/
/*			Glides until it stops or meets the wall behind		*/
/*==============================================================*/
static void ashura_stop( PLAYER_WORK *pl_w )
{
	pl_dir_set( pl_w ) ;
	ashura_ef_set( pl_w ) ;
	pl_w->push_ng_timer = 2 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->acc.x * pl_w->spd.x < 0.0f ){
		if( !( pl_w->wall_flag & ( 1 << ( pl_w->pl_dir ^ 1 ) ) ) ){
			pl_char_move( pl_w ) ;
			return ;
		}
	}
	pl_w->routine_2 ++ ;
	pl_char_set( pl_w, Char_Special, 22 ) ;
}

/*--------------------------------------------------------------*/
static void ashura_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void ashura_b_init( PLAYER_WORK *pl_w );
static void ashura_b_move( PLAYER_WORK *pl_w );
static void ashura_b_stop( PLAYER_WORK *pl_w );
static void ashura_b_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The same glide backward								*/
/*==============================================================*/
static void sp_ashura_b( PLAYER_WORK *pl_w )
{
	static void ( *ashura_b_jmp[])( PLAYER_WORK *pl_w ) = {
		ashura_b_init, ashura_b_move, ashura_b_stop, ashura_b_end,
	} ;

	ashura_b_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void ashura_b_init( PLAYER_WORK *pl_w )
{
	Float spd, acc ;

	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_gauge_add( pl_w, 2 ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->timer0 = 0 ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	spd = pl_w->atck_pk ? -DOT_X( 10.0f ) : -DOT_X( 14.0f ) ;
	acc = pl_w->atck_pk ? 0.6770833f : 0.78125f ;
	pl_w->spd.x = pl_w->pl_dir ? spd : -spd ;
	pl_w->acc.x = pl_w->pl_dir ? acc : -acc ;
	pl_char_set( pl_w, Char_Special, 21 ) ;
}

/*--------------------------------------------------------------*/
static void ashura_b_move( PLAYER_WORK *pl_w )
{
	Float x ;

	pl_dir_set( pl_w ) ;
	ashura_ef_set( pl_w ) ;
	pl_w->push_ng_timer = 2 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_prm ){
		x = pl_w->cg_prm ;
		x *= 1.6666666f ;
		pl_w->cg_prm = 0 ;
		pl_w->pos.x += pl_w->pl_dir ? -x : x ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
	}
}

/*==============================================================*/
/*			Glides until it stops or meets the wall ahead		*/
/*==============================================================*/
static void ashura_b_stop( PLAYER_WORK *pl_w )
{
	pl_dir_set( pl_w ) ;
	ashura_ef_set( pl_w ) ;
	pl_w->push_ng_timer = 2 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->acc.x * pl_w->spd.x < 0.0f && !( pl_w->wall_flag & ( 1 << pl_w->pl_dir ) ) ){
		pl_char_move( pl_w ) ;
		return ;
	}
	pl_w->routine_2 ++ ;
	pl_char_set( pl_w, Char_Special, 22 ) ;
}

/*--------------------------------------------------------------*/
static void ashura_b_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void shungoku_init( PLAYER_WORK *pl_w );
static void shungoku_wait( PLAYER_WORK *pl_w );
static void shungoku_start( PLAYER_WORK *pl_w );
static void shungoku_run( PLAYER_WORK *pl_w );
static void shungoku_catch( PLAYER_WORK *pl_w );
static void shungoku_hits( PLAYER_WORK *pl_w );
static void shungoku_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo: glides in and, if it catches, a		*/
/*			flurry of hits										*/
/*==============================================================*/
static void sp_shungoku( PLAYER_WORK *pl_w )
{
	static void ( *shungoku_jmp[])( PLAYER_WORK *pl_w ) = {
		shungoku_init, shungoku_wait, shungoku_start, shungoku_run,
		shungoku_catch, shungoku_hits, shungoku_end,
	} ;

	shungoku_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void shungoku_init( PLAYER_WORK *pl_w )
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
	pl_sp_ef_set( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_w->spd.x = pl_w->pl_dir ? DOT_X( 3.5f ) : -DOT_X( 3.5f ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->timer0 = 40 ;
	pl_char_set( pl_w, Char_Super, 6 ) ;
}

/*--------------------------------------------------------------*/
static void shungoku_wait( PLAYER_WORK *pl_w )
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
		ofs.y = DOT_Y( 92.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shungoku_start( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->routine_2 ++ ;
	pl_char_set( pl_w, Char_Super, 9 ) ;
}

/*==============================================================*/
/*			Glides in; catches the opponent if it can			*/
/*==============================================================*/
static void shungoku_run( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( ( pl_w->wall_flag & ( 1 << ( pl_w->pl_dir ^ 1 ) ) ) || -- pl_w->timer0 <= 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( ( em = pl_catch_ck( pl_w ) ) ){
		pl_w->routine_2 ++ ;
		scr_excep_set( pl_w, 5, 5 ) ;
		pl_char_set( pl_w, Char_Catch, 2 ) ;
		ofs.x = -DOT_X( 50.0f ) ;
		ofs.y = DOT_Y( 74.0f ) ;
		nage_hit_set( pl_w, &ofs ) ;
		pl_w->timer0 = 16 ;
		pl_w->catch_no = 0xc2 ;
		pl_sp_catch_set( pl_w, em ) ;
	}
}

/*--------------------------------------------------------------*/
static void shungoku_catch( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	if( -- pl_w->timer0 <= 0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 64 ;
		pl_w->timer1 = 4 ;
		pl_char_set( pl_w, Char_Catch, 3 ) ;
		flash_dark_long_set() ;
		flash_work.timer = 2 ;
	}
}

/*==============================================================*/
/*			The flurry of hits, with sparks around him			*/
/*==============================================================*/
static void shungoku_hits( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 pos ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->catch_flag = 1 ;
	flash_work.timer = 2 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	if( -- pl_w->timer1 <= 0 ){
		pl_w->timer1 = 4 ;
		if( Rnd() & 1 ) pl_se_req( pl_w, 0 ) ;
		else pl_se_req( pl_w, 1 ) ;
		pos.x = ( Rnd() & 127 ) * 1.6666666f ;
		pos.y = ( Rnd() & 63 ) * DOT_Y( 1.0f ) ;
		if( Rnd() & 1 ) pos.x = -pos.x ;
		if( Rnd() & 1 ) pos.y = -pos.y ;
		pos.x += pl_w->pos.x ;
		pos.y += pl_w->pos.y + DOT_Y( 96.0f ) ;
		pos.z = pl_w->pos.z ;
		hitmark_set1( &pos, pl_w->disp_dir, 1, 0 ) ;
	}
	if( -- pl_w->timer0 <= 0 ){
		pl_w->routine_2 ++ ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		em = pl_w->catch_w ;
		combo_add( em, pl_w, 14 ) ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 8 ;
		em->pl_stance = STANCE_STAND ;
		scr_excep_set( pl_w, 0, 0 ) ;
		em->atck_no = pl1e_at_shungoku ;
		pl_w->timer0 = 64 ;
		pls1e_03_set( pl_w, 2 ) ;
		flash_work.timer = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shungoku_end( PLAYER_WORK *pl_w )
{
	pl_w->push_ng_timer = 2 ;
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 <= 0 ){
		pl_w->target_w->catch_ng_timer = 48 ;
		pl_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
/*			Falls, and falls faster once the jump turns over	*/
/*==============================================================*/
static void gou_shouryuu_y_move( PLAYER_WORK *pl_w )
{
	Float spd ;

	spd = pl_w->spd.y ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( spd * pl_w->spd.y < 0.0f ) pl_w->acc.y = -1.2053571f ;
}

/*==============================================================*/
/*			The same as cg_hit									*/
/*==============================================================*/
static void gou_shouryuu_cg_hit( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_flag1 ){
		pl_w->atck_no = pl_w->cg_flag1 ;
		HIT_RESET( pl_w ) ;
		pl_w->cg_flag1 = 0 ;
	}
}

static void gou_shouryuu_init( PLAYER_WORK *pl_w );
static void gou_shouryuu_wait( PLAYER_WORK *pl_w );
static void gou_shouryuu_start( PLAYER_WORK *pl_w );
static void gou_shouryuu_rush( PLAYER_WORK *pl_w );
static void gou_shouryuu_rise( PLAYER_WORK *pl_w );
static void gou_shouryuu_jump( PLAYER_WORK *pl_w );
static void gou_shouryuu_fall( PLAYER_WORK *pl_w );
static void gou_shouryuu_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo: a rush forward that ends in a		*/
/*			leap												*/
/*==============================================================*/
static void sp_gou_shouryuu( PLAYER_WORK *pl_w )
{
	static void ( *gou_shouryuu_jmp[])( PLAYER_WORK *pl_w ) = {
		gou_shouryuu_init, gou_shouryuu_wait, gou_shouryuu_start, gou_shouryuu_rush,
		gou_shouryuu_rise, gou_shouryuu_jump, gou_shouryuu_fall, gou_shouryuu_land,
	} ;

	gou_shouryuu_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void gou_shouryuu_init( PLAYER_WORK *pl_w )
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
	pl_w->atck_no = pl1e_at_gou_shouryuu ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 0 ) ;
}

/*--------------------------------------------------------------*/
static void gou_shouryuu_wait( PLAYER_WORK *pl_w )
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
		ofs.x = -25.0f ;
		ofs.y = DOT_Y( 47.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void gou_shouryuu_start( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->routine_2 ++ ;
	pl_char_set( pl_w, Char_Super, 7 ) ;
	gou_shouryuu_cg_hit( pl_w ) ;
}

/*==============================================================*/
/*			The animation marks a second push (2) and the		*/
/*			jump (any other) in cg_type							*/
/*==============================================================*/
static void gou_shouryuu_rush( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_w->acc.x * pl_w->spd.x > 0.0f ){
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
	}
	pl_char_move( pl_w ) ;
	gou_shouryuu_cg_hit( pl_w ) ;
	if( pl_w->cg_type < 0 ) pl_w->cg_type = 0 ;
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
static void gou_shouryuu_rise( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_char_move( pl_w ) ;
	gou_shouryuu_cg_hit( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_air_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void gou_shouryuu_jump( PLAYER_WORK *pl_w )
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
	gou_shouryuu_fall( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void gou_shouryuu_fall( PLAYER_WORK *pl_w )
{
	gou_shouryuu_y_move( pl_w ) ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_land_ef_set( pl_w ) ;
		gou_shouryuu_land( pl_w ) ;
	} else if( !pl_w->cg_type ){
		pl_char_move( pl_w ) ;
		gou_shouryuu_cg_hit( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void gou_shouryuu_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void gou_hadou_init( PLAYER_WORK *pl_w, PL1E_LOCAL_W *lw );
static void gou_hadou_wait( PLAYER_WORK *pl_w, PL1E_LOCAL_W *lw );
static void gou_hadou_start( PLAYER_WORK *pl_w, PL1E_LOCAL_W *lw );
static void gou_hadou_shot( PLAYER_WORK *pl_w, PL1E_LOCAL_W *lw );
static void gou_hadou_beam( PLAYER_WORK *pl_w, PL1E_LOCAL_W *lw );
static void gou_hadou_end( PLAYER_WORK *pl_w, PL1E_LOCAL_W *lw );

/*==============================================================*/
/*			A hyper combo: a beam that lasts longer for			*/
/*			pressed buttons										*/
/*==============================================================*/
static void sp_gou_hadou( PLAYER_WORK *pl_w )
{
	static void ( *gou_hadou_jmp[])( PLAYER_WORK *pl_w, PL1E_LOCAL_W *lw ) = {
		gou_hadou_init, gou_hadou_wait, gou_hadou_start, gou_hadou_shot, gou_hadou_beam, gou_hadou_end,
	} ;

	gou_hadou_jmp[pl_w->routine_2]( pl_w, ( PL1E_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void gou_hadou_init( PLAYER_WORK *pl_w, PL1E_LOCAL_W *lw )
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
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	lw->renda_cnt = pl_w->cpu_flag ? 16 : 24 ;
	pl_w->timer0 = 40 ;
	pl_w->atck_no = pl1e_at_gou_hadou ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 1 ) ;
}

/*--------------------------------------------------------------*/
static void gou_hadou_wait( PLAYER_WORK *pl_w, PL1E_LOCAL_W *lw )
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
		ofs.x = DOT_X( 37.0f ) ;
		ofs.y = DOT_Y( 50.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void gou_hadou_start( PLAYER_WORK *pl_w, PL1E_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->routine_2 ++ ;
	pl_char_set( pl_w, Char_Super, 8 ) ;
}

/*--------------------------------------------------------------*/
static void gou_hadou_shot( PLAYER_WORK *pl_w, PL1E_LOCAL_W *lw )
{
	Float x, y ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		x = 195.0f ;
		y = DOT_Y( 53.0f ) ;
		pls1e_01_set( pl_w ) ;
		shl1e_01_set( pl_w, 0, x, y ) ;
	}
}

/*==============================================================*/
/*			Each press while lw->renda_cnt lasts holds the		*/
/*			beam a frame longer									*/
/*==============================================================*/
static void gou_hadou_beam( PLAYER_WORK *pl_w, PL1E_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( lw->renda_cnt > 0 && sw_renda_ck( pl_w ) ){
		lw->renda_cnt -- ;
		pl_w->timer0 ++ ;
	}
	if( -- pl_w->timer0 <= 0 ){
		pl_w->routine_2 ++ ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_char_set( pl_w, Char_Super, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void gou_hadou_end( PLAYER_WORK *pl_w, PL1E_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void kuujin_init( PLAYER_WORK *pl_w );
static void kuujin_move( PLAYER_WORK *pl_w );
static void kuujin_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A kick diving down from a jump						*/
/*==============================================================*/
static void sp_kuujin( PLAYER_WORK *pl_w )
{
	static void ( *kuujin_jmp[])( PLAYER_WORK *pl_w ) = {
		kuujin_init, kuujin_move, kuujin_end,
	} ;

	kuujin_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void kuujin_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_gauge_add( pl_w, 10 ) ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl1e_at_kuujin ;
	HIT_RESET( pl_w ) ;
	pl_w->spd.x = pl_w->pl_dir ? DOT_X( 8.0f ) : -DOT_X( 8.0f ) ;
	pl_w->spd.y = -DOT_Y( 8.0f ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 25 ) ;
	cg_hit( pl_w ) ;
}

/*==============================================================*/
/*			The animation holds the dive still while cg_type	*/
/*			is set												*/
/*==============================================================*/
static void kuujin_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 27 ) ;
		pl_land_ef_set( pl_w ) ;
	}
	cg_hit( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void kuujin_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
		return ;
	}
	cg_hit( pl_w ) ;
}

/*==============================================================*/
/*			A pose held for 60 frames							*/
/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Extra, 6 ) ;
		pl_w->timer0 = 60 ;
	} else {
		if( -- pl_w->timer0 <= 0 ){
			pl_stand_ret( pl_w ) ;
			return ;
		}
		pl_char_move( pl_w ) ;
	}
}

static void snapback_init( PLAYER_WORK *pl_w );
static void snapback_move( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Steps forward on the animation's marks, with an		*/
/*			effect at cg_type									*/
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
	pl_w->atck_no = pl1e_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 11 ) ;
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
		ofs.x = DOT_X( 23.0f ) ;
		ofs.y = DOT_Y( 95.0f ) ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
	if( pl_w->cg_prm ){
		spd = pl_w->pl_dir ? DOT_X( 2.0f ) : -DOT_X( 2.0f ) ;
		pl_w->pos.x += spd ;
	}
}

/*==============================================================*/
/*			A new hit on each of the animation's marks			*/
/*==============================================================*/
static void gou_rasen_hit( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->atck_no = pl1e_at_gou_rasen ;
		HIT_RESET( pl_w ) ;
	}
}

static void gou_rasen_init( PLAYER_WORK *pl_w );
static void gou_rasen_wait( PLAYER_WORK *pl_w );
static void gou_rasen_rise( PLAYER_WORK *pl_w );
static void gou_rasen_last( PLAYER_WORK *pl_w );
static void gou_rasen_fall( PLAYER_WORK *pl_w );
static void gou_rasen_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo: a rising uppercut with many hits		*/
/*==============================================================*/
static void sp_gou_rasen( PLAYER_WORK *pl_w )
{
	static void ( *gou_rasen_jmp[])( PLAYER_WORK *pl_w ) = {
		gou_rasen_init, gou_rasen_wait, gou_rasen_rise, gou_rasen_last, gou_rasen_fall, gou_rasen_land,
	} ;

	gou_rasen_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void gou_rasen_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_gauge_add( pl_w, 3 ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->atck_no = pl1e_at_gou_rasen ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 10 ) ;
}

/*==============================================================*/
/*			Ofs is never set before it is added to, in the		*/
/*			arcade too											*/
/*==============================================================*/
static void gou_rasen_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type == 2 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 45 ;
		pl_w->timer1 = 0 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.y = DOT_Y( 8.0f ) ;
		pl_air_set( pl_w ) ;
		pls1e_04_set( pl_w, 1 ) ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x += DOT_X( 20.0f ) ;
		ofs.y += DOT_Y( 40.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*==============================================================*/
/*			Rises for timer0 frames; timer1 counts the			*/
/*			frames that hit										*/
/*==============================================================*/
static void gou_rasen_rise( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	gou_rasen_hit( pl_w ) ;
	if( pl_w->hit_flag ) pl_w->timer1 ++ ;
	if( -- pl_w->timer0 <= 0 ){
		pl_w->routine_2 ++ ;
		if( pl_w->timer1 ){
			pl_w->atck_no = pl1e_at_gou_rasen_last ;
			HIT_RESET( pl_w ) ;
			pl_char_set( pl_w, Char_Super, 11 ) ;
		} else {
			pl_w->routine_2 ++ ;
			pl_char_set( pl_w, Char_Special, 5 ) ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_w->acc.y = -DOT_Y( 0.375f ) ;
		}
	}
}

/*==============================================================*/
/*			The last, strongest hit								*/
/*==============================================================*/
static void gou_rasen_last( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, 5 ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
	}
}

/*--------------------------------------------------------------*/
static void gou_rasen_fall( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Special, 7 ) ;
		pl_land_ef_set( pl_w ) ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
	} else pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void gou_rasen_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_stand_ret( pl_w ) ;
	}
}

static PLAYER_WORK *nml_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *crouch_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *jump_catch_ck( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Can a throw start?									*/
/****************************************************************/
PLAYER_WORK *pl1e_catch_ck( PLAYER_WORK *pl_w )
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
	if( pl_w->atck_pk == 0 && pl_w->atck_level == 1 && pl_w->pos.y > DOT_Y( 64.0f ) ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 3 ;
			return em ;
		}
		return 0 ;
	}
	return 0 ;
}

static void nml_k_set( PLAYER_WORK *pl_w );
static void nml_p_set( PLAYER_WORK *pl_w );
static void jump_k_set( PLAYER_WORK *pl_w );
static void catch_com_set( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Start the throw that pl1e_catch_ck or				*/
/*			catch_com_ck chose									*/
/****************************************************************/
void	pl1e_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_k_set, nml_p_set, nml_p_set, jump_k_set, catch_com_set,
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
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
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
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_char_set( pl_w, Char_Catch, 1 ) ;
}

/*==============================================================*/
static void jump_k_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	scr_excep_set( pl_w, 5, 5 ) ;
	ofs.x = -DOT_X( 50.0f ) ;
	ofs.y = DOT_Y( 74.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	catch_gauge_add( pl_w ) ;
	pl_w->routine_2 = 0 ;
	pl_char_set( pl_w, Char_Catch, 4 ) ;
}

/*==============================================================*/
static void catch_com_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	scr_excep_set( pl_w, 5, 5 ) ;
	ofs.x = -DOT_X( 50.0f ) ;
	ofs.y = DOT_Y( 74.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	pl_gauge_add( pl_w, 8 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_char_set( pl_w, Char_Catch, 6 ) ;
}

static void nml_k_catch( PLAYER_WORK *pl_w );
static void nml_p_catch( PLAYER_WORK *pl_w );
static void jump_k_catch( PLAYER_WORK *pl_w );
static void catch_com_throw( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void	pl1e_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_k_catch, nml_p_catch, nml_p_catch, jump_k_catch, catch_com_throw,
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
		em->atck_no = pl1e_at_throw_k ;
		em->pl_dir = pl_w->pl_dir ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

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
		em->atck_no = pl1e_at_throw_p ;
		em->pl_dir = pl_w->pl_dir ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			The air throw: Akuma lands with the opponent,		*/
/*			then throws											*/
/*==============================================================*/
static void jump_k_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 pos ;

	if( !pl_w->routine_2 ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		pl_char_move( pl_w ) ;
		if( pl_w->pos.y <= pl_w->ground_y ){
			pl_w->routine_2 ++ ;
			pl_w->pos.y = pl_w->ground_y ;
			pos.x = pl_w->pos.x ;
			pos.y = pl_w->pos.y ;
			pos.z = pl_w->pos.z ;
			pos.x += pl_w->pl_dir ? DOT_X( 8.0f ) : -DOT_X( 8.0f ) ;
			land_dust_set( &pos, pl_w->catch_w->pl_no ) ;
			scr_work.quake_level = 2 ;
			scr_work.quake_flag = 1 ;
			catch_se_req( pl_w->catch_w ) ;
			pl_char_set( pl_w, Char_Catch, 5 ) ;
		}
	} else {
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
			em->atck_no = pl1e_at_air_throw ;
			em->pl_dir = pl_w->pl_dir ;
		}
	}
}

/*==============================================================*/
/*			Jigoku Guruma: a hop forward with the opponent		*/
/*==============================================================*/
static void catch_com_throw( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( !pl_w->routine_2 ){
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
		em->atck_no = pl1e_at_guruma ;
		em->pl_dir = pl_w->pl_dir ;
	}
	if( !pl_w->cg_prm ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->pos.y <= pl_w->ground_y ){
			pl_w->pos.y = pl_w->ground_y ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.y = 0.0f ;
		}
	}
}

static void hold( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Holding the opponent								*/
/****************************************************************/
void	pl1e_hold( PLAYER_WORK *pl_w )
{
	static void ( *hold_jmp[])( PLAYER_WORK *pl_w ) = {
		hold, hold, hold, hold, hold,
	} ;

	hold_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void hold( PLAYER_WORK *pl_w )
{
	catch_pos_set( pl_w->catch_w, pl_w ) ;
}

static int shungoku_delay_ck( PLAYER_WORK *pl_w );
static int gou_hadou_delay_ck( PLAYER_WORK *pl_w );
static int gou_shouryuu_delay_ck( PLAYER_WORK *pl_w );
static int gou_rasen_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl1e_delay_ck( PLAYER_WORK *pl_w )
{
	if( shungoku_delay_ck( pl_w ) || gou_hadou_delay_ck( pl_w ) || gou_shouryuu_delay_ck( pl_w ) || gou_rasen_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static int shungoku_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_seq_ck( pl_w, com_shungoku, &pl_w->com_w8 ) ) return 0 ;
	if( *pl_w->gauge_level < 3 ) return 0 ;
	pl_w->delay_sp_type = 8 ;
	return 1 ;
}

/*==============================================================*/
static int gou_hadou_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_gou_hadou, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 13 ;
	return 1 ;
}

/*==============================================================*/
static int gou_shouryuu_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_gou_shouryuu, &pl_w->com_w9 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 11 ;
	return 1 ;
}

/*==============================================================*/
static int gou_rasen_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_gou_rasen, &pl_w->com_w12 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 17 ;
	return 1 ;
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void	pl1e_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl1e_sp_gou_hadou ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl1e_sp_gou_shouryuu ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl1e_sp_gou_rasen ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl1e_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl1e_sp_gou_hadou ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl1e_sp_gou_shouryuu ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl1e_sp_gou_rasen ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl1e_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl1e_sp_hadou ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl1e_sp_shouryuu ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl1e_sp_tatsumaki ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void	pl1e_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl1e_sp_hadou ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl1e_sp_shouryuu ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl1e_sp_tatsumaki ;
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
void	pl1e_t_kick( PLAYER_WORK *pl_w )
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
	pl_w->atck_no = pl1e_at_t_kick ;
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
/* "s_pl1e.c" End of File	*/

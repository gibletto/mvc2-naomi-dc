/* Venom (character 0e): command checks, normal attacks, appearance, win and lose poses, special
   moves and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl0e_def.h"

void pl0e_main( PLAYER_WORK *pl_w );
void pl0e_com_ck( PLAYER_WORK *pl_w );
void pl0e_atck_set( PLAYER_WORK *pl_w );
void pl0e_atck( PLAYER_WORK *pl_w );
void pl0e_dash( PLAYER_WORK *pl_w );
void pl0e_noki( PLAYER_WORK *pl_w );
void pl0e_appear( PLAYER_WORK *pl_w );
void pl0e_win( PLAYER_WORK *pl_w );
void pl0e_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl0e_catch_ck( PLAYER_WORK *pl_w );
void pl0e_catch_set( PLAYER_WORK *pl_w );
void pl0e_catch( PLAYER_WORK *pl_w );
void pl0e_hold( PLAYER_WORK *pl_w );
int pl0e_delay_ck( PLAYER_WORK *pl_w );
void pl0e_t_kick( PLAYER_WORK *pl_w );
void pl0e_upper( PLAYER_WORK *pl_w );
void pl0e_sp_unison( PLAYER_WORK *pl_w );
void pl0e_sp_gattai( PLAYER_WORK *pl_w );
void pl0e_sp_assist( PLAYER_WORK *pl_w );
void pl0e_sp_zero( PLAYER_WORK *pl_w );
void pl0e_web_swing( PLAYER_WORK *pl_w );

/* each attack's table: on the ground and in a jump, then in a super jump */
static Sint8 lp_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 mp_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 hp_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 lk_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 mk_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 hk_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 sj_lp_tbl[4] = {  1,  2,  0,  2 } ;
static Sint8 sj_mp_tbl[4] = { -1,  2,  1,  2 } ;
static Sint8 sj_hp_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 sj_lk_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 sj_mk_tbl[4] = { -1,  2, -1,  2 } ;
static Sint8 sj_hk_tbl[4] = { -1, -1, -1, -1 } ;

/* commands: time or steps, kind, the special move, then the stick and buttons */
static Uint16 com_web[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl0e_sp_web, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Venom Web */
static Uint16 com_bite[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl0e_sp_bite, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Death Bite */
static Uint16 com_webthrow[] = { 5, COM_GROUND, pl0e_sp_webthrow, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Web Throw */
static Uint16 com_fang[] = { 3, 0, pl0e_sp_fang, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Venom Fang */
static Uint16 com_rush[] = { 3, COM_GROUND, pl0e_sp_rush, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Venom Rush */

/* sp_fang's leap, x then y, for each strength: from the ground, then in the air */
static Float fang_spd[2][2][2] = {
	{ { -DOT_X( 4.0f ), DOT_Y( 6.0f ) }, { -DOT_X( 10.0f ), DOT_Y( 10.0f ) } },
	{ { -5.0f, 2.142857f }, { -DOT_X( 8.0f ), 15.0f } },
} ;

/* where sp_webthrow's web leaves the hand, x then y, picked by the animation */
static Sint16 sp_ofs_tbl[30] = {
	-105,  88,  -88, 132,  -16, 165, -105,  88,  -92, 135,
	 -16, 168,  -64,  76,   -9, 121,   89,  16,   68,  64,
	  45, 120,  -76,  38,    0,   0,  -61, 163,  -78, 128,
} ;

static void ( *pl0e_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl0e_main,
	pl0e_com_ck,
	pl0e_atck_set,
	pl0e_atck,
	pl0e_dash,
	pl0e_noki,
	pl0e_appear,
	pl0e_win,
	pl0e_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl0e_catch_ck,	/* returns a value */
	pl0e_catch_set,
	pl0e_catch,
	pl0e_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl0e_delay_ck,
	pl0e_t_kick,
	pl0e_upper,
	pl0e_sp_unison,
	pl0e_sp_gattai,
	pl0e_sp_assist,
	pl0e_sp_zero,
	0,
	pl0e_web_swing,
} ;

static Uint8 fang_ck( PLAYER_WORK *pl_w );
static Uint8 rush_ck( PLAYER_WORK *pl_w );
static Uint8 webthrow_ck( PLAYER_WORK *pl_w );
static Uint8 web_ck( PLAYER_WORK *pl_w );
static Uint8 bite_ck( PLAYER_WORK *pl_w );
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w );
static Uint8 snapback_ck( PLAYER_WORK *pl_w );
static Uint8 web_delay_ck( PLAYER_WORK *pl_w );
static Uint8 bite_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl0e_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl0e_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl0e_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( web_ck( pl_w ) ) return ;
	if( bite_ck( pl_w ) ) return ;
	if( webthrow_ck( pl_w ) ) return ;
	if( fang_ck( pl_w ) ) return ;
	if( rush_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( pl_air_dash_ck( pl_w, &pl_w->com_w12 ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
static Uint8 fang_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_fang, &pl_w->com_w0 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0e_sp_fang ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 rush_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_rush, &pl_w->com_w1 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0e_sp_rush ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 webthrow_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_webthrow, &pl_w->com_w2 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0e_sp_webthrow ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 web_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_web, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w3 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0e_sp_web ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 bite_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_bite, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w4 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0e_sp_bite ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 5 ) ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0e_sp_chouhatsu ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 snapback_ck( PLAYER_WORK *pl_w )
{
	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0e_sp_snapback ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl0e_delay_ck( PLAYER_WORK *pl_w )
{
	if( web_delay_ck( pl_w ) || bite_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static Uint8 web_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_web, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 3 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 bite_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_bite, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 4 ;
	return 1 ;
}

/****************************************************************/
/*			The colour of the hyper combo sp_web, kept while		*/
/*			its hit lasts										*/
/****************************************************************/
void pl0e_main( PLAYER_WORK *pl_w )
{
	PL0E_LOCAL_W *lw = ( PL0E_LOCAL_W * )pl_w->tlocal ;

	if( !lw->hyper_col_flag ) return ;
	if( pl_w->act_st == N_SPECIAL && pl_w->pl_sp_type == pl0e_sp_web && pl_w->char_no.b.grp == Char_Super
	 && pl_w->char_no.b.no == 0 && lw->hyper_col_flag ){
		if( Sys->game_timer0 & 1 ) ef_color_set( pl_w, 0, 0, 1 ) ;
		else pl_color_set( pl_w, 1 ) ;
	} else {
		lw->hyper_col_flag = 0 ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
	}
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
void pl0e_atck_set( PLAYER_WORK *pl_w )
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
	if( pl_w->atck_pk == 0 ){
		if( pl_w->pl_stance == STANCE_STAND ) stand_punch( pl_w ) ;
		else crouch_punch( pl_w ) ;
	} else {
		if( pl_w->pl_stance == STANCE_STAND ) stand_kick( pl_w ) ;
		else crouch_kick( pl_w ) ;
	}
}

/*==============================================================*/
static void stand_punch( PLAYER_WORK *pl_w )
{
	Uint8 atck, no ;
	int se ;

	switch( pl_w->nml_no ){
	case 0:
		no = atck = 0 ;
		se = 20 ;
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		no = atck = 1 ;
		se = 21 ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		no = atck = 2 ;
		se = 22 ;
		pl_w->chain_tbl = hp_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	pl_w->atck_no = atck ;
	HIT_RESET( pl_w ) ;
	pl_se_req( pl_w, se ) ;
	pl_char_set( pl_w, Char_Stand_P, no ) ;
}

/*==============================================================*/
static void crouch_punch( PLAYER_WORK *pl_w )
{
	Uint8 no, atck ;
	int se ;

	switch( pl_w->nml_no ){
	case 0:
		atck = 6 ;
		no = 0 ;
		se = 20 ;
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		atck = 7 ;
		no = 1 ;
		se = 21 ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		atck = 8 ;
		no = 2 ;
		se = 22 ;
		pl_w->chain_tbl = hp_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	pl_w->atck_no = atck ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_se_req( pl_w, se ) ;
	pl_char_set( pl_w, Char_Crouch_P, no ) ;
}

/*==============================================================*/
static void stand_kick( PLAYER_WORK *pl_w )
{
	Uint8 atck, no ;
	int se ;

	switch( pl_w->nml_no ){
	case 0:
		atck = 3 ;
		no = 0 ;
		se = 20 ;
		pl_w->chain_tbl = lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		atck = 4 ;
		no = 1 ;
		se = 21 ;
		pl_w->chain_tbl = mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		atck = 5 ;
		no = 2 ;
		se = 22 ;
		pl_w->chain_tbl = hk_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	pl_w->atck_no = atck ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_se_req( pl_w, se ) ;
	pl_char_set( pl_w, Char_Stand_K, no ) ;
}

/*==============================================================*/
static void crouch_kick( PLAYER_WORK *pl_w )
{
	Uint8 atck, no ;
	int se ;

	switch( pl_w->nml_no ){
	case 0:
		atck = 9 ;
		no = 0 ;
		se = 20 ;
		pl_w->chain_tbl = lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		atck = 10 ;
		no = 1 ;
		se = 21 ;
		pl_w->chain_tbl = mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		atck = 11 ;
		no = 2 ;
		se = 22 ;
		pl_w->chain_tbl = hk_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	pl_w->atck_no = atck ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_se_req( pl_w, se ) ;
	pl_char_set( pl_w, Char_Crouch_K, no ) ;
}

/*==============================================================*/
/*			In the air, only while that button's jump			*/
/*			attacks are left									*/
/*==============================================================*/
static void jump_atck_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->atck_pk == 0 && ( pl_w->jump_atk_cnt & JATK_P_MASK ) ){
		jump_atck( pl_w ) ;
		return ;
	}
	if( pl_w->atck_pk != 0 && ( pl_w->jump_atk_cnt & JATK_K_MASK ) ) jump_atck( pl_w ) ;
}

/*==============================================================*/
static void jump_atck( PLAYER_WORK *pl_w )
{
	if( pl_w->atck_pk == 0 ) jump_punch( pl_w ) ;
	else jump_kick( pl_w ) ;
}

/*==============================================================*/
static void jump_punch( PLAYER_WORK *pl_w )
{
	Uint8 atck, no ;
	int se ;

	switch( pl_w->nml_no ){
	case 0:
		atck = 12 ; no = 0 ; se = 20 ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = lp_tbl ;
		else pl_w->chain_tbl = sj_lp_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		atck = 13 ; no = 1 ; se = 21 ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = mp_tbl ;
		else pl_w->chain_tbl = sj_mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		atck = 14 ; no = 2 ; se = 22 ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = hp_tbl ;
		else pl_w->chain_tbl = sj_hp_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	pl_w->atck_no = atck ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_se_req( pl_w, se ) ;
	pl_char_set( pl_w, Char_Jump_P, no ) ;
	if( pl_w->jump_atk_cnt & JATK_P_MASK ) pl_w->jump_atk_cnt -= 0x01 ;
}

/*==============================================================*/
static void jump_kick( PLAYER_WORK *pl_w )
{
	Uint8 atck, no ;
	int se ;

	switch( pl_w->nml_no ){
	case 0:
		atck = 15 ; no = 0 ; se = 20 ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = lk_tbl ;
		else pl_w->chain_tbl = sj_lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		atck = 16 ; no = 1 ; se = 21 ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = mk_tbl ;
		else pl_w->chain_tbl = sj_mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		atck = 17 ; no = 2 ; se = 22 ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = hk_tbl ;
		else pl_w->chain_tbl = sj_hk_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	pl_w->atck_no = atck ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_se_req( pl_w, se ) ;
	pl_char_set( pl_w, Char_Jump_K, no ) ;
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
void pl0e_atck( PLAYER_WORK *pl_w )
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
	if( pl_w->atck_pk == 0 ){
		if( pl_w->pl_stance == STANCE_STAND ) stand_punch_move( pl_w ) ;
		else crouch_punch_move( pl_w ) ;
	} else {
		if( pl_w->pl_stance == STANCE_STAND ) stand_kick_move( pl_w ) ;
		else crouch_kick_move( pl_w ) ;
	}
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
/*			The medium and heavy ones hit again					*/
/*==============================================================*/
static void stand_kick_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	switch( pl_w->nml_no ){
	case 0:
		break ;
	case 1:
		if( pl_w->cg_flag1 ){
			pl_w->cg_flag1 = 0 ;
			pl_w->atck_no = pl0e_at_stand_mk_2 ;
			HIT_RESET( pl_w ) ;
		}
		break ;
	case 2:
		if( pl_w->cg_flag1 ){
			pl_w->cg_flag1 = 0 ;
			pl_w->atck_no = pl0e_at_stand_hk_2 ;
			HIT_RESET( pl_w ) ;
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
	if( !pl_w->atck_pk ) jump_punch_move( pl_w ) ;
	else jump_kick_move( pl_w ) ;
	if( pl_land_ck( pl_w ) ) pl_land_set( pl_w ) ;
}

/*==============================================================*/
/*			The heavy one hits again, as the animation says		*/
/*==============================================================*/
static void jump_punch_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_jump_ret( pl_w ) ;
		return ;
	}
	switch( pl_w->nml_no ){
	case 0:
	case 1:
		break ;
	case 2:
		if( pl_w->cg_flag1 ){
			pl_w->atck_no = pl_w->cg_flag1 ;
			HIT_RESET( pl_w ) ;
			pl_w->cg_flag1 = 0 ;
		}
		break ;
	}
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
	case 1:
		break ;
	case 2:
		if( pl_w->cg_flag1 ){
			pl_w->atck_no = pl_w->cg_flag1 ;
			HIT_RESET( pl_w ) ;
			pl_w->cg_flag1 = 0 ;
		}
		break ;
	}
}

/****************************************************************/
/*			The launcher										*/
/****************************************************************/
void pl0e_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->atck_no = pl0e_at_upper ;
		HIT_RESET( pl_w ) ;
		pl_brake_set( pl_w ) ;
		pl_gauge_add( pl_w, 5 ) ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_char_set( pl_w, Char_Extra, 4 ) ;
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

/****************************************************************/
/*			The dash: a glide for a set time					*/
/****************************************************************/
void pl0e_dash( PLAYER_WORK *pl_w )
{
	static void ( *dash_jmp[])( PLAYER_WORK *pl_w ) = {
		dash_init, dash_move,
	} ;

	dash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dash_init( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = -DOT_X( 9.5f ) ;
		pl_w->acc.x = 0.3125f ;
		pl_w->timer1 = 24 ;
		if( pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
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
	if( --pl_w->timer1 == 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void noki_init( PLAYER_WORK *pl_w );
static void noki_move( PLAYER_WORK *pl_w );
static void noki_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash										*/
/****************************************************************/
void pl0e_noki( PLAYER_WORK *pl_w )
{
	static void ( *noki_jmp[])( PLAYER_WORK *pl_w ) = {
		noki_init, noki_move, noki_end,
	} ;

	noki_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void noki_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->timer0 = 24 ;
	pl_w->spd.x = DOT_X( 9.5f ) ;
	pl_w->acc.x = -0.4017857f ;
	if( pl_w->pl_dir ){
		pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = -pl_w->acc.x ;
	}
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	noki_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void noki_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_char_move( pl_w ) ;
	if( --pl_w->timer0 == 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Dash, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void noki_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		RATE_CLR_X( pl_w ) ;
		pl_stand_ret( pl_w ) ;
	}
}

/****************************************************************/
void pl0e_appear( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->disp_flag = 1 ;
		pl_char_set( pl_w, Char_Appear, 0 ) ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_1 ++ ;
		pl_char_set( pl_w, Char_Stand, 0 ) ;
	}
}

static void win_init( PLAYER_WORK *pl_w );
static void win_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl0e_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_init,
		win_move,
	} ;

	win_jmp[ pl_w->routine_2 ]( pl_w ) ;
}

/*==============================================================*/
/*			Won by KO or on time: one of two poses; lost or		*/
/*			a draw: the third									*/
/*==============================================================*/
static void win_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	switch( pl_w->type ){
	case 0:
	case 2:
		pl_char_set( pl_w, Char_Win, Rnd() & 1 ) ;
		break ;
	case 1:
	case 3:
	case 4:
		pl_char_set( pl_w, Char_Win, 2 ) ;
		break ;
	}
}

/*--------------------------------------------------------------*/
static void win_move( PLAYER_WORK *pl_w )
{
	if( pl_w->act_st == N_WIN && pl_win_end_ck( pl_w ) ) pl_stand_ret( pl_w ) ;
	else pl_char_move( pl_w ) ;
}

static void sp_fang( PLAYER_WORK *pl_w );
static void sp_rush( PLAYER_WORK *pl_w );
static void sp_webthrow( PLAYER_WORK *pl_w );
static void sp_web( PLAYER_WORK *pl_w );
static void sp_bite( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_gamma( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );

/****************************************************************/
void	pl0e_sp_atck( PLAYER_WORK *pl_w )
{
	static void (*pl_sp_tbl[])( PLAYER_WORK *pl_w ) ={
		sp_fang,					/* 00 Venom Fang */
		sp_rush,					/* 01 Venom Rush */
		sp_webthrow,					/* 02 Web Throw */
		sp_web,					/* 03 Venom Web (hyper) */
		sp_bite,					/* 04 Death Bite (hyper) */
		sp_chouhatsu,					/* 05 Taunt */
		sp_gamma,					/* 06 */
		sp_snapback,					/* 07 Snapback */
	} ;

	pl_sp_tbl[ pl_w->pl_sp_type ]( pl_w ) ;
}

static void fang_init( PLAYER_WORK *pl_w );
static void fang_jump( PLAYER_WORK *pl_w );
static void fang_move( PLAYER_WORK *pl_w );
static void fang_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Venom Fang: a leap, from the ground or in the air	*/
/*==============================================================*/
static void sp_fang( PLAYER_WORK *pl_w )
{
	static void ( *fang_jmp[])( PLAYER_WORK *pl_w ) = {
		fang_init, fang_jump, fang_move, fang_land,
	} ;

	fang_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void fang_init( PLAYER_WORK *pl_w )
{
	Uint8 atck, no ;

	pl_w->routine_2 ++ ;
	pl_w->timer0 = 1 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_dir_set( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	if( pl_w->pl_stance != STANCE_AIR ){
		pl_w->pos.y = pl_w->ground_y ;
		no = pl_w->pl_stance = STANCE_STAND ;
		atck = 40 ;
		pl_sp_ef_set( pl_w ) ;
	} else {
		no = 2 ;
		atck = 45 ;
	}
	pl_w->atck_no = atck ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + no ) ;
}

/*--------------------------------------------------------------*/
static void fang_jump( PLAYER_WORK *pl_w )
{
	Float *spd ;
	Float ofs ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		if( pl_w->pl_stance != STANCE_AIR ) pls0e_00_set( pl_w, 1, 0 ) ;
		ofs = -DOT_X( 32.0f ) ;
		if( pl_w->pl_dir ) ofs = DOT_X( 32.0f ) ;
		pl_w->pos.x += ofs ;
		spd = &fang_spd[0][0][0] ;
		if( pl_w->pl_stance == STANCE_AIR ) spd += 4 ;
		spd += pl_w->atck_level * 2 ;
		pl_w->spd.x = *spd++ ;
		pl_w->spd.y = *spd ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = -DOT_Y( 0.75f ) ;
		if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
		pls0e_00_set( pl_w, 0, 0 ) ;
		pl_voice_req( pl_w, 20 ) ;
		pl_air_set( pl_w ) ;
	}
}

/*==============================================================*/
/*			In the air: the animation can make it hit again		*/
/*			until it has hit once								*/
/*==============================================================*/
static void fang_move( PLAYER_WORK *pl_w )
{
	pl_w->push_ng_timer = 2 ;
	pl_char_move( pl_w ) ;
	if( pl_w->hit_flag && pl_w->timer0 ){
		pl_w->timer0 = 0 ;
		pl_w->cg_flag1 = 0 ;
	}
	if( !pl_w->timer0 && pl_w->cg_flag1 ){
		pl_w->atck_no = pl_w->cg_flag1 ;
		HIT_RESET( pl_w ) ;
		pl_w->cg_flag1 = 0 ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_w->push_ng_timer = 0 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 4 ) ;
	}
}

/*--------------------------------------------------------------*/
static void fang_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pls0e_00_set( pl_w, 1, 0 ) ;
	}
}

static void rush_init( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw );
static void rush_shot( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw );
static void rush_wait( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw );
static void rush_end( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw );

/*==============================================================*/
/*			Venom Rush: sends out an effect and waits for it to	*/
/*			finish												*/
/*==============================================================*/
static void sp_rush( PLAYER_WORK *pl_w )
{
	static void ( *rush_jmp[])( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw ) = {
		rush_init, rush_shot, rush_wait, rush_end,
	} ;

	rush_jmp[pl_w->routine_2]( pl_w, ( PL0E_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void rush_init( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	lw->rush_st = 0 ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl0e_at_rush ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 12 ) ;
}

/*--------------------------------------------------------------*/
static void rush_shot( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_gauge_add( pl_w, 5 ) ;
		if( !pls0e_01_set( pl_w ) ) lw->rush_st = -1 ;
	}
}

/*--------------------------------------------------------------*/
static void rush_wait( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( lw->rush_st == -1 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 14 ) ;
	}
}

/*--------------------------------------------------------------*/
static void rush_end( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void webthrow_init( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw );
static void webthrow_move( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw );
static void webthrow_end( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw );
static void webthrow_shot( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw );
static void webthrow_wait( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw );
static void webthrow_throw( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw );
static void webthrow_release( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw );
static void webthrow_ofs_set( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw );
static void webthrow_ang_set( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw );

/*==============================================================*/
/*			Web Throw: a web that catches the opponent and		*/
/*			throws them											*/
/*==============================================================*/
static void sp_webthrow( PLAYER_WORK *pl_w )
{
	static void ( *webthrow_jmp[])( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw ) = {
		webthrow_init, webthrow_move, webthrow_end,
	} ;

	webthrow_jmp[pl_w->routine_2]( pl_w, ( PL0E_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void webthrow_init( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->timer0 = 0 ;
	pl_w->dir = pl_w->atck_level ? 28 : 24 ;
	lw->line_st = 0 ;
	lw->web_line = pl_w->dir ;
	lw->web_dir = pl_w->dir ;
	lw->web_turn = 0.0f ;
	pl_w->atck_no = pl_w->atck_level + 50 ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 5 ) ;
}

/*--------------------------------------------------------------*/
static void webthrow_move( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw )
{
	static void ( *webthrow_move_jmp[])( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw ) = {
		webthrow_shot, webthrow_wait, webthrow_throw, webthrow_release,
	} ;

	webthrow_move_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*==============================================================*/
static void webthrow_shot( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_prm ){
		pl_w->routine_3 ++ ;
		pl_w->timer1 = 360 ;
		pl_w->unk_27b = 1 ;
		pl_w->cancel_timer = 16 ;
		pl_gauge_add( pl_w, 5 ) ;
		shl0e_03_set( pl_w ) ;
		webthrow_ofs_set( pl_w, lw ) ;
	}
}

/*==============================================================*/
/*			The web is out: it has caught (lw->line_st above	*/
/*			0), missed (below 0) or timed out					*/
/*==============================================================*/
static void webthrow_wait( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	pl_char_move( pl_w ) ;
	webthrow_ofs_set( pl_w, lw ) ;
	if( lw->line_st > 0 ){
		pl_w->routine_3 ++ ;
		pl_w->catch_flag = 1 ;
		pl_w->catch_no = 0xc3 ;
		pl_w->sp_ng_timer = 3 ;
		pl_w->timer0 = 0 ;
		pl_w->timer1 = 0 ;
		em = pl_w->catch_w ;
		em->wall_ng_timer = 2 ;
		em->catch_no = pl_w->catch_no ;
		hold_renda_set( em, 35 ) ;
		catch_renda_set( pl_w, 20 ) ;
		scr_excep_set( pl_w, 0, 5 ) ;
		pl_char_set( pl_w, Char_Special, 9 ) ;
		webthrow_ofs_set( pl_w, lw ) ;
		webthrow_ang_set( pl_w, lw ) ;
		return ;
	}
	if( lw->line_st < 0 || --pl_w->timer1 == 0 ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 7 ) ;
	}
}

/*--------------------------------------------------------------*/
static void webthrow_throw( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw )
{
	PLAYER_WORK *em ;
	VEC3 pos ;

	pl_w->catch_flag = 1 ;
	pl_w->sp_ng_timer = 3 ;
	em = pl_w->catch_w ;
	webthrow_ang_set( pl_w, lw ) ;
	if( pl_w->cg_prm ){
		pl_w->cg_prm = 0 ;
		em->dm_hit_w = pl_w ;
		em->atck_no = pl0e_at_webthrow ;
		if( pl_w->timer0 ) em->atck_no = pl0e_at_webthrow_renda ;
		catch_damage( pl_w ) ;
		pos.x = em->pos.x ;
		pos.y = em->ground_y ;
		land_dust_set( &pos, em->pl_no ) ;
		catch_se_req( pl_w ) ;
		scr_work.quake_level = 1 ;
		scr_work.quake_flag = 1 ;
		return ;
	}
	if( hold_renda_ck( em ) ){
		pl_w->timer0 = 0 ;
		hold_renda_set( em, 35 ) ;
	}
	if( catch_renda_ck( pl_w ) && !pl_w->timer0 ){
		pl_w->timer0 ++ ;
		catch_renda_set( pl_w, 20 ) ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		if( !pl_w->timer0 || pl_w->timer1 ){
			pl_w->routine_3 ++ ;
			em->routine_2 ++ ;
			lw->line_st = -1 ;
			pl_char_set( pl_w, Char_Special, 11 ) ;
			return ;
		}
		pl_w->timer0 = 0 ;
		pl_w->timer1 = 1 ;
		pl_char_set( pl_w, Char_Special, 9 ) ;
	}
	webthrow_ofs_set( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void webthrow_release( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	pl_w->catch_flag = 1 ;
	webthrow_ang_set( pl_w, lw ) ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->sp_ng_timer = 1 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->pl_dir = pl_w->pl_dir ;
		em->pl_stance = STANCE_AIR ;
		em->atck_no = pl0e_at_webthrow_end ;
		em->routine_2 = 0 ;
		em->wall_flag = 0 ;
		scr_excep_clr() ;
		pls0e_06_set( pl_w, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void webthrow_end( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_dir_set( pl_w ) ;
		pl_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
/*			Where the web leaves the hand, from the				*/
/*			animation											*/
/*==============================================================*/
static void webthrow_ofs_set( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw )
{
	Sint16 *ofs ;
	Float x, y ;

	ofs = &sp_ofs_tbl[pl_w->cg_type >> 1] ;
	x = *ofs++ * 1.6666666f ;
	y = *ofs * 2.142857f ;
	if( pl_w->pl_dir ) x = -x ;
	lw->hand_x = x ;
	lw->hand_y = y ;
}

/*==============================================================*/
/*			Turns the web's angle (0 to 32) toward the			*/
/*			animation's, over cg_ctr frames						*/
/*==============================================================*/
static void webthrow_ang_set( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw )
{
	Float ang ;

	ang = pl_w->cg_flag1 & 0x7f ;
	if( lw->web_dir != ang ){
		lw->web_line = lw->web_dir ;
		lw->web_dir = ang ;
		lw->web_turn = lw->web_dir - lw->web_line ;
		if( lw->web_turn >= 16.0f ) lw->web_turn -= 32.0f ;
		if( lw->web_turn <= -16.0f ) lw->web_turn += 32.0f ;
		lw->web_turn /= pl_w->cg_ctr ;
	}
	if( lw->web_dir == lw->web_line ) lw->web_turn = 0.0f ;
	lw->web_line += lw->web_turn ;
	if( lw->web_line < 0.0f ) lw->web_line += 32.0f ;
	if( lw->web_line >= 32.0f ) lw->web_line -= 32.0f ;
	pl_w->dir = lw->web_line ;
}

static void web_rise( PLAYER_WORK *pl_w );
static void web_atck( PLAYER_WORK *pl_w );
static void web_end( PLAYER_WORK *pl_w );
static void web_init( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw );
static void web_flash( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw );
static void web_fly( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw );
static void web_shot( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw );
static void web_wait( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw );
static void web_aim( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw );
static void web_dive( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw );
static void web_bite( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw );
static void web_fall( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw );

/*==============================================================*/
/*			Venom Web, a hyper combo: he rises to the middle of	*/
/*			the screen, webs the opponent and dives on them		*/
/*==============================================================*/
static void sp_web( PLAYER_WORK *pl_w )
{
	static void ( *web_jmp[])( PLAYER_WORK *pl_w ) = {
		web_rise, web_atck, web_end,
	} ;

	web_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void web_rise( PLAYER_WORK *pl_w )
{
	static void ( *web_rise_jmp[])( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw ) = {
		web_init, web_flash, web_fly,
	} ;

	web_rise_jmp[pl_w->routine_3]( pl_w, ( PL0E_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void web_init( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 255 ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_3 ++ ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl0e_at_web ;
	HIT_RESET( pl_w ) ;
	scr_excep_set( pl_w, 1, 13 ) ;
	pl_w->spd.x = ( ( scr_work.scr_left + scr_work.scr_right ) / 2.0f - pl_w->pos.x ) / 16.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 6.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->timer0 = 16 ;
	pl_w->timer1 = 0 ;
	lw->web_line_st = 0 ;
	lw->hyper_col_flag = 1 ;
	pl_char_set( pl_w, Char_Super, 0 ) ;
}

/*==============================================================*/
/*			Flashes until the animation lets him go				*/
/*==============================================================*/
static void web_flash( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type == 0 ){
		pl_w->routine_3 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->pl_stance = STANCE_AIR ;
		lw->hyper_col_flag = -1 ;
		pl_color_set( pl_w, 1 ) ;
	} else if( pl_w->timer1 ^= 1 ) pl_color_set( pl_w, 8 ) ;
	else pl_color_set( pl_w, 1 ) ;
}

/*--------------------------------------------------------------*/
static void web_fly( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_w->push_ng_timer = 2 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( --pl_w->timer0 == 0 ){
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->cg_ctr = 1 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		ofs.x = -DOT_X( 17.0f ) ;
		ofs.y = DOT_Y( 69.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void web_atck( PLAYER_WORK *pl_w )
{
	static void ( *web_atck_jmp[])( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw ) = {
		web_shot, web_wait, web_aim, web_dive, web_bite, web_fall,
	} ;

	web_atck_jmp[pl_w->routine_3]( pl_w, ( PL0E_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void web_shot( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		lw->web_web_flag = 1 ;
		shl0e_01_set( pl_w ) ;
	}
}

/*==============================================================*/
/*			Missed: he falls back down							*/
/*==============================================================*/
static void web_wait( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( lw->web_line_st < 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 1 ;
		lw->web_web_flag = 0 ;
		scr_excep_set( pl_w, 0, 13 ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->acc.y = -0.9375f ;
		pl_char_set( pl_w, Char_Super, 6 ) ;
	} else if( lw->web_line_st > 0 ) pl_w->routine_3 ++ ;
}

/*==============================================================*/
/*			Caught: he heads for the opponent					*/
/*==============================================================*/
static void web_aim( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw )
{
	PLAYER_WORK *em ;
	Float ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->timer0 = 8 ;
		pl_dir_set( pl_w ) ;
		em = pl_w->catch_w ;
		ofs = DOT_X( 32.0f ) ;
		if( pl_w->pl_dir ) ofs = -DOT_X( 32.0f ) ;
		pl_w->spd.x = ( em->pos.x + ofs - pl_w->pos.x ) / 8.0f ;
		pl_w->spd.y = ( em->pos.y - pl_w->pos.y ) / 8.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Super, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void web_dive( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->push_ng_timer = 2 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( --pl_w->timer0 == 0 ){
		pl_w->routine_3 ++ ;
		pl_w->atck_no = pl0e_at_web_dive ;
		HIT_RESET( pl_w ) ;
		pl_dir_set( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 2 ) ;
	}
}

/*==============================================================*/
/*			Each bite the animation marks hits again; if one	*/
/*			misses he falls										*/
/*==============================================================*/
static void web_bite( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw )
{
	Float ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_flag1 ){
		if( pl_w->hit_flag ){
			pl_w->atck_no = pl_w->cg_flag1 ;
			HIT_RESET( pl_w ) ;
			pl_w->atck_attr |= ATR_BACK_POS ;
			pl_w->cg_flag1 = 0 ;
		} else {
			pl_w->delay_flag = 0 ;
			pl_w->delay_timer = 0 ;
			pl_w->hyper_bg = 0 ;
			pl_w->hyper_bg_tm = 0 ;
			pl_w->routine_2 ++ ;
			pl_w->routine_3 = 1 ;
			lw->web_web_flag = 0 ;
			scr_excep_set( pl_w, 0, 13 ) ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = -0.9375f ;
			pl_char_set( pl_w, Char_Super, 6 ) ;
		}
	}
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		scr_excep_set( pl_w, 0, 13 ) ;
		pl_w->spd.x = -10.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 2.0f ) ;
		pl_w->acc.y = -0.9375f ;
		if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
		ofs = DOT_X( 32.0f ) ;
		if( pl_w->pl_dir ) ofs = -DOT_X( 32.0f ) ;
		pl_w->pos.x += ofs ;
		pls0e_00_set( pl_w, 0, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void web_fall( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->push_ng_timer = 2 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_flag1 ){
		pl_w->atck_no = pl_w->cg_flag1 ;
		HIT_RESET( pl_w ) ;
		pl_w->cg_flag1 = 0 ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_CROUCH ;
		lw->web_web_flag = 0 ;
		pls0e_00_set( pl_w, 1, 0 ) ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 3 ) ;
	}
}

/*==============================================================*/
/*			Lands; after a miss he falls first					*/
/*==============================================================*/
static void web_end( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_3 ){
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	} else {
		pl_char_move( pl_w ) ;
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->pos.y < pl_w->ground_y ){
			pl_w->routine_3 -- ;
			pl_w->pos.y = pl_w->ground_y ;
			pl_w->pl_stance = STANCE_STAND ;
			pl_land_ef_set( pl_w ) ;
			pl_char_set( pl_w, Char_Super, 5 ) ;
		}
	}
}

static void bite_start( PLAYER_WORK *pl_w );
static void bite_wait( PLAYER_WORK *pl_w );
static void bite_end( PLAYER_WORK *pl_w );
static void bite_init( PLAYER_WORK *pl_w );
static void bite_flash( PLAYER_WORK *pl_w );
static void bite_shot( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Death Bite, a hyper combo: he sends out an effect	*/
/*			and waits for it									*/
/*==============================================================*/
static void sp_bite( PLAYER_WORK *pl_w )
{
	static void ( *bite_jmp[])( PLAYER_WORK *pl_w ) = {
		bite_start, bite_wait, bite_end,
	} ;

	bite_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void bite_start( PLAYER_WORK *pl_w )
{
	static void ( *bite_start_jmp[])( PLAYER_WORK *pl_w ) = {
		bite_init, bite_flash, bite_shot,
	} ;

	bite_start_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
/*			With too few effects free it does nothing			*/
/*==============================================================*/
static void bite_init( PLAYER_WORK *pl_w )
{
	PL0E_LOCAL_W *lw = ( PL0E_LOCAL_W * )pl_w->tlocal ;
	Uint8 atck ;

	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 255 ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_3 ++ ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->timer0 = 0 ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	if( work_rest_cnt <= 2 ){
		pl_w->routine_2 = 2 ;
		pl_w->routine_3 = 0 ;
		pl_char_set( pl_w, Char_Super, 8 ) ;
		return ;
	}
	pls0e_02_set( pl_w, 0 ) ;
	pls0e_02_set( pl_w, 1 ) ;
	lw->bite_st = 0 ;
	lw->bite_hit_cnt = 14 ;
	atck = 61 ;
	if( pl_w->team_type == 4 || pl_w->team_type == 5 ) atck = 68 ;
	pl_w->atck_no = atck ;
	HIT_RESET( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_char_set( pl_w, Char_Super, 7 ) ;
}

/*--------------------------------------------------------------*/
static void bite_flash( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->routine_3 ++ ;
		pl_w->cg_type = 0 ;
		ofs.x = DOT_X( 16.0f ) ;
		ofs.y = DOT_Y( 96.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void bite_shot( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->cg_type = 0 ;
		pl_w->timer0 = 360 ;
		shl0e_02_set( pl_w, 0, pl_w ) ;
	}
}

/*==============================================================*/
/*			Until the effect is over (lw->bite_st below 0),		*/
/*			or has hit and the time is up						*/
/*==============================================================*/
static void bite_wait( PLAYER_WORK *pl_w )
{
	PL0E_LOCAL_W *lw = ( PL0E_LOCAL_W * )pl_w->tlocal ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( lw->bite_st >= 0 ){
		if( !lw->bite_st ) return ;
		if( --pl_w->timer0 ) return ;
	}
	pl_w->delay_flag = 0 ;
	pl_w->delay_timer = 0 ;
	pl_w->hyper_bg = 0 ;
	pl_w->hyper_bg_tm = 0 ;
	pl_w->routine_2 ++ ;
	pl_w->routine_3 = 0 ;
	pl_char_set( pl_w, Char_Super, 8 ) ;
}

/*--------------------------------------------------------------*/
static void bite_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void chouhatsu_init( PLAYER_WORK *pl_w );
static void chouhatsu_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The taunt: a pose									*/
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
	pl_w->routine_3 = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pls0e_06_set( pl_w, 1 ) ;
	pl_char_set( pl_w, Char_Extra, 2 ) ;
}

/*--------------------------------------------------------------*/
static void chouhatsu_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void gamma_init( PLAYER_WORK *pl_w );
static void gamma_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			An attack on the spot, for the gamma assist and		*/
/*			counter												*/
/*==============================================================*/
static void sp_gamma( PLAYER_WORK *pl_w )
{
	static void ( *gamma_jmp[])( PLAYER_WORK *pl_w ) = {
		gamma_init, gamma_end,
	} ;

	gamma_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void gamma_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl0e_at_assist ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 16 ) ;
}

/*--------------------------------------------------------------*/
static void gamma_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void snapback_init( PLAYER_WORK *pl_w );
static void snapback_move( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The snapback: an attack that sends out an effect	*/
/*			when the animation says								*/
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
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl0e_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 17 ) ;
}

/*--------------------------------------------------------------*/
static void snapback_move( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	else {
		if( pl_w->cg_type & 1 ){
			pl_w->cg_type ^= 1 ;
			ofs.x = -DOT_X( 14.0f ) ;
			ofs.y = DOT_Y( 80.0f ) ;
			pl_snap_flash( pl_w, &ofs ) ;
		}
		if( pl_w->cg_type & 2 ){
			pl_w->cg_type ^= 2 ;
			pl_se_req( pl_w, SE_SWING_H ) ;
		}
	}
}

/****************************************************************/
void	pl0e_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0e_sp_bite ;
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl0e_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0e_sp_bite ;
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl0e_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl0e_sp_fang ;
		pl_w->atck_level = 0 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl0e_sp_rush ;
		pl_w->atck_level = 0 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl0e_sp_gamma ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void	pl0e_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl0e_sp_fang ;
		pl_w->atck_level = 0 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl0e_sp_rush ;
		pl_w->atck_level = 0 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl0e_sp_gamma ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_move( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
void	pl0e_t_kick( PLAYER_WORK *pl_w )
{
	static void ( *t_kick_jmp[])( PLAYER_WORK *pl_w ) = {
		t_kick_init, t_kick_move, t_kick_land,
	} ;

	t_kick_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void t_kick_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->spd.x = -30.0f ;
	pl_w->spd.y = DOT_X( 0.125f ) ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
	pl_w->atck_no = pl0e_at_t_kick ;
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
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pls0e_06_set( pl_w, 2 ) ;
		pl_char_set( pl_w, Char_Extra, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void t_kick_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void swing_init( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw );
static void swing_move( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw );
static void swing_fall( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw );

/****************************************************************/
/*			Swings on a web until he meets a wall or the web	*/
/*			passes the end of its arc							*/
/****************************************************************/
void	pl0e_web_swing( PLAYER_WORK *pl_w )
{
	static void ( *swing_jmp[])( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw ) = {
		swing_init, swing_move, swing_fall,
	} ;
	PL0E_LOCAL_W *lw = ( PL0E_LOCAL_W * )pl_w->tlocal ;

	pl_w->push_ng_timer = 1 ;
	swing_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/*==============================================================*/
static void swing_init( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_w->routine_3 = 0 ;
	pl_w->dir = 28 ;
	pl_w->sp_ng_timer = 12 ;
	pl_w->atck_ng_timer = 12 ;
	lw->swing_st = 0 ;
	pls0e_05_set( pl_w, 8, 0 ) ;
	pl_char_set( pl_w, Char_Extra, 3 ) ;
	webthrow_ofs_set( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void swing_move( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw )
{
	Float spd ;

	/* a wall behind him lets go at once */
	if( pl_w->wall_flag ){
		if( !( pl_w->wall_flag & 1 << pl_w->pl_dir ) ) goto let_go ;
	}
	pl_w->push_ng_timer = 0 ;
	pl_char_move( pl_w ) ;
	webthrow_ofs_set( pl_w, lw ) ;
	if( !pl_w->routine_3 ){
		if( pl_w->cg_prm ) return ;
		pl_w->routine_3 ++ ;
		pl_w->spd.x = -DOT_X( 3.25f ) ;
		pl_w->acc.x = -1.25f ;
		pl_w->spd.y = -DOT_Y( 3.0f ) ;
		pl_w->acc.y = -0.9375f ;
		if( pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
	}
	spd = pl_w->spd.x + pl_w->acc.x ;
	pl_w->spd.x = spd ;
	pl_w->pos.x += spd ;
	pl_w->pos.y += pl_w->spd.y ;
	if( !( pl_w->wall_flag & 1 << ( pl_w->pl_dir ^ 1 ) ) && 1 << pl_w->dir & 0xff00000f ) return ;
let_go:
	lw->swing_st = -1 ;
	pl_w->routine_2 ++ ;
	pl_w->routine_3 = 0 ;
	pl_w->cg_ctr = 1 ;
	pl_w->spd.x = -10.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 6.0f ) ;
	pl_w->acc.y = -0.9375f ;
	if( pl_w->pl_dir ){
		pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = -pl_w->acc.x ;
	}
	pl_char_set2( pl_w, Char_Extra, 3, 4 ) ;
}

/*--------------------------------------------------------------*/
static void swing_fall( PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
	else if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_set( pl_w ) ;
	}
}

/****************************************************************/
PLAYER_WORK *pl0e_catch_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_w->pl_stance == STANCE_CROUCH ) return 0 ;
	if( !( pl_w->atck_lever & ( KEY_FORWARD | KEY_BACK ) ) ) return 0 ;
	if( !pl_w->atck_level ) return 0 ;
	if( !pl_w->atck_pk ){
		if( pl_w->pl_stance != STANCE_AIR ){
			if( ( em = pl_catch_ck( pl_w ) ) ) pl_w->catch_no = 0 ;
		} else {
			if( ( em = pl_catch_ck( pl_w ) ) ) pl_w->catch_no = 2 ;
		}
	} else {
		if( pl_w->pl_stance == STANCE_AIR ) return 0 ;
		if( ( em = pl_catch_ck( pl_w ) ) ) pl_w->catch_no = 1 ;
	}
	return em ;
}

static void nml_p_set( PLAYER_WORK *pl_w );
static void nml_k_set( PLAYER_WORK *pl_w );
static void jump_p_set( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Start the throw that pl0e_catch_ck chose			*/
/****************************************************************/
void	pl0e_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_set, nml_k_set, jump_p_set,
	} ;

	catch_set_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	scr_excep_set( pl_w, 5, 5 ) ;
	if( pl_w->atck_lever & KEY_BACK ){
		pl_w->disp_dir ^= 1 ;
		pl_w->pl_dir ^= 1 ;
	}
	ofs.x = -DOT_X( 88.0f ) ;
	ofs.y = DOT_Y( 88.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	catch_gauge_add( pl_w ) ;
	pl_w->hit_stop = 10 ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_char_set( pl_w, Char_Catch, 0 ) ;
}

/*==============================================================*/
static void nml_k_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	scr_excep_set( pl_w, 5, 5 ) ;
	if( pl_w->atck_lever & KEY_FORWARD ){
		pl_w->disp_dir ^= 1 ;
		pl_w->pl_dir ^= 1 ;
	}
	ofs.x = -DOT_X( 104.0f ) ;
	ofs.y = DOT_Y( 88.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	catch_gauge_add( pl_w ) ;
	pl_w->hit_stop = 10 ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_char_set( pl_w, Char_Catch, 1 ) ;
}

/*==============================================================*/
static void jump_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	scr_excep_set( pl_w, 5, 5 ) ;
	if( pl_w->atck_lever & KEY_BACK ){
		pl_w->disp_dir ^= 1 ;
		pl_w->pl_dir ^= 1 ;
	}
	ofs.x = -DOT_X( 104.0f ) ;
	ofs.y = DOT_Y( 104.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	catch_gauge_add( pl_w ) ;
	pl_w->hit_stop = 10 ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_char_set( pl_w, Char_Catch, 2 ) ;
}

static void nml_p_catch( PLAYER_WORK *pl_w );
static void nml_k_catch( PLAYER_WORK *pl_w );
static void jump_p_catch( PLAYER_WORK *pl_w );
static void catch_dummy( PLAYER_WORK *pl_w );

/****************************************************************/
void	pl0e_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_catch, nml_k_catch, jump_p_catch, catch_dummy, catch_dummy,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

static void nml_p_init( PLAYER_WORK *pl_w );
static void nml_p_move( PLAYER_WORK *pl_w );
static void nml_p_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void nml_p_catch( PLAYER_WORK *pl_w )
{
	static void ( *nml_p_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_init, nml_p_move, nml_p_end,
	} ;

	nml_p_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	nml_p_move( pl_w ) ;
}

/*==============================================================*/
/*			The animation says when it hits; a knocked out		*/
/*			opponent is let go at once							*/
/*==============================================================*/
static void nml_p_move( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->atck_no = pl0e_at_throw_p ;
		catch_damage( pl_w ) ;
		if( !em->vital_new ){
			pl_dir_set( pl_w ) ;
			em->catch_type = 1 ;
			em->pl_dir = pl_w->pl_dir ^ 1 ;
			em->disp_dir = pl_w->disp_dir ^ 1 ;
			em->pl_stance = STANCE_AIR ;
			scr_excep_clr() ;
			pl_w->catch_flag = 0 ;
			pl_stand_ret( pl_w ) ;
		} else {
			pls0e_04_set( pl_w, em, 0 ) ;
			pls0e_04_set( pl_w, em, 1 ) ;
			scr_excep_clr() ;
		}
	}
}

/*--------------------------------------------------------------*/
static void nml_p_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_dir_set( pl_w ) ;
		pl_stand_ret( pl_w ) ;
	}
}

static void nml_k_init( PLAYER_WORK *pl_w );
static void nml_k_move( PLAYER_WORK *pl_w );
static void nml_k_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void nml_k_catch( PLAYER_WORK *pl_w )
{
	static void ( *nml_k_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_k_init, nml_k_move, nml_k_end,
	} ;

	nml_k_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void nml_k_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	nml_k_move( pl_w ) ;
}

/*==============================================================*/
/*			Lets the opponent go into the air, and shakes		*/
/*			the screen											*/
/*==============================================================*/
static void nml_k_move( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 pos ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 11 ;
		em->pl_dir = pl_w->pl_dir ;
		em->disp_dir = pl_w->disp_dir ;
		em->pl_stance = STANCE_AIR ;
		em->atck_no = pl0e_at_throw_k ;
		scr_excep_clr() ;
		pos.x = em->pos.x ;
		pos.y = em->ground_y ;
		land_dust_set( &pos, em->pl_no ) ;
		catch_se_req( pl_w ) ;
		scr_work.quake_level = 1 ;
		scr_work.quake_flag = 1 ;
	}
}

/*--------------------------------------------------------------*/
static void nml_k_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_dir_set( pl_w ) ;
		pl_stand_ret( pl_w ) ;
	}
}

static void jump_p_init( PLAYER_WORK *pl_w );
static void jump_p_move( PLAYER_WORK *pl_w );
static void jump_p_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void jump_p_catch( PLAYER_WORK *pl_w )
{
	static void ( *jump_p_jmp[])( PLAYER_WORK *pl_w ) = {
		jump_p_init, jump_p_move, jump_p_end,
	} ;

	jump_p_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void jump_p_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.y = DOT_Y( 2.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.75f ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
	jump_p_move( pl_w ) ;
}

/*==============================================================*/
/*			Comes down with the opponent, then throws them		*/
/*			up and leaps after									*/
/*==============================================================*/
static void jump_p_move( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 pos ;

	if( !pl_w->routine_3 ){
		pl_char_move( pl_w ) ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->pos.y < pl_w->ground_y ){
			pl_w->routine_3 ++ ;
			pl_w->pos.y = pl_w->ground_y ;
			pl_w->pl_stance = STANCE_STAND ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_char_set( pl_w, Char_Catch, 3 ) ;
		}
	} else if( pl_char_move( pl_w ) < 0 ){
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 2 ;
		em->pl_dir = pl_w->pl_dir ;
		em->disp_dir = pl_w->disp_dir ;
		em->pl_stance = STANCE_AIR ;
		em->atck_no = pl0e_at_air_throw ;
		pos.x = em->pos.x ;
		pos.y = em->ground_y ;
		land_dust_set( &pos, em->pl_no ) ;
		catch_se_req( pl_w ) ;
		scr_excep_clr() ;
		scr_work.quake_level = 2 ;
		scr_work.quake_flag = 1 ;
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->spd.x = 5.0f ;
		pl_w->acc.x = -0.0390625f ;
		pl_w->spd.y = 18.75f ;
		pl_w->acc.y = -0.8705357f ;
		if( pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		pl_char_set( pl_w, Char_Catch, 4 ) ;
	}
}

/*--------------------------------------------------------------*/
static void jump_p_end( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
static void catch_dummy( PLAYER_WORK *pl_w )
{
}

static void hold( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void webthrow_hold( PLAYER_WORK *pl_w, PLAYER_WORK *em );

/****************************************************************/
void	pl0e_hold( PLAYER_WORK *pl_w )
{
	static void ( *hold_jmp[])( PLAYER_WORK *pl_w, PLAYER_WORK *em ) = {
		hold, hold, hold, webthrow_hold,
	} ;

	hold_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w, pl_w->catch_w ) ;
}

/*==============================================================*/
static void hold( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	catch_pos_set( em, pl_w ) ;
}

/*==============================================================*/
/*			For catch_no 3, the Web Throw's catch				*/
/*==============================================================*/
static void webthrow_hold( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	if( !pl_w->routine_2 ) pl_w->wall_ng_timer = 2 ;
	else catch_pos_set( em, pl_w ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl0e.c" End of File	*/

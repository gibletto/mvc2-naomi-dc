/* Sabretooth (character 2b): command checks, normal attacks, dashes, appearance, win poses,
   special moves and hyper combos, the partner moves and throws. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl2b_def.h"

void pl2b_main( PLAYER_WORK *pl_w );
void pl2b_com_ck( PLAYER_WORK *pl_w );
void pl2b_atck_set( PLAYER_WORK *pl_w );
void pl2b_atck( PLAYER_WORK *pl_w );
void pl2b_dash( PLAYER_WORK *pl_w );
void pl2b_noki( PLAYER_WORK *pl_w );
void pl2b_appear( PLAYER_WORK *pl_w );
void pl2b_win( PLAYER_WORK *pl_w );
void pl2b_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl2b_catch_ck( PLAYER_WORK *pl_w );
void pl2b_catch_set( PLAYER_WORK *pl_w );
void pl2b_catch( PLAYER_WORK *pl_w );
void pl2b_hold( PLAYER_WORK *pl_w );
int pl2b_delay_ck( PLAYER_WORK *pl_w );
void pl2b_t_kick( PLAYER_WORK *pl_w );
void pl2b_upper( PLAYER_WORK *pl_w );
void pl2b_sp_unison( PLAYER_WORK *pl_w );
void pl2b_sp_gattai( PLAYER_WORK *pl_w );
void pl2b_sp_assist( PLAYER_WORK *pl_w );
void pl2b_sp_zero( PLAYER_WORK *pl_w );

/* each attack's table: standing and crouching, then in the air */
static Sint8 lp_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 mp_tbl[4] = { -1,  2, -1,  2 } ;
static Sint8 hp_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 lk_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 mk_tbl[4] = { -1,  2, -1,  2 } ;
static Sint8 hk_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 j_lp_tbl[4] = {  1,  2,  0,  2 } ;
static Sint8 j_mp_tbl[4] = { -1,  2,  1,  2 } ;
static Sint8 j_hp_tbl[4] = { -1, -1, -1,  2 } ;
static Sint8 j_lk_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 j_mk_tbl[4] = { -1,  2, -1,  2 } ;
static Sint8 j_hk_tbl[4] = { -1, -1, -1, -1 } ;

/* the speeds of sp_berserk's dash and sp_fang's leap, for each strength */
static Float berserk_spd[2][2] = { { -30.0f, 0.0f }, { -DOT_X( 26.0f ), 0.0f } } ;
static Float fang_spd[2] = { -5.0f, -DOT_X( 7.0f ) } ;

/* commands: time or steps, kind, the special move, then the stick and buttons */
static Uint16 com_berserk[] = { 3, COM_GROUND, pl2b_sp_berserk, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Berserker Claw */
static Uint16 com_birdie[] = { 5, COM_GROUND, pl2b_sp_birdie, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Birdie */
static Uint16 com_fang[] = { 5, COM_GROUND, pl2b_sp_fang, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Wild Fang */
static Uint16 com_hbirdie[] = { 5, COM_GROUND | COM_HYPER | COM_ALL_SW, pl2b_sp_hbirdie, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Hyper Birdie */
static Uint16 com_clawx[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl2b_sp_clawx, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Berserker Claw X (hyper) */
static Uint16 com_weapon[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl2b_sp_weapon, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_PUNCH } ;	/* Weapon X Rush (hyper) */

static void ( *pl2b_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl2b_main,
	pl2b_com_ck,
	pl2b_atck_set,
	pl2b_atck,
	pl2b_dash,
	pl2b_noki,
	pl2b_appear,
	pl2b_win,
	pl2b_sp_atck,
	( void ( * )( PLAYER_WORK *pl_w ) )pl2b_catch_ck,	/* returns a value */
	pl2b_catch_set,
	pl2b_catch,
	pl2b_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl2b_delay_ck,
	pl2b_t_kick,
	pl2b_upper,
	pl2b_sp_unison,
	pl2b_sp_gattai,
	pl2b_sp_assist,
	pl2b_sp_zero,
} ;

static Uint8 berserk_ck( PLAYER_WORK *pl_w );
static Uint8 birdie_ck( PLAYER_WORK *pl_w );
static Uint8 fang_ck( PLAYER_WORK *pl_w );
static Uint8 clawx_ck( PLAYER_WORK *pl_w );
static Uint8 hbirdie_ck( PLAYER_WORK *pl_w );
static Uint8 weapon_ck( PLAYER_WORK *pl_w );
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w );
static Uint8 snapback_ck( PLAYER_WORK *pl_w );
static int clawx_delay_ck( PLAYER_WORK *pl_w );
static int hbirdie_delay_ck( PLAYER_WORK *pl_w );
static int weapon_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl2b_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl2b_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl2b_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( weapon_ck( pl_w ) ) return ;
	if( clawx_ck( pl_w ) ) return ;
	if( hbirdie_ck( pl_w ) ) return ;
	if( fang_ck( pl_w ) ) return ;
	if( birdie_ck( pl_w ) ) return ;
	if( berserk_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
static Uint8 berserk_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_berserk, &pl_w->com_w0 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl2b_sp_berserk ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 birdie_ck( PLAYER_WORK *pl_w )
{
	PL2B_LOCAL_W *lw = PL2B_LW( pl_w ) ;

	if( !com_nml_ck( pl_w, com_birdie, &pl_w->com_w1 ) ) return 0 ;
	if( lw->birdie_flag ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl2b_sp_birdie ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 fang_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_fang, &pl_w->com_w3 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w3 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl2b_sp_fang ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 clawx_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_clawx, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w4 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl2b_sp_clawx ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 hbirdie_ck( PLAYER_WORK *pl_w )
{
	PL2B_LOCAL_W *lw = PL2B_LW( pl_w ) ;

	if( !com_nml_ck( pl_w, com_hbirdie, &pl_w->com_w2 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( lw->birdie_flag ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl2b_sp_hbirdie ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 weapon_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_weapon, &pl_w->com_w5 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w5 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl2b_sp_weapon ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 9 ) ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl2b_sp_chouhatsu ;
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
	pl_w->pl_sp_type = pl2b_sp_snapback ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl2b_delay_ck( PLAYER_WORK *pl_w )
{
	if( clawx_delay_ck( pl_w ) || hbirdie_delay_ck( pl_w ) || weapon_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static int clawx_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_clawx, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl2b_sp_clawx ;
	return 1 ;
}

/*==============================================================*/
static int hbirdie_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_hbirdie, &pl_w->com_w2 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl2b_sp_hbirdie ;
	return 1 ;
}

/*==============================================================*/
static int weapon_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_weapon, &pl_w->com_w5 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl2b_sp_weapon ;
	return 1 ;
}

/****************************************************************/
void pl2b_main( PLAYER_WORK *pl_w )
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
void pl2b_atck_set( PLAYER_WORK *pl_w )
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
	Sint8 no ;
	int voice ;
	Uint8 atck ;

	switch( pl_w->nml_no ){
	case 0:
		atck = 0 ;
		no = 0 ;
		voice = 20 ;
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		atck = 1 ;
		no = 1 ;
		voice = 21 ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		atck = 2 ;
		no = 2 ;
		voice = 22 ;
		pl_w->chain_tbl = hp_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	pl_w->atck_no = atck ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_se_req( pl_w, voice ) ;
	pl_char_set( pl_w, Char_Stand_P, no ) ;
}

/*==============================================================*/
static void crouch_punch( PLAYER_WORK *pl_w )
{
	Sint8 no ;
	int voice ;
	Uint8 atck ;

	switch( pl_w->nml_no ){
	case 0:
		atck = 6 ;
		no = 0 ;
		voice = 20 ;
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		atck = 7 ;
		no = 1 ;
		voice = 21 ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		atck = 8 ;
		no = 2 ;
		voice = 22 ;
		pl_w->chain_tbl = hp_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	pl_w->atck_no = atck ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_se_req( pl_w, voice ) ;
	pl_char_set( pl_w, Char_Crouch_P, no ) ;
}

/*==============================================================*/
static void stand_kick( PLAYER_WORK *pl_w )
{
	Sint8 no ;
	int voice ;
	Uint8 atck ;

	switch( pl_w->nml_no ){
	case 0:
		atck = 3 ;
		no = 0 ;
		voice = 20 ;
		pl_w->chain_tbl = lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		atck = 4 ;
		no = 1 ;
		voice = 21 ;
		pl_w->chain_tbl = mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		atck = 5 ;
		no = 2 ;
		voice = 22 ;
		pl_w->chain_tbl = hk_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	pl_w->atck_no = atck ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_se_req( pl_w, voice ) ;
	pl_char_set( pl_w, Char_Stand_K, no ) ;
}

/*==============================================================*/
static void crouch_kick( PLAYER_WORK *pl_w )
{
	Sint8 no ;
	int voice ;
	Uint8 atck ;

	switch( pl_w->nml_no ){
	case 0:
		atck = 9 ;
		no = 0 ;
		voice = 20 ;
		pl_w->chain_tbl = lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		atck = 10 ;
		no = 1 ;
		voice = 21 ;
		pl_w->chain_tbl = mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		atck = 11 ;
		no = 2 ;
		voice = 22 ;
		pl_w->chain_tbl = hk_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	pl_w->atck_no = atck ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_se_req( pl_w, voice ) ;
	pl_char_set( pl_w, Char_Crouch_K, no ) ;
}

/*==============================================================*/
/*			In a jump, only while that button's attacks are		*/
/*			left												*/
/*==============================================================*/
static void jump_atck_ck( PLAYER_WORK *pl_w )
{
	if( !pl_w->atck_pk && ( pl_w->jump_atk_cnt & JATK_P_MASK ) ){
		jump_atck( pl_w ) ;
		return ;
	}
	if( pl_w->atck_pk && ( pl_w->jump_atk_cnt & JATK_K_MASK ) ) jump_atck( pl_w ) ;
}

/*==============================================================*/
static void jump_atck( PLAYER_WORK *pl_w )
{
	if( !pl_w->atck_pk ) jump_punch( pl_w ) ;
	else jump_kick( pl_w ) ;
}

/*==============================================================*/
static void jump_punch( PLAYER_WORK *pl_w )
{
	Sint8 no ;
	int voice ;
	Uint8 atck ;

	switch( pl_w->nml_no ){
	case 0:
		atck = 12 ;
		no = 0 ;
		voice = 20 ;
		pl_w->chain_tbl = j_lp_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		atck = 13 ;
		no = 1 ;
		voice = 21 ;
		pl_w->chain_tbl = j_mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		atck = 14 ;
		no = 2 ;
		voice = 22 ;
		pl_w->chain_tbl = j_hp_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	pl_w->atck_no = atck ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_se_req( pl_w, voice ) ;
	pl_char_set( pl_w, Char_Jump_P, no ) ;
	if( pl_w->jump_atk_cnt & JATK_P_MASK ) pl_w->jump_atk_cnt -= 0x01 ;
}

/*==============================================================*/
static void jump_kick( PLAYER_WORK *pl_w )
{
	Sint8 no ;
	int voice ;
	Uint8 atck ;

	switch( pl_w->nml_no ){
	case 0:
		atck = 15 ;
		no = 0 ;
		voice = 20 ;
		pl_w->chain_tbl = j_lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		atck = 16 ;
		no = 1 ;
		voice = 21 ;
		pl_w->chain_tbl = j_mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		atck = 17 ;
		no = 2 ;
		voice = 22 ;
		pl_w->chain_tbl = j_hk_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	pl_w->atck_no = atck ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_se_req( pl_w, voice ) ;
	pl_char_set( pl_w, Char_Jump_K, no ) ;
}

static void nml_move( PLAYER_WORK *pl_w );
static void jump_move( PLAYER_WORK *pl_w );
static void sjump_move( PLAYER_WORK *pl_w );
static void nml_move_dash( PLAYER_WORK *pl_w );
static void stand_punch_move( PLAYER_WORK *pl_w );
static void crouch_punch_move( PLAYER_WORK *pl_w );
static void stand_kick_move( PLAYER_WORK *pl_w );
static void crouch_kick_move( PLAYER_WORK *pl_w );
static void jump_punch_move( PLAYER_WORK *pl_w );
static void jump_kick_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl2b_atck( PLAYER_WORK *pl_w )
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
/*			The light and heavy punches hit a second time		*/
/*==============================================================*/
static void stand_punch_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	switch( pl_w->nml_no ){
	case 0:
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pl_w->atck_no = pl2b_at_stand_lp_2 ;
			HIT_RESET( pl_w ) ;
			pl_se_req( pl_w, SE_SWING_L ) ;
		}
		break ;
	case 1:
		break ;
	case 2:
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pl_w->atck_no = pl2b_at_stand_hp_2 ;
			HIT_RESET( pl_w ) ;
			pl_se_req( pl_w, SE_SWING_H ) ;
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
	if( !pl_w->atck_pk ) jump_punch_move( pl_w ) ;
	else jump_kick_move( pl_w ) ;
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
void pl2b_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_w->atck_no = pl2b_at_upper ;
		HIT_RESET( pl_w ) ;
		pl_brake_set( pl_w ) ;
		pl_gauge_add( pl_w, 5 ) ;
		pl_char_set( pl_w, Char_Extra, 5 ) ;
		pl_se_req( pl_w, SE_SWING_H ) ;
	}
	pl_w->atck_type = 3 ;
	pl_smoke_set( pl_w ) ;
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
void pl2b_dash( PLAYER_WORK *pl_w )
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
		pl_w->timer0 = 34 ;
		pl_w->spd.x = -DOT_X( 9.5f ) ;
		pl_w->acc.x = 0.0f ;
		if( pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
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
	if( pl_w->timer0 == 22 ){
		pl_w->spd.x = -10.0f ;
		pl_w->acc.x = DOT_X( 0.25f ) ;
		if( pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		pl_char_set( pl_w, Char_Dash, 2 ) ;
	}
	if( !-- pl_w->timer0 ) pl_w->routine_2 ++ ;
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
/*			The back dash, a hop								*/
/****************************************************************/
void pl2b_noki( PLAYER_WORK *pl_w )
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
		pl_w->spd.x = DOT_X( 9.5f ) ;
		pl_w->acc.x = -0.3125f ;
		pl_w->spd.y = DOT_Y( 3.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.25f ) ;
		if( pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
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
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
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
static void appear_ef_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl2b_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_init, appear_move, appear_ef_move,
	} ;

	appear_jmp[pl_w->type]( pl_w ) ;
}

/*==============================================================*/
/*			One of two entrances, picked at random				*/
/*==============================================================*/
static void appear_init( PLAYER_WORK *pl_w )
{
	pl_w->type = ( Rnd() & 1 ) + 1 ;
	pl_w->disp_flag = 1 ;
	pl_char_set( pl_w, Char_Appear, pl_w->type - 1 ) ;
	if( pl_w->type == 2 ) pls2b_06_set( pl_w, 2 ) ;
}

/*==============================================================*/
static void appear_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_1 ++ ;
		pl_char_set( pl_w, Char_Stand, 0 ) ;
	}
}

/*==============================================================*/
static void appear_ef_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_1 ++ ;
		pl_char_set( pl_w, Char_Stand, 0 ) ;
	}
}

static void win_init( PLAYER_WORK *pl_w );
static void win_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl2b_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_init, win_move,
	} ;

	win_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void win_init( PLAYER_WORK *pl_w )
{
	int no ;

	pl_w->routine_2 ++ ;
	switch( pl_w->type ){
	case 0:
		pl_w->sub_type = ( Rnd() & 0x0f ) % 3 ;
		if( !vs_sys_work.vs_end_flag ){
			pl_w->type = 2 ;
			pl_w->sub_type = 0 ;
			no = 0 ;
			break ;
		}
		if( pl_be_cnt( pl_w ) > 1 ){
			pl_w->type = 2 ;
			pl_w->sub_type = 0 ;
			no = 0 ;
			break ;
		}
		no = pl_w->sub_type & 1 ;
		if( pl_w->sub_type != 2 ){
			pl_w->win_wait = 0xff ;
			pls2b_06_set( pl_w, pl_w->sub_type ) ;
		}
		break ;
	case 2:
		pl_w->sub_type = 0 ;
		no = 0 ;
		break ;
	case 1:
	case 3:
	case 4:
		no = 3 ;
	}
	pl_char_set( pl_w, Char_Win, no ) ;
}

/*--------------------------------------------------------------*/
static void win_move( PLAYER_WORK *pl_w )
{
	int voice ;

	if( pl_w->act_st == N_WIN && pl_win_end_ck( pl_w ) ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	switch( pl_w->type ){
	case 0:
		if( ( pl_w->sub_type & 0x7f ) != 2 ) pl_w->win_wait = 0xff ;
		if( ( pl_w->sub_type & 0x7f ) == 1 ){
			pl_char_move( pl_w ) ;
			if( pl_w->sub_type & 0x80 ){
				pl_w->sub_type = 3 ;
				pl_voice_req( pl_w, 17 ) ;
				pl_char_set( pl_w, Char_Win, 2 ) ;
			}
		} else {
			if( pl_w->cg_flag1 ){
				pl_w->cg_flag1 = 0 ;
				if( pl_w->sub_type ) voice = 15 ;
				if( pl_w->sub_type == 2 ) voice = 16 ;
				pl_voice_req( pl_w, voice ) ;
			}
			pl_char_move( pl_w ) ;
		}
		break ;
	case 2:
	case 1:
	case 3:
	case 4:
		pl_char_move( pl_w ) ;
		break ;
	}
}

static void sp_berserk( PLAYER_WORK *pl_w );
static void sp_birdie( PLAYER_WORK *pl_w );
static void sp_fang( PLAYER_WORK *pl_w );
static void sp_clawx( PLAYER_WORK *pl_w );
static void sp_hbirdie( PLAYER_WORK *pl_w );
static void sp_weapon( PLAYER_WORK *pl_w );
static void sp_crouch_hp( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );

/****************************************************************/
void pl2b_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *pl_sp_tbl[])( PLAYER_WORK *pl_w ) = {
		sp_berserk, sp_birdie, sp_fang, sp_clawx, sp_hbirdie, sp_weapon, sp_berserk, sp_crouch_hp, sp_snapback, sp_chouhatsu,
	} ;

	pl_sp_tbl[pl_w->pl_sp_type]( pl_w ) ;
}

static void berserk_init( PLAYER_WORK *pl_w );
static void berserk_move( PLAYER_WORK *pl_w );
static void berserk_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A dash along the ground; slower if it hit			*/
/*==============================================================*/
static void sp_berserk( PLAYER_WORK *pl_w )
{
	static void ( *berserk_jmp[])( PLAYER_WORK *pl_w ) = {
		berserk_init, berserk_move, berserk_end,
	} ;

	berserk_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void berserk_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->timer0 = 20 ;
	if( pl_w->atck_level ) pl_w->timer0 = 32 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl_w->atck_level + 48 ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level ) ;
}

/*--------------------------------------------------------------*/
static void berserk_move( PLAYER_WORK *pl_w )
{
	Sint8 no ;

	pl_w->push_ng_timer = 2 ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->spd.x = berserk_spd[pl_w->atck_level][0] ;
		pl_w->acc.x = berserk_spd[pl_w->atck_level][1] ;
		if( pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		pls2b_01_set( pl_w ) ;
		pls2b_02_set( pl_w, 1, 0 ) ;
		pls2b_02_set( pl_w, 1, 1 ) ;
	}
	if( pl_w->hit_flag && ( pl_w->hit_flag & HR_GUARD ) ) pl_w->timer0 = 1 ;
	pl_char_move( pl_w ) ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->push_ng_timer = 0 ;
		if( pl_w->hit_flag && !( pl_w->hit_flag & HR_GUARD ) ){
			pl_w->spd.x = -DOT_X( 5.0f ) ;
			pl_w->acc.x = DOT_X( 0.125f ) ;
			no = 3 ;
		} else {
			pl_w->spd.x = -5.0f ;
			pl_w->acc.x = DOT_X( 0.125f ) ;
			no = 4 ;
		}
		if( pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		pl_char_set( pl_w, Char_Special, no ) ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
}

/*--------------------------------------------------------------*/
static void berserk_end( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_w->spd.x * pl_w->acc.x >= 0.0f ){
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
	} else if( Sys->game_timer0 & 1 ) pl_smoke_set( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void birdie_init( PLAYER_WORK *pl_w, PL2B_LOCAL_W *lw );
static void birdie_move( PLAYER_WORK *pl_w, PL2B_LOCAL_W *lw );

/*==============================================================*/
/*			Fires the helper effect, shl2b_00_set( pl_w, 0		*/
/*			), while none is out								*/
/*==============================================================*/
static void sp_birdie( PLAYER_WORK *pl_w )
{
	static void ( *birdie_jmp[])( PLAYER_WORK *pl_w, PL2B_LOCAL_W *lw ) = {
		birdie_init, birdie_move,
	} ;

	birdie_jmp[pl_w->routine_2]( pl_w, PL2B_LW( pl_w ) ) ;
}

/*==============================================================*/
static void birdie_init( PLAYER_WORK *pl_w, PL2B_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl2b_at_birdie ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_char_set( pl_w, Char_Special, 5 ) ;
}

/*--------------------------------------------------------------*/
static void birdie_move( PLAYER_WORK *pl_w, PL2B_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shl2b_00_set( pl_w, 0 ) ;
	}
}

static void fang_init( PLAYER_WORK *pl_w );
static void fang_move( PLAYER_WORK *pl_w );
static void fang_end( PLAYER_WORK *pl_w );
static void fang_jump( PLAYER_WORK *pl_w );
static void fang_fly( PLAYER_WORK *pl_w );
static void fang_land( PLAYER_WORK *pl_w );
static void fang_throw( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A leap that, if it catches, throws					*/
/*==============================================================*/
static void sp_fang( PLAYER_WORK *pl_w )
{
	static void ( *fang_jmp[])( PLAYER_WORK *pl_w ) = {
		fang_init, fang_move, fang_end,
	} ;

	fang_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void fang_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl2b_at_fang ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_char_set( pl_w, Char_Special, 6 ) ;
}

/*--------------------------------------------------------------*/
static void fang_move( PLAYER_WORK *pl_w )
{
	static void ( *fang_move_jmp[])( PLAYER_WORK *pl_w ) = {
		fang_jump, fang_fly, fang_land, fang_throw,
	} ;

	fang_move_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void fang_jump( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		pl_w->cg_type = 0 ;
		pl_air_set( pl_w ) ;
		pl_w->spd.x = fang_spd[pl_w->atck_level] ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 12.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.625f ) ;
		if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
	}
}

/*--------------------------------------------------------------*/
static void fang_fly( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->hit_flag ){
		if( pl_sp_catch_ck( pl_w ) ){
			pl_w->routine_3 ++ ;
			pl_w->dm_flag = 0 ;
			pl_w->catch_no = 0xc2 ;
			pl_w->catch_grip_old = -1 ;
			pl_w->catch_flag = 1 ;
			pl_w->sp_ng_timer = 3 ;
			pl_sp_catch_set( pl_w, pl_w->em_w ) ;
			pl_w->spd.x = -5.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->spd.y = DOT_Y( 6.0f ) ;
			pl_w->acc.y = -DOT_Y( 0.5f ) ;
			if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
			pl_char_set( pl_w, Char_Catch, 5 ) ;
			scr_excep_set( pl_w, 5, 5 ) ;
		} else {
			pl_w->routine_2 ++ ;
			pl_w->routine_3 = 1 ;
			pl_w->spd.x = -( pl_w->spd.x / 8.0f ) ;
			pl_w->spd.y = DOT_Y( 8.0f ) ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = -DOT_Y( 0.5f ) ;
			pl_char_set( pl_w, Char_Special, 10 ) ;
		}
	} else if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Jump, 3 ) ;
		pl_land_ef_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void fang_land( PLAYER_WORK *pl_w )
{
	pl_w->sp_ng_timer = 3 ;
	pl_w->catch_flag = 1 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->spd.y < 0.0f && pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_3 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Catch, 6 ) ;
		pl_land_ef_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void fang_throw( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_w->sp_ng_timer = 3 ;
	pl_w->catch_flag = 1 ;
	if( pl_char_move( pl_w ) < 0 ){
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->atck_no = pl2b_at_fang_throw ;
		pl_w->dir = 25 ;
		if( pl_w->pl_dir ) pl_w->dir = 32 - pl_w->dir ;
		scr_excep_clr() ;
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_char_set( pl_w, Char_Special, 9 ) ;
	}
}

/*--------------------------------------------------------------*/
static void fang_end( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_3 ){
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	} else {
		pl_char_move( pl_w ) ;
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->spd.y < 0.0f && pl_w->pos.y < pl_w->ground_y ){
			pl_w->pl_stance = STANCE_STAND ;
			pl_w->pos.y = pl_w->ground_y ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_land_set( pl_w ) ;
		}
	}
}

static void clawx_init( PLAYER_WORK *pl_w );
static void clawx_move( PLAYER_WORK *pl_w );
static void clawx_end( PLAYER_WORK *pl_w );
static void clawx_start( PLAYER_WORK *pl_w );
static void clawx_flash( PLAYER_WORK *pl_w );
static void clawx_pose( PLAYER_WORK *pl_w );
static void clawx_ready( PLAYER_WORK *pl_w );
static void clawx_rush( PLAYER_WORK *pl_w );
static void clawx_hit( PLAYER_WORK *pl_w );
static void clawx_last( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo: a rush that, if it hits, becomes		*/
/*			a run of blows										*/
/*==============================================================*/
static void sp_clawx( PLAYER_WORK *pl_w )
{
	static void ( *clawx_jmp[])( PLAYER_WORK *pl_w ) = {
		clawx_init, clawx_move, clawx_end,
	} ;

	clawx_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void clawx_init( PLAYER_WORK *pl_w )
{
	static void ( *clawx_init_jmp[])( PLAYER_WORK *pl_w ) = {
		clawx_start, clawx_flash, clawx_pose, clawx_ready,
	} ;

	clawx_init_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void clawx_start( PLAYER_WORK *pl_w )
{
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
	pl_w->atck_no = pl2b_at_clawx ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_w->atck_attr = ATR_BACK_POS ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 0 ) ;
}

/*--------------------------------------------------------------*/
static void clawx_flash( PLAYER_WORK *pl_w )
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
		ofs.x = 0.0f ;
		ofs.y = DOT_Y( 96.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void clawx_pose( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->routine_3 ++ ;
	pl_char_set2( pl_w, Char_Super, 0, 7 ) ;
}

/*--------------------------------------------------------------*/
static void clawx_ready( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->routine_2 ++ ;
	pl_w->routine_3 = 0 ;
	pl_w->timer0 = 20 ;
	pl_w->timer1 = 0 ;
	pl_char_set( pl_w, Char_Super, 1 ) ;
}

/*--------------------------------------------------------------*/
static void clawx_move( PLAYER_WORK *pl_w )
{
	static void ( *clawx_move_jmp[])( PLAYER_WORK *pl_w ) = {
		clawx_rush, clawx_hit, clawx_last,
	} ;

	pl_w->push_ng_timer = 2 ;
	clawx_move_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void clawx_rush( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	Float ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->spd.x = -DOT_X( 38.0f ) ;
		pl_w->acc.x = 0.0f ;
		if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
		pl_w->atck_no = pl_w->timer1 + 53 ;
		HIT_RESET( pl_w ) ;
		pls2b_01_set( pl_w ) ;
		pls2b_02_set( pl_w, ( pl_w->timer1 & 1 ) ^ 1, 0 ) ;
		pls2b_02_set( pl_w, ( pl_w->timer1 & 1 ) ^ 1, 1 ) ;
	}
	if( !-- pl_w->timer0 ){
		if( pl_w->hit_flag && !( pl_w->hit_flag & HR_GUARD ) ){
			if( pl_sp_catch_ck( pl_w ) ){
				em = pl_w->em_w ;
				ofs = DOT_X( 16.0f ) ;
				if( pl_w->pl_dir ) ofs = -DOT_X( 16.0f ) ;
				em->pos.x += ofs ;
				em->pos.y = pl_w->ground_y ;
				em->pl_stance = STANCE_STAND ;
				pl_w->routine_3 ++ ;
				pl_w->spd.x = -DOT_X( 4.0f ) ;
				pl_w->acc.x = 1.6666666f ;
				if( pl_w->pl_dir ){
					pl_w->spd.x = -pl_w->spd.x ;
					pl_w->acc.x = -pl_w->acc.x ;
				}
				pl_char_set( pl_w, Char_Super, 5 ) ;
			} else {
				pl_w->delay_flag = 0 ;
				pl_w->delay_timer = 0 ;
				pl_w->hyper_bg = 0 ;
				pl_w->hyper_bg_tm = 0 ;
				pl_w->routine_2 ++ ;
				pl_w->routine_3 = 0 ;
				pl_w->spd.x = -DOT_X( 4.0f ) ;
				pl_w->acc.x = DOT_X( 0.25f ) ;
				if( pl_w->pl_dir ){
					pl_w->spd.x = -pl_w->spd.x ;
					pl_w->acc.x = -pl_w->acc.x ;
				}
				pl_char_set( pl_w, Char_Super, 8 ) ;
			}
		} else {
			pl_w->delay_flag = 0 ;
			pl_w->delay_timer = 0 ;
			pl_w->hyper_bg = 0 ;
			pl_w->hyper_bg_tm = 0 ;
			pl_w->routine_2 ++ ;
			pl_w->routine_3 = 0 ;
			pl_w->spd.x = -DOT_X( 4.0f ) ;
			pl_w->acc.x = DOT_X( 0.25f ) ;
			if( pl_w->pl_dir ){
				pl_w->spd.x = -pl_w->spd.x ;
				pl_w->acc.x = -pl_w->acc.x ;
			}
			pl_char_set( pl_w, Char_Super, 8 ) ;
		}
	}
}

/*==============================================================*/
/*			Three blows, then the last							*/
/*==============================================================*/
static void clawx_hit( PLAYER_WORK *pl_w )
{
	Sint8 no ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_w->spd.x > 0.0f ) pl_smoke_set( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->timer0 = 20 ;
		if( ++ pl_w->timer1 == 3 ){
			pl_w->routine_3 ++ ;
			no = 6 ;
		} else {
			pl_w->routine_3 -- ;
			no = ( ( pl_w->timer1 & 1 ) << 1 ) + 1 ;
		}
		pl_dir_set( pl_w ) ;
		pl_char_set( pl_w, Char_Super, no ) ;
	} else if( pl_w->cg_type & 1 ){
		pl_w->cg_type = 0 ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
	}
}

/*--------------------------------------------------------------*/
static void clawx_last( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->spd.x = -DOT_X( 26.0f ) ;
		pl_w->acc.x = 0.0f ;
		if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
		pl_w->atck_no = pl2b_at_clawx_last ;
		HIT_RESET( pl_w ) ;
		pls2b_01_set( pl_w ) ;
		pls2b_04_set( pl_w, 0, 0 ) ;
		pls2b_04_set( pl_w, 0, 1 ) ;
		pls2b_04_set( pl_w, 1, 0 ) ;
		pls2b_04_set( pl_w, 1, 1 ) ;
	}
	if( !-- pl_w->timer0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->spd.x = -DOT_X( 4.0f ) ;
		pl_w->acc.x = DOT_X( 0.125f ) ;
		if( pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		pl_char_set( pl_w, Char_Super, 8 ) ;
	}
}

/*--------------------------------------------------------------*/
static void clawx_end( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_w->spd.x > 0.0f ) pl_smoke_set( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void hbirdie_init( PLAYER_WORK *pl_w, PL2B_LOCAL_W *lw );
static void hbirdie_move( PLAYER_WORK *pl_w, PL2B_LOCAL_W *lw );
static void hbirdie_end( PLAYER_WORK *pl_w, PL2B_LOCAL_W *lw );

/*==============================================================*/
/*			A hyper combo: the helper effect, shl2b_00_set(		*/
/*			pl_w, 1 ), held until it is gone					*/
/*==============================================================*/
static void sp_hbirdie( PLAYER_WORK *pl_w )
{
	static void ( *hbirdie_jmp[])( PLAYER_WORK *pl_w, PL2B_LOCAL_W *lw ) = {
		hbirdie_init, hbirdie_move, hbirdie_end,
	} ;

	hbirdie_jmp[pl_w->routine_2]( pl_w, PL2B_LW( pl_w ) ) ;
}

/*==============================================================*/
static void hbirdie_init( PLAYER_WORK *pl_w, PL2B_LOCAL_W *lw )
{
	VEC3 ofs ;

	if( !pl_w->routine_3 ){
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
		pl_w->atck_no = pl2b_at_hbirdie ;
		HIT_RESET( pl_w ) ;
		pl_dir_set( pl_w ) ;
		pl_sp_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 9 ) ;
	} else {
		pl_w->delay_timer = 2 ;
		pl_w->hyper_bg_tm = 5 ;
		pl_char_move( pl_w ) ;
		if( pl_w->cg_type ){
			pl_w->stop_move_flag = 0 ;
			pl_w->stop_move_timer = 0 ;
			pl_w->routine_2 ++ ;
			pl_w->routine_3 = 0 ;
			pl_w->cg_type = 0 ;
			ofs.x = 0.0f ;
			ofs.y = DOT_Y( 96.0f ) ;
			pl_hyper_start( pl_w, &ofs, 1 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void hbirdie_move( PLAYER_WORK *pl_w, PL2B_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( !pl_w->routine_3 ){
		if( pl_w->cg_type ){
			pl_w->routine_3 ++ ;
			pl_w->cg_type = 0 ;
			shl2b_00_set( pl_w, 1 ) ;
		}
	} else if( !lw->birdie_flag ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_char_set( pl_w, Char_Super, 10 ) ;
	}
}

/*--------------------------------------------------------------*/
static void hbirdie_end( PLAYER_WORK *pl_w, PL2B_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void weapon_init( PLAYER_WORK *pl_w );
static void weapon_move( PLAYER_WORK *pl_w );
static void weapon_end( PLAYER_WORK *pl_w );
static void weapon_start( PLAYER_WORK *pl_w );
static void weapon_flash( PLAYER_WORK *pl_w );
static void weapon_pose( PLAYER_WORK *pl_w );
static void weapon_ready( PLAYER_WORK *pl_w );
static void weapon_jump( PLAYER_WORK *pl_w, PL2B_LOCAL_W *lw );
static void weapon_drag( PLAYER_WORK *pl_w, PL2B_LOCAL_W *lw );
static void weapon_dash( PLAYER_WORK *pl_w, PL2B_LOCAL_W *lw );
static void weapon_slide( PLAYER_WORK *pl_w, PL2B_LOCAL_W *lw );

/*==============================================================*/
/*			A hyper combo: a leap that, if it catches, drags	*/
/*			the opponent along the ground						*/
/*==============================================================*/
static void sp_weapon( PLAYER_WORK *pl_w )
{
	static void ( *weapon_jmp[])( PLAYER_WORK *pl_w ) = {
		weapon_init, weapon_move, weapon_end,
	} ;

	weapon_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void weapon_init( PLAYER_WORK *pl_w )
{
	static void ( *weapon_init_jmp[])( PLAYER_WORK *pl_w ) = {
		weapon_start, weapon_flash, weapon_pose, weapon_ready,
	} ;

	weapon_init_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void weapon_start( PLAYER_WORK *pl_w )
{
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
	pl_w->atck_no = pl2b_at_weapon ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 11 ) ;
}

/*--------------------------------------------------------------*/
static void weapon_flash( PLAYER_WORK *pl_w )
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
		ofs.x = 0.0f ;
		ofs.y = DOT_Y( 96.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void weapon_pose( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->routine_3 ++ ;
	pl_char_set2( pl_w, Char_Super, 11, 7 ) ;
}

/*--------------------------------------------------------------*/
static void weapon_ready( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_char_set( pl_w, Char_Super, 12 ) ;
	}
}

/*--------------------------------------------------------------*/
static void weapon_move( PLAYER_WORK *pl_w )
{
	static void ( *weapon_move_jmp[])( PLAYER_WORK *pl_w, PL2B_LOCAL_W *lw ) = {
		weapon_jump, weapon_drag, weapon_dash, weapon_slide,
	} ;

	weapon_move_jmp[pl_w->routine_3]( pl_w, PL2B_LW( pl_w ) ) ;
}

/*==============================================================*/
static void weapon_jump( PLAYER_WORK *pl_w, PL2B_LOCAL_W *lw )
{
	PLAYER_WORK *em ;
	Float ofs ;
	Sint8 no ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->push_ng_timer = 2 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->spd.x = -10.0f ;
		if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 10.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.625f ) ;
	}
	if( pl_w->hit_flag ){
		if( !pl_sp_catch_ck( pl_w ) ){
			pl_w->delay_flag = 0 ;
			pl_w->delay_timer = 0 ;
			pl_w->hyper_bg = 0 ;
			pl_w->hyper_bg_tm = 0 ;
			pl_w->routine_2 ++ ;
			pl_w->routine_3 = 1 ;
			pl_w->spd.x = DOT_X( 2.0f ) ;
			if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = 0.0f ;
			pl_w->spd.y = DOT_Y( 3.0f ) ;
			pl_w->acc.y = -DOT_Y( 0.375f ) ;
			pl_w->pl_stance = STANCE_AIR ;
			pl_char_set2( pl_w, Char_Jump, 2, 13 ) ;
		} else {
			pl_w->routine_3 ++ ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_dir_set( pl_w ) ;
			em = pl_w->em_w ;
			em->pl_stance = STANCE_STAND ;
			pl_w->pl_stance = STANCE_STAND ;
			pl_w->pos.y = em->pos.y = pl_w->ground_y ;
			em->pos.x = pl_w->pos.x ;
			ofs = -80.0f ;
			pl_w->spd.x = -DOT_X( 0.125f ) ;
			if( pl_w->pl_dir ){
				ofs = 80.0f ;
				pl_w->spd.x = -pl_w->spd.x ;
			}
			em->pos.x += ofs ;
			lw->drag_w = em ;
			no = pl_w->pl_no ? 3 : 4 ;
			scr_excep_set( pl_w, no, no ) ;
			pl_char_set( pl_w, Char_Super, 13 ) ;
		}
	} else if( pl_w->pos.y + DOT_Y( 16.0f ) < pl_w->ground_y ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 2 ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Super, 15 ) ;
	}
}

/*==============================================================*/
/*			Dragging the opponent: each blow the animation		*/
/*			marks is a hit										*/
/*==============================================================*/
static void weapon_drag( PLAYER_WORK *pl_w, PL2B_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_char_move( pl_w ) ;
	if( pl_w->hit_flag ){
		em = lw->drag_w ;
		if( pl_w->em_w != em ){
			scr_excep_clr() ;
			pl_stand_ret( pl_w ) ;
		}
	}
	if( pl_w->cg_flag1 ){
		pl_w->atck_no = pl_w->cg_flag1 + 59 ;
		HIT_RESET( pl_w ) ;
		pl_w->cg_flag1 = 0 ;
	}
	if( pl_w->cg_type ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg_tm = 5 ;
		pl_w->routine_3 ++ ;
		pl_w->timer0 = 50 ;
		pl_w->push_ng_timer = 2 ;
		pl_w->spd.x = -DOT_X( 0.5f ) ;
		if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
		em = lw->drag_w ;
		pl_w->pos.x = em->pos.x ;
		shl2b_02_set( pl_w, 0 ) ;
		shl2b_02_set( pl_w, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void weapon_dash( PLAYER_WORK *pl_w, PL2B_LOCAL_W *lw )
{
	pl_w->hyper_bg_tm = 5 ;
	pl_w->push_ng_timer = 2 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_char_move( pl_w ) ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_3 ++ ;
		pl_w->timer0 = 8 ;
		pl_w->spd.x = -DOT_X( 16.0f ) ;
		if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
		scr_excep_clr() ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void weapon_slide( PLAYER_WORK *pl_w, PL2B_LOCAL_W *lw )
{
	pl_w->push_ng_timer = 2 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->spd.x = -DOT_X( 4.0f ) ;
		pl_w->acc.x = DOT_X( 0.125f ) ;
		if( pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		pl_char_set( pl_w, Char_Super, 14 ) ;
	}
}

/*--------------------------------------------------------------*/
static void weapon_end( PLAYER_WORK *pl_w )
{
	switch( pl_w->routine_3 ){
	case 0:
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->spd.x > 0.0f ) pl_smoke_set( pl_w ) ;
		pl_char_move( pl_w ) ;
		if( pl_w->cg_type ){
			pl_w->routine_3 = 2 ;
			pl_w->cg_type = 0 ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
		}
		break ;
	case 1:
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		pl_char_move( pl_w ) ;
		if( pl_w->pos.y < pl_w->ground_y ){
			pl_w->routine_3 ++ ;
			pl_w->pl_stance = STANCE_STAND ;
			pl_w->pos.y = pl_w->ground_y ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_char_set( pl_w, Char_Jump, 3 ) ;
			pl_land_ef_set( pl_w ) ;
		}
		break ;
	case 2:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	}
}

static void crouch_hp_init( PLAYER_WORK *pl_w );
static void crouch_hp_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_crouch_hp( PLAYER_WORK *pl_w )
{
	static void ( *crouch_hp_jmp[])( PLAYER_WORK *pl_w ) = {
		crouch_hp_init, crouch_hp_move,
	} ;

	crouch_hp_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void crouch_hp_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl2b_at_crouch_hp ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 11 ) ;
	pl_se_req( pl_w, SE_SWING_L ) ;
}

/*--------------------------------------------------------------*/
static void crouch_hp_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->atck_no = pl2b_at_crouch_hp_2 ;
		HIT_RESET( pl_w ) ;
		pl_se_req( pl_w, SE_SWING_H ) ;
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
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl2b_at_snapback ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 12 ) ;
}

/*--------------------------------------------------------------*/
static void snapback_move( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	else {
		if( pl_w->cg_type & 1 ){
			pl_w->cg_type ^= 1 ;
			ofs.x = DOT_X( 16.0f ) ;
			ofs.y = DOT_Y( 88.0f ) ;
			pl_snap_flash( pl_w, &ofs ) ;
		}
		if( pl_w->cg_type & 2 ){
			pl_w->cg_type ^= 2 ;
			pl_se_req( pl_w, SE_SWING_H ) ;
		}
	}
}

static void chouhatsu_init( PLAYER_WORK *pl_w );
static void chouhatsu_move( PLAYER_WORK *pl_w );
static void chouhatsu_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w )
{
	static void ( *chouhatsu_jmp[])( PLAYER_WORK *pl_w ) = {
		chouhatsu_init, chouhatsu_move, chouhatsu_end,
	} ;

	chouhatsu_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void chouhatsu_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->timer0 = 60 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_dir_set( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 2 ) ;
}

/*--------------------------------------------------------------*/
static void chouhatsu_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Extra, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void chouhatsu_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void pl2b_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl2b_sp_clawx ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl2b_sp_clawx ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl2b_sp_hbirdie ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl2b_sp_hbirdie ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl2b_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl2b_sp_clawx ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl2b_sp_hbirdie ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl2b_sp_hbirdie ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl2b_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl2b_sp_berserk ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl2b_sp_birdie ;
		pl_w->atck_level = 0 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl2b_sp_crouch_hp ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void pl2b_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl2b_sp_berserk ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl2b_sp_birdie ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl2b_sp_crouch_hp ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_jump( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The partner's jump in								*/
/****************************************************************/
void pl2b_t_kick( PLAYER_WORK *pl_w )
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
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->spd.x = -30.0f ;
	pl_w->spd.y = -DOT_Y( 4.5f ) ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	if( pl_w->pl_dir ){
		pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = -pl_w->acc.x ;
	}
	pl_w->atck_no = pl2b_at_t_kick ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
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
	if( pl_w->pos.y < pl_w->ground_y + -DOT_Y( 40.0f ) ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->timer0 = 6 ;
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
	if( pl_char_move( pl_w ) < 0 && !-- pl_w->timer0 ) pl_stand_ret( pl_w ) ;
}

/****************************************************************/
/*			Can a throw start? on the ground, or in the air		*/
/*			when high enough									*/
/****************************************************************/
PLAYER_WORK *pl2b_catch_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = 0 ;

	if( !pl_w->atck_level || pl_w->atck_pk || !( pl_w->atck_lever & ( KEY_FORWARD | KEY_BACK ) ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR ){
		if( pl_w->pos.y < pl_w->ground_y + DOT_X( 64.0f ) ) return 0 ;
		if( ( em = pl_catch_ck( pl_w ) ) ) pl_w->catch_no = 1 ;
	} else if( pl_w->pl_stance == STANCE_STAND ){
		if( ( em = pl_catch_ck( pl_w ) ) ) pl_w->catch_no = 0 ;
	}
	return em ;
}

static void nml_catch_set( PLAYER_WORK *pl_w );
static void jump_catch_set( PLAYER_WORK *pl_w );
static void dummy_catch_set( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Start the throw that pl2b_catch_ck chose			*/
/****************************************************************/
void pl2b_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_catch_set, jump_catch_set, dummy_catch_set,
	} ;

	scr_excep_set( pl_w, 5, 5 ) ;
	catch_gauge_add( pl_w ) ;
	catch_set_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_catch_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_w->atck_lever & KEY_FORWARD ){
		pl_w->disp_dir ^= 1 ;
		pl_w->pl_dir ^= 1 ;
	}
	ofs.x = -DOT_X( 64.0f ) ;
	ofs.y = DOT_Y( 110.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_char_set( pl_w, Char_Catch, 0 ) ;
}

/*==============================================================*/
static void jump_catch_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	ofs.x = -80.0f ;
	ofs.y = DOT_Y( 128.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_char_set( pl_w, Char_Catch, 2 ) ;
}

/*==============================================================*/
static void dummy_catch_set( PLAYER_WORK *pl_w )
{
}

static void nml_catch( PLAYER_WORK *pl_w );
static void jump_catch( PLAYER_WORK *pl_w );
static void dummy_catch( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void pl2b_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_catch, jump_catch, dummy_catch,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

static void nml_catch_init( PLAYER_WORK *pl_w );
static void nml_catch_grab( PLAYER_WORK *pl_w );
static void nml_catch_throw( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The ground throw: a grab, then a slam				*/
/*==============================================================*/
static void nml_catch( PLAYER_WORK *pl_w )
{
	static void ( *nml_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_catch_init, nml_catch_grab, nml_catch_throw,
	} ;

	nml_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void nml_catch_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->timer0 = 0 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	nml_catch_grab( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void nml_catch_grab( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pls2b_05_set( pl_w, 0 ) ;
		pl_char_set( pl_w, Char_Catch, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void nml_catch_throw( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_prm & 2 ){
		pl_w->cg_prm ^= 2 ;
		em = pl_w->catch_w ;
		ofs.x = em->pos.x ;
		ofs.y = em->ground_y ;
		land_dust_set( &ofs, em->pl_no ) ;
		catch_se_req( pl_w ) ;
		scr_work.quake_level = 2 ;
		scr_work.quake_flag = 1 ;
		em->dm_hit_w = pl_w ;
		em->atck_no = pl2b_at_throw ;
		catch_damage( pl_w ) ;
	}
	if( pl_w->cg_prm & 1 ){
		pl_w->cg_prm ^= 1 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->atck_no = pl2b_at_throw_2 ;
		em->disp_dir = pl_w->disp_dir ^ 1 ;
		em->pl_dir = em->disp_dir ;
		scr_excep_clr() ;
	}
}

static void jump_catch_init( PLAYER_WORK *pl_w );
static void jump_catch_move( PLAYER_WORK *pl_w );
static void jump_catch_end( PLAYER_WORK *pl_w );
static void jump_catch_hold( PLAYER_WORK *pl_w );
static void jump_catch_fall( PLAYER_WORK *pl_w );
static void jump_catch_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The air throw: down to the ground together, a		*/
/*			slam, then a bounce back							*/
/*==============================================================*/
static void jump_catch( PLAYER_WORK *pl_w )
{
	static void ( *jump_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		jump_catch_init, jump_catch_move, jump_catch_end,
	} ;

	jump_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void jump_catch_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pls2b_05_set( pl_w, 1 ) ;
	jump_catch_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void jump_catch_move( PLAYER_WORK *pl_w )
{
	static void ( *jump_catch_move_jmp[])( PLAYER_WORK *pl_w ) = {
		jump_catch_hold, jump_catch_fall, jump_catch_land,
	} ;

	jump_catch_move_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void jump_catch_hold( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = -DOT_Y( 4.5f ) ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = -DOT_Y( 0.5f ) ;
		pl_char_set( pl_w, Char_Catch, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void jump_catch_fall( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	pl_char_move( pl_w ) ;
	if( !pl_w->cg_prm ){
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
	if( pl_w->pos.y < pl_w->ground_y ){
		em = pl_w->catch_w ;
		ofs.x = em->pos.x ;
		ofs.y = em->ground_y ;
		land_dust_set( &ofs, em->pl_no ) ;
		catch_se_req( pl_w ) ;
		scr_work.quake_level = 2 ;
		scr_work.quake_flag = 1 ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 2 ;
		em->atck_no = pl2b_at_air_throw ;
		em->disp_dir = pl_w->disp_dir ;
		em->pl_dir = pl_w->pl_dir ;
		scr_excep_clr() ;
		pl_w->routine_3 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.x = DOT_X( 2.5f ) ;
		pl_w->spd.y = DOT_Y( 6.0f ) ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
		pl_char_set( pl_w, Char_Catch, 4 ) ;
	}
}

/*--------------------------------------------------------------*/
static void jump_catch_land( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_prm ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->pos.y < pl_w->ground_y ){
			pl_w->routine_2 ++ ;
			pl_w->routine_3 = 0 ;
			pl_w->pl_stance = STANCE_STAND ;
			pl_w->pos.y = pl_w->ground_y ;
			pl_char_set( pl_w, Char_Jump, 3 ) ;
			pl_land_ef_set( pl_w ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void jump_catch_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void dummy_catch( PLAYER_WORK *pl_w )
{
}

static void hold( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void fang_hold( PLAYER_WORK *pl_w, PLAYER_WORK *em );

/****************************************************************/
/*			Holding the opponent								*/
/****************************************************************/
void pl2b_hold( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->catch_w ;

	static void ( *hold_jmp[])( PLAYER_WORK *pl_w, PLAYER_WORK *em ) = {
		hold, hold, fang_hold,
	} ;

	hold_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w, em ) ;
}

/*==============================================================*/
static void hold( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	pl_w->wall_ng_timer = 2 ;
	catch_pos_set( em, pl_w ) ;
}

/*==============================================================*/
static void fang_hold( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	catch_pos_set( em, pl_w ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl2b.c" End of File	*/

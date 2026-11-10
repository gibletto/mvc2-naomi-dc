/* Shuma-Gorath (character 2d): command checks, normal attacks, dashes, appearance, win poses,
   special moves and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl2d_def.h"

void pl2d_main( PLAYER_WORK *pl_w );
void pl2d_com_ck( PLAYER_WORK *pl_w );
void pl2d_atck_set( PLAYER_WORK *pl_w );
void pl2d_atck( PLAYER_WORK *pl_w );
void pl2d_dash( PLAYER_WORK *pl_w );
void pl2d_noki( PLAYER_WORK *pl_w );
void pl2d_appear( PLAYER_WORK *pl_w );
void pl2d_win( PLAYER_WORK *pl_w );
void pl2d_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl2d_catch_ck( PLAYER_WORK *pl_w );
void pl2d_catch_set( PLAYER_WORK *pl_w );
void pl2d_catch( PLAYER_WORK *pl_w );
void pl2d_hold( PLAYER_WORK *pl_w );
int pl2d_delay_ck( PLAYER_WORK *pl_w );
void pl2d_t_kick( PLAYER_WORK *pl_w );
void pl2d_upper( PLAYER_WORK *pl_w );
void pl2d_sp_unison( PLAYER_WORK *pl_w );
void pl2d_sp_gattai( PLAYER_WORK *pl_w );
void pl2d_sp_assist( PLAYER_WORK *pl_w );
void pl2d_sp_zero( PLAYER_WORK *pl_w );

/* each attack's table */
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
static Uint16 com_stare[] = { 40, COM_GROUND, pl2d_sp_stare, 0, KEY_BACK, KEY_FORWARD, KEY_PUNCH } ;	/* Mystic Stare */
static Uint16 com_smash[] = { 40, 0, pl2d_sp_smash, 0, KEY_BACK, KEY_FORWARD, KEY_KICK } ;	/* Mystic Smash */
static Uint16 com_devital[] = { 5, COM_GROUND, pl2d_sp_devital, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Devitalization */
static Uint16 com_chaos[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl2d_sp_chaos, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Chaos Dimension */
static Uint16 com_hsmash[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl2d_sp_hsmash, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Hyper Mystic Smash */

/* the win pose for each of 16 random numbers */
static Sint8 win_tbl[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1 } ;

static Sint32 smash_spd_0[12] = {
	0xfff98000, 0x00000000, 0x0007e000, 0xffff7000,
	0xfff90000, 0x00000000, 0x000d0000, 0xffff7000,
	0xfff98000, 0x00000000, 0x000b0000, 0xffff7000,
} ;
static Sint32 smash_spd_1[12] = {
	0xfffb8000, 0x00000000, 0x00000000, 0xffff9000,
	0xfff90000, 0x00000000, 0x00040000, 0xffff9000,
	0xfffa0000, 0x00000000, 0x00020000, 0xffff9000,
} ;
static Sint32 *smash_spd_tbl[2] = { smash_spd_0, smash_spd_1 } ;
static Sint16 chaos_ofs_tbl[12] = { -72, 48, -88, 40, -96, 80, -48, 48, -64, 0, -80, 96 } ;

static void ( *pl2d_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl2d_main,
	pl2d_com_ck,
	pl2d_atck_set,
	pl2d_atck,
	pl2d_dash,
	pl2d_noki,
	pl2d_appear,
	pl2d_win,
	pl2d_sp_atck,
	( void ( * )( PLAYER_WORK *pl_w ) )pl2d_catch_ck,	/* returns a value */
	pl2d_catch_set,
	pl2d_catch,
	pl2d_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl2d_delay_ck,
	pl2d_t_kick,
	pl2d_upper,
	pl2d_sp_unison,
	pl2d_sp_gattai,
	pl2d_sp_assist,
	pl2d_sp_zero,
} ;

static Uint8 stare_ck( PLAYER_WORK *pl_w );
static Uint8 smash_ck( PLAYER_WORK *pl_w );
static Uint8 devital_ck( PLAYER_WORK *pl_w );
static Uint8 chaos_ck( PLAYER_WORK *pl_w );
static Uint8 hsmash_ck( PLAYER_WORK *pl_w );
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w );
static Uint8 snapback_ck( PLAYER_WORK *pl_w );
static int hsmash_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl2d_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl2d_tbl[i] ;
}

/****************************************************************/
/*			Commands: only the hyper combos and throws while	*/
/*			powered up											*/
/****************************************************************/
void pl2d_com_ck( PLAYER_WORK *pl_w )
{
	PL2D_LOCAL_W *lw = PL2D_LW( pl_w ) ;

	if( pl_w->char_no.w == 0x0201 ) return ;
	if( lw->dive_flag ) return ;
	if( !lw->power_step && pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( !lw->power_step ){
		if( chaos_ck( pl_w ) ) return ;
		if( hsmash_ck( pl_w ) ) return ;
		if( stare_ck( pl_w ) ) return ;
		if( smash_ck( pl_w ) ) return ;
		if( devital_ck( pl_w ) ) return ;
		if( chouhatsu_ck( pl_w ) ) return ;
		if( snapback_ck( pl_w ) ) return ;
	}
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
static Uint8 stare_ck( PLAYER_WORK *pl_w )
{
	PL2D_LOCAL_W *lw = PL2D_LW( pl_w ) ;

	if( !com_tame_ck( pl_w, com_stare, &pl_w->com_w0 ) ) return 0 ;
	if( lw->orb_flag ) return 0 ;
	if( lw->power_step ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl2d_sp_stare ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 smash_ck( PLAYER_WORK *pl_w )
{
	PL2D_LOCAL_W *lw = PL2D_LW( pl_w ) ;

	if( !com_tame_ck( pl_w, com_smash, &pl_w->com_w1 ) ) return 0 ;
	if( pl_w->jump_com_cnt || lw->power_step ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl2d_sp_smash ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 devital_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_devital, &pl_w->com_w2 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl2d_sp_devital ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			A hyper combo of three levels						*/
/*==============================================================*/
static Uint8 chaos_ck( PLAYER_WORK *pl_w )
{
	PL2D_LOCAL_W *lw = PL2D_LW( pl_w ) ;

	if( !com_nml_ck( pl_w, com_chaos, &pl_w->com_w3 ) ) return 0 ;
	if( *pl_w->gauge_level < 3 ) return 0 ;
	if( lw->power_step ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w4 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl2d_sp_chaos ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 hsmash_ck( PLAYER_WORK *pl_w )
{
	PL2D_LOCAL_W *lw = PL2D_LW( pl_w ) ;

	if( !com_nml_ck( pl_w, com_hsmash, &pl_w->com_w4 ) ) return 0 ;
	if( *pl_w->gauge_level < 1 ) return 0 ;
	if( lw->power_step ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w4 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl2d_sp_hsmash ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w )
{
	PL2D_LOCAL_W *lw = PL2D_LW( pl_w ) ;

	if( !pl_chouhatsu_ck( pl_w, 6 ) ) return 0 ;
	if( lw->power_step ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl2d_sp_chouhatsu ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 snapback_ck( PLAYER_WORK *pl_w )
{
	PL2D_LOCAL_W *lw = PL2D_LW( pl_w ) ;

	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( lw->power_step ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl2d_sp_snapback ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl2d_delay_ck( PLAYER_WORK *pl_w )
{
	if( hsmash_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static int hsmash_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_hsmash, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl2d_sp_hsmash ;
	return 1 ;
}

static void flag_clr( PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw );
static void power_color( PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw );
static void catch_color( PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw );
static void dive_color( PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw );
static void punch_color( PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw );

/****************************************************************/
void pl2d_main( PLAYER_WORK *pl_w )
{
	PL2D_LOCAL_W *lw = PL2D_LW( pl_w ) ;

	flag_clr( pl_w, lw ) ;
	power_color( pl_w, lw ) ;
	catch_color( pl_w, lw ) ;
	dive_color( pl_w, lw ) ;
	punch_color( pl_w, lw ) ;
}

/*==============================================================*/
static void flag_clr( PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw )
{
	if( !lw->orb_fire_flag ) lw->orb_flag = 0 ;
}

/*==============================================================*/
/*			The power-up's colour cycle, until it runs out		*/
/*			or he is knocked down								*/
/*==============================================================*/
static void power_color( PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw )
{
	if( !lw->power_step ) return ;
	if( pl_w->act_st == N_CHANGE || pl_w->act_st == N_WIN || !pl_w->vital_new ) lw->power_timer = 1 ;
	if( !-- lw->power_timer ){
		lw->power_step = 0 ;
		lw->col_timer = 0 ;
		lw->col_cnt = 0 ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
		return ;
	}
	if( !-- lw->col_cnt ){
		lw->col_cnt = 8 ;
		ef_color_set( pl_w, 0, pl_w->pl_color * 48 + lw->col_no + 37, 2 ) ;
		lw->col_no += lw->power_step ;
		if( !lw->col_no || lw->col_no == 4 ) lw->power_step = -lw->power_step ;
	}
}

/*==============================================================*/
static void catch_color( PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw )
{
	if( pl_w->char_no.b.grp == Char_Catch && pl_w->char_no.b.no == 2 ){
		if( pl_w->timer0 ){
			ef_color_set( pl_w, 0, pl_w->pl_color * 48 + lw->col_cnt + 20, 1 ) ;
			if( !-- lw->col_timer ){
				lw->col_timer = 9 ;
				lw->col_no = -lw->col_no ;
			}
			lw->col_cnt += lw->col_no ;
		} else pl_color_set( pl_w, PC_DEFAULT ) ;
	}
}

/*==============================================================*/
static void dive_color( PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw )
{
	if( !lw->dive_flag ) return ;
	lw->dive_flag = 0 ;
	if( pl_w->char_no.b.grp == Char_Jump_K && pl_w->char_no.b.no == 5 ){
		lw->dive_flag = 1 ;
		ef_color_set( pl_w, 0, pl_w->pl_color * 48 + pl_w->cg_type + 5, 1 ) ;
	} else {
		pl_color_set( pl_w, PC_DEFAULT ) ;
	}
}

/*==============================================================*/
static void punch_color( PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw )
{
	if( ( pl_w->char_no.b.grp == Char_Stand_P || pl_w->char_no.b.grp == Char_Crouch_P || pl_w->char_no.b.grp == Char_Jump_P ) && pl_w->atck_level )
		ef_color_set( pl_w, 0, pl_w->pl_color * 48 + 34, 1 ) ;
}

static void nml_atck( PLAYER_WORK *pl_w );
static void jump_atck_ck( PLAYER_WORK *pl_w );
static void jump_atck( PLAYER_WORK *pl_w );
static void stand_punch( PLAYER_WORK *pl_w );
static void crouch_punch( PLAYER_WORK *pl_w );
static void stand_kick( PLAYER_WORK *pl_w );
static void crouch_kick( PLAYER_WORK *pl_w );
static void jump_punch( PLAYER_WORK *pl_w );
static void jump_kick( PLAYER_WORK *pl_w );
static void power_atck( PLAYER_WORK *pl_w );
static void power_jump_atck( PLAYER_WORK *pl_w );
static void power_move( PLAYER_WORK *pl_w );
static void power_jump_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl2d_atck_set( PLAYER_WORK *pl_w )
{
	static void ( *atck_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_atck, jump_atck_ck, jump_atck, nml_atck,
	} ;

	atck_set_jmp[pl_w->atck_type]( pl_w ) ;
}

/*==============================================================*/
static void nml_atck( PLAYER_WORK *pl_w )
{
	PL2D_LOCAL_W *lw = PL2D_LW( pl_w ) ;

	pl_brake_set( pl_w ) ;
	if( lw->power_step && pl_w->nml_no == 2 ){
		power_atck( pl_w ) ;
		return ;
	}
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
	Uint8 no, atck ;

	switch( pl_w->nml_no ){
	case 0:
		atck = 0 ;
		no = 0 ;
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		break ;
	case 1:
		atck = 1 ;
		no = 1 ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		break ;
	case 2:
		atck = 2 ;
		no = 2 ;
		pl_w->chain_tbl = hp_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	pl_w->atck_no = atck ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_char_set( pl_w, Char_Stand_P, no ) ;
}

/*==============================================================*/
static void crouch_punch( PLAYER_WORK *pl_w )
{
	Uint8 no, atck ;

	switch( pl_w->nml_no ){
	case 0:
		no = 0 ;
		atck = 6 ;
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		break ;
	case 1:
		no = 1 ;
		atck = 7 ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		break ;
	case 2:
		no = 2 ;
		atck = 8 ;
		pl_w->chain_tbl = hp_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	pl_w->atck_no = atck ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_char_set( pl_w, Char_Crouch_P, no ) ;
}

/*==============================================================*/
static void stand_kick( PLAYER_WORK *pl_w )
{
	Uint8 no, atck ;

	switch( pl_w->nml_no ){
	case 0:
		no = 0 ;
		atck = 3 ;
		pl_w->chain_tbl = lk_tbl ;
		pl_w->nml_level = 0 ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		break ;
	case 1:
		no = 1 ;
		atck = 4 ;
		pl_w->chain_tbl = mk_tbl ;
		pl_w->nml_level = 1 ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		break ;
	case 2:
		no = 2 ;
		atck = 5 ;
		pl_w->chain_tbl = hk_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	pl_w->atck_no = atck ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_char_set( pl_w, Char_Stand_K, no ) ;
}

/*==============================================================*/
static void crouch_kick( PLAYER_WORK *pl_w )
{
	Uint8 no, atck ;
	int voice ;

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
		no = 2 ;
		voice = 22 ;
		atck = 11 ;
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
	PL2D_LOCAL_W *lw = PL2D_LW( pl_w ) ;

	if( lw->power_step && pl_w->nml_no == 2 ){
		power_jump_atck( pl_w ) ;
		return ;
	}
	if( !pl_w->atck_pk ) jump_punch( pl_w ) ;
	else jump_kick( pl_w ) ;
}

/*==============================================================*/
static void jump_punch( PLAYER_WORK *pl_w )
{
	Uint8 no, atck ;

	switch( pl_w->nml_no ){
	case 0:
		atck = 12 ;
		no = 0 ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = lp_tbl ;
		else pl_w->chain_tbl = sj_lp_tbl ;
		pl_w->nml_level = 0 ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		break ;
	case 1:
		atck = 13 ;
		no = 1 ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = mp_tbl ;
		else pl_w->chain_tbl = sj_mp_tbl ;
		pl_w->nml_level = 1 ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		break ;
	case 2:
		no = 5 ;
		atck = 14 ;
		if( pl_w->atck_lever & KEY_UP ){
			no = 2 ;
			atck = 19 ;
		}
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = hp_tbl ;
		else pl_w->chain_tbl = sj_hp_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	pl_w->atck_no = atck ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_char_set( pl_w, Char_Jump_P, no ) ;
	if( pl_w->jump_atk_cnt & JATK_P_MASK ) pl_w->jump_atk_cnt -= 0x01 ;
}

/*==============================================================*/
/*			The heavy kick with the stick down, high enough,	*/
/*			dives												*/
/*==============================================================*/
static void jump_kick( PLAYER_WORK *pl_w )
{
	PL2D_LOCAL_W *lw = PL2D_LW( pl_w ) ;
	int voice ;
	Uint8 no, atck ;

	switch( pl_w->nml_no ){
	case 0:
		atck = 15 ;
		no = 0 ;
		voice = 20 ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = lk_tbl ;
		else pl_w->chain_tbl = sj_lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		atck = 16 ;
		no = 1 ;
		voice = 21 ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = mk_tbl ;
		else pl_w->chain_tbl = sj_mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		atck = 17 ;
		no = 2 ;
		voice = 22 ;
		if( ( pl_w->atck_lever & KEY_DOWN ) && pl_w->pos.y > pl_w->ground_y + DOT_Y( 88.0f ) ){
			atck = 18 ;
			no = 5 ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_w->routine_2 = 0 ;
			pl_w->sjump_flag = 0 ;
			pl_w->jump_atk_cnt = 0 ;
			pl_w->jump_com_cnt = 0 ;
			lw->dive_flag = 1 ;
		}
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = hk_tbl ;
		else pl_w->chain_tbl = sj_hk_tbl ;
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

static void nml_move_dash( PLAYER_WORK *pl_w );
static void nml_move( PLAYER_WORK *pl_w );
static void sjump_move( PLAYER_WORK *pl_w );
static void jump_move( PLAYER_WORK *pl_w );
static void stand_punch_move( PLAYER_WORK *pl_w );
static void crouch_punch_move( PLAYER_WORK *pl_w );
static void stand_kick_move( PLAYER_WORK *pl_w );
static void crouch_kick_move( PLAYER_WORK *pl_w );
static void jump_punch_move( PLAYER_WORK *pl_w );
static void jump_kick_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl2d_atck( PLAYER_WORK *pl_w )
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
	PL2D_LOCAL_W *lw = PL2D_LW( pl_w ) ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_brake_stop( pl_w ) ;
	if( lw->power_step && pl_w->atck_level ){
		power_move( pl_w ) ;
		return ;
	}
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
/*			The medium punch hits again							*/
/*==============================================================*/
static void crouch_punch_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	else switch( pl_w->nml_no ){
	case 0:
	case 2:
		break ;
	case 1:
		if( pl_w->cg_type ){
			pl_w->atck_no = pl2d_at_crouch_mp_2 ;
			pl_w->hit_flag = pl_w->atck_attr = 0 ;
			pl_w->unk_1c4 = 0 ;
			vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
			pl_w->cg_type = 0 ;
		}
		break ;
	}
}

/*==============================================================*/
/*			The heavy kick hits again							*/
/*==============================================================*/
static void stand_kick_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	else switch( pl_w->nml_no ){
	case 0:
	case 1:
		break ;
	case 2:
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pl_w->atck_no = ATCK_STAND_HK ;
			pl_w->hit_flag = pl_w->atck_attr = 0 ;
			pl_w->unk_1c4 = 0 ;
			vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
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
	PL2D_LOCAL_W *lw = PL2D_LW( pl_w ) ;

	pl_jump_move( pl_w ) ;
	pl_sjump_top_ck( pl_w ) ;
	if( lw->power_step && pl_w->atck_level ){
		power_jump_move( pl_w ) ;
		return ;
	}
	if( !pl_w->atck_pk ) jump_punch_move( pl_w ) ;
	else jump_kick_move( pl_w ) ;
}

/*==============================================================*/
static void jump_punch_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
	else if( pl_land_ck( pl_w ) ) pl_land_set( pl_w ) ;
}

static void dive_init( PLAYER_WORK *pl_w );
static void dive_fall( PLAYER_WORK *pl_w );
static void dive_bound( PLAYER_WORK *pl_w );
static void dive_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void jump_kick_move( PLAYER_WORK *pl_w )
{
	static void ( *dive_jmp[])( PLAYER_WORK *pl_w ) = {
		dive_init, dive_fall, dive_bound, dive_end,
	} ;
	PL2D_LOCAL_W *lw = PL2D_LW( pl_w ) ;

	switch( pl_w->nml_no ){
	case 0:
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
		else if( pl_land_ck( pl_w ) ) pl_land_set( pl_w ) ;
		break ;
	case 2:
		if( lw->dive_flag ) dive_jmp[pl_w->routine_2]( pl_w ) ;
		else {
			if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
			else if( pl_land_ck( pl_w ) ) pl_land_set( pl_w ) ;
		}
		break ;
	}
}

/*==============================================================*/
static void dive_init( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_flag1 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = -DOT_Y( 8.0f ) ;
		pl_w->acc.y = -2.142857f ;
	}
}

/*--------------------------------------------------------------*/
static void dive_fall( PLAYER_WORK *pl_w )
{
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y + -120.0f ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.y = -( pl_w->spd.y / 8.0f ) ;
		pl_char_move( pl_w ) ;
		pl_land_ef_set( pl_w ) ;
		pl_se_req( pl_w, 48 ) ;
	}
}

/*--------------------------------------------------------------*/
static void dive_bound( PLAYER_WORK *pl_w )
{
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
}

/*--------------------------------------------------------------*/
static void dive_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 = 0 ;
		pl_stand_ret( pl_w ) ;
	}
}

/****************************************************************/
/*			The launcher										*/
/****************************************************************/
void pl2d_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->atck_no = pl2d_at_upper ;
		pl_w->hit_flag = pl_w->atck_attr = 0 ;
		pl_w->unk_1c4 = 0 ;
		vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
		pl_brake_set( pl_w ) ;
		pl_gauge_add( pl_w, 5 ) ;
		pl_char_set( pl_w, Char_Extra, 2 ) ;
		pl_se_req( pl_w, SE_SWING_M ) ;
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
void pl2d_dash( PLAYER_WORK *pl_w )
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
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 30 ;
		pl_w->spd.x = -10.0f ;
		pl_w->acc.x = 0.0f ;
		if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
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
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 ++ ;
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

/****************************************************************/
/*			The back dash: he vanishes and reappears further	*/
/*			back												*/
/****************************************************************/
void pl2d_noki( PLAYER_WORK *pl_w )
{
	static void ( *noki_jmp[])( PLAYER_WORK *pl_w ) = {
		noki_init, noki_move,
	} ;

	noki_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void noki_init( PLAYER_WORK *pl_w )
{
	PL2D_LOCAL_W *lw = PL2D_LW( pl_w ) ;
	Float ofs ;

	pl_w->routine_2 ++ ;
	lw->noki_flag = 0 ;
	pl_voice_req( pl_w, 30 ) ;
	pls2d_00_set( pl_w, 0 ) ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	ofs = 160.0f ;
	pl_w->spd.x = DOT_X( 4.125f ) ;
	if( pl_w->pl_dir ){
		ofs = -160.0f ;
		pl_w->spd.x = -pl_w->spd.x ;
	}
	pl_w->pos.x += ofs ;
	pl_w->acc.x = 0.0f ;
}

/*--------------------------------------------------------------*/
static void noki_move( PLAYER_WORK *pl_w )
{
	PL2D_LOCAL_W *lw = PL2D_LW( pl_w ) ;

	if( !lw->noki_flag ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
	} else if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void appear_wait( PLAYER_WORK *pl_w );
static void appear_move( PLAYER_WORK *pl_w );
static void appear_end( PLAYER_WORK *pl_w );

/****************************************************************/
void pl2d_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_wait, appear_move, appear_end,
	} ;

	appear_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void appear_wait( PLAYER_WORK *pl_w )
{
	pl_w->disp_flag = 0 ;
	if( vs_sys_work.vs_st == VS_APPEAR ){
		pl_w->routine_2 ++ ;
		pl_w->disp_flag = 1 ;
		pl_char_set( pl_w, Char_Appear, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Jump, 3 ) ;
		pl_land_ef_set( pl_w ) ;
		pls2d_00_set( pl_w, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_1 ++ ;
		pl_char_set( pl_w, Char_Stand, 0 ) ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
	}
}

static void win_init( PLAYER_WORK *pl_w );
static void win_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl2d_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_init, win_move,
	} ;

	win_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void win_init( PLAYER_WORK *pl_w )
{
	PL2D_LOCAL_W *lw = PL2D_LW( pl_w ) ;
	Uint32 rnd ;

	pl_w->routine_2 ++ ;
	switch( pl_w->type ){
	case 0:
	case 2:
		rnd = Rnd() & 15 ;
		lw->win_no = win_tbl[rnd] ;
		pl_char_set( pl_w, Char_Win, lw->win_no ) ;
		pl_voice_req( pl_w, 10 ) ;
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
	PL2D_LOCAL_W *lw = PL2D_LW( pl_w ) ;
	int col ;

	if( pl_w->act_st == N_WIN && pl_win_end_ck( pl_w ) ){
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_stand_ret( pl_w ) ;
		return ;
	}
	switch( pl_w->type ){
	case 0:
		if( !lw->win_no ){
			pl_char_move( pl_w ) ;
			break ;
		}
		pl_char_move( pl_w ) ;
		if( pl_w->cg_type ){
			col = pl_w->pl_color * 48 + 35 ;
			if( pl_w->cg_type & 1 ) col ++ ;
			ef_color_set( pl_w, 0, col, 1 ) ;
		}
		break ;
	case 2:
		pl_char_move( pl_w ) ;
		break ;
	case 1:
	case 3:
	case 4:
		pl_char_move( pl_w ) ;
		break ;
	}
}

static void sp_stare( PLAYER_WORK *pl_w );
static void sp_smash( PLAYER_WORK *pl_w );
static void sp_devital( PLAYER_WORK *pl_w );
static void sp_chaos( PLAYER_WORK *pl_w );
static void sp_hsmash( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );

/****************************************************************/
void pl2d_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *sp_atck_jmp[])( PLAYER_WORK *pl_w ) = {
		sp_stare, sp_smash, sp_devital, sp_chaos, sp_stare, sp_hsmash, sp_chouhatsu, sp_snapback,
	} ;

	sp_atck_jmp[pl_w->pl_sp_type]( pl_w ) ;
}

static void stare_init( PLAYER_WORK *pl_w );
static void stare_move( PLAYER_WORK *pl_w );
static void stare_end( PLAYER_WORK *pl_w );
static void stare_charge( PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw );
static void stare_wait( PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw );

/*==============================================================*/
static void sp_stare( PLAYER_WORK *pl_w )
{
	static void ( *stare_jmp[])( PLAYER_WORK *pl_w ) = {
		stare_init, stare_move, stare_end,
	} ;

	stare_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void stare_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->atck_no = pl2d_at_stare ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level ) ;
}

/*--------------------------------------------------------------*/
static void stare_move( PLAYER_WORK *pl_w )
{
	static void ( *stare_move_jmp[])( PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw ) = {
		stare_charge, stare_wait,
	} ;

	stare_move_jmp[pl_w->routine_3]( pl_w, PL2D_LW( pl_w ) ) ;
}

/*==============================================================*/
static void stare_charge( PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		if( work_rest_cnt > 6 ){
			pl_w->routine_3 ++ ;
			lw->orb_fire_flag = 0 ;
			lw->orb_cnt = 6 ;
			shl2d_00_set( pl_w ) ;
		} else {
			pl_w->routine_2 = 2 ;
		}
	}
}

/*--------------------------------------------------------------*/
static void stare_wait( PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw )
{
	if( lw->orb_fire_flag >= 0 ){
		if( lw->orb_fire_flag != 63 ) return ;
		lw->orb_flag = 1 ;
		pl_gauge_add( pl_w, 5 ) ;
		pl_voice_req( pl_w, 32 ) ;
	}
	pl_w->routine_2 ++ ;
	pl_w->routine_3 = 0 ;
}

/*--------------------------------------------------------------*/
static void stare_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void smash_init( PLAYER_WORK *pl_w );
static void smash_move( PLAYER_WORK *pl_w );
static void smash_end( PLAYER_WORK *pl_w );
static void smash_start( PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw );
static void smash_fly( PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw );
static void smash_fall( PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw );
static void smash_homing( PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw );

/*==============================================================*/
static void sp_smash( PLAYER_WORK *pl_w )
{
	static void ( *smash_jmp[])( PLAYER_WORK *pl_w ) = {
		smash_init, smash_move, smash_end,
	} ;

	smash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void smash_init( PLAYER_WORK *pl_w )
{
	PL2D_LOCAL_W *lw = PL2D_LW( pl_w ) ;

	pl_w->routine_2 ++ ;
	pl_w->push_ng_timer = 2 ;
	lw->unk_06 = 0 ;
	lw->smash_type = ( pl_w->pl_stance & 2 ) >> 1 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl_w->atck_level + 50 ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	if( pl_w->pl_stance == STANCE_AIR ){
		pl_w->pos.y += -DOT_Y( 40.0f ) ;
	} else {
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_sp_ef_set( pl_w ) ;
	}
	pl_dir_set( pl_w ) ;
	dm00_09_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, lw->smash_type + 3 ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	ef_color_set( pl_w, 1, pl_w->pl_color * 48 + 47, 1 ) ;
}

/*--------------------------------------------------------------*/
static void smash_move( PLAYER_WORK *pl_w )
{
	static void ( *smash_move_jmp[])( PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw ) = {
		smash_start, smash_fly, smash_fall,
	} ;

	pl_w->push_ng_timer = 2 ;
	smash_move_jmp[pl_w->routine_3]( pl_w, PL2D_LW( pl_w ) ) ;
}

/*==============================================================*/
static void smash_start( PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw )
{
	Sint32 *spd ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_voice_req( pl_w, 21 ) ;
		if( pl_w->pl_stance == STANCE_AIR ) pl_w->pos.y += DOT_Y( 40.0f ) ;
		shl2d_02_set( pl_w ) ;
		pl_gauge_add( pl_w, 5 ) ;
		spd = smash_spd_tbl[lw->smash_type] ;
		spd += pl_w->atck_level * 4 ;
		pl_w->spd.x = *spd++ * 1.6666666f / 65536.0f ;
		pl_w->acc.x = *spd++ * 1.6666666f / 65536.0f ;
		pl_w->spd.y = *spd++ * 2.142857f / 65536.0f ;
		pl_w->acc.y = *spd * 2.142857f / 65536.0f ;
		if( pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		pl_w->atck_no = pl_w->atck_level + 50 ;
		pl_w->hit_flag = pl_w->atck_attr = 0 ;
		pl_w->unk_1c4 = 0 ;
		vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
		pl_w->atck_attr |= ATR_BACK_POS ;
		pl_w->routine_3 ++ ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_char_set( pl_w, Char_Special, 6 ) ;
	}
}

/*--------------------------------------------------------------*/
static void smash_fly( PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw )
{
	if( pl_w->hit_flag ){
		if( pl_w->hit_flag & HR_GUARD ){
			pl_w->routine_3 ++ ;
			pl_w->spd.x = -pl_w->spd.x / 4.0f ;
			pl_w->spd.y = DOT_Y( 6.0f ) ;
			pl_w->acc.y = -1.2053571f ;
			pl_char_set( pl_w, Char_Special, 7 ) ;
			return ;
		}
	}
	if( pl_w->pl_sp_type == pl2d_sp_smash ) smash_homing( pl_w, lw ) ;
	if( !pl_w->cg_type ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->pos.y < pl_w->ground_y ){
			pl_w->routine_2 ++ ;
			pl_w->routine_3 = 0 ;
			pl_w->pl_stance = STANCE_STAND ;
			pl_w->pos.y = pl_w->ground_y ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_land_ef_set( pl_w ) ;
			pl_char_set( pl_w, Char_Special, 11 ) ;
			return ;
		}
		if( pl_w->wall_flag && !( pl_w->wall_flag & ( 1 << pl_w->pl_dir ) ) ){
			pl_w->routine_3 ++ ;
			pl_w->spd.x = -pl_w->spd.x / 4.0f ;
			pl_w->spd.y = DOT_Y( 6.0f ) ;
			pl_w->acc.y = -1.2053571f ;
			pl_char_set( pl_w, Char_Special, 7 ) ;
			return ;
		}
		pl_char_move( pl_w ) ;
	} else if( pl_char_move( pl_w ) < 0 ){
		lw->smash_type = 0 ;
		pl_char_set( pl_w, Char_Special, 9 ) ;
	}
}

/*--------------------------------------------------------------*/
static void smash_fall( PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw )
{
	if( !pl_w->cg_type ){
		pl_char_move( pl_w ) ;
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	} else if( pl_char_move( pl_w ) < 0 ){
		lw->smash_type = 0 ;
		pl_char_set( pl_w, Char_Special, 9 ) ;
	}
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->pos.y = pl_w->ground_y ;
		lw->smash_type = -1 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 11 ) ;
	}
}

/*--------------------------------------------------------------*/
static void smash_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
/*			Turn to home in on the nearest of the other			*/
/*			side's three characters								*/
/*==============================================================*/
static void smash_homing( PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw )
{
	PLAYER_WORK *em ;
	int i ;
	Float r, dx, dy ;

	em = pl_work ;
	em += 1 >> ( pl_w->pl_no & 1 ) ;
	for( i = 0 ; i < 3 ; i++, em += 2 ){
		if( !em->be_flag ) continue ;
		r = ( em->size_top + em->size_bottom ) * 2.142857f ;
		dx = pl_w->pos.x - em->pos.x ;
		dy = pl_w->pos.y - em->pos.y + DOT_Y( em->size_bottom ) - r ;
		if( dx * dx + dy * dy < r * r ){
			if( !lw->smash_type ){
				lw->smash_type = 1 ;
				pl_char_set( pl_w, Char_Special, 5 ) ;
				return ;
			}
		} else if( lw->smash_type ){
			lw->smash_type = 0 ;
			pl_char_set( pl_w, Char_Special, 9 ) ;
		}
	}
}

static void devital_init( PLAYER_WORK *pl_w );
static void devital_move( PLAYER_WORK *pl_w );
static void devital_end( PLAYER_WORK *pl_w );
static void devital_catch( PLAYER_WORK *pl_w );
static void devital_hold( PLAYER_WORK *pl_w );
static void devital_rise( PLAYER_WORK *pl_w );
static void devital_throw( PLAYER_WORK *pl_w );
static void devital_fall( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_devital( PLAYER_WORK *pl_w )
{
	static void ( *devital_jmp[])( PLAYER_WORK *pl_w ) = {
		devital_init, devital_move, devital_end,
	} ;

	devital_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void devital_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 12 ) ;
}

/*--------------------------------------------------------------*/
static void devital_move( PLAYER_WORK *pl_w )
{
	static void ( *devital_move_jmp[])( PLAYER_WORK *pl_w ) = {
		devital_catch, devital_hold, devital_rise, devital_throw, devital_fall,
	} ;

	devital_move_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void devital_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 pos ;	/* never used, but it takes stack space */

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, 17 ) ;
	} else if( ( em = pl_catch_ck( pl_w ) ) ){
		pl_w->routine_3 ++ ;
		pl_w->sp_ng_timer = 3 ;
		pl_w->catch_flag = 1 ;
		pl_w->catch_no = 0xc3 ;
		scr_excep_set( pl_w, 5, 5 ) ;
		pl_sp_catch_set( pl_w, em ) ;
		em->ukemi_flag = 0 ;
		pl_char_set( pl_w, Char_Special, 13 ) ;
		nage_se_req( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void devital_hold( PLAYER_WORK *pl_w )
{
	PL2D_LOCAL_W *lw = PL2D_LW( pl_w ) ;

	pl_w->catch_flag = 1 ;
	pl_w->sp_ng_timer = 3 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->spd.y = DOT_Y( 13.0f ) ;
		pl_w->acc.y = -0.6696428f ;
		lw->rise_flag = 0 ;
		pls2d_00_set( pl_w, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void devital_rise( PLAYER_WORK *pl_w )
{
	PL2D_LOCAL_W *lw = PL2D_LW( pl_w ) ;

	pl_w->catch_flag = 1 ;
	pl_w->sp_ng_timer = 3 ;
	if( lw->rise_flag >= 0 ){
		pl_char_move( pl_w ) ;
		if( pl_w->spd.y >= 0.0f ){
			pl_w->pos.y += pl_w->spd.y ;
			pl_w->spd.y += pl_w->acc.y ;
		}
		if( lw->rise_flag > 0 ){
			pl_w->routine_3 ++ ;
			pl_char_set( pl_w, Char_Special, 14 ) ;
		}
	} else {
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->catch_flag = 0 ;
		pl_stand_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void devital_throw( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_w->catch_flag = 1 ;
	pl_w->sp_ng_timer = 3 ;
	if( pl_w->spd.y >= 0.0f ){
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->spd.y = -DOT_Y( 11.0f ) ;
		pl_w->acc.y = 0.0f ;
		pl_w->atck_no = pl2d_at_devital ;
		pl_w->hit_flag = pl_w->atck_attr = 0 ;
		pl_w->unk_1c4 = 0 ;
		vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
		pl_char_set( pl_w, Char_Special, 15 ) ;
	} else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->dir = 16 ;
		pl_w->catch_flag = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->pl_dir = pl_w->pl_dir ;
		em->atck_no = pl2d_at_devital_throw ;
		em->ukemi_flag = 0 ;
		SCR_EXCEP_SET ;
	}
}

/*--------------------------------------------------------------*/
static void devital_fall( PLAYER_WORK *pl_w )
{
	PL2D_LOCAL_W *lw = PL2D_LW( pl_w ) ;
	Float y ;

	if( pl_w->cg_type ){
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		y = pl_w->ground_y + DOT_Y( 36.0f ) ;
		if( pl_w->pos.y <= y ) pl_w->pos.y = y ;
		else return ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->pos.y = pl_w->ground_y ;
		lw->rise_flag = -1 ;
		pl_char_set( pl_w, Char_Special, 16 ) ;
	}
}

/*--------------------------------------------------------------*/
static void devital_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void chaos_init( PLAYER_WORK *pl_w );
static void chaos_move( PLAYER_WORK *pl_w );
static void chaos_end( PLAYER_WORK *pl_w );
static void chaos_catch( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_chaos( PLAYER_WORK *pl_w )
{
	static void ( *chaos_jmp[])( PLAYER_WORK *pl_w ) = {
		chaos_init, chaos_move, chaos_end, chaos_catch,
	} ;

	chaos_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void chaos_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 0 ) ;
}

/*--------------------------------------------------------------*/
static void chaos_move( PLAYER_WORK *pl_w )
{
	PL2D_LOCAL_W *lw = PL2D_LW( pl_w ) ;
	VEC3 ofs ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type & 1 ){
		pl_w->cg_type ^= 1 ;
		ofs.x = DOT_X( 40.0f ) ;
		ofs.y = DOT_Y( 62.0f ) ;
		pl_hyper_start( pl_w, &ofs, 3 ) ;
		pl_w->atck_no = pl2d_at_chaos ;
		pl_w->hit_flag = pl_w->atck_attr = 0 ;
		pl_w->unk_1c4 = 0 ;
		vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	}
	if( pl_w->cg_type & 2 ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type ^= 2 ;
		pls2d_00_set( pl_w, 5 ) ;
		lw->power_step = 2 ;
		lw->power_timer = 500 ;
		lw->release_flag = 0 ;
		lw->col_cnt = 8 ;
		lw->col_no = 0 ;
		Timer_Gauge_Set( pl_w, &lw->power_timer, 6 ) ;
	}
}

/*--------------------------------------------------------------*/
static void chaos_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void chaos_catch_start( PLAYER_WORK *pl_w );
static void chaos_catch_wait( PLAYER_WORK *pl_w );
static void chaos_catch_up( PLAYER_WORK *pl_w );
static void chaos_catch_burst( PLAYER_WORK *pl_w );
static void chaos_catch_drop( PLAYER_WORK *pl_w );
static void chaos_catch_appear( PLAYER_WORK *pl_w );
static void chaos_catch_fall( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The powered-up grab: carried off screen and			*/
/*			dropped back										*/
/*==============================================================*/
static void chaos_catch( PLAYER_WORK *pl_w )
{
	static void ( *chaos_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		chaos_catch_start, chaos_catch_wait, chaos_catch_up, chaos_catch_burst,
		chaos_catch_drop, chaos_catch_appear, chaos_catch_fall,
	} ;

	chaos_catch_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void chaos_catch_start( PLAYER_WORK *pl_w )
{
	Sint16 *ofs ;
	Float x ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->catch_flag = 1 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
	if( pl_w->cg_type & 1 ){
		pl_w->cg_type ^= 1 ;
		ofs = &chaos_ofs_tbl[pl_w->pl_stance * 2] ;
		x = *ofs++ * 1.6666666f ;
		if( pl_w->pl_dir ) x = -x ;
		pl_w->pos.x += x ;
		pl_w->pos.y += *ofs * 2.142857f ;
	}
}

/*--------------------------------------------------------------*/
static void chaos_catch_wait( PLAYER_WORK *pl_w )
{
	SCR_WORK *scr = &scr_work ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->catch_flag = 1 ;
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		scr_excep_set( pl_w, 5, 5 ) ;
		pl_w->spd.x = scr->scr_left + 320.0f ;
		pl_w->spd.y = scr->stage_top + -DOT_Y( 144.0f ) ;
	}
}

/*--------------------------------------------------------------*/
static void chaos_catch_up( PLAYER_WORK *pl_w )
{
	int dx, dy ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->catch_flag = 1 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += ( pl_w->spd.x - pl_w->pos.x ) / 16.0f ;
	pl_w->pos.y += ( pl_w->spd.y - pl_w->pos.y ) / 16.0f ;
	dx = ( Sint16 )pl_w->pos.x - ( Sint16 )pl_w->spd.x ;
	dy = ( Sint16 )pl_w->pos.y - ( Sint16 )pl_w->spd.y ;
	if( dx < 0 ) dx = -dx ;
	if( dy < 0 ) dy = -dy ;
	if( dx <= 1 && dy <= 1 ){
		pl_w->routine_3 ++ ;
		pl_w->pos.x = pl_w->spd.x ;
		pl_w->pos.y = pl_w->spd.y ;
		pl_char_set( pl_w, Char_Catch, 7 ) ;
	}
}

/*--------------------------------------------------------------*/
static void chaos_catch_burst( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	int i ;
	PLAYER_WORK *em ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->catch_flag = 1 ;
	if( pl_char_move( pl_w ) >= 0 ){
		if( pl_w->cg_type & 1 ){
			pl_w->cg_type ^= 1 ;
			scr_excep_set( pl_w, 1, 1 ) ;
			pls2d_00_set( pl_w, 6 ) ;
		}
	} else {
		for( i = 0 ; i < 16 ; i++ ){
			if( !( ew = pls2d_00_set( pl_w, 7 ) ) ) break ;
			ew->sub_type = i ;
		}
		em = pl_w->catch_w ;
		em->routine_2 ++ ;
		pl_w->routine_3 ++ ;
		pl_w->disp_flag = 0 ;
		pl_w->timer0 = 48 ;
	}
}

/*--------------------------------------------------------------*/
static void chaos_catch_drop( PLAYER_WORK *pl_w )
{
	PL2D_LOCAL_W *lw = PL2D_LW( pl_w ) ;
	PLAYER_WORK *em ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->catch_flag = 1 ;
	if( -- pl_w->timer0 == 0 ){
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->pl_dir = pl_w->pl_dir ;
		em->pl_stance = STANCE_AIR ;
		em->atck_no = pl2d_at_chaos_drop ;
		em->routine_2 = 0 ;
		em->ukemi_flag = 0 ;
		scr_excep_clr() ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_3 ++ ;
		pl_w->type = 0 ;
		pl_w->timer0 = 64 ;
		lw->release_flag = -1 ;
		RATE_CLR_X( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void chaos_catch_appear( PLAYER_WORK *pl_w )
{
	PL2D_LOCAL_W *lw = PL2D_LW( pl_w ) ;

	if( !pl_w->type ){
		if( -- pl_w->timer0 == 0 ){
			pl_w->type ++ ;
			pl_w->disp_flag = 1 ;
			pl_w->pl_stance = STANCE_AIR ;
			pl_w->pos.y = pl_w->ground_y ;
			pl_w->pos.y += DOT_Y( 64.0f ) ;
			pl_dir_set( pl_w ) ;
			pl_char_set( pl_w, Char_Appear, 0 ) ;
		}
	} else if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pls2d_00_set( pl_w, 1 ) ;
		pl_w->spd.y = -DOT_Y( 5.0f ) ;
		pl_w->acc.y = -0.9375f ;
		lw->power_step = 0 ;
		pl_char_set( pl_w, Char_Jump, 9 ) ;
	}
}

/*--------------------------------------------------------------*/
static void chaos_catch_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_land_ef_set( pl_w ) ;
		pl_stand_ret( pl_w ) ;
	}
}

static void power_catch( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A powered-up normal attack							*/
/*==============================================================*/
static void power_atck( PLAYER_WORK *pl_w )
{
	Uint8 atck ;
	Sint8 no ;

	if( !pl_w->pl_stance ){
		no = 1 ;
		atck = 58 ;
	} else {
		no = 2 ;
		atck = 59 ;
	}
	pl_w->atck_no = atck ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_char_set( pl_w, Char_Super, no ) ;
}

/*==============================================================*/
static void power_jump_atck( PLAYER_WORK *pl_w )
{
	pl_w->atck_no = pl2d_at_chaos_jump ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_char_set( pl_w, Char_Super, 3 ) ;
}

/*==============================================================*/
static void power_move( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type && pl_sp_catch_ck( pl_w ) ){
		power_catch( pl_w ) ;
		return ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void power_jump_move( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type && pl_sp_catch_ck( pl_w ) ){
		power_catch( pl_w ) ;
		return ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		pl_jump_ret( pl_w ) ;
		return ;
	}
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_land_set( pl_w ) ;
	}
}

/*==============================================================*/
/*			A powered-up attack hit: it becomes the grab,		*/
/*			and the power-up ends								*/
/*==============================================================*/
static void power_catch( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;	/* unused */
	PL2D_LOCAL_W *lw = PL2D_LW( pl_w ) ;
	PLAYER_WORK *em = pl_w->em_w ;

	lw->power_timer = 1 ;
	pl_w->catch_flag = 1 ;
	pl_w->catch_no = 0xc4 ;
	pl_sp_catch_set( pl_w, em ) ;
	nage_se_req( pl_w ) ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = 3 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl2d_sp_chaos ;
	em->routine_2 = 0 ;
	em->routine_3 = 0 ;
	em->ukemi_flag = 0 ;
	scr_excep_set( pl_w, 5, 0 ) ;
	pl_char_set( pl_w, Char_Catch, pl_w->pl_stance + 3 ) ;
	pl_w->delay_flag = 0xff ;
	pl_w->delay_timer = 16 ;
	pl_w->hyper_bg = 1 ;
	vs_sys_w->hyper_bg_req = ( pl_w->pl_no << 4 ) | 1 ;
	pl_w->hyper_bg_tm = 16 ;
}

static void hsmash_init( PLAYER_WORK *pl_w );
static void hsmash_move( PLAYER_WORK *pl_w );
static void hsmash_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_hsmash( PLAYER_WORK *pl_w )
{
	static void ( *hsmash_jmp[])( PLAYER_WORK *pl_w ) = {
		hsmash_init, hsmash_move, hsmash_end,
	} ;

	hsmash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void hsmash_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->routine_3 = 0 ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->timer0 = 10 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl2d_at_hsmash ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 7 ) ;
}

/*--------------------------------------------------------------*/
static void hsmash_move( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( !pl_w->routine_3 ){
		pl_w->delay_timer = 2 ;
		pl_w->hyper_bg_tm = 5 ;
		pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
		pl_char_move( pl_w ) ;
		if( pl_w->cg_type & 1 ){
			pl_w->stop_move_flag = 0 ;
			pl_w->stop_move_timer = 0 ;
			pl_w->routine_3 ++ ;
			pl_w->cg_type ^= 1 ;
			ofs.x = DOT_X( 40.0f ) ;
			ofs.y = -DOT_Y( 62.0f ) ;
			pl_hyper_start( pl_w, &ofs, 1 ) ;
		}
	} else {
		pl_w->delay_timer = 2 ;
		pl_w->hyper_bg_tm = 5 ;
		if( pl_char_move( pl_w ) >= 0 ){
			if( pl_w->cg_type & 2 ){
				pl_w->cg_type ^= 2 ;
				shl2d_01_set( pl_w ) ;
			}
		} else if( -- pl_w->timer0 == 0 ){
			pl_w->delay_flag = 0 ;
			pl_w->delay_timer = 0 ;
			pl_w->routine_2 ++ ;
			pl_w->routine_3 = 0 ;
			pl_char_set( pl_w, Char_Super, 9 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void hsmash_end( PLAYER_WORK *pl_w )
{
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
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
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_char_set( pl_w, Char_Extra, 0 ) ;
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
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl2d_at_snapback ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	ef_color_set( pl_w, 0, pl_w->pl_color * 48 + 34, 1 ) ;
	pl_char_set( pl_w, Char_Special, 20 ) ;
}

/*--------------------------------------------------------------*/
static void snapback_move( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
	} else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		ofs.x = -DOT_X( 14.0f ) ;
		ofs.y = DOT_Y( 80.0f ) ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void pl2d_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 = pl_w->routine_2 = pl_w->routine_1 = PL_NORM ;
	pl_w->pl_sp_type = pl2d_sp_hsmash ;
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl2d_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 = pl_w->routine_2 = pl_w->routine_1 = PL_NORM ;
	pl_w->pl_sp_type = pl2d_sp_hsmash ;
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl2d_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 = pl_w->routine_2 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl2d_sp_stare ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl2d_sp_smash ;
		pl_w->atck_level = 2 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl2d_sp_stare ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void pl2d_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 = pl_w->routine_2 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl2d_sp_stare ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
	case 2:
		pl_w->pl_sp_type = pl2d_sp_smash ;
		pl_w->atck_level = 2 ;
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
void pl2d_t_kick( PLAYER_WORK *pl_w )
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
	pl_w->spd.x = -DOT_X( 22.0f ) ;
	if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
	pl_w->spd.y = -DOT_Y( 4.0f ) ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl2d_at_t_kick ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_char_set( pl_w, Char_Special, 18 ) ;
}

/*--------------------------------------------------------------*/
static void t_kick_jump( PLAYER_WORK *pl_w )
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
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 19 ) ;
	}
}

/*--------------------------------------------------------------*/
static void t_kick_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stop_stand_ret( pl_w ) ;
}

/****************************************************************/
/*			Can a throw start?									*/
/****************************************************************/
PLAYER_WORK *pl2d_catch_ck( PLAYER_WORK *pl_w )
{
	PL2D_LOCAL_W *lw = PL2D_LW( pl_w ) ;
	PLAYER_WORK *em ;

	if( lw->power_step ) return 0 ;
	if( lw->unk_0c ) return 0 ;
	if( !( pl_w->atck_lever & ( KEY_FORWARD | KEY_BACK ) ) ) return 0 ;
	if( !pl_w->atck_level ) return 0 ;
	if( !pl_w->atck_pk ){
		if( pl_w->pl_stance != STANCE_AIR ){
			if( ( em = pl_catch_ck( pl_w ) ) ){
				pl_w->catch_no = 0 ;
				return em ;
			}
		} else {
			if( ( em = pl_catch_ck( pl_w ) ) ){
				pl_w->catch_no = 1 ;
				return em ;
			}
		}
	} else {
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 2 ;
			return em ;
		}
	}
	return 0 ;
}

static void nml_p_set( PLAYER_WORK *pl_w );
static void jump_p_set( PLAYER_WORK *pl_w );
static void k_set( PLAYER_WORK *pl_w );
static void catch_set_none( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Start the throw that pl2d_catch_ck chose			*/
/****************************************************************/
void pl2d_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_set, jump_p_set, k_set, catch_set_none, catch_set_none,
	} ;

	catch_set_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	scr_excep_set( pl_w, 5, 5 ) ;
	if( pl_w->atck_lever & KEY_FORWARD ){
		pl_w->disp_dir ^= 1 ;
		pl_w->pl_dir ^= 1 ;
	}
	catch_gauge_add( pl_w ) ;
	ofs.x = -40.0f ;
	ofs.y = DOT_Y( 64.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	pl_w->routine_3 = pl_w->routine_2 = 0 ;
	pl_char_set( pl_w, Char_Catch, 0 ) ;
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
	catch_gauge_add( pl_w ) ;
	ofs.x = -DOT_X( 64.0f ) ;
	ofs.y = DOT_Y( 80.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	pl_w->routine_3 = pl_w->routine_2 = 0 ;
	pl_char_set( pl_w, Char_Catch, 1 ) ;
}

/*==============================================================*/
static void k_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	scr_excep_set( pl_w, 5, 5 ) ;
	if( pl_w->atck_lever & KEY_BACK ){
		pl_w->disp_dir ^= 1 ;
		pl_w->pl_dir ^= 1 ;
	}
	catch_gauge_add( pl_w ) ;
	ofs.x = -DOT_X( 80.0f ) ;
	ofs.y = DOT_Y( 48.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	pls2d_00_set( pl_w, 2 ) ;
	hold_renda_set( pl_w->catch_w, 50 ) ;
	catch_renda_set( pl_w, 3 ) ;
	pl_w->routine_3 = pl_w->routine_2 = 0 ;
	pl_char_set( pl_w, Char_Catch, 2 ) ;
}

/*==============================================================*/
static void catch_set_none( PLAYER_WORK *pl_w )
{
}

static void nml_p_catch( PLAYER_WORK *pl_w );
static void jump_p_catch( PLAYER_WORK *pl_w );
static void k_catch( PLAYER_WORK *pl_w );
static void catch_none_devital( PLAYER_WORK *pl_w );
static void catch_none_chaos( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void pl2d_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_catch, jump_p_catch, k_catch, catch_none_devital, catch_none_chaos,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

static void nml_p_catch_init( PLAYER_WORK *pl_w );
static void nml_p_catch_move( PLAYER_WORK *pl_w );
static void nml_p_catch_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void nml_p_catch( PLAYER_WORK *pl_w )
{
	static void ( *nml_p_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_catch_init, nml_p_catch_move, nml_p_catch_end,
	} ;

	nml_p_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_catch_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	nml_p_catch_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void nml_p_catch_move( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->pl_dir = pl_w->pl_dir ;
		em->pl_stance = STANCE_AIR ;
		em->atck_no = pl2d_at_throw_p ;
		pl_w->dir = 5 ;
		if( pl_w->pl_dir ) pl_w->dir = 27 ;
		scr_excep_clr() ;
	}
}

/*--------------------------------------------------------------*/
static void nml_p_catch_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_dir_set( pl_w ) ;
		pl_stand_ret( pl_w ) ;
	}
}

static void jump_p_catch_init( PLAYER_WORK *pl_w );
static void jump_p_catch_move( PLAYER_WORK *pl_w );
static void jump_p_catch_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void jump_p_catch( PLAYER_WORK *pl_w )
{
	static void ( *jump_p_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		jump_p_catch_init, jump_p_catch_move, jump_p_catch_end,
	} ;

	jump_p_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void jump_p_catch_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	jump_p_catch_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void jump_p_catch_move( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->pl_dir = pl_w->pl_dir ;
		em->pl_stance = STANCE_AIR ;
		em->atck_no = pl2d_at_air_throw ;
		pl_w->dir = 10 ;
		if( pl_w->pl_dir ) pl_w->dir = 22 ;
		scr_excep_clr() ;
	}
}

/*--------------------------------------------------------------*/
static void jump_p_catch_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.y = -DOT_Y( 3.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.25f ) ;
		pl_jump_ret( pl_w ) ;
	}
}

static void k_catch_init( PLAYER_WORK *pl_w );
static void k_catch_move( PLAYER_WORK *pl_w );
static void k_catch_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void k_catch( PLAYER_WORK *pl_w )
{
	static void ( *k_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		k_catch_init, k_catch_move, k_catch_end,
	} ;

	k_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void k_catch_init( PLAYER_WORK *pl_w )
{
	PL2D_LOCAL_W *lw = PL2D_LW( pl_w ) ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 120 ;
		pl_w->type = ( pl_w->pl_stance & 2 ) >> 1 ;
		lw->col_timer = 9 ;
		lw->col_cnt = 0 ;
		lw->col_no = 1 ;
		pl_w->type = ( pl_w->pl_stance & 2 ) >> 1 ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = DOT_Y( 0.125f ) ;
		pl_w->pos.y -= DOT_Y( 4.0f ) ;
		pl_voice_req( pl_w, 36 ) ;
	}
}

/*--------------------------------------------------------------*/
static void k_catch_move( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->catch_w ;
	Float spd ;
	Uint8 atck ;

	if( pl_char_move( pl_w ) < 0 ){
		hold_damage( em, -3 ) ;
		if( !em->vital_new ) goto release ;
		vital_add( pl_w, 2 ) ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	spd = pl_w->spd.y ;
	if( spd < 0.0f ) spd = -spd ;
	if( spd >= DOT_Y( 1.375f ) ) pl_w->acc.y = -pl_w->acc.y ;
	if( -- pl_w->timer0 == 0 ) goto release ;
	if( catch_renda_ck( pl_w ) ) pl_w->cg_ctr = 1 ;
	if( !hold_renda_ck( em ) ) return ;
release:
	pl_w->routine_2 ++ ;
	pl_w->routine_3 = 0 ;
	pl_w->timer0 = 0 ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_voice_req( pl_w, 43 ) ;
	atck = 37 ;
	if( pl_w->pl_stance != STANCE_AIR ){
		atck = 33 ;
		pl_w->pos.y = pl_w->ground_y ;
	}
	em->dm_hit_w = pl_w ;
	em->catch_type = 1 ;
	em->catch_cnt = 2 ;
	em->pl_dir = pl_w->pl_dir ;
	em->pl_stance = STANCE_AIR ;
	em->atck_no = atck ;
	scr_excep_clr() ;
	pl_w->dir = 24 ;
	pl_w->spd.x = 5.0f ;
	if( pl_w->pl_dir ){
		pl_w->dir = 8 ;
		pl_w->spd.x = -pl_w->spd.x ;
	}
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 5.0f ) ;
	pl_w->acc.y = -0.9375f ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_char_set( pl_w, Char_Catch, 11 ) ;
}

/*--------------------------------------------------------------*/
static void k_catch_end( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->type ){
		if( pl_char_move( pl_w ) < 0 ){
			pl_w->jump_com_cnt = 1 ;
			pl_sjump_ret( pl_w ) ;
		} else {
			if( pl_w->pos.y <= pl_w->ground_y ){
				pl_w->routine_3 ++ ;
				pl_w->pos.y = pl_w->ground_y ;
				pl_w->pl_stance = STANCE_STAND ;
				pl_w->spd.x = 0.0f ;
				pl_w->spd.y = 0.0f ;
				pl_w->acc.x = 0.0f ;
				pl_w->acc.y = 0.0f ;
				pl_land_ef_set( pl_w ) ;
				pl_char_set( pl_w, Char_Catch, 10 ) ;
			}
		}
	} else if( !pl_w->routine_3 ){
		pl_char_move( pl_w ) ;
		if( pl_w->pos.y <= pl_w->ground_y ){
			pl_w->routine_3 ++ ;
			pl_w->pos.y = pl_w->ground_y ;
			pl_w->pl_stance = STANCE_STAND ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_land_ef_set( pl_w ) ;
			pl_char_set( pl_w, Char_Catch, 10 ) ;
		}
	} else if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void catch_none_devital( PLAYER_WORK *pl_w )
{
}

/*==============================================================*/
static void catch_none_chaos( PLAYER_WORK *pl_w )
{
}

static void hold_nml( PLAYER_WORK *pl_w );
static void hold_power( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The throw's hold on the player being thrown			*/
/****************************************************************/
void pl2d_hold( PLAYER_WORK *pl_w )
{
	static void ( *hold_jmp[])( PLAYER_WORK *pl_w ) = {
		hold_nml, hold_nml, hold_nml, hold_nml, hold_power,
	} ;

	hold_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void hold_nml( PLAYER_WORK *pl_w )
{
	catch_pos_set( pl_w->catch_w, pl_w ) ;
}

static void hold_power_keep( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void hold_power_start( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void hold_power_float( PLAYER_WORK *pl_w, PLAYER_WORK *em );

/*==============================================================*/
static void hold_power( PLAYER_WORK *pl_w )
{
	static void ( *hold_power_jmp[])( PLAYER_WORK *pl_w, PLAYER_WORK *em ) = {
		hold_power_keep, hold_power_start, hold_power_float,
	} ;

	hold_power_jmp[pl_w->routine_2]( pl_w, pl_w->catch_w ) ;
}

/*==============================================================*/
static void hold_power_keep( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	catch_pos_set( em, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void hold_power_start( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 5.0f ) ;
	pl_w->acc.y = -pl_w->spd.y / 16.0f ;
	pl_char_set( pl_w, Char_Damage, 29 ) ;
}

/*--------------------------------------------------------------*/
static void hold_power_float( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	Float spd ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	spd = pl_w->spd.y ;
	if( spd < 0.0f ) spd = -pl_w->spd.y ;
	if( spd >= DOT_Y( 5.0f ) ) pl_w->acc.y = -( pl_w->acc.y * 2.0f ) ;
	pl_w->cg_ctr = 1 ;
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl2d.c" End of File	*/

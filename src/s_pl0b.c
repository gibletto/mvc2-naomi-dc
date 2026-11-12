/* Captain America (character 0b): command checks, normal attacks, appearance, win and lose poses,
   special moves and hyper combos, the shield he throws, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "burst_ef_old.h"
#include "pl0b_def.h"
#include "shl0b_00_old.h"

void pl0b_main( PLAYER_WORK *pl_w );
void pl0b_com_ck( PLAYER_WORK *pl_w );
void pl0b_atck_set( PLAYER_WORK *pl_w );
void pl0b_atck( PLAYER_WORK *pl_w );
void pl0b_dash( PLAYER_WORK *pl_w );
void pl0b_noki( PLAYER_WORK *pl_w );
void pl0b_appear( PLAYER_WORK *pl_w );
void pl0b_win( PLAYER_WORK *pl_w );
void pl0b_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl0b_catch_ck( PLAYER_WORK *pl_w );
void pl0b_catch_set( PLAYER_WORK *pl_w );
void pl0b_catch( PLAYER_WORK *pl_w );
void pl0b_hold( PLAYER_WORK *pl_w );
int pl0b_delay_ck( PLAYER_WORK *pl_w );
void pl0b_t_kick( PLAYER_WORK *pl_w );
void pl0b_upper( PLAYER_WORK *pl_w );
void pl0b_sp_unison( PLAYER_WORK *pl_w );
void pl0b_sp_gattai( PLAYER_WORK *pl_w );
void pl0b_sp_assist( PLAYER_WORK *pl_w );
void pl0b_sp_zero( PLAYER_WORK *pl_w );
void pl0b_tlocal_init( PLAYER_WORK *pl_w );
int pl0b_shield_look( PLAYER_WORK *pl_w );
int pl0b_shield_guard_ck( PLAYER_WORK *pl_w, PLAYER_WORK *em );

/* how sp_stars rises and falls, by strength: speed and acceleration in x, then in y */
static Sint32 stars_rise_tbl[4][4] = {
	{ 0x00040000, -0x8000, 0x000a0000, -0x8000 }, { 0x000b0000, -0x8000, 0x00120000, -0x8000 },
	{ 0x00020000, 0x1000, 0x000a0000, 0x6000 }, { 0x00020000, 0x1000, 0x000a0000, 0x6000 },
} ;

static Sint32 stars_fall_tbl[4][4] = {
	{ 0, 0, -0x50000, -0x6800 }, { 0, 0, -0x80000, -0x6800 },
	{ 0, 0, -0x40000, -0x6000 }, { 0, 0, -0x40000, -0x6000 },
} ;

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
static Uint16 com_hstars[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl0b_sp_hstars, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_PUNCH } ;	/* Hyper Stars & Stripes */
static Uint16 com_justice[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl0b_sp_justice, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Final Justice */
static Uint16 com_hcharge[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl0b_sp_hcharge, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Hyper Charging Star */
static Uint16 com_stars[] = { 3, COM_GROUND, pl0b_sp_stars, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_PUNCH } ;	/* Stars & Stripes */
static Uint16 com_slash[] = { 3, 0, pl0b_sp_slash, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Shield Slash */
static Uint16 com_cartwheel[] = { 5, COM_GROUND, pl0b_sp_cartwheel, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Roll */
static Uint16 com_charge[] = { 3, COM_GROUND, pl0b_sp_charge, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Charging Star */

static void ( *pl0b_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl0b_main,
	pl0b_com_ck,
	pl0b_atck_set,
	pl0b_atck,
	pl0b_dash,
	pl0b_noki,
	pl0b_appear,
	pl0b_win,
	pl0b_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl0b_catch_ck,	/* returns a value */
	pl0b_catch_set,
	pl0b_catch,
	pl0b_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl0b_delay_ck,
	pl0b_t_kick,
	pl0b_upper,
	pl0b_sp_unison,
	pl0b_sp_gattai,
	pl0b_sp_assist,
	pl0b_sp_zero,
	0,
	0,
	0,
	pl0b_tlocal_init,
	( void ( * )( PLAYER_WORK *pl_w ) )pl0b_shield_look,
	( void ( * )( PLAYER_WORK *pl_w ) )pl0b_shield_guard_ck,	/* takes the attacker too */
} ;

static Uint8 hstars_ck( PLAYER_WORK *pl_w );
static Uint8 justice_ck( PLAYER_WORK *pl_w );
static Uint8 hcharge_ck( PLAYER_WORK *pl_w );
static Uint8 stars_ck( PLAYER_WORK *pl_w );
static Uint8 slash_ck( PLAYER_WORK *pl_w );
static Uint8 cartwheel_ck( PLAYER_WORK *pl_w );
static Uint8 charge_ck( PLAYER_WORK *pl_w );
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static int hstars_delay_ck( PLAYER_WORK *pl_w );
static int hcharge_delay_ck( PLAYER_WORK *pl_w );
static int justice_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl0b_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl0b_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl0b_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( hstars_ck( pl_w ) ) return ;
	if( justice_ck( pl_w ) ) return ;
	if( hcharge_ck( pl_w ) ) return ;
	if( stars_ck( pl_w ) ) return ;
	if( slash_ck( pl_w ) ) return ;
	if( cartwheel_ck( pl_w ) ) return ;
	if( charge_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
static Uint8 hstars_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_hstars, &pl_w->com_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->atck_level = 2 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0b_sp_hstars ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 justice_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_justice, &pl_w->com_w0 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0b_sp_justice ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 hcharge_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_hcharge, &pl_w->com_w1 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0b_sp_hcharge ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 stars_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_stars, &pl_w->com_w2 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0b_sp_stars ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 slash_ck( PLAYER_WORK *pl_w )
{
	PL0B_LOCAL_W *lw = ( PL0B_LOCAL_W * )pl_w->tlocal ;

	if( !com_nml_ck( pl_w, com_slash, &pl_w->com_w3 ) ) return 0 ;
	if( lw->shield_st ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w3 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0b_sp_slash ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 cartwheel_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_cartwheel, &pl_w->com_w4 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w4 ) ;
	pl_w->sp_ng_timer = 3 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0b_sp_cartwheel ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 charge_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_charge, &pl_w->com_w5 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w5 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0b_sp_charge ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 4 ) ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0b_sp_chouhatsu ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int snapback_ck( PLAYER_WORK *pl_w )
{
	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl0b_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static int hstars_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_hstars, &pl_w->com_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl0b_sp_hstars ;
	return 1 ;
}

/*==============================================================*/
static int hcharge_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_hcharge, &pl_w->com_w1 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl0b_sp_hcharge ;
	return 1 ;
}

/*==============================================================*/
static int justice_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_justice, &pl_w->com_w0 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl0b_sp_justice ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl0b_delay_ck( PLAYER_WORK *pl_w )
{
	if( hstars_delay_ck( pl_w ) || justice_delay_ck( pl_w ) || hcharge_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/****************************************************************/
void pl0b_main( PLAYER_WORK *pl_w )
{
	PL0B_LOCAL_W *lw = ( PL0B_LOCAL_W * )pl_w->tlocal ;

	if( !lw->guard_flag ) return ;
	if( pl_w->routine_1 == PL_NORM ){
		if( pl_w->act_st == N_SPECIAL && pl_w->cg_type ) return ;
		if( pl_w->act_st == N_SUPER && pl_w->cg_type ) return ;
	}
	lw->guard_flag = 0 ;
}

/*==============================================================*/
static void stand_punch( PLAYER_WORK *pl_w )
{
	PL0B_LOCAL_W *lw = ( PL0B_LOCAL_W * )pl_w->tlocal ;

	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_STAND_LP ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		pl_w->unk_2a2 = 1 ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		if( lw->shield_st ) pl_w->atck_no = ATCK_STAND_MP ;
		else pl_w->atck_no = pl0b_at_shield_mp ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		if( lw->shield_st ){
			pl_w->char_no.b.no = 2 ;
			pl_w->atck_no = ATCK_STAND_HP ;
		} else {
			pl_w->char_no.b.no = 4 ;
			pl_w->atck_no = pl0b_at_shield_hp ;
		}
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
	PL0B_LOCAL_W *lw = ( PL0B_LOCAL_W * )pl_w->tlocal ;

	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.no = 0 ;
		if( lw->shield_st ) pl_w->atck_no = ATCK_CROUCH_LP ;
		else pl_w->atck_no = pl0b_at_shield_clp ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		if( lw->shield_st ){
			pl_w->char_no.b.no = 1 ;
			pl_w->atck_no = ATCK_CROUCH_MP ;
		} else {
			pl_w->char_no.b.no = 4 ;
			pl_w->atck_no = pl0b_at_shield_cmp ;
		}
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		if( lw->shield_st ){
			pl_w->char_no.b.no = 2 ;
			pl_w->atck_no = ATCK_CROUCH_HP ;
		} else {
			pl_w->char_no.b.no = 5 ;
			pl_w->atck_no = pl0b_at_shield_chp ;
		}
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
		pl_se_req( pl_w, SE_SWING_M ) ;
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
static void jump_punch( PLAYER_WORK *pl_w )
{
	PL0B_LOCAL_W *lw = ( PL0B_LOCAL_W * )pl_w->tlocal ;

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
		if( lw->shield_st ){
			pl_w->char_no.b.no = 1 ;
			pl_w->atck_no = ATCK_JUMP_MP ;
		} else {
			pl_w->char_no.b.no = 6 ;
			pl_w->atck_no = pl0b_at_shield_jmp ;
		}
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_JUMP_MP ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mp_tbl ;
		else pl_w->chain_tbl = sj_mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		if( lw->shield_st ){
			pl_w->char_no.b.no = 2 ;
			pl_w->atck_no = ATCK_JUMP_HP ;
		} else {
			pl_w->char_no.b.no = 7 ;
			pl_w->atck_no = pl0b_at_shield_jhp ;
		}
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
		if( pl_w->atck_lever & KEY_DOWN ){
			pl_w->char_no.b.no = 3 ;
			pl_w->atck_no = pl0b_at_jdown_hk ;
		} else if( pl_w->atck_lever & KEY_UP ){
			pl_w->char_no.b.no = 4 ;
			pl_w->atck_no = pl0b_at_jup_hk ;
		} else {
			pl_w->char_no.b.no = 2 ;
			pl_w->atck_no = ATCK_JUMP_HK ;
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

/*==============================================================*/
static void jump_atck( PLAYER_WORK *pl_w )
{
	if( pl_w->atck_pk == 1 ) jump_kick( pl_w ) ;
	else jump_punch( pl_w ) ;
}

/*==============================================================*/
static void jump_atck_ck( PLAYER_WORK *pl_w )
{
	if( ( pl_w->atck_pk == 0 && ( pl_w->jump_atk_cnt & JATK_P_MASK ) )
	 || ( pl_w->atck_pk != 0 && ( pl_w->jump_atk_cnt & JATK_K_MASK ) ) ) jump_atck( pl_w ) ;
}

/*==============================================================*/
static void nml_atck_dash( PLAYER_WORK *pl_w )
{
	nml_atck( pl_w ) ;
}

/****************************************************************/
/*			Normal attacks										*/
/****************************************************************/
void pl0b_atck_set( PLAYER_WORK *pl_w )
{
	static void ( *atck_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_atck, jump_atck_ck, jump_atck, nml_atck_dash,
	} ;

	atck_set_jmp[pl_w->atck_type]( pl_w ) ;
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
/*			The medium one goes on to a second kick if the		*/
/*			button is pressed again								*/
/*==============================================================*/
static void stand_kick_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		else if( pl_w->routine_2 == 0 && pl_w->cg_type && ( ( pl_w->pl_sw_now | pl_w->pl_stop_now ) & 0x40 ) ){
			pl_w->pl_stop_now = 0 ;
			pl_w->routine_2 ++ ;
			pl_char_set( pl_w, Char_Stand_K, 4 ) ;
			pl_se_req( pl_w, SE_SWING_M ) ;
			pl_w->atck_no = pl0b_at_stand_mk_2 ;
			HIT_RESET( pl_w ) ;
		}
		break ;
	case 2:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
static void crouch_kick_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
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
static void jump_punch_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/*==============================================================*/
static void jump_kick_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
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
static void sjump_move( PLAYER_WORK *pl_w )
{
	pl_jump_hit_ck( pl_w ) ;
	pl_sjump_ctrl( pl_w ) ;
	jump_move( pl_w ) ;
}

/*==============================================================*/
static void nml_move_dash( PLAYER_WORK *pl_w )
{
	pl_smoke_set( pl_w ) ;
	nml_move( pl_w ) ;
}

/****************************************************************/
void pl0b_atck( PLAYER_WORK *pl_w )
{
	static void ( *atck_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_move, jump_move, sjump_move, nml_move_dash,
	} ;

	atck_jmp[pl_w->atck_type]( pl_w ) ;
}

/****************************************************************/
/*			The attacks done with the stick held				*/
/****************************************************************/
void pl0b_upper( PLAYER_WORK *pl_w )
{
	PL0B_LOCAL_W *lw = ( PL0B_LOCAL_W * )pl_w->tlocal ;

	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_gauge_add( pl_w, 5 ) ;
		if( pl_w->atck_pk == 0 ){
			pl_w->pl_stance = STANCE_CROUCH ;
			if( lw->shield_st ){
				pl_w->atck_no = pl0b_at_upper_noshield ;
				pl_char_set( pl_w, Char_Extra, 7 ) ;
			} else {
				pl_w->atck_no = pl0b_at_upper ;
				pl_char_set( pl_w, Char_Extra, 8 ) ;
			}
			pl_se_req( pl_w, SE_SWING_H ) ;
			HIT_RESET( pl_w ) ;
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
static void dash_stop( PLAYER_WORK *pl_w );
static void dash_end( PLAYER_WORK *pl_w );

/****************************************************************/
void pl0b_dash( PLAYER_WORK *pl_w )
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
		pl_w->acc.x = pl_w->pl_dir ? -0.3645833f : 0.3645833f ;
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
	if( pl_w->timer0-- == 0 ) pl_w->routine_2 ++ ;
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
		pl_char_set( pl_w, Char_Dash, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void dash_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void noki_init( PLAYER_WORK *pl_w );
static void noki_step( PLAYER_WORK *pl_w );
static void noki_jump( PLAYER_WORK *pl_w );
static void noki_fall( PLAYER_WORK *pl_w );
static void noki_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash: two steps back, then a hop			*/
/****************************************************************/
void pl0b_noki( PLAYER_WORK *pl_w )
{
	static void ( *noki_jmp[])( PLAYER_WORK *pl_w ) = {
		noki_init, noki_step, noki_jump, noki_fall, noki_land,
	} ;

	noki_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void noki_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 8.0f ) : DOT_X( 8.0f ) ;
	pl_w->acc.x = pl_w->pl_dir ? -0.104166664f : 0.104166664f ;
	noki_step( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void noki_step( PLAYER_WORK *pl_w )
{
	Float step ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		step = pl_w->pl_dir ? -DOT_X( 80.0f ) : DOT_X( 80.0f ) ;
		if( pl_w->cg_type < 0 ){
			pl_w->routine_2 ++ ;
			step = pl_w->pl_dir ? -80.0f : 80.0f ;
		}
		pl_w->cg_type = 0 ;
		pl_w->pos.x += step ;
	}
}

/*--------------------------------------------------------------*/
static void noki_jump( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->spd.y = DOT_Y( 4.5f ) ;
		pl_w->acc.y = -2.142857f ;
		if( pl_w->pl_dir ){
			pl_w->spd.x = -DOT_X( 8.0f ) ;
			pl_w->acc.x = DOT_X( 0.5f ) ;
		} else {
			pl_w->spd.x = DOT_X( 8.0f ) ;
			pl_w->acc.x = -DOT_X( 0.5f ) ;
		}
		pl_w->routine_2 ++ ;
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
}

/*--------------------------------------------------------------*/
static void noki_fall( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_char_set( pl_w, Char_Dash, 3 ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		if( pl_w->pl_dir ){
			pl_w->pos.x += -DOT_X( 32.0f ) ;
			pl_w->spd.x = -DOT_X( 3.5f ) ;
			pl_w->acc.x = DOT_X( 0.25f ) ;
		} else {
			pl_w->pos.x += DOT_X( 32.0f ) ;
			pl_w->spd.x = DOT_X( 3.5f ) ;
			pl_w->acc.x = -DOT_X( 0.25f ) ;
		}
		pl_w->routine_2 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void noki_land( PLAYER_WORK *pl_w )
{
	Float spd = pl_w->spd.x ;

	if( !pl_w->pl_dir ) spd = -spd ;
	if( spd < 0.0f ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void appear_init( PLAYER_WORK *pl_w );
static void appear_jump( PLAYER_WORK *pl_w );
static void appear_shield( PLAYER_WORK *pl_w );

/****************************************************************/
/*			He comes in one of two ways, picked at random		*/
/****************************************************************/
void pl0b_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_init, appear_jump, appear_shield,
	} ;

	appear_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void appear_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->routine_3 = 0 ;
	pl_w->disp_flag = 1 ;
	if( nlRand() & 1 ) appear_jump( pl_w ) ;
	else {
		pl_w->routine_2 ++ ;
		appear_shield( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_jump_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_w->routine_1 ++ ;
}

/*--------------------------------------------------------------*/
static void appear_jump_fall( PLAYER_WORK *pl_w )
{
	PL0B_LOCAL_W *lw = ( PL0B_LOCAL_W * )pl_w->tlocal ;

	if( pl_w->cg_type < 0 ) pl_char_move( pl_w ) ;
	else {
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->pos.y > pl_w->ground_y ) pl_char_move( pl_w ) ;
		else {
			pl_w->pos.x = lw->appear_x ;
			pl_w->pos.y = pl_w->ground_y ;
			pl_land_ef_set( pl_w ) ;
			pl_w->routine_3 ++ ;
			pl_char_set( pl_w, Char_Appear, 4 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void appear_jump_fly( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->cg_type <= 0 ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = -( pl_w->spd.x / 4.0f ) ;
		pl_w->spd.y = 2.142857f ;
		pl_w->acc.y = -0.4017857f ;
	}
	if( !( Sys->game_timer0 & 3 ) ) pls0b_01_set( pl_w, 2, 0 ) ;
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void appear_jump_wait( PLAYER_WORK *pl_w )
{
	if( vs_sys_work.vs_st < VS_APPEAR ){
		pl_w->disp_flag = 0 ;
		return ;
	}
	pl_w->routine_3 ++ ;
	pl_w->disp_flag = 1 ;
	appear_jump_fly( pl_w ) ;
}

/*==============================================================*/
static void appear_jump_init( PLAYER_WORK *pl_w )
{
	PL0B_LOCAL_W *lw = ( PL0B_LOCAL_W * )pl_w->tlocal ;
	Sint32 x, spd ;

	lw->appear_x = pl_w->pos.x ;
	lw->appear_y = pl_w->pos.y ;
	pl_w->pos.y += DOT_Y( 80.0f ) ;
	x = -0x4fa000 ;
	if( pl_w->pl_no ) x = 0x4fa000 ;
	pl_w->pos.x += x * 1.6666666f / 65536.0f ;
	spd = 0x68000 ;
	if( pl_w->pl_no ) spd = -0x68000 ;
	pl_w->spd.x = spd * 1.6666666f / 65536.0f ;
	pl_w->spd.y = -DOT_Y( 0.375f ) ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_char_set( pl_w, Char_Appear, 3 ) ;
	pls0b_01_set( pl_w, 0, 0 ) ;
	pl_w->routine_3 ++ ;
}

/*==============================================================*/
/*			He jumps in from off the screen						*/
/*==============================================================*/
static void appear_jump( PLAYER_WORK *pl_w )
{
	static void ( *appear_jump_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_jump_init, appear_jump_wait, appear_jump_fly, appear_jump_fall, appear_jump_land,
	} ;

	appear_jump_jmp[pl_w->routine_3]( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void appear_shield_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_w->routine_1 ++ ;
}

/*--------------------------------------------------------------*/
static void appear_shield_jump( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type >= 0 ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_land_ef_set( pl_w ) ;
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Appear, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_shield_keep( PLAYER_WORK *pl_w )
{
	if( -- pl_w->timer0 ){
		pl_char_move( pl_w ) ;
		return ;
	}
	pl_w->routine_3 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 6.625f ) ;
	pl_w->acc.y = -DOT_Y( 0.25f ) ;
	pl_char_set( pl_w, Char_Appear, 1 ) ;
}

/*--------------------------------------------------------------*/
static void appear_shield_wait( PLAYER_WORK *pl_w )
{
	if( vs_sys_work.vs_st != VS_APPEAR ){
		pl_char_move( pl_w ) ;
		return ;
	}
	pl_w->routine_3 ++ ;
	appear_shield_keep( pl_w ) ;
}

/*==============================================================*/
static void appear_shield_init( PLAYER_WORK *pl_w )
{
	PL0B_LOCAL_W *lw = ( PL0B_LOCAL_W * )pl_w->tlocal ;

	pl_w->routine_3 ++ ;
	lw->shield_st = 1 ;
	lw->shield_sub = 8 ;
	pl_w->timer0 = 64 ;
	pl_char_set( pl_w, Char_Appear, 0 ) ;
}

/*==============================================================*/
/*			He stands, then jumps up to catch his shield		*/
/*==============================================================*/
static void appear_shield( PLAYER_WORK *pl_w )
{
	static void ( *appear_shield_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_shield_init, appear_shield_wait, appear_shield_keep, appear_shield_jump, appear_shield_land,
	} ;

	appear_shield_jmp[pl_w->routine_3]( pl_w ) ;
}

static void win_init( PLAYER_WORK *pl_w );
static void win_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl0b_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_init, win_move,
	} ;

	win_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			The win poses after a knockout: sub_type keeps		*/
/*			the one picked										*/
/*==============================================================*/
static void win_ko_set( PLAYER_WORK *pl_w )
{
	PL0B_LOCAL_W *lw = ( PL0B_LOCAL_W * )pl_w->tlocal ;

	if( !( Sys->game_timer0 & 1 ) || Sys->ending_flag ){
		if( vs_sys_work.vs_end_flag && pl_be_cnt( pl_w ) == 1 ){
			pl_w->sub_type = 5 ;
			pl_w->char_no.b.no = 7 ;
			pls0b_00_set( pl_w, 0, 0 ) ;
		} else if( nlRand() & 1 ){
			pl_w->sub_type = 0 ;
			pl_w->char_no.b.no = 0 ;
		} else {
			pl_w->sub_type = 2 ;
			pl_w->char_no.b.no = 4 ;
			shield_req( lw, 0, 2 ) ;
		}
	} else {
		if( vs_sys_work.vs_end_flag && pl_be_cnt( pl_w ) == 1 ){
			pl_w->sub_type = 1 ;
			pl_w->char_no.b.no = 1 ;
		} else if( nlRand() & 1 ){
			pl_w->sub_type = 0 ;
			pl_w->char_no.b.no = 0 ;
		} else {
			pl_w->sub_type = 2 ;
			pl_w->char_no.b.no = 4 ;
			shield_req( lw, 0, 2 ) ;
		}
	}
	pl_w->char_no.b.grp = Char_Win ;
	pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
static void win_time_set( PLAYER_WORK *pl_w )
{
	pl_w->sub_type = 3 ;
	pl_w->char_no.b.grp = Char_Win ;
	pl_w->char_no.b.no = 2 ;
	pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
static void lose_set( PLAYER_WORK *pl_w )
{
	pl_w->sub_type = 4 ;
	pl_w->char_no.b.grp = Char_Win ;
	pl_w->char_no.b.no = 3 ;
	pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
static void win_init( PLAYER_WORK *pl_w )
{
	static void ( *win_set_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko_set, lose_set, win_time_set, lose_set, lose_set,
	} ;

	pl_w->routine_2 ++ ;
	pl_w->routine_3 = 0 ;
	win_set_jmp[pl_w->type]( pl_w ) ;
}

/*==============================================================*/
static void win_wait( PLAYER_WORK *pl_w )
{
	if( pl_win_end_ck( pl_w ) ) pl_stand_ret( pl_w ) ;
	else pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void win0_move( PLAYER_WORK *pl_w )
{
	static void ( *win0_jmp[])( PLAYER_WORK *pl_w ) = {
		win_wait,
	} ;

	win0_jmp[pl_w->routine_3]( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_cross_pose( PLAYER_WORK *pl_w )
{
	PL0B_LOCAL_W *lw = ( PL0B_LOCAL_W * )pl_w->tlocal ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 = 1 ;
		lw->away_flag = 0 ;
		pl_w->char_no.b.grp = Char_Win ;
		pl_w->char_no.b.no = 6 ;
		pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
	}
}

/*--------------------------------------------------------------*/
static void win_cross_wait( PLAYER_WORK *pl_w )
{
	PL0B_LOCAL_W *lw = ( PL0B_LOCAL_W * )pl_w->tlocal ;

	if( lw->away_flag ){
		pl_w->routine_3 ++ ;
		pl_w->char_no.b.grp = Char_Win ;
		pl_w->char_no.b.no = 5 ;
		pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
		win_cross_pose( pl_w ) ;
	}
}

/*==============================================================*/
static void win_cross_start( PLAYER_WORK *pl_w )
{
	PL0B_LOCAL_W *lw = ( PL0B_LOCAL_W * )pl_w->tlocal ;

	if( pl_char_move( pl_w ) >= 0 ){
		lw->away_flag = 0 ;
		pl_w->timer0 = 64 ;
		return ;
	}
	pl_w->routine_3 ++ ;
	win_cross_wait( pl_w ) ;
}

/*==============================================================*/
static void win_cross_move( PLAYER_WORK *pl_w )
{
	static void ( *win_cross_jmp[])( PLAYER_WORK *pl_w ) = {
		win_cross_start, win_cross_wait, win_cross_pose,
	} ;

	if( pl_win_end_ck( pl_w ) ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( !( Sys->game_timer0 & 7 ) ) pls0b_00_set( pl_w, 2, 0 ) ;
	win_cross_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void win_shield_move( PLAYER_WORK *pl_w )
{
	static void ( *win_shield_jmp[])( PLAYER_WORK *pl_w ) = {
		win_wait,
	} ;

	win_shield_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void win_time_move( PLAYER_WORK *pl_w )
{
	static void ( *win_time_jmp[])( PLAYER_WORK *pl_w ) = {
		win_wait,
	} ;

	win_time_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void lose_move( PLAYER_WORK *pl_w )
{
	static void ( *lose_jmp[])( PLAYER_WORK *pl_w ) = {
		win_wait,
	} ;

	lose_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void win_join_move( PLAYER_WORK *pl_w )
{
	pl_w->win_wait = 0xff ;
	pl_win_end_ck( pl_w ) ;
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_move( PLAYER_WORK *pl_w )
{
	static void ( *win_move_jmp[])( PLAYER_WORK *pl_w ) = {
		win0_move, win_cross_move, win_shield_move, win_time_move, lose_move, win_join_move,
	} ;

	win_move_jmp[pl_w->sub_type]( pl_w ) ;
}

static void sp_slash( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw );
static void sp_stars( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw );
static void sp_cartwheel( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw );
static void sp_charge( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw );
static void sp_chouhatsu( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw );
static void sp_hstars( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw );
static void sp_justice( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw );
static void sp_hcharge( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw );
static void sp_snapback( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw );

/****************************************************************/
void pl0b_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *pl_sp_tbl[])( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw ) = {
		sp_slash, sp_stars, sp_cartwheel, sp_charge, sp_chouhatsu, sp_hstars, sp_justice, sp_slash,
		sp_hcharge, sp_slash, sp_slash, sp_slash, sp_slash, sp_slash, sp_snapback,
	} ;

	pl_sp_tbl[pl_w->pl_sp_type]( pl_w, ( PL0B_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
/*			A special move starts standing on the ground		*/
/*==============================================================*/
static void sp_stand_set( PLAYER_WORK *pl_w )
{
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->sjump_flag = 0 ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
}

/*==============================================================*/
static void slash_atck_set( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 3 ){
		pl_w->atck_no = pl0b_at_slash_l ;
		HIT_RESET( pl_w ) ;
	} else {
		pl_w->atck_no = pl_w->atck_level + 68 ;
		HIT_RESET( pl_w ) ;
	}
	if( pl_w->pl_stance == STANCE_AIR ){
		pl_w->atck_no = pl_w->atck_level + 72 ;
		HIT_RESET( pl_w ) ;
	}
}

/*==============================================================*/
static void slash_init( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_gauge_add( pl_w, 5 ) ;
	slash_atck_set( pl_w ) ;
	if( pl_w->pl_stance == STANCE_AIR ){
		pl_dir_set( pl_w ) ;
		pl_w->spd.x = pl_w->spd.x / 8.0f ;
		pl_w->spd.y = pl_w->spd.y / 8.0f ;
		pl_w->acc.x = pl_w->acc.x / 8.0f ;
		pl_w->acc.y = pl_w->acc.y / 8.0f ;
		pl_w->char_no.b.no = pl_w->atck_level + 2 ;
	} else {
		pl_w->char_no.b.no = pl_w->atck_level ;
		sp_stand_set( pl_w ) ;
	}
	pl_w->char_no.b.grp = 21 ;
	pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
}

/*--------------------------------------------------------------*/
static void slash_move( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	Uint8 no ;

	if( pl_char_move( pl_w ) < 0 ){
		if( pl_w->pl_stance == STANCE_AIR ){
			pl_w->spd.y += 2.142857f ;
			pl_w->acc.y = -DOT_Y( 0.25f ) ;
			pl_jump_ret( pl_w ) ;
		} else {
			pl_stand_ret( pl_w ) ;
		}
		return ;
	}
	pl_brake_stop( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
	if( !lw->shield_st && pl_w->cg_prm ){
		pl_w->cg_prm = 0 ;
		if( pl_w->hit_flag == 0 ){
			if( pl_w->pl_stance == STANCE_AIR ){
				switch( pl_w->atck_level ){
				case 0 :
					no = 6 ;
					break ;
				case 1 :
					no = 4 ;
					break ;
				}
			} else {
				switch( pl_w->atck_level ){
				case 0 :
					no = 1 ;
					break ;
				case 1 :
					no = 2 ;
					break ;
				}
			}
			shield_req( lw, 1, no ) ;
		}
		pl_voice_req( pl_w, 30 ) ;
	}
}

/*==============================================================*/
static void sp_slash( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	static void ( *slash_jmp[])( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw ) = {
		slash_init, slash_move,
	} ;

	slash_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/*==============================================================*/
/*			A hit of a move that hits more than once			*/
/*==============================================================*/
static void sp_hit_set( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	Uint8 no ;

	if( lw->hit_cnt == 0 ) return ;
	no = pl_w->cg_type & 0x7f ;
	if( !( pl_w->cg_type & 0x7f ) ) return ;
	pl_w->cg_type = pl_w->cg_type & 0x80 ;
	if( lw->hit_cnt < 0 ) lw->hit_cnt = lw->hit_cnt & 0x7f ;
	if( pl_w->hit_flag < 0 ){
		lw->hit_cnt -- ;
		if( lw->hit_cnt <= 0 ){
			lw->hit_cnt = 0 ;
			return ;
		}
	}
	if( pl_w->team_type == 3 ){
		if( lw->shield_st == 0 ) pl_w->atck_no = no + 35 ;
		else pl_w->atck_no = no + 27 ;
	} else {
		pl_w->atck_no = no + 47 ;
	}
	HIT_RESET( pl_w ) ;
}

/*==============================================================*/
static void stars_rise_init( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	pl_w->routine_3 ++ ;
	pl_brake_set( pl_w ) ;
	lw->hit_cnt = -127 ;
	pl_w->char_no.b.grp = 21 ;
	pl_w->char_no.b.no = pl_w->atck_level + 5 ;
	pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
	sp_hit_set( pl_w, lw ) ;
}

static void stars_fall( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw );

/*--------------------------------------------------------------*/
static void stars_rise( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	Sint32 *tbl ;
	Sint32 x, ax ;
	Float ofs ;

	pl_char_move( pl_w ) ;
	sp_hit_set( pl_w, lw ) ;
	switch( pl_w->pl_stance ){
	case STANCE_STAND :
		if( pl_w->cg_type >= 0 ){
			pl_w->pos.x += pl_w->spd.x ;
			pl_w->spd.x += pl_w->acc.x ;
			pl_w->pos.y += pl_w->spd.y ;
			pl_w->spd.y += pl_w->acc.y ;
			pl_brake_stop( pl_w ) ;
			break ;
		}
		pl_air_set( pl_w ) ;
		ofs = 80.0f ;
		tbl = stars_rise_tbl[pl_w->atck_level] ;
		x = *tbl++ ;
		ax = *tbl++ ;
		if( pl_w->disp_dir ){
			pl_w->pos.x += ofs ;
		} else {
			pl_w->pos.x -= ofs ;
			x = -x ;
			ax = -ax ;
		}
		pl_w->spd.x = x * 1.6666666f / 65536.0f ;
		pl_w->acc.x = ax * 1.6666666f / 65536.0f ;
		pl_w->spd.y = *tbl++ * 2.142857f / 65536.0f ;
		pl_w->acc.y = *tbl * 2.142857f / 65536.0f ;
	case STANCE_AIR :
		if( pl_w->cg_type < 0 ){
			pl_w->pos.x += pl_w->spd.x ;
			pl_w->spd.x += pl_w->acc.x ;
			pl_w->pos.y += pl_w->spd.y ;
			pl_w->spd.y += pl_w->acc.y ;
			break ;
		}
		pl_w->routine_3 ++ ;
		tbl = stars_fall_tbl[pl_w->atck_level] ;
		x = *tbl++ ;
		ax = *tbl++ ;
		if( pl_w->disp_dir == 0 ){
			x = -x ;
			ax = -ax ;
		}
		pl_w->spd.x = x ;
		pl_w->acc.x = ax ;
		pl_w->spd.y += *tbl++ * 2.142857f / 65536.0f ;
		pl_w->acc.y = *tbl * 2.142857f / 65536.0f ;
		stars_fall( pl_w, lw ) ;
		break ;
	}
}

/*--------------------------------------------------------------*/
static void stars_fall( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_3 ++ ;
		pl_w->char_no.b.grp = 21 ;
		if( lw->shield_st ) pl_w->char_no.b.no = pl_w->atck_level + 7 ;
		else pl_w->char_no.b.no = pl_w->atck_level + 11 ;
		pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
		pl_land_ef_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void stars_land( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void stars_rise_init_shield( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	pl_w->routine_3 ++ ;
	pl_brake_set( pl_w ) ;
	lw->hit_cnt = -127 ;
	pl_w->char_no.b.grp = 21 ;
	pl_w->char_no.b.no = pl_w->atck_level + 9 ;
	pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
	sp_hit_set( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void stars_no_shield( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	static void ( *stars_no_shield_jmp[])( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw ) = {
		stars_rise_init, stars_rise, stars_fall, stars_land,
	} ;

	stars_no_shield_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void stars_shield( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	static void ( *stars_shield_jmp[])( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw ) = {
		stars_rise_init_shield, stars_rise, stars_fall, stars_land,
	} ;

	stars_shield_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*==============================================================*/
static void stars_init( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->atck_no = pl0b_at_stars ;
	HIT_RESET( pl_w ) ;
	sp_stand_set( pl_w ) ;
	if( lw->shield_st ){
		stars_no_shield( pl_w, lw ) ;
		return ;
	}
	pl_w->routine_2 ++ ;
	stars_shield( pl_w, lw ) ;
}

/*==============================================================*/
static void sp_stars( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	static void ( *stars_jmp[])( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw ) = {
		stars_init, stars_no_shield, stars_shield,
	} ;

	stars_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void cartwheel_move( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	PLAYER_WORK *em = pl_w->target_w ;

	if( pl_char_move( pl_w ) >= 0 ){
		pl_w->push_ng_timer = 1 ;
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		return ;
	}
	pl_w->catch_atck_ng_timer = 5 ;
	pl_w->pl_dir ^= 1 ;
	if( pl_w->wall_flag == em->wall_flag ){
		if( em->wall_flag == WALL_RIGHT ) em->pos.x = pl_w->pos.x + -1.0f ;
		if( em->wall_flag == WALL_LEFT ) em->pos.x = pl_w->pos.x + 1.0f ;
	}
	pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void cartwheel_init( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	Float spd ;

	pl_w->routine_2 ++ ;
	spd = DOT_X( 5.0f ) ;
	if( pl_w->disp_dir == 0 ) spd = -DOT_X( 5.0f ) ;
	pl_w->spd.x = spd ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_gauge_add( pl_w, 4 ) ;
	pl_w->char_no.b.grp = 21 ;
	pl_w->char_no.b.no = 4 ;
	pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
static void sp_cartwheel( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	static void ( *cartwheel_jmp[])( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw ) = {
		cartwheel_init, cartwheel_move,
	} ;

	pl_w->sp_ng_timer = 3 ;
	cartwheel_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

static void charge_no_shield( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw );

/*==============================================================*/
static void charge_init( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_gauge_add( pl_w, 10 ) ;
	sp_stand_set( pl_w ) ;
	if( shield_req( lw, 0, 1 ) == 0 ) pl_w->routine_2 ++ ;
	charge_no_shield( pl_w, lw ) ;
}

/*==============================================================*/
static void charge_start( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	pl_w->routine_3 ++ ;
	pl_brake_set( pl_w ) ;
	pl_w->char_no.b.grp = 21 ;
	pl_w->char_no.b.no = pl_w->atck_level + 13 ;
	pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
	if( pl_w->team_type == 3 ){
		pl_w->atck_no = pl0b_at_charge_h ;
		HIT_RESET( pl_w ) ;
	} else {
		pl_w->atck_no = pl_w->atck_level + 64 ;
		HIT_RESET( pl_w ) ;
	}
	pls0b_00_set( pl_w, 8, 0 ) ;
}

/*==============================================================*/
static void charge_dash_set( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	Float spd_0, spd_1, acc ;

	spd_0 = DOT_X( 10.0f ) ;
	spd_1 = DOT_X( 11.0f ) ;
	acc = -DOT_X( 0.125f ) ;
	if( pl_w->disp_dir == 0 ){
		spd_0 = -DOT_X( 10.0f ) ;
		spd_1 = -DOT_X( 11.0f ) ;
		acc = DOT_X( 0.125f ) ;
	}
	if( pl_w->atck_level ) pl_w->spd.x = spd_1 ;
	else pl_w->spd.x = spd_0 ;
	pl_w->acc.x = acc ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
}

/*==============================================================*/
static void charge_stop_set( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	Float spd, acc ;

	spd = 5.0f ;
	acc = -0.104166664f ;
	if( pl_w->disp_dir == 0 ){
		spd = -5.0f ;
		acc = 0.104166664f ;
	}
	pl_w->spd.x = spd ;
	pl_w->acc.x = acc ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
}

static void charge_dash( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw );

/*--------------------------------------------------------------*/
static void charge_wait( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type == 0 ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		pl_brake_stop( pl_w ) ;
		return ;
	}
	pl_w->routine_3 ++ ;
	charge_dash_set( pl_w, lw ) ;
	pls0b_00_set( pl_w, 4, 0 ) ;
	pls0b_00_set( pl_w, 4, 1 ) ;
	pls0b_00_set( pl_w, 4, 2 ) ;
	if( pl_w->team_type == 3 ){
		pl_w->atck_no = pl0b_at_charge_dash_h ;
		HIT_RESET( pl_w ) ;
	} else {
		pl_w->atck_no = pl_w->atck_level + 66 ;
		HIT_RESET( pl_w ) ;
	}
	charge_dash( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void charge_dash( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		return ;
	}
	pl_w->routine_3 ++ ;
	lw->guard_flag = 0 ;
	charge_stop_set( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void charge_dash_guard( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	lw->guard_flag = 1 ;
	charge_dash( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void charge_stop( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
}

/*--------------------------------------------------------------*/
static void charge_no_shield( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	static void ( *charge_no_shield_jmp[])( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw ) = {
		charge_start, charge_wait, charge_dash, charge_stop,
	} ;

	charge_no_shield_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void charge_shield( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	static void ( *charge_shield_jmp[])( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw ) = {
		charge_start, charge_wait, charge_dash_guard, charge_stop,
	} ;

	charge_shield_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*==============================================================*/
static void sp_charge( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	static void ( *charge_jmp[])( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw ) = {
		charge_init, charge_no_shield, charge_shield,
	} ;

	charge_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/*==============================================================*/
static void chouhatsu_init( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->disp_dir &= 0xfe ;
	pl_w->char_no.b.grp = 20 ;
	pl_w->char_no.b.no = pl_w->pl_dir == 0 ? 3 : 4 ;
	pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
	pl_voice_req( pl_w, 33 ) ;
}

/*--------------------------------------------------------------*/
static void chouhatsu_turn( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->disp_dir = pl_w->pl_dir ;
		pl_w->char_no.b.grp = 20 ;
		pl_w->char_no.b.no = 5 ;
		pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
	}
}

/*--------------------------------------------------------------*/
static void chouhatsu_end( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	static void ( *chouhatsu_jmp[])( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw ) = {
		chouhatsu_init, chouhatsu_turn, chouhatsu_end,
	} ;

	chouhatsu_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/*==============================================================*/
static void hstars_spd_set( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	Float spd, acc ;

	spd = DOT_X( 11.0f ) ;
	acc = -DOT_X( 0.5f ) ;
	if( pl_w->disp_dir == 0 ){
		spd = -DOT_X( 11.0f ) ;
		acc = DOT_X( 0.5f ) ;
	}
	pl_w->spd.x = spd ;
	pl_w->acc.x = acc ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
}

/*--------------------------------------------------------------*/
static void hstars_wait( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->atck_no = pl0b_at_hstars ;
		HIT_RESET( pl_w ) ;
		ofs.x = DOT_X( 16.0f ) ;
		ofs.y = DOT_Y( 48.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*==============================================================*/
static void hstars_init( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_3 ++ ;
	sp_stand_set( pl_w ) ;
	pl_w->char_no.b.grp = 22 ;
	pl_w->char_no.b.no = 13 ;
	pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
	hstars_wait( pl_w, lw ) ;
}

/*==============================================================*/
static void hstars_hit_set( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	Uint8 no ;

	no = pl_w->cg_type ;
	no &= 0x0f ;
	if( !no ) return ;
	pl_w->cg_type &= 0xf0 ;
	pl_w->atck_no = no + 79 ;
	HIT_RESET( pl_w ) ;
}

/*==============================================================*/
static void hstars_start( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->routine_3 ++ ;
	lw->hit_cnt = -121 ;
	pl_w->char_no.b.grp = 22 ;
	pl_w->char_no.b.no = 0 ;
	pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
	sp_hit_set( pl_w, lw ) ;
	hstars_spd_set( pl_w, lw ) ;
	pls0b_00_set( pl_w, 6, 0 ) ;
}

/*--------------------------------------------------------------*/
static void hstars_rise( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	Float spd, x ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	hstars_hit_set( pl_w, lw ) ;
	switch( pl_w->cg_prm ){
	case 0 :
		return ;
	case 1 :
		pl_w->cg_prm ++ ;
		hstars_spd_set( pl_w, lw ) ;
	case 2 :
		spd = pl_w->spd.x ;
		spd += pl_w->acc.x ;
		if( pl_w->disp_dir == 0 ){
			if( spd > 0.0f ) pl_w->acc.x = spd = 0.0f ;
		} else {
			if( spd < 0.0f ) pl_w->acc.x = spd = 0.0f ;
		}
		pl_w->spd.x = spd ;
		pl_w->pos.x += pl_w->spd.x ;
		break ;
	case 3 :
		pl_w->routine_3 ++ ;
		pl_w->cg_prm = 0 ;
		pl_w->pl_stance = STANCE_AIR ;
		x = 80.0f ;
		spd = DOT_X( 4.0f ) ;
		if( pl_w->pl_dir == 0 ){
			x = -80.0f ;
			spd = -DOT_X( 4.0f ) ;
		}
		pl_w->pos.x = pl_w->pos.x + x ;
		pl_w->spd.x = spd ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 18.0f ) ;
		pl_w->acc.y = DOT_Y( 0.5f ) ;
		break ;
	}
}

/*--------------------------------------------------------------*/
static void hstars_fly( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	Sint32 y ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	hstars_hit_set( pl_w, lw ) ;
	if( pl_w->cg_prm != 3 ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		y = pl_w->ground_y ;
		if( pl_w->pos.y > y ) return ;
		pl_w->pos.y = y ;
	}
	pl_w->delay_flag = 0 ;
	pl_w->delay_timer = 0 ;
	pl_w->hyper_bg = 0 ;
	pl_w->hyper_bg_tm = 0 ;
	pl_w->routine_3 ++ ;
	pl_w->spd.x = pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 4.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.5f ) ;
}

/*--------------------------------------------------------------*/
static void hstars_fall( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_3 ++ ;
		pl_w->char_no.b.grp = 22 ;
		pl_w->char_no.b.no = 1 ;
		pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
		pl_land_ef_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void hstars_land( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void hstars_ready( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	static void ( *hstars_ready_jmp[])( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw ) = {
		hstars_init, hstars_wait,
	} ;

	hstars_ready_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void hstars_move( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	static void ( *hstars_move_jmp[])( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw ) = {
		hstars_start, hstars_rise, hstars_fly, hstars_fall, hstars_land,
	} ;

	hstars_move_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*==============================================================*/
static void sp_hstars( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	static void ( *hstars_jmp[])( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw ) = {
		hstars_ready, hstars_move,
	} ;

	hstars_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void justice_wait( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = 40.0f ;
		ofs.y = DOT_Y( 93.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*==============================================================*/
static void justice_init( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_3 ++ ;
	lw->unk_13 = 0 ;
	sp_stand_set( pl_w ) ;
	pl_w->char_no.b.grp = 22 ;
	pl_w->char_no.b.no = 2 ;
	pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = pl0b_at_justice ;
	HIT_RESET( pl_w ) ;
	justice_wait( pl_w, lw ) ;
}

/*==============================================================*/
static void justice_dash_init( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	Float spd ;

	pl_w->routine_3 ++ ;
	spd = DOT_X( 10.0f ) ;
	if( pl_w->disp_dir == 0 ) spd = -DOT_X( 10.0f ) ;
	pl_w->spd.x = spd ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 1.5f ) ;
	pl_w->acc.y = -0.3348214f ;
	pl_w->char_no.b.grp = 22 ;
	pl_w->char_no.b.no = 3 ;
	pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
static void justice_miss( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	Float spd ;

	pl_w->delay_flag = 0 ;
	pl_w->delay_timer = 0 ;
	pl_w->hyper_bg = 0 ;
	pl_w->hyper_bg_tm = 0 ;
	pl_w->routine_3 ++ ;
	spd = DOT_X( 2.5f ) ;
	if( pl_w->disp_dir == 0 ) spd = -DOT_X( 2.5f ) ;
	pl_w->spd.x = spd ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->char_no.b.grp = 22 ;
	pl_w->char_no.b.no = 4 ;
	pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
/*			The dash reached the opponent: catch them			*/
/*==============================================================*/
static void justice_dash( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	pl_w->pos.y = pl_w->ground_y ;
	em = pl_w->em_w ;
	if( pl_sp_catch_ck( pl_w ) == 0 || em->dm_no != 2 ){
		justice_miss( pl_w, lw ) ;
		return ;
	}
	lw->em_w = em ;
	pl_w->routine_2 ++ ;
	pl_w->routine_3 = 0 ;
	pl_w->catch_no = 0xc5 ;
	pl_sp_catch_set( pl_w, em ) ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	em->dm_hit_w = pl_w ;
	em->disp_dir = pl_w->disp_dir ;
	em->catch_type = 17 ;
	em->pl_stance = STANCE_AIR ;
	em->atck_no = pl0b_at_justice ;
}

/*--------------------------------------------------------------*/
static void justice_dash_move( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	Sint8 hit = pl_w->hit_flag ;

	if( hit < 0 ){
		hit &= 0x7f ;
		if( hit == 0 ){
			justice_dash( pl_w, lw ) ;
			return ;
		}
		pl_w->pos.y = pl_w->ground_y ;
		justice_miss( pl_w, lw ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ) return ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_land_ck( pl_w ) ) justice_miss( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void justice_miss_move( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			The one he caught comes down with him				*/
/*==============================================================*/
static void justice_strike( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw );

static void justice_em_land( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	PLAYER_WORK *em = lw->em_w ;
	Float spd, acc, y ;

	if( em->spd.y > 0.0f ) return ;
	spd = em->spd.y ;
	acc = 2.142857f ;
	if( DOT_Y( 15.0f ) > spd ) spd = DOT_Y( 15.0f ) ;
	if( em->acc.y > acc ) acc = em->acc.y ;
	y = spd + acc ;
	y += y ;
	y += em->ground_y ;
	if( y > em->pos.y ){
		em->pos.y = y ;
		em->spd.x = 0.0f ;
		em->spd.y = 0.0f ;
		em->acc.x = 0.0f ;
		em->acc.y = 0.0f ;
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		/* from the European disc on, the next step starts on the frame they land */
#if BUILD_DATE >= 20000412
		justice_strike( pl_w, lw ) ;
#endif
	}
}

/*--------------------------------------------------------------*/
static void justice_fall( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ) pl_w->routine_3 ++ ;
	justice_em_land( pl_w, lw ) ;
}

/*==============================================================*/
static void justice_throw_init( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	pl_w->routine_3 ++ ;
	pl_w->disp_dir = pl_w->disp_dir ^ 1 ;
	pl_w->pl_dir = pl_w->pl_dir ^ 1 ;
	pl_w->spd.x = pl_w->disp_dir ? -10.0f : 10.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	em = lw->em_w ;
	em->ukemi_flag = 0 ;
	pl_char_set( pl_w, Char_Super, 4 ) ;
}

/*--------------------------------------------------------------*/
static void justice_throw( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		return ;
	}
	if( pl_w->cg_prm ){
		em = lw->em_w ;
		em->dm_hit_w = pl_w ;
		pl_w->atck_no = pl_w->cg_prm + 76 ;
		em->atck_no = pl_w->cg_prm + 76 ;
		pl_w->cg_prm = 0 ;
		catch_damage( pl_w ) ;
		ofs.x = -80.0f ;
		ofs.y = DOT_Y( 64.0f ) ;
		catch_hit_mark_set( pl_w, &ofs ) ;
		combo_hit( lw->em_w, pl_w ) ;
		if( pl_w->cg_flag1 ){
			hit_se_req( pl_w->catch_w, pl_w->cg_flag1 + 0xff ) ;
			pl_w->cg_flag1 = 0 ;
		}
	}
}

/*==============================================================*/
static void justice_hit_init( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	pl_w->routine_3 ++ ;
	pl_char_set( pl_w, Char_Super, 5 ) ;
	pl_w->catch_no = 0xc5 ;
	pl_ef_catch_set( pl_w, lw->em_w ) ;
}

/*--------------------------------------------------------------*/
static void justice_end( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void justice_land( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_3 ++ ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 8 ) ;
	}
}

/*==============================================================*/
/*			He lets go and jumps away							*/
/*==============================================================*/
static void justice_let_go( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	if( pl_w->timer0 -- == 0 ){
		pl_w->routine_3 ++ ;
		pl_w->cg_type = 0 ;
		SCR_EXCEP_SET ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->atck_no = pl0b_at_justice_toss ;
		em->pl_stance = STANCE_AIR ;
		em->catch_type = 1 ;
		em->pl_dir = pl_w->pl_dir ;
		pl_w->pl_dir = pl_w->pl_dir ^ 1 ;
		pl_w->spd.x = pl_w->disp_dir ? -DOT_X( 8.0f ) : DOT_X( 8.0f ) ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 8.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.5f ) ;
		pl_char_set( pl_w, Char_Super, 7 ) ;
	}
}

/*--------------------------------------------------------------*/
static void justice_drop( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type == 0 ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
	if( pl_w->pos.y > pl_w->ground_y ){
		if( pl_w->type == 0 && pl_w->pos.y <= pl_w->ground_y + DOT_Y( 128.0f ) ){
			pl_w->type ++ ;
			pl_voice_req( pl_w, 5 ) ;
		}
		return ;
	}
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->routine_3 ++ ;
	pl_w->timer0 = 16 ;
	em = pl_w->catch_w ;
	em->dm_hit_w = pl_w ;
	em->atck_no = pl0b_at_justice_slam ;
	catch_damage( pl_w ) ;
	combo_hit( em, pl_w ) ;
	scr_work.quake_level = 3 ;
	scr_work.quake_flag = 1 ;
	ofs.x = em->pos.x ;
	ofs.y = em->ground_y ;
	burst_pos_set( &ofs, 1 ) ;
	pl_se_req( pl_w, 74 ) ;
	catch_se_req( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void justice_drop_init( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_w->routine_3 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = -DOT_Y( 4.0f ) ;
	pl_w->acc.y = -2.142857f ;
	pl_w->type = 0 ;
	ofs.x = -0.0f ;
	ofs.y = DOT_Y( 27.0f ) ;
	hit_mark_set( pl_w, &ofs, 0 ) ;
	pl_se_req( pl_w, 35 ) ;
	justice_drop( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void justice_hit_wait( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	PLAYER_WORK *em ;
	Float dx ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type < 0 ) return ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_land_ck( pl_w ) ){
		SCR_EXCEP_SET ;
		pl_stand_ret( pl_w ) ;
		return ;
	}
	em = pl_w->catch_w ;
	if( em->pl_stance != STANCE_AIR ) return ;
	dx = em->pos.x - pl_w->pos.x ;
	if( 0.0f > dx ) dx = -dx ;
	if( dx > DOT_X( 64.0f ) ) return ;
	if( 0.0f > em->pos.y - pl_w->pos.y + DOT_Y( 32.0f ) ){
		pl_char_set( pl_w, Char_Catch, 4 ) ;
		pl_w->catch_no = 0xc4 ;
		pl_ef_catch_set( pl_w, em ) ;
		combo_hit( em, pl_w ) ;
		scr_excep_set( pl_w, 5, 5 ) ;
		em->ukemi_flag = 0 ;
		pl_w->routine_3 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void justice_hop_init( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	pl_w->routine_3 ++ ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 24.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_char_set( pl_w, Char_Super, 6 ) ;
}

/* how far each blow of the rush carries him */
static Sint16 justice_hit_ofs[6] = { 0, 0x600, 0x500, 0x400, 0x200, 0 } ;

/*--------------------------------------------------------------*/
static void justice_hits( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	PLAYER_WORK *em ;
	Sint8 n ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		justice_hop_init( pl_w, lw ) ;
		return ;
	}
	if( pl_w->cg_prm ){
		pl_w->cg_prm = 0 ;
		if( pl_w->disp_dir ) pl_w->pos.x += DOT_X( 32.0f ) ;
		else pl_w->pos.x -= DOT_X( 32.0f ) ;
	}
	n = pl_w->cg_type & 0x7f ;
	if( n ){
		if( pl_w->disp_dir ) pl_w->pos.x += justice_hit_ofs[n] * 1.6666666f / 256.0f ;
		else pl_w->pos.x -= justice_hit_ofs[n] * 1.6666666f / 256.0f ;
	}
	if( pl_w->cg_type < 0 ){
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->atck_no = pl0b_at_justice_hits ;
		em->pl_stance = STANCE_AIR ;
		em->catch_type = 17 ;
		em->pl_dir = pl_w->pl_dir ;
		pl_w->pl_dir = pl_w->pl_dir ^ 1 ;
	}
}

/*--------------------------------------------------------------*/
static void justice_hits_init( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_w->routine_3 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	ofs.x = -DOT_X( 46.0f ) ;
	ofs.y = DOT_Y( 93.0f ) ;
	hit_mark_set( pl_w, &ofs, 0 ) ;
	pl_se_req( pl_w, 35 ) ;
	justice_hits( pl_w, lw ) ;
}

/*==============================================================*/
static void justice_land_ck( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	PLAYER_WORK *em = lw->em_w ;

	if( em->vital_new == 0 ){
		em->spd.y = -DOT_Y( 0.375f ) ;
		pl_stand_ret( pl_w ) ;
		return ;
	}
	pl_w->routine_3 ++ ;
	pl_char_set( pl_w, Char_Catch, 3 ) ;
	pl_w->catch_no = 0xc3 ;
	pl_ef_catch_set( pl_w, em ) ;
	combo_hit( em, pl_w ) ;
	scr_excep_set( pl_w, 5, 5 ) ;
	em->ukemi_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void justice_down_jump( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type >= 0 ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_land_ef_set( pl_w ) ;
		pl_land_set( pl_w ) ;
	}
}

/*==============================================================*/
static void justice_down_init( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	pl_w->routine_3 ++ ;
	pl_w->spd.y = DOT_Y( 24.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_char_set( pl_w, Char_Super, 6 ) ;
}

/*==============================================================*/
static void justice_ready( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	static void ( *justice_ready_jmp[])( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw ) = {
		justice_init, justice_wait,
	} ;

	justice_ready_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void justice_rush( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	static void ( *justice_rush_jmp[])( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw ) = {
		justice_dash_init, justice_dash_move, justice_miss_move,
	} ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	justice_rush_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void justice_carry( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	static void ( *justice_carry_jmp[])( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw ) = {
		justice_throw_init, justice_fall, justice_em_land,
	} ;

	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	justice_carry_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void justice_strike( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	static void ( *justice_strike_jmp[])( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw ) = {
		justice_hit_init, justice_throw,
	} ;

	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	justice_strike_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void justice_finish( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	static void ( *justice_finish_jmp[])( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw ) = {
		justice_land_ck, justice_hits_init, justice_hits, justice_hop_init, justice_hit_wait,
		justice_drop_init, justice_drop, justice_let_go, justice_land, justice_end,
	} ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	justice_finish_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void justice_down( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	static void ( *justice_down_jmp[])( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw ) = {
		justice_down_init, justice_down_jump,
	} ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	justice_down_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*==============================================================*/
static void sp_justice( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	static void ( *justice_jmp[])( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw ) = {
		justice_ready, justice_rush, justice_carry, justice_strike, justice_finish, justice_down,
	} ;

	justice_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/*==============================================================*/
static void hcharge_atck_set( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	Uint8 no = pl_w->cg_prm + 81 ;

	pl_w->atck_no = no ;
	HIT_RESET( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void hcharge_wait( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		if( shield_req( lw, 0, 1 ) == 0 ) pl_w->routine_2 ++ ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = -DOT_X( 16.0f ) ;
		ofs.y = DOT_Y( 72.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*==============================================================*/
static void hcharge_init( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_3 ++ ;
	sp_stand_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->char_no.b.grp = 22 ;
	pl_w->char_no.b.no = 12 ;
	pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
	pl_w->atck_no = pl0b_at_hcharge ;
	HIT_RESET( pl_w ) ;
	hcharge_wait( pl_w, lw ) ;
}

/*==============================================================*/
static void hcharge_start( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	pl_w->routine_3 ++ ;
	lw->dash_hit_flag = 0 ;
	pl_brake_set( pl_w ) ;
	pl_w->char_no.b.grp = 22 ;
	pl_w->char_no.b.no = 9 ;
	pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
	lw->hit_cnt = 8 ;
	pl_w->atck_no = pl0b_at_hcharge ;
	HIT_RESET( pl_w ) ;
	pls0b_00_set3( pl_w, 9, 0 ) ;
	pls0b_00_set3( pl_w, 9, 2 ) ;
}

/*--------------------------------------------------------------*/
static void hcharge_charge( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type == 0 ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		pl_brake_stop( pl_w ) ;
		return ;
	}
	pl_w->timer0 = 32 ;
	pl_w->routine_3 ++ ;
	charge_dash_set( pl_w, lw ) ;
	hcharge_atck_set( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void hcharge_dash( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 >= 0 ){
		if( !( pl_w->hit_flag & HR_HIT ) ){
			hcharge_atck_set( pl_w, lw ) ;
			pl_w->pos.x += pl_w->spd.x ;
			pl_w->spd.x += pl_w->acc.x ;
			pl_w->pos.y += pl_w->spd.y ;
			pl_w->spd.y += pl_w->acc.y ;
			return ;
		}
		lw->dash_hit_flag = -1 ;
		if( -- lw->hit_cnt > 0 ){
			hcharge_atck_set( pl_w, lw ) ;
			pl_w->pos.x += pl_w->spd.x ;
			pl_w->spd.x += pl_w->acc.x ;
			pl_w->pos.y += pl_w->spd.y ;
			pl_w->spd.y += pl_w->acc.y ;
			return ;
		}
	}
	if( lw->dash_hit_flag == 0 ){
		pl_w->routine_3 ++ ;
		pl_w->routine_3 ++ ;
		pl_w->char_no.b.grp = 22 ;
		pl_w->char_no.b.no = 11 ;
		pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		return ;
	}
	pl_w->routine_3 ++ ;
	pl_w->char_no.b.grp = 22 ;
	pl_w->char_no.b.no = 10 ;
	pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
	hcharge_atck_set( pl_w, lw ) ;
	pl_w->cg_prm = 0 ;
	charge_stop_set( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void hcharge_stop( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) >= 0 ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		return ;
	}
	pl_w->delay_flag = 0 ;
	pl_w->delay_timer = 0 ;
	pl_w->hyper_bg = 0 ;
	pl_w->hyper_bg_tm = 0 ;
	pl_w->routine_3 ++ ;
	pl_w->char_no.b.grp = 22 ;
	pl_w->char_no.b.no = 11 ;
	pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	charge_stop_set( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void hcharge_end( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type == 0 ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
}

/*--------------------------------------------------------------*/
static void hcharge_dash_guard( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	lw->guard_flag = -1 ;
	hcharge_dash( pl_w, lw ) ;
}

/*==============================================================*/
static void hcharge_ready( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	static void ( *hcharge_ready_jmp[])( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw ) = {
		hcharge_init, hcharge_wait,
	} ;

	hcharge_ready_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void hcharge_no_shield( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	static void ( *hcharge_no_shield_jmp[])( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw ) = {
		hcharge_start, hcharge_charge, hcharge_dash, hcharge_stop, hcharge_end,
	} ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	hcharge_no_shield_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void hcharge_shield( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	static void ( *hcharge_shield_jmp[])( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw ) = {
		hcharge_start, hcharge_charge, hcharge_dash_guard, hcharge_stop, hcharge_end,
	} ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	hcharge_shield_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*==============================================================*/
static void sp_hcharge( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	static void ( *hcharge_jmp[])( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw ) = {
		hcharge_ready, hcharge_no_shield, hcharge_shield,
	} ;

	hcharge_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

static void snapback_init( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw );
static void snapback_move( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw );

/*==============================================================*/
static void sp_snapback( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	static void ( *snapback_jmp[])( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw ) = {
		snapback_init, snapback_move,
	} ;

	snapback_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/*==============================================================*/
static void snapback_init( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
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
	pl_w->atck_no = pl0b_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 15 ) ;
}

/*--------------------------------------------------------------*/
static void snapback_move( PLAYER_WORK *pl_w, PL0B_LOCAL_W *lw )
{
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		ofs.x = DOT_X( 16.0f ) ;
		ofs.y = DOT_Y( 48.0f ) ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
	if( pl_w->cg_flag1 ){
		pl_w->cg_flag1 = 0 ;
		pl_se_req( pl_w, SE_SWING_H ) ;
	}
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_move( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A jump straight after a throw is escaped			*/
/****************************************************************/
void pl0b_t_kick( PLAYER_WORK *pl_w )
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
	pl_w->spd.y = DOT_Y( 0.125f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->atck_no = pl0b_at_t_kick ;
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
		pl_char_set( pl_w, Char_Extra, 2 ) ;
		pl_land_ef_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void t_kick_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_stop_stand_ret( pl_w ) ;
	}
}

/****************************************************************/
/*			Can a throw start?									*/
/****************************************************************/
PLAYER_WORK *pl0b_catch_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( !( pl_w->atck_lever & ( KEY_FORWARD | KEY_BACK ) ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_CROUCH ) return 0 ;
	if( !pl_w->atck_level ) return 0 ;
	if( pl_w->atck_pk ){
		if( pl_w->pl_stance == STANCE_AIR ) return 0 ;
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 1 ;
			return em ;
		}
		return 0 ;
	}
	if( pl_w->pl_stance == STANCE_AIR ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 2 ;
			return em ;
		}
		return 0 ;
	}
	if( ( em = pl_catch_ck( pl_w ) ) ){
		pl_w->catch_no = 0 ;
		return em ;
	}
	return 0 ;
}

static void nml_p_set( PLAYER_WORK *pl_w );
static void nml_k_set( PLAYER_WORK *pl_w );
static void jump_p_set( PLAYER_WORK *pl_w );
static void catch_dummy( PLAYER_WORK *pl_w );
static void catch_dummy_air( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Start the throw that pl0b_catch_ck chose			*/
/****************************************************************/
void pl0b_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_set, nml_k_set, jump_p_set, catch_dummy, catch_dummy_air,
	} ;

	catch_set_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_w->atck_lever & KEY_FORWARD ){
		pl_w->pl_dir = pl_w->pl_dir ^ 1 ;
		pl_w->disp_dir = pl_w->disp_dir ^ 1 ;
	}
	scr_excep_set( pl_w, 5, 5 ) ;
	pl_w->hit_stop = 10 ;
	pl_w->routine_2 = 0 ;
	ofs.x = -DOT_X( 32.0f ) ;
	ofs.y = DOT_Y( 64.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 0 ) ;
}

/*==============================================================*/
static void nml_k_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_w->atck_lever & KEY_FORWARD ){
		pl_w->pl_dir = pl_w->pl_dir ^ 1 ;
		pl_w->disp_dir = pl_w->disp_dir ^ 1 ;
	}
	scr_excep_set( pl_w, 5, 5 ) ;
	pl_w->hit_stop = 10 ;
	pl_w->routine_2 = 0 ;
	ofs.x = -DOT_X( 64.0f ) ;
	ofs.y = DOT_Y( 64.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 1 ) ;
}

/*==============================================================*/
static void jump_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_w->atck_lever & KEY_FORWARD ){
		pl_w->pl_dir = pl_w->pl_dir ^ 1 ;
		pl_w->disp_dir = pl_w->disp_dir ^ 1 ;
	}
	scr_excep_set( pl_w, 5, 5 ) ;
	pl_w->hit_stop = 10 ;
	pl_w->routine_2 = 0 ;
	ofs.x = -80.0f ;
	ofs.y = DOT_Y( 64.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 2 ) ;
}

/*==============================================================*/
static void catch_dummy( PLAYER_WORK *pl_w )
{
}

/*==============================================================*/
static void catch_dummy_air( PLAYER_WORK *pl_w )
{
}

static void nml_p_catch( PLAYER_WORK *pl_w );
static void nml_k_catch( PLAYER_WORK *pl_w );
static void jump_p_catch( PLAYER_WORK *pl_w );
static void catch_cancel( PLAYER_WORK *pl_w );
static void catch_cancel_air( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void pl0b_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_catch, nml_k_catch, jump_p_catch, catch_cancel, catch_cancel_air,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void nml_p_throw( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->disp_dir = pl_w->disp_dir ^ 1 ;
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		scr_excep_set( pl_w, 0, 0 ) ;
		em_w = pl_w->catch_w ;
		em_w->dm_hit_w = pl_w ;
		em_w->atck_no = pl0b_at_throw_p ;
		em_w->pl_stance = STANCE_AIR ;
		em_w->catch_type = 11 ;
		em_w->pl_dir = pl_w->pl_dir ;
		pl_w->pl_dir = pl_w->pl_dir ^ 1 ;
		catch_se_req( pl_w ) ;
	}
}

/*==============================================================*/
static void nml_p_catch_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	nml_p_throw( pl_w ) ;
}

/*==============================================================*/
/*			The punch throw on the ground						*/
/*==============================================================*/
static void nml_p_catch( PLAYER_WORK *pl_w )
{
	static void ( *nml_p_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_catch_init, nml_p_throw,
	} ;

	nml_p_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void nml_k_throw( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->disp_dir = pl_w->disp_dir ^ 1 ;
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		scr_excep_set( pl_w, 0, 0 ) ;
		em_w = pl_w->catch_w ;
		em_w->dm_hit_w = pl_w ;
		em_w->atck_no = pl0b_at_throw_k ;
		em_w->pl_stance = STANCE_AIR ;
		em_w->catch_type = 1 ;
		em_w->pl_dir = pl_w->pl_dir ;
		pl_w->pl_dir = pl_w->pl_dir ^ 1 ;
	}
}

/*==============================================================*/
static void nml_k_catch_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	nml_k_throw( pl_w ) ;
}

/*==============================================================*/
/*			The kick throw on the ground						*/
/*==============================================================*/
static void nml_k_catch( PLAYER_WORK *pl_w )
{
	static void ( *nml_k_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_k_catch_init, nml_k_throw,
	} ;

	nml_k_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void jump_p_throw( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w ;
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->disp_dir = pl_w->disp_dir ^ 1 ;
		pl_jump_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		scr_excep_set( pl_w, 0, 0 ) ;
		em_w = pl_w->catch_w ;
		em_w->dm_hit_w = pl_w ;
		em_w->atck_no = pl0b_at_air_throw ;
		em_w->pl_stance = STANCE_AIR ;
		em_w->catch_type = 1 ;
		em_w->pl_dir = pl_w->pl_dir ;
		pl_w->pl_dir = pl_w->pl_dir ^ 1 ;
		ofs.x = 0.0f ;
		ofs.y = DOT_Y( 93.0f ) ;
		catch_hit_mark_set( pl_w, &ofs ) ;
		pl_se_req( pl_w, 1 ) ;
	}
}

/*==============================================================*/
static void jump_p_catch_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	jump_p_throw( pl_w ) ;
}

/*==============================================================*/
/*			The throw in the air								*/
/*==============================================================*/
static void jump_p_catch( PLAYER_WORK *pl_w )
{
	static void ( *jump_p_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		jump_p_catch_init, jump_p_throw,
	} ;

	jump_p_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void catch_cancel( PLAYER_WORK *pl_w )
{
	pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void catch_cancel_air( PLAYER_WORK *pl_w )
{
	pl_stand_ret( pl_w ) ;
}

/****************************************************************/
/*			The opponent's side of a throw						*/
/****************************************************************/
void pl0b_hold( PLAYER_WORK *pl_w )
{
	catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/****************************************************************/
void pl0b_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl0b_sp_hcharge ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl0b_sp_hstars ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl0b_sp_hcharge ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl0b_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl0b_sp_hcharge ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl0b_sp_hstars ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl0b_sp_hcharge ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl0b_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl0b_sp_slash ;
		pl_w->atck_level = 0 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl0b_sp_stars ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl0b_sp_charge ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void pl0b_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl0b_sp_slash ;
		pl_w->atck_level = 0 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl0b_sp_stars ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl0b_sp_charge ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
/*			While the shield is out and coming back, a pose		*/
/*			looking up or down at it							*/
/****************************************************************/
int pl0b_shield_look( PLAYER_WORK *pl_w )
{
	PL0B_LOCAL_W *lw = ( PL0B_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *sw ;

	if( pl_w->pl_type != PL_CAPTAIN_AMERICA ) return 0 ;
	if( lw->shield_st ) return 0 ;
	if( !lw->shield_type ) return 0 ;
	if( !lw->shield_w ) return 0 ;
	sw = lw->shield_w ;
	if( pl_w->pos.y + DOT_Y( 48.0f ) > sw->pos.y ) pl_char_set( pl_w, Char_Extra, 0 ) ;
	else pl_char_set( pl_w, Char_Extra, 1 ) ;
	return 1 ;
}

/****************************************************************/
/*			Whether the shield takes a hit, as the armour		*/
/*			does in s_pl22										*/
/****************************************************************/
int pl0b_shield_guard_ck( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	PL0B_LOCAL_W *lw = ( PL0B_LOCAL_W * )pl_w->tlocal ;
	Float spd ;
	Sint16 n ;

	if( pl_w->pl_type != PL_CAPTAIN_AMERICA || lw->shield_st || !( em->atck_flag & HIT_KIND_MASK ) || !lw->guard_flag ) return 0 ;
	if( lw->guard_flag > 0 ){
		if( em->hit_data_adrs->atck_kind & A_HYPER ) return 0 ;
		n = 4 ;
	} else n = 2 ;
	if( pl_w->spd.x < 0.0f ) spd = -pl_w->spd.x ;
	else spd = pl_w->spd.x ;
	if( spd > 5.0f ) pl_w->spd.x /= n ;
	else {
		pl_w->spd.x = pl_w->pl_dir ? 5.0f : -5.0f ;
		pl_w->acc.x = 0.0f ;
	}
	return 1 ;
}

/****************************************************************/
/*			Clear the work, all but the shield					*/
/****************************************************************/
void pl0b_tlocal_init( PLAYER_WORK *pl_w )
{
	PL0B_LOCAL_W *lw = ( PL0B_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *shield = lw->shield_w ;
	Uint8 keep[2] ;
	Uint8 keep_10 ;

	keep[0] = lw->shield_st ;
	keep[1] = lw->shield_sub ;
	keep_10 = lw->shield_type ;
	memset( pl_w->tlocal, 0, sizeof( pl_w->tlocal ) ) ;
	lw->shield_w = shield ;
	lw->shield_st = keep[0] ;
	lw->shield_sub = keep[1] ;
	lw->shield_type = keep_10 ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl0b.c" End of File	*/

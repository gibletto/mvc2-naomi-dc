/* Charlie (character 21): command checks, normal attacks, appearance, win and lose poses, special
   moves and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl21_def.h"

/* speeds for a jump that starts straight up */
typedef struct SMR_SPD {
	Float	spd_x, spd_y, acc_y;
} SMR_SPD;

/* speeds for a jump */
typedef struct JUMP_SPD {
	Float	spd_x, acc_x, spd_y, acc_y;
} JUMP_SPD;

void pl21_main( PLAYER_WORK *pl_w );
void pl21_com_ck( PLAYER_WORK *pl_w );
void pl21_atck_set( PLAYER_WORK *pl_w );
void pl21_atck( PLAYER_WORK *pl_w );
void pl21_dash( PLAYER_WORK *pl_w );
void pl21_noki( PLAYER_WORK *pl_w );
void pl21_appear( PLAYER_WORK *pl_w );
void pl21_win( PLAYER_WORK *pl_w );
void pl21_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl21_catch_ck( PLAYER_WORK *pl_w );
void pl21_catch_set( PLAYER_WORK *pl_w );
void pl21_catch( PLAYER_WORK *pl_w );
void pl21_hold( PLAYER_WORK *pl_w );
int pl21_delay_ck( PLAYER_WORK *pl_w );
void pl21_t_kick( PLAYER_WORK *pl_w );
void pl21_upper( PLAYER_WORK *pl_w );
void pl21_sp_unison( PLAYER_WORK *pl_w );
void pl21_sp_gattai( PLAYER_WORK *pl_w );
void pl21_sp_assist( PLAYER_WORK *pl_w );
void pl21_sp_zero( PLAYER_WORK *pl_w );

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
static Uint16 com_sonic[] = { 40, COM_GROUND, pl21_sp_sonic, 0, KEY_BACK, KEY_FORWARD, KEY_PUNCH } ;	/* Sonic Boom */
static Uint16 com_smr[] = { 40, COM_GROUND, pl21_sp_smr, 0, KEY_DOWN, KEY_UP, KEY_KICK } ;	/* Somersault Shell */
static Uint16 com_moonsault[] = { 3, COM_AIR, pl21_sp_moonsault, 0, KEY_UP, KEY_UP | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Moonsault Slash */
static Uint16 com_break[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl21_sp_break, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Sonic Break (hyper) */
static Uint16 com_blitz[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl21_sp_blitz, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Crossfire Blitz (hyper) */
static Uint16 com_justice[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl21_sp_justice, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Somersault Justice (hyper) */

static void ( *pl21_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl21_main,
	pl21_com_ck,
	pl21_atck_set,
	pl21_atck,
	pl21_dash,
	pl21_noki,
	pl21_appear,
	pl21_win,
	pl21_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl21_catch_ck,	/* returns a value */
	pl21_catch_set,
	pl21_catch,
	pl21_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl21_delay_ck,
	pl21_t_kick,
	pl21_upper,
	pl21_sp_unison,
	pl21_sp_gattai,
	pl21_sp_assist,
	pl21_sp_zero,
} ;

static Uint8 sonic_ck( PLAYER_WORK *pl_w );
static Uint8 smr_ck( PLAYER_WORK *pl_w );
static Uint8 moonsault_ck( PLAYER_WORK *pl_w );
static Uint8 break_ck( PLAYER_WORK *pl_w );
static Uint8 blitz_ck( PLAYER_WORK *pl_w );
static Uint8 justice_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static int chouhatsu_ck( PLAYER_WORK *pl_w );
static int justice_delay_ck( PLAYER_WORK *pl_w );
static int blitz_delay_ck( PLAYER_WORK *pl_w );
static int break_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl21_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl21_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl21_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( justice_ck( pl_w ) ) return ;
	if( blitz_ck( pl_w ) ) return ;
	if( sonic_ck( pl_w ) ) return ;
	if( smr_ck( pl_w ) ) return ;
	if( moonsault_ck( pl_w ) ) return ;
	if( break_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
static Uint8 sonic_ck( PLAYER_WORK *pl_w )
{
	PL21_LOCAL_W *lw = ( PL21_LOCAL_W * )pl_w->tlocal ;

	if( !com_tame_ck( pl_w, com_sonic, &pl_w->com_w0 ) ) return 0 ;
	if( lw->shot_flag ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl21_sp_sonic ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 smr_ck( PLAYER_WORK *pl_w )
{
	if( !com_tame_ck( pl_w, com_smr, &pl_w->com_w1 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl21_sp_smr ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			In the air, once a jump unless it is a super		*/
/*			jump												*/
/*==============================================================*/
static Uint8 moonsault_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_moonsault, &pl_w->com_w2 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	if( pl_w->jump_com_cnt && !pl_w->sjump_flag ) return 0 ;
	pl_w->jump_com_cnt ++ ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl21_sp_moonsault ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 break_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_break, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w3 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl21_sp_break ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 blitz_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_blitz, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w4 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl21_sp_blitz ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 justice_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_justice, &pl_w->com_w5 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w5 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl21_sp_justice ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static int snapback_ck( PLAYER_WORK *pl_w )
{
	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl21_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static int chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 7 ) ) return 0 ;
	pl_w->pl_sp_type = pl21_sp_chouhatsu ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl21_delay_ck( PLAYER_WORK *pl_w )
{
	if( justice_delay_ck( pl_w ) || blitz_delay_ck( pl_w ) || break_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static int justice_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_justice, &pl_w->com_w5 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 3 ;
	return 1 ;
}

/*==============================================================*/
static int blitz_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_blitz, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 4 ;
	return 1 ;
}

/*==============================================================*/
static int break_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_break, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 2 ;
	return 1 ;
}

/****************************************************************/
void pl21_main( PLAYER_WORK *pl_w )
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
void pl21_atck_set( PLAYER_WORK *pl_w )
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
		if( pl_w->atck_lever & KEY_FORWARD ){
			pl_w->char_no.b.no = 3 ;
			pl_w->atck_no = pl21_at_fwd_hp ;
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
/*			The heavy kick has two forms with the stick held	*/
/*			forward or back										*/
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
		if( pl_w->atck_lever & KEY_FORWARD ){
			pl_w->routine_2 = 2 ;
			pl_w->char_no.b.no = 4 ;
			pl_w->atck_no = pl21_at_fwd_hk ;
			pl_w->spd.x = pl_w->pl_dir ? DOT_X( 4.0f ) : -DOT_X( 4.0f ) ;
			pl_w->acc.x = 0.0f ;
		} else if( pl_w->atck_lever & KEY_BACK ){
			pl_w->routine_2 = 1 ;
			pl_w->char_no.b.no = 3 ;
			pl_w->atck_no = pl21_at_back_hk ;
		} else {
			pl_w->char_no.b.no = 2 ;
			pl_w->atck_no = ATCK_STAND_HK ;
		}
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
		if( pl_w->atck_lever & KEY_DOWN ){
			pl_w->char_no.b.no = 0 ;
			pl_w->atck_no = pl21_at_jdown_lk ;
		} else {
			pl_w->char_no.b.no = 3 ;
			pl_w->atck_no = ATCK_JUMP_LK ;
		}
		pl_se_req( pl_w, SE_SWING_L ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_lk_tbl ;
		else pl_w->chain_tbl = sj_lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->char_no.b.no = 4 ;
		pl_w->atck_no = ATCK_JUMP_MK ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mk_tbl ;
		else pl_w->chain_tbl = sj_mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 5 ;
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
static void lever_atck1( PLAYER_WORK *pl_w );
static void lever_atck1_start( PLAYER_WORK *pl_w );
static void lever_atck1_jump( PLAYER_WORK *pl_w );
static void lever_atck1_land( PLAYER_WORK *pl_w );
static void lever_atck2( PLAYER_WORK *pl_w );

/****************************************************************/
void pl21_atck( PLAYER_WORK *pl_w )
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
	case 0:
	case 1:
	case 2:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	}
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
/*			The heavy kick held back hops, held forward			*/
/*			slides												*/
/*==============================================================*/
static void stand_kick_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 2:
		switch( pl_w->routine_2 ){
		case 1:
			lever_atck1( pl_w ) ;
			break ;
		default:
			if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
			else if( pl_w->routine_2 == 2 ) lever_atck2( pl_w ) ;
			break ;
		}
		break ;
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
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->spd.x = pl_w->pl_dir ? 5.0f : -5.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 4.0f ) ;
		pl_w->acc.y = -1.0044643f ;
	}
}

/*--------------------------------------------------------------*/
static void lever_atck1_jump( PLAYER_WORK *pl_w )
{
	if( !pl_w->cg_type ) pl_char_move( pl_w ) ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_3 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->sjump_flag = 0 ;
		pl_w->air_guard_cnt = 0 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
}

/*--------------------------------------------------------------*/
static void lever_atck1_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			The slide stops when the animation says				*/
/*==============================================================*/
static void lever_atck2( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type ){
		pl_w->routine_2 = 0 ;
		pl_w->spd.x = 0.0f ;
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
/*			The launcher										*/
/****************************************************************/
void pl21_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_char_set( pl_w, Char_Extra, 8 ) ;
		pl_w->atck_no = pl21_at_upper ;
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
static void dash_end( PLAYER_WORK *pl_w );

/****************************************************************/
void pl21_dash( PLAYER_WORK *pl_w )
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
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 9.5f ) : -DOT_X( 9.5f ) ;
		pl_w->acc.x = pl_w->pl_dir ? -DOT_X( 0.25f ) : DOT_X( 0.25f ) ;
	}
}

/*--------------------------------------------------------------*/
static void dash_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) >= 0 ){
		pl_smoke_set( pl_w ) ;
	} else {
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
void pl21_noki( PLAYER_WORK *pl_w )
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
		pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 9.5f ) : DOT_X( 9.5f ) ;
		pl_w->acc.x = pl_w->pl_dir ? DOT_X( 0.375f ) : -DOT_X( 0.375f ) ;
		pl_w->spd.y = 5.0f ;
		pl_w->acc.y = -DOT_X( 0.5f ) ;
	}
}

/*--------------------------------------------------------------*/
static void noki_jump( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Dash, 3 ) ;
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
		return ;
	}
	if( pl_w->cg_type ){
		pl_smoke_set( pl_w ) ;
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
}

static void appear_init( PLAYER_WORK *pl_w );
static void appear_move( PLAYER_WORK *pl_w );
static void win_time_ko( PLAYER_WORK *pl_w );
static void win_ko_init( PLAYER_WORK *pl_w );
static void win_ko_move( PLAYER_WORK *pl_w );
static void win_time( PLAYER_WORK *pl_w );
static void lose_time_ko( PLAYER_WORK *pl_w );
static void draw_time( PLAYER_WORK *pl_w );

/****************************************************************/
void pl21_appear( PLAYER_WORK *pl_w )
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
void pl21_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_time_ko,
		lose_time_ko,
		win_time,
		lose_time_ko,
		draw_time,
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
/*			Won by KO: one of four poses						*/
/*==============================================================*/
static void win_ko_init( PLAYER_WORK *pl_w )
{
	pl_w -> routine_2 ++ ;
	pl_w -> timer0 = Rnd() & 0x3 ;

	switch( pl_w -> timer0 ){
	case 0 :
		pl_char_set( pl_w, Char_Win, CH21_WIN_KO0 ) ;
		break ;
	case 1 :
		pl_char_set( pl_w, Char_Win, CH21_WIN_KO1 ) ;
		break ;
	case 2 :
		pl_char_set( pl_w, Char_Win, CH21_WIN_KO2 ) ;
		break ;
	case 3 :
		pl_char_set( pl_w, Char_Win, CH21_WIN_KO3 ) ;
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
		pl_char_set( pl_w, Char_Win, CH21_WIN_TIME ) ;
	}else{
		pl_char_move( pl_w ) ;
	}
}

/*==============================================================*/
static void lose_time_ko( PLAYER_WORK *pl_w )
{
	if( !pl_w -> routine_2 ){
		pl_w -> routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, CH21_LOSE ) ;
	}else{
		pl_char_move( pl_w ) ;
	}
}

/*==============================================================*/
static void draw_time( PLAYER_WORK *pl_w )
{
	if( !pl_w -> routine_2 ){
		pl_w -> routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, CH21_TM_DRAW ) ;
	}else{
		pl_char_move( pl_w ) ;
	}
}

static void sp_sonic( PLAYER_WORK *pl_w );
static void sp_smr( PLAYER_WORK *pl_w );
static void sp_break( PLAYER_WORK *pl_w );
static void sp_justice( PLAYER_WORK *pl_w );
static void sp_blitz( PLAYER_WORK *pl_w );
static void sp_moonsault( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );

/****************************************************************/
void	pl21_sp_atck( PLAYER_WORK *pl_w )
{
	static void (*pl_sp_tbl[])( PLAYER_WORK *pl_w ) ={
		sp_sonic,				/* 00 Sonic Boom */
		sp_smr,					/* 01 Somersault Shell */
		sp_break,					/* 02 Sonic Break (hyper) */
		sp_justice,					/* 03 Somersault Justice (hyper) */
		sp_blitz,					/* 04 Crossfire Blitz (hyper) */
		sp_sonic,				/* 05 */
		sp_moonsault,					/* 06 Moonsault Slash */
		sp_chouhatsu,					/* 07 Taunt */
		sp_sonic,				/* 08 */
		sp_sonic,				/* 09 */
		sp_sonic,				/* 0a */
		sp_snapback,					/* 0b Snapback */
	} ;

	pl_sp_tbl[ pl_w->pl_sp_type ]( pl_w ) ;
}

static void sonic_init( PLAYER_WORK *pl_w );
static void sonic_move( PLAYER_WORK *pl_w );
static void sonic_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Sonic Boom											*/
/*==============================================================*/
static void sp_sonic( PLAYER_WORK *pl_w )
{
	static void ( *sonic_jmp[])( PLAYER_WORK *pl_w ) = {
		sonic_init, sonic_move, sonic_end,
	} ;

	sonic_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void sonic_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl21_at_sonic ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_voice_req( pl_w, 21 ) ;
	shl21_00_set( pl_w, 1, 0 ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 0 ) ;
}

/*==============================================================*/
/*			The boom leaves when the animation says				*/
/*==============================================================*/
static void sonic_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) >= 0 ){
		if( HI_BYTE( pl_w->cg_rival ) & 1 ){
			HI_BYTE( pl_w->cg_rival ) &= ~1 ;
			pl_voice_req( pl_w, 30 ) ;
			shl21_00_set( pl_w, 0, 0 ) ;
		}
	} else {
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void sonic_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void smr_init( PLAYER_WORK *pl_w );
static void smr_start( PLAYER_WORK *pl_w );
static void smr_jump( PLAYER_WORK *pl_w );
static void smr_fall( PLAYER_WORK *pl_w );
static void smr_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Somersault Shell									*/
/*==============================================================*/
static void sp_smr( PLAYER_WORK *pl_w )
{
	static void ( *smr_jmp[])( PLAYER_WORK *pl_w ) = {
		smr_init, smr_start, smr_jump, smr_fall, smr_land,
	} ;

	smr_jmp[pl_w->routine_2]( pl_w ) ;
}

/* its speeds for each strength */
static SMR_SPD smr_spd[2] = {
	{ DOT_X( 2.5f ), DOT_Y( 9.0f ), -DOT_Y( 0.5f ) },
	{ 5.0f, DOT_Y( 13.0f ), -DOT_Y( 0.5f ) },
} ;

/*==============================================================*/
static void smr_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	if( pl_w->team_type == 3 ){
		pl_w->atck_no = pl21_at_smr_counter ;
		HIT_RESET( pl_w ) ;
	} else {
		pl_w->atck_no = pl_w->atck_level ? 0x35 : 0x33 ;
		HIT_RESET( pl_w ) ;
		pl_gauge_add( pl_w, 10 ) ;
	}
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = pl_w->pl_dir ? smr_spd[pl_w->atck_level].spd_x : -smr_spd[pl_w->atck_level].spd_x ;
	pl_w->spd.y = smr_spd[pl_w->atck_level].spd_y ;
	pl_w->acc.y = smr_spd[pl_w->atck_level].acc_y ;
	pl_w->acc.x = 0.0f ;
	shl21_00_set( pl_w, 2, 0 ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->char_no.b.no = pl_w->atck_level ? 4 : 2 ;
	pl_char_set( pl_w, Char_Special, pl_w->char_no.b.no ) ;
}

/*--------------------------------------------------------------*/
static void smr_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( HI_BYTE( pl_w->cg_rival ) & 1 ){
		pl_w->routine_2 ++ ;
		pl_air_set( pl_w ) ;
		shl21_00_set( pl_w, 7, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void smr_jump( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( HI_BYTE( pl_w->cg_rival ) >= 0 ){
		pl_char_move( pl_w ) ;
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			if( pl_w->team_type == 3 ){
				pl_w->atck_no = pl21_at_smr_counter_2 ;
				HIT_RESET( pl_w ) ;
			} else {
				pl_w->atck_no = pl21_at_smr_2 ;
				HIT_RESET( pl_w ) ;
			}
		}
	}
	if( pl_w->spd.y < 0.0f ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, 5 ) ;
	}
}

/*--------------------------------------------------------------*/
static void smr_fall( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( HI_BYTE( pl_w->cg_rival ) >= 0 ) pl_char_move( pl_w ) ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 6 ) ;
	}
}

/*--------------------------------------------------------------*/
static void smr_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void break_init( PLAYER_WORK *pl_w, PL21_LOCAL_W *lw );
static void break_wait( PLAYER_WORK *pl_w, PL21_LOCAL_W *lw );
static void break_move( PLAYER_WORK *pl_w, PL21_LOCAL_W *lw );
static void break_end( PLAYER_WORK *pl_w, PL21_LOCAL_W *lw );
static void break_shot( PLAYER_WORK *pl_w, Uint8 no );

/*==============================================================*/
/*			A hyper combo: a run of Sonic Booms, more for		*/
/*			each button pressed									*/
/*==============================================================*/
static void sp_break( PLAYER_WORK *pl_w )
{
	static void ( *break_jmp[])( PLAYER_WORK *pl_w, PL21_LOCAL_W *lw ) = {
		break_init, break_wait, break_move, break_end,
	} ;

	break_jmp[pl_w->routine_2]( pl_w, ( PL21_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void break_init( PLAYER_WORK *pl_w, PL21_LOCAL_W *lw )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl21_at_break ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->timer0 = 0 ;
	pl_w->timer1 = 2 ;
	lw->renda_cnt = 14 ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 25 ) ;
}

/*--------------------------------------------------------------*/
static void break_wait( PLAYER_WORK *pl_w, PL21_LOCAL_W *lw )
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
		ofs.x = DOT_X( 53.0f ) ;
		ofs.y = DOT_Y( 76.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void break_move( PLAYER_WORK *pl_w, PL21_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	/* each eighth press adds a boom */
	if( lw->renda_cnt ){
		if( sw_renda_ck( pl_w ) || pl_w->team_type == 5 || pl_w->team_type == 4 ){
			lw->renda_cnt -- ;
			if( !( lw->renda_cnt & 7 ) ) pl_w->timer1 ++ ;
		}
	}
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		break_shot( pl_w, 1 ) ;
		return ;
	}
	if( HI_BYTE( pl_w->cg_rival ) & 1 ){
		HI_BYTE( pl_w->cg_rival ) &= ~1 ;
		pl_voice_req( pl_w, 30 ) ;
		shl21_00_set( pl_w, 11, 0 ) ;
		shl21_00_set( pl_w, 11, 1 ) ;
		shl21_00_set( pl_w, 11, 2 ) ;
	}
	if( ( Uint8 )HI_BYTE( pl_w->cg_rival ) == 0xfe ){
		if( pl_w->timer1 ){
			HI_BYTE( pl_w->cg_rival ) = 0xfc ;
			pl_w->cg_ctr = 32 ;
		} else {
			HI_BYTE( pl_w->cg_rival ) = 0xfa ;
		}
	}
	if( ( Uint8 )HI_BYTE( pl_w->cg_rival ) == 0xfc ){
		pl_w->timer1 -- ;
		pl_w->timer0 = ( pl_w->timer0 + 1 ) & 3 ;
		break_shot( pl_w, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void break_end( PLAYER_WORK *pl_w, PL21_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			A boom, and the punch that throws it				*/
/*==============================================================*/
static void break_shot( PLAYER_WORK *pl_w, Uint8 no )
{
	static int break_char[8] = {
		7, 8, 9, 10, 11, 12, 13, 14,
	} ;

	shl21_00_set( pl_w, pl_w->timer0 + 3, 0 ) ;
	pl_w->char_no.b.no = break_char[pl_w->timer0 * 2 + no] ;
	pl_char_set( pl_w, Char_Special, pl_w->char_no.b.no ) ;
}

static void justice_init( PLAYER_WORK *pl_w );
static void justice_wait( PLAYER_WORK *pl_w );
static void justice_move( PLAYER_WORK *pl_w );
static void justice_up( PLAYER_WORK *pl_w );
static void justice_air( PLAYER_WORK *pl_w );
static void justice_fall( PLAYER_WORK *pl_w );
static void justice_land( PLAYER_WORK *pl_w );
static void justice_hit( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo: a rush with several hits, then a		*/
/*			Somersault Shell									*/
/*==============================================================*/
static void sp_justice( PLAYER_WORK *pl_w )
{
	static void ( *justice_jmp[])( PLAYER_WORK *pl_w ) = {
		justice_init, justice_wait, justice_move, justice_move, justice_up, justice_air, justice_fall, justice_land,
	} ;

	justice_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void justice_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl21_at_justice_1 ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->timer0 = 0 ;
	pl_w->spd.x = pl_w->pl_dir ? DOT_X( 5.0f ) : -DOT_X( 5.0f ) ;
	pl_sp_ef_set( pl_w ) ;
	shl21_00_set( pl_w, 2, 0 ) ;
	pl_char_set( pl_w, Char_Special, 18 ) ;
}

/*--------------------------------------------------------------*/
static void justice_wait( PLAYER_WORK *pl_w )
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
		ofs.x = -DOT_X( 4.0f ) ;
		ofs.y = DOT_Y( 59.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void justice_move( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) >= 0 ){
		justice_hit( pl_w ) ;
		if( HI_BYTE( pl_w->cg_rival ) & 1 ) pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->cg_flag1 ){
			pl_w->cg_flag1 = 0 ;
			shl21_00_set( pl_w, 8, 0 ) ;
		}
		return ;
	}
	if( pl_w->routine_2 == 2 ){
		pl_w->routine_2 ++ ;
		pl_w->atck_no = pl21_at_justice_1 ;
		HIT_RESET( pl_w ) ;
		pl_w->timer0 = 0 ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 5.0f ) : -DOT_X( 5.0f ) ;
		shl21_00_set( pl_w, 2, 0 ) ;
		pl_char_set( pl_w, Char_Special, 19 ) ;
	} else {
		pl_w->routine_2 ++ ;
		pl_w->atck_no = pl21_at_justice_2 ;
		HIT_RESET( pl_w ) ;
		pl_w->timer0 = 0 ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 16.0f ) : -DOT_X( 16.0f ) ;
		pl_w->acc.x = pl_w->pl_dir ? -1.25f : 1.25f ;
		pl_w->spd.y = DOT_Y( 16.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.5f ) ;
		shl21_00_set( pl_w, 8, 0 ) ;
		pl_char_set( pl_w, Char_Special, 20 ) ;
	}
}

/*--------------------------------------------------------------*/
static void justice_up( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	justice_hit( pl_w ) ;
	if( HI_BYTE( pl_w->cg_rival ) & 1 ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_AIR ;
		shl21_00_set( pl_w, 8, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void justice_air( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	/* stop once the rush has run out */
	if( ( pl_w->pl_dir && pl_w->spd.x < 0.0f ) || ( !pl_w->pl_dir && pl_w->spd.x > 0.0f ) ){
		RATE_CLR_X( pl_w ) ;
	}
	if( HI_BYTE( pl_w->cg_rival ) >= 0 ){
		pl_char_move( pl_w ) ;
		justice_hit( pl_w ) ;
	}
	if( pl_w->spd.y < 0.0f ){
		pl_w->routine_2 ++ ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		RATE_CLR_X( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 5 ) ;
	}
}

/*--------------------------------------------------------------*/
static void justice_fall( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( HI_BYTE( pl_w->cg_rival ) >= 0 ) pl_char_move( pl_w ) ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 6 ) ;
	}
}

/*--------------------------------------------------------------*/
static void justice_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			Each hit of the rush, up to three					*/
/*==============================================================*/
static void justice_hit( PLAYER_WORK *pl_w )
{
	static Uint8 justice_atck[] = { 0x37, 0x37, 0x37, 0x37, 0x38, 0x38, 0x38 } ;

	if( !pl_w->cg_type ) return ;
	if( pl_w->hit_flag ) pl_w->timer0 ++ ;
	if( pl_w->timer0 >= 3 ) return ;
	pl_w->atck_no = justice_atck[pl_w->cg_type >> 1] ;
	HIT_RESET( pl_w ) ;
}

static void blitz_init( PLAYER_WORK *pl_w, PL21_LOCAL_W *lw );
static void blitz_wait( PLAYER_WORK *pl_w, PL21_LOCAL_W *lw );
static void blitz_dash( PLAYER_WORK *pl_w, PL21_LOCAL_W *lw );
static void blitz_catch( PLAYER_WORK *pl_w, PL21_LOCAL_W *lw );
static void blitz_end( PLAYER_WORK *pl_w, PL21_LOCAL_W *lw );

/*==============================================================*/
/*			A hyper combo: a dash that grabs the opponent		*/
/*==============================================================*/
static void sp_blitz( PLAYER_WORK *pl_w )
{
	static void ( *blitz_jmp[])( PLAYER_WORK *pl_w, PL21_LOCAL_W *lw ) = {
		blitz_init, blitz_wait, blitz_dash, blitz_catch, blitz_end,
	} ;

	blitz_jmp[pl_w->routine_2]( pl_w, ( PL21_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void blitz_init( PLAYER_WORK *pl_w, PL21_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl21_at_blitz ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	em = pl_w->target_w ;
	lw->target_type = em->pl_type ;
	pl_w->timer0 = 16 ;
	pl_w->spd.x = pl_w->pl_dir ? 20.0f : -20.0f ;
	pl_sp_ef_set( pl_w ) ;
	shl21_00_set( pl_w, 6, 0 ) ;
	pl_char_set( pl_w, Char_Super, 0 ) ;
}

/*--------------------------------------------------------------*/
static void blitz_wait( PLAYER_WORK *pl_w, PL21_LOCAL_W *lw )
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
		ofs.x = DOT_X( 19.0f ) ;
		ofs.y = DOT_Y( 76.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void blitz_dash( PLAYER_WORK *pl_w, PL21_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->hit_flag ){
		/* caught: pull the opponent in front */
		if( pl_sp_catch_ck( pl_w ) ){
			pl_w->routine_2 ++ ;
			em = pl_w->em_w ;
			em->pos.x = pl_w->pos.x + ( pl_w->pl_dir ? 100.0f : -100.0f ) ;
			em->pos.y = pl_w->pos.y ;
			em->pl_stance = STANCE_STAND ;
			pl_w->spd.x /= 8.0f ;
			pl_char_set( pl_w, Char_Super, 3 ) ;
			return ;
		}
		pl_w->timer0 = 1 ;
	}
	if( -- pl_w->timer0 ) return ;
	pl_w->routine_2 = 4 ;
	pl_w->spd.x /= 8.0f ;
	pl_char_set( pl_w, Char_Super, 1 ) ;
}

/*--------------------------------------------------------------*/
static void blitz_catch( PLAYER_WORK *pl_w, PL21_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	em = pl_w->target_w ;
	if( em->pl_type != lw->target_type ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->routine_3 == 0 ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		pl_char_move( pl_w ) ;
		if( pl_w->cg_type ){
			pl_w->routine_3 ++ ;
			pl_w->cg_type = 0 ;
			pl_w->spd.x /= 4.0f ;
			pl_w->spd.y = DOT_Y( 8.0f ) ;
			pl_w->acc.y = -2.142857f ;
		}
		if( pl_w->cg_flag1 ){
			pl_w->atck_no = pl_w->cg_flag1 ;
			HIT_RESET( pl_w ) ;
			pl_w->cg_flag1 = 0 ;
		}
	} else {
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( !pl_w->cg_type ) pl_char_move( pl_w ) ;
		if( pl_w->cg_flag1 ){
			pl_w->atck_no = pl_w->cg_flag1 ;
			HIT_RESET( pl_w ) ;
			pl_w->cg_flag1 = 0 ;
		}
		if( pl_w->pos.y <= pl_w->ground_y ){
			pl_w->routine_2 ++ ;
			pl_w->pos.y = pl_w->ground_y ;
			pl_w->pl_stance = STANCE_STAND ;
			pl_w->delay_flag = 0 ;
			pl_w->delay_timer = 0 ;
			pl_w->hyper_bg = 0 ;
			pl_w->hyper_bg_tm = 0 ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_land_ef_set( pl_w ) ;
			pl_char_set( pl_w, Char_Jump, 3 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void blitz_end( PLAYER_WORK *pl_w, PL21_LOCAL_W *lw )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void moonsault_init( PLAYER_WORK *pl_w );
static void moonsault_move( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A jumping attack, farther for the strong button		*/
/*==============================================================*/
static void sp_moonsault( PLAYER_WORK *pl_w )
{
	static void ( *moonsault_jmp[])( PLAYER_WORK *pl_w ) = {
		moonsault_init, moonsault_move,
	} ;

	moonsault_jmp[pl_w->routine_2]( pl_w ) ;
}

static JUMP_SPD moonsault_spd[2] = {
	{ DOT_X( 4.0f ), -0.0f, 0.0f, -2.142857f },
	{ DOT_X( 8.0f ), -0.0f, 0.0f, -2.142857f },
} ;

/*==============================================================*/
static void moonsault_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl_w->atck_level ? 72 : 70 ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->spd.x = pl_w->pl_dir ? moonsault_spd[pl_w->atck_level].spd_x : -moonsault_spd[pl_w->atck_level].spd_x ;
	pl_w->acc.x = pl_w->pl_dir ? moonsault_spd[pl_w->atck_level].acc_x : -moonsault_spd[pl_w->atck_level].acc_x ;
	pl_w->spd.y = moonsault_spd[pl_w->atck_level].spd_y ;
	pl_w->acc.y = moonsault_spd[pl_w->atck_level].acc_y ;
	shl21_00_set( pl_w, 9, 0 ) ;
	pl_w->char_no.b.no = pl_w->atck_level ? 23 : 21 ;
	pl_char_set( pl_w, Char_Special, pl_w->char_no.b.no ) ;
}

/*--------------------------------------------------------------*/
static void moonsault_move( PLAYER_WORK *pl_w )
{
	if( !pl_w->cg_type ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( ( pl_w->pl_dir && pl_w->spd.x < 0.0f ) || ( !pl_w->pl_dir && pl_w->spd.x > 0.0f ) ){
			RATE_CLR_X( pl_w ) ;
		}
		if( pl_w->pos.y <= pl_w->ground_y ) pl_w->pos.y = pl_w->ground_y ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		RATE_CLR_X( pl_w ) ;
		pl_jump_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_flag1 ){
		pl_w->cg_flag1 = 0 ;
		pl_w->atck_no = pl21_at_moonsault_2 ;
		HIT_RESET( pl_w ) ;
	}
}

static void snapback_init( PLAYER_WORK *pl_w );
static void snapback_wait( PLAYER_WORK *pl_w );
static void snapback_jump( PLAYER_WORK *pl_w );
static void snapback_air( PLAYER_WORK *pl_w );
static void snapback_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The snapback: a flash, then a short leap forward	*/
/*==============================================================*/
static void sp_snapback( PLAYER_WORK *pl_w )
{
	static void ( *snapback_jmp[])( PLAYER_WORK *pl_w ) = {
		snapback_init, snapback_wait, snapback_jump, snapback_air, snapback_end,
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
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = pl21_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 24 ) ;
}

/*--------------------------------------------------------------*/
static void snapback_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type & 1 ){
		pl_w->cg_type = 0 ;
		ofs.x = 25.0f ;
		ofs.y = DOT_Y( 88.0f ) ;
		ofs.z = 0.0f ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
}

/*--------------------------------------------------------------*/
static void snapback_jump( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->spd.x = pl_w->pl_dir ? 5.0f : -5.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 4.0f ) ;
		pl_w->acc.y = -1.0044643f ;
		pl_se_req( pl_w, SE_SWING_M ) ;
	}
}

/*--------------------------------------------------------------*/
static void snapback_air( PLAYER_WORK *pl_w )
{
	if( !pl_w->cg_type ) pl_char_move( pl_w ) ;
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
	}
}

/*--------------------------------------------------------------*/
static void snapback_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			A special move: a pose that ends standing			*/
/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_char_set( pl_w, Char_Extra, 2 ) ;
	} else if( pl_char_move( pl_w ) < 0 ){
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_move( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A kick that leaps off the wall						*/
/****************************************************************/
void	pl21_t_kick( PLAYER_WORK *pl_w )
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
	pl_w->pos.y += DOT_Y( 48.0f ) ;
	pl_w->spd.x = DOT_X( 16.0f ) ;
	if( !pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = -DOT_Y( 0.25f ) ;
	pl_w->atck_no = pl21_at_t_kick ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 3 ) ;
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
		pl_char_set( pl_w, Char_Extra, 4 ) ;
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
PLAYER_WORK *pl21_catch_ck( PLAYER_WORK *pl_w )
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
			pl_w->catch_no = 1 ;
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

/****************************************************************/
/*			Start the throw that pl21_catch_ck chose			*/
/****************************************************************/
void	pl21_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_set, nml_k_set, jump_p_set,
	} ;

	catch_gauge_add( pl_w ) ;
	catch_set_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->catch_w->pl_dir = pl_w->catch_w->disp_dir = pl_w->pl_dir ^ 1 ;
	if( pl_w->dir & 1 ){
		pl_w->pl_dir = pl_w->disp_dir = pl_w->pl_dir ^ 1 ;
		pl_w->catch_w->pl_dir = pl_w->catch_w->disp_dir = pl_w->pl_dir ^ 1 ;
	}
	ofs.x = DOT_X( 20.0f ) ;
	ofs.y = 195.0f ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	scr_excep_set( pl_w, 5, 5 ) ;
	pl_char_set( pl_w, Char_Catch, 0 ) ;
}

/*==============================================================*/
static void nml_k_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->catch_w->pl_dir = pl_w->catch_w->disp_dir = pl_w->pl_dir ^ 1 ;
	ofs.x = -80.0f ;
	ofs.y = DOT_Y( 86.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	scr_excep_set( pl_w, 5, 5 ) ;
	pl_char_set( pl_w, Char_Catch, 1 ) ;
}

/*==============================================================*/
static void jump_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->catch_w->pl_dir = pl_w->catch_w->disp_dir = pl_w->pl_dir ^ 1 ;
	ofs.x = -DOT_X( 14.0f ) ;
	ofs.y = DOT_Y( 107.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	scr_excep_set( pl_w, 5, 5 ) ;
	pl_char_set( pl_w, Char_Catch, 2 ) ;
}

static void nml_p_catch( PLAYER_WORK *pl_w );
static void nml_k_catch( PLAYER_WORK *pl_w );
static void jump_p_catch( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void	pl21_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_catch, nml_k_catch, jump_p_catch,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_char_move( pl_w ) >= 0 ){
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			em = pl_w->catch_w ;
			em->dm_hit_w = pl_w ;
			em->catch_type = 11 ;
			em->atck_no = pl21_at_throw_p ;
			SCR_EXCEP_SET ;
			catch_se_req( pl_w ) ;
		}
		return ;
	}
	pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void nml_k_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	if( pl_char_move( pl_w ) >= 0 ){
		if( pl_w->cg_type < 0 ){
			pl_w->cg_type = 0 ;
			em = pl_w->catch_w ;
			em->atck_no = pl21_at_throw_k_hit ;
			catch_damage( pl_w ) ;
			ofs.x = -DOT_X( 32.0f ) ;
			ofs.y = DOT_Y( 32.0f ) ;
			ofs.z = 0.0f ;
			hit_mark_set( pl_w, &ofs, 1 ) ;
			pl_voice_req( pl_w, 3 ) ;
			pl_se_req( pl_w, 0 ) ;
		} else if( pl_w->cg_type > 0 ){
			pl_w->cg_type = 0 ;
			em = pl_w->catch_w ;
			em->dm_hit_w = pl_w ;
			em->catch_type = 1 ;
			em->atck_no = pl21_at_throw_k ;
			SCR_EXCEP_SET ;
		}
		return ;
	}
	pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void jump_p_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	if( pl_w->routine_2 == 0 ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( !pl_w->cg_type ) pl_char_move( pl_w ) ;
		if( pl_w->routine_3 == 0 && pl_w->pos.y <= DOT_Y( 160.0f ) ){
			pl_w->routine_3 ++ ;
			pl_voice_req( pl_w, 5 ) ;
		}
		if( pl_w->pos.y <= pl_w->ground_y ){
			pl_w->routine_2 ++ ;
			pl_char_set( pl_w, Char_Catch, 3 ) ;
			catch_se_req( pl_w ) ;
		}
	} else {
		if( pl_char_move( pl_w ) >= 0 ){
			if( pl_w->cg_type < 0 ){
				pl_w->cg_type = 0 ;
				em = pl_w->catch_w ;
				em->atck_no = pl21_at_air_throw_hit ;
				catch_damage( pl_w ) ;
				ofs.x = -DOT_X( 32.0f ) ;
				ofs.y = DOT_Y( 32.0f ) ;
				ofs.z = 0.0f ;
				hit_mark_set( pl_w, &ofs, 1 ) ;
			} else if( pl_w->cg_type > 0 ){
				pl_w->cg_type = 0 ;
				em = pl_w->catch_w ;
				em->dm_hit_w = pl_w ;
				em->catch_type = 1 ;
				em->atck_no = pl21_at_air_throw ;
			}
			return ;
		}
		pl_stand_ret( pl_w ) ;
	}
}

/****************************************************************/
/*			Holding the opponent								*/
/****************************************************************/
void	pl21_hold( PLAYER_WORK *pl_w )
{
	catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void	pl21_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl21_sp_break ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl21_sp_justice ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl21_sp_break ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl21_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl21_sp_break ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl21_sp_justice ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl21_sp_break ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl21_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl21_sp_sonic ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl21_sp_smr ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl21_sp_sonic ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void	pl21_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl21_sp_sonic ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl21_sp_smr ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl21_sp_smr ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl21.c" End of File	*/

/* Blackheart (character 35): command checks, normal attacks, appearance, win and lose poses,
   special moves and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl35_def.h"

void pl35_main( PLAYER_WORK *pl_w );
void pl35_com_ck( PLAYER_WORK *pl_w );
void pl35_atck_set( PLAYER_WORK *pl_w );
void pl35_atck( PLAYER_WORK *pl_w );
void pl35_dash( PLAYER_WORK *pl_w );
void pl35_noki( PLAYER_WORK *pl_w );
void pl35_appear( PLAYER_WORK *pl_w );
void pl35_win( PLAYER_WORK *pl_w );
void pl35_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl35_catch_ck( PLAYER_WORK *pl_w );
void pl35_catch_set( PLAYER_WORK *pl_w );
void pl35_catch( PLAYER_WORK *pl_w );
void pl35_hold( PLAYER_WORK *pl_w );
int pl35_delay_ck( PLAYER_WORK *pl_w );
void pl35_t_kick( PLAYER_WORK *pl_w );
void pl35_upper( PLAYER_WORK *pl_w );
void pl35_sp_unison( PLAYER_WORK *pl_w );
void pl35_sp_gattai( PLAYER_WORK *pl_w );
void pl35_sp_assist( PLAYER_WORK *pl_w );
void pl35_sp_zero( PLAYER_WORK *pl_w );

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
static Sint8 sj_hp_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 sj_lk_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 sj_mk_tbl[4] = { -1,  2, -1,  2 } ;
static Sint8 sj_hk_tbl[4] = { -1, -1, -1, -1 } ;

/* commands: time or steps, kind, the special move, then the stick and buttons */
static Uint16 com_heart[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl35_sp_heart, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Heart of Darkness (hyper) */
static Uint16 com_armageddon[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl35_sp_armageddon, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Armageddon (hyper) */
static Uint16 com_judgement[] = { 3, COM_HYPER | COM_ALL_SW, pl35_sp_judgement, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Judgement Day */
static Uint16 com_thunder[] = { 5, COM_GROUND, pl35_sp_thunder, 0, KEY_BACK, KEY_DOWN | KEY_BACK, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Dark Thunder */
static Uint16 com_inferno[] = { 5, COM_GROUND, pl35_sp_inferno, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Inferno */

static void ( *pl35_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl35_main,
	pl35_com_ck,
	pl35_atck_set,
	pl35_atck,
	pl35_dash,
	pl35_noki,
	pl35_appear,
	pl35_win,
	pl35_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl35_catch_ck,	/* returns a value */
	pl35_catch_set,
	pl35_catch,
	pl35_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl35_delay_ck,
	pl35_t_kick,
	pl35_upper,
	pl35_sp_unison,
	pl35_sp_gattai,
	pl35_sp_assist,
	pl35_sp_zero,
} ;

/* the character's own work, set by the routines that use it */
static PL35_LOCAL_W *local_w ;

static Uint8 heart_ck( PLAYER_WORK *pl_w );
static Uint8 armageddon_ck( PLAYER_WORK *pl_w );
static Uint8 thunder_ck( PLAYER_WORK *pl_w );
static Uint8 inferno_ck( PLAYER_WORK *pl_w );
static Uint8 judgement_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static int chouhatsu_ck( PLAYER_WORK *pl_w );
static int heart_delay_ck( PLAYER_WORK *pl_w );
static int armageddon_delay_ck( PLAYER_WORK *pl_w );
static int judgement_delay_ck( PLAYER_WORK *pl_w );
static void shot_init( PLAYER_WORK *pl_w );

/****************************************************************/
void pl35_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl35_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl35_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->act_st == N_DASH || pl_w->act_st == N_BACK_DASH ) return ;
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( heart_ck( pl_w ) ) return ;
	if( armageddon_ck( pl_w ) ) return ;
	if( judgement_ck( pl_w ) ) return ;
	if( thunder_ck( pl_w ) ) return ;
	if( inferno_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	if( pl_air_dash_sw_ck( pl_w ) ) return ;
	if( pl_air_dash_ck( pl_w, &pl_w->com_w12 ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
static Uint8 heart_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_heart, &pl_w->com_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl35_sp_heart ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 armageddon_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_armageddon, &pl_w->com_w0 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl35_sp_armageddon ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 thunder_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_thunder, &pl_w->com_w1 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl35_sp_thunder ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 inferno_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_inferno, &pl_w->com_w2 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl35_sp_inferno ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			Also in the air, once a jump						*/
/*==============================================================*/
static Uint8 judgement_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_judgement, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl35_sp_judgement ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static int snapback_ck( PLAYER_WORK *pl_w )
{
	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl35_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static int chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 5 ) ) return 0 ;
	pl_w->pl_sp_type = pl35_sp_chouhatsu ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl35_delay_ck( PLAYER_WORK *pl_w )
{
	if( heart_delay_ck( pl_w ) || armageddon_delay_ck( pl_w ) || judgement_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static int heart_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_heart, &pl_w->com_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 0 ;
	return 1 ;
}

/*==============================================================*/
static int armageddon_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_armageddon, &pl_w->com_w0 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 1 ;
	return 1 ;
}

/*==============================================================*/
static int judgement_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_judgement, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 8 ;
	return 1 ;
}

/*==============================================================*/
/*			The flash of a heavy attack, in the colour's own	*/
/*			shade												*/
/*==============================================================*/
static void atck_color_set( PLAYER_WORK *pl_w )
{
	if( !( pl_w->pl_color & 1 ) ) ef_color_set( pl_w, 3, 4, 1 ) ;
	else ef_color_set( pl_w, 3, 16, 1 ) ;
}

/*--------------------------------------------------------------*/
static void stand_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/****************************************************************/
void pl35_main( PLAYER_WORK *pl_w )
{
	PL35_LOCAL_W *lw = ( PL35_LOCAL_W * )pl_w->tlocal ;

	if( lw->hold_timer ) lw->hold_timer -- ;
	if( lw->se_timer && !-- lw->se_timer ) pl_se_req( pl_w, 59 ) ;
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
void pl35_atck_set( PLAYER_WORK *pl_w )
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
		ef_color_set( pl_w, 1, 18, 1 ) ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_STAND_HP ;
		pl_se_req( pl_w, SE_SWING_H ) ;
		pl_w->chain_tbl = hp_tbl ;
		pl_w->nml_level = 2 ;
		atck_color_set( pl_w ) ;
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
		atck_color_set( pl_w ) ;
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
		atck_color_set( pl_w ) ;
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
		atck_color_set( pl_w ) ;
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
		ef_color_set( pl_w, 1, 18, 1 ) ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_JUMP_HP ;
		pl_se_req( pl_w, SE_SWING_H ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_hp_tbl ;
		else pl_w->chain_tbl = sj_hp_tbl ;
		pl_w->nml_level = 2 ;
		atck_color_set( pl_w ) ;
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
		atck_color_set( pl_w ) ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Jump_K, pl_w->char_no.b.no ) ;
	if( pl_w->jump_atk_cnt & JATK_K_MASK ) pl_w->jump_atk_cnt -= 0x10 ;
}

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
void pl35_atck( PLAYER_WORK *pl_w )
{
	static void ( *atck_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_move, jump_move, sjump_move, nml_move,
	} ;

	atck_jmp[pl_w->atck_type]( pl_w ) ;
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
/*			The heavy and middle attacks set off their			*/
/*			effects as the animation says						*/
/*==============================================================*/
static void stand_punch_move( PLAYER_WORK *pl_w )
{
	Uint8 i ;

	switch( pl_w->nml_no ){
	case 2:
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			shot_init( pl_w ) ;
			for( i = 0 ; i < 5 ; i++ ) shl35_04_set( pl_w, 0, i ) ;
		}
		break ;
	case 1:
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			shl35_04_set( pl_w, 6, 0 ) ;
		}
		break ;
	}
	stand_end( pl_w ) ;
}

/*==============================================================*/
static void crouch_punch_move( PLAYER_WORK *pl_w )
{
	Uint8 i ;

	switch( pl_w->nml_no ){
	case 2:
		if( pl_w->cg_type ){
			if( ( Uint8 )pl_w->cg_type != 0x81 ) shot_init( pl_w ) ;
			pl_w->cg_type = 0 ;
			for( i = 0 ; i < 3 ; i++ ) shl35_04_set( pl_w, 1, i ) ;
		}
		break ;
	case 1:
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			shl35_04_set( pl_w, 7, 0 ) ;
		}
		break ;
	}
	stand_end( pl_w ) ;
}

/*==============================================================*/
static void stand_kick_move( PLAYER_WORK *pl_w )
{
	Uint8 i ;

	if( pl_w->nml_no == 2 && pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shot_init( pl_w ) ;
		for( i = 0 ; i < 6 ; i++ ) shl35_04_set( pl_w, 3, i ) ;
	}
	stand_end( pl_w ) ;
}

/*==============================================================*/
static void crouch_kick_move( PLAYER_WORK *pl_w )
{
	if( pl_w->nml_no == 2 && pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shot_init( pl_w ) ;
		shl35_04_set( pl_w, 4, 0 ) ;
	}
	stand_end( pl_w ) ;
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
	Uint8 i ;

	switch( pl_w->nml_no ){
	case 2:
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			shot_init( pl_w ) ;
			for( i = 0 ; i < 3 ; i++ ) shl35_04_set( pl_w, 2, i ) ;
		}
		break ;
	case 1:
		if( pl_w->cg_prm ){
			pl_w->cg_prm = 0 ;
			shl35_04_set( pl_w, 8, 0 ) ;
		}
		break ;
	default:
		break ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/*==============================================================*/
static void jump_kick_move( PLAYER_WORK *pl_w )
{
	Uint8 i ;

	if( pl_w->nml_no == 2 && pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shot_init( pl_w ) ;
		for( i = 0 ; i < 5 ; i++ ) shl35_04_set( pl_w, 5, i ) ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/****************************************************************/
/*			The launcher										*/
/****************************************************************/
void pl35_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_gauge_add( pl_w, 5 ) ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Extra, 5 ) ;
		pl_w->atck_no = pl35_at_upper ;
		HIT_RESET( pl_w ) ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shl35_04_set( pl_w, 6, 0 ) ;
	}
	if( pl_w->atck_type == 3 ) pl_smoke_set( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_brake_stop( pl_w ) ;
	stand_end( pl_w ) ;
}

static void dash_init( PLAYER_WORK *pl_w );
static void dash_move( PLAYER_WORK *pl_w );
static void dash_stop( PLAYER_WORK *pl_w );
static void dash_end( PLAYER_WORK *pl_w );
static void step_move( PLAYER_WORK *pl_w );
static void dash_lever_ck( PLAYER_WORK *pl_w );
static void noki_lever_ck( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The dash: he moves in steps the animation gives		*/
/****************************************************************/
void pl35_dash( PLAYER_WORK *pl_w )
{
	static void ( *dash_jmp[])( PLAYER_WORK *pl_w ) = {
		dash_init, dash_move, dash_stop, dash_end,
	} ;

	local_w = ( PL35_LOCAL_W * )pl_w->tlocal ;
	dash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dash_init( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		local_w->lever_off_flag = 0 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		dash_move( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void dash_move( PLAYER_WORK *pl_w )
{
	dash_lever_ck( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		if( local_w->lever_off_flag ) pl_char_set( pl_w, Char_Dash, 4 ) ;
		else pl_char_set( pl_w, Char_Dash, 5 ) ;
	}
	step_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void dash_stop( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Dash, 2 ) ;
	}
	step_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void dash_end( PLAYER_WORK *pl_w )
{
	step_move( pl_w ) ;
	stand_end( pl_w ) ;
}

/*==============================================================*/
/*			A step forward, its length chosen by the			*/
/*			animation											*/
/*==============================================================*/
static void step_move( PLAYER_WORK *pl_w )
{
	static Float step_tbl[] = {
		0.0f, 240.0f, DOT_X( 32.0f ), DOT_X( 112.0f ), DOT_X( 16.0f ), -205.0f, -DOT_X( 32.0f ), -205.0f,
		-DOT_X( 16.0f ), -240.0f, 205.0f,
	} ;
	Uint8 no ;
	Float step ;

	if( pl_w->cg_type < 0 ){
		pl_w->push_ng_timer = 1 ;
		if( ( no = pl_w->cg_type & 0x7f ) ){
			pl_w->cg_type = 0x80 ;
			step = step_tbl[no] ;
			if( !pl_w->pl_dir ) step = -step ;
			pl_w->pos.x += step ;
		}
	}
}

/*==============================================================*/
/*			The stick let go of forward							*/
/*==============================================================*/
static void dash_lever_ck( PLAYER_WORK *pl_w )
{
	Sint16 lever = pl_w->pl_sw_new & pl_w->pl_sw_old ;

	if( lever != KEY_FORWARD ) local_w->lever_off_flag = 1 ;
}

/*==============================================================*/
/*			The stick let go of back							*/
/*==============================================================*/
static void noki_lever_ck( PLAYER_WORK *pl_w )
{
	Sint16 lever = pl_w->pl_sw_new & pl_w->pl_sw_old ;

	if( lever != KEY_BACK ) local_w->lever_off_flag = 1 ;
}

static void noki_init( PLAYER_WORK *pl_w );
static void noki_move( PLAYER_WORK *pl_w );
static void noki_stop( PLAYER_WORK *pl_w );
static void noki_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash										*/
/****************************************************************/
void pl35_noki( PLAYER_WORK *pl_w )
{
	static void ( *noki_jmp[])( PLAYER_WORK *pl_w ) = {
		noki_init, noki_move, noki_stop, noki_end,
	} ;

	local_w = ( PL35_LOCAL_W * )pl_w->tlocal ;
	noki_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void noki_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	local_w->lever_off_flag = 0 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	noki_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void noki_move( PLAYER_WORK *pl_w )
{
	noki_lever_ck( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		if( local_w->lever_off_flag ) pl_char_set( pl_w, Char_Dash, 6 ) ;
		else pl_char_set( pl_w, Char_Dash, 7 ) ;
	}
	step_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void noki_stop( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Dash, 3 ) ;
	}
	step_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void noki_end( PLAYER_WORK *pl_w )
{
	step_move( pl_w ) ;
	stand_end( pl_w ) ;
}

static void appear_init( PLAYER_WORK *pl_w );
static void appear_move( PLAYER_WORK *pl_w );
static void appear_end( PLAYER_WORK *pl_w );

/****************************************************************/
void pl35_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_init, appear_move, appear_end,
	} ;

	appear_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			He rises out of the floor							*/
/*==============================================================*/
static void appear_init( PLAYER_WORK *pl_w )
{
	local_w = ( PL35_LOCAL_W * )pl_w->tlocal ;
	pl_w->routine_2 ++ ;
	pl_w->disp_flag = 1 ;
	pl_w->pos.y = pl_w->ground_y + -DOT_Y( 178.0f ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.y = DOT_Y( 4.0f ) ;
	pl_w->timer0 = 0 ;
	pl_w->timer1 = 4 ;
	if( pl_w->pl_color & 1 ){
		ef_color_set( pl_w, 1, 32, 1 ) ;
		ef_color_set( pl_w, 2, 33, 1 ) ;
	} else {
		ef_color_set( pl_w, 1, 35, 1 ) ;
		ef_color_set( pl_w, 2, 36, 1 ) ;
	}
	local_w->land_flag = 0 ;
	pls35_01_set( pl_w ) ;
	pl_char_set( pl_w, Char_Appear, 0 ) ;
}

/*--------------------------------------------------------------*/
static void appear_move( PLAYER_WORK *pl_w )
{
	local_w = ( PL35_LOCAL_W * )pl_w->tlocal ;
	if( !pl_w->cg_type ){
		pl_w->disp_flag = 1 ;
		if( !( pl_w->timer0 & 0x0f ) ) pls35_02_set( pl_w ) ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->ground_y < pl_w->pos.y ){
			pl_w->routine_2 ++ ;
			local_w->land_flag = 1 ;
			pl_w->pos.y = pl_w->ground_y ;
			pl_w->pl_stance = STANCE_STAND ;
			pl_char_set( pl_w, Char_Appear, 11 ) ;
			return ;
		}
		if( !-- pl_w->timer1 ){
			pl_w->timer1 = 4 ;
			if( ++ pl_w->timer0 >= 11 ) pl_w->timer0 = 11 ;
			pl_char_set( pl_w, Char_Appear, pl_w->timer0 ) ;
			return ;
		}
	} else {
		pl_w->disp_flag = 0 ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void appear_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_w->routine_1 ++ ;
}

static void win_init( PLAYER_WORK *pl_w );
static void win_ko( PLAYER_WORK *pl_w );
static void win_time( PLAYER_WORK *pl_w );
static void lose( PLAYER_WORK *pl_w );
static void win_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl35_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_init, win_move,
	} ;

	if( pl_win_end_ck( pl_w ) ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	win_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void win_init( PLAYER_WORK *pl_w )
{
	static void ( *win_init_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko, lose, win_time, lose, lose,
	} ;

	pl_w->routine_2 ++ ;
	win_init_jmp[pl_w->type]( pl_w ) ;
}

/*==============================================================*/
static void win_ko( PLAYER_WORK *pl_w )
{
	ef_color_set( pl_w, 2, 17, 1 ) ;
	pl_char_set( pl_w, Char_Win, Rnd() & 1 ) ;
}

/*==============================================================*/
static void win_time( PLAYER_WORK *pl_w )
{
	pl_char_set( pl_w, Char_Win, 2 ) ;
}

/*==============================================================*/
static void lose( PLAYER_WORK *pl_w )
{
	pl_char_set( pl_w, Char_Win, 3 ) ;
}

/*--------------------------------------------------------------*/
static void win_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

static void sp_heart( PLAYER_WORK *pl_w );
static void sp_armageddon( PLAYER_WORK *pl_w );
static void sp_thunder( PLAYER_WORK *pl_w );
static void sp_inferno( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_judgement( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );
static void sp_gamma( PLAYER_WORK *pl_w );

/****************************************************************/
void pl35_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *pl_sp_tbl[])( PLAYER_WORK *pl_w ) = {
		sp_heart, sp_armageddon, sp_thunder, sp_inferno, sp_heart, sp_chouhatsu, sp_heart, sp_heart, sp_judgement, sp_snapback, sp_gamma,
	} ;

	local_w = ( PL35_LOCAL_W * )pl_w->tlocal ;
	pl_sp_tbl[pl_w->pl_sp_type]( pl_w ) ;
}

static void heart_init( PLAYER_WORK *pl_w );
static void heart_wait( PLAYER_WORK *pl_w );
static void heart_start( PLAYER_WORK *pl_w );
static void heart_move( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Heart of Darkness									*/
/*==============================================================*/
static void sp_heart( PLAYER_WORK *pl_w )
{
	static void ( *heart_jmp[])( PLAYER_WORK *pl_w ) = {
		heart_init, heart_wait, heart_start, heart_move, stand_end,
	} ;

	heart_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void heart_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = 0 ;
	HIT_RESET( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 0 ) ;
}

/*--------------------------------------------------------------*/
static void heart_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 0x80 ;
		pl_w->timer1 = 6 ;
		pl_w->cg_type = 0 ;
		ofs.x = -DOT_X( 7.0f ) ;
		ofs.y = DOT_Y( 92.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	} else pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void heart_start( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->routine_2 ++ ;
	ef_color_set( pl_w, 2, 10, 1 ) ;
	pl_se_req( pl_w, 77 ) ;
	heart_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void heart_move( PLAYER_WORK *pl_w )
{
	PL35_LOCAL_W *lw = ( PL35_LOCAL_W * )pl_w->tlocal ;

	lw->se_timer = 4 ;
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( !-- pl_w->timer0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Super, 1 ) ;
		return ;
	}
	if( !-- pl_w->timer1 ){
		pl_w->timer1 = 5 ;
		shl35_00_set( pl_w ) ;
	}
}

static void armageddon_init( PLAYER_WORK *pl_w );
static void armageddon_wait( PLAYER_WORK *pl_w );
static void armageddon_start( PLAYER_WORK *pl_w );
static void armageddon_move( PLAYER_WORK *pl_w );
static void armageddon_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Armageddon: he rises off the screen					*/
/*==============================================================*/
static void sp_armageddon( PLAYER_WORK *pl_w )
{
	static void ( *armageddon_jmp[])( PLAYER_WORK *pl_w ) = {
		armageddon_init, armageddon_wait, armageddon_start, armageddon_move, armageddon_end, stand_end,
	} ;

	armageddon_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void armageddon_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = 0 ;
	HIT_RESET( pl_w ) ;
	pl_w->timer0 = 112 ;
	local_w->fiend_end_flag = 0 ;
	pl_w->spd.y = DOT_Y( 1.5f ) ;
	pl_air_set( pl_w ) ;
	if( pl_w->pl_color & 1 ){
		ef_color_set( pl_w, 1, 32, 1 ) ;
		ef_color_set( pl_w, 2, 33, 1 ) ;
	} else {
		ef_color_set( pl_w, 1, 35, 1 ) ;
		ef_color_set( pl_w, 2, 36, 1 ) ;
	}
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 4 ) ;
}

/*--------------------------------------------------------------*/
static void armageddon_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		if( !shl35_02_set( pl_w ) ) pl_stand_ret( pl_w ) ;
		else {
			ofs.x = -DOT_X( 7.0f ) ;
			ofs.y = DOT_Y( 92.0f ) ;
			ofs.z = 0.0f ;
			pl_hyper_start( pl_w, &ofs, 1 ) ;
		}
	} else pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void armageddon_start( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->routine_2 ++ ;
	armageddon_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void armageddon_move( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !-- pl_w->timer0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_2 ++ ;
		local_w->fiend_end_flag = 0xff ;
		ef_color_set( pl_w, 2, 19, 1 ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->acc.y = -0.46875f ;
		pl_char_set( pl_w, Char_Jump, 9 ) ;
	}
}

/*--------------------------------------------------------------*/
static void armageddon_end( PLAYER_WORK *pl_w )
{
	pl_jump_ret( pl_w ) ;
}

static void thunder_init( PLAYER_WORK *pl_w );
static void thunder_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_thunder( PLAYER_WORK *pl_w )
{
	static void ( *thunder_jmp[])( PLAYER_WORK *pl_w ) = {
		thunder_init, thunder_move, stand_end,
	} ;

	thunder_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void thunder_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = 0 ;
	HIT_RESET( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_gauge_add( pl_w, 10 ) ;
	pl_w->timer0 = 80 ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->char_no.b.no = 1 ;
	pl_char_set( pl_w, Char_Special, 1 ) ;
}

/*--------------------------------------------------------------*/
static void thunder_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 ){
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			ef_color_set( pl_w, 1, 5, 1 ) ;
			shl35_01_set( pl_w ) ;
		}
	} else {
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, 2 ) ;
	}
}

static void inferno_init( PLAYER_WORK *pl_w );
static void inferno_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_inferno( PLAYER_WORK *pl_w )
{
	static void ( *inferno_jmp[])( PLAYER_WORK *pl_w ) = {
		inferno_init, inferno_move,
	} ;

	inferno_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void inferno_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = 0 ;
	HIT_RESET( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->timer0 = 112 ;
	pl_w->pl_stance = STANCE_STAND ;
	ef_color_set( pl_w, 1, 7, 1 ) ;
	ef_color_set( pl_w, 4, 7, 1 ) ;
	pl_gauge_add( pl_w, 10 ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 0 ) ;
}

/*--------------------------------------------------------------*/
static void inferno_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 ){
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pl_w->unk_27b = 1 ;
			pl_w->cancel_timer = 16 ;
			shl35_03_start( pl_w ) ;
		}
	} else pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w )
{
	if( pl_w->routine_2 ){
		stand_end( pl_w ) ;
		return ;
	}
	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_char_set( pl_w, Char_Extra, 4 ) ;
}

static void judgement_init( PLAYER_WORK *pl_w );
static void judgement_wait( PLAYER_WORK *pl_w );
static void judgement_move( PLAYER_WORK *pl_w );
static void judgement_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo, on the ground or in the air			*/
/*==============================================================*/
static void sp_judgement( PLAYER_WORK *pl_w )
{
	static void ( *judgement_jmp[])( PLAYER_WORK *pl_w ) = {
		judgement_init, judgement_wait, judgement_move, judgement_end,
	} ;

	judgement_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void judgement_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl35_at_judgement ;
	HIT_RESET( pl_w ) ;
	pl_w->timer0 = 96 ;
	pl_w->timer1 = 2 ;
	if( pl_w->pl_stance == STANCE_AIR ) pl_char_set( pl_w, Char_Super, 15 ) ;
	else {
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Super, 13 ) ;
		pl_sp_ef_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void judgement_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		atck_color_set( pl_w ) ;
		ofs.x = -DOT_X( 7.0f ) ;
		ofs.y = DOT_Y( 92.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	} else pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void judgement_move( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 ){
		if( !-- pl_w->timer1 ){
			pl_w->timer1 = 4 ;
			if( pl_w->pl_stance != STANCE_AIR ) shl35_04_set( pl_w, 10, 0 ) ;
			else shl35_04_set( pl_w, 11, 0 ) ;
		}
	} else {
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Super, pl_w->pl_stance == STANCE_AIR ? 16 : 14 ) ;
	}
}

/*--------------------------------------------------------------*/
static void judgement_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		if( pl_w->pl_stance == STANCE_AIR ) pl_jump_ret( pl_w ) ;
		else pl_stand_ret( pl_w ) ;
	}
}

static void snapback_init( PLAYER_WORK *pl_w );
static void snapback_move( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A move that sets off an effect in front of him		*/
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
	pl_char_set( pl_w, Char_Special, 26 ) ;
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
		ofs.x = DOT_X( 80.0f ) ;
		ofs.y = DOT_Y( 40.0f ) ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
	if( pl_w->cg_prm ){
		pl_w->cg_prm = 0 ;
		shl35_04_set( pl_w, 7, 1 ) ;
		pl_se_req( pl_w, SE_SWING_H ) ;
	}
}

/*==============================================================*/
/*			The assist's move									*/
/*==============================================================*/
static void sp_gamma( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->pos.y = pl_w->ground_y ;
		if( pl_w->team_type == 3 ){
			pl_w->atck_no = pl35_at_counter ;
			HIT_RESET( pl_w ) ;
		} else {
			pl_w->atck_no = pl35_at_gamma ;
			HIT_RESET( pl_w ) ;
		}
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_dir_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 27 ) ;
	} else {
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			shl35_04_set( pl_w, 6, 0 ) ;
		}
		stand_end( pl_w ) ;
	}
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_move( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The partner's jump in								*/
/****************************************************************/
void pl35_t_kick( PLAYER_WORK *pl_w )
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
	pl_w->atck_no = pl35_at_t_kick ;
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
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, 1 ) ;
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

static PLAYER_WORK *nml_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *crouch_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *jump_catch_ck( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Can a throw start?									*/
/****************************************************************/
PLAYER_WORK *pl35_catch_ck( PLAYER_WORK *pl_w )
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

	if( !( pl_w->dir = ( pl_w->atck_lever & ( KEY_DOWN | KEY_FORWARD | KEY_BACK ) ) >> 10 ) ) return 0 ;
	if( pl_w->atck_level == 1 && pl_w->pos.y > DOT_Y( 64.0f ) ){
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
static void jump_set( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Start the throw that pl35_catch_ck chose			*/
/****************************************************************/
void pl35_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_set, nml_k_set, jump_set,
	} ;
	PLAYER_WORK *em ;

	catch_gauge_add( pl_w ) ;
	em = pl_w->catch_w ;
	em->pl_dir = em->disp_dir = pl_w->pl_dir ^ 1 ;
	catch_set_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	if( pl_w->dir & 1 ){
		pl_w->pl_dir = pl_w->disp_dir = pl_w->pl_dir ^ 1 ;
		em = pl_w->catch_w ;
		em->pl_dir = em->disp_dir = em->pl_dir ^ 1 ;
	}
	ofs.x = -DOT_X( 64.0f ) ;
	ofs.y = DOT_Y( 96.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	ef_color_set( pl_w, 1, 11, 1 ) ;
	pl_char_set( pl_w, Char_Catch, 0 ) ;
}

/*==============================================================*/
static void nml_k_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	if( pl_w->dir & 1 ){
		pl_w->pl_dir = pl_w->disp_dir = pl_w->pl_dir ^ 1 ;
		em = pl_w->catch_w ;
		em->pl_dir = em->disp_dir = em->pl_dir ^ 1 ;
	}
	ofs.x = -DOT_X( 64.0f ) ;
	ofs.y = DOT_Y( 96.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	ef_color_set( pl_w, 1, 5, 1 ) ;
	pl_w->catch_w->dm_elem = 3 ;
	pl_w->catch_w->unk_228 = 3 ;
	dm00_04_set( pl_w->catch_w ) ;
	pls35_00_set( pl_w, 1 ) ;
	pl_char_set( pl_w, Char_Catch, 2 ) ;
}

/*==============================================================*/
static void jump_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	if( pl_w->dir & 1 ){
		pl_w->pl_dir = pl_w->disp_dir = pl_w->pl_dir ^ 1 ;
		em = pl_w->catch_w ;
		em->pl_dir = em->disp_dir = em->pl_dir ^ 1 ;
	}
	ofs.x = -DOT_X( 40.0f ) ;
	ofs.y = DOT_Y( 120.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	ef_color_set( pl_w, 1, 5, 1 ) ;
	pl_w->catch_w->dm_elem = 3 ;
	pl_w->catch_w->unk_228 = 3 ;
	dm00_04_set( pl_w->catch_w ) ;
	pl_w->hit_stop = 10 ;
	pl_char_set( pl_w, Char_Catch, 4 ) ;
}

static void nml_p_catch( PLAYER_WORK *pl_w );
static void nml_k_catch( PLAYER_WORK *pl_w );
static void jump_catch( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void pl35_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_catch, nml_k_catch, jump_catch,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

static void p_catch_start( PLAYER_WORK *pl_w );
static void p_catch_wait( PLAYER_WORK *pl_w );
static void p_catch_throw( PLAYER_WORK *pl_w );
static void p_catch_color( PLAYER_WORK *pl_w );

/*==============================================================*/
static void nml_p_catch( PLAYER_WORK *pl_w )
{
	static void ( *nml_p_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		p_catch_start, p_catch_wait, p_catch_throw, stand_end,
	} ;

	nml_p_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void p_catch_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pls35_00_set( pl_w, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void p_catch_wait( PLAYER_WORK *pl_w )
{
	p_catch_color( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Catch, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void p_catch_throw( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	p_catch_color( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->pl_dir = pl_w->pl_dir ^ 1 ;
		em->atck_no = pl35_at_throw_p ;
		em->catch_type = 1 ;
		SCR_EXCEP_SET ;
		pl_color_set( em, PC_DEFAULT ) ;
	}
}

/*==============================================================*/
/*			The opponent's colour while he is held				*/
/*==============================================================*/
static void p_catch_color( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->catch_w ;

	if( Sys->game_timer0 & 1 ) pl_color_set( em, 1 ) ;
	else pl_color_set( em, 8 ) ;
}

static void k_catch_wait( PLAYER_WORK *pl_w );
static void k_catch_end( PLAYER_WORK *pl_w );
static void k_catch_color( PLAYER_WORK *pl_w );

/*==============================================================*/
static void nml_k_catch( PLAYER_WORK *pl_w )
{
	static void ( *nml_k_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		k_catch_wait, k_catch_end, stand_end,
	} ;

	nml_k_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void k_catch_wait( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_w->catch_w->dm_elem = 3 ;
	pl_w->catch_w->unk_228 = 3 ;
	k_catch_color( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type == 1 ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->pl_dir = pl_w->pl_dir ^ 1 ;
		em->atck_no = pl35_at_throw_k ;
		em->catch_type = 10 ;
		SCR_EXCEP_SET ;
		pl_color_set( em, 1 ) ;
		pls35_00_set( pl_w, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void k_catch_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Catch, 3 ) ;
	}
}

/*==============================================================*/
static void k_catch_color( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->catch_w ;

	if( Sys->game_timer0 & 1 ) pl_color_set( em, 5 ) ;
	else pl_color_set( em, 4 ) ;
}

static void j_catch_wait( PLAYER_WORK *pl_w );
static void j_catch_end( PLAYER_WORK *pl_w );
static void j_catch_land( PLAYER_WORK *pl_w );

/*==============================================================*/
static void jump_catch( PLAYER_WORK *pl_w )
{
	static void ( *jump_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		j_catch_wait, j_catch_end, j_catch_land,
	} ;

	jump_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void j_catch_wait( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_w->catch_w->dm_elem = 3 ;
	pl_w->catch_w->unk_228 = 3 ;
	k_catch_color( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->pl_dir = pl_w->pl_dir ^ 1 ;
		em->atck_no = pl35_at_air_throw ;
		em->catch_type = 10 ;
		SCR_EXCEP_SET ;
		pl_color_set( em, 1 ) ;
		pls35_00_set( pl_w, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void j_catch_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Catch, 5 ) ;
	}
}

/*--------------------------------------------------------------*/
static void j_catch_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

static void hold( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Holding the opponent								*/
/****************************************************************/
void pl35_hold( PLAYER_WORK *pl_w )
{
	static void ( *hold_jmp[])( PLAYER_WORK *pl_w ) = {
		hold, hold, hold,
	} ;

	hold_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void hold( PLAYER_WORK *pl_w )
{
	catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/*==============================================================*/
/*			The heavy attacks' effects start over				*/
/*==============================================================*/
static void shot_init( PLAYER_WORK *pl_w )
{
	PL35_LOCAL_W *lw = ( PL35_LOCAL_W * )pl_w->tlocal ;

	lw->unk_06 = -1 ;
	lw->shot_no ++ ;
	lw->free_cnt = 0 ;
	lw->hold_cnt = 0 ;
	lw->hold_timer = 0 ;
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void pl35_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl35_sp_judgement ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl35_sp_armageddon ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl35_sp_heart ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl35_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl35_sp_judgement ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl35_sp_armageddon ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl35_sp_heart ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl35_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl35_sp_thunder ;
		pl_w->atck_level = 0 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl35_sp_inferno ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl35_sp_gamma ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void pl35_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl35_sp_thunder ;
		pl_w->atck_level = 0 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl35_sp_inferno ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl35_sp_gamma ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl35.c" End of File	*/

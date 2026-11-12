/* Cable (character 17): command checks, normal attacks, appearance, win poses, special moves and
   hyper combos, throws and the partner moves. */
#define PL_GAUGE_ADD_OLD
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl17_def.h"

void pl17_main( PLAYER_WORK *pl_w );
void pl17_com_ck( PLAYER_WORK *pl_w );
void pl17_atck_set( PLAYER_WORK *pl_w );
void pl17_atck( PLAYER_WORK *pl_w );
void pl17_dash( PLAYER_WORK *pl_w );
void pl17_noki( PLAYER_WORK *pl_w );
void pl17_appear( PLAYER_WORK *pl_w );
void pl17_win( PLAYER_WORK *pl_w );
void pl17_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl17_catch_ck( PLAYER_WORK *pl_w );
void pl17_catch_set( PLAYER_WORK *pl_w );
void pl17_catch( PLAYER_WORK *pl_w );
void pl17_hold( PLAYER_WORK *pl_w );
int pl17_delay_ck( PLAYER_WORK *pl_w );
void pl17_t_kick( PLAYER_WORK *pl_w );
void pl17_upper( PLAYER_WORK *pl_w );
void pl17_sp_unison( PLAYER_WORK *pl_w );
void pl17_sp_gattai( PLAYER_WORK *pl_w );
void pl17_sp_assist( PLAYER_WORK *pl_w );
void pl17_sp_zero( PLAYER_WORK *pl_w );

/* commands: time or steps, kind, the special move, then the stick and buttons */
static Uint16 com_hviper[] = { 3, COM_HYPER | COM_ALL_SW, pl17_sp_hviper, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Hyper Viper Beam */
static Uint16 com_timeflip[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl17_sp_timeflip, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Time Flip (hyper) */
static Uint16 com_psimitar[] = { 3, COM_GROUND, pl17_sp_psimitar, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_PUNCH } ;	/* Psimitar */
static Uint16 com_viper[] = { 3, 0, pl17_sp_viper, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Viper Beam */
/* not read */
static Uint16 com_viper_0[] = { 2, 10, pl17_sp_viper, 0, KEY_PUNCH } ;	/* Viper Beam */
static Uint16 com_viper_1[] = { 4, 10, pl17_sp_viper, 0, KEY_PUNCH } ;	/* Viper Beam */
static Uint16 com_viper_2[] = { 6, 10, pl17_sp_viper, 0, KEY_PUNCH } ;	/* Viper Beam */
static Uint16 com_crackdown[] = { 3, COM_GROUND, pl17_sp_crackdown, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Crackdown */
static Uint16 com_electrap[] = { 3, 0, pl17_sp_electrap, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Electrap */
static Uint16 com_psycharge[] = { 5, COM_GROUND, pl17_sp_psycharge, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Psy-Charge */

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

static void ( *pl17_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl17_main,
	pl17_com_ck,
	pl17_atck_set,
	pl17_atck,
	pl17_dash,
	pl17_noki,
	pl17_appear,
	pl17_win,
	pl17_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl17_catch_ck,	/* returns a value */
	pl17_catch_set,
	pl17_catch,
	pl17_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl17_delay_ck,
	pl17_t_kick,
	pl17_upper,
	pl17_sp_unison,
	pl17_sp_gattai,
	pl17_sp_assist,
	pl17_sp_zero,
} ;

static Uint8 hviper_ck( PLAYER_WORK *pl_w );
static Uint8 timeflip_ck( PLAYER_WORK *pl_w );
static Uint8 psycharge_ck( PLAYER_WORK *pl_w );
static Uint8 psimitar_ck( PLAYER_WORK *pl_w );
static Uint8 viper_ck( PLAYER_WORK *pl_w );
static Uint8 crackdown_ck( PLAYER_WORK *pl_w );
static Uint8 electrap_ck( PLAYER_WORK *pl_w );
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static Uint8 jump_com_ck( PLAYER_WORK *pl_w );
static int hviper_delay_ck( PLAYER_WORK *pl_w );
static int timeflip_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl17_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl17_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl17_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( hviper_ck( pl_w ) ) return ;
	if( timeflip_ck( pl_w ) ) return ;
	if( psycharge_ck( pl_w ) ) return ;
	if( psimitar_ck( pl_w ) ) return ;
	if( viper_ck( pl_w ) ) return ;
	if( crackdown_ck( pl_w ) ) return ;
	if( electrap_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
static Uint8 hviper_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_hviper, &pl_w->com_w ) ) return 0 ;
	if( !jump_com_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( PL17_LW( pl_w )->timeflip_shot_flag[0] ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl17_sp_hviper ;
	act_st_set( pl_w, N_SUPER ) ;
	PL17_LW( pl_w )->beam_lv[0] = 0 ;
	PL17_LW( pl_w )->beam_time[0] = 0 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 timeflip_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_timeflip, &pl_w->com_w0 ) ) return 0 ;
	if( !jump_com_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( PL17_LW( pl_w )->timeflip_shot_flag[0] ) return 0 ;
	if( PL17_LW( pl_w )->timeflip_wait_timer[0] ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl17_sp_timeflip ;
	act_st_set( pl_w, N_SUPER ) ;
	PL17_LW( pl_w )->timeflip_wait_timer[0] = 1 ;
	pl_w->team_ng_timer = 2 ;
	PL17_LW( pl_w )->timeflip_catch_flag[0] = 0 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 psimitar_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_psimitar, &pl_w->com_w1 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl17_sp_psimitar ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 viper_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_viper, &pl_w->com_w2 ) ) return 0 ;
	if( !jump_com_ck( pl_w ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl17_sp_viper ;
	act_st_set( pl_w, N_SPECIAL ) ;
	PL17_LW( pl_w )->beam_lv[0] = 0 ;
	PL17_LW( pl_w )->beam_time[0] = 0 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 crackdown_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_crackdown, &pl_w->com_w3 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w3 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl17_sp_crackdown ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 electrap_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_electrap, &pl_w->com_w4 ) ) return 0 ;
	if( !jump_com_ck( pl_w ) ) return 0 ;
	if( PL17_LW( pl_w )->grenade_flag[0] ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w4 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl17_sp_electrap ;
	act_st_set( pl_w, N_SPECIAL ) ;
	PL17_LW( pl_w )->sw_hold[0] = pl_w->pl_new_lvbt ;
	PL17_LW( pl_w )->sw_hold[0] &= 0x60 ;
	PL17_LW( pl_w )->sw_off_flag[0] = 0 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 7 ) ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl17_sp_chouhatsu ;
	act_st_set( pl_w, N_SPECIAL ) ;
	PL17_LW( pl_w )->start_cnt[0] = 0 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 psycharge_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_psycharge, &pl_w->com_w5 ) ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl17_sp_psycharge ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			In the air, once a jump unless super jumping		*/
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
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl17_delay_ck( PLAYER_WORK *pl_w )
{
	if( hviper_delay_ck( pl_w ) || timeflip_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static int hviper_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_hviper, &pl_w->com_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl17_sp_hviper ;
	return 1 ;
}

/*==============================================================*/
static int timeflip_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_timeflip, &pl_w->com_w0 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl17_sp_timeflip ;
	return 1 ;
}

/*==============================================================*/
static int snapback_ck( PLAYER_WORK *pl_w )
{
	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl17_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/****************************************************************/
void pl17_main( PLAYER_WORK *pl_w )
{
	PL17_LW( pl_w )->sw_hold[0] &= pl_w->pl_new_lvbt ;
	if( PL17_LW( pl_w )->timeflip_wait_timer[0] ){
		if( PL17_LW( pl_w )->timeflip_wait_timer[0] < 0 ) PL17_LW( pl_w )->timeflip_wait_timer[0] = 0 ;
		else PL17_LW( pl_w )->timeflip_wait_timer[0] -- ;
	}
}

static void nml_atck( PLAYER_WORK *pl_w );
static void jump_atck_ck( PLAYER_WORK *pl_w );
static void jump_atck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl17_atck_set( PLAYER_WORK *pl_w )
{
	static void ( *atck_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_atck, jump_atck_ck, jump_atck, nml_atck,
	} ;

	atck_set_jmp[pl_w->atck_type]( pl_w ) ;
}

static void punch_s( PLAYER_WORK *pl_w );
static void punch_c( PLAYER_WORK *pl_w );
static void kick_s( PLAYER_WORK *pl_w );
static void kick_c( PLAYER_WORK *pl_w );
static void jump_punch( PLAYER_WORK *pl_w );
static void jump_kick( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Normal attacks										*/
/*==============================================================*/
static void nml_atck( PLAYER_WORK *pl_w )
{
	pl_brake_set( pl_w ) ;
	if( pl_w->atck_pk == 1 ){
		if( pl_w->pl_stance == STANCE_CROUCH ) kick_c( pl_w ) ;
		else kick_s( pl_w ) ;
	} else {
		if( pl_w->pl_stance == STANCE_CROUCH ) punch_c( pl_w ) ;
		else punch_s( pl_w ) ;
	}
}

/*==============================================================*/
static void punch_s( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_STAND_LP ;
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		pl_w->unk_2a2 = 1 ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_STAND_MP ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_STAND_HP ;
		pl_w->chain_tbl = hp_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Stand_P, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
static void punch_c( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_CROUCH_LP ;
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
		pl_w->chain_tbl = hp_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Crouch_P, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
/*			The heavy kick with the stick forward is another	*/
/*			attack												*/
/*==============================================================*/
static void kick_s( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_STAND_LK ;
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
		if( pl_w->atck_lever & KEY_FORWARD ){
			pl_w->char_no.b.no = 3 ;
			pl_w->atck_no = pl17_at_fwd_hk ;
			pl_w->chain_tbl = hk_tbl ;
			pl_w->nml_level = 2 ;
		} else {
			pl_w->char_no.b.no = 2 ;
			pl_w->atck_no = ATCK_STAND_HK ;
			pl_w->chain_tbl = hk_tbl ;
			pl_w->nml_level = 2 ;
		}
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Stand_K, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
static void kick_c( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_CROUCH_LK ;
		pl_w->chain_tbl = lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_CROUCH_MK ;
		pl_w->chain_tbl = mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_CROUCH_HK ;
		pl_w->chain_tbl = hk_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Crouch_K, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
/*			In a jump only while that button's attacks are		*/
/*			left												*/
/*==============================================================*/
static void jump_atck_ck( PLAYER_WORK *pl_w )
{
	if( ( pl_w->atck_pk == 0 && ( pl_w->jump_atk_cnt & JATK_P_MASK ) ) || ( pl_w->atck_pk != 0 && ( pl_w->jump_atk_cnt & JATK_K_MASK ) ) ) jump_atck( pl_w ) ;
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
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_lk_tbl ;
		else pl_w->chain_tbl = sj_lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_JUMP_MK ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mk_tbl ;
		else pl_w->chain_tbl = sj_mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_JUMP_HK ;
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
static void sjump_move( PLAYER_WORK *pl_w );
static void jump_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl17_atck( PLAYER_WORK *pl_w )
{
	static void ( *atck_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_move, jump_move, sjump_move, nml_move_dash,
	} ;

	atck_jmp[pl_w->atck_type]( pl_w ) ;
}

static void punch_s_move( PLAYER_WORK *pl_w );
static void punch_c_move( PLAYER_WORK *pl_w );
static void kick_s_move( PLAYER_WORK *pl_w );
static void kick_c_move( PLAYER_WORK *pl_w );

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
		if( pl_w->pl_stance == STANCE_CROUCH ) kick_c_move( pl_w ) ;
		else kick_s_move( pl_w ) ;
	} else {
		if( pl_w->pl_stance == STANCE_CROUCH ) punch_c_move( pl_w ) ;
		else punch_s_move( pl_w ) ;
	}
}

static void hp_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void punch_s_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	case 2:
		hp_move( pl_w ) ;
		break ;
	}
}

static void hp_init( PLAYER_WORK *pl_w );
static void hp_loop( PLAYER_WORK *pl_w );
static void hp_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The standing heavy punch: he keeps firing while		*/
/*			the button is pressed								*/
/*==============================================================*/
static void hp_move( PLAYER_WORK *pl_w )
{
	static void ( *hp_jmp[])( PLAYER_WORK *pl_w ) = {
		hp_init, hp_loop, hp_end,
	} ;

	hp_jmp[pl_w->routine_3]( pl_w ) ;
}

static int hp_btn_ck( PLAYER_WORK *pl_w );

/* the frames to the next shot */
static Sint16 hp_time_tbl[5] = { 8, 15, 16, 31, 32 } ;

/*==============================================================*/
static void hp_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 = 1 ;
	PL17_LW( pl_w )->hp_btn[0] = 0 ;
	PL17_LW( pl_w )->hp_shot[0] = 0 ;
	pl_w->timer0 = 0 ;
	hp_loop( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void hp_loop( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type == 5 ){
		if( -- pl_w->timer0 < 0 ) pl_stand_ret( pl_w ) ;
	} else {
		PL17_LW( pl_w )->hp_btn[0] += hp_btn_ck( pl_w ) ;
		if( pl_w->cg_type == 2 ){
			pl_w->cg_type = 0 ;
			shl17_06_set( pl_w, 0 ) ;
			pls17_06_set( pl_w, 0 ) ;
			pl_se_req( pl_w, 36 ) ;
		}
		if( pl_w->cg_type == 3 ){
			pl_w->cg_type = 0 ;
			pls17_06_set( pl_w, 3 ) ;
		}
		if( pl_w->cg_type == 4 ){
			pl_w->cg_type = 0 ;
			if( PL17_LW( pl_w )->hp_shot[0] < 3 ){
				if( pl_w->cpu_flag ){
					if( Rnd() & 1 ) PL17_LW( pl_w )->hp_btn[0] = 1 ;
					else PL17_LW( pl_w )->hp_btn[0] = 0 ;
				}
				if( PL17_LW( pl_w )->hp_btn[0] >= 1 ){
					pl_char_set( pl_w, Char_Stand_P, 3 ) ;
					PL17_LW( pl_w )->hp_shot[0] ++ ;
				}
				PL17_LW( pl_w )->hp_btn[0] = 0 ;
			} else pl_w->routine_3 ++ ;
			pl_w->timer0 = hp_time_tbl[PL17_LW( pl_w )->hp_shot[0]] ;
		}
	}
}

/*--------------------------------------------------------------*/
static void hp_end( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type == 5 && -- pl_w->timer0 < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			The heavy punch button pressed this frame			*/
/*==============================================================*/
static int hp_btn_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->pl_now_lvbt & KEY_HP ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static void punch_c_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	else if( pl_w->nml_no == 2 ){
		if( pl_w->cg_type == 2 ){
			pl_w->cg_type = 0 ;
			shl17_06_set( pl_w, 1 ) ;
			pls17_06_set( pl_w, 1 ) ;
		}
		if( pl_w->cg_type == 3 ){
			pl_w->cg_type = 0 ;
			pls17_06_set( pl_w, 4 ) ;
		}
	}
}

/*==============================================================*/
/*			The heavy kick with the stick forward steps in		*/
/*==============================================================*/
static void kick_s_move( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	switch( pl_w->nml_no ){
	case 0:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	case 2:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		else if( pl_w->char_no.b.no == 3 ){
			switch( pl_w->cg_type ){
			case 1:
				pl_w->spd.x = 0.0f ;
				pl_w->spd.y = 0.0f ;
				pl_w->acc.x = 0.0f ;
				pl_w->acc.y = 0.0f ;
				break ;
			case 2:
				pl_w->spd.x = pl_w->disp_dir ? 2.0f : -2.0f ;
				pl_w->cg_type = 0 ;
				break ;
			case 3:
				ofs.x = 52.0f ;
				pl_w->spd.x = pl_w->disp_dir ? 0.4f : -0.4f ;
				pl_w->cg_type = 0 ;
				dust_ofs_set( pl_w, &ofs, 1.0f, 1.0f ) ;
				break ;
			case 4:
				pl_w->spd.x = 0.0f ;
				pl_w->spd.y = 0.0f ;
				pl_w->acc.x = 0.0f ;
				pl_w->acc.y = 0.0f ;
				break ;
			}
			pl_w->pos.x += pl_w->spd.x ;
			pl_w->spd.x += pl_w->acc.x ;
		}
		break ;
	}
}

/*==============================================================*/
static void kick_c_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
	case 1:
	case 2:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	}
}

static void jump_punch_move( PLAYER_WORK *pl_w );
static void jump_kick_move( PLAYER_WORK *pl_w );

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
/*			The jumping heavy punch fires down and holds him	*/
/*			in the air											*/
/*==============================================================*/
static void jump_punch_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
	else if( pl_w->nml_no == 2 ){
		if( pl_w->cg_type == 2 ){
			pl_w->cg_type = 0 ;
			shl17_06_set( pl_w, 2 ) ;
			pls17_06_set( pl_w, 2 ) ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_w->spd.y = DOT_X( 5.0f ) ;
			pl_w->acc.y = -1.5625f ;
		}
		if( pl_w->cg_type == 3 ){
			pl_w->cg_type = 0 ;
			pls17_06_set( pl_w, 5 ) ;
		}
	}
}

/*==============================================================*/
static void jump_kick_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/****************************************************************/
/*			The launcher										*/
/****************************************************************/
void pl17_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		if( pl_w->atck_pk == 0 ){
			pl_w->atck_no = pl17_at_upper ;
			pl_w->pl_stance = STANCE_STAND ;
			pl_char_set( pl_w, Char_Extra, 5 ) ;
		}
		HIT_RESET( pl_w ) ;
		pl_se_req( pl_w, SE_SWING_M ) ;
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
void pl17_dash( PLAYER_WORK *pl_w )
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
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	if( pl_w->cg_type ) return ;
	dash_ef_set( pl_w ) ;
	pl_w->routine_2 ++ ;
	pl_w->spd.x = pl_w->pl_dir ? DOT_X( 11.0f ) : -DOT_X( 11.0f ) ;
	pl_w->acc.x = pl_w->pl_dir ? -DOT_X( 0.375f ) : DOT_X( 0.375f ) ;
	pl_w->timer0 = 12 ;
	dash_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void dash_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_w->spd.x * pl_w->acc.x >= 0.0f ) RATE_CLR_X( pl_w ) ;
	if( !--pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->char_no.b.no = 2 ;
		pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
	}
}

/*--------------------------------------------------------------*/
static void dash_end( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_w->spd.x * pl_w->acc.x >= 0.0f ) RATE_CLR_X( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		RATE_CLR_X( pl_w ) ;
		pl_stand_ret( pl_w ) ;
	}
}

static void noki_init( PLAYER_WORK *pl_w );
static void noki_move( PLAYER_WORK *pl_w );
static void noki_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash										*/
/****************************************************************/
void pl17_noki( PLAYER_WORK *pl_w )
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
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->routine_2 ++ ;
	pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 5.5f ) : DOT_X( 5.5f ) ;
	pl_w->timer0 = 8 ;
	noki_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void noki_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !--pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->acc.x = pl_w->pl_dir ? DOT_X( 0.125f ) : -DOT_X( 0.125f ) ;
		pl_w->char_no.b.no = 3 ;
		pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
	}
}

/*--------------------------------------------------------------*/
static void noki_end( PLAYER_WORK *pl_w )
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

static void appear_init( PLAYER_WORK *pl_w );
static void appear_move( PLAYER_WORK *pl_w );
static void appear_00( PLAYER_WORK *pl_w );
static void appear_01( PLAYER_WORK *pl_w );
static void app00_init( PLAYER_WORK *pl_w );
static void app00_move( PLAYER_WORK *pl_w );
static void app01_init( PLAYER_WORK *pl_w );
static void app01_fade( PLAYER_WORK *pl_w );
static void app01_fall( PLAYER_WORK *pl_w );
static void app01_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The entrance: one of two, chosen at random			*/
/****************************************************************/
void pl17_appear( PLAYER_WORK *pl_w )
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
	pl_w->routine_3 = Rnd() ;
	pl_w->routine_3 &= 1 ;
	appear_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void appear_move( PLAYER_WORK *pl_w )
{
	static void ( *appear_move_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_00, appear_01,
	} ;

	appear_move_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void appear_00( PLAYER_WORK *pl_w )
{
	static void ( *app00_jmp[])( PLAYER_WORK *pl_w ) = {
		app00_init, app00_move,
	} ;

	app00_jmp[pl_w->type]( pl_w ) ;
}

/*==============================================================*/
static void app00_init( PLAYER_WORK *pl_w )
{
	pl_w->type ++ ;
	pl_w->disp_flag = 1 ;
	pl_char_set( pl_w, Char_Appear, 0 ) ;
	pl_voice_req( pl_w, 10 ) ;
}

/*==============================================================*/
static void app00_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_w->routine_1 ++ ;
}

/*==============================================================*/
/*			He fades in high up and drops to the ground			*/
/*==============================================================*/
static void appear_01( PLAYER_WORK *pl_w )
{
	static void ( *app01_jmp[])( PLAYER_WORK *pl_w ) = {
		app01_init, app01_fade, app01_fall, app01_land,
	} ;

	app01_jmp[pl_w->type]( pl_w ) ;
}

/*==============================================================*/
static void app01_init( PLAYER_WORK *pl_w )
{
	pl_w->type ++ ;
	pl_w->disp_flag = 1 ;
	pl_char_set( pl_w, Char_Appear, 1 ) ;
	pl_w->alpha = 0.2f ;
	pl_w->pos.y += 100.0f ;
	pl_w->pl_stance = STANCE_AIR ;
	pls17_04_set( pl_w, 4 ) ;
}

/*==============================================================*/
static void app01_fade( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type == 2 ){
		pl_w->type ++ ;
		pl_w->alpha = 1.0f ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
	}
	if( pl_w->cg_type == 3 ) pl_w->alpha += 0.016f ;
}

/*==============================================================*/
static void app01_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->type ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_char_set( pl_w, Char_Jump, 3 ) ;
		land_ef_set( &pl_w->pos, 0 ) ;
	}
}

/*==============================================================*/
static void app01_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_1 ++ ;
		pl_w->alpha = 1.0f ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
	}
}

static void win_00( PLAYER_WORK *pl_w );
static void win_01( PLAYER_WORK *pl_w );
static void win_02( PLAYER_WORK *pl_w );
static void win00_init( PLAYER_WORK *pl_w );
static void win00_move( PLAYER_WORK *pl_w );
static void win01_init( PLAYER_WORK *pl_w );
static void win01_move( PLAYER_WORK *pl_w );
static void win02_init( PLAYER_WORK *pl_w );
static void win02_move( PLAYER_WORK *pl_w );
static void win_type_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl17_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_01,
		win_00, win_00,
		win_02, win_02,
		win_01, win_01, win_01,
		win_00, win_00, win_00,
	} ;

	if( pl_win_end_ck( pl_w ) ){
		pl_stand_ret( pl_w ) ;
	}else{
		if( !pl_w->type ) win_type_ck( pl_w ) ;
		win_jmp[pl_w->type]( pl_w ) ;
	}
}

/*==============================================================*/
static void win_00( PLAYER_WORK *pl_w )
{
	static void ( *win00_jmp[])( PLAYER_WORK *pl_w ) = {
		win00_init, win00_move,
	} ;

	win00_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void win00_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_char_set( pl_w, Char_Win, 0 ) ;
	win00_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win00_move( PLAYER_WORK *pl_w )
{
	if( pl_win_end_ck( pl_w ) ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_voice_req( pl_w, 16 ) ;
	}
}

/*==============================================================*/
static void win_01( PLAYER_WORK *pl_w )
{
	static void ( *win01_jmp[])( PLAYER_WORK *pl_w ) = {
		win01_init, win01_move,
	} ;

	win01_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void win01_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_char_set( pl_w, Char_Win, 1 ) ;
	win01_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win01_move( PLAYER_WORK *pl_w )
{
	if( pl_win_end_ck( pl_w ) ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void win_02( PLAYER_WORK *pl_w )
{
	static void ( *win02_jmp[])( PLAYER_WORK *pl_w ) = {
		win02_init, win02_move,
	} ;

	win02_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void win02_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_char_set( pl_w, Char_Win, 2 ) ;
	win02_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win02_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

/* the win pose for a button held at the end, or one at random */
static Sint16 win_type_tbl[16] = { 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10 } ;

/*==============================================================*/
static void win_type_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->pad_sw.now & KEY_BUTTON ){
		if( pl_w->pl_new_lvbt & KEY_LP ) pl_w->type = 5 ;
		else if( pl_w->pl_new_lvbt & KEY_HP ) pl_w->type = 6 ;
		else if( pl_w->pl_new_lvbt & KEY_A1 ) pl_w->type = 7 ;
		else if( pl_w->pl_new_lvbt & KEY_LK ) pl_w->type = 8 ;
		else if( pl_w->pl_new_lvbt & KEY_HK ) pl_w->type = 9 ;
		else pl_w->type = 10 ;
	} else pl_w->type = win_type_tbl[Rnd() & 0x0f] ;
}

static void sp_hviper( PLAYER_WORK *pl_w );
static void sp_timeflip( PLAYER_WORK *pl_w );
static void sp_psycharge( PLAYER_WORK *pl_w );
static void sp_psimitar( PLAYER_WORK *pl_w );
static void sp_viper( PLAYER_WORK *pl_w );
static void sp_crackdown( PLAYER_WORK *pl_w );
static void sp_electrap( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );

/****************************************************************/
void pl17_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *pl_sp_tbl[])( PLAYER_WORK *pl_w ) = {
		sp_hviper, sp_timeflip, sp_psycharge, sp_psimitar, sp_viper, sp_crackdown, sp_electrap, sp_chouhatsu, sp_snapback,
	} ;

	pl_sp_tbl[pl_w->pl_sp_type]( pl_w ) ;
}

/*==============================================================*/
/*			Lands him if he has reached the ground				*/
/*==============================================================*/
static Uint8 land_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pos.y = pl_w->ground_y ;
		return 1 ;
	}
	return 0 ;
}

/*==============================================================*/
/*			A special move starts standing on the ground		*/
/*==============================================================*/
static void sp_stand_set( PLAYER_WORK *pl_w )
{
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 1 ) ;
	pl_sp_ef_set( pl_w ) ;
}

static void beam_lv_ck( PLAYER_WORK *pl_w );
static void hviper_init( PLAYER_WORK *pl_w );
static void hviper_move( PLAYER_WORK *pl_w );
static void hviper_wait( PLAYER_WORK *pl_w );
static void hviper_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_hviper( PLAYER_WORK *pl_w )
{
	static void ( *hviper_jmp[])( PLAYER_WORK *pl_w ) = {
		hviper_init, hviper_move, hviper_wait, hviper_end,
	} ;

	beam_lv_ck( pl_w ) ;
	hviper_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void hviper_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	if( pl_w->pl_stance == STANCE_AIR ){
		pl_w->char_no.b.no = 3 ;
		pl_dir_set( pl_w ) ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pls17_00_set3( pl_w, 1 ) ;
	} else {
		sp_stand_set( pl_w ) ;
		pl_w->char_no.b.no = 0 ;
		pls17_00_set3( pl_w, 0 ) ;
	}
	pl_char_set( pl_w, Char_Super, pl_w->char_no.b.no ) ;
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	PL17_LW( pl_w )->beam_hit_cnt[0] = 0 ;
	/* from the Japanese disc on, a move starts with its own attack set, not the one before it */
#if BUILD_DATE >= 20000303
	pl_w->atck_no = pl17_at_hviper ;
	HIT_RESET( pl_w ) ;
#endif
	hviper_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void hviper_move( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	land_ck( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		if( pl_w->pl_stance == STANCE_AIR ){
			pl_w->char_no.b.no = 4 ;
			pls17_01_set( pl_w, 1 ) ;
		} else {
			pl_w->pl_stance = STANCE_STAND ;
			pl_w->char_no.b.no = 1 ;
			pls17_01_set( pl_w, 0 ) ;
		}
		pl_char_set( pl_w, Char_Super, pl_w->char_no.b.no ) ;
		shl17_03_set( pl_w ) ;
		pl_w->timer1 = 0x87 ;
		return ;
	}
	if( pl_w->cg_type == 1 ){
		pl_w->cg_type = 0 ;
		ofs.x = -104.0f ;
		ofs.y = 255.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void hviper_wait( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	land_ck( pl_w ) ;
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( --pl_w->timer1 < 0 ){
		pl_w->routine_2 ++ ;
		if( pl_w->pl_stance == STANCE_AIR ){
			pl_w->char_no.b.no = 5 ;
			pl_w->acc.y = -DOT_Y( 0.375f ) ;
		} else pl_w->char_no.b.no = 2 ;
		pl_char_set( pl_w, Char_Super, pl_w->char_no.b.no ) ;
	}
}

/*--------------------------------------------------------------*/
static void hviper_end( PLAYER_WORK *pl_w )
{
	pl_w->delay_flag = 0 ;
	pl_w->delay_timer = 0 ;
	pl_w->hyper_bg = 0 ;
	pl_w->hyper_bg_tm = 0 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pl_stance != STANCE_AIR ){
		land_ck( pl_w ) ;
		if( pl_char_move( pl_w ) < 0 ){
			pl_stand_ret( pl_w ) ;
			pl_w->work2 = 0 ;
		}
	} else {
		pl_char_move( pl_w ) ;
		if( land_ck( pl_w ) ){
			pl_jump_ret( pl_w ) ;
			pl_w->work2 = 0 ;
		}
	}
}

static void timeflip_init( PLAYER_WORK *pl_w );
static void timeflip_move( PLAYER_WORK *pl_w );
static void timeflip_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_timeflip( PLAYER_WORK *pl_w )
{
	static void ( *timeflip_jmp[])( PLAYER_WORK *pl_w ) = {
		timeflip_init, timeflip_move, timeflip_end,
	} ;

	timeflip_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void timeflip_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_gauge_add( pl_w, 0 ) ;
	sp_stand_set( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 6 ) ;
	pl_w->atck_no = pl17_at_timeflip ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_voice_req( pl_w, 23 ) ;
	PL17_LW( pl_w )->timeflip_fly_flag[0] = 1 ;
}

/*--------------------------------------------------------------*/
static void timeflip_move( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( !PL17_LW( pl_w )->timeflip_fly_flag[0] ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type == 2 ){
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->cg_type = 0 ;
	}
	if( pl_w->cg_type == 1 ){
		pl_w->cg_type = 0 ;
		pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
		ofs.x = -DOT_X( 10.0f ) ;
		ofs.y = 210.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
		pls17_04_set( pl_w, 0 ) ;
		pls17_04_set( pl_w, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void timeflip_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void psycharge_init( PLAYER_WORK *pl_w );
static void psycharge_move( PLAYER_WORK *pl_w );
static void psycharge_end( PLAYER_WORK *pl_w );
static void psycharge_hit( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A throw												*/
/*==============================================================*/
static void sp_psycharge( PLAYER_WORK *pl_w )
{
	static void ( *psycharge_jmp[])( PLAYER_WORK *pl_w ) = {
		psycharge_init, psycharge_move, psycharge_end,
	} ;

	psycharge_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void psycharge_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = 1 ;
	sp_stand_set( pl_w ) ;
	pl_w->atck_no = pl17_at_psycharge ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_char_set( pl_w, Char_Special, 21 ) ;
	psycharge_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void psycharge_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_voice_req( pl_w, 3 ) ;
		pl_w->routine_2 = 2 ;
		pl_char_set( pl_w, Char_Special, 22 ) ;
		return ;
	}
	if( pl_w->hit_flag && pl_sp_catch_ck( pl_w ) ){
		pl_voice_req( pl_w, 5 ) ;
		psycharge_hit( pl_w ) ;
	}
}

/*==============================================================*/
/*			The opponent is caught								*/
/*==============================================================*/
static void psycharge_hit( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w = pl_w->em_w ;

	pl_w->catch_no = pl_w->target_w->catch_no = 0xc2 ;
	pl_catch_set( pl_w, em_w ) ;
}

/*--------------------------------------------------------------*/
static void psycharge_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	else if( pl_w->cg_type == 2 ){
		pl_w->cg_type = 0 ;
		pls17_03_set( pl_w, 1 ) ;
	}
}

/* the jump speeds and gravities of sp_psimitar, x and y for each strength of button */
static const Float psimitar_spd[4] = { DOT_X( 8.0f ), DOT_Y( 10.0f ), 20.0f, DOT_Y( 16.0f ) } ;
static const Float psimitar_acc[4] = { -0.72916663f, -DOT_Y( 0.75f ), -0.72916663f, -DOT_Y( 0.75f ) } ;

static void psimitar_init( PLAYER_WORK *pl_w );
static void psimitar_move( PLAYER_WORK *pl_w );
static void psimitar_rise( PLAYER_WORK *pl_w );
static void psimitar_top( PLAYER_WORK *pl_w );
static void psimitar_fall( PLAYER_WORK *pl_w );
static void psimitar_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_psimitar( PLAYER_WORK *pl_w )
{
	static void ( *psimitar_jmp[])( PLAYER_WORK *pl_w ) = {
		psimitar_init, psimitar_move, psimitar_rise, psimitar_top, psimitar_fall, psimitar_end,
	} ;

	psimitar_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void psimitar_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	if( pl_w->team_type != 3 ) pl_gauge_add( pl_w, 5 ) ;
	sp_stand_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 10 ) ;
	pl_voice_req( pl_w, 21 ) ;
#if BUILD_DATE >= 20000303	/* the attack, as in hviper_init */
	if( pl_w->atck_level ){
		pl_w->atck_no = pl17_at_psimitar_h_2 ;
		HIT_RESET( pl_w ) ;
	} else {
		pl_w->atck_no = pl17_at_psimitar_l_2 ;
		HIT_RESET( pl_w ) ;
	}
#endif
	psimitar_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void psimitar_move( PLAYER_WORK *pl_w )
{
	const Float *spd = psimitar_spd ;
	const Float *acc = psimitar_acc ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_air_set( pl_w ) ;
		pl_w->spd.x = pl_w->disp_dir ? spd[pl_w->atck_level * 2] : -spd[pl_w->atck_level * 2] ;
		pl_w->spd.y = spd[pl_w->atck_level * 2 + 1] ;
		pl_w->acc.x = pl_w->disp_dir ? acc[pl_w->atck_level * 2] : -acc[pl_w->atck_level * 2] ;
		pl_w->acc.y = acc[pl_w->atck_level * 2 + 1] ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 11 ) ;
		if( pl_w->atck_level ){
			pl_w->atck_no = pl17_at_psimitar_h ;
			HIT_RESET( pl_w ) ;
		} else {
			pl_w->atck_no = pl17_at_psimitar_l ;
			HIT_RESET( pl_w ) ;
		}
	}
	if( HI_BYTE( pl_w->cg_rival ) == 1 ){
		HI_BYTE( pl_w->cg_rival ) = 0 ;
		if( pl_w->atck_level ){
			pl_w->atck_no = pl17_at_psimitar_h_2 ;
			HIT_RESET( pl_w ) ;
		} else {
			pl_w->atck_no = pl17_at_psimitar_l_2 ;
			HIT_RESET( pl_w ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void psimitar_rise( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type == 3 ){
		pl_w->cg_type = 0 ;
		shl17_02_set( pl_w ) ;
	}
	if( pl_w->spd.y - 8.0f < 0.0f ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, 13 ) ;
		RATE_CLR_X( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void psimitar_top( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 14 ) ;
	}
}

/*--------------------------------------------------------------*/
static void psimitar_fall( PLAYER_WORK *pl_w )
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
		pl_land_ef_set( pl_w ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 16 ) ;
	}
}

/*--------------------------------------------------------------*/
static void psimitar_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void viper_init( PLAYER_WORK *pl_w );
static void viper_move( PLAYER_WORK *pl_w );
static void viper_wait( PLAYER_WORK *pl_w );
static void viper_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_viper( PLAYER_WORK *pl_w )
{
	static void ( *viper_jmp[])( PLAYER_WORK *pl_w ) = {
		viper_init, viper_move, viper_wait, viper_end,
	} ;

	beam_lv_ck( pl_w ) ;
	viper_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void viper_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	if( pl_w->team_type != 3 ) pl_gauge_add( pl_w, 5 ) ;
	if( pl_w->pl_stance == STANCE_AIR ){
		pl_dir_set( pl_w ) ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
		if( pl_w->atck_level ) pl_char_set( pl_w, Char_Special, 7 ) ;
		else pl_char_set( pl_w, Char_Special, 6 ) ;
		pls17_00_set2( pl_w, 1 ) ;
	} else {
		sp_stand_set( pl_w ) ;
		if( pl_w->atck_level ) pl_char_set( pl_w, Char_Special, 3 ) ;
		else pl_char_set( pl_w, Char_Special, 2 ) ;
		pls17_00_set2( pl_w, 0 ) ;
	}
	pl_w->spd.x /= 16.0f ;
	pl_w->spd.y /= 8.0f ;
	pl_w->acc.y /= 64.0f ;
	pl_w->acc.x = 0.0f ;
#if BUILD_DATE >= 20000303	/* the attack, as in hviper_init */
	if( pl_w->atck_level ){
		pl_w->atck_no = pl17_at_viper_h ;
		HIT_RESET( pl_w ) ;
	} else {
		pl_w->atck_no = pl17_at_viper_l ;
		HIT_RESET( pl_w ) ;
	}
#endif
	viper_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void viper_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	land_ck( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		if( pl_w->pl_stance == STANCE_AIR ){
			pl_char_set( pl_w, Char_Special, 8 ) ;
			pls17_00_set( pl_w, 1 ) ;
		} else {
			pl_char_set( pl_w, Char_Special, 4 ) ;
			pls17_00_set( pl_w, 0 ) ;
		}
		shl17_00_set( pl_w ) ;
		pl_w->timer1 = pl_w->atck_level ? 40 : 14 ;
		pl_w->cancel_timer = 16 ;
		pl_w->unk_27b = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void viper_wait( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	land_ck( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( --pl_w->timer1 < 0 ){
		pl_w->routine_2 ++ ;
		if( pl_w->pl_stance == STANCE_AIR ){
			pl_w->char_no.b.no = 9 ;
			pl_w->acc.y = -DOT_Y( 0.375f ) ;
		} else pl_w->char_no.b.no = 5 ;
		pl_char_set( pl_w, Char_Special, pl_w->char_no.b.no ) ;
	}
}

/*--------------------------------------------------------------*/
static void viper_end( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	land_ck( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		if( pl_w->pl_stance != STANCE_AIR ) pl_stand_ret( pl_w ) ;
		else pl_jump_ret( pl_w ) ;
		pl_w->work2 = 0 ;
	}
}

/* the time each beam level lasts */
static const Uint8 beam_lv_time[4] = { 60, 10, 8, 4 } ;

/*==============================================================*/
/*			The beam's level drops with time and rises with		*/
/*			the punch buttons, or at random for the computer	*/
/*==============================================================*/
static void beam_lv_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->team_no ) return ;
	if( pl_w->cpu_flag ){
		if( !PL17_LW( pl_w )->beam_time[0] ){
			if( PL17_LW( pl_w )->beam_lv[0] ) PL17_LW( pl_w )->beam_lv[0] -- ;
			PL17_LW( pl_w )->beam_time[0] = beam_lv_time[PL17_LW( pl_w )->beam_lv[0]] ;
		}
		PL17_LW( pl_w )->beam_time[0] -- ;
		if( !( Rnd() & 7 ) ){
			if( PL17_LW( pl_w )->beam_lv[0] < 3 ) PL17_LW( pl_w )->beam_lv[0] ++ ;
			PL17_LW( pl_w )->beam_time[0] = beam_lv_time[PL17_LW( pl_w )->beam_lv[0]] ;
		}
	} else if( vs_sys_work.team_hyper_flag ){
		if( !PL17_LW( pl_w )->beam_time[0] ){
			if( PL17_LW( pl_w )->beam_lv[0] ) PL17_LW( pl_w )->beam_lv[0] -- ;
			PL17_LW( pl_w )->beam_time[0] = beam_lv_time[PL17_LW( pl_w )->beam_lv[0]] ;
		}
		PL17_LW( pl_w )->beam_time[0] -- ;
		if( Rnd() & 1 ){
			if( PL17_LW( pl_w )->beam_lv[0] < 3 ) PL17_LW( pl_w )->beam_lv[0] ++ ;
			PL17_LW( pl_w )->beam_time[0] = beam_lv_time[PL17_LW( pl_w )->beam_lv[0]] ;
		}
	} else {
		if( !PL17_LW( pl_w )->beam_time[0] ){
			if( PL17_LW( pl_w )->beam_lv[0] ) PL17_LW( pl_w )->beam_lv[0] -- ;
			PL17_LW( pl_w )->beam_time[0] = beam_lv_time[PL17_LW( pl_w )->beam_lv[0]] ;
		}
		PL17_LW( pl_w )->beam_time[0] -- ;
		if( pl_w->pl_now_lvbt & KEY_PUNCH ){
			if( PL17_LW( pl_w )->beam_lv[0] < 3 ) PL17_LW( pl_w )->beam_lv[0] ++ ;
			PL17_LW( pl_w )->beam_time[0] = beam_lv_time[PL17_LW( pl_w )->beam_lv[0]] ;
		}
	}
}

static void crackdown_init( PLAYER_WORK *pl_w );
static void crackdown_move( PLAYER_WORK *pl_w );
static void crackdown_dash( PLAYER_WORK *pl_w );
static void crackdown_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A rush at the opponent								*/
/*==============================================================*/
static void sp_crackdown( PLAYER_WORK *pl_w )
{
	static void ( *crackdown_jmp[])( PLAYER_WORK *pl_w ) = {
		crackdown_init, crackdown_move, crackdown_dash, crackdown_end,
	} ;

	crackdown_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void crackdown_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	if( pl_w->team_type != 3 ) pl_gauge_add( pl_w, 5 ) ;
	pl_char_set( pl_w, Char_Special, 18 ) ;
	sp_stand_set( pl_w ) ;
	pl_voice_req( pl_w, 3 ) ;
#if BUILD_DATE >= 20000303	/* the attack, as in hviper_init: it was set in crackdown_move */
	if( pl_w->atck_level ){
		pl_w->atck_no = pl17_at_crackdown_h ;
		HIT_RESET( pl_w ) ;
	} else {
		pl_w->atck_no = pl17_at_crackdown_l ;
		HIT_RESET( pl_w ) ;
	}
#endif
	crackdown_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void crackdown_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type == 2 ) pl_w->cg_type = 0 ;
	if( pl_w->cg_type == 3 ){
		pl_w->cg_type = 0 ;
		pls17_05_set( pl_w, 0 ) ;
	}
	if( pl_w->cg_type == 4 ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->timer0 = pl_w->atck_level ? 12 : 8 ;
		if( pl_w->atck_level ){
			pl_w->spd.x = pl_w->disp_dir ? 30.0f : -30.0f ;
#if BUILD_DATE < 20000303	/* later, in crackdown_init */
			pl_w->atck_no = pl17_at_crackdown_h ;
			HIT_RESET( pl_w ) ;
#endif
		} else {
			pl_w->spd.x = pl_w->disp_dir ? DOT_X( 16.0f ) : -DOT_X( 16.0f ) ;
#if BUILD_DATE < 20000303
			pl_w->atck_no = pl17_at_crackdown_l ;
			HIT_RESET( pl_w ) ;
#endif
		}
		pl_w->acc.x = pl_w->disp_dir ? -1.25f : 1.25f ;
		pls17_05_set( pl_w, 1 ) ;
	}
}

/*==============================================================*/
/*			He runs until time is up, he is near the			*/
/*			opponent or he hits									*/
/*==============================================================*/
static void crackdown_dash( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_w->spd.x * pl_w->acc.x >= 0.0f ) RATE_CLR_X( pl_w ) ;
	if( --pl_w->timer0 && fabs( pl_w->pos.x - pl_w->target_w->pos.x ) > 90.0f && !pl_w->hit_flag ) return ;
	pl_w->atck_level ? pl_char_set( pl_w, Char_Special, 20 ) : pl_char_set( pl_w, Char_Special, 19 ) ;
	pl_w->spd.x = pl_w->disp_dir ? 4.0f : -4.0f ;
	pl_w->acc.x = pl_w->disp_dir ? -0.2f : 0.2f ;
	pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void crackdown_end( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_w->spd.x * pl_w->acc.x >= 0.0f ) RATE_CLR_X( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	switch( pl_w->cg_type ){
	case 2:
		pl_w->cg_type = 0 ;
		shl17_04_set( pl_w ) ;
		break ;
	case 3:
		pl_w->cg_type = 0 ;
		break ;
	case 4:
		pl_w->cg_type = 0 ;
		RATE_CLR_X( pl_w ) ;
		break ;
	}
}

static void electrap_init( PLAYER_WORK *pl_w );
static void electrap_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_electrap( PLAYER_WORK *pl_w )
{
	static void ( *electrap_jmp[])( PLAYER_WORK *pl_w ) = {
		electrap_init, electrap_move,
	} ;

	electrap_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void electrap_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->atck_no = pl17_at_electrap_burst ;
	HIT_RESET( pl_w ) ;
	if( pl_w->pl_stance == STANCE_AIR ){
		pl_dir_set( pl_w ) ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_w->char_no.b.no = 1 ;
		pl_w->spd.x /= 16.0f ;
		pl_w->spd.y /= 8.0f ;
		pl_w->acc.y /= 64.0f ;
		pl_w->acc.x = 0.0f ;
	} else {
		pl_w->char_no.b.no = 0 ;
		sp_stand_set( pl_w ) ;
	}
	pl_char_set( pl_w, Char_Special, pl_w->char_no.b.no ) ;
	pl_voice_req( pl_w, 5 ) ;
	electrap_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void electrap_move( PLAYER_WORK *pl_w )
{
	Uint8 no ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	land_ck( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		if( pl_w->pl_stance != STANCE_AIR ) pl_stand_ret( pl_w ) ;
		else pl_jump_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		PL17_LW( pl_w )->sw_off_flag[0] = 1 ;
		if( PL17_LW( pl_w )->sw_hold[0] ) PL17_LW( pl_w )->sw_off_flag[0] = 0 ;
		no = 0 ;
		if( pl_w->pl_stance == STANCE_AIR ) no = 2 ;
		shl17_01_set( pl_w, no ) ;
		PL17_LW( pl_w )->grenade_flag[0] = 1 ;
	}
}

static void chouhatsu_init( PLAYER_WORK *pl_w );
static void chouhatsu_move( PLAYER_WORK *pl_w );
static void chouhatsu_loop( PLAYER_WORK *pl_w );
static void chouhatsu_end( PLAYER_WORK *pl_w );
static void chouhatsu_btn_ck( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Goes on while the button is tapped					*/
/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w )
{
	static void ( *chouhatsu_jmp[])( PLAYER_WORK *pl_w ) = {
		chouhatsu_init, chouhatsu_move, chouhatsu_loop, chouhatsu_end,
	} ;

	chouhatsu_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void chouhatsu_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_char_set( pl_w, Char_Extra, 2 ) ;
	chouhatsu_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void chouhatsu_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Extra, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void chouhatsu_loop( PLAYER_WORK *pl_w )
{
	chouhatsu_btn_ck( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		if( pl_w->cpu_flag ){
			if( Rnd() & 1 ) PL17_LW( pl_w )->start_cnt[0] = 1 ;
			else PL17_LW( pl_w )->start_cnt[0] = 0 ;
		}
		if( PL17_LW( pl_w )->start_cnt[0] >= 1 ) pl_char_set( pl_w, Char_Extra, 3 ) ;
		else {
			pl_w->routine_2 ++ ;
			pl_char_set( pl_w, Char_Extra, 4 ) ;
		}
		PL17_LW( pl_w )->start_cnt[0] = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void chouhatsu_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void chouhatsu_btn_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->pl_now_lvbt & KEY_START ) PL17_LW( pl_w )->start_cnt[0] ++ ;
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
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = pl17_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 23 ) ;
}

/*--------------------------------------------------------------*/
static void snapback_move( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type == 15 ){
		pl_w->cg_type = 0 ;
		ofs.x = DOT_X( 16.0f ) ;
		ofs.y = DOT_Y( 40.0f ) ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
}

static PLAYER_WORK *nml_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *crouch_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *jump_catch_ck( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Can a throw start?									*/
/****************************************************************/
PLAYER_WORK *pl17_catch_ck( PLAYER_WORK *pl_w )
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

	if( !( pl_w->dir = ( pl_w->atck_lever & ( KEY_DOWN | KEY_FORWARD | KEY_BACK ) ) >> 10 ) ) return 0 ;
	if( pl_w->atck_pk == 1 && pl_w->atck_level == 1 && pl_w->pos.y > DOT_Y( 64.0f ) ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 0 ;
			return em ;
		}
		return 0 ;
	}
	return 0 ;
}

static void nml_k_set( PLAYER_WORK *pl_w );
static void psycharge_set( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Start the throw that pl17_catch_ck or sp_psycharge			*/
/*			chose												*/
/****************************************************************/
void pl17_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_k_set, nml_k_set, psycharge_set,
	} ;

	catch_set_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_k_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_w->atck_lever & KEY_FORWARD ){
		pl_w->disp_dir ^= 1 ;
		pl_w->pl_dir ^= 1 ;
	}
	ofs.x = -DOT_X( 50.0f ) ;
	ofs.y = DOT_Y( 69.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	scr_excep_set( pl_w, 5, 5 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->hit_stop = 10 ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, pl_w->pl_stance == STANCE_AIR ? 3 : 0 ) ;
}

/*==============================================================*/
static void psycharge_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	ofs.x = -DOT_X( 74.0f ) ;
	ofs.y = DOT_Y( 76.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	catch_gauge_add( pl_w ) ;
	pl_w->hit_stop = 10 ;
	pl_char_set( pl_w, Char_Catch, 1 ) ;
}

static void nml_k_catch( PLAYER_WORK *pl_w );
static void no_catch( PLAYER_WORK *pl_w );
static void psycharge_catch( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void pl17_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_k_catch, no_catch, psycharge_catch,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void no_catch( PLAYER_WORK *pl_w )
{
}

/*==============================================================*/
static void nml_k_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	Float mv ;

	pl_char_move( pl_w ) ;
	if( ( mv = pl_w->cg_flag1 ) ){
		if( pl_w->disp_dir ) mv = -mv ;
		pl_w->pos.x += mv ;
		pl_w->cg_flag1 = 0 ;
	}
	if( !pl_w->routine_2 ){
		if( pl_w->cg_type ){
			pl_w->routine_2 ++ ;
			pl_w->cg_type = 0 ;
			if( pl_w->pl_stance == STANCE_AIR ) pl_w->acc.y = -DOT_Y( 0.375f ) ;
			em = pl_w->catch_w ;
			em->dm_hit_w = pl_w ;
			em->catch_type = 1 ;
			em->atck_no = pl17_at_throw_k ;
			em->pl_dir = pl_w->pl_dir ;
			SCR_EXCEP_SET ;
		}
	} else if( pl_w->cg_prm ){
		if( pl_w->pl_stance == STANCE_AIR ) pl_jump_ret( pl_w ) ;
		else pl_stand_ret( pl_w ) ;
	}
}

static void psycharge_catch_init( PLAYER_WORK *pl_w );
static void psycharge_catch_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void psycharge_catch( PLAYER_WORK *pl_w )
{
	static void ( *psycharge_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		psycharge_catch_init, psycharge_catch_move,
	} ;

	psycharge_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void psycharge_catch_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	psycharge_catch_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void psycharge_catch_move( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew = pl_w->target_w ;
	PLAYER_WORK *em ;
	VEC3 pos ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type & 8 ){
		pl_w->cg_type = 0 ;
		pl_w->catch_w->dm_hit_w = pl_w ;
		pl_w->catch_w->atck_no = pl17_at_psycharge_hit ;
		ew->dm_elem = 3 ;
		ew->unk_228 = 3 ;
		dm00_04_set( ew ) ;
		catch_damage( pl_w ) ;
	}
	if( pl_w->cg_type == 2 ){
		pos = pl_w->pos ;
		pos.x += pl_w->disp_dir ? 140.0f : -140.0f ;
		pos.y += 204.0f ;
		pls17_03_set( pl_w, 2 ) ;
		burst_set( pl_w, &pos, 1, 0x91 ) ;
		pl_w->cg_type = 0 ;
		thunder_spark_set( pl_w->target_w ) ;
	}
	if( HI_BYTE( pl_w->cg_rival ) == 15 ){
		HI_BYTE( pl_w->cg_rival ) = 0 ;
		pl_w->cg_type = 0 ;
		pls17_03_set( pl_w, 0 ) ;
	}
	if( pl_w->cg_type == 4 ){
		pl_w->cg_type = 0 ;
		pls17_03_set( pl_w, 3 ) ;
		pl_stand_ret( pl_w ) ;
	}
	if( pl_w->cg_type ){
		SCR_EXCEP_SET ;
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->atck_no = pl17_at_psycharge_2 ;
	}
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void pl17_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl17_sp_hviper ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl17_sp_hviper ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl17_sp_hviper ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl17_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl17_sp_hviper ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl17_sp_hviper ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl17_sp_hviper ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl17_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl17_sp_viper ;
		pl_w->atck_level = 0 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl17_sp_psimitar ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl17_sp_electrap ;
		pl_w->atck_level = 0 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void pl17_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl17_sp_viper ;
		pl_w->atck_level = 0 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl17_sp_psimitar ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl17_sp_crackdown ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

static void hold( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Holding the opponent								*/
/****************************************************************/
void pl17_hold( PLAYER_WORK *pl_w )
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

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_move( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A kick that leaps off the wall						*/
/****************************************************************/
void pl17_t_kick( PLAYER_WORK *pl_w )
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
	pl_w->spd.x = 30.0f ;
	if( !pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 2.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->atck_no = pl17_at_t_kick ;
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
		pl_voice_req( pl_w, 13 ) ;
	}
}

/*--------------------------------------------------------------*/
static void t_kick_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stop_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl17.c" End of File	*/

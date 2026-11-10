/* Felicia (character 20): command checks, normal attacks, appearance, win and lose poses, special
   moves and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl20_def.h"

/* speeds for a jump */
typedef struct JUMP_SPD {
	Float	spd_x, acc_x, spd_y, acc_y;
} JUMP_SPD;

void pl20_main( PLAYER_WORK *pl_w );
void pl20_com_ck( PLAYER_WORK *pl_w );
void pl20_atck_set( PLAYER_WORK *pl_w );
void pl20_atck( PLAYER_WORK *pl_w );
void pl20_dash( PLAYER_WORK *pl_w );
void pl20_noki( PLAYER_WORK *pl_w );
void pl20_appear( PLAYER_WORK *pl_w );
void pl20_win( PLAYER_WORK *pl_w );
void pl20_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl20_catch_ck( PLAYER_WORK *pl_w );
void pl20_catch_set( PLAYER_WORK *pl_w );
void pl20_catch( PLAYER_WORK *pl_w );
void pl20_hold( PLAYER_WORK *pl_w );
int pl20_delay_ck( PLAYER_WORK *pl_w );
void pl20_t_kick( PLAYER_WORK *pl_w );
void pl20_upper( PLAYER_WORK *pl_w );
void pl20_sp_unison( PLAYER_WORK *pl_w );
void pl20_sp_gattai( PLAYER_WORK *pl_w );
void pl20_sp_assist( PLAYER_WORK *pl_w );
void pl20_sp_zero( PLAYER_WORK *pl_w );
void pl20_tjump( PLAYER_WORK *pl_w );

/* each attack's table: standing and crouching, jumping, super jumping */
static Sint8 lp_tbl[4] = {  1,  2,  0,  2 } ;
static Sint8 mp_tbl[4] = { -1,  2,  1,  2 } ;
static Sint8 hp_tbl[4] = { -1, -1, -1,  2 } ;
static Sint8 lk_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 mk_tbl[4] = { -1,  2, -1,  2 } ;
static Sint8 hk_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 j_lp_tbl[4] = {  1,  2,  0,  2 } ;
static Sint8 j_mp_tbl[4] = { -1,  2,  1,  2 } ;
static Sint8 j_hp_tbl[4] = { -1, -1, -1,  2 } ;
static Sint8 j_lk_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 j_mk_tbl[4] = { -1,  2, -1,  2 } ;
static Sint8 j_hk_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 sj_lp_tbl[4] = {  1,  2,  0,  2 } ;
static Sint8 sj_mp_tbl[4] = { -1,  2,  1,  2 } ;
static Sint8 sj_hp_tbl[4] = { -1, -1, -1,  2 } ;
static Sint8 sj_lk_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 sj_mk_tbl[4] = { -1,  2, -1,  2 } ;
static Sint8 sj_hk_tbl[4] = { -1, -1, -1, -1 } ;

/* commands: steps, kind, the special move, then the stick and buttons */
static Uint16 com_delta[] = { 3, 0, pl20_sp_delta, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_KICK } ;	/* Delta Kick */
static Uint16 com_rolling[] = { 3, COM_GROUND, pl20_sp_rolling, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Rolling Buckler */
static Uint16 com_sand[] = { 3, COM_GROUND, pl20_sp_sand, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Sand Splash */
static Uint16 com_spike[] = { 3, COM_GROUND, pl20_sp_spike, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_PUNCH } ;	/* Cat Spike */
static Uint16 com_hellcat[] = { 5, COM_GROUND, pl20_sp_hellcat, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Hell Cat */
static Uint16 com_dancing[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl20_sp_dancing, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Dancing Flash (hyper) */
static Uint16 com_help[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl20_sp_help, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Please Help Me (hyper) */
static Uint16 com_ssand[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl20_sp_ssand, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Super Sand Splash (hyper) */

static void ( *pl20_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl20_main,
	pl20_com_ck,
	pl20_atck_set,
	pl20_atck,
	pl20_dash,
	pl20_noki,
	pl20_appear,
	pl20_win,
	pl20_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl20_catch_ck,	/* returns a value */
	pl20_catch_set,
	pl20_catch,
	pl20_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl20_delay_ck,
	pl20_t_kick,
	pl20_upper,
	pl20_sp_unison,
	pl20_sp_gattai,
	pl20_sp_assist,
	pl20_sp_zero,
	0,
	0,
	pl20_tjump,
} ;

static Uint8 rolling_ck( PLAYER_WORK *pl_w );
static Uint8 delta_ck( PLAYER_WORK *pl_w );
static Uint8 sand_ck( PLAYER_WORK *pl_w );
static Uint8 spike_ck( PLAYER_WORK *pl_w );
static Uint8 hellcat_ck( PLAYER_WORK *pl_w );
static Uint8 dancing_ck( PLAYER_WORK *pl_w );
static Uint8 help_ck( PLAYER_WORK *pl_w );
static Uint8 ssand_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static int chouhatsu_ck( PLAYER_WORK *pl_w );
static int help_delay_ck( PLAYER_WORK *pl_w );
static int dancing_delay_ck( PLAYER_WORK *pl_w );
static int ssand_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl20_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl20_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl20_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( help_ck( pl_w ) ) return ;
	if( dancing_ck( pl_w ) ) return ;
	if( ssand_ck( pl_w ) ) return ;
	if( hellcat_ck( pl_w ) ) return ;
	if( spike_ck( pl_w ) ) return ;
	if( delta_ck( pl_w ) ) return ;
	if( rolling_ck( pl_w ) ) return ;
	if( sand_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
static Uint8 rolling_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_rolling, &pl_w->com_w0 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl20_sp_rolling ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			On the ground, or once a jump in the air			*/
/*==============================================================*/
static Uint8 delta_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_delta, &pl_w->com_w1 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->routine_1 = PL_NORM ;
	if( pl_w->pl_stance == STANCE_AIR ) pl_w->routine_2 = 1 ;
	else pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl20_sp_delta ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 sand_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_sand, &pl_w->com_w2 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl20_sp_sand ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 spike_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_spike, &pl_w->com_w3 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w3 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl20_sp_spike ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 hellcat_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_hellcat, &pl_w->com_w4 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w4 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl20_sp_hellcat ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 dancing_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_dancing, &pl_w->com_w6 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w6 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl20_sp_dancing ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 help_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_help, &pl_w->com_w7 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w7 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl20_sp_help ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 ssand_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_ssand, &pl_w->com_w8 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w8 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl20_sp_ssand ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static int snapback_ck( PLAYER_WORK *pl_w )
{
	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl20_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static int chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 10 ) ) return 0 ;
	pl_w->pl_sp_type = pl20_sp_chouhatsu ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl20_delay_ck( PLAYER_WORK *pl_w )
{
	if( help_delay_ck( pl_w ) || dancing_delay_ck( pl_w ) || ssand_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
/*			Not in the air										*/
/*==============================================================*/
static int help_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_help, &pl_w->com_w7 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR ) return 0 ;
	pl_w->delay_sp_type = 7 ;
	return 1 ;
}

/*==============================================================*/
static int dancing_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_dancing, &pl_w->com_w6 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 6 ;
	return 1 ;
}

/*==============================================================*/
static int ssand_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_ssand, &pl_w->com_w8 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 8 ;
	return 1 ;
}

/****************************************************************/
void pl20_main( PLAYER_WORK *pl_w )
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
void pl20_atck_set( PLAYER_WORK *pl_w )
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
		pl_w->char_no.b.no = 3 ;
		pl_w->atck_no = pl20_at_stand_lp ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->char_no.b.no = 4 ;
		pl_w->atck_no = pl20_at_stand_mp ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->routine_2 ++ ;
		pl_w->char_no.b.no = 5 ;
		pl_w->atck_no = pl20_at_stand_hp_1 ;
		pl_se_req( pl_w, SE_SWING_M ) ;
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
		pl_w->char_no.b.no = 4 ;
		pl_w->atck_no = pl20_at_stand_mk ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_w->chain_tbl = mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		if( pl_w->atck_lever & KEY_BACK ){
			pl_w->routine_2 ++ ;
			pl_w->char_no.b.no = 5 ;
			pl_w->atck_no = pl20_at_back_hk_1 ;
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
/*			Rising and falling have their own animations		*/
/*==============================================================*/
static void jump_punch( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		if( pl_w->jump_dir < 0 ) pl_w->char_no.b.no = 0 ;
		else pl_w->char_no.b.no = 3 ;
		pl_w->atck_no = ATCK_JUMP_LP ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_lp_tbl ;
		else pl_w->chain_tbl = sj_lp_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		if( pl_w->jump_dir < 0 ) pl_w->char_no.b.no = 1 ;
		else pl_w->char_no.b.no = 4 ;
		pl_w->atck_no = ATCK_JUMP_MP ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mp_tbl ;
		else pl_w->chain_tbl = sj_mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		if( pl_w->jump_dir < 0 ) pl_w->char_no.b.no = 2 ;
		else pl_w->char_no.b.no = 5 ;
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
		if( pl_w->jump_dir < 0 ) pl_w->char_no.b.no = 0 ;
		else pl_w->char_no.b.no = 3 ;
		pl_w->atck_no = ATCK_JUMP_LK ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_lk_tbl ;
		else pl_w->chain_tbl = sj_lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		if( pl_w->jump_dir < 0 ) pl_w->char_no.b.no = 1 ;
		else pl_w->char_no.b.no = 4 ;
		pl_w->atck_no = ATCK_JUMP_MK ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mk_tbl ;
		else pl_w->chain_tbl = sj_mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		if( pl_w->jump_dir < 0 ) pl_w->char_no.b.no = 2 ;
		else pl_w->char_no.b.no = 5 ;
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

static void nml_move( PLAYER_WORK *pl_w );
static void nml_move_dash( PLAYER_WORK *pl_w );
static void stand_punch_move( PLAYER_WORK *pl_w );
static void crouch_punch_move( PLAYER_WORK *pl_w );
static void stand_kick_move( PLAYER_WORK *pl_w );
static void crouch_kick_move( PLAYER_WORK *pl_w );
static void jump_move( PLAYER_WORK *pl_w );
static void sjump_move( PLAYER_WORK *pl_w );
static void jump_punch_move( PLAYER_WORK *pl_w );
static void jump_kick_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl20_atck( PLAYER_WORK *pl_w )
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
/*			The heavy one is a chain of hits					*/
/*==============================================================*/
static void stand_punch_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 2:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		else if( pl_w->routine_2 ){
			switch( pl_w->cg_type ){
			case 1:
				pl_w->cg_type = 0 ;
				pl_w->atck_no = pl20_at_stand_hp_2 ;
				HIT_RESET( pl_w ) ;
				break ;
			case 2:
				pl_w->cg_type = 0 ;
				pl_w->atck_no = pl20_at_stand_hp_3 ;
				HIT_RESET( pl_w ) ;
				break ;
			}
		} else if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pl_w->atck_no = pl20_at_stand_hp_alt ;
			HIT_RESET( pl_w ) ;
		}
		break ;
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
	case 0:
	case 1:
	case 2:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
/*			The heavy one held back hits again					*/
/*==============================================================*/
static void stand_kick_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 2:
		if( pl_w->routine_2 ){
			if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
			else if( pl_w->cg_type ){
				pl_w->cg_type = 0 ;
				pl_w->atck_no = pl20_at_back_hk_2 ;
				HIT_RESET( pl_w ) ;
			}
			break ;
		}
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
/*			The medium one hits twice							*/
/*==============================================================*/
static void jump_punch_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
		else if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pl_w->atck_no = pl20_at_jump_mp_2 ;
			HIT_RESET( pl_w ) ;
		}
		break ;
	case 0:
	case 2:
		if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
		break ;
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
void pl20_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_char_set( pl_w, Char_Extra, 5 ) ;
		pl_w->atck_no = pl20_at_upper ;
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
static void dash_jump( PLAYER_WORK *pl_w );
static void dash_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The dash, a low leap								*/
/****************************************************************/
void pl20_dash( PLAYER_WORK *pl_w )
{
	static void ( *dash_jmp[])( PLAYER_WORK *pl_w ) = {
		dash_init, dash_move, dash_jump, dash_end,
	} ;

	dash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dash_init( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->timer0 = 18 ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 11.5f ) : -DOT_X( 11.5f ) ;
		pl_w->acc.x = pl_w->pl_dir ? -0.7291666f : 0.7291666f ;
		pl_w->spd.y = DOT_Y( 4.5f ) ;
		pl_w->acc.y = -1.0044643f ;
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
	if( !--pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->acc.x = pl_w->pl_dir ? -0.7291666f : -0.7291666f ;
		pl_w->acc.y = -1.0044643f ;
		pl_char_set( pl_w, Char_Dash, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void dash_jump( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !pl_w->cg_type ) pl_char_move( pl_w ) ;
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
static void dash_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void noki_init( PLAYER_WORK *pl_w );
static void noki_move( PLAYER_WORK *pl_w );
static void noki_jump( PLAYER_WORK *pl_w );
static void noki_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash, a leap								*/
/****************************************************************/
void pl20_noki( PLAYER_WORK *pl_w )
{
	static void ( *noki_jmp[])( PLAYER_WORK *pl_w ) = {
		noki_init, noki_move, noki_jump, noki_end,
	} ;

	noki_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void noki_init( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 18 ;
		pl_w->cg_type = 0 ;
		pl_w->spd.x = pl_w->pl_dir ? -15.0f : 15.0f ;
		pl_w->acc.x = pl_w->pl_dir ? DOT_X( 0.25f ) : -DOT_X( 0.25f ) ;
		pl_w->spd.y = DOT_Y( 5.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.5f ) ;
	}
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
		pl_w->acc.x = pl_w->pl_dir ? DOT_X( 0.25f ) : DOT_X( 0.25f ) ;
		pl_w->acc.y = -DOT_Y( 0.5f ) ;
		pl_char_set( pl_w, Char_Dash, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void noki_jump( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !pl_w->cg_type ) pl_char_move( pl_w ) ;
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
static void noki_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void appear_init( PLAYER_WORK *pl_w );
static void appear_move( PLAYER_WORK *pl_w );
static void win_time_ko( PLAYER_WORK *pl_w );
static void win_ko_init( PLAYER_WORK *pl_w );
static void win_ko_move( PLAYER_WORK *pl_w );
static void win_ko_move_e( PLAYER_WORK *pl_w );
static void win_ko_move_2( PLAYER_WORK *pl_w );
static void lose_time_ko( PLAYER_WORK *pl_w );

/****************************************************************/
void pl20_appear( PLAYER_WORK *pl_w )
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
/*			Lost and draw share their pose						*/
/****************************************************************/
void pl20_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_time_ko,
		lose_time_ko,
		win_time_ko,
		lose_time_ko,
		lose_time_ko,
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
		win_ko_move_e,
		win_ko_move_2,
	} ;

	win_ko_jmp[ pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			One of three poses, picked by the frame count		*/
/*==============================================================*/
static void win_ko_init( PLAYER_WORK *pl_w )
{
	static Uint8 win_tbl[4] = { 0, 1, 2, 0 } ;
	static Uint8 win_tbl_2[4] = { 0, 2, 0, 2 } ;

	if( vs_sys_work.vs_end_flag ) pl_w -> timer0 = win_tbl[Sys->game_timer0 & 3] ;
	else pl_w -> timer0 = win_tbl_2[Sys->game_timer0 & 3] ;
	if( pl_be_cnt( pl_w ) >= 2 ) pl_w -> timer0 = 0 ;

	switch( pl_w -> timer0 ){
	case 0 :
		pl_w -> routine_2 += 1 ;
		pl_w -> char_no.b.no = 0 ;
		break ;
	case 1 :
		pl_w -> routine_2 += 2 ;
		pl_w -> char_no.b.no = 1 ;
		break ;
	case 2 :
		pl_w -> routine_2 += 3 ;
		pl_w -> char_no.b.no = 2 ;
		break ;
	}
	pl_char_set( pl_w, Char_Win, pl_w->char_no.b.no ) ;
}

/*--------------------------------------------------------------*/
static void win_ko_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_ko_move_e( PLAYER_WORK *pl_w )
{
	pl_w->win_wait = 0xff ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pls20_00_set( pl_w, 4, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void win_ko_move_2( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void lose_time_ko( PLAYER_WORK *pl_w )
{
	if( !pl_w -> routine_2 ){
		pl_w -> routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, 3 ) ;
	}else{
		pl_char_move( pl_w ) ;
	}
}

static void sp_delta( PLAYER_WORK *pl_w );
static void sp_rolling( PLAYER_WORK *pl_w );
static void sp_sand( PLAYER_WORK *pl_w );
static void sp_spike( PLAYER_WORK *pl_w );
static void sp_hellcat( PLAYER_WORK *pl_w );
static void sp_dancing( PLAYER_WORK *pl_w );
static void sp_help( PLAYER_WORK *pl_w );
static void sp_ssand( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Special moves and hyper combos; 5 is done as 3		*/
/****************************************************************/
void pl20_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *sp_atck_jmp[])( PLAYER_WORK *pl_w ) = {
		sp_delta, sp_rolling, sp_sand, sp_spike, sp_hellcat, sp_spike, sp_dancing, sp_help, sp_ssand, sp_snapback, sp_chouhatsu,
	} ;

	sp_atck_jmp[pl_w->pl_sp_type]( pl_w ) ;
}

static void delta_ground( PLAYER_WORK *pl_w );
static void delta_air( PLAYER_WORK *pl_w );
static void delta_init( PLAYER_WORK *pl_w, PL20_LOCAL_W *lw );
static void delta_start( PLAYER_WORK *pl_w, PL20_LOCAL_W *lw );
static void delta_rise( PLAYER_WORK *pl_w, PL20_LOCAL_W *lw );
static void delta_fall( PLAYER_WORK *pl_w, PL20_LOCAL_W *lw );
static void delta_hit_fall( PLAYER_WORK *pl_w, PL20_LOCAL_W *lw );
static void delta_land( PLAYER_WORK *pl_w, PL20_LOCAL_W *lw );
static void delta_move( PLAYER_WORK *pl_w, PL20_LOCAL_W *lw );
static void delta_air_init( PLAYER_WORK *pl_w, PL20_LOCAL_W *lw );
static void delta_air_start( PLAYER_WORK *pl_w, PL20_LOCAL_W *lw );

/*==============================================================*/
/*			A kick rising, then diving forward; also in the		*/
/*			air													*/
/*==============================================================*/
static void sp_delta( PLAYER_WORK *pl_w )
{
	static void ( *delta_jmp[])( PLAYER_WORK *pl_w ) = {
		delta_ground, delta_air,
	} ;

	delta_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void delta_ground( PLAYER_WORK *pl_w )
{
	static void ( *delta_ground_jmp[])( PLAYER_WORK *pl_w, PL20_LOCAL_W *lw ) = {
		delta_init, delta_start, delta_rise, delta_fall, delta_hit_fall, delta_land,
	} ;

	delta_ground_jmp[pl_w->routine_3]( pl_w, ( PL20_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
/*			The strength picks the animation: an unsigned 1		*/
/*			or 0												*/
/*==============================================================*/
static void delta_init( PLAYER_WORK *pl_w, PL20_LOCAL_W *lw )
{
	/* the rise for each strength: spd.y, acc.y */
	static Float delta_rise_spd[2][2] = {
		{ DOT_Y( 8.0f ), -1.2053571f },
		{ DOT_Y( 11.5f ), -1.4732143f },
	} ;

	pl_w->routine_3 ++ ;
	pl_w->atck_no = pl_w->atck_level ? 50 : 48 ;
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
	pl_w->spd.y = delta_rise_spd[pl_w->atck_level][0] ;
	pl_w->acc.y = delta_rise_spd[pl_w->atck_level][1] ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level ? 1U : 0U ) ;
}

/*--------------------------------------------------------------*/
static void delta_start( PLAYER_WORK *pl_w, PL20_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		pl_air_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void delta_rise( PLAYER_WORK *pl_w, PL20_LOCAL_W *lw )
{
	/* the dive for each strength, on the ground and in the air */
	static JUMP_SPD delta_dive_spd[4] = {
		{ DOT_X( 3.5f ), 0.57942706f, -3.75f, -0.38504463f },
		{ 7.5f, 0.7486979f, -DOT_Y( 2.25f ), -0.48549107f },
		{ DOT_X( 3.5f ), DOT_X( 0.125f ), -DOT_Y( 5.5f ), -DOT_Y( 0.25f ) },
		{ 7.5f, DOT_X( 0.375f ), -DOT_Y( 4.5f ), -0.60267854f },
	} ;

	pl_char_move( pl_w ) ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->cg_type && !pl_w->hit_flag ){
		pl_w->cg_type = 0 ;
		if( pl_w->routine_2 ){
			pl_w->atck_no = pl_w->atck_level ? 87 : 85 ;
			HIT_RESET( pl_w ) ;
		}else{
			pl_w->atck_no = pl_w->atck_level ? 51 : 49 ;
			HIT_RESET( pl_w ) ;
		}
	}
	if( pl_w->spd.y < 0.0f ){
		pl_w->routine_3 ++ ;
		pl_w->timer0 = pl_w->routine_2 ? 2 : 0 ;
		pl_w->timer0 += pl_w->atck_level ;
		pl_w->spd.x = pl_w->pl_dir ? delta_dive_spd[pl_w->timer0].spd_x : -delta_dive_spd[pl_w->timer0].spd_x ;
		pl_w->acc.x = pl_w->pl_dir ? delta_dive_spd[pl_w->timer0].acc_x : -delta_dive_spd[pl_w->timer0].acc_x ;
		pl_w->spd.y = delta_dive_spd[pl_w->timer0].spd_y ;
		pl_w->acc.y = delta_dive_spd[pl_w->timer0].acc_y ;
		lw->dive_spd_x = pl_w->spd.x * 2.0f ;
		lw->dive_spd_y = pl_w->spd.y * 2.0f ;
		pl_w->dir = 0 ;
		if( pl_w->routine_2 ){
			pl_w->atck_no = pl_w->atck_level ? 90 : 88 ;
			HIT_RESET( pl_w ) ;
		}else{
			pl_w->atck_no = pl_w->atck_level ? 54 : 52 ;
			HIT_RESET( pl_w ) ;
		}
		pl_char_set( pl_w, Char_Special, pl_w->atck_level ? 3 : 2 ) ;
	}
}

/*==============================================================*/
/*			A hit bounces her back up							*/
/*==============================================================*/
static void delta_fall( PLAYER_WORK *pl_w, PL20_LOCAL_W *lw )
{
	Uint8 no ;

	delta_move( pl_w, lw ) ;
	pl_char_move( pl_w ) ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_3 += 2 ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_land_ef_set( pl_w ) ;
		no = pl_w->atck_level ? 7 : 6 ;
	}else{
		if( !pl_w->hit_flag ) return ;
		pl_w->routine_3 ++ ;
		if( pl_w->routine_2 ){
			pl_w->atck_no = pl_w->atck_level ? 91 : 89 ;
			HIT_RESET( pl_w ) ;
		}else{
			pl_w->atck_no = pl_w->atck_level ? 55 : 53 ;
			HIT_RESET( pl_w ) ;
		}
		no = pl_w->atck_level ? 5 : 4 ;
	}
	pl_char_set( pl_w, Char_Special, no ) ;
}

/*--------------------------------------------------------------*/
static void delta_hit_fall( PLAYER_WORK *pl_w, PL20_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	delta_move( pl_w, lw ) ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_3 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level ? 7 : 6 ) ;
	}
}

/*--------------------------------------------------------------*/
static void delta_land( PLAYER_WORK *pl_w, PL20_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
static void delta_move( PLAYER_WORK *pl_w, PL20_LOCAL_W *lw )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
}

/*--------------------------------------------------------------*/
static void delta_air( PLAYER_WORK *pl_w )
{
	static void ( *delta_air_jmp[])( PLAYER_WORK *pl_w, PL20_LOCAL_W *lw ) = {
		delta_air_init, delta_air_start, delta_rise, delta_fall, delta_hit_fall, delta_land,
	} ;

	delta_air_jmp[pl_w->routine_3]( pl_w, ( PL20_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void delta_air_init( PLAYER_WORK *pl_w, PL20_LOCAL_W *lw )
{
	static Float delta_air_spd[2][2] = {
		{ DOT_Y( 8.0f ), -1.2053571f },
		{ DOT_Y( 8.0f ), -1.2053571f },
	} ;

	pl_w->routine_3 ++ ;
	pl_w->atck_no = pl_w->atck_level ? 86 : 84 ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.y = delta_air_spd[pl_w->atck_level][0] ;
	pl_w->acc.y = delta_air_spd[pl_w->atck_level][1] ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level ? 29 : 28 ) ;
}

/*--------------------------------------------------------------*/
static void delta_air_start( PLAYER_WORK *pl_w, PL20_LOCAL_W *lw )
{
	pl_w->routine_3 ++ ;
	delta_rise( pl_w, lw ) ;
}

static void rolling_init( PLAYER_WORK *pl_w );
static void rolling_roll( PLAYER_WORK *pl_w );
static void rolling_jump( PLAYER_WORK *pl_w );
static void rolling_rise( PLAYER_WORK *pl_w );
static void rolling_fall( PLAYER_WORK *pl_w );
static void rolling_land( PLAYER_WORK *pl_w );
static void rolling_punch_move( PLAYER_WORK *pl_w );
static void rolling_kick_move( PLAYER_WORK *pl_w );
static void rolling_up( PLAYER_WORK *pl_w );
static void rolling_punch( PLAYER_WORK *pl_w );
static void rolling_kick( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A rolling attack, with three follow-ups on			*/
/*			another button										*/
/*==============================================================*/
static void sp_rolling( PLAYER_WORK *pl_w )
{
	static void ( *rolling_jmp[])( PLAYER_WORK *pl_w ) = {
		rolling_init, rolling_roll, rolling_jump, rolling_rise, rolling_fall, rolling_land, rolling_punch_move, rolling_kick_move,
	} ;

	rolling_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void rolling_init( PLAYER_WORK *pl_w )
{
	/* acc.x and the top speed, for each strength */
	static Float rolling_roll_spd[2][2] = {
		{ 1.6666666f, DOT_X( 8.0f ) },
		{ 1.6666666f, 20.0f },
	} ;

	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl_w->atck_level ? 57 : 56 ;
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
	pl_w->pl_stop_now = 0 ;
	pl_w->timer0 = 30 ;
	pl_w->acc.x = pl_w->pl_dir ? rolling_roll_spd[pl_w->atck_level][0] : -rolling_roll_spd[pl_w->atck_level][0] ;
	pl_w->spd.y = pl_w->pl_dir ? rolling_roll_spd[pl_w->atck_level][1] : -rolling_roll_spd[pl_w->atck_level][1] ;
	pls20_00_set( pl_w, 0, 0 ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level ? 9 : 8 ) ;
}

/*==============================================================*/
/*			Rolls up to the top speed held in spd.y; a hit		*/
/*			or a button gives a follow-up						*/
/*==============================================================*/
static void rolling_roll( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ) return ;
	if( pl_w->hit_flag ){
		if( pl_w->cpu_flag ){
			if( pl_w->atck_pk ) rolling_kick( pl_w ) ;
			else if( pl_w->atck_level ) rolling_up( pl_w ) ;
			else rolling_punch( pl_w ) ;
			return ;
		} else if( pl_w->pl_stop_now & KEY_LP ){ rolling_punch( pl_w ) ; return ; }
		else if( pl_w->pl_stop_now & KEY_HP ){ rolling_up( pl_w ) ; return ; }
		else if( pl_w->pl_stop_now & KEY_KICK ){ rolling_kick( pl_w ) ; return ; }
		pl_w->routine_2 = 4 ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->spd.x = pl_w->hit_flag & ( HR_GUARD | HR_ARMOR ) ? -0.0f : -1.25f ;
		if( !pl_w->pl_dir ) pl_w->spd.x *= -1.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 12.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.875f ) ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level ? 13 : 12 ) ;
		return ;
	}
		if( pl_w->pl_now_lvbt & KEY_LP ){ rolling_punch( pl_w ) ; return ; }
		else if( pl_w->pl_now_lvbt & KEY_HP ){ rolling_up( pl_w ) ; return ; }
		else if( pl_w->pl_now_lvbt & KEY_KICK ){ rolling_kick( pl_w ) ; return ; }
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		if( --pl_w->timer0 ){
			if( fabs( pl_w->spd.x ) >= fabs( pl_w->spd.y ) ){
				pl_w->spd.x = pl_w->spd.y ;
				pl_w->acc.x = 0.0f ;
			}
			return ;
		}
		pl_w->routine_2 = 5 ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level ? 15 : 14 ) ;
}

/*--------------------------------------------------------------*/
static void rolling_jump( PLAYER_WORK *pl_w )
{
	/* the leap for each strength */
	static JUMP_SPD rolling_jump_spd[2] = {
		{ DOT_X( 4.0f ), -0.5729166f, DOT_Y( 6.0f ), -DOT_Y( 0.5f ) },
		{ 12.5f, -0.5729166f, DOT_Y( 10.0f ), -DOT_Y( 0.5f ) },
	} ;

	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_air_set( pl_w ) ;
		pl_w->spd.x = pl_w->pl_dir ? rolling_jump_spd[pl_w->atck_level].spd_x : -rolling_jump_spd[pl_w->atck_level].spd_x ;
		pl_w->acc.x = pl_w->pl_dir ? rolling_jump_spd[pl_w->atck_level].acc_x : -rolling_jump_spd[pl_w->atck_level].acc_x ;
		pl_w->spd.y = rolling_jump_spd[pl_w->atck_level].spd_y ;
		pl_w->acc.y = rolling_jump_spd[pl_w->atck_level].acc_y ;
	}
}

/*==============================================================*/
/*			Stops going forward at the top						*/
/*==============================================================*/
static void rolling_rise( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( ( !pl_w->pl_dir && pl_w->spd.x > 0.0f ) || ( pl_w->pl_dir && pl_w->spd.x < 0.0f ) ){
		pl_w->spd.x = pl_w->acc.x = 0.0f ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->atck_no = pl_w->atck_level ? 61 : 60 ;
		HIT_RESET( pl_w ) ;
	}
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 += 2 ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level ? 15 : 14 ) ;
	}
}

/*--------------------------------------------------------------*/
static void rolling_fall( PLAYER_WORK *pl_w )
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
		pl_char_set( pl_w, Char_Special, pl_w->atck_level ? 15 : 14 ) ;
	}
}

/*--------------------------------------------------------------*/
static void rolling_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
static void rolling_up( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl_w->atck_level ? 59 : 58 ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level ? 11 : 10 ) ;
}

/*==============================================================*/
static void rolling_punch( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = 6 ;
	pl_w->atck_no = pl_w->atck_level ? 63 : 62 ;
	HIT_RESET( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.x = pl_w->pl_dir ? 10.0f : -10.0f ;
	pl_w->acc.x = pl_w->pl_dir ? -0.3125f : 0.3125f ;
	pl_char_set( pl_w, Char_Special, 25 ) ;
}

/*==============================================================*/
static void rolling_kick( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = 7 ;
	pl_w->atck_no = pl_w->atck_level ? 65 : 64 ;
	HIT_RESET( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.x = pl_w->pl_dir ? DOT_X( 8.5f ) : -DOT_X( 8.5f ) ;
	pl_w->acc.x = pl_w->pl_dir ? -0.7291666f : 0.7291666f ;
	pl_char_set( pl_w, Char_Special, 26 ) ;
}

/*--------------------------------------------------------------*/
static void rolling_punch_move( PLAYER_WORK *pl_w )
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
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
}

/*==============================================================*/
/*			Slides on until it slows to a stop					*/
/*==============================================================*/
static void rolling_kick_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
		return ;
	}
	pl_smoke_set2( pl_w, 3 ) ;
	if( pl_w->cg_type ){
		if( pl_w->spd.x * pl_w->acc.x < 0.0f ) pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
}

static void sand_init( PLAYER_WORK *pl_w );
static void sand_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_sand( PLAYER_WORK *pl_w )
{
	static void ( *sand_jmp[])( PLAYER_WORK *pl_w ) = {
		sand_init, sand_move,
	} ;

	sand_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void sand_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl_w->atck_level ? 93 : 92 ;
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
	pl_char_set( pl_w, Char_Special, pl_w->atck_level ? 17 : 16 ) ;
}

/*--------------------------------------------------------------*/
static void sand_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shl20_00_set( pl_w, 1 ) ;
	}
}

static void spike_init( PLAYER_WORK *pl_w );
static void spike_jump( PLAYER_WORK *pl_w );
static void spike_rise( PLAYER_WORK *pl_w );
static void spike_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A leap that comes down on the opponent				*/
/*==============================================================*/
static void sp_spike( PLAYER_WORK *pl_w )
{
	static void ( *spike_jmp[])( PLAYER_WORK *pl_w ) = {
		spike_init, spike_jump, spike_rise, spike_land,
	} ;

	spike_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void spike_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl_w->atck_level ? 69 : 68 ;
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
	shl20_00_set( pl_w, 0 ) ;
	pls20_00_set( pl_w, 1, 0 ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 18 ) ;
}

/*==============================================================*/
/*			The animation's marks push the player forward		*/
/*==============================================================*/
static void spike_jump( PLAYER_WORK *pl_w )
{
	/* the leap for each strength */
	static JUMP_SPD spike_jump_spd[3] = {
		{ DOT_X( 4.0f ), -0.052083332f, 0.0f, 0.0f },
		{ 7.5f, -0.013020833f, DOT_Y( 3.0f ), -DOT_Y( 0.25f ) },
		{ 1.6666666f, -0.0f, 15.0f, -DOT_Y( 0.25f ) },
	} ;
	Float step ;

	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_air_set( pl_w ) ;
		pl_w->timer0 = 12 ;
		pl_w->spd.x = pl_w->pl_dir ? spike_jump_spd[pl_w->atck_level].spd_x : -spike_jump_spd[pl_w->atck_level].spd_x ;
		pl_w->acc.x = pl_w->pl_dir ? spike_jump_spd[pl_w->atck_level].acc_x : -spike_jump_spd[pl_w->atck_level].acc_x ;
		pl_w->spd.y = spike_jump_spd[pl_w->atck_level].spd_y ;
		pl_w->acc.y = spike_jump_spd[pl_w->atck_level].acc_y ;
	}
	step = pl_w->cg_flag1 ;
	if( step ){
		pl_w->cg_flag1 = 0 ;
		step *= 1.6666666f ;
		if( !pl_w->pl_dir ) step *= -1.0f ;
		pl_w->pos.x += step ;
	}
}

/*==============================================================*/
/*			The strongest one lands at once, the others			*/
/*			after a while										*/
/*==============================================================*/
static void spike_rise( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( ( !pl_w->pl_dir && pl_w->spd.x > 0.0f ) || ( pl_w->pl_dir && pl_w->spd.x < 0.0f ) ){
		pl_w->spd.x = pl_w->acc.x = 0.0f ;
	}
	if( ( pl_w->spd.y < 0.0f && pl_w->pos.y <= pl_w->ground_y ) || ( !pl_w->atck_level && !--pl_w->timer0 ) ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 21 ) ;
	}
}

/*--------------------------------------------------------------*/
static void spike_land( PLAYER_WORK *pl_w )
{
	Float step ;

	if( pl_char_move( pl_w ) >= 0 ){
		step = pl_w->cg_flag1 ;
		if( step ){
			pl_w->cg_flag1 = 0 ;
			step *= 1.6666666f ;
			if( !pl_w->pl_dir ) step *= -1.0f ;
			pl_w->pos.x += step ;
		}
		return ;
	}
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_stand_ret( pl_w ) ;
}

static void hellcat_init( PLAYER_WORK *pl_w );
static void hellcat_move( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A reach that becomes a throw when it catches		*/
/*==============================================================*/
static void sp_hellcat( PLAYER_WORK *pl_w )
{
	static void ( *hellcat_jmp[])( PLAYER_WORK *pl_w ) = {
		hellcat_init, hellcat_move,
	} ;

	hellcat_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void hellcat_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 24 ) ;
}

/*--------------------------------------------------------------*/
static void hellcat_move( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->routine_2 = 0 ;
			pl_w->catch_no = 0xc3 ;
			pl_catch_set( pl_w, em ) ;
		}
	}
}

static void dancing_init( PLAYER_WORK *pl_w );
static void dancing_wait( PLAYER_WORK *pl_w );
static void dancing_start( PLAYER_WORK *pl_w );
static void dancing_roll( PLAYER_WORK *pl_w );
static void dancing_hit( PLAYER_WORK *pl_w );
static void dancing_jump( PLAYER_WORK *pl_w );
static void dancing_fall( PLAYER_WORK *pl_w );
static void dancing_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo: a roll that carries the opponent		*/
/*			up into a flurry of hits							*/
/*==============================================================*/
static void sp_dancing( PLAYER_WORK *pl_w )
{
	static void ( *dancing_jmp[])( PLAYER_WORK *pl_w ) = {
		dancing_init, dancing_wait, dancing_start, dancing_roll,
		dancing_hit, dancing_jump, dancing_fall, dancing_land,
	} ;

	dancing_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dancing_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl20_at_dancing ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 0 ) ;
}

/*--------------------------------------------------------------*/
static void dancing_wait( PLAYER_WORK *pl_w )
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
		ofs.x = -5.0f ;
		ofs.y = DOT_Y( 62.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*==============================================================*/
/*			The animation's marks step forward until the		*/
/*			roll starts											*/
/*==============================================================*/
static void dancing_start( PLAYER_WORK *pl_w )
{
	Float step ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		step = pl_w->cg_flag1 ;
		if( step ){
			if( !pl_w->pl_dir ) step = -step ;
			pl_w->pos.x += step ;
		}
		return ;
	}
	pl_w->routine_2 ++ ;
	pl_w->cg_type = 0 ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.x = pl_w->pl_dir ? DOT_X( 5.0f ) : -DOT_X( 5.0f ) ;
	pl_w->acc.x = pl_w->pl_dir ? 1.25f : -1.25f ;
	pl_w->spd.y = DOT_X( 13.0f ) ;
	pl_w->timer0 = 40 ;
}

/*==============================================================*/
/*			Rolls up to the top speed held in spd.y; a hit		*/
/*			holds the opponent									*/
/*==============================================================*/
static void dancing_roll( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_w->hit_flag ){
		if( pl_sp_catch_ck( pl_w ) ){
			pl_w->routine_2 ++ ;
			pl_dir_set( pl_w ) ;
			pl_w->em_w->hit_stop = 32 ;
			pl_w->em_w->pl_stance = STANCE_STAND ;
			pl_w->em_w->pos.y = pl_w->em_w->ground_y ;
			pl_w->pos.x = pl_w->em_w->pos.x + ( pl_w->pl_dir ? -DOT_X( 56.0f ) : DOT_X( 56.0f ) ) ;
			pl_char_set( pl_w, Char_Super, 1 ) ;
			return ;
		} else {
			pl_w->routine_2 = 6 ;
			pl_w->pl_stance = STANCE_AIR ;
			pl_w->delay_flag = 0 ;
			pl_w->delay_timer = 0 ;
			pl_w->hyper_bg = 0 ;
			pl_w->hyper_bg_tm = 0 ;
			pl_w->spd.x = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->spd.y = DOT_Y( 12.0f ) ;
			pl_w->acc.y = -DOT_Y( 0.875f ) ;
			pl_char_set( pl_w, Char_Super, 2 ) ;
			return ;
		}
	} else if( !--pl_w->timer0 ){
		pl_w->routine_2 = 7 ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_char_set( pl_w, Char_Super, 3 ) ;
		return ;
	} else if( pl_w->spd.y <= fabs( pl_w->spd.x ) ){
		pl_w->spd.x = pl_w->pl_dir ? pl_w->spd.y : -pl_w->spd.y ;
		pl_w->acc.x = 0.0f ;
	}
}

/*==============================================================*/
/*			Hits on the animation's marks, then leaps with		*/
/*			the opponent										*/
/*==============================================================*/
static void dancing_hit( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->cg_type = 0 ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 8.0f ) : -DOT_X( 8.0f ) ;
		pl_w->acc.x = pl_w->pl_dir ? -0.5729166f : 0.5729166f ;
		pl_w->spd.y = DOT_Y( 11.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.5f ) ;
		return ;
	}
	if( pl_w->cg_flag1 ){
		if( !pl_w->hit_flag ){
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_stand_ret( pl_w ) ;
			return ;
		}
		pl_w->atck_no = pl_w->cg_flag1 + 73 ;
		HIT_RESET( pl_w ) ;
		pl_w->cg_flag1 = 0 ;
	}
}

/*==============================================================*/
/*			Hits on the way up; the animation's marks move		*/
/*			the player											*/
/*==============================================================*/
static void dancing_jump( PLAYER_WORK *pl_w )
{
	static Float dancing_jump_ofs[2][2] = {
		{ DOT_X( 4.0f ), DOT_Y( 20.0f ) },
		{ 0.0f, DOT_Y( 38.0f ) },
	} ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( ( !pl_w->pl_dir && pl_w->spd.x > 0.0f ) || ( pl_w->pl_dir && pl_w->spd.x < 0.0f ) ){
		pl_w->spd.x = pl_w->acc.x = 0.0f ;
	}
	if( pl_w->cg_flag1 ){
		pl_w->atck_no = pl_w->cg_flag1 + 77 ;
		HIT_RESET( pl_w ) ;
		pl_w->cg_flag1 = 0 ;
	}
	if( pl_w->spd.y > 0.0f ){
		if( !pl_w->cg_type ) return ;
		pl_w->cg_type -- ;
		pl_w->pos.x += pl_w->pl_dir ? -dancing_jump_ofs[pl_w->cg_type][0] : dancing_jump_ofs[pl_w->cg_type][0] ;
		pl_w->pos.y += dancing_jump_ofs[pl_w->cg_type][1] ;
		pl_w->cg_type = 0 ;
		return ;
	}
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void dancing_fall( PLAYER_WORK *pl_w )
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
		pl_char_set( pl_w, Char_Super, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void dancing_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void help_main( PLAYER_WORK *pl_w );
static void help_init( PLAYER_WORK *pl_w, PL20_LOCAL_W *lw );
static void help_wait( PLAYER_WORK *pl_w, PL20_LOCAL_W *lw );
static void help_check( PLAYER_WORK *pl_w, PL20_LOCAL_W *lw );
static void help_miss( PLAYER_WORK *pl_w, PL20_LOCAL_W *lw );
static void help_hold( PLAYER_WORK *pl_w, PL20_LOCAL_W *lw );
static void help_mash( PLAYER_WORK *pl_w, PL20_LOCAL_W *lw );
static void help_last( PLAYER_WORK *pl_w, PL20_LOCAL_W *lw );
static void help_end( PLAYER_WORK *pl_w, PL20_LOCAL_W *lw );

/*==============================================================*/
/*			A hyper combo: the effects that go out report		*/
/*			back in the local work								*/
/*==============================================================*/
static void sp_help( PLAYER_WORK *pl_w )
{
	static void ( *help_jmp[])( PLAYER_WORK *pl_w ) = {
		help_main, help_main,
	} ;

	help_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void help_main( PLAYER_WORK *pl_w )
{
	static void ( *help_main_jmp[])( PLAYER_WORK *pl_w, PL20_LOCAL_W *lw ) = {
		help_init, help_wait, help_check, help_miss,
		help_hold, help_mash, help_last, help_end,
	} ;

	help_main_jmp[pl_w->routine_3]( pl_w, ( PL20_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void help_init( PLAYER_WORK *pl_w, PL20_LOCAL_W *lw )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_3 ++ ;
	pl_w->atck_no = pl20_at_help ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 4 ) ;
}

/*==============================================================*/
/*			Sends out the effects, unless the timer is			*/
/*			nearly out											*/
/*==============================================================*/
static void help_wait( PLAYER_WORK *pl_w, PL20_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		lw->catch_flag = 0 ;
		lw->help_phase = 0 ;
		if( work_rest_cnt < 4 ){
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_stand_ret( pl_w ) ;
		} else {
			shl20_01_set( pl_w, 3 ) ;
			shl20_01_set( pl_w, 2 ) ;
			shl20_01_set( pl_w, 1 ) ;
			shl20_01_set( pl_w, 0 ) ;
			ofs.x = -1.6666666f ;
			ofs.y = DOT_Y( 61.0f ) ;
			ofs.z = 0.0f ;
			pl_hyper_start( pl_w, &ofs, 1 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void help_check( PLAYER_WORK *pl_w, PL20_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	switch( lw->catch_flag ){
	case 0:
		break ;
	case 1:
		pl_w->routine_3 += 2 ;
		pl_w->timer0 = 23 ;
		pl_char_set( pl_w, Char_Super, 5 ) ;
		break ;
	case 2:
		pl_voice_req( pl_w, 23 ) ;
	default:
		pl_w->routine_3 ++ ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		break ;
	}
}

/*--------------------------------------------------------------*/
static void help_miss( PLAYER_WORK *pl_w, PL20_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( lw->catch_flag < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void help_hold( PLAYER_WORK *pl_w, PL20_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( !--pl_w->timer0 ){
		pl_w->routine_3 ++ ;
		pl_w->timer0 = 60 ;
		lw->help_phase = 1 ;
		pl_w->timer1 = 0 ;
		pl_w->dir = 5 ;
		pls20_00_set( pl_w, 2, 0 ) ;
	}
}

/*==============================================================*/
/*			Mashing the buttons makes it last longer			*/
/*==============================================================*/
static void help_mash( PLAYER_WORK *pl_w, PL20_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->dir && sw_renda_ck( pl_w ) ){
		pl_w->timer1 ++ ;
		if( pl_w->timer1 > 4 ){
			pl_w->timer1 = 0 ;
			pl_w->timer0 += 4 ;
			pl_w->dir -- ;
		}
	}
	if( vs_sys_work.vs_st >= VS_END_WAIT || !--pl_w->timer0 ){
		pl_w->routine_3 ++ ;
		lw->help_phase = 2 ;
		pl_w->timer0 = 24 ;
		pl_char_set( pl_w, Char_Super, 6 ) ;
	}
}

/*--------------------------------------------------------------*/
static void help_last( PLAYER_WORK *pl_w, PL20_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( !--pl_w->timer0 ){
		pl_w->routine_3 ++ ;
		lw->help_phase = 3 ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void help_end( PLAYER_WORK *pl_w, PL20_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void ssand_init( PLAYER_WORK *pl_w );
static void ssand_wait( PLAYER_WORK *pl_w );
static void ssand_shot( PLAYER_WORK *pl_w );
static void ssand_hold( PLAYER_WORK *pl_w );
static void ssand_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo that sends out an effect and holds	*/
/*			the screen											*/
/*==============================================================*/
static void sp_ssand( PLAYER_WORK *pl_w )
{
	static void ( *ssand_jmp[])( PLAYER_WORK *pl_w ) = {
		ssand_init, ssand_wait, ssand_shot, ssand_hold, ssand_end,
	} ;

	ssand_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void ssand_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl20_at_ssand ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 10 ) ;
}

/*--------------------------------------------------------------*/
static void ssand_wait( PLAYER_WORK *pl_w )
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
		scr_excep_set( pl_w, 1, 13 ) ;
		ofs.x = -DOT_X( 40.0f ) ;
		ofs.y = DOT_Y( 11.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void ssand_shot( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		shl20_00_set( pl_w, 4 ) ;
		pl_w->timer0 = 104 ;
		pl_w->timer1 = 27 ;
	}
}

/*--------------------------------------------------------------*/
static void ssand_hold( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	/* counted down here, and nothing is done with it */
	if( !--pl_w->timer1 ){
	}
	if( !--pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_char_set( pl_w, Char_Super, 11 ) ;
	}
}

/*--------------------------------------------------------------*/
static void ssand_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		scr_excep_clr() ;
		pl_stand_ret( pl_w ) ;
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
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_CROUCH ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = pl20_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 27 ) ;
}

/*==============================================================*/
/*			The animation's marks send out an effect and a		*/
/*			sound												*/
/*==============================================================*/
static void snapback_move( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type & 1 ){
		pl_w->cg_type = 0 ;
		ofs.x = -55.0f ;
		ofs.y = DOT_Y( 27.0f ) ;
		ofs.z = 0.0f ;
		pl_snap_flash( pl_w, &ofs ) ;
	} else if( pl_w->cg_type & 2 ){
		pl_w->cg_type = 0 ;
		pl_se_req( pl_w, SE_SWING_H ) ;
	}
}

/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_char_set( pl_w, Char_Extra, 6 ) ;
	} else if( pl_char_move( pl_w ) < 0 ){
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void p_hold( PLAYER_WORK *pl_w );
static void k_hold( PLAYER_WORK *pl_w );
static void hellcat_hold( PLAYER_WORK *pl_w );
static void hold_dummy( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The opponent's side of a throw						*/
/****************************************************************/
void	pl20_hold( PLAYER_WORK *pl_w )
{
	static void ( *hold_jmp[])( PLAYER_WORK *pl_w ) = {
		p_hold, k_hold, k_hold, hellcat_hold, hold_dummy,
	} ;

	hold_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void p_hold( PLAYER_WORK *pl_w )
{
	if( pl_w->catch_w->routine_2 > 0 ) catch_em_pos_set0( pl_w->catch_w, pl_w ) ;
	else catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/*==============================================================*/
static void k_hold( PLAYER_WORK *pl_w )
{
	catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/*==============================================================*/
static void hellcat_hold( PLAYER_WORK *pl_w )
{
	if( pl_w->catch_w->routine_2 > 0 ) catch_pos_set( pl_w->catch_w, pl_w ) ;
	else catch_em_pos_set0( pl_w->catch_w, pl_w ) ;
}

/*==============================================================*/
static void hold_dummy( PLAYER_WORK *pl_w )
{
}

static PLAYER_WORK *nml_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *crouch_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *jump_catch_ck( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Can a throw start?									*/
/****************************************************************/
PLAYER_WORK *pl20_catch_ck( PLAYER_WORK *pl_w )
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

static void p_set( PLAYER_WORK *pl_w );
static void k_set( PLAYER_WORK *pl_w );
static void jump_set( PLAYER_WORK *pl_w );
static void hellcat_set( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Start the throw that pl20_catch_ck chose			*/
/****************************************************************/
void	pl20_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		p_set, k_set, jump_set, hellcat_set,
	} ;

	catch_set_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	scr_excep_set( pl_w, 6, 6 ) ;
	catch_gauge_add( pl_w ) ;
	pl_w->hit_stop = 10 ;
	ofs.x = -80.0f ;
	ofs.y = DOT_Y( 60.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_char_set( pl_w, Char_Catch, 0 ) ;
}

/*==============================================================*/
static void k_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	scr_excep_set( pl_w, 5, 5 ) ;
	catch_gauge_add( pl_w ) ;
	pl_w->hit_stop = 10 ;
	ofs.x = -80.0f ;
	ofs.y = DOT_Y( 60.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_char_set( pl_w, Char_Catch, 2 ) ;
}

/*==============================================================*/
static void jump_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	scr_excep_set( pl_w, 5, 5 ) ;
	catch_gauge_add( pl_w ) ;
	pl_w->hit_stop = 10 ;
	ofs.x = -55.0f ;
	ofs.y = DOT_Y( 55.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	if( !( pl_w->dir & 2 ) ){
		pl_w->pl_dir ^= 1 ;
		pl_w->disp_dir = pl_w->pl_dir ;
	}
	pl_char_set( pl_w, Char_Catch, 4 ) ;
}

/*==============================================================*/
static void hellcat_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	scr_excep_set( pl_w, 6, 6 ) ;
	pl_w->hit_stop = 10 ;
	ofs.x = -120.0f ;
	ofs.y = 120.0f ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	if( PL_CK( pl_w->catch_w, PL_SERVBOT ) ) pl_w->catch_w->pos.y = pl_w->ground_y + DOT_Y( 32.0f ) ;
	pl_char_set( pl_w, Char_Catch, 6 ) ;
}

static void p_catch( PLAYER_WORK *pl_w );
static void k_catch( PLAYER_WORK *pl_w );
static void jump_catch( PLAYER_WORK *pl_w );
static void hellcat_catch( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void	pl20_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		p_catch, k_catch, jump_catch, hellcat_catch,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

static void p_catch_hold( PLAYER_WORK *pl_w );
static void p_catch_hit( PLAYER_WORK *pl_w );
static void p_catch_throw( PLAYER_WORK *pl_w );
static void p_catch_fall( PLAYER_WORK *pl_w );
static void p_catch_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The punch throw: hits on the held opponent until	*/
/*			the time runs out, then a throw						*/
/*==============================================================*/
static void p_catch( PLAYER_WORK *pl_w )
{
	static void ( *p_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		p_catch_hold, p_catch_hit, p_catch_throw, p_catch_fall, p_catch_land,
	} ;

	p_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void p_catch_hold( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		catch_renda_set( pl_w, 3 ) ;
		hold_renda_set( pl_w->catch_w, 48 ) ;
		pl_w->timer0 = 120 ;
		pl_w->timer1 = 0 ;
		if( !( pl_w->dir & 2 ) ){
			pl_w->pl_dir ^= 1 ;
			pl_w->disp_dir = pl_w->pl_dir ;
		}
		if( !SMALL_CK( pl_w->catch_w ) ) pl_w->catch_w->pos.y = pl_w->ground_y ;
	}
}

/*==============================================================*/
/*			A hit on each of the animation's marks; unk_1e		*/
/*			ends it												*/
/*==============================================================*/
static void p_catch_hit( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->catch_w ;
	VEC3 ofs ;

	pl_char_move( pl_w ) ;
	if( catch_renda_ck( pl_w ) ) pl_w->cg_ctr = 1 ;
	if( ( !pl_w->timer1 && hold_renda_ck( pl_w->catch_w ) )
	 || ( !pl_w->timer1 && !--pl_w->timer0 )
	 || ( !pl_w->timer1 && !em->vital_new ) ) pl_w->timer1 = 1 ;
	if( !pl_w->cg_type ) return ;
	else if( pl_w->cg_type > 0 ){
		em->atck_no = ( pl_w->cg_type & 1 ) ? 33 : 34 ;
		catch_damage( pl_w ) ;
		pl_se_req( pl_w, 15 ) ;
		pl_w->cg_type = 0 ;
		ofs.x = -110.0f ;
		ofs.y = DOT_Y( 65.0f ) ;
		ofs.z = 0.0f ;
		hit_mark_set( pl_w, &ofs, 1 ) ;
	} else if( pl_w->timer1 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Catch, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void p_catch_throw( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->atck_no = pl20_at_throw_p ;
		SCR_EXCEP_SET ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 4.0f ) : DOT_X( 4.0f ) ;
		pl_w->acc.x = pl_w->pl_dir ? 0.032552082f : -0.032552082f ;
		pl_w->spd.y = DOT_Y( 9.0f ) ;
		pl_w->acc.y = -1.5401785f ;
		if( !pl_w->pl_dir ) pl_w->pos.x += DOT_X( 32.0f ) ;
		else pl_w->pos.x -= DOT_X( 32.0f ) ;
		pl_w->pos.y += DOT_Y( 24.0f ) ;
	}
}

/*--------------------------------------------------------------*/
static void p_catch_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->spd.y > 0.0f ) return ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Jump, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void p_catch_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void k_catch_turn( PLAYER_WORK *pl_w );
static void k_catch_jump( PLAYER_WORK *pl_w );
static void k_catch_fall( PLAYER_WORK *pl_w );
static void k_catch_throw( PLAYER_WORK *pl_w );
static void k_catch_end( PLAYER_WORK *pl_w );
static void k_catch_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The kick throw: a leap with the opponent			*/
/*==============================================================*/
static void k_catch( PLAYER_WORK *pl_w )
{
	static void ( *k_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		k_catch_turn, k_catch_jump, k_catch_fall, k_catch_throw, k_catch_end, k_catch_land,
	} ;

	k_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void k_catch_turn( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		if( !( pl_w->dir & 1 ) ){
			pl_w->pl_dir ^= 1 ;
			pl_w->disp_dir = pl_w->pl_dir ;
		}
	}
}

/*--------------------------------------------------------------*/
static void k_catch_jump( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->spd.x = pl_w->pl_dir ? -10.0f : 10.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 4.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.25f ) ;
	}
}

/*--------------------------------------------------------------*/
static void k_catch_fall( PLAYER_WORK *pl_w )
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
		pl_land_ef_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void k_catch_throw( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_char_move( pl_w ) ;
	pl_w->routine_2 ++ ;
	em = pl_w->catch_w ;
	em->dm_hit_w = pl_w ;
	em->catch_type = 1 ;
	em->atck_no = pl20_at_throw_k ;
	SCR_EXCEP_SET ;
}

/*--------------------------------------------------------------*/
static void k_catch_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->pl_dir = pl_w->disp_dir = pl_w->pl_dir ^ 1 ;
		pl_char_set( pl_w, Char_Catch, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void k_catch_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void jump_catch_throw( PLAYER_WORK *pl_w );
static void jump_catch_push( PLAYER_WORK *pl_w );
static void jump_catch_fall( PLAYER_WORK *pl_w );
static void jump_catch_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The air throw										*/
/*==============================================================*/
static void jump_catch( PLAYER_WORK *pl_w )
{
	static void ( *jump_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		jump_catch_throw, jump_catch_push, jump_catch_fall, jump_catch_land,
	} ;

	jump_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void jump_catch_throw( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->atck_no = pl20_at_air_throw ;
		SCR_EXCEP_SET ;
	}
}

/*--------------------------------------------------------------*/
static void jump_catch_push( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 2.0f ) : DOT_X( 2.0f ) ;
		pl_w->acc.y = -0.9375f ;
		if( pl_w->pl_dir ) pl_w->pos.x -= 40.0f ;
		else pl_w->pos.x += 40.0f ;
		pl_w->pos.y += DOT_Y( 12.0f ) ;
	}
}

/*--------------------------------------------------------------*/
static void jump_catch_fall( PLAYER_WORK *pl_w )
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
		pl_char_set( pl_w, Char_Catch, 5 ) ;
	}
}

/*--------------------------------------------------------------*/
static void jump_catch_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void hellcat_catch_hit( PLAYER_WORK *pl_w );
static void hellcat_catch_throw( PLAYER_WORK *pl_w );
static void hellcat_catch_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Hell Cat's throw: hits, then a step past the		*/
/*			opponent and a throw								*/
/*==============================================================*/
static void hellcat_catch( PLAYER_WORK *pl_w )
{
	static void ( *hellcat_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		hellcat_catch_hit, hellcat_catch_throw, hellcat_catch_land,
	} ;

	hellcat_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void hellcat_catch_hit( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type > 0 ){
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->atck_no = pl20_at_hellcat_hit ;
		catch_damage( pl_w ) ;
		ofs.x = 0.0f ;
		ofs.y = -DOT_Y( 3.0f ) ;
		ofs.z = 0.0f ;
		hit_mark_set( pl_w, &ofs, 1 ) ;
		pl_se_req( pl_w, 15 ) ;
	}
	if( pl_w->cg_type < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		SCR_EXCEP_SET ;
		pl_w->pos.x += pl_w->pl_dir ? DOT_X( 100.0f ) : -DOT_X( 100.0f ) ;
		pl_w->pos.y = pl_w->ground_y ;
	}
}

/*--------------------------------------------------------------*/
static void hellcat_catch_throw( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->atck_no = pl20_at_hellcat_throw ;
		SCR_EXCEP_SET ;
	}
}

/*--------------------------------------------------------------*/
static void hellcat_catch_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
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
void	pl20_t_kick( PLAYER_WORK *pl_w )
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
	pl_w->atck_no = pl20_at_t_kick ;
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
	} else if( pl_w->cg_type ) pl_w->cg_type = 0 ;
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void	pl20_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl20_sp_ssand ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl20_sp_ssand ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl20_sp_ssand ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl20_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl20_sp_ssand ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl20_sp_ssand ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl20_sp_ssand ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl20_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl20_sp_delta ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl20_sp_sand ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl20_sp_spike ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void	pl20_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl20_sp_delta ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl20_sp_spike ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl20_sp_sand ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

static void tjump_init( PLAYER_WORK *pl_w );
static void tjump_wait( PLAYER_WORK *pl_w );
static void tjump_fall( PLAYER_WORK *pl_w );
static void tjump_land( PLAYER_WORK *pl_w );
static void tjump_pos( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Coming in from above the screen						*/
/****************************************************************/
void	pl20_tjump( PLAYER_WORK *pl_w )
{
	static void ( *tjump_jmp[])( PLAYER_WORK *pl_w ) = {
		tjump_init, tjump_wait, tjump_fall, tjump_land,
	} ;

	tjump_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			Runs on into tjump_wait								*/
/*==============================================================*/
static void tjump_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->jump_dash_cnt ++ ;
	pl_w->jump_com_cnt = 0 ;
	pl_w->jump_atk_cnt = JATK_ONE ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->timer0 = 60 ;
	pl_w->disp_dir = pl_w->pl_dir ^ 1 ;
	pl_char_set( pl_w, Char_Extra, 2 ) ;
	tjump_wait( pl_w ) ;
}

/*==============================================================*/
/*			Waits for the time, a button or the stick let go	*/
/*==============================================================*/
static void tjump_wait( PLAYER_WORK *pl_w )
{
	Uint8 no ;

	tjump_pos( pl_w ) ;
	if( !--pl_w->timer0 || pl_w->pl_new_lvbt & KEY_DOWN ){
		pl_w->routine_2 ++ ;
		no = 3 ;
	} else if( !( pl_w->pl_sw_new & KEY_BACK ) ){
		pl_w->routine_2 += 2 ;
		no = 4 ;
	} else return ;
	pl_char_set( pl_w, Char_Extra, no ) ;
}

/*--------------------------------------------------------------*/
static void tjump_fall( PLAYER_WORK *pl_w )
{
	tjump_pos( pl_w ) ;
	pl_w->pos.y -= 2.142857f ;
	if( pl_w->pos.y <= pl_w->ground_y + DOT_Y( 48.0f ) || !( pl_w->pl_sw_new & KEY_BACK ) ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Extra, 4 ) ;
	}
}

/*--------------------------------------------------------------*/
static void tjump_land( PLAYER_WORK *pl_w )
{
	tjump_pos( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		act_st_set( pl_w, N_JUMP ) ;
		pl_w->routine_2 = 1 ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->disp_dir = pl_w->pl_dir ;
		pl_char_set2( pl_w, Char_Jump, 1, 3 ) ;
	}
}

/*==============================================================*/
/*			Keeps the player at the side of the stage it		*/
/*			faces away from										*/
/*==============================================================*/
static void tjump_pos( PLAYER_WORK *pl_w )
{
	Float cx = scr_work.eye[0].x ;

	if( pl_w->disp_dir ) pl_w->pos.x = cx + 288.33334f ;
	else pl_w->pos.x = cx - 288.33334f ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl20.c" End of File	*/

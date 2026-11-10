/* Hulk (character 0d): command checks, normal attacks, appearance, win and lose poses, special
   moves and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl0d_def.h"

void pl0d_main( PLAYER_WORK *pl_w );
void pl0d_com_ck( PLAYER_WORK *pl_w );
void pl0d_atck_set( PLAYER_WORK *pl_w );
void pl0d_atck( PLAYER_WORK *pl_w );
void pl0d_dash( PLAYER_WORK *pl_w );
void pl0d_noki( PLAYER_WORK *pl_w );
void pl0d_appear( PLAYER_WORK *pl_w );
void pl0d_win( PLAYER_WORK *pl_w );
void pl0d_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl0d_catch_ck( PLAYER_WORK *pl_w );
void pl0d_catch_set( PLAYER_WORK *pl_w );
void pl0d_catch( PLAYER_WORK *pl_w );
void pl0d_hold( PLAYER_WORK *pl_w );
int pl0d_delay_ck( PLAYER_WORK *pl_w );
void pl0d_t_kick( PLAYER_WORK *pl_w );
void pl0d_upper( PLAYER_WORK *pl_w );
void pl0d_sp_unison( PLAYER_WORK *pl_w );
void pl0d_sp_gattai( PLAYER_WORK *pl_w );
void pl0d_sp_assist( PLAYER_WORK *pl_w );
void pl0d_sp_zero( PLAYER_WORK *pl_w );

/* each attack's table: standing and crouching, jumping, super jumping */
static Sint8 lp_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 mp_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 hp_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 lk_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 mk_tbl[4] = { -1, -1, -1, -1 } ;
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
static Uint16 com_charge[] = { 40, COM_GROUND, pl0d_sp_charge, 0, KEY_BACK, KEY_FORWARD, KEY_KICK } ;	/* Gamma Charge */
static Uint16 com_ucharge[] = { 40, COM_GROUND, pl0d_sp_ucharge, 0, KEY_DOWN, KEY_UP, KEY_KICK } ;	/* Gamma Charge, upward */
static Uint16 com_slam[] = { 3, COM_GROUND, pl0d_sp_slam, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Gamma Slam */
static Uint16 com_tornado[] = { 5, COM_GROUND, pl0d_sp_tornado, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Gamma Tornado */
static Uint16 com_crush[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl0d_sp_crush, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Gamma Crush */
static Uint16 com_wave[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl0d_sp_wave, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Gamma Wave (hyper) */
static Uint16 com_quake[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl0d_sp_quake, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Gamma Quake */

static void ( *pl0d_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl0d_main,
	pl0d_com_ck,
	pl0d_atck_set,
	pl0d_atck,
	pl0d_dash,
	pl0d_noki,
	pl0d_appear,
	pl0d_win,
	pl0d_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl0d_catch_ck,	/* returns a value */
	pl0d_catch_set,
	pl0d_catch,
	pl0d_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl0d_delay_ck,
	pl0d_t_kick,
	pl0d_upper,
	pl0d_sp_unison,
	pl0d_sp_gattai,
	pl0d_sp_assist,
	pl0d_sp_zero,
} ;

static Uint8 charge_ck( PLAYER_WORK *pl_w );
static Uint8 ucharge_ck( PLAYER_WORK *pl_w );
static Uint8 slam_ck( PLAYER_WORK *pl_w );
static Uint8 tornado_ck( PLAYER_WORK *pl_w );
static Uint8 crush_ck( PLAYER_WORK *pl_w );
static Uint8 wave_ck( PLAYER_WORK *pl_w );
static Uint8 quake_ck( PLAYER_WORK *pl_w );
static int chouhatsu_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static int crush_delay_ck( PLAYER_WORK *pl_w );
static int wave_delay_ck( PLAYER_WORK *pl_w );
static int quake_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl0d_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl0d_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl0d_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( crush_ck( pl_w ) ) return ;
	if( wave_ck( pl_w ) ) return ;
	if( quake_ck( pl_w ) ) return ;
	if( charge_ck( pl_w ) ) return ;
	if( ucharge_ck( pl_w ) ) return ;
	if( slam_ck( pl_w ) ) return ;
	if( tornado_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
static Uint8 charge_ck( PLAYER_WORK *pl_w )
{
	if( !com_tame_ck( pl_w, com_charge, &pl_w->com_w0 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	if( !pl_w->team_no ) pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0d_sp_charge ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 ucharge_ck( PLAYER_WORK *pl_w )
{
	if( !com_tame_ck( pl_w, com_ucharge, &pl_w->com_w1 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0d_sp_ucharge ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 slam_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_slam, &pl_w->com_w2 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0d_sp_slam ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 tornado_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_tornado, &pl_w->com_w3 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w3 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0d_sp_tornado ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 crush_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_crush, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0d_sp_crush ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 wave_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_wave, &pl_w->com_w5 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0d_sp_wave ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 quake_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_quake, &pl_w->com_w6 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0d_sp_quake ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static int chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 9 ) ) return 0 ;
	pl_w->pl_sp_type = pl0d_sp_chouhatsu ;
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
	pl_w->pl_sp_type = pl0d_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl0d_delay_ck( PLAYER_WORK *pl_w )
{
	if( crush_delay_ck( pl_w ) || wave_delay_ck( pl_w ) || quake_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static int crush_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_crush, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 3 ;
	return 1 ;
}

/*==============================================================*/
static int wave_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_wave, &pl_w->com_w5 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 6 ;
	return 1 ;
}

/*==============================================================*/
static int quake_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_quake, &pl_w->com_w6 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 10 ;
	return 1 ;
}

static void charge_lever_ck( PLAYER_WORK *pl_w );
static void crush_timer( PLAYER_WORK *pl_w );

/****************************************************************/
void pl0d_main( PLAYER_WORK *pl_w )
{
	pl_w->armor_timer = 2 ;
	charge_lever_ck( pl_w ) ;
	crush_timer( pl_w ) ;
}

/* the kick and the stick that steer sp_charge and sp_ucharge, while they are charged */
static Sint8 charge_dir_tbl[16] = { 0, 1, 2, 0, 0, 1, 2, 0, 0, 1, 2, 0, 0, 0, 0, 0 } ;
static Sint8 ucharge_dir_tbl[16] = { 2, 2, 0, 0, 2, 2, 0, 0, 2, 2, 1, 0, 0, 0, 0, 0 } ;
/* for the computer: the kick, then the way, by atck_pk */
static Sint8 charge_com_tbl[20] = { 0, 0, 1, 0, 0, 1, 1, 1, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0 } ;

/*==============================================================*/
static void charge_lever_ck( PLAYER_WORK *pl_w )
{
	Uint16 sw ;

	if( ( pl_w->pl_sp_type == pl0d_sp_charge || pl_w->pl_sp_type == pl0d_sp_ucharge ) && !pl_w->routine_2 ){
		if( pl_w->routine_3 == 1 || pl_w->routine_3 == 2 ){
			if( !pl_w->cpu_flag ){
				if( pl_w->pl_sw_now & KEY_KICK ){
					if( pl_w->pl_sw_now & KEY_LK ) pl_w->type = 0 ;
					else pl_w->type = 1 ;
					sw = ( pl_w->pl_sw_new & KEY_LEVER ) >> 10 ;
					if( pl_w->pl_sp_type == pl0d_sp_charge ) pl_w->sub_type = charge_dir_tbl[sw] ;
					else pl_w->sub_type = ucharge_dir_tbl[sw] ;
					pl_w->sub_no = 1 ;
				} else pl_w->sub_no = 0 ;
			} else if( !pl_w->team_no ){
				pl_w->type = charge_com_tbl[pl_w->atck_pk * 2] ;
				pl_w->sub_type = charge_com_tbl[pl_w->atck_pk * 2 + 1] ;
				pl_w->sub_no = 1 ;
			}
			if( pl_w->hit_flag ){
				if( pl_w->hit_flag & HR_GUARD ) pl_w->sub_no = 0 ;
			}
		}
	}
}

/*==============================================================*/
static void crush_timer( PLAYER_WORK *pl_w )
{
	PL0D_LOCAL_W *lw = ( PL0D_LOCAL_W * )pl_w->tlocal ;

	if( pl_w->pl_sp_type == pl0d_sp_crush && lw->crush_time ){
		lw->crush_time -- ;
		pl_w->wall_ng_timer = 2 ;
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
void pl0d_atck_set( PLAYER_WORK *pl_w )
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
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_STAND_MP ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		pl_se_req( pl_w, SE_SWING_M ) ;
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
static void crouch_punch( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_CROUCH_LP ;
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		pl_se_req( pl_w, SE_SWING_L ) ;
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
void pl0d_atck( PLAYER_WORK *pl_w )
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
/*			The heavy one hits twice							*/
/*==============================================================*/
static void crouch_punch_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	case 2:
		if( pl_char_move( pl_w ) < 0 ){
			pl_stand_ret( pl_w ) ;
			return ;
		}
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pl_w->atck_no = pl0d_at_crouch_hp_2 ;
			HIT_RESET( pl_w ) ;
		}
		break ;
	}
}

/*==============================================================*/
static void stand_kick_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	case 2:
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

static void jump_lp_move( PLAYER_WORK *pl_w );
static void jump_mp_move( PLAYER_WORK *pl_w );
static void jump_hp_move( PLAYER_WORK *pl_w );
static void jump_lk_move( PLAYER_WORK *pl_w );
static void jump_mk_move( PLAYER_WORK *pl_w );
static void jump_hk_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void jump_punch_move( PLAYER_WORK *pl_w )
{
	static void ( *jump_punch_jmp[])( PLAYER_WORK *pl_w ) = {
		jump_lp_move, jump_mp_move, jump_hp_move,
	} ;

	jump_punch_jmp[pl_w->nml_no]( pl_w ) ;
}

/*==============================================================*/
static void jump_lp_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/*==============================================================*/
static void jump_mp_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/*==============================================================*/
/*			The heavy punch stops in the air					*/
/*==============================================================*/
static void jump_hp_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_jump_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		RATE_CLR_X( pl_w ) ;
		pl_w->spd.y = DOT_Y( 3.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pls0d_00_set( pl_w, 0, 0 ) ;
		pls0d_00_set( pl_w, 0, 1 ) ;
	}
}

/*==============================================================*/
static void jump_kick_move( PLAYER_WORK *pl_w )
{
	static void ( *jump_kick_jmp[])( PLAYER_WORK *pl_w ) = {
		jump_lk_move, jump_mk_move, jump_hk_move,
	} ;

	jump_kick_jmp[pl_w->nml_no]( pl_w ) ;
}

/*==============================================================*/
static void jump_lk_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/*==============================================================*/
static void jump_mk_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/*==============================================================*/
static void jump_hk_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

static void upper_init( PLAYER_WORK *pl_w );
static void upper_p_move( PLAYER_WORK *pl_w );
static void upper_k_move( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The launchers: the punch hits twice					*/
/****************************************************************/
void pl0d_upper( PLAYER_WORK *pl_w )
{
	static void ( *upper_jmp[])( PLAYER_WORK *pl_w ) = {
		upper_init, upper_p_move, upper_k_move,
	} ;

	upper_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void upper_init( PLAYER_WORK *pl_w )
{
	pl_brake_set( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	if( !pl_w->atck_pk ){
		pl_w->routine_2 = 1 ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_char_set( pl_w, Char_Extra, 3 ) ;
		pl_w->atck_no = pl0d_at_upper_p ;
		HIT_RESET( pl_w ) ;
		upper_p_move( pl_w ) ;
	} else {
		pl_w->routine_2 = 2 ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Extra, 4 ) ;
		pl_w->atck_no = pl0d_at_upper_k ;
		HIT_RESET( pl_w ) ;
		upper_k_move( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void upper_p_move( PLAYER_WORK *pl_w )
{
	if( pl_w->atck_type == 3 ) pl_smoke_set( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_brake_stop( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->atck_no = pl0d_at_upper_p_2 ;
		HIT_RESET( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void upper_k_move( PLAYER_WORK *pl_w )
{
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
void pl0d_dash( PLAYER_WORK *pl_w )
{
	static void ( *dash_jmp[])( PLAYER_WORK *pl_w ) = {
		dash_init, dash_move, dash_stop, dash_end,
	} ;

	dash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			A low hop forward									*/
/*==============================================================*/
static void dash_init( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.y = 2.142857f ;
		pl_w->acc.y = -DOT_Y( 0.125f ) ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 8.0f ) : -DOT_X( 8.0f ) ;
		pl_w->acc.x = pl_w->pl_dir ? -0.46875f : 0.46875f ;
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
	if( pl_w->ground_y >= pl_w->pos.y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Dash, 2 ) ;
		pl_w->routine_2 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void dash_stop( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->spd.x * pl_w->acc.x > 0.0f ){
		pl_w->routine_2 ++ ;
		RATE_CLR_X( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void dash_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void noki_init( PLAYER_WORK *pl_w );
static void noki_move( PLAYER_WORK *pl_w );
static void noki_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash										*/
/****************************************************************/
void pl0d_noki( PLAYER_WORK *pl_w )
{
	static void ( *noki_jmp[])( PLAYER_WORK *pl_w ) = {
		noki_init, noki_move, noki_end,
	} ;

	noki_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void noki_init( PLAYER_WORK *pl_w )
{
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->routine_2 ++ ;
	pl_w->spd.x = pl_w->pl_dir ? -12.5f : 12.5f ;
	noki_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void noki_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->char_no.b.grp = 2 ;
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
	/* only the speed is under the if, as the arcade's code has it */
	if( pl_w->cg_type ) pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void appear_init( PLAYER_WORK *pl_w );
static void appear_fall( PLAYER_WORK *pl_w );
static void appear_move( PLAYER_WORK *pl_w );

/****************************************************************/
/*			He drops in from above								*/
/****************************************************************/
void pl0d_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_init, appear_fall, appear_move,
	} ;

	appear_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void appear_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->disp_flag = 1 ;
	pl_w->pos.y += 480.0f ;
	pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = -DOT_Y( 4.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.25f ) ;
	pl_w->char_no.b.grp = Char_Appear ;
	pl_w->char_no.b.no = 0 ;
	pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
}

/*--------------------------------------------------------------*/
static void appear_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->ground_y >= pl_w->pos.y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		scr_work.quake_level = 3 ;
		scr_work.quake_flag = 1 ;
		pl_w->char_no.b.grp = Char_Appear ;
		pl_w->char_no.b.no = 1 ;
		pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
		pls0d_00_set( pl_w, 2, 0 ) ;
		pls0d_00_set( pl_w, 2, 1 ) ;
		pl_w->routine_2 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void appear_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_w->routine_1 ++ ;
}

static void win_ko_init( PLAYER_WORK *pl_w );
static void win_ko_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void win_ko_init( PLAYER_WORK *pl_w )
{
	if( !( Rnd() & 1 ) ) pl_w->char_no.b.no = 0 ;
	else pl_w->char_no.b.no = 1 ;
	pl_char_set( pl_w, Char_Win, pl_w->char_no.b.no ) ;
	pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void win_ko_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void win_time_ko( PLAYER_WORK *pl_w )
{
	static void ( *win_ko_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko_init, win_ko_move,
	} ;

	win_ko_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void lose_time_ko( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, 2 ) ;
	} else pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void win_time( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, 0 ) ;
	} else pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void draw_time( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, 2 ) ;
	} else pl_char_move( pl_w ) ;
}

/****************************************************************/
void pl0d_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_time_ko, lose_time_ko, win_time, lose_time_ko, draw_time,
	} ;

	if( pl_win_end_ck( pl_w ) ) pl_stand_ret( pl_w ) ;
	else win_jmp[pl_w->type]( pl_w ) ;
}

/*==============================================================*/
/*			The partner's flying kick							*/
/*==============================================================*/
static void t_kick_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->spd.x = -30.0f ;
	if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = -DOT_Y( 0.125f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->atck_no = pl0d_at_t_kick ;
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
		pl_char_set( pl_w, Char_Extra, 1 ) ;
		pl_land_ef_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void t_kick_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stop_stand_ret( pl_w ) ;
}

/****************************************************************/
void pl0d_t_kick( PLAYER_WORK *pl_w )
{
	static void ( *t_kick_jmp[])( PLAYER_WORK *pl_w ) = {
		t_kick_init, t_kick_move, t_kick_land,
	} ;

	t_kick_jmp[pl_w->routine_2]( pl_w ) ;
}

/* Gamma Charge: the charge along the ground, in 16.16 by strength, and how long it runs */
static Sint32 charge_spd[2] = { 0x000a0000, 0x000c0000 } ;
static Sint8 charge_time[2] = { 0x10, 0x24 } ;

static void charge_g_init( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw );
static void charge_g_start( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw );
static void charge_g_move( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw );
static void charge_g_end( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw );

/*==============================================================*/
static void charge_g_init( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	pl_w->routine_3 ++ ;
	pl_w->sub_no = 0 ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->atck_no = pl_w->atck_level * 2 + 0x30 ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level ) ;
	charge_g_start( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void charge_g_start( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	Sint32 spd ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		spd = charge_spd[pl_w->atck_level] ;
		pl_w->spd.x = pl_w->pl_dir ? spd * 1.6666666f / 65536.0f : -( spd * 1.6666666f / 65536.0f ) ;
		pl_w->acc.x = pl_w->pl_dir ? -0.104166664f : 0.104166664f ;
		pl_w->timer0 = charge_time[pl_w->atck_level] ;
		pl_w->timer1 = 8 ;
		pl_voice_req( pl_w, 20 ) ;
		pl_voice_req( pl_w, 32 ) ;
		pls0d_00_set( pl_w, 5, 0 ) ;
	}
}

/*==============================================================*/
/*			A kick held after the first frames turns it into	*/
/*			the leap											*/
/*==============================================================*/
static void charge_g_move( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( !pl_w->timer1 ){
		if( pl_w->sub_no ){
			pl_w->routine_2 ++ ;
			pl_w->routine_3 = 0 ;
			return ;
		}
	} else pl_w->timer1 -- ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_3 ++ ;
		pl_w->char_no.b.grp = 21 ;
		pl_w->char_no.b.no = pl_w->atck_level + 2 ;
		pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 4.0f ) : -DOT_X( 4.0f ) ;
		pl_w->acc.x = pl_w->pl_dir ? -DOT_X( 0.125f ) : DOT_X( 0.125f ) ;
	}
}

/*--------------------------------------------------------------*/
static void charge_g_end( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	/* only the speed is under the if, as in noki_end */
	if( pl_w->cg_type ) pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void charge_ground( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	static void ( *charge_g_jmp[])( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw ) = {
		charge_g_init, charge_g_start, charge_g_move, charge_g_end,
	} ;

	charge_g_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/* the leap: its attack and animation by the way (sub_type) and the kick (type), and its speeds */
static Uint8 charge_a_atck[6] = { 0x33, 0x35, 0x36, 0x38, 0x39, 0x3b } ;
static Uint8 charge_a_char[6] = { 4, 5, 6, 7, 8, 9 } ;
static Sint32 charge_a_spd[3][4] = {
	{ 0x00000000, 0x00000000, 0x000c0000, -0x00004800 },
	{ -0x00080000, 0x00002000, 0x000c0000, -0x00004800 },
	{ 0x00080000, -0x00002000, 0x000a0000, -0x00004800 },
} ;

static void charge_a_init( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw );
static void charge_a_start( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw );
static void charge_a_move( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw );
static void charge_a_fall( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw );
static void charge_a_land( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw );
static void charge_a_end( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw );

/*==============================================================*/
static void charge_a_init( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	Uint8 no ;

	pl_w->routine_3 ++ ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_air_set( pl_w ) ;
	no = charge_a_atck[pl_w->type + pl_w->sub_type * 2] ;
	pl_w->atck_no = no ;
	HIT_RESET( pl_w ) ;
	pl_w->char_no.b.grp = 21 ;
	pl_w->char_no.b.no = charge_a_char[pl_w->type + pl_w->sub_type * 2] ;
	pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
	charge_a_start( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void charge_a_start( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	Sint32 *spd ;
	Sint32 sx, ax, sy, ay ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->timer0 = 30 ;
		spd = charge_a_spd[pl_w->sub_type] ;
		sx = spd[0] ;
		ax = spd[1] ;
		sy = spd[2] ;
		ay = spd[3] ;
		pl_w->spd.x = pl_w->pl_dir ? sx * 1.6666666f / 65536.0f : -( sx * 1.6666666f / 65536.0f ) ;
		pl_w->acc.x = pl_w->pl_dir ? ax * 1.6666666f / 65536.0f : -( ax * 1.6666666f / 65536.0f ) ;
		pl_w->spd.y = sy * 2.142857f / 65536.0f ;
		pl_w->acc.y = ay * 2.142857f / 65536.0f ;
		pl_voice_req( pl_w, 20 ) ;
		pl_voice_req( pl_w, 32 ) ;
		pls0d_00_set( pl_w, 5, pl_w->sub_type + 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void charge_a_move( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_3 ++ ;
		pl_w->char_no.b.grp = 21 ;
		pl_w->char_no.b.no = 10 ;
		pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
	}
}

/*--------------------------------------------------------------*/
static void charge_a_fall( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) >= 0 ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_w->sjump_flag = 2 ;
		pl_w->char_no.b.grp = 21 ;
		pl_w->char_no.b.no = 12 ;
		pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
	}
}

/*--------------------------------------------------------------*/
static void charge_a_land( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->ground_y >= pl_w->pos.y ){
		pl_w->routine_3 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_ef_set( pl_w ) ;
		pl_w->char_no.b.grp = 21 ;
		pl_w->char_no.b.no = 13 ;
		pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
	}
}

/*--------------------------------------------------------------*/
static void charge_a_end( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void charge_air( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	static void ( *charge_a_jmp[])( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw ) = {
		charge_a_init, charge_a_start, charge_a_move, charge_a_fall, charge_a_land, charge_a_end,
	} ;

	charge_a_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*==============================================================*/
/*			Gamma Charge: the charge, and the leap a kick		*/
/*			turns it into										*/
/*==============================================================*/
static void sp_charge( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	static void ( *charge_jmp[])( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw ) = {
		charge_ground, charge_air,
	} ;

	charge_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/* Gamma Charge, upward: x then y in 16.16 by strength, and how long it runs */
static Sint32 ucharge_spd[4] = { -0x00020000, 0x00080000, -0x00040000, 0x000e0000 } ;
static Sint8 ucharge_time[2] = { 0x10, 0x20 } ;

static void ucharge_g_init( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw );
static void ucharge_g_start( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw );
static void ucharge_g_move( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw );
static void ucharge_g_fall( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw );
static void ucharge_g_land( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw );
static void ucharge_g_end( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw );

/*==============================================================*/
static void ucharge_g_init( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	pl_w->routine_3 ++ ;
	pl_w->sub_no = 0 ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->atck_no = pl_w->atck_level * 2 + 0x43 ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 14 ) ;
	ucharge_g_start( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void ucharge_g_start( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	Sint32 spd ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		pl_air_set( pl_w ) ;
		spd = ucharge_spd[pl_w->atck_level * 2] ;
		pl_w->spd.x = pl_w->pl_dir ? -( spd * 1.6666666f / 65536.0f ) : spd * 1.6666666f / 65536.0f ;
		pl_w->acc.x = pl_w->pl_dir ? -DOT_X( 0.125f ) : DOT_X( 0.125f ) ;
		spd = ucharge_spd[pl_w->atck_level * 2 + 1] ;
		pl_w->spd.y = spd * 2.142857f / 65536.0f ;
		pl_w->acc.y = -0.46875f ;
		pl_w->timer0 = ucharge_time[pl_w->atck_level] ;
		pl_w->timer1 = 4 ;
		pl_voice_req( pl_w, 20 ) ;
		pl_voice_req( pl_w, 32 ) ;
		pls0d_00_set( pl_w, 5, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void ucharge_g_move( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !pl_w->timer1 ){
		if( pl_w->sub_no ){
			pl_w->routine_2 ++ ;
			pl_w->routine_3 = 0 ;
			return ;
		}
	} else pl_w->timer1 -- ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_3 ++ ;
		pl_w->char_no.b.grp = 21 ;
		pl_w->char_no.b.no = 10 ;
		pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
		pl_w->spd.x = pl_w->spd.x / 4.0f ;
		pl_w->acc.x = pl_w->acc.x / 4.0f ;
	}
}

/*--------------------------------------------------------------*/
static void ucharge_g_fall( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) >= 0 ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_w->sjump_flag = 2 ;
		pl_w->char_no.b.grp = 21 ;
		pl_w->char_no.b.no = 12 ;
		pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
	}
}

/*--------------------------------------------------------------*/
static void ucharge_g_land( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->ground_y >= pl_w->pos.y ){
		pl_w->routine_3 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_ef_set( pl_w ) ;
		pl_w->char_no.b.grp = 21 ;
		pl_w->char_no.b.no = 13 ;
		pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
	}
}

/*--------------------------------------------------------------*/
static void ucharge_g_end( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void ucharge_ground( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	static void ( *ucharge_g_jmp[])( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw ) = {
		ucharge_g_init, ucharge_g_start, ucharge_g_move, ucharge_g_fall, ucharge_g_land, ucharge_g_end,
	} ;

	ucharge_g_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/* the turn in the air: attack, animation and speeds by the way (sub_type) and the kick (type) */
static Uint8 ucharge_a_atck[6] = { 0x46, 0x48, 0x49, 0x4b, 0x4c, 0x4e } ;
static Uint8 ucharge_a_char[6] = { 0x10, 0x11, 0x12, 0x13, 0x14, 0x15 } ;
static Sint32 ucharge_a_spd[24] = {			/* spd.x, acc.x, spd.y, acc.y */
	-0x00060000, 0x00002000, 0x00000000, 0x00000000,
	-0x000a0000, 0x00002000, 0x00000000, 0x00000000,
	-0x00060000, 0x00002000, 0x00080000, -0x00004800,
	-0x000a0000, 0x00002000, 0x000e0000, -0x00004800,
	0x00000000, 0x00000000, 0x00080000, -0x00004800,
	0x00000000, 0x00000000, 0x000e0000, -0x00004800,
} ;
static Uint8 ucharge_a_time[2] = { 0x10, 0x20 } ;
static Sint8 ucharge_a_eff[3] = { 0, 3, 1 } ;

static void ucharge_a_init( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw );
static void ucharge_a_start( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw );
static void ucharge_a_move( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw );
static void ucharge_a_fall( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw );
static void ucharge_a_land( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw );
static void ucharge_a_end( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw );

/*==============================================================*/
static void ucharge_a_init( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	Uint8 no ;

	pl_w->routine_3 ++ ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	no = ucharge_a_atck[pl_w->type + pl_w->sub_type * 2] ;
	pl_w->atck_no = no ;
	HIT_RESET( pl_w ) ;
	pl_w->char_no.b.grp = 21 ;
	pl_w->char_no.b.no = ucharge_a_char[pl_w->type + pl_w->sub_type * 2] ;
	pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
	ucharge_a_start( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void ucharge_a_start( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	Sint32 *spd ;
	Sint32 sx, ax, sy, ay ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		pl_w->timer0 = ucharge_a_time[pl_w->type] ;
		spd = &ucharge_a_spd[pl_w->sub_type * 8 + pl_w->type * 4] ;
		sx = spd[0] ;
		ax = spd[1] ;
		sy = spd[2] ;
		ay = spd[3] ;
		pl_w->spd.x = pl_w->pl_dir ? -( sx * 1.6666666f / 65536.0f ) : sx * 1.6666666f / 65536.0f ;
		pl_w->acc.x = pl_w->pl_dir ? -( ax * 1.6666666f / 65536.0f ) : ax * 1.6666666f / 65536.0f ;
		pl_w->spd.y = sy * 2.142857f / 65536.0f ;
		pl_w->acc.y = ay * 2.142857f / 65536.0f ;
		pl_voice_req( pl_w, 20 ) ;
		pl_voice_req( pl_w, 32 ) ;
		pls0d_00_set( pl_w, 5, ucharge_a_eff[pl_w->sub_type] ) ;
	}
}

/*--------------------------------------------------------------*/
static void ucharge_a_move( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_3 ++ ;
		pl_w->char_no.b.grp = 21 ;
		pl_w->char_no.b.no = 10 ;
		pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
	}
}

/*--------------------------------------------------------------*/
static void ucharge_a_fall( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) >= 0 ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_w->sjump_flag = 2 ;
		pl_w->char_no.b.grp = 21 ;
		pl_w->char_no.b.no = 12 ;
		pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
	}
}

/*--------------------------------------------------------------*/
static void ucharge_a_land( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->ground_y >= pl_w->pos.y ){
		pl_w->routine_3 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_ef_set( pl_w ) ;
		pl_w->char_no.b.grp = 21 ;
		pl_w->char_no.b.no = 13 ;
		pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
	}
}

/*--------------------------------------------------------------*/
static void ucharge_a_end( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void ucharge_air( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	static void ( *ucharge_a_jmp[])( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw ) = {
		ucharge_a_init, ucharge_a_start, ucharge_a_move, ucharge_a_fall, ucharge_a_land, ucharge_a_end,
	} ;

	ucharge_a_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*==============================================================*/
/*			Gamma Charge, upward: the charge, and the turn a	*/
/*			kick gives it										*/
/*==============================================================*/
static void sp_ucharge( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	static void ( *ucharge_jmp[])( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw ) = {
		ucharge_ground, ucharge_air,
	} ;

	ucharge_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/* Gamma Slam: the wave along the ground */
static void slam_init( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw );
static void slam_move( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw );

/*==============================================================*/
static void slam_init( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->atck_no = pl0d_at_slam ;
	HIT_RESET( pl_w ) ;
	pl_w->char_no.b.grp = 21 ;
	pl_w->char_no.b.no = pl_w->atck_level + 22 ;
	pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
	slam_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void slam_move( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type & 2 ){
		pl_w->cg_type = 0 ;
		shl0d_01_set( pl_w, 0, 0 ) ;
	}
}

/*==============================================================*/
static void sp_slam( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	static void ( *slam_jmp[])( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw ) = {
		slam_init, slam_move,
	} ;

	slam_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/* Gamma Tornado: the spin that catches */
static void tornado_init( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw );
static void tornado_move( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw );

/*==============================================================*/
static void tornado_init( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->atck_no = pl0d_at_tornado ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 24 ) ;
	tornado_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void tornado_move( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 26 ) ;
		return ;
	}
	if( pl_sp_catch_ck( pl_w ) ){
		pl_w->catch_no = 2 ;
		pl_w->catch_no |= CATCH_ESC_NG ;
		pl_w->catch_no |= CATCH_ESC2_NG ;
		pl_w->routine_2 = pl_w->routine_3 = 0 ;
		em = pl_w->em_w ;
		em->spd.x = em->spd.y = em->acc.x = em->acc.y = 0.0f ;
		pl_catch_set( pl_w, em ) ;
	}
}

/*--------------------------------------------------------------*/
static void tornado_end( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_tornado( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	static void ( *tornado_jmp[])( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw ) = {
		tornado_init, tornado_move, tornado_end,
	} ;

	tornado_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/* Gamma Crush: the hyper combo that leaps, picks a way with the stick and comes down hard */
static void crush_init( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw );
static void crush_start( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw );

/*==============================================================*/
static void crush_init( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	lw->crush_bound_flag = 0 ;
	lw->crush_land_flag = 0 ;
	lw->crush_time = 0 ;
	pl_w->atck_no = pl0d_at_crush ;
	HIT_RESET( pl_w ) ;
	pl_w->char_no.b.grp = 22 ;
	pl_w->char_no.b.no = 0 ;
	pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
	crush_start( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void crush_start( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		scr_excep_set( pl_w, 7, 7 ) ;
		ofs.x = 0.0f ;
		ofs.y = 180.0f ;
		ofs.z = 0.0f ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void crush_jump( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_AIR ;
		if( !pl_w->pl_dir ){
			pl_w->spd.x = -DOT_X( 4.0f ) ;
			pl_w->acc.x = 0.15625f ;
		} else {
			pl_w->spd.x = DOT_X( 4.0f ) ;
			pl_w->acc.x = -0.15625f ;
		}
		pl_w->spd.y = DOT_Y( 16.0f ) ;
		pl_w->acc.y = DOT_Y( 0.5f ) ;
		lw->crush_time = 70 ;
		pl_w->timer0 = 24 ;
		pl_w->timer1 = 3 ;
	}
}

/*--------------------------------------------------------------*/
static void crush_rise( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->hit_flag ) pl_w->timer1 = 0 ;
	if( pl_w->timer1 && !-- pl_w->timer1 ){
		pl_w->atck_no = pl0d_at_crush_rise ;
		HIT_RESET( pl_w ) ;
	}
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->timer0 = 10 ;
	}
}

/*==============================================================*/
/*			At the top: forward, back or straight down, by		*/
/*			the stick or where the opponent is					*/
/*==============================================================*/
static void crush_turn( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	PLAYER_WORK *em = pl_w->target_w ;
	void *ew ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		if( pl_w->pl_sw_new & KEY_FORWARD ) pl_w->type = 0 ;
		else if( pl_w->pl_sw_new & KEY_BACK ) pl_w->type = 1 ;
		else if( pl_w->pl_sw_new & KEY_DOWN ) pl_w->type = 2 ;
		else if( em->pos.x < pl_w->pos.x + DOT_X( 32.0f ) && em->pos.x > pl_w->pos.x - DOT_X( 32.0f ) ) pl_w->type = 2 ;
		else if( em->pos.x < pl_w->pos.x ){
			if( !pl_w->pl_dir ) pl_w->type = 0 ;
			else pl_w->type = 1 ;
		} else {
			if( !pl_w->pl_dir ) pl_w->type = 1 ;
			else pl_w->type = 0 ;
		}
		pl_w->atck_no = pl_w->type + 63 ;
		HIT_RESET( pl_w ) ;
		pl_w->char_no.b.grp = 22 ;
		pl_w->char_no.b.no = pl_w->type + 1 ;
		pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
		switch( pl_w->type ){
		case 0:
			pl_w->spd.x = -DOT_X( 4.0f ) ;
			break ;
		case 1:
			pl_w->spd.x = DOT_X( 4.0f ) ;
			break ;
		default:
			pl_w->spd.x = 0.0f ;
		}
		if( pl_w->pl_dir ) pl_w->spd.x *= -1.0f ;
		pl_w->spd.y = -DOT_Y( 8.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		ef_color_set( pl_w, 2, 1, 1 ) ;
		shl0d_00_set( pl_w, 0, 0 ) ;
		ew = shl0d_00_set( pl_w, 2, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void crush_fall( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	int i ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->push_ng_timer = 1 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y + DOT_Y( 64.0f ) ){
		pl_w->routine_2 ++ ;
		lw->crush_land_flag = 1 ;
		scr_work.quake_level = 3 ;
		scr_work.quake_flag = 1 ;
		pl_voice_req( pl_w, 31 ) ;
		for( i = 0 ; i < 20 ; i ++ ) shl0d_00_set( pl_w, 1, 0 ) ;
		pl_w->spd.x /= 4.0f ;
		pl_w->spd.y /= 8.0f ;
		pl_w->acc.y /= 8.0f ;
		pl_w->timer0 = 16 ;
	}
}

/*--------------------------------------------------------------*/
static void crush_bound( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		lw->crush_bound_flag = 1 ;
		SCR_EXCEP_SET ;
		pl_w->char_no.b.grp = 22 ;
		pl_w->char_no.b.no = 4 ;
		pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
		switch( pl_w->type ){
		case 0:
			pl_w->spd.x = DOT_X( 2.0f ) ;
			break ;
		case 1:
			pl_w->spd.x = -DOT_X( 2.0f ) ;
			break ;
		default:
			pl_w->spd.x = 0.0f ;
		}
		if( pl_w->pl_dir ) pl_w->spd.x *= -1.0f ;
		pl_w->spd.y = DOT_Y( 8.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.5f ) ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void crush_land( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->ground_y >= pl_w->pos.y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_ef_set( pl_w ) ;
		pl_w->char_no.b.grp = 22 ;
		pl_w->char_no.b.no = 5 ;
		pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
	}
}

/*--------------------------------------------------------------*/
static void crush_end( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			Keeps him inside the screen's edges					*/
/*==============================================================*/
static void crush_pos_ck( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	if( scr_work.scr_left + DOT_X( 32.0f ) > pl_w->pos.x ) pl_w->pos.x = scr_work.scr_left + DOT_X( 32.0f ) ;
	if( pl_w->pos.x > scr_work.scr_right + -DOT_X( 32.0f ) ) pl_w->pos.x = scr_work.scr_right + -DOT_X( 32.0f ) ;
}

/*==============================================================*/
static void sp_crush( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	static void ( *crush_jmp[])( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw ) = {
		crush_init, crush_start, crush_jump, crush_rise, crush_turn,
		crush_fall, crush_bound, crush_land, crush_end,
	} ;

	crush_jmp[pl_w->routine_2]( pl_w, lw ) ;
	crush_pos_ck( pl_w, lw ) ;
}

/* Gamma Wave: the hyper combo's wave along the ground */
static void wave_init( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw );
static void wave_start( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw );

/*==============================================================*/
static void wave_init( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->atck_no = pl0d_at_wave ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 10 ) ;
	wave_start( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void wave_start( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type & 1 ){
		pl_w->cg_type &= 0xfe ;
		pl_w->routine_2 ++ ;
		ofs.x = -DOT_X( 8.0f ) ;
		ofs.y = 300.0f ;
		ofs.z = 0.0f ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void wave_move( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type & 2 ){
		pl_w->cg_type &= 0xfd ;
		shl0d_01_set( pl_w, 2, 0 ) ;
		scr_work.quake_level = 3 ;
		scr_work.quake_flag = 1 ;
	}
}

/*==============================================================*/
static void sp_wave( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	static void ( *wave_jmp[])( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw ) = {
		wave_init, wave_start, wave_move,
	} ;

	wave_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/* Gamma Quake: the hyper combo that strikes the ground and brings rocks down */
static void quake_init( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw );
static void quake_start( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw );

/*==============================================================*/
static void quake_init( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->atck_no = pl0d_at_quake ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 13 ) ;
	quake_start( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void quake_start( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type & 1 ){
		pl_w->cg_type &= 0xfe ;
		pl_w->routine_2 ++ ;
		ofs.x = 95.0f ;
		ofs.y = DOT_Y( 122.0f ) ;
		ofs.z = 0.0f ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void quake_move( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type & 1 ){
		pl_w->cg_type &= 0xfe ;
		shl0d_02_set( pl_w, 0, 0 ) ;
		scr_work.quake_level = 3 ;
		scr_work.quake_flag = 1 ;
	}
}

/*==============================================================*/
static void sp_quake( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	static void ( *quake_jmp[])( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw ) = {
		quake_init, quake_start, quake_move,
	} ;

	quake_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/* the taunt */
static void chouhatsu_init( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw );
static void chouhatsu_move( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw );

/*==============================================================*/
static void chouhatsu_init( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_char_set( pl_w, Char_Extra, 2 ) ;
	chouhatsu_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void chouhatsu_move( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	static void ( *chouhatsu_jmp[])( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw ) = {
		chouhatsu_init, chouhatsu_move,
	} ;

	chouhatsu_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/* the snapback */
static void snapback_init( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw );
static void snapback_move( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw );

/*==============================================================*/
static void snapback_init( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
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
	pl_w->atck_no = pl0d_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 29 ) ;
	snapback_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void snapback_move( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		ofs.x = -DOT_X( 67.0f ) ;
		ofs.y = DOT_Y( 118.0f ) ;
		ofs.z = 0.0f ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
}

/*==============================================================*/
static void sp_snapback( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw )
{
	static void ( *snapback_jmp[])( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw ) = {
		snapback_init, snapback_move,
	} ;

	snapback_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/****************************************************************/
void pl0d_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *sp_atck_tbl[])( PLAYER_WORK *pl_w, PL0D_LOCAL_W *lw ) = {
		sp_charge, sp_charge, sp_slam, sp_crush, sp_tornado, sp_ucharge, sp_wave, sp_charge, sp_charge, sp_chouhatsu, sp_quake, sp_snapback,
	} ;
	PL0D_LOCAL_W *lw = ( PL0D_LOCAL_W * )pl_w->tlocal ;

	sp_atck_tbl[pl_w->pl_sp_type]( pl_w, lw ) ;
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
	if( pl_w->atck_pk == 0 && pl_w->atck_level == 1 ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 1 ;
			return em ;
		}
		return 0 ;
	}
	return 0 ;
}

/****************************************************************/
/*			Can a throw start?									*/
/****************************************************************/
PLAYER_WORK *pl0d_catch_ck( PLAYER_WORK *pl_w )
{
	static PLAYER_WORK *( *catch_ck_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_catch_ck, crouch_catch_ck, jump_catch_ck, nml_catch_ck,
	} ;

	return catch_ck_jmp[pl_w->pl_stance]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_w->dir & 2 ){
		pl_w->pl_dir ^= 1 ;
		pl_w->disp_dir = pl_w->pl_dir ;
	}
	pl_w->hit_stop = 10 ;
	pl_char_set( pl_w, Char_Catch, 0 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	ofs.x = -DOT_X( 143.0f ) ;
	ofs.y = DOT_Y( 79.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	catch_gauge_add( pl_w ) ;
}

/*==============================================================*/
static void jump_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_char_set( pl_w, Char_Catch, 1 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	ofs.x = -DOT_X( 89.0f ) ;
	ofs.y = DOT_Y( 125.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	catch_gauge_add( pl_w ) ;
}

/*==============================================================*/
/*			The throw Gamma Tornado catches with				*/
/*==============================================================*/
static void tornado_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	scr_excep_set( pl_w, 5, 5 ) ;
	pl_char_set( pl_w, Char_Catch, pl_w->atck_level + 2 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	ofs.x = -DOT_X( 143.0f ) ;
	ofs.y = DOT_Y( 79.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	catch_gauge_add( pl_w ) ;
}

/****************************************************************/
/*			Start the throw that pl0d_catch_ck or Gamma			*/
/*			Tornado chose										*/
/****************************************************************/
void pl0d_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_set, jump_p_set, tornado_set,
	} ;

	catch_set_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->disp_dir ^= 1 ;
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->catch_type = 11 ;
		em->pl_stance = STANCE_AIR ;
		em->atck_no = pl0d_at_throw_p ;
		pl_w->atck_no = pl0d_at_throw_p ;
		em->pl_dir = pl_w->pl_dir ;
		pl_w->pl_dir ^= 1 ;
		catch_se_req( em ) ;
	}
}

/*==============================================================*/
static void jump_p_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_jump_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type & 1 ){
		pl_w->cg_type &= 0xfe ;
		pls0d_00_set( pl_w, 4, 0 ) ;
	}
	if( pl_w->cg_type & 2 ){
		pl_w->cg_type &= 0xfd ;
		em = pl_w->catch_w ;
		em->catch_type = 1 ;
		em->pl_stance = STANCE_AIR ;
		em->atck_no = pl0d_at_air_throw ;
		pl_w->atck_no = pl0d_at_air_throw ;
		em->pl_dir = pl_w->pl_dir ;
	}
}

/*==============================================================*/
static void tornado_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->disp_dir ^= 1 ;
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( !( pl_w->cg_type &= 1 ) ) return ;
	pl_w->cg_type = 0 ;
	em = pl_w->catch_w ;
	em->catch_type = 1 ;
	em->pl_stance = STANCE_AIR ;
	em->atck_no = pl_w->atck_level + 34 ;
	pl_w->atck_no = pl_w->atck_level + 34 ;
	em->pl_dir = pl_w->pl_dir ;
	pl_w->pl_dir ^= 1 ;
}

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void pl0d_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_catch, jump_p_catch, tornado_catch,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void hold( PLAYER_WORK *pl_w )
{
	catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/****************************************************************/
/*			Holding the opponent								*/
/****************************************************************/
void pl0d_hold( PLAYER_WORK *pl_w )
{
	static void ( *hold_jmp[])( PLAYER_WORK *pl_w ) = {
		hold, hold, hold,
	} ;

	hold_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void pl0d_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl0d_sp_wave ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl0d_sp_crush ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl0d_sp_quake ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl0d_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl0d_sp_wave ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl0d_sp_crush ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl0d_sp_quake ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl0d_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl0d_sp_slam ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl0d_sp_charge ;
		pl_w->routine_2 = 0 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl0d_sp_charge ;
		pl_w->type = 1 ;
		pl_w->sub_type = 2 ;
		pl_w->routine_2 = 1 ;
		break ;
	}
	pl_w->atck_level = 0 ;
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void pl0d_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl0d_sp_slam ;
		pl_w->atck_level = 0 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl0d_sp_charge ;
		pl_w->atck_level = 0 ;
		pl_w->routine_2 = 0 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl0d_sp_ucharge ;
		pl_w->atck_level = 0 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl0d.c" End of File	*/

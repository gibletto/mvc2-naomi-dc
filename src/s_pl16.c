/* Marrow (character 16): command checks, normal attacks, appearance, win and lose poses, special
   moves and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl16_def.h"

void pl16_main( PLAYER_WORK *pl_w );
void pl16_com_ck( PLAYER_WORK *pl_w );
void pl16_atck_set( PLAYER_WORK *pl_w );
void pl16_atck( PLAYER_WORK *pl_w );
void pl16_dash( PLAYER_WORK *pl_w );
void pl16_noki( PLAYER_WORK *pl_w );
void pl16_appear( PLAYER_WORK *pl_w );
void pl16_win( PLAYER_WORK *pl_w );
void pl16_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl16_catch_ck( PLAYER_WORK *pl_w );
void pl16_catch_set( PLAYER_WORK *pl_w );
void pl16_catch( PLAYER_WORK *pl_w );
void pl16_hold( PLAYER_WORK *pl_w );
int pl16_delay_ck( PLAYER_WORK *pl_w );
void pl16_t_kick( PLAYER_WORK *pl_w );
void pl16_upper( PLAYER_WORK *pl_w );
void pl16_sp_unison( PLAYER_WORK *pl_w );
void pl16_sp_gattai( PLAYER_WORK *pl_w );
void pl16_sp_assist( PLAYER_WORK *pl_w );
void pl16_sp_zero( PLAYER_WORK *pl_w );

/* each attack's table: standing and crouching, jumping, super jumping */
static Sint8 lp_tbl[4] = {  1,  2,  0,  2 } ;
static Sint8 mp_tbl[4] = { -1,  2,  1,  2 } ;
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
static Uint16 com_bone[] = { 3, 0, pl16_sp_bone, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Bone-merang */
static Uint16 com_spine[] = { 3, COM_GROUND, pl16_sp_spine, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_PUNCH } ;	/* Towering Spine */
static Uint16 com_ricochet[] = { 3, COM_GROUND, pl16_sp_ricochet, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Ricochet Slash */
static Uint16 com_ride[] = { 5, 0, pl16_sp_ride, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Ride and Slash */
static Uint16 com_stinger[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl16_sp_stinger, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Stinger Bones (hyper) */
static Uint16 com_burst[] = { 3, COM_HYPER | COM_ALL_SW, pl16_sp_burst, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Bone Burst (hyper) */

static void ( *pl16_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl16_main,
	pl16_com_ck,
	pl16_atck_set,
	pl16_atck,
	pl16_dash,
	pl16_noki,
	pl16_appear,
	pl16_win,
	pl16_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl16_catch_ck,	/* returns a value */
	pl16_catch_set,
	pl16_catch,
	pl16_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl16_delay_ck,
	pl16_t_kick,
	pl16_upper,
	pl16_sp_unison,
	pl16_sp_gattai,
	pl16_sp_assist,
	pl16_sp_zero,
} ;

static Uint8 bone_ck( PLAYER_WORK *pl_w );
static Uint8 spine_ck( PLAYER_WORK *pl_w );
static Uint8 ricochet_ck( PLAYER_WORK *pl_w );
static Uint8 ride_ck( PLAYER_WORK *pl_w );
static Uint8 stinger_ck( PLAYER_WORK *pl_w );
static Uint8 burst_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static int chouhatsu_ck( PLAYER_WORK *pl_w );
static int burst_delay_ck( PLAYER_WORK *pl_w );
static int stinger_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl16_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl16_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl16_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( stinger_ck( pl_w ) ) return ;
	if( burst_ck( pl_w ) ) return ;
	if( ride_ck( pl_w ) ) return ;
	if( ricochet_ck( pl_w ) ) return ;
	if( spine_ck( pl_w ) ) return ;
	if( bone_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
/*			Also in the air, once a jump unless super			*/
/*			jumping, and not while one is out					*/
/*==============================================================*/
static Uint8 bone_ck( PLAYER_WORK *pl_w )
{
	PL16_LOCAL_W *lw = ( PL16_LOCAL_W * )&pl_w->tlocal[0] ;

	if( !com_nml_ck( pl_w, com_bone, &pl_w->com_w0 ) ) return 0 ;
	if( lw->shot_flag ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR ){
		if( pl_w->jump_com_cnt && !pl_w->sjump_flag ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl16_sp_bone ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 spine_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_spine, &pl_w->com_w1 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl16_sp_spine ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 ricochet_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_ricochet, &pl_w->com_w2 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl16_sp_ricochet ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			In the air it starts from its second step			*/
/*==============================================================*/
static Uint8 ride_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_ride, &pl_w->com_w3 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR ){
		if( pl_w->jump_com_cnt && !pl_w->sjump_flag ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w3 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl16_sp_ride ;
	act_st_set( pl_w, N_SPECIAL ) ;
	if( pl_w->pl_stance == STANCE_AIR ) pl_w->routine_2 = 1 ;
	else pl_w->routine_2 = 0 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 stinger_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_stinger, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w4 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl16_sp_stinger ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 burst_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_burst, &pl_w->com_w5 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w5 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl16_sp_burst ;
	act_st_set( pl_w, N_SUPER ) ;
	if( pl_w->pl_stance == STANCE_AIR ) pl_w->routine_2 = 1 ;
	else pl_w->routine_2 = 0 ;
	return 1 ;
}

/*==============================================================*/
static int snapback_ck( PLAYER_WORK *pl_w )
{
	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl16_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static int chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 7 ) ) return 0 ;
	pl_w->pl_sp_type = pl16_sp_chouhatsu ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl16_delay_ck( PLAYER_WORK *pl_w )
{
	if( burst_delay_ck( pl_w ) || stinger_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static int burst_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_burst, &pl_w->com_w5 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR ) return 0 ;
	pl_w->delay_sp_type = pl16_sp_burst ;
	return 1 ;
}

/*==============================================================*/
static int stinger_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_stinger, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl16_sp_stinger ;
	return 1 ;
}

/****************************************************************/
void pl16_main( PLAYER_WORK *pl_w )
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
void pl16_atck_set( PLAYER_WORK *pl_w )
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
		pl_voice_req( pl_w, 31 ) ;
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		pl_w->unk_2a2 = 1 ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_STAND_MP ;
		pl_voice_req( pl_w, 32 ) ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_STAND_HP ;
		pls16_01_set( pl_w, 2 ) ;
		pl_voice_req( pl_w, 33 ) ;
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
		pl_voice_req( pl_w, 31 ) ;
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_CROUCH_MP ;
		pl_voice_req( pl_w, 32 ) ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_CROUCH_HP ;
		pl_voice_req( pl_w, 33 ) ;
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
		pl_voice_req( pl_w, 31 ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_lp_tbl ;
		else pl_w->chain_tbl = sj_lp_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_JUMP_MP ;
		pl_voice_req( pl_w, 32 ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mp_tbl ;
		else pl_w->chain_tbl = sj_mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_JUMP_HP ;
		pl_voice_req( pl_w, 33 ) ;
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
static void stand_hp_move( PLAYER_WORK *pl_w );
static void stand_hp_start( PLAYER_WORK *pl_w );
static void stand_hp_end( PLAYER_WORK *pl_w );
static void crouch_punch_move( PLAYER_WORK *pl_w );
static void stand_kick_move( PLAYER_WORK *pl_w );
static void crouch_kick_move( PLAYER_WORK *pl_w );
static void sjump_move( PLAYER_WORK *pl_w );
static void jump_move( PLAYER_WORK *pl_w );
static void jump_punch_move( PLAYER_WORK *pl_w );
static void jump_kick_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl16_atck( PLAYER_WORK *pl_w )
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
	if( pl_w->atck_pk == 1 ){
		if( pl_w->pl_stance == STANCE_CROUCH ) crouch_kick_move( pl_w ) ;
		else stand_kick_move( pl_w ) ;
	} else {
		if( pl_w->pl_stance == STANCE_CROUCH ) crouch_punch_move( pl_w ) ;
		else stand_punch_move( pl_w ) ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_brake_stop( pl_w ) ;
}

/*==============================================================*/
static void stand_punch_move( PLAYER_WORK *pl_w )
{
	if( pl_w->nml_no == 2 ) stand_hp_move( pl_w ) ;
	else if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			The heavy punch goes on into a second animation		*/
/*==============================================================*/
static void stand_hp_move( PLAYER_WORK *pl_w )
{
	static void ( *stand_hp_jmp[])( PLAYER_WORK *pl_w ) = {
		stand_hp_start, stand_hp_end,
	} ;

	stand_hp_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void stand_hp_start( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Stand_P, 4 ) ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pls16_01_set( pl_w, 0 ) ;
		if( pl_w->hit_flag ) pls16_01_set( pl_w, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void stand_hp_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
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
void pl16_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_gauge_add( pl_w, 5 ) ;
		pl_w->atck_no = pl16_at_upper ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_char_set( pl_w, Char_Extra, 2 ) ;
		HIT_RESET( pl_w ) ;
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
void pl16_dash( PLAYER_WORK *pl_w )
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
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 10.0f ) : -DOT_X( 10.0f ) ;
		pl_w->acc.x = 0.0f ;
		pl_w->timer1 = 20 ;
	}
}

/*==============================================================*/
/*			A run for a set time, then slowing to a stop		*/
/*==============================================================*/
static void dash_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !pl_w->timer1-- ){
		pl_w->routine_2 ++ ;
		pl_w->acc.x = pl_w->pl_dir ? -1.6666666f : 1.6666666f ;
		pl_char_set( pl_w, Char_Dash, 2 ) ;
	}
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
static void noki_jump( PLAYER_WORK *pl_w );
static void noki_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash, a hop								*/
/****************************************************************/
void pl16_noki( PLAYER_WORK *pl_w )
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
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 20.0f ;
		pl_w->acc.x = -DOT_X( 0.375f ) ;
		pl_w->spd.y = DOT_Y( 3.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.25f ) ;
		if( pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		noki_jump( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void noki_jump( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->spd.x * pl_w->acc.x >= 0.0f ) RATE_CLR_X( pl_w ) ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Dash, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void noki_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void appear_init( PLAYER_WORK *pl_w );
static void appear_move( PLAYER_WORK *pl_w );
static void appear_jump( PLAYER_WORK *pl_w );
static void win_time_ko( PLAYER_WORK *pl_w );
static void win_ko_init( PLAYER_WORK *pl_w );
static void win_ko_move( PLAYER_WORK *pl_w );
static void win_ko_move_e( PLAYER_WORK *pl_w );
static void lose_time_ko( PLAYER_WORK *pl_w );
static void draw_time( PLAYER_WORK *pl_w );

/****************************************************************/
void pl16_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_init,
		appear_move,
		appear_jump,
	} ;

	appear_jmp[ pl_w->routine_2 ]( pl_w ) ;
}

/*==============================================================*/
static void appear_init( PLAYER_WORK *pl_w )
{
	pl_w -> routine_2 ++ ;
	pl_w -> disp_flag = 1 ;
	pl_char_set( pl_w, Char_Appear, 0 ) ;
	pls16_00_set( pl_w, 0 ) ;
}

/*==============================================================*/
/*			Then a leap up										*/
/*==============================================================*/
static void appear_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w -> routine_2 ++ ;
		pl_char_set( pl_w, Char_Appear, 1 ) ;
		pl_w -> spd.y = DOT_Y( 8.0f ) ;
		pl_w -> acc.y = -DOT_Y( 0.375f ) ;
		pls16_00_set( pl_w, 1 ) ;
		pl_se_req( pl_w, 50 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_jump( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->routine_1 ++ ;
		pl_voice_req( pl_w, 10 ) ;
	}
}

/****************************************************************/
void pl16_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_time_ko,
		lose_time_ko,
		win_time_ko,
		lose_time_ko,
		draw_time,
	} ;

	pl_w -> pos.y = pl_w -> ground_y ;
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
	} ;

	win_ko_jmp[ pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void win_ko_init( PLAYER_WORK *pl_w )
{
	pl_w -> routine_2 ++ ;

	/* won on time */
	if( pl_w->type == 2 ){
		pl_w -> routine_2 = 2 ;
		pl_char_set( pl_w, Char_Win, CH16_WIN_KO1 ) ;
		return ;
	}

	/* won by KO: one of two poses */
	if( vs_sys_work.vs_end_flag ) pl_w -> timer0 = Rnd() & 1 ;
	else pl_w -> timer0 = 1 ;
	if( pl_be_cnt( pl_w ) >= 2 ) pl_w -> timer0 = 1 ;

	switch( pl_w -> timer0 ){
	case 0 :
		pl_w -> routine_2 = 1 ;
		pl_char_set( pl_w, Char_Win, CH16_WIN_KO0 ) ;
		break ;
	case 1 :
		pl_w -> routine_2 = 2 ;
		pl_char_set( pl_w, Char_Win, CH16_WIN_KO1 ) ;
		break ;
	case 2 :
		break ;
	}
}

/*--------------------------------------------------------------*/
static void win_ko_move( PLAYER_WORK *pl_w )
{
	pl_w->win_wait = 0xff ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pls16_08_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void win_ko_move_e( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void lose_time_ko( PLAYER_WORK *pl_w )
{
	if( !pl_w -> routine_2 ){
		pl_w -> routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, CH16_LOSE ) ;
	}else{
		pl_char_move( pl_w ) ;
	}
}

/*==============================================================*/
static void draw_time( PLAYER_WORK *pl_w )
{
	if( !pl_w -> routine_2 ){
		pl_w -> routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, CH16_TM_DRAW ) ;
	}else{
		pl_char_move( pl_w ) ;
	}
}

static void sp_bone( PLAYER_WORK *pl_w );
static void sp_spine( PLAYER_WORK *pl_w );
static void sp_ricochet( PLAYER_WORK *pl_w );
static void sp_ride( PLAYER_WORK *pl_w );
static void sp_stinger( PLAYER_WORK *pl_w );
static void sp_burst( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );

/****************************************************************/
void	pl16_sp_atck( PLAYER_WORK *pl_w )
{
	static void (*pl_sp_tbl[])( PLAYER_WORK *pl_w ) ={
		sp_bone,					/* 00 Bone-merang */
		sp_spine,					/* 01 Towering Spine */
		sp_ricochet,					/* 02 Ricochet Slash */
		sp_ride,					/* 03 Ride and Slash */
		sp_stinger,					/* 04 Stinger Bones (hyper) */
		sp_burst,					/* 05 Bone Burst (hyper) */
		sp_snapback,					/* 06 the snapback */
		sp_chouhatsu,					/* 07 the taunt */
	} ;

	pl_sp_tbl[ pl_w->pl_sp_type ]( pl_w ) ;
}

/*==============================================================*/
/*			A special move starts standing on the ground		*/
/*==============================================================*/
static void sp_stand_set( PLAYER_WORK *pl_w )
{
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
}

/*==============================================================*/
/*			Move, but not below the ground						*/
/*==============================================================*/
static void sp_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ) pl_w->pos.y = pl_w->ground_y ;
}

static void bone_init( PLAYER_WORK *pl_w );
static void bone_move( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Bone-merang, also in the air						*/
/*==============================================================*/
static void sp_bone( PLAYER_WORK *pl_w )
{
	static void ( *bone_jmp[])( PLAYER_WORK *pl_w ) = {
		bone_init, bone_move,
	} ;

	bone_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void bone_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl16_at_bone ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->spd.x /= 16.0f ;
	pl_w->spd.y /= 8.0f ;
	pl_w->acc.y /= 64.0f ;
	pl_w->acc.x = 0.0f ;
	if( pl_w->pl_stance == STANCE_AIR ){
		if( !pl_w->atck_level ) pl_char_set( pl_w, Char_Special, 2 ) ;
		else pl_char_set( pl_w, Char_Special, 3 ) ;
	} else {
		sp_stand_set( pl_w ) ;
		if( !pl_w->atck_level ) pl_char_set( pl_w, Char_Special, 0 ) ;
		else pl_char_set( pl_w, Char_Special, 1 ) ;
	}
	bone_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void bone_move( PLAYER_WORK *pl_w )
{
	sp_move( pl_w ) ;
	if( pl_w->cg_type ){
		shl16_00_set( pl_w ) ;
		pl_w->cg_type = 0 ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		if( pl_w->pl_stance == STANCE_AIR ){
			pl_w->spd.y = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_jump_ret( pl_w ) ;
		} else {
			pl_stand_ret( pl_w ) ;
		}
	}
}

static void spine_init( PLAYER_WORK *pl_w );
static void spine_move( PLAYER_WORK *pl_w );
static void spine_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Towering Spine										*/
/*==============================================================*/
static void sp_spine( PLAYER_WORK *pl_w )
{
	static void ( *spine_jmp[])( PLAYER_WORK *pl_w ) = {
		spine_init, spine_move, spine_end,
	} ;

	spine_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void spine_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	sp_stand_set( pl_w ) ;
	pl_w->atck_no = pl16_at_spine ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_char_set( pl_w, Char_Special, 4 ) ;
}

/*--------------------------------------------------------------*/
static void spine_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		shl16_02_set( pl_w, 0 ) ;
		shl16_02_set( pl_w, 1 ) ;
		shl16_02_set( pl_w, 2 ) ;
		shl16_02_set( pl_w, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void spine_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->atck_no = pl_w->cg_type + 0x30 ;
		HIT_RESET( pl_w ) ;
		pl_w->cg_type = 0 ;
	}
}

static void ricochet_init( PLAYER_WORK *pl_w );
static void ricochet_start( PLAYER_WORK *pl_w );
static void ricochet_rise( PLAYER_WORK *pl_w );
static void ricochet_turn( PLAYER_WORK *pl_w );
static void ricochet_atck( PLAYER_WORK *pl_w );
static void ricochet_fall( PLAYER_WORK *pl_w );
static void ricochet_end( PLAYER_WORK *pl_w );
static void ricochet_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Ricochet Slash: a leap to the far side of the		*/
/*			screen												*/
/*==============================================================*/
static void sp_ricochet( PLAYER_WORK *pl_w )
{
	static void ( *ricochet_jmp[])( PLAYER_WORK *pl_w ) = {
		ricochet_init, ricochet_start, ricochet_rise, ricochet_turn, ricochet_atck, ricochet_fall, ricochet_end,
	} ;

	ricochet_jmp[pl_w->routine_2]( pl_w ) ;
}

/* the dive's speed, and the jump off the opponent, for each strength */
static Float ricochet_dive_spd[2][2] = {
	{ 20.0f, -DOT_Y( 5.5f ) },
	{ 30.0f, -DOT_Y( 5.0f ) },
} ;
static Float ricochet_jump_spd[2][3] = {
	{ -DOT_X( 2.0f ), DOT_Y( 4.0f ), -DOT_Y( 0.375f ) },
	{ -DOT_X( 4.0f ), DOT_Y( 6.0f ), -DOT_Y( 0.375f ) },
} ;

/*==============================================================*/
static void ricochet_init( PLAYER_WORK *pl_w )
{
	Float spd[2][2] = {
		{ DOT_Y( 9.0f ), -DOT_Y( 0.375f ) },
		{ DOT_Y( 9.0f ), -DOT_Y( 0.375f ) },
	} ;
	Float x ;

	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl_w->atck_level + 0x36 ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	sp_stand_set( pl_w ) ;
	/* land at the far edge of the screen */
	x = pl_w->pl_dir ? scr_work.eye[0].x + -320.0f : scr_work.eye[0].x + 320.0f ;
	pl_w->spd.x = ( x - pl_w->pos.x ) / 32.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = spd[pl_w->atck_level][0] ;
	pl_w->acc.y = spd[pl_w->atck_level][1] ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 12 ) ;
}

/*--------------------------------------------------------------*/
static void ricochet_start( PLAYER_WORK *pl_w )
{
	PL16_LOCAL_W *lw = ( PL16_LOCAL_W * )&pl_w->tlocal[0] ;

	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_air_set( pl_w ) ;
		pl_w->pl_dir = pl_w->disp_dir = pl_w->pl_dir ^ 1 ;
		lw->scr_x = scr_work.eye[0].x ;
	}
}

/*==============================================================*/
/*			Rise with the screen, and turn at the top			*/
/*==============================================================*/
static void ricochet_rise( PLAYER_WORK *pl_w )
{
	PL16_LOCAL_W *lw = ( PL16_LOCAL_W * )&pl_w->tlocal[0] ;

	pl_w->pos.x += scr_work.eye[0].x - lw->scr_x ;
	lw->scr_x = scr_work.eye[0].x ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		ricochet_land( pl_w ) ;
		return ;
	}
	if( pl_w->pos.y >= pl_w->ground_y + DOT_Y( 104.0f ) && pl_w->wall_flag ){
		pl_w->routine_2 ++ ;
		pl_w->pl_dir = pl_w->disp_dir = pl_w->pl_dir ^ 1 ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 14 ) ;
	}
}

/*==============================================================*/
/*			At the far side, dive								*/
/*==============================================================*/
static void ricochet_turn( PLAYER_WORK *pl_w )
{
	pl_w->pos.x = pl_w->pl_dir ? scr_work.eye[0].x + -320.0f : scr_work.eye[0].x + 320.0f ;
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		/* three hits at either strength */
		pl_w->timer0 = pl_w->atck_level ? 3 : 3 ;
		pl_w->timer1 = 0 ;
		pl_w->dir = 0 ;
		pl_w->atck_no = pl_w->atck_level ? 0x39 : 0x36 ;
		HIT_RESET( pl_w ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? ricochet_dive_spd[pl_w->atck_level][0] : -ricochet_dive_spd[pl_w->atck_level][0] ;
		pl_w->spd.y = ricochet_dive_spd[pl_w->atck_level][1] ;
		if( pl_w->atck_level ) pl_w->rot[2] = 0xf200 ;
		pls16_02_set( pl_w, 0 ) ;
	}
}

/*==============================================================*/
/*			Hit up to three times, then jump off				*/
/*==============================================================*/
static void ricochet_atck( PLAYER_WORK *pl_w )
{
	if( pl_w->hit_flag && !pl_w->timer1 ){
		pl_w->timer1 = 1 ;
		pl_w->dir ++ ;
		if( !--pl_w->timer0 ){
			pl_w->routine_2 ++ ;
			pl_w->spd.x = pl_w->pl_dir ? ricochet_jump_spd[pl_w->atck_level][0] : -ricochet_jump_spd[pl_w->atck_level][0] ;
			pl_w->acc.x = 0.0f ;
			pl_w->spd.y = ricochet_jump_spd[pl_w->atck_level][1] ;
			pl_w->acc.y = ricochet_jump_spd[pl_w->atck_level][2] ;
			pl_w->rot[2] = 0 ;
			pl_char_set( pl_w, Char_Special, pl_w->atck_level + 16 ) ;
			pls16_02_set( pl_w, 1 ) ;
			return ;
		}
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->rot[2] = 0 ;
		ricochet_land( pl_w ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->atck_no = ( pl_w->atck_level ? 0x39 : 0x36 ) + pl_w->dir ;
		HIT_RESET( pl_w ) ;
		pl_w->cg_type = 0 ;
		pl_w->timer1 = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void ricochet_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ) ricochet_land( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void ricochet_end( PLAYER_WORK *pl_w )
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
static void ricochet_land( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = 6 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_land_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 18 ) ;
}

static void ride_nml( PLAYER_WORK *pl_w );
static void ride_jump( PLAYER_WORK *pl_w );
static void ride_init( PLAYER_WORK *pl_w );
static void ride_start( PLAYER_WORK *pl_w );
static void ride_rise( PLAYER_WORK *pl_w );
static void ride_fly( PLAYER_WORK *pl_w );
static void ride_bound( PLAYER_WORK *pl_w );
static void ride_end( PLAYER_WORK *pl_w );
static void ride_j_init( PLAYER_WORK *pl_w );
static void ride_j_start( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Ride and Slash: a leap that throws the opponent		*/
/*			if it catches them, from the ground or the air		*/
/*==============================================================*/
static void sp_ride( PLAYER_WORK *pl_w )
{
	static void ( *ride_jmp[])( PLAYER_WORK *pl_w ) = {
		ride_nml, ride_jump,
	} ;

	ride_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void ride_nml( PLAYER_WORK *pl_w )
{
	static void ( *ride_nml_jmp[])( PLAYER_WORK *pl_w ) = {
		ride_init, ride_start, ride_rise, ride_fly, ride_bound, ride_end,
	} ;

	ride_nml_jmp[pl_w->routine_3]( pl_w ) ;
}

/* the leap's speeds for each strength */
static Float ride_spd[2][4] = {
	{ DOT_X( 4.0f ), 0.0f, DOT_Y( 9.0f ), -1.2723213f },
	{ 5.0f, DOT_X( 0.25f ), DOT_Y( 9.0f ), -1.2723213f },
} ;

/*==============================================================*/
static void ride_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_w->atck_no = pl16_at_ride ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	sp_stand_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 20 ) ;
}

/*--------------------------------------------------------------*/
static void ride_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		pl_air_set( pl_w ) ;
		pl_w->spd.x = pl_w->pl_dir ? ride_spd[pl_w->atck_level][0] : -ride_spd[pl_w->atck_level][0] ;
		pl_w->acc.x = pl_w->pl_dir ? ride_spd[pl_w->atck_level][1] : -ride_spd[pl_w->atck_level][1] ;
		pl_w->spd.y = ride_spd[pl_w->atck_level][2] ;
		pl_w->acc.y = ride_spd[pl_w->atck_level][3] ;
	}
}

/*--------------------------------------------------------------*/
static void ride_rise( PLAYER_WORK *pl_w )
{
	if( !pl_w->cg_type ) pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->spd.y < 0.0f ) pl_w->routine_3 ++ ;
}

/*==============================================================*/
/*			On a hit, throw them if they can be caught, or		*/
/*			bounce off											*/
/*==============================================================*/
static void ride_fly( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->hit_flag ){
		if( pl_sp_catch_ck( pl_w ) ){
			pl_w->routine_3 ++ ;
			pl_w->catch_no = 0xc2 ;
			pl_catch_set( pl_w, pl_w->em_w ) ;
			pls16_05_set( pl_w ) ;
			return ;
		}
		pl_w->routine_3 ++ ;
		pl_w->spd.x = -( pl_w->spd.x / 8.0f ) ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 4.0f ) ;
		pl_w->acc.y = -0.6696428f ;
		pl_char_set( pl_w, Char_Special, 22 ) ;
		return ;
	}
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_3 += 2 ;
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
static void ride_bound( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_3 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Jump, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void ride_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void ride_jump( PLAYER_WORK *pl_w )
{
	static void ( *ride_jump_jmp[])( PLAYER_WORK *pl_w ) = {
		ride_j_init, ride_j_start, ride_rise, ride_fly, ride_bound, ride_end,
	} ;

	ride_jump_jmp[pl_w->routine_3]( pl_w ) ;
}

static Float ride_j_spd[2][4] = {
	{ DOT_X( 4.0f ), 0.0f, DOT_Y( 9.0f ), -1.2723213f },
	{ 10.0f, 0.0f, DOT_Y( 9.0f ), -1.2723213f },
} ;

/*==============================================================*/
static void ride_j_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_w->atck_no = pl16_at_ride ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 21 ) ;
}

/*--------------------------------------------------------------*/
static void ride_j_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = pl_w->pl_dir ? ride_j_spd[pl_w->atck_level][0] : -ride_j_spd[pl_w->atck_level][0] ;
		pl_w->acc.x = pl_w->pl_dir ? ride_j_spd[pl_w->atck_level][1] : -ride_j_spd[pl_w->atck_level][1] ;
		pl_w->spd.y = ride_j_spd[pl_w->atck_level][2] ;
		pl_w->acc.y = ride_j_spd[pl_w->atck_level][3] ;
	}
}

static void stinger_init( PLAYER_WORK *pl_w );
static void stinger_start( PLAYER_WORK *pl_w );
static void stinger_move( PLAYER_WORK *pl_w );
static void stinger_keep( PLAYER_WORK *pl_w );
static void stinger_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Stinger Bones, a hyper combo						*/
/*==============================================================*/
static void sp_stinger( PLAYER_WORK *pl_w )
{
	static void ( *stinger_jmp[])( PLAYER_WORK *pl_w ) = {
		stinger_init, stinger_start, stinger_move, stinger_keep, stinger_end,
	} ;

	stinger_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void stinger_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl16_at_stinger ;
	HIT_RESET( pl_w ) ;
	sp_stand_set( pl_w ) ;
	pls16_04_set( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 2 ) ;
}

/*--------------------------------------------------------------*/
static void stinger_start( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = DOT_X( 10.0f ) ;
		ofs.y = 195.0f ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void stinger_move( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->timer0 = 0 ;
		shl16_03_set( pl_w ) ;
		pls16_06_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void stinger_keep( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void stinger_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void burst_nml( PLAYER_WORK *pl_w );
static void burst_jump( PLAYER_WORK *pl_w );
static void burst_init( PLAYER_WORK *pl_w );
static void burst_rise( PLAYER_WORK *pl_w );
static void burst_wait( PLAYER_WORK *pl_w );
static void burst_fall( PLAYER_WORK *pl_w );
static void burst_end( PLAYER_WORK *pl_w );
static void burst_j_init( PLAYER_WORK *pl_w );
static void burst_j_rise( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Bone Burst, a hyper combo, from the ground or		*/
/*			the air												*/
/*==============================================================*/
static void sp_burst( PLAYER_WORK *pl_w )
{
	static void ( *burst_jmp[])( PLAYER_WORK *pl_w ) = {
		burst_nml, burst_jump,
	} ;

	burst_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void burst_nml( PLAYER_WORK *pl_w )
{
	static void ( *burst_nml_jmp[])( PLAYER_WORK *pl_w ) = {
		burst_init, burst_rise, burst_wait, burst_fall, burst_end,
	} ;

	burst_nml_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void burst_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_3 ++ ;
	pl_w->atck_no = pl16_at_burst ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	if( pl_w->pl_no ) scr_excep_set( pl_w, 1, 4 ) ;
	else scr_excep_set( pl_w, 1, 3 ) ;
	RATE_CLR_X( pl_w ) ;
	pl_w->spd.y = DOT_Y( 2.0f ) ;
	pl_w->acc.y = 0.0f ;
	pl_w->timer0 = 16 ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_char_set( pl_w, Char_Super, 0 ) ;
}

/*--------------------------------------------------------------*/
static void burst_rise( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !--pl_w->timer0 ){
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Super, 1 ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		ofs = pl_w->pos ;
		ofs.x = -DOT_X( 32.0f ) ;
		ofs.y = DOT_Y( 96.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void burst_wait( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_w->cg_type ){
		shl16_01_set( pl_w ) ;
		pl_w->cg_type = 0 ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_3 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_char_set( pl_w, Char_Jump, 9 ) ;
	}
}

/*--------------------------------------------------------------*/
static void burst_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_3 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_char_set( pl_w, Char_Jump, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void burst_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void burst_jump( PLAYER_WORK *pl_w )
{
	static void ( *burst_jump_jmp[])( PLAYER_WORK *pl_w ) = {
		burst_j_init, burst_j_rise, burst_wait, burst_fall, burst_end,
	} ;

	burst_jump_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void burst_j_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_3 ++ ;
	pl_w->atck_no = pl16_at_burst ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	if( pl_w->pl_no ) scr_excep_set( pl_w, 1, 4 ) ;
	else scr_excep_set( pl_w, 1, 3 ) ;
	pl_w->timer0 = 16 ;
	pl_char_set( pl_w, Char_Super, 0 ) ;
}

/*--------------------------------------------------------------*/
static void burst_j_rise( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( !--pl_w->timer0 ){
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Super, 1 ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		ofs = pl_w->pos ;
		ofs.x = -DOT_X( 32.0f ) ;
		ofs.y = DOT_Y( 96.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

static void snapback_init( PLAYER_WORK *pl_w );
static void snapback_move( PLAYER_WORK *pl_w );
static void snapback_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The snapback										*/
/*==============================================================*/
static void sp_snapback( PLAYER_WORK *pl_w )
{
	static void ( *snapback_jmp[])( PLAYER_WORK *pl_w ) = {
		snapback_init, snapback_move, snapback_end,
	} ;

	snapback_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void snapback_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	sp_stand_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = pl16_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 9 ) ;
	pls16_01_set( pl_w, 2 ) ;
}

/*--------------------------------------------------------------*/
static void snapback_move( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, 11 ) ;
	}
	if( pl_w->cg_type > 0 ){
		pl_w->cg_type = 0 ;
		pls16_01_set( pl_w, 0 ) ;
		if( pl_w->hit_flag ) pls16_01_set( pl_w, 1 ) ;
	}
	if( pl_w->cg_type < 0 ){
		pl_w->cg_type = 0 ;
		ofs.x = -85.0f ;
		ofs.y = DOT_Y( 75.0f ) ;
		ofs.z = 0.0f ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
}

/*--------------------------------------------------------------*/
static void snapback_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			The taunt											*/
/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Extra, 4 ) ;
	} else if( pl_char_move( pl_w ) < 0 ){
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	} else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pls16_07_set( pl_w ) ;
	}
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_move( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A kick that leaps off the wall						*/
/****************************************************************/
void	pl16_t_kick( PLAYER_WORK *pl_w )
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
	pl_w->atck_no = pl16_at_t_kick ;
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
	if( pl_char_move( pl_w ) < 0 ){
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_stop_stand_ret( pl_w ) ;
	} else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
	}
}

static void hold( PLAYER_WORK *pl_w );
static void jump_hold( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Holding the opponent								*/
/****************************************************************/
void	pl16_hold( PLAYER_WORK *pl_w )
{
	static void ( *hold_jmp[])( PLAYER_WORK *pl_w ) = {
		hold, hold, jump_hold,
	} ;

	hold_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void hold( PLAYER_WORK *pl_w )
{
	catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/*==============================================================*/
static void jump_hold( PLAYER_WORK *pl_w )
{
	catch_em_pos_set1( pl_w->catch_w, pl_w ) ;
}

static PLAYER_WORK *nml_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *crouch_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *jump_catch_ck( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Can a throw start? only with a punch				*/
/****************************************************************/
PLAYER_WORK *pl16_catch_ck( PLAYER_WORK *pl_w )
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
			pl_w->catch_no = 1 ;
			return em ;
		}
		return 0 ;
	}
	return 0 ;
}

static void nml_p_set( PLAYER_WORK *pl_w );
static void jump_p_set( PLAYER_WORK *pl_w );
static void ride_set( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Start the throw that pl16_catch_ck or Ride and		*/
/*			Slash chose											*/
/****************************************************************/
void	pl16_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_set, jump_p_set, ride_set,
	} ;

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
		em->pl_dir = em->disp_dir = pl_w->pl_dir ^ 1 ;
	}
	catch_gauge_add( pl_w ) ;
	pl_w->hit_stop = 10 ;
	ofs.x = -DOT_X( 37.0f ) ;
	ofs.y = DOT_Y( 64.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_char_set( pl_w, Char_Catch, 0 ) ;
	pls16_03_set( pl_w, 0 ) ;
}

/*==============================================================*/
static void jump_p_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	if( pl_w->dir & 1 ){
		pl_w->pl_dir = pl_w->disp_dir = pl_w->pl_dir ^ 1 ;
		em = pl_w->catch_w ;
		em->pl_dir = em->disp_dir = pl_w->pl_dir ^ 1 ;
	}
	catch_gauge_add( pl_w ) ;
	pl_w->hit_stop = 10 ;
	ofs.x = -60.0f ;
	ofs.y = DOT_Y( 88.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_char_set( pl_w, Char_Catch, 1 ) ;
	pls16_03_set( pl_w, 1 ) ;
}

/*==============================================================*/
static void ride_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	scr_excep_set( pl_w, 6, 6 ) ;
	catch_gauge_add( pl_w ) ;
	pl_w->hit_stop = 10 ;
	ofs.x = 0.0f ;
	ofs.y = 0.0f ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_char_set( pl_w, Char_Catch, 2 ) ;
}

static void nml_p_catch( PLAYER_WORK *pl_w );
static void jump_p_catch( PLAYER_WORK *pl_w );
static void ride_catch( PLAYER_WORK *pl_w );
static void nml_p_start( PLAYER_WORK *pl_w );
static void nml_p_hit( PLAYER_WORK *pl_w );
static void nml_p_end( PLAYER_WORK *pl_w );
static void jump_p_start( PLAYER_WORK *pl_w );
static void jump_p_hit( PLAYER_WORK *pl_w );
static void jump_p_end( PLAYER_WORK *pl_w );
static void ride_catch_hit( PLAYER_WORK *pl_w );
static void ride_catch_fall( PLAYER_WORK *pl_w );
static void ride_catch_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void	pl16_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_catch, jump_p_catch, ride_catch,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_catch( PLAYER_WORK *pl_w )
{
	static void ( *nml_p_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_start, nml_p_hit, nml_p_end,
	} ;

	nml_p_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void nml_p_hit( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		ofs.x = -DOT_X( 68.0f ) ;
		ofs.y = DOT_Y( 72.0f ) ;
		ofs.z = 0.0f ;
		hit_mark_set( pl_w, &ofs, 1 ) ;
		pl_se_req( pl_w, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void nml_p_end( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->catch_w ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->atck_no = pl16_at_throw_p ;
	}
}

/*==============================================================*/
static void jump_p_catch( PLAYER_WORK *pl_w )
{
	static void ( *jump_p_jmp[])( PLAYER_WORK *pl_w ) = {
		jump_p_start, jump_p_hit, jump_p_end,
	} ;

	jump_p_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void jump_p_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void jump_p_hit( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		ofs.x = -DOT_X( 70.0f ) ;
		ofs.y = DOT_Y( 95.0f ) ;
		ofs.z = 0.0f ;
		hit_mark_set( pl_w, &ofs, 1 ) ;
		pl_se_req( pl_w, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void jump_p_end( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->catch_w ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_jump_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->atck_no = pl16_at_air_throw ;
	}
}

/*==============================================================*/
static void ride_catch( PLAYER_WORK *pl_w )
{
	static void ( *ride_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		ride_catch_hit, ride_catch_fall, ride_catch_end,
	} ;

	ride_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			Hit three times in the air, then let go				*/
/*==============================================================*/
static void ride_catch_hit( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->catch_w ;
	VEC3 ofs ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type < 0 ){
		pl_w->routine_2 ++ ;
		SCR_EXCEP_SET ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->atck_no = pl16_at_ride_3 ;
		pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 3.5f ) : DOT_X( 3.5f ) ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 6.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
	} else {
		switch( pl_w->cg_type ){
		case 1 :
			pl_w->cg_type = 0 ;
			em->atck_no = pl16_at_ride_1 ;
			catch_damage( pl_w ) ;
			ofs.x = -DOT_X( 22.0f ) ;
			ofs.y = DOT_Y( 52.0f ) ;
			ofs.z = 0.0f ;
			hit_mark_set( pl_w, &ofs, 1 ) ;
			pl_se_req( pl_w, 4 ) ;
			break ;
		case 2 :
			pl_w->cg_type = 0 ;
			em->atck_no = pl16_at_ride_1 ;
			catch_damage( pl_w ) ;
			ofs.x = -45.0f ;
			ofs.y = DOT_Y( 45.0f ) ;
			ofs.z = 0.0f ;
			hit_mark_set( pl_w, &ofs, 1 ) ;
			pl_se_req( pl_w, 4 ) ;
			break ;
		case 3 :
			pl_w->cg_type = 0 ;
			em->atck_no = pl16_at_ride_2 ;
			catch_damage( pl_w ) ;
			ofs.x = -60.0f ;
			ofs.y = 90.0f ;
			ofs.z = 0.0f ;
			hit_mark_set( pl_w, &ofs, 1 ) ;
			pl_se_req( pl_w, 6 ) ;
			break ;
		}
	}
}

/*--------------------------------------------------------------*/
static void ride_catch_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	/* nothing until the fall */
	if( pl_w->spd.y > 0.0f ) return ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Jump, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void ride_catch_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void	pl16_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl16_sp_burst ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl16_sp_burst ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl16_sp_stinger ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl16_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl16_sp_burst ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl16_sp_burst ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl16_sp_stinger ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl16_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl16_sp_bone ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl16_sp_spine ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl16_sp_ricochet ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void	pl16_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl16_sp_bone ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl16_sp_spine ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl16_sp_ricochet ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl16.c" End of File	*/

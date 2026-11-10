/* Iceman (character 09): command checks, normal attacks, appearance, win and lose poses, special
   moves and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl09_def.h"

void pl09_main( PLAYER_WORK *pl_w );
void pl09_com_ck( PLAYER_WORK *pl_w );
void pl09_atck_set( PLAYER_WORK *pl_w );
void pl09_atck( PLAYER_WORK *pl_w );
void pl09_dash( PLAYER_WORK *pl_w );
void pl09_noki( PLAYER_WORK *pl_w );
void pl09_appear( PLAYER_WORK *pl_w );
void pl09_win( PLAYER_WORK *pl_w );
void pl09_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl09_catch_ck( PLAYER_WORK *pl_w );
void pl09_catch_set( PLAYER_WORK *pl_w );
void pl09_catch( PLAYER_WORK *pl_w );
void pl09_hold( PLAYER_WORK *pl_w );
int pl09_delay_ck( PLAYER_WORK *pl_w );
void pl09_t_kick( PLAYER_WORK *pl_w );
void pl09_upper( PLAYER_WORK *pl_w );
void pl09_sp_unison( PLAYER_WORK *pl_w );
void pl09_sp_gattai( PLAYER_WORK *pl_w );
void pl09_sp_assist( PLAYER_WORK *pl_w );
void pl09_sp_zero( PLAYER_WORK *pl_w );

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
static Uint16 com_avalanche[] = { 3, 0, pl09_sp_avalanche, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Ice Avalanche */
static Uint16 com_beam[] = { 3, COM_GROUND, pl09_sp_beam, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Ice Beam */
static Uint16 com_jbeam[] = { 3, COM_AIR, pl09_sp_jbeam, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Ice Beam */
static Uint16 com_fist[] = { 3, COM_GROUND, pl09_sp_fist, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Ice Fist */
static Uint16 com_arctic[] = { 3, COM_HYPER | COM_ALL_SW, pl09_sp_arctic, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Arctic Attack */

static void ( *pl09_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl09_main,
	pl09_com_ck,
	pl09_atck_set,
	pl09_atck,
	pl09_dash,
	pl09_noki,
	pl09_appear,
	pl09_win,
	pl09_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl09_catch_ck,	/* returns a value */
	pl09_catch_set,
	pl09_catch,
	pl09_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl09_delay_ck,
	pl09_t_kick,
	pl09_upper,
	pl09_sp_unison,
	pl09_sp_gattai,
	pl09_sp_assist,
	pl09_sp_zero,
} ;

static int snapback_ck( PLAYER_WORK *pl_w );
static int chouhatsu_ck( PLAYER_WORK *pl_w );
static Uint8 beam_ck( PLAYER_WORK *pl_w );
static Uint8 jbeam_ck( PLAYER_WORK *pl_w );
static Uint8 avalanche_ck( PLAYER_WORK *pl_w );
static Uint8 fist_ck( PLAYER_WORK *pl_w );
static Uint8 arctic_ck( PLAYER_WORK *pl_w );
static int arctic_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl09_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl09_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl09_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( arctic_ck( pl_w ) ) return ;
	if( fist_ck( pl_w ) ) return ;
	if( avalanche_ck( pl_w ) ) return ;
	if( beam_ck( pl_w ) ) return ;
	if( jbeam_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
static int snapback_ck( PLAYER_WORK *pl_w )
{
	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl09_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static int chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 5 ) ) return 0 ;
	pl_w->pl_sp_type = pl09_sp_chouhatsu ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 beam_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_beam, &pl_w->com_w0 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->pl_sp_type = pl09_sp_beam ;
	return 1 ;
}

/*==============================================================*/
/*			In the air: once a jump, unless it is a super		*/
/*			jump												*/
/*==============================================================*/
static Uint8 jbeam_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_jbeam, &pl_w->com_w3 ) ) return 0 ;
	if( !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w3 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl09_sp_jbeam ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 avalanche_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_avalanche, &pl_w->com_w2 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl09_sp_avalanche ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 fist_ck( PLAYER_WORK *pl_w )
{
	PL09_LOCAL_W *lw = ( PL09_LOCAL_W * )pl_w->tlocal ;

	if( !com_nml_ck( pl_w, com_fist, &pl_w->com_w ) ) return 0 ;
	if( lw->arm_flag ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl09_sp_fist ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 arctic_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_arctic, &pl_w->com_w1 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR ){
		if( pl_w->jump_com_cnt && !pl_w->sjump_flag ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl09_sp_arctic ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl09_delay_ck( PLAYER_WORK *pl_w )
{
	if( arctic_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static int arctic_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_arctic, &pl_w->com_w1 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 2 ;
	return 1 ;
}

/****************************************************************/
/*			The body's colour cycles, and some attacks call		*/
/*			out													*/
/****************************************************************/
void pl09_main( PLAYER_WORK *pl_w )
{
	PL09_LOCAL_W *lw = ( PL09_LOCAL_W * )pl_w->tlocal ;

	lw->unk_2 = 0 ;
	if( lw->arm_flag && !lw->arm_hit_cnt ) lw->arm_flag = 0 ;
	if( --lw->col_timer <= 0 ){
		lw->col_timer = 4 ;
		if( ++lw->col_step > 6 ){
			lw->col_step = 0 ;
			lw->col_timer += Rnd() % 48 + 16 ;
		}
		ef_color_set( pl_w, 0, pl_w->pl_color * 7 + lw->col_step + 24, 1 ) ;
	}
	if( !pl_w->char_no.b.grp && ( pl_w->char_no.b.no == 2 || pl_w->char_no.b.no == 3 ) && pl_w->cg_type ){
		pls09_01_set( pl_w, pl_w->cg_type + 0xff ) ;
		pl_w->cg_type = 0 ;
	}
}

/*==============================================================*/
/*			With the arm effect on, a punch starts one			*/
/*==============================================================*/
static void punch_ef( PLAYER_WORK *pl_w )
{
	PL09_LOCAL_W *lw = ( PL09_LOCAL_W * )pl_w->tlocal ;

	if( lw->arm_flag ) shl09_04_set( pl_w ) ;
}

static void nml_atck( PLAYER_WORK *pl_w );
static void stand_punch( PLAYER_WORK *pl_w );
static void crouch_punch( PLAYER_WORK *pl_w );
static void stand_kick( PLAYER_WORK *pl_w );
static void crouch_kick( PLAYER_WORK *pl_w );
static void jump_atck( PLAYER_WORK *pl_w );
static void jump_punch( PLAYER_WORK *pl_w );
static void jump_kick( PLAYER_WORK *pl_w );
static void jump_atck_ck( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Normal attacks										*/
/****************************************************************/
void pl09_atck_set( PLAYER_WORK *pl_w )
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
		pl_se_req( pl_w, SE_SWING_H ) ;
		pl_w->chain_tbl = hp_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Stand_P, pl_w->char_no.b.no ) ;
	punch_ef( pl_w ) ;
}

/*==============================================================*/
static void crouch_punch( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.grp = 9 ;
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_CROUCH_LP ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		punch_ef( pl_w ) ;
		break ;
	case 1:
		pl_w->char_no.b.grp = 9 ;
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_CROUCH_MP ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		punch_ef( pl_w ) ;
		break ;
	case 2:
		pl_w->char_no.b.grp = 9 ;
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_CROUCH_HP ;
		pl_w->chain_tbl = hp_tbl ;
		pl_se_req( pl_w, SE_SWING_H ) ;
		pl_w->nml_level = 2 ;
		shl09_00_set( pl_w, 3 ) ;
		shl09_00_set( pl_w, 4 ) ;
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
		pl_w->char_no.b.grp = 11 ;
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_JUMP_LP ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_lp_tbl ;
		else pl_w->chain_tbl = sj_lp_tbl ;
		pl_w->nml_level = 0 ;
		punch_ef( pl_w ) ;
		shl09_00_set( pl_w, 2 ) ;
		break ;
	case 1:
		pl_w->char_no.b.grp = 11 ;
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_JUMP_MP ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mp_tbl ;
		else pl_w->chain_tbl = sj_mp_tbl ;
		pl_w->nml_level = 1 ;
		punch_ef( pl_w ) ;
		break ;
	case 2:
		pl_w->char_no.b.grp = 11 ;
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_JUMP_HP ;
		if( pl_w->atck_lever & KEY_DOWN ){
			pl_w->char_no.b.no = 5 ;
			pl_w->atck_no = pl09_at_jdown_hp ;
		} else punch_ef( pl_w ) ;
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
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_JUMP_HK ;
		if( pl_w->atck_lever & KEY_DOWN ){
			pl_w->char_no.b.no = 5 ;
			pl_w->atck_no = pl09_at_jdown_hk ;
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
/*			In the air, only while that button's jump			*/
/*			attacks are left									*/
/*==============================================================*/
static void jump_atck_ck( PLAYER_WORK *pl_w )
{
	if( ( pl_w->atck_pk == 0 && ( pl_w->jump_atk_cnt & JATK_P_MASK ) )
	 || ( pl_w->atck_pk != 0 && ( pl_w->jump_atk_cnt & JATK_K_MASK ) ) ) jump_atck( pl_w ) ;
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
void pl09_atck( PLAYER_WORK *pl_w )
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
	PL09_LOCAL_W *lw = ( PL09_LOCAL_W * )pl_w->tlocal ;

	if( lw->arm_flag ) pl_w->hit_ng_timer = 1 ;
	switch( pl_w->nml_no ){
	case 2:
		if( pl_char_move( pl_w ) < 0 ){
			pl_stand_ret( pl_w ) ;
			return ;
		}
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			if( !lw->arm_flag ) shl09_00_set( pl_w, 0 ) ;
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
	PL09_LOCAL_W *lw = ( PL09_LOCAL_W * )pl_w->tlocal ;

	switch( pl_w->nml_no ){
	case 0:
	case 1:
		if( lw->arm_flag ) pl_w->hit_ng_timer = 1 ;
	case 2:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
static void stand_kick_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 2:
		if( pl_char_move( pl_w ) < 0 ){
			pl_stand_ret( pl_w ) ;
			return ;
		}
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			shl09_00_set( pl_w, 1 ) ;
		}
		break ;
	case 0:
	case 1:
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
	PL09_LOCAL_W *lw = ( PL09_LOCAL_W * )pl_w->tlocal ;

	if( lw->arm_flag ) pl_w->hit_ng_timer = 1 ;
	switch( pl_w->nml_no ){
	case 2:
		if( pl_char_move( pl_w ) < 0 ){
			pl_jump_ret( pl_w ) ;
			return ;
		}
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			shl09_00_set( pl_w, 6 ) ;
		}
		break ;
	case 0:
	case 1:
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
void pl09_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_w->atck_no = pl09_at_upper ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_char_set( pl_w, Char_Extra, 9 ) ;
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
void pl09_dash( PLAYER_WORK *pl_w )
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
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 10.0f ) : -DOT_X( 10.0f ) ;
		pl_w->timer0 = 16 ;
		dash_move( pl_w ) ;
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
	if( --pl_w->timer0 < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 4.0f ) : -DOT_X( 4.0f ) ;
		pl_char_set( pl_w, Char_Dash, 2 ) ;
		dash_end( pl_w ) ;
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
static void noki_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash, a hop								*/
/****************************************************************/
void pl09_noki( PLAYER_WORK *pl_w )
{
	static void ( *noki_jmp[])( PLAYER_WORK *pl_w ) = {
		noki_init, noki_jump, noki_land, noki_end,
	} ;

	noki_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void noki_init( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 8.0f ) : DOT_X( 8.0f ) ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 3.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.25f ) ;
		noki_jump( pl_w ) ;
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
		pl_land_ef_set( pl_w ) ;
		pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 4.0f ) : DOT_X( 4.0f ) ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		noki_land( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void noki_land( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type == 6 ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void noki_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void appear_init( PLAYER_WORK *pl_w );
static void appear_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl09_appear( PLAYER_WORK *pl_w )
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
	pl_w -> disp_dir = 0 ;
	pl_char_set( pl_w, Char_Appear, 0 ) ;
	pls09_00_set( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void appear_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w -> routine_1 ++ ;
		pl_w -> disp_dir = pl_w -> pl_dir ;
	}
}

/* the win poses: three at random after a KO, one for each other way to win */
static Sint8 win_ko_no[3] = { 0, 1, 2 } ;
static Sint8 win_type_no[5] = { 4, 4, 3, 4, 4 } ;

static void win_init( PLAYER_WORK *pl_w );
static void win_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl09_win( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ) win_init( pl_w ) ;
	else win_move( pl_w ) ;
}

/*==============================================================*/
static void win_init( PLAYER_WORK *pl_w )
{
	Sint8 no ;

	pl_w -> routine_2 ++ ;
	if( pl_be_cnt( pl_w ) >= 2 ){
		pl_char_set( pl_w, Char_Win, 2 ) ;
		return ;
	}
	no = win_type_no[pl_w->type] ;
	if( !pl_w->type ){
		pl_w -> timer0 = Rnd() % 3 ;
		no = win_ko_no[pl_w->timer0] ;
		if( pl_w->timer0 != 2 ) pls09_03_set( pl_w, pl_w->timer0 ) ;
	}
	pl_char_set( pl_w, Char_Win, no ) ;
}

/*==============================================================*/
static void win_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_win_end_ck( pl_w ) ) pl_stand_ret( pl_w ) ;
}

static void sp_avalanche( PLAYER_WORK *pl_w );
static void sp_beam( PLAYER_WORK *pl_w );
static void sp_arctic( PLAYER_WORK *pl_w );
static void sp_fist( PLAYER_WORK *pl_w );
static void sp_jbeam( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Special moves and hyper combos						*/
/****************************************************************/
void pl09_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *pl_sp_tbl[])( PLAYER_WORK *pl_w ) = {
		sp_avalanche,					/* 00 Ice Avalanche */
		sp_beam,					/* 01 Ice Beam */
		sp_arctic,					/* 02 Arctic Attack (hyper) */
		sp_fist,					/* 03 Ice Fist */
		sp_jbeam,					/* 04 Ice Beam, in the air */
		sp_chouhatsu,					/* 05 Taunt */
		sp_snapback,					/* 06 Snapback */
	} ;

	pl_sp_tbl[ pl_w->pl_sp_type ]( pl_w ) ;
}

static void avalanche_init( PLAYER_WORK *pl_w );
static void avalanche_move( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Done on the ground or in the air					*/
/*==============================================================*/
static void sp_avalanche( PLAYER_WORK *pl_w )
{
	static void ( *avalanche_jmp[])( PLAYER_WORK *pl_w ) = {
		avalanche_init, avalanche_move,
	} ;

	avalanche_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void avalanche_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	if( pl_w->pl_stance == STANCE_AIR ){
		pl_w->atck_no = pl_w->atck_level + 50 ;
		HIT_RESET( pl_w ) ;
		pl_gauge_add( pl_w, 5 ) ;
		pl_w->spd.x /= 8.0f ;
		pl_w->spd.y /= 8.0f ;
		pl_w->acc.x /= 8.0f ;
		pl_w->acc.y /= 8.0f ;
		pl_w->timer1 = 2 ;
	} else {
		pl_w->atck_no = pl_w->atck_level + 48 ;
		HIT_RESET( pl_w ) ;
		pl_gauge_add( pl_w, 5 ) ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_sp_ef_set( pl_w ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->timer1 = 0 ;
	}
	pl_char_set( pl_w, Char_Special, 0 ) ;
	avalanche_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void avalanche_move( PLAYER_WORK *pl_w )
{
	if( pl_w->pl_stance == STANCE_AIR ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->pos.y <= pl_w->ground_y ){
			pl_w->pos.y = pl_w->ground_y ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_land_ef_set( pl_w ) ;
			pl_w->pl_stance = STANCE_STAND ;
		}
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shl09_02_set( pl_w, pl_w->timer1 + pl_w->atck_level ) ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		if( pl_w->pl_stance == STANCE_AIR ){
			pl_w->acc.y = -DOT_Y( 0.375f ) ;
			pl_jump_ret( pl_w ) ;
		} else pl_stand_ret( pl_w ) ;
	}
}

static void beam_init( PLAYER_WORK *pl_w );
static void beam_wait( PLAYER_WORK *pl_w );
static void beam_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_beam( PLAYER_WORK *pl_w )
{
	static void ( *beam_jmp[])( PLAYER_WORK *pl_w ) = {
		beam_init, beam_wait, beam_end,
	} ;

	beam_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void beam_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = pl_w->atck_level + 52 ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 6 ) ;
	pl_w->timer0 = 40 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	beam_wait( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void beam_wait( PLAYER_WORK *pl_w )
{
	if( --pl_w->timer0 <= 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 10 ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		shl09_03_set( pl_w, pl_w->atck_level ) ;
		pl_w->cg_type = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void beam_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void arctic_init( PLAYER_WORK *pl_w );
static void arctic_start( PLAYER_WORK *pl_w );
static void arctic_set( PLAYER_WORK *pl_w );
static void arctic_keep( PLAYER_WORK *pl_w );
static void arctic_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The hyper combo, on the ground or in the air		*/
/*==============================================================*/
static void sp_arctic( PLAYER_WORK *pl_w )
{
	static void ( *arctic_jmp[])( PLAYER_WORK *pl_w ) = {
		arctic_init, arctic_start, arctic_set, arctic_keep, arctic_end,
	} ;

	arctic_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			Drifts down to the floor							*/
/*==============================================================*/
static void arctic_fall( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
}

/*==============================================================*/
static void arctic_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl09_at_avalanche_l ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	if( pl_w->pl_stance != STANCE_AIR ){
		pl_char_set( pl_w, Char_Super, 0 ) ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_sp_ef_set( pl_w ) ;
	} else {
		pl_char_set( pl_w, Char_Super, 1 ) ;
		pl_w->spd.x /= 8.0f ;
		pl_w->acc.x /= 8.0f ;
		pl_w->spd.y /= 8.0f ;
		pl_w->acc.y /= 8.0f ;
	}
	pl_w->timer0 = 104 ;
	pl_w->timer1 = 30 ;
	arctic_start( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void arctic_start( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	arctic_fall( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
		if( pl_w->pl_stance == STANCE_AIR ){
			ofs.x = 0.0f ;
			ofs.y = 270.0f ;
		} else {
			ofs.x = 10.0f ;
			ofs.y = DOT_Y( 125.0f ) ;
		}
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void arctic_set( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->routine_2 ++ ;
	arctic_fall( pl_w ) ;
	if( pl_w->pl_stance != STANCE_AIR ) pl_char_set( pl_w, Char_Super, 2 ) ;
	else pl_char_set( pl_w, Char_Super, 3 ) ;
}

/*==============================================================*/
/*			Lasts longer while the buttons are pressed			*/
/*==============================================================*/
static void arctic_keep( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	arctic_fall( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shl09_01_set( pl_w ) ;
	}
	if( pl_w->timer1 >= 0 && sw_renda_ck( pl_w ) ){
		pl_w->timer1 -- ;
		pl_w->timer0 ++ ;
	}
	if( --pl_w->timer0 <= 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 60 ;
		if( pl_w->pl_stance != STANCE_AIR ) pl_char_set2( pl_w, Char_Super, 6, pl_w->cg_prm ) ;
		else pl_char_set2( pl_w, Char_Super, 7, pl_w->cg_prm ) ;
	}
}

/*--------------------------------------------------------------*/
static void arctic_end( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	arctic_fall( pl_w ) ;
	if( --pl_w->timer0 <= 0 ){
		if( pl_w->pl_stance == STANCE_AIR ) pl_jump_ret( pl_w ) ;
		else pl_stand_ret( pl_w ) ;
	}
}

static void fist_init( PLAYER_WORK *pl_w );
static void fist_move( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Puts the effect on the arm							*/
/*==============================================================*/
static void sp_fist( PLAYER_WORK *pl_w )
{
	static void ( *fist_jmp[])( PLAYER_WORK *pl_w ) = {
		fist_init, fist_move,
	} ;

	fist_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void fist_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 5 ) ;
	fist_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void fist_move( PLAYER_WORK *pl_w )
{
	PL09_LOCAL_W *lw = ( PL09_LOCAL_W * )pl_w->tlocal ;

	if( pl_char_move( pl_w ) < 0 ){
		lw->arm_hit_cnt = 3 ;
		lw->arm_flag = 1 ;
		pls09_02_set( pl_w ) ;
		pl_stand_ret( pl_w ) ;
	}
}

static void jbeam_init( PLAYER_WORK *pl_w );
static void jbeam_wait( PLAYER_WORK *pl_w );
static void jbeam_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			In the air											*/
/*==============================================================*/
static void sp_jbeam( PLAYER_WORK *pl_w )
{
	static void ( *jbeam_jmp[])( PLAYER_WORK *pl_w ) = {
		jbeam_init, jbeam_wait, jbeam_end,
	} ;

	jbeam_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void jbeam_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_air_set( pl_w ) ;
	pl_w->atck_no = pl_w->atck_level + 54 ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_char_set( pl_w, Char_Special, ( pl_w->atck_level ^ 1 ) + 8 ) ;
	pl_w->spd.x /= 8.0f ;
	pl_w->spd.y /= 8.0f ;
	pl_w->acc.x /= 8.0f ;
	pl_w->acc.y /= 8.0f ;
	pl_w->timer0 = 40 ;
	jbeam_wait( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void jbeam_wait( PLAYER_WORK *pl_w )
{
	if( --pl_w->timer0 <= 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, ( pl_w->atck_level ^ 1 ) + 12 ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shl09_03_set( pl_w, ( pl_w->atck_level ^ 1 ) + 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void jbeam_end( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Extra, 7 ) ;
	} else if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
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
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = pl09_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 8 ) ;
}

/*--------------------------------------------------------------*/
static void snapback_move( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_prm ){
		pl_w->cg_prm = 0 ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
		ofs.x = DOT_X( 44.0f ) ;
		ofs.y = DOT_Y( 48.0f ) ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shl09_00_set( pl_w, 5 ) ;
	}
}

static PLAYER_WORK *nml_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *crouch_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *jump_catch_ck( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Can a throw start?									*/
/****************************************************************/
PLAYER_WORK *pl09_catch_ck( PLAYER_WORK *pl_w )
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
/*			Start the throw that pl09_catch_ck chose			*/
/****************************************************************/
void	pl09_catch_set( PLAYER_WORK *pl_w )
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

	scr_excep_set( pl_w, 6, 6 ) ;
	if( PL_CK( pl_w->catch_w, PL_SERVBOT ) ) pl_w->catch_w->pos.y = DOT_Y( 57.0f ) ;
	if( PL_CK( pl_w->catch_w, PL_MEGAMAN ) ) pl_w->catch_w->pos.y = DOT_Y( 12.0f ) ;
	if( PL_CK( pl_w->catch_w, PL_ROLL ) ) pl_w->catch_w->pos.y = DOT_Y( 26.0f ) ;
	if( PL_CK( pl_w->catch_w, PL_MARROW ) ) pl_w->catch_w->pos.y = 30.0f ;
	ofs.x = -75.0f ;
	ofs.y = DOT_Y( 80.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 0 ) ;
	pls09_05_set( pl_w ) ;
}

/*==============================================================*/
static void nml_k_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	scr_excep_set( pl_w, 5, 5 ) ;
	if( PL_CK( pl_w->catch_w, PL_SERVBOT ) ) pl_w->catch_w->pos.y = DOT_Y( 57.0f ) ;
	if( PL_CK( pl_w->catch_w, PL_MEGAMAN ) ) pl_w->catch_w->pos.y = 30.0f ;
	if( PL_CK( pl_w->catch_w, PL_ROLL ) ) pl_w->catch_w->pos.y = DOT_Y( 31.0f ) ;
	if( PL_CK( pl_w->catch_w, PL_MARROW ) ) pl_w->catch_w->pos.y = 30.0f ;
	if( PL_CK( pl_w->catch_w, PL_BONE_WOLVERINE ) ) pl_w->catch_w->pos.y = 45.0f ;
	if( PL_CK( pl_w->catch_w, PL_WOLVERINE ) ) pl_w->catch_w->pos.y = 45.0f ;
	ofs.x = -75.0f ;
	ofs.y = DOT_Y( 80.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	catch_gauge_add( pl_w ) ;
	pl_w->routine_2 = 0 ;
	pl_char_set( pl_w, Char_Catch, 1 ) ;
}

/*==============================================================*/
static void jump_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	scr_excep_set( pl_w, 6, 6 ) ;
	ofs.x = -75.0f ;
	ofs.y = DOT_Y( 83.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 2 ) ;
	pls09_05_set( pl_w ) ;
}

static void nml_p_catch( PLAYER_WORK *pl_w );
static void nml_k_catch( PLAYER_WORK *pl_w );
static void jump_p_catch( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void	pl09_catch( PLAYER_WORK *pl_w )
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

	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 4 ;
		em->pl_stance = STANCE_AIR ;
		SCR_EXCEP_SET ;
		em->atck_no = pl09_at_throw_p ;
		em->pl_dir = pl_w->pl_dir ^ 1 ;
		pl_voice_req( pl_w, 32 ) ;
		pl_w->pos.y = pl_w->ground_y ;
	}
}

/*==============================================================*/
static void nml_k_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	if( !pl_w->routine_2 && pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pls09_04_set( pl_w ) ;
	}
	if( pl_w->cg_type < 0 ){
		pl_w->cg_type &= 15 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->pl_stance = STANCE_AIR ;
		SCR_EXCEP_SET ;
		em->atck_no = pl09_at_throw_k ;
		em->pl_dir = pl_w->pl_dir ;
		ofs.y = pl_w->pos.y + 105.0f ;
		ofs.x = !pl_w->disp_dir ? DOT_X( 19.0f ) : -DOT_X( 19.0f ) ;
		ofs.x += pl_w->pos.x ;
		ofs.z = pl_w->pos.z ;
		hitmark_set( &ofs, pl_w->disp_dir, 2, 0 ) ;
		pl_se_req( pl_w, 1 ) ;
		pl_w->pos.y = pl_w->ground_y ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void jump_p_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->pl_stance = STANCE_AIR ;
		SCR_EXCEP_SET ;
		em->atck_no = pl09_at_air_throw ;
		em->pl_dir = pl_w->pl_dir ;
		pl_voice_req( pl_w, 32 ) ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

static void hold( PLAYER_WORK *pl_w );
static void hold_grip( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Holding the opponent								*/
/****************************************************************/
void	pl09_hold( PLAYER_WORK *pl_w )
{
	static void ( *hold_jmp[])( PLAYER_WORK *pl_w ) = {
		hold, hold_grip, hold_grip,
	} ;

	hold_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void hold( PLAYER_WORK *pl_w )
{
	catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/*==============================================================*/
static void hold_grip( PLAYER_WORK *pl_w )
{
	if( pl_w->catch_w->cg_prm ) catch_em_pos_set0( pl_w->catch_w, pl_w ) ;
	else catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void	pl09_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl09_sp_arctic ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl09_sp_arctic ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl09_sp_arctic ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl09_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl09_sp_arctic ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl09_sp_arctic ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl09_sp_arctic ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl09_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl09_sp_beam ;
		pl_w->atck_level = 0 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl09_sp_avalanche ;
		pl_w->atck_level = 0 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl09_sp_beam ;
		pl_w->atck_level = 0 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void	pl09_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl09_sp_beam ;
		pl_w->atck_level = 0 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl09_sp_avalanche ;
		pl_w->atck_level = 0 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl09_sp_avalanche ;
		pl_w->atck_level = 0 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_move( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A kick that leaps off the wall						*/
/****************************************************************/
void	pl09_t_kick( PLAYER_WORK *pl_w )
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
	pl_w->atck_no = pl09_at_t_kick ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 5 ) ;
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
		pl_char_set( pl_w, Char_Extra, 6 ) ;
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

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl09.c" End of File	*/

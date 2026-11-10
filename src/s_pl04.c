/* Anakaris (character 04): command checks, normal attacks, appearance, win and lose poses, special
   moves and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl04_def.h"

/* where the shots start, by level, and by timer0 */
const Uint16 pl04_coffin_x_tbl[4] = { 96, 160, 224, 288 } ;
const Uint16 pl04_coffins_x_tbl[8] = { 160, 256, 112, 192, 96, 256, 144, 224 } ;

void pl04_main( PLAYER_WORK *pl_w );
void pl04_com_ck( PLAYER_WORK *pl_w );
void pl04_atck_set( PLAYER_WORK *pl_w );
void pl04_atck( PLAYER_WORK *pl_w );
void pl04_dash( PLAYER_WORK *pl_w );
void pl04_noki( PLAYER_WORK *pl_w );
void pl04_appear( PLAYER_WORK *pl_w );
void pl04_win( PLAYER_WORK *pl_w );
void pl04_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl04_catch_ck( PLAYER_WORK *pl_w );
void pl04_catch_set( PLAYER_WORK *pl_w );
void pl04_catch( PLAYER_WORK *pl_w );
void pl04_hold( PLAYER_WORK *pl_w );
int pl04_delay_ck( PLAYER_WORK *pl_w );
void pl04_t_kick( PLAYER_WORK *pl_w );
void pl04_upper( PLAYER_WORK *pl_w );
void pl04_sp_unison( PLAYER_WORK *pl_w );
void pl04_sp_gattai( PLAYER_WORK *pl_w );
void pl04_sp_assist( PLAYER_WORK *pl_w );
void pl04_sp_zero( PLAYER_WORK *pl_w );

/* commands: time or steps, kind, the special move, then the stick and buttons */
static Uint16 com_coffin[] = { 3, 0, pl04_sp_coffin, 0, KEY_DOWN, 0x0000, KEY_DOWN, KEY_ATCK } ;	/* Coffin Dance */
static Uint16 com_judgement[] = { 3, COM_AIR, pl04_sp_judgement, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Royal Judgement */
static Uint16 com_cobra[] = { 2, 0, pl04_sp_cobra, 0, KEY_BACK, KEY_FORWARD, KEY_PUNCH } ;	/* Cobra Blow */
static Uint16 com_mummy[] = { 3, 0, pl04_sp_mummy, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Mummy Drop */
static Uint16 com_magic[] = { 4, COM_HYPER, pl04_sp_magic, 0, KEY_HK, KEY_LP, KEY_DOWN, KEY_LK, KEY_HP } ;	/* Pharaoh Magic (hyper) */
static Uint16 com_cobras[] = { 2, COM_HYPER | COM_ALL_SW, pl04_sp_cobras, 0, KEY_BACK, KEY_FORWARD, KEY_PUNCH } ;	/* Pharaoh Cobra Throw (hyper) */
static Uint16 com_illusion[] = { 4, COM_GROUND | COM_HYPER, pl04_sp_illusion, 0, KEY_LP, KEY_LP, KEY_FORWARD, KEY_LK, KEY_HP } ;	/* Pharaoh Illusion (hyper) */
static Uint16 com_coffins[] = { 3, COM_HYPER | COM_ALL_SW, pl04_sp_coffins, 0, KEY_DOWN, 0x0000, KEY_DOWN, KEY_PUNCH } ;	/* Coffin Fall (hyper) */

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

static void ( *pl04_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl04_main,
	pl04_com_ck,
	pl04_atck_set,
	pl04_atck,
	pl04_dash,
	pl04_noki,
	pl04_appear,
	pl04_win,
	pl04_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl04_catch_ck,	/* returns a value */
	pl04_catch_set,
	pl04_catch,
	pl04_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl04_delay_ck,
	pl04_t_kick,
	pl04_upper,
	pl04_sp_unison,
	pl04_sp_gattai,
	pl04_sp_assist,
	pl04_sp_zero,
} ;

static Uint8 coffin_ck( PLAYER_WORK *pl_w );
static Uint8 judgement_ck( PLAYER_WORK *pl_w );
static Uint8 cobra_ck( PLAYER_WORK *pl_w );
static Uint8 mummy_ck( PLAYER_WORK *pl_w );
static Uint8 magic_ck( PLAYER_WORK *pl_w );
static Uint8 cobras_ck( PLAYER_WORK *pl_w );
static Uint8 illusion_ck( PLAYER_WORK *pl_w );
static Uint8 coffins_ck( PLAYER_WORK *pl_w );
static Uint8 snapback_ck( PLAYER_WORK *pl_w );
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w );
static int magic_delay_ck( PLAYER_WORK *pl_w );
static int cobras_delay_ck( PLAYER_WORK *pl_w );
static int illusion_delay_ck( PLAYER_WORK *pl_w );
static int coffins_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl04_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl04_tbl[i] ;
}

/*==============================================================*/
/*			Stop, and back to standing							*/
/*==============================================================*/
static void stop_stand_ret( PLAYER_WORK *pl_w )
{
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_stand_ret( pl_w ) ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl04_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( magic_ck( pl_w ) ) return ;
	if( cobras_ck( pl_w ) ) return ;
	if( illusion_ck( pl_w ) ) return ;
	if( coffins_ck( pl_w ) ) return ;
	if( coffin_ck( pl_w ) ) return ;
	if( judgement_ck( pl_w ) ) return ;
	if( mummy_ck( pl_w ) ) return ;
	if( cobra_ck( pl_w ) ) return ;
	if( pl_w->cg_atck_flag != CG_ATCK_ON && pl_air_dash_sw_ck( pl_w ) ) return ;
	if( pl_air_dash_ck( pl_w, &pl_w->com_w12 ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
/*			The punch's strength counts double with a kick		*/
/*==============================================================*/
static Uint8 coffin_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_coffin, &pl_w->com_w0 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->atck_level += pl_w->atck_pk * 2 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl04_sp_coffin ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 judgement_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_judgement, &pl_w->com_w1 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl04_sp_judgement ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 cobra_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_cobra, &pl_w->com_w2 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl04_sp_cobra ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 mummy_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_mummy, &pl_w->com_w3 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w3 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl04_sp_mummy ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 magic_ck( PLAYER_WORK *pl_w )
{
	if( !com_seq_ck( pl_w, com_magic, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl04_sp_magic ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 cobras_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_cobras, &pl_w->com_w5 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl04_sp_cobras ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 illusion_ck( PLAYER_WORK *pl_w )
{
	if( !com_seq_ck( pl_w, com_illusion, &pl_w->com_w6 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl04_sp_illusion ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 coffins_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_coffins, &pl_w->com_w7 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl04_sp_coffins ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 snapback_ck( PLAYER_WORK *pl_w )
{
	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl04_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 9 ) ) return 0 ;
	pl_w->pl_sp_type = pl04_sp_chouhatsu ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl04_delay_ck( PLAYER_WORK *pl_w )
{
	if( magic_delay_ck( pl_w ) || cobras_delay_ck( pl_w ) || illusion_delay_ck( pl_w ) || coffins_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static int magic_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_seq_ck( pl_w, com_magic, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 4 ;
	return 1 ;
}

/*==============================================================*/
static int cobras_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_cobras, &pl_w->com_w5 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 5 ;
	return 1 ;
}

/*==============================================================*/
static int illusion_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_seq_ck( pl_w, com_illusion, &pl_w->com_w6 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 6 ;
	return 1 ;
}

/*==============================================================*/
static int coffins_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_seq_ck( pl_w, com_coffins, &pl_w->com_w7 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 7 ;
	return 1 ;
}

/****************************************************************/
void pl04_main( PLAYER_WORK *pl_w )
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
void pl04_atck_set( PLAYER_WORK *pl_w )
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
		pl_w->unk_2a2 = 1 ;
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
/*			Forward or back with the heavy kick slides him		*/
/*==============================================================*/
static void stand_kick( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		if( pl_w->atck_lever & KEY_BACK ){
			pl_w->char_no.b.no = 11 ;
			pl_w->atck_no = pl04_at_back_lk ;
			pl_se_req( pl_w, SE_SWING_M ) ;
			pl_w->chain_tbl = mk_tbl ;
			pl_w->nml_level = 1 ;
			HIT_RESET( pl_w ) ;
			pl_char_set( pl_w, Char_Extra, pl_w->char_no.b.no ) ;
			return ;
		}
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
		if( pl_w->atck_lever & ( KEY_FORWARD | KEY_BACK ) ){
			pl_w->spd.x = -1.6666666f ;
			pl_w->acc.x = 0.01953125f ;
			if( ( ( pl_w->atck_lever & KEY_BACK ) && !pl_w->disp_dir )
			 || ( ( pl_w->atck_lever & KEY_FORWARD ) && pl_w->disp_dir ) ){
				pl_w->spd.x = -pl_w->spd.x ;
				pl_w->acc.x = -pl_w->acc.x ;
			}
		} else {
			pl_w->spd.x = 0.0f ;
			pl_w->acc.x = 0.0f ;
		}
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
/*			Down with a kick drops him, and forward with the	*/
/*			heavy kick dives									*/
/*==============================================================*/
static void jump_kick( PLAYER_WORK *pl_w )
{
	if( pl_w->atck_lever & KEY_DOWN ){
		pl_w->routine_2 = 1 ;
		pl_w->jump_atk_cnt &= JATK_P_MASK ;
		pl_w->sjump_flag = 0 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		if( pl_w->atck_lever & ( KEY_FORWARD | KEY_BACK ) ){
			pl_w->char_no.b.no = 13 ;
			pl_w->atck_no = pl04_at_jdiag_k ;
		} else {
			pl_w->char_no.b.no = 12 ;
			pl_w->atck_no = pl04_at_jdown_k ;
		}
		pl_w->chain_tbl = j_hk_tbl ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, pl_w->char_no.b.no ) ;
		return ;
	}
	if( ( pl_w->atck_lever & KEY_LEVER ) == KEY_FORWARD ){
		pl_w->routine_2 = 3 ;
		pl_w->jump_atk_cnt &= JATK_P_MASK ;
		pl_w->sjump_flag = 0 ;
		/* both strengths take the heavy kick's table */
		if( !pl_w->nml_no ) pl_w->chain_tbl = j_hk_tbl ;
		else pl_w->chain_tbl = j_hk_tbl ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->atck_no = pl04_at_jfwd_k ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, 14 ) ;
		return ;
	}
	switch( pl_w->nml_no ){
	case 0:
		if( !pl_w->sjump_flag ){
			pl_w->char_no.b.no = 0 ;
			pl_w->atck_no = ATCK_JUMP_LK ;
			pl_w->chain_tbl = j_lk_tbl ;
		} else {
			pl_w->char_no.b.no = 8 ;
			pl_w->atck_no = pl04_at_sjump_lk ;
			pl_w->chain_tbl = sj_lk_tbl ;
		}
		pl_se_req( pl_w, SE_SWING_L ) ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		if( !pl_w->sjump_flag ){
			pl_w->char_no.b.no = 1 ;
			pl_w->atck_no = ATCK_JUMP_MK ;
			pl_w->chain_tbl = j_mk_tbl ;
		} else {
			pl_w->char_no.b.no = 9 ;
			pl_w->atck_no = pl04_at_sjump_mk ;
			pl_w->chain_tbl = sj_mk_tbl ;
		}
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		if( !pl_w->sjump_flag ){
			pl_w->char_no.b.no = 2 ;
			pl_w->atck_no = ATCK_JUMP_HK ;
			pl_w->chain_tbl = j_hk_tbl ;
		} else {
			pl_w->char_no.b.no = 10 ;
			pl_w->atck_no = pl04_at_sjump_hk ;
			pl_w->chain_tbl = sj_hk_tbl ;
		}
		pl_se_req( pl_w, SE_SWING_H ) ;
		pl_w->nml_level = 2 ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	if( !pl_w->sjump_flag ) pl_w->char_no.b.grp = 12 ;
	else pl_w->char_no.b.grp = 20 ;
	pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
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
static void jump_kick_nml( PLAYER_WORK *pl_w );
static void drop_start( PLAYER_WORK *pl_w );
static void drop_fall( PLAYER_WORK *pl_w );
static void dive_start( PLAYER_WORK *pl_w );
static void dive_fall( PLAYER_WORK *pl_w );
static void kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
void pl04_atck( PLAYER_WORK *pl_w )
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
static void stand_punch_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			The medium one hits again							*/
/*==============================================================*/
static void crouch_punch_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->atck_no = ATCK_CROUCH_MP ;
		HIT_RESET( pl_w ) ;
	}
}

/*==============================================================*/
static void stand_kick_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
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
	if( pl_w->acc.y == 0.0f ) pl_w->acc.y = -DOT_Y( 0.375f ) ;
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/*==============================================================*/
static void jump_kick_nml( PLAYER_WORK *pl_w )
{
	if( pl_w->acc.y == 0.0f ) pl_w->acc.y = -DOT_Y( 0.375f ) ;
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void drop_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		if( pl_w->char_no.b.no == 13 ){
			pl_w->spd.x = -DOT_X( 3.5f ) ;
			pl_w->spd.y = -7.5f ;
			if( pl_w->disp_dir ) pl_w->spd.x = -pl_w->spd.x ;
		} else {
			pl_w->spd.y = -DOT_Y( 4.0f ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void drop_fall( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 = 5 ;
		pl_w->spd.x = pl_w->acc.x = pl_w->spd.y = 0.0f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_char_set( pl_w, Char_Jump, 9 ) ;
	}
}

/*--------------------------------------------------------------*/
static void dive_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = -10.0f ;
		pl_w->acc.x = DOT_X( 0.375f ) ;
		if( pl_w->disp_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
	}
}

/*--------------------------------------------------------------*/
static void dive_fall( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = pl_w->acc.x = pl_w->spd.y = 0.0f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_char_set( pl_w, Char_Jump, 9 ) ;
	}
}

/*--------------------------------------------------------------*/
static void kick_land( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void jump_kick_move( PLAYER_WORK *pl_w )
{
	static void ( *jump_kick_jmp[])( PLAYER_WORK *pl_w ) = {
		jump_kick_nml, drop_start, drop_fall, dive_start, dive_fall, kick_land,
	} ;

	jump_kick_jmp[pl_w->routine_2]( pl_w ) ;
}

/****************************************************************/
/*			The launcher										*/
/****************************************************************/
void pl04_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_se_req( pl_w, SE_SWING_H ) ;
		pl_gauge_add( pl_w, 5 ) ;
		pl_w->atck_no = pl04_at_upper ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_char_set( pl_w, Char_Extra, 7 ) ;
		HIT_RESET( pl_w ) ;
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
/*			The forward dash: a glide for a few frames			*/
/****************************************************************/
void pl04_dash( PLAYER_WORK *pl_w )
{
	static void ( *dash_jmp[])( PLAYER_WORK *pl_w ) = {
		dash_init, dash_move, dash_stop, dash_end,
	} ;

	dash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dash_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->timer0 = 4 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.x = pl_w->pl_dir ? 12.5f : -12.5f ;
	pl_w->acc.x = pl_w->pl_dir ? -0.078125f : 0.078125f ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
}

/*--------------------------------------------------------------*/
static void dash_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( !--pl_w->timer0 ) pl_w->routine_2 ++ ;
}

/*==============================================================*/
/*			Ends with the animation, or as soon as the stick	*/
/*			lets go of forward									*/
/*==============================================================*/
static void dash_stop( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 || ( !pl_w->cpu_flag && ( pl_w->pl_sw_new & KEY_BACK ) ) ){
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
static void noki_stop( PLAYER_WORK *pl_w );
static void noki_end( PLAYER_WORK *pl_w );
static void wrap_start( PLAYER_WORK *pl_w );
static void wrap_out( PLAYER_WORK *pl_w );
static void wrap_wait( PLAYER_WORK *pl_w );
static void wrap_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash; from step 4 on, he leaves by one		*/
/*			side of the screen and comes back by the other		*/
/****************************************************************/
void pl04_noki( PLAYER_WORK *pl_w )
{
	static void ( *noki_jmp[])( PLAYER_WORK *pl_w ) = {
		noki_init, noki_move, noki_stop, noki_end,
		wrap_start, wrap_out, wrap_wait, wrap_end,
	} ;

	noki_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void noki_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->timer0 = 4 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 5.0f ) : DOT_X( 5.0f ) ;
	pl_w->acc.x = pl_w->pl_dir ? 0.065104164f : -0.065104164f ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
}

/*--------------------------------------------------------------*/
static void noki_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( !--pl_w->timer0 ) pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void noki_stop( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 || ( !pl_w->cpu_flag && ( pl_w->pl_sw_new & KEY_FORWARD ) ) ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Dash, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void noki_end( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void wrap_start( PLAYER_WORK *pl_w )
{
	pl_w->wall_ng_timer = 3 ;
	pl_w->catch_ng_timer = 3 ;
	pl_w->atck_ng_timer = 3 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( !--pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 4 ;
	}
}

/*==============================================================*/
/*			Out of sight: turn round and move to the			*/
/*			screen's other side									*/
/*==============================================================*/
static void wrap_out( PLAYER_WORK *pl_w )
{
	pl_w->wall_ng_timer = 3 ;
	pl_w->catch_ng_timer = 3 ;
	pl_w->atck_ng_timer = 3 ;
	if( !--pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 3 ;
		if( pl_w->disp_dir ^= 1 ) pl_w->pos.x = scr_work.scr_left + -DOT_X( 80.0f ) ;
		else pl_w->pos.x = scr_work.scr_right + DOT_X( 80.0f ) ;
		pl_char_set( pl_w, Char_Extra, 5 ) ;
	}
}

/*--------------------------------------------------------------*/
static void wrap_wait( PLAYER_WORK *pl_w )
{
	pl_w->wall_ng_timer = 3 ;
	pl_w->catch_ng_timer = 3 ;
	pl_w->atck_ng_timer = 3 ;
	if( !--pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 20 ;
		pl_w->catch_ng_timer = 0 ;
		scr_excep_clr() ;
	}
}

/*--------------------------------------------------------------*/
static void wrap_end( PLAYER_WORK *pl_w )
{
	pl_w->atck_ng_timer = 3 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( !--pl_w->timer0 ){
		pl_w->atck_ng_timer = 0 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void appear_init( PLAYER_WORK *pl_w );
static void appear_fall( PLAYER_WORK *pl_w );
static void appear_move( PLAYER_WORK *pl_w );
static void win_ko( PLAYER_WORK *pl_w );
static void lose_time_ko( PLAYER_WORK *pl_w );
static void win_time( PLAYER_WORK *pl_w );
static void draw_time( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The entrance: in a team match he drops in from		*/
/*			above												*/
/****************************************************************/
void pl04_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_init,
		appear_fall,
		appear_move,
	} ;

	appear_jmp[ pl_w->routine_2 ]( pl_w ) ;
}

/*==============================================================*/
static void appear_init( PLAYER_WORK *pl_w )
{
	if( vs_sys_work.vs_st < VS_APPEAR ){
		pl_w->disp_flag = 0 ;
		return ;
	} else {
		pl_w->routine_2 ++ ;
		pl_w->disp_flag = 1 ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->pos.y += DOT_Y( 248.0f ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = -DOT_Y( 3.0f ) ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Appear, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_fall( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_2 ++ ;
		pl_se_req( pl_w, 47 ) ;
		pl_char_move( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_1 ++ ;
		pl_char_set( pl_w, Char_Stand, 0 ) ;
	}
}

/****************************************************************/
void pl04_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko,
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
static void win_ko( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		if( ( Sys->game_timer & 1 ) && pl_be_cnt( pl_w ) == 1 ){
			pl_char_set( pl_w, Char_Win, CH04_WIN_KO0 ) ;
			pls04_01_set( pl_w ) ;
		} else {
			pl_char_set( pl_w, Char_Win, CH04_WIN_KO1 ) ;
		}
	}else{
		pl_char_move( pl_w ) ;
	}
}

/*==============================================================*/
static void lose_time_ko( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, CH04_LOSE ) ;
	}else{
		pl_char_move( pl_w ) ;
	}
}

/*==============================================================*/
static void win_time( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, CH04_WIN_TIME ) ;
	}else{
		pl_char_move( pl_w ) ;
	}
}

/*==============================================================*/
static void draw_time( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, CH04_TM_DRAW ) ;
	}else{
		pl_char_move( pl_w ) ;
	}
}

static void sp_coffin( PLAYER_WORK *pl_w );
static void sp_judgement( PLAYER_WORK *pl_w );
static void sp_cobra( PLAYER_WORK *pl_w );
static void sp_mummy( PLAYER_WORK *pl_w );
static void sp_magic( PLAYER_WORK *pl_w );
static void sp_cobras( PLAYER_WORK *pl_w );
static void sp_illusion( PLAYER_WORK *pl_w );
static void sp_coffins( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );

/****************************************************************/
void pl04_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *pl_sp_tbl[])( PLAYER_WORK *pl_w ) = {
		sp_coffin, sp_judgement, sp_cobra, sp_mummy, sp_magic, sp_cobras, sp_illusion, sp_coffins, sp_snapback, sp_chouhatsu,
	} ;

	pl_sp_tbl[ pl_w->pl_sp_type ]( pl_w ) ;
}

/*==============================================================*/
static void sp_coffin( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_dir_set( pl_w ) ;
		pl_gauge_add( pl_w, 5 ) ;
		if( pl_w->pl_stance != STANCE_AIR ){
			pl_w->pl_stance = STANCE_STAND ;
			pl_sp_ef_set( pl_w ) ;
			pl_w->atck_no = pl04_at_coffin ;
			HIT_RESET( pl_w ) ;
			pl_char_set( pl_w, Char_Special, 0 ) ;
		} else {
			pl_w->pl_stance = STANCE_AIR ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_w->acc.y = -DOT_Y( 0.375f ) ;
			pl_w->atck_no = pl04_at_jcoffin ;
			HIT_RESET( pl_w ) ;
			pl_char_set( pl_w, Char_Special, 1 ) ;
		}
	} else {
		if( pl_w->cg_type ){
			if( ew = shl04_00_set( pl_w, 0 ) ){
				pl_w->cg_type = 0 ;
				ew->pos.x = DOT_X( pl04_coffin_x_tbl[pl_w->atck_level] ) ;
				if( !pl_w->disp_dir ) ew->pos.x = -ew->pos.x ;
				ew->pos.x += pl_w->pos.x ;
			}
		}
		if( pl_w->pl_stance != STANCE_AIR ){
			if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		} else {
			if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
		}
	}
}

/*==============================================================*/
static void sp_judgement( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew, *e ;
	int i ;

	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_dir_set( pl_w ) ;
		pl_gauge_add( pl_w, 2 ) ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_w->atck_no = pl_w->atck_level + 50 ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 2 ) ;
	} else {
		if( pl_w->cg_type ){
			if( ew = shl04_01_set( pl_w, 0 ) ){
				pl_w->cg_type = 0 ;
				i = 0 ;
				while( i < 4 ){
					if( e = pls04_00_set2( ew, 0, 0 ) ) e->timer0 = i * 7 ;
					i ++ ;
				}
			}
		}
		if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
	}
}

/*==============================================================*/
static void sp_cobra( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_dir_set( pl_w ) ;
		pl_gauge_add( pl_w, 16 ) ;
		if( pl_w->pl_stance != STANCE_AIR ){
			pl_w->pl_stance = STANCE_STAND ;
			pl_sp_ef_set( pl_w ) ;
			if( pl_w->team_type == 3 ){
				pl_w->atck_no = pl04_at_vc_cobra ;
				HIT_RESET( pl_w ) ;
			}else{
				pl_w->atck_no = pl_w->atck_level + 52 ;
				HIT_RESET( pl_w ) ;
			}
			pl_char_set( pl_w, Char_Special, pl_w->atck_level + 4 ) ;
		} else {
			pl_w->pl_stance = STANCE_AIR ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_w->acc.y = -DOT_Y( 0.375f ) ;
			pl_w->atck_no = pl_w->atck_level + 54 ;
			HIT_RESET( pl_w ) ;
			pl_char_set( pl_w, Char_Special, pl_w->atck_level + 6 ) ;
		}
	} else {
		if( pl_w->cg_type ){
			/* nothing: the arcade keeps the empty test */
		}
		if( pl_w->pl_stance != STANCE_AIR ){
			if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		} else {
			if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
		}
	}
}

static void mummy_init( PLAYER_WORK *pl_w );
static void mummy_move( PLAYER_WORK *pl_w );
static void mummy_catch( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_mummy( PLAYER_WORK *pl_w )
{
	static void ( *mummy_jmp[])( PLAYER_WORK *pl_w ) = {
		mummy_init, mummy_move, mummy_catch,
	} ;

	mummy_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void mummy_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_gauge_add( pl_w, 16 ) ;
	if( pl_w->pl_stance != STANCE_AIR ){
		pl_w->pl_stance = STANCE_STAND ;
		pl_sp_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 9 ) ;
	} else {
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 14 ) ;
	}
	pl_w->dir = 0 ;
	pl_w->spd.z = 160.0f ;
	if( pl_w->atck_level ) pl_w->spd.z = DOT_X( 208.0f ) ;
	if( !pl_w->disp_dir ) pl_w->spd.z = -pl_w->spd.z ;
}

/*--------------------------------------------------------------*/
static void mummy_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pls04_00_set( pl_w, 1, 0 ) ;
		pls04_00_set( pl_w, 1, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void mummy_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_w->target_w->pl_stance != STANCE_CROUCH && pl_w->cg_type ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->routine_2 = 0 ;
			pl_w->dir = 1 ;
			pl_w->catch_no = 0xc0 ;
			pl_catch_set( pl_w, em ) ;
			return ;
		}
	}
	if( pl_w->pl_stance != STANCE_AIR ){
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	} else {
		if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
	}
}

static void magic_init( PLAYER_WORK *pl_w );
static void magic_start( PLAYER_WORK *pl_w );
static void magic_shot( PLAYER_WORK *pl_w );
static void magic_wait( PLAYER_WORK *pl_w );
static void magic_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_magic( PLAYER_WORK *pl_w )
{
	static void ( *magic_jmp[])( PLAYER_WORK *pl_w ) = {
		magic_init, magic_start, magic_shot, magic_wait, magic_end,
	} ;

	magic_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void magic_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->timer0 = 0 ;
	pl_w->timer1 = 0 ;
	pl_w->ef_w = 0 ;
	pl_dir_set( pl_w ) ;
	if( pl_w->pl_stance == STANCE_AIR ){
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_w->atck_no = pl04_at_magic ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 3 ) ;
	} else {
		pl_w->pl_stance = STANCE_STAND ;
		pl_sp_ef_set( pl_w ) ;
		pl_w->atck_no = pl04_at_magic ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void magic_start( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->hit_flag = 0 ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = -DOT_X( 16.0f ) ;
		ofs.y = DOT_Y( 128.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void magic_shot( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew, *em ;
	int i ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->hyper_bg_tm = 5 ;
	if( -- pl_w->timer1 < 0 ){
		pl_char_move( pl_w ) ;
		if( pl_w->cg_type ){
			pl_w->routine_2 ++ ;
			pl_w->cg_type = 0 ;
			pl_w->hit_flag = 0 ;
			if( ++ pl_w->timer0 >= 4 ){
				pl_w->delay_flag = 0 ;
				pl_w->delay_timer = 0 ;
				pl_w->hyper_bg = 0 ;
				pl_w->hyper_bg_tm = 0 ;
				pl_w->timer1 = 0 ;
				if( ew = shl04_00_set( pl_w, 2 ) ){
					if( !( em = pl_w->ef_w ) ) em = pl_w->target_w ;
					ew->pos.x = em->pos.x ;
					ew->dir = 0 ;
					if( !pl_w->cpu_flag && ( pl_w->pl_new_lvbt & KEY_START ) ) ew->dir = 1 ;
				}
			} else {
				pl_w->timer1 = 96 ;
				if( ew = shl04_01_set( pl_w, pl_w->timer0 ) ){
					i = 0 ;
					while( i < 4 ){
						pls04_00_set2( ew, 2, i * 32 ) ;
						i ++ ;
					}
				}
			}
		}
	}
}

/*--------------------------------------------------------------*/
static void magic_wait( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->hyper_bg_tm = 5 ;
	if( -- pl_w->timer1 < 1 ){
		pl_w->routine_2 ++ ;
		if( pl_w->pl_stance != STANCE_AIR ) pl_char_set( pl_w, Char_Super, 2 ) ;
		else pl_char_set( pl_w, Char_Super, 5 ) ;
		return ;
	}
	if( pl_w->hit_flag ){
		pl_w->routine_2 -- ;
		pl_w->timer1 = 43 ;
	}
}

/*--------------------------------------------------------------*/
static void magic_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		if( pl_w->pl_stance == STANCE_AIR ) pl_jump_ret( pl_w ) ;
		else pl_stand_ret( pl_w ) ;
	}
}

static void cobras_init( PLAYER_WORK *pl_w );
static void cobras_start( PLAYER_WORK *pl_w );
static void cobras_shot( PLAYER_WORK *pl_w );
static void cobras_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_cobras( PLAYER_WORK *pl_w )
{
	static void ( *cobras_jmp[])( PLAYER_WORK *pl_w ) = {
		cobras_init, cobras_start, cobras_shot, cobras_end,
	} ;

	cobras_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void cobras_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->timer0 = 0 ;
	pl_dir_set( pl_w ) ;
	if( pl_w->pl_stance == STANCE_AIR ){
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
	} else {
		pl_w->pl_stance = STANCE_STAND ;
		pl_sp_ef_set( pl_w ) ;
	}
	pl_w->atck_no = pl04_at_cobras ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 16 ) ;
}

/*--------------------------------------------------------------*/
static void cobras_start( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->dir = 0 ;
		pl_w->timer1 = 0 ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = -DOT_X( 32.0f ) ;
		ofs.y = 240.0f ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void cobras_shot( PLAYER_WORK *pl_w )
{
	Uint8 rnd ;
	Sint8 flag ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_prm ){
		pl_w->cg_prm = 0 ;
		pl_se_req( pl_w, 48 ) ;
	}
	if( !( flag = pl_w->cg_type ) ) return ;
	if( flag == -1 ){
		pl_w->routine_2 ++ ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
	} else if( pl_w->team_type == 5 || pl_w->cpu_flag || pl_w->team_type == 4 ){
		if( Sys->game_timer0 & 7 ) return ;
		rnd = Rnd() & 15 ;
		if( ( rnd & 1 ) && !( pl_w->timer1 & 1 ) ){
			pl_w->timer1 |= 1 ;
			shl04_03_set( pl_w, 0, 0 ) ;
		}
		if( ( rnd & 2 ) && !( pl_w->timer1 & 2 ) ){
			pl_w->timer1 |= 2 ;
			shl04_03_set( pl_w, 0, 1 ) ;
		}
		if( ( rnd & 4 ) && !( pl_w->timer1 & 4 ) ){
			pl_w->timer1 |= 4 ;
			shl04_03_set( pl_w, 0, 2 ) ;
		}
		if( ( rnd & 8 ) && !( pl_w->timer1 & 8 ) ){
			pl_w->timer1 |= 8 ;
			shl04_03_set( pl_w, 0, 3 ) ;
		}
	} else {
		if( ( pl_w->pl_now_lvbt & KEY_LP ) && !( pl_w->timer1 & 1 ) ){
			pl_w->timer1 |= 1 ;
			shl04_03_set( pl_w, 0, 0 ) ;
		}
		if( ( pl_w->pl_now_lvbt & KEY_HP ) && !( pl_w->timer1 & 2 ) ){
			pl_w->timer1 |= 2 ;
			shl04_03_set( pl_w, 0, 1 ) ;
		}
		if( ( pl_w->pl_now_lvbt & KEY_LK ) && !( pl_w->timer1 & 4 ) ){
			pl_w->timer1 |= 4 ;
			shl04_03_set( pl_w, 0, 2 ) ;
		}
		if( ( pl_w->pl_now_lvbt & KEY_HK ) && !( pl_w->timer1 & 8 ) ){
			pl_w->timer1 |= 8 ;
			shl04_03_set( pl_w, 0, 3 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void cobras_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		if( pl_w->pl_stance == STANCE_AIR ) pl_jump_ret( pl_w ) ;
		else pl_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
/*			Steers with the stick, or drifts back to the		*/
/*			middle of the screen, and bobs at the top			*/
/*==============================================================*/
static void fly_move( PLAYER_WORK *pl_w )
{
	Float x, y ;

	if( pl_w->pl_new_lvbt & KEY_LEVER ){
		if( pl_w->pl_new_lvbt & KEY_BACK ){
			pl_w->spd.x += 1.6666666f ;
			if( pl_w->spd.x > DOT_X( 16.0f ) ) pl_w->spd.x = DOT_X( 16.0f ) ;
		}
		if( pl_w->pl_new_lvbt & KEY_FORWARD ){
			pl_w->spd.x -= 1.6666666f ;
			if( pl_w->spd.x < -DOT_X( 16.0f ) ) pl_w->spd.x = -DOT_X( 16.0f ) ;
		}
	} else {
		x = pl_w->pos.x - ( scr_work.scr_left + scr_work.scr_right ) / 2.0f ;
		if( x > 0.0f ) pl_w->spd.x -= 1.6666666f ;
		if( x < 0.0f ) pl_w->spd.x += 1.6666666f ;
	}
	pl_w->pos.x = ( scr_work.scr_left + scr_work.scr_right ) / 2.0f + pl_w->spd.x ;
	y = scr_work.scr_bottom + ( scr_work.scr_top - scr_work.scr_bottom ) / 8.0f - pl_w->pos.y
		+ nlSin( (int)( pl_w->timer1 * 65536.0f / 360.0f + 0.5f ) & 0xffff ) * 10.0f ;
	if( y > DOT_Y( 4.0f ) ) y = DOT_Y( 4.0f ) ;
	if( y < -DOT_Y( 4.0f ) ) y = -DOT_Y( 4.0f ) ;
	pl_w->pos.y += y ;
	if( ( pl_w->timer1 += 6 ) >= 360 ) pl_w->timer1 = 0 ;
}

static void illusion_init( PLAYER_WORK *pl_w );
static void illusion_start( PLAYER_WORK *pl_w );
static void illusion_rise( PLAYER_WORK *pl_w );
static void illusion_appear( PLAYER_WORK *pl_w );
static void illusion_fly( PLAYER_WORK *pl_w );
static void illusion_next( PLAYER_WORK *pl_w );
static void illusion_fall( PLAYER_WORK *pl_w );
static void illusion_land( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_illusion( PLAYER_WORK *pl_w )
{
	static void ( *illusion_jmp[])( PLAYER_WORK *pl_w ) = {
		illusion_init, illusion_start, illusion_rise, illusion_appear,
		illusion_fly, illusion_next, illusion_fall, illusion_land,
	} ;

	illusion_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void illusion_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->dir = 0 ;
	pl_w->timer1 = 0 ;
	pl_dir_set( pl_w ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = pl04_at_illusion ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 20 ) ;
}

/*--------------------------------------------------------------*/
static void illusion_start( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->spd.y = DOT_Y( 32.0f ) ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = -80.0f ;
		ofs.y = DOT_Y( 40.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void illusion_rise( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_prm ){
		pl_w->cg_prm = 0 ;
		pl_se_req( pl_w, 50 ) ;
	}
	pl_w->push_ng_timer = 3 ;
	if( pl_w->cg_type ){
		scr_excep_set( pl_w, 7, 7 ) ;
		pl_w->pos.y += pl_w->spd.y ;
		if( pl_w->pos.y > scr_work.scr_top ){
			pl_w->routine_2 ++ ;
			pl_w->timer0 = 32 ;
			pl_w->dir = 0xff ;
			pl_w->alpha = 0.0f ;
			pl_w->scale.x = 0.0f ;
			pl_w->scale.y = 0.0f ;
			pl_w->disp_dir = 0 ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			SCR_EXCEP_SET ;
			fly_move( pl_w ) ;
			pl_char_set( pl_w, Char_Super, 21 ) ;
			pl_voice_req( pl_w, 16 ) ;
			shl04_05_set( pl_w, 0 ) ;
			shl04_05_set( pl_w, 1 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void illusion_appear( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->push_ng_timer = 3 ;
	pl_w->alpha += 0.03125f ;
	pl_w->scale.x += 0.03125f ;
	pl_w->scale.y += 0.03125f ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 180 ;
		pl_w->alpha = 1.0f ;
		pl_w->scale.x = 1.0f ;
		pl_w->scale.y = 1.0f ;
	}
	pl_char_move( pl_w ) ;
	fly_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void illusion_fly( PLAYER_WORK *pl_w )
{
	static Uint8 dir_tbl[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 4, 8, 5, 6, 9, 10 } ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	fly_move( pl_w ) ;
	pl_w->dir = 0 ;
	pl_w->push_ng_timer = 3 ;
	if( pl_w->team_type == 5 || pl_w->cpu_flag || pl_w->team_type == 4 ){
		if( !( Sys->game_timer & 7 ) ){
			pl_w->dir = ( Rnd() & 3 ) + 1 ;
			pl_w->sub_type = dir_tbl[ Rnd() & 15 ] ;
		}
	} else if( pl_w->pl_now_lvbt & KEY_ATCK ){
		if( pl_w->pl_now_lvbt & KEY_LP ) pl_w->dir = 1 ;
		if( pl_w->pl_now_lvbt & KEY_HP ) pl_w->dir = 2 ;
		if( pl_w->pl_now_lvbt & KEY_LK ) pl_w->dir = 3 ;
		if( pl_w->pl_now_lvbt & KEY_HK ) pl_w->dir = 4 ;
		pl_w->sub_type = ( pl_w->pl_new_lvbt & KEY_LEVER ) >> 10 ;
	}
	if( !-- pl_w->timer0 || vs_sys_work.vs_st >= VS_END_WAIT ){
		pl_w->routine_2 ++ ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_dir_set( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 22 ) ;
	}
}

/*--------------------------------------------------------------*/
static void illusion_next( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void illusion_fall( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		scr_work.quake_level = 3 ;
		scr_work.quake_flag = 1 ;
		pl_se_req( pl_w, 47 ) ;
	}
}

/*--------------------------------------------------------------*/
static void illusion_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void coffins_init( PLAYER_WORK *pl_w );
static void coffins_start( PLAYER_WORK *pl_w );
static void coffins_shot( PLAYER_WORK *pl_w );
static void coffins_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_coffins( PLAYER_WORK *pl_w )
{
	static void ( *coffins_jmp[])( PLAYER_WORK *pl_w ) = {
		coffins_init, coffins_start, coffins_shot, coffins_end,
	} ;

	coffins_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void coffins_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->timer0 = 0 ;
	pl_dir_set( pl_w ) ;
	if( pl_w->pl_stance == STANCE_AIR ){
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_w->atck_no = pl04_at_coffins ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 8 ) ;
	} else {
		pl_w->pl_stance = STANCE_STAND ;
		pl_sp_ef_set( pl_w ) ;
		pl_w->atck_no = pl04_at_coffins ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 6 ) ;
	}
}

/*--------------------------------------------------------------*/
static void coffins_start( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = -DOT_X( 32.0f ) ;
		ofs.y = DOT_Y( 176.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void coffins_shot( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_prm ){
		pl_w->cg_prm = 0 ;
		pl_se_req( pl_w, 48 ) ;
	}
	if( pl_w->cg_type ){
		if( ++ pl_w->timer0 >= 8 ){
			pl_w->routine_2 ++ ;
			pl_w->delay_flag = 0 ;
			pl_w->delay_timer = 0 ;
			pl_w->hyper_bg = 0 ;
			pl_w->hyper_bg_tm = 0 ;
			if( pl_w->pl_stance != STANCE_AIR ) pl_char_set( pl_w, Char_Super, 7 ) ;
			else pl_char_set( pl_w, Char_Super, 9 ) ;
		} else if( ew = shl04_00_set( pl_w, 1 ) ){
			ew->pos.x = DOT_X( pl04_coffins_x_tbl[pl_w->timer0] ) ;
			if( !pl_w->disp_dir ) ew->pos.x = -ew->pos.x ;
			ew->pos.x += pl_w->pos.x ;
		}
	}
}

/*--------------------------------------------------------------*/
static void coffins_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		if( pl_w->pl_stance == STANCE_AIR ) pl_jump_ret( pl_w ) ;
		else pl_stand_ret( pl_w ) ;
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
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_se_req( pl_w, SE_SWING_H ) ;
	pl_w->atck_no = pl04_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 2 ) ;
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
		ofs.x = 0.0f ;
		ofs.y = DOT_Y( 64.0f ) ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
	if( pl_w->cg_prm ){
		pl_w->cg_prm = 0 ;
		pl_se_req( pl_w, SE_SWING_H ) ;
	}
}

/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Extra, 3 ) ;
	} else {
		if( pl_w->cg_prm ){
			pl_w->cg_prm = 0 ;
			pl_se_req( pl_w, 48 ) ;
		}
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	}
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_move( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A kick that leaps off the wall						*/
/****************************************************************/
void pl04_t_kick( PLAYER_WORK *pl_w )
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
	pl_w->atck_no = pl04_at_t_kick ;
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
	}
}

/****************************************************************/
PLAYER_WORK *pl04_catch_ck( PLAYER_WORK *pl_w )
{
	return 0 ;
}

/****************************************************************/
void pl04_catch_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_w->team_type != 8 && pl_w->team_type != 3 ) scr_excep_set( pl_w, 1, 1 ) ;
	catch_gauge_add( pl_w ) ;
	pl_w->timer0 = 12 ;
	pl_w->catch_flag = 1 ;
	ofs.x = pl_w->spd.z ;
	ofs.y = 0.6110491f ;
	nage_hit_set( pl_w, &ofs ) ;
	if( pl_w->pl_stance != STANCE_AIR ){
		if( pl_w->team_type != 8 ) pl_char_set( pl_w, Char_Special, 11 ) ;
		else pl_char_set( pl_w, Char_Special, 21 ) ;
	} else {
		pl_char_set( pl_w, Char_Special, 16 ) ;
	}
}

static void catch_wait( PLAYER_WORK *pl_w );
static void catch_pull( PLAYER_WORK *pl_w );
static void catch_hit( PLAYER_WORK *pl_w );
static void catch_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void pl04_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		catch_wait, catch_pull, catch_hit, catch_end,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void catch_wait( PLAYER_WORK *pl_w )
{
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 6 ;
		pl_w->acc.z = -pl_w->spd.z / 8.0f ;
	}
}

/*--------------------------------------------------------------*/
static void catch_pull( PLAYER_WORK *pl_w )
{
	pl_w->spd.z += pl_w->acc.z ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 4 ;
	}
}

/*--------------------------------------------------------------*/
static void catch_hit( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	Sint8 flag ;

	em = pl_w->catch_w ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_prm ){
		pl_w->cg_prm = 0 ;
		pl_se_req( pl_w, 23 ) ;
	}
	if( flag = pl_w->cg_type ){
		if( pl_w->pl_stance != STANCE_AIR ) em->atck_no = pl04_at_throw_hit ;
		else em->atck_no = pl04_at_air_throw_hit ;
		catch_damage( pl_w ) ;
		pl_w->cg_type = 0 ;
		if( !-- pl_w->timer0 ){
			pl_w->routine_2 ++ ;
			pls04_00_set( pl_w, 4, 0 ) ;
			if( flag < 0 ) pl_w->disp_dir ^= 1 ;
			if( pl_w->pl_stance != STANCE_AIR ) pl_w->char_no.b.no = 12 ;
			else pl_w->char_no.b.no = 17 ;
			pl_char_set( pl_w, Char_Special, pl_w->char_no.b.no ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void catch_end( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	em = pl_w->catch_w ;
	if( pl_w->pl_stance != STANCE_AIR ){
		if( pl_char_move( pl_w ) < 0 ){
			stop_stand_ret( pl_w ) ;
			return ;
		}
	} else {
		if( pl_char_move( pl_w ) < 0 ){
			pl_jump_ret( pl_w ) ;
			return ;
		}
	}
	if( ( pl_w->cg_type & 0x3f ) == 2 ){
		pl_w->cg_type &= 0xc0 ;
		em->dm_hit_w = pl_w ;
		SCR_EXCEP_SET ;
		em->pl_dir = pl_w->pl_dir ^ 1 ;
		if( pl_w->pl_stance != STANCE_AIR ){
			em->catch_type = 2 ;
			em->atck_no = pl04_at_throw ;
		} else {
			em->catch_type = 1 ;
			em->atck_no = pl04_at_air_throw ;
			em->pl_stance = STANCE_AIR ;
		}
		catch_se_req( pl_w ) ;
	}
}

/****************************************************************/
/*			Holding the opponent								*/
/****************************************************************/
void pl04_hold( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	em = pl_w->catch_w ;
	pl_w->disp_flag = 1 ;
	if( LO_BYTE( em->cg_rival ) == 2 ) pl_w->disp_flag = 0 ;
	catch_pos_set( em, pl_w ) ;
	if( HI_BYTE( em->cg_rival ) == 36 ) pl_w->pos.x += em->spd.z ;
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void pl04_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl04_sp_cobras ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl04_sp_illusion ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl04_sp_coffins ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl04_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl04_sp_cobras ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl04_sp_illusion ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl04_sp_coffins ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl04_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl04_sp_cobra ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl04_sp_mummy ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl04_sp_coffin ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void pl04_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl04_sp_cobra ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl04_sp_mummy ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl04_sp_coffin ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl04.c" End of File	*/

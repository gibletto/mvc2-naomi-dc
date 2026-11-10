/* Servbot (character 3a): command checks, normal attacks, appearance, win poses, special moves
   and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl3a_def.h"
#include "pl10_def.h"

void pl3a_main( PLAYER_WORK *pl_w );
void pl3a_com_ck( PLAYER_WORK *pl_w );
void pl3a_atck_set( PLAYER_WORK *pl_w );
void pl3a_atck( PLAYER_WORK *pl_w );
void pl3a_dash( PLAYER_WORK *pl_w );
void pl3a_noki( PLAYER_WORK *pl_w );
void pl3a_appear( PLAYER_WORK *pl_w );
void pl3a_win( PLAYER_WORK *pl_w );
void pl3a_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl3a_catch_ck( PLAYER_WORK *pl_w );
void pl3a_catch_set( PLAYER_WORK *pl_w );
void pl3a_catch( PLAYER_WORK *pl_w );
void pl3a_hold( PLAYER_WORK *pl_w );
int pl3a_delay_ck( PLAYER_WORK *pl_w );
void pl3a_t_kick( PLAYER_WORK *pl_w );
void pl3a_upper( PLAYER_WORK *pl_w );
void pl3a_sp_unison( PLAYER_WORK *pl_w );
void pl3a_sp_gattai( PLAYER_WORK *pl_w );
void pl3a_sp_assist( PLAYER_WORK *pl_w );
void pl3a_sp_zero( PLAYER_WORK *pl_w );
void pl3a_ko_set( PLAYER_WORK *pl_w );
void pl3a_stand_ret( PLAYER_WORK *pl_w );

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

/* the tables by button and strength */
static Sint8 *nml_tbl[6] = { lp_tbl, mp_tbl, hp_tbl, lk_tbl, mk_tbl, hk_tbl } ;
static Sint8 *jump_tbl[12] = {
	j_lp_tbl, j_mp_tbl, j_hp_tbl, j_lk_tbl, j_mk_tbl, j_hk_tbl,
	sj_lp_tbl, sj_mp_tbl, sj_hp_tbl, sj_lk_tbl, sj_mk_tbl, sj_hk_tbl,
} ;

/* commands: time or steps, kind, the special move, then the stick and buttons */
static Uint16 com_lunch[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl3a_sp_lunch, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Lunch Rush (hyper) */
static Uint16 com_king[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl3a_sp_king, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* King Kobun (hyper) */
static Uint16 com_engo[] = { 3, COM_GROUND, pl3a_sp_engo, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Engo Kougeki, punch */
static Uint16 com_fire[] = { 3, 0, pl3a_sp_fire, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Kobun Fire */
static Uint16 com_bull[] = { 3, COM_GROUND, pl3a_sp_bull, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Bull Mover */
static Uint16 com_engo_k[] = { 3, COM_GROUND, pl3a_sp_engo_k, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Engo Kougeki, kick */
static Uint16 com_plane[] = { 3, COM_GROUND | COM_SW_NOW, pl3a_sp_plane, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_START } ;	/* Paper Planes */

static void ( *pl3a_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl3a_main,
	pl3a_com_ck,
	pl3a_atck_set,
	pl3a_atck,
	pl3a_dash,
	pl3a_noki,
	pl3a_appear,
	pl3a_win,
	pl3a_sp_atck,
	( void ( * )( PLAYER_WORK *pl_w ) )pl3a_catch_ck,	/* returns a value */
	pl3a_catch_set,
	pl3a_catch,
	pl3a_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl3a_delay_ck,
	pl3a_t_kick,
	pl3a_upper,
	pl3a_sp_unison,
	pl3a_sp_gattai,
	pl3a_sp_assist,
	pl3a_sp_zero,
	0, 0, 0, 0,
	pl3a_ko_set,
} ;

static int fire_ck( PLAYER_WORK *pl_w );
static int engo_k_ck( PLAYER_WORK *pl_w );
static int bull_ck( PLAYER_WORK *pl_w );
static int engo_ck( PLAYER_WORK *pl_w );
static int lunch_ck( PLAYER_WORK *pl_w );
static int king_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static int plane_ck( PLAYER_WORK *pl_w );
static int chouhatsu_ck( PLAYER_WORK *pl_w );
static int lunch_delay_ck( PLAYER_WORK *pl_w );
static int king_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl3a_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl3a_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl3a_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( lunch_ck( pl_w ) ) return ;
	if( king_ck( pl_w ) ) return ;
	if( plane_ck( pl_w ) ) return ;
	if( fire_ck( pl_w ) ) return ;
	if( bull_ck( pl_w ) ) return ;
	if( engo_k_ck( pl_w ) ) return ;
	if( engo_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
/*			In the air, once a jump unless super jumping		*/
/*==============================================================*/
static int fire_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_fire, &pl_w->com_w0 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl3a_sp_fire ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			Not while the last shot is still counting down		*/
/*==============================================================*/
static int engo_k_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_engo_k, &pl_w->com_w4 ) ) return 0 ;
	if( pl_w->tlocal[TM_SHOT] > 0 ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w4 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl3a_sp_engo_k ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int bull_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_bull, &pl_w->com_w5 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w5 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl3a_sp_bull ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int engo_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_engo, &pl_w->com_w1 ) ) return 0 ;
	if( pl_w->tlocal[TM_SHOT] > 0 ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl3a_sp_engo ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int lunch_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_lunch, &pl_w->com_w2 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( pl_w->tlocal[TM_SHOT] > 0 ) return 0 ;
	pl_w->disp_flag = 1 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl3a_sp_lunch ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static int king_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_king, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->disp_flag = 1 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl3a_sp_king ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->armor_flag = ARMOR_HEAVY ;
	pl_w->obj_flag = 10 ;
	return 1 ;
}

/*==============================================================*/
static int snapback_ck( PLAYER_WORK *pl_w )
{
	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl3a_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static int plane_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_plane, &pl_w->com_w6 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w6 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl3a_sp_plane ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			The tackle instead while its time lasts				*/
/*==============================================================*/
static int chouhatsu_ck( PLAYER_WORK *pl_w )
{
	Uint8 sp ;

	if( !pl_chouhatsu_ck( pl_w, 4 ) ) return 0 ;
	sp = pl3a_sp_chouhatsu ;
	if( pl_w->tlocal[TM_TACKLE] > 0 ){
		sp = pl3a_sp_tackle ;
		pl_w->tlocal[TM_TACKLE] = 0 ;
	}
	pl_w->pl_sp_type = sp ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl3a_delay_ck( PLAYER_WORK *pl_w )
{
	if( lunch_delay_ck( pl_w ) || king_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static int lunch_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_lunch, &pl_w->com_w2 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->disp_flag = 1 ;
	pl_w->delay_sp_type = pl3a_sp_lunch ;
	return 1 ;
}

/*==============================================================*/
static int king_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_king, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->disp_flag = 1 ;
	pl_w->delay_sp_type = pl3a_sp_king ;
	pl_w->armor_flag = ARMOR_HEAVY ;
	pl_w->obj_flag = 60 ;
	return 1 ;
}

/****************************************************************/
/*			Counts down the timers, and opens sp_tackle after a		*/
/*			certain hit from Tron Bonne in the same colour		*/
/****************************************************************/
void pl3a_main( PLAYER_WORK *pl_w )
{
	if( pl_w->tlocal[TM_SHOT] > 0 ) pl_w->tlocal[TM_SHOT] -- ;
	if( !pl_w->hit_stop && pl_w->obj_flag > 0 && ( pl_w->obj_flag = pl_w->obj_flag - 1 ) <= 0 ) pl_w->armor_flag = 0 ;
	if( pl_w->vital_new < 32 && !pl_w->work1 && pls3a_02_set( pl_w, 4 ) ) pl_w->work1 = 1 ;
	if( pl_w->tlocal[TM_TACKLE] > 0 ) pl_w->tlocal[TM_TACKLE] -- ;
	if( pl_w->target_w->pl_type == PL_TRON ){
		if( pl_w->target_w->pl_color == pl_w->pl_color && pl_w->routine_1 == PL_NORM
		 && pl_w->act_st == N_SJUMP_AIR && pl_w->old_act_st == N_CAUGHT && pl_w->dm_atck_no == pl10_at_beacon ) pl_w->tlocal[TM_TACKLE] = 120 ;
	}
}

/* each attack's number, strength, animation and group */
static Uint8 nml_atck_no[12] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 } ;
static Uint8 nml_level[3] = { 0, 1, 2 } ;
static Uint8 nml_char_no[12] = { 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2 } ;
static Uint8 nml_char_grp[12] = { Char_Stand_P, Char_Stand_P, Char_Stand_P, Char_Stand_K, Char_Stand_K, Char_Stand_K, Char_Crouch_P, Char_Crouch_P, Char_Crouch_P, Char_Crouch_K, Char_Crouch_K, Char_Crouch_K } ;
static Uint8 nml_unk[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 } ;
static Uint8 jump_atck_no[6] = { 12, 13, 14, 15, 16, 17 } ;
static Uint8 jump_level[3] = { 0, 1, 2 } ;
static Uint8 jump_char_no[6] = { 0, 1, 2, 0, 1, 2 } ;
static Uint8 jump_char_grp[6] = { Char_Jump_P, Char_Jump_P, Char_Jump_P, Char_Jump_K, Char_Jump_K, Char_Jump_K } ;

/*==============================================================*/
/*			Normal attacks										*/
/*==============================================================*/
static void nml_atck( PLAYER_WORK *pl_w )
{
	Uint8 no ;

	pl_brake_set( pl_w ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	no = pl_w->nml_no ;
	if( pl_w->pl_stance == STANCE_CROUCH ) no += 6 ;
	if( pl_w->atck_pk ) no += 3 ;
	PL3A_LW( pl_w )->unk_7c = nml_unk[no] ;
	pl_w->chain_tbl = nml_tbl[no % 6] ;
	pl_w->nml_level = nml_level[pl_w->nml_no] ;
	pl_w->atck_no = nml_atck_no[no] ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, nml_char_grp[no], nml_char_no[no] ) ;
	if( pl_w->cg_type ){
		shl3a_03_set( pl_w, pl_w->cg_type - 1 ) ;
		pl_w->cg_type = 0 ;
	}
	if( pl_w->cg_prm ){
		shl3a_03_set( pl_w, pl_w->cg_prm - 1 ) ;
		pl_w->cg_prm = 0 ;
	}
}

/*==============================================================*/
static void jump_atck( PLAYER_WORK *pl_w )
{
	Uint8 no ;

	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	no = pl_w->nml_no ;
	if( pl_w->atck_pk ) no += 3 ;
	pl_w->nml_level = jump_level[pl_w->nml_no] ;
	pl_w->atck_no = jump_atck_no[no] ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, jump_char_grp[no], jump_char_no[no] ) ;
	if( pl_w->sjump_flag ) no += 6 ;
	pl_w->chain_tbl = jump_tbl[no] ;
	if( !pl_w->atck_pk ){
		if( pl_w->jump_atk_cnt & JATK_P_MASK ) pl_w->jump_atk_cnt -= 0x01 ;
	} else {
		if( pl_w->jump_atk_cnt & JATK_K_MASK ) pl_w->jump_atk_cnt -= 0x10 ;
	}
}

/*==============================================================*/
/*			In a jump only while that button's attacks are		*/
/*			left												*/
/*==============================================================*/
static void jump_atck_ck( PLAYER_WORK *pl_w )
{
	if( !pl_w->atck_pk ){
		if( pl_w->jump_atk_cnt & JATK_P_MASK ) jump_atck( pl_w ) ;
	} else if( pl_w->jump_atk_cnt & JATK_K_MASK ) jump_atck( pl_w ) ;
}

/****************************************************************/
void pl3a_atck_set( PLAYER_WORK *pl_w )
{
	static void ( *atck_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_atck, jump_atck_ck, jump_atck, nml_atck,
	} ;

	atck_set_jmp[pl_w->atck_type]( pl_w ) ;
}

/*==============================================================*/
static void jump_move( PLAYER_WORK *pl_w )
{
	pl_jump_move( pl_w ) ;
	pl_sjump_top_ck( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_jump_ret( pl_w ) ;
		return ;
	}
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
/*			Steps forward while the animation says so			*/
/*==============================================================*/
static void nml_move( PLAYER_WORK *pl_w )
{
	Float spd ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_brake_stop( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ) pl3a_stand_ret( pl_w ) ;
	else {
		if( pl_w->cg_type ){
			shl3a_03_set( pl_w, pl_w->cg_type - 1 ) ;
			pl_w->cg_type = 0 ;
		}
		if( pl_w->cg_flag1 ){
			spd = -DOT_X( 4.0f ) ;
			if( pl_w->disp_dir ) spd = DOT_X( 4.0f ) ;
			pl_w->pos.x += spd ;
		}
	}
}

/*==============================================================*/
static void nml_move_dash( PLAYER_WORK *pl_w )
{
	pl_smoke_set( pl_w ) ;
	nml_move( pl_w ) ;
}

/****************************************************************/
void pl3a_atck( PLAYER_WORK *pl_w )
{
	static void ( *atck_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_move, jump_move, sjump_move, nml_move_dash,
	} ;

	atck_jmp[pl_w->atck_type]( pl_w ) ;
}

/****************************************************************/
/*			The launcher										*/
/****************************************************************/
void pl3a_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_brake_set( pl_w ) ;
		pl_gauge_add( pl_w, 5 ) ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->atck_no = pl3a_at_upper ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, 1 ) ;
	}
	if( pl_w->atck_type == 3 ) pl_smoke_set( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_brake_stop( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ) pl3a_stand_ret( pl_w ) ;
	else if( pl_w->cg_type ){
		shl3a_03_set( pl_w, pl_w->cg_type - 1 ) ;
		pl_w->cg_type = 0 ;
	}
}

/****************************************************************/
void pl3a_dash( PLAYER_WORK *pl_w )
{
	Float spd ;

	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 30 ;
		spd = -DOT_X( 6.5f ) ;
		if( pl_w->pl_dir ) spd = DOT_X( 6.5f ) ;
		pl_w->spd.x = spd ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
	} else {
		pl_char_move( pl_w ) ;
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( !-- pl_w->timer0 ) pl3a_stand_ret( pl_w ) ;
	}
}

/****************************************************************/
/*			The back dash										*/
/****************************************************************/
void pl3a_noki( PLAYER_WORK *pl_w )
{
	Float spd ;

	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		spd = DOT_X( 6.5f ) ;
		if( pl_w->disp_dir ) spd = -DOT_X( 6.5f ) ;
		pl_w->spd.x = spd ;
		pl_w->acc.x = 0.0f ;
		pl_w->timer0 = 30 ;
	}
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( !-- pl_w->timer0 ) pl3a_stand_ret( pl_w ) ;
}

static void appear_init( PLAYER_WORK *pl_w );
static void appear_fall( PLAYER_WORK *pl_w );
static void appear_end( PLAYER_WORK *pl_w );

/****************************************************************/
void pl3a_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_init, appear_fall, appear_end,
	} ;

	appear_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			Walks in, or drops from above; against another		*/
/*			Servbot the second player drops						*/
/*==============================================================*/
static void appear_init( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	int no ;

	pl_w->disp_flag = 0 ;
	if( vs_sys_work.vs_st < VS_APPEAR ) return ;
	pl_w->disp_flag = 1 ;
	em = pl_w->target_w ;
	no = Rnd() & 1 ;
	if( em->pl_type == PL_SERVBOT ) no = pl_w->pl_no ;
	if( !no ){
		pl_w->routine_2 = 2 ;
		pl_w->pl_stance = STANCE_STAND ;
		pls3a_00_set( pl_w, 0 ) ;
	} else {
		pl_w->routine_2 = 1 ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->pos.y += DOT_Y( 256.0f ) ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = -0.4017857f ;
	}
	pl_char_set( pl_w, Char_Appear, no ) ;
}

/*--------------------------------------------------------------*/
static void appear_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->pos.y = pl_w->ground_y ;
		scr_work.quake_level = 1 ;
		scr_work.quake_flag = 1 ;
		pl_se_req( pl_w, 44 ) ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Appear, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_w->routine_1 ++ ;
}

static void win_time_ko( PLAYER_WORK *pl_w );
static void lose_time_ko( PLAYER_WORK *pl_w );

/****************************************************************/
void pl3a_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_time_ko, lose_time_ko, win_time_ko, lose_time_ko, lose_time_ko,
	} ;

	if( pl_win_end_ck( pl_w ) ){
		scr_excep_clr() ;
		pl3a_stand_ret( pl_w ) ;
	} else win_jmp[pl_w->type]( pl_w ) ;
}

/*==============================================================*/
/*			The first pose, or a walk that turns at the			*/
/*			screen's edges										*/
/*==============================================================*/
static void win_time_ko( PLAYER_WORK *pl_w )
{
	int no ;
	int x ;

	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		scr_excep_set( pl_w, 1, 0 ) ;
		no = Rnd() & 1 ;
		if( !vs_sys_work.vs_end_flag ) no = 0 ;
		pl_w->routine_3 = no ;
		pl_char_set( pl_w, Char_Win, no ) ;
	} else {
		pl_char_move( pl_w ) ;
		if( pl_w->routine_3 ){
			pl_w->push_ng_timer = 2 ;
			pl_w->wall_ng_timer = 2 ;
			if( pl_w->cg_type ){
				pl_w->cg_type = 0 ;
				shl3a_02_set( pl_w, 0 ) ;
			}
			if( pl_w->cg_prm ){
				x = 10 ;
				if( !pl_w->disp_dir ) x = -10 ;
				pl_w->pos.x += x ;
			}
			if( !scr_in_x_ck( pl_w ) ){
				pl_w->disp_dir ^= 1 ;
				no = Rnd() & 1 ;
				pl_char_set( pl_w, Char_Win, no + 3 ) ;
				shl3a_02_set( pl_w, no ) ;
			}
		}
	}
}

/*==============================================================*/
static void lose_time_ko( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, 2 ) ;
	} else pl_char_move( pl_w ) ;
}

static void sp_fire( PLAYER_WORK *pl_w );
static void sp_engo( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );
static void sp_lunch( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_king( PLAYER_WORK *pl_w );
static void sp_rking( PLAYER_WORK *pl_w );
static void sp_plane( PLAYER_WORK *pl_w );
static void sp_bull( PLAYER_WORK *pl_w );
static void sp_tackle( PLAYER_WORK *pl_w );
static void sp_lunch_drop( PLAYER_WORK *pl_w );
static void sp_lunch_burst( PLAYER_WORK *pl_w );

/****************************************************************/
/*			By the partner type picked, then the move; sp_lunch	*/
/*			is different for each								*/
/****************************************************************/
void pl3a_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *pl_sp_tbl[3][11])( PLAYER_WORK *pl_w ) = {
		{ sp_fire, sp_engo, sp_snapback, sp_lunch, sp_chouhatsu, sp_king, sp_rking, sp_engo, sp_plane, sp_bull, sp_tackle },
		{ sp_fire, sp_engo, sp_snapback, sp_lunch_drop, sp_chouhatsu, sp_king, sp_rking, sp_engo, sp_plane, sp_bull, sp_tackle },
		{ sp_fire, sp_engo, sp_snapback, sp_lunch_burst, sp_chouhatsu, sp_king, sp_rking, sp_engo, sp_plane, sp_bull, sp_tackle },
	} ;

	pl_sp_tbl[pl_w->pl_personal][pl_w->pl_sp_type]( pl_w ) ;
}

static void fire_init( PLAYER_WORK *pl_w );
static void fire_move( PLAYER_WORK *pl_w );
static void fire_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_fire( PLAYER_WORK *pl_w )
{
	static void ( *fire_jmp[])( PLAYER_WORK *pl_w ) = {
		fire_init, fire_move, fire_end,
	} ;

	fire_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void fire_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	if( pl_w->pl_stance != STANCE_AIR ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_sp_ef_set( pl_w ) ;
	} else {
		pl_w->spd.x /= 16.0f ;
		pl_w->spd.y /= 8.0f ;
		pl_w->acc.y /= 64.0f ;
		pl_w->acc.x = 0.0f ;
	}
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->timer0 = 1 ;
	pl_w->atck_no = pl3a_at_fire ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 3 ) ;
}

/*==============================================================*/
/*			Lets go when the button is let go, or when the		*/
/*			animation says										*/
/*==============================================================*/
static void fire_move( PLAYER_WORK *pl_w )
{
	int sw ;
	int no ;

	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->cg_prm ){
		sw = 0x200 ;
		if( pl_w->atck_level ) sw = 0x100 ;
		if( !( pl_w->pl_sw_new & sw ) ){
			no = pl_w->cg_flag1 ;
			pl_char_set( pl_w, Char_Special, pl_w->atck_level + 14 ) ;
			goto fire_next ;
		}
	}
	if( pl_w->cg_type ){
		no = pl_w->cg_flag1 ;
fire_next:
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		shl3a_00_set( pl_w, pl_w->atck_level, no ) ;
	}
	if( pl_w->pos.y < pl_w->ground_y && pl_w->pl_stance == STANCE_AIR ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_ef_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void fire_end( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_ef_set( pl_w ) ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		if( pl_w->pl_stance != STANCE_AIR ) pl3a_stand_ret( pl_w ) ;
		else pl_jump_ret( pl_w ) ;
	}
}

static void snapback_init( PLAYER_WORK *pl_w );
static void snapback_move( PLAYER_WORK *pl_w );
static void snapback_end( PLAYER_WORK *pl_w );

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
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_CROUCH ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = pl3a_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 0 ) ;
}

/*--------------------------------------------------------------*/
static void snapback_move( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		ofs.x = -DOT_X( 28.0f ) ;
		ofs.y = DOT_Y( 16.0f ) ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
}

/*--------------------------------------------------------------*/
static void snapback_end( PLAYER_WORK *pl_w )
{
	Float spd ;

	if( pl_char_move( pl_w ) < 0 ) pl3a_stand_ret( pl_w ) ;
	else if( pl_w->cg_flag1 ){
		spd = -DOT_X( 4.0f ) ;
		if( pl_w->disp_dir ) spd = DOT_X( 4.0f ) ;
		pl_w->pos.x += spd ;
	}
}

static void lunch_init( PLAYER_WORK *pl_w );
static void lunch_start( PLAYER_WORK *pl_w );
static void lunch_move( PLAYER_WORK *pl_w );
static void lunch_wait( PLAYER_WORK *pl_w );
static void lunch_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo										*/
/*==============================================================*/
static void sp_lunch( PLAYER_WORK *pl_w )
{
	static void ( *lunch_jmp[])( PLAYER_WORK *pl_w ) = {
		lunch_init, lunch_start, lunch_move, lunch_wait, lunch_end,
	} ;

	lunch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void lunch_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	PL3A_LW( pl_w )->piece_cnt.w[0] = 0x04050405 ;
	PL3A_LW( pl_w )->piece_cnt.w[1] = 0x01020405 ;
	PL3A_LW( pl_w )->piece_cnt.w[2] = 0x01020101 ;
	PL3A_LW( pl_w )->piece_cnt.w[3] = 0x00010202 ;
	pl_w->atck_no = pl3a_at_lunch ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 2 ) ;
}

/*--------------------------------------------------------------*/
static void lunch_start( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->timer0 = 240 ;
		ofs.x = 0.0f ;
		ofs.y = DOT_Y( 26.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void lunch_move( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( !pl_w->routine_3 ){
		pl_w->routine_3 ++ ;
		shl3a_01_line_set( pl_w ) ;
	}
	if( -- pl_w->timer0 <= 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 60 ;
		pl_char_set( pl_w, Char_Super, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void lunch_wait( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 <= 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Super, 4 ) ;
	}
}

/*--------------------------------------------------------------*/
static void lunch_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl3a_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Extra, 0 ) ;
	} else if( pl_char_move( pl_w ) < 0 ) pl3a_stand_ret( pl_w ) ;
}

static void king_init( PLAYER_WORK *pl_w );
static void king_start( PLAYER_WORK *pl_w );
static void king_next( PLAYER_WORK *pl_w );
static void king_loop( PLAYER_WORK *pl_w );
static void king_end( PLAYER_WORK *pl_w );
static void king_stand( PLAYER_WORK *pl_w );
static void king_walk( PLAYER_WORK *pl_w );
static void king_dash( PLAYER_WORK *pl_w );
static void king_punch( PLAYER_WORK *pl_w );
static int turn_ck( PLAYER_WORK *pl_w );
static int walk_turn( PLAYER_WORK *pl_w );
static int dash_ck( PLAYER_WORK *pl_w );
static int walk_ck( PLAYER_WORK *pl_w );
static int walk_end_ck( PLAYER_WORK *pl_w );
static void face_em( PLAYER_WORK *pl_w );
static void king_stand_set( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo steered with the stick				*/
/*==============================================================*/
static void sp_king( PLAYER_WORK *pl_w )
{
	static void ( *king_jmp[])( PLAYER_WORK *pl_w ) = {
		king_init, king_start, king_next, king_loop, king_end,
	} ;

	pl_w->nml_catch_ng_timer = 2 ;
	pl_w->obj_flag = 3 ;
	combo_reset( pl_w ) ;
	king_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void king_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->timer0 = pl_w->pl_color * 5 ;
	pl_w->atck_no = pl3a_at_king ;
	HIT_RESET( pl_w ) ;
	pls3a_03_set( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 5 ) ;
}

/*--------------------------------------------------------------*/
static void king_start( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_prm ){
		if( pl_w->cg_prm < 0 ) pl_color_set( pl_w, 1 ) ;
		else ef_color_set( pl_w, 0, pl_w->timer0 + pl_w->cg_flag1, 1 ) ;
	}
	if( pl_w->cg_type ){
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->tlocal[TM_KING] = 600 ;
		pl_color_set( pl_w, 1 ) ;
		ofs.x = 0.0f ;
		ofs.y = DOT_Y( 64.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
		Timer_Gauge_Set( pl_w, &pl_w->tlocal[TM_KING], 6 ) ;
	}
}

/*--------------------------------------------------------------*/
static void king_next( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->type = 0 ;
	pl_w->pl_stop_now = 0 ;
	king_loop( pl_w ) ;
}

/*==============================================================*/
/*			Until the time runs out or the round ends			*/
/*==============================================================*/
static void king_loop( PLAYER_WORK *pl_w )
{
	static void ( *king_type_jmp[])( PLAYER_WORK *pl_w ) = {
		king_stand, king_walk, king_dash, king_punch,
	} ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( vs_sys_work.time_over || vs_sys_work.vs_st == VS_END_WAIT ) goto king_kill ;
	if( -- pl_w->tlocal[TM_KING] <= 0 ){
king_kill:
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Super, 11 ) ;
	} else king_type_jmp[pl_w->type]( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void king_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl3a_stand_ret( pl_w ) ;
	else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pls3a_03_set2( pl_w ) ;
	}
}

/*==============================================================*/
static void king_stand( PLAYER_WORK *pl_w )
{
	if( dash_ck( pl_w ) ) return ;
	if( walk_ck( pl_w ) ) return ;
	if( turn_ck( pl_w ) ) return ;
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void king_walk( PLAYER_WORK *pl_w )
{
	if( dash_ck( pl_w ) ) return ;
	if( walk_end_ck( pl_w ) ) return ;
	walk_turn( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_flag1 ){
		pl_w->cg_flag1 = 0 ;
		scr_work.quake_level = 1 ;
		scr_work.quake_flag = 1 ;
	}
	if( !pl_w->cg_type ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
	}
}

/*==============================================================*/
/*			The dash, which can be followed by a second one		*/
/*			while time is left									*/
/*==============================================================*/
static void king_dash( PLAYER_WORK *pl_w )
{
	VEC3 pos ;
	Uint8 dir ;

	if( pl_char_move( pl_w ) < 0 ) king_stand_set( pl_w ) ;
	else{
		if( pl_w->cg_type && pl_w->tlocal[TM_KING] > 0 ){
			if( !( pl_w->cpu_flag && pl_w->hit_flag ) ){
				if( !( pl_w->pl_sw_now & KEY_ATCK ) && !( pl_w->pl_stop_now & KEY_ATCK ) ) goto quake ;
			}
			pl_w->cg_type = 0 ;
			pl_w->pl_stop_now = 0 ;
			pl_w->tlocal[TM_KING] -= 30 ;
			face_em( pl_w ) ;
			dir = pl_w->disp_dir ;
			if( pl_w->pl_new_lvbt & KEY_FORWARD ) dir = 0 ;
			if( pl_w->pl_new_lvbt & KEY_BACK ) dir = 1 ;
			pl_w->disp_dir = pl_w->pl_dir = dir ;
			pl_w->atck_no = pl3a_at_king_dash ;
			HIT_RESET( pl_w ) ;
			pl_w->atck_attr |= ATR_DOWN_HIT_NG ;
			pl_char_set( pl_w, Char_Super, 13 ) ;
		}
	quake:
		if( pl_w->cg_flag1 ){
			pl_w->cg_flag1 = 0 ;
			scr_work.quake_level = 1 ;
			scr_work.quake_flag = 1 ;
			pos.x = -300.0f ;
			if( pl_w->disp_dir ) pos.x = -pos.x ;
			pos.x += pl_w->pos.x ;
			pos.y = pl_w->pos.y ;
			land_dust_set( &pos, -1 ) ;
		}
	}
}

/*==============================================================*/
static void king_punch( PLAYER_WORK *pl_w )
{
	if( dash_ck( pl_w ) ) return ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_char_move( pl_w ) < 0 ) king_stand_set( pl_w ) ;
	else if( pl_w->cg_flag1 ){
		pl_w->cg_flag1 = 0 ;
		pl_w->atck_no = pl3a_at_king_punch ;
		HIT_RESET( pl_w ) ;
	}
}

/*==============================================================*/
/*			Turns to face the opponent							*/
/*==============================================================*/
static int turn_ck( PLAYER_WORK *pl_w )
{
	int dir ;

	dir = 0 ;
	if( pl_w->pos.x < pl_w->target_w->pos.x ) dir = 1 ;
	if( pl_w->pl_dir != dir ){
		pl_w->type = 0 ;
		pl_w->disp_dir = dir ;
		pl_w->pl_dir = dir ;
		pl_char_set( pl_w, Char_Super, 6 ) ;
		return 1 ;
	}
	return 0 ;
}

/*==============================================================*/
/*			Turns while walking, then runs the new animation	*/
/*			up to the walk's frame								*/
/*==============================================================*/
static int walk_turn( PLAYER_WORK *pl_w )
{
	int dir ;
	int walk_ctr ;
	int walk_prm ;
	int no ;

	dir = 0 ;
	if( pl_w->pos.x < pl_w->target_w->pos.x ) dir = 1 ;
	if( pl_w->pl_dir != dir ){
		pl_w->disp_dir = dir ;
		pl_w->pl_dir = dir ;
		walk_ctr = pl_w->cg_ctr ;
		walk_prm = pl_w->cg_prm ;
		no = 7 ;
		if( pl_w->pl_sw_new & KEY_FORWARD ) no = 14 ;
		pl_char_set( pl_w, Char_Super, no ) ;
		for( ;; ){
			if( pl_w->cg_prm == walk_prm ){
				pl_w->cg_ctr = walk_ctr ;
				break ;
			}
			pl_w->cg_ctr = 1 ;
			pl_char_move( pl_w ) ;
		}
		return 1 ;
	}
	return 0 ;
}

/*==============================================================*/
/*			The dash, with the buttons or, for the computer,	*/
/*			by distance											*/
/*==============================================================*/
static int dash_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	int sw ;
	int lever ;
	Float d ;
	int dir ;

	if( pl_w->tlocal[TM_KING] > 0 ){
		sw = pl_w->pl_sw_now | pl_w->pl_stop_now ;
		lever = pl_w->pl_new_lvbt ;
		em = pl_w->target_w ;
		if( pl_w->cpu_flag ){
			d = em->pos.x - pl_w->pos.x ;
			sw = 0 ;
			if( d > 0.0f ) lever = KEY_BACK ;
			else {
				lever = KEY_FORWARD ;
				d = -d ;
			}
			if( !( d > DOT_X( 160.0f ) ) ) sw = KEY_LP ;
		}
		if( sw & KEY_ATCK ){
			pl_w->pl_stop_now = 0 ;
			pl_w->type = 2 ;
			pl_w->tlocal[TM_KING] -= 30 ;
			pl_w->spd.x = 0.0f ;
			face_em( pl_w ) ;
			dir = 0 ;
			if( pl_w->pos.x < em->pos.x ) dir = 1 ;
			if( lever & KEY_FORWARD ) dir = 0 ;
			if( lever & KEY_BACK ) dir = 1 ;
			pl_w->disp_dir = dir ;
			pl_w->atck_no = pl3a_at_king_dash ;
			HIT_RESET( pl_w ) ;
			pl_w->atck_attr |= ATR_DOWN_HIT_NG ;
			pl_char_set( pl_w, Char_Super, 10 ) ;
			return 1 ;
		}
	}
	return 0 ;
}

/*==============================================================*/
/*			Starts walking with the stick, or, for the			*/
/*			computer, by distance								*/
/*==============================================================*/
static int walk_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	int lever ;
	Float d ;
	Float spd ;
	int dir ;
	int no ;

	lever = pl_w->pl_new_lvbt ;
	if( pl_w->cpu_flag ){
		lever = 0 ;
		em = pl_w->target_w ;
		d = em->pos.x - pl_w->pos.x ;
		if( d > DOT_X( 160.0f ) ) lever = KEY_BACK ;
		if( -DOT_X( 160.0f ) > d ) lever = KEY_FORWARD ;
	}
	if( lever & ( KEY_FORWARD | KEY_BACK ) ){
		pl_w->type = 1 ;
		spd = 5.0f ;
		if( lever & KEY_FORWARD ) spd = -5.0f ;
		pl_w->spd.x = spd ;
		face_em( pl_w ) ;
		no = 7 ;
		dir = 0 ;
		if( pl_w->spd.x > 0.0f ) dir = 1 ;
		if( pl_w->disp_dir != dir ) no = 14 ;
		pl_char_set( pl_w, Char_Super, no ) ;
		return 1 ;
	}
	return 0 ;
}

/*==============================================================*/
/*			Stops walking when the stick is let go, or turns	*/
/*			the walk around										*/
/*==============================================================*/
static int walk_end_ck( PLAYER_WORK *pl_w )
{
	int lever ;
	int old ;
	Float spd ;
	int no ;

	lever = pl_w->pl_new_lvbt & ( KEY_FORWARD | KEY_BACK ) ;
	if( pl_w->cpu_flag ) lever = ( KEY_FORWARD | KEY_BACK ) ;
	if( !lever ){
		pl_w->type = 0 ;
		face_em( pl_w ) ;
		no = 12 ;
		if( pl_w->pl_sw_old & KEY_BACK ) no = 6 ;
		pl_char_set( pl_w, Char_Super, no ) ;
		return 1 ;
	}
	old = pl_w->pl_last_lvbt & ( KEY_FORWARD | KEY_BACK ) ;
	if( pl_w->cpu_flag ) old = ( KEY_FORWARD | KEY_BACK ) ;
	if( lever != old ){
		spd = 5.0f ;
		no = 7 ;
		if( pl_w->pl_sw_new & KEY_BACK ){
			spd = -5.0f ;
			no = 14 ;
		}
		pl_w->spd.x = spd ;
		pl_char_set( pl_w, Char_Super, no ) ;
	}
	return 0 ;
}

/*==============================================================*/
static void face_em( PLAYER_WORK *pl_w )
{
	Uint8 dir ;

	dir = 0 ;
	if( pl_w->pos.x < pl_w->target_w->pos.x ) dir = 1 ;
	pl_w->disp_dir = pl_w->pl_dir = dir ;
}

/*==============================================================*/
static void king_stand_set( PLAYER_WORK *pl_w )
{
	pl_w->type = 0 ;
	pl_w->pl_dir = pl_w->disp_dir ;
	pl_w->spd.x = 0.0f ;
	pl_char_set( pl_w, Char_Super, 12 ) ;
}

static void rking_init( PLAYER_WORK *pl_w );
static void rking_start( PLAYER_WORK *pl_w );
static void rking_set( PLAYER_WORK *pl_w );
static void rking_run( PLAYER_WORK *pl_w );
static void rking_stop( PLAYER_WORK *pl_w );
static void rking_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo that runs across the screen,			*/
/*			faster while the button is held						*/
/*==============================================================*/
static void sp_rking( PLAYER_WORK *pl_w )
{
	static void ( *rking_jmp[])( PLAYER_WORK *pl_w ) = {
		rking_init, rking_start, rking_set, rking_run, rking_stop, rking_end,
	} ;

	pl_w->nml_catch_ng_timer = 2 ;
	pl_w->obj_flag = 3 ;
	combo_reset( pl_w ) ;
	rking_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void rking_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->armor_flag = ARMOR_HEAVY ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->atck_no = pl3a_at_king_punch ;
	HIT_RESET( pl_w ) ;
	pls3a_03_set( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 5 ) ;
}

/*--------------------------------------------------------------*/
static void rking_start( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_prm ){
		if( pl_w->cg_prm < 0 ) pl_color_set( pl_w, 1 ) ;
		else ef_color_set( pl_w, 0, pl_w->timer0 + pl_w->cg_flag1, 1 ) ;
	}
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		ofs.x = 0.0f ;
		ofs.y = DOT_Y( 16.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*==============================================================*/
/*			Spd.y keeps the speed and acc.y the speed with		*/
/*			the button held										*/
/*==============================================================*/
static void rking_set( PLAYER_WORK *pl_w )
{
	Float spd ;

	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 120 ;
		spd = 10.0f ;
		if( !pl_w->disp_dir ) spd = -10.0f ;
		pl_w->spd.x = pl_w->spd.y = spd ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = spd * 2.0f ;
		pl_char_set( pl_w, Char_Super, 15 ) ;
		rking_run( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void rking_run( PLAYER_WORK *pl_w )
{
	Float spd ;

	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	spd = pl_w->spd.y ;
	if( sw_renda_ck( pl_w ) ){
		spd = pl_w->acc.y ;
		pl_char_move( pl_w ) ;
	}
	pl_w->spd.x = spd ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_w->cg_flag1 ){
		pl_w->cg_flag1 = 0 ;
		pl_w->atck_no = pl3a_at_king_punch ;
		HIT_RESET( pl_w ) ;
	}
	if( -- pl_w->timer0 == 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Super, 10 ) ;
		pl_w->atck_no = pl3a_at_rking_last ;
		HIT_RESET( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void rking_stop( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Super, 11 ) ;
	}
}

/*--------------------------------------------------------------*/
static void rking_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl3a_stand_ret( pl_w ) ;
	else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pls3a_03_set2( pl_w ) ;
	}
}

/*==============================================================*/
static void sp_engo( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_dir_set( pl_w ) ;
		pl_sp_ef_set( pl_w ) ;
		pl_gauge_add( pl_w, 5 ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->timer0 = 60 ;
		pl_char_set( pl_w, Char_Special, 5 ) ;
		if( pl_w->pl_sp_type == pl3a_sp_engo ){
			if( !pl_w->atck_level ) shl3a_01_piece_set( pl_w, 0 ) ;
			else shl3a_01_piece_set( pl_w, 7 ) ;
		} else {
			if( !pl_w->atck_level ) shl3a_01_piece_set( pl_w, 4 ) ;
			else shl3a_01_piece_set( pl_w, 3 ) ;
		}
		pl_w->unk_27b = 0 ;
		pl_w->cancel_timer = 16 ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		if( !pl_w->cg_type ) pl3a_stand_ret( pl_w ) ;
	} else if( -- pl_w->timer0 == 0 ) pl_char_set( pl_w, Char_Special, 6 ) ;
}

/*==============================================================*/
static void sp_plane( PLAYER_WORK *pl_w )
{
	Float spd ;

	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_dir_set( pl_w ) ;
		pl_sp_ef_set( pl_w ) ;
		pl_gauge_add( pl_w, 5 ) ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->atck_no = pl3a_at_plane ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 7 ) ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shl3a_02_set( pl_w, 3 ) ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		spd = -DOT_X( 16.0f ) ;
		if( pl_w->disp_dir ) spd = DOT_X( 16.0f ) ;
		pl_w->pos.x += spd ;
		pl3a_stand_ret( pl_w ) ;
	}
}

static void bull_init( PLAYER_WORK *pl_w );
static void bull_roll( PLAYER_WORK *pl_w );
static void bull_wait( PLAYER_WORK *pl_w );
static void bull_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A roll along the ground that hits several times		*/
/*==============================================================*/
static void sp_bull( PLAYER_WORK *pl_w )
{
	static void ( *bull_jmp[])( PLAYER_WORK *pl_w ) = {
		bull_init, bull_roll, bull_wait, bull_end,
	} ;

	pl_w->pos.y = pl_w->ground_y ;
	bull_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void bull_init( PLAYER_WORK *pl_w )
{
	static Float bull_spd[] = {
		DOT_X( 2.0f ), -0.052083332f,
		10.0f, -0.15625f,
	} ;
	static Uint8 bull_atck[] = { 110, 113 } ;
	static Uint8 bull_hits[] = { 3, 5 } ;
	int level ;
	int no ;

	pl_w->routine_2 ++ ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	level = pl_w->atck_level ;
	pl_w->atck_no = bull_atck[level] ;
	HIT_RESET( pl_w ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->timer1 = bull_hits[level] ;
	pl_w->dir = 0 ;
	level <<= 1 ;
	pl_w->spd.x = bull_spd[level] ;
	pl_w->acc.x = bull_spd[level + 1] ;
	no = 8 ;
	if( pls3a_01_set( pl_w, 0 ) ) pl_w->disp_flag = 0 ;
	else {
		pl_w->disp_flag = 1 ;
		no = 11 ;
	}
	pl_char_set( pl_w, Char_Special, no ) ;
}

/*--------------------------------------------------------------*/
static void bull_roll( PLAYER_WORK *pl_w )
{
	Float spd ;
	Sint8 hit ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ) return ;
	spd = pl_w->spd.x ;
	if( !pl_w->disp_dir ) spd = -spd ;
	pl_w->pos.x += spd ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_w->spd.x < 0.0f ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 60 ;
		pl_w->char_no.b.no ++ ;
		hit = pl_w->hit_flag ;
		if( hit && !( hit & 0x11 ) ){
			pl_w->routine_2 ++ ;
			pl_w->char_no.b.no ++ ;
		}
		pl_char_set( pl_w, Char_Special, pl_w->char_no.b.no ) ;
	}
	if( ( pl_w->dir += 32 ) == 0 ){
		if( -- pl_w->timer1 > 0 ){
			pl_w->atck_no = ++ pl_w->atck_no ;
			HIT_RESET( pl_w ) ;
		}
		pl_se_req( pl_w, 75 ) ;
	}
}

/*--------------------------------------------------------------*/
static void bull_wait( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 <= 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, ++ pl_w->char_no.b.no ) ;
	}
}

/*--------------------------------------------------------------*/
static void bull_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->disp_flag = 1 ;
		pl3a_stand_ret( pl_w ) ;
	}
}

static void lunch_burst_init( PLAYER_WORK *pl_w );
static void lunch_burst_start( PLAYER_WORK *pl_w );
static void lunch_burst_set( PLAYER_WORK *pl_w );
static void lunch_burst_loop( PLAYER_WORK *pl_w );
static void lunch_burst_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The gamma type's Lunch Rush: a burst of pieces		*/
/*==============================================================*/
static void sp_lunch_burst( PLAYER_WORK *pl_w )
{
	static void ( *lunch_burst_jmp[])( PLAYER_WORK *pl_w ) = {
		lunch_burst_init, lunch_burst_start, lunch_burst_set, lunch_burst_loop, lunch_burst_end,
	} ;

	lunch_burst_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void lunch_burst_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->timer0 = 10 ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->atck_no = pl3a_at_lunch_burst ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 16 ) ;
}

/*--------------------------------------------------------------*/
static void lunch_burst_start( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 <= 0 ){
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 120 ;
		pl_w->timer1 = 10 ;
		ofs.x = 0.0f ;
		ofs.y = DOT_Y( 26.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void lunch_burst_set( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_char_set( pl_w, Char_Super, 17 ) ;
	lunch_burst_loop( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void lunch_burst_loop( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->timer1 && -- pl_w->timer1 <= 0 ) shl3a_01_burst_set( pl_w ) ;
	if( -- pl_w->timer0 <= 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Super, 18 ) ;
	}
}

/*--------------------------------------------------------------*/
static void lunch_burst_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl3a_stand_ret( pl_w ) ;
}

static void lunch_drop_init( PLAYER_WORK *pl_w );
static void lunch_drop_start( PLAYER_WORK *pl_w );
static void lunch_drop_shot( PLAYER_WORK *pl_w );
static void lunch_drop_wait( PLAYER_WORK *pl_w );
static void lunch_drop_stop( PLAYER_WORK *pl_w );
static void lunch_drop_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The beta type's Lunch Rush: pieces from above		*/
/*==============================================================*/
static void sp_lunch_drop( PLAYER_WORK *pl_w )
{
	static void ( *lunch_drop_jmp[])( PLAYER_WORK *pl_w ) = {
		lunch_drop_init, lunch_drop_start, lunch_drop_shot, lunch_drop_wait, lunch_drop_stop, lunch_drop_end,
	} ;

	lunch_drop_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void lunch_drop_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->timer0 = 10 ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->atck_no = pl3a_at_lunch_drop ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 19 ) ;
}

/*--------------------------------------------------------------*/
static void lunch_drop_start( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 <= 0 ){
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->routine_2 ++ ;
		ofs.x = 0.0f ;
		ofs.y = DOT_Y( 26.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
		scr_excep_set( pl_w, 7, 7 ) ;
	}
}

/*--------------------------------------------------------------*/
static void lunch_drop_shot( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ef ;
	Float x ;

	pl_w->routine_2 ++ ;
	pl_w->timer0 = 30 ;
	pl_w->timer1 = 12 ;
	if( ef = shl3a_01_piece_set( pl_w, 18 ) ){
		x = DOT_X( 32.0f ) ;
		if( pl_w->disp_dir ) x = -DOT_X( 32.0f ) ;
		ef->pos.x = pl_w->pos.x + x ;
		ef->pos.y = scr_work.scr_top + DOT_Y( 32.0f ) ;
	}
	pl_char_set( pl_w, Char_Super, 20 ) ;
	lunch_drop_wait( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void lunch_drop_wait( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 <= 0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 30 ;
		pl_char_set( pl_w, Char_Super, 21 ) ;
	}
}

/*--------------------------------------------------------------*/
static void lunch_drop_stop( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 <= 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 180 ;
		pl_char_set( pl_w, Char_Super, 22 ) ;
	}
}

/*--------------------------------------------------------------*/
static void lunch_drop_end( PLAYER_WORK *pl_w )
{
	pl_w->timer0 -- ;
	if( pl_char_move( pl_w ) < 0 ){
		scr_excep_clr() ;
		pl3a_stand_ret( pl_w ) ;
	} else if( pl_w->cg_prm ){
		pl_w->cg_prm = 0 ;
		if( pl_w->timer0 > 0 ) pl_char_set( pl_w, Char_Super, 23 ) ;
	}
}

static void tackle_init( PLAYER_WORK *pl_w );
static void tackle_move( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A dash through the opponent, cut short when the		*/
/*			button is let go									*/
/*==============================================================*/
static void sp_tackle( PLAYER_WORK *pl_w )
{
	static void ( *tackle_jmp[])( PLAYER_WORK *pl_w ) = {
		tackle_init, tackle_move,
	} ;

	if( lever_sw_renda_ck( pl_w ) ){
		pl3a_stand_ret( pl_w ) ;
		return ;
	}
	tackle_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void tackle_init( PLAYER_WORK *pl_w )
{
	Float spd ;

	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_dir_set( pl_w ) ;
	spd = -DOT_X( 4.0f ) ;
	if( pl_w->disp_dir ) spd = DOT_X( 4.0f ) ;
	pl_w->spd.x = spd ;
	pl_w->atck_no = pl3a_at_fire ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 16 ) ;
	pls3a_02_set( pl_w, 3 ) ;
}

/*--------------------------------------------------------------*/
static void tackle_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( vs_sys_work.push_bit & 1 << pl_w->work_no ){
		pl3a_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->wall_flag & 1 << ( pl_w->pl_dir ^ 1 ) ) pl3a_stand_ret( pl_w ) ;
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_move( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The partner's attack as he comes in					*/
/****************************************************************/
void pl3a_t_kick( PLAYER_WORK *pl_w )
{
	static void ( *t_kick_jmp[])( PLAYER_WORK *pl_w ) = {
		t_kick_init, t_kick_move, t_kick_land,
	} ;

	t_kick_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void t_kick_init( PLAYER_WORK *pl_w )
{
	Float spd ;

	pl_w->routine_2 ++ ;
	pl_color_set( pl_w, 0 ) ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->pos.y += DOT_Y( 64.0f ) ;
	spd = 30.0f ;
	if( !pl_w->pl_dir ) spd = -30.0f ;
	pl_w->spd.x = spd ;
	pl_w->spd.y = -DOT_Y( 0.125f ) ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->atck_no = pl3a_at_t_kick ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 1 ) ;
}

/*--------------------------------------------------------------*/
static void t_kick_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_2 ++ ;
		pl_w->cg_ctr = 1 ;
		pl_land_ef_set( pl_w ) ;
		pl_char_move( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void t_kick_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stop_stand_ret( pl_w ) ;
}

/****************************************************************/
/*			A strong punch, standing or in the air				*/
/****************************************************************/
PLAYER_WORK *pl3a_catch_ck( PLAYER_WORK *pl_w )
{
	static Sint32 catch_tbl[][2] = {
		{ 1, 0 }, { 0, 0 }, { 2, 0 }, { 0, 0 },
	} ;
	Sint32 type ;
	Uint32 work ;
	PLAYER_WORK *em = 0 ;

	if( type = catch_tbl[pl_w->pl_stance][pl_w->atck_pk] ){
		if( work = pl_w->atck_lever & ( KEY_FORWARD | KEY_BACK ) ){
			pl_w->dir = work >> 10 ;
			if( pl_w->atck_level == 1 ){
				if( em = pl_catch_ck( pl_w ) ) pl_w->catch_no = type - 1 ;
			}
		}
	}
	return em ;
}

/****************************************************************/
/*			Start the throw that pl3a_catch_ck chose			*/
/****************************************************************/
void pl3a_catch_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;
	Float x, y ;

	ofs.x = -DOT_X( 8.0f ) ;
	ofs.y = DOT_Y( 16.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	scr_excep_set( pl_w, 5, 5 ) ;
	pl_char_set( pl_w, Char_Catch, pl_w->catch_no & CATCH_NO_MASK ) ;
	x = pl_w->pos.x ;
	y = pl_w->pos.y ;
	catch_em_pos_set0( pl_w, pl_w->catch_w ) ;
	pl_w->pos.x = x ;
	pl_w->pos.y = y ;
	catch_gauge_add( pl_w ) ;
	if( pl_w->pl_stance != STANCE_AIR ) pls3a_00_set( pl_w, 1 ) ;
}

static void p_catch( PLAYER_WORK *pl_w );
static void j_catch( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void pl3a_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		p_catch, j_catch,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

static void p_catch_jump( PLAYER_WORK *pl_w );
static void p_catch_rise( PLAYER_WORK *pl_w );
static void catch_carry( PLAYER_WORK *pl_w );
static void catch_throw( PLAYER_WORK *pl_w );
static void catch_fall( PLAYER_WORK *pl_w );
static void catch_land( PLAYER_WORK *pl_w );
static void j_catch_init( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The ground throw: jumps up with the opponent and	*/
/*			throws him down										*/
/*==============================================================*/
static void p_catch( PLAYER_WORK *pl_w )
{
	static void ( *p_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		p_catch_jump, p_catch_rise, catch_carry, catch_throw, catch_fall, catch_land,
	} ;

	p_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			The jump's speeds come from where the				*/
/*			animation's next frames put him						*/
/*==============================================================*/
static void p_catch_jump( PLAYER_WORK *pl_w )
{
	Float x, y ;

	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 40 ;
		x = pl_w->pos.x ;
		y = pl_w->pos.y ;
		pl_w->catch_grip_old = -1 ;
		LO_BYTE( pl_w->cg_rival ) = 3 ;
		catch_em_pos_set0( pl_w, pl_w->catch_w ) ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->spd.x = ( pl_w->pos.x - x ) / 40.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_w->spd.y = ( pl_w->pos.y - y ) / 40.0f + DOT_Y( 7.5f ) ;
		pl_w->catch_grip_old = -1 ;
		LO_BYTE( pl_w->cg_rival ) = 1 ;
		catch_em_pos_set0( pl_w, pl_w->catch_w ) ;
		pl_w->pos.x = x ;
		pl_w->pos.y = y ;
	}
}

/*--------------------------------------------------------------*/
static void p_catch_rise( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_w->timer0 -- ;
	if( pl_w->spd.y < 0.0f ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Catch, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void catch_carry( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( -- pl_w->timer0 <= 0 ){
		pl_w->routine_2 ++ ;
		pl_char_move( pl_w ) ;
		catch_em_pos_set0( pl_w, pl_w->catch_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void catch_throw( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_flag1 ){
		pl_w->cg_flag1 = 0 ;
		ofs.x = 0.0f ;
		ofs.y = 0.0f ;
		catch_hit_mark_set( pl_w, &ofs ) ;
		pl_se_req( pl_w, 2 ) ;
	}
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->catch_w->dm_hit_w = pl_w ;
		pl_w->catch_w->catch_type = 1 ;
		pl_w->catch_w->atck_no = pl3a_at_throw ;
		ofs.x = DOT_X( 160.0f ) ;
		if( pl_w->disp_dir ) ofs.x = -ofs.x ;
		ofs.x += pl_w->pos.x ;
		ofs.y = pl_w->ground_y ;
		pl_w->spd.x = ( pl_w->pos.x - ofs.x ) / 32.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_w->spd.y = ( pl_w->pos.y - ofs.y ) / 32.0f + DOT_Y( 6.0f ) ;
		scr_excep_clr() ;
	} else if( pl_w->cg_prm ) catch_pos_set( pl_w, pl_w->catch_w ) ;
	else catch_em_pos_set0( pl_w, pl_w->catch_w ) ;
}

/*--------------------------------------------------------------*/
static void catch_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Catch, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void catch_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl3a_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			The air throw: already in the air, so it starts		*/
/*			with the carry										*/
/*==============================================================*/
static void j_catch( PLAYER_WORK *pl_w )
{
	static void ( *j_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		j_catch_init, j_catch_init, catch_carry, catch_throw, catch_fall, catch_land,
	} ;

	j_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void j_catch_init( PLAYER_WORK *pl_w )
{
	Float x, y ;

	pl_w->routine_2 = 2 ;
	pl_w->timer0 = 20 ;
	x = pl_w->pos.x ;
	y = pl_w->pos.y ;
	pl_w->catch_grip_old = -1 ;
	LO_BYTE( pl_w->cg_rival ) = 3 ;
	catch_em_pos_set0( pl_w, pl_w->catch_w ) ;
	pl_w->spd.x = ( pl_w->pos.x - x ) / 20.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->spd.y = ( pl_w->pos.y - y ) / 20.0f + DOT_Y( 3.75f ) ;
	pl_w->catch_grip_old = -1 ;
	LO_BYTE( pl_w->cg_rival ) = 1 ;
	catch_em_pos_set0( pl_w, pl_w->catch_w ) ;
	pl_w->pos.x = x ;
	pl_w->pos.y = y ;
}

/****************************************************************/
/*			Holding the opponent: nothing to do					*/
/****************************************************************/
void pl3a_hold( PLAYER_WORK *pl_w )
{
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void pl3a_sp_unison( PLAYER_WORK *pl_w )
{
	static Uint8 unison_tbl[3] = { pl3a_sp_lunch, pl3a_sp_rking, pl3a_sp_lunch } ;

	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = unison_tbl[pl_w->pl_personal] ;
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl3a_sp_gattai( PLAYER_WORK *pl_w )
{
	static Uint8 gattai_tbl[3] = { pl3a_sp_lunch, pl3a_sp_rking, pl3a_sp_lunch } ;

	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = gattai_tbl[pl_w->pl_personal] ;
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
/*			The special move and its strength for each			*/
/*			assist												*/
/****************************************************************/
void pl3a_sp_assist( PLAYER_WORK *pl_w )
{
	static Uint8 assist_tbl[3][2] = {
		{ pl3a_sp_engo, 0 }, { pl3a_sp_fire, 1 }, { pl3a_sp_fire, 0 },
	} ;

	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = assist_tbl[pl_w->pl_personal][0] ;
	pl_w->atck_level = assist_tbl[pl_w->pl_personal][1] ;
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void pl3a_sp_zero( PLAYER_WORK *pl_w )
{
	static Uint8 zero_tbl[3][2] = {
		{ pl3a_sp_engo, 0 }, { pl3a_sp_fire, 1 }, { pl3a_sp_fire, 0 },
	} ;

	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = zero_tbl[pl_w->pl_personal][0] ;
	pl_w->atck_level = zero_tbl[pl_w->pl_personal][1] ;
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
/*			One of three animations at random on a KO			*/
/****************************************************************/
void pl3a_ko_set( PLAYER_WORK *pl_w )
{
	pl_char_set( pl_w, Char_Stand, Rnd() % 3 + 10 ) ;
}

/****************************************************************/
/*			Back to standing, at rest							*/
/****************************************************************/
void pl3a_stand_ret( PLAYER_WORK *pl_w )
{
	pl_w->obj_flag = 0 ;
	pl_w->armor_flag = 0 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl3a.c" End of File	*/

/* Storm (character 2a): command checks, normal attacks, appearance, win and lose poses, special
   moves and hyper combos, Lightning Attack, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl2a_def.h"

void pl2a_main( PLAYER_WORK *pl_w );
void pl2a_com_ck( PLAYER_WORK *pl_w );
void pl2a_atck_set( PLAYER_WORK *pl_w );
void pl2a_atck( PLAYER_WORK *pl_w );
void pl2a_dash( PLAYER_WORK *pl_w );
void pl2a_noki( PLAYER_WORK *pl_w );
void pl2a_appear( PLAYER_WORK *pl_w );
void pl2a_win( PLAYER_WORK *pl_w );
void pl2a_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl2a_catch_ck( PLAYER_WORK *pl_w );
void pl2a_catch_set( PLAYER_WORK *pl_w );
void pl2a_catch( PLAYER_WORK *pl_w );
void pl2a_hold( PLAYER_WORK *pl_w );
int pl2a_delay_ck( PLAYER_WORK *pl_w );
void pl2a_t_kick( PLAYER_WORK *pl_w );
void pl2a_upper( PLAYER_WORK *pl_w );
void pl2a_sp_unison( PLAYER_WORK *pl_w );
void pl2a_sp_gattai( PLAYER_WORK *pl_w );
void pl2a_sp_assist( PLAYER_WORK *pl_w );
void pl2a_sp_zero( PLAYER_WORK *pl_w );
void pl2a_float( PLAYER_WORK *pl_w );
static void stand_ret( PLAYER_WORK *pl_w );

/* the stick, as four bits, to one of eight ways (never used) */
static Uint8 lattack_dir_no[16] = {
	0, 2, 6, 0, 4, 3, 5, 0, 0, 1, 7, 0, 0, 0, 0, 0,
} ;

/* the stick, as four bits, to the way a move goes */
static Uint8 lattack_dir[16] = {
	0x00, 0x08, 0x18, 0x00, 0x10, 0x0c, 0x14, 0x00, 0x00, 0x04, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00,
} ;

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

/* the tables by attack: the ground ones, then jumping and super jumping */
static Sint8 *nml_tbl[6] = {
	lp_tbl, mp_tbl, hp_tbl, lk_tbl, mk_tbl, hk_tbl,
} ;
static Sint8 *jump_tbl[12] = {
	j_lp_tbl, j_mp_tbl, j_hp_tbl, j_lk_tbl, j_mk_tbl, j_hk_tbl,
	sj_lp_tbl, sj_mp_tbl, sj_hp_tbl, sj_lk_tbl, sj_mk_tbl, sj_hk_tbl,
} ;

/* commands: time or steps, kind, the special move, then the stick and buttons */
static Uint16 com_typhoon[] = { 3, 0, pl2a_sp_typhoon, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Typhoon */
static Uint16 com_dtyphoon[] = { 5, 0, pl2a_sp_dtyphoon, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Double Typhoon */
static Uint16 com_lstorm[] = { 5, COM_HYPER | COM_ALL_SW, pl2a_sp_lstorm, 0, KEY_BACK, KEY_DOWN | KEY_BACK, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Lightning Storm */
static Uint16 com_fly[] = { 3, COM_ALL_SW, pl2a_sp_fly, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Flight */
static Uint16 com_hail[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl2a_sp_hail, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Hail Storm */
static Uint16 com_jsphere[] = { 3, COM_AIR, pl2a_sp_jsphere, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Lightning Sphere (air) */

static void ( *pl2a_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl2a_main,
	pl2a_com_ck,
	pl2a_atck_set,
	pl2a_atck,
	pl2a_dash,
	pl2a_noki,
	pl2a_appear,
	pl2a_win,
	pl2a_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl2a_catch_ck,	/* returns a value */
	pl2a_catch_set,
	pl2a_catch,
	pl2a_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl2a_delay_ck,
	pl2a_t_kick,
	pl2a_upper,
	pl2a_sp_unison,
	pl2a_sp_gattai,
	pl2a_sp_assist,
	pl2a_sp_zero,
	0,
	0,
	0,
	0,
	pl2a_float,
} ;

static int hail_ck( PLAYER_WORK *pl_w );
static int typhoon_ck( PLAYER_WORK *pl_w );
static int lstorm_ck( PLAYER_WORK *pl_w );
static int dtyphoon_ck( PLAYER_WORK *pl_w );
static int lattack_ck( PLAYER_WORK *pl_w );
static int fly_ck( PLAYER_WORK *pl_w );
static int chouhatsu_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static int jsphere_ck( PLAYER_WORK *pl_w );
static int hail_delay_ck( PLAYER_WORK *pl_w );
static int lstorm_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl2a_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl2a_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl2a_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( hail_ck( pl_w ) ) return ;
	if( lstorm_ck( pl_w ) ) return ;
	if( fly_ck( pl_w ) ) return ;
	if( jsphere_ck( pl_w ) ) return ;
	if( typhoon_ck( pl_w ) ) return ;
	if( dtyphoon_ck( pl_w ) ) return ;
	if( lattack_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( pl_air_dash_sw_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
static int hail_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_hail, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w3 ) ;
	pl_w->pl_sp_type = pl2a_sp_hail ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
/*			On the ground, or in the air once a jump			*/
/*==============================================================*/
static int typhoon_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_typhoon, &pl_w->com_w ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w ) ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	pl_w->pl_sp_type = pl2a_sp_typhoon ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int lstorm_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_lstorm, &pl_w->com_w0 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->pl_sp_type = pl2a_sp_lstorm ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static int dtyphoon_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_dtyphoon, &pl_w->com_w4 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w4 ) ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	pl_w->pl_sp_type = pl2a_sp_dtyphoon ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			Lightning Attack, which can follow another one		*/
/*==============================================================*/
static int lattack_ck( PLAYER_WORK *pl_w )
{
	static Uint8 dir_tbl[16] = {
		0x00, 0x02, 0x01, 0x03, 0x04, 0x06, 0x05, 0x07, 0x08, 0x0a, 0x09, 0x0b, 0x0c, 0x0e, 0x0d, 0x0f,
	} ;
	Uint16 sw ;

	if( !sw_now_ck( pl_w, 0x0140, &sw, 4 ) ) return 0 ;
	com_sw_set( pl_w, ( COM_WORK * )&sw ) ;
	if( pl_w->ground_y + DOT_Y( 32.0f ) > pl_w->pos.y && ( pl_w->pl_sw_new & KEY_DOWN ) ) return 0 ;
	if( pl_w->act_st == N_SPECIAL && pl_w->pl_sp_type == pl2a_sp_lattack ){
		if( pl_w->tlocal[TL_DASH_WAIT] ) return 0 ;
		if( !pl_w->tlocal[TL_DASH_CNT] ) return 0 ;
		if( !sp_ok_hyper_ck( pl_w ) ) return 0 ;
		if( pl_w->hit_flag & ( HR_GUARD | HR_ARMOR ) ) return 0 ;
		pl_w->tlocal[TL_DASH_AGAIN] = 1 ;
		pl_w->tlocal[TL_DASH_CNT] -- ;
	} else {
		if( pl_w->pl_stance == STANCE_AIR && pl_w->jump_com_cnt ) return 0 ;
		if( !sp_ok_ck( pl_w ) ) return 0 ;
		if( pl_w->cpu_flag ){
			sw = pl_w->atck_lever >> 10 ;
			if( pl_w->pl_dir ){
				sw &= 0x0f ;
				sw = dir_tbl[sw] ;
			}
			pl_w->tlocal[TL_DASH_SW] = sw ;
		}
		pl_w->tlocal[TL_DASH_AGAIN] = 0 ;
		pl_w->tlocal[TL_DASH_CNT] = 2 ;
		pl_w->tlocal[TL_FLY_TM] = 1 ;
		pl_w->fly_flag = 0 ;
	}
	pl_w->atck_level = 0 ;
	pl_w->jump_com_cnt ++ ;
	pl_w->tlocal[TL_DASH_WAIT] = 16 ;
	pl_w->pl_sp_type = pl2a_sp_lattack ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int fly_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_fly, &pl_w->com_w2 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	if( pl_w->fly_flag ){
		pl_w->tlocal[TL_FLY_TM] = 1 ;
		return 0 ;
	}
	pl_w->pl_sp_type = pl2a_sp_fly ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 8 ) ) return 0 ;
	pl_w->pl_sp_type = pl2a_sp_chouhatsu ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int snapback_ck( PLAYER_WORK *pl_w )
{
	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl2a_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static int jsphere_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_jsphere, &pl_w->com_w1 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	pl_w->pl_sp_type = pl2a_sp_jsphere ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl2a_delay_ck( PLAYER_WORK *pl_w )
{
	if( hail_delay_ck( pl_w ) || lstorm_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static int hail_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_hail, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w3 ) ;
	pl_w->delay_sp_type = pl2a_sp_hail ;
	return 1 ;
}

/*==============================================================*/
static int lstorm_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_lstorm, &pl_w->com_w0 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->delay_sp_type = pl2a_sp_lstorm ;
	return 1 ;
}

/****************************************************************/
/*			Every frame: the timers and the two colour			*/
/*			cycles												*/
/****************************************************************/
void pl2a_main( PLAYER_WORK *pl_w )
{
	Uint32 i ;

	pl_w->tlocal[TL_ANM_FLAG] = HI_BYTE( pl_w->cg_rival ) ;
	if( pl_w->tlocal[TL_DASH_WAIT] ) pl_w->tlocal[TL_DASH_WAIT] -- ;
	if( pl_w->fly_flag ){
		if( !-- pl_w->tlocal[TL_FLY_TM] ) pl_w->fly_flag = 0 ;
	}
	if( pl_w->act_st == N_STAND ){
		if( -- pl_w->tlocal[TL_IDLE_TM] < 0 ){
			pl_w->tlocal[TL_IDLE_TM] = ( Sys->game_timer0 & 31 ) + 32 ;
			pls2a_01_set( pl_w, 0, Rnd() & 1 ) ;
		}
	}
	if( pl_w->tlocal[TL_COL_STOP] ){
		pl_w->tlocal[TL_COL_STOP] -- ;
		return ;
	}
	if( -- pl_w->tlocal[TL_COL_TM] < 0 ){
		pl_w->tlocal[TL_COL_TM] = 1 ;
		pl_w->tlocal[TL_COL_1] ++ ;
		pl_w->tlocal[TL_COL_1] &= 3 ;
		ef_color_set( pl_w, 1, pl_w->tlocal[TL_COL_1] + 6, 1 ) ;
		pl_w->tlocal[TL_COL_2] ++ ;
		if( pl_w->tlocal[TL_COL_2] > 4 ) pl_w->tlocal[TL_COL_2] = 0 ;
		ef_color_set( pl_w, 2, pl_w->tlocal[TL_COL_2] + 10, 1 ) ;
	}
	if( pl_w->tlocal[TL_NML_COL_TM] > 0 ){
		if( pl_w->char_no.w != ( Uint16 )pl_w->tlocal[TL_NML_CHAR] ){
			pl_w->tlocal[TL_NML_COL_TM] = 0 ;
			pl_color_set( pl_w, 1 ) ;
			return ;
		}
		i = ( -- pl_w->tlocal[TL_NML_COL_TM] % 6 ) >> 1 ;
		ef_color_set( pl_w, 0, pl_w->pl_color * 3 + 16 + i, 1 ) ;
	}
}

/* each attack's number, strength and animation */
static Uint8 nml_atck_no[12] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 } ;
static Uint8 nml_atck_lv[3] = { 0, 1, 2 } ;
static Uint8 nml_char_no[12] = { 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2 } ;
static Uint8 nml_char_grp[12] = { Char_Stand_P, Char_Stand_P, Char_Stand_P, Char_Stand_K, Char_Stand_K, Char_Stand_K, Char_Crouch_P, Char_Crouch_P, Char_Crouch_P, Char_Crouch_K, Char_Crouch_K, Char_Crouch_K } ;
static Uint8 jump_atck_no[6] = { 12, 13, 14, 15, 16, 17 } ;
static Uint8 jump_atck_lv[3] = { 0, 1, 2 } ;
static Uint8 jump_char_no[6] = { 0, 1, 2, 0, 1, 2 } ;
static Uint8 jump_char_grp[6] = { Char_Jump_P, Char_Jump_P, Char_Jump_P, Char_Jump_K, Char_Jump_K, Char_Jump_K } ;

/*==============================================================*/
static void nml_atck( PLAYER_WORK *pl_w )
{
	Uint8 no ;

	pl_brake_set( pl_w ) ;
	no = pl_w->nml_no ;
	if( pl_w->pl_stance == STANCE_CROUCH ) no += 6 ;
	if( pl_w->atck_pk ) no += 3 ;
	pl_w->chain_tbl = nml_tbl[no % 6] ;
	pl_w->nml_level = nml_atck_lv[pl_w->nml_no] ;
	pl_w->atck_no = nml_atck_no[no] ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, nml_char_grp[no], nml_char_no[no] ) ;
	if( !pl_w->atck_pk ){
		pl_w->tlocal[TL_NML_COL_TM] = 32 ;
		pl_w->tlocal[TL_NML_CHAR] = pl_w->char_no.w ;
	}
}

/*==============================================================*/
static void jump_atck( PLAYER_WORK *pl_w )
{
	Uint8 no ;
	Uint8 char_no ;

	no = pl_w->nml_no ;
	if( pl_w->atck_pk ) no += 3 ;
	pl_w->tlocal[TL_JUMP_MP] = 0 ;
	pl_w->nml_level = jump_atck_lv[pl_w->nml_no] ;
	pl_w->atck_no = jump_atck_no[no] ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, jump_char_grp[no], jump_char_no[no] ) ;
	if( no == 1 ){
		pl_w->tlocal[TL_JUMP_MP] = 1 ;
		char_no = 1 ;
		if( pl_w->atck_lever & KEY_UP ) char_no = 7 ;
		if( pl_w->atck_lever & KEY_DOWN ) char_no = 6 ;
		pl_char_set( pl_w, Char_Jump_P, char_no ) ;
	}
	if( pl_w->sjump_flag ) no += 6 ;
	pl_w->chain_tbl = jump_tbl[no] ;
	if( !pl_w->atck_pk ){
		if( pl_w->jump_atk_cnt & JATK_P_MASK ) pl_w->jump_atk_cnt -= 0x01 ;
		pl_w->tlocal[TL_NML_COL_TM] = 32 ;
		pl_w->tlocal[TL_NML_CHAR] = pl_w->char_no.w ;
	} else {
		if( pl_w->jump_atk_cnt & JATK_K_MASK ) pl_w->jump_atk_cnt -= 0x10 ;
	}
}

/*==============================================================*/
/*			In the air, only while that button's jump			*/
/*			attacks are left									*/
/*==============================================================*/
static void jump_atck_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->fly_flag ) jump_atck( pl_w ) ;
	if( !pl_w->atck_pk ){
		if( pl_w->jump_atk_cnt & JATK_P_MASK ) jump_atck( pl_w ) ;
	} else if( pl_w->jump_atk_cnt & JATK_K_MASK ) jump_atck( pl_w ) ;
}

/****************************************************************/
/*			Normal attacks										*/
/****************************************************************/
void pl2a_atck_set( PLAYER_WORK *pl_w )
{
	static void ( *atck_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_atck, jump_atck_ck, jump_atck, nml_atck,
	} ;

	atck_set_jmp[pl_w->atck_type]( pl_w ) ;
}

/*==============================================================*/
static void jump_move( PLAYER_WORK *pl_w )
{
	pl2a_float( pl_w ) ;
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
	if( !pl_w->fly_flag ){
		pl_jump_hit_ck( pl_w ) ;
		pl_sjump_ctrl( pl_w ) ;
	}
	jump_move( pl_w ) ;
}

/*==============================================================*/
static void nml_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_brake_stop( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ) stand_ret( pl_w ) ;
}

/*==============================================================*/
static void nml_move_dash( PLAYER_WORK *pl_w )
{
	pl_smoke_set( pl_w ) ;
	nml_move( pl_w ) ;
}

/****************************************************************/
void pl2a_atck( PLAYER_WORK *pl_w )
{
	static void ( *atck_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_move, jump_move, sjump_move, nml_move_dash,
	} ;

	atck_jmp[pl_w->atck_type]( pl_w ) ;
}

/****************************************************************/
/*			The launcher										*/
/****************************************************************/
void pl2a_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_brake_set( pl_w ) ;
		pl_gauge_add( pl_w, 5 ) ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->atck_no = pl2a_at_upper ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, 6 ) ;
	}
	if( pl_w->atck_type == 3 ) pl_smoke_set( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_brake_stop( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ) stand_ret( pl_w ) ;
}

static void dash_init( PLAYER_WORK *pl_w );
static void dash_move( PLAYER_WORK *pl_w );
static void dash_end( PLAYER_WORK *pl_w );

/****************************************************************/
void pl2a_dash( PLAYER_WORK *pl_w )
{
	static void ( *dash_jmp[])( PLAYER_WORK *pl_w ) = {
		dash_init, dash_move, dash_end,
	} ;

	dash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dash_init( PLAYER_WORK *pl_w )
{
	Float spd, acc ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ) return ;
	pl_w->routine_2 ++ ;
	pl_w->timer0 = 16 ;
	spd = -DOT_X( 10.0f ) ;
	acc = DOT_X( 0.125f ) ;
	if( pl_w->pl_dir ){
		spd = DOT_X( 10.0f ) ;
		acc = -DOT_X( 0.125f ) ;
	}
	pl_w->spd.x = spd ;
	pl_w->acc.x = acc ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	if( pl_w->old_act_st != N_DASH ) pls2a_01_set( pl_w, 1, 0 ) ;
	pl_voice_req( pl_w, 33 ) ;
}

/*--------------------------------------------------------------*/
static void dash_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 < 0 ){
		pl_w->routine_2 ++ ;
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
	if( pl_char_move( pl_w ) < 0 ) stand_ret( pl_w ) ;
}

static void noki_init( PLAYER_WORK *pl_w );
static void noki_move( PLAYER_WORK *pl_w );
static void noki_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash										*/
/****************************************************************/
void pl2a_noki( PLAYER_WORK *pl_w )
{
	static void ( *noki_jmp[])( PLAYER_WORK *pl_w ) = {
		noki_init, noki_move, noki_end,
	} ;

	noki_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void noki_init( PLAYER_WORK *pl_w )
{
	Float spd ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ) return ;
	pl_w->routine_2 ++ ;
	pl_w->timer0 = 16 ;
	spd = 17.5f ;
	if( pl_w->pl_dir ) spd = -17.5f ;
	pl_w->spd.x = spd ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pls2a_01_set( pl_w, 1, 1 ) ;
	pl_voice_req( pl_w, 33 ) ;
}

/*--------------------------------------------------------------*/
static void noki_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Dash, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void noki_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) stand_ret( pl_w ) ;
}

static void appear_init( PLAYER_WORK *pl_w );
static void appear_move( PLAYER_WORK *pl_w );
static void appear_land( PLAYER_WORK *pl_w );
static void win_time_ko( PLAYER_WORK *pl_w );
static void lose_time_ko( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Flying in from above								*/
/****************************************************************/
void pl2a_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_init,
		appear_move,
		appear_land,
	} ;

	appear_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void appear_init( PLAYER_WORK *pl_w )
{
	Float ofs ;

	if( vs_sys_work.vs_st < VS_APPEAR ) return ;
	pl_w->routine_2 ++ ;
	pl_w->disp_flag = 1 ;
	pl_w->spd.z = pl_w->pos.x ;
	pl_w->pos.y = pl_w->ground_y + -DOT_Y( 10.0f ) ;
	ofs = DOT_X( 64.0f ) ;
	if( pl_w->pl_no ) ofs = -DOT_X( 64.0f ) ;
	pl_w->pos.x -= ofs ;
	pl_char_set( pl_w, Char_Appear, 0 ) ;
}

/* the speeds for each step of the flight in */
typedef struct APPEAR_SPD {
	Sint16	x, y;
} APPEAR_SPD;

static APPEAR_SPD appear_spd[20] = {
	{  0x0400,  0x0000 }, {  0x0600,  0x0000 }, {  0x0800,  0x0000 }, {  0x0800,  0x0000 },
	{  0x0400,  0x0000 }, {  0x0200,  0x0100 }, {  0x0000,  0x0200 }, { -0x0200,  0x0400 },
	{ -0x0200,  0x0500 }, { -0x0200,  0x0100 }, { -0x0100,  0x0000 }, { -0x0100, -0x0080 },
	{  0x0060, -0x0180 }, {  0x0060, -0x0100 }, {  0x0060, -0x0080 }, {  0x0060, -0x0140 },
	{  0x0040, -0x0100 }, {  0x0040, -0x00c0 }, {  0x0000, -0x0080 }, {  0x0000,  0x0000 },
} ;

/*--------------------------------------------------------------*/
static void appear_move( PLAYER_WORK *pl_w )
{
	APPEAR_SPD *spd ;
	int x, y ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Stand, 0 ) ;
		return ;
	}
	spd = &appear_spd[pl_w->cg_type] ;
	x = spd->x << 8 ;
	y = spd->y << 8 ;
	if( !pl_w->disp_dir ) x = -x ;
	pl_w->spd.x = x * 1.6666666f / 65536.0f ;
	pl_w->spd.y = y * 2.142857f / 65536.0f ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
}

/*--------------------------------------------------------------*/
static void appear_land( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y > pl_w->ground_y ) return ;
	pl_w->routine_1 ++ ;
	pl_w->pos.x = pl_w->spd.z ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_land_ef_set( pl_w ) ;
}

/****************************************************************/
void pl2a_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_time_ko,
		lose_time_ko,
		win_time_ko,
		lose_time_ko,
		lose_time_ko,
	} ;

	if( pl_win_end_ck( pl_w ) ) stand_ret( pl_w ) ;
	else win_jmp[pl_w->type]( pl_w ) ;
}

/*==============================================================*/
static void win_time_ko( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, CH2A_WIN ) ;
	}else{
		pl_char_move( pl_w ) ;
	}
}

/*==============================================================*/
static void lose_time_ko( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, CH2A_LOSE ) ;
	}else{
		pl_char_move( pl_w ) ;
	}
}

static void sp_typhoon( PLAYER_WORK *pl_w );
static void sp_lstorm( PLAYER_WORK *pl_w );
static void sp_lattack( PLAYER_WORK *pl_w );
static void sp_fly( PLAYER_WORK *pl_w );
static void sp_hail( PLAYER_WORK *pl_w );
static void sp_dtyphoon( PLAYER_WORK *pl_w );
static void sp_07( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_09( PLAYER_WORK *pl_w );
static void sp_10( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );
static void sp_jsphere( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Special moves and hyper combos						*/
/****************************************************************/
void pl2a_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *pl_sp_tbl[])( PLAYER_WORK *pl_w ) = {
		sp_typhoon, sp_lstorm, sp_lattack, sp_fly, sp_lattack, sp_hail, sp_dtyphoon, sp_07, sp_chouhatsu, sp_09, sp_10, sp_snapback, sp_jsphere,
	} ;

	pl_sp_tbl[pl_w->pl_sp_type]( pl_w ) ;
}

/*==============================================================*/
/*			On the ground or in the air; in the air she			*/
/*			slows to a near stop								*/
/*==============================================================*/
static void sp_typhoon( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_dir_set( pl_w ) ;
		pl_gauge_add( pl_w, 5 ) ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level ) ;
		pl_w->atck_no = pl2a_at_typhoon ;
		HIT_RESET( pl_w ) ;
		if( pl_w->pl_stance != STANCE_AIR ){
			pl_w->pl_stance = STANCE_STAND ;
			pl_sp_ef_set( pl_w ) ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
		} else {
			pl_w->spd.x /= 16.0f ;
			pl_w->spd.y /= 8.0f ;
			pl_w->acc.y /= 64.0f ;
			pl_w->acc.x = 0.0f ;
		}
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ) pl_w->pos.y = pl_w->ground_y ;
	if( pl_char_move( pl_w ) < 0 ){
		if( pl_w->pl_stance == STANCE_AIR ) pl_jump_ret( pl_w ) ;
		else stand_ret( pl_w ) ;
	} else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shl2a_00_set( pl_w ) ;
		pl_w->unk_27b = 0 ;
		pl_w->cancel_timer = 16 ;
	}
}

static void lstorm_init( PLAYER_WORK *pl_w );
static void lstorm_wait( PLAYER_WORK *pl_w );
static void lstorm_move( PLAYER_WORK *pl_w );
static void lstorm_end( PLAYER_WORK *pl_w );
static void lstorm_color( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo, kept up by pressing the buttons		*/
/*==============================================================*/
static void sp_lstorm( PLAYER_WORK *pl_w )
{
	static void ( *lstorm_jmp[])( PLAYER_WORK *pl_w ) = {
		lstorm_init, lstorm_wait, lstorm_move, lstorm_end,
	} ;

	lstorm_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void lstorm_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 1 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	if( pl_w->pl_stance != STANCE_AIR ){
		pl_w->pl_stance = STANCE_STAND ;
		pl_sp_ef_set( pl_w ) ;
	}
	pl_w->atck_no = pl2a_at_lstorm ;
	HIT_RESET( pl_w ) ;
	pl_w->timer0 = 48 ;
	pl_w->timer1 = 16 ;
	pl_w->tlocal[TL_LSTORM_COL_TM] = 2 ;
	pl_w->tlocal[TL_LSTORM_COL] = 0 ;
	pl_char_set( pl_w, Char_Special, 4 ) ;
}

/*--------------------------------------------------------------*/
static void lstorm_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		ofs.x = 0.0f ;
		ofs.y = 210.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void lstorm_move( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	lstorm_color( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_color_set( pl_w, 1 ) ;
		pl_char_set( pl_w, Char_Special, 27 ) ;
	} else if( pl_w->cg_type < 0 ){
		pl_w->cg_type = 0 ;
		if( pl_w->pl_sw_now & KEY_ATCK ){
			if( -- pl_w->timer1 >= 0 ) pl_w->timer0 ++ ;
		}
		shl2a_02_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void lstorm_end( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		if( pl_w->pl_stance == STANCE_AIR ) pl_jump_ret( pl_w ) ;
		else stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
/*			Steps the colour every other frame					*/
/*==============================================================*/
static void lstorm_color( PLAYER_WORK *pl_w )
{
	if( !-- pl_w->tlocal[TL_LSTORM_COL_TM] ){
		pl_w->tlocal[TL_LSTORM_COL_TM] = 2 ;
		if( ++ pl_w->tlocal[TL_LSTORM_COL] > 2 ) pl_w->tlocal[TL_LSTORM_COL] = 0 ;
		ef_color_set( pl_w, 0, pl_w->tlocal[TL_LSTORM_COL], 1 ) ;
	}
}

static void lattack_init( PLAYER_WORK *pl_w );
static void lattack_start( PLAYER_WORK *pl_w );
static void lattack_move( PLAYER_WORK *pl_w );
static void lattack_bound( PLAYER_WORK *pl_w );
static void lattack_end( PLAYER_WORK *pl_w );
static void lattack_spd_set( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Lightning Attack: a dash in any of eight ways		*/
/*==============================================================*/
static void sp_lattack( PLAYER_WORK *pl_w )
{
	static void ( *lattack_jmp[])( PLAYER_WORK *pl_w ) = {
		lattack_init, lattack_start, lattack_move, lattack_bound, lattack_end,
	} ;

	lattack_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void lattack_init( PLAYER_WORK *pl_w )
{
	int sw, dir, no ;

	pl_w->routine_2 ++ ;
	if( pl_w->pl_stance != STANCE_AIR ){
		pl_w->pos.y = pl_w->ground_y + 2.142857f ;
		pl_sp_ef_set( pl_w ) ;
	}
	pl_air_set( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	sw = pl_w->pl_new_lvbt >> 10 ;
	if( pl_w->cpu_flag ) sw = pl_w->tlocal[TL_DASH_SW] ;
	dir = 24 ;
	if( pl_w->disp_dir ) dir = 8 ;
	sw &= 0x0f ;
	if( sw ) dir = lattack_dir[sw] ;
	pl_w->tlocal[TL_DIR] = dir ;
	pl_w->dir = dir ;
	pl_w->atck_no = pl_w->tlocal[TL_DASH_CNT] + 84 ;
	HIT_RESET( pl_w ) ;
	no = 0 ;
	if( pl_w->tlocal[TL_DASH_AGAIN] ) no = 10 ;
	pl_char_set( pl_w, Char_Super, no ) ;
}

/*--------------------------------------------------------------*/
static void lattack_start( PLAYER_WORK *pl_w )
{
	Uint32 dir ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 32 ;
		pl_w->timer1 = 0 ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		lattack_spd_set( pl_w ) ;
		dir = pl_w->dir ;
		if( pl_w->disp_dir ) dir = ( 32 - dir ) & 31 ;
		pl_char_set( pl_w, Char_Super, ( dir >> 2 ) + 1 ) ;
		pls2a_04_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void lattack_move( PLAYER_WORK *pl_w )
{
	Float y, spd ;

	pl_w->push_ng_timer = 2 ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 = 4 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Special, 20 ) ;
	} else if( pl_w->pos.y <= pl_w->ground_y + -DOT_Y( 16.0f ) ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->routine_2 = 4 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Special, 20 ) ;
	} else {
		if( pl_w->hit_flag ){
			if( pl_w->hit_flag & ( HR_GUARD | HR_ARMOR ) ){
				pl_w->routine_2 ++ ;
				pl_w->timer0 = 32 ;
				pl_w->tlocal[TL_DASH_WAIT] = 16 ;
				spd = 1.6666666f ;
				if( pl_w->disp_dir ) spd = -1.6666666f ;
				pl_w->spd.x = spd ;
				pl_w->acc.x = 0.0f ;
				pl_w->spd.y = DOT_Y( 10.0f ) ;
				pl_w->acc.y = -DOT_Y( 0.5f ) ;
				pl_char_set( pl_w, Char_Super, 11 ) ;
				return ;
			}
			pl_w->acc.x = -pl_w->spd.x / 24.0f ;
			pl_w->acc.y = -pl_w->spd.y / 24.0f ;
		}
		y = pl_w->pos.y ;
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->pos.y > scr_work.stage_top ) pl_w->pos.y = y ;
		pl_char_move( pl_w ) ;
	}
}

/*==============================================================*/
/*			Knocked back off what the dash hit					*/
/*==============================================================*/
static void lattack_bound( PLAYER_WORK *pl_w )
{
	pl_w->tlocal[TL_DASH_WAIT] = 16 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y + -DOT_Y( 16.0f ) ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		stand_ret( pl_w ) ;
		return ;
	}
	if( -- pl_w->timer0 <= 0 ) pl_jump_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void lattack_end( PLAYER_WORK *pl_w )
{
	pl_w->tlocal[TL_DASH_WAIT] = 16 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		if( pl_w->pl_stance == STANCE_AIR ) pl_jump_ret( pl_w ) ;
		else stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
/*			The speed for the dash's way, taking back this		*/
/*			frame's step										*/
/*==============================================================*/
static void lattack_spd_set( PLAYER_WORK *pl_w )
{
	dir_spd_move( pl_w, 0x578 ) ;
	pl_w->pos.x -= pl_w->spd.x ;
	pl_w->pos.y -= pl_w->spd.y ;
}

/*==============================================================*/
/*			Flight, for 900 frames								*/
/*==============================================================*/
static void sp_fly( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_dir_set( pl_w ) ;
		if( pl_w->pl_stance != STANCE_AIR ) pl_sp_ef_set( pl_w ) ;
		pl_w->fly_flag = 1 ;
		pl_w->sjump_flag = 0 ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->sjump_flag = 0 ;
		pl_w->jump_com_cnt = 0 ;
		pl_w->jump_dash_cnt = 0 ;
		pl_w->tlocal[TL_FLY_TM] = 900 ;
		pl_w->timer0 = 36 ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 6.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.125f ) ;
		pl_char_set( pl_w, Char_Fly, 0 ) ;
		return ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 < 0 ) pl_jump_ret( pl_w ) ;
}

static void hail_init( PLAYER_WORK *pl_w );
static void hail_wait( PLAYER_WORK *pl_w );
static void hail_move( PLAYER_WORK *pl_w );
static void hail_hold( PLAYER_WORK *pl_w );
static void hail_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo that rises, then is kept up by		*/
/*			pressing the buttons								*/
/*==============================================================*/
static void sp_hail( PLAYER_WORK *pl_w )
{
	static void ( *hail_jmp[])( PLAYER_WORK *pl_w ) = {
		hail_init, hail_wait, hail_move, hail_hold, hail_end,
	} ;

	hail_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void hail_init( PLAYER_WORK *pl_w )
{
	Float spd ;

	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_w->timer0 = 40 ;
	pl_w->atck_no = pl2a_at_hail ;
	HIT_RESET( pl_w ) ;
	pl_air_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	spd = DOT_X( 4.0f ) ;
	if( pl_w->pl_dir ) spd = -DOT_X( 4.0f ) ;
	pl_w->spd.x = spd ;
	pl_w->spd.y = DOT_Y( 2.0f ) ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_char_set( pl_w, Char_Special, 23 ) ;
	pl_w->tlocal[TL_COL_STOP] = 96 ;
	pl_color_set( pl_w, 1 ) ;
	ef_color_set( pl_w, 3, 18, 1 ) ;
}

/*--------------------------------------------------------------*/
static void hail_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_w->tlocal[TL_COL_STOP] = 96 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !-- pl_w->timer0 ){
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 8 ;
		ofs.x = -DOT_X( 52.0f ) ;
		ofs.y = DOT_Y( 80.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void hail_move( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->tlocal[TL_COL_STOP] = 96 ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 32 ;
		pl_w->timer1 = 16 ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void hail_hold( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->tlocal[TL_COL_STOP] = 96 ;
	if( !-- pl_w->timer0 ) pl_w->routine_2 ++ ;
	if( pl_w->pl_sw_now & KEY_ATCK ){
		if( -- pl_w->timer1 >= 0 ) pl_w->timer0 ++ ;
	}
	shl2a_03_set( pl_w ) ;
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void hail_end( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->tlocal[TL_COL_STOP] = 96 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->tlocal[TL_COL_STOP] = 32 ;
		pl_jump_ret( pl_w ) ;
	}
}

/*==============================================================*/
/*			Like sp_typhoon, with its own attack and animation		*/
/*==============================================================*/
static void sp_dtyphoon( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_dir_set( pl_w ) ;
		pl_gauge_add( pl_w, 5 ) ;
		pl_w->atck_no = pl2a_at_dtyphoon ;
		HIT_RESET( pl_w ) ;
		if( pl_w->pl_stance != STANCE_AIR ){
			pl_w->pl_stance = STANCE_STAND ;
			pl_sp_ef_set( pl_w ) ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
		} else {
			pl_w->spd.x /= 16.0f ;
			pl_w->spd.y /= 8.0f ;
			pl_w->acc.y /= 64.0f ;
			pl_w->acc.x = 0.0f ;
		}
		pl_char_set( pl_w, Char_Special, 21 ) ;
		return ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ) pl_w->pos.y = pl_w->ground_y ;
	if( pl_char_move( pl_w ) < 0 ){
		if( pl_w->pl_stance == STANCE_AIR ) pl_jump_ret( pl_w ) ;
		else stand_ret( pl_w ) ;
	} else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shl2a_01_set( pl_w ) ;
	}
}

/*==============================================================*/
static void sp_07( PLAYER_WORK *pl_w )
{
}

/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Extra, 3 ) ;
	} else if( pl_char_move( pl_w ) < 0 ) stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_09( PLAYER_WORK *pl_w )
{
}

/*==============================================================*/
static void sp_10( PLAYER_WORK *pl_w )
{
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
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = pl2a_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 5 ) ;
}

/*--------------------------------------------------------------*/
static void snapback_move( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		ef_color_set( pl_w, 2, 10, 1 ) ;
		ofs.x = 5.0f ;
		ofs.y = DOT_Y( 74.0f ) ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
}

/*--------------------------------------------------------------*/
static void snapback_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			In the air only										*/
/*==============================================================*/
static void sp_jsphere( PLAYER_WORK *pl_w )
{
	int no ;

	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_dir_set( pl_w ) ;
		pl_gauge_add( pl_w, 5 ) ;
		pl_w->atck_no = ATCK_JUMP_MP ;
		pl_w->hit_flag = pl_w->atck_attr = 0 ;
		pl_w->unk_1c4 = 0 ;
		vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
		pl_w->spd.x /= 16.0f ;
		pl_w->spd.y /= 8.0f ;
		pl_w->acc.y /= 64.0f ;
		pl_w->acc.x = 0.0f ;
		no = 8 ;
		if( !pl_w->atck_level ) no = 7 ;
		pl_char_set( pl_w, Char_Extra, no ) ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ) pl_w->pos.y = pl_w->ground_y ;
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
	else if( pl_w->cg_type ){
		shl2a_04_set( pl_w, pl_w->cg_type - 1 ) ;
		pl_w->cg_type = 0 ;
	}
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_move( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A kick that leaps off the wall						*/
/****************************************************************/
void pl2a_t_kick( PLAYER_WORK *pl_w )
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
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->pos.y += DOT_Y( 64.0f ) ;
	spd = 30.0f ;
	if( !pl_w->pl_dir ) spd = -30.0f ;
	pl_w->spd.x = spd ;
	pl_w->spd.y = -DOT_Y( 0.125f ) ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->atck_no = pl2a_at_t_kick ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 24 ) ;
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
		pl_char_set( pl_w, Char_Special, 25 ) ;
	}
}

/*--------------------------------------------------------------*/
static void t_kick_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stop_stand_ret( pl_w ) ;
}

/* the throw for each stance and button, plus one; 0 for none */
static int catch_tbl[4][2] = {
	{ 2, 1 }, { 0, 0 }, { 2, 0 }, { 0, 0 },
} ;

/****************************************************************/
PLAYER_WORK *pl2a_catch_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	int catch_no ;
	Uint32 sw ;

	em = 0 ;
	if( ( catch_no = catch_tbl[pl_w->pl_stance][pl_w->atck_pk] ) ){
		if( ( sw = pl_w->atck_lever & ( KEY_FORWARD | KEY_BACK ) ) ){
			pl_w->dir = sw >> 10 ;
			if( pl_w->atck_level == ATCK_H ){
				if( ( em = pl_catch_ck( pl_w ) ) ) pl_w->catch_no = catch_no - 1 ;
			}
		}
	}
	return em ;
}

/* where the opponent is held: for the kick throw, then the punch throws */
static VEC3 catch_ofs[2] = {
	{ -80.0f, DOT_Y( 88.0f ), 0.0f },
	{ 0.0f, DOT_Y( 88.0f ), 0.0f },
} ;

/****************************************************************/
/*			Start the throw that pl2a_catch_ck chose			*/
/****************************************************************/
void pl2a_catch_set( PLAYER_WORK *pl_w )
{
	int i ;

	if( pl_w->dir & 1 ){
		pl_w->disp_dir ^= 1 ;
		pl_w->pl_dir ^= 1 ;
	}
	if( !( pl_w->catch_no & CATCH_NO_MASK ) ) pls2a_00_set( pl_w, 1 ) ;
	i = 0 ;
	if( pl_w->catch_no & CATCH_NO_MASK ){
		i = 1 ;
		pl_w->timer0 = 64 ;
	}
	nage_hit_set( pl_w, &catch_ofs[i] ) ;
	pl_w->hit_stop = 10 ;
	scr_excep_set( pl_w, 0, 5 ) ;
	pl_char_set( pl_w, Char_Catch, pl_w->catch_no & CATCH_NO_MASK ) ;
	catch_gauge_add( pl_w ) ;
}

/****************************************************************/
void pl2a_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_w->catch_flag = 1 ;
	if( !( pl_w->catch_no & CATCH_NO_MASK ) ){
		if( pl_char_move( pl_w ) < 0 ){
			if( pl_w->pl_stance == STANCE_AIR ) pl_jump_ret( pl_w ) ;
			else stand_ret( pl_w ) ;
		} else if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			em = pl_w->catch_w ;
			em->dm_hit_w = pl_w ;
			em->catch_type = 16 ;
			pl_w->atck_no = em->atck_no = pl2a_at_throw_k ;
			em->disp_dir = em->pl_dir = pl_w->pl_dir ^ 1 ;
			scr_excep_clr() ;
		}
		return ;
	}
	if( !pl_w->routine_2 ){
		pl_char_move( pl_w ) ;
		em = pl_w->catch_w ;
		if( -- pl_w->timer0 < 0 ){
			pl_w->routine_2 ++ ;
			em->dm_hit_w = pl_w ;
			em->catch_type = 16 ;
			pl_w->atck_no = em->atck_no = pl2a_at_throw_p ;
			em->disp_dir = em->pl_dir = pl_w->pl_dir ^ 1 ;
			scr_excep_clr() ;
			pl_char_set( pl_w, Char_Catch, 2 ) ;
		} else {
			if( pl_w->cg_type ){
				pl_w->cg_type = 0 ;
				em->atck_no = pl2a_at_throw_p_hit ;
				catch_damage( pl_w ) ;
			}
			if( pl_w->cg_prm ){
				pl_w->cg_prm = 0 ;
				pls2a_00_set( pl_w, 2 ) ;
			}
		}
		return ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		if( pl_w->pl_stance == STANCE_AIR ){
			pl_w->spd.x = pl_w->acc.x = pl_w->spd.y = 0.0f ;
			pl_w->acc.y = -DOT_Y( 0.375f ) ;
			pl_jump_ret( pl_w ) ;
		} else stand_ret( pl_w ) ;
	}
}

/****************************************************************/
/*			Drifting with the stick, and floating down with		*/
/*			up held, while in the air							*/
/****************************************************************/
void pl2a_float( PLAYER_WORK *pl_w )
{
	Uint16 sw ;
	Float x ;

	if( ( sw = pl_w->pl_new_lvbt ) & 0x0c00 ){
		x = DOT_X( 2.0f ) ;
		if( pl_w->sjump_flag ) x = DOT_X( 0.5f ) ;
		if( sw & KEY_FORWARD ) x = -x ;
		pl_w->pos.x += x ;
	}
	if( pl_w->spd.y > 0.0f ) return ;
	if( pl_w->fly_flag ) return ;
	if( ground_ck( pl_w ) ){
		if( sw & KEY_UP ){
			if( !( pl_w->cg_atck_flag & CG_ATCK_ALL ) ){
				if( -- pl_w->cg_ctr <= 0 ) pl_w->cg_ctr = 1 ;
			}
			pl_w->spd.y = -DOT_Y( 2.0f ) ;
			pl_w->acc.y = 0.0f ;
		}
		if( !( ~pl_w->pl_new_lvbt & pl_w->pl_last_lvbt & KEY_UP ) ) return ;
	}
	pl_w->spd.y = -DOT_Y( 4.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
}

/*==============================================================*/
/*			Back to standing, at rest							*/
/*==============================================================*/
static void stand_ret( PLAYER_WORK *pl_w )
{
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_stand_ret( pl_w ) ;
}

/****************************************************************/
void pl2a_hold( PLAYER_WORK *pl_w )
{
	catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void pl2a_sp_unison( PLAYER_WORK *pl_w )
{
	static Uint8 unison_sp[3] = { pl2a_sp_lstorm, pl2a_sp_lstorm, pl2a_sp_hail } ;

	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	pl_w->pl_sp_type = unison_sp[pl_w->pl_personal] ;
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl2a_sp_gattai( PLAYER_WORK *pl_w )
{
	static Uint8 gattai_sp[3] = { pl2a_sp_lstorm, pl2a_sp_lstorm, pl2a_sp_hail } ;

	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	pl_w->pl_sp_type = gattai_sp[pl_w->pl_personal] ;
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl2a_sp_assist( PLAYER_WORK *pl_w )
{
	static Uint8 assist_sp[3][2] = {
		{ pl2a_sp_typhoon, 1 }, { pl2a_sp_lattack_a, 0 }, { pl2a_sp_dtyphoon, 1 },
	} ;

	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	pl_w->pl_sp_type = assist_sp[pl_w->pl_personal][0] ;
	pl_w->atck_level = assist_sp[pl_w->pl_personal][1] ;
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void pl2a_sp_zero( PLAYER_WORK *pl_w )
{
	static Uint8 zero_sp[3][2] = {
		{ pl2a_sp_typhoon, 1 }, { pl2a_sp_lattack_a, 0 }, { pl2a_sp_dtyphoon, 1 },
	} ;

	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	pl_w->pl_sp_type = zero_sp[pl_w->pl_personal][0] ;
	pl_w->atck_level = zero_sp[pl_w->pl_personal][1] ;
	act_st_set( pl_w, N_SPECIAL ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl2a.c" End of File	*/

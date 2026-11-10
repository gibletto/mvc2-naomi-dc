/* Silver Samurai (character 2f): command checks, normal attacks, appearance, win poses, special
   moves and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl2f_def.h"

/* elem_timer as a word of the local work, the way elem_k_set hands it to Timer_Gauge_Set */
#define TM_ELEM		6
/* shuriken_timer: no new shuriken until it runs out */
#define TM_SHURIKEN		19
/* sound_timer: frames to the next sound */
#define TM_SOUND	13
/* elem_lv: the element's level */
#define ELEM_LV		21
/* the flash, flash_step, flash_timer and flash_col */
#define FL_STEP		0	/* its stage */
#define FL_TIME		16	/* frames to its next colour */
#define FL_COL		17	/* its colour */

void pl2f_main( PLAYER_WORK *pl_w );
void pl2f_com_ck( PLAYER_WORK *pl_w );
void pl2f_atck_set( PLAYER_WORK *pl_w );
void pl2f_atck( PLAYER_WORK *pl_w );
void pl2f_dash( PLAYER_WORK *pl_w );
void pl2f_noki( PLAYER_WORK *pl_w );
void pl2f_appear( PLAYER_WORK *pl_w );
void pl2f_win( PLAYER_WORK *pl_w );
void pl2f_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl2f_catch_ck( PLAYER_WORK *pl_w );
void pl2f_catch_set( PLAYER_WORK *pl_w );
void pl2f_catch( PLAYER_WORK *pl_w );
void pl2f_hold( PLAYER_WORK *pl_w );
int pl2f_delay_ck( PLAYER_WORK *pl_w );
void pl2f_t_kick( PLAYER_WORK *pl_w );
void pl2f_upper( PLAYER_WORK *pl_w );
void pl2f_sp_unison( PLAYER_WORK *pl_w );
void pl2f_sp_gattai( PLAYER_WORK *pl_w );
void pl2f_sp_assist( PLAYER_WORK *pl_w );
void pl2f_sp_zero( PLAYER_WORK *pl_w );
void pl2f_tlocal_init( PLAYER_WORK *pl_w );

/* the attack's kind for each element */
static Sint16 elem_kind_tbl[4] = { 0, 3, 1, 2 } ;

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
static Sint8 sj_hp_tbl[4] = { -1, -1, -1,  2 } ;
static Sint8 sj_lk_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 sj_mk_tbl[4] = { -1,  2, -1,  2 } ;
static Sint8 sj_hk_tbl[4] = { -1, -1, -1, -1 } ;

static Sint8 *nml_atck_tbl[6] = {
	lp_tbl, mp_tbl, hp_tbl, lk_tbl, mk_tbl, hk_tbl,
} ;
static Sint8 *jump_atck_tbl[12] = {
	j_lp_tbl, j_mp_tbl, j_hp_tbl, j_lk_tbl, j_mk_tbl, j_hk_tbl,
	sj_lp_tbl, sj_mp_tbl, sj_hp_tbl, sj_lk_tbl, sj_mk_tbl, sj_hk_tbl,
} ;

/* commands: time or steps, kind, the special move, then the stick and buttons */
static Uint16 com_shuriken[] = { 3, 0, pl2f_sp_shuriken, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Shuriken */
static Uint16 com_raimei[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl2f_sp_raimei, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Raimeiken */
static Uint16 com_rapid_rapid[] = { 4, 10, pl2f_sp_rapid, 0, KEY_PUNCH } ;	/* Hyakuretsutou */
static Uint16 com_dash[] = { 3, COM_GROUND | COM_FREE | COM_NO_SW, pl2f_sp_dash, 0, KEY_FORWARD, 0x0000, KEY_FORWARD, KEY_PUNCH } ;	/* Running Hyakuretsutou */
/* not used */
static Uint16 com_bushin[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl2f_sp_bushin, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;
static Uint16 com_elem_k[] = { 3, COM_GROUND | COM_HYPER, pl2f_sp_elem_k, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Toki-Hyo (LK, ice) and Toki-En (HK, fire) (hyper) */
static Uint16 com_elem_p[] = { 3, COM_GROUND | COM_HYPER, pl2f_sp_elem_p, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Toki-Rai (lightning element, hyper) */
static Uint16 com_hshuriken[] = { 3, COM_HYPER | COM_ALL_SW, pl2f_sp_hshuriken, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Cho-Shuriken (hyper) */
static Uint16 com_rapid[] = { 3, COM_GROUND, pl2f_sp_rapid, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_PUNCH } ;	/* Hyakuretsutou */

static void ( *pl2f_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl2f_main,
	pl2f_com_ck,
	pl2f_atck_set,
	pl2f_atck,
	pl2f_dash,
	pl2f_noki,
	pl2f_appear,
	pl2f_win,
	pl2f_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl2f_catch_ck,	/* returns a value */
	pl2f_catch_set,
	pl2f_catch,
	pl2f_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl2f_delay_ck,
	pl2f_t_kick,
	pl2f_upper,
	pl2f_sp_unison,
	pl2f_sp_gattai,
	pl2f_sp_assist,
	pl2f_sp_zero,
	0,
	0,
	0,
	pl2f_tlocal_init,
} ;

static int hshuriken_ck( PLAYER_WORK *pl_w );
static int raimei_ck( PLAYER_WORK *pl_w );
static int rapid_ck( PLAYER_WORK *pl_w );
static int shuriken_ck( PLAYER_WORK *pl_w );
static int elem_p_ck( PLAYER_WORK *pl_w );
static int elem_k_ck( PLAYER_WORK *pl_w );
static int dash_ck( PLAYER_WORK *pl_w );
static int rapid_rapid_ck( PLAYER_WORK *pl_w );
static int chouhatsu_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static int hshuriken_delay_ck( PLAYER_WORK *pl_w );
static int raimei_delay_ck( PLAYER_WORK *pl_w );
static void elem_cnt( PLAYER_WORK *pl_w );
static void flash_ck( PLAYER_WORK *pl_w );
static void flash_color( PLAYER_WORK *pl_w, Uint32 base );
static void atck_voice( PLAYER_WORK *pl_w );
static void atck_end( PLAYER_WORK *pl_w );

/****************************************************************/
void pl2f_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl2f_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl2f_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( hshuriken_ck( pl_w ) ) return ;
	if( raimei_ck( pl_w ) ) return ;
	if( rapid_ck( pl_w ) ) return ;
	if( shuriken_ck( pl_w ) ) return ;
	if( elem_p_ck( pl_w ) ) return ;
	if( elem_k_ck( pl_w ) ) return ;
	if( dash_ck( pl_w ) ) return ;
	if( rapid_rapid_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
/*			In the air only with element 1, and once a jump		*/
/*			unless super jumping								*/
/*==============================================================*/
static int hshuriken_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_hshuriken, &pl_w->com_w6 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	if( pl_w->pl_stance == STANCE_AIR && PL2F_LW( pl_w )->elem != 1 ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w6 ) ;
	pl_w->pl_sp_type = pl2f_sp_hshuriken ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/* the hyper combo for each element */
static Uint8 raimei_tbl[4] = { pl2f_sp_raimei, pl2f_sp_raimei, pl2f_sp_hyouga, pl2f_sp_homura } ;

/*==============================================================*/
static int raimei_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_raimei, &pl_w->com_w0 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->pl_sp_type = raimei_tbl[PL2F_LW( pl_w )->elem] ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static int rapid_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_rapid, &pl_w->com_w8 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w8 ) ;
	pl_w->pl_sp_type = pl2f_sp_rapid ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			The shuriken, on the ground or in the air			*/
/*==============================================================*/
static int shuriken_ck( PLAYER_WORK *pl_w )
{
	Uint8 no ;

	if( !com_nml_ck( pl_w, com_shuriken, &pl_w->com_w ) ) return 0 ;
	if( pl_w->tlocal[TM_SHURIKEN] ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w ) ;
	no = pl2f_sp_jshuriken ;
	if( pl_w->pl_stance != STANCE_AIR ) no = pl2f_sp_shuriken ;
	pl_w->pl_sp_type = no ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int elem_p_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_elem_p, &pl_w->com_w7 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w7 ) ;
	pl_w->pl_sp_type = pl2f_sp_elem_p ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static int elem_k_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_elem_k, &pl_w->com_w10 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w10 ) ;
	pl_w->pl_sp_type = pl2f_sp_elem_k ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
/*			Only during the Hyakuretsutou, while dash_timer		*/
/*			runs												*/
/*==============================================================*/
static int dash_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->act_st != N_SPECIAL ) return 0 ;
	if( pl_w->pl_sp_type != pl2f_sp_rapid ) return 0 ;
	if( PL2F_LW( pl_w )->dash_timer <= 0 ) return 0 ;
	if( !com_nml_ck( pl_w, com_dash, &pl_w->com_w2 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->pl_sp_type = pl2f_sp_dash ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			On the ground only									*/
/*==============================================================*/
static int rapid_rapid_ck( PLAYER_WORK *pl_w )
{
	if( !com_renda_ck( pl_w, com_rapid_rapid, &pl_w->com_w1 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR ) return 0 ;
	if( pl_w->pl_stance == STANCE_DOWN ) return 0 ;
	if( !sp_ok_ck( pl_w ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->pl_sp_type = pl2f_sp_rapid ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 13 ) ) return 0 ;
	pl_w->pl_sp_type = pl2f_sp_chouhatsu ;
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
	pl_w->pl_sp_type = pl2f_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl2f_delay_ck( PLAYER_WORK *pl_w )
{
	if( hshuriken_delay_ck( pl_w ) || raimei_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static int hshuriken_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_hshuriken, &pl_w->com_w6 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w6 ) ;
	pl_w->delay_sp_type = pl2f_sp_hshuriken ;
	return 1 ;
}

static Uint8 raimei_delay_tbl[4] = { pl2f_sp_raimei, pl2f_sp_raimei, pl2f_sp_hyouga, pl2f_sp_homura } ;

/*==============================================================*/
static int raimei_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_raimei, &pl_w->com_w0 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->delay_sp_type = raimei_delay_tbl[PL2F_LW( pl_w )->elem] ;
	return 1 ;
}

/****************************************************************/
/*			Counts down the element and the flash, then			*/
/*			flashes												*/
/****************************************************************/
void pl2f_main( PLAYER_WORK *pl_w )
{
	if( PL2F_LW( pl_w )->elem_flag ) elem_cnt( pl_w ) ;
	if( PL2F_LW( pl_w )->dash_timer ) PL2F_LW( pl_w )->dash_timer -- ;
	if( pl_w->tlocal[TM_SHURIKEN] ) pl_w->tlocal[TM_SHURIKEN] -- ;
	flash_ck( pl_w ) ;
}

/*==============================================================*/
/*			The element runs out; hit stop holds its time		*/
/*==============================================================*/
static void elem_cnt( PLAYER_WORK *pl_w )
{
	if( pl_w->hit_stop ) PL2F_LW( pl_w )->elem_timer ++ ;
	if( -- PL2F_LW( pl_w )->elem_timer <= 0 ){
		PL2F_LW( pl_w )->elem_flag = 0 ;
		PL2F_LW( pl_w )->elem = 0 ;
		PL2F_LW( pl_w )->elem_timer = 0 ;
		pl_w->atck_pow = 0 ;
		pl_w->def_pow = 0 ;
		pl_w->armor_flag = 0 ;
	} else if( PL2F_LW( pl_w )->elem == 2 ){
		if( !pl_w->armor_flag && pl_w->tlocal[ELEM_LV] < 2 ) pl_w->armor_timer = 3 ;
	}
}

/*==============================================================*/
/*			Steps the flash: two colours with random pauses,	*/
/*			then from stage 2 a run of five						*/
/*==============================================================*/
static void flash_ck( PLAYER_WORK *pl_w )
{
	Uint32 base ;

	if( pl_w->tlocal[FL_STEP] < 2 ){
		if( !pl_w->tlocal[FL_STEP] ){
			pl_w->tlocal[FL_STEP] ++ ;
			PL2F_LW( pl_w )->unk_48 = 0 ;
			pl_w->tlocal[FL_TIME] = 1 ;
			pl_w->tlocal[FL_COL] = 0 ;
		}
		if( -- pl_w->tlocal[FL_TIME] < 0 ){
			pl_w->tlocal[FL_TIME] = 1 ;
			if( pl_w->tlocal[FL_COL] > 1 ){
				pl_w->tlocal[FL_COL] = 0 ;
				if( !( Rnd() & 15 ) ){
					pl_w->tlocal[FL_STEP] ++ ;
					pl_w->tlocal[FL_TIME] = 4 ;
					pl_w->tlocal[FL_COL] = 0 ;
				}
			}
			flash_color( pl_w, 0 ) ;
		}
	} else if( -- pl_w->tlocal[FL_TIME] < 0 ){
		pl_w->tlocal[FL_TIME] = 1 ;
		if( pl_w->tlocal[FL_COL] > 4 ){
			pl_w->tlocal[FL_STEP] = 1 ;
			pl_w->tlocal[FL_TIME] = 1 ;
			pl_w->tlocal[FL_COL] = 0 ;
			base = 0 ;
		} else base = 2 ;
		flash_color( pl_w, base ) ;
	}
}

/*==============================================================*/
static void flash_color( PLAYER_WORK *pl_w, Uint32 base )
{
	ef_color_set( pl_w, 0, pl_w->pl_color * 8 + base + pl_w->tlocal[FL_COL] ++, 1 ) ;
}

/* for each attack: its number, strength, animation and move */
static Uint8 nml_no_tbl[12] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 } ;
static Uint8 nml_str_tbl[3] = { 0, 1, 2 } ;
static Uint8 nml_char_tbl[12] = { 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2 } ;
static Uint8 nml_grp_tbl[12] = { Char_Stand_P, Char_Stand_P, Char_Stand_P, Char_Stand_K, Char_Stand_K, Char_Stand_K, Char_Crouch_P, Char_Crouch_P, Char_Crouch_P, Char_Crouch_K, Char_Crouch_K, Char_Crouch_K } ;
static Uint8 nml_move_tbl[12] = { 2, 3, 4, 0, 0, 5, 1, 0, 0, 0, 0, 0 } ;
static Uint8 jump_no_tbl[6] = { 12, 13, 14, 15, 16, 17 } ;
static Uint8 jump_str_tbl[3] = { 0, 1, 2 } ;
static Uint8 jump_char_tbl[6] = { 0, 1, 2, 0, 1, 2 } ;
static Uint8 jump_grp_tbl[6] = { Char_Jump_P, Char_Jump_P, Char_Jump_P, Char_Jump_K, Char_Jump_K, Char_Jump_K } ;

/*==============================================================*/
/*			Normal attacks; element 1 uses the super jump's		*/
/*			tables												*/
/*==============================================================*/
static void nml_atck( PLAYER_WORK *pl_w )
{
	Uint8 no ;
	Sint8 *tbl ;

	pl_brake_set( pl_w ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	no = pl_w->nml_no ;
	if( pl_w->pl_stance == STANCE_CROUCH ) no += 6 ;
	if( pl_w->atck_pk ) no += 3 ;
	PL2F_LW( pl_w )->nml_move = nml_move_tbl[no] ;
	tbl = nml_atck_tbl[no % 6] ;
	if( PL2F_LW( pl_w )->elem == 1 ) tbl = jump_atck_tbl[no % 6 + 6] ;
	pl_w->chain_tbl = tbl ;
	pl_w->nml_level = nml_str_tbl[pl_w->nml_no] ;
	pl_w->atck_no = nml_no_tbl[no] ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, nml_grp_tbl[no], nml_char_tbl[no] ) ;
	atck_voice( pl_w ) ;
}

/*==============================================================*/
static void jump_atck( PLAYER_WORK *pl_w )
{
	Uint8 no ;
	Sint8 *tbl ;

	no = pl_w->nml_no ;
	if( pl_w->atck_pk ) no += 3 ;
	pl_w->nml_level = jump_str_tbl[pl_w->nml_no] ;
	pl_w->atck_no = jump_no_tbl[no] ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, jump_grp_tbl[no], jump_char_tbl[no] ) ;
	if( pl_w->sjump_flag ) no += 6 ;
	tbl = jump_atck_tbl[no] ;
	if( PL2F_LW( pl_w )->elem == 1 ) tbl = jump_atck_tbl[no % 6 + 6] ;
	pl_w->chain_tbl = tbl ;
	if( no != 3 && no != 9 ) atck_voice( pl_w ) ;
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
void pl2f_atck_set( PLAYER_WORK *pl_w )
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
static void sj_move( PLAYER_WORK *pl_w )
{
	pl_jump_hit_ck( pl_w ) ;
	pl_sjump_ctrl( pl_w ) ;
	jump_move( pl_w ) ;
}

static void nml_move_nml( PLAYER_WORK *pl_w );
static void nml_move_crouch_lp( PLAYER_WORK *pl_w );
static void nml_move_stand_lp( PLAYER_WORK *pl_w );
static void nml_move_stand_mp( PLAYER_WORK *pl_w );
static void nml_move_stand_hp( PLAYER_WORK *pl_w );
static void nml_move_stand_hk( PLAYER_WORK *pl_w );

/*==============================================================*/
static void nml_move( PLAYER_WORK *pl_w )
{
	static void ( *nml_move_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_move_nml, nml_move_crouch_lp, nml_move_stand_lp, nml_move_stand_mp, nml_move_stand_hp, nml_move_stand_hk,
	} ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_brake_stop( pl_w ) ;
	nml_move_jmp[PL2F_LW( pl_w )->nml_move]( pl_w ) ;
}

/*==============================================================*/
static void nml_move_nml( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) atck_end( pl_w ) ;
}

/*==============================================================*/
/*			The crouching light punch chains into a second		*/
/*			hit													*/
/*==============================================================*/
static void nml_move_crouch_lp( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) atck_end( pl_w ) ;
	else if( pl_w->cg_type ){
		pl_w->hit_flag = 0 ;
		if( ( pl_w->pl_sw_now | pl_w->pl_stop_now ) & 0x200 ){
			pl_w->pl_stop_now = 0 ;
			pl_w->atck_no = pl2f_at_crouch_lp_2 ;
			HIT_RESET( pl_w ) ;
			pl_char_set( pl_w, Char_Crouch_P, 3 ) ;
			atck_voice( pl_w ) ;
		}
	}
}

/*==============================================================*/
/*			The light punch repeats, two ways in turn			*/
/*==============================================================*/
static void nml_move_stand_lp( PLAYER_WORK *pl_w )
{
	Sint8 no ;
	Uint8 atck ;

	if( pl_char_move( pl_w ) < 0 ) atck_end( pl_w ) ;
	else if( pl_w->cg_type ){
		pl_w->hit_flag = 0 ;
		if( ( pl_w->pl_sw_now | pl_w->pl_stop_now ) & 0x200 ){
			pl_w->pl_stop_now = 0 ;
			atck = 0 ;
			no = 0 ;
			if( pl_w->routine_3 ^= 1 ){
				atck = 25 ;
				no = 3 ;
			}
			pl_w->atck_no = atck ;
			HIT_RESET( pl_w ) ;
			pl_char_set( pl_w, Char_Stand_P, no ) ;
			atck_voice( pl_w ) ;
		}
	}
}

/*==============================================================*/
static void nml_move_stand_mp( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		PL2F_LW( pl_w )->nml_move = 0 ;
		pl_w->atck_no = pl2f_at_stand_mp_2 ;
		HIT_RESET( pl_w ) ;
		atck_voice( pl_w ) ;
	}
}

/*==============================================================*/
/*			The heavy punch can follow with a lunge				*/
/*==============================================================*/
static void nml_move_stand_hp( PLAYER_WORK *pl_w )
{
	Float spd ;

	if( pl_char_move( pl_w ) < 0 ) atck_end( pl_w ) ;
	else if( pl_w->cg_type && !pl_turn_ck( pl_w ) && !pl_crouch_ck( pl_w )
	 && ( ( pl_w->pl_sw_now | pl_w->pl_stop_now ) & 0x100 ) ){
		pl_w->pl_stop_now = 0 ;
		spd = 160.0f ;
		if( !pl_w->pl_dir ) spd = -160.0f ;
		pl_w->pos.x += spd ;
		pl_w->atck_no = pl2f_at_stand_hp_2 ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Stand_P, 4 ) ;
		atck_voice( pl_w ) ;
	}
}

/*==============================================================*/
/*			The heavy kick hops									*/
/*==============================================================*/
static void nml_move_stand_hk( PLAYER_WORK *pl_w )
{
	Float spd, acc ;

	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->jump_com_cnt = 0 ;
		pl_w->jump_atk_cnt = 0 ;
		spd = -DOT_X( 4.0f ) ;
		acc = 0.104166664f ;
		if( pl_w->pl_dir ){
			spd = DOT_X( 4.0f ) ;
			acc = -0.104166664f ;
		}
		pl_w->spd.x = spd ;
		pl_w->acc.x = acc ;
		pl_w->spd.y += DOT_Y( 5.0f ) ;
		pl_w->acc.y += -DOT_Y( 0.375f ) ;
	} else if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.y = pl_w->acc.y = 0.0f ;
		pl_se_req( pl_w, 52 ) ;
	}
	if( pl_char_move( pl_w ) < 0 ) atck_end( pl_w ) ;
}

/*==============================================================*/
static void nml_move_dash( PLAYER_WORK *pl_w )
{
	pl_smoke_set( pl_w ) ;
	nml_move( pl_w ) ;
}

/****************************************************************/
void pl2f_atck( PLAYER_WORK *pl_w )
{
	static void ( *atck_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_move, jump_move, sj_move, nml_move_dash,
	} ;

	atck_jmp[pl_w->atck_type]( pl_w ) ;
}

/****************************************************************/
/*			The launcher										*/
/****************************************************************/
void pl2f_upper( PLAYER_WORK *pl_w )
{
	Uint8 no ;

	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_brake_set( pl_w ) ;
		pl_gauge_add( pl_w, 5 ) ;
		pl_w->pl_stance = STANCE_CROUCH ;
		no = 61 ;
		if( PL2F_LW( pl_w )->elem == 3 ) no = 93 ;
		pl_w->atck_no = no ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, 8 ) ;
		atck_voice( pl_w ) ;
	}
	if( pl_w->atck_type == 3 ) pl_smoke_set( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_brake_stop( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ) atck_end( pl_w ) ;
}

static void dash_init( PLAYER_WORK *pl_w );
static void dash_move( PLAYER_WORK *pl_w );
static void dash_end( PLAYER_WORK *pl_w );

/****************************************************************/
void pl2f_dash( PLAYER_WORK *pl_w )
{
	static void ( *dash_jmp[])( PLAYER_WORK *pl_w ) = {
		dash_init, dash_move, dash_end,
	} ;

	dash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dash_init( PLAYER_WORK *pl_w )
{
	Float spd ;

	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 32 ;
		spd = -10.0f ;
		if( pl_w->pl_dir ) spd = 10.0f ;
		pl_w->spd.x = spd ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
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
	if( !-- pl_w->timer0 ) pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void dash_end( PLAYER_WORK *pl_w )
{
	atck_end( pl_w ) ;
}

static void noki_init( PLAYER_WORK *pl_w );
static void noki_move( PLAYER_WORK *pl_w );
static void noki_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash, a hop								*/
/****************************************************************/
void pl2f_noki( PLAYER_WORK *pl_w )
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
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		spd = DOT_X( 8.0f ) ;
		if( pl_w->pl_dir ) spd = -DOT_X( 8.0f ) ;
		pl_w->spd.x = spd ;
		pl_w->spd.y = DOT_Y( 4.0f ) ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
	}
}

/*--------------------------------------------------------------*/
static void noki_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->acc.x = -pl_w->spd.x / 8.0f ;
		pl_char_set( pl_w, Char_Dash, 3 ) ;
		pl_land_ef_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void noki_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) atck_end( pl_w ) ;
}

static void appear_init( PLAYER_WORK *pl_w );
static void appear_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl2f_appear( PLAYER_WORK *pl_w )
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
	pl_w->disp_flag = 1 ;
	pl_char_set( pl_w, Char_Appear, 0 ) ;
}

/*--------------------------------------------------------------*/
static void appear_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_w->routine_1 ++ ;
}

/* the first of each round end's two win poses */
static Uint8 win_tbl[5] = { 0, 2, 0, 4, 4 } ;

/****************************************************************/
void pl2f_win( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, win_tbl[pl_w->type] + ( Rnd() & 1 ) ) ;
	} else {
		pl_char_move( pl_w ) ;
		if( pl_win_end_ck( pl_w ) ) atck_end( pl_w ) ;
	}
}

static void sp_shuriken( PLAYER_WORK *pl_w );
static void sp_jshuriken( PLAYER_WORK *pl_w );
static void sp_raimei( PLAYER_WORK *pl_w );
static void sp_rapid( PLAYER_WORK *pl_w );
static void sp_bushin( PLAYER_WORK *pl_w );
static void sp_hyouga( PLAYER_WORK *pl_w );
static void sp_dash( PLAYER_WORK *pl_w );
static void sp_hrapid( PLAYER_WORK *pl_w );
static void sp_hshuriken( PLAYER_WORK *pl_w );
static void sp_09( PLAYER_WORK *pl_w );
static void sp_elem_k( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );
static void sp_crouch_hp( PLAYER_WORK *pl_w );
static void sp_homura( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Special moves and hyper combos; the three that		*/
/*			take on an element are one routine					*/
/****************************************************************/
void pl2f_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *pl_sp_tbl[])( PLAYER_WORK *pl_w ) = {
		sp_shuriken, sp_jshuriken, sp_raimei, sp_rapid, sp_bushin, sp_hyouga, sp_dash, sp_hrapid, sp_hshuriken, sp_09, sp_elem_k, sp_elem_k, sp_elem_k,
		sp_chouhatsu, sp_snapback, sp_crouch_hp, sp_homura,
	} ;

	pl_sp_tbl[pl_w->pl_sp_type]( pl_w ) ;
}

/*==============================================================*/
/*			The shuriken										*/
/*==============================================================*/
static void sp_shuriken( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_dir_set( pl_w ) ;
		pl_sp_ef_set( pl_w ) ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_gauge_add( pl_w, 10 ) ;
		pl_w->atck_no = pl2f_at_shuriken ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Special, PL2F_LW( pl_w )->elem + 56 ) ;
		pl_voice_req( pl_w, 4 ) ;
	}
	if( pl_char_move( pl_w ) < 0 ) atck_end( pl_w ) ;
	else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_voice_req( pl_w, 32 ) ;
		shl2f_00_set( pl_w, 0 ) ;
		pls2f_00_set( pl_w, 0 ) ;
		pl_w->unk_27b = 0 ;
		pl_w->cancel_timer = 16 ;
	}
}

/*==============================================================*/
/*			The shuriken in the air: the jump slows while it	*/
/*			is thrown											*/
/*==============================================================*/
static void sp_jshuriken( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_dir_set( pl_w ) ;
		pl_w->atck_no = pl2f_at_jshuriken ;
		HIT_RESET( pl_w ) ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_gauge_add( pl_w, 10 ) ;
		pl_char_set( pl_w, Char_Special, PL2F_LW( pl_w )->elem + 60 ) ;
		pl_w->spd.x /= 16.0f ;
		pl_w->spd.y /= 8.0f ;
		pl_w->acc.y /= 64.0f ;
		pl_w->acc.x = 0.0f ;
		pl_voice_req( pl_w, 4 ) ;
	}
	if( pl_w->pos.y >= pl_w->ground_y - DOT_Y( 16.0f ) ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_jump_ret( pl_w ) ;
	} else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_voice_req( pl_w, 32 ) ;
		shl2f_00_set( pl_w, 0 ) ;
		pls2f_00_set( pl_w, 0 ) ;
		pl_w->unk_27b = 0 ;
		pl_w->cancel_timer = 16 ;
	}
}

static void raimei_init( PLAYER_WORK *pl_w );
static void raimei_wait( PLAYER_WORK *pl_w );
static void raimei_move( PLAYER_WORK *pl_w );
static void raimei_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Raimeiken (hyper)									*/
/*==============================================================*/
static void sp_raimei( PLAYER_WORK *pl_w )
{
	static void ( *raimei_jmp[])( PLAYER_WORK *pl_w ) = {
		raimei_init, raimei_wait, raimei_move, raimei_end,
	} ;

	raimei_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void raimei_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = pl2f_at_raimei ;
	HIT_RESET( pl_w ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->timer0 = 128 ;
	pl_w->timer1 = 30 ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_char_set( pl_w, Char_Special, 4 ) ;
	pl_voice_req( pl_w, 20 ) ;
	raimei_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void raimei_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	pl_w->timer0 -- ;
	if( pl_w->cg_type ){
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_color_set( pl_w, 1 ) ;
		pl_w->tlocal[FL_STEP] = 0 ;
		ofs.x = DOT_X( 25.0f ) ;
		ofs.y = DOT_Y( 87.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void raimei_move( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 < 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, 5 ) ;
	} else {
		if( lever_sw_renda_ck( pl_w ) && -- pl_w->timer1 >= 0 ) pl_w->timer0 ++ ;
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pl_voice_req( pl_w, 31 ) ;
			pls2f_00_set( pl_w, 1 ) ;
		}
		if( pl_w->cg_prm ){
			pl_w->cg_prm = 0 ;
			shl2f_02_start( pl_w ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void raimei_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_voice_req( pl_w, 43 ) ;
		atck_end( pl_w ) ;
	}
}

static void rapid_init( PLAYER_WORK *pl_w );
static void rapid_move( PLAYER_WORK *pl_w );
static void rapid_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Hyakuretsutou, held for longer by the computer		*/
/*==============================================================*/
static void sp_rapid( PLAYER_WORK *pl_w )
{
	static void ( *rapid_jmp[])( PLAYER_WORK *pl_w ) = {
		rapid_init, rapid_move, rapid_end,
	} ;

	rapid_jmp[pl_w->routine_2]( pl_w ) ;
}

/* its animation for each element and strength */
static Uint8 rapid_char_tbl[4][2] = {
	{ 0x29, 0x2b }, { 0x29, 0x2b }, { 0x2d, 0x2f }, { 0x31, 0x33 },
} ;

/*==============================================================*/
static void rapid_init( PLAYER_WORK *pl_w )
{
	Sint16 time ;

	pl_w->routine_2 ++ ;
	PL2F_LW( pl_w )->dash_timer = 4 ;
	pl_w->atck_no = pl2f_at_rapid ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->type = 0 ;
	PL2F_LW( pl_w )->unk_28 = 96 ;
	time = 120 ;
	if( !pl_w->cpu_flag || pl_w->team_no ) time = 30 ;
	pl_w->timer0 = time ;
	pl_gauge_add( pl_w, 10 ) ;
	pl_char_set( pl_w, Char_Special, rapid_char_tbl[PL2F_LW( pl_w )->elem][pl_w->atck_level] ) ;
	pl_w->tlocal[TM_SOUND] = pl_w->cg_ctr ;
}

/*--------------------------------------------------------------*/
static void rapid_move( PLAYER_WORK *pl_w )
{
	PL2F_LW( pl_w )->dash_timer = 4 ;
	if( -- pl_w->tlocal[TM_SOUND] < 0 ){
		pl_w->tlocal[TM_SOUND] = 16 ;
		pl_voice_req( pl_w, 3 ) ;
	}
	pl_char_move( pl_w ) ;
	if( pl_w->cg_flag1 ){
		pl_w->atck_no = pl_w->cg_flag1 + PL2F_LW( pl_w )->elem ;
		HIT_RESET( pl_w ) ;
		pl_w->cg_flag1 = 0 ;
	}
	if( pl_w->cg_type < 0 ){
		pl_w->cg_type = 0 ;
		shl2f_01_set( pl_w ) ;
	}
	if( !sw_renda_ck( pl_w ) || pl_w->cpu_flag ){
		if( !-- pl_w->timer0 ){
			pl_w->routine_2 ++ ;
			pl_char_set( pl_w, Char_Special, pl_w->char_no.b.no + 1 ) ;
		}
	} else pl_w->timer0 = 30 ;
}

/*--------------------------------------------------------------*/
static void rapid_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) atck_end( pl_w ) ;
}

static void bushin_init( PLAYER_WORK *pl_w );
static void bushin_move( PLAYER_WORK *pl_w );
static void bushin_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Bushin, only while two effect works are free		*/
/*==============================================================*/
static void sp_bushin( PLAYER_WORK *pl_w )
{
	static void ( *bushin_jmp[])( PLAYER_WORK *pl_w ) = {
		bushin_init, bushin_move, bushin_end,
	} ;

	bushin_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void bushin_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->timer0 = 1 ;
	pl_w->timer1 = 44 ;
	pl_w->type = 3 ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 15 ) ;
	if( work_rest_cnt < 2 ) atck_end( pl_w ) ;
	else {
		pl_voice_req( pl_w, 30 ) ;
		shl2f_03_set( pl_w, pl_w ) ;
		pl_voice_req( pl_w, 21 ) ;
		bushin_move( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void bushin_move( PLAYER_WORK *pl_w )
{
	if( -- pl_w->timer1 < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, 16 ) ;
	}
	pl_char_move( pl_w ) ;
	if( pl_w->type >= 0 && -- pl_w->timer0 < 0 ){
		pl_w->timer0 = 1 ;
		if( work_rest_cnt >= 2 ) pl_w->type -- ;
	}
}

/*--------------------------------------------------------------*/
static void bushin_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) atck_end( pl_w ) ;
}

static void hyouga_init( PLAYER_WORK *pl_w );
static void hyouga_rise( PLAYER_WORK *pl_w );
static void hyouga_fall( PLAYER_WORK *pl_w );
static void hyouga_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Hyougaken (hyper): a leap to the middle of the		*/
/*			screen and a fall									*/
/*==============================================================*/
static void sp_hyouga( PLAYER_WORK *pl_w )
{
	static void ( *hyouga_jmp[])( PLAYER_WORK *pl_w ) = {
		hyouga_init, hyouga_rise, hyouga_fall, hyouga_end,
	} ;

	hyouga_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void hyouga_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->push_ng_timer = 2 ;
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->atck_no = pl2f_at_hyouga ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_sp_ef_set( pl_w ) ;
	scr_excep_set( pl_w, 1, 0 ) ;
	pl_w->spd.x = ( ( scr_work.scr_left + scr_work.scr_right ) / 2.0f - pl_w->pos.x ) / 16.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 4.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->timer0 = 16 ;
	pl_voice_req( pl_w, 3 ) ;
	pl_char_set( pl_w, Char_Super, 1 ) ;
}

/*--------------------------------------------------------------*/
static void hyouga_rise( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_w->push_ng_timer = 2 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !-- pl_w->timer0 ){
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->routine_2 ++ ;
		pl_color_set( pl_w, 1 ) ;
		pl_w->tlocal[FL_STEP] = 0 ;
		pl_w->spd.x = pl_w->acc.x = pl_w->spd.y = 0.0f ;
		ofs.x = -DOT_X( 8.0f ) ;
		ofs.y = DOT_Y( 104.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void hyouga_fall( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( !pl_w->cg_type ) pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_CROUCH ;
		ofs.x = -DOT_X( 8.0f ) ;
		if( pl_w->disp_dir ) ofs.x = -ofs.x ;
		ofs.x += pl_w->pos.x ;
		ofs.y = pl_w->pos.y + DOT_Y( 32.0f ) ;
		hitmark_set( &ofs, pl_w->pl_dir, 15, 0 ) ;
		pl_land_ef_set( pl_w ) ;
		shl2f_04_set( pl_w ) ;
		pl_se_req( pl_w, 49 ) ;
		scr_work.quake_level = 3 ;
		scr_work.quake_flag = 1 ;
	}
}

/*--------------------------------------------------------------*/
static void hyouga_end( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		scr_excep_clr() ;
		atck_end( pl_w ) ;
	}
}

static void sp_dash_init( PLAYER_WORK *pl_w );
static void sp_dash_move( PLAYER_WORK *pl_w );
static void sp_dash_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Running Hyakuretsutou: a dash out of the slashes	*/
/*==============================================================*/
static void sp_dash( PLAYER_WORK *pl_w )
{
	static void ( *sp_dash_jmp[])( PLAYER_WORK *pl_w ) = {
		sp_dash_init, sp_dash_move, sp_dash_end,
	} ;

	sp_dash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			Falls into sp_dash_move								*/
/*==============================================================*/
static void sp_dash_init( PLAYER_WORK *pl_w )
{
	Float spd ;

	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl2f_at_rapid ;
	HIT_RESET( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->type = 0 ;
	pl_w->pos.y = pl_w->ground_y ;
	spd = -DOT_X( 4.0f ) ;
	if( pl_w->pl_dir ) spd = DOT_X( 4.0f ) ;
	pl_w->spd.x = spd ;
	pl_w->acc.x = 0.0f ;
	pl_char_set( pl_w, Char_Special, 11 ) ;
	pl_w->tlocal[TM_SOUND] = pl_w->cg_ctr ;
	shl2f_01_set( pl_w ) ;
	pl_gauge_add( pl_w, 10 ) ;
	sp_dash_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void sp_dash_move( PLAYER_WORK *pl_w )
{
	Float spd, acc ;
	Uint8 no ;

	if( -- pl_w->tlocal[TM_SOUND] < 0 ){
		pl_w->tlocal[TM_SOUND] = 8 ;
		pl_se_req( pl_w, 26 ) ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		spd = -DOT_X( 4.0f ) ;
		acc = DOT_X( 0.125f ) ;
		if( pl_w->disp_dir ){
			spd = DOT_X( 4.0f ) ;
			acc = -DOT_X( 0.125f ) ;
		}
		pl_w->spd.x = spd ;
		pl_w->acc.x = acc ;
		pl_w->atck_no = pl2f_at_rapid ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 64 ) ;
	}
	no = pl_w->cg_flag1 ;
	if( no ){
		no += PL2F_LW( pl_w )->elem ;
		pl_w->atck_no = no ;
		HIT_RESET( pl_w ) ;
		pl_w->cg_flag1 = 0 ;
	}
}

/*==============================================================*/
/*			Sliding to a stop									*/
/*==============================================================*/
static void sp_dash_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) atck_end( pl_w ) ;
	else if( pl_w->cg_type ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		if( pl_w->spd.x * pl_w->acc.x >= 0.0f ) pl_w->spd.x = pl_w->acc.x = 0.0f ;
	}
}

static void hrapid_init( PLAYER_WORK *pl_w );
static void hrapid_wait( PLAYER_WORK *pl_w );
static void hrapid_turn( PLAYER_WORK *pl_w );
static void hrapid_start( PLAYER_WORK *pl_w );
static void hrapid_move( PLAYER_WORK *pl_w );
static void hrapid_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Hyper Hyakuretsutou, the team hyper combo			*/
/*==============================================================*/
static void sp_hrapid( PLAYER_WORK *pl_w )
{
	static void ( *hrapid_jmp[])( PLAYER_WORK *pl_w ) = {
		hrapid_init, hrapid_wait, hrapid_turn, hrapid_start, hrapid_move, hrapid_end,
	} ;

	hrapid_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void hrapid_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = pl2f_at_rapid ;
	HIT_RESET( pl_w ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_char_set( pl_w, Char_Extra, 5 ) ;
}

/*--------------------------------------------------------------*/
static void hrapid_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_voice_req( pl_w, 21 ) ;
		pl_color_set( pl_w, 1 ) ;
		pl_w->tlocal[FL_STEP] = 0 ;
		ofs.x = DOT_X( 13.0f ) ;
		ofs.y = DOT_Y( 75.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void hrapid_turn( PLAYER_WORK *pl_w )
{
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Super, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void hrapid_start( PLAYER_WORK *pl_w )
{
	Float spd ;

	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 90 ;
		pl_w->tlocal[TM_SOUND] = 8 ;
		spd = -10.0f ;
		if( pl_w->pl_dir ) spd = 10.0f ;
		pl_w->spd.x = spd ;
		pl_w->acc.x = 0.0f ;
		shl2f_01_set( pl_w ) ;
		hrapid_move( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void hrapid_move( PLAYER_WORK *pl_w )
{
	Float spd, acc ;

	pl_w->hyper_bg_tm = 5 ;
	if( -- pl_w->tlocal[TM_SOUND] < 0 ){
		pl_w->tlocal[TM_SOUND] = 8 ;
		pl_se_req( pl_w, 26 ) ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_char_move( pl_w ) < 0 ) pl_voice_req( pl_w, 3 ) ;
	else if( -- pl_w->timer0 < 0 ){
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_2 ++ ;
		spd = -DOT_X( 4.0f ) ;
		acc = 0.15625f ;
		if( pl_w->disp_dir ){
			spd = DOT_X( 4.0f ) ;
			acc = -0.15625f ;
		}
		pl_w->spd.x = spd ;
		pl_w->acc.x = acc ;
		pl_w->atck_no = pl2f_at_hrapid_end ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 7 ) ;
	}
	if( pl_w->cg_flag1 ){
		pl_w->atck_no = pl_w->cg_flag1 + PL2F_LW( pl_w )->elem ;
		HIT_RESET( pl_w ) ;
		pl_w->cg_flag1 = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void hrapid_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) atck_end( pl_w ) ;
	else if( pl_w->cg_type ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		if( pl_w->spd.x * pl_w->acc.x >= 0.0f ) pl_w->spd.x = pl_w->acc.x = 0.0f ;
	}
}

/*==============================================================*/
/*			Cho-Shuriken (hyper), on the ground or in the air	*/
/*==============================================================*/
static void sp_hshuriken( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;
	int no ;

	if( !pl_w->routine_2 ){
		if( pl_w->team_type == 6 ){
			pl_w->stop_move_flag = 0xff ;
			pl_w->stop_move_timer = 16 ;
		}
		pl_w->routine_2 ++ ;
		pl_dir_set( pl_w ) ;
		pl_w->atck_no = pl2f_at_hshuriken ;
		HIT_RESET( pl_w ) ;
		if( pl_w->pl_stance != STANCE_AIR ){
			pl_w->pl_stance = STANCE_STAND ;
			pl_w->pos.y = pl_w->ground_y ;
			pl_sp_ef_set( pl_w ) ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			no = 0 ;
		} else {
			no = 14 ;
			pl_w->spd.x /= 16.0f ;
			pl_w->spd.y /= 8.0f ;
			pl_w->acc.y /= 64.0f ;
			pl_w->acc.x = 0.0f ;
		}
		pl_char_set( pl_w, Char_Special, no ) ;
		pl_voice_req( pl_w, 4 ) ;
	}
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	if( pl_w->pos.y >= pl_w->ground_y - DOT_Y( 48.0f ) ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		if( pl_w->pl_stance == STANCE_AIR ) pl_jump_ret( pl_w ) ;
		else atck_end( pl_w ) ;
	} else {
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			shl2f_00_set( pl_w, 1 ) ;
			shl2f_00_set( pl_w, 2 ) ;
			shl2f_00_set( pl_w, 3 ) ;
			pls2f_00_set( pl_w, 0 ) ;
			pl_voice_req( pl_w, 32 ) ;
		}
		if( pl_w->cg_prm ){
			pl_w->stop_move_flag = 0 ;
			pl_w->stop_move_timer = 0 ;
			pl_w->cg_prm = 0 ;
			pl_color_set( pl_w, 1 ) ;
			pl_w->tlocal[FL_STEP] = 0 ;
			ofs.x = DOT_X( 25.0f ) ;
			ofs.y = DOT_Y( 87.0f ) ;
			if( pl_w->pl_stance == STANCE_AIR ){
				ofs.x = -DOT_X( 4.0f ) ;
				ofs.y = DOT_Y( 122.0f ) ;
			}
			pl_hyper_start( pl_w, &ofs, 1 ) ;
		}
	}
}

/*==============================================================*/
/*			Special move 9, on the ground or in the air			*/
/*==============================================================*/
static void sp_09( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		if( pl_w->pl_stance != STANCE_AIR ){
			pl_w->pl_stance = STANCE_STAND ;
			pl_sp_ef_set( pl_w ) ;
		}
		pl_char_set( pl_w, Char_Special, 19 ) ;
		pl_voice_req( pl_w, 30 ) ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		if( pl_w->pl_stance == STANCE_AIR ) pl_jump_ret( pl_w ) ;
		else atck_end( pl_w ) ;
	}
}

static void elem_k_init( PLAYER_WORK *pl_w );
static void elem_k_set( PLAYER_WORK *pl_w );
static void elem_k_wait( PLAYER_WORK *pl_w );
static void elem_k_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The Toki hyper combos: taking on an element			*/
/*==============================================================*/
static void sp_elem_k( PLAYER_WORK *pl_w )
{
	static void ( *elem_k_jmp[])( PLAYER_WORK *pl_w ) = {
		elem_k_init, elem_k_set, elem_k_wait, elem_k_end,
	} ;

	elem_k_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void elem_k_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->timer0 = 32 ;
	pl_char_set( pl_w, Char_Special, 40 ) ;
	pl_voice_req( pl_w, 21 ) ;
	pl_sp_ef_set( pl_w ) ;
	elem_k_set( pl_w ) ;
}

/* atck_pow and def_pow for each level and element */
static Uint8 elem_atck_pow_tbl[3][4] = {
	{ 0x00, 0x10, 0x10, 0x28 }, { 0x00, 0x18, 0x18, 0x28 }, { 0x00, 0x18, 0x20, 0x30 },
} ;
static Uint8 elem_def_pow_tbl[3][4] = {
	{ 0x00, 0x28, 0x10, 0x30 }, { 0x00, 0x28, 0x10, 0x28 }, { 0x00, 0x20, 0x10, 0x20 },
} ;

/*==============================================================*/
/*			The element: 1 to 3 by the button and strength;		*/
/*			the same one again goes up a level					*/
/*==============================================================*/
static void elem_k_set( PLAYER_WORK *pl_w )
{
	Sint32 no, time ;
	VEC3 ofs ;

	pl_char_move( pl_w ) ;
	pl_w->timer0 -- ;
	if( !pl_w->cg_type ) return ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->routine_2 ++ ;
	pl_w->cg_type = 0 ;
	no = 0 ;
	if( pl_w->atck_pk ) no = 2 ;
	if( pl_w->atck_level ) no ++ ;
	if( !no ) no = 1 ;
	if( PL2F_LW( pl_w )->elem != no ){
		pls2f_04_set( pl_w ) ;
		pl_w->tlocal[ELEM_LV] = 0 ;
	} else {
		pl_w->tlocal[ELEM_LV] ++ ;
		if( pl_w->tlocal[ELEM_LV] > 2 ) pl_w->tlocal[ELEM_LV] = 2 ;
	}
	PL2F_LW( pl_w )->elem = no ;
	PL2F_LW( pl_w )->elem_flag = 0xff ;
	PL2F_LW( pl_w )->unk_20 = 0 ;
	pl_w->atck_pow = elem_atck_pow_tbl[pl_w->tlocal[ELEM_LV]][PL2F_LW( pl_w )->elem] ;
	pl_w->def_pow = elem_def_pow_tbl[pl_w->tlocal[ELEM_LV]][PL2F_LW( pl_w )->elem] ;
	pl_w->armor_flag = 0 ;
	time = pl_w->tlocal[TM_ELEM] ;
	pl_w->tlocal[TM_ELEM] = 0x480 ;
	switch( PL2F_LW( pl_w )->elem ){
	case 1 :
		pls2f_03_set( pl_w ) ;
		break ;
	case 2 :
		pls2f_02_set( pl_w ) ;
		if( pl_w->tlocal[ELEM_LV] < 2 ) armor_set( pl_w, 0x480, 0, 0 ) ;
		else pl_w->armor_flag = ARMOR_ON ;
		break ;
	case 3 :
		pls2f_01_set( pl_w ) ;
		break ;
	case 0 :
		pl_w->tlocal[TM_ELEM] = 0 ;
		break ;
	}
	Timer_Gauge_Set( pl_w, &pl_w->tlocal[TM_ELEM], pl_w->tlocal[ELEM_LV] + 10 ) ;
	if( pl_w->tlocal[ELEM_LV] ){
		if( time < 0x240 ) time = 0x240 ;
		pl_w->tlocal[TM_ELEM] = time ;
	}
	pl_color_set( pl_w, 1 ) ;
	pl_w->tlocal[FL_STEP] = 0 ;
	ofs.x = DOT_X( 13.0f ) ;
	ofs.y = DOT_Y( 172.0f ) ;
	pl_hyper_start( pl_w, &ofs, 1 ) ;
	pl_w->delay_flag = 0 ;
	pl_w->delay_timer = 0 ;
	pl_w->hyper_bg = 0 ;
	pl_w->hyper_bg_tm = 0 ;
	pl_w->hyper_bg = 1 ;
	vs_sys_w->hyper_bg_req = ( pl_w->pl_no << 4 ) | 1 ;
	pl_w->hyper_bg_tm = 16 ;
}

/*--------------------------------------------------------------*/
static void elem_k_wait( PLAYER_WORK *pl_w )
{
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, 5 ) ;
	}
}

/*--------------------------------------------------------------*/
static void elem_k_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		atck_end( pl_w ) ;
	}
}

/*==============================================================*/
/*			The taunt, kept up by a player's START button		*/
/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->timer0 = 60 ;
		pl_char_set( pl_w, Char_Special, 37 ) ;
		pl_voice_req( pl_w, 21 ) ;
	} else {
		pl_char_move( pl_w ) ;
		if( vs_sys_work.vs_st == VS_FIGHT && !pl_w->cpu_flag && ( pl_w->pl_sw_new & KEY_START ) && pl_w->timer0 <= 1 )
			pl_w->timer0 = 2 ;
		if( !-- pl_w->timer0 ) atck_end( pl_w ) ;
	}
}

static void snapback_init( PLAYER_WORK *pl_w );
static void snapback_wait( PLAYER_WORK *pl_w );
static void snapback_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The snapback										*/
/*==============================================================*/
static void sp_snapback( PLAYER_WORK *pl_w )
{
	static void ( *snapback_jmp[])( PLAYER_WORK *pl_w ) = {
		snapback_init, snapback_wait, snapback_end,
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
	pl_w->atck_no = pl2f_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 6 ) ;
}

/*--------------------------------------------------------------*/
static void snapback_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_color_set( pl_w, 1 ) ;
		pl_w->tlocal[FL_STEP] = 0 ;
		ofs.x = DOT_X( 28.0f ) ;
		ofs.y = DOT_Y( 76.0f ) ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
}

/*--------------------------------------------------------------*/
static void snapback_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) atck_end( pl_w ) ;
}

/*==============================================================*/
/*			The crouching heavy punch (gamma assist)			*/
/*==============================================================*/
static void sp_crouch_hp( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_dir_set( pl_w ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_w->atck_no = ATCK_CROUCH_HP ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, 7 ) ;
	} else if( pl_char_move( pl_w ) < 0 ) atck_end( pl_w ) ;
}

static void homura_init( PLAYER_WORK *pl_w );
static void homura_wait( PLAYER_WORK *pl_w );
static void homura_move( PLAYER_WORK *pl_w );
static void homura_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Homuraken (hyper): Raimeiken with the fire element	*/
/*==============================================================*/
static void sp_homura( PLAYER_WORK *pl_w )
{
	static void ( *homura_jmp[])( PLAYER_WORK *pl_w ) = {
		homura_init, homura_wait, homura_move, homura_end,
	} ;

	homura_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void homura_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = pl2f_at_homura ;
	HIT_RESET( pl_w ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->timer0 = 128 ;
	pl_w->timer1 = 30 ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_voice_req( pl_w, 21 ) ;
	pl_char_set( pl_w, Char_Special, 4 ) ;
	homura_wait( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void homura_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	pl_w->timer0 -- ;
	if( pl_w->cg_type ){
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_color_set( pl_w, 1 ) ;
		pl_w->tlocal[FL_STEP] = 0 ;
		ofs.x = DOT_X( 25.0f ) ;
		ofs.y = DOT_Y( 87.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
		shl2f_05_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void homura_move( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 < 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, 5 ) ;
	} else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_se_req( pl_w, 76 ) ;
	}
}

/*--------------------------------------------------------------*/
static void homura_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) atck_end( pl_w ) ;
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_move( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A kick that leaps off the wall						*/
/****************************************************************/
void pl2f_t_kick( PLAYER_WORK *pl_w )
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
	spd = -30.0f ;
	if( pl_w->pl_dir ) spd = 30.0f ;
	pl_w->spd.x = spd ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = -DOT_Y( 0.125f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->atck_no = pl2f_at_t_kick ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 38 ) ;
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
		pl_w->timer0 = 40 ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 39 ) ;
	}
}

/*--------------------------------------------------------------*/
static void t_kick_land( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !-- pl_w->timer0 ) pl_stop_stand_ret( pl_w ) ;
}

/* the throw for each stance and button, plus one; 0 for none */
static int catch_tbl[4][2] = {
	{ 1, 2 }, { 0, 0 }, { 0, 3 }, { 0, 0 },
} ;

/****************************************************************/
PLAYER_WORK *pl2f_catch_ck( PLAYER_WORK *pl_w )
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

/****************************************************************/
/*			Start the throw that pl2f_catch_ck chose			*/
/****************************************************************/
void pl2f_catch_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( !( pl_w->catch_no & CATCH_NO_MASK ) ) pl_w->pl_dir ^= 1 ;
	if( !( pl_w->dir & 1 ) ){
		pl_w->disp_dir ^= 1 ;
		pl_w->pl_dir ^= 1 ;
	}
	ofs.x = -DOT_X( 34.0f ) ;
	ofs.y = DOT_Y( 85.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	scr_excep_set( pl_w, 5, 5 ) ;
	if( pl_w->catch_no & CATCH_NO_MASK ) pl_w->hit_stop = 10 ;
	pl_char_set( pl_w, Char_Catch, pl_w->catch_no & CATCH_NO_MASK ) ;
	catch_gauge_add( pl_w ) ;
}

static void nml_p_catch( PLAYER_WORK *pl_w );
static void nml_k_catch( PLAYER_WORK *pl_w );
static void jump_k_catch( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void pl2f_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_catch, nml_k_catch, jump_k_catch,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
/*			The punch throw: a leap with the opponent,			*/
/*			slammed down on landing								*/
/*==============================================================*/
static void nml_p_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 pos ;
	Float x, y, spd ;

	if( pl_w->routine_2 < 2 ){
		if( pl_w->routine_2 != 1 ){
			pl_char_move( pl_w ) ;
			if( pl_w->cg_prm ){
				pl_w->routine_2 ++ ;
				pl_w->cg_prm = 0 ;
				spd = DOT_X( 4.0f ) ;
				if( pl_w->disp_dir ) spd = -DOT_X( 4.0f ) ;
				pl_w->spd.x = spd ;
				pl_w->acc.x = 0.0f ;
				pl_w->spd.y = DOT_Y( 16.0f ) ;
				pl_w->acc.y = -DOT_Y( 0.75f ) ;
			}
			if( pl_w->cg_type < 0 ){
				x = pl_w->pos.x ;
				y = pl_w->pos.y ;
				catch_em_pos_set1( pl_w, pl_w->catch_w ) ;
				pl_w->pos.x = ( pl_w->pos.x + x * 3.0f ) / 4.0f ;
				pl_w->pos.y = ( pl_w->pos.y + y * 3.0f ) / 4.0f ;
			}
			return ;
		}
		if( pl_w->cg_prm >= 0 ) pl_char_move( pl_w ) ;
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		em = pl_w->catch_w ;
		if( pl_w->pos.y < pl_w->ground_y ){
			pl_w->pos.y = pl_w->ground_y ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_w->cg_prm = 0 ;
			pl_w->cg_ctr = 1 ;
			pl_char_move( pl_w ) ;
			catch_pos_set( pl_w, em ) ;
			pos = em->pos ;
			pos.y = em->ground_y ;
			land_dust_set( &pos, em->pl_no ) ;
			catch_se_req( pl_w ) ;
			scr_work.quake_level = 3 ;
			scr_work.quake_flag = 1 ;
			pl_land_ef_set( pl_w ) ;
		}
		if( pl_w->cg_type < 0 ){
			pl_w->routine_2 ++ ;
			pl_w->cg_type = 0 ;
			pl_w->atck_attr = elem_kind_tbl[PL2F_LW( pl_w )->elem] ;
			em->dm_hit_w = pl_w ;
			em->catch_type = 1 ;
			em->pl_dir = pl_w->pl_dir ^ 1 ;
			em->atck_no = pl2f_at_throw_p ;
			scr_excep_clr() ;
			pl_w->disp_dir ^= 1 ;
			spd = 2.5f ;
			if( pl_w->disp_dir ) spd = -2.5f ;
			pl_w->spd.x = spd ;
			pl_w->acc.x = 0.0f ;
			pl_w->spd.y = DOT_Y( 8.0f ) ;
			pl_w->acc.y = -DOT_Y( 0.375f ) ;
			pl_char_set( pl_w, Char_Jump, 11 ) ;
		}
	} else {
		pl_char_move( pl_w ) ;
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->pos.y < pl_w->ground_y ){
			pl_w->pos.y = pl_w->ground_y ;
			pl_land_ef_set( pl_w ) ;
			atck_end( pl_w ) ;
		}
	}
}

/*==============================================================*/
/*			The kick throw: a step forward, then the			*/
/*			opponent is let go									*/
/*==============================================================*/
static void nml_k_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	Float spd ;

	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		spd = DOT_X( 4.0f ) ;
		if( pl_w->pl_dir ) spd = -DOT_X( 4.0f ) ;
		pl_w->spd.x = spd ;
		pl_w->acc.x = 0.0f ;
	}
	if( pl_w->cg_prm > 0 ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
	if( pl_char_move( pl_w ) < 0 ) atck_end( pl_w ) ;
	else if( pl_w->cg_type < 0 ){
		pl_w->cg_type = 0 ;
		pl_w->spd.x = 0.0f ;
		pl_w->atck_attr = elem_kind_tbl[PL2F_LW( pl_w )->elem] ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->pl_dir = pl_w->pl_dir ;
		em->atck_no = pl2f_at_throw_k ;
		scr_excep_clr() ;
	}
}

/*==============================================================*/
/*			The kick throw in the air							*/
/*==============================================================*/
static void jump_k_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
	else if( pl_w->cg_type < 0 ){
		pl_w->cg_type = 0 ;
		pl_w->spd.x = 0.0f ;
		pl_w->atck_attr = elem_kind_tbl[PL2F_LW( pl_w )->elem] ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->pl_dir = pl_w->pl_dir ;
		em->atck_no = pl2f_at_air_throw ;
		scr_excep_clr() ;
	}
}

/****************************************************************/
void pl2f_tlocal_init( PLAYER_WORK *pl_w )
{
	pl_w->armor_flag = 0 ;
	pl_w->atck_pow = 0 ;
	pl_w->def_pow = 0 ;
	memset( pl_w->tlocal, 0, sizeof( pl_w->tlocal ) ) ;
}

/* the sound and the attack's kind for the element */
static Uint8 elem_voice_tbl[4] = { 0, 79, 0, 75 } ;
static Sint16 elem_kind_tbl2[4] = { 0, 3, 1, 0x0102 } ;

/*==============================================================*/
static void atck_voice( PLAYER_WORK *pl_w )
{
	int voice ;

	if( voice = elem_voice_tbl[PL2F_LW( pl_w )->elem] ) pl_se_req( pl_w, voice ) ;
	pl_w->atck_attr = elem_kind_tbl2[PL2F_LW( pl_w )->elem] ;
}

/*==============================================================*/
static void atck_end( PLAYER_WORK *pl_w )
{
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_stand_ret( pl_w ) ;
}

/****************************************************************/
/*			Holding the opponent: the kick throws, or the		*/
/*			punch throw once it lands							*/
/****************************************************************/
void pl2f_hold( PLAYER_WORK *pl_w )
{
	if( ( pl_w->catch_no & CATCH_NO_MASK ) || pl_w->catch_w->cg_type > 0 ) catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist; the third	*/
/*			assist type uses the element's hyper combo			*/
/****************************************************************/
void pl2f_sp_unison( PLAYER_WORK *pl_w )
{
	static Uint8 unison_sp[6] = {
		pl2f_sp_hrapid, pl2f_sp_hshuriken, pl2f_sp_raimei, pl2f_sp_raimei, pl2f_sp_hyouga, pl2f_sp_homura,
	} ;
	Uint8 no ;

	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	no = pl_w->pl_personal ;
	if( no == 2 ) no += PL2F_LW( pl_w )->elem ;
	pl_w->pl_sp_type = unison_sp[no] ;
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl2f_sp_gattai( PLAYER_WORK *pl_w )
{
	static Uint8 gattai_sp[6] = {
		pl2f_sp_hrapid, pl2f_sp_hshuriken, pl2f_sp_raimei, pl2f_sp_raimei, pl2f_sp_hyouga, pl2f_sp_homura,
	} ;
	Uint8 no ;

	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	no = pl_w->pl_personal ;
	if( no == 2 ) no += PL2F_LW( pl_w )->elem ;
	pl_w->pl_sp_type = gattai_sp[no] ;
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl2f_sp_assist( PLAYER_WORK *pl_w )
{
	static Uint8 assist_sp[3][2] = {
		{ pl2f_sp_rapid, 1 }, { pl2f_sp_shuriken, 1 }, { pl2f_sp_crouch_hp, 1 },
	} ;

	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	pl_w->pl_sp_type = assist_sp[pl_w->pl_personal][0] ;
	pl_w->atck_level = assist_sp[pl_w->pl_personal][1] ;
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void pl2f_sp_zero( PLAYER_WORK *pl_w )
{
	static Uint8 zero_sp[3][2] = {
		{ pl2f_sp_rapid, 1 }, { pl2f_sp_shuriken, 1 }, { pl2f_sp_crouch_hp, 1 },
	} ;

	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	pl_w->pl_sp_type = zero_sp[pl_w->pl_personal][0] ;
	pl_w->atck_level = zero_sp[pl_w->pl_personal][1] ;
	act_st_set( pl_w, N_SPECIAL ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl2f.c" End of File	*/

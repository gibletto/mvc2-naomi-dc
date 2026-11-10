/* Thanos (character 36): command checks, normal attacks, appearance, win and lose poses, special
   moves and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl36_def.h"

extern PLAYER_WORK *pls36_03_set( PLAYER_WORK *pl_w, Uint16 no );

void pl36_main( PLAYER_WORK *pl_w );
void pl36_com_ck( PLAYER_WORK *pl_w );
void pl36_atck_set( PLAYER_WORK *pl_w );
void pl36_atck( PLAYER_WORK *pl_w );
void pl36_dash( PLAYER_WORK *pl_w );
void pl36_noki( PLAYER_WORK *pl_w );
void pl36_appear( PLAYER_WORK *pl_w );
void pl36_win( PLAYER_WORK *pl_w );
void pl36_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl36_catch_ck( PLAYER_WORK *pl_w );
void pl36_catch_set( PLAYER_WORK *pl_w );
void pl36_catch( PLAYER_WORK *pl_w );
void pl36_hold( PLAYER_WORK *pl_w );
int pl36_delay_ck( PLAYER_WORK *pl_w );
void pl36_t_kick( PLAYER_WORK *pl_w );
void pl36_upper( PLAYER_WORK *pl_w );
void pl36_sp_unison( PLAYER_WORK *pl_w );
void pl36_sp_gattai( PLAYER_WORK *pl_w );
void pl36_sp_assist( PLAYER_WORK *pl_w );
void pl36_sp_zero( PLAYER_WORK *pl_w );

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

static Sint8 *atck_tbl[18] = {
	lp_tbl, mp_tbl, hp_tbl, lk_tbl, mk_tbl, hk_tbl,
	j_lp_tbl, j_mp_tbl, j_hp_tbl, j_lk_tbl, j_mk_tbl, j_hk_tbl,
	sj_lp_tbl, sj_mp_tbl, sj_hp_tbl, sj_lk_tbl, sj_mk_tbl, sj_hk_tbl,
} ;

/* commands: time or steps, kind, the special move, then the stick and buttons */
static Uint16 com_titan[] = { 3, 0, pl36_sp_titan, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Titan Crush */
static Uint16 com_sphere[] = { 3, COM_GROUND, pl36_sp_sphere, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Death Sphere */
static Uint16 com_reality[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl36_sp_reality, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Gauntlet Reality (hyper) */
static Uint16 com_soul[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl36_sp_soul, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Soul Gem (hyper) */
static Uint16 com_space[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl36_sp_space, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Gauntlet Space (hyper) */
static Uint16 com_power[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl36_sp_power, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Power Gem (hyper) */

static void ( *pl36_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl36_main,
	pl36_com_ck,
	pl36_atck_set,
	pl36_atck,
	pl36_dash,
	pl36_noki,
	pl36_appear,
	pl36_win,
	pl36_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl36_catch_ck,	/* returns a value */
	pl36_catch_set,
	pl36_catch,
	pl36_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl36_delay_ck,
	pl36_t_kick,
	pl36_upper,
	pl36_sp_unison,
	pl36_sp_gattai,
	pl36_sp_assist,
	pl36_sp_zero,
} ;

static int titan_ck( PLAYER_WORK *pl_w );
static int sphere_ck( PLAYER_WORK *pl_w );
static int reality_ck( PLAYER_WORK *pl_w );
static int soul_ck( PLAYER_WORK *pl_w );
static int space_ck( PLAYER_WORK *pl_w );
static int power_ck( PLAYER_WORK *pl_w );
static int chouhatsu_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static int reality_delay_ck( PLAYER_WORK *pl_w );
static int space_delay_ck( PLAYER_WORK *pl_w );
static int power_delay_ck( PLAYER_WORK *pl_w );
static int soul_delay_ck( PLAYER_WORK *pl_w );
static void stand_ret( PLAYER_WORK *pl_w );

/****************************************************************/
void pl36_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl36_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl36_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( soul_ck( pl_w ) ) return ;
	if( reality_ck( pl_w ) ) return ;
	if( space_ck( pl_w ) ) return ;
	if( power_ck( pl_w ) ) return ;
	if( titan_ck( pl_w ) ) return ;
	if( sphere_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
/*			Also in the air, once a jump unless super			*/
/*			jumping, and not again too soon						*/
/*==============================================================*/
static int titan_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_titan, &pl_w->com_w ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	if( pl_w->pl_stance == STANCE_AIR && PL36_LW( pl_w )->titan_timer > 0 ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl36_sp_titan ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int sphere_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_sphere, &pl_w->com_w0 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl36_sp_sphere ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int reality_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_reality, &pl_w->com_w1 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl36_sp_reality ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static int soul_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_soul, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl36_sp_soul ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static int space_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_space, &pl_w->com_w2 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl36_sp_space ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static int power_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_power, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl36_sp_power ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static int chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 8 ) ) return 0 ;
	pl_w->pl_sp_type = pl36_sp_chouhatsu ;
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
	pl_w->pl_sp_type = pl36_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl36_delay_ck( PLAYER_WORK *pl_w )
{
	if( soul_delay_ck( pl_w ) || reality_delay_ck( pl_w ) || space_delay_ck( pl_w ) || power_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static int reality_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_reality, &pl_w->com_w1 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl36_sp_reality ;
	return 1 ;
}

/*==============================================================*/
static int space_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_space, &pl_w->com_w2 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl36_sp_space ;
	return 1 ;
}

/*==============================================================*/
static int power_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_power, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl36_sp_power ;
	return 1 ;
}

/*==============================================================*/
static int soul_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_soul, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl36_sp_soul ;
	return 1 ;
}

/****************************************************************/
/*			Flashes the gauntlet while that animation plays,	*/
/*			and counts the timers down							*/
/****************************************************************/
void pl36_main( PLAYER_WORK *pl_w )
{
	int	flag ;
	Uint16	no ;

	if( pl_w->char_no.b.grp == Char_Get_Up ){
		if( ( flag = pl_w->cg_prm ) ){
			pl_w->cg_prm = 0 ;
			if( flag == 1 ) no = 0 ;
			else no = 1 ;
			ef_color_set( pl_w, no, 0, 1 ) ;
		} else pl_color_trans( pl_w, 0, 0, 2 ) ;
	}
	if( PL36_LW( pl_w )->titan_timer ) PL36_LW( pl_w )->titan_timer -- ;
	if( PL36_LW( pl_w )->shot_timer[0] ) PL36_LW( pl_w )->shot_timer[0] -- ;
	if( PL36_LW( pl_w )->hyper_timer ) PL36_LW( pl_w )->hyper_timer -- ;
}

static void nml_atck( PLAYER_WORK *pl_w );
static void jump_atck( PLAYER_WORK *pl_w );
static void jump_atck_ck( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Normal attacks: nml_no is the strength, then		*/
/*			kicks and crouching ones follow						*/
/*==============================================================*/
static void nml_atck( PLAYER_WORK *pl_w )
{
	static Uint8 atck_no[12] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 } ;
	static Uint8 atck_level[3] = { 0, 1, 2 } ;
	static Uint8 char_no[12] = { 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2 } ;
	static Uint8 char_group[12] = { Char_Stand_P, Char_Stand_P, Char_Stand_P, Char_Stand_K, Char_Stand_K, Char_Stand_K, Char_Crouch_P, Char_Crouch_P, Char_Crouch_P, Char_Crouch_K, Char_Crouch_K, Char_Crouch_K } ;
	static Uint8 two_hits[12] = { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } ;
	Uint8 no ;

	pl_brake_set( pl_w ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	no = pl_w->nml_no ;
	if( pl_w->pl_stance == STANCE_CROUCH ) no += 6 ;
	if( pl_w->atck_pk ) no += 3 ;
	PL36_LW( pl_w )->two_hit_flag = two_hits[no] ;
	pl_w->chain_tbl = atck_tbl[no % 6] ;
	pl_w->nml_level = atck_level[pl_w->nml_no] ;
	pl_w->atck_no = atck_no[no] ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, char_group[no], char_no[no] ) ;
	if( no == 5 ){
		if( pl_w->atck_lever & KEY_BACK ){
			pl_w->atck_no = pl36_at_back_hk ;
			HIT_RESET( pl_w ) ;
			pl_char_set( pl_w, Char_Stand_K, 3 ) ;
		}
	}
	if( no == 2 ) ef_color_set( pl_w, 1, 11, 1 ) ;
	if( no == 7 ) pls36_03_set( pl_w, 2 ) ;
}

/*==============================================================*/
static void jump_atck( register PLAYER_WORK *pl_w )
{
	static Uint8 atck_no[6] = { 12, 13, 14, 15, 16, 17 } ;
	static Uint8 atck_level[3] = { 0, 1, 2 } ;
	static Uint8 char_no[6] = { 0, 1, 2, 0, 1, 2 } ;
	static Uint8 char_group[6] = { Char_Jump_P, Char_Jump_P, Char_Jump_P, Char_Jump_K, Char_Jump_K, Char_Jump_K } ;
	register Uint8 no ;

	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	no = pl_w->nml_no ;
	if( pl_w->atck_pk ) no += 3 ;
	PL36_LW( pl_w )->two_hit_flag = 0 ;
	pl_w->nml_level = atck_level[pl_w->nml_no] ;
	pl_w->atck_no = atck_no[no] ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, char_group[no], char_no[no] ) ;
	if( no == 5 ){
		if( pl_w->atck_lever & KEY_DOWN ){
			if( pl_w->pos.y > pl_w->ground_y + DOT_Y( 80.0f ) ){
				pl_w->spd.x = 0.0f ;
				pl_w->spd.y = 0.0f ;
				pl_w->acc.x = 0.0f ;
				pl_w->acc.y = 0.0f ;
				PL36_LW( pl_w )->two_hit_flag = 1 ;
				pl_w->jump_atk_cnt &= JATK_P_MASK ;
				pl_w->atck_no = pl36_at_jdown_hk ;
				HIT_RESET( pl_w ) ;
				pl_char_set( pl_w, Char_Jump_K, 5 ) ;
				ef_color_set( pl_w, 1, 14, 1 ) ;
			}
		}
	}
	if( no == 2 ) ef_color_set( pl_w, 1, 15, 1 ) ;
	if( pl_w->sjump_flag ) no += 6 ;
	pl_w->chain_tbl = atck_tbl[6 + no] ;
	if( !pl_w->atck_pk ){
		if( pl_w->jump_atk_cnt & JATK_P_MASK ) pl_w->jump_atk_cnt -= 0x01 ;
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
	if( !pl_w->atck_pk ){
		if( pl_w->jump_atk_cnt & JATK_P_MASK ) jump_atck( pl_w ) ;
	} else {
		if( pl_w->jump_atk_cnt & JATK_K_MASK ) jump_atck( pl_w ) ;
	}
}

/****************************************************************/
void pl36_atck_set( PLAYER_WORK *pl_w )
{
	static void ( *atck_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_atck, jump_atck_ck, jump_atck, nml_atck,
	} ;

	atck_set_jmp[pl_w->atck_type]( pl_w ) ;
}

static void jump_move( PLAYER_WORK *pl_w );
static void jump_kick_stomp( PLAYER_WORK *pl_w );
static void jump_kick_stomp_start( PLAYER_WORK *pl_w );
static void jump_kick_stomp_hit( PLAYER_WORK *pl_w );
static void jump_kick_stomp_end( PLAYER_WORK *pl_w );
static void sjump_move( PLAYER_WORK *pl_w );
static void nml_move( PLAYER_WORK *pl_w );
static void nml_move_2hit( PLAYER_WORK *pl_w );
static void nml_move_dash( PLAYER_WORK *pl_w );

/*==============================================================*/
static void jump_move( PLAYER_WORK *pl_w )
{
	pl_jump_move( pl_w ) ;
	pl_sjump_top_ck( pl_w ) ;
	if( !PL36_LW( pl_w )->two_hit_flag ){
		if( pl_char_move( pl_w ) < 0 ){
			pl_jump_ret( pl_w ) ;
			return ;
		}
	} else jump_kick_stomp( pl_w ) ;
	if( pl_land_ck( pl_w ) ) pl_land_set( pl_w ) ;
}

/*==============================================================*/
/*			The heavy kick held down in a high jump: a stomp	*/
/*			that bounces off									*/
/*==============================================================*/
static void jump_kick_stomp( PLAYER_WORK *pl_w )
{
	static void ( *jump_kick_stomp_jmp[])( PLAYER_WORK *pl_w ) = {
		jump_kick_stomp_start, jump_kick_stomp_hit, jump_kick_stomp_end,
	} ;

	jump_kick_stomp_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void jump_kick_stomp_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		if( pl_w->sjump_flag ){
			pl_w->sjump_flag = 0 ;
			pl_w->spd.x = 1.6666666f ;
			pl_w->spd.y = -DOT_Y( 2.0f ) ;
		}
		pl_w->spd.x += DOT_X( 4.0f ) ;
		pl_w->spd.y += -DOT_Y( 8.0f ) ;
		if( !pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
	}
}

/*--------------------------------------------------------------*/
static void jump_kick_stomp_hit( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->hit_flag ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = -( pl_w->spd.x * 3.0f ) / 4.0f ;
		pl_w->spd.y = DOT_Y( 8.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_char_set( pl_w, Char_Jump_K, ++ pl_w->char_no.b.no ) ;
	}
}

/*--------------------------------------------------------------*/
static void jump_kick_stomp_end( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void sjump_move( PLAYER_WORK *pl_w )
{
	pl_jump_hit_ck( pl_w ) ;
	pl_sjump_ctrl( pl_w ) ;
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
	if( !PL36_LW( pl_w )->two_hit_flag ){
		if( pl_char_move( pl_w ) < 0 ) stand_ret( pl_w ) ;
	} else nml_move_2hit( pl_w ) ;
}

/*==============================================================*/
/*			The second hit of an attack that hits twice			*/
/*==============================================================*/
static void nml_move_2hit( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) stand_ret( pl_w ) ;
	else if( pl_w->cg_type ){
		PL36_LW( pl_w )->two_hit_flag = 0 ;
		pl_w->atck_no = pl36_at_stand_mp_2 ;
		HIT_RESET( pl_w ) ;
	}
}

/*==============================================================*/
static void nml_move_dash( PLAYER_WORK *pl_w )
{
	pl_smoke_set( pl_w ) ;
	nml_move( pl_w ) ;
}

/****************************************************************/
void pl36_atck( PLAYER_WORK *pl_w )
{
	static void ( *atck_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_move, jump_move, sjump_move, nml_move_dash,
	} ;

	atck_jmp[pl_w->atck_type]( pl_w ) ;
}

/****************************************************************/
/*			The launcher										*/
/****************************************************************/
void pl36_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_brake_set( pl_w ) ;
		pl_gauge_add( pl_w, 5 ) ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_w->atck_no = pl36_at_upper ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, 6 ) ;
		pls36_03_set( pl_w, 2 ) ;
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
/*			A float forward, for as long as the stick is		*/
/*			held												*/
/****************************************************************/
void pl36_dash( PLAYER_WORK *pl_w )
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

	pl_w->routine_2 ++ ;
	spd = 10.0f ;
	if( !pl_w->pl_dir ) spd = -10.0f ;
	pl_w->spd.x = spd ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->timer0 = 5 ;
	dash_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void dash_move( PLAYER_WORK *pl_w )
{
	Float spd ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ){
		if( -- pl_w->timer0 > 0 && ( pl_w->cpu_flag || ( pl_w->pl_sw_new & KEY_FORWARD ) ) ) return ;
		pl_w->routine_2 ++ ;
		spd = DOT_X( 0.5f ) ;
		if( !pl_w->pl_dir ) spd = -DOT_X( 0.5f ) ;
		pl_w->spd.x = spd ;
		pl_w->acc.x = 0.0f ;
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
/*			The back dash, a float back							*/
/****************************************************************/
void pl36_noki( PLAYER_WORK *pl_w )
{
	static void ( *noki_jmp[])( PLAYER_WORK *pl_w ) = {
		noki_init, noki_move, noki_end,
	} ;

	noki_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void noki_init( PLAYER_WORK *pl_w )
{
	Float spd, acc ;

	pl_w->routine_2 ++ ;
	pl_w->timer0 = 16 ;
	spd = -DOT_X( 8.0f ) ;
	acc = DOT_X( 0.25f ) ;
	if( !pl_w->pl_dir ){
		spd = DOT_X( 8.0f ) ;
		acc = -DOT_X( 0.25f ) ;
	}
	pl_w->spd.x = spd ;
	pl_w->acc.x = acc ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	noki_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void noki_move( PLAYER_WORK *pl_w )
{
	Float spd ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type >= 0 ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( -- pl_w->timer0 < 0 ){
			pl_w->routine_2 ++ ;
			spd = -1.6666666f ;
			if( !pl_w->pl_dir ) spd = 1.6666666f ;
			pl_w->spd.x = spd ;
			pl_w->acc.x = 0.0f ;
			pl_char_set( pl_w, Char_Dash, 3 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void noki_end( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ) stand_ret( pl_w ) ;
}

/****************************************************************/
/*			He stays hidden until the round's start				*/
/****************************************************************/
void pl36_appear( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		if( vs_sys_work.vs_st < VS_APPEAR ){
			pl_w->disp_flag = 0 ;
			return ;
		}
		pl_w->routine_2 ++ ;
		pl_w->disp_flag = 1 ;
		pl_char_set( pl_w, Char_Appear, 0 ) ;
	} else if( pl_char_move( pl_w ) < 0 ) pl_w->routine_1 ++ ;
}

static void win_time_ko( PLAYER_WORK *pl_w );
static void lose_time_ko( PLAYER_WORK *pl_w );

/****************************************************************/
void pl36_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_time_ko,
		lose_time_ko,
		win_time_ko,
		lose_time_ko,
		lose_time_ko,
	} ;

	if( pl_win_end_ck( pl_w ) ) stand_ret( pl_w ) ;
	else win_jmp[ pl_w->type ]( pl_w ) ;
}

/*==============================================================*/
static void win_time_ko( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, Rnd() & 1 ) ;
		pls36_03_set( pl_w, 1 ) ;
		pl_voice_req( pl_w, Rnd() % 3 + 15 ) ;
	} else pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void lose_time_ko( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, CH36_LOSE ) ;
	} else pl_char_move( pl_w ) ;
}

static void sp_titan( PLAYER_WORK *pl_w );
static void sp_sphere( PLAYER_WORK *pl_w );
static void sp_reality( PLAYER_WORK *pl_w );
static void sp_space( PLAYER_WORK *pl_w );
static void sp_power( PLAYER_WORK *pl_w );
static void sp_soul( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_upper( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );

/****************************************************************/
void pl36_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *pl_sp_tbl[])( PLAYER_WORK *pl_w ) = {
		sp_titan,					/* 00 Titan Crush */
		sp_sphere,					/* 01 Death Sphere */
		sp_reality,					/* 02 Gauntlet Reality (hyper) */
		sp_space,					/* 03 Gauntlet Space (hyper) */
		sp_power,					/* 04 Power Gem (hyper) */
		sp_power,					/* 05 */
		sp_soul,					/* 06 Soul Gem (hyper) */
		sp_power,					/* 07 */
		sp_chouhatsu,					/* 08 Taunt */
		sp_upper,					/* 09 Uppercut (assist) */
		sp_snapback,					/* 0a Snapback */
	} ;

	pl_sp_tbl[ pl_w->pl_sp_type ]( pl_w ) ;
}

static void titan_init( PLAYER_WORK *pl_w );
static void titan_move( PLAYER_WORK *pl_w );
static void titan_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A charge forward, slowing down; also in the air		*/
/*==============================================================*/
static void sp_titan( PLAYER_WORK *pl_w )
{
	static void ( *titan_jmp[])( PLAYER_WORK *pl_w ) = {
		titan_init, titan_move, titan_end,
	} ;

	titan_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void titan_init( PLAYER_WORK *pl_w )
{
	static Float titan_spd[3 * 2] = {
		-DOT_X( 10.0f ), 0.46875f,
		-DOT_X( 11.5f ), 0.46875f,
		-DOT_X( 13.0f ), 0.46875f,
	} ;
	static Uint16 titan_atck[3] = { 0x30, 0x31, 0x32 } ;
	Float *tbl, spd, acc ;

	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	if( pl_w->pl_stance != STANCE_AIR ){
		pl_w->pl_stance = STANCE_STAND ;
		pl_sp_ef_set( pl_w ) ;
	}
	tbl = &titan_spd[pl_w->atck_level * 2] ;
	spd = *tbl++ ;
	acc = *tbl ;
	if( pl_w->pl_dir ){
		spd = -spd ;
		acc = -acc ;
	}
	pl_w->spd.x = spd ;
	pl_w->acc.x = acc ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	ef_color_set( pl_w, 1, 20, 1 ) ;
	pl_w->atck_no = titan_atck[pl_w->atck_level] ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level ) ;
}

/*--------------------------------------------------------------*/
static void titan_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type < 0 ) return ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->spd.x * pl_w->acc.x >= 0.0f ){
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		if( pl_w->pl_stance == STANCE_AIR ){
			PL36_LW( pl_w )->titan_timer = 12 ;
			pl_jump_ret( pl_w ) ;
			return ;
		}
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void titan_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_sphere( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 17 ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_dir_set( pl_w ) ;
		pl_gauge_add( pl_w, 5 ) ;
		pl_sp_ef_set( pl_w ) ;
		pl_w->atck_no = pl36_at_sphere ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 6 ) ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type & 1 ){
		shl36_03_set( pl_w ) ;
		if( !PL36_LW( pl_w )->shot_timer[0] ) dm00_0a_set( pl_w ) ;
		pl_voice_req( pl_w, 34 ) ;
		pl_w->unk_27b = 0 ;
		pl_w->cancel_timer = 16 ;
	}
	pl_w->cg_type &= ~1 ;
	if( !pl_w->cg_prm ) return ;
	pl_w->timer0 ^= 7 ;
	if( pl_w->cg_prm & 1 ) ef_color_set( pl_w, 1, pl_w->timer0, 1 ) ;
}

static void hyper_color( PLAYER_WORK *pl_w, int col );
static void reality_init( PLAYER_WORK *pl_w );
static void reality_wait( PLAYER_WORK *pl_w );
static void reality_start( PLAYER_WORK *pl_w );
static void reality_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_reality( PLAYER_WORK *pl_w )
{
	static void ( *reality_jmp[])( PLAYER_WORK *pl_w ) = {
		reality_init, reality_wait, reality_start, reality_move,
	} ;

	PL36_LW( pl_w )->hyper_timer = 4 ;
	reality_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void reality_init( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_sp_ef_set( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_w->atck_no = pl36_at_reality ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 9 ) ;
	if( ( ew = pls36_03_set( pl_w, 4 ) ) ) ew->dir = 0 ;
	if( ( ew = pls36_03_set( pl_w, 4 ) ) ) ew->dir = 16 ;
}

/*--------------------------------------------------------------*/
static void reality_wait( PLAYER_WORK *pl_w )
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
		pl_w->timer0 = 180 ;
		pl_voice_req( pl_w, 21 ) ;
		ofs.x = 40.0f ;
		ofs.y = DOT_Y( 160.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void reality_start( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	shl36_01_set( pl_w ) ;
	ef_color_set( pl_w, 2, 9, 1 ) ;
	pl_se_req( pl_w, 77 ) ;
	scr_work.quake_level = 3 ;
	scr_work.quake_flag = 1 ;
	reality_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void reality_move( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	PL36_LW( pl_w )->unk_20 = 0xff ;
	hyper_color( pl_w, 17 ) ;
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 < 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_color_trans( pl_w, 0, 0, 1 ) ;
		pl_se_req( pl_w, 59 ) ;
		stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
/*			The hyper combos' flashing colour					*/
/*==============================================================*/
static void hyper_color( PLAYER_WORK *pl_w, int col )
{
	if( Sys->game_timer0 & 1 ) pl_color_trans( pl_w, 0, 0, 1 ) ;
	else ef_color_set( pl_w, 0, col, 1 ) ;
}

static void space_init( PLAYER_WORK *pl_w );
static void space_wait( PLAYER_WORK *pl_w );
static void space_start( PLAYER_WORK *pl_w );
static void space_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_space( PLAYER_WORK *pl_w )
{
	static void ( *space_jmp[])( PLAYER_WORK *pl_w ) = {
		space_init, space_wait, space_start, space_move,
	} ;

	PL36_LW( pl_w )->hyper_timer = 4 ;
	space_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void space_init( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_sp_ef_set( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_w->atck_no = pl36_at_space ;
	HIT_RESET( pl_w ) ;
	pls36_01_set( pl_w, 0 ) ;
	pl_char_set( pl_w, Char_Special, 10 ) ;
	if( ( ew = pls36_03_set( pl_w, 5 ) ) ) ew->dir = 0 ;
	if( ( ew = pls36_03_set( pl_w, 5 ) ) ) ew->dir = 16 ;
}

/*--------------------------------------------------------------*/
static void space_wait( PLAYER_WORK *pl_w )
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
		pl_w->timer0 = 185 ;
		pl_w->timer1 = 0 ;
		pl_voice_req( pl_w, 23 ) ;
		ofs.x = 40.0f ;
		ofs.y = DOT_Y( 160.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void space_start( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_voice_req( pl_w, 37 ) ;
	pl_se_req( pl_w, 77 ) ;
	space_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void space_move( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	PL36_LW( pl_w )->unk_20 = 0xff ;
	hyper_color( pl_w, 21 ) ;
	if( !( pl_w->timer1 += 0x2000 ) ){
		scr_work.quake_level = 2 ;
		scr_work.quake_flag = 1 ;
	}
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 < 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_se_req( pl_w, 59 ) ;
		pl_color_trans( pl_w, 0, 0, 1 ) ;
		stand_ret( pl_w ) ;
	}
}

static void power_init( PLAYER_WORK *pl_w );
static void power_wait( PLAYER_WORK *pl_w );
static void power_start( PLAYER_WORK *pl_w );
static void power_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_power( PLAYER_WORK *pl_w )
{
	static void ( *power_jmp[])( PLAYER_WORK *pl_w ) = {
		power_init, power_wait, power_start, power_move,
	} ;

	PL36_LW( pl_w )->hyper_timer = 4 ;
	power_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void power_init( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_sp_ef_set( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_w->atck_no = pl36_at_power ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 11 ) ;
	if( ( ew = pls36_03_set( pl_w, 8 ) ) ) ew->dir = 0 ;
	if( ( ew = pls36_03_set( pl_w, 8 ) ) ) ew->dir = 16 ;
}

/*--------------------------------------------------------------*/
static void power_wait( PLAYER_WORK *pl_w )
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
		pl_w->timer0 = 1 ;
		pl_voice_req( pl_w, 20 ) ;
		ofs.x = 40.0f ;
		ofs.y = DOT_Y( 160.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void power_start( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	PL36_LW( pl_w )->unk_20 = 0xff ;
	hyper_color( pl_w, 20 ) ;
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 > 0 ) return ;
	scr_work.quake_level = 2 ;
	scr_work.quake_flag = 1 ;
	shl36_00_set( pl_w ) ;
	ef_color_set( pl_w, 3, 8, 1 ) ;
	pl_w->routine_2 ++ ;
	pl_w->timer0 = 17 ;
	pl_char_set( pl_w, Char_Super, 12 ) ;
}

/*--------------------------------------------------------------*/
static void power_move( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	hyper_color( pl_w, 20 ) ;
	PL36_LW( pl_w )->unk_20 = 0xff ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_color_trans( pl_w, 0, 0, 1 ) ;
		stand_ret( pl_w ) ;
	} else if( pl_w->cg_prm ){
		pl_w->timer0 ^= 7 ;
		if( pl_w->cg_prm & 1 ) ef_color_set( pl_w, 1, pl_w->timer0, 1 ) ;
		if( pl_w->cg_prm & 4 ){
			pl_w->delay_flag = 0 ;
			pl_w->delay_timer = 0 ;
			pl_w->hyper_bg = 0 ;
			pl_w->hyper_bg_tm = 0 ;
			pl_color_trans( pl_w, 0, 0, 1 ) ;
		}
	}
}

static void soul_init( PLAYER_WORK *pl_w );
static void soul_wait( PLAYER_WORK *pl_w );
static void soul_start( PLAYER_WORK *pl_w );
static void soul_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_soul( PLAYER_WORK *pl_w )
{
	static void ( *soul_jmp[])( PLAYER_WORK *pl_w ) = {
		soul_init, soul_wait, soul_start, soul_move,
	} ;

	PL36_LW( pl_w )->hyper_timer = 4 ;
	soul_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void soul_init( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = pl36_at_soul ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 13 ) ;
	if( ( ew = pls36_03_set( pl_w, 6 ) ) ) ew->dir = 0 ;
	if( ( ew = pls36_03_set( pl_w, 6 ) ) ) ew->dir = 16 ;
}

/*--------------------------------------------------------------*/
static void soul_wait( PLAYER_WORK *pl_w )
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
		pl_w->timer0 = 420 ;
		pl_voice_req( pl_w, 22 ) ;
		ofs.x = 40.0f ;
		ofs.y = DOT_Y( 160.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void soul_start( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->routine_2 ++ ;
	pl_w->timer0 = 420 ;
	shl36_04_set( pl_w ) ;
	pl_se_req( pl_w, 79 ) ;
	scr_work.quake_level = 2 ;
	scr_work.quake_flag = 1 ;
	soul_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void soul_move( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	PL36_LW( pl_w )->unk_20 = 0xff ;
	hyper_color( pl_w, 18 ) ;
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 < 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_color_trans( pl_w, 0, 0, 1 ) ;
		stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
/*			The taunt											*/
/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->timer1 = 60 ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_char_set( pl_w, Char_Win, 0 ) ;
		pl_voice_req( pl_w, 16 ) ;
	} else {
		pl_char_move( pl_w ) ;
		if( -- pl_w->timer1 <= 0 ){
			pl_color_trans( pl_w, 0, 0, 1 ) ;
			stand_ret( pl_w ) ;
		}
	}
}

/*==============================================================*/
static void sp_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_dir_set( pl_w ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_w->atck_no = pl36_at_upper_assist ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, 5 ) ;
		pls36_03_set( pl_w, 2 ) ;
	} else if( pl_char_move( pl_w ) < 0 ) stand_ret( pl_w ) ;
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
	pl_w->atck_no = pl36_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 4 ) ;
}

/*--------------------------------------------------------------*/
static void snapback_move( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		ofs.x = DOT_X( 28.0f ) ;
		ofs.y = DOT_Y( 17.0f ) ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
}

/*--------------------------------------------------------------*/
static void snapback_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) stand_ret( pl_w ) ;
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_move( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The partner's attack as he comes in					*/
/****************************************************************/
void pl36_t_kick( PLAYER_WORK *pl_w )
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
	pl_color_set( pl_w, 1 ) ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->pos.y += DOT_Y( 64.0f ) ;
	spd = 30.0f ;
	if( !pl_w->pl_dir ) spd = -30.0f ;
	pl_w->spd.x = spd ;
	pl_w->spd.y = -DOT_Y( 0.125f ) ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->atck_no = pl36_at_t_kick ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 3 ) ;
}

/*--------------------------------------------------------------*/
static void t_kick_move( PLAYER_WORK *pl_w )
{
	if( !pl_w->cg_type ) pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_2 ++ ;
		pl_land_ef_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void t_kick_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stop_stand_ret( pl_w ) ;
}

/****************************************************************/
/*			Only standing, with a strong button					*/
/****************************************************************/
PLAYER_WORK *pl36_catch_ck( PLAYER_WORK *pl_w )
{
	Uint32 work ;
	PLAYER_WORK *em = 0 ;

	if( !pl_w->pl_stance ){
		work = pl_w->atck_lever & ( KEY_FORWARD | KEY_BACK ) ;
		if( work ){
			pl_w->dir = work >> 10 ;
			if( pl_w->atck_level == 1 ){
				if( ( em = pl_catch_ck( pl_w ) ) ){
					work = 0 ;
					if( pl_w->atck_pk == 1 ) work = 1 ;
					pl_w->catch_no = work ;
				}
			}
		}
	}
	return em ;
}

/****************************************************************/
/*			Start the throw that pl36_catch_ck chose			*/
/****************************************************************/
void pl36_catch_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;
	Sint8 no ;

	pl_w->hit_stop = 10 ;
	scr_excep_set( pl_w, 5, 5 ) ;
	catch_gauge_add( pl_w ) ;
	if( !( pl_w->catch_no & CATCH_NO_MASK ) ){
		if( pl_w->dir & 1 ){
			pl_w->pl_dir ^= 1 ;
			pl_w->disp_dir ^= 1 ;
		}
		ef_color_set( pl_w, 1, 11, 1 ) ;
		ofs.x = -DOT_X( 64.0f ) ;
		no = 0 ;
		ofs.y = DOT_Y( 80.0f ) ;
	} else {
		if( pl_w->dir & 1 ){
			pl_w->pl_dir ^= 1 ;
			pl_w->disp_dir ^= 1 ;
		}
		ef_color_set( pl_w, 1, 9, 1 ) ;
		ofs.x = -160.0f ;
		no = 1 ;
		ofs.y = DOT_Y( 88.0f ) ;
	}
	nage_hit_set( pl_w, &ofs ) ;
	pl_char_set( pl_w, Char_Catch, no ) ;
}

static void nml_p_catch( PLAYER_WORK *pl_w );
static void nml_k_catch( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void pl36_catch( PLAYER_WORK *pl_w )
{
	pl_w->catch_flag = 1 ;
	if( !( pl_w->catch_no & CATCH_NO_MASK ) ) nml_p_catch( pl_w ) ;
	else nml_k_catch( pl_w ) ;
}

/*==============================================================*/
static void nml_p_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	int col ;

	col = 11 ;
	if( Sys->game_timer0 & 1 ) col = 12 ;
	ef_color_set( pl_w, 1, col, 1 ) ;
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_se_req( pl_w, 79 ) ;
		pl_se_req( pl_w, 9 ) ;
	}
	if( pl_char_move( pl_w ) < 0 ) stand_ret( pl_w ) ;
	else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 2 ;
		em->pl_stance = STANCE_AIR ;
		em->pl_dir = pl_w->pl_dir ^ 1 ;
		em->atck_no = pl36_at_throw_p ;
		scr_excep_clr() ;
		catch_se_req( em ) ;
		pl_se_req( pl_w, 58 ) ;
	}
}

/*==============================================================*/
static void nml_k_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		pls36_03_set( pl_w, 0 ) ;
		stand_ret( pl_w ) ;
	} else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->pl_dir = pl_w->pl_dir ^ 1 ;
		em->atck_no = pl36_at_throw_k ;
		em->pl_stance = STANCE_AIR ;
		ofs.x = -DOT_X( 80.0f ) ;
		ofs.y = DOT_Y( 120.0f ) ;
		hit_mark_set( pl_w, &ofs, 2 ) ;
		pl_se_req( pl_w, 3 ) ;
		scr_excep_clr() ;
	}
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
/*			Holding the opponent								*/
/****************************************************************/
void pl36_hold( PLAYER_WORK *pl_w )
{
	if( ( pl_w->catch_no & CATCH_NO_MASK ) < 2 ) catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void pl36_sp_unison( PLAYER_WORK *pl_w )
{
	static Uint8 unison_tbl[3] = { pl36_sp_power, pl36_sp_reality, pl36_sp_space } ;

	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = unison_tbl[pl_w->pl_personal] ;
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl36_sp_gattai( PLAYER_WORK *pl_w )
{
	static Uint8 gattai_tbl[3] = { pl36_sp_power, pl36_sp_reality, pl36_sp_space } ;

	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = gattai_tbl[pl_w->pl_personal] ;
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
/*			The special move and its strength for each			*/
/*			assist												*/
/****************************************************************/
void pl36_sp_assist( PLAYER_WORK *pl_w )
{
	static Uint8 assist_tbl[3][2] = {
		{ pl36_sp_sphere, 0 }, { pl36_sp_titan, 1 }, { pl36_sp_upper, 1 },
	} ;

	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = assist_tbl[pl_w->pl_personal][0] ;
	pl_w->atck_level = assist_tbl[pl_w->pl_personal][1] ;
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void pl36_sp_zero( PLAYER_WORK *pl_w )
{
	static Uint8 zero_tbl[3][2] = {
		{ pl36_sp_sphere, 0 }, { pl36_sp_titan, 1 }, { pl36_sp_upper, 1 },
	} ;

	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = zero_tbl[pl_w->pl_personal][0] ;
	pl_w->atck_level = zero_tbl[pl_w->pl_personal][1] ;
	act_st_set( pl_w, N_SPECIAL ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl36.c" End of File	*/

/* Abyss, first form (character 18): command checks, normal attacks, appearance, win and lose,
   special moves and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl_act.h"
#include "pl18_def.h"

/* moves by the distance the animation gives, the way the player faces */
#define	CG_MOVE_X( pl )	\
	if( ( pl )->cg_rival ){	\
		if( !( pl )->disp_dir ) ( pl )->pos.x += ( Sint16 )( pl )->cg_rival * 1.6666666f ;	\
		else ( pl )->pos.x -= ( Sint16 )( pl )->cg_rival * 1.6666666f ;	\
		( pl )->cg_rival = 0 ;	\
	}

void pl18_main( PLAYER_WORK *pl_w );
void pl18_com_ck( PLAYER_WORK *pl_w );
void pl18_atck_set( PLAYER_WORK *pl_w );
void pl18_atck( PLAYER_WORK *pl_w );
void pl18_dash( PLAYER_WORK *pl_w );
void pl18_noki( PLAYER_WORK *pl_w );
void pl18_appear( PLAYER_WORK *pl_w );
void pl18_win( PLAYER_WORK *pl_w );
static void pl18_sp_atck( PLAYER_WORK *pl_w );
int pl18_catch_ck( PLAYER_WORK *pl_w );
void pl18_catch_set( PLAYER_WORK *pl_w );
void pl18_catch( PLAYER_WORK *pl_w );
void pl18_hold( PLAYER_WORK *pl_w );
int pl18_delay_ck( PLAYER_WORK *pl_w );
void pl18_t_kick( PLAYER_WORK *pl_w );
void pl18_sp_unison( PLAYER_WORK *pl_w );
void pl18_sp_gattai( PLAYER_WORK *pl_w );
void pl18_sp_assist( PLAYER_WORK *pl_w );
void pl18_sp_zero( PLAYER_WORK *pl_w );
void pl18_change( PLAYER_WORK *pl_w );

/* commands: time or steps, kind, the special move, then the stick and buttons */
static Uint16 com_sweep[] = { 3, COM_GROUND, pl18_sp_sweep, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ; /* a long sweeping blow */
static Uint16 com_punch[] = { 3, COM_GROUND, pl18_sp_punch, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ; /* a dark fist thrown out */
static Uint16 com_thunder[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl18_sp_thunder, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ; /* dark bolts dropped on the opponent */
static Uint16 com_ball[] = { 3, COM_GROUND, pl18_sp_ball, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ; /* three homing dark balls */
static Uint16 com_rush[] = { 3, COM_GROUND, pl18_sp_rush, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ; /* a charging rush */
static Uint16 com_pillar[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl18_sp_pillar, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ; /* a row of dark pillars */

static void ( *pl18_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl18_main,
	pl18_com_ck,
	pl18_atck_set,
	pl18_atck,
	pl18_dash,
	pl18_noki,
	pl18_appear,
	pl18_win,
	pl18_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl18_catch_ck,	/* returns a value */
	pl18_catch_set,
	pl18_catch,
	pl18_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl18_delay_ck,
	pl18_t_kick,
	0,
	pl18_sp_unison,
	pl18_sp_gattai,
	pl18_sp_assist,
	pl18_sp_zero,
	0,
	0,
	0,
	0,
	pl18_change,
} ;

static Uint8 sweep_ck( PLAYER_WORK *pl_w );
static Uint8 punch_ck( PLAYER_WORK *pl_w );
static Uint8 thunder_ck( PLAYER_WORK *pl_w );
static Uint8 ball_ck( PLAYER_WORK *pl_w );
static Uint8 pillar_ck( PLAYER_WORK *pl_w );
static Uint8 rush_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl18_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl18_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl18_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( thunder_ck( pl_w ) ) return ;
	if( pillar_ck( pl_w ) ) return ;
	if( punch_ck( pl_w ) ) return ;
	if( sweep_ck( pl_w ) ) return ;
	if( rush_ck( pl_w ) ) return ;
	if( ball_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
static Uint8 sweep_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_sweep, &pl_w->com_w ) ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl18_sp_sweep ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 punch_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_punch, &pl_w->com_w0 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl18_sp_punch ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 thunder_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_thunder, &pl_w->com_w1 ) ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl18_sp_thunder ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 ball_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_ball, &pl_w->com_w2 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl18_sp_ball ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 pillar_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_pillar, &pl_w->com_w3 ) ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl18_sp_pillar ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 rush_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_rush, &pl_w->com_w4 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w4 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl18_sp_rush ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/****************************************************************/
int pl18_delay_ck( PLAYER_WORK *pl_w )
{
	return 0 ;
}

static void hit_shake( PLAYER_WORK *pl_w );

/****************************************************************/
void pl18_main( PLAYER_WORK *pl_w )
{
	hit_shake( pl_w ) ;
	if( vs_sys_w->vs_st >= VS_END_WAIT
	 && ( vs_sys_w->winner_id < 0 || vs_sys_w->winner_id != pl_w->pl_no )
	 && pl_w->routine_1 == PL_NORM && pl_w->act_st == N_STAND ){
		pl_act_set( pl_w, N_SPECIAL ) ;
		pl_w->pl_sp_type = pl18_sp_change ;
	}
	if( pl_w->vital_new == 0 ) pl_w->push_ng_timer = 1 ;
}

/*==============================================================*/
/*			Shakes from side to side while the hit stops the	*/
/*			action												*/
/*==============================================================*/
static void hit_shake( PLAYER_WORK *pl_w )
{
	Sint16 ofs ;

	pl_w->col_flag = -1 ;
	pl_w->col_no = col_no_tbl[pl_w->work_no] ;
	if( !pl_w->armor_hit_flag ){
		if( pl_w->hit_stop && pl_w->routine_1 == PL_NORM && pl_w->hit_box_no ){
			pl_w->armor_hit_flag ++ ;
		} else {
			if( pl_w->armor_wait && !pl_w->hit_stop ){
				if( !--pl_w->armor_wait ) pl_w->armor_cnt = 0 ;
			}
			return ;
		}
	}
	if( !pl_w->hit_stop ){
		pl_w->armor_hit_flag = 0 ;
		return ;
	}
	ofs = 4 - ( pl_w->hit_stop & 1 ) * 8 ;
	if( pl_w->disp_dir ) ofs = -ofs ;
	pl_w->pos.x += 1.6666666f * ofs ;
	if( !( pl_w->hit_stop & 1 ) ){
		pl_w->col_flag = 1 ;
		pl_w->col_no = col_no_tbl[pl_w->work_no] + 4 ;
	}
}

static void nml_atck( PLAYER_WORK *pl_w );
static void jump_atck( PLAYER_WORK *pl_w );
static void jump_atck_ck( PLAYER_WORK *pl_w );

/*==============================================================*/
static void nml_atck( PLAYER_WORK *pl_w )
{
	Uint8 atck ;

	pl_brake_set( pl_w ) ;
	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.grp = 9 ;
		pl_w->char_no.b.no = 0 ;
		atck = 6 ;
		break ;
	case 1:
		pl_w->char_no.b.grp = 9 ;
		pl_w->char_no.b.no = 1 ;
		atck = 7 ;
		break ;
	case 2:
		pl_w->char_no.b.grp = 9 ;
		pl_w->char_no.b.no = 2 ;
		atck = 8 ;
		break ;
	case 3:
		pl_w->char_no.b.grp = 10 ;
		pl_w->char_no.b.no = 0 ;
		atck = 9 ;
		break ;
	case 4:
		pl_w->char_no.b.grp = 10 ;
		pl_w->char_no.b.no = 1 ;
		atck = 10 ;
		break ;
	case 5:
		pl_w->char_no.b.grp = 10 ;
		pl_w->char_no.b.no = 2 ;
		atck = 11 ;
		break ;
	case 6:
		pl_w->char_no.b.grp = 7 ;
		pl_w->char_no.b.no = 0 ;
		atck = 0 ;
		break ;
	case 7:
		pl_w->char_no.b.grp = 7 ;
		pl_w->char_no.b.no = 1 ;
		atck = 1 ;
		break ;
	case 8:
		pl_w->char_no.b.grp = 7 ;
		pl_w->char_no.b.no = 2 ;
		atck = 2 ;
		break ;
	case 9:
		pl_w->char_no.b.grp = 8 ;
		pl_w->char_no.b.no = 0 ;
		atck = 3 ;
		break ;
	case 10:
		pl_w->char_no.b.grp = 8 ;
		pl_w->char_no.b.no = 1 ;
		atck = 4 ;
		break ;
	case 11:
		pl_w->char_no.b.grp = 8 ;
		pl_w->char_no.b.no = 2 ;
		atck = 5 ;
		break ;
	}
	pl_w->atck_no = atck ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
static void jump_atck( PLAYER_WORK *pl_w )
{
	Uint8 atck ;

	switch( pl_w->nml_no ){
	case 0:
	case 6:
		pl_w->char_no.b.grp = 11 ;
		pl_w->char_no.b.no = 0 ;
		atck = 12 ;
		break ;
	case 1:
	case 7:
		pl_w->char_no.b.grp = 11 ;
		pl_w->char_no.b.no = 1 ;
		atck = 13 ;
		break ;
	case 2:
	case 8:
		pl_w->char_no.b.grp = 11 ;
		pl_w->char_no.b.no = 2 ;
		atck = 14 ;
		break ;
	case 3:
	case 9:
		pl_w->char_no.b.grp = 12 ;
		pl_w->char_no.b.no = 0 ;
		atck = 15 ;
		break ;
	case 4:
	case 10:
		pl_w->char_no.b.grp = 12 ;
		pl_w->char_no.b.no = 1 ;
		atck = 16 ;
		break ;
	case 5:
	case 11:
		pl_w->char_no.b.grp = 12 ;
		pl_w->char_no.b.no = 2 ;
		atck = 17 ;
		break ;
	}
	pl_w->atck_no = atck ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
	if( !pl_w->atck_pk ){
		if( pl_w->jump_atk_cnt & JATK_P_MASK ) pl_w->jump_atk_cnt -= 1 ;
	} else {
		if( pl_w->jump_atk_cnt & JATK_K_MASK ) pl_w->jump_atk_cnt -= 16 ;
	}
}

/*==============================================================*/
/*			A jump attack only while one is left				*/
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
/*			Normal attacks										*/
/****************************************************************/
void pl18_atck_set( PLAYER_WORK *pl_w )
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
static void nml_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_brake_stop( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	switch( pl_w->nml_no ){
	}
}

/****************************************************************/
void pl18_atck( PLAYER_WORK *pl_w )
{
	static void ( *atck_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_move, jump_move, sjump_move, nml_move,
	} ;

	atck_jmp[pl_w->atck_type]( pl_w ) ;
}

/****************************************************************/
void pl18_dash( PLAYER_WORK *pl_w )
{
	pl_stand_ret( pl_w ) ;
}

/****************************************************************/
/*			Not called from anywhere							*/
/****************************************************************/
void pl18_dash_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 120 ;
		pl_w->spd.x = !pl_w->pl_dir ? -DOT_X( 8.0f ) : DOT_X( 8.0f ) ;
	}
}

/****************************************************************/
void pl18_noki( PLAYER_WORK *pl_w )
{
	pl_stand_ret( pl_w ) ;
}

static void appear_init( PLAYER_WORK *pl_w );
static void appear_wait( PLAYER_WORK *pl_w );
static void appear_fade( PLAYER_WORK *pl_w );
static void appear_move( PLAYER_WORK *pl_w );
static void appear_end( PLAYER_WORK *pl_w );

/****************************************************************/
void pl18_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_init,
		appear_wait,
		appear_fade,
		appear_move,
		appear_end,
	} ;

	appear_jmp[ pl_w->routine_2 ]( pl_w ) ;
}

/*==============================================================*/
static void appear_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->disp_flag = 0 ;
	pl_w->alpha = 0.0f ;
	pl_w->timer0 = 140 ;
	if( vs_sys_work.game_mode == 8 ) halo_ef_set( pl_w ) ;
	pls18_01_set( pl_w, 0 ) ;
	pls18_01_set( pl_w, 1 ) ;
	pls18_01_set( pl_w, 2 ) ;
	pls18_01_set( pl_w, 3 ) ;
	pls18_01_set( pl_w, 4 ) ;
	pls18_01_set( pl_w, 5 ) ;
	pls18_01_set( pl_w, 6 ) ;
	pls18_01_set( pl_w, 7 ) ;
	pls18_01_set( pl_w, 8 ) ;
	pls18_01_set( pl_w, 9 ) ;
	pls18_01_set( pl_w, 10 ) ;
	pls18_01_set( pl_w, 11 ) ;
	pls18_01_set( pl_w, 12 ) ;
	pls18_01_set( pl_w, 13 ) ;
	pls18_01_set( pl_w, 14 ) ;
	pls18_01_set( pl_w, 15 ) ;
	pls18_01_set( pl_w, 16 ) ;
	pls18_01_set( pl_w, 17 ) ;
	pls18_01_set( pl_w, 18 ) ;
	pls18_01_set( pl_w, 19 ) ;
	pls18_01_set( pl_w, 20 ) ;
	pls18_01_set( pl_w, 21 ) ;
	pls18_01_set( pl_w, 22 ) ;
	pls18_01_set( pl_w, 23 ) ;
	pls18_01_set( pl_w, 24 ) ;
	pls18_01_set( pl_w, 25 ) ;
	pl_char_set( pl_w, Char_Appear, 0 ) ;
}

/*--------------------------------------------------------------*/
static void appear_wait( PLAYER_WORK *pl_w )
{
	if( --pl_w->timer0 ) return ;
	pl_w->routine_2 ++ ;
	pl_w->disp_flag = 1 ;
	pl_voice_req( pl_w, 4 ) ;
}

/*--------------------------------------------------------------*/
static void appear_fade( PLAYER_WORK *pl_w )
{
	pl_w->alpha += 0.05f ;
	if( pl_w->alpha >= 1.0f ){
		pl_w->alpha = 1.0f ;
		pl_w->routine_2 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void appear_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ) pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void appear_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_w->routine_1 ++ ;
}

static void win_init( PLAYER_WORK *pl_w );
static void win_type( PLAYER_WORK *pl_w );
static void win_time_ko( PLAYER_WORK *pl_w );
static void win_ko_init( PLAYER_WORK *pl_w );
static void win_ko_move( PLAYER_WORK *pl_w );
static void win_ko_move2( PLAYER_WORK *pl_w );
static void win_ko_move_e( PLAYER_WORK *pl_w );
static void lose_time_ko( PLAYER_WORK *pl_w );

/****************************************************************/
void pl18_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_init,
		win_type,
	} ;

	win_jmp[ pl_w->routine_2 ]( pl_w ) ;
}

/*==============================================================*/
static void win_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	win_type( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_type( PLAYER_WORK *pl_w )
{
	static void ( *win_type_jmp[])( PLAYER_WORK *pl_w ) = {
		win_time_ko,
		lose_time_ko,
		win_time_ko,
		lose_time_ko,
		lose_time_ko,
	} ;

	win_type_jmp[ pl_w->type ]( pl_w ) ;
}

/*==============================================================*/
static void win_time_ko( PLAYER_WORK *pl_w )
{
	static void ( *win_ko_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko_init,
		win_ko_move,
		win_ko_move2,
		win_ko_move_e,
	} ;

	win_ko_jmp[ pl_w->routine_3 ]( pl_w ) ;
}

/*==============================================================*/
static void win_ko_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_char_set( pl_w, Char_Win, 0 ) ;
	win_ko_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_ko_move( PLAYER_WORK *pl_w )
{
	if( pl_win_end_ck( pl_w ) ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		pls18_00_set( pl_w, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void win_ko_move2( PLAYER_WORK *pl_w )
{
	if( pl_win_end_ck( pl_w ) ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
	if( pl_w->cg_prm >= 1 ){
		pl_w->routine_3 ++ ;
		pls18_00_set( pl_w, 16 ) ;
	}
}

/*--------------------------------------------------------------*/
static void win_ko_move_e( PLAYER_WORK *pl_w )
{
	if( pl_win_end_ck( pl_w ) ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
/*			Losing, it changes to the next form					*/
/*==============================================================*/
static void lose_time_ko( PLAYER_WORK *pl_w )
{
	pl_act_set( pl_w, N_SPECIAL ) ;
	pl_w->pl_sp_type = pl18_sp_change ;
	pl18_sp_atck( pl_w ) ;
}

static void sp_punch( PLAYER_WORK *pl_w );
static void sp_thunder( PLAYER_WORK *pl_w );
static void sp_ball( PLAYER_WORK *pl_w );
static void sp_pillar( PLAYER_WORK *pl_w );
static void sp_change( PLAYER_WORK *pl_w );
static void sp_sweep( PLAYER_WORK *pl_w );
static void sp_rush( PLAYER_WORK *pl_w );

/*==============================================================*/
static void pl18_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *pl_sp_tbl[])( PLAYER_WORK *pl_w ) = {
		sp_punch,					/* 00 a dark fist thrown out */
		sp_thunder,					/* 01 dark bolts dropped on the opponent (hyper) */
		sp_ball,					/* 02 three homing dark balls */
		sp_pillar,					/* 03 a row of dark pillars (hyper) */
		sp_change,					/* 04 his fall, to change into the second form */
		sp_sweep,					/* 05 a long sweeping blow */
		sp_rush,					/* 06 a charging rush */
	} ;

	pl_sp_tbl[ pl_w->pl_sp_type ]( pl_w ) ;
}

static void punch_init( PLAYER_WORK *pl_w );
static void punch_move( PLAYER_WORK *pl_w );
static void punch_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_punch( PLAYER_WORK *pl_w )
{
	static void ( *punch_jmp[])( PLAYER_WORK *pl_w ) = {
		punch_init, punch_move, punch_end,
	} ;

	punch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void punch_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->atck_no = pl18_at_punch ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 0 ) ;
	pl_sp_ef_set( pl_w ) ;
	punch_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void punch_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		shl18_02_set( pl_w, pl_w, pl_w->atck_level ) ;
	}
	CG_MOVE_X( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void punch_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	CG_MOVE_X( pl_w ) ;
}

static void thunder_init( PLAYER_WORK *pl_w );
static void thunder_move( PLAYER_WORK *pl_w );
static void thunder_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_thunder( PLAYER_WORK *pl_w )
{
	static void ( *thunder_jmp[])( PLAYER_WORK *pl_w ) = {
		thunder_init, thunder_move, thunder_end,
	} ;

	thunder_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void thunder_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->atck_no = pl18_at_thunder ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 1 ) ;
	pl_sp_ef_set( pl_w ) ;
	thunder_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void thunder_move( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		ofs.x = -DOT_X( 62.0f ) ;
		ofs.y = DOT_Y( 202.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void thunder_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->hyper_bg_tm = 5 ;
		if( pl_w->cg_ctr & 1 ) shl18_00_set_all( pl_w, 0, 0 ) ;
	}
}

static void ball_init( PLAYER_WORK *pl_w );
static void ball_first( PLAYER_WORK *pl_w );
static void ball_second( PLAYER_WORK *pl_w );
static void ball_third( PLAYER_WORK *pl_w );
static void ball_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_ball( PLAYER_WORK *pl_w )
{
	static void ( *ball_jmp[])( PLAYER_WORK *pl_w ) = {
		ball_init, ball_first, ball_second, ball_third, ball_end,
	} ;

	ball_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void ball_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->atck_no = pl18_at_ball ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 2 ) ;
	pls18_00_set( pl_w, 0 ) ;
	pl_sp_ef_set( pl_w ) ;
	ball_first( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void ball_first( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		shl18_01_set( pl_w, 0 ) ;
	}
	CG_MOVE_X( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void ball_second( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		shl18_01_set( pl_w, 1 ) ;
	}
	CG_MOVE_X( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void ball_third( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		shl18_01_set( pl_w, 2 ) ;
	}
	CG_MOVE_X( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void ball_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	CG_MOVE_X( pl_w ) ;
}

static void pillar_init( PLAYER_WORK *pl_w, PL18_LOCAL_W *lw );
static void pillar_move( PLAYER_WORK *pl_w, PL18_LOCAL_W *lw );
static void pillar_wait( PLAYER_WORK *pl_w, PL18_LOCAL_W *lw );
static void pillar_shot( PLAYER_WORK *pl_w, PL18_LOCAL_W *lw );
static void pillar_hold( PLAYER_WORK *pl_w, PL18_LOCAL_W *lw );
static void pillar_end( PLAYER_WORK *pl_w, PL18_LOCAL_W *lw );

/*==============================================================*/
static void sp_pillar( PLAYER_WORK *pl_w )
{
	static void ( *pillar_jmp[])( PLAYER_WORK *pl_w, PL18_LOCAL_W *lw ) = {
		pillar_init, pillar_move, pillar_wait, pillar_shot, pillar_hold, pillar_end,
	} ;

	pillar_jmp[pl_w->routine_2]( pl_w, PL18_LW( pl_w ) ) ;
}

/*==============================================================*/
static void pillar_init( PLAYER_WORK *pl_w, PL18_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	lw->hold_cnt = pl_w->cpu_flag ? 12 : 24 ;
	pl_w->timer0 = 64 ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->atck_no = pl18_at_pillar ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 3 ) ;
	pl_sp_ef_set( pl_w ) ;
	pillar_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void pillar_move( PLAYER_WORK *pl_w, PL18_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_prm ){
		pl_w->routine_2 ++ ;
		pl_w->cg_prm = 0 ;
		pls18_00_pillar_set( pl_w, 10 ) ;
		pls18_00_pillar_set( pl_w, 11 ) ;
		pls18_00_pillar_set( pl_w, 12 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pillar_wait( PLAYER_WORK *pl_w, PL18_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		ofs.x = -DOT_X( 32.0f ) ;
		ofs.y = DOT_Y( 100.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pillar_shot( PLAYER_WORK *pl_w, PL18_LOCAL_W *lw )
{
	Float x, y ;

	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		x = DOT_X( 56.0f ) ;
		y = DOT_Y( 106.0f ) ;
		shl18_03_set( pl_w, 0, x, y ) ;
		pls18_00_set( pl_w, 13 ) ;
		pls18_00_set( pl_w, 21 ) ;
	}
}

/*==============================================================*/
/*			Held longer by mashing								*/
/*==============================================================*/
static void pillar_hold( PLAYER_WORK *pl_w, PL18_LOCAL_W *lw )
{
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( lw->hold_cnt >= 0 && sw_renda_ck( pl_w ) ){
		lw->hold_cnt -- ;
		pl_w->timer0 ++ ;
	}
	if( --pl_w->timer0 <= 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, 4 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pillar_end( PLAYER_WORK *pl_w, PL18_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void change_init( PLAYER_WORK *pl_w );
static void change_move( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The change to the next form							*/
/*==============================================================*/
static void sp_change( PLAYER_WORK *pl_w )
{
	static void ( *change_jmp[])( PLAYER_WORK *pl_w ) = {
		change_init, change_move,
	} ;

	pl_w->push_ng_timer = 1 ;
	change_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void change_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_char_set( pl_w, Char_Special, 5 ) ;
	change_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void change_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pls18_00_set( pl_w, 6 ) ;
		pl_act_set( pl_w, N_DOWN ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		if( !pl_w->cg_prm ) scr_work.quake_level = 1 ;
		else scr_work.quake_level = 3 ;
		scr_work.quake_flag = 1 ;
	}
}

static void sweep_init( PLAYER_WORK *pl_w );
static void sweep_move( PLAYER_WORK *pl_w );
static void sweep_hit( PLAYER_WORK *pl_w );
static void sweep_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_sweep( PLAYER_WORK *pl_w )
{
	static void ( *sweep_jmp[])( PLAYER_WORK *pl_w ) = {
		sweep_init, sweep_move, sweep_hit, sweep_end,
	} ;

	sweep_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void sweep_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->atck_no = pl18_at_sweep ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 6 ) ;
	sweep_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void sweep_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pls18_00_set( pl_w, 2 ) ;
	}
	CG_MOVE_X( pl_w ) ;
}

/*==============================================================*/
/*			Hits each of the four things in reach, once			*/
/*==============================================================*/
static void sweep_hit( PLAYER_WORK *pl_w )
{
	VEC3 pos ;
	Float near_x, far_x, x ;
	Uint8 i, bit ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type != 1 ){
		pl_w->routine_2 ++ ;
		if( !pl_w->disp_dir ){
			near_x = pl_w->pos.x + -160.0f ;
			far_x = pl_w->pos.x + -DOT_X( 112.0f ) ;
		} else {
			near_x = pl_w->pos.x + 160.0f ;
			far_x = pl_w->pos.x + DOT_X( 112.0f ) ;
		}
		i = 0 ;
		while( i < 4 ){
			bit = 1 << i ;
			if( !( PL18_LW( pl_w )->hit_flag & bit ) ){
				set08_00_cross_get( &pos.x, &pos.y, i ) ;
				x = ( i == 1 || i == 2 ) ? near_x : far_x ;
				if( x + -40.0f <= pos.x && pos.x <= x + 40.0f ){
					PL18_LW( pl_w )->hit_flag |= bit ;
					hitmark_set( &pos, pl_w->disp_dir, 3, 0 ) ;
					shl18_04_set_all( pl_w, &pos, i ) ;
				}
			}
			i ++ ;
		}
	}
	CG_MOVE_X( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void sweep_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	CG_MOVE_X( pl_w ) ;
}

static void rush_init( PLAYER_WORK *pl_w );
static void rush_start( PLAYER_WORK *pl_w );
static void rush_move( PLAYER_WORK *pl_w );
static void rush_keep( PLAYER_WORK *pl_w );
static void rush_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_rush( PLAYER_WORK *pl_w )
{
	static void ( *rush_jmp[])( PLAYER_WORK *pl_w ) = {
		rush_init, rush_start, rush_move, rush_keep, rush_end,
	} ;

	rush_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void rush_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->timer1 = 0 ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->atck_no = pl18_at_rush_1 ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 7 ) ;
	rush_start( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void rush_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 60 ;
		pl_w->spd.x = !pl_w->pl_dir ? -DOT_X( 8.0f ) : DOT_X( 8.0f ) ;
	}
}

/*--------------------------------------------------------------*/
static void rush_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		if( ++pl_w->timer1 & 1 ){
			pl_w->atck_no = pl18_at_rush_1 ;
			HIT_RESET( pl_w ) ;
		} else {
			pl_w->atck_no = pl18_at_rush_2 ;
			HIT_RESET( pl_w ) ;
		}
	}
	if( pl_w->cg_prm ){
		pl_w->routine_2 ++ ;
		pls18_00_set( pl_w, 4 ) ;
		pls18_00_set( pl_w, 5 ) ;
	}
}

/*--------------------------------------------------------------*/
static void rush_keep( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		if( pl_w->timer0 >= 6 ){
			if( ++pl_w->timer1 & 1 ){
				pl_w->atck_no = pl18_at_rush_1 ;
				HIT_RESET( pl_w ) ;
			} else {
				pl_w->atck_no = pl18_at_rush_2 ;
				HIT_RESET( pl_w ) ;
			}
		} else {
			pl_w->atck_no = pl18_at_rush_end ;
			HIT_RESET( pl_w ) ;
		}
	}
	if( !--pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->acc.x = !pl_w->pl_dir ? 0.26041666f : -0.26041666f ;
		pl_char_set( pl_w, Char_Special, 8 ) ;
	}
}

/*--------------------------------------------------------------*/
static void rush_end( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		RATE_CLR_X( pl_w ) ;
	}
}

static void tk_init( PLAYER_WORK *pl_w );
static void tk_jump( PLAYER_WORK *pl_w );
static void tk_land( PLAYER_WORK *pl_w );

/****************************************************************/
void pl18_t_kick( PLAYER_WORK *pl_w )
{
	static void ( *t_kick_jmp[])( PLAYER_WORK *pl_w ) = {
		tk_init, tk_jump, tk_land,
	} ;

	t_kick_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void tk_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	if( !pl_w->disp_dir ) pl_w->spd.x = -30.0f ;
	else pl_w->spd.x = 30.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 0.125f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->atck_no = pl18_at_t_kick ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 0 ) ;
}

/*--------------------------------------------------------------*/
static void tk_jump( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Extra, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void tk_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stop_stand_ret( pl_w ) ;
}

/****************************************************************/
int pl18_catch_ck( PLAYER_WORK *pl_w )
{
	return 0 ;
}

/****************************************************************/
void pl18_catch_set( PLAYER_WORK *pl_w )
{
}

/****************************************************************/
void pl18_catch( PLAYER_WORK *pl_w )
{
}

/****************************************************************/
void pl18_hold( PLAYER_WORK *pl_w )
{
	catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/****************************************************************/
void pl18_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl18_sp_punch ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl18_sp_punch ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl18_sp_punch ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl18_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl18_sp_punch ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl18_sp_punch ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl18_sp_punch ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl18_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl18_sp_punch ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl18_sp_punch ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl18_sp_punch ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void pl18_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl18_sp_punch ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl18_sp_punch ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl18_sp_punch ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
/*			Starts the change to the next form					*/
/****************************************************************/
void pl18_change( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl18_sp_change ;
	act_st_set( pl_w, N_SPECIAL ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl18.c" End of File	*/

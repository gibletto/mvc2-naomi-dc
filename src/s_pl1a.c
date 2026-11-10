/* Abyss, third form (character 1a): command checks, the dash, appearance, win and lose scenes,
   special moves, throws, the change on a KO and the computer player. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl1a_def.h"

void pl1a_main( PLAYER_WORK *pl_w );
void pl1a_com_ck( PLAYER_WORK *pl_w );
void pl1a_atck_set( PLAYER_WORK *pl_w );
void pl1a_atck( PLAYER_WORK *pl_w );
void pl1a_dash( PLAYER_WORK *pl_w );
void pl1a_noki( PLAYER_WORK *pl_w );
void pl1a_appear( PLAYER_WORK *pl_w );
void pl1a_win( PLAYER_WORK *pl_w );
void pl1a_sp_atck( PLAYER_WORK *pl_w );
int pl1a_catch_ck( PLAYER_WORK *pl_w );
void pl1a_catch_set( PLAYER_WORK *pl_w );
void pl1a_catch( PLAYER_WORK *pl_w );
void pl1a_hold( PLAYER_WORK *pl_w );
void pl1a_ctrl( PLAYER_WORK *pl_w );

/* commands: time or steps, kind, the special move, then the stick and buttons */
static Uint16 com_storm[] = { 3, COM_GROUND | COM_HYPER, pl1a_sp_storm, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_LP } ; /* a storm of shots */
static Uint16 com_stomp[] = { 3, COM_GROUND, pl1a_sp_stomp, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_HP } ; /* a leap that comes down on the opponent */
static Uint16 com_jump[] = { 3, COM_GROUND, pl1a_sp_jump, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_LK } ; /* jumps at the opponent, firing */
static Uint16 com_wave[] = { 3, COM_GROUND, pl1a_sp_wave, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_HK } ; /* a shot along the floor */
static Uint16 com_spray[] = { 3, COM_GROUND, pl1a_sp_spray, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_LP } ; /* shots sprayed out at random */
static Uint16 com_blast[] = { 3, COM_GROUND, pl1a_sp_blast, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_HP } ; /* a lasting blast in front of him */
static Uint16 com_win[] = { 3, COM_GROUND, pl1a_sp_win, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_HK } ; /* sinks and comes up before the opponent (win) */
static Uint16 com_win_voice[] = { 3, COM_GROUND, pl1a_sp_win_voice, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_LP } ; /* sinks and comes up before the opponent (win, with a voice) */
static Uint16 com_throw[] = { 3, COM_GROUND, pl1a_sp_throw, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_HP } ; /* grabs, leaps and drops on the opponent */
static Uint16 com_sink[] = { 3, COM_GROUND, pl1a_sp_sink, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_LK } ; /* sinks away for a time */
static Uint16 com_fade[] = { 3, COM_GROUND, pl1a_sp_fade, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_HK } ; /* fades away (lose) */
static Uint16 com_flame[] = { 3, COM_GROUND, pl1a_sp_flame, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_LP } ; /* hides in dark flames */

static void ( *pl1a_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl1a_main,
	pl1a_com_ck,
	pl1a_atck_set,
	pl1a_atck,
	pl1a_dash,
	pl1a_noki,
	pl1a_appear,
	pl1a_win,
	pl1a_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl1a_catch_ck,	/* returns a value */
	pl1a_catch_set,
	pl1a_catch,
	pl1a_hold,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	pl1a_ctrl,
} ;

static int storm_ck( PLAYER_WORK *pl_w );
static int stomp_ck( PLAYER_WORK *pl_w );
static int jump_ck( PLAYER_WORK *pl_w );
static int wave_ck( PLAYER_WORK *pl_w );
static int spray_ck( PLAYER_WORK *pl_w );
static int blast_ck( PLAYER_WORK *pl_w );
static int win_ck( PLAYER_WORK *pl_w );
static int win_voice_ck( PLAYER_WORK *pl_w );
static int throw_ck( PLAYER_WORK *pl_w );
static int sink_ck( PLAYER_WORK *pl_w );
static int fade_ck( PLAYER_WORK *pl_w );
static int flame_ck( PLAYER_WORK *pl_w );
static void stand_ret( PLAYER_WORK *pl_w );
static Uint8 em_range( PLAYER_WORK *pl_w );

/****************************************************************/
void pl1a_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl1a_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl1a_com_ck( PLAYER_WORK *pl_w )
{
	if( storm_ck( pl_w ) ) return ;
	if( stomp_ck( pl_w ) ) return ;
	if( jump_ck( pl_w ) ) return ;
	if( wave_ck( pl_w ) ) return ;
	if( spray_ck( pl_w ) ) return ;
	if( blast_ck( pl_w ) ) return ;
	if( win_ck( pl_w ) ) return ;
	if( win_voice_ck( pl_w ) ) return ;
	if( throw_ck( pl_w ) ) return ;
	if( sink_ck( pl_w ) ) return ;
	if( fade_ck( pl_w ) ) return ;
	if( flame_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
/*			The hyper combo, only while there is a level of		*/
/*			the gauge											*/
/*==============================================================*/
static int storm_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_storm, &pl_w->com_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1a_sp_storm ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static int stomp_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_stomp, &pl_w->com_w0 ) ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1a_sp_stomp ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int jump_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_jump, &pl_w->com_w1 ) ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1a_sp_jump ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int wave_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_wave, &pl_w->com_w2 ) ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1a_sp_wave ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int spray_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_spray, &pl_w->com_w3 ) ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1a_sp_spray ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int blast_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_blast, &pl_w->com_w4 ) ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1a_sp_blast ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int win_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_win, &pl_w->com_w5 ) ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1a_sp_win ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int win_voice_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_win_voice, &pl_w->com_w6 ) ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1a_sp_win_voice ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int throw_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_throw, &pl_w->com_w7 ) ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1a_sp_throw ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int sink_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_sink, &pl_w->com_w8 ) ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1a_sp_sink ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int fade_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_fade, &pl_w->com_w9 ) ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1a_sp_fade ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int flame_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_flame, &pl_w->com_w12 ) ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1a_sp_flame ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

static void hit_shake( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Sets how he is drawn while a hit stops the			*/
/*			action												*/
/****************************************************************/
void pl1a_main( PLAYER_WORK *pl_w )
{
	PL1A_LOCAL_W *lw = PL1A_LW( pl_w ) ;

	if( !pl_w->hit_stop ){
		PL1A_LW( pl_w )->body_atck_flag = 0 ;
		PL1A_LW( pl_w )->damage_flag = 0 ;
	}
	if( pl_w->hit_stop && pl_w->armor_hit_flag ){
		pl_w->fade.trnsl = 0.8f ;
		pl_w->fade.r = 1.0f ;
		if( pl_w->hit_stop & 1 ) pl_w->fade.r = 0.0f ;
		pl_w->fade.g = 0.0f ;
		pl_w->fade.b = 0.0f ;
	}
	if( !pl_w->hit_stop ) lw->body_alpha = 1.0f ;
	pl_w->alpha = 1.0f ;
	pl_w->disp_flag = 1 ;
	pl_w->push_ng_timer = 2 ;
	pl_w->wall_ng_timer = 2 ;
	hit_shake( pl_w ) ;
}

/*==============================================================*/
/*			Shakes from side to side while the hit stops the	*/
/*			action, and flashes while the armour is hit			*/
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
		pl_w->fade.r = 0.0f ;
		return ;
	}
	ofs = 4 - ( Sys->play_timer & 1 ) * 8 ;
	if( pl_w->disp_dir ) ofs = -ofs ;
	pl_w->pos.x += 1.6666666f * ofs ;
	if( !( pl_w->hit_stop & 1 ) && pl_w->armor_hit_flag ){
		pl_w->col_flag = 1 ;
		pl_w->col_no = col_no_tbl[pl_w->work_no] + 3 ;
	}
}

/****************************************************************/
void pl1a_atck_set( PLAYER_WORK *pl_w )
{
	stand_ret( pl_w ) ;
}

/****************************************************************/
void pl1a_atck( PLAYER_WORK *pl_w )
{
	stand_ret( pl_w ) ;
}

static void dash_start( PLAYER_WORK *pl_w );
static void dash_move( PLAYER_WORK *pl_w );
static void dash_end( PLAYER_WORK *pl_w );

/****************************************************************/
void pl1a_dash( PLAYER_WORK *pl_w )
{
	static void ( *dash_jmp[])( PLAYER_WORK *pl_w ) = {
		dash_start, dash_move, dash_end,
	} ;

	dash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dash_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 6.5f ) : -DOT_X( 6.5f ) ;
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
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? 1.6666666f : -1.6666666f ;
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

/****************************************************************/
void pl1a_noki( PLAYER_WORK *pl_w )
{
	stand_ret( pl_w ) ;
}

static void appear_init( PLAYER_WORK *pl_w );
static void appear_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl1a_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_init, appear_move,
	} ;

	pl_w->wall_ng_timer = 2 ;
	appear_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void appear_init( PLAYER_WORK *pl_w )
{
	pl_w->disp_flag = 0 ;
	pl_w->cg_shadow = 0xff ;
	if( vs_sys_work.vs_st < VS_APPEAR ) return ;
	pl_w->routine_2 ++ ;
	pl_w->disp_flag = 1 ;
	pl_dir_set( pl_w ) ;
	pl_char_set( pl_w, Char_Appear, 0 ) ;
	pl_w->fade.trnsl = 0.85f ;
	pl_w->fade.r = 0.0f ;
	pl_w->fade.g = 0.0f ;
	pl_w->fade.b = 0.0f ;
	shl1a_02_appear_set( pl_w ) ;
	shl1a_01_set( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void appear_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_w->routine_1 ++ ;
}

/****************************************************************/
/*			The win scene, by how close the opponent is, or		*/
/*			the lose scene										*/
/****************************************************************/
void pl1a_win( PLAYER_WORK *pl_w )
{
	Float x ;

	switch( pl_w->type ){
	case 0:
	case 2:
	case 4:
		pl_w->routine_1 = PL_NORM ;
		pl_w->routine_2 = pl_w->routine_3 = 0 ;
		x = pl_w->target_w->pos.x - pl_w->pos.x ;
		if( !pl_w->pl_dir ) x = -x ;
		x -= pl_w->scale.x * 160.0f ;
		if( ( Uint32 )x <= pl_w->scale.x * DOT_X( 128.0f ) && vs_sys_w->winner_id == pl_w->pl_no && !vs_sys_w->time_over ){
			pl_w->pl_sp_type = pl1a_sp_throw ;
		} else {
			if( Sys->play_timer & 1 ) pl_w->pl_sp_type = pl1a_sp_win ;
			else pl_w->pl_sp_type = pl1a_sp_win_voice ;
		}
		act_st_set( pl_w, N_SPECIAL ) ;
		break ;
	case 1:
	case 3:
		pl_w->routine_1 = PL_NORM ;
		pl_w->routine_2 = pl_w->routine_3 = 0 ;
		pl_w->pl_sp_type = pl1a_sp_fade ;
		act_st_set( pl_w, N_SPECIAL ) ;
		break ;
	}
}

static void sp_storm( PLAYER_WORK *pl_w );
static void sp_stomp( PLAYER_WORK *pl_w );
static void sp_jump( PLAYER_WORK *pl_w );
static void sp_wave( PLAYER_WORK *pl_w );
static void sp_spray( PLAYER_WORK *pl_w );
static void sp_blast( PLAYER_WORK *pl_w );
static void sp_win( PLAYER_WORK *pl_w );
static void sp_win_voice( PLAYER_WORK *pl_w );
static void sp_throw( PLAYER_WORK *pl_w );
static void sp_sink( PLAYER_WORK *pl_w );
static void sp_fade( PLAYER_WORK *pl_w );
static void sp_flame( PLAYER_WORK *pl_w );

/****************************************************************/
void pl1a_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *pl_sp_tbl[])( PLAYER_WORK *pl_w ) = {
		sp_storm, sp_stomp, sp_jump, sp_wave, sp_spray, sp_blast, sp_win, sp_win_voice, sp_throw, sp_sink, sp_fade, sp_flame,
	} ;

	pl_sp_tbl[pl_w->pl_sp_type]( pl_w ) ;
}

/*==============================================================*/
/*			The hyper combo: a storm of shots					*/
/*==============================================================*/
static void sp_storm( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;
	Uint8 no ;

	switch( pl_w->routine_2 ){
	case 0:
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, 0 ) ;
		shl1a_00_set( pl_w, 0, 0 ) ;
		pl_w->timer0 = 10 ;
		pl_w->timer1 = 0 ;
		pl_w->dir = 0 ;
		break ;
	case 1:
		pl_char_move( pl_w ) ;
		if( pl_w->cg_type ){
			pl_w->routine_2 ++ ;
			pl_w->cg_type = 0 ;
			ofs.x = -( pl_w->scale.x * DOT_X( 61.0f ) ) ;
			ofs.y = pl_w->scale.y * 165.0f ;
			ofs.z = pl_w->pos.z ;
			pl_hyper_flash3( pl_w, &ofs ) ;
		}
		break ;
	case 2:
		pl_w->hyper_bg_tm = 5 ;
		if( --pl_w->timer1 <= 0 ){
			pl_w->timer1 = Rnd() & 7 ;
			if( pl_w->dir = ( pl_w->dir + 1 ) & 1 ){
				pl_w->timer1 += 8 ;
				pl_w->timer0 -- ;
				no = pl_w->timer0 ;
			} else {
				no = Rnd() % 3 + 0x80 ;
			}
			shl1a_00_set( pl_w, 6, no ) ;
			if( pl_w->timer0 <= 0 ) pl_w->routine_2 ++ ;
		}
	case 3:
		if( pl_char_move( pl_w ) < 0 ){
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			stand_ret( pl_w ) ;
		}
		break ;
	}
}

static void stomp_init( PLAYER_WORK *pl_w );
static void stomp_jump( PLAYER_WORK *pl_w );
static void stomp_move( PLAYER_WORK *pl_w );
static void stomp_land( PLAYER_WORK *pl_w );
static void stomp_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			He leaps and comes down on the opponent				*/
/*==============================================================*/
static void sp_stomp( PLAYER_WORK *pl_w )
{
	static void ( *stomp_jmp[])( PLAYER_WORK *pl_w ) = {
		stomp_init, stomp_jump, stomp_move, stomp_land, stomp_end,
	} ;

	stomp_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void stomp_init( PLAYER_WORK *pl_w )
{
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->routine_2 ++ ;
	pl_char_set( pl_w, Char_Special, 1 ) ;
	pl_w->timer0 = 0 ;
	pl_w->timer1 = 3 ;
	pl_w->routine_3 = 0 ;
	if( !scr_in_x_ck( pl_w ) ) pl_dir_set( pl_w ) ;
	pl_w->atck_no = pl1a_at_stomp ;
	HIT_RESET( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void stomp_jump( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pls1a_00_set( pl_w, 0 ) ;
		pl_w->spd.x = -DOT_X( 16.0f ) ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 12.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.75f ) ;
		if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
		pl_w->type = 0 ;
		pl_w->pl_stance = STANCE_AIR ;
	}
}

/*--------------------------------------------------------------*/
static void stomp_move( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ef ;
	Float ofs ;
	Uint8 atck ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->hit_flag && !pl_w->routine_3 && pl_w->timer0 < 4 ){
		pl_w->routine_3 = 1 ;
		pl_w->timer1 = 3 ;
	}
	if( pl_w->routine_3 && --pl_w->timer1 <= 0 ){
		pl_w->routine_3 = 0 ;
		pl_w->timer0 ++ ;
		atck = 50 ;
		if( pl_w->timer0 >= 4 ) atck = 51 ;
		pl_w->atck_no = atck ;
		HIT_RESET( pl_w ) ;
	}
	if( !pl_w->type && pl_w->spd.y < 0.0f ){
		pl_w->type = 1 ;
		pl_char_set2( pl_w, Char_Special, 1, 8 ) ;
	}
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pls1a_00_set( pl_w, 2 ) ;
		if( ef = pls1a_00_set( pl_w, 2 ) ) ef->sub_type = 1 ;
		ofs = pl_w->scale.x * 200.0f ;
		if( !pl_w->disp_dir ) ofs = -ofs ;
		pl_w->pos.x += ofs ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Special, 2 ) ;
		pl_w->pl_stance = STANCE_STAND ;
	}
}

/*--------------------------------------------------------------*/
static void stomp_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_dir_set( pl_w ) ;
		pl_char_set( pl_w, Char_Crouch, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void stomp_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) stand_ret( pl_w ) ;
}

static void jump_init( PLAYER_WORK *pl_w );
static void jump_move( PLAYER_WORK *pl_w );
static void jump_shot( PLAYER_WORK *pl_w );
static void jump_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			He jumps at the opponent, firing as he goes			*/
/*==============================================================*/
static void sp_jump( PLAYER_WORK *pl_w )
{
	static void ( *jump_jmp[])( PLAYER_WORK *pl_w ) = {
		jump_init, jump_move, jump_shot, jump_end,
	} ;

	PL1A_LW( pl_w )->body_atck_flag = 1 ;
	jump_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void jump_init( PLAYER_WORK *pl_w )
{
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->routine_2 ++ ;
	pl_char_set( pl_w, Char_Special, 3 ) ;
	pl_w->atck_no = pl1a_at_jump ;
	HIT_RESET( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void jump_move( PLAYER_WORK *pl_w )
{
	Uint8 range ;

	pl_char_move( pl_w ) ;
	if( !pl_w->routine_3 ){
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pl_w->routine_3 ++ ;
		}
	} else if( !pl_w->cg_type ){
		pl_w->alpha = ( Float )pl_w->cg_ctr / ( Float )pl_w->cg_adrs[2] ;
	} else {
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->timer1 = 6 ;
		pl_w->dir = 0 ;
		pl_w->timer0 = 10 ;
		pl_w->cg_atck_flag |= CG_ATCK_ON ;
		if( !pl_w->cpu_flag ){
			if( pl_w->pl_sw_new & KEY_FORWARD ) pl_w->atck_level = 0 ;
			else if( pl_w->pl_sw_new & KEY_BACK ) pl_w->atck_level = 1 ;
			else pl_w->atck_level = 2 ;
		} else {
			range = em_range( pl_w ) ;
			switch( range ){
			case 4:
				if( Rnd() & 1 ) pl_w->atck_level = 0 ;
				else pl_w->atck_level = 2 ;
				break ;
			case 3:
			case 5:
				pl_w->atck_level = 1 ;
				break ;
			default:
				pl_w->atck_level = 2 ;
				break ;
			}
		}
		switch( pl_w->atck_level ){
		case 0:
		case 2:
			pl_w->acc = pl_w->pos ;
			break ;
		case 1:
			pl_w->acc = pl_w->target_w->pos ;
			break ;
		}
		pl_w->acc.y = pl_w->ground_y ;
	}
}

/*==============================================================*/
/*			Aims the next jump at the target kept in acc.x		*/
/*			and acc.y											*/
/*==============================================================*/
static void jump_spd_set( PLAYER_WORK *pl_w )
{
	Sint16 x ;
	int y ;
	Float ofs ;

	switch( pl_w->atck_level ){
	case 0:
		x = 160 - pl_w->timer0 * 16 ;
		if( pl_w->timer0 & 1 ) x = -x ;
		break ;
	case 1:
		x = pl_w->timer0 * 16 - 160 ;
		break ;
	case 2:
		x = 160 - pl_w->timer0 * 16 ;
		break ;
	}
	x += 32 - ( Rnd() & 63 ) ;
	ofs = x * pl_w->scale.x * 1.6666666f ;
	if( pl_w->disp_dir ) ofs = -ofs ;
	pl_w->spd.x = pl_w->acc.x - ofs ;
	y = ( Rnd() % 3 - 1 ) * 8 ;
	pl_w->spd.y = pl_w->acc.y + DOT_Y( 1.0f ) * y ;
}

/*--------------------------------------------------------------*/
static void jump_shot( PLAYER_WORK *pl_w )
{
	Uint8 type, no ;

	if( --pl_w->timer1 <= 0 ){
		pl_w->timer1 = Rnd() & 7 ;
		if( pl_w->dir = ( pl_w->dir + 1 ) % 3 ){
			type = 0 ;
			pl_w->timer1 += 8 ;
			pl_w->timer0 -- ;
		} else {
			type = 1 ;
			pl_w->timer1 += 4 ;
		}
		no = Rnd() & 15 ;
		jump_spd_set( pl_w ) ;
		shl1a_00_set( pl_w, type + 1, no ) ;
	}
	if( pl_w->timer0 <= 0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 20 ;
	}
}

/*--------------------------------------------------------------*/
static void jump_end( PLAYER_WORK *pl_w )
{
	if( pl_w->timer0 ){
		if( !--pl_w->timer0 ) pl_char_set( pl_w, Char_Crouch, 2 ) ;
		return ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		stand_ret( pl_w ) ;
	}
}

static void wave_init( PLAYER_WORK *pl_w );
static void wave_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_wave( PLAYER_WORK *pl_w )
{
	static void ( *wave_jmp[])( PLAYER_WORK *pl_w ) = {
		wave_init, wave_move,
	} ;

	wave_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void wave_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->atck_no = pl1a_at_wave ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 4 ) ;
}

/*==============================================================*/
/*			A shot in front of him, at the height of the		*/
/*			floor												*/
/*==============================================================*/
static void wave_move( PLAYER_WORK *pl_w )
{
	VEC3 pos ;

	if( pl_char_move( pl_w ) < 0 ){
		stand_ret( pl_w ) ;
	} else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pos = pl_w->pos ;
		if( pl_w->disp_dir ) pos.x += pl_w->scale.x * 320.0f ;
		else pos.x -= pl_w->scale.x * 320.0f ;
		pos.y = pl_w->ground_y + -40.0f ;
		shl1a_00_pos_set( pl_w, &pos, 4, 0 ) ;
	}
}

static void spray_init( PLAYER_WORK *pl_w );
static void spray_move( PLAYER_WORK *pl_w );
static void spray_shot( PLAYER_WORK *pl_w );
static void spray_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_spray( PLAYER_WORK *pl_w )
{
	static void ( *spray_jmp[])( PLAYER_WORK *pl_w ) = {
		spray_init, spray_move, spray_shot, spray_end,
	} ;

	spray_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void spray_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->atck_no = pl1a_at_spray ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 5 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
}

/*--------------------------------------------------------------*/
static void spray_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 3 ;
		pl_w->timer1 = 0 ;
		pl_w->dir = Rnd() ;
		pl_w->sub_type = Rnd() % 3 ;
	}
}

/*--------------------------------------------------------------*/
static void spray_shot( PLAYER_WORK *pl_w )
{
	Uint8 no ;

	pl_char_move( pl_w ) ;
	if( --pl_w->timer1 <= 0 ){
		pl_w->timer1 = Rnd() & 4 ;
		if( pl_w->dir = ( pl_w->dir + 1 ) & 1 ){
			pl_w->timer1 += 8 ;
			pl_w->timer0 -- ;
			no = pl_w->sub_type ;
			pl_w->sub_type = ( pl_w->sub_type + 1 ) % 3 ;
		} else {
			no = Rnd() % 3 + 0x80 ;
		}
		shl1a_00_set( pl_w, 6, no ) ;
		if( pl_w->timer0 <= 0 ) pl_w->routine_2 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void spray_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) stand_ret( pl_w ) ;
}

static void blast_init( PLAYER_WORK *pl_w );
static void blast_move( PLAYER_WORK *pl_w );
static void blast_wait( PLAYER_WORK *pl_w );
static void blast_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_blast( PLAYER_WORK *pl_w )
{
	static void ( *blast_jmp[])( PLAYER_WORK *pl_w ) = {
		blast_init, blast_move, blast_wait, blast_end,
	} ;

	blast_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void blast_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->atck_no = pl1a_at_blast ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 6 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
}

/*--------------------------------------------------------------*/
static void blast_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		shl1a_00_set( pl_w, 8, 0 ) ;
		pl_w->timer0 = 120 ;
	}
}

/*--------------------------------------------------------------*/
static void blast_wait( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( --pl_w->timer0 < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, 7 ) ;
	}
}

/*--------------------------------------------------------------*/
static void blast_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			He sinks away and comes up again in front of the	*/
/*			opponent; his moves from here to sp_fade keep			*/
/*			their animations in group 19, the win poses'		*/
/*			group for the others								*/
/*==============================================================*/
static void sp_win( PLAYER_WORK *pl_w )
{
	Float ofs ;

	switch( pl_w->routine_2 ){
	case 0:
		if( scr_in_x_ck( pl_w ) ){
			pl_w->routine_2 = 2 ;
			break ;
		}
		pl_w->routine_2 = 1 ;
		pl_dir_set( pl_w ) ;
		ofs = pl_w->scale.x * DOT_X( 64.0f ) ;
		if( pl_w->pl_dir ) ofs = -ofs ;
		pl_w->pos.x = scr_work.eye[0].x + ofs ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_char_set( pl_w, Char_Stand, 4 ) ;
		break ;
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_w->routine_2 ++ ;
		break ;
	case 2:
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, 19, 0 ) ;
		break ;
	case 3:
		pl_char_move( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
static void sp_win_voice( PLAYER_WORK *pl_w )
{
	Float ofs ;

	switch( pl_w->routine_2 ){
	case 0:
		if( scr_in_x_ck( pl_w ) ) pl_w->routine_2 = 2 ;
		else {
			pl_w->routine_2 = 1 ;
			pl_dir_set( pl_w ) ;
			ofs = pl_w->scale.x * DOT_X( 64.0f ) ;
			if( pl_w->pl_dir ) ofs = -ofs ;
			pl_w->pos.x = scr_work.eye[0].x + ofs ;
			pl_w->pos.y = pl_w->ground_y ;
			pl_char_set( pl_w, Char_Stand, 4 ) ;
		}
		break ;
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_w->routine_2 ++ ;
		break ;
	case 2:
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, 19, 1 ) ;
		break ;
	case 3:
		if( pl_char_move( pl_w ) < 0 ){
			pl_w->routine_2 ++ ;
			pl_char_set( pl_w, 19, 2 ) ;
			pl_w->timer0 = 145 ;
		}
		break ;
	case 4:
		if( pl_w->timer0 > 0 && !--pl_w->timer0 ) pl_voice_req( pl_w, 3 ) ;
		pl_char_move( pl_w ) ;
		break ;
	}
}

static void throw_init( PLAYER_WORK *pl_w );
static int throw_catch_ck( PLAYER_WORK *pl_w );
static void throw_wait( PLAYER_WORK *pl_w );
static void throw_throw( PLAYER_WORK *pl_w );
static void throw_end( PLAYER_WORK *pl_w );
static void throw_ret( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A throw: he grabs the opponent, leaps and drops		*/
/*			on him												*/
/*==============================================================*/
static void sp_throw( PLAYER_WORK *pl_w )
{
	static void ( *sp_throw_jmp[])( PLAYER_WORK *pl_w ) = {
		throw_init, throw_wait, throw_throw, throw_end,
	} ;

	sp_throw_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void throw_init( PLAYER_WORK *pl_w )
{
	VEC3 pos ;
	Sint8 hit ;

	pl_w->catch_flag = 1 ;
	if( !pl_w->routine_3 ){
		pl_w->routine_3 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->atck_no = pl1a_at_throw ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, 19, 3 ) ;
	} else {
		if( pl_char_move( pl_w ) < 0 ){
			if( pl_w->target_w->routine_1 != PL_CAUGHT ){
				throw_ret( pl_w ) ;
				return ;
			}
			pl_w->routine_2 ++ ;
			pl_w->routine_3 = 0 ;
		} else {
			hit = pl_w->hit_flag ;
			if( !( hit & 0x08 ) && hit ){
				pl_w->hit_flag |= HR_ONCE ;
				if( throw_catch_ck( pl_w ) ){
					pl_w->catch_no = 0xc0 ;
					pl_sp_catch_set( pl_w, pl_w->target_w ) ;
				}
			}
		}
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pos = pl_w->pos ;
			if( pl_w->disp_dir ) pos.x += pl_w->scale.x * 320.0f ;
			else pos.x -= pl_w->scale.x * 320.0f ;
			pos.y = pl_w->ground_y + -40.0f ;
			shl1a_00_pos_set( pl_w, &pos, 4, 1 ) ;
		}
	}
}

/*==============================================================*/
/*			Whether the hit can turn into the throw				*/
/*==============================================================*/
static int throw_catch_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( !( pl_w->hit_flag & HR_HIT ) ) return 0 ;
	if( pl_w->em_w->work_kind ) return 0 ;
	if( pl_w->hit_flag & ( HR_GUARD | HR_ARMOR ) ) return 0 ;
	if( pl_w != pl_w->em_w->dm_hit_w ) return 0 ;
	em = pl_w->em_w ;
	if( em->team_no ) return 0 ;
	if( em->pl_stance == STANCE_DOWN ) return 0 ;
	if( ABYSS_CK( em ) ) return 0 ;
	if( em->routine_1 != PL_DAMAGE ) return 0 ;
	return 1 ;
}

/*--------------------------------------------------------------*/
static void throw_wait( PLAYER_WORK *pl_w )
{
	pl_w->catch_flag = 1 ;
	if( pl_w->target_w->routine_1 != PL_CAUGHT ){
		throw_ret( pl_w ) ;
		return ;
	}
	if( pl_w->target_w->spd.y < 0.0f ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
	}
}

static void throw_jump( PLAYER_WORK *pl_w );
static void throw_fly( PLAYER_WORK *pl_w );
static void throw_fall( PLAYER_WORK *pl_w );
static void throw_land( PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void throw_throw( PLAYER_WORK *pl_w )
{
	static void ( *throw_jmp[])( PLAYER_WORK *pl_w ) = {
		throw_jump, throw_fly, throw_fall, throw_land,
	} ;

	pl_w->catch_flag = 1 ;
	throw_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void throw_jump( PLAYER_WORK *pl_w )
{
	Float ofs ;

	pl_w->routine_3 ++ ;
	pl_w->pos.x = pl_w->target_w->pos.x ;
	ofs = DOT_X( 640.0f ) ;
	pl_w->spd.x = -DOT_X( 8.0f ) ;
	if( pl_w->disp_dir ){
		ofs = -DOT_X( 640.0f ) ;
		pl_w->spd.x = -pl_w->spd.x ;
	}
	pl_w->spd.y = DOT_Y( 16.0f ) ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = -DOT_Y( 1.0f ) ;
	pl_w->timer0 = 28 ;
	pl_w->timer1 = 4 ;
	pl_w->pos.x += ofs ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_char_set( pl_w, 19, 4 ) ;
}

/*--------------------------------------------------------------*/
static void throw_fly( PLAYER_WORK *pl_w )
{
	if( pl_w->timer1 ){
		pl_w->timer1 -- ;
		pl_voice_req( pl_w, 3 ) ;
		pl_voice_req( pl_w, 22 ) ;
		return ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( --pl_w->timer0 < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->catch_w->disp_flag = 0 ;
		pl_w->cg_shadow = 0xff ;
		pl_char_set( pl_w, 19, 5 ) ;
	}
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		throw_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void throw_fall( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ef ;
	Float ofs ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_3 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pls1a_00_set( pl_w, 2 ) ;
		if( ef = pls1a_00_set( pl_w, 2 ) ) ef->sub_type = 1 ;
		ofs = pl_w->scale.x * 200.0f ;
		if( !pl_w->disp_dir ) ofs = -ofs ;
		pl_w->pos.x += ofs ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Special, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void throw_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_char_set( pl_w, 19, 6 ) ;
	}
}

/*--------------------------------------------------------------*/
static void throw_end( PLAYER_WORK *pl_w )
{
	pl_w->catch_flag = 1 ;
	pl_char_move( pl_w ) ;
}

static void sink_init( PLAYER_WORK *pl_w );
static void sink_move( PLAYER_WORK *pl_w );
static void sink_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_sink( PLAYER_WORK *pl_w )
{
	static void ( *sink_jmp[])( PLAYER_WORK *pl_w ) = {
		sink_init, sink_move, sink_end,
	} ;

	PL1A_LW( pl_w )->body_atck_flag = 1 ;
	sink_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void sink_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->disp_flag = 0 ;
	pl_w->timer0 = 270 ;
	pl_w->timer1 = 0 ;
	pl_char_set( pl_w, 19, 7 ) ;
}

/*--------------------------------------------------------------*/
static void sink_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->routine_3 ){
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pl_w->routine_3 ++ ;
		}
	} else if( !pl_w->cg_type ){
		pl_w->alpha = ( Float )pl_w->cg_ctr / ( Float )pl_w->cg_adrs[2] ;
	} else {
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void sink_end( PLAYER_WORK *pl_w )
{
	pl_w->disp_flag = 0 ;
	if( --pl_w->timer0 < 0 ) stand_ret( pl_w ) ;
	else if( --pl_w->timer1 < 0 ){
		pl_w->timer1 = 12 ;
		pls1a_00_set( pl_w, 3 ) ;
	}
}

/*==============================================================*/
static void sp_fade( PLAYER_WORK *pl_w )
{
	PL1A_LW( pl_w )->body_atck_flag = 1 ;
	switch( pl_w->routine_2 ){
	case 0:
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->timer0 = 270 ;
		pl_char_set( pl_w, 19, 8 ) ;
		break ;
	case 1:
		pl_char_move( pl_w ) ;
		if( !pl_w->routine_3 ){
			if( pl_w->cg_type ){
				pl_w->cg_type = 0 ;
				pl_w->routine_3 ++ ;
			}
		} else if( !pl_w->cg_type ){
			pl_w->alpha = ( Float )pl_w->cg_ctr / ( Float )pl_w->cg_adrs[2] ;
		} else {
			pl_w->routine_2 ++ ;
			pl_w->routine_3 = 0 ;
		}
		break ;
	case 2:
		pl_w->disp_flag = 0 ;
		if( --pl_w->timer0 < 0 ) stand_ret( pl_w ) ;
		break ;
	}
}

static void flame_init( PLAYER_WORK *pl_w );
static void flame_wait( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_flame( PLAYER_WORK *pl_w )
{
	static void ( *flame_jmp[])( PLAYER_WORK *pl_w ) = {
		flame_init, flame_wait,
	} ;

	flame_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void flame_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->disp_flag = 0 ;
	pl_w->timer0 = 270 ;
	pl_w->timer1 = 0 ;
	shl1a_02_set( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void flame_wait( PLAYER_WORK *pl_w )
{
	pl_w->disp_flag = 0 ;
	if( --pl_w->timer0 < 0 ) stand_ret( pl_w ) ;
}

/****************************************************************/
int pl1a_catch_ck( PLAYER_WORK *pl_w )
{
	return 0 ;
}

/****************************************************************/
void pl1a_catch_set( PLAYER_WORK *pl_w )
{
}

/****************************************************************/
void pl1a_catch( PLAYER_WORK *pl_w )
{
}

/****************************************************************/
/*			The thrown player, until he lands					*/
/****************************************************************/
void pl1a_hold( PLAYER_WORK *pl_w )
{
	pl_w->prio = pl_w->catch_w->prio - 1 ;
	if( pl_w->routine_2 == 0 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = pl_w->back_spd_x ;
		pl_w->spd.y = pl_w->back_spd_y ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = -1.4732143f ;
		pl_w->timer1 = 0 ;
	} else if( pl_w->routine_2 == 1 ){
		dm_blow_move( pl_w ) ;
		pl_char_move( pl_w ) ;
		if( pl_w->timer0 && !--pl_w->timer0 ) dm_blow_char_set( pl_w ) ;
		if( !pl_w->timer1 && pl_w->spd.y < 0.0f && !pl_w->timer0 ){
			pl_w->timer1 = 0xff ;
			dm_fall_char_set( pl_w ) ;
		}
		if( !pl_w->disp_flag ) pl_w->routine_2 ++ ;
	} else catch_pos_set( pl_w->catch_w, pl_w ) ;
}

static void cpu_ctrl( PLAYER_WORK *pl_w );
static int walk_ck( PLAYER_WORK *pl_w, int kind );
static int act_set( PLAYER_WORK *pl_w, Uint8 act );
static void ctrl_act( PLAYER_WORK *pl_w );
static void ctrl_damage( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Runs him each frame in place of the common			*/
/*			player control										*/
/****************************************************************/
void pl1a_ctrl( PLAYER_WORK *pl_w )
{
	static void ( *ctrl_jmp[])( PLAYER_WORK *pl_w ) = {
		ctrl_act, ctrl_act, ctrl_act, ctrl_damage,
	} ;

	if( pl_w->cpu_flag ) cpu_ctrl( pl_w ) ;
	pl_sw_set( pl_w ) ;
	pl_wait_com_ck( pl_w ) ;
	pl1a_main( pl_w ) ;
	if( !pl_w->armor_flag && pl_w->armor_timer ){
		if( --pl_w->armor_timer <= 0 ){
			pl_w->armor_timer_flag = 0 ;
			armor_init( pl_w ) ;
			pl_color_set( pl_w, 1 ) ;
		} else pl_w->armor_timer_flag = 1 ;
	}
	if( pl_w->armor_flag || pl_w->armor_timer_flag ) armor_move( pl_w ) ;
	armor_back_move( pl_w ) ;
	if( pl_w->hit_stop ){
		pl_w->hit_stop -- ;
		pl_w->pl_stop_new |= pl_w->pl_new_lvbt ;
		pl_w->pl_stop_now |= pl_w->pl_now_lvbt ;
		dm_ko_shake( pl_w ) ;
		if( pl_w->routine_1 != PL_DAMAGE ) pl_frame_jump( pl_w ) ;
		if( pl_w->routine_1 == PL_CATCH ) pl_w->catch_flag = 1 ;
	} else {
		if( pl_w->catch_ng_timer ) pl_w->catch_ng_timer -- ;
		if( pl_w->nml_catch_ng_timer ) pl_w->nml_catch_ng_timer -- ;
		if( pl_w->catch_atck_ng_timer ) pl_w->catch_atck_ng_timer -- ;
		if( pl_w->hit_ng_timer ) pl_w->hit_ng_timer -- ;
		if( pl_w->sp_ng_timer ) pl_w->sp_ng_timer -- ;
		if( pl_w->atck_ng_timer ) pl_w->atck_ng_timer -- ;
		if( pl_w->hyper_bg_tm && --pl_w->hyper_bg_tm <= 0 ) pl_w->hyper_bg = 0 ;
		if( pl_w->atck_plus_timer && !--pl_w->atck_plus_timer ) pl_w->atck_plus = 0 ;
		if( pl_w->def_plus_timer && !--pl_w->def_plus_timer ) pl_w->def_plus = 0 ;
		ctrl_jmp[pl_w->routine_1]( pl_w ) ;
		if( pl_w->target_w->fly_screen_flag ){
			if( pl_w->target_w->routine_1 == PL_NORM ) pl_w->target_w->fly_screen_flag = 0 ;
		}
	}
	pl_w->cg_shadow = 0xff ;
	pl_w->atck_flag = HIT_PL ;
	if( pl_w->hit_ng_timer ) pl_w->atck_flag = 0 ;
	pl_w->dm_flag = HIT_PL ;
	if( pl_w->catch_ng_timer || pl_w->routine_1 == PL_CATCH || ( pl_w->routine_1 == PL_CAUGHT && !pl_w->catch_type ) ) pl_w->dm_flag = 0 ;
}

static void act_stand( PLAYER_WORK *pl_w );
static void act_walk( PLAYER_WORK *pl_w );
static void act_lose( PLAYER_WORK *pl_w );

/*==============================================================*/
static void ctrl_act( PLAYER_WORK *pl_w )
{
	static void ( *act_jmp[])( PLAYER_WORK *pl_w ) = {
		act_stand, act_walk, act_stand, act_stand, act_stand, act_stand, act_stand, act_stand,
		act_stand, act_stand, act_stand, act_stand, act_stand, act_stand, act_stand, act_stand,
		act_stand, act_stand, act_stand, act_stand, act_stand, pl1a_sp_atck, pl1a_win, act_lose,
		act_stand, act_stand, act_stand, act_stand, act_stand, pl1a_sp_atck, act_stand, act_stand,
		act_stand, act_stand,
	} ;

	combo_reset( pl_w ) ;
	pl_w->unk_228 = 0 ;
	act_jmp[pl_w->act_st]( pl_w ) ;
}

/*==============================================================*/
static void act_stand( PLAYER_WORK *pl_w )
{
	if( vs_sys_w->vs_st >= VS_WIN && !pl_w->win_flag ){
		pl_w->win_flag = 1 ;
		act_set( pl_w, 22 ) ;
		return ;
	}
	if( !walk_ck( pl_w, 0 ) ){
		pl_char_move( pl_w ) ;
		if( pl_w->pos.y > pl_w->ground_y ) pl_w->pos.y = pl_w->ground_y ;
	}
}

/*==============================================================*/
static void act_walk( PLAYER_WORK *pl_w )
{
	switch( pl_w->routine_2 ){
	case 0:
		PL1A_LW( pl_w )->body_atck_flag = 1 ;
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Stand, 2 ) ;
		break ;
	case 1:
		PL1A_LW( pl_w )->body_atck_flag = 1 ;
		pl_char_move( pl_w ) ;
		if( !pl_w->routine_3 ){
			if( pl_w->cg_type ){
				pl_w->cg_type = 0 ;
				pl_w->routine_3 ++ ;
			}
		} else if( !pl_w->cg_type ){
			pl_w->alpha = ( Float )pl_w->cg_ctr / ( Float )pl_w->cg_adrs[2] ;
		} else {
			pl_w->routine_2 ++ ;
			pl_w->routine_3 = 0 ;
			pl_w->timer0 = 30 ;
			pl_w->spd.x = -DOT_X( 8.0f ) ;
			pl_w->acc.x = 0.0f ;
			if( pl_w->disp_dir ) pl_w->spd.x = -pl_w->spd.x ;
		}
		break ;
	case 2:
		PL1A_LW( pl_w )->body_atck_flag = 1 ;
		pl_w->timer0 -- ;
		pl_walk_spd_set( pl_w ) ;
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		PL1A_LW( pl_w )->body_alpha = ( Float )pl_w->timer0 / 30.0f ;
		if( !pl_w->timer0 ){
			pl_w->routine_2 ++ ;
			PL1A_LW( pl_w )->body_atck_flag = 0 ;
		}
		break ;
	case 3:
		if( !( vs_sys_work.ctrl_flag & CTRL_NO_INPUT ) && ( pl_w->pl_new_lvbt & ( KEY_FORWARD | KEY_BACK ) ) ){
			pl_walk_spd_set( pl_w ) ;
			pl_w->pos.x += pl_w->spd.x ;
			pl_w->spd.x += pl_w->acc.x ;
		} else {
			pl_w->routine_2 ++ ;
			pl_dir_set( pl_w ) ;
			pl_char_set( pl_w, Char_Stand, 4 ) ;
		}
		break ;
	case 4:
		if( pl_char_move( pl_w ) < 0 ) stand_ret( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
static void act_lose( PLAYER_WORK *pl_w )
{
	Float ofs ;

	pl_w->disp_flag = 0 ;
	pl_w->catch_ng_timer = 16 ;
	pl_w->dm_flag = 0 ;
	pl_w->cg_shadow = 0xff ;
	switch( pl_w->routine_2 ){
	case 0:
		ofs = pl_w->scale.x * 40.0f ;
		if( !pl_w->disp_dir ) ofs = -ofs ;
		pl_w->pos.x = scr_work.eye[0].x - ofs ;
		PL1A_LW( pl_w )->body_atck_flag = 1 ;
		if( vs_sys_w->vs_st >= VS_EXIT ){
			pl_w->routine_2 ++ ;
			PL1A_LW( pl_w )->body_atck_flag = 0 ;
			shl1a_02_set( pl_w ) ;
			pl_w->timer0 = 220 ;
		}
		break ;
	case 1:
		if( --pl_w->timer0 <= 0 ){
			pl_w->routine_2 ++ ;
			fade_set( -1, 32, FADE_OUT ) ;
		}
		break ;
	}
}

/*--------------------------------------------------------------*/
static void ctrl_damage( PLAYER_WORK *pl_w )
{
	PL1A_LW( pl_w )->damage_flag = 1 ;
	PL1A_LW( pl_w )->body_alpha = 1.0f ;
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
	} else {
		dm_back_move( pl_w ) ;
		if( pl_char_move( pl_w ) < 0 ) stand_ret( pl_w ) ;
	}
}

/****************************************************************/
/*			Nothing calls it, and its switch has no work		*/
/*			left												*/
/****************************************************************/
void pl1a_dummy( PLAYER_WORK *pl_w )
{
	switch( pl_w->routine_1 ){
	case PL_NORM:
		break ;
	}
}

/*==============================================================*/
static void stand_ret( PLAYER_WORK *pl_w )
{
	act_set( pl_w, 0 ) ;
}

/*==============================================================*/
static void throw_ret( PLAYER_WORK *pl_w )
{
	stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			Starts a walk while the stick is held to a side		*/
/*==============================================================*/
static int walk_ck( PLAYER_WORK *pl_w, int kind )
{
	Uint16 sw = kind ? pl_w->pl_now_lvbt : pl_w->pl_new_lvbt ;

	if( !( sw & ( KEY_FORWARD | KEY_BACK ) ) ) return 0 ;
	act_set( pl_w, 1 ) ;
	return 1 ;
}

/*==============================================================*/
static int act_set( PLAYER_WORK *pl_w, Uint8 act )
{
	Uint8 type ;
	Sint8 win ;

	pl_w->disp_flag = 1 ;
	act_st_set( pl_w, act ) ;
	pl_w->routine_1 = pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->rot[2] = 0 ;
	pl_w->scale = pl_w->pl_scale ;
	vs_sys_work.move_again_flag = 1 ;
	switch( act ){
	case 1:
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->sjump_flag = 0 ;
		pl_w->pl_stance = STANCE_STAND ;
		break ;
	case 22:
		win = vs_sys_work.winner_id ;
		if( win < 0 ) type = 4 ;
		else {
			type = 0 ;
			if( win != pl_w->pl_no ) type ++ ;
			if( vs_sys_work.time_over ) type += 2 ;
		}
		pl_w->type = type ;
		pl_w->pl_stance = STANCE_STAND ;
		break ;
	case 0:
	default:
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->jump_dir = -1 ;
		pl_w->sjump_flag = 0 ;
		pl_w->pl_stance = STANCE_STAND ;
		if( pl_w->armor_flag & ARMOR_HEAVY ) pl_w->char_no.b.no = 1 ;
		else pl_w->char_no.b.no = 0 ;
		pl_char_set( pl_w, Char_Stand, pl_w->char_no.b.no ) ;
		break ;
	}
	return 1 ;
}

/* the computer's control */
static void cpu_norm( PLAYER_WORK *pl_w );
static void cpu_range( PLAYER_WORK *pl_w );
static void cpu_event( PLAYER_WORK *pl_w );
static void cpu_event_ck( PLAYER_WORK *pl_w );
static void cmd_wait( PLAYER_WORK *pl_w );
static void cmd_walk( PLAYER_WORK *pl_w );
static void cmd_special( PLAYER_WORK *pl_w );
static void cmd_rest( PLAYER_WORK *pl_w );

/*==============================================================*/
static void cpu_ctrl( PLAYER_WORK *pl_w )
{
	static void ( *cpu_jmp[])( PLAYER_WORK *pl_w ) = {
		cpu_norm, cpu_range, cpu_event,
	} ;

	if( vs_sys_w->vs_st == VS_FIGHT && !pl_w->hit_stop ){
		pl_w->pad_sw.now = 0 ;
		pl_w->cpu_sp_flag = 0 ;
		cpu_event_ck( pl_w ) ;
		cpu_jmp[pl_w->cpu_routine_1]( pl_w ) ;
	}
}

/* the frames a wait lasts, by the time left and at random */
static const Uint8 wait_time_tbl[3][16] = {
	{ 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 120, 120, 60, 60 },
	{ 180, 180, 180, 180, 180, 180, 180, 180, 120, 120, 120, 120, 60, 60, 60, 60 },
	{ 120, 120, 120, 120, 120, 120, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60 },
} ;
/* the computer's chances out of 32, by level of play */
static const Uint8 cpu_rate0[] = { 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28 } ;
static const Uint8 cpu_rate1[] = { 24, 24, 24, 26, 26, 26, 28, 28, 28, 30, 30, 30 } ;
static const Uint8 cpu_rate2[] = { 24, 24, 24, 24, 24, 24, 26, 26, 28, 28, 30, 30 } ;
static const Uint8 cpu_rate3[] = { 24, 24, 24, 24, 26, 26, 26, 26, 28, 28, 28, 28 } ;
static const Uint8 cpu_rate4[] = { 20, 20, 22, 22, 24, 24, 24, 24, 26, 26, 26, 26 } ;

/* the commands: what each does, and the lists of them */
static void ( *cmd_jmp[])( PLAYER_WORK *pl_w ) = {
	cmd_wait, cmd_walk, cmd_special, cmd_rest,
} ;

static Uint8 norm_cmd[] = { 0, 10, 1, 10, 0, 10, 3, 0, 10, 3, 10, 10, 3, 99 } ;
static Uint8 time_lv_tbl[] = { 0, 0, 0, 1, 1, 2, 2, 2, 3, 3 } ;
static Uint8 vital_lv_tbl[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 } ;
static Uint8 wait_tbl[] = { 30, 15, 10, 0 } ;
static Uint8 far_cmd0[] = { 2, 1, 2, 2, 99 } ;
static Uint8 far_cmd1[] = { 2, 2, 2, 1, 99 } ;
static Uint8 far_cmd2[] = { 2, 2, 2, 1, 99 } ;
static Uint8 far_cmd3[] = { 2, 1, 2, 2, 2, 3, 99 } ;
static Uint8 *far_cmd[] = { far_cmd0, far_cmd1, far_cmd2, far_cmd3 } ;
static Uint8 edge_cmd0[] = { 1, 50, 99 } ;
static Uint8 edge_cmd1[] = { 1, 50, 99 } ;
static Uint8 edge_cmd2[] = { 1, 20, 2, 1, 99 } ;
static Uint8 edge_cmd3[] = { 1, 30, 2, 1, 99 } ;
static Uint8 *edge_cmd[] = { edge_cmd0, edge_cmd1, edge_cmd2, edge_cmd3 } ;
static const Uint8 *rate_tbl[] = {
	cpu_rate2, cpu_rate2, cpu_rate3, cpu_rate3, cpu_rate2, cpu_rate4,
} ;
static Uint8 r0_sel[4][16] = {
	{ 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3 },
	{ 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3 },
	{ 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3 },
	{ 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3 },
} ;
static Uint8 r0_cmd0[] = { 4, 30, 0, 2, 2, 4, 30, 0, 2, 1, 5, 99 } ;
static Uint8 r0_cmd1[] = { 4, 30, 0, 7, 4, 10, 0, 2, 3, 4, 10, 0, 2, 2, 8, 5, 99 } ;
static Uint8 r0_cmd2[] = { 4, 30, 0, 7, 2, 2, 8, 5, 99 } ;
static Uint8 r0_cmd3[] = { 4, 30, 0, 2, 4, 4, 30, 0, 2, 2, 5, 99 } ;
static Uint8 *r0_cmd[] = { r0_cmd0, r0_cmd1, r0_cmd2, r0_cmd3 } ;
static Uint8 r1_sel[4][16] = {
	{ 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3 },
	{ 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3 },
	{ 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3 },
	{ 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3 },
} ;
static Uint8 r1_cmd0[] = { 4, 20, 0, 2, 4, 4, 10, 0, 2, 3, 5, 99 } ;
static Uint8 r1_cmd1[] = { 4, 10, 0, 2, 3, 2, 4, 4, 10, 0, 2, 1, 5, 99 } ;
static Uint8 r1_cmd2[] = { 4, 10, 0, 2, 2, 4, 10, 0, 2, 4, 5, 99 } ;
static Uint8 r1_cmd3[] = { 4, 10, 0, 2, 3, 4, 10, 0, 2, 1, 5, 99 } ;
static Uint8 *r1_cmd[] = { r1_cmd0, r1_cmd1, r1_cmd2, r1_cmd3 } ;
static Uint8 r2_sel[4][16] = {
	{ 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3 },
	{ 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3 },
	{ 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3 },
	{ 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3 },
} ;
static Uint8 r2_cmd0[] = { 4, 10, 0, 2, 3, 4, 10, 0, 2, 2, 4, 8, 0, 2, 5, 4, 10, 0, 2, 1, 5, 99 } ;
static Uint8 r2_cmd1[] = { 4, 10, 0, 2, 1, 4, 10, 0, 2, 4, 4, 10, 0, 2, 3, 5, 99 } ;
static Uint8 r2_cmd2[] = { 2, 4, 4, 10, 0, 2, 1, 5, 99 } ;
static Uint8 r2_cmd3[] = { 4, 10, 0, 2, 2, 4, 10, 0, 2, 3, 4, 8, 0, 2, 0, 5, 99 } ;
static Uint8 *r2_cmd[] = { r2_cmd0, r2_cmd1, r2_cmd2, r2_cmd3 } ;
static Uint8 r3_sel[4][16] = {
	{ 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3 },
	{ 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3 },
	{ 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3 },
	{ 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3 },
} ;
static Uint8 r3_cmd0[] = { 4, 10, 0, 2, 3, 4, 10, 0, 2, 0, 4, 10, 0, 2, 1, 5, 99 } ;
static Uint8 r3_cmd1[] = { 4, 10, 0, 2, 3, 4, 8, 0, 2, 1, 4, 10, 0, 2, 3, 5, 99 } ;
static Uint8 r3_cmd2[] = { 4, 10, 0, 2, 2, 4, 8, 0, 2, 4, 4, 10, 0, 2, 5, 4, 8, 0, 7, 2, 0, 8, 5, 99 } ;
static Uint8 r3_cmd3[] = { 4, 10, 0, 2, 2, 4, 10, 0, 2, 4, 4, 10, 0, 2, 3, 4, 10, 0, 2, 1, 5, 99 } ;
static Uint8 *r3_cmd[] = { r3_cmd0, r3_cmd1, r3_cmd2, r3_cmd3 } ;
static Uint8 r4_sel[4][16] = {
	{ 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3 },
	{ 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3 },
	{ 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3 },
	{ 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3 },
} ;
static Uint8 r4_cmd0[] = { 7, 4, 10, 0, 2, 2, 4, 10, 0, 2, 1, 8, 5, 99 } ;
static Uint8 r4_cmd1[] = { 7, 4, 10, 0, 2, 2, 4, 10, 0, 2, 1, 4, 10, 0, 2, 0, 8, 5, 99 } ;
static Uint8 r4_cmd2[] = { 7, 4, 10, 0, 2, 2, 4, 10, 0, 2, 1, 4, 10, 0, 2, 2, 4, 10, 0, 2, 1, 8, 5, 99 } ;
static Uint8 r4_cmd3[] = { 7, 4, 10, 0, 2, 2, 2, 1, 4, 10, 0, 2, 4, 4, 10, 0, 2, 1, 8, 5, 99 } ;
static Uint8 *r4_cmd[] = { r4_cmd0, r4_cmd1, r4_cmd2, r4_cmd3 } ;
static Uint8 ( *range_sel[])[16] = { r0_sel, r1_sel, r2_sel, r3_sel, r4_sel, r4_sel } ;
static Uint8 **range_cmd[] = { r0_cmd, r1_cmd, r2_cmd, r3_cmd, r4_cmd, r4_cmd } ;

static Uint8 cpu_lv_ck( PLAYER_WORK *pl_w );
static Uint8 *cpu_cmd_read( PLAYER_WORK *pl_w, Uint8 *cmd );
static Uint8 cpu_time_lv( void );
static Uint8 *cpu_cmd_get( PLAYER_WORK *pl_w );
static int cpu_far_ck( PLAYER_WORK *pl_w );
static int cpu_edge_ck( PLAYER_WORK *pl_w );
static int cpu_rate_ck( PLAYER_WORK *pl_w, const Uint8 *tbl );

/*==============================================================*/
/*			Mode 0: the same commands over and over				*/
/*==============================================================*/
static void cpu_norm( PLAYER_WORK *pl_w )
{
	switch( pl_w->cpu_routine_2 ){
	case 0:
		pl_w->cpu_routine_2 ++ ;
		pl_w->cpu_norm_sc.adrs = norm_cmd ;
	case 1:
		pl_w->cpu_routine_2 ++ ;
		if( cpu_lv_ck( pl_w ) ) pl_w->cpu_norm_sc.adrs = norm_cmd ;
		pl_w->cpu_norm_sc.adrs = cpu_cmd_read( pl_w, pl_w->cpu_norm_sc.adrs ) ;
		break ;
	case 2:
		cmd_jmp[pl_w->cpu_cmd]( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
/*			Mode 1: commands picked by how far away the			*/
/*			opponent is											*/
/*==============================================================*/
static void cpu_range( PLAYER_WORK *pl_w )
{
	switch( pl_w->cpu_routine_2 ){
	case 0:
		pl_w->cpu_routine_2 ++ ;
		pl_w->cpu_range_sc.adrs = cpu_cmd_get( pl_w ) ;
	case 1:
		pl_w->cpu_routine_2 ++ ;
		if( cpu_lv_ck( pl_w ) ) pl_w->cpu_range_sc.adrs = cpu_cmd_get( pl_w ) ;
		em_range( pl_w ) ;
		pl_w->cpu_range_sc.adrs = cpu_cmd_read( pl_w, pl_w->cpu_range_sc.adrs ) ;
		break ;
	case 2:
		cmd_jmp[pl_w->cpu_cmd]( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
/*			Mode 2: the commands cpu_event_ck() chose			*/
/*==============================================================*/
static void cpu_event( PLAYER_WORK *pl_w )
{
	switch( pl_w->cpu_routine_2 ){
	case 0:
		pl_w->cpu_routine_2 ++ ;
	case 1:
		pl_w->cpu_routine_2 ++ ;
		pl_w->cpu_event_sc.adrs = cpu_cmd_read( pl_w, pl_w->cpu_event_sc.adrs ) ;
		break ;
	case 2:
		cmd_jmp[pl_w->cpu_cmd]( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
/*			Sets the level from the time and the vitality		*/
/*			left, and says whether it changed					*/
/*==============================================================*/
static Uint8 cpu_lv_ck( PLAYER_WORK *pl_w )
{
	Uint8 lv, lv2, old ;
	Sint16 vital ;

	if( pl_w->cpu_lock_flag ) return 0 ;
	lv = time_lv_tbl[ 9 - vs_sys_w->round_time / 10 ] ;
	vital = pl_w->vital_new - pl_w->target_w->vital_new + 144 ;
	lv2 = vital_lv_tbl[ 40 - vital / 10 ] ;
	if( lv < lv2 ) lv = lv2 ;
	old = pl_w->cpu_lv ;
	pl_w->cpu_lv = lv % 4 ;
	if( pl_w->cpu_lv != old ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static void cmd_wait( PLAYER_WORK *pl_w )
{
	switch( pl_w->cpu_routine_3 ){
	case 0:
		pl_w->cpu_routine_3 ++ ;
		if( !pl_w->cpu_cmd_timer ) pl_w->cpu_cmd_timer = wait_time_tbl[cpu_time_lv()][Rnd() & 15] ;
	case 1:
		if( !--pl_w->cpu_cmd_timer ) pl_w->cpu_routine_2 = 1 ;
		break ;
	}
}

/*==============================================================*/
static void cmd_walk( PLAYER_WORK *pl_w )
{
	switch( pl_w->cpu_routine_3 ){
	case 0:
		pl_w->cpu_routine_3 ++ ;
		pl_w->cpu_work0.l = 480 ;
		break ;
	case 1:
		if( pl_w->act_st == N_WALK && pl_w->routine_2 == 3 && !--pl_w->cpu_cmd_timer ){
			pl_w->cpu_routine_3 = 2 ;
			return ;
		}
		if( --pl_w->cpu_work0.l < 0 ) pl_w->cpu_routine_3 = 2 ;
		if( pl_w->jump_dir == 0 && pl_w->disp_dir == 0 ) pl_w->pad_sw.now = KEY_LEFT ;
		else pl_w->pad_sw.now = KEY_RIGHT ;
		break ;
	case 2:
		if( pl_w->act_st != N_WALK ) pl_w->cpu_routine_2 = 1 ;
		break ;
	}
}

/*==============================================================*/
static void cmd_special( PLAYER_WORK *pl_w )
{
	switch( pl_w->cpu_routine_3 ){
	case 0:
		pl_w->cpu_sp_flag = 1 ;
		pl_w->cpu_routine_3 ++ ;
		break ;
	case 1:
		if( pl_w->act_st != N_SPECIAL && pl_w->act_st != N_SUPER ) pl_w->cpu_routine_2 = 1 ;
		break ;
	}
}

/*==============================================================*/
static void cmd_rest( PLAYER_WORK *pl_w )
{
	switch( pl_w->cpu_routine_3 ){
	case 0:
		pl_w->cpu_routine_3 ++ ;
		pl_w->cpu_cmd_timer += Rnd() % ( pl_w->cpu_wait_rnd + 1 ) + wait_tbl[pl_w->cpu_lv] + 1 ;
		break ;
	case 1:
		if( !--pl_w->cpu_cmd_timer ) pl_w->cpu_routine_2 = 1 ;
		break ;
	}
}

/*==============================================================*/
/*			Reads commands up to the next one that takes		*/
/*			time, and returns where it stopped					*/
/*==============================================================*/
static Uint8 *cpu_cmd_read( PLAYER_WORK *pl_w, Uint8 *cmd )
{
	Uint8 next ;

	do {
		next = 0 ;
		switch( *cmd++ ){
		case 0:
			pl_w->cpu_cmd_timer = *cmd ;
			cmd++ ;
			pl_w->cpu_cmd_old = pl_w->cpu_cmd ;
			pl_w->cpu_cmd = 0 ;
			pl_w->cpu_routine_3 = 0 ;
			pl_w->cpu_routine_4 = 0 ;
			break ;
		case 1:
			pl_w->cpu_cmd_timer = *cmd ;
			cmd++ ;
			pl_w->jump_dir = 0 ;
			pl_w->cpu_cmd_old = pl_w->cpu_cmd ;
			pl_w->cpu_cmd = 1 ;
			pl_w->cpu_routine_3 = 0 ;
			pl_w->cpu_routine_4 = 0 ;
			break ;
		case 10:
			pl_w->cpu_cmd_timer = *cmd ;
			cmd++ ;
			pl_w->jump_dir = 1 ;
			pl_w->cpu_cmd_old = pl_w->cpu_cmd ;
			pl_w->cpu_cmd = 1 ;
			pl_w->cpu_routine_3 = 0 ;
			pl_w->cpu_routine_4 = 0 ;
			break ;
		case 2:
			pl_w->cpu_sp_flag = 1 ;
			pl_w->cpu_sp_no = *cmd ;
			cmd++ ;
			pl_w->cpu_cmd_old = pl_w->cpu_cmd ;
			pl_w->cpu_cmd = 2 ;
			pl_w->cpu_routine_3 = 0 ;
			pl_w->cpu_routine_4 = 0 ;
			break ;
		case 3:
			if( !cpu_rate_ck( pl_w, cpu_rate1 ) ){
				next = 1 ;
				break ;
			}
			pl_w->cpu_routine_1 = 1 ;
			pl_w->cpu_routine_2 = 0 ;
			pl_w->cpu_routine_3 = 0 ;
			pl_w->cpu_routine_4 = 0 ;
			break ;
		case 5:
			if( !cpu_rate_ck( pl_w, rate_tbl[0] ) ){
				pl_w->cpu_routine_1 = 0 ;
				pl_w->cpu_routine_2 = 0 ;
				pl_w->cpu_routine_3 = 0 ;
				pl_w->cpu_routine_4 = 0 ;
				pl_w->ev_try_flag = 0 ;
				pl_w->cpu_lock_flag = 0 ;
				break ;
			}
			pl_w->cpu_routine_1 = 1 ;
			pl_w->cpu_routine_2 = 0 ;
			pl_w->cpu_routine_3 = 0 ;
			pl_w->cpu_routine_4 = 0 ;
			break ;
		case 4:
			pl_w->cpu_cmd_timer = *cmd ;
			cmd++ ;
			pl_w->cpu_wait_rnd = *cmd++ ;
			pl_w->cpu_cmd_old = pl_w->cpu_cmd ;
			pl_w->cpu_cmd = 3 ;
			pl_w->cpu_routine_3 = 0 ;
			pl_w->cpu_routine_4 = 0 ;
			break ;
		case 7:
			pl_w->cpu_lock_flag = 1 ;
			next = 1 ;
			break ;
		case 8:
			pl_w->cpu_lock_flag = 0 ;
			next = 1 ;
			break ;
		case 9:
			pl_w->ev_try_flag = 0 ;
			pl_w->cpu_lock_flag = 0 ;
			next = 1 ;
			break ;
		default:
			pl_w->cpu_routine_1 = 0 ;
			pl_w->cpu_routine_2 = 0 ;
			pl_w->cpu_routine_3 = 0 ;
			pl_w->cpu_routine_4 = 0 ;
			pl_w->ev_try_flag = 0 ;
			pl_w->cpu_lock_flag = 0 ;
			break ;
		}
	} while( next ) ;
	return cmd ;
}

/*==============================================================*/
static Uint8 cpu_time_lv( void )
{
	Sint16 t ;

	t = cpu_level_work.level - 20 ;
	if( t < 0 ) t = 0 ;
	if( t > 11 ) t = 11 ;
	return ( t >> 2 ) % 3 ;
}

/*==============================================================*/
/*			How far away the opponent is, 0 to 5: 4 and 5		*/
/*			are close, 0 to 3 farther and farther				*/
/*==============================================================*/
static Uint8 em_range( PLAYER_WORK *pl_w )
{
	Float x ;
	Uint8 range ;

	x = pl_w->target_w->pos.x - pl_w->pos.x ;
	if( pl_w->disp_dir ) x = -x ;
	if( x > -( pl_w->scale.x * DOT_X( 80.0f ) ) ){
		range = 4 ;
		if( x > pl_w->scale.x * 160.0f ) range ++ ;
	} else {
		range = 0 ;
		if( x < -( pl_w->scale.x * DOT_X( 136.0f ) ) ) range ++ ;
		if( x < -( pl_w->scale.x * 320.0f ) ) range ++ ;
		if( x < -( pl_w->scale.x * 400.0f ) ) range ++ ;
	}
	return range ;
}

/*==============================================================*/
/*			Starts mode 2 once a round, when the opponent is	*/
/*			close or he is near an end of the stage				*/
/*==============================================================*/
static void cpu_event_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->cpu_lock_flag ) return ;
	if( pl_w->act_st ) return ;
	if( cpu_far_ck( pl_w ) ) return ;
	if( cpu_edge_ck( pl_w ) ) return ;
}

/*==============================================================*/
static int cpu_far_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->ev_try_flag & 1 ) return 0 ;
	if( em_range( pl_w ) >= 4 ){
		pl_w->ev_try_flag |= 1 ;
		if( cpu_rate_ck( pl_w, cpu_rate0 ) ){
			pl_w->cpu_routine_1 = 2 ;
			pl_w->cpu_routine_2 = 0 ;
			pl_w->cpu_routine_3 = 0 ;
			pl_w->cpu_routine_4 = 0 ;
			pl_w->unk_481 = 0 ;
			pl_w->cpu_lock_flag = 1 ;
			pl_w->cpu_event_sc.adrs = far_cmd[Rnd() & 3] ;
			return 1 ;
		}
	}
	return 0 ;
}

/*==============================================================*/
static int cpu_edge_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->ev_try_flag & 2 ) return 0 ;
	if( scr_work.stage_left >= pl_w->pos.x || scr_work.stage_right <= pl_w->pos.x ){
		pl_w->ev_try_flag |= 2 ;
		pl_w->cpu_routine_1 = 2 ;
		pl_w->cpu_routine_2 = 0 ;
		pl_w->cpu_routine_3 = 0 ;
		pl_w->cpu_routine_4 = 0 ;
		pl_w->unk_481 = 1 ;
		pl_w->cpu_lock_flag = 1 ;
		pl_w->cpu_event_sc.adrs = edge_cmd[Rnd() & 3] ;
		return 1 ;
	}
	return 0 ;
}

/*==============================================================*/
/*			A chance out of 32, from a table of 12 by the		*/
/*			level of play										*/
/*==============================================================*/
static int cpu_rate_ck( PLAYER_WORK *pl_w, const Uint8 *tbl )
{
	Sint16 t ;
	Uint8 r ;

	t = cpu_level_work.level - 20 ;
	if( t < 0 ) t = 0 ;
	r = Rnd() & 31 ;
	if( r < tbl[t % 12] ) return 1 ;
	return 0 ;
}

/*==============================================================*/
/*			Picks the commands for the range and the level		*/
/*==============================================================*/
static Uint8 *cpu_cmd_get( PLAYER_WORK *pl_w )
{
	Uint8 range ;
	Uint8 ( *sel )[16] ;
	Uint8 **cmd ;

	range = em_range( pl_w ) ;
	sel = range_sel[range] ;
	pl_w->cpu_range_sc.no = sel[pl_w->cpu_lv][Rnd() & 15] ;
	cmd = range_cmd[range] ;
	return cmd[pl_w->cpu_range_sc.no] ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl1a.c" End of File	*/

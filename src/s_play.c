/* The game scene: setting up the players, the select, each fight and its end, and the attract
   demo's fight. */
#include "player.h"

extern void pl_work_clear( void ) ;
extern void pl_init( void ) ;
extern void pl_restart( void ) ;
extern void pl_work_disp( void ) ;
extern void pl_work_init( PLAYER_WORK *pl_w ) ;
extern int sprite_set_load( PLAYER_WORK *w, int no ) ;
extern void sprite_set_free( PLAYER_WORK *w, int no ) ;
extern void total_vol_init( void ) ;
extern void sel_main( void ) ;	/* the select */
extern void StartDemoMain( void ) ;
extern void pl_data_set( PLAYER_WORK *pl_w, int no ) ;
extern void bg_init( void ) ;
extern void disp_front_obj( void ) ;
extern void game_disp( void ) ;
extern void s_play_ef0_set( int no ) ;
extern void hyper_bg_set( void ) ;
extern void stage_set( void ) ;

static void game_pl_set( void ) ;
static void game_cpu_select( void ) ;
static void game_st_demo( void ) ;
static void game_fight( void ) ;
static void game_ranking( void ) ;
static void play_init( void ) ;
static void play_start( void ) ;
static void play_move( void ) ;
static void play_end( void ) ;
static void team_no_init( int side ) ;
static void team_no_set( int flag ) ;
static void work_kill( Uint8 type, Uint8 no, int list ) ;
void demo_pl_set( void ) ;
static void demo_start( void ) ;
static void demo_move( void ) ;

/****************************************************************/
/*			The game scene, run each frame						*/
/****************************************************************/
void game_main( void )
{
	static void ( *game_jmp[] )( void ) = {
		game_pl_set, game_cpu_select, game_st_demo, game_fight, game_ranking,
	} ;

	game_jmp[Sys->routine_0]() ;
}

/*==============================================================*/
/*			The game's start: who plays, the teams in their		*/
/*			chosen order, and the scores carried over			*/
/*==============================================================*/
static void game_pl_set( void )
{
	PLAYER_WORK *pl_w, *em ;
	int i ;
	Sint8 type[6], personal[6], color[6] ;
	int no ;

	Sys->routine_0 ++ ;
	Sys->routine_1 = 0 ;
	Sys->routine_2 = 0 ;
	Sys->routine_3 = 0 ;
	Sys->exit_flag = 0 ;
	Sys->unk_86 = 0 ;
	Sys->entry_stop = 1 ;
	Sys->name_stop = 0 ;
	for( i = 0 ; i < 6 ; i ++ ) pl_work[i].cpu_flag = 1 ;
	for( i = 0 ; i < 6 ; i ++ ){
		pl_w = &pl_work[i] ;
		no = pl_w->team_order ;
		type[i] = pl_work[( pl_w->pl_index & 1 ) + no * 2].load_type ;
		personal[i] = pl_work[( pl_w->pl_index & 1 ) + no * 2].pl_personal ;
		color[i] = pl_work[( pl_w->pl_index & 1 ) + no * 2].load_color ;
		pl_w->team_order = i / 2 ;
	}
	for( i = 0 ; i < 6 ; i ++ ){
		pl_w = &pl_work[i] ;
		pl_w->load_type = type[i] ;
		pl_w->pl_personal = personal[i] ;
		pl_w->load_color = color[i] ;
	}
	if( ( Sys->fight_side = Sys->play_side ) == 3 ){
		Sys->vs_flag = 1 ;
		Sys->stage_cnt = 0 ;
	} else {
		Sys->vs_flag = 0 ;
		pl_w = &pl_work[0] ;
		em = &pl_work[1] ;
		if( Sys->play_side != 1 ){
			pl_w = &pl_work[1] ;
			em = &pl_work[0] ;
		}
		pl_w->score[1] = pl_w->old_score_hi ;
		pl_w->score[0] = pl_w->old_score ;
		em->score[1] = 0 ;
		em->score[0] = 0 ;
		Sys->stage_cnt = pl_w->stage_cnt ;
		if( !Sys->speed_fix ) Sys->game_speed = pl_w->sel_speed ;
	}
	if( Sys->play_side & 1 ){
		pl_work[0].cpu_flag = 0 ;
		pl_work[2].cpu_flag = 0 ;
		pl_work[4].cpu_flag = 0 ;
	}
	if( Sys->play_side & 2 ){
		pl_work[1].cpu_flag = 0 ;
		pl_work[3].cpu_flag = 0 ;
		pl_work[5].cpu_flag = 0 ;
	}
	pl_work_clear() ;
	work_init() ;
	se_vol_up( 0, 1 ) ;
	se_vol_up( 1, 1 ) ;
}

/*==============================================================*/
/*			The select, until it is done						*/
/*==============================================================*/
static void game_cpu_select( void )
{
	if( Sys->sel_done_flag || Sys->vs_flag ){
		Sys->routine_0 ++ ;
		Sys->routine_1 = 0 ;
		Sys->routine_2 = 0 ;
		Sys->routine_3 = 0 ;
		Sys->exit_flag = 0 ;
		Sys->sel_done_flag = 0 ;
		Sys->entry_stop = 1 ;
	} else sel_main() ;
}

/*--------------------------------------------------------------*/
static void game_st_demo( void )
{
	StartDemoMain() ;
}

/*==============================================================*/
/*			The fights											*/
/*==============================================================*/
static void game_fight( void )
{
	static void ( *play_jmp[] )( void ) = {
		play_init, play_start, play_move, play_end,
	} ;

	play_jmp[Sys->routine_1]() ;
}

/*==============================================================*/
/*			Load the fight: the stage's music, the players'		*/
/*			sprites and the works								*/
/*==============================================================*/
static void play_init( void )
{
	PLAYER_WORK *pl_w ;
	int i ;

	if( load_ck() ) return ;
	Sys->routine_1 ++ ;
	Sys->entry_stop = 1 ;
	Sys->play_timer = 0 ;
	stream_req( stage_no ) ;
	Palette_Init() ;
	Texture_Init() ;
	TexLoad( 1 ) ;
	TexLoad( 2 ) ;
	if( Sys->play_mode != 64 ) TexLoad( 4 ) ;
	for( i = 0 ; i < 6 ; i ++ ){
		pl_w = &pl_work[i] ;
		pl_data_set( pl_w, i ) ;
		sprite_set_load( pl_w, i ) ;
	}
	vs_round_init() ;
	work_init() ;
	pl_init() ;
	hit_work_init() ;
	scr_work_init() ;
	fog_init() ;
	bright_init() ;
	if( Sys->play_mode != 64 ){
		stage_set() ;
		bg_init() ;
	}
}

/*==============================================================*/
/*			Start the fight										*/
/*==============================================================*/
static void play_start( void )
{
	PLAYER_WORK *pl_w ;
	int i ;

	Sys->routine_1 ++ ;
	Sys->entry_stop = 1 ;
	Sys->unk_87 = 0 ;
	Sys->play_timer = 0 ;
	Sys->hyper_bg = 0 ;
	for( i = 0 ; i < 6 ; i ++ ){
		pl_w = &pl_work[i] ;
		pl_w->routine_0 = 0 ;
		pl_w->routine_1 = 0 ;
		pl_w->routine_2 = 0 ;
		pl_w->routine_3 = 0 ;
	}
	pl_restart() ;
	hyper_bg_set() ;
	se_vol_up( 0, 1 ) ;
	se_vol_up( 1, 1 ) ;
	stream_start() ;
	s3l_fade_in_set( 1 ) ;
}

/*==============================================================*/
/*			A frame of the fight: the works move and are		*/
/*			hit, twice in a frame at the faster speeds			*/
/*==============================================================*/
static void play_move( void )
{
	if( Sys->join_side ){
		Sound_Heart() ;
		if( Sys->heart_step > 1 ) SysTransSet( disp_front_obj ) ;
		else SysTransSet( game_disp ) ;
	} else {
		if( vs_sys_work.vs_st == VS_FIGHT && !Sys->vs_flag ){
			if( Sys->play_side & 1 ) pl_work[0].play_time ++ ;
			if( Sys->play_side & 2 ) pl_work[1].play_time ++ ;
		}
		cpu_level_time() ;
		vs_sys_w->speed_flag = 0 ;
		if( vs_sys_w->speed_rate ) vs_sys_w->speed_cnt -- ;
again:
		if( Sys->play_timer != -1 ) Sys->play_timer ++ ;
		side_flag_clr() ;
		vs_sys_w->slow_step = ( vs_sys_w->slow_step + 1 ) & 15 ;
		all_work_move() ;
		hit_ck0() ;
		vs_sys_move() ;
		flash_move() ;
		if( vs_sys_w->speed_rate && !vs_sys_w->speed_cnt ){
			vs_sys_w->speed_cnt = vs_sys_w->speed_rate ;
			vs_sys_w->speed_flag = 0xff ;
			goto again ;
		}
		if( vs_sys_work.vs_st == VS_FIGHT ) vs_sys_work.fight_frame ++ ;
		disp_prio_clr() ;
		pl_work_disp() ;
		work_list_disp( W_PLS ) ;
		work_list_disp( W_PLS_ST ) ;
		work_list_disp( W_SHL ) ;
		work_list_disp( W_SHL_ST ) ;
		if( dbg_flag_ck( 0, 1 ) ) nlSetBorderColor( 0x7f7fff ) ;
		SysTransSet( game_disp ) ;
		if( dbg_flag_ck( 0, 1 ) ) nlSetBorderColor( 0x7fff7f ) ;
	}
}

/*==============================================================*/
/*			The fight is over: the winner's record, the next	*/
/*			fight's order, and the scores kept					*/
/*==============================================================*/
static void play_end( void )
{
	PLAYER_WORK *pl_w, *em ;
	int side, no ;
	int ch ;

	Sys->routine_0 ++ ;
	Sys->routine_1 = 0 ;
	Sys->routine_2 = 0 ;
	Sys->routine_3 = 0 ;
	Sys->exit_flag = 0 ;
	Sys->entry_stop = 1 ;
	if( Sys->ending_flag ){
		Sys->scene_no = 4 ;
		Sys->routine_0 = 0 ;
	}
	if( Sys->vs_flag ){
		if( vs_sys_work.winner_id == 1 ) Sys->rank_side = 1 ;
		else Sys->rank_side = 0 ;
	} else if( Sys->fight_side == 1 ) Sys->rank_side = 0 ;
	else Sys->rank_side = 1 ;
	if( vs_sys_work.winner_id < 0 ){
		Sys->play_side = 0 ;
		team_no_set( 3 ) ;
	} else {
		Sys->play_side &= 1 << vs_sys_work.winner_id ;
		team_no_set( 3 - Sys->play_side ) ;
	}
	if( Sys->event_flag ) Sys->play_side = 0 ;
	if( vs_sys_work.winner_id >= 0 ){
		side = vs_sys_work.winner_id ;
		no = side ^ 1 ;
		pl_w = &pl_work[side] ;
		if( Sys->vs_flag ){
			if( ++ pl_w->win_cnt > 99 ) pl_w->win_cnt = 99 ;
			if( pl_w->win_max < pl_w->win_cnt ) pl_w->win_max = pl_w->win_cnt ;
			Sys->stage_cnt = pl_w->stage_cnt ;
		} else if( !pl_w->cpu_flag ){
			Sys->stage_cnt ++ ;
			pl_w->stage_cnt = Sys->stage_cnt ;
			ch = ( pl_work + no )[0].load_type ;
			if( ch < 32 ) pl_w->beat_flag[0] |= 1 << ch ;
			else pl_w->beat_flag[1] |= 1 << ( ch - 32 ) ;
			ch = ( pl_work + no )[2].load_type ;
			if( ch < 32 ) pl_w->beat_flag[0] |= 1 << ch ;
			else pl_w->beat_flag[1] |= 1 << ( ch - 32 ) ;
			ch = ( pl_work + no )[4].load_type ;
			if( ch < 32 ) pl_w->beat_flag[0] |= 1 << ch ;
			else pl_w->beat_flag[1] |= 1 << ( ch - 32 ) ;
		}
	} else if( Sys->vs_flag ){	/* empty */
	} else {
	}
	pl_w = &pl_work[0] ;
	em = &pl_work[1] ;
	if( Sys->vs_flag ){
		vs_rank_entry( pl_w ) ;
		vs_rank_entry( em ) ;
	} else {
		if( !pl_w->cpu_flag ){
			pl_w->old_score_hi = pl_w->score[1] ;
			pl_w->old_score = pl_w->score[0] ;
			rank_entry( pl_w ) ;
		}
		if( !em->cpu_flag ){
			em->old_score_hi = em->score[1] ;
			em->old_score = em->score[0] ;
			rank_entry( em ) ;
		}
	}
	stream_stop() ;
	while( !nlaCheckWaitROMDMA() ) ;
	sound_all_off() ;
	se_all_off() ;
	disp_prio_clr() ;
	pl_work_disp() ;
	work_list_disp( W_PLS ) ;
	work_list_disp( W_PLS_ST ) ;
	work_list_disp( W_SHL ) ;
	work_list_disp( W_SHL_ST ) ;
	SysTransSet( game_disp ) ;
}

/*--------------------------------------------------------------*/
static void game_ranking( void )
{
	RankingMain() ;
}

/*==============================================================*/
/*			The side's three players in their places again		*/
/*==============================================================*/
static void team_no_init( int side )
{
	int i ;

	for( i = 0 ; i < 3 ; i ++ ) pl_work[i * 2 + side].team_order = i ;
}

/*==============================================================*/
/*			The same, for each side in flag						*/
/*==============================================================*/
static void team_no_set( int flag )
{
	if( flag & 1 ) team_no_init( 0 ) ;
	if( flag & 2 ) team_no_init( 1 ) ;
}

/****************************************************************/
/*			Back to the start: no one plays						*/
/****************************************************************/
void game_reset( void )
{
	Sys->scene_no = 0 ;
	Sys->routine_0 = 0 ;
	Sys->routine_1 = 0 ;
	Sys->routine_0 = 0 ;
	Sys->routine_2 = 0 ;
	Sys->routine_3 = 0 ;
	Sys->exit_flag = 0 ;
	Sys->play_side = 0 ;
	Sys->entry_side = 3 ;
	Sys->new_side = 3 ;
	Sys->join_side = 3 ;
	team_no_set( 3 ) ;
}

/*==============================================================*/
/*			Free the works of a list that belong to player		*/
/*			no of character type								*/
/*==============================================================*/
static void work_kill( Uint8 type, Uint8 no, int list )
{
	PLAYER_WORK *w ;

	w = work_top[list] ;
	if( w ){
		do {
			if( w->work_no == no && w->pl_type == type ) work_free( w ) ;
		} while( ( w = w->behind_w ) ) ;
	}
}

/****************************************************************/
/*			Change a player to the character chosen for it		*/
/****************************************************************/
void pl_change( PLAYER_WORK *pl_w )
{
	work_kill( pl_w->pl_type, pl_w->work_no, 3 ) ;
	work_kill( pl_w->pl_type, pl_w->work_no, 4 ) ;
	work_kill( pl_w->pl_type, pl_w->work_no, 2 ) ;
	work_kill( pl_w->pl_type, pl_w->work_no, 2 ) ;
	pl_w->pl_type = pl_w->load_type ;
	sprite_set_free( pl_w, pl_w->work_no ) ;
	tex_dummy_0( pl_w, pl_w->work_no ) ;
	sprite_set_load( pl_w, pl_w->work_no ) ;
	pl_w->routine_0 = 0 ;
	pl_w->routine_1 = 0 ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	if( !( pl_w->side_sw.now & ( KEY_LEFT | KEY_RIGHT ) ) ) pl_w->routine_1 = 1 ;
	pl_work_init( pl_w ) ;
	pl_w->team_ng_timer = pl_w->atck_ng_timer = 32 ;
}

/****************************************************************/
/*			Set up the attract demo's fight						*/
/****************************************************************/
void demo_init( void )
{
	memset( &sys_work.game_timer, 0, SYS_HALF ) ;
	Sys->entry_side = 3 ;
	Sys->play_side = 0 ;
	Sys->cpu_level_max = 1 ;
	Sys->unk_4c = 0 ;
	vs_sys_init() ;
	cpu_level_init() ;
	pl_restart() ;
	game_pl_set() ;
	demo_pl_set() ;
	Sys->play_side = 3 ;
	play_init() ;
	total_vol_init() ;
	if( Sys->vms_mode ) TexLoad( 14 ) ;
	vs_sys_work.gauge_level[0] = 5 ;
	vs_sys_work.gauge_level[1] = 5 ;
	Sys->routine_0 = 0 ;
	Sys->routine_1 = 0 ;
	Sys->routine_2 = 0 ;
	Sys->routine_3 = 0 ;
	Sys->exit_flag = 0 ;
}

/* the six characters of each attract demo */
const Sint8 demo_pl_tbl[4 * 6] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
	0x06, 0x07, 0x09, 0x0b, 0x0c, 0x0d,
	0x0f, 0x12, 0x13, 0x14, 0x15, 0x16,
	0x17, 0x1e, 0x1f, 0x28, 0x29, 0x2d,
} ;

/****************************************************************/
/*			The demo's six players, from the demo's number		*/
/****************************************************************/
void demo_pl_set( void )
{
	PLAYER_WORK *pl_w ;
	int i ;

	for( i = 0 ; i < 6 ; i ++ ){
		pl_w = &pl_work[i] ;
		pl_w->be_flag = 1 ;
		pl_w->disp_flag = 0 ;
		pl_w->pl_index = i ;
		pl_w->load_type = demo_pl_tbl[( Sys->demo_no & 3 ) * 6 + i] ;
		pl_w->pl_personal = 0 ;
		pl_w->load_color = 0 ;
		pl_w->team_order = i / 2 ;
	}
	stage_no = Sys->demo_no & 7 ;
	Sys->stage_cnt = Sys->demo_no & 7 ;
}

/****************************************************************/
/*			The attract demo, run each frame					*/
/****************************************************************/
void demo_main( void )
{
	static void ( *demo_jmp[] )( void ) = {
		demo_start, demo_move,
	} ;
	int i ;

	Sys->cpu_level_max = 1 ;
	Sys->game_timer ++ ;
	demo_jmp[Sys->scene_no]() ;
	for( i = 0 ; i < 6 ; i ++ ){
		pl_work[i].vital_new = 144 ;
		pl_work[i].vital_red = 144 ;
	}
	if( Sys->play_timer != -1 ) Sys->play_timer ++ ;
	side_flag_clr() ;
	vs_sys_work.slow_step = ( vs_sys_work.slow_step + 1 ) & 15 ;
	if( Sys->timer0 ){
		all_work_move() ;
		hit_ck0() ;
	}
	flash_move() ;
	disp_prio_clr() ;
	if( Sys->timer0 ){
		pl_work_disp() ;
		work_list_disp( W_PLS ) ;
		work_list_disp( W_PLS_ST ) ;
		work_list_disp( W_SHL ) ;
		work_list_disp( W_SHL_ST ) ;
	}
	SysTransSet( game_disp ) ;
}

/*==============================================================*/
/*			The demo's start: the 3D view, and the fight		*/
/*			under way											*/
/*==============================================================*/
static void demo_start( void )
{
	nlInitMatrix( matrix_stack, 64 ) ;
	nlMatrixMode( 3 ) ;
	nlUnitMatrix() ;
	nlPerspective( 0x1d28, 1.3333334f, 0.3f, 12000.0f ) ;
	nlMatrixMode( 1 ) ;
	nlInitDrawModel( draw_vertex_work, draw_light_work ) ;
	nlObjPutInit() ;
	nlLightInit() ;
	stage_set() ;
	play_start() ;
	Sys->scene_no ++ ;
	Sys->routine_0 = 0 ;
	Sys->routine_1 = 0 ;
	Sys->routine_2 = 0 ;
	Sys->routine_3 = 0 ;
	Sys->exit_flag = 0 ;
	Sys->timer0 = 1080 ;
	Sys->hyper_bg = 0 ;
	vs_sys_w = &vs_sys_work ;
	vs_sys_w->ctrl_flag = CTRL_PLAY ;
	vs_sys_w->first_hit = 0 ;
	vs_sys_w->time_over = 0 ;
	vs_sys_w->ko_hit_data = 0 ;
	vs_sys_w->ko_atck_no = 0 ;
	vs_sys_w->ko_at_w = 0 ;
	vs_sys_w->win_skip_flag = 0 ;
	vs_sys_w->vs_st = VS_FIGHT ;
	vs_sys_w->vs_timer = 0 ;
	scr_work.y_mode = 0 ;
	scr_work.x_mode = 0 ;
	pl_work[0].routine_0 = 0 ;
	pl_work[0].routine_1 = 1 ;
	pl_work[0].routine_2 = 0 ;
	pl_work[0].routine_3 = 0 ;
	pl_work_init( &pl_work[0] ) ;
	pl_work[1].routine_0 = 0 ;
	pl_work[1].routine_1 = 1 ;
	pl_work[1].routine_2 = 0 ;
	pl_work[1].routine_3 = 0 ;
	pl_work_init( &pl_work[1] ) ;
	if( Sys->vms_mode ) s_play_ef0_set( 0 ) ;
	fade_set( 0xff000000, 60, FADE_IN ) ;
}

/*==============================================================*/
/*			The demo's end: fade out and go back to the			*/
/*			title												*/
/*==============================================================*/
static void demo_move( void )
{
	Sys->timer0 -- ;
	if( Sys->timer0 == 60 ){
		fade_set( 0xff000000, 60, FADE_OUT ) ;
		s3l_fade_out_set( 60 ) ;
	}
	if( !Sys->timer0 ){
		Sys->task_step ++ ;
		Sys->scene_no = 0 ;
		Sys->routine_0 = 0 ;
		Sys->routine_1 = 0 ;
		Sys->routine_2 = 0 ;
		Sys->routine_3 = 0 ;
		Sys->exit_flag = 0 ;
		Sys->cpu_level_max = 3 ;
		work_init() ;
		memset( &sys_work.game_timer, 0, SYS_HALF ) ;
		memset( &vs_sys_work, 0, sizeof( VS_SYS_WORK ) ) ;
		sound_all_off() ;
		se_all_off() ;
		stream_stop() ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_play.c" End of File	*/

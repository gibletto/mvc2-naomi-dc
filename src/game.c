/* Task 2, the game: its set-up, then its steps run the scenes one after another. */
#include "player.h"

extern void entry_main( void ) ;
extern void total_vol_set( int vol ) ;
extern void pl_sw_copy( void ) ;
extern void SelectMainMove_yu( void ) ;
extern void game_main( void ) ;
extern void cont_main( void ) ;
extern void over_main( void ) ;
extern void staff_main( void ) ;

static void game_init( void ) ;
static void game_move( void ) ;
static void game_start( void ) ;
static void game_select( void ) ;
static void game_play( void ) ;
static void game_cont( void ) ;
static void game_staff( void ) ;
static void game_over( void ) ;
static void game_end( void ) ;

/****************************************************************/
/*			Task 2: the game									*/
/****************************************************************/
void task_game( void )
{
	static void ( *game_jmp[] )( void ) = {
		game_init,
		game_move,
	} ;

	entry_main() ;
#ifndef TARGET_DC
	Sys->game_timer ++ ;
	pl_sw_copy() ;
	game_jmp[Sys->task_step]() ;
	test_sw_ck() ;
#else
	/* a network game reads its buttons itself; the reset keys end the game */
	if( !net_play_flag ){
		Sys->game_timer ++ ;
		pl_sw_copy() ;
	}
	if( !soft_reset_ck( 1 ) ) game_jmp[Sys->task_step]() ;
#endif
}

/*==============================================================*/
/*			The game's set-up: the 3D view, and the six			*/
/*			player works										*/
/*==============================================================*/
static void game_init( void )
{
	int i ;

#ifdef TARGET_DC
	reset_lock = 0 ;
#endif
	Sys->task_step ++ ;
	Sys->entry_stop = 1 ;
	memset( &sys_work.game_timer, 0, SYS_HALF ) ;
	memset( &vs_sys_work, 0, sizeof( vs_sys_work ) ) ;
	Sys->new_side = Sys->entry_side ;
	Sys->cpu_level_max = 0 ;
	Sys->cpu_renda_level = Sys->renda_level ;
	Sys->attract_step = 6 ;
	Sys->demo_no = 0 ;
	Sys->disp_line = 3 ;
#ifdef TARGET_DC
	/* the mode's own set-up: points for starting an arcade game, the training's rules, the
	   network's */
#if BUILD_DATE >= 20000412
	if( option_w.unk_18 == 1 ) Sys->disp_line = 7 ;
#else
	if( option_w.unk_18 ) Sys->disp_line = 7 ;
#endif
	if( Sys->home_mode == HOME_ARCADE ){
		d_point_add( Sys->new_side != 3 ? 100 : 200 ) ;
	}
	if( Sys->home_mode == HOME_TRAINING ){
		training_init() ;
		Sys->no_ko_flag = 1 ;
	}
	if( Sys->home_mode == HOME_NETWORK ) net_rule_set() ;
#endif
	Palette_Init() ;
	Texture_Init() ;
	fade_init() ;
	back_color_set( 0 ) ;
	stream_stop() ;
	sound_all_off() ;
#ifndef TARGET_DC
	se_all_off() ;
#endif
	total_vol_set( 127 ) ;
	nlInitMatrix( matrix_stack, 64 ) ;
	nlMatrixMode( 3 ) ;
	nlUnitMatrix() ;
	nlPerspective( 0x1d28, 1.3333334f, 0.3f, 12000.0f ) ;
	nlMatrixMode( 1 ) ;
	nlSetBackColor( 0x808080, 0x80, 0x808080 ) ;
	nlInitDrawModel( ScreenVertexBuffer, LightFragmentBuffer ) ;
	nlObjPutInit() ;
	nlLightInit() ;
	work_init() ;
	for( i = 0 ; i < 6 ; i ++ ){
		pl_work[i].be_flag = 1 ;
		pl_work[i].disp_flag = 0 ;
		pl_work[i].pl_index = i ;
		pl_work[i].pl_type = i ;
		pl_work[i].load_type = i ;
		pl_work[i].team_order = i / 2 ;
		pl_work[i].handicap = 2 ;
	}
}

/*==============================================================*/
/*			The game's scenes									*/
/*==============================================================*/
static void game_move( void )
{
	static void ( *game_move_jmp[] )( void ) = {
		game_start,
		game_select,
		game_play,
		game_cont,
		game_staff,
		game_over,
		game_end,
	} ;

	game_move_jmp[Sys->scene_no]() ;
	light_dummy() ;
}

/*==============================================================*/
static void game_start( void )
{
	Sys->scene_no = 1 ;
	Sys->routine_0 = 0 ;
	Sys->routine_1 = 0 ;
	Sys->routine_2 = 0 ;
	Sys->routine_3 = 0 ;
	Sys->exit_flag = 0 ;
	Sys->sel_done_flag = 1 ;
	Sys->unk_50 = 0 ;
	Sys->new_side_keep = Sys->new_side ;
	Sys->stage_cnt = 0 ;
	if( ( Sys->new_side | Sys->play_side ) != 3 ) Sys->stage_cnt = pl_work[Sys->new_side - 1].stage_cnt ;
	Palette_Init() ;
	Texture_Init() ;
	back_color_set( 0 ) ;
	vs_sys_init() ;
	cpu_level_init() ;
#ifdef TARGET_DC
	/* who may pause, and the mode's start: points for a versus or score attack game, the
	   network game's flags */
	side_sw_adrs[0] = &pl_work[0].side_sw ;
	side_sw_adrs[1] = &pl_work[1].side_sw ;
	Sys->pause_off = 0 ;
	pause_init() ;
	switch( Sys->home_mode ){
	case 0 :
	case 2 :
		break ;
	case 1 :
	case 7 :
		d_point_add( 100 ) ;
		break ;
	case 5 :
		Sys->pause_off = 1 ;
		unk_8C2132CA = unk_8C2132CB = 1 ;
		unk_8C2134CC = unk_8C2134CD = 1 ;
		unk_8C2134CE = 0 ;
		unk_8C213048 = unk_8C213049 = 0 ;
		net_play_flag = 1 ;
		unk_8C2136D1 = unk_8C2136D0 = 0 ;
		break ;
	}
#endif
}

/*==============================================================*/
static void game_select( void )
{
	SelectMainMove_yu() ;
}

/*==============================================================*/
static void game_play( void )
{
	game_main() ;
}

/*==============================================================*/
static void game_cont( void )
{
	cont_main() ;
}

/*==============================================================*/
static void game_over( void )
{
	over_main() ;
}

/*==============================================================*/
static void game_staff( void )
{
	staff_main() ;
}

/*==============================================================*/
/*			The game is over: back to the attract				*/
/*==============================================================*/
static void game_end( void )
{
	Sys->play_mode = 64 ;
	Sys->task_no = 1 ;
	Sys->task_step = 0 ;
	Sys->scene_no = 0 ;
	Sys->routine_0 = 0 ;
	Sys->routine_1 = 0 ;
	Sys->routine_2 = 0 ;
	Sys->routine_3 = 0 ;
	Sys->exit_flag = 0 ;
	Sys->timer0 = 0 ;
	Sys->timer1 = 0 ;
	Sys->timer2 = 0 ;
	Sys->timer3 = 0 ;
#ifdef TARGET_DC
	if( Sys->home_mode == HOME_SCORE ) Sys->demo_no = 2 ;	/* the score attack's ranking */
#endif
	work_init() ;
	bright_init() ;
	fog_init() ;
	fade_init() ;
	back_color_set( 0 ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "game.c" End of File	*/

/* The home game's mode menu, the system's task 3: the main menu's ring of modes, OPTION, the
   SECRET FACTOR and its shop, the memory card menu and the records. */
#include "player.h"
#include "shinobi.h"
#include "dm.h"

extern void entry_init( PLAYER_WORK *pl_w ) ;
extern void disp_menu( void ) ;
extern void dm0c_wall_paper( int no ) ;
extern void dm0c_vm_win_set( int st ) ;
extern void dm0c_vm_msg_set( void ) ;
extern void home_menu_set( void ) ;
extern void headline_sub_set( int no ) ;
extern void option_game_set( void ) ;
extern void option_key_set( void ) ;
extern void option_adjust_set( void ) ;
extern void disp_adjust_clear( void ) ;
extern void sys_set_init( int kind ) ;
extern void stereo_set( void ) ;
extern void mono_set( void ) ;
extern void vib_req( int pl_no, int no ) ;
extern void disp_adjust_set( void ) ;
extern void d_point_get( void ) ;
extern void n_point_get( void ) ;
extern int decide_ck( void ) ;
extern int cancel_ck( void ) ;
extern int decide_cancel_ck( void ) ;

void home_menu_init( void ) ;
int home_menu_2p_ck( void ) ;
void home_menu_select( void ) ;
void home_menu_mode( void ) ;
int home_menu_sw_ck( Uint16 side ) ;
void home_menu_decide( void ) ;
void home_game_start( void ) ;
void option_main( void ) ;
void option_return( void ) ;
void option_exit( OPTION_WORK *ow ) ;
void option_game( OPTION_WORK *ow ) ;
void option_key( OPTION_WORK *ow ) ;
void option_adjust( OPTION_WORK *ow ) ;
void option_end( OPTION_WORK *ow ) ;
void secret_factor( void ) ;
void vm_menu( void ) ;
void mode8_main( void ) ;
void option_key_move( OPTION_WORK *ow, Uint16 side ) ;
int lr_select( Uint16 sw, int no, int max, int mode ) ;
int ud_select_loop( Uint16 sw, int no, int max ) ;

static Sint32 stereo_keep ;	/* the sound output as OPTION found it */
static Uint16 menu_trg ;	/* both sides' newly pressed buttons */
static Uint16 menu_now ;	/* and those held */
static Float adjust_x_keep ;	/* the screen position as SCREEN ADJUST found it */
static Float adjust_y_keep ;
static Sint32 screen_keep ;

static VEC3 light_dir = { 0.0f, 1.0f, 1.0f } ;	/* the menu's light */

/****************************************************************/
/*			The mode menu, each frame							*/
/****************************************************************/
void home_menu( void )
{
	static void ( *home_menu_jmp[] )( void ) = {
		home_menu_init, home_menu_select, home_menu_mode
	} ;

	if( soft_reset_ck( 1 ) ) return ;
	menu_trg = sw_work[0].on | sw_work[1].on ;
	menu_now = sw_work[0].now | sw_work[1].now ;
	home_menu_jmp[Sys->task_step]() ;
	work_list_move( 11 ) ;
	work_list_move( 12 ) ;
	SysTransSet( disp_menu ) ;
}

/****************************************************************/
/*			Its set-up, and its music							*/
/****************************************************************/
void home_menu_init( void )
{
	if( Sys->scene_no == 0 ){
		Sys->scene_no ++ ;
		Palette_Init() ;
		Texture_Init() ;
		TexLoad( 16 ) ;
		work_init() ;
		scr_work_init() ;
		fade_init() ;
		back_color_set( 0 ) ;
		light_dir_set( &light_dir ) ;
		light_amb_set( 1.0f ) ;
		Sys->timer0 = 0 ;
		home_menu_set() ;
		dm0c_wall_paper( 0 ) ;
		fade_set( 0xff000000, 25, FADE_IN ) ;
	} else if( Sys->ended_mode >= 0 ){
		if( Sys->ended_mode == HOME_ARCADE || Sys->ended_mode == HOME_VERSUS || Sys->ended_mode == HOME_TRAINING
		 || Sys->ended_mode == HOME_SCORE || Sys->ended_mode == HOME_NETWORK ){
			stream_req( 28 ) ;
			s3l_fade_in_set( 1 ) ;
			stream_start() ;
		}
		Sys->task_step ++ ;
		Sys->scene_no = 0 ;
	} else {
		Sys->task_step ++ ;
		Sys->scene_no = 0 ;
		stream_req( 28 ) ;
		s3l_fade_in_set( 1 ) ;
		stream_start() ;
	}
}

/****************************************************************/
/*			1 if a side has no pad: VERSUS can't be chosen		*/
/****************************************************************/
int home_menu_2p_ck( void )
{
	if( !( port_peri[0]->info->type & PDD_DEVTYPE_CONTROLLER ) || !( port_peri[1]->info->type & PDD_DEVTYPE_CONTROLLER ) ) return 1 ;
	return 0 ;
}

/****************************************************************/
/*			The cursor on the ring of modes						*/
/****************************************************************/
void home_menu_select( void )
{
	static Sint8 ud_tbl[10] = { 4, 2, 1, 0, 0, 8, 7, 6, 6, 6 } ;

	if( !Sys->scene_no ){
		if( fade_cnt() ) return ;
		Sys->scene_no ++ ;
		Sys->timer1 = 8 ;
		Sys->timer0 = Sys->home_mode ;
	} else if( Sys->scene_no == 1 ){
		if( !Sys->timer1 ){
			if( menu_trg & KEY_LEFT ){
				if( Sys->timer0 >= 5 ){
					Sys->timer0 ++ ;
					if( Sys->timer0 > 8 ) Sys->timer0 = 5 ;
					if( Sys->timer0 == 5 ) Sys->timer0 = 6 ;
				} else {
					Sys->timer0 -- ;
					if( Sys->timer0 == 1 && home_menu_2p_ck() ) Sys->timer0 = 0 ;
					if( Sys->timer0 < 0 ) Sys->timer0 = 4 ;
				}
				na_req( 1 ) ;
				Sys->timer1 = 4 ;
			}
			if( menu_trg & KEY_RIGHT ){
				if( Sys->timer0 >= 5 ){
					Sys->timer0 -- ;
					if( Sys->timer0 < 5 ) Sys->timer0 = 8 ;
					if( Sys->timer0 == 5 ) Sys->timer0 = 8 ;
				} else {
					Sys->timer0 ++ ;
					if( Sys->timer0 == 1 && home_menu_2p_ck() ) Sys->timer0 = 2 ;
					if( Sys->timer0 >= 5 ) Sys->timer0 = 0 ;
				}
				na_req( 1 ) ;
				Sys->timer1 = 4 ;
			}
			if( ( menu_trg & KEY_UP ) || ( menu_trg & KEY_DOWN ) ){
				if( Sys->timer0 >= 5 ){
					Sys->timer0 = ud_tbl[Sys->timer0 - 5] ;
					if( Sys->timer0 == 1 && home_menu_2p_ck() ) Sys->timer0 = 0 ;
				} else {
					Sys->timer0 = ud_tbl[Sys->timer0 + 5] ;
				}
				na_req( 1 ) ;
				Sys->timer1 = 4 ;
			}
			if( Sys->timer0 == 1 && home_menu_2p_ck() ) Sys->timer0 = 0 ;
			Sys->home_mode = Sys->timer0 ;
			home_menu_decide() ;
		} else {
			if( -- Sys->timer1 < 0 ) Sys->timer1 = 0 ;
		}
	} else {
		if( fade_cnt() ) return ;
		Sys->play_mode = 64 ;
		Sys->task_no = 1 ;
		Sys->task_step = 7 ;
		Sys->scene_no = Sys->routine_0 = Sys->routine_1 = Sys->routine_2 = Sys->routine_3 = Sys->exit_flag
			= Sys->timer0 = Sys->timer1 = Sys->timer2 = Sys->timer3 = 0 ;
		Sys->home_mode = HOME_ARCADE ;
		Sys->ended_mode = 0 ;
	}
}

/****************************************************************/
/*			The mode chosen, each frame							*/
/****************************************************************/
void home_menu_mode( void )
{
	static void ( *home_mode_jmp[] )( void ) = {
		home_game_start, home_game_start, home_game_start, option_main, vm_menu,
		home_game_start, secret_factor, home_game_start, mode8_main
	} ;

	if( Sys->home_mode != HOME_NETWORK && fade_cnt() ) return ;
	home_mode_jmp[Sys->home_mode]() ;
}

/****************************************************************/
/*			A side's decide button								*/
/****************************************************************/
int home_menu_sw_ck( Uint16 side )
{
	Uint16 sw ;

	if( port_peri[side]->support & ( PDD_DGT_TC | PDD_DGT_TZ ) ) sw = KEY_LK | KEY_A2 ;
	else sw = KEY_LK ;
	if( sw_work[side].on & sw ) return 1 ;
	return 0 ;
}

/****************************************************************/
/*			A mode is decided: the sides that play, and the		*/
/*			mode's rules										*/
/****************************************************************/
void home_menu_decide( void )
{
	PLAYER_WORK *pl_w ;
	Sint8 side = 0 ;
	Sint8 i ;

	if( ( sw_work[0].on & KEY_START ) || home_menu_sw_ck( 0 ) ) side = 1 ;
	if( ( sw_work[1].on & KEY_START ) || home_menu_sw_ck( 1 ) ) side += 2 ;
	if( side != 0 ){
		Sys->task_step ++ ;
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
		Sys->entry_side = 0 ;
		sys_set_load() ;
		training_init() ;
		switch( Sys->home_mode ){
		case HOME_ARCADE :
			sys_set_load() ;
			break ;
		case HOME_VERSUS :
			Sys->event_flag = 1 ;
			Sys->damage_level = 1 ;
			break ;
		case HOME_TRAINING :
			Sys->no_challenge_flag = -1 ;
			Sys->round_time_no = 3 ;
			Sys->speed_fix = 1 ;
			training_work.side = 0 ;
			mode_cnt_up() ;
			break ;
		case HOME_OPTION :
			break ;
		case HOME_CARD :
			break ;
		case HOME_NETWORK :
			Sys->home_mode = HOME_ARCADE ;
			sys_set_load() ;
			break ;
		case HOME_SECRET :
			break ;
		case HOME_SCORE :
			Sys->cont_enable = 0 ;
			Sys->no_challenge_flag = -1 ;
			Sys->renda_level = 1 ;
			Sys->round_time_no = 2 ;
			Sys->damage_level = 1 ;
			Sys->speed_fix = 1 ;
			mode_cnt_up() ;
			break ;
		case HOME_RECORDS :
			break ;
		}
		Sys->ended_mode = Sys->home_mode ;
		if( Sys->no_challenge_flag ){
			if( ( sw_work[0].on & KEY_START ) || home_menu_sw_ck( 0 ) ) side = 1 ;
			if( ( sw_work[1].on & KEY_START ) || home_menu_sw_ck( 1 ) ) side = 2 ;
		}
		if( Sys->event_flag ) side = 3 ;
		Sys->entry_side = side ;
		Sys->new_side = side ;
		Sys->play_mode = 96 ;
		for( i = 0 ; i < 6 ; i ++ ){
			pl_w = &pl_work[i] ;
			memset( pl_w, 0, sizeof( PLAYER_WORK ) ) ;
			pl_w->pl_index = i ;
			entry_init( pl_w ) ;
		}
		if( Sys->entry_side & 1 ) pl_work[0].entry_routine_1 = ENTRY_PLAY ;
		if( Sys->entry_side & 2 ) pl_work[1].entry_routine_1 = ENTRY_PLAY ;
		pl_work[0].side_bit = 1 ;
		pl_work[1].side_bit = 2 ;
		na_req( 2 ) ;
		fade_set( 0xff000000, 10, FADE_OUT ) ;
	} else if( ( sw_work[0].on & KEY_HK ) || ( sw_work[1].on & KEY_HK ) ){
		Sys->scene_no = 2 ;
		na_req( 3 ) ;
		fade_set( 0xff000000, 10, FADE_OUT ) ;
	}
}

/****************************************************************/
/*			A game's mode: into the game						*/
/****************************************************************/
void home_game_start( void )
{
	PLAYER_WORK *pl_w ;
	Sint8 i ;

	if( fade_cnt() ) return ;
	Sys->task_no = 2 ;
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
	Sys->play_mode = 96 ;
	for( i = 0 ; i < 6 ; i ++ ){
		pl_w = &pl_work[i] ;
		memset( pl_w, 0, sizeof( PLAYER_WORK ) ) ;
		pl_w->pl_index = i ;
		entry_init( pl_w ) ;
	}
	if( Sys->entry_side & 1 ) pl_work[0].entry_routine_1 = ENTRY_PLAY ;
	if( Sys->entry_side & 2 ) pl_work[1].entry_routine_1 = ENTRY_PLAY ;
	pl_work[0].side_bit = 1 ;
	pl_work[1].side_bit = 2 ;
	stream_stop() ;
}

/****************************************************************/
/*			OPTION: its four items								*/
/****************************************************************/
void option_main( void )
{
	static void ( *option_jmp[] )( OPTION_WORK *ow ) = {
		option_game, option_key, option_adjust, option_exit, option_end
	} ;

	switch( Sys->routine_0 ){
	case 0 :
		if( fade_cnt() ) return ;
		Sys->routine_0 ++ ;
		Sys->timer0 = Sys->routine_1 ;
		Sys->routine_1 = 0 ;
		Sys->routine_2 = 0 ;
		Sys->routine_3 = 0 ;
		Sys->exit_flag = 0 ;
		stereo_keep = conf_stereo ;
	case 1 :
		work_list_kill( 11 ) ;
		work_list_kill( 12 ) ;
		dm0c_wall_paper( 1 ) ;
		headline_set( 1 ) ;
		headline_sub_set( 1 ) ;
		option_menu_set() ;
		Sys->routine_0 ++ ;
		fade_set( 0xff000000, 10, FADE_IN ) ;
		disp_adjust_set() ;
		break ;
	case 2 :
		if( !fade_cnt() ) Sys->routine_0 ++ ;
		break ;
	case 3 :
		if( decide_ck() ){
			Sys->routine_0 ++ ;
			Sys->routine_1 = Sys->timer0 ;
			goto fade ;
		} else if( cancel_ck() ){
			Sys->routine_0 ++ ;
			Sys->routine_1 = 3 ;
		fade :
			Sys->routine_2 = 0 ;
			Sys->routine_3 = 0 ;
			Sys->exit_flag = 0 ;
			fade_set( 0xff000000, 10, FADE_OUT ) ;
		} else {
			Sys->timer0 = ud_select_loop( menu_sw_get( 3 ), Sys->timer0, 3 ) ;
		}
		break ;
	case 4 :
		option_jmp[Sys->routine_1]( &option_w ) ;
		break ;
	}
}

/****************************************************************/
/*			Back to the mode menu								*/
/****************************************************************/
void option_return( void )
{
	Sys->scene_no = 0 ;
	Sys->routine_0 = 0 ;
	fade_set( 0xff000000, 10, FADE_OUT ) ;
}

/****************************************************************/
/*			OPTION's EXIT: the points, and the auto save		*/
/****************************************************************/
void option_exit( OPTION_WORK *ow )
{
	int ret ;

	menu_sw_get( 3 ) ;
	switch( Sys->routine_3 ){
	case 0 :
		if( vms_work.file_flag != 1 ){
			Sys->routine_1 = 4 ;
			Sys->routine_2 = Sys->routine_3 = Sys->exit_flag = 0 ;
			if( vms_work.d_point ) d_point_get() ;
			if( vms_work.n_point ) n_point_get() ;
			break ;
		}
		Sys->routine_3 ++ ;
		Sys->exit_flag = 0 ;
		work_init() ;
		dm0c_wall_paper( 9 ) ;
		Sys->timer0 = 120 ;
		vms_work.card_task_step[0][0] = vms_work.card_task_step[0][1] = 0 ;
		vms_work.card_task_step[1][0] = vms_work.card_task_step[1][1] = 0 ;
		if( vms_work.d_point && vms_work.n_point ){
			vms_work.vm_msg[0] = 3 ;
			dm0c_vm_msg_set() ;
			vms_work.vm_msg[1] = 1 ;
			dm0c_vm_win_set( vms_work.vm_msg[1] ) ;
			fade_set( 0xff000000, 10, FADE_IN ) ;
		} else if( vms_work.d_point ){
			vms_work.vm_msg[0] = 1 ;
			goto msg ;
		} else if( vms_work.n_point ){
			vms_work.vm_msg[0] = 2 ;
		msg :
			dm0c_vm_msg_set() ;
			vms_work.vm_msg[1] = 1 ;
			dm0c_vm_win_set( vms_work.vm_msg[1] ) ;
			fade_set( 0xff000000, 10, FADE_IN ) ;
			break ;
		} else {
			Sys->timer0 = 1 ;
			fade_set( 0xff000000, 10, FADE_IN ) ;
		}
		break ;
	case 1 :
		if( !fade_cnt() ) Sys->routine_3 ++ ;
		break ;
	case 2 :
		if( cancel_ck() ) Sys->timer0 = 1 ;
		if( Sys->timer0 -- == 0 ){
			Sys->routine_3 ++ ;
			vms_work.vm_msg[0] = 4 ;
			dm0c_vm_msg_set() ;
			vms_work.vm_msg[1] = 2 ;
			dm0c_vm_win_set( vms_work.vm_msg[1] ) ;
			if( vms_work.d_point ) d_point_get() ;
			if( vms_work.n_point ) n_point_get() ;
		}
		return ;
	case 3 :
		ret = vms_task_save_all( vms_work.file_port, 0 ) ;
		if( ret == 2 ) return ;
		if( ret == 1 ){
			Sys->routine_3 = 4 ;
			Sys->timer0 = 60 ;
			vms_work.vm_msg[0] = 5 ;
			dm0c_vm_msg_set() ;
		} else if( ret == 0 ){
			Sys->routine_3 = 5 ;
			vms_work.vm_msg[0] = 6 ;
			dm0c_vm_msg_set() ;
		} else if( ret == 3 ){
			Sys->routine_3 = 5 ;
			vms_work.vm_msg[0] = 7 ;
			dm0c_vm_msg_set() ;
		}
		return ;
	case 4 :
		if( decide_cancel_ck() ) Sys->timer0 = 1 ;
		if( Sys->timer0 -- == 0 ){
			Sys->routine_3 = 6 ;
			fade_set( 0xff000000, 10, FADE_OUT ) ;
		}
		break ;
	case 5 :
		if( !decide_cancel_ck() ) break ;
		vms_work.file_flag = 0 ;
		Sys->routine_3 = 6 ;
		fade_set( 0xff000000, 10, FADE_OUT ) ;
		break ;
	case 6 :
		if( !fade_cnt() ){
			Sys->routine_1 = 4 ;
			Sys->routine_2 = Sys->routine_3 = Sys->exit_flag = 0 ;
		}
		break ;
	}
}

/****************************************************************/
/*			GAME OPTION's lines									*/
/****************************************************************/
void opt_difficulty( OPTION_WORK *ow )
{
	conf_difficulty = lr_select( menu_sw_get( 3 ), conf_difficulty, 7, 2 ) ;
	ow->difficulty = conf_difficulty ;
}

void opt_time( OPTION_WORK *ow )
{
	conf_round_time = lr_select( menu_sw_get( 3 ), conf_round_time, 3, 2 ) ;
	ow->round_time = conf_round_time ;
}

void opt_damage( OPTION_WORK *ow )
{
	conf_damage_level = lr_select( menu_sw_get( 3 ), conf_damage_level, 3, 2 ) ;
	ow->damage_level = conf_damage_level ;
}

void opt_speed( OPTION_WORK *ow )
{
	conf_speed_fix = lr_select( menu_sw_get( 3 ), conf_speed_fix, 2, 2 ) ;
	ow->speed_fix = conf_speed_fix ;
}

void opt_sound( OPTION_WORK *ow )
{
	conf_stereo = lr_select( menu_sw_get( 3 ), conf_stereo, 1, 2 ) ;
	if( ow->stereo != conf_stereo ){
		if( conf_stereo ) stereo_set() ;
		else mono_set() ;
		ow->stereo = conf_stereo ;
	}
}

void opt_default( OPTION_WORK *ow )
{
	if( decide_ck() ) sys_set_init( 1 ) ;
}

void opt_exit( OPTION_WORK *ow )
{
	if( decide_ck() ) option_return() ;
}

/****************************************************************/
/*			GAME OPTION											*/
/****************************************************************/
void option_game( OPTION_WORK *ow )
{
	static void ( *option_game_jmp[] )( OPTION_WORK *ow ) = {
		opt_difficulty, opt_time, opt_damage, opt_speed, opt_sound, opt_default, opt_exit
	} ;
	Uint16 sw ;

	if( !Sys->routine_2 ){
		work_list_kill( 11 ) ;
		work_list_kill( 12 ) ;
		Sys->routine_2 ++ ;
		Sys->timer0 = 0 ;
		dm0c_wall_paper( 1 ) ;
		headline_set( 0 ) ;
		headline_sub_set( 0 ) ;
		option_game_set() ;
		fade_set( 0xff000000, 10, FADE_IN ) ;
	} else if( Sys->routine_2 == 1 ){
		if( !fade_cnt() ) Sys->routine_2 ++ ;
	} else if( cancel_ck() ){
		option_return() ;
	} else {
		sw = menu_sw_get( 3 ) & KEY_LEVER ;
		if( sw == KEY_UP || sw == KEY_DOWN ){
			Sys->timer0 = ud_select_loop( menu_sw_get( 3 ), Sys->timer0, 6 ) ;
		} else {
			option_game_jmp[Sys->timer0]( ow ) ;
		}
	}
}

/****************************************************************/
/*			BUTTON CONFIG, both sides at once					*/
/****************************************************************/
void option_key( OPTION_WORK *ow )
{
	if( !Sys->routine_2 ){
		work_list_kill( 11 ) ;
		work_list_kill( 12 ) ;
		Sys->routine_2 ++ ;
		Sys->option_cursor[0] = 0 ;
		Sys->option_cursor[1] = 0 ;
		Sys->timer3 = 0 ;
		Sys->timer0 = 0 ;
		dm0c_wall_paper( 1 ) ;
		headline_set( 2 ) ;
		headline_sub_set( 2 ) ;
		option_key_set() ;
		fade_set( 0xff000000, 10, FADE_IN ) ;
	} else if( Sys->routine_2 == 1 ){
		if( !fade_cnt() ) Sys->routine_2 ++ ;
	} else {
		if( ++ Sys->timer3 < 8 ) return ;
		else Sys->timer3 = 8 ;
		if( Sys->routine_2 == 2 ){
			option_key_move( ow, 0 ) ;
			option_key_move( ow, 1 ) ;
		} else {
			option_return() ;
		}
	}
}

/****************************************************************/
/*			One side's button config							*/
/****************************************************************/
void option_key_move( OPTION_WORK *ow, Uint16 side )
{
	static Uint8 key_line_tbl[6] = { 0, 1, 2, 3, 4, 5 } ;
	Uint16 sw ;
	Uint16 key, new ;
	Uint16 i ;

	sw = menu_sw_get( 1 << side ) ;
	if( sw == KEY_UP || sw == KEY_DOWN ){
		Sys->option_cursor[side] = ud_select_loop( sw, Sys->option_cursor[side], 9 ) ;
		if( Sys->option_cursor[side] == 6 ) Sys->unk_72 = 0 ;
	} else if( sw == KEY_LEFT || sw == KEY_RIGHT ){
		if( Sys->option_cursor[side] < 6 ){
			i = key_line_tbl[Sys->option_cursor[side]] ;
			key = ow->key_config[side][i] ;
			new = lr_select( sw, ow->key_config[side][i], 11, 2 ) ;
			if( key != new ) ow->key_config[side][i] = new ;
		} else if( Sys->option_cursor[side] == 6 ){
			ow->vib_bit ^= ( 1 << side ) ;
			if( ow->vib_bit & ( 1 << side ) ) vib_req( side, 8 ) ;
			na_req( 4 ) ;
		} else if( Sys->option_cursor[side] == 7 ){
			ow->com_time[side] ^= 1 ;
			na_req( 4 ) ;
		}
	} else if( home_menu_sw_ck( side ) ){
		if( Sys->option_cursor[side] == 8 ){
			for( i = 0 ; i < 6 ; i ++ ) ow->key_config[side][i] = i ;
			ow->com_time[side] = 0 ;
			ow->vib_bit &= ~( 1 << side ) ;
			na_req( 2 ) ;
		} else if( Sys->option_cursor[side] == 9 ){
			Sys->routine_2 ++ ;
			na_req( 2 ) ;
		}
	} else if( sw_work[side].on & KEY_HK ){
		Sys->routine_2 ++ ;
		na_req( 3 ) ;
	}
}

/****************************************************************/
/*			The screen moved, a dot at a time					*/
/****************************************************************/
void option_adjust_move( OPTION_WORK *ow )
{
	Uint16 sw ;

	sw = menu_sw_get( 3 ) ;
	if( sw & KEY_LEFT ){
		ow->adjust_x -- ;
		if( ow->adjust_x < -10.0f ) ow->adjust_x = -10.0f ;
		else na_req( 4 ) ;
	} else if( sw & KEY_RIGHT ){
		ow->adjust_x ++ ;
		if( ow->adjust_x > 10.0f ) ow->adjust_x = 10.0f ;
		else na_req( 4 ) ;
	}
	if( sw & KEY_UP ){
		ow->adjust_y -- ;
		if( ow->adjust_y < -10.0f ) ow->adjust_y = -10.0f ;
		else na_req( 4 ) ;
	} else if( sw & KEY_DOWN ){
		ow->adjust_y ++ ;
		if( ow->adjust_y > 10.0f ) ow->adjust_y = 10.0f ;
		else na_req( 4 ) ;
	}
}

/****************************************************************/
/*			SCREEN ADJUST										*/
/****************************************************************/
void option_adjust( OPTION_WORK *ow )
{
	Uint16 sw ;

	sw = menu_sw_get( 3 ) ;
	switch( Sys->routine_2 ){
	case 0 :
		work_list_kill( 11 ) ;
		work_list_kill( 12 ) ;
		Sys->routine_2 ++ ;
		Sys->timer3 = 0 ;
		Sys->option_cursor[0] = 0 ;
		Sys->timer0 = 0 ;
		adjust_x_keep = ow->adjust_x ;
		adjust_y_keep = ow->adjust_y ;
		screen_keep = ow->unk_18 ;
		option_adjust_set() ;
		disp_adjust_clear() ;
		fade_set( 0xff000000, 10, FADE_IN ) ;
		break ;
	case 1 :
		if( fade_cnt() ) return ;
		if( ++ Sys->timer3 >= 8 ){
			Sys->routine_2 ++ ;
			Sys->timer3 = 8 ;
		}
		break ;
	case 2 :
		Sys->timer0 = ud_select_loop( sw, Sys->timer0, 2 ) ;
		if( decide_ck() ) Sys->routine_2 = 3 ;
		else if( cancel_ck() ) Sys->routine_2 = 4 ;
		break ;
	case 3 :
		if( Sys->timer0 == 2 ) Sys->routine_2 = 4 ;
		else if( Sys->timer0 == 1 ){
			Sys->routine_2 = 2 ;
			sys_set_init( 4 ) ;
		} else if( cancel_ck() ) Sys->routine_2 = 2 ;
		else if( Sys->timer0 == 0 ) option_adjust_move( ow ) ;
		break ;
	case 4 :
		option_return() ;
		break ;
	}
}

/****************************************************************/
/*			Leaving OPTION: the sound output to the console		*/
/****************************************************************/
void option_end( OPTION_WORK *ow )
{
	Sint32 mode ;

	if( fade_cnt() ) return ;
	fade_set( 0xff000000, 10, FADE_IN ) ;
	if( conf_stereo ) mode = SYD_CFG_STEREO ;
	else mode = SYD_CFG_MONO ;
	syCfgSetSoundMode( mode ) ;
	work_list_kill( 11 ) ;
	work_list_kill( 12 ) ;
	Sys->task_step = 1 ;
	Sys->scene_no = 0 ;
	Sys->routine_0 = 0 ;
	Sys->routine_1 = 0 ;
	Sys->routine_2 = 0 ;
	Sys->routine_3 = 0 ;
	Sys->exit_flag = 0 ;
	home_menu_set() ;
	dm0c_wall_paper( 0 ) ;
}

/****************************************************************/
/*			A value moved by left and right: mode 1 stops it at	*/
/*			the ends, where it would go round; mode 2 sounds it	*/
/****************************************************************/
int lr_select( Uint16 sw, int no, int max, int mode )
{
	int stop, se, lever ;

	lever = sw & KEY_LEVER ;
	stop = mode & 1 ;
	se = mode & 2 ;
	if( lever == KEY_LEFT ){
		no -- ;
		if( stop ){
			if( no < 0 ) no = 0 ;
			else na_req( se ? 4 : 1 ) ;
		} else {
			if( no < 0 ) no = max ;
			na_req( se ? 4 : 1 ) ;
		}
	}
	if( lever == KEY_RIGHT ){
		no ++ ;
		if( stop ){
			if( no > max ) no = max ;
			else na_req( se ? 4 : 1 ) ;
		} else {
			if( no > max ) no = 0 ;
			na_req( se ? 4 : 1 ) ;
		}
	}
	if( 0 ){ na_req( 1 ) ; na_req( 4 ) ; }	/* left out of the build */
	return no ;
}

int lr_select_loop( Uint16 sw, int no, int max )
{
	return lr_select( sw, no, max, 0 ) ;
}

int lr_select_stop( Uint16 sw, int no, int max )
{
	return lr_select( sw, no, max, 1 ) ;
}

/****************************************************************/
/*			And by up and down									*/
/****************************************************************/
int ud_select( Uint16 sw, int no, int max, int mode )
{
	int stop, se, lever ;

	lever = sw & KEY_LEVER ;
	stop = mode & 1 ;
	se = mode & 2 ;
	if( lever == KEY_UP ){
		no -- ;
		if( stop ){
			if( no < 0 ) no = 0 ;
			else na_req( se ? 4 : 1 ) ;
		} else {
			if( no < 0 ) no = max ;
			na_req( se ? 4 : 1 ) ;
		}
	}
	if( lever == KEY_DOWN ){
		no ++ ;
		if( stop ){
			if( no > max ) no = max ;
			else na_req( se ? 4 : 1 ) ;
		} else {
			if( no > max ) no = 0 ;
			na_req( se ? 4 : 1 ) ;
		}
	}
	if( 0 ){ na_req( 1 ) ; na_req( 4 ) ; }	/* left out of the build */
	return no ;
}

int ud_select_loop( Uint16 sw, int no, int max )
{
	return ud_select( sw, no, max, 0 ) ;
}

int ud_select_stop( Uint16 sw, int no, int max )
{
	return ud_select( sw, no, max, 1 ) ;
}

/****************************************************************/
/*			The keys of the sides in side_bit: newly pressed,	*/
/*			and the lever's repeats								*/
/****************************************************************/
Uint16 menu_sw_get( Uint8 side_bit )
{
	Uint16 sw = 0 ;

	if( side_bit & 1 ) sw |= sw_work[0].on | ( sw_work[0].rep & KEY_LEVER ) ;
	if( side_bit & 2 ) sw |= sw_work[1].on | ( sw_work[1].rep & KEY_LEVER ) ;
	return sw ;
}

/****************************************************************/
/*			The same with the keyboard's						*/
/****************************************************************/
Uint16 menu_sw_kbd_get( Uint8 side_bit )
{
	Uint16 sw = 0 ;

	if( side_bit & 1 ) sw |= sw_work[0].on | ( sw_work[0].rep & KEY_LEVER ) ;
	if( side_bit & 2 ) sw |= sw_work[1].on | ( sw_work[1].rep & KEY_LEVER ) ;
	sw |= kbd_on | ( kbd_rep & KEY_LEVER ) ;
	return sw ;
}

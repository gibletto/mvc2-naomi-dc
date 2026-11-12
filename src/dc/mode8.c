/* The home game's mode 8: the ranking's sheets. */
#include "player.h"
#include "shinobi.h"
#include "dm.h"
#include "secret.h"

static Sint8 mode8_line ;	/* the line mode8_view scrolls to */

/****************************************************************/
/*			Mode 8: the three sheets to choose from				*/
/****************************************************************/
void mode8_main( void )
{
	Uint16 sw ;

	sw = menu_sw_get( 3 ) ;
	switch( Sys->routine_0 ){
	case 0 :
		Sys->routine_1 = 0 ;
		Palette_Init() ;
		Texture_Init() ;
		TexLoad( 18 ) ;
	case 1 :
		Sys->routine_0 = 2 ;
		Sys->routine_2 = 0 ;
		work_init() ;
		dm_obj_set( 8, obj_dm0f ) ;
		dm0c_wall_paper( 7 ) ;
		headline_set( 13 ) ;
		dm0f_mode8_set() ;
		headline_sub_set( 13 ) ;
		fade_set( 0xff000000, 10, FADE_IN ) ;
		break ;
	case 2 :
		if( !fade_cnt() ){
			Sys->routine_1 = ud_select_loop( sw, Sys->routine_1, 2 ) ;
			if( decide_ck() ){
				na_req( 2 ) ;
				fade_set( 0xff000000, 10, FADE_OUT ) ;
				if( Sys->routine_1 == 2 ){
					Sys->routine_0 = 4 ;
					return ;
				}
				Sys->routine_0 = 3 ;
				return ;
			}
			if( cancel_ck() ){
				na_req( 3 ) ;
				Sys->routine_0 = 4 ;
				fade_set( 0xff000000, 10, FADE_OUT ) ;
				return ;
			}
		}
		break ;
	case 3 :
		if( !fade_cnt() ) mode8_view( sw ) ;
		break ;
	case 4 :
		if( !fade_cnt() ) Sys->task_step = 0 ;
		break ;
	}
}

/****************************************************************/
/*			Mode 8: a sheet, scrolled up and down				*/
/****************************************************************/
void mode8_view( Uint16 sw )
{
	switch( Sys->routine_2 ){
	case 0 :
		work_init() ;
		Sys->routine_2 = 1 ;
		Sys->routine_3 = 0 ;
		Sys->exit_flag = 0 ;
		Sys->timer3 = 0 ;
		mode8_line = 0 ;
		dm_obj_set( 8, obj_dm0f ) ;
		dm0c_wall_paper( 7 ) ;
		headline_set( 14 ) ;
		dm0f_mode8_sheet_set() ;
		dm0f_mode8_frame_set() ;
		dm0f_mode8_title_set() ;
		fade_set( 0xff000000, 10, FADE_IN ) ;
		break ;
	case 1 :
		if( fade_cnt() ) break ;
		else if( !Sys->exit_flag ){
			mode8_line = ud_select_stop( sw, Sys->routine_3, 17 ) ;
			if( Sys->routine_3 != mode8_line ){
				if( Sys->routine_3 >= mode8_line ) Sys->exit_flag = 1 ;
				else Sys->exit_flag = 2 ;
				Sys->timer3 = 11 ;
				Sys->routine_2 = 2 ;
			}
		}
		goto cancel ;
	case 2 :
		Sys->timer3 -- ;
		if( Sys->timer3 == 0 ){
			Sys->exit_flag = 0 ;
			Sys->routine_3 = mode8_line ;
			Sys->routine_2 = 1 ;
		}
	cancel :
		if( cancel_ck() ){
			na_req( 3 ) ;
			Sys->routine_2 = 3 ;
			fade_set( 0xff000000, 10, FADE_OUT ) ;
		}
		break ;
	case 3 :
		if( !fade_cnt() ) Sys->routine_0 = 1 ;
		break ;
	}
}

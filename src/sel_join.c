/* The work that sets sel_w.join_flag, the bits of the sides that have chosen (kind 5 work), made by the character modules. */
#include "player.h"
#include "select.h"

static void sel_join_main( PLAYER_WORK *ew );

/****************************************************************/
void sel_join_make( void )
{
	PLAYER_WORK *ew ;

	if( !( ew = work_get( 0, W_SET, LIST_END ) ) ) return ;
	ew->disp_flag = 0 ;
	ew->work_func = sel_join_main ;
	ew->sub_no = Sys->new_side | Sys->play_side ;
	switch( ew->sub_no ){
	case 1:
		ew->type = 1 ;
		ew->sub_type = 2 ;
		ew->routine_1 = ew->routine_0 = 0 ;
		break ;
	case 2:
		ew->type = 2 ;
		ew->sub_type = 1 ;
		ew->routine_1 = ew->routine_0 = 0 ;
		break ;
	case 3:
		ew->routine_0 = 2 ;
		sel_w.join_flag = 3 ;
		break ;
	}
}

static void sel_join_step( PLAYER_WORK *ew );

/*==============================================================*/
static void sel_join_main( PLAYER_WORK *ew )
{
	if( sel_w.cur_go_flag ) return ;
	switch( ew->routine_0 ){
	case 0:
		sel_join_step( ew ) ;
		break ;
	case 2:
		work_free( ew ) ;
		break ;
	}
}

/*==============================================================*/
static void sel_join_step( PLAYER_WORK *ew )
{
	switch( ew->routine_2 ){
	case 0:
		if( Sys->routine_0 < 3 ) break ;
		ew->routine_2 ++ ;
		sel_w.join_flag |= ew->type ;
	case 1:
		if( Sys->routine_0 == 3 ){
			if( Sys->new_side & ew->sub_type ){
				sel_w.join_flag |= 3 ;
				ew->routine_2 ++ ;
			}
		}
		else if( Sys->routine_0 == 5 ){
			ew->routine_2 ++ ;
			sel_w.join_flag |= 3 ;
		}
		break ;
	case 2:
		break ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "sel_join.c" End of File	*/

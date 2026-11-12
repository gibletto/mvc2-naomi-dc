/* DM0F, a part whose model follows Sys->routine_1, the item chosen. */
#include "player.h"

void dm0f_part3_move( PLAYER_WORK *w ) ;

void dm0f_part3_set( void )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_END ) ) ){
		w->disp_flag = 1 ;
		w->work_func = dm0f_part3_move ;
		w->type = 138 ;
	}
}

void dm0f_part3_move( PLAYER_WORK *w )
{
	w->obj = obj_dm0f->obj[Sys->routine_1 + 138] ;
}

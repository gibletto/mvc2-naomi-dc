/* DM10, a mark beside the memory card menu's line (Sys->routine_1); nothing sets it. */
#include "player.h"

void dm10_mark_move( PLAYER_WORK *w ) ;

static Float dm10_mark_y[3] = { 3.77f, 1.69f, -0.39f } ;

void dm10_mark_set( void )
{
	PLAYER_WORK *w ;

	if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = dm10_mark_move ;
	w->obj = obj_dm10->obj[73] ;
	w->obj_flag = OBJ_POS ;
	w->pos.z = -12.99f ;
}

void dm10_mark_move( PLAYER_WORK *w )
{
	w->pos.y = dm10_mark_y[Sys->routine_1] ;
}

/* DM12: one work drawing the first model of obj_dm12; nothing calls it. */
#include "player.h"

void dm12_nop( PLAYER_WORK *w ) ;

void dm12_set( void )
{
	PLAYER_WORK *w ;

	if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = dm12_nop ;
	w->obj = obj_dm12->obj[0] ;
	w->pos.x = 0.0f ;
	w->pos.y = 0.0f ;
	w->pos.z = 0.0f ;
	w->obj_flag = OBJ_POS ;
	w->timer0 = 0 ;
	w->timer1 = 0 ;
}

void dm12_nop( PLAYER_WORK *w )
{
}

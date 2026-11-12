/* The training menu's backdrop: obj_stg's model 1 on the pause's list, its picture scrolled while
   training_work.unk_1c is set. */
#include "player.h"
#include "dm.h"

void pause_bg_move( PLAYER_WORK *w ) ;

void pause_bg_set( void )
{
	PLAYER_WORK *w ;

	if( !( w = work_get2( 0, W_PAUSE, LIST_TOP ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = pause_bg_move ;
	w->obj = obj_stg->obj[1] ;
	w->pos.x = 0.0f ;
	w->pos.y = 0.0f ;
	w->pos.z = 0.0f ;
	w->obj_flag = OBJ_POS | OBJ_BRIGHT ;
	w->work2 = 0 ;
	obj_uv_clear( w->obj ) ;
}

void pause_bg_move( PLAYER_WORK *w )
{
	Float v, u ;

	if( !training_work.unk_1c ){
		work_free( w ) ;
		return ;
	}
	w->timer0 ++ ;
	if( w->timer0 >= 1000 ) w->timer0 = 0 ;
	w->timer1 ++ ;
	if( w->timer1 >= 1000 ) w->timer1 = 0 ;
	obj_uv_begin( obj_stg->obj[2], w->obj ) ;
	while( !obj_uv_next() ){
		obj_uv_get( &u, &v ) ;
		u -= 1.0f - w->timer0 * 0.001f ;
		v -= 1.0f - w->timer1 * 0.001f ;
		obj_uv_put( &u, &v ) ;
	}
}

/* fade in or out over 30 frames (pause_fade_in, pause_fade_out) */
void pause_fade_set( PLAYER_WORK *w )
{
	w->acc.x = 1.0f / 30.0f ;
	w->timer0 = 30 ;
}

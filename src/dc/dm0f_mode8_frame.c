/* DM0F, mode 8's arrows: above and below the sheet, blinking while there are places to scroll
   to that way. */
#include "player.h"
#include "dm.h"

void dm0f_mode8_frame_move( PLAYER_WORK *w ) ;

static VEC3 dm0f_mode8_frame_pos[2] = { { 0.0f, 5.0f, -12.85f }, { 0.0f, -6.9f, -12.85f } } ;

/****************************************************************/
/*			The arrows: the lower one turned over				*/
/****************************************************************/
void dm0f_mode8_frame_set( void )
{
	PLAYER_WORK *w ;
	int i ;

	for( i = 0 ; i < 2 ; i ++ ){
		if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return ;
		w->disp_flag = 1 ;
		w->work_func = dm0f_mode8_frame_move ;
		w->obj = obj_dm0f->obj[36] ;
		w->obj_flag = OBJ_POS | OBJ_ROT_Z ;
		w->pos = dm0f_mode8_frame_pos[i] ;
		w->rot[2] = DEG_ANG( i * 180 ) ;
		w->type = i ;
		w->timer0 = 0 ;
	}
}

/****************************************************************/
/*			An arrow: blinks every 30 frames, hidden at the top	*/
/*			or the bottom of the ranking						*/
/****************************************************************/
void dm0f_mode8_frame_move( PLAYER_WORK *w )
{
	w->timer0 ++ ;
	if( !( w->timer0 %= 30 ) ) w->routine_0 ^= 1 ;
	if( w->type ){
		if( Sys->routine_3 == 17 ) w->disp_flag = 0 ;
		else w->disp_flag = w->routine_0 ;
	} else if( !Sys->routine_3 ) w->disp_flag = 0 ;
	else w->disp_flag = w->routine_0 ;
}

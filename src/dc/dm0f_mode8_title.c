/* DM0F, mode 8's title: the ranking shown, rank_sa's or rank_score's. */
#include "player.h"

void dm0f_mode8_title_nop( PLAYER_WORK *w ) ;

static VEC3 dm0f_mode8_title_pos = { -0.3f, 6.38f, -12.9f } ;
static VEC3 dm0f_mode8_title_scale = { 0.9f, 0.77f, 1.0f } ;

void dm0f_mode8_title_set( void )
{
	PLAYER_WORK *w ;

	if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = dm0f_mode8_title_nop ;
	w->obj = obj_dm0f->obj[Sys->routine_1 == 0 ? 12 : 13] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE ;
	w->pos = dm0f_mode8_title_pos ;
	w->scale = dm0f_mode8_title_scale ;
}

void dm0f_mode8_title_nop( PLAYER_WORK *w )
{
}

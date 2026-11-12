/* DM0F, the top screen's gauge: its length is CS_Secret_Work.vm_exp per cent. */
#include "player.h"

void dm0f_part2_move( PLAYER_WORK *w ) ;

static VEC3 dm0f_part2_pos = { -1.9f, 4.15f, -12.69f } ;

void dm0f_part2_set( void )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_END ) ) ){
		w->disp_flag = 1 ;
		w->work_func = dm0f_part2_move ;
		w->obj = obj_dm0f->obj[136] ;
		w->obj_flag = OBJ_POS | OBJ_SCALE ;
		w->pos = dm0f_part2_pos ;
		w->scale.x = 1.0f ;
		w->scale.y = 1.0f ;
		w->scale.z = 1.0f ;
	}
}

void dm0f_part2_move( PLAYER_WORK *w )
{
	w->scale.x = CS_Secret_Work.vm_exp / 100.0f ;
}

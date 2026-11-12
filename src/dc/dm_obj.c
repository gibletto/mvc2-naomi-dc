/* The plain model setter: a work that only draws model no of a file. */
#include "player.h"

void dm_obj_set( int no, OBJ_FILE *file ) ;
void dm_obj_nop( PLAYER_WORK *w ) ;

/* model no of DM10 */
void dm10_obj_set( int no )
{
	dm_obj_set( no, obj_dm10 ) ;
}

void dm_obj_set( int no, OBJ_FILE *file )
{
	PLAYER_WORK *w ;

	if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = dm_obj_nop ;
	w->obj = file->obj[no] ;
}

void dm_obj_nop( PLAYER_WORK *w )
{
}

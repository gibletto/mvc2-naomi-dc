/* DM0D, the backdrop behind the lobby's screens: two fixed pictures and one for the screen. */
#include "player.h"
#include "dm0d.h"

void back_move( PLAYER_WORK *w ) ;
PLAYER_WORK *back_parts_set( int obj ) ;

static VEC3 back_pos = { 0.0f, 0.0f, 0.0f } ;
static VEC3 back_scale = { 1.0f, 1.0f, 1.0f } ;
static FADE_COL back_fade = { 1.0f, 1.0f, 1.0f, 1.0f } ;

void dm0d_back_set( int no )
{
	static Uint8 back_obj[7] = { 0x45, 0x46, 0x49, 0x4a, 0x4b, 0x4c, 0x7a } ;

	work_list_kill( W_SYS ) ;
	back_parts_set( 59 ) ;
	back_parts_set( 60 ) ;
	back_parts_set( back_obj[no] ) ;
	back_color_set2( 0xff00003c, 0xff800000 ) ;
}

PLAYER_WORK *back_parts_set( int obj )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_END ) ) ){
		w->disp_flag = 1 ;
		w->work_func = back_move ;
		w->obj = obj_dm0d->obj[obj] ;
		w->sub_no = obj ;
		w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
		w->pos = back_pos ;
		w->scale = back_scale ;
		w->fade = back_fade ;
	}
	return w ;
}

void back_move( PLAYER_WORK *w )
{
}

/* DM0F, the fixed parts of SECRET FACTOR's top screen. */
#include "player.h"

void dm0f_part_set( int n ) ;
void dm0f_part_nop( PLAYER_WORK *w ) ;

static VEC3 dm0f_part_pos[5] = {
	{ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 4.65f, 1.13f, -12.7f }, { 2.5f, 3.11f, -12.69f }, { 3.9f, 3.11f, -12.69f }
} ;
static Uint8 dm0f_part_obj[5] = { 129, 135, 137, 143, 143 } ;

void dm0f_part1_set( void )
{
	int i ;

	for( i = 0 ; i < 5 ; i ++ ) dm0f_part_set( i ) ;
}

void dm0f_part_set( int n )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_END ) ) ){
		w->disp_flag = 1 ;
		w->work_func = dm0f_part_nop ;
		w->obj = obj_dm0f->obj[dm0f_part_obj[n]] ;
		w->obj_flag = OBJ_POS ;
		w->pos = dm0f_part_pos[n] ;
	}
}

void dm0f_part_nop( PLAYER_WORK *w )
{
}

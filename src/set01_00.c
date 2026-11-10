/* Stage 01's first object (kind 5 works): a model whose texture slides
   down as the count runs round. */
#include "player.h"

extern int obj_uv_begin( int *src, void *obj ) ;
extern int obj_uv_next( void ) ;
extern int obj_uv_get( Float *x, Float *y ) ;
extern int obj_uv_put( Float *x, Float *y ) ;

/*==============================================================*/
static void set01_00_main( PLAYER_WORK *ew )
{
	Float x, y ;

	switch( ew->routine_0 ){
	case 0:
		ew->timer0 ++ ;
		if( ew->timer0 >= 500 ) ew->timer0 = 0 ;
		obj_uv_begin( obj_stg->obj[2], ew->obj ) ;
		while( obj_uv_next() == 0 ){
			obj_uv_get( &x, &y ) ;
			y += ew->timer0 * 0.002f ;
			obj_uv_put( &x, &y ) ;
		}
		break ;
	}
}

/****************************************************************/
void set01_00_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set01_00_main ;
		ew->obj = obj_stg->obj[1] ;
		ew->obj_flag = OBJ_POS | OBJ_BRIGHT ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set01_00.c" End of File	*/

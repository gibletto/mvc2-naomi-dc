/* Stage 4's object 1 (the second entry of its table, kind 5 works): a model whose texture slides
   as the count runs round. */
#include "player.h"

extern int obj_uv_begin( int *src, void *obj ) ;
extern int obj_uv_next( void ) ;
extern int obj_uv_get( Float *x, Float *y ) ;
extern int obj_uv_put( Float *x, Float *y ) ;
extern void obj_uv_clear( void *obj ) ;

static void set04_01_main( PLAYER_WORK *ew ) ;

/****************************************************************/
void set04_01_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set04_01_main ;
		ew->obj = obj_stg->obj[2] ;
		ew->obj_flag = OBJ_BRIGHT ;
		obj_uv_clear( ew->obj ) ;
	}
}

/*==============================================================*/
static void set04_01_main( PLAYER_WORK *ew )
{
	Float x, y ;

	switch( ew->routine_0 ){
	case 0:
		ew->timer0 ++ ;
		if( ew->timer0 >= 100 ) ew->timer0 = 0 ;
		obj_uv_begin( obj_stg->obj[3], ew->obj ) ;
		while( obj_uv_next() == 0 ){
			obj_uv_get( &x, &y ) ;
			y += ew->timer0 * 0.01f ;
			obj_uv_put( &x, &y ) ;
		}
		break ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set04_01.c" End of File	*/

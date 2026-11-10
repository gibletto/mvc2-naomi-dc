/* Stage 08's object 7, which nothing calls: a model whose texture slides along u
   once every thousand frames. */
#include "player.h"

extern void obj_uv_begin( int *src, void *obj ) ;
extern int obj_uv_next( void ) ;
extern void obj_uv_get( Float *x, Float *y ) ;
extern void obj_uv_put( Float *x, Float *y ) ;
extern void obj_uv_clear( void *obj ) ;

static void set08_07_main( PLAYER_WORK *ew ) ;

/****************************************************************/
void set08_07_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->obj = obj_stg->obj[19] ;
		ew->work_func = set08_07_main ;
		ew->obj_flag = OBJ_BRIGHT ;
		obj_uv_clear( ew->obj ) ;
	}
}

/*==============================================================*/
static void set08_07_main( PLAYER_WORK *ew )
{
	Float x, y ;

	ew->timer0 ++ ;
	if( ew->timer0 >= 1000 ) ew->timer0 = 0 ;
	obj_uv_begin( obj_stg->obj[20], ew->obj ) ;
	while( obj_uv_next() == 0 ){
		obj_uv_get( &x, &y ) ;
		y += 0.001f * ew->timer0 ;
		obj_uv_put( &x, &y ) ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set08_07.c" End of File	*/

/* Stage 4's object 6 (kind 5 works): a model whose texture slides as the count runs round. */
#include "player.h"

extern int obj_uv_begin( int *src, void *obj ) ;
extern int obj_uv_next( void ) ;
extern int obj_uv_get( Float *x, Float *y ) ;
extern int obj_uv_put( Float *x, Float *y ) ;
extern void obj_uv_clear( void *obj ) ;

/* the frames the texture takes to go round, and how far it slides in one */
#ifdef TARGET_DC	/* five times slower */
#define SET04_06_TIME	1000
#define SET04_06_SPD	0.001f
#else
#define SET04_06_TIME	200
#define SET04_06_SPD	0.005f
#endif

static void set04_06_main( PLAYER_WORK *ew ) ;

/****************************************************************/
void set04_06_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set04_06_main ;
		ew->obj = obj_stg->obj[34] ;
		ew->obj_flag = OBJ_BRIGHT ;
		obj_uv_clear( ew->obj ) ;
	}
}

/*==============================================================*/
static void set04_06_main( PLAYER_WORK *ew )
{
	Float x, y ;

	switch( ew->routine_0 ){
	case 0:
		ew->timer0 ++ ;
		if( ew->timer0 >= SET04_06_TIME ) ew->timer0 = 0 ;
		obj_uv_begin( obj_stg->obj[35], ew->obj ) ;
		while( obj_uv_next() == 0 ){
			obj_uv_get( &x, &y ) ;
			y += ew->timer0 * SET04_06_SPD ;
			obj_uv_put( &x, &y ) ;
		}
		break ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set04_06.c" End of File	*/

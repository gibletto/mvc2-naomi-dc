/* The select scene's plate behind the cards (a kind 5 work), whose texture scrolls. ef05.c, which
   follows, has the portraits and cursors. */
#include "player.h"
#include "select.h"

extern void obj_uv_begin( int *src, void *obj );
extern void obj_uv_get( Float *x, Float *y );
extern void obj_uv_put( Float *x, Float *y );
extern int obj_uv_next( void );
extern void obj_uv_clear( void *obj );

static VEC3 sel_plate_pos = { 0.0f, 200.0f, -750.0f } ;
static VEC3 sel_plate_rot = { 0.0f, 0.0f, 0.0f } ;	/* never used */

static void sel_plate_main( PLAYER_WORK *ew );

/****************************************************************/
/*			The plate behind the cards, whose texture			*/
/*			scrolls												*/
/****************************************************************/
void sel_plate_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = sel_plate_main ;
		ew->obj = obj_dm01->obj[6] ;
		ew->pos = sel_plate_pos ;
		ew->obj_flag = OBJ_POS | OBJ_BRIGHT ;
		obj_uv_clear( ew->obj ) ;
	}
}

/*==============================================================*/
static void sel_plate_main( PLAYER_WORK *ew )
{
	Float px, py ;

	ew->timer0 ++ ;
	if( ew->timer0 >= 1000 ) ew->timer0 = 0 ;
	obj_uv_begin( obj_dm01->obj[7], ew->obj ) ;
	while( obj_uv_next() == 0 ){
		obj_uv_get( &px, &py ) ;
		py -= ew->timer0 * 0.001f ;
		obj_uv_put( &px, &py ) ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "sel_plate.c" End of File	*/

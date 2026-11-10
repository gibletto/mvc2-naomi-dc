/* The start demo's starburst (kind 5): a picture of the demo's models that
   st_demo_ef2.c makes at a place, which grows for eight frames and ends. */
#include "player.h"

static const VEC3 st_demo_ef3_one = { 1.0f, 1.0f, 1.0f } ;

static void st_demo_ef3_main( PLAYER_WORK *ew ) ;

/****************************************************************/
void st_demo_ef3_set( int no, VEC3 pos )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->type = no ;
		ew->work_func = st_demo_ef3_main ;
		ew->obj = obj_dm02->obj[no + 19] ;
		ew->scale = st_demo_ef3_one ;
		ew->pos = pos ;
		ew->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_BRIGHT ;
		ew->timer0 = 0 ;
	}
}

/*==============================================================*/
static void st_demo_ef3_main( PLAYER_WORK *ew )
{
	if( Sys->routine_0 == 2 && ew->timer0 != 8 ){
		ew->timer0 ++ ;
		ew->scale.x = 1.0f + ew->timer0 * 0.375f ;
		ew->scale.y = ew->scale.x ;
	} else {
		work_free( ew ) ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "st_demo_ef3.c" End of File	*/

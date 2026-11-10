/* The first of the stage's objects set up by bg_init: a work of kind 11 that shows an object of the
   scene's model file at the origin and does nothing else. */
#include "player.h"

/* where it stands, and its size */
static VEC3 bg_obj0_pos = { 0.0f, 0.0f, 0.0f } ;
static VEC3 bg_obj0_scale = { 1.0f, 1.0f, 1.0f } ;

static void bg_obj0_main( PLAYER_WORK *ew );

/****************************************************************/
void bg_obj0_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SYS, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = bg_obj0_main ;
		ew->obj = obj_dm00->obj[60] ;
		ew->obj_flag = 0x1081f ;
		ew->pos = bg_obj0_pos ;
		ew->scale = bg_obj0_scale ;
	}
}

/*==============================================================*/
static void bg_obj0_main( PLAYER_WORK *ew )
{
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "bg_obj0.c" End of File	*/

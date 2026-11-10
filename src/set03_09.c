/* Stage 03's object 9: a still model (object 11 of the stage's file). */
#include "player.h"

static VEC3 set03_09_pos = { 847.0f, 700.0f, -1695.0f } ;

/*==============================================================*/
static void set03_09_main( PLAYER_WORK *ew )
{
}

/****************************************************************/
void set03_09_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set03_09_main ;
		ew->obj = obj_stg->obj[11] ;
		ew->pos = set03_09_pos ;
		ew->obj_flag = OBJ_POS | OBJ_FACE_Y | OBJ_BRIGHT ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set03_09.c" End of File	*/

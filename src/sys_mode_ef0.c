/* A still model of the demo before the attract's end (kind 5 work): the model 8
   of obj_dm09, made when sys_mode.c's count reaches 0xeb. */
#include "player.h"

static void sys_mode_ef0_main( PLAYER_WORK *ew ) ;

/****************************************************************/
void sys_mode_ef0_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = sys_mode_ef0_main ;
		ew->obj = obj_dm09->obj[8] ;
		ew->obj_flag = OBJ_BRIGHT ;
	}
}

/*==============================================================*/
static void sys_mode_ef0_main( PLAYER_WORK *ew )
{
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "sys_mode_ef0.c" End of File	*/

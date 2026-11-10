/* Stage 4's object 3 (kind 5 works): a model that changes to one of three others in turn, five
   frames to each, as the count runs round. */
#include "player.h"

/* the models' order, as steps from the stage file's fifth */
static Sint8 set04_03_obj_tbl[15] = { 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2 } ;

/*==============================================================*/
static void set04_03_main( PLAYER_WORK *ew )
{
	ew->obj = ( obj_stg->obj + 5 )[set04_03_obj_tbl[ew->timer0]] ;
	if( ( Uint32 )++ew->timer0 >= 15 ) ew->timer0 = 0 ;
}

/****************************************************************/
void set04_03_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set04_03_main ;
		ew->obj_flag = OBJ_BRIGHT ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set04_03.c" End of File	*/

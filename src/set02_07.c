/* Stage 02's object 7: a model that is changed for the next of four (objects 45 to 48 of the
   stage's file) every ten frames, and blinks. */
#include "player.h"

static const Sint32 set02_07_obj_tbl[4] = { 45, 46, 47, 48 } ;
static const VEC3 set02_07_pos = { 2040.0f, -560.0f, -12890.0f } ;

/*==============================================================*/
static void set02_07_main( PLAYER_WORK *ew )
{
	if( ++ ew->timer0 >= 10 ){
		ew->timer0 = 0 ;
		ew->routine_0 ++ ;
		ew->routine_0 &= 3 ;
		ew->obj = obj_stg->obj[set02_07_obj_tbl[ew->routine_0]] ;
	}
	if( !( ew->timer0 % 2 ) ) ew->disp_flag ^= 1 ;
}

/*==============================================================*/
static void set02_07_make( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set02_07_main ;
		ew->obj = obj_stg->obj[45] ;
		ew->obj_flag = OBJ_POS | OBJ_BRIGHT ;
		ew->pos = set02_07_pos ;
	}
}

/****************************************************************/
void set02_07_set( void )
{
	set02_07_make() ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set02_07.c" End of File	*/

/* Stage 05's puff (the object 2 the stage's table has no entry for): the
   model (object 5 of the stage's file) that the falling models of object 1 leave where they land;
   it grows for a time and goes. */
#include "player.h"

static const VEC3 set05_02_scale = { 1.0f, 1.0f, 1.0f } ;	/* a scale of one on each axis */

static void set05_02_main( PLAYER_WORK *ew ) ;

/****************************************************************/
/*			A puff at a place									*/
/****************************************************************/
void set05_02_set( VEC3 *pos )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set05_02_main ;
		ew->obj = obj_stg->obj[5] ;
		ew->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_BRIGHT ;
		ew->pos = *pos ;
		ew->scale = set05_02_scale ;
	}
}

/*==============================================================*/
static void set05_02_main( PLAYER_WORK *ew )
{
	switch( ew->routine_0 ){
	case 0 :
		ew->routine_0 ++ ;
		ew->timer1 = 120 ;
		break ;
	case 1 :
		ew->scale.x += 0.1f ;
		ew->scale.z += 0.1f ;
		if( ++ ew->timer0 >= ew->timer1 ) work_free( ew ) ;
		break ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set05_02.c" End of File	*/

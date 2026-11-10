/* Stage 08's object 4: a model that waits for the second fight, then changes its fade colour from
   green to red over 300 frames. */
#include "player.h"

static Sint32 set08_04_dur = 300 ;

/*==============================================================*/
static void set08_04_main( PLAYER_WORK *ew )
{
	switch( ew->routine_0 ){
	case 0 :
		if( stage_work.event >= 2 ) ew->routine_0 ++ ;
		break ;
	case 1 :
		ew->fade.r += 1.0f / ( Float )set08_04_dur ;
		ew->fade.g -= 1.0f / ( Float )set08_04_dur ;
		if( ++ ew->timer0 >= set08_04_dur ){
			ew->routine_0 = 2 ;
			ew->timer0 = 0 ;
			ew->fade.r = 1.0f ;
			ew->fade.g = 0.0f ;
		}
		break ;
	case 2 :
		break ;
	}
}

/****************************************************************/
void set08_04_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set08_04_main ;
		ew->obj = obj_stg->obj[0] ;
		ew->obj_flag = OBJ_POS | OBJ_FADE | OBJ_BRIGHT ;
		ew->fade.r = 0.0f ;
		ew->fade.g = 1.0f ;
		ew->fade.b = 0.0f ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set08_04.c" End of File	*/

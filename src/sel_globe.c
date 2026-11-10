/* The select scene's globe (a kind 5 work): it starts big and turns, and settles to its size. */
#include "player.h"
#include "select.h"

static void sel_globe_main( PLAYER_WORK *ew );

/****************************************************************/
/*			The globe: it starts big and turns, and settles		*/
/*			to its size											*/
/****************************************************************/
void sel_globe_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = sel_globe_main ;
		ew->base_mtx = &sel_back_w->obj_mtx ;
		ew->obj = obj_dm01->obj[1] ;
		ew->pos.x = 0.0f ;
		ew->pos.y = -1.0f ;
		ew->pos.z = 0.0f ;
		ew->rot[1] = 0x4000 ;
		ew->scale.x = 4.0f ;
		ew->scale.y = 4.0f ;
		ew->scale.z = 1.0f ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_SCALE | OBJ_BRIGHT ;
	}
}

/*==============================================================*/
static void sel_globe_main( PLAYER_WORK *ew )
{
	ew->rot[1] -= 0x100 ;
	switch( ew->routine_0 ){
	case 0:
		ew->scale.x -= 0.05f ;
		ew->scale.y -= 0.05f ;
		if( ew->scale.x < 1.0f ){
			ew->routine_0 ++ ;
			ew->scale.x = 1.0f ;
			ew->scale.y = 1.0f ;
		}
		break ;
	case 1:
		break ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "sel_globe.c" End of File	*/

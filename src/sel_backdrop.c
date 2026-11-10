/* The select scene's backdrop works (kind 5), after the picture module. */
#include "player.h"
#include "select.h"

static void sel_backdrop_set( int side );

/****************************************************************/
void sel_backdrop_make( void )
{
	int i ;

	for( i = 0 ; i < 2 ; i ++ ) sel_backdrop_set( i ) ;
}

static void sel_backdrop_main( PLAYER_WORK *ew );

/*==============================================================*/
/*			The side's backdrop piece, turning slowly on the	*/
/*			select's back work									*/
/*==============================================================*/
static void sel_backdrop_set( int side )
{
	PLAYER_WORK *ew ;

	if( !( ew = work_get( 0, W_SET, LIST_END ) ) ) return ;
	ew->disp_flag = 1 ;
	ew->base_mtx = &sel_back_w->obj_mtx ;
	ew->work_func = sel_backdrop_main ;
	ew->obj = obj_dm01->obj[side + 8] ;
	ew->pos.x = 0.0f ;
	ew->pos.y = 0.0f ;
	ew->pos.z = 0.0f ;
	ew->rot[0] = 0 ;
	ew->rot[1] = 0 ;
	ew->rot[2] = 0 ;
	ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_ROT_Z | OBJ_BRIGHT ;
	ew->work3 = 0 ;
}

/*==============================================================*/
static void sel_backdrop_main( PLAYER_WORK *ew )
{
	ew->rot[0] += 0x100 ;
	ew->rot[1] += 0x180 ;
	ew->rot[2] += 0x80 ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "sel_backdrop.c" End of File	*/

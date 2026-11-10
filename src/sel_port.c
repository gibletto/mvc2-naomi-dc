/* The part that rides each portrait of the select scene's grid (kind 5 works), a module after sel_light. */
#include "player.h"
#include "select.h"

static void sel_port_main( PLAYER_WORK *ew );

/****************************************************************/
/*			A part that takes the portrait's place				*/
/****************************************************************/
PLAYER_WORK *sel_port_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( !( ew = work_get( 0, W_SET, LIST_END ) ) ) return ew ;

	ew->disp_flag = 1 ;
	ew->work_func = sel_port_main ;
	ew->master_w = pl_w ;
	ew->base_mtx = &pl_w->obj_mtx ;
	ew->sub_type = pl_w->sub_type ;
	ew->obj = obj_dm01->obj[ef05_cell_tbl[pl_w->sub_type].row + 121] ;
	ew->pos.x = 0.0f ;
	ew->pos.y = 0.0f ;
	ew->pos.z = 0.0f ;
	ef05_rot_set( ew, ew->sub_type ) ;
	ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_BRIGHT ;
	return ew ;
}

static void sel_port_fly( PLAYER_WORK *ew );

/****************************************************************/
/*			The same part, for a portrait that flies out		*/
/****************************************************************/
void sel_port_fly_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = sel_port_set( pl_w ) ) ) ew->work_func = sel_port_fly ;
}

static void sel_port_wait( PLAYER_WORK *ew );
static void sel_port_free( PLAYER_WORK *ew );

/*==============================================================*/
static void sel_port_main( PLAYER_WORK *ew )
{
	static void ( *sel_port_jmp[] )( PLAYER_WORK *ew ) = {
		sel_port_wait, sel_port_free,
	} ;

	sel_port_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
/*			It is hidden once the portrait has come in or		*/
/*			the grid is chosen from								*/
/*==============================================================*/
static void sel_port_wait( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->pos.z < 170.0f || pl_w->routine_0 >= 2 ){
		ew->disp_flag = 0 ;
		ew->routine_0 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void sel_port_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
static void sel_port_fly( PLAYER_WORK *ew )
{
	ew->disp_flag = ew->master_w->disp_flag ;
	if( !ew->routine_0 ){
		if( !ew->disp_flag ) ew->routine_0 ++ ;
	}
	else work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "sel_port.c" End of File	*/

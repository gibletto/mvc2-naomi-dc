/* The select scene's picture work (kind 5), after the face module. */
#include "player.h"
#include "select.h"

static void sel_pic_main( PLAYER_WORK *ew );

/****************************************************************/
/*			The picture of the character a piece's side			*/
/*			chose, shown once it has							*/
/****************************************************************/
void sel_pic_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( !( ew = work_get( 0, W_SET, LIST_END ) ) ) return ;
	ew->disp_flag = 0 ;
	ew->work_func = sel_pic_main ;
	ew->master_w = pl_w ;
	ew->base_mtx = &pl_w->obj_mtx ;
	ew->ef_w = pl_w->ef_w ;
	ew->type = pl_w->type ;
	ew->sub_type = pl_w->sub_type ;
	ew->pos.x = 0.0f ;
	ew->pos.y = 0.0f ;
	ew->pos.z = 1.0f ;
	ew->obj_flag = OBJ_POS | OBJ_BRIGHT ;
}

/* which model of the file goes with each side's three personal places */
static Sint8 sel_pic_obj_tbl[2][3] = {
	{ 0x52, 0x53, 0x54 },
	{ 0x55, 0x56, 0x57 },
} ;

static Sint32 sel_pic_unk_tbl[7] = { 8, 9, 9, 9, 10, 10, 10 } ;

/*==============================================================*/
static void sel_pic_main( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	switch( ew->routine_0 ){
	case 0:
		if( sel_w.pick_flag[ew->type] & ( 16 << ew->sub_type ) ){
			pl_w = ew->ef_w ;
			ew->routine_0 ++ ;
			ew->disp_flag = 1 ;
			ew->obj = obj_dm01->obj[sel_pic_obj_tbl[ew->type][pl_w->pl_personal]] ;
		}
		break ;
	case 1:
		break ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "sel_pic.c" End of File	*/

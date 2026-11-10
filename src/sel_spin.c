/* The piece that spins behind each cursor card of the select scene (kind 5 works). */
#include "player.h"
#include "select.h"

static void sel_spin_main( PLAYER_WORK *ew );

/* where each side's piece stands */
static VEC3 sel_spin_pos_tbl[2] = {
	{ 0.0f, 0.0f, 208.2f },
	{ 0.0f, 0.0f, 208.0f },
} ;

/****************************************************************/
void sel_spin_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( !( ew = work_get( 0, W_SET, LIST_END ) ) ) return ;
	ew->disp_flag = 1 ;
	ew->master_w = pl_w ;
	ew->base_mtx = &pl_w->obj_mtx ;
	ew->type = pl_w->type ;
	ew->sub_type = pl_w->sub_type ;
	ew->work_func = sel_spin_main ;
	ew->obj = obj_dm01->obj[pl_w->type + 2] ;
	ew->pos = sel_spin_pos_tbl[pl_w->type] ;
	ew->rot[2] = 0 ;
	ew->obj_flag = OBJ_POS | OBJ_ROT_Z | OBJ_SCALE | OBJ_BRIGHT ;
	ew->scale.x = 1.0f ;
	ew->scale.y = 1.0f ;
	ew->scale.z = 1.0f ;
	ew->dir = pl_w->type ;
}

static void sel_spin_turn( PLAYER_WORK *ew );
static void sel_spin_show( PLAYER_WORK *ew );
static void sel_spin_free( PLAYER_WORK *ew );

/*==============================================================*/
static void sel_spin_main( PLAYER_WORK *ew )
{
	static void ( *sel_spin_jmp[] )( PLAYER_WORK *ew ) = {
		sel_spin_turn, sel_spin_show, sel_spin_free,
	} ;

	sel_spin_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
/*			It spins until its card is chosen					*/
/*==============================================================*/
static void sel_spin_turn( PLAYER_WORK *ew )
{
	if( ew->type ) ew->rot[2] -= 0x400 ;
	else ew->rot[2] += 0x400 ;
	if( sel_w.pick_flag[ew->type] & ( 1 << ew->sub_type ) ){
		ew->routine_0 ++ ;
		ew->obj = obj_dm01->obj[ef05_cell_tbl[ew->master_w->pl_type].row + 120] ;
		ew->obj_flag = OBJ_SCALE | OBJ_BRIGHT ;
		ew->timer0 = 20 ;
	}
}

/*--------------------------------------------------------------*/
static void sel_spin_show( PLAYER_WORK *ew )
{
	ew->timer0 -- ;
	if( ew->timer0 <= 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void sel_spin_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "sel_spin.c" End of File	*/

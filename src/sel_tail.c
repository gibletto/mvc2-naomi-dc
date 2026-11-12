/* Small routines of the select scene's kind 5 works, after the cells: the mark at a character's
   own assist type (the arcade's routine is empty), and a work's steps: hide it, hide and free it. */
#include "player.h"

extern void sel_dummy_1( PLAYER_WORK *w );
void sel_hide( PLAYER_WORK *ew );
void sel_hide_free( PLAYER_WORK *ew );

/* six places, by side and slot (the arcade never uses them) */
static VEC3 sel_tail_pos_tbl[2][3] = {
	{ { 4.0f, 78.0f, 19.9f }, { 8.0f, 79.5f, 19.9f }, { -8.0f, 82.5f, 19.9f } },
	{ { -4.0f, 78.0f, 19.9f }, { -8.0f, 79.5f, 19.9f }, { 8.0f, 82.5f, 19.9f } },
} ;

/* each character's assist type to start with, by character */
Sint8 sel_personal_tbl[60] = {
	1, 1, 2, 2, 0, 0, 0, 0, 2, 2, 1, 2, 0, 1, 0, 2, 0, 0, 0, 1,
	0, 0, 2, 0, 0, 0, 0, 0, 2, 2, 2, 1, 1, 2, 0, 2, 0, 1, 1, 0,
	1, 1, 0, 0, 1, 2, 0, 0, 0, 2, 0, 1, 0, 0, 0, 0, 1, 2, 2, 0,
} ;

/* a work's steps with the routines below (the arcade never uses them) */
static void ( *sel_tail_jmp[] )( PLAYER_WORK *ew ) = {
	sel_dummy_1, sel_dummy_1, sel_dummy_1, sel_hide, sel_hide, sel_hide_free,
} ;

static void sel_tail_main( PLAYER_WORK *ew ) ;

/****************************************************************/
void sel_nop( PLAYER_WORK *pl_w )
{
#ifdef TARGET_DC	/* a mark, and its plate behind it, at the place of the character's own type */
	PLAYER_WORK *ew, *tw ;

	tw = pl_w->ef_w ;
	if( pl_w->sub_type != sel_personal_tbl[tw->load_type] ) return ;
	if( ( ew = work_get( 0, W_SET, LIST_END ) ) == 0 ) return ;
	ew->disp_flag = 1 ;
	ew->master_w = pl_w->master_w ;
	ew->base_mtx = &pl_w->obj_mtx ;
	ew->ef_w = pl_w->ef_w ;
	ew->type = pl_w->type ;
	ew->sub_type = pl_w->sub_type ;
	ew->obj = obj_dm01->obj[179] ;
	ew->work_func = sel_tail_main ;
	ew->pos = sel_tail_pos_tbl[ew->type][ew->sub_type] ;
	if( ew->type ){
		ew->rot[0] = 0x2000 ;
		ew->rot[1] = 0x8000 ;
	} else {
		ew->rot[0] = 0xe001 ;
		ew->rot[1] = 0 ;
	}
	ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_BRIGHT ;
	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->master_w = pl_w->master_w ;
		ew->base_mtx = &pl_w->obj_mtx ;
		ew->ef_w = pl_w->ef_w ;
		ew->type = pl_w->type ;
		ew->sub_type = pl_w->sub_type ;
		if( Sys->cpu_level_type ) ew->obj = obj_dm01->obj[174] ;
		else ew->obj = obj_dm01->obj[180] ;
		ew->work_func = sel_tail_main ;
		ew->pos = sel_tail_pos_tbl[ew->type][ew->sub_type] ;
		ew->pos.z += 0.1f ;
		ew->rot[0] = 0xe001 ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_BRIGHT ;
	}
#endif
}

/*==============================================================*/
static void sel_tail_main( PLAYER_WORK *ew )
{
	sel_tail_jmp[ew->master_w->routine_0]( ew ) ;
}

/****************************************************************/
void sel_hide( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
}

/****************************************************************/
void sel_hide_free( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "sel_tail.c" End of File	*/

/* The face that rides on a cursor piece in the select scene (a kind 5 work), a module of its own
   after the cursor pieces of ef05.c: its pool ends at 0C1C6754. */
#include "player.h"
#include "select.h"

extern void face_tex_set( Sint32 no );
extern void sel_dummy_1( PLAYER_WORK *w );

static VEC3 sel_face_pos_tbl[2] = {
	{ 0.0f, 0.0f, -0.0f },
	{ -0.0f, 0.0f, -0.0f },
} ;

static void sel_face_main( PLAYER_WORK *ew );

/****************************************************************/
/*			The face that goes with a cursor piece, riding		*/
/*			on it												*/
/****************************************************************/
void sel_face_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( !( ew = work_get( 0, W_SET, LIST_END ) ) ) return ;
	ew->disp_flag = 1 ;
	ew->work_func = sel_face_main ;
	ew->master_w = pl_w ;
	ew->base_mtx = &pl_w->obj_mtx ;
	ew->ef_w = pl_w->ef_w ;
	ew->type = pl_w->type ;
	ew->sub_type = pl_w->sub_type ;
	ew->pos = sel_face_pos_tbl[pl_w->type] ;
	ew->obj = obj_dm01->obj[pl_w->type * 3 + pl_w->sub_type + 76] ;
	ew->rot[0] = 0 ;
	ew->rot[1] = 0 ;
	ew->rot[2] = 0 ;
	ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_BRIGHT ;
	if( sel_w.pick_flag[pl_w->type] & ( 1 << pl_w->sub_type ) ){
		ew->routine_0 = 6 ;
		tex_req_flag = 1 ;
		tex_req_no |= ew->type + 1 ;
		face_tex_set( ew->sub_type * 2 + ew->type ) ;
		if( !ew->type ) ew->rot[1] = 0x8000 ;
		if( Sys->stage_cnt == 7 || ew->sub_type == 2 ) sel_w.work_step[ew->type] = 1 ;
	}
	else ew->timer0 = pl_w->sub_type * 10 ;
}

static void sel_face_wait( PLAYER_WORK *ew );
static void sel_face_pop( PLAYER_WORK *ew );
static void sel_face_pick( PLAYER_WORK *ew );
static void sel_face_out( PLAYER_WORK *ew );
static void sel_face_go( PLAYER_WORK *ew );
static void sel_face_end( PLAYER_WORK *ew );

/*==============================================================*/
static void sel_face_main( PLAYER_WORK *ew )
{
	static void ( *ef05_face_jmp[] )( PLAYER_WORK *ew ) = {
		sel_face_wait, sel_face_pop, sel_face_pick, sel_face_out, sel_face_go, sel_face_end, sel_dummy_1,
	} ;

	ef05_face_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void sel_face_wait( PLAYER_WORK *ew )
{
	ew->timer0 -- ;
	if( ew->timer0 <= 0 ) ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void sel_face_pop( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	if( ew->sub_type == 2 ) sel_w.work_step[ew->type] = 1 ;
}

/*==============================================================*/
/*			When the side's pick names this piece's				*/
/*			character											*/
/*==============================================================*/
static void sel_face_pick( PLAYER_WORK *ew )
{
	if( sel_w.pick_no[ew->type] ){
		if( sel_w.pick_no[ew->type] - 1 == ew->sub_type ){
			ew->routine_0 ++ ;
			if( !ew->type ) ew->rot[1] = 0x8000 ;
		}
	}
}

/*--------------------------------------------------------------*/
static void sel_face_out( PLAYER_WORK *ew )
{
	if( sel_w.pick_flag[ew->type] & ( 1 << ew->sub_type ) ) ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void sel_face_go( PLAYER_WORK *ew )
{
	if( sel_w.pick_flag[ew->type] & ( 16 << ew->sub_type ) ) ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void sel_face_end( PLAYER_WORK *ew )
{
	if( sel_w.work_step[ew->type] == 3 ) ew->routine_0 ++ ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "sel_face.c" End of File	*/

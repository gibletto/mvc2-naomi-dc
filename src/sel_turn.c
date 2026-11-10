/* The select scene's turning backdrop piece (kind 5), a module of its own after the name plate's:
   its text starts on a fresh word and its pool is apart from sel_name.c's. */
#include "player.h"
#include "select.h"

/* the turns, in degrees, of the backdrop piece for each side and each personal place */
static Sint32 sel_turn_ang_tbl[2][59][2] = {
	{
		{ 341, 339 },
		{ 351, 340 },
		{ 331, 340 },
		{ 11, 340 },
		{ 1, 340 },
		{ 21, 339 },
		{ 331, 2 },
		{ 341, 13 },
		{ 321, 2 },
		{ 351, 13 },
		{ 321, 13 },
		{ 1, 2 },
		{ 1, 13 },
		{ 21, 13 },
		{ 11, 13 },
		{ 11, 2 },
		{ 341, 328 },
		{ 331, 328 },
		{ 1, 351 },
		{ 351, 351 },
		{ 341, 351 },
		{ 331, 351 },
		{ 341, 2 },
		{ 351, 2 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 31, 351 },
		{ 321, 340 },
		{ 31, 340 },
		{ 351, 328 },
		{ 11, 351 },
		{ 321, 328 },
		{ 351, 317 },
		{ 11, 329 },
		{ 11, 317 },
		{ 21, 328 },
		{ 1, 317 },
		{ 321, 351 },
		{ 341, 317 },
		{ 351, 24 },
		{ 1, 24 },
		{ 31, 13 },
		{ 11, 35 },
		{ 331, 13 },
		{ 21, 2 },
		{ 331, 24 },
		{ 341, 24 },
		{ 31, 2 },
		{ 1, 36 },
		{ 341, 35 },
		{ 21, 24 },
		{ 31, 24 },
		{ 351, 35 },
		{ 321, 24 },
		{ 1, 328 },
		{ 21, 351 },
		{ 11, 24 },
		{ 31, 328 },
	},
	{
		{ -11, -14 },
		{ -1, -14 },
		{ -21, -13 },
		{ 18, -14 },
		{ 8, -14 },
		{ 29, -13 },
		{ 339, 9 },
		{ -11, 20 },
		{ 329, 9 },
		{ 359, 20 },
		{ -31, 20 },
		{ 9, 9 },
		{ 9, 20 },
		{ 29, 20 },
		{ 18, 20 },
		{ 19, 9 },
		{ -11, -25 },
		{ -21, -24 },
		{ 9, 358 },
		{ -1, -2 },
		{ -11, 358 },
		{ -21, -2 },
		{ -11, 9 },
		{ -1, 9 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 38, 358 },
		{ -31, -13 },
		{ 38, 346 },
		{ -1, -25 },
		{ 19, 358 },
		{ -31, -24 },
		{ -1, 325 },
		{ 18, 335 },
		{ 19, 325 },
		{ 29, 336 },
		{ 9, 325 },
		{ -31, -2 },
		{ -11, 325 },
		{ -1, 31 },
		{ 9, 31 },
		{ 38, 20 },
		{ 18, 43 },
		{ -21, 20 },
		{ 29, 9 },
		{ -21, 31 },
		{ -11, 31 },
		{ 38, 9 },
		{ 9, 43 },
		{ -11, 42 },
		{ 29, 31 },
		{ 38, 31 },
		{ -1, 42 },
		{ -31, 31 },
		{ 9, 336 },
		{ 29, -2 },
		{ 19, 31 },
		{ 38, 335 },
	},
} ;

static void sel_turn_main( PLAYER_WORK *ew );

/****************************************************************/
/*			A side's turning piece, set from the angle table	*/
/*			by the personal place of tw							*/
/****************************************************************/
void sel_turn_set( PLAYER_WORK *tw, Uint8 side )
{
	PLAYER_WORK *ew ;

	if( !( ew = work_get( 0, W_SET, LIST_END ) ) ) return ;
	ew->disp_flag = 1 ;
	ew->work_func = sel_turn_main ;
	ew->base_mtx = &sel_back_w->obj_mtx ;
	ew->type = side ;
	ew->obj = obj_dm01->obj[side + 68] ;
	ew->rot[0] = ( Sint32 )( sel_turn_ang_tbl[side][tw->load_type][0] * 65536.0f / 360.0f + 0.5f ) & 0xffff ;
	ew->rot[1] = ( Sint32 )( sel_turn_ang_tbl[side][tw->load_type][1] * 65536.0f / 360.0f + 0.5f ) & 0xffff ;
	ew->rot[2] = 0 ;
	ew->obj_flag = OBJ_ROT_X | OBJ_ROT_Y | OBJ_BRIGHT ;
	ew->pl_type = tw->load_type ;
}

/*==============================================================*/
static void sel_turn_main( PLAYER_WORK *ew )
{
	switch( ew->routine_0 ){
	case 0:
		if( Sys->exit_flag ){
			ew->routine_0 ++ ;
			ew->disp_flag = 0 ;
		}
		break ;
	case 1:
		work_free( ew ) ;
		break ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "sel_turn.c" End of File	*/

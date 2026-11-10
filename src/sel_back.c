/* The select scene's background works (kind 5): a ring of 32 copies of one model that move with the
   scene's step. sel_globe.c and sel_plate.c follow. */
#include "player.h"
#include "select.h"

extern void move_set_xy( PLAYER_WORK *wk, Sint32 time, Float x, Float y, Sint8 x_type, Sint8 y_type );

/* where the ring's copies go in each step of the scene (x and y; the last is for the pair) */
static VEC3 sel_back_pos[] = {
	{ 0.0f, 190.0f, 0.0f },
	{ 80.0f, 190.0f, 0.0f },
	{ -80.0f, 190.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f },
} ;

static void sel_back_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *sel_back_set( void )
{
	register PLAYER_WORK *ew, *top ;
	register Uint8 i ;

	if( ( top = work_get( 0, W_SET, LIST_END ) ) ){
		top->disp_flag = 1 ;
		top->work_func = sel_back_main ;
		top->obj = obj_dm01->obj[0] ;
		top->pos = sel_back_pos[0] ;
		top->rot[1] = 0 ;
		top->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_BRIGHT ;
		top->work3 = 0 ;
		for( i = 1 ; i < 32 ; i ++ ){
			if( !( ew = work_get( 0, W_SET, LIST_END ) ) ) return ew ;
			ew->disp_flag = 1 ;
			ew->work_func = sel_back_main ;
			ew->obj = obj_dm01->obj[0] ;
			ew->pos = sel_back_pos[0] ;
			ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_BRIGHT ;
			ew->rot[1] = ( Sint32 )( i * 737280.0f / 360.0f + 0.5f ) & 0xffff ;
			ew->work3 = 0 ;
		}
	}
	return top ;
}

/****************************************************************/
/*			Nothing calls this one: two more works of the		*/
/*			ring, the model and a flat copy of it				*/
/****************************************************************/
void sel_back_set2( void )
{
	PLAYER_WORK *ew ;

	if( !( ew = work_get( 0, W_SET, LIST_END ) ) ) return ;
	ew->disp_flag = 1 ;
	ew->work_func = sel_back_main ;
	ew->obj = obj_dm01->obj[0] ;
	ew->pos = sel_back_pos[0] ;
	ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_BRIGHT ;
	if( !( ew = work_get( 0, W_SET, LIST_END ) ) ) return ;
	ew->disp_flag = 1 ;
	ew->work_func = sel_back_main ;
	ew->obj = obj_dm01->obj[67] ;
	ew->pos = sel_back_pos[3] ;
	ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_BRIGHT ;
}

/*==============================================================*/
/*			A copy of the ring moves to the place of the		*/
/*			scene's step, each time the step changes			*/
/*==============================================================*/
static void sel_back_main( PLAYER_WORK *ew )
{
	SEL_WORK *sw = &sel_w ;
	Uint8 x ;

	if( sw->cur_go_flag ) return ;
	switch( ew->routine_0 ){
	case 0:
		x = ew->work3 ^ sw->join_flag ;
		if( !x ) break ;
		ew->routine_0 ++ ;
		ew->timer0 = 25 ;
		if( sw->join_flag == 3 ){
			move_set_xy( ew, ew->timer0, sel_back_pos[0].x, sel_back_pos[0].y, 0, 0 ) ;
		} else {
			move_set_xy( ew, ew->timer0, sel_back_pos[x].x, sel_back_pos[x].y, 0, 0 ) ;
		}
		ew->work3 |= x ;
		break ;
	case 1:
		ew->timer0 -- ;
		if( ew->timer0 <= 0 ) ew->routine_0 = 0 ;
		else ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		break ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "sel_back.c" End of File	*/

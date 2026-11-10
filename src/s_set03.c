/* Stage 03's own code that is not an object's set-up: the lit parts of object 7, each of which
   changes from one model to another by a table. */
#include "player.h"

/* which of each part's two models shows, for each of the table's rows */
static Sint32 set03_07_on_tbl[8][3] = {
	{ 0, 0, 0 },
	{ 0, 0, 1 },
	{ 0, 1, 0 },
	{ 0, 1, 1 },
	{ 1, 0, 0 },
	{ 1, 0, 1 },
	{ 1, 1, 0 },
	{ 1, 1, 1 },
} ;
/* the row of that table each step shows */
static Sint32 set03_07_row_tbl[20] = { 7, 7, 0, 4, 2, 1, 4, 2, 1, 0, 7, 7, 0, 7, 7, 0, 4, 6, 7, 0 } ;

/*==============================================================*/
static void set03_07_part_move( PLAYER_WORK *ew )
{
	if( set03_07_on_tbl[set03_07_row_tbl[ew->timer0]][ew->type] ) ew->obj = obj_stg->obj[31 + ew->type] ;
	else ew->obj = obj_stg->obj[34 + ew->type] ;
	if( ++ ew->timer1 >= 10 ){
		ew->timer1 = 0 ;
		if( ++ ew->timer0 >= sizeof( set03_07_row_tbl ) / sizeof( set03_07_row_tbl[0] ) ) ew->timer0 = 0 ;
	}
}

/****************************************************************/
void set03_07_part_set( PLAYER_WORK *pl_w, int type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set03_07_part_move ;
		ew->obj_flag = OBJ_BRIGHT ;
		ew->base_mtx = &pl_w->obj_mtx ;
		ew->type = type ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_set03.c" End of File	*/

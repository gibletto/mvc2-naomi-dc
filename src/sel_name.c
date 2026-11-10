/* The select scene's name plate works (kind 5), after the backdrop module. */
#include "player.h"
#include "select.h"

extern void move_set_x( PLAYER_WORK *wk, Sint32 time, Float x, Sint8 x_type );
extern void sel_dummy_1( PLAYER_WORK *w );

/* where the name plate stands for each column the cursor can be in */
static VEC3 sel_name_pos_tbl[2][3] = {
	{
		{ -165.0f, 120.0f, 240.0f },
		{ -145.0f, 120.0f, 240.0f },
		{ -145.0f, 120.0f, 240.0f },
	},
	{
		{ 165.0f, 120.0f, 240.0f },
		{ 145.0f, 120.0f, 240.0f },
		{ 142.0f, 120.0f, 240.0f },
	},
} ;

static void sel_name_main( PLAYER_WORK *ew );

/****************************************************************/
/*			A plate for the side of pl_w (the cursor's);		*/
/*			flag sets it going at once							*/
/****************************************************************/
PLAYER_WORK *sel_name_set( PLAYER_WORK *pl_w, Uint8 flag )
{
	PLAYER_WORK *ew ;

	if( flag && Sys->stage_cnt == 7 ) return ;
	if( !( ew = work_get( 0, W_SET, LIST_END ) ) ) return ;
	ew->disp_flag = 1 ;
	ew->master_w = pl_w ;
	ew->type = pl_w->pl_index ;
	ew->sub_type = pl_w->timer1 ;
	if( pl_w->pl_index ){
		ew->rot[1] = 0x8000 ;
		ew->obj = obj_dm01->obj[178] ;
	}
	else{
		ew->rot[1] = 0 ;
		ew->obj = obj_dm01->obj[177] ;
	}
	if( sel_w.join_flag == 3 ) ew->pos = sel_name_pos_tbl[pl_w->pl_index][0] ;
	else ew->pos = sel_name_pos_tbl[pl_w->pl_index][sel_w.join_flag] ;
	ew->obj_flag = OBJ_POS | OBJ_BRIGHT ;
	ew->work3 = sel_w.join_flag ;
	if( flag ) ew->work_func = sel_dummy_1 ;
	else ew->work_func = sel_name_main ;
	return ew ;
}

static void sel_name_in( PLAYER_WORK *ew );
static void sel_name_out( PLAYER_WORK *ew );
static void sel_name_free( PLAYER_WORK *ew );

/*==============================================================*/
static void sel_name_main( PLAYER_WORK *ew )
{
	static void ( *ef05_plate_jmp[] )( PLAYER_WORK *ew ) = {
		sel_name_in, sel_name_out, sel_name_free,
	} ;

	ef05_plate_jmp[ew->routine_0]( ew ) ;
}

static void sel_name_move( PLAYER_WORK *ew );

/*==============================================================*/
static void sel_name_in( PLAYER_WORK *ew )
{
	sel_name_move( ew ) ;
	if( sel_w.pick_flag[ew->type] & ( 1 << ew->sub_type ) ) ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void sel_name_out( PLAYER_WORK *ew )
{
	sel_name_move( ew ) ;
	if( sel_w.pick_flag[ew->type] & ( 16 << ew->sub_type ) ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void sel_name_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			The plate slides to the column the side's cursor	*/
/*			moved to											*/
/*==============================================================*/
static void sel_name_move( PLAYER_WORK *ew )
{
	Uint8 chg ;
	SEL_WORK *sw = &sel_w ;

	if( sw->cur_go_flag ) return ;
	switch( ew->routine_2 ){
	case 0:
		chg = ew->work3 ^ sw->join_flag ;
		if( chg ){
			ew->routine_2 ++ ;
			ew->timer1 = 30 ;
			if( sw->join_flag == 3 ) move_set_x( ew, ew->timer1, sel_name_pos_tbl[ew->type][0].x, 0 ) ;
			else move_set_x( ew, ew->timer1, sel_name_pos_tbl[ew->type][chg].x, 0 ) ;
			ew->work3 |= chg ;
		}
		break ;
	case 1:
		ew->timer1 -- ;
		if( ew->timer1 <= 0 ) ew->routine_2 = 0 ;
		else ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		break ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "sel_name.c" End of File	*/

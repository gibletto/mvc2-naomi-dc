/* The select scene's cursor and its pieces (kind 5 works). */
#include "player.h"
#include "select.h"

extern void move_set_z( PLAYER_WORK *wk, Sint32 time, Float z, Sint8 z_type );
extern void move_set_x( PLAYER_WORK *wk, Sint32 time, Float x, Sint8 x_type );
extern void move_stop_y( PLAYER_WORK *wk, Sint32 time );
extern void move_z_add( PLAYER_WORK *wk );

/* where the three pieces of each side's cursor stand, and the turns of its model */
static VEC3 ef05_cur_pos_tbl[2][3][3] = {
	{
		{
			{ -300.0f, 260.0f, -50.0f },
			{ -300.0f, 260.0f, -245.0f },
			{ -300.0f, 260.0f, -450.0f },
		},
		{
			{ -280.0f, 260.0f, -50.0f },
			{ -260.0f, 260.0f, -245.0f },
			{ -240.0f, 260.0f, -450.0f },
		},
		{
			{ -280.0f, 260.0f, -50.0f },
			{ -260.0f, 260.0f, -245.0f },
			{ -240.0f, 260.0f, -450.0f },
		},
	},
	{
		{
			{ 300.0f, 260.0f, -50.0f },
			{ 300.0f, 260.0f, -245.0f },
			{ 300.0f, 260.0f, -450.0f },
		},
		{
			{ 280.0f, 260.0f, -50.0f },
			{ 260.0f, 260.0f, -245.0f },
			{ 240.0f, 260.0f, -450.0f },
		},
		{
			{ 280.0f, 260.0f, -50.0f },
			{ 260.0f, 260.0f, -245.0f },
			{ 240.0f, 260.0f, -450.0f },
		},
	},
} ;

static Sint32 ef05_cur_rot_tbl[2][3] = {
	{ 0, 14564, 1092 },
	{ 0, 50973, 64445 },
} ;

static void ef05_cur_main( PLAYER_WORK *ew );
static void ef05_cur_boss_main( PLAYER_WORK *ew );
void ef05_cur_piece_main( PLAYER_WORK *ew );
void ef05_cur_boss_piece_main( PLAYER_WORK *ew );

/****************************************************************/
/*			A side's cursor: it starts hidden and is shown		*/
/*			by the step it is given								*/
/****************************************************************/
void ef05_cur_make( Uint8 side, int a )
{
	PLAYER_WORK *ew ;

	if( !( ew = work_get( 0, W_SET, LIST_END ) ) ) return ;
	ew->disp_flag = 0 ;
	ew->work_func = ef05_cur_main ;
	ew->type = side ;
	ew->timer0 = 0 ;
	ew->sub_type = 2 ;
	ew->work3 = 0 ;
	ew->work2 = a ;
}

/****************************************************************/
void ef05_cur_boss_make( Uint8 side )
{
	PLAYER_WORK *ew ;

	if( !( ew = work_get( 0, W_SET, LIST_END ) ) ) return ;
	ew->disp_flag = 0 ;
	ew->work_func = ef05_cur_boss_main ;
	ew->type = side ;
	ew->timer0 = 0 ;
	ew->sub_type = 0 ;
	ew->work3 = 0 ;
}

/*==============================================================*/
/*			One of the cursor's three pieces for the master,	*/
/*			made at its place									*/
/*==============================================================*/
static void ef05_cur_set( PLAYER_WORK *pl_w, Uint8 flag )
{
	PLAYER_WORK *ew ;
	Sint8 no ;

	if( !( ew = work_get( 0, W_SET, LIST_END ) ) ) return ;
	ew->disp_flag = 1 ;
	ew->type = pl_w->type ;
	ew->sub_type = pl_w->sub_type ;
	ew->work1 = pl_w->sub_type ;
	ew->master_w = pl_w ;
	ew->ef_w = &pl_work[ew->sub_type * 2 + ew->type] ;
	ew->obj = obj_dm01->obj[ew->type * 3 + ew->sub_type + 70] ;
	ew->pos = ef05_cur_pos_tbl[ew->type][0][ew->sub_type] ;
	if( flag ) ew->work_func = ef05_cur_boss_piece_main ;
	else{
		if( pl_w->work2 || sel_w.pick_flag[ew->type] == 119 ){
			no = ew->sub_type - 2 ;
			no = -no ;
			ew->work_func = ef05_cur_boss_piece_main ;
			ew->pos = ef05_cur_pos_tbl[ew->type][0][no] ;
		}
		ew->work_func = ef05_cur_piece_main ;
	}
	ew->rot[0] = ef05_cur_rot_tbl[ew->type][0] ;
	ew->rot[1] = ef05_cur_rot_tbl[ew->type][1] ;
	ew->rot[2] = ef05_cur_rot_tbl[ew->type][2] ;
	ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_ROT_Z | OBJ_SCALE | OBJ_BRIGHT ;
	ew->scale.x = 0.1f ;
	ew->scale.y = 1.0f ;
	ew->scale.z = 1.0f ;
	ew->work3 = 0 ;
	ew->sub_no = 0 ;
}

static void ef05_cur_pop( PLAYER_WORK *ew );
static void ef05_cur_step( PLAYER_WORK *ew );
static void ef05_cur_free( PLAYER_WORK *ew );

/*==============================================================*/
static void ef05_cur_main( PLAYER_WORK *ew )
{
	static void ( *ef05_cur_jmp[] )( PLAYER_WORK *ew ) = {
		ef05_cur_pop, ef05_cur_pop, ef05_cur_pop, ef05_cur_step, ef05_cur_free,
	} ;

	ef05_cur_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
/*			Every five frames one more piece of the cursor,		*/
/*			from the last to the first							*/
/*==============================================================*/
static void ef05_cur_pop( PLAYER_WORK *ew )
{
	ew->timer0 -- ;
	if( ew->timer0 <= 0 ){
		ew->routine_0 ++ ;
		ew->timer0 = 5 ;
		ef05_cur_set( ew, 0 ) ;
		ew->sub_type -- ;
	}
}

static void ef05_cur_wait( PLAYER_WORK *ew );
static void ef05_cur_pick0( PLAYER_WORK *ew );
static void ef05_cur_done( PLAYER_WORK *ew );
static void ef05_cur_pick1( PLAYER_WORK *ew );
static void ef05_cur_pick2( PLAYER_WORK *ew );
static void ef05_cur_last( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void ef05_cur_step( PLAYER_WORK *ew )
{
	static void ( *ef05_cur_step_jmp[] )( PLAYER_WORK *ew ) = {
		ef05_cur_wait, ef05_cur_pick0, ef05_cur_done, ef05_cur_pick1, ef05_cur_done, ef05_cur_pick2, ef05_cur_last,
	} ;

	ef05_cur_step_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void ef05_cur_wait( PLAYER_WORK *ew )
{
	if( sel_w.pick_flag[ew->type] == 119 ) ew->routine_1 = 6 ;
	else ew->routine_1 ++ ;
}

/*==============================================================*/
/*			The first piece of the cursor goes when its side	*/
/*			has chosen (bit 4 of unk_02)						*/
/*==============================================================*/
static void ef05_cur_pick0( PLAYER_WORK *ew )
{
	if( pl_work[ew->type].timer1 == 0 ){
		if( sel_w.pick_flag[ew->type] & 16 ){
			ew->routine_1 ++ ;
			ew->dir = 0 ;
			sel_w.cur_go_flag |= 1 << ew->type ;
		}
	}
}

/*==============================================================*/
/*			When its pieces have all gone (dir 7) the side's	*/
/*			step is 3											*/
/*==============================================================*/
static void ef05_cur_done( PLAYER_WORK *ew )
{
	if( ew->dir == 7 ){
		ew->routine_1 ++ ;
		sel_w.work_step[ew->type] = 3 ;
		if( ew->type ) sel_w.cur_go_flag &= 13 ;
		else sel_w.cur_go_flag &= 14 ;
	}
}

/*--------------------------------------------------------------*/
static void ef05_cur_pick1( PLAYER_WORK *ew )
{
	if( pl_work[ew->type].timer1 == 1 ){
		if( sel_w.pick_flag[ew->type] & 32 ){
			ew->routine_1 ++ ;
			ew->dir = 0 ;
			sel_w.cur_go_flag |= 1 << ew->type ;
		}
	}
}

/*--------------------------------------------------------------*/
static void ef05_cur_pick2( PLAYER_WORK *ew )
{
	if( pl_work[ew->type].timer1 == 2 ){
		if( sel_w.pick_flag[ew->type] & 64 ){
			ew->routine_1 ++ ;
			ew->dir = 0 ;
			sel_w.cur_go_flag |= 1 << ew->type ;
		}
	}
}

/*--------------------------------------------------------------*/
static void ef05_cur_last( PLAYER_WORK *ew )
{
	if( ew->dir == 7 ){
		sel_w.work_step[ew->type] = 3 ;
		ew->routine_0 = 4 ;
		if( ew->type ) sel_w.cur_go_flag &= 13 ;
		else sel_w.cur_go_flag &= 14 ;
	}
}

/*--------------------------------------------------------------*/
static void ef05_cur_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

static void ef05_cur_boss_set( PLAYER_WORK *ew );
static void ef05_cur_boss_wait( PLAYER_WORK *ew );

/*==============================================================*/
static void ef05_cur_boss_main( PLAYER_WORK *ew )
{
	static void ( *ef05_cur2_jmp[] )( PLAYER_WORK *ew ) = {
		ef05_cur_boss_set, ef05_cur_boss_wait, ef05_cur_free,
	} ;

	ef05_cur2_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void ef05_cur_boss_set( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ef05_cur_set( ew, 1 ) ;
}

/*--------------------------------------------------------------*/
static void ef05_cur_boss_wait( PLAYER_WORK *ew )
{
	if( ew->dir ){
		ew->routine_0 ++ ;
		sel_w.work_step[ew->type] = 3 ;
	}
}

void sel_face_set( PLAYER_WORK *pl_w );
void sel_pic_set( PLAYER_WORK *pl_w );
static void ef05_cur_grow( PLAYER_WORK *ew );
static void ef05_cur_open( PLAYER_WORK *ew );
static void ef05_cur_ctl( PLAYER_WORK *ew );
static void ef05_cur_slide_in( PLAYER_WORK *ew );

/****************************************************************/
/*			The work_func of a cursor piece: it grows, takes	*/
/*			its place, and goes by its master's step			*/
/****************************************************************/
void ef05_cur_piece_main( PLAYER_WORK *ew )
{
	static void ( *ef05_cur_piece_jmp[] )( PLAYER_WORK *ew ) = {
		ef05_cur_grow, ef05_cur_open, ef05_cur_ctl, ef05_cur_slide_in,
	} ;

	ef05_cur_piece_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void ef05_cur_grow( PLAYER_WORK *ew )
{
	ew->scale.x += 0.1f ;
	if( ew->scale.x >= 1.0f ){
		ew->routine_0 ++ ;
		if( ew->sub_type == 0 ){
			sel_w.work_step[ew->type] = 0 ;
			na_req( 57 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void ef05_cur_open( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	sel_face_set( ew ) ;
	sel_pic_set( ew ) ;
}

static void ef05_cur_ctl_idle( PLAYER_WORK *ew );
static void ef05_cur_ctl_in( PLAYER_WORK *ew );
static void ef05_cur_ctl_out( PLAYER_WORK *ew );
static void ef05_cur_ctl_end( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void ef05_cur_ctl( PLAYER_WORK *ew )
{
	static void ( *ef05_cur_ctl_jmp[] )( PLAYER_WORK *ew ) = {
		ef05_cur_ctl_idle, ef05_cur_ctl_idle, ef05_cur_ctl_in, ef05_cur_ctl_idle,
		ef05_cur_ctl_out, ef05_cur_ctl_idle, ef05_cur_ctl_end,
	} ;

	ef05_cur_ctl_jmp[ew->master_w->routine_1]( ew ) ;
}

static void ef05_cur_slide( PLAYER_WORK *ew );

/*==============================================================*/
static void ef05_cur_ctl_idle( PLAYER_WORK *ew )
{
	ew->routine_1 = 0 ;
	ef05_cur_slide( ew ) ;
}

/*==============================================================*/
/*			The piece leaves for its place on the master's		*/
/*			side												*/
/*==============================================================*/
static void ef05_cur_ctl_in( PLAYER_WORK *ew )
{
	Uint8 no ;

	switch( ew->routine_1 ){
	case 0:
		ew->routine_1 ++ ;
		ew->timer0 = 48 ;
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		if( sel_w.join_flag == 3 ) no = 0 ;
		else no = ew->work3 ;
		ew->routine_2 = 0 ;
		switch( ew->sub_type ){
		case 0:
			na_req( 58 + ew->type ) ;
			move_set_x( ew, ew->timer0, ef05_cur_pos_tbl[ew->type][no][2].x, 2 ) ;
			move_set_z( ew, ew->timer0, ef05_cur_pos_tbl[ew->type][no][2].z, 2 ) ;
			ew->acc.y = 1.2053571f ;
			move_stop_y( ew, ew->timer0 ) ;
			ew->work1 = 2 ;
			break ;
		case 1:
			move_set_x( ew, ew->timer0, ef05_cur_pos_tbl[ew->type][no][0].x, 2 ) ;
			move_set_z( ew, ew->timer0, ef05_cur_pos_tbl[ew->type][no][0].z, 2 ) ;
			ew->work1 = 0 ;
			break ;
		case 2:
			move_set_x( ew, ew->timer0, ef05_cur_pos_tbl[ew->type][no][1].x, 2 ) ;
			move_set_z( ew, ew->timer0, ef05_cur_pos_tbl[ew->type][no][1].z, 2 ) ;
			ew->work1 = 1 ;
			break ;
		}
		break ;
	case 1:
		ew->timer0 -- ;
		if( ew->timer0 < 0 ){
			ew->routine_1 ++ ;
			ew->master_w->dir |= 1 << ew->sub_type ;
		}
		else{
			ew->pos.x += ew->spd.x ;
			ew->spd.x += ew->acc.x ;
			ew->pos.y += ew->spd.y ;
			ew->spd.y += ew->acc.y ;
			move_z_add( ew ) ;
		}
		break ;
	case 2:
		break ;
	}
}

/*==============================================================*/
/*			The same, back to the place it came from			*/
/*==============================================================*/
static void ef05_cur_ctl_out( PLAYER_WORK *ew )
{
	Uint8 no ;

	switch( ew->routine_1 ){
	case 0:
		ew->routine_1 ++ ;
		ew->timer0 = 48 ;
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		if( sel_w.join_flag == 3 ) no = 0 ;
		else no = ew->work3 ;
		ew->routine_2 = 0 ;
		switch( ew->sub_type ){
		case 0:
			ew->acc.z = 0.0f ;
			ew->spd.z = 0.0f ;
			ew->work1 = 2 ;
			move_set_x( ew, ew->timer0, ef05_cur_pos_tbl[ew->type][no][2].x, 2 ) ;
			move_set_z( ew, ew->timer0, ef05_cur_pos_tbl[ew->type][no][2].z, 2 ) ;
			break ;
		case 1:
			na_req( 58 + ew->type ) ;
			move_set_x( ew, ew->timer0, ef05_cur_pos_tbl[ew->type][no][1].x, 2 ) ;
			move_set_z( ew, ew->timer0, ef05_cur_pos_tbl[ew->type][no][1].z, 2 ) ;
			ew->acc.y = 1.2053571f ;
			move_stop_y( ew, ew->timer0 ) ;
			ew->work1 = 1 ;
			break ;
		case 2:
			move_set_x( ew, ew->timer0, ef05_cur_pos_tbl[ew->type][no][0].x, 2 ) ;
			move_set_z( ew, ew->timer0, ef05_cur_pos_tbl[ew->type][no][0].z, 2 ) ;
			ew->work1 = 0 ;
			break ;
		}
		break ;
	case 1:
		ew->timer0 -- ;
		if( ew->timer0 < 0 ){
			ew->routine_1 ++ ;
			ew->master_w->dir |= 1 << ew->sub_type ;
		}
		else{
			ew->pos.x += ew->spd.x ;
			ew->spd.x += ew->acc.x ;
			ew->pos.y += ew->spd.y ;
			ew->spd.y += ew->acc.y ;
			move_z_add( ew ) ;
		}
		break ;
	case 2:
		break ;
	}
}

/*--------------------------------------------------------------*/
static void ef05_cur_ctl_end( PLAYER_WORK *ew )
{
	switch( ew->routine_1 ){
	case 0:
		ew->routine_1 ++ ;
		ew->timer0 = 58 ;
		ew->routine_2 = 0 ;
		break ;
	case 1:
		ew->timer0 -- ;
		if( ew->timer0 < 0 ){
			ew->routine_1 ++ ;
			ew->routine_0 ++ ;
			ew->master_w->dir |= 1 << ew->sub_type ;
		}
		break ;
	case 2:
		ef05_cur_slide( ew ) ;
		break ;
	}
}

/*--------------------------------------------------------------*/
static void ef05_cur_slide_in( PLAYER_WORK *ew )
{
	ef05_cur_slide( ew ) ;
}

static void ef05_cur_slide_set( PLAYER_WORK *ew );

/****************************************************************/
/*			The work_func of the cursor pieces ef05_cur_set		*/
/*			makes for the second sort							*/
/****************************************************************/
void ef05_cur_boss_piece_main( PLAYER_WORK *ew )
{
	static void ( *ef05_cur_piece2_jmp[] )( PLAYER_WORK *ew ) = {
		ef05_cur_grow, ef05_cur_slide_set, ef05_cur_ctl, ef05_cur_slide_in,
	} ;

	ef05_cur_piece2_jmp[ew->routine_0]( ew ) ;
}

/*--------------------------------------------------------------*/
static void ef05_cur_slide_set( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	sel_face_set( ew ) ;
}

/*==============================================================*/
/*			The piece slides to the column its side's cursor	*/
/*			moved to											*/
/*==============================================================*/
static void ef05_cur_slide( PLAYER_WORK *ew )
{
	Uint8 chg ;

	if( sel_w.cur_go_flag ) return ;
	switch( ew->routine_2 ){
	case 0:
		chg = ew->work3 ^ sel_w.join_flag ;
		if( chg ){
			ew->routine_2 ++ ;
			ew->timer1 = 30 ;
			if( sel_w.join_flag == 3 ) move_set_x( ew, ew->timer1, ef05_cur_pos_tbl[ew->type][0][ew->work1].x, 0 ) ;
			else move_set_x( ew, ew->timer1, ef05_cur_pos_tbl[ew->type][chg][ew->work1].x, 0 ) ;
			ew->work3 |= chg ;
		}
		break ;
	case 1:
		ew->timer1 -- ;
		if( ew->timer1 <= 0 ){
			ew->routine_1 = 0 ;
			ew->routine_2 = 0 ;
		}
		else ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		break ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "ef05_cur.c" End of File	*/

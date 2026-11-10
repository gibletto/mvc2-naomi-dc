/* The select scene's opening (kind 5 works): the big piece shrinks and turns, the arc of small pieces and the three
   triangles come in round it, and the pieces fade out at the end. */
#include "player.h"
#include "select.h"

extern void move_set_xy( PLAYER_WORK *wk, Sint32 time, Float x, Float y, Sint8 x_type, Sint8 y_type );
extern void move_set_z( PLAYER_WORK *wk, Sint32 time, Float z, Sint8 z_type );
extern void move_z_add( PLAYER_WORK *wk );

static void sel_open_main( PLAYER_WORK *ew );

/****************************************************************/
void sel_open_make( void )
{
	PLAYER_WORK *ew ;

	if( !( ew = work_get( 0, W_SET, LIST_END ) ) ) return ;
	ew->disp_flag = 1 ;
	ew->base_mtx = &sel_back_w->obj_mtx ;
	ew->work_func = sel_open_main ;
	ew->scale.x = 1.4f ;
	ew->scale.y = 1.4f ;
	ew->scale.z = 1.4f ;
	ew->fade.trnsl = 1.0f ;
	ew->obj = obj_dm01->obj[88] ;
	ew->rot[1] = 0xc71c ;
	ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_SCALE | OBJ_TRNSL | OBJ_BRIGHT ;
	na_req( 50 ) ;
}

/* where the arc's pieces stand */
static VEC3 sel_arc_pos_tbl[8] = {
	{ 1.8f, 0.0f, -215.0f },
	{ -73.6f, 0.0f, -202.0f },
	{ -138.2f, 0.0f, -164.7f },
	{ -186.2f, 0.0f, -107.5f },
	{ -211.7f, 0.0f, -37.3f },
	{ -211.7f, 0.0f, 37.3f },
	{ -186.2f, 0.0f, 107.5f },
	{ 0.0f, 0.0f, -215.0f },
} ;

static void sel_arc_main( PLAYER_WORK *ew );

/*==============================================================*/
/*			The arc of small pieces round the big one, or		*/
/*			the one piece when the computer's limit is at		*/
/*			its top												*/
/*==============================================================*/
static void sel_arc_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	Sint16 i ;
	Sint32 ang ;

	if( Sys->stage_cnt == 7 ){
		if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
			ew->disp_flag = 1 ;
			ew->master_w = pl_w ;
			ew->base_mtx = &pl_w->obj_mtx ;
			ew->work_func = sel_arc_main ;
			ew->type = 7 ;
			ew->pos = sel_arc_pos_tbl[7] ;
			ew->obj = obj_dm01->obj[97] ;
			ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_TRNSL | OBJ_BRIGHT ;
			ew->fade.trnsl = 1.0f ;
			ew->rot[1] = 0 ;
		}
	}
	else{
		ang = 0 ;
		for( i = 0 ; i < 7 ; i ++, ang += 0xe39 ){
			if( !( ew = work_get( 0, W_SET, LIST_END ) ) ) return ;
			ew->disp_flag = 1 ;
			ew->master_w = pl_w ;
			ew->base_mtx = &pl_w->obj_mtx ;
			ew->work_func = sel_arc_main ;
			ew->type = i ;
			ew->pos = sel_arc_pos_tbl[i] ;
			ew->obj = obj_dm01->obj[i + 90] ;
			ew->rot[1] = ang ;
			ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_TRNSL | OBJ_BRIGHT ;
			ew->fade.trnsl = 0.0f ;
		}
	}
}

/* where the three triangles come to rest */
static Float sel_tri_pos_tbl[3][2] = {
	{ 0.0f, 32.0f },
	{ -26.0f, -21.0f },
	{ 26.0f, -21.0f },
} ;

/* and how each is turned */
static Sint32 sel_tri_rot_tbl[3][2] = {
	{ 0xaab, 0 },
	{ 0x7555, 0xd556 },
	{ 0x7555, 0x2aab },
} ;

static void sel_tri_main( PLAYER_WORK *ew );
static void sel_piece_main( PLAYER_WORK *ew );

/*==============================================================*/
/*			The three triangles, flying in round a piece of		*/
/*			their own											*/
/*==============================================================*/
static void sel_tri_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	PLAYER_WORK *tw ;
	Sint8 i ;

	if( !( ew = work_get( 0, W_SET, LIST_END ) ) ) return ;
	ew->master_w = pl_w ;
	ew->work_func = sel_tri_main ;
	ew->pos.x = 0.0f ;
	ew->pos.y = 190.0f ;
	ew->pos.z = 0.0f ;
	ew->rot[0] = 0 ;
	ew->rot[1] = 0 ;
	ew->rot[2] = 0 ;
	ew->obj_flag = OBJ_POS | OBJ_ROT_Z | OBJ_BRIGHT ;
	ew->obj = obj_dm01->obj[130] ;
	ew->disp_flag = 0 ;
	for( i = 0 ; i < 3 ; i ++ ){
		if( !( tw = work_get( 0, W_SET, LIST_END ) ) ) return ;
		tw->master_w = pl_w ;
		tw->ef_w = ew ;
		tw->base_mtx = &ew->obj_mtx ;
		tw->disp_flag = 1 ;
		tw->work_func = sel_piece_main ;
		tw->type = i ;
		tw->pos.x = sel_tri_pos_tbl[i][0] * 5.0f ;
		if( i & 1 ) tw->pos.y = 0.0f ;
		else tw->pos.y = sel_tri_pos_tbl[i][1] * 2.0f ;
		tw->pos.z = 1150.0f ;
		tw->timer0 = 30 ;
		move_set_xy( tw, tw->timer0, sel_tri_pos_tbl[i][0], sel_tri_pos_tbl[i][1], 2, 2 ) ;
		move_set_z( tw, tw->timer0, 230.0f, 2 ) ;
		tw->rot[0] = sel_tri_rot_tbl[i][0] ;
		tw->rot[2] = sel_tri_rot_tbl[i][1] ;
		tw->rot[1] = 0 ;
		tw->fade.trnsl = 0.2f ;
		tw->obj = obj_dm01->obj[130] ;
		tw->timer1 = 4 ;
		tw->dir = 0 ;
		tw->sub_no = 0 ;
		tw->scale.x = 1.0f ;
		tw->scale.y = 1.0f ;
		tw->scale.z = 1.0f ;
		tw->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_ROT_Z | OBJ_SCALE | OBJ_TRNSL | OBJ_BRIGHT ;
	}
	na_req( 57 ) ;
}

static void sel_open_shrink( PLAYER_WORK *ew );
static void sel_open_turn( PLAYER_WORK *ew );
static void sel_open_wait( PLAYER_WORK *ew );
static void sel_open_slow( PLAYER_WORK *ew );
static void sel_open_fade( PLAYER_WORK *ew );
static void sel_open_end( PLAYER_WORK *ew );

/*==============================================================*/
static void sel_open_main( PLAYER_WORK *ew )
{
	static void ( *sel_open_jmp[] )( PLAYER_WORK *ew ) = {
		sel_open_shrink, sel_open_turn, sel_open_wait, sel_open_wait, sel_open_slow,
		sel_open_wait, sel_open_fade, sel_open_wait, sel_open_end,
	} ;

	sel_open_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
/*			The big piece shrinks to its size, and the arc		*/
/*			comes in											*/
/*==============================================================*/
static void sel_open_shrink( PLAYER_WORK *ew )
{
	ew->scale.x -= 0.02f ;
	ew->scale.y -= 0.02f ;
	ew->scale.z -= 0.02f ;
	if( ew->scale.x <= 1.0f ){
		ew->routine_0 ++ ;
		ew->scale.z = ew->scale.y = ew->scale.x = 1.0f ;
		sel_arc_set( ew ) ;
	}
}

/* where it turns to, by the computer's limit */
static Sint32 sel_open_rot_tbl[8] = {
	0x8000, 0x71c7, 0x638e, 0x5555, 0x471c, 0x38e4, 0x2aab, 0x8000,
} ;

/*--------------------------------------------------------------*/
static void sel_open_turn( PLAYER_WORK *ew )
{
	ew->rot[1] -= 0x100 ;
	ew->rot[1] &= 0xffff ;
	if( ew->rot[1] <= sel_open_rot_tbl[Sys->stage_cnt] ){
		ew->routine_0 ++ ;
		ew->rot[1] = sel_open_rot_tbl[Sys->stage_cnt] ;
		sel_tri_set( ew ) ;
		ew->timer0 = 90 ;
	}
}

/*--------------------------------------------------------------*/
static void sel_open_wait( PLAYER_WORK *ew )
{
	ew->timer0 -- ;
	if( ew->timer0 <= 0 ){
		ew->routine_0 ++ ;
		ew->timer0 = 40 ;
	}
	else if( ew->routine_0 == 5 ){
		ew->rot[1] -= 0x100 ;
		ew->rot[1] &= 0xffff ;
	}
}

/*--------------------------------------------------------------*/
static void sel_open_slow( PLAYER_WORK *ew )
{
	ew->rot[1] -= 0x100 ;
	ew->rot[1] &= 0xffff ;
	if( ew->rot[1] > 0 && ew->rot[1] < 0x71c ){
		ew->routine_0 ++ ;
		ew->timer0 = 60 ;
	}
}

/*--------------------------------------------------------------*/
static void sel_open_fade( PLAYER_WORK *ew )
{
	ew->rot[1] -= 0x100 ;
	ew->rot[1] &= 0xffff ;
	ew->scale.x += 0.05f ;
	ew->scale.y += 0.05f ;
	ew->fade.trnsl -= 0.05f ;
	if( ew->fade.trnsl <= 0.0f ){
		ew->routine_0 ++ ;
		ew->fade.trnsl = 0.0f ;
		ew->disp_flag = 0 ;
		ew->timer0 = 60 ;
	}
}

/*--------------------------------------------------------------*/
static void sel_open_end( PLAYER_WORK *ew )
{
	sel_w.ready_flag = 2 ;
	work_free( ew ) ;
}

static void sel_arc_off( PLAYER_WORK *ew );
static void sel_arc_on( PLAYER_WORK *ew );
static void sel_arc_hide( PLAYER_WORK *ew );
static void sel_arc_free( PLAYER_WORK *ew );

/*==============================================================*/
static void sel_arc_main( PLAYER_WORK *ew )
{
	static void ( *sel_arc_jmp[] )( PLAYER_WORK *ew ) = {
		sel_arc_off, sel_arc_on, sel_arc_hide, sel_arc_free,
	} ;

	sel_arc_jmp[ew->routine_0]( ew ) ;
}

static Uint8 sel_arc_ck( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A piece fades while the big piece's turn is not		*/
/*			at it												*/
/*==============================================================*/
static void sel_arc_off( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_0 >= 6 ) ew->routine_0 = 2 ;
	else if( sel_arc_ck( ew, pl_w ) ) ew->routine_0 = 1 ;
	else{
		ew->fade.trnsl -= 0.05f ;
		if( ew->fade.trnsl <= 0.0f ) ew->fade.trnsl = 0.0f ;
	}
}

/*==============================================================*/
/*			And comes back while it is							*/
/*==============================================================*/
static void sel_arc_on( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_0 >= 6 ) ew->routine_0 = 2 ;
	else if( !sel_arc_ck( ew, pl_w ) ) ew->routine_0 = 0 ;
	else{
		ew->fade.trnsl += 0.05f ;
		if( ew->fade.trnsl >= 1.0f ) ew->fade.trnsl = 1.0f ;
	}
}

/*--------------------------------------------------------------*/
static void sel_arc_hide( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void sel_arc_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			The piece's turn and the big piece's turn put it	*/
/*			in front											*/
/*==============================================================*/
static Uint8 sel_arc_ck( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float v = pl_w->rot[1] + ew->rot[1] ;

	if( v > 14564.0f && v < 50972.0f ) return 1 ;
	return 0 ;
}

/*==============================================================*/
/*			The hub of the triangles turns once the opening		*/
/*			is under way										*/
/*==============================================================*/
static void sel_tri_main( PLAYER_WORK *ew )
{
	switch( ew->routine_0 ){
	case 0:
		break ;
	case 1:
		ew->routine_0 ++ ;
	case 2:
		ew->rot[2] += 0x280 ;
		break ;
	}
}

static void sel_piece_in( PLAYER_WORK *ew );
static void sel_piece_anim( PLAYER_WORK *ew );
static void sel_piece_out( PLAYER_WORK *ew );
static void sel_piece_free( PLAYER_WORK *ew );

/*==============================================================*/
static void sel_piece_main( PLAYER_WORK *ew )
{
	static void ( *sel_piece_jmp[] )( PLAYER_WORK *ew ) = {
		sel_piece_in, sel_piece_anim, sel_piece_out, sel_piece_free,
	} ;

	sel_piece_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
/*			A triangle flies in and comes to rest				*/
/*==============================================================*/
static void sel_piece_in( PLAYER_WORK *ew )
{
	ew->fade.trnsl += 0.05f ;
	if( ew->fade.trnsl > 1.0f ) ew->fade.trnsl = 1.0f ;
	ew->timer0 -- ;
	if( ew->timer0 <= 0 ){
		ew->routine_0 ++ ;
		ew->pos.x = sel_tri_pos_tbl[ew->type][0] ;
		ew->pos.y = sel_tri_pos_tbl[ew->type][1] ;
		ew->pos.z = 230.0f ;
		ew->fade.trnsl = 1.0f ;
		if( !ew->type ) ew->ef_w->routine_0 = 1 ;
	}
	else{
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		move_z_add( ew ) ;
	}
}

/* the model of each picture of its turn */
static const Uint8 sel_piece_obj_tbl[6] = { 130, 131, 132, 133, 132, 131 } ;

/*--------------------------------------------------------------*/
static void sel_piece_anim( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_0 >= 3 ) ew->routine_0 ++ ;
	else{
		ew->timer1 -- ;
		if( ew->timer1 <= 0 ){
			ew->timer1 = 4 ;
			ew->dir ++ ;
			if( ew->dir > 5 ) ew->dir = 0 ;
			ew->obj = obj_dm01->obj[sel_piece_obj_tbl[ew->dir]] ;
		}
	}
}

/*--------------------------------------------------------------*/
static void sel_piece_out( PLAYER_WORK *ew )
{
	ew->fade.trnsl -= 0.05f ;
	if( ew->fade.trnsl <= 0.0f ){
		ew->routine_0 ++ ;
		ew->fade.trnsl = 0.0f ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void sel_piece_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "sel_open.c" End of File	*/

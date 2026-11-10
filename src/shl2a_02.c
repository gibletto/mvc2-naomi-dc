/* Storm's effect 02 (character 2a's shot 02): the bolts of her Lightning Storm. Eight start in a
   ring round her, and each runs a short way, then makes a second bolt that jumps off to a
   random place from a table. */
#include "player.h"
#include "pl_char.h"

/* where in the ring each bolt of the first eight starts, in front of Storm */
static Sint16 shl2a_02_ring_tbl[8][2] = {
	{ 0, 128 }, { 12, 112 }, { 24, 88 }, { 12, 60 }, { 0, 32 }, { -12, 60 }, { -24, 88 }, { -12, 112 },
} ;

/* where the second bolt jumps to (x, y), whether it turns about, and its animation; four to pick
   from for each bolt of the ring */
static Sint16 shl2a_02_jump_tbl[8][4][4] = {
	{ { 0, 72, 0, 11 }, { 0, 64, 0, 12 }, { 0, 72, 1, 11 }, { 0, 64, 1, 12 } },
	{ { 80, 20, 0, 5 }, { 34, 44, 0, 7 }, { 64, 52, 0, 8 }, { 80, 32, 0, 9 } },
	{ { 80, 8, 1, 6 }, { 80, 12, 1, 10 }, { 80, -8, 0, 29 }, { 80, -12, 0, 33 } },
	{ { 80, -20, 1, 28 }, { 34, -44, 1, 30 }, { 64, -52, 1, 31 }, { 80, -32, 1, 32 } },
	{ { 0, -72, 1, 34 }, { 0, -64, 1, 35 }, { 0, -72, 1, 34 }, { 0, -64, 1, 35 } },
	{ { -80, -20, 0, 28 }, { -34, -44, 0, 30 }, { -64, -52, 0, 31 }, { -80, -32, 0, 32 } },
	{ { -80, 8, 0, 6 }, { -80, 12, 0, 10 }, { -80, -8, 1, 29 }, { -80, -12, 1, 33 } },
	{ { -80, 20, 1, 5 }, { -34, 44, 1, 7 }, { -64, 52, 1, 8 }, { -80, 32, 1, 9 } },
} ;

static void shl2a_02_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl2a_02_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	Uint32 i ;

	for( i = 0 ; i < 8 ; i ++ ){
		if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
			ew->work_func = shl2a_02_main ;
			ew->master_w = pl_w ;
			ew->pos.x = pl_w->pos.x ;
			ew->pos.y = pl_w->pos.y ;
			ew->routine_3 = 0 ;
			ew->type = i ;
			ew->pl_type = pl_w->pl_type ;
			ew->ef_no = 0x2a02 ;
		}
	}
	return ew ;
}

/****************************************************************/
PLAYER_WORK *shl2a_02_set0( PLAYER_WORK *w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl2a_02_main ;
		ew->master_w = w->master_w ;
		ew->pos.x = w->pos.x ;
		ew->pos.y = w->pos.y ;
		ew->routine_3 = 1 ;
		ew->type = w->type ;
		ew->pl_type = w->pl_type ;
		ew->ef_no = 0x2a02 ;
	}
	return ew ;
}

static void shl2a_02_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2a_02_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2a_02_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl2a_02_main( PLAYER_WORK *ew )
{
	static void ( *shl2a_02_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2a_02_init, shl2a_02_move, shl2a_02_free, shl2a_02_free,
	} ;

	shl2a_02_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void shl2a_02_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Sint16 *jump ;
	Sint16 x, y ;

	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, pl_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = pl_w->pl_no ;
	ew->pl_type = pl_w->pl_type ;
	ew->scale.x = pl_w->scale.x ;
	ew->scale.y = pl_w->scale.y ;
	ew->atck_level = pl_w->atck_level ;
	ew->work_no = pl_w->work_no ;
	ew->col_cnt = pl_w->col_cnt ;
	ew->scale = pl_w->scale ;
	ew->prio = pl_w->prio ;
	ew->prio = 0 ;
	ew->atck_flag = HIT_ON ;
	ew->dm_flag = 0 ;
	ew->atck_no = ( Rnd() & 1 ) + 93 ;
	HIT_RESET( ew ) ;
	ew->obj_flag = Rnd() & 3 ;
	if( !ew->routine_3 ){
		x = shl2a_02_ring_tbl[ew->type][0] ;
		y = shl2a_02_ring_tbl[ew->type][1] ;
		if( ew->disp_dir ) x = -x ;
		ew->pos.x += DOT_X( x ) ;
		ew->pos.y += DOT_Y( y ) ;
	}
	jump = shl2a_02_jump_tbl[ew->type][ew->obj_flag] ;
	x = *jump ++ ;
	y = *jump ++ ;
	if( ew->disp_dir ) x = -x ;
	ew->pos.x += DOT_X( x ) ;
	ew->pos.y += DOT_Y( y ) ;
	ew->disp_dir ^= *jump ++ ;
	pl_char_set( ew, 21, *jump ) ;
	ew->size_top = ew->size_bottom = ew->size_front = ew->size_back = 64 ;
	if( !scr_in_x_ck( ew ) || !scr_in_y_ck( ew ) ) shl2a_02_free( ew, pl_w ) ;
	else hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl2a_02_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ew->pl_type != pl_w->pl_type ){
	} else if( ew->hit_flag ){
	} else {
		if( pl_char_move( ew ) >= 0 ) return ;
		shl2a_02_set0( ew ) ;
	}
	shl2a_02_free( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl2a_02_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl2a_02.c" End of File	*/

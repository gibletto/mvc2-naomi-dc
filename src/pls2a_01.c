/* Storm's player effect 01: a copy of her sprite, by type and side, that shows an animation
   of group 27 at her place and ends with her animation. */
#include "player.h"
#include "ef_char.h"

static void pls2a_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls2a_01_set( PLAYER_WORK *pl_w, Uint8 no, Uint8 side )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls2a_01_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->sub_type = side ;
		ew->ef_no = 0x2a01 ;
	}
	return ew ;
}

static void pls2a_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2a_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2a_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2a_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2a_01_main( PLAYER_WORK *ew )
{
	static void ( *pls2a_01_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2a_01_init, pls2a_01_move, pls2a_01_end, pls2a_01_free,
	} ;

	pls2a_01_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void pls2a_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	int no ;

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
	ew->sub_prio = -1 ;
	ew->obj_flag = pl_w->char_no.w ;
	if( !ew->type ){
		no = 48 ;
		if( ew->sub_type ) no = 49 ;
	} else {
		if( ew->sub_type ) ew->disp_dir ^= 1 ;
		no = 50 ;
	}
	ef_char_set( ew, 27, no ) ;
	pls2a_01_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls2a_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ew->obj_flag != pl_w->char_no.w || ew->pl_type != pl_w->pl_type ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->prio = pl_w->prio ;
	ew->pos.x = pl_w->pos.x ;
	if( !ew->type ){
		ew->pos.y = pl_w->ground_y ;
		ew->disp_dir = pl_w->disp_dir ;
	} else ew->pos.y = pl_w->pos.y ;
	if( ef_char_move( ew ) >= 0 ) return ;
	if( ew->type ) return ;
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls2a_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls2a_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls2a_01.c" End of File	*/

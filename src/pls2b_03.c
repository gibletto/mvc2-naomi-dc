/* Sabretooth's player effect 03: a string of twelve copies of his sprite behind one that comes
   in from the screen's corner, each following the one before it (kind 0 and 1 go opposite ways). */
#include "player.h"
#include "pl_char.h"
#include "pl2b_def.h"

static void pls2b_03_main( PLAYER_WORK *ew );
static void pls2b_03_trail( PLAYER_WORK *ew );

/****************************************************************/
int pls2b_03_set( PLAYER_WORK *pl_w, int no )
{
	PLAYER_WORK *ew, *cw ;
	int i ;

	if( work_rest_cnt <= 13 ) return 0 ;
	ew = work_get( 0, W_PLS, LIST_END ) ;
	ew->ef_no = 0x2b03 ;
	ew->sub_type = no ;
	ew->master_w = pl_w ;
	ew->work_func = pls2b_03_main ;
	for( i = 0 ; i < 6 ; i ++ ){
		cw = work_get( 0, W_PLS, LIST_END ) ;
		cw->ef_no = 0x2b03 ;
		cw->sub_no = i ;
		cw->type = 0 ;
		cw->sub_type = no ;
		cw->ef_w = ew ;
		cw->work_func = pls2b_03_trail ;
	}
	for( i = 0 ; i < 6 ; i ++ ){
		cw = work_get( 0, W_PLS, LIST_END ) ;
		cw->ef_no = 0x2b03 ;
		cw->sub_no = i ;
		cw->type = 1 ;
		cw->sub_type = no ;
		cw->ef_w = ew ;
		cw->work_func = pls2b_03_trail ;
	}
	return 1 ;
}

static void pls2b_03_init( PLAYER_WORK *ew );
static void pls2b_03_move( PLAYER_WORK *ew );
static void pls2b_03_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls2b_03_main( PLAYER_WORK *ew )
{
	static void ( *pls2b_03_jmp[] )( PLAYER_WORK *ew ) = {
		pls2b_03_init, pls2b_03_move, pls2b_03_free,
	} ;

	pls2b_03_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
/*			The first work makes up all thirteen, which the		*/
/*			pool gave in a row									*/
/*==============================================================*/
static void pls2b_03_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *cw = ew ;
	int i ;

	for( i = 0 ; i < 13 ; i ++ ){
		CHAR_PART_COPY( cw, pl_w ) ;
		cw->disp_flag = 1 ;
		cw->pl_no = pl_w->pl_no ;
		cw->pl_type = pl_w->pl_type ;
		cw->scale.x = pl_w->scale.x ;
		cw->scale.y = pl_w->scale.y ;
		cw->atck_level = pl_w->atck_level ;
		cw->work_no = pl_w->work_no ;
		cw->col_cnt = pl_w->col_cnt ;
		cw->scale = pl_w->scale ;
		cw->prio = pl_w->prio ;
		cw = cw->behind_w ;
	}
	ew->routine_0 ++ ;
	ew->disp_dir = ew->sub_type ;
	ew->pos.x = scr_work.scr_left + 320.0f ;
	ew->pos.y = scr_work.scr_bottom + 240.0f ;
	pl_char_set( ew, Char_Effect, 20 ) ;
}

/*--------------------------------------------------------------*/
static void pls2b_03_move( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void pls2b_03_free( PLAYER_WORK *ew )
{
	PLAYER_WORK *cw = ew->behind_w ;
	int i ;

	for( i = 0 ; i < 12 ; i ++ ){
		work_free( cw ) ;
		cw = cw->behind_w ;
	}
	work_free( ew ) ;
}

/*==============================================================*/
/*			A copy takes the animation of the one before it		*/
/*			(the first, of the head) and stands apart			*/
/*==============================================================*/
static void pls2b_03_trail( PLAYER_WORK *ew )
{
	PLAYER_WORK *bw ;
	Float x, y ;

	if( !ew->sub_no ) bw = ew->ef_w ;
	else bw = ew->before_w ;
	ew->disp_dir = ew->sub_type ;
	ew->disp_flag = bw->disp_flag ;
	ew->spr_no = bw->spr_no ;
	ew->pos = bw->pos ;
	x = DOT_X( 31.0f ) ;
	y = -DOT_Y( 16.0f ) ;
	if( ew->type ){
		x = -DOT_X( 31.0f ) ;
		y = DOT_Y( 16.0f ) ;
	}
	if( ew->disp_dir ) x = -x ;
	ew->pos.x += x ;
	ew->pos.y += y ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls2b_03.c" End of File	*/

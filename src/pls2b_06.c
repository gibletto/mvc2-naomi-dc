/* Sabretooth's player effect 06: the figure that comes to him in two of his win poses and in one
   entrance (type 0 comes in from off the screen, brakes and stays; 1 runs in and stops beside him;
   2 waits beside him as he appears, then runs off the screen). */
#include "player.h"
#include "pl_char.h"
#include "pl2b_def.h"

static void pls2b_06_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls2b_06_set( PLAYER_WORK *pl_w, int type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_END ) ) ){
		ew->be_flag = 1 ;
		ew->ef_no = 0x2b06 ;
		ew->type = type ;
		ew->work_func = pls2b_06_main ;
		ew->master_w = pl_w ;
	}
	return ew ;
}

static void pls2b_06_win_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2b_06_win_side( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2b_06_appear( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2b_06_main( PLAYER_WORK *ew )
{
	static void ( *pls2b_06_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2b_06_win_run, pls2b_06_win_side, pls2b_06_appear,
	} ;

	pls2b_06_jmp[ew->type]( ew, ew->master_w ) ;
}

static void pls2b_06_win_run_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2b_06_win_run_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2b_06_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2b_06_win_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls2b_06_win_run_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2b_06_win_run_init, pls2b_06_win_run_move, pls2b_06_free,
	} ;

	pls2b_06_win_run_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls2b_06_win_run_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float x ;

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
	ew->routine_0 ++ ;
	ew->prio = 12 ;
	ew->pos.y = pl_w->ground_y ;
	ew->timer0 = 32 ;
	x = scr_work.scr_right + 480.0f ;
	ew->spd.x = -DOT_X( 10.0f ) ;
	if( pl_w->pl_dir ){
		x = scr_work.scr_left + -DOT_X( 320.0f ) ;
		ew->spd.x = -ew->spd.x ;
	}
	ew->pos.x = x ;
	pl_char_set( ew, Char_Effect, 26 ) ;
}

/*--------------------------------------------------------------*/
static void pls2b_06_win_run_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	switch( ew->routine_1 ){
	case 0 :
		pl_char_move( ew ) ;
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		if( !-- ew->timer0 ){
			ew->routine_1 ++ ;
			ew->acc.x = -( ew->spd.x / 4.0f ) ;
			pl_char_set( ew, Char_Effect, 27 ) ;
		}
		break ;
	case 1 :
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		if( ew->spd.x * ew->acc.x >= 0.0f ){
			ew->routine_1 ++ ;
			ew->spd.x = 0.0f ;
			ew->acc.x = 0.0f ;
		}
		break ;
	case 2 :
		if( pl_char_move( ew ) < 0 ) ew->routine_1 ++ ;
		break ;
	}
}

static void pls2b_06_win_side_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2b_06_win_side_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2b_06_win_side( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls2b_06_win_side_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2b_06_win_side_init, pls2b_06_win_side_move,
	} ;

	pls2b_06_win_side_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls2b_06_win_side_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float x ;

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
	ew->routine_0 ++ ;
	ew->prio = 11 ;
	ew->pos.y = pl_w->ground_y ;
	x = scr_work.scr_right + DOT_X( 64.0f ) ;
	ew->spd.x = -10.0f ;
	if( pl_w->pl_dir ){
		x = scr_work.scr_left + -DOT_X( 64.0f ) ;
		ew->spd.x = -ew->spd.x ;
	}
	ew->pos.x = x ;
	pl_shadow_set( ew ) ;
	pl_char_set( ew, Char_Effect, 28 ) ;
}

/*--------------------------------------------------------------*/
static void pls2b_06_win_side_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float x ;

	switch( ew->routine_1 ){
	case 0 :
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		pl_char_move( ew ) ;
		x = ew->pos.x - pl_w->pos.x ;
		if( x < 0.0f ) x = -x ;
		if( x < DOT_X( 64.0f ) ){
			ew->routine_1 ++ ;
			x = DOT_X( 64.0f ) ;
			if( pl_w->pl_dir ) x = -DOT_X( 64.0f ) ;
			ew->pos.x = pl_w->pos.x + x ;
			ew->spd.x = 0.0f ;
			ew->spd.y = 0.0f ;
			ew->acc.x = 0.0f ;
			ew->acc.y = 0.0f ;
			pl_char_set( ew, Char_Effect, 29 ) ;
		}
		break ;
	case 1 :
		if( !pl_w->cg_type ){
			pl_char_move( ew ) ;
			if( ew->cg_type ){
				ew->routine_1 ++ ;
				ew->cg_type = 0 ;
				pl_w->sub_type |= 0x80 ;
			}
		}
		break ;
	case 2 :
		pl_char_move( ew ) ;
		break ;
	}
}

static void pls2b_06_appear_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2b_06_appear_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2b_06_appear( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls2b_06_appear_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2b_06_appear_init, pls2b_06_appear_move,
	} ;

	pls2b_06_appear_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls2b_06_appear_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float x ;

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
	ew->routine_0 ++ ;
	ew->prio = 12 ;
	ew->size_front = 48 ;
	ew->size_back = 48 ;
	x = -60.0f ;
	ew->spd.x = 10.0f ;
	if( pl_w->pl_dir ){
		x = 60.0f ;
		ew->spd.x = -ew->spd.x ;
	}
	ew->pos.x = pl_w->pos.x + x ;
	ew->pos.y = pl_w->ground_y ;
	pl_shadow_set( ew ) ;
	pl_char_set( ew, Char_Effect, 30 ) ;
}

/*--------------------------------------------------------------*/
static void pls2b_06_appear_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	switch( ew->routine_1 ){
	case 0 :
		pl_char_move( ew ) ;
		if( !ew->cg_type ) break ;
		ew->routine_1 ++ ;
		ew->cg_type = 0 ;
	case 1 :
		if( !pl_w->cg_type ) break ;
		ew->routine_1 ++ ;
	case 2 :
		pl_char_move( ew ) ;
		if( !ew->cg_type ) break ;
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		if( !scr_in_x_ck( ew ) ){
			ew->routine_0 = 2 ;
			ew->routine_1 = 0 ;
			ew->disp_flag = 0 ;
		}
		break ;
	}
}

/*--------------------------------------------------------------*/
static void pls2b_06_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->routine_1 ) ew->routine_1 ++ ;
	else work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls2b_06.c" End of File	*/

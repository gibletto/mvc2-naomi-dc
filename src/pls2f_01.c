/* Silver Samurai's player effect 01: the mark that shows where his attack struck, in front of him; it
   floats up and ends when the struck box is gone or its animation is over. */
#include "player.h"
#include "ef_char.h"
#include "pl2f_def.h"

static void pls2f_01_main( PLAYER_WORK *ew );

/****************************************************************/
void *pls2f_01_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS_ST, LIST_TOP ) ) ){
		ew->work_func = pls2f_01_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x2f01 ;
	}
	return ew ;
}

static void pls2f_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2f_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2f_01_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2f_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2f_01_main( PLAYER_WORK *ew )
{
	static void ( *pls2f_01_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2f_01_init, pls2f_01_move, pls2f_01_wait, pls2f_01_free,
	} ;

	pls2f_01_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void pls2f_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Sint16 ofs ;

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
	ofs = 14 ;
	if( ew->disp_dir ) ofs = -14 ;
	ew->pos.x = pl_w->pos.x + DOT_X( ofs ) ;
	ew->pos.y = pl_w->pos.y + 330.0f ;
	ef_char_set( ew, 27, 4 ) ;
	pl_se_req( ew, 75 ) ;
	pls2f_01_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls2f_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->hit_box_no || ef_char_move( ew ) < 0 ){
		pls2f_01_free( ew, pl_w ) ;
	}
	else {
		ew->pos.y += DOT_Y( 0.5f ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls2f_01_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
}

/*--------------------------------------------------------------*/
static void pls2f_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls2f_01.c" End of File	*/

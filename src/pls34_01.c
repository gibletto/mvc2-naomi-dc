/* Sentinel's player effect 01: the copy of his sprite that flies off to the side in Hard Drive
   and lasts while his animation does. */
#include "player.h"
#include "pl_char.h"
#include "pl34_def.h"

static void pls34_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls34_01_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls34_01_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x3401 ;
	}
	return ew ;
}

static void pls34_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls34_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls34_01_next( PLAYER_WORK *ew );
static void pls34_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls34_01_main( PLAYER_WORK *ew )
{
	static void ( *pls34_01_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls34_01_init, pls34_01_move, pls34_01_next, pls34_01_free,
	} ;

	pls34_01_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void pls34_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
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
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->timer0 = pl_w->char_no.w ;
	ew->sub_prio = -2 ;
	ew->spd.x = 40.0f ;
	ew->spd.y = 240.0f ;
	if( ew->disp_dir ) ew->spd.x = -ew->spd.x ;
	pl_char_set( ew, Char_Effect, 3 ) ;
	pls34_01_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls34_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ew->timer0 != pl_w->char_no.w ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->prio = pl_w->prio ;
	ew->pos.x = pl_w->pos.x + ew->spd.x ;
	ew->pos.y = pl_w->pos.y + ew->spd.y ;
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls34_01_next( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls34_01_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls34_01.c" End of File	*/

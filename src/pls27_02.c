/* Ken's player effect 02: a pair of marks, one each side, that stand at a place beside him
   by type, one of them above the other, and play an animation through. */
#include "player.h"
#include "pl_char.h"

static void pls27_02_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls27_02_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls27_02_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x2702 ;
		ew->type = no ;
		ew->timer0 = 0 ;
	}
	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls27_02_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x2702 ;
		ew->type = no ;
		ew->timer0 = 1 ;
	}
	return ew ;
}

/*==============================================================*/
static void pls27_02_pos_set( PLAYER_WORK *ew )
{
	static Sint16 pls27_02_ofs_tbl[] = { 0, 0, 0, 0, 4, 33, 75, -4 } ;
	Float dx ;

	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	dx = DOT_X( pls27_02_ofs_tbl[ew->type * 2] ) ;
	if( ew->disp_dir ) dx = -dx ;
	ew->pos.x += dx ;
	ew->pos.y += pls27_02_ofs_tbl[ew->type * 2 + 1] ;
	if( ew->timer0 ) ew->pos.y -= DOT_Y( 16.0f ) ;
}

static void pls27_02_init( PLAYER_WORK *ew );
static void pls27_02_move( PLAYER_WORK *ew );
static void pls27_02_end( PLAYER_WORK *ew );
static void pls27_02_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls27_02_main( PLAYER_WORK *ew )
{
	static void ( *pls27_02_jmp[] )( PLAYER_WORK *ew ) = {
		pls27_02_init, pls27_02_move, pls27_02_end, pls27_02_free,
	} ;

	pls27_02_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls27_02_init( PLAYER_WORK *ew )
{
	static Sint8 pls27_02_dir_tbl[] = { 0, 1, 1, 1 } ;

	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->disp_flag = 1 ;
	ew->prio = 0 ;
	ew->disp_dir = ew->disp_dir ^ pls27_02_dir_tbl[ew->type] ;
	pls27_02_pos_set( ew ) ;
	pl_char_set( ew, Char_Effect, 36 ) ;
	pls27_02_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls27_02_move( PLAYER_WORK *ew )
{
	pls27_02_pos_set( ew ) ;
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		pls27_02_end( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls27_02_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	pls27_02_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls27_02_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls27_02.c" End of File	*/

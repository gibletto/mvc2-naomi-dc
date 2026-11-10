/* Akuma's player effect 02: a still copy of his sprite, kept for 8 frames at the place where it was made. */
#include "player.h"

static void pls1e_02_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls1e_02_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls1e_02_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x1e02 ;
		ew->type = HI_BYTE( pl_w->cg_rival ) ;
	}
	return ew ;
}

static void pls1e_02_init( PLAYER_WORK *ew );
static void pls1e_02_move( PLAYER_WORK *ew );
static void pls1e_02_end( PLAYER_WORK *ew );
static void pls1e_02_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls1e_02_main( PLAYER_WORK *ew )
{
	static void ( *pls1e_02_jmp[] )( PLAYER_WORK *ew ) = {
		pls1e_02_init, pls1e_02_move, pls1e_02_end, pls1e_02_free,
	} ;

	pls1e_02_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls1e_02_init( PLAYER_WORK *ew )
{
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
	ew->prio = 15 ;
	ew->timer0 = 8 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	pls1e_02_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls1e_02_move( PLAYER_WORK *ew )
{
	if( -- ew->timer0 <= 0 ) ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void pls1e_02_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	pls1e_02_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls1e_02_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls1e_02.c" End of File	*/

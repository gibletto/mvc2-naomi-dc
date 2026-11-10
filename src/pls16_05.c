/* Marrow's player effect 05: a copy of her shape at her place that shows the frame her cg_flag1 asks for. */
#include "player.h"
#include "ef_char.h"
#include "pl_char.h"
#include "pl16_def.h"

static void pls16_05_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls16_05_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls16_05_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x1605 ;
	}
	return ew ;
}

static void pls16_05_init( PLAYER_WORK *ew );
static void pls16_05_move( PLAYER_WORK *ew );
static void pls16_05_end( PLAYER_WORK *ew );
static void pls16_05_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls16_05_main( PLAYER_WORK *ew )
{
	static void ( *pls16_05_jmp[] )( PLAYER_WORK *ew ) = {
		pls16_05_init, pls16_05_move, pls16_05_end, pls16_05_free,
	} ;

	pls16_05_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls16_05_init( PLAYER_WORK *ew )
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
	ew->prio = 0 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ef_char_set( ew, 27, 11 ) ;
}

/*--------------------------------------------------------------*/
static void pls16_05_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->prio = 0 ;
	if( pl_w->cg_flag1 - 1 < 0 ){
		pls16_05_end( ew ) ;
		return ;
	}
	ef_char_set2( ew, 27, 11, ( Sint8 )pl_w->cg_flag1 - 1 ) ;
}

/*--------------------------------------------------------------*/
static void pls16_05_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	pls16_05_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls16_05_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls16_05.c" End of File	*/

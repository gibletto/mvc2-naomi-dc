/* Amingo's player effect 03: a copy of his shape that follows him and takes up the animation his frames ask for. */
#include "player.h"
#include "pl_char.h"
#include "pl15_def.h"

static void pls15_03_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls15_03_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls15_03_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x1503 ;
	}
	return ew ;
}

static void pls15_03_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls15_03_end( PLAYER_WORK *ew );
static void pls15_03_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls15_03_main( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	switch( ew->routine_0 ){
	case 0:
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
		ew->disp_flag = 0 ;
	case 1:
		ew->prio = 0 ;
		pls15_03_move( ew, pl_w ) ;
		break ;
	case 2:
		pls15_03_end( ew ) ;
		break ;
	case 3:
		pls15_03_free( ew ) ;
		break ;
	}
}

static void pls15_03_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls15_03_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	switch( ew->routine_1 ){
	case 0:
		pls15_03_wait( ew, pl_w ) ;
		break ;
	case 1:
		ew->pos = pl_w->pos ;
		ew->disp_dir = pl_w->disp_dir ;
		if( pl_w->cg_flag1 != ew->sub_type ){
			ew->routine_1 = 0 ;
			ew->disp_flag = 0 ;
			ew->sub_type = 0 ;
			pls15_03_wait( ew, pl_w ) ;
		}
		break ;
	}
}

/*==============================================================*/
static void pls15_03_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Uint8 no ;

	if( pl_w->cg_flag1 >= 0x80 ){
		ew->routine_1 ++ ;
		ew->disp_flag = 1 ;
		ew->pos = pl_w->pos ;
		ew->sub_type = pl_w->cg_flag1 ;
		no = pl_w->cg_flag1 + 0x80 ;
		if( no == 18 ){
			ew->prio = 7 ;
			ew->sub_prio = pl_w->sub_prio + 8 ;
			ew->sub_prio &= 0x7f ;
		} else {
			ew->prio = 0 ;
		}
		ef_char_set2( ew, 27, 1, no ) ;
		ew->disp_dir = pl_w->disp_dir ;
	}
}

/*==============================================================*/
static void pls15_03_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*==============================================================*/
static void pls15_03_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls15_03.c" End of File	*/

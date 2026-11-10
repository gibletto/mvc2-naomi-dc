/* Sabretooth's player effect 01: a copy of his sprite that follows him while the act and the
   special move he was doing when it was made go on. */
#include "player.h"
#include "pl_char.h"
#include "pl2b_def.h"

static void pls2b_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls2b_01_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_END ) ) ){
		ew->ef_no = 0x2b01 ;
		ew->dir = pl_w->act_st ;
		ew->sub_no = pl_w->pl_sp_type ;
		ew->work_func = pls2b_01_main ;
		ew->master_w = pl_w ;
	}
	return ew ;
}

/*==============================================================*/
static void pls2b_01_main( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	switch( ew->routine_0 ){
	case 0 :
		ew->routine_0 ++ ;
		ew->pos = pl_w->pos ;
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
		pl_char_set( ew, Char_Effect, 11 ) ;
	case 1 :
		if( pl_w->act_st != ew->dir || pl_w->pl_sp_type != ew->sub_no ){
			ew->routine_0 ++ ;
			ew->disp_flag = 0 ;
			break ;
		}
		ew->pos = pl_w->pos ;
		if( !pl_w->hit_stop && pl_char_move( ew ) < 0 ){
			ew->routine_0 ++ ;
			ew->disp_flag = 0 ;
		}
		break ;
	case 2 :
		work_free( ew ) ;
		break ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls2b_01.c" End of File	*/

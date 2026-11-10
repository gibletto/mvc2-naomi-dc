/* Sabretooth's player effect 02: the pictures put on the opponent when his Berserker Claw or the
   rush of Berserker Claw X hits, two for each of two kinds, shown while that move goes on. */
#include "player.h"
#include "pl_char.h"
#include "pl2b_def.h"

static void pls2b_02_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls2b_02_set( PLAYER_WORK *pl_w, int type, int kind )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_END ) ) ){
		ew->ef_no = 0x2b00 ;
		ew->type = type ;
		ew->sub_type = kind ;
		ew->dir = pl_w->act_st ;
		ew->timer0 = pl_w->pl_sp_type ;
		ew->work_func = pls2b_02_main ;
		ew->master_w = pl_w ;
	}
	return ew ;
}

static void pls2b_02_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2b_02_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2b_02_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2b_02_main( PLAYER_WORK *ew )
{
	static void ( *pls2b_02_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2b_02_init, pls2b_02_move, pls2b_02_free,
	} ;

	pls2b_02_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void pls2b_02_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	int anm ;

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
	ew->disp_flag = 0 ;
	ew->prio = 7 ;
	anm = ew->type * 3 + ew->sub_type ;
	pl_char_set( ew, Char_Effect, anm + 12 ) ;
}

/*--------------------------------------------------------------*/
static void pls2b_02_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w ;

	if( pl_w->act_st == ew->dir && !pl_w->routine_1 && pl_w->pl_sp_type == ew->timer0 ){
		if( pl_w->hit_flag && !( pl_w->hit_flag & HR_GUARD ) && !ew->sub_no ){
			ew->disp_flag = 1 ;
			ew->sub_no = 1 ;
			em_w = pl_w->em_w ;
			ew->pos = em_w->pos ;
		}
		if( !ew->disp_flag ) return ;
		if( !ew->routine_1 ){
			if( pl_char_move( ew ) >= 0 || pl_w->hit_stop ) return ;
			if( !ew->sub_type ){
				ew->routine_1 ++ ;
				pl_char_set( ew, Char_Effect, ew->type * 3 + 14 ) ;
				return ;
			}
		}
		else if( pl_char_move( ew ) >= 0 ) return ;
	}
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls2b_02_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls2b_02.c" End of File	*/

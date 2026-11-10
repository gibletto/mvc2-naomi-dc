/* Sabretooth's player effect 04: the pictures put by the opponent, 16 dots either side and 32 up,
   when the last blow of his Berserker Claw X hits (two sides, and two animations as pls2b_02's). */
#include "player.h"
#include "pl_char.h"
#include "pl2b_def.h"

static void pls2b_04_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls2b_04_set( PLAYER_WORK *pl_w, int type, int kind )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_END ) ) ){
		ew->ef_no = 0x2b00 ;
		ew->type = type ;
		ew->sub_type = kind ;
		ew->work_func = pls2b_04_main ;
		ew->master_w = pl_w ;
	}
	return ew ;
}

static void pls2b_04_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2b_04_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2b_04_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2b_04_main( PLAYER_WORK *ew )
{
	static void ( *pls2b_04_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2b_04_init, pls2b_04_move, pls2b_04_free,
	} ;

	pls2b_04_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void pls2b_04_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->disp_flag = 0 ;
	ew->disp_dir = ew->type ;
	ew->prio = ew->sub_type ? 12 : 11 ;
	pl_char_set( ew, Char_Effect, ew->sub_type + 21 ) ;
}

/*--------------------------------------------------------------*/
static void pls2b_04_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w ;
	Float x ;

	if( !pl_w->routine_1 && pl_w->act_st == N_SUPER && pl_w->pl_sp_type == 3 ){
		if( pl_w->hit_flag && !( pl_w->hit_flag & HR_GUARD ) && !ew->sub_no ){
			ew->disp_flag = 1 ;
			em_w = pl_w->em_w ;
			x = DOT_X( 16.0f ) ;
			if( ew->type ) x = -DOT_X( 16.0f ) ;
			ew->pos.x = em_w->pos.x + x ;
			ew->pos.y = em_w->pos.y + DOT_Y( 32.0f ) ;
		}
		if( !ew->disp_flag ) return ;
		if( !ew->routine_1 ){
			if( pl_char_move( ew ) >= 0 || pl_w->hit_stop ) return ;
			if( !ew->sub_type ){
				ew->routine_1 ++ ;
				pl_char_set( ew, Char_Effect, 23 ) ;
				return ;
			}
		}
		else if( pl_char_move( ew ) >= 0 ) return ;
	}
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls2b_04_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls2b_04.c" End of File	*/

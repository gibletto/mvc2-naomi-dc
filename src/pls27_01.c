/* Ken's player effect 01: five copies of his sprite stacked above his place, one on each
   rung of a ladder, that follow him while he is in his Shinryuuken and fall away after. */
#include "player.h"
#include "pl_char_old.h"

static void pls27_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls27_01_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls27_01_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x2701 ;
		ew->type = 0 ;
	}
	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls27_01_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x2701 ;
		ew->type = 1 ;
	}
	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls27_01_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x2701 ;
		ew->type = 2 ;
	}
	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls27_01_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x2701 ;
		ew->type = 3 ;
	}
	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls27_01_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x2701 ;
		ew->type = 4 ;
	}
	return ew ;
}

static void pls27_01_init( PLAYER_WORK *ew );
static void pls27_01_move( PLAYER_WORK *ew );
static void pls27_01_end( PLAYER_WORK *ew );
static void pls27_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls27_01_main( PLAYER_WORK *ew )
{
	static void ( *pls27_01_jmp[] )( PLAYER_WORK *ew ) = {
		pls27_01_init, pls27_01_move, pls27_01_end, pls27_01_free,
	} ;

	pls27_01_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls27_01_init( PLAYER_WORK *ew )
{
	static Sint16 pls27_01_ofs_tbl[] = { 0, 127, 254, 381, 508 } ;
	PLAYER_WORK *pl_w = ew->master_w ;
	Sint8 no ;

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
	ew->timer0 = 0 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->ground_y + DOT_Y( pls27_01_ofs_tbl[ew->type] ) ;
	if( ew->type ) no = 23 ;
	else no = 0 ;
	pl_char_set( ew, Char_Effect, no ) ;
	pls27_01_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls27_01_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Sint8 no ;

	ew->pos.x = ew->master_w->pos.x ;
	if( pl_w->act_st != N_SUPER ){
		ew->routine_0 ++ ;
		pls27_01_end( ew ) ;
		return ;
	}
	if( !ew->routine_1 && ew->master_w->spd.y < 0.0f ){
		ew->routine_1 ++ ;
		if( ew->type ) no = 25 ;
		else no = 24 ;
		pl_char_set( ew, Char_Effect, no ) ;
	}
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls27_01_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	pls27_01_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls27_01_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls27_01.c" End of File	*/

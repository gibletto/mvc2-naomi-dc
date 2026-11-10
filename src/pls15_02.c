/* Amingo's player effect 02: the shape that rides up from his hand, falls back and returns to it
   while his hyper combo Taiyou no Megumi lasts. */
#include "player.h"
#include "ef_char.h"
#include "pl15_def.h"

static void pls15_02_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls15_02_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls15_02_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x1502 ;
	}
	return ew ;
}

static void pls15_02_init( PLAYER_WORK *ew );
static void pls15_02_move( PLAYER_WORK *ew );
static void pls15_02_end( PLAYER_WORK *ew );
static void pls15_02_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls15_02_main( PLAYER_WORK *ew )
{
	static void ( *pls15_02_jmp[] )( PLAYER_WORK *ew ) = {
		pls15_02_init, pls15_02_move, pls15_02_end, pls15_02_free,
	} ;

	pls15_02_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls15_02_init( PLAYER_WORK *ew )
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
	ew->pos = ew->master_w->pos ;
	ef_char_set( ew, 27, 3 ) ;
}

static void pls15_02_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls15_02_up( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls15_02_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls15_02_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void pls15_02_move( PLAYER_WORK *ew )
{
	static void ( *pls15_02_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls15_02_hold, pls15_02_up, pls15_02_wait, pls15_02_fall,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_1 || pl_w->act_st != N_SUPER ) ew->routine_0 = 2 ;
	else pls15_02_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls15_02_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Sint16 pls15_02_y_tbl[4] = { 0, 0x2000, 0x2a00, 0x4300 } ;

	ef_char_move( ew ) ;
	if( pl_w->cg_flag2 ){
		if( pl_w->cg_flag2 == 4 ){
			ew->routine_1 ++ ;
			ew->spd.y = DOT_Y( 16.0f ) ;
			ew->acc.y = DOT_Y( 0.125f ) ;
		} else {
			ew->pos.y = pl_w->pos.y + pls15_02_y_tbl[pl_w->cg_flag2] * 2.142857f / 256.0f ;
		}
	}
}

/*--------------------------------------------------------------*/
static void pls15_02_up( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ef_char_move( ew ) ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( DOT_Y( 528.0f ) < ew->pos.y ){
		ew->routine_1 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void pls15_02_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->routine_2 == 2 ){
		ew->routine_1 ++ ;
		ew->disp_flag = 1 ;
		ew->timer0 = 21 ;
		ew->acc.y = -DOT_Y( 0.5f ) ;
		ew->spd.y = ( pl_w->pos.y - ew->pos.y ) / 21.0f - ew->acc.y * 21.0f / 2.0f ;
	}
}

/*--------------------------------------------------------------*/
static void pls15_02_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ef_char_move( ew ) ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	ew->timer0 -- ;
	if( ew->timer0 <= 0 ) ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void pls15_02_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls15_02_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls15_02.c" End of File	*/

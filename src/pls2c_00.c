/* Magneto's player effect 00: a copy of his sprite that follows him and shows the animation
   his cg_rival asks for. */
#include "player.h"
#include "ef_char_old.h"
#include "pl2c_def.h"

static void pls2c_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls2c_00_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls2c_00_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
	}
	return ew ;
}

static void pls2c_00_init( PLAYER_WORK *ew );
static void pls2c_00_move( PLAYER_WORK *ew );
static void pls2c_00_next( PLAYER_WORK *ew );
static void pls2c_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls2c_00_main( PLAYER_WORK *ew )
{
	static void ( *pls2c_00_jmp[] )( PLAYER_WORK *ew ) = {
		pls2c_00_init, pls2c_00_move, pls2c_00_next, pls2c_00_free,
	} ;

	pls2c_00_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls2c_00_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
	pl_w = ew->master_w ;
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
	ew->timer0 = -1 ;
	pls2c_00_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls2c_00_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Uint8 no ;

	ew->disp_flag = 0 ;
	ew->prio = pl_w->prio ;
	if( ew->pl_type != pl_w->pl_type ){
		ew->routine_0 = 2 ;
		return ;
	}
	if( pl_w->disp_flag ){
		ew->pos = pl_w->pos ;
		ew->disp_dir = pl_w->disp_dir ;
		if( ( no = HI_BYTE( pl_w->cg_rival ) ) ){
			ew->disp_flag = 1 ;
			if( ew->timer0 != no ){
				ew->timer0 = no ;
				ef_char_set( ew, 27, no - 1 ) ;
			}
			else ef_char_move( ew ) ;
			return ;
		}
		ew->timer0 = -1 ;
	}
}

/*--------------------------------------------------------------*/
static void pls2c_00_next( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls2c_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls2c_00.c" End of File	*/

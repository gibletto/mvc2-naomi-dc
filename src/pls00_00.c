/* Ryu's player effect 00: a copy of him drawn over him (work kind 4), that takes up his animation
   when the animation's frame asks for another. */
#include "player.h"
#include "pl_char.h"

static void pls00_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls00_00_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS_ST, LIST_TOP ) ) ){
		ew->work_func = pls00_00_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0 ;
	}
	return ew ;
}

/*==============================================================*/
/*			The animation the master's frame asks for, in		*/
/*			the high byte of cg_rival: takes it up, or goes		*/
/*			on													*/
/*==============================================================*/
static void pls00_00_sync( PLAYER_WORK *ew )
{
	int no = HI_BYTE( ew->master_w->cg_rival ) ;

	if( no ){
		ew->disp_flag = 1 ;
		if( ew->timer0 != no ){
			pl_char_set( ew, Char_Effect, no ) ;
			ew->timer0 = no ;
			return ;
		}
		pl_char_move( ew ) ;
	}
	if( ew->master_w->be_flag ) ew->timer0 = HI_BYTE( ew->master_w->cg_rival ) ;
}

static void pls00_00_init( PLAYER_WORK *ew ) ;
static void pls00_00_move( PLAYER_WORK *ew ) ;
static void pls00_00_end( PLAYER_WORK *ew ) ;
static void pls00_00_free( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void pls00_00_main( PLAYER_WORK *ew )
{
	static void ( *pls00_00_jmp[] )( PLAYER_WORK *ew ) = {
		pls00_00_init, pls00_00_move, pls00_00_end, pls00_00_free,
	} ;

	pls00_00_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls00_00_init( PLAYER_WORK *ew )
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
	ew->disp_flag = 0 ;
	ew->prio = 0 ;
	ew->timer0 = 0 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	pls00_00_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls00_00_move( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	if( ew->master_w->disp_flag ){
		ew->pos.x = ew->master_w->pos.x ;
		ew->pos.y = ew->master_w->pos.y ;
		ew->disp_dir = ew->master_w->disp_dir ;
		pls00_00_sync( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls00_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	pls00_00_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls00_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls00_00.c" End of File	*/

/* Storm's player effect 00: a copy of her sprite, by type, that shows an animation of
   group 27 at her place while her own animation stays the one it began with. */
#include "player.h"
#include "ef_char.h"

static void pls2a_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls2a_00_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls2a_00_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->ef_no = 0x2a00 ;
	}
	return ew ;
}

static void pls2a_00_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2a_00_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2a_00_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2a_00_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2a_00_main( PLAYER_WORK *ew )
{
	static void ( *pls2a_00_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2a_00_init, pls2a_00_move, pls2a_00_wait, pls2a_00_end,
	} ;

	pls2a_00_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void pls2a_00_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->sub_prio = -1 ;
	ew->obj_flag = pl_w->char_no.w ;
	ef_char_set( ew, 27, ew->type + 45 ) ;
	pls2a_00_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls2a_00_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->disp_dir = pl_w->disp_dir ;
	ew->prio = pl_w->prio ;
	if( ew->obj_flag != pl_w->char_no.w ){
		pls2a_00_end( ew, pl_w ) ;
		return ;
	}
	if( ef_char_move( ew ) >= 0 ) return ;
	if( ew->type == 2 ) return ;
	pls2a_00_end( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls2a_00_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
}

/*--------------------------------------------------------------*/
static void pls2a_00_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls2a_00.c" End of File	*/

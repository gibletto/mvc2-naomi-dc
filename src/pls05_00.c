/* Strider's player effect 00: a short animation shown at a point beside the player, which
   frees itself when the animation is over. */
#include "player.h"
#include "pl_char.h"

static void pls05_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls05_00_set( PLAYER_WORK *pl_w, Float x, Float y )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls05_00_main ;
		ew->master_w = pl_w ;
		ew->type = 0 ;
		ew->ef_no = 0x500 ;
		ew->pos.x = pl_w->pos.x + ( pl_w->pl_dir ? -x : x ) ;
		ew->pos.y = pl_w->pos.y + y ;
	}
	return ew ;
}

static void pls05_00_init( PLAYER_WORK *ew );
static void pls05_00_move( PLAYER_WORK *ew );
static void pls05_00_end( PLAYER_WORK *ew );
static void pls05_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls05_00_main( PLAYER_WORK *ew )
{
	static void ( *pls05_00_jmp[] )( PLAYER_WORK *ew ) = {
		pls05_00_init, pls05_00_move, pls05_00_end, pls05_00_free,
	} ;

	pls05_00_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls05_00_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 1 ;
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
	ew->disp_dir = ew->master_w->disp_dir ;
	ew->prio = 0 ;
	pl_char_set( ew, Char_Effect, 24 ) ;
}

/*--------------------------------------------------------------*/
static void pls05_00_move( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ) pls05_00_end( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls05_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 = 3 ;
	pls05_00_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls05_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls05_00.c" End of File	*/

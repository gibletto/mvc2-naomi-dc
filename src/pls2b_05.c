/* Sabretooth's player effect 05: a picture on him in his throws (type 0 on the ground, 1 in the
   air), showing the frame his animation's code asks for. */
#include "player.h"
#include "pl2b_def.h"

static void pls2b_05_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls2b_05_set( PLAYER_WORK *pl_w, int type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_END ) ) ){
		ew->ef_no = 0x2b05 ;
		ew->type = type ;
		ew->work_func = pls2b_05_main ;
		ew->master_w = pl_w ;
	}
	return ew ;
}

static void pls2b_05_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2b_05_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2b_05_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2b_05_main( PLAYER_WORK *ew )
{
	static void ( *pls2b_05_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2b_05_init, pls2b_05_move, pls2b_05_free,
	} ;

	pls2b_05_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void pls2b_05_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->disp_dir = pl_w->disp_dir ;
	ew->prio = 12 ;
	pls2b_05_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls2b_05_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	ew->pos = pl_w->pos ;
	ew->disp_dir = pl_w->disp_dir ;
	if( pl_w->routine_1 != PL_CATCH ) ew->routine_0 = 2 ;
	if( pl_w->char_no.b.grp != Char_Catch ) ew->routine_0 = 2 ;
	if( pl_w->cg_type && ew->routine_0 != 2 ){
		ew->disp_flag = 1 ;
		pl_char_set2( ew, Char_Effect, ew->type + 24, pl_w->cg_type - 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls2b_05_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls2b_05.c" End of File	*/

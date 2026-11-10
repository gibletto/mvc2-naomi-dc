/* Abyss's second form's glow (effect 02): a faint copy of him that stays at his place, turns as he turns and
   goes when his work has gone past its first steps. */
#include "player.h"
#include "ef_char.h"
#include "pl19_def.h"

static void pls19_02_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls19_02_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls19_02_main ;
		ew->master_w = pl_w ;
	}
	return ew ;
}

static void pls19_02_init( PLAYER_WORK *ew );
static void pls19_02_move( PLAYER_WORK *ew );
static void pls19_02_end( PLAYER_WORK *ew );
static void pls19_02_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls19_02_main( PLAYER_WORK *ew )
{
	static void ( *pls19_02_jmp[] )( PLAYER_WORK *ew ) = {
		pls19_02_init, pls19_02_move, pls19_02_end, pls19_02_free,
	} ;

	pls19_02_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls19_02_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
	ew->ef_no = 0x1902 ;
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
	ew->disp_flag = 0 ;
	ew->prio = 7 ;
	ew->alpha = 0.7f ;
	ef_char_set( ew, 27, 1 ) ;
	pls19_02_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls19_02_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_0 >= 2 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->disp_dir = pl_w->disp_dir ;
	ew->pos = pl_w->pos ;
}

/*--------------------------------------------------------------*/
static void pls19_02_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls19_02_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls19_02.c" End of File	*/

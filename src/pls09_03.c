/* Iceman's player effect 03: the flash that follows his win pose (Char_Win), and the
   second one that starts beside it when his animation gets going. */
#include "player.h"
#include "pl_char.h"

static void pls09_03_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls09_03_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls09_03_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->ef_no = 0x903 ;
	}
	return ew ;
}

/*==============================================================*/
/*			The second flash, of the same master				*/
/*==============================================================*/
static void pls09_03_set2( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls09_03_main ;
		ew->master_w = pl_w->master_w ;
		ew->type = 2 ;
		ew->ef_no = 0x903 ;
	}
}

/*==============================================================*/
/*			Its place among the works of his: in front for		*/
/*			the first, behind for the others					*/
/*==============================================================*/
static void pls09_03_prio_set( PLAYER_WORK *ew )
{
	ew->prio = ew->master_w->prio ;
	if( ew->type ){
		ew->sub_prio = 1 ;
		return ;
	} else ew->sub_prio = -1 ;
}

static void pls09_03_init( PLAYER_WORK *ew );
static void pls09_03_move( PLAYER_WORK *ew );
static void pls09_03_end( PLAYER_WORK *ew );
static void pls09_03_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls09_03_main( PLAYER_WORK *ew )
{
	static void ( *pls09_03_jmp[] )( PLAYER_WORK *ew ) = {
		pls09_03_init, pls09_03_move, pls09_03_end, pls09_03_free,
	} ;

	pls09_03_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls09_03_init( PLAYER_WORK *ew )
{
	Float no ;

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
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	if( ew->type ){
		ew->prio = 7 ;
		no = 8 ;
		if( ew->type == 1 ){
			no = 6 ;
			if( ew->master_w->disp_dir ) ew->pos.x += DOT_X( 142.0f ) ;
			ew->disp_dir = 0 ;
		}
	} else {
		ew->prio = 0 ;
		no = 5 ;
	}
	pl_char_set( ew, 19, no ) ;
	pls09_03_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls09_03_move( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	pls09_03_prio_set( ew ) ;
	if( ew->master_w->char_no.b.grp != Char_Win ){
		ew->routine_0 ++ ;
		pls09_03_end( ew ) ;
		return ;
	}
	if( !ew->type && !ew->routine_1 && ew->master_w->cg_type ){
		ew->routine_1 ++ ;
		pl_char_set( ew, 19, 7 ) ;
		pls09_03_set2( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls09_03_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	pls09_03_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls09_03_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls09_03.c" End of File	*/

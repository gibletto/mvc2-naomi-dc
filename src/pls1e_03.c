/* Akuma's player effect 03: a copy of his sprite, by type, that follows him offset sideways while his animation lasts. */
#include "player.h"
#include "pl_char.h"

static void pls1e_03_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls1e_03_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls1e_03_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x1e03 ;
		ew->type = no ;
	}
	return ew ;
}

static void pls1e_03_init( PLAYER_WORK *ew );
static void pls1e_03_move( PLAYER_WORK *ew );
static void pls1e_03_end( PLAYER_WORK *ew );
static void pls1e_03_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls1e_03_main( PLAYER_WORK *ew )
{
	static void ( *pls1e_03_jmp[] )( PLAYER_WORK *ew ) = {
		pls1e_03_init, pls1e_03_move, pls1e_03_end, pls1e_03_free,
	} ;

	pls1e_03_jmp[ew->routine_0]( ew ) ;
}

/* by type: how far the copy sits from him, and the animation it shows */
static Sint8 pls1e_03_ofs_tbl[] = { -24, -4, -4 } ;
static Uint8 pls1e_03_no_tbl[] = { 10, 11, 12 } ;

/*==============================================================*/
static void pls1e_03_follow( PLAYER_WORK *ew )
{
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	if( ew->master_w->disp_dir ) ew->pos.x += DOT_X( pls1e_03_ofs_tbl[ew->type] ) ;
	ew->prio = ew->master_w->prio ;
	ew->sub_prio = -1 ;
}

/*==============================================================*/
static void pls1e_03_init( PLAYER_WORK *ew )
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
	ew->disp_flag = 1 ;
	ew->prio = 0 ;
	ew->disp_dir = 0 ;
	pls1e_03_follow( ew ) ;
	pl_char_set( ew, Char_Effect, pls1e_03_no_tbl[ew->type] ) ;
}

/*--------------------------------------------------------------*/
static void pls1e_03_move( PLAYER_WORK *ew )
{
	if( ew->master_w->char_no.b.grp != Char_Win && ew->type == 1 ){
		ew->routine_0 ++ ;
		pls1e_03_end( ew ) ;
		return ;
	}
	pls1e_03_follow( ew ) ;
	if( ew->type != 1 ){
		if( pl_char_move( ew ) < 0 ){
			ew->routine_0 ++ ;
			pls1e_03_end( ew ) ;
		}
	}
	else pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls1e_03_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	pls1e_03_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls1e_03_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls1e_03.c" End of File	*/

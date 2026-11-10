/* Ryu's player effect 02: three works (kind 3) of his Shinkuu Tatsumaki Senpuukyaku that sit on the ground by him, one under him and two
   at his sides, and stay while his animation is the one they were made for. */
#include "player.h"
#include "pl_char.h"

static void pls00_02_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls00_02_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls00_02_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 2 ;
		ew->type = 0 ;
		ew->sub_type = 0 ;
	}
	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls00_02_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 2 ;
		ew->type = 1 ;
		ew->sub_type = 1 ;
	}
	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls00_02_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 2 ;
		ew->type = 2 ;
		ew->sub_type = 1 ;
	}
	return ew ;
}

static void pls00_02_init( PLAYER_WORK *ew ) ;
static void pls00_02_move( PLAYER_WORK *ew ) ;
static void pls00_02_end( PLAYER_WORK *ew ) ;
static void pls00_02_free( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void pls00_02_main( PLAYER_WORK *ew )
{
	static void ( *pls00_02_jmp[] )( PLAYER_WORK *ew ) = {
		pls00_02_init, pls00_02_move, pls00_02_end, pls00_02_free,
	} ;

	pls00_02_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
/*			On the master's ground, at the side its type		*/
/*			says												*/
/*==============================================================*/
static void pls00_02_pos_set( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Float x ;

	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->ground_y ;
	switch( ew->type ){
	case 0:
		x = 0.0f ;
		break ;
	case 1:
		x = 80.0f ;
		break ;
	case 2:
		x = 80.0f ;
		ew->disp_dir ^= 1 ;
		break ;
	}
	if( ew->disp_dir ) x = -x ;
	ew->pos.x += x ;
}

/*==============================================================*/
static void pls00_02_init( PLAYER_WORK *ew )
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
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	pls00_02_pos_set( ew ) ;
	pl_char_set( ew, Char_Effect, ew->sub_type ? 12 : 13 ) ;
}

/*--------------------------------------------------------------*/
static void pls00_02_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	pls00_02_pos_set( ew ) ;
	pl_w = ew->master_w ;
	if( pl_w->char_no.b.grp != Char_Super || pl_w->char_no.b.no != 1 ){
		pls00_02_end( ew ) ;
		return ;
	}
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls00_02_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	pls00_02_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls00_02_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls00_02.c" End of File	*/

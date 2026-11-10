/* Mega Man's player effect 01: nine pictures strung up between the player and the effect it
   keeps hold of, each one a step further along. */
#include "player.h"
#include "ef_char.h"
#include "pl1c_def.h"

static void pls1c_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls1c_01_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls1c_01_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
	}
	return ew ;
}

static void pls1c_01_init( PLAYER_WORK *ew );
static void pls1c_01_move( PLAYER_WORK *ew );
static void pls1c_01_end( PLAYER_WORK *ew );
static void pls1c_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls1c_01_main( PLAYER_WORK *ew )
{
	static void ( *pls1c_01_jmp[] )( PLAYER_WORK *ew ) = {
		pls1c_01_init, pls1c_01_move, pls1c_01_end, pls1c_01_free,
	} ;

	pls1c_01_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls1c_01_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
	ew->ef_no = 0x1c01 ;
	ew->disp_flag = 1 ;
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
	ew->sub_prio = -1 ;
	ew->pos.x = pl_w->pos.x ;
	if( ew->type ){
		ef_char_set( ew, 27, 1 ) ;
	} else {
		ef_char_set( ew, 27, 2 ) ;
	}
	pls1c_01_move( ew ) ;
}

static void pls1c_01_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void pls1c_01_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_2 > 2 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->prio = pl_w->prio ;
	if( !ew->type ){
		ew->pos.y = pl_w->pos.y + DOT_Y( 142.0f ) ;
	} else {
		pls1c_01_pos_set( ew, pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls1c_01_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls1c_01_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			The picture's step of the way up from the player	*/
/*			to the effect it holds								*/
/*==============================================================*/
static void pls1c_01_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ef_w ;
	Float y ;

	pl_w = ew->master_w ;
	ef_w = pl_w->ef_w ;
	y = ( ef_w->pos.y - pl_w->pos.y + -DOT_Y( 16.0f ) ) / 8.0f ;
	ew->pos.y = pl_w->pos.y + y * ew->type ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls1c_01.c" End of File	*/

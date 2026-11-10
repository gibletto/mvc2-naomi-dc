/* Strider's player effect 02: a mark set on the player's position for a number of frames. It
   is dropped when the player is hit, kept still while he is in hit stop, and may flicker. */
#include "player.h"

static void pls05_02_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls05_02_set( PLAYER_WORK *pl_w, Uint8 flicker, Sint16 time )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls05_02_main ;
		ew->master_w = pl_w ;
		ew->sub_type = flicker ;
		ew->timer0 = time ;
		ew->ef_no = 0x502 ;
	}
	return ew ;
}

static void pls05_02_init( PLAYER_WORK *ew );
static void pls05_02_move( PLAYER_WORK *ew );
static void pls05_02_end( PLAYER_WORK *ew );
static void pls05_02_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls05_02_main( PLAYER_WORK *ew )
{
	static void ( *pls05_02_jmp[] )( PLAYER_WORK *ew ) = {
		pls05_02_init, pls05_02_move, pls05_02_end, pls05_02_free,
	} ;

	pls05_02_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls05_02_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

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
	ew->prio = 7 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	pls05_02_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls05_02_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->work_kind == W_PL ){
		if( pl_w->hit_box_no ){
			pls05_02_end( ew ) ;
			return ;
		}
		if( pl_w->hit_stop ){
			ew->disp_flag = 1 ;
			return ;
		}
	}
	if( !-- ew->timer0 ){
		pls05_02_end( ew ) ;
		return ;
	}
	ew->disp_flag = 1 ;
	if( ew->sub_type ) ew->disp_flag = Sys->game_timer0 & 1 ;
}

/*--------------------------------------------------------------*/
static void pls05_02_end( PLAYER_WORK *ew )
{
	ew->routine_0 = 3 ;
	ew->disp_flag = 0 ;
	pls05_02_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls05_02_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls05_02.c" End of File	*/

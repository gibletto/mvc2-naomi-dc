/* Damage work 05 (work kind 10, 0x0005): while the player is in his bounce off the screen's edges
   (pl_sp_type 4, act 29, animation group 22), it keeps him inside the edge his animation names. */
#include "player.h"

static void dm00_05_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *dm00_05_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_DM, LIST_END ) ) ){
		ew->ef_no = 5 ;
		ew->work_func = dm00_05_main ;
		ew->master_w = pl_w ;
	}
	return ew ;
}

/*==============================================================*/
static void dm00_05_main( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Float x, y ;

	if( !pl_w->routine_1 && pl_w->pl_sp_type == 4 && pl_w->act_st == N_SUPER && pl_w->char_no.b.grp == Char_Super ){
		x = pl_w->pos.x ;
		y = pl_w->pos.y ;
		switch( pl_w->char_no.b.no ){
		case 4:
		case 5:
			if( scr_work.scr_bottom > pl_w->ground_y ) y = scr_work.scr_bottom ;
			else y = pl_w->ground_y ;
			break ;
		case 6:
		case 7:
			y = scr_work.scr_top - DOT_Y( 120.0f ) ;
			break ;
		case 8:
		case 9:
			x = scr_work.scr_left + 40.0f ;
			break ;
		case 10:
		case 11:
			x = scr_work.scr_right - 40.0f ;
			break ;
		default:
			break ;
		}
		pl_w->pos.x = x ;
		pl_w->pos.y = y ;
		return ;
	}
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "dm00_05.c" End of File	*/

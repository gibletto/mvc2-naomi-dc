/* Damage work 01 (work kind 10, 0x0001): while the player is in the grab of his special move
   (pl_sp_type 1, act 21, animation 20), it keeps him at the edge of the screen he faces away from. */
#include "player.h"

static void dm00_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *dm00_01_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_DM, LIST_END ) ) ){
		ew->ef_no = 1 ;
		ew->work_func = dm00_01_main ;
		ew->master_w = pl_w ;
	}
	return ew ;
}

/*==============================================================*/
static void dm00_01_main( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Float x ;

	if( pl_w->pl_sp_type == 1 && pl_w->act_st == N_SPECIAL && pl_w->char_no.b.grp == Char_Special ){
		if( pl_w->char_no.b.no == 20 ){
			x = scr_work.scr_left + 40.0f ;
			if( pl_w->pl_dir ) x = scr_work.scr_right - 40.0f ;
			pl_w->pos.x = x ;
		}
		return ;
	}
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "dm00_01.c" End of File	*/

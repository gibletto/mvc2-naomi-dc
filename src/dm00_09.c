/* Damage work 09 (work kind 10, 0x0009): lives while the player is in the first act of his
   special move 1 (pl_sp_type 1, act 21, animation group 21) and ends with it. */
#include "player.h"

static void dm00_09_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *dm00_09_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_DM, LIST_END ) ) ){
		ew->ef_no = 9 ;
		ew->work_func = dm00_09_main ;
		ew->master_w = pl_w ;
	}
	return ew ;
}

/*==============================================================*/
static void dm00_09_main( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( !pl_w->routine_1 && pl_w->pl_sp_type == 1 && pl_w->act_st == N_SPECIAL && pl_w->char_no.b.grp == Char_Special ) return ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "dm00_09.c" End of File	*/

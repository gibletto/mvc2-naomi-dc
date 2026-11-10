/* Damage work 0a (work kind 10, 0x000a): while Thanos's count (tlocal unk_14) is above
   zero it steps another count round six values and sets his effect colour from it each frame. */
#include "player.h"

static void dm00_0a_main( PLAYER_WORK *ew );

/****************************************************************/
void dm00_0a_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_DM, LIST_TOP ) ) ){
		ew->work_func = dm00_0a_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 10 ;
	}
}

/*==============================================================*/
/*			Thanos's PL36_LOCAL_W unk_14[0] and unk_14[1]		*/
/*			are tlocal[5] and tlocal[6] here					*/
/*==============================================================*/
static void dm00_0a_main( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->tlocal[5] > 0 ){
		pl_w->tlocal[6] ++ ;
		pl_w->tlocal[6] %= 6 ;
		ef_color_set( pl_w, 4, pl_w->tlocal[6] + 16, 1 ) ;
	} else {
		work_free( ew ) ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "dm00_0a.c" End of File	*/

/* A model of the game's start (kind 11 work): s_play.c makes one for each
   model of obj_dm0a, which is shown after 480 frames. */
#include "player.h"

static void s_play_ef0_main( PLAYER_WORK *ew ) ;

/****************************************************************/
void s_play_ef0_set( int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SYS, LIST_END ) ) ){
		ew->disp_flag = 0 ;
		ew->type = no ;
		ew->work_func = s_play_ef0_main ;
		ew->obj = obj_dm0a->obj[no] ;
		ew->obj_flag = 0 ;
		ew->timer0 = 480 ;
	}
}

/*==============================================================*/
static void s_play_ef0_main( PLAYER_WORK *ew )
{
	if( ew->timer0 ){
		ew->timer0 -- ;
		return ;
	}
	ew->disp_flag = 1 ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_play_ef0.c" End of File	*/

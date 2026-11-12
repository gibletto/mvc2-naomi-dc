/* DM0F, a part chosen by CS_Secret_Work.unk_11; nothing sets it. */
#include "player.h"

void dm0f_part4_nop( PLAYER_WORK *w ) ;

void dm0f_part4_set( void )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_END ) ) ){
		w->disp_flag = 1 ;
		w->work_func = dm0f_part4_nop ;
		w->obj = obj_dm0f->obj[127 - CS_Secret_Work.unk_11] ;
	}
}

void dm0f_part4_nop( PLAYER_WORK *w )
{
}

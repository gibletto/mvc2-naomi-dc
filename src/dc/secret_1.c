/* SECRET FACTOR's first page: the characters bought. */
#include "player.h"
#include "shinobi.h"
#include "dm.h"
#include "secret.h"

static PLAYER_WORK *page1_w ;	/* secret_factor_1's frame */

/****************************************************************/
/*			What has been bought								*/
/****************************************************************/
void secret_factor_1( Uint16 sw, Uint16 sw2 )
{
	switch( Sys->routine_2 ){
	case 0 :
		work_list_kill( 11 ) ;
		dm0c_wall_paper( 5 ) ;
		headline_set( 11 ) ;
		page1_w = dm0f_page1_set( vms_work.stage_ex_cnt ) ;
		dm0f_page1_icon_set() ;
		fade_set( 0xff000000, 10, FADE_IN ) ;
		Sys->routine_2 ++ ;
	case 1 :
		if( !fade_cnt() ) Sys->routine_2 ++ ;
		break ;
	case 2 :
		if( decide_ck_nose() ){
			if( page1_w->sub_no ){
				if( page1_w->routine_1 != 0 ) goto close ;
			} else if( page1_w->routine_1 == 7 ) goto close ;
		}
		if( cancel_ck_nose() ) goto close ;
		break ;
	case 3 :
		if( !fade_cnt() ){
			Sys->routine_0 = 1 ;
			Sys->routine_2 = 0 ;
		}
		break ;
	}
	return ;
close :
	if( decide_ck_nose() ) na_req( 2 ) ;
	else na_req( 3 ) ;
	fade_set( 0xff000000, 10, FADE_OUT ) ;
	Sys->routine_2 ++ ;
}

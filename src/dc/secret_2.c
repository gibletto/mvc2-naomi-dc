/* SECRET FACTOR's second page: the extras bought. */
#include "player.h"
#include "shinobi.h"
#include "dm.h"
#include "secret.h"

/****************************************************************/
/*			A page of fourteen									*/
/****************************************************************/
void secret_factor_2( Uint16 sw, Uint16 sw2 )
{
	int i ;

	switch( Sys->routine_2 ){
	case 0 :
		work_list_kill( 11 ) ;
		for( i = 0 ; i < 14 ; i++ ) dm0f_page2_set( i ) ;
		fade_set( 0xff000000, 10, FADE_IN ) ;
		Sys->routine_2 ++ ;
	case 1 :
		if( !fade_cnt() ) Sys->routine_2 ++ ;
		break ;
	case 2 :
		if( cancel_ck() ){
			fade_set( 0xff000000, 10, FADE_OUT ) ;
			Sys->routine_2 ++ ;
		}
		break ;
	case 3 :
		if( !fade_cnt() ){
			Sys->routine_0 = 1 ;
			Sys->routine_2 = 0 ;
		}
		break ;
	}
}

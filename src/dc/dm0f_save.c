/* DM0F, saving the VMU game: the two answers of secret_factor_3, the one under the cursor
   (Sys->timer0) glowing until it is chosen. */
#include "player.h"
#include "nl.h"
#include "dm.h"

void dm0f_save_move( PLAYER_WORK *w ) ;

static Float dm0f_save_y[2] = { 4.0f, 1.0f } ;

/****************************************************************/
/*			The answers											*/
/****************************************************************/
void dm0f_save_set( void )
{
	PLAYER_WORK *w ;
	int i ;

	for( i = 0 ; i < 2 ; i ++ ){
		if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return ;
		w->disp_flag = 1 ;
		w->work_func = dm0f_save_move ;
		w->obj = obj_dm0f->obj[i + 271] ;
		w->obj_flag = OBJ_POS | OBJ_TRNSL ;
		w->pos.y = dm0f_save_y[i] ;
		w->pos.z = -12.99f ;
		w->type = i ;
	}
}

/****************************************************************/
/*			An answer: shown under the cursor, glowing until	*/
/*			it is chosen (bit 7)								*/
/****************************************************************/
void dm0f_save_move( PLAYER_WORK *w )
{
	if( ( Sys->timer0 & 0x7f ) == w->type ){
		w->disp_flag = 1 ;
		if( Sys->timer0 & 0x80 ){
			w->timer0 = 0 ;
			w->fade.trnsl = 1.0f ;
		} else {
			w->timer0 += 9 ;
			if( w->timer0 >= 360 ) w->timer0 = 0 ;
			w->fade.trnsl = nlCos( DEG_ANG( w->timer0 ) ) * 0.5f + 0.5f ;
		}
	} else {
		w->disp_flag = 0 ;
		w->timer0 = 0 ;
	}
}

/* Stage 05's object 4: a model (object 17 of the stage's file) that waits for a side to be in the
   fight and then pulses in colour for a time. */
#include "player.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

/*==============================================================*/
static void set05_04_main( PLAYER_WORK *ew )
{
	switch( ew->routine_0 ){
	case 0 :
		if( side_flag_get( 0 ) ){
			ew->routine_0 ++ ;
			ew->timer0 = -90 ;
		}
		break ;
	case 1 :
		ew->fade.r = 0.5f + 0.5f * nlSin( DEG_ANG( ew->timer0 ) ) ;
		ew->fade.g = 0.5f + 0.5f * nlSin( DEG_ANG( ew->timer0 ) ) ;
		ew->fade.b = 0.5f + 0.5f * nlSin( DEG_ANG( ew->timer0 ) ) ;
		ew->timer0 += 2 ;
		if( ew->timer0 >= 270 ){
			ew->routine_0 = 0 ;
			ew->timer0 = -90 ;
		}
		break ;
	}
}

/****************************************************************/
void set05_04_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set05_04_main ;
		ew->obj = obj_stg->obj[17] ;
		ew->obj_flag = OBJ_POS | OBJ_FADE | OBJ_BRIGHT ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set05_04.c" End of File	*/

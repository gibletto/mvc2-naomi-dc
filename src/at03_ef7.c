/* A glow in the third attract demo (kind 11 works): the demo's model 9,
   made when the demo's count reaches 0x177, which brightens to three quarters and then breathes
   about it. */
#include "player.h"

static VEC3 at03_ef7_rot = { 0.0f, 0.0f, 0.0f } ;	/* never used */
static VEC3 at03_ef7_scale = { 1.0f, 1.0f, 1.0f } ;	/* never used */

static void at03_ef7_main( PLAYER_WORK *ew ) ;

/****************************************************************/
void at03_ef7_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SYS, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = at03_ef7_main ;
		ew->obj = obj_dm07->obj[9] ;
		ew->obj_flag = OBJ_FADE | OBJ_BRIGHT ;
	}
}

/*==============================================================*/
static void at03_ef7_main( PLAYER_WORK *ew )
{
	Float f ;

	switch( ew->routine_0 ){
	case 0:
		f = ew->fade.r ;
		f += 0.01f ;
		if( f >= 0.75f ){
			ew->routine_0 ++ ;
			f = 0.75f ;
		}
		ew->fade.r = f ;
		ew->fade.g = f ;
		ew->fade.b = f ;
		break ;
	case 1:
		f = 0.75f + nlSin( ( int )( ew->timer0 * 65536.0f / 360.0f + 0.5f ) & 0xffff ) * 0.25f ;
		ew->fade.r = f ;
		ew->fade.g = f ;
		ew->fade.b = f ;
		ew->timer0 += 5 ;
		if( ew->timer0 >= 360 ) ew->timer0 = 0 ;
		break ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "at03_ef7.c" End of File	*/

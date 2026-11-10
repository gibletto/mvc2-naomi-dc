/* A flash in the third attract demo (kind 11 works): the demo's model 10,
   made when the demo's count reaches 0x17c, which dims away and ends the work. */
#include "player.h"

static VEC3 at03_ef6_rot = { 0.0f, 0.0f, 0.0f } ;	/* never used */
static VEC3 at03_ef6_scale = { 1.0f, 1.0f, 1.0f } ;	/* never used */

static void at03_ef6_main( PLAYER_WORK *ew ) ;

/****************************************************************/
void at03_ef6_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SYS, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = at03_ef6_main ;
		ew->obj = obj_dm07->obj[10] ;
		ew->obj_flag = OBJ_FADE | OBJ_BRIGHT ;
		ew->fade.r = 1.0f ;
		ew->fade.g = 1.0f ;
		ew->fade.b = 1.0f ;
	}
}

/*==============================================================*/
static void at03_ef6_main( PLAYER_WORK *ew )
{
	ew->fade.r -= 0.05f ;
	ew->fade.g -= 0.05f ;
	ew->fade.b -= 0.05f ;
	if( ew->fade.r <= 0.0f ) work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "at03_ef6.c" End of File	*/

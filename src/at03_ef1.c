/* The third attract demo's sky (kind 11 works): the demo's model 5, which
   brightens in until the game's check passes, while the texture of model 6 slides. */
#include "player.h"

static VEC3 at03_ef1_rot = { 0.0f, 0.0f, 0.0f } ;	/* never used */
static VEC3 at03_ef1_scale = { 1.0f, 1.0f, 1.0f } ;	/* never used */

extern void obj_uv_begin( int *src, void *obj ) ;
extern int obj_uv_next( void ) ;
extern void obj_uv_get( Float *x, Float *y ) ;
extern void obj_uv_put( Float *x, Float *y ) ;
extern void obj_uv_clear( void *obj ) ;
extern int game_staff_ck( void ) ;

static void at03_ef1_main( PLAYER_WORK *ew ) ;

/****************************************************************/
void at03_ef1_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SYS, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = at03_ef1_main ;
		ew->obj = obj_dm07->obj[5] ;
		ew->obj_flag = OBJ_BRIGHT ;
		obj_uv_clear( obj_dm07->obj[5] ) ;
	}
}

static void at03_ef1_scroll( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void at03_ef1_main( PLAYER_WORK *ew )
{
	Float f ;

	if( ++ ew->timer0 >= 360 ) ew->timer0 = 0 ;
	if( ++ ew->timer1 >= 200 ) ew->timer1 = 0 ;
	if( !game_staff_ck() ){
		f = ew->fade.r ;
		if( f < 1.0f ){
			f = f + 0.003125f ;
			ew->fade.r = f ;
			ew->fade.g = f ;
			ew->fade.b = f ;
		}
	}
	at03_ef1_scroll( ew ) ;
}

/*==============================================================*/
/*			The texture slides									*/
/*==============================================================*/
/* u is unused */
static void at03_ef1_scroll( PLAYER_WORK *ew )
{
	Float u[3] ;
	Float py, px ;

	obj_uv_begin( obj_dm07->obj[6], ew->obj ) ;
	while( obj_uv_next() == 0 ){
		obj_uv_get( &px, &py ) ;
		px += ew->timer1 * 0.005f ;
		py += ew->timer1 * 0.005f ;
		obj_uv_put( &px, &py ) ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "at03_ef1.c" End of File	*/

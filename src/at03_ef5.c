/* A scrolling model of the second attract demo's file (kind 11 works): the
   demo's model 7, whose texture is the one of model 8 sliding both ways. */
#include "player.h"

static VEC3 at03_ef5_rot = { 0.0f, 0.0f, 0.0f } ;	/* never used */
static VEC3 at03_ef5_scale = { 1.0f, 1.0f, 1.0f } ;	/* never used */

extern void obj_uv_begin( int *src, void *obj ) ;
extern int obj_uv_next( void ) ;
extern void obj_uv_get( Float *x, Float *y ) ;
extern void obj_uv_put( Float *x, Float *y ) ;
extern void obj_uv_clear( void *obj ) ;

static void at03_ef5_main( PLAYER_WORK *ew ) ;

/****************************************************************/
void at03_ef5_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SYS, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = at03_ef5_main ;
		ew->obj = obj_dm07->obj[7] ;
		ew->obj_flag = 0 ;
		obj_uv_clear( obj_dm07->obj[7] ) ;
	}
}

static void at03_ef5_scroll( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void at03_ef5_main( PLAYER_WORK *ew )
{
	if( ++ ew->timer0 >= 360 ) ew->timer0 = 0 ;
	if( ++ ew->timer1 >= 200 ) ew->timer1 = 0 ;
	at03_ef5_scroll( ew ) ;
}

/*==============================================================*/
/* u is unused */
static void at03_ef5_scroll( PLAYER_WORK *ew )
{
	Float u[3] ;
	Float py, px ;

	obj_uv_begin( obj_dm07->obj[8], ew->obj ) ;
	while( obj_uv_next() == 0 ){
		obj_uv_get( &px, &py ) ;
		px -= ew->timer1 * 0.005f ;
		py += ew->timer1 * 0.005f ;
		obj_uv_put( &px, &py ) ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "at03_ef5.c" End of File	*/

/* Stage 05's object 3: four models (objects 11, 13, 15 and 17 of the stage's file) that change
   colour with the frame count and whose texture slides, each started a quarter of the way on from
   the one before it, by their texture's model. */
#include "player.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

extern void obj_uv_begin( int *src, void *obj ) ;
extern int obj_uv_next( void ) ;
extern void obj_uv_get( Float *x, Float *y ) ;
extern void obj_uv_put( Float *x, Float *y ) ;
extern void obj_uv_clear( int *obj ) ;

static void set05_03_make( int no ) ;

/****************************************************************/
void set05_03_set( void )
{
	int i ;

	for( i = 0 ; i < 4 ; i ++ ) set05_03_make( i ) ;
}

static void set05_03_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set05_03_make( int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set05_03_main ;
		ew->obj = obj_stg->obj[no * 2 + 9] ;
		ew->obj_flag = OBJ_POS | OBJ_FADE | OBJ_BRIGHT ;
		ew->type = no ;
		ew->fade.r = 0.0f ;
		ew->fade.g = 0.0f ;
		ew->fade.b = 0.0f ;
		ew->timer0 = 180 - no * 360 / 6 ;
		obj_uv_clear( ew->obj ) ;
	}
}

/*==============================================================*/
static void set05_03_main( PLAYER_WORK *ew )
{
	Float uv_y, uv_x ;

	ew->timer0 ++ ;
	if( ew->timer0 >= 1000 ) ew->timer0 = 0 ;
	obj_uv_begin( obj_stg->obj[ew->type * 2 + 9 + 1], ew->obj ) ;
	while( obj_uv_next() == 0 ){
		obj_uv_get( &uv_x, &uv_y ) ;
		uv_y += ew->timer0 * 0.001f ;
		obj_uv_put( &uv_x, &uv_y ) ;
	}
	ew->fade.r = 0.25f + 0.25f * nlSin( DEG_ANG( ew->timer0 * 3600.0f / 1000.0f ) ) ;
	ew->fade.g = ew->fade.r ;
	ew->fade.b = ew->fade.r ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set05_03.c" End of File	*/

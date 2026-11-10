/* Stage 05's object 0: a model (object 1 of the stage's file) whose points rise and fall in waves
   while its texture slides. */
#include "player.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

extern void obj_uv_begin( int *src, void *obj ) ;
extern int obj_uv_next( void ) ;
extern void obj_uv_pos_get( VEC3 *v ) ;
extern void obj_uv_pos_put( VEC3 *v ) ;
extern void obj_uv_get( Float *x, Float *y ) ;
extern void obj_uv_put( Float *x, Float *y ) ;
extern void obj_uv_clear( int *obj ) ;

static void set05_00_main( PLAYER_WORK *ew ) ;

/****************************************************************/
void set05_00_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set05_00_main ;
		ew->obj = obj_stg->obj[1] ;
		ew->obj_flag = OBJ_POS | OBJ_BRIGHT ;
		obj_uv_clear( ew->obj ) ;
	}
}

/*==============================================================*/
static void set05_00_main( PLAYER_WORK *ew )
{
	VEC3 v ;
	Float uv_y, uv_x ;
	int phase ;

	switch( ew->routine_0 ){
	case 0 :
		if( ++ ew->timer0 >= 360 ) ew->timer0 = 0 ;
		phase = 0 ;
		if( ++ ew->timer1 >= 2000 ) ew->timer1 = 0 ;
		obj_uv_begin( obj_stg->obj[2], ew->obj ) ;
		while( obj_uv_next() == 0 ){
			obj_uv_pos_get( &v ) ;
			v.y = -53.6f + nlSin( DEG_ANG( ew->timer0 + phase ) ) * 10.0f ;
			obj_uv_pos_put( &v ) ;
			phase += 30 ;
			obj_uv_get( &uv_x, &uv_y ) ;
			uv_y += ew->timer1 * 0.0005f ;
			obj_uv_put( &uv_x, &uv_y ) ;
		}
		break ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set05_00.c" End of File	*/

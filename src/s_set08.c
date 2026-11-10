/* Stage 08's wave: a model, made by the first object's main, whose points ripple and whose texture
   slides while it fades in and out through the fight (the entry s_set08_set is called from
   set08_00.c). */
#include "player.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )


extern void obj_uv_begin( int *src, void *obj ) ;
extern int obj_uv_next( void ) ;
extern int obj_uv_pos_get( VEC3 *v ) ;
extern int obj_uv_pos_put( VEC3 *v ) ;
extern void obj_uv_get( Float *x, Float *y ) ;
extern void obj_uv_put( Float *x, Float *y ) ;
extern void obj_uv_clear( void *obj ) ;

static const VEC3 set08_wave_pos = { 0.0f, 20.0f, 100.0f } ;

static void set08_wave_main( PLAYER_WORK *ew ) ;

/****************************************************************/
void s_set08_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_MARK, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set08_wave_main ;
		ew->obj = obj_stg->obj[47] ;
		ew->obj_flag = OBJ_POS | OBJ_FADE | OBJ_BRIGHT ;
		ew->pos = set08_wave_pos ;
		ew->fade.r = 0.0f ;
		ew->fade.g = 0.0f ;
		ew->fade.b = 0.0f ;
		obj_uv_clear( ew->obj ) ;
	}
}

/*==============================================================*/
static void set08_wave_main( PLAYER_WORK *ew )
{
	VEC3 v ;
	Float y, x ;

	ew->disp_flag = 1 ;
	if( Sys->hyper_bg || vs_sys_work.game_stop || flash_flag_get() == 2 ) ew->disp_flag = 0 ;
	switch( ew->routine_0 ){
	case 0 :
		ew->fade.g += 0.0029166667f ;
		if( ew->fade.g >= 0.7f ){
			ew->routine_0 ++ ;
			ew->fade.g = 0.7f ;
		}
		break ;
	case 1 :
		if( stage_work.event == 2 ) ew->routine_0 ++ ;
		break ;
	case 2 :
		ew->fade.r += 0.0041666669f ;
		ew->fade.g -= 0.0029166667f ;
		if( ew->fade.r >= 1.0f ){
			ew->routine_0 ++ ;
			ew->fade.r = 1.0f ;
			ew->fade.g = 0.0f ;
		}
		break ;
	case 3 :
		break ;
	}
	if( ++ ew->timer0 >= 200 ) ew->timer0 = 0 ;
	if( ew->timer1 >= 360 ) ew->timer1 = 0 ;
	ew->timer1 += 10 ;
	obj_uv_begin( obj_stg->obj[48], ew->obj ) ;
	while( obj_uv_next() == 0 ){
		obj_uv_pos_get( &v ) ;
		if( v.y > 0.0f ){
			v.y += 1.0f * nlSin( DEG_ANG( ew->timer1 + v.x * 10.0f ) ) ;
		}
		obj_uv_pos_put( &v ) ;
		obj_uv_get( &x, &y ) ;
		x += 0.005f * ew->timer0 ;
		obj_uv_put( &x, &y ) ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_set08.c" End of File	*/

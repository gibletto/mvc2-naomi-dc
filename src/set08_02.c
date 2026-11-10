/* Stage 08's object 2: a glow model that comes on with the round, slides its texture and fades its
   colour through four steps. */
#include "player.h"

extern void obj_uv_begin( int *src, void *obj ) ;
extern int obj_uv_next( void ) ;
extern void obj_uv_get( Float *x, Float *y ) ;
extern void obj_uv_put( Float *x, Float *y ) ;
extern void obj_uv_clear( void *obj ) ;

static VEC3 set08_02_pos = { 0.0f, 2000.0f, -7000.0f } ;
static Sint32 set08_02_dur = 300 ;

/*==============================================================*/
static void set08_02_main( PLAYER_WORK *ew )
{
	Float x, y ;

	switch( ew->routine_0 ){
	case 0 :
		switch( stage_work.event ){
		case 1 :
			ew->routine_0 ++ ;
			ew->disp_flag = 1 ;
			ew->fade.r = 0.0f ;
			ew->fade.g = 0.0f ;
			ew->fade.b = 0.0f ;
			break ;
		}
		break ;
	case 1 :
		switch( ew->routine_1 ){
		case 0 :
			ew->fade.g += 0.0020833334f ;
			if( ew->fade.g >= 1.0f ){
				ew->routine_1 ++ ;
				ew->fade.r = 0.0f ;
				ew->fade.g = 1.0f ;
				ew->fade.b = 0.0f ;
			}
			break ;
		case 1 :
			switch( stage_work.event ){
			case 2 :
				ew->routine_1 ++ ;
				break ;
			}
			break ;
		case 2 :
			ew->fade.r += 1.0f / ( Float )set08_02_dur ;
			ew->fade.g -= 1.0f / ( Float )set08_02_dur ;
			ew->fade.b += 0.2f / ( Float )set08_02_dur ;
			if( ++ ew->timer1 >= set08_02_dur ){
				ew->routine_1 ++ ;
				ew->fade.r = 1.0f ;
				ew->fade.g = 0.2f ;
				ew->fade.b = 0.2f ;
			}
			break ;
		case 3 :
			break ;
		}
		ew->timer0 ++ ;
		if( ew->timer0 >= 1000 ) ew->timer0 = 0 ;
		obj_uv_begin( obj_stg->obj[11], ew->obj ) ;
		while( obj_uv_next() == 0 ){
			obj_uv_get( &x, &y ) ;
			y += 0.001f * ew->timer0 ;
			obj_uv_put( &x, &y ) ;
		}
		ew->rot[2] += 55 ;
		break ;
	}
}

/****************************************************************/
void set08_02_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 0 ;
		ew->work_func = set08_02_main ;
		ew->obj = obj_stg->obj[10] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Z | OBJ_FACE_Y | OBJ_FADE | OBJ_BRIGHT ;
		ew->pos = set08_02_pos ;
		obj_uv_clear( ew->obj ) ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set08_02.c" End of File	*/

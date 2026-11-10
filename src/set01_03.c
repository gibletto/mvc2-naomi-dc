/* Stage 01's fourth object (kind 5 works): two models that spin their
   texture's vertices in a swell, while a speed that falls to nothing is started again at random. */
#include "player.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

extern void obj_uv_begin( int *src, void *obj ) ;
extern int obj_uv_next( void ) ;
extern void obj_uv_pos_get( VEC3 *v ) ;
extern void obj_uv_pos_put( VEC3 *v ) ;

static VEC3 set01_03_pos = { 940.0f, 270.0f, -752.0f } ;

/*==============================================================*/
static void set01_03_main( PLAYER_WORK *ew )
{
	VEC3 v ;

	switch( ew->routine_0 ){
	case 0:
		ew->timer0 += 5 ;
		if( ew->timer0 >= 360 ) ew->timer0 = 0 ;
		ew->spd.x += ew->spd.y ;
		if( ew->spd.y > 0.0f ){
			ew->spd.y += -5.0e-06f ;
		}
		if( !( ew->spd.x > 5.0e-05f ) ){
			ew->spd.x = 0.0f ;
			ew->spd.y = fabs( nlRandom() * 0.0001f + nlRandom() * 0.0001f + -0.0001f ) ;
		}
		obj_uv_begin( obj_stg->obj[( ew->type << 1 ) + 8 + 1], ew->obj ) ;
		while( obj_uv_next() == 0 ){
			obj_uv_pos_get( &v ) ;
			if( v.y < 0.0f ){
				if( ew->type ){
					v.z -= ( nlSin( DEG_ANG( ( int )v.y + ew->timer0 ) ) + 1.0f ) * v.y * v.y * ew->spd.x * 3.0f ;
				} else {
					v.z -= ( nlSin( DEG_ANG( ( int )v.y + ew->timer0 ) ) + 1.0f ) * v.y * v.y * ew->spd.x ;
				}
			}
			obj_uv_pos_put( &v ) ;
		}
		break ;
	case 1:
		break ;
	}
}

/*==============================================================*/
static void set01_03_make( int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set01_03_main ;
		ew->obj = obj_stg->obj[no * 2 + 8] ;
		ew->pos = set01_03_pos ;
		ew->rot[1] = 0xdd29 ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_BRIGHT ;
		ew->type = no ;
	}
}

/****************************************************************/
void set01_03_set( void )
{
	int i ;

	for( i = 0 ; i < 2 ; i ++ ){
		set01_03_make( i ) ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set01_03.c" End of File	*/

/* Stage 01's sixth object (kind 5 works): a model that waits for the first
   side to be in the game, rises and falls back while its far vertices are squashed, and all the
   while a swell runs over another model's vertices. */
#include "player.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

extern void obj_uv_begin( int *src, void *obj ) ;
extern int obj_uv_next( void ) ;
extern void obj_uv_pos_get( VEC3 *v ) ;
extern void obj_uv_pos_put( VEC3 *v ) ;

static VEC3 set01_05_pos = { 840.0f, 290.0f, -860.0f } ;

static void set01_05_main( PLAYER_WORK *ew ) ;
static void set01_05_squash_b( PLAYER_WORK *ew ) ;
static void set01_05_squash_a( PLAYER_WORK *ew ) ;
static void set01_05_swell( PLAYER_WORK *ew ) ;

/****************************************************************/
void set01_05_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set01_05_main ;
		ew->obj = obj_stg->obj[5] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_BRIGHT ;
		ew->pos = set01_05_pos ;
		ew->rot[1] = 0xdd29 ;
	}
}

/*==============================================================*/
static void set01_05_main( PLAYER_WORK *ew )
{
	set01_05_swell( ew ) ;
	switch( ew->routine_0 ){
	case 0:
		if( -- ew->timer1 < 0 ){
			ew->timer1 = 0 ;
		} else {
			set01_05_squash_a( ew ) ;
		}
		if( side_flag_get( 0 ) ){
			ew->routine_0 ++ ;
			ew->timer1 = 0 ;
		}
		break ;
	case 1:
		ew->pos.y = set01_05_pos.y + ( 10.0f - ( Float )ew->timer1 * 0.25f ) * ew->timer1 * 0.5f ;
		if( ++ ew->timer1 >= 20 ){
			ew->routine_0 ++ ;
			ew->timer1 = 0 ;
		}
		break ;
	case 2:
		set01_05_squash_b( ew ) ;
		ew->pos.y = set01_05_pos.y - ( Float )ew->timer1 * ew->timer1 * 0.125f + 50.0f ;
		if( ++ ew->timer1 >= 20 ){
			ew->routine_0 = 0 ;
			ew->pos.y = set01_05_pos.y ;
		}
		break ;
	}
}

/*==============================================================*/
static void set01_05_squash_b( PLAYER_WORK *ew )
{
	VEC3 v ;

	obj_uv_begin( ew->obj, ew->obj ) ;
	while( obj_uv_next() == 0 ){
		obj_uv_pos_get( &v ) ;
		if( v.y < -50.0f ){
			v.y = v.y * ( 100.0f - ew->timer1 * 0.5f ) / 100.0f ;
			v.z = v.z * ( 100.0f + 5.0f * ew->timer1 ) / 100.0f ;
		}
		obj_uv_pos_put( &v ) ;
	}
}

/*==============================================================*/
static void set01_05_squash_a( PLAYER_WORK *ew )
{
	VEC3 v ;

	obj_uv_begin( ew->obj, ew->obj ) ;
	while( obj_uv_next() == 0 ){
		obj_uv_pos_get( &v ) ;
		if( v.y < -50.0f ){
			v.y = v.y * ( 100.0f - ew->timer1 * 0.5f ) / 100.0f ;
			v.z = v.z * ( 100.0f + 5.0f * ew->timer1 ) / 100.0f ;
		}
		obj_uv_pos_put( &v ) ;
	}
}

/*==============================================================*/
static void set01_05_swell( PLAYER_WORK *ew )
{
	VEC3 v ;

	ew->timer0 += 2 ;
	if( ew->timer0 >= 360 ) ew->timer0 = 0 ;
	ew->acc.x += ew->acc.y ;
	if( ew->acc.y > 0.0f ){
		ew->acc.y += -5.0e-07f ;
	}
	if( !( ew->acc.x > 5.0e-06f ) ){
		ew->acc.x = 0.0f ;
		ew->acc.y = nlRandom() * 2.0e-05f ;
	}
	obj_uv_begin( obj_stg->obj[6], ew->obj ) ;
	while( obj_uv_next() == 0 ){
		obj_uv_pos_get( &v ) ;
		if( v.y < 0.0f ){
			v.z -= ( nlSin( DEG_ANG( ( int )v.y + ew->timer0 + ( int )( v.x / 2.0f ) ) ) + 1.0f ) * v.y * v.y * ew->acc.x ;
		}
		obj_uv_pos_put( &v ) ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set01_05.c" End of File	*/

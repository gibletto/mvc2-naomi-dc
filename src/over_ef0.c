/* The game over scene's first work (kind 11): the scene's model 4, which
   moves to its place in 60 frames while the texture of model 5 ripples. */
#include "player.h"

/* degrees to an angle */
#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

extern void obj_uv_begin( int *src, void *obj ) ;
extern int obj_uv_next( void ) ;
extern void obj_uv_pos_get( VEC3 *v ) ;
extern void obj_uv_pos_put( VEC3 *v ) ;

/* where it starts and stops */
static const VEC3 over_ef0_pos_tbl[2] = {
	{ 25.0f, -1.0f, 0.0f },
	{ 25.0f, -1.0f, -30.0f },
} ;

static void over_ef0_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void over_ef0_move( PLAYER_WORK *ew )
{
	const VEC3 *p = &over_ef0_pos_tbl[0], *q = &over_ef0_pos_tbl[1] ;

	ew->pos.x = p[0].x + ( q[0].x - p[0].x ) * ew->timer0 / 60.0f ;
	ew->pos.y = p[0].y + ( q[0].y - p[0].y ) * ew->timer0 / 60.0f ;
	ew->pos.z = p[0].z + ( q[0].z - p[0].z ) * ew->timer0 / 60.0f ;
	ew->pos.x /= 10.0f ;
	ew->pos.y /= 10.0f ;
	ew->pos.z /= 10.0f ;
}

/****************************************************************/
void over_ef0_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SYS, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = over_ef0_main ;
		ew->obj = obj_dm0b->obj[4] ;
		ew->pos = over_ef0_pos_tbl[0] ;
		ew->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_BRIGHT ;
		ew->timer0 = 0 ;
		ew->timer1 = 0 ;
		ew->scale.x = 0.1f ;
		ew->scale.y = 0.1f ;
		ew->scale.z = 0.1f ;
	}
}

/*==============================================================*/
static void over_ef0_main( PLAYER_WORK *ew )
{
	VEC3 v ;
	int off ;

	if( Sys->scene_no != 5 ){
		work_free( ew ) ;
		return ;
	}
	if( ew->timer0 != 60 ){
		ew->timer0 ++ ;
		over_ef0_move( ew ) ;
	} else {
		ew->timer1 += 20 ;
		if( ew->timer1 >= 360 ) ew->timer1 = 0 ;
		obj_uv_begin( obj_dm0b->obj[5], ew->obj ) ;
		off = 0 ;
		while( obj_uv_next() == 0 ){
			obj_uv_pos_get( &v ) ;
			v.z += v.y * v.y * nlSin( DEG_ANG( ew->timer1 + off ) ) * 0.02f ;
			off += 20 ;
			obj_uv_pos_put( &v ) ;
		}
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "over_ef0.c" End of File	*/

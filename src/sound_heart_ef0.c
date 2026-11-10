/* The Sound_Heart scene's first work (kind 11): the scene's model 0, which
   moves and turns to its place in 60 frames while the texture of model 1 ripples. */
#include "player.h"

/* degrees to an angle */
#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

extern void obj_uv_begin( int *src, void *obj ) ;
extern int obj_uv_next( void ) ;
extern void obj_uv_pos_get( VEC3 *v ) ;
extern void obj_uv_pos_put( VEC3 *v ) ;

/* where it starts and stops */
static const VEC3 sound_heart_ef0_pos_tbl[2] = {
	{ 20.0f, 0.0f, 0.0f },
	{ -18.0f, 0.0f, -20.0f },
} ;
/* its turns, in degrees */
static const VEC3 sound_heart_ef0_rot_tbl[2] = {
	{ 0.0f, -90.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f },
} ;

static void sound_heart_ef0_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void sound_heart_ef0_move( PLAYER_WORK *ew )
{
	const VEC3 *p0 = &sound_heart_ef0_pos_tbl[0] ;
	const VEC3 *p1 = &sound_heart_ef0_pos_tbl[1] ;
	const VEC3 *r0 = &sound_heart_ef0_rot_tbl[0] ;
	const VEC3 *r1 = &sound_heart_ef0_rot_tbl[1] ;

	ew->pos.x = (*p0).x + ( ( (*p1).x - (*p0).x ) / 60.0f ) * ew->timer0 ;
	ew->pos.y = (*p0).y + ( ( (*p1).y - (*p0).y ) / 60.0f ) * ew->timer0 ;
	ew->pos.z = (*p0).z + ( ( (*p1).z - (*p0).z ) / 60.0f ) * ew->timer0 ;
	ew->rot[0] = DEG_ANG( (*r0).x + ( ( (*r1).x - (*r0).x ) / 60.0f ) * ew->timer0 ) ;
	ew->rot[1] = DEG_ANG( (*r0).y + ( ( (*r1).y - (*r0).y ) / 60.0f ) * ew->timer0 ) ;
	ew->rot[2] = DEG_ANG( (*r0).z + ( ( (*r1).z - (*r0).z ) / 60.0f ) * ew->timer0 ) ;
}

/****************************************************************/
void sound_heart_ef0_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SYS, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = sound_heart_ef0_main ;
		ew->obj = obj_dm04->obj[0] ;
		ew->pos = sound_heart_ef0_pos_tbl[0] ;
		ew->rot[0] = DEG_ANG( sound_heart_ef0_rot_tbl[0].x ) ;
		ew->rot[1] = DEG_ANG( sound_heart_ef0_rot_tbl[0].y ) ;
		ew->rot[2] = DEG_ANG( sound_heart_ef0_rot_tbl[0].z ) ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y ;
		ew->timer0 = 0 ;	/* a dead store */
		ew->timer0 = 1 ;
	}
}

/*==============================================================*/
static void sound_heart_ef0_main( PLAYER_WORK *ew )
{
	VEC3 v ;
	int off ;

	if( ew->timer0 != 60 ){
		ew->timer0 ++ ;
		sound_heart_ef0_move( ew ) ;
	}
	ew->timer1 += 20 ;
	if( ew->timer1 >= 360 ) ew->timer1 = 0 ;
	obj_uv_begin( obj_dm04->obj[1], ew->obj ) ;
	off = 0 ;
	while( obj_uv_next() == 0 ){
		obj_uv_pos_get( &v ) ;
		v.z += v.y * v.y * nlSin( DEG_ANG( ew->timer1 + off ) ) * 0.02f ;
		off += 20 ;
		obj_uv_pos_put( &v ) ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "sound_heart_ef0.c" End of File	*/

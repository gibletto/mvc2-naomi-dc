/* The Sound_Heart scene's eight pictures (kind 11): sound_heart.c makes one
   for each of its models, which slides in 30 frames to the place its table gives; the first two
   also slide their textures, one way and the other. */
#include "player.h"

/* degrees to an angle */
#define DEG_ANG( n )	( 0xffff & ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) )

extern void obj_uv_begin( int *src, void *obj ) ;
extern int obj_uv_next( void ) ;
extern void obj_uv_get( Float *x, Float *y ) ;
extern void obj_uv_put( Float *x, Float *y ) ;
extern void obj_uv_clear( void *obj ) ;

/* where each starts and stops */
static const VEC3 sound_heart_ef1_pos_tbl[16] = {
	{ -150.0f, 25.834f, -36.995f },
	{ 0.0f, 25.834f, -36.995f },
	{ 150.0f, 20.196f, -34.903f },
	{ 0.0f, 20.196f, -34.903f },
	{ -150.0f, 14.557f, -32.851f },
	{ 0.0f, 14.557f, -32.851f },
	{ 150.0f, 8.919f, -30.799f },
	{ 0.0f, 8.919f, -30.799f },
	{ -150.0f, 3.281f, -28.746f },
	{ 0.0f, 3.281f, -28.746f },
	{ 150.0f, -2.357f, -26.694f },
	{ 0.0f, -2.357f, -26.694f },
	{ -150.0f, -7.995f, -24.642f },
	{ 0.0f, -7.995f, -24.642f },
	{ 150.0f, -13.663f, -22.59f },
	{ 0.0f, -13.663f, -22.59f },
} ;
static const VEC3 sound_heart_ef1_rot = { -20.0f, 0.0f, 0.0f } ;
/* which of the scene's models each shows */
static const Uint8 sound_heart_ef1_obj_tbl[8] = { 4, 2, 4, 2, 4, 2, 4, 2 } ;

static void sound_heart_ef1_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void sound_heart_ef1_move( PLAYER_WORK *ew )
{
	const VEC3 *p0 = &sound_heart_ef1_pos_tbl[ew->type * 2] ;
	const VEC3 *p1 = &sound_heart_ef1_pos_tbl[ew->type * 2 + 1] ;

	ew->pos.x = (*p0).x + ( ( (*p1).x - (*p0).x ) / 30.0f ) * ew->timer0 ;
	ew->pos.y = (*p0).y + ( ( (*p1).y - (*p0).y ) / 30.0f ) * ew->timer0 ;
	ew->pos.z = (*p0).z + ( ( (*p1).z - (*p0).z ) / 30.0f ) * ew->timer0 ;
}

/****************************************************************/
void sound_heart_ef1_set( int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SYS, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->type = no ;
		ew->work_func = sound_heart_ef1_main ;
		ew->dir = sound_heart_ef1_obj_tbl[no] ;
		ew->obj = obj_dm04->obj[ew->dir] ;
		ew->pos = sound_heart_ef1_pos_tbl[no * 2] ;
		ew->rot[0] = DEG_ANG( sound_heart_ef1_rot.x ) ;
		ew->rot[1] = DEG_ANG( sound_heart_ef1_rot.y ) ;
		ew->rot[2] = DEG_ANG( sound_heart_ef1_rot.z ) ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X ;
		ew->timer0 = 0 ;
		ew->timer1 = 0 ;
		if( no >= 2 ) return ;
		obj_uv_clear( ew->obj ) ;
	}
}

/*==============================================================*/
static void sound_heart_ef1_main( PLAYER_WORK *ew )
{
	Float x, y ;

	if( ew->timer0 != 30 ){
		ew->timer0 ++ ;
		sound_heart_ef1_move( ew ) ;
	}
	if( ew->type >= 2 ) return ;
	ew->timer1 ++ ;
	if( ew->timer1 >= 200 ) ew->timer1 = 0 ;
	obj_uv_begin( obj_dm04->obj[ew->dir + 1], ew->obj ) ;
	while( obj_uv_next() == 0 ){
		obj_uv_get( &x, &y ) ;
		if( ew->type & 1 ) x += ew->timer1 * 0.005f ;
		else x += 1.0f - ew->timer1 * 0.005f ;
		obj_uv_put( &x, &y ) ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "sound_heart_ef1.c" End of File	*/

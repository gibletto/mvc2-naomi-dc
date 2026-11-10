/* The game over scene's fourteen pictures (kind 11): over.c makes one for
   each, which slides in 90 frames to the place its table gives; the first two of each seven also
   ripple their textures. */
#include "player.h"

/* degrees to an angle */
#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

extern void obj_uv_begin( int *src, void *obj ) ;
extern int obj_uv_next( void ) ;
extern void obj_uv_get( Float *x, Float *y ) ;
extern void obj_uv_put( Float *x, Float *y ) ;
extern void obj_uv_clear( void *obj ) ;

/* where each starts and stops */
static const VEC3 over_ef1_pos_tbl[28] = {
	{ 140.0f, -20.0f, -30.0f },
	{ 0.0f, -20.0f, -30.0f },
	{ -140.0f, -17.1064f, -34.0777f },
	{ 0.0f, -17.1064f, -34.0777f },
	{ 140.0f, -14.2129f, -38.1553f },
	{ 0.0f, -14.2129f, -38.1553f },
	{ -140.0f, -11.3193f, -42.233f },
	{ 0.0f, -11.3193f, -42.233f },
	{ 140.0f, -8.4258f, -46.3106f },
	{ 0.0f, -8.4258f, -46.3106f },
	{ -140.0f, -5.5322f, -50.3883f },
	{ 0.0f, -5.5322f, -50.3883f },
	{ 140.0f, -2.6386f, -54.466f },
	{ 0.0f, -2.6386f, -54.466f },
	{ -140.0f, 2.6386f, -54.466f },
	{ 0.0f, 2.6386f, -54.466f },
	{ 140.0f, 5.5322f, -50.3883f },
	{ 0.0f, 5.5322f, -50.3883f },
	{ -140.0f, 8.4258f, -46.3106f },
	{ 0.0f, 8.4258f, -46.3106f },
	{ 140.0f, 11.3193f, -42.233f },
	{ 0.0f, 11.3193f, -42.233f },
	{ -140.0f, 14.2129f, -38.1553f },
	{ 0.0f, 14.2129f, -38.1553f },
	{ 140.0f, 17.1064f, -34.0777f },
	{ 0.0f, 17.1064f, -34.0777f },
	{ -140.0f, 20.0f, -30.0f },
	{ 0.0f, 20.0f, -30.0f },
} ;
/* the turns, in degrees, of the first seven and of the rest */
static const VEC3 over_ef1_rot_tbl[2] = {
	{ 0.0f, 0.0f, 0.0f },
	{ 109.28f, 0.0f, 0.0f },
} ;
/* which of the scene's models each shows */
static const Uint8 over_ef1_obj_tbl[14] = { 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2 } ;

static void over_ef1_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void over_ef1_move( PLAYER_WORK *ew )
{
	const VEC3 *p = &over_ef1_pos_tbl[ew->type * 2], *q = &over_ef1_pos_tbl[ew->type * 2 + 1] ;

	ew->pos.x = p->x + ( q->x - p->x ) * ew->timer0 / 90.0f ;
	ew->pos.y = p->y + ( q->y - p->y ) * ew->timer0 / 90.0f ;
	ew->pos.z = p->z + ( q->z - p->z ) * ew->timer0 / 90.0f ;
	ew->pos.x /= 10.0f ;
	ew->pos.y /= 10.0f ;
	ew->pos.z /= 10.0f ;
}

/****************************************************************/
void over_ef1_set( int no )
{
	PLAYER_WORK *ew ;
	int half ;

	if( ( ew = work_get( 0, W_SYS, LIST_END ) ) ){
		half = ( no >= 7 ) ? 1 : 0 ;
		ew->disp_flag = 1 ;
		ew->type = no ;
		ew->work_func = over_ef1_main ;
		ew->dir = over_ef1_obj_tbl[no] ;
		ew->obj = obj_dm0b->obj[ew->dir] ;
		ew->pos = over_ef1_pos_tbl[no * 2] ;
		ew->rot[0] = DEG_ANG( over_ef1_rot_tbl[half].x ) ;
		ew->rot[1] = DEG_ANG( over_ef1_rot_tbl[half].y ) ;
		ew->rot[2] = DEG_ANG( over_ef1_rot_tbl[half].z ) ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_SCALE | OBJ_BRIGHT ;
		ew->timer0 = 0 ;
		ew->timer1 = 0 ;
		ew->scale.x = 0.1f ;
		ew->scale.y = 0.1f ;
		ew->scale.z = 0.1f ;
		if( no <= 1 ) obj_uv_clear( ew->obj ) ;
	}
}

/*==============================================================*/
static void over_ef1_main( PLAYER_WORK *ew )
{
	Float py, px ;

	if( Sys->scene_no != 5 ){
		work_free( ew ) ;
		return ;
	}
	if( ew->timer0 != 90 ){
		ew->timer0 ++ ;
		over_ef1_move( ew ) ;
		return ;
	}
	if( ew->type > 1 ) return ;
	ew->timer1 ++ ;
	if( ew->timer1 >= 200 ) ew->timer1 = 0 ;
	obj_uv_begin( obj_dm0b->obj[ew->dir + 1], ew->obj ) ;
	while( obj_uv_next() == 0 ){
		obj_uv_get( &px, &py ) ;
		py += 1.0f - ew->timer1 * 0.005f ;
		obj_uv_put( &px, &py ) ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "over_ef1.c" End of File	*/

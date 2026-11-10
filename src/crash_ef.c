/* The burst of rubble where something crashes down (kind 7 works, nothing in the common code calls
   it): a ring that swells, then nine chips that fly up and bounce on the ground. */
#include "player.h"

extern void move_pos_spd( PLAYER_WORK *ew );
/* Capcom's library */

/* two vectors of the ring: one the code does not use, and its size; the chips' acceleration */
static VEC3 crash_unit = { 1.0f, 1.0f, 1.0f } ;
static VEC3 crash_scale = { 1.0f, 1.0f, 1.0f } ;
static VEC3 crash_acc = { 0.0f, -2.0f, 0.0f } ;

/* the ring's size, then its three sizes and its fade, by frame */
static MOTION crash_x_key[] = {
	{ 0, 0.2f }, { 9, 1.2f },
} ;
static MOTION crash_y_key[] = {
	{ 0, 0.2f }, { 2, 1.2f }, { 9, 0.0f },
} ;
static MOTION crash_z_key[] = {
	{ 0, 0.2f }, { 9, 1.2f },
} ;
static MOTION crash_fade_key[] = {
	{ 0, 1.0f }, { 6, 0.5f }, { 9, 0.0f },
} ;

/*==============================================================*/
static void crash_ring_main( PLAYER_WORK *ew )
{
	if( ew->timer0 >= 9 ){
		work_free( ew ) ;
		return ;
	}
	ew->scale.x = get_motion( crash_x_key, &ew->routine_0, ew->timer0 ) ;
	ew->scale.y = get_motion( crash_y_key, &ew->routine_1, ew->timer0 ) ;
	ew->scale.z = get_motion( crash_z_key, &ew->routine_2, ew->timer0 ) ;
	ew->fade.r = get_motion( crash_fade_key, &ew->routine_3, ew->timer0 ) ;
	ew->fade.g = ew->fade.r ;
	ew->fade.b = ew->fade.r ;
	ew->timer0 ++ ;
}

/*==============================================================*/
static void crash_ring_set( VEC3 *pos )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = crash_ring_main ;
		ew->obj = obj_efky->obj[81] ;
		ew->obj_flag = OBJ_POS | OBJ_SCALE ;
		ew->pos = *pos ;
	}
}

/*==============================================================*/
static void crash_chip_main( PLAYER_WORK *ew )
{
	move_pos_spd( ew ) ;
	if( ew->pos.y < 0.0f ){
		ew->spd.y *= -0.5f ;
		ew->pos.y = 0.0f ;
	}
	if( ++ ew->timer0 >= 60 ){
		work_free( ew ) ;
		return ;
	}
}

/*==============================================================*/
static void crash_chip_set( VEC3 *pos, int no )
{
	PLAYER_WORK *ew ;
	int deg ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = crash_chip_main ;
		ew->obj = obj_efky->obj[no + 72] ;
		ew->obj_flag = OBJ_POS ;
		ew->pos = *pos ;
		ew->type = no ;
		deg = no * 360 / 9 + 20 ;
		ew->spd.x = nlSin( ( int )( deg * 65536.0f / 360.0f + 0.5f ) & 0xffff ) * 10.0f ;
		ew->spd.y = nlRandom() * 20.0f + 10.0f ;
		ew->spd.z = nlCos( ( int )( deg * 65536.0f / 360.0f + 0.5f ) & 0xffff ) * 10.0f ;
		ew->acc = crash_acc ;
	}
}

/*==============================================================*/
static void crash_main( PLAYER_WORK *ew )
{
	int i ;

	ew->timer0 ++ ;
	if( ew->timer0 == 6 ) crash_ring_set( &ew->pos ) ;
	if( ew->timer0 > 15 ){
		for( i = 0 ; i < 9 ; i ++ ) crash_chip_set( &ew->pos, i ) ;
		work_free( ew ) ;
		return ;
	}
}

/****************************************************************/
void crash_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = crash_main ;
		ew->obj = obj_efky->obj[71] ;
		ew->obj_flag = OBJ_POS | OBJ_SCALE ;
		ew->pos = pl_w->pos ;
		ew->pos.y += 100.0f ;
		ew->scale = crash_scale ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "crash_ef.c" End of File	*/

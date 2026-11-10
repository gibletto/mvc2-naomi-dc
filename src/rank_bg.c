/* The ranking screen's scenery (kind 5 works): three models of the ranking's
   file, two that turn on the spot while their texture slides, and one that circles. They fade in
   a little once the scene has begun. */
#include "player.h"

extern void obj_uv_begin( int *src, void *obj ) ;
extern int obj_uv_next( void ) ;
extern void obj_uv_get( Float *x, Float *y ) ;
extern void obj_uv_put( Float *x, Float *y ) ;
extern void obj_uv_clear( void *obj ) ;

#define RANK_BG_OBJ( w )	( ( w )->dir )	/* the model's number in the ranking's file */

/* the model of each type, where it stands, and the turn it starts at in degrees */
static Uint8 rank_bg_obj_tbl[3] = { 0, 2, 7 } ;
static VEC3 rank_bg_pos_tbl[3] = {
	{ 5.06f, 4.12f, -8.6f },
	{ 5.06f, 4.12f, -8.6f },
	{ 1421.1f, 1154.8f, -2410.5f },
} ;
static VEC3 rank_bg_rot_tbl[3] = {
	{ 227.81f, -52.74f, -140.94f },
	{ 227.81f, -52.74f, -140.94f },
	{ 227.81f, -52.74f, -140.94f },
} ;

static void rank_bg_main( PLAYER_WORK *ew ) ;

/****************************************************************/
void rank_bg_set( int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->type = no ;
		ew->work_func = rank_bg_main ;
		RANK_BG_OBJ( ew ) = rank_bg_obj_tbl[no] ;
		ew->obj = obj_dm06->obj[RANK_BG_OBJ( ew )] ;
		ew->pos = rank_bg_pos_tbl[no] ;
		ew->rot[0] = ( int )( rank_bg_rot_tbl[no].x * 65536.0f / 360.0f + 0.5f ) & 0xffff ;
		ew->rot[1] = ( int )( rank_bg_rot_tbl[no].y * 65536.0f / 360.0f + 0.5f ) & 0xffff ;
		ew->rot[2] = ( int )( rank_bg_rot_tbl[no].z * 65536.0f / 360.0f + 0.5f ) & 0xffff ;
		switch( no ){
		case 0:
		case 1:
			obj_uv_clear( ew->obj ) ;
			ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_ROT_Z | OBJ_BRIGHT ;
			ew->timer1 = 0 ;
			break ;
		case 2:
			ew->obj_flag = OBJ_BRIGHT | OBJ_MTX ;
			ew->timer0 = 64 ;
			ew->timer1 = 3 ;
			ew->spd.x = 48.0f ;
			break ;
		}
		ew->obj_flag |= OBJ_FADE ;
		ew->fade.r = 1.0f ;
		ew->fade.g = 1.0f ;
		ew->fade.b = 1.0f ;
	}
}

static void rank_bg_turn0( PLAYER_WORK *ew ) ;
static void rank_bg_turn1( PLAYER_WORK *ew ) ;
static void rank_bg_fly( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void rank_bg_main( PLAYER_WORK *ew )
{
	static void ( *rank_bg_jmp[] )( PLAYER_WORK *ew ) = {
		rank_bg_turn0, rank_bg_turn1, rank_bg_fly,
	} ;

	if( Sys->scene_no >= 3 ){
		if( ew->fade.r >= 0.5f ){
			ew->fade.r -= 0.01f ;
			ew->fade.g = ew->fade.r ;
			ew->fade.b = ew->fade.r ;
		}
	}
	rank_bg_jmp[ew->type]( ew ) ;
}

/*==============================================================*/
/*			It turns one way about x and its texture slides		*/
/*==============================================================*/
static void rank_bg_turn0( PLAYER_WORK *ew )
{
	Float px, py ;

	ew->rot[0] += 0x100 ;
	ew->timer1 ++ ;
	if( ew->timer1 >= 200 ) ew->timer1 = 0 ;
	obj_uv_begin( obj_dm06->obj[RANK_BG_OBJ( ew ) + 1], ew->obj ) ;
	while( obj_uv_next() == 0 ){
		obj_uv_get( &px, &py ) ;
		py += 1.0f - ew->timer1 * 0.005f ;
		obj_uv_put( &px, &py ) ;
	}
}

/*==============================================================*/
/*			The same the other way								*/
/*==============================================================*/
static void rank_bg_turn1( PLAYER_WORK *ew )
{
	Float px, py ;

	ew->rot[0] -= 0x100 ;
	ew->timer1 ++ ;
	if( ew->timer1 >= 200 ) ew->timer1 = 0 ;
	obj_uv_begin( obj_dm06->obj[RANK_BG_OBJ( ew ) + 1], ew->obj ) ;
	while( obj_uv_next() == 0 ){
		obj_uv_get( &px, &py ) ;
		py += 1.0f - ew->timer1 * 0.005f ;
		obj_uv_put( &px, &py ) ;
	}
}

/*==============================================================*/
/*			It circles: back to its place every 64 frames,		*/
/*			and gone after three rounds							*/
/*==============================================================*/
static void rank_bg_fly( PLAYER_WORK *ew )
{
	if( -- ew->timer0 == 0 ){
		ew->timer0 = 64 ;
		ew->pos = rank_bg_pos_tbl[ew->type] ;
		if( -- ew->timer1 == 0 ){
			work_free( ew ) ;
			return ;
		}
	}
	nlPushMatrix( 0 ) ;
	nlLoadMatrix( &ew->obj_mtx ) ;
	nlUnitMatrix() ;
	nlTranslateV( &ew->pos ) ;
	nlRotateAZ( ew->rot[2] ) ;
	nlRotateAY( ew->rot[1] ) ;
	nlRotateAX( ew->rot[0] ) ;
	nlTranslate( ew->spd.x, 0.0f, 0.0f ) ;
	nlGetMatrix( &ew->obj_mtx ) ;
	ew->pos.x = ew->obj_mtx.m[3][0] ;
	ew->pos.y = ew->obj_mtx.m[3][1] ;
	ew->pos.z = ew->obj_mtx.m[3][2] ;
	nlPopMatrix( 1 ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "rank_bg.c" End of File	*/

/* The ranking screen's marks (kind 5 works): four models of the ranking's file
   that wait for the page to finish turning, then slide in to a common place (types 0 and 3), grow
   out of nothing (type 1) or blink (type 2). */
#include "player.h"

#define RANK_MARK_NO( w )	( ( w )->dir )	/* which of the marks of its type */

/* the model of each type, where the type's marks start (type 2 has one) and the turn they start at, in degrees */
static Uint8 rank_mark_obj_tbl[4] = { 17, 21, 20, 28 } ;
static VEC3 rank_mark_pos_tbl[5] = {
	{ -34.3f, -27.66f, 53.85f },
	{ -34.0f, -28.13f, 53.82f },
	{ -33.68f, -28.65f, 53.68f },
	{ -33.66f, -29.34f, 53.44f },
	{ -33.78f, -30.0f, 53.15f },
} ;
static VEC3 rank_mark_pos1_tbl[5] = {
	{ -34.51f, -27.95f, 54.09f },
	{ -34.16f, -28.44f, 54.03f },
	{ -33.8f, -28.93f, 53.86f },
	{ -33.89f, -29.7f, 53.58f },
	{ -34.02f, -30.36f, 53.36f },
} ;
static VEC3 rank_mark_pos2 = { -32.93f, -26.15f, 52.25f } ;
static VEC3 rank_mark_rot_tbl[5] = {
	{ -7.8f, 54.44f, 39.23f },
	{ 13.52f, 54.44f, 39.23f },
	{ 30.71f, 54.44f, 39.23f },
	{ 39.31f, 54.44f, 39.23f },
	{ 45.72f, 54.44f, 39.23f },
} ;
static VEC3 rank_mark_rot2 = { 33.8f, 54.09f, 38.99f } ;

static void rank_mark_main( PLAYER_WORK *ew ) ;

/****************************************************************/
void rank_mark_set( int no, int a )
{
	PLAYER_WORK *ew ;
	VEC3 *pos, *rot ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 0 ;
		ew->type = no ;
		RANK_MARK_NO( ew ) = a ;
		ew->work_func = rank_mark_main ;
		ew->obj = obj_dm06->obj[rank_mark_obj_tbl[no]] ;
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
		ew->scale.z = 1.0f ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_ROT_Z | OBJ_SCALE | OBJ_BRIGHT ;
		ew->timer0 = 0 ;
		ew->timer1 = 30 ;
		switch( no ){
		case 0:
		case 3:
			pos = &rank_mark_pos_tbl[a] ;
			rot = &rank_mark_rot_tbl[a] ;
			break ;
		case 1:
			pos = &rank_mark_pos1_tbl[a] ;
			rot = &rank_mark_rot_tbl[a] ;
			break ;
		case 2:
			pos = &rank_mark_pos2 ;
			rot = &rank_mark_rot2 ;
			break ;
		}
		ew->pos = *pos ;
		ew->rot[0] = ( int )( rot->x * 65536.0f / 360.0f + 0.5f ) & 0xffff ;
		ew->rot[1] = ( int )( rot->y * 65536.0f / 360.0f + 0.5f ) & 0xffff ;
		ew->rot[2] = ( int )( rot->z * 65536.0f / 360.0f + 0.5f ) & 0xffff ;
	}
}

static void rank_mark_slide( PLAYER_WORK *ew ) ;
static void rank_mark_grow( PLAYER_WORK *ew ) ;
static void rank_mark_blink( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void rank_mark_main( PLAYER_WORK *ew )
{
	static void ( *rank_mark_jmp[] )( PLAYER_WORK *ew ) = {
		rank_mark_slide, rank_mark_grow, rank_mark_blink, rank_mark_slide,
	} ;

	if( Sys->routine_0 != 4 || Sys->join_flag ){
		work_free( ew ) ;
		return ;
	}
	if( Sys->timer3 == 3 ) rank_mark_jmp[ew->type]( ew ) ;
}

/*==============================================================*/
/*			It slides from its start to the camera's eye, a		*/
/*			thirtieth of the way for each frame					*/
/*==============================================================*/
static void rank_mark_slide( PLAYER_WORK *ew )
{
	VEC3 *from ;
	VEC3 *eye ;

	ew->disp_flag = 1 ;
	if( ew->timer1 ){
		ew->timer1 -- ;
		from = &rank_mark_pos_tbl[RANK_MARK_NO( ew )] ;
		eye = scr_work.eye ;
		ew->pos.x = from->x + ( eye->x - from->x ) / 30.0f * ew->timer1 ;
		ew->pos.y = from->y + ( eye->y - from->y ) / 30.0f * ew->timer1 ;
		ew->pos.z = from->z + ( eye->z - from->z ) / 30.0f * ew->timer1 ;
	}
}

/*==============================================================*/
/*			It waits, then widens to 128 times in 30 frames		*/
/*==============================================================*/
static void rank_mark_grow( PLAYER_WORK *ew )
{
	if( ew->timer1 ){
		ew->timer1 -- ;
		return ;
	}
	ew->disp_flag = 1 ;
	if( ew->timer0 == 30 ){
		ew->scale.x = 128.2f ;
	} else {
		ew->timer0 ++ ;
		ew->scale.x = ew->timer0 * 127.2f / 30.0f + 1.0f ;
	}
}

/*==============================================================*/
static void rank_mark_blink( PLAYER_WORK *ew )
{
	if( ew->timer0 ++ == 30 ){
		ew->timer0 = 0 ;
		ew->disp_flag ^= 1 ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "rank_mark.c" End of File	*/

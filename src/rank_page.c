/* The ranking screen's pages (kind 5 works): each page is a model of the
   ranking's file that slides in from its start to its place over 30 frames, then turns on the spot
   (types 0 and 1) or with the continue count (type 2) while its texture slides. */
#include "player.h"

extern void obj_uv_begin( int *src, void *obj ) ;
extern int obj_uv_next( void ) ;
extern void obj_uv_get( Float *x, Float *y ) ;
extern void obj_uv_put( Float *x, Float *y ) ;
extern void obj_uv_clear( void *obj ) ;

#define RANK_PAGE_OBJ( w )	( ( w )->dir )	/* the model's number in the ranking's file */

/* the model of each page, where it starts and ends (the type picks the place), and its turn in degrees */
#ifdef TARGET_DC	/* the home game's second ranking has the first's pages */
static Uint8 rank_page_obj_tbl[16] = { 8, 10, 12, 15, 14, 8, 10, 12, 15, 14, 31, 29, 30, 0, 0, 0 } ;
#else
static Uint8 rank_page_obj_tbl[16] = { 8, 10, 12, 15, 14, 22, 24, 18, 26, 14, 31, 29, 30, 0, 0, 0 } ;
#endif
static VEC3 rank_page_pos_tbl[8] = {
	{ -30.44f, -24.4f, 53.71f },
	{ -29.89f, -25.22f, 53.66f },
	{ -28.95f, -24.97f, 44.79f },
	{ -30.07f, -23.77f, 46.99f },
	{ -30.07f, -23.77f, 46.99f },
	{ -30.13f, -24.71f, 53.91f },
	{ -30.13f, -24.71f, 53.91f },
	{ -30.13f, -24.71f, 53.91f },
} ;
static VEC3 rank_page_rot_tbl[8] = {
	{ 32.5f, 54.23f, 39.31f },
	{ 32.5f, 54.23f, 39.31f },
	{ 33.68f, 54.22f, 38.92f },
	{ 30.71f, 54.44f, 39.23f },
	{ 30.71f, 54.44f, 39.23f },
	{ 25.9f, 53.8f, 38.4f },
	{ 24.8f, 52.0f, 37.0f },
	{ 24.8f, 52.0f, 37.0f },
} ;
static VEC3 rank_page_end_tbl[8] = {
	{ -33.59f, -24.46f, 51.93f },
	{ -33.03f, -25.28f, 51.88f },
	{ -32.45f, -27.8f, 51.04f },
	{ -32.22f, -25.7f, 51.28f },
	{ -32.7f, -25.91f, 51.73f },
	{ -33.32f, -25.08f, 51.74f },
	{ -33.32f, -25.08f, 51.74f },
	{ -33.32f, -25.08f, 51.74f },
} ;

static void rank_page_main( PLAYER_WORK *ew ) ;

/****************************************************************/
void rank_page_set( int no, int a )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 0 ;
		ew->type = no ;
		ew->work_func = rank_page_main ;
		RANK_PAGE_OBJ( ew ) = rank_page_obj_tbl[a] ;
		ew->obj = obj_dm06->obj[RANK_PAGE_OBJ( ew )] ;
		ew->pos = rank_page_pos_tbl[no] ;
		ew->rot[0] = ( int )( rank_page_rot_tbl[no].x * 65536.0f / 360.0f + 0.5f ) & 0xffff ;
		ew->rot[1] = ( int )( rank_page_rot_tbl[no].y * 65536.0f / 360.0f + 0.5f ) & 0xffff ;
		ew->rot[2] = ( int )( rank_page_rot_tbl[no].z * 65536.0f / 360.0f + 0.5f ) & 0xffff ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_ROT_Z | OBJ_BRIGHT ;
		ew->timer0 = 0 ;
		ew->timer1 = 0 ;
		if( no >= 4 ) return ;
		obj_uv_clear( ew->obj ) ;
	}
}

/*==============================================================*/
/*			Its place, a thirtieth of the way from the start	*/
/*			to the end for each frame							*/
/*==============================================================*/
static void rank_page_slide( PLAYER_WORK *ew )
{
	VEC3 *from = &rank_page_pos_tbl[ew->type] ;
	VEC3 *to = &rank_page_end_tbl[ew->type] ;

	ew->pos.x = from->x + ( to->x - from->x ) / 30.0f * ew->timer0 ;
	ew->pos.y = from->y + ( to->y - from->y ) / 30.0f * ew->timer0 ;
	ew->pos.z = from->z + ( to->z - from->z ) / 30.0f * ew->timer0 ;
}

static void rank_page_turn( PLAYER_WORK *ew ) ;
static void rank_page_count( PLAYER_WORK *ew ) ;
static void rank_page_nop( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void rank_page_main( PLAYER_WORK *ew )
{
	static void ( *rank_page_jmp[] )( PLAYER_WORK *ew ) = {
		rank_page_turn, rank_page_turn, rank_page_count, rank_page_nop,
		rank_page_nop, rank_page_nop, rank_page_nop, rank_page_nop,
	} ;

	if( Sys->routine_0 != 4 || Sys->join_flag ){
		work_free( ew ) ;
		return ;
	}
	if( !ew->routine_0 ){
		if( !Sys->timer3 ) return ;
		ew->disp_flag = 1 ;
		if( ew->timer0 >= 30 ){
			ew->routine_0 ++ ;
			ew->pos = rank_page_end_tbl[ew->type] ;
			return ;
		}
		ew->timer0 ++ ;
		rank_page_slide( ew ) ;
	} else {
		rank_page_jmp[ew->type]( ew ) ;
	}
}

/*==============================================================*/
/*			The page turns on the spot and its texture			*/
/*			slides												*/
/*==============================================================*/
static void rank_page_turn( PLAYER_WORK *ew )
{
	Float y, x ;

	ew->timer1 ++ ;
	if( ew->timer1 >= 200 ) ew->timer1 = 0 ;
	obj_uv_begin( obj_dm06->obj[RANK_PAGE_OBJ( ew ) + 1], ew->obj ) ;
	while( obj_uv_next() == 0 ){
		obj_uv_get( &x, &y ) ;
		x += ew->timer1 * 0.005f ;
		y += ew->timer1 * 0.005f ;
		obj_uv_put( &x, &y ) ;
	}
}

/*==============================================================*/
/*			The texture slides with the continue count			*/
/*==============================================================*/
static void rank_page_count( PLAYER_WORK *ew )
{
	Float px, py ;

	obj_uv_begin( obj_dm06->obj[RANK_PAGE_OBJ( ew ) + 1], ew->obj ) ;
	while( obj_uv_next() == 0 ){
		obj_uv_get( &px, &py ) ;
		py += ( Sys->timer1 * 8 + Sys->timer2 ) * 1.0f / 64.0f ;
		obj_uv_put( &px, &py ) ;
	}
}

/*==============================================================*/
static void rank_page_nop( PLAYER_WORK *ew )
{
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "rank_page.c" End of File	*/

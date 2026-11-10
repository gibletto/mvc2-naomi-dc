/* The start demo's twelve small pictures (kind 5): one for each of the six
   characters' places on both sides, which wait, grow in with a bounce, slide to their places and
   then make the starburst (st_demo_ef3_set) at the end. */
#include "player.h"

extern void st_demo_ef3_set( int no, VEC3 pos ) ;

/* three places for each: where it starts, where it stops and where it ends */
static const VEC3 st_demo_ef2_pos_tbl[36] = {
	{ 0.0f, 150.0f, 700.0f },
	{ -80.2f, 170.0f, 450.0f },
	{ -85.2f, 175.0f, 450.0f },
	{ 0.0f, 150.0f, 700.0f },
	{ -48.2f, 170.0f, 450.0f },
	{ -53.2f, 175.0f, 450.0f },
	{ 0.0f, 150.0f, 700.0f },
	{ -16.2f, 170.0f, 450.0f },
	{ -21.2f, 175.0f, 450.0f },
	{ 0.0f, 150.0f, 700.0f },
	{ 15.8f, 170.0f, 450.0f },
	{ 10.8f, 175.0f, 450.0f },
	{ 0.0f, 150.0f, 700.0f },
	{ 47.8f, 170.0f, 450.0f },
	{ 42.8f, 175.0f, 450.0f },
	{ 0.0f, 150.0f, 700.0f },
	{ 79.8f, 170.0f, 450.0f },
	{ 74.8f, 175.0f, 450.0f },
	{ 0.0f, 150.0f, 700.0f },
	{ -80.2f, 170.0f, 460.0f },
	{ -75.2f, 165.0f, 440.0f },
	{ 0.0f, 150.0f, 700.0f },
	{ -48.2f, 170.0f, 460.0f },
	{ -45.2f, 165.0f, 440.0f },
	{ 0.0f, 150.0f, 700.0f },
	{ -16.2f, 170.0f, 460.0f },
	{ -11.2f, 165.0f, 440.0f },
	{ 0.0f, 150.0f, 700.0f },
	{ 15.8f, 170.0f, 460.0f },
	{ 20.8f, 165.0f, 440.0f },
	{ 0.0f, 150.0f, 700.0f },
	{ 47.8f, 170.0f, 460.0f },
	{ 52.8f, 165.0f, 440.0f },
	{ 0.0f, 150.0f, 700.0f },
	{ 79.8f, 170.0f, 460.0f },
	{ 84.8f, 165.0f, 440.0f },
} ;
static const VEC3 st_demo_ef2_one = { 1.0f, 1.0f, 1.0f } ;

static void st_demo_ef2_main( PLAYER_WORK *ew ) ;
static void st_demo_ef2_delay( PLAYER_WORK *ew ) ;
static void st_demo_ef2_in( PLAYER_WORK *ew ) ;
static void st_demo_ef2_wait( PLAYER_WORK *ew ) ;
static void st_demo_ef2_slide( PLAYER_WORK *ew ) ;
static void st_demo_ef2_end( PLAYER_WORK *ew ) ;

/*==============================================================*/
/*			Moves the work along a leg of its path: from		*/
/*			point ofs to the next, in cnt frames				*/
/*==============================================================*/
static void st_demo_ef2_move( PLAYER_WORK *ew, int cnt, int ofs )
{
	const VEC3 *p = &st_demo_ef2_pos_tbl[( ew->sub_no * 6 + ew->type ) * 3 + ofs] ;
	const VEC3 *q = &st_demo_ef2_pos_tbl[( ew->sub_no * 6 + ew->type ) * 3 + ofs + 1] ;

	ew->pos.x = p->x + ( ( q->x - p->x ) / cnt ) * ew->timer0 ;
	ew->pos.y = p->y + ( ( q->y - p->y ) / cnt ) * ew->timer0 ;
	ew->pos.z = p->z + ( ( q->z - p->z ) / cnt ) * ew->timer0 ;
}

/****************************************************************/
void st_demo_ef2_set( int no, int side )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 0 ;
		ew->type = no ;
		ew->sub_no = side ;
		ew->work_func = st_demo_ef2_main ;
		ew->obj = obj_dm02->obj[side * 6 + no + 7] ;
		ew->pos = st_demo_ef2_pos_tbl[( side * 6 + no ) * 3] ;
		ew->scale = st_demo_ef2_one ;
		ew->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_BRIGHT ;
		ew->timer0 = side * 2 + no * 13 + 60 ;
	}
}

/*==============================================================*/
static void st_demo_ef2_main( PLAYER_WORK *ew )
{
	static void ( *st_demo_ef2_jmp[5] )( PLAYER_WORK *ew ) = {
		st_demo_ef2_delay, st_demo_ef2_in, st_demo_ef2_wait, st_demo_ef2_slide, st_demo_ef2_end,
	} ;

	if( Sys->routine_0 != 2 || Sys->timer3 == 3 ){
		work_free( ew ) ;
		return ;
	}
	st_demo_ef2_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
/*			The picture waits to show							*/
/*==============================================================*/
static void st_demo_ef2_delay( PLAYER_WORK *ew )
{
	if( -- ew->timer0 == 0 ){
		ew->routine_1 ++ ;
		ew->disp_flag = 1 ;
		ew->timer0 = 0 ;
	}
}

/*==============================================================*/
/*			It grows in and slides to the middle, where it		*/
/*			bursts												*/
/*==============================================================*/
static void st_demo_ef2_in( PLAYER_WORK *ew )
{
	if( ew->timer0 ++ != 8 ){
		ew->scale.x = 1.0f + ( 8 - ew->timer0 ) * 0.375f ;
		ew->scale.y = ew->scale.x ;
		st_demo_ef2_move( ew, 8, 0 ) ;
	} else {
		ew->routine_1 ++ ;
		ew->timer0 = 13 ;
		ew->pos = st_demo_ef2_pos_tbl[( ew->sub_no * 6 + ew->type ) * 3 + 1] ;
		ew->scale = st_demo_ef2_one ;
		st_demo_ef3_set( 0, ew->pos ) ;
		na_req( 69 ) ;
	}
}

/*--------------------------------------------------------------*/
static void st_demo_ef2_wait( PLAYER_WORK *ew )
{
	if( Sys->timer3 ){
		ew->routine_1 ++ ;
		ew->timer0 = 0 ;
	}
}

/*==============================================================*/
/*			It slides on to its place							*/
/*==============================================================*/
static void st_demo_ef2_slide( PLAYER_WORK *ew )
{
	if( ew->timer0 ++ != 8 ){
		st_demo_ef2_move( ew, 8, 1 ) ;
		return ;
	}
	ew->routine_1 ++ ;
	ew->pos = st_demo_ef2_pos_tbl[( ew->sub_no * 6 + ew->type ) * 3 + 2] ;
}

/*--------------------------------------------------------------*/
static void st_demo_ef2_end( PLAYER_WORK *ew )
{
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "st_demo_ef2.c" End of File	*/

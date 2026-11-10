/* The start demo's six pictures (kind 5): each slides in 30 frames from the
   first to the second place its table gives it, until the demo's step passes. */
#include "player.h"

/* where each starts and stops */
static const VEC3 st_demo_ef1_pos_tbl[12] = {
	{ -260.0f, 330.0f, 700.0f },
	{ -130.0f, 243.0f, 400.0f },
	{ -260.0f, 130.0f, 700.0f },
	{ -130.0f, 140.0f, 400.0f },
	{ -81.0f, 30.0f, 700.0f },
	{ -40.5f, 90.0f, 400.0f },
	{ 81.0f, 350.0f, 700.0f },
	{ 40.5f, 250.0f, 400.0f },
	{ 260.0f, 250.0f, 700.0f },
	{ 130.0f, 200.0f, 400.0f },
	{ 260.0f, 50.0f, 700.0f },
	{ 130.0f, 97.0f, 400.0f },
} ;

static void st_demo_ef1_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void st_demo_ef1_move( PLAYER_WORK *ew )
{
	const VEC3 *p0 = &st_demo_ef1_pos_tbl[ew->type * 2] ;
	const VEC3 *p1 = &st_demo_ef1_pos_tbl[ew->type * 2 + 1] ;

	ew->pos.x = p0->x + ( ( p1->x - p0->x ) / 30.0f ) * ew->timer0 ;
	ew->pos.y = p0->y + ( ( p1->y - p0->y ) / 30.0f ) * ew->timer0 ;
	ew->pos.z = p0->z + ( ( p1->z - p0->z ) / 30.0f ) * ew->timer0 ;
}

/****************************************************************/
void st_demo_ef1_set( int no )
{
	PLAYER_WORK *ew ;

	if( no % 3 != 1 ){
		int i = no % 3 * 2 + no / 3 ;

		if( pl_work[i].load_type == no % 3 + 24 ) return ;
	}
	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->type = no ;
		ew->work_func = st_demo_ef1_main ;
		ew->obj = obj_dm02->obj[no + 1] ;
		ew->pos = st_demo_ef1_pos_tbl[no * 2] ;
		ew->obj_flag = OBJ_POS | OBJ_BRIGHT ;
		ew->timer0 = 0 ;
	}
}

/*==============================================================*/
static void st_demo_ef1_main( PLAYER_WORK *ew )
{
	if( Sys->routine_0 != 2 || Sys->timer3 == 3 ){
		work_free( ew ) ;
		return ;
	}
	if( ew->timer0 == 30 ) return ;
	ew->timer0 ++ ;
	st_demo_ef1_move( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "st_demo_ef1.c" End of File	*/

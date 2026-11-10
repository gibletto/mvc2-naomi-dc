/* Stage 08's object 8, which nothing calls: two models that drift along x with
   the frame count and start over after 6000 frames. */
#include "player.h"

static const VEC3 set08_08_pos_tbl[2] = {
	{ 319.0f, 25.7f, -943.0f },
	{ -1668.1f, 13.36f, -600.7f },
} ;
static const Float set08_08_spd_tbl[2] = {
	4224.24f,
	2868.1f,
} ;

static void set08_08_make( int no ) ;

/****************************************************************/
void set08_08_set( void )
{
	int i ;

	for( i = 0 ; i < 2 ; i ++ ) set08_08_make( i ) ;
}

static void set08_08_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set08_08_make( int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set08_08_main ;
		ew->obj = obj_stg->obj[no + 37] ;
		ew->obj_flag = OBJ_POS | OBJ_BRIGHT ;
		ew->pos = set08_08_pos_tbl[no] ;
		ew->type = no ;
	}
}

/*==============================================================*/
static void set08_08_main( PLAYER_WORK *ew )
{
	ew->pos.x = set08_08_pos_tbl[ew->type].x + set08_08_spd_tbl[ew->type] * ew->timer0 / 6000.0f ;
	if( ++ ew->timer0 >= 6000 ){
		ew->timer0 = 0 ;
		ew->pos = set08_08_pos_tbl[ew->type] ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set08_08.c" End of File	*/

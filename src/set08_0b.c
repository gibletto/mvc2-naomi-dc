/* Stage 08's object 11, which nothing calls: two models that swing about a place
   on x with a sine and drift on y, round and round. */
#include "player.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

static const Sint32 set08_0b_dur_tbl[2] = { 400, 600, } ;
static const VEC3 set08_0b_pos_tbl[2] = {
	{ -165.3f, 836.0f, -903.0f },
	{ -194.0f, 343.5f, -870.0f },
} ;
static const Float set08_0b_rise_tbl[2] = {
	-1354.9f,
	-2250.9f,
} ;

static void set08_0b_make( int no ) ;

/****************************************************************/
void set08_0b_set( void )
{
	int i ;

	for( i = 0 ; i < 2 ; i ++ ) set08_0b_make( i ) ;
}

static void set08_0b_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set08_0b_make( int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set08_0b_main ;
		ew->obj = obj_stg->obj[no + 23] ;
		ew->obj_flag = OBJ_POS | OBJ_BRIGHT ;
		ew->pos = set08_0b_pos_tbl[no] ;
		ew->type = no ;
	}
}

/*==============================================================*/
static void set08_0b_main( PLAYER_WORK *ew )
{
	ew->pos.x = set08_0b_pos_tbl[ew->type].x + 100.0f * nlSin( DEG_ANG( ew->timer0 * 360.0f / set08_0b_dur_tbl[ew->type] ) ) ;
	ew->pos.y = set08_0b_pos_tbl[ew->type].y + set08_0b_rise_tbl[ew->type] * ew->timer0 / set08_0b_dur_tbl[ew->type] ;
	ew->timer0 ++ ;
	if( ew->timer0 >= set08_0b_dur_tbl[ew->type] ) ew->timer0 = 0 ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set08_0b.c" End of File	*/

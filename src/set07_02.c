/* Stage 07's object 2: three models (objects 15 to 17 of the stage's file) that bob up and down
   about a height of their own, each started a third of the way on from the one before. */
#include "player.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

static VEC3 set07_02_pos_tbl[3] = {
	{ 66.334f, -114.4f, -694.63f },
	{ -2.444f, -138.4f, -1031.57f },
	{ 1168.127f, -16.8f, -652.317f },
} ;
static Float set07_02_amp_tbl[3] = { 25.0f, 20.0f, 35.0f } ;
static VEC3 set07_02_unused_vec = { 1.0f, 1.0f, 1.0f } ;

static void set07_02_make( int no ) ;

/****************************************************************/
void set07_02_set( void )
{
	int i ;

	for( i = 0 ; i < 3 ; i ++ ) set07_02_make( i ) ;
}

static void set07_02_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set07_02_make( int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set07_02_main ;
		ew->obj = *( obj_stg->obj + no + 15 ) ;
		ew->pos = set07_02_pos_tbl[no] ;
		ew->obj_flag = OBJ_POS | OBJ_BRIGHT ;
		ew->type = no ;
		ew->timer0 = no * 120 ;
	}
}

/*==============================================================*/
static void set07_02_main( PLAYER_WORK *ew )
{
	ew->pos.y = set07_02_pos_tbl[ew->type].y + set07_02_amp_tbl[ew->type] * nlSin( DEG_ANG( ew->timer0 ) ) ;
	ew->timer0 ++ ;
	if( ew->timer0 >= 360 ) ew->timer0 = 0 ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set07_02.c" End of File	*/

/* Stage 03's object 1: two models (objects 3 and 5 of the stage's file) that rock about one axis,
   the second the other way about. */
#include "player.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

static VEC3 set03_01_pos_tbl[2] = {
	{ -446.0f, 53.0f, -528.0f },
	{ 350.0f, 56.0f, -934.0f },
} ;
static Float set03_01_rot_tbl[2] = { -33.0f, -98.0f } ;

/*==============================================================*/
static void set03_01_move( PLAYER_WORK *ew )
{
	if( ++ ew->timer0 >= 360 ) ew->timer0 = 0 ;
	ew->rot[0] = ( Sint32 )( ( ew->type ? 1 : -1 ) * nlSin( DEG_ANG( ew->timer0 ) ) * 917504.0f / 360.0f + 0.5f ) & 0xffff ;
}

/*==============================================================*/
static void set03_01_make( int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set03_01_move ;
		ew->obj = obj_stg->obj[no * 2 + 3] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_BRIGHT ;
		ew->pos = set03_01_pos_tbl[no] ;
		ew->rot[1] = DEG_ANG( set03_01_rot_tbl[no] ) ;
		ew->type = no ;
	}
}

/****************************************************************/
void set03_01_set( void )
{
	set03_01_make( 0 ) ;
	set03_01_make( 1 ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set03_01.c" End of File	*/

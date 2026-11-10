/* Stage 03's object 7: a model (object 30 of the stage's file) with three lit parts on it (their
   code is in s_set03.c) that change from one model to another by a table. */
#include "player.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

static VEC3 set03_07_pos = { -150.0f, 850.0f, -900.0f } ;
static VEC3 set03_07_rot = { 0.0f, 25.0f, 0.0f } ;

/*==============================================================*/
static void set03_07_main( PLAYER_WORK *ew )
{
}

extern void set03_07_part_set( PLAYER_WORK *pl_w, int type ) ;

/****************************************************************/
void set03_07_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set03_07_main ;
		ew->obj = obj_stg->obj[30] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_ROT_Z | OBJ_BRIGHT ;
		ew->pos = set03_07_pos ;
		ew->rot[0] = DEG_ANG( set03_07_rot.x ) ;
		ew->rot[1] = DEG_ANG( set03_07_rot.y ) ;
		ew->rot[2] = DEG_ANG( set03_07_rot.z ) ;
		set03_07_part_set( ew, 0 ) ;
		set03_07_part_set( ew, 1 ) ;
		set03_07_part_set( ew, 2 ) ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set03_07.c" End of File	*/

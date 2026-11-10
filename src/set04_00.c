/* Stage 4's object 0 (the first entry of its table): a model (object 1 of the stage's file) that
   turns about one axis. */
#include "player.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

/* its place and turn: tables of one, as the stage modules keep for their objects */
static VEC3 set04_00_pos_tbl[1] = { { -920.0f, 80.0f, -620.0f } } ;
static Sint32 set04_00_rot_tbl[1] = { 46422 } ;

/*==============================================================*/
static void set04_00_main( PLAYER_WORK *ew )
{
	ew->timer0 ++ ;
	if( ew->timer0 >= 360 ) ew->timer0 = 0 ;
	ew->rot[0] = DEG_ANG( ew->timer0 ) ;
}

/****************************************************************/
void set04_00_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set04_00_main ;
		ew->obj = obj_stg->obj[1] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_BRIGHT ;
		ew->pos = set04_00_pos_tbl[0] ;
		ew->rot[1] = set04_00_rot_tbl[0] ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set04_00.c" End of File	*/

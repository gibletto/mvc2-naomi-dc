/* Stage 01's second object (kind 5 works): a model that waits for the first
   side to be in the game, jumps up spinning, falls back to its place and stands. */
#include "player.h"

/* degrees to an angle */
#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

static VEC3 set01_01_pos = { 596.0f, 439.0f, -1168.0f } ;

/*==============================================================*/
static void set01_01_main( PLAYER_WORK *ew )
{
	switch( ew->routine_0 ){
	case 0:
		if( side_flag_get( 0 ) ){
			ew->routine_0 ++ ;
			ew->spd.y = 20.0f ;
			ew->rot[0] = DEG_ANG( nlRand() % 5 ) ;
			ew->rot[2] = ew->rot[0] / 30 ;
		}
		break ;
	case 1:
		ew->pos.y += ew->spd.y ;
		if( ew->spd.y <= 0.0f ){
			ew->spd.y += -0.4f ;
		} else {
			ew->spd.y += -2.0f ;
			ew->rot[1] += ew->rot[0] ;
			ew->rot[0] -= ew->rot[2] ;
		}
		if( ew->pos.y < set01_01_pos.y + 50.0f && ew->spd.y < 0.0f ){
			ew->routine_0 ++ ;
			ew->pos.y = set01_01_pos.y + 50.0f ;
			ew->spd.y = 0.0f ;
		}
		break ;
	case 2:
		ew->pos.y += ew->spd.y ;
		ew->spd.y += -0.4f ;
		if( ew->pos.y < set01_01_pos.y ){
			ew->pos.y = set01_01_pos.y ;
			ew->routine_0 = 0 ;
		}
		break ;
	}
}

/****************************************************************/
void set01_01_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set01_01_main ;
		ew->obj = obj_stg->obj[3] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_BRIGHT ;
		ew->pos = set01_01_pos ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set01_01.c" End of File	*/

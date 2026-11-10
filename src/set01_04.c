/* Stage 01's fifth object (kind 5 works): two models that fall back to
   their places and jump up again when the first side is in the game. */
#include "player.h"

/* where each stands, and how fast it jumps */
static const VEC3 set01_04_pos_tbl[2] = {
	{ -1240.0f, 10.0f, -420.0f },
	{ -1090.0f, 10.0f, -300.0f },
} ;
static const Float set01_04_jump_tbl[2] = { 10.0f, 15.0f } ;

/*==============================================================*/
static void set01_04_main( PLAYER_WORK *ew )
{
	switch( ew->routine_0 ){
	case 0:
		ew->spd.y += -4.0f ;
		ew->pos.y += ew->spd.y ;
		if( ew->pos.y < set01_04_pos_tbl[ew->type].y ){
			ew->pos.y = set01_04_pos_tbl[ew->type].y ;
			ew->spd.y = 0.0f ;
			if( ew->timer0 ) ew->timer0 = 0 ;
		}
		if( side_flag_get( 0 ) ){
			ew->spd.y = set01_04_jump_tbl[ew->type] ;
			ew->timer0 = 1 ;
		}
		break ;
	}
}

/*==============================================================*/
static void set01_04_make( int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set01_04_main ;
		ew->obj = obj_stg->obj[no + 12] ;
		ew->obj_flag = OBJ_POS | OBJ_BRIGHT ;
		ew->pos = set01_04_pos_tbl[no] ;
		ew->type = no ;
	}
}

/****************************************************************/
void set01_04_set( void )
{
	int i ;

	for( i = 0 ; i < 2 ; i ++ ){
		set01_04_make( i ) ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set01_04.c" End of File	*/

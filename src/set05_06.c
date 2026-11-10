/* Stage 05's object 6: a model (one of objects 21 to 24 of the stage's file, by a counter) that
   jumps to a place picked at random every eight frames. */
#include "player.h"

static void set05_06_main( PLAYER_WORK *ew ) ;

/****************************************************************/
void set05_06_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set05_06_main ;
		ew->obj_flag = OBJ_POS | OBJ_BRIGHT ;
	}
}

static Sint32 set05_06_obj_tbl[8] = {
	0, 0, 1, 1, 2, 2, 3, 3,
} ;
static VEC3 set05_06_pos_tbl[8] = {
	{ -1000.0f, -12.0f, -280.0f },
	{ 0.0f, -12.0f, -400.0f },
	{ -1000.0f, 7.0f, -180.0f },
	{ -1000.0f, -5.5f, 100.0f },
	{ -200.0f, -10.0f, 100.0f },
	{ 600.0f, -4.0f, 130.0f },
	{ 100.0f, 320.0f, -2000.0f },
	{ -50.0f, 50.0f, -1700.0f },
} ;

/*==============================================================*/
static void set05_06_main( PLAYER_WORK *ew )
{
	ew->obj = *( obj_stg->obj + set05_06_obj_tbl[ew->timer0] + 21 ) ;
	if( ( Uint32 )( ++ ew->timer0 ) >= 8 ){
		ew->pos = set05_06_pos_tbl[nlRand() & 7] ;
		ew->timer0 = 0 ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set05_06.c" End of File	*/

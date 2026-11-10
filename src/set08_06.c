/* Stage 08's object 6, which nothing calls: a model that waits a second, jumps to
   a random place on the far wall, slides across it and fades out, then waits again. */
#include "player.h"

static const VEC3 set08_06_pos = { 5320.0f, 14000.0f, -30000.0f } ;

static void set08_06_main( PLAYER_WORK *ew ) ;

/****************************************************************/
void set08_06_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 0 ;
		ew->obj = obj_stg->obj[15] ;
		ew->work_func = set08_06_main ;
		ew->obj_flag = OBJ_POS | OBJ_FADE | OBJ_BRIGHT ;
	}
}

static void set08_06_wait( PLAYER_WORK *ew ) ;
static void set08_06_move( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set08_06_main( PLAYER_WORK *ew )
{
	switch( ew->routine_0 ){
	case 0 :
		set08_06_wait( ew ) ;
		break ;
	case 1 :
		set08_06_move( ew ) ;
		break ;
	}
}

/*==============================================================*/
static void set08_06_wait( PLAYER_WORK *ew )
{
	if( ++ ew->timer0 >= 60 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 1 ;
		ew->pos = set08_06_pos ;
		ew->pos.x += ( Float )( nlRand() % 50000 - 10000 ) ;
		ew->fade.r = ew->fade.g = ew->fade.b = 1.0f ;
	}
}

/*==============================================================*/
static void set08_06_move( PLAYER_WORK *ew )
{
	ew->pos.x += -145.0f ;
	ew->pos.y += -84.0f ;
	switch( ew->routine_1 ){
	case 0 :
		if( ++ ew->timer0 >= 50 ){
			ew->routine_1 ++ ;
			ew->timer0 = 0 ;
		}
		break ;
	case 1 :
		ew->fade.r -= 0.02f ;
		ew->fade.g -= 0.02f ;
		ew->fade.b -= 0.02f ;
		if( ew->fade.r <= 0.0f ){
			ew->routine_0 = 0 ;
			ew->disp_flag = 0 ;
		}
		break ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set08_06.c" End of File	*/

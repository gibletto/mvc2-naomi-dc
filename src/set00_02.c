/* Stage 00's third object (kind 5 works): a model that waits out the first
   frames of a fight, then comes up at one of nine places, blinking, and fades again. */
#include "player.h"

/* where it comes up */
static const VEC3 set00_02_pos_tbl[9] = {
	{ -200.0f, 5000.0f, -15000.0f },
	{ -1500.0f, 1000.0f, -14000.0f },
	{ 1500.0f, 3500.0f, -17000.0f },
	{ -3000.0f, 2500.0f, -12000.0f },
	{ 2700.0f, 2000.0f, -15000.0f },
	{ 3000.0f, 3000.0f, -16000.0f },
	{ 4000.0f, 3500.0f, -10000.0f },
	{ -5000.0f, 4000.0f, -15000.0f },
	{ 6000.0f, 4000.0f, -15000.0f },
} ;

/* and its turns about y and z */
static const Float set00_02_rot_tbl[9][2] = {
	{ 0.0f, 180.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 180.0f }, { 0.0f, 0.0f },
	{ 0.0f, -20.0f }, { 0.0f, 0.0f }, { 0.0f, 30.0f }, { 180.0f, -30.0f },
} ;

static void set00_02_main( PLAYER_WORK *ew ) ;

/****************************************************************/
void set00_02_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 0 ;
		ew->work_func = set00_02_main ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_ROT_Z | OBJ_FADE | OBJ_BRIGHT ;
	}
}

static void set00_02_wait( PLAYER_WORK *ew ) ;
static void set00_02_show( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set00_02_main( PLAYER_WORK *ew )
{
	switch( ew->routine_0 ){
	case 0:
		set00_02_wait( ew ) ;
		break ;
	case 1:
		set00_02_show( ew ) ;
		break ;
	}
}

/*==============================================================*/
/*			Wait, then pick the model and the place				*/
/*==============================================================*/
static void set00_02_wait( PLAYER_WORK *ew )
{
	Uint32 t, no ;

	if( ++ ew->timer0 >= 120 ){
		ew->timer0 = 120 ;
		t = Sys->play_timer ;
		if( t > 500 && t < 1000 ) return ;
		if( t > 3500 ) return ;
		ew->routine_0 ++ ;
		ew->routine_1 = 0 ;
		ew->timer0 = 0 ;
		ew->obj = obj_stg->obj[nlRand() % 4 + 3] ;
		no = ( Uint32 )nlRand() % 9 ;
		ew->pos = set00_02_pos_tbl[no] ;
		ew->rot[1] = set00_02_rot_tbl[no][0] ;
		ew->rot[2] = set00_02_rot_tbl[no][1] ;
		ew->fade.g = ew->fade.r = 1.0f ;
		ew->fade.b = ew->fade.r ;
	}
}

/*==============================================================*/
/*			Blink in, then fade out								*/
/*==============================================================*/
static void set00_02_show( PLAYER_WORK *ew )
{
	switch( ew->routine_1 ){
	case 0:
		ew->timer0 ++ ;
		ew->disp_flag = ew->timer0 % 2 ;
		if( ew->timer0 >= 5 ){
			ew->routine_1 ++ ;
			ew->timer0 = 0 ;
			ew->disp_flag = 1 ;
			ew->fade.g = ew->fade.r = 1.0f ;
			ew->fade.b = ew->fade.r ;
		}
		break ;
	case 1:
		ew->fade.r -= 0.06666667f ;
		ew->fade.g = ew->fade.r ;
		ew->fade.b = ew->fade.r ;
		if( ++ ew->timer0 >= 15 ){
			ew->routine_0 = 0 ;
			ew->disp_flag = 0 ;
			if( Sys->play_timer > 4500 ) work_free( ew ) ;
		}
		break ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set00_02.c" End of File	*/

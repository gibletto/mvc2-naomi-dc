/* Stage 08's object 5: two models that blink through a pose of four frames, hide for a random
   time and start again, until the first fight is over. */
#include "player.h"

static VEC3 set08_05_pos_tbl[2] = {
	{ -220.0f, 350.0f, -1050.0f },
	{ 220.0f, 350.0f, -1050.0f },
} ;

/*==============================================================*/
static void set08_05_main( PLAYER_WORK *ew )
{
	if( stage_work.event >= 1 ) work_free( ew ) ;
	switch( ew->routine_0 ){
	case 0 :
		ew->disp_flag = 1 ;
		ew->obj = obj_stg->obj[78 + ew->type * 4 + ew->timer0 / 2] ;
		if( ++ ew->timer0 >= 8 ){
			ew->timer0 = 0 ;
			ew->timer1 = nlRand() % 30 + 30 ;
			ew->routine_0 ++ ;
		}
		break ;
	case 1 :
		ew->disp_flag = 0 ;
		if( ++ ew->timer0 >= ew->timer1 ){
			ew->routine_0 = ew->timer0 = 0 ;
		}
		break ;
	}
}

/*==============================================================*/
static void set08_05_make( int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->work_func = set08_05_main ;
		ew->obj_flag = OBJ_POS | OBJ_FACE_Y | OBJ_BRIGHT ;
		ew->pos = set08_05_pos_tbl[no] ;
		ew->type = no ;
	}
}

/****************************************************************/
void set08_05_set( void )
{
	int i ;

	for( i = 0 ; i < 2 ; i ++ ) set08_05_make( i ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set08_05.c" End of File	*/

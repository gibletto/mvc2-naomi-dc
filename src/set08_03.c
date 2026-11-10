/* Stage 08's object 3: two models that flick between poses at random intervals and shimmer in
   colour until the round's first fight ends. */
#include "player.h"

extern void obj_argb_set( void *obj, Float a, Float r, Float g, Float b ) ;

static VEC3 set08_03_pos_tbl[2] = {
	{ -780.0f, 300.0f, -1100.0f },
	{ 780.0f, 300.0f, -1100.0f },
} ;

/*==============================================================*/
static void set08_03_main( PLAYER_WORK *ew )
{
	switch( ew->routine_0 ){
	case 0 :
		ew->timer0 ++ ;
		if( ew->timer0 >= ew->timer1 ){
			ew->timer1 = nlRand() % 27 + 24 ;
			ew->timer0 = 0 ;
		}
		ew->obj = obj_stg->obj[ew->timer0 / 3 + 12] ;
		obj_argb_set( ew->obj, 1.0f,
			0.75f + 0.25f * nlSin( Sys->wave_ang ),
			0.75f + 0.25f * nlSin( Sys->wave_ang << 1 ),
			0.75f + 0.25f * nlSin( Sys->wave_ang << 2 ) ) ;
		if( stage_work.event == 1 ) work_free( ew ) ;
		break ;
	}
}

/*==============================================================*/
static void set08_03_make( int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set08_03_main ;
		ew->obj = obj_stg->obj[no + 12] ;
		ew->obj_flag = OBJ_POS | OBJ_FACE_Y | OBJ_BRIGHT ;
		ew->pos = set08_03_pos_tbl[no] ;
	}
}

/****************************************************************/
void set08_03_set( void )
{
	int i ;

	for( i = 0 ; i < 2 ; i ++ ) set08_03_make( i ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set08_03.c" End of File	*/

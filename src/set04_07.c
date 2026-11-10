/* Stage 4's object 7 (kind 5 works): a model that slides along a line through three keys, round and
   round. */
#include "player.h"

extern void spline_weight( Float *w0, Float *w1, Float *w2, Float t ) ;

/* a key: the frame it lies at, and the place and turn there */
typedef struct SET04_07_KEY {
	Float	time;
	VEC3	pos;
	VEC3	rot;
} SET04_07_KEY;

static SET04_07_KEY set04_07_key_tbl[3] = {
	{ 0.0f, { 12000.0f, 125.0f, -9240.0f }, { 0.0f, 0.0f, 0.0f } },
	{ 6000.0f, { 0.0f, 125.0f, -9240.0f }, { 0.0f, 0.0f, 0.0f } },
	{ 12000.0f, { -12000.0f, 125.0f, -9240.0f }, { 0.0f, 0.0f, 0.0f } },
} ;

static void set04_07_main( PLAYER_WORK *ew ) ;

/****************************************************************/
void set04_07_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set04_07_main ;
		ew->obj = obj_stg->obj[36] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_ROT_Z | OBJ_BRIGHT ;
		ew->timer1 = 1 ;
	}
}

/*==============================================================*/
static void set04_07_main( PLAYER_WORK *ew )
{
	Float w0, w1, w2 ;
	Float t ;
	int i ;

	switch( ew->routine_0 ){
	case 0:
		t = ( Float )ew->timer0 / ( set04_07_key_tbl[ew->timer1 + 1].time - set04_07_key_tbl[ew->timer1].time ) ;
		spline_weight( &w0, &w1, &w2, t ) ;
		i = ew->timer1 ;
		ew->pos.x = set04_07_key_tbl[i - 1].pos.x * w0 + set04_07_key_tbl[i].pos.x * w1 + set04_07_key_tbl[i + 1].pos.x * w2 ;
		ew->pos.y = set04_07_key_tbl[i - 1].pos.y * w0 + set04_07_key_tbl[i].pos.y * w1 + set04_07_key_tbl[i + 1].pos.y * w2 ;
		ew->pos.z = set04_07_key_tbl[i - 1].pos.z * w0 + set04_07_key_tbl[i].pos.z * w1 + set04_07_key_tbl[i + 1].pos.z * w2 ;
		ew->timer0 ++ ;
		if( ew->timer0 >= ( set04_07_key_tbl[ew->timer1 + 1].time - set04_07_key_tbl[ew->timer1].time ) ){
			ew->timer0 = 0 ;
			ew->timer1 ++ ;
			if( ( Uint32 )( ew->timer1 + 1 ) >= 3 ) ew->timer1 = 1 ;
		}
		break ;
	case 1:
		break ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set04_07.c" End of File	*/

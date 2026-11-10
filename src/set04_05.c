/* Stage 4's object 5 (kind 5 works): a model that slides along a line through four keys, round and
   round, with a second model on it whose texture slides. */
#include "player.h"

extern void spline_weight( Float *w0, Float *w1, Float *w2, Float t ) ;
extern int obj_uv_begin( int *src, void *obj ) ;
extern int obj_uv_next( void ) ;
extern int obj_uv_get( Float *x, Float *y ) ;
extern int obj_uv_put( Float *x, Float *y ) ;
extern void obj_uv_clear( void *obj ) ;

/* a key: the frame it lies at, and the place and turn there */
typedef struct SET04_05_KEY {
	Float	time;
	VEC3	pos;
	VEC3	rot;
} SET04_05_KEY;

static SET04_05_KEY set04_05_key_tbl[4] = {
	{ 0.0f, { 13000.0f, -103.0f, 5765.0f }, { 0.0f, 0.0f, 0.0f } },
	{ 3000.0f, { 5800.0f, -103.0f, 510.0f }, { 0.0f, 0.0f, 0.0f } },
	{ 6000.0f, { -1400.0f, -103.0f, -4745.0f }, { 0.0f, 0.0f, 0.0f } },
	{ 9000.0f, { -8600.0f, -103.0f, -10000.0f }, { 0.0f, 0.0f, 0.0f } },
} ;

static void set04_05_main( PLAYER_WORK *ew ) ;
static void set04_05_sub_set( PLAYER_WORK *pl_w ) ;

/****************************************************************/
void set04_05_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set04_05_main ;
		ew->obj = obj_stg->obj[8] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_ROT_Z | OBJ_BRIGHT ;
		ew->timer1 = 1 ;
		set04_05_sub_set( ew ) ;
	}
}

/*==============================================================*/
static void set04_05_main( PLAYER_WORK *ew )
{
	Float w0, w1, w2 ;
	Float t ;
	int i ;

	switch( ew->routine_0 ){
	case 0:
		t = ( Float )ew->timer0 / ( set04_05_key_tbl[ew->timer1 + 1].time - set04_05_key_tbl[ew->timer1].time ) ;
		spline_weight( &w0, &w1, &w2, t ) ;
		i = ew->timer1 ;
		ew->pos.x = set04_05_key_tbl[i - 1].pos.x * w0 + set04_05_key_tbl[i].pos.x * w1 + set04_05_key_tbl[i + 1].pos.x * w2 ;
		ew->pos.y = set04_05_key_tbl[i - 1].pos.y * w0 + set04_05_key_tbl[i].pos.y * w1 + set04_05_key_tbl[i + 1].pos.y * w2 ;
		ew->pos.z = set04_05_key_tbl[i - 1].pos.z * w0 + set04_05_key_tbl[i].pos.z * w1 + set04_05_key_tbl[i + 1].pos.z * w2 ;
		ew->timer0 ++ ;
		if( ew->timer0 >= ( set04_05_key_tbl[ew->timer1 + 1].time - set04_05_key_tbl[ew->timer1].time ) ){
			ew->timer0 = 0 ;
			ew->timer1 ++ ;
			if( ( Uint32 )( ew->timer1 + 1 ) >= 4 ) ew->timer1 = 1 ;
		}
		break ;
	case 1:
		break ;
	}
}

static void set04_05_sub_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set04_05_sub_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set04_05_sub_main ;
		ew->obj = obj_stg->obj[30] ;
		ew->obj_flag = OBJ_BRIGHT ;
		ew->ef_w = pl_w ;
		ew->base_mtx = &pl_w->obj_mtx ;
		obj_uv_clear( ew->obj ) ;
	}
}

/*==============================================================*/
static void set04_05_sub_main( PLAYER_WORK *ew )
{
	Float x, y ;

	switch( ew->routine_0 ){
	case 0:
		ew->timer0 ++ ;
		if( ew->timer0 >= 100 ) ew->timer0 = 0 ;
		obj_uv_begin( obj_stg->obj[31], ew->obj ) ;
		while( obj_uv_next() == 0 ){
			obj_uv_get( &x, &y ) ;
			y += ew->timer0 * 0.01f ;
			obj_uv_put( &x, &y ) ;
		}
		break ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set04_05.c" End of File	*/

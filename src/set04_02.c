/* Stage 4's object 2 (kind 5 works): a model that takes a place and a turn from six keys, blending
   the three keys around its count with a quadratic spline, and a second model on it whose texture
   slides once it has come far enough. */
#include "player.h"

/* degrees to an angle */
#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

extern void spline_weight( Float *w0, Float *w1, Float *w2, Float t ) ;
extern int obj_uv_begin( int *src, void *obj ) ;
extern int obj_uv_next( void ) ;
extern int obj_uv_pos_get( VEC3 *v ) ;
extern int obj_uv_pos_put( VEC3 *v ) ;

/* a key: the frame it lies at, and the place and turn there */
typedef struct SET04_02_KEY {
	Float	time;
	VEC3	pos;
	VEC3	rot;
} SET04_02_KEY;

static SET04_02_KEY set04_02_key_tbl[6] = {
	{ 0.0f, { 2250.0f, 27.0f, -6000.0f }, { 0.0f, 30.0f, 0.0f } },
	{ 200.0f, { 1375.0f, 27.0f, -4230.0f }, { 0.0f, 10.0f, 0.0f } },
	{ 400.0f, { 500.0f, 27.0f, -2460.0f }, { 0.0f, 0.0f, 0.0f } },
	{ 600.0f, { 200.0f, 40.0f, -2150.0f }, { -5.0f, 0.0f, 0.0f } },
	{ 800.0f, { 340.0f, -34.0f, -2150.0f }, { -5.0f, 0.0f, -11.5f } },
	{ 1000.0f, { 400.0f, -49.0f, -2150.0f }, { 0.0f, 0.0f, -20.0f } },
} ;

static VEC3 set04_02_sub_pos = { -390.0f, 950.0f, 160.0f } ;
static Float set04_02_sub_unk = 0.5f ;	/* never used */

static void set04_02_main( PLAYER_WORK *ew ) ;
static void set04_02_sub_set( PLAYER_WORK *pl_w ) ;

/****************************************************************/
void set04_02_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set04_02_main ;
		ew->obj = obj_stg->obj[4] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_ROT_Z | OBJ_BRIGHT ;
		ew->timer1 = 1 ;
		set04_02_sub_set( ew ) ;
	}
}

/*==============================================================*/
static void set04_02_main( PLAYER_WORK *ew )
{
	Float w0, w1, w2 ;
	Float t ;
	int i ;

	switch( ew->routine_0 ){
	case 0:
		t = ( Float )ew->timer0 / ( set04_02_key_tbl[ew->timer1 + 1].time - set04_02_key_tbl[ew->timer1].time ) ;
		spline_weight( &w0, &w1, &w2, t ) ;
		i = ew->timer1 ;
		ew->pos.x = set04_02_key_tbl[i - 1].pos.x * w0 + set04_02_key_tbl[i].pos.x * w1 + set04_02_key_tbl[i + 1].pos.x * w2 ;
		ew->pos.y = set04_02_key_tbl[i - 1].pos.y * w0 + set04_02_key_tbl[i].pos.y * w1 + set04_02_key_tbl[i + 1].pos.y * w2 ;
		ew->pos.z = set04_02_key_tbl[i - 1].pos.z * w0 + set04_02_key_tbl[i].pos.z * w1 + set04_02_key_tbl[i + 1].pos.z * w2 ;
		ew->rot[0] = DEG_ANG( set04_02_key_tbl[i - 1].rot.x * w0 + set04_02_key_tbl[i].rot.x * w1 + set04_02_key_tbl[i + 1].rot.x * w2 ) ;
		ew->rot[1] = DEG_ANG( set04_02_key_tbl[i - 1].rot.y * w0 + set04_02_key_tbl[i].rot.y * w1 + set04_02_key_tbl[i + 1].rot.y * w2 ) ;
		ew->rot[2] = DEG_ANG( set04_02_key_tbl[i - 1].rot.z * w0 + set04_02_key_tbl[i].rot.z * w1 + set04_02_key_tbl[i + 1].rot.z * w2 ) ;
		ew->timer0 ++ ;
		if( ew->timer0 >= ( set04_02_key_tbl[ew->timer1 + 1].time - set04_02_key_tbl[ew->timer1].time ) ){
			ew->timer0 = 0 ;
			ew->timer1 ++ ;
			if( ( Uint32 )( ew->timer1 + 1 ) >= 6 ) ew->routine_0 ++ ;
			if( ( Uint32 )( ew->timer1 + 1 ) >= 4 ) ew->routine_1 ++ ;
		}
		break ;
	case 1:
		break ;
	}
}

static void set04_02_sub_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set04_02_sub_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set04_02_sub_main ;
		ew->obj = obj_stg->obj[32] ;
		ew->pos = set04_02_sub_pos ;
		ew->obj_flag = OBJ_POS | OBJ_BRIGHT ;
		ew->base_mtx = &pl_w->obj_mtx ;
		ew->ef_w = pl_w ;
	}
}

/*==============================================================*/
static void set04_02_sub_main( PLAYER_WORK *ew )
{
	VEC3 v ;

	switch( ew->routine_0 ){
	case 0:
		if( ew->ef_w->routine_1 ) ew->routine_0 ++ ;
		break ;
	case 1:
		ew->timer0 ++ ;
		if( ew->timer0 >= 400 ){
			ew->timer0 = 0 ;
			ew->routine_0 ++ ;
			break ;
		}
		obj_uv_begin( obj_stg->obj[33], ew->obj ) ;
		while( obj_uv_next() == 0 ){
			obj_uv_pos_get( &v ) ;
			v.x -= v.y * ( ew->timer0 / 400.0f ) * 0.2f ;
			obj_uv_pos_put( &v ) ;
		}
		break ;
	case 2:
		break ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set04_02.c" End of File	*/

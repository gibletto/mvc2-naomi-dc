/* Stage 00's first object (kind 5 works): a model that takes a place and a
   turn from nine keys, blending the three keys around its count with a quadratic spline, and the two
   models that ride on it and spin. */
#include "player.h"

/* degrees to an angle */
#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

extern void spline_weight( Float *w0, Float *w1, Float *w2, Float t ) ;

/* a key: the frame it lies at, and the place and turn there */
typedef struct SET00_00_KEY {
	Float	time;
	VEC3	pos;
	VEC3	rot;
} SET00_00_KEY;

static const SET00_00_KEY set00_00_key_tbl[9] = {
	{ 0.0f, { 7000.0f, -8000.0f, -7000.0f }, { 0.0f, -220.0f, 0.0f } },
	{ 500.0f, { 7000.0f, 3000.0f, -7000.0f }, { 0.0f, -120.0f, 0.0f } },
	{ 1000.0f, { 0.0f, 3500.0f, -9000.0f }, { 0.0f, -90.0f, 0.0f } },
	{ 1500.0f, { -3000.0f, 2500.0f, -8000.0f }, { 0.0f, -20.0f, 0.0f } },
	{ 2000.0f, { -3000.0f, 4000.0f, 200.0f }, { 0.0f, 30.0f, 0.0f } },
	{ 2500.0f, { 4000.0f, 4000.0f, 100.0f }, { 0.0f, 120.0f, 0.0f } },
	{ 3200.0f, { 7000.0f, 5000.0f, -10000.0f }, { 0.0f, 240.0f, 0.0f } },
	{ 4000.0f, { -5000.0f, 10000.0f, -25000.0f }, { 0.0f, 200.0f, 0.0f } },
	{ 6000.0f, { -30000.0f, 60000.0f, -95000.0f }, { 0.0f, 210.0f, 0.0f } },
} ;

/* where the two riders stand */
static const VEC3 set00_00_pos_tbl[2] = {
	{ 1600.0f, -70.0f, -1110.0f },
	{ -1600.0f, -70.0f, -1110.0f },
} ;

static void set00_00_main( PLAYER_WORK *ew ) ;
static void set00_00_obj_set( PLAYER_WORK *pl_w, Sint32 no ) ;

/****************************************************************/
void set00_00_set( void )
{
	PLAYER_WORK *ew ;
	Sint32 i ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set00_00_main ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_ROT_Z | OBJ_BRIGHT ;
		ew->timer1 = 1 ;
		for( i = 0 ; i < 2 ; i ++ ) set00_00_obj_set( ew, i ) ;
	}
}

/*==============================================================*/
static void set00_00_main( PLAYER_WORK *ew )
{
	Float w0, w1, w2 ;
	const SET00_00_KEY *key, *prev, *next ;
	Sint32 n ;

	switch( ew->routine_0 ){
	case 0:
		n = ew->timer1 ;
		if( n > 2000 && n < 2500 ){
			ew->disp_flag = 0 ;
		} else {
			ew->disp_flag = 1 ;
			if( n <= 2000 ) ew->obj = obj_stg->obj[1] ;
			else ew->obj = obj_stg->obj[14] ;
			key = &set00_00_key_tbl[n] ;
			next = key + 1 ;
			spline_weight( &w0, &w1, &w2, ( Float )ew->timer0 / ( next->time - key->time ) ) ;
			prev = &set00_00_key_tbl[n - 1] ;
			ew->pos.x = prev->pos.x * w0 + key->pos.x * w1 + next->pos.x * w2 ;
			ew->pos.y = prev->pos.y * w0 + key->pos.y * w1 + next->pos.y * w2 ;
			ew->pos.z = prev->pos.z * w0 + key->pos.z * w1 + next->pos.z * w2 ;
			ew->rot[0] = DEG_ANG( prev->rot.x * w0 + key->rot.x * w1 + next->rot.x * w2 ) ;
			ew->rot[1] = DEG_ANG( prev->rot.y * w0 + key->rot.y * w1 + next->rot.y * w2 ) ;
			ew->rot[2] = DEG_ANG( prev->rot.z * w0 + key->rot.z * w1 + next->rot.z * w2 ) ;
		}
		ew->timer0 ++ ;
		if( ew->timer0 >= ( set00_00_key_tbl[ew->timer1 + 1].time - set00_00_key_tbl[ew->timer1].time ) ){
			ew->timer0 = 0 ;
			ew->timer1 ++ ;
			if( ( Uint32 )( ew->timer1 + 1 ) >= 9 ) ew->routine_0 = ew->routine_0 + 1 ;
		}
		break ;
	case 1:
		work_free( ew ) ;
		break ;
	}
}

static void set00_00_obj_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set00_00_obj_set( PLAYER_WORK *pl_w, Sint32 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->work_func = set00_00_obj_main ;
		ew->obj = ( obj_stg->obj + 7 )[no] ;
		ew->pos = set00_00_pos_tbl[no] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Z | OBJ_BRIGHT ;
		ew->base_mtx = &pl_w->obj_mtx ;
		ew->ef_w = pl_w ;
		ew->type = no ;
	}
}

/*==============================================================*/
static void set00_00_obj_main( PLAYER_WORK *ew )
{
	if( ew->ef_w->routine_0 == 1 ){
		work_free( ew ) ;
		return ;
	}
	ew->disp_flag = ew->ef_w->disp_flag ;
	switch( ew->type ){
	case 0:
		ew->rot[2] -= 0x100 ;
		break ;
	case 1:
		ew->rot[2] += 0x100 ;
		break ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set00_00.c" End of File	*/

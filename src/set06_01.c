/* Stage 06's object 1: a model (object 5 of the stage's file) that travels along three curves over
   6000 frames and is then freed, with a model riding on it (object 11) that is freed at the same
   time. */
#include "player.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

static VEC3 set06_01_pos = { -3848.0f, 3061.0f, -8948.0f } ;
static VEC3 set06_01_rot = { 0.0f, 117.0f, 0.0f } ;
/* where it is on each axis, by frame */
static MOTION set06_01_x_key[] = {
	{ 0, -6000.0f }, { 6000, 2675.0f },
} ;
static MOTION set06_01_y_key[] = {
	{ 0, 3000.0f }, { 6000, 3000.0f },
} ;
static MOTION set06_01_z_key[] = {
	{ 0, -12200.0f }, { 6000, 4826.0f },
} ;

static void set06_01_main( PLAYER_WORK *ew ) ;
static void set06_01_rider_set( PLAYER_WORK *pl_w ) ;

/****************************************************************/
void set06_01_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set06_01_main ;
		ew->obj = obj_stg->obj[5] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_BRIGHT ;
		ew->pos = set06_01_pos ;
		ew->rot[0] = DEG_ANG( set06_01_rot.x ) ;
		ew->rot[1] = DEG_ANG( set06_01_rot.y ) ;
		ew->rot[2] = DEG_ANG( set06_01_rot.z ) ;
		set06_01_rider_set( ew ) ;
	}
}

/*==============================================================*/
static void set06_01_main( PLAYER_WORK *ew )
{
	if( ew->timer0 >= 6000 ){
		work_free( ew ) ;
		return ;
	}
	ew->pos.x = get_motion( set06_01_x_key, &ew->routine_0, ew->timer0 ) ;
	ew->pos.y = get_motion( set06_01_y_key, &ew->routine_1, ew->timer0 ) ;
	ew->pos.z = get_motion( set06_01_z_key, &ew->routine_2, ew->timer0 ) ;
	ew->timer0 ++ ;
}

static void set06_01_rider_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set06_01_rider_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set06_01_rider_main ;
		ew->obj = obj_stg->obj[11] ;
		ew->obj_flag = OBJ_BRIGHT | OBJ_SHADOW ;
		ew->base_mtx = &pl_w->obj_mtx ;
	}
}

/*==============================================================*/
static void set06_01_rider_main( PLAYER_WORK *ew )
{
	if( ew->timer0 >= 6000 ) work_free( ew ) ;
	else ew->timer0 ++ ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set06_01.c" End of File	*/

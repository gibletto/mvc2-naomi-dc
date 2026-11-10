/* Stage 06's object 0: models (objects 1 to 7 of the stage's file) that sway or turn about one axis
   with the frame count: two the one way, two the other with a model riding on each, and a fixed
   one. */
#include "player.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

static VEC3 set06_00_a_pos_tbl[2] = {
	{ -296.0f, 75.0f, -3157.0f },
	{ -465.0f, -90.0f, -2745.0f },
} ;
static Sint32 set06_00_a_rot_tbl[2] = { 55160, 55160, } ;
static VEC3 set06_00_b_pos_tbl[2] = {
	{ 1260.0f, 1100.0f, -4110.0f },
	{ 2650.0f, 1100.0f, -5070.0f },
} ;
static Sint32 set06_00_b_rot_tbl[2] = { 6554, 6554, } ;
static VEC3 set06_00_d_pos = { -354.0f, -120.0f, -2450.0f } ;
static VEC3 set06_00_d_rot = { -90.0f, 126.0f, 0.0f } ;

/*==============================================================*/
static void set06_00_a_main( PLAYER_WORK *ew )
{
	switch( ew->routine_0 ){
	case 0 :
		ew->timer0 ++ ;
		if( ew->timer0 >= 1440 ) ew->timer0 = 0 ;
		switch( ew->type ){
		case 0 :
			ew->rot[0] = DEG_ANG( ew->timer0 / 2.0f ) ;
			break ;
		case 1 :
			ew->rot[0] = DEG_ANG( ew->timer0 / 4.0f ) ;
			break ;
		}
		break ;
	}
}

/*==============================================================*/
static void set06_00_a_make( int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set06_00_a_main ;
		ew->obj = *( obj_stg->obj + no + 1 ) ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_BRIGHT ;
		ew->pos = set06_00_a_pos_tbl[no] ;
		ew->rot[1] = set06_00_a_rot_tbl[no] ;
		ew->type = no ;
	}
}

/*==============================================================*/
static void set06_00_b_main( PLAYER_WORK *ew )
{
	switch( ew->routine_0 ){
	case 0 :
		ew->timer0 += 2 ;
		if( ew->timer0 >= 360 ) ew->timer0 = 0 ;
		ew->rot[0] = DEG_ANG( nlSin( DEG_ANG( ew->timer0 ) ) * 45.0f ) ;
		break ;
	}
}

static void set06_00_b_arm_set( PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void set06_00_b_make( int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set06_00_b_main ;
		ew->obj = obj_stg->obj[3] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_BRIGHT ;
		ew->pos = set06_00_b_pos_tbl[no] ;
		ew->rot[1] = set06_00_b_rot_tbl[no] ;
		ew->timer0 = no * 180 ;
		set06_00_b_arm_set( ew ) ;
	}
}

/*==============================================================*/
static void set06_00_b_arm_main( PLAYER_WORK *ew )
{
	switch( ew->routine_0 ){
	case 0 :
		ew->timer0 += 3 ;
		if( ew->timer0 >= 360 ) ew->timer0 = 0 ;
		ew->rot[0] = DEG_ANG( nlSin( DEG_ANG( ew->timer0 ) ) * 20.0f ) ;
		break ;
	}
}

/*==============================================================*/
/*			The model that rides on one of the swaying ones		*/
/*==============================================================*/
static void set06_00_b_arm_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set06_00_b_arm_main ;
		ew->obj = obj_stg->obj[7] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_BRIGHT ;
		ew->base_mtx = &pl_w->obj_mtx ;
	}
}

/*==============================================================*/
static void set06_00_d_main( PLAYER_WORK *ew )
{
	switch( ew->routine_0 ){
	case 0 :
		ew->timer0 ++ ;
		if( ew->timer0 >= 360 ) ew->timer0 = 0 ;
		ew->rot[0] = DEG_ANG( -90.0f + nlSin( DEG_ANG( ew->timer0 ) ) * 45.0f ) ;
		break ;
	}
}

/*==============================================================*/
static void set06_00_d_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set06_00_d_main ;
		ew->obj = obj_stg->obj[6] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_ROT_Z | OBJ_BRIGHT ;
		ew->pos = set06_00_d_pos ;
		ew->rot[0] = DEG_ANG( set06_00_d_rot.x ) ;
		ew->rot[1] = DEG_ANG( set06_00_d_rot.y ) ;
		ew->rot[2] = DEG_ANG( set06_00_d_rot.z ) ;
	}
}

/****************************************************************/
void set06_00_set( void )
{
	int i ;

	for( i = 0 ; i < 2 ; i ++ ) set06_00_a_make( i ) ;
	for( i = 0 ; i < 2 ; i ++ ) set06_00_b_make( i ) ;
	set06_00_d_set() ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set06_00.c" End of File	*/

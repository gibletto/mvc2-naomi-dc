/* Stage 08's object 1: four lamp models along the wall that flicker between three poses, fade in
   once the round starts and then fade out; each makes a short-lived model that cycles through four
   poses. */
#include "player.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )


extern void obj_uv_begin( int *src, void *obj ) ;
extern int obj_uv_next( void ) ;
extern void obj_uv_get( Float *x, Float *y ) ;
extern void obj_uv_put( Float *x, Float *y ) ;

static VEC3 set08_01_pos_tbl[4] = {
	{ -780.0f, 350.0f, -200.0f },
	{ -270.0f, 260.0f, -270.0f },
	{ 270.0f, 260.0f, -270.0f },
	{ 780.0f, 350.0f, -200.0f },
} ;
static VEC3 set08_01_sub_pos_tbl[4] = {
	{ -780.0f, 340.0f, -210.0f },
	{ -270.0f, 260.0f, -280.0f },
	{ 270.0f, 260.0f, -280.0f },
	{ 780.0f, 340.0f, -210.0f },
} ;
static Sint8 set08_01_sub_obj_tbl[13] = { 0, 1, 2, 3, 2, 1, 3, 2, 1, 3, 2, 1, 0, } ;

static void set08_01_make( int no ) ;

/****************************************************************/
void set08_01_set( void )
{
	int i ;

	for( i = 0 ; i < 4 ; i ++ ) set08_01_make( i ) ;
}

static void set08_01_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set08_01_make( int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set08_01_main ;
		ew->obj = obj_stg->obj[no + 6] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Z | OBJ_FACE_X | OBJ_FACE_Y | OBJ_BRIGHT ;
		ew->pos = set08_01_pos_tbl[no] ;
		ew->type = no ;
	}
}

static void set08_01_sub_make( int no ) ;

/*==============================================================*/
static void set08_01_main( PLAYER_WORK *ew )
{
	switch( ew->routine_0 ){
	case 0 :
		if( ++ ew->timer0 >= 2 ){
			ew->timer0 = 0 ;
			ew->obj = obj_stg->obj[nlRand() % 4 + 6] ;
			ew->rot[2] = DEG_ANG( nlRand() % 90 ) ;
		}
		if( side_flag_get( 0 ) ) set08_01_sub_make( ew->type ) ;
		break ;
	}
	switch( ew->routine_1 ){
	case 0 :
		if( stage_work.event >= 1 ){
			ew->routine_1 ++ ;
			ew->obj_flag = ew->obj_flag | OBJ_FADE ;
			ew->fade.r = 1.0f ;
			ew->fade.g = 1.0f ;
			ew->fade.b = 1.0f ;
		}
		break ;
	case 1 :
		ew->fade.r -= 0.033333335f ;
		ew->fade.g -= 0.033333335f ;
		ew->fade.b -= 0.033333335f ;
		if( ++ ew->timer1 >= 30 ) work_free( ew ) ;
		break ;
	}
}

static void set08_01_sub_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set08_01_sub_make( int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set08_01_sub_main ;
		ew->obj = obj_stg->obj[set08_01_sub_obj_tbl[0] + 64] ;
		ew->obj_flag = OBJ_POS | OBJ_FACE_X | OBJ_FACE_Y | OBJ_BRIGHT ;
		ew->pos = set08_01_sub_pos_tbl[no] ;
	}
}

/*==============================================================*/
static void set08_01_sub_main( PLAYER_WORK *ew )
{
	ew->obj = obj_stg->obj[set08_01_sub_obj_tbl[ew->timer0 / 2] + 64] ;
	if( ( Uint32 )( ++ ew->timer0 ) >= 26 ) ew->timer0 = 0 ;
	switch( ew->routine_0 ){
	case 0 :
		if( ++ ew->timer1 >= 30 ){
			ew->routine_0 ++ ;
			ew->timer1 = 0 ;
			ew->obj_flag = ew->obj_flag | OBJ_FADE ;
			ew->fade.r = 1.0f ;
			ew->fade.g = 1.0f ;
			ew->fade.b = 1.0f ;
		}
		break ;
	case 1 :
		ew->fade.r -= 0.033333335f ;
		ew->fade.g -= 0.033333335f ;
		ew->fade.b -= 0.033333335f ;
		if( ++ ew->timer1 >= 30 ) work_free( ew ) ;
		break ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set08_01.c" End of File	*/

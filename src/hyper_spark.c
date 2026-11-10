/* The sparks of a hyper combo's screen flash (kind 8 works): 13 sparks that fly in to the player,
   then a ring and a glow that grow round him. */
#include "player.h"

/* two vectors the code does not use, and where the 13 sparks start from the player */
static VEC3 hyper_spark_unit = { 1.0f, 1.0f, 1.0f } ;
static VEC3 hyper_spark_half = { 0.5f, 0.5f, 0.5f } ;
static VEC3 hyper_spark_ofs[13] = {
	{ 360.0f, 380.0f, 1090.0f }, { 600.0f, 130.0f, 820.0f }, { 300.0f, -80.0f, 740.0f },
	{ 230.0f, -310.0f, 1130.0f }, { -90.0f, -530.0f, 670.0f }, { -70.0f, 60.0f, 1000.0f },
	{ -600.0f, 0.0f, 0.0f }, { -450.0f, 420.0f, 790.0f }, { -80.0f, 600.0f, 800.0f },
	{ 60.0f, 100.0f, 780.0f }, { -400.0f, -400.0f, 300.0f }, { -230.0f, 340.0f, 460.0f },
	{ 470.0f, -510.0f, 900.0f },
} ;

/*==============================================================*/
static void hyper_spark_move( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->pos.y += ew->spd.y ;
	ew->pos.z += ew->spd.z ;
}

/*==============================================================*/
static void hyper_spark_main( PLAYER_WORK *ew )
{
	hyper_spark_move( ew ) ;
	ew->scale.x -= 0.033333335f ;
	ew->scale.y -= 0.033333335f ;
	ew->scale.z -= 0.033333335f ;
	if( Sys->join_flag || ew->timer0 >= 15 ){
		work_free( ew ) ;
		return ;
	}
	ew->timer0 ++ ;
}

/*==============================================================*/
static void hyper_spark_set( VEC3 *pos, int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_MARK, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = hyper_spark_main ;
		ew->obj = obj_efky->obj[no + 44] ;
		ew->obj_flag = OBJ_POS | OBJ_FADE ;
		ew->pos = *pos ;
		ew->pos.x += hyper_spark_ofs[no].x ;
		ew->pos.y += hyper_spark_ofs[no].y ;
		ew->pos.z += hyper_spark_ofs[no].z ;
		ew->spd.x = -hyper_spark_ofs[no].x / 15.0f ;
		ew->spd.y = -hyper_spark_ofs[no].y / 15.0f ;
		ew->spd.z = -hyper_spark_ofs[no].z / 15.0f ;
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
		ew->scale.z = 1.0f ;
		ew->fade.r = 1.0f ;
		ew->fade.g = 1.0f ;
		ew->fade.b = 1.0f ;
	}
}

/*==============================================================*/
static void hyper_ring_main( PLAYER_WORK *ew )
{
	hyper_spark_move( ew ) ;
	ew->scale.x += ew->spd.x ;
	ew->scale.y += ew->spd.x ;
	ew->scale.z += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->fade.r -= 0.1f ;
	ew->fade.g -= 0.1f ;
	ew->fade.b -= 0.1f ;
	if( Sys->join_flag ){
		ew->disp_flag = 0 ;
		work_free( ew ) ;
		return ;
	}
	if( ew->timer0 >= 10 ){
		work_free( ew ) ;
		return ;
	}
	ew->timer0 ++ ;
}

/*==============================================================*/
static void hyper_ring_set( VEC3 *pos )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_MARK, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = hyper_ring_main ;
		ew->obj = obj_efky->obj[42] ;
		ew->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_FADE ;
		ew->pos = *pos ;
		ew->spd.x = 0.4f ;
		ew->acc.x = -0.04f ;
		ew->scale.x = 0.5f ;
		ew->scale.y = 0.5f ;
		ew->scale.z = 0.5f ;
		ew->fade.r = 1.0f ;
		ew->fade.g = 1.0f ;
		ew->fade.b = 1.0f ;
	}
}

/*==============================================================*/
static void hyper_glow_main( PLAYER_WORK *ew )
{
	ew->scale.x += 0.02f ;
	ew->scale.y += 0.02f ;
	ew->scale.z += 0.02f ;
	ew->fade.r -= 0.05f ;
	ew->fade.g -= 0.05f ;
	ew->fade.b -= 0.05f ;
	if( ew->timer0 >= 15 ){
		work_free( ew ) ;
		return ;
	}
	ew->timer0 ++ ;
}

/*==============================================================*/
static void hyper_glow_set( VEC3 *pos )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_MARK, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = hyper_glow_main ;
		ew->obj = obj_efky->obj[43] ;
		ew->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_FADE ;
		ew->pos = *pos ;
		ew->scale.x = 0.5f ;
		ew->scale.y = 0.5f ;
		ew->scale.z = 0.5f ;
		ew->fade.r = 1.0f ;
		ew->fade.g = 1.0f ;
		ew->fade.b = 1.0f ;
	}
}

/*==============================================================*/
static void hyper_flash_main( PLAYER_WORK *ew )
{
	int i ;

	if( Sys->join_flag ){
		ew->disp_flag = 0 ;
		work_free( ew ) ;
		return ;
	}
	switch( ew->routine_0 ){
	case 0:
		for( i = 0 ; i < 13 ; i ++ ) hyper_spark_set( &ew->pos, i ) ;
		ew->routine_0 ++ ;
		break ;
	case 1:
		if( ++ ew->timer0 > 15 ){
			hyper_ring_set( &ew->pos ) ;
			ew->routine_0 ++ ;
			ew->timer0 = 0 ;
		}
		break ;
	case 2:
		hyper_glow_set( &ew->pos ) ;
		work_free( ew ) ;
		break ;
	}
}

/****************************************************************/
void hyper_flash_set( VEC3 *pos, Sint16 dir, int a, int b )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_MARK, LIST_END ) ) ){
		ew->disp_flag = 0 ;
		ew->work_func = hyper_flash_main ;
		ew->pos = *pos ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "hyper_spark.c" End of File	*/

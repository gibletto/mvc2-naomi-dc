/* The four waves that spread from the player at the start of a delayed hyper combo (kind 8 works,
   started by pl_hyper_start): rings that grow, shrink and fade in turn. */
#include "player.h"

/*==============================================================*/
static void hyper_wave0_main( PLAYER_WORK *ew )
{
	switch( ew->routine_0 ){
	case 0:
		ew->scale.x += 0.062f ;
		ew->scale.y += 0.062f ;
		if( ++ ew->timer0 >= 8 ) ew->routine_0 ++ ;
		break ;
	case 1:
		ew->scale.x -= 0.042f ;
		ew->scale.y -= 0.042f ;
		if( ++ ew->timer0 >= 20 ) work_free( ew ) ;
		break ;
	}
}

/*==============================================================*/
static void hyper_wave0_set( VEC3 *pos )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_MARK, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = hyper_wave0_main ;
		ew->obj = obj_efky->obj[57] ;
		ew->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_FADE ;
		ew->pos = *pos ;
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
		ew->scale.z = 1.0f ;
		ew->fade.r = 1.0f ;
		ew->fade.g = 1.0f ;
		ew->fade.b = 1.0f ;
	}
}

/*==============================================================*/
static void hyper_wave1_main( PLAYER_WORK *ew )
{
	ew->rot[2] += 0x333 ;
	ew->timer0 ++ ;
	switch( ew->routine_0 ){
	case 0:
		ew->scale.x += 0.572f ;
		ew->scale.y += 0.572f ;
		if( ++ ew->timer0 >= 7 ) ew->routine_0 ++ ;
		break ;
	case 1:
		ew->scale.x -= 0.308f ;
		ew->scale.y -= 0.308f ;
		if( ++ ew->timer0 >= 20 ){
			work_free( ew ) ;
			return ;
		}
		break ;
	}
	switch( ew->routine_0 ){
	case 0:
		ew->fade.r += 0.5f ;
		ew->fade.g += 0.5f ;
		ew->fade.b += 0.5f ;
		if( ew->timer0 >= 2 ) ew->routine_1 ++ ;
		break ;
	case 1:
		if( ew->timer0 >= 12 ) ew->routine_1 ++ ;
		break ;
	case 2:
		ew->fade.r -= 0.125f ;
		ew->fade.g -= 0.125f ;
		ew->fade.b -= 0.125f ;
		if( ew->timer0 >= 20 ) work_free( ew ) ;
		break ;
	}
}

/*==============================================================*/
static void hyper_wave1_set( VEC3 *pos )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_MARK, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = hyper_wave1_main ;
		ew->obj = obj_efky->obj[58] ;
		ew->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_FADE ;
		ew->pos = *pos ;
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
		ew->scale.z = 1.0f ;
		ew->fade.r = 0.0f ;
		ew->fade.g = 0.0f ;
		ew->fade.b = 0.0f ;
	}
}

/*==============================================================*/
static void hyper_wave2_main( PLAYER_WORK *ew )
{
	switch( ew->routine_0 ){
	case 0:
		ew->scale.x += 0.428f ;
		ew->scale.y += 0.428f ;
		if( ++ ew->timer0 >= 7 ) ew->routine_0 ++ ;
		break ;
	case 1:
		ew->scale.x -= 0.23f ;
		ew->scale.y -= 0.23f ;
		if( ++ ew->timer0 >= 20 ){
			work_free( ew ) ;
			return ;
		}
		break ;
	}
	switch( ew->routine_0 ){
	case 0:
		ew->fade.r += 0.5f ;
		ew->fade.g += 0.5f ;
		ew->fade.b += 0.5f ;
		if( ew->timer0 >= 2 ) ew->routine_1 ++ ;
		break ;
	case 1:
		if( ew->timer0 >= 12 ) ew->routine_1 ++ ;
		break ;
	case 2:
		ew->fade.r -= 0.125f ;
		ew->fade.g -= 0.125f ;
		ew->fade.b -= 0.125f ;
		if( ew->timer0 >= 20 ) work_free( ew ) ;
		break ;
	}
}

/*==============================================================*/
static void hyper_wave2_set( VEC3 *pos )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_MARK, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = hyper_wave2_main ;
		ew->obj = obj_efky->obj[59] ;
		ew->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_FADE ;
		ew->pos = *pos ;
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
		ew->scale.z = 1.0f ;
		ew->fade.r = 0.0f ;
		ew->fade.g = 0.0f ;
		ew->fade.b = 0.0f ;
	}
}

/*==============================================================*/
static void hyper_wave3_main( PLAYER_WORK *ew )
{
	switch( ew->routine_0 ){
	case 0:
		ew->scale.x += 0.45f ;
		ew->scale.y += 0.45f ;
		if( ++ ew->timer0 >= 20 ) ew->routine_0 ++ ;
		break ;
	case 1:
		ew->scale.x -= 0.23f ;
		ew->scale.y -= 0.23f ;
		if( ++ ew->timer0 >= 20 ){
			work_free( ew ) ;
			return ;
		}
		break ;
	}
	switch( ew->routine_0 ){
	case 0:
		ew->fade.r += 0.2f ;
		ew->fade.g += 0.2f ;
		ew->fade.b += 0.2f ;
		if( ew->timer0 >= 5 ) ew->routine_1 ++ ;
		break ;
	case 1:
		if( ew->timer0 >= 15 ) ew->routine_1 ++ ;
		break ;
	case 2:
		ew->fade.r -= 0.2f ;
		ew->fade.g -= 0.2f ;
		ew->fade.b -= 0.2f ;
		if( ew->timer0 >= 20 ) work_free( ew ) ;
		break ;
	}
}

/*==============================================================*/
static void hyper_wave3_set( VEC3 *pos )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_MARK, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = hyper_wave3_main ;
		ew->obj = obj_efky->obj[60] ;
		ew->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_FADE ;
		ew->pos = *pos ;
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
		ew->scale.z = 1.0f ;
		ew->fade.r = 0.0f ;
		ew->fade.g = 0.0f ;
		ew->fade.b = 0.0f ;
	}
}

/*==============================================================*/
static void hyper_wave_main( PLAYER_WORK *ew )
{
	hyper_wave0_set( &ew->pos ) ;
	hyper_wave1_set( &ew->pos ) ;
	hyper_wave2_set( &ew->pos ) ;
	hyper_wave3_set( &ew->pos ) ;
	work_free( ew ) ;
}

static void hyper_wave_set( VEC3 *pos );

/****************************************************************/
void hyper_wave_start( PLAYER_WORK *pl_w, VEC3 *ofs )
{
	VEC3 pos ;

	if( pl_w->disp_dir ) pos.x = pl_w->pos.x - ofs->x ;
	else pos.x = pl_w->pos.x + ofs->x ;
	pos.y = pl_w->pos.y + ofs->y ;
	pos.z = 0.0f ;
	hyper_wave_set( &pos ) ;
}

/*==============================================================*/
static void hyper_wave_set( VEC3 *pos )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_MARK, LIST_END ) ) ){
		ew->disp_flag = 0 ;
		ew->work_func = hyper_wave_main ;
		ew->pos = *pos ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "hyper_wave.c" End of File	*/

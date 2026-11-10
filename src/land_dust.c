/* The dust of a player who lands hard or is thrown down (kind 7 works): four layers of smoke, each
   a model that swells and fades by its own steps, and a shake of the pad when a player is named
  . */
#include "player.h"

static VEC3 land_dust_scale = { 1.0f, 1.0f, 1.0f } ;

/*==============================================================*/
static void land_dust0_main( PLAYER_WORK *ew )
{
	ew->timer0 ++ ;
	switch( ew->routine_0 ){
	case 0:
		ew->scale.x += 0.025f ;
		ew->scale.z += 0.025f ;
		if( ew->timer0 >= 16 ) ew->routine_0 ++ ;
		break ;
	}
	switch( ew->routine_1 ){
	case 0:
		ew->scale.y += 0.083f ;
		if( ew->timer0 >= 6 ) ew->routine_1 ++ ;
		break ;
	case 1:
		ew->scale.y -= 0.036f ;
		break ;
	}
	switch( ew->routine_2 ){
	case 0:
		ew->fade.trnsl += 0.25f ;
		ew->fade.r += 0.25f ;
		ew->fade.g += 0.25f ;
		ew->fade.b += 0.25f ;
		if( ew->timer0 >= 4 ) ew->routine_2 ++ ;
		break ;
	case 1:
		if( ew->timer0 >= 10 ) ew->routine_2 ++ ;
		break ;
	case 2:
		ew->fade.trnsl -= 0.1f ;
		ew->fade.r -= 0.1f ;
		ew->fade.g -= 0.1f ;
		ew->fade.b -= 0.1f ;
		break ;
	}
	if( ew->timer0 > 20 ) work_free( ew ) ;
}

/*==============================================================*/
static void land_dust0_set( VEC3 *pos )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = land_dust0_main ;
		ew->obj = obj_efky->obj[62] ;
		ew->pos = *pos ;
		ew->scale = land_dust_scale ;
		ew->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL ;
	}
}

/*==============================================================*/
static void land_dust1_main( PLAYER_WORK *ew )
{
	ew->timer0 ++ ;
	switch( ew->routine_0 ){
	case 0:
		if( ew->timer0 >= 7 ) ew->routine_0 ++ ;
		break ;
	case 1:
		ew->scale.x += 0.046f ;
		ew->scale.z += 0.046f ;
		break ;
	}
	switch( ew->routine_1 ){
	case 0:
		ew->scale.y += 0.571f ;
		if( ew->timer0 >= 7 ) ew->routine_1 ++ ;
		break ;
	case 1:
		ew->scale.y -= 0.308f ;
		break ;
	}
	switch( ew->routine_2 ){
	case 0:
		ew->fade.trnsl += 0.16f ;
		ew->fade.r += 0.16f ;
		ew->fade.g += 0.16f ;
		ew->fade.b += 0.16f ;
		if( ew->timer0 >= 6 ) ew->routine_2 ++ ;
		break ;
	case 1:
		ew->fade.trnsl -= 0.083f ;
		ew->fade.r -= 0.083f ;
		ew->fade.g -= 0.083f ;
		ew->fade.b -= 0.083f ;
		if( ew->timer0 >= 18 ) ew->routine_2 ++ ;
		break ;
	case 2:
		break ;
	}
	if( ew->timer0 > 20 ) work_free( ew ) ;
}

/*==============================================================*/
static void land_dust1_set( VEC3 *pos )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = land_dust1_main ;
		ew->obj = obj_efky->obj[63] ;
		ew->pos = *pos ;
		ew->scale = land_dust_scale ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Z | OBJ_SCALE | OBJ_TRNSL ;
	}
}

/*==============================================================*/
static void land_dust2_main( PLAYER_WORK *ew )
{
	ew->timer0 ++ ;
	switch( ew->routine_0 ){
	case 0:
		ew->scale.x += 0.05f ;
		ew->scale.z += 0.05f ;
		if( ew->timer0 >= 20 ) ew->routine_0 ++ ;
		break ;
	}
	switch( ew->routine_1 ){
	case 0:
		ew->scale.y += 0.1875f ;
		if( ew->timer0 >= 8 ) ew->routine_1 ++ ;
		break ;
	case 1:
		ew->scale.y += -0.125f ;
		break ;
	}
	switch( ew->routine_2 ){
	case 0:
		ew->fade.trnsl += 0.14285715f ;
		ew->fade.r += 0.14285715f ;
		ew->fade.g += 0.14285715f ;
		ew->fade.b += 0.14285715f ;
		if( ew->timer0 >= 7 ) ew->routine_2 ++ ;
		break ;
	case 1:
		if( ew->timer0 >= 17 ) ew->routine_2 ++ ;
		break ;
	case 2:
		ew->fade.trnsl += -0.33333334f ;
		ew->fade.r += -0.33333334f ;
		ew->fade.g += -0.33333334f ;
		ew->fade.b += -0.33333334f ;
		break ;
	}
	if( ew->timer0 > 20 ) work_free( ew ) ;
}

/*==============================================================*/
static void land_dust2_set( VEC3 *pos )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = land_dust2_main ;
		ew->obj = obj_efky->obj[64] ;
		ew->pos = *pos ;
		ew->scale = land_dust_scale ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Z | OBJ_SCALE | OBJ_TRNSL ;
	}
}

/*==============================================================*/
static void land_dust3_main( PLAYER_WORK *ew )
{
	ew->timer0 ++ ;
	switch( ew->routine_0 ){
	case 0:
		ew->scale.x += 0.05f ;
		ew->scale.z += 0.05f ;
		break ;
	}
	switch( ew->routine_1 ){
	case 0:
		ew->scale.y += -0.025f ;
		break ;
	}
	switch( ew->routine_2 ){
	case 0:
		ew->fade.trnsl += 0.1f ;
		ew->fade.r += 0.1f ;
		ew->fade.g += 0.1f ;
		ew->fade.b += 0.1f ;
		if( ew->timer0 >= 10 ) ew->routine_2 ++ ;
		break ;
	case 1:
		if( ew->timer0 >= 13 ) ew->routine_2 ++ ;
		break ;
	case 2:
		ew->fade.trnsl += -0.14285715f ;
		ew->fade.r += -0.14285715f ;
		ew->fade.g += -0.14285715f ;
		ew->fade.b += -0.14285715f ;
		break ;
	}
	if( ew->timer0 > 20 ) work_free( ew ) ;
}

/*==============================================================*/
static void land_dust3_set( VEC3 *pos )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = land_dust3_main ;
		ew->obj = obj_efky->obj[65] ;
		ew->pos = *pos ;
		ew->scale = land_dust_scale ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Z | OBJ_SCALE | OBJ_TRNSL ;
	}
}

/****************************************************************/
void land_dust_set( VEC3 *pos, int no )
{
	if( no >= 0 ){
		vs_sys_work.land_flag = 1 ;
		vib_req( no, 4 ) ;
	}
	land_dust0_set( pos ) ;
	land_dust1_set( pos ) ;
	land_dust2_set( pos ) ;
	land_dust3_set( pos ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "land_dust.c" End of File	*/

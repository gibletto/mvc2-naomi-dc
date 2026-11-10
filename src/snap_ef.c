/* The flash of a snap back (kind 8 works): three models set together at the
   player's place, one that fades in and out, one that widens and thins, one that changes its model
   by frame. */
#include "player.h"

static void snap_flash0_set( VEC3 *pos ) ;
static void snap_flash1_set( VEC3 *pos ) ;
static void snap_flash2_set( VEC3 *pos ) ;

/****************************************************************/
/*			The dir, a and b of the caller are not used			*/
/****************************************************************/
void snap_flash_set( VEC3 *pos, Sint16 dir, int a, int b )
{
	snap_flash0_set( pos ) ;
	snap_flash1_set( pos ) ;
	snap_flash2_set( pos ) ;
}

static void snap_flash0_main( PLAYER_WORK *ew ) ;
static void snap_flash1_main( PLAYER_WORK *ew ) ;
static void snap_flash2_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void snap_flash0_set( VEC3 *pos )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_MARK, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = snap_flash0_main ;
		ew->obj = obj_efky->obj[220] ;
		ew->pos = *pos ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_TRNSL ;
		ew->timer0 = 0 ;
		ew->fade.trnsl = 0.0f ;
		ew->fade.r = 0.0f ;
		ew->fade.g = 0.0f ;
		ew->fade.b = 0.0f ;
	}
}

/*==============================================================*/
static void snap_flash1_set( VEC3 *pos )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_MARK, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = snap_flash1_main ;
		ew->obj = obj_efky->obj[221] ;
		ew->pos = *pos ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_SCALE | OBJ_TRNSL ;
		ew->timer0 = 0 ;
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
		ew->scale.z = 1.0f ;
		ew->fade.trnsl = 0.0f ;
		ew->fade.r = 0.0f ;
		ew->fade.g = 0.0f ;
		ew->fade.b = 0.0f ;
	}
}

/*==============================================================*/
static void snap_flash2_set( VEC3 *pos )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_MARK, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = snap_flash2_main ;
		ew->obj = obj_efky->obj[222] ;
		ew->pos = *pos ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_TRNSL ;
		ew->timer0 = 0 ;
		ew->fade.trnsl = 1.0f ;
		ew->fade.r = 0.0f ;
		ew->fade.g = 0.0f ;
		ew->fade.b = 0.0f ;
	}
}

/* its alpha by frame */
static Float snap_flash0_alpha[41] = {
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.6f, 0.4f, 0.3f, 0.2f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
} ;
static Float snap_flash1_alpha[41] = {
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.2f, 0.5f, 0.7f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.7f, 0.5f, 0.2f, 0.0f,
} ;
static Sint32 snap_flash2_obj_tbl[22] = {
	222, 222, 222, 223, 223, 223, 224, 224, 225, 225, 226, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236,
} ;
static Float snap_flash2_alpha[22] = {
	1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
} ;

/*==============================================================*/
static void snap_flash0_main( PLAYER_WORK *ew )
{
	if( ew->timer0 > 40 ){
		work_free( ew ) ;
		return ;
	}
	ew->fade.trnsl = snap_flash0_alpha[ew->timer0] ;
	ew->timer0 ++ ;
}

/*==============================================================*/
static void snap_flash1_main( PLAYER_WORK *ew )
{
	if( ew->timer0 > 40 ){
		work_free( ew ) ;
		return ;
	}
	if( ew->timer0 >= 28 ){
		ew->scale.x += DOT_X( 0.25f ) ;
		ew->scale.y -= 0.16333334f ;
	}
	if( ew->scale.y < 0.02f ) ew->scale.y = 0.02f ;
	ew->fade.trnsl = snap_flash1_alpha[ew->timer0] ;
	ew->timer0 ++ ;
}

/*==============================================================*/
static void snap_flash2_main( PLAYER_WORK *ew )
{
	if( ew->timer0 >= 22 ){
		work_free( ew ) ;
		return ;
	}
	ew->obj = obj_efky->obj[snap_flash2_obj_tbl[ew->timer0]] ;
	ew->fade.trnsl = snap_flash2_alpha[ew->timer0] ;
	ew->timer0 ++ ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "snap_ef.c" End of File	*/

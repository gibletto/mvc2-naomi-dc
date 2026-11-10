/* The flashes, rings and spreading shapes of a throw's release (`nage_nuke`). */
#include "player.h"

/* the release's fades: the flash, the two rings and the shape that spreads, a value per frame */
static Float nage_nuke_fade0_tbl[15] = {
	0.0f, 0.3f, 0.7f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.6f, 0.4f, 0.2f, 0.0f,
} ;
static Float nage_nuke_fade1_tbl[15] = {
	0.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.6f, 0.4f, 0.2f, 0.1f, 0.0f,
} ;
static Float nage_nuke_fade2_tbl[15] = {
	1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.7f, 0.5f, 0.4f, 0.2f, 0.0f,
} ;

static void nage_nuke_flash_set( VEC3 *pos );
static void nage_nuke_ring_set( VEC3 *pos );
static void nage_nuke_wave_set( VEC3 *pos );
static int nage_nuke_wave_x_set( VEC3 *pos );

/****************************************************************/
/* the last setter returns a number nobody reads, as in sp_ef_set */
void nage_nuke( VEC3 *pos )
{
	int r ;

	nage_nuke_flash_set( pos ) ;
	nage_nuke_ring_set( pos ) ;
	nage_nuke_wave_set( pos ) ;
	r = nage_nuke_wave_x_set( pos ) ;
}

static void nage_nuke_flash_main( PLAYER_WORK *ew );

/*==============================================================*/
static void nage_nuke_flash_set( VEC3 *pos )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = nage_nuke_flash_main ;
		ew->obj = obj_efky->obj[186] ;
		ew->pos = *pos ;
		ew->pos.y += DOT_Y( 64.0f ) ;
		ew->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL ;
		ew->timer0 = 0 ;
		ew->fade.trnsl = 1.0f ;
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
		ew->scale.z = 1.0f ;
	}
}

static void nage_nuke_ring_main( PLAYER_WORK *ew );

/*==============================================================*/
static void nage_nuke_ring_set( VEC3 *pos )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = nage_nuke_ring_main ;
		ew->obj = obj_efky->obj[186] ;
		ew->pos = *pos ;
		ew->pos.y += DOT_Y( 64.0f ) ;
		ew->obj_flag = OBJ_POS | OBJ_TRNSL ;
		ew->timer0 = 0 ;
		ew->timer1 = 0 ;
		ew->fade.trnsl = 1.0f ;
	}
}

static void nage_nuke_wave_main( PLAYER_WORK *ew );

/*==============================================================*/
static void nage_nuke_wave_set( VEC3 *pos )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = nage_nuke_wave_main ;
		ew->obj = obj_efky->obj[195] ;
		ew->pos = *pos ;
		ew->pos.y += DOT_Y( 64.0f ) ;
		ew->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL ;
		ew->timer0 = 0 ;
		ew->fade.trnsl = 1.0f ;
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
		ew->scale.z = 1.0f ;
	}
}

static void nage_nuke_wave_x_main( PLAYER_WORK *ew );

/*==============================================================*/
static int nage_nuke_wave_x_set( VEC3 *pos )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = nage_nuke_wave_x_main ;
		ew->obj = obj_efky->obj[196] ;
		ew->pos = *pos ;
		ew->pos.y += DOT_Y( 64.0f ) ;
		ew->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL ;
		ew->timer0 = 0 ;
		ew->fade.trnsl = 1.0f ;
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
		ew->scale.z = 1.0f ;
	}
}

/*==============================================================*/
static void nage_nuke_flash_main( PLAYER_WORK *ew )
{
	if( ew->timer0 > 15 ){
		work_free( ew ) ;
		return ;
	}
	ew->fade.trnsl = nage_nuke_fade0_tbl[ew->timer0] ;
	ew->scale.x += 0.12f ;
	ew->scale.y += 0.12f ;
	ew->timer0 ++ ;
}

/*==============================================================*/
static void nage_nuke_ring_main( PLAYER_WORK *ew )
{
	ew->obj = obj_efky->obj[186 + ew->timer1] ;
	if( ew->timer0 > 15 ){
		work_free( ew ) ;
		return ;
	}
	ew->fade.trnsl = nage_nuke_fade2_tbl[ew->timer0] ;
	ew->timer0 ++ ;
	if( ew->timer0 < 8 ) ew->timer1 ++ ;
}

/*==============================================================*/
static void nage_nuke_wave_main( PLAYER_WORK *ew )
{
	if( ew->timer0 > 14 ){
		work_free( ew ) ;
		return ;
	}
	ew->fade.trnsl = nage_nuke_fade1_tbl[ew->timer0] ;
	if( ew->timer0 < 9 ){
		ew->scale.x -= 0.1f ;
		ew->scale.y += 0.25f ;
	}
	if( ew->scale.x < 0.1f ) ew->scale.x = 0.1f ;
	ew->timer0 ++ ;
}

/*==============================================================*/
static void nage_nuke_wave_x_main( PLAYER_WORK *ew )
{
	if( ew->timer0 > 14 ){
		work_free( ew ) ;
		return ;
	}
	ew->fade.trnsl = nage_nuke_fade1_tbl[ew->timer0] ;
	if( ew->timer0 < 9 ){
		ew->scale.x += 0.25f ;
		ew->scale.y -= 0.1f ;
	}
	if( ew->scale.y < 0.1f ) ew->scale.y = 0.1f ;
	ew->timer0 ++ ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "nage_nuke.c" End of File	*/

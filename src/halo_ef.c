/* The halo that rides an effect a character made (kind 5 works, made by halo_ef_set for the
   effect it holds in ef_w): a ring that turns about it and two glows that cycle their colour. */
#include "player.h"

/* a 16-bit angle from degrees */
#define HALO_ANG( d )	( ( int )( ( d ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

static VEC3 halo_scale = { 0.5f, 0.5f, 0.5f } ;

/*==============================================================*/
static void halo_ring_main( PLAYER_WORK *ew )
{
	Sint16 ofs ;

	ew->pos = ew->ef_w->pos ;
	ofs = ( int )( ew->ef_w->scale.x * DOT_X( 8.0f ) ) ;
	if( !ew->ef_w->disp_dir ) ofs = -ofs ;
	ew->pos.x = ew->ef_w->pos.x + ofs ;
	ew->pos.y += ew->ef_w->scale.y * DOT_Y( 104.0f ) ;
	if( !ew->ef_w->hit_stop ){
		ew->timer0 ++ ;
		ew->rot[1] = HALO_ANG( ( ew->timer0 + ew->timer1 ) * 2 ) ;
		ew->rot[2] = HALO_ANG( ( ew->timer0 + ew->timer1 ) * 3 ) ;
	}
	if( ew->ef_w->routine_0 ) work_free( ew ) ;
	if( ew->ef_w->pl_type != PL_ABYSS1 ) work_free( ew ) ;
}

/*==============================================================*/
static void halo_ring_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = halo_ring_main ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_ROT_Z | OBJ_SCALE | OBJ_TRNSL ;
		ew->obj = obj_stg->obj[90] ;
		ew->ef_w = pl_w ;
		ew->fade.trnsl = 1.0f ;
		ew->scale = halo_scale ;
	}
}

/*==============================================================*/
static void halo_glow_main( PLAYER_WORK *ew )
{
	Sint16 ofs ;

	ew->pos = ew->ef_w->pos ;
	ofs = ( int )( ew->ef_w->scale.x * DOT_X( 8.0f ) ) ;
	if( !ew->ef_w->disp_dir ) ofs = -ofs ;
	ew->pos.x = ew->ef_w->pos.x + ofs ;
	ew->pos.y += ew->ef_w->scale.y * DOT_Y( 104.0f ) ;
	if( !ew->ef_w->hit_stop ){
		ew->timer0 ++ ;
		ew->rot[1] = HALO_ANG( ( ew->timer0 + ew->timer1 ) * 2 ) ;
		ew->rot[2] = HALO_ANG( ( ew->timer0 + ew->timer1 ) * 3 ) ;
		ew->fade.r = 0.5f + 0.5f * nlSin( HALO_ANG( ew->timer0 % 360 ) ) ;
		ew->fade.g = 0.5f + 0.5f * nlCos( HALO_ANG( ew->timer0 % 360 ) ) ;
	}
	if( ew->ef_w->routine_0 ) work_free( ew ) ;
	if( ew->ef_w->pl_type != PL_ABYSS1 ) work_free( ew ) ;
}

/*==============================================================*/
static void halo_glow0_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = halo_glow_main ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_ROT_Z | OBJ_SCALE | OBJ_FADE ;
		ew->obj = obj_stg->obj[88] ;
		ew->ef_w = pl_w ;
		ew->fade.r = 1.0f ;
		ew->fade.g = 0.0f ;
		ew->fade.b = 0.0f ;
		ew->timer1 = nlRand() % 360 ;
		ew->scale = halo_scale ;
	}
}

/*==============================================================*/
static void halo_glow1_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = halo_glow_main ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_ROT_Z | OBJ_SCALE | OBJ_FADE ;
		ew->obj = obj_stg->obj[89] ;
		ew->ef_w = pl_w ;
		ew->fade.r = 1.0f ;
		ew->fade.g = 0.0f ;
		ew->fade.b = 0.0f ;
		ew->timer1 = nlRand() % 360 ;
		ew->scale = halo_scale ;
	}
}

/****************************************************************/
/*			The ring and the two glows on an effect				*/
/****************************************************************/
void halo_ef_set( PLAYER_WORK *pl_w )
{
	halo_ring_set( pl_w ) ;
	halo_glow0_set( pl_w ) ;
	halo_glow1_set( pl_w ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "halo_ef.c" End of File	*/

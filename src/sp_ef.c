/* The effect on the ground where a special move starts (kind 7 works, set by pl_sp_ef_set): a
   flash that fades, a ring that swells and three bursts of pictures. */
#include "player.h"

/* the flash's grey, and the fades and models of the ring and the bursts, a value per frame */
static Float sp_flash_tbl[21] = {
	0.0f, 0.0f, 0.0f, 0.0f, 0.1f, 0.3f, 0.5f, 0.5f, 0.3f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
} ;
static Float sp_ring_tbl[12] = {
	0.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.6f, 0.4f, 0.2f, 0.0f,
} ;
static Float sp_fade_tbl[12] = {
	0.5f, 0.8f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.4f, 0.4f, 0.2f, 0.2f, 0.0f,
} ;
static Sint32 sp_obj_tbl0[11] = { 171, 172, 173, 174, 175, 176, 177, 178, 178, 179, 179 } ;
static Sint32 sp_obj_tbl1[11] = { 180, 181, 198, 199, 200, 201, 202, 203, 203, 204, 204 } ;
static Sint32 sp_obj_tbl2[11] = { 205, 206, 207, 208, 209, 210, 211, 212, 212, 213, 213 } ;

static void sp_flash_main( PLAYER_WORK *ew );
static void sp_ring_main( PLAYER_WORK *ew );
static void sp_burst0_main( PLAYER_WORK *ew );
static void sp_burst1_main( PLAYER_WORK *ew );
static void sp_burst2_main( PLAYER_WORK *ew );

static void sp_flash_set( VEC3 *pos, int dir );
static void sp_ring_set( VEC3 *pos, int dir );
static void sp_burst0_set( VEC3 *pos, int dir );
static void sp_burst1_set( VEC3 *pos, int dir );
static int sp_burst2_set( VEC3 *pos, int dir );

/****************************************************************/
/* the ground effect of a special move: the flash, the ring and the three bursts. The last setter
   returns a number nobody reads. */
void sp_ef_set( VEC3 *pos, int dir )
{
	int r ;

	sp_flash_set( pos, dir ) ;
	sp_ring_set( pos, dir ) ;
	sp_burst0_set( pos, dir ) ;
	sp_burst1_set( pos, dir ) ;
	r = sp_burst2_set( pos, dir ) ;
}

/*==============================================================*/
static void sp_flash_set( VEC3 *pos, int dir )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = sp_flash_main ;
		ew->obj = obj_efky->obj[169] ;
		ew->pos = *pos ;
		ew->pos.y += 10.0f ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_FADE ;
		ew->timer0 = 0 ;
		ew->timer1 = 0 ;
		ew->fade.trnsl = 1.0f ;
		ew->fade.r = 0.0f ;
		ew->fade.g = 0.0f ;
		ew->fade.b = 0.0f ;
		ew->disp_dir = dir ;
		ew->rot[1] += ew->disp_dir ? 0x8000 : 0 ;
	}
}

/*==============================================================*/
static void sp_ring_set( VEC3 *pos, int dir )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = sp_ring_main ;
		ew->pos = *pos ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_SCALE | OBJ_FADE ;
		ew->timer0 = 0 ;
		ew->timer1 = 0 ;
		ew->fade.trnsl = 1.0f ;
		ew->fade.r = 0.0f ;
		ew->fade.g = 0.0f ;
		ew->fade.b = 0.0f ;
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
		ew->scale.z = 1.0f ;
		ew->disp_dir = dir ;
		ew->rot[1] += ew->disp_dir ? 0x8000 : 0 ;
	}
}

/*==============================================================*/
static void sp_burst0_set( VEC3 *pos, int dir )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = sp_burst0_main ;
		ew->pos = *pos ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_TRNSL ;
		ew->timer0 = 0 ;
		ew->timer1 = 0 ;
		ew->fade.trnsl = 0.0f ;
		ew->disp_dir = dir ;
		ew->rot[1] += ew->disp_dir ? 0x8000 : 0 ;
	}
}

/*==============================================================*/
static void sp_burst1_set( VEC3 *pos, int dir )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = sp_burst1_main ;
		ew->pos = *pos ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_TRNSL ;
		ew->timer0 = 0 ;
		ew->timer1 = 0 ;
		ew->fade.trnsl = 0.0f ;
		ew->disp_dir = dir ;
		ew->rot[1] += ew->disp_dir ? 0x8000 : 0 ;
	}
}

/*==============================================================*/
static int sp_burst2_set( VEC3 *pos, int dir )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = sp_burst2_main ;
		ew->pos = *pos ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_TRNSL ;
		ew->timer0 = 0 ;
		ew->timer1 = 0 ;
		ew->fade.trnsl = 0.0f ;
		ew->disp_dir = dir ;
		ew->rot[1] += ew->disp_dir ? 0x8000 : 0 ;
	}
}

/*==============================================================*/
static void sp_flash_main( PLAYER_WORK *ew )
{
	if( ew->timer0 > 20 ){
		work_free( ew ) ;
		return ;
	}
	if( ew->timer0 < 8 ) ew->pos.y -= 6.0f ;
	ew->fade.r = sp_flash_tbl[ew->timer0] ;
	ew->fade.g = sp_flash_tbl[ew->timer0] ;
	ew->fade.b = sp_flash_tbl[ew->timer0] ;
	ew->timer0 ++ ;
}

/*==============================================================*/
static void sp_ring_main( PLAYER_WORK *ew )
{
	if( ew->timer0 < 7 ){
		ew->timer0 ++ ;
		return ;
	}
	if( ew->timer1 > 10 ){
		work_free( ew ) ;
		return ;
	}
	ew->obj = obj_efky->obj[170] ;
	ew->fade.r = sp_ring_tbl[ew->timer0] ;
	ew->fade.g = sp_ring_tbl[ew->timer0] ;
	ew->fade.b = sp_ring_tbl[ew->timer0] ;
	ew->scale.x += 0.13636364f ;
	ew->scale.z += 0.13636364f ;
	ew->scale.y -= 0.045454547f ;
	ew->rot[1] += ew->sub_type ? 0xba3 : -0xba3 ;
	ew->timer1 ++ ;
}

/*==============================================================*/
static void sp_burst0_main( PLAYER_WORK *ew )
{
	if( ew->timer0 < 9 ){
		ew->timer0 ++ ;
		return ;
	}
	if( ew->timer1 > 10 ){
		work_free( ew ) ;
		return ;
	}
	ew->obj = obj_efky->obj[sp_obj_tbl0[ew->timer1]] ;
	ew->fade.trnsl = sp_fade_tbl[ew->timer1] ;
	ew->timer1 ++ ;
}

/*==============================================================*/
static void sp_burst1_main( PLAYER_WORK *ew )
{
	if( ew->timer0 < 9 ){
		ew->timer0 ++ ;
		return ;
	}
	if( ew->timer1 > 10 ){
		work_free( ew ) ;
		return ;
	}
	ew->obj = obj_efky->obj[sp_obj_tbl1[ew->timer1]] ;
	ew->fade.trnsl = sp_fade_tbl[ew->timer1] ;
	ew->timer1 ++ ;
}

/*==============================================================*/
static void sp_burst2_main( PLAYER_WORK *ew )
{
	if( ew->timer0 < 9 ){
		ew->timer0 ++ ;
		return ;
	}
	if( ew->timer1 > 10 ){
		work_free( ew ) ;
		return ;
	}
	ew->obj = obj_efky->obj[sp_obj_tbl2[ew->timer1]] ;
	ew->fade.trnsl = sp_fade_tbl[ew->timer1] ;
	ew->timer1 ++ ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "sp_ef.c" End of File	*/

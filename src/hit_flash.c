/* The flashes of a big hit (kind 7 works): the burst that a hit with the 0x40 flag adds to its hit
   mark, and the pair of rings that grow and vanish. */
#include "player.h"

static VEC3 hit_ring_scale = { 1.0f, 1.0f, 1.0f } ;

/*==============================================================*/
static void hit_flash_ring0_main( PLAYER_WORK *ew )
{
	ew->scale.x += 0.8f ;
	ew->scale.y += 0.8f ;
	ew->scale.z += 0.8f ;
	if( ew->timer0 >= 10 ){
		work_free( ew ) ;
		return ;
	}
	ew->timer0 ++ ;
}

/*==============================================================*/
static void hit_flash_ring0_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = hit_flash_ring0_main ;
		ew->obj = obj_efky->obj[23] ;
		ew->pos = pl_w->pos ;
		ew->rot[2] = 0x1555 ;
		ew->scale = hit_ring_scale ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Z | OBJ_SCALE | OBJ_FACE_X | OBJ_FACE_Y ;
	}
}

/*==============================================================*/
static void hit_flash_ring1_main( PLAYER_WORK *ew )
{
	ew->scale.x += 0.8f ;
	ew->scale.y += 0.8f ;
	ew->scale.z += 0.8f ;
	if( ew->timer0 >= 10 ){
		work_free( ew ) ;
		return ;
	}
	ew->timer0 ++ ;
}

/*==============================================================*/
static void hit_flash_ring1_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = hit_flash_ring1_main ;
		ew->obj = obj_efky->obj[24] ;
		ew->pos = pl_w->pos ;
		ew->scale = hit_ring_scale ;
		ew->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_FACE_X | OBJ_FACE_Y ;
	}
}

/*==============================================================*/
static void hit_flash_main( PLAYER_WORK *ew )
{
	if( ew->timer0 == 2 ) hit_flash_ring0_set( ew ) ;
	if( ew->timer0 == 10 ) hit_flash_ring1_set( ew ) ;
	if( ew->timer0 >= 10 ){
		work_free( ew ) ;
		return ;
	}
	ew->timer0 ++ ;
}

/****************************************************************/
void hit_flash_set( VEC3 *pos, int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 0 ;
		ew->work_func = hit_flash_main ;
		ew->pos = *pos ;
	}
}

/*==============================================================*/
static void hit_burst_ring0_main( PLAYER_WORK *ew )
{
	ew->scale.x += 0.8f ;
	ew->scale.y += 0.8f ;
	ew->scale.z += 0.8f ;
	if( ew->timer0 >= 10 ){
		work_free( ew ) ;
		return ;
	}
	ew->timer0 ++ ;
}

/*==============================================================*/
static void hit_burst_ring0_set( PLAYER_WORK *pl_w, Uint8 kind )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, kind, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = hit_burst_ring0_main ;
		ew->obj = obj_efky->obj[23] ;
		ew->pos = pl_w->pos ;
		ew->rot[2] = 0x1555 ;
		ew->scale = hit_ring_scale ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Z | OBJ_SCALE | OBJ_FACE_X | OBJ_FACE_Y ;
	}
}

/*==============================================================*/
static void hit_burst_ring1_main( PLAYER_WORK *ew )
{
	ew->scale.x += 0.8f ;
	ew->scale.y += 0.8f ;
	ew->scale.z += 0.8f ;
	if( ew->timer0 >= 10 ){
		work_free( ew ) ;
		return ;
	}
	ew->timer0 ++ ;
}

/*==============================================================*/
static void hit_burst_ring1_set( PLAYER_WORK *pl_w, Uint8 kind )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, kind, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = hit_burst_ring1_main ;
		ew->obj = obj_efky->obj[24] ;
		ew->pos = pl_w->pos ;
		ew->scale = hit_ring_scale ;
		ew->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_FACE_X | OBJ_FACE_Y ;
	}
}

/*==============================================================*/
static void hit_burst_main( PLAYER_WORK *ew )
{
	if( ew->timer0 == 2 ) hit_burst_ring0_set( ew, ew->sub_no ) ;
	if( ew->timer0 == 10 ) hit_burst_ring1_set( ew, ew->sub_no ) ;
	if( ew->timer0 >= 10 ){
		vs_sys_work.hit_mark_cnt -- ;
		work_free( ew ) ;
		return ;
	}
	ew->timer0 ++ ;
}

/****************************************************************/
void hit_burst_set( VEC3 *pos, int no, Uint8 kind )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, kind, LIST_END ) ) ){
		vs_sys_work.hit_mark_cnt ++ ;
		ew->disp_flag = 0 ;
		ew->work_func = hit_burst_main ;
		ew->pos = *pos ;
		ew->sub_no = kind ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "hit_flash.c" End of File	*/

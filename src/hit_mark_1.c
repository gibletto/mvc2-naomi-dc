/* The second hit mark (kind 8 works; hit_mark.c picks it for marks 1 and 2 with
   hit_mark_1_set, and a variant without the sparks with hit_mark_1_plain_set): a work that counts the mark's
   eleven frames, with four pieces that ride its matrix and change by frame through the tables below. */
#include "player.h"

extern void hit_mark_burst_set( VEC3 *pos, int no, Uint8 b, Sint8 list ) ;

static VEC3 hit_mark_1_scale0 = { 1.0f, 1.0f, 1.0f } ;	/* never used */
static VEC3 hit_mark_1_scale = { 0.8f, 0.8f, 0.8f } ;
static MOTION hit_mark_1_a_fade_tbl[2] = { { 0, 1.0f }, { 5, 0.1f } } ;
static MOTION hit_mark_1_b_scale_tbl[2] = { { 0, 1.0f }, { 5, 1.5f } } ;
static MOTION hit_mark_1_b_fade_tbl[2] = { { 0, 1.0f }, { 5, 0.1f } } ;
static Sint32 hit_mark_1_c_obj_tbl[6] = { 108, 109, 110, 111, 112, 113 } ;
static Float hit_mark_1_c_fade_tbl[6] = { 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f } ;
static Sint32 hit_mark_1_d_obj_tbl[7] = { 114, 115, 116, 117, 118, 119, 120 } ;
static Float hit_mark_1_d_fade_tbl[7] = { 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f } ;

static void hit_mark_1_main( PLAYER_WORK *ew ) ;
static void hit_mark_1_a_set( PLAYER_WORK *master, Uint8 list ) ;
static void hit_mark_1_b_set( PLAYER_WORK *master, Uint8 list ) ;
static void hit_mark_1_c_set( PLAYER_WORK *master, Uint8 list ) ;
static void hit_mark_1_d_set( PLAYER_WORK *master, Uint8 list ) ;

/****************************************************************/
void hit_mark_1_set( VEC3 *pos, Uint16 dir, int no, Uint8 b, Sint8 list )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, list, LIST_END ) ) ){
		vs_sys_work.hit_mark_cnt ++ ;
		ew->disp_flag = 0 ;
		ew->work_func = hit_mark_1_main ;
		ew->pos = *pos ;
		ew->obj_flag = OBJ_POS | OBJ_FACE_X | OBJ_FACE_Y ;
		if( no == 1 ){
			ew->obj_flag = ew->obj_flag | OBJ_SCALE ;
			ew->scale = hit_mark_1_scale ;
		}
		hit_mark_1_a_set( ew, list ) ;
		hit_mark_1_b_set( ew, list ) ;
		hit_mark_1_c_set( ew, list ) ;
		hit_mark_1_d_set( ew, list ) ;
		hit_mark_burst_set( pos, no, b, list ) ;
	}
}

/****************************************************************/
void hit_mark_1_plain_set( VEC3 *pos, Uint16 dir, int no, int b, Sint8 list )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, list, LIST_END ) ) ){
		vs_sys_work.hit_mark_cnt ++ ;
		ew->disp_flag = 0 ;
		ew->work_func = hit_mark_1_main ;
		ew->pos = *pos ;
		ew->obj_flag = OBJ_POS | OBJ_FACE_X | OBJ_FACE_Y ;
		if( no == 1 ){
			ew->obj_flag = ew->obj_flag | OBJ_SCALE ;
			ew->scale = hit_mark_1_scale ;
		}
		hit_mark_1_a_set( ew, list ) ;
		hit_mark_1_b_set( ew, list ) ;
		hit_mark_1_c_set( ew, list ) ;
		hit_mark_1_d_set( ew, list ) ;
	}
}

/*==============================================================*/
static void hit_mark_1_main( PLAYER_WORK *ew )
{
	if( ew->timer0 > 10 ){
		vs_sys_work.hit_mark_cnt -- ;
		work_free( ew ) ;
		return ;
	}
	ew->timer0 ++ ;
}

static void hit_mark_1_a_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void hit_mark_1_a_set( PLAYER_WORK *master, Uint8 list )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, list, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = hit_mark_1_a_main ;
		ew->obj = obj_efky->obj[82] ;
		ew->obj_flag = OBJ_FADE ;
		ew->base_mtx = &master->obj_mtx ;
	}
}

/*==============================================================*/
static void hit_mark_1_a_main( PLAYER_WORK *ew )
{
	if( ew->timer0 >= 4 ){
		work_free( ew ) ;
		return ;
	}
	ew->fade.r = get_motion( hit_mark_1_a_fade_tbl, &ew->routine_0, ew->timer0 ) ;
	ew->fade.g = ew->fade.r ;
	ew->fade.b = ew->fade.r ;
	ew->timer0 ++ ;
}

static void hit_mark_1_b_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void hit_mark_1_b_set( PLAYER_WORK *master, Uint8 list )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, list, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = hit_mark_1_b_main ;
		ew->obj = obj_efky->obj[83] ;
		ew->obj_flag = OBJ_SCALE | OBJ_FADE ;
		ew->base_mtx = &master->obj_mtx ;
	}
}

/*==============================================================*/
static void hit_mark_1_b_main( PLAYER_WORK *ew )
{
	if( ew->timer0 >= 5 ){
		work_free( ew ) ;
		return ;
	}
	ew->scale.x = get_motion( hit_mark_1_b_scale_tbl, &ew->routine_0, ew->timer0 ) ;
	ew->scale.y = ew->scale.x ;
	ew->fade.r = get_motion( hit_mark_1_b_fade_tbl, &ew->routine_1, ew->timer0 ) ;
	ew->fade.g = ew->fade.r ;
	ew->fade.b = ew->fade.r ;
	ew->timer0 ++ ;
}

static void hit_mark_1_c_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void hit_mark_1_c_set( PLAYER_WORK *master, Uint8 list )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, list, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = hit_mark_1_c_main ;
		ew->obj_flag = OBJ_FADE ;
		ew->base_mtx = &master->obj_mtx ;
	}
}

/*==============================================================*/
static void hit_mark_1_c_main( PLAYER_WORK *ew )
{
	if( ew->timer0 >= 6 ){
		work_free( ew ) ;
		return ;
	}
	ew->obj = obj_efky->obj[hit_mark_1_c_obj_tbl[ew->timer0]] ;
	ew->fade.g = ew->fade.r = hit_mark_1_c_fade_tbl[ew->timer0] ;
	ew->fade.b = ew->fade.r ;
	ew->timer0 ++ ;
}

static void hit_mark_1_d_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void hit_mark_1_d_set( PLAYER_WORK *master, Uint8 list )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, list, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = hit_mark_1_d_main ;
		ew->obj_flag = OBJ_FADE ;
		ew->base_mtx = &master->obj_mtx ;
	}
}

/*==============================================================*/
static void hit_mark_1_d_main( PLAYER_WORK *ew )
{
	if( ew->timer0 >= 7 ){
		work_free( ew ) ;
		return ;
	}
	ew->obj = obj_efky->obj[hit_mark_1_d_obj_tbl[ew->timer0]] ;
	ew->fade.g = ew->fade.r = hit_mark_1_d_fade_tbl[ew->timer0] ;
	ew->fade.b = ew->fade.r ;
	ew->timer0 ++ ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "hit_mark_1.c" End of File	*/

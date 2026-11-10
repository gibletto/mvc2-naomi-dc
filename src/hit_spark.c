/* The sparks of hit marks 4 to 6 (kinds given by the caller, 8 from hitmark_set):
   a burst made at the blow's angle, each ten frames or so adding pieces that swell and fade
   by their own curves and a spread of stars that step through their pictures. */
#include "player.h"

/* a scale the code does not use, then each piece's curves: its scale and its fade */
static VEC3 hit_spark_unit = { 1.0f, 1.0f, 1.0f } ;
static MOTION hit_spark_a_scale_tbl[2] = { { 0, 1.0f }, { 6, 1.2f } } ;
static MOTION hit_spark_a_fade_tbl[2] = { { 0, 1.0f }, { 6, 0.1f } } ;
static MOTION hit_spark_b_scale_tbl[2] = { { 0, 1.07f }, { 4, 1.2f } } ;
static MOTION hit_spark_b_fade_tbl[3] = { { 0, 1.0f }, { 3, 1.0f }, { 4, 0.5f } } ;
static MOTION hit_spark_c_scale_tbl[2] = { { 0, 1.17f }, { 4, 1.32f } } ;
static MOTION hit_spark_c_fade_tbl[3] = { { 0, 1.0f }, { 3, 1.0f }, { 4, 0.5f } } ;
static MOTION hit_spark_d_scale_tbl[2] = { { 0, 1.2f }, { 6, 1.5f } } ;
static MOTION hit_spark_d_fade_tbl[3] = { { 0, 1.0f }, { 3, 1.0f }, { 6, 0.2f } } ;
/* the last piece's scale, turn, place and fade */
static MOTION hit_spark_e_scale_tbl[2] = { { 0, 1.0f }, { 15, 1.5f } } ;
static MOTION hit_spark_e_ang_tbl[2] = { { 0, 0.0f }, { 15, 180.0f } } ;
static MOTION hit_spark_e_pos_tbl[2] = { { 0, 40.0f }, { 15, 60.0f } } ;
static MOTION hit_spark_e_fade_tbl[3] = { { 0, 1.0f }, { 10, 1.0f }, { 15, 0.2f } } ;
/* the stars of six kinds: the picture and fade of each frame */
static MOTION hit_spark_star0_tbl[5] = { { 0x81, 0.3f }, { 0x82, 0.5f }, { 0x83, 1.0f }, { 0x84, 1.0f }, { 0x85, 1.0f } } ;
static MOTION hit_spark_star1_tbl[5] = { { 0x87, 0.3f }, { 0x88, 0.5f }, { 0x89, 1.0f }, { 0x8a, 1.0f }, { 0x8b, 1.0f } } ;
static MOTION hit_spark_star2_tbl[4] = { { 0x8d, 0.5f }, { 0x8e, 1.0f }, { 0x8f, 1.0f }, { 0x90, 1.0f } } ;
static MOTION hit_spark_star3_tbl[7] = {
	{ 0x92, 0.3f }, { 0x93, 0.5f }, { 0x94, 1.0f }, { 0x95, 1.0f }, { 0x96, 1.0f }, { 0x97, 1.0f }, { 0x98, 1.0f },
} ;
static MOTION hit_spark_star4_tbl[4] = { { 0x99, 0.5f }, { 0x9a, 1.0f }, { 0x9b, 1.0f }, { 0x9c, 1.0f } } ;
static MOTION hit_spark_star5_tbl[7] = {
	{ 0x9e, 0.3f }, { 0x9f, 0.5f }, { 0xa0, 1.0f }, { 0xa1, 1.0f }, { 0xa2, 1.0f }, { 0xa3, 1.0f }, { 0xa4, 1.0f },
} ;
static MOTION *hit_spark_star_tbl[6] = {
	hit_spark_star0_tbl, hit_spark_star1_tbl, hit_spark_star2_tbl,
	hit_spark_star3_tbl, hit_spark_star4_tbl, hit_spark_star5_tbl,
} ;
static Sint32 hit_spark_star_cnt[6] = { 5, 5, 4, 7, 4, 7 } ;

static void hit_spark_main( PLAYER_WORK *ew ) ;

/****************************************************************/
/*			The first piece of a spark at pos, turned by b:		*/
/*			the blow's angle in degrees							*/
/****************************************************************/
void hit_mark_spark_set( VEC3 *pos, Uint16 dir, int no, Uint8 b, Sint8 list )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, list, LIST_END ) ) ){
		vs_sys_work.hit_mark_cnt ++ ;
		ew->disp_flag = 0 ;
		ew->work_func = hit_spark_main ;
		ew->pos = *pos ;
		ew->sub_no = list ;
		ew->rot[2] = ( ( int )( b * 737280.0f / 360.0f + 0.5f ) & 0xffff ) + 0xc000 ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Z | OBJ_FACE_X | OBJ_FACE_Y ;
	}
}

static void hit_spark_a_set( PLAYER_WORK *par_w, Uint8 list ) ;
static void hit_spark_b_set( PLAYER_WORK *par_w, Uint8 list ) ;
static void hit_spark_c_set( PLAYER_WORK *par_w, Uint8 list ) ;
static void hit_spark_d_set( PLAYER_WORK *par_w, Uint8 list ) ;
static void hit_spark_e_set( PLAYER_WORK *par_w, Uint8 list ) ;
static void hit_spark_star_set( PLAYER_WORK *par_w, int no, Uint8 list ) ;

/*==============================================================*/
static void hit_spark_main( PLAYER_WORK *ew )
{
	switch( ew->timer0 ){
	case 0:
		hit_spark_a_set( ew, ew->sub_no ) ;
		hit_spark_e_set( ew, ew->sub_no ) ;
		break ;
	case 3:
		hit_spark_b_set( ew, ew->sub_no ) ;
		break ;
	case 4:
		hit_spark_star_set( ew, 0, ew->sub_no ) ;
		break ;
	case 6:
		hit_spark_c_set( ew, ew->sub_no ) ;
		hit_spark_star_set( ew, 1, ew->sub_no ) ;
		break ;
	case 7:
		hit_spark_star_set( ew, 3, ew->sub_no ) ;
		break ;
	case 9:
		hit_spark_d_set( ew, ew->sub_no ) ;
		break ;
	case 10:
		hit_spark_star_set( ew, 2, ew->sub_no ) ;
		break ;
	case 12:
		hit_spark_star_set( ew, 5, ew->sub_no ) ;
		break ;
	case 14:
		hit_spark_star_set( ew, 4, ew->sub_no ) ;
		break ;
	}
	if( ew->timer0 > 18 ){
		work_free( ew ) ;
		vs_sys_work.hit_mark_cnt -- ;
		return ;
	}
	ew->timer0 ++ ;
}

static void hit_spark_a_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void hit_spark_a_set( PLAYER_WORK *par_w, Uint8 list )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, list, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = hit_spark_a_main ;
		ew->obj = obj_efky->obj[124] ;
		ew->obj_flag = OBJ_SCALE | OBJ_FADE ;
		ew->base_mtx = &par_w->obj_mtx ;
	}
}

/*==============================================================*/
static void hit_spark_a_main( PLAYER_WORK *ew )
{
	if( ew->timer0 > 6 ){
		work_free( ew ) ;
		return ;
	}
	ew->scale.x = get_motion( hit_spark_a_scale_tbl, &ew->routine_0, ew->timer0 ) ;
	ew->scale.y = ew->scale.x ;
	ew->scale.z = ew->scale.x ;
	ew->fade.r = get_motion( hit_spark_a_fade_tbl, &ew->routine_1, ew->timer0 ) ;
	ew->fade.g = ew->fade.r ;
	ew->fade.b = ew->fade.r ;
	ew->timer0 ++ ;
}

static void hit_spark_b_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void hit_spark_b_set( PLAYER_WORK *par_w, Uint8 list )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, list, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = hit_spark_b_main ;
		ew->obj = obj_efky->obj[125] ;
		ew->obj_flag = OBJ_SCALE | OBJ_FADE ;
		ew->base_mtx = &par_w->obj_mtx ;
	}
}

/*==============================================================*/
static void hit_spark_b_main( PLAYER_WORK *ew )
{
	if( ew->timer0 > 4 ){
		work_free( ew ) ;
		return ;
	}
	ew->scale.x = get_motion( hit_spark_b_scale_tbl, &ew->routine_0, ew->timer0 ) ;
	ew->scale.y = ew->scale.x ;
	ew->scale.z = ew->scale.x ;
	ew->fade.r = get_motion( hit_spark_b_fade_tbl, &ew->routine_1, ew->timer0 ) ;
	ew->fade.g = ew->fade.r ;
	ew->fade.b = ew->fade.r ;
	ew->timer0 ++ ;
}

static void hit_spark_c_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void hit_spark_c_set( PLAYER_WORK *par_w, Uint8 list )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, list, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = hit_spark_c_main ;
		ew->obj = obj_efky->obj[126] ;
		ew->obj_flag = OBJ_SCALE | OBJ_FADE ;
		ew->base_mtx = &par_w->obj_mtx ;
	}
}

/*==============================================================*/
static void hit_spark_c_main( PLAYER_WORK *ew )
{
	if( ew->timer0 > 4 ){
		work_free( ew ) ;
		return ;
	}
	ew->scale.x = get_motion( hit_spark_c_scale_tbl, &ew->routine_0, ew->timer0 ) ;
	ew->scale.y = ew->scale.x ;
	ew->scale.z = ew->scale.x ;
	ew->fade.r = get_motion( hit_spark_c_fade_tbl, &ew->routine_1, ew->timer0 ) ;
	ew->fade.g = ew->fade.r ;
	ew->fade.b = ew->fade.r ;
	ew->timer0 ++ ;
}

static void hit_spark_d_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void hit_spark_d_set( PLAYER_WORK *par_w, Uint8 list )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, list, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = hit_spark_d_main ;
		ew->obj = obj_efky->obj[127] ;
		ew->obj_flag = OBJ_SCALE | OBJ_FADE ;
		ew->base_mtx = &par_w->obj_mtx ;
	}
}

/*==============================================================*/
static void hit_spark_d_main( PLAYER_WORK *ew )
{
	if( ew->timer0 > 6 ){
		work_free( ew ) ;
		return ;
	}
	ew->scale.x = get_motion( hit_spark_d_scale_tbl, &ew->routine_0, ew->timer0 ) ;
	ew->scale.y = ew->scale.x ;
	ew->scale.z = ew->scale.x ;
	ew->fade.r = get_motion( hit_spark_d_fade_tbl, &ew->routine_1, ew->timer0 ) ;
	ew->fade.g = ew->fade.r ;
	ew->fade.b = ew->fade.r ;
	ew->timer0 ++ ;
}

static void hit_spark_e_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void hit_spark_e_set( PLAYER_WORK *par_w, Uint8 list )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, list, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = hit_spark_e_main ;
		ew->obj = obj_efky->obj[128] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_SCALE | OBJ_FADE ;
		ew->base_mtx = &par_w->obj_mtx ;
	}
}

/*==============================================================*/
static void hit_spark_e_main( PLAYER_WORK *ew )
{
	if( ew->timer0 > 15 ){
		work_free( ew ) ;
		return ;
	}
	ew->scale.x = get_motion( hit_spark_e_scale_tbl, &ew->routine_0, ew->timer0 ) ;
	ew->scale.y = ew->scale.x ;
	ew->scale.z = ew->scale.x ;
	ew->rot[0] = ( int )( get_motion( hit_spark_e_ang_tbl, &ew->routine_1, ew->timer0 ) * 65536.0f / 360.0f + 0.5f ) & 0xffff ;
	ew->pos.x = get_motion( hit_spark_e_pos_tbl, &ew->routine_2, ew->timer0 ) ;
	ew->fade.r = get_motion( hit_spark_e_fade_tbl, &ew->routine_3, ew->timer0 ) ;
	ew->fade.g = ew->fade.r ;
	ew->fade.b = ew->fade.r ;
	ew->timer0 ++ ;
}

static void hit_spark_star_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void hit_spark_star_set( PLAYER_WORK *par_w, int no, Uint8 list )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, list, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = hit_spark_star_main ;
		ew->obj_flag = OBJ_FADE ;
		ew->base_mtx = &par_w->obj_mtx ;
		ew->type = no ;
	}
}

/*==============================================================*/
static void hit_spark_star_main( PLAYER_WORK *ew )
{
	if( ew->timer0 >= hit_spark_star_cnt[ew->type] ){
		work_free( ew ) ;
		return ;
	}
	ew->obj = obj_efky->obj[hit_spark_star_tbl[ew->type][ew->timer0].frame] ;
	ew->fade.g = ew->fade.r = hit_spark_star_tbl[ew->type][ew->timer0].val ;
	ew->fade.b = ew->fade.r ;
	ew->timer0 ++ ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "hit_spark.c" End of File	*/

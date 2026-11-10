/* Landing and dust effects of the common code: the two bursts of sparks where a player lands (kinds
   5 and 7) and the puffs of dust at his feet when he slides (kind 7). */
#include "player.h"

/* the model each frame shows */
static Sint32 dust_obj_no0[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 } ;
static Sint32 dust_obj_no1[18] = { 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9 } ;
static Sint32 *dust_obj_tbl[4] = { dust_obj_no0, dust_obj_no0, dust_obj_no0, dust_obj_no1 } ;
static Sint32 dust_frame_cnt[4] = { 9, 9, 9, 18 } ;
static Float land_unk_tbl[3] = { 40.0f, 70.0f, 100.0f } ;
/* how fast the sparks fly out, and their scale, for each of the three sizes */
static Float land_spd_tbl[3] = { 15.0f, 30.0f, 15.0f } ;
static VEC3 land_scale_tbl[3] = {
	{ 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.3f, 0.3f, 0.3f },
} ;

/* the sixth part of a turn for each spark, as a 16-bit angle */
#define LAND_ANG( i )	( ( int )( ( i ) * 60 * 65536.0f / 360.0f + 0.5f ) & 0xffff )

/*==============================================================*/
static void land_big_main( PLAYER_WORK *ew )
{
	if( ++ ew->timer1 % 4 == 0 ){
		if( ++ ew->timer0 >= dust_frame_cnt[0] ){
			work_free( ew ) ;
			return ;
		}
		ew->obj = obj_efky->obj[dust_obj_tbl[0][ew->timer0]] ;
	}
	ew->pos.x += ew->spd.x ;
	ew->pos.z += ew->spd.z ;
}

/*==============================================================*/
static void land_big_one_set( VEC3 *pos, int no, int i )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = land_big_main ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Z | OBJ_SCALE | OBJ_FACE_Y ;
		ew->pos = *pos ;
		ew->spd.x = land_spd_tbl[no] * nlSin( LAND_ANG( i ) ) ;
		ew->spd.z = land_spd_tbl[no] * nlCos( LAND_ANG( i ) ) ;
		ew->rot[2] = nlRand() % 0x4000 ;
		ew->scale = land_scale_tbl[no] ;
		ew->scale.x *= 50.0f ;
		ew->scale.y *= 50.0f ;
		ew->scale.z *= 1.0f ;
		ew->obj = obj_efky->obj[dust_obj_tbl[0][ew->timer0]] ;
		ew->timer1 = 0 ;
	}
}

/****************************************************************/
void land_big_ef_set( VEC3 *pos, Sint8 no )
{
	int i ;

	if( no > 2 ) return ;
	for( i = 0 ; i < 6 ; i ++ ) land_big_one_set( pos, no, i ) ;
}

/*==============================================================*/
static void land_ef_main( PLAYER_WORK *ew )
{
	ew->obj = obj_efky->obj[dust_obj_tbl[0][ew->timer0]] ;
	ew->pos.x += ew->spd.x ;
	ew->pos.z += ew->spd.z ;
	if( ++ ew->timer0 >= dust_frame_cnt[0] ) work_free( ew ) ;
}

/*==============================================================*/
static void land_ef_one_set( VEC3 *pos, int no, int i )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = land_ef_main ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Z | OBJ_FACE_Y ;
		ew->pos = *pos ;
		ew->spd.x = land_spd_tbl[no] * nlSin( LAND_ANG( i ) ) ;
		ew->spd.z = land_spd_tbl[no] * nlCos( LAND_ANG( i ) ) ;
		ew->rot[2] = nlRand() % 0x4000 ;
		ew->scale = land_scale_tbl[no] ;
	}
}

/****************************************************************/
void land_ef_set( VEC3 *pos, Sint8 no )
{
	int i ;

	if( no > 2 ) return ;
	for( i = 0 ; i < 6 ; i ++ ) land_ef_one_set( pos, no, i ) ;
}

/*==============================================================*/
static void dust_main( PLAYER_WORK *ew )
{
	ew->obj = obj_efky->obj[dust_obj_tbl[0][ew->timer0]] ;
	if( ++ ew->timer0 >= dust_frame_cnt[0] ) work_free( ew ) ;
}

/****************************************************************/
void dust_set( PLAYER_WORK *pl_w, Uint8 x, Uint8 y, Float a, Float b )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = dust_main ;
		ew->obj_flag = OBJ_POS | OBJ_SCALE ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		ew->pos.z = 0 ;
		ew->pos.x += pl_w->disp_dir ? x : -x ;
		ew->pos.y += y ;
		ew->scale.x = a ;
		ew->scale.y = b ;
		ew->scale.z = 0 ;
	}
}

/****************************************************************/
void dust_ofs_set( PLAYER_WORK *pl_w, VEC3 *ofs, Float a, Float b )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = dust_main ;
		ew->obj_flag = OBJ_POS | OBJ_SCALE ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		ew->pos.z = 0 ;
		ew->pos.x += pl_w->disp_dir ? ofs->x : -ofs->x ;
		ew->pos.y += ofs->y ;
		ew->scale.x = a ;
		ew->scale.y = b ;
		ew->scale.z = 0 ;
	}
}

/****************************************************************/
void slide_dust_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = dust_main ;
		ew->obj_flag = OBJ_POS | OBJ_SCALE ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		ew->pos.z = 0 ;
		ew->pos.x += pl_w->disp_dir ? -32 : 32 ;
		ew->pos.y = pl_w->ground_y ;
		ew->scale.x = 1 ;
		ew->scale.y = 1 ;
		ew->scale.z = 0 ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "ef_dust.c" End of File	*/

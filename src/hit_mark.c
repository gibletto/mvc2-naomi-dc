/* The hit marks: the sparks and flashes made where a blow lands. `hitmark_set` picks the mark by the
   attack's hit mark number. */
#define HIT_MARK_SET_NEW
#include "player.h"

/* the first mark's data: the ring's scale, curves for its pieces and the star's pictures and fades */
static VEC3 hit_mark_ring_scale = { 1.0f, 1.0f, 1.0f } ;
static VEC3 hit_mark_unk_0C260B94 = { 0.5f, 0.5f, 0.5f } ;
static MOTION hit_mark_fade0_tbl[2] = { { 0, 1.0f }, { 5, 0.1f } } ;
static MOTION hit_mark_fade1_tbl[3] = { { 0, 0.0f }, { 3, 1.0f }, { 8, 0.2f } } ;
static MOTION hit_mark_scale_y_tbl[3] = { { 0, 1.0f }, { 5, 0.12f }, { 9, 0.12f } } ;
static MOTION hit_mark_scale_z_tbl[3] = { { 0, 1.0f }, { 5, 1.0f }, { 9, 7.0f } } ;
static MOTION hit_mark_ang_tbl[2] = { { 0, 0.0f }, { 11, 45.0f } } ;
static MOTION hit_mark_fade2_tbl[2] = { { 0, 100.0f }, { 10, 0.2f } } ;
static Sint32 hit_mark_star_obj_tbl[6] = { 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d } ;
static Float hit_mark_star_fade_tbl[6] = { 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.2f } ;

/* the marks of each kind: this module's first, the others in modules after it */
static void hit_mark_make( VEC3 *pos, Uint16 dir, int no, Uint8 b, Sint8 list );
extern void hit_mark_1_set( VEC3 *pos, Uint16 dir, int no, Uint8 b, Sint8 list );
extern void hit_mark_1_plain_set( VEC3 *pos, Uint16 dir, int no, int b, Sint8 list );
extern void hit_mark_spark_set( VEC3 *pos, Uint16 dir, int no, Uint8 b, Sint8 list );
extern void hit_mark_rings_set( VEC3 *pos, Uint16 dir, int no, Uint8 b, Sint8 list );
extern void move_pos_spd( PLAYER_WORK *ew );

/****************************************************************/
/*			The value of a curve at a frame: it goes			*/
/*			straight from point to point, and *no moves to		*/
/*			the next point when the frame reaches it			*/
/****************************************************************/
Float get_motion( MOTION *tbl, Uint8 *no, Sint16 frame )
{
	Float val ;
	Float dt, dv ;

	dt = tbl[*no + 1].frame - tbl[*no].frame ;
	dv = tbl[*no + 1].val - tbl[*no].val ;
	val = tbl[*no].val + ( frame - tbl[*no].frame ) * dv / dt ;
	if( frame >= tbl[*no + 1].frame ) ( *no ) ++ ;
	return val ;
}

/****************************************************************/
void hitmark_set( VEC3 *pos, Uint16 dir, int no, Uint8 b )
{
	if( vs_sys_work.hit_mark_cnt > 5 ) return ;
	switch( no ){
	case 0:
		hit_mark_make( pos, dir, no, b, 8 ) ;
		break ;
	case 1:
		hit_mark_1_set( pos, dir, no, b, 8 ) ;
		break ;
	case 2:
		hit_mark_1_set( pos, dir, no, b, 8 ) ;
		break ;
	case 3:
	case 8:
		hit_mark_1_plain_set( pos, dir, no, b, 8 ) ;
		break ;
	case 4:
	case 5:
	case 6:
		if( vs_sys_work.hit_mark_cnt > 3 ) break ;
		hit_mark_spark_set( pos, dir, no, b, 8 ) ;
		break ;
	case 7:
		hit_burst_set( pos, no, 8 ) ;
		break ;
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
		hit_mark_rings_set( pos, dir, no, b, 8 ) ;
		break ;
	}
}

/****************************************************************/
void hitmark_set1( VEC3 *pos, Uint16 dir, int no, Uint8 b )
{
	switch( no ){
	case 0:
		hit_mark_make( pos, dir, no, b, 9 ) ;
		break ;
	case 1:
		hit_mark_1_set( pos, dir, no, b, 9 ) ;
		break ;
	case 2:
		hit_mark_1_set( pos, dir, no, b, 9 ) ;
		break ;
	case 3:
	case 8:
		hit_mark_1_set( pos, dir, no, b, 9 ) ;
		break ;
	case 4:
	case 5:
	case 6:
	case 7:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
		hit_mark_rings_set( pos, dir, no, b, 9 ) ;
		break ;
	}
}

/****************************************************************/
void hit_mark_set( PLAYER_WORK *pl_w, VEC3 *ofs, int no )
{
	VEC3 pos ;

	if( pl_w->disp_dir ) pos.x = pl_w->pos.x - ofs->x ;
	else pos.x = pl_w->pos.x + ofs->x ;
	pos.y = pl_w->pos.y + ofs->y ;
	pos.z = pl_w->pos.z ;
	hitmark_set( &pos, pl_w->disp_dir, no, 0 ) ;
}

/****************************************************************/
void hit_mark_set1( PLAYER_WORK *pl_w, VEC3 *ofs, int no )
{
	VEC3 pos ;

	if( pl_w->disp_dir ) pos.x = pl_w->pos.x - ofs->x ;
	else pos.x = pl_w->pos.x + ofs->x ;
	pos.y = pl_w->pos.y + ofs->y ;
	pos.z = pl_w->pos.z ;
	hitmark_set1( &pos, pl_w->disp_dir, no, 0 ) ;
}

/****************************************************************/
void catch_hit_mark_set( PLAYER_WORK *pl_w, VEC3 *ofs )
{
	PLAYER_WORK *catch_w = pl_w->catch_w ;
	VEC3 pos ;

	if( pl_w->disp_dir ) pos.x = pl_w->pos.x - ofs->x ;
	else pos.x = pl_w->pos.x + ofs->x ;
	pos.y = pl_w->pos.y + ofs->y ;
	pos.z = pl_w->pos.z ;
	hitmark_set( &pos, catch_w->disp_dir, 1, 0 ) ;
}

/* the first mark: four pieces on one work that stays 10 frames */
static void hit_mark_flash_set( PLAYER_WORK *pl_w, Uint8 list );
static void hit_mark_flash2_set( PLAYER_WORK *pl_w, Uint8 list );
static void hit_mark_ring_set( PLAYER_WORK *pl_w, Uint8 list );
static void hit_mark_star_set( PLAYER_WORK *pl_w, Uint8 list );
void hit_mark_burst_set( VEC3 *pos, int no, Uint8 b, Sint8 list );

static void hit_mark_main( PLAYER_WORK *ew );

/*==============================================================*/
static void hit_mark_make( VEC3 *pos, Uint16 dir, int no, Uint8 b, Sint8 list )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, list, LIST_END ) ) ){
		vs_sys_work.hit_mark_cnt ++ ;
		ew->disp_flag = 0 ;
		ew->work_func = hit_mark_main ;
		ew->pos = *pos ;
		ew->obj_flag = OBJ_POS | OBJ_FACE_X | OBJ_FACE_Y ;
		hit_mark_flash_set( ew, list ) ;
		hit_mark_flash2_set( ew, list ) ;
		hit_mark_ring_set( ew, list ) ;
		hit_mark_star_set( ew, list ) ;
		hit_mark_burst_set( pos, no, b, list ) ;
	}
}

/*==============================================================*/
static void hit_mark_main( PLAYER_WORK *ew )
{
	if( ew->timer0 > 10 ){
		work_free( ew ) ;
		vs_sys_work.hit_mark_cnt -- ;
		return ;
	}
	ew->timer0 ++ ;
}

static void hit_mark_flash_main( PLAYER_WORK *ew );

/*==============================================================*/
static void hit_mark_flash_set( PLAYER_WORK *pl_w, Uint8 list )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, list, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = hit_mark_flash_main ;
		ew->obj = obj_efky->obj[0x52] ;
		ew->obj_flag = OBJ_FADE ;
		ew->base_mtx = &pl_w->obj_mtx ;
	}
}

/*==============================================================*/
static void hit_mark_flash_main( PLAYER_WORK *ew )
{
	if( ew->timer0 > 3 ){
		work_free( ew ) ;
		return ;
	}
	ew->fade.r = get_motion( hit_mark_fade0_tbl, &ew->routine_0, ew->timer0 ) ;
	ew->fade.g = ew->fade.r ;
	ew->fade.b = ew->fade.r ;
	ew->timer0 ++ ;
}

static void hit_mark_flash2_main( PLAYER_WORK *ew );

/*==============================================================*/
static void hit_mark_flash2_set( PLAYER_WORK *pl_w, Uint8 list )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, list, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = hit_mark_flash2_main ;
		ew->obj = obj_efky->obj[0x53] ;
		ew->obj_flag = OBJ_FADE ;
		ew->base_mtx = &pl_w->obj_mtx ;
	}
}

/*==============================================================*/
static void hit_mark_flash2_main( PLAYER_WORK *ew )
{
	if( ew->timer0 > 8 ){
		work_free( ew ) ;
		return ;
	}
	ew->fade.r = get_motion( hit_mark_fade1_tbl, &ew->routine_0, ew->timer0 ) ;
	ew->fade.g = ew->fade.r ;
	ew->fade.b = ew->fade.r ;
	ew->timer0 ++ ;
}

static void hit_mark_ring_main( PLAYER_WORK *ew );

/*==============================================================*/
static void hit_mark_ring_set( PLAYER_WORK *pl_w, Uint8 list )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, list, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = hit_mark_ring_main ;
		ew->obj = obj_efky->obj[0x54] ;
		ew->scale = hit_mark_ring_scale ;
		ew->obj_flag = OBJ_ROT_X | OBJ_SCALE | OBJ_FADE ;
		ew->base_mtx = &pl_w->obj_mtx ;
	}
}

/*==============================================================*/
static void hit_mark_ring_main( PLAYER_WORK *ew )
{
	if( ew->timer0 > 10 ){
		work_free( ew ) ;
		return ;
	}
	ew->scale.y = get_motion( hit_mark_scale_y_tbl, &ew->routine_0, ew->timer0 ) ;
	ew->scale.z = get_motion( hit_mark_scale_z_tbl, &ew->routine_1, ew->timer0 ) ;
	ew->rot[0] = ( int )( get_motion( hit_mark_ang_tbl, &ew->routine_2, ew->timer0 ) * 65536.0f / 360.0f + 0.5f ) & 0xffff ;
	ew->fade.r = get_motion( hit_mark_fade2_tbl, &ew->routine_3, ew->timer0 ) ;
	ew->fade.g = ew->fade.r ;
	ew->fade.b = ew->fade.r ;
	ew->timer0 ++ ;
}

static void hit_mark_star_main( PLAYER_WORK *ew );

/*==============================================================*/
static void hit_mark_star_set( PLAYER_WORK *pl_w, Uint8 list )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, list, LIST_END ) ) ){
		ew->disp_flag = 0 ;
		ew->work_func = hit_mark_star_main ;
		ew->rot[2] = ( int )( ( nlRand() % 90 ) * 65536.0f / 360.0f + 0.5f ) & 0xffff ;
		ew->obj_flag = OBJ_ROT_Z | OBJ_FADE ;
		ew->base_mtx = &pl_w->obj_mtx ;
	}
}

/*==============================================================*/
static void hit_mark_star_main( PLAYER_WORK *ew )
{
	switch( ew->routine_0 ){
	case 0:
		if( ++ ew->timer0 > 2 ){
			ew->routine_0 ++ ;
			ew->timer0 = 0 ;
			ew->disp_flag = 1 ;
		}
		break ;
	case 1:
		if( ew->timer0 > 5 ){
			work_free( ew ) ;
			return ;
		}
		ew->obj = obj_efky->obj[hit_mark_star_obj_tbl[ew->timer0]] ;
		ew->fade.r = hit_mark_star_fade_tbl[ew->timer0] ;
		ew->fade.g = ew->fade.r ;
		ew->fade.b = ew->fade.r ;
		ew->timer0 ++ ;
		break ;
	}
}

/* a direction of 0 to 31 as a 16-bit angle */
#define HIT_ANG( dir )	( ( int )( ( dir ) * 737280.0f / 360.0f + 0.5f ) & 0xffff )

/* sparks thrown out of the hit, 10 of them at a speed by the mark's size */
static void hit_spark_set( VEC3 *pos, Float spd, Uint8 dir, Sint8 list );
static void hit_spark_main( PLAYER_WORK *ew );

/****************************************************************/
void hit_mark_burst_set( VEC3 *pos, int no, Uint8 b, Sint8 list )
{
	Float spd ;
	int i ;

	switch( no ){
	case 0:
		spd = 10.0f ;
		break ;
	case 1:
		spd = 20.0f ;
		break ;
	case 2:
		spd = 30.0f ;
		break ;
	case 3:
		spd = 40.0f ;
		break ;
	}
	for( i = 0 ; i < 10 ; i ++ ) hit_spark_set( pos, spd, b, list ) ;
}

/*==============================================================*/
static void hit_spark_set( VEC3 *pos, Float spd, Uint8 dir, Sint8 list )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, list, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = hit_spark_main ;
		ew->obj = obj_efky->obj[96 + nlRand() % 3] ;
		ew->pos = *pos ;
		ew->spd.x = spd * ( nlRandom() + nlSin( HIT_ANG( dir ) ) * 0.5f - 0.5f ) ;
		ew->spd.y = spd * ( nlRandom() + nlCos( HIT_ANG( dir ) ) * 0.5f - 0.5f ) ;
		ew->spd.z = spd * nlRandom() * 0.5f ;
		ew->acc.x = -ew->spd.x / 30.0f ;
		ew->acc.y = -ew->spd.y / 30.0f ;
		ew->acc.z = -ew->spd.z / 30.0f ;
		ew->fade.trnsl = 1.0f ;
		ew->obj_flag = OBJ_POS | OBJ_TRNSL ;
	}
}

/*==============================================================*/
static void hit_spark_main( PLAYER_WORK *ew )
{
	ew->timer0 ++ ;
	if( ew->timer0 > 30 ){
		work_free( ew ) ;
		return ;
	}
	move_pos_spd( ew ) ;
	ew->fade.trnsl -= 0.016666668f ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "hit_mark.c" End of File	*/

/* Twelve flying pictures of the third attract demo (kind 11 works): each
   makes two moves of its own in a few frames, the demo's model 11 and up, and grows from a thin
   shape; once the count reaches 350 it shows, and at 390 it ends. */
#include "player.h"

/* three places for each: the start, the middle and the end of its moves */
static VEC3 at03_ef2_pos_tbl[36] = {
	{ 123.5f, 78.3f, 16.0f },
	{ 4.0f, 7.4f, -141.3f },
	{ 4.0f, 7.4f, -141.3f },
	{ 142.6f, 88.3f, 16.0f },
	{ 23.1f, 7.4f, -141.3f },
	{ 23.1f, 7.4f, -141.3f },
	{ 164.6f, 98.3f, 16.0f },
	{ 45.6f, 7.4f, -141.3f },
	{ 45.6f, 7.4f, -141.3f },
	{ 183.6f, 108.3f, 16.0f },
	{ 64.4f, 7.4f, -141.3f },
	{ 64.4f, 7.4f, -141.3f },
	{ 203.7f, 118.3f, 16.0f },
	{ 85.1f, 7.4f, -141.3f },
	{ 85.1f, 7.4f, -141.3f },
	{ 227.8f, 128.3f, 16.0f },
	{ 109.5f, 7.4f, -141.3f },
	{ 109.5f, 7.4f, -141.3f },
	{ -204.8f, 78.3f, -8.0f },
	{ -102.3f, 50.8f, -141.3f },
	{ -102.3f, 50.8f, -141.3f },
	{ -182.6f, 78.3f, -8.0f },
	{ -79.7f, 50.8f, -141.3f },
	{ -79.7f, 50.8f, -141.3f },
	{ -163.0f, 78.3f, -8.0f },
	{ -59.6f, 50.8f, -141.3f },
	{ -59.6f, 50.8f, -141.3f },
	{ -146.7f, 78.3f, -8.0f },
	{ -43.5f, 50.8f, -141.3f },
	{ -43.5f, 50.8f, -141.3f },
	{ -130.4f, 78.3f, -8.0f },
	{ -26.65f, 50.8f, -141.3f },
	{ -26.65f, 50.8f, -141.3f },
	{ -114.1f, 78.3f, -8.0f },
	{ -9.9f, 50.8f, -141.3f },
	{ -9.9f, 50.8f, -141.3f },
} ;
/* the sizes it has at those places */
static VEC3 at03_ef2_scale_tbl[36] = {
	{ 1.0f, 1.0f, 10.0f },
	{ 1.0f, 1.0f, 10.0f },
	{ 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 10.0f },
	{ 1.0f, 1.0f, 10.0f },
	{ 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 10.0f },
	{ 1.0f, 1.0f, 10.0f },
	{ 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 10.0f },
	{ 1.0f, 1.0f, 10.0f },
	{ 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 10.0f },
	{ 1.0f, 1.0f, 10.0f },
	{ 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 10.0f },
	{ 1.0f, 1.0f, 10.0f },
	{ 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 10.0f },
	{ 1.0f, 1.0f, 10.0f },
	{ 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 10.0f },
	{ 1.0f, 1.0f, 10.0f },
	{ 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 10.0f },
	{ 1.0f, 1.0f, 10.0f },
	{ 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 10.0f },
	{ 1.0f, 1.0f, 10.0f },
	{ 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 10.0f },
	{ 1.0f, 1.0f, 10.0f },
	{ 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 10.0f },
	{ 1.0f, 1.0f, 10.0f },
	{ 1.0f, 1.0f, 1.0f },
} ;
/* the frames of each move */
static const int at03_ef2_time_tbl[8] = { 20, 30, 130, 10, 10, 10, 10, 10 } ;

static void at03_ef2_main( PLAYER_WORK *ew ) ;

/****************************************************************/
void at03_ef2_set( int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SYS, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = at03_ef2_main ;
		ew->obj = obj_dm07->obj[no + 11] ;
		ew->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_FADE | OBJ_BRIGHT ;
		ew->type = no ;
		ew->pos = at03_ef2_pos_tbl[no * 3] ;
		ew->scale = at03_ef2_scale_tbl[no * 3] ;
		ew->fade.r = 0.8f ;
		ew->fade.g = 0.8f ;
		ew->fade.b = 0.8f ;
	}
}

/*==============================================================*/
static void at03_ef2_main( PLAYER_WORK *ew )
{
	int no, i, t, n, m ;
	VEC3 *p0, *p1, *s0, *s1 ;

	switch( ew->routine_0 ){
	case 0:
		no = ew->type ;
		i = ew->timer1 ;
		t = at03_ef2_time_tbl[i] ;
		n = i + no * 3 ;
		m = n + 1 ;
		p0 = &at03_ef2_pos_tbl[n] ;
		p1 = &at03_ef2_pos_tbl[m] ;
		ew->pos.x += ( p1->x - p0->x ) / t ;
		ew->pos.y += ( p1->y - p0->y ) / t ;
		ew->pos.z += ( p1->z - p0->z ) / t ;
		s0 = &at03_ef2_scale_tbl[n] ;
		s1 = &at03_ef2_scale_tbl[m] ;
		ew->scale.x += ( s1->x - s0->x ) / t ;
		ew->scale.y += ( s1->y - s0->y ) / t ;
		ew->scale.z += ( s1->z - s0->z ) / t ;
		if( ++ew->timer0 >= t ){
			ew->timer0 = 0 ;
			ew->pos = at03_ef2_pos_tbl[m] ;
			ew->scale = at03_ef2_scale_tbl[m] ;
			if( ( Uint32 )++ew->timer1 >= 2 ){
				ew->routine_0 ++ ;
				ew->scale.z = 0.0f ;
			}
		}
		break ;
	case 1:
		if( Sys->timer1 >= 350 ){
			ew->routine_0 ++ ;
			ew->fade.r = 1.0f ;
			ew->fade.g = 1.0f ;
			ew->fade.b = 1.0f ;
		}
		break ;
	case 2:
		ew->fade.r -= 0.025f ;
		ew->fade.g -= 0.025f ;
		ew->fade.b -= 0.025f ;
		if( Sys->timer1 >= 390 ) work_free( ew ) ;
		break ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "at03_ef2.c" End of File	*/

/* The start demo's first work (kind 5): it takes a place and a turn from two
   tables of 42 keys, blending the three keys around the count with a quadratic spline, five frames
   to a key, and keeps going while the start demo runs. */
#include "player.h"

/* degrees to an angle */
#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

extern void spline_weight( Float *w0, Float *w1, Float *w2, Float t ) ;

/* the places */
static const VEC3 st_demo_ef0_pos_tbl[42] = {
	{ -5000.0f, 300.0f, -2000.0f },
	{ -5000.0f, 300.0f, -1000.0f },
	{ -5000.0f, 300.0f, 0.0f },
	{ -5000.0f, 300.0f, 1000.0f },
	{ -5000.0f, 300.0f, 2000.0f },
	{ -5224.2f, 300.0f, 2000.0f },
	{ -5541.19f, 300.0f, 2000.0f },
	{ -5899.6f, 300.0f, 1711.8f },
	{ -6335.4f, 300.0f, 1179.0f },
	{ -6521.0f, 300.0f, 1064.6f },
	{ -6675.59f, 300.0f, 832.0f },
	{ -6834.6f, 300.0f, 380.56f },
	{ -6887.4f, 300.0f, -92.0f },
	{ -6849.9f, 300.0f, -341.4f },
	{ -6741.4f, 300.0f, -814.6f },
	{ -6574.9f, 300.0f, -1136.8f },
	{ -6260.59f, 300.0f, -1493.0f },
	{ -5967.8f, 300.0f, -1637.2f },
	{ -5671.8f, 300.0f, -1818.79f },
	{ -5239.6f, 300.0f, -1983.59f },
	{ -5000.0f, 300.0f, -2000.0f },
	{ -4702.8f, 1961.4f, -1000.0f },
	{ -3826.8f, 3435.0f, 0.0f },
	{ -2493.8f, 4573.6f, 1000.0f },
	{ -887.2f, 5105.4f, 2000.0f },
	{ 2577.59f, 4831.0f, 2000.0f },
	{ 4282.99f, 3812.0f, 2000.0f },
	{ 5915.8f, 2396.59f, 1513.2f },
	{ 6086.8f, 2490.39f, 1156.4f },
	{ 6695.0f, 300.0f, 972.8f },
	{ 6545.4f, -2086.0f, 528.0f },
	{ 5241.79f, -4205.2f, 169.8f },
	{ 3406.2f, -5647.39f, -398.8f },
	{ 1177.0f, -6405.4f, -744.6f },
	{ -1134.59f, -6140.8f, -1040.6f },
	{ -3128.39f, -5148.0f, -1351.6f },
	{ -3926.29f, -4411.24f, -1424.91f },
	{ -4556.99f, -3567.2f, -1496.2f },
	{ -5261.19f, -1662.6f, -1794.4f },
	{ -5202.79f, 250.0f, -1900.0f },
	{ -5000.0f, 300.0f, -2000.0f },
	{ -5000.0f, 300.0f, -1000.0f },
} ;
/* the turns, in degrees */
static const VEC3 st_demo_ef0_rot_tbl[42] = {
	{ 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f },
	{ 0.0f, -10.88f, 0.0f },
	{ 0.0f, -20.63f, 0.0f },
	{ 0.0f, -33.2315f, 0.0f },
	{ 0.0f, -47.55f, 0.0f },
	{ 0.0f, -56.72f, 0.0f },
	{ 0.0f, -67.61f, 0.0f },
	{ 0.0f, -79.07f, 0.0f },
	{ 0.0f, -91.01f, 0.0f },
	{ 0.0f, -100.84f, 0.0f },
	{ 0.0f, -112.87f, 0.0f },
	{ 0.0f, -123.76f, 0.0f },
	{ 0.0f, -135.79f, 0.0f },
	{ 0.0f, -146.1f, 0.0f },
	{ 0.0f, -156.992f, 0.0f },
	{ 0.0f, -169.02f, 0.0f },
	{ 0.0f, -180.0f, 0.0f },
	{ 0.0f, -180.0f, -20.0f },
	{ 0.0f, -180.0f, -40.0f },
	{ 0.0f, -180.0f, -60.0f },
	{ 0.0f, -180.0f, -80.0f },
	{ 0.0f, -190.8f, -120.0f },
	{ 0.0f, -201.2f, -140.0f },
	{ 0.0f, -224.69f, -160.0f },
	{ 0.0f, -236.14f, -160.0f },
	{ 0.0f, -245.89f, -180.0f },
	{ 0.0f, -257.92f, -200.0f },
	{ 0.0f, -268.8f, -220.0f },
	{ 0.0f, -281.4f, -240.0f },
	{ 0.0f, -292.29f, -260.0f },
	{ 0.0f, -303.181f, -280.0f },
	{ 0.0f, -314.64f, -300.0f },
	{ 0.0f, -319.75f, -310.0f },
	{ 0.0f, -324.95f, -320.0f },
	{ 0.0f, -336.99f, -340.0f },
	{ 0.0f, -347.87f, -360.0f },
	{ 0.0f, -360.0f, -360.0f },
	{ 0.0f, -360.0f, -360.0f },
} ;

static void st_demo_ef0_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void st_demo_ef0_move( PLAYER_WORK *ew )
{
	const VEC3 *pos0, *pos1, *pos2, *rot0, *rot1, *rot2 ;
	Float w0, w1, w2 ;
	Float t ;
	int i0, i1, i2 ;

	t = ew->timer1 / 5.0f ;
	spline_weight( &w0, &w1, &w2, t ) ;
	i0 = ew->timer0 - 1 ;
	i1 = ew->timer0 ;
	pos0 = &st_demo_ef0_pos_tbl[i0] ;
	rot0 = &st_demo_ef0_rot_tbl[i0] ;
	pos1 = &st_demo_ef0_pos_tbl[i1] ;
	rot1 = &st_demo_ef0_rot_tbl[i1] ;
	i2 = i1 + 1 ;
	pos2 = &st_demo_ef0_pos_tbl[i2] ;
	rot2 = &st_demo_ef0_rot_tbl[i2] ;
	ew->rot[0] = DEG_ANG( rot0->x * w0 + rot1->x * w1 + rot2->x * w2 ) ;
	ew->rot[1] = DEG_ANG( rot0->y * w0 + rot1->y * w1 + rot2->y * w2 ) ;
	ew->rot[2] = DEG_ANG( rot0->z * w0 + rot1->z * w1 + rot2->z * w2 ) ;
	ew->acc.x = rot0->x * w0 + rot1->x * w1 + rot2->x * w2 ;
	ew->acc.y = rot0->y * w0 + rot1->y * w1 + rot2->y * w2 ;
	ew->acc.z = rot0->z * w0 + rot1->z * w1 + rot2->z * w2 ;
	ew->pos.x = pos0->x * w0 + pos1->x * w1 + pos2->x * w2 ;
	ew->pos.y = pos0->y * w0 + pos1->y * w1 + pos2->y * w2 ;
	ew->pos.z = pos0->z * w0 + pos1->z * w1 + pos2->z * w2 ;
	ew->timer1 ++ ;
	ew->timer1 %= 5 ;
	if( !ew->timer1 ){
		if( ++ ew->timer0 >= 41 ){
			ew->timer0 = 1 ;
			ew->timer1 = 0 ;
		}
	}
}

/****************************************************************/
void st_demo_ef0_set( int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->type = no ;
		ew->work_func = st_demo_ef0_main ;
		ew->obj = obj_dm02->obj[0] ;
		ew->pos = st_demo_ef0_pos_tbl[0] ;
		ew->rot[0] = DEG_ANG( st_demo_ef0_rot_tbl[0].x ) ;
		ew->rot[1] = DEG_ANG( st_demo_ef0_rot_tbl[0].y ) ;
		ew->rot[2] = DEG_ANG( st_demo_ef0_rot_tbl[0].z ) ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_ROT_Z | OBJ_BRIGHT ;
		ew->timer0 = ( Sys->vblank_cnt & 31 ) + 1 ;
		ew->timer1 = 0 ;
	}
}

/*==============================================================*/
static void st_demo_ef0_main( PLAYER_WORK *ew )
{
	st_demo_ef0_move( ew ) ;
	if( Sys->routine_0 != 2 ) work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "st_demo_ef0.c" End of File	*/

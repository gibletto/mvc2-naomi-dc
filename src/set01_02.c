/* Stage 01's third object (kind 5 works): the stage's camera-like model that
   follows a table of key poses by a quadratic spline, and the wave that runs over one model's vertices. */
#include "player.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

extern void obj_uv_begin( int *src, void *obj ) ;
extern int obj_uv_next( void ) ;
extern void obj_uv_pos_get( VEC3 *v ) ;
extern void obj_uv_pos_put( VEC3 *v ) ;

/* a key: its frame, its place and its turns about x, y and z in degrees */
typedef struct SET01_02_KEY {
	Float	frame ;
	VEC3	pos ;
	VEC3	rot ;
} SET01_02_KEY ;

static SET01_02_KEY set01_02_key_tbl[34] = {
	{ 0.0f, { -6000.0f, 1000.0f, -4500.0f }, { 10.0f, 45.0f, 0.0f } },
	{ 150.0f, { -2840.0f, 900.0f, -3460.0f }, { 25.0f, 55.0f, 28.0f } },
	{ 300.0f, { 310.0f, 900.0f, -2950.0f }, { 10.0f, 75.0f, 0.0f } },
	{ 450.0f, { 3470.0f, 900.0f, -2620.0f }, { 90.0f, 99.0f, 75.0f } },
	{ 600.0f, { 6220.0f, 910.0f, -3320.0f }, { 0.0f, 90.0f, 0.0f } },
	{ 700.0f, { 5300.0f, 1300.0f, -6500.0f }, { 27.0f, -45.0f, -20.0f } },
	{ 800.0f, { 1040.0f, 1400.0f, -5000.0f }, { 1.5f, -70.0f, 7.9f } },
	{ 900.0f, { -950.0f, 1440.0f, -4990.0f }, { 2.0f, -110.0f, 0.0f } },
	{ 1000.0f, { -2020.0f, 1480.0f, -7050.0f }, { 5.0f, -170.0f, -17.0f } },
	{ 1100.0f, { -1030.0f, 1500.0f, -8420.0f }, { -10.0f, -230.0f, -20.0f } },
	{ 1200.0f, { 940.0f, 1500.0f, -8740.0f }, { 50.0f, -285.0f, 45.0f } },
	{ 1300.0f, { 2470.0f, 1500.0f, -6990.0f }, { 0.19f, -380.0f, 2.8f } },
	{ 1400.0f, { 1640.0f, 1500.0f, -5130.0f }, { -14.0f, -420.0f, 20.0f } },
	{ 1500.0f, { -950.0f, 1440.0f, -4990.0f }, { 2.0f, -470.0f, 0.0f } },
	{ 1600.0f, { -2020.0f, 1440.0f, -7050.0f }, { 5.0f, -530.0f, -17.0f } },
	{ 1700.0f, { -1030.0f, 1440.0f, -8420.0f }, { -10.0f, -590.0f, -20.0f } },
	{ 1800.0f, { 940.0f, 1440.0f, -8740.0f }, { 50.0f, -645.0f, 45.0f } },
	{ 1900.0f, { 2470.0f, 1440.0f, -6990.0f }, { 0.19f, -740.0f, 2.8f } },
	{ 2000.0f, { 1640.0f, 1440.0f, -5130.0f }, { -14.0f, -780.0f, 20.0f } },
	{ 2100.0f, { -950.0f, 1400.0f, -4990.0f }, { 2.0f, -830.0f, 0.0f } },
	{ 2200.0f, { -2020.0f, 1400.0f, -7050.0f }, { 5.0f, -890.0f, -17.0f } },
	{ 2300.0f, { -1030.0f, 1400.0f, -8420.0f }, { -10.0f, -950.0f, -20.0f } },
	{ 2400.0f, { 940.0f, 1400.0f, -8740.0f }, { 50.0f, -1005.0f, 45.0f } },
	{ 2500.0f, { 2470.0f, 1400.0f, -6990.0f }, { 0.19f, -1100.0f, 2.8f } },
	{ 2600.0f, { 1640.0f, 1400.0f, -5130.0f }, { -14.0f, -1120.0f, 20.0f } },
	{ 2700.0f, { -550.0f, 1400.0f, -4640.0f }, { 102.0f, -1177.0f, -99.0f } },
	{ 2800.0f, { -2460.0f, 1400.0f, -5570.0f }, { 45.0f, -1195.0f, -42.0f } },
	{ 2900.0f, { -5870.0f, 1350.0f, -8290.0f }, { 0.0f, -1220.0f, 0.0f } },
	{ 3000.0f, { -5870.0f, 1200.0f, -9370.0f }, { 22.0f, 20.0f, 0.0f } },
	{ 3080.0f, { -3600.0f, 1000.0f, -6470.0f }, { 4.0f, 20.0f, 0.0f } },
	{ 3160.0f, { -2400.0f, 1200.0f, -4280.0f }, { 0.0f, 20.0f, 0.0f } },
	{ 3240.0f, { -1250.0f, 1400.0f, -2060.0f }, { -7.0f, 20.0f, 0.0f } },
	{ 3320.0f, { 0.0f, 1500.0f, 200.0f }, { -22.0f, 20.0f, 0.0f } },
	{ 3400.0f, { 1250.0f, 1600.0f, 2200.0f }, { -30.0f, 20.0f, 0.0f } },
} ;

/****************************************************************/
/*			The three weights of the quadratic spline at t		*/
/*			(set00_00 and st_demo_ef0 call it too)				*/
/****************************************************************/
void spline_weight( Float *a, Float *b, Float *c, Float t )
{
	*a = 0.5f * ( ( 1.0f - t ) * ( 1.0f - t ) ) ;
	*b = t * ( 1.0f - t ) + 0.5f ;
	*c = 0.5f * ( t * t ) ;
}

static void set01_02_wave( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set01_02_main( PLAYER_WORK *ew )
{
	Float wa, wb, wc ;
	Float t ;
	int i ;

	t = ew->timer0 / ( set01_02_key_tbl[ew->timer1 + 1].frame - set01_02_key_tbl[ew->timer1].frame ) ;
	spline_weight( &wa, &wb, &wc, t ) ;
	i = ew->timer1 ;
	ew->pos.x = set01_02_key_tbl[i - 1].pos.x * wa + set01_02_key_tbl[i].pos.x * wb + set01_02_key_tbl[i + 1].pos.x * wc ;
	ew->pos.y = set01_02_key_tbl[i - 1].pos.y * wa + set01_02_key_tbl[i].pos.y * wb + set01_02_key_tbl[i + 1].pos.y * wc ;
	ew->pos.z = set01_02_key_tbl[i - 1].pos.z * wa + set01_02_key_tbl[i].pos.z * wb + set01_02_key_tbl[i + 1].pos.z * wc ;
	ew->rot[0] = DEG_ANG( set01_02_key_tbl[i - 1].rot.x * wa + set01_02_key_tbl[i].rot.x * wb + set01_02_key_tbl[i + 1].rot.x * wc ) ;
	ew->rot[1] = DEG_ANG( set01_02_key_tbl[i - 1].rot.y * wa + set01_02_key_tbl[i].rot.y * wb + set01_02_key_tbl[i + 1].rot.y * wc ) ;
	ew->rot[2] = DEG_ANG( set01_02_key_tbl[i - 1].rot.z * wa + set01_02_key_tbl[i].rot.z * wb + set01_02_key_tbl[i + 1].rot.z * wc ) ;
	ew->timer0 ++ ;
	if( ew->timer0 >= set01_02_key_tbl[ew->timer1 + 1].frame - set01_02_key_tbl[ew->timer1].frame ){
		ew->timer0 = 0 ;
		ew->timer1 ++ ;
		if( ew->timer1 + 1 >= 34U ) ew->timer1 = 1 ;
	}
	set01_02_wave( ew ) ;
}

/*==============================================================*/
/*			The far sides of the model ripple					*/
/*==============================================================*/
static void set01_02_wave( PLAYER_WORK *ew )
{
	VEC3 v, n ;
	Float len ;

	obj_uv_begin( obj_stg->obj[15], ew->obj ) ;
	while( obj_uv_next() == 0 ){
		obj_uv_pos_get( &v ) ;
		n = v ;
		len = nlUnitVector( &n ) ;
		if( ( v.x < -350.0f || v.x > 350.0f ) && v.z < -100.0f ){
			v.y += 0.02f * ( len * nlSin( DEG_ANG( ew->timer0 * 30 + ( int )len ) ) ) ;
		}
		obj_uv_pos_put( &v ) ;
	}
}

/****************************************************************/
void set01_02_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set01_02_main ;
		ew->obj = obj_stg->obj[14] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_ROT_Z | OBJ_BRIGHT ;
		ew->timer1 = 1 ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set01_02.c" End of File	*/

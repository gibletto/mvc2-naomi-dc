/* Stage 03's object 2: a model (object 7 of the stage's file) whose texture points above a height
   are pushed out in waves, travelling round with the frame count. */
#include "player.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

extern void obj_uv_begin( int *src, void *obj ) ;
extern int obj_uv_next( void ) ;
extern void obj_uv_pos_get( VEC3 *v ) ;
extern void obj_uv_pos_put( VEC3 *v ) ;

static VEC3 set03_02_pos = { -1124.31f, 352.0f, -387.96f } ;
static VEC3 set03_02_rot = { 0.0f, 45.0f, 180.0f } ;

/*==============================================================*/
static void set03_02_main( PLAYER_WORK *ew )
{
	VEC3 v ;
	int phase ;

	ew->timer0 += 4 ;
	if( ew->timer0 >= 360 ) ew->timer0 = 0 ;
	obj_uv_begin( obj_stg->obj[8], ew->obj ) ;
	phase = 0 ;
	while( obj_uv_next() == 0 ){
		obj_uv_pos_get( &v ) ;
		if( v.y > 10.0f ) v.z += v.y * v.y * nlSin( DEG_ANG( ew->timer0 + phase ) ) * 0.0002f ;
		phase += 30 ;
		obj_uv_pos_put( &v ) ;
	}
}

/****************************************************************/
void set03_02_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set03_02_main ;
		ew->obj = obj_stg->obj[7] ;
		ew->pos = set03_02_pos ;
		ew->rot[0] = DEG_ANG( set03_02_rot.x ) ;
		ew->rot[1] = DEG_ANG( set03_02_rot.y ) ;
		ew->rot[2] = DEG_ANG( set03_02_rot.z ) ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_ROT_Z | OBJ_BRIGHT ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set03_02.c" End of File	*/

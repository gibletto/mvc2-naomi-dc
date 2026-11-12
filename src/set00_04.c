/* Stage 00's fifth object (kind 5 works): a model whose vertices that lie
   in front sway sideways in a wave that runs through them. */
#include "player.h"

/* degrees to an angle */
#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

extern int obj_uv_begin( int *src, void *obj ) ;
extern int obj_uv_next( void ) ;
extern int obj_uv_pos_get( VEC3 *v ) ;
extern int obj_uv_pos_put( VEC3 *v ) ;

static VEC3 set00_04_pos[] = {
	{ -300.0f, 1450.0f, -4110.0f },
#ifdef TARGET_DC	/* the stage's other look */
	{ -690.0f, 1500.0f, -4180.0f },
#endif
} ;

static void set00_04_main( PLAYER_WORK *ew ) ;

/****************************************************************/
void set00_04_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set00_04_main ;
		ew->obj = obj_stg->obj[11] ;
#ifdef TARGET_DC
		if( stage_no == 0 ) ew->pos = set00_04_pos[0] ;
		else ew->pos = set00_04_pos[1] ;
#else
		ew->pos = set00_04_pos[0] ;
#endif
		ew->obj_flag = OBJ_POS | OBJ_BRIGHT ;
	}
}

/*==============================================================*/
static void set00_04_main( PLAYER_WORK *ew )
{
	VEC3 v ;
	Sint32 n ;

	ew->timer0 += 4 ;
	if( ew->timer0 >= 360 ) ew->timer0 = 0 ;
	obj_uv_begin( obj_stg->obj[12], ew->obj ) ;
	n = 0 ;
	while( obj_uv_next() == 0 ){
		obj_uv_pos_get( &v ) ;
		if( v.z < -10.0f ) v.x += v.z * v.z * nlSin( DEG_ANG( ew->timer0 + n ) ) * 0.0002f ;
		n += 30 ;
		obj_uv_pos_put( &v ) ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set00_04.c" End of File	*/

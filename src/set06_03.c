/* Stage 06's object 3, which the stage's table has no entry for (object 4's setter makes it, with
   itself as the master): a model whose texture ripples in time while object 4 runs. */
#include "player.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

/* Kamui */
extern void obj_uv_begin( int *src, void *obj ) ;
extern int obj_uv_next( void ) ;
extern void obj_uv_pos_get( VEC3 *pos ) ;
extern void obj_uv_pos_put( VEC3 *pos ) ;

static VEC3 set06_03_pos = { 1313.0f, 1197.0f, -730.0f } ;

static void set06_03_main( PLAYER_WORK *ew ) ;

/****************************************************************/
void set06_03_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set06_03_main ;
		ew->obj = obj_stg->obj[21] ;
		ew->pos = set06_03_pos ;
		ew->rot[1] = 0xe001 ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_BRIGHT ;
		ew->master_w = pl_w ;
	}
}

/*==============================================================*/
/*			Once object 4 has begun, the model is hidden;		*/
/*			before, the texture points below a line ripple		*/
/*==============================================================*/
static void set06_03_main( PLAYER_WORK *ew )
{
	VEC3 uv ;
	int i ;

	if( ew->master_w->routine_0 > 0 ){
		ew->disp_flag = 0 ;
		return ;
	}
	ew->disp_flag = 1 ;
	ew->timer0 += 4 ;
	if( ew->timer0 >= 360 ) ew->timer0 = 0 ;
	obj_uv_begin( obj_stg->obj[22], ew->obj ) ;
	i = 0 ;
	while( obj_uv_next() == 0 ){
		obj_uv_pos_get( &uv ) ;
		if( uv.y < -10.0f ){
			uv.z += 0.0002f * ( fabs( uv.y ) * fabs( uv.y ) * nlSin( DEG_ANG( ew->timer0 + i ) ) ) ;
		}
		i += 30 ;
		obj_uv_pos_put( &uv ) ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set06_03.c" End of File	*/

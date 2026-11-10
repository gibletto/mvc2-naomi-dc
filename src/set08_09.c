/* Stage 08's object 9, which set08_0a.c makes: a puff that swells on x and z and
   fades out. */
#include "player.h"

static const VEC3 set08_09_scale = { 10.0f, 10.0f, 10.0f } ;

static void set08_09_main( PLAYER_WORK *ew ) ;

/****************************************************************/
void set08_09_make( VEC3 *pos )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set08_09_main ;
		ew->obj = obj_stg->obj[5] ;
		ew->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_FADE | OBJ_BRIGHT ;
		ew->pos = *pos ;
		ew->scale = set08_09_scale ;
		ew->fade.r = ew->fade.g = ew->fade.b = 1.0f ;
	}
}

/*==============================================================*/
static void set08_09_main( PLAYER_WORK *ew )
{
	ew->scale.x += 1.0f ;
	ew->scale.z += 1.0f ;
	ew->fade.r -= 0.05f ;
	ew->fade.g = ew->fade.b = ew->fade.r ;
	if( ew->fade.r <= 0.0f ) work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set08_09.c" End of File	*/

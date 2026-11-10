/* A still model in the third attract demo (kind 11 works): the demo's
   model 0 at a fixed place, which nothing steps. */
#include "player.h"

const VEC3 unk_0C230B0C[2] = { { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f } } ;	/* never used */
static const VEC3 at03_ef8_pos = { 0.0f, -0.6f, -154.0f } ;	/* where it stands */
const VEC3 at03_ef8_rot = { 0.0f, 0.0f, 0.0f } ;		/* never used */
const VEC3 at03_ef8_scale = { 1.0f, 1.0f, 1.0f } ;	/* never used */

static void at03_ef8_main( PLAYER_WORK *ew ) ;

/****************************************************************/
void at03_ef8_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SYS, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = at03_ef8_main ;
		ew->obj = obj_dm07->obj[0] ;
		ew->obj_flag = OBJ_POS | OBJ_BRIGHT ;
		ew->pos = at03_ef8_pos ;
	}
}

/*==============================================================*/
static void at03_ef8_main( PLAYER_WORK *ew )
{
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "at03_ef8.c" End of File	*/

/* A model that fades in during the third attract demo (kind 11 works):
   sys_mode.c makes five of them, from objects of the demo's file. */
#include "player.h"

static VEC3 at03_ef0_rot = { 0.0f, 0.0f, 0.0f } ;	/* never used */
static VEC3 at03_ef0_scale = { 1.0f, 1.0f, 1.0f } ;	/* never used */

static const Sint32 at03_ef0_obj_tbl[8] = { 1, 2, 3, 30, 31, 0, 0, 0 } ;	/* the object of each */
const VEC3 unk_0C2309E4 = { 1.0f, 1.0f, 1.0f } ;	/* never used */

static void at03_ef0_main( PLAYER_WORK *ew ) ;

/****************************************************************/
void at03_ef0_set( int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SYS, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = at03_ef0_main ;
		ew->obj = obj_dm07->obj[at03_ef0_obj_tbl[no]] ;
		ew->type = no ;
		ew->obj_flag = OBJ_TRNSL | OBJ_BRIGHT ;
		ew->fade.trnsl = 0.0f ;
	}
}

/*==============================================================*/
static void at03_ef0_main( PLAYER_WORK *ew )
{
	switch( ew->routine_0 ){
	case 0:
		ew->fade.trnsl += 0.2f ;
		if( ew->fade.trnsl >= 1.0f ){
			ew->routine_0 ++ ;
			ew->fade.trnsl = 1.0f ;
		}
		break ;
	case 1:
		break ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "at03_ef0.c" End of File	*/

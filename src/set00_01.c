/* Stage 00's second object (kind 5 works): four models that turn about the
   vertical axis, in two kinds, each at its own place. */
#include "player.h"

static VEC3 set00_01_pos_tbl[] = {
	{ -780.5f, 980.0f, -2200.0f }, { 1690.0f, 980.0f, -1580.5f },
	{ 580.0f, 1800.0f, -7620.0f }, { 3020.0f, 1800.0f, -7000.0f },
#ifdef TARGET_DC	/* the stage's other look */
	{ -1007.0f, 1020.0f, -2235.0f }, { 1596.0f, 1020.0f, -1820.0f },
	{ -88.0f, 1800.0f, -7742.0f }, { 2400.0f, 1800.0f, -7335.0f },
#endif
} ;

static void set00_01_obj_set( Sint32 no ) ;

/****************************************************************/
void set00_01_set( void )
{
	Sint32 i ;

	for( i = 0 ; i < 4 ; i ++ ) set00_01_obj_set( i ) ;
}

static void set00_01_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set00_01_obj_set( Sint32 no )
{
	PLAYER_WORK *ew ;
#ifdef TARGET_DC
	VEC3 *pos ;
#endif

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set00_01_main ;
		if( no < 2 ) ew->obj = obj_stg->obj[2] ;
		else ew->obj = obj_stg->obj[13] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_BRIGHT ;
#ifdef TARGET_DC
		pos = &set00_01_pos_tbl[no] ;
		if( stage_no == 0 ) ew->pos = pos[0] ;
		else ew->pos = pos[4] ;
#else
		ew->pos = set00_01_pos_tbl[no] ;
#endif
	}
}

/*==============================================================*/
static void set00_01_main( PLAYER_WORK *ew )
{
#ifdef TARGET_DC
	if( stage_no == 0 ) ew->rot[1] += 0x200 ;
	else ew->rot[1] += 0x100 ;
#else
	ew->rot[1] += 0x200 ;
#endif
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set00_01.c" End of File	*/

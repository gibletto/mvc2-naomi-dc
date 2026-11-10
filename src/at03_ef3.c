/* The sparkles of the third attract demo (kind 11 works): sys_mode.c makes
   six at the counts it picks. Each is the demo's model 26 at a place and size of its own, which
   grows and flashes with a sine for 60 frames. */
#include "player.h"

static VEC3 at03_ef3_rot = { 0.0f, 0.0f, 0.0f } ;	/* never used */
static VEC3 at03_ef3_scale = { 1.0f, 1.0f, 1.0f } ;	/* never used */

/* where each stands and its first size */
static VEC3 at03_ef3_pos_tbl[6] = {
	{ -65.0f, -45.0f, -149.5f },
	{ 52.0f, -40.0f, -149.5f },
	{ 70.0f, 50.0f, -149.5f },
	{ -58.0f, -5.0f, -149.5f },
	{ 70.0f, 28.0f, -149.5f },
	{ 67.0f, -32.0f, -149.5f },
} ;
static Float at03_ef3_scale_tbl[6] = { 0.7f, 0.5f, 0.85f, 1.0f, 0.5f, 0.6f } ;

static void at03_ef3_main( PLAYER_WORK *ew ) ;

/****************************************************************/
void at03_ef3_set( int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SYS, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = at03_ef3_main ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Z | OBJ_SCALE | OBJ_FADE | OBJ_BRIGHT ;
		ew->type = no ;
		ew->pos = at03_ef3_pos_tbl[no] ;
		ew->scale.y = ew->scale.x = at03_ef3_scale_tbl[no] ;
	}
}

/*==============================================================*/
static void at03_ef3_main( PLAYER_WORK *ew )
{
	ew->obj = obj_dm07->obj[26] ;
	ew->scale.x += 0.04f ;
	ew->scale.y += 0.04f ;
	ew->fade.r = nlSin( ( int )( ew->timer0 * 3 * 65536.0f / 360.0f + 0.5f ) & 0xffff ) ;
	ew->fade.g = ew->fade.r ;
	ew->fade.b = ew->fade.r ;
	if( ++ ew->timer0 >= 60 ) work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "at03_ef3.c" End of File	*/

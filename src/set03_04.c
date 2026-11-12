/* Stage 03's object 4: four models (objects 22 to 25 of the stage's file) that sway about two
   axes and change colour, each a quarter of a turn on from the one before. */
#include "player.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

static VEC3 set03_04_pos_tbl[4] = {
	{ 218.0f, 0.0f, -3500.0f },
	{ -1915.0f, 0.0f, -2906.0f },
	{ 1890.0f, 0.0f, -3400.0f },
	{ -400.0f, 0.0f, -3340.0f },
} ;
static VEC3 set03_04_rot_tbl[4] = {
	{ 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f },
} ;

/*==============================================================*/
static void set03_04_main( PLAYER_WORK *ew )
{
	if( ew->timer0 >= 720 ) ew->timer0 = 0 ;
	if( ew->timer1 >= 1080 ) ew->timer1 = 0 ;
	ew->rot[0] = ( Sint32 )( nlSin( DEG_ANG( ew->timer0 / 2.0f + ew->type * 90 ) ) * 1310720.0f / 360.0f + 0.5f ) & 0xffff ;
	ew->rot[2] = ( Sint32 )( nlCos( DEG_ANG( ew->timer0 / 2.0f + ew->type * 90 ) ) * 3932160.0f / 360.0f + 0.5f ) & 0xffff ;
#ifdef TARGET_DC	/* the colour changes less in the stage's other look */
	if( stage_no == 3 ){
		ew->fade.r = 0.5f + 0.5f * nlSin( DEG_ANG( ew->timer1 / 3.0f ) ) ;
		ew->fade.g = 0.5f + 0.5f * nlSin( DEG_ANG( ew->timer1 / 2.0f ) ) ;
		ew->fade.b = 0.5f + 0.5f * nlSin( DEG_ANG( ew->timer1 / 1.0f ) ) ;
	} else {
		ew->fade.r = 0.5f + 0.1f * nlSin( DEG_ANG( ew->timer1 / 3.0f ) ) ;
		ew->fade.g = 0.5f + 0.1f * nlSin( DEG_ANG( ew->timer1 / 2.0f ) ) ;
		ew->fade.b = 0.5f + 0.1f * nlSin( DEG_ANG( ew->timer1 / 1.0f ) ) ;
	}
#else
	ew->fade.r = 0.5f + 0.5f * nlSin( DEG_ANG( ew->timer1 / 3.0f ) ) ;
	ew->fade.g = 0.5f + 0.5f * nlSin( DEG_ANG( ew->timer1 / 2.0f ) ) ;
	ew->fade.b = 0.5f + 0.5f * nlSin( DEG_ANG( ew->timer1 / 1.0f ) ) ;
#endif
	ew->timer0 ++ ;
	ew->timer1 ++ ;
}

/*==============================================================*/
static void set03_04_make( int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set03_04_main ;
		ew->obj = obj_stg->obj[22 + no] ;
		ew->pos = set03_04_pos_tbl[no] ;
		ew->rot[1] = DEG_ANG( set03_04_rot_tbl[no].y ) ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Z | OBJ_FADE | OBJ_BRIGHT ;
		ew->type = no ;
		ew->timer1 = no * 270 ;
	}
}

/****************************************************************/
void set03_04_set( void )
{
	int i ;

	for( i = 0 ; i < 4 ; i ++ ) set03_04_make( i ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set03_04.c" End of File	*/

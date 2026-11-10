/* Stage 03's object 5: a model (objects 26 and 29 to 31 of the stage's file) with
   two parts riding on it. It turns and pulses in colour, its parts going through states that tip
   them. */
#include "player.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

static VEC3 set03_05_pos = { -1266.31f, 310.0f, -409.396f } ;
static VEC3 set03_05_rot = { 90.0f, 40.0f, 0.0f } ;
static VEC3 set03_05_part_pos_tbl[2] = {
	{ -1321.31f, 180.0f, -338.396f },
	{ -1205.31f, 180.0f, -436.396f },
} ;
static VEC3 set03_05_rot_tbl[4] = {
	{ 0.0f, 40.0f, 0.0f },
	{ 0.0f, 40.0f, 180.0f },
	{ 0.0f, -140.0f, 0.0f },
	{ 0.0f, -65.0f, 180.0f },
} ;
static Float set03_05_ang_a_tbl[2] = { -9.0f, -5.25f } ;
static Float set03_05_ang_b_tbl[2] = { 1.8f, 1.05f } ;

static void set03_05_make( ) ;

/****************************************************************/
void set03_05_set( void )
{
	set03_05_make() ;
}

static void set03_05_part_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
/*			One of the two parts of object 5 on the model		*/
/*==============================================================*/
static void set03_05_part_set( int no, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set03_05_part_main ;
		ew->obj = obj_stg->obj[no * 2 + 4] ;
		ew->pos = set03_05_part_pos_tbl[no] ;
		ew->rot[1] = DEG_ANG( set03_05_rot_tbl[no].y ) ;
		ew->rot[2] = DEG_ANG( set03_05_rot_tbl[no].z ) ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_ROT_Z | OBJ_BRIGHT ;
		ew->type = no ;
		ew->ef_w = pl_w ;
	}
}

/*==============================================================*/
static void set03_05_part_main( PLAYER_WORK *ew )
{
	switch( ew->routine_0 ){
	case 0 :
		if( ew->ef_w->timer1 ) break ;
		if( ew->ef_w->routine_0 == 2 || ew->ef_w->routine_0 == ew->type ){
			ew->routine_0 ++ ;
			ew->timer0 = 0 ;
		}
		break ;
	case 1 :
		ew->rot[1] += DEG_ANG( set03_05_ang_a_tbl[ew->type] ) ;
		ew->timer0 ++ ;
		if( ew->timer0 >= 20 ){
			ew->routine_0 ++ ;
			ew->timer0 = 0 ;
			ew->rot[1] = DEG_ANG( set03_05_rot_tbl[ew->type + 2].y ) ;
		}
		break ;
	case 2 :
		ew->rot[1] = DEG_ANG( set03_05_rot_tbl[ew->type + 2].y + ( 60 - ew->timer0 ) / 60.0f * nlSin( DEG_ANG( ew->timer0 * 36 ) ) * 10.0f ) ;
		ew->timer0 ++ ;
		if( ew->timer0 >= 60 ){
			ew->routine_0 ++ ;
			ew->timer0 = 0 ;
			ew->rot[1] = DEG_ANG( set03_05_rot_tbl[ew->type + 2].y ) ;
		}
		break ;
	case 3 :
		ew->timer0 ++ ;
		if( ew->timer0 >= 60 ){
			ew->routine_0 ++ ;
			ew->timer0 = 0 ;
		}
		break ;
	case 4 :
		ew->rot[1] += DEG_ANG( set03_05_ang_b_tbl[ew->type] ) ;
		ew->timer0 ++ ;
		if( ew->timer0 >= 100 ){
			ew->timer0 = 0 ;
			ew->routine_0 = 0 ;
			ew->rot[1] = DEG_ANG( set03_05_rot_tbl[ew->type].y ) ;
		}
		break ;
	}
}

static void set03_05_move( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set03_05_make( )
{
	PLAYER_WORK *ew ;
	int i ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set03_05_move ;
		ew->obj = obj_stg->obj[26] ;
		ew->pos = set03_05_pos ;
		ew->rot[0] = DEG_ANG( set03_05_rot.x ) ;
		ew->rot[1] = DEG_ANG( set03_05_rot.y ) ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_FADE | OBJ_BRIGHT ;
		for( i = 0 ; i < 2 ; i ++ ) set03_05_part_set( i, ew ) ;
	}
}

/*==============================================================*/
/*			The colour pulses, and every ten seconds or so		*/
/*			the model turns to one of three ways				*/
/*==============================================================*/
static void set03_05_move( PLAYER_WORK *ew )
{
	Float ang ;

	ew->timer0 ++ ;
	if( ew->timer0 >= 1080 ) ew->timer0 = 0 ;
	ew->fade.r = 0.5f + 0.5f * nlSin( DEG_ANG( ew->timer0 / 3.0f ) ) ;
	ew->fade.g = 0.5f + 0.5f * nlSin( DEG_ANG( ew->timer0 / 2.0f ) ) ;
	ew->fade.b = 0.5f + 0.5f * nlSin( DEG_ANG( ew->timer0 / 1.0f ) ) ;
	ew->timer1 ++ ;
	if( ew->timer1 >= 600 ){
		ew->timer1 = 0 ;
		ew->routine_0 = nlRand() % 3 ;
		ang = set03_05_rot.y ;
		if( !ew->routine_0 ) ang += 180.0f ;
		ew->rot[1] = DEG_ANG( ang ) ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set03_05.c" End of File	*/

/* Stage 03's object 6: a model (objects 28 and 29 of the stage's file) that circles
   about a point, with two parts riding on it that spin. */
#include "player.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

static VEC3 set03_06_pos = { 2400.0f, 230.0f, -1000.0f } ;
static VEC3 set03_06_part_pos_tbl[2] = {
	{ 0.0f, -50.0f, 80.0f },
	{ 0.0f, -50.0f, -80.0f },
} ;

/*==============================================================*/
static void set03_06_spin( PLAYER_WORK *ew )
{
	ew->rot[0] += 0x200 ;
}

/*==============================================================*/
static void set03_06_part_set( PLAYER_WORK *pl_w, int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set03_06_spin ;
		ew->obj = obj_stg->obj[29] ;
		ew->pos = set03_06_part_pos_tbl[no] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_BRIGHT ;
		ew->ef_w = pl_w ;
		ew->base_mtx = &pl_w->obj_mtx ;
	}
}

/*==============================================================*/
/*			The model circles about a point, turning to face	*/
/*			along the way										*/
/*==============================================================*/
static void set03_06_move( PLAYER_WORK *ew )
{
	if( ew->timer0 >= 300 ) ew->timer0 = 180 ;
	ew->rot[1] = DEG_ANG( ew->timer0 + 90 ) ;
	ew->pos.x = set03_06_pos.x + 850.0f * nlSin( DEG_ANG( ew->timer0 ) ) ;
	ew->pos.z = set03_06_pos.z + 850.0f * nlCos( DEG_ANG( ew->timer0 ) ) ;
	ew->timer0 ++ ;
}

/****************************************************************/
void set03_06_set( void )
{
	PLAYER_WORK *ew ;
	int i ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set03_06_move ;
		ew->obj = obj_stg->obj[28] ;
		ew->pos = set03_06_pos ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_BRIGHT ;
		for( i = 0 ; i < 2 ; i ++ ) set03_06_part_set( ew, i ) ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set03_06.c" End of File	*/

/* Stage 03's object 3: a ring of twelve models (object 10 of the stage's file) round a model
   (object 9), each lit in a colour of its own, that turn about their own axes. */
#include "player.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

static VEC3 set03_03_pos = { 3066.0f, 2727.0f, -6526.0f } ;
static VEC3 set03_03_col_tbl[12] = {
	{ 1.0f, 0.5f, 0.75f },
	{ 1.0f, 0.9f, 0.6f },
	{ 0.8f, 0.6f, 1.0f },
	{ 1.0f, 0.6f, 0.6f },
	{ 1.0f, 0.85f, 0.6f },
	{ 1.0f, 0.6f, 1.0f },
	{ 1.0f, 0.5f, 0.85f },
	{ 1.0f, 0.6f, 0.6f },
	{ 1.0f, 1.0f, 0.6f },
	{ 1.0f, 0.9f, 0.6f },
	{ 1.0f, 0.85f, 0.6f },
	{ 1.0f, 0.6f, 0.6f },
} ;

/*==============================================================*/
static void set03_03_ring_move( PLAYER_WORK *ew )
{
	ew->rot[0] -= 16 ;
}

/*==============================================================*/
/*			One of the ring, 30 degrees on from the one			*/
/*			before												*/
/*==============================================================*/
static void set03_03_ring_set( PLAYER_WORK *pl_w, int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set03_03_ring_move ;
		ew->obj = obj_stg->obj[10] ;
		ew->pos.y = nlCos( DEG_ANG( no * 30 ) ) * 2730.0f ;
		ew->pos.z = nlSin( DEG_ANG( no * 30 ) ) * 2730.0f ;
		ew->fade.r = set03_03_col_tbl[no].x ;
		ew->fade.g = set03_03_col_tbl[no].y ;
		ew->fade.b = set03_03_col_tbl[no].z ;
#ifdef TARGET_DC	/* paler in the stage's other look */
		if( stage_no == 12 ){
			ew->fade.r *= 0.2f ;
			ew->fade.g *= 0.2f ;
			ew->fade.b *= 0.2f ;
			ew->fade.r += 0.45f ;
			ew->fade.g += 0.55f ;
			ew->fade.b += 0.6f ;
		}
#endif
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_FADE | OBJ_BRIGHT ;
		ew->base_mtx = &pl_w->obj_mtx ;
	}
}

/*==============================================================*/
static void set03_03_move( PLAYER_WORK *ew )
{
	ew->rot[0] += 16 ;
}

/****************************************************************/
void set03_03_set( void )
{
	PLAYER_WORK *ew ;
	int i ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set03_03_move ;
		ew->obj = obj_stg->obj[9] ;
		ew->pos = set03_03_pos ;
		ew->rot[1] = 39687 ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_BRIGHT ;
		for( i = 0 ; i < 12 ; i ++ ) set03_03_ring_set( ew, i ) ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set03_03.c" End of File	*/

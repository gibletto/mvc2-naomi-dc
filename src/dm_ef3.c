/* The sparks round a player who is hit by element 3 (kind 7 works, five made by
   dm_ef3_set for dm00_04): each waits its turn, then shows one of two lightning models, frame by
   frame, sliding its texture. */
#include "player.h"

extern void obj_uv_begin( int *src, void *obj ) ;
extern int obj_uv_next( void ) ;
extern void obj_uv_get( Float *x, Float *y ) ;
extern void obj_uv_put( Float *x, Float *y ) ;

/* where the texture is for each frame, as u and v */
static const Float dm_ef3_uv_tbl[32] = {
	0.0f, 0.0f, 0.25f, 0.0f, 0.5f, 0.0f, 0.75f, 0.0f,
	0.0f, 0.25f, 0.25f, 0.25f, 0.5f, 0.25f, 0.75f, 0.25f,
	0.0f, 0.5f, 0.25f, 0.5f, 0.5f, 0.5f, 0.75f, 0.5f,
	0.0f, 0.75f, 0.25f, 0.75f, 0.5f, 0.75f, 0.75f, 0.75f,
} ;
/* the model's alpha and the frames it stays, for each frame of each of the two models */
static const Float dm_ef3_fade_tbl[16] = {
	1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
} ;
static const Sint16 dm_ef3_time_tbl[16] = {
	10, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2,
} ;

static void dm_ef3_main( PLAYER_WORK *ew );
static void dm_ef3_disp( PLAYER_WORK *ew );

static void dm_ef3_wait( PLAYER_WORK *pl_w, PLAYER_WORK *ew );
static void dm_ef3_show( PLAYER_WORK *pl_w, PLAYER_WORK *ew );
static void dm_ef3_move( PLAYER_WORK *pl_w, PLAYER_WORK *ew );
static void dm_ef3_hide( PLAYER_WORK *pl_w, PLAYER_WORK *ew );
static void dm_ef3_end( PLAYER_WORK *pl_w, PLAYER_WORK *ew );

/*==============================================================*/
static void dm_ef3_main( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	static void ( *dm_ef3_jmp[] )( PLAYER_WORK *pl_w, PLAYER_WORK *ew ) = {
		dm_ef3_wait, dm_ef3_show, dm_ef3_move, dm_ef3_hide, dm_ef3_end,
	} ;

	dm_ef3_jmp[ew->routine_0]( pl_w, ew ) ;
}

/*==============================================================*/
/*			It starts when its turn comes, unless the hit		*/
/*			has changed to another element						*/
/*==============================================================*/
static void dm_ef3_wait( PLAYER_WORK *pl_w, PLAYER_WORK *ew )
{
	if( pl_w->dm_elem != 1 ) EF_WK( ew )->work[2] = 0 ;
	if( -- EF_WK( ew )->work[2] + 1 == 0 ){
		ew->routine_0 = 1 ;
		obj_func_set( ew, dm_ef3_disp ) ;
	}
}

/*--------------------------------------------------------------*/
static void dm_ef3_show( PLAYER_WORK *pl_w, PLAYER_WORK *ew )
{
	const Sint16 *time ;
	const Float *fade ;

	fade = dm_ef3_fade_tbl ;
	time = dm_ef3_time_tbl ;
	time += EF_WK( ew )->work[3] * 8 ;
	fade += EF_WK( ew )->work[3] * 8 ;
	ew->disp_flag = 1 ;
	time += ew->timer0 ;
	ew->timer1 = *time ;
	fade += ew->timer0 ;
	ew->fade.trnsl = *fade ;
	ew->routine_0 = 2 ;
	if( ew->routine_3 ){
		ew->pos.x = pl_w->pos.x + ew->spd.x ;
		ew->pos.y = pl_w->pos.y + ew->spd.y ;
	}
	dm_ef3_move( pl_w, ew ) ;
}

/*--------------------------------------------------------------*/
static void dm_ef3_move( PLAYER_WORK *pl_w, PLAYER_WORK *ew )
{
	if( ew->routine_3 ){
		ew->routine_3 = 0 ;
		ew->pos.x = pl_w->pos.x + ew->spd.x ;
		ew->pos.y = pl_w->pos.y + ew->spd.y ;
	}
	if( -- ew->timer1 == 0 ){
		ew->routine_0 = 1 ;
		if( ++ ew->timer0 >= 8 ){
			ew->routine_0 = 3 ;
			ew->timer0 = 7 ;
		}
	}
}

/*--------------------------------------------------------------*/
static void dm_ef3_hide( PLAYER_WORK *pl_w, PLAYER_WORK *ew )
{
	obj_func_set( ew, 0 ) ;
	ew->disp_flag = 0 ;
	ew->routine_0 = 4 ;
}

/*--------------------------------------------------------------*/
static void dm_ef3_end( PLAYER_WORK *pl_w, PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			The texture slides to the frame's place				*/
/*==============================================================*/
static void dm_ef3_disp( PLAYER_WORK *ew )
{
	const Float *uv ;
	Float py, px ;

	uv = dm_ef3_uv_tbl ;
	uv += ew->timer0 * 2 ;

	if( EF_WK( ew )->work[3] ) obj_uv_begin( obj_efky->obj[107], ew->obj ) ;
	else obj_uv_begin( obj_efky->obj[105], ew->obj ) ;
	while( obj_uv_next() == 0 ){
		obj_uv_get( &px, &py ) ;
		px += uv[0] ;
		py -= uv[1] ;
		obj_uv_put( &px, &py ) ;
	}
}

/*==============================================================*/
static void dm_ef3_spark_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	Uint8 i ;
	int x, w ;

	for( i = 0 ; i < 5 ; i ++ ){
		if( !( ew = work_get( 0, W_EF, LIST_END ) ) ) return ;
		ew->disp_flag = 0 ;
		ew->work_func = dm_ef3_main ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_TRNSL ;
		ew->sub_no = i ;
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
		EF_WK( ew )->work[2] = ew->sub_no * 8 ;
		ew->master_w = pl_w ;
		ew->rot[1] = pl_w->disp_dir ? 0 : 0x8000 ;
		EF_WK( ew )->work[3] = Rnd() ;
		EF_WK( ew )->work[3] %= 2 ;
		if( EF_WK( ew )->work[3] ) ew->obj = obj_efky->obj[106] ;
		else ew->obj = obj_efky->obj[104] ;
		x = nlRand() ;
		w = ( int )( pl_w->size_front * pl_w->scale.x * 1.6666666f ) ;
		ew->spd.x += x % 2 ? x % w : -( x % w ) ;
		x = nlRand() ;
		w = ( int )( pl_w->size_top * pl_w->scale.y * 2.142857f ) ;
		ew->spd.y += x % w ;
		ew->pos.x = pl_w->pos.x + ew->spd.x ;
		ew->pos.y = pl_w->pos.y + ew->spd.y ;
		ew->routine_3 = 4 ;
		ew->timer0 = 2 ;
	}
}

/****************************************************************/
/*			Five sparks round the player						*/
/****************************************************************/
void dm_ef3_set( PLAYER_WORK *pl_w )
{
	dm_ef3_spark_set( pl_w ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "dm_ef3.c" End of File	*/

/* The sparks round a player who is hit by element 1 (kind 7 works, five made by
   dm_ef1_set for dm00_02): each waits its turn, then shows one of two lightning models, frame by
   frame, sliding its texture. */
#include "player.h"

extern void obj_uv_begin( int *src, void *obj ) ;
extern int obj_uv_next( void ) ;
extern void obj_uv_get( Float *x, Float *y ) ;
extern void obj_uv_put( Float *x, Float *y ) ;

/* where the texture is for each frame, as u and v */
static const Float dm_ef1_uv_tbl[16][2] = {
	{ 0.0f, 0.0f }, { 0.25f, 0.0f }, { 0.5f, 0.0f }, { 0.75f, 0.0f },
	{ 0.0f, 0.25f }, { 0.25f, 0.25f }, { 0.5f, 0.25f }, { 0.75f, 0.25f },
	{ 0.0f, 0.5f }, { 0.25f, 0.5f }, { 0.5f, 0.5f }, { 0.75f, 0.5f },
	{ 0.0f, 0.75f }, { 0.25f, 0.75f }, { 0.5f, 0.75f }, { 0.75f, 0.75f },
} ;
/* the model's alpha and the frames it stays, for each frame of each of the two models */
static const Float dm_ef1_fade_tbl[2][8] = {
	{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
} ;
static const Sint16 dm_ef1_time_tbl[2][8] = {
	{ 10, 2, 2, 2, 2, 2, 2, 2 },
	{ 2, 2, 2, 2, 2, 2, 2, 2 },
} ;

static void dm_ef1_main( PLAYER_WORK *ew );
static void dm_ef1_disp( PLAYER_WORK *ew );

static void dm_ef1_wait( PLAYER_WORK *pl_w, PLAYER_WORK *ew );
static void dm_ef1_show( PLAYER_WORK *pl_w, PLAYER_WORK *ew );
static void dm_ef1_move( PLAYER_WORK *pl_w, PLAYER_WORK *ew );
static void dm_ef1_hide( PLAYER_WORK *pl_w, PLAYER_WORK *ew );
static void dm_ef1_end( PLAYER_WORK *pl_w, PLAYER_WORK *ew );

/*==============================================================*/
static void dm_ef1_main( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	static void ( *dm_ef1_jmp[] )( PLAYER_WORK *pl_w, PLAYER_WORK *ew ) = {
		dm_ef1_wait, dm_ef1_show, dm_ef1_move, dm_ef1_hide, dm_ef1_end,
	} ;

	dm_ef1_jmp[ew->routine_0]( pl_w, ew ) ;
}

/*==============================================================*/
/*			It starts when its turn comes, unless the hit		*/
/*			has changed to another element						*/
/*==============================================================*/
static void dm_ef1_wait( PLAYER_WORK *pl_w, PLAYER_WORK *ew )
{
	if( pl_w->dm_elem != 1 ) EF_WK( ew )->work[2] = 0 ;
	if( -- EF_WK( ew )->work[2] + 1 == 0 ){
		ew->routine_0 = 1 ;
		obj_func_set( ew, dm_ef1_disp ) ;
	}
}

/*--------------------------------------------------------------*/
static void dm_ef1_show( PLAYER_WORK *pl_w, PLAYER_WORK *ew )
{
	const Sint16 *time ;
	const Float *fade ;

	fade = dm_ef1_fade_tbl[0] ;
	time = dm_ef1_time_tbl[0] ;
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
	dm_ef1_move( pl_w, ew ) ;
}

/*--------------------------------------------------------------*/
static void dm_ef1_move( PLAYER_WORK *pl_w, PLAYER_WORK *ew )
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
static void dm_ef1_hide( PLAYER_WORK *pl_w, PLAYER_WORK *ew )
{
	obj_func_set( ew, 0 ) ;
	ew->disp_flag = 0 ;
	ew->routine_0 = 4 ;
}

/*--------------------------------------------------------------*/
static void dm_ef1_end( PLAYER_WORK *pl_w, PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			The texture slides to the frame's place				*/
/*==============================================================*/
static void dm_ef1_disp( PLAYER_WORK *ew )
{
	const Float *uv ;
	Float py, px ;

	uv = dm_ef1_uv_tbl[0] ;
	uv += ew->timer0 * 2 ;

	if( EF_WK( ew )->work[3] ) obj_uv_begin( obj_efky->obj[102], ew->obj ) ;
	else obj_uv_begin( obj_efky->obj[103], ew->obj ) ;
	while( obj_uv_next() == 0 ){
		obj_uv_get( &px, &py ) ;
		px += uv[0] ;
		py -= uv[1] ;
		obj_uv_put( &px, &py ) ;
	}
}

/*==============================================================*/
static void dm_ef1_spark_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	Uint8 i ;
	int x, w ;

	i = 0 ;
	do {
		if( !( ew = work_get( 0, W_EF, LIST_END ) ) ) break ;
		ew->disp_flag = 0 ;
		ew->work_func = dm_ef1_main ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_TRNSL ;
		ew->sub_no = i ;
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
		EF_WK( ew )->work[2] = ew->sub_no * 8 ;
		ew->master_w = pl_w ;
		ew->rot[1] = pl_w->disp_dir ? 0 : 0x8000 ;
		EF_WK( ew )->work[3] = Rnd() ;
		EF_WK( ew )->work[3] %= 2 ;
		if( EF_WK( ew )->work[3] ) ew->obj = obj_efky->obj[101] ;
		else ew->obj = obj_efky->obj[99] ;
		x = nlRand() ;
		w = ( int )( pl_w->size_front * pl_w->scale.x * 1.6666666f ) ;
		ew->spd.x += x % 2 ? x % w : -( x % w ) ;
		w = nlRand() ;
		x = ( int )( pl_w->size_top * pl_w->scale.y * 2.142857f ) ;
		ew->spd.y += w % x ;
		ew->pos.x = pl_w->pos.x + ew->spd.x ;
		ew->pos.y = pl_w->pos.y + ew->spd.y ;
		ew->rot[1] = ew->disp_dir ? 0 : 0x8000 ;
		ew->routine_3 = 4 ;
		i ++ ;
	} while( i < 5 ) ;
}

/****************************************************************/
/*			Five sparks round the player						*/
/****************************************************************/
void dm_ef1_set( PLAYER_WORK *pl_w )
{
	dm_ef1_spark_set( pl_w ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "dm_ef1.c" End of File	*/

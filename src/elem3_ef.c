/* The five sparks of a hit by element 3 (kind 7 works): each stays near the
   player, steps through eight frames with its own time and alpha, and its texture slides by frame.
   dm00_04 calls the setter. */
#include "player.h"

extern void obj_uv_begin( int *src, void *obj );
extern int obj_uv_next( void );
extern void obj_uv_get( Float *x, Float *y );
extern void obj_uv_put( Float *x, Float *y );

/* a texture's place by frame, then each kind's alpha and frames by step */
static const Float elem3_uv_tbl[16][2] = {
	{ 0.0f, 0.0f }, { 0.25f, 0.0f }, { 0.5f, 0.0f }, { 0.75f, 0.0f },
	{ 0.0f, 0.25f }, { 0.25f, 0.25f }, { 0.5f, 0.25f }, { 0.75f, 0.25f },
	{ 0.0f, 0.5f }, { 0.25f, 0.5f }, { 0.5f, 0.5f }, { 0.75f, 0.5f },
	{ 0.0f, 0.75f }, { 0.25f, 0.75f }, { 0.5f, 0.75f }, { 0.75f, 0.75f },
} ;
static const Float elem3_alpha_tbl[16] = {
	1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
} ;
static const Sint16 elem3_time_tbl[16] = {
	2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2,
} ;

static void elem3_ef_wait( PLAYER_WORK *pl_w, PLAYER_WORK *ew ) ;
static void elem3_ef_init( PLAYER_WORK *pl_w, PLAYER_WORK *ew ) ;
static void elem3_ef_move( PLAYER_WORK *pl_w, PLAYER_WORK *ew ) ;
static void elem3_ef_off( PLAYER_WORK *pl_w, PLAYER_WORK *ew ) ;
static void elem3_ef_end( PLAYER_WORK *pl_w, PLAYER_WORK *ew ) ;

/*==============================================================*/
static void elem3_ef_main( PLAYER_WORK *ew )
{
	static void ( *elem3_ef_jmp[] )( PLAYER_WORK *pl_w, PLAYER_WORK *ew ) = {
		elem3_ef_wait, elem3_ef_init, elem3_ef_move, elem3_ef_off, elem3_ef_end,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	elem3_ef_jmp[ew->routine_0]( pl_w, ew ) ;
}

static void elem3_ef_disp( PLAYER_WORK *ew ) ;

/*==============================================================*/
/*			It waits for its turn, as long as the player is		*/
/*			hit by element 3									*/
/*==============================================================*/
static void elem3_ef_wait( PLAYER_WORK *pl_w, PLAYER_WORK *ew )
{
	if( pl_w->dm_elem != 3 ) EF_WK( ew )->work[2] = 0 ;
	if( -- EF_WK( ew )->work[2] + 1 == 0 ){
		ew->routine_0 = 1 ;
		obj_func_set( ew, elem3_ef_disp ) ;
	}
}

/*--------------------------------------------------------------*/
static void elem3_ef_init( PLAYER_WORK *pl_w, PLAYER_WORK *ew )
{
	const Sint16 *time ;
	const Float *fade ;

	fade = elem3_alpha_tbl ;
	time = elem3_time_tbl ;
	time += EF_WK( ew )->work[3] * 8 ;
	fade += EF_WK( ew )->work[3] * 8 ;
	ew->disp_flag = 1 ;
	time += ew->timer0 ;
	ew->timer1 = *time ;
	fade += ew->timer0 ;
	ew->fade.trnsl = *fade ;
	ew->routine_0 = 2 ;
	ew->pos.x = pl_w->pos.x + ew->spd.x ;
	ew->pos.y = pl_w->pos.y + ew->spd.y ;
	elem3_ef_move( pl_w, ew ) ;
}

/*--------------------------------------------------------------*/
static void elem3_ef_move( PLAYER_WORK *pl_w, PLAYER_WORK *ew )
{
	ew->pos.x = pl_w->pos.x + ew->spd.x ;
	ew->pos.y = pl_w->pos.y + ew->spd.y ;
	if( !( -- ew->timer1 ) ){
		ew->routine_0 = 1 ;
		if( ++ ew->timer0 >= 8 ){
			ew->routine_0 = 3 ;
			ew->timer0 = 7 ;
		}
	}
}

/*--------------------------------------------------------------*/
static void elem3_ef_off( PLAYER_WORK *pl_w, PLAYER_WORK *ew )
{
	obj_func_set( ew, 0 ) ;
	ew->disp_flag = 0 ;
	ew->routine_0 = 4 ;
}

/*--------------------------------------------------------------*/
static void elem3_ef_end( PLAYER_WORK *pl_w, PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			Its texture steps through a sheet of sixteen		*/
/*			pictures											*/
/*==============================================================*/
static void elem3_ef_disp( PLAYER_WORK *ew )
{
	Float px, py ;
	const Float *uv = elem3_uv_tbl[0] ;

	uv += ew->timer0 * 2 ;
	if( EF_WK( ew )->work[3] ) obj_uv_begin( obj_efky->obj[81], ew->obj ) ;
	else obj_uv_begin( obj_efky->obj[87], ew->obj ) ;
	while( obj_uv_next() == 0 ){
		obj_uv_get( &px, &py ) ;
		px += uv[0] ;
		py -= uv[1] ;
		obj_uv_put( &px, &py ) ;
	}
}

/*==============================================================*/
static void elem3_ef_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	Uint8 i ;
	Sint32 rnd, n ;

	i = 0 ;
	do {
		if( !( ew = work_get( 0, W_EF, LIST_END ) ) ) break ;
		ew->disp_flag = 0 ;
		ew->work_func = elem3_ef_main ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_ROT_Z | OBJ_TRNSL ;
		ew->sub_no = i ;
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
		EF_WK( ew )->work[2] = ew->sub_no * 8 ;
		ew->master_w = pl_w ;
		ew->rot[1] = pl_w->disp_dir ? 0 : 0x8000 ;
		EF_WK( ew )->work[3] = Rnd() ;
		EF_WK( ew )->work[3] %= 2 ;
		if( EF_WK( ew )->work[3] ) ew->obj = obj_efky->obj[80] ;
		else ew->obj = obj_efky->obj[86] ;
		rnd = nlRand() ;
		n = ( int )( pl_w->size_front * pl_w->scale.x * 1.6666666f ) ;
		ew->spd.x += ( rnd % 2 ) ? rnd % n : -( rnd % n ) ;
		rnd = nlRand() ;
		n = ( int )( pl_w->size_top * pl_w->scale.y * 2.142857f ) ;
		ew->spd.y += rnd % n ;
		ew->pos.x = pl_w->pos.x + ew->spd.x ;
		ew->pos.y = pl_w->pos.y + ew->spd.y ;
		rnd = nlRand() ;
		ew->rot[0] = ( rnd % 2 ) ? 0 : 0x8000 ;
		rnd = nlRand() ;
		ew->rot[1] = ( rnd % 2 ) ? 0 : 0x8000 ;
		rnd = nlRand() ;
		ew->rot[2] = ( rnd % 2 ) ? 0 : 0x8000 ;
		i ++ ;
	} while( i < 5 ) ;
}

/****************************************************************/
void thunder_spark_set( PLAYER_WORK *pl_w )
{
	elem3_ef_set( pl_w ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "elem3_ef.c" End of File	*/

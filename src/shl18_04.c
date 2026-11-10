/* Abyss's effect 04 (character 18's shot 04): the six dark flares of pl18_sp_sweep that fly out of
   one point in a fan, and the trail of little pictures each leaves behind. */
#include "player.h"
#include "pl_char.h"
#include "pl18_def.h"

/* the flare's animation, by its kind and its place in the fan */
static Uint8 shl18_04_char_tbl[4][6] = {
	{ 0, 1, 2, 0, 1, 0 }, { 1, 0, 0, 2, 1, 0 }, { 2, 0, 1, 0, 1, 0 }, { 1, 0, 1, 0, 2, 0 },
} ;

static Sint32 shl18_04_obj_tbl[] = { 28, 30, 29 } ;
static Float shl18_04_dist_tbl[] = { 40.0f, 70.0f, 100.0f } ;
static Float shl18_04_spd_tbl[] = { 15.0f, 15.0f, 15.0f, 15.0f } ;
static VEC3 shl18_04_scale = { 0.1f, 0.1f, 0.1f } ;

static void shl18_04_init( PLAYER_WORK *ew );
static void shl18_04_move( PLAYER_WORK *ew );
static void shl18_04_end( PLAYER_WORK *ew );
static void shl18_04_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl18_04_main( PLAYER_WORK *ew )
{
	static void ( *shl18_04_jmp[] )( PLAYER_WORK *ew ) = {
		shl18_04_init, shl18_04_move, shl18_04_end, shl18_04_free,
	} ;

	shl18_04_jmp[ew->routine_0]( ew ) ;
}

static void shl18_04_trail_set( PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl18_04_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
	ew->ef_no = 0x1804 ;
	pl_w = ew->master_w ;
	CHAR_PART_COPY( ew, pl_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = pl_w->pl_no ;
	ew->pl_type = pl_w->pl_type ;
	ew->scale.x = pl_w->scale.x ;
	ew->scale.y = pl_w->scale.y ;
	ew->atck_level = pl_w->atck_level ;
	ew->work_no = pl_w->work_no ;
	ew->col_cnt = pl_w->col_cnt ;
	ew->scale = pl_w->scale ;
	ew->prio = pl_w->prio ;
	ew->disp_flag = 1 ;
	ew->size_top = 16 ;
	ew->size_bottom = 16 ;
	ew->size_front = 16 ;
	ew->size_back = 16 ;
	ew->sub_no = shl18_04_char_tbl[ew->type][ew->sub_type] ;
	ew->spd.x = shl18_04_spd_tbl[ew->type] * nlSin( ( int )( ( ew->sub_type * 15 + 60 ) * 65536.0f / 360.0f + 0.5f ) & 0xffff ) ;
	ew->spd.y = shl18_04_spd_tbl[ew->type] * nlCos( ( int )( ( ew->sub_type * 15 + 60 ) * 65536.0f / 360.0f + 0.5f ) & 0xffff ) ;
	if( !ew->disp_dir ) ew->spd.x = -ew->spd.x ;
	ew->acc.x = 0 ;
	ew->acc.y = 0 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = ew->sub_no + 55 ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, Char_Effect, ew->sub_no + 4 ) ;
	shl18_04_trail_set( ew ) ;
	shl18_04_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl18_04_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !scr_in_ck( ew ) ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl18_04_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl18_04_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			The trail picture stays on the flare's place		*/
/*			until it is gone									*/
/*==============================================================*/
static void shl18_04_trail( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_0 >= 2 ){
		work_free( ew ) ;
		return ;
	}
	ew->pos = pl_w->pos ;
}

/*==============================================================*/
static void shl18_04_trail_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->master_w = pl_w ;
		ew->ef_no = 0x1804 ;
		ew->disp_flag = 1 ;
		ew->work_func = shl18_04_trail ;
		ew->obj_flag = 31 ;
		ew->rot[2] = nlRand() % 0x4000 ;
		ew->scale = shl18_04_scale ;
		ew->obj = obj_stg->obj[shl18_04_obj_tbl[pl_w->sub_no]] ;
	}
}

/*==============================================================*/
static void shl18_04_set( PLAYER_WORK *pl_w, VEC3 *pos, Uint8 type, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->master_w = pl_w ;
		ew->work_func = shl18_04_main ;
		ew->pos = *pos ;
		ew->type = type ;
		ew->sub_type = no ;
	}
}

/****************************************************************/
void shl18_04_set_all( PLAYER_WORK *pl_w, VEC3 *pos, Uint8 type )
{
	Uint8 i ;

	for( i = 0 ; i < 6 ; i ++ ){
		shl18_04_set( pl_w, pos, type, i ) ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl18_04.c" End of File	*/

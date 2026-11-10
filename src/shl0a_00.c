/* Rogue's effect 00 (character 0a's shot 00): a sprite that drifts out from her for 32 frames,
   turning through eight directions as it goes; kind 0 hits, the others are only drawn. Its
   timers, the frames left and the frames to the next turn, are kept in obj_mtx, which a sprite
   doesn't use. */
#include "player.h"
#include "pl_char.h"
#include "pl0a_def.h"

/* the effect's own work */
typedef struct SHL0A_00_LOCAL_W {
	Uint16	char_no;	/* 000: Rogue's animation when it was set */
} SHL0A_00_LOCAL_W;
#define SHL0A_00_LW( w )	( ( SHL0A_00_LOCAL_W * )&( w )->obj_flag )

static void shl0a_00_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl0a_00_set( PLAYER_WORK *pl_w, Float x, Float y, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl0a_00_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0xa00 ;
		SHL0A_00_LW( ew )->char_no = pl_w->char_no.w ;
		if( pl_w->disp_dir ) x = -x ;
		ew->pos.x = pl_w->pos.x + DOT_X( x ) ;
		ew->pos.y = pl_w->pos.y + DOT_Y( y ) ;
		ew->sub_type = no ;
	}
	return ew ;
}

static void shl0a_00_init( PLAYER_WORK *ew );
static void shl0a_00_move( PLAYER_WORK *ew );
static void shl0a_00_end( PLAYER_WORK *ew );
static void shl0a_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl0a_00_main( PLAYER_WORK *ew )
{
	static void ( *shl0a_00_jmp[] )( PLAYER_WORK *ew ) = {
		shl0a_00_init, shl0a_00_move, shl0a_00_end, shl0a_00_free,
	} ;

	shl0a_00_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl0a_00_init( PLAYER_WORK *ew )
{
	static Uint8 shl0a_00_kind[4][2] = {	/* by the kind: whether it scatters, and whether it is only drawn */
		{ 0, 0 }, { 0, 1 }, { 1, 1 }, { 0, 0 },
	} ;

	Uint8 *kind ;
	Sint16 spd ;
	Uint8 dir ;

	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->prio = 0 ;
	dir = 0 ;
	if( !ew->disp_dir ) dir = 24 ;
	ew->type = dir ;	/* the first direction, in 32 steps */
	ew->sub_no = 4 ;
	kind = shl0a_00_kind[ew->sub_type] ;
	spd = 100 ;
	if( kind[0] ){
		spd = 200 ;
		ew->type = Rnd() & 31 ;
	}
	ew->timer1 = spd ;
	ew->obj_mtx.m[0][0] = 32.0f ;
	ew->obj_mtx.m[0][1] = 2.0f ;
	ew->sub_type = kind[1] ;
	if( !ew->sub_type ){
		ew->atck_flag = HIT_ON | HIT_SHOT ;
		ew->dm_flag = HIT_ON | HIT_SHOT ;
		ew->atck_no = pl0a_at_kiss ;
		HIT_RESET( ew ) ;
	}
	pl_char_set( ew, Char_Effect, 0 ) ;
}

/*--------------------------------------------------------------*/
static void shl0a_00_move( PLAYER_WORK *ew )
{
	if( -- ew->obj_mtx.m[0][1] < 0.0f ){
		ew->obj_mtx.m[0][1] = 2.0f ;
		ew->sub_no ++ ;
		ew->sub_no &= 7 ;
		ew->dir = ew->type + ew->sub_no ;
	}
	if( -- ew->obj_mtx.m[0][0] == 0.0f ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	dir_spd_move( ew, ew->timer1 ) ;
	pl_char_move( ew ) ;
	if( !ew->sub_type ) hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl0a_00_end( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	shl0a_00_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl0a_00_free( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl0a_00.c" End of File	*/

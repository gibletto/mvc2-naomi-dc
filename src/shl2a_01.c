/* Storm's effect 01 (character 2a's shot 01): the two twisters of her Double Typhoon (sp_dtyphoon),
   side by side at her opponent's feet, and the two pieces each one stacks above itself. */
#include "player.h"
#include "pl_char.h"
#include "pl2a_def.h"

/* the effect's own work, from obj_flag */
typedef struct SHL2A_01_LOCAL_W {
	Sint32	unk_00[2];
	PLAYER_WORK	*pl_w;	/* 008: Storm */
} SHL2A_01_LOCAL_W;
#define SHL2A_01_LW( w )	( ( SHL2A_01_LOCAL_W * )&( w )->obj_flag )

static void shl2a_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl2a_01_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	Sint16 i ;

	for( i = 1 ; i >= 0 ; i -- ){
		if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
			ew->work_func = shl2a_01_main ;
			ew->master_w = pl_w ;
			SHL2A_01_LW( ew )->pl_w = pl_w ;
			ew->pl_type = pl_w->pl_type ;
			ew->sub_type = 0 ;
			ew->type = i ;
			ew->ef_no = 0x2a01 ;
		}
	}
	return ew ;
}

/****************************************************************/
PLAYER_WORK *shl2a_01_set0( PLAYER_WORK *w, Uint16 y )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl2a_01_main ;
		ew->master_w = w ;
		SHL2A_01_LW( ew )->pl_w = w->master_w ;
		ew->pl_type = w->pl_type ;
		ew->sub_type = 1 ;
		ew->pos.x = w->pos.x ;
		ew->pos.y = w->pos.y + DOT_Y( y ) ;
		ew->ef_no = 0x2a01 ;
	}
	return ew ;
}

static void shl2a_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2a_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2a_01_stay( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2a_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl2a_01_main( PLAYER_WORK *ew )
{
	static void ( *shl2a_01_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2a_01_init, shl2a_01_move, shl2a_01_stay, shl2a_01_free,
	} ;

	shl2a_01_jmp[ew->routine_0]( ew, SHL2A_01_LW( ew )->pl_w ) ;
}

/*==============================================================*/
static void shl2a_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w ;
	Sint16 ofs ;
	Uint8 no ;
	Sint8 prio ;

	ew->routine_0 ++ ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = pl2a_at_dtyphoon ;
	HIT_RESET( ew ) ;
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
	ew->disp_flag = 0 ;
	if( !ew->sub_type ){
		prio = 0 ;
		if( ew->type ) prio = 7 ;
		ew->prio = prio ;
		em_w = pl_w->target_w ;
		ew->pos.x = em_w->pos.x ;
		ew->pos.y = em_w->ground_y ;
		ofs = 88 ;
		if( ew->disp_dir ) ofs = -88 ;
		ew->pos.x += DOT_X( ofs ) ;
		ofs = ew->type << 5 ;
		if( ew->type ) ofs = -ofs ;
		ew->pos.x += DOT_X( ofs ) ;
		no = 19 ;
	} else {
		ew->routine_1 = 1 ;
		ew->timer0 = 60 ;
		no = 20 ;
	}
	pl_char_set( ew, Char_Effect, no ) ;
	pl_w->cg_ctr = ( Rnd() & 3 ) + 1 ;
}

/*--------------------------------------------------------------*/
static void shl2a_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Sint16 ofs ;

	if( ew->pl_type != ew->master_w->pl_type ){
		shl2a_01_free( ew, pl_w ) ;
		return ;
	}
	ew->disp_flag = 1 ;
	if( pl_w->routine_1 != PL_NORM ) goto end ;
	if( !ew->sub_type && !ew->routine_1 ){
		pl_char_move( ew ) ;
		if( !ew->cg_type ) return ;
		ew->routine_1 ++ ;
		ew->cg_type = 0 ;
		ew->timer0 = 60 ;
		ofs = 88 ;
		if( ew->disp_dir ) ofs = -88 ;
		ew->pos.x -= DOT_X( ofs ) ;
		shl2a_01_set0( ew, 136 ) ;
		shl2a_01_set0( ew, 272 ) ;
		return ;
	}
	if( ! -- ew->timer0 ) goto end ;
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
	return ;
end:
	if( ew->routine_1 ){
		ew->routine_0 ++ ;
		pl_char_set( ew, Char_Effect, 21 ) ;
		return ;
	}
	shl2a_01_free( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl2a_01_stay( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ) shl2a_01_free( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl2a_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl2a_01.c" End of File	*/

/* Sentinel's effect 04 (character 34's shot 04): Plasma Storm's blow that comes out in front
   of him, a fist that appears there and strikes while his animation runs. */
#include "player.h"
#include "pl_char.h"
#include "pl34_def.h"

static void shl34_04_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl34_04_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl34_04_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x3404 ;
	}
	return ew ;
}

static void shl34_04_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl34_04_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl34_04_end( PLAYER_WORK *ew );
static void shl34_04_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl34_04_main( PLAYER_WORK *ew )
{
	static void ( *shl34_04_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl34_04_init, shl34_04_move, shl34_04_end, shl34_04_free,
	} ;

	shl34_04_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void shl34_04_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
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
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->spd.x = DOT_X( 127.0f ) ;
	ew->spd.y = 240.0f ;
	if( !ew->disp_dir ) ew->spd.x = -ew->spd.x ;
	ew->sub_prio = -1 ;
	ew->atck_no = pl34_at_plasma ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, Char_Effect, 1 ) ;
	ew->pos.x = pl_w->pos.x + ew->spd.x ;
	ew->pos.y = pl_w->pos.y + ew->spd.y ;
}

static void shl34_04_reach( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl34_04_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->prio = pl_w->prio ;
	ew->pos.x = pl_w->pos.x + ew->spd.x ;
	ew->pos.y = pl_w->pos.y + ew->spd.y ;
	ew->hit_box_no = 0 ;
	if( pl_w->routine_1 || pl_w->act_st != N_SUPER ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( pl_w->routine_2 >= 3 && !ew->routine_1 ){
		ew->routine_1 ++ ;
		ew->disp_flag = 1 ;
		pl_char_set( ew, Char_Effect, 2 ) ;
	}
	if( !ew->routine_1 ){
		pl_char_move( ew ) ;
	} else if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( ew->cg_type ){
		ew->atck_no = ew->cg_type ;
		HIT_RESET( ew ) ;
		ew->cg_type = 0 ;
	}
	shl34_04_reach( ew ) ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl34_04_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl34_04_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			His attack's reach: it hits harder when the			*/
/*			opponent is far										*/
/*==============================================================*/
static void shl34_04_reach( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *tw = pl_w->target_w ;
	Float dx ;

	if( !ew->routine_1 ){
		dx = tw->pos.x - ew->pos.x ;
		if( !pl_w->disp_dir ) dx = -dx ;
		if( dx > 80.0f ) ew->atck_attr = ATR_BACK_POS ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl34_04.c" End of File	*/

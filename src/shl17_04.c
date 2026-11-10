/* Cable's effect 04 (character 17's shot 04): the blast at the end of Crackdown's charge, which
   grows and fades as it flies; it hits only if the charge itself didn't. */
#include "player.h"
#include "pl_char.h"
#include "pl17_def.h"

static void shl17_04_main( PLAYER_WORK *ew );
static void shl17_04_show_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl17_04_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( pl_w->hit_flag ){
		if( !( ew = work_get( 0, W_PLS, LIST_TOP ) ) ) return ;
		ew->work_func = shl17_04_show_main ;
	} else {
		if( !( ew = work_get( 0, W_SHL, LIST_TOP ) ) ) return ;
		ew->work_func = shl17_04_main ;
	}
	ew->master_w = pl_w ;
	ew->ef_no = 0x1704 ;
	ew->disp_dir = ew->master_w->disp_dir ;
	return ew ;
}

static void shl17_04_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl17_04_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl17_04_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl17_04_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl17_04_main( PLAYER_WORK *ew )
{
	static void ( *shl17_04_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl17_04_init, shl17_04_move, shl17_04_end, shl17_04_free,
	} ;

	shl17_04_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void shl17_04_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 1 ;
	ew->pos = pl_w->pos ;
	ew->pos.x += pl_w->disp_dir ? 117.0f : -117.0f ;
	ew->pos.y += 129.0f ;
	ew->prio = 0 ;
	ew->timer0 = 14 ;
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
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = pl17_at_crackdown_blast ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, Char_Effect, 11 ) ;
	ew->spd.x = ew->disp_dir ? 10.0f : -10.0f ;
	ew->scale.x = 1.0f ;
	ew->scale.y = 1.0f ;
}

/*--------------------------------------------------------------*/
static void shl17_04_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->scale.x += 1.0f / 14 ;
	ew->scale.y += 1.0f / 14 ;
	if( ew->hit_box_no || -- ew->timer0 < 0 ){
		ew->alpha = 0.0f ;
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( ew->timer0 < 7 ) ew->alpha -= 1.0f / 7 ;
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl17_04_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl17_04_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

static void shl17_04_show_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl17_04_show_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The blast that only shows							*/
/*==============================================================*/
static void shl17_04_show_main( PLAYER_WORK *ew )
{
	static void ( *shl17_04_show_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl17_04_show_init, shl17_04_show_move, shl17_04_end, shl17_04_free,
	} ;

	shl17_04_show_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void shl17_04_show_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->disp_flag = 1 ;
	ew->pos = pl_w->pos ;
	ew->pos.x += pl_w->disp_dir ? 117.0f : -117.0f ;
	ew->pos.y += 129.0f ;
	ew->prio = 0 ;
	ew->timer0 = 14 ;
	pl_char_set( ew, Char_Effect, 11 ) ;
	ew->spd.x = ew->disp_dir ? 10.0f : -10.0f ;
	ew->scale.x = 1.0f ;
	ew->scale.y = 1.0f ;
}

/*--------------------------------------------------------------*/
static void shl17_04_show_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->scale.x += 1.0f / 14 ;
	ew->scale.y += 1.0f / 14 ;
	if( ew->pl_type != pl_w->pl_type ){
		shl17_04_free( ew, pl_w ) ;
		return ;
	}
	if( -- ew->timer0 < 0 ){
		ew->alpha = 0.0f ;
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( ew->timer0 < 7 ) ew->alpha -= 1.0f / 7 ;
	pl_char_move( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl17_04.c" End of File	*/

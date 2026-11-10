/* Zangief's breath (character 01's shot 01): it stays in front of him while he breathes, hits for
   a time set by the strength, and tells him when it is done. */
#include "player.h"
#include "pl_char.h"
#include "pl01_def.h"

static void shl01_01_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl01_01_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl01_01_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = 0 ;
		ew->sub_type = 0 ;
		ew->ef_no = 0x101 ;
	}
	return ew ;
}

static void shl01_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl01_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl01_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl01_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl01_01_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl01_01_main( PLAYER_WORK *ew )
{
	static void ( *shl01_01_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl01_01_init, shl01_01_move, shl01_01_end, shl01_01_free,
	} ;

	shl01_01_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void shl01_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Sint16 shl01_01_time[] = { 30, 60 } ;	/* frames it hits, by strength */

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
	shl01_01_pos_set( ew, pl_w ) ;
	ew->timer0 = shl01_01_time[pl_w->atck_level] ;
	ew->timer1 = pl_w->char_no.w ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	pl_char_set( ew, 20, 1 ) ;
	ew->atck_no = pl01_at_vodka ;
	HIT_RESET( ew ) ;
	shl01_01_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl01_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ew->timer1 != pl_w->char_no.w || pl_w->act_st != N_SPECIAL ){
		ew->routine_0 ++ ;
		return ;
	}
	ew->sub_prio = -2 ;
	shl01_01_pos_set( ew, pl_w ) ;
	if( ew->routine_1 ){
		if( pl_char_move( ew ) >= 0 ) return ;
		ew->disp_flag = 0 ;
		ew->routine_0 ++ ;
		return ;
	}
	if( ew->cg_type ){
		ew->cg_type = 0 ;
		ew->atck_no = pl01_at_vodka ;
		HIT_RESET( ew ) ;
	}
	if( !ew->hit_box_no ){
		pl_char_move( ew ) ;
		if( ew->timer0 -- ){
			hit_work_set( ew ) ;
			return ;
		}
		ew->timer0 = 1 ;
		if( !ew->cg_type ) return ;
	}
	ew->routine_1 ++ ;
	pl_char_set( ew, 20, 2 ) ;
}

/*--------------------------------------------------------------*/
static void shl01_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL01_LOCAL_W *lw = ( PL01_LOCAL_W * )pl_w->tlocal ;

	lw->breath_end = 1 ;
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl01_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			In front of Zangief's mouth							*/
/*==============================================================*/
static void shl01_01_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->disp_dir ) ew->pos.x = pl_w->pos.x + 140.0f ;
	else ew->pos.x = pl_w->pos.x - 140.0f ;
	ew->pos.y = pl_w->pos.y + DOT_Y( 83.0f ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl01_01.c" End of File	*/

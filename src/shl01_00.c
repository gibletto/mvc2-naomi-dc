/* Zangief's effect 00 (character 01's shot 00): an attack that stays on him while his animation
   lasts. */
#include "player.h"
#include "pl_char.h"

static void shl01_00_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl01_00_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_END ) ) ){
		ew->work_func = shl01_00_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = 0 ;
		ew->sub_type = 0 ;
		ew->ef_no = 0x100 ;
	}
	return ew ;
}

static void shl01_00_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl01_00_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl01_00_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl01_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl01_00_main( PLAYER_WORK *ew )
{
	static void ( *shl01_00_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl01_00_init, shl01_00_move, shl01_00_end, shl01_00_free,
	} ;

	PLAYER_WORK *pl_w = ew->master_w ;

	ew->hit_box_no = 0 ;
	ew->prio = pl_w->prio ;
	shl01_00_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl01_00_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->atck_level = pl_w->atck_level ;
	ew->timer1 = pl_w->char_no.w ;
	ew->sub_prio = -2 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	pl_char_set( ew, Char_Effect, 0 ) ;
	ew->atck_no = ew->atck_level + 48 ;
	HIT_RESET( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl01_00_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->sub_prio = -2 ;
	if( ew->timer1 != pl_w->char_no.w ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( !pl_w->hit_stop ){
		if( pl_char_move( ew ) < 0 ){
			ew->routine_0 ++ ;
			ew->disp_flag = 0 ;
			return ;
		}
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl01_00_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl01_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl01_00.c" End of File	*/

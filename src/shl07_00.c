/* Wolverine's effect 00 (character 07's shot 00): two layers of the slashes of his Berserker
   Barrage and Berserker Barrage X, one of which hits. */
#include "player.h"
#include "pl_char.h"

static void shl07_00_main( PLAYER_WORK *ew );

/****************************************************************/
int shl07_00_set( PLAYER_WORK *pl_w, int no )
{
	PLAYER_WORK *ew ;
	int i ;

	if( work_rest_cnt <= 2 ) return 0 ;
	for( i = 0 ; i < 2 ; i ++ ){
		ew = work_get( 0, W_SHL, LIST_END ) ;
		ew->ef_no = 0x700 ;
		ew->type = no ;
		ew->sub_type = i ;
		ew->work_func = shl07_00_main ;
		ew->master_w = pl_w ;
	}
	return 1 ;
}

static void shl07_00_init( PLAYER_WORK *ew );
static void shl07_00_move( PLAYER_WORK *ew );
static void shl07_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl07_00_main( PLAYER_WORK *ew )
{
	static void ( *shl07_00_jmp[] )( PLAYER_WORK *ew ) = {
		shl07_00_init, shl07_00_move, shl07_00_free,
	} ;

	shl07_00_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl07_00_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	int no ;

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
	ew->atck_flag = HIT_ON ;
	ew->dm_flag = HIT_ON ;
	ew->prio = ew->sub_type ? 12 : 11 ;
	ew->pos = pl_w->pos ;
	if( !ew->sub_type ){
		no = ew->atck_level * 2 + ew->type + 59 ;
		if( ( Sint8 )pl_w->type > 0 ) no = 97 ;	/* read signed */
		if( pl_w->pl_sp_type == 5 ) no = ew->type + 65 ;
		ew->atck_no = no ;
		HIT_RESET( ew ) ;
		if( pl_w->pl_sp_type == 5 ) ew->atck_attr = ATR_DOWN_LIFT ;
		hit_work_set( ew ) ;
	}
	no = ew->atck_level * 4 + ew->type * 2 + ew->sub_type ;
	pl_char_set( ew, Char_Effect, no + 2 ) ;
}

/*--------------------------------------------------------------*/
static void shl07_00_move( PLAYER_WORK *ew )
{
	if( !( vs_sys_work.slow_bit & 1 << vs_sys_work.slow_step ) ){
		if( pl_char_move( ew ) < 0 ){
			ew->routine_0 ++ ;
			ew->disp_flag = 0 ;
		} else if( !ew->type ) hit_work_set( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl07_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl07_00.c" End of File	*/

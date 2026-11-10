/* Wolverine's effect 01 (character 07's shot 01): two layers of a slash at the height of the
   opponent he has caught. */
#include "player.h"
#include "pl_char.h"
#include "pl07_def.h"

static void shl07_01_main( PLAYER_WORK *ew );

/****************************************************************/
int shl07_01_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	int i ;

	if( work_rest_cnt <= 2 ) return 0 ;
	for( i = 0 ; i < 2 ; i ++ ){
		ew = work_get( 0, W_SHL, LIST_END ) ;
		ew->ef_no = 0x701 ;
		ew->type = i ;
		ew->work_func = shl07_01_main ;
		ew->master_w = pl_w ;
	}
	return 1 ;
}

static void shl07_01_init( PLAYER_WORK *ew );
static void shl07_01_move( PLAYER_WORK *ew );
static void shl07_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl07_01_main( PLAYER_WORK *ew )
{
	static void ( *shl07_01_jmp[] )( PLAYER_WORK *ew ) = {
		shl07_01_init, shl07_01_move, shl07_01_free,
	} ;

	shl07_01_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl07_01_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PL07_LOCAL_W *lw = ( PL07_LOCAL_W * )pl_w->tlocal ;

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
	ew->prio = ew->type ? 12 : 11 ;
	ew->pos = pl_w->pos ;
	{
		PLAYER_WORK *em = lw->em_w ;

		ew->pos.y += em->size_top / 2 * 2.142857f ;
	}
	if( !ew->sub_type ){
		ew->atck_no = pl07_at_weapon_x_slash ;
		HIT_RESET( ew ) ;
		hit_work_set( ew ) ;
	}
	pl_char_set( ew, Char_Effect, ew->type ) ;
}

/*--------------------------------------------------------------*/
static void shl07_01_move( PLAYER_WORK *ew )
{
	if( !( vs_sys_work.slow_bit & 1 << vs_sys_work.slow_step ) ){
		if( pl_char_move( ew ) < 0 ){
			ew->routine_0 ++ ;
			ew->disp_flag = 0 ;
		} else if( !ew->type ) hit_work_set( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl07_01_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl07_01.c" End of File	*/

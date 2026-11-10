/* Venom's Venom Rush attack (character 0e's shot 00): the hit that rides on the effect which sets it, for
   as long as Venom stays in the move. */
#include "player.h"
#include "pl_char.h"
#include "pl0e_def.h"

static void shl0e_00_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl0e_00_set( PLAYER_WORK *wk, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_END ) ) ){
		ew->ef_no = 0x0e00 ;
		ew->work_func = shl0e_00_main ;
		ew->ef_w = wk ;
		ew->master_w = pl_w ;
	}
	return ew ;
}

static void shl0e_00_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0e_00_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0e_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl0e_00_main( PLAYER_WORK *ew )
{
	static void ( *shl0e_00_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0e_00_init, shl0e_00_move, shl0e_00_free,
	} ;

	shl0e_00_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void shl0e_00_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0E_LOCAL_W *lw = ( PL0E_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *wk = ew->ef_w ;

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
	ew->prio = pl_w->prio ;
	ew->sub_prio = -8 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->pos = wk->pos ;
	ew->size_front = 32 ;
	ew->size_back = 32 ;
	if( !scr_in_x_ck( ew ) ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		lw->rush_st = 1 ;
		return ;
	}
	ew->atck_no = pl0e_at_rush ;
	HIT_RESET( ew ) ;
	hit_work_set( ew ) ;
	pl_char_set( ew, Char_Effect, 20 ) ;
}

/*--------------------------------------------------------------*/
static void shl0e_00_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0E_LOCAL_W *lw = ( PL0E_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *wk = ew->ef_w ;

	if( pl_w->routine_1 || pl_w->act_st != N_SPECIAL || pl_w->pl_sp_type != pl0e_sp_rush ) goto vanish ;
	ew->prio = pl_w->prio ;
	ew->sub_prio = -8 ;
	if( vs_sys_work.slow_bit & 1 << vs_sys_work.slow_step ) return ;
	if( ew->hit_box_no || pl_char_move( ew ) < 0 ) goto vanish ;
	ew->pos = wk->pos ;
	if( ew->cg_flag1 ){
		ew->atck_no = ew->cg_flag1 ;
		HIT_RESET( ew ) ;
		ew->cg_flag1 = 0 ;
	}
	hit_work_set( ew ) ;
	return ;
vanish :
	ew->routine_0 = 2 ;
	ew->disp_flag = 0 ;
	lw->rush_st = 1 ;
}

/*--------------------------------------------------------------*/
static void shl0e_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl0e_00.c" End of File	*/

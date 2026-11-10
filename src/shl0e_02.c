/* Venom's Death Bite (character 0e's shot 02): a chain of hits, each set ahead of the one before, while
   Venom stays in the move and has hits left. */
#include "player.h"
#include "pl_char.h"
#include "pl0e_def.h"

static void shl0e_02_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl0e_02_set( PLAYER_WORK *wk, int no, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_END ) ) ){
		ew->ef_no = 0x0e02 ;
		ew->type = no ;
		ew->pos = wk->pos ;
		ew->master_w = pl_w ;
		ew->work_func = shl0e_02_main ;
	}
	return ew ;
}

static void shl0e_02_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0e_02_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0e_02_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl0e_02_main( PLAYER_WORK *ew )
{
	static void ( *shl0e_02_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0e_02_init, shl0e_02_move, shl0e_02_free,
	} ;

	shl0e_02_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void shl0e_02_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0E_LOCAL_W *lw = ( PL0E_LOCAL_W * )pl_w->tlocal ;
	Float x ;

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
	ew->routine_0 ++ ;
	ew->atck_flag = HIT_ON | HIT_PL_ONLY ;
	ew->dm_flag = HIT_ON | HIT_PL_ONLY ;
	ew->prio = pl_w->prio ;
	ew->sub_prio = -8 ;
	ew->timer0 = 0 ;
	x = -160.0f ;
	if( ew->disp_dir ) x = 160.0f ;
	ew->pos.x += x ;
	ew->size_front = 32 ;
	ew->size_back = 32 ;
	if( !scr_in_x_ck( ew ) || ew->type > 6 || lw->bite_hit_cnt <= 0 ){
		ew->routine_1 = 2 ;
		ew->disp_flag = 0 ;
		ew->timer1 = 20 ;
		return ;
	}
	ew->atck_no = pl0e_at_bite ;
	HIT_RESET( ew ) ;
	hit_work_set( ew ) ;
	pl_char_set( ew, Char_Effect, 18 ) ;
}

static void shl0e_02_hit( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw );
static void shl0e_02_last( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw );
static void shl0e_02_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw );

/*--------------------------------------------------------------*/
static void shl0e_02_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0e_02_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw ) = {
		shl0e_02_hit, shl0e_02_last, shl0e_02_wait,
	} ;
	PL0E_LOCAL_W *lw = ( PL0E_LOCAL_W * )pl_w->tlocal ;

	if( pl_w->act_st == N_CHANGE || pl_w->routine_1 || pl_w->pl_sp_type != pl0e_sp_bite ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->prio = pl_w->prio ;
	ew->sub_prio = -8 ;
	shl0e_02_move_jmp[ew->routine_1]( ew, pl_w, lw ) ;
}

static void shl0e_02_hit_ck( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl0e_02_hit( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw )
{
	if( !( vs_sys_work.slow_bit & 1 << vs_sys_work.slow_step ) ){
		shl0e_02_hit_ck( ew, pl_w ) ;
		pl_char_move( ew ) ;
		if( ew->cg_flag1 ){
			ew->atck_no = ew->cg_flag1 ;
			HIT_RESET( ew ) ;
			ew->cg_flag1 = 0 ;
			ew->timer0 = 0 ;
		}
		if( ew->cg_type ){
			ew->routine_1 ++ ;
			if( !shl0e_02_set( ew, ew->type + 1, pl_w ) ) lw->bite_st = -1 ;
		}
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl0e_02_last( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw )
{
	if( !( vs_sys_work.slow_bit & 1 << vs_sys_work.slow_step ) ){
		shl0e_02_hit_ck( ew, pl_w ) ;
		if( pl_char_move( ew ) < 0 ){
			ew->routine_0 = 2 ;
			ew->disp_flag = 0 ;
			return ;
		}
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl0e_02_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw )
{
	if( !-- ew->timer1 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		lw->bite_st = 1 ;
	}
}

/*--------------------------------------------------------------*/
static void shl0e_02_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			A hit uses up one of Venom's hits					*/
/*==============================================================*/
static void shl0e_02_hit_ck( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0E_LOCAL_W *lw = ( PL0E_LOCAL_W * )pl_w->tlocal ;

	if( ew->hit_flag && ew->timer0 ){
		lw->bite_hit_cnt -- ;
		ew->timer0 = 1 ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl0e_02.c" End of File	*/

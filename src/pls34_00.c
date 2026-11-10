/* Sentinel's player effect 00: the animation that rides on him while his fist is out, six
   kinds (type): in the Rocket Punch (dir 0) or in a throw (dir 1), where it places the one held. */
#include "player.h"
#include "pl_char.h"
#include "pl34_def.h"

static void pls34_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls34_00_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls34_00_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->dir = 0 ;
		ew->ef_no = 0x3400 ;
	}
	return ew ;
}

/****************************************************************/
PLAYER_WORK *pls34_00_set2( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls34_00_main ;
		ew->master_w = pl_w ;
		ew->dir = 1 ;
		ew->type = no ;
		ew->ef_no = 0x3400 ;
	}
	return ew ;
}

static void pls34_00_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls34_00_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls34_00_next( PLAYER_WORK *ew );
static void pls34_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls34_00_main( PLAYER_WORK *ew )
{
	static void ( *pls34_00_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls34_00_init, pls34_00_move, pls34_00_next, pls34_00_free,
	} ;

	pls34_00_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void pls34_00_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	switch( ew->type ){
	case 0 : ew->sub_type = 6 ; break ;
	case 1 : ew->sub_type = 2 ; break ;
	case 2 : ew->sub_type = 10 ; break ;
	case 3 : ew->sub_type = 18 ; break ;
	case 4 : ew->sub_type = 16 ; break ;
	case 5 : ew->sub_type = 20 ; break ;
	}
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->sub_prio = 1 ;
	pl_char_set( ew, 21, ew->sub_type ) ;
	pls34_00_move( ew, pl_w ) ;
}

/*==============================================================*/
static void pls34_00_fist_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL34_LOCAL_W *lw = ( PL34_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *em ;

	if( ew->cg_type != pl_w->cg_type ) pl_char_set2( ew, 21, ew->sub_type, pl_w->cg_type ) ;
	if( ew->dir && ew->cg_flag1 && !lw->fist_end_flag ){
		em = pl_w->catch_w ;
		if( em->wall_flag ){
			pl_w->pos.x = ew->pos.x ;
			pl_w->pos.y = ew->pos.y ;
		} else {
			ew->pos.x = pl_w->pos.x ;
			ew->pos.y = pl_w->pos.y ;
		}
		catch_pos_set( ew, em ) ;
	} else {
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
	}
	if( lw->fist_end_flag ){
		ew->routine_1 ++ ;
		switch( ew->type ){
		case 0 : ew->sub_type = 7 ; break ;
		case 1 : ew->sub_type = 3 ; break ;
		case 2 : ew->sub_type = 11 ; break ;
		case 3 : ew->sub_type = 19 ; break ;
		case 4 : ew->sub_type = 17 ; break ;
		case 5 : ew->sub_type = 21 ; break ;
		}
		pl_char_set( ew, 21, ew->sub_type ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls34_00_fist_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	if( pl_w->cg_flag1 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( ew->cg_type != pl_w->cg_type ){
		if( pl_w->cg_type >= 11 ) pl_char_set2( ew, 21, ew->sub_type, 0 ) ;
		else pl_char_set2( ew, 21, ew->sub_type, pl_w->cg_type ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls34_00_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls34_00_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls34_00_fist_wait, pls34_00_fist_end,
	} ;

	ew->prio = pl_w->prio ;
	if( !ew->dir ){
		if( pl_w->routine_1 || pl_w->act_st != N_SPECIAL || pl_w->pl_sp_type ){
			ew->routine_0 ++ ;
			ew->disp_flag = 0 ;
			return ;
		}
	} else if( pl_w->routine_1 != PL_CATCH || pl_w->act_st != N_CATCH ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	pls34_00_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls34_00_next( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls34_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls34_00.c" End of File	*/

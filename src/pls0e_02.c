/* Venom's player effect 02: the line of his Death Bite. Type 0 is the mark at his feet
   (set by the move itself); type 1 is each piece of the line, which goes out from the one before it
   to the edge of the screen and then comes back. */
#include "player.h"
#include "pl_char.h"
#include "pl0e_def.h"

static void pls0e_02_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls0e_02_set( PLAYER_WORK *pl_w, int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->ef_no = 0xe02 ;
		ew->type = no ;
		ew->sub_type = 1 ;
		ew->work_func = pls0e_02_main ;
		ew->master_w = pl_w ;
		ew->ef_w = pl_w ;
	}
	return ew ;
}

static void pls0e_02_foot( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0e_02_line( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls0e_02_main( PLAYER_WORK *ew )
{
	static void ( *pls0e_02_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0e_02_foot, pls0e_02_line,
	} ;

	pls0e_02_jmp[ew->type]( ew, ew->master_w ) ;
}

static void pls0e_02_foot_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0e_02_foot_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0e_02_foot_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls0e_02_foot( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls0e_02_foot_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0e_02_foot_init, pls0e_02_foot_move, pls0e_02_foot_free,
	} ;

	pls0e_02_foot_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls0e_02_foot_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
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
	ew->prio = 12 ;
	pl_char_set( ew, Char_Effect, 19 ) ;
}

/*--------------------------------------------------------------*/
static void pls0e_02_foot_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0E_LOCAL_W *lw = ( PL0E_LOCAL_W * )pl_w->tlocal ;

	if( pl_w->act_st == N_CHANGE || pl_w->routine_1 || pl_w->pl_sp_type != 4 || lw->bite_st < 0 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->prio = 12 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->ground_y ;
}

/*--------------------------------------------------------------*/
static void pls0e_02_foot_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

static void pls0e_02_line_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0e_02_line_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0e_02_line_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls0e_02_line( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls0e_02_line_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0e_02_line_init, pls0e_02_line_move, pls0e_02_line_free,
	} ;

	pls0e_02_line_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls0e_02_line_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
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
	ew->prio = 12 ;
	ew->size_top = 16 ;
	ew->size_bottom = 16 ;
	ew->size_front = 16 ;
	ew->size_back = 16 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->acc.x = -DOT_X( 4.0f ) ;
	if( ew->disp_dir ) ew->acc.x = -ew->acc.x ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	pl_char_set( ew, Char_Effect, 19 ) ;
}

static void pls0e_02_out( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw );
static void pls0e_02_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw );
static void pls0e_02_back( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw );

/*--------------------------------------------------------------*/
static void pls0e_02_line_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls0e_02_line_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw ) = {
		pls0e_02_out, pls0e_02_hold, pls0e_02_back,
	} ;
	PL0E_LOCAL_W *lw = ( PL0E_LOCAL_W * )pl_w->tlocal ;

	if( pl_w->act_st == N_CHANGE || pl_w->routine_1 || pl_w->pl_sp_type != 4 || lw->bite_st < 0 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->prio = 12 ;
	pls0e_02_line_move_jmp[ew->routine_1]( ew, pl_w, lw ) ;
}

/*==============================================================*/
/*			Goes out; when it is 160 away it makes the next		*/
/*			piece												*/
/*==============================================================*/
static void pls0e_02_out( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw )
{
	PLAYER_WORK *w ;
	Float spd, ofs ;

	if( ( vs_sys_work.slow_bit & ( 1 << vs_sys_work.slow_step ) ) || !lw->bite_st ){
		w = ew->ef_w ;
		ew->pos.x = w->pos.x ;
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		spd = ew->spd.x ;
		if( spd < 0 ) spd = -spd ;
		spd -= 160.0f ;
		if( spd < 0 ) return ;
		ew->routine_1 ++ ;
		ofs = 160.0f ;
		if( !ew->disp_dir ){
			ofs = -160.0f ;
			spd = -spd ;
		}
		ew->pos.x = w->pos.x + ofs ;
		ew->spd.x = ofs ;
		ew->acc.x = 0 ;
		if( ew->timer0 < 7 ){
			if( ( w = work_get( ew, W_PLS, LIST_BEHIND ) ) ){
				w->ef_no = 0xe02 ;
				w->type = 1 ;
				w->dir = 0 ;
				w->spd.x = spd ;
				w->timer0 = ew->timer0 + 1 ;
				w->master_w = ew->master_w ;
				w->ef_w = ew ;
				w->work_func = pls0e_02_main ;
				ew->dir = 1 ;
				return ;
			}
		}
		else return ;
	}
	ew->routine_1 = 2 ;
	ew->acc.x = DOT_X( 16.0f ) ;
	if( ew->disp_dir ) ew->acc.x = -ew->acc.x ;
}

/*==============================================================*/
/*			Holds still until the move ends or it is off the	*/
/*			screen												*/
/*==============================================================*/
static void pls0e_02_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw )
{
	PLAYER_WORK *w ;

	if( vs_sys_work.slow_bit & ( 1 << vs_sys_work.slow_step ) ) return ;
	w = ew->ef_w ;
	ew->pos.x = w->pos.x + ew->spd.x ;
	if( !lw->bite_st && ew->sub_type ){
		if( scr_in_x_ck( ew ) ) return ;
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		w = ew->behind_w ;
		w->sub_type = 1 ;
		w->ef_w = pl_w ;
		return ;
	}
	ew->routine_1 ++ ;
	ew->acc.x = DOT_X( 16.0f ) ;
	if( ew->disp_dir ) ew->acc.x = -ew->acc.x ;
}

/*==============================================================*/
/*			Comes back to the one before it						*/
/*==============================================================*/
static void pls0e_02_back( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PL0E_LOCAL_W *lw )
{
	if( vs_sys_work.slow_bit & ( 1 << vs_sys_work.slow_step ) ) return ;
	pl_w = ew->ef_w ;
	ew->pos.x = pl_w->pos.x ;
	if( ew->dir ) ew->pos.x += ew->spd.x ;
	else {
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		if( ew->spd.x * ew->acc.x >= 0 ){
			ew->routine_0 = 2 ;
			ew->disp_flag = 0 ;
			ew = ew->ef_w ;
			if( !ew->work_kind ) lw->bite_st = -1 ;
			else ew->dir = 0 ;
		}
	}
}

/*--------------------------------------------------------------*/
static void pls0e_02_line_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls0e_02.c" End of File	*/

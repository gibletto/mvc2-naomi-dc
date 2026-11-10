/* Venom's player effect 01: the pieces of the web line of his Venom Rush, set in a row; they
   follow him out to a distance, hold while it lasts and come back. */
#include "player.h"
#include "pl_char.h"
#include "pl0e_def.h"

/* how fast the first piece goes out, by the attack's strength */
static Float pls0e_01_spd_tbl[] = { -DOT_X( 10.0f ), -DOT_X( 32.0f ) } ;

static void pls0e_01_main( PLAYER_WORK *ew );

/****************************************************************/
int pls0e_01_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	int i, n ;

	n = pl_w->atck_level * 4 + 1 ;
	if( work_rest_cnt <= n ) return 0 ;
	for( i = 0 ; i < n ; i ++ ){
		if( ( ew = work_get( 0, W_PLS, LIST_END ) ) ){
			ew->ef_no = 0xe01 ;
			ew->type = i ;
			ew->sub_type = n ;
			ew->work_func = pls0e_01_main ;
			ew->master_w = pl_w ;
		}
	}
	return 1 ;
}

static void pls0e_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0e_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0e_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls0e_01_main( PLAYER_WORK *ew )
{
	static void ( *pls0e_01_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0e_01_init, pls0e_01_move, pls0e_01_free,
	} ;

	pls0e_01_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void pls0e_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float spd ;

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
	ew->prio = 12 ;
	ew->pos = pl_w->pos ;
	ew->spd.x = 0 ;
	ew->spd.y = 0 ;
	ew->acc.x = 0 ;
	ew->acc.y = 0 ;
	if( !ew->type ){
		spd = pls0e_01_spd_tbl[pl_w->atck_level] ;
		if( ew->disp_dir ) spd = -spd ;
		ew->spd.x = spd ;
		ew->acc.x = spd ;
		ew->timer0 = 8 ;
	}
	else ew->routine_1 = 3 ;
	pl_char_set( ew, Char_Effect, 19 ) ;
}

static void pls0e_01_out( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0e_01_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0e_01_back( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0e_01_follow( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void pls0e_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls0e_01_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0e_01_out, pls0e_01_hold, pls0e_01_back, pls0e_01_follow,
	} ;

	if( pl_w->routine_1 || pl_w->act_st != N_SPECIAL || pl_w->pl_sp_type != 1 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->prio = 12 ;
	if( vs_sys_work.slow_bit & ( 1 << vs_sys_work.slow_step ) ) return ;
	pls0e_01_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
/*			Goes out, and sets the hit when it has gone as		*/
/*			far as it goes										*/
/*==============================================================*/
static void pls0e_01_out( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos = pl_w->pos ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	if( -- ew->timer0 == 0 ){
		ew->routine_1 ++ ;
		shl0e_00_set( ew, pl_w ) ;
	}
	ew->spd.y = ew->spd.x / ew->sub_type ;
}

/*==============================================================*/
/*			Holds until he lets go, then turns back				*/
/*==============================================================*/
static void pls0e_01_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0E_LOCAL_W *lw = ( PL0E_LOCAL_W * )pl_w->tlocal ;

	ew->pos = pl_w->pos ;
	ew->pos.x += ew->spd.x ;
	if( lw->rush_st ){
		ew->routine_1 ++ ;
		ew->acc.x = -ew->acc.x ;
		ew->timer0 = 8 ;
	}
	ew->spd.y = ew->spd.x / ew->sub_type ;
}

/*--------------------------------------------------------------*/
static void pls0e_01_back( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0E_LOCAL_W *lw = ( PL0E_LOCAL_W * )pl_w->tlocal ;

	ew->pos = pl_w->pos ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	if( -- ew->timer0 == 0 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		lw->rush_st = -1 ;
		return ;
	}
	ew->spd.y = ew->spd.x / ew->sub_type ;
}

/*==============================================================*/
/*			The pieces after the first take their line from		*/
/*			the one before										*/
/*==============================================================*/
static void pls0e_01_follow( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0E_LOCAL_W *lw = ( PL0E_LOCAL_W * )pl_w->tlocal ;

	if( lw->rush_st >= 0 ){
		pl_w = ew->before_w ;
		ew->spd.y = pl_w->spd.y ;
		ew->pos.x = pl_w->pos.x - ew->spd.y ;
	}
	else {
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void pls0e_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls0e_01.c" End of File	*/

/* Blackheart's player effect 01: the shadows that stand on the ground beside him in his entrance,
   a pair that appears together and runs a short animation. */
#include "player.h"
#include "pl_char.h"
#include "pl35_def.h"

static void pls35_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls35_01_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls35_01_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = 0 ;
		ew->sub_type = 0 ;
		ew->ef_no = 0x3501 ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
	}
	return ew ;
}

/****************************************************************/
PLAYER_WORK *pls35_01_set2( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls35_01_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = 0 ;
		ew->sub_type = 1 ;
		ew->ef_no = 0x3501 ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
	}
	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls35_01_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = 1 ;
		ew->sub_type = 1 ;
		ew->ef_no = 0x3501 ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
	}
	return ew ;
}

/*==============================================================*/
/*			A work of the same player as ew, of another type	*/
/*==============================================================*/
static PLAYER_WORK *pls35_01_clone( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls35_01_main ;
		ew->master_w = pl_w->master_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = type ;
		ew->sub_type = pl_w->sub_type ;
		ew->ef_no = 0x3501 ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
	}
	return ew ;
}

static void pls35_01_init( PLAYER_WORK *ew );
static void pls35_01_move( PLAYER_WORK *ew );
static void pls35_01_wait( PLAYER_WORK *ew );
static void pls35_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls35_01_main( PLAYER_WORK *ew )
{
	static void ( *pls35_01_jmp[] )( PLAYER_WORK *ew ) = {
		pls35_01_init, pls35_01_move, pls35_01_wait, pls35_01_free,
	} ;

	PLAYER_WORK *w = ew ;

	pls35_01_jmp[w->routine_0]( w ) ;
}

/*==============================================================*/
static void pls35_01_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
	pl_w = ew->master_w ;
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
	ew->pos.y = pl_w->ground_y ;
	ew->scale.x = 1.2f ;
	ew->scale.y = 1.2f ;
	if( !ew->type ){
		ew->timer0 = 16 ;
		ew->prio = 13 ;
	} else {
		ew->timer0 = 4 ;
		if( ew->type == 1 ) ew->prio = 11 ;
		else ew->prio = 9 ;
	}
	pl_char_set( ew, 18, ew->type + 12 ) ;
}

/*--------------------------------------------------------------*/
static void pls35_01_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PL35_LOCAL_W *lw = ( PL35_LOCAL_W * )&pl_w->tlocal[0] ;

	pl_char_move( ew ) ;
	if( !ew->type && ew->cg_type ){
		ew->cg_type = 0 ;
		pls35_01_clone( ew, 1 ) ;
		pls35_01_clone( ew, 2 ) ;
	}
	if( lw->land_flag ){
		ew->routine_0 ++ ;
		pls35_01_wait( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls35_01_wait( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	if( !-- ew->timer0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void pls35_01_free( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls35_01.c" End of File	*/

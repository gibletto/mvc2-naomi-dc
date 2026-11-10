/* Rogue's effect 03 (her pls0a): the hit that goes with an animation frame of hers, put
   out by her routines in her place: a spark of 32 frames facing one way or the other. */
#include "player.h"
#include "pl_char.h"

static void pls0a_03_main( PLAYER_WORK *ew );

/****************************************************************/
/*			A: the spark in the low seven bits, and the way		*/
/*			it faces in the top bit								*/
/****************************************************************/
PLAYER_WORK *pls0a_03_set( PLAYER_WORK *pl_w, int a, int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls0a_03_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0xa03 ;
		ew->pl_type = pl_w->pl_type ;
		ew->sub_type = a & 127 ;
		ew->dir = a & 128 ;
		ew->atck_no = no ;
	}
	return ew ;
}

static void pls0a_03_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls0a_03_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls0a_03_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls0a_03_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void pls0a_03_main( PLAYER_WORK *ew )
{
	static void ( *pls0a_03_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0a_03_init, pls0a_03_move, pls0a_03_end, pls0a_03_free,
	} ;

	pls0a_03_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void pls0a_03_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	int d ;
	Uint8 no ;

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
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	HIT_RESET( ew ) ;
	ew->pos = pl_w->pos ;
	d = -1 ;
	if( ew->sub_type & 1 ) d = 1 ;
	if( ew->dir ) d = -1 ;
	ew->timer0 = d ;
	ew->timer1 = 32 ;
	ew->sub_prio = ew->timer0 ;
	no = ew->sub_type + 3 ;
	if( ew->dir ) no += 4 ;
	pl_char_set( ew, Char_Effect, no ) ;
}

/*--------------------------------------------------------------*/
static void pls0a_03_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->prio = pl_w->prio ;
	if( !ew->dir ){
		if( pl_char_move( ew ) < 0 ) pls0a_03_free( ew, pl_w ) ;
		else hit_work_set( ew ) ;
	} else {
		pl_char_move( ew ) ;
		if( -- ew->timer1 <= 0 ){
			if( ew->sub_type <= 1 ){
				ew->routine_0 ++ ;
				pl_char_set( ew, Char_Effect, ew->sub_type + 11 ) ;
			}
			else pls0a_03_free( ew, pl_w ) ;
		}
		else hit_work_set( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls0a_03_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void pls0a_03_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls0a_03.c" End of File	*/

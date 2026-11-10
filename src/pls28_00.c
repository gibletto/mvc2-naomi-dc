/* Gambit's player effect 00: the sparks that fly from a player he hits with a flash of
   element 15; up to seven are out at once, and the last ones to end ring out with a sound. */
#include "player.h"
#include "pl_char.h"

static void pls28_00_main( PLAYER_WORK *ew );

/****************************************************************/
void pls28_00_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( pl_w->elem_ef_cnt < 7 ){
		if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
			ew->work_func = pls28_00_main ;
			ew->master_w = pl_w ;
			ew->type = no ;
			ew->ef_no = 0x2800 ;
			pl_w->elem_ef_cnt ++ ;
		}
	}
}

static void pls28_00_init( PLAYER_WORK *ew );
static void pls28_00_move( PLAYER_WORK *ew );
static void pls28_00_end( PLAYER_WORK *ew );
static void pls28_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls28_00_main( PLAYER_WORK *ew )
{
	static void ( *pls28_00_jmp[] )( PLAYER_WORK *ew ) = {
		pls28_00_init, pls28_00_move, pls28_00_end, pls28_00_free,
	} ;

	pls28_00_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls28_00_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
	pl_w = ew->master_w->dm_at_w ;
	if( pl_w->pl_type != PL_GAMBIT ){
		pls28_00_free( ew ) ;
		return ;
	}
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
	ew->ef_w = pl_w ;
	ew->disp_dir = pl_w->disp_dir ;
	ew->sub_prio = -1 ;
	pl_char_set( ew, Char_Effect, 7 ) ;
	pls28_00_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls28_00_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	int ang ;

	ew->prio = pl_w->prio ;
	pl_char_move( ew ) ;
	if( pl_w->routine_1 != PL_DAMAGE || pl_w->dm_no != 15 ){
		ew->routine_0 ++ ;
		ew->routine_1 = 0 ;
		ew->disp_flag = 0 ;
		if( -- pl_w->elem_ef_cnt < 0 ) pl_w->elem_ef_cnt = 0 ;
		if( pl_w->elem_ef_cnt < 3 ){
			if( pl_w->elem_ef_cnt ){ }
			burst_set( pl_w, &ew->pos, 1, 8 ) ;
			pl_se_req( ew, 73 ) ;
		}
		return ;
	}
	ew->pos = pl_w->pos ;
	if( !ew->routine_1 ){
		ew->routine_1 ++ ;
		ew->spd.y = pl_w->size_top * 2.142857f / 2.0f ;
		ew->acc.y = ew->spd.y / 2.0f ;
		ang = Rnd() << 8 ;
		ew->spd.x = ew->acc.y * nlCos( ang ) + ew->acc.y * nlSin( ang ) / 8.0f ;
		ew->spd.y += -( ew->acc.y * nlSin( ang ) ) + ew->acc.y * nlCos( ang ) ;
	}
	ew->pos.x += ew->spd.x ;
	ew->pos.y += ew->spd.y ;
}

/*--------------------------------------------------------------*/
static void pls28_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls28_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls28_00.c" End of File	*/

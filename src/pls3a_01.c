/* Servbot's player effect 01: the piece that shows him through Bull Mover: it takes his
   sprite and ground place and lifts him by the frame's height; when he is knocked out of the
   move it leaves a burst (burst_set). */
#include "player.h"

static void pls3a_01_main( PLAYER_WORK *ew );

/****************************************************************/
/*			The piece that shows him through Bull Mover			*/
/****************************************************************/
void *pls3a_01_set( PLAYER_WORK *pl_w, int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_END ) ) ){
		ew->work_func = pls3a_01_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x3a00 ;
		ew->type = no ;
	}
	return ew ;
}

static void pls3a_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls3a_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls3a_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls3a_01_main( PLAYER_WORK *ew )
{
	static void ( *pls3a_01_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls3a_01_init, pls3a_01_move, pls3a_01_free, pls3a_01_free,
	} ;

	pls3a_01_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void pls3a_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	pls3a_01_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls3a_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	VEC3 pos ;

	if( !pl_w->routine_1 ){
		if( pl_w->act_st != N_SPECIAL || pl_w->pl_sp_type != 9 ){
			pls3a_01_free( ew, pl_w ) ;
			return ;
		}
		ew->disp_flag = 1 ;
		ew->spr_no = pl_w->spr_no ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->ground_y ;
		ew->disp_dir = pl_w->disp_dir ;
		pl_w->pos.y = pl_w->ground_y + DOT_Y( pl_w->cg_flag1 ) ;
		return ;
	}
	pos.x = ew->pos.x ;
	pos.y = pl_w->ground_y + DOT_Y( 48.0f ) ;
	pos.z = ew->pos.z ;
	burst_set( ew, &pos, 1, 8 ) ;
	pls3a_01_free( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls3a_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls3a_01.c" End of File	*/

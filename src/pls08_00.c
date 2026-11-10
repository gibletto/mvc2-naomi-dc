/* Psylocke's player effect 00: a copy of her sprite that follows the screen's centre and, by
   type, carries her across it (type 0) or only stays in view (type 1) until its animation ends. */
#include "player.h"
#include "pl_char.h"
#include "pl08_def.h"

/* the speed of the carry, by his attack strength: x and y, and a spare */
const Float pls08_00_spd_tbl[4][3] = {
	{ -DOT_X( 128.0f ), DOT_Y( 128.0f ), 0.0f }, { DOT_X( 128.0f ), DOT_Y( 128.0f ), 0.0f },
	{ -DOT_X( 128.0f ), 0.0f, 0.0f }, { DOT_X( 128.0f ), 0.0f, 0.0f }
} ;

static void pls08_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls08_00_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls08_00_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->ef_no = 0x800 ;
	}
	return ew ;
}

static void pls08_00_init( PLAYER_WORK *ew );
static void pls08_00_move( PLAYER_WORK *ew );
static void pls08_00_end( PLAYER_WORK *ew );
static void pls08_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls08_00_main( PLAYER_WORK *ew )
{
	static void ( *pls08_00_jmp[] )( PLAYER_WORK *ew ) = {
		pls08_00_init, pls08_00_move, pls08_00_end, pls08_00_free,
	} ;

	pls08_00_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
/*			Its own attack strength, set by the one who made	*/
/*			it, is kept through the copy						*/
/*==============================================================*/
static void pls08_00_init( PLAYER_WORK *ew )
{
	Uint8 level ;
	const Float *spd ;

	ew->routine_0 ++ ;
	level = ew->atck_level ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->disp_flag = 1 ;
	ew->atck_level = level ;
	ew->prio = 7 ;
	spd = pls08_00_spd_tbl[ew->atck_level] ;
	ew->spd.x = spd[0] ;
	ew->spd.y = spd[1] ;
	ew->disp_dir = ew->master_w->disp_dir ;
	pl_char_set( ew, 21, 45 ) ;
	pls08_00_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls08_00_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	PL08_LOCAL_W *lw ;

	pl_w = ew->master_w ;
	if( ew->type && ( pl_w->routine_1 || pl_w->act_st != N_SUPER ) ){
		ew->disp_flag = 0 ;
		ew->routine_0 = 3 ;
		return ;
	}
	ew->pos.x = scr_work.eye[0].x + ew->spd.x ;
	ew->pos.y = ew->spd.y ;
	pl_w->pos.x = ew->pos.x ;
	pl_w->pos.y = ew->pos.y ;
	if( pl_char_move( ew ) < 0 ){
		ew->disp_flag = 0 ;
		ew->routine_0 = 3 ;
	}
	else if( ew->cg_type ){
		ew->routine_0 ++ ;
		ew->cg_type = 0 ;
		lw = ( PL08_LOCAL_W * )pl_w->tlocal ;
		lw->warp_flag = 1 ;
		pl_w->disp_dir = ew->disp_dir ;
		pl_w->pl_dir = ew->disp_dir ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
}

/*--------------------------------------------------------------*/
static void pls08_00_end( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ){
		ew->disp_flag = 0 ;
		ew->routine_0 = 3 ;
	}
}

/*--------------------------------------------------------------*/
static void pls08_00_free( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls08_00.c" End of File	*/

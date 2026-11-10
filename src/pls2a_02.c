/* Storm's player effect 02: a copy of her sprite that follows her while she is drawn and
   plays the animation of group 27 that her frame code asks for (codes 16 and up). */
#include "player.h"
#include "ef_char.h"
#include "pl2a_def.h"

static Uint8 pls2a_02_tbl[] = {
	1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
} ;

static void pls2a_02_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls2a_02_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls2a_02_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x2a02 ;
	}
	return ew ;
}

static void pls2a_02_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2a_02_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2a_02_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2a_02_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2a_02_main( PLAYER_WORK *ew )
{
	static void ( *pls2a_02_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2a_02_init, pls2a_02_move, pls2a_02_wait, pls2a_02_end,
	} ;

	pls2a_02_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void pls2a_02_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->disp_flag = 0 ;
	ew->sub_prio = -1 ;
	ew->pos = pl_w->pos ;
	ew->obj_flag = -1 ;
	PL2A_LW( pl_w )->last_code = -1 ;
	pls2a_02_move( ew, pl_w ) ;
}

/*==============================================================*/
/*			Follows the player: group 27 animation for his		*/
/*			frame codes 16 and up, set again when the code		*/
/*			changes												*/
/*==============================================================*/
static void pls2a_02_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Uint8 no ;

	if( !pl_w->disp_flag ) return ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->disp_dir = pl_w->disp_dir ;
	ew->prio = pl_w->prio ;
	no = HI_BYTE( pl_w->cg_rival ) ;
	if( ew->disp_flag && ew->obj_flag == pl_w->char_no.w ){
		if( no < 16 ) goto move ;
	}
	else if( no < 16 ) goto kill ;
	ew->disp_flag = 1 ;
	if( ( Uint8 )PL2A_LW( pl_w )->last_code != no || ew->obj_flag != pl_w->char_no.w ){
		ew->obj_flag = pl_w->char_no.w ;
		ef_char_set( ew, 27, pls2a_02_tbl[no - 16] ) ;
	} else {
move:
		if( pl_w->hit_stop && !( pl_w->cg_prm & 0x80 ) ) return ;
		if( ef_char_move( ew ) < 0 && HI_BYTE( pl_w->cg_rival ) != 39 ) goto kill ;
	}
	return ;
kill:
	ew->obj_flag = -1 ;
	ew->disp_flag = 0 ;
	PL2A_LW( pl_w )->last_code = -1 ;
}

/*--------------------------------------------------------------*/
static void pls2a_02_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
}

/*--------------------------------------------------------------*/
static void pls2a_02_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls2a_02.c" End of File	*/

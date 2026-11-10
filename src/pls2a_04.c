/* Storm's player effect 04: a copy of her sprite that stands at an offset from her (by her
   frame code) and blinks, while her animation is the one it began with. */
#include "player.h"
#include "pl_char.h"

static void pls2a_04_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls2a_04_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls2a_04_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x2a04 ;
		ew->dir = pl_w->dir ;
		ew->obj_flag = pl_w->char_no.b.grp ;
		ew->work1 = pl_w->char_no.b.no ;
	}
	return ew ;
}

static void pls2a_04_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2a_04_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2a_04_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2a_04_main( PLAYER_WORK *ew )
{
	static void ( *pls2a_04_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2a_04_init, pls2a_04_move, pls2a_04_end, pls2a_04_end,
	} ;

	pls2a_04_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

static void pls2a_04_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2a_04_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->timer0 = 0 ;
	pls2a_04_pos_set( ew, pl_w ) ;
	pl_char_set( ew, 11, 8 ) ;
}

/*==============================================================*/
/*			Stays with him, blinking, while he shows the		*/
/*			frame it was made for; ends when that changes		*/
/*==============================================================*/
static void pls2a_04_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ew->pl_type != pl_w->pl_type ) goto end ;
	if( ew->obj_flag != pl_w->char_no.b.grp ) goto end ;
	if( ew->work1 != pl_w->char_no.b.no ) goto end ;
	ew->prio = pl_w->prio ;
	pls2a_04_pos_set( ew, pl_w ) ;
	pl_char_move( ew ) ;
	ew->disp_flag ^= 1 ;
	return ;
end:
	pls2a_04_end( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls2a_04_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
static void pls2a_04_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Sint16 pls2a_04_ofs_tbl[][2] = {
		{ -12, 132 }, { 46, 112 }, { 66, 87 }, { 58, 57 },
		{ -11, 20 }, { -53, 57 }, { -56, 88 }, { -45, 114 },
	} ;
	Sint16 dx = pls2a_04_ofs_tbl[pl_w->cg_prm][0] ;
	Sint16 dy = pls2a_04_ofs_tbl[pl_w->cg_prm][1] ;

	if( pl_w->disp_dir ) dx = -dx ;
	ew->pos.x = pl_w->pos.x + DOT_X( dx ) ;
	ew->pos.y = pl_w->pos.y + DOT_Y( dy ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls2a_04.c" End of File	*/

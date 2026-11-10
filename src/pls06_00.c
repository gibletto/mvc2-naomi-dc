/* Cyclops's player effect 00: a mark that rides on the player (type 0, which follows the
   animation's code to move by the table and makes a trail of copies of itself) and the copies
   (type 1), which play out their animation and are freed. */
#include "player.h"
#include "ef_char.h"

/* how far the mark moves from the player for each code of his animation */
static struct { Sint16 x, y ; } pls06_00_ofs_tbl[] = {
	{ 0, 0 }, { 8, 100 }, { 8, 101 }, { 29, 99 },
	{ 27, 101 }, { 27, 102 }, { 27, 100 }, { 29, 98 },
	{ 27, 97 }, { 27, 96 }, { 3, 97 }, { 3, 98 },
	{ 3, 99 }, { 3, 100 }, { 6, 60 }, { 6, 61 },
	{ 6, 62 }, { 8, 58 }, { 8, 56 }, { 8, 57 },
	{ 7, 59 }, { 16, 80 }, { 9, 71 }, { 2, 53 },
	{ 6, 60 }, { 18, 52 }, { 22, 70 }, { 7, 104 },
	{ 8, 101 },
} ;

static void pls06_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls06_00_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls06_00_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
		ew->ef_no = 0x600 ;
	}
	return ew ;
}

/*==============================================================*/
/*			A copy of the mark, at the same place				*/
/*==============================================================*/
static PLAYER_WORK *pls06_00_copy_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( pl_w, W_PLS, LIST_BEHIND ) ) ){
		ew->work_func = pls06_00_main ;
		ew->master_w = pl_w->master_w ;
		ew->ef_w = pl_w ;
		ew->pos = pl_w->pos ;
		ew->type = type ;
		ew->ef_no = 0x600 ;
	}
	return ew ;
}

static void pls06_00_init( PLAYER_WORK *ew );
static void pls06_00_move( PLAYER_WORK *ew );
static void pls06_00_end( PLAYER_WORK *ew );
static void pls06_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls06_00_main( PLAYER_WORK *ew )
{
	static void ( *pls06_00_jmp[] )( PLAYER_WORK *ew ) = {
		pls06_00_init, pls06_00_move, pls06_00_end, pls06_00_free,
	} ;

	pls06_00_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls06_00_init( PLAYER_WORK *ew )
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
	ew->timer0 = 0 ;
	if( !ew->type ) ew->char_no.b.no = 0 ;
	else ew->char_no.b.no = 1 ;
	ew->sub_prio = -1 ;
	ef_char_set( ew, 27, ew->char_no.b.no ) ;
	pls06_00_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls06_00_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Float ofs[2] ;	/* only ofs[0] is used */

	ew->disp_flag = 0 ;
	ew->prio = pl_w->prio ;
	if( !ew->type ){
		ew->pos = pl_w->pos ;
		ew->disp_flag = 0 ;
		if( !pl_w->disp_flag || !HI_BYTE( pl_w->cg_rival ) ) return ;
		ofs[0] = DOT_X( pls06_00_ofs_tbl[HI_BYTE( pl_w->cg_rival )].x ) ;
		if( !pl_w->disp_dir ) ofs[0] = -ofs[0] ;
		ew->pos.x += ofs[0] ;
		ew->pos.y += DOT_Y( pls06_00_ofs_tbl[HI_BYTE( pl_w->cg_rival )].y ) ;
		if( -- ew->timer0 < 0 ){
			ew->timer0 = 3 ;
			pls06_00_copy_set( ew, 1 ) ;
		}
		ef_char_move( ew ) ;
		ew->disp_flag = Sys->game_timer0 & 1 ;
	}
	else if( ef_char_move( ew ) < 0 ) ew->routine_0 ++ ;
	else ew->disp_flag = Sys->game_timer0 & 1 ;
}

/*--------------------------------------------------------------*/
static void pls06_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls06_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls06_00.c" End of File	*/

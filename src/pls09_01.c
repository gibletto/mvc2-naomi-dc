/* Iceman's player effect 01: the four sparks that fly out at the corners of his body. */
#include "player.h"
#include "pl_char.h"

/* where each spark sits from his position, in 1/256 */
static Sint16 pls09_01_ofs_tbl[8] = { 14, -6, 13, -1, -23, -7, -24, -2 } ;

static void pls09_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls09_01_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls09_01_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->ef_no = 0x901 ;
	}
	return ew ;
}

static void pls09_01_init( PLAYER_WORK *ew );
static void pls09_01_move( PLAYER_WORK *ew );
static void pls09_01_end( PLAYER_WORK *ew );
static void pls09_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls09_01_main( PLAYER_WORK *ew )
{
	static void ( *pls09_01_jmp[] )( PLAYER_WORK *ew ) = {
		pls09_01_init, pls09_01_move, pls09_01_end, pls09_01_free,
	} ;

	pls09_01_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls09_01_init( PLAYER_WORK *ew )
{
	Float x ;

	ew->routine_0 ++ ;
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
	ew->prio = 7 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	x = DOT_X( pls09_01_ofs_tbl[ew->type * 2] ) ;
	if( ew->master_w->disp_dir ) x = -x ;
	ew->pos.x += x ;
	ew->pos.y += DOT_Y( pls09_01_ofs_tbl[ew->type * 2 + 1] ) ;
	if( ew->type & 1 ) pl_char_set( ew, 20, 1 ) ;
	else pl_char_set( ew, 20, 0 ) ;
	pls09_01_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls09_01_move( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		pls09_01_end( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls09_01_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	pls09_01_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls09_01_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls09_01.c" End of File	*/

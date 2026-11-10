/* Tron Bonne's player effect 07: the six pieces that fly off the work it rides on (shl10_02),
   from places turned with its angle, and blink out. */
#include "player.h"
#include "ef_char.h"

/* where each piece starts against the work, its draw priority and animation */
typedef struct PLS10_07_TBL {
	Sint16	x ;
	Sint16	y ;
	Sint8	prio ;
	Uint8	no ;
} PLS10_07_TBL ;

static PLS10_07_TBL pls10_07_tbl[6] = {
	{ -24, 13, 11, 31 }, { 44, 0, 11, 32 }, { 16, 18, 11, 33 },
	{ -32, -12, 11, 28 }, { -4, -10, 11, 29 }, { 28, -12, 11, 30 },
} ;

static void pls10_07_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls10_07_set( PLAYER_WORK *pl_w, PLAYER_WORK *em, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls10_07_main ;
		ew->master_w = pl_w ;
		ew->ef_w = em ;
		ew->type = type ;
	}
	return ew ;
}

static void pls10_07_init( PLAYER_WORK *ew );
static void pls10_07_move( PLAYER_WORK *ew );
static void pls10_07_end( PLAYER_WORK *ew );
static void pls10_07_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls10_07_main( PLAYER_WORK *ew )
{
	static void ( *pls10_07_jmp[] )( PLAYER_WORK *ew ) = {
		pls10_07_init, pls10_07_move, pls10_07_end, pls10_07_free,
	} ;

	pls10_07_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls10_07_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	PLAYER_WORK *em ;
	PLS10_07_TBL *tbl ;
	VEC2 ofs_buf ;
	VEC2 *ofs ;

	pl_w = ew->master_w ;
	em = ew->ef_w ;
	ew->routine_0 ++ ;
	ew->ef_no = 0x1007 ;
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
	ew->timer0 = 30 ;
	tbl = &pls10_07_tbl[ew->type] ;
	ew->prio = tbl->prio ;
	ofs = &ofs_buf ;
	ofs->x = DOT_X( tbl->x ) ;
	ofs->y = DOT_Y( tbl->y ) ;
	ew->rot[2] = em->rot[2] ;
	/* ofs is reached through a pointer */
	if( !( ew->disp_dir = em->disp_dir ) ) ew->pos.x = em->pos.x + ofs->x * nlCos( ew->rot[2] ) - ofs->y * nlSin( ew->rot[2] ) ;
	else ew->pos.x = em->pos.x - ofs->x * nlCos( ew->rot[2] ) + ofs->y * nlSin( ew->rot[2] ) ;
	ew->pos.y = em->pos.y + ofs->x * nlSin( ew->rot[2] ) + ofs->y * nlCos( ew->rot[2] ) ;
	ew->spd.x = ( ew->pos.x - em->pos.x ) / 32.0f ;
	ew->acc.x = 0 ;
	ew->spd.y = DOT_Y( 4.0f ) ;
	ew->acc.y = -DOT_Y( 0.375f ) ;
	ef_char_set( ew, 27, tbl->no ) ;
	pls10_07_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls10_07_move( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( -- ew->timer0 == 0 ) ew->routine_0 ++ ;
	if( ew->timer0 <= 10 ) ew->disp_flag = ew->timer0 & 1 ;
}

/*--------------------------------------------------------------*/
static void pls10_07_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls10_07_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls10_07.c" End of File	*/

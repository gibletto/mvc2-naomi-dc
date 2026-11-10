/* Abyss's (character 18) player effects of the second kind, 26 by type: a picture of him that comes in
   beside him, rushes to him and sets off the marks of pls18_00 on him. */
#include "player.h"
#include "pl_char.h"
#include "pl18_def.h"

/* a kind's place beside its master (the x and y in 3/5 pixels), its order among the works and its animation */
typedef struct PLS18_01_TBL {
	Sint16	x;
	Sint16	y;
	Sint8	prio;
	Uint8	no;
} PLS18_01_TBL;

static PLS18_01_TBL pls18_01_tbl[] = {
	{ -64, 16, -1, 16 }, { 64, 16, 1, 17 }, { -64, 0, -3, 18 }, { 64, 0, 3, 19 },
	{ 64, -16, 2, 20 }, { -64, -16, -3, 21 }, { -64, 0, -1, 22 }, { 64, 0, 1, 23 },
	{ -64, 0, -2, 24 }, { 64, 0, 2, 25 }, { -64, 0, -1, 26 }, { 64, 0, 1, 27 },
	{ -64, 0, -2, 28 }, { 64, 0, 2, 29 }, { -64, 0, -1, 30 }, { 64, 0, 1, 31 },
	{ -64, 0, -1, 32 }, { 64, 0, 1, 33 }, { -64, 0, 0, 34 }, { 64, 0, 1, 35 },
	{ 64, -32, -4, 36 }, { -64, -32, 4, 37 }, { -64, 32, 3, 38 }, { 64, 32, -3, 39 },
	{ 64, 0, -3, 40 }, { 64, -16, -3, 41 },
} ;

/* the frames each kind waits before it shows */
static Uint8 pls18_01_timer_tbl[] = {
	49, 49, 65, 65, 59, 59, 10, 10, 41, 41, 14, 14, 36, 36, 27, 27, 24, 24, 100, 100, 81, 76, 96, 90, 65, 59,
} ;

/* the kind of pls18_00 each kind sets off on arrival, and its sub_no (none where 0) */
static Uint8 pls18_01_mark_tbl[][2] = {
	{ 20, 0 }, { 0, 0 }, { 20, 1 }, { 0, 0 }, { 20, 2 }, { 0, 0 }, { 20, 3 }, { 0, 0 }, { 20, 4 }, { 0, 0 }, { 20, 5 }, { 0, 0 }, { 20, 6 }, { 0, 0 }, { 20, 7 }, { 0, 0 }, { 20, 8 }, { 0, 0 }, { 20, 9 }, { 0, 0 }, { 15, 10 }, { 15, 11 }, { 15, 12 }, { 15, 13 }, { 0, 0 }, { 0, 0 },
} ;

static void pls18_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls18_01_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls18_01_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
	}
	return ew ;
}

static void pls18_01_init( PLAYER_WORK *ew );
static void pls18_01_type( PLAYER_WORK *ew );
static void pls18_01_end( PLAYER_WORK *ew );
static void pls18_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls18_01_main( PLAYER_WORK *ew )
{
	static void ( *pls18_01_jmp[] )( PLAYER_WORK *ew ) = {
		pls18_01_init, pls18_01_type, pls18_01_end, pls18_01_free,
	} ;

	pls18_01_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls18_01_init( PLAYER_WORK *ew )
{
	PLS18_01_TBL *tbl ;
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->routine_0 ++ ;
	ew->ef_no = 0x1801 ;
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
	ew->alpha = 0.0f ;
	ew->timer0 = pls18_01_timer_tbl[ew->type] ;
	tbl = &pls18_01_tbl[ew->type] ;
	ew->prio = 7 ;
	ew->sub_prio = tbl->prio ;
	ew->pos.x = pl_w->pos.x + DOT_X( tbl->x ) ;
	ew->pos.y = pl_w->pos.y + DOT_Y( tbl->y ) ;
	ef_char_set2( ew, 27, tbl->no, 1 ) ;
	pls18_01_type( ew ) ;
}

static void pls18_01_wait( PLAYER_WORK *ew );
static void pls18_01_move( PLAYER_WORK *ew );
static void pls18_01_nop( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls18_01_type( PLAYER_WORK *ew )
{
	static void ( *pls18_01_type_jmp[] )( PLAYER_WORK *ew ) = {
		pls18_01_wait, pls18_01_move, pls18_01_nop,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_0 > 0 || pl_w->routine_2 > 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	pls18_01_type_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void pls18_01_wait( PLAYER_WORK *ew )
{
	if( --ew->timer0 == 0 ){
		ew->routine_1 ++ ;
		ew->disp_flag = 1 ;
		ew->timer0 = 10 ;
		if( ( 1 << ew->type ) & 0x3F55555 ) pl_voice_req( ew, 20 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls18_01_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *mw ;

	ew->alpha += 0.2f ;
	if( ew->alpha >= 1.0f ) ew->alpha = 1.0f ;
	ew->pos.x += ( pl_w->pos.x - ew->pos.x ) / 2.0f ;
	ew->pos.y += ( pl_w->pos.y - ew->pos.y ) / 2.0f ;
	if( --ew->timer0 == 0 ){
		ew->routine_1 ++ ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		if( ( mw = pls18_00_set( pl_w, 14 ) ) ) mw->ef_w = ew ;
		if( pls18_01_mark_tbl[ew->type][0] ){
			if( ( mw = pls18_00_set( pl_w, pls18_01_mark_tbl[ew->type][0] ) ) ){
				mw->ef_w = ew ;
				mw->sub_no = pls18_01_mark_tbl[ew->type][1] ;
			}
		}
	}
}

/*--------------------------------------------------------------*/
static void pls18_01_nop( PLAYER_WORK *ew )
{
}

/*--------------------------------------------------------------*/
static void pls18_01_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls18_01_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls18_01.c" End of File	*/

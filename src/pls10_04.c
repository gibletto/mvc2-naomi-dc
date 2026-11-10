/* Tron Bonne's player effect 04: the ten marks that run in from the edge of the screen, one
   after another, while she is in a win pose, and stop at their places. */
#include "player.h"
#include "pl_char.h"

/* each mark's draw priority, its speed, the frames it waits and the frames it runs */
typedef struct PLS10_04_TBL {
	Sint8	prio ;
	Uint8	unk_01[3] ;
	Float	spd_x ;
	Sint16	wait ;
	Sint16	run ;
} PLS10_04_TBL ;

static PLS10_04_TBL pls10_04_tbl[10] = {
	{ 0, { 0, 0, 0 }, -DOT_X( 4.0f ), 0, 92 },
	{ 0, { 0, 0, 0 }, -DOT_X( 4.0f ), 8, 96 },
	{ 0, { 0, 0, 0 }, -DOT_X( 4.0f ), 12, 88 },
	{ 0, { 0, 0, 0 }, -DOT_X( 4.0f ), 20, 100 },
	{ 0, { 0, 0, 0 }, -DOT_X( 4.0f ), 25, 94 },
	{ 0, { 0, 0, 0 }, -DOT_X( 4.0f ), 27, 98 },
	{ 0, { 0, 0, 0 }, -DOT_X( 4.0f ), 32, 90 },
	{ 0, { 0, 0, 0 }, -DOT_X( 4.0f ), 36, 84 },
	{ 0, { 0, 0, 0 }, -DOT_X( 4.0f ), 42, 86 },
	{ 0, { 0, 0, 0 }, -DOT_X( 4.0f ), 50, 104 },
} ;

static void pls10_04_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls10_04_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls10_04_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
	}
	return ew ;
}

static void pls10_04_init( PLAYER_WORK *ew );
static void pls10_04_move( PLAYER_WORK *ew );
static void pls10_04_end( PLAYER_WORK *ew );
static void pls10_04_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls10_04_main( PLAYER_WORK *ew )
{
	static void ( *pls10_04_jmp[] )( PLAYER_WORK *ew ) = {
		pls10_04_init, pls10_04_move, pls10_04_end, pls10_04_free,
	} ;

	pls10_04_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls10_04_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	PLS10_04_TBL *tbl ;

	pl_w = ew->master_w ;
	ew->routine_0 ++ ;
	ew->ef_no = 0x1004 ;
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
	ew->disp_dir ^= 1 ;
	tbl = &pls10_04_tbl[ew->type] ;
	ew->prio = tbl->prio ;
	ew->timer0 = tbl->wait ;
	ew->timer1 = tbl->run ;
	if( !ew->disp_dir ) ew->pos.x = scr_work.scr_right + DOT_X( 32.0f ) ;
	else ew->pos.x = scr_work.scr_left + -DOT_X( 32.0f ) ;
	ew->pos.y = pl_w->ground_y ;
	ew->spd.x = tbl->spd_x ;
	if( ew->disp_dir ) ew->spd.x = -ew->spd.x ;
	ew->acc.x = 0 ;
	ew->spd.y = 0 ;
	ew->acc.y = 0 ;
	pl_char_set( ew, Char_Effect, 20 ) ;
	pls10_04_move( ew ) ;
}

static void pls10_04_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls10_04_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls10_04_stop( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void pls10_04_move( PLAYER_WORK *ew )
{
	static void ( *pls10_04_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls10_04_wait, pls10_04_run, pls10_04_stop,
	} ;

	pls10_04_move_jmp[ew->routine_1]( ew, ew->master_w ) ;
}

/*==============================================================*/
/*			Waits its turn, and goes if she stops her act		*/
/*==============================================================*/
static void pls10_04_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->act_st != N_WIN ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( -- ew->timer0 == 0 ){
		ew->routine_1 ++ ;
		ew->disp_flag = 1 ;
	}
}

/*--------------------------------------------------------------*/
static void pls10_04_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	if( -- ew->timer1 == 0 ){
		ew->routine_1 ++ ;
		pl_char_set( ew, Char_Effect, 21 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls10_04_stop( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls10_04_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls10_04_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls10_04.c" End of File	*/

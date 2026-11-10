/* Spiral's player effect 00: pieces that stand by her while her animation lasts: her entrance's,
   which starts, waits and plays on (0); the win pose's (1) and Metamorphose's (2) follow her. */
#include "player.h"
#include "pl_char.h"
#include "pl31_def.h"

/* the effect's own work, from obj_flag */
typedef struct PLS31_00_LOCAL_W {
	Uint16	char_no;	/* 00: her animation when it was made; the piece ends when it changes */
} PLS31_00_LOCAL_W;
#define PLS31_00_LW( w )	( ( PLS31_00_LOCAL_W * )&( w )->obj_flag )

static void pls31_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls31_00_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 kind )
{
	PLAYER_WORK *ew ;
	PLS31_00_LOCAL_W *lw ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls31_00_main ;
		ew->ef_no = 0x3100 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = type ;
		ew->sub_type = kind ;
		lw = PLS31_00_LW( ew ) ;
		lw->char_no = pl_w->char_no.w ;
	}
	return ew ;
}

/****************************************************************/
PLAYER_WORK *pls31_00_set2( PLAYER_WORK *pl_w, Uint8 type, Uint8 kind )
{
	PLAYER_WORK *ew ;
	PLS31_00_LOCAL_W *lw ;

	if( ( ew = work_get( 0, W_PLS_ST, LIST_TOP ) ) ){
		ew->work_func = pls31_00_main ;
		ew->ef_no = 0x3100 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = type ;
		ew->sub_type = kind ;
		lw = PLS31_00_LW( ew ) ;
		lw->char_no = pl_w->char_no.w ;
	}
	return ew ;
}

static void pls31_00_appear( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls31_00_win( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls31_00_metamor( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls31_00_main( PLAYER_WORK *ew )
{
	static void ( *pls31_00_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls31_00_appear, pls31_00_win, pls31_00_metamor,
	} ;

	pls31_00_jmp[ew->type]( ew, ew->master_w ) ;
}

static void pls31_00_appear_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls31_00_appear_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls31_00_next( PLAYER_WORK *ew );
static void pls31_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls31_00_appear_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	if( !ew->sub_type ){
		ew->sub_prio = -1 ;
	} else {
		ew->sub_prio = 1 ;
	}
	if( ew->sub_type ) ew->disp_flag = 0 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->ground_y ;
	ew->pos.z = pl_w->pos.z ;
	pl_char_set( ew, Char_Effect, 7 ) ;
	pls31_00_appear_move( ew, pl_w ) ;
}

/*==============================================================*/
static void pls31_00_appear_start( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		ew->disp_flag = 1 ;
		if( !ew->sub_type ){
			ew->char_no.b.no = 8 ;
		} else {
			ew->char_no.b.no = 9 ;
		}
		pl_char_set( ew, Char_Effect, ew->char_no.b.no ) ;
		ew->timer0 = 90 ;
	}
}

/*--------------------------------------------------------------*/
static void pls31_00_appear_wait( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	if( !ew->timer0 -- ){
		ew->routine_1 ++ ;
		if( ew->sub_type ) ew->disp_flag = 0 ;
		pl_char_set( ew, Char_Effect, 10 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls31_00_appear_end( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void pls31_00_appear_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls31_00_appear_jmp[] )( PLAYER_WORK *ew ) = {
		pls31_00_appear_start, pls31_00_appear_wait, pls31_00_appear_end,
	} ;

	ew->prio = pl_w->prio ;
	pls31_00_appear_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void pls31_00_appear( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls31_00_appear_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls31_00_appear_init, pls31_00_appear_move, pls31_00_next, pls31_00_free,
	} ;

	pls31_00_appear_jmp[ew->routine_0]( ew, pl_w ) ;
}

static void pls31_00_win_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls31_00_win_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls31_00_win_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->sub_prio = -1 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y + DOT_Y( 86.0f ) ;
	ew->pos.z = pl_w->pos.z ;
	ew->spd.x = -DOT_X( 7.0f ) ;
	if( ew->disp_dir ) ew->spd.x = -ew->spd.x ;
	ew->pos.x += ew->spd.x ;
	pl_char_set( ew, Char_Effect, 6 ) ;
	pls31_00_win_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls31_00_win_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS31_00_LOCAL_W *lw = PLS31_00_LW( ew ) ;

	if( pl_w->char_no.w == lw->char_no ){
		ew->pos.x = pl_w->pos.x + ew->spd.x ;
		ew->pos.y = pl_w->pos.y + ew->spd.y ;
		ew->prio = pl_w->prio ;
		if( pl_char_move( ew ) >= 0 ) return ;
	}
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*==============================================================*/
static void pls31_00_win( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls31_00_win_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls31_00_win_init, pls31_00_win_move, pls31_00_next, pls31_00_free,
	} ;

	pls31_00_win_jmp[ew->routine_0]( ew, pl_w ) ;
}

static void pls31_00_metamor_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls31_00_metamor_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls31_00_metamor_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->sub_prio = 1 ;
	ew->pos.z = pl_w->pos.z ;
	pl_char_set( ew, Char_Effect, 5 ) ;
	ew->timer0 = 16 ;
	pls31_00_metamor_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls31_00_metamor_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS31_00_LOCAL_W *lw = PLS31_00_LW( ew ) ;

	if( pl_w->char_no.w != lw->char_no ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->prio = pl_w->prio ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	pl_char_move( ew ) ;
	if( ew->timer0 -- ) return ;
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*==============================================================*/
static void pls31_00_metamor( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls31_00_metamor_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls31_00_metamor_init, pls31_00_metamor_move, pls31_00_next, pls31_00_free,
	} ;

	pls31_00_metamor_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls31_00_next( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls31_00_free( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls31_00.c" End of File	*/

/* Blackheart's player effect 00: a copy of his sprite that follows him, shows the animation his
   frames ask for, and goes when it ends. Four types, set by his throws. */
#include "player.h"
#include "pl_char.h"
#include "pl35_def.h"

static void pls35_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls35_00_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls35_00_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = type ;
		ew->ef_no = 0x3500 ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		ew->timer0 = pl_w->char_no.w ;
	}
	return ew ;
}

static void pls35_00_init( PLAYER_WORK *ew );
static void pls35_00_move( PLAYER_WORK *ew );
static void pls35_00_end( PLAYER_WORK *ew );
static void pls35_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls35_00_main( PLAYER_WORK *ew )
{
	static void ( *pls35_00_jmp[] )( PLAYER_WORK *ew ) = {
		pls35_00_init, pls35_00_move, pls35_00_end, pls35_00_free,
	} ;
	PLAYER_WORK *w = ew ;

	pls35_00_jmp[w->routine_0]( w ) ;
}

static void pls35_00_p_catch_init( PLAYER_WORK *ew );
static void pls35_00_k_catch_init( PLAYER_WORK *ew );
static void pls35_00_k_throw_init( PLAYER_WORK *ew );
static void pls35_00_j_throw_init( PLAYER_WORK *ew );

/*==============================================================*/
static void pls35_00_init( PLAYER_WORK *ew )
{
	static void ( *pls35_00_init_jmp[] )( PLAYER_WORK *ew ) = {
		pls35_00_p_catch_init, pls35_00_k_catch_init, pls35_00_k_throw_init, pls35_00_j_throw_init,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

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
	pls35_00_init_jmp[ew->type]( ew ) ;
}

/*==============================================================*/
static void pls35_00_p_catch_init( PLAYER_WORK *ew )
{
	ew->prio = 11 ;
	pl_char_set( ew, Char_Effect, 3 ) ;
}

/*==============================================================*/
static void pls35_00_k_catch_init( PLAYER_WORK *ew )
{
	ew->prio = 11 ;
	ew->dir = 0 ;
	pl_char_set( ew, Char_Effect, 7 ) ;
}

/*==============================================================*/
static void pls35_00_k_throw_init( PLAYER_WORK *ew )
{
	Float x ;

	ew->prio = 11 ;
	x = DOT_X( 142.0f ) ;
	if( !ew->disp_dir ) x = -DOT_X( 142.0f ) ;
	ew->pos.x += x ;
	ew->pos.y += DOT_Y( 75.0f ) ;
	pl_char_set( ew, Char_Effect, 8 ) ;
}

/*==============================================================*/
static void pls35_00_j_throw_init( PLAYER_WORK *ew )
{
	Float x ;

	ew->prio = 11 ;
	x = DOT_X( 97.0f ) ;
	if( !ew->disp_dir ) x = -DOT_X( 97.0f ) ;
	ew->pos.x += x ;
	ew->pos.y += 210.0f ;
	pl_char_set( ew, Char_Effect, 8 ) ;
}

static void pls35_00_p_catch_move( PLAYER_WORK *ew );
static void pls35_00_k_catch_move( PLAYER_WORK *ew );
static void pls35_00_throw_move( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls35_00_move( PLAYER_WORK *ew )
{
	static void ( *pls35_00_move_jmp[] )( PLAYER_WORK *ew ) = {
		pls35_00_p_catch_move, pls35_00_k_catch_move, pls35_00_throw_move, pls35_00_throw_move,
	} ;

	pls35_00_move_jmp[ew->type]( ew ) ;
}

/*==============================================================*/
static void pls35_00_p_catch_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	pl_char_move( ew ) ;
	if( pl_w->char_no.s != ew->timer0 ) pls35_00_end( ew ) ;
}

/*==============================================================*/
static void pls35_00_k_catch_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	if( !pl_w->cg_type ){
		pls35_00_end( ew ) ;
	} else if( ( ew->dir == pl_w->cg_type ) & 1 ){
		ew->dir = pl_w->cg_type & 1 ;
		ew->cg_ctr = 1 ;
		pl_char_move( ew ) ;
	}
}

/*==============================================================*/
static void pls35_00_throw_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( !ew->routine_1 ){
		if( pl_w->char_no.s != ew->timer0 ){
			ew->routine_1 ++ ;
			pl_char_set( ew, Char_Effect, 9 ) ;
		}
		pl_char_move( ew ) ;
	}else{
		if( pl_char_move( ew ) < 0 ) pls35_00_end( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls35_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls35_00_free( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls35_00.c" End of File	*/

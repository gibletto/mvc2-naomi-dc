/* Silver Samurai's player effect 00: a copy of his sprite that stays at his place (kind 0) or goes
   on with the animation it was made in and ends when his own changes (kind 1). */
#include "player.h"
#include "pl_char.h"
#include "pl2f_def.h"

static void pls2f_00_main( PLAYER_WORK *ew );

/****************************************************************/
/*			Sets a copy of his sprite, of kind no				*/
/****************************************************************/
void *pls2f_00_set( PLAYER_WORK *pl_w, Sint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls2f_00_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->ef_no = 0x2f00 ;
		ew->obj_flag = pl_w->char_no.w ;
	}
	return ew ;
}

static void pls2f_00_init( PLAYER_WORK *ew );
static void pls2f_00_move( PLAYER_WORK *ew );
static void pls2f_00_hide( PLAYER_WORK *ew );
static void pls2f_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls2f_00_main( PLAYER_WORK *ew )
{
	static void ( *pls2f_00_jmp[] )( PLAYER_WORK *ew ) = {
		pls2f_00_init, pls2f_00_move, pls2f_00_hide, pls2f_00_free,
	} ;

	pls2f_00_jmp[ew->routine_0]( ew ) ;
}

static void pls2f_00_t0_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2f_00_t1_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2f_00_init( PLAYER_WORK *ew )
{
	static void ( *pls2f_00_init_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2f_00_t0_init, pls2f_00_t1_init,
	} ;
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
	ew->disp_flag = 1 ;
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
	ew->sub_prio = -1 ;
	pls2f_00_init_jmp[ew->type]( ew, pl_w ) ;
	pls2f_00_move( ew ) ;
}

/*==============================================================*/
static void pls2f_00_t0_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos = pl_w->pos ;
	pl_char_set( ew, 21, 1 ) ;
}

/*==============================================================*/
static void pls2f_00_t1_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_set( ew, 21, 6 ) ;
}

static void pls2f_00_t0_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2f_00_t1_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void pls2f_00_move( PLAYER_WORK *ew )
{
	static void ( *pls2f_00_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2f_00_t0_move, pls2f_00_t1_move, pls2f_00_t0_move, pls2f_00_t0_move,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->prio = pl_w->prio ;
	pls2f_00_move_jmp[ew->type]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls2f_00_t0_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void pls2f_00_t1_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos = pl_w->pos ;
	if( ew->obj_flag != pl_w->char_no.w ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
	}
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls2f_00_hide( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls2f_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls2f_00.c" End of File	*/

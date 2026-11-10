/* Silver Samurai's player effect 02: the string of eight marks that follows a move of his. Kind 0
   keeps count and makes a kind 1 mark for each beat (work1); each of those stands off his place
   by pls2f_02_tbl, shows an animation and floats up. */
#include "player.h"
#include "ef_char.h"
#include "pl2f_def.h"

/* where mark n stands from him, in pixels, and the animation it shows (6 more) */
typedef struct PLS2F_02_REC {
	Sint16	x;
	Sint16	y;
	Sint16	no;
} PLS2F_02_REC;

static void pls2f_02_main( PLAYER_WORK *ew );

/****************************************************************/
/*			Sets the string of marks (its kind 0)				*/
/****************************************************************/
void *pls2f_02_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS_ST, LIST_TOP ) ) ){
		ew->work_func = pls2f_02_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x2f02 ;
		ew->type = 0 ;
	}
	return ew ;
}

/*==============================================================*/
static PLAYER_WORK *pls2f_02_mark_set( PLAYER_WORK *ef_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( ef_w, W_PLS_ST, LIST_BEHIND ) ) ){
		ew->work_func = pls2f_02_main ;
		ew->master_w = ef_w ;
		ew->ef_no = 0x2f02 ;
		ew->type = 1 ;
		ew->sub_type = 0 ;
		ew->work1 = ef_w->work1 ;
	}
	return ew ;
}

static void pls2f_02_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2f_02_step( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2f_02_next( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2f_02_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2f_02_main( PLAYER_WORK *ew )
{
	static void ( *pls2f_02_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2f_02_init, pls2f_02_step, pls2f_02_next, pls2f_02_free,
	} ;

	pls2f_02_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

static PLS2F_02_REC pls2f_02_tbl[8] = {
	{ 16, 152, 2 }, { 20, 160, 1 }, { 14, 164, 2 }, { 15, 172, 3 },
	{ 13, 181, 0 }, { 17, 190, 2 }, { 14, 197, 2 }, { 17, 203, 2 },
} ;

/*==============================================================*/
static void pls2f_02_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Sint16 x ;

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
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	if( !ew->type ){
		ew->disp_flag = 0 ;
		ew->obj_flag = 0 ;
		ew->work1 = 0 ;
		ew->work2 = 0 ;
	}
	else {
		ew->disp_flag = 1 ;
		ew->prio = 0 ;
		x = pls2f_02_tbl[ew->work1].x ;
		if( ew->disp_dir ) x = -x ;
		ew->pos.x += DOT_X( x ) ;
		ew->pos.y += DOT_Y( pls2f_02_tbl[ew->work1].y ) ;
		ew->sub_type = pls2f_02_tbl[ew->work1].no ;
		ef_char_set( ew, 27, ew->sub_type + 6 ) ;
	}
	pls2f_02_step( ew, pl_w ) ;
}

static void pls2f_02_t0( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2f_02_t1( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void pls2f_02_step( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->type ) pls2f_02_t0( ew, pl_w ) ;
	else pls2f_02_t1( ew, pl_w ) ;
}

/*==============================================================*/
static void pls2f_02_t0( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->routine_1 ){
		if( pl_w->hit_box_no ){
			ew->work2 = 0xff ;
			pls2f_02_next( ew, pl_w ) ;
			return ;
		}
		if( -- ew->obj_flag < 0 ){
			if( ew->work1 == 8 ){
				ew->routine_1 ++ ;
				ew->obj_flag = 3 ;
				ew->work2 = 0xff ;
			}
			else {
				pls2f_02_mark_set( ew ) ;
				ew->obj_flag = 3 ;
				if( ( ew->work1 = ew->work1 + 1 ) & 1 ) pl_se_req( ew, 80 ) ;
			}
		}
	}
	else {
		if( -- ew->obj_flag < 0 ) pls2f_02_next( ew, pl_w ) ;
	}
}

/*==============================================================*/
static void pls2f_02_t1( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->routine_1 ){
		if( pl_w->work2 ){
			ew->routine_1 ++ ;
			ef_char_set( ew, 27, ew->sub_type + 10 ) ;
		}
	}
	else {
		if( ef_char_move( ew ) < 0 ){
			pls2f_02_free( ew, pl_w ) ;
			return ;
		}
		ew->pos.y -= 2.142857f ;
	}
	ef_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls2f_02_next( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void pls2f_02_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls2f_02.c" End of File	*/

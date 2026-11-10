/* Servbot's player effect 00: a marker that stands beside him for as long as his animation
   group stays, drifting a little (no 0 also puts up the effect pl_shadow_set draws). */
#include "player.h"
#include "ef_char.h"

static void pls3a_00_main( PLAYER_WORK *ew );

/****************************************************************/
/*			A marker of the kind no								*/
/****************************************************************/
void *pls3a_00_set( PLAYER_WORK *pl_w, int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls3a_00_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x3a00 ;
		ew->type = no ;
	}
	return ew ;
}

static void pls3a_00_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls3a_00_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls3a_00_next( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls3a_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls3a_00_main( PLAYER_WORK *ew )
{
	static void ( *pls3a_00_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls3a_00_init, pls3a_00_move, pls3a_00_next, pls3a_00_free,
	} ;

	pls3a_00_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/* where it stands from him and how fast it drifts, by no */
static Float pls3a_00_ofs_tbl[4] = {
	-DOT_X( 7.0f ), DOT_X( 4.0f ), DOT_X( 20.0f ), 10.0f,
} ;

/*==============================================================*/
static void pls3a_00_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	int x, y ;

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
	ew->be_flag = 1 ;
	ew->prio = 8 ;
	ew->pos = pl_w->pos ;
	x = pls3a_00_ofs_tbl[ew->type * 2] ;
	y = pls3a_00_ofs_tbl[ew->type * 2 + 1] ;
	if( ew->disp_dir ){
		x = -x ;
		y = -y ;
	}
	ew->pos.x += x ;
	ew->spd.x = y ;
	ew->size_top = ew->size_bottom = ew->size_front = ew->size_back = 32 ;
	ew->obj_flag = pl_w->char_no.b.grp ;
	ef_char_set( ew, 27, ew->type ) ;
	if( !ew->type ){
		ew->be_flag = 1 ;
		ew->cg_shadow = 26 ;
		pl_shadow_set( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls3a_00_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ef_char_move( ew ) ;
	ew->cg_shadow = 26 ;
	if( !ew->cg_type ){
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
	}
	if( ew->obj_flag == pl_w->char_no.b.grp && scr_in_x_ck( ew ) ) return ;
	pls3a_00_next( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls3a_00_next( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 = 3 ;
	ew->be_flag = 0 ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls3a_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls3a_00.c" End of File	*/

/* Morrigan's Finishing Shower (character 03's shot 01): each shot is a pair, a head that flies
   from her (or from one of her effects, pls03_00) and a tail that rides on it, and a head that is
   told to by its animation puts out a burst on itself. */
#include "player.h"
#include "pl_char.h"
#include "pl03_def.h"

/* the effect's own work, from obj_mtx */
typedef struct SHL03_01_LOCAL_W {
	Uint16	char_no;	/* 000: Morrigan's animation when it was put out */
} SHL03_01_LOCAL_W;
#define SHL03_01_LW( w )	( ( SHL03_01_LOCAL_W * )&( w )->obj_mtx )

static void shl03_01_main( PLAYER_WORK *ew );

/****************************************************************/
/*			It returns nothing at its end: its caller			*/
/*			ignores it											*/
/****************************************************************/
int shl03_01_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew, *tw ;

	if( work_rest_cnt < 2 ) return 0 ;
	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl03_01_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = 0 ;
		ew->sub_type = 0 ;
		ew->ef_no = 0x301 ;
	}
	if( ( tw = work_get( ew, W_SHL, LIST_BEHIND ) ) && ew ){
		tw->work_func = shl03_01_main ;
		tw->master_w = pl_w ;
		tw->ef_w = ew ;
		tw->pl_type = pl_w->pl_type ;
		tw->type = 1 ;
		tw->sub_type = 0 ;
		tw->ef_no = 0x301 ;
		ew->ef_w = tw ;
	}
}

/****************************************************************/
/*			The same pair, flying from the work w				*/
/****************************************************************/
int shl03_01_ef_set( PLAYER_WORK *w )
{
	PLAYER_WORK *ew, *tw ;

	if( work_rest_cnt < 2 ) return 0 ;
	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl03_01_main ;
		ew->master_w = w->master_w ;
		ew->ef_w = w ;
		ew->pl_type = w->pl_type ;
		ew->type = 3 ;
		ew->sub_type = 0 ;
		ew->ef_no = 0x301 ;
	}
	if( ( tw = work_get( ew, W_SHL, LIST_BEHIND ) ) && ew ){
		tw->work_func = shl03_01_main ;
		tw->master_w = w->master_w ;
		tw->ef_w = ew ;
		tw->pl_type = w->pl_type ;
		tw->type = 4 ;
		tw->sub_type = 0 ;
		tw->ef_no = 0x301 ;
		w->ef_w = tw ;
	}
}

/*==============================================================*/
/*			A burst on the head w								*/
/*==============================================================*/
static PLAYER_WORK *shl03_01_burst_set( PLAYER_WORK *w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( w, W_SHL, LIST_BEHIND ) ) ){
		ew->work_func = shl03_01_main ;
		ew->master_w = w->master_w ;
		ew->ef_w = w ;
		ew->pl_type = w->pl_type ;
		ew->type = 2 ;
		ew->sub_type = 0 ;
		ew->ef_no = 0x301 ;
	}
	return ew ;
}

/*==============================================================*/
/*			And on a head from shl03_01_ef_set					*/
/*==============================================================*/
static PLAYER_WORK *shl03_01_ef_burst_set( PLAYER_WORK *w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( w, W_SHL, LIST_BEHIND ) ) ){
		ew->work_func = shl03_01_main ;
		ew->master_w = w->master_w ;
		ew->ef_w = w ;
		ew->pl_type = w->pl_type ;
		ew->type = 5 ;
		ew->sub_type = 0 ;
		ew->ef_no = 0x301 ;
	}
	return ew ;
}

static void shl03_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl03_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl03_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl03_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl03_01_main( PLAYER_WORK *ew )
{
	static void ( *shl03_01_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl03_01_init, shl03_01_move, shl03_01_end, shl03_01_free,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->prio = pl_w->prio ;
	shl03_01_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl03_01_head_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL03_01_LOCAL_W *lw = SHL03_01_LW( ew ) ;

	BODY_SIZE_SET( ew, 0x30305040 ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = 0 ;
	ew->atck_no = pl03_at_shower ;
	HIT_RESET( ew ) ;
	ew->spd.x = DOT_X( 88.0f ) ;
	ew->spd.y = DOT_Y( 47.0f ) ;
	if( !ew->disp_dir ) ew->spd.x = -ew->spd.x ;
	lw->char_no = pl_w->char_no.w ;
	ew->char_no.b.grp = 22 ;
	ew->char_no.b.no = 29 ;
	pl_char_set( ew, ew->char_no.b.grp, ew->char_no.b.no ) ;
	ew->cg_prm = 0 ;
	ew->sub_prio = -4 ;
	shl03_01_move( ew, pl_w ) ;
}

/*==============================================================*/
static void shl03_01_tail_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *mw = ew->ef_w ;
	SHL03_01_LOCAL_W *lw = SHL03_01_LW( ew ) ;

	BODY_SIZE_SET( ew, 0x20202020 ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = 0 ;
	ew->atck_no = pl03_at_shower ;
	HIT_RESET( ew ) ;
	ew->spd.x = DOT_X( 80.0f ) ;
	ew->acc.x = 80.0f ;
	if( !ew->disp_dir ){
		ew->spd.x = -ew->spd.x ;
		ew->acc.x = -ew->acc.x ;
	}
	lw->char_no = pl_w->char_no.w ;
	ew->char_no.b.grp = 22 ;
	ew->char_no.b.no = 31 ;
	pl_char_set2( ew, ew->char_no.b.grp, ew->char_no.b.no, mw->cg_type ) ;
	ew->sub_prio = -4 ;
	shl03_01_move( ew, pl_w ) ;
}

/*==============================================================*/
static void shl03_01_burst_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *mw = ew->ef_w ;
	SHL03_01_LOCAL_W *lw = SHL03_01_LW( ew ) ;

	BODY_SIZE_SET( ew, 0x20203030 ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = 0 ;
	ew->atck_no = mw->cg_flag1 + 69 ;
	HIT_RESET( ew ) ;
	ew->spd.x = 160.0f ;
	ew->acc.x = 105.0f ;
	if( !ew->disp_dir ){
		ew->spd.x = -ew->spd.x ;
		ew->acc.x = -ew->acc.x ;
	}
	lw->char_no = pl_w->char_no.w ;
	ew->char_no.b.grp = 22 ;
	ew->char_no.b.no = 30 ;
	pl_char_set2( ew, ew->char_no.b.grp, ew->char_no.b.no, mw->cg_type ) ;
	ew->sub_prio = -4 ;
	shl03_01_move( ew, pl_w ) ;
}

/*==============================================================*/
static void shl03_01_ef_head_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL03_01_LOCAL_W *lw = SHL03_01_LW( ew ) ;

	BODY_SIZE_SET( ew, 0x30305040 ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = 0 ;
	ew->atck_no = pl03_at_shower_next ;
	HIT_RESET( ew ) ;
	ew->spd.x = DOT_X( 50.0f ) ;
	ew->spd.y = 0.0f ;
	if( !ew->disp_dir ) ew->spd.x = -ew->spd.x ;
	lw->char_no = pl_w->char_no.w ;
	ew->char_no.b.grp = 22 ;
	ew->char_no.b.no = 35 ;
	pl_char_set( ew, ew->char_no.b.grp, ew->char_no.b.no ) ;
	ew->cg_prm = 0 ;
	ew->sub_prio = -6 ;
	shl03_01_move( ew, pl_w ) ;
}

/*==============================================================*/
static void shl03_01_ef_tail_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *mw = ew->ef_w ;
	SHL03_01_LOCAL_W *lw = SHL03_01_LW( ew ) ;

	BODY_SIZE_SET( ew, 0x20202020 ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = 0 ;
	ew->atck_no = pl03_at_shower_next ;
	HIT_RESET( ew ) ;
	ew->spd.x = DOT_X( 80.0f ) ;
	ew->acc.x = 80.0f ;
	if( !ew->disp_dir ){
		ew->spd.x = -ew->spd.x ;
		ew->acc.x = -ew->acc.x ;
	}
	lw->char_no = pl_w->char_no.w ;
	ew->char_no.b.grp = 22 ;
	ew->char_no.b.no = 37 ;
	pl_char_set2( ew, ew->char_no.b.grp, ew->char_no.b.no, mw->cg_type ) ;
	ew->sub_prio = -6 ;
	shl03_01_move( ew, pl_w ) ;
}

/*==============================================================*/
static void shl03_01_ef_burst_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *mw = ew->ef_w ;
	SHL03_01_LOCAL_W *lw = SHL03_01_LW( ew ) ;

	BODY_SIZE_SET( ew, 0x20203030 ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = 0 ;
	ew->atck_no = pl03_at_shower_next ;
	HIT_RESET( ew ) ;
	ew->spd.x = 160.0f ;
	ew->acc.x = 105.0f ;
	if( !ew->disp_dir ){
		ew->spd.x = -ew->spd.x ;
		ew->acc.x = -ew->acc.x ;
	}
	lw->char_no = pl_w->char_no.w ;
	ew->char_no.b.grp = 22 ;
	ew->char_no.b.no = 36 ;
	pl_char_set2( ew, ew->char_no.b.grp, ew->char_no.b.no, mw->cg_type ) ;
	ew->sub_prio = -6 ;
	shl03_01_move( ew, pl_w ) ;
}

/*==============================================================*/
static void shl03_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl03_01_init_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl03_01_head_init, shl03_01_tail_init, shl03_01_burst_init,
		shl03_01_ef_head_init, shl03_01_ef_tail_init, shl03_01_ef_burst_init,
	} ;

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
	shl03_01_init_jmp[ew->type]( ew, pl_w ) ;
}

/*==============================================================*/
/*			A head flies with Morrigan's animation, and puts	*/
/*			out a burst when its own animation says				*/
/*==============================================================*/
static void shl03_01_head_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL03_01_LOCAL_W *lw = SHL03_01_LW( ew ) ;
	PLAYER_WORK *w, *tw ;

	if( pl_w->char_no.w != lw->char_no ){
		ew->routine_1 ++ ;
		ew->char_no.b.grp = 22 ;
		ew->char_no.b.no = 32 ;
		pl_char_set( ew, ew->char_no.b.grp, ew->char_no.b.no ) ;
		return ;
	}
	ew->pos.x = pl_w->pos.x + ew->spd.x ;
	ew->pos.y = pl_w->pos.y + ew->spd.y ;
	if( vs_sys_work.slow_bit & 1 << vs_sys_work.slow_step ) return ;
	pl_char_move( ew ) ;
	if( ew->cg_prm ){
		ew->cg_prm = 0 ;
		ew->atck_no = pl03_at_shower_hit ;
		HIT_RESET( ew ) ;
		w = shl03_01_burst_set( ew ) ;
		if( w ){
			tw = ew->ef_w ;	/* the tail rides on the burst now */
			tw->ef_w = w ;
			ew->ef_w = w ;
		}
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl03_01_head_vanish( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->act_st == N_SUPER ){
		ew->pos.x = pl_w->pos.x + ew->spd.x ;
		ew->pos.y = pl_w->pos.y + ew->spd.y ;
	}
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void shl03_01_head_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl03_01_head_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl03_01_head_fly, shl03_01_head_vanish,
	} ;

	shl03_01_head_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
/*			A tail rides on its head, in step with the			*/
/*			head's animation									*/
/*==============================================================*/
static void shl03_01_tail_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL03_01_LOCAL_W *lw = SHL03_01_LW( ew ) ;
	PLAYER_WORK *mw = ew->ef_w ;

	if( pl_w->char_no.w != lw->char_no ){
		ew->routine_1 ++ ;
		ew->char_no.b.grp = 22 ;
		ew->char_no.b.no = 34 ;
		pl_char_set( ew, ew->char_no.b.grp, ew->char_no.b.no ) ;
		return ;
	}
	if( mw->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->pos.x = mw->pos.x + ( !mw->type ? ew->spd.x : ew->acc.x ) ;
	ew->pos.y = mw->pos.y ;
	if( vs_sys_work.slow_bit & 1 << vs_sys_work.slow_step ) return ;
	ew->char_no.b.grp = 22 ;
	ew->char_no.b.no = 31 ;
	pl_char_set2( ew, ew->char_no.b.grp, ew->char_no.b.no, mw->cg_type ) ;
	if( !scr_in_x_ck( ew ) ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl03_01_tail_vanish( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *mw = ew->ef_w ;

	ew->pos.x = mw->pos.x + ( !mw->type ? ew->spd.x : ew->acc.x ) ;
	ew->pos.y = mw->pos.y ;
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void shl03_01_tail_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl03_01_tail_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl03_01_tail_fly, shl03_01_tail_vanish,
	} ;

	shl03_01_tail_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl03_01_burst_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL03_01_LOCAL_W *lw = SHL03_01_LW( ew ) ;
	PLAYER_WORK *mw = ew->ef_w ;

	if( pl_w->char_no.w != lw->char_no ){
		ew->routine_1 ++ ;
		ew->char_no.b.grp = 22 ;
		ew->char_no.b.no = 33 ;
		pl_char_set( ew, ew->char_no.b.grp, ew->char_no.b.no ) ;
		return ;
	}
	if( mw->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->pos.x = mw->pos.x + ( !mw->type ? ew->spd.x : ew->acc.x ) ;
	ew->pos.y = mw->pos.y ;
	if( vs_sys_work.slow_bit & 1 << vs_sys_work.slow_step ) return ;
	ew->char_no.b.grp = 22 ;
	ew->char_no.b.no = 30 ;
	pl_char_set2( ew, ew->char_no.b.grp, ew->char_no.b.no, mw->cg_type ) ;
	if( !scr_in_x_ck( ew ) ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl03_01_burst_vanish( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *mw = ew->ef_w ;

	ew->pos.x = mw->pos.x + ( !mw->type ? ew->spd.x : ew->acc.x ) ;
	ew->pos.y = mw->pos.y ;
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void shl03_01_burst_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl03_01_burst_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl03_01_burst_fly, shl03_01_burst_vanish,
	} ;

	shl03_01_burst_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
/*			A head from shl03_01_ef_set flies with the work it	*/
/*			came from											*/
/*==============================================================*/
static void shl03_01_ef_head_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL03_01_LOCAL_W *lw = SHL03_01_LW( ew ) ;
	PLAYER_WORK *mw = ew->ef_w ;
	PLAYER_WORK *w, *tw ;

	if( pl_w->char_no.w != lw->char_no ){
		ew->routine_1 ++ ;
		ew->char_no.b.grp = 22 ;
		ew->char_no.b.no = 38 ;
		pl_char_set( ew, ew->char_no.b.grp, ew->char_no.b.no ) ;
		return ;
	}
	ew->pos.x = mw->pos.x + ew->spd.x ;
	ew->pos.y = mw->pos.y + ew->spd.y ;
	if( vs_sys_work.slow_bit & 1 << vs_sys_work.slow_step ) return ;
	pl_char_move( ew ) ;
	if( ew->cg_prm ){
		ew->cg_prm = 0 ;
		w = shl03_01_ef_burst_set( ew ) ;
		if( w ){
			tw = mw->ef_w ;
			tw->ef_w = w ;
			mw->ef_w = w ;
		}
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl03_01_ef_head_vanish( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *mw = ew->ef_w ;

	ew->pos.x = mw->pos.x + ew->spd.x ;
	ew->pos.y = mw->pos.y + ew->spd.y ;
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void shl03_01_ef_head_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl03_01_ef_head_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl03_01_ef_head_fly, shl03_01_ef_head_vanish,
	} ;

	shl03_01_ef_head_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl03_01_ef_tail_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL03_01_LOCAL_W *lw = SHL03_01_LW( ew ) ;
	PLAYER_WORK *mw = ew->ef_w ;

	if( pl_w->char_no.w != lw->char_no ){
		ew->routine_1 ++ ;
		ew->char_no.b.grp = 22 ;
		ew->char_no.b.no = 40 ;
		pl_char_set( ew, ew->char_no.b.grp, ew->char_no.b.no ) ;
		return ;
	}
	if( mw->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->pos.x = mw->pos.x + ( mw->type == 3 ? ew->spd.x : ew->acc.x ) ;
	ew->pos.y = mw->pos.y ;
	if( vs_sys_work.slow_bit & 1 << vs_sys_work.slow_step ) return ;
	ew->char_no.b.grp = 22 ;
	ew->char_no.b.no = 37 ;
	pl_char_set2( ew, ew->char_no.b.grp, ew->char_no.b.no, mw->cg_type ) ;
	if( !scr_in_x_ck( ew ) ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl03_01_ef_tail_vanish( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *mw = ew->ef_w ;

	ew->pos.x = mw->pos.x + ( mw->type == 3 ? ew->spd.x : ew->acc.x ) ;
	ew->pos.y = mw->pos.y ;
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void shl03_01_ef_tail_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl03_01_ef_tail_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl03_01_ef_tail_fly, shl03_01_ef_tail_vanish,
	} ;

	shl03_01_ef_tail_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl03_01_ef_burst_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL03_01_LOCAL_W *lw = SHL03_01_LW( ew ) ;
	PLAYER_WORK *mw = ew->ef_w ;

	if( pl_w->char_no.w != lw->char_no ){
		ew->routine_1 ++ ;
		ew->char_no.b.grp = 22 ;
		ew->char_no.b.no = 39 ;
		pl_char_set( ew, ew->char_no.b.grp, ew->char_no.b.no ) ;
		return ;
	}
	if( mw->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->pos.x = mw->pos.x + ( mw->type == 3 ? ew->spd.x : ew->acc.x ) ;
	ew->pos.y = mw->pos.y ;
	if( vs_sys_work.slow_bit & 1 << vs_sys_work.slow_step ) return ;
	ew->char_no.b.grp = 22 ;
	ew->char_no.b.no = 36 ;
	pl_char_set2( ew, ew->char_no.b.grp, ew->char_no.b.no, mw->cg_type ) ;
	if( !scr_in_x_ck( ew ) ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl03_01_ef_burst_vanish( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *mw = ew->ef_w ;

	ew->pos.x = mw->pos.x + ( mw->type == 3 ? ew->spd.x : ew->acc.x ) ;
	ew->pos.y = mw->pos.y ;
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void shl03_01_ef_burst_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl03_01_ef_burst_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl03_01_ef_burst_fly, shl03_01_ef_burst_vanish,
	} ;

	shl03_01_ef_burst_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl03_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl03_01_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl03_01_head_move, shl03_01_tail_move, shl03_01_burst_move,
		shl03_01_ef_head_move, shl03_01_ef_tail_move, shl03_01_ef_burst_move,
	} ;

	shl03_01_move_jmp[ew->type]( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl03_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl03_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl03_01.c" End of File	*/

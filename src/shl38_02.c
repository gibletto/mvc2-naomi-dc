/* Captain Commando's third effect (character 38's shot 02): a head that flies out from him and
   seven works chained behind it, each one following the one before. */
#include "player.h"
#include "pl_char.h"
#include "pl38_def.h"

/* the effect's own work, from obj_flag */
typedef struct SHL38_02_LOCAL_W {
	PLAYER_WORK	*next_w;	/* 000: the work after the chain (head) */
	PLAYER_WORK	*head_w;	/* 004: the head (chain) */
	Sint8		no;		/* 008: place in the chain, 6 for the first (chain) */
	Uint8		flag;		/* 009: the chain is shown (head) */
} SHL38_02_LOCAL_W;
#define SHL38_02_LW( w )	( ( SHL38_02_LOCAL_W * )&( w )->obj_flag )

static void shl38_02_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *Shl38_02_set( PLAYER_WORK *pl_w )
{
	register PLAYER_WORK *ew = 0 ;
	register PLAYER_WORK *head ;
	register int i ;

	if( work_rest_cnt > 7 ){
		ew = work_get( 0, W_SHL_ST, LIST_TOP ) ;
		ew->work_func = shl38_02_main ;
		ew->type = ew->sub_type = 0 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x3803 ;
		head = ew ;
		for( i = 6 ; i >= 0 ; i -- ){
			register PLAYER_WORK *prev = ew ;

			ew = work_get( prev, W_SHL_ST, LIST_BEHIND ) ;
			ew->work_func = shl38_02_main ;
			ew->type = 1 ;
			ew->sub_type = 0 ;
			ew->master_w = prev->master_w ;
			ew->pl_type = pl_w->pl_type ;
			ew->ef_no = 0x3803 ;
			SHL38_02_LW( ew )->head_w = head ;
			SHL38_02_LW( ew )->no = i ;
			ew->dir = 0xff ;
			ew->disp_flag = 0 ;
		}
	}
	return ew ;
}

static void shl38_02_head_main( PLAYER_WORK *ew ) ;
static void shl38_02_chain_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PLAYER_WORK *prev_w ) ;

/*==============================================================*/
static void shl38_02_main( PLAYER_WORK *ew )
{
	if( !ew->type ) shl38_02_head_main( ew ) ;
	else shl38_02_chain_main( ew, ew->master_w, ew->before_w ) ;
}

static void shl38_02_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_02_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_02_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void shl38_02_head_main( PLAYER_WORK *ew )
{
	static void ( *shl38_02_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl38_02_init, shl38_02_move, shl38_02_end, shl38_02_end,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->char_no.b.grp == Char_Super && !pl_w->char_no.b.no ) shl38_02_jmp[ew->routine_0]( ew, pl_w ) ;
	else shl38_02_end( ew, pl_w ) ;
}

/*==============================================================*/
static void shl38_02_init( register PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	register PLAYER_WORK *w ;
	Float x ;
	register int i ;

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
	ew->atck_no = pl38_at_sword ;
	ew->hit_flag = ew->atck_attr = 0 ;
	ew->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[ew->pl_no] ++ ;
	ew->atck_flag = HIT_ON | HIT_SHOT | HIT_OWNER ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->dir = 0xff ;
	ew->timer0 = 10 ;
	ew->spd.y = DOT_Y( 160.0f ) ;
	x = DOT_X( 8.0f ) ;
	if( ew->disp_dir ) x = -DOT_X( 8.0f ) ;
	ew->spd.x += x ;
	ew->pos.x = pl_w->pos.x + x ;
	ew->pos.y = pl_w->pos.y + DOT_Y( 160.0f ) ;
	ew->sub_prio = -1 ;
	pl_char_set( ew, 22, 2 ) ;
	hit_work_set( ew ) ;
	w = ew ;
	for( i = 0 ; i <= 6 ; i ++ ){
		w = w->behind_w ;
		CHAR_PART_COPY( w, pl_w ) ;
		w->disp_flag = 1 ;
		w->pl_no = pl_w->pl_no ;
		w->pl_type = pl_w->pl_type ;
		w->scale.x = pl_w->scale.x ;
		w->scale.y = pl_w->scale.y ;
		w->atck_level = pl_w->atck_level ;
		w->work_no = pl_w->work_no ;
		w->col_cnt = pl_w->col_cnt ;
		w->scale = pl_w->scale ;
		w->prio = pl_w->prio ;
		w->atck_flag = HIT_ON | HIT_SHOT | HIT_OWNER ;
		w->dm_flag = HIT_ON | HIT_SHOT ;
		w->size_top = w->size_bottom = w->size_front = w->size_back = 64 ;
	}
	SHL38_02_LW( ew )->next_w = w ;
	SHL38_02_LW( ew )->flag = 0 ;
	pl_se_req( ew, 72 ) ;
}

static void shl38_02_head_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_02_head_swing( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_02_head_vanish( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*--------------------------------------------------------------*/
static void shl38_02_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl38_02_head_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl38_02_head_start, shl38_02_head_swing, shl38_02_head_vanish,
	} ;

	shl38_02_head_jmp[ew->routine_1]( ew, pl_w ) ;
}

/* the sides it pushes the opponent on by dir (1 x, 2 y) */
static Sint8 shl38_02_push_tbl[20] = {
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0,
} ;

/* by dir, x then y: the head's speed, the chain's first and later places, and a step on from them */
static Float shl38_02_spd_tbl[36] = {
	DOT_X( 8.0f ), DOT_Y( 160.0f ),
	DOT_X( 8.0f ), DOT_Y( 160.0f ),
	DOT_X( 8.0f ), DOT_Y( 160.0f ),
	DOT_X( 8.0f ), DOT_Y( 160.0f ),
	-DOT_X( 10.0f ), DOT_Y( 169.0f ),
	-DOT_X( 10.0f ), DOT_Y( 169.0f ),
	-DOT_X( 58.0f ), DOT_Y( 152.0f ),
	-DOT_X( 74.0f ), DOT_Y( 124.0f ),
	-DOT_X( 100.0f ), DOT_Y( 116.0f ),
	-DOT_X( 125.0f ), DOT_Y( 89.0f ),
	-DOT_X( 122.0f ), DOT_Y( 93.0f ),
	-DOT_X( 125.0f ), DOT_Y( 64.0f ),
	-205.0f, 30.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 0.0f,
} ;

static Float shl38_02_head_tbl[36] = {
	0.0f, DOT_Y( 144.0f ),
	0.0f, DOT_Y( 144.0f ),
	0.0f, DOT_Y( 144.0f ),
	0.0f, DOT_Y( 144.0f ),
	0.0f, DOT_Y( 144.0f ),
	-DOT_X( 26.0f ), DOT_Y( 151.0f ),
	-100.0f, DOT_Y( 132.0f ),
	-130.0f, DOT_Y( 120.0f ),
	-DOT_X( 103.0f ), DOT_Y( 104.0f ),
	-DOT_X( 122.0f ), DOT_Y( 72.0f ),
	-230.0f, DOT_Y( 58.0f ),
	-235.0f, DOT_Y( 33.0f ),
	-240.0f, 0.0f,
	-DOT_X( 352.0f ), DOT_Y( 24.0f ),
	-DOT_X( 352.0f ), DOT_Y( 24.0f ),
	-DOT_X( 352.0f ), DOT_Y( 24.0f ),
	-DOT_X( 352.0f ), DOT_Y( 24.0f ),
	-DOT_X( 352.0f ), DOT_Y( 24.0f ),
} ;

static Float shl38_02_tail_tbl[36] = {
	0.0f, DOT_Y( 96.0f ),
	0.0f, DOT_Y( 96.0f ),
	0.0f, DOT_Y( 96.0f ),
	0.0f, DOT_Y( 96.0f ),
	0.0f, DOT_Y( 96.0f ),
	-DOT_X( 16.0f ), DOT_Y( 95.0f ),
	-65.0f, DOT_Y( 88.0f ),
	-85.0f, DOT_Y( 76.0f ),
	-DOT_X( 67.0f ), DOT_Y( 67.0f ),
	-135.0f, 105.0f,
	-DOT_X( 89.0f ), DOT_Y( 36.0f ),
	-155.0f, 45.0f,
	-160.0f, 0.0f,
	-160.0f, 0.0f,
	-160.0f, 0.0f,
	-160.0f, 0.0f,
	-160.0f, 0.0f,
	-160.0f, 0.0f,
} ;

static Float shl38_02_ofs_tbl[36] = {
	0.0f, -1.0f,
	0.0f, -1.0f,
	0.0f, -1.0f,
	0.0f, -1.0f,
	0.0f, -1.0f,
	1.0f, -1.0f,
	1.0f, -1.0f,
	1.0f, -1.0f,
	1.0f, -1.0f,
	1.0f, -1.0f,
	1.0f, -1.0f,
	1.0f, -1.0f,
	1.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 0.0f,
} ;

/*==============================================================*/
static void shl38_02_head_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float x ;
	Sint8 no ;
	int t ;

	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->prio = pl_w->prio ;
	ew->sub_prio = -8 ;
	if( pl_char_move( ew ) >= 0 ){
		ew->pos.x += ew->spd.x ;
		ew->pos.y += ew->spd.y ;
	} else {
		ew->routine_1 ++ ;
		SHL38_02_LW( ew )->flag = 1 ;
		pl_w->cg_ctr = 2 ;
		ew->dir = 0 ;
		ew->spd.y = DOT_Y( 160.0f ) ;
		x = DOT_X( 8.0f ) ;
		if( ew->disp_dir ) x = -DOT_X( 8.0f ) ;
		ew->spd.x = x ;
		ew->pos.x = pl_w->pos.x + x ;
		ew->pos.y = pl_w->pos.y + DOT_Y( 160.0f ) ;
		pl_char_set2( ew, 22, 3, 0 ) ;
		no = ew->cg_flag1 ;
		if( no ){
			if( ew->cg_flag1 == 60 ){
				t = pl_w->team_type ;
				if( t > 3 && t < 6 ) no = 47 ;
			}
			ew->atck_no = no ;
			HIT_RESET( ew ) ;
		}
	}
	hit_work_set( ew ) ;
}

/****************************************************************/
void shl38_02_spd_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float *p = &shl38_02_spd_tbl[ew->dir * 2] ;
	Float x = *p ++ ;
	Float y = *p ;

	if( ew->disp_dir ) x = -x ;
	ew->spd.x = x ;
	ew->spd.y = y ;
}

/*--------------------------------------------------------------*/
static void shl38_02_head_swing( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Sint8 no ;
	int t ;

	if( pl_w->cg_type >= 0 ){
		if( ew->dir != pl_w->cg_type ){
			ew->dir = pl_w->cg_type ;
			pl_char_set2( ew, 22, 3, ew->dir ) ;
			no = ew->cg_flag1 ;
			if( no ){
				if( ew->cg_flag1 == 60 ){
					t = pl_w->team_type ;
					if( t > 3 && t < 3 ) no = 47 ;	/* 3 twice */
				}
				ew->atck_no = no ;
				HIT_RESET( ew ) ;
			}
			shl38_02_spd_set( ew, pl_w ) ;
		}
		ew->pos.x = pl_w->pos.x + ew->spd.x ;
		ew->pos.y = pl_w->pos.y + ew->spd.y ;
		if( !pl_w->hit_flag ) hit_work_set( ew ) ;
	} else {
		SHL38_02_LW( ew )->flag = 0 ;
		ew->routine_1 ++ ;
		pl_char_set( ew, 22, 5 ) ;
		shl38_02_head_vanish( ew, pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl38_02_head_vanish( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	if( pl_char_move( ew ) < 0 ) shl38_02_end( ew, pl_w ) ;
}

static void shl38_02_push( PLAYER_WORK *ew, PLAYER_WORK *em_w ) ;

/*==============================================================*/
static void shl38_02_chain_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PLAYER_WORK *prev_w )
{
	Float hx, hy, tx, ty, ox ;
	Sint8 no ;
	int t ;
	PLAYER_WORK *head_w ;

	if( ew->routine_0 ){
		work_free( ew ) ;
		return ;
	}
	if( !( head_w = SHL38_02_LW( ew )->head_w ) ){
		work_free( ew ) ;
		return ;
	}
	if( prev_w->dir != ew->dir ){
		ew->dir = prev_w->dir ;
		ew->disp_flag = 1 ;
		pl_char_set2( ew, 22, 4, ew->dir ) ;
		if( ( no = ew->cg_flag1 ) ){
			if( no == 60 ){
				t = pl_w->team_type ;
				if( t > 3 && t < 3 ) no = 47 ;	/* 3 twice */
			}
			ew->atck_no = no ;
			HIT_RESET( ew ) ;
		}
	}
	hx = shl38_02_head_tbl[ew->dir * 2] ;
	hy = shl38_02_head_tbl[ew->dir * 2 + 1] ;
	tx = shl38_02_tail_tbl[ew->dir * 2] ;
	ty = shl38_02_tail_tbl[ew->dir * 2 + 1] ;
	ox = shl38_02_ofs_tbl[ew->dir * 2] ;
	if( ew->disp_dir ){
		hx = -hx ;
		tx = -tx ;
		ox = -ox ;
	}
	if( SHL38_02_LW( ew )->no == 6 ){
		ew->pos.x = prev_w->pos.x + hx ;
		ew->pos.y = prev_w->pos.y + hy ;
	} else {
		ew->pos.x = prev_w->pos.x + tx ;
		ew->pos.y = prev_w->pos.y + ty ;
	}
	ew->pos.x += ox ;
	ew->pos.y += shl38_02_ofs_tbl[ew->dir * 2 + 1] ;
	if( SHL38_02_LW( head_w )->flag ){
		if( !pl_w->hit_flag ) hit_work_set( ew ) ;
		if( scr_in_ck( ew ) ) ew->disp_flag = 1 ;
		else ew->disp_flag = 0 ;
	} else ew->disp_flag = 0 ;
	if( ew->hit_flag ) shl38_02_push( ew, ew->em_w ) ;
}

/*==============================================================*/
static void shl38_02_push( PLAYER_WORK *ew, PLAYER_WORK *em_w )
{
	HIT_BOX *pb ;
	Float x[2], y[2], ov, oy ;

	if( em_w && !em_w->work_kind && em_w->routine_1 == PL_DAMAGE && shl38_02_push_tbl[ew->dir] ){
		pb = em_w->hit_box_tbl + PUSH_BOX ;
		x[0] = DOT_X( pb->x ) ;
		if( em_w->pl_dir ) x[0] = -x[0] ;
		x[0] += em_w->pos.x ;
		x[1] = 0.0f ;
		if( em_w->pl_dir ) x[1] = -x[1] ;
		x[1] += ew->pos.x ;
		if( ( x[0] -= x[1] ) < 0.0f ) x[0] = -x[0] ;
		x[1] = DOT_X( pb->w ) + DOT_X( 64.0f ) ;
		if( ( ov = x[1] - x[0] ) < 0.0f ) return ;
		y[0] = DOT_Y( pb->y ) + em_w->pos.y ;
		y[1] = ew->pos.y ;
		if( ( y[0] -= y[1] ) < 0.0f ) y[0] = -y[0] ;
		y[1] = DOT_Y( pb->h ) + DOT_Y( 64.0f ) ;
		if( ( oy = y[1] - y[0] ) < 0.0f ) return ;
		if( shl38_02_push_tbl[ew->dir] & 1 ){
			if( ew->disp_dir ) ov = -ov ;
			em_w->pos.x += ov ;
		}
		if( shl38_02_push_tbl[ew->dir] & 2 ){
			if( oy < 0.0f ){
				oy = -oy ;
				em_w->pos.y = ew->pos.y ;
			}
			if( ( em_w->pos.y -= oy ) < em_w->ground_y ) em_w->pos.y = em_w->ground_y ;
		}
	}
}

/*--------------------------------------------------------------*/
static void shl38_02_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *w = ew->behind_w ;
	int i ;

	for( i = 0 ; i <= 6 ; i ++ ){
		w->routine_0 = 1 ;
		w = w->behind_w ;
	}
	pl_se_req( ew, 58 ) ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl38_02.c" End of File	*/

/* Sentinel's effect 03 (character 34's shot 03): the chain of eleven parts behind his heavy punch's
   fist, standing (type 0) or crouching (type 1). Type 2 hits as the snapback, but nothing sets it.
   The head (sub_type 0) keeps the chain's count and a part follows the one before it. */
#include "player.h"
#include "pl34_def.h"

/* the effect's own work, at obj_mtx */
typedef struct SHL34_03_LOCAL_W {
	Sint16	a;		/* alternates between 0 and 1 each frame */
	Sint16	b;		/* -1 once it hit, set by a part */
	Sint16	c;
	Sint16	d;		/* the parts out so far */
	Sint16	e;		/* a part has been added */
	Sint8	f;		/* his tlocal punch_cg, kept */
} SHL34_03_LOCAL_W;
#define SHL34_03_LW( w )	( ( SHL34_03_LOCAL_W * )&( w )->obj_mtx )

static void shl34_03_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl34_03_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl34_03_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->sub_type = 0 ;
		ew->ef_no = 0x3403 ;
	}
	return ew ;
}

/*==============================================================*/
static PLAYER_WORK *shl34_03_part_set( PLAYER_WORK *ew, Uint8 no )
{
	PLAYER_WORK *tw ;

	if( ( tw = work_get( ew, W_SHL, LIST_BEHIND ) ) ){
		tw->work_func = shl34_03_main ;
		tw->master_w = ew->master_w ;
		tw->ef_w = ew ;
		tw->type = ew->type ;
		tw->sub_type = no ;
		tw->ef_no = 0x3403 ;
	}
	return tw ;
}

static void shl34_03_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl34_03_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl34_03_end( PLAYER_WORK *ew );
static void shl34_03_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl34_03_main( PLAYER_WORK *ew )
{
	static void ( *shl34_03_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl34_03_init, shl34_03_move, shl34_03_end, shl34_03_free,
	} ;

	shl34_03_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void shl34_03_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL34_03_LOCAL_W *lw = SHL34_03_LW( ew ) ;
	int i ;

	BODY_SIZE_SET( ew, 0x10101010 ) ;
	if( !ew->sub_type ){
		for( i = 11 ; i >= 1 ; i -- ){
			shl34_03_part_set( ew, i ) ;
			continue ;
		}
	}
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
	lw->f = 0 ;
	lw->a = 0 ;
	lw->c = 0 ;
	lw->b = 0 ;
	lw->e = 0 ;
	lw->d = 1 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON ;
	if( ew->type == 2 ){
		ew->atck_no = pl34_at_snapback ;
		HIT_RESET( ew ) ;
	} else {
		ew->atck_no = ATCK_STAND_HP ;
		HIT_RESET( ew ) ;
	}
	hit_work_set( ew ) ;
	shl34_03_move( ew, pl_w ) ;
}

static void shl34_03_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl34_03_char_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl34_03_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL34_03_LOCAL_W *lw = SHL34_03_LW( ew ) ;
	PL34_LOCAL_W *tl = ( PL34_LOCAL_W * )pl_w->tlocal ;

	lw->f = tl->punch_cg ;
	if( lw->f < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	lw->a ++ ;
	lw->a &= 1 ;
	shl34_03_pos_set( ew, pl_w ) ;
	shl34_03_char_set( ew, pl_w ) ;
}

/*==============================================================*/
/*			The head waits for the animation's code, 127 or		*/
/*			another, which sets b								*/
/*==============================================================*/
static void shl34_03_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL34_03_LOCAL_W *lw = SHL34_03_LW( ew ) ;

	if( pl_w->cg_prm ){
		if( pl_w->cg_prm == 127 ) lw->b = -1 ;
		else lw->b = 1 ;
		pl_w->cg_prm = 0 ;
	}
	if( lw->b < 0 ){
		ew->timer0 = 8 ;
		ew->routine_1 ++ ;
	}
	shl34_03_hold( ew, pl_w ) ;
}

/*==============================================================*/
static void shl34_03_part_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *hw = ew->ef_w ;
	PLAYER_WORK *bw = ew->before_w ;
	SHL34_03_LOCAL_W *lw = SHL34_03_LW( ew ) ;
	SHL34_03_LOCAL_W *hlw = SHL34_03_LW( hw ) ;
	SHL34_03_LOCAL_W *blw = SHL34_03_LW( bw ) ;

	if( hw->routine_0 != 1 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	lw->f = hlw->f ;
	lw->a = blw->a ;
	shl34_03_pos_set( ew, pl_w ) ;
	shl34_03_char_set( ew, pl_w ) ;
	if( ew->hit_box_no ){
		hlw->d = ew->sub_type ;
		hlw->b = -1 ;
		lw->b = 0 ;
		ew->hit_box_no = 0 ;
		return ;
	}
	if( ew->hit_flag ){
		hlw->d = ew->sub_type ;
		hlw->b = -1 ;
		lw->b = 0 ;
		ew->hit_flag = 0 ;
		return ;
	}
	if( ew->disp_flag ){
		if( hlw->e ){
			hlw->e = 0 ;
			ew->disp_flag = 0 ;
			return ;
		}
		if( hlw->d == ew->sub_type ){
			if( hlw->d < 11 ){
				hlw->d ++ ;
				hlw->e = 1 ;
			}
		}
		if( hlw->b > 0 ){
			if( Sys->game_timer0 & 1 ) hit_work_set( ew ) ;
		}
		ew->disp_flag = scr_in_ck( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl34_03_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl34_03_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl34_03_wait, shl34_03_hold,
	} ;

	ew->prio = pl_w->prio ;
	if( ew->sub_type ){
		shl34_03_part_move( ew, pl_w ) ;
		return ;
	}
	if( ew->type == 2 ){
		if( pl_w->routine_1 || pl_w->act_st != N_SUPER ){
			ew->routine_0 ++ ;
			ew->disp_flag = 0 ;
			return ;
		}
	} else {
		if( pl_w->routine_1 || pl_w->act_st != N_NML_ATCK ){
			ew->routine_0 ++ ;
			ew->disp_flag = 0 ;
			return ;
		}
	}
	shl34_03_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl34_03_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void shl34_03_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
static void shl34_03_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *hw = ew->ef_w ;
	PLAYER_WORK *bw = ew->before_w ;
	SHL34_03_LOCAL_W *hlw = SHL34_03_LW( hw ) ;

	if( !ew->sub_type ){
		if( ew->type == 0 || ew->type == 2 ){
			ew->spd.x = DOT_X( 88.0f ) ;
			ew->spd.y = DOT_Y( 104.0f ) ;
		} else {
			ew->spd.x = DOT_X( 88.0f ) ;
			ew->spd.y = DOT_Y( 65.0f ) ;
		}
		if( !ew->disp_dir ) ew->spd.x = -ew->spd.x ;
		ew->pos.x = pl_w->pos.x + ew->spd.x ;
		ew->pos.y = pl_w->pos.y + ew->spd.y ;
		return ;
	}
	if( ew->sub_type <= hlw->d ){
		ew->spd.x = DOT_X( 32.0f ) ;
		if( !ew->disp_dir ) ew->spd.x = -ew->spd.x ;
		ew->pos.x = bw->pos.x + ew->spd.x ;
		ew->pos.y = bw->pos.y ;
		ew->disp_flag = 1 ;
	} else ew->disp_flag = 0 ;
}

/*==============================================================*/
static void shl34_03_char_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Sint8 head_tbl[10] = { 0, 1, 4, 4, 6, 6, 8, 8, 10, 10 } ;
	static Sint8 part_tbl[10] = { 2, 3, 5, 5, 7, 7, 9, 9, 10, 10 } ;
	SHL34_03_LOCAL_W *lw = SHL34_03_LW( ew ) ;
	SHL34_03_LOCAL_W *hlw = SHL34_03_LW( ew->ef_w ) ;

	int no ;

	if( ew->sub_type ){
		if( ew->sub_type != hlw->d ) no = part_tbl[lw->f + lw->a] ;
		else no = head_tbl[lw->f + lw->a] ;
	} else no = part_tbl[lw->f + lw->a] ;
	pl_char_set2( ew, Char_Effect, 0, no ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl34_03.c" End of File	*/

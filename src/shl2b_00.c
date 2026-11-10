/* Sabretooth's effect 00 (character 2b's shot 00): the claws of his two ground attacks (Birdie and
   Hyper Birdie) as they swing out, strike and come back, with the bits (shl2b_01) that fly off. */
#include "player.h"
#include "pl_char.h"
#include "pl2b_def.h"

/* the piece that flies off a swing */
extern PLAYER_WORK *pls2b_00_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w, int no );

/* the animation each swing takes, picked at random */
static Uint8 shl2b_00_anm_tbl[] = {
	0, 0, 1, 1, 2, 2, 0, 2, 0, 0, 1, 1, 2, 2, 1, 0,
} ;

static void shl2b_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl2b_00_set( PLAYER_WORK *pl_w, int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_END ) ) ){
		ew->ef_no = 0x2b00 ;
		ew->type = no ;
		ew->work_func = shl2b_00_main ;
		ew->master_w = pl_w ;
	}
	return ew ;
}

static void shl2b_00_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2b_00_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2b_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl2b_00_main( PLAYER_WORK *ew )
{
	static void ( *shl2b_00_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2b_00_init, shl2b_00_move, shl2b_00_free,
	} ;

	shl2b_00_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void shl2b_00_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL2B_LOCAL_W *lw = PL2B_LW( pl_w ) ;

	ew->be_flag = 1 ;
	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->prio = 0 ;
	ew->disp_dir = pl_w->disp_dir ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->size_top = 112 ;
	ew->size_bottom = 0 ;
	ew->size_front = 64 ;
	ew->size_back = 48 ;
	ew->pos.x = scr_work.scr_right + DOT_X( 64.0f ) ;
	ew->spd.x = -10.0f ;
	ew->acc.x = 0.0f ;
	if( pl_w->pl_dir ){
		ew->pos.x = scr_work.scr_left + -DOT_X( 64.0f ) ;
		ew->spd.x = -ew->spd.x ;
	}
	ew->pos.y = pl_w->ground_y ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	lw->birdie_flag = 1 ;
	pl_shadow_set( ew ) ;
	hit_work_set( ew ) ;
	pl_char_set( ew, Char_Effect, 0 ) ;
}

/*--------------------------------------------------------------*/
static void shl2b_00_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL2B_LOCAL_W *lw = PL2B_LW( pl_w ) ;

	switch( ew->routine_1 ){
	case 0 :
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		if( pl_char_move( ew ) < 0 ){
			ew->routine_1 ++ ;
			if( !ew->type ){
				ew->timer0 = 3 ;
				ew->timer1 = pl_w->atck_level ;
				ew->sub_type = 1 ;
			}
			else {
				ew->timer0 = 32 ;
				ew->timer1 = shl2b_00_anm_tbl[Rnd() & 15] ;
				ew->sub_type = 6 ;
			}
			pl_char_set( ew, Char_Effect, ew->timer1 + ew->sub_type ) ;
		}
		hit_work_set( ew ) ;
		break ;
	case 1 :
		if( pl_w->hit_box_no || ew->hit_box_no ){
			ew->routine_1 ++ ;
			pl_char_set( ew, Char_Effect, 5 ) ;
			return ;
		}
		if( pl_w->act_st == N_CHANGE && pl_w->team_type != 8 ){
			ew->routine_1 ++ ;
			pl_char_set( ew, Char_Effect, 4 ) ;
			return ;
		}
		if( ew->cg_type & 1 ){
			ew->cg_type = 0 ;
			shl2b_01_set( ew, pl_w, ew->type, ew->timer1 ) ;
		}
		if( ew->cg_type & 2 ){
			ew->cg_type = 0 ;
			pls2b_00_set( ew, pl_w, ew->timer1 ) ;
		}
		if( pl_char_move( ew ) < 0 ){
			if( ! -- ew->timer0 ){
				ew->routine_1 ++ ;
				pl_char_set( ew, Char_Effect, 4 ) ;
			}
			else if( ew->type ){
				ew->timer1 = shl2b_00_anm_tbl[Rnd() & 15] ;
				pl_char_set( ew, Char_Effect, ew->timer1 + ew->sub_type ) ;
			}
		}
		hit_work_set( ew ) ;
		break ;
	case 2 :
		pl_char_move( ew ) ;
		if( ew->cg_type ){
			ew->routine_1 ++ ;
			ew->spd.x = DOT_X( 8.5f ) ;
			if( ew->disp_dir ) ew->spd.x = -ew->spd.x ;
			ew->spd.y = DOT_Y( 4.5f ) ;
			ew->acc.x = 0.0f ;
			ew->acc.y = 0.0f ;
		}
		return ;
	case 3 :
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		if( !scr_in_ck( ew ) ){
			ew->routine_0 ++ ;
			ew->routine_1 = 0 ;
			ew->disp_flag = 0 ;
			lw->birdie_flag = 0 ;
		}
		return ;
	}
}

/*--------------------------------------------------------------*/
static void shl2b_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->routine_1 ) ew->routine_1 ++ ;
	else work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl2b_00.c" End of File	*/

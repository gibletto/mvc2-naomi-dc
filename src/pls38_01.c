/* Captain Commando's player effect 01: the animation of his frame code that rides on him (the first
   setter), and the spark that flies off beside him and follows his frame codes (the second). */
#include "player.h"
#include "ef_char.h"
#include "pl_char.h"
#include "pl38_def.h"

extern void shl38_02_spd_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;	/* shl38_02.c */

/* stops the effect's animation */
#define PLS38_01_STOP	( vs_sys_work.unk_44 )

static void pls38_01_main( PLAYER_WORK *ew );
static void pls38_01_sword_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls38_01_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls38_01_main ;
		ew->type = ew->sub_type = 0 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x3801 ;
	}
	return ew ;
}

/****************************************************************/
PLAYER_WORK *pls38_01_sword_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS_ST, LIST_TOP ) ) ){
		ew->work_func = pls38_01_sword_main ;
		ew->type = ew->sub_type = 0 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x3801 ;
		ew->obj_flag = pl_w->char_no.w ;
	}
	return ew ;
}

/*==============================================================*/
static void pls38_01_main( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Uint16 no ;

	if( ew->routine_0 >= 2 ){
		work_free( ew ) ;
		return ;
	}
	if( !ew->routine_0 ){
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
		if( Sys->routine_0 == 4 ){
			work_free( ew ) ;
			return ;
		}
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		ew->timer0 = 0 ;
		ew->sub_prio -- ;
	}
	ew->disp_flag = 0 ;
	ew->prio = pl_w->prio ;
	if( pl_w->disp_flag ){
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		ew->disp_dir = pl_w->disp_dir ;
		no = HI_BYTE( pl_w->cg_rival ) ;
		if( no ){
			ew->disp_flag = 1 ;
			if( ew->timer0 != no ){
				ew->timer0 = no ;
				pl_char_set( ew, Char_Effect, no ) ;
			} else if( !PLS38_01_STOP ){
				pl_char_move( ew ) ;
			}
		} else {
			ew->timer0 = no ;
		}
	} else {
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void pls38_01_sword_main( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( ew->routine_0 >= 2 || ew->obj_flag != pl_w->char_no.w ){
		work_free( ew ) ;
		return ;
	}
	if( !ew->routine_0 ){
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
		ew->spd.y = DOT_Y( 160.0f ) ;
		ew->spd.x = DOT_X( 8.0f ) ;
		if( ew->disp_dir ) ew->spd.x = -ew->spd.x ;
		ew->pos.x = pl_w->pos.x + ew->spd.x ;
		ew->pos.y = pl_w->pos.y + ew->spd.y ;
		ew->sub_prio = -8 ;
		ef_char_set( ew, 27, 0 ) ;
	} else if( !ew->routine_1 ){
		ew->pos.x = pl_w->pos.x + ew->spd.x ;
		ew->pos.y = pl_w->pos.y + ew->spd.y ;
		ew->prio = pl_w->prio ;
		ew->sub_prio = -8 ;
		if( ef_char_move( ew ) < 0 ){
			ew->routine_1 ++ ;
			ew->dir = 0 ;
			ef_char_set2( ew, 27, 1, 0 ) ;
		}
	} else if( pl_w->cg_type >= 0 ){
		if( ew->dir != pl_w->cg_type ){
			ew->dir = pl_w->cg_type ;
			ef_char_set2( ew, 27, 1, ew->dir ) ;
			shl38_02_spd_set( ew, pl_w ) ;
		}
		ew->pos.x = pl_w->pos.x + ew->spd.x ;
		ew->pos.y = pl_w->pos.y + ew->spd.y ;
	} else {
		ew->disp_flag = 0 ;
		ew->routine_0 ++ ;
		ew->routine_1 = 0 ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls38_01.c" End of File	*/

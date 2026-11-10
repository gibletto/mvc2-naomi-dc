/* Juggernaut's player effect 00: copies of his sprite, by type (0 to 8), that stand on his
   animation (a type 2 one starts a type 3 one from its frame codes) and end with it. */
#include "player.h"
#include "pl_char.h"

static void pls29_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls29_00_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_END ) ) ){
		ew->work_func = pls29_00_main ;
		ew->type = no ;
		ew->sub_type = 0 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		ew->ef_no = 0x2900 ;
		ew->obj_flag = pl_w->char_no.w ;
	}
	return ew ;
}

static void pls29_00_a( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls29_00_b( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls29_00_c( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls29_00_d( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls29_00_e( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls29_00_main( PLAYER_WORK *ew )
{
	static void ( *pls29_00_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls29_00_a, pls29_00_b, pls29_00_c, pls29_00_d, pls29_00_a,
		pls29_00_a, pls29_00_a, pls29_00_e, pls29_00_a,
	} ;

	PLAYER_WORK *pl_w = ew->master_w ;

	if( ew->routine_0 >= 2 ){
		work_free( ew ) ;
	} else pls29_00_jmp[ew->type]( ew, pl_w ) ;
}

/*==============================================================*/
/*			Types 0, 4, 5, 6, 8: while he is in his				*/
/*			Headcrush's animation (Char_Super 0)				*/
/*==============================================================*/
static void pls29_00_a( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
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
		pl_char_set( ew, Char_Effect, 1 ) ;
	}
	ew->prio = pl_w->prio ;
	ew->sub_prio = -1 ;
	if( pl_w->char_no.b.grp == Char_Super && !pl_w->char_no.b.no ){
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		pl_char_move( ew ) ;
	} else work_free( ew ) ;
}

/*==============================================================*/
/*			Type 1: takes the frame his animation shows			*/
/*==============================================================*/
static void pls29_00_b( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
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
		ew->disp_flag = 0 ;
		ew->sub_prio = -1 ;
	}
	if( pl_w->char_no.w == ew->obj_flag ){
		if( pl_w->cg_type ){
			pl_char_set2( ew, Char_Effect, 2, pl_w->cg_type - 1 ) ;
			ew->pos.x = pl_w->pos.x ;
			ew->pos.y = pl_w->pos.y ;
			ew->prio = pl_w->prio ;
			ew->disp_flag = 1 ;
		} else ew->disp_flag = 0 ;
	} else work_free( ew ) ;
}

/* type 2: follows the frames of his special move's animation (Char_Special); those that ask for it start a type 3 work */
static void pls29_00_c( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	register PLAYER_WORK *ef_w ;
	Sint8 cg ;

	if( !ew->routine_0 ){
		ew->routine_0 ++ ;
		ew->routine_1 = ew->cg_type = 0 ;
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
		ew->disp_flag = 0 ;
		ew->sub_prio = -1 ;
	}
	if( !ew->routine_1 ){
		if( pl_w->char_no.b.grp == Char_Special ){
			ew->pos.x = pl_w->pos.x ;
			ew->pos.y = pl_w->pos.y ;
			ew->prio = pl_w->prio ;
			cg = pl_w->cg_type ;
			if( cg & 0x80 ){
				cg &= 0x7f ;
				if( cg ){
					if( ew->cg_type != cg ){
						ew->disp_flag = 1 ;
						pl_char_set2( ew, Char_Effect, 3, cg ) ;
						if( ew->cg_prm ){
							ew->cg_prm = 0 ;
							if( ( ef_w = work_get( ew, W_PLS, LIST_BEHIND ) ) ){
								ef_w->work_func = pls29_00_main ;
								ef_w->type = 3 ;
								ef_w->sub_type = 0 ;
								ef_w->master_w = pl_w ;
								ef_w->pl_type = pl_w->pl_type ;
								ef_w->pos.x = pl_w->pos.x ;
								ef_w->pos.y = pl_w->pos.y ;
								ef_w->ef_no = 0x2900 ;
							}
						}
					}
					return ;
				}
				ew->routine_1 ++ ;
			} else return ;
		} else {
			work_free( ew ) ;
			return ;
		}
	}
	if( pl_char_move( ew ) < 0 ) work_free( ew ) ;
}

/*==============================================================*/
/*			Type 3: plays on him from the frame it was			*/
/*			started at, and ends with his special move's		*/
/*==============================================================*/
static void pls29_00_d( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->routine_0 ){
		ew->routine_0 ++ ;
		ew->routine_2 = ew->cg_type = 0 ;
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
		ew->disp_flag = 1 ;
		pl_char_set( ew, Char_Effect, 4 ) ;
	}
	ew->prio = pl_w->prio ;
	if( pl_w->char_no.b.grp == Char_Special ){
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		if( pl_char_move( ew ) >= 0 ) return ;
	}
	work_free( ew ) ;
}

/*==============================================================*/
/*			Type 7: a copy that stands beside him, to the		*/
/*			side he faces										*/
/*==============================================================*/
static void pls29_00_e( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
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
		ew->sub_prio = -1 ;
		ew->disp_flag = 1 ;
		pl_char_set( ew, Char_Effect, 8 ) ;
	}
	if( !ew->disp_dir ) ew->pos.x = pl_w->pos.x - DOT_X( 14.0f ) ;
	else ew->pos.x = pl_w->pos.x + DOT_X( 14.0f ) ;
	ew->pos.y = pl_w->pos.y + DOT_Y( 132.0f ) ;
	ew->prio = pl_w->prio ;
	if( pl_char_move( ew ) < 0 ) work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls29_00.c" End of File	*/

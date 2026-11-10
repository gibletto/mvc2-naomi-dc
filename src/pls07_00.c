/* Wolverine's player effect 00: the eight marks that show in his animation groups, by type:
   each one rides on him while the group they belong to lasts. */
#include "player.h"
#include "ef_char.h"

/* the animation each type shows */
static Uint8 pls07_00_char_tbl[] = { 0, 1, 2, 3, 4, 5, 7, 5 } ;

static void pls07_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls07_00_set( PLAYER_WORK *pl_w, int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_END ) ) ){
		ew->ef_no = 0x700 ;
		ew->type = no ;
		ew->work_func = pls07_00_main ;
		ew->master_w = pl_w ;
	}
	return ew ;
}

static void pls07_00_init( PLAYER_WORK *ew );
static void pls07_00_move( PLAYER_WORK *ew );
static void pls07_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls07_00_main( PLAYER_WORK *ew )
{
	static void ( *pls07_00_jmp[] )( PLAYER_WORK *ew ) = {
		pls07_00_init, pls07_00_move, pls07_00_free,
	} ;

	pls07_00_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls07_00_init( PLAYER_WORK *ew )
{
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
	ew->prio = 0 ;
	ew->pos = pl_w->pos ;
	ef_char_set( ew, 27, pls07_00_char_tbl[ew->type] ) ;
}

static void pls07_00_move0( PLAYER_WORK *ew );
static void pls07_00_move1( PLAYER_WORK *ew );
static void pls07_00_move2( PLAYER_WORK *ew );
static void pls07_00_move4( PLAYER_WORK *ew );
static void pls07_00_move5( PLAYER_WORK *ew );
static void pls07_00_move6( PLAYER_WORK *ew );
static void pls07_00_move7( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls07_00_move( register PLAYER_WORK *ew )
{
	static void ( *pls07_00_move_jmp[] )( PLAYER_WORK *ew ) = {
		pls07_00_move0, pls07_00_move1, pls07_00_move2, pls07_00_move2,
		pls07_00_move4, pls07_00_move5, pls07_00_move6, pls07_00_move7,
	} ;

	pls07_00_move_jmp[ew->type]( ew ) ;
}

static int pls07_00_group_ck( PLAYER_WORK *ew, int no );
static void pls07_00_end( PLAYER_WORK *ew );

/*==============================================================*/
static void pls07_00_move0( PLAYER_WORK *ew )
{
	if( ef_char_move( ew ) < 0 ) pls07_00_end( ew ) ;
}

/*==============================================================*/
static void pls07_00_move1( PLAYER_WORK *ew )
{
	if( !( ew->routine_1 || pls07_00_group_ck( ew, 7 ) || pls07_00_group_ck( ew, 20 )
		|| pls07_00_group_ck( ew, 21 ) ) ){
		pls07_00_end( ew ) ;
		return ;
	}
	if( ef_char_move( ew ) < 0 ) pls07_00_end( ew ) ;
}

/*==============================================================*/
static void pls07_00_move2( PLAYER_WORK *ew )
{
	if( !( ew->routine_1 || pls07_00_group_ck( ew, 9 ) || pls07_00_group_ck( ew, 21 ) ) ){
		pls07_00_end( ew ) ;
		return ;
	}
	if( ef_char_move( ew ) < 0 ) pls07_00_end( ew ) ;
}

/*==============================================================*/
static void pls07_00_move4( PLAYER_WORK *ew )
{
	if( !( ew->routine_1 || pls07_00_group_ck( ew, 11 ) ) ){
		pls07_00_end( ew ) ;
		return ;
	}
	if( ef_char_move( ew ) < 0 ) pls07_00_end( ew ) ;
	else ew->pos = ew->master_w->pos ;
}

/*==============================================================*/
static void pls07_00_move5( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_1 != PL_DAMAGE && ( pl_w->wall_flag & WALL_RIGHT ) != pl_w->pl_dir ){
		ew->pos = pl_w->pos ;
		if( !ew->routine_1 ){
			ef_char_move( ew ) ;
			if( pl_w->char_no.b.grp == Char_Super ){
				if( pl_w->char_no.b.no == 10 || pl_w->spd.x == 0.0f ){
					ew->routine_1 ++ ;
					ef_char_set( ew, 27, 6 ) ;
				}
				return ;
			}
		}
		else if( ef_char_move( ew ) >= 0 ) return ;
	}
	pls07_00_end( ew ) ;
}

/*==============================================================*/
static void pls07_00_move6( PLAYER_WORK *ew )
{
	int on ;

	if( !ew->routine_1 ){
		on = pls07_00_group_ck( ew, 23 ) ;
		on |= pls07_00_group_ck( ew, 19 ) ;
		on |= pls07_00_group_ck( ew, 20 ) ;
		if( !on ){
			pls07_00_end( ew ) ;
			return ;
		}
	}
	if( ef_char_move( ew ) < 0 ) pls07_00_end( ew ) ;
	else ew->pos = ew->master_w->pos ;
}

/*==============================================================*/
static void pls07_00_move7( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_1 ) pls07_00_end( ew ) ;
	else{
		ew->pos = pl_w->pos ;
		if( !ew->routine_1 ){
			if( ef_char_move( ew ) < 0 ){
				ew->routine_1 ++ ;
				ef_char_set( ew, 27, 6 ) ;
			}
		}
		else if( ef_char_move( ew ) < 0 ) pls07_00_end( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls07_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			Whether his animation is in the group, a new		*/
/*			frame of the animation moving the mark on			*/
/*==============================================================*/
static int pls07_00_group_ck( PLAYER_WORK *ew, int no )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( ew->cg_type ) ew->routine_1 ++ ;
	return pl_w->char_no.b.grp == no ;
}

/*==============================================================*/
static void pls07_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 = 2 ;
	ew->disp_flag = 0 ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls07_00.c" End of File	*/

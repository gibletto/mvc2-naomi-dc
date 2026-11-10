/* Damage work 04 (work kind 10, 0x0400): the colour flash on a player who is being hit by the
   element 3 (or none), like dm00_03; it runs 150 frames at most and ends with its colour. */
#include "player.h"
#include "pl_color.h"


static void dm00_04_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *dm00_04_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_DM, LIST_TOP ) ) ){
		ew->work_func = dm00_04_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x400 ;
		pl_w->elem_ef_cnt ++ ;
	}
	return ew ;
}

static void dm00_04_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void dm00_04_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void dm00_04_nop( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void dm00_04_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void dm00_04_main( PLAYER_WORK *ew )
{
	static void ( *dm00_04_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		dm00_04_init, dm00_04_move, dm00_04_nop, dm00_04_end,
	} ;

	dm00_04_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

static void dm00_04_spray( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void dm00_04_color( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void dm00_04_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->pl_type = pl_w->pl_type ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->prio = pl_w->prio ;
	pl_se_req( ew, 9 ) ;
	if( pl_w->be_flag ) dm00_04_spray( ew, pl_w ) ;
	ew->timer0 = 0x96 ;
	dm00_04_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void dm00_04_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Uint8 elem ;

	if( pl_w->be_flag ){
		elem = pl_w->dm_elem ;
		if( elem == 3 || !elem ){
			if( ew->pl_type == pl_w->pl_type ){
				if( --ew->timer0 ){
					if( pl_w->routine_1 == PL_DAMAGE || pl_w->routine_1 == PL_CAUGHT ){
						dm00_04_color( ew, pl_w ) ;
						return ;
					}
				}
			}
		}
	}
	dm00_04_end( ew, pl_w ) ;
}

/*==============================================================*/
static void dm00_04_spray( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	thunder_spark_set( pl_w ) ;
}

/*==============================================================*/
static void dm00_04_color( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_color_set( pl_w, 4 ) ;
}

/*--------------------------------------------------------------*/
static void dm00_04_nop( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
}

/*--------------------------------------------------------------*/
static void dm00_04_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->dm_elem == 3 ){
		if( --pl_w->elem_ef_cnt < 0 ) pl_w->elem_ef_cnt = 0 ;
	}
	pl_color_set( pl_w, 1 ) ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "dm00_04.c" End of File	*/

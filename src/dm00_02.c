/* Damage work 02 (work kind 10, 0x0200): the colour flash on a player who is being hit by the
   element 1 (or none), made as he is struck; it runs 150 frames at most and ends with its colour. */
#include "player.h"
#include "pl_color.h"

extern void dm_ef1_set( PLAYER_WORK *pl_w );

static void dm00_02_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *dm00_02_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_DM, LIST_TOP ) ) ){
		ew->work_func = dm00_02_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x200 ;
		pl_w->elem_ef_cnt ++ ;
	}
	return ew ;
}

static void dm00_02_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void dm00_02_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void dm00_02_color( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void dm00_02_nop( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void dm00_02_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void dm00_02_main( PLAYER_WORK *ew )
{
	static void ( *dm00_02_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		dm00_02_init, dm00_02_move, dm00_02_nop, dm00_02_end,
	} ;

	dm00_02_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void dm00_02_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->pl_type = pl_w->pl_type ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->prio = pl_w->prio ;
	ew->timer0 = 0 ;
	ew->timer1 = 0x96 ;
	pl_se_req( ew, 80 ) ;
	dm_ef1_set( pl_w ) ;
	dm00_02_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void dm00_02_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Uint8 elem ;

	if( pl_w->be_flag ){
		elem = pl_w->dm_elem ;
		if( elem == 1 || !elem ){
			if( ew->pl_type == pl_w->pl_type ){
				if( --ew->timer1 >= 0 ){
					if( pl_w->routine_1 == PL_DAMAGE || pl_w->routine_1 == PL_CAUGHT ){
						dm00_02_color( ew, pl_w ) ;
						if( --ew->timer0 < 0 ){
							ew->timer0 = ( Rnd() & 3 ) + 5 ;
							if( work_rest_cnt <= 32 ) return ;
						}
						return ;
					}
				}
			}
		}
	}
	dm00_02_end( ew, pl_w ) ;
}

/*==============================================================*/
static void dm00_02_color( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_color_set( pl_w, 7 ) ;
}

/*--------------------------------------------------------------*/
static void dm00_02_nop( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
}

/*--------------------------------------------------------------*/
static void dm00_02_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->dm_elem == 1 ){
		if( --pl_w->elem_ef_cnt < 0 ) pl_w->elem_ef_cnt = 0 ;
	}
	pl_color_set( pl_w, 1 ) ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "dm00_02.c" End of File	*/

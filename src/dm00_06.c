/* Damage work 06 (work kind 10, 0x0600): shakes the player a throw holds (and the work riding on
   him) a few units left and right every other frame while the thrower is in the act, and
   flashes his colour. Type 0 does this, the others only wait and end. */
#include "player.h"
#include "pl_color.h"
#include "pl0b_def.h"

static void dm00_06_main( PLAYER_WORK *ew );

/****************************************************************/
void dm00_06_set( PLAYER_WORK *pl_w, PLAYER_WORK *w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_DM, LIST_TOP ) ) ){
		ew->work_func = dm00_06_main ;
		ew->master_w = pl_w ;
		ew->ef_w = w ;
		ew->type = type ;
		ew->ef_no = 0x600 ;
	}
}

static void dm00_06_init( PLAYER_WORK *ew );
static void dm00_06_move( PLAYER_WORK *ew );
static void dm00_06_end( PLAYER_WORK *ew );
static void dm00_06_free( PLAYER_WORK *ew );
static void dm00_06_main2( PLAYER_WORK *ew );
static void dm00_06_nop( PLAYER_WORK *ew );
static void dm00_06_nop2( PLAYER_WORK *ew );
static void dm00_06_stop( PLAYER_WORK *ew );

static void dm00_06_main1( PLAYER_WORK *ew );

/*==============================================================*/
static void dm00_06_main( PLAYER_WORK *ew )
{
	if( !ew->type ) dm00_06_main1( ew ) ;
	else dm00_06_main2( ew ) ;
}

/*==============================================================*/
/*			The routine table of type 0; main reaches it by		*/
/*			a tail jump											*/
/*==============================================================*/
static void dm00_06_main1( PLAYER_WORK *ew )
{
	static void ( *dm00_06_jmp[] )( PLAYER_WORK *ew ) = {
		dm00_06_init, dm00_06_move, dm00_06_end, dm00_06_free,
	} ;

	dm00_06_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void dm00_06_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->timer0 = 0 ;
	ew->timer1 = 0 ;
	ew->dir = 0 ;
	dm00_06_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void dm00_06_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *dm_w ;
	PL0B_LOCAL_W *lw ;
	PLAYER_WORK *w ;
	Float x ;

	if( pl_w->routine_1 || pl_w->act_st != N_SPECIAL || ( pl_w->routine_2 != 9 && pl_w->routine_2 != 10 ) ){
		dm00_06_end( ew ) ;
		return ;
	}
	if( ++ew->timer0 & 1 ){
		x = DOT_X( 2.0f ) ;
		if( pl_w->disp_dir ) x = -DOT_X( 2.0f ) ;
		dm_w = pl_w->catch_w ;
		if( PL_CK( dm_w, PL_CAPTAIN_AMERICA ) ){
			lw = ( PL0B_LOCAL_W * )&dm_w->tlocal[0] ;
			if( !lw->shield_st ){
				w = lw->shield_w ;
				w->pos.x += x ;
			}
		}
		dm_w->pos.x += x ;
		w = ew->ef_w ;
		if( w ) w->pos.x += x ;
	}
	if( --ew->timer1 > 0 ) return ;
	ew->timer1 = 4 ;
	pl_color_set( dm_w, 5 ) ;
}

/*--------------------------------------------------------------*/
static void dm00_06_end( PLAYER_WORK *ew )
{
	ew->routine_0 = 3 ;
	pl_color_set( ew->master_w->catch_w, 1 ) ;
}

/*==============================================================*/
static void dm00_06_main2( PLAYER_WORK *ew )
{
	static void ( *dm00_06_jmp2[] )( PLAYER_WORK *ew ) = {
		dm00_06_nop, dm00_06_nop2, dm00_06_stop, dm00_06_free,
	} ;

	dm00_06_jmp2[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void dm00_06_nop( PLAYER_WORK *ew )
{
}

/*--------------------------------------------------------------*/
static void dm00_06_nop2( PLAYER_WORK *ew )
{
}

/*--------------------------------------------------------------*/
static void dm00_06_stop( PLAYER_WORK *ew )
{
	ew->routine_0 = 3 ;
}

/*--------------------------------------------------------------*/
static void dm00_06_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "dm00_06.c" End of File	*/

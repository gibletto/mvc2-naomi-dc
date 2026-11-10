/* Damage work 08_00 (work kind 10, 0x0800): a debug caption for the end of a round. Once the
   flash is over it prints "K.O" for 60 frames, or ends at once when the round was a draw. Nothing
   calls its setter. */
#include "player.h"

extern int sprintf( char *buf, const char *fmt, ... );

static void dm08_00_main( PLAYER_WORK *ew );

/****************************************************************/
void dm08_00_set( Uint8 type, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get2( 0, W_DM, LIST_END ) ) ){
		ew->work_func = dm08_00_main ;
		ew->type = type ;
		ew->sub_type = no ;
		ew->ef_no = 0x800 ;
	}
}

static void dm08_00_main1( PLAYER_WORK *ew );

/*==============================================================*/
static void dm08_00_main( PLAYER_WORK *ew )
{
	static void ( *dm08_00_jmp[] )( PLAYER_WORK *ew ) = {
		dm08_00_main1,
	} ;

	dm08_00_jmp[ew->type]( ew ) ;
}

static void dm08_00_init( PLAYER_WORK *ew );
static void dm08_00_move( PLAYER_WORK *ew );
static void dm08_00_stop( PLAYER_WORK *ew );
static void dm08_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void dm08_00_main1( PLAYER_WORK *ew )
{
	static void ( *dm08_00_jmp1[] )( PLAYER_WORK *ew ) = {
		dm08_00_init, dm08_00_move, dm08_00_stop, dm08_00_free,
	} ;

	dm08_00_jmp1[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void dm08_00_init( PLAYER_WORK *ew )
{
	if( flash_work.timer ) return ;
	if( vs_sys_w->winner_id < 0 ){
		ew->routine_0 = 2 ;
		return ;
	}
	ew->routine_0 ++ ;
	ew->timer0 = 60 ;
}

/*--------------------------------------------------------------*/
static void dm08_00_move( PLAYER_WORK *ew )
{
	char buf[16] ;

	if( --ew->timer0 > 0 ){
		sprintf( buf, "K.O" ) ;
		STrPrint( 35, 25, 7, buf ) ;
	} else {
		ew->routine_0 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void dm08_00_stop( PLAYER_WORK *ew )
{
	ew->routine_0 = 3 ;
}

/*--------------------------------------------------------------*/
static void dm08_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "dm08_00.c" End of File	*/

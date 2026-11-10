/* The developers' switch menu: sixty named switches (SYSTEM, AUTO GUARD, NO DAMAGE...) that work
   while the DEVELOP switch is on. */
#include "player.h"

#define DBG_ROWS	15
#define DBG_DEVELOP_ROW	12	/* DEVELOP, the first switch of the row */

/* the menu's cursor */
static Sint8 dbg_col ;
static Sint8 dbg_row ;

static char dbg_title[19][16] = {
	"HELLO",
	"****", "****", "****", "****", "****", "****", "****", "****", "****", "****",
	"****", "****", "****", "****", "****",
} ;

static char dbg_name[DBG_ROWS * 4][16] = {
	"SYSTEM", "AUTO GUARD", "GAUGE_MAX", "NO ASSIST",
	"LOAD BAR", "LOAD BAR 2", "WORK CHAIN", "LAST STAGE",
	"HIT FRAME", "SOUND CODE", "CAMERA", "stage",
	"SLOW 1", "NML GRID", "TECH HIT", "list(set)",
	"SLOW 2", "CPS GRID", "UKEMI", "list(efct)",
	"TIME STOP", "stage GRID", "VITAL 0(1p)", "gauge",
	"NO DAMAGE", "player GRID", "VITAL 1(2p)", "MUSIC OFF",
	"VITAL FULL", "TIMER 3", "----", "----",
	"-----", "-----", "-----", "-----",
	"-----", "-----", "-----", "-----",
	"-----", "-----", "-----", "-----",
	"-----", "-----", "-----", "-----",
	"DEVELOP", "CPU VS", "CPU SEL", "PON1",
	"PUB_ROM", "CPU WARI", "NEXT ROUND", "LEADER",
	"COMBO CK", "CPU ONI", "MINORU", "----",
} ;

/****************************************************************/
/*			All switches off									*/
/****************************************************************/
void dbg_flag_init( void )
{
	int i, j ;

	dbg_col = 0 ;
	dbg_row = 0 ;
	for( i = 0 ; i < 12 ; i ++ ){
		for( j = 0 ; j < 4 ; j ++ ) dbg_flag[i * 4 + j] = 0 ;
	}
	for( i = 12 ; i < DBG_ROWS ; i ++ ){
		for( j = 0 ; j < 4 ; j ++ ) dbg_flag[i * 4 + j] = 0 ;
	}
}

/****************************************************************/
/*			A switch, when the DEVELOP switch is on				*/
/****************************************************************/
int dbg_flag_ck( Sint8 col, Sint8 row )
{
	if( !Sys->develop_flag ) return 0 ;
	return dbg_flag[row * 4 + col] ;
}

#define SW_NEW( sw )	( ( sw )[0].on | ( sw )[1].on | ( sw )[2].on | ( sw )[3].on )
#define SW_REP( sw )	( ( sw )[0].rep | ( sw )[1].rep | ( sw )[2].rep | ( sw )[3].rep )

/****************************************************************/
/*			Move the menu's cursor with any player's stick,		*/
/*			and turn a switch over with a button				*/
/****************************************************************/
void dbg_menu_move( void )
{
	if( ( SW_NEW( sw_work ) | SW_REP( sw_work ) ) & 0x2000 ) dbg_row -- ;
	if( ( SW_NEW( sw_work ) | SW_REP( sw_work ) ) & 0x1000 ) dbg_row ++ ;
	if( ( SW_NEW( sw_work ) | SW_REP( sw_work ) ) & 0x800 ) dbg_col -- ;
	if( ( SW_NEW( sw_work ) | SW_REP( sw_work ) ) & 0x400 ) dbg_col ++ ;
	dbg_col &= 3 ;
	dbg_row = ( dbg_row + DBG_ROWS ) % DBG_ROWS ;
	if( SW_NEW( sw_work ) & 0x40 ) dbg_flag[dbg_row * 4 + dbg_col] ^= 1 ;
}

/****************************************************************/
/*			The menu: each switch's state and name, the			*/
/*			cursor's one marked									*/
/****************************************************************/
void dbg_menu_disp( void )
{
	register Uint32 col ;
	Sint8 row, i, line ;
	Sint8 c ;

	if( Sys->develop_flag != 0 ) col = 0xff3f3f7f ;
	else col = 0xff7f3f3f ;
	nlSetBackColor( col, col, col ) ;
	line = 0 ;
	for( row = 0 ; row < DBG_ROWS ; row ++ ){
		for( i = 0 ; i < 4 ; i ++ ){
			c = 7 ;
			if( i == dbg_col && row == dbg_row ){
				STrPrint( i * 16 + 3, line * 2 + 4, 8, ">" ) ;
				c = 9 ;
			}
			STrPrint( i * 16 + 4, line * 2 + 4, c, "%1d %s", dbg_flag[row * 4 + i], dbg_name[row * 4 + i] ) ;
		}
		line ++ ;
		if( line == 8 ) line ++ ;
		if( line == 13 ) line ++ ;
	}
}

/****************************************************************/
/*			The first player's start button opens the menu;		*/
/*			closing it sets the DEVELOP switch					*/
/****************************************************************/
void dbg_menu_ck( void )
{
	if( dbg_mode & 1 ){
		if( sw_work[0].on == KEY_LP ){
			dbg_mode ^= 1 ;
			Sys->develop_flag = dbg_flag[DBG_DEVELOP_ROW * 4] ;
		}
	} else if( sw_work[0].on == KEY_LP && ( sw_work[0].now & KEY_START ) ){
		dbg_mode ^= 1 ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "sys_debug.c" End of File	*/

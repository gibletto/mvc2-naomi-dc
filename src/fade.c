/* The screen fade: a colour drawn over the screen whose alpha steps to or from nothing over a
   number of frames, run by System_Init's frame loop. */
#include "player.h"


/****************************************************************/
/*			Stop the fade										*/
/****************************************************************/
void fade_init( void )
{
	memset( &fade_work, 0, sizeof( fade_work ) ) ;
}

/****************************************************************/
/*			Start a fade of colour col over time frames, out	*/
/*			to cover the screen or not to clear it				*/
/****************************************************************/
void fade_set( int col, int time, int out )
{
	FADE_WORK *fw = &fade_work ;

	fw->on = 1 ;
	fw->col = col ;
	fw->cnt = time ;
	fw->time = time ;
	fw->out = out ;
}

/****************************************************************/
/*			Step the fade and have it drawn						*/
/****************************************************************/
void fade_move( void )
{
	Uint32 alpha ;

	if( !fade_work.on ) return ;
	if( fade_work.cnt > 0 ) fade_work.cnt -- ;
	if( !fade_work.cnt && !fade_work.out ){
		fade_work.on = 0 ;
		return ;
	}
	if( !fade_work.out ) alpha = fade_work.cnt * 255 / fade_work.time ;
	else alpha = ( fade_work.time - fade_work.cnt ) * 255 / fade_work.time ;
	fade_work.col &= 0xffffff ;
	fade_work.col |= ( Uint8 )alpha << 24 ;
	SysTransSet( fade_disp ) ;
}

/****************************************************************/
/*			The frames the fade has left						*/
/****************************************************************/
Uint32 fade_cnt( void )
{
	return fade_work.cnt ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "fade.c" End of File	*/

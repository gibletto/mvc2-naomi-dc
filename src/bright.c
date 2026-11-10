/* The screen's brightness: the scales the background and the fog colours are drawn with, halved
   while the game is stopped. */
#include "player.h"

/****************************************************************/
/*			All bright											*/
/****************************************************************/
void bright_init( void )
{
	memset( &bright_work, 0, sizeof( bright_work ) ) ;
	bright_work.rate[1] = bright_work.rate[2] = bright_work.rate[3] = 1.0f ;
}

/****************************************************************/
/*			The same scale for red, green and blue				*/
/****************************************************************/
void bright_set( Float rate )
{
	bright_work.rate[1] = bright_work.rate[2] = bright_work.rate[3] = rate ;
}

/****************************************************************/
/*			Half bright while the game is stopped				*/
/****************************************************************/
void bright_stop_set( void )
{
	Float rate ;

	if( !vs_sys_work.game_stop ) rate = 1.0f ;
	else rate = 0.5f ;
	bright_set( rate ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "bright.c" End of File	*/

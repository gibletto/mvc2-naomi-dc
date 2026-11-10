/* The screen's flash: a kind, a colour and a time set by the fight's events, and each frame's
   flash, lit, blinking or dark, that flash_work.flag gives the screen. */
#include "player.h"

#define WHITE	0xffffff

/****************************************************************/
/*			A white blink, if the flashes are on and none is	*/
/*			running												*/
/****************************************************************/
void flash_blink_set( void )
{
	if( !Sys->no_flash_flag ){
		if( !flash_work.timer ){
			flash_work.type = 2 ;
			flash_work.timer = 2 ;
			flash_work.col = WHITE ;
		}
	}
}

/****************************************************************/
/*			A white blink of 16 frames							*/
/****************************************************************/
void flash_white_set( void )
{
	flash_work.type = 1 ;
	flash_work.timer = 16 ;
	flash_work.col = WHITE ;
}

/****************************************************************/
/*			The white blink of a KO, if the flashes are on		*/
/****************************************************************/
void flash_ko_set( void )
{
	if( Sys->no_flash_flag ) return ;
	flash_work.type = 6 ;
	flash_work.timer = 24 ;
	flash_work.col = WHITE ;
}

void flash_long_on( void ) ;

/****************************************************************/
/*			Two seconds lit, if the flashes are on				*/
/****************************************************************/
void flash_long_set( void )
{
	if( Sys->no_flash_flag ) return ;
	flash_long_on() ;
}

/****************************************************************/
/*			Two seconds lit										*/
/****************************************************************/
void flash_long_on( void )
{
	flash_work.type = 0 ;
	flash_work.timer = 120 ;
	flash_work.col = 0xffffffff ;
}

/****************************************************************/
/*			Two seconds dark									*/
/****************************************************************/
void flash_dark_long_set( void )
{
	flash_work.type = 0 ;
	flash_work.timer = 120 ;
	flash_work.col = 0 ;
}

/****************************************************************/
/*			Half a second dark									*/
/****************************************************************/
void flash_dark_set( void )
{
	flash_work.type = 0 ;
	flash_work.timer = 30 ;
	flash_work.col = 0 ;
}

/****************************************************************/
/*			The finish of a fight won with a hyper combo		*/
/****************************************************************/
void flash_finish_set( void )
{
	flash_work.type = 8 ;
	flash_work.timer = 185 ;
	flash_work.col = 0 ;
}

/****************************************************************/
/*			The finish before the ending, unless the other		*/
/*			is running											*/
/****************************************************************/
void flash_ending_set( void )
{
	if( flash_work.type == 8 ) return ;
	flash_work.type = 9 ;
	flash_work.timer = 180 ;
	flash_work.col = 0 ;
}

/*==============================================================*/
static void flash_on( void )
{
	flash_work.timer -- ;
	flash_work.flag = 1 ;
}

/*==============================================================*/
static void flash_blink_1( void )
{
	flash_work.timer -- ;
	if( flash_work.flag = flash_work.timer & 1 ){ }
}

/*==============================================================*/
static void flash_blink_2( void )
{
	flash_work.timer -- ;
	if( flash_work.flag = flash_work.timer >> 1 & 1 ){ }
}

/*==============================================================*/
static void flash_finish( void )
{
	flash_work.timer -- ;
	flash_work.flag = 2 ;
}

/*==============================================================*/
static void flash_ending( void )
{
	flash_work.timer -- ;
	flash_work.flag = 3 ;
}

/****************************************************************/
/*			This frame's flash									*/
/****************************************************************/
void flash_move( void )
{
	if( flash_work.timer ){
		switch( flash_work.type ){
		case 0:
			flash_on() ;
			break ;
		case 1:
			flash_blink_2() ;
			break ;
		case 2:
			flash_blink_1() ;
			break ;
		case 3:
		case 4:
		case 5:
		case 6:
			flash_blink_2() ;
			break ;
		case 7:
		case 8:
			flash_finish() ;
			break ;
		case 9:
			flash_ending() ;
			break ;
		case 10:
		case 11:
		case 12:
			break ;
		}
	} else flash_work.flag = 0 ;
}

/****************************************************************/
/*			The flash this frame, 0 for none					*/
/****************************************************************/
Sint32 flash_flag_get( void )
{
	if( flash_work.flag ) return flash_work.flag ;
	return 0 ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "flashing.c" End of File	*/

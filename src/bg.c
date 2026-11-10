/* The stage's objects: setting them up for a fight, and the angles they turn by, stepped each
   frame. */
#include "player.h"

extern void bg_obj0_set( void ) ;
extern void gauge_init( void ) ;
extern void name_init( void ) ;
extern void ef12_init( void ) ;
extern void bg_hud_set( void ) ;

/****************************************************************/
/*			Set up the stage's objects							*/
/****************************************************************/
void bg_init( void )
{
	bg_ang0 = bg_ang1 = bg_ang2 = bg_ang3 = 0 ;
	bg_obj0_set() ;
	gauge_init() ;
	name_init() ;
	ef12_init() ;
	bg_hud_set() ;
}

/****************************************************************/
/*			Step the stage's angles								*/
/****************************************************************/
void bg_move( void )
{
	bg_ang0 += 0x1000 ;
	bg_ang1 += 0x800 ;
	bg_ang2 += 0x400 ;
	bg_ang3 += 0x200 ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "bg.c" End of File	*/

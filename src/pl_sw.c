/* The players' buttons for the frame. */
#include "player.h"

/****************************************************************/
/*			Give each player work its side's buttons for the	*/
/*			frame												*/
/****************************************************************/
void pl_sw_copy( void )
{
	int i ;

	for( i = 0 ; i < 6 ; i ++ ) pl_work[i].side_sw = sw_work[i & 1] ;
	for( i = 0 ; i < 6 ; i ++ ) pl_work[i].pad_sw = sw_work[i & 1] ;
}

/****************************************************************/
/*			The buttons a player holds							*/
/****************************************************************/
int pl_button_ck( PLAYER_WORK *pl_w )
{
	int sw ;

	sw = pl_w->pad_sw.now & KEY_BUTTON ;
	return sw ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pl_sw.c" End of File	*/

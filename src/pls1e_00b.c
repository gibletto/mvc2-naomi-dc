/* Akuma's player effect 00: its end and free, which come after a pool of their own. */
#include "player.h"

void pls1e_00_free( PLAYER_WORK *ew );

/****************************************************************/
void pls1e_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	pls1e_00_free( ew ) ;
}

/****************************************************************/
void pls1e_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls1e_00b.c" End of File	*/

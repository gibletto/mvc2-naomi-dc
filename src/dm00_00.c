/* Damage work 00 (work kind 10, 0x0500): while the player hangs on the wall in his wall dash,
   it keeps him one wall's width in from the screen's centre. */
#include "player.h"
#include "pl05_def.h"

static PL05_LOCAL_W *dm00_00_lw_p ;

static void dm00_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *dm00_00_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_DM, LIST_TOP ) ) ){
		ew->work_func = dm00_00_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = 0 ;
		ew->ef_no = 0x500 ;
	}
	return ew ;
}

/*==============================================================*/
static void dm00_00_main( PLAYER_WORK *ew )
{
	Float x ;

	dm00_00_lw_p = ( PL05_LOCAL_W * )ew->master_w->tlocal ;
	if( ew->pl_type != ew->master_w->pl_type ){
		work_free( ew ) ;
		return ;
	}
	if( ew->master_w->act_st != N_SPECIAL ){
		work_free( ew ) ;
		return ;
	}
	if( ew->master_w->char_no.b.grp != Char_Extra ){
		work_free( ew ) ;
		return ;
	}
	if( !dm00_00_lw_p->pl05_wall_flag1 ){
		work_free( ew ) ;
		return ;
	}
	x = scr_work.eye[0].x + ( ew->master_w->pl_dir ? -285.0f : 285.0f ) ;
	ew->master_w->pos.x = x ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "dm00_00.c" End of File	*/

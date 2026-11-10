/* Dhalsim's effect 02 (character 25's shot 02): the puff of smoke that rises from his mouth, drifting
   up and a little forward, and blinks by side. */
#include "player.h"
#include "pl_char.h"
#include "pl25_def.h"

static void shl25_02_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl25_02_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl25_02_main ;
		ew->master_w = pl_w ;
	}
	return ew ;
}

static void shl25_02_init( PLAYER_WORK *ew );
static void shl25_02_move( PLAYER_WORK *ew );
static void shl25_02_end( PLAYER_WORK *ew );
static void shl25_02_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl25_02_main( PLAYER_WORK *ew )
{
	static void ( *shl25_02_jmp[] )( PLAYER_WORK *ew ) = {
		shl25_02_init, shl25_02_move, shl25_02_end, shl25_02_free,
	} ;

	shl25_02_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl25_02_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->prio = 11 ;
	ew->disp_flag = 1 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	pl_char_set( ew, Char_Effect, 0 ) ;
	shl25_02_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl25_02_move( PLAYER_WORK *ew )
{
	if( ew->cg_type ){
		ew->pos.y += -2.142857f ;
		if( !ew->disp_dir ) ew->pos.x += DOT_X( 0.5f ) ;
		else ew->pos.x += -DOT_X( 0.5f ) ;
		ew->disp_flag = ( Sys->game_timer0 & 1 ) ^ ew->pl_no ;
	}
	if( pl_char_move( ew ) < 0 ) shl25_02_end( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl25_02_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl25_02_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl25_02.c" End of File	*/

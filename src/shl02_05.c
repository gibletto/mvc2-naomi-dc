/* Guile's effect 05 (character 02's shot 05): an animation that slides out in front of him like
   shl02_04's shot, but without hitting, until it has played. */
#include "player.h"
#include "pl_char.h"

static void shl02_05_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl02_05_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl02_05_main ;
		ew->master_w = pl_w ;
	}
	return ew ;
}

static void shl02_05_init( PLAYER_WORK *ew );
static void shl02_05_move( PLAYER_WORK *ew );
static void shl02_05_end( PLAYER_WORK *ew );
static void shl02_05_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl02_05_main( PLAYER_WORK *ew )
{
	static void ( *shl02_05_jmp[] )( PLAYER_WORK *ew ) = {
		shl02_05_init, shl02_05_move, shl02_05_end, shl02_05_free,
	} ;

	shl02_05_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl02_05_init( PLAYER_WORK *ew )
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
	ew->spd.x = DOT_X( 8.0f ) ;
	ew->acc.x = -DOT_X( 0.125f ) ;
	ew->pos.x = ew->master_w->pos.x ;
	if( !ew->disp_dir ){
		ew->spd.x = -ew->spd.x ;
		ew->acc.x = -ew->acc.x ;
	}
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.y += 120.0f ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->prio = 11 ;
	pl_char_set( ew, Char_Effect, 7 ) ;
	shl02_05_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl02_05_move( PLAYER_WORK *ew )
{
	ew->spd.x += ew->acc.x ;
	ew->pos.x += ew->spd.x ;
	if( pl_char_move( ew ) < 0 ) shl02_05_end( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl02_05_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl02_05_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl02_05.c" End of File	*/

/* Guile's effect 03 (character 02's shot 03): an animation on him as he throws a Sonic Boom,
   played once. */
#include "player.h"
#include "pl_char.h"

static void shl02_03_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl02_03_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl02_03_main ;
		ew->master_w = pl_w ;
	}
	return ew ;
}

static void shl02_03_init( PLAYER_WORK *ew );
static void shl02_03_move( PLAYER_WORK *ew );
static void shl02_03_end( PLAYER_WORK *ew );
static void shl02_03_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl02_03_main( PLAYER_WORK *ew )
{
	static void ( *shl02_03_jmp[] )( PLAYER_WORK *ew ) = {
		shl02_03_init, shl02_03_move, shl02_03_end, shl02_03_free,
	} ;

	shl02_03_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl02_03_init( PLAYER_WORK *ew )
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
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	if( ew->disp_dir ) ew->pos.x += 0.0f ;
	else ew->pos.x -= 0.0f ;
	ew->prio = 11 ;
	pl_char_set( ew, Char_Effect, 6 ) ;
	shl02_03_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl02_03_move( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ) shl02_03_end( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl02_03_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl02_03_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl02_03.c" End of File	*/

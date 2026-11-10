/* Guile's effect 02 (character 02's shot 02): an animation in front of him that keeps step with
   his own, by the code in the frames' cg_rival. */
#include "player.h"
#include "pl_char.h"

static void shl02_02_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl02_02_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl02_02_main ;
		ew->master_w = pl_w ;
	}
	return ew ;
}

static void shl02_02_init( PLAYER_WORK *ew );
static void shl02_02_move( PLAYER_WORK *ew );
static void shl02_02_end( PLAYER_WORK *ew );
static void shl02_02_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl02_02_main( PLAYER_WORK *ew )
{
	static void ( *shl02_02_jmp[] )( PLAYER_WORK *ew ) = {
		shl02_02_init, shl02_02_move, shl02_02_end, shl02_02_free,
	} ;

	shl02_02_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl02_02_init( PLAYER_WORK *ew )
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
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	if( ew->disp_dir ) ew->pos.x += DOT_X( 32.0f ) ;
	else ew->pos.x -= DOT_X( 32.0f ) ;
	ew->scale.x += 0.03f ;
	ew->scale.y += 0.03f ;
	ew->prio = 11 ;
	shl02_02_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl02_02_move( PLAYER_WORK *ew )
{
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	if( ew->disp_dir ) ew->pos.x += DOT_X( 32.0f ) ;
	else ew->pos.x -= DOT_X( 32.0f ) ;
	pl_char_set( ew, Char_Effect, 3 ) ;
	for( ;; ){
		if( HI_BYTE( ew->master_w->cg_rival ) == HI_BYTE( ew->cg_rival ) ){
			ew->disp_flag = 1 ;
			break ;
		}
		if( pl_char_move( ew ) < 0 ){
			shl02_02_end( ew ) ;
			break ;
		}
	}
}

/*--------------------------------------------------------------*/
static void shl02_02_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl02_02_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl02_02.c" End of File	*/

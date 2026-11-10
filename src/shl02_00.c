/* Guile's Sonic Boom (character 02's shot 00): it flies straight until it hits something or leaves
   the screen. */
#include "player.h"
#include "pl_char.h"
#include "pl02_def.h"

static void shl02_00_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl02_00_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl02_00_main ;
		ew->master_w = pl_w ;
	}
	return ew ;
}

static void shl02_00_init( PLAYER_WORK *ew );
static void shl02_00_move( PLAYER_WORK *ew );
static void shl02_00_hit( PLAYER_WORK *ew );
static void shl02_00_end( PLAYER_WORK *ew );
static void shl02_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl02_00_main( PLAYER_WORK *ew )
{
	static void ( *shl02_00_jmp[] )( PLAYER_WORK *ew ) = {
		shl02_00_init, shl02_00_move, shl02_00_hit, shl02_00_end, shl02_00_free,
	} ;

	shl02_00_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl02_00_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PL02_LOCAL_W *lw = ( PL02_LOCAL_W * )pl_w->tlocal ;

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
	ew->pos.y += DOT_Y( 64.0f ) ;
	ew->pos.z = ew->master_w->pos.z ;
	if( ew->disp_dir ) ew->pos.x += DOT_X( 64.0f ) ;
	else ew->pos.x -= DOT_X( 64.0f ) ;
	if( !ew->atck_level ) ew->spd.x = -5.0f ;
	else ew->spd.x = -DOT_X( 5.0f ) ;
	if( ew->disp_dir ) ew->spd.x = -ew->spd.x ;
	ew->prio = 0 ;
	ew->atck_no = ew->atck_level + 48 ;
	HIT_RESET( ew ) ;
	lw->shot_flag = 1 ;
	pl_char_set( ew, Char_Effect, ew->atck_level ) ;
	shl02_00_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl02_00_move( PLAYER_WORK *ew )
{
	if( !scr_in_x_ck( ew ) ){
		shl02_00_end( ew ) ;
		return ;
	}
	if( ew->hit_flag || ew->hit_box_no ){
		ew->routine_0 ++ ;
		ew->char_no.b.grp = 23 ;
		ew->char_no.b.no = 2 ;
		pl_char_set( ew, ew->char_no.b.grp, ew->char_no.b.no ) ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	hit_work_set( ew ) ;
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl02_00_hit( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ) shl02_00_end( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl02_00_end( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PL02_LOCAL_W *lw = ( PL02_LOCAL_W * )pl_w->tlocal ;

	lw->shot_flag = 0 ;
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl02_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl02_00.c" End of File	*/

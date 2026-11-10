/* Sentinel's effect 02 (character 34's shot 02): the shot a drone (effect 01) drops. It falls from
   the drone and bursts when it hits or reaches the floor. */
#include "player.h"
#include "pl_char.h"
#include "pl34_def.h"

/* the effect's own work, at obj_mtx */
typedef struct SHL34_02_LOCAL_W {
	Float	ofs[2];		/* where it starts from the drone */
} SHL34_02_LOCAL_W;
#define SHL34_02_LW( w )	( ( SHL34_02_LOCAL_W * )&( w )->obj_mtx )

static void shl34_02_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl34_02_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( pl_w, W_SHL, LIST_BEHIND ) ) ){
		ew->work_func = shl34_02_main ;
		ew->master_w = pl_w->master_w ;
		ew->ef_w = pl_w ;
		ew->ef_no = 0x3402 ;
	}
	return ew ;
}

static void shl34_02_init( PLAYER_WORK *ew, PLAYER_WORK *ef_w );
static void shl34_02_move( PLAYER_WORK *ew, PLAYER_WORK *ef_w );
static void shl34_02_end( PLAYER_WORK *ew, PLAYER_WORK *ef_w );
static void shl34_02_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl34_02_main( PLAYER_WORK *ew )
{
	static void ( *shl34_02_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *ef_w ) = {
		shl34_02_init, shl34_02_move, shl34_02_end, shl34_02_free,
	} ;

	shl34_02_jmp[ew->routine_0]( ew, ew->ef_w ) ;
}

/*==============================================================*/
static void shl34_02_init( PLAYER_WORK *ew, PLAYER_WORK *ef_w )
{
	SHL34_02_LOCAL_W *lw = SHL34_02_LW( ew ) ;
	PLAYER_WORK *pl_w = ew->master_w ;

	BODY_SIZE_SET( ew, 0x8080808 ) ;
	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, pl_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = pl_w->pl_no ;
	ew->pl_type = pl_w->pl_type ;
	ew->scale.x = pl_w->scale.x ;
	ew->scale.y = pl_w->scale.y ;
	ew->atck_level = pl_w->atck_level ;
	ew->work_no = pl_w->work_no ;
	ew->col_cnt = pl_w->col_cnt ;
	ew->scale = pl_w->scale ;
	ew->prio = pl_w->prio ;
	lw->ofs[0] = 25.0f ;
	lw->ofs[1] = 30.0f ;
	if( !ef_w->disp_dir ) lw->ofs[0] = -lw->ofs[0] ;
	ew->pos.x = ef_w->pos.x + lw->ofs[0] ;
	ew->pos.y = ef_w->pos.y + lw->ofs[1] ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->atck_no = pl34_at_force_shot ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, 21, 26 ) ;
}

/*--------------------------------------------------------------*/
static void shl34_02_fall( PLAYER_WORK *ew, PLAYER_WORK *ef_w )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( ew->hit_flag || ew->hit_box_no ){
		ew->routine_0 ++ ;
		ew->routine_1 = 0 ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( !( ew->pos.y > pl_w->ground_y ) ){
		ew->pos.y = pl_w->ground_y ;
		ew->routine_0 ++ ;
		ew->routine_1 = 0 ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
}

/*==============================================================*/
static void shl34_02_start( PLAYER_WORK *ew, PLAYER_WORK *ef_w )
{
	SHL34_02_LOCAL_W *lw = SHL34_02_LW( ew ) ;

	ew->pos.x = ef_w->pos.x + lw->ofs[0] ;
	ew->pos.y = ef_w->pos.y + lw->ofs[1] ;
	ew->spd.x = DOT_X( 2.0f ) ;
	ew->acc.x = 0.0f ;
	ew->spd.y = -2.142857f ;
	ew->acc.y = -DOT_Y( 0.125f ) ;
	if( !ef_w->disp_dir ) ew->spd.x = -ew->spd.x ;
	ew->routine_1 ++ ;
}

/*--------------------------------------------------------------*/
static void shl34_02_move( PLAYER_WORK *ew, PLAYER_WORK *ef_w )
{
	static void ( *shl34_02_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *ef_w ) = {
		shl34_02_start, shl34_02_fall,
	} ;

	shl34_02_move_jmp[ew->routine_1]( ew, ef_w ) ;
}

/*--------------------------------------------------------------*/
static void shl34_02_end( PLAYER_WORK *ew, PLAYER_WORK *ef_w )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->routine_0 ++ ;
	burst_set( pl_w, &ew->pos, 1, 0x409 ) ;
	pl_se_req( ew, 73 ) ;
}

/*--------------------------------------------------------------*/
static void shl34_02_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl34_02.c" End of File	*/

/* Morrigan's effect 02 (character 03's shot 02): the bursts of her Silhouette Blade, a chain of
   five that rise from the ground one after another in front of her, each putting out the next. */
#include "player.h"
#include "pl_char.h"
#include "pl03_def.h"

/* the effect's own work, from obj_mtx */
typedef struct SHL03_02_LOCAL_W {
	Uint8	hit;		/* 000: something struck it */
	Uint8	unk_1;		/* 001 */
} SHL03_02_LOCAL_W;
#define SHL03_02_LW( w )	( ( SHL03_02_LOCAL_W * )&( w )->obj_mtx )

static void shl03_02_main( PLAYER_WORK *ew );

/****************************************************************/
/*			The first burst, from Morrigan						*/
/****************************************************************/
PLAYER_WORK *shl03_02_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl03_02_main ;
		ew->master_w = pl_w ;
		ew->ef_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = 0 ;
		ew->sub_type = 0 ;
		ew->ef_no = 0x302 ;
		ew->spd.x = pl_w->pos.x ;
	}
	return ew ;
}

/*==============================================================*/
/*			The next one, from the burst w						*/
/*==============================================================*/
static PLAYER_WORK *shl03_02_next_set( PLAYER_WORK *w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( w, W_SHL, LIST_BEHIND ) ) ){
		ew->work_func = shl03_02_main ;
		ew->master_w = w->master_w ;
		ew->ef_w = w ;
		ew->pl_type = w->pl_type ;
		ew->type = w->type + 1 ;
		ew->sub_type = 0 ;
		ew->ef_no = 0x302 ;
		ew->spd.x = w->pos.x ;
	}
	return ew ;
}

static void shl03_02_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl03_02_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl03_02_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl03_02_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl03_02_main( PLAYER_WORK *ew )
{
	static void ( *shl03_02_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl03_02_init, shl03_02_move, shl03_02_end, shl03_02_free,
	} ;

	shl03_02_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void shl03_02_init( register PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	/* by the burst's place in the chain: its frames, the frames before it puts out the next,
	   and its rise and the rise's change, in 8.8 */
	static struct { Sint16 time, next, spd_y, acc_y ; } shl03_02_tbl[] = {
		{ 11, 8, 0x1000, -0x200 }, { 13, 9, 0x1200, -0x1e0 }, { 15, 10, 0x1400, -0x1c0 },
		{ 17, 11, 0x1600, -0x1a0 }, { 19, 12, 0x1800, -0x180 },
	} ;
	PLAYER_WORK *mw = ew->ef_w ;
	SHL03_02_LOCAL_W *lw = SHL03_02_LW( ew ) ;

	ew->routine_0 ++ ;
	ew->routine_1 = 0 ;
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
	lw->unk_1 = 3 ;
	lw->hit = 0 ;
	BODY_SIZE_SET( ew, 0x60003030 ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = ew->type + 72 ;
	HIT_RESET( ew ) ;
	ew->pos.x = 80.0f ;
	if( mw->disp_dir ) ew->pos.x = ew->pos.x + ew->spd.x ;
	else ew->pos.x = -ew->pos.x + ew->spd.x ;
	ew->pos.y = pl_w->ground_y ;
	ew->spd.x = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->timer0 = shl03_02_tbl[ew->type].time ;
	ew->timer1 = shl03_02_tbl[ew->type].next ;
	ew->spd.y = shl03_02_tbl[ew->type].spd_y * 2.142857f / 256.0f ;
	ew->acc.y = shl03_02_tbl[ew->type].acc_y * 2.142857f / 256.0f ;
	ew->char_no.b.grp = 22 ;
	ew->char_no.b.no = 44 ;
	pl_char_set( ew, ew->char_no.b.grp, ew->char_no.b.no ) ;
	pl_shadow_set( ew ) ;
	ew->be_flag = 1 ;
}

/*==============================================================*/
static void shl03_02_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL03_02_LOCAL_W *lw = SHL03_02_LW( ew ) ;

	if( ew->hit_box_no ) lw->hit = 1 ;
	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		pl_se_req( ew, SE_SWING_H ) ;
		pl_char_set( ew, 22, 45 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl03_02_rise( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL03_02_LOCAL_W *lw = SHL03_02_LW( ew ) ;

	pl_char_move( ew ) ;
	if( lw->hit || ew->hit_box_no || !--ew->timer0 ){
		ew->routine_1 ++ ;
		pl_char_set( ew, 22, 46 ) ;
		return ;
	}
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
}

/*--------------------------------------------------------------*/
static void shl03_02_vanish( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( ew->cg_type ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
/*			It lasts while Morrigan is still in the hyper		*/
/*			combo												*/
/*==============================================================*/
static void shl03_02_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl03_02_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl03_02_start, shl03_02_rise, shl03_02_vanish,
	} ;
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	ew->sub_prio = -2 ;
	if( pl_w->routine_1 || pl_w->act_st != N_SUPER ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( ew->type < 4 && ew->timer1 && !--ew->timer1 && shl03_02_next_set( ew ) ) lw->shot_cnt ++ ;
	shl03_02_move_jmp[ew->routine_1]( ew, pl_w ) ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl03_02_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	lw->shot_cnt -- ;
}

/*--------------------------------------------------------------*/
static void shl03_02_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl03_02.c" End of File	*/

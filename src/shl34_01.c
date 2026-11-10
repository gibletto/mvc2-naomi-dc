/* Sentinel's effect 01 (character 34's shot 01): the three drones that fly across the screen from
   one side. Sentinel Force's light ones (type 0) rise and fall with his stick, a few frames late;
   its heavy ones (type 1) stop and drop three shots; Hyper Sentinel Force's (type 2) fly on. */
#include "player.h"
#include "pl_char.h"
#include "pl34_def.h"

/* the effect's own work, at obj_mtx */
typedef struct SHL34_01_LOCAL_W {
	Uint16	hist[16];	/* his stick, the last sixteen frames */
	Sint8	rd;		/* which of them it follows */
	Sint8	wr;		/* where this frame's goes */
	Sint8	cnt;		/* shots left */
	Uint8	unk_23;
	Uint8	flag;		/* the button was let go */
} SHL34_01_LOCAL_W;
#define SHL34_01_LW( w )	( ( SHL34_01_LOCAL_W * )&( w )->obj_mtx )

static void shl34_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl34_01_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew, *tw ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl34_01_main ;
		ew->master_w = pl_w ;
		ew->ef_w = ew ;
		ew->type = no ;
		ew->sub_type = 0 ;
		ew->ef_no = 0x3401 ;
	}
	if( ( tw = work_get( ew, W_SHL, LIST_BEHIND ) ) ){
		tw->work_func = shl34_01_main ;
		tw->master_w = pl_w ;
		tw->ef_w = ew ;
		tw->type = no ;
		tw->sub_type = 1 ;
		tw->ef_no = 0x3401 ;
	}
	if( ( tw = work_get( ew, W_SHL, LIST_BEHIND ) ) ){
		tw->work_func = shl34_01_main ;
		tw->master_w = pl_w ;
		tw->ef_w = ew ;
		tw->type = no ;
		tw->sub_type = 2 ;
		tw->ef_no = 0x3401 ;
	}
	return ew ;
}

static void shl34_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl34_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl34_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl34_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl34_01_main( PLAYER_WORK *ew )
{
	static void ( *shl34_01_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl34_01_init, shl34_01_move, shl34_01_end, shl34_01_free,
	} ;

	shl34_01_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/* his speed, x and y, by type */
typedef struct SHL34_01_SPD {
	Sint16	x, y;
} SHL34_01_SPD;

/*==============================================================*/
static void shl34_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Uint8 rd_tbl[3] = { 0, 4, 8 } ;
	static Sint16 ofs_tbl[3][3] = {
		{ 0x1000, 0x4000, 0x7000 }, { 0x1000, 0x2000, 0x3000 }, { 0x7000, 0x4000, 0x1000 },
	} ;
	static SHL34_01_SPD spd_tbl[3] = { { 0x700, -0x180 }, { 0x300, 0 }, { 0x1000, 0 } } ;
	SHL34_01_LOCAL_W *lw = SHL34_01_LW( ew ) ;
	PL34_LOCAL_W *tl = ( PL34_LOCAL_W * )pl_w->tlocal ;

	BODY_SIZE_SET( ew, 0x20204060 ) ;
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
	ew->prio = 0 ;
	ew->sub_prio = ew->sub_type ;
	tl->drone_cnt ++ ;
	if( !pl_w->disp_dir ){
		ew->pos.x = scr_work.scr_right + ofs_tbl[ew->type][ew->sub_type] * 1.6666666f / 256.0f ;
		ew->spd.x = -( spd_tbl[ew->type].x * 1.6666666f / 256.0f ) ;
	} else {
		ew->pos.x = scr_work.scr_left - ofs_tbl[ew->type][ew->sub_type] * 1.6666666f / 256.0f ;
		ew->spd.x = spd_tbl[ew->type].x * 1.6666666f / 256.0f ;
	}
	if( ew->type != 2 ){
		ew->pos.y = pl_w->ground_y + DOT_Y( 144.0f ) ;
		if( pl_w->team_type == 3 ){
			ew->atck_no = pl34_at_force ;
			HIT_RESET( ew ) ;
		} else {
			ew->atck_no = pl34_at_force ;
			HIT_RESET( ew ) ;
		}
	} else {
		ew->pos.y = pl_w->ground_y + ( ew->sub_type * 0x3000 + 0x2000 ) * 2.142857f / 256.0f ;
		ew->atck_no = pl34_at_hforce ;
		HIT_RESET( ew ) ;
	}
	ew->spd.y = spd_tbl[ew->type].y * 2.142857f / 256.0f ;
	ew->acc.x = ew->acc.y = 0.0f ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	pl_char_set( ew, 21, 23 ) ;
	lw->cnt = 0 ;
	lw->unk_23 = 0 ;
	lw->rd = 0 ;
	lw->wr = rd_tbl[ew->sub_type] ;
}

/*==============================================================*/
/*			The plain drone: follows his stick and flies on		*/
/*			until it hits or leaves the screen					*/
/*==============================================================*/
static void shl34_01_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL34_01_LOCAL_W *lw = SHL34_01_LW( ew ) ;

	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	lw->hist[lw->wr] = pl_w->pl_sw_new ;
	if( lw->hist[lw->rd] & 0x2000 ) ew->pos.y += DOT_Y( 2.0f ) ;
	if( lw->hist[lw->rd] & 0x1000 ) ew->pos.y -= 2.142857f ;
	lw->rd ++ ;
	lw->wr ++ ;
	lw->rd &= 15 ;
	lw->wr &= 15 ;
	if( ew->hit_flag ){
		ew->routine_0 = 2 ;
		ew->routine_1 = 0 ;
	}
	if( ew->hit_box_no ){
		ew->routine_0 = 2 ;
		ew->routine_1 = 0 ;
	}
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
	if( !scr_in_ck( ew ) ) ew->routine_0 = 3 ;
}

/*==============================================================*/
static void shl34_01_start( PLAYER_WORK *ew )
{
	static Sint16 time_tbl[3] = { 1, 2, 3 } ;
	SHL34_01_LOCAL_W *lw = SHL34_01_LW( ew ) ;

	ew->routine_1 ++ ;
	ew->acc.x = ew->acc.y = 0.0f ;
	ew->timer0 = time_tbl[ew->sub_type] ;
	lw->cnt = 3 ;
	pl_voice_req( ew, 35 ) ;
	pl_char_set( ew, 21, 24 ) ;
}

/*--------------------------------------------------------------*/
static void shl34_01_fire( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL34_01_LOCAL_W *lw = SHL34_01_LW( ew ) ;

	if( ew->hit_flag ){
		ew->routine_0 = 2 ;
		ew->routine_1 = 0 ;
	}
	if( ew->hit_box_no ){
		ew->routine_0 = 2 ;
		ew->routine_1 = 0 ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( pl_char_move( ew ) < 0 ){
		if( !-- ew->timer0 ){
			shl34_02_set( ew ) ;
			ew->timer0 = 1 ;
			if( !-- lw->cnt ){
				ew->routine_0 ++ ;
				ew->routine_1 = 0 ;
				pl_char_set( ew, 21, 25 ) ;
			}
		}
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl34_01_hold( PLAYER_WORK *ew, register PLAYER_WORK *pl_w )
{
	SHL34_01_LOCAL_W *lw = SHL34_01_LW( ew ) ;

	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->hit_flag ){
		ew->routine_0 = 2 ;
		ew->routine_1 = 0 ;
	}
	if( ew->hit_box_no ){
		ew->routine_0 = 2 ;
		ew->routine_1 = 0 ;
	}
	if( !lw->flag ){
		if( pl_w->pl_sw_now & KEY_HK ) lw->flag = 1 ;
	} else if( ew->timer0 <= 60 ){
		shl34_01_start( ew ) ;
		return ;
	}
	if( !-- ew->timer0 ){
		shl34_01_start( ew ) ;
		return ;
	}
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
}

/*==============================================================*/
static void shl34_01_ready( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL34_01_LOCAL_W *lw = SHL34_01_LW( ew ) ;

	ew->timer0 = 60 ;
	if( pl_w->pl_sw_new & KEY_HK ) lw->flag = 0 ;
	else lw->flag = 1 ;
	ew->routine_1 ++ ;
	shl34_01_hold( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl34_01_hforce_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	if( !scr_in_ck( ew ) ) ew->routine_0 = 3 ;
}

/*==============================================================*/
static void shl34_01_hforce_atck( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ew->hit_flag || ew->hit_box_no ) ew->routine_1 ++ ;
	hit_work_set( ew ) ;
	shl34_01_hforce_fly( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl34_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl34_01_light_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl34_01_fly,
	} ;
	static void ( *shl34_01_heavy_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl34_01_ready, shl34_01_hold, shl34_01_fire,
	} ;
	static void ( *shl34_01_hforce_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl34_01_hforce_atck, shl34_01_hforce_fly,
	} ;

	switch( ew->type ){
	case 0:
		shl34_01_light_jmp[ew->routine_1]( ew, pl_w ) ;
		break ;
	case 1:
		shl34_01_heavy_jmp[ew->routine_1]( ew, pl_w ) ;
		break ;
	case 2:
		shl34_01_hforce_jmp[ew->routine_1]( ew, pl_w ) ;
		break ;
	}
}

/*--------------------------------------------------------------*/
static void shl34_01_out_move( PLAYER_WORK *ew )
{
	if( !scr_in_y_ck( ew ) ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
}

/*==============================================================*/
static void shl34_01_out_start( PLAYER_WORK *ew )
{
	ew->routine_1 ++ ;
	ew->spd.x = 1.6666666f ;
	ew->acc.x = 0.0f ;
	if( !ew->disp_dir ) ew->spd.x = -ew->spd.x ;
	ew->spd.y = DOT_Y( 1.5f ) ;
	ew->acc.y = DOT_Y( 0.25f ) ;
	shl34_01_out_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl34_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl34_01_end_jmp[] )( PLAYER_WORK *ew ) = {
		shl34_01_out_start, shl34_01_out_move,
	} ;

	shl34_01_end_jmp[ew->routine_1]( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl34_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL34_LOCAL_W *tl = ( PL34_LOCAL_W * )pl_w->tlocal ;

	tl->drone_cnt -- ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl34_01.c" End of File	*/

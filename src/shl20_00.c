/* Felicia's effect 00 (character 20's shot 00): type 0, from her Cat Spike, a ball thrown in an arc
   that bounces and can be hit; type 1, from Sand Splash, the sand that hits in front of
   her; type 4, from Super Sand Splash, the one that sends a row of types 5 and 7 across
   the screen. Types 2 and 3 run type 1's code and type 6 type 4's; nothing sets them. */
#include "player.h"
#include "pl_char.h"
#include "pl20_def.h"

/* the effect's own work, from obj_flag */
typedef struct SHL20_00_LOCAL_W {
	Uint16	char_no;	/* 00: Felicia's animation when it was set */
	Sint8	state;		/* 02: for the row: 1 hit, -1 spent, 0 flying */
	Sint8	hit_timer;	/* 03: frames the row is out of play after a hit */
	Sint8	last;		/* 04: set for the last few frames */
} SHL20_00_LOCAL_W;
#define SHL20_00_LW( w )	( ( SHL20_00_LOCAL_W * )&( w )->obj_flag )

static PL20_LOCAL_W *shl20_00_pl_lw ;
static SHL20_00_LOCAL_W *shl20_00_lw ;

static void shl20_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl20_00_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl20_00_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
		ew->ef_no = 0x2000 ;
	}
	return ew ;
}

/*==============================================================*/
static PLAYER_WORK *shl20_00_part_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( pl_w, W_SHL, LIST_BEFORE ) ) ){
		ew->work_func = shl20_00_main ;
		ew->master_w = pl_w->master_w ;
		ew->type = type ;
		ew->ef_no = 0x2000 ;
	}
	return ew ;
}

static void shl20_00_spike_main( PLAYER_WORK *ew );
static void shl20_00_sand_main( PLAYER_WORK *ew );
static void shl20_00_ssand_main( PLAYER_WORK *ew );
static void shl20_00_ssand_part_main( PLAYER_WORK *ew );

/*==============================================================*/
static void shl20_00_main( PLAYER_WORK *ew )
{
	static void ( *shl20_00_jmp[] )( PLAYER_WORK *ew ) = {
		shl20_00_spike_main, shl20_00_sand_main, shl20_00_sand_main, shl20_00_sand_main,
		shl20_00_ssand_main, shl20_00_ssand_part_main, shl20_00_ssand_main, shl20_00_ssand_part_main,
	} ;

	shl20_00_pl_lw = ( PL20_LOCAL_W * )&ew->master_w->tlocal[0] ;
	shl20_00_lw = SHL20_00_LW( ew ) ;
	shl20_00_jmp[ew->type]( ew ) ;
}

static void shl20_00_spike_init( PLAYER_WORK *ew );
static void shl20_00_spike_move( PLAYER_WORK *ew );
static void shl20_00_end( PLAYER_WORK *ew );
static void shl20_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl20_00_spike_main( PLAYER_WORK *ew )
{
	static void ( *shl20_00_spike_jmp[] )( PLAYER_WORK *ew ) = {
		shl20_00_spike_init, shl20_00_spike_move, shl20_00_end, shl20_00_free,
	} ;

	shl20_00_spike_jmp[ew->routine_0]( ew ) ;
}

/* the speeds by the attack's level: x, its change, y, its change */
static Float shl20_00_spd_tbl[4][4] = {
	{ 15.0f, 0.0f, DOT_Y( 2.5f ), -0.4017857f },
	{ DOT_X( 5.5f ), 0.0f, 7.5f, -0.4017857f },
	{ DOT_X( 7.25f ), 0.0f, DOT_Y( 4.0f ), -0.4017857f },
	{ DOT_X( 2.0f ), 0.0f, DOT_Y( 7.5f ), -0.4017857f },
} ;

/*==============================================================*/
static void shl20_00_spike_init( PLAYER_WORK *ew )
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
	ew->hit_box_no = 0 ;
	ew->pl_no ^= 1 ;
	ew->atck_flag = 0 ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->size_front = 40 ;
	ew->size_back = 40 ;
	ew->pos.x = ew->master_w->pos.x + ( ew->disp_dir ? DOT_X( 8.0f ) : -DOT_X( 8.0f ) ) ;
	ew->pos.y = ew->master_w->pos.y + DOT_Y( 32.0f ) ;
	ew->spd.x = ew->disp_dir ? shl20_00_spd_tbl[ew->atck_level][0] : -shl20_00_spd_tbl[ew->atck_level][0] ;
	ew->acc.x = ew->disp_dir ? shl20_00_spd_tbl[ew->atck_level][1] : -shl20_00_spd_tbl[ew->atck_level][1] ;
	ew->spd.y = shl20_00_spd_tbl[ew->atck_level][2] ;
	ew->acc.y = shl20_00_spd_tbl[ew->atck_level][3] ;
	shl20_00_lw->char_no = ew->master_w->char_no.w ;
	pl_char_set( ew, Char_Effect, 0 ) ;
}

static void shl20_00_spike_fly( PLAYER_WORK *ew );
static void shl20_00_spike_hit( PLAYER_WORK *ew );
static void shl20_00_spike_bound( PLAYER_WORK *ew );
static void shl20_00_spike_vanish( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl20_00_spike_move( PLAYER_WORK *ew )
{
	static void ( *shl20_00_spike_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl20_00_spike_fly, shl20_00_spike_hit, shl20_00_spike_bound, shl20_00_spike_vanish,
	} ;

	if( !scr_in_x_ck( ew ) ){
		shl20_00_end( ew ) ;
		return ;
	}
	shl20_00_spike_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void shl20_00_spike_fly( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( shl20_00_lw->char_no != ew->master_w->char_no.w ){
		ew->routine_1 += 2 ;
		return ;
	}
	if( ew->hit_box_no ){
		ew->routine_1 ++ ;
		pl_char_set( ew, Char_Effect, 2 ) ;
		return ;
	}
	if( ew->pos.y <= ew->master_w->ground_y ){
		ew->routine_1 += 2 ;
		ew->routine_2 ++ ;
		ew->pos.y = ew->master_w->ground_y ;
		pl_char_set( ew, Char_Effect, 1 ) ;
		return ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl20_00_spike_hit( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ) shl20_00_end( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl20_00_spike_bound( PLAYER_WORK *ew )
{
	if( !ew->routine_2 ){
		pl_char_move( ew ) ;
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		if( ew->pos.y > ew->master_w->ground_y ) return ;
		ew->routine_2 ++ ;
		ew->pos.y = ew->master_w->ground_y ;
		pl_char_set( ew, Char_Effect, 1 ) ;
		return ;
	}
	pl_char_move( ew ) ;
	if( ew->cg_type ) return ;
	ew->routine_1 ++ ;
	ew->routine_2 = 0 ;
	ew->spd.y *= -1.0f ;
	ew->spd.y /= 2.0f ;
	ew->timer0 = 64 ;
	pl_char_set( ew, Char_Effect, 0 ) ;
}

/*--------------------------------------------------------------*/
static void shl20_00_spike_vanish( PLAYER_WORK *ew )
{
	if( !ew->routine_2 ){
		pl_char_move( ew ) ;
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		if( ew->pos.y <= ew->master_w->ground_y ){
			ew->routine_2 ++ ;
			ew->pos.y = ew->master_w->ground_y ;
			pl_char_set( ew, Char_Effect, 1 ) ;
		}
	} else {
		pl_char_move( ew ) ;
		if( !ew->cg_type ){
			ew->routine_2 = 0 ;
			ew->spd.y *= -1.0f ;
			pl_char_set( ew, Char_Effect, 0 ) ;
		}
	}
	if( ! -- ew->timer0 ){
		shl20_00_end( ew ) ;
		return ;
	}
	ew->disp_flag = Sys->game_timer0 & 1 ;
}

static void shl20_00_sand_init( PLAYER_WORK *ew );
static void shl20_00_sand_move( PLAYER_WORK *ew );

/*==============================================================*/
static void shl20_00_sand_main( PLAYER_WORK *ew )
{
	static void ( *shl20_00_sand_jmp[] )( PLAYER_WORK *ew ) = {
		shl20_00_sand_init, shl20_00_sand_move, shl20_00_end, shl20_00_free,
	} ;

	shl20_00_sand_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl20_00_sand_init( PLAYER_WORK *ew )
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
	ew->atck_no = ew->atck_level + 66 ;
	HIT_RESET( ew ) ;
	shl20_00_lw->char_no = ew->master_w->char_no.w ;
	ew->pos.x = ew->master_w->pos.x + ( ew->disp_dir ? DOT_X( 64.0f ) : -DOT_X( 64.0f ) ) ;
	ew->pos.y = ew->master_w->ground_y ;
	pl_char_set( ew, Char_Effect, 3 ) ;
}

/*--------------------------------------------------------------*/
static void shl20_00_sand_move( PLAYER_WORK *ew )
{
	if( shl20_00_lw->char_no != ew->master_w->char_no.w ){
		shl20_00_end( ew ) ;
		return ;
	}
	if( pl_char_move( ew ) < 0 ){
		shl20_00_end( ew ) ;
		return ;
	}
	hit_work_set( ew ) ;
}

static void shl20_00_ssand_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl20_00_ssand_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl20_00_ssand_main( PLAYER_WORK *ew )
{
	static void ( *shl20_00_ssand_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl20_00_ssand_init, shl20_00_ssand_move, shl20_00_end, shl20_00_free,
	} ;

	shl20_00_ssand_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void shl20_00_ssand_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *tw ;
	SHL20_00_LOCAL_W *lw ;
	Float spd, x, no ;
	int i ;

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
	ew->disp_flag = 0 ;
	ew->prio = 8 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	x = scr_work.scr_left - DOT_X( 32.0f ) ;
	spd = -DOT_X( 64.0f ) ;
	if( !pl_w->disp_dir ){
		x = scr_work.scr_right + DOT_X( 32.0f ) ;
		spd = DOT_X( 64.0f ) ;
	}
	ew->pos.x = x ;
	ew->pos.y = pl_w->ground_y - DOT_Y( 24.0f ) ;
	ew->spd.x = spd ;
	ew->timer0 = 0 ;
	i = 21 ;
	do {
		if( ( tw = shl20_00_part_set( ew, 5 ) ) ){
			tw->pos.x = ew->pos.x ;
			tw->pos.y = pl_w->ground_y - DOT_Y( 24.0f ) ;
			tw->ef_w = ew ;
			tw->sub_type = ew->timer0 ++ ;
			ew->pos.x += ew->spd.x ;
		}
	} while( -- i ) ;
	x = scr_work.scr_left - DOT_X( 736.0f ) ;
	spd = -DOT_X( 64.0f ) ;
	if( !pl_w->disp_dir ){
		x = scr_work.scr_right + DOT_X( 736.0f ) ;
		spd = DOT_X( 64.0f ) ;
	}
	ew->pos.x = x ;
	ew->pos.y = pl_w->ground_y - DOT_Y( 24.0f ) ;
	ew->spd.x = spd ;
	ew->timer0 = 0 ;
	i = 20 ;
	do {
		if( ( tw = shl20_00_part_set( ew, 7 ) ) ){
			tw->pos.x = ew->pos.x ;
			tw->pos.y = pl_w->ground_y - DOT_Y( 24.0f ) ;
			tw->ef_w = ew ;
			tw->sub_type = ew->timer0 ++ ;
			ew->pos.x += ew->spd.x ;
		}
	} while( -- i ) ;
	ew->timer1 = 10 ;
	ew->ef_w = ew ;
	ew->sub_type = 20 ;
	lw = SHL20_00_LW( ew->ef_w ) ;
	lw->state = 0 ;
	lw->hit_timer = 0 ;
	lw->last = 0 ;
	ew->size_front = 0xff ;
	ew->size_back = 32 ;
	ew->timer0 = 0x80 ;
	spd = 40.0f ;
	if( !ew->disp_dir ) spd = -40.0f ;
	ew->spd.x = spd ;
	ew->acc.x = 0.0f ;
	ew->atck_no = pl20_at_ssand_hit ;
	ew->hit_flag = ew->atck_attr = 0 ;
	ew->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[ew->pl_no] ++ ;
	no = 20.0f ;
	if( ew->type > 5 ) no = 41.0f ;
	no += ew->sub_type ;
	pl_char_set( ew, Char_Effect, no ) ;
}

/*--------------------------------------------------------------*/
static void shl20_00_ssand_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL20_00_LOCAL_W *lw = SHL20_00_LW( ew->ef_w ) ;
	Sint32 unk[8] ;	/* unused */

	if( lw->state > 0 ){
		if( -- ew->timer1 <= 0 ){
			lw->state = -1 ;
		} else {
			lw->state = 0 ;
			if( ew->timer1 <= 2 ) lw->last = 1 ;
		}
	}
	if( lw->hit_timer ) lw->hit_timer -- ;
	if( !ew->routine_1 ){
		if( pl_w->routine_1 ){
			ew->routine_1 ++ ;
			ew->hit_flag = HR_GUARD ;
		}
		if( ew->hit_flag ){
			lw->state = 1 ;
			lw->hit_timer = 8 ;
			ew->hit_flag = 0 ;
		}
		if( lw->hit_timer ){
			ew->atck_flag = 0 ;
		} else if( lw->state < 0 ){
			ew->routine_1 ++ ;
		} else {
			ew->atck_flag = HIT_ON | HIT_SHOT ;
			if( lw->last ){
				ew->atck_no = pl20_at_ssand_last ;
				HIT_RESET( ew ) ;
			} else {
				ew->atck_no = pl20_at_ssand_hit ;
				HIT_RESET( ew ) ;
			}
		}
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->timer0 -- ;
	ew->disp_flag = 1 ;
	if( !scr_in_x_ck( ew ) ){
		ew->disp_flag = 0 ;
		if( ew->timer0 < 0 ){
			shl20_00_end( ew ) ;
			return ;
		}
	}
	hit_work_set( ew ) ;
}

static void shl20_00_ssand_part_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl20_00_ssand_part_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl20_00_ssand_part_main( PLAYER_WORK *ew )
{
	static void ( *shl20_00_ssand_part_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl20_00_ssand_part_init, shl20_00_ssand_part_move, shl20_00_end, shl20_00_free,
	} ;

	shl20_00_ssand_part_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void shl20_00_ssand_part_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float spd ;
	Uint8 no ;

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
	ew->disp_flag = 0 ;
	ew->prio = 8 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->timer0 = 0x80 ;
	spd = 40.0f ;
	if( !ew->disp_dir ) spd = -40.0f ;
	ew->spd.x = spd ;
	ew->acc.x = 0.0f ;
	ew->atck_no = pl20_at_ssand_hit ;
	HIT_RESET( ew ) ;
	ew->size_front = 0xff ;
	ew->size_back = 32 ;
	no = 20 ;
	if( ew->type > 5 ) no = 41 ;
	no += ew->sub_type ;
	pl_char_set( ew, Char_Effect, no ) ;
}

/*--------------------------------------------------------------*/
static void shl20_00_ssand_part_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL20_00_LOCAL_W *lw = SHL20_00_LW( ew->ef_w ) ;

	if( !ew->routine_1 ){
		if( pl_w->routine_1 ){
			ew->routine_1 ++ ;
			ew->hit_flag = HR_GUARD ;
		}
		if( ew->hit_flag ){
			lw->state = 1 ;
			lw->hit_timer = 8 ;
			ew->hit_flag = 0 ;
		}
		if( lw->hit_timer ){
			ew->atck_flag = 0 ;
		} else if( lw->state < 0 ){
			ew->routine_1 ++ ;
		} else {
			ew->atck_flag = HIT_ON | HIT_SHOT ;
			if( lw->last ){
				ew->atck_no = pl20_at_ssand_last ;
				HIT_RESET( ew ) ;
			} else {
				ew->atck_no = pl20_at_ssand_hit ;
				HIT_RESET( ew ) ;
			}
		}
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->timer0 -- ;
	ew->disp_flag = 1 ;
	if( !scr_in_x_ck( ew ) ){
		ew->disp_flag = 0 ;
		if( ew->timer0 < 0 ){
			shl20_00_end( ew ) ;
			return ;
		}
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl20_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 = 3 ;
	ew->disp_flag = 0 ;
	shl20_00_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl20_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl20_00.c" End of File	*/

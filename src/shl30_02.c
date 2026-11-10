/* Omega Red's effect 02 (character 30's shot 02): the small pieces that go with his special moves.
   One module makes several kinds, picked by the type: the coil that comes up in front of him (0, 1 and 6), the
   two images of his entrance (2), the pieces that rise off them (3), the copy of his frame (4), and
   the ones of his other moves (5, 7, 8, 9). */
#include "player.h"
#include "pl_char.h"
#include "pl30_def.h"

/* the work of a piece that rides on another, in obj_flag */
typedef struct SHL30_02_LOCAL_W {
	PLAYER_WORK	*pl_w;		/* cc: the player the head belongs to */
	Sint16		*timer;		/* d0: the counter it reads */
} SHL30_02_LOCAL_W;
#define SHL30_02_LW( w )	( ( SHL30_02_LOCAL_W * )&( w )->obj_flag )

/* the work of the win pose's piece, which follows a path, in obj_flag */
typedef struct SHL30_02_PATH_W {
	Sint32		step;		/* cc: how far along the path it is */
} SHL30_02_PATH_W;
#define SHL30_02_PW( w )	( ( SHL30_02_PATH_W * )&( w )->obj_flag )

static void shl30_02_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl30_02_set( PLAYER_WORK *pl_w, Uint8 no, Uint8 hand )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) != 0 ){
		ew->work_func = shl30_02_main ;
		ew->master_w = pl_w ;
		ew->timer0 = pl_w->char_no.w ;
		ew->type = no ;
		ew->sub_type = hand ;
		ew->ef_no = 0x3002 ;
	}
	return ew ;
}

/****************************************************************/
/*			A piece that rises off old_w, the entrance's		*/
/*			image, at its next turn								*/
/****************************************************************/
void shl30_02_piece_set( PLAYER_WORK *old_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) != 0 ){
		ew->work_func = shl30_02_main ;
		ew->master_w = old_w->master_w ;
		ew->timer0 = old_w->master_w->char_no.w ;
		ew->type = 3 ;
		ew->sub_type = old_w->sub_type ;
		ew->dir = old_w->dir ;
		ew->routine_1 = old_w->routine_1 ;
		ew->ef_no = 0x3002 ;
	}
}

/****************************************************************/
PLAYER_WORK *shl30_02_drain_set( PLAYER_WORK *ew_w, PLAYER_WORK *head_w, Sint16 *timer )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_END ) ) != 0 ){
		ew->work_func = shl30_02_main ;
		ew->type = 5 ;
		ew->master_w = ew_w ;
		ew->ef_w = head_w ;
		SHL30_02_LW( ew )->pl_w = head_w->master_w ;
		SHL30_02_LW( ew )->timer = timer ;
		ew->ef_no = 0x3002 ;
	}
	return ew ;
}

/****************************************************************/
PLAYER_WORK *shl30_02_hold_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_END ) ) != 0 ){
		ew->work_func = shl30_02_main ;
		ew->type = 4 ;
		ew->master_w = pl_w ;
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
	}
	return ew ;
}

static void shl30_02_burrow_main( PLAYER_WORK *ew );
static void shl30_02_appear_main( PLAYER_WORK *ew );
static void shl30_02_piece_main( PLAYER_WORK *ew );
static void shl30_02_hold_main( PLAYER_WORK *ew );
static void shl30_02_drain_main( PLAYER_WORK *ew );

static void shl30_02_win_main( PLAYER_WORK *ew );
static void shl30_02_win_trail_main( PLAYER_WORK *ew );
static void shl30_02_lose_main( PLAYER_WORK *ew );

/*==============================================================*/
static void shl30_02_main( PLAYER_WORK *ew )
{
	static void ( *shl30_02_jmp[] )( PLAYER_WORK *ew ) = {
		shl30_02_burrow_main, shl30_02_burrow_main, shl30_02_appear_main, shl30_02_piece_main, shl30_02_hold_main,
		shl30_02_drain_main, shl30_02_burrow_main, shl30_02_win_main, shl30_02_win_trail_main, shl30_02_lose_main,
	} ;

	shl30_02_jmp[ew->type]( ew ) ;
}

static void shl30_02_burrow_init( PLAYER_WORK *ew );
static void shl30_02_burrow_move( PLAYER_WORK *ew );
static void shl30_02_end( PLAYER_WORK *ew );
static void shl30_02_free( PLAYER_WORK *ew );

/*==============================================================*/
/*			The coil that comes up in front of him and hits:	*/
/*			it lasts while his animation is the same and he		*/
/*			is in the move's act								*/
/*==============================================================*/
static void shl30_02_burrow_main( PLAYER_WORK *ew )
{
	static void ( *shl30_02_burrow_jmp[] )( PLAYER_WORK *ew ) = {
		shl30_02_burrow_init, shl30_02_burrow_move, shl30_02_end, shl30_02_free,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( ew->type == 0 ){
		if( pl_w->char_no.w != ew->timer0 || pl_w->act_st != N_NML_ATCK ){
			shl30_02_free( ew ) ;
			return ;
		}
	} else {
		if( pl_w->char_no.w != ew->timer0 || pl_w->act_st != N_SPECIAL ){
			shl30_02_free( ew ) ;
			return ;
		}
	}
	shl30_02_burrow_jmp[ew->routine_0]( ew ) ;
}

/* how far from him the coil comes up, by the stick */
static Sint16 shl30_02_burrow_ofs[4] = { -160, -64, -256, -160 } ;

/*==============================================================*/
static void shl30_02_burrow_init( PLAYER_WORK *ew )
{
	Sint8 atck_no ;

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
	ew->disp_flag = 0 ;
	ew->atck_flag = HIT_ON | HIT_OBJ | HIT_OWNER ;
	ew->dm_flag = 0 ;
	ew->prio = 0 ;
	ew->spd.x = DOT_X( shl30_02_burrow_ofs[ew->sub_type] ) ;
	if( ew->disp_dir ) ew->spd.x = -ew->spd.x ;
	atck_no = 11 ;
	if( ew->type ) atck_no = 60 ;
	ew->atck_no = atck_no ;
	HIT_RESET( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl30_02_burrow_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( !ew->routine_1 ){
		if( pl_w->cg_type == 0 ) return ;
		ew->routine_1 ++ ;
		pl_char_set( ew, 20, 0 ) ;
		ew->disp_flag = 1 ;
	}
	ew->pos = pl_w->pos ;
	ew->pos.x += ew->spd.x ;
	if( pl_w->hit_stop ) return ;
	if( pl_char_move( ew ) < 0 ){
		pl_w->timer0 = 0 ;
		shl30_02_free( ew ) ;
		return ;
	}
	if( pl_w->speed_up_flag && pl_char_move( ew ) < 0 ){
		pl_w->timer0 = 0 ;
		shl30_02_free( ew ) ;
		return ;
	}
	hit_work_set( ew ) ;
}

static void shl30_02_appear_init( PLAYER_WORK *ew );
static void shl30_02_appear_move( PLAYER_WORK *ew );

/*==============================================================*/
/*			An image of his entrance: every few frames it		*/
/*			makes a piece that rises off it						*/
/*==============================================================*/
static void shl30_02_appear_main( PLAYER_WORK *ew )
{
	static void ( *shl30_02_appear_jmp[] )( PLAYER_WORK *ew ) = {
		shl30_02_appear_init, shl30_02_appear_move, shl30_02_end, shl30_02_free,
	} ;

	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->char_no.w != ew->timer0 ){
		shl30_02_free( ew ) ;
		return ;
	}
	shl30_02_appear_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl30_02_appear_init( PLAYER_WORK *ew )
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
	ew->pos = ew->master_w->pos ;
	ew->timer1 = 0 ;
	ew->dir = 0 ;
	if( ew->sub_type == 0 ) ew->prio = 0 ;
	else ew->prio = 7 ;
	pl_char_set( ew, 18, ew->sub_type + 1 ) ;
}

/*--------------------------------------------------------------*/
static void shl30_02_appear_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( -- ew->timer1 <= 0 ){
		shl30_02_piece_set( ew ) ;
		if( ew->routine_1 == 0 ) ew->timer1 = 4 ;
		else ew->timer1 = 7 ;
		ew->dir = ( ew->dir + 1 ) & 7 ;
	}
	if( !ew->routine_1 ){
		pl_char_move( ew ) ;
		if( pl_w->cg_flag1 ){
			ew->routine_1 ++ ;
			pl_char_set( ew, 18, ew->sub_type + 3 ) ;
		}
	} else if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

static void shl30_02_piece_init( PLAYER_WORK *ew );
static void shl30_02_piece_move( PLAYER_WORK *ew );

/*==============================================================*/
/*			A piece that rises off the entrance's image			*/
/*==============================================================*/
static void shl30_02_piece_main( PLAYER_WORK *ew )
{
	static void ( *shl30_02_piece_jmp[] )( PLAYER_WORK *ew ) = {
		shl30_02_piece_init, shl30_02_piece_move, shl30_02_end, shl30_02_free,
	} ;

	shl30_02_piece_jmp[ew->routine_0]( ew ) ;
}

/* the place of a rising piece beside the image, by its turn */
static Sint16 shl30_02_piece_ofs[8] = { 0, 64, -48, 16, -64, 32, -16, 48 } ;

/*==============================================================*/
static void shl30_02_piece_init( PLAYER_WORK *ew )
{
	Float ofs_x ;
	Float ofs_dx ;
	int frame ;
	Uint8 dir ;

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
	ew->pos = ew->master_w->pos ;
	dir = ew->dir ;
	frame = 0 ;
	if( ew->sub_type == 0 ){
		ew->prio = 0 ;
		ofs_x = 0.0f ;
		ew->spd.y = DOT_Y( 1.0f ) ;
		ew->acc.y = DOT_Y( 0.25f ) ;
	} else {
		ew->prio = 7 ;
		frame = 4 ;
		if( ew->routine_1 == 0 ) ew->pos.y += DOT_Y( 16.0f ) ;
		else ew->pos.y += DOT_Y( 80.0f ) ;
		ew->spd.y = DOT_Y( 2.0f ) ;
		ew->acc.y = DOT_Y( 0.5f ) ;
		ofs_x = -DOT_X( 8.0f ) ;
		dir = ~dir & 7 ;
	}
	ofs_dx = DOT_X( shl30_02_piece_ofs[dir] ) ;
	ofs_x += ofs_dx ;
	if( ew->disp_dir ) ofs_x = -ofs_x ;
	ew->pos.x += ofs_x ;
	ew->spd.x = 0.0f ;
	ew->acc.x = 0.0f ;
	pl_char_set2( ew, 18, 5, frame ) ;
}

/*--------------------------------------------------------------*/
static void shl30_02_piece_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *m_w ;
	Float spd_max ;
	Float floor_y ;

	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	spd_max = ew->routine_1 == 0 ? 7.5f : DOT_Y( 8.0f ) ;
	if( ew->spd.y > spd_max ){
		ew->acc.y = 0.0f ;
		ew->spd.y = spd_max ;
	}
	m_w = ew->master_w ;
	floor_y = m_w->pos.y + DOT_Y( 48.0f ) ;
	if( ew->sub_type ) floor_y += DOT_Y( 16.0f ) ;
	if( ew->routine_1 ) floor_y += DOT_Y( 128.0f ) ;
	if( ew->pos.y > floor_y ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
/*			The copy of his image that shows in the order of	*/
/*			the one he holds									*/
/*==============================================================*/
static void shl30_02_hold_main( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	ew->prio = ew->master_w->catch_w->prio ;
	ew->sub_prio = -1 ;
	switch( ew->routine_0 ){
	case 0 :
	case 1 :
		ew->disp_flag = 1 ;
		break ;
	case 2 :
	case 3 :
	default :
		shl30_02_free( ew ) ;
		break ;
	}
}

static void shl30_02_drain_init( PLAYER_WORK *ew );
static void shl30_02_drain_move( PLAYER_WORK *ew );

/*==============================================================*/
/*			A part of the coil that blinks beside the head		*/
/*==============================================================*/
static void shl30_02_drain_main( PLAYER_WORK *ew )
{
	static void ( *shl30_02_drain_jmp[] )( PLAYER_WORK *ew ) = {
		shl30_02_drain_init, shl30_02_drain_move, shl30_02_end, shl30_02_free,
	} ;

	shl30_02_drain_jmp[ew->routine_0]( ew ) ;
}

/* the animation of a part: by the kind of drain (1 punch, 2 kick), then by its number */
static Uint8 shl30_02_drain_no[3][4] = {
	{ 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 },
} ;
static Uint8 shl30_02_drain_step[16] = { 0, 1, 2, 3, 3, 2, 1, 0, 1, 1, 1, 2, 2, 1, 1, 1 } ;

/*==============================================================*/
static void shl30_02_drain_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *head_w ;
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
	pl_w = ew->master_w ;
	head_w = ew->ef_w ;
	CHAR_PART_COPY( ew, head_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = head_w->pl_no ;
	ew->pl_type = head_w->pl_type ;
	ew->scale.x = head_w->scale.x ;
	ew->scale.y = head_w->scale.y ;
	ew->atck_level = head_w->atck_level ;
	ew->work_no = head_w->work_no ;
	ew->col_cnt = head_w->col_cnt ;
	ew->scale = head_w->scale ;
	ew->prio = head_w->prio ;
	ew->disp_flag = 0 ;
	ew->pos = pl_w->pos ;
	ew->sub_prio = 5 ;
	ew->char_no.b.no = shl30_02_drain_no[ew->sub_type][shl30_02_drain_step[ew->timer0]] ;
	pl_char_set( ew, Char_Effect, ew->char_no.b.no ) ;
	if( Sys->game_timer0 & 1 ) ew->disp_flag = 1 ;
}

/*--------------------------------------------------------------*/
static void shl30_02_drain_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = SHL30_02_LW( ew )->pl_w ;
	PLAYER_WORK *head_w = ew->ef_w ;
	PLAYER_WORK *m_w = ew->master_w ;

	ew->disp_flag = 0 ;
	ew->prio = head_w->prio ;
	if( pl_w->routine_1 || pl_w->act_st != N_SPECIAL || head_w->routine_1 != 5 ){
		ew->routine_0 ++ ;
		return ;
	}
	{
		if( *SHL30_02_LW( ew )->timer == 2 ){
			if( -- ew->timer1 < 0 ){
				ew->routine_0 ++ ;
				return ;
			}
			m_w = m_w->behind_w ;
			ew->master_w = m_w ;
		}
		ew->pos = m_w->pos ;
		if( Sys->game_timer0 & 1 ){
			pl_char_move( ew ) ;
			ew->disp_flag = 1 ;
		}
		return ;
	}
}

static void shl30_02_win_init( PLAYER_WORK *ew );
static void shl30_02_win_move( PLAYER_WORK *ew );

/*==============================================================*/
/*			A part of the coil that slides in steps from his	*/
/*			hand, while he is in his win pose					*/
/*==============================================================*/
static void shl30_02_win_main( PLAYER_WORK *ew )
{
	static void ( *shl30_02_win_jmp[] )( PLAYER_WORK *ew ) = {
		shl30_02_win_init, shl30_02_win_move, shl30_02_end, shl30_02_free,
	} ;

	if( ew->master_w->act_st != N_WIN ){
		shl30_02_free( ew ) ;
		return ;
	}
	shl30_02_win_jmp[ew->routine_0]( ew ) ;
}

/* where it stands in each step, from his position (x and y; by hand) */
static Sint8 shl30_02_win_ofs[3][2][2] = {
	{ { -16, 53 }, { 31, 53 } }, { { -45, 63 }, { 60, 63 } }, { { -60, 84 }, { 75, 84 } },
} ;
/* the frames of each step */
static Sint16 shl30_02_win_time[4] = { 16, 8, 24, 0 } ;

/*==============================================================*/
static void shl30_02_win_pos( PLAYER_WORK *ew )
{
	Sint8 *ofs ;
	Float dx ;

	ew->pos = ew->master_w->pos ;
	ofs = shl30_02_win_ofs[SHL30_02_PW( ew )->step][ew->sub_type] ;
	dx = DOT_X( ofs[0] ) ;
	ew->pos.y += DOT_Y( ofs[1] ) ;
	if( ew->disp_dir ) dx = -dx ;
	ew->pos.x += dx ;
}

/*==============================================================*/
static void shl30_02_win_timer( PLAYER_WORK *ew )
{
	ew->timer1 = shl30_02_win_time[SHL30_02_PW( ew )->step] ;
}

/*==============================================================*/
static void shl30_02_win_init( PLAYER_WORK *ew )
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
	ew->prio = 0 ;
	SHL30_02_PW( ew )->step = 0 ;
	shl30_02_win_pos( ew ) ;
	shl30_02_win_timer( ew ) ;
	ew->char_no.b.no = 2 ;
	if( ew->sub_type ) ew->char_no.b.no = 4 ;
	pl_char_set( ew, 19, ew->char_no.b.no ) ;
}

/*--------------------------------------------------------------*/
static void shl30_02_win_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *nw ;

	if( -- ew->timer1 <= 0 ){
		SHL30_02_PW( ew )->step ++ ;
		shl30_02_win_timer( ew ) ;
		if( ew->timer1 == 0 ){
			ew->routine_0 ++ ;
			if( !ew->sub_type ) ew->master_w->routine_3 = 2 ;
			return ;
		} else if( ( nw = work_get( 0, W_SHL, LIST_END ) ) != 0 ){
			nw->work_func = shl30_02_main ;
			nw->master_w = ew->master_w ;
			nw->type = 8 ;
			nw->sub_type = ew->sub_type ;
			nw->ef_no = 0x3002 ;
			nw->pos = ew->pos ;
		}
	}
	shl30_02_win_pos( ew ) ;
	pl_char_move( ew ) ;
}

static void shl30_02_win_trail_init( PLAYER_WORK *ew );
static void shl30_02_win_trail_move( PLAYER_WORK *ew );

/*==============================================================*/
/*			The piece left behind at each step of the win's: it	*/
/*			blinks for a few frames								*/
/*==============================================================*/
static void shl30_02_win_trail_main( PLAYER_WORK *ew )
{
	static void ( *shl30_02_win_trail_jmp[] )( PLAYER_WORK *ew ) = {
		shl30_02_win_trail_init, shl30_02_win_trail_move, shl30_02_end, shl30_02_free,
	} ;

	if( ew->master_w->act_st != N_WIN ){
		shl30_02_free( ew ) ;
		return ;
	}
	shl30_02_win_trail_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl30_02_win_trail_init( PLAYER_WORK *ew )
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
	ew->prio = 0 ;
	ew->timer1 = 8 ;
	if( !ew->sub_type ){
		ew->char_no.b.no = 3 ;
	} else {
		ew->char_no.b.no = 5 ;
	}
	pl_char_set( ew, 19, ew->char_no.b.no ) ;
}

/*--------------------------------------------------------------*/
static void shl30_02_win_trail_move( PLAYER_WORK *ew )
{
	if( -- ew->timer1 <= 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	} else {
		pl_char_move( ew ) ;
	}
}

/*==============================================================*/
/*			The copy of his image held for as long as his		*/
/*			animation stays the same							*/
/*==============================================================*/
static void shl30_02_lose_main( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->char_no.w != ew->timer0 ){
		shl30_02_free( ew ) ;
		return ;
	}
	ew->pos = pl_w->pos ;
	if( ew->routine_0 == 0 ){
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
		if( !ew->sub_type ){
			ew->char_no.b.no = 13 ;
			ew->prio = 0 ;
		} else {
			ew->char_no.b.no = 14 ;
			ew->prio = 7 ;
		}
		pl_char_set( ew, Char_Effect, ew->char_no.b.no ) ;
	} else {
		pl_char_move( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl30_02_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl30_02_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl30_02.c" End of File	*/

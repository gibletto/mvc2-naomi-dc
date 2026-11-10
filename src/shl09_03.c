/* Iceman's effect 03 (character 09's shot 03): a beam of ice that grows out of his hands piece by
   piece while his move lasts, in one of four directions, with a root and a tip. */
#include "player.h"
#include "pl_char_old.h"

static void shl09_03_main( PLAYER_WORK *ew );

static Sint16 shl09_03_pos[8] = { -73, 64, -43, 104, -66, 44, -91, 88 } ;	/* the beam's start, x and y */
static Uint8 shl09_03_beam_char[4] = { 17, 22, 27, 17 } ;
static Sint16 shl09_03_step[8] = { 31, 0, 23, 17, 23, -17, 31, 0 } ;	/* from one piece to the next */
static Uint8 shl09_03_part_char[8] = { 16, 21, 26, 16, 15, 20, 25, 15 } ;	/* a piece's, then the last one's */
static Uint8 shl09_03_near_char[8] = { 29, 30, 31, 29, 15, 20, 25, 15 } ;	/* the same for the first piece */
static Uint8 shl09_03_root_char[4] = { 18, 23, 28, 18 } ;
static Uint8 shl09_03_tip_char[4] = { 14, 19, 24, 14 } ;

/****************************************************************/
void *shl09_03_set( PLAYER_WORK *pl_w, Sint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl09_03_main ;
		ew->type = 0 ;
		ew->sub_type = no ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x903 ;
	}
	return ew ;
}

/*==============================================================*/
/*			The beam's eleven pieces, the farthest first		*/
/*==============================================================*/
static void shl09_03_part_set( PLAYER_WORK *ew )
{
	PLAYER_WORK *pw ;
	Sint8 i ;
	Uint8 no = 11 ;

	for( i = 0 ; i < 11 ; i ++ ){
		if( ( pw = work_get( ew, W_SHL, LIST_BEHIND ) ) ){
			pw->work_func = shl09_03_main ;
			pw->type = 1 ;
			pw->sub_type = ew->sub_type ;
			pw->routine_2 = no ;
			no -- ;
			pw->master_w = ew->master_w ;
			pw->ef_w = ew ;
			pw->pl_type = ew->pl_type ;
			pw->ef_no = 0x903 ;
		}
	}
}

/*==============================================================*/
/*			The root (type 2) or the tip (type 3)				*/
/*==============================================================*/
static void shl09_03_cap_set( PLAYER_WORK *ew, Uint8 type )
{
	PLAYER_WORK *pw ;

	if( ( pw = work_get( ew, W_SHL, LIST_BEHIND ) ) ){
		pw->work_func = shl09_03_main ;
		pw->type = type ;
		pw->sub_type = ew->sub_type ;
		pw->master_w = ew->master_w ;
		pw->ef_w = ew ;
		pw->pl_type = ew->pl_type ;
		pw->ef_no = 0x903 ;
	}
}

/*==============================================================*/
static void shl09_03_color( PLAYER_WORK *ew )
{
	if( -- ew->timer1 < 0 ){
		ew->timer1 = 1 ;
		if( ++ ew->obj_flag > 3 ) ew->obj_flag = 0 ;
		ef_color_set( ew, 2, ew->pl_color * 4 + ew->obj_flag, 1 ) ;
	}
}

/*==============================================================*/
/*			At the player's hands								*/
/*==============================================================*/
static void shl09_03_pos_set( PLAYER_WORK *ew )
{
	Float x ;

	x = DOT_X( shl09_03_pos[ew->sub_type * 2] ) ;
	if( ew->disp_dir ) x = -x ;
	ew->pos.x = ew->master_w->pos.x + x ;
	ew->pos.y = ew->master_w->pos.y + DOT_Y( shl09_03_pos[ew->sub_type * 2 + 1] ) ;
}

/*==============================================================*/
/*			A piece, routine_2 steps out along the beam			*/
/*			(ef_w)												*/
/*==============================================================*/
static void shl09_03_part_pos( PLAYER_WORK *ew )
{
	Float x, y ;

	x = DOT_X( shl09_03_step[ew->sub_type * 2] ) ;
	if( !ew->disp_dir ) x = -x ;
	y = DOT_Y( shl09_03_step[ew->sub_type * 2 + 1] ) ;
	x = x * ew->routine_2 ;
	y = ew->routine_2 * y ;
	ew->pos.x = ew->ef_w->pos.x + x ;
	ew->pos.y = ew->ef_w->pos.y + y ;
}

/*==============================================================*/
/*			A piece is hidden past the beam's end (the			*/
/*			beam's routine_3 once it is stopped, else its		*/
/*			routine_2), and drawn as the last one at it.		*/
/*==============================================================*/
static void shl09_03_part_char_set( PLAYER_WORK *ew )
{
	Uint8 ofs = 0 ;

	ew->disp_flag = 1 ;
	if( ew->ef_w->routine_3 ){
		if( ew->ef_w->routine_3 < ew->routine_2 ) ew->disp_flag = 0 ;
		if( ew->ef_w->routine_3 == ew->routine_2 ) ofs = 4 ;
	} else {
		if( ew->ef_w->routine_2 < ew->routine_2 ) ew->disp_flag = 0 ;
		if( ew->ef_w->routine_2 == ew->routine_2 ) ofs = 4 ;
	}
	if( ew->routine_2 == 1 ) pl_char_set( ew, 21, shl09_03_near_char[ew->sub_type + ofs] ) ;
	else pl_char_set( ew, 21, shl09_03_part_char[ew->sub_type + ofs] ) ;
}

/*==============================================================*/
/*			At the beam's end									*/
/*==============================================================*/
static void shl09_03_tip_pos( PLAYER_WORK *ew )
{
	Uint8 n ;
	Float x, y ;

	if( ew->ef_w->routine_3 ) n = ew->ef_w->routine_3 ;
	else n = ew->ef_w->routine_2 ;
	x = DOT_X( shl09_03_step[ew->sub_type * 2] ) ;
	if( !ew->disp_dir ) x = -x ;
	y = DOT_Y( shl09_03_step[ew->sub_type * 2 + 1] ) ;
	x = x * n ;
	y = n * y ;
	ew->pos.x = ew->ef_w->pos.x + x ;
	ew->pos.y = ew->ef_w->pos.y + y ;
}

static void shl09_03_init( PLAYER_WORK *ew );
static void shl09_03_move( PLAYER_WORK *ew );
static void shl09_03_end( PLAYER_WORK *ew );
static void shl09_03_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl09_03_main( PLAYER_WORK *ew )
{
	static void ( *shl09_03_jmp[] )( PLAYER_WORK *ew ) = {
		shl09_03_init, shl09_03_move, shl09_03_end, shl09_03_free,
	} ;

	shl09_03_jmp[ew->routine_0]( ew ) ;
}

static void shl09_03_beam_init( PLAYER_WORK *ew );
static void shl09_03_part_init( PLAYER_WORK *ew );
static void shl09_03_cap_init( PLAYER_WORK *ew );

/*==============================================================*/
static void shl09_03_init( PLAYER_WORK *ew )
{
	static void ( *shl09_03_init_jmp[] )( PLAYER_WORK *ew ) = {
		shl09_03_beam_init, shl09_03_part_init, shl09_03_cap_init, shl09_03_cap_init,
	} ;

	shl09_03_init_jmp[ew->type]( ew ) ;
}

/*==============================================================*/
static void shl09_03_beam_init( PLAYER_WORK *ew )
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
	ew->disp_flag = 1 ;
	ew->sub_no = 6 ;	/* the hits it can give */
	shl09_03_pos_set( ew ) ;
	ew->prio = 11 ;
	ew->atck_flag = 0 ;
	ew->dm_flag = 0 ;
	ew->timer0 = 1 ;
	ew->routine_2 = 1 ;
	ew->routine_3 = 0 ;
	ew->timer1 = 1 ;
	ew->obj_flag = 0 ;
	pl_char_set( ew, 21, shl09_03_beam_char[ew->sub_type] ) ;
	shl09_03_cap_set( ew, 2 ) ;
	shl09_03_cap_set( ew, 3 ) ;
	shl09_03_part_set( ew ) ;
	shl09_03_color( ew ) ;
	pl_voice_req( ew, 30 ) ;
}

/*==============================================================*/
static void shl09_03_part_init( PLAYER_WORK *ew )
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
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = ew->sub_type + 52 ;
	HIT_RESET( ew ) ;
	shl09_03_part_pos( ew ) ;
	shl09_03_part_char_set( ew ) ;
}

/*==============================================================*/
static void shl09_03_cap_init( PLAYER_WORK *ew )
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
	ew->prio = 10 ;
	ew->atck_flag = 0 ;
	ew->dm_flag = 0 ;
	if( ew->type == 2 ){
		shl09_03_pos_set( ew ) ;
		pl_char_set( ew, 21, shl09_03_root_char[ew->sub_type] ) ;
	} else {
		shl09_03_tip_pos( ew ) ;
		pl_char_set( ew, 21, shl09_03_tip_char[ew->sub_type] ) ;
	}
}

static void shl09_03_beam_move( PLAYER_WORK *ew );
static void shl09_03_part_move( PLAYER_WORK *ew );
static void shl09_03_cap_move( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl09_03_move( PLAYER_WORK *ew )
{
	static void ( *shl09_03_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl09_03_beam_move, shl09_03_part_move, shl09_03_cap_move, shl09_03_cap_move,
	} ;

	shl09_03_move_jmp[ew->type]( ew ) ;
}

/*==============================================================*/
/*			Grows a piece a frame while the player holds		*/
/*			frames 6 to 13 of animation 21, and stops			*/
/*			growing from frame 10								*/
/*==============================================================*/
static void shl09_03_beam_move( PLAYER_WORK *ew )
{
	shl09_03_color( ew ) ;
	if( !ew->routine_1 ){
		if( ew->master_w->char_no.b.grp != Char_Special || ew->master_w->char_no.b.no < 6 || ew->master_w->char_no.b.no > 13 ){
			ew->routine_0 ++ ;
			shl09_03_end( ew ) ;
		} else {
			if( ew->master_w->char_no.b.grp == Char_Special && ew->master_w->char_no.b.no >= 10 && !( ew->master_w->char_no.b.no > 13 ) ){
				ew->routine_1 ++ ;
			}
			ew->routine_3 = 0 ;
			shl09_03_pos_set( ew ) ;
			if( -- ew->timer0 <= 0 && ew->routine_2 < 11 ){
				ew->timer0 = 1 ;
				ew->routine_2 ++ ;
			}
		}
	} else {
		shl09_03_pos_set( ew ) ;
		if( pl_char_move( ew ) < 0 ) shl09_03_end( ew ) ;
	}
}

/*==============================================================*/
/*			A piece that hits stops the beam there; so does		*/
/*			one that reaches the ground							*/
/*==============================================================*/
static void shl09_03_part_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *mw = ew->master_w ;

	if( !ew->routine_1 ){
		if( ew->master_w->char_no.b.grp != Char_Special || ew->master_w->char_no.b.no < 6 || ew->master_w->char_no.b.no > 13 ){
			ew->routine_0 ++ ;
			shl09_03_end( ew ) ;
		} else {
			if( ew->master_w->char_no.b.grp == Char_Special && ew->master_w->char_no.b.no >= 10 && !( ew->master_w->char_no.b.no > 13 ) ){
				ew->routine_1 ++ ;
				pl_voice_req( ew, 43 ) ;
			}
			if( ew->hit_flag && !ew->ef_w->routine_3 ){
				ew->ef_w->routine_3 = ew->routine_2 ;
				if( ew->ef_w->sub_no > 1 ){
					ew->ef_w->sub_no -- ;
					ew->atck_no = ew->sub_type + 52 ;
					HIT_RESET( ew ) ;
				}
			}
			if( ew->pos.y <= mw->ground_y && ew->ef_w->routine_3 == 0 && ew->ef_w->routine_2 >= ew->routine_2 ){
				ew->ef_w->routine_3 = ew->routine_2 ;
			}
			shl09_03_part_pos( ew ) ;
			shl09_03_part_char_set( ew ) ;
			if( !ew->ef_w->routine_3 || ew->ef_w->routine_3 == ew->routine_2 ){
				if( ew->ef_w->routine_2 >= ew->routine_2 ){
					if( ew->ef_w->sub_no > 0 ){
						hit_work_set( ew ) ;
					}
				}
			}
		}
	} else {
		shl09_03_part_pos( ew ) ;
		if( pl_char_move( ew ) < 0 ) shl09_03_end( ew ) ;
	}
}

/*==============================================================*/
/*			The root and the tip, while the player holds		*/
/*			frames 6 to 9										*/
/*==============================================================*/
static void shl09_03_cap_move( PLAYER_WORK *ew )
{
	if( ew->master_w->char_no.b.grp != Char_Special || ew->master_w->char_no.b.no < 6 || ew->master_w->char_no.b.no > 9 ){
		ew->routine_0 ++ ;
		shl09_03_end( ew ) ;
		return ;
	}
	if( ew->type == 2 ) shl09_03_pos_set( ew ) ;
	else shl09_03_tip_pos( ew ) ;
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl09_03_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl09_03_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl09_03.c" End of File	*/

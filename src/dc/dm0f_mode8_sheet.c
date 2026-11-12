/* DM0F, mode 8's ranking sheet: three lines of rank_sa (Sys->routine_1 0) or rank_score from
   place Sys->routine_3, a fourth that scrolls in, and on each line its place, team, ending mark,
   wins, time, name and score. */
#include "player.h"

void dm0f_mode8_sheet_move( PLAYER_WORK *w ) ;
void dm0f_mode8_scroll( PLAYER_WORK *w ) ;
void dm0f_mode8_scroll_init( PLAYER_WORK *w ) ;
void dm0f_mode8_scroll_move( PLAYER_WORK *w ) ;
void dm0f_mode8_next_set( void ) ;
void dm0f_mode8_follow( PLAYER_WORK *w ) ;
void dm0f_mode8_rank_set( PLAYER_WORK *mw, int line ) ;
void dm0f_mode8_rank_move( PLAYER_WORK *w ) ;
void dm0f_mode8_face_set( PLAYER_WORK *mw, int line, int n ) ;
void dm0f_mode8_face_move( PLAYER_WORK *w ) ;
void dm0f_mode8_end_set( PLAYER_WORK *mw, int line ) ;
void dm0f_mode8_end_move( PLAYER_WORK *w ) ;
void dm0f_mode8_char_set( PLAYER_WORK *fw ) ;
void dm0f_mode8_char_move( PLAYER_WORK *w ) ;
void dm0f_mode8_label_set( PLAYER_WORK *mw, int line ) ;
void dm0f_mode8_label_move( PLAYER_WORK *w ) ;
void dm0f_mode8_plural_set( PLAYER_WORK *mw, int line ) ;
void dm0f_mode8_plural_move( PLAYER_WORK *w ) ;
void dm0f_mode8_place_set( PLAYER_WORK *mw, int line, int n ) ;
void dm0f_mode8_place_move( PLAYER_WORK *w ) ;
void dm0f_mode8_win_set( PLAYER_WORK *mw, int line, int n ) ;
void dm0f_mode8_win_move( PLAYER_WORK *w ) ;
void dm0f_mode8_min_set( PLAYER_WORK *mw, int line, int n ) ;
void dm0f_mode8_min_move( PLAYER_WORK *w ) ;
void dm0f_mode8_sec_set( PLAYER_WORK *mw, int line, int n ) ;
void dm0f_mode8_sec_move( PLAYER_WORK *w ) ;
void dm0f_mode8_csec_set( PLAYER_WORK *mw, int line, int n ) ;
void dm0f_mode8_csec_move( PLAYER_WORK *w ) ;
void dm0f_mode8_colon_set( PLAYER_WORK *mw, int line ) ;
void dm0f_mode8_colon_move( PLAYER_WORK *w ) ;
void dm0f_mode8_nowin_set( PLAYER_WORK *mw, int line ) ;
void dm0f_mode8_nowin_move( PLAYER_WORK *w ) ;
void dm0f_mode8_name_set( PLAYER_WORK *mw, int line, int n ) ;
void dm0f_mode8_name_move( PLAYER_WORK *w ) ;
void dm0f_mode8_score_set( PLAYER_WORK *mw, int line, int n ) ;
void dm0f_mode8_score_move( PLAYER_WORK *w ) ;
Uint8 dm0f_mode8_score_ck( PLAYER_WORK *w, int n ) ;
void dm0f_mode8_col_set( PLAYER_WORK *w, Float r, Float g, Float b ) ;

/* a name's letters: 0xf0 and up the digits, the rest letter models */
static const Uint8 dm0f_mode8_font[44] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b,
	0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9,
	0x1f, 0x20, 0x21, 0x1d, 0x1c, 0x1e
} ;
static const VEC3 dm0f_mode8_scale = { 0.65f, 0.65f, 1.0f } ;	/* the digits' and letters' */

static Float dm0f_mode8_y[3] = { 2.6f, -0.74f, -4.08f } ;	/* the lines' */
static void ( *dm0f_mode8_scroll_jmp[2] )( PLAYER_WORK *w ) = { dm0f_mode8_scroll_init, dm0f_mode8_scroll_move } ;
static Float dm0f_mode8_next_y[2] = { 5.94f, -7.42f } ;	/* above and below the three */
static Float dm0f_mode8_rank_y[3] = { -0.6f, -0.6f, -0.6f } ;
static Float dm0f_mode8_face_x[3] = { -3.15f, 0.68f, 4.51f } ;
static Float dm0f_mode8_unused[3] = { 0.35000014f, 0.34f, 0.32999992f } ;
static Float dm0f_mode8_score_x[11] = {
	2.41f, 1.75f, 1.11f, 0.46f, -0.19f, -0.85f, -1.5f, -2.14f, -2.79f, -3.43f, -4.1f
} ;

/****************************************************************/
/*			The sheet: three lines and the one to scroll in		*/
/****************************************************************/
void dm0f_mode8_sheet_set( void )
{
	PLAYER_WORK *w ;
	int i ;

	for( i = 0 ; i < 3 ; i ++ ){
		if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return ;
		w->disp_flag = 1 ;
		w->work_func = dm0f_mode8_sheet_move ;
		w->obj = obj_dm0f->obj[15] ;
		w->obj_flag = OBJ_POS | OBJ_TRNSL | OBJ_FADE ;
		w->pos.x = 0.0f ;
		w->pos.y = dm0f_mode8_y[i] ;
		w->pos.z = -13.0f ;
		w->timer1 = i ;
		w->fade.trnsl = 1.0f ;
		w->sub_no = Sys->routine_3 ;
		dm0f_mode8_rank_set( w, i ) ;
		dm0f_mode8_face_set( w, i, 0 ) ;
		dm0f_mode8_face_set( w, i, 1 ) ;
		dm0f_mode8_face_set( w, i, 2 ) ;
		dm0f_mode8_end_set( w, i ) ;
		dm0f_mode8_label_set( w, i ) ;
		dm0f_mode8_place_set( w, i, 0 ) ;
		dm0f_mode8_place_set( w, i, 1 ) ;
		if( !Sys->routine_1 ) dm0f_mode8_nowin_set( w, i ) ;
		else {
			dm0f_mode8_win_set( w, i, 0 ) ;
			dm0f_mode8_win_set( w, i, 1 ) ;
		}
		dm0f_mode8_min_set( w, i, 0 ) ;
		dm0f_mode8_min_set( w, i, 1 ) ;
		dm0f_mode8_sec_set( w, i, 0 ) ;
		dm0f_mode8_sec_set( w, i, 1 ) ;
		dm0f_mode8_csec_set( w, i, 0 ) ;
		dm0f_mode8_csec_set( w, i, 1 ) ;
		dm0f_mode8_colon_set( w, i ) ;
		dm0f_mode8_name_set( w, i, 0 ) ;
		dm0f_mode8_name_set( w, i, 1 ) ;
		dm0f_mode8_name_set( w, i, 2 ) ;
		dm0f_mode8_score_set( w, i, 0 ) ;
		dm0f_mode8_score_set( w, i, 1 ) ;
		dm0f_mode8_score_set( w, i, 2 ) ;
		dm0f_mode8_score_set( w, i, 3 ) ;
		dm0f_mode8_score_set( w, i, 4 ) ;
		dm0f_mode8_score_set( w, i, 5 ) ;
		dm0f_mode8_score_set( w, i, 6 ) ;
		dm0f_mode8_score_set( w, i, 7 ) ;
		dm0f_mode8_score_set( w, i, 8 ) ;
		dm0f_mode8_score_set( w, i, 9 ) ;
		dm0f_mode8_score_set( w, i, 10 ) ;
	}
	dm0f_mode8_next_set() ;
}

/****************************************************************/
/*			A line: scrolled by Sys->exit_flag (1 down, 2 up),	*/
/*			coloured by its place								*/
/****************************************************************/
void dm0f_mode8_sheet_move( PLAYER_WORK *w )
{
	SYSTEM_WORK *sys ;

	sys = Sys ;
	if( !w->routine_0 && sys->exit_flag ) w->routine_0 = sys->exit_flag == 1 ? 1 : 2 ;
	if( w->routine_0 ) dm0f_mode8_scroll( w ) ;
	switch( Sys->routine_3 + w->timer1 ){
	case 0 :
		w->fade.r = 1.0f ;
		w->fade.g = 0.9f ;
		w->fade.b = 0.3f ;
		break ;
	case 1 :
		w->fade.r = 0.9f ;
		w->fade.g = 0.9f ;
		w->fade.b = 0.9f ;
		break ;
	case 2 :
		w->fade.r = 1.0f ;
		w->fade.g = 0.6f ;
		w->fade.b = 0.15f ;
		break ;
	default :
		w->fade.r = 0.5f ;
		w->fade.g = 0.5f ;
		w->fade.b = 0.5f ;
		break ;
	}
}

void dm0f_mode8_scroll( PLAYER_WORK *w )
{
	dm0f_mode8_scroll_jmp[w->routine_1]( w ) ;
}

/****************************************************************/
/*			A scroll: ten frames; the line scrolling in fades	*/
/*			in, the one scrolling out fades out					*/
/****************************************************************/
void dm0f_mode8_scroll_init( PLAYER_WORK *w )
{
	if( w->work3 ){
		w->disp_flag = 1 ;
		w->fade.trnsl = 0.0f ;
		w->timer0 = 10 ;
		if( w->routine_0 == 1 ){
			w->pos.y = dm0f_mode8_y[0] + 3.34f ;
			w->spd.y = -0.334f ;
			w->timer1 = -1 ;
		} else {
			w->pos.y = dm0f_mode8_y[2] + -3.34f ;
			w->spd.y = 0.334f ;
			w->timer1 = 3 ;
		}
	} else {
		w->disp_flag = 1 ;
		w->timer0 = 10 ;
		if( w->routine_0 == 1 ) w->spd.y = -0.334f ;
		else w->spd.y = 0.334f ;
	}
	w->routine_1 ++ ;
}

void dm0f_mode8_scroll_move( PLAYER_WORK *w )
{
	if( w->timer0 ){
		w->timer0 -- ;
		if( w->work3 ){
			w->pos.y += w->spd.y ;
			w->fade.trnsl += 0.1f ;
		} else {
			if( w->routine_0 == 1 ){
				if( w->timer1 == 2 ) w->fade.trnsl -= 0.1f ;
			} else if( !w->timer1 ) w->fade.trnsl -= 0.1f ;
			w->pos.y += w->spd.y ;
		}
	} else {
		w->routine_1 = w->routine_0 = 0 ;
		if( w->work3 ){
			w->disp_flag = 0 ;
			w->fade.trnsl = 0.0f ;
		} else {
			w->disp_flag = 1 ;
			w->fade.trnsl = 1.0f ;
			w->pos.y = dm0f_mode8_y[w->timer1] ;
		}
	}
}

/****************************************************************/
/*			The line that scrolls in, hidden below				*/
/****************************************************************/
void dm0f_mode8_next_set( void )
{
	PLAYER_WORK *w ;

	if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return ;
	w->disp_flag = 0 ;
	w->work_func = dm0f_mode8_sheet_move ;
	w->obj = obj_dm0f->obj[15] ;
	w->obj_flag = OBJ_POS | OBJ_TRNSL | OBJ_FADE ;
	w->work3 = 1 ;
	w->pos.x = 0.0f ;
	w->pos.y = dm0f_mode8_next_y[1] ;
	w->pos.z = -13.0f ;
	w->timer1 = 1 ;
	w->fade.trnsl = 1.0f ;
	w->sub_no = Sys->routine_3 ;
	if( !Sys->routine_1 ) dm0f_mode8_nowin_set( w, 1 ) ;
	else {
		dm0f_mode8_win_set( w, 1, 0 ) ;
		dm0f_mode8_win_set( w, 1, 1 ) ;
	}
	dm0f_mode8_rank_set( w, 1 ) ;
	dm0f_mode8_face_set( w, 1, 0 ) ;
	dm0f_mode8_face_set( w, 1, 1 ) ;
	dm0f_mode8_face_set( w, 1, 2 ) ;
	dm0f_mode8_end_set( w, 1 ) ;
	dm0f_mode8_label_set( w, 1 ) ;
	dm0f_mode8_place_set( w, 1, 0 ) ;
	dm0f_mode8_place_set( w, 1, 1 ) ;
	dm0f_mode8_min_set( w, 1, 0 ) ;
	dm0f_mode8_min_set( w, 1, 1 ) ;
	dm0f_mode8_sec_set( w, 1, 0 ) ;
	dm0f_mode8_sec_set( w, 1, 1 ) ;
	dm0f_mode8_csec_set( w, 1, 0 ) ;
	dm0f_mode8_csec_set( w, 1, 1 ) ;
	dm0f_mode8_colon_set( w, 1 ) ;
	dm0f_mode8_name_set( w, 1, 0 ) ;
	dm0f_mode8_name_set( w, 1, 1 ) ;
	dm0f_mode8_name_set( w, 1, 2 ) ;
	dm0f_mode8_score_set( w, 1, 0 ) ;
	dm0f_mode8_score_set( w, 1, 1 ) ;
	dm0f_mode8_score_set( w, 1, 2 ) ;
	dm0f_mode8_score_set( w, 1, 3 ) ;
	dm0f_mode8_score_set( w, 1, 4 ) ;
	dm0f_mode8_score_set( w, 1, 5 ) ;
	dm0f_mode8_score_set( w, 1, 6 ) ;
	dm0f_mode8_score_set( w, 1, 7 ) ;
	dm0f_mode8_score_set( w, 1, 8 ) ;
	dm0f_mode8_score_set( w, 1, 9 ) ;
	dm0f_mode8_score_set( w, 1, 10 ) ;
}

/****************************************************************/
/*			A part of a line: it moves to the place of the		*/
/*			line it is on when that one scrolls in				*/
/****************************************************************/
void dm0f_mode8_follow( PLAYER_WORK *w )
{
	if( w->master_w->work3 ){
		if( w->master_w->routine_0 == 1 ) w->timer1 = w->master_w->timer1 ;
		else w->timer1 = w->master_w->timer1 ;
	}
}

/****************************************************************/
/*			The place's mark: 1st, 2nd, 3rd or the rest			*/
/****************************************************************/
void dm0f_mode8_rank_set( PLAYER_WORK *mw, int line )
{
	PLAYER_WORK *w ;

	if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = dm0f_mode8_rank_move ;
	w->obj_flag = OBJ_POS | OBJ_TRNSL ;
	w->pos.x = -9.0f ;
	w->pos.y = dm0f_mode8_rank_y[line] ;
	w->pos.z = 0.13f ;
	w->timer1 = line ;
	w->master_w = mw ;
	w->base_mtx = &mw->obj_mtx ;
}

void dm0f_mode8_rank_move( PLAYER_WORK *w )
{
	int n ;

	dm0f_mode8_follow( w ) ;
	w->disp_flag = w->master_w->disp_flag ;
	if( Sys->routine_3 + w->timer1 < 3 ){
		switch( Sys->routine_3 + w->timer1 ){
		case 0 :
			n = 22 ;
			break ;
		case 1 :
			n = 23 ;
			break ;
		case 2 :
			n = 24 ;
			break ;
		}
	} else n = 25 ;
	w->obj = obj_dm0f->obj[n] ;
	w->fade.trnsl = w->master_w->fade.trnsl ;
}

/****************************************************************/
/*			A team member's face, and its name under it			*/
/****************************************************************/
void dm0f_mode8_face_set( PLAYER_WORK *mw, int line, int n )
{
	PLAYER_WORK *w ;

	if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = dm0f_mode8_face_move ;
	w->obj_flag = OBJ_POS | OBJ_TRNSL ;
	w->pos.x = dm0f_mode8_face_x[n] ;
	w->pos.y = 0.34f ;
	w->pos.z = 0.069999695f ;
	w->base_mtx = &mw->obj_mtx ;
	w->timer1 = line ;
	w->sub_type = n ;
	w->master_w = mw ;
	dm0f_mode8_char_set( w ) ;
}

void dm0f_mode8_face_move( PLAYER_WORK *w )
{
	dm0f_mode8_follow( w ) ;
	w->disp_flag = w->master_w->disp_flag ;
	if( !Sys->routine_1 ) w->obj = obj_dm0f->obj[rank_sa[Sys->routine_3 + w->timer1].pl_personal[w->sub_type] + 49] ;
	else w->obj = obj_dm0f->obj[rank_score[Sys->routine_3 + w->timer1].pl_personal[w->sub_type] + 49] ;
	w->fade.trnsl = w->master_w->fade.trnsl ;
}

/****************************************************************/
/*			The ending mark: the team saw the ending			*/
/****************************************************************/
void dm0f_mode8_end_set( PLAYER_WORK *mw, int line )
{
	PLAYER_WORK *w ;

	if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return ;
	w->work_func = dm0f_mode8_end_move ;
	w->obj = obj_dm0f->obj[52] ;
	w->obj_flag = OBJ_POS | OBJ_TRNSL | OBJ_FADE ;
	w->pos.x = -7.5f ;
	w->pos.y = -0.96f ;
	w->pos.z = 0.07f ;
	w->base_mtx = &mw->obj_mtx ;
	w->timer1 = line ;
	w->master_w = mw ;
}

void dm0f_mode8_end_move( PLAYER_WORK *w )
{
	if( !( Sys->routine_3 + w->timer1 ) ){
		w->fade.r = 1.0f ;
		w->fade.g = 1.0f ;
		w->fade.b = 0.0f ;
	} else {
		w->fade.r = 1.0f ;
		w->fade.g = 1.0f ;
		w->fade.b = 1.0f ;
	}
	dm0f_mode8_follow( w ) ;
	if( !Sys->routine_1 ){
		if( rank_sa[Sys->routine_3 + w->timer1].ending_flag ){
			w->disp_flag = 1 ;
			w->disp_flag = w->master_w->disp_flag ;
		} else w->disp_flag = 0 ;
	} else if( rank_score[Sys->routine_3 + w->timer1].ending_flag ){
		w->disp_flag = 1 ;
		w->disp_flag = w->master_w->disp_flag ;
	} else w->disp_flag = 0 ;
	w->fade.trnsl = w->master_w->fade.trnsl ;
}

/****************************************************************/
/*			A face's name										*/
/****************************************************************/
void dm0f_mode8_char_set( PLAYER_WORK *fw )
{
	PLAYER_WORK *w ;

	if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = dm0f_mode8_char_move ;
	w->obj = obj_dm0f->obj[58] ;
	w->obj_flag = OBJ_POS | OBJ_TRNSL ;
	w->pos.x = -1.6f ;
	w->pos.y = -0.09f ;
	w->ef_w = fw ;
	w->base_mtx = &fw->obj_mtx ;
	w->master_w = fw ;
}

void dm0f_mode8_char_move( PLAYER_WORK *w )
{
	PLAYER_WORK *fw ;
	Sint8 n ;

	fw = w->ef_w ;
	if( !Sys->routine_1 ) n = rank_sa[Sys->routine_3 + fw->timer1].load_type[fw->sub_type] ;
	else n = rank_score[Sys->routine_3 + fw->timer1].load_type[fw->sub_type] ;
	w->disp_flag = w->master_w->disp_flag ;
	if( n > 23 ) n -= 3 ;
	w->obj = obj_dm0f->obj[n + 58] ;
	w->fade.trnsl = w->master_w->fade.trnsl ;
}

/****************************************************************/
/*			"WIN", and its "S" for more than one				*/
/****************************************************************/
void dm0f_mode8_label_set( PLAYER_WORK *mw, int line )
{
	PLAYER_WORK *w ;

	if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = dm0f_mode8_label_move ;
	w->obj = obj_dm0f->obj[47] ;
	w->obj_flag = OBJ_POS | OBJ_TRNSL ;
	w->pos.x = 7.82f ;
	w->pos.y = 0.34f ;
	w->pos.z = 0.07f ;
	w->base_mtx = &mw->obj_mtx ;
	w->timer1 = line ;
	w->master_w = mw ;
	dm0f_mode8_plural_set( w, line ) ;
}

void dm0f_mode8_label_move( PLAYER_WORK *w )
{
	dm0f_mode8_follow( w ) ;
	w->disp_flag = w->master_w->disp_flag ;
	w->fade.trnsl = w->master_w->fade.trnsl ;
}

void dm0f_mode8_plural_set( PLAYER_WORK *mw, int line )
{
	PLAYER_WORK *w ;

	if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return ;
	w->obj_flag = OBJ_POS | OBJ_TRNSL ;
	w->work_func = dm0f_mode8_plural_move ;
	w->obj = obj_dm0f->obj[48] ;
	w->base_mtx = &mw->obj_mtx ;
	w->timer1 = line ;
	w->master_w = mw ;
}

void dm0f_mode8_plural_move( PLAYER_WORK *w )
{
	dm0f_mode8_follow( w ) ;
	if( !Sys->routine_1 ){
		if( rank_sa[Sys->routine_3 + w->master_w->timer1].win_max > 1 ){
			w->disp_flag = 1 ;
			w->disp_flag = w->master_w->disp_flag ;
		} else w->disp_flag = 0 ;
	} else if( rank_score[Sys->routine_3 + w->master_w->timer1].win_max > 1 ){
		w->disp_flag = 1 ;
		w->disp_flag = w->master_w->disp_flag ;
	} else w->disp_flag = 0 ;
	w->fade.trnsl = w->master_w->fade.trnsl ;
}

/****************************************************************/
/*			The place's two digits (n 1 the tens)				*/
/****************************************************************/
void dm0f_mode8_place_set( PLAYER_WORK *mw, int line, int n )
{
	PLAYER_WORK *w ;

	if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return ;
	w->pos.x = n ? -10.9f : -10.1f ;
	w->pos.y = -0.54f ;
	w->pos.z = 0.13f ;
	w->disp_flag = 1 ;
	w->work_func = dm0f_mode8_place_move ;
	w->obj_flag = OBJ_POS | OBJ_TRNSL ;
	w->timer1 = line ;
	w->sub_no = n ;
	w->base_mtx = &mw->obj_mtx ;
	w->master_w = mw ;
}

void dm0f_mode8_place_move( PLAYER_WORK *w )
{
	int ten, one ;

	dm0f_mode8_follow( w ) ;
	if( w->sub_no ){
		ten = ( Sys->routine_3 + w->timer1 + 1 ) / 10 ;
		if( ten ){
			w->disp_flag = 1 ;
			w->disp_flag = w->master_w->disp_flag ;
			w->obj = obj_dm0f->obj[ten + 26] ;
		} else w->disp_flag = 0 ;
	} else {
		one = ( Sys->routine_3 + w->timer1 + 1 ) % 10 ;
		w->disp_flag = w->master_w->disp_flag ;
		w->obj = obj_dm0f->obj[one + 26] ;
	}
	w->fade.trnsl = w->master_w->fade.trnsl ;
}

/****************************************************************/
/*			The wins' two digits, in rank_score					*/
/****************************************************************/
void dm0f_mode8_win_set( PLAYER_WORK *mw, int line, int n )
{
	PLAYER_WORK *w ;

	if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = dm0f_mode8_win_move ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->timer1 = line ;
	w->sub_no = n ;
	w->pos.x = n ? 6.43f : 7.1f ;
	w->pos.y = 0.38f ;
	w->pos.z = 0.07f ;
	w->scale = dm0f_mode8_scale ;
	dm0f_mode8_col_set( w, 0.0f, 1.0f, 1.0f ) ;
	w->base_mtx = &mw->obj_mtx ;
	w->master_w = mw ;
}

void dm0f_mode8_win_move( PLAYER_WORK *w )
{
	int ten, one ;

	dm0f_mode8_follow( w ) ;
	if( w->sub_no ){
		if( !Sys->routine_1 ) ten = rank_sa[Sys->routine_3 + w->timer1].win_max / 10 ;
		else ten = rank_score[Sys->routine_3 + w->timer1].win_max / 10 ;
		if( ten ){
			w->disp_flag = 1 ;
			w->disp_flag = w->master_w->disp_flag ;
			w->obj = obj_dm0f->obj[ten + 37] ;
		} else w->disp_flag = 0 ;
	} else {
		if( !Sys->routine_1 ) one = rank_sa[Sys->routine_3 + w->timer1].win_max % 10 ;
		else one = rank_score[Sys->routine_3 + w->timer1].win_max % 10 ;
		w->disp_flag = w->master_w->disp_flag ;
		w->obj = obj_dm0f->obj[one + 37] ;
	}
	w->fade.trnsl = w->master_w->fade.trnsl ;
}

/****************************************************************/
/*			The time played: minutes, seconds, hundredths		*/
/****************************************************************/
void dm0f_mode8_min_set( PLAYER_WORK *mw, int line, int n )
{
	PLAYER_WORK *w ;

	if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = dm0f_mode8_min_move ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->timer1 = line ;
	w->sub_no = n ;
	w->pos.x = n ? 6.26f : 6.86f ;
	w->pos.y = -0.99f ;
	w->pos.z = 0.07f ;
	w->scale = dm0f_mode8_scale ;
	dm0f_mode8_col_set( w, 1.0f, 0.3744f, 0.6825f ) ;
	w->base_mtx = &mw->obj_mtx ;
	w->master_w = mw ;
}

void dm0f_mode8_min_move( PLAYER_WORK *w )
{
	int ten, one ;

	dm0f_mode8_follow( w ) ;
	w->disp_flag = w->master_w->disp_flag ;
	if( w->sub_no ){
		if( !Sys->routine_1 ) ten = rank_sa[Sys->routine_3 + w->timer1].play_min / 10 ;
		else ten = rank_score[Sys->routine_3 + w->timer1].play_min / 10 ;
		w->obj = obj_dm0f->obj[ten + 37] ;
	} else {
		if( !Sys->routine_1 ) one = rank_sa[Sys->routine_3 + w->timer1].play_min % 10 ;
		else one = rank_score[Sys->routine_3 + w->timer1].play_min % 10 ;
		w->obj = obj_dm0f->obj[one + 37] ;
	}
	w->fade.trnsl = w->master_w->fade.trnsl ;
}

void dm0f_mode8_sec_set( PLAYER_WORK *mw, int line, int n )
{
	PLAYER_WORK *w ;

	if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = dm0f_mode8_sec_move ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->timer1 = line ;
	w->sub_no = n ;
	w->pos.x = n ? 8.0f : 8.63f ;
	w->pos.y = -0.99f ;
	w->pos.z = 0.07f ;
	w->scale = dm0f_mode8_scale ;
	dm0f_mode8_col_set( w, 1.0f, 0.3744f, 0.6825f ) ;
	w->base_mtx = &mw->obj_mtx ;
	w->master_w = mw ;
}

void dm0f_mode8_sec_move( PLAYER_WORK *w )
{
	int ten, one ;

	dm0f_mode8_follow( w ) ;
	w->disp_flag = w->master_w->disp_flag ;
	if( w->sub_no ){
		if( !Sys->routine_1 ) ten = rank_sa[Sys->routine_3 + w->timer1].play_sec / 10 ;
		else ten = rank_score[Sys->routine_3 + w->timer1].play_sec / 10 ;
		w->obj = obj_dm0f->obj[ten + 37] ;
	} else {
		if( !Sys->routine_1 ) one = rank_sa[Sys->routine_3 + w->timer1].play_sec % 10 ;
		else one = rank_score[Sys->routine_3 + w->timer1].play_sec % 10 ;
		w->obj = obj_dm0f->obj[one + 37] ;
	}
	w->fade.trnsl = w->master_w->fade.trnsl ;
}

void dm0f_mode8_csec_set( PLAYER_WORK *mw, int line, int n )
{
	PLAYER_WORK *w ;

	if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = dm0f_mode8_csec_move ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->timer1 = line ;
	w->sub_no = n ;
	w->pos.x = n ? 9.91f : 10.55f ;
	w->pos.y = -0.99f ;
	w->pos.z = 0.07f ;
	w->scale = dm0f_mode8_scale ;
	dm0f_mode8_col_set( w, 1.0f, 0.3744f, 0.6825f ) ;
	w->base_mtx = &mw->obj_mtx ;
	w->master_w = mw ;
}

void dm0f_mode8_csec_move( PLAYER_WORK *w )
{
	int ten, one ;

	dm0f_mode8_follow( w ) ;
	w->disp_flag = w->master_w->disp_flag ;
	if( w->sub_no ){
		if( !Sys->routine_1 ) ten = rank_sa[Sys->routine_3 + w->timer1].play_csec / 10 ;
		else ten = rank_score[Sys->routine_3 + w->timer1].play_csec / 10 ;
		w->obj = obj_dm0f->obj[ten + 37] ;
	} else {
		if( !Sys->routine_1 ) one = rank_sa[Sys->routine_3 + w->timer1].play_csec % 10 ;
		else one = rank_score[Sys->routine_3 + w->timer1].play_csec % 10 ;
		w->obj = obj_dm0f->obj[one + 37] ;
	}
	w->fade.trnsl = w->master_w->fade.trnsl ;
}

/****************************************************************/
/*			The time's colons									*/
/****************************************************************/
void dm0f_mode8_colon_set( PLAYER_WORK *mw, int line )
{
	PLAYER_WORK *w ;

	if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = dm0f_mode8_colon_move ;
	w->obj_flag = OBJ_POS | OBJ_TRNSL | OBJ_FADE ;
	w->timer1 = line ;
	w->pos.x = 8.33f ;
	w->pos.y = -0.99f ;
	w->pos.z = 0.08f ;
	dm0f_mode8_col_set( w, 1.0f, 0.6201f, 0.8073f ) ;
	w->obj = obj_dm0f->obj[56] ;
	w->base_mtx = &mw->obj_mtx ;
	w->master_w = mw ;
}

void dm0f_mode8_colon_move( PLAYER_WORK *w )
{
	dm0f_mode8_follow( w ) ;
	w->disp_flag = w->master_w->disp_flag ;
	w->fade.trnsl = w->master_w->fade.trnsl ;
}

/****************************************************************/
/*			In rank_sa, which keeps no wins, a mark in their	*/
/*			place												*/
/****************************************************************/
void dm0f_mode8_nowin_set( PLAYER_WORK *mw, int line )
{
	PLAYER_WORK *w ;

	if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = dm0f_mode8_nowin_move ;
	w->obj_flag = OBJ_POS | OBJ_TRNSL | OBJ_FADE ;
	w->pos.x = 6.77f ;
	w->pos.y = 0.37f ;
	w->pos.z = 0.08f ;
	w->timer1 = line ;
	dm0f_mode8_col_set( w, 1.0f, 1.0f, 0.0f ) ;
	w->obj = obj_dm0f->obj[57] ;
	w->base_mtx = &mw->obj_mtx ;
	w->master_w = mw ;
}

void dm0f_mode8_nowin_move( PLAYER_WORK *w )
{
	dm0f_mode8_follow( w ) ;
	w->disp_flag = w->master_w->disp_flag ;
	w->fade.trnsl = w->master_w->fade.trnsl ;
}

/****************************************************************/
/*			The name's three letters							*/
/****************************************************************/
void dm0f_mode8_name_set( PLAYER_WORK *mw, int line, int n )
{
	PLAYER_WORK *w ;

	if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = dm0f_mode8_name_move ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	switch( n ){
	case 0 :
		w->pos.x = -7.93f ;
		break ;
	case 1 :
		w->pos.x = -7.28f ;
		break ;
	case 2 :
		w->pos.x = -6.63f ;
		break ;
	}
	w->pos.y = 0.38f ;
	w->pos.z = 0.07f ;
	w->timer1 = line ;
	w->sub_no = n ;
	dm0f_mode8_col_set( w, 1.0f, 1.0f, 0.0f ) ;
	w->scale = dm0f_mode8_scale ;
	w->base_mtx = &mw->obj_mtx ;
	w->master_w = mw ;
}

void dm0f_mode8_name_move( PLAYER_WORK *w )
{
	const Uint8 *p ;

	p = dm0f_mode8_font ;
	dm0f_mode8_follow( w ) ;
	w->disp_flag = w->master_w->disp_flag ;
	if( !Sys->routine_1 ) p += rank_sa[Sys->routine_3 + w->timer1].name[w->sub_no] ;
	else p += rank_score[Sys->routine_3 + w->timer1].name[w->sub_no] ;
	if( *p >= 0xf0 ) w->obj = obj_dm0f->obj[*p - 0xf0 + 37] ;
	else w->obj = obj_dm0f->obj[*p + 314] ;
	w->fade.trnsl = w->master_w->fade.trnsl ;
}

/****************************************************************/
/*			The score's eleven digits, without leading zeros	*/
/****************************************************************/
void dm0f_mode8_score_set( PLAYER_WORK *mw, int line, int n )
{
	PLAYER_WORK *w ;

	if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = dm0f_mode8_score_move ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->pos.x = dm0f_mode8_score_x[n] ;
	switch( line ){
	case 0 :
		w->pos.y = -0.9799999f ;
		break ;
	case 1 :
		w->pos.y = -0.98f ;
		break ;
	case 2 :
		w->pos.y = -0.98f ;
		break ;
	}
	w->pos.z = 0.07f ;
	w->timer1 = line ;
	w->sub_no = n ;
	dm0f_mode8_col_set( w, 0.4992f, 1.0f, 1.0f ) ;
	w->scale = dm0f_mode8_scale ;
	w->base_mtx = &mw->obj_mtx ;
	w->master_w = mw ;
}

void dm0f_mode8_score_move( PLAYER_WORK *w )
{
	Uint32 n ;
	int sft ;

	dm0f_mode8_follow( w ) ;
	if( w->sub_no >= 8 ){
		sft = ( w->sub_no - 8 ) * 4 ;
		if( !Sys->routine_1 ) n = rank_sa[Sys->routine_3 + w->timer1].score_hi ;
		else n = rank_score[Sys->routine_3 + w->timer1].score_hi ;
		n >>= ( Uint8 )sft ;
		n &= 15 ;
		w->disp_flag = 1 ;
		w->disp_flag = w->master_w->disp_flag ;
		if( !n && !dm0f_mode8_score_ck( w, w->sub_no ) ) w->disp_flag = 0 ;
		w->obj = obj_dm0f->obj[n + 37] ;
	} else {
		sft = w->sub_no * 4 ;
		if( 0 ) n = rank_sa[0].score ;	/* left out of the build */
		if( !Sys->routine_1 ) n = rank_sa[Sys->routine_3 + w->timer1].score ;
		else n = rank_score[Sys->routine_3 + w->timer1].score ;
		n >>= ( Uint8 )sft ;
		n &= 15 ;
		w->disp_flag = 1 ;
		w->disp_flag = w->master_w->disp_flag ;
		if( !n && !dm0f_mode8_score_ck( w, w->sub_no ) ) w->disp_flag = 0 ;
		w->obj = obj_dm0f->obj[n + 37] ;
	}
	w->fade.trnsl = w->master_w->fade.trnsl ;
}

/* whether digit n or one above it is not 0 */
Uint8 dm0f_mode8_score_ck( PLAYER_WORK *w, int n )
{
	Uint32 score ;
	Uint32 mask ;

	if( n >= 8 ){
		mask = -1 ;
		if( !Sys->routine_1 ) score = rank_sa[Sys->routine_3 + w->timer1].score_hi ;
		else score = rank_score[Sys->routine_3 + w->timer1].score_hi ;
		n -= 8 ;
		n *= 4 ;
		mask <<= n ;
		if( score & mask ) return 1 ;
	} else {
		mask = -1 ;
		if( !Sys->routine_1 ) score = rank_sa[Sys->routine_3 + w->timer1].score ;
		else score = rank_score[Sys->routine_3 + w->timer1].score ;
		n *= 4 ;
		mask <<= n ;
		if( score & mask ) return 1 ;
		if( dm0f_mode8_score_ck( w, 8 ) ) return 1 ;
	}
	return 0 ;
}

void dm0f_mode8_col_set( PLAYER_WORK *w, Float r, Float g, Float b )
{
	w->fade.r = r ;
	w->fade.g = g ;
	w->fade.b = b ;
}

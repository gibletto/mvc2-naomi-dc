/* DM0C, OPTION's button page: each side's pad with its buttons, the line to the button chosen,
   and the marks for what each button does, the vibration and the side's setting. */
#include "player.h"
#include "shinobi.h"
#include "dm.h"

/* a piece of a page: where it is, its model, and whether more pieces follow it */

extern void dm0c_work_free( PLAYER_WORK *w ) ;
extern void dm0c_work_free2( PLAYER_WORK *w ) ;

void key_move( PLAYER_WORK *w ) ;
void key_nop( PLAYER_WORK *w ) ;
void key_head_init( PLAYER_WORK *w ) ;
void key_line_init( PLAYER_WORK *w ) ;
void key_mark_col( PLAYER_WORK *w ) ;
void key_mark_init( PLAYER_WORK *w ) ;
void key_init( PLAYER_WORK *w ) ;
void key_nop2( PLAYER_WORK *w ) ;
void key_line_main( PLAYER_WORK *w ) ;
void key_mark_main( PLAYER_WORK *w ) ;
void key_main( PLAYER_WORK *w ) ;

static VEC3 key_one = { 1.0f, 1.0f, 1.0f } ;
static VEC3 key_line_pos[8] = {	/* where each side's line starts: a button of the pad, or the start button */
	{ 0.0f, 0.0f, 0.0f }, { -6.57f, -5.2f, -12.39f }, { -6.0f, -6.1f, -12.4f }, { 0.0f, 0.0f, 0.0f },
	{ 12.0f, 2.0f, 0.0f }, { 5.43f, -3.2f, -12.39f }, { 6.0f, -4.1f, -12.4f }, { 12.0f, 2.0f, 0.0f }
} ;
static Uint16 key_button_obj[20] = {	/* a line's button: on a pad, then on a stick with six buttons */
	171, 172, 173, 174, 175, 176, 177, 178, 94, 205,
	171, 172, 173, 174, 206, 207, 177, 178, 94, 205
} ;
static VEC3 key_scale_tbl[6] = {
	{ 1.0f, 1.0f, 1.0f }, { 2.4f, 1.0f, 1.0f }, { 0.72f, 1.0f, 1.0f },
	{ 1.41f, 1.0f, 1.0f }, { 1.0f, 0.75f, 1.0f }, { 0.45f, 0.2f, 1.0f }
} ;
static Float key_y_tbl[10] = { 2.0f, 1.1f, 0.2f, -0.7f, -1.6f, -2.5f, -3.4f, -4.3f, 0.0f, 0.0f } ;
static DM0C_PARTS key_parts0[6] = {
	{ { -8.14f, 2.0f, -12.4f }, 163, 0, 0 }, { { -5.5f, 2.0f, -12.4f }, 164, 1, 0 },
	{ { -2.86f, 2.0f, -12.4f }, 165, 0, 0 }, { { -8.14f, 2.0f, -12.39f }, 166, 0, 0 },
	{ { -5.5f, 2.0f, -12.39f }, 167, 1, 0 }, { { -2.86f, 2.0f, -12.39f }, 168, 0, -1 }
} ;
static DM0C_PARTS key_parts1[6] = {
	{ { -4.73f, -3.4f, -12.4f }, 163, 0, 0 }, { { -3.94f, -3.4f, -12.4f }, 164, 2, 0 },
	{ { -3.15f, -3.4f, -12.4f }, 165, 0, 0 }, { { -4.73f, -3.4f, -12.39f }, 166, 0, 0 },
	{ { -3.94f, -3.4f, -12.39f }, 167, 2, 0 }, { { -3.15f, -3.4f, -12.39f }, 168, 0, -1 }
} ;
static DM0C_PARTS key_parts2[6] = {
	{ { -5.33f, -4.3f, -12.4f }, 163, 0, 0 }, { { -3.94f, -4.3f, -12.4f }, 164, 3, 0 },
	{ { -2.55f, -4.3f, -12.4f }, 165, 0, 0 }, { { -5.33f, -4.3f, -12.39f }, 166, 0, 0 },
	{ { -3.94f, -4.3f, -12.39f }, 167, 3, 0 }, { { -2.55f, -4.3f, -12.39f }, 168, 0, -1 }
} ;
static DM0C_PARTS key_parts3[3] = {
	{ { 0.0f, 0.0f, 0.03f }, 124, 4, 0 }, { { 0.0f, 0.0f, 0.02f }, 127, 5, 0 },
	{ { 0.0f, 0.0f, 0.01f }, 128, 5, -1 }
} ;
static DM0C_PARTS *key_parts_tbl[10] = {	/* a line's box, by the line */
	key_parts0, key_parts0, key_parts0, key_parts0, key_parts0, key_parts0,
	key_parts1, key_parts2, key_parts3, key_parts3
} ;
static Uint8 key_mark_idx[6] = { 0, 1, 2, 3, 4, 5 } ;	/* the button a mark shows */
static FADE_COL key_fade_tbl[6] = {
	{ 1.0f, 1.0f, 0.75f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f },
	{ 1.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }
} ;
static VEC3 key_head_pos[6] = {
	{ -6.0f, -2.0f, -12.39f }, { -7.5f, 2.85f, -12.39f }, { 0.0f, 0.0f, 0.0f },
	{ 6.0f, 0.0f, -12.39f }, { 7.5f, -4.85f, -12.39f }, { 12.0f, 2.0f, 0.0f }
} ;
static Uint16 key_head_obj[6] = { 161, 203, 170, 161, 204, 170 } ;	/* the pads and their names */
static Float key_mark_y[2][8] = {	/* a mark's height, on each side */
	{ 2.0f, 1.1f, 0.2f, -0.7f, -1.6f, -2.5f, -3.4f, -4.3f },
	{ 4.0f, 3.1f, 2.2f, 1.3f, 0.4f, -0.5f, -1.4f, -2.3f }
} ;
static Uint16 key_mark_obj[12] = { 196, 198, 197, 199, 193, 194, 200, 201, 191, 192, 195, 202 } ;	/* by a button's job */
static void ( *key_init_jmp[6] )( PLAYER_WORK *w ) = {
	key_head_init, key_line_init, key_line_init, key_mark_init, key_mark_init, key_nop
} ;
static void ( *key_main_jmp[6] )( PLAYER_WORK *w ) = {
	key_nop2, key_line_main, key_line_main, key_mark_main, key_mark_main, key_nop
} ;
static void ( *key_jmp[7] )( PLAYER_WORK *w ) = {
	key_init, key_main, dm0c_work_free, dm0c_work_free2
} ;

/****************************************************************/
/*			A piece of the page, by its kind					*/
/****************************************************************/
PLAYER_WORK *key_obj_set( Uint8 kind, Uint8 no )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_TOP ) ) ){
		w->disp_flag = 0 ;
		w->work_func = key_move ;
		w->sub_no = kind ;
		w->type = no ;
		w->scale = key_one ;
		w->obj_flag = OBJ_POS | OBJ_SCALE ;
	}
	return w ;
}

/****************************************************************/
/*			A line's box, from key_parts_tbl					*/
/****************************************************************/
PLAYER_WORK *key_sub_set( PLAYER_WORK *mw, Uint8 kind, int no )
{
	PLAYER_WORK *w ;
	DM0C_PARTS *p ;
	int i ;

	w = NULL ;
	p = key_parts_tbl[no] ;
	i = 1 ;
	for( ; ; p ++ ){
		if( !( w = work_get( 0, W_SYS, LIST_TOP ) ) ) break ;
		w->disp_flag = 1 ;
		w->work_func = key_move ;
		w->sub_no = kind ;
		w->type = mw->type ;
		w->sub_type = i ++ ;
		w->base_mtx = &mw->obj_mtx ;
		w->obj = obj_dm0c->obj[p->obj] ;
		w->pos = p->pos ;
		w->pos.y = key_y_tbl[w->type] ;
		w->scale = key_scale_tbl[p->scale] ;
		w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
		w->fade = mw->fade ;
		w->routine_0 = 1 ;
		if( mw->type == 9 && w->sub_type ){
			w->fade.r = 0.6f ;
			w->fade.g = 0.6f ;
			w->fade.b = 0.6f ;
		}
		if( p->next < 0 ) break ;
	}
	return w ;
}

/****************************************************************/
/*			Nothing to do										*/
/****************************************************************/
void key_nop( PLAYER_WORK *w )
{
}

/****************************************************************/
/*			A pad, or its name									*/
/****************************************************************/
void key_head_init( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
	w->disp_flag = 1 ;
	w->obj = obj_dm0c->obj[key_head_obj[w->type]] ;
	w->pos = key_head_pos[w->type] ;
	w->scale = key_one ;
	if( w->type == 3 ){
		w->rot[2] = DEG_ANG( 180.0f ) ;
		w->obj_flag = w->obj_flag | ( OBJ_ROT_Z | OBJ_TRNSL | OBJ_FADE ) ;
	} else w->obj_flag = w->obj_flag | ( OBJ_TRNSL | OBJ_FADE ) ;
	w->fade = key_fade_tbl[w->type] ;
}

/****************************************************************/
/*			A side's button: its model by the pad, the line		*/
/*			to it, lit when the side's cursor is on it			*/
/****************************************************************/
void key_line_init( PLAYER_WORK *w )
{
	int ofs ;

	w->routine_0 ++ ;
	w->disp_flag = 1 ;
	ofs = 0 ;
	if( port_peri[w->sub_no - 1]->support & ( PDD_DGT_TC | PDD_DGT_TZ ) ) ofs = 10 ;
	w->obj = obj_dm0c->obj[key_button_obj[w->type + ofs]] ;
	ofs = w->type < 8 ? 0 : w->type - 7 ;
	w->pos = key_line_pos[( w->sub_no - 1 ) * 4 + ofs] ;
	w->scale = key_one ;
	w->obj_flag = w->obj_flag | OBJ_TRNSL | OBJ_FADE ;
	w->fade.trnsl = 1.0f ;
	if( Sys->option_cursor[w->sub_no - 1] != w->type ){
		if( w->type == 9 && !w->sub_type ){
			w->disp_flag = 0 ;
		w->fade.r = 1.0f ;
		w->fade.g = 1.0f ;
		w->fade.b = 1.0f ;
		} else {
		w->fade.r = 0.6f ;
		w->fade.g = 0.6f ;
		w->fade.b = 0.6f ;
		}
	} else {
		w->fade.r = 1.0f ;
		w->fade.g = 1.0f ;
		w->fade.b = 1.0f ;
	}
	if( w->type != 8 ) key_sub_set( w, w->sub_no, w->type ) ;
}

/****************************************************************/
/*			A mark: what the button does, the vibration or		*/
/*			the side's setting, lit under the side's cursor		*/
/****************************************************************/
void key_mark_col( PLAYER_WORK *w )
{
	Uint16 no ;

	if( w->type == 6 ){
		no = ( option_w.vib_bit & 1 << ( w->sub_no - 3 ) ) ? 0 : 1 ;
		w->obj = obj_dm0c->obj[no + 189] ;
	} else if( w->type == 7 ) w->obj = obj_dm0c->obj[option_w.com_time[w->sub_no - 3] + 187] ;
	else {
		no = option_w.key_config[w->sub_no - 3][key_mark_idx[w->type]] ;
		w->obj = obj_dm0c->obj[key_mark_obj[no]] ;
	}
	if( w->sub_no & 1 ){
		if( w->type == 6 || w->type == 7 ) w->pos.x = -3.94f ;
		else w->pos.x = -5.5f ;
		w->pos.y = ( *key_mark_y )[w->type] ;
	} else {
		if( w->type == 6 || w->type == 7 ) w->pos.x = 8.06f ;
		else w->pos.x = 6.5f ;
		w->pos.y = key_mark_y[1][w->type] ;
	}
	if( Sys->option_cursor[w->sub_no - 3] == w->type ){
		w->fade.r = 1.0f ;
		w->fade.g = 1.0f ;
		w->fade.b = 1.0f ;
	} else {
		w->fade.r = 0.7f ;
		w->fade.g = 0.7f ;
		w->fade.b = 0.7f ;
	}
}

/****************************************************************/
/*			A mark's start										*/
/****************************************************************/
void key_mark_init( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
	w->disp_flag = 1 ;
	key_mark_col( w ) ;
	w->pos.z = -12.38f ;
	w->scale = key_one ;
	w->obj_flag = w->obj_flag | OBJ_TRNSL | OBJ_FADE ;
	w->fade.trnsl = 1.0f ;
}

/****************************************************************/
/*			A piece's start, by its kind						*/
/****************************************************************/
void key_init( PLAYER_WORK *w )
{
	key_init_jmp[w->sub_no]( w ) ;
}

/****************************************************************/
/*			Nothing to do										*/
/****************************************************************/
void key_nop2( PLAYER_WORK *w )
{
}

/****************************************************************/
/*			A side's button each frame							*/
/****************************************************************/
void key_line_main( PLAYER_WORK *w )
{
	int ofs ;

	if( Sys->routine_0 == 1 ){
		if( Sys->option_cursor[w->sub_no - 1] == w->type ){
			if( w->type == 9 && !w->sub_type ){
				w->disp_flag = 1 ;
				w->fade.r = w->fade.g = w->fade.b = 1.0f ;
			}
		}
	} else {
		if( !w->sub_type ){
			ofs = 0 ;
			if( port_peri[w->sub_no - 1]->support & ( PDD_DGT_TC | PDD_DGT_TZ ) ) ofs = 10 ;
			w->obj = obj_dm0c->obj[key_button_obj[w->type + ofs]] ;
		}
		if( Sys->option_cursor[w->sub_no - 1] == w->type ){
			if( w->type == 9 && !w->sub_type ){
				w->disp_flag = 1 ;
				w->fade.r = 0.6f ;
				w->fade.g = 0.6f ;
				w->fade.b = 0.6f ;
			} else {
				w->fade.r = 1.0f ;
				w->fade.g = 1.0f ;
				w->fade.b = 1.0f ;
			}
		} else if( w->type == 9 && !w->sub_type ) w->disp_flag = 0 ;
		else {
			w->fade.r = 0.6f ;
			w->fade.g = 0.6f ;
			w->fade.b = 0.6f ;
		}
	}
}

/****************************************************************/
/*			A mark each frame									*/
/****************************************************************/
void key_mark_main( PLAYER_WORK *w )
{
	key_mark_col( w ) ;
}

/****************************************************************/
/*			A piece each frame, by its kind						*/
/****************************************************************/
void key_main( PLAYER_WORK *w )
{
	key_main_jmp[w->sub_no]( w ) ;
}

/****************************************************************/
/*			A piece of the page, each frame						*/
/****************************************************************/
void key_move( PLAYER_WORK *w )
{
	key_jmp[w->routine_0]( w ) ;
}

/****************************************************************/
/*			The button page's pieces							*/
/****************************************************************/
void option_key_set( void )
{
	int i ;

	for( i = 0 ; i < 6 ; i ++ ) key_obj_set( 0, i ) ;
	for( i = 0 ; i < 10 ; i ++ ) key_obj_set( 1, i ) ;
	for( i = 0 ; i < 10 ; i ++ ) key_obj_set( 2, i ) ;
	for( i = 0 ; i < 8 ; i ++ ) key_obj_set( 3, i ) ;
	for( i = 0 ; i < 8 ; i ++ ) key_obj_set( 4, i ) ;
}

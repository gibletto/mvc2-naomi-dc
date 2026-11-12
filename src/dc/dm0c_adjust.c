/* DM0C, OPTION's screen position page: its frame and lines, the corner marks, the numbers of the
   position and size, and the picture that shows where the screen sits. */
#include "player.h"
#include "nl.h"
#include "dm.h"

/* a piece of a page: where it is, its model, and whether more pieces follow it */

/* a character of a number drawn by the menu */

extern Float adjust_size_tbl[6] ;	/* system.c: the screen's size, 100% to 95%, by option_w.unk_18 */
extern void dm0c_work_free( PLAYER_WORK *w ) ;
extern void dm0c_work_free2( PLAYER_WORK *w ) ;

void adjust_move( PLAYER_WORK *w ) ;
void adjust_line_init( PLAYER_WORK *w ) ;
void adjust_corner_init( PLAYER_WORK *w ) ;
void adjust_num_init( PLAYER_WORK *w ) ;
void adjust_nop( PLAYER_WORK *w ) ;
void adjust_screen_init( PLAYER_WORK *w ) ;
void adjust_init( PLAYER_WORK *w ) ;
void adjust_line_main( PLAYER_WORK *w ) ;
void adjust_corner_main( PLAYER_WORK *w ) ;
void adjust_num_main( PLAYER_WORK *w ) ;
void adjust_nop2( PLAYER_WORK *w ) ;
void adjust_screen_main( PLAYER_WORK *w ) ;
void adjust_main( PLAYER_WORK *w ) ;

static VEC3 adjust_one = { 1.0f, 1.0f, 1.0f } ;
static FADE_COL adjust_fade = { 1.0f, 1.0f, 1.0f, 1.0f } ;
static FADE_COL adjust_fade_off = { 1.0f, 1.0f, 1.0f, 1.0f } ;
static DM0C_PARTS adjust_parts_tbl[4] = {	/* the frame and the three lines */
	{ { 0.0f, 0.0f, -13.0f }, 32, 0, 0 }, { { 0.0f, 2.2f, -13.0f }, 35, 0, 3 },
	{ { 0.0f, 0.0f, -13.0f }, 37, 0, 5 }, { { 0.0f, -1.8f, -13.0f }, 39, 0, 6 }
} ;
static DM0C_PARTS adjust_sub_tbl[7] = {
	{ { 0.0f, 0.0f, -0.15f }, 34, 0, 0 }, { { 0.0f, 0.0f, 0.0f }, 33, 0, 0 },
	{ { 0.0f, 0.0f, 13.0f }, 41, 0, -1 }, { { 0.0f, 0.0f, -0.1f }, 36, 0, -1 },
	{ { 0.0f, 0.0f, -0.1f }, 36, 0, -1 }, { { 0.0f, 0.0f, -0.1f }, 38, 0, -1 },
	{ { 0.0f, 0.0f, -0.1f }, 40, 0, -1 }
} ;
static VEC3 adjust_corner_pos[4] = {
	{ -11.5f, 8.3f, -13.0f }, { -11.5f, -8.3f, -13.0f }, { 11.5f, 8.3f, -13.0f }, { 11.5f, -8.3f, -13.0f }
} ;
static DM0C_CHAR adjust_num_tbl[3][4] = {	/* across, down and the size: a sign or a hundred, two digits */
	{ { 1, 0, 0x41, 0.0f }, { 1, 0, 0x38, 0.5f }, { 1, 0, 0x37, 0.5f }, { -1, 0, 0, 0.0f } },
	{ { 1, 0, 0x42, 0.0f }, { 1, 0, 0x38, 0.5f }, { 1, 0, 0x37, 0.5f }, { -1, 0, 0, 0.0f } },
	{ { 1, 0, 0x38, 0.0f }, { 1, 0, 0x37, 0.5f }, { 1, 0, 0x37, 0.5f }, { -1, 0, 0, 0.0f } }
} ;
static VEC3 adjust_num_pos[3] = { { -1.9f, 1.85f, -13.0f }, { 1.9f, 1.85f, -13.0f }, { -0.75f, 0.15f, -13.0f } } ;
static void ( *adjust_init_jmp[5] )( PLAYER_WORK *w ) = {
	adjust_line_init, adjust_corner_init, adjust_num_init, adjust_nop, adjust_screen_init
} ;
static void ( *adjust_main_jmp[5] )( PLAYER_WORK *w ) = {
	adjust_line_main, adjust_corner_main, adjust_num_main, adjust_nop2, adjust_screen_main
} ;
static void ( *adjust_jmp[7] )( PLAYER_WORK *w ) = {
	adjust_init, adjust_main, dm0c_work_free, dm0c_work_free2
} ;

/****************************************************************/
/*			A piece of the page: its kind and number			*/
/****************************************************************/
PLAYER_WORK *dm0c_adjust_set( Uint8 kind, Uint8 no )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_TOP ) ) ){
		w->disp_flag = 0 ;
		w->work_func = adjust_move ;
		w->sub_no = kind ;
		w->type = no ;
		w->scale = adjust_one ;
		w->fade = adjust_fade ;
		w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	}
	return w ;
}

/****************************************************************/
/*			The pieces that ride on a line, from adjust_sub_tbl	*/
/****************************************************************/
PLAYER_WORK *dm0c_adjust_sub_set( PLAYER_WORK *mw, Uint8 kind, int no )
{
	PLAYER_WORK *w ;
	DM0C_PARTS *p ;
	int i ;

	w = NULL ;
	p = &adjust_sub_tbl[no] ;
	i = 1 ;
	for( ; ; p ++ ){
		if( !( w = work_get( 0, W_SYS, LIST_TOP ) ) ) break ;
		w->routine_0 = w->disp_flag = 1 ;
		w->work_func = adjust_move ;
		w->sub_no = kind ;
		w->type = mw->type ;
		w->sub_type = i ++ ;
		w->base_mtx = &mw->obj_mtx ;
		w->obj = obj_dm0c->obj[p->obj] ;
		w->pos = p->pos ;
		w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
		w->scale = adjust_one ;
		if( mw->type && w->sub_type ){
			w->disp_flag = 0 ;
			w->fade = adjust_fade_off ;
		} else w->fade = adjust_fade ;
		if( p->next < 0 ) break ;
	}
	return w ;
}

/****************************************************************/
/*			Nothing to do										*/
/****************************************************************/
void adjust_nop( PLAYER_WORK *w )
{
}

/****************************************************************/
/*			A number's characters: the position across or		*/
/*			down with its sign, or the size						*/
/****************************************************************/
void adjust_num_set( PLAYER_WORK *w )
{
	DM0C_CHAR *c ;
	int type ;
	int n ;

	type = w->type ;
	c = adjust_num_tbl[type] ;
	if( type == 2 ){
		n = 100 - option_w.unk_18 ;
		if( n / 100 ) c->disp = 1 ;
		else c->disp = 0 ;
		n %= 100 ;
	} else {
		if( !w->type ) n = option_w.adjust_x ;
		else n = option_w.adjust_y ;
		if( n < 0 ){
			c->code = 0x42 ;
			n = -n ;
		} else c->code = 0x41 ;
	}
	c ++ ;
	c->code = n / 10 + 0x37 ;
	c ++ ;
	c->code = n % 10 + 0x37 ;
}

/****************************************************************/
/*			The frame and the lines: the chosen line lit		*/
/****************************************************************/
void adjust_line_init( PLAYER_WORK *w )
{
	DM0C_PARTS *p ;

	w->routine_0 ++ ;
	w->disp_flag = 1 ;
	p = &adjust_parts_tbl[w->type] ;
	w->obj = obj_dm0c->obj[p->obj] ;
	w->pos = p->pos ;
	w->scale = adjust_one ;
	if( !w->type ){
		w->fade.r = 0.96f ;
		w->fade.g = 0.9f ;
		w->fade.b = 0.98f ;
	} else if( Sys->timer0 == w->type - 1 ){
		w->fade.r = 0.98f ;
		w->fade.g = 1.0f ;
		w->fade.b = 0.33f ;
	} else {
		w->fade.r = 0.29f ;
		w->fade.g = 0.48f ;
		w->fade.b = 0.72f ;
	}
	dm0c_adjust_sub_set( w, 0, p->next ) ;
}

/****************************************************************/
/*			The four corner marks, turned to their corners		*/
/****************************************************************/
void adjust_corner_init( PLAYER_WORK *w )
{
	Float rot_tbl[4] = { -90.0f, 0.0f, 180.0f, 90.0f } ;

	w->routine_0 ++ ;
	w->disp_flag = 1 ;
	w->obj = obj_dm0c->obj[67] ;
	w->pos = adjust_corner_pos[w->type] ;
	w->scale = adjust_one ;
	w->rot[2] = DEG_ANG( rot_tbl[w->type] ) ;
}

/****************************************************************/
/*			A number										*/
/****************************************************************/
void adjust_num_init( PLAYER_WORK *w )
{
	DM0C_CHAR *c ;

	w->routine_0 ++ ;
	w->disp_flag = 1 ;
	w->pos = adjust_num_pos[w->type] ;
	c = adjust_num_tbl[w->type] ;
	w->work2 = ( Sint32 )c ;
	adjust_num_set( w ) ;
}

/****************************************************************/
/*			The picture of the screen, where it sits and how	*/
/*			big													*/
/****************************************************************/
void adjust_screen_init( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
	w->disp_flag = 1 ;
	w->scale = adjust_one ;
	w->obj = obj_dm0c->obj[67] ;
	w->obj_flag = OBJ_POS | OBJ_ROT_Z | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->pos.x = option_w.adjust_x / 10.0f ;
	w->pos.y = -( option_w.adjust_y / 10.0f ) ;
	w->scale.x = w->scale.y = adjust_size_tbl[option_w.unk_18] ;
}

/****************************************************************/
/*			A piece's start, by its kind						*/
/****************************************************************/
void adjust_init( PLAYER_WORK *w )
{
	adjust_init_jmp[w->sub_no]( w ) ;
}

/****************************************************************/
/*			The lines each frame: the chosen one blinks			*/
/****************************************************************/
void adjust_line_main( PLAYER_WORK *w )
{
	if( w->type ){
		if( Sys->timer0 == w->type - 1 ){
			if( w->sub_type ){
				if( Sys->routine_2 == 3 ) w->disp_flag = 1 ;
				else w->disp_flag = 0 ;
			} else {
				w->fade.r = 0.98f ;
				w->fade.g = 1.0f ;
				w->fade.b = 0.33f ;
			}
			w->timer1 += 9 ;
			if( w->timer1 >= 360 ) w->timer1 = 0 ;
			w->fade.trnsl = nlCos( DEG_ANG( w->timer1 ) ) * 0.5f + 0.5f ;
		} else if( w->sub_type ) w->disp_flag = 0 ;
		else {
			w->fade.r = 0.29f ;
			w->fade.g = 0.48f ;
			w->fade.b = 0.72f ;
			w->timer1 = 0 ;
			w->fade.trnsl = 1.0f ;
		}
	}
}

/****************************************************************/
/*			The corner marks stay								*/
/****************************************************************/
void adjust_corner_main( PLAYER_WORK *w )
{
}

/****************************************************************/
/*			A number, each frame								*/
/****************************************************************/
void adjust_num_main( PLAYER_WORK *w )
{
	adjust_num_set( w ) ;
}

/****************************************************************/
/*			Nothing to do										*/
/****************************************************************/
void adjust_nop2( PLAYER_WORK *w )
{
}

/****************************************************************/
/*			The picture of the screen, each frame				*/
/****************************************************************/
void adjust_screen_main( PLAYER_WORK *w )
{
	w->pos.x = option_w.adjust_x / 10.0f ;
	w->pos.y = -( option_w.adjust_y / 10.0f ) ;
	w->scale.x = w->scale.y = adjust_size_tbl[option_w.unk_18] ;
}

/****************************************************************/
/*			A piece each frame, by its kind						*/
/****************************************************************/
void adjust_main( PLAYER_WORK *w )
{
	adjust_main_jmp[w->sub_no]( w ) ;
}

/****************************************************************/
/*			A piece of the page, each frame						*/
/****************************************************************/
void adjust_move( PLAYER_WORK *w )
{
	adjust_jmp[w->routine_0]( w ) ;
}

/****************************************************************/
/*			The screen position page's pieces					*/
/****************************************************************/
void option_adjust_set( void )
{
	PLAYER_WORK *w ;
	int i ;

	for( i = 0 ; i < 4 ; i ++ ) dm0c_adjust_set( 0, i ) ;
	dm0c_adjust_set( 4, 0 ) ;
	for( i = 0 ; i < 2 ; i ++ ){
		if( ( w = work_get( 0, W_FRONT, LIST_TOP ) ) ){
			w->disp_flag = 0 ;
			w->work_func = adjust_move ;
			w->sub_no = 2 ;
			w->type = i ;
			w->scale = adjust_one ;
			w->fade = adjust_fade ;
		}
	}
}

/* DM0D, the lobby's frame: its plates, the turning globe, the cursor and the title; then the bar
   that slides in at the top with its arrows and marks. The USA build makes it but never shows it. */
#include "player.h"
#include "nl.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

extern void obj_uv_clear( void *obj ) ;
extern void obj_uv_begin( int *src, void *obj ) ;
extern int obj_uv_next( void ) ;
extern void obj_uv_get( Float *x, Float *y ) ;
extern void obj_uv_put( Float *x, Float *y ) ;

void frame_move( PLAYER_WORK *w ) ;
void frame_cursor_main( PLAYER_WORK *w ) ;
void frame_title_main( PLAYER_WORK *w ) ;
void bar_move( PLAYER_WORK *w ) ;
void bar_arrow_main( PLAYER_WORK *w ) ;

static VEC3 frame_zero = { 0.0f, 0.0f, 0.0f } ;	/* unused */
static VEC3 frame_one = { 1.0f, 1.0f, 1.0f } ;
static FADE_COL frame_fade = { 1.0f, 1.0f, 1.0f, 1.0f } ;
static VEC3 frame_pos[11] = {
	{ 0.0f, 0.0f, -185.0f }, { 0.0f, 0.29f, 0.76f }, { 0.0f, 5.68f, -14.0f }, { 0.0f, 2.98f, -14.0f },
	{ 0.0f, 0.37f, -14.0f }, { 0.0f, -2.35f, -14.0f }, { 0.0f, -6.73f, -14.2f }, { 0.0f, -6.73f, -14.0f },
	{ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.21f, -15.0f }
} ;
static Uint16 frame_obj[11] = { 0x81, 0x82, 0x10, 0x10, 0x10, 0x10, 0x0d, 0x0c, 0x88, 0x8d, 0x01 } ;
static VEC3 frame_cursor_pos[4] = {
	{ 0.0f, 5.58f, -13.85f }, { 0.0f, 2.93f, -13.85f }, { 0.0f, 0.35f, -13.85f }, { 0.0f, -2.29f, -13.85f }
} ;

/****************************************************************/
/*			A piece of the frame: its kind, number and sort		*/
/****************************************************************/
PLAYER_WORK *dm0d_frame_set( Uint8 kind, Uint8 type, Uint8 sub_type )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_TOP ) ) ){
		w->disp_flag = 0 ;
		w->work_func = frame_move ;
		w->sub_no = kind ;
		w->type = type ;
		w->sub_type = sub_type ;
		w->scale = frame_one ;
		w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
		w->fade = frame_fade ;
	}
	return w ;
}

/****************************************************************/
/*			The backdrop and the plates							*/
/****************************************************************/
void frame_parts_init( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
	w->disp_flag = 1 ;
	w->pos = frame_pos[w->type] ;
	w->obj = obj_dm0d->obj[frame_obj[w->type]] ;
	if( w->type == 6 || w->type == 7 ){
		w->scale.x = 1.12f ;
		w->scale.y = 1.12f ;
		w->scale.z = 1.0f ;
	} else if( frame_obj[w->type] == 0x82 ){
		w->scale.x = 1.52f ;
		w->scale.y = 1.52f ;
		w->scale.z = 1.52f ;
	} else if( frame_obj[w->type] == 0x10 ){
		w->scale.x = 1.6f ;
		w->scale.y = 1.6f ;
		w->scale.z = 1.6f ;
	}
}

/****************************************************************/
/*			The globe, turned on all three axes					*/
/****************************************************************/
void frame_globe_init( PLAYER_WORK *w )
{
	w->obj_flag = w->obj_flag | OBJ_ROT_X | OBJ_ROT_Y | OBJ_ROT_Z ;
	w->routine_0 ++ ;
	w->disp_flag = 1 ;
	w->pos.x = 0.07f ;
	w->pos.y = 2.54f ;
	w->pos.z = -29.94f ;
	w->obj = obj_dm0d->obj[131] ;
	w->rot[1] = 0x1d5d ;
	w->rot[2] = 0xc001 ;
	w->timer0 = 0 ;
}

/****************************************************************/
/*			The cursor											*/
/****************************************************************/
void frame_cursor_init( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
	w->disp_flag = 1 ;
	w->obj = obj_dm0d->obj[17] ;
	w->work2 = w->type ;
	w->scale.x = 1.6f ;
	w->scale.y = 1.65f ;
	w->scale.y = 1.6f ;
	w->timer1 = 0 ;
	frame_cursor_main( w ) ;
}

/****************************************************************/
/*			The title of the chosen item						*/
/****************************************************************/
void frame_title_init( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
	w->disp_flag = 1 ;
	frame_title_main( w ) ;
}

/****************************************************************/
/*			A piece's start, by its kind						*/
/****************************************************************/
void frame_init( PLAYER_WORK *w )
{
	static void ( *frame_init_jmp[4] )( PLAYER_WORK *w ) = {
		frame_parts_init, frame_globe_init, frame_cursor_init, frame_title_init
	} ;

	frame_init_jmp[w->sub_no]( w ) ;
}

/****************************************************************/
/*			The plates stay										*/
/****************************************************************/
void frame_parts_main( PLAYER_WORK *w )
{
}

/****************************************************************/
/*			The globe turns										*/
/****************************************************************/
void frame_globe_main( PLAYER_WORK *w )
{
	w->timer0 += 32 ;
	w->rot[0] = w->timer0 ;
}

/****************************************************************/
/*			The cursor goes to its item and blinks				*/
/****************************************************************/
void frame_cursor_main( PLAYER_WORK *w )
{
	w->pos = frame_cursor_pos[w->type] ;
	if( w->work2 != w->type ){
		w->work2 = w->type ;
		w->timer1 = 0 ;
	}
	w->timer1 += 9 ;
	if( w->timer1 >= 360 ) w->timer1 = 0 ;
	w->fade.trnsl = nlCos( DEG_ANG( w->timer1 ) ) * 0.5f + 0.5f ;
}

/****************************************************************/
/*			The title follows the menu's cursor					*/
/****************************************************************/
void frame_title_main( PLAYER_WORK *w )
{
	w->obj = obj_dm0d->obj[Sys->timer0 + 132] ;
}

/****************************************************************/
/*			A piece each frame, by its kind						*/
/****************************************************************/
void frame_main( PLAYER_WORK *w )
{
	static void ( *frame_main_jmp[4] )( PLAYER_WORK *w ) = {
		frame_parts_main, frame_globe_main, frame_cursor_main, frame_title_main
	} ;

	frame_main_jmp[w->sub_no]( w ) ;
}

/****************************************************************/
/*			Hidden, on to the next step							*/
/****************************************************************/
void frame_end( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
	w->disp_flag = 0 ;
}

/****************************************************************/
/*			The piece is done									*/
/****************************************************************/
void frame_free( PLAYER_WORK *w )
{
	work_free( w ) ;
}

/****************************************************************/
/*			A piece of the frame, each frame					*/
/****************************************************************/
void frame_move( PLAYER_WORK *w )
{
	static void ( *frame_jmp[4] )( PLAYER_WORK *w ) = { frame_init, frame_main, frame_end, frame_free } ;

	frame_jmp[w->routine_0]( w ) ;
}

/****************************************************************/
/*			The lobby's frame									*/
/****************************************************************/
void dm0d_frame_all_set( void )
{
	int i ;

	for( i = 0 ; i < 11 ; i ++ ) dm0d_frame_set( 0, i, 0 ) ;
	dm0d_frame_set( 1, 0, 0 ) ;
	dm0d_frame_set( 3, 0, 0 ) ;
	dm0d_frame_set( 2, 0, 0 ) ;
	if( 0 ) dm0d_frame_set( 2, 0, 0 ) ;
}

/****************************************************************/
/*			The bar and its pieces								*/
/****************************************************************/
void bar_base_init( PLAYER_WORK *w )
{
	static Uint16 obj_tbl0[2] = { 0xb9, 0xb6 } ;
	static VEC3 pos_tbl0[2] = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 6.01f, -13.0f } } ;
	static VEC3 pos_tbl1[5] = {
		{ 0.0f, 0.0f, 0.0f }, { 0.0f, 2.95f, -12.99f }, { 0.0f, -1.26f, -12.99f }, { 0.32f, -0.5f, -12.99f },
		{ 0.0f, 6.01f, -13.0f }
	} ;
	static Uint16 obj_tbl1[5] = { 0xbc, 0xbd, 0xbf, 0xb5, 0xb6 } ;

	w->routine_0 ++ ;
	w->disp_flag = 1 ;
	w->pos.x = w->pos.y = w->pos.z = 0.0f ;
	if( !w->sub_no ){
		w->obj = obj_dm0d->obj[180] ;
		w->pos.y = -20.0f ;
	} else {
		if( !w->type ){
			w->obj = obj_dm0d->obj[obj_tbl0[w->sub_type]] ;
			w->pos = pos_tbl0[w->sub_type] ;
		} else {
			w->obj = obj_dm0d->obj[obj_tbl1[w->sub_type]] ;
			w->pos = pos_tbl1[w->sub_type] ;
		}
		w->base_mtx = &w->master_w->obj_mtx ;
		if( w->type ) w->fade.trnsl = 0.0f ;
	}
	w->timer0 = 20 ;
}

/****************************************************************/
/*			The arrows at each side								*/
/****************************************************************/
void bar_arrow_init( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
	w->disp_flag = 1 ;
	w->pos.x = !w->sub_type ? -3.89f : 3.89f ;
	w->pos.y = -6.01f ;
	w->pos.z = -13.0f ;
	if( w->type ) w->pos.z = -13.01f ;
	w->scale.x = 0.21f ;
	w->scale.y = 0.14f ;
	w->scale.z = 1.0f ;
	w->obj = obj_dm0d->obj[w->type + 59] ;
	w->base_mtx = &w->master_w->obj_mtx ;
	w->fade.trnsl = 0.0f ;
}

/****************************************************************/
/*			The mark at the side being chosen					*/
/****************************************************************/
void bar_side_init( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
	w->disp_flag = 1 ;
	w->pos.x = !Sys->routine_1 ? -3.89f : 3.89f ;
	w->pos.y = -6.01f ;
	w->pos.z = -13.02f ;
	w->scale.x = 1.0f ;
	w->scale.y = 1.0f ;
	w->scale.z = 1.0f ;
	w->obj = obj_dm0d->obj[183] ;
	w->base_mtx = &w->master_w->obj_mtx ;
	w->fade.trnsl = 0.0f ;
}

/****************************************************************/
/*			The strip whose picture scrolls						*/
/****************************************************************/
void bar_scroll_init( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
	w->disp_flag = 1 ;
	w->obj = obj_dm0d->obj[184] ;
	w->base_mtx = &w->master_w->obj_mtx ;
	w->obj_flag = w->obj_flag | OBJ_ROT_Z ;
	obj_uv_clear( w->obj ) ;
	w->timer0 = 0 ;
}

/****************************************************************/
/*			A tag on the bar									*/
/****************************************************************/
void bar_tag_init( PLAYER_WORK *w )
{
	static VEC3 pos_tbl[2] = { { 10.2f, -8.55f, -13.03f }, { 0.0f, -8.55f, -13.03f } } ;

	w->routine_0 ++ ;
	w->disp_flag = 1 ;
	w->obj = obj_dm0d->obj[187] ;
	w->base_mtx = &w->master_w->obj_mtx ;
	w->pos = pos_tbl[w->type] ;
	w->fade.trnsl = 0.0f ;
}

/****************************************************************/
/*			A piece's start, by its kind						*/
/****************************************************************/
void bar_init( PLAYER_WORK *w )
{
	static void ( *bar_init_jmp[6] )( PLAYER_WORK *w ) = {
		bar_base_init, bar_base_init, bar_arrow_init, bar_side_init, bar_scroll_init, bar_tag_init
	} ;

	bar_init_jmp[w->sub_no]( w ) ;
}

/****************************************************************/
/*			The bar slides down, waits, and goes				*/
/****************************************************************/
void bar_base_main( PLAYER_WORK *w )
{
	switch( w->routine_1 ){
	case 0 :
		if( -- w->timer0 < 0 ) w->routine_1 ++ ;
		break ;
	case 1 :
		if( ( w->pos.y += 0.625f ) >= 0.0f ){
			w->routine_1 ++ ;
			w->pos.y = 0.0f ;
			Sys->routine_0 ++ ;
		}
		break ;
	case 2 :
		if( Sys->routine_0 >= 4 ){
			w->routine_1 ++ ;
			w->timer0 = 20 ;
		}
		break ;
	case 3 :
		if( -- w->timer0 < 0 ){
			w->routine_1 ++ ;
			Sys->routine_0 ++ ;
		}
		break ;
	}
}

/****************************************************************/
/*			The two headings swap as the choice turns over		*/
/****************************************************************/
void bar_head_main( PLAYER_WORK *w )
{
	switch( w->routine_1 ){
	case 0 :
		if( Sys->timer0 ){
			w->timer0 = 16 ;
			w->spd.x = 1.0f / w->timer0 ;
			if( !w->type ) w->spd.x = -w->spd.x ;
			w->routine_1 ++ ;
		}
		break ;
	case 1 :
		w->fade.trnsl += w->spd.x ;
		if( -- w->timer0 < 0 ){
			if( !w->type ) w->fade.trnsl = 0.0f ;
			if( w->type ) w->fade.trnsl = 1.0f ;
			w->routine_1 ++ ;
			Sys->timer1 = 0 ;
		}
		break ;
	case 2 :
		if( !Sys->timer0 ){
			w->timer0 = 16 ;
			w->spd.x = 1.0f / w->timer0 ;
			if( w->type ) w->spd.x = -w->spd.x ;
			w->routine_1 ++ ;
		}
		break ;
	case 3 :
		w->fade.trnsl += w->spd.x ;
		if( -- w->timer0 < 0 ){
			if( w->type ) w->fade.trnsl = 0.0f ;
			if( !w->type ) w->fade.trnsl = 1.0f ;
			w->routine_1 = 0 ;
			Sys->timer1 = 0 ;
		}
		break ;
	}
}

/****************************************************************/
/*			The arrows show while the choice is turned over		*/
/****************************************************************/
void bar_arrow_main( PLAYER_WORK *w )
{
	switch( w->routine_1 ){
	case 0 :
		if( Sys->timer0 ){
			w->timer0 = 16 ;
			w->spd.x = 1.0f / w->timer0 ;
			w->routine_1 ++ ;
		}
		break ;
	case 1 :
		w->fade.trnsl += w->spd.x ;
		if( -- w->timer0 < 0 ){
			w->fade.trnsl = 1.0f ;
			w->routine_1 ++ ;
		}
		break ;
	case 2 :
		if( !Sys->timer0 ){
			w->timer0 = 16 ;
			w->spd.x = -( 1.0f / w->timer0 ) ;
			w->routine_1 ++ ;
		}
		break ;
	case 3 :
		w->fade.trnsl += w->spd.x ;
		if( -- w->timer0 < 0 ){
			w->fade.trnsl = 0.0f ;
			w->routine_1 = 0 ;
		}
		break ;
	}
}

/****************************************************************/
/*			The mark moves to the side being chosen				*/
/****************************************************************/
void bar_side_main( PLAYER_WORK *w )
{
	if( Sys->timer0 ) w->pos.x = !Sys->routine_1 ? -3.89f : 3.89f ;
	bar_arrow_main( w ) ;
}

/****************************************************************/
/*			The strip scrolls, turned over with the choice		*/
/****************************************************************/
void bar_scroll_main( PLAYER_WORK *w )
{
	Float u, v ;

	if( Sys->timer0 ) w->rot[2] = DEG_ANG( 180 ) ;
	else w->rot[2] = 0 ;
	w->timer0 ++ ;
	if( w->timer0 >= 100 ) w->timer0 = 0 ;
	obj_uv_begin( obj_dm0d->obj[190], w->obj ) ;
	while( !obj_uv_next() ){
		obj_uv_get( &u, &v ) ;
		v -= 1.0f - w->timer0 * 0.01f ;
		obj_uv_put( &u, &v ) ;
	}
}

/****************************************************************/
/*			A piece each frame, by its kind						*/
/****************************************************************/
void bar_main( PLAYER_WORK *w )
{
	static void ( *bar_main_jmp[6] )( PLAYER_WORK *w ) = {
		bar_base_main, bar_head_main, bar_arrow_main, bar_side_main, bar_scroll_main, bar_arrow_main
	} ;

	bar_main_jmp[w->sub_no]( w ) ;
}

/****************************************************************/
/*			A piece of the bar, each frame						*/
/****************************************************************/
void bar_move( PLAYER_WORK *w )
{
	static void ( *bar_jmp[4] )( PLAYER_WORK *w ) = { bar_init, bar_main, frame_end, frame_free } ;

	bar_jmp[w->routine_0]( w ) ;
}

/****************************************************************/
/*			A work made a piece of the bar						*/
/****************************************************************/
void bar_work_set( PLAYER_WORK *w, Uint8 kind, Uint8 type, Uint8 sub_type )
{
	w->disp_flag = 0 ;
	w->work_func = bar_move ;
	w->sub_no = kind ;
	w->type = type ;
	w->sub_type = sub_type ;
	w->scale = frame_one ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->fade = frame_fade ;
}

/****************************************************************/
/*			The bar, with its pieces behind it					*/
/****************************************************************/
PLAYER_WORK *dm0d_bar_set( void )
{
	PLAYER_WORK *mw, *w ;
	int i ;

	if( ( mw = work_get( 0, W_SYS, LIST_TOP ) ) ){
		bar_work_set( mw, 0, 0, 0 ) ;
		w = work_get( mw, W_SYS, LIST_BEHIND ) ;
		bar_work_set( w, 1, 0, 0 ) ;
		w->master_w = mw ;
		w = work_get( mw, W_SYS, LIST_BEHIND ) ;
		bar_work_set( w, 1, 0, 1 ) ;
		w->master_w = mw ;
		for( i = 0 ; i < 5 ; i ++ ){
			w = work_get( mw, W_SYS, LIST_BEHIND ) ;
			bar_work_set( w, 1, 1, i ) ;
			w->master_w = mw ;
		}
		for( i = 0 ; i < 4 ; i ++ ){
			w = work_get( mw, W_SYS, LIST_BEHIND ) ;
			bar_work_set( w, 2, i / 2, i % 2 ) ;
			w->master_w = mw ;
		}
		w = work_get( mw, W_SYS, LIST_BEHIND ) ;
		bar_work_set( w, 3, 0, 0 ) ;
		w->master_w = mw ;
		w = work_get( mw, W_SYS, LIST_BEHIND ) ;
		bar_work_set( w, 4, 0, 0 ) ;
		w->master_w = mw ;
		w = work_get( mw, W_SYS, LIST_BEHIND ) ;
		bar_work_set( w, 5, 0, 0 ) ;
		w->master_w = mw ;
	}
	return mw ;
}

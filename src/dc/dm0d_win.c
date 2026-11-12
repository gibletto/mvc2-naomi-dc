/* DM0D, the lobby's windows: a window that slides in with its frame pieces, its title and moving
   picture, the cursor on its items, and the page of a player's profile. */
#include "player.h"
#include "nl.h"
#include "dm0d.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

/* a piece of a window: where it is, its model, its width, and whether more pieces follow it */
typedef struct DM0D_PARTS {
	VEC3	pos;
	Uint16	obj;
	Sint8	width;		/* 1 on, the width from win_parts_width; 0 the model's own */
	Sint8	next;		/* -1 for the last */
} DM0D_PARTS;

extern char *strncpy( char *s1, const char *s2, unsigned long n ) ;

void win_move( PLAYER_WORK *w ) ;
void win_pict_main( PLAYER_WORK *w ) ;
void win_cursor_main( PLAYER_WORK *w ) ;
void win_pict_set( int no ) ;

static VEC3 win_zero = { 0.0f, 0.0f, 0.0f } ;
static VEC3 win_one = { 1.0f, 1.0f, 1.0f } ;
static FADE_COL win_fade = { 1.0f, 1.0f, 1.0f, 1.0f } ;
static DM0D_PARTS win_parts0[10] = {
	{ { 0.0f, 0.0f, 0.0f }, 0x19, 0, 0 }, { { 0.0f, 0.0f, 0.0f }, 0x1e, 0, 0 },
	{ { -10.332f, 5.09f, -13.916f }, 0x24, 0, 0 }, { { -11.712f, 1.43f, -13.916f }, 0x25, 0, 0 },
	{ { -10.332f, -2.29f, -13.916f }, 0x26, 0, 0 }, { { 0.0f, -6.66f, -14.088f }, 0x2c, 0, 0 },
	{ { 0.0f, 3.404f, -14.3f }, 0x1b, 1, 0 }, { { 0.0f, -0.376f, -14.3f }, 0x1b, 5, 0 },
	{ { 0.0f, -4.116f, -14.3f }, 0x1b, 2, 0 }, { { 0.0f, -7.8f, -14.3f }, 0x1b, 8, -1 }
} ;
static DM0D_PARTS win_parts1[15] = {
	{ { 0.0f, 0.0f, 0.0f }, 0x1a, 0, 0 }, { { 0.0f, 0.0f, 0.0f }, 0x20, 0, 0 },
	{ { -12.4f, 5.75f, -13.916f }, 0x27, 0, 0 }, { { -12.4f, 3.05f, -13.916f }, 0x28, 0, 0 },
	{ { -12.4f, 0.2f, -13.916f }, 0x29, 0, 0 }, { { -12.4f, -2.83f, -13.916f }, 0x2a, 0, 0 },
	{ { 0.0f, -6.08f, -14.088f }, 0x2f, 0, 0 }, { { -8.356f, -8.1f, -14.088f }, 0x30, 0, 0 },
	{ { 8.228f, -8.1f, -14.088f }, 0x31, 0, 0 }, { { 0.0f, 4.984f, -14.3f }, 0x1b, 4, 0 },
	{ { 0.0f, 2.156f, -14.3f }, 0x1b, 6, 0 }, { { 0.0f, -0.836f, -14.3f }, 0x1b, 4, 0 },
	{ { 4.5f, -4.358f, -14.3f }, 0x1b, 5, 0 }, { { 0.0f, 0.0f, 0.0f }, 0x79, 0, 0 },
	{ { -7.03f, -6.06f, -14.088f }, 0x2e, 0, -1 }
} ;
static Float win_parts_width[8] = { 1.8f, 4.0f, 1.4f, 3.5f, 2.5f, 2.0f, 3.0f, 1.9f } ;
static VEC3 win_cursor_pos0[9] = {	/* the items of the first window, then the second's */
	{ 0.0f, 3.46f, -14.445f }, { 0.0f, -0.32f, -14.445f }, { 0.0f, -4.08f, -14.445f },
	{ 0.0f, -7.793f, -14.445f }, { 0.0f, -10.185f, -14.445f }, { -10.336f, 5.12f, -14.1f },
	{ -11.524f, 1.46f, -14.1f }, { -10.336f, -2.3f, -14.1f }, { 0.0f, 0.0f, 0.0f }
} ;
static Sint16 win_cursor_obj0[10] = { 0x1c, 0x1c, 0x1c, 0x1c, 0x78, 0x21, 0x22, 0x21, 0x16, -1 } ;
static VEC3 win_cursor_pos1[13] = {
	{ 0.0f, 5.12f, -14.445f }, { 0.0f, 2.24f, -14.445f }, { 0.0f, -0.8f, -14.445f }, { 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f }, { 0.0f, -6.6595f, -14.445f }, { -8.1f, -8.8f, -14.445f }, { 8.3f, -8.8f, -14.445f },
	{ -12.328f, 5.82f, -14.099f }, { -12.328f, 3.16f, -14.099f }, { -12.328f, 0.2f, -14.099f },
	{ -11.524f, -2.742f, -14.099f }, { -3.196f, -2.71f, -14.099f }
} ;
static Sint16 win_cursor_obj1[17] = {
	0x1c, 0x1c, 0x1c, 0x136, 0x137, 0x1d, 0x78, 0x78, 0x23, 0x23, 0x23, 0x22, 0x23, -1, -1, -1, -1
} ;
static VEC3 win_title_pos[3] = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 6.86f, -12.78f } } ;

/****************************************************************/
/*			A piece of a window: its kind, number and sort		*/
/****************************************************************/
PLAYER_WORK *dm0d_win_set( Uint8 kind, Uint8 type, Uint8 sub_type )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_TOP ) ) ){
		w->disp_flag = 0 ;
		w->work_func = win_move ;
		w->sub_no = kind ;
		w->type = type ;
		w->sub_type = sub_type ;
		w->scale = win_one ;
		w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
		w->fade = win_fade ;
	}
	return w ;
}

/****************************************************************/
/*			A window's frame pieces, from a table				*/
/****************************************************************/
PLAYER_WORK *win_parts_set( PLAYER_WORK *mw, DM0D_PARTS *p )
{
	PLAYER_WORK *w ;
	int i ;

	w = NULL ;
	i = 0 ;
	for( ; ; p ++ ){
		if( !( w = work_get( mw, W_SYS, LIST_BEHIND ) ) ) break ;
		w->disp_flag = 0 ;
		w->work_func = win_move ;
		w->sub_no = 1 ;
		w->type = mw->type ;
		w->sub_type = i ++ ;
		w->base_mtx = &mw->obj_mtx ;
		w->obj = obj_dm0d->obj[p->obj] ;
		w->pos = p->pos ;
		w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
		w->fade = win_fade ;
		w->scale = win_one ;
		w->master_w = mw ;
		if( p->width ) w->scale.x = win_parts_width[p->width - 1] ;
		if( p->next < 0 ) break ;
	}
	return w ;
}

/****************************************************************/
/*			The cursor and its shadow							*/
/****************************************************************/
PLAYER_WORK *win_cursor_set( void )
{
	PLAYER_WORK *mw, *w ;

	if( ( mw = work_get( 0, W_SYS, LIST_TOP ) ) ){
		mw->disp_flag = 0 ;
		mw->work_func = win_move ;
		mw->sub_no = 4 ;
		mw->type = 0 ;
		mw->sub_type = 0 ;
		mw->scale = win_one ;
		mw->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
		mw->fade = win_fade ;
	}
	if( ( w = work_get( mw, W_SYS, LIST_BEHIND ) ) ){
		w->disp_flag = 0 ;
		w->work_func = win_move ;
		w->sub_no = 4 ;
		w->type = 0 ;
		w->sub_type = 1 ;
		w->scale = win_one ;
		w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
		w->fade = win_fade ;
		w->ef_w = mw ;
	}
	return mw ;
}

/****************************************************************/
/*			The window, with its shadow, and its pieces			*/
/****************************************************************/
void win_frame_init( PLAYER_WORK *w )
{
	PLAYER_WORK *sw ;

	w->routine_0 ++ ;
	w->disp_flag = 0 ;
	if( !w->type ){
		w->pos = win_zero ;
		w->scale = win_one ;
		sw = work_get( w, W_SYS, LIST_BEHIND ) ;
		sw->work_func = win_move ;
		sw->sub_no = w->sub_no ;
		sw->type = 1 ;
		sw->sub_type = w->sub_type ;
		sw->pos = w->pos ;
		sw->scale = w->scale ;
		sw->ef_w = w ;
		sw->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_FADE ;
		win_parts_set( w, win_parts0 ) ;
	} else {
		sw = w->ef_w ;
		w->pos.x = sw->pos.x + 32.0f ;
		win_parts_set( w, win_parts1 ) ;
	}
}

/****************************************************************/
/*			A frame piece										*/
/****************************************************************/
void win_parts_init( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
	w->disp_flag = 1 ;
}

/****************************************************************/
/*			The window's title									*/
/****************************************************************/
void win_title_init( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
	w->disp_flag = 1 ;
	w->pos = win_title_pos[w->type] ;
	w->scale = win_one ;
	w->obj = obj_dm0d->obj[w->type + 22] ;
}

/****************************************************************/
/*			The moving picture and its frame					*/
/****************************************************************/
void win_pict_init( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
	w->disp_flag = 1 ;
	if( !w->sub_type ){
		w->scale.x = w->scale.y = 1.2f ;
		w->scale.z = 1.0f ;
		w->pos.z = -3.0f ;
	} else w->scale = win_one ;
	w->obj = obj_dm0d->obj[w->work1] ;
	if( !w->type ) obj_uv_clear( w->obj ) ;
	w->timer1 = w->timer0 = 0 ;
	win_pict_main( w ) ;
}

/****************************************************************/
/*			The cursor											*/
/****************************************************************/
void win_cursor_init( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
	w->dir = 0xff ;
	win_cursor_main( w ) ;
}

/****************************************************************/
/*			Nothing to do										*/
/****************************************************************/
void win_nop( PLAYER_WORK *w )
{
}

/****************************************************************/
/*			The mark that blinks								*/
/****************************************************************/
void win_mark_init( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
	w->disp_flag = 0 ;
	w->scale = win_one ;
	if( !w->type ) w->obj = obj_dm0d->obj[119] ;
	else w->obj = obj_dm0d->obj[269] ;
}

/****************************************************************/
/*			The profile's page									*/
/****************************************************************/
void win_rec_init( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
	w->disp_flag = 0 ;
}

/****************************************************************/
/*			A piece's start, by its kind						*/
/****************************************************************/
void win_init( PLAYER_WORK *w )
{
	static void ( *win_init_jmp[8] )( PLAYER_WORK *w ) = {
		win_frame_init, win_parts_init, win_title_init, win_pict_init, win_cursor_init, win_nop,
		win_mark_init, win_rec_init
	} ;

	win_init_jmp[w->sub_no]( w ) ;
}

/****************************************************************/
/*			The window slides out and back in as the page		*/
/*			turns; its shadow follows							*/
/****************************************************************/
void win_frame_main( PLAYER_WORK *w )
{
	PLAYER_WORK *mw ;

	if( !w->sub_type ){
		switch( w->routine_1 ){
		case 0 :
			if( Sys->timer0 ){
				w->timer0 = 32 ;
				w->spd.x = 32 / w->timer0 ;
				w->routine_1 ++ ;
			}
			break ;
		case 1 :
			if( -- w->timer0 < 0 ){
				w->routine_1 ++ ;
				Sys->timer0 = 0 ;
			} else w->pos.x -= w->spd.x ;
			break ;
		case 2 :
			if( Sys->timer0 ){
				if( Sys->timer0 < 0 ){
					w->timer0 = 32 ;
					w->routine_1 ++ ;
				}
			}
			break ;
		case 3 :
			if( -- w->timer0 < 0 ){
				w->routine_1 = 0 ;
				Sys->timer0 = 0 ;
			} else w->pos.x += w->spd.x ;
			break ;
		}
	} else {
		mw = w->ef_w ;
		w->pos.x = mw->pos.x + 32.0f ;
	}
}

/****************************************************************/
/*			A frame piece: the mark of whose profile it is		*/
/****************************************************************/
void win_parts_main( PLAYER_WORK *w )
{
	Sint8 side ;

	if( w->sub_type == 14 ){
		if( unk_8C212F1B & 0x80 ) side = 1 ;
		else side = 0 ;
		if( net_rec[side].unk_220 ){
			w->obj = obj_dm0d->obj[45] ;
			w->pos.z = -14.064f ;
		} else {
			w->obj = obj_dm0d->obj[46] ;
			w->pos.z = -14.088f ;
		}
	}
}

/****************************************************************/
/*			The title stays										*/
/****************************************************************/
void win_title_main( PLAYER_WORK *w )
{
}

/****************************************************************/
/*			The picture scrolls									*/
/****************************************************************/
void win_pict_main( PLAYER_WORK *w )
{
	Float v, u ;

	if( !w->type ){
		obj_uv_begin( obj_dm0d->obj[w->work2], w->obj ) ;
		while( !obj_uv_next() ){
			obj_uv_get( &u, &v ) ;
			u -= 1.0f - win_pict_u * 0.002f ;
			v -= 1.0f - win_pict_v * 0.001f ;
			obj_uv_put( &u, &v ) ;
		}
	}
}

/****************************************************************/
/*			The cursor goes to its item and blinks; hidden		*/
/*			while the page turns								*/
/****************************************************************/
void win_cursor_main( PLAYER_WORK *w )
{
	register PLAYER_WORK *mw ;
	register SYSTEM_WORK *sys ;
	int n, obj ;

	if( sys = Sys, !w->routine_1 ){
		if( sys->timer0 ){
			w->disp_flag = 0 ;
			w->routine_1 ++ ;
			w->timer1 = 0 ;
			return ;
		}
		if( w->sub_type ){
			mw = w->ef_w ;
			w->type = mw->type ;
			w->work1 = mw->work1 ;
		}
		if( !w->work1 ){
			n = w->sub_type * 5 + w->type ;
			obj = win_cursor_obj0[n] ;
			if( obj < 0 ) goto hide ;
			w->disp_flag = 1 ;
			w->obj = obj_dm0d->obj[obj] ;
			w->pos = win_cursor_pos0[n] ;
		} else {
			n = w->sub_type * 8 + w->type ;
			obj = win_cursor_obj1[n] ;
			if( obj < 0 ){
hide:
				w->disp_flag = 0 ;
			} else {
				w->disp_flag = 1 ;
				w->obj = obj_dm0d->obj[obj] ;
				w->pos = win_cursor_pos1[n] ;
			}
		}
		if( w->dir != n ){
			w->dir = n ;
			w->timer1 = 0 ;
		}
		w->timer1 += 9 ;
		if( w->timer1 >= 360 ) w->timer1 = 0 ;
		w->fade.trnsl = nlCos( DEG_ANG( w->timer1 ) ) * 0.5f + 0.5f ;
	} else if( !sys->timer0 ) w->routine_1 = 0 ;
}

/****************************************************************/
/*			Nothing to do										*/
/****************************************************************/
void win_nop2( PLAYER_WORK *w )
{
}

/****************************************************************/
/*			The mark blinks										*/
/****************************************************************/
void win_mark_main( PLAYER_WORK *w )
{
	if( unk_8C212F10 & 0x80 ) w->disp_flag = 1 ;
	else w->disp_flag = 0 ;
}

/****************************************************************/
/*			The profile's page: it slides with the window,		*/
/*			and its lines are written each frame				*/
/****************************************************************/
void win_rec_main( PLAYER_WORK *w )
{
	static VEC3 pos_tbl[9] = {
		{ 240.0f, 166.0f, 14.25f }, { 215.0f, 238.0f, 14.25f }, { 110.0f, 310.0f, 14.25f },
		{ 260.0f, 380.0f, 14.25f }, { 780.0f, 134.0f, 14.25f }, { 856.0f, 189.0f, 14.25f },
		{ 760.0f, 247.0f, 14.25f }, { 737.0f, 315.0f, 14.25f }, { 910.0f, 314.0f, 14.25f }
	} ;
	char buf[128] ;
	VEC3 *p ;
	Sint16 side ;
	Sint16 i ;

	switch( w->routine_1 ){
	case 0 :
		if( Sys->timer0 ){
			w->timer0 = 32 ;
			w->spd.x = 640 / w->timer0 ;
			w->routine_1 ++ ;
		}
		break ;
	case 1 :
		if( -- w->timer0 < 0 ) w->routine_1 ++ ;
		else w->pos.x -= w->spd.x ;
		break ;
	case 2 :
		if( Sys->timer0 ){
			if( Sys->timer0 < 0 ){
				w->timer0 = 32 ;
				w->routine_1 ++ ;
			}
		}
		break ;
	case 3 :
		if( -- w->timer0 < 0 ) w->routine_1 = 0 ;
		else w->pos.x += w->spd.x ;
		break ;
	}
	if( unk_8C212F1B & 0x80 ) side = 1 ;
	else side = 0 ;
	p = pos_tbl ;
	if( net_rec[side].handle[0] )
		str_put( w->pos.x + p->x, w->pos.y + p->y, p->z, 0xff000000, net_rec[side].handle ) ;
	p ++ ;
	if( net_rec[side].name[0] ){
		memset( buf, 0, 128 ) ;
		strncpy( buf, net_rec[side].name, 18 ) ;
		str_put( w->pos.x + p->x, w->pos.y + p->y, p->z, 0xff000000, buf ) ;
	}
	p ++ ;
	if( net_rec[side].unk_080[0] ){
		strncpy( buf, net_rec[side].unk_080, 34 ) ;
		str_put( w->pos.x + p->x, w->pos.y + p->y, p->z, 0xff000000, buf ) ;
	}
	p ++ ;
	if( net_rec[side].entry_str[0] ){
		memset( buf, 0, 128 ) ;
		num_comma_set( str_num_get( net_rec[side].entry_str ), buf ) ;
		str_put( w->pos.x + p->x, w->pos.y + p->y, p->z, 0xff000000, buf ) ;
	}
	p ++ ;
	if( net_rec[side].unk_100[0] ){
		memset( buf, 0, 128 ) ;
		strncpy( buf, net_rec[side].unk_100, 36 ) ;
		str_put( w->pos.x + p->x, w->pos.y + p->y, p->z, 0xff000000, buf ) ;
	}
	p ++ ;
	if( net_rec[side].age[0] ){
		memset( buf, 0, 128 ) ;
		num_comma_set( str_num_get( net_rec[side].age ), buf ) ;
		str_put( w->pos.x + p->x, w->pos.y + p->y, p->z, 0xff000000, buf ) ;
	}
	str_put( w->pos.x + p->x + 108.0f, w->pos.y + p->y + -1.0f, p->z, 0xff000000, "\x8d\xcb" ) ;	/* "years old" */
	p ++ ;
	if( net_rec[side].unk_140[0] ){
		memset( buf, 0, 128 ) ;
		strncpy( buf, net_rec[side].unk_140, 40 ) ;
		str_put( w->pos.x + p->x, w->pos.y + p->y, p->z, 0xff000000, buf ) ;
	}
	p ++ ;
	memset( buf, 0, 128 ) ;
	if( net_rec[side].post_code[0] ){
		for( i = 0 ; i < 3 ; i ++ ) buf[i] = net_rec[side].post_code[i] ;
		buf[i] = 0 ;
		str_put( w->pos.x + p->x, w->pos.y + p->y, p->z, 0xff000000, buf ) ;
		for( i = 0 ; i < 4 ; i ++ ) buf[i] = net_rec[side].post_code[i + 3] ;
		if( 0 ) i = 4 ;	/* left out of the build */
		buf[i] = 0 ;
		str_put( w->pos.x + p->x + 50.0f, w->pos.y + p->y, p->z, 0xff000000, buf ) ;
	}
	p ++ ;
	if( net_rec[side].unk_18a[0] ){
		memset( buf, 0, 128 ) ;
		strncpy( buf, net_rec[side].unk_18a, 28 ) ;
		str_put( w->pos.x + p->x, w->pos.y + p->y, p->z, 0xff000000, buf ) ;
	}
}

/****************************************************************/
/*			A piece each frame, by its kind						*/
/****************************************************************/
void win_main( PLAYER_WORK *w )
{
	static void ( *win_main_jmp[8] )( PLAYER_WORK *w ) = {
		win_frame_main, win_parts_main, win_title_main, win_pict_main, win_cursor_main, win_nop2,
		win_mark_main, win_rec_main
	} ;

	win_main_jmp[w->sub_no]( w ) ;
}

/****************************************************************/
/*			On to the next step									*/
/****************************************************************/
void win_next( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
}

/****************************************************************/
/*			The piece is done									*/
/****************************************************************/
void win_free( PLAYER_WORK *w )
{
	work_free( w ) ;
}

/****************************************************************/
/*			A piece of a window, each frame						*/
/****************************************************************/
void win_move( PLAYER_WORK *w )
{
	static void ( *win_jmp[4] )( PLAYER_WORK *w ) = { win_init, win_main, win_next, win_free } ;

	win_jmp[w->routine_0]( w ) ;
}

/****************************************************************/
/*			The lobby's windows									*/
/****************************************************************/
void dm0d_win_all_set( void )
{
	dm0d_win_set( 0, 0, 0 ) ;
	dm0d_win_set( 2, 1, 0 ) ;
	dm0d_win_set( 2, 2, 0 ) ;
	dm0d_win_set( 6, 0, 0 ) ;
	dm0d_win_set( 6, 1, 0 ) ;
	dm0d_win_set( 7, 0, 0 ) ;
	win_pict_set( 0 ) ;
}

/****************************************************************/
/*			A window's moving picture, and its frame			*/
/****************************************************************/
void win_pict_set( int no )
{
	static Sint16 pict_tbl[8][4] = {
		{ 0xd3, 0xd4, 0xd2, -1 }, { 0x8b, 0x8c, -1, -1 }, { 0xd3, 0xd4, 0xd2, -1 }, { 0xa2, 0xa3, -1, -1 },
		{ 0xc3, 0xc4, -1, -1 }, { 0xf0, 0xf1, -1, -1 }, { 0xd3, 0xd4, -1, -1 }, { 0x11e, 0x11f, -1, -1 }
	} ;
	PLAYER_WORK *w ;
	Sint16 *p ;

	p = pict_tbl[no] ;
	w = dm0d_win_set( 3, 0, no ) ;
	w->work1 = *p ++ ;
	w->work2 = *p ++ ;
	if( *p >= 0 ){
		w = dm0d_win_set( 3, 1, no ) ;
		w->work1 = *p ;
	}
}

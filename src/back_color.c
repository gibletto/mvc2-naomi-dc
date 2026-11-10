/* The background colour behind the polygons: one colour or a gradient of two or three, darkened
   with the screen, and the flash's colour while one shows. */
#include "player.h"

static void back_color_bright( Uint32 *col0, Uint32 *col1, Uint32 *col2 ) ;
static void back_color_get( Uint32 *col0, Uint32 *col1, Uint32 *col2 ) ;

/****************************************************************/
/*			One colour											*/
/****************************************************************/
void back_color_set( Uint32 col )
{
	back_work.mode = 0 ;
	back_work.col[0].argb = col ;
}

/****************************************************************/
/*			The first colour at the top and bottom, the			*/
/*			second in the middle								*/
/****************************************************************/
void back_color_set1( Uint32 col0, Uint32 col1 )
{
	back_work.mode = 1 ;
	back_work.col[0].argb = col0 ;
	back_work.col[1].argb = col1 ;
}

/****************************************************************/
/*			The first colour above, the second below			*/
/****************************************************************/
void back_color_set2( Uint32 col0, Uint32 col1 )
{
	back_work.mode = 2 ;
	back_work.col[0].argb = col0 ;
	back_work.col[1].argb = col1 ;
}

/****************************************************************/
/*			Three colours, top to bottom						*/
/****************************************************************/
void back_color_set3( Uint32 col0, Uint32 col1, Uint32 col2 )
{
	back_work.mode = 3 ;
	back_work.col[0].argb = col0 ;
	back_work.col[1].argb = col1 ;
	back_work.col[2].argb = col2 ;
}

/****************************************************************/
/*			Send the frame's background colours					*/
/****************************************************************/
void back_color_disp( void )
{
	Uint32 col0, col1, col2 ;

	if( game_play_ck() || cpu_max_ck() ) back_color_bright( &col0, &col1, &col2 ) ;
	else back_color_get( &col0, &col1, &col2 ) ;
	switch( back_work.mode ){
	case 0:
		nlSetBackColorZ( col0, col0, col0, 10000000.0f ) ;
		break ;
	case 1:
		nlSetBackColorZ( col0, col1, col0, 10000000.0f ) ;
		break ;
	case 2:
		nlSetBackColorZ( col0, col0, col1, 10000000.0f ) ;
		break ;
	case 3:
		nlSetBackColorZ( col0, col1, col2, 10000000.0f ) ;
		break ;
	}
}

/*==============================================================*/
/*			The colours darkened with the screen; the			*/
/*			flash's colour, or black in a hyper combo's stop	*/
/*==============================================================*/
static void back_color_bright( Uint32 *col0, Uint32 *col1, Uint32 *col2 )
{
	Float r_rate, g_rate, b_rate ;
	Uint8 r, g, b ;

	if( flash_flag_get() ){
		back_work.mode = 0 ;
		*col0 = flash_work.col ;
	} else if( vs_sys_w->hyper_bg_req && vs_sys_w->game_stop ){
		back_work.mode = 0 ;
		*col0 = 0 ;
	} else {
		r_rate = bright_work.rate[1] ;
		g_rate = bright_work.rate[2] ;
		b_rate = bright_work.rate[3] ;
		switch( back_work.mode ){
		case 3:
			r = r_rate * back_work.col[2].c.r ;
			g = g_rate * back_work.col[2].c.g ;
			b = b_rate * back_work.col[2].c.b ;
			*col2 = r << 16 | g << 8 | b ;
		case 2:
		case 1:
			r = r_rate * back_work.col[1].c.r ;
			g = g_rate * back_work.col[1].c.g ;
			b = b_rate * back_work.col[1].c.b ;
			*col1 = r << 16 | g << 8 | b ;
		case 0:
			r = r_rate * back_work.col[0].c.r ;
			g = g_rate * back_work.col[0].c.g ;
			b = b_rate * back_work.col[0].c.b ;
			*col0 = r << 16 | g << 8 | b ;
		}
	}
}

/*==============================================================*/
/*			The colours as they are set							*/
/*==============================================================*/
static void back_color_get( Uint32 *col0, Uint32 *col1, Uint32 *col2 )
{
	Uint8 r, g, b ;

	switch( back_work.mode ){
	case 3:
		r = back_work.col[2].c.r ;
		g = back_work.col[2].c.g ;
		b = back_work.col[2].c.b ;
		*col2 = r << 16 | g << 8 | b ;
	case 2:
	case 1:
		r = back_work.col[1].c.r ;
		g = back_work.col[1].c.g ;
		b = back_work.col[1].c.b ;
		*col1 = r << 16 | g << 8 | b ;
	case 0:
		r = back_work.col[0].c.r ;
		g = back_work.col[0].c.g ;
		b = back_work.col[0].c.b ;
		*col0 = r << 16 | g << 8 | b ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "back_color.c" End of File	*/

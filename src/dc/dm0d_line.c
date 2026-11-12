/* DM0D, the lobby's lines: the room's name with its blinking plate, the arrows that show the list
   scrolls, and the panels that slide in while the server is asked. */
#include "player.h"
#include "nl.h"
#include "dm0d.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

void line_set( Uint8 no, Uint16 obj ) ;
void line_move( PLAYER_WORK *w ) ;
void line_init( PLAYER_WORK *w ) ;
void line_main( PLAYER_WORK *w ) ;
void line_blink( PLAYER_WORK *w ) ;
void line_arrow( PLAYER_WORK *w ) ;
void line_slide( PLAYER_WORK *w ) ;
void line_slide_wait( PLAYER_WORK *w ) ;
void line_slide_in( PLAYER_WORK *w ) ;
void line_slide_stay( PLAYER_WORK *w ) ;
void line_slide_out( PLAYER_WORK *w ) ;
void line_room_put( void ) ;
void line_next( PLAYER_WORK *w ) ;
void line_free( PLAYER_WORK *w ) ;

/****************************************************************/
/*			The lines, one work each							*/
/****************************************************************/
void dm0d_line_set( void )
{
	line_set( 0, 0xa4 ) ;
	line_set( 1, 0xa5 ) ;
	line_set( 2, 0xa6 ) ;
	line_set( 3, 0xa8 ) ;
	line_set( 4, 0xb3 ) ;
	line_set( 5, 0xf2 ) ;
	line_set( 6, 0xf3 ) ;
	line_set( 7, 0xf4 ) ;
	line_set( 8, 0xf5 ) ;
	line_set( 9, 0xf5 ) ;
	line_set( 10, 0xf6 ) ;
	line_set( 11, 0xf6 ) ;
	line_set( 12, 0xf7 ) ;
	line_set( 13, 0x10d ) ;
	line_set( 14, 0x10c ) ;
	line_set( 15, 0xb2 ) ;
	line_set( 16, 0xaf ) ;
	line_set( 17, 0x109 ) ;
	win_pict_set( 5 ) ;
}

/****************************************************************/
/*			A line: its number and its model					*/
/****************************************************************/
void line_set( Uint8 no, Uint16 obj )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_TOP ) ) ){
		w->disp_flag = 0 ;
		w->work_func = line_move ;
		w->obj = obj_dm0d->obj[obj] ;
		w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
		w->sub_no = no ;
		w->fade.b = w->fade.g = w->fade.r = w->fade.trnsl = 1.0f ;
		w->scale.z = w->scale.y = w->scale.x = 1.0f ;
	}
}

void line_move( PLAYER_WORK *w )
{
	static void ( *line_jmp[4] )( PLAYER_WORK *w ) = { line_init, line_main, line_next, line_free } ;

	line_jmp[w->routine_0]( w ) ;
}

/****************************************************************/
/*			A line's place and size								*/
/****************************************************************/
void line_init( PLAYER_WORK *w )
{
	w->disp_flag = 1 ;
	w->routine_0 ++ ;
	switch( w->sub_no ){
	case 8 :
		w->pos.x = 10.653f ;
		w->pos.y = 8.0f ;
		w->pos.z = -18.1789f ;
		break ;
	case 9 :
		w->pos.x = 10.653f ;
		w->pos.y = -4.1f ;
		w->pos.z = -18.1789f ;
		break ;
	case 10 :
		w->pos.x = 10.653f ;
		w->pos.y = 8.0f ;
		w->pos.z = -18.1789f ;
		break ;
	case 11 :
		w->pos.x = 10.653f ;
		w->pos.y = -4.1f ;
		w->pos.z = -18.1789f ;
		break ;
	case 13 :
		w->pos.x = 38.2f ;
		w->pos.y = 1.6f ;
		w->pos.z = 0.0f ;
		w->scale.x = 0.8f ;
		w->scale.y = 0.6f ;
		w->scale.z = 1.0f ;
		break ;
	case 14 :
		w->pos.x = 32.75f ;
		w->pos.y = 3.15f ;
		w->pos.z = 0.0f ;
		break ;
	case 15 :
		w->pos.x = 34.0f ;
		w->pos.y = 3.0f ;
		w->pos.z = -7.0f ;
		w->scale.x = 0.31f ;
		w->scale.y = 0.31f ;
		w->scale.z = 0.31f ;
		break ;
	case 16 :
		w->pos.x = 34.0f ;
		w->pos.y = 3.0f ;
		w->pos.z = -7.0f ;
		w->scale.x = 0.31f ;
		w->scale.y = 0.31f ;
		w->scale.z = 0.31f ;
		break ;
	case 17 :
		w->pos.x = 38.0f ;
		w->pos.y = 2.5f ;
		w->pos.z = -13.91f ;
		break ;
	}
}

/****************************************************************/
/*			A line each frame									*/
/****************************************************************/
void line_main( PLAYER_WORK *w )
{
	switch( w->sub_no ){
	case 4 :
		line_blink( w ) ;
		line_room_put() ;
		break ;
	case 10 :
	case 11 :
		line_arrow( w ) ;
		break ;
	case 13 :
	case 14 :
	case 15 :
	case 16 :
	case 17 :
		line_slide( w ) ;
		break ;
	}
}

/****************************************************************/
/*			The room's plate blinks								*/
/****************************************************************/
void line_blink( PLAYER_WORK *w )
{
	w->timer0 ++ ;
	w->fade.r = w->fade.g = w->fade.b = ( w->timer0 & 1 ) ? 0.8f : 1.0f ;
}

/****************************************************************/
/*			An arrow, lit while the list can scroll its way		*/
/****************************************************************/
void line_arrow( PLAYER_WORK *w )
{
	if( w->sub_no == 10 ){
		if( net_cursor_no ){
			w->disp_flag = 0 ;
			w->timer0 = 0 ;
			return ;
		}
	} else if( !net_cursor_no ){
		w->disp_flag = 0 ;
		w->timer0 = 0 ;
		return ;
	}
	w->disp_flag = 0 ;
	if( unk_8C21429C ) return ;
	w->disp_flag = 1 ;
	w->timer0 ++ ;
	w->timer0 %= 45 ;
	w->fade.r = nlSin( DEG_ANG( w->timer0 * 4 ) ) ;
	w->fade.g = nlSin( DEG_ANG( w->timer0 * 4 ) ) ;
	w->fade.b = nlSin( DEG_ANG( w->timer0 * 4 ) ) ;
}

/****************************************************************/
/*			A panel that slides in while the server is asked	*/
/****************************************************************/
void line_slide( PLAYER_WORK *w )
{
	static void ( *line_slide_jmp[4] )( PLAYER_WORK *w ) = {
		line_slide_wait, line_slide_in, line_slide_stay, line_slide_out
	} ;

	line_slide_jmp[w->routine_2]( w ) ;
}

void line_slide_wait( PLAYER_WORK *w )
{
	w->disp_flag = 0 ;
	if( !net_cursor_no && unk_8C2136FD == 3 ){
		w->disp_flag = 1 ;
		w->timer1 = 30 ;
		w->spd.x = 1.0f ;
		w->routine_2 ++ ;
		unk_8C212F20 = 1 ;
	}
}

void line_slide_in( PLAYER_WORK *w )
{
	if( w->timer1 ){
		w->timer1 -- ;
		w->pos.x -= w->spd.x ;
	}
	if( !w->timer1 ){
		w->routine_2 ++ ;
		switch( w->sub_no ){
		case 13 :
			w->pos.x = 8.2f ;
			unk_8C212F20 = 0 ;
			break ;
		case 14 :
			w->pos.x = 2.75f ;
			break ;
		case 15 :
			w->pos.x = 4.0f ;
			break ;
		case 16 :
			w->pos.x = 4.0f ;
			break ;
		case 17 :
			w->pos.x = 8.0f ;
			break ;
		}
	}
}

void line_slide_stay( PLAYER_WORK *w )
{
	if( w->sub_no == 16 ){
		w->timer0 ++ ;
		w->timer0 %= 45 ;
		w->fade.r = 1.0f - nlSin( DEG_ANG( w->timer0 * 4 ) ) * 0.9f ;
		w->fade.g = 1.0f - nlSin( DEG_ANG( w->timer0 * 4 ) ) * 0.9f ;
		w->fade.b = 1.0f - nlSin( DEG_ANG( w->timer0 * 4 ) ) * 0.9f ;
	}
	if( unk_8C2136FD == 2 && unk_8C2136FE == 6 ){
		w->routine_2 ++ ;
		w->timer1 = 30 ;
		unk_8C212F20 = 1 ;
	}
}

void line_slide_out( PLAYER_WORK *w )
{
	if( w->timer1 ){
		w->timer1 -- ;
		w->pos.x += w->spd.x ;
	}
	if( !w->timer1 ){
		w->routine_2 = 0 ;
		switch( w->sub_no ){
		case 13 :
			w->pos.x = 38.2f ;
			unk_8C212F20 = 0 ;
			break ;
		case 14 :
			w->pos.x = 32.75f ;
			break ;
		case 15 :
			w->pos.x = 34.0f ;
			break ;
		case 16 :
			w->pos.x = 34.0f ;
			break ;
		case 17 :
			w->pos.x = 38.0f ;
			break ;
		}
	}
}

/****************************************************************/
/*			The room's name										*/
/****************************************************************/
void line_room_put( void )
{
	char buf[32] ;

	str_put( 417.0f, 35.0f, 14.45f, 0xffffffff, room_info[net_room_no].name ) ;
}

void line_next( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
}

void line_free( PLAYER_WORK *w )
{
	work_free( w ) ;
}

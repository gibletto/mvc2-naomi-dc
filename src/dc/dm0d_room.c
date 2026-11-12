/* DM0D, the room list: the lobby's rooms, a cursor on the area and one on the room, the list's
   lines with each room's players and whether it is open, and the marks for the two buttons. */
#include "player.h"
#include "nl.h"
#include "dm0d.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

/* a piece of the screen: where it is, its model and how it is drawn */
typedef struct ROOM_PARTS {
	VEC3	pos;
	Sint32	obj;
	Sint32	flag;		/* obj_flag */
} ROOM_PARTS;

extern int sprintf( char *s, const char *fmt, ... ) ;
extern void dm0d_menu_return( void ) ;
extern void dm0d_clock_set( int type ) ;

void room_start( void ) ;
void room_select( void ) ;
void room_end( void ) ;
void room_stay( void ) ;
void room_nop( PLAYER_WORK *w ) ;
void room_parts_set( Uint8 type, Uint8 sub_type ) ;
void room_list_set( Uint8 type, Sint8 sub_type ) ;
void room_obj_move( PLAYER_WORK *w ) ;
void room_obj_nop( PLAYER_WORK *w ) ;
void room_obj_stay( PLAYER_WORK *w ) ;
void room_cursor0( PLAYER_WORK *w ) ;
void room_cursor1( PLAYER_WORK *w ) ;
void room_blink( PLAYER_WORK *w ) ;
void room_flow( PLAYER_WORK *w ) ;
void room_dim( PLAYER_WORK *w ) ;
void room_hide( PLAYER_WORK *w ) ;
void room_arrow_up( PLAYER_WORK *w ) ;
void room_arrow_down( PLAYER_WORK *w ) ;
void room_mark0( PLAYER_WORK *w ) ;
void room_mark1( PLAYER_WORK *w ) ;
void room_mark( PLAYER_WORK *w ) ;
void room_list_put( PLAYER_WORK *w ) ;

static Uint8 room_r0, room_r1, room_r2, room_r3 ;
static Sint8 room_no ;			/* the area the cursor is on */
static Sint8 room_line ;		/* the room the cursor is on */
static Sint8 room_no_old, room_line_old ;
static Sint8 room_wait ;		/* set while the server answers */
static Sint8 room_wait_old ;
static Uint8 room_unk_5e[32] ;
static Sint8 room_top ;			/* the first and last rooms in the list's nine lines */
static Sint8 room_bottom ;
static Sint8 room_unk_80 ;
static Uint8 room_unk_81 ;
static Sint8 room_flag0, room_flag1 ;	/* a button is held */
static Uint8 room_unk_84[4] ;	/* unused */

static VEC3 room_zero = { 0.0f, 0.0f, 0.0f } ;
static VEC3 room_one = { 1.0f, 1.0f, 1.0f } ;
static FADE_COL room_fade = { 1.0f, 1.0f, 1.0f, 1.0f } ;
static ROOM_PARTS room_parts[21] = {
	{ { 0.0f, 0.0f, 0.0f }, 0xa2, OBJ_POS | OBJ_SCALE },
	{ { 0.0f, 0.0f, 0.0f }, 0xa3, OBJ_POS | OBJ_SCALE },
	{ { 0.0f, 0.0f, 0.0f }, 0xa4, OBJ_POS | OBJ_SCALE },
	{ { 0.0f, 0.0f, 0.0f }, 0xa5, OBJ_POS | OBJ_SCALE },
	{ { 0.0f, 0.0f, 0.0f }, 0xa6, OBJ_POS | OBJ_SCALE },
	{ { -7.5f, 9.97f, -19.28f }, 0xa7, OBJ_POS | OBJ_SCALE | OBJ_FADE },
	{ { 0.0f, 0.0f, 0.0f }, 0xa8, OBJ_POS | OBJ_SCALE },
	{ { 0.0f, 0.0f, 0.0f }, 0xaa, OBJ_POS | OBJ_SCALE | OBJ_FADE },
	{ { 0.0f, 0.0f, 0.0f }, 0xab, OBJ_POS | OBJ_SCALE },
	{ { 0.0f, 0.0f, 0.0f }, 0xad, OBJ_POS | OBJ_SCALE },
	{ { 0.0f, 0.0f, 0.0f }, 0xae, OBJ_POS | OBJ_SCALE },
	{ { 0.0f, 0.0f, 0.0f }, 0xaf, OBJ_POS | OBJ_SCALE },
	{ { 0.0f, 0.0f, 0.0f }, 0xb0, OBJ_POS | OBJ_SCALE },
	{ { -7.5f, -11.23f, -19.28f }, 0x134, OBJ_POS | OBJ_SCALE | OBJ_FADE },
	{ { 0.0f, 0.0f, 0.0f }, 0xb2, OBJ_POS | OBJ_SCALE },
	{ { 0.0f, 0.0f, 0.0f }, 0xb3, OBJ_POS | OBJ_SCALE | OBJ_FADE },
	{ { 2.63f, 9.1622f, -18.712f }, 0xac, OBJ_POS | OBJ_SCALE | OBJ_FADE },
	{ { -7.5631f, 8.2194f, -19.52f }, 0xa9, OBJ_POS | OBJ_SCALE | OBJ_FADE },
	{ { 0.0f, 0.0f, 0.0f }, 0x12, OBJ_POS | OBJ_SCALE },
	{ { 0.0f, 0.0f, 0.0f }, 0x13, OBJ_POS | OBJ_SCALE },
	{ { 0.0f, 0.0f, 0.0f }, 0x10d, OBJ_POS | OBJ_SCALE },
} ;
static VEC3 room_pos0[5] = {
	{ 2.63f, 9.1622f, -18.712f },
	{ 5.8379f, 9.1622f, -18.712f },
	{ 9.03f, 9.1622f, -18.712f },
	{ 12.242f, 9.1622f, -18.712f },
	{ 15.43f, 9.1622f, -18.712f }
} ;
static VEC3 room_pos1[10] = {
	{ -7.5631f, 8.2194f, -19.52f },
	{ -7.5631f, 6.2634f, -19.52f },
	{ -7.5631f, 4.3434f, -19.52f },
	{ -7.5631f, 2.3634f, -19.52f },
	{ -7.5631f, 0.4834f, -19.52f },
	{ -7.5631f, -1.4366f, -19.52f },
	{ -7.5631f, -3.2766f, -19.52f },
	{ -7.5631f, -5.1566f, -19.52f },
	{ -7.5631f, -7.1166f, -19.52f },
	{ -7.5631f, -9.1766f, -19.52f }
} ;
static Float room_flow_step[12] = {
	3.0f, 2.0f, 0.0f, 3.0f, -2.0f, 0.0f, -3.0f, 2.0f, 0.0f, -3.0f, -2.0f, 0.0f
} ;
static Sint16 room_unk_660[16] = { 0x0, 0x1, 0x2, 0x20, 0x28, 0x1f7, 0x6a, 0xcf, 0x134, 0x199, 0x1fe, 0x263, 0x2c8, 0x32d, 0x392, 0x393 } ;
static Uint8 room_unk_680[16] = { 0, 0, 0, 2, 0, 2, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1 } ;
static char *room_str[15] = {
	"\x83\x8b\x81[\x83\x80\x82\xa0", "\x83\x8b\x81[\x83\x80\x82\xa2", "\x83\x8b\x81[\x83\x80\x82\xa4", "\x83\x8b\x81[\x83\x80\x82\xa6", "\x83\x8b\x81[\x83\x80\x82\xa8", "\x83\x8b\x81[\x83\x80\x82\xa9", "\x83\x8b\x81[\x83\x80\x82\xab", "\x83\x8b\x81[\x83\x80\x82\xad\x82\xad", "\x83\x8b\x81[\x83\x80\x96k\x8a" "C\x93\xb9", "\x83\x8b\x81[\x83\x80\x82\xb1\x82\xb1\x82\xb1\x82\xb1", "\x83\x8b\x81[\x83\x80\x82\xb3", "\x83\x8b\x81[\x83\x80\x82\xb5", "\x83\x8b\x81[\x83\x80\x82\xb7", "\x83\x8b\x81[\x83\x80\x82\xb9", "\x83\x8b\x81[\x83\x80\x82\xbb"
} ;
static char *room_unit_str[1] = { "\x90l" } ;
static char *room_open_str[2] = { "\x81\x9b", "\x81~" } ;

/****************************************************************/
/*			The room list's start								*/
/****************************************************************/
void room_init( void )
{
	room_r0 = 0 ;
	room_r1 = 0 ;
	room_r2 = 0 ;
	room_r3 = 0 ;
	work_list_kill( W_SYS ) ;
}

/****************************************************************/
/*			The room list, each frame							*/
/****************************************************************/
void dm0d_room_main( void )
{
	static void ( *room_jmp[4] )( void ) = { room_start, room_select, room_end, room_stay } ;

	room_jmp[room_r0]() ;
}

/****************************************************************/
/*			The pieces are made and the screen fades in			*/
/****************************************************************/
void room_start( void )
{
	switch( room_r1 ){
	case 0 :
		room_r1 ++ ;
		win_pict_set( 3 ) ;
		room_parts_set( 1, 2 ) ;
		room_parts_set( 1, 3 ) ;
		room_parts_set( 1, 4 ) ;
		room_parts_set( 1, 6 ) ;
		room_parts_set( 1, 8 ) ;
		room_parts_set( 1, 9 ) ;
		room_parts_set( 1, 10 ) ;
		room_parts_set( 1, 14 ) ;
		room_parts_set( 2, 16 ) ;
		room_parts_set( 3, 17 ) ;
		room_parts_set( 4, 15 ) ;
		room_parts_set( 5, 12 ) ;
		room_parts_set( 6, 7 ) ;
		room_parts_set( 7, 11 ) ;
		room_parts_set( 10, 18 ) ;
		room_parts_set( 11, 19 ) ;
		room_parts_set( 12, 20 ) ;
		room_list_set( 13, 0 ) ;
		room_no = room_no_old = 0 ;
		room_line = room_line_old = room_unk_80 = 0 ;
		room_wait = room_wait_old = 0 ;
		room_top = 0 ;
		room_bottom = 9 ;
		room_flag0 = room_flag1 = 0 ;
		dm0d_clock_set( 0 ) ;
		fade_set( 0xff000000, 10, FADE_IN ) ;
	case 1 :
		if( !fade_cnt() ){
			room_r0 ++ ;
			room_r1 = 0 ;
		}
		break ;
	}
}

/****************************************************************/
/*			The cursors follow the server's choice, and the		*/
/*			list scrolls to keep the room in its nine lines		*/
/****************************************************************/
void room_select( void )
{
	Uint16 sw0, sw1 ;

	sw0 = sw_work[0].on ;
	sw1 = sw_work[1].on ;
	sw0 = sw_work[0].now ;
	sw1 = sw_work[1].now ;
	room_wait_old = room_wait ;
	switch( room_r1 ){
	case 0 :
		room_no_old = room_no ;
		room_line_old = room_line ;
		room_no = net_area_no ;
		room_line = net_cursor_no ;
		if( room_line != room_line_old ){
			if( room_line < room_line_old ){
				if( room_line < room_top ){
					room_top = room_line ;
					room_bottom -- ;
				}
			} else if( room_line > room_bottom ){
				room_bottom = room_line ;
				room_top ++ ;
			}
		}
		if( unk_8C2136FF ){
			switch( unk_8C2141E8 ){
			case 1 :
				room_flag0 = 1 ;
				room_r1 = 1 ;
				break ;
			case 2 :
				room_flag1 = 1 ;
				room_r1 = 1 ;
				break ;
			default :
				room_r1 = 16 ;
				break ;
			}
		}
		break ;
	case 1 :
		if( !unk_8C2141E8 ){
			room_r1 = 0 ;
			room_flag1 = 0 ;
			room_flag0 = 0 ;
		}
		break ;
	case 16 :
		room_wait = 1 ;
		if( !unk_8C2136FF ){
			room_r1 = 0 ;
			room_wait = 0 ;
		}
		break ;
	}
}

/****************************************************************/
/*			The screen fades out, then the next scene			*/
/****************************************************************/
void room_end( void )
{
	switch( room_r1 ){
	case 0 :
		room_r1 ++ ;
		fade_set( 0xff000000, 10, FADE_OUT ) ;
	case 1 :
		if( !fade_cnt() ) dm0d_menu_return() ;
		break ;
	}
}

void room_stay( void )
{
}

void room_nop( PLAYER_WORK *w )
{
}

/****************************************************************/
/*			The two backdrops									*/
/****************************************************************/
void room_back_set( void )
{
	PLAYER_WORK *w ;

	win_pict_set( 0 ) ;
	if( ( w = work_get( 0, W_SYS, LIST_END ) ) ){
		w->work_func = room_nop ;
		w->type = 0 ;
		w->sub_type = 0 ;
		w->disp_flag = 1 ;
		w->pos = room_parts[19].pos ;
		w->scale = room_one ;
		w->rot[0] = 0 ;
		w->rot[1] = 0 ;
		w->rot[2] = 0 ;
		w->obj_flag = room_parts[19].flag ;
		w->obj = obj_dm0d->obj[room_parts[19].obj] ;
	}
	if( ( w = work_get( 0, W_SYS, LIST_END ) ) ){
		w->work_func = room_nop ;
		w->type = 0 ;
		w->sub_type = 0 ;
		w->disp_flag = 1 ;
		w->pos = room_parts[20].pos ;
		w->scale = room_one ;
		w->rot[0] = 0 ;
		w->rot[1] = 0 ;
		w->rot[2] = 0 ;
		w->obj_flag = room_parts[20].flag ;
		w->obj = obj_dm0d->obj[room_parts[20].obj] ;
	}
}

/****************************************************************/
/*			A piece: its kind and its place in room_parts		*/
/****************************************************************/
void room_parts_set( Uint8 type, Uint8 sub_type )
{
	PLAYER_WORK *w ;
	ROOM_PARTS *p ;

	if( ( w = work_get( 0, W_SYS, LIST_END ) ) ){
		w->work_func = room_obj_move ;
		w->type = type ;
		w->sub_type = sub_type ;
		p = &room_parts[w->sub_type] ;
		w->disp_flag = 1 ;
		w->pos = p->pos ;
		w->scale = room_one ;
		w->rot[0] = 0 ;
		w->rot[1] = 0 ;
		w->rot[2] = 0 ;
		w->obj_flag = p->flag ;
		w->obj = obj_dm0d->obj[p->obj] ;
	}
}

/****************************************************************/
/*			A piece with no model								*/
/****************************************************************/
void room_list_set( Uint8 type, Sint8 sub_type )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_END ) ) ){
		w->work_func = room_obj_move ;
		w->type = type ;
		w->sub_type = sub_type ;
		w->disp_flag = 0 ;
	}
}

/****************************************************************/
/*			A piece each frame, by its kind						*/
/****************************************************************/
void room_obj_move( PLAYER_WORK *w )
{
	static void ( *room_obj_jmp[14] )( PLAYER_WORK *w ) = {
		room_obj_nop, room_obj_stay, room_cursor0, room_cursor1, room_blink, room_flow, room_dim,
		room_hide, room_arrow_up, room_arrow_down, room_mark0, room_mark1, room_mark, room_list_put
	} ;

	room_obj_jmp[w->type]( w ) ;
}

void room_obj_nop( PLAYER_WORK *w )
{
}

void room_obj_stay( PLAYER_WORK *w )
{
	if( !w->routine_0 ) w->routine_0 ++ ;
}

/****************************************************************/
/*			The area's cursor glows and goes to the area		*/
/****************************************************************/
void room_cursor0( PLAYER_WORK *w )
{
	if( !w->routine_0 ){
		w->routine_0 ++ ;
		w->timer1 = 0 ;
	}
	w->timer1 += 9 ;
	if( w->timer1 >= 360 ) w->timer1 = 0 ;
	w->fade.r = w->fade.g = w->fade.b = nlCos( DEG_ANG( w->timer1 ) ) * 0.5f + 0.5f ;
	if( room_no != room_no_old ){
		w->pos = room_pos0[net_area_no] ;
		w->timer1 = 0 ;
	}
	if( room_wait ) w->timer1 = 0 ;
}

/****************************************************************/
/*			The room's cursor									*/
/****************************************************************/
void room_cursor1( PLAYER_WORK *w )
{
	if( !w->routine_0 ){
		w->routine_0 ++ ;
		w->timer1 = 0 ;
	}
	w->timer1 += 9 ;
	if( w->timer1 >= 360 ) w->timer1 = 0 ;
	w->fade.r = w->fade.g = w->fade.b = nlCos( DEG_ANG( w->timer1 ) ) * 0.5f + 0.5f ;
	if( room_line != room_line_old ){
		w->pos = room_pos1[net_cursor_no] ;
		w->timer1 = 0 ;
	}
	if( room_wait ) w->timer1 = 0 ;
}

/****************************************************************/
/*			A piece that flickers								*/
/****************************************************************/
void room_blink( PLAYER_WORK *w )
{
	if( !w->routine_0 ){
		w->routine_0 ++ ;
		w->timer0 = 0 ;
	}
	w->timer0 ++ ;
	w->fade.r = w->fade.g = w->fade.b = ( w->timer0 & 1 ) ? 0.8f : 1.0f ;
}

/****************************************************************/
/*			The strip whose texture runs, stopped while the		*/
/*			server answers										*/
/****************************************************************/
void room_flow( PLAYER_WORK *w )
{
	Float v, u ;

	if( !w->routine_0 ){
		w->routine_0 ++ ;
		w->timer0 = 0 ;
		w->timer1 = 0 ;
		obj_uv_clear( w->obj ) ;
	}
	if( room_wait != room_wait_old ){
		if( !room_wait ) w->disp_flag = 1 ;
		else w->disp_flag = 0 ;
	}
	if( !room_wait ){
		if( ++ w->timer0 >= 4 ) w->timer0 = 0 ;
		obj_uv_begin( obj_dm0d->obj[177], w->obj ) ;
		while( !obj_uv_next() ){
			obj_uv_get( &u, &v ) ;
			u -= 1.0f - room_flow_step[w->timer0 * 2] * 0.01f ;
			v -= 1.0f - room_flow_step[w->timer0 * 2 + 1] * 0.01f ;
			obj_uv_put( &u, &v ) ;
		}
	}
}

/****************************************************************/
/*			A piece lit while the server answers				*/
/****************************************************************/
void room_dim( PLAYER_WORK *w )
{
	if( !w->routine_0 ){
		w->routine_0 ++ ;
		w->fade.r = w->fade.g = w->fade.b = 0.5f ;
	}
	if( room_wait != room_wait_old ){
		if( !room_wait ) w->fade.r = w->fade.g = w->fade.b = 0.5f ;
		else w->fade.r = w->fade.g = w->fade.b = 1.0f ;
	}
}

/****************************************************************/
/*			A piece shown while the server answers				*/
/****************************************************************/
void room_hide( PLAYER_WORK *w )
{
	if( !w->routine_0 ){
		w->routine_0 ++ ;
		w->disp_flag = 0 ;
	}
	if( room_wait != room_wait_old ){
		if( !room_wait ) w->disp_flag = 0 ;
		else w->disp_flag = 1 ;
	}
}

/****************************************************************/
/*			The arrows, shown while the list goes on above or	*/
/*			below												*/
/****************************************************************/
void room_arrow_up( PLAYER_WORK *w )
{
	if( !w->routine_0 ){
		w->routine_0 ++ ;
		w->timer1 = w->timer0 = 0 ;
	}
	if( !room_top ) w->disp_flag = 0 ;
	else w->disp_flag = 1 ;
	w->timer1 += 18 ;
	if( w->timer1 >= 360 ) w->timer1 = 0 ;
	w->fade.r = w->fade.g = w->fade.b = nlCos( DEG_ANG( w->timer1 ) ) * 0.3f + 0.7f ;
}

void room_arrow_down( PLAYER_WORK *w )
{
	if( !w->routine_0 ){
		w->routine_0 ++ ;
		w->timer1 = w->timer0 = 0 ;
	}
	if( room_bottom == net_cursor_no ) w->disp_flag = 0 ;
	else w->disp_flag = 1 ;
	w->timer1 += 18 ;
	if( w->timer1 >= 360 ) w->timer1 = 0 ;
	w->fade.r = w->fade.g = w->fade.b = nlCos( DEG_ANG( w->timer1 ) ) * 0.3f + 0.7f ;
}

/****************************************************************/
/*			The buttons' marks									*/
/****************************************************************/
void room_mark0( PLAYER_WORK *w )
{
	if( !w->routine_0 ) w->routine_0 ++ ;
	if( !room_flag0 ) w->disp_flag = 0 ;
	else w->disp_flag = 1 ;
}

void room_mark1( PLAYER_WORK *w )
{
	if( !w->routine_0 ) w->routine_0 ++ ;
	if( !room_flag1 ) w->disp_flag = 0 ;
	else w->disp_flag = 1 ;
}

void room_mark( PLAYER_WORK *w )
{
	if( !w->routine_0 ) w->routine_0 ++ ;
	if( room_flag0 || room_flag1 ) w->disp_flag = 1 ;
	else w->disp_flag = 0 ;
}

/****************************************************************/
/*			The list's lines: each room's name, its players		*/
/*			and whether it is open								*/
/****************************************************************/
void room_list_put( PLAYER_WORK *w )
{
	char buf[16] ;
	register Sint8 n ;
	register Uint8 i ;

	if( !w->routine_0 ) w->routine_0 ++ ;
	n = room_info_cnt ;
	for( i = 0 ; i < n ; i ++ ){
		str_put( 60.0f, 96.0f + i * 31.5f, 14.45f, 0xffffffff, room_info[room_top + i].name ) ;
		sprintf( buf, "%3d", room_info[room_top + i].member_cnt ) ;
		str_put( 220.0f, 96.0f + i * 31.5f, 14.45f, 0xff0affff, buf ) ;
		str_put( 260.0f, 96.0f + i * 31.5f, 14.45f, 0xff0affff, room_unit_str[0] ) ;
		if( room_info[room_top + i].open_flag ) str_put( 298.0f, 96.0f + i * 31.5f, 14.45f, 0xffffff0a, room_open_str[0] ) ;
		else str_put( 298.0f, 96.0f + i * 31.5f, 14.45f, 0xffffff0a, room_open_str[1] ) ;
	}
}

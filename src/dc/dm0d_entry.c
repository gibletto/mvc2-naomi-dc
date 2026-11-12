/* DM0D, the entry screen: the player's number typed on the keyboard, today's date, and a choice
   that goes on to the memory card or back to the menu. */
#include "player.h"
#include "shinobi.h"
#include "dm0d.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

/* a piece to make: its kind, the model and the place in entry_pos */
typedef struct ENTRY_LIST {
	Uint8	type;
	Uint8	sub_type;
	Uint8	obj;
	Uint8	pos;
} ENTRY_LIST;

extern int sprintf( char *s, const char *fmt, ... ) ;
extern char *strcpy( char *s1, const char *s2 ) ;
extern int decide_kbd_ck( void ) ;
extern int cancel_kbd_ck( void ) ;
extern int ud_select_loop( Uint16 sw, int no, int max ) ;
extern int lr_select_loop( Uint16 sw, int no, int max ) ;
extern Uint16 menu_sw_kbd_get( Uint8 side_bit ) ;
extern void net_scene_set( int no ) ;
extern void dm0d_menu_return( void ) ;
extern void net_vms_init( void ) ;
extern int net_vms_main( Uint8 kind ) ;

void entry_parts_set( void ) ;
PLAYER_WORK *entry_work_set( int type, int sub_type, int obj, VEC3 *pos ) ;
void entry_obj_move( PLAYER_WORK *w ) ;
void entry_flow( PLAYER_WORK *w ) ;
void entry_text( PLAYER_WORK *w ) ;
void entry_cursor( PLAYER_WORK *w ) ;

static Uint8 entry_r0 ;
static Uint8 entry_line ;		/* the line the cursor is on */
static Uint8 entry_kbd ;		/* the keyboard is up */
static Uint16 entry_year0 ;
static Uint8 entry_month0, entry_day0 ;
static Uint16 entry_year1 ;
static Uint8 entry_month1, entry_day1 ;
static Sint32 entry_num0, entry_num1, entry_num2 ;
static Uint8 entry_unk_a8[20] ;
static Uint8 entry_card ;		/* go on to the memory card */

static VEC3 entry_zero = { 0.0f, 0.0f, 0.0f } ;
static VEC3 entry_one = { 1.0f, 1.0f, 1.0f } ;
static VEC3 entry_scale0 = { 0.35f, 1.0f, 1.0f } ;
static VEC3 entry_scale1 = { 0.9f, 0.9f, 1.0f } ;
static VEC3 entry_scale2 = { 0.65f, 1.0f, 1.0f } ;
static FADE_COL entry_black = { 1.0f, 0.0f, 0.0f, 0.0f } ;
static FADE_COL entry_fade = { 1.0f, 1.0f, 1.0f, 1.0f } ;
static ENTRY_LIST entry_list[16] = {
	{ 1, 0x00, 0xde, 0 }, { 1, 0x01, 0xdf, 1 }, { 1, 0x02, 0xe9, 2 }, { 1, 0x03, 0xe9, 3 },
	{ 1, 0x10, 0xe2, 17 }, { 1, 0x11, 0xe2, 19 }, { 1, 0x31, 0xda, 16 }, { 1, 0x34, 0xe8, 0 },
	{ 1, 0x40, 0xe5, 4 }, { 1, 0x41, 0xe5, 5 }, { 1, 0x42, 0xe5, 6 }, { 2, 0x00, 0xe0, 20 },
	{ 2, 0x01, 0xe1, 22 }, { 2, 0x02, 0xe1, 23 }, { 2, 0x03, 0xe3, 21 }, { 0xff, 0xff, 0xff, 0xff },
} ;
static VEC3 entry_pos[24] = {
	{ 0.0f, 0.0f, 0.0f },
	{ 0.0f, 3.1f, -12.9f },
	{ 0.0f, 0.5f, -12.9f },
	{ 0.0f, -2.1f, -12.9f },
	{ 4.0f, 3.1f, -12.9f },
	{ 4.0f, 0.5f, -12.9f },
	{ 4.0f, -2.1f, -12.9f },
	{ 7.0f, 3.0f, -12.9f },
	{ 7.0f, 1.5f, -12.9f },
	{ 7.0f, -1.0f, -12.9f },
	{ 10.5f, 3.0f, -12.9f },
	{ 10.5f, 1.5f, -12.9f },
	{ 10.5f, -1.0f, -12.9f },
	{ 8.2f, 6.45f, -12.9f },
	{ 8.2f, 5.2f, -12.9f },
	{ -8.5f, -5.5f, -12.9f },
	{ -2.0f, -5.5f, -12.9f },
	{ -8.5f, -5.5f, -12.9f },
	{ 0.0f, 0.0f, 0.0f },
	{ -2.0f, -5.5f, -12.9f },
	{ 0.0f, 3.1f, -12.8f },
	{ -5.25f, -5.5f, -12.8f },
	{ 0.0f, 3.1f, -13.0f },
	{ 0.0f, -5.5f, -13.0f }
} ;
static Float entry_cursor_x[3] = { 0.8f, 7.0f, 10.5f } ;
static Float entry_yes_x[2] = { -8.5f, -2.0f } ;
static Sint16 entry_unk_ab8[3] = { 0x27, 0x37, 0x47 } ;
static Float entry_num_y[3] = { 153.0f, 218.0f, 283.0f } ;

/****************************************************************/
/*			The entry screen's start: today's date				*/
/****************************************************************/
void dm0d_entry_init( void )
{
	SYS_RTC_DATE date ;

	entry_r0 = 0 ;
	entry_line = 0 ;
	entry_kbd = 0 ;
	syRtcGetDate( &date ) ;
	entry_year1 = entry_year0 = date.year ;
	entry_month1 = entry_month0 = date.month ;
	entry_day1 = entry_day0 = date.day ;
	entry_card = 0 ;
	entry_num0 = net_entry_num ;
	entry_num1 = unk_8C213AF8 ;
	entry_num2 = unk_8C213AFC ;
	sprintf( net_entry_str, "%d", net_entry_num ) ;
}

/****************************************************************/
/*			The entry screen, each frame						*/
/****************************************************************/
void dm0d_entry_main( void )
{
	KB_SET kb ;
	Uint16 sw0, sw1 ;
	int ret ;

	sw0 = sw_work[0].on ;
	sw1 = sw_work[1].on ;
	sw0 = sw_work[0].now ;
	sw1 = sw_work[1].now ;
	switch( entry_r0 ){
	case 0 :
		entry_r0 ++ ;
		work_list_kill( W_SYS ) ;
		Sys->timer0 = 0 ;
		win_pict_set( 2 ) ;
		entry_parts_set() ;
		fade_set( 0xff000000, 10, FADE_IN ) ;
		break ;
	case 1 :
	case 6 :
		if( !fade_cnt() ) entry_r0 ++ ;
		break ;
	case 2 :
		if( decide_kbd_ck() ){
			if( entry_line ){
				entry_r0 = 6 ;
				entry_card = Sys->timer0 ^ 1 ;
				Sys->timer0 = 0 ;
				fade_set( 0xff000000, 10, FADE_OUT ) ;
			} else {
				entry_r0 ++ ;
				Sys->timer0 = 0 ;
			}
		} else if( cancel_kbd_ck() ){
			entry_r0 = 6 ;
			Sys->timer0 = 0 ;
			fade_set( 0xff000000, 10, FADE_OUT ) ;
		} else {
			entry_line = ud_select_loop( menu_sw_kbd_get( 3 ), entry_line, 1 ) ;
			if( entry_line ) Sys->timer0 = lr_select_loop( menu_sw_kbd_get( 3 ), Sys->timer0, 1 ) ;
			else Sys->timer0 = 0 ;
		}
		break ;
	case 3 :
		if( !entry_kbd ){
			if( decide_kbd_ck() ){
				entry_kbd = 1 ;
				kb.max = 7 ;
				kb.mode = 130 ;
				kb.mode_ok = 146 ;
				kb.str = net_entry_str ;
				kb.size = 1 ;
				kb_init( &kb ) ;
			} else if( cancel_kbd_ck() ){
				entry_r0 = 2 ;
				Sys->timer0 = 0 ;
			}
		} else if( kb_main() ){
			entry_r0 = 4 ;
			Sys->timer0 = 0 ;
			if( ( entry_num0 = str_num_get( net_entry_str ) ) < 0 ) entry_num0 = 0 ;
			sprintf( net_entry_str, "%d", entry_num0 ) ;
		}
		break ;
	case 4 :
		if( decide_kbd_ck() ){
			entry_r0 = 2 ;
			if( Sys->timer0 ){
				entry_num0 = 0 ;
				sprintf( net_entry_str, "%d", 0 ) ;
			}
			Sys->timer0 = 0 ;
		} else if( cancel_kbd_ck() ){
			entry_r0 = 3 ;
			Sys->timer0 = 0 ;
		} else {
			Sys->timer0 = lr_select_loop( menu_sw_kbd_get( 3 ), Sys->timer0, 1 ) ;
		}
		break ;
	case 7 :
		if( entry_card ){
			entry_r0 ++ ;
			net_vms_init() ;
			net_entry_num = str_num_get( net_entry_str ) ;
			strcpy( net_rec[0].entry_str, net_entry_str ) ;
			strcpy( net_rec[1].entry_str, net_entry_str ) ;
		} else {
			dm0d_menu_return() ;
		}
		break ;
	case 8 :
		if( ( ret = net_vms_main( 3 ) ) != 2 ){
			if( ret == 1 ) dm0d_menu_return() ;
			else net_scene_set( 0 ) ;
		}
		break ;
	}
}

/****************************************************************/
/*			The pieces, from entry_list							*/
/****************************************************************/
void entry_parts_set( void )
{
	ENTRY_LIST *p ;

	p = entry_list ;
	while( p->type != 0xff ){
		entry_work_set( p->type, p->sub_type, p->obj, p->pos + entry_pos ) ;
		p ++ ;
	}
}

/****************************************************************/
/*			A piece: its kind, its model and where it is		*/
/****************************************************************/
PLAYER_WORK *entry_work_set( int type, int sub_type, int obj, VEC3 *pos )
{
	PLAYER_WORK *w ;

	if( !( w = work_get( 0, W_SYS, LIST_TOP ) ) ) return 0 ;
	w->disp_flag = 1 ;
	w->work_func = entry_obj_move ;
	w->obj = obj_dm0d->obj[obj] ;
	w->sub_no = obj ;
	w->type = type ;
	w->sub_type = sub_type ;
	w->pos = *pos ;
	w->scale = entry_one ;
	w->fade = entry_fade ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	return w ;
}

/****************************************************************/
/*			A piece each frame, by its kind						*/
/****************************************************************/
void entry_obj_move( PLAYER_WORK *w )
{
	static void ( *entry_obj_jmp[3] )( PLAYER_WORK *w ) = { entry_flow, entry_text, entry_cursor } ;

	entry_obj_jmp[w->type]( w ) ;
}

/****************************************************************/
/*			A piece whose picture scrolls						*/
/****************************************************************/
void entry_flow( PLAYER_WORK *w )
{
	Float v, u ;

	if( !w->sub_type ) return ;
	switch( w->routine_0 ){
	case 0 :
		w->routine_0 ++ ;
		w->timer1 = w->timer0 = 0 ;
		obj_uv_clear( w->obj ) ;
		break ;
	case 1 :
		if( ++ w->timer0 >= 1000 ) w->timer0 = 0 ;
		if( ++ w->timer1 >= 1000 ) w->timer1 = 0 ;
		obj_uv_begin( obj_dm0d->obj[w->sub_no + 1], w->obj ) ;
		while( !obj_uv_next() ){
			obj_uv_get( &u, &v ) ;
			u -= 1.0f - w->timer0 * 0.001f ;
			v -= 1.0f - w->timer1 * 0.001f ;
			obj_uv_put( &u, &v ) ;
		}
		break ;
	}
}

/****************************************************************/
/*			The numbers and the dates							*/
/****************************************************************/
void entry_text( PLAYER_WORK *w )
{
	int i ;
	char str[16] ;
	int num ;

	if( ( w->sub_type & 0xf0 ) == 0x40 ){
		switch( i = w->sub_type & ~0xf0 ){
		case 0 :
			num = entry_num0 ;
			break ;
		case 1 :
			num = entry_num1 ;
			break ;
		case 2 :
			num = entry_num2 ;
			break ;
		}
		if( num > 9999999 ) num = 9999999 ;
		num_comma_set( num, str ) ;
		str_put( 294.0f, entry_num_y[i], 11.5f, 0xffffffff, str ) ;
	}
	if( ( w->sub_type & 0xf0 ) == 0x20 ){
		if( !( w->sub_type & ~0xf0 ) ){
			sprintf( str, "%4d/%2d/%2d", entry_year0, entry_month0, entry_day0 ) ;
			STrPrint( 115, 18, 29, str ) ;
		} else {
			sprintf( str, "%4d/%2d/%2d", entry_year1, entry_month1, entry_day1 ) ;
			STrPrint( 115, 26, 29, str ) ;
		}
	}
	if( w->sub_type == 0x30 ) w->scale = entry_scale1 ;
}

/****************************************************************/
/*			The cursors: each shows on its own step				*/
/****************************************************************/
void entry_cursor( PLAYER_WORK *w )
{
	int i ;

	w->disp_flag = 0 ;
	w->spd = w->pos ;
	w->timer1 += 9 ;
	if( w->timer1 >= 360 ) w->timer1 = 0 ;
	w->fade.trnsl = nlCos( DEG_ANG( w->timer1 ) ) * 0.5f + 0.5f ;
	switch( w->sub_type ){
	case 0 :
		if( !entry_line ) w->disp_flag = 1 ;
		break ;
	case 1 :
		if( entry_r0 >= 3 && entry_r0 <= 4 ){
			i = Sys->timer0 + entry_r0 - 3 ;
			w->disp_flag = 1 ;
			w->pos.x = entry_cursor_x[i] ;
			if( i ) w->scale = entry_scale0 ;
			else w->scale = entry_scale1 ;
		}
		break ;
	case 2 :
		if( entry_r0 == 2 && entry_line ){
			w->disp_flag = 1 ;
			w->pos.x = entry_yes_x[Sys->timer0] ;
			w->scale = entry_scale2 ;
		}
		break ;
	case 3 :
		if( entry_line ) w->disp_flag = 1 ;
		break ;
	}
	if( !w->disp_flag || w->spd.x != w->pos.x || w->spd.y != w->pos.y ) w->timer1 = 0 ;
}

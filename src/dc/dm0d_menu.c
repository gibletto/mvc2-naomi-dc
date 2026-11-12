/* DM0D, the network menu: three choices with a cursor and a picture for the one it is on; the
   choice goes on to the lobby, to the entry screen, or back out of the network. */
#include "player.h"
#include "nl.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

/* a piece of the screen: indexes into the position, angle and scale tables, its model and how
   it is drawn */
typedef struct MENU_PARTS {
	Uint8	pos;
	Uint8	rot;
	Uint8	scale;
	Uint8	pad;
	Sint32	obj;
	Sint32	flag;		/* obj_flag */
} MENU_PARTS;

/* a piece to make: its kind and its place in menu_parts */
typedef struct MENU_LIST {
	Sint8	type;
	Sint8	sub_type;
} MENU_LIST;

extern int decide_ck( void ) ;
extern int cancel_ck( void ) ;
extern int ud_select_loop( Uint16 sw, int no, int max ) ;
extern Uint16 menu_sw_get( Uint8 side_bit ) ;
extern void win_pict_set( int no ) ;
extern void net_scene_clear( void ) ;
extern void dm0d_entry_init( void ) ;
extern void dm0d_entry_main( void ) ;

void dm0d_menu_init( void ) ;
void dm0d_menu_main( void ) ;
void menu_lobby( void ) ;
void menu_entry( void ) ;
void menu_exit( void ) ;
void dm0d_menu_return( void ) ;
void menu_parts_set( void ) ;
void menu_obj_move( PLAYER_WORK *w ) ;
void menu_obj_nop( PLAYER_WORK *w ) ;
void menu_piece( PLAYER_WORK *w ) ;
void menu_cursor( PLAYER_WORK *w ) ;
void menu_text( PLAYER_WORK *w ) ;

static Uint8 menu_r0, menu_r1, menu_r2, menu_r3 ;
static Uint8 menu_sel ;			/* the choice made */

static MENU_PARTS menu_parts[12] = {
	{ 1, 0, 1, 0, 0xc, OBJ_POS | OBJ_SCALE },
	{ 10, 0, 1, 0, 0xd, OBJ_POS | OBJ_SCALE },
	{ 2, 0, 2, 0, 0x10, OBJ_POS | OBJ_SCALE },
	{ 3, 0, 3, 0, 0x11, OBJ_POS | OBJ_SCALE },
	{ 4, 0, 0, 0, 0xd5, OBJ_POS },
	{ 5, 0, 0, 0, 0xd5, OBJ_POS },
	{ 6, 0, 0, 0, 0xd5, OBJ_POS },
	{ 7, 0, 0, 0, 0xd6, OBJ_POS },
	{ 8, 0, 0, 0, 0xd6, OBJ_POS },
	{ 9, 0, 0, 0, 0xd6, OBJ_POS },
	{ 0, 0, 0, 0, 0xd9, OBJ_POS },
	{ 6, 0, 0, 0, 0xda, OBJ_POS },
} ;
static VEC3 menu_pos[11] = {
	{ 0.0f, 0.0f, 0.0f },
	{ 0.0f, -6.0f, -14.0f },
	{ 0.0f, 5.5f, -11.0f },
	{ 0.0f, 5.7f, -11.5f },
	{ 0.0f, 3.8f, -12.9f },
	{ 0.0f, 1.2f, -12.9f },
	{ 0.0f, -1.4f, -12.9f },
	{ 0.0f, 3.8f, -13.0f },
	{ 0.0f, 1.2f, -13.0f },
	{ 0.0f, -1.4f, -13.0f },
	{ 0.0f, -6.0f, -14.1f }
} ;
static Sint32 menu_rot[1][3] = { { 0, 0, 0 } } ;
static VEC3 menu_scale[4] = {
	{ 1.0f, 1.0f, 1.0f },
	{ 1.12f, 1.12f, 1.0f },
	{ 1.3f, 1.3f, 1.0f },
	{ 1.5f, 1.5f, 1.0f }
} ;

/****************************************************************/
/*			The menu's start									*/
/****************************************************************/
void dm0d_menu_init( void )
{
	menu_r0 = menu_r1 = menu_r2 = menu_r3 = 0 ;
	work_init() ;
	Sys->timer0 = menu_sel = 0 ;
}

/****************************************************************/
/*			The menu, each frame								*/
/****************************************************************/
void dm0d_menu_main( void )
{
	switch( menu_r1 ){
	case 0 :
		menu_r1 ++ ;
		menu_parts_set() ;
		fade_set( 0xff000000, 10, FADE_IN ) ;
		break ;
	case 1 :
	case 3 :
		if( !fade_cnt() ) menu_r1 ++ ;
		break ;
	case 2 :
		if( cancel_ck() ){
			menu_r1 = 3 ;
			menu_r2 = 0 ;
			menu_sel = 2 ;
			fade_set( 0xff000000, 10, FADE_OUT ) ;
		} else if( decide_ck() ){
			menu_r1 = 3 ;
			menu_r2 = 0 ;
			menu_sel = Sys->timer0 ;
			fade_set( 0xff000000, 10, FADE_OUT ) ;
		} else {
			Sys->timer0 = ud_select_loop( menu_sw_get( 3 ), Sys->timer0, 2 ) ;
		}
		break ;
	case 4 :
		switch( menu_sel ){
		case 0 :
			menu_lobby() ;
			break ;
		case 1 :
			menu_entry() ;
			break ;
		case 2 :
			menu_exit() ;
			break ;
		}
		break ;
	}
}

void menu_lobby( void )
{
	unk_8C212F1B = 0x80 ;
}

void menu_entry( void )
{
	switch( menu_r2 ){
	case 0 :
		menu_r2 ++ ;
		dm0d_entry_init() ;
	case 1 :
		dm0d_entry_main() ;
		break ;
	}
}

void menu_exit( void )
{
	net_scene_clear() ;
}

/****************************************************************/
/*			Back to the menu, the cursor on the last choice		*/
/****************************************************************/
void dm0d_menu_return( void )
{
	menu_r0 = menu_r1 = menu_r2 = menu_r3 = 0 ;
	work_init() ;
	Sys->timer0 = menu_sel ;
}

/****************************************************************/
/*			The pieces: each kind and its place in menu_parts	*/
/****************************************************************/
void menu_parts_set( void )
{
	static MENU_LIST menu_list[16] = {
		{ 1, 0 }, { 1, 1 }, { 1, 2 }, { 1, 3 }, { 1, 4 }, { 1, 5 }, { 1, 6 }, { 1, 7 },
		{ 1, 8 }, { 1, 9 }, { 1, 10 }, { 1, 11 }, { 2, 0 }, { 2, 1 }, { 3, 0 }, { -1, 0 }
	} ;
	PLAYER_WORK *w ;
	MENU_LIST *p ;
	register int i ;

	for( i = 0 ; menu_list[i].type > 0 ; i ++ ){
		if( ( w = work_get( 0, W_SYS, LIST_END ) ) ){
			p = &menu_list[i] ;
			w->work_func = menu_obj_move ;
			w->type = menu_list[i].type ;
			w->sub_type = p->sub_type ;
		}
	}
	win_pict_set( 2 ) ;
}

/****************************************************************/
/*			A piece each frame, by its kind						*/
/****************************************************************/
void menu_obj_move( PLAYER_WORK *w )
{
	static void ( *menu_obj_jmp[4] )( PLAYER_WORK *w ) = {
		menu_obj_nop, menu_piece, menu_cursor, menu_text
	} ;

	menu_obj_jmp[w->type]( w ) ;
}

void menu_obj_nop( PLAYER_WORK *w )
{
}

/****************************************************************/
/*			A piece that stays as it is set						*/
/****************************************************************/
void menu_piece( PLAYER_WORK *w )
{
	MENU_PARTS *p ;

	if( w->routine_0 == 0 ){
		p = &menu_parts[w->sub_type] ;
		w->routine_0 ++ ;
		w->disp_flag = 1 ;
		w->pos = menu_pos[p->pos] ;
		w->rot[0] = menu_rot[p->rot][0] ;
		w->rot[1] = menu_rot[p->rot][1] ;
		w->rot[2] = menu_rot[p->rot][2] ;
		w->scale = menu_scale[p->scale] ;
		w->obj_flag = p->flag ;
		w->obj = obj_dm0d->obj[p->obj] ;
	}
}

/****************************************************************/
/*			The cursor: it pulses on the choice it is on		*/
/****************************************************************/
void menu_cursor( PLAYER_WORK *w )
{
	static VEC3 menu_cursor_pos[2][3] = {
		{ { 0.0f, 3.8f, -12.8f }, { 0.0f, 1.2f, -12.8f }, { 0.0f, -1.4f, -12.8f } },
		{ { 0.0f, 3.8f, -12.95f }, { 0.0f, 1.2f, -12.95f }, { 0.0f, -1.4f, -12.95f } }
	} ;

	if( w->routine_0 == 0 ){
		w->routine_0 ++ ;
		w->disp_flag = 1 ;
		w->obj_flag = OBJ_POS | OBJ_TRNSL ;
		w->scale = menu_scale[0] ;
		w->obj = obj_dm0d->obj[w->sub_type + 215] ;
		w->timer1 = 0 ;
		w->routine_1 = Sys->timer0 ;
	}
	w->timer1 += 9 ;
	if( w->timer1 >= 360 ) w->timer1 = 0 ;
	w->fade.trnsl = nlCos( DEG_ANG( w->timer1 ) ) * 0.5f + 0.5f ;
	if( w->routine_1 != Sys->timer0 ){
		w->routine_1 = Sys->timer0 ;
		w->timer1 = 0 ;
	}
	w->pos = menu_cursor_pos[w->sub_type][Sys->timer0] ;
}

/****************************************************************/
/*			The picture for the choice the cursor is on			*/
/****************************************************************/
void menu_text( PLAYER_WORK *w )
{
	if( w->routine_0 == 0 ){
		w->routine_0 ++ ;
		w->disp_flag = 1 ;
	}
	w->obj = obj_dm0d->obj[Sys->timer0 + 219] ;
}

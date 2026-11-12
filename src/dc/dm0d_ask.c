/* DM0D, a question with two answers: its frame, a cursor on the answer net_cursor_no is on, and
   the text, all gone once unk_8C212F20 says it is answered. */
#include "player.h"
#include "nl.h"
#include "dm0d.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

PLAYER_WORK *ask_parts_set( int type, int obj ) ;
void ask_move( PLAYER_WORK *w ) ;
void ask_top( PLAYER_WORK *w ) ;
void ask_parts( PLAYER_WORK *w ) ;
void ask_cursor( PLAYER_WORK *w ) ;
void ask_text( PLAYER_WORK *w ) ;

static VEC3 ask_scale = { 1.0f, 1.0f, 1.0f } ;
static FADE_COL ask_fade = { 1.0f, 1.0f, 1.0f, 1.0f } ;
static VEC3 ask_cursor_pos[2] = { { -1.77f, -0.24f, -12.04f }, { 1.33f, -0.24f, -12.04f } } ;

void dm0d_ask_set( void )
{
	ask_parts_set( 0, 0x105 ) ;
}

/* type is the part's kind, and timer0 keeps its model's number */
PLAYER_WORK *ask_parts_set( int type, int obj )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_END ) ) ){
		w->disp_flag = 1 ;
		w->work_func = ask_move ;
		w->timer0 = obj ;
		w->type = type ;
		w->obj = obj_dm0d->obj[obj] ;
		w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
		w->scale = ask_scale ;
		w->fade = ask_fade ;
	}
	return w ;
}

void ask_move( PLAYER_WORK *w )
{
	static void ( *ask_jmp[4] )( PLAYER_WORK *w ) = { ask_top, ask_parts, ask_cursor, ask_text } ;

	ask_jmp[w->type]( w ) ;
}

/* the frame sets the others, then the answer's text */
void ask_top( PLAYER_WORK *w )
{
	switch( w->routine_0 ){
	case 0 :
		w->routine_0 ++ ;
		w->pos.x = 0.0f ;
		w->pos.y = 0.0f ;
		w->pos.z = 0.0f ;
		ask_parts_set( 1, 0x118 ) ;
		ask_parts_set( 1, 0x119 ) ;
		ask_parts_set( 2, 0x11d ) ;
		break ;
	case 1 :
		if( unk_8C212F20 ){
			w->routine_0 ++ ;
			ask_parts_set( 3, unk_8C212F20 + 0x119 ) ;
			if( unk_8C212F20 == 1 ) ask_parts_set( 3, 56 ) ;
		}
		break ;
	case 2 :
		break ;
	}
}

void ask_parts( PLAYER_WORK *w )
{
	if( !w->routine_0 ){
		if( unk_8C212F20 ){
			w->routine_0 ++ ;
			w->disp_flag = 0 ;
		}
	} else {
		work_free( w ) ;
	}
}

void ask_cursor( PLAYER_WORK *w )
{
	Float x ;

	w->timer1 += 9 ;
	if( w->timer1 >= 360 ) w->timer1 = 0 ;
	w->fade.trnsl = nlCos( DEG_ANG( w->timer1 ) ) * 0.5f + 0.5f ;
	switch( w->routine_0 ){
	case 0 :
		w->routine_0 ++ ;
	case 1 :
		x = w->pos.x ;
		w->pos = ask_cursor_pos[net_cursor_no] ;
		if( w->pos.x != x ) w->timer1 = 0 ;
		if( unk_8C212F20 ){
			w->routine_0 ++ ;
			w->disp_flag = 0 ;
		}
		break ;
	case 2 :
		work_free( w ) ;
		break ;
	}
}

void ask_text( PLAYER_WORK *w )
{
	if( w->timer0 == 56 ){
		str_put( 240.0f, 212.0f, 12.1f, 0xffffffff, net_vs_handle ) ;
		str_put( 261.0f, 244.0f, 12.1f, 0xffffffff, net_vs_key ) ;
	}
}

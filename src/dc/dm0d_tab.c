/* DM0D, two tabs: a frame of eight pieces, a pulsing cursor on the tab the list shows, and the
   tabs' title. */
#include "player.h"
#include "nl.h"
#include "dm0d.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

/* a piece of the frame: its model, place and size */
typedef struct TAB_PARTS {
	Uint16	obj;
	VEC3	pos;
	VEC3	scale;
} TAB_PARTS;

void tab_move( PLAYER_WORK *w ) ;

static TAB_PARTS tab_parts[9] = {
	{ 0xb2, { 0.75f, -0.3f, -2.7f }, { 0.5f, 0.5f, 0.5f } },
	{ 0xb2, { -14.2f, -0.3f, -2.7f }, { 0.5f, 0.5f, 0.5f } },
	{ 0x10c, { 1.94f, -1.72f, 0.54f }, { 1.0f, 1.0f, 1.0f } },
	{ 0x10b, { 0.21f, 0.15f, 0.35f }, { 1.0f, 1.0f, 1.0f } },
	{ 0x109, { 0.0f, 1.6f, -12.67f }, { 1.0f, 1.0f, 1.0f } },
	{ 0x8d, { 0.0f, -6.5f, 0.0f }, { 0.8f, 1.0f, 1.0f } },
	{ 0xd2, { 0.0f, 0.0f, -8.7f }, { 1.5f, 1.7f, 1.0f } },
	{ 3, { 0.11f, 4.41f, 0.0f }, { 1.0f, 1.0f, 1.0f } },
	{ 4, { 0.11f, 4.7f, 0.0f }, { 1.0f, 1.0f, 1.0f } },
} ;
static VEC3 tab_cursor_pos[2] = { { -14.08f, -0.15f, -2.62f }, { 0.59f, -0.15f, -2.62f } } ;
static Uint16 tab_title_obj[3] = { 0x106, 0x108, 0x107 } ;
static FADE_COL tab_fade = { 1.0f, 1.0f, 1.0f, 1.0f } ;

/****************************************************************/
/*			The frame, the cursor and title					*/
/****************************************************************/
#define TAB_ONE	0	/* a build setting: the frame's first piece alone */
void dm0d_tab_set( Uint16 no )
{
	PLAYER_WORK *w ;
	TAB_PARTS *p ;
	Sint8 i ;

	p = tab_parts ;
	for( i = 0 ; i < 8 ; i ++ ){
		if( !( w = work_get( 0, W_SYS, LIST_TOP ) ) ) return ;
		w->disp_flag = 1 ;
		w->work_func = tab_move ;
		w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
		w->fade = tab_fade ;
		w->type = i ;
		w->sub_type = 0 ;
		w->pos.x = p->pos.x ;
		w->pos.y = p->pos.y ;
		w->pos.z = p->pos.z ;
		w->scale.x = p->scale.x ;
		w->scale.y = p->scale.y ;
		w->scale.z = p->scale.z ;
		w->obj = obj_dm0d->obj[p->obj] ;
		p ++ ;
		if( TAB_ONE ) break ;	/* left out of the build */
	}
	if( !( w = work_get( 0, W_SYS, LIST_TOP ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = tab_move ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->fade = tab_fade ;
	w->scale.x = 0.5f ;
	w->scale.y = 0.5f ;
	w->scale.z = 0.5f ;
	w->type = net_cursor_no ;
	w->sub_type = 1 ;
	w->pos = tab_cursor_pos[net_cursor_no] ;
	w->obj = obj_dm0d->obj[0xaf] ;
	w->timer1 = 0 ;
	if( !( w = work_get( 0, W_SYS, LIST_TOP ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = tab_move ;
	w->obj_flag = OBJ_POS | OBJ_TRNSL | OBJ_FADE ;
	w->fade = tab_fade ;
	w->type = 0 ;
	w->sub_type = 0 ;
	w->pos.x = 0.0f ;
	w->pos.y = 0.0f ;
	w->pos.z = 0.0f ;
	w->sub_no = no ;
	w->obj = obj_dm0d->obj[tab_title_obj[w->sub_no]] ;
}

/* the cursor follows the tab and pulses; the rest stand still */
void tab_move( PLAYER_WORK *w )
{
	if( w->sub_type ){
		w->pos = tab_cursor_pos[net_cursor_no] ;
		if( w->type != net_cursor_no ){
			w->timer1 = 0 ;
			w->type = net_cursor_no ;
			w->fade = tab_fade ;
		}
		w->timer1 += 9 ;
		if( w->timer1 >= 360 ) w->timer1 = 0 ;
		w->fade.r = w->fade.g = w->fade.b = nlCos( DEG_ANG( w->timer1 ) ) * 0.5f + 0.5f ;
	}
}

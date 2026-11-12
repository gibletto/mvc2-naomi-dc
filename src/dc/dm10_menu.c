/* DM10, the memory card menu's parts: the pieces of each of its three screens, from a table, with
   a scrolled picture, a turned piece, the cursor and the eight card ports. */
#include "player.h"
#include "nl.h"
#include "dm.h"

PLAYER_WORK *dm10_sub_set( int type, int n, int obj, VEC3 *pos ) ;
void dm10_move( PLAYER_WORK *w ) ;
void dm10_flow( PLAYER_WORK *w ) ;
void dm10_turn( PLAYER_WORK *w ) ;
void dm10_cursor( PLAYER_WORK *w ) ;
void dm10_port( PLAYER_WORK *w ) ;

static VEC3 dm10_zero = { 0.0f, 0.0f, 0.0f } ;	/* unused */
static VEC3 dm10_one = { 1.0f, 1.0f, 1.0f } ;
static VEC3 dm10_big = { 1.2f, 1.2f, 1.2f } ;
static FADE_COL dm10_none = { 1.0f, 0.0f, 0.0f, 0.0f } ;	/* no card */
static FADE_COL dm10_white = { 1.0f, 1.0f, 1.0f, 1.0f } ;
static FADE_COL dm10_grey = { 1.0f, 0.6f, 0.6f, 0.6f } ;	/* a card */

/* each screen's pieces: kind (0 scrolled, 1 still, 2 the cursor, 3 a port), number, model,
   position; 0xff at the end */
static Uint8 dm10_tbl_0[8][4] = {
	{ 1, 0, 2, 1 }, { 1, 1, 6, 1 }, { 1, 2, 3, 2 }, { 1, 3, 4, 2 },
	{ 1, 7, 0x26, 1 }, { 1, 4, 0x0f, 1 }, { 2, 0, 0x49, 4 },
	{ 0xff, 0xff, 0xff, 0xff }
} ;
static Uint8 dm10_tbl_1[16][4] = {
	{ 1, 0, 2, 1 }, { 1, 1, 6, 1 }, { 1, 2, 3, 2 }, { 1, 3, 4, 2 },
	{ 1, 7, 0x26, 1 }, { 1, 6, 0x0e, 0 }, { 1, 5, 0x0d, 5 },
	{ 3, 0, 5, 6 }, { 3, 1, 5, 6 }, { 3, 2, 5, 6 }, { 3, 3, 5, 6 },
	{ 3, 4, 5, 6 }, { 3, 5, 5, 6 }, { 3, 6, 5, 6 }, { 3, 7, 5, 6 },
	{ 0xff, 0xff, 0xff, 0xff }
} ;
static Uint8 dm10_tbl_2[9][4] = {
	{ 1, 0, 2, 1 }, { 1, 1, 6, 1 }, { 1, 2, 3, 2 }, { 1, 3, 4, 2 },
	{ 1, 7, 0x42, 1 }, { 1, 4, 0x09, 1 }, { 2, 1, 0x49, 4 }, { 1, 5, 0x0d, 5 },
	{ 0xff, 0xff, 0xff, 0xff }
} ;
static Uint8 *dm10_tbl[3] = { dm10_tbl_0[0], dm10_tbl_1[0], dm10_tbl_2[0] } ;
static VEC3 dm10_pos[7] = {
	{ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { -9.14f, 6.5f, -13.01f }, { 0.0f, 6.5f, -13.0f },
	{ 0.0f, 3.77f, -12.99f }, { 0.0f, -6.0f, -13.08f }, { 0.0f, 0.0f, -13.0f }
} ;
static Float dm10_cursor_y[4] = { 2.4f, -0.77f, -3.91f, 0.0f } ;	/* on the menu's lines */
static Float dm10_yesno_y[4] = { 3.12f, 0.13f, 0.0f, 0.0f } ;	/* on yes and no */
static Float dm10_port_x[4] = { -5.12f, -1.16f, 2.87f, 6.8f } ;	/* by the port's pair */
static Float dm10_port_y[2] = { 3.33f, 0.02f } ;	/* and its place in the pair */
static void ( *dm10_jmp[4] )( PLAYER_WORK *w ) = { dm10_flow, dm10_turn, dm10_cursor, dm10_port } ;

/****************************************************************/
/*			Screen n's pieces									*/
/****************************************************************/
void dm10_set( int n )
{
	Uint8 *p ;

	p = dm10_tbl[n] ;
	while( *p != 0xff ){
		dm10_sub_set( p[0], p[1], p[2], &dm10_pos[p[3]] ) ;
		p += 4 ;
	}
}

/****************************************************************/
/*			One piece											*/
/****************************************************************/
PLAYER_WORK *dm10_sub_set( int type, int n, int obj, VEC3 *pos )
{
	PLAYER_WORK *w ;

	if( !( w = work_get( 0, W_SYS, LIST_TOP ) ) ) return NULL ;
	w->disp_flag = 1 ;
	w->work_func = dm10_move ;
	w->sub_no = obj ;
	w->type = type ;
	w->sub_type = n ;
	w->obj = obj_dm10->obj[obj] ;
	w->pos = *pos ;
	w->scale = dm10_one ;
	w->fade = dm10_white ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	return w ;
}

void dm10_move( PLAYER_WORK *w )
{
	dm10_jmp[w->type]( w ) ;
}

/****************************************************************/
/*			A piece whose picture scrolls						*/
/****************************************************************/
void dm10_flow( PLAYER_WORK *w )
{
	Float v, u ;

	switch( w->routine_0 ){
	case 0 :
		w->routine_0 ++ ;
		w->timer0 = 0 ;
		w->timer1 = 0 ;
		obj_uv_clear( w->obj ) ;
		break ;
	case 1 :
		if( ++ w->timer0 >= 1000 ) w->timer0 = 0 ;
		if( ++ w->timer1 >= 1000 ) w->timer1 = 0 ;
		obj_uv_begin( obj_dm10->obj[w->sub_no + 1], w->obj ) ;
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
/*			A still piece; number 3 is turned					*/
/****************************************************************/
void dm10_turn( PLAYER_WORK *w )
{
	if( w->sub_type == 3 ){
		w->obj_flag = w->obj_flag | OBJ_ROT_Z ;
		w->rot[2] = DEG_ANG( 15 ) ;
	}
}

/****************************************************************/
/*			The cursor: glowing on the menu's line				*/
/*			(Sys->routine_1), or on yes and no (Sys->timer0,	*/
/*			bit 7 once chosen)									*/
/****************************************************************/
void dm10_cursor( PLAYER_WORK *w )
{
	w->timer1 += 9 ;
	if( w->timer1 >= 360 ) w->timer1 = 0 ;
	w->fade.trnsl = nlCos( DEG_ANG( w->timer1 ) ) * 0.5f + 0.5f ;
	if( !w->sub_type ) w->pos.y = dm10_cursor_y[Sys->routine_1] ;
	else {
		if( Sys->timer0 & 0x80 ) w->fade = dm10_white ;
		w->pos.y = dm10_yesno_y[Sys->timer0 & 0x7f] ;
	}
}

/****************************************************************/
/*			A port: grey with a card, black without, white,		*/
/*			larger and bobbing when the screen points at it		*/
/****************************************************************/
void dm10_port( PLAYER_WORK *w )
{
	int port ;

	port = vms_work.cursor_port ;	/* read here; the test below reads it again */
	switch( w->routine_0 ){
	case 0 :
		w->routine_0 ++ ;
		w->timer0 = 0 ;
		w->pos.x = dm10_port_x[w->sub_type / 2] ;
		w->pos.y = dm10_port_y[w->sub_type & 1] ;
	case 1 :
		if( vms_work.cursor_port == w->sub_type && vms_work.cursor_port != -1 && ( vms_work.card_bit & ( 1 << vms_work.cursor_port ) ) ){
			w->fade = dm10_white ;
			w->scale = dm10_big ;
			w->timer0 += 5 ;
			if( w->timer0 >= 360 ) w->timer0 = 0 ;
			w->pos.y = dm10_port_y[w->sub_type & 1] ;
			w->pos.y += nlSin( DEG_ANG( w->timer0 ) ) * 0.2f ;
			return ;
		}
		if( vms_work.card_bit & ( 1 << w->sub_type ) ) w->fade = dm10_grey ;
		else w->fade = dm10_none ;
		w->scale = dm10_one ;
		w->pos.y = dm10_port_y[w->sub_type & 1] ;
		w->timer0 = 0 ;
		break ;
	}
}

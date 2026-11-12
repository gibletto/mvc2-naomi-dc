/* DM0F, the card ports' picture: the eight ports, lit where a card is, the one the card screens
   point at (vms_work.cursor_port) larger and bobbing. */
#include "player.h"
#include "nl.h"
#include "dm.h"

PLAYER_WORK *dm0f_port_sub_set( int type, int port, int obj, VEC3 *pos ) ;
void dm0f_port_move( PLAYER_WORK *w ) ;
void dm0f_port_nop( PLAYER_WORK *w ) ;
void dm0f_port_card( PLAYER_WORK *w ) ;

static VEC3 dm0f_port_zero = { 0.0f, 0.0f, 0.0f } ;	/* unused */
static VEC3 dm0f_port_one = { 1.0f, 1.0f, 1.0f } ;
static VEC3 dm0f_port_big = { 1.2f, 1.2f, 1.2f } ;
static FADE_COL dm0f_port_none = { 1.0f, 0.0f, 0.0f, 0.0f } ;	/* no card */
static FADE_COL dm0f_port_white = { 1.0f, 1.0f, 1.0f, 1.0f } ;
static FADE_COL dm0f_port_grey = { 1.0f, 0.6f, 0.6f, 0.6f } ;	/* a card */

/* the parts: kind (0 still, 1 a port), port, model past the window's, position; 0xff at the end */
static Uint8 dm0f_port_tbl[11][4] = {
	{ 0, 0, 4, 0 }, { 0, 1, 6, 1 },
	{ 1, 0, 5, 2 }, { 1, 1, 5, 2 }, { 1, 2, 5, 2 }, { 1, 3, 5, 2 },
	{ 1, 4, 5, 2 }, { 1, 5, 5, 2 }, { 1, 6, 5, 2 }, { 1, 7, 5, 2 },
	{ 0xff, 0xff, 0xff, 0xff }
} ;
static VEC3 dm0f_port_pos[3] = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -13.0f } } ;
static Float dm0f_port_x[4] = { -5.12f, -1.16f, 2.87f, 6.8f } ;	/* by the port's pair */
static Float dm0f_port_y[2] = { 3.33f, 0.02f } ;	/* and its place in the pair */
static void ( *dm0f_port_jmp[2] )( PLAYER_WORK *w ) = { dm0f_port_nop, dm0f_port_card } ;

/****************************************************************/
/*			The picture											*/
/****************************************************************/
void dm0f_port_set( void )
{
	Uint8 *p ;

	p = dm0f_port_tbl[0] ;
	while( *p != 0xff ){
		dm0f_port_sub_set( p[0], p[1], p[2], &dm0f_port_pos[p[3]] ) ;
		p += 4 ;
	}
}

/****************************************************************/
/*			One part											*/
/****************************************************************/
PLAYER_WORK *dm0f_port_sub_set( int type, int port, int obj, VEC3 *pos )
{
	PLAYER_WORK *w ;

	if( !( w = work_get( 0, W_SYS, LIST_TOP ) ) ) return NULL ;
	w->disp_flag = 1 ;
	w->work_func = dm0f_port_move ;
	w->sub_no = obj ;
	w->type = type ;
	w->sub_type = port ;
	w->obj = obj_dm0f->obj[obj + 270] ;
	w->pos = *pos ;
	w->scale = dm0f_port_one ;
	w->fade = dm0f_port_white ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	return w ;
}

void dm0f_port_move( PLAYER_WORK *w )
{
	dm0f_port_jmp[w->type]( w ) ;
}

void dm0f_port_nop( PLAYER_WORK *w )
{
}

/****************************************************************/
/*			A port: grey with a card, black without, white,		*/
/*			larger and bobbing when the screen points at it		*/
/****************************************************************/
void dm0f_port_card( PLAYER_WORK *w )
{
	int port ;

	port = vms_work.cursor_port ;	/* read here; the test below reads it again */
	switch( w->routine_0 ){
	case 0 :
		w->routine_0 ++ ;
		w->timer0 = 0 ;
		w->pos.x = dm0f_port_x[w->sub_type / 2] ;
		w->pos.y = dm0f_port_y[w->sub_type & 1] ;
	case 1 :
		if( vms_work.cursor_port == w->sub_type && vms_work.cursor_port != -1 && ( vms_work.card_bit & ( 1 << vms_work.cursor_port ) ) ){
			w->fade = dm0f_port_white ;
			w->scale = dm0f_port_big ;
			w->timer0 += 5 ;
			if( w->timer0 >= 360 ) w->timer0 = 0 ;
			w->pos.y = dm0f_port_y[w->sub_type & 1] ;
			w->pos.y += nlSin( DEG_ANG( w->timer0 ) ) * 0.2f ;
			return ;
		}
		if( vms_work.card_bit & ( 1 << w->sub_type ) ) w->fade = dm0f_port_grey ;
		else w->fade = dm0f_port_none ;
		w->scale = dm0f_port_one ;
		w->pos.y = dm0f_port_y[w->sub_type & 1] ;
		w->timer0 = 0 ;
		break ;
	}
}

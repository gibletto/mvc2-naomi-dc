/* DM0C, the network menu: its plates and backdrop, the cursor, the scrolling and glowing
   pieces, and the window that opens over it. The USA build makes it but never shows it. */
#include "player.h"
#include "nl.h"
#include "dm.h"

extern void dm_obj_set( int no, OBJ_FILE *file ) ;

void net_move( PLAYER_WORK *w ) ;
void home_menu_net_obj( PLAYER_WORK *w ) ;

static VEC3 net_one = { 1.0f, 1.0f, 1.0f } ;
static VEC3 net_plate_pos[7] = {	/* the backdrop's two halves, the frame, the four plates */
	{ 0.0f, -6.73f, -14.0f }, { 0.0f, -6.73f, -14.1f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 5.02f, -14.0f },
	{ 0.0f, 2.51f, -14.0f }, { 0.0f, 0.06f, -14.0f }, { 0.0f, -2.53f, -14.0f }
} ;
static Sint16 net_flow_time[8] = { 100, 50, 20, 20, 100, 50, 20, 20 } ;
static Float net_flow_spd[8] = { 0.01f, 0.02f, 0.05f, 0.05f, 0.01f, 0.02f, 0.05f, 0.05f } ;
static Uint16 net_plate_obj[2][7] = {	/* in DM0C, in DM0D */
	{ 4, 5, 0x123, 0x12c, 0x12c, 0x12c, 0x12c }, { 4, 5, 1, 0x12c, 0x12c, 0x12c, 0x12c }
} ;
static VEC3 net_back_pos = { 0.0f, 0.0f, -35.0f } ;

/****************************************************************/
/*			A part of the menu: its kind, number and file		*/
/****************************************************************/
PLAYER_WORK *dm0c_net_set( Uint8 kind, Uint8 type, Uint8 sub_type )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_TOP ) ) ){
		w->disp_flag = 0 ;
		w->work_func = net_move ;
		w->sub_no = kind ;
		w->type = type ;
		w->sub_type = sub_type ;
		w->scale = net_one ;
		w->fade.trnsl = w->fade.r = w->fade.g = w->fade.b = 1.0f ;
		w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	}
	return w ;
}

/****************************************************************/
/*			The backdrop, the frame and the plates				*/
/****************************************************************/
void net_plate_init( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
	w->disp_flag = 1 ;
	if( !w->sub_type ) w->obj = obj_dm0c->obj[net_plate_obj[0][w->type]] ;
	else w->obj = obj_dm0d->obj[net_plate_obj[1][w->type]] ;
	w->pos = net_plate_pos[w->type] ;
	if( w->type < 2 ){
		w->scale.x = 1.12f ;
		w->scale.y = 1.12f ;
		w->scale.z = 1.0f ;
	} else w->scale = net_one ;
}

/****************************************************************/
/*			The chosen mode's name								*/
/****************************************************************/
void net_name_init( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
	w->disp_flag = 1 ;
	w->scale = net_one ;
	home_menu_net_obj( w ) ;
}

/****************************************************************/
/*			The cursor, on the chosen plate						*/
/****************************************************************/
void net_cursor_init( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
	w->disp_flag = 1 ;
	w->scale = net_one ;
	w->obj = obj_dm0c->obj[299] ;
	w->work2 = Sys->timer0 ;
}

/****************************************************************/
/*			A piece whose picture scrolls						*/
/****************************************************************/
void net_scroll_init( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
	w->disp_flag = 1 ;
	w->scale = net_one ;
	if( !w->sub_type ) w->obj = obj_dm0c->obj[297] ;
	else w->obj = obj_dm0d->obj[14] ;
	obj_uv_clear( w->obj ) ;
	w->timer0 = 0 ;
}

/****************************************************************/
/*			A piece that glows									*/
/****************************************************************/
void net_glow_init( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
	w->disp_flag = 1 ;
	w->scale = net_one ;
	if( !w->sub_type ) w->obj = obj_dm0c->obj[290] ;
	else w->obj = obj_dm0d->obj[0] ;
	w->timer1 = w->timer0 = 0 ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_FADE ;
	w->fade.trnsl = w->fade.r = w->fade.g = w->fade.b = 1.0f ;
}

/****************************************************************/
/*			A stream, flowing at its own speed					*/
/****************************************************************/
void net_flow_init( PLAYER_WORK *w )
{
	Sint8 type ;

	w->routine_0 ++ ;
	w->disp_flag = 1 ;
	w->scale = net_one ;
	if( !w->sub_type ) w->obj = obj_dm0c->obj[w->type * 2 + 306] ;
	else w->obj = obj_dm0d->obj[w->type * 2 + 123] ;
	obj_uv_clear( w->obj ) ;
	w->timer0 = 0 ;
	type = w->type ;
	w->timer1 = net_flow_time[type] ;
	w->spd.z = net_flow_spd[type] ;
}

/****************************************************************/
/*			The window: its frame, two arrows, the bar that		*/
/*			opens and the mark									*/
/****************************************************************/
void net_win_init( PLAYER_WORK *w )
{
	static Uint16 obj_tbl[5] = { 0x155, 0x153, 0x154, 0x156, 0x137 } ;
	static VEC3 pos_tbl[3] = { { 0.0f, 0.9f, -12.7f }, { 0.0f, -0.9f, -12.7f }, { 0.0f, 0.0f, -12.9f } } ;

	w->routine_0 ++ ;
	w->scale = net_one ;
	if( w->sub_type == 3 ){
		w->scale.x = 0.0f ;
		w->pos = pos_tbl[2] ;
		w->disp_flag = 1 ;
	} else if( w->sub_type == 1 || w->sub_type == 2 ){
		w->pos = pos_tbl[w->sub_type - 1] ;
		w->work2 = w->type ;
	}
	if( w->sub_type == 4 ) w->obj = obj_dm0c->obj[( w->type ^ 1 ) + 311] ;
	else w->obj = obj_dm0c->obj[obj_tbl[w->sub_type]] ;
	w->timer0 = 10 ;
}

/****************************************************************/
/*			A part's start, by its kind							*/
/****************************************************************/
void net_init( PLAYER_WORK *w )
{
	static void ( *net_init_jmp[7] )( PLAYER_WORK *w ) = {
		net_plate_init, net_name_init, net_cursor_init, net_scroll_init, net_glow_init, net_flow_init,
		net_win_init
	} ;

	net_init_jmp[w->sub_no]( w ) ;
}

/****************************************************************/
/*			The plates stay										*/
/****************************************************************/
void net_plate_main( PLAYER_WORK *w )
{
}

/****************************************************************/
/*			The chosen mode's name, shown while the menu is up	*/
/****************************************************************/
void home_menu_net_obj( PLAYER_WORK *w )
{
	if( Sys->scene_no < 2 && Sys->routine_0 < 2 ){
		w->disp_flag = 1 ;
		if( Sys->timer0 >= 2 ) w->obj = obj_dm0c->obj[Sys->timer0 + 301] ;
		else if( net_info_flag ) w->obj = obj_dm0c->obj[Sys->timer0 + 301] ;
		else w->obj = obj_dm0c->obj[313] ;
	} else w->disp_flag = 0 ;
}

/****************************************************************/
/*			The cursor follows the choice and blinks			*/
/****************************************************************/
void net_cursor_main( PLAYER_WORK *w )
{
	static Float pos_tbl[4] = { 5.02f, 2.51f, 0.06f, -2.53f } ;

	if( Sys->scene_no < 2 && Sys->routine_0 < 2 ){
		w->pos.y = pos_tbl[Sys->timer0] ;
		w->pos.z = -14.1f ;
		if( w->work2 != Sys->timer0 ){
			w->work2 = Sys->timer0 ;
			w->timer1 = 0 ;
		}
		w->timer1 += 9 ;
		if( w->timer1 >= 360 ) w->timer1 = 0 ;
		w->fade.trnsl = nlCos( DEG_ANG( w->timer1 ) ) * 0.5f + 0.5f ;
	} else {
		w->timer1 = 0 ;
		w->fade.trnsl = 1.0f ;
	}
}

/****************************************************************/
/*			The picture scrolls									*/
/****************************************************************/
void net_scroll_main( PLAYER_WORK *w )
{
	Float u, v ;

	w->timer0 ++ ;
	if( w->timer0 >= 25 ) w->timer0 = 0 ;
	if( !w->sub_type ) obj_uv_begin( obj_dm0c->obj[298], w->obj ) ;
	else obj_uv_begin( obj_dm0d->obj[15], w->obj ) ;
	while( !obj_uv_next() ){
		obj_uv_get( &u, &v ) ;
		v -= 1.0f - w->timer0 * 0.04f ;
		obj_uv_put( &u, &v ) ;
	}
}

/****************************************************************/
/*			The glow brightens and fades						*/
/****************************************************************/
void net_glow_main( PLAYER_WORK *w )
{
	w->timer0 ++ ;
	if( w->timer0 >= 80 ){
		w->timer0 = 0 ;
		w->timer1 ^= 1 ;
	}
	if( !w->timer1 ) w->fade.r -= 0.005f ;
	else w->fade.r += 0.005f ;
	w->fade.g = w->fade.b = w->fade.r ;
}

/****************************************************************/
/*			The stream flows									*/
/****************************************************************/
void net_flow_main( PLAYER_WORK *w )
{
	Sint8 type ;
	Float u, v ;

	w->timer0 ++ ;
	if( w->timer0 >= w->timer1 ){
		w->timer0 = 0 ;
		type = w->type ;
		w->timer1 = net_flow_time[type] ;
		w->spd.z = net_flow_spd[type] ;
	}
	if( !w->sub_type ) obj_uv_begin( obj_dm0c->obj[w->type * 2 + 305], w->obj ) ;
	else obj_uv_begin( obj_dm0d->obj[w->type * 2 + 124], w->obj ) ;
	while( !obj_uv_next() ){
		obj_uv_get( &u, &v ) ;
		v += 1.0f - w->spd.z * w->timer0 ;
		obj_uv_put( &u, &v ) ;
	}
}

/****************************************************************/
/*			The window opens, then its arrows blink; the		*/
/*			pieces follow the frame								*/
/****************************************************************/
void net_win_main( PLAYER_WORK *w )
{
	static VEC3 pos_tbl[2] = { { 0.0f, 0.9f, -12.71f }, { 0.0f, -0.9f, -12.71f } } ;
	PLAYER_WORK *mw ;

	if( !w->routine_1 ){
		if( -- w->timer0 < 0 ){
			if( w->sub_type == 3 ) w->scale.x = 1.0f ;
			else if( !w->sub_type ) Sys->routine_0 ++ ;
			w->disp_flag = 1 ;
			w->routine_1 ++ ;
		} else if( w->sub_type == 3 ) w->scale.x += 0.1f ;
	} else {
		if( !w->sub_type ){
			w->pos = pos_tbl[w->type] ;
			if( w->type != w->work2 ){
				w->work2 = w->type ;
				w->timer1 = 0 ;
			}
			w->timer1 += 9 ;
			if( w->timer1 >= 360 ) w->timer1 = 0 ;
			w->fade.trnsl = nlCos( DEG_ANG( w->timer1 ) ) * 0.5f + 0.5f ;
		}
		if( ( mw = w->ef_w ) ) w->routine_0 = mw->routine_0 ;
	}
}

/****************************************************************/
/*			A part each frame, by its kind						*/
/****************************************************************/
void net_main( PLAYER_WORK *w )
{
	static void ( *net_main_jmp[7] )( PLAYER_WORK *w ) = {
		net_plate_main, home_menu_net_obj, net_cursor_main, net_scroll_main, net_glow_main,
		net_flow_main, net_win_main
	} ;

	net_main_jmp[w->sub_no]( w ) ;
}

/****************************************************************/
/*			On to the next step									*/
/****************************************************************/
void net_next( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
}

/****************************************************************/
/*			The part is done									*/
/****************************************************************/
void net_free( PLAYER_WORK *w )
{
	work_free( w ) ;
}

/****************************************************************/
/*			A part of the menu, each frame						*/
/****************************************************************/
void net_move( PLAYER_WORK *w )
{
	static void ( *net_jmp[4] )( PLAYER_WORK *w ) = { net_init, net_main, net_next, net_free } ;

	net_jmp[w->routine_0]( w ) ;
}

void net_back_set( Uint8 file ) ;

/****************************************************************/
/*			The network menu's parts							*/
/****************************************************************/
void net_menu_set( void )
{
	dm_obj_set( 0x125, obj_dm0c ) ;
	dm_obj_set( 0x126, obj_dm0c ) ;
	dm_obj_set( 0x127, obj_dm0c ) ;
	dm_obj_set( 0x128, obj_dm0c ) ;
	dm0c_net_set( 0, 0, 0 ) ;
	dm0c_net_set( 0, 1, 0 ) ;
	dm0c_net_set( 0, 3, 0 ) ;
	dm0c_net_set( 0, 4, 0 ) ;
	dm0c_net_set( 0, 5, 0 ) ;
	dm0c_net_set( 0, 6, 0 ) ;
	dm0c_net_set( 1, 0, 0 ) ;
	dm0c_net_set( 2, 0, 0 ) ;
	net_back_set( 0 ) ;
}

/****************************************************************/
/*			The frame, the glows and the streams, from DM0C		*/
/*			or DM0D												*/
/****************************************************************/
void net_back_set( Uint8 file )
{
	dm0c_net_set( 0, 2, file ) ;
	dm0c_net_set( 3, 0, file ) ;
	dm0c_net_set( 4, 0, file ) ;
	dm0c_net_set( 5, 0, file ) ;
	dm0c_net_set( 5, 1, file ) ;
	dm0c_net_set( 5, 2, file ) ;
}

/****************************************************************/
/*			The window and its four pieces						*/
/****************************************************************/
PLAYER_WORK *net_win_set( int type )
{
	PLAYER_WORK *mw, *w ;
	int i ;

	mw = dm0c_net_set( 6, 0, 0 ) ;
	mw->ef_w = NULL ;
	for( i = 1 ; i < 5 ; i ++ ){
		w = work_get( mw, W_SYS, LIST_BEHIND ) ;
		w->disp_flag = 0 ;
		w->work_func = net_move ;
		w->sub_no = 6 ;
		w->type = type ;
		w->sub_type = i ;
		w->scale = net_one ;
		w->fade.trnsl = w->fade.r = w->fade.g = w->fade.b = 1.0f ;
		w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
		w->ef_w = mw ;
	}
	return mw ;
}

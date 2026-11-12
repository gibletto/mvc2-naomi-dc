/* DM0F, SECRET FACTOR's top menu: its five items, lit by Sys->routine_1, each on a plate with a
   shadow and a glow, and the cursor that bobs beside the fourth. */
#include "player.h"
#include "nl.h"
#include "dm.h"

void dm0f_menu_move( PLAYER_WORK *w ) ;
void dm0f_menu_plate_set( PLAYER_WORK *w, int n ) ;
void dm0f_menu_nop( PLAYER_WORK *w ) ;
void dm0f_menu_shadow_set( PLAYER_WORK *w ) ;
void dm0f_menu_nop2( PLAYER_WORK *w ) ;
void dm0f_menu_glow_set( PLAYER_WORK *w, int n ) ;
void dm0f_menu_glow( PLAYER_WORK *w ) ;
void dm0f_menu_cursor_set( void ) ;
void dm0f_menu_cursor( PLAYER_WORK *w ) ;

static Float dm0f_menu_y[5] = { 2.7f, 0.9f, -0.9f, -3.2f, -6.0f } ;
static VEC3 dm0f_menu_scale[5] = {	/* each item's plate and glow */
	{ 1.0f, 0.3f, 1.0f }, { 1.0f, 0.3f, 1.0f }, { 1.0f, 0.3f, 1.0f }, { 1.2f, 0.5f, 1.0f }, { 0.6f, 0.3f, 1.0f }
} ;
static Uint8 dm0f_menu_obj[5] = { 130, 131, 132, 133, 134 } ;

/****************************************************************/
/*			The top menu's items								*/
/****************************************************************/
void dm0f_menu_set( void )
{
	PLAYER_WORK *w ;
	int i ;

	for( i = 0 ; i < 5 ; i ++ ){
		if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) break ;
		w->disp_flag = 1 ;
		w->work_func = dm0f_menu_move ;
		w->obj = obj_dm0f->obj[dm0f_menu_obj[i]] ;
		w->obj_flag = OBJ_POS | OBJ_FADE ;
		w->pos.x = -8.8f ;
		w->pos.y = dm0f_menu_y[i] ;
		w->pos.z = -12.99f ;
		w->type = i ;
		dm0f_menu_plate_set( w, i ) ;
		dm0f_menu_glow_set( w, i ) ;
		if( i == 3 ) dm0f_menu_cursor_set() ;
	}
}

/****************************************************************/
/*			An item: bright while it is chosen					*/
/****************************************************************/
void dm0f_menu_move( PLAYER_WORK *w )
{
	FADE_COL *f ;
	Float c ;

	f = &w->fade ;
	if( Sys->routine_1 == w->type ) c = 1.0f ;
	else c = 0.8f ;
	f->r = c ;
	f->g = c ;
	f->b = c ;
}

/****************************************************************/
/*			The plate behind an item, and its shadow			*/
/****************************************************************/
void dm0f_menu_plate_set( PLAYER_WORK *w, int n )
{
	PLAYER_WORK *w2 ;

	if( ( w2 = work_get( 0, W_SYS, LIST_END ) ) ){
		w2->disp_flag = 1 ;
		w2->work_func = dm0f_menu_nop ;
		w2->obj = obj_dm0f->obj[4] ;
		w2->obj_flag = OBJ_POS | OBJ_SCALE ;
		w2->pos.z = -0.1f ;
		w2->scale = dm0f_menu_scale[n] ;
		w2->base_mtx = &w->obj_mtx ;
		dm0f_menu_shadow_set( w2 ) ;
	}
}

void dm0f_menu_nop( PLAYER_WORK *w )
{
}

void dm0f_menu_shadow_set( PLAYER_WORK *w )
{
	PLAYER_WORK *w2 ;

	if( ( w2 = work_get( 0, W_SYS, LIST_END ) ) ){
		w2->disp_flag = 1 ;
		w2->work_func = dm0f_menu_nop2 ;
		w2->obj = obj_dm0f->obj[3] ;
		w2->obj_flag = OBJ_POS ;
		w2->pos.z = -0.1f ;
		w2->base_mtx = &w->obj_mtx ;
	}
}

void dm0f_menu_nop2( PLAYER_WORK *w )
{
}

/****************************************************************/
/*			The glow over an item, shown while it is chosen		*/
/****************************************************************/
void dm0f_menu_glow_set( PLAYER_WORK *w, int n )
{
	PLAYER_WORK *w2 ;

	if( ( w2 = work_get( 0, W_SYS, LIST_END ) ) ){
		w2->work_func = dm0f_menu_glow ;
		w2->obj = obj_dm0f->obj[188] ;
		w2->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL ;
		w2->pos.z = -0.01f ;
		w2->scale = dm0f_menu_scale[n] ;
		w2->base_mtx = &w->obj_mtx ;
		w2->type = n ;
	}
}

void dm0f_menu_glow( PLAYER_WORK *w )
{
	if( Sys->routine_1 == w->type ){
		w->disp_flag = 1 ;
		w->timer1 += 9 ;
		if( w->timer1 >= 360 ) w->timer1 = 0 ;
		w->fade.trnsl = nlCos( DEG_ANG( w->timer1 ) ) * 0.5f + 0.5f ;
	} else {
		w->disp_flag = 0 ;
		w->timer1 = 0 ;
	}
}

/****************************************************************/
/*			The cursor beside the fourth item					*/
/****************************************************************/
void dm0f_menu_cursor_set( void )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_END ) ) ){
		w->disp_flag = 1 ;
		w->work_func = dm0f_menu_cursor ;
		w->obj = obj_dm0f->obj[128] ;
		w->obj_flag = OBJ_POS | OBJ_FADE ;
		w->pos.x = -10.8f ;
		w->pos.z = -12.99f ;
	}
}

void dm0f_menu_cursor( PLAYER_WORK *w )
{
	FADE_COL *f ;

	f = &w->fade ;
	if( Sys->routine_1 == 3 ){
		f->r = f->g = f->b = 1.0f ;
		w->timer0 += 5 ;
		if( w->timer0 >= 360 ) w->timer0 = 0 ;
		w->pos.y = dm0f_menu_y[3] + nlSin( DEG_ANG( w->timer0 ) ) * 0.2f ;
	} else {
		f->r = f->g = f->b = 0.8f ;
		w->timer0 = 0 ;
		w->pos.y = dm0f_menu_y[3] ;
	}
}

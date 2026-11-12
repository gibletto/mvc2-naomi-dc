/* DM0F, mode 8's menu: its three sheets' items, the one Sys->routine_1 points at glowing, each
   with its frame, its shadow and its name drawn on it. */
#include "player.h"
#include "nl.h"
#include "dm.h"

void dm0f_mode8_move( PLAYER_WORK *w ) ;
void dm0f_mode8_sub_set( PLAYER_WORK *wk, Float z, int n ) ;
void dm0f_mode8_nop( PLAYER_WORK *w ) ;

static VEC3 dm0f_mode8_pos[3] = {
	{ 2.39f, 2.62f, -12.99f }, { 0.41f, -0.66f, -12.99f }, { -1.57f, -3.94f, -12.99f }
} ;
static int dm0f_mode8_name[3] = { 12, 13, 14 } ;	/* each item's name */

/****************************************************************/
/*			The items											*/
/****************************************************************/
void dm0f_mode8_set( void )
{
	PLAYER_WORK *w ;
	int i ;

	for( i = 0 ; i < 3 ; i ++ ){
		if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return ;
		w->disp_flag = 1 ;
		w->work_func = dm0f_mode8_move ;
		w->obj = obj_dm0f->obj[11] ;
		w->obj_flag = OBJ_POS | OBJ_TRNSL ;
		w->pos = dm0f_mode8_pos[i] ;
		w->type = i ;
		dm0f_mode8_sub_set( w, 0.01f, 9 ) ;
		dm0f_mode8_sub_set( w, -0.01f, 10 ) ;
		dm0f_mode8_sub_set( w, 0.01f, dm0f_mode8_name[i] ) ;
	}
}

/****************************************************************/
/*			An item: shown and glowing under the cursor			*/
/****************************************************************/
void dm0f_mode8_move( PLAYER_WORK *w )
{
	if( Sys->routine_1 == w->type ){
		w->disp_flag = 1 ;
		w->timer0 += 9 ;
		if( w->timer0 >= 360 ) w->timer0 = 0 ;
		w->fade.trnsl = nlCos( DEG_ANG( w->timer0 ) ) * 0.5f + 0.5f ;
	} else {
		w->disp_flag = 0 ;
		w->timer0 = 0 ;
	}
}

/****************************************************************/
/*			A model drawn on an item, z in front of it			*/
/****************************************************************/
void dm0f_mode8_sub_set( PLAYER_WORK *wk, Float z, int n )
{
	PLAYER_WORK *w ;

	if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = dm0f_mode8_nop ;
	w->obj = obj_dm0f->obj[n] ;
	w->obj_flag = OBJ_POS ;
	w->pos.z = z ;
	w->base_mtx = &wk->obj_mtx ;
	if( n >= 12 ) w->pos.y = -0.06f ;
}

void dm0f_mode8_nop( PLAYER_WORK *w )
{
}

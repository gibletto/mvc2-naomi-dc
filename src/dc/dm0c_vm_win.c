/* DM0C, the memory card window's frame: its texture scrolled, and its parts gone when the window's
   state moves on. */
#include "player.h"
#include "dm.h"

void vm_win_move( PLAYER_WORK *w ) ;
void vm_win_wall( PLAYER_WORK *w ) ;
void vm_win_wait( PLAYER_WORK *w ) ;
void vm_win_wait2( PLAYER_WORK *w ) ;
void vm_win_end( PLAYER_WORK *w ) ;

static Uint16 vm_win_obj[4] = { 0, 316, 317, 0 } ;	/* the frame's models, then the scroll's own */
static VEC3 vm_win_zero = { 0.0f, 0.0f, 0.0f } ;
static VEC3 vm_win_one = { 1.0f, 1.0f, 1.0f } ;
static FADE_COL vm_win_fade = { 1.0f, 1.0f, 1.0f, 1.0f } ;
static Float vm_win_col[3] = { 0.95f, 0.55f, 0.65f } ;
static void ( *vm_win_jmp[3] )( PLAYER_WORK *w ) = { vm_win_wall, vm_win_wait, vm_win_wait2 } ;

/****************************************************************/
/*			A part of the window's frame						*/
/****************************************************************/
void dm0c_vm_win_set( int n )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_END ) ) ){
		w->disp_flag = 1 ;
		w->work_func = vm_win_move ;
		w->sub_no = n ;
		w->obj = obj_dm0c->obj[vm_win_obj[n]] ;
		w->scale = vm_win_one ;
		w->fade = vm_win_fade ;
		w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	}
}

/****************************************************************/
/*			A part each frame, by its kind						*/
/****************************************************************/
void vm_win_move( PLAYER_WORK *w )
{
	vm_win_jmp[w->sub_no]( w ) ;
}

/****************************************************************/
/*			The frame's back: its colour, its texture scrolled	*/
/****************************************************************/
void vm_win_wall( PLAYER_WORK *w )
{
	Float u, v ;

	switch( w->routine_0 ){
	case 0 :
		w->routine_0 ++ ;
		w->timer1 = w->timer0 = 0 ;
		w->fade.r = vm_win_col[0] ;
		w->fade.g = vm_win_col[1] ;
		w->fade.b = vm_win_col[2] ;
		obj_uv_clear( w->obj ) ;
		break ;
	case 1 :
		if( ++ w->timer0 >= 2000 ) w->timer0 = 0 ;
		if( ++ w->timer1 >= 2000 ) w->timer1 = 0 ;
		obj_uv_begin( obj_dm0c->obj[vm_win_obj[w->sub_no + 1]], w->obj ) ;
		while( !obj_uv_next() ){
			obj_uv_get( &u, &v ) ;
			u += 1.0f - w->timer0 * 0.001f ;
			v += 1.0f - w->timer1 * 0.0005f ;
			obj_uv_put( &u, &v ) ;
		}
		break ;
	}
}

/****************************************************************/
/*			A part that goes when the window's state moves on	*/
/****************************************************************/
void vm_win_wait( PLAYER_WORK *w )
{
	if( w->routine_0 ){
		vm_win_end( w ) ;
		return ;
	}
	if( vms_work.vm_msg[1] != w->sub_no ){
		w->routine_0 ++ ;
		w->disp_flag = 0 ;
	}
}

void vm_win_wait2( PLAYER_WORK *w )
{
	if( w->routine_0 ){
		vm_win_end( w ) ;
		return ;
	}
	if( vms_work.vm_msg[1] != w->sub_no ){
		w->routine_0 ++ ;
		w->disp_flag = 0 ;
	}
}

void vm_win_end( PLAYER_WORK *w )
{
	work_free( w ) ;
}

/* DM10, the memory card menu message's yes/no: a window that opens, its two answers and the cursor on the
   one in vms_work.vm_msg[1] (bit 7 once it is chosen, 0xff to close). */
#include "player.h"
#include "nl.h"
#include "dm.h"

PLAYER_WORK *dm10_vm_sel_sub_set( int n, int kind ) ;
void dm10_vm_sel_move( PLAYER_WORK *w ) ;
void dm10_vm_sel_ans( PLAYER_WORK *w ) ;
void dm10_vm_sel_cursor( PLAYER_WORK *w ) ;
void dm10_vm_sel_win( PLAYER_WORK *w ) ;

static VEC3 dm10_vm_sel_one = { 1.0f, 1.0f, 1.0f } ;
static VEC3 dm10_vm_sel_shut = { 0.0f, 1.0f, 1.0f } ;	/* the window's scale before it opens */
static FADE_COL dm10_vm_sel_fade = { 1.0f, 1.0f, 1.0f, 1.0f } ;
static VEC3 dm10_vm_sel_pos[4] = {	/* yes, no, the cursor, the window */
	{ 0.0f, 0.9f, -12.0f }, { 0.0f, -0.9f, -12.0f }, { 0.0f, -0.9f, -12.1f }, { 0.0f, 0.0f, -12.3f }
} ;
static void ( *dm10_vm_sel_jmp[4] )( PLAYER_WORK *w ) = {
	dm10_vm_sel_ans, dm10_vm_sel_ans, dm10_vm_sel_cursor, dm10_vm_sel_win
} ;

/****************************************************************/
/*			The yes/no for message n: its window first			*/
/****************************************************************/
void dm10_vm_yesno_set( int n )
{
	PLAYER_WORK *w ;

	if( ( w = dm10_vm_sel_sub_set( n, 3 ) ) ) w->scale = dm10_vm_sel_shut ;
}

/****************************************************************/
/*			One part: the answers, the cursor or the window		*/
/****************************************************************/
PLAYER_WORK *dm10_vm_sel_sub_set( int n, int kind )
{
	PLAYER_WORK *w ;

	if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return NULL ;
	w->disp_flag = 1 ;
	w->work_func = dm10_vm_sel_move ;
	w->pos = dm10_vm_sel_pos[kind] ;
	w->obj = obj_dm10->obj[kind + 67] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->scale = dm10_vm_sel_one ;
	w->fade = dm10_vm_sel_fade ;
	w->sub_no = n ;
	w->type = kind ;
	return w ;
}

void dm10_vm_sel_move( PLAYER_WORK *w )
{
	dm10_vm_sel_jmp[w->type]( w ) ;
}

/****************************************************************/
/*			An answer: shown until it is chosen, then 30		*/
/*			frames more											*/
/****************************************************************/
void dm10_vm_sel_ans( PLAYER_WORK *w )
{
	if( vms_work.vm_msg[0] != w->sub_no ){
		w->routine_0 = 2 ;
		w->disp_flag = 0 ;
	}
	switch( w->routine_0 ){
	case 0 :
		if( vms_work.vm_msg[1] == 0xff ){
			w->routine_0 = 2 ;
			w->disp_flag = 0 ;
		} else if( vms_work.vm_msg[1] & 0x80 ){
			w->routine_0 ++ ;
			w->timer0 = 30 ;
		}
		break ;
	case 1 :
		if( !-- w->timer0 ){
			w->routine_0 = 2 ;
			w->disp_flag = 0 ;
		}
		break ;
	case 2 :
		work_free( w ) ;
		break ;
	}
}

/****************************************************************/
/*			The cursor: it glows on the answer, and blinks		*/
/*			once one is chosen									*/
/****************************************************************/
void dm10_vm_sel_cursor( PLAYER_WORK *w )
{
	if( vms_work.vm_msg[0] != w->sub_no ){
		w->routine_0 = 2 ;
		w->disp_flag = 0 ;
	}
	switch( w->routine_0 ){
	case 0 :
		w->timer1 += 9 ;
		if( w->timer1 >= 360 ) w->timer1 = 0 ;
		w->fade.trnsl = nlCos( DEG_ANG( w->timer1 ) ) * 0.5f + 0.5f ;
		w->pos.x = 0.0f ;
		w->pos.z = -12.1f ;
		w->spd.y = w->pos.y ;
		if( w->spd.y != ( w->pos.y = !( vms_work.vm_msg[1] & 0x7f ) ? 0.9f : -0.9f ) ) w->timer1 = 0 ;
		if( vms_work.vm_msg[1] == 0xff ){
			w->routine_0 = 2 ;
			w->disp_flag = 0 ;
		} else if( vms_work.vm_msg[1] & 0x80 ){
			w->routine_0 ++ ;
			w->fade = dm10_vm_sel_fade ;
			w->timer0 = 0 ;
			w->timer1 = 30 ;
		}
		break ;
	case 1 :
		if( !( ++ w->timer0 & 3 ) ) w->disp_flag ^= 1 ;
		if( !-- w->timer1 ){
			w->routine_0 ++ ;
			w->disp_flag = 0 ;
		}
		break ;
	case 2 :
		work_free( w ) ;
		break ;
	}
}

/****************************************************************/
/*			The window: it opens and sets the answers and the	*/
/*			cursor, and closes 30 frames after the choice		*/
/****************************************************************/
void dm10_vm_sel_win( PLAYER_WORK *w )
{
	if( vms_work.vm_msg[0] != w->sub_no ){
		w->routine_0 = 4 ;
		w->disp_flag = 0 ;
	}
	switch( w->routine_0 ){
	case 0 :
		w->scale.x += 0.2f ;
		if( w->scale.x >= 1.0f ){
			w->routine_0 ++ ;
			w->scale.x = 1.0f ;
			dm10_vm_sel_sub_set( w->sub_no, 0 ) ;
			dm10_vm_sel_sub_set( w->sub_no, 1 ) ;
			dm10_vm_sel_sub_set( w->sub_no, 2 ) ;
		}
		break ;
	case 1 :
		if( vms_work.vm_msg[1] == 0xff ) w->routine_0 = 3 ;
		else if( vms_work.vm_msg[1] & 0x80 ){
			w->routine_0 = 2 ;
			w->timer0 = 30 ;
		}
		break ;
	case 2 :
		if( -- w->timer0 ) break ;
		w->routine_0 ++ ;
	case 3 :
		w->scale.x -= 0.2f ;
		if( w->scale.x <= 0.0f ){
			w->routine_0 ++ ;
			w->disp_flag = 0 ;
		}
		break ;
	case 4 :
		vms_work.vm_msg[1] = 0 ;
		work_free( w ) ;
		break ;
	}
}

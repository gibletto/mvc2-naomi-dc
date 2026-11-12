/* DM0F, the memory card window's message (DM0C's, with the card's date and the yes/no): its lines
   by vms_work.vm_msg[0], gone when the message changes. */
#include "player.h"
#include "nl.h"

#define VM_MSG_OBJ	270	/* the window's first model in obj_dm0f */

extern void dm0f_vm_yesno_set( int n ) ;

PLAYER_WORK *dm0f_vm_msg_sub_set( int n, int code, VEC3 *pos ) ;
void dm0f_vm_msg_move( PLAYER_WORK *w ) ;
void dm0f_vm_msg_init( PLAYER_WORK *w ) ;
void dm0f_vm_msg_wait( PLAYER_WORK *w ) ;
void dm0f_vm_msg_end( PLAYER_WORK *w ) ;

/* each message's lines: a model past the window's own in the low byte, what to draw in the high
   byte, -1 at the end */
static Sint16 dm0f_vm_msg_0[2] = { 0x00, -1 } ;
static Sint16 dm0f_vm_msg_1[2] = { 0x0a, -1 } ;
static Sint16 dm0f_vm_msg_2[2] = { 0x0b, -1 } ;
static Sint16 dm0f_vm_msg_3[2] = { 0x30c, -1 } ;
static Sint16 dm0f_vm_msg_4[2] = { 0x30d, -1 } ;
static Sint16 dm0f_vm_msg_5[3] = { 0x0e, 0x10f, -1 } ;
static Sint16 dm0f_vm_msg_6[2] = { 0x10, -1 } ;
static Sint16 dm0f_vm_msg_7[2] = { 0x11, -1 } ;
static Sint16 dm0f_vm_msg_8[2] = { 0x12, -1 } ;
static Sint16 dm0f_vm_msg_9[2] = { 0x313, -1 } ;
static Sint16 dm0f_vm_msg_10[2] = { 0x314, -1 } ;
static Sint16 dm0f_vm_msg_11[2] = { 0x15, -1 } ;
static Sint16 dm0f_vm_msg_12[2] = { 0x16, -1 } ;
static Sint16 dm0f_vm_msg_13[2] = { 0x17, -1 } ;
static Sint16 dm0f_vm_msg_14[2] = { 0x18, -1 } ;
static Sint16 dm0f_vm_msg_15[2] = { 0x19, -1 } ;
static Sint16 dm0f_vm_msg_16[2] = { 0x1a, -1 } ;
static Sint16 dm0f_vm_msg_17[2] = { 0x1b, -1 } ;
static Sint16 dm0f_vm_msg_18[2] = { 0x41c, -1 } ;
static Sint16 dm0f_vm_msg_19[2] = { 0x1d, -1 } ;
static Sint16 dm0f_vm_msg_20[2] = { 0x08, -1 } ;
static Sint16 dm0f_vm_msg_21[2] = { 0x09, -1 } ;
static Sint16 dm0f_vm_msg_22[2] = { 0x07, -1 } ;
static Sint16 dm0f_vm_msg_23[2] = { 0x4e, -1 } ;
static Sint16 *dm0f_vm_msg_tbl[24] = {
	dm0f_vm_msg_0, dm0f_vm_msg_1, dm0f_vm_msg_2, dm0f_vm_msg_3, dm0f_vm_msg_4, dm0f_vm_msg_5,
	dm0f_vm_msg_6, dm0f_vm_msg_7, dm0f_vm_msg_8, dm0f_vm_msg_9, dm0f_vm_msg_10, dm0f_vm_msg_11,
	dm0f_vm_msg_12, dm0f_vm_msg_13, dm0f_vm_msg_14, dm0f_vm_msg_15, dm0f_vm_msg_16, dm0f_vm_msg_17,
	dm0f_vm_msg_18, dm0f_vm_msg_19, dm0f_vm_msg_20, dm0f_vm_msg_21, dm0f_vm_msg_22, dm0f_vm_msg_23
} ;

static VEC3 dm0f_vm_msg_pos[20] = {	/* the date's twelve digits, then what the other lines draw in */
	{ -5.8f, -6.7f, -12.7f }, { -5.1f, -6.7f, -12.7f },
	{ -4.4f, -6.7f, -12.7f }, { -3.7f, -6.7f, -12.7f },
	{ -1.9f, -6.7f, -12.7f }, { -1.2f, -6.7f, -12.7f },
	{ 0.4f, -6.7f, -12.7f }, { 1.1f, -6.7f, -12.7f },
	{ 2.5f, -6.7f, -12.7f }, { 3.2f, -6.7f, -12.7f },
	{ 4.4f, -6.7f, -12.7f }, { 5.1f, -6.7f, -12.7f },
	{ 2.75f, -4.6f, -12.7f }, { 0.5f, -6.0f, -12.7f },
	{ 1.2f, -6.0f, -12.7f }, { 0.5f, -5.3f, -12.7f },
	{ 1.2f, -5.3f, -12.7f }, { 1.9f, -7.4f, -12.7f },
	{ 2.7f, -7.4f, -12.7f }, { 2.15f, -5.3f, -12.7f }
} ;
static Uint8 dm0f_vm_msg_idx[7] = { 0, 0, 12, 0, 17, 19, 15 } ;	/* the first of dm0f_vm_msg_pos by what is drawn */
static void ( *dm0f_vm_msg_jmp[3] )( PLAYER_WORK *w ) = { dm0f_vm_msg_init, dm0f_vm_msg_wait, dm0f_vm_msg_end } ;

/****************************************************************/
/*			The card message's lines							*/
/****************************************************************/
void dm0f_vm_msg_set( void )
{
	Sint16 *p ;
	VEC3 pos ;
	int n ;

	pos.x = 0.0f ;
	pos.y = 0.0f ;
	pos.z = 0.0f ;
	n = vms_work.vm_msg[0] ;
	p = dm0f_vm_msg_tbl[n] ;
	while( *p != -1 ) dm0f_vm_msg_sub_set( n, *p ++, &pos ) ;
}

/****************************************************************/
/*			A line of the message, or a character drawn in		*/
/****************************************************************/
PLAYER_WORK *dm0f_vm_msg_sub_set( int n, int code, VEC3 *pos )
{
	PLAYER_WORK *w ;

	if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return NULL ;
	w->disp_flag = 1 ;
	w->work_func = dm0f_vm_msg_move ;
	w->obj = obj_dm0f->obj[( code & ~0xff00 ) + VM_MSG_OBJ] ;
	w->obj_flag = OBJ_POS ;
	w->pos = *pos ;
	w->type = n ;
	w->sub_type = 0 ;
	w->timer0 = code & 0xff00 ;
	return w ;
}

/****************************************************************/
/*			A line each frame									*/
/****************************************************************/
void dm0f_vm_msg_move( PLAYER_WORK *w )
{
	dm0f_vm_msg_jmp[w->routine_0]( w ) ;
}

/****************************************************************/
/*			What the line draws in: the date, the port, the		*/
/*			yes/no												*/
/****************************************************************/
void dm0f_vm_msg_init( PLAYER_WORK *w )
{
	PLAYER_WORK *w2 ;
	VEC3 *pos ;
	VMS_DATE date ;
	int i ;

	w->routine_0 ++ ;
	pos = &dm0f_vm_msg_pos[dm0f_vm_msg_idx[w->timer0 >> 8]] ;
	switch( w->timer0 ){
	case 0x100 :
		for( i = 0 ; i < 12 ; i ++ ){
			if( !( w2 = dm0f_vm_msg_sub_set( w->type, 30, pos ++ ) ) ) return ;
			w2->sub_type = i + 1 ;
			w2->timer0 = 0 ;
		}
		break ;
	case 0x500 :
	case 0x200 :
		dm0f_vm_msg_sub_set( w->type, 35, pos ++ ) ;
		if( w->timer0 == 0x500 ) break ;
	case 0x600 :
		dm0f_vm_msg_sub_set( w->type, 36, pos ++ ) ;
		dm0f_vm_msg_sub_set( w->type, 34, pos ) ;
		break ;
	case 0x400 :
		dm0f_vm_msg_sub_set( w->type, vms_work.cursor_port / 2 + 40, pos ++ ) ;
		dm0f_vm_msg_sub_set( w->type, ( vms_work.cursor_port & 1 ) + 31, pos ) ;
	case 0x300 :
		dm0f_vm_yesno_set( w->type ) ;
		break ;
	}
	dm0f_vm_msg_wait( w ) ;
}

/****************************************************************/
/*			Shown until the message changes; a digit of the		*/
/*			card's date (sub_type), without a leading 0			*/
/****************************************************************/
void dm0f_vm_msg_wait( PLAYER_WORK *w )
{
	VMS_DATE date ;
	int n ;

	if( vms_work.vm_msg[0] != w->type ){
		w->routine_0 ++ ;
		w->disp_flag = 0 ;
		return ;
	}
	if( w->timer0 == 0x100 || w->sub_type ){
		w->disp_flag = 1 ;
		if( !vms_date_get( vms_work.cursor_port, &date ) ){
			w->disp_flag = 0 ;
			return ;
		}
	}
	if( !w->sub_type ) return ;
	switch( w->sub_type ){
	case 1 :
		n = date.year / 1000 ;
		break ;
	case 2 :
		n = date.year % 1000 / 100 ;
		break ;
	case 3 :
		n = date.year % 1000 % 100 / 10 ;
		break ;
	case 4 :
		n = date.year % 1000 % 100 % 10 ;
		break ;
	case 5 :
		w->disp_flag = 1 ;
		if( !( n = date.month / 10 ) ){
			w->disp_flag = 0 ;
			return ;
		}
		break ;
	case 6 :
		n = date.month % 10 ;
		break ;
	case 7 :
		w->disp_flag = 1 ;
		if( !( n = date.day / 10 ) ){
			w->disp_flag = 0 ;
			return ;
		}
		break ;
	case 8 :
		n = date.day % 10 ;
		break ;
	case 9 :
		n = date.hour / 10 ;
		break ;
	case 10 :
		n = date.hour % 10 ;
		break ;
	case 11 :
		n = date.minute / 10 ;
		break ;
	case 12 :
		n = date.minute % 10 ;
		break ;
	}
	w->obj = obj_dm0f->obj[n + 30 + VM_MSG_OBJ] ;
}

void dm0f_vm_msg_end( PLAYER_WORK *w )
{
	work_free( w ) ;
}

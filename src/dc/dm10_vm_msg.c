/* DM10, the memory card menu's message (DM0F's, with a line's height from a table): its lines by
   vms_work.vm_msg[0], gone when the message changes. */
#include "player.h"
#include "nl.h"

extern void dm10_vm_yesno_set( int n ) ;

PLAYER_WORK *dm10_vm_msg_sub_set( int n, int code, VEC3 *pos ) ;
void dm10_vm_msg_move( PLAYER_WORK *w ) ;
void dm10_vm_msg_init( PLAYER_WORK *w ) ;
void dm10_vm_msg_wait( PLAYER_WORK *w ) ;
void dm10_vm_msg_end( PLAYER_WORK *w ) ;

/* each message's lines: a model of obj_dm10 in the low byte, what to draw in the high byte, -1 at
   the end */
static Sint16 dm10_vm_msg_0[1] = { -1 } ;
static Sint16 dm10_vm_msg_1[3] = { 0x28, 0x129, -1 } ;
static Sint16 dm10_vm_msg_2[2] = { 0x2b, -1 } ;
static Sint16 dm10_vm_msg_3[2] = { 0x2c, -1 } ;
static Sint16 dm10_vm_msg_4[2] = { 0x2d, -1 } ;
static Sint16 dm10_vm_msg_5[2] = { 0x32f, -1 } ;
static Sint16 dm10_vm_msg_6[3] = { 0x30, 0x331, -1 } ;
static Sint16 dm10_vm_msg_7[3] = { 0x32, 0x33, -1 } ;
static Sint16 dm10_vm_msg_8[2] = { 0x34, -1 } ;
static Sint16 dm10_vm_msg_9[3] = { 0x334, 0x435, -1 } ;
static Sint16 dm10_vm_msg_10[2] = { 0x37, -1 } ;
static Sint16 dm10_vm_msg_11[2] = { 0x38, -1 } ;
static Sint16 dm10_vm_msg_12[3] = { 0x39, 0x129, -1 } ;
static Sint16 dm10_vm_msg_13[2] = { 0x33a, -1 } ;
static Sint16 dm10_vm_msg_14[2] = { 0x3b, -1 } ;
static Sint16 dm10_vm_msg_15[3] = { 0x3c, 0x33, -1 } ;
static Sint16 dm10_vm_msg_16[3] = { 0x33d, 0x435, -1 } ;
static Sint16 dm10_vm_msg_17[2] = { 0x3f, -1 } ;
static Sint16 dm10_vm_msg_18[2] = { 0x3e, -1 } ;
static Sint16 dm10_vm_msg_31[2] = { 0x3d, -1 } ;
static Sint16 dm10_vm_msg_19[2] = { 0x50, -1 } ;
static Sint16 dm10_vm_msg_20[3] = { 0x4a, 0x4b, -1 } ;
static Sint16 dm10_vm_msg_21[2] = { 0x34c, -1 } ;
static Sint16 dm10_vm_msg_22[4] = { 0x4d, 0x4e, 0x34f, -1 } ;
static Sint16 dm10_vm_msg_23[4] = { 0x751, 0x552, 0x53, -1 } ;
static Sint16 dm10_vm_msg_24[3] = { 0x652, 0x53, -1 } ;
static Sint16 dm10_vm_msg_25[3] = { 0x54, 0x33, -1 } ;
static Sint16 dm10_vm_msg_26[4] = { 0x0c, 0x435, 0x336, -1 } ;
static Sint16 dm10_vm_msg_27[3] = { 0x56, 0x38, -1 } ;
static Sint16 dm10_vm_msg_28[2] = { 0x56, -1 } ;
static Sint16 dm10_vm_msg_33[2] = { 0x0c, -1 } ;
static Sint16 dm10_vm_msg_29[4] = { 0x751, 0x808, 0x2e, -1 } ;
static Sint16 dm10_vm_msg_30[4] = { 0x07, 0x808, 0x2e, -1 } ;
static Sint16 dm10_vm_msg_32[5] = { 0x34, 0x935, 0x0a, 0x11, -1 } ;
static Sint16 *dm10_vm_msg_tbl[34] = {
	dm10_vm_msg_0, dm10_vm_msg_1, dm10_vm_msg_2, dm10_vm_msg_3, dm10_vm_msg_4, dm10_vm_msg_5,
	dm10_vm_msg_6, dm10_vm_msg_7, dm10_vm_msg_8, dm10_vm_msg_9, dm10_vm_msg_10, dm10_vm_msg_11,
	dm10_vm_msg_12, dm10_vm_msg_13, dm10_vm_msg_14, dm10_vm_msg_15, dm10_vm_msg_16, dm10_vm_msg_17,
	dm10_vm_msg_18, dm10_vm_msg_19, dm10_vm_msg_20, dm10_vm_msg_21, dm10_vm_msg_22, dm10_vm_msg_23,
	dm10_vm_msg_24, dm10_vm_msg_25, dm10_vm_msg_26, dm10_vm_msg_27, dm10_vm_msg_28, dm10_vm_msg_29,
	dm10_vm_msg_30, dm10_vm_msg_31, dm10_vm_msg_32, dm10_vm_msg_33
} ;

/* and each line's height */
static Float dm10_vm_msg_y_0[1] = { 0.0f } ;
static Float dm10_vm_msg_y_1[2] = { -4.3f, -5.58f } ;
static Float dm10_vm_msg_y_2[1] = { -4.3f } ;
static Float dm10_vm_msg_y_3[1] = { -4.3f } ;
static Float dm10_vm_msg_y_4[2] = { -4.3f, -5.58f } ;
static Float dm10_vm_msg_y_5[1] = { -4.3f } ;
static Float dm10_vm_msg_y_6[2] = { -4.3f, -5.58f } ;
static Float dm10_vm_msg_y_7[2] = { -4.3f, -5.58f } ;
static Float dm10_vm_msg_y_8[1] = { -4.3f } ;
static Float dm10_vm_msg_y_9[2] = { -4.3f, -5.58f } ;
static Float dm10_vm_msg_y_10[1] = { -4.3f } ;
static Float dm10_vm_msg_y_11[1] = { -4.3f } ;
static Float dm10_vm_msg_y_12[2] = { -4.3f, -5.58f } ;
static Float dm10_vm_msg_y_13[1] = { -4.3f } ;
static Float dm10_vm_msg_y_14[1] = { -4.3f } ;
static Float dm10_vm_msg_y_15[2] = { -4.3f, -5.58f } ;
static Float dm10_vm_msg_y_16[2] = { -4.3f, -5.58f } ;
static Float dm10_vm_msg_y_17[1] = { -4.3f } ;
static Float dm10_vm_msg_y_18[1] = { -4.3f } ;
static Float dm10_vm_msg_y_31[1] = { -4.3f } ;
static Float dm10_vm_msg_y_19[1] = { -4.3f } ;
static Float dm10_vm_msg_y_20[2] = { -4.3f, -5.58f } ;
static Float dm10_vm_msg_y_21[1] = { -4.3f } ;
static Float dm10_vm_msg_y_22[3] = { -4.3f, -5.58f, -6.86f } ;
static Float dm10_vm_msg_y_23[3] = { -4.3f, -5.58f, -6.86f } ;
static Float dm10_vm_msg_y_24[2] = { -4.3f, -5.58f } ;
static Float dm10_vm_msg_y_25[2] = { -4.3f, -5.58f } ;
static Float dm10_vm_msg_y_26[3] = { -4.3f, -5.58f, -6.86f } ;
static Float dm10_vm_msg_y_27[2] = { -4.3f, -5.58f } ;
static Float dm10_vm_msg_y_28[1] = { -4.3f } ;
static Float dm10_vm_msg_y_33[1] = { -4.3f } ;
static Float dm10_vm_msg_y_29[3] = { -4.3f, -5.58f, -6.86f } ;
static Float dm10_vm_msg_y_30[3] = { -4.3f, -5.58f, -6.86f } ;
static Float dm10_vm_msg_y_32[4] = { -4.2f, -5.4f, -6.5f, -7.6f } ;
static Float *dm10_vm_msg_y[34] = {
	dm10_vm_msg_y_0, dm10_vm_msg_y_1, dm10_vm_msg_y_2, dm10_vm_msg_y_3, dm10_vm_msg_y_4,
	dm10_vm_msg_y_5, dm10_vm_msg_y_6, dm10_vm_msg_y_7, dm10_vm_msg_y_8, dm10_vm_msg_y_9,
	dm10_vm_msg_y_10, dm10_vm_msg_y_11, dm10_vm_msg_y_12, dm10_vm_msg_y_13, dm10_vm_msg_y_14,
	dm10_vm_msg_y_15, dm10_vm_msg_y_16, dm10_vm_msg_y_17, dm10_vm_msg_y_18, dm10_vm_msg_y_19,
	dm10_vm_msg_y_20, dm10_vm_msg_y_21, dm10_vm_msg_y_22, dm10_vm_msg_y_23, dm10_vm_msg_y_24,
	dm10_vm_msg_y_25, dm10_vm_msg_y_26, dm10_vm_msg_y_27, dm10_vm_msg_y_28, dm10_vm_msg_y_29,
	dm10_vm_msg_y_30, dm10_vm_msg_y_31, dm10_vm_msg_y_32, dm10_vm_msg_y_33
} ;

static VEC3 dm10_vm_msg_pos[24] = {	/* the date's twelve digits, then what the other lines draw in */
	{ -7.31f, -5.58f, -13.0f }, { -6.33f, -5.58f, -13.0f }, { -5.39f, -5.58f, -13.0f }, { -4.44f, -5.58f, -13.0f },
	{ -2.37f, -5.58f, -13.0f }, { -1.42f, -5.58f, -13.0f }, { 0.52f, -5.58f, -13.0f }, { 1.46f, -5.58f, -13.0f },
	{ 3.45f, -5.58f, -13.0f }, { 4.42f, -5.58f, -13.0f }, { 6.21f, -5.58f, -13.0f }, { 7.17f, -5.58f, -13.0f },
	{ -0.78f, -4.3f, -13.0f }, { 3.27f, -6.86f, -13.0f }, { 4.24f, -6.86f, -13.0f }, { 3.27f, -6.86f, -13.0f },
	{ 4.24f, -6.86f, -13.0f }, { 0.94f, -4.3f, -13.0f }, { -0.03f, -5.58f, -13.0f }, { -0.24f, -4.3f, -13.0f },
	{ 0.7f, -4.3f, -13.0f }, { 2.47f, -5.58f, -13.0f }, { -3.12f, -5.35f, -13.0f }, { -2.14f, -5.35f, -13.0f }
} ;
static Uint8 dm10_vm_msg_idx[10] = { 0, 0, 12, 0, 13, 18, 19, 17, 21, 22 } ;	/* the first of dm10_vm_msg_pos by what is drawn */
static void ( *dm10_vm_msg_jmp[3] )( PLAYER_WORK *w ) = { dm10_vm_msg_init, dm10_vm_msg_wait, dm10_vm_msg_end } ;

/****************************************************************/
/*			The card message's lines							*/
/****************************************************************/
void dm10_vm_msg_set( void )
{
	Sint16 *p ;
	Float *y ;
	VEC3 pos ;
	int n ;

	n = vms_work.vm_msg[0] ;
	p = dm10_vm_msg_tbl[n] ;
	y = dm10_vm_msg_y[n] ;
	pos.x = 0.0f ;
	pos.z = -13.0f ;
	while( *p != -1 ){
		pos.y = *y ++ ;
		dm10_vm_msg_sub_set( n, *p ++, &pos ) ;
	}
}

/****************************************************************/
/*			A line of the message, or a character drawn in		*/
/****************************************************************/
PLAYER_WORK *dm10_vm_msg_sub_set( int n, int code, VEC3 *pos )
{
	PLAYER_WORK *w ;

	if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return NULL ;
	w->disp_flag = 1 ;
	w->work_func = dm10_vm_msg_move ;
	w->obj = obj_dm10->obj[code & ~0xff00] ;
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
void dm10_vm_msg_move( PLAYER_WORK *w )
{
	dm10_vm_msg_jmp[w->routine_0]( w ) ;
}

/****************************************************************/
/*			What the line draws in: the date, the port, the		*/
/*			yes/no												*/
/****************************************************************/
void dm10_vm_msg_init( PLAYER_WORK *w )
{
	PLAYER_WORK *w2 ;
	VEC3 *pos ;
	VMS_DATE date ;
	int i ;

	w->routine_0 ++ ;
	pos = &dm10_vm_msg_pos[dm10_vm_msg_idx[w->timer0 >> 8]] ;
	switch( w->timer0 ){
	case 0x100 :
		for( i = 0 ; i < 12 ; i ++ ){
			if( !( w2 = dm10_vm_msg_sub_set( w->type, 28, pos ++ ) ) ) return ;
			w2->sub_type = i + 1 ;
			w2->timer0 = 0 ;
		}
		break ;
	case 0x200 :
	case 0x700 :
		dm10_vm_msg_sub_set( w->type, 33, pos ) ;
		break ;
	case 0x300 :
		dm10_vm_yesno_set( w->type ) ;
		break ;
	case 0x400 :
		if( w->type >= 12 ) pos += 2 ;
	case 0x900 :
		dm10_vm_msg_sub_set( w->type, vms_work.cursor_port / 2 + 24, pos ++ ) ;
		dm10_vm_msg_sub_set( w->type, ( vms_work.cursor_port & 1 ) + 29, pos ) ;
		break ;
	case 0x500 :
	case 0x800 :
		dm10_vm_msg_sub_set( w->type, 34, pos ) ;
		break ;
	case 0x600 :
		dm10_vm_msg_sub_set( w->type, 33, pos ++ ) ;
		dm10_vm_msg_sub_set( w->type, 28, pos ) ;
	}
	dm10_vm_msg_wait( w ) ;
}

/****************************************************************/
/*			Shown until the message changes; a digit of the		*/
/*			card's date (sub_type), without a leading 0			*/
/****************************************************************/
void dm10_vm_msg_wait( PLAYER_WORK *w )
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
	w->obj = obj_dm10->obj[n + 28] ;
}

void dm10_vm_msg_end( PLAYER_WORK *w )
{
	work_free( w ) ;
}

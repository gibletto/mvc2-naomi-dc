/* DM0D, the memory card window: its frame and its message's lines by vms_work.vm_msg[0], with the
   yes/no cursor, gone when the message changes. */
#include "player.h"
#include "nl.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

void dm0d_vm_win_parts_set( Uint8 n, Uint16 obj ) ;
void dm0d_vm_win_move( PLAYER_WORK *w ) ;
void dm0d_vm_win_init( PLAYER_WORK *w ) ;
void dm0d_vm_win_wait( PLAYER_WORK *w ) ;
void dm0d_vm_msg_parts_set( Uint8 n, Uint16 obj ) ;
void dm0d_vm_msg_move( PLAYER_WORK *w ) ;
void dm0d_vm_msg_init( PLAYER_WORK *w ) ;
void dm0d_vm_msg_main( PLAYER_WORK *w ) ;
void dm0d_vm_msg_nop( PLAYER_WORK *w ) ;
void dm0d_vm_msg_cursor( PLAYER_WORK *w ) ;
void dm0d_vm_msg_cursor_move( PLAYER_WORK *w ) ;
void dm0d_vm_msg_cursor_flash( PLAYER_WORK *w ) ;
Uint8 dm0d_vm_msg_gone( PLAYER_WORK *w ) ;
void dm0d_vm_off( PLAYER_WORK *w ) ;
void dm0d_vm_free( PLAYER_WORK *w ) ;

/****************************************************************/
/*			The window's frame: three models					*/
/****************************************************************/
void dm0d_vm_win_set( void )
{
	dm0d_vm_win_parts_set( 0, 59 ) ;
	dm0d_vm_win_parts_set( 1, 60 ) ;
	dm0d_vm_win_parts_set( 2, 61 ) ;
}

void dm0d_vm_win_parts_set( Uint8 n, Uint16 obj )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_TOP ) ) ){
		w->disp_flag = 0 ;
		w->work_func = dm0d_vm_win_move ;
		w->obj_flag = OBJ_POS ;
		w->obj = obj_dm0d->obj[obj] ;
		w->type = vms_work.vm_msg[0] ;
		w->sub_no = n ;
	}
}

void dm0d_vm_win_move( PLAYER_WORK *w )
{
	static void ( *dm0d_vm_win_jmp[4] )( PLAYER_WORK *w ) = {
		dm0d_vm_win_init, dm0d_vm_win_wait, dm0d_vm_off, dm0d_vm_free
	} ;

	dm0d_vm_win_jmp[w->routine_0]( w ) ;
}

void dm0d_vm_win_init( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
	w->disp_flag = 1 ;
	switch( w->sub_no ){
	case 0 :
		w->pos.z = -13.0f ;
		break ;
	case 1 :
		w->pos.z = -13.3f ;
		break ;
	case 2 :
		w->pos.z = -12.7f ;
		break ;
	}
}

/* the frame stays until the message is cleared */
void dm0d_vm_win_wait( PLAYER_WORK *w )
{
	if( !vms_work.vm_msg[0] ) w->routine_0 = 2 ;
}

/****************************************************************/
/*			A line of the message								*/
/****************************************************************/
void dm0d_vm_msg_parts_set( Uint8 n, Uint16 obj )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_TOP ) ) ){
		w->disp_flag = 0 ;
		w->work_func = dm0d_vm_msg_move ;
		w->obj_flag = OBJ_POS | OBJ_TRNSL ;
		w->sub_no = n ;
		w->obj = obj_dm0d->obj[obj] ;
		w->type = vms_work.vm_msg[0] ;
		w->fade.trnsl = 1.0f ;
	}
}

void dm0d_vm_msg_move( PLAYER_WORK *w )
{
	static void ( *dm0d_vm_msg_jmp[4] )( PLAYER_WORK *w ) = {
		dm0d_vm_msg_init, dm0d_vm_msg_main, dm0d_vm_off, dm0d_vm_free
	} ;

	dm0d_vm_msg_jmp[w->routine_0]( w ) ;
}

void dm0d_vm_msg_init( PLAYER_WORK *w )
{
	if( w->sub_no == 11 ){
		if( vms_work.vm_msg[1] & 0x7f ) w->pos.x = 4.7f ;
		else w->pos.x = 0.0f ;
	}
	w->disp_flag = 1 ;
	w->routine_0 ++ ;
}

/* line 11 is the yes/no cursor */
void dm0d_vm_msg_main( PLAYER_WORK *w )
{
	static void ( *dm0d_vm_msg_main_jmp[14] )( PLAYER_WORK *w ) = {
		dm0d_vm_msg_nop, dm0d_vm_msg_nop, dm0d_vm_msg_nop, dm0d_vm_msg_nop,
		dm0d_vm_msg_nop, dm0d_vm_msg_nop, dm0d_vm_msg_nop, dm0d_vm_msg_nop,
		dm0d_vm_msg_nop, dm0d_vm_msg_nop, dm0d_vm_msg_nop, dm0d_vm_msg_cursor,
		dm0d_vm_msg_nop, dm0d_vm_msg_nop
	} ;

	dm0d_vm_msg_main_jmp[w->sub_no]( w ) ;
	if( dm0d_vm_msg_gone( w ) ) w->routine_0 = 2 ;
}

void dm0d_vm_msg_nop( PLAYER_WORK *w )
{
}

void dm0d_vm_msg_cursor( PLAYER_WORK *w )
{
	static void ( *dm0d_vm_cursor_jmp[2] )( PLAYER_WORK *w ) = {
		dm0d_vm_msg_cursor_move, dm0d_vm_msg_cursor_flash
	} ;

	dm0d_vm_cursor_jmp[w->routine_1]( w ) ;
}

/* pulses on yes or no; vm_msg[1]'s top bit is the choice made, 0xff the window closed */
void dm0d_vm_msg_cursor_move( PLAYER_WORK *w )
{
	w->timer1 += 9 ;
	if( w->timer1 >= 360 ) w->timer1 = 0 ;
	w->fade.trnsl = nlCos( DEG_ANG( w->timer1 ) ) * 0.5f + 0.5f ;
	w->spd.x = w->pos.x ;
	if( vms_work.vm_msg[1] & 0x7f ) w->pos.x = 4.7f ;
	else w->pos.x = 0.0f ;
	if( w->spd.x != w->pos.x ) w->timer1 = 0 ;
	if( vms_work.vm_msg[1] == 0xff ){
		w->routine_0 = 2 ;
		w->disp_flag = 0 ;
		vms_work.vm_msg[1] = 0 ;
	} else if( vms_work.vm_msg[1] & 0x80 ){
		w->routine_1 ++ ;
		w->fade.trnsl = 1.0f ;
		w->timer0 = 0 ;
		w->timer1 = 30 ;
	}
}

/* the choice flashes for half a second */
void dm0d_vm_msg_cursor_flash( PLAYER_WORK *w )
{
	if( !( ++ w->timer0 & 3 ) ) w->disp_flag ^= 1 ;
	if( !( -- w->timer1 ) ){
		w->routine_0 = 2 ;
		w->disp_flag = 0 ;
		vms_work.vm_msg[1] = 0 ;
	}
}

Uint8 dm0d_vm_msg_gone( PLAYER_WORK *w )
{
	if( w->routine_0 < 2 && w->type != vms_work.vm_msg[0] ) return 1 ;
	return 0 ;
}

/****************************************************************/
/*			The message's lines, with the card's port			*/
/****************************************************************/
void dm0d_vm_msg_set( void )
{
	switch( vms_work.vm_msg[0] ){
	case 1 :
		dm0d_vm_msg_parts_set( 2, 0x126 ) ;
		break ;
	case 2 :
		dm0d_vm_msg_parts_set( 3, 0x127 ) ;
		break ;
	case 3 :
		dm0d_vm_msg_parts_set( 4, 0x128 ) ;
		dm0d_vm_msg_parts_set( 5, 0x12c ) ;
		if( vms_work.file_flag != 1 ){
			dm0d_vm_msg_parts_set( 6, 0x12d ) ;
			dm0d_vm_msg_parts_set( 7, 0x131 ) ;
		} else {
			switch( vms_work.file_port ){
			case 0 :
				dm0d_vm_msg_parts_set( 6, 0x12d ) ;
				dm0d_vm_msg_parts_set( 7, 0x131 ) ;
				break ;
			case 1 :
				dm0d_vm_msg_parts_set( 6, 0x12d ) ;
				dm0d_vm_msg_parts_set( 7, 0x132 ) ;
				break ;
			case 2 :
				dm0d_vm_msg_parts_set( 6, 0x12e ) ;
				dm0d_vm_msg_parts_set( 7, 0x131 ) ;
				break ;
			case 3 :
				dm0d_vm_msg_parts_set( 6, 0x12e ) ;
				dm0d_vm_msg_parts_set( 7, 0x132 ) ;
				break ;
			case 4 :
				dm0d_vm_msg_parts_set( 6, 0x12f ) ;
				dm0d_vm_msg_parts_set( 7, 0x131 ) ;
				break ;
			case 5 :
				dm0d_vm_msg_parts_set( 6, 0x12f ) ;
				dm0d_vm_msg_parts_set( 7, 0x132 ) ;
				break ;
			case 6 :
				dm0d_vm_msg_parts_set( 6, 0x130 ) ;
				dm0d_vm_msg_parts_set( 7, 0x131 ) ;
				break ;
			case 7 :
				dm0d_vm_msg_parts_set( 6, 0x130 ) ;
				dm0d_vm_msg_parts_set( 7, 0x132 ) ;
				break ;
			}
		}
		break ;
	case 4 :
		dm0d_vm_msg_parts_set( 8, 0x129 ) ;
		break ;
	case 5 :
		dm0d_vm_msg_parts_set( 9, 0x12a ) ;
		dm0d_vm_msg_parts_set( 10, 0x10a ) ;
		dm0d_vm_msg_parts_set( 11, 0x133 ) ;
		break ;
	case 6 :
		dm0d_vm_msg_parts_set( 12, 0x12b ) ;
		dm0d_vm_msg_parts_set( 10, 0x10a ) ;
		dm0d_vm_msg_parts_set( 11, 0x133 ) ;
		break ;
	case 7 :
		dm0d_vm_msg_parts_set( 13, 20 ) ;
		break ;
	}
}

void dm0d_vm_off( PLAYER_WORK *w )
{
	w->disp_flag = 0 ;
	w->routine_0 ++ ;
}

void dm0d_vm_free( PLAYER_WORK *w )
{
	work_free( w ) ;
}

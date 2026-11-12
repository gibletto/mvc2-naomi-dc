/* SECRET FACTOR's third choice: the VMU game's save to a memory card. */
#include "player.h"
#include "shinobi.h"
#include "dm.h"
#include "secret.h"

extern void vms_port_move() ;

#define VM_NOP()	do { } while( 0 )	/* a statement Capcom's source had here, empty in this build */

/****************************************************************/
/*			The VMU game, saved to a card						*/
/****************************************************************/
void secret_factor_3( Uint16 sw, Uint16 sw2 )
{
	switch( Sys->routine_2 ){
	case 0 :
		Sys->routine_2 ++ ;
		Sys->routine_3 = 0 ;
		Sys->timer0 = 0 ;
		Sys->timer1 = 0 ;
		work_list_kill( 11 ) ;
		dm0c_wall_paper( 4 ) ;
		headline_set( 15 ) ;
		dm0f_save_set() ;
		vms_work.vm_msg[0] = 1 ;
		dm0f_vm_msg_set() ;
		dm_obj_set( 270, obj_dm0f ) ;
		fade_set( 0xff000000, 10, FADE_IN ) ;
		break ;
	case 1 :
		if( !fade_cnt() ) Sys->routine_2 ++ ;
		break ;
	case 2 :
		switch( Sys->routine_3 ){
		case 0 :
			Sys->timer0 = ud_select_loop( sw, Sys->timer0, 1 ) ;
			if( 0 ) dm0f_save_set() ;	/* left out of the build */
			if( Sys->timer0 + 1 != vms_work.vm_msg[0] ){
				if( 0 ) work_list_kill( 11 ) ;	/* left out of the build */
				vms_work.vm_msg[0] = Sys->timer0 + 1 ;
				dm0f_vm_msg_set() ;
			}
			if( decide_ck() ){
				if( Sys->timer0 == 0 ){
					Sys->routine_3 ++ ;
					vms_work.vm_msg[0] = 3 ;
					vms_work.vm_msg[1] = 1 ;
					Sys->timer0 |= 0x80 ;
					Sys->timer1 = vms_work.vm_msg[1] ;
					dm0f_vm_msg_set() ;
					break ;
				}
			} else if( !cancel_ck() ) break ;
			Sys->routine_2 ++ ;
			fade_set( 0xff000000, 10, FADE_OUT ) ;
			break ;
		case 1 :
			Sys->timer1 = ud_select_loop( sw, Sys->timer1, 1 ) ;
			if( Sys->timer1 != vms_work.vm_msg[1] ) vms_work.vm_msg[1] = Sys->timer1 ;
			if( decide_ck() ){
				if( Sys->timer1 == 0 ) Sys->routine_3 ++ ;
				else Sys->routine_3 = 5 ;
				vms_work.vm_msg[1] |= 0x80 ;
			} else if( cancel_ck() ){
				Sys->routine_3 = 5 ;
				vms_work.vm_msg[1] = 0xff ;
			}
			break ;
		case 2 :
			if( vms_work.vm_msg[1] ) break ;
			Sys->routine_3 ++ ;
			vms_work.vm_msg[0] = 4 ;
			vms_work.vm_msg[1] = 1 ;
			Sys->timer1 = vms_work.vm_msg[1] ;
			dm0f_vm_msg_set() ;
			break ;
		case 3 :
			Sys->timer1 = ud_select_loop( sw, Sys->timer1, 1 ) ;
			if( Sys->timer1 != vms_work.vm_msg[1] ) vms_work.vm_msg[1] = Sys->timer1 ;
			if( decide_ck() ){
				if( Sys->timer1 == 0 ) Sys->routine_3 = 7 ;
				else Sys->routine_3 = 6 ;
				vms_work.vm_msg[1] |= 0x80 ;
			} else if( cancel_ck() ){
				Sys->routine_3 = 6 ;
				vms_work.vm_msg[1] = 0xff ;
			}
			break ;
		case 4 :
			if( !fade_cnt() ){
				Sys->routine_2 = 4 ;
				Sys->routine_3 = Sys->exit_flag = 0 ;
			}
			break ;
		case 5 :
			if( vms_work.vm_msg[1] ) break ;
			Sys->routine_3 = 0 ;
			Sys->timer0 = 0 ;
			Sys->timer1 = 0 ;
			vms_work.vm_msg[0] = 1 ;
			dm0f_vm_msg_set() ;
			break ;
		case 6 :
			if( vms_work.vm_msg[1] ) break ;
			Sys->routine_3 = 1 ;
			vms_work.vm_msg[0] = 3 ;
			vms_work.vm_msg[1] = 1 ;
			Sys->timer1 = vms_work.vm_msg[1] ;
			dm0f_vm_msg_set() ;
			break ;
		case 7 :
			if( vms_work.vm_msg[1] ) break ;
			Sys->routine_3 = 4 ;
			vms_work.vm_msg[0] = 0 ;
			vms_work.vm_msg[1] = 0 ;
			Sys->timer1 = vms_work.vm_msg[1] ;
			fade_set( 0xff000000, 10, FADE_OUT ) ;
			break ;
		}
		break ;
	case 3 :
		if( !fade_cnt() ){
			Sys->routine_0 = 1 ;
			Sys->routine_2 = 0 ;
			Sys->routine_3 = 0 ;
		}
		break ;
	case 4 :
		secret_factor_3_port( sw ) ;
		break ;
	}
}

/****************************************************************/
/*			The card to save to									*/
/****************************************************************/
void secret_factor_3_port( Uint16 sw )
{
	vms_card_bit_set() ;
	switch( Sys->routine_3 ){
	case 0 :
		Sys->routine_3 ++ ;
		Sys->exit_flag = 0 ;
		Sys->timer0 = 0 ;
		vms_work.cursor_port = -1 ;
		work_list_kill( 11 ) ;
		dm0c_wall_paper( 4 ) ;
		headline_set( 15 ) ;
		vms_work.vm_msg[0] = 5 ;
		dm0f_vm_msg_set() ;
		dm0f_port_set() ;
		fade_set( 0xff000000, 10, FADE_IN ) ;
	case 1 :
		if( !fade_cnt() ) Sys->routine_3 ++ ;
		break ;
	case 2 :
		if( !vms_port_first() ){
			Sys->routine_3 = 5 ;
			Sys->exit_flag = 0 ;
			vms_work.cursor_port = -1 ;
			break ;
		}
		Sys->routine_3 = 3 ;
		vms_work.vm_msg[0] = 5 ;
		dm0f_vm_msg_set() ;
		break ;
	case 3 :
		if( !vms_work.card_bit ){
			Sys->routine_3 = 5 ;
			Sys->exit_flag = 0 ;
			vms_work.cursor_port = -1 ;
			break ;
		}
		vms_port_move( sw ) ;
		if( decide_ck() ){
			Sys->routine_3 = 4 ;
			return ;
		}
		if( cancel_ck() ){
			Sys->routine_3 = 6 ;
			fade_set( 0xff000000, 10, FADE_OUT ) ;
		}
		break ;
	case 4 :
		secret_factor_3_save( sw ) ;
		break ;
	case 5 :
		if( vms_work.card_bit != 0 ){
			Sys->routine_3 = 2 ;
			Sys->exit_flag = 0 ;
			break ;
		}
		switch( Sys->exit_flag ){
		case 0 :
			Sys->exit_flag ++ ;
			vms_work.vm_msg[0] = 6 ;
			dm0f_vm_msg_set() ;
			break ;
		case 1 :
			if( decide_cancel_ck() ){
				Sys->exit_flag ++ ;
				vms_work.vm_msg[0] = 7 ;
				dm0f_vm_msg_set() ;
			}
			break ;
		case 2 :
			if( decide_cancel_ck() ){
				na_req( 3 ) ;
				Sys->routine_3 = 6 ;
				fade_set( 0xff000000, 10, FADE_OUT ) ;
			}
			break ;
		}
		break ;
	case 6 :
		if( !fade_cnt() ){
			Sys->routine_2 = 0 ;
			Sys->routine_3 = 0 ;
			Sys->exit_flag = 0 ;
		}
		break ;
	}
}

/****************************************************************/
/*			The save to the card: the VMU game's file, made		*/
/*			or written again									*/
/****************************************************************/
/* SHC compiles this routine unoptimized: it is too big for the optimizer. That mode keeps what
   cannot be reached, so the gotos twice over and the steps after a goto are as the ROM has them. */
void secret_factor_3_save( Uint16 sw )
{
	int bit, ret ;

	switch( Sys->exit_flag ){
	case 0 :
		Sys->timer2 = 0 ;
		bit = vms_check_other_execfile() ;
		if( bit & ( 1 << vms_work.cursor_port ) ){
			Sys->exit_flag = 2 ;
			vms_work.vm_msg[0] = 16 ;
			goto msg ;
			goto msg ;
		}
		if( vms_card_sys_ck( vms_work.cursor_port ) ){
			bit = vms_check_execfile() ;
			if( bit & ( 1 << vms_work.cursor_port ) ){
				Sys->exit_flag = 1 ;
				vms_work.vm_msg[0] = 10 ;
				vms_work.vm_msg[1] = 1 ;
				goto msg ;
			}
			bit = vms_check_free_exec() ;
			if( bit & ( 1 << vms_work.cursor_port ) ){
				Sys->exit_flag = 1 ;
				vms_work.vm_msg[0] = 10 ;
				vms_work.vm_msg[1] = 1 ;
				VM_NOP() ;
				goto msg ;
				goto msg ;
			}
			Sys->exit_flag = 2 ;
			Sys->timer2 = 1 ;
			vms_work.vm_msg[0] = 8 ;
			dm0f_vm_msg_set() ;
			VM_NOP() ;
			return ;
		}
		bit = vms_check_execfile() ;
		if( bit & ( 1 << vms_work.cursor_port ) ){
			bit = vms_check_free_exec() ;
			if( bit & ( 1 << vms_work.cursor_port ) ){
				Sys->exit_flag = 3 ;
				vms_work.vm_msg[0] = 11 ;
				goto msg ;
			}
			Sys->exit_flag = 2 ;
			Sys->timer2 = 2 ;
			vms_work.vm_msg[0] = 8 ;
			VM_NOP() ;
			goto msg ;
			goto msg ;
		}
		bit = vms_check_free_exec() ;
		if( bit & ( 1 << vms_work.cursor_port ) ){
			Sys->exit_flag = 3 ;
			vms_work.vm_msg[0] = 11 ;
			VM_NOP() ;
			goto msg ;
			goto msg ;
		}
		Sys->exit_flag = 2 ;
		Sys->timer2 = 3 ;
		vms_work.vm_msg[0] = 8 ;
		VM_NOP() ;
		goto msg ;
		VM_NOP() ;
		break ;
		VM_NOP() ;
		break ;
		VM_NOP() ;
		break ;
	case 1 :
		ret = vms_card_ck( vms_work.cursor_port ) ;
		if( ret == 0 || ret == -1 ) goto back ;
		switch( Sys->timer2 ){
		case 0 :
			vms_work.vm_msg[1] = ud_select_loop( sw, vms_work.vm_msg[1], 1 ) ;
			if( decide_ck() ){
				if( !vms_work.vm_msg[1] ) Sys->timer2 = 1 ;
				else Sys->timer2 = 2 ;
				goto decide ;
			} else if( cancel_ck() ){
				vms_work.vm_msg[1] = 0xff ;
				Sys->timer2 = 2 ;
				VM_NOP() ;
				return ;
			}
			VM_NOP() ;
			break ;
		case 1 :
			if( !vms_work.vm_msg[1] ){
				Sys->exit_flag = 3 ;
				Sys->timer2 = 0 ;
				vms_work.vm_msg[0] = 11 ;
				dm0f_vm_msg_set() ;
				VM_NOP() ;
				VM_NOP() ;
				return ;
			}
			VM_NOP() ;
			break ;
		case 2 :
			if( !vms_work.vm_msg[1] ) goto back ;
			VM_NOP() ;
			return ;
		default :
			VM_NOP() ;
			return ;
		}
		VM_NOP() ;
		break ;
	case 2 :
		switch( Sys->timer2 ){
		case 0 :
			if( decide_cancel_ck() ){
				Sys->timer2 = 4 ;
				vms_work.vm_msg[0] = 21 ;
				dm0f_vm_msg_set() ;
			} else break ;
			VM_NOP() ;
			return ;
			VM_NOP() ;
			break ;
		case 1 :
			if( decide_cancel_ck() ){
				Sys->timer2 = 4 ;
				vms_work.vm_msg[0] = 15 ;
				dm0f_vm_msg_set() ;
			} else break ;
			VM_NOP() ;
			return ;
			VM_NOP() ;
			break ;
		case 2 :
			if( decide_cancel_ck() ){
				Sys->timer2 = 4 ;
				vms_work.vm_msg[0] = 23 ;
				dm0f_vm_msg_set() ;
			} else break ;
			VM_NOP() ;
			return ;
			VM_NOP() ;
			break ;
		case 3 :
			if( decide_cancel_ck() ){
				Sys->timer2 = 4 ;
				vms_work.vm_msg[0] = 7 ;
				dm0f_vm_msg_set() ;
				if( 0 ) Sys->timer2 = 4 ;	/* left out of the build */
			} else break ;
			VM_NOP() ;
			return ;
			VM_NOP() ;
			break ;
		case 4 :
			if( !decide_cancel_ck() ) break ;
			goto cancel ;
		default :
			VM_NOP() ;
			return ;
		}
		VM_NOP() ;
		break ;
	case 3 :
		vms_work.save_busy = 1 ;
		ret = vms_data_save( 1, vms_work.cursor_port ) ;
		if( ret == -1 ) break ;
		else {
			vms_work.save_busy = 0 ;
			if( ret == 0 ){
				Sys->exit_flag = 4 ;
				Sys->timer2 = 0 ;
				vms_work.vm_msg[0] = 12 ;
				VM_NOP() ;
			} else {
				Sys->exit_flag = 6 ;
				Sys->timer2 = 0 ;
				if( vms_work.file_flag == 1 && vms_work.file_port == vms_work.cursor_port ){
					vms_work.file_flag = 0 ;
					vms_work.vm_msg[0] = 20 ;
				} else vms_work.vm_msg[0] = 21 ;
			}
			dm0f_vm_msg_set() ;
		}
		VM_NOP() ;
		return ;
		VM_NOP() ;
		break ;
	case 4 :
		switch( Sys->timer2 ){
		case 0 :
			bit = vms_check_execfile() ;
			if( bit & ( 1 << vms_work.cursor_port ) ) Sys->timer2 = 2 ;
			else Sys->timer2 = 1 ;
			vms_work.vm_msg[0] = 17 ;
			VM_NOP() ;
			goto msg ;
		msg :
			VM_NOP() ;
			dm0f_vm_msg_set() ;
			VM_NOP() ;
			VM_NOP() ;
			return ;
			VM_NOP() ;
			break ;
		case 1 :
			vms_work.save_busy = 1 ;
			ret = vms_exec_save( vms_work.cursor_port ) ;
			if( ret != -1 ){
				if( ret == 0 ){
					vms_work.save_busy = 1 ;
					Sys->timer2 ++ ;
				} else {
					vms_work.save_busy = 0 ;
					Sys->exit_flag = 6 ;
					Sys->timer2 = 0 ;
					if( vms_work.file_flag == 1 && vms_work.file_port == vms_work.cursor_port ){
						vms_work.file_flag = 0 ;
						vms_work.vm_msg[0] = 20 ;
					} else vms_work.vm_msg[0] = 21 ;
					dm0f_vm_msg_set() ;
				}
				VM_NOP() ;
				return ;
			}
			VM_NOP() ;
			break ;
		case 2 :
			vms_work.save_busy = 1 ;
			ret = vms_exec_resave( vms_work.cursor_port, 0 ) ;
			if( ret == -1 ) break ;
			else {
			vms_work.save_busy = 0 ;
			if( ret == 0 ){
				Sys->exit_flag ++ ;
				if( vms_work.file_flag == 1 && vms_work.file_port == vms_work.cursor_port ){
					Sys->timer2 = 2 ;
					vms_work.vm_msg[0] = 22 ;
				} else {
					Sys->timer2 = 0 ;
					vms_work.vm_msg[0] = 18 ;
					vms_work.vm_msg[1] = 1 ;
				}
				goto set42 ;
			} else {
				Sys->exit_flag = 6 ;
				Sys->timer2 = 0 ;
				if( vms_work.file_flag == 1 && vms_work.file_port == vms_work.cursor_port ){
					vms_work.file_flag = 0 ;
					vms_work.vm_msg[0] = 20 ;
				} else vms_work.vm_msg[0] = 21 ;
			}
		set42 :
			dm0f_vm_msg_set() ;
			VM_NOP() ;
			VM_NOP() ;
			return ;
			}
		default :
			VM_NOP() ;
			return ;
		}
		VM_NOP() ;
		break ;
	case 5 :
		switch( Sys->timer2 ){
		case 0 :
			vms_work.vm_msg[1] = ud_select_loop( sw, vms_work.vm_msg[1], 1 ) ;
			if( decide_ck() ){
				if( !vms_work.vm_msg[1] ){
					vms_work.file_port = vms_work.cursor_port ;
					vms_work.file_flag = 1 ;
				}
				Sys->timer2 = 1 ;
			decide :
				VM_NOP() ;
				vms_work.vm_msg[1] |= 0x80 ;
				VM_NOP() ;
				return ;
			} else if( cancel_ck() ){
				Sys->timer2 = 1 ;
				vms_work.vm_msg[1] = 0xff ;
				VM_NOP() ;
				return ;
			}
			VM_NOP() ;
			break ;
		case 1 :
			if( !vms_work.vm_msg[1] ){
				Sys->exit_flag = 7 ;
				fade_set( 0xff000000, 10, FADE_OUT ) ;
				VM_NOP() ;
				return ;
			}
			VM_NOP() ;
			break ;
		case 2 :
			if( decide_cancel_ck() ){
				Sys->exit_flag = 7 ;
				fade_set( 0xff000000, 10, FADE_OUT ) ;
				VM_NOP() ;
				return ;
			}
			VM_NOP() ;
			break ;
		default :
			VM_NOP() ;
			return ;
		}
		VM_NOP() ;
		break ;
	case 6 :
		if( decide_cancel_ck() ){
		cancel :
			VM_NOP() ;
			na_req( 3 ) ;
			goto back ;
		}
		VM_NOP() ;
		break ;
	case 7 :
		if( fade_cnt() ) break ;
		goto end ;
	back :
		VM_NOP() ;
		Sys->routine_3 = 2 ;
		Sys->exit_flag = 0 ;
		Sys->timer2 = 0 ;
		vms_work.vm_msg[0] = 0 ;
		vms_work.vm_msg[1] = 0 ;
		Sys->timer0 = 0 ;
		Sys->timer1 = 0 ;
		VM_NOP() ;
		return ;
	end :
		VM_NOP() ;
		Sys->routine_2 = 0 ;
		Sys->routine_3 = 0 ;
		Sys->exit_flag = 0 ;
		Sys->timer2 = 0 ;
		vms_work.vm_msg[0] = 0 ;
		vms_work.vm_msg[1] = 0 ;
		Sys->timer0 = 0 ;
		Sys->timer1 = 0 ;
		VM_NOP() ;
		break ;
	default :
		VM_NOP() ;
		return ;
	}
}

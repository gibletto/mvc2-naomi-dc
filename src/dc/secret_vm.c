/* The home game's memory card menus: the system file's save and load. */
#include "player.h"
#include "shinobi.h"
#include "dm.h"
#include "secret.h"

extern void vms_port_move( Uint16 key ) ;

/****************************************************************/
/*			The memory card menu's steps, from the start		*/
/****************************************************************/
void vm_menu_clear( void )
{
	Sys->routine_0 = 0 ;
	Sys->routine_1 = 0 ;
	Sys->routine_2 = 0 ;
}

/****************************************************************/
/*			The memory card menu: SAVE, LOAD and EXIT			*/
/****************************************************************/
void vm_menu( void )
{
	Uint16 sw ;

	sw = menu_sw_get( 3 ) ;
	switch( Sys->routine_0 ){
	case 0 :
		Palette_Init() ;
		Texture_Init() ;
		TexLoad( 19 ) ;
		Sys->routine_1 = 0 ;
		Sys->routine_2 = 0 ;
	case 1 :
		Sys->routine_0 = 2 ;
		work_init() ;
		vms_work.card_bit = 0 ;
		vms_work.card_bit_keep = 0 ;
		dm10_set( 0 ) ;
		dm0c_wall_paper( 8 ) ;
		fade_set( 0xff000000, 10, FADE_IN ) ;
		break ;
	case 2 :
		if( !fade_cnt() ) Sys->routine_0 ++ ;
		break ;
	case 3 :
		Sys->routine_1 = ud_select_loop( sw, Sys->routine_1, 2 ) ;
		if( decide_ck() ){
			if( Sys->routine_1 == 2 ) goto six ;
			Sys->routine_0 = 4 ;
			fade_set( 0xff000000, 10, FADE_OUT ) ;
			return ;
		}
		if( cancel_ck() ){
		six :
			Sys->routine_0 = 6 ;
			fade_set( 0xff000000, 10, FADE_OUT ) ;
		}
		break ;
	case 4 :
		if( !fade_cnt() ){
			Sys->routine_0 ++ ;
			return ;
		}
		break ;
	case 5 :
		switch( Sys->routine_1 ){
		case 0 :
			vm_save_port( sw ) ;
			break ;
		case 1 :
			vm_load_port( sw ) ;
			break ;
		default :
			Sys->routine_0 ++ ;
			fade_set( 0xff000000, 10, FADE_OUT ) ;
			break ;
		}
		break ;
	case 6 :
		if( !fade_cnt() ){
			Sys->task_step = Sys->scene_no = 0 ;
			vm_menu_clear() ;
		}
		break ;
	}
}

/****************************************************************/
/*			SAVE: the card to save to							*/
/****************************************************************/
void vm_save_port( Uint16 sw )
{
	vms_card_bit_set() ;
	switch( Sys->routine_2 ){
	case 0 :
		work_init() ;
		Sys->routine_2 = 1 ;
		vms_work.cursor_port = -1 ;
		dm10_set( 1 ) ;
		dm0c_wall_paper( 8 ) ;
		fade_set( 0xff000000, 10, FADE_IN ) ;
		break ;
	case 1 :
		if( !fade_cnt() ){
			Sys->routine_2 ++ ;
			return ;
		}
		break ;
	case 2 :
		if( !vms_port_first() ){
			Sys->routine_2 = 5 ;
			Sys->routine_3 = 0 ;
			vms_work.cursor_port = -1 ;
			break ;
		}
		Sys->routine_2 ++ ;
		vms_work.vm_msg[0] = 1 ;
		dm10_vm_msg_set() ;
		break ;
	case 3 :
		if( !vms_work.card_bit ){
			Sys->routine_2 = 5 ;
			Sys->routine_3 = 0 ;
			vms_work.cursor_port = -1 ;
			break ;
		}
		vms_port_move( sw ) ;
		if( decide_ck() ) Sys->routine_2 = 4 ;
		else if( cancel_ck() ){
			Sys->routine_2 = 6 ;
			fade_set( 0xff000000, 10, FADE_OUT ) ;
		}
		break ;
	case 4 :
		vm_save( sw ) ;
		break ;
	case 5 :
		if( vms_work.card_bit != 0 ){
			Sys->routine_2 = 2 ;
			Sys->routine_3 = 0 ;
			vms_work.vm_msg[0] = 1 ;
			dm10_vm_msg_set() ;
			break ;
		}
		switch( Sys->routine_3 ){
		case 0 :
			Sys->routine_3 ++ ;
			vms_work.vm_msg[0] = 2 ;
			dm10_vm_msg_set() ;
		case 1 :
			if( decide_cancel_ck() ){
				Sys->routine_3 ++ ;
				vms_work.vm_msg[0] = 4 ;
				dm10_vm_msg_set() ;
			}
			break ;
		case 2 :
			if( decide_cancel_ck() ){
				na_req( 3 ) ;
				Sys->routine_2 = 6 ;
				fade_set( 0xff000000, 10, FADE_OUT ) ;
			}
			break ;
		}
		break ;
	case 6 :
		if( !fade_cnt() ){
			Sys->routine_0 = 1 ;
			Sys->routine_2 = 0 ;
			Sys->routine_3 = 0 ;
			Sys->exit_flag = 0 ;
			vms_work.vm_msg[0] = 0 ;
		}
		break ;
	}
}

/****************************************************************/
/*			SAVE: to the card chosen							*/
/****************************************************************/
void vm_save( Uint16 sw )
{
	int ret ;
	int port ;
	int bit ;

	if( 0 ) if( decide_cancel_ck() ) Sys->exit_flag = 2 ;	/* left out of the build */
	port = vms_work.cursor_port ;
	switch( Sys->routine_3 ){
	case 0 :
		if( vms_card_sys_ck( port ) ){
			Sys->routine_3 = 1 ;
			vms_work.vm_msg[0] = 6 ;
			vms_work.vm_msg[1] = 1 ;
			dm10_vm_msg_set() ;
		} else Sys->routine_3 = 2 ;
		Sys->exit_flag = 0 ;
		return ;
		break ;	/* left out of the build */
	case 1 :
		ret = vms_card_ck( port ) ;
		if( ret == 0 || ret == -1 ) goto back ;
		switch( Sys->exit_flag ){
		case 0 :
			vms_work.vm_msg[1] = ud_select_loop( sw, vms_work.vm_msg[1], 1 ) ;
			if( decide_ck() ){
				if( !vms_work.vm_msg[1] ) Sys->exit_flag = 1 ;
				else Sys->exit_flag = 2 ;
				vms_work.vm_msg[1] |= 0x80 ;
			} else if( cancel_ck() ){
				vms_work.vm_msg[1] = 0xff ;
				Sys->exit_flag = 2 ;
			}
			break ;
		case 1 :
			if( !vms_work.vm_msg[1] ) goto save ;
			else break ;
		case 2 :
			if( !vms_work.vm_msg[1] ) goto back ;
			break ;
		}
		break ;
	case 2 :
		bit = vms_check_free_data() ;
		if( bit & ( 1 << vms_work.cursor_port ) ){
		save :
			Sys->routine_3 = 4 ;
			Sys->exit_flag = 0 ;
			vms_work.vm_msg[0] = 7 ;
			dm10_vm_msg_set() ;
			return ;
		}
		Sys->routine_3 = 3 ;
		Sys->exit_flag = 0 ;
		vms_work.vm_msg[0] = 3 ;
		dm10_vm_msg_set() ;
		break ;
	case 3 :
		switch( Sys->exit_flag ){
		case 0 :
			if( decide_cancel_ck() ){
				Sys->exit_flag ++ ;
				vms_work.vm_msg[0] = 4 ;
				dm10_vm_msg_set() ;
			}
			break ;
		case 1 :
			if( decide_cancel_ck() ){
				na_req( 3 ) ;
				goto back ;
			}
			break ;
		}
		break ;
		if( 0 ) ;	/* left out of the build */
	case 4 :
		vms_work.save_busy = 1 ;
		ret = vms_data_save( 1, vms_work.cursor_port ) ;
		if( ret == -1 ) break ;
		if( ret ){
			vms_work.save_busy = 0 ;
			Sys->routine_3 = 7 ;
			if( vms_work.file_port == vms_work.cursor_port && vms_work.file_flag == 1 ){
				vms_work.vm_msg[0] = 10 ;
				vms_work.file_flag = 0 ;
			} else vms_work.vm_msg[0] = 11 ;
			dm10_vm_msg_set() ;
			break ;
		}
		bit = vms_check_execfile() ;
		if( bit & ( 1 << vms_work.cursor_port ) ){
			Sys->routine_3 ++ ;
			break ;
		}
		vms_work.save_busy = 0 ;
		Sys->routine_3 = 6 ;
		goto done ;
	case 5 :
		vms_work.save_busy = 1 ;
		ret = vms_exec_resave( vms_work.cursor_port, 0 ) ;
		if( ret == -1 ) break ;
		if( ret ) goto fail ;
		vms_work.save_busy = 0 ;
		Sys->routine_3 ++ ;
	done :
		if( vms_work.file_port == vms_work.cursor_port && vms_work.file_flag == 1 ){
			Sys->exit_flag = 2 ;
			vms_work.vm_msg[0] = 8 ;
		} else {
			Sys->exit_flag = 0 ;
			vms_work.vm_msg[0] = 9 ;
			vms_work.vm_msg[1] = 1 ;
		}
		dm10_vm_msg_set() ;
		break ;
	fail :
		vms_work.save_busy = 0 ;
		Sys->routine_3 = 7 ;
		if( vms_work.file_port == vms_work.cursor_port && vms_work.file_flag == 1 ){
			vms_work.vm_msg[0] = 10 ;
			vms_work.file_flag = 0 ;
		} else vms_work.vm_msg[0] = 11 ;
		dm10_vm_msg_set() ;
		break ;
	case 6 :
		switch( Sys->exit_flag ){
		case 0 :
			vms_work.vm_msg[1] = ud_select_loop( sw, vms_work.vm_msg[1], 1 ) ;
			if( decide_ck() ){
				if( !vms_work.vm_msg[1] ){
					vms_work.file_port = vms_work.cursor_port ;
					vms_work.file_flag = 1 ;
					vms_date_keep( vms_work.file_port ) ;
				}
				Sys->exit_flag = 1 ;
				vms_work.vm_msg[1] |= 0x80 ;
				return ;
			} else if( cancel_ck() ){
				Sys->exit_flag = 1 ;
				vms_work.vm_msg[1] = 0xff ;
			}
			break ;
		case 1 :
			if( !vms_work.vm_msg[1] ){
				Sys->routine_3 = 8 ;
				fade_set( 0xff000000, 10, FADE_OUT ) ;
			}
			break ;
		case 2 :
			if( !decide_cancel_ck() ) break ;
			na_req( 3 ) ;
			Sys->routine_3 = 8 ;
			fade_set( 0xff000000, 10, FADE_OUT ) ;
			break ;
		}
		break ;
	case 7 :
		if( decide_cancel_ck() ){
			na_req( 3 ) ;
			goto back ;
		}
		break ;
	case 8 :
		if( !fade_cnt() ){
			Sys->routine_0 = 1 ;
			Sys->routine_2 = 0 ;
			Sys->routine_3 = 0 ;
			Sys->exit_flag = 0 ;
			vms_work.vm_msg[0] = 0 ;
		}
		break ;
	back :
		Sys->routine_2 = 1 ;
		Sys->routine_3 = 0 ;
		Sys->exit_flag = 0 ;
		Sys->timer1 = 0 ;
		vms_work.vm_msg[0] = 1 ;
		dm10_vm_msg_set() ;
		break ;
	}
}

/****************************************************************/
/*			LOAD: the card to load from							*/
/****************************************************************/
void vm_load_port( Uint16 sw )
{
	vms_card_bit_set() ;
	switch( Sys->routine_2 ){
	case 0 :
		Sys->routine_2 ++ ;
		vms_work.cursor_port = -1 ;
		work_list_kill( 11 ) ;
		dm10_set( 1 ) ;
		dm0c_wall_paper( 8 ) ;
		fade_set( 0xff000000, 10, FADE_IN ) ;
		break ;
	case 1 :
		if( !fade_cnt() ){
			if( vms_port_first() ){
				Sys->routine_2 ++ ;
				Sys->routine_2 = 2 ;
				vms_work.vm_msg[0] = 12 ;
				dm10_vm_msg_set() ;
			} else {
				Sys->routine_2 = 4 ;
				Sys->routine_3 = 0 ;
				vms_work.vm_msg[0] = 2 ;
				dm10_vm_msg_set() ;
			}
		}
		break ;
	case 2 :
		if( !vms_work.card_bit ){
			Sys->routine_2 = 4 ;
			Sys->routine_3 = 0 ;
			vms_work.vm_msg[0] = 2 ;
			dm10_vm_msg_set() ;
			vms_work.cursor_port = -1 ;
			break ;
		}
		vms_port_move( sw ) ;
		if( decide_ck() ){
			Sys->routine_2 = 3 ;
			Sys->routine_3 = 0 ;
			return ;
		} else if( cancel_ck() ){
			Sys->routine_2 = 5 ;
			fade_set( 0xff000000, 10, FADE_OUT ) ;
		}
		break ;
	case 3 :
		vm_load( sw ) ;
		break ;
	case 4 :
		if( vms_work.card_bit != 0 ){
			Sys->routine_2 = 2 ;
			vms_work.vm_msg[0] = 12 ;
			dm10_vm_msg_set() ;
			vms_port_first() ;
		}
		if( decide_cancel_ck() ){
			na_req( 3 ) ;
			Sys->routine_2 = 5 ;
			fade_set( 0xff000000, 10, FADE_OUT ) ;
		}
		break ;
	case 5 :
		if( !fade_cnt() ){
			Sys->routine_0 = 1 ;
			Sys->routine_2 = 0 ;
			Sys->routine_3 = 0 ;
			Sys->exit_flag = 0 ;
		}
		break ;
	}
}

/****************************************************************/
/*			LOAD: from the card chosen							*/
/****************************************************************/
void vm_load( register Uint16 sw )
{
	int ret ;
	int port ;

	port = vms_work.cursor_port ;
	switch( Sys->routine_3 ){
	case 0 :
		Sys->routine_3 ++ ;
		Sys->exit_flag = 0 ;
		vms_work.disp_adjust_req = 0 ;
		break ;
	case 1 :
		switch( Sys->exit_flag ){
		case 0 :
			if( vms_card_sys_ck( port ) ){
				Sys->routine_3 = 2 ;
				Sys->exit_flag = 0 ;
				vms_work.vm_msg[0] = 15 ;
				dm10_vm_msg_set() ;
				vms_work.card_task_step[0][0] = 0 ;
				return ;
			} else {
				Sys->exit_flag = 1 ;
				vms_work.vm_msg[0] = 14 ;
				dm10_vm_msg_set() ;
			}
			break ;
		case 1 :
			if( decide_cancel_ck() ){
				na_req( 3 ) ;
				goto back ;
			}
			break ;
		}
		break ;
	case 2 :
		vms_work.card_done_bit = 1 ;
		ret = vms_task_load_all( vms_work.cursor_port, 0 ) ;
		if( ret == 1 ){
			vms_work.card_done_bit = 0 ;
			Sys->routine_3 ++ ;
		} else if( ret == 0 ){
			vms_work.card_done_bit = 0 ;
			Sys->routine_3 = 4 ;
		} else break ;
		Sys->exit_flag = 0 ;
		return ;
	case 3 :
		switch( Sys->exit_flag ){
		case 0 :
			if( vms_work.file_flag == 1 && vms_work.file_port == port ){
				Sys->exit_flag = 4 ;
				vms_work.vm_msg[0] = 31 ;
			} else {
				Sys->exit_flag = 1 ;
				vms_work.vm_msg[0] = 16 ;
				vms_work.vm_msg[1] = 1 ;
			}
			dm10_vm_msg_set() ;
			break ;
		case 1 :
			vms_work.vm_msg[1] = ud_select_loop( sw, vms_work.vm_msg[1], 1 ) ;
			if( decide_ck() ){
				if( !vms_work.vm_msg[1] ) Sys->exit_flag = 2 ;
				else Sys->exit_flag = 3 ;
				vms_work.vm_msg[1] |= 0x80 ;
				return ;
			} else if( cancel_ck() ){
				Sys->exit_flag = 3 ;
				vms_work.vm_msg[1] = 0xff ;
			}
			break ;
		case 2 :
			if( !vms_work.vm_msg[1] ){
				vms_work.file_flag = 1 ;
				vms_work.file_port = vms_work.cursor_port ;
				vms_date_keep( vms_work.file_port ) ;
				Sys->routine_3 = 5 ;
				fade_set( 0xff000000, 10, FADE_OUT ) ;
			}
			break ;
		case 3 :
			if( !vms_work.vm_msg[1] ){
				Sys->routine_3 = 5 ;
				fade_set( 0xff000000, 10, FADE_OUT ) ;
				return ;
			}
			break ;
		case 4 :
			if( decide_cancel_ck() ){
				na_req( 3 ) ;
				Sys->routine_3 = 5 ;
				fade_set( 0xff000000, 10, FADE_OUT ) ;
			}
			break ;
		}
		break ;
	case 4 :
		switch( Sys->exit_flag ){
		case 0 :
			Sys->exit_flag ++ ;
			if( vms_work.file_port == vms_work.cursor_port && vms_work.file_flag == 1 ){
				vms_work.vm_msg[0] = 17 ;
				vms_work.file_flag = 0 ;
			} else vms_work.vm_msg[0] = 18 ;
			dm10_vm_msg_set() ;
			break ;
		case 1 :
			if( decide_cancel_ck() ) goto back ;
			break ;
		}
		break ;
	case 5 :
		if( !fade_cnt() ){
			Sys->routine_0 = 1 ;
			Sys->routine_2 = 0 ;
			Sys->routine_3 = 0 ;
			Sys->exit_flag = 0 ;
			vms_work.vm_msg[0] = 0 ;
			if( vms_work.disp_adjust_req ){
				vms_work.disp_adjust_req = 0 ;
				disp_adjust_set() ;
			}
		}
		break ;
	back :
		Sys->routine_2 = 1 ;
		Sys->routine_3 = 0 ;
		Sys->exit_flag = 0 ;
		Sys->timer1 = 0 ;
		vms_work.vm_msg[0] = 12 ;
		dm10_vm_msg_set() ;
		break ;
	}
}

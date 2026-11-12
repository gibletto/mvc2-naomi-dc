/* The home game's SECRET FACTOR, in secret.c, secret_1.c to secret_3.c, secret_vm.c and mode8.c:
   its steps, the card menus and what they call. */
#ifndef SECRET_H
#define SECRET_H

extern void dm0c_wall_paper( int no ) ;
extern void home_menu_set( void ) ;
extern void dm0c_vm_win_set( int st ) ;
extern void dm0c_vm_msg_set( void ) ;
extern void dm0f_menu_set( void ) ;
extern void dm0f_part1_set( void ) ;
extern void dm0f_part2_set( void ) ;
extern void dm0f_part3_set( void ) ;
extern void dm0f_point_set( void ) ;
extern void dm0f_page2_set( int no ) ;
extern PLAYER_WORK *dm0f_page1_set( int flag ) ;
extern void dm0f_page1_icon_set( void ) ;
extern PLAYER_WORK *dm0f_30_set( void ) ;
extern void dm0f_30_pts_set( void ) ;
extern void dm0f_30_shop1_set( int flag, int goods, Sint16 pts ) ;
extern void dm0f_30_shop2_set( int no, int goods, Sint16 *pts ) ;
extern PLAYER_WORK *dm0f_30_subwin_set( void ) ;
extern PLAYER_WORK *dm0f_30_cursor_set( void ) ;
extern PLAYER_WORK *dm0f_30_msg_set( void ) ;
extern void dm0f_30_goods_off( int no ) ;
extern void dm0f_30_shop_init( void ) ;
extern void dm0f_save_set( void ) ;
extern void dm0f_vm_msg_set( void ) ;
extern void dm0f_port_set( void ) ;
extern void dm_obj_set( int no, OBJ_FILE *file ) ;
extern void dm10_set( int no ) ;
extern void dm10_vm_msg_set( void ) ;
extern void dm0f_mode8_set( void ) ;
extern void dm0f_mode8_sheet_set( void ) ;
extern void dm0f_mode8_frame_set( void ) ;
extern void dm0f_mode8_title_set( void ) ;
extern void headline_sub_set( int no ) ;
extern void disp_adjust_set( void ) ;
extern void d_point_get( void ) ;
extern void n_point_get( void ) ;
extern int decide_ck( void ) ;
extern int cancel_ck( void ) ;
extern int decide_cancel_ck( void ) ;
extern int decide_ck_nose( void ) ;
extern int cancel_ck_nose( void ) ;
extern void com_se_req( Uint8 no ) ;
extern void vms_card_bit_set( void ) ;
extern int vms_port_first( void ) ;
extern int ud_select_loop( Uint16 sw, int no, int max ) ;
extern int ud_select_stop( Uint16 sw, int no, int max ) ;
extern int vms_card_ck( int port ) ;
extern int vms_card_sys_ck( int port ) ;
extern int vms_check_execfile( void ) ;
extern int vms_check_other_execfile( void ) ;
extern int vms_check_free_data( void ) ;
extern int vms_check_free_exec( void ) ;
extern int vms_data_save( Sint8 type, int port ) ;
extern int vms_exec_save( int port ) ;
extern int vms_exec_resave( int port, Uint8 no ) ;


void secret_factor_0( Uint16 sw, Uint16 sw2 ) ;
void secret_factor_1( Uint16 sw, Uint16 sw2 ) ;
void secret_factor_2( Uint16 sw, Uint16 sw2 ) ;
void secret_factor_3( Uint16 sw, Uint16 sw2 ) ;
void secret_factor_3_port( Uint16 sw ) ;
void secret_factor_3_save( Uint16 sw ) ;
void vm_save_port( Uint16 sw ) ;
void vm_save( Uint16 sw ) ;
void vm_load_port( Uint16 sw ) ;
void vm_load( Uint16 sw ) ;
void mode8_view( Uint16 sw ) ;

#endif

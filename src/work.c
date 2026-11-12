/* The game's global work: the I/O boards and the settings, the library's buffers, the system's,
   the players' and the stage's works, and the work pool and its lists. A file of data alone,
   linked after task_init; work.h declares it in its order. */
#include "naomi.h"
#include "shinobi.h"
#include "player.h"

#ifdef TARGET_DC
/* the home system's: the peripheral library's buffers, the controls, the options and the modes */
Uint8 peri_recv_buff[0xc020] ;
Uint8 peri_send_buff[0xc020] ;
PDS_PERIPHERAL *port_peri[4] ;
const PDS_KEYBOARD *kbd_data ;
Sint32 reset_lock ;
Sint32 exit_req ;
OPTION_WORK option_w ;
SECRET_WORK CS_Secret_Work ;
Uint8 unk_8C212C52[0x4a] ;
PAUSE_WORK pause_work ;
Sint8 conf_stereo, conf_difficulty, conf_round_time, conf_damage_level, conf_speed_fix ;
Uint8 unk_8C212CC1[11] ;
TRAINING_WORK training_work ;
RANK_DATA rank_sa[21] ;
BTN_WORK btn_work[2] ;
Uint16 kbd_now, kbd_old, kbd_on, kbd_rep ;

/* the network's */
Uint8 unk_8C212F0C, unk_8C212F0D, unk_8C212F0E, unk_8C212F0F ;
Sint16 unk_8C212F10 ;
Uint8 unk_8C212F12[2] ;
Sint16 yesno_ans ;
Sint16 unk_8C212F16 ;
Sint8 rec_item_no, net_cursor_no ;
Uint8 unk_8C212F1A ;
Sint8 unk_8C212F1B ;
Sint8 board_cursor_no[2] ;
Sint8 board_decide[2] ;
Sint8 unk_8C212F20 ;
Sint8 net_info_flag ;
Sint16 win_pict_u, win_pict_v ;
SW_WORK *side_sw_adrs[2] ;
NET_WORK net_w ;
Uint32 net_mdm_hn ;
Uint8 unk_8C212FB4[0x90] ;
Sint8 net_side ;
Uint8 unk_8C213045 ;
Sint8 net_play_flag ;
Uint8 unk_8C213047 ;
Sint8 unk_8C213048, unk_8C213049 ;
Uint8 unk_8C21304A[0x280] ;
Sint8 unk_8C2132CA, unk_8C2132CB ;
Uint8 unk_8C2132CC[0x200] ;
Sint8 unk_8C2134CC, unk_8C2134CD, unk_8C2134CE ;
Uint8 unk_8C2134CF[0x201] ;
Sint8 unk_8C2136D0, unk_8C2136D1 ;
Uint8 unk_8C2136D2[0x2a] ;
Uint8 unk_8C2136FC ;
Sint8 unk_8C2136FD, unk_8C2136FE ;
Uint8 unk_8C2136FF, unk_8C213700 ;
Uint8 unk_8C213701[4] ;
Sint8 unk_8C213705 ;
Uint8 unk_8C213706[0x100] ;
char net_user_id[0x1c] ;
char net_password[0x14] ;
Uint8 net_board_side ;
Uint8 unk_8C213837[0xb] ;
Uint8 net_file_key[8] ;
Uint8 net_handle[18] ;
Uint8 unk_8C21385C[14] ;
Uint8 unk_8C21386A[0x80] ;
Uint8 unk_8C2138EA[0x80] ;
Uint8 unk_8C21396A[0x40] ;
Uint8 unk_8C2139AA[0x80] ;
Uint8 unk_8C213A2A[0x40] ;
Uint8 unk_8C213A6A[10] ;
Uint8 unk_8C213A74[118] ;
Uint8 unk_8C213AEA[4] ;
Sint16 unk_8C213AEE ;
Sint16 net_win_add ;
Sint16 net_lose_add ;
Uint8 unk_8C213AF4[4] ;
Uint32 unk_8C213AF8, unk_8C213AFC, net_entry_num ;
Sint32 net_play_time ;
Sint8 net_rank ;
Sint32 unk_8C213B0C ;
Uint8 unk_8C213B10[0x80] ;
Sint16 net_win_cnt ;
Sint16 net_lose_cnt ;
Uint8 unk_8C213B94[0x114] ;
ROOM_INFO room_info[10] ;
Uint8 unk_8C2141E4 ;
Sint8 net_area_no ;
Uint8 unk_8C2141E6 ;
Sint8 room_info_cnt, unk_8C2141E8 ;
Sint8 net_room_no ;
char net_vs_key[8] ;
char net_vs_handle[0x92] ;
Sint8 net_vs_rank ;
Uint8 unk_8C214285 ;
Sint16 net_vs_win_cnt ;
Sint16 net_vs_lose_cnt ;
Uint8 unk_8C21428A[2] ;
Sint16 net_vs_win_add ;
Sint16 net_vs_lose_add ;
Uint8 unk_8C214290[2] ;
Uint8 unk_8C214292 ;
Uint8 unk_8C214293 ;
Uint8 unk_8C214294 ;
Uint8 unk_8C214295[7] ;
Sint32 unk_8C21429C ;
Uint8 unk_8C2142A0[7] ;
Sint8 net_pad_bit ;
Uint8 unk_8C2142A8[0x106] ;
Uint8 unk_8C2143AE ;
Uint8 unk_8C2143AF[0x13] ;
Sint8 unk_8C2143C2 ;
Uint8 unk_8C2143C3 ;
Sint16 unk_8C2143C4 ;
NET_ENTRY net_key_list[50] ;
Uint8 unk_8C2148DA[0x33c] ;
NET_REC net_rec[2] ;
Uint8 unk_8C215068[0xbc] ;
#endif

NLA_SYSTEM_FLAG *sys_flag ;
NLA_CREDIT_CONF *credit_conf ;
NLA_CREDIT_INFO credit_info ;
int jamma_cnt ;
int jamma_err ;
int jamma_err_timer ;
Sint32 jamma_err_cnt1[8] ;
Sint32 jamma_err_cnt2[8] ;
int book_save ;
char credit_str1[32], credit_str2[32] ;
Uint32 coin_old ;
Uint32 service_old ;
Uint32 unk_0C283F78 ;
Uint32 peri_bit ;
SYS_SET sys_set ;
#ifndef TARGET_DC
BOOK_WORK book ;
Sint8 conf_country ;
Sint8 conf_stereo ;
Sint8 conf_demo_sound_half, conf_cont_enable, conf_voice_type, conf_difficulty ;
Sint8 conf_time_speed ;
Sint8 conf_damage_level ;
Sint8 conf_vms_mode ;
Sint8 conf_vm_write, conf_speed_fix, conf_event_flag, conf_no_challenge_flag, conf_open_lv ;
SW_WORK sys_sw ;
SW_WORK board_sw ;
PDS_PERIPHERAL *pad_peri[2] ;
#else
Uint8 unk_8C21524C[0x32] ;
char net_entry_str[12] ;	/* the lobby's entry number, as text */
Sint8 lobby_time_flag ;
Uint32 lobby_time ;
Uint8 unk_8C215290[4] ;
Uint32 unk_8C215294 ;
Uint8 unk_8C215298[4] ;
Uint16 log_scroll ;
Uint8 unk_8C21529E[2] ;
Sint16 unk_8C2152A0, unk_8C2152A2 ;
Uint8 unk_8C2152A4[2] ;
Uint16 button_cfg[2][6] ;	/* each side's button config */
Uint8 unk_8C2152BE[14] ;
#endif
Sint32 task_wait ;
Sint32 vsync_count ;
Sint32 vsync_flag ;
Sint32 SortMode ;
MATRIX matrix_stack[64] ;
Uint8 unk_0C285024[64] ;

Sint32 common_tex_work[54480] ;
Sint32 sprite_work[26720] ;
Sint32 palette_work[128] ;

COL_WORK col_work[128] ;
Uint16 col_send_buff[2048] ;
SW_WORK sw_work[4] ;
VIB_WORK vib_w[2] ;
SYSTEM_WORK *Sys ;
SYSTEM_WORK sys_work ;
PLAYER_WORK pl_work[6] ;
SCR_WORK scr_work ;
FOG_WORK fog_work ;
BRIGHT_WORK bright_work ;
BACK_WORK back_work ;
STAGE_WORK stage_work ;
FADE_WORK fade_work ;
FLASH_WORK flash_work ;
LIGHT_WORK light_work ;

OBJ_FILE *obj_stg ;
OBJ_FILE *obj_efky ;
OBJ_FILE *obj_dm00 ;
OBJ_FILE *obj_dm01 ;
OBJ_FILE *obj_dm02 ;
OBJ_FILE *obj_dm03 ;
OBJ_FILE *obj_dm04 ;
OBJ_FILE *obj_dm05 ;
OBJ_FILE *obj_dm06 ;
OBJ_FILE *obj_dm07 ;
OBJ_FILE *obj_dm08 ;
OBJ_FILE *obj_dm09 ;
OBJ_FILE *obj_dm0a ;
OBJ_FILE *obj_dm0b ;
OBJ_FILE *obj_dm0c ;
#ifndef TARGET_DC
Uint8 unk_0C2D9688[28] ;
#else
OBJ_FILE *obj_dm0d ;
OBJ_FILE *obj_dm0f ;
OBJ_FILE *obj_dm10 ;
OBJ_FILE *obj_dm11 ;
OBJ_FILE *obj_dm12 ;
Uint8 unk_8C26A954[8] ;
#endif

Uint8 stage_no ;
Sint8 dbg_mode ;
Uint8 unk_0C2D96A6 ;
Sint8 sprite_keep_flag ;
Sint8 tex_req_flag ;
Sint8 tex_req_no ;
Uint8 rom_dma_flag ;
Uint8 stream_start_flag ;
#ifndef TARGET_DC
Uint8 unk_0C2D96AC[16] ;
#else
Uint8 unk_8C26A964, unk_8C26A965, unk_8C26A966 ;
Uint8 unk_8C26A967[13] ;
#endif
Float prio_z[16] ;
Uint8 unk_0C2D96FC[112] ;
Sint16 spr_set_tex_cnt[8] ;
Sint32 spr_set_size[8] ;

WORK_BUFF work_buff[WORK_MAX] ;
PLAYER_WORK *free_top ;
PLAYER_WORK *free_end ;
PLAYER_WORK *work_top[LIST_MAX] ;
PLAYER_WORK *work_end[LIST_MAX] ;
Sint16 work_cnt[LIST_MAX] ;
Sint16 work_rest_cnt ;
Sint16 free_cnt ;
PLAYER_WORK *hit_work_w[2][HIT_W_MAX] ;
Sint8 hit_work_cnt[2] ;
Sint8 hit_work_old[2] ;
PLAYER_WORK *disp_prio_w[PRIO_MAX][PRIO_W_MAX] ;
Sint8 disp_prio_cnt[PRIO_MAX] ;

VS_SYS_WORK vs_sys_work ;
VS_SYS_WORK *vs_sys_w ;
Sint8 dbg_flag[60] ;
Float obj_col_buff[4][4] ;
CPU_LEVEL_WORK cpu_level_work ;
VMS_WORK vms_work ;
RANK_DATA rank_score[21] ;
RANK_DATA rank_win[21] ;
Sint16 bg_ang0, bg_ang1, bg_ang2, bg_ang3 ;
void ( *pl_tbl_buff[6][32] )( PLAYER_WORK *pl_w ) ;
Uint8 unk_0C2F8C20[160] ;

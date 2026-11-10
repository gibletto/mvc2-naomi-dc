/* The game's global work: the I/O boards and the settings, the library's buffers, the system's,
   the players' and the stage's works, and the work pool and its lists. A file of data alone,
   linked after task_init; work.h declares it in its order. */
#include "naomi.h"
#include "shinobi.h"
#include "player.h"

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
Uint8 unk_0C2D9688[28] ;

Uint8 stage_no ;
Sint8 dbg_mode ;
Uint8 unk_0C2D96A6 ;
Sint8 sprite_keep_flag ;
Sint8 tex_req_flag ;
Sint8 tex_req_no ;
Uint8 rom_dma_flag ;
Uint8 unk_0C2D96AB ;
Uint8 unk_0C2D96AC[16] ;
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
Uint8 unk_0C2F8500[40] ;
RANK_DATA rank_score[21] ;
RANK_DATA rank_win[21] ;
Sint16 bg_ang0, bg_ang1, bg_ang2, bg_ang3 ;
void ( *pl_tbl_buff[6][32] )( PLAYER_WORK *pl_w ) ;
Uint8 unk_0C2F8C20[160] ;

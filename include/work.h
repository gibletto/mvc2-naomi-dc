/* The game's global work (work.c), declared in the order work.c lays it out. */
#ifndef WORK_H
#define WORK_H

#include "book.h"

/* the I/O boards (sys_flag, credit_conf and credit_info come first: naomi.h) */
extern int jamma_cnt;	/* the I/O boards found */
extern int jamma_err;	/* the I/O board's error: 1 none found, 2 too few switches or coin slots */
extern int jamma_err_timer;	/* frames until the I/O boards' errors are checked */
extern Sint32 jamma_err_cnt1[8];	/* the first board's error counts at the start */
extern Sint32 jamma_err_cnt2[8];	/* the second's */
extern int book_save;		/* the book-keeping is to be saved, 1 for a coin */
extern char credit_str1[32], credit_str2[32];	/* each side's credit display, from credit_str_set */
extern Uint32 coin_old;	/* the coins counted so far */
extern Uint32 service_old;	/* the service credits counted so far */
extern Uint32 unk_0C283F78;
extern Uint32 peri_bit;	/* what is connected, from the test program's peri_ck: bits 0 and 1 each side's pad; 2 and 3 a memory card or vibration pack in each side's first socket, 4 and 5 in its second */
extern SYS_SET sys_set;
extern BOOK_WORK book;

/* the game's settings, as the test menu leaves them in the EEPROM; System_Init copies them to Sys */
extern Sint8 conf_country;	/* the country */
extern Sint8 conf_stereo;	/* Sys->stereo */
extern Sint8 conf_demo_sound_half, conf_cont_enable, conf_voice_type, conf_difficulty;
extern Sint8 conf_time_speed;	/* Sys->time_speed */
extern Sint8 conf_damage_level;	/* Sys->damage_level */
extern Sint8 conf_vms_mode;	/* 2 with a Dreamcast pad */
extern Sint8 conf_vm_write, conf_speed_fix, conf_event_flag, conf_no_challenge_flag, conf_open_lv;

extern SW_WORK sys_sw;		/* the system switches of the I/O board: test and service */
extern SW_WORK board_sw;	/* the switches of the NAOMI board itself */
extern struct PDS_PERIPHERAL *pad_peri[2];	/* each side's pad */
extern Sint32 task_wait;	/* frames before the task runs again, after a reset or the test button */
extern Sint32 vsync_count;	/* the vertical blanks since the frame began */
extern Sint32 vsync_flag;	/* set by the vertical blank */
extern Sint32 SortMode;
extern MATRIX matrix_stack[64];	/* the stack nlInitMatrix is given */
extern Uint8 unk_0C285024[64];

/* the library's common textures, sprites and palettes */
extern Sint32 common_tex_work[54480];
extern Sint32 sprite_work[26720];
extern Sint32 palette_work[128];

extern COL_WORK col_work[];
extern Uint16 col_send_buff[2048];
extern SW_WORK sw_work[4];
extern VIB_WORK vib_w[2];
extern SYSTEM_WORK *Sys;
extern SYSTEM_WORK sys_work;
extern PLAYER_WORK pl_work[6];
extern SCR_WORK scr_work;
extern FOG_WORK fog_work;
extern BRIGHT_WORK bright_work;
extern BACK_WORK back_work;
extern STAGE_WORK stage_work;
extern FADE_WORK fade_work;
extern FLASH_WORK flash_work;
extern LIGHT_WORK light_work;

/* the model files, named as the model viewer lists them: the stage's (stg00 to stg08, one at a
   time), the common effects' (efky) and those of the scenes outside the fight (dm00 to dm0c) */
extern OBJ_FILE *obj_stg ;
extern OBJ_FILE *obj_efky ;
extern OBJ_FILE *obj_dm00 ;
extern OBJ_FILE *obj_dm01 ;
extern OBJ_FILE *obj_dm02 ;
extern OBJ_FILE *obj_dm03 ;
extern OBJ_FILE *obj_dm04 ;
extern OBJ_FILE *obj_dm05 ;
extern OBJ_FILE *obj_dm06 ;
extern OBJ_FILE *obj_dm07 ;
extern OBJ_FILE *obj_dm08 ;
extern OBJ_FILE *obj_dm09 ;
extern OBJ_FILE *obj_dm0a ;
extern OBJ_FILE *obj_dm0b ;
extern OBJ_FILE *obj_dm0c ;
extern Uint8 unk_0C2D9688[28];

extern Uint8 stage_no;		/* the stage; 8 a special one */
extern Sint8 dbg_mode;		/* 1 while the debug switch menu is up, 4 while the model viewer runs */
extern Uint8 unk_0C2D96A6;
extern Sint8 sprite_keep_flag;	/* while set, sprite_init leaves the sprites as they are */
extern Sint8 tex_req_flag;	/* a texture send is asked for */
extern Sint8 tex_req_no;	/* which one, 1 to 29 */
extern Uint8 rom_dma_flag;	/* set while a file is read from the ROM; the controls aren't read */
extern Uint8 unk_0C2D96AB;
extern Uint8 unk_0C2D96AC[16];
extern Float prio_z[16];	/* each draw priority's depth, a little more for each sprite drawn */
extern Uint8 unk_0C2D96FC[112];
extern Sint16 spr_set_tex_cnt[8];	/* the textures of each loaded sprite set */
extern Sint32 spr_set_size[8];	/* and their bytes */

/* the work pool, its lists and the draw lists (hit_work.c) */
extern WORK_BUFF work_buff[WORK_MAX];
extern PLAYER_WORK *free_top;	/* the free works, taken from the top */
extern PLAYER_WORK *free_end;
extern PLAYER_WORK *work_top[LIST_MAX];	/* each list, from its first work */
extern PLAYER_WORK *work_end[LIST_MAX];	/* and from its last */
extern Sint16 work_cnt[LIST_MAX];
extern Sint16 work_rest_cnt;
extern Sint16 free_cnt;
extern PLAYER_WORK *hit_work_w[2][HIT_W_MAX];	/* the works each side entered this frame */
extern Sint8 hit_work_cnt[2];
extern Sint8 hit_work_old[2];	/* the counts of the frame before */
extern PLAYER_WORK *disp_prio_w[PRIO_MAX][PRIO_W_MAX];
extern Sint8 disp_prio_cnt[PRIO_MAX];

extern VS_SYS_WORK vs_sys_work;
extern VS_SYS_WORK *vs_sys_w;	/* always &vs_sys_work */
extern Sint8 dbg_flag[60];	/* the debug switches, row * 4 + column */
extern Float obj_col_buff[4][4];
extern CPU_LEVEL_WORK cpu_level_work;
extern VMS_WORK vms_work;
extern Uint8 unk_0C2F8500[40];
extern RANK_DATA rank_score[21];	/* the first ranking, by score */
extern RANK_DATA rank_win[21];	/* the second, by wins in a row against another player */
extern Sint16 bg_ang0, bg_ang1, bg_ang2, bg_ang3;	/* angles the stage and gauges turn by, stepped each frame */
extern void ( *pl_tbl_buff[6][32] )( PLAYER_WORK *pl_w );	/* the routine tables pl_char_set and the character modules fill, one for each work */
extern Uint8 unk_0C2F8C20[160];

#endif

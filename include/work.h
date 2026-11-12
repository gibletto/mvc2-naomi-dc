/* The game's global work (work.c), declared in the order work.c lays it out. */
#ifndef WORK_H
#define WORK_H

#include "book.h"

#ifdef TARGET_DC
#include "naomi.h"

/* a player's profile from the network: text fields, each empty when its first byte is 0 */
typedef struct NET_REC {
	char	name[128];	/* 000: the name, up to 18 bytes */
	char	unk_080[128];	/* 080: up to 34 bytes */
	char	unk_100[64];	/* 100: up to 36 bytes */
	char	unk_140[64];	/* 140: up to 40 bytes */
	char	post_code[10];	/* 180: the postcode, seven digits shown as three and four */
	char	unk_18a[118];	/* 18a: up to 28 bytes */
	char	handle[18];	/* 200: the name the player goes by, up to 16 bytes */
	char	age[4];		/* 212: up to two digits */
	char	entry_str[10];	/* 216: the entry number, as net_entry_str */
	Sint8	unk_220;	/* 220 */
	Sint8	unk_221[8];
} NET_REC;

/* a room as the server lists it */
typedef struct ROOM_INFO {
	Uint8	unk_00[2];
	Sint8	open_flag;	/* 002: shown as a circle when set, a cross when not */
	Uint8	unk_03;
	Sint16	member_cnt;	/* 004: the players in it */
	char	name[128];	/* 006 */
} ROOM_INFO;

/* the home system's work, which comes first: the peripheral library's buffers, the console's
   controls (src/dc/system.c reads them each vertical blank), the options and the modes */
extern Uint8 peri_recv_buff[0xc020];
extern Uint8 peri_send_buff[0xc020];
extern struct PDS_PERIPHERAL *port_peri[4];	/* the pad of each of the console's ports */
extern const struct PDS_KEYBOARD *kbd_data;	/* port D's keyboard, or NULL */
extern Sint32 reset_lock;	/* while set, the reset keys do nothing */
extern Sint32 exit_req;	/* 1: go back to the console's menu (the door was opened, the disc gone, or the reset keys held in the attract) */
extern OPTION_WORK option_w;
extern SECRET_WORK CS_Secret_Work;
extern Uint8 unk_8C212C52[0x4a];
extern PAUSE_WORK pause_work;
extern Sint8 conf_stereo, conf_difficulty, conf_round_time, conf_damage_level, conf_speed_fix;	/* the option menu's, which sys_set_load copies to Sys */
extern Uint8 unk_8C212CC1[11];
extern TRAINING_WORK training_work;
extern RANK_DATA rank_sa[21];	/* the home game's third ranking, by score in the score attack */
extern BTN_WORK btn_work[2];
extern Uint16 kbd_now, kbd_old, kbd_on;	/* the keyboard as a pad's keys: held, held the frame before, newly pressed */
extern Uint16 kbd_rep;	/* and given again while held */

/* the network's */
extern Uint8 unk_8C212F0C, unk_8C212F0D, unk_8C212F0E, unk_8C212F0F;	/* cleared when a network screen starts */
extern Sint16 unk_8C212F10;	/* bit 7 shows the lobby window's mark */
extern Uint8 unk_8C212F12[2];
extern Sint16 yesno_ans;	/* the yes/no bar's answer, 0 or 1; -1 takes the bar away */
extern Sint16 unk_8C212F16;	/* set while the yes/no bar opens or its cursor slides */
extern Sint8 rec_item_no;	/* the record panel's chosen item */
extern Sint8 net_cursor_no;	/* the network screen's cursor: the answer, the line, the room or the tab it is on */
extern Uint8 unk_8C212F1A;
extern Sint8 unk_8C212F1B;	/* bit 7: the lobby window shows the second profile (net_rec[1]) */
extern Sint8 board_cursor_no[2];	/* each side's character on the record board, 0 to 2 */
extern Sint8 board_decide[2];	/* and set once it is chosen: the cursor goes */
extern Sint8 unk_8C212F20;	/* set while a screen waits: the clock's dots light, the record's strip runs; the question's answer, 1 or 2 */
extern Sint8 net_info_flag;	/* the console has a provider set up */
extern Sint16 win_pict_u, win_pict_v;	/* how far the lobby window's picture has scrolled */
extern SW_WORK *side_sw_adrs[2];	/* each side's buttons, for a network game to write */
extern NET_WORK net_w;
extern Uint32 net_mdm_hn;	/* the modem's handle, which nothing opens */
extern Uint8 unk_8C212FB4[0x90];
extern Sint8 net_side;	/* the network game: this console's side, from 1 */
extern Uint8 unk_8C213045;
extern Sint8 net_play_flag;	/* a network game is running */
extern Uint8 unk_8C213047;
extern Sint8 unk_8C213048, unk_8C213049;	/* set when a network game starts (game.c), as are 2132CA to 2136D1 */
extern Uint8 unk_8C21304A[0x280];
extern Sint8 unk_8C2132CA, unk_8C2132CB;
extern Uint8 unk_8C2132CC[0x200];
extern Sint8 unk_8C2134CC, unk_8C2134CD, unk_8C2134CE;
extern Uint8 unk_8C2134CF[0x201];
extern Sint8 unk_8C2136D0, unk_8C2136D1;
extern Uint8 unk_8C2136D2[0x2a];
extern Uint8 unk_8C2136FC;	/* this and the four below: a network screen's steps, set by net_scene_set */
extern Sint8 unk_8C2136FD, unk_8C2136FE;
extern Uint8 unk_8C2136FF, unk_8C213700;
extern Uint8 unk_8C213701[4];
extern Sint8 unk_8C213705;	/* below 0 the line to the server is broken: the lamp shows it */
extern Uint8 unk_8C213706[0x100];
extern char net_user_id[0x1c];	/* the ID the password panel shows */
extern char net_password[0x14];	/* and its password, shown as stars */
extern Uint8 net_board_side;	/* the side of the record board this console's player's record is on */
extern Uint8 unk_8C213837[0xb];
extern Uint8 net_file_key[8];	/* the network file's key */
extern Uint8 net_handle[18];	/* this console's player's handle; the message log marks its own lines by it */
extern Uint8 unk_8C21385C[14];	/* kept in the network file, as are the arrays below but unk_8C2139AA */
extern Uint8 unk_8C21386A[0x80];
extern Uint8 unk_8C2138EA[0x80];
extern Uint8 unk_8C21396A[0x40];
extern Uint8 unk_8C2139AA[0x80];
extern Uint8 unk_8C213A2A[0x40];
extern Uint8 unk_8C213A6A[10];
extern Uint8 unk_8C213A74[118];
extern Uint8 unk_8C213AEA[4];
extern Sint16 unk_8C213AEE;	/* a count the record board shows in its middle; while it is not 0 the faces change */
extern Sint16 net_win_add;	/* the record board adds these two to net_win_cnt and net_lose_cnt */
extern Sint16 net_lose_add;
extern Uint8 unk_8C213AF4[4];
extern Uint32 unk_8C213AF8, unk_8C213AFC;	/* the entry screen's second and third numbers, kept in the network file */
extern Uint32 net_entry_num;	/* net_entry_str as a number: the entry screen's first */
extern Sint32 net_play_time;	/* the time played, in seconds */
extern Sint8 net_rank;	/* the player's rank, which picks the rank mark's model */
extern Sint32 unk_8C213B0C;	/* two counts, its low and high halves, beside the name on the record panel */
extern Uint8 unk_8C213B10[0x80];
extern Sint16 net_win_cnt;	/* the player's record: the wins and the losses, shown after their total */
extern Sint16 net_lose_cnt;
extern Uint8 unk_8C213B94[0x114];
extern ROOM_INFO room_info[10];	/* the rooms the server lists */
extern Uint8 unk_8C2141E4;
extern Sint8 net_area_no;	/* the area the room list's cursor is on */
extern Uint8 unk_8C2141E6;
extern Sint8 room_info_cnt;	/* the rooms in room_info */
extern Sint8 unk_8C2141E8;	/* in the room list, 1 or 2 lights a button's mark while unk_8C2136FF is set */
extern Sint8 net_room_no;	/* the room the player is in, in room_info */
extern char net_vs_key[8];	/* the other player's ID, as net_file_key */
extern char net_vs_handle[0x92];	/* and handle */
extern Sint8 net_vs_rank;	/* and rank */
extern Uint8 unk_8C214285;
extern Sint16 net_vs_win_cnt;	/* the other player's, as net_win_cnt on: the board shows them */
extern Sint16 net_vs_lose_cnt;
extern Uint8 unk_8C21428A[2];
extern Sint16 net_vs_win_add;	/* and the two the board adds */
extern Sint16 net_vs_lose_add;
extern Uint8 unk_8C214290[2];
extern Uint8 unk_8C214292;	/* kept in the network file, as is unk_8C214294 */
extern Uint8 unk_8C214293;
extern Uint8 unk_8C214294;
extern Uint8 unk_8C214295[7];
extern Sint32 unk_8C21429C;	/* while set, the selector closes and the lobby's arrows hide */
extern Uint8 unk_8C2142A0[7];
extern Sint8 net_pad_bit;	/* in a network game, the pads that play: bit 0 port A, bit 1 port B */
extern Uint8 unk_8C2142A8[0x106];
extern Uint8 unk_8C2143AE;	/* vms_work's net_cnt_flag, as the network file was loaded */
extern Uint8 unk_8C2143AF[0x13];
extern Sint8 unk_8C2143C2;	/* -1 while the selector is up, 0 once it has gone */
extern Uint8 unk_8C2143C3;
extern Sint16 unk_8C2143C4;	/* the selector's box chosen, from 1; 0 for none */
typedef struct NET_ENTRY {
	Uint8	key[8];
	Uint8	handle[18];
} NET_ENTRY;
extern NET_ENTRY net_key_list[50];	/* the keys the network file lists */
extern Uint8 unk_8C2148DA[0x33c];
extern NET_REC net_rec[2];	/* each side's profile */
extern Uint8 unk_8C215068[0xbc];

/* the board's, which the disc keeps and doesn't use */
extern NLA_SYSTEM_FLAG *sys_flag;
extern NLA_CREDIT_CONF *credit_conf;
extern NLA_CREDIT_INFO credit_info;
#endif

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
#ifndef TARGET_DC
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
#else
/* in their place on the disc */
extern Uint8 unk_8C21524C[0x32];
extern char net_entry_str[12];	/* the lobby's entry number, as text */
extern Sint8 lobby_time_flag;	/* while set, the vertical blank counts lobby_time */
extern Uint32 lobby_time;	/* frames, shown as a clock on the network screens */
extern Uint8 unk_8C215290[4];
extern Uint32 unk_8C215294;
extern Uint8 unk_8C215298[4];
extern Uint16 log_scroll;	/* the message log is scrolled by hand: up and down move it, and it no longer follows */
extern Uint8 unk_8C21529E[2];
extern Sint16 unk_8C2152A0, unk_8C2152A2;	/* the data card's second record, as net_win_cnt and net_lose_cnt */
extern Uint8 unk_8C2152A4[2];
extern Uint16 button_cfg[2][6];	/* each side's button config */
extern Uint8 unk_8C2152BE[14];
#endif
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
#ifndef TARGET_DC
extern Uint8 unk_0C2D9688[28];
#else
/* the home menus' model files, where the arcade has unk_0C2D9688 */
extern OBJ_FILE *obj_dm0d ;
extern OBJ_FILE *obj_dm0f ;
extern OBJ_FILE *obj_dm10 ;
extern OBJ_FILE *obj_dm11 ;
extern OBJ_FILE *obj_dm12 ;
extern Uint8 unk_8C26A954[8];
#endif

extern Uint8 stage_no;		/* the stage; 8 a special one */
extern Sint8 dbg_mode;		/* 1 while the debug switch menu is up, 4 while the model viewer runs */
extern Uint8 unk_0C2D96A6;
extern Sint8 sprite_keep_flag;	/* while set, sprite_init leaves the sprites as they are */
extern Sint8 tex_req_flag;	/* a texture send is asked for */
extern Sint8 tex_req_no;	/* which one, 1 to 29 */
extern Uint8 rom_dma_flag;	/* set while a file is read from the ROM; the controls aren't read */
extern Uint8 stream_start_flag;	/* a song was started this frame: the copy queue waits for the vertical blank */
#ifndef TARGET_DC
extern Uint8 unk_0C2D96AC[16];
#else
extern Uint8 unk_8C26A964;	/* unk_0C2D96AC's first three bytes, which the home System_Init sets (6, 0, 0) */
extern Uint8 unk_8C26A965;
extern Uint8 unk_8C26A966;
extern Uint8 unk_8C26A967[13];
#endif
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
extern RANK_DATA rank_score[21];	/* the first ranking, by score */
extern RANK_DATA rank_win[21];	/* the second, by wins in a row against another player */
extern Sint16 bg_ang0, bg_ang1, bg_ang2, bg_ang3;	/* angles the stage and gauges turn by, stepped each frame */
extern void ( *pl_tbl_buff[6][32] )( PLAYER_WORK *pl_w );	/* the routine tables pl_char_set and the character modules fill, one for each work */
extern Uint8 unk_0C2F8C20[160];

#endif

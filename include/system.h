/* The system: SYSTEM_WORK, the settings, the ranking and the memory cards, and the routines for
   files, textures and sound. player.h includes it. */
#ifndef SYSTEM_H
#define SYSTEM_H

/* a file in the ROM: its address (the top bit set when it is packed), and its size */
typedef struct FILE_DATA {
	Uint32	adrs;
	Uint32	size;
} FILE_DATA;

extern const FILE_DATA stream_tbl[];	/* am_load.c: every file in the ROM, by number */

typedef struct SYSTEM_WORK {
	Sint8	task_no;		/* 000: the system task running: 2 a game, 3 the attract demo or the test menu */
	Sint8	task_step;		/* 001: the task's step; counts the attract demos shown */
	Sint8	scene_no;		/* 002: the scene: 2 the game, 3 the continue, 4 the ending, 5 the game over, 6 the attract; in the attract demo, its step */
	Sint8	routine_0;		/* 003: the scene's step */
	Sint8	routine_1;		/* 004: the step within it; in the game, the fight's: 3 once a fight is over */
	Sint8	routine_2;		/* 005: the step within routine_1 */
	Sint8	routine_3;	/* 006: the step within routine_2; staff_init's set-up step */
	Sint8	exit_flag;	/* 007: the scene is ending: the select's effects leave; in the continue, 2 counting down holds back disp_stage_front */
	Sint16	timer0;	/* 008: the scene's first timer: frames left in the select's steps, the ranking, the game over, the attract demo */
	Sint16	timer1;	/* 00a: the second: the attract scene's frame count, frames before a continue starts the game again, the ranking's page */
	Sint16	timer2;	/* 00c: the third: the continue count shown, the staff roll's picture time, the ranking's line */
	Sint16	timer3;	/* 00e: the fourth: the continue count's frames, the game over's fog, the ranking's step, the staff roll's waits */
	Sint32	vblank_cnt;	/* 010: counts every vertical blank; its low byte picks the ranking's announcement and lights the LED */
	Sint32	play_mode;	/* 014: what the machine is doing: 32 power on, 64 the attract, 96 a game, 0x80 the test menu */
	Sint8	entry_side;	/* 018: a bit for each side in the game, from joining to its game over */
	Uint8	entry_stop;	/* 019: while set, no side can join or continue: set while a fight starts and ends */
	Sint8	task_stop;	/* 01a: holds the system task for a frame; also stops the sprite frame count */
	Sint8	damage_level;	/* 01b: the test menu's damage level, 0 to 3 */
	Sint32	game_timer0;		/* 01c: counts every frame */
	Sint32	wave_ang;	/* 020: an angle for colour pulses: steps 80 a frame, back to 0 at 0x10000 */
	Sint32	spr_cnt;	/* 024: the sprites drawn this frame */
	Uint8	no_flash_flag;	/* 028: while set, the fight's screen flashes are off */
	Sint8	cpu_level_type;		/* 029: the system flag's Country, which picks cpu_level.c's tables */
	Uint8	event_flag;	/* 02a: the event setting: both sides start together, no continue, no score or name entry */
	Uint8	unk_2b;	/* 02b: cleared at power on and by the reset; never read */
	Uint8	demo_no;		/* 02c: the name entry is for the second ranking */
	Sint8	no_bgm_flag;		/* 02d: while set, songs and streamed music don't start */
	Sint8	cpu_level_max;		/* 02e: keeps the computer at its top level */
	Uint8	develop_flag;	/* 02f: the DEVELOP switch, copied when the switch menu closes; gates every dbg flag */
	Uint32	char_open0;	/* 030: the characters opened, a bit for each of the first 32 */
	Uint32	char_open1;	/* 034: and of the rest */
	Uint32	char_open_all0;	/* 038: the characters opened a second time, by char_open_all, the first 32 */
	Uint32	char_open_all1;	/* 03c: and of the rest */
	Uint8	monitor_31k;	/* 040: 1 on a 31 kHz monitor (the system flag's DisplayMode) */
	Sint8	st_demo_flag;		/* 041 */
	Uint8	stereo;		/* 042: the test menu's sound output: 1 stereo */
	Sint8	demo_sound;		/* 043: 2 lowers the total volume */
	Uint8	cont_enable;	/* 044: the continue setting: a side may continue */
	Uint8	voice_type;		/* 045: the test menu's voice type, 0 or 1 */
	Sint8	time_speed;		/* 046: the clock's speed setting, 0 to 3 */
	Sint8	vms_mode;	/* 047: the memory card setting: 0 no cards, 1 cards, 2 also the second slot */
	Uint8	vm_write;		/* 048: a setting: shows the points each side earns */
	Sint8	speed_fix;	/* 049: the speed setting: 0 the players pick, else (value - 1) & 1 is the speed */
	Sint8	no_challenge_flag;	/* 04a: the joining setting: no one can join in ("NO CHALLENGER") */
	Uint8	open_lv;
	Sint8	home_mode;	/* 04c: the mode chosen on the home game's menu (HOME_*) */
	Sint8	round_time_no;		/* 04d: the home game's round time setting: 30, 60 or 99 seconds, 3 no clock */
	Uint8	attract_step;	/* 04e: the attract's step to start at, once (6 after a game) */
	Uint8	renda_level;	/* 04f: the mashing setting, copied to cpu_renda_level at each game */
	Uint8	unk_50;	/* 050: set when a home game is quit from the pause, cleared as a game starts; never read */
	Uint8	arcade_clear;	/* 051: the arcade game has been cleared (kept on the card): the stage select has stage 8 too */
	Uint8	gauge_double;	/* 052: a home rule: the hyper combo gauge fills twice as fast */
	Uint8	red_vital_keep;		/* 053: a home rule: the red vital does not come back */
	Uint8	vital_drain;		/* 054: a home rule: the vital drains with time */
	Sint8	red_vital_rule;		/* 055: a home rule for the red vital: 1 none, 2 all the damage */
	Sint8	guard_dm_rule;		/* 056: a home rule for a guarded hit's damage: 1 none, 2 double */
	Uint8	share_vital;	/* 057: a team shares one vital (a test option) */
	Uint8	one_char;		/* 058: a home rule: one character a side */
	Sint8	ended_mode;		/* 059: the home menu's mode whose game has just ended, -1 for none: the attract then saves its points */
	Uint8	unk_5a[0x5c - 0x5a];
	Uint8	adx_pause_flag;	/* 05c: the home game's music is paused (adx_pause) */
	Uint8	unk_5d;		/* 05d: cleared by the home game's reset */
	Uint8	unk_5e;	/* 05e: cleared with unk_5d */
	Uint8	unk_5f[0x70 - 0x5f];
	Uint8	option_cursor[2];	/* 070: each side's line on the home game's button config (the screen adjust uses the first) */
	Uint8	unk_72;	/* 072: cleared when the button config's cursor reaches the vibration line; never read */
	Uint8	unk_73[0x80 - 0x73];
	Sint8	game_timer;	/* 080: counts the game task's frames; its low bits pick random win poses and moves */
	Sint8	stage_cnt;	/* 081: the fight on the player's route, 0 to 7 (7 the last); its low bits pick the computer's level column */
	Uint8	unk_82;
	Sint8	cpu_renda_level;		/* 083: the row of the computer's mashing table */
	Sint8	play_side;	/* 084: a bit for each side a human plays in this fight (3 a versus) */
	Sint8	new_side;	/* 085: a bit for each side that has joined and not yet been through the select */
	Uint8	unk_86;	/* 086: cleared as a game starts (game_pl_set); never read */
	Uint8	unk_87;	/* 087: cleared as a fight starts (play_start); never read */
	Uint8	ending_flag;		/* 088: the game goes into the ending */
	Sint8	game_speed;	/* 089: the speed picked: 0 normal, 1 turbo; sets vs_sys_work's speed_rate */
	Sint8	cont_flag;		/* 08a: the sides in their continue this frame */
	Sint8	vs_flag;		/* 08b: two human players are in the game */
	Sint8	join_side;	/* 08c: the sides that just joined mid-scene; while set, the scene runs Sound_Heart */
	Sint8	join_flag;	/* 08d: a side has joined this scene: holds the entries, the sprite frame count, hyper sparks and ranking objects */
	Sint8	heart_step;		/* 08e: above 1, only the front is drawn; the step of Sound_Heart, while join_side is set */
	Sint8	rank_char;	/* 08f: the character the ranking shows, or -1 */
	Uint32	play_timer;	/* 090: the fight's frames, counted each move (twice a frame at the faster speeds); -1 stops it */
	Uint8	*tex_buff;		/* 094: a buffer for the scene's textures */
	Sint8	hyper_bg;	/* 098: vs_sys_work's hyper_bg_req as last copied: the hyper's background darkening, its colour in the low four bits */
	Sint8	fight_side;	/* 099: play_side as the fight started */
	Uint8	sel_done_flag;		/* 09a: ends the select; set when the game's fights start */
	Sint8	rank_side;		/* 09b: the side that goes on: the winner of a versus, else the human player; whose team the ranking shows */
	Uint8	unk_9c[0xa4 - 0x9c];
	Uint8	name_stop;		/* 0a4: set with ending_flag */
	Sint8	new_side_keep;		/* 0a5: a bit for each player */
	Uint8	no_ko_flag;	/* 0a6: a KO leaves 1 vital (a test option) */
	Sint8	dummy_guard;		/* 0a7: training: the dummy guards, 1 after the first hit, 2 always */
	Uint8	dummy_air_recover;		/* 0a8: training: the dummy recovers in the air */
	Uint8	dummy_change;		/* 0a9: training: the dummy changes partner when its vital is low */
	Uint8	auto_gauge;		/* 0aa: training: the hyper combo gauges fill by themselves */
	Uint8	boss_flag;	/* 0ab: the fight is on stage 8, the last on the route; with game_mode 8 it runs the boss rules */
	Uint8	delay_hyper_free;	/* 0ac: a home rule: a player already in the chain of delayed hyper combos may join it again */
	Sint8	rank_disp;		/* 0ad: the ranking shown: vs_flag, or 2 when home_mode is HOME_SCORE (the home game's score attack) */
	Uint8	damage_disp;	/* 0ae: training: the damage and the combo are shown */
	Uint8	entry_flag;	/* 0af: a side has joined or continued since the game began: clearing the arcade earns fewer points */
	Uint8	pause_off;		/* 0b0: the home game's pause is off: set for a network game */
	Sint8	net_pause;		/* 0b1: a network game's own pause */
	Uint8	disp_line;		/* 0b2: the line the scores and wins are printed on */
	Sint8	armor_all;	/* 0b3: a home rule: every player has the armour */
	Uint8	unk_b4;	/* 0b4: a network rule (net_w.rule[14]); never read */
	Uint8	partner_ng;	/* 0b5: a home rule: no partner can be called in */
	Uint8	partner_free;	/* 0b6: a home rule: a partner can be called in at any time */
	Uint8	unk_b7;	/* 0b7: a network rule, net_w.rule[11]'s second bit (the first is one_char); never read */
	Uint8	unk_b8[0x100 - 0xb8];
} SYSTEM_WORK;
#define SYS_HALF	0x80	/* sys_work is cleared in two halves: at power on, and for each game */

/* Sys->home_mode: the mode chosen on the home game's menu (always 0 on the board) */
#define HOME_ARCADE		0
#define HOME_VERSUS		1
#define HOME_TRAINING	2
#define HOME_OPTION		3
#define HOME_CARD		4	/* the memory card menu */
#define HOME_NETWORK	5
#define HOME_SECRET		6	/* the secret factor */
#define HOME_SCORE		7	/* the score attack */
#define HOME_RECORDS	8	/* the records */

/* the home game's options, set on its OPTION screens */
typedef struct OPTION_WORK {
	Sint8	difficulty;	/* 000: the game options, which sys_set_init copies to the conf_ bytes */
	Sint8	round_time;	/* 001 */
	Sint8	damage_level;	/* 002 */
	Sint8	speed_fix;	/* 003 */
	Sint8	stereo;		/* 004: the sound output, at first the console's own setting */
	Sint8	unk_05;
	Sint8	vib_bit;	/* 006: the vibration setting: a bit for each side whose packs may shake */
	Uint8	unk_07;
	Sint8	com_time[2];	/* 008: each side's COMMAND setting: 1 waits 10 frames more for a command's steps (not over the network) */
	Uint16	unk_0a;
	Uint8	unk_0c;
	Uint8	unk_0d;
	Uint8	unk_0e;
	Uint8	unk_0f;
	Float	adjust_x;	/* 010: the screen position: added to where a scene puts its pictures */
	Float	adjust_y;	/* 014 */
	Sint32	unk_18;		/* 018: the screen, from the cable at start-up; 1 draws the front's OBJ_FIT models less tall */
	Uint8	unk_1c;
	Uint8	unk_1d;
	Uint8	unk_1e;
	Uint8	unk_1f;
	Uint16	key_config[2][6];	/* 020: what each side's six buttons do, a number in system.c's key_func_tbl */
	Uint8	unk_38[0x40 - 0x38];
} OPTION_WORK;


/* the home game's secret factor: the level and the points its shop takes (kept in the card's system file) */
typedef struct SECRET_WORK {
	Uint8	vm_level;	/* 000 */
	Uint8	vm_exp;		/* 001: the points toward the next level, 0 to 99 */
	Uint16	d_pts;		/* 002 */
	Uint16	unk_04;
	Uint16	n_pts;		/* 006: the second kind's, as d_pts */
	Uint16	unk_08;
	Uint16	unk_0a;
	Uint16	unk_0c;
	Uint16	unk_0e;
	Uint8	unk_10;		/* 010: the network games counted, up to 99 */
	Uint8	unk_11;
	Uint8	play_hour;	/* 012: the time played, up to 99:59:59 */
	Uint8	play_min;
	Uint8	play_sec;
} SECRET_WORK;


/* the settings of a country */
typedef struct SYS_SET {
	Uint32	id;	/* 000: BOOK_ID */
	Sint8	country;	/* 004: the country again */
	Sint8	stereo, demo_sound_half, cont_enable, voice_type, difficulty, damage_level, time_speed;
	Sint8	vms_mode, vm_write, speed_fix, event_flag, no_challenge_flag, open_lv;
} SYS_SET;

/* a side's vibration: its frames left (-1 to start it) and its kind in vib_tbl */
typedef struct VIB_WORK {
	Sint32	cnt;
	Sint32	no;
} VIB_WORK;

/* one place in a ranking */
typedef struct RANK_DATA {
	Sint8	rank;	/* 000: the place, from 1; picks its mark, 1 to 21 */
	Sint8	load_type[3];	/* 001: the team's characters, their load_type */
	Sint8	pl_personal[3];	/* 004: and their pl_personal */
	Sint8	ending_flag;	/* 007: Sys->ending_flag: the ending was reached, shows a mark */
	Uint8	win_max;	/* 008: the player's win_max, which orders the second ranking */
	Sint8	play_min;	/* 009: the time played: minutes, */
	Sint8	play_sec;	/* 00a: seconds */
	Sint8	play_csec;	/* 00b: and hundredths */
	Uint8	name[4];	/* 00c: the name entered */
	Uint32	score;	/* 010: the score's low eight digits (BCD), from old_score */
	Uint32	score_hi;	/* 014: and its high ones, from old_score_hi */
} RANK_DATA;

/* the points and memory cards (vms.c) */
typedef struct VMS_WORK {
	Sint32	point;		/* 000: the points, up to 999999 */
	Uint32	card_open[2];	/* 004: the characters the cards open, a bit each */
	Uint32	card_ex[2];
	Uint32	play_time;	/* 014: the time played, in tenths of a second, up to an hour */
	Sint32	card_bit;	/* 018: the ports with a memory card, 0xc the first side's, 0x30 the second's; cleared by the select scene */
	Sint32	card_bit_keep;	/* 01c: card_bit as it was when a side began reading its card */
	Sint16	point_lv;	/* 020: the level the points have reached */
	Uint8	card_task_step[2][2];	/* 022: each side's step in vms_task_card, and the step to go back to after a wait */
	Sint16	card_task_timer[2];	/* 026: each side's wait in vms_task_card's step 4, 60 frames */
	Uint8	stage_ex_cnt;	/* 02a: the stages whose other look is opened, 0 to 7: the shop sells the next, the download file raises it */
	Sint8	file_flag;	/* 02b: 1 while the home game's files are on the card at file_port: the game saves to it */
	Sint32	mount_port;	/* 02c: the port whose card is mounted, -1 for none */
	Sint32	task_port;	/* 030: the port a card task works on */
	Sint32	file_port;	/* 034: the port of the card the home game's files are on */
	Sint32	task_flag;	/* 038: a card task is running; vms_check waits */
	Uint32	bu_busy_bit;	/* 03c: a bit for each port waiting on the backup library's callback */
	Sint32	get_point[2];	/* 040: each side's points given to its card, shown when it joins */
	Sint32	d_point;	/* 048: the home game's points earned since the last save, 0 to 9999 (d_point_add) */
	Sint32	n_point;	/* 04c: and its second kind (n_point_add) */
	Uint8	task_step;	/* 050: where a card task is, 0 to 5 */
	Uint8	card_done_bit;	/* 051: a bit for each side whose card has been read; it can't read again */
	Uint8	save_busy;	/* 052: set while a file is being saved; main waits on it before it exits */
	Uint8	unk_53[3];
	Uint8	card_busy_bit;	/* 056: a bit for each side whose card is being read */
	Uint8	card_step[2];	/* 057: each side's step in reading its card (card_ck) */
	Uint8	vm_msg[2];	/* 059: the home game's card message shown, and its window's state */
	Sint8	disp_adjust_req;	/* 05b: the system file was loaded: its screen position is to be set (disp_adjust_set) */
	Sint32	cursor_port;	/* 05c: the port the home game's card screens point at, -1 for none */
	Sint32	add_point[2];	/* 060: each side's points from its card, added to the points when it joins */
	/* the rest is the home game's (the board clears it with the others) */
	Uint16	year;		/* 068: the date of the system file the game last loaded or saved */
	Uint8	month, day, hour, minute, second, week;
	Uint8	net_key[8];	/* 070: the network file's key */
	Sint8	id[6];		/* 078: the console's ID as the network file has it */
	Uint8	net_cnt_flag;	/* 07e: a network game to count: vms_net_cnt_up adds it to the secret work's count, once */
	Uint8	unk_7f;
	Uint8	net_key_keep[8];	/* 080: the key as it was when the file was loaded */
	Uint8	unk_88;		/* 088 */
	Uint8	unk_89;
	Uint8	unk_8a[0x90 - 0x8a];
} VMS_WORK;

/* the clock's date, and the one a file is saved with (vms.c) */
typedef struct VMS_DATE {
	Uint16	year ;
	Uint8	month, day, hour, minute, second, dayofweek ;
} VMS_DATE ;

extern void Sound_Heart( void );
extern void RankingMain( void );
extern void RankDemoMain( void );
extern void rank_data_init( void );
extern void rank_name_chk( void );
extern void rank_entry( PLAYER_WORK *wk );
extern void vs_rank_entry( PLAYER_WORK *wk );
extern void TexLoad( int no );
extern void Palette_Init( void );
extern void Texture_Init( void );
extern void tex_dummy_0();	/* s_play passes it a work and its number, which it doesn't read */
extern void rand_pic_load( void );	/* tex_load.c: the home game's pictures (empty on the board) */
extern void rand_pic_set( void );
extern void ask_file_load( void );
extern void keyboard_tex_load( void );
extern void keyboard_tex_set( void );
extern int thd_file_load( int no, void *adrs );
extern Uint32 file_load2( Sint32 no, void *adrs, Sint32 a );
extern Sint32 load_ck( void );
extern int test_sw_ck( void );
extern Sint32 vblank_func( void );	/* the vertical blank interrupt's routine */
extern void score_tex_set( Uint32 hi, Uint32 lo, Uint8 *dst_top, const Uint8 *pos );
extern void win_tex_set( Uint32 cnt, NLthd *thd );

/* cache.src: the operand cache over a range dropped (cache_inv) or written back (cache_purge) */
extern void cache_inv( void *adrs, Uint32 size );
extern void cache_purge( void *adrs, Uint32 size );

/* vms.c: the VMU code the system calls */
extern void vms_check( void );
extern int vms_ports_card_now_ck( void );
extern int vms_task_card( int port, Uint8 side );
#ifdef TARGET_DC
extern int vms_task_load_all( int port, Uint8 mode );	/* the home game's files from a card: 2 while it works, 1 done, 0 failed */
extern int vms_task_auto_save( void );
extern int vms_task_save_all( int port, Uint8 mode );
extern void mode_cnt_up( void );	/* the play counts of the mode being played */
extern void pl_cnt_up( Uint8 pl );
extern int vms_date_get( int port, VMS_DATE *date );	/* the date of the system file on a port's card, 0 for none */
extern void vms_date_keep( int port );	/* kept, to tell later whether the card is the same */
#endif

/* sys_task.c */
extern int game_play_ck( void );
extern int cpu_max_ck( void );

/* sound.c: both targets have all of these. The board plays a scene's song with song_req and a
   stage's stream with the stream calls; the disc streams both (stream_req, s3l_fade_in_set, stream_start) */
extern void s3l_fade_out_set( Uint8 spd );
extern void s3l_fade_in_set( Uint8 spd );
extern void stream_req( Sint8 no );
extern void stream_start( void );
extern void stream_stop( void );
extern void na_req( Uint8 no );
extern int card_ck( Uint8 side );
extern void song_req( Uint8 no );
/* a scene's own music: the board's song, or on the disc the stream that takes its place */
#ifdef TARGET_DC
#define scene_song_req( song, stream )	( stream_req( stream ), s3l_fade_in_set( 1 ), stream_start() )
#else
#define scene_song_req( song, stream )	song_req( song )
#endif
extern void sound_all_off( void );
extern void se_all_off( void );
extern void se_vol_down( Uint16 pack, Uint8 spd );
extern void se_vol_up( Uint16 pack, Uint8 spd );
extern void fade_out_set( Uint16 pack, Uint8 spd );
extern void staff_se_req( Uint8 no );
extern void staff_sound_load( void );
extern void STrPrint( Uint8 x, Uint8 y, Uint8 col, const char *fmt, ... );

#ifdef TARGET_DC
/* the home game's flow: what the Dreamcast program has for the board's coins and test menu */
extern void sys_set_load( void );	/* system.c: the option menu's settings into Sys */
extern void home_menu( void );	/* the mode menu, the system's task 3 */
extern void pause_init( void );	/* pl_sw.c: which sides may pause, by the mode */
extern void training_init( void );	/* pl_sw.c: the training mode's rules */
extern void net_rule_set( void );	/* pl_sw.c: the network game's */
extern void net_rule_init( void );
extern void net_pause_set( int flag );	/* pl_sw.c: the other console holds the game */
extern void net_pause_move( void );
extern int soft_reset_ck( int game );	/* system.c: the reset keys (START+A+B+X+Y) are held */
extern void d_point_add( int pt );	/* system.c: points toward the secret factor's shop */

/* the home game's pause (pl_sw.c) */
typedef struct PAUSE_WORK {
	Sint8	pause_flag;	/* 000: set while the game is held: 1 or 2 the side that paused, bit 2 while a pad is pulled out */
	Sint8	side;		/* 001: a bit for each side that may pause; in the training, the sides whose pad shakes */
	Uint8	dark;		/* 002: how dark the screen behind is: it rises to 96 while no button is held */
	Sint8	unk_03;
	Sint8	pad_on[2];	/* 004: each side's pad was there the frame before */
	Sint8	req[2];		/* 006: what each side asks for: 1 its pad was pulled out, 2 it pressed start */
	Sint16	dark_timer;	/* 008: frames until the screen behind darkens */
	Sint16	open_timer;	/* 00a: frames until the window is open */
	Sint8	open_flag;	/* 00c: the window is open */
	Sint8	page;		/* 00d: the menu shown: 0 the pause's, 1 QUIT GAME?, 2 the training's */
	Sint8	cursor;		/* 00e: the menu's line the cursor is on */
	Sint8	cursor_mes;	/* 00f: that line's message in pause_mes, which the cursor is drawn behind */
	Float	win_w;		/* 010: half the window's size */
	Float	win_h;		/* 014 */
	Sint8	config_flag;	/* 018: the button config has the pause: start does not end it */
	Sint8	config_on;	/* 019: the button config is up */
	Sint8	config_open;	/* 01a: and its window is open */
	Sint8	stop_keep;	/* 01b: vs_sys_work's game_stop, kept while the training menu is up */
	Uint8	unk_1c[0x20 - 0x1c];
} PAUSE_WORK;
extern void pause_ck( void );	/* a side's start button, or its pad pulled out, holds the fight */
extern void pause_main( void );	/* the pause's menus, each frame the fight is held */

/* the training mode's settings, from its menu (pl_sw.c) */
typedef struct TRAINING_WORK {
	Sint8	page;		/* 000: the menu's page, a routine of training_menu */
	Sint8	cursor;		/* 001: the first page's line */
	Sint8	cursor2;	/* 002: the line of the page shown */
	Sint8	unk_03;
	Sint8	unk_04;		/* 004: the dummy: 1 the computer plays it */
	Sint8	stance;		/* 005: what the dummy does: 1 crouches, 2 jumps */
	Sint8	dummy_guard;	/* 006: the dummy guards: 1 after the first hit, 2 always */
	Sint8	dummy_air_recover;	/* 007: the dummy recovers in the air */
	Sint8	dummy_change;	/* 008: the dummy changes partner when its vital is low */
	Sint8	damage_disp;	/* 009: the damage and the combo are shown (to Sys's) */
	Sint8	renda_level;	/* 00a: the computer's mashing, for Sys's cpu_renda_level */
	Sint8	damage_level;	/* 00b: the damage setting */
	Sint8	game_speed;	/* 00c: the speed */
	Sint8	auto_gauge;	/* 00d: the hyper combo gauges fill by themselves */
	Sint8	menu_on;	/* 00e: set while the training menu is on the screen */
	Sint8	side;		/* 00f: the side that trains, a bit; the other side's work is the dummy */
	Uint8	unk_10[0x19 - 0x10];
	Sint8	delay_hyper_free;	/* 019: a player in the chain of delayed hyper combos may join it again (to Sys's) */
	Sint8	step[2];	/* 01a: the page's step (fade out, wait, the menu, leave), and the line chosen */
	Sint8	unk_1c;		/* 01c: the menu's objects to show */
	Uint8	unk_1d[0x20 - 0x1d];
} TRAINING_WORK;

/* a part of a training menu window, in the list a work's work2 points at (pause_obj_disp) */
typedef struct PAUSE_PART {
	Sint8	kind;		/* 000: drawn unless 0; -1 ends the list */
	Sint8	scale;		/* 001: its scale in pause_scale, 0 for none */
	Uint16	model;		/* 002: in obj_stg */
	VEC3	pos;		/* 004 */
} PAUSE_PART;
extern VEC3 pause_scale[20];	/* pause_win.c: the widths the windows' parts and cursors take */

/* a side's button config while its menu is up (pl_sw.c) */
typedef struct BTN_WORK {
	Sint8	cursor;		/* 000: the line */
	Sint8	side;		/* 001 */
	Sint8	vib;		/* 002: the vibration is on */
	Sint8	com_time;	/* 003: the COMMAND line's setting, for option_w's */
	Sint8	key[6];		/* 004: what each button does */
	Sint8	cursor_mes;	/* 00a: the message the cursor is drawn behind */
	Uint8	unk_0b[0x10 - 0x0b];
} BTN_WORK;

/* the network game's rules and characters, the same on both consoles (pl_sw.c) */
typedef struct NET_WORK {
	Sint8	rule[16];	/* 000: the rules, which net_rule_set copies to Sys */
	Uint32	def_team_bit[2][3];	/* 010: each side's default team, as select.c's tables: their bits in the masks */
	Sint32	def_team_tbl[2][3];	/* 028: and their characters */
	/* the characters that may be chosen, as vms_work's card_open and card_ex: the cards' and
	   unk_70's, with the or masks added, and the and masks taken */
	Uint32	open_and[2];	/* 040 */
	Uint32	ex_and[2];	/* 048 */
	Uint32	open_or[2];	/* 050 */
	Uint32	ex_or[2];	/* 058 */
	Uint32	char_open[2];	/* 060 */
	Uint32	char_ex[2];	/* 068 */
	Uint32	unk_70[2];	/* 070 */
	Uint32	unk_78[2];	/* 078 */
} NET_WORK;

/* the network's text: the glyph cache over the console's fonts (font.c) and the message log */
#define FONT_SLOT	320		/* the cache's glyphs in each of its two buffers */
#define FONT_TEX_NO	0x110		/* the common texture of the first buffer's first glyph */
#define LOG_LINE	10000		/* the message log's lines, a ring */
typedef struct FONT_LOG {
	char	tag[LOG_LINE][8];	/* a line's tag; empty for a line that goes on from the one before */
	char	name[LOG_LINE][17];	/* who wrote it */
	char	text[LOG_LINE][43];
	Sint32	w;			/* the line written next */
	Sint32	view;			/* the first line shown */
	Sint32	top;			/* the oldest line kept */
} FONT_LOG;
typedef struct FONT_WORK {
	Uint16	pixel[32][32];		/* 000: the glyph being made */
	Sint16	code[2][FONT_SLOT];	/* 800: each slot's character (Shift-JIS) */
	Uint8	flag[2][FONT_SLOT];	/* d00: 1 drawn this frame, 2 to be made, 4 made */
	Uint8	font[2][FONT_SLOT];	/* f80: its font */
	Uint8	col[2][FONT_SLOT];	/* 1200: its colours, in font_col_tbl */
	Uint8	unk_1480[8];
	Sint32	rows[2];		/* 1488: each font's rows of a glyph */
	Sint32	bytes[2];		/* 1490: the bytes of a row, four pixels each */
	Sint32	size[2];		/* 1498: a glyph's record */
	Uint8	*adrs[2];		/* 14a0: the font's file */
	Sint32	next[2];		/* 14a8: each buffer's next slot to take */
	Sint32	buf;			/* 14b0: the buffer made this frame */
	FONT_LOG	log;		/* 14b4: the message log */
} FONT_WORK;
extern FONT_WORK *font_w;
extern void str_put( Float x, Float y, Float z, Uint32 col, char *str ) ;	/* font_str.c */

#endif

#endif

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
	Uint8	unk_2b;
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
	Uint8	no_challenge_flag;	/* 04a: the joining setting: no one can join in ("NO CHALLENGER") */
	Uint8	open_lv;
	Sint8	unk_4c;		/* 04c */
	Uint8	unk_4d;
	Uint8	attract_step;	/* 04e: the attract's step to start at, once (6 after a game) */
	Uint8	renda_level;	/* 04f: the mashing setting, copied to cpu_renda_level at each game */
	Uint8	unk_50;
	Uint8	unk_51[0x57 - 0x51];
	Uint8	share_vital;	/* 057: a team shares one vital (a test option) */
	Uint8	unk_58[0x80 - 0x58];
	Sint8	game_timer;	/* 080: counts the game task's frames; its low bits pick random win poses and moves */
	Sint8	stage_cnt;	/* 081: the fight on the player's route, 0 to 7 (7 the last); its low bits pick the computer's level column */
	Uint8	unk_82;
	Sint8	cpu_renda_level;		/* 083: the row of the computer's mashing table */
	Sint8	play_side;	/* 084: a bit for each side a human plays in this fight (3 a versus) */
	Sint8	new_side;	/* 085: a bit for each side that has joined and not yet been through the select */
	Uint8	unk_86;		/* 086 */
	Uint8	unk_87;		/* 087 */
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
	Uint8	unk_a7[0xab - 0xa7];
	Uint8	boss_flag;	/* 0ab: the fight is on stage 8, the last on the route; with game_mode 8 it runs the boss rules */
	Uint8	unk_ac;
	Uint8	rank_disp;		/* 0ad: the ranking shown: vs_flag, or 2 when unk_4c is 7 */
	Uint8	unk_ae;
	Uint8	unk_af;		/* 0af */
	Uint8	unk_b0[0xb2 - 0xb0];
	Uint8	disp_line;		/* 0b2: the line the scores and wins are printed on */
	Uint8	unk_b3[0x100 - 0xb3];
} SYSTEM_WORK;
#define SYS_HALF	0x80	/* sys_work is cleared in two halves: at power on, and for each game */

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
	Uint8	play_min;	/* 009: the time played: minutes, */
	Uint8	play_sec;	/* 00a: seconds */
	Uint8	play_csec;	/* 00b: and hundredths */
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
	Uint8	unk_2a[2];
	Sint32	mount_port;	/* 02c: the port whose card is mounted, -1 for none */
	Sint32	task_port;	/* 030: the port a card task works on */
	Uint8	unk_34[4];
	Sint32	task_flag;	/* 038: a card task is running; vms_main waits */
	Uint32	bu_busy_bit;	/* 03c: a bit for each port waiting on the backup library's callback */
	Sint32	get_point[2];	/* 040: each side's points given to its card, shown when it joins */
	Uint8	unk_48[8];
	Uint8	task_step;	/* 050: where a card task is, 0 to 5 */
	Uint8	card_done_bit;	/* 051: a bit for each side whose card has been read; it can't read again */
	Uint8	unk_52[4];
	Uint8	card_busy_bit;	/* 056: a bit for each side whose card is being read */
	Uint8	card_step[2];	/* 057: each side's step in reading its card (card_ck) */
	Uint8	unk_59[0x60 - 0x59];
	Sint32	add_point[2];	/* 060: each side's points from its card, added to the points when it joins */
} VMS_WORK;

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
extern void vms_main( void );
extern int vms_ports_card_now_ck( void );
extern int vms_task_card( int port, Uint8 side );

/* sys_task.c */
extern int game_play_ck( void );
extern int cpu_max_ck( void );

/* sound.c */
extern void s3l_fade_out_set( Uint8 spd );
extern void s3l_fade_in_set( Uint8 spd );
extern void stream_req( Sint8 no );
extern void stream_start( void );
extern void stream_stop( void );
extern void na_req( Uint8 no );
extern int card_ck( Uint8 side );
extern void song_req( Uint8 no );
extern void sound_all_off( void );
extern void se_all_off( void );
extern void se_vol_down( Uint16 pack, Uint8 spd );
extern void se_vol_up( Uint16 pack, Uint8 spd );
extern void fade_out_set( Uint16 pack, Uint8 spd );
extern void staff_se_req( Uint8 no );
extern void staff_sound_load( void );
extern void STrPrint( Uint8 x, Uint8 y, int col, const char *fmt, ... );

#endif

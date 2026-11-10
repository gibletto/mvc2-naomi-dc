/* The fight (vs_sys.c): VS_SYS_WORK and the steps of its progress. player.h includes it. */
#ifndef VS_SYS_H
#define VS_SYS_H

/* the versus game's state */
/* vs_st: the fight's progress */
#define VS_INIT		0
#define VS_WAIT		1
#define VS_APPEAR	2	/* the entrances */
#define VS_READY	3
#define VS_FIGHT	4
#define VS_END_WAIT	5	/* the fight is over */
#define VS_WIN		6
#define VS_BOSS		7
#define VS_BONUS	8
#define VS_EXIT		9
typedef struct VS_SYS_WORK {
	Uint8	vs_st;		/* 000: the fight's progress: 2 the entrances, 4 the fight, 5 on after it */
	Uint8	ctrl_flag;	/* 001: what the fight lets the players do (CTRL_) */
	Uint8	first_hit;	/* 002: the side that hit first, with FIRST_HIT_SET; 0 until then (it scores the first attack's bonus) */
	Uint8	time_over;	/* 003: the round ended on time, not by a KO */
	Uint8	move_bit;	/* 004: a bit for each player work still to move this frame; a throw sets its victim's so it moves after the thrower */
	Uint8	move_again_flag;	/* 005: set by act_set when a player's act changes, so pl_work_move runs that player again at once */
	Uint8	game_stop;		/* 006: the game is stopped: works stand still and nothing hits */
	Uint8	ko_atck_no;	/* 007: the attack that ended the round (dm_atck_no) */
	HIT_DATA	*ko_hit_data;	/* 008: the hit data of the attack that ended the round */
	PLAYER_WORK	*ko_at_w;	/* 00c: the attacker that ended the round */
	Sint16	fight_frame;	/* 010: the frames fought while vs_st is 4, shown by the time bonus */
	Sint16	vs_timer;		/* 012: the frames left in the fight's current step */
	Sint16	pose_wait_timer;	/* 014: frames the winner's pose waits at least after the round's end (150) */
	Uint8	unk_16[0x18 - 0x16];
	PLAYER_WORK	*team_w[2][3];	/* 018: each side's three players, by team_no */
	Uint8	speed_rate;	/* 030: an extra game frame each this many frames: 6, 4 or 2 by the speed setting; 0 none */
	Uint8	speed_cnt;	/* 031: frames left to the next extra frame, from speed_rate */
	Uint8	speed_flag;	/* 032: 0xff during the extra frame, else 0 (nothing reads it) */
	Uint8	land_flag;	/* 033: set this frame by a landing that shakes the screen; side_flag_get( 0 ) */
	Sint8	be_cnt[2];	/* 034: each side's players in the fight (be_flag) */
	Sint8	ko_cnt[2];	/* 036: each side's players knocked out */
	Uint8	team_hyper_flag;	/* 038: a bit for each side in a team hyper combo; that side's input is ignored */
	Uint8	team_delay_flag;		/* 039: a bit for each side in a delayed partner's attack (team_type 8) */
	Uint8	unk_3a;
	Uint8	slow_step;	/* 03b: the frame count, 0 to 15, picking the bit of slow_bit */
	Uint16	slow_bit;	/* 03c: the slow motion: a bit for each of 16 frames that stands still (SLOW_HALF at a KO) */
	Sint8	winner_id;	/* 03e: the side that won, or -1 for a draw */
	Uint8	win_skip_flag;	/* 03f: set when the winner presses START to end the win pose (nothing reads it) */
	Sint8	round_time;	/* 040: the round's clock, seconds left from 99 */
	Sint8	time_frame;		/* 041: frames left in the clock's second */
	Uint8	team_hyper_bit[2];	/* 042: by side, a bit for each team_no in the team hyper combo */
	Uint8	unk_44;
	Uint8	round_cnt;	/* 045: the fights played in the match, counted at each fight's end */
	Uint8	vs_end_flag;	/* 046: set when the fight's end also ends the match; picks the last win poses */
	Uint8	unk_47;
	Uint8	game_mode;		/* 048: a mode, 8 for a special one; copied from stage_no */
	Uint8	cpu_level;		/* 049: the computer's level */
	Uint8	hyper_bit[2];	/* 04a: each side's hyper combo: 0x10, and a bit for each player (work_no / 2) in it */
	Uint8	push_bit;	/* 04c: a bit for each work pushed apart this frame */
	Uint8	unk_4d;
	Sint16	combo_total[2];	/* 04e: each side's hits in its current combo */
	Sint16	combo_hit_total[2];	/* 052: each side's hits in its current combo, by the hits each attack counts for, up to 999 */
	Sint16	gauge[2];	/* 056: each side's hyper combo gauge within its level, 0 to 144 (PLAYER_WORK's gauge points here) */
	Sint8	gauge_level[2];	/* 05a: each side's gauge level (PLAYER_WORK's gauge_level points here) */
	Uint8	unk_5c[0x60 - 0x5c];
	Sint8	hit_mark_cnt;	/* 060: the hit marks showing */
	Uint8	wall_flag;	/* 061: the wall a fighter flew into this frame (WALL_RIGHT, WALL_LEFT); side_flag_get( 1 ) */
	Uint8	unk_62[0x64 - 0x62];
	Uint32	bonus_lo;		/* 064: the bonus for the round's last hit, in BCD: the low digits */
	Uint32	bonus_hi;		/* 068: and the high ones */
	Uint32	combo_pts[2];	/* 06c: each side's last combo of three or more hits, its points in BCD */
	Uint8	hyper_cnt[2];	/* 074: each side's hyper combos in the current run */
	Uint8	hyper_bg_req;	/* 076: the hyper combo screen effect showing: its kind, and the side */
	Sint8	burst_cnt;	/* 077: the bursts alive, 6 at most */
	Sint16	combo_max[2];	/* 078: each side's longest combo_total */
	Sint16	atck_cnt[2];	/* 07c: each side's attacks started this round */
	Sint16	hit_cnt[2];	/* 080: each side's attacks that hit this round (with atck_cnt, the hit rate) */
	Uint8	ending_wait_flag;	/* 084: set while the ending waits to start */
	Uint8	ending_wait_timer;	/* 085: frames left of that wait (60) */
	Sint8	cpu_time;	/* 086: a second copy of the clock, from 99, that the computer reads */
	Uint8	time_frame_max;		/* 087: the frames of the clock's second, as set */
	Sint8	hyper_chain_cnt[2];	/* 088: each side's hyper combos chained in the current one, 1 to 3; picks the damage table */
	Uint8	unk_8a[0x9a - 0x8a];
	Uint8	no_ko_cnt[2];	/* 09a: each side's wins in a row with no player knocked out; scores a bonus at the round's end */
	Sint8	win_cnt[2];	/* 09c: each side's wins in a row against the other player */
	Uint8	unk_9e[0xc0 - 0x9e];
} VS_SYS_WORK;

/* ctrl_flag: what the fight lets the players do */
#define CTRL_PLAY	0x01	/* the fight is on (never tested) */
#define CTRL_NO_INPUT	0x02	/* the players' input is ignored (all but START) */
#define CTRL_WALK	0x04	/* only forward and back, at step speed; no computer moves, partner calls or delayed hypers */
#define CTRL_NO_THROW	0x08	/* no throws */

/* first_hit */
#define FIRST_HIT_SET	0x80	/* a side is recorded (side 0 then reads nonzero) */
#define FIRST_HIT_SIDE	0x7f	/* the side */

/* slow_bit */
#define SLOW_HALF	0xaaaa	/* every other frame stands still: half speed */

/* vs_sys.c: the fight's progress */
extern void vs_sys_init( void );
extern void vs_round_init( void );
extern void vs_sys_move( void );
extern void side_flag_clr( void );
extern int side_flag_get( int side );
extern void naomi_level_disp( void );

#endif

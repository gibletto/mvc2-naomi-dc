/* The computer player: its level (cpu_level.c), its modes and command picks, and its scripts.
   player.h includes it. */
#ifndef EM_PLAY_H
#define EM_PLAY_H

extern int em_atck_hit_ck( PLAYER_WORK *pl_w, PLAYER_WORK *em_w, int no );

/* the computer's level of play (cpu_level.c) */
typedef struct CPU_LEVEL_WORK {
	Sint16	row ;		/* 00: the row of the level limits, from Sys->cpu_renda_level */
	Sint16	level ;		/* 02: the level, 0 to 31 */
	Sint16	sec ;		/* 04: seconds fought */
	Sint16	frame ;		/* 06: frames left of the second */
	Sint16	sec5 ;		/* 08: seconds, counted to 5 */
	Sint16	step ;		/* 0a: the place in time_tbl */
	Uint8	unk_0c[0x20 - 0x0c] ;
} CPU_LEVEL_WORK ;

/* cpu_level.c: the computer's level */
extern void cpu_level_init( void );
extern void cpu_level_time( void );
extern void cpu_level_set( void );
extern void cpu_level_hit( void );
extern void cpu_level_down( PLAYER_WORK *pl_w );
extern void cpu_level_perfect( void );

extern int em_rate_ck( PLAYER_WORK *pl_w, int a, int b );
extern void em_near_set( PLAYER_WORK *pl_w );
extern void em_play( PLAYER_WORK *pl_w );
/* the computer's modes, its command picks and its tests (em_play.c and the modules after it) */
extern int em_guard_ck( PLAYER_WORK *pl_w );
extern void em_round_init( PLAYER_WORK *pl_w );
extern void em_init( PLAYER_WORK *pl_w );
extern void em_cmd_clr( PLAYER_WORK *pl_w );
extern int em_rnd_get( PLAYER_WORK *pl_w, int no );
extern void em_cmd_sel( PLAYER_WORK *pl_w, int no, int kind );
extern void em_answer_sel( PLAYER_WORK *pl_w );
extern int em_mode_set( PLAYER_WORK *pl_w, int no, int lv );
extern int em_mode_clr( PLAYER_WORK *pl_w, int no );
extern int em_act_ck( PLAYER_WORK *pl_w, int no );
extern int em_old_act_ck( PLAYER_WORK *pl_w, int no );
extern int em_lv_ck( PLAYER_WORK *pl_w );
extern int em_atck_kind( PLAYER_WORK *pl_w );
extern PLAYER_WORK *em_atck_near( PLAYER_WORK *pl_w );
extern int em_area_ck( PLAYER_WORK *pl_w, CPU_PRM *area );
extern int em_atck_area_ck( PLAYER_WORK *pl_w, CPU_PRM *area );

/* em_sub: the script helpers */
extern void cmd_read( PLAYER_WORK *pl_w, CPU_SCRIPT *sc, int type );
extern int cmd_act_ck( PLAYER_WORK *pl_w, int no );
extern int cmd_start_ck( PLAYER_WORK *pl_w, int no );
extern int cmd_flag_ck( PLAYER_WORK *pl_w, CPU_SCRIPT *sc );

/* em_script: the script commands */
extern int cmd_start( PLAYER_WORK *pl_w, CPU_SCRIPT *sc );
extern int cmd_move( PLAYER_WORK *pl_w, CPU_SCRIPT *sc );
extern int goto_endif( PLAYER_WORK *pl_w, CPU_SCRIPT *sc );
extern int goto_hiendif( PLAYER_WORK *pl_w, CPU_SCRIPT *sc );

#endif

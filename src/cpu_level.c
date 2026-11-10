/* The computer's level of play: points each side earns through the fight (the time it lasts, the
   hits it lands, a perfect win), and the level they give, capped by the settings. */
#include "player.h"

#define LEVEL_MAX	32
#define FULL_VITAL	432	/* a team's three vitals, untouched */

/* tables by the setting Sys->cpu_level_type: the first for setting 0, the second for the rest */
static Sint16 hit_rate_tbl0[4] = { 80, 50, 40, 0 } ;
static Sint16 hit_rate_tbl1[4] = { 100, 80, 50, 20 } ;
static Sint16 *hit_rate_tbl[7] = {
	hit_rate_tbl0, hit_rate_tbl1, hit_rate_tbl1, hit_rate_tbl1,
	hit_rate_tbl1, hit_rate_tbl1, hit_rate_tbl1,
} ;
static Sint16 down_tbl0[17] = {
	0, 400, 640, 640, 1040, 1040, 1040, 1040, 1040, 1040, 1040, 1040, 1040, 1040, 1040, 1040, 1040,
} ;
static Sint16 down_tbl1[17] = {
	0, 0, 200, 400, 640, 640, 640, 800, 800, 800, 1040, 1040, 1040, 1040, 1040, 1040, 1040,
} ;
static Sint16 *down_tbl[7] = {
	down_tbl0, down_tbl1, down_tbl1, down_tbl1, down_tbl1, down_tbl1, down_tbl1,
} ;
static Uint8 limit_tbl0[64] = {
	 4,  8, 12, 16, 18, 20, 24, 26,   4,  8, 12, 16, 18, 20, 24, 26,
	 6, 10, 20, 22, 31, 31, 31, 31,   6, 10, 22, 26, 31, 31, 31, 31,
	10, 20, 30, 31, 31, 31, 31, 31,  10, 26, 30, 31, 31, 31, 31, 31,
	12, 26, 31, 31, 31, 31, 31, 31,  31, 31, 31, 31, 31, 31, 31, 31,
} ;
static Uint8 limit_tbl1[64] = {
	 6, 10, 18, 22, 26, 26, 26, 26,   6, 10, 18, 22, 26, 26, 26, 31,
	 8, 18, 26, 28, 31, 31, 31, 31,   8, 18, 26, 31, 31, 31, 31, 31,
	10, 22, 28, 31, 31, 31, 31, 31,  10, 22, 31, 31, 31, 31, 31, 31,
	12, 27, 31, 31, 31, 31, 31, 31,  31, 31, 31, 31, 31, 31, 31, 31,
} ;
static Uint8 *limit_tbl[7] = {
	limit_tbl0, limit_tbl1, limit_tbl1, limit_tbl1, limit_tbl1, limit_tbl1, limit_tbl1,
} ;
/* the points that reach each level */
static Sint16 level_tbl[LEVEL_MAX] = {
	  79,  159,  239,  319,  399,  479,  559,  639,  719,  799,  879,  959, 1039, 1119, 1199, 1279,
	1359, 1439, 1519, 1599, 1679, 1759, 1839, 1919, 1999, 2079, 2159, 2239, 2319, 2399, 2479, 2559,
} ;
/* the points for each 5 seconds of a fight */
static Uint8 time_tbl0[32] = {
	10, 10, 10, 10, 15, 15, 15, 15, 18, 18, 18, 18, 18, 18, 18, 18,
	20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
} ;
static Uint8 time_tbl1[32] = {
	80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
	80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
} ;
static Uint8 *time_tbl[7] = {
	time_tbl0, time_tbl1, time_tbl1, time_tbl1, time_tbl1, time_tbl1, time_tbl1,
} ;

static void level_set( PLAYER_WORK *pl_w ) ;
static void hit_rate_add( PLAYER_WORK *pl_w, Sint16 hit, Sint16 atck ) ;

/****************************************************************/
void cpu_level_init( void )
{
	CPU_LEVEL_WORK *w ;

	w = &cpu_level_work ;
	memset( w, 0, sizeof( CPU_LEVEL_WORK ) ) ;
	w->row = Sys->cpu_renda_level ;
	if( !( Rnd() & 127 ) ) w->row = 0 ;
	if( Sys->cpu_level_max ) w->level = LEVEL_MAX - 1 ;
}

/****************************************************************/
/*			Each second of the fight; every 5 seconds both		*/
/*			sides earn the time's points						*/
/****************************************************************/
void cpu_level_time( void )
{
	Uint8 *tbl ;

	if( Sys->vs_flag ) return ;
	if( -- cpu_level_work.frame >= 0 ) return ;
	cpu_level_work.frame = 59 ;
	cpu_level_work.sec ++ ;
	if( ++ cpu_level_work.sec5 < 5 ) return ;
	cpu_level_work.sec5 = 0 ;
	if( cpu_level_work.sec < 160 ) cpu_level_work.step = ( cpu_level_work.step + 1 ) & 31 ;
	tbl = time_tbl[Sys->cpu_level_type] ;
	pl_work[0].cpu_level_pts += tbl[cpu_level_work.step] ;
	pl_work[1].cpu_level_pts += tbl[cpu_level_work.step] ;
}

/****************************************************************/
/*			The level from the human side's points				*/
/****************************************************************/
void cpu_level_set( void )
{
	PLAYER_WORK *pl_w ;

	if( Sys->vs_flag ) return ;
	pl_w = &pl_work[0] ;
	if( Sys->play_side == 2 ) pl_w = &pl_work[1] ;
	level_set( pl_w ) ;
}

/*==============================================================*/
static void level_set( PLAYER_WORK *pl_w )
{
	Uint32 i ;
	Uint8 *lim ;

	if( Sys->cpu_level_max ){
		cpu_level_work.level = LEVEL_MAX - 1 ;
		return ;
	}
	for( i = 0 ; i < LEVEL_MAX - 1 ; i ++ ){
		if( pl_w->cpu_level_pts <= level_tbl[i] ) break ;
		if( i + 1 >= LEVEL_MAX ) pl_w->cpu_level_pts = ( &level_tbl[i] )[1] ;	/* never true */
	}
	cpu_level_work.level = i ;
	lim = limit_tbl[Sys->cpu_level_type] ;
	lim += cpu_level_work.row * 8 + ( Sys->stage_cnt & 7 ) ;
	if( cpu_level_work.level >= *lim ){
		cpu_level_work.level = *lim ;
		pl_w->cpu_level_pts = level_tbl[cpu_level_work.level] ;
	}
	cpu_level_work.level &= 31 ;
}

/****************************************************************/
/*			The human side's points for the share of its		*/
/*			attacks that hit									*/
/****************************************************************/
void cpu_level_hit( void )
{
	PLAYER_WORK *pl_w ;

	if( Sys->vs_flag ) return ;
	pl_w = &pl_work[0] ;
	if( Sys->play_side == 2 ) pl_w = &pl_work[1] ;
	hit_rate_add( pl_w, vs_sys_w->hit_cnt[pl_w->pl_no], vs_sys_w->atck_cnt[pl_w->pl_no] ) ;
}

/*==============================================================*/
static void hit_rate_add( PLAYER_WORK *pl_w, Sint16 hit, Sint16 atck )
{
	Sint16 rate ;
	Sint16 *tbl ;
	int i ;
	int lim ;

	rate = hit * 100 / atck ;
	tbl = hit_rate_tbl[Sys->cpu_level_type] ;
	for( i = 0, lim = 100 ; i < 3 ; i ++, lim -= 10 ){
		if( rate >= lim ) break ;
	}
	pl_w->cpu_level_pts += tbl[i] ;
}

/****************************************************************/
/*			A side's points lost, and the clock of the			*/
/*			time's points started again							*/
/****************************************************************/
void cpu_level_down( PLAYER_WORK *pl_w )
{
	Sint16 *tbl ;
	int lv ;

	if( lv > 16 ){ }	/* a test of a level never set, left in the arcade's code */
	tbl = down_tbl[Sys->cpu_level_type] ;
	if( ( pl_w->cpu_level_pts -= tbl[16] ) < 0 ) pl_w->cpu_level_pts = 0 ;
	cpu_level_work.step = cpu_level_work.sec5 = cpu_level_work.frame = cpu_level_work.sec = 0 ;
}

/****************************************************************/
/*			A computer team that won without a scratch gives	*/
/*			its side 80 points									*/
/****************************************************************/
void cpu_level_perfect( void )
{
	VS_SYS_WORK *vs_w ;
	Sint16 vital ;

	vs_w = vs_sys_w ;
	if( vs_w->winner_id < 0 ) return ;
	if( !vs_w->team_w[vs_w->winner_id][0]->cpu_flag ) return ;
	vital = vs_w->team_w[vs_w->winner_id][0]->vital_new ;
	vital += vs_w->team_w[vs_w->winner_id][1]->vital_new ;
	vital += vs_w->team_w[vs_w->winner_id][2]->vital_new ;
	if( vital == FULL_VITAL ) pl_work[vs_w->winner_id].cpu_level_pts += 80 ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "cpu_level.c" End of File	*/

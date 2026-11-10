/* The computer's set-up for a round, its chances by the level of play, and its picks of a mode
   and of command lists from the character's data. */
#include "player.h"

/* where in the character's data, or in which shared table, each mode's chance is */
static Uint8 rate_kind_tbl[32] = {
	0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2,
} ;

/* the shared chances, out of 32 by the level of play: modes 3 and 4, and 29 to 31 */
extern Uint8 *em_rate_tbl[2];
extern Uint8 *em_rate_tbl2[3];
/* pairs of tables: a base by the level of play, and a random step */
extern Sint8 *em_rnd_tbl[];

/* an address in the computer's data, from an offset in it */
#define CPU_ADRS( base, ofs )	( ( Uint8 * )( base ) + ( ofs ) )

/****************************************************************/
/*			At a round's start, unless the level is kept or		*/
/*			the player is new									*/
/****************************************************************/
void em_round_init( PLAYER_WORK *pl_w )
{
	if( Sys->cpu_level_max || !( Sys->play_side & 1 << pl_w->pl_no ) ){
		pl_w->cpu_loop_max = 0 ;
		pl_w->fight_sec = 0 ;
		em_init( pl_w ) ;
	}
}

/****************************************************************/
/*			Back to mode 0 at the lowest level					*/
/****************************************************************/
void em_init( PLAYER_WORK *pl_w )
{
	pl_w->cpu_routine_1 = 0 ;
	pl_w->cpu_routine_3 = 0 ;
	pl_w->old_routine_1 = 0 ;
	pl_w->cpu_lv = 0 ;
	pl_w->cpu_ev_lv = 0 ;
	pl_w->cpu_guard_ng_timer = 0 ;
	pl_w->em_atck_no = EM_ATCK_NONE ;
	pl_w->unk_4a8 = -1 ;
	em_cmd_sel( pl_w, 0, 0 ) ;
	em_cmd_clr( pl_w ) ;
}

/****************************************************************/
/*			Clears the command under way						*/
/****************************************************************/
void em_cmd_clr( PLAYER_WORK *pl_w )
{
	pl_w->cpu_cmd_timer = 0 ;
	pl_w->cpu_guard_timer = 0 ;
	pl_w->ev_try_flag = 0 ;
	pl_w->cpu_cancel_flag = 0 ;
	pl_w->cpu_jump_flag = 0 ;
	pl_w->cpu_fly_flag = 0 ;
	pl_w->fly_flag = 0 ;
	pl_w->cpu_lock_flag = 0 ;
	pl_w->cpu_ex_flag = 0 ;
	pl_w->cpu_air_jump_flag = 0 ;
	pl_w->cpu_vcounter_cnt = 0 ;
	pl_w->cpu_guard_back_cnt = 0 ;
	pl_w->cpu_crouch_flag = 0 ;
	pl_w->cpu_crouch_old = 0 ;
	pl_w->cpu_sp_timer = 0 ;
	pl_w->cpu_cmd_old = 0 ;
}

/****************************************************************/
/*			Whether mode no is taken, by its chance at the		*/
/*			level of play; kind 2 for the event modes			*/
/****************************************************************/
int em_rate_ck( PLAYER_WORK *pl_w, int no, int kind )
{
	CPU_DATA *base, *rec, **pp ;
	Uint8 *tbl ;

	switch( rate_kind_tbl[no] ){
	case 0 :
		pp = pl_w->cpu_adrs ;
		rec = pp[kind] ;
		base = rec ;
		if( kind == 2 ) rec += no - 2 ;
		tbl = CPU_ADRS( base, rec->rate ) ;
		break ;
	case 1 :
		if( kind != 2 ) return 0 ;
		tbl = em_rate_tbl[no - 3] ;
		break ;
	case 2 :
		if( kind != 2 ) return 0 ;
		tbl = em_rate_tbl2[no - 29] ;
		break ;
	}
	return ( Rnd() & 31 ) < tbl[cpu_level_work.level] ;
}

/****************************************************************/
/*			A value from the pair of tables no: its base by		*/
/*			the level of play, and a random step				*/
/****************************************************************/
int em_rnd_get( PLAYER_WORK *pl_w, int no )
{
	Sint8 *base, *step ;
	int n ;

	base = em_rnd_tbl[no] ;
	step = em_rnd_tbl[no + 1] ;
	n = base[cpu_level_work.level] + step[Rnd() & 31] ;
	if( n < 0 ) n = 0 ;
	return n ;
}

/****************************************************************/
/*			Picks a command list for mode no from the			*/
/*			character's data of kind							*/
/****************************************************************/
void em_cmd_sel( PLAYER_WORK *pl_w, int no, int kind )
{
	Uint8 *list ;
	CPU_DATA *base, *rec, **pp ;
	CPU_SCRIPT *cw ;
	Sint32 *tbl, *cmd ;
	int sel ;

	pp = pl_w->cpu_adrs ;
	base = rec = pp[kind] ;
	if( kind == 2 ){
		rec += no - 2 ;
		list = ( Uint8 * )rec->sel ;
	} else {
		tbl = ( Sint32 * )CPU_ADRS( base, rec->sel ) ;
		list = ( Uint8 * )tbl[pl_w->cpu_lv] ;
	}
	list += ( int )base ;	/* the list's offset, made an address */
	sel = list[Rnd() & 31] ;
	cmd = ( Sint32 * )CPU_ADRS( base, rec->cmd ) ;
	cw = &pl_w->cpu_norm_sc + kind ;
	cw->adrs = CPU_ADRS( base, cmd[sel] ) ;
	cw->ofs = 0 ;
	cw->no = sel ;
	cw->cmd_cnt = 0 ;
	em_cmd_clr( pl_w ) ;
}

/****************************************************************/
/*			Picks the answer to the attack in em_atck_w and		*/
/*			em_atck_no												*/
/****************************************************************/
void em_answer_sel( PLAYER_WORK *pl_w )
{
	HIT_DATA *hd ;
	CPU_DATA *base, *rec ;
	Sint32 *cmd ;
	Uint8 *sel ;
	int no ;
	PLAYER_WORK *em ;

	em = pl_w->em_atck_w ;
	hd = em->hit_data_tbl ;
	hd += pl_w->em_atck_no ;
	base = pl_w->cpu_answer_adrs ;
	rec = base ;
	rec += hd->cpu_answer_no ;
	cmd = ( Sint32 * )CPU_ADRS( base, rec->cmd ) ;
	sel = CPU_ADRS( base, rec->sel ) ;
	no = sel[Rnd() & 31] ;
	pl_w->cpu_event_sc.adrs = CPU_ADRS( base, cmd[no] ) ;
	pl_w->cpu_event_sc.ofs = 0 ;
	pl_w->cpu_event_sc.no = no ;
	pl_w->cpu_event_sc.cmd_cnt = 0 ;
	em_cmd_clr( pl_w ) ;
}

/* how far away the nearest of the other side's players is */
/****************************************************************/
/*			(em is reused for the loop: with none found it		*/
/*			is left past the last work)							*/
/****************************************************************/
void em_near_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->target_w ;
	Float dist, dx ;
	int i, no ;

	dist = em->pos.x - pl_w->pos.x ;
	if( dist < 0.0f ) dist = -dist ;
	em = &pl_work[pl_w->pl_no ^ 1] ;
	no = -1 ;
	for( i = 0 ; i < 3 ; i ++, em += 2 ){
		if( !em->be_flag ) continue ;
		dx = em->pos.x - pl_w->pos.x ;
		if( dx < 0.0f ) dx = -dx ;
		if( dx > dist ) continue ;
		dist = dx ;
		no = em->work_no ;
	}
	if( no != -1 ) em = &pl_work[no] ;
	pl_w->em_len_x = dist ;
	pl_w->em_dist_y = em->pos.y - pl_w->pos.y ;
	pl_w->em_dist_x = pl_w->em_len_x - em->size_front * 1.6666666f ;
}

/****************************************************************/
/*			Starts event mode no, unless it is on or was		*/
/*			tried; 0 once it starts								*/
/****************************************************************/
int em_mode_set( PLAYER_WORK *pl_w, int no, int lv )
{
	if( pl_w->cpu_routine_1 == no ) return 1 ;
	if( pl_w->ev_try_flag & 1 << no ) return 1 ;
	pl_w->ev_try_flag |= 1 << no ;
	if( pl_w->cpu_routine_1 <= 1 ){
		if( !em_rate_ck( pl_w, no, 2 ) ) return 1 ;
	}
	pl_w->cpu_routine_1 = no ;
	pl_w->cpu_routine_2 = 0 ;
	pl_w->cpu_routine_3 = 0 ;
	em_cmd_clr( pl_w ) ;
	pl_w->cpu_ev_lv = lv ;
	return 0 ;
}

/****************************************************************/
/*			Lets event mode no be tried again					*/
/****************************************************************/
int em_mode_clr( PLAYER_WORK *pl_w, int no )
{
	pl_w->ev_try_flag &= ~( 1 << no ) ;
	return 1 ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "em_cmd.c" End of File	*/

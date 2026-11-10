/* The computer's play: each frame its event checks pick a mode from what the players are doing,
   and the mode runs its command lists. */
#include "player.h"

extern Uint8 pl_guard_ok( PLAYER_WORK *pl_w );

/* the event checks, in the order they are tried, to 0xff */
extern Uint8 em_ev_list[];

static int em_ev_ck( PLAYER_WORK *pl_w );
static int em_norm_set( PLAYER_WORK *pl_w );
static int ev_ck_00( PLAYER_WORK *pl_w, int mode );
static int ev_ck_01( PLAYER_WORK *pl_w, int mode );
static int ev_ck_catch( PLAYER_WORK *pl_w, int mode );
static int ev_ck_guard( PLAYER_WORK *pl_w, int mode );
static int ev_ck_guard_after( PLAYER_WORK *pl_w, int mode );
static int ev_ck_air_guard_after( PLAYER_WORK *pl_w, int mode );
static int ev_ck_dm_after( PLAYER_WORK *pl_w, int mode );
static int ev_ck_air_dm_after( PLAYER_WORK *pl_w, int mode );
static int ev_ck_get_up_after( PLAYER_WORK *pl_w, int mode );
static int ev_ck_em_dash( PLAYER_WORK *pl_w, int mode );
static int ev_ck_em_fly( PLAYER_WORK *pl_w, int mode );
static int ev_ck_em_jump( PLAYER_WORK *pl_w, int mode );
static int ev_ck_em_atck( PLAYER_WORK *pl_w, int mode );
static void em_hit_ck( PLAYER_WORK *pl_w );
static int em_mode_run( PLAYER_WORK *pl_w );
static int em_mode_norm( PLAYER_WORK *pl_w );
static int em_mode_next( PLAYER_WORK *pl_w );
static int em_mode_event( PLAYER_WORK *pl_w );
static int em_mode_em_move( PLAYER_WORK *pl_w );
static int em_mode_answer( PLAYER_WORK *pl_w );
static int em_mode_ex( PLAYER_WORK *pl_w );
static int em_cmd_run( PLAYER_WORK *pl_w, int no );
static int em_cmd_end( PLAYER_WORK *pl_w, int no );

/****************************************************************/
/*			The computer's turn, each frame it plays			*/
/****************************************************************/
void em_play( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	Uint32 act ;
	int sw ;

	pl_w->cpu_loop_cnt = 0 ;
	pl_w->cpu_sp_flag = 0 ;
	if( Sys->cpu_level_max ) pl_w->cpu_lv = 7 ;
	pl_w->pad_sw.now = 0 ;
	if( pl_w->cpu_guard_ng_timer ) pl_w->cpu_guard_ng_timer -- ;
	if( pl_w->team_no ) return ;
	if( vs_sys_work.team_hyper_flag & 1 << pl_w->pl_no ) return ;
	if( pl_w->hit_stop ) return ;
	if( pl_w->cpu_guard_timer ) pl_w->cpu_guard_timer -- ;
	if( vs_sys_work.vs_st > VS_FIGHT ){
		em_init( pl_w ) ;
		return ;
	}
	if( vs_sys_work.ctrl_flag & CTRL_NO_INPUT ) return ;
	em = pl_w->target_w ;
	if( em->fly_screen_flag ) return ;
	do {
		if( em_ev_ck( pl_w ) == 2 ) em_norm_set( pl_w ) ;
		em_hit_ck( pl_w ) ;
	} while( em_mode_run( pl_w ) == 1 ) ;
	if( pl_w->cpu_crouch_flag ){
		sw = 0x1000 ;
		if( pl_w->cpu_crouch_flag == pl_w->cpu_crouch_old ){
			act = pl_w->act_st ;
			if( act != N_CROUCH && act != N_CROUCHING && act != N_CROUCH_TURN && act != N_NML_ATCK ){
				pl_w->cpu_crouch_flag = 0 ;
				sw = 0 ;
			}
		}
		pl_w->pad_sw.now |= sw ;
	}
	pl_w->cpu_crouch_old = pl_w->cpu_crouch_flag ;
	pl_w->old_routine_1 = pl_w->routine_1 ;
}

/*==============================================================*/
/*			Runs the event checks until one decides: 0 none,	*/
/*			1 try the next, 2 back to mode 0					*/
/*==============================================================*/
static int em_ev_ck( PLAYER_WORK *pl_w )
{
	static int ( *ev_ck_jmp[] )( PLAYER_WORK *pl_w, int mode ) = {
		ev_ck_00, ev_ck_01, ev_ck_catch, ev_ck_guard, ev_ck_guard, ev_ck_guard_after, ev_ck_air_guard_after, ev_ck_dm_after,
		ev_ck_air_dm_after, ev_ck_get_up_after, ev_ck_em_dash, ev_ck_em_fly, ev_ck_em_jump, ev_ck_em_atck,
	} ;
	Uint8 *ev ;
	int ret ;

	if( vs_sys_work.ctrl_flag & CTRL_WALK ) return 0 ;
	if( pl_w->routine_1 > PL_CATCH ) return 2 ;
	if( pl_w->cpu_lock_flag ) return 0 ;
	if( pl_w->cpu_routine_1 == 13 ) return 0 ;
	if( !em_act_ck( pl_w, 15 ) ) return 0 ;
	ev = em_ev_list ;
	do {
		ret = ev_ck_jmp[*ev++]( pl_w, pl_w->cpu_ev_lv ) ;
		if( *ev == 0xff ) ret = 0 ;
	} while( ret == 1 ) ;
	return ret ;
}

/*==============================================================*/
/*			Back to mode 0										*/
/*==============================================================*/
static int em_norm_set( PLAYER_WORK *pl_w )
{
	if( pl_w->cpu_routine_1 ){
		pl_w->cpu_routine_1 = 0 ;
		pl_w->cpu_routine_2 = 1 ;
		pl_w->cpu_routine_3 = 0 ;
		em_cmd_clr( pl_w ) ;
		pl_w->em_atck_no = 0 ;
		pl_w->unk_4a8 = 0 ;
		pl_w->cpu_ev_lv = 0 ;
	}
	return 0 ;
}

/*==============================================================*/
static int ev_ck_00( PLAYER_WORK *pl_w, int mode )
{
	pl_w->cpu_ev_lv = 0 ;
	return 0 ;
}

/*==============================================================*/
static int ev_ck_01( PLAYER_WORK *pl_w, int mode )
{
	pl_w->cpu_ev_lv = 1 ;
	return 0 ;
}

/*==============================================================*/
/*			A throw												*/
/*==============================================================*/
static int ev_ck_catch( PLAYER_WORK *pl_w, int mode )
{
	if( mode > 2 ) return 0 ;
	if( !pl_w->fly_flag && pl_catch_ck( pl_w ) ) return em_mode_set( pl_w, 2, 2 ) ;
	else return em_mode_clr( pl_w, 2 ) ;
}

/*==============================================================*/
static int ev_ck_guard( PLAYER_WORK *pl_w, int mode )
{
	return 1 ;
}

/****************************************************************/
/*			Guards a hit: mode 3 (4 in the air), by its			*/
/*			chance; called from guard_ck						*/
/****************************************************************/
int em_guard_ck( PLAYER_WORK *pl_w )
{
	int no ;

	if( pl_w->cpu_ev_lv > 3 ) return 0 ;
	if( !em_act_ck( pl_w, 1 ) ) return 0 ;
	if( !pl_guard_ok( pl_w ) ) return 0 ;
	if( pl_w->cpu_guard_ng_timer ) return 0 ;
	if( !pl_w->cpu_guard_timer ){
		no = 3 ;
		if( pl_w->pl_stance == STANCE_AIR ) no = 4 ;
		if( !em_rate_ck( pl_w, no, 2 ) ) return 0 ;
	}
	pl_w->cpu_routine_1 = 3 ;
	pl_w->cpu_routine_2 = 0 ;
	pl_w->cpu_routine_3 = 0 ;
	pl_w->cpu_ev_lv = 3 ;
	em_cmd_clr( pl_w ) ;
	return 1 ;
}

/*==============================================================*/
static int ev_ck_guard_after( PLAYER_WORK *pl_w, int mode )
{
	if( mode > 3 ) return 0 ;
	if( pl_w->fly_flag ) return em_mode_clr( pl_w, 5 ) ;
	if( pl_w->act_st == pl_w->old_act_st ) return 1 ;
	if( pl_w->old_act_st == N_GUARD && pl_w->act_st != N_AIR_GUARD ) return em_mode_set( pl_w, 5, 3 ) ;
	else return em_mode_clr( pl_w, 5 ) ;
}

/*==============================================================*/
static int ev_ck_air_guard_after( PLAYER_WORK *pl_w, int mode )
{
	if( mode > 3 ) return 0 ;
	if( pl_w->fly_flag ) return em_mode_clr( pl_w, 6 ) ;
	if( pl_w->act_st == pl_w->old_act_st ) return 1 ;
	if( pl_w->old_act_st == N_AIR_GUARD && pl_w->act_st != N_GUARD ) return em_mode_set( pl_w, 6, 3 ) ;
	else return em_mode_clr( pl_w, 6 ) ;
}

/*==============================================================*/
static int ev_ck_dm_after( PLAYER_WORK *pl_w, int mode )
{
	if( mode > 3 ) return 0 ;
	if( !pl_w->fly_flag ){
		if( pl_w->routine_1 == pl_w->old_routine_1 ) return 1 ;
		if( pl_w->old_routine_1 == 3 && pl_w->pl_stance != STANCE_AIR ) return em_mode_set( pl_w, 7, 3 ) ;
		else return em_mode_clr( pl_w, 7 ) ;
	} else return em_mode_clr( pl_w, 7 ) ;
}

/*==============================================================*/
static int ev_ck_air_dm_after( PLAYER_WORK *pl_w, int mode )
{
	if( mode > 3 ) return 0 ;
	if( !pl_w->fly_flag ){
		if( pl_w->routine_1 == pl_w->old_routine_1 ) return 1 ;
		if( pl_w->old_routine_1 == 3 && pl_w->pl_stance == STANCE_AIR ) return em_mode_set( pl_w, 7, 3 ) ;
		else return em_mode_clr( pl_w, 7 ) ;
	} else return em_mode_clr( pl_w, 7 ) ;
}

/*==============================================================*/
static int ev_ck_get_up_after( PLAYER_WORK *pl_w, int mode )
{
	if( mode > 4 ) return 0 ;
	if( !pl_w->fly_flag && pl_w->old_act_st == N_GET_UP ) return em_mode_set( pl_w, 9, 4 ) ;
	else return em_mode_clr( pl_w, 9 ) ;
}

/*==============================================================*/
static int ev_ck_em_dash( PLAYER_WORK *pl_w, int mode )
{
	PLAYER_WORK *em = pl_w->target_w ;

	if( mode > 3 ) return 0 ;
	if( pl_w->fly_flag || pl_w->pl_stance == STANCE_AIR || em->act_st != N_DASH ) return em_mode_clr( pl_w, 10 ) ;
	return em_mode_set( pl_w, 10, 3 ) ;
}

/*==============================================================*/
static int ev_ck_em_fly( PLAYER_WORK *pl_w, int mode )
{
	PLAYER_WORK *em = pl_w->target_w ;

	if( mode > 3 ) return 0 ;
	if( !pl_w->fly_flag && pl_w->pl_stance != STANCE_AIR && ( em->fly_flag || em->act_st == N_SJUMP_AIR ) ) return em_mode_set( pl_w, 11, 3 ) ;
	return em_mode_clr( pl_w, 11 ) ;
}

/*==============================================================*/
static int ev_ck_em_jump( PLAYER_WORK *pl_w, int mode )
{
	PLAYER_WORK *em = pl_w->target_w ;

	if( mode > 3 ) return 0 ;
	if( !pl_w->fly_flag && pl_w->pl_stance != STANCE_AIR && em->act_st == N_JUMP_AIR ) return em_mode_set( pl_w, 12, 3 ) ;
	return em_mode_clr( pl_w, 12 ) ;
}

/*==============================================================*/
/*			An attack coming: answers it once					*/
/*==============================================================*/
static int ev_ck_em_atck( PLAYER_WORK *pl_w, int mode )
{
	PLAYER_WORK *ew ;

	if( mode > 3 ) return 0 ;
	if( !pl_w->fly_flag && pl_w->pl_stance != STANCE_AIR && ( ew = em_atck_near( pl_w ) ) ){
		if( ew == pl_w->em_atck_w &&( ew->atck_no & ATCK_NO_MASK ) == pl_w->em_atck_no ) return 1 ;
		if( ew->work_kind == W_SHL || ew->work_kind == W_PLS ){
			pl_w->em_atck_w = ew ;
			pl_w->em_atck_no = ew->atck_no & ATCK_NO_MASK ;
			return em_mode_set( pl_w, 13, 3 ) ;
		}
		if( !ew->work_kind && ( ew->act_st == N_NML_ATCK || ew->act_st == N_SPECIAL ) ){
			pl_w->em_atck_w = ew ;
			pl_w->em_atck_no = ew->atck_no & ATCK_NO_MASK ;
			return em_mode_set( pl_w, 13, 3 ) ;
		}
	}
	pl_w->em_atck_w = 0 ;
	pl_w->em_atck_no = EM_ATCK_NONE ;
	return em_mode_clr( pl_w, 13 ) ;
}

/*==============================================================*/
/*			Keeps whether the computer's attack hit (0x80)		*/
/*			or was guarded (2)									*/
/*==============================================================*/
static void em_hit_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->cpu_cancel_flag ){
		if( !( pl_w->cpu_cancel_flag & CC_GUARD ) && pl_w->hit_flag ){
			pl_w->cpu_cancel_flag |= ~0x7f ;
			if( pl_w->hit_flag & HR_GUARD ) pl_w->cpu_cancel_flag |= CC_GUARD ;
		}
	}
}

/*==============================================================*/
/*			Runs the mode; 1 to check the events again			*/
/*==============================================================*/
static int em_mode_run( PLAYER_WORK *pl_w )
{
	static int ( *mode_jmp[] )( PLAYER_WORK *pl_w ) = {
		em_mode_norm, em_mode_next, em_mode_event, em_mode_event, em_mode_event, em_mode_event, em_mode_event, em_mode_event,
		em_mode_event, em_mode_event, em_mode_em_move, em_mode_em_move, em_mode_em_move, em_mode_answer, em_mode_answer,
	} ;
	int ret ;

	ret = 1 ;
	while( ret == 1 ){
		vs_sys_work.move_again_flag = 0 ;
		if( ++ pl_w->cpu_loop_cnt >= pl_w->cpu_loop_max ){
			pl_w->cpu_loop_max = pl_w->cpu_loop_cnt ;
			if( pl_w->cpu_loop_max > 31 ){
				ret = 0 ;
				continue ;
			}
		}
		if( pl_w->routine_1 != PL_NORM ) ret = 0 ;
		else if( pl_w->cpu_ex_flag ) ret = em_mode_ex( pl_w ) ;
		else ret = mode_jmp[pl_w->cpu_routine_1]( pl_w ) ;
	}
	switch( ret ){
	case 0 :
		ret = 0 ;
		break ;
	default :
		ret = 0 ;
		break ;
	case 2 :
		ret = 1 ;
		break ;
	}
	return ret ;
}

/*==============================================================*/
/*			Mode 0: the usual commands							*/
/*==============================================================*/
static int em_mode_norm( PLAYER_WORK *pl_w )
{
	switch( pl_w->cpu_routine_2 ){
	case 0 :
		pl_w->cpu_routine_2 ++ ;
		if( vs_sys_work.ctrl_flag & CTRL_WALK ) pl_w->cpu_lv = 8 ;
		em_cmd_sel( pl_w, 0, 0 ) ;
	case 1 :
		if( !( vs_sys_work.ctrl_flag & CTRL_WALK ) && em_lv_ck( pl_w ) ) em_cmd_sel( pl_w, 0, 0 ) ;
		return em_cmd_run( pl_w, 0 ) ;
	case 2 :
		return em_cmd_end( pl_w, 0 ) ;
	}
}

/*==============================================================*/
/*			Mode 1												*/
/*==============================================================*/
static int em_mode_next( PLAYER_WORK *pl_w )
{
	switch( pl_w->cpu_routine_2 ){
	case 0 :
		pl_w->cpu_routine_2 ++ ;
		em_cmd_sel( pl_w, 1, 1 ) ;
	case 1 :
		if( !( vs_sys_work.ctrl_flag & CTRL_WALK ) && em_lv_ck( pl_w ) ) em_cmd_sel( pl_w, 0, 1 ) ;
		return em_cmd_run( pl_w, 1 ) ;
	case 2 :
		return em_cmd_end( pl_w, 1 ) ;
	}
}

/*==============================================================*/
/*			Modes 2 to 9: the event's own commands				*/
/*==============================================================*/
static int em_mode_event( PLAYER_WORK *pl_w )
{
	switch( pl_w->cpu_routine_2 ){
	case 0 :
		pl_w->cpu_routine_2 ++ ;
		em_cmd_sel( pl_w, pl_w->cpu_routine_1, 2 ) ;
	case 1 :
		return em_cmd_run( pl_w, 2 ) ;
	case 2 :
		return em_cmd_end( pl_w, 2 ) ;
	}
}

/*==============================================================*/
/*			Modes 10 to 12: given up once the opponent is		*/
/*			down or the computer is hit							*/
/*==============================================================*/
static int em_mode_em_move( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	int st ;

	if( !pl_w->cpu_lock_flag ){
		em = pl_w->target_w ;
		if( em->routine_1 == PL_NORM ){
			if( em->act_st <= N_WALK ){
				em_norm_set( pl_w ) ;
				em_hit_ck( pl_w ) ;
				return 1 ;
			}
			st = pl_w->act_st ;
			if( st >= 5 && st <= 9 ){
				em_norm_set( pl_w ) ;
				em_hit_ck( pl_w ) ;
				return 1 ;
			}
		}
	}
	return em_mode_event( pl_w ) ;
}

/*==============================================================*/
/*			Modes 13 and 14: the answer to an attack			*/
/*==============================================================*/
static int em_mode_answer( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	int st ;

	if( !pl_w->cpu_lock_flag ){
		em = pl_w->target_w ;
		if( em->routine_1 == PL_NORM ){
			if( em->act_st <= N_WALK ){
				em_norm_set( pl_w ) ;
				em_hit_ck( pl_w ) ;
				return 1 ;
			}
			st = pl_w->act_st ;
			if( st >= 5 && st <= 9 ){
				em_norm_set( pl_w ) ;
				em_hit_ck( pl_w ) ;
				return 1 ;
			}
		}
	}
	switch( pl_w->cpu_routine_2 ){
	case 0 :
		pl_w->cpu_routine_2 ++ ;
		em_answer_sel( pl_w ) ;
	case 1 :
		return em_cmd_run( pl_w, 2 ) ;
	case 2 :
		return em_cmd_end( pl_w, 2 ) ;
	}
}

/*==============================================================*/
/*			While cpu_ex_flag is set: the cpu_ex_adrs commands			*/
/*==============================================================*/
static int em_mode_ex( PLAYER_WORK *pl_w )
{
	switch( pl_w->cpu_routine_2 ){
	case 0 :
		pl_w->cpu_routine_2 ++ ;
		pl_w->cpu_ex_sc.adrs = ( Uint8 * )pl_w->cpu_ex_adrs + pl_w->cpu_ex_adrs[pl_w->cpu_ex_sc.no] ;
		pl_w->cpu_ex_sc.ofs = 0 ;
		pl_w->cpu_ex_sc.cmd_cnt = 0 ;
	case 1 :
		return em_cmd_run( pl_w, 3 ) ;
	case 2 :
		return em_cmd_end( pl_w, 3 ) ;
	}
}

/*==============================================================*/
/*			Reads command list no, by way of a jump or a		*/
/*			fall when one is due								*/
/*==============================================================*/
static int em_cmd_run( PLAYER_WORK *pl_w, int no )
{
	CPU_SCRIPT *cw = &pl_w->cpu_norm_sc + no ;

	if( pl_w->cpu_jump_flag > 0 && pl_w->act_st != N_JUMP && pl_w->pl_stance != STANCE_AIR ){
		pl_w->cpu_cancel_flag = pl_w->cpu_jump_flag = 0 ;
		return goto_endif( pl_w, cw ) ;
	}
	if( pl_w->cpu_jump_flag < 0 && pl_w->act_st != N_SJUMP && ( pl_w->pl_stance != STANCE_AIR || !pl_w->sjump_flag ) ){
		pl_w->cpu_cancel_flag = pl_w->cpu_jump_flag = 0 ;
		return goto_endif( pl_w, cw ) ;
	}
	if( pl_w->cpu_fly_flag && !pl_w->fly_flag ){
		pl_w->cpu_cancel_flag = pl_w->cpu_fly_flag = 0 ;
		return goto_hiendif( pl_w, cw ) ;
	}
	return cmd_start( pl_w, cw ) ;
}

/*==============================================================*/
static int em_cmd_end( PLAYER_WORK *pl_w, int no )
{
	register CPU_SCRIPT *cw = &pl_w->cpu_norm_sc + no ;

	return cmd_move( pl_w, cw ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "em_play.c" End of File	*/

/* The fight's progress in vs_sys_work: the entrances, the clock, KOs and time over, the winner and
   the scores, and the arcade's display of each side's score and wins. */
#include "player.h"
#include "pl_color.h"

#define VS_ROUND_SIZE	0x94	/* the part of vs_sys_work set up again for each fight */
#define FULL_VITAL	432	/* a team's three vitals, untouched */

extern int sprintf( char *s, const char *fmt, ... ) ;
extern void vs_msg_set( int no ) ;	/* the fight's start and end messages */
extern void vs_res_set( int no ) ;	/* the result messages */
extern void finish_ef_set( void ) ;

static void vs_move_init( VS_SYS_WORK *vs_w ) ;
static void vs_move_wait( VS_SYS_WORK *vs_w ) ;
static void vs_move_appear( VS_SYS_WORK *vs_w ) ;
static void vs_move_ready( VS_SYS_WORK *vs_w ) ;
static void vs_move_fight( VS_SYS_WORK *vs_w ) ;
static int ko_ck( VS_SYS_WORK *vs_w ) ;
static void time_over_ck( VS_SYS_WORK *vs_w ) ;
static int win_set_1p( VS_SYS_WORK *vs_w ) ;
static int win_set_2p( VS_SYS_WORK *vs_w ) ;
static int draw_set( VS_SYS_WORK *vs_w ) ;
static void pl_round_end_clr( PLAYER_WORK *pl_w ) ;
static void round_end_set( VS_SYS_WORK *vs_w ) ;
static void vs_move_end_wait( VS_SYS_WORK *vs_w ) ;
static void win_mes_set( void ) ;
static void vs_move_win( VS_SYS_WORK *vs_w ) ;
static void vs_move_boss( VS_SYS_WORK *vs_w ) ;
static void vs_move_bonus( VS_SYS_WORK *vs_w ) ;
static void vs_move_exit( VS_SYS_WORK *vs_w ) ;
static void vs_rule_set( VS_SYS_WORK *vs_w ) ;
static Uint8 pl_end_wait_ck( PLAYER_WORK *pl_w ) ;
static int land_flag_get( void ) ;
static int wall_flag_get( void ) ;
static void win_cnt_set( VS_SYS_WORK *vs_w ) ;
static void round_time_init( void ) ;
static int round_time_ck( VS_SYS_WORK *vs_w ) ;
static void time_frame_set( VS_SYS_WORK *vs_w ) ;
static void pl_stand_all( void ) ;

/****************************************************************/
/*			Set up the match: the teams in their chosen			*/
/*			order												*/
/****************************************************************/
void vs_sys_init( void )
{
	memset( &vs_sys_work, 0, sizeof( VS_SYS_WORK ) ) ;
	vs_sys_w = &vs_sys_work ;
	pl_work[0].team_no = pl_work[0].team_order ;
	pl_work[2].team_no = pl_work[2].team_order ;
	pl_work[4].team_no = pl_work[4].team_order ;
	pl_work[1].team_no = pl_work[1].team_order ;
	pl_work[3].team_no = pl_work[3].team_order ;
	pl_work[5].team_no = pl_work[5].team_order ;
	vs_sys_work.team_w[0][0] = &pl_work[0] ;
	vs_sys_work.team_w[0][1] = &pl_work[2] ;
	vs_sys_work.team_w[0][2] = &pl_work[4] ;
	vs_sys_work.team_w[1][0] = &pl_work[1] ;
	vs_sys_work.team_w[1][1] = &pl_work[3] ;
	vs_sys_work.team_w[1][2] = &pl_work[5] ;
	vs_sys_work.speed_rate = 6 ;
	vs_sys_work.speed_cnt = 1 ;
	vs_sys_work.speed_flag = 0 ;
}

/****************************************************************/
/*			Set up a fight: the clock, and each player in		*/
/*			its team's place									*/
/****************************************************************/
void vs_round_init( void )
{
	int i ;

	memset( &vs_sys_work, 0, VS_ROUND_SIZE ) ;
	vs_sys_work.speed_rate = 6 ;
	vs_sys_work.speed_cnt = 1 ;
	vs_sys_work.speed_flag = 0 ;
	vs_sys_work.game_mode = stage_no ;
	round_time_init() ;
	pl_work[0].team_no = pl_work[0].team_order ;
	pl_work[2].team_no = pl_work[2].team_order ;
	pl_work[4].team_no = pl_work[4].team_order ;
	pl_work[1].team_no = pl_work[1].team_order ;
	pl_work[3].team_no = pl_work[3].team_order ;
	pl_work[5].team_no = pl_work[5].team_order ;
	i = 0 ;
	while( i < 6 ){
		vs_sys_work.team_w[i & 1][pl_work[i].team_no] = &pl_work[i] ;
		i ++ ;
	}
}

/****************************************************************/
/*			Each frame: the fight's step, then the score		*/
/*			display												*/
/****************************************************************/
void vs_sys_move( void )
{
	static void ( *vs_jmp[10] )( VS_SYS_WORK *vs_w ) = {
		vs_move_init, vs_move_wait, vs_move_appear, vs_move_ready, vs_move_fight,
		vs_move_end_wait, vs_move_win, vs_move_boss, vs_move_bonus, vs_move_exit,
	} ;

	vs_jmp[vs_sys_work.vs_st]( &vs_sys_work ) ;
	naomi_level_disp() ;
}

/*==============================================================*/
/*			The fight's start									*/
/*==============================================================*/
static void vs_move_init( VS_SYS_WORK *vs_w )
{
	Sys->entry_stop = 1 ;
	Sys->hyper_bg = 0 ;
	vs_w->ctrl_flag = CTRL_NO_INPUT | CTRL_NO_THROW ;
	vs_w->first_hit = 0 ;
	vs_w->time_over = 0 ;
	vs_w->ko_hit_data = 0 ;
	vs_w->ko_atck_no = 0 ;
	vs_w->ko_at_w = 0 ;
	vs_w->vs_timer = 1 ;
	if( !vs_w->round_cnt ){
		scr_excep_set( 0, 11, 11 ) ;
	} else {
		if( vs_w->game_mode == 8 && Sys->boss_flag ){
			Sys->entry_stop = 0 ;
			if( vs_w->round_cnt == 1 ){
				vs_w->vs_timer = 420 ;
				stage_work.event = 1 ;
			} else {
				vs_w->vs_timer = 300 ;
				stage_work.event = 2 ;
			}
		}
		pl_stand_all() ;
	}
	vs_w->vs_st = VS_WAIT ;
}

/*==============================================================*/
/*			Wait, then the entrances							*/
/*==============================================================*/
static void vs_move_wait( VS_SYS_WORK *vs_w )
{
	if( -- vs_w->vs_timer > 0 ) return ;
	vs_rule_set( vs_w ) ;
	if( !vs_w->round_cnt ) vs_w->vs_timer = 120 ;
	else vs_w->vs_timer = 1 ;
	Sys->entry_stop = 0 ;
	vs_w->vs_st = VS_APPEAR ;
}

/*==============================================================*/
/*			The entrances: the point players don't push			*/
/*==============================================================*/
static void vs_move_appear( VS_SYS_WORK *vs_w )
{
	vs_w->team_w[0][0]->push_ng_timer = vs_w->team_w[1][0]->push_ng_timer = 1 ;
	if( -- vs_w->vs_timer > 0 ) return ;
	vs_w->vs_timer = 90 ;	/* set over at once */
	vs_w->vs_timer = 250 ;
	vs_w->vs_st = VS_READY ;
	vs_w->ctrl_flag = CTRL_WALK ;
	vs_msg_set( 0 ) ;
}

/*==============================================================*/
/*			The start message, then the fight					*/
/*==============================================================*/
static void vs_move_ready( VS_SYS_WORK *vs_w )
{
	if( vs_w->vs_timer == 60 ) vs_msg_set( 1 ) ;
	if( -- vs_w->vs_timer > 0 ) return ;
	scr_excep_clr() ;
	pl_work[0].pl_new_lvbt = pl_work[0].pl_last_lvbt = pl_work[0].pad_sw.now & ( KEY_LEVER | KEY_BUTTON | KEY_START ) ;
	pl_work[1].pl_new_lvbt = pl_work[1].pl_last_lvbt = pl_work[1].pad_sw.now & ( KEY_LEVER | KEY_BUTTON | KEY_START ) ;
	vs_w->ctrl_flag = CTRL_PLAY | CTRL_NO_THROW ;
	vs_w->vs_timer = 60 ;
	vs_w->win_skip_flag = 0 ;
	vs_w->vs_st = VS_FIGHT ;
	if( vs_w->round_cnt && vs_w->game_mode == 8 && Sys->boss_flag ){
		stream_stop() ;
		s3l_fade_in_set( 1 ) ;
		stream_req( vs_w->round_cnt == 1 ? 9 : 10 ) ;
		stream_start() ;
	}
}

/*==============================================================*/
/*			The fight, until a KO or time over					*/
/*==============================================================*/
static void vs_move_fight( VS_SYS_WORK *vs_w )
{
	if( vs_w->vs_timer ){
		if( -- vs_w->vs_timer <= 0 ) vs_w->ctrl_flag = CTRL_PLAY ;
	}
	if( !ko_ck( vs_w ) ){
		if( round_time_ck( vs_w ) ) return ;
		time_over_ck( vs_w ) ;
	}
	round_end_set( vs_w ) ;
}

/*==============================================================*/
/*			A team with no vital left loses						*/
/*==============================================================*/
static int ko_ck( VS_SYS_WORK *vs_w )
{
	Sint16 vital0, vital1 ;

	vital0 = pl_work[0].vital_new + pl_work[2].vital_new + pl_work[4].vital_new ;
	vital1 = pl_work[1].vital_new + pl_work[3].vital_new + pl_work[5].vital_new ;
	if( vital0 + vital1 == 0 ) return draw_set( vs_w ) ;
	if( !vital1 ) return win_set_1p( vs_w ) ;
	if( !vital0 ) return win_set_2p( vs_w ) ;
	return 0 ;
}

/*==============================================================*/
/*			Time over: the team with more vital wins			*/
/*==============================================================*/
static void time_over_ck( VS_SYS_WORK *vs_w )
{
	Sint16 vital0, vital1 ;
	PLAYER_WORK *pl_w ;

	vs_w->time_over = 1 ;
	vital0 = pl_work[0].vital_new + pl_work[2].vital_new + pl_work[4].vital_new ;
	vital1 = pl_work[1].vital_new + pl_work[3].vital_new + pl_work[5].vital_new ;
	if( vital0 == vital1 ){
		draw_set( vs_w ) ;
		return ;
	}
	if( vital0 > vital1 ) win_set_1p( vs_w ) ;
	else win_set_2p( vs_w ) ;
	pl_w = vs_w->team_w[vs_w->winner_id][0] ;
	if( !Sys->vs_flag && pl_w->pl_type == PL_ABYSS2 && pl_w->cpu_flag ) vs_w->winner_id ^= 1 ;
}

/*==============================================================*/
static int win_set_1p( VS_SYS_WORK *vs_w )
{
	vs_w->winner_id = 0 ;
	return 1 ;
}

/*==============================================================*/
static int win_set_2p( VS_SYS_WORK *vs_w )
{
	vs_w->winner_id = 1 ;
	return 1 ;
}

/*==============================================================*/
static int draw_set( VS_SYS_WORK *vs_w )
{
	vs_w->winner_id = -1 ;
	return 1 ;
}

/*==============================================================*/
/*			Clear a player's commands at the fight's end		*/
/*==============================================================*/
static void pl_round_end_clr( PLAYER_WORK *pl_w )
{
	memset( &pl_w->dash_w, 0, sizeof( COM_WORK ) * 2 ) ;
	pl_w->win_flag = 0 ;
	pl_w->win_wait = 4 ;
	pl_w->fly_flag = 0 ;
}

/*==============================================================*/
/*			The fight is over: the result, its message and		*/
/*			the last hit's bonus. kind is how it ended;			*/
/*			nothing here reads it, but its tests remain			*/
/*==============================================================*/
static void round_end_set( VS_SYS_WORK *vs_w )
{
	PLAYER_WORK *pl_w ;
	PLAYER_WORK *em ;
	HIT_DATA *hd ;
	int flag ;
	int kind ;
	int i ;

	vs_w->ctrl_flag = CTRL_NO_INPUT ;
	vs_w->vs_st = VS_END_WAIT ;
	vs_w->pose_wait_timer = 150 ;
	vs_w->vs_timer = 360 ;
	for( i = 0 ; i < 6 ; i ++ ) pl_round_end_clr( &pl_work[i] ) ;
	kind = 0 ;
	if( vs_w->time_over ){
		vs_res_set( 2 ) ;
		kind = 4 ;
	} else {
		vs_msg_set( 2 ) ;
		if( vs_w->winner_id >= 0 ){
			flag = 0 ;
			em = vs_w->ko_at_w ;
			if( em && em->pl_type == PL_HAYATO ) flag = ( ( int ( * )( PLAYER_WORK * ) )em->pl_tbl[TBL_EX0] )( em ) ;
			pl_w = vs_w->team_w[vs_w->winner_id][0] ;
			hd = vs_w->ko_hit_data ;
			if( hd ){
				if( ( hd->atck_kind & A_HYPER ) || flag ){
					flash_finish_set() ;
					finish_ef_set() ;
					if( vs_w->team_hyper_flag && vs_w->be_cnt[vs_w->winner_id] > 1 ){
						if( vs_w->be_cnt[vs_w->winner_id] > 2 ) vs_w->bonus_lo = 0x50000000 ;
						else vs_w->bonus_lo = 0x30000000 ;
						vs_w->bonus_hi = 0 ;
						kind = 2 ;
					} else if( pl_w->team_type == 6 ){
						vs_w->bonus_hi = 0 ;
						vs_w->bonus_lo = 0x70000000 ;
						kind = 3 ;
					} else {
						vs_w->bonus_hi = 0 ;
						vs_w->bonus_lo = 0x10000000 ;
						kind = 1 ;
					}
					score_add( pl_w, vs_w->bonus_hi, vs_w->bonus_lo ) ;
					em = vs_w->ko_at_w ;
					if( em && em->pl_type == PL_AKUMA && vs_w->ko_atck_no == 34 ) pl_voice_req( em, 32 ) ;
					else pl_se_req( em, 65 ) ;
					na_req( 32 ) ;
				} else {
					pl_se_req( pl_w, 66 ) ;
					na_req( 31 ) ;
					kind = 0 ;
				}
			}
		}
		vs_w->slow_bit = SLOW_HALF ;
		Sys->hyper_bg = 0 ;
		vs_w->hyper_bg_req = 0 ;
	}
	if( vs_w->winner_id < 0 ){
		if( vs_w->time_over ) kind = 6 ;
		else kind = 5 ;
	}
	Sys->entry_stop = 1 ;
	vs_w->vs_end_flag = 1 ;
	if( vs_w->winner_id >= 0 ){
		pl_w = vs_w->team_w[vs_w->winner_id][0] ;
		if( !pl_w->cpu_flag && vs_w->game_mode == 8 && Sys->boss_flag ){
			if( vs_w->round_cnt == 2 ){
				if( !Sys->vs_flag ){
					Sys->ending_flag = 1 ;
					Sys->name_stop = 1 ;
					vs_w->ending_wait_flag = 1 ;
					vs_w->ending_wait_timer = 60 ;
					hyper_ef_set( vs_w->team_w[vs_w->winner_id][0], 7 ) ;
					flash_ending_set() ;
				}
			} else {
				Sys->entry_stop = 0 ;
				vs_w->vs_end_flag = 0 ;
			}
		}
	}
	Sys->entry_stop = 1 ;
	win_cnt_set( vs_w ) ;
}

/*==============================================================*/
/*			The winner's pose: wait for every player to			*/
/*			finish what it is doing								*/
/*==============================================================*/
static void vs_move_end_wait( VS_SYS_WORK *vs_w )
{
	int i ;

	if( vs_w->game_stop ) return ;
	if( vs_w->ending_wait_flag ){
		if( -- vs_w->ending_wait_timer > 0 ) return ;
		vs_w->ending_wait_flag = 0 ;
	}
	if( flash_flag_get() ) return ;
	if( Sys->hyper_bg ) return ;
	if( -- vs_w->vs_timer > 0 ){
		if( vs_w->pose_wait_timer ){
			if( -- vs_w->pose_wait_timer > 0 ) return ;
		}
		vs_w->slow_bit = 0 ;
		for( i = 0 ; i < 6 ; i ++ ){
			if( pl_end_wait_ck( &pl_work[i] ) ) return ;
		}
	}
	vs_w->slow_bit = 0 ;
	win_mes_set() ;
	cpu_level_perfect() ;
	if( vs_w->winner_id > 0 || vs_w->time_over ) vs_w->ctrl_flag = CTRL_NO_INPUT | CTRL_NO_THROW ;
	vs_w->vs_st = VS_WIN ;
	vs_w->vs_timer = 130 ;
}

/*==============================================================*/
/*			The winner's message								*/
/*==============================================================*/
static void win_mes_set( void )
{
	if( vs_sys_w->winner_id < 0 ){
		vs_res_set( 0 ) ;
		return ;
	}
	if( Sys->play_side == 3 ){
		vs_res_set( vs_sys_w->winner_id + 6 ) ;
		return ;
	} else if( Sys->play_side & 1 << vs_sys_w->winner_id ) vs_res_set( 4 ) ;
	else vs_res_set( 5 ) ;
}

/*==============================================================*/
/*			After the pose: the next fight, or the special		*/
/*			mode's next stage									*/
/*==============================================================*/
static void vs_move_win( VS_SYS_WORK *vs_w )
{
	PLAYER_WORK *pl_w ;
	Sint16 vital ;

	if( -- vs_w->vs_timer > 0 ) return ;
	combo_reset( vs_w->team_w[0][0] ) ;
	combo_reset( vs_w->team_w[0][1] ) ;
	combo_reset( vs_w->team_w[0][2] ) ;
	combo_reset( vs_w->team_w[1][0] ) ;
	combo_reset( vs_w->team_w[1][1] ) ;
	combo_reset( vs_w->team_w[1][2] ) ;
	vs_w->round_cnt ++ ;
	if( vs_w->winner_id >= 0 ){
		pl_w = !vs_w->winner_id ? vs_w->team_w[1][0] : vs_w->team_w[0][0] ;
		if( vs_w->game_mode == 8 && Sys->boss_flag && FLAG_CK( pl_w->pl_bit, PLB0( PL_ABYSS1 ) | PLB0( PL_ABYSS2 ), 0 ) ){
			vs_w->ctrl_flag = CTRL_NO_INPUT ;
			vs_w->vs_timer = 360 ;
			vs_w->vs_st = VS_BOSS ;
			return ;
		}
	}
	vs_w->vs_timer = 120 ;
	vs_w->vs_st = VS_BONUS ;
	s3l_fade_out_set( 60 ) ;
	if( vs_w->winner_id >= 0 ){
		pl_w = vs_w->team_w[0][0] ;
		if( vs_w->winner_id ) pl_w = vs_w->team_w[1][0] ;
		if( !pl_w->cpu_flag ){
			vital = vs_w->team_w[pl_w->pl_no][0]->vital_new ;
			vital += vs_w->team_w[pl_w->pl_no][1]->vital_new ;
			vital += vs_w->team_w[pl_w->pl_no][2]->vital_new ;
			if( vital == FULL_VITAL ) vs_res_set( 3 ) ;
			else vs_res_set( 0x83 ) ;
			return ;
		}
	}
	vs_w->vs_st = VS_EXIT ;
	if( vs_w->game_mode == 8 && Sys->boss_flag && vs_w->round_cnt == 3 ) vs_w->vs_timer += 90 ;
}

/*==============================================================*/
/*			The special mode: the boss changes					*/
/*==============================================================*/
static void vs_move_boss( VS_SYS_WORK *vs_w )
{
	PLAYER_WORK *pl_w ;
	int i ;

	if( -- vs_w->vs_timer > 0 ){
		for( i = 0 ; i < 6 ; i ++ ){
			if( pl_end_wait_ck( &pl_work[i] ) ) return ;
		}
	}
	round_time_init() ;
	pl_w = !vs_w->winner_id ? vs_w->team_w[1][0] : vs_w->team_w[0][0] ;
	if( vs_w->game_mode == 8 && Sys->boss_flag && FLAG_CK( pl_w->pl_bit, PLB0( PL_ABYSS1 ) | PLB0( PL_ABYSS2 ), 0 ) ){
		if( pl_w->pl_type == PL_ABYSS1 ) pl_boss_change( 0 ) ;
		else pl_boss_change( 1 ) ;
		s3l_fade_out_set( 60 ) ;
	}
	vs_w->vs_st = VS_INIT ;
}

/*==============================================================*/
/*			The fade out										*/
/*==============================================================*/
static void vs_move_bonus( VS_SYS_WORK *vs_w )
{
	if( -- vs_w->vs_timer > 0 ) return ;
	vs_w->speed_rate = vs_w->speed_cnt = vs_w->speed_flag = 0 ;
	vs_w->vs_timer = 0 ;
	vs_w->vs_st = VS_EXIT ;
	if( Sys->ending_flag ){
		vs_w->vs_timer = 390 ;
		work_list_kill( W_SYS ) ;
		work_list_kill( W_FRONT ) ;
	}
}

/*==============================================================*/
/*			The match is over									*/
/*==============================================================*/
static void vs_move_exit( VS_SYS_WORK *vs_w )
{
	if( -- vs_w->vs_timer > 0 ) return ;
	Sys->routine_1 = 3 ;
}

/*==============================================================*/
static void vs_rule_set( VS_SYS_WORK *vs_w )
{
	vs_w->speed_rate = 6 ;
	vs_w->speed_cnt = 1 ;
	vs_w->speed_flag = 0 ;
	switch( Sys->game_speed ){
	case 0 :
		vs_w->speed_rate = 6 ;
		vs_w->speed_cnt = 1 ;
		break ;
	case 1 :
		vs_w->speed_rate = 4 ;
		vs_w->speed_cnt = 1 ;
		break ;
	case 2 :
		vs_w->speed_rate = 2 ;
		vs_w->speed_cnt = 1 ;
		break ;
	}
}

/*==============================================================*/
/*			1 while a player is still busy with an act the		*/
/*			win pose must wait for								*/
/*==============================================================*/
static Uint8 pl_end_wait_ck( PLAYER_WORK *pl_w )
{
	int act ;

	if( !pl_w->be_flag ) return 0 ;
	if( pl_w->pl_type == PL_ABYSS1 && !pl_w->vital_new ){
		if( pl_w->act_st == N_DOWN ) goto end ;
		return 1 ;
	} else {
		if( pl_w->routine_1 ) return 1 ;
		if( ( act = pl_w->act_st ) != N_DOWN && act && act != N_WIN ) return 1 ;
	}
end:
	return 0 ;
}

/****************************************************************/
void side_flag_clr( void )
{
	vs_sys_work.land_flag = 0 ;
	vs_sys_work.wall_flag = 0 ;
}

/****************************************************************/
int side_flag_get( int side )
{
	static int ( *side_flag_jmp[2] )( void ) = {
		land_flag_get, wall_flag_get,
	} ;

	return side_flag_jmp[side]() ;
}

/*==============================================================*/
static int land_flag_get( void )
{
	return vs_sys_work.land_flag ;
}

/*==============================================================*/
static int wall_flag_get( void )
{
	return vs_sys_work.wall_flag ;
}

/*==============================================================*/
/*			Count the winner's wins in a row					*/
/*==============================================================*/
static void win_cnt_set( VS_SYS_WORK *vs_w )
{
	int side ;

	if( vs_w->vs_end_flag ){
		if( Sys->vs_flag ){
			if( vs_w->winner_id < 0 ){
				vs_w->win_cnt[0] = vs_w->win_cnt[1] = 0 ;
			} else {
				side = 0 ;
				if( vs_w->winner_id ) side = 1 ;
				if( pl_work[vs_w->winner_id].cpu_flag ) return ;
				if( ++ vs_w->win_cnt[side] > 99 ) vs_w->win_cnt[side] = 99 ;
				vs_w->win_cnt[side ^ 1] = 0 ;
			}
		} else if( vs_w->winner_id < 0 ){
			/* a draw changes nothing */
		} else if( !vs_w->ko_cnt[vs_w->winner_id] ){
			vs_w->no_ko_cnt[vs_w->winner_id] ++ ;
		} else {
			vs_w->no_ko_cnt[vs_w->winner_id] = 0 ;
		}
	}
}

/*==============================================================*/
static void round_time_init( void )
{
	vs_sys_work.round_time = vs_sys_work.cpu_time = 99 ;
	time_frame_set( &vs_sys_work ) ;
}

/*==============================================================*/
/*			The clock: 0 when it has run out					*/
/*==============================================================*/
static int round_time_ck( VS_SYS_WORK *vs_w )
{
	if( vs_w->game_stop == 0 ){
		if( !-- vs_w->time_frame ){
			if( !vs_w->round_time ) return 0 ;
			vs_w->team_w[0][0]->fight_sec ++ ;
			vs_w->team_w[1][0]->fight_sec ++ ;
			if( vs_w->round_time > 0 ){
				vs_w->round_time -- ;
				if( vs_w->round_time == 10 || vs_w->round_time == 20 ) na_req( 24 ) ;
			}
			if( vs_w->cpu_time > 0 ) vs_w->cpu_time -- ;
			time_frame_set( vs_w ) ;
		}
	}
	return 1 ;
}

/*==============================================================*/
/*			The frames in a second of the clock, by the			*/
/*			speed setting										*/
/*==============================================================*/
static void time_frame_set( VS_SYS_WORK *vs_w )
{
	static Uint8 time_frame_tbl[4] = { 180, 120, 90, 70 } ;

	vs_w->time_frame = vs_w->time_frame_max = time_frame_tbl[Sys->time_speed] ;
}

/****************************************************************/
/*			Run in each frame of a win pose: 1 when the pose	*/
/*			must end											*/
/****************************************************************/
int pl_win_end_ck( PLAYER_WORK *pl_w )
{
	VS_SYS_WORK *vs_w = &vs_sys_work ;

	if( vs_w->vs_st < VS_WIN ) goto end ;
	if( Sys->ending_flag ) goto keep ;
	if( vs_w->winner_id < 0 ) return 0 ;
	if( vs_w->winner_id != pl_w->pl_no ) return 0 ;
	if( vs_w->time_over ) return 0 ;
	if( vs_w->be_cnt[pl_w->pl_no] != 1 ) return 0 ;
	if( ABYSS_CK( pl_w->target_w ) ) return 0 ;
	if( pl_w->win_wait ){
		pl_w->win_wait -- ;
		goto keep ;
	}
	if( !( pl_w->pl_new_lvbt & KEY_START ) ) return 0 ;
	vs_w->ctrl_flag = CTRL_PLAY ;
	vs_w->win_skip_flag = 1 ;
	pl_w->win_flag |= 0x80 ;
end:
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 1 ) ;
	SCR_EXCEP_SET ;
	return 1 ;
keep:
	pl_w->catch_ng_timer = 3 ;
	pl_w->dm_flag = 0 ;
	return 0 ;
}

/*==============================================================*/
/*			Players left in act 22 stand						*/
/*==============================================================*/
static void pl_stand_all( void )
{
	int i ;
	PLAYER_WORK *pl_w ;

	for( i = 0 ; i < 6 ; i ++ ){
		pl_w = &pl_work[i] ;
		if( pl_w->act_st == N_WIN ) pl_stand_ret( pl_w ) ;
	}
}

/****************************************************************/
/*			The arcade's display: each side's wins in a row,	*/
/*			or the player's score								*/
/****************************************************************/
void naomi_level_disp( void )
{
	int side ;
	Uint8 x ;
	Uint8 wx ;
	char buf[16] ;

	if( vs_sys_w->vs_st == VS_EXIT ) return ;
	if( flash_flag_get() ) return ;
	if( !Sys->vs_flag ){
		if( Sys->play_side & 1 ) side = 0 ;
		else side = 1 ;
		x = side * 27 ;
		if( vs_sys_w->win_cnt[side] ){
			wx = ( x + 3 ) * 3 ;
			sprintf( buf, "WINS %2d", vs_sys_w->win_cnt[side] ) ;
			STrPrint( wx, Sys->disp_line, 19, buf ) ;
		}
		x = ( x + 15 ) * 3 ;
		if( pl_work[side].score[1] ){
			sprintf( buf, "%08x", pl_work[side].score[0] ) ;
			STrPrint( x, Sys->disp_line, 21, buf ) ;
			sprintf( buf, "%3x", pl_work[side].score[1] ) ;
			STrPrint( x - 9, Sys->disp_line, 21, buf ) ;
		} else {
			sprintf( buf, "%8x", pl_work[side].score[0] ) ;
			STrPrint( x, Sys->disp_line, 21, buf ) ;
		}
	} else {
		if( !vs_sys_w->win_cnt[0] ){
			if( !vs_sys_w->vs_end_flag && vs_sys_w->vs_st < VS_END_WAIT ){
				sprintf( buf, "NEW HERO" ) ;
				STrPrint( 42, Sys->disp_line, 19, buf ) ;
			}
		} else {
			sprintf( buf, "WINS %2d", vs_sys_w->win_cnt[0] ) ;
			STrPrint( 45, Sys->disp_line, 19, buf ) ;
		}
		if( !vs_sys_w->win_cnt[1] ){
			if( !vs_sys_w->vs_end_flag && vs_sys_w->vs_st < VS_END_WAIT ){
				sprintf( buf, "NEW HERO" ) ;
				STrPrint( 93, Sys->disp_line, 19, buf ) ;
			}
		} else {
			sprintf( buf, "WINS %2d", vs_sys_w->win_cnt[1] ) ;
			STrPrint( 96, Sys->disp_line, 19, buf ) ;
		}
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "vs_sys.c" End of File	*/

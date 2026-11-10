/* The player's state machine, shared by every character: the entrance, the frame's timers and the
   acts of the normal state (standing, walking, jumping, guarding), throws and being thrown. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl_act.h"

extern Uint8 pl_nml_atck_ck( PLAYER_WORK *pl_w );	/* s_pl14 declares it as an int */

void player_move( PLAYER_WORK *pl_w );
static void pl_move_appear( PLAYER_WORK *pl_w );
static void pl_move_fight( PLAYER_WORK *pl_w );
void pl_move_2( PLAYER_WORK *pl_w );
void pl_move_3( PLAYER_WORK *pl_w );
static void appear_move( PLAYER_WORK *pl_w );
static void appear_wait( PLAYER_WORK *pl_w );
static void pl_normal( PLAYER_WORK *pl_w );
void pl_catch( PLAYER_WORK *pl_w );
void pl_caught( PLAYER_WORK *pl_w );
void pl_dm_move( PLAYER_WORK *pl_w );
static void act_stand( PLAYER_WORK *pl_w );
static Uint8 act_stand_ck( PLAYER_WORK *pl_w );
static void act_stand_end( PLAYER_WORK *pl_w );
static void act_33( PLAYER_WORK *pl_w );
static void stand_bob( PLAYER_WORK *pl_w );
static void act_walk( PLAYER_WORK *pl_w );
static void act_jump( PLAYER_WORK *pl_w );
static void act_jump_air( PLAYER_WORK *pl_w );
static void act_land( PLAYER_WORK *pl_w );
static void act_crouch( PLAYER_WORK *pl_w );
static void act_crouching( PLAYER_WORK *pl_w );
static void act_stand_up( PLAYER_WORK *pl_w );
static void act_turn( PLAYER_WORK *pl_w );
static void act_crouch_turn( PLAYER_WORK *pl_w );
static void act_dash( PLAYER_WORK *pl_w );
static void act_back_dash( PLAYER_WORK *pl_w );
static void act_dizzy( PLAYER_WORK *pl_w );
static void act_sjump( PLAYER_WORK *pl_w );
static void act_sjump_air( PLAYER_WORK *pl_w );
static void act_air_recover( PLAYER_WORK *pl_w );
static void act_fall( PLAYER_WORK *pl_w );
static void act_get_up( PLAYER_WORK *pl_w );
static void act_get_up_init( PLAYER_WORK *pl_w );
static void act_get_up_rise( PLAYER_WORK *pl_w );
static void act_get_up_roll( PLAYER_WORK *pl_w );
static void act_guard( PLAYER_WORK *pl_w );
static void act_guard_start( PLAYER_WORK *pl_w );
static void act_guard_hold( PLAYER_WORK *pl_w );
static void act_guard_end( PLAYER_WORK *pl_w );
static void act_guard_back( PLAYER_WORK *pl_w );
static void act_guard_back_init( PLAYER_WORK *pl_w );
static void act_air_guard( PLAYER_WORK *pl_w );
static void act_air_guard_start( PLAYER_WORK *pl_w );
static void act_air_guard_hold( PLAYER_WORK *pl_w );
static void act_air_guard_back( PLAYER_WORK *pl_w );
static void act_nml_atck( PLAYER_WORK *pl_w );
static void act_special( PLAYER_WORK *pl_w );
static void act_win( PLAYER_WORK *pl_w );
static void act_down( PLAYER_WORK *pl_w );
static void act_fly( PLAYER_WORK *pl_w );
static void act_wall_jump( PLAYER_WORK *pl_w );
static void act_air_dash( PLAYER_WORK *pl_w );
static void act_escape( PLAYER_WORK *pl_w );
static void act_change( PLAYER_WORK *pl_w );
static void act_change_tag( PLAYER_WORK *pl_w );
static void act_change_tag_init( PLAYER_WORK *pl_w );
static void act_change_tag_wait( PLAYER_WORK *pl_w );
static void act_change_tag_end( PLAYER_WORK *pl_w );
static void act_change_tag_air( PLAYER_WORK *pl_w );
static void act_change_out( PLAYER_WORK *pl_w );
static void act_change_out_init( PLAYER_WORK *pl_w );
static void act_change_out_recover( PLAYER_WORK *pl_w );
static void act_change_out_fall( PLAYER_WORK *pl_w );
static void act_change_out_bound( PLAYER_WORK *pl_w );
static void act_change_out_hop( PLAYER_WORK *pl_w );
static void act_change_gattai( PLAYER_WORK *pl_w );
static void act_change_gattai_init( PLAYER_WORK *pl_w );
static void act_change_gattai_move( PLAYER_WORK *pl_w );
static void act_change_vcounter( PLAYER_WORK *pl_w );
static void act_change_vcounter_init( PLAYER_WORK *pl_w );
static void act_change_vcounter_end( PLAYER_WORK *pl_w );
static void act_change_zero( PLAYER_WORK *pl_w );
static void act_change_zero_init( PLAYER_WORK *pl_w );
static void act_change_zero_move( PLAYER_WORK *pl_w );
static void act_change_assist( PLAYER_WORK *pl_w );
static void act_change_assist_init( PLAYER_WORK *pl_w );
static void act_change_assist_move( PLAYER_WORK *pl_w );
static void act_change_hyper( PLAYER_WORK *pl_w );
static void act_change_hyper_init( PLAYER_WORK *pl_w );
static void act_change_hyper_move( PLAYER_WORK *pl_w );
static void act_change_t_kick( PLAYER_WORK *pl_w );
static void act_change_call( PLAYER_WORK *pl_w );
static void act_change_assist_end( PLAYER_WORK *pl_w );
static void act_cmd_atck( PLAYER_WORK *pl_w );
static void caught_move( PLAYER_WORK *pl_w );
void caught_blow( PLAYER_WORK *pl_w );
static void caught_blow_init( PLAYER_WORK *pl_w );
static void caught_blow_fly( PLAYER_WORK *pl_w );
static void caught_blow_down( PLAYER_WORK *pl_w );
static void caught_end( PLAYER_WORK *pl_w );
void caught_slam( PLAYER_WORK *pl_w );
static void caught_slam_init( PLAYER_WORK *pl_w );
static void caught_slam_bound( PLAYER_WORK *pl_w );
static void caught_slam_fly( PLAYER_WORK *pl_w );
static void caught_slam_land( PLAYER_WORK *pl_w );
static void caught_slam_down( PLAYER_WORK *pl_w );
void caught_down( PLAYER_WORK *pl_w );
void caught_dm( PLAYER_WORK *pl_w );
void caught_hold( PLAYER_WORK *pl_w );
void caught_blow_fix( PLAYER_WORK *pl_w );
static void caught_blow_fix_init( PLAYER_WORK *pl_w );
static void caught_blow_fix_fly( PLAYER_WORK *pl_w );
static void caught_blow_fix_down( PLAYER_WORK *pl_w );
void caught_down_combo( PLAYER_WORK *pl_w );
void caught_elem( PLAYER_WORK *pl_w );
static void caught_elem_init( PLAYER_WORK *pl_w );
static void caught_elem_fly( PLAYER_WORK *pl_w );
static void caught_elem_down( PLAYER_WORK *pl_w );
void caught_slam_hard( PLAYER_WORK *pl_w );
static void caught_slam_hard_init( PLAYER_WORK *pl_w );
static void caught_slam_hard_bound( PLAYER_WORK *pl_w );
static void caught_slam_hard_fly( PLAYER_WORK *pl_w );
static void caught_slam_hard_down( PLAYER_WORK *pl_w );
void caught_wait( PLAYER_WORK *pl_w );
void caught_toss( PLAYER_WORK *pl_w );
static void caught_toss_init( PLAYER_WORK *pl_w );
static void caught_toss_fly( PLAYER_WORK *pl_w );
void caught_burn( PLAYER_WORK *pl_w );
static void caught_burn_init( PLAYER_WORK *pl_w );
static void caught_burn_fly( PLAYER_WORK *pl_w );
static void caught_burn_down( PLAYER_WORK *pl_w );
void caught_spin( PLAYER_WORK *pl_w );
static void caught_spin_init( PLAYER_WORK *pl_w );
static void caught_spin_fly( PLAYER_WORK *pl_w );
static void caught_spin_down( PLAYER_WORK *pl_w );
void caught_shock( PLAYER_WORK *pl_w );
static void caught_shock_init( PLAYER_WORK *pl_w );
static void caught_shock_wait( PLAYER_WORK *pl_w );
static void caught_shock_fly( PLAYER_WORK *pl_w );
static void caught_shock_down( PLAYER_WORK *pl_w );
void caught_drop( PLAYER_WORK *pl_w );
static void caught_drop_init( PLAYER_WORK *pl_w );
static void caught_drop_wait( PLAYER_WORK *pl_w );
static void caught_drop_fall( PLAYER_WORK *pl_w );
void caught_pos_set( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The player's move for the frame, by routine_0:		*/
/*			the entrance, the fight, and two that do nothing	*/
/****************************************************************/
void player_move( PLAYER_WORK *pl_w )
{
	static void ( *player_jmp[4] )( PLAYER_WORK *pl_w ) = {
		pl_move_appear, pl_move_fight, pl_move_2, pl_move_3,
	} ;

	if( !pl_w->be_flag ) return ;
	if( pl_w->target_w->hyper_stop_flag ) return ;
	if( vs_sys_work.game_stop ){
		if( vs_sys_work.game_stop & 1 << ( pl_w->pl_no ^ 1 ) ) return ;
		if( !pl_w->stop_move_flag ){
			pl_frame_jump( pl_w ) ;
			return ;
		}
	}
	player_jmp[pl_w->routine_0]( pl_w ) ;
}

/*==============================================================*/
/*			The entrance										*/
/*==============================================================*/
static void pl_move_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[2] )( PLAYER_WORK *pl_w ) = {
		appear_move, appear_wait,
	} ;

	appear_jmp[pl_w->routine_1]( pl_w ) ;
}

/*==============================================================*/
static void appear_move( PLAYER_WORK *pl_w )
{
	pl_w->dm_flag = 0 ;
	pl_w->pl_tbl[TBL_APPEAR]( pl_w ) ;
	if( pl_w->routine_1 ){
		pl_w->routine_1 = 1 ;
		pl_char_set( pl_w, Char_Stand, 0 ) ;
	}
}

/*==============================================================*/
/*			Stand until the fight starts						*/
/*==============================================================*/
static void appear_wait( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( vs_sys_work.vs_st <= VS_APPEAR ) return ;
	pl_w->routine_0 = PL_FIGHT ;
	pl_w->routine_1 = pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->dm_flag = -128 ;
	if( !FLAG_CK( pl_w->pl_bit, PLB0( PL_ABYSS2 ) | PLB0( PL_ABYSS3 ), 0 ) ){
		pl_w->pl_dir = !pl_w->pl_no ? 1 : 0 ;
		pl_w->disp_dir = pl_w->pl_dir ;
	}
	pl_w->act_st = N_STAND ;
	pl_act_set( pl_w, N_STAND ) ;
}

/*==============================================================*/
/*			The fight: the input, the timers, then the move		*/
/*			by routine_1. The hit stop jumps to the wall		*/
/*			checks												*/
/*==============================================================*/
static void pl_move_fight( PLAYER_WORK *pl_w )
{
	static void ( *pl_jmp[4] )( PLAYER_WORK *pl_w ) = {
		pl_normal, pl_catch, pl_caught, pl_dm_move,
	} ;

	if( pl_w->pl_type == PL_ABYSS3 ){
		pl_w->pl_tbl[TBL_EX0]( pl_w ) ;
		return ;
	}
	pl_em_dist_set( pl_w ) ;
	em_near_set( pl_w ) ;
	if( pl_w->cpu_flag ) em_play( pl_w ) ;
	if( !pl_w->team_no ) pl_sw_set( pl_w ) ;
	if( vs_sys_work.ctrl_flag & CTRL_NO_INPUT || vs_sys_work.team_hyper_flag & 1 << pl_w->pl_no ){
		pl_w->pl_new_lvbt &= KEY_START ;
		pl_w->pl_sw_now = 0 ;
		pl_w->pl_sw_new = 0 ;
		pl_w->pl_now_lvbt = 0 ;
	} else if( vs_sys_work.ctrl_flag & CTRL_WALK ){
		pl_w->pl_new_lvbt &= ( KEY_FORWARD | KEY_BACK ) ;
		pl_w->pl_now_lvbt &= ( KEY_FORWARD | KEY_BACK ) ;
		pl_w->pl_last_lvbt &= ( KEY_FORWARD | KEY_BACK ) ;
		pl_w->pl_sw_new &= ( KEY_FORWARD | KEY_BACK ) ;
		pl_w->pl_sw_now &= ( KEY_FORWARD | KEY_BACK ) ;
		pl_w->pl_sw_old &= ( KEY_FORWARD | KEY_BACK ) ;
	}
	if( 1 ) pl_w->change_req = 0 ;
	else if( pl_w->cpu_flag ) em_play( pl_w ) ;
	pl_wait_com_ck( pl_w ) ;
	if( pl_w->delay_flag ) pl_delay_hyper_ck( pl_w ) ;
	if( pl_w->unk_23f ) pl_w->unk_23f -- ;
	if( pl_w->target_w->fly_screen_flag ) pl_stage_end_ck( pl_w ) ;
	if( pl_w->change_req ) return ;
	pl_w->pl_tbl[TBL_MAIN]( pl_w ) ;
	pl_w->jump_move_flag = 0 ;
	pl_renda_ck( pl_w ) ;
	if( pl_w->down_timer ) pl_w->down_timer -- ;
	if( pl_w->hit_stop ){
		pl_w->hit_stop -- ;
		pl_w->pl_stop_new |= pl_w->pl_new_lvbt ;
		pl_w->pl_stop_now |= pl_w->pl_now_lvbt ;
		dm_ko_shake( pl_w ) ;
		if( pl_w->routine_1 != PL_DAMAGE ) pl_frame_jump( pl_w ) ;
		if( pl_w->routine_1 == PL_CATCH ) pl_w->catch_flag = 1 ;
		goto wall ;
	}
	if( pl_w->catch_ng_timer ) pl_w->catch_ng_timer -- ;
	if( pl_w->nml_catch_ng_timer ) pl_w->nml_catch_ng_timer -- ;
	if( pl_w->catch_atck_ng_timer ) pl_w->catch_atck_ng_timer -- ;
	if( pl_w->hit_ng_timer ) pl_w->hit_ng_timer -- ;
	if( pl_w->stun_ng_timer ) pl_w->stun_ng_timer -- ;
	if( pl_w->auto_guard_timer ) pl_w->auto_guard_timer -- ;
	if( pl_w->vcounter_msg_timer ) pl_w->vcounter_msg_timer -- ;
	if( pl_w->guard_ng_timer ) pl_w->guard_ng_timer -- ;
	if( pl_w->team_ng_timer ) pl_w->team_ng_timer -- ;
	if( pl_w->sp_ng_timer ) pl_w->sp_ng_timer -- ;
	if( pl_w->atck_ng_timer ) pl_w->atck_ng_timer -- ;
	if( pl_w->hyper_bg_tm && --pl_w->hyper_bg_tm <= 0 ){
		pl_w->hyper_bg_tm = 0 ;
		pl_w->hyper_bg = 0 ;
	}
	if( pl_w->speed_up_timer ) pl_w->speed_up_timer -- ;
	if( pl_w->speed_plus_timer ) pl_w->speed_plus_timer -- ;
	if( pl_w->speed_up_timer > 0 || pl_w->speed_plus_timer > 0 ) pl_w->speed_up_flag = 1 ;
	else pl_w->speed_up_flag = 0 ;
	if( pl_w->atck_plus_timer && !--pl_w->atck_plus_timer ) pl_w->atck_plus = 0 ;
	if( pl_w->def_plus_timer && !--pl_w->def_plus_timer ) pl_w->def_plus = 0 ;
	if( pl_w->stop_move_timer && --pl_w->stop_move_timer <= 0 ) pl_w->stop_move_flag = 0 ;
	if( pl_w->delay_timer && --pl_w->delay_timer <= 0 ) pl_w->delay_flag = 0 ;
	if( pl_w->cancel_timer > 0 ) pl_w->cancel_timer -- ;
	pl_jmp[pl_w->routine_1]( pl_w ) ;
	if( pl_w->wall_ng_timer ) pl_w->wall_ng_timer -- ;
	if( pl_w->wall_wide_timer ) pl_w->wall_wide_timer -- ;
	if( pl_w->target_w->fly_screen_flag ) goto armor ;
wall :
	pl_wall_ck( pl_w ) ;
	pl_wall_wide_ck( pl_w ) ;
armor :
	if( !pl_w->armor_flag && pl_w->armor_timer && !pl_w->hit_stop ){
		if( --pl_w->armor_timer <= 0 ){
			pl_w->armor_timer_flag = 0 ;
			armor_init( pl_w ) ;
			pl_color_set( pl_w, 1 ) ;
		} else pl_w->armor_timer_flag = 1 ;
	}
	if( pl_w->armor_flag || pl_w->armor_timer_flag ) armor_move( pl_w ) ;
	armor_back_move( pl_w ) ;
	pl_w->atck_flag = -128 ;
	if( pl_w->hit_ng_timer ) pl_w->atck_flag = 0 ;
	pl_w->dm_flag = -128 ;
	if( pl_w->catch_ng_timer || pl_w->routine_1 == PL_CATCH || pl_w->routine_1 == PL_CAUGHT && !pl_w->catch_type ) pl_w->dm_flag = 0 ;
}

/*==============================================================*/
/*			The normal state: the act act_st_set() chose		*/
/*==============================================================*/
static void pl_normal( PLAYER_WORK *pl_w )
{
	static void ( *act_jmp[35] )( PLAYER_WORK *pl_w ) = {
		act_stand, act_walk, act_jump, act_jump_air, act_land, act_crouch, act_crouching, act_stand_up,
		act_turn, act_crouch_turn, act_dash, act_back_dash, act_dizzy, act_sjump, act_sjump_air, act_air_recover,
		act_fall, act_get_up, act_guard, act_air_guard, act_nml_atck, act_special, act_win, act_down,
		act_fly, act_wall_jump, act_air_dash, act_escape, act_change, act_special, act_stand, act_stand,
		act_stand, act_33, act_cmd_atck,
	} ;
	/* the acts that keep catch_cnt */
	static Uint8 act_23a_tbl[35] = {
		0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
		0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0,
		0, 0, 0,
	} ;

	if( pl_w->stun_reset_timer && !--pl_w->stun_reset_timer ) pl_w->stun_cnt = 80 ;
	pl_w->unk_228 = 0 ;
	if( !act_23a_tbl[pl_w->act_st] ) pl_w->catch_cnt = 0 ;
	act_jmp[pl_w->act_st]( pl_w ) ;
}

/* the standing bob, a step every other frame */
static Sint8 bob_tbl[40] = {
	1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, -1, 0, -1, 0,
	-1, -1, -1, -1, -1, -1, -1, -1, 0, -1, 0, -1, 0, 0, 0, 0,
	1, 0, 1, 0, 1, 1, 1, 1,
} ;

/*==============================================================*/
/*			Standing											*/
/*==============================================================*/
static void act_stand( PLAYER_WORK *pl_w )
{
	Float ofs ;
	Sint8 flag ;

	if( vs_sys_work.vs_st >= VS_WIN && pl_w->win_flag >= 0 ){
		/* the arcade's own '&' for '|': this always stores 0 */
		flag = pl_w->win_flag & 0x80 ;
		pl_w->win_flag = ( pl_w->win_flag + 1 ) & 0x0f & flag ;
		pl_act_set( pl_w, N_WIN ) ;
		return ;
	}
	ofs = DOT_Y( 16.0f ) ;
	if( act_stand_ck( pl_w ) ){
		if( pl_w->float_flag && pl_w->pos.y < pl_w->ground_y + ofs ) pl_w->pos.y = pl_w->float_y ;
		return ;
	}
	if( pl_w->char_no.w == 0x1803 ){
		if( pl_char_move( pl_w ) < 0 ) pl_char_set( pl_w, Char_Stand, 0 ) ;
	} else {
		if( pl_char_move( pl_w ) < 0 ) act_stand_end( pl_w ) ;
		if( pl_w->speed_up_flag ){
			if( pl_char_move( pl_w ) < 0 ) act_stand_end( pl_w ) ;
		}
	}
	if( pl_w->delay_change_flag ){
		if( pl_w->float_flag && pl_w->pos.y < pl_w->ground_y + ofs ) pl_w->pos.y = pl_w->float_y ;
		pl_char_set( pl_w, Char_Change, 3 ) ;
	}
	if( pl_w->char_no.w != 0x1803 ){
		if( pl_w->float_flag ){
			stand_bob( pl_w ) ;
			if( pl_w->speed_up_flag ) stand_bob( pl_w ) ;
			if( pl_w->pos.y < pl_w->ground_y + ofs ){
				pl_w->spd.z = pl_w->pos.y ;
				pl_w->pos.y = pl_w->float_y ;
				pl_fall_ck( pl_w ) ;
				pl_w->pos.y = pl_w->spd.z ;
				return ;
			}
		}
	}
	if( pl_w->pl_type == PL_CAPTAIN_AMERICA ) pl_w->pl_tbl[TBL_EX0]( pl_w ) ;
	pl_fall_ck( pl_w ) ;
}

/*==============================================================*/
/*			What a standing player can start					*/
/*==============================================================*/
static Uint8 act_stand_ck( PLAYER_WORK *pl_w )
{
	if( !pl_w->fly_flag && pl_sjump_set( pl_w ) ) return 1 ;
	if( pl_dash_set( pl_w ) ) return 1 ;
	if( pl_cmd_atck_ck( pl_w ) ) return 1 ;
	if( pl_nml_atck_ck( pl_w ) ) return 1 ;
	if( pl_guard_req_ck( pl_w ) ) return 1 ;
	if( pl_turn_ck( pl_w ) ) return 1 ;
	if( pl_jump_ck( pl_w ) ) return 1 ;
	if( pl_crouch_ck( pl_w ) ) return 1 ;
	if( pl_stand_ck( pl_w ) ) return 1 ;
	if( pl_walk_ck( pl_w, 0 ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
/*			The standing animation has run out					*/
/*==============================================================*/
static void act_stand_end( PLAYER_WORK *pl_w )
{
	switch( pl_w->pl_type ){
	case PL_ROGUE :
		pl_w->pl_tbl[TBL_EX0]( pl_w ) ;
		break ;
	case PL_CAPTAIN_COMMANDO :
		pl_w->pl_tbl[TBL_EX0]( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
static void act_33( PLAYER_WORK *pl_w )
{
	if( pl_cmd_atck_ck( pl_w ) ) return ;
	if( pl_nml_atck_ck( pl_w ) ) return ;
	if( pl_guard_req_ck( pl_w ) ) return ;
	if( pl_jump_ck( pl_w ) ) return ;
	if( pl_w->catch_w->act_st != N_STAND ){
		pl_w->jump_dir = -1 ;
		pl_act_set( pl_w, N_JUMP ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
	pl_fall_ck( pl_w ) ;
}

/*==============================================================*/
static void stand_bob( PLAYER_WORK *pl_w )
{
	Float y ;

	if( --pl_w->float_wait < 0 ){
		pl_w->float_wait = 1 ;
		y = DOT_Y( bob_tbl[pl_w->float_st] ) ;
		pl_w->pos.y -= y ;
		pl_w->float_st = ( pl_w->float_st + 1 ) % 40 ;
	}
}

/*==============================================================*/
/*			Walking												*/
/*==============================================================*/
static void act_walk( PLAYER_WORK *pl_w )
{
	Sint8 dir ;

	dir = ( pl_w->pl_sw_new & BACK ) >> 10 ;
	if( dir != pl_w->jump_dir ) pl_w->routine_2 = 0 ;
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->jump_dir = ( pl_w->pl_sw_new & BACK ) >> 10 ;
		if( !( vs_sys_work.ctrl_flag & CTRL_WALK ) ){
			pl_w->routine_3 = 0 ;
			pl_w->char_no.b.no = 2 ;
			if( pl_w->armor_flag & ARMOR_HEAVY ) pl_w->char_no.b.no += 6 ;
		} else {
			pl_w->routine_3 = 1 ;
			pl_w->char_no.b.no = 6 ;
		}
		pl_w->char_no.b.no += pl_w->jump_dir ;
		pl_char_set( pl_w, Char_Stand, pl_w->char_no.b.no ) ;
	}
	if( pl_dash_set( pl_w ) ) return ;
	if( pl_sjump_set( pl_w ) ) return ;
	if( pl_cmd_atck_ck( pl_w ) ) return ;
	if( pl_nml_atck_ck( pl_w ) ) return ;
	if( pl_guard_req_ck( pl_w ) ) return ;
	if( pl_turn_ck( pl_w ) ) return ;
	if( pl_jump_ck( pl_w ) ) return ;
	if( pl_crouch_ck( pl_w ) ) return ;
	if( pl_stand_ck( pl_w ) ) return ;
	if( pl_walk_ck( pl_w, 1 ) ) return ;
	if( !( vs_sys_work.ctrl_flag & CTRL_NO_INPUT ) && pl_w->pl_new_lvbt & KEY_LEVER ){
		if( pl_fall_ck( pl_w ) ) return ;
		if( !( vs_sys_work.ctrl_flag & CTRL_WALK ) && pl_w->routine_3 ) pl_w->routine_2 = 0 ;
		else if( vs_sys_work.ctrl_flag & CTRL_WALK ) pl_step_spd_set( pl_w ) ;
		else pl_walk_spd_set( pl_w ) ;
		if( pl_w->speed_up_flag ) pl_char_move( pl_w ) ;
		pl_char_move( pl_w ) ;
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
	} else {
		dir = pl_w->jump_dir ;
		pl_act_set( pl_w, N_STAND ) ;
		if( vs_sys_work.ctrl_flag & CTRL_WALK ) return ;
		switch( pl_w->pl_type ){
		case PL_CAPTAIN_COMMANDO :
			pl_w->pl_tbl[TBL_EX1]( pl_w ) ;
			break ;
		case PL_SERVBOT :
			pl_w->pl_tbl[TBL_EX0]( pl_w ) ;
			break ;
		default :
			dir += 4 ;
			if( pl_w->armor_flag & ARMOR_HEAVY ) dir = 1 ;
			pl_char_set( pl_w, Char_Stand, dir ) ;
			break ;
		}
	}
}

/*==============================================================*/
/*			The jump's take-off									*/
/*==============================================================*/
static void act_jump( PLAYER_WORK *pl_w )
{
	pl_w->pl_stance = STANCE_STAND ;
	if( pl_guard_req_ck( pl_w ) ) return ;
	if( !pl_w->cg_type & 0x0f ){
	} else {
		pl_char_move( pl_w ) ;
		if( pl_w->cg_type & 0x0f ) return ;
	}
	pl_w->air_jump_cnt = 0 ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->routine_2 ++ ;
	act_st_set( pl_w, N_JUMP_AIR ) ;
	act_jump_air( pl_w ) ;
}

/*==============================================================*/
/*			In the air											*/
/*==============================================================*/
static void act_jump_air( PLAYER_WORK *pl_w )
{
	if( pl_nml_atck_ck( pl_w ) ) return ;
	if( pl_guard_req_ck( pl_w ) ) return ;
	if( WALL_JUMP_CK( pl_w ) ){
		if( pl_wall_jump_ck( pl_w ) ) return ;
	}
	if( AIR_JUMP_CK( pl_w ) ) pl_air_jump_ck( pl_w ) ;
	if( pl_w->pl_type == PL_STORM ) pl_w->pl_tbl[TBL_EX0]( pl_w ) ;
	pl_char_move( pl_w ) ;
	pl_jump_move( pl_w ) ;
	if( pl_land_ck( pl_w ) && !pl_guard_req_ck( pl_w ) ){
		if( pl_w->cpu_flag ) pl_dir_set( pl_w ) ;
		pl_land_set( pl_w ) ;
	}
}

/*==============================================================*/
static void act_land( PLAYER_WORK *pl_w )
{
	if( pl_w->old_act_st != N_SPECIAL && pl_dash_set( pl_w ) ) return ;
	if( pl_sjump_set( pl_w ) ) return ;
	if( pl_cmd_atck_ck( pl_w ) ) return ;
	if( pl_nml_atck_ck( pl_w ) ) return ;
	if( pl_guard_req_ck( pl_w ) ) return ;
	if( pl_turn_ck( pl_w ) ) return ;
	if( pl_crouch_ck( pl_w ) ) return ;
	if( !pl_w->fly_flag ){
		if( pl_jump_ck( pl_w ) ) return ;
		if( pl_walk_ck( pl_w, 0 ) ) return ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_act_set( pl_w, N_STAND ) ;
}

/*==============================================================*/
/*			Crouching down										*/
/*==============================================================*/
static void act_crouch( PLAYER_WORK *pl_w )
{
	if( !pl_w->fly_flag ){
		if( pl_sjump_set( pl_w ) ) return ;
		if( pl_dash_set( pl_w ) ) return ;
		if( pl_cmd_atck_ck( pl_w ) ) return ;
		if( pl_nml_atck_ck( pl_w ) ) return ;
		if( pl_guard_req_ck( pl_w ) ) return ;
		if( pl_fall_ck( pl_w ) ) return ;
	}
	if( !( pl_w->pl_new_lvbt & KEY_DOWN ) ){
		pl_act_set( pl_w, N_STAND_UP ) ;
		pl_char_set( pl_w, Char_Crouch, 2 ) ;
		return ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_brake_stop( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ) pl_act_set( pl_w, N_CROUCHING ) ;
}

/*==============================================================*/
/*			Crouching											*/
/*==============================================================*/
static void act_crouching( PLAYER_WORK *pl_w )
{
	if( pl_cmd_atck_ck( pl_w ) ) return ;
	if( pl_nml_atck_ck( pl_w ) ) return ;
	if( pl_guard_req_ck( pl_w ) ) return ;
	if( !pl_w->fly_flag ){
		if( pl_sjump_set( pl_w ) ) return ;
		if( pl_dash_set( pl_w ) ) return ;
		if( pl_jump_ck( pl_w ) ) return ;
	}
	if( pl_fall_ck( pl_w ) ) return ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_brake_stop( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( !pl_turn_ck( pl_w ) ) pl_crouch_end_ck( pl_w ) ;
}

/*==============================================================*/
/*			Standing up											*/
/*==============================================================*/
static void act_stand_up( PLAYER_WORK *pl_w )
{
	if( pl_cmd_atck_ck( pl_w ) ) return ;
	if( pl_nml_atck_ck( pl_w ) ) return ;
	if( pl_guard_req_ck( pl_w ) ) return ;
	if( !pl_w->fly_flag ){
		if( pl_sjump_set( pl_w ) ) return ;
		if( pl_dash_set( pl_w ) ) return ;
		if( pl_walk_ck( pl_w, 0 ) ) return ;
		if( pl_jump_ck( pl_w ) ) return ;
	}
	if( pl_fall_ck( pl_w ) ) return ;
	if( pl_w->pl_new_lvbt & KEY_DOWN ){
		pl_act_set( pl_w, N_CROUCH ) ;
		pl_char_set( pl_w, Char_Crouch, 0 ) ;
		return ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_act_set( pl_w, N_STAND ) ;
}

/*==============================================================*/
static void act_turn( PLAYER_WORK *pl_w )
{
	if( pl_cmd_atck_ck( pl_w ) ) return ;
	if( pl_nml_atck_ck( pl_w ) ) return ;
	if( pl_guard_req_ck( pl_w ) ) return ;
	if( pl_walk_ck( pl_w, 0 ) ) return ;
	if( pl_jump_ck( pl_w ) ) return ;
	if( pl_fall_ck( pl_w ) ) return ;
	if( pl_char_move( pl_w ) < 0 ) pl_act_set( pl_w, N_STAND ) ;
}

/*==============================================================*/
static void act_crouch_turn( PLAYER_WORK *pl_w )
{
	if( pl_cmd_atck_ck( pl_w ) ) return ;
	if( pl_nml_atck_ck( pl_w ) ) return ;
	if( pl_guard_req_ck( pl_w ) ) return ;
	if( pl_jump_ck( pl_w ) ) return ;
	if( pl_fall_ck( pl_w ) ) return ;
	if( pl_char_move( pl_w ) < 0 || pl_w->pl_new_lvbt & ( KEY_FORWARD | KEY_BACK ) ) pl_act_set( pl_w, N_CROUCHING ) ;
}

/*==============================================================*/
/*			Dashing forward										*/
/*==============================================================*/
static void act_dash( PLAYER_WORK *pl_w )
{
	if( pl_w->pl_type != PL_ZANGIEF ) pl_w->catch_atck_ng_timer = 5 ;
	if( pl_w->pl_type == PL_BLACKHEART && !pl_w->cg_prm ) goto dash ;
	if( pl_dash_set( pl_w ) ) return ;
	if( pl_sjump_set( pl_w ) ) return ;
	if( !pl_w->fly_flag ){
		if( pl_cmd_atck_ck( pl_w ) ) return ;
		if( pl_nml_atck_ck( pl_w ) ) return ;
		if( pl_guard_req_ck( pl_w ) ) return ;
		if( pl_jump_ck( pl_w ) ) return ;
		if( pl_crouch_now_ck( pl_w ) ) return ;
	}
	else {
		if( pl_nml_atck_ck( pl_w ) ) return ;
		if( pl_guard_req_ck( pl_w ) ) return ;
		if( pl_w->pl_sw_now & KEY_BACK ){
			pl_act_set( pl_w, N_STAND ) ;
			return ;
		}
	}
dash:
	pl_w->pl_tbl[TBL_DASH]( pl_w ) ;
}

/*==============================================================*/
/*			Dashing back										*/
/*==============================================================*/
static void act_back_dash( PLAYER_WORK *pl_w )
{
	if( pl_w->pl_type == PL_SHUMA_GORATH || ( pl_w->pl_type == PL_BLACKHEART && !pl_w->cg_prm ) ) goto dash ;
	if( pl_dash_set( pl_w ) ) return ;
	if( pl_sjump_set( pl_w ) ) return ;
	if( pl_cmd_atck_ck( pl_w ) ) return ;
	if( pl_nml_atck_ck( pl_w ) ) return ;
	if( pl_guard_req_now_ck( pl_w ) ) return ;
	if( !pl_w->fly_flag ){
		if( pl_jump_ck( pl_w ) ) return ;
		if( pl_crouch_now_ck( pl_w ) ) return ;
	}
	else if( pl_w->pl_sw_now & KEY_FORWARD ) pl_act_set( pl_w, N_STAND ) ;
dash:
	pl_w->pl_tbl[TBL_NOKI]( pl_w ) ;
}

/*==============================================================*/
static void act_dizzy( PLAYER_WORK *pl_w )
{
	pl_w->stun_ng_timer = 180 ;
	pl_w->stun_ng_flag = 0xff ;
	if( vs_sys_work.vs_st < VS_END_WAIT ){
		if( pl_w->vital_new <= 0 ){
			pl_char_move( pl_w ) ;
			return ;
		}
		if( -- pl_w->dizzy_timer >= 0 && pl_w->renda_cnt ){
			pl_char_move( pl_w ) ;
			return ;
		}
	}
	pl_w->renda_cnt = 0 ;
	pl_w->auto_guard_timer = 8 ;
	pl_act_set( pl_w, N_STAND ) ;
}

/*==============================================================*/
static void act_sjump( PLAYER_WORK *pl_w )
{
	if( pl_guard_req_ck( pl_w ) ) return ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type & 0x0f ) return ;
	pl_w->pl_stance = STANCE_AIR ;
	act_st_set( pl_w, N_SJUMP_AIR ) ;
	pl_w->air_jump_cnt = 0 ;
	sjump_se_req( pl_w ) ;
	shock_set( &pl_w->pos, 0 ) ;
}

/*==============================================================*/
/*			Super jump											*/
/*==============================================================*/
static void act_sjump_air( PLAYER_WORK *pl_w )
{
	pl_dir_set( pl_w ) ;
	if( pl_nml_atck_ck( pl_w ) ) return ;
	if( WALL_JUMP_CK( pl_w ) ){
		if( pl_wall_jump_ck( pl_w ) ) return ;
	}
	if( AIR_JUMP_CK( pl_w ) ) pl_air_jump_ck( pl_w ) ;
	if( pl_w->pl_type == PL_STORM ) pl_w->pl_tbl[TBL_EX0]( pl_w ) ;
	pl_char_move( pl_w ) ;
	pl_sjump_ctrl( pl_w ) ;
	pl_jump_move( pl_w ) ;
	if( pl_guard_req_ck( pl_w ) ) return ;
	pl_fall_limit( pl_w ) ;
	pl_sjump_top_ck( pl_w ) ;
	if( pl_land_ck( pl_w ) ) pl_land_set( pl_w ) ;
}

/*==============================================================*/
static void act_air_recover( PLAYER_WORK *pl_w )
{
	if( pl_w->pl_type == PL_DHALSIM && pl_w->old_act_st == N_DAMAGE ){
		if( pl_w->char_no.w == 0xd1b ){
			if( pl_char_move( pl_w ) < 0 ) goto stop ;
			pl_w->pl_tbl[TBL_EX0]( pl_w ) ;
			if( pl_w->cg_type ){
				pl_w->pl_stance = STANCE_STAND ;
				pl_w->spd.x = 0.0f ;
				pl_w->spd.y = 0.0f ;
				pl_w->acc.x = 0.0f ;
				pl_w->acc.y = 0.0f ;
				if( pl_nml_atck_ck( pl_w ) ) goto stop ;
				if( pl_guard_req_ck( pl_w ) ) goto stop ;
			}
		}
		else {
			if( pl_char_move( pl_w ) < 0 ) goto fall ;
			pl_w->pl_tbl[TBL_EX0]( pl_w ) ;
			if( pl_w->cg_type ){
				pl_w->pl_stance = STANCE_AIR ;
				pl_w->spd.x = 0.0f ;
				pl_w->spd.y = 0.0f ;
				pl_w->acc.x = 0.0f ;
				pl_w->acc.y = 0.0f ;
				if( pl_nml_atck_ck( pl_w ) ){
					pl_color_set( pl_w, 1 ) ;
					return ;
				}
				if( pl_guard_req_ck( pl_w ) ) pl_color_set( pl_w, 1 ) ;
			}
		}
	}
	else {
		dm_blow_move( pl_w ) ;
		if( pl_char_move( pl_w ) < 0 ) goto fall ;
		if( pl_nml_atck_ck( pl_w ) ) return ;
		if( pl_guard_req_ck( pl_w ) ) return ;
		if( pl_land_ck( pl_w ) ) pl_land_set( pl_w ) ;
	}
	return ;
fall:
	pl_w->routine_1 = PL_NORM ;
	pl_w->jump_dir = 1 ;
	pl_sjump_ret( pl_w ) ;
	return ;
stop:
	pl_color_set( pl_w, 1 ) ;
	pl_act_set( pl_w, N_STAND ) ;
}

/*==============================================================*/
static void act_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_guard_req_ck( pl_w ) ) return ;
	if( pl_land_ck( pl_w ) ) pl_land_set( pl_w ) ;
}

/*==============================================================*/
static void act_get_up( PLAYER_WORK *pl_w )
{
	static void ( *act_get_up_jmp[3] )( PLAYER_WORK *pl_w ) = {
		act_get_up_init, act_get_up_rise, act_get_up_roll,
	} ;

	act_get_up_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void act_get_up_init( PLAYER_WORK *pl_w )
{
	Float spd ;

	pl_w->routine_2 ++ ;
	pl_w->nml_catch_ng_timer = 8 ;
	if( pl_w->jump_dir < 0 ){
		act_get_up_rise( pl_w ) ;
		return ;
	}
	pl_w->routine_2 ++ ;
	pl_w->catch_ng_timer = 1 ;
	pl_w->dm_flag = 0 ;
	pl_dir_set( pl_w ) ;
	if( !pl_w->jump_dir ) spd = -15.0f ;
	else spd = 10.0f ;
	if( pl_w->pl_dir ) spd = -spd ;
	pl_w->spd.x = spd ;
	act_get_up_roll( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void act_get_up_rise( PLAYER_WORK *pl_w )
{
	pl_w->catch_ng_timer = 1 ;
	pl_w->dm_flag = 0 ;
	pl_w->nml_catch_ng_timer = 4 ;
	pl_w->pl_stance = STANCE_STAND ;
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void act_get_up_roll( PLAYER_WORK *pl_w )
{
	pl_w->catch_ng_timer = 1 ;
	pl_w->dm_flag = 0 ;
	pl_w->nml_catch_ng_timer = 4 ;
	pl_w->push_ng_timer = 1 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->pl_stance = STANCE_STAND ;
		pl_stand_ret( pl_w ) ;
		return ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
}

/*==============================================================*/
/*			Guarding											*/
/*==============================================================*/
static void act_guard( PLAYER_WORK *pl_w )
{
	static void ( *act_guard_jmp[4] )( PLAYER_WORK *pl_w ) = {
		act_guard_start, act_guard_hold, act_guard_end, act_guard_back,
	} ;

	dm_back_move( pl_w ) ;
	act_guard_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void act_guard_start( PLAYER_WORK *pl_w )
{
	if( pl_cmd_atck_ck( pl_w ) ) return ;
	if( pl_nml_atck_ck( pl_w ) ) return ;
	if( pl_dash_set( pl_w ) ) return ;
	if( pl_sjump_set( pl_w ) ){
		pl_act_set( pl_w, N_AIR_GUARD ) ;
		return ;
	}
	if( pl_char_move( pl_w ) >= 0 ) return ;
	pl_w->routine_2 ++ ;
	pl_w->char_no.b.no = pl_w->pl_stance == STANCE_CROUCH ? 5 : 1 ;
	pl_char_set( pl_w, Char_Guard, pl_w->char_no.b.no ) ;
}

/*--------------------------------------------------------------*/
static void act_guard_hold( PLAYER_WORK *pl_w )
{
	if( pl_w->guard_back_flag < 0 && pl_w->guard_timer ){
		act_guard_back_init( pl_w ) ;
		return ;
	}
	if( pl_w->pl_stance != STANCE_CROUCH ){
		if( pl_w->pl_new_lvbt & KEY_DOWN ){
			pl_w->pl_stance = STANCE_CROUCH ;
			pl_char_set( pl_w, Char_Guard, 5 ) ;
		}
	}
	else if( !( pl_w->pl_new_lvbt & KEY_DOWN ) ){
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Guard, 1 ) ;
	}
	pl_char_move( pl_w ) ;
	if( pl_w->guard_timer ){
		pl_w->guard_timer -- ;
		return ;
	}
	pl_w->guard_cnt = 0 ;
	if( pl_dash_set( pl_w ) ) return ;
	if( pl_sjump_set( pl_w ) ){
		Uint8 step = pl_w->routine_2 ;
		pl_act_set( pl_w, N_AIR_GUARD ) ;
		pl_w->routine_2 = step ;
		return ;
	}
	if( pl_cmd_atck_ck( pl_w ) ) return ;
	if( pl_nml_atck_ck( pl_w ) ) return ;
	if( pl_jump_ck( pl_w ) ){
		Uint8 step = pl_w->routine_2 ;
		pl_act_set( pl_w, N_AIR_GUARD ) ;
		pl_w->routine_2 = step ;
		return ;
	}
	if( pl_guard_dir_ck( pl_w ) ) return ;
	pl_w->routine_2 = 2 ;
	pl_w->char_no.b.no = pl_w->pl_stance == STANCE_CROUCH ? 6 : 2 ;
	pl_char_set( pl_w, Char_Guard, pl_w->char_no.b.no ) ;
}

/*--------------------------------------------------------------*/
static void act_guard_end( PLAYER_WORK *pl_w )
{
	if( pl_w->guard_back_flag >= 0 ){
		if( pl_dash_set( pl_w ) ) return ;
		if( pl_sjump_set( pl_w ) ) return ;
		if( pl_w->pl_new_lvbt & KEY_ATCK ){
			pl_stand_ret( pl_w ) ;
			return ;
		}
	}
	if( pl_char_move( pl_w ) >= 0 ) return ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			The guard's push back, by the steps of				*/
/*			guard_back_tbl										*/
/*==============================================================*/
static void act_guard_back( PLAYER_WORK *pl_w )
{
	static Sint16 guard_back_tbl[28] = {
		0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x0c00, 0x0c00,
		0x0c00, 0x0c00, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0400,
		0x0400, 0x0400, 0x0400, 0x0200, 0x0200, 0x0200, 0x0200, 0x0100,
		0x0100, 0x0100, 0x0100, 0x0000,
	} ;
	PLAYER_WORK *wk ;
	Float x ;

	wk = pl_w->dm_at_w ;
	if( !wk ) wk = pl_w->target_w ;
	x = guard_back_tbl[pl_w->timer0] * 1.6666666f / 256.0f ;
	if( x == 0.0f ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	pl_w->timer0 ++ ;
	if( wk->disp_dir ) x = -x ;
	if( !( wk->wall_flag & 1 << wk->disp_dir ) ) wk->pos.x += x ;
}

/*==============================================================*/
static void act_guard_back_init( PLAYER_WORK *pl_w )
{
	static Sint16 guard_back_end[2] = { -1, -1 } ;
	VEC3 pos ;

	pl_w->step_adrs = guard_back_end ;
	pl_w->routine_2 = 3 ;
	pl_w->timer0 = 0 ;
	pl_w->guard_back_flag = 0 ;
	pos.x = pl_w->guard_pos.x ;
	pos.y = pl_w->guard_pos.y ;
	pos.z = pl_w->pos.z ;
	guard_wave_set( &pos, pl_w->disp_dir ) ;
	pl_se_req( pl_w, 68 ) ;
}

/*==============================================================*/
/*			Guarding in the air									*/
/*==============================================================*/
static void act_air_guard( PLAYER_WORK *pl_w )
{
	static void ( *act_air_guard_jmp[3] )( PLAYER_WORK *pl_w ) = {
		act_air_guard_start, act_air_guard_hold, act_air_guard_back,
	} ;

	act_air_guard_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void act_air_guard_start( PLAYER_WORK *pl_w )
{
	pl_jump_move( pl_w ) ;
	pl_sjump_top_ck( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Air_Guard, 1 ) ;
		return ;
	}
	if( pl_nml_atck_ck( pl_w ) ) return ;
	if( pl_land_ck( pl_w ) && !pl_guard_req_ck( pl_w ) ) pl_land_set( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void act_air_guard_hold( PLAYER_WORK *pl_w )
{
	VEC3 pos ;

	if( pl_w->guard_back_flag < 0 && pl_w->guard_timer ){
		pl_w->routine_2 = 2 ;
		pl_w->timer0 = 28 ;
		pl_w->guard_back_flag = 0 ;
		pos.x = pl_w->guard_pos.x ;
		pos.y = pl_w->guard_pos.y ;
		pos.z = pl_w->pos.z ;
		guard_wave_set( &pos, pl_w->disp_dir ) ;
		pl_se_req( pl_w, 68 ) ;
		return ;
	}
	pl_jump_move( pl_w ) ;
	pl_sjump_top_ck( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( pl_w->guard_timer ){
		pl_w->guard_timer -- ;
		back_move_wall( pl_w ) ;
	}
	else {
		pl_w->guard_cnt = 0 ;
		if( pl_nml_atck_ck( pl_w ) ) return ;
		if( !pl_guard_dir_ck( pl_w ) ){
			if( pl_w->acc.y > -0.06696428f ) pl_w->acc.y = -DOT_Y( 0.375f ) ;
			pl_w->acc.x = 0.0f ;
			if( !pl_w->sjump_flag ){
				pl_w->routine_2 = 1 ;
				act_st_set( pl_w, N_JUMP_AIR ) ;
			}
			else {
				pl_w->routine_2 = 1 ;
				act_st_set( pl_w, N_SJUMP_AIR ) ;
			}
			pl_char_set( pl_w, Char_Jump, 9 ) ;
			return ;
		}
	}
	if( pl_land_ck( pl_w ) ){
		if( pl_w->guard_timer ) pl_w->auto_guard_timer = 8 ;
		if( !pl_guard_req_ck( pl_w ) && !pl_crouch_ck( pl_w ) ) pl_land_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void act_air_guard_back( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *wk ;
	Float x ;

	wk = pl_w->dm_at_w ;
	if( !wk ) wk = pl_w->target_w ;
	if( -- pl_w->timer0 <= 0 ){
		pl_jump_ret( pl_w ) ;
		return ;
	}
	x = 5.0f ;
	if( wk->disp_dir ) x = -5.0f ;
	if( !( wk->wall_flag & 1 << wk->disp_dir ) ) wk->pos.x += x ;
}

/*==============================================================*/
static void act_nml_atck( PLAYER_WORK *pl_w )
{
	if( pl_sjump_set( pl_w ) ) return ;
	if( pl_w->pl_stance != STANCE_AIR && pl_w->atck_level != 1 && pl_w->hit_flag && !( pl_w->hit_flag & ( HR_GUARD | HR_ONCE ) ) ){
		pl_w->hit_flag |= HR_ONCE ;
		if( ( !pl_w->pl_dir && pl_w->spd.x < 0.0f ) || ( pl_w->pl_dir && pl_w->spd.x > 0.0f ) ) pl_w->acc.x *= 4.0f ;
	}
	if( pl_nml_atck_ck( pl_w ) ) return ;
	pl_w->pl_tbl[TBL_ATCK]( pl_w ) ;
	if( pl_w->speed_up_flag && pl_w->act_st == N_NML_ATCK ) pl_w->pl_tbl[TBL_ATCK]( pl_w ) ;
}

/*==============================================================*/
/*			The character's special moves						*/
/*==============================================================*/
static void act_special( PLAYER_WORK *pl_w )
{
	pl_w->pl_tbl[TBL_SP_ATCK]( pl_w ) ;
}

/*==============================================================*/
static void act_win( PLAYER_WORK *pl_w )
{
	pl_w->pl_tbl[TBL_WIN]( pl_w ) ;
}

/*==============================================================*/
static void act_down( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *wk1, *wk2 ;

	if( !pl_w->routine_2 ){
		pl_char_move( pl_w ) ;
		if( pl_w->vital_new ){
			if( pl_w->renda_cnt && pl_w->down_timer ) return ;
			if( ABYSS_CK( pl_w ) ) return ;
			pl_w->push_ng_timer = 0 ;
			pl_w->jump_dir = -1 ;
			pl_w->renda_cnt = 0 ;
			pl_act_set( pl_w, N_GET_UP ) ;
			return ;
		} else if( vs_sys_work.vs_st < VS_END_WAIT ){
			wk1 = vs_sys_work.team_w[pl_w->pl_no][( pl_w->team_no + 2 ) % 3] ;
			wk2 = vs_sys_work.team_w[pl_w->pl_no][( pl_w->team_no + 4 ) % 3] ;
			combo_reset( pl_w ) ;
			if( wk1->vital_new || wk2->vital_new ){
				pl_w->catch_ng_timer = 2 ;
				pl_w->timer0 = 60 ;
				if( pl_w->team_no ) pl_w->timer0 = 0 ;
				pl_w->routine_2 ++ ;
				return ;
			}
		}
		if( vs_sys_work.winner_id >= 0 && vs_sys_work.winner_id == pl_w->pl_no && !pl_w->down_timer ){
			if( !ABYSS_CK( pl_w ) ){
				combo_reset( pl_w ) ;
				pl_w->catch_ng_timer = 2 ;
				pl_w->timer0 = 4 ;
				if( pl_w->team_no ) pl_w->timer0 = 0 ;
				pl_w->routine_2 ++ ;
			}
		}
		return ;
	}
	combo_reset( pl_w ) ;
	if( vs_sys_w->ko_cnt[pl_w->pl_no] >= 3 ) return ;
	pl_w->catch_ng_timer = 2 ;
	if( pl_w->team_ng_timer ) return ;
	if( !pl_w->team_no ){
		if( vs_sys_work.be_cnt[pl_w->pl_no] != 1 ) return ;
		if( -- pl_w->timer0 > 0 ) return ;
	}
	pl_appear_set( pl_w ) ;
	pl_w->change_req = 2 ;
}

/*==============================================================*/
static void act_fly( PLAYER_WORK *pl_w )
{
	pl_fly( pl_w ) ;
}

/*==============================================================*/
/*			Jumping off the screen's edge						*/
/*==============================================================*/
static void act_wall_jump( PLAYER_WORK *pl_w )
{
	Float x ;

	switch( pl_w->pl_type ){
	case PL_FELICIA :
		pl_w->pl_tbl[TBL_TJUMP]( pl_w ) ;
		return ;
	}
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->jump_dash_cnt ++ ;
		pl_w->jump_com_cnt = 0 ;
		pl_w->jump_atk_cnt = JATK_ONE ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_char_set( pl_w, Char_Jump, 12 ) ;
	}
	else {
		pl_w->pos.x -= scr_work.eye[1].x - scr_work.eye[1].x ;
		if( pl_char_move( pl_w ) >= 0 ){
			if( ( x = pl_w->cg_type ) ){
				if( pl_w->pl_dir ) x = -x ;
				pl_w->pos.x += DOT_X( x ) ;
				pl_w->cg_type = 0 ;
			}
		} else {
			act_st_set( pl_w, N_JUMP ) ;
			pl_w->routine_2 = 1 ;
			pl_w->pl_stance = STANCE_AIR ;
			pl_w->disp_dir = pl_w->pl_dir ;
			if( FLAG_CK( pl_w->pl_bit, PLB0( PL_WOLVERINE ), PLB1( PL_BONE_WOLVERINE ) ) ) pl_char_set2( pl_w, Char_Jump, 1, 2 ) ;
			else pl_char_set( pl_w, Char_Jump, 9 ) ;
		}
	}
}

/*==============================================================*/
static void act_air_dash( PLAYER_WORK *pl_w )
{
	if( pl_nml_atck_ck( pl_w ) ) return ;
	pl_fly_start( pl_w ) ;
}

/*==============================================================*/
/*			The round-end type read as signed: 0x80 is set		*/
/*			when the move is over								*/
/*==============================================================*/
static void act_escape( PLAYER_WORK *pl_w )
{
	pl_w->catch_ng_timer = 2 ;
	if( ( Sint8 )pl_w->type == 0 ){
		if( pl_char_move( pl_w ) < 0 ){
			pl_jump_ret( pl_w ) ;
			return ;
		}
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_land_ck( pl_w ) && !pl_guard_req_ck( pl_w ) && !pl_crouch_ck( pl_w ) ) pl_land_set( pl_w ) ;
	} else if( ( Sint8 )pl_w->type > 0 ){
		pl_w->pos.x += pl_w->spd.x ;
		if( pl_char_move( pl_w ) >= 0 ) return ;
		pl_w->type = 0x80 ;
		pl_char_set( pl_w, Char_Crouch, 1 ) ;
	} else {
		pl_char_move( pl_w ) ;
		pl_smoke_set( pl_w ) ;
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		if( pl_w->spd.x * pl_w->acc.x >= 0.0f ) pl_act_set( pl_w, N_CROUCHING ) ;
	}
}

/*==============================================================*/
/*			By change_type											*/
/*==============================================================*/
static void act_change( PLAYER_WORK *pl_w )
{
	static void ( *act_change_jmp[12] )( PLAYER_WORK *pl_w ) = {
		act_change_tag, act_change_tag_air, act_change_out, act_change_gattai, act_change_vcounter, act_change_zero,
		act_change_out, act_change_assist, act_change_hyper, act_change_t_kick, act_change_call, act_change_assist_end,
	} ;

	act_change_jmp[pl_w->change_type]( pl_w ) ;
}

/*==============================================================*/
static void act_change_tag( PLAYER_WORK *pl_w )
{
	static void ( *act_change_tag_jmp[3] )( PLAYER_WORK *pl_w ) = {
		act_change_tag_init, act_change_tag_wait, act_change_tag_end,
	} ;

	pl_w->catch_ng_timer = 2 ;
	act_change_tag_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void act_change_tag_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->timer0 = 16 ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_dir_set( pl_w ) ;
	pl_char_set( pl_w, Char_Change, 0 ) ;
}

/*--------------------------------------------------------------*/
static void act_change_tag_wait( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->change_req = 1 ;
	}
}

/*--------------------------------------------------------------*/
static void act_change_tag_end( PLAYER_WORK *pl_w )
{
	pl_w->wall_ng_timer = 2 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->change_type = 2 ;
		pl_w->routine_2 = pl_w->routine_3 = 0 ;
	}
}

/*==============================================================*/
static void act_change_tag_air( PLAYER_WORK *pl_w )
{
	pl_w->catch_ng_timer = 2 ;
	pl_w->change_req = 1 ;
	pl_w->change_type = 2 ;
}

/*==============================================================*/
static void act_change_out( PLAYER_WORK *pl_w )
{
	static void ( *act_change_out_jmp[4] )( PLAYER_WORK *pl_w ) = {
		act_change_out_init, act_change_out_recover, act_change_out_fall, act_change_out_hop,
	} ;

	pl_w->catch_ng_timer = 2 ;
	pl_w->wall_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	act_change_out_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void act_change_out_init( PLAYER_WORK *pl_w )
{
	pl_w->fly_flag = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->rot[2] = 0 ;
	pl_w->scale = pl_w->pl_scale ;
	pl_w->alpha = 1.0f ;
	pl_color_set( pl_w, 1 ) ;
	pl_voice_req( pl_w, 43 ) ;
	if( pl_w->pl_stance == STANCE_AIR ){
		pl_w->routine_2 = 1 ;
		pl_char_set( pl_w, Char_Damage, 30 ) ;
		if( pl_w->old_act_st == N_SPECIAL || pl_w->old_act_st == N_SUPER ){
			pl_w->spd.x = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.y = -DOT_Y( 0.375f ) ;
		}
	}
	else {
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->routine_2 = 3 ;
		act_change_out_hop( pl_w ) ;
	}
	combo_reset( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void act_change_out_recover( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	dm_spd_y_limit( pl_w ) ;
	if( pl_char_move( pl_w ) >= 0 ){
		if( pl_land_ck( pl_w ) ){
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_w->pl_stance = STANCE_STAND ;
			pl_w->pos.y = pl_w->ground_y ;
			pl_w->routine_2 = 3 ;
			act_change_out_hop( pl_w ) ;
		}
		return ;
	}
	pl_w->routine_2 ++ ;
	pl_w->jump_dir = 1 ;
	pl_w->sjump_flag = 1 ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_char_set( pl_w, Char_Jump, 11 ) ;
}

/*--------------------------------------------------------------*/
static void act_change_out_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_jump_move( pl_w ) ;
	pl_fall_limit( pl_w ) ;
	pl_sjump_top_ck( pl_w ) ;
	if( pl_land_ck( pl_w ) ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->routine_2 = 3 ;
		act_change_out_hop( pl_w ) ;
	}
}

/*==============================================================*/
/*			A bounce off the floor								*/
/*==============================================================*/
static void act_change_out_bound( PLAYER_WORK *pl_w )
{
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->spd.x = DOT_X( 10.0f ) ;
	pl_w->acc.x = 0.0f ;
	if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
	pl_w->spd.y = DOT_Y( 8.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_char_set( pl_w, Char_Change, 2 ) ;
}

/*--------------------------------------------------------------*/
static void act_change_out_hop( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_3 ){
		pl_w->routine_3 ++ ;
		act_change_out_bound( pl_w ) ;
	}
	else {
		pl_char_move( pl_w ) ;
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_land_ck( pl_w ) ) act_change_out_bound( pl_w ) ;
		if( !scr_in_ck( pl_w ) ){
			pl_w->be_flag = 0 ;
			pl_w->disp_flag = 0 ;
			pl_change_out( pl_w ) ;
			team_mark_set( pl_w ) ;
		}
	}
}

/*==============================================================*/
static void act_change_gattai( PLAYER_WORK *pl_w )
{
	static void ( *act_change_gattai_jmp[2] )( PLAYER_WORK *pl_w ) = {
		act_change_gattai_init, act_change_gattai_move,
	} ;

	pl_w->catch_ng_timer = 2 ;
	pl_w->wall_ng_timer = 2 ;
	pl_w->stop_move_timer = 2 ;
	act_change_gattai_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void act_change_gattai_init( PLAYER_WORK *pl_w )
{
	pl_w->stop_move_flag = 0xff ;
	pl_w->stop_move_timer = 16 ;
	pl_w->routine_2 ++ ;
	pl_w->wall_flag = 0 ;
	pl_w->disp_flag = 1 ;
	pl_change_in_set( pl_w ) ;
	pl_w->stun_cnt = 80 ;
	pl_char_set( pl_w, Char_Jump, 1 ) ;
	pl_color_set( pl_w, 1 ) ;
	pl_w->timer0 = ( pl_w->team_no - 1 ) * 10 ;
}

/*--------------------------------------------------------------*/
static void act_change_gattai_move( PLAYER_WORK *pl_w )
{
	if( pl_w->timer0 ){
		pl_w->timer0 -- ;
		return ;
	}
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y > pl_w->ground_y ) return ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->stop_move_flag = 0 ;
	pl_w->stop_move_timer = 0 ;
	pl_char_set( pl_w, Char_Stand, 0 ) ;
	pl_w->pl_tbl[TBL_SP_GATTAI]( pl_w ) ;
	pl_land_ef_set( pl_w ) ;
}

/*==============================================================*/
static void act_change_vcounter( PLAYER_WORK *pl_w )
{
	static void ( *act_change_vcounter_jmp[2] )( PLAYER_WORK *pl_w ) = {
		act_change_vcounter_init, act_change_vcounter_end,
	} ;

	pl_w->catch_ng_timer = 2 ;
	act_change_vcounter_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void act_change_vcounter_init( PLAYER_WORK *pl_w )
{
	VEC3 pos ;

	pl_w->routine_2 ++ ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_char_set( pl_w, Char_Change, 1 ) ;
	pl_w->change_req = 3 ;
	*( VEC2 * )&pos = pl_w->guard_pos ;
	pos.z = pl_w->pos.z ;
	clash_ef_set( &pos, pl_w->disp_dir ) ;
	pl_se_req( pl_w, 63 ) ;
}

/*--------------------------------------------------------------*/
static void act_change_vcounter_end( PLAYER_WORK *pl_w )
{
	pl_w->change_type = 2 ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
}

/*==============================================================*/
static void act_change_zero( PLAYER_WORK *pl_w )
{
	static void ( *act_change_zero_jmp[2] )( PLAYER_WORK *pl_w ) = {
		act_change_zero_init, act_change_zero_move,
	} ;

	pl_w->catch_ng_timer = 2 ;
	pl_w->wall_ng_timer = 2 ;
	pl_w->stop_move_timer = 2 ;
	act_change_zero_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void act_change_zero_init( PLAYER_WORK *pl_w )
{
	pl_w->stop_move_flag = 0xff ;
	pl_w->stop_move_timer = 16 ;
	pl_w->routine_2 ++ ;
	pl_w->wall_flag = 0 ;
	pl_w->disp_flag = 1 ;
	pl_w->stun_cnt = 80 ;
	pl_char_set( pl_w, Char_Jump, 1 ) ;
	pl_color_set( pl_w, 1 ) ;
}

/*--------------------------------------------------------------*/
static void act_change_zero_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y > pl_w->ground_y ) return ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->stop_move_flag = 0 ;
	pl_w->stop_move_timer = 0 ;
	pl_char_set( pl_w, Char_Stand, 0 ) ;
	pl_w->pl_tbl[TBL_SP_ZERO]( pl_w ) ;
	pl_w->vcounter_msg_timer = 60 ;
	pl_land_ef_set( pl_w ) ;
}

/*==============================================================*/
static void act_change_assist( PLAYER_WORK *pl_w )
{
	static void ( *act_change_assist_jmp[2] )( PLAYER_WORK *pl_w ) = {
		act_change_assist_init, act_change_assist_move,
	} ;

	pl_w->catch_ng_timer = 2 ;
	pl_w->wall_ng_timer = 2 ;
	act_change_assist_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void act_change_assist_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->wall_flag = 0 ;
	pl_w->disp_flag = 1 ;
	pl_assist_in_set( pl_w ) ;
	pl_w->stun_cnt = 80 ;
	pl_char_set( pl_w, Char_Jump, 1 ) ;
	pl_color_set( pl_w, 1 ) ;
}

/*--------------------------------------------------------------*/
static void act_change_assist_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y > pl_w->ground_y ) return ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_char_set( pl_w, Char_Stand, 0 ) ;
	pl_land_ef_set( pl_w ) ;
	if( vs_sys_w->team_w[pl_w->pl_no][0]->routine_1 >= PL_CAUGHT ) pl_act_set( pl_w, N_STAND ) ;
	else pl_w->pl_tbl[TBL_SP_ASSIST]( pl_w ) ;
}

/*==============================================================*/
static void act_change_hyper( PLAYER_WORK *pl_w )
{
	static void ( *act_change_hyper_jmp[2] )( PLAYER_WORK *pl_w ) = {
		act_change_hyper_init, act_change_hyper_move,
	} ;

	pl_w->catch_ng_timer = 2 ;
	pl_w->wall_ng_timer = 2 ;
	pl_w->stop_move_timer = 2 ;
	act_change_hyper_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void act_change_hyper_init( PLAYER_WORK *pl_w )
{
	pl_w->stop_move_flag = 0xff ;
	pl_w->stop_move_timer = 16 ;
	pl_w->routine_2 ++ ;
	pl_w->wall_flag = 0 ;
	pl_w->disp_flag = 1 ;
	pl_w->stun_cnt = 80 ;
	pl_char_set( pl_w, Char_Jump, 1 ) ;
	pl_color_set( pl_w, 1 ) ;
}

/*--------------------------------------------------------------*/
static void act_change_hyper_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y > pl_w->ground_y ) return ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_char_set( pl_w, Char_Stand, 0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_3 = 0 ;
	pl_w->routine_2 = 0 ;
	pl_w->pl_sp_type = pl_w->delay_sp_type ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_land_ef_set( pl_w ) ;
}

/*==============================================================*/
static void act_change_t_kick( PLAYER_WORK *pl_w )
{
	pl_w->pl_tbl[TBL_T_KICK]( pl_w ) ;
}

/*==============================================================*/
static void act_change_call( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		if( pl_w->float_flag ){
			if( pl_w->pos.y < pl_w->ground_y + DOT_Y( 16.0f ) ) pl_w->pos.y = pl_w->float_y ;
		}
		else pl_w->pos.y = pl_w->ground_y ;
		pl_char_set( pl_w, Char_Change, 3 ) ;
	}
	else if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void act_change_assist_end( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->timer0 = 40 ;
		pl_char_set( pl_w, Char_Change, 4 ) ;
	}
	else {
		pl_char_move( pl_w ) ;
		if( -- pl_w->timer0 < 0 ){
			pl_w->change_type = 2 ;
			pl_w->routine_2 = pl_w->routine_3 = 0 ;
			pl_w->catch_ng_timer = 2 ;
		}
	}
}

/*==============================================================*/
static void act_cmd_atck( PLAYER_WORK *pl_w )
{
	if( pl_sjump_set( pl_w ) ) return ;
	if( pl_w->pl_stance != STANCE_AIR && pl_w->atck_level != 1 && pl_w->hit_flag && !( pl_w->hit_flag & ( HR_GUARD | HR_ONCE ) ) ){
		pl_w->hit_flag |= HR_ONCE ;
		if( ( !pl_w->pl_dir && pl_w->spd.x < 0.0f ) || ( pl_w->pl_dir && pl_w->spd.x > 0.0f ) ) pl_w->acc.x *= 4.0f ;
	}
	pl_w->pl_tbl[TBL_UPPER]( pl_w ) ;
	if( pl_w->speed_up_flag && pl_w->act_st == N_CMD_ATCK ) pl_w->pl_tbl[TBL_UPPER]( pl_w ) ;
}

/****************************************************************/
/*			Throwing: the character's own throw					*/
/****************************************************************/
void pl_catch( PLAYER_WORK *pl_w )
{
	pl_w->pl_tbl[TBL_CATCH]( pl_w ) ;
}

/****************************************************************/
/*			Being thrown: the thrower's routine moves this		*/
/*			player, or the throw type's own						*/
/****************************************************************/
void pl_caught( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *wk ;

	pl_w->fly_flag = 0 ;
	pl_w->push_ng_timer = 1 ;
	if( pl_w->vital_new && vs_sys_work.vs_st == VS_FIGHT ){
		if( pl_catch_escape( pl_w ) ) return ;
	}
	if( pl_w->catch_type ){
		caught_move( pl_w ) ;
		return ;
	}
	wk = pl_w->catch_w ;
	if( !wk ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( wk->hit_stop ) goto move ;
	if( wk->change_req ) goto move ;
	if( wk->catch_flag ) goto release ;
	if( !pl_w->vital_new ){
		dm_ko_set( pl_w ) ;
		return ;
	}
	pl_color_set( pl_w, 1 ) ;
	if( pl_land_ck( pl_w ) ) pl_stand_ret( pl_w ) ;
	else pl_jump_ret( pl_w ) ;
	return ;
release :
	wk->catch_flag = 0 ;
move :
	wk->pl_tbl[TBL_HOLD]( pl_w ) ;
}

/*==============================================================*/
/*			By the throw's type									*/
/*==============================================================*/
static void caught_move( PLAYER_WORK *pl_w )
{
	static void ( *caught_jmp[21] )( PLAYER_WORK *pl_w ) = {
		caught_blow, caught_blow, caught_slam, caught_down, caught_dm, caught_dm, caught_hold,
		caught_blow_fix, caught_down_combo, caught_elem, caught_elem, caught_slam_hard, caught_wait, caught_toss,
		caught_burn, caught_spin, caught_blow, caught_blow, caught_blow, caught_shock, caught_drop,
	} ;

	caught_jmp[pl_w->catch_type]( pl_w ) ;
}

/****************************************************************/
/*			Thrown and let go: flung by the throw's hit,		*/
/*			landing on the ground								*/
/****************************************************************/
void caught_blow( PLAYER_WORK *pl_w )
{
	static void ( *caught_00_jmp[4] )( PLAYER_WORK *pl_w ) = {
		caught_blow_init, caught_blow_fly, caught_blow_down, caught_end,
	} ;

	if( pl_w->catch_type == 17 || pl_w->catch_type == 18 ) pl_w->catch_ng_timer = 2 ;
	caught_00_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void caught_blow_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->catch_cnt ++ ;
	pl_w->wall_hit_flag = 0 ;
	first_atck_catch( pl_w ) ;
	pl_w->hit_data_adrs = &pl_w->catch_w->hit_data_tbl[pl_w->atck_no & ATCK_NO_MASK] ;
	if( pl_w->catch_type == 16 || pl_w->catch_type == 18 ) catch_hit_set2( pl_w ) ;
	else catch_hit_set( pl_w ) ;
	if( pl_w->hit_data_adrs->damage_type & FLY_SCREEN ) pl_w->fly_screen_flag = 1 ;
	pl_w->recover_timer = pl_w->dm_recover_time ;
	pl_w->disp_dir = pl_w->pl_dir ;
	pl_w->pl_stance = STANCE_AIR ;
	if( pl_w->atck_no & ATCK_DIR ){
		pl_w->dm_dir = pl_w->catch_w->dir ;
		if( pl_w->catch_w->disp_dir ) pl_w->dm_dir = ( 32 - pl_w->dm_dir ) & 31 ;
	}
	pl_w->disp_flag = 1 ;
	pl_w->rot[2] = 0 ;
	pl_w->scale = pl_w->pl_scale ;
	pl_w->alpha = 1.0f ;
	dm_blow_char_set( pl_w ) ;
	catch_blow_spd_set( pl_w ) ;
	pl_w->acc.y = -1.2053571f ;
	pl_w->spd.x = pl_w->back_spd_x ;
	pl_w->spd.y = pl_w->back_spd_y ;
	pl_w->timer1 = 0 ;
	caught_blow_fly( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void caught_blow_fly( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->wall_flag && !pl_w->wall_hit_flag ){
		pl_w->wall_hit_flag = 1 ;
		down_se_req( pl_w ) ;
		wall_dust_set( pl_w ) ;
		if( pl_w->fly_screen_flag ){ }
	}
	if( !pl_w->timer1 && pl_w->spd.y <= 0.0f ){
		pl_w->timer1 = 1 ;
		dm_fall_char_set( pl_w ) ;
	}
	if( pl_w->dm_no != 2 && -- pl_w->recover_timer < 0 && pl_w->vital_new ){
		pl_w->routine_2 = 3 ;
		pl_w->jump_atk_cnt = -1 ;
		pl_w->sjump_flag = 1 ;
		pl_char_set( pl_w, Char_Damage, 30 ) ;
		caught_end( pl_w ) ;
	}
	else if( pl_w->spd.y <= 0.0f && pl_land_ck( pl_w ) ){
		pl_w->nml_catch_ng_timer = 2 ;
		pl_w->escape_timer = 5 ;
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_DOWN ;
		pl_char_set( pl_w, Char_Damage, 26 ) ;
	}
}

/*==============================================================*/
/*			Down: a quake for a heavy fall, then a quick		*/
/*			recovery or getting up								*/
/*==============================================================*/
static void caught_blow_down( PLAYER_WORK *pl_w )
{
	Sint8 lv ;

	pl_w->nml_catch_ng_timer = 2 ;
	if( pl_w->escape_timer < 0 || ( pl_w->vital_new > 0 && pl_w->escape_timer > 0 ) ){
	} else {
		pl_w->escape_timer = -1 ;
		land_se_req( pl_w ) ;
		land_dust_set( &pl_w->pos, pl_w->pl_no ) ;
		if( pl_w->dm_no != 1 ){
			lv = pl_w->weight < 5 ? 1 : 3 ;
			scr_work.quake_level = lv ;
			scr_work.quake_flag = 1 ;
		}
		if( pl_w->fly_screen_flag || !pl_w->vital_new || !pl_w->ukemi_flag ){
		} else {
			if( pl_w->cpu_flag ){
				if( em_rate_ck( pl_w, 29, 2 ) ) pl_w->ukemi_flag = -1 ;
				else pl_w->ukemi_flag = 0 ;
			}
			if( pl_w->ukemi_flag < 0 ){
				pl_w->jump_dir = 0 ;
				pl_act_set( pl_w, N_GET_UP ) ;
				return ;
			}
		}
	}
	if( pl_char_move( pl_w ) < 0 ) pl_act_set( pl_w, N_DOWN ) ;
}

/*==============================================================*/
/*			Let go in the air: fall, and act again once the		*/
/*			animation ends										*/
/*==============================================================*/
static void caught_end( PLAYER_WORK *pl_w )
{
	if( pl_w->pl_type == PL_DHALSIM ){
		if( pl_char_move( pl_w ) < 0 ){
			pl_w->routine_1 = PL_NORM ;
			pl_w->jump_dir = 1 ;
			pl_sjump_ret( pl_w ) ;
		}
		else {
			pl_w->pl_tbl[TBL_EX0]( pl_w ) ;
			if( pl_w->cg_type ){
				pl_w->pl_stance = STANCE_AIR ;
				pl_w->spd.x = 0.0f ;
				pl_w->spd.y = 0.0f ;
				pl_w->acc.x = 0.0f ;
				pl_w->acc.y = 0.0f ;
				if( pl_nml_atck_ck( pl_w ) ){
					pl_color_set( pl_w, 1 ) ;
					return ;
				}
				if( pl_guard_req_ck( pl_w ) ){
					pl_color_set( pl_w, 1 ) ;
					return ;
				}
				if( pl_land_ck( pl_w ) ){
					pl_color_set( pl_w, 1 ) ;
					pl_land_set( pl_w ) ;
				}
			}
		}
		return ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	dm_spd_y_limit( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_1 = PL_NORM ;
		pl_w->jump_dir = 1 ;
		pl_sjump_ret( pl_w ) ;
	}
	else {
		if( pl_nml_atck_ck( pl_w ) ) return ;
		if( pl_guard_req_ck( pl_w ) ) return ;
		if( pl_land_ck( pl_w ) ) pl_land_set( pl_w ) ;
	}
}

/****************************************************************/
/*			Slammed down: bounced off the ground where the		*/
/*			throw lets go										*/
/****************************************************************/
void caught_slam( PLAYER_WORK *pl_w )
{
	static void ( *caught_02_jmp[5] )( PLAYER_WORK *pl_w ) = {
		caught_slam_init, caught_slam_bound, caught_slam_fly, caught_slam_land, caught_slam_down,
	} ;

	caught_02_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void caught_slam_init( PLAYER_WORK *pl_w )
{
	VEC3 pos ;
	Sint8 lv ;

	pl_w->routine_2 ++ ;
	pl_w->catch_cnt ++ ;
	first_atck_catch( pl_w ) ;
	pl_w->hit_data_adrs = &pl_w->catch_w->hit_data_tbl[pl_w->atck_no & ATCK_NO_MASK] ;
	catch_hit_set( pl_w ) ;
	if( pl_w->hit_data_adrs->damage_type & FLY_SCREEN ) pl_w->fly_screen_flag = 1 ;
	pl_w->pl_stance = STANCE_AIR ;
	if( pl_w->pl_type == PL_ZANGIEF && !( ( int (*)( PLAYER_WORK * ) )pl_w->pl_tbl[TBL_EX0] )( pl_w ) ) down_se_req( pl_w ) ;
	pos = pl_w->pos ;
	pos.y = pl_w->ground_y ;
	land_dust_set( &pos, pl_w->pl_no ) ;
	lv = pl_w->weight < 5 ? 1 : 3 ;
	scr_work.quake_level = lv ;
	scr_work.quake_flag = 1 ;
	pl_w->hit_stop = pl_w->catch_w->hit_stop = pl_w->hit_data_adrs->hit_stop ;
	catch_blow_spd_set( pl_w ) ;
	pl_w->acc.y = -1.2053571f ;
	pl_w->spd.x = pl_w->back_spd_x ;
	pl_w->spd.y = pl_w->back_spd_y ;
}

/*--------------------------------------------------------------*/
static void caught_slam_bound( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_catch_dir_set( pl_w ) ;
	pl_w->disp_flag = 1 ;
	pl_w->rot[2] = 0 ;
	pl_w->scale = pl_w->pl_scale ;
	pl_w->alpha = 1.0f ;
	dm_blow_char_set( pl_w ) ;
	if( pl_w->pos.y < pl_w->ground_y ) pl_w->pos.y = pl_w->ground_y ;
	pl_w->timer0 = 0 ;
	caught_slam_fly( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void caught_slam_fly( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->wall_flag && !pl_w->wall_hit_flag ){
		pl_w->wall_hit_flag = 1 ;
		down_se_req( pl_w ) ;
		wall_dust_set( pl_w ) ;
		if( pl_w->fly_screen_flag ){ }
	}
	if( !pl_w->timer0 && pl_w->spd.y <= 0.0f ){
		pl_w->timer0 = 1 ;
		dm_fall_char_set( pl_w ) ;
	}
	if( pl_land_ck( pl_w ) ){
		pl_w->nml_catch_ng_timer = 2 ;
		pl_w->escape_timer = 5 ;
		pl_w->routine_2 ++ ;
		pl_w->char_no.b.no = 27 ;
		if( pl_w->dm_no != 1 || !pl_w->vital_new ){
			pl_w->routine_2 ++ ;
			pl_w->char_no.b.no = 26 ;
			pl_w->pl_stance = STANCE_DOWN ;
		}
		pl_char_set( pl_w, Char_Damage, pl_w->char_no.b.no ) ;
	}
}

/*--------------------------------------------------------------*/
static void caught_slam_land( PLAYER_WORK *pl_w )
{
	pl_w->nml_catch_ng_timer = 2 ;
	if( pl_char_move( pl_w ) < 0 ) pl_act_set( pl_w, N_STAND ) ;
}

/*==============================================================*/
/*			As caught_blow_down										*/
/*==============================================================*/
static void caught_slam_down( PLAYER_WORK *pl_w )
{
	Sint8 lv ;

	pl_w->nml_catch_ng_timer = 2 ;
	if( pl_w->escape_timer < 0 || ( pl_w->vital_new > 0 && pl_w->escape_timer > 0 ) ){
	} else {
		pl_w->escape_timer = -1 ;
		land_se_req( pl_w ) ;
		land_dust_set( &pl_w->pos, pl_w->pl_no ) ;
		if( pl_w->dm_no != 1 ){
			lv = pl_w->weight < 5 ? 1 : 3 ;
			scr_work.quake_level = lv ;
			scr_work.quake_flag = 1 ;
		}
		if( pl_w->fly_screen_flag || !pl_w->vital_new || !pl_w->ukemi_flag ){
		} else {
			if( pl_w->cpu_flag ){
				if( em_rate_ck( pl_w, 29, 2 ) ) pl_w->ukemi_flag = -1 ;
				else pl_w->ukemi_flag = 0 ;
			}
			if( pl_w->ukemi_flag < 0 ){
				pl_w->jump_dir = 0 ;
				pl_act_set( pl_w, N_GET_UP ) ;
				return ;
			}
		}
	}
	if( pl_char_move( pl_w ) < 0 ) pl_act_set( pl_w, N_DOWN ) ;
}

/****************************************************************/
/*			Thrown straight down onto the ground				*/
/****************************************************************/
void caught_down( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->catch_cnt += 2 ;
		first_atck_catch( pl_w ) ;
	}
	pl_w->hit_data_adrs = &pl_w->catch_w->hit_data_tbl[pl_w->atck_no & ATCK_NO_MASK] ;
	catch_hit_set( pl_w ) ;
	pl_w->disp_dir = pl_w->pl_dir ;
	pl_w->pl_stance = STANCE_AIR ;
	down_se_req( pl_w ) ;
	pl_w->hit_stop = pl_w->catch_w->hit_stop = pl_w->hit_data_adrs->hit_stop ;
	pl_w->pos.y = pl_w->ground_y ;
	land_dust_set( &pl_w->pos, pl_w->pl_no ) ;
	pl_w->disp_flag = 1 ;
	pl_w->rot[2] = 0 ;
	pl_w->scale = pl_w->pl_scale ;
	pl_w->alpha = 1.0f ;
	pl_act_set( pl_w, N_DOWN ) ;
}

/****************************************************************/
/*			Let go into the hit state, flung by the throw's		*/
/*			hit													*/
/****************************************************************/
void caught_dm( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->catch_cnt += 2 ;
		first_atck_catch( pl_w ) ;
	}
	pl_w->hit_data_adrs = &pl_w->catch_w->hit_data_tbl[pl_w->atck_no & ATCK_NO_MASK] ;
	catch_hit_set( pl_w ) ;
	pl_w->recover_timer = pl_w->dm_recover_time ;
	pl_w->routine_1 = PL_DAMAGE ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	if( pl_w->vital_new ) pl_w->dm_no = 23 ;
	else {
		pl_w->recover_ng_flag = 1 ;
		pl_w->dm_no = 2 ;
	}
	pl_w->pl_dir = pl_w->disp_dir ;
	catch_blow_spd_set( pl_w ) ;
	pl_w->acc.y = -1.2053571f ;
	pl_w->pl_stance = STANCE_AIR ;
	if( pl_w->pos.y < pl_w->ground_y ) pl_w->pos.y = pl_w->ground_y ;
	pl_w->disp_flag = 1 ;
	pl_w->rot[2] = 0 ;
	pl_w->scale = pl_w->pl_scale ;
	pl_w->alpha = 1.0f ;
	pl_char_set( pl_w, Char_Damage, 0 ) ;
}

/****************************************************************/
/*			Held: the thrower moves this player; falls if it	*/
/*			is let go in the air								*/
/****************************************************************/
void caught_hold( PLAYER_WORK *pl_w )
{
	if( pl_w->catch_w->pl_type == PL_MAGNETO || pl_w->catch_w->pl_type == PL_SPIRAL ) return ;
	if( pl_w->catch_w->pl_type == PL_THANOS || pl_w->catch_w->pl_type == PL_TRON ) pl_char_move( pl_w ) ;
	pl_w->push_ng_timer = 0 ;
	if( pl_w->pl_stance == STANCE_AIR ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->pos.y <= pl_w->ground_y ){
			pl_w->pos.y = pl_w->ground_y ;
			pl_w->pl_stance = STANCE_STAND ;
		}
	}
}

/****************************************************************/
/*			Knocked away by the throw: a fixed blow up and		*/
/*			back												*/
/****************************************************************/
void caught_blow_fix( PLAYER_WORK *pl_w )
{
	static void ( *caught_07_jmp[4] )( PLAYER_WORK *pl_w ) = {
		caught_blow_fix_init, caught_blow_fix_fly, caught_blow_fix_down, caught_end,
	} ;

	caught_07_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void caught_blow_fix_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->catch_cnt ++ ;
	pl_w->hit_data_adrs = &pl_w->catch_w->hit_data_tbl[pl_w->atck_no & ATCK_NO_MASK] ;
	pl_w->recover_timer = 0 ;
	pl_w->dm_no = pl_w->vital_new > 0 ? 1 : 2 ;
	pl_w->dm_dir = 28 ;
	pl_w->dm_lv = 4 ;
	pl_w->dm_down_time = 0 ;
	pl_w->disp_dir = pl_w->pl_dir ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->disp_flag = 1 ;
	pl_w->rot[2] = 0 ;
	pl_w->scale = pl_w->pl_scale ;
	pl_w->alpha = 1.0f ;
	dm_blow_char_set( pl_w ) ;
	catch_blow_spd_fix( pl_w, pl_w->dm_lv ) ;
	pl_w->acc.y = -1.2053571f ;
	pl_w->spd.x = pl_w->back_spd_x ;
	pl_w->spd.y = pl_w->back_spd_y ;
	pl_w->timer0 = 0 ;
	caught_blow_fix_fly( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void caught_blow_fix_fly( PLAYER_WORK *pl_w )
{
	Sint8 lv ;

	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !pl_w->timer0 && pl_w->spd.y <= 0.0f ){
		pl_w->timer0 = 1 ;
		dm_fall_char_set( pl_w ) ;
	}
	if( pl_w->dm_no != 2 && -- pl_w->recover_timer < 0 && pl_w->vital_new ){
		pl_w->routine_2 = 3 ;
		pl_w->jump_atk_cnt = -1 ;
		pl_w->sjump_flag = 1 ;
		pl_char_set( pl_w, Char_Damage, 30 ) ;
		caught_end( pl_w ) ;
		return ;
	}
	if( pl_land_ck( pl_w ) ){
		land_se_req( pl_w ) ;
		land_dust_set( &pl_w->pos, pl_w->pl_no ) ;
		lv = pl_w->weight < 5 ? 1 : 3 ;
		scr_work.quake_level = lv ;
		scr_work.quake_flag = 1 ;
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_DOWN ;
		pl_char_set( pl_w, Char_Damage, 26 ) ;
	}
}

/*--------------------------------------------------------------*/
static void caught_blow_fix_down( PLAYER_WORK *pl_w )
{
	pl_w->nml_catch_ng_timer = 2 ;
	if( pl_char_move( pl_w ) < 0 ) pl_act_set( pl_w, N_DOWN ) ;
}

/****************************************************************/
/*			A throw that counts in the combo and goes			*/
/*			straight to the hit state							*/
/****************************************************************/
void caught_down_combo( PLAYER_WORK *pl_w )
{
	pl_w->hit_data_adrs = &pl_w->catch_w->hit_data_tbl[pl_w->atck_no & ATCK_NO_MASK] ;
	combo_hit( pl_w, pl_w->catch_w ) ;
	catch_hit_set( pl_w ) ;
	pl_w->disp_dir = pl_w->pl_dir ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->down_timer = 48 ;
	pl_w->push_ng_timer = 48 ;
	pl_w->renda_cnt = 100 ;
	pl_w->disp_flag = 1 ;
	pl_w->rot[2] = 0 ;
	pl_w->scale = pl_w->pl_scale ;
	pl_w->alpha = 1.0f ;
	pl_act_set( pl_w, N_DOWN ) ;
}

/****************************************************************/
/*			An elemental throw: burnt (9) or shocked, then		*/
/*			flung												*/
/****************************************************************/
void caught_elem( PLAYER_WORK *pl_w )
{
	static void ( *caught_09_jmp[4] )( PLAYER_WORK *pl_w ) = {
		caught_elem_init, caught_elem_fly, caught_elem_down, caught_end,
	} ;

	caught_09_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			Set up an elemental throw: the element by type,		*/
/*			its effect while the element has not built up		*/
/*==============================================================*/
static void caught_elem_init( PLAYER_WORK *pl_w )
{
	Uint8 elem ;

	pl_w->routine_2 ++ ;
	pl_w->catch_cnt ++ ;
	pl_w->wall_hit_flag = 0 ;
	first_atck_catch( pl_w ) ;
	pl_w->hit_data_adrs = &pl_w->catch_w->hit_data_tbl[pl_w->atck_no & ATCK_NO_MASK] ;
	catch_hit_set( pl_w ) ;
	if( pl_w->hit_data_adrs->damage_type & FLY_SCREEN ) pl_w->fly_screen_flag = 1 ;
	elem = pl_w->catch_type == 9 ? 2 : 3 ;
	if( ( pl_w->dm_elem = elem ) != pl_w->dm_elem_old ) pl_w->elem_ef_cnt = 0 ;
	if( pl_w->elem_ef_cnt < 4 ){
		if( elem == 2 ) dm00_03_set( pl_w ) ;
		else dm00_04_set( pl_w ) ;
	}
	pl_w->recover_timer = pl_w->dm_recover_time ;
	pl_w->disp_dir = pl_w->pl_dir ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->disp_flag = 1 ;
	pl_w->rot[2] = 0 ;
	pl_w->scale = pl_w->pl_scale ;
	pl_w->alpha = 1.0f ;
	dm_blow_char_set( pl_w ) ;
	catch_blow_spd_set( pl_w ) ;
	pl_w->acc.y = -1.2053571f ;
	pl_w->spd.x = pl_w->back_spd_x ;
	pl_w->spd.y = pl_w->back_spd_y ;
	pl_w->timer0 = 0 ;
	caught_elem_fly( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void caught_elem_fly( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !pl_w->wall_hit_flag && pl_w->wall_flag ){
		pl_w->wall_hit_flag = 1 ;
		down_se_req( pl_w ) ;
		wall_dust_set( pl_w ) ;
		if( pl_w->fly_screen_flag ){ }
	}
	if( !pl_w->timer0 && pl_w->spd.y <= 0.0f ){
		pl_w->timer0 = 1 ;
		dm_fall_char_set( pl_w ) ;
	}
	if( pl_w->dm_no != 2 && -- pl_w->recover_timer < 0 && pl_w->vital_new ){
		pl_w->routine_2 = 3 ;
		pl_w->jump_atk_cnt = -1 ;
		pl_w->sjump_flag = 1 ;
		pl_char_set( pl_w, Char_Damage, 30 ) ;
		caught_end( pl_w ) ;
	}
	else if( pl_w->spd.y <= 0.0f && pl_land_ck( pl_w ) ){
		pl_w->routine_2 ++ ;
		pl_w->nml_catch_ng_timer = 2 ;
		pl_w->escape_timer = 5 ;
		pl_w->pl_stance = STANCE_DOWN ;
		pl_char_set( pl_w, Char_Damage, 26 ) ;
	}
}

/*==============================================================*/
/*			As caught_blow_down, with a quake whatever the			*/
/*			reaction											*/
/*==============================================================*/
static void caught_elem_down( PLAYER_WORK *pl_w )
{
	Sint8 lv ;

	pl_w->nml_catch_ng_timer = 2 ;
	if( pl_w->escape_timer < 0 || ( pl_w->vital_new > 0 && pl_w->escape_timer > 0 ) ){
	} else {
		pl_w->escape_timer = -1 ;
		land_se_req( pl_w ) ;
		land_dust_set( &pl_w->pos, pl_w->pl_no ) ;
		lv = pl_w->weight < 5 ? 1 : 3 ;
		scr_work.quake_level = lv ;
		scr_work.quake_flag = 1 ;
		if( pl_w->fly_screen_flag || !pl_w->vital_new || !pl_w->ukemi_flag ){
		} else {
			if( pl_w->cpu_flag ){
				if( em_rate_ck( pl_w, 29, 2 ) ) pl_w->ukemi_flag = -1 ;
				else pl_w->ukemi_flag = 0 ;
			}
			if( pl_w->ukemi_flag < 0 ){
				pl_w->jump_dir = 0 ;
				pl_act_set( pl_w, N_GET_UP ) ;
				return ;
			}
		}
	}
	if( pl_char_move( pl_w ) < 0 ) pl_act_set( pl_w, N_DOWN ) ;
}

/****************************************************************/
/*			Slammed down hard: a bounce, then flung				*/
/****************************************************************/
void caught_slam_hard( PLAYER_WORK *pl_w )
{
	static void ( *caught_11_jmp[5] )( PLAYER_WORK *pl_w ) = {
		caught_slam_hard_init, caught_slam_hard_bound, caught_slam_hard_fly, caught_slam_hard_down, caught_end,
	} ;

	caught_11_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void caught_slam_hard_init( PLAYER_WORK *pl_w )
{
	VEC3 pos ;
	Sint8 lv ;

	pl_w->routine_2 ++ ;
	pl_w->catch_cnt ++ ;
	first_atck_catch( pl_w ) ;
	pl_w->hit_data_adrs = &pl_w->catch_w->hit_data_tbl[pl_w->atck_no & ATCK_NO_MASK] ;
	catch_hit_set( pl_w ) ;
	if( pl_w->hit_data_adrs->damage_type & FLY_SCREEN ) pl_w->fly_screen_flag = 1 ;
	pl_w->recover_timer = pl_w->dm_recover_time ;
	pl_w->pl_stance = STANCE_AIR ;
	if( pl_w->pl_type == PL_ZANGIEF && !( ( int (*)( PLAYER_WORK * ) )pl_w->pl_tbl[TBL_EX0] )( pl_w ) ) down_se_req( pl_w ) ;
	pos = pl_w->pos ;
	pos.y = pl_w->ground_y ;
	land_dust_set( &pos, pl_w->pl_no ) ;
	lv = pl_w->weight < 5 ? 1 : 3 ;
	scr_work.quake_level = lv ;
	scr_work.quake_flag = 1 ;
	pl_w->hit_stop = pl_w->catch_w->hit_stop = pl_w->hit_data_adrs->hit_stop ;
	catch_blow_spd_set( pl_w ) ;
	pl_w->acc.y = -1.2053571f ;
	pl_w->spd.x = pl_w->back_spd_x ;
	pl_w->spd.y = pl_w->back_spd_y ;
	pl_rot_ofs_set( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void caught_slam_hard_bound( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_catch_dir_set( pl_w ) ;
	pl_w->disp_flag = 1 ;
	pl_w->rot[2] = 0 ;
	pl_w->scale = pl_w->pl_scale ;
	pl_w->alpha = 1.0f ;
	dm_blow_char_set( pl_w ) ;
	if( pl_w->pos.y < pl_w->ground_y ) pl_w->pos.y = pl_w->ground_y ;
	pl_w->timer0 = 0 ;
	caught_slam_hard_fly( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void caught_slam_hard_fly( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->wall_flag && !pl_w->wall_hit_flag ){
		pl_w->wall_hit_flag = 1 ;
		down_se_req( pl_w ) ;
		wall_dust_set( pl_w ) ;
		if( pl_w->fly_screen_flag ){ }
	}
	if( !pl_w->timer0 && pl_w->spd.y <= 0.0f ){
		pl_w->timer0 = 1 ;
		dm_fall_char_set( pl_w ) ;
	}
	if( pl_w->dm_no != 2 && -- pl_w->recover_timer < 0 && pl_w->vital_new ){
		pl_w->routine_2 = 4 ;
		pl_w->jump_atk_cnt = -1 ;
		pl_w->sjump_flag = 1 ;
		pl_char_set( pl_w, Char_Damage, 30 ) ;
		caught_end( pl_w ) ;
	}
	else if( pl_w->spd.y <= 0.0f && pl_land_ck( pl_w ) ){
		pl_w->routine_2 ++ ;
		pl_w->nml_catch_ng_timer = 2 ;
		pl_w->escape_timer = 5 ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_char_set( pl_w, Char_Damage, 26 ) ;
	}
}

/*==============================================================*/
/*			As caught_blow_down										*/
/*==============================================================*/
static void caught_slam_hard_down( PLAYER_WORK *pl_w )
{
	Sint8 lv ;

	pl_w->nml_catch_ng_timer = 2 ;
	if( pl_w->escape_timer < 0 || ( pl_w->vital_new > 0 && pl_w->escape_timer > 0 ) ){
	} else {
		pl_w->escape_timer = -1 ;
		land_se_req( pl_w ) ;
		land_dust_set( &pl_w->pos, pl_w->pl_no ) ;
		if( pl_w->dm_no != 1 ){
			lv = pl_w->weight < 5 ? 1 : 3 ;
			scr_work.quake_level = lv ;
			scr_work.quake_flag = 1 ;
		}
		if( pl_w->fly_screen_flag || !pl_w->vital_new || !pl_w->ukemi_flag ){
		} else {
			if( pl_w->cpu_flag ){
				if( em_rate_ck( pl_w, 29, 2 ) ) pl_w->ukemi_flag = -1 ;
				else pl_w->ukemi_flag = 0 ;
			}
			if( pl_w->ukemi_flag < 0 ){
				pl_w->jump_dir = 0 ;
				pl_act_set( pl_w, N_GET_UP ) ;
				return ;
			}
		}
	}
	if( pl_char_move( pl_w ) < 0 ) pl_act_set( pl_w, N_DOWN ) ;
}

/****************************************************************/
/*			Held where the thrower leaves this player, for a	*/
/*			time or the throw's hold							*/
/****************************************************************/
void caught_wait( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 64 ;
		pl_w->recover_timer = pl_w->dm_recover_time ;
	}
	pl_w->push_ng_timer = 0 ;
	pl_w->recover_timer -- ;
	pl_w->timer0 -- ;
	if( !pl_w->recover_timer && !pl_w->timer0 ){
		if( pl_w->pl_stance != STANCE_AIR ) pl_stand_ret( pl_w ) ;
		else pl_jump_ret( pl_w ) ;
	}
}

/****************************************************************/
/*			Tossed up and dropped, then down as caught_blow		*/
/****************************************************************/
void caught_toss( PLAYER_WORK *pl_w )
{
	static void ( *caught_13_jmp[4] )( PLAYER_WORK *pl_w ) = {
		caught_toss_init, caught_toss_fly, caught_blow_down, caught_end,
	} ;

	caught_13_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void caught_toss_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->catch_cnt ++ ;
	first_atck_catch( pl_w ) ;
	pl_w->hit_data_adrs = &pl_w->catch_w->hit_data_tbl[pl_w->atck_no & ATCK_NO_MASK] ;
	catch_hit_set( pl_w ) ;
	pl_w->recover_timer = pl_w->dm_recover_time ;
	pl_w->disp_dir = pl_w->pl_dir ;
	pl_w->pl_stance = STANCE_AIR ;
	if( pl_w->atck_no & ATCK_DIR ){
		pl_w->dm_dir = pl_w->catch_w->dir ;
		if( pl_w->catch_w->disp_dir ) pl_w->dm_dir = ( 32 - pl_w->dm_dir ) & 31 ;
	}
	pl_w->disp_flag = 1 ;
	pl_w->rot[2] = 0 ;
	pl_w->scale = pl_w->pl_scale ;
	pl_w->alpha = 1.0f ;
	dm_blow_char_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = 2.142857f ;
	pl_w->acc.y = -0.016741071f ;
	pl_w->timer0 = 0 ;
	caught_toss_fly( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void caught_toss_fly( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !pl_w->timer0 && pl_w->spd.y <= 0.0f ){
		pl_w->timer0 = 1 ;
		dm_fall_char_set( pl_w ) ;
	}
	if( pl_w->dm_no != 2 && -- pl_w->recover_timer < 0 && pl_w->vital_new ){
		pl_w->routine_2 = 3 ;
		pl_w->jump_atk_cnt = -1 ;
		pl_w->sjump_flag = 1 ;
		pl_char_set( pl_w, Char_Damage, 30 ) ;
		caught_end( pl_w ) ;
	}
	else if( pl_w->spd.y <= 0.0f && pl_land_ck( pl_w ) ){
		pl_w->routine_2 ++ ;
		pl_w->nml_catch_ng_timer = 2 ;
		pl_w->escape_timer = 5 ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_char_set( pl_w, Char_Damage, 26 ) ;
	}
}

/****************************************************************/
/*			Flung burning: the colour stays until the player	*/
/*			hits a wall or the ground							*/
/****************************************************************/
void caught_burn( PLAYER_WORK *pl_w )
{
	static void ( *caught_14_jmp[3] )( PLAYER_WORK *pl_w ) = {
		caught_burn_init, caught_burn_fly, caught_burn_down,
	} ;

	caught_14_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void caught_burn_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->catch_cnt ++ ;
	pl_w->wall_hit_flag = 0 ;
	first_atck_catch( pl_w ) ;
	pl_w->hit_data_adrs = &pl_w->catch_w->hit_data_tbl[pl_w->atck_no & ATCK_NO_MASK] ;
	catch_hit_set( pl_w ) ;
	if( pl_w->hit_data_adrs->damage_type & FLY_SCREEN ) pl_w->fly_screen_flag = 1 ;
	pl_w->recover_timer = pl_w->dm_recover_time ;
	pl_w->disp_dir = pl_w->pl_dir ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->disp_flag = 1 ;
	pl_w->rot[2] = 0 ;
	pl_w->scale = pl_w->pl_scale ;
	pl_w->alpha = 1.0f ;
	dm_blow_char_set( pl_w ) ;
	catch_blow_spd_set( pl_w ) ;
	pl_w->spd.x = pl_w->back_spd_x ;
	pl_w->spd.y = pl_w->back_spd_y ;
	pl_w->acc.y = -1.2053571f ;
	pl_w->timer0 = 0 ;
	pl_w->timer1 = 320 ;
}

/*--------------------------------------------------------------*/
static void caught_burn_fly( PLAYER_WORK *pl_w )
{
	if( pl_w->timer1 > 0 ) pl_color_set( pl_w, 6 ) ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->wall_flag && !pl_w->wall_hit_flag ){
		burst_set( pl_w, &pl_w->pos, 1, 8 ) ;
		pl_w->timer1 = -1 ;
		pl_color_set( pl_w, 1 ) ;
		pl_w->wall_hit_flag = 1 ;
		down_se_req( pl_w ) ;
		wall_dust_set( pl_w ) ;
		if( pl_w->fly_screen_flag ){
			scr_work.quake_level = 3 ;
			scr_work.quake_flag = 1 ;
		}
	}
	if( !pl_w->timer0 && pl_w->spd.y <= 0.0f ){
		pl_w->timer0 = 1 ;
		dm_fall_char_set( pl_w ) ;
	}
	if( pl_w->spd.y <= 0.0f && pl_land_ck( pl_w ) ){
		if( !pl_w->wall_hit_flag ){
			burst_set( pl_w, &pl_w->pos, 1, 8 ) ;
			pl_w->timer1 = -1 ;
			pl_color_set( pl_w, 1 ) ;
		}
		pl_w->routine_2 ++ ;
		pl_w->nml_catch_ng_timer = 2 ;
		pl_w->escape_timer = 5 ;
		pl_w->pl_stance = STANCE_DOWN ;
		pl_char_set( pl_w, Char_Damage, 26 ) ;
	}
}

/*==============================================================*/
/*			As caught_blow_down										*/
/*==============================================================*/
static void caught_burn_down( PLAYER_WORK *pl_w )
{
	Sint8 lv ;

	pl_w->nml_catch_ng_timer = 2 ;
	if( pl_w->escape_timer < 0 || ( pl_w->vital_new > 0 && pl_w->escape_timer > 0 ) ){
	} else {
		pl_w->escape_timer = -1 ;
		land_se_req( pl_w ) ;
		land_dust_set( &pl_w->pos, pl_w->pl_no ) ;
		if( pl_w->dm_no != 1 ){
			lv = pl_w->weight < 5 ? 1 : 3 ;
			scr_work.quake_level = lv ;
			scr_work.quake_flag = 1 ;
		}
		if( pl_w->fly_screen_flag || !pl_w->vital_new || !pl_w->ukemi_flag ){
		} else {
			if( pl_w->cpu_flag ){
				if( em_rate_ck( pl_w, 29, 2 ) ) pl_w->ukemi_flag = -1 ;
				else pl_w->ukemi_flag = 0 ;
			}
			if( pl_w->ukemi_flag < 0 ){
				pl_w->jump_dir = 0 ;
				pl_act_set( pl_w, N_GET_UP ) ;
				return ;
			}
		}
	}
	if( pl_char_move( pl_w ) < 0 ) pl_act_set( pl_w, N_DOWN ) ;
}

/****************************************************************/
/*			Thrown spinning, by the knock-back's direction		*/
/****************************************************************/
void caught_spin( PLAYER_WORK *pl_w )
{
	static void ( *caught_15_jmp[4] )( PLAYER_WORK *pl_w ) = {
		caught_spin_init, caught_spin_fly, caught_spin_down, caught_end,
	} ;

	caught_15_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void caught_spin_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->catch_cnt ++ ;
	pl_w->wall_hit_flag = 0 ;
	first_atck_catch( pl_w ) ;
	pl_w->hit_data_adrs = &pl_w->catch_w->hit_data_tbl[pl_w->atck_no & ATCK_NO_MASK] ;
	catch_hit_set( pl_w ) ;
	if( pl_w->hit_data_adrs->damage_type & FLY_SCREEN ) pl_w->fly_screen_flag = 1 ;
	pl_w->recover_timer = pl_w->dm_recover_time ;
	pl_w->disp_dir = pl_w->pl_dir ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->dm_dir = ( pl_w->dm_dir + 8 ) & 16 ;
	pl_w->char_no.b.no = pl_w->dm_dir ? 29 : 28 ;
	pl_w->disp_flag = 1 ;
	pl_w->rot[2] = 0 ;
	pl_w->scale = pl_w->pl_scale ;
	pl_w->alpha = 1.0f ;
	pl_char_set( pl_w, Char_Damage, pl_w->char_no.b.no ) ;
	catch_blow_spd_set( pl_w ) ;
	pl_w->spd.x = pl_w->back_spd_x ;
	pl_w->spd.y = pl_w->back_spd_y ;
	pl_w->acc.y = -1.2053571f ;
	pl_w->timer0 = pl_w->spd.y <= 0.0f ? 1 : 0 ;
	caught_spin_fly( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void caught_spin_fly( PLAYER_WORK *pl_w )
{
	Sint8 lv ;

	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->wall_flag && !pl_w->wall_hit_flag ){
		pl_w->wall_hit_flag = 1 ;
		down_se_req( pl_w ) ;
		wall_dust_set( pl_w ) ;
		if( pl_w->fly_screen_flag ){
			lv = pl_w->weight < 5 ? 1 : 3 ;
			scr_work.quake_level = lv ;
			scr_work.quake_flag = 1 ;
		}
	}
	if( !pl_w->timer0 && pl_w->spd.y <= 0.0f ){
		pl_char_set( pl_w, Char_Damage, 29 ) ;
		pl_w->timer0 = 1 ;
	}
	if( pl_w->dm_no != 2 && -- pl_w->recover_timer < 0 && pl_w->vital_new ){
		pl_w->routine_2 = 3 ;
		pl_w->jump_atk_cnt = -1 ;
		pl_w->sjump_flag = 1 ;
		pl_char_set( pl_w, Char_Damage, 30 ) ;
		caught_end( pl_w ) ;
	}
	else if( pl_w->spd.y <= 0.0f && pl_land_ck( pl_w ) ){
		pl_w->nml_catch_ng_timer = 2 ;
		pl_w->escape_timer = 5 ;
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_DOWN ;
		pl_char_set( pl_w, Char_Damage, 26 ) ;
	}
}

/*==============================================================*/
/*			As caught_blow_down										*/
/*==============================================================*/
static void caught_spin_down( PLAYER_WORK *pl_w )
{
	Sint8 lv ;

	pl_w->nml_catch_ng_timer = 2 ;
	if( pl_w->escape_timer < 0 || ( pl_w->vital_new > 0 && pl_w->escape_timer > 0 ) ){
	} else {
		pl_w->escape_timer = -1 ;
		land_se_req( pl_w ) ;
		land_dust_set( &pl_w->pos, pl_w->pl_no ) ;
		if( pl_w->dm_no != 1 ){
			lv = pl_w->weight < 5 ? 1 : 3 ;
			scr_work.quake_level = lv ;
			scr_work.quake_flag = 1 ;
		}
		if( pl_w->fly_screen_flag || !pl_w->vital_new || !pl_w->ukemi_flag ){
		} else {
			if( pl_w->cpu_flag ){
				if( em_rate_ck( pl_w, 29, 2 ) ) pl_w->ukemi_flag = -1 ;
				else pl_w->ukemi_flag = 0 ;
			}
			if( pl_w->ukemi_flag < 0 ){
				pl_w->jump_dir = 0 ;
				pl_act_set( pl_w, N_GET_UP ) ;
				return ;
			}
		}
	}
	if( pl_char_move( pl_w ) < 0 ) pl_act_set( pl_w, N_DOWN ) ;
}

/****************************************************************/
/*			Shocked in place for a few beats, then flung		*/
/****************************************************************/
void caught_shock( PLAYER_WORK *pl_w )
{
	static void ( *caught_19_jmp[5] )( PLAYER_WORK *pl_w ) = {
		caught_shock_init, caught_shock_wait, caught_shock_fly, caught_shock_down, caught_end,
	} ;

	caught_19_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void caught_shock_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->disp_dir = pl_w->pl_dir ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->timer0 = 7 ;
	pl_w->timer1 = 0 ;
	pl_w->disp_flag = 1 ;
	pl_w->rot[2] = 0 ;
	pl_w->scale = pl_w->pl_scale ;
	pl_w->alpha = 1.0f ;
	pl_char_set( pl_w, Char_Damage, 16 ) ;
	caught_shock_wait( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void caught_shock_wait( PLAYER_WORK *pl_w )
{
	pl_w->catch_ng_timer = 2 ;
	pl_frame_jump( pl_w ) ;
	if( -- pl_w->timer1 >= 0 ) return ;
	if( pl_w->timer0 ){
		pls38_02_set( pl_w, pl_w->timer0 & 3 ) ;
		pl_w->timer1 = 2 ;
		if( !-- pl_w->timer0 ) pl_w->timer1 = 10 ;
	}
	else {
	pl_w->routine_2 ++ ;
	pl_w->hit_data_adrs = &pl_w->catch_w->hit_data_tbl[pl_w->atck_no & ATCK_NO_MASK] ;
	catch_hit_set2( pl_w ) ;
	if( pl_w->hit_data_adrs->damage_type & FLY_SCREEN ) pl_w->fly_screen_flag = 1 ;
	pl_w->recover_timer = pl_w->dm_recover_time ;
	pl_w->disp_dir = pl_w->pl_dir ;
	dm_blow_char_set( pl_w ) ;
	catch_blow_spd_set( pl_w ) ;
	pl_w->acc.y = -1.2053571f ;
	pl_w->spd.x = pl_w->back_spd_x ;
	pl_w->spd.y = pl_w->back_spd_y ;
	pl_w->timer0 = 0 ;
	caught_shock_fly( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void caught_shock_fly( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->wall_flag && !pl_w->wall_hit_flag ){
		pl_w->wall_hit_flag = 1 ;
		down_se_req( pl_w ) ;
		wall_dust_set( pl_w ) ;
		if( pl_w->fly_screen_flag ){ }
	}
	if( !pl_w->timer0 && pl_w->spd.y <= 0.0f ){
		pl_w->timer0 = 1 ;
		dm_fall_char_set( pl_w ) ;
	}
	if( pl_w->dm_no != 2 && -- pl_w->recover_timer < 0 && pl_w->vital_new ){
		pl_w->routine_2 = 4 ;
		pl_w->jump_atk_cnt = -1 ;
		pl_w->sjump_flag = 1 ;
		pl_char_set( pl_w, Char_Damage, 30 ) ;
		caught_end( pl_w ) ;
	}
	else if( pl_w->spd.y <= 0.0f && pl_land_ck( pl_w ) ){
		pl_w->nml_catch_ng_timer = 2 ;
		pl_w->escape_timer = 5 ;
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_DOWN ;
		pl_char_set( pl_w, Char_Damage, 26 ) ;
	}
}

/*==============================================================*/
/*			As caught_blow_down										*/
/*==============================================================*/
static void caught_shock_down( PLAYER_WORK *pl_w )
{
	Sint8 lv ;

	pl_w->nml_catch_ng_timer = 2 ;
	if( pl_w->escape_timer < 0 || ( pl_w->vital_new > 0 && pl_w->escape_timer > 0 ) ){
	} else {
		pl_w->escape_timer = -1 ;
		land_se_req( pl_w ) ;
		land_dust_set( &pl_w->pos, pl_w->pl_no ) ;
		if( pl_w->dm_no != 1 ){
			lv = pl_w->weight < 5 ? 1 : 3 ;
			scr_work.quake_level = lv ;
			scr_work.quake_flag = 1 ;
		}
		if( pl_w->fly_screen_flag || !pl_w->vital_new || !pl_w->ukemi_flag ){
		} else {
			if( pl_w->cpu_flag ){
				if( em_rate_ck( pl_w, 29, 2 ) ) pl_w->ukemi_flag = -1 ;
				else pl_w->ukemi_flag = 0 ;
			}
			if( pl_w->ukemi_flag < 0 ){
				pl_w->jump_dir = 0 ;
				pl_act_set( pl_w, N_GET_UP ) ;
				return ;
			}
		}
	}
	if( pl_char_move( pl_w ) < 0 ) pl_act_set( pl_w, N_DOWN ) ;
}

/****************************************************************/
/*			Thrown high from above the thrower, and dropped		*/
/*			onto the ground										*/
/****************************************************************/
void caught_drop( PLAYER_WORK *pl_w )
{
	static void ( *caught_20_jmp[3] )( PLAYER_WORK *pl_w ) = {
		caught_drop_init, caught_drop_wait, caught_drop_fall,
	} ;

	caught_20_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void caught_drop_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->catch_cnt ++ ;
	first_atck_catch( pl_w ) ;
	pl_w->disp_flag = 1 ;
	pl_w->rot[2] = 0 ;
	pl_w->scale = pl_w->pl_scale ;
	pl_w->alpha = 1.0f ;
	pl_char_set( pl_w, Char_Damage, 29 ) ;
	pl_w->spd.y = -DOT_Y( 4.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->timer0 = 20 ;
	pl_w->pos = pl_w->catch_w->pos ;
	pl_w->pos.y += DOT_Y( 384.0f ) ;
	if( pl_w->catch_w->pl_dir ) pl_w->pos.x += DOT_X( 128.0f ) ;
	else pl_w->pos.x -= DOT_X( 128.0f ) ;
	caught_drop_wait( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void caught_drop_wait( PLAYER_WORK *pl_w )
{
	if( -- pl_w->timer0 < 0 ) pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void caught_drop_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_land_ck( pl_w ) ){
		pl_w->hit_data_adrs = &pl_w->catch_w->hit_data_tbl[pl_w->atck_no & ATCK_NO_MASK] ;
		catch_hit_set( pl_w ) ;
		pl_w->disp_dir = pl_w->pl_dir ;
		down_se_req( pl_w ) ;
		land_dust_set( &pl_w->pos, pl_w->pl_no ) ;
		scr_work.quake_level = 3 ;
		scr_work.quake_flag = 1 ;
		pl_w->pl_stance = STANCE_DOWN ;
		pl_w->down_timer = pl_w->push_ng_timer = 64 ;
		pl_w->renda_cnt = 100 ;
		pl_act_set( pl_w, N_DOWN ) ;
	}
}

/****************************************************************/
/*			The two top states that do nothing					*/
/****************************************************************/
void pl_move_2( PLAYER_WORK *pl_w )
{
}

/****************************************************************/
void pl_move_3( PLAYER_WORK *pl_w )
{
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pl_main.c" End of File	*/

/* The player's command checks: dashes and super jumps, the team moves (assists, tags, team hyper
   combos, counters and snapbacks), the taunt, and the readers for motion, charge and button
   commands. */
#include "player.h"
#include "pl_act.h"

/* each character's least height for an air dash, in 256ths of 2.142857 */
extern Sint16 air_dash_y_tbl[];

/* a command's move number, read as the low byte of com[2] */
#define COM_NO( com )	( ( Uint8 * )&( com )[2] )[0]

static Uint8 dash_cancel_ck( PLAYER_WORK *pl_w );
static Uint8 sjump_ukemi_ck( PLAYER_WORK *pl_w );
static Uint8 cpu_partner_sel( PLAYER_WORK *pl_w );
static Uint8 cpu_snapback_sel( PLAYER_WORK *pl_w );
static Uint8 com_nml_start( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw );
static Uint8 com_nml_lever( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw );
static Uint8 com_nml_sw( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw );
static Uint8 com_nml_sw_all( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw );
static Uint8 com_nml_04( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw );
static Uint8 com_tame_start( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw );
static Uint8 com_tame_hold( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw );
static Uint8 com_tame_release( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw );
static Uint8 com_tame_lever( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw );
static Uint8 com_tame_sw( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw );
static Uint8 com_renda_start( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw );
static Uint8 com_renda_count( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw );
static Uint8 com_renda_end( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw );
static Uint8 com_rot_start( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw );
static Uint8 com_rot_dir( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw );
static Uint8 com_rot_turn( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw );
static Uint8 com_seq_start( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw );
static Uint8 com_seq_next( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw );
static Uint8 com_seq_sw( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw );
static Uint8 com_hold_start( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw, Uint16 sw );
static Uint8 com_hold_wait( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw, Uint16 sw );
static Uint8 com_hold_release( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw, Uint16 sw );
static Uint8 com_hold_retry( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw, Uint16 sw );
static Uint8 com_sw_ok( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw, Uint16 sw );
static Uint8 com_ok( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw, Uint16 sw );
static Uint8 sp_ok_cut_ck( PLAYER_WORK *pl_w );
static void com_timer_set( PLAYER_WORK *pl_w, COM_WORK *cw );
static int cpu_renda_ck( PLAYER_WORK *pl_w );

/****************************************************************/
Uint8 pl_dash_ck( PLAYER_WORK *pl_w )
{
	COM_WORK *cw = &pl_w->dash_w ;
	Uint16 sw ;

	cw->ok_flag = 0 ;
	if( pl_w->act_st == N_GUARD || pl_w->act_st == N_AIR_GUARD ){
		if( !dash_cancel_ck( pl_w ) ) return 0 ;
	}
	if( pl_w->target_w->fly_screen_flag ) return ;	/* no value, as in the arcade build */
	if( sw_now_ck( pl_w, 0x0300, &sw, 0 ) ){
		cw->sw = pl_w->pl_sw_new ;
	} else {
		if( !cw->step ){
			sw = pl_w->pl_sw_now ;
			sw &= ( KEY_FORWARD | KEY_BACK ) ;
			if( !sw ) return 0 ;
			if( sw != pl_w->pl_sw_now ) return 0 ;
			cw->step = 1 ;
			cw->sw = sw ;
			cw->timer = 15 ;
			return 0 ;
		}
		if( -- cw->timer < 0 ){
			cw->step = 0 ;
			return 0 ;
		}
		sw = pl_w->pl_sw_now ;
		sw &= ( KEY_FORWARD | KEY_BACK ) ;
		if( !sw ) return 0 ;
		if( sw != pl_w->pl_sw_now ) return 0 ;
		if( sw != cw->sw ) return 0 ;
		if( pl_w->pl_stance == STANCE_CROUCH ) return 0 ;
	}
	cw->step = 0 ;
	cw->ok_flag = 1 ;
	cw->dir = ( cw->sw & 0x0400 ) >> 10 ;
	return 1 ;
}

/****************************************************************/
Uint8 pl_dash_set( PLAYER_WORK *pl_w )
{
	COM_WORK *cw ;
	Uint8 act ;

	if( ABYSS_CK( pl_w ) ) return 0 ;
	cw = &pl_w->dash_w ;
	if( cw->ok_flag != 1 ) return 0 ;
	cw->ok_flag = 0 ;
	if( !cw->dir ){
		act = 10 ;
	} else {
		if( pl_w->pl_type == PL_ZANGIEF ) return 0 ;
		act = 11 ;
	}
	if( pl_w->act_st != act ) return pl_act_set( pl_w, act ) ;
	return 0 ;
}

/****************************************************************/
Uint8 pl_air_dash_ck( PLAYER_WORK *pl_w, COM_WORK *cw )
{
	Uint16 sw ;
	Uint16 lever ;
	Float y ;

	if( pl_w->target_w->fly_screen_flag ) return 0 ;
	y = pl_w->ground_y + air_dash_y_tbl[pl_w->pl_type] * 2.142857f / 256.0f ;
	if( pl_w->pos.y < y ) return 0 ;
	if( sw_now_ck( pl_w, 0x0300, &sw, 0 ) ){
		cw->sw = pl_w->pl_sw_new ;
	} else {
		sw = 0x0800 ;
		if( DASH8_CK( pl_w ) ) sw = 0x3c00 ;
		if( DASH2_CK( pl_w ) ) sw = 0x0c00 ;
		if( !cw->step ){
			if( cw->sw = pl_w->pl_sw_now & sw ){
				cw->timer = 15 ;
				cw->step ++ ;
			}
			return 0 ;
		}
		if( -- cw->timer <= 0 ) return cw->step = 0 ;
		if( !( lever = pl_w->pl_sw_now & sw ) ) return 0 ;
		if( lever != cw->sw ) return 0 ;
	}
	cw->step = 0 ;
	if( pl_w->pl_stance != STANCE_AIR ) return 0 ;
	if( pl_w->routine_1 ) return 0 ;
	if( pl_w->cg_atck_flag & CG_ATCK_ALL ) return 0 ;
	if( pl_w->jump_dash_cnt ) return 0 ;
	if( pl_w->act_st == N_AIR_DASH ) return 0 ;
	if( pl_w->act_st == N_AIR_GUARD ) return 0 ;
	if( pl_w->act_st == N_SPECIAL ) return 0 ;
	if( pl_w->act_st == N_SUPER ) return 0 ;
	if( pl_w->act_st == N_ESCAPE ) return 0 ;
	if( pl_w->act_st == N_CHANGE ) return 0 ;
	if( !pl_w->fly_flag ) pl_w->jump_dash_cnt ++ ;
	sw = 0x0800 ;
	if( DASH8_CK( pl_w ) ) sw = 0x3c00 ;
	if( DASH2_CK( pl_w ) ) sw = 0x0c00 ;
	pl_w->dir = ( cw->sw & sw ) >> 10 ;
	pl_act_set( pl_w, N_AIR_DASH ) ;
}

/****************************************************************/
Uint8 pl_air_dash_sw_ck( PLAYER_WORK *pl_w )
{
	Uint16 sw ;
	Float y ;

	if( pl_w->target_w->fly_screen_flag ) return 0 ;
	if( pl_w->pl_stance != STANCE_AIR ) return 0 ;
	if( pl_w->routine_1 != PL_NORM ) return 0 ;
	if( pl_w->jump_dash_cnt ) return 0 ;
	if( pl_w->act_st == N_AIR_DASH ) return 0 ;
	if( pl_w->act_st == N_AIR_GUARD ) return 0 ;
	if( pl_w->act_st == N_ESCAPE ) return 0 ;
	if( !pl_w->fly_flag ){
		y = pl_w->ground_y + air_dash_y_tbl[pl_w->pl_type] * 2.142857f / 256.0f ;
		if( pl_w->pos.y < y ) return 0 ;
	}
	if( !sw_now_ck( pl_w, 0x0300, &sw, 0 ) ) return 0 ;
	if( ( pl_w->cg_atck_flag & CG_ATCK_ALL ) && pl_w->fly_flag ) return 0 ;
	if( !sp_ok_ck( pl_w ) ) return 0 ;
	pl_w->dir = ( pl_w->pl_sw_new & KEY_LEVER ) >> 10 ;
	if( !pl_w->fly_flag ) pl_w->jump_dash_cnt ++ ;
	pl_act_set( pl_w, N_AIR_DASH ) ;
}

/*==============================================================*/
/*			During a dash: both punches again end it, unless	*/
/*			guard_back_flag lets the dash go on							*/
/*==============================================================*/
static Uint8 dash_cancel_ck( PLAYER_WORK *pl_w )
{
	Uint16 sw ;

	if( pl_w->guard_back_flag > 0 ){
		if( !sw_now_ck( pl_w, 0x0300, &sw, 0 ) ) return 1 ;
		pl_w->guard_back_flag = -1 ;
	}
	memset( &pl_w->dash_w, 0, sizeof( COM_WORK ) ) ;
	return 0 ;
}

/****************************************************************/
Uint8 pl_sjump_ck( PLAYER_WORK *pl_w )
{
	COM_WORK *cw = &pl_w->sjump_w ;

	if( pl_w->routine_1 != PL_NORM ) return sjump_ukemi_ck( pl_w ) ;
	if( !pl_w->hit_stop ) cw->ok_flag = 0 ;
	if( !cw->step ){
		if( pl_w->pl_sw_now & KEY_DOWN ){
			cw->step = 1 ;
			cw->timer = 15 ;
		}
		return 0 ;
	}
	if( -- cw->timer < 0 ){
		cw->step = 0 ;
		return 0 ;
	}
	if( !( pl_w->pl_sw_now & KEY_UP ) ) return 0 ;
	cw->step = 0 ;
	cw->ok_flag = 2 ;
	return 1 ;
}

/* the throw escape: back, down-back, down and a button */
static Uint16 com_ukemi[] = { 3, COM_FREE, 0, 0, KEY_BACK, KEY_DOWN | KEY_BACK, KEY_DOWN, KEY_ATCK } ;

/*==============================================================*/
static Uint8 sjump_ukemi_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->ukemi_flag > 0 ){
		if( !com_nml_ck( pl_w, com_ukemi, &pl_w->sjump_w ) ) return 0 ;
		pl_w->ukemi_flag = -1 ;
		com_sw_set( pl_w, &pl_w->sjump_w ) ;
	}
	memset( &pl_w->sjump_w, 0, sizeof( COM_WORK ) ) ;
	return 0 ;
}

/****************************************************************/
Uint8 pl_sjump_set( PLAYER_WORK *pl_w )
{
	COM_WORK *cw = &pl_w->sjump_w ;
	HIT_DATA *hd ;
	PLAYER_WORK *em ;

	if( cw->ok_flag != 2 ){
		if( ( Sint8 )( pl_w->cg_atck_flag & CG_ATCK_ALL ) >= 0 ) return 0 ;
		if( !pl_w->hit_flag ) return 0 ;
		if( pl_w->hit_flag & HR_GUARD ) return 0 ;
		if( !( pl_w->pl_sw_new & KEY_UP ) ) return 0 ;
		if( 1 ) hd = &pl_w->hit_data_tbl[pl_w->atck_no & ATCK_NO_MASK] ;
		else hd = &pl_w->hit_data_tbl[pl_w->atck_no & ATCK_NO_MASK] ;
		if( !( hd->hit_flag & BLACK_COMBO ) ){
			if( hd->power <= 10 ) return 0 ;
			if( ( ( ( Uint8 )hd->vector + 3 ) & 31 ) > 5 ) return 0 ;
			em = pl_w->em_w ;
			if( em->work_kind ) em = pl_w->target_w ;
			if( em->pl_stance != STANCE_AIR ) return 0 ;
		}
	}
	if( vs_sys_work.vs_st < VS_FIGHT ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR ) return 0 ;
	if( pl_w->cg_atck_flag & CG_ATCK_ALL ){
		if( ( pl_w->cg_atck_flag & CG_ATCK_ALL ) != CG_ATCK_ON ) return 0 ;
		if( !pl_w->hit_flag || ( pl_w->hit_flag & HR_GUARD ) ) return 0 ;
	}
	if( pl_w->target_w->fly_screen_flag ) return 0 ;
	cw->ok_flag = 0 ;
	return pl_act_set( pl_w, N_SJUMP ) ;
}

/****************************************************************/
Uint8 pl_v_combi_ck( PLAYER_WORK *pl_w )
{
	Uint16 sw ;
	PLAYER_WORK *mw ;
	int lvl ;
	int i, n ;
	Uint8 bit ;

	if( pl_w->cpu_flag && pl_w->cpu_sp_flag && pl_w->cpu_sp_no == 126 ){
	} else if( !sw_on_ck( pl_w, 0x0090, &sw ) ) return 0 ;
	if( !sp_ok_ck( pl_w ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR ) return 0 ;
	if( vs_sys_work.vs_st >= VS_END_WAIT ) return 0 ;
	if( vs_sys_work.be_cnt[pl_w->pl_no] > 1 ) return 0 ;
	if( !( lvl = *pl_w->gauge_level ) ) return 0 ;
	if( pl_w->team_ng_timer ) return 0 ;
	if( lvl > 3 ) lvl = 3 ;
	n = 1 ;
	bit = 1 << pl_w->work_no / 2 ;
	i = 1 ;
	while( i < 3 && n < lvl ){
		mw = vs_sys_w->team_w[pl_w->pl_no][i] ;
		if( mw->vital_new > 0 && !mw->vital_red_wait ){
			n ++ ;
			bit |= 1 << mw->work_no / 2 ;
		}
		i ++ ;
	}
	pl_w->v_combi_cnt = n ;
	pl_w->hyper_team_bit = bit ;
	pl_w->change_req = 4 ;
	return 1 ;
}

/* a partner called in: 1 and 2 tag partner 1 or 2 in, 3 and 4 call them as an assist */
/*==============================================================*/
static int partner_ck( PLAYER_WORK *pl_w, Uint8 no )
{
	Uint8 idx = no ;
	Sint8 *cnt ;
	Uint16 sw ;
	Uint8 flag ;

	if( no == 1 ){
		cnt = &pl_w->change_timer[0] ;
		sw = 0x0080 ;
	} else if( no == 2 ){
		cnt = &pl_w->change_timer[1] ;
		sw = 0x0010 ;
	} else {
		if( no == 3 ){
			cnt = &pl_w->assist_timer[0] ;
			sw = 0x0080 ;
			idx -= 2 ;
		} else {
			cnt = &pl_w->assist_timer[1] ;
			sw = 0x0010 ;
			idx -= 2 ;
		}
	}
	if( pl_w->cpu_flag ) goto cpu ;
	if( !*cnt ){
		if( no < 3 ){
			if( ( pl_w->pl_now_lvbt & sw ) && !( pl_w->pl_sw_new & ( KEY_FORWARD | KEY_BACK ) ) ) *cnt = 16 ;
		} else {
			if( pl_w->pl_now_lvbt & sw ) *cnt = 16 ;
		}
		return 0 ;
	}
	else {
		if( -- *cnt ){
			if( pl_w->pl_new_lvbt & sw ) return 0 ;
			goto cpu ;
		}
		return 0 ;
	}
cpu :
	if( pl_w->routine_1 ) return 0 ;
	if( no <= 2 ){
		if( pl_w->pl_stance == STANCE_DOWN || pl_w->pl_stance == STANCE_AIR ) return 0 ;
	} else {
		if( pl_w->pl_stance == STANCE_DOWN || pl_w->pl_stance == STANCE_AIR && pl_w->sjump_flag ) return 0 ;
		if( pl_w->act_st == N_NML_ATCK && pl_w->atck_type == 2 ) return 0 ;
	}
	if( 0 ) flag = 0 ;
	flag = vs_sys_w->team_w[pl_w->pl_no ^ 1][0]->assist_dm_cnt ;
	flag |= vs_sys_w->team_w[pl_w->pl_no ^ 1][1]->assist_dm_cnt ;
	flag |= vs_sys_w->team_w[pl_w->pl_no ^ 1][2]->assist_dm_cnt ;
	if( flag ) return 0 ;
	if( vs_sys_work.vs_st >= VS_END_WAIT ) return 0 ;
	if( vs_sys_work.be_cnt[pl_w->pl_no] > 1 ) return 0 ;
	if( 0 ) goto ng ;
	if( pl_w->team_ng_timer ) return 0 ;
	if( vs_sys_w->team_w[pl_w->pl_no][idx]->be_flag ) return 0 ;
	if( vs_sys_w->team_w[pl_w->pl_no][idx]->vital_red_wait ) return 0 ;
	if( !sp_ok_ck( pl_w ) ) return 0 ;
	else if( !vs_sys_w->team_w[pl_w->pl_no][idx]->vital_new ){
		no = ( ( no - 1 ) ^ 1 ) + 1 ;
		idx = ( ( idx - 1 ) ^ 1 ) + 1 ;
		if( vs_sys_w->team_w[pl_w->pl_no][idx]->be_flag ) return 0 ;
		if( vs_sys_w->team_w[pl_w->pl_no][idx]->vital_red_wait ) return 0 ;
		if( !vs_sys_w->team_w[pl_w->pl_no][idx]->vital_new ) goto ng ;
	}
	if( no < 3 ){
		pl_w->partner_no = idx ;
		act_st_set( pl_w, N_CHANGE ) ;
		pl_w->routine_2 = pl_w->routine_3 = 0 ;
		if( pl_w->pl_stance != STANCE_AIR ) pl_w->change_type = 0 ;
		else pl_w->change_type = 1 ;
	} else {
		pl_w->partner_no = idx ;
		pl_w->delay_change_flag = -1 ;
	}
	*cnt = 0 ;
	return 1 ;
ng:
	return 0 ;
}

/****************************************************************/
Uint8 pl_change_ck( PLAYER_WORK *pl_w )
{
	Uint16 sw ;
	Uint8 no ;

	if( pl_w->pl_stance == STANCE_AIR || pl_w->pl_stance == STANCE_DOWN ) return 0 ;
	if( vs_sys_w->vs_st >= VS_END_WAIT ) return 0 ;
	if( vs_sys_w->be_cnt[pl_w->pl_no] > 1 ) return 0 ;
	if( pl_w->team_ng_timer ) return 0 ;
	if( pl_w->cpu_flag ){
		if( !pl_w->cpu_sp_flag ) return 0 ;
		if( pl_w->cpu_sp_no != 122 ) return 0 ;
		no = cpu_partner_sel( pl_w ) ;
	} else {
		no = 1 ;
		if( !sw_now_ck( pl_w, 0x0240, &sw, 0 ) ){
			no = 2 ;
			if( !sw_now_ck( pl_w, 0x0120, &sw, 0 ) ) return 0 ;
		}
	}
	if( !sp_ok_ck( pl_w ) ) return 0 ;
	if( vs_sys_w->team_w[pl_w->pl_no][no]->vital_red_wait ) return 0 ;
	if( !vs_sys_w->team_w[pl_w->pl_no][no]->vital_new ){
		no ^= 3 ;
		if( vs_sys_w->team_w[pl_w->pl_no][no]->vital_red_wait ) return 0 ;
		if( !vs_sys_w->team_w[pl_w->pl_no][no]->vital_new ) return 0 ;
	}
	pl_w->partner_no = no ;
	act_st_set( pl_w, N_CHANGE ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->change_type = 0 ;
	return 1 ;
}

/****************************************************************/
Uint8 pl_assist_ck( PLAYER_WORK *pl_w )
{
	Uint8 no ;

	if( pl_w->cpu_flag ){
		if( pl_w->cpu_sp_flag && pl_w->cpu_sp_no == 123 ){
			no = cpu_partner_sel( pl_w ) ;
			if( partner_ck( pl_w, no + 2 ) ) return 1 ;
		}
		return 0 ;
	}
	if( partner_ck( pl_w, 3 ) || partner_ck( pl_w, 4 ) ) return 1 ;
	return 0 ;
}

/* the variable counter: back, down-back, down and both assist buttons while guarding */
static Uint16 com_vcounter[] = { 3, COM_GROUND | COM_CUT, 0x007d, 0, KEY_BACK, KEY_DOWN | KEY_BACK, KEY_DOWN, KEY_ASSIST } ;

/*==============================================================*/
static int vcounter_ck( PLAYER_WORK *pl_w, int no )
{
	Uint8 tm ;

	if( pl_w->act_st != N_GUARD ) return 0 ;
	if( vs_sys_work.vs_st >= VS_END_WAIT ) return 0 ;
	if( vs_sys_work.team_w[pl_w->pl_no][no]->vital_red_wait ) return 0 ;
	if( vs_sys_work.be_cnt[pl_w->pl_no] != 1 ) return 0 ;
	if( !com_nml_ck( pl_w, com_vcounter, &pl_w->vcounter_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( !pl_w->guard_timer ) return 0 ;
	if( pl_w->cpu_flag ) tm = cpu_partner_sel( pl_w ) ;
	else if( pl_w->vcounter_w.sw & 0x0080 ) tm = 1 ;
	else tm = 2 ;
	if( !vs_sys_work.team_w[pl_w->pl_no][tm]->vital_new ){
		tm ^= 3 ;
		if( vs_sys_work.team_w[pl_w->pl_no][tm]->vital_red_wait ) return 0 ;
		if( !vs_sys_work.team_w[pl_w->pl_no][tm]->vital_new ) return 0 ;
	}
	pl_w->hit_stop = 0 ;
	pl_w->partner_no = tm ;
	pl_w->change_type = 4 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_CHANGE ) ;
	score_add( pl_w, 0, 0x00700000 ) ;
	return 1 ;
}

/****************************************************************/
Uint8 pl_vcounter_ck( PLAYER_WORK *pl_w )
{
	if( vcounter_ck( pl_w, 1 ) ) return 1 ;
	return 0 ;
}

/* the snapback: down, down-forward, forward and an assist button */
static Uint16 com_snapback[] = { 3, COM_GROUND, 0x007c, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_ASSIST } ;

/****************************************************************/
int pl_snapback_ck( PLAYER_WORK *pl_w )
{
	Uint8 no ;

	if( !com_nml_ck( pl_w, com_snapback, &pl_w->snapback_w ) ) return 0 ;
	if( pl_w->pl_stance != STANCE_STAND && pl_w->pl_stance != STANCE_CROUCH ) return 0 ;
	if( pl_w->cpu_flag ) no = cpu_snapback_sel( pl_w ) ;
	else if( pl_w->snapback_w.sw & 0x0080 ) no = 1 ;
	else no = 2 ;
	pl_w->atck_level = no ;
	return 1 ;
}

/****************************************************************/
Uint8 pl_chouhatsu_ck( PLAYER_WORK *pl_w, Uint8 no )
{
	if( vs_sys_work.vs_st >= VS_END_WAIT ){ }
	if( pl_w->cpu_flag ){
		if( !pl_w->cpu_sp_flag || pl_w->cpu_sp_flag && pl_w->cpu_sp_no != no ) return 0 ;
	} else {
		if( !( pl_w->pl_now_lvbt & KEY_START ) ) return 0 ;
	}
	if( pl_w->pl_stance != STANCE_STAND && pl_w->pl_stance != STANCE_CROUCH ){
		if( pl_w->pl_type != PL_DAN ) return 0 ;
		if( pl_w->pl_stance != STANCE_AIR || pl_w->spd.y < 0.0f ) return 0 ;
		if( !sp_ok_ck( pl_w ) ) return 0 ;
	} else {
		if( !sp_ok_ck( pl_w ) ) return 0 ;
		pl_w->pos.y = pl_w->ground_y ;
		if( pl_w->pl_type != PL_SERVBOT ) pl_w->pl_stance = STANCE_STAND ;
	}
	return 1 ;
}

/****************************************************************/
/*			Motion commands: the stick's steps in time, then	*/
/*			the buttons											*/
/****************************************************************/
Uint8 com_nml_ck( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw )
{
	static Uint8 ( *com_nml_jmp[] )( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw ) = {
		com_nml_start, com_nml_lever, com_nml_sw, com_nml_sw_all, com_nml_04,
	} ;

	if( pl_w->cpu_flag ){
		if( pl_w->cpu_sp_flag && pl_w->cpu_sp_no == COM_NO( com ) ) return com_ok( pl_w, com, cw, 0 ) ;
		return 0 ;
	}
	return com_nml_jmp[cw->step]( pl_w, com, cw ) ;
}

/*==============================================================*/
static Uint8 com_nml_start( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw )
{
	cw->cnt = 0 ;
	if( ( pl_w->pl_sw_new & KEY_LEVER ) == com[4] ){
		com_timer_set( pl_w, cw ) ;
		cw->step = 1 ;
		cw->cnt = 1 ;
	}
	return 0 ;
}

/*==============================================================*/
static Uint8 com_nml_lever( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw )
{
	if( -- cw->timer <= 0 ) return cw->step = 0 ;
	if( ( pl_w->pl_sw_new & KEY_LEVER ) == com[cw->cnt + 4] ){
		com_timer_set( pl_w, cw ) ;
		if( ++ cw->cnt == com[0] ){
			if( com[1] & COM_NO_SW ) return com_ok( pl_w, com, cw, pl_w->pl_sw_new ) ;
			cw->step = 2 ;
			cw->timer = 15 ;
			return com_nml_sw( pl_w, com, cw ) ;
		}
	}
	return 0 ;
}

/*==============================================================*/
static Uint8 com_nml_sw( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw )
{
	Uint16 sw ;

	if( -- cw->timer <= 0 ) return cw->step = 0 ;
	sw = ( pl_w->pl_sw_new ^ pl_w->pl_sw_old ) & com[cw->cnt + 4] ;
	if( !sw ) return 0 ;
	else {
		if( ( com[1] & COM_ALL_SW ) == COM_ALL_SW ){
			if( sw == com[cw->cnt + 4] ) return com_sw_ok( pl_w, com, cw, sw ) ;
			cw->step ++ ;
		} else if( ( com[1] & COM_SW_NOW ) == COM_SW_NOW ) return com_ok( pl_w, com, cw, sw ) ;
		else return com_sw_ok( pl_w, com, cw, sw ) ;
		return 0 ;
	}
}

/*==============================================================*/
static Uint8 com_nml_sw_all( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw )
{
	Uint16 sw ;

	cw->step = 0 ;
	if( sw_on_ck( pl_w, com[cw->cnt + 4], &sw ) ) return com_sw_ok( pl_w, com, cw, sw ) ;
	return 0 ;
}

/*==============================================================*/
static Uint8 com_nml_04( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw )
{
	return 0 ;
}

/****************************************************************/
/*			Charge commands: a direction held, then the			*/
/*			other way and the buttons							*/
/****************************************************************/
Uint8 com_tame_ck( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw )
{
	static Uint8 ( *com_tame_jmp[] )( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw ) = {
		com_tame_start, com_tame_hold, com_tame_release, com_tame_lever, com_tame_sw,
	} ;

	if( pl_w->cpu_flag ){
		if( pl_w->cpu_sp_flag && pl_w->cpu_sp_no == COM_NO( com ) ) return com_ok( pl_w, com, cw, 0 ) ;
		return 0 ;
	}
	return com_tame_jmp[cw->step]( pl_w, com, cw ) ;
}

/*==============================================================*/
static Uint8 com_tame_start( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw )
{
	if( !( pl_w->pl_sw_new & com[4] ) ) return cw->step = 0 ;
	cw->timer = com[0] ;
	cw->cnt = pl_w->pl_dir ;
	cw->step ++ ;
	return 0 ;
}

/*==============================================================*/
static Uint8 com_tame_hold( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw )
{
	if( -- cw->timer > 0 ){
		if( !( pl_w->pl_sw_new & com[4] ) ) cw->step = 0 ;
	} else {
		cw->step ++ ;
	}
	return 0 ;
}

/*==============================================================*/
static Uint8 com_tame_release( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw )
{
	if( pl_w->pl_sw_new & com[4] ) return 0 ;
	if( com[4] != 0x1000 && com[4] != 0x2000 && cw->cnt != pl_w->pl_dir ){
		return cw->step = 0 ;
	}
	cw->step ++ ;
	cw->timer = 15 ;
	return com_tame_lever( pl_w, com, cw ) ;
}

/*==============================================================*/
static Uint8 com_tame_lever( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw )
{
	if( -- cw->timer <= 0 ) return cw->step = 0 ;
	if( com[5] == 0x0800 ){
		if( pl_w->pl_sw_new != com[5] ) return 0 ;
	} else {
		if( !( pl_w->pl_sw_new & com[5] ) ) return 0 ;
	}
	cw->step ++ ;
	cw->timer = 15 ;
	return com_tame_sw( pl_w, com, cw ) ;
}

/*==============================================================*/
static Uint8 com_tame_sw( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw )
{
	if( -- cw->timer <= 0 ) return cw->step = 0 ;
	if( pl_w->pl_sw_new & com[6] ) return com_ok( pl_w, com, cw, pl_w->pl_sw_new & com[6] ) ;
	return 0 ;
}

/****************************************************************/
/*			Mashing commands: a button pressed com[0] times,	*/
/*			each within com[1] frames							*/
/****************************************************************/
Uint8 com_renda_ck( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw )
{
	static Uint8 ( *com_renda_jmp[] )( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw ) = {
		com_renda_start, com_renda_count, com_renda_end,
	} ;

	if( pl_w->cpu_flag ){
		if( pl_w->cpu_sp_flag && pl_w->cpu_sp_no == COM_NO( com ) ) return com_ok( pl_w, com, cw, 0 ) ;
		return 0 ;
	}
	return com_renda_jmp[cw->step]( pl_w, com, cw ) ;
}

/*==============================================================*/
static Uint8 com_renda_start( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw )
{
	if( !( pl_w->pl_sw_now & com[4] ) ) return cw->step = 0 ;
	cw->sw = pl_w->pl_sw_now & com[4] ;
	cw->cnt = 1 ;
	cw->step ++ ;
	cw->timer = com[1] ;
	if( com[0] == cw->cnt ){
		cw->step = 2 ;
		cw->timer = 14 ;
		return com_renda_end( pl_w, com, cw ) ;
	}
	return 0 ;
}

/*==============================================================*/
static Uint8 com_renda_count( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw )
{
	if( -- cw->timer <= 0 ) return cw->step = 0 ;
	if( !( pl_w->pl_sw_now & com[4] ) ) return 0 ;
	if( ( pl_w->pl_sw_now & com[4] ) != cw->sw ) return 0 ;
	cw->cnt ++ ;
	cw->timer = com[1] ;
	if( com[0] == cw->cnt ){
		cw->step = 2 ;
		cw->timer = 14 ;
		return com_renda_end( pl_w, com, cw ) ;
	}
	return 0 ;
}

/*==============================================================*/
static Uint8 com_renda_end( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw )
{
	if( -- cw->timer <= 0 ) return cw->step = 0 ;
	if( ( pl_w->pl_sw_now & com[4] ) == cw->sw ) return 1 ;
	return 0 ;
}

/****************************************************************/
/*			Turns of the stick: com[0] quarter turns either		*/
/*			way, then the buttons as for a motion				*/
/****************************************************************/
Uint8 com_rot_ck( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw )
{
	static Uint8 ( *com_rot_jmp[] )( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw ) = {
		com_rot_start, com_rot_dir, com_rot_turn, com_nml_sw, com_nml_sw_all, com_nml_04,
	} ;

	if( pl_w->cpu_flag ){
		if( pl_w->cpu_sp_flag && pl_w->cpu_sp_no == COM_NO( com ) ) return com_ok( pl_w, com, cw, 0 ) ;
		return 0 ;
	}
	return com_rot_jmp[cw->step]( pl_w, com, cw ) ;
}

/* the stick's four ways in turning order: up, forward, down, back */
static Uint8 rot_tbl[4] = { 0x08, 0x02, 0x04, 0x01 } ;

/*==============================================================*/
static Uint8 com_rot_start( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw )
{
	Uint16 lever ;
	Uint8 dir ;

	lever = pl_w->pl_sw_new & KEY_LEVER ;
	if( !lever ) return cw->step = 0 ;
	if( lever != KEY_UP && lever != KEY_DOWN && lever != KEY_FORWARD && lever != KEY_BACK ) return cw->step = 0 ;
	if( lever == KEY_UP ) dir = 0 ;
	if( lever == KEY_FORWARD ) dir = 1 ;
	if( lever == KEY_DOWN ) dir = 2 ;
	if( lever == KEY_BACK ) dir = 3 ;
	cw->step ++ ;
	cw->cnt = com[0] - 1 ;
	cw->rot_dir = dir ;
	com_timer_set( pl_w, cw ) ;
	return 0 ;
}

/*==============================================================*/
static Uint8 com_rot_dir( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw )
{
	Uint16 lever ;

	if( -- cw->timer <= 0 ) return cw->step = 0 ;
	if( !( lever = ( pl_w->pl_sw_new & KEY_LEVER ) >> 10 ) ) return 0 ;
	if( lever == rot_tbl[( cw->rot_dir + 1 ) & 3] ) cw->dir = 1 ;
	else if( lever == rot_tbl[( cw->rot_dir - 1 ) & 3] ) cw->dir = -1 ;
	else return 0 ;
	cw->step ++ ;
	cw->cnt -- ;
	cw->rot_dir = ( cw->rot_dir + cw->dir ) & 3 ;
	com_timer_set( pl_w, cw ) ;
	return 0 ;
}

/*==============================================================*/
static Uint8 com_rot_turn( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw )
{
	Uint16 lever ;

	if( -- cw->timer <= 0 ) return cw->step = 0 ;
	if( !( lever = ( pl_w->pl_sw_new & KEY_LEVER ) >> 10 ) ) return 0 ;
	if( lever != rot_tbl[( cw->rot_dir + cw->dir ) & 3] ) return 0 ;
	if( !-- cw->cnt ){
		pl_w->sjump_w.step = 0 ;
		cw->step ++ ;
		cw->timer = 15 ;
		cw->cnt = 0 ;
		return com_nml_sw( pl_w, com, cw ) ;
	}
	cw->rot_dir = ( cw->rot_dir + cw->dir ) & 3 ;
	com_timer_set( pl_w, cw ) ;
	return 0 ;
}

/****************************************************************/
/*			Button sequences: com[0] presses of stick and		*/
/*			buttons, in order, then the buttons					*/
/****************************************************************/
Uint8 com_seq_ck( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw )
{
	static Uint8 ( *com_seq_jmp[] )( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw ) = {
		com_seq_start, com_seq_next, com_seq_sw, com_nml_sw_all, com_nml_04,
	} ;

	if( pl_w->cpu_flag ){
		if( pl_w->cpu_sp_flag && pl_w->cpu_sp_no == COM_NO( com ) ) return com_ok( pl_w, com, cw, 0 ) ;
		return 0 ;
	}
	return com_seq_jmp[cw->step]( pl_w, com, cw ) ;
}

/*==============================================================*/
static Uint8 com_seq_start( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw )
{
	cw->cnt = 0 ;
	if( ( pl_w->pl_sw_now & ( KEY_LEVER | KEY_ATCK ) ) == com[4] ){
		com_timer_set( pl_w, cw ) ;
		cw->step = 1 ;
		cw->cnt = 1 ;
	}
	return 0 ;
}

/*==============================================================*/
static Uint8 com_seq_next( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw )
{
	if( -- cw->timer <= 0 ) return cw->step = 0 ;
	if( ( pl_w->pl_sw_now & ( KEY_LEVER | KEY_ATCK ) ) == com[cw->cnt + 4] ){
		com_timer_set( pl_w, cw ) ;
		if( ++ cw->cnt == com[0] ){
			cw->step = 2 ;
			cw->timer = 15 ;
		}
	}
	return 0 ;
}

/*==============================================================*/
static Uint8 com_seq_sw( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw )
{
	Uint16 sw ;

	if( -- cw->timer <= 0 ) return cw->step = 0 ;
	sw = pl_w->pl_sw_now & com[cw->cnt + 4] & ( KEY_LEVER | KEY_ATCK ) ;
	if( !sw ) return 0 ;
	else {
		if( ( com[1] & COM_ALL_SW ) == COM_ALL_SW ){
			if( sw == com[cw->cnt + 4] ) return com_sw_ok( pl_w, com, cw, sw ) ;
			cw->step ++ ;
		} else if( ( com[1] & COM_SW_NOW ) == COM_SW_NOW ) return com_ok( pl_w, com, cw, sw ) ;
		else return com_sw_ok( pl_w, com, cw, sw ) ;
		return 0 ;
	}
}

/****************************************************************/
/*			A button held and let go							*/
/****************************************************************/
Uint8 com_hold_ck( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw )
{
	static Uint8 ( *com_hold_jmp[] )( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw, Uint16 sw ) = {
		com_hold_start, com_hold_wait, com_hold_release, com_hold_retry,
	} ;

	if( pl_w->cpu_flag ){
		if( pl_w->cpu_sp_flag && pl_w->cpu_sp_no == COM_NO( com ) ) pl_w->cpu_hold_sw = ~com[4] ;
		return com_hold_jmp[cw->step]( pl_w, com, cw, pl_w->cpu_hold_sw ) ;
	}
	return com_hold_jmp[cw->step]( pl_w, com, cw, pl_w->pl_sw_new ) ;
}

/*==============================================================*/
static Uint8 com_hold_start( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw, Uint16 sw )
{
	if( !( com[4] & sw ) ) return cw->step = 0 ;
	cw->timer = com[0] ;
	cw->step ++ ;
	return 0 ;
}

/*==============================================================*/
static Uint8 com_hold_wait( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw, Uint16 sw )
{
	if( -- cw->timer > 0 ){
		if( !( com[4] & sw ) ) cw->step = 0 ;
		return 0 ;
	}
	cw->step ++ ;
	return 0 ;
}

/*==============================================================*/
static Uint8 com_hold_release( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw, Uint16 sw )
{
	Uint16 now ;

	if( ( now = com[4] & sw ) != 0 ) return 0 ;
	if( com_ok( pl_w, com, cw, now ) ){
		cw->step = 0 ;
		return 1 ;
	} else {
		cw->step ++ ;
		cw->timer = 10 ;
	}
	return 0 ;
}

/*==============================================================*/
static Uint8 com_hold_retry( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw, Uint16 sw )
{
	if( !-- cw->timer ) return cw->step = 0 ;
	if( !com_ok( pl_w, com, cw, sw ) ) return 0 ;
	cw->step = 0 ;
	return 1 ;
}

/*==============================================================*/
/*			The buttons that finish a command: a punch or		*/
/*			kick pair needs time left on the clock				*/
/*==============================================================*/
static Uint8 com_sw_ok( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw, Uint16 sw )
{
	if( sw & KEY_HEAVY ){
		if( cw->timer - 4 >= 0 ) return com_ok( pl_w, com, cw, sw ) ;
		if( !( com[1] & ( COM_ALL_SW | 0x0600 ) ) ) sw &= ~KEY_HEAVY ;
	}
	if( ( sw & ( KEY_LIGHT | KEY_ASSIST ) ) && cw->timer - 2 >= 0 ) return com_ok( pl_w, com, cw, sw ) ;
	cw->step = 0 ;
	return 0 ;
}

/*==============================================================*/
/*			A command done: check the move may start now		*/
/*==============================================================*/
static Uint8 com_ok( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw, Uint16 sw )
{
	Uint16 attr = com[1] ;

	if( !( attr & COM_FREE ) ){
		if( ( attr & COM_ATCK_NG ) && ( pl_w->cg_atck_flag & CG_ATCK_ALL ) ) return 0 ;
		if( attr & COM_CUT ){
			if( !sp_ok_cut_ck( pl_w ) ) return 0 ;
		} else if( attr & COM_HYPER ){
			if( !sp_ok_hyper_ck( pl_w ) ) return 0 ;
		} else {
			if( !sp_ok_ck( pl_w ) ) return 0 ;
		}
	}
	if( ( attr & COM_AIR ) && pl_w->pl_stance != STANCE_AIR ) return 0 ;
	if( ( attr & COM_GROUND ) && pl_w->pl_stance == STANCE_AIR ) return 0 ;
	cw->step = 0 ;
	cw->sw = sw ;
	return 1 ;
}

/* the acts a special move can't start from */
static Uint8 sp_ng_act_tbl[35] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1,
	0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
} ;

/* the frames allowed for each step of a command, picked at random */
static Sint8 com_time_tbl[32] = {
	12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 13, 13, 13, 13,
	13, 13, 13, 13, 14, 14, 14, 14, 14, 14, 15, 15, 15, 16, 16, 17,
} ;

/****************************************************************/
Uint8 sp_ok_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->sp_ng_timer ) return 0 ;
	if( pl_w->atck_ng_timer ) return 0 ;
	if( ( pl_w->act_st == N_GUARD || pl_w->act_st == N_AIR_GUARD ) && pl_w->guard_timer ) return 0 ;
	if( pl_w->cg_atck_flag & CG_ATCK_NG ) return 0 ;
	if( pl_w->routine_1 != PL_NORM ) return 0 ;
	if( pl_w->act_st == N_SPECIAL ) return 0 ;
	if( pl_w->act_st == N_CHANGE ) return 0 ;
	if( pl_w->target_w->fly_screen_flag ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR ){
		if( !( pl_w->pos.y > pl_w->ground_y ) ) return 0 ;
		if( !( pl_w->pos.y < scr_work.stage_top + -DOT_Y( 32.0f ) ) ) return 0 ;
	}
	if( sp_ng_act_tbl[pl_w->act_st] ) return 0 ;
	return 1 ;
}

/*==============================================================*/
/*			Sp_ok_ck without the timers: for the moves that		*/
/*			may cut into another								*/
/*==============================================================*/
static Uint8 sp_ok_cut_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->routine_1 != PL_NORM ) return 0 ;
	if( pl_w->act_st == N_SPECIAL ) return 0 ;
	if( pl_w->act_st == N_CHANGE ) return 0 ;
	if( pl_w->target_w->fly_screen_flag ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR ){
		if( !( pl_w->pos.y > pl_w->ground_y ) ) return 0 ;
		if( !( pl_w->pos.y < scr_work.stage_top + -DOT_Y( 32.0f ) ) ) return 0 ;
	}
	if( sp_ng_act_tbl[pl_w->act_st] ) return 0 ;
	return 1 ;
}

/****************************************************************/
/*			Sp_ok_ck for a hyper combo: from a special move		*/
/*			only while it may still be cancelled				*/
/****************************************************************/
Uint8 sp_ok_hyper_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->sp_ng_timer ) return 0 ;
	if( pl_w->atck_ng_timer ) return 0 ;
	if( ( pl_w->act_st == N_GUARD || pl_w->act_st == N_AIR_GUARD ) && pl_w->guard_timer ) return 0 ;
	if( pl_w->cg_atck_flag & CG_ATCK_NG ) return 0 ;
	if( pl_w->routine_1 != PL_NORM ) return 0 ;
	if( pl_w->act_st == N_SUPER ) return 0 ;
	if( pl_w->act_st == N_CHANGE ) return 0 ;
	if( pl_w->act_st == N_SPECIAL ){
		if( pl_w->cancel_timer < 0 ){
			if( ( pl_w->cg_atck_flag & CG_ATCK_ALL ) != CG_ATCK_ON ) return 0 ;
		} else if( !pl_w->cancel_timer ) return 0 ;
	}
	if( pl_w->target_w->fly_screen_flag ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR ){
		if( !( pl_w->pos.y > pl_w->ground_y ) ) return 0 ;
		if( !( pl_w->pos.y < scr_work.stage_top + -DOT_Y( 32.0f ) ) ) return 0 ;
	}
	if( sp_ng_act_tbl[pl_w->act_st] ) return 0 ;
	return 1 ;
}

/*==============================================================*/
static void com_timer_set( PLAYER_WORK *pl_w, COM_WORK *cw )
{
	cw->timer = com_time_tbl[Rnd() & 31] ;
}

/****************************************************************/
void com_sw_set( PLAYER_WORK *pl_w, COM_WORK *cw )
{
	if( pl_w->cpu_flag ){
		pl_w->atck_pk = pl_w->cpu_atck_pk ;
		pl_w->atck_level = pl_w->cpu_atck_level ;
		pl_w->atck_lever = pl_w->cpu_atck_lever ;
		return ;
	}
	if( cw->sw & 0x0240 ) pl_w->atck_level = 0 ;
	if( cw->sw & 0x0120 ) pl_w->atck_level = 1 ;
	if( cw->sw & 0x0300 ) pl_w->atck_pk = 0 ;
	if( cw->sw & 0x0060 ) pl_w->atck_pk = 1 ;
}

/****************************************************************/
/*			Buttons pressed together: each of sw newly down		*/
/*			this frame or the one before						*/
/****************************************************************/
Uint8 sw_on_ck( PLAYER_WORK *pl_w, Uint16 sw, Uint16 *now )
{
	if( ( *now = ( ( pl_w->pl_old_lvbt ^ pl_w->pl_last_lvbt ) | ( pl_w->pl_new_lvbt ^ pl_w->pl_last_lvbt ) ) & sw ) == sw ) return 1 ;
	return 0 ;
}

/****************************************************************/
Uint8 sw_now_ck( PLAYER_WORK *pl_w, Uint16 sw, Uint16 *now, Uint8 no )
{
	if( pl_w->cpu_flag ){
		if( pl_w->cpu_sp_flag && pl_w->cpu_sp_no == no ) return 1 ;
		return 0 ;
	}
	if( ( *now = pl_w->pl_now_lvbt & sw ) == sw ) return 1 ;
	return 0 ;
}

/****************************************************************/
int lever_gacha_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->team_no ) return 0 ;
	if( pl_w->cpu_flag ) return cpu_renda_ck( pl_w ) ;
	else if( pl_w->pl_now_lvbt & KEY_LEVER ) return 1 ;
	return 0 ;
}

/****************************************************************/
int sw_renda_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->team_no ) return 0 ;
	if( pl_w->cpu_flag ) return cpu_renda_ck( pl_w ) ;
	else if( pl_w->pl_now_lvbt & KEY_ATCK ) return 1 ;
	return 0 ;
}

/****************************************************************/
int lever_sw_renda_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->team_no ) return 0 ;
	if( pl_w->cpu_flag ) return cpu_renda_ck( pl_w ) ;
	else if( pl_w->pl_now_lvbt & ( KEY_LEVER | KEY_ATCK ) ) return 1 ;
	return 0 ;
}

/* the computer's mashing: a chance by the game's level */
static Uint32 cpu_renda_tbl[] = {
	0x00000001, 0x00000003, 0x00000003, 0x00000007, 0x00000007, 0x0000000f, 0x0000001f, 0x0000001f,
	0x0000003f, 0x0000003f, 0x0000007f, 0x0000007f, 0x0000007f, 0x000000ff, 0x000000ff, 0x000000ff,
	0x000001ff, 0x000001ff, 0x000001ff, 0x000003ff, 0x000003ff, 0x000003ff, 0x000007ff, 0x000007ff,
	0x000007ff, 0x00000fff, 0x00000fff, 0x00000fff, 0x00001fff, 0x00001fff, 0x00003fff, 0x00003fff,
} ;

/*==============================================================*/
static int cpu_renda_ck( PLAYER_WORK *pl_w )
{
	Uint32 bit ;
	Sint8 lv ;

	if( pl_w->team_no ) return 0 ;
	if( vs_sys_w->vs_st != VS_FIGHT ) return 0 ;
	bit = 1 << ( Rnd() & 31 ) ;
	lv = Sys->cpu_renda_level ;
	if( bit & cpu_renda_tbl[lv * 32] ) return 1 ;
	return 0 ;
}

/* the computer's chance out of 32, by its level, of calling the partner with less vitality */
static Uint8 cpu_partner_tbl[32] = {
	16, 16, 16, 16, 18, 18, 20, 20, 20, 20, 22, 22, 22, 24, 24, 24,
	24, 24, 24, 24, 26, 26, 26, 26, 26, 26, 28, 28, 28, 30, 30, 30,
} ;

/*==============================================================*/
static Uint8 cpu_partner_sel( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *p1, *p2, *t ;
	Uint8 rnd ;

	p1 = vs_sys_w->team_w[pl_w->pl_no][1] ;
	p2 = vs_sys_w->team_w[pl_w->pl_no][2] ;
	if( p1->vital_new < p2->vital_new ){
		t = p1 ;
		p1 = p2 ;
		p2 = t ;
	}
	rnd = Rnd() & 31 ;
	if( rnd > cpu_partner_tbl[cpu_level_work.level] ) p1 = p2 ;
	return p1->team_no ;
}

/* the same for the opponent's partner a snapback brings in, by the vitality each has lost */
static Uint8 cpu_snapback_tbl[32] = {
	16, 16, 16, 16, 18, 18, 20, 20, 20, 20, 22, 22, 22, 24, 24, 24,
	24, 24, 24, 24, 26, 26, 26, 26, 26, 26, 28, 28, 28, 30, 30, 30,
} ;

/*==============================================================*/
static Uint8 cpu_snapback_sel( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *p1, *p2, *t ;
	Uint8 rnd ;

	p1 = vs_sys_w->team_w[pl_w->pl_no ^ 1][1] ;
	p2 = vs_sys_w->team_w[pl_w->pl_no ^ 1][2] ;
	if( p1->vital_red - p1->vital_new < p2->vital_red - p2->vital_new ){
		t = p1 ;
		p1 = p2 ;
		p2 = t ;
	}
	rnd = Rnd() & 31 ;
	if( rnd > cpu_snapback_tbl[cpu_level_work.level] ) p1 = p2 ;
	return p1->team_no ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pl_com.c" End of File	*/

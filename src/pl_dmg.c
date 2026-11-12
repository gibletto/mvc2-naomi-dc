/* The damage code: a player struck or guarding, the damage and stun a hit does, the gauge it
   gives, the knock-back and the reactions, the hit marks, and the vital and armour helpers. */
#include "player.h"
#include "pl_char.h"
#include "pl_color_old.h"

/* the reaction routines take the attacker too */
typedef void ( *DM_FUNC )( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
typedef int ( *DM_CK )( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
typedef int ( *DM_CK1 )( PLAYER_WORK *dm_w );

/* the tables of the hit data module */
extern Sint16 dm_pow_tbl[];	/* the knock-back strengths */
extern Sint16 back_spd_tbl[];	/* the knock-back speeds */
extern Sint8 *back_step_tbl[];	/* the knock-back steps */
extern Sint8 weight_diff_tbl[];	/* the knock-back by the players' places */
extern Sint8 gauge_rate_tbl[];	/* the gauge rate of each character */
extern Sint8 gauge_combo_tbl[];	/* the gauge rate by hits taken */
extern Sint8 stun_rnd_tbl[];	/* the stun's random part */
extern Uint8 damage_level_tbl[];	/* the damage rate by the test menu's level */
extern Uint8 cpu_atck_rate_tbl[];	/* the computer's attack rate by its level */
#ifdef TARGET_DC
extern Uint8 handicap_tbl[];	/* the damage rate by the attacker's handicap */
#endif
extern Uint8 cpu_def_rate_tbl[];	/* the computer's defence rate by its level */
extern Uint8 dm_hit_rate_tbl[][20];	/* the damage rate by the hits in a combo */
extern Sint16 vital_rate_tbl[][2];	/* the damage rate by the vital left */
extern Uint8 def_rate_tbl[];	/* the defence rate of each character */
extern Sint8 guard_tbl[];	/* the guard tables */
extern Sint8 guard_hyper_tbl[];
extern Sint8 armor_tbl[][2];	/* each character's own armour */

extern Uint8 pl_guard_ok( PLAYER_WORK *pl_w );
extern void dm_voice_req( PLAYER_WORK *pl_w );
extern void vib_req( int pl_no, int no );
extern void dm_se_req( PLAYER_WORK *pl_w );
extern void hit_flash_set( VEC3 *pos, int no );
extern void guard_mark_ef_set( VEC3 *pos, int dir );
extern PLAYER_WORK *pls19_00_set2( PLAYER_WORK *pl_w, VEC3 *pos, Uint8 no );
extern void pls1a_00_set2( PLAYER_WORK *pl_w, VEC3 *pos, Uint8 no );
extern PLAYER_WORK *dm00_02_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *dm00_07_set( PLAYER_WORK *pl_w );
extern void pl_tlocal_init( PLAYER_WORK *pl_w );

static void dm_data_set( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static Sint16 dm_calc( PLAYER_WORK *dm_w, PLAYER_WORK *at_w, HIT_DATA *hd, Sint16 dmg );
static Uint8 guard_ck( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
#ifdef TARGET_DC
static int dummy_guard_ck( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
#endif
static int auto_guard_ck( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static int guard_ng_ck( PLAYER_WORK *dm_w );
static void armor_guard( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static void guard_set( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static void guard_char_set( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static void gauge_add( PLAYER_WORK *pl_w, Sint16 no );
static void dm_gauge_add( PLAYER_WORK *dm_w );
static void guard_back_set( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static void dm_back_set( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static void back_set_nml( PLAYER_WORK *dm_w, Uint8 no );
static void back_set_fix( PLAYER_WORK *dm_w, Uint8 no );
static void back_set_push( PLAYER_WORK *dm_w, Uint8 no );
static void back_set_catch( PLAYER_WORK *dm_w, Uint8 no );
static void back_set_turn( PLAYER_WORK *dm_w, Uint8 no );
static void dm_blow_set( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static void back_move_push( PLAYER_WORK *pl_w );
void back_move_catch( PLAYER_WORK *pl_w );
static void armor_back_set( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static void guard_ef_set( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static void dm_hitmark_set( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
void clash_mark_set( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static void guard_mark_set( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static void hit_pos_get( PLAYER_WORK *dm_w, PLAYER_WORK *at_w, VEC3 *pos );
void catch_hitmark( PLAYER_WORK *pl_w );
void catch_hitmark_no_se( PLAYER_WORK *pl_w );

static void dm_zuri( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static void abyss_dm_zuri( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static void dm_buttobi( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static void dm_buttobi_ng( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static void dm_buttobi_ukemi_ng( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static void dm_snap( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static void dm_slam( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static void dm_slam_ng( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static void dm_down_set( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static void dm_piyo( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static void dm_sweep( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static void dm_hold( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static void dm_07( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static void dm_08( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static void dm_07_set( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static void dm_freeze( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static void dm_freeze_air( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static void dm_away( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static void dm_bound( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static void dm_nop( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static void dm_spark( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static void dm_dizzy( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static void dm_sandbag( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static void dm_curse( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static void dm_air( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );

static void dm_char_set( PLAYER_WORK *dm_w );
static void dm_air_char_set( PLAYER_WORK *dm_w );
static void dm_elem_set( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static Uint8 dm_back_no_get( PLAYER_WORK *dm_w );
static void dm_blow_spd_set( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static void blow_spd_nml( PLAYER_WORK *dm_w, Uint8 no );
static void blow_spd_fix( PLAYER_WORK *dm_w, Uint8 no );
static void blow_spd_push( PLAYER_WORK *dm_w, Uint8 no );
static void blow_spd_catch( PLAYER_WORK *dm_w, Uint8 no );
void catch_blow_spd_set( PLAYER_WORK *dm_w );
static void catch_blow_spd_nml( PLAYER_WORK *dm_w, Uint8 no );
void catch_blow_spd_fix( PLAYER_WORK *dm_w, Uint8 no );
static void dm_spd_get( Uint8 dir, Uint8 no, VEC2 *spd );
static int dm_air_combo_ck( PLAYER_WORK *dm_w );
void dm_spd_y_limit( PLAYER_WORK *pl_w );
void dm_ko_set( PLAYER_WORK *dm_w );
void dm_ko_dir_set( PLAYER_WORK *dm_w, Uint8 no, Uint8 dir );
static void dm_ko_blow( PLAYER_WORK *dm_w, Uint8 no );
static void dm_ko_set3( PLAYER_WORK *dm_w );
void catch_hit_set( PLAYER_WORK *pl_w );
void catch_hit_set2( PLAYER_WORK *pl_w );
static void dm_red_set( PLAYER_WORK *dm_w, Sint16 *dmg );
void vital_sub( PLAYER_WORK *pl_w, Sint16 dmg );
static void dm_ko_req( PLAYER_WORK *pl_w );
void catch_ko_ck( PLAYER_WORK *pl_w );
void vital_red_recover( void );
void team_ko_set( PLAYER_WORK *pl_w );
static void dm_vital_disp( PLAYER_WORK *pl_w, Sint16 dmg );
void vital_red_timer( void );
void def_plus_set( PLAYER_WORK *pl_w, int time, int pow );
void speed_up_set( PLAYER_WORK *pl_w, int time );
static void abyss_damage( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static void armor_hit_wait( PLAYER_WORK *pl_w );
static void armor_hit_shake( PLAYER_WORK *pl_w );
static int armor_ck( PLAYER_WORK *pl_w );
static void ko_se_req( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
static void dm_fly_screen_clr( PLAYER_WORK *dm_w );
Sint8 *guard_tbl_get( PLAYER_WORK *pl_w, HIT_DATA *hd );
/* the reactions, by stance (standing, crouching, in the air, down) and the hit's reaction */
static DM_FUNC dm_tbl[4][22] = {
	{	dm_zuri, dm_buttobi, dm_buttobi_ng, dm_piyo, dm_sweep, dm_hold, dm_hold, dm_07, dm_08, dm_freeze, dm_away,
		dm_bound, dm_snap, dm_zuri, dm_nop, dm_spark, dm_dizzy, dm_slam, dm_slam_ng, dm_sandbag, dm_curse, dm_buttobi_ukemi_ng },
	{	dm_zuri, dm_buttobi, dm_buttobi_ng, dm_piyo, dm_sweep, dm_hold, dm_hold, dm_07, dm_08, dm_freeze, dm_away,
		dm_bound, dm_snap, dm_zuri, dm_nop, dm_spark, dm_dizzy, dm_slam, dm_slam_ng, dm_sandbag, dm_curse, dm_buttobi_ukemi_ng },
	{	dm_buttobi, dm_buttobi, dm_buttobi_ng, dm_piyo, dm_buttobi_ng, dm_hold, dm_hold, dm_07, dm_08, dm_freeze_air, dm_away,
		dm_bound, dm_snap, dm_zuri, dm_nop, dm_spark, dm_dizzy, dm_buttobi, dm_buttobi_ng, dm_sandbag, dm_curse, dm_buttobi_ukemi_ng },
	{	dm_air, dm_air, dm_air, dm_air, dm_air, dm_hold, dm_hold, dm_air, dm_air, dm_air, dm_air,
		dm_air, dm_snap, dm_air, dm_air, dm_air, dm_air, dm_air, dm_air, dm_sandbag, dm_curse, dm_air },
} ;

/* the damage rate by the hits in a combo, for each side's damage level */
static Uint8 *dm_lv_tbl[4] = { dm_hit_rate_tbl[1], dm_hit_rate_tbl[1], dm_hit_rate_tbl[2], dm_hit_rate_tbl[3] } ;

/****************************************************************/
/*			A player struck by an attack						*/
/****************************************************************/
void pl_damage( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	HIT_DATA *hd ;

	if( dm_w->cpu_flag ){
		if( dm_w->cpu_vcounter_cnt ) dm_w->cpu_vcounter_cnt -- ;
		if( dm_w->cpu_guard_back_cnt ) dm_w->cpu_guard_back_cnt -- ;
	}
	if( ( ( Uint16 )at_w->atck_attr & ATR_SHOT_HOLD ) && at_w->work_kind == W_SHL && dm_w->routine_1 == PL_CAUGHT && dm_w->catch_type == 6 ) return ;
	dm_w->hit_data_adrs = &at_w->hit_data_tbl[dm_w->dm_atck_no & ATCK_NO_MASK] ;
	hd = dm_w->hit_data_adrs ;
	if( ABYSS_CK( dm_w ) ){
		abyss_damage( dm_w, at_w ) ;
		return ;
	}
	dm_w->dm_elem_old = dm_w->dm_elem ;
	dm_w->dm_elem = 0 ;
	dm_w->hit_stop = hd->hit_stop ;
	at_w->hit_stop = hd->hit_stop ;
	if( at_w->atck_flag & HIT_OWNER ) dm_w->dm_at_w->hit_stop = hd->hit_stop ;
	dm_w->renda_cnt = 0 ;
	if( dm_w->pl_type == PL_CAPTAIN_AMERICA ){
		if( ( ( DM_CK )dm_w->pl_tbl[TBL_EX1] )( dm_w, at_w ) ){
			armor_guard( dm_w, at_w ) ;
			return ;
		}
	}
	if( dm_w->pl_type == PL_SAKURA ){
		if( ( ( DM_CK )dm_w->pl_tbl[TBL_EX0] )( dm_w, at_w ) ){
			armor_guard( dm_w, at_w ) ;
			return ;
		}
	}
	if( dm_w->pl_type == PL_COLOSSUS ){
		if( ( ( DM_CK )dm_w->pl_tbl[TBL_EX0] )( dm_w, at_w ) ){
			armor_guard( dm_w, at_w ) ;
			return ;
		}
	}
	if( dm_w->pl_type == PL_MEGAMAN || dm_w->pl_type == PL_ROLL ){
		if( ( ( DM_CK1 )dm_w->pl_tbl[TBL_EX0] )( dm_w ) ){
			armor_guard( dm_w, at_w ) ;
			return ;
		}
	}
	if( !dm_w->fly_flag && !( dm_w->armor_flag & ARMOR_HEAVY ) ){
#ifdef TARGET_DC	/* the training's dummy, set to guard every hit */
		if( Sys->dummy_guard == 2 && dummy_guard_ck( dm_w, at_w ) ){
			guard_set( dm_w, at_w ) ;
			if( dm_w->vital_new ) return ;
		}
#endif
		if( ( pl_guard_ok( dm_w ) && guard_ck( dm_w, at_w ) ) || auto_guard_ck( dm_w, at_w ) ){
			guard_set( dm_w, at_w ) ;
			if( dm_w->vital_new ) return ;
		}
	}
	if( !dm_w->routine_1 ){
		if( dm_w->pl_type == PL_MEGAMAN || dm_w->pl_type == PL_ROLL ){
			if( ( ( DM_CK1 )dm_w->pl_tbl[TBL_EX1] )( dm_w ) ){
				armor_guard( dm_w, at_w ) ;
				return ;
			}
		}
	}
	pl_color_set( dm_w, 1 ) ;
	dm_w->guard_cnt = 0 ;
	combo_hit2( dm_w, at_w, hd ) ;
	dm_w->combo_reset_flag = 0xff ;
	dm_data_set( dm_w, at_w ) ;
	dm_w->fly_flag = 0 ;
	dm_gauge_add( dm_w ) ;
	vs_sys_w->hit_cnt[dm_w->pl_no ^ 1] ++ ;
	if( dm_w->dm_no != 5 && dm_w->dm_no != 12 ){
		if( !dm_w->stun_cnt ) dm_w->dm_no = 3 ;
	}
	if( dm_w->dm_no != 12 && dm_w->snap_partner ) dm_w->dm_no = 12 ;
	if( dm_w->pl_stance != STANCE_AIR && dm_w->dm_no != 5 ){
		if( dm_w->routine_1 == PL_NORM && dm_w->act_st == N_STAND && dm_w->float_flag ) dm_w->pos.y = dm_w->ground_y ;
		else if( dm_w->pos.y > dm_w->ground_y ) dm_w->pl_stance = STANCE_AIR ;
		else dm_w->pos.y = dm_w->ground_y ;
	}
	if( !dm_w->vital_new ){
		if( dm_w->pl_stance == STANCE_DOWN ){
			dm_w->pl_stance = STANCE_CROUCH ;
			pl_dir_set( dm_w ) ;
		}
		if( dm_w->dm_no != 2 ){
			dm_w->dm_dir = 30 ;
			dm_w->dm_lv = 12 ;
			if( vs_sys_work.vs_st >= VS_WIN ) dm_w->dm_lv = 8 ;
		}
		dm_w->recover_timer = 10 ;
		dm_w->dm_no = 2 ;
		dm_w->snap_partner = 0 ;
		if( !dm_w->ko_flag ){
			dm_w->ko_flag = 0xff ;
			at_w->hit_stop = 10 ;
			dm_w->hit_stop = 10 ;
			if( at_w->atck_flag & HIT_OWNER ) dm_w->dm_at_w->hit_stop = at_w->hit_stop ;
			flash_ko_set() ;
			if( vs_sys_w->ko_cnt[dm_w->pl_no] <= 2 ) ko_se_req( dm_w, at_w ) ;
			pl_voice_req( dm_w, 0 ) ;
		}
	} else if( dm_w->armor_flag ){
		if( dm_w->routine_1 == PL_CAUGHT ){
			if( dm_w->pos.y > dm_w->ground_y ) pl_jump_ret( dm_w ) ;
			else pl_stand_ret( dm_w ) ;
		}
		goto armor ;
	} else if( dm_w->armor_timer_flag && !armor_ck( dm_w ) ){
armor:
		armor_back_set( dm_w, at_w ) ;
		dm_hitmark_set( dm_w, at_w ) ;
		return ;
	}
	if( dm_w->pl_stance != STANCE_DOWN ) pl_dir_set( dm_w ) ;
	if( !( Rnd() & 1 ) ) dm_voice_req( dm_w ) ;
#ifdef TARGET_DC	/* the training's dummy, set to guard after the first hit */
	if( Sys->dummy_guard == 1 ) dm_w->auto_guard_timer = 63 ;
#endif
	if( dm_w->down_lift_flag ){
		dm_w->down_lift_flag = 0 ;
		dm_w->pl_stance = STANCE_STAND ;
		dm_w->recover_ng_flag = 0 ;
	}
	dm_tbl[dm_w->pl_stance][dm_w->dm_no]( dm_w, at_w ) ;
	dm_w->routine_1 = PL_DAMAGE ;
	dm_w->routine_2 = dm_w->routine_3 = 0 ;
	dm_w->wall_hit_flag = 0 ;
	dm_w->disp_flag = 1 ;
	dm_w->rot[2] = 0 ;
	dm_w->scale = dm_w->pl_scale ;
	dm_w->alpha = 1.0f ;
	act_st_set( dm_w, N_DAMAGE ) ;
	dm_fly_screen_clr( dm_w ) ;
	if( dm_w->fly_screen_flag || dm_w->snap_partner ) pl_se_req( dm_w, 40 ) ;
	dm_hitmark_set( dm_w, at_w ) ;
}

/*==============================================================*/
/*			Takes the hit's data: the damage, the stun, the		*/
/*			reaction and the throw protection					*/
/*==============================================================*/
static void dm_data_set( register PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	HIT_DATA *hd = dm_w->hit_data_adrs ;
	Sint16 dmg ;
	Uint8 kind, stun ;
	int no ;

	dm_w->dm_kind = hd->damage_kind ;
	dm_w->snap_partner = 0 ;
	if( dm_w->dm_at_w->team_no ) dm_w->assist_dm_cnt ++ ;
	no = 1 ;
	if( hd->damage > 13 ) no = 3 ;
	vib_req( dm_w->pl_no, no ) ;
	dmg = hd->damage ;
	if( dmg ){
		if( vs_sys_work.vs_st == VS_FIGHT ) dmg = dm_calc( dm_w, &pl_work[at_w->work_no], hd, dmg ) ;
	}
	if( vs_sys_work.vs_st == VS_FIGHT && dm_w->vital_new ){
		dm_red_set( dm_w, &dmg ) ;
		if( ( dm_w->vital_new -= dmg ) <= 0 ){
#ifdef TARGET_DC	/* nobody is knocked out in the training */
			if( !( hd->hit_flag & NO_KO ) && !Sys->no_ko_flag ) dm_ko_req( dm_w ) ;
#else
			if( !( hd->hit_flag & NO_KO ) ) dm_ko_req( dm_w ) ;
#endif
			else {
				dm_w->vital_new = 1 ;
				if( dm_w->vital_red <= 0 ) dm_w->vital_red = 1 ;
			}
		}
		dm_vital_disp( dm_w, dmg ) ;
#ifdef TARGET_DC
		team_ko_set( dm_w ) ;
#endif
	}
	dm_w->dm_guard_type = hd->atck_kind ;
	dm_w->dm_recover_time = hd->sandbag_time ;
	dm_w->dm_down_time = hd->sleep_time ;
	kind = hd->damage_type & DM_NO_MASK ;
	if( ( Uint16 )at_w->atck_attr & ATR_FREEZE ) kind = 9 ;
	if( kind == 9 ){
		if( dm_w->routine_1 == PL_DAMAGE && ( dm_w->dm_no == 9 || dm_w->dm_no == 23 ) && ( dm_w->dm_kind & K_ELEM_MASK ) != 5 ) kind = 1 ;
	}
	if( kind == 20 ){
		if( dm_w->routine_1 == PL_DAMAGE && dm_w->dm_no == 20 ) kind = 1 ;
	}
	dm_w->dm_no = kind ;
	if( dm_w->pl_stance == STANCE_DOWN && ( ( Uint16 )at_w->atck_attr & ATR_DOWN_LIFT ) ) dm_w->down_lift_flag = 0xff ;
	if( hd->catch_ng_time ){
		dm_w->catch_ng_timer = hd->catch_ng_time & CATCH_NG_MASK ;
		dm_w->dm_flag = 0 ;
		if( at_w->work_kind == W_PL ){
			at_w->catch_ng_timer = dm_w->catch_ng_timer ;
			at_w->dm_flag = dm_w->dm_flag ;
		}
	}
	stun = hd->piyo ;
	if( stun ){
		stun += stun_rnd_tbl[Rnd() & 31] ;
		if( dm_w->stun_cnt != 1 && dm_w->stun_cnt <= stun && stun < 20 ) dm_w->stun_cnt = 1 ;
		else {
			if( !dm_w->stun_ng_flag && !dm_w->stun_ng_timer ){
				if( ( Sint8 )( dm_w->stun_cnt -= stun ) < 0 ) dm_w->stun_cnt = 0 ;
			}
		}
	}
	dm_w->dm_dir = hd->vector ;
	if( dm_w->dm_atck_no & ATCK_DIR ) dm_w->dm_dir = at_w->dir ;
	dm_w->dm_lv = hd->power ;
	if( ( Uint16 )at_w->atck_attr & ATR_KNOCK_AWAY ){
		dm_w->dm_no = 10 ;
		dm_w->dm_dir = 0 ;
	}
	dm_w->stun_reset_timer = 60 ;
	if( vs_sys_work.vs_st == VS_FIGHT ) hit_score_add( dm_w->dm_at_w, dm_w->hit_data_adrs ) ;
	if( hd->unk_13 ){ }
}

/*==============================================================*/
/*			The damage a hit does, after all the rates			*/
/*==============================================================*/
static Sint16 dm_calc( PLAYER_WORK *dm_w, PLAYER_WORK *at_w, HIT_DATA *hd, Sint16 dmg )
{
	Uint8 *tbl ;
	Uint8 lv ;
	Float d, rate, def, f ;
	int i ;

	d = dmg ;
	if( at_w->cpu_flag ) d *= cpu_atck_rate_tbl[vs_sys_w->cpu_level] / 32.0f ;
	if( ( Uint8 )dm_w->dm_hit_cnt > 20 ){ }
	if( ( lv = dm_w->dm_hit_cnt ) > 19 ) lv = 19 ;
	rate = 32.0f ;
	if( !( hd->hit_flag & NO_SCALE ) ){
		if( hd->atck_kind & A_HYPER ) tbl = dm_lv_tbl[vs_sys_w->hyper_chain_cnt[at_w->pl_no]] ;
		else tbl = dm_hit_rate_tbl[0] ;
		rate = tbl[lv] ;
	}
	if( dm_w->team_no ) rate += 2.0f ;
	if( ( d = d * rate / 32.0f ) < 1.0f ) d = 1.0f ;
	def = def_rate_tbl[dm_w->pl_type] / 32.0f ;
	if( dm_w->cpu_flag ) def *= cpu_def_rate_tbl[vs_sys_w->cpu_level] / 32.0f ;
	for( i = 0 ; i < 4 ; i ++ ){
		if( dm_w->vital_new <= vital_rate_tbl[i][0] ) break ;
	}
	rate = vital_rate_tbl[i][1] / 32.0f ;
#ifdef TARGET_DC
	if( ( f = d * def * rate * ( damage_level_tbl[Sys->damage_level] / 32.0f ) * ( handicap_tbl[at_w->handicap] / 32.0f ) ) < 1.0f ) f = 1.0f ;
#else
	if( ( f = d * def * rate * ( damage_level_tbl[Sys->damage_level] / 32.0f ) ) < 1.0f ) f = 1.0f ;
#endif
	if( dm_w->target_w->v_combi_cnt >= 2 ){
		if( ( f *= 0.875f ) < 1.0f ) f = 1.0f ;
	}
	if( ( f *= 1.0f ) < 1.0f ) f = 1.0f ;
	if( at_w->atck_pow ) f *= at_w->atck_pow / 32.0f ;
	else if( at_w->atck_plus ) f *= at_w->atck_plus / 32.0f ;
	if( dm_w->def_pow ) f *= dm_w->def_pow / 32.0f ;
	else if( dm_w->def_plus ) f *= dm_w->def_plus / 32.0f ;
	if( f < 1.0f ) f = 1.0f ;
	return f ;
}

/*==============================================================*/
/*			Whether the player guards the hit					*/
/*==============================================================*/
static Uint8 guard_ck( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	HIT_DATA *hd = dm_w->hit_data_adrs ;
	Uint8 type ;

	if( hd->atck_kind & A_GUARD_NG ) return 0 ;
	if( !dm_w->cpu_flag || !em_guard_ck( dm_w ) ){
		if( dm_w->routine_1 ) return 0 ;
		if( dm_w->act_st != N_AIR_GUARD && dm_w->act_st != N_GUARD ) return 0 ;
		if( ( type = hd->atck_kind & A_GUARD_MASK ) ){
			if( type == 2 ){
				if( dm_w->pl_stance == STANCE_CROUCH ){
					if( dm_w->pl_new_lvbt & KEY_DOWN ) return 0 ;
				}
			} else if( type == 4 ){
				if( dm_w->pl_stance != STANCE_CROUCH ){
					if( !( dm_w->pl_new_lvbt & KEY_DOWN ) ) return 0 ;
				}
			} else return 0 ;
		}
	}
	guard_mark_set( dm_w, at_w ) ;
	return 1 ;
}

#ifdef TARGET_DC
/*==============================================================*/
/*			Whether the training's dummy guards the hit			*/
/*==============================================================*/
static int dummy_guard_ck( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	if( guard_ng_ck( dm_w ) ) return 0 ;
	guard_mark_set( dm_w, at_w ) ;
	return 1 ;
}
#endif

/*==============================================================*/
/*			Whether the player guards by itself					*/
/*==============================================================*/
static int auto_guard_ck( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	if( !dm_w->auto_guard_timer || guard_ng_ck( dm_w ) ) return 0 ;
	guard_mark_set( dm_w, at_w ) ;
	return 1 ;
}

/*==============================================================*/
/*			Whether the hit can't be guarded by itself			*/
/*==============================================================*/
static int guard_ng_ck( PLAYER_WORK *dm_w )
{
	HIT_DATA *hd = dm_w->hit_data_adrs ;

	dm_w->dm_guard_type = hd->atck_kind ;
	if( dm_w->dm_guard_type & A_GUARD_NG ) return 1 ;
	if( dm_w->team_no ) return 1 ;
	if( !( dm_w->dm_hit_w->cg_atck_flag & CG_ATCK_ALL ) ) return 1 ;
	dm_w->dm_no = hd->damage_type & DM_NO_MASK ;
	if( dm_w->routine_1 == PL_DAMAGE ) return 1 ;
	if( !dm_w->routine_1 && dm_w->act_st == N_SPECIAL ) return 1 ;
	if( dm_w->pl_stance == STANCE_DOWN ) return 1 ;
	if( dm_w->act_st == N_WALL_JUMP ) return 1 ;
	if( dm_w->routine_1 == PL_CAUGHT ) return 1 ;
	if( !pl_guard_ok( dm_w ) ) return 1 ;
	if( dm_w->cg_atck_flag & CG_ATCK_ALL ) return 1 ;
	return 0 ;
}

/*==============================================================*/
/*			A hit taken by armour or a shield					*/
/*==============================================================*/
static void armor_guard( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	at_w->hit_flag |= HR_ARMOR ;
	if( at_w->atck_flag & HIT_OWNER ) dm_w->dm_at_w->hit_flag |= HR_ARMOR ;
	guard_mark_set( dm_w, at_w ) ;
}

/*==============================================================*/
/*			A hit guarded										*/
/*==============================================================*/
static void guard_set( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	dm_w->guard_cnt ++ ;
	dm_w->hit_box_no |= -0x80 ;	/* the top bit: the hit was guarded */
	at_w->hit_flag |= HR_GUARD ;
	if( at_w->atck_flag & HIT_OWNER ) dm_w->dm_at_w->hit_flag |= HR_GUARD ;
	pl_dir_set( dm_w ) ;
	pl_color_set( dm_w, 1 ) ;
	guard_char_set( dm_w, at_w ) ;
	guard_ef_set( dm_w, at_w ) ;
	dm_w->hit_flag &= 0x81 ;
}

/*==============================================================*/
/*			The guard's animation and knock-back				*/
/*==============================================================*/
static void guard_char_set( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	HIT_DATA *hd ;
	Uint8 no[2] ;

	dm_w->routine_1 = PL_NORM ;
	hd = dm_w->hit_data_adrs ;
	dm_w->dm_dir = hd->vector ;
	if( dm_w->dm_atck_no & ATCK_DIR ) dm_w->dm_dir = at_w->dir ;
	dm_w->dm_lv = hd->power ;
	dm_w->dm_guard_type = hd->atck_kind ;
	if( !( no[0] = hd->guard_time ) ){
		no[0] = 18 ;
		if( !( hd->atck_kind & 0xf0 ) && ( no[1] = hd->damage_kind & K_LV_MASK ) ){
			if( no[1] == 32 ) no[0] = 14 ;
			else if( no[1] == 16 ) no[0] = 11 ;
			else no[0] = 18 ;
		}
	}
	dm_w->guard_timer = no[0] ;
	if( ( dm_w->act_st == N_GUARD && dm_w->routine_2 == 3 ) || ( dm_w->act_st == N_AIR_GUARD && dm_w->routine_2 == 2 ) ){
		if( dm_w->pl_stance == STANCE_AIR ) return ;
		no[0] = 3 ;
		if( !dm_w->dm_at_w->fly_flag ){
			if( ( dm_w->dm_guard_type & A_GUARD_MASK ) == A_LO ) no[0] = 7 ;
			else if( ( dm_w->dm_guard_type & A_GUARD_MASK ) != A_HI ){
				if( dm_w->pl_new_lvbt & KEY_DOWN ) no[0] = 7 ;
			}
		} else if( dm_w->pl_new_lvbt & KEY_DOWN ) no[0] = 7 ;
		pl_char_set( dm_w, Char_Guard, no[0] ) ;
		return ;
	}
	dm_w->guard_back_flag = 20 ;
	if( dm_w->pl_stance == STANCE_AIR || dm_w->act_st == N_AIR_GUARD ){
		dm_back_set( dm_w, at_w ) ;
		dm_w->spd.x = dm_w->back_spd_x ;
		act_st_set( dm_w, N_AIR_GUARD ) ;
		dm_w->routine_2 = 1 ;
		dm_w->acc.x /= 256.0f ;
		if( dm_w->spd.y > 0.0f ) dm_w->spd.y = 0.0f ;
		dm_w->spd.y /= 16.0f ;
		if( !dm_w->fly_flag ){
			if( !dm_w->sjump_flag ) dm_w->air_guard_cnt ++ ;
			if( dm_w->spd.y < -0.06696428f ) dm_w->spd.y = -DOT_Y( 0.375f ) ;
		}
		pl_char_set( dm_w, Char_Air_Guard, 2 ) ;
	} else {
		guard_back_set( dm_w, at_w ) ;
		act_st_set( dm_w, N_GUARD ) ;
		dm_w->pos.y = dm_w->ground_y ;
		no[0] = 3 ;
		no[1] = 1 ;
		if( !dm_w->dm_at_w->fly_flag ){
			if( ( dm_w->dm_guard_type & A_GUARD_MASK ) == A_LO ){ no[0] = 7 ; no[1] = 1 ; }
			else if( ( dm_w->dm_guard_type & A_GUARD_MASK ) != A_HI ){
				if( dm_w->pl_new_lvbt & KEY_DOWN ){ no[0] = 7 ; no[1] = 1 ; }
			}
		} else if( dm_w->pl_new_lvbt & KEY_DOWN ){ no[0] = 7 ; no[1] = 1 ; }
		dm_w->routine_2 = no[1] ;
		pl_char_set( dm_w, Char_Guard, no[0] ) ;
	}
}

/****************************************************************/
/*			Adds to the hyper combo gauge, unless a power-up	*/
/*			stops it											*/
/****************************************************************/
void pl_gauge_add( PLAYER_WORK *pl_w, Sint16 no )
{
	if( pl_w->speed_up_flag && pl_w->routine_1 <= PL_CATCH ){
		if( SPEED_UP_CK( pl_w ) ) return ;
	}
	gauge_add( pl_w, no ) ;
}

/*==============================================================*/
static void gauge_add( PLAYER_WORK *pl_w, Sint16 no )
{
	Sint16 g ;

	if( !pl_w->routine_1 && pl_w->team_type == 3 && !pl_w->hit_box_no ) return ;
#ifdef TARGET_DC	/* a home rule: the gauge fills at twice the rate */
	if( Sys->gauge_double ) no *= 2 ;
#endif
	if( !no ) return ;
	if( no > 0 ){
		if( vs_sys_w->vs_st != VS_FIGHT ) return ;
		if( pl_w->team_type == 3 ) return ;
		if( *pl_w->gauge_level >= 5 ){
			*pl_w->gauge = 0 ;
			return ;
		}
		g = *pl_w->gauge + no ;
		if( g > 144 ){
			g -= 144 ;
			if( ++ *pl_w->gauge_level >= 5 ){
				*pl_w->gauge_level = 5 ;
				g = 0 ;
			}
			pl_se_req( pl_w, 69 ) ;
		}
	} else {
		g = *pl_w->gauge + no ;
		if( g < 0 ){
			if( *pl_w->gauge_level ){
				g += 144 ;
				if( -- *pl_w->gauge_level < 0 ){
					*pl_w->gauge_level = 0 ;
					g = 0 ;
				}
			} else g = 0 ;
		}
	}
	*pl_w->gauge = g ;
}

/****************************************************************/
/*			Uses up gauge levels								*/
/****************************************************************/
void pl_gauge_sub( PLAYER_WORK *pl_w, int no )
{
	if( ( *pl_w->gauge_level -= no ) < 0 ) *pl_w->gauge_level = 0 ;
}

/****************************************************************/
/*			The gauge for a throw								*/
/****************************************************************/
void catch_gauge_add( PLAYER_WORK *pl_w )
{
	pl_gauge_add( pl_w, 8 ) ;
}

/*==============================================================*/
/*			The gauge a hit gives, to the one hit and to the	*/
/*			attacker											*/
/*==============================================================*/
static void dm_gauge_add( PLAYER_WORK *dm_w )
{
	HIT_DATA *hd = dm_w->hit_data_adrs ;
	PLAYER_WORK *at_w = dm_w->dm_at_w ;
	Sint16 no, lv ;
	Sint8 rate ;

	no = 1 ;
	if( !at_w ) at_w = dm_w->target_w ;
	if( !( hd->atck_kind & A_HYPER ) ){
		no = hd->shc & 63 ;
		if( !no ){
			if( hd->atck_kind & 0x40 ) no = 8 ;
			else if( hd->atck_kind & A_SPECIAL ) no = 8 ;
			else no = at_w->gauge_no ;
		}
	}
	if( !( no & 0x80 ) ) gauge_add( dm_w, no ) ;
	if( ( hd->atck_kind & A_HYPER ) || ( hd->shc & 0x40 ) ) return ;
	no = hd->shc & 63 ;
	if( !no ){
		if( hd->atck_kind & 0x40 ) no = 8 ;
		else if( hd->atck_kind & A_SPECIAL ) no = 8 ;
		else no = at_w->gauge_no ;
	}
	if( !( no = no * 32 >> 5 ) ) no = 1 ;
	if( ( lv = at_w->combo_cnt ) > 10 ) lv = 10 ;
	rate = gauge_combo_tbl[lv] ;
	if( !( no = no * rate >> 5 ) ) no = 1 ;
	rate = gauge_rate_tbl[at_w->pl_type] ;
	if( !( no = no * rate >> 5 ) ) no = 1 ;
	pl_gauge_add( at_w, no ) ;
}

/*==============================================================*/
/*			The knock-back of a guard							*/
/*==============================================================*/
static void guard_back_set( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	HIT_DATA *hd ;
	Uint8 no ;

	dm_w->dm_attr = at_w->atck_attr ;
	hd = dm_w->hit_data_adrs ;
	if( dm_w->dm_attr & ATR_BACK_WORK ){
		dm_back_set( dm_w, at_w ) ;
		dm_w->spd.x = dm_w->back_spd_x ;
		return ;
	}
	if( hd->damage_kind & ( K_BACK_MAX | K_LV_MASK ) ){
		if( hd->damage_kind & K_BACK_MAX ) no = 4 ;
		else no = ( ( hd->damage_kind & K_LV_MASK ) >> 4 ) - 1 ;
	} else {
		no = 0 ;
		if( hd->power >= 5 ) no ++ ;
		if( hd->power >= 12 ) no ++ ;
	}
	dm_w->step_adrs = back_step_tbl[no] ;
	dm_w->spd.x = 0.0f ;
}

/*==============================================================*/
/*			The knock-back speed of a hit						*/
/*==============================================================*/
static void dm_back_set( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	HIT_DATA *hd ;
	Uint8 no ;

	dm_w->dm_attr = at_w->atck_attr ;
	no = dm_back_no_get( dm_w ) ;
	if( ( Uint16 )at_w->atck_attr & ATR_BACK_POS ){
		back_set_push( dm_w, no ) ;
		return ;
	}
	if( ( Uint16 )at_w->atck_attr & ATR_BACK_WORK ){
		back_set_catch( dm_w, no ) ;
		return ;
	}
	if( ( Uint16 )at_w->atck_attr & ATR_BACK_TURN ){
		back_set_turn( dm_w, no ) ;
		return ;
	}
	hd = dm_w->hit_data_adrs ;
	if( hd->damage_type & NO_VEC ){
		back_set_fix( dm_w, no ) ;
		return ;
	}
	back_set_nml( dm_w, no ) ;
}

/*==============================================================*/
/*			Away from the attacker, with its speed				*/
/*==============================================================*/
static void back_set_nml( PLAYER_WORK *dm_w, Uint8 no )
{
	PLAYER_WORK *at_w = dm_w->dm_at_w ;
	Float spd, lim ;

	if( !at_w ) at_w = dm_w->target_w ;
	spd = back_spd_tbl[no] * 1.6666666f / 256.0f ;
	dm_w->acc.x = 1.9791666f ;
	if( dm_w->dm_at_dir ){
		spd = -spd ;
		dm_w->acc.x = -dm_w->acc.x ;
	}
	if( spd < 0.0f && at_w->spd.x < 1.6666666f ) spd += at_w->spd.x ;
	else if( spd > 0.0f && at_w->spd.x > -1.6666666f ) spd += at_w->spd.x ;
	if( spd > 0.0f ) lim = DOT_X( 19.0f ) ;
	else lim = -DOT_X( 19.0f ) ;
	if( spd > 0.0f ){
		if( spd > lim ) spd = lim ;
	} else if( spd < lim ) spd = lim ;
	dm_w->back_spd_x = spd ;
}

/*==============================================================*/
/*			Away from the attacker								*/
/*==============================================================*/
static void back_set_fix( PLAYER_WORK *dm_w, Uint8 no )
{
	Float spd ;

	spd = back_spd_tbl[no] * 1.6666666f / 256.0f ;
	dm_w->acc.x = 1.9791666f ;
	if( dm_w->dm_at_dir ){
		spd = -spd ;
		dm_w->acc.x = -dm_w->acc.x ;
	}
	dm_w->back_spd_x = spd ;
}

/*==============================================================*/
/*			Away from where the attacker is						*/
/*==============================================================*/
static void back_set_push( PLAYER_WORK *dm_w, Uint8 no )
{
	PLAYER_WORK *at_w ;
	Float spd ;

	if( dm_w->dm_hit_w == dm_w->dm_at_w ) dm_w->dm_at_w->push_ng_timer = 3 ;
	at_w = dm_w->dm_at_w ;
	if( !at_w ) at_w = dm_w->target_w ;
	spd = back_spd_tbl[no] * 1.6666666f / 256.0f ;
	dm_w->acc.x = 1.9791666f ;
	if( dm_w->pos.x < at_w->pos.x ){
		spd = -spd ;
		dm_w->acc.x = -dm_w->acc.x ;
	}
	dm_w->back_spd_x = spd ;
}

/*==============================================================*/
/*			Away from where the thrower is						*/
/*==============================================================*/
static void back_set_catch( PLAYER_WORK *dm_w, Uint8 no )
{
	PLAYER_WORK *at_w ;
	Float spd ;

	if( dm_w->dm_hit_w == dm_w->dm_at_w ) dm_w->dm_at_w->push_ng_timer = 3 ;
	at_w = dm_w->dm_hit_w ;
	if( !at_w ) at_w = dm_w->target_w ;
	spd = back_spd_tbl[no] * 1.6666666f / 256.0f ;
	dm_w->acc.x = 1.9791666f ;
	if( dm_w->pos.x < at_w->pos.x ){
		spd = -spd ;
		dm_w->acc.x = -dm_w->acc.x ;
	}
	dm_w->back_spd_x = spd ;
}

/*==============================================================*/
/*			Backwards from the player's own facing				*/
/*==============================================================*/
static void back_set_turn( PLAYER_WORK *dm_w, Uint8 no )
{
	Float spd ;

	spd = back_spd_tbl[no] * 1.6666666f / 256.0f ;
	dm_w->acc.x = 1.9791666f ;
	if( !dm_w->pl_dir ){
		spd = -spd ;
		dm_w->acc.x = -dm_w->acc.x ;
	}
	dm_w->back_spd_x = spd ;
}

/*==============================================================*/
/*			A hit that knocks the player into the air			*/
/*==============================================================*/
static void dm_blow_set( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	memset( &dm_w->sjump_w, 0, sizeof( dm_w->sjump_w ) ) ;
	dm_w->ukemi_flag = 120 ;
	if( !( dm_w->recover_timer = dm_w->dm_recover_time ) ) dm_w->recover_timer = 16 ;
	dm_blow_spd_set( dm_w, at_w ) ;
	dm_elem_set( dm_w, at_w ) ;
	if( dm_w->pl_stance == STANCE_AIR ) dm_air_char_set( dm_w ) ;
	else {
		dm_char_set( dm_w ) ;
		dm_w->timer0 = 2 ;
		dm_w->pl_stance = STANCE_AIR ;
	}
}

/****************************************************************/
/*			The knock-back, each frame							*/
/****************************************************************/
void dm_back_move( register PLAYER_WORK *pl_w )
{
	HIT_DATA *hd = pl_w->hit_data_adrs ;
	PLAYER_WORK *at_w ;
	Sint8 *step ;
	register Uint8 dir ;
	register Float spd ;
	Float rate ;

	if( !hd ) return ;
	if( pl_w->dm_attr & ATR_BACK_POS ){
		back_move_push( pl_w ) ;
		return ;
	}
	if( pl_w->dm_attr & ATR_BACK_WORK ){
		back_move_wall( pl_w ) ;
		return ;
	}
	dir = pl_w->dm_at_dir ;
	if( pl_w->dm_attr & ATR_BACK_TURN ) dir = pl_w->pl_dir ^ 1 ;
	rate = 1.6666666f ;
	if( pl_w->wall_flag ) if( !( hd->damage_type & NO_ZU ) && ( pl_w->wall_flag & 1 << ( pl_w->pl_dir & 1 ) ) ){
		at_w = pl_w->dm_at_w ;
		if( !at_w ) at_w = pl_w->target_w ;
		if( !( at_w->wall_flag & 1 << ( at_w->pl_dir & 1 ) ) ){
			step = pl_w->step_adrs ;
			if( ( spd = *step++ ) > 0.0f ){
				pl_w->step_adrs = step ;
				if( !dir ) spd = -spd ;
				at_w->pos.x -= spd * rate ;
			}
			return ;
		}
	}
	step = pl_w->step_adrs ;
	if( ( spd = *step++ ) > 0.0f ){
		pl_w->step_adrs = step ;
		if( !dir ) spd = -spd ;
		pl_w->pos.x += rate * spd ;
	}
}

/*==============================================================*/
/*			Pushed away by the attack							*/
/*==============================================================*/
static void back_move_push( PLAYER_WORK *pl_w )
{
	Sint8 *step ;
	Float spd ;

	pl_at_dir_set( pl_w ) ;
	if( pl_w->dm_hit_w == pl_w->dm_at_w ) pl_w->dm_at_w->push_ng_timer = 3 ;
	step = pl_w->step_adrs ;
	if( ( spd = *step++ ) > 0.0f ){
		pl_w->step_adrs = step ;
		if( !pl_w->pl_dir ) spd = -spd ;
		pl_w->pos.x += DOT_X( spd ) ;
	}
}

/****************************************************************/
/*			Pushed away from the thrower						*/
/****************************************************************/
void back_move_catch( PLAYER_WORK *pl_w )
{
	Sint8 *step ;
	Float spd ;

	pl_at_dir_set( pl_w ) ;
	if( pl_w->dm_hit_w == pl_w->dm_at_w ) pl_w->dm_at_w->push_ng_timer = 3 ;
	step = pl_w->step_adrs ;
	if( ( spd = *step++ ) > 0.0f ){
		pl_w->step_adrs = step ;
		if( pl_w->pos.x > pl_w->dm_hit_w->pos.x ) spd = -spd ;
		pl_w->pos.x += DOT_X( spd ) ;
	}
}

/****************************************************************/
/*			The knock-back's speed, against the screen edge		*/
/****************************************************************/
void back_move_wall( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *at_w ;
	HIT_DATA *hd ;

	if( pl_w->dm_attr & ( ATR_BACK_POS | ATR_BACK_WORK ) ){
		if( pl_w->dm_hit_w == pl_w->dm_at_w ) pl_w->dm_at_w->push_ng_timer = 3 ;
	}
	if( !pl_w->wall_flag ) goto move ;
	if( !( hd = pl_w->hit_data_adrs ) ) goto move ;
	if( hd->damage_type & NO_ZU ) goto move ;
	if( !( pl_w->wall_flag & 1 << pl_w->pl_dir ) ) goto move ;
	if( !pl_w->pl_dir && pl_w->spd.x < 0.0f ) goto move ;
	if( pl_w->pl_dir && pl_w->spd.x > 0.0f ) goto move ;
	at_w = pl_w->dm_at_w ;
	if( !at_w ) at_w = pl_w->target_w ;
	if( at_w->wall_flag != ( pl_w->wall_flag ^ ( WALL_RIGHT | WALL_LEFT ) ) ) at_w->pos.x -= pl_w->spd.x ;
	else {
move :
		pl_w->pos.x += pl_w->spd.x ;
	}
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_w->spd.x * pl_w->acc.x < 0.0f ) return ;
	pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
}

/*==============================================================*/
/*			The knock-back taken by armour						*/
/*==============================================================*/
static void armor_back_set( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	Float acc = dm_w->acc.x ;

	dm_back_set( dm_w, at_w ) ;
	dm_w->armor_back_spd = dm_w->back_spd_x ;
	dm_w->armor_back_acc = dm_w->acc.x ;
	dm_w->acc.x = acc ;
	dm_w->armor_back_timer = 5 ;
}

/****************************************************************/
/*			Moves by the knock-back armour took					*/
/****************************************************************/
void armor_back_move( PLAYER_WORK *pl_w )
{
	Float keep[2] ;

	if( pl_w->routine_1 != PL_NORM ) return ;
	if( pl_w->hit_stop ) return ;
	if( !pl_w->armor_back_timer ) return ;
	pl_w->armor_back_timer -- ;
	if( !( Sint32 )pl_w->armor_back_spd ) return ;
	keep[0] = pl_w->spd.x ;
	keep[1] = pl_w->acc.x ;
	pl_w->spd.x = pl_w->armor_back_spd ;
	pl_w->acc.x = pl_w->armor_back_acc ;
	back_move_wall( pl_w ) ;
	pl_w->armor_back_spd = pl_w->spd.x ;
	pl_w->armor_back_acc = pl_w->acc.x ;
	pl_w->spd.x = keep[0] ;
	pl_w->acc.x = keep[1] ;
}

/*==============================================================*/
/*			The damage a guarded hit still does					*/
/*==============================================================*/
static void guard_ef_set( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	HIT_DATA *hd ;
	PLAYER_WORK *em ;
	Sint16 dmg ;

	if( vs_sys_w->vs_st != VS_FIGHT ) return ;
	hd = dm_w->hit_data_adrs ;
	if( dm_w->pl_type == PL_ICEMAN && ( at_w->atck_flag & HIT_SHOT ) ) return ;
	if( !( dmg = hd->damage ) ) return ;
	if( !( ( Uint16 )at_w->atck_attr & ATR_CHIP ) && !( hd->atck_kind & 0xb0 ) ) return ;
	if( !( em = dm_w->dm_at_w ) ) em = dm_w->target_w ;
	dmg = dm_calc( dm_w, em, hd, dmg ) ;
	dmg = dmg / 8.0f ;
	if( dmg < 1 ) dmg = 1 ;
#ifdef TARGET_DC	/* a home rule: a guard takes no damage, or twice as much */
	if( Sys->guard_dm_rule == 1 ) dmg = 0 ;
	else if( Sys->guard_dm_rule == 2 ) dmg *= 2 ;
#endif
	if( !( hd->atck_kind & ( A_SPECIAL | A_HYPER ) ) && ( ( Uint16 )at_w->atck_attr & ATR_CHIP ) && dm_w->vital_new == 1 ) return ;
	if( hd->hit_flag & NO_CHIP ) return ;
	if( vs_sys_work.vs_st != VS_FIGHT || !dm_w->vital_new ) return ;
#ifdef TARGET_DC	/* a home rule: no red vital */
	if( Sys->red_vital_rule != 1 )
#endif
	if( ( dm_w->vital_red -= dmg ) <= 0 ) dm_w->vital_red = 0 ;
	dm_w->combo_vital = dm_w->combo_damage = dm_w->vital_new ;
	if( ( dm_w->vital_new -= dmg ) <= 0 ){
#ifdef TARGET_DC
		if( !( hd->hit_flag & NO_KO ) && !Sys->no_ko_flag ) dm_ko_req( dm_w ) ;
#else
		if( !( hd->hit_flag & NO_KO ) ) dm_ko_req( dm_w ) ;
#endif
		else {
			dm_w->vital_new = 1 ;
			if( dm_w->vital_red <= 0 ) dm_w->vital_red = 1 ;
		}
	}
	dm_vital_disp( dm_w, dmg ) ;
#ifdef TARGET_DC
	team_ko_set( dm_w ) ;
#endif
}

/*==============================================================*/
/*			The hit mark and sound of a hit						*/
/*==============================================================*/
static void dm_hitmark_set( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	HIT_DATA *hd = dm_w->hit_data_adrs ;
	VEC3 pos ;
	Uint8 dir ;

	if( ( Sint8 )hd->hit_mark >= 0 ){
		hit_pos_get( dm_w, at_w, &pos ) ;
		dir = hd->vector ;
		if( dm_w->dm_at_dir ) dir = 32 - dir & 31 ;
		hitmark_set( &pos, dm_w->disp_dir, hd->hit_mark, dir ) ;
		if( hd->hit_flag & BLACK_COMBO ) hit_flash_set( &pos, 0 ) ;
	}
	dm_se_req( dm_w ) ;
}

/****************************************************************/
/*			The mark of two attacks that struck each other		*/
/****************************************************************/
void clash_mark_set( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	VEC3 pos ;

	hit_pos_get( dm_w, at_w, &pos ) ;
	clash_ef_set( &pos, dm_w->disp_dir ) ;
	pl_se_req( dm_w, 63 ) ;
}

/*==============================================================*/
/*			The mark and sound of a guard						*/
/*==============================================================*/
static void guard_mark_set( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	VEC3 pos ;
	Uint8 se ;

	hit_pos_get( dm_w, at_w, &pos ) ;
	dm_w->guard_pos.x = pos.x ;
	dm_w->guard_pos.y = pos.y ;
	guard_mark_ef_set( &pos, dm_w->disp_dir ) ;
	se = 37 ;
	if( METAL_CK( dm_w ) ) se = 38 ;
	pl_se_req( dm_w, se ) ;
	vib_req( dm_w->pl_no, 2 ) ;
}

/*==============================================================*/
/*			Where an attack's box meets the body's, for the		*/
/*			hit mark											*/
/*==============================================================*/
static void hit_pos_get( PLAYER_WORK *dm_w, PLAYER_WORK *at_w, VEC3 *pos )
{
	HIT_BOX *at_box = &at_w->hit_box_tbl[at_w->at_box_no] ;
	HIT_BOX *dm_box = &dm_w->hit_box_tbl[dm_w->dm_box_no] ;
	Float p[2] ;

	p[0] = at_box->x * at_w->scale.x * 1.6666666f ;
	p[1] = dm_box->x * dm_w->scale.x * 1.6666666f ;
	if( at_w->disp_dir ) p[0] = -p[0] ;
	if( dm_w->disp_dir ) p[1] = -p[1] ;
	p[0] += at_w->pos.x ;
	p[1] += dm_w->pos.x ;
	if( p[0] > p[1] ){
		p[0] -= at_box->w * at_w->scale.x * 1.6666666f ;
		p[1] += dm_box->w * dm_w->scale.x * 1.6666666f ;
	} else {
		p[0] += at_box->w * at_w->scale.x * 1.6666666f ;
		p[1] -= dm_box->w * dm_w->scale.x * 1.6666666f ;
	}
	pos->x = ( p[0] + p[1] ) / 2.0f ;
	p[0] = -( at_box->y * at_w->scale.y * 2.142857f ) + at_w->pos.y ;
	p[1] = -( dm_box->y * dm_w->scale.y * 2.142857f ) + dm_w->pos.y ;
	if( p[0] > p[1] ){
		p[0] -= at_box->h * at_w->scale.y * 2.142857f ;
		p[1] += dm_box->h * dm_w->scale.y * 2.142857f ;
	} else {
		p[0] += at_box->h * at_w->scale.y * 2.142857f ;
		p[1] -= dm_box->h * dm_w->scale.y * 2.142857f ;
	}
	pos->y = ( p[0] + p[1] ) / 2.0f ;
	pos->z = dm_w->pos.z ;
}

/****************************************************************/
/*			The hit mark of a throw's hit						*/
/****************************************************************/
void catch_hitmark( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->dm_hit_w ;
	int kind ;

	if( !em ) return ;
	kind = em->work_kind ;
	if( kind && kind != 1 ) return ;
	pl_w->hit_data_adrs = &em->hit_data_tbl[pl_w->dm_atck_no & ATCK_NO_MASK] ;
	if( pl_w->hit_data_adrs ) dm_hitmark_set( pl_w, em ) ;
}

/****************************************************************/
/*			The same, without the hit's sound					*/
/****************************************************************/
void catch_hitmark_no_se( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->dm_hit_w ;
	HIT_DATA *hd ;
	VEC3 pos ;
	int kind ;
	Uint8 dir ;

	if( !em ) return ;
	kind = em->work_kind ;
	if( kind && kind != 1 ) return ;
	pl_w->hit_data_adrs = &em->hit_data_tbl[pl_w->dm_atck_no & ATCK_NO_MASK] ;
	if( !pl_w->hit_data_adrs ) return ;
	hd = pl_w->hit_data_adrs ;
	if( ( Sint8 )hd->hit_mark < 0 ) return ;
	hit_pos_get( pl_w, em, &pos ) ;
	dir = hd->vector ;
	if( pl_w->dm_at_dir ) dir = 32 - dir & 31 ;
	hitmark_set( &pos, pl_w->disp_dir, hd->hit_mark, dir ) ;
	if( hd->hit_flag & BLACK_COMBO ) hit_flash_set( &pos, 0 ) ;
}

/* the reactions; each sets dm_no to its number */

/*==============================================================*/
static void dm_zuri( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	dm_w->dm_no = 0 ;
	dm_w->recover_ng_flag = 0 ;
	guard_back_set( dm_w, at_w ) ;
	dm_elem_set( dm_w, at_w ) ;
	dm_char_set( dm_w ) ;
}

/*==============================================================*/
/*			Dm_00 for the Abyss, with an animation by the		*/
/*			hit's strength										*/
/*==============================================================*/
static void abyss_dm_zuri( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	Uint8 no[2] ;

	dm_w->dm_no = 0 ;
	dm_w->recover_ng_flag = 0 ;
	guard_back_set( dm_w, at_w ) ;
	no[0] = 0 ;
	if( ( no[1] = dm_w->dm_kind & K_LV_MASK ) ){
		if( no[1] == 0x30 ) no[0] += 2 ;
		else if( no[1] == 0x20 ) no[0] ++ ;
		goto set ;
	} else {
		if( dm_w->dm_lv >= 5 ) no[0] ++ ;
		if( dm_w->dm_lv >= 12 ) no[0] ++ ;
	}
set :
	pl_char_set( dm_w, Char_Damage, no[0] ) ;
}

/*==============================================================*/
static void dm_buttobi( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	if( dm_air_combo_ck( dm_w ) ){
		dm_w->recover_ng_flag = 1 ;
		dm_w->dm_lv = 12 ;
		if( dm_w->dm_dir > 27 ) dm_w->dm_dir = 27 ;
		dm_w->fly_screen_flag = 1 ;
		dm_w->dm_no = 2 ;
		dm_blow_set( dm_w, at_w ) ;
	} else {
		dm_w->recover_ng_flag = 0 ;
		dm_w->dm_no = 1 ;
		dm_blow_set( dm_w, at_w ) ;
	}
}

/*==============================================================*/
static void dm_buttobi_ng( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	PLAYER_WORK *em ;

	dm_w->recover_ng_flag = 1 ;
	if( dm_w->hit_data_adrs->damage_type & FLY_SCREEN ){
		em = dm_w->dm_at_w ;
		if( !em ) em = dm_w->target_w ;
		if( em->fly_screen_flag ){
			dm_w->fly_screen_flag = 0 ;
			em->fly_screen_flag = 0 ;
		} else dm_w->fly_screen_flag = 1 ;
	}
	dm_w->dm_no = 2 ;
	dm_blow_set( dm_w, at_w ) ;
}

/*==============================================================*/
static void dm_buttobi_ukemi_ng( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	PLAYER_WORK *em ;

	dm_w->recover_ng_flag = 1 ;
	if( dm_w->hit_data_adrs->damage_type & FLY_SCREEN ){
		em = dm_w->dm_at_w ;
		if( !em ) em = dm_w->target_w ;
		if( em->fly_screen_flag ){
			dm_w->fly_screen_flag = 0 ;
			em->fly_screen_flag = 0 ;
		} else dm_w->fly_screen_flag = 1 ;
	}
	dm_w->dm_no = 2 ;
	dm_blow_set( dm_w, at_w ) ;
	dm_w->ukemi_flag = 0 ;
}

/*==============================================================*/
static void dm_snap( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	Uint8 no ;

	if( !dm_w->snap_partner ) no = at_w->atck_level ;
	else no = dm_w->snap_partner ;
	if( vs_sys_w->ko_cnt[dm_w->pl_no] >= 2 || dm_w->team_no || !no ){
		dm_w->snap_partner = 0 ;
		dm_buttobi_ng( dm_w, at_w ) ;
		return ;
	}
	dm_w->recover_ng_flag = 1 ;
	if( !dm_w->snap_partner ) dm_w->snap_partner = no ;
	dm_w->fly_screen_flag = 0 ;
	dm_w->dm_no = 12 ;
	dm_blow_set( dm_w, at_w ) ;
}

/*==============================================================*/
static void dm_slam( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	if( dm_w->routine_1 != PL_DAMAGE && !dm_w->recover_ng_flag ) dm_w->recover_ng_flag = 0 ;
	dm_down_set( dm_w, at_w ) ;
}

/*==============================================================*/
static void dm_slam_ng( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	dm_w->recover_ng_flag = 1 ;
	dm_down_set( dm_w, at_w ) ;
}

/*==============================================================*/
/*			Knocked flat										*/
/*==============================================================*/
static void dm_down_set( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	dm_w->dm_no = 17 ;
	memset( &dm_w->sjump_w, 0, sizeof( dm_w->sjump_w ) ) ;
	dm_w->ukemi_flag = 120 ;
	if( !( dm_w->recover_timer = dm_w->dm_recover_time ) ) dm_w->recover_timer = 16 ;
	dm_blow_spd_set( dm_w, at_w ) ;
	dm_elem_set( dm_w, at_w ) ;
	dm_w->pos.y = dm_w->ground_y ;
	dm_w->pl_stance = STANCE_DOWN ;
	pl_char_set( dm_w, Char_Damage, 26 ) ;
	dm_w->timer0 = 4 ;
	down_se_req( dm_w ) ;
	land_dust_set( &dm_w->pos, dm_w->pl_no ) ;
}

/*==============================================================*/
static void dm_piyo( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	PLAYER_WORK *em = dm_w->dm_at_w ;

	if( !em ) em = dm_w->target_w ;
	dm_w->dm_no = 3 ;
	dm_w->recover_ng_flag = 0 ;
	dm_w->stun_ng_flag = 0xff ;
	dm_w->pl_stance = STANCE_AIR ;
	dm_w->acc.x = 0.0f ;
	if( em->pl_dir ) dm_w->back_spd_x = 5.0f ;
	else dm_w->back_spd_x = -5.0f ;
	dm_w->back_spd_y = DOT_Y( 18.0f ) ;
	dm_w->acc.y = -0.9375f ;
	dm_w->stun_cnt = 80 ;
	dm_w->catch_ng_timer = 2 ;
	dm_w->dm_flag = 0 ;
	dm_elem_set( dm_w, at_w ) ;
	pl_char_set( dm_w, Char_Damage, 28 ) ;
}

/*==============================================================*/
static void dm_sweep( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	dm_w->dm_no = 4 ;
	memset( &dm_w->sjump_w, 0, sizeof( dm_w->sjump_w ) ) ;
	dm_w->ukemi_flag = 120 ;
	dm_w->recover_ng_flag = 0 ;
	dm_w->spd.x = 0.0f ;
	dm_w->acc.x = 0.0f ;
	dm_w->spd.y = DOT_Y( 4.0f ) ;
	dm_w->acc.y = -DOT_Y( 0.5f ) ;
	dm_w->pl_stance = STANCE_AIR ;
	dm_elem_set( dm_w, at_w ) ;
	pl_char_set( dm_w, Char_Damage, 9 ) ;
}

/*==============================================================*/
static void dm_hold( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	dm_w->dm_no = 5 ;
	dm_w->recover_ng_flag = 0 ;
	if( !dm_w->vital_new ){
		dm_ko_set( dm_w ) ;
		return ;
	}
	dm_elem_set( dm_w, at_w ) ;
	if( dm_w->pl_stance != STANCE_AIR ) dm_char_set( dm_w ) ;
	else {
		dm_air_char_set( dm_w ) ;
		dm_w->pl_stance = STANCE_AIR ;
		dm_w->timer0 = 10 ;
	}
}

/*==============================================================*/
static void dm_07( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	if( dm_w->routine_1 != PL_DAMAGE && !dm_w->recover_ng_flag ) dm_w->recover_ng_flag = 0 ;
	dm_07_set( dm_w, at_w ) ;
}

/*==============================================================*/
static void dm_08( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	dm_w->recover_ng_flag = 1 ;
	dm_07_set( dm_w, at_w ) ;
}

/*==============================================================*/
static void dm_07_set( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	dm_w->dm_no = 7 ;
	memset( &dm_w->sjump_w, 0, sizeof( dm_w->sjump_w ) ) ;
	dm_w->ukemi_flag = 120 ;
	if( !( dm_w->recover_timer = dm_w->dm_recover_time ) ) dm_w->recover_timer = 16 ;
	dm_blow_spd_set( dm_w, at_w ) ;
	dm_elem_set( dm_w, at_w ) ;
	if( dm_w->pl_stance == STANCE_AIR ) dm_air_char_set( dm_w ) ;
	else {
		dm_w->timer0 = 2 ;
		dm_w->pl_stance = STANCE_AIR ;
		dm_char_set( dm_w ) ;
	}
}

/*==============================================================*/
static void dm_freeze( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	dm_w->dm_no = 9 ;
	dm_w->recover_ng_flag = 0 ;
	dm_back_set( dm_w, at_w ) ;
	dm_elem_set( dm_w, at_w ) ;
	pl_char_set( dm_w, Char_Damage, 0 ) ;
}

/*==============================================================*/
static void dm_freeze_air( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	if( dm_w->team_no ){
		if( dm_w->pl_stance == STANCE_AIR ) dm_buttobi( dm_w, at_w ) ;
		else dm_zuri( dm_w, at_w ) ;
	} else {
		dm_w->dm_no = 23 ;
		dm_blow_spd_set( dm_w, at_w ) ;
		dm_elem_set( dm_w, at_w ) ;
		dm_w->pl_stance = STANCE_AIR ;
		pl_char_set( dm_w, Char_Damage, 0 ) ;
	}
}

/*==============================================================*/
static void dm_away( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	dm_w->recover_ng_flag = 1 ;
	dm_w->dm_no = 10 ;
	memset( &dm_w->sjump_w, 0, sizeof( dm_w->sjump_w ) ) ;
	dm_w->ukemi_flag = 120 ;
	dm_blow_spd_set( dm_w, at_w ) ;
	dm_elem_set( dm_w, at_w ) ;
	pl_char_set( dm_w, Char_Damage, 28 ) ;
}

/*==============================================================*/
static void dm_bound( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	dm_w->recover_ng_flag = 1 ;
	dm_w->dm_no = 11 ;
	dm_blow_spd_set( dm_w, at_w ) ;
	dm_elem_set( dm_w, at_w ) ;
	pl_char_set( dm_w, Char_Damage, 29 ) ;
}

/*==============================================================*/
static void dm_nop( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
}

/*==============================================================*/
static void dm_spark( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	memset( &dm_w->sjump_w, 0, sizeof( dm_w->sjump_w ) ) ;
	dm_w->ukemi_flag = 120 ;
	dm_w->dm_no = 15 ;
	dm_w->recover_ng_flag = 0 ;
	dm_w->atck_level = at_w->atck_level ;
	if( ( dm_w->dm_elem = 5 ) != dm_w->dm_elem_old ) dm_w->elem_ef_cnt = 0 ;
	dm_w->unk_228 = 5 ;
	if( dm_w->pl_stance != STANCE_AIR ){
		dm_char_set( dm_w ) ;
		return ;
	}
	dm_w->timer0 = 10 ;
	pl_char_set( dm_w, Char_Damage, 16 ) ;
}

/*==============================================================*/
static void dm_dizzy( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	dm_w->dm_no = 16 ;
	dm_w->recover_ng_flag = 0 ;
	dm_w->pl_stance = STANCE_STAND ;
	dm_w->pos.y = dm_w->ground_y ;
	guard_back_set( dm_w, at_w ) ;
	dm_w->stun_cnt = 80 ;
	dm_elem_set( dm_w, at_w ) ;
	pl_char_set( dm_w, Char_Damage, 26 ) ;
}

/*==============================================================*/
static void dm_sandbag( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	dm_w->dm_no = 19 ;
	dm_w->recover_ng_flag = 0 ;
	if( !dm_w->vital_new ){
		dm_ko_set( dm_w ) ;
		return ;
	}
	dm_elem_set( dm_w, at_w ) ;
	if( dm_w->pl_stance != STANCE_AIR ) dm_char_set( dm_w ) ;
	else {
		dm_air_char_set( dm_w ) ;
		dm_w->pl_stance = STANCE_AIR ;
	}
	dm_w->recover_timer = dm_w->dm_recover_time ;
}

/*==============================================================*/
static void dm_curse( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	if( dm_w->team_no ){
		if( dm_w->pl_stance == STANCE_AIR ) dm_buttobi( dm_w, at_w ) ;
		else dm_zuri( dm_w, at_w ) ;
	} else {
		dm_w->dm_no = 20 ;
		dm_w->recover_ng_flag = 0 ;
		dm_blow_set( dm_w, at_w ) ;
		dm_w->disp_flag = 0 ;
		dm_w->cg_shadow = 0xff ;
		dm_w->timer0 = 300 ;
		shl04_04_set( dm_w ) ;
		pl_tlocal_init( dm_w ) ;
		memset( &dm_w->pl_new_lvbt, 0, 200 ) ;	/* the input, up to gauge */
	}
}

/*==============================================================*/
/*			Struck while down									*/
/*==============================================================*/
static void dm_air( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	PLAYER_WORK *em ;
	Uint8 dir ;

	dm_w->dm_no = 24 ;
	if( !( ( Uint16 )at_w->atck_attr & ATR_DOWN_HIT_NG ) ) dm_w->down_hit_flag = 0xff ;
	dm_w->recover_ng_flag = 0 ;
	em = dm_w->dm_at_w ;
	if( !em ) em = dm_w->target_w ;
	dir = dm_w->dm_at_dir ;
	if( dm_w->pl_dir ){
		if( em->pos.x + -DOT_X( 14.0f ) > dm_w->pos.x ) dm_w->dm_at_dir ^= 1 ;
	} else if( dm_w->pos.x > em->pos.x + DOT_X( 14.0f ) ) dm_w->dm_at_dir ^= 1 ;
	dm_back_set( dm_w, at_w ) ;
	dm_w->dm_at_dir = dir ;
	dm_elem_set( dm_w, at_w ) ;
	if( !dm_w->stun_cnt ) dm_w->stun_cnt = 1 ;
	dm_w->pl_stance = STANCE_DOWN ;
	pl_char_set( dm_w, Char_Damage, 13 ) ;
}

/*==============================================================*/
/*			The animation of a hit on the ground, by its		*/
/*			strength											*/
/*==============================================================*/
static void dm_char_set( PLAYER_WORK *dm_w )
{
	Uint8 no[4] ;

	if( dm_w->pl_stance == STANCE_CROUCH ){
		no[0] = 6 ;
		no[1] = 1 ;
	} else {
		no[0] = 3 ;
		no[1] = 0 ;
		if( !( dm_w->dm_kind & K_LOW ) ){
			if( ( dm_w->dm_kind & K_LV_MASK ) || ( dm_w->hit_box_no & 4 ) == 1 ) no[0] = 0 ;
		}
	}
	dm_w->pl_stance = no[1] ;
	if( ( no[3] = dm_w->dm_kind & K_LV_MASK ) ){
		if( no[3] == 0x30 ) no[0] += 2 ;
		else if( no[3] == 0x20 ) no[0] ++ ;
		goto set ;
	} else {
		if( dm_w->dm_lv >= 5 ) no[0] ++ ;
		if( dm_w->dm_lv >= 12 ) no[0] ++ ;
	}
set :
	pl_char_set( dm_w, Char_Damage, no[0] ) ;
}

/*==============================================================*/
/*			The animation of a hit in the air					*/
/*==============================================================*/
static void dm_air_char_set( PLAYER_WORK *dm_w )
{
	Uint8 kind ;

	dm_w->char_no.b.no = 10 ;
	kind = dm_w->dm_kind & K_LV_MASK ;
	if( kind != 0x10 ){
		dm_w->char_no.b.no ++ ;
		if( kind != 0x20 ) dm_w->char_no.b.no ++ ;
	}
	pl_char_set( dm_w, Char_Damage, dm_w->char_no.b.no ) ;
	dm_w->timer0 = 10 ;
	dm_w->pl_stance = STANCE_AIR ;
}

/*==============================================================*/
/*			The effect of the attack's element					*/
/*==============================================================*/
static void dm_elem_set( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	Uint8 elem, no ;

	if( !( elem = ( Uint16 )at_w->atck_attr & ATR_ELEM_MASK ) ){
		if( !( elem = dm_w->dm_kind & K_ELEM_MASK ) ) return ;
	}
	if( elem & 8 ) elem = 1 ;
	switch( elem ){
	case 1 :
		no = 1 ;
		break ;
	case 2 :
		no = 2 ;
		break ;
	case 3 :
		no = 3 ;
		break ;
	case 6 :
		no = 4 ;
		break ;
	default :
		return ;
	}
	if( ( dm_w->dm_elem = no ) != dm_w->dm_elem_old ) dm_w->elem_ef_cnt = 0 ;
	if( dm_w->elem_ef_cnt >= 4 ) return ;
	switch( no ){
	case 1 :
		dm00_02_set( dm_w ) ;
		break ;
	case 2 :
		dm00_03_set( dm_w ) ;
		break ;
	case 3 :
		dm00_04_set( dm_w ) ;
		break ;
	case 4 :
		dm00_07_set( dm_w ) ;
		break ;
	default :
		return ;
	}
}

/*==============================================================*/
/*			The knock-back's strength, by where the players		*/
/*			stand												*/
/*==============================================================*/
static Uint8 dm_back_no_get( PLAYER_WORK *dm_w )
{
	HIT_DATA *hd = dm_w->hit_data_adrs ;
	Sint8 no = dm_w->dm_lv ;
	PLAYER_WORK *em ;
	Sint8 wt ;	/* the change by the two weights */

	if( !( hd->hit_flag & NO_WEIGHT ) ){
		em = dm_w->dm_at_w ;
		if( !em ) em = dm_w->target_w ;
		wt = weight_diff_tbl[dm_w->weight - em->weight + 7 & 15] ;
		no += wt ;
		if( no < 0 ) no = 0 ;
		if( no > 15 ) no = 15 ;
	}
	return no ;
}

/*==============================================================*/
/*			The knock-back of a hit that sends the player		*/
/*			flying												*/
/*==============================================================*/
static void dm_blow_spd_set( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	HIT_DATA *hd ;
	Uint8 no = dm_back_no_get( dm_w ) ;

	dm_w->dm_attr = at_w->atck_attr ;
	dm_w->blow_spd = back_spd_tbl[no] * 1.6666666f / 256.0f ;
	dm_w->blow_acc = 1.9791666f ;
	if( !dm_w->pl_dir ){
		dm_w->blow_spd = -dm_w->blow_spd ;
		dm_w->blow_acc = -dm_w->blow_acc ;
	}
	if( dm_w->dm_attr & ATR_BACK_POS ){
		blow_spd_push( dm_w, no ) ;
		return ;
	}
	if( dm_w->dm_attr & ATR_BACK_WORK ){
		blow_spd_catch( dm_w, no ) ;
		return ;
	}
	hd = dm_w->hit_data_adrs ;
	if( hd->damage_type & NO_VEC ){
		blow_spd_fix( dm_w, no ) ;
		return ;
	}
	blow_spd_nml( dm_w, no ) ;
}

/*==============================================================*/
/*			Flying, carried by the attacker's own speed			*/
/*==============================================================*/
static void blow_spd_nml( PLAYER_WORK *dm_w, Uint8 no )
{
	PLAYER_WORK *em = dm_w->dm_at_w ;
	VEC2 spd ;
	Float max ;

	if( !em ) em = dm_w->target_w ;
	if( em->routine_1 >= PL_CAUGHT ){
		blow_spd_fix( dm_w, no ) ;
		return ;
	}
	dm_spd_get( dm_w->dm_dir, no, &spd ) ;
	if( dm_w->dm_attr & ATR_BACK_TURN ){
		if( !dm_w->pl_dir ) spd.x = -spd.x ;
	} else if( dm_w->dm_at_dir ) spd.x = -spd.x ;
	if( spd.x ){
		if( ( spd.x < 0.0f && em->spd.x < 1.6666666f ) || ( spd.x > 0.0f && em->spd.x > -1.6666666f ) ) spd.x += em->spd.x ;
		max = spd.x > 0.0f ? DOT_X( 19.0f ) : -DOT_X( 19.0f ) ;
		if( spd.x > 0.0f ){
			if( spd.x > max ) spd.x = max ;
		} else if( spd.x < max ) spd.x = max ;
	}
	dm_w->back_spd_x = spd.x ;
	dm_w->acc.x = 0.0f ;
	if( em->sjump_flag ) dm_w->back_spd_y = spd.y + em->spd.y / 2.0f ;
	else dm_w->back_spd_y = spd.y + em->spd.y ;
	dm_w->acc.y = -DOT_Y( 0.375f ) ;
}

/*==============================================================*/
/*			Flying at the hit's own angle						*/
/*==============================================================*/
static void blow_spd_fix( PLAYER_WORK *dm_w, Uint8 no )
{
	VEC2 spd ;

	dm_spd_get( dm_w->dm_dir, no, &spd ) ;
	if( dm_w->dm_at_dir ) spd.x = -spd.x ;
	dm_w->back_spd_x = spd.x ;
	dm_w->acc.x = 0.0f ;
	dm_w->back_spd_y = spd.y ;
	dm_w->acc.y = -DOT_Y( 0.375f ) ;
}

/*==============================================================*/
/*			Pushed away by the attack							*/
/*==============================================================*/
static void blow_spd_push( PLAYER_WORK *dm_w, Uint8 no )
{
	VEC2 spd ;

	pl_at_dir_set( dm_w ) ;
	dm_spd_get( dm_w->dm_dir, no, &spd ) ;
	if( ( dm_w->disp_dir != 0 && spd.x < 0.0f ) || ( dm_w->disp_dir == 0 && spd.x > 0.0f ) ) spd.x = -spd.x ;
	dm_w->back_spd_x = spd.x ;
	dm_w->acc.x = 0.0f ;
	dm_w->back_spd_y = spd.y ;
	dm_w->acc.y = -DOT_Y( 0.375f ) ;
}

/*==============================================================*/
/*			Thrown away from the thrower						*/
/*==============================================================*/
static void blow_spd_catch( PLAYER_WORK *dm_w, Uint8 no )
{
	VEC2 spd ;

	pl_at_dir_set( dm_w ) ;
	dm_spd_get( dm_w->dm_dir, no, &spd ) ;
	if( ( dm_w->pos.x < dm_w->dm_hit_w->pos.x && spd.x < 0.0f ) || ( dm_w->pos.x > dm_w->dm_hit_w->pos.x && spd.x > 0.0f ) ) spd.x = -spd.x ;
	dm_w->back_spd_x = spd.x ;
	dm_w->acc.x = 0.0f ;
	dm_w->back_spd_y = spd.y ;
	dm_w->acc.y = -DOT_Y( 0.375f ) ;
}

/****************************************************************/
/*			The knock-back of a hit from a partner's attack		*/
/****************************************************************/
void catch_blow_spd_set( PLAYER_WORK *dm_w )
{
	Uint8 no = dm_back_no_get( dm_w ) ;
	PLAYER_WORK *em ;

	dm_w->blow_spd = back_spd_tbl[no] * 1.6666666f / 256.0f ;
	dm_w->blow_acc = 1.9791666f ;
	em = dm_w->catch_w ;
	if( !em ) em = dm_w->target_w ;
	if( em->pl_dir ){
		dm_w->blow_spd = -dm_w->blow_spd ;
		dm_w->blow_acc = -dm_w->blow_acc ;
	}
	if( dm_w->routine_1 == PL_CAUGHT || ( dm_w->hit_data_adrs->damage_type & NO_VEC ) ){
		catch_blow_spd_fix( dm_w, no ) ;
		return ;
	}
	catch_blow_spd_nml( dm_w, no ) ;
}

/*==============================================================*/
static void catch_blow_spd_nml( PLAYER_WORK *dm_w, Uint8 no )
{
	PLAYER_WORK *em = dm_w->catch_w ;
	VEC2 spd ;
	Float max ;

	dm_spd_get( dm_w->dm_dir, no, &spd ) ;
	if( em->pl_dir ) spd.x = -spd.x ;
	spd.x += em->spd.x ;
	max = spd.x > 0.0f ? DOT_X( 19.0f ) : -DOT_X( 19.0f ) ;
	if( spd.x > 0.0f ){
		if( spd.x > max ) spd.x = max ;
	} else if( spd.x < max ) spd.x = max ;
	dm_w->back_spd_x = spd.x ;
	dm_w->acc.x = 0.0f ;
	if( em->sjump_flag ) dm_w->back_spd_y = spd.y + em->spd.y / 2.0f ;
	else dm_w->back_spd_y = spd.y + em->spd.y ;
	dm_w->acc.y = -DOT_Y( 0.375f ) ;
}

/****************************************************************/
void catch_blow_spd_fix( PLAYER_WORK *dm_w, Uint8 no )
{
	PLAYER_WORK *em = dm_w->catch_w ;
	VEC2 spd ;

	dm_spd_get( dm_w->dm_dir, no, &spd ) ;
	if( em->pl_dir ) spd.x = -spd.x ;
	dm_w->back_spd_x = spd.x ;
	dm_w->acc.x = 0.0f ;
	dm_w->back_spd_y = spd.y ;
	dm_w->acc.y = -DOT_Y( 0.375f ) ;
}

/*==============================================================*/
/*			The speed of a knock-back at an angle (32 to the	*/
/*			circle) and strength								*/
/*==============================================================*/
static void dm_spd_get( Uint8 dir, Uint8 no, VEC2 *spd )
{
	Float pow = dm_pow_tbl[no] ;
	Sint16 ang = ( 40 - dir & 31 ) << 11 ;

	spd->x = pow * nlCos( ang ) / 150.0f * 1.6666666f ;
	spd->y = pow * nlSin( ang ) / 125.0f * 2.142857f ;
}

/*==============================================================*/
/*			Whether a hit in the air is a super jump's			*/
/*			juggle												*/
/*==============================================================*/
static int dm_air_combo_ck( PLAYER_WORK *dm_w )
{
	PLAYER_WORK *em ;

	if( !( dm_w->hit_data_adrs->hit_flag & AERIAL_FINISH ) || dm_w->pl_stance != STANCE_AIR ) return 0 ;
	em = dm_w->dm_at_w ;
	if( !( em->combo_kind & 1 ) || em->pl_stance != STANCE_AIR || !em->sjump_flag ) return 0 ;
	if( em->combo_cnt < 2 ) return 0 ;
	em->air_combo_cnt ++ ;
	return 1 ;
}

/****************************************************************/
/*			The knock-back of a flying player, each frame		*/
/****************************************************************/
void dm_blow_move( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	HIT_DATA *hd ;

	if( pl_w->dm_attr & ( ATR_BACK_POS | ATR_BACK_WORK ) ){
		if( pl_w->dm_hit_w == pl_w->dm_at_w ) pl_w->dm_at_w->push_ng_timer = 3 ;
	}
	pl_w->blow_spd += pl_w->blow_acc ;
	if( pl_w->blow_acc ){
		if( pl_w->blow_acc * pl_w->blow_spd > 0.0f ) pl_w->blow_spd = pl_w->blow_acc = 0.0f ;
	}
	if( pl_w->wall_flag && !pl_w->fly_screen_flag ){
		hd = pl_w->hit_data_adrs ;
		if( !( hd->damage_type & NO_ZU ) && ( pl_w->dm_dir + 1 & 31 ) > 2 ){
			em = pl_w->dm_at_w ;
			if( !em ) em = pl_w->target_w ;
			if( ( pl_w->wall_flag ^ ( WALL_RIGHT | WALL_LEFT ) ) != em->wall_flag ) em->pos.x -= pl_w->blow_spd ;
		}
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	dm_spd_y_limit( pl_w ) ;
}

/****************************************************************/
/*			Keeps a falling or flying player's speed in			*/
/*			bounds												*/
/****************************************************************/
void dm_spd_y_limit( PLAYER_WORK *pl_w )
{
	if( pl_w->spd.y > DOT_Y( 32.0f ) ) pl_w->spd.y = DOT_Y( 32.0f ) ;
	if( pl_w->spd.y < -DOT_Y( 32.0f ) ) pl_w->spd.y = -DOT_Y( 32.0f ) ;
}

/****************************************************************/
/*			Knocked out											*/
/****************************************************************/
void dm_ko_set( PLAYER_WORK *dm_w )
{
	dm_w->routine_1 = PL_DAMAGE ;
	dm_w->routine_2 = dm_w->routine_3 = 0 ;
	dm_w->wall_hit_flag = 0 ;
	dm_w->disp_flag = 1 ;
	dm_w->rot[2] = 0 ;
	dm_w->scale = dm_w->pl_scale ;
	dm_w->alpha = 1.0f ;
	act_st_set( dm_w, N_DAMAGE ) ;
	dm_w->dm_no = 2 ;
	dm_w->dm_dir = 30 ;
	dm_w->dm_lv = 5 ;
	dm_w->recover_ng_flag = 1 ;
	dm_ko_blow( dm_w, 5 ) ;
}

/****************************************************************/
/*			Knocked out, at a given angle and strength			*/
/****************************************************************/
void dm_ko_dir_set( PLAYER_WORK *dm_w, Uint8 no, Uint8 dir )
{
	dm_w->routine_1 = PL_DAMAGE ;
	dm_w->routine_2 = dm_w->routine_3 = 0 ;
	dm_w->wall_hit_flag = 0 ;
	dm_w->disp_flag = 1 ;
	dm_w->rot[2] = 0 ;
	dm_w->scale = dm_w->pl_scale ;
	dm_w->alpha = 1.0f ;
	act_st_set( dm_w, N_DAMAGE ) ;
	dm_w->dm_no = 2 ;
	dm_w->dm_dir = dir % 32 ;
	dm_w->dm_lv = no % 16 ;
	dm_w->recover_ng_flag = 1 ;
	dm_ko_blow( dm_w, no ) ;
}

/*==============================================================*/
static void dm_ko_blow( PLAYER_WORK *dm_w, Uint8 no )
{
	if( dm_w->hit_data_adrs && ( dm_w->hit_data_adrs->damage_type & NO_VEC ) ) blow_spd_fix( dm_w, no ) ;
	else blow_spd_nml( dm_w, no ) ;
	dm_w->pl_stance = STANCE_AIR ;
	pl_char_set( dm_w, Char_Damage, 10 ) ;
}

/*==============================================================*/
/*			Knocked out by a throw								*/
/*==============================================================*/
static void dm_ko_set3( PLAYER_WORK *dm_w )
{
	dm_w->routine_1 = PL_DAMAGE ;
	dm_w->routine_2 = dm_w->routine_3 = 0 ;
	dm_w->wall_hit_flag = 0 ;
	act_st_set( dm_w, N_DAMAGE ) ;
	dm_w->ukemi_flag = 0 ;
	dm_w->dm_no = 22 ;
	dm_w->dm_dir = 0 ;
	dm_w->dm_lv = 10 ;
	dm_w->recover_ng_flag = 1 ;
	blow_spd_fix( dm_w, dm_w->dm_lv ) ;
	dm_w->pl_stance = STANCE_AIR ;
	dm_w->hit_stop = 1 ;
}

/****************************************************************/
/*			A throw's hit on the thrown player					*/
/****************************************************************/
void catch_hit_set( PLAYER_WORK *pl_w )
{
	pl_w->dm_at_w = pl_w->catch_w ;
	pl_w->dm_atck_no = pl_w->atck_no ;
	dm_data_set( pl_w, pl_w->dm_at_w ) ;
	if( pl_w->dm_no == 21 ){
		pl_w->dm_no = 2 ;
		pl_w->ukemi_flag = 0 ;
	}
	if( pl_w->dm_lv < 11 ) pl_w->dm_lv = 11 ;
	if( !pl_w->vital_new && !pl_w->ko_flag ){
		pl_w->ko_flag = 0xff ;
		flash_ko_set() ;
		if( vs_sys_w->ko_cnt[pl_w->pl_no] <= 2 ) ko_se_req( pl_w, pl_w->dm_at_w ) ;
		pl_voice_req( pl_w, 0 ) ;
	}
	pl_color_set( pl_w, 1 ) ;
	dm_gauge_add( pl_w ) ;
}

/****************************************************************/
/*			The same, keeping the hit's strength				*/
/****************************************************************/
void catch_hit_set2( PLAYER_WORK *pl_w )
{
	pl_w->dm_at_w = pl_w->catch_w ;
	pl_w->dm_atck_no = pl_w->atck_no ;
	dm_data_set( pl_w, pl_w->dm_at_w ) ;
	if( pl_w->dm_no == 21 ){
		pl_w->dm_no = 2 ;
		pl_w->ukemi_flag = 0 ;
	}
	if( !pl_w->vital_new && !pl_w->ko_flag ){
		pl_w->ko_flag = 0xff ;
		flash_ko_set() ;
		if( vs_sys_w->ko_cnt[pl_w->pl_no] <= 2 ) ko_se_req( pl_w, pl_w->dm_at_w ) ;
		pl_voice_req( pl_w, 0 ) ;
	}
	pl_color_set( pl_w, 1 ) ;
	dm_gauge_add( pl_w ) ;
}

/*==============================================================*/
/*			The part of a hit's damage that can come back		*/
/*==============================================================*/
static void dm_red_set( PLAYER_WORK *dm_w, Sint16 *dmg )
{
	Float red ;
	Sint16 no ;

#ifdef TARGET_DC	/* the home rules: a shared vital, or the red part none or all */
	if( !*dmg ) return ;
	if( Sys->share_vital ) red = *dmg ;
	else {
		switch( Sys->red_vital_rule ){
		case 0 :
			if( dm_w->team_no ) return ;
			red = *dmg * 3 / 4.0f ;
			if( red < 1.0f ) red = 1.0f ;
			break ;
		case 1 :
			return ;
			break ;
		case 2 :
			red = *dmg ;
			break ;
		}
	}
#else
	if( !*dmg || dm_w->team_no ) return ;
	red = *dmg * 3 / 4.0f ;
	if( red < 1.0f ) red = 1.0f ;
#endif
	no = red ;
	if( ( dm_w->vital_red -= no ) < 0 ) dm_w->vital_red = 0 ;
	dm_w->vital_red_dm = no ;
}

/****************************************************************/
/*			Adds to the vital, or takes away					*/
/****************************************************************/
void vital_add( PLAYER_WORK *pl_w, Sint16 no )
{
	if( vs_sys_w->vs_st == VS_FIGHT && pl_w->vital_new ){
		int d = no ;
		if( d ){
			if( d < 0 ) vital_sub( pl_w, no ) ;
			else {
				if( ( pl_w->vital_new += no ) > 144 ) pl_w->vital_new = 144 ;
				if( pl_w->vital_new > pl_w->vital_red ) pl_w->vital_new = pl_w->vital_red ;
			}
#ifdef TARGET_DC
			team_ko_set( pl_w ) ;
#endif
		}
	}
}

/****************************************************************/
/*			The same, showing the damage						*/
/****************************************************************/
void vital_add_dmg( PLAYER_WORK *pl_w, Sint16 no )
{
	if( vs_sys_w->vs_st == VS_FIGHT && pl_w->vital_new ){
		int d = no ;
		if( d ){
			if( d < 0 ){
				vital_sub( pl_w, no ) ;
				dm_vital_disp( pl_w, no ) ;
			} else {
				if( ( pl_w->vital_new += no ) > 144 ) pl_w->vital_new = 144 ;
				if( pl_w->vital_new > pl_w->vital_red ) pl_w->vital_new = pl_w->vital_red ;
			}
#ifdef TARGET_DC
			team_ko_set( pl_w ) ;
#endif
		}
	}
}

/****************************************************************/
void vital_sub( PLAYER_WORK *pl_w, Sint16 dmg )
{
	int lv ;

	dmg = -dmg ;
	lv = 1 ;
	if( dmg > 13 ) lv = 3 ;
	vib_req( pl_w->pl_no, lv ) ;
	dm_red_set( pl_w, &dmg ) ;
	if( ( pl_w->vital_new -= dmg ) < 0 ){
		pl_w->vital_new = 0 ;
		if( Sys->no_ko_flag ){
			pl_w->vital_new = 1 ;
			if( pl_w->vital_red <= 0 ) pl_w->vital_red = 1 ;
		}
	}
}

/****************************************************************/
void vital_add_dmg2( PLAYER_WORK *pl_w, Sint16 no )
{
	if( vs_sys_w->vs_st != VS_FIGHT || !pl_w->vital_new ) return ;
	vital_add( pl_w, no ) ;
	if( pl_w->vital_new <= 0 ) pl_w->vital_new = pl_w->vital_red = 1 ;
	dm_vital_disp( pl_w, no ) ;
#ifdef TARGET_DC
	team_ko_set( pl_w ) ;
#endif
}

/****************************************************************/
/*			The same, knocking the player out					*/
/****************************************************************/
void hold_damage( PLAYER_WORK *pl_w, Sint16 no )
{
	if( vs_sys_w->vs_st != VS_FIGHT || !pl_w->vital_new ) return ;
	vital_add( pl_w, no ) ;
#ifdef TARGET_DC	/* nobody is knocked out in the training */
	if( pl_w->vital_new <= 0 ){
		if( Sys->no_ko_flag ){
			pl_w->vital_new = 1 ;
			if( pl_w->vital_red <= 0 ) pl_w->vital_red = 1 ;
		}
	}
#endif
	if( pl_w->vital_new <= 0 && !pl_w->ko_flag ){
		pl_w->ko_flag = 0xff ;
		dm_ko_req( pl_w ) ;
		flash_ko_set() ;
		if( vs_sys_w->ko_cnt[pl_w->pl_no] <= 2 ) ko_se_req( pl_w, 0 ) ;
		pl_voice_req( pl_w, 0 ) ;
	}
	dm_vital_disp( pl_w, no ) ;
#ifdef TARGET_DC
	team_ko_set( pl_w ) ;
#endif
}

/****************************************************************/
void catch_damage( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->catch_w ;

	em->hit_data_adrs = &pl_w->hit_data_tbl[em->atck_no & ATCK_NO_MASK] ;
	catch_hit_set( em ) ;
}

/****************************************************************/
/*			The player knocks itself out						*/
/****************************************************************/
void dm_ko_self( PLAYER_WORK *pl_w )
{
	pl_w->combo_vital = pl_w->combo_damage = pl_w->vital_new ;
	pl_w->vital_dm = 0 ;
	pl_w->dm_atck_no = pl_w->atck_no ;
	pl_w->dm_at_w = pl_w ;
	pl_w->dm_at_dir = pl_w->disp_dir ^ 1 ;
	pl_w->dm_pl_dir = pl_w->disp_dir ;
	pl_w->hit_data_adrs = &pl_w->hit_data_tbl[pl_w->dm_atck_no & ATCK_NO_MASK] ;
	catch_hit_set( pl_w ) ;
	dm_buttobi_ng( pl_w, pl_w ) ;
	pl_w->ukemi_flag = 0 ;
	pl_w->routine_1 = PL_DAMAGE ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->wall_hit_flag = 0 ;
	pl_w->disp_flag = 1 ;
	pl_w->rot[2] = 0 ;
	pl_w->scale = pl_w->pl_scale ;
	pl_w->alpha = 1.0f ;
	act_st_set( pl_w, N_DAMAGE ) ;
}

/*==============================================================*/
/*			The vital is gone: the round's end is told			*/
/*==============================================================*/
static void dm_ko_req( PLAYER_WORK *pl_w )
{
	pl_w->vital_new = pl_w->vital_red = 0 ;
	vs_sys_work.ko_hit_data = pl_w->hit_data_adrs ;
	vs_sys_work.ko_atck_no = pl_w->dm_atck_no ;
	vs_sys_work.ko_at_w = pl_w->dm_at_w ;
	if( FLAG_CK( pl_w->pl_bit, PLB0( PL_SONSON ), PLB1( PL_SERVBOT ) ) ){
		pl_w->armor_flag = 0 ;
		pl_color_set( pl_w, 1 ) ;
	}
#ifdef TARGET_DC	/* a home rule: one character a side */
	if( Sys->one_char ){
		++ vs_sys_w->ko_cnt[pl_w->pl_no] ;
		if( !pl_w->one_char_flag ) hyper_ef_set( pl_w, 3 ) ;
	} else
#endif
	if( ++ vs_sys_w->ko_cnt[pl_w->pl_no] < 3 ) hyper_ef_set( pl_w, 3 ) ;
	vib_req( pl_w->pl_no, 6 ) ;
}

/****************************************************************/
/*			A thrower knocked out lets go						*/
/****************************************************************/
void catch_ko_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	int i ;

	for( i = 0 ; i < 3 ; i ++ ){
		em = vs_sys_work.team_w[pl_w->pl_no ^ 1][i] ;
		if( em->be_flag && em->routine_1 == PL_CAUGHT && ( !em->catch_type || em->catch_type == 13 ) && pl_w == em->catch_w ) dm_ko_set3( em ) ;
	}
}

/****************************************************************/
/*			The red vital comes back, a point every 50			*/
/*			frames												*/
/****************************************************************/
void vital_red_recover( void )
{
	PLAYER_WORK *pl_w ;
	int i ;

	if( vs_sys_w->game_stop || vs_sys_w->vs_st != VS_FIGHT ) return ;
#ifdef TARGET_DC	/* the home rules: the red vital does not come back, or is shared */
	else if( !Sys->red_vital_keep && !Sys->share_vital ){
#else
	else {
#endif
		for( i = 0 ; i < 6 ; i ++ ){
			pl_w = &pl_work[i] ;
			if( !pl_w->be_flag && pl_w->vital_new < pl_w->vital_red && !pl_w->vital_red_wait && -- pl_w->vital_red_cnt < 0 ){
				pl_w->vital_red_cnt = 50 ;
				pl_w->vital_new ++ ;
			}
		}
	}
}

#ifdef TARGET_DC
/****************************************************************/
/*			The training's vital fills again after a hit; a		*/
/*			home rule drains it with time						*/
/****************************************************************/
void vital_drain_move( void )
{
	PLAYER_WORK *pl_w ;
	int i ;

	if( vs_sys_w->game_stop || vs_sys_w->vs_st != VS_FIGHT ) return ;
	if( Sys->no_ko_flag ){
		for( i = 0 ; i < 2 ; i ++ ){
			pl_w = vs_sys_w->team_w[i][0] ;
			if( pl_w->vital_fill_cnt && pl_w->be_flag && pl_w->routine_1 == PL_NORM && -- pl_w->vital_fill_cnt <= 0 ){
				pl_w->vital_fill_cnt = 1 ;
				if( ( pl_w->vital_new += 2 ) > 144 ) pl_w->vital_new = 144 ;
				if( ( pl_w->vital_red += 2 ) > 144 ) pl_w->vital_red = 144 ;
				team_ko_set( pl_w ) ;
			}
		}
	}
	if( Sys->vital_drain ){
		for( i = 0 ; i < 2 ; i ++ ){
			pl_w = vs_sys_w->team_w[i][0] ;
			if( -- pl_w->drain_timer < 0 && pl_w->vital_new > 0 ){
				pl_w->drain_timer = 100 ;
				if( -- pl_w->vital_new < 1 ) pl_w->vital_new = 1 ;
				if( -- pl_w->vital_red < 1 ) pl_w->vital_red = 1 ;
				team_ko_set( pl_w ) ;
			}
		}
	}
}
#endif

/****************************************************************/
/*			The point player's partners share its vital, and	*/
/*			fall with it										*/
/****************************************************************/
void team_ko_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	int i ;

	if( !Sys->share_vital ) return ;
	for( i = 0 ; i < 2 ; i ++ ){
		em = vs_sys_w->team_w[pl_w->pl_no][( pl_w->team_no + i + 1 ) % 3] ;
		em->vital_new = pl_w->vital_new ;
		em->vital_red = pl_w->vital_red ;
		if( !em->vital_new && !em->ko_flag && em->be_flag && em->routine_1 != PL_CAUGHT ){
			if( em->routine_1 == PL_NORM && !em->hit_box_no ){
				em->dm_hit_w = pl_w->dm_hit_w ;
				em->dm_atck_no = pl_w->dm_atck_no ;
				em->dm_no = pl_w->dm_no ;
				em->dm_at_w = pl_w->dm_at_w ;
				em->dm_at_dir = pl_w->dm_at_w->pl_dir ;
				em->dm_pl_dir = pl_w->pl_dir ;
				em->hit_box_no = pl_w->hit_box_no ;
			}
			if( em->pl_stance == STANCE_DOWN ){
				em->pl_stance = STANCE_CROUCH ;
				pl_dir_set( em ) ;
			}
			if( em->dm_no != 2 ){
				em->dm_dir = 30 ;
				em->dm_lv = 12 ;
				if( vs_sys_work.vs_st >= VS_WIN ) em->dm_lv = 8 ;
			}
			em->recover_timer = 10 ;
			em->dm_no = 2 ;
			em->snap_partner = 0 ;
			em->ko_flag = 0xff ;
			pl_voice_req( em, 0 ) ;
			em->hit_data_adrs = &em->dm_hit_w->hit_data_tbl[em->dm_atck_no & ATCK_NO_MASK] ;
			dm_tbl[em->pl_stance][em->dm_no]( em, em->dm_hit_w ) ;
			em->routine_1 = PL_DAMAGE ;
			em->routine_2 = em->routine_3 = 0 ;
			em->wall_hit_flag = 0 ;
			em->disp_flag = 1 ;
			em->rot[2] = 0 ;
			em->scale = em->pl_scale ;
			em->alpha = 1.0f ;
			act_st_set( em, N_DAMAGE ) ;
		}
	}
}

/*==============================================================*/
/*			The vital the bar shows falling						*/
/*==============================================================*/
static void dm_vital_disp( PLAYER_WORK *pl_w, Sint16 dmg )
{
#ifdef TARGET_DC
	if( dmg ){
		pl_w->vital_dm = pl_w->combo_damage - pl_w->vital_new ;
		pl_w->combo_damage = pl_w->vital_new ;
	}
	if( Sys->no_ko_flag ) pl_w->vital_fill_cnt = 50 ;	/* the training's vital fills again */
#else
	if( !dmg ) return ;
	pl_w->vital_dm = pl_w->combo_damage - pl_w->vital_new ;
	pl_w->combo_damage = pl_w->vital_new ;
#endif
}

/****************************************************************/
/*			Shakes a knocked-out player during the hit stop		*/
/****************************************************************/
void dm_ko_shake( PLAYER_WORK *pl_w )
{
	Float x ;
	Sint8 bit ;

	if( pl_w->routine_1 != PL_DAMAGE ) return ;
	x = 1.6666666f ;
	bit = pl_w->hit_stop & 31 ;
	if( !( 0xCCCCCCCC & 1 << bit ) ) x = -1.6666666f ;
	if( pl_w->disp_dir ) x = -x ;
	pl_w->pos.x += x ;
}

/****************************************************************/
/*			Counts down the time the red vital is held			*/
/****************************************************************/
void vital_red_timer( void )
{
	PLAYER_WORK *pl_w ;
	int i ;

	if( vs_sys_w->game_stop || vs_sys_w->vs_st != VS_FIGHT ) return ;
	else {
		for( i = 0 ; i < 6 ; i ++ ){
			pl_w = &pl_work[i] ;
			if( !pl_w->be_flag && pl_w->vital_red_wait ) pl_w->vital_red_wait -- ;
		}
	}
}

/****************************************************************/
/*			The timed attack power-up							*/
/****************************************************************/
void atck_plus_set( PLAYER_WORK *pl_w, int time, int pow )
{
	if( pl_w->atck_plus_timer = time ){
		pl_w->atck_plus = pow ;
		pl_w->def_plus_timer = 0 ;
		pl_w->def_plus = 0 ;
		pl_w->speed_plus_timer = 0 ;
		pl_w->speed_up_flag = 0 ;
	} else pl_w->atck_plus = 0 ;
}

/****************************************************************/
/*			The timed defence power-up							*/
/****************************************************************/
void def_plus_set( PLAYER_WORK *pl_w, int time, int pow )
{
	if( pl_w->def_plus_timer = time ){
		pl_w->def_plus = pow ;
		pl_w->atck_plus_timer = 0 ;
		pl_w->atck_plus = 0 ;
		pl_w->speed_plus_timer = 0 ;
		pl_w->speed_up_flag = 0 ;
	} else pl_w->def_plus = 0 ;
}

/****************************************************************/
void armor_set( PLAYER_WORK *pl_w, Sint16 time, Sint8 a, Uint8 b )
{
	pl_w->armor_timer = time ;
	pl_w->armor_hit_flag = 0 ;
	pl_w->armor_cnt = 0 ;
	if( !a && !b ){
		armor_init( pl_w ) ;
		return ;
	}
	pl_w->armor_time = a ;
	pl_w->armor_max = b ;
}

/****************************************************************/
/*			The timed speed-up									*/
/****************************************************************/
void speed_up_set( PLAYER_WORK *pl_w, int time )
{
	if( pl_w->speed_plus_timer = time ){
		pl_w->speed_up_flag = 1 ;
		pl_w->atck_plus_timer = 0 ;
		pl_w->atck_plus = 0 ;
		pl_w->def_plus_timer = 0 ;
		pl_w->def_plus = 0 ;
	} else pl_w->speed_up_flag = 0 ;
}

/*==============================================================*/
/*			The Abyss: no reactions, only damage and the hit	*/
/*			mark												*/
/*==============================================================*/
static void abyss_damage( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	HIT_DATA *hd = dm_w->hit_data_adrs ;
	VEC3 pos ;
	Uint8 no ;

	dm_w->hit_stop = at_w->hit_stop = hd->hit_stop ;
	if( at_w->atck_flag & HIT_OWNER ) dm_w->dm_at_w->hit_stop = hd->hit_stop ;
	dm_w->guard_cnt = 0 ;
	combo_hit2( dm_w, at_w, hd ) ;
	dm_data_set( dm_w, at_w ) ;
	dm_gauge_add( dm_w ) ;
	dm_w->armor_wait = dm_w->armor_time ;
	no = hd->abyss_jug ? hd->abyss_jug : hd->jug ;
	dm_w->armor_cnt += no ;
	if( ( dm_w->vital_new && !dm_w->routine_1 ) || dm_w->pl_stance == STANCE_AIR ){
		if( !dm_w->vital_new ) dm_w->vital_new = 1 ;
	} else {
		dm_w->armor_cnt = 0 ;
		dm_w->pl_stance = STANCE_STAND ;
		dm_w->dm_no = 0 ;
		abyss_dm_zuri( dm_w, at_w ) ;
		dm_w->routine_1 = PL_DAMAGE ;
		dm_w->routine_2 = dm_w->routine_3 = 0 ;
		dm_w->wall_hit_flag = 0 ;
		dm_w->disp_flag = 1 ;
		dm_w->rot[2] = 0 ;
		dm_w->scale = dm_w->pl_scale ;
		dm_w->alpha = 1.0f ;
		act_st_set( dm_w, N_DAMAGE ) ;
		dm_w->fly_screen_flag = 0 ;
		dm_w->recover_ng_flag = 0 ;
		if( !( Rnd() & 1 ) ) dm_voice_req( dm_w ) ;
	}
	if( !dm_w->vital_new ){
		if( !dm_w->ko_flag ){
			dm_w->ko_flag = 0xff ;
			pl_voice_req( dm_w, 0 ) ;
		}
		if( dm_w->pl_type == PL_ABYSS1 || dm_w->pl_type == PL_ABYSS2 ) dm_w->pl_tbl[TBL_EX0]( dm_w ) ;
	}
	if( dm_w->pl_type == PL_ABYSS1 ) dm_hitmark_set( dm_w, at_w ) ;
	else {
		hit_pos_get( dm_w, at_w, &pos ) ;
		if( no = dm_w->dm_kind & K_LV_MASK ){
			if( no == 0x30 ) no = 2 ;
			else if( no == 0x20 ) no = 1 ;
			else no = 0 ;
		} else {
			if( dm_w->dm_lv >= 5 ) no ++ ;
			if( dm_w->dm_lv >= 12 ) no ++ ;
		}
		if( dm_w->pl_type == PL_ABYSS2 ) pls19_00_set2( dm_w, &pos, hd->hit_mark % 3 ) ;
		else if( dm_w->dm_hit_w->em_w == dm_w ) pls1a_00_set2( dm_w, &pos, no ) ;
	}
	dm_se_req( dm_w ) ;
}

/*==============================================================*/
/*			The armour, each frame: a hit on it shakes the		*/
/*			player												*/
/*==============================================================*/
static void armor_hit_wait( PLAYER_WORK *pl_w )
{
	if( pl_w->hit_box_no && pl_w->routine_1 == PL_NORM && pl_w->vital_new ){
		if( !( pl_w->hit_box_no & 0xc0 ) ){
			pl_w->armor_hit_flag ++ ;
			pl_w->hit_stop += 6 ;
			armor_hit_shake( pl_w ) ;
			return ;
		}
	}
	if( pl_w->armor_wait && !-- pl_w->armor_wait ) pl_w->armor_cnt = 0 ;
}

/*==============================================================*/
/*			The armour shaking, to one side and the other on	*/
/*			alternate frames									*/
/*==============================================================*/
static void armor_hit_shake( PLAYER_WORK *pl_w )
{
	static Sint16 armor_shake[2] = { 4, -4 } ;
	Uint8 side = Sys->play_timer & 1 ;

	pl_w->pos.x += DOT_X( armor_shake[side] ) ;
	if( !( pl_w->hit_stop & 1 ) ){
		if( pl_w->hit_stop & 2 ) pl_color_set( pl_w, 3 ) ;
		else pl_color_set( pl_w, 1 ) ;
	}
	if( !pl_w->hit_stop ){
		pl_color_set( pl_w, 1 ) ;
		pl_w->armor_hit_flag = 0 ;
	}
}

/****************************************************************/
void armor_move( PLAYER_WORK *pl_w )
{
	static void ( *armor_hit_tbl[2] )( PLAYER_WORK *pl_w ) = { armor_hit_wait, armor_hit_shake } ;

	armor_hit_tbl[pl_w->armor_hit_flag]( pl_w ) ;
}

/*==============================================================*/
/*			Whether a hit gets through the armour				*/
/*==============================================================*/
static int armor_ck( PLAYER_WORK *pl_w )
{
	HIT_DATA *hd ;

	if( pl_w->routine_1 || pl_w->pl_stance == STANCE_AIR || pl_w->pl_stance == STANCE_DOWN || pl_w->act_st == N_DIZZY || !pl_w->vital_new ) return 1 ;
	pl_w->armor_wait = pl_w->armor_time ;
	hd = pl_w->hit_data_adrs ;
	pl_w->armor_cnt += hd->jug ;
	if( pl_w->armor_cnt < pl_w->armor_max ) return 0 ;
	pl_w->armor_cnt = 0 ;
	pl_w->armor_hit_flag = 0 ;
	pl_w->armor_back_spd = 0.0f ;
	pl_w->armor_back_acc = 0.0f ;
	return 1 ;
}

/****************************************************************/
/*			The character's own armour							*/
/****************************************************************/
void armor_init( PLAYER_WORK *pl_w )
{
	pl_w->armor_hit_flag = 0 ;
	pl_w->armor_cnt = 0 ;
	pl_w->armor_time = armor_tbl[pl_w->pl_type][0] ;
	pl_w->armor_max = armor_tbl[pl_w->pl_type][1] ;
}

/*==============================================================*/
/*			The sound of a knock-out							*/
/*==============================================================*/
static void ko_se_req( PLAYER_WORK *dm_w, PLAYER_WORK *at_w )
{
	PLAYER_WORK *em = dm_w->dm_at_w ;

	if( em && em->pl_type == PL_AKUMA && dm_w->dm_atck_no == 34 ){
		pl_voice_req( em, 32 ) ;
		return ;
	}
	pl_se_req( em, 67 ) ;
}

/*==============================================================*/
/*			Clears the knock-down marks once the round is		*/
/*			decided												*/
/*==============================================================*/
static void dm_fly_screen_clr( PLAYER_WORK *dm_w )
{
	PLAYER_WORK **tw = vs_sys_w->team_w[dm_w->pl_no ^ 1] ;
	PLAYER_WORK *em0 = tw[0], *em1 = tw[1], *em2 = tw[2] ;

	if( em0->routine_1 < PL_DAMAGE && em1->routine_1 < PL_DAMAGE && em2->routine_1 < PL_DAMAGE && !vs_sys_w->team_hyper_flag && !dm_w->dm_at_w->team_no ) return ;
	dm_w->fly_screen_flag = 0 ;
	em0->fly_screen_flag = 0 ;
	em1->fly_screen_flag = 0 ;
	em2->fly_screen_flag = 0 ;
}

/****************************************************************/
Sint8 *guard_tbl_get( PLAYER_WORK *pl_w, HIT_DATA *hd )
{
	if( hd->atck_kind & A_HYPER ) return guard_hyper_tbl ;
	return guard_tbl ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pl_dmg.c" End of File	*/

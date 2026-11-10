/* The routines the character modules share: animation, commands, acts, throws, damage, combos,
   sounds and the helpers of o_sub1.c. player.h includes it. */
#ifndef PL_SUB_H
#define PL_SUB_H

/* sprite */
extern int sprite_tex_set( PLAYER_WORK *w );

extern Sint8 pl_char_move( PLAYER_WORK *pl_w );
extern Sint8 ef_char_move( PLAYER_WORK *ef_w );
extern Uint16 *get_pl_col_adrs( PLAYER_WORK *pl_w );
extern void cg_se_req( PLAYER_WORK *pl_w );
extern int lz_unpack( Sint8 *src, int size, Uint8 *dst );
extern void lz_unpack16( Sint16 *src, Uint16 *dst );
extern Uint32 Rnd( void );
extern void act_st_set( PLAYER_WORK *pl_w, Uint8 act );

extern Uint8 pl_v_combi_ck( PLAYER_WORK *pl_w );
extern Uint8 pl_assist_ck( PLAYER_WORK *pl_w );
extern Uint8 pl_change_ck( PLAYER_WORK *pl_w );
extern Uint8 pl_vcounter_ck( PLAYER_WORK *pl_w );
extern int pl_snapback_ck( PLAYER_WORK *pl_w );
extern Uint8 pl_chouhatsu_ck( PLAYER_WORK *pl_w, Uint8 no );
extern Uint8 com_nml_ck( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw );
extern Uint8 com_seq_ck( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw );
extern Uint8 com_tame_ck( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw );
extern void com_sw_set( PLAYER_WORK *pl_w, COM_WORK *cw );
extern Uint8 pl_dash_ck( PLAYER_WORK *pl_w );
extern Uint8 pl_sjump_ck( PLAYER_WORK *pl_w );
extern Uint8 pl_air_dash_ck( PLAYER_WORK *pl_w, COM_WORK *cw );
extern void pl_se_req( PLAYER_WORK *pl_w, Uint8 no );
extern int pl_far_ck( PLAYER_WORK *pl_w, Float range );
extern int pl_far_xy_ck( PLAYER_WORK *pl_w, Float range_x, Float range_y );
extern void pl_brake_set( PLAYER_WORK *pl_w );
extern void pl_smoke_set( PLAYER_WORK *pl_w );
extern void pl_brake_stop( PLAYER_WORK *pl_w );
extern void pl_stand_ret( PLAYER_WORK *pl_w );
extern void pl_jump_hit_ck( PLAYER_WORK *pl_w );
extern void pl_sjump_ctrl( PLAYER_WORK *pl_w );
extern void pl_jump_move( PLAYER_WORK *pl_w );
extern void pl_sjump_top_ck( PLAYER_WORK *pl_w );
extern Uint8 pl_land_ck( PLAYER_WORK *pl_w );
extern void pl_land_set( PLAYER_WORK *pl_w );
extern void pl_jump_ret( PLAYER_WORK *pl_w );
extern void pl_land_ef_set( PLAYER_WORK *pl_w );
/* s_pl28 was built against an older prototype with an unsigned amount */
#ifdef PL_GAUGE_ADD_OLD
extern void pl_gauge_add( PLAYER_WORK *pl_w, Uint16 no );
#else
extern void pl_gauge_add( PLAYER_WORK *pl_w, Sint16 no );
#endif
extern void pl_dir_set( PLAYER_WORK *pl_w );
extern void pl_sp_ef_set( PLAYER_WORK *pl_w );
extern void pl_voice_req( PLAYER_WORK *pl_w, Uint8 no );
extern void pl04_voice_req( PLAYER_WORK *ew, PLAYER_WORK *pl_w, Uint8 no );
extern void pl_air_set( PLAYER_WORK *pl_w );
extern void dir_move( PLAYER_WORK *pl_w, Sint16 spd );
extern int pl_win_end_ck( PLAYER_WORK *pl_w );
/* the callers saw a Uint16 here, though the routine itself reads an int */
extern void pl_stop_stand_ret( PLAYER_WORK *pl_w );
extern void catch_pos_set( PLAYER_WORK *em, PLAYER_WORK *pl_w );
extern void catch_se_req( PLAYER_WORK *pl_w );
extern void catch_damage( PLAYER_WORK *pl_w );
extern PLAYER_WORK *pl_catch_ck( PLAYER_WORK *pl_w );	/* the opponent, if a throw can start */
extern void catch_gauge_add( PLAYER_WORK *pl_w );
extern int pl_sp_catch_ck( PLAYER_WORK *pl_w );
extern void pl_snap_flash( PLAYER_WORK *pl_w, VEC3 *ofs );
/* pl_sub.c defines it with no a Sint8 (as pl_hyper_flash); its callers were built against a
   prototype with no an int */
#ifdef PL_HYPER_START_NEW
extern void pl_hyper_start( PLAYER_WORK *pl_w, VEC3 *ofs, Sint8 no );
#else
extern void pl_hyper_start( PLAYER_WORK *pl_w, VEC3 *ofs, int no );
#endif
extern void pl_jump_ret2( PLAYER_WORK *pl_w );
/* pl_char.c defines it with no an int and col a Uint16 (as pl_color_trans); its callers were built
   against an older prototype with no a Uint16 and col an int */
#ifdef EF_COLOR_SET_NEW
extern void ef_color_set( PLAYER_WORK *pl_w, int no, Uint16 col, int cnt );
#else
extern void ef_color_set( PLAYER_WORK *pl_w, Uint16 no, int col, int cnt );
#endif
extern int pl_be_cnt( PLAYER_WORK *pl_w ) ;
extern void pl_catch_set( PLAYER_WORK *pl_w, PLAYER_WORK *em );
extern int lever_gacha_ck( PLAYER_WORK *pl_w );
extern int sw_renda_ck( PLAYER_WORK *pl_w );
extern int lever_sw_renda_ck( PLAYER_WORK *pl_w );
extern void pl_sp_catch_set( PLAYER_WORK *pl_w, PLAYER_WORK *em_w );
extern void hit_se_req( PLAYER_WORK *pl_w, Uint8 no );
extern void pl_damage( PLAYER_WORK *dm_w, PLAYER_WORK *at_w );
extern void combo_hit( PLAYER_WORK *pl_w, PLAYER_WORK *em_w );
extern void combo_hit2( PLAYER_WORK *dm_w, PLAYER_WORK *ew, HIT_DATA *hd );
extern void combo_dm_clr( PLAYER_WORK *pl_w );
extern void first_atck_catch( PLAYER_WORK *pl_w );
extern void hit_score_add( PLAYER_WORK *pl_w, HIT_DATA *hd );
extern void score_add( PLAYER_WORK *pl_w, Uint32 hi, Uint32 lo );
extern void score_bcd_add( Uint32 *score, Uint32 hi, Uint32 lo );
extern void bonus_add( Uint32 *hi, Uint32 *lo );
extern void vital_bonus( Uint32 *hi, Uint32 *lo );
extern void time_bonus( Uint32 *hi, Uint32 *lo );
extern void unk_9a_bonus( Uint32 *hi, Uint32 *lo );
extern void pl_fly( PLAYER_WORK *pl_w );
extern void pl_fly_start( PLAYER_WORK *pl_w );
extern Uint8 pl_guard_req_ck( PLAYER_WORK *pl_w );
extern void pl_ef_catch_set( PLAYER_WORK *pl_w, PLAYER_WORK *em_w );
extern void catch_em_pos_set0( PLAYER_WORK *em, PLAYER_WORK *pl_w );
extern void pl_catch_pass( PLAYER_WORK *pl_w, PLAYER_WORK *ew );
extern Uint8 pl_air_dash_sw_ck( PLAYER_WORK *pl_w );
extern void hold_renda_set( PLAYER_WORK *pl_w, Sint16 no );
extern void catch_renda_set( PLAYER_WORK *pl_w, Sint16 no );
extern int hold_renda_ck( PLAYER_WORK *pl_w );
extern int hold_renda_move_ck( PLAYER_WORK *pl_w );
extern int catch_renda_ck( PLAYER_WORK *pl_w );
extern Uint8 sw_now_ck( PLAYER_WORK *pl_w, Uint16 sw, Uint16 *now, Uint8 no );
extern Uint8 sp_ok_hyper_ck( PLAYER_WORK *pl_w );
extern Uint8 sp_ok_ck( PLAYER_WORK *pl_w );
extern void dir_spd_move( PLAYER_WORK *pl_w, Sint16 no );
extern int ground_ck( PLAYER_WORK *pl_w );
extern void pl_color_trans( PLAYER_WORK *pl_w, int no, Uint16 col, int cnt );
#if defined( PL_CHAR_SET2_OLD )	/* as some older modules declared it: all ints, which the caller extends */
extern void pl_char_set2( PLAYER_WORK *pl_w, int group, int no, int frame );
#elif !defined( PL_CHAR_SET2_BYTE )
extern void pl_char_set2( PLAYER_WORK *pl_w, Uint8 group, Sint8 no, int frame );
#endif
extern void fall_se_req( PLAYER_WORK *pl_w );
extern void nage_se_req( PLAYER_WORK *pl_w );
extern void pl_sjump_ret( PLAYER_WORK *pl_w );
extern void vital_add( PLAYER_WORK *pl_w, Sint16 no );
extern void hold_damage( PLAYER_WORK *pl_w, Sint16 no );
extern void pl_hyper_flash3( PLAYER_WORK *pl_w, VEC3 *ofs );
extern int scr_in_x_ck( PLAYER_WORK *pl_w );
extern int scr_in_y_ck( PLAYER_WORK *pl_w );
extern void dm_blow_move( PLAYER_WORK *pl_w );
extern void dm_blow_char_set( PLAYER_WORK *pl_w );
extern void dm_fall_char_set( PLAYER_WORK *pl_w );
extern void catch_hit_set( PLAYER_WORK *pl_w );
extern void catch_hit_set2( PLAYER_WORK *pl_w );
extern void catch_blow_spd_set( PLAYER_WORK *dm_w );
extern void catch_blow_spd_fix( PLAYER_WORK *dm_w, Uint8 no );
extern void down_se_req( PLAYER_WORK *pl_w );
extern void land_se_req( PLAYER_WORK *pl_w );
extern void pl_sw_set( PLAYER_WORK *pl_w );
extern void pl_wait_com_ck( PLAYER_WORK *pl_w );
extern void armor_init( PLAYER_WORK *pl_w );
extern void armor_move( PLAYER_WORK *pl_w );
extern void armor_back_move( PLAYER_WORK *pl_w );
extern void dm_ko_shake( PLAYER_WORK *pl_w );
extern void pl_frame_jump( PLAYER_WORK *pl_w );
extern void combo_reset( PLAYER_WORK *pl_w );
extern void pl_walk_spd_set( PLAYER_WORK *pl_w );
extern void dm_back_move( PLAYER_WORK *pl_w );
extern void catch_em_pos_set1( PLAYER_WORK *em, PLAYER_WORK *pl_w );
extern Uint8 pl_land_ck3( PLAYER_WORK *pl_w );
extern void vital_add_dmg2( PLAYER_WORK *pl_w, Sint16 no );
extern Uint8 pl_land_y_ck( PLAYER_WORK *pl_w, Float y );
extern Uint8 com_renda_ck( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw );
extern Uint8 pl_turn_ck( PLAYER_WORK *pl_w );
extern Uint8 pl_crouch_ck( PLAYER_WORK *pl_w );
extern void armor_set( PLAYER_WORK *pl_w, Sint16 a, Sint8 b, Uint8 c );

extern void pl_boss_change( int no );
extern void pl_hyper_flash( PLAYER_WORK *pl_w, VEC3 *ofs, Sint8 no );
extern void pl_wall_ck( PLAYER_WORK *pl_w );
extern void pl_rot_ofs_set( PLAYER_WORK *pl_w );
extern Uint8 com_rot_ck( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw );	/* turns of the stick */
extern PLAYER_WORK *pl_catch_ck2( PLAYER_WORK *pl_w );	/* the opponent, if a special throw can start */

extern void pl_smoke_set2( PLAYER_WORK *pl_w, int no );
extern void vital_add_dmg( PLAYER_WORK *pl_w, Sint16 no );
extern void def_plus_set( PLAYER_WORK *pl_w, int time, int pow );
extern void combo_add( PLAYER_WORK *dm_w, PLAYER_WORK *at_w, Sint16 cnt );
extern void pl_jump_spd_set( PLAYER_WORK *pl_w );
extern Uint8 sw_on_ck( PLAYER_WORK *pl_w, Uint16 sw, Uint16 *now );
extern void combo_set( PLAYER_WORK *dm_w, PLAYER_WORK *at_w, Sint16 cnt );
extern void pl_catch_dir_set( PLAYER_WORK *pl_w );
extern void atck_plus_set( PLAYER_WORK *pl_w, int time, int b );
extern void pl_atck_gauge_add( PLAYER_WORK *pl_w );
extern Uint8 com_hold_ck( PLAYER_WORK *pl_w, Uint16 *com, COM_WORK *cw );	/* a button held and let go */
extern void dm_ko_self( PLAYER_WORK *pl_w );
extern int scr_in_ck( PLAYER_WORK *pl_w );

/* pl_act */
extern int pl_walk_ck( PLAYER_WORK *pl_w, int now );
extern int pl_cmd_atck_ck( PLAYER_WORK *pl_w );
extern Uint8 pl_jump_ck( PLAYER_WORK *pl_w );
extern int pl_air_jump_ck( PLAYER_WORK *pl_w );
extern Uint8 pl_wall_jump_ck( PLAYER_WORK *pl_w );
extern Uint8 pl_catch_escape( PLAYER_WORK *pl_w );
extern void pl_at_dir_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *pl_tbl_catch_ck( PLAYER_WORK *pl_w );

/* pl_main */
extern void player_move( PLAYER_WORK *pl_w );
extern void pl_em_dist_set( PLAYER_WORK *pl_w );
extern void pl_delay_hyper_ck( PLAYER_WORK *pl_w );
extern void pl_stage_end_ck( PLAYER_WORK *pl_w );
extern void pl_renda_ck( PLAYER_WORK *pl_w );
extern void pl_wall_wide_ck( PLAYER_WORK *pl_w );
extern Uint8 pl_sjump_set( PLAYER_WORK *pl_w );
extern Uint8 pl_dash_set( PLAYER_WORK *pl_w );
extern int pl_stand_ck( PLAYER_WORK *pl_w );
extern Uint8 pl_fall_ck( PLAYER_WORK *pl_w );
extern void pl_step_spd_set( PLAYER_WORK *pl_w );
extern int pl_crouch_end_ck( PLAYER_WORK *pl_w );
extern Uint8 pl_crouch_now_ck( PLAYER_WORK *pl_w );
extern Uint8 pl_guard_req_now_ck( PLAYER_WORK *pl_w );
extern void sjump_se_req( PLAYER_WORK *pl_w );
extern void pl_fall_limit( PLAYER_WORK *pl_w );
extern Uint8 pl_guard_dir_ck( PLAYER_WORK *pl_w );
extern void back_move_wall( PLAYER_WORK *pl_w );
extern void pl_appear_set( PLAYER_WORK *pl_w );
extern void dm_spd_y_limit( PLAYER_WORK *pl_w );
extern void pl_change_out( PLAYER_WORK *pl_w );
extern void pl_change_in_set( PLAYER_WORK *pl_w );
extern void pl_assist_in_set( PLAYER_WORK *pl_w );
extern void dm_ko_set( PLAYER_WORK *pl_w );
extern void pl_lever_drift( PLAYER_WORK *pl_w );

/* pl_dmg: the hit mark of a work struck in its body */
extern void catch_hitmark( PLAYER_WORK *pl_w );

extern int scr_top_ck( PLAYER_WORK *pl_w );
extern void pl19_voice_req( PLAYER_WORK *pl_w, Uint8 no );

#endif

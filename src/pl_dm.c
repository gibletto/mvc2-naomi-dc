/* Holding a thrown player in place, and the reactions to being hit: the state machine by dm_no. */
#include "player.h"
#include "pl_char_old.h"
#include "pl_act.h"

/* the start of a player's animation frame, as pl_char.c reads it */
typedef struct CHAR_FRAME {
	Uint8	unk_00[0x12];
	Uint16	unk_12;		/* 012: the frame's entry in frame_box_tbl */
} CHAR_FRAME;

static void catch_em_pos_set( PLAYER_WORK *pl_w, PLAYER_WORK *em, Uint8 wall );

/****************************************************************/
/*			The lift of a turned player: half its frame's		*/
/*			box height, down; none while upright				*/
/****************************************************************/
void pl_rot_ofs_set( PLAYER_WORK *pl_w )
{
	Uint32 *tbl ;
	CHAR_FRAME *cg ;
	FRAME_BOX *box ;

	pl_w->rot_ofs_y = 0 ;
	if( pl_w->rot[2] ){
		tbl = ( Uint32 * )( pl_w->char_adrs + pl_w->char_ofs ) ;
		tbl = ( Uint32 * )( pl_w->char_adrs + tbl[16] ) ;
		cg = ( CHAR_FRAME * )( pl_w->char_adrs + tbl[pl_w->char_no.b.no] ) ;
		box = &pl_w->frame_box_tbl[( Uint8 )cg->unk_12] ;
		pl_w->rot_ofs_y = -( Uint8 )box->unk_0e / 2 ;
	}
}

/****************************************************************/
/*			Hold the victim pl_w where the thrower em's			*/
/*			frame grips it										*/
/****************************************************************/
void catch_pos_set( PLAYER_WORK *em, PLAYER_WORK *pl_w )
{
	CATCH_POS *pos ;
	Sint8 prio ;

	pos = &em->catch_pos_tbl[pl_w->pl_type][LO_BYTE( em->cg_rival )] ;
	if( em->disp_dir ) pl_w->pos.x = em->pos.x - pos->x * 1.6666666f ;
	else pl_w->pos.x = em->pos.x + DOT_X( pos->x ) ;
	pl_wall_ck( pl_w ) ;
	if( pl_w->wall_flag ){
		em->pos.x = pl_w->pos.x ;
		if( em->disp_dir ) em->pos.x += DOT_X( pos->x ) ;
		else em->pos.x -= DOT_X( pos->x ) ;
	}
	pl_w->pos.y = em->pos.y + DOT_Y( pos->y ) ;
	pl_w->char_no.b.no = pos->no ;
	pl_w->disp_dir = ( pos->dir & 1 ) ^ em->disp_dir ;
	if( pos->prio > 0 ){
		if( em->prio > pl_w->prio ){
			prio = pl_w->prio ;
			pl_w->prio = em->prio ;
			em->prio = prio ;
		}
	}
	else if( em->prio < pl_w->prio ){
		prio = pl_w->prio ;
		pl_w->prio = em->prio ;
		em->prio = prio ;
	}
	pl_w->rot[2] = em->rot[2] + ( Float )( pos->rot << 8 ) ;
	if( LO_BYTE( em->cg_rival ) != pl_w->catch_grip_old ){
		pl_char_set( pl_w, Char_Caught, pl_w->char_no.b.no ) ;
		pl_w->catch_grip_old = LO_BYTE( em->cg_rival ) ;
	}
	pl_rot_ofs_set( pl_w ) ;
}

/****************************************************************/
/*			The same for a victim held by its catch_w, with		*/
/*			the grip's priority offset and no wall				*/
/****************************************************************/
void caught_pos_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->catch_w ;
	CATCH_POS *pos ;
	Sint8 prio ;

	pos = &em->catch_pos_tbl[pl_w->pl_type][LO_BYTE( em->cg_rival )] ;
	if( em->disp_dir ) pl_w->pos.x = em->pos.x - pos->x * 1.6666666f ;
	else pl_w->pos.x = em->pos.x + DOT_X( pos->x ) ;
	pl_w->pos.y = em->pos.y + DOT_Y( pos->y ) ;
	pl_w->char_no.b.no = pos->no ;
	pl_w->disp_dir = ( pos->dir & 1 ) ^ em->disp_dir ;
	pl_w->prio = em->prio + pos->prio ;
	if( pos->prio > 0 ){
		if( em->prio > pl_w->prio ){
			prio = pl_w->prio ;
			pl_w->prio = em->prio ;
			em->prio = prio ;
		}
	}
	else if( em->prio < pl_w->prio ){
		prio = pl_w->prio ;
		pl_w->prio = em->prio ;
		em->prio = prio ;
	}
	pl_w->rot[2] = em->rot[2] + ( Float )( pos->rot << 8 ) ;
	pl_rot_ofs_set( pl_w ) ;
}

/****************************************************************/
/*			The thrower pl_w moves to hold em by its own		*/
/*			grip; em takes the grip's pose						*/
/****************************************************************/
void catch_em_pos_set0( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	catch_em_pos_set( pl_w, em, 0 ) ;
}

/****************************************************************/
/*			The same, kept off the screen's edges				*/
/****************************************************************/
void catch_em_pos_set1( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	catch_em_pos_set( pl_w, em, 1 ) ;
}

/*==============================================================*/
static void catch_em_pos_set( PLAYER_WORK *pl_w, PLAYER_WORK *em, Uint8 wall )
{
	CATCH_POS *pos ;
	Sint8 prio ;

	pos = &pl_w->catch_pos_tbl[em->pl_type][LO_BYTE( pl_w->cg_rival )] ;
	if( em->disp_dir ) pl_w->pos.x = em->pos.x - pos->x * 1.6666666f ;
	else pl_w->pos.x = em->pos.x + DOT_X( pos->x ) ;
	if( wall ){
		pl_wall_ck( pl_w ) ;
		if( pl_w->wall_flag ){
			em->pos.x = pl_w->pos.x ;
			if( em->disp_dir ) em->pos.x += DOT_X( pos->x ) ;
			else em->pos.x -= DOT_X( pos->x ) ;
		}
	}
	pl_w->pos.y = em->pos.y - pos->y * 2.142857f ;
	em->char_no.b.no = pos->no ;
	em->disp_dir = ( pos->dir & 1 ) ^ pl_w->disp_dir ;
	if( pos->prio > 0 ){
		if( pl_w->prio > em->prio ){
			prio = em->prio ;
			em->prio = pl_w->prio ;
			pl_w->prio = prio ;
		}
	}
	else if( pl_w->prio < em->prio ){
		prio = em->prio ;
		em->prio = pl_w->prio ;
		pl_w->prio = prio ;
	}
	em->rot[2] = pl_w->rot[2] + ( Float )( pos->rot << 8 ) ;
	if( LO_BYTE( pl_w->cg_rival ) != em->catch_grip_old ){
		pl_char_set( em, Char_Caught, em->char_no.b.no ) ;
		em->catch_grip_old = LO_BYTE( pl_w->cg_rival ) ;
	}
	pl_rot_ofs_set( em ) ;
}

static void dm_zuri_move( PLAYER_WORK *pl_w );
static void dm_buttobi_move( PLAYER_WORK *pl_w );
static void dm_buttobi_init( PLAYER_WORK *pl_w );
static void dm_buttobi_fly( PLAYER_WORK *pl_w );
static void dm_buttobi_down( PLAYER_WORK *pl_w );
static void dm_piyo_move( PLAYER_WORK *pl_w );
static void dm_piyo_init( PLAYER_WORK *pl_w );
static void dm_piyo_rise( PLAYER_WORK *pl_w );
static void dm_sweep_move( PLAYER_WORK *pl_w );
static void dm_hold_move( PLAYER_WORK *pl_w );
static void dm_sandbag_move( PLAYER_WORK *pl_w );
static void dm_07_move( PLAYER_WORK *pl_w );
static void dm_07_0( PLAYER_WORK *pl_w );
static void dm_07_1( PLAYER_WORK *pl_w );
static void dm_07_2( PLAYER_WORK *pl_w );
static void dm_07_3( PLAYER_WORK *pl_w );
static void dm_freeze_move( PLAYER_WORK *pl_w );
static void dm_freeze_init( PLAYER_WORK *pl_w );
static void dm_freeze_slide( PLAYER_WORK *pl_w );
static void dm_freeze_wait( PLAYER_WORK *pl_w );
static void dm_away_move( PLAYER_WORK *pl_w );
static void dm_away_init( PLAYER_WORK *pl_w );
static void dm_away_fly( PLAYER_WORK *pl_w );
static void dm_away_down( PLAYER_WORK *pl_w );
static void dm_bound_move( PLAYER_WORK *pl_w );
static void dm_bound_init( PLAYER_WORK *pl_w );
static void dm_bound_fly( PLAYER_WORK *pl_w );
static void dm_bound_hop( PLAYER_WORK *pl_w );
static void dm_bound_down( PLAYER_WORK *pl_w );
static void dm_snap_move( PLAYER_WORK *pl_w );
static void dm_snap_init( PLAYER_WORK *pl_w );
static void dm_snap_fly( PLAYER_WORK *pl_w );
static void dm_snap_down( PLAYER_WORK *pl_w );
static void dm_snap_out( PLAYER_WORK *pl_w );
static void dm_snap_change( PLAYER_WORK *pl_w );
static void dm_snap_disp_ck( PLAYER_WORK *pl_w );
static void dm_snap_land( PLAYER_WORK *pl_w );
static void dm_spark_move( PLAYER_WORK *pl_w );
static void dm_dizzy_move( PLAYER_WORK *pl_w );
static void dm_dizzy_init( PLAYER_WORK *pl_w );
static void dm_dizzy_down( PLAYER_WORK *pl_w );
static void dm_dizzy_wait( PLAYER_WORK *pl_w );
static void dm_dizzy_up( PLAYER_WORK *pl_w );
static void dm_slam_move( PLAYER_WORK *pl_w );
static void dm_slam_init( PLAYER_WORK *pl_w );
static void dm_slam_wait( PLAYER_WORK *pl_w );
static void dm_slam_fly( PLAYER_WORK *pl_w );
static void dm_slam_down( PLAYER_WORK *pl_w );
static void dm_freeze_air_move( PLAYER_WORK *pl_w );
static void dm_freeze_air_init( PLAYER_WORK *pl_w );
static void dm_freeze_air_fly( PLAYER_WORK *pl_w );
static void dm_freeze_air_bound( PLAYER_WORK *pl_w );
static void dm_down_hit_move( PLAYER_WORK *pl_w );
static void dm_curse_move( PLAYER_WORK *pl_w );
static void dm_curse_init( PLAYER_WORK *pl_w );
static void dm_curse_stand( PLAYER_WORK *pl_w );
static void dm_curse_crouch( PLAYER_WORK *pl_w );
static void dm_curse_jump( PLAYER_WORK *pl_w );
static void dm_curse_atck( PLAYER_WORK *pl_w );
static void dm_curse_fly( PLAYER_WORK *pl_w );
static void dm_curse_walk( PLAYER_WORK *pl_w );
static int dm_curse_atck_ck( PLAYER_WORK *pl_w );
static void dm_curse_stand_set( PLAYER_WORK *pl_w );
static void dm_curse_crouch_set( PLAYER_WORK *pl_w );
static void dm_curse_jump_set( PLAYER_WORK *pl_w );
static void dm_curse_atck_set( PLAYER_WORK *pl_w );
static void dm_curse_walk_set( PLAYER_WORK *pl_w );
static void dm_curse_set( PLAYER_WORK *pl_w, Uint8 no );
static void dm_away_scr_ck( PLAYER_WORK *pl_w );

/* a landing's quake, a bigger one for a heavy character */
#define LAND_QUAKE( pl )	{ Sint8 lv = ( pl )->weight < 5 ? 1 : 3 ; scr_work.quake_level = lv ; scr_work.quake_flag = 1 ; }

/****************************************************************/
/*			Being hit: the reaction by dm_no, which				*/
/*			pl_damage chose										*/
/****************************************************************/
void pl_dm_move( PLAYER_WORK *pl_w )
{
	static void ( *dm_jmp[25] )( PLAYER_WORK *pl_w ) = {
		dm_zuri_move, dm_buttobi_move, dm_buttobi_move, dm_piyo_move, dm_sweep_move, dm_hold_move, dm_hold_move,
		dm_07_move, dm_07_move, dm_freeze_move, dm_away_move, dm_bound_move, dm_snap_move, dm_zuri_move,
		dm_buttobi_move, dm_spark_move, dm_dizzy_move, dm_slam_move, dm_slam_move, dm_sandbag_move, dm_curse_move,
		dm_buttobi_move, dm_away_move, dm_freeze_air_move, dm_down_hit_move,
	} ;

	dm_jmp[pl_w->dm_no]( pl_w ) ;
}

/*==============================================================*/
/*			Pushed back on the ground							*/
/*==============================================================*/
static void dm_zuri_move( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
	else {
		dm_back_move( pl_w ) ;
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
/*			Knocked into the air: flying, landing, and lying	*/
/*			down												*/
/*==============================================================*/
static void dm_buttobi_move( PLAYER_WORK *pl_w )
{
	static void ( *dm_01_jmp[3] )( PLAYER_WORK *pl_w ) = {
		dm_buttobi_init, dm_buttobi_fly, dm_buttobi_down,
	} ;

	pl_lever_drift( pl_w ) ;
	if( vs_sys_work.vs_st < VS_END_WAIT && pl_w->ko_flag && pl_w->wall_hit_flag ) pl_w->catch_ng_timer = 2 ;
	dm_01_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dm_buttobi_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.x = pl_w->back_spd_x ;
	pl_w->spd.y = pl_w->back_spd_y ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = -1.4732143f ;
	pl_w->timer1 = 0 ;
	dm_buttobi_fly( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void dm_buttobi_fly( PLAYER_WORK *pl_w )
{
	dm_blow_move( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( pl_w->wall_flag && !pl_w->wall_hit_flag ){
		pl_w->wall_hit_flag = 1 ;
		if( pl_w->fly_screen_flag ){ }
		down_se_req( pl_w ) ;
		wall_dust_set( pl_w ) ;
	}
	if( !pl_w->recover_ng_flag && pl_w->vital_new && -- pl_w->recover_timer < 0 ){
		pl_w->recover_timer = 0 ;
		if( pl_w->spd.y < -DOT_Y( 6.0f ) ){
			pl_act_set( pl_w, N_AIR_RECOVER ) ;
			return ;
		}
	}
	if( pl_w->timer0 ){
		if( -- pl_w->timer0 == 0 ) dm_blow_char_set( pl_w ) ;
	}
	if( !pl_w->timer1 ){
		if( pl_w->spd.y >= 0.0f ) return ;
		if( !pl_w->timer0 ){
			pl_w->timer1 = 0xff ;
			dm_fall_char_set( pl_w ) ;
		}
	}
	if( pl_w->spd.y < 0.0f && pl_land_ck( pl_w ) ){
		if( pl_w->vital_new && !pl_w->recover_ng_flag ){
			pl_w->spd.y = -( pl_w->spd.y / 2.0f ) ;
			land_se_req( pl_w ) ;
			land_dust_set( &pl_w->pos, pl_w->pl_no ) ;
			pl_act_set( pl_w, N_AIR_RECOVER ) ;
		}
		else {
			land_se_req( pl_w ) ;
			land_dust_set( &pl_w->pos, pl_w->pl_no ) ;
			if( pl_w->fly_screen_flag ){
				LAND_QUAKE( pl_w ) ;
			}
			pl_w->routine_2 ++ ;
			pl_w->pl_stance = STANCE_DOWN ;
			pl_char_set( pl_w, Char_Damage, 26 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void dm_buttobi_down( PLAYER_WORK *pl_w )
{
	if( pl_w->down_hit_flag ){
		pl_w->catch_ng_timer = 2 ;
		pl_w->dm_flag = 0 ;
	}
	if( !pl_w->fly_screen_flag && pl_w->vital_new && pl_w->ukemi_flag ){
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
	if( pl_char_move( pl_w ) < 0 ) pl_act_set( pl_w, N_DOWN ) ;
}

/*==============================================================*/
/*			Knocked up and away									*/
/*==============================================================*/
static void dm_piyo_move( PLAYER_WORK *pl_w )
{
	static void ( *dm_03_jmp[2] )( PLAYER_WORK *pl_w ) = {
		dm_piyo_init, dm_piyo_rise,
	} ;

	pl_w->catch_ng_timer = 2 ;
	pl_w->dm_flag = 0 ;
	dm_03_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dm_piyo_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.x = pl_w->back_spd_x ;
	pl_w->spd.y = pl_w->back_spd_y ;
	pl_w->acc.x = 0.0f ;
	dm_piyo_rise( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void dm_piyo_rise( PLAYER_WORK *pl_w )
{
	dm_blow_move( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( pl_w->spd.y < 0.0f ) pl_act_set( pl_w, N_AIR_RECOVER ) ;
}

/*==============================================================*/
/*			Falling to the floor, then lying there				*/
/*==============================================================*/
static void dm_sweep_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->acc.y ){
		if( pl_land_ck( pl_w ) ){
			pl_w->spd.y = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_w->pl_stance = STANCE_DOWN ;
			land_se_req( pl_w ) ;
			land_dust_set( &pl_w->pos, pl_w->pl_no ) ;
		}
	}
	else if( pl_w->vital_new && pl_w->ukemi_flag ){
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
	if( pl_char_move( pl_w ) < 0 ) pl_act_set( pl_w, N_DOWN ) ;
}

/*==============================================================*/
/*			Held still while the attacker's move goes on		*/
/*==============================================================*/
static void dm_hold_move( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *wk ;

	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 60 ;
	}
	wk = pl_w->dm_at_w ;
	if( !wk ) wk = pl_w->target_w ;
	if( wk->routine_1 == PL_DAMAGE ){
		dm_ko_set( pl_w ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 > 0 ) return ;
	if( !pl_w->vital_new ) dm_ko_set( pl_w ) ;
	else if( pl_w->pl_stance == STANCE_AIR ) pl_jump_ret( pl_w ) ;
	else pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void dm_sandbag_move( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *wk ;

	wk = pl_w->dm_at_w ;
	if( !wk ) wk = pl_w->target_w ;
	if( wk->routine_1 == PL_DAMAGE ){
		dm_ko_set( pl_w ) ;
		return ;
	}
	if( -- pl_w->recover_timer > 0 ) return ;
	if( !pl_w->vital_new ) dm_ko_set( pl_w ) ;
	else if( pl_w->pl_stance == STANCE_AIR ) pl_jump_ret( pl_w ) ;
	else pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			Blown away, turning over							*/
/*==============================================================*/
static void dm_07_move( PLAYER_WORK *pl_w )
{
	static void ( *dm_07_jmp[4] )( PLAYER_WORK *pl_w ) = {
		dm_07_0, dm_07_1, dm_07_2, dm_07_3,
	} ;

	dm_07_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dm_07_0( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.x = pl_w->back_spd_x ;
	pl_w->spd.y = pl_w->back_spd_y ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	dm_07_1( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void dm_07_1( PLAYER_WORK *pl_w )
{
	dm_blow_move( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( pl_w->wall_flag && !pl_w->wall_hit_flag ){
		pl_w->wall_hit_flag = 1 ;
		down_se_req( pl_w ) ;
		wall_dust_set( pl_w ) ;
	}
	if( !pl_w->recover_ng_flag && pl_w->vital_new && -- pl_w->recover_timer < 0 ){
		pl_w->recover_timer = 0 ;
		if( pl_w->spd.y < -DOT_Y( 6.0f ) ){
			pl_act_set( pl_w, N_AIR_RECOVER ) ;
			return ;
		}
	}
	if( pl_w->timer0 ){
		if( -- pl_w->timer0 <= 0 ) pl_char_set( pl_w, Char_Damage, 17 ) ;
	}
	if( pl_w->spd.y <= 0.0f && pl_land_ck( pl_w ) ){
		land_se_req( pl_w ) ;
		land_dust_set( &pl_w->pos, pl_w->pl_no ) ;
		if( !pl_w->recover_ng_flag && pl_w->vital_new ){
			pl_w->spd.y = -( pl_w->spd.y / 2.0f ) ;
			pl_act_set( pl_w, N_AIR_RECOVER ) ;
		}
		else {
			pl_w->pl_stance = STANCE_DOWN ;
			pl_w->routine_2 = 3 ;
			pl_char_set( pl_w, Char_Damage, 26 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void dm_07_2( PLAYER_WORK *pl_w )
{
}

/*--------------------------------------------------------------*/
static void dm_07_3( PLAYER_WORK *pl_w )
{
	if( pl_w->down_hit_flag ){
		pl_w->catch_ng_timer = 2 ;
		pl_w->dm_flag = 0 ;
	}
	if( !pl_w->fly_screen_flag && pl_w->vital_new && pl_w->ukemi_flag ){
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
	if( pl_char_move( pl_w ) < 0 ) pl_act_set( pl_w, N_DOWN ) ;
}

/*==============================================================*/
/*			Pushed to the wall, then stunned					*/
/*==============================================================*/
static void dm_freeze_move( PLAYER_WORK *pl_w )
{
	static void ( *dm_09_jmp[3] )( PLAYER_WORK *pl_w ) = {
		dm_freeze_init, dm_freeze_slide, dm_freeze_wait,
	} ;

	dm_09_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dm_freeze_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.x = pl_w->back_spd_x ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	dm_freeze_slide( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void dm_freeze_slide( PLAYER_WORK *pl_w )
{
	back_move_wall( pl_w ) ;
	if( !pl_w->spd.x ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		if( ( pl_w->dm_kind & K_ELEM_MASK ) == 5 ){
			pl_w->dizzy_timer = 2 ;
			combo_reset( pl_w ) ;
		}
		else {
			pl_w->renda_cnt = 60 ;
			pl_w->dizzy_timer = 120 ;
			combo_reset( pl_w ) ;
		}
	}
}

/*==============================================================*/
/*			On the ground: gets up when its timer runs out,		*/
/*			or at once with no vital left or when falling		*/
/*==============================================================*/
static void dm_freeze_wait( PLAYER_WORK *pl_w )
{
	if( !pl_w->vital_new ) goto stand ;
	if( pl_fall_ck( pl_w ) ) goto stand ;
	if( ( pl_w->dm_kind & K_ELEM_MASK ) != 5 && !pl_w->renda_cnt ) goto stand ;
	if( -- pl_w->dizzy_timer >= 0 ) return ;
stand:
	pl_w->auto_guard_timer = 8 ;
	pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			Knocked away, kept on the screen					*/
/*==============================================================*/
static void dm_away_move( PLAYER_WORK *pl_w )
{
	static void ( *dm_0a_jmp[3] )( PLAYER_WORK *pl_w ) = {
		dm_away_init, dm_away_fly, dm_away_down,
	} ;

	pl_w->wall_ng_timer = 2 ;
	dm_away_scr_ck( pl_w ) ;
	dm_0a_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dm_away_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.x = pl_w->back_spd_x ;
	pl_w->spd.y = pl_w->back_spd_y ;
	pl_w->acc.x = 0.0f ;
	pl_w->timer0 = 0 ;
	if( pl_w->dm_no == 22 ){
		pl_char_set( pl_w, Char_Damage, 28 ) ;
		pl_w->disp_flag = 1 ;
		pl_w->rot[2] = 0 ;
		pl_w->scale = pl_w->pl_scale ;
		pl_w->alpha = 1.0f ;
	}
	dm_away_fly( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void dm_away_fly( PLAYER_WORK *pl_w )
{
	dm_blow_move( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( !pl_w->timer0 ){
		if( pl_w->spd.y > 0.0f ) return ;
		pl_w->timer0 = 1 ;
		pl_char_set( pl_w, Char_Damage, 29 ) ;
	}
	if( pl_w->spd.y <= 0.0f && pl_land_ck( pl_w ) ){
		land_dust_set( &pl_w->pos, pl_w->pl_no ) ;
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_DOWN ;
		pl_char_set( pl_w, Char_Damage, 26 ) ;
		land_se_req( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void dm_away_down( PLAYER_WORK *pl_w )
{
	if( pl_w->down_hit_flag ){
		pl_w->catch_ng_timer = 2 ;
		pl_w->dm_flag = 0 ;
	}
	if( !pl_w->fly_screen_flag && pl_w->vital_new && pl_w->ukemi_flag ){
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
	if( pl_char_move( pl_w ) < 0 ) pl_act_set( pl_w, N_DOWN ) ;
}

/*==============================================================*/
/*			Knocked down with a bounce							*/
/*==============================================================*/
static void dm_bound_move( PLAYER_WORK *pl_w )
{
	static void ( *dm_0b_jmp[4] )( PLAYER_WORK *pl_w ) = {
		dm_bound_init, dm_bound_fly, dm_bound_hop, dm_bound_down,
	} ;

	dm_0b_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dm_bound_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.x = pl_w->back_spd_x ;
	pl_w->spd.y = pl_w->back_spd_y ;
	pl_w->acc.y = -1.4732143f ;
	dm_bound_fly( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void dm_bound_fly( PLAYER_WORK *pl_w )
{
	dm_blow_move( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( pl_w->spd.y <= 0.0f && pl_land_ck( pl_w ) ){
		pl_w->routine_2 ++ ;
		pl_w->spd.y = -( pl_w->spd.y / 2.0f ) ;
		land_se_req( pl_w ) ;
		land_dust_set( &pl_w->pos, pl_w->pl_no ) ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->timer0 = 0 ;
		pl_char_set( pl_w, Char_Damage, 28 ) ;
	}
}

/*--------------------------------------------------------------*/
static void dm_bound_hop( PLAYER_WORK *pl_w )
{
	dm_blow_move( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( !pl_w->timer0 && pl_w->spd.y > 0.0f ){
		pl_w->timer0 = 1 ;
		pl_char_set( pl_w, Char_Damage, 29 ) ;
	}
	if( pl_land_ck( pl_w ) ){
		land_se_req( pl_w ) ;
		land_dust_set( &pl_w->pos, pl_w->pl_no ) ;
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_DOWN ;
		pl_char_set( pl_w, Char_Damage, 26 ) ;
	}
}

/*--------------------------------------------------------------*/
static void dm_bound_down( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_act_set( pl_w, N_DOWN ) ;
}

/*==============================================================*/
/*			Knocked off the screen, then back in from the		*/
/*			side												*/
/*==============================================================*/
static void dm_snap_move( PLAYER_WORK *pl_w )
{
	static void ( *dm_0c_jmp[5] )( PLAYER_WORK *pl_w ) = {
		dm_snap_init, dm_snap_fly, dm_snap_down, dm_snap_out, dm_snap_change,
	} ;

	pl_w->wall_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	pl_w->disp_flag = 1 ;
	dm_snap_disp_ck( pl_w ) ;
	dm_0c_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dm_snap_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.x = pl_w->back_spd_x ;
	pl_w->spd.y = pl_w->back_spd_y ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = -1.4732143f ;
	dm_snap_fly( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void dm_snap_fly( PLAYER_WORK *pl_w )
{
	dm_blow_move( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( pl_w->timer0 ){
		if( -- pl_w->timer0 <= 0 ) pl_char_set( pl_w, Char_Damage, 34 ) ;
	}
	if( pl_w->spd.y <= 0.0f && pl_land_ck( pl_w ) ){
		dm_snap_land( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_DOWN ;
		if( pl_w->spd.x > 0.0f ) pl_w->spd.x = 10.0f ;
		else pl_w->spd.x = -10.0f ;
		pl_w->acc.x = 0.0f ;
		pl_char_set( pl_w, Char_Damage, 13 ) ;
	}
}

/*--------------------------------------------------------------*/
static void dm_snap_down( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Damage, 34 ) ;
	}
}

/*--------------------------------------------------------------*/
static void dm_snap_out( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !pl_w->disp_flag ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 30 ;
		return ;
	}
	pl_char_move( pl_w ) ;
	if( pl_w->spd.y <= 0.0f && pl_land_ck( pl_w ) ) dm_snap_land( pl_w ) ;
}

/*==============================================================*/
/*			Off the screen: the partner comes in				*/
/*==============================================================*/
static void dm_snap_change( PLAYER_WORK *pl_w )
{
	pl_w->catch_ng_timer = 2 ;
	if( pl_w->team_ng_timer ) return ;
	if( pl_w->team_no || ( vs_sys_work.be_cnt[pl_w->pl_no] == 1 && -- pl_w->timer0 <= 0 ) ){
		pl_w->change_req = 9 ;
		pl_w->partner_no = pl_w->snap_partner ;
		pl_w->snap_partner = 0 ;
		pl_w->vital_red_wait = 300 ;
		combo_reset( pl_w ) ;
	}
}

/*==============================================================*/
/*			Hidden once past the screen's edge					*/
/*==============================================================*/
static void dm_snap_disp_ck( PLAYER_WORK *pl_w )
{
	SCR_WORK *sw = &scr_work ;
	Float x ;

	x = sw->eye[0].x - 460.0f ;
	if( pl_w->pos.x <= x ) pl_w->disp_flag = 0 ;
	else {
		x = sw->eye[0].x + 460.0f ;
		if( pl_w->pos.x >= x ) pl_w->disp_flag = 0 ;
	}
}

/*==============================================================*/
/*			The landing's sound, dust and quake, and a small	*/
/*			bounce												*/
/*==============================================================*/
static void dm_snap_land( PLAYER_WORK *pl_w )
{
	down_se_req( pl_w ) ;
	land_dust_set( &pl_w->pos, pl_w->pl_no ) ;
	if( pl_w->disp_flag ){ }
	LAND_QUAKE( pl_w ) ;
	pl_w->spd.y = DOT_Y( 5.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
}

/*==============================================================*/
/*			Electrocuted: the shocks, then the KO				*/
/*==============================================================*/
static void dm_spark_move( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->timer1 = pl_w->atck_level * 4 + 3 ;
		pl_w->timer0 = 0 ;
	}
	if( -- pl_w->timer0 >= 0 ) return ;
	pl_w->timer0 = 2 ;
	if( pl_w->timer1 ){
		pls28_00_set( pl_w, pl_w->timer1 ) ;
		if( -- pl_w->timer1 == 0 ) pl_w->timer0 = 10 ;
	}
	else dm_ko_set( pl_w ) ;
}

/*==============================================================*/
/*			Crumpling to the floor								*/
/*==============================================================*/
static void dm_dizzy_move( PLAYER_WORK *pl_w )
{
	static void ( *dm_10_jmp[4] )( PLAYER_WORK *pl_w ) = {
		dm_dizzy_init, dm_dizzy_down, dm_dizzy_wait, dm_dizzy_up,
	} ;

	dm_10_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dm_dizzy_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->timer0 = 16 ;
	down_se_req( pl_w ) ;
	land_dust_set( &pl_w->pos, pl_w->pl_no ) ;
	dm_dizzy_down( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void dm_dizzy_down( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Damage, 31 ) ;
	}
}

/*--------------------------------------------------------------*/
static void dm_dizzy_wait( PLAYER_WORK *pl_w )
{
	if( -- pl_w->timer0 < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Get_Up, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void dm_dizzy_up( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_act_set( pl_w, N_DIZZY ) ;
}

/*==============================================================*/
/*			Knocked into the air after a pause					*/
/*==============================================================*/
static void dm_slam_move( PLAYER_WORK *pl_w )
{
	static void ( *dm_11_jmp[4] )( PLAYER_WORK *pl_w ) = {
		dm_slam_init, dm_slam_wait, dm_slam_fly, dm_slam_down,
	} ;

	if( vs_sys_work.vs_st < VS_END_WAIT && pl_w->ko_flag && pl_w->wall_hit_flag ) pl_w->catch_ng_timer = 2 ;
	dm_11_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dm_slam_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.x = pl_w->back_spd_x ;
	pl_w->spd.y = pl_w->back_spd_y ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = -1.4732143f ;
	pl_w->timer1 = 0 ;
	dm_slam_wait( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void dm_slam_wait( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 == 0 ){
		pl_w->routine_2 ++ ;
		dm_blow_char_set( pl_w ) ;
		dm_slam_fly( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void dm_slam_fly( PLAYER_WORK *pl_w )
{
	dm_blow_move( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( pl_w->wall_flag && !pl_w->wall_hit_flag ){
		pl_w->wall_hit_flag = 1 ;
		if( pl_w->fly_screen_flag ){ }
		down_se_req( pl_w ) ;
		wall_dust_set( pl_w ) ;
	}
	if( !pl_w->recover_ng_flag && pl_w->vital_new && -- pl_w->recover_timer < 0 ){
		pl_w->recover_timer = 0 ;
		if( pl_w->spd.y < -DOT_Y( 6.0f ) ){
			pl_act_set( pl_w, N_AIR_RECOVER ) ;
			return ;
		}
	}
	if( !pl_w->timer1 ){
		if( pl_w->spd.y >= 0.0f ) return ;
		if( !pl_w->timer0 ){
			pl_w->timer1 = 0xff ;
			dm_fall_char_set( pl_w ) ;
		}
	}
	if( pl_w->spd.y < 0.0f && pl_land_ck( pl_w ) ){
		if( pl_w->vital_new && !pl_w->recover_ng_flag ){
			pl_w->spd.y = -( pl_w->spd.y / 2.0f ) ;
			land_se_req( pl_w ) ;
			land_dust_set( &pl_w->pos, pl_w->pl_no ) ;
			pl_act_set( pl_w, N_AIR_RECOVER ) ;
		}
		else {
			land_se_req( pl_w ) ;
			land_dust_set( &pl_w->pos, pl_w->pl_no ) ;
			if( pl_w->fly_screen_flag ){
				LAND_QUAKE( pl_w ) ;
			}
			pl_w->routine_2 ++ ;
			pl_w->pl_stance = STANCE_DOWN ;
			pl_char_set( pl_w, Char_Damage, 26 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void dm_slam_down( PLAYER_WORK *pl_w )
{
	if( pl_w->down_hit_flag ){
		pl_w->catch_ng_timer = 2 ;
		pl_w->dm_flag = 0 ;
	}
	if( !pl_w->fly_screen_flag && pl_w->vital_new && pl_w->ukemi_flag ){
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
	if( pl_char_move( pl_w ) < 0 ) pl_act_set( pl_w, N_DOWN ) ;
}

/*==============================================================*/
/*			Knocked down with a bounce, then stunned			*/
/*==============================================================*/
static void dm_freeze_air_move( PLAYER_WORK *pl_w )
{
	static void ( *dm_17_jmp[4] )( PLAYER_WORK *pl_w ) = {
		dm_freeze_air_init, dm_freeze_air_fly, dm_freeze_air_bound, dm_freeze_wait,
	} ;

	dm_17_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dm_freeze_air_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.x = pl_w->back_spd_x ;
	pl_w->spd.y = pl_w->back_spd_y ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	dm_freeze_air_fly( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void dm_freeze_air_fly( PLAYER_WORK *pl_w )
{
	dm_blow_move( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( pl_w->wall_flag && !pl_w->wall_hit_flag ){
		pl_w->wall_hit_flag = 1 ;
		down_se_req( pl_w ) ;
		wall_dust_set( pl_w ) ;
	}
	if( pl_land_ck( pl_w ) ){
		land_se_req( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_w->spd.y = -( pl_w->spd.y / 2.0f ) ;
		if( pl_w->spd.y <= DOT_Y( 2.0f ) ) pl_w->spd.y = DOT_Y( 2.0f ) ;
	}
}

/*--------------------------------------------------------------*/
static void dm_freeze_air_bound( PLAYER_WORK *pl_w )
{
	dm_blow_move( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		if( ( pl_w->dm_kind & K_ELEM_MASK ) == 5 ){
			pl_w->dizzy_timer = 2 ;
			combo_reset( pl_w ) ;
		}
		else {
			if( pl_w->dm_at_w->pl_type == PL_ICEMAN || pl_w->dm_at_w->pl_type == PL_MAGNETO ){
				pl_w->renda_cnt = 1 ;
				pl_w->dizzy_timer = 48 ;
			}
			else {
				pl_w->renda_cnt = 60 ;
				pl_w->dizzy_timer = 120 ;
			}
			combo_reset( pl_w ) ;
		}
	}
}

/*==============================================================*/
/*			Thrown up and dropped								*/
/*==============================================================*/
static void dm_down_hit_move( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_dir_set( pl_w ) ;
		if( pl_w->pl_dir ){
			pl_w->spd.x = -DOT_X( 6.5f ) ;
			pl_w->acc.x = 0.3125f ;
		}
		else {
			pl_w->spd.x = DOT_X( 6.5f ) ;
			pl_w->acc.x = -0.3125f ;
		}
		pl_w->spd.y = DOT_Y( 8.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.75f ) ;
		if( !pl_w->down_hit_flag ){
			pl_w->spd.x = 0.0f ;
			pl_w->acc.x = 0.0f ;
		}
	}
	back_move_wall( pl_w ) ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		if( !pl_w->down_hit_flag ){
			pl_act_set( pl_w, N_DOWN ) ;
			land_dust_set( &pl_w->pos, pl_w->pl_no ) ;
		}
		else {
			pl_w->jump_dir = 1 ;
			pl_act_set( pl_w, N_GET_UP ) ;
		}
	}
}

/*==============================================================*/
/*			Hidden, and moving freely until the timer runs		*/
/*			out													*/
/*==============================================================*/
static void dm_curse_move( PLAYER_WORK *pl_w )
{
	static FRAME_BOX dm_14_box = { 0, 0, 0, 251, 0, 0, 0, 0 } ;
	static void ( *dm_14_jmp[7] )( PLAYER_WORK *pl_w ) = {
		dm_curse_init, dm_curse_stand, dm_curse_crouch, dm_curse_jump, dm_curse_atck, dm_curse_fly, dm_curse_walk,
	} ;

	if( -- pl_w->timer0 <= 0 || vs_sys_w->vs_st >= VS_END_WAIT ){
		pl_w->disp_flag = 1 ;
		pl_w->cg_shadow = 0 ;
		if( pl_w->pl_stance == STANCE_AIR ) pl_jump_ret( pl_w ) ;
		else pl_stand_ret( pl_w ) ;
		return ;
	}
	pl_w->disp_flag = 0 ;
	pl_w->cg_shadow = 0xff ;
	pl_w->frame_box = &dm_14_box ;
	dm_14_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dm_curse_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = 5 ;
	pl_w->routine_3 = 0 ;
	pl_w->spd.x = pl_w->back_spd_x ;
	pl_w->spd.y = pl_w->back_spd_y ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = -1.4732143f ;
	dm_curse_fly( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void dm_curse_stand( PLAYER_WORK *pl_w )
{
	pl_dir_set( pl_w ) ;
	if( dm_curse_atck_ck( pl_w ) ) return ;
	if( pl_w->pl_new_lvbt & KEY_UP ){
		dm_curse_set( pl_w, 3 ) ;
		return ;
	}
	if( pl_w->pl_new_lvbt & KEY_DOWN ){
		dm_curse_set( pl_w, 2 ) ;
		return ;
	}
	if( pl_w->pl_new_lvbt & ( KEY_FORWARD | KEY_BACK ) || pl_w->cpu_flag ) dm_curse_set( pl_w, 6 ) ;
}

/*--------------------------------------------------------------*/
static void dm_curse_crouch( PLAYER_WORK *pl_w )
{
	pl_dir_set( pl_w ) ;
	if( dm_curse_atck_ck( pl_w ) ) return ;
	if( pl_w->pl_new_lvbt & KEY_UP ){
		dm_curse_set( pl_w, 3 ) ;
		return ;
	}
	if( !( pl_w->pl_new_lvbt & KEY_DOWN ) ) dm_curse_set( pl_w, 1 ) ;
}

/*--------------------------------------------------------------*/
static void dm_curse_jump( PLAYER_WORK *pl_w )
{
	pl_jump_move( pl_w ) ;
	if( !dm_curse_atck_ck( pl_w ) && pl_land_ck( pl_w ) ){
		pl_w->pos.y = pl_w->ground_y ;
		dm_curse_set( pl_w, 1 ) ;
		pl_dir_set( pl_w ) ;
		pl_land_ef_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void dm_curse_atck( PLAYER_WORK *pl_w )
{
	if( pl_w->atck_type ){
		pl_jump_move( pl_w ) ;
		if( pl_land_ck( pl_w ) ){
			pl_w->pos.y = pl_w->ground_y ;
			dm_curse_set( pl_w, 1 ) ;
			pl_dir_set( pl_w ) ;
			pl_land_ef_set( pl_w ) ;
		}
	}
	else if( -- pl_w->timer1 <= 0 ){
		if( pl_w->pl_new_lvbt & KEY_DOWN ) dm_curse_set( pl_w, 2 ) ;
		else dm_curse_set( pl_w, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void dm_curse_fly( PLAYER_WORK *pl_w )
{
	dm_blow_move( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( pl_w->wall_flag && !pl_w->wall_hit_flag ){
		pl_w->wall_hit_flag = 1 ;
		down_se_req( pl_w ) ;
		wall_dust_set( pl_w ) ;
	}
	if( pl_w->spd.y < 0.0f && pl_land_ck( pl_w ) ){
		pl_w->pos.y = pl_w->ground_y ;
		dm_curse_set( pl_w, 1 ) ;
		combo_reset( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void dm_curse_walk( PLAYER_WORK *pl_w )
{
	pl_dir_set( pl_w ) ;
	if( dm_curse_atck_ck( pl_w ) ) return ;
	if( pl_w->pl_new_lvbt & KEY_UP ){
		dm_curse_set( pl_w, 3 ) ;
		return ;
	}
	if( pl_w->pl_new_lvbt & KEY_DOWN ){
		dm_curse_set( pl_w, 2 ) ;
		return ;
	}
	if( pl_w->cpu_flag ) pl_w->jump_dir = 1 ;
	else {
		pl_w->jump_dir = ( pl_w->pl_sw_new & BACK ) >> 10 ;
		if( !( pl_w->pl_new_lvbt & ( KEY_FORWARD | KEY_BACK ) ) ){
			dm_curse_set( pl_w, 1 ) ;
			return ;
		}
	}
}

/*==============================================================*/
/*			An attack: against the wall for the computer		*/
/*==============================================================*/
static int dm_curse_atck_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->pl_stance == STANCE_AIR && pl_w->pos.y <= pl_w->ground_y ) return 0 ;
	if( pl_w->cpu_flag ){
		if( !( pl_w->wall_flag & 1 << pl_w->pl_dir ) ) return 0 ;
	}
	else if( !( Uint16 )( pl_w->pl_now_lvbt & KEY_ATCK ) ) return 0 ;
	dm_curse_set( pl_w, 4 ) ;
	return 1 ;
}

/*==============================================================*/
static void dm_curse_stand_set( PLAYER_WORK *pl_w )
{
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->routine_2 = 1 ;
	pl_w->routine_3 = 0 ;
}

/*==============================================================*/
static void dm_curse_crouch_set( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = 2 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_stance = STANCE_CROUCH ;
}

/*==============================================================*/
static void dm_curse_jump_set( PLAYER_WORK *pl_w )
{
	static Sint16 jump_tbl[3][4] = {
		{ 2176, -108, 0, 0 },
		{ 2176, -108, -704, 6 },
		{ 2176, -108, 512, -6 },
	} ;
	Sint16 *tbl ;

	pl_w->routine_2 = 3 ;
	pl_w->routine_3 = 0 ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_AIR ;
	if( !( pl_w->pl_sw_new & ( KEY_FORWARD | KEY_BACK ) ) ) pl_w->jump_dir = -1 ;
	else if( pl_w->pl_sw_new & KEY_FORWARD ) pl_w->jump_dir = 0 ;
	else pl_w->jump_dir = 1 ;
	tbl = jump_tbl[0] ;
	tbl += ( pl_w->jump_dir + 1 ) * 4 ;
	pl_w->spd.y = tbl[0] * 2.142857f / 256.0f ;
	pl_w->acc.y = tbl[1] * 2.142857f / 256.0f ;
	pl_w->spd.x = tbl[2] * 1.6666666f / 256.0f ;
	pl_w->acc.x = tbl[3] * 1.6666666f / 256.0f ;
	if( pl_w->pl_dir ){
		pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = -pl_w->acc.x ;
	}
}

/*==============================================================*/
static void dm_curse_atck_set( PLAYER_WORK *pl_w )
{
	if( pl_w->pl_stance == STANCE_AIR ) pl_w->atck_type = 1 ;
	else {
		pl_w->pos.y = pl_w->ground_y ;
		if( pl_w->pl_new_lvbt & KEY_DOWN ) pl_w->pl_stance = STANCE_CROUCH ;
		else pl_w->pl_stance = STANCE_STAND ;
		pl_w->atck_type = 0 ;
	}
	pl_w->routine_2 = 4 ;
	pl_w->routine_3 = 0 ;
	pl_w->timer1 = 40 ;
}

/*==============================================================*/
static void dm_curse_walk_set( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = 6 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
}

/*==============================================================*/
static void dm_curse_set( PLAYER_WORK *pl_w, Uint8 no )
{
	static void ( *dm_14_set_jmp[7] )( PLAYER_WORK *pl_w ) = {
		dm_curse_stand_set, dm_curse_stand_set, dm_curse_crouch_set, dm_curse_jump_set, dm_curse_atck_set, dm_curse_stand_set, dm_curse_walk_set,
	} ;

	dm_14_set_jmp[no]( pl_w ) ;
}

/*==============================================================*/
/*			Kept on the screen									*/
/*==============================================================*/
static void dm_away_scr_ck( PLAYER_WORK *pl_w )
{
	SCR_WORK *sw = &scr_work ;
	Float x ;

	x = sw->eye[0].x - 285.0f ;
	if( pl_w->pos.x <= x ) pl_w->pos.x = x ;
	else {
		x = sw->eye[0].x + 285.0f ;
		if( pl_w->pos.x >= x ) pl_w->pos.x = x ;
	}
}

/****************************************************************/
/*			The knock-back's animation, rising and falling,		*/
/*			by its angle										*/
/****************************************************************/
void dm_blow_char_set( PLAYER_WORK *pl_w )
{
	Uint8 dir ;
	Sint8 no ;

	dir = pl_w->dm_dir ;
	if( dir > 16 ) dir = ( 32 - dir ) & 31 ;
	no = ( dir + 2 ) / 4 ;
	pl_char_set( pl_w, Char_Damage, no + 16 ) ;
}

/****************************************************************/
void dm_fall_char_set( PLAYER_WORK *pl_w )
{
	Uint8 dir ;
	Sint8 no ;

	dir = pl_w->dm_dir ;
	if( dir > 16 ) dir = ( 32 - dir ) & 31 ;
	no = ( dir + 2 ) / 4 ;
	pl_char_set( pl_w, Char_Damage, no + 21 ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pl_dm.c" End of File	*/

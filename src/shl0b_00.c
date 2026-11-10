/* Captain America's effect 00 (character 0b's shot 00): his shield. One work follows him through
   all of its states (ew->type, which he picks in lw->shield_st); shl0b_01's parts ride on it. */
#include "player.h"
#include "pl_char.h"
#include "pl0b_def.h"
#include "shl0b_00.h"

/* where each of his shield throws starts, and how fast it goes */
typedef struct SHL0B_00_HIT {
	Uint8	atck_no;
	Sint16	x, y;
	Sint16	spd_x, spd_y;
} SHL0B_00_HIT;

static SHL0B_00_HIT shl0b_00_hit_tbl[] = {
	{ 70, 108, 68, 9, 0 }, { 70, 30, 58, 9, 3 }, { 71, 98, 26, 8, -1 },
	{ 70, 116, 86, 9, 0 }, { 70, 96, 128, 9, 3 }, { 71, 104, 50, 7, -3 },
} ;

static void shl0b_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl0b_00_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	PL0B_LOCAL_W *lw = PL0B_LW( pl_w ) ;

	if( ( ew = work_get( 0, W_SHL_ST, LIST_TOP ) ) ){
		ew->work_func = shl0b_00_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = 0 ;
		ew->sub_type = 0 ;
		ew->ef_no = 0xb00 ;
		lw->shield_sub = lw->shield_st = 0 ;
		lw->shield_w = ew ;
	}
	return ew ;
}

static void shl0b_00_frame_set( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw );
static int shl0b_00_disp_ck( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw );
static void shl0b_00_follow_move( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw );
static void shl0b_00_away_move( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw );

static void shl0b_00_init( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw );
static void shl0b_00_move( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw );
static void shl0b_00_end( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw );
static void shl0b_00_free( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw );

/*==============================================================*/
static void shl0b_00_main( PLAYER_WORK *ew )
{
	static void ( *shl0b_00_jmp[] )( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw ) = {
		shl0b_00_init, shl0b_00_move, shl0b_00_end, shl0b_00_free,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->prio = pl_w->prio ;
	shl0b_00_jmp[ew->routine_0]( ew, SHL0B_00_LW( ew ) ) ;
}

/*==============================================================*/
static void shl0b_00_init( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	shl0b_01_set( ew ) ;
	shl0b_00_move( ew, lw ) ;
}

static void shl0b_00_back( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw );
static int shl0b_00_wait_ck( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw );
static int shl0b_00_scr_ck( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw );
static void shl0b_00_stage_ck( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw );
static void shl0b_00_dm_ck( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw );
static void shl0b_00_dm_set( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw );
static void shl0b_00_throw_low_init( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw );
static void shl0b_00_throw_low_fall( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw );
static void shl0b_00_throw_low_fly( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw );

/*==============================================================*/
/*			The shield has two kinds of state (ew->type): 0		*/
/*			follows him, 1 is away from him; ew->sub_type is	*/
/*			the state											*/
/*==============================================================*/
static void shl0b_00_move( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	static void ( *shl0b_00_state_jmp[] )( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw ) = {
		shl0b_00_follow_move, shl0b_00_away_move,
	} ;
	PL0B_LOCAL_W *plw = PL0B_LW( ew->master_w ) ;

	plw->shield_type = ew->type ;
	if( ew->type != plw->shield_st ){
		ew->type = plw->shield_st ;
		ew->sub_type = plw->shield_sub ;
		shl0b_00_back( ew, lw ) ;
	}
	ew->disp_flag = ew->master_w->disp_flag ;
	shl0b_00_state_jmp[ew->type]( ew, lw ) ;
}

/*==============================================================*/
/*			A state that does nothing							*/
/*==============================================================*/
static void shl0b_00_nop( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
}

/*==============================================================*/
/*			He is no longer in Charging Star or its hyper:		*/
/*			the shield goes back to its first state				*/
/*==============================================================*/
static void shl0b_00_state_ck( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	register int sp ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_1 != PL_NORM ) shl0b_00_back( ew, lw ) ;
	if( pl_w->act_st != N_SPECIAL ) shl0b_00_back( ew, lw ) ;
	sp = pl_w->pl_sp_type ;
	if( sp == 3 || sp == 8 ) return ;
	shl0b_00_back( ew, lw ) ;
}

/*==============================================================*/
static void shl0b_00_pose_ck( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->char_no.b.grp == Char_Win && pl_w->char_no.b.no == 4 ) return ;
	shl0b_00_back( ew, lw ) ;
}

/*==============================================================*/
static void shl0b_00_pose_set( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw, Uint8 no )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	lw->no = 0xffff ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->disp_dir = pl_w->disp_dir ;
	ew->rot[2] = pl_w->rot[2] ;
	ew->disp_flag = 1 ;
	ew->char_no.b.grp = 23 ;
	ew->char_no.b.no = no ;
	pl_char_set2( ew, ew->char_no.b.grp, ew->char_no.b.no, pl_w->cg_type & 127 ) ;
	if( ew->cg_type < 0 ) ew->sub_prio = 2 ;
	else ew->sub_prio = -2 ;
	shl0b_00_disp_ck( ew, lw ) ;
}

/*--------------------------------------------------------------*/
static void shl0b_00_hand_move( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->cg_type < 0 ){
		if( pl_w->char_no.w == 0x1605 ){ shl0b_00_pose_set( ew, lw, 19 ) ; return ; }
		if( pl_w->char_no.w == 0x704 ){ shl0b_00_pose_set( ew, lw, 12 ) ; return ; }
		if( pl_w->char_no.w == 0xb06 ){ shl0b_00_pose_set( ew, lw, 19 ) ; return ; }
		if( pl_w->char_no.w == 0xb07 ){ shl0b_00_pose_set( ew, lw, 13 ) ; return ; }
		if( pl_w->char_no.w == 0x904 ){ shl0b_00_pose_set( ew, lw, 14 ) ; return ; }
		if( pl_w->char_no.w == 0x905 ){ shl0b_00_pose_set( ew, lw, 15 ) ; return ; }
		if( pl_w->char_no.w == 0x1408 ){ shl0b_00_pose_set( ew, lw, 15 ) ; return ; }
	}
	shl0b_00_frame_set( ew, lw ) ;
}

/*==============================================================*/
static void shl0b_00_hand_init( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	ew->routine_1 ++ ;
	lw->no = 0xffff ;
	shl0b_00_hand_move( ew, lw ) ;
}

/*==============================================================*/
static void shl0b_00_hand( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	static void ( *hand_jmp[] )( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw ) = {
		shl0b_00_hand_init, shl0b_00_hand_move,
	} ;

	hand_jmp[ew->routine_1]( ew, lw ) ;
}

/*--------------------------------------------------------------*/
static void shl0b_00_charge_move( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( !pl_w->cg_prm ){
		shl0b_00_frame_set( ew, lw ) ;
		return ;
	}
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	if( pl_w->cg_prm == 1 ){
		pl_w->cg_prm = 2 ;
		ew->char_no.b.grp = 23 ;
		ew->char_no.b.no = 31 ;
		pl_char_set( ew, ew->char_no.b.grp, ew->char_no.b.no ) ;
	}
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
	shl0b_00_disp_ck( ew, lw ) ;
}

/*==============================================================*/
static void shl0b_00_charge_init( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	ew->routine_1 ++ ;
	lw->no = 0xffff ;
	shl0b_00_charge_move( ew, lw ) ;
}

/*==============================================================*/
static void shl0b_00_charge( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	static void ( *charge_jmp[] )( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw ) = {
		shl0b_00_charge_init, shl0b_00_charge_move,
	} ;

	charge_jmp[ew->routine_1]( ew, lw ) ;
}

/*--------------------------------------------------------------*/
static void shl0b_00_win_move( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->cg_type < 0 ){
		pl_w->cg_type = 1 ;
		pls0b_00_set2( ew, 3, 1 ) ;
	}
	shl0b_00_frame_set( ew, lw ) ;
}

/*==============================================================*/
static void shl0b_00_win_init( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	ew->routine_1 ++ ;
	lw->no = 0xffff ;
	shl0b_00_win_move( ew, lw ) ;
}

/*==============================================================*/
static void shl0b_00_win( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	static void ( *win_jmp[] )( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw ) = {
		shl0b_00_win_init, shl0b_00_win_move,
	} ;

	win_jmp[ew->routine_1]( ew, lw ) ;
}

/*==============================================================*/
static void shl0b_00_follow_state( register PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	static void ( *follow_state_jmp[] )( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw ) = {
		shl0b_00_hand, shl0b_00_charge, shl0b_00_win,
	} ;

	follow_state_jmp[ew->sub_type]( ew, lw ) ;
}

/*==============================================================*/
/*			The shield's work goes on as he goes on: one		*/
/*			check and one action for each state					*/
/*==============================================================*/
static void shl0b_00_follow_move( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	static void ( *ck_jmp[] )( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw ) = {
		shl0b_00_nop, shl0b_00_state_ck, shl0b_00_pose_ck,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;
	PL0B_LOCAL_W *plw = PL0B_LW( pl_w ) ;

	plw->shield_st = 0 ;
	lw->leave_flag = 0 ;
	lw->blink_timer = 0 ;
	if( ew->sub_type != plw->shield_sub ){
		ew->sub_type = plw->shield_sub ;
		ew->routine_1 = 0 ;
	}
	if( pl_w->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		return ;
	}
	if( !pl_w->disp_flag ){
		ew->disp_flag = 0 ;
		return ;
	}
	ck_jmp[ew->sub_type]( ew, lw ) ;
	shl0b_00_follow_state( ew, lw ) ;
}

/*--------------------------------------------------------------*/
static void shl0b_00_ground_wait( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	shl0b_00_dm_ck( ew, lw ) ;
	if( !shl0b_00_disp_ck( ew, lw ) ){
		if( !pl_w->be_flag && !pl_w->vital_new ) ew->routine_0 ++ ;
		if( !-- ew->timer0 ){
			ew->sub_type = 9 ;
			shl0b_00_back( ew, lw ) ;
			shl0b_00_dm_ck( ew, lw ) ;
		}
	} else {
		ew->timer0 = 0x1e0 ;
		if( !pl_w->vital_new && !pl_w->be_flag ) ew->routine_0 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void shl0b_00_ground_land( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		ew->timer0 = 0x1e0 ;
		ew->char_no.b.grp = 23 ;
		ew->char_no.b.no = 24 ;
		pl_char_set( ew, ew->char_no.b.grp, ew->char_no.b.no ) ;
		pls0b_00_set2( ew, 5, 0 ) ;
		pls0b_00_set2( ew, 1, 0 ) ;
		shl0b_00_ground_wait( ew, lw ) ;
		return ;
	}
	shl0b_00_dm_ck( ew, lw ) ;
	shl0b_00_disp_ck( ew, lw ) ;
}

/*==============================================================*/
static void shl0b_00_ground_init( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->routine_1 ++ ;
	lw->unk_0a = 0 ;
	lw->unk_0c = 0 ;
	ew->pos.y = pl_w->ground_y ;
	ew->char_no.b.grp = 23 ;
	ew->char_no.b.no = 26 ;
	pl_char_set( ew, ew->char_no.b.grp, ew->char_no.b.no ) ;
	shl0b_00_ground_land( ew, lw ) ;
}

/*==============================================================*/
static void shl0b_00_ground( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	static void ( *ground_jmp[] )( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw ) = {
		shl0b_00_ground_init, shl0b_00_ground_land, shl0b_00_ground_wait,
	} ;

	ground_jmp[ew->routine_1]( ew, lw ) ;
}

/*==============================================================*/
/*			Sets the hit of one of his throws of the shield:	*/
/*			where it starts, how fast it goes					*/
/*==============================================================*/
static void shl0b_00_hit_set( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw, Uint8 no )
{
	Sint16 x, y, spd_x, spd_y ;

	ew->atck_no = shl0b_00_hit_tbl[no].atck_no ;
	HIT_RESET( ew ) ;
	x = shl0b_00_hit_tbl[no].x ;
	y = shl0b_00_hit_tbl[no].y ;
	spd_x = shl0b_00_hit_tbl[no].spd_x ;
	spd_y = shl0b_00_hit_tbl[no].spd_y ;
	if( !ew->disp_dir ){
		x = -x ;
		spd_x = -spd_x ;
	}
	ew->pos.x += DOT_X( x ) ;
	ew->pos.y += DOT_Y( y ) ;
	ew->spd.x = DOT_X( spd_x ) ;
	ew->spd.y = DOT_Y( spd_y ) ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
}

/*--------------------------------------------------------------*/
static void shl0b_00_throw_fly( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( ew->hit_flag < 0 ) goto set ;
	if( ew->hit_box_no ){
set :
		pls0b_00_set2( ew, 10, ew->sub_type ) ;
	} else {
		if( 1 ) pl_char_move( ew ) ;
		else pl_char_move( pl_w ) ;
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		if( !shl0b_00_scr_ck( ew, lw ) ){
			hit_work_set( ew ) ;
			shl0b_00_disp_ck( ew, lw ) ;
			return ;
		}
	}
	hit_work_set( ew ) ;
	shl0b_00_disp_ck( ew, lw ) ;
	ew->sub_type = 7 ;
	shl0b_00_back( ew, lw ) ;
	shl0b_00_dm_ck( ew, lw ) ;
}

/*==============================================================*/
static void shl0b_00_throw_init( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->routine_2 ++ ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->disp_dir = pl_w->disp_dir ;
	shl0b_00_hit_set( ew, lw, ew->sub_type + 255 ) ;
	switch( ew->sub_type ){
		case 1 :
		case 4 :
			ew->char_no.b.no = 21 ;
			break ;
		case 2 :
		case 5 :
			ew->char_no.b.no = 22 ;
			break ;
		case 3 :
		case 6 :
			ew->char_no.b.no = 23 ;
			break ;
	}
	ew->char_no.b.grp = 23 ;
	pl_char_set( ew, ew->char_no.b.grp, ew->char_no.b.no ) ;
	shl0b_00_dm_set( ew, lw ) ;
	shl0b_00_throw_fly( ew, lw ) ;
}

/*==============================================================*/
static void shl0b_00_throw( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	static void ( *throw_jmp[] )( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw ) = {
		shl0b_00_throw_init, shl0b_00_throw_fly, shl0b_00_throw_low_init, shl0b_00_throw_low_fall, shl0b_00_throw_low_fly,
	} ;

	throw_jmp[ew->routine_2]( ew, lw ) ;
}

/*==============================================================*/
/*			The shield is done: it is drawn once more, and		*/
/*			goes back to its state								*/
/*==============================================================*/
static void shl0b_00_throw_end( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	hit_work_set( ew ) ;
	shl0b_00_disp_ck( ew, lw ) ;
	ew->sub_type = 7 ;
	shl0b_00_back( ew, lw ) ;
	shl0b_00_dm_ck( ew, lw ) ;
}

/*==============================================================*/
static void shl0b_00_throw_dm_end( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	pls0b_00_set2( ew, 10, ew->sub_type ) ;
	shl0b_00_throw_end( ew, lw ) ;
}

/*==============================================================*/
static void shl0b_00_throw_catch_end( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	catch_hitmark( ew ) ;
	shl0b_00_throw_end( ew, lw ) ;
}

/*==============================================================*/
static void shl0b_00_throw_scr_end( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	if( !shl0b_00_scr_ck( ew, lw ) ){
		hit_work_set( ew ) ;
		shl0b_00_disp_ck( ew, lw ) ;
	}
	shl0b_00_throw_end( ew, lw ) ;
}

/*--------------------------------------------------------------*/
static void shl0b_00_throw_low_fly( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	if( ew->hit_flag < 0 ){
		shl0b_00_throw_dm_end( ew, lw ) ;
		return ;
	}
	if( ew->hit_box_no ){
		shl0b_00_throw_catch_end( ew, lw ) ;
		return ;
	}
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	shl0b_00_throw_scr_end( ew, lw ) ;
}

/*--------------------------------------------------------------*/
static void shl0b_00_throw_low_fall( register PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	register PLAYER_WORK *pl_w = ew->master_w ;

	if( ew->hit_flag < 0 ){
		shl0b_00_throw_dm_end( ew, lw ) ;
		return ;
	}
	if( ew->hit_box_no ){
		shl0b_00_throw_catch_end( ew, lw ) ;
		return ;
	}
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( pl_w->ground_y + 0.20089285f > ew->pos.y ){
		ew->pos.y = pl_w->ground_y + 0.20089285f ;
		ew->routine_2 ++ ;
		ew->spd.y = 0.0f ;
		ew->acc.y = 0.0f ;
	}
	shl0b_00_throw_scr_end( ew, lw ) ;
}

/*--------------------------------------------------------------*/
static void shl0b_00_throw_low_init( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->routine_2 ++ ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->disp_dir = pl_w->disp_dir ;
	shl0b_00_hit_set( ew, lw, 2 ) ;
	ew->char_no.b.grp = 23 ;
	ew->char_no.b.no = 21 ;
	pl_char_set( ew, ew->char_no.b.grp, ew->char_no.b.no ) ;
	shl0b_00_dm_set( ew, lw ) ;
	shl0b_00_throw_low_fall( ew, lw ) ;
}

/*==============================================================*/
/*			The states that throw the shield: each starts		*/
/*			the flight, and goes on in shl0b_00_throw			*/
/*==============================================================*/
static void shl0b_00_slash_fwd_move( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	shl0b_00_throw( ew, lw ) ;
}

/*==============================================================*/
static void shl0b_00_slash_fwd_init( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	ew->routine_1 ++ ;
	ew->routine_2 = 0 ;
	ew->sub_prio = -2 ;
	shl0b_00_slash_fwd_move( ew, lw ) ;
}

/*==============================================================*/
static void shl0b_00_slash_fwd( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	static void ( *slash_fwd_jmp[] )( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw ) = {
		shl0b_00_slash_fwd_init, shl0b_00_slash_fwd_move,
	} ;

	slash_fwd_jmp[ew->routine_1]( ew, lw ) ;
}

/*--------------------------------------------------------------*/
static void shl0b_00_slash_up_move( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	shl0b_00_throw( ew, lw ) ;
}

/*==============================================================*/
static void shl0b_00_slash_up_init( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	ew->routine_1 ++ ;
	ew->routine_2 = 0 ;
	ew->sub_prio = -2 ;
	shl0b_00_slash_up_move( ew, lw ) ;
}

/*==============================================================*/
static void shl0b_00_slash_up( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	static void ( *slash_up_jmp[] )( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw ) = {
		shl0b_00_slash_up_init, shl0b_00_slash_up_move,
	} ;

	slash_up_jmp[ew->routine_1]( ew, lw ) ;
}

/*--------------------------------------------------------------*/
static void shl0b_00_slash_low_move( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
}

/*==============================================================*/
static void shl0b_00_slash_low_init( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	ew->routine_1 ++ ;
	ew->routine_2 = 0 ;
	ew->sub_prio = -2 ;
	shl0b_00_slash_low_move( ew, lw ) ;
}

/*==============================================================*/
static void shl0b_00_slash_low( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	static void ( *slash_low_jmp[] )( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw ) = {
		shl0b_00_slash_low_init, shl0b_00_slash_low_move,
	} ;

	slash_low_jmp[ew->routine_1]( ew, lw ) ;
}

/*--------------------------------------------------------------*/
static void shl0b_00_jslash_fwd_move( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	shl0b_00_throw( ew, lw ) ;
}

/*==============================================================*/
static void shl0b_00_jslash_fwd_init( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	ew->routine_1 ++ ;
	ew->routine_2 = 0 ;
	ew->sub_prio = -2 ;
	shl0b_00_jslash_fwd_move( ew, lw ) ;
}

/*==============================================================*/
static void shl0b_00_jslash_fwd( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	static void ( *jslash_fwd_jmp[] )( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw ) = {
		shl0b_00_jslash_fwd_init, shl0b_00_jslash_fwd_move,
	} ;

	jslash_fwd_jmp[ew->routine_1]( ew, lw ) ;
}

/*--------------------------------------------------------------*/
static void shl0b_00_jslash_up_move( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	shl0b_00_throw( ew, lw ) ;
}

/*==============================================================*/
static void shl0b_00_jslash_up_init( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	ew->routine_1 ++ ;
	ew->routine_2 = 0 ;
	ew->sub_prio = -2 ;
	shl0b_00_jslash_up_move( ew, lw ) ;
}

/*==============================================================*/
static void shl0b_00_jslash_up( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	static void ( *jslash_up_jmp[] )( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw ) = {
		shl0b_00_jslash_up_init, shl0b_00_jslash_up_move,
	} ;

	jslash_up_jmp[ew->routine_1]( ew, lw ) ;
}

/*--------------------------------------------------------------*/
static void shl0b_00_jslash_down_move( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	shl0b_00_throw( ew, lw ) ;
}

/*==============================================================*/
static void shl0b_00_jslash_down_init( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	ew->routine_1 ++ ;
	ew->routine_2 = 0 ;
	ew->sub_prio = -2 ;
	shl0b_00_jslash_down_move( ew, lw ) ;
}

/*==============================================================*/
static void shl0b_00_jslash_down( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	static void ( *jslash_down_jmp[] )( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw ) = {
		shl0b_00_jslash_down_init, shl0b_00_jslash_down_move,
	} ;

	jslash_down_jmp[ew->routine_1]( ew, lw ) ;
}

/*--------------------------------------------------------------*/
static void shl0b_00_rebound_move( register PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	register PLAYER_WORK *pl_w = ew->master_w ;

	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	shl0b_00_stage_ck( ew, lw ) ;
	if( pl_w->ground_y + DOT_Y( 16.0f ) > ew->pos.y ){
		ew->pos.y = pl_w->ground_y + DOT_Y( 16.0f ) ;
		ew->sub_type = 0 ;
		shl0b_00_back( ew, lw ) ;
	}
	shl0b_00_disp_ck( ew, lw ) ;
	shl0b_00_dm_ck( ew, lw ) ;
}

/*==============================================================*/
static void shl0b_00_rebound_init( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Sint16 dx, vx, vy ;

	ew->routine_1 ++ ;
	dx = pl_w->pos.x - ew->pos.x ;
	if( dx < 0 ) dx = -dx ;
	dx = dx * 3 * 16 ;
	dx /= 96 ;
	vy = dx << 3 ;
	ew->spd.y = vy * 2.142857f / 256.0f ;
	ew->acc.y = -DOT_Y( 0.375f ) ;
	vx = 0x500 ;
	if( ew->disp_dir ) vx = -0x500 ;
	ew->spd.x = vx * 1.6666666f / 256.0f ;
	ew->acc.x = 0.0f ;
	ew->char_no.b.grp = 23 ;
	ew->char_no.b.no = 3 ;
	pl_char_set( ew, ew->char_no.b.grp, ew->char_no.b.no ) ;
	shl0b_00_rebound_move( ew, lw ) ;
}

/*==============================================================*/
static void shl0b_00_rebound( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	static void ( *rebound_jmp[] )( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw ) = {
		shl0b_00_rebound_init, shl0b_00_rebound_move,
	} ;

	rebound_jmp[ew->routine_1]( ew, lw ) ;
}

/*--------------------------------------------------------------*/
static void shl0b_00_appear_fly( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	if( ew->timer0 > 0 ){
		ew->timer0 -- ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( scr_work.scr_left - 160.0f > ew->pos.x ){
		ew->pos.x = scr_work.scr_left ;
		ew->sub_type = 7 ;
		shl0b_00_back( ew, lw ) ;
	} else if( scr_work.scr_right + 160.0f < ew->pos.x ){
		ew->pos.x = scr_work.scr_right ;
		ew->sub_type = 7 ;
		shl0b_00_back( ew, lw ) ;
	}
	shl0b_00_dm_ck( ew, lw ) ;
	pl_char_move( ew ) ;
	shl0b_00_disp_ck( ew, lw ) ;
}

/*--------------------------------------------------------------*/
static void shl0b_00_appear_wait( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	if( vs_sys_work.vs_st == VS_APPEAR ){
		ew->routine_1 ++ ;
		shl0b_00_appear_fly( ew, lw ) ;
	}
}

/*==============================================================*/
static void shl0b_00_appear_init( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Sint32 ofs = 0x1800000 ;

	ew->routine_1 ++ ;
	if( !ew->disp_dir ) ofs = -0x1800000 ;
	ew->pos.x = pl_w->pos.x + ofs * 1.6666666f / 65536.0f ;
	ew->pos.y = pl_w->pos.y + DOT_Y( 160.0f ) ;
	if( pl_w->disp_dir ) ew->spd.x = -DOT_X( 7.0f ) ;
	else ew->spd.x = DOT_X( 7.0f ) ;
	ew->acc.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->timer0 = 32 ;
	ew->char_no.b.grp = 23 ;
	ew->char_no.b.no = 9 ;
	pl_char_set( ew, ew->char_no.b.grp, ew->char_no.b.no ) ;
}

/*==============================================================*/
static void shl0b_00_appear( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	static void ( *appear_jmp[] )( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw ) = {
		shl0b_00_appear_init, shl0b_00_appear_wait, shl0b_00_appear_fly,
	} ;

	appear_jmp[ew->routine_1]( ew, lw ) ;
}

/* The shield comes down from the top of the screen where he is */

/*--------------------------------------------------------------*/
static void shl0b_00_drop_fall( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	shl0b_00_stage_ck( ew, lw ) ;
	if( pl_w->ground_y + DOT_Y( 16.0f ) > ew->pos.y ){
		ew->pos.y = pl_w->ground_y + DOT_Y( 16.0f ) ;
		ew->sub_type = 0 ;
		shl0b_00_back( ew, lw ) ;
	}
	shl0b_00_dm_ck( ew, lw ) ;
	if( ew->disp_flag ) shl0b_00_disp_ck( ew, lw ) ;
}

/*==============================================================*/
static void shl0b_00_drop_init( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->routine_1 ++ ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = scr_work.scr_top + DOT_Y( 64.0f ) ;
	ew->spd.x = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = -DOT_Y( 8.0f ) ;
	ew->acc.y = 0.0f ;
	ew->char_no.b.grp = 23 ;
	ew->char_no.b.no = 3 ;
	pl_char_set( ew, ew->char_no.b.grp, ew->char_no.b.no ) ;
	shl0b_00_dm_ck( ew, lw ) ;
	if( ew->disp_flag ) shl0b_00_disp_ck( ew, lw ) ;
}

/*==============================================================*/
static void shl0b_00_drop( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	static void ( *drop_jmp[] )( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw ) = {
		shl0b_00_drop_init, shl0b_00_drop_fall,
	} ;

	drop_jmp[ew->routine_1]( ew, lw ) ;
}

/*==============================================================*/
/*			The frame he shows ends: the shield's frame is		*/
/*			dropped, or it stays while the timer runs			*/
/*==============================================================*/
static void shl0b_00_frame_ck( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( lw->no >= 0 ){
		if( pl_w->cg_rival != (Sint16)lw->no ) goto done ;
		if( --ew->timer0 >= 0 ) return ;
	}
	lw->no = 0xffff ;
done:
	shl0b_00_dm_ck( ew, lw ) ;
}

/*--------------------------------------------------------------*/
static void shl0b_00_hop_move( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	shl0b_00_stage_ck( ew, lw ) ;
	if( ew->pos.y > pl_w->ground_y + DOT_Y( 16.0f ) ){
		shl0b_00_frame_ck( ew, lw ) ;
		shl0b_00_disp_ck( ew, lw ) ;
		return ;
	}
	ew->pos.y = pl_w->ground_y + DOT_Y( 16.0f ) ;
	shl0b_00_disp_ck( ew, lw ) ;
	ew->sub_type = 0 ;
	shl0b_00_back( ew, lw ) ;
	shl0b_00_frame_ck( ew, lw ) ;
}

/*==============================================================*/
static void shl0b_00_hop_init( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->routine_1 ++ ;
	ew->routine_2 = 0 ;
	lw->no = pl_w->cg_rival ;
	ew->timer0 = 8 ;
	ew->spd.x = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = DOT_Y( 8.0f ) ;
	ew->acc.y = -DOT_Y( 0.5f ) ;
	ew->char_no.b.grp = 23 ;
	ew->char_no.b.no = 7 ;
	pl_char_set( ew, ew->char_no.b.grp, ew->char_no.b.no ) ;
	shl0b_00_hop_move( ew, lw ) ;
}

/*==============================================================*/
static void shl0b_00_hop( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	static void ( *hop_jmp[] )( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw ) = {
		shl0b_00_hop_init, shl0b_00_hop_move,
	} ;

	hop_jmp[ew->routine_1]( ew, lw ) ;
}

/*--------------------------------------------------------------*/
static void shl0b_00_lost_wait( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	if( !-- ew->timer0 ){
		ew->sub_type = 9 ;
		shl0b_00_back( ew, lw ) ;
		shl0b_00_dm_ck( ew, lw ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl0b_00_lost_fly( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.x <= scr_work.scr_left ){
		ew->pos.x = scr_work.scr_left ;
		ew->routine_1 ++ ;
		ew->timer0 = 0x258 ;
		return ;
	}
	if( ew->pos.x >= scr_work.scr_right ){
		ew->pos.x = scr_work.scr_right ;
		ew->routine_1 ++ ;
		ew->timer0 = 0x258 ;
		return ;
	}
	shl0b_00_disp_ck( ew, lw ) ;
}

/*==============================================================*/
static void shl0b_00_lost_init( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->routine_1 ++ ;
	ew->routine_2 = 0 ;
	if( !pl_w->disp_dir ){
		ew->spd.x = 10.0f ;
		ew->acc.x = DOT_X( 0.5f ) ;
	} else {
		ew->spd.x = -10.0f ;
		ew->acc.x = -DOT_X( 0.5f ) ;
	}
	ew->spd.y = DOT_Y( 12.0f ) ;
	ew->acc.y = -DOT_Y( 0.25f ) ;
	ew->char_no.b.grp = 23 ;
	ew->char_no.b.no = 7 ;
	pl_char_set( ew, ew->char_no.b.grp, ew->char_no.b.no ) ;
	shl0b_00_lost_fly( ew, lw ) ;
}

/*==============================================================*/
static void shl0b_00_lost( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	static void ( *lost_jmp[] )( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw ) = {
		shl0b_00_lost_init, shl0b_00_lost_fly, shl0b_00_lost_wait,
	} ;

	lost_jmp[ew->routine_1]( ew, lw ) ;
}

/*==============================================================*/
static void shl0b_00_away_move( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	static void ( *move_jmp[] )( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw ) = {
		shl0b_00_ground, shl0b_00_slash_fwd, shl0b_00_slash_up, shl0b_00_slash_low, shl0b_00_jslash_fwd, shl0b_00_jslash_up,
		shl0b_00_jslash_down, shl0b_00_rebound, shl0b_00_appear, shl0b_00_drop, shl0b_00_hop, shl0b_00_lost,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;
	PL0B_LOCAL_W *plw = PL0B_LW( pl_w ) ;

	plw->shield_st = 1 ;
	if( vs_sys_work.game_stop & ( 1 << ( pl_w->pl_no ^ 1 ) ) ) return ;
	if( shl0b_00_wait_ck( ew, lw ) ) return ;
	move_jmp[ew->sub_type]( ew, lw ) ;
}

/*==============================================================*/
static int shl0b_00_disp_ck( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	if( !scr_in_x_ck( ew ) || ( lw->leave_flag && ( lw->blink_timer & 1 ) ) ) ew->disp_flag = 0 ;
	else ew->disp_flag = 1 ;
	return ew->disp_flag ;
}

/*==============================================================*/
/*			The shield's frame, as he shows it: placed by		*/
/*			the frame's offset, turned with him					*/
/*==============================================================*/
static void shl0b_00_frame_set( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	SHL0B_00_FRAME *frame = pl_w->ef_pos_tbl ;
	Sint16 no, ofs_x, ofs_y, x, y, dx, dy ;

	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->disp_dir = pl_w->disp_dir ;
	ew->rot[2] = pl_w->rot[2] ;
	no = pl_w->cg_rival ;
	if( !no ){
		ew->disp_flag = 0 ;
		lw->no = 0xffff ;
		return ;
	} else ew->disp_flag = 1 ;
	if( lw->no != no ){
		lw->no = no ;
		frame += no ;
		lw->frame = frame ;
		lw->x = frame->x ;
		lw->y = frame->y ;
		lw->sub_prio = frame->sub_prio ;
		ew->char_no.b.grp = 26 ;
		ew->char_no.b.no = frame->anm[0] ;
		pl_char_set2( ew, ew->char_no.b.grp, ew->char_no.b.no, frame->anm[1] ) ;
	}
	ew->sub_prio = lw->sub_prio ;
	ofs_x = lw->x ;
	ofs_y = lw->y ;
	if( ew->disp_dir ) ofs_x = -ofs_x ;
	x = DOT_X( ofs_x ) ;
	y = -ofs_y * 2.142857f ;
	dx = ( x + pl_w->rot_ofs_x ) * nlCos( ew->rot[2] ) + ( y + pl_w->rot_ofs_y * 2 ) * nlSin( ew->rot[2] ) - pl_w->rot_ofs_x ;
	dy = ( -x - pl_w->rot_ofs_x ) * nlSin( ew->rot[2] ) + ( y + pl_w->rot_ofs_y * 2 ) * nlCos( ew->rot[2] ) - pl_w->rot_ofs_y * 2 ;
	ew->pos.x += dx ;
	ew->pos.y += dy ;
	shl0b_00_disp_ck( ew, lw ) ;
}

/*==============================================================*/
static int shl0b_00_wait_ck( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( !lw->leave_flag ){
		if( ( pl_w->act_st == N_CHANGE && pl_w->change_type == 2 ) || !pl_w->be_flag ){
			lw->leave_flag = -1 ;
			lw->blink_timer = 20 ;
		} else return 0 ;
	}
	if( !lw->blink_timer || -- lw->blink_timer ) return 0 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.y += DOT_Y( 64.0f ) ;
	ew->type = 0 ;
	shl0b_00_back( ew, lw ) ;
	return 1 ;
}

/*==============================================================*/
static int shl0b_00_hit_ck( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	FRAME_BOX *at = ew->frame_box ;
	FRAME_BOX *dm = pl_w->frame_box ;

	if( box_ck( ew, pl_w, at->catch_box & 0xff, dm->body_box0 & 0xff ) ) return 1 ;
	if( box_ck( ew, pl_w, at->catch_box & 0xff, dm->body_box1 & 0xff ) ) return 1 ;
	if( box_ck( ew, pl_w, at->catch_box & 0xff, dm->body_box2 & 0xff ) ) return 1 ;
	if( dm->body_box3 ){
		if( box_ck( ew, pl_w, at->catch_box & 0xff, dm->body_box3 & 0xff ) ) return 1 ;
	}
	return 0 ;
}

/*==============================================================*/
static void shl0b_00_dm_ck( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	int hit = shl0b_00_hit_ck( ew, lw ) ;

	if( !hit ) return ;
	if( shl0b_00_hit_ck( ew, lw ) < 0 ){
		ew->sub_type = 10 ;
		shl0b_00_back( ew, lw ) ;
		return ;
	}
	ew->type = 0 ;
	shl0b_00_back( ew, lw ) ;
	pl_voice_req( ew, 31 ) ;
}

/*==============================================================*/
static void shl0b_00_dm_set( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	ew->hit_box_no = 0 ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
}

/*==============================================================*/
static int shl0b_00_scr_ck( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	int r = 0 ;

	if( ew->pos.x <= scr_work.scr_left ){
		r = 1 ;
		ew->pos.x = scr_work.scr_left ;
	} else if( ew->pos.x >= scr_work.scr_right ){
		r = 1 ;
		ew->pos.x = scr_work.scr_right ;
	}
	if( pl_w->ground_y >= ew->pos.y ){
		r += 2 ;
		ew->pos.y = pl_w->ground_y ;
	} else if( ew->pos.y >= scr_work.scr_top ){
		r += 2 ;
		ew->pos.y = scr_work.scr_top ;
	}
	return r ;
}

/*==============================================================*/
static void shl0b_00_stage_ck( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	Float lim = scr_work.stage_left + DOT_X( 32.0f ) ;

	if( ew->pos.x < lim ){
		ew->pos.x = lim ;
		return ;
	}
	lim = scr_work.stage_right + -DOT_X( 32.0f ) ;
	if( ew->pos.x > lim ) ew->pos.x = lim ;
}

/*--------------------------------------------------------------*/
static void shl0b_00_end( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl0b_00_free( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			Back to the first state; he gets the state the		*/
/*			shield is in										*/
/*==============================================================*/
static void shl0b_00_back( PLAYER_WORK *ew, SHL0B_00_LOCAL_W *lw )
{
	PL0B_LOCAL_W *plw = PL0B_LW( ew->master_w ) ;

	if( !ew->type ) ew->sub_type = 0 ;
	plw->shield_st = ew->type ;
	plw->shield_sub = ew->sub_type ;
	ew->routine_1 = 0 ;
}

/****************************************************************/
/*			Asks for state a, if the shield is free; gives		*/
/*			the state it is in otherwise						*/
/****************************************************************/
int shield_req( PL0B_LOCAL_W *lw, Uint8 a, Uint8 b )
{
	if( lw->shield_st ) return lw->shield_st ;
	lw->shield_st = a ;
	lw->shield_sub = b ;
	return 0 ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl0b_00.c" End of File	*/

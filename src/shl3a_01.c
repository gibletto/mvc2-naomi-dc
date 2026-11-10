/* Servbot's effect 01 (0x3a01): the pieces of Servbot's attack. The setters put out one
   piece or a burst of forty; shl3a_01_mode_set starts a piece in a mode (routine_1) and
   shl3a_01_mode_jmp moves it: a piece runs in from the screen's edge, flies, lands, bounces,
   falls, or holds the opponent. */
#include "player.h"
#include "pl_char.h"
#include "pl3a_def.h"

/* the effect's own work, from obj_flag */
typedef struct SHL3A_01_LOCAL_W {
	Uint32	unk_00;			/* 000 */
	PLAYER_WORK	*hold_w;	/* 004: the player it holds */
	PLAYER_WORK	*part_w;	/* 008: the piece it follows */
} SHL3A_01_LOCAL_W;
#define SHL3A_01_LW( w )	( ( SHL3A_01_LOCAL_W * )&( w )->obj_flag )

static void shl3a_01_main( PLAYER_WORK *ew );
static void shl3a_01_line_main( PLAYER_WORK *ew );
static void shl3a_01_mode_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w, Uint32 no );

/* where the pieces of a burst start and how fast they fly, by type: the first twenty */
static Sint16 shl3a_01_pos_y_tbl[] = { 0, 32, 0, 0, 16, 16, 48, 48 } ;
static Sint16 shl3a_01_spd_y_tbl[] = { 2048, 1024, 1536, 1536, 2048, 1024, 1024, 2048 } ;
static Sint16 shl3a_01_acc_y_tbl[] = { -96, -64, -80, -80, -96, -64, -64, -96 } ;
/* and the second twenty */
static Sint16 shl3a_01_pos_y_tbl2[] = { 96, 88, 128, 96, 80, 112, 136, 120 } ;
static Sint16 shl3a_01_spd_y_tbl2[] = { -768, -544, -512, -64, -256, -512, -768, -768 } ;
static Sint16 shl3a_01_acc_y_tbl2[] = { 48, 32, 48, 0, 64, 48, 96, 16 } ;

/****************************************************************/
/*			The burst of forty pieces							*/
/****************************************************************/
PLAYER_WORK *shl3a_01_burst_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	int i ;
	int spd ;

	for( i = 0 ; i < 20 ; i ++ ){
		if( ew = work_get( 0, W_SHL, LIST_END ) ){
			ew->work_func = shl3a_01_main ;
			ew->master_w = pl_w ;
			ew->pl_type = pl_w->pl_type ;
			ew->ef_no = 0x3a01 ;
			ew->type = Rnd() & 7 ;
			if( !( i & 3 ) ) ew->timer0 = 1 ;
			ew->timer1 = i * 4 ;
			CHAR_PART_COPY( ew, pl_w ) ;
			ew->disp_flag = 1 ;
			ew->pl_no = pl_w->pl_no ;
			ew->pl_type = pl_w->pl_type ;
			ew->scale.x = pl_w->scale.x ;
			ew->scale.y = pl_w->scale.y ;
			ew->atck_level = pl_w->atck_level ;
			ew->work_no = pl_w->work_no ;
			ew->col_cnt = pl_w->col_cnt ;
			ew->scale = pl_w->scale ;
			ew->prio = pl_w->prio ;
			ew->prio = 8 ;
			shl3a_01_mode_set( ew, pl_w, 2 ) ;
			spd = ( ( Rnd() & 0x1ff ) + 0x700 ) * 1.6666666f / 256.0f ;
			if( !ew->disp_dir ) spd = -spd ;
			ew->spd.x = spd ;
			ew->spd.y = shl3a_01_spd_y_tbl[ew->type] * 2.142857f / 256.0f ;
			ew->acc.y = shl3a_01_acc_y_tbl[ew->type] * 2.142857f / 256.0f ;
			ew->pos.y = pl_w->ground_y + 2.142857f * shl3a_01_pos_y_tbl[ew->type] ;
			if( ew->sub_type >= 24 ){
				ew->pos.y = pl_w->ground_y ;
				ew->acc.y = 0.0f ;
				ew->spd.y = 0.0f ;
			}
		}
	}
	for( i = 0 ; i < 80 ; i += 4 ){
		if( ew = work_get( 0, W_SHL, LIST_END ) ){
			ew->work_func = shl3a_01_main ;
			ew->master_w = pl_w ;
			ew->pl_type = pl_w->pl_type ;
			ew->ef_no = 0x3a01 ;
			ew->type = Rnd() & 7 ;
			ew->timer1 = i ;
			CHAR_PART_COPY( ew, pl_w ) ;
			ew->disp_flag = 1 ;
			ew->pl_no = pl_w->pl_no ;
			ew->pl_type = pl_w->pl_type ;
			ew->scale.x = pl_w->scale.x ;
			ew->scale.y = pl_w->scale.y ;
			ew->atck_level = pl_w->atck_level ;
			ew->work_no = pl_w->work_no ;
			ew->col_cnt = pl_w->col_cnt ;
			ew->scale = pl_w->scale ;
			ew->prio = pl_w->prio ;
			ew->prio = 8 ;
			shl3a_01_mode_set( ew, pl_w, 6 ) ;
			ew->spd.y = shl3a_01_spd_y_tbl2[ew->type] * 2.142857f / 256.0f ;
			ew->acc.y = shl3a_01_acc_y_tbl2[ew->type] * 2.142857f / 256.0f ;
			ew->pos.y = pl_w->ground_y + 2.142857f * shl3a_01_pos_y_tbl2[ew->type] ;
		}
	}
	return ew ;
}

/****************************************************************/
/*			The line of pieces that follows the master's		*/
/*			slide												*/
/****************************************************************/
PLAYER_WORK *shl3a_01_line_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ew = work_get( 0, W_SHL, LIST_END ) ){
		ew->work_func = shl3a_01_line_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x3a01 ;
		ew->timer1 = 39 ;
		ew->disp_flag = 0 ;
	}
	return ew ;
}

/****************************************************************/
/*			One piece of a type									*/
/****************************************************************/
PLAYER_WORK *shl3a_01_set( PLAYER_WORK *pl_w, int type )
{
	PLAYER_WORK *ew ;

	if( ew = work_get( 0, W_SHL, LIST_END ) ){
		ew->work_func = shl3a_01_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x3a01 ;
		ew->type = type ;
		CHAR_PART_COPY( ew, pl_w ) ;
		ew->disp_flag = 1 ;
		ew->pl_no = pl_w->pl_no ;
		ew->pl_type = pl_w->pl_type ;
		ew->scale.x = pl_w->scale.x ;
		ew->scale.y = pl_w->scale.y ;
		ew->atck_level = pl_w->atck_level ;
		ew->work_no = pl_w->work_no ;
		ew->col_cnt = pl_w->col_cnt ;
		ew->scale = pl_w->scale ;
		ew->prio = pl_w->prio ;
		ew->prio = 8 ;
		shl3a_01_mode_set( ew, pl_w, 1 ) ;
	}
	return ew ;
}

/****************************************************************/
/*			One piece started in a mode							*/
/****************************************************************/
PLAYER_WORK *shl3a_01_piece_set( PLAYER_WORK *pl_w, int no )
{
	PLAYER_WORK *ew ;

	if( ew = work_get( 0, W_SHL, LIST_END ) ){
		ew->work_func = shl3a_01_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x3a01 ;
		CHAR_PART_COPY( ew, pl_w ) ;
		ew->disp_flag = 1 ;
		ew->pl_no = pl_w->pl_no ;
		ew->pl_type = pl_w->pl_type ;
		ew->scale.x = pl_w->scale.x ;
		ew->scale.y = pl_w->scale.y ;
		ew->atck_level = pl_w->atck_level ;
		ew->work_no = pl_w->work_no ;
		ew->col_cnt = pl_w->col_cnt ;
		ew->scale = pl_w->scale ;
		ew->prio = pl_w->prio ;
		ew->prio = 8 ;
		shl3a_01_mode_set( ew, pl_w, no ) ;
	}
	return ew ;
}

static void shl3a_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void shl3a_01_main( PLAYER_WORK *ew )
{
	static void ( *shl3a_01_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl3a_01_init, shl3a_01_move, shl3a_01_end, shl3a_01_free,
	} ;

	shl3a_01_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

static void shl3a_01_body_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void shl3a_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	shl3a_01_body_set( ew, pl_w ) ;
	ew->be_flag = 1 ;
	pl_shadow_set( ew ) ;
	shl3a_01_move( ew, pl_w ) ;
}

static void shl3a_01_mode_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_01_mode_lunch( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_01_mode_burst_bound( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_01_mode_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_01_mode_dive( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_01_mode_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_01_mode_burst_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_01_mode_catch( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_01_mode_hit_bound( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_01_mode_hit_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_01_mode_run_out( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_01_mode_rebound( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_01_mode_hop( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_01_mode_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_01_mode_get_up( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_01_mode_land( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_01_mode_drop( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_01_mode_land_item( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_01_mode_carry( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_01_mode_watch( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_01_mode_turn_back( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*--------------------------------------------------------------*/
static void shl3a_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl3a_01_mode_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl3a_01_mode_run, shl3a_01_mode_lunch, shl3a_01_mode_burst_bound, shl3a_01_mode_fly,
		shl3a_01_mode_dive, shl3a_01_mode_fall, shl3a_01_mode_burst_fly, shl3a_01_mode_catch,
		shl3a_01_mode_hit_bound, shl3a_01_mode_hit_fall, shl3a_01_mode_run_out, shl3a_01_mode_run_out,
		shl3a_01_mode_run_out, shl3a_01_mode_rebound, shl3a_01_mode_hop, shl3a_01_mode_wait,
		shl3a_01_mode_get_up, shl3a_01_mode_land, shl3a_01_mode_drop, shl3a_01_mode_land_item,
		shl3a_01_mode_carry, shl3a_01_mode_watch, shl3a_01_mode_turn_back,
	} ;

	if( ew->pl_type != pl_w->pl_type ){
		shl3a_01_end( ew, pl_w ) ;
		return ;
	}
	shl3a_01_mode_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl3a_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	ew->routine_0 = 3 ;
}

/*--------------------------------------------------------------*/
static void shl3a_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/* the opponent's front, for the piece that holds him */
static void shl3a_01_em_pos( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
/* starts a piece in a mode: its place and speed, its attack, and the animation */
static void shl3a_01_mode_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w, Uint32 no )
{
	int i ;
	int at ;
	Sint8 *cnt ;
	Float x ;
	Float spd ;
	VEC3 ofs ;

	ew->routine_2 = ew->routine_3 = 0 ;
	switch( no ){
	case 0 :
		x = scr_work.scr_left - DOT_X( 16.0f ) ;
		spd = DOT_X( 8.0f ) ;
		if( !ew->disp_dir ){
			x = scr_work.scr_right + DOT_X( 16.0f ) ;
			spd = -DOT_X( 8.0f ) ;
		}
		ew->pos.x = x ;
		ew->pos.y = pl_w->ground_y ;
		ew->spd.x = spd ;
		ew->acc.x = 0.0f ;
		ew->timer0 = 20 ;
		ew->sub_type = Rnd() % 14 * 2 + 1 ;
		ew->atck_no = ( ew->sub_type >> 1 ) + 48 ;
		ew->hit_flag = ew->atck_attr = 0 ;
		ew->unk_1c4 = 0 ;
		vs_sys_w->atck_cnt[ew->pl_no] ++ ;
		pl_char_set( ew, Char_Effect, ew->sub_type + 2 ) ;
		break ;
	case 1 :
		x = -640.0f ;
		spd = DOT_X( 8.0f ) ;
		if( !ew->disp_dir ){
			x = 640.0f ;
			spd = -DOT_X( 8.0f ) ;
		}
		ew->pos.x = pl_w->pos.x + x ;
		ew->pos.y = pl_w->ground_y ;
		ew->spd.x = spd ;
		ew->acc.x = 0.0f ;
		ew->sub_no = 0 ;
		ew->timer0 = 600 ;
		if( !( ew->type & 3 ) ) ew->timer1 = 1 ;
		i = Rnd() % 15 ;
		for(;;){
			ew->sub_type = i ;
			cnt = ( Sint8 * )&pl_w->tlocal[TM_PIECE] ;
			if( cnt[i] > 0 ){
				cnt[i] -- ;
				break ;
			} else {
				i = ( ew->sub_type + 1 ) % 15 ;
			}
		}
		at = ew->sub_type + 64 ;
		if( !ew->type ) at = pl3a_at_lunch_first ;
		ew->atck_no = at ;
		ew->hit_flag = ew->atck_attr = 0 ;
		ew->unk_1c4 = 0 ;
		vs_sys_w->atck_cnt[ew->pl_no] ++ ;
		ew->atck_attr |= ATR_DOWN_LIFT ;
		ew->sub_type *= 2 ;
		pl_char_set( ew, Char_Effect, ew->sub_type + 2 ) ;
		break ;
	case 2 :
		x = scr_work.scr_left - DOT_X( 16.0f ) ;
		if( !ew->disp_dir ) x = scr_work.scr_right + DOT_X( 16.0f ) ;
		ew->pos.x = x ;
		ew->pos.y = pl_w->ground_y ;
		ew->acc.x = 0.0f ;
		ew->sub_type = Rnd() % 13 * 2 + 1 ;
		ew->atck_no = pl3a_at_lunch_burst ;
		ew->hit_flag = ew->atck_attr = 0 ;
		ew->unk_1c4 = 0 ;
		vs_sys_w->atck_cnt[ew->pl_no] ++ ;
		pl_char_set( ew, Char_Effect, ew->sub_type + 2 ) ;
		break ;
	case 3 :
		ew->timer0 = 20 ;
		ew->timer1 = 3 ;
		x = scr_work.scr_left - DOT_X( 64.0f ) ;
		spd = DOT_X( 7.0f ) ;
		if( !ew->disp_dir ){
			spd = -DOT_X( 7.0f ) ;
			x = scr_work.scr_right + DOT_X( 64.0f ) ;
		}
		ew->pos.x = x ;
		ew->pos.y = pl_w->ground_y + DOT_Y( 160.0f ) ;
		ew->spd.x = spd ;
		ew->acc.x = 0.0f ;
		ew->spd.y = -DOT_Y( 4.0f ) ;
		ew->acc.y = DOT_Y( 0.125f ) ;
		ew->atck_no = pl3a_at_engo_k_h ;
		ew->hit_flag = ew->atck_attr = 0 ;
		ew->unk_1c4 = 0 ;
		vs_sys_w->atck_cnt[ew->pl_no] ++ ;
		pl_char_set( ew, Char_Effect, 36 ) ;
		break ;
	case 4 :
		ew->timer0 = 20 ;
		ew->timer1 = 3 ;
		x = DOT_X( 16.0f ) ;
		spd = -5.0f ;
		if( ew->disp_dir ){
			x = -DOT_X( 16.0f ) ;
			spd = 5.0f ;
		}
		ew->pos.x = pl_w->pos.x + x ;
		ew->pos.y = pl_w->ground_y + 480.0f ;
		ew->spd.x = spd ;
		ew->acc.x = 0.0f ;
		ew->spd.y = -DOT_Y( 9.0f ) ;
		ew->acc.y = DOT_Y( 0.25f ) ;
		ew->atck_no = pl3a_at_engo_k_l ;
		ew->hit_flag = ew->atck_attr = 0 ;
		ew->unk_1c4 = 0 ;
		vs_sys_w->atck_cnt[ew->pl_no] ++ ;
		pl_char_set( ew, Char_Effect, 36 ) ;
		break ;
	case 5 :
		ew->spd.y = 0.0f ;
		ew->acc.y = -DOT_Y( 0.375f ) ;
		pl_char_set( ew, Char_Effect, 45 ) ;
		break ;
	case 6 :
		ew->timer0 = 20 ;
		x = scr_work.scr_left - DOT_X( 64.0f ) ;
		spd = 15.0f ;
		if( !ew->disp_dir ){
			x = scr_work.scr_right + DOT_X( 64.0f ) ;
			spd = -15.0f ;
		}
		ew->pos.x = x ;
		ew->pos.y = pl_w->ground_y ;
		ew->spd.x = spd ;
		ew->acc.x = 0.0f ;
		ew->atck_no = pl3a_at_lunch_burst_fly ;
		ew->hit_flag = ew->atck_attr = 0 ;
		ew->unk_1c4 = 0 ;
		vs_sys_w->atck_cnt[ew->pl_no] ++ ;
		pl_char_set( ew, Char_Effect, 36 ) ;
		break ;
	case 7 :
		{
			PLAYER_WORK *part_w ;

			ew->prio = 9 ;
			x = scr_work.scr_right + DOT_X( 32.0f ) ;
			spd = -DOT_X( 7.0f ) ;
			if( ew->disp_dir ){
				x = scr_work.scr_left - DOT_X( 32.0f ) ;
				spd = DOT_X( 7.0f ) ;
			}
			ew->pos.x = x ;
			ew->pos.y = pl_w->ground_y + DOT_Y( 32.0f ) ;
			ew->spd.x = spd ;
			ew->acc.x = ew->spd.y = ew->acc.y = 0.0f ;
			ew->atck_no = pl3a_at_engo_h ;
			ew->hit_flag = ew->atck_attr = 0 ;
			ew->unk_1c4 = 0 ;
			vs_sys_w->atck_cnt[ew->pl_no] ++ ;
			ew->atck_attr |= ATR_SHOT_HOLD ;
			pl_char_set( ew, Char_Effect, 37 ) ;
			if( part_w = shl3a_01_piece_set( pl_w, 20 ) ){
				SHL3A_01_LW( part_w )->part_w = ew ;
				part_w->pos.x = ew->pos.x ;
			}
		}
		break ;
	case 8 :
	case 9 :
		if( ew->routine_1 != 1 && ew->routine_1 != 2 && ew->routine_1 != 6 && ew->routine_1 != 18 ) pl_voice_req( ew, 39 ) ;
		ofs.x = ofs.z = 0.0f ;
		ofs.y += DOT_Y( 8.0f ) ;	/* ofs.y is never set */
		hit_mark_set( ew, &ofs, 0 ) ;
		if( ew->pos.y <= pl_w->ground_y ){
			ew->pos.y = pl_w->ground_y ;
			ew->spd.x = -ew->spd.x ;
			ew->acc.x = 0.0f ;
			ew->spd.y = ( Rnd() & 7 ) * 2.142857f ;
			ew->acc.y = -DOT_Y( 0.25f ) ;
			pl_char_set( ew, Char_Damage, Rnd() & 7 ) ;
		} else {
			ew->spd.x = -ew->spd.x / 4.0f ;
			ew->acc.x = 0.0f ;
			ew->spd.y = -DOT_Y( 4.0f ) ;
			ew->acc.y = -DOT_Y( 0.25f ) ;
			pl_char_set( ew, Char_Damage, 11 ) ;
		}
		break ;
	case 10 :
		ew->disp_dir ^= 1 ;
		spd = -10.0f ;
		x = DOT_X( 16.0f ) ;
		if( ew->disp_dir ){
			spd = 10.0f ;
			x = -DOT_X( 16.0f ) ;
		}
		ew->pos.x += x ;
		ew->spd.x = spd ;
		ew->acc.x = 0.0f ;
		pl_char_set( ew, Char_Effect, 44 ) ;
		break ;
	case 11 :
		ew->disp_dir ^= 1 ;
		spd = -10.0f ;
		if( ew->disp_dir ) spd = 10.0f ;
		ew->spd.x = spd ;
		ew->acc.x = 0.0f ;
		pl_char_set( ew, Char_Effect, 43 ) ;
		break ;
	case 12 :
		ew->disp_dir ^= 1 ;
		spd = -10.0f ;
		if( ew->disp_dir ) spd = 10.0f ;
		ew->spd.x = spd ;
		ew->acc.x = 0.0f ;
		pl_char_set( ew, Char_Effect, 42 ) ;
		shl3a_04_set( ew, 0x10000 ) ;
		break ;
	case 13 :
		ew->spd.x = -ew->spd.x / 2.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = -DOT_Y( 0.375f ) ;
		spd = -DOT_Y( 2.0f ) ;
		if( ew->pos.y <= pl_w->ground_y ) spd = DOT_Y( 4.0f ) ;
		ew->spd.y = spd ;
		pl_char_set( ew, Char_Effect, 41 ) ;
		break ;
	case 14 :
		ew->timer0 = 2 ;
		pl_char_set( ew, Char_Effect, 40 ) ;
		break ;
	case 15 :
		ew->timer0 = 30 ;
		pl_char_set( ew, Char_Effect, 39 ) ;
		break ;
	case 16 :
		ew->pos.y = pl_w->ground_y ;
		pl_char_set( ew, Char_Get_Up, 0 ) ;
		break ;
	case 17 :
		ew->routine_3 = 11 ;
		ew->acc.x = ew->spd.y = 0.0f ;
		ew->acc.y = -DOT_Y( 0.375f ) ;
		pl_char_set( ew, Char_Jump, 9 ) ;
		break ;
	case 18 :
		pl_w->timer1 -- ;
		ew->routine_3 = 10 ;
		ew->timer1 = 16 ;
		ew->spd.x = ew->acc.x = 0.0f ;
		ew->spd.y = -DOT_Y( 6.0f ) ;
		ew->acc.y = -DOT_Y( 0.375f ) ;
		ew->atck_no = pl3a_at_lunch_drop ;
		ew->hit_flag = ew->atck_attr = 0 ;
		ew->unk_1c4 = 0 ;
		vs_sys_w->atck_cnt[ew->pl_no] ++ ;
		pl_char_set( ew, Char_Effect, 49 ) ;
		break ;
	case 19 :
		ew->routine_3 = 12 ;
		ew->acc.x = ew->spd.y = 0.0f ;
		ew->acc.y = -DOT_Y( 0.375f ) ;
		pl_char_set( ew, Char_Effect, 50 ) ;
		shl3a_04_set( ew, 0 ) ;
		break ;
	case 20 :
		ew->pos.y = pl_w->ground_y ;
		spd = -DOT_X( 5.0f ) ;
		if( ew->disp_dir ) spd = DOT_X( 5.0f ) ;
		ew->spd.x = spd ;
		ew->acc.x = 0.0f ;
		ew->timer0 = 30 ;
		pl_char_set( ew, Char_Effect, 47 ) ;
		break ;
	case 21 :
		ew->timer0 = 120 ;
		pl_char_set( ew, Char_Effect, 48 ) ;
		break ;
	case 22 :
		ew->disp_dir ^= 1 ;
		ew->spd.x = -ew->spd.x ;
		pl_voice_req( ew, 39 ) ;
		ofs = ew->pos ;
		ofs.y += DOT_Y( 8.0f ) ;
		hit_mark_set( ew, &ofs, 0 ) ;
		break ;
	}
	ew->routine_1 = no ;
}

/*==============================================================*/
static void shl3a_01_mode_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->routine_1 != PL_NORM && !ew->hit_flag ){
		shl3a_01_mode_set( ew, pl_w, 15 ) ;
		return ;
	}
	if( !scr_in_x_ck( ew ) ){
		shl3a_01_end( ew, pl_w ) ;
		return ;
	}
	pl_w->tlocal[TM_SHOT] = 4 ;
	if( ew->hit_flag & ( HR_GUARD | HR_ARMOR ) ){
		shl3a_01_mode_set( ew, pl_w, 13 ) ;
		return ;
	}
	if( ew->hit_box_no ){
		int no = 46 ;

		if( ew->cg_flag1 > 44 ) no = ew->cg_flag1 + 1 ;
		pl_voice_req( ew, no ) ;
		shl3a_01_mode_set( ew, pl_w, 8 ) ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	pl_char_move( ew ) ;
	if( -- ew->timer0 == 0 ) pl_voice_req( ew, ew->cg_flag1 ) ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl3a_01_mode_lunch( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float d ;
	int v ;

	if( ew->timer0 > 0 && -- ew->timer0 <= 0 ) ew->sub_no = 1 ;
	if( !scr_in_x_ck( ew ) ){
		ew->disp_flag = 0 ;
		if( ew->sub_no ){
			shl3a_01_end( ew, pl_w ) ;
			return ;
		}
	} else {
		ew->disp_flag = 1 ;
		if( ew->timer1 ){
			ew->timer1 = 0 ;
			pl_voice_req( ew, Rnd() % 6 + 22 ) ;
		}
	}
	pl_w->tlocal[TM_SHOT] = 4 ;
	if( ew->hit_box_no ){
		if( ew->sub_type == 28 ) shl3a_01_mode_set( ew, pl_w, 22 ) ;
		else shl3a_01_mode_set( ew, pl_w, 8 ) ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	pl_char_move( ew ) ;
	d = pl_w->pos.x - ew->pos.x ;
	if( !ew->disp_dir ) d = -d ;
	if( d < 0.0f && ew->cg_type ){
		v = 20 ;
		if( !ew->type ) v = 2 ;
		ew->sub_no = 1 ;
		pl_w->timer0 = v ;
		pl_char_set( ew, Char_Effect, ew->char_no.b.no + 1 ) ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl3a_01_mode_burst_bound( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ew->timer1 ){
		ew->disp_flag = 0 ;
		if( -- ew->timer1 == 0 && ew->timer0 ) pl_voice_req( ew, Rnd() % 6 + 22 ) ;
		return ;
	}
	if( !scr_in_x_ck( ew ) ){
		ew->disp_flag = 0 ;
		shl3a_01_end( ew, pl_w ) ;
		return ;
	}
	ew->disp_flag = 1 ;
	pl_w->tlocal[TM_SHOT] = 4 ;
	if( ew->hit_box_no ){
		shl3a_01_mode_set( ew, pl_w, 9 ) ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y < pl_w->ground_y ) ew->spd.y = -ew->spd.y ;
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
}

/*==============================================================*/
/*			In flight: the piece ends past the stage top or		*/
/*			the screen, and changes mode when hit or when it	*/
/*			hits a guard or armor								*/
/*==============================================================*/
static void shl3a_01_mode_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->routine_1 != PL_NORM ){
		shl3a_01_mode_set( ew, pl_w, 5 ) ;
		return ;
	}
	if( !ground_ck( ew ) || !scr_in_x_ck( ew ) ){
		shl3a_01_end( ew, pl_w ) ;
		return ;
	}
	pl_w->tlocal[TM_SHOT] = 4 ;
	if( ew->hit_flag & ( HR_GUARD | HR_ARMOR ) ){
		shl3a_01_mode_set( ew, pl_w, 13 ) ;
		return ;
	}
	if( ew->hit_box_no ){
		pl_voice_req( ew, 52 ) ;
		shl3a_01_mode_set( ew, pl_w, 9 ) ;
		return ;
	}
	if( -- ew->timer0 == 0 ) pl_voice_req( ew, ew->cg_flag1 ) ;
	if( ew->hit_stop ) ew->hit_stop -- ;
	else {
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		pl_char_move( ew ) ;
		if( ew->hit_flag && -- ew->timer1 > 0 ){
			ew->atck_no = pl3a_at_engo_k_h ;
			HIT_RESET( ew ) ;
		}
	}
	hit_work_set( ew ) ;
}

/*==============================================================*/
/*			As shl3a_01_mode_fly; it falls once it would rise	*/
/*==============================================================*/
static void shl3a_01_mode_dive( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->routine_1 != PL_NORM ){
		shl3a_01_mode_set( ew, pl_w, 5 ) ;
		return ;
	}
	if( !ground_ck( ew ) || !scr_in_x_ck( ew ) ){
		shl3a_01_end( ew, pl_w ) ;
		return ;
	}
	pl_w->tlocal[TM_SHOT] = 4 ;
	if( ew->hit_flag & ( HR_GUARD | HR_ARMOR ) ){
		shl3a_01_mode_set( ew, pl_w, 13 ) ;
		return ;
	}
	if( ew->hit_box_no ){
		pl_voice_req( ew, 52 ) ;
		shl3a_01_mode_set( ew, pl_w, 9 ) ;
		return ;
	}
	if( -- ew->timer0 == 0 ) pl_voice_req( ew, ew->cg_flag1 ) ;
	if( ew->hit_stop ) ew->hit_stop -- ;
	else {
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		pl_char_move( ew ) ;
		if( ew->spd.y > 0.0f ) shl3a_01_mode_set( ew, pl_w, 5 ) ;
		if( ew->hit_flag && -- ew->timer1 > 0 ){
			ew->atck_no = pl3a_at_engo_k_h ;
			HIT_RESET( ew ) ;
		}
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl3a_01_mode_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->routine_2 ){
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		pl_char_move( ew ) ;
		if( ew->pos.y < pl_w->ground_y ){
			ew->routine_2 ++ ;
			ew->pos.y = pl_w->ground_y ;
			pl_char_set( ew, Char_Effect, 46 ) ;
		}
	} else if( pl_char_move( ew ) < 0 ) shl3a_01_mode_set( ew, pl_w, 11 ) ;
}

/*--------------------------------------------------------------*/
static void shl3a_01_mode_burst_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ew->timer1 ){
		ew->disp_flag = 0 ;
		ew->timer1 -- ;
		return ;
	}
	if( !ground_ck( ew ) || !scr_in_x_ck( ew ) ){
		shl3a_01_end( ew, pl_w ) ;
		return ;
	}
	ew->disp_flag = 1 ;
	pl_w->tlocal[TM_SHOT] = 4 ;
	if( ew->hit_box_no ){
		shl3a_01_mode_set( ew, pl_w, 9 ) ;
		return ;
	}
	if( ew->hit_stop ) ew->hit_stop -- ;
	else {
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		pl_char_move( ew ) ;
	}
	hit_work_set( ew ) ;
}

static void shl3a_01_mode_catch_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_01_mode_catch_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_01_mode_catch_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*--------------------------------------------------------------*/
static void shl3a_01_mode_catch( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl3a_01_mode_catch_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl3a_01_mode_catch_run, shl3a_01_mode_catch_start, shl3a_01_mode_catch_hold,
	} ;

	shl3a_01_mode_catch_jmp[ew->routine_2]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl3a_01_mode_catch_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->routine_1 ){
		ew->spd.x /= 2.0f ;
		shl3a_01_mode_set( ew, pl_w, 17 ) ;
		return ;
	}
	pl_w->tlocal[TM_SHOT] = 4 ;
	if( ew->hit_box_no ){
		shl3a_01_mode_set( ew, pl_w, 9 ) ;
		return ;
	}
	if( ew->hit_flag ){
		if( !pl_sp_catch_ck( ew ) ){
			shl3a_01_mode_set( ew, pl_w, 13 ) ;
			return ;
		}
		ew->routine_2 ++ ;
		ew->prio = 8 ;
		shl3a_01_em_pos( ew, pl_w ) ;
		pl_se_req( ew, 35 ) ;
		pl_char_set( ew, Char_Effect, 38 ) ;
	} else {
		pl_char_move( ew ) ;
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		if( !scr_in_x_ck( ew ) ){
			shl3a_01_end( ew, pl_w ) ;
			return ;
		}
		hit_work_set( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl3a_01_mode_catch_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w = ew->em_w ;

	pl_char_move( ew ) ;
	if( !em_w->hit_stop ){
		ew->routine_2 ++ ;
		pl_char_set( em_w, Char_Dizzy, 0 ) ;
		pl_ef_catch_set( ew, em_w ) ;
		SHL3A_01_LW( ew )->hold_w = pl_w->catch_w ;
		ew->timer0 = 120 ;
		ew->timer1 = 60 ;
		hold_renda_set( pl_w->catch_w, 1 ) ;
		em_w->catch_type = 6 ;
		em_w->catch_no = pl_w->catch_no = 0xc2 ;
		pl_w->catch_flag = 1 ;
		pl_w->catch_grip_old = -1 ;
		if( em_w->pl_stance == STANCE_AIR ){
			em_w->spd.x /= 4.0f ;
			em_w->acc.x /= 4.0f ;
			if( em_w->spd.y > 0.0f ) em_w->spd.y /= 4.0f ;
			em_w->acc.y = -DOT_Y( 0.375f ) ;
		}
	}
}

/*==============================================================*/
static void shl3a_01_mode_catch_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *hold_w = SHL3A_01_LW( ew )->hold_w ;
	Uint32 no ;
	Uint8 step ;

	pl_char_move( ew ) ;
	shl3a_01_em_pos( ew, pl_w ) ;
	if( ew != hold_w->catch_pair.w ){
		ew->spd.x = 0.0f ;
		shl3a_01_mode_set( ew, pl_w, 17 ) ;
		ew->routine_3 = 10 ;
		return ;
	}
	if( hold_w->hit_box_no && hold_w->routine_1 == PL_DAMAGE ){
		ew->spd.x = 0.0f ;
		shl3a_01_mode_set( ew, pl_w, 17 ) ;
		ew->routine_3 = 10 ;
		return ;
	}
	ew->timer1 -- ;
	if( hold_renda_move_ck( hold_w ) ) ew->timer0 -= 3 ;
	if( -- ew->timer0 < 0 ){
		hold_w->catch_type = 0 ;
		hold_w->auto_guard_timer = 8 ;
		if( hold_w->vital_new > 0 ){
			if( hold_w->pl_stance != STANCE_AIR ) pl_stand_ret( hold_w ) ;
			else pl_sjump_ret( hold_w ) ;
		}
		ew->spd.x = 0.0f ;
		step = 11 ;
		no = 17 ;
		if( ew->timer1 <= 0 ){
			step = 12 ;
			no = 19 ;
		}
		shl3a_01_mode_set( ew, pl_w, no ) ;
		ew->routine_3 = step ;
	}
}

/*==============================================================*/
/*			The opponent's front, where the piece holds him		*/
/*==============================================================*/
static void shl3a_01_em_pos( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w = ew->em_w ;
	Float x ;

	x = ( em_w->size_front >> 2 ) * 1.6666666f ;
	if( !em_w->disp_dir ) x = -x ;
	ew->pos.x = em_w->pos.x + x ;
	if( DOT_Y( 64.0f ) > em_w->size_top ) ew->pos.y = em_w->pos.y + ( em_w->size_top >> 2 ) * 2.142857f ;
}

/*--------------------------------------------------------------*/
static void shl3a_01_mode_hit_bound( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !scr_in_x_ck( ew ) ){
		shl3a_01_end( ew, pl_w ) ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y < pl_w->ground_y ){
		ew->pos.y = pl_w->ground_y ;
		ew->spd.y = -ew->spd.y / 4.0f ;
	}
}

/*--------------------------------------------------------------*/
static void shl3a_01_mode_hit_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !scr_in_x_ck( ew ) ){
		shl3a_01_end( ew, pl_w ) ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y < pl_w->ground_y ) shl3a_01_mode_set( ew, pl_w, 16 ) ;
}

/*--------------------------------------------------------------*/
static void shl3a_01_mode_run_out( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !scr_in_x_ck( ew ) ){
		shl3a_01_end( ew, pl_w ) ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl3a_01_mode_rebound( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->routine_2 ){
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		pl_char_move( ew ) ;
		if( ew->pos.y < pl_w->ground_y ) shl3a_01_mode_set( ew, pl_w, 16 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl3a_01_mode_hop( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y < pl_w->ground_y ){
		ew->pos.y = pl_w->ground_y ;
		ew->cg_prm = 0 ;
		if( -- ew->timer0 <= 0 ) shl3a_01_mode_set( ew, pl_w, 10 ) ;
	}
	if( !ew->cg_prm ) pl_char_move( ew ) ;
	if( ew->cg_type ){
		ew->cg_type = 0 ;
		ew->spd.y = DOT_Y( 4.0f ) ;
		ew->acc.y = -DOT_Y( 0.375f ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl3a_01_mode_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( -- ew->timer0 <= 0 ) shl3a_01_mode_set( ew, pl_w, 11 ) ;
}

/*--------------------------------------------------------------*/
static void shl3a_01_mode_get_up( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ) shl3a_01_mode_set( ew, pl_w, 11 ) ;
}

/*--------------------------------------------------------------*/
static void shl3a_01_mode_land( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->routine_2 ){
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		pl_char_move( ew ) ;
		if( ew->pos.y < pl_w->ground_y ){
			ew->routine_2 ++ ;
			ew->pos.y = pl_w->ground_y ;
			pl_char_set( ew, Char_Jump, 3 ) ;
		}
	} else if( pl_char_move( ew ) < 0 ) shl3a_01_mode_set( ew, pl_w, ew->routine_3 ) ;
}

/*==============================================================*/
static void shl3a_01_mode_drop( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *part_w ;
	Float x ;

	if( !ew->routine_2 ){
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		pl_char_move( ew ) ;
		if( -- ew->timer1 == 0 && pl_w->routine_1 == PL_NORM && scr_in_x_ck( ew ) && pl_w->timer1 > 0 ){
			if( part_w = shl3a_01_piece_set( pl_w, 18 ) ){
				part_w->disp_dir = ew->disp_dir ;
				x = -65.0f ;
				if( ew->disp_dir ) x = 65.0f ;
				part_w->pos.x = ew->pos.x + x ;
				part_w->pos.y = scr_work.scr_top + DOT_Y( 64.0f ) ;
			}
		}
		if( ew->pos.y < pl_w->ground_y ){
			ew->routine_2 ++ ;
			ew->pos.y = pl_w->ground_y ;
			if( scr_in_x_ck( ew ) ){ Sint8 lv = 1 ; scr_work.quake_level = lv ; scr_work.quake_flag = lv ; }
			pl_char_set( ew, Char_Jump, 3 ) ;
			pl_se_req( ew, 53 ) ;
		}
		if( ew->hit_box_no ){
			x = -DOT_X( 4.0f ) ;
			if( ew->disp_dir ) x = DOT_X( 4.0f ) ;
			ew->spd.x = x ;
			shl3a_01_mode_set( ew, pl_w, 9 ) ;
			return ;
		}
		if( ew->hit_flag & ( HR_GUARD | HR_ARMOR ) ){
			x = -DOT_X( 8.0f ) ;
			if( ew->disp_dir ) x = DOT_X( 8.0f ) ;
			ew->spd.x = x ;
			shl3a_01_mode_set( ew, pl_w, 13 ) ;
			return ;
		}
		hit_work_set( ew ) ;
	} else if( pl_char_move( ew ) < 0 ) shl3a_01_mode_set( ew, pl_w, ew->routine_3 ) ;
}

/*--------------------------------------------------------------*/
static void shl3a_01_mode_land_item( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->routine_2 ){
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		pl_char_move( ew ) ;
		if( ew->pos.y < pl_w->ground_y ){
			ew->routine_2 ++ ;
			ew->pos.y = pl_w->ground_y ;
			pl_char_set( ew, Char_Effect, 51 ) ;
			shl3a_04_set( ew, 0 ) ;
		}
	} else if( pl_char_move( ew ) < 0 ) shl3a_01_mode_set( ew, pl_w, ew->routine_3 ) ;
}

/*--------------------------------------------------------------*/
static void shl3a_01_mode_carry( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *part_w ;
	Float x ;
	Uint32 no ;

	if( pl_w->routine_1 != PL_NORM ){
		shl3a_01_mode_set( ew, pl_w, 15 ) ;
		return ;
	}
	if( !scr_in_x_ck( ew ) ){
		shl3a_01_end( ew, pl_w ) ;
		return ;
	}
	pl_w->tlocal[TM_SHOT] = 4 ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	pl_char_move( ew ) ;
	if( -- ew->timer0 >= 0 ){
		part_w = SHL3A_01_LW( ew )->part_w ;
		if( !part_w ) return ;
		if( part_w->routine_1 == 13 || part_w->routine_1 == 9 ){
			shl3a_01_mode_set( ew, pl_w, 15 ) ;
			return ;
		}
		if( part_w->hit_flag ){
			no = 14 ;
			if( ew->hit_flag & ( HR_GUARD | HR_ARMOR ) ) no = 15 ;
		} else {
			x = DOT_X( 4.0f ) ;
			if( ew->disp_dir ) x = -DOT_X( 4.0f ) ;
			part_w->pos.x = ew->pos.x + x ;
			part_w->pos.y = ew->pos.y + DOT_Y( 18.0f ) ;
			return ;
		}
	} else no = 21 ;
	shl3a_01_mode_set( ew, pl_w, no ) ;
}

/*--------------------------------------------------------------*/
static void shl3a_01_mode_watch( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *part_w ;
	Uint32 no ;

	if( pl_w->routine_1 != PL_NORM ){
		shl3a_01_mode_set( ew, pl_w, 15 ) ;
		return ;
	}
	if( !scr_in_x_ck( ew ) ){
		shl3a_01_end( ew, pl_w ) ;
		return ;
	}
	pl_char_move( ew ) ;
	if( -- ew->timer0 >= 0 ){
		part_w = SHL3A_01_LW( ew )->part_w ;
		if( !part_w || part_w->routine_1 == 9 ){
			shl3a_01_mode_set( ew, pl_w, 15 ) ;
			return ;
		}
		if( part_w->routine_1 == 13 ){
			shl3a_01_mode_set( ew, pl_w, 15 ) ;
			return ;
		}
		if( !part_w->hit_flag ) return ;
		no = 14 ;
		if( part_w->hit_flag & ( HR_GUARD | HR_ARMOR ) ) no = 15 ;
		shl3a_01_mode_set( ew, pl_w, no ) ;
	} else shl3a_01_mode_set( ew, pl_w, 15 ) ;
}

/*--------------------------------------------------------------*/
static void shl3a_01_mode_turn_back( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !scr_in_x_ck( ew ) ){
		shl3a_01_end( ew, pl_w ) ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	pl_char_move( ew ) ;
}

/*==============================================================*/
/*			The hit boxes of a piece that follows another		*/
/*==============================================================*/
static void shl3a_01_body_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->size_top = ew->size_bottom = ew->size_front = ew->size_back = 32 ;
	ew->col_no += 6 ;
}

/*==============================================================*/
/*			A piece is put out each frame while the master		*/
/*			slides; it follows the one before					*/
/*==============================================================*/
static void shl3a_01_line_main( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *part_w ;
	PLAYER_WORK *prev_w ;
	Float x ;

	if( !pl_w->routine_1 && pl_w->act_st == N_SUPER ){
		if( !( part_w = shl3a_01_set( pl_w, ew->timer1 ) ) ) return ;
		prev_w = ew->ef_w ;
		if( prev_w ){
			x = DOT_X( 32.0f ) ;
			if( part_w->disp_dir ) x = -DOT_X( 32.0f ) ;
			part_w->pos.x = prev_w->pos.x + x ;
		}
		ew->ef_w = part_w ;
		if( -- ew->timer1 >= 0 ) return ;
	}
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl3a_01.c" End of File	*/

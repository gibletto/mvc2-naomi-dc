/* Jill's effect 03 (character 11's shot 03): the hand that reaches out in Tyrant Escape, grabs the
   opponent, carries him through a run of holds and throws him. Type 0 is the reach, type 1 the
   grab that rides on it. */
#include "player.h"
#include "pl_char.h"
#include "pl11_def.h"

/* the effect's own work, from obj_flag */
typedef struct SHL11_03_LOCAL_W {
	Uint16	char_no;	/* 00: the animation Jill was in when it began */
} SHL11_03_LOCAL_W;

static PL11_LOCAL_W *shl11_03_lw ;	/* Jill's tlocal, set each frame by the main routine */
static SHL11_03_LOCAL_W *shl11_03_ew_w ;	/* the effect's own work */

static void shl11_03_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl11_03_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	PLAYER_WORK *bw ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl11_03_main ;
		ew->master_w = pl_w ;
		ew->type = 0 ;
		ew->ef_no = 0x1103 ;
	}
	if( ( bw = work_get( ew, W_SHL, LIST_BEHIND ) ) ){
		bw->work_func = shl11_03_main ;
		bw->master_w = pl_w ;
		bw->ef_w = ew ;
		bw->type = 1 ;
		bw->ef_no = 0x1103 ;
	}
	return ew ;
}

static void shl11_03_a_main( PLAYER_WORK *ew );
static void shl11_03_b_main( PLAYER_WORK *ew );

/*==============================================================*/
static void shl11_03_main( PLAYER_WORK *ew )
{
	static void ( *shl11_03_jmp[] )( PLAYER_WORK *ew ) = {
		shl11_03_a_main, shl11_03_b_main,
	} ;

	shl11_03_lw = ( PL11_LOCAL_W * )&ew->master_w->tlocal[0] ;
	shl11_03_ew_w = ( SHL11_03_LOCAL_W * )&ew->obj_flag ;
	shl11_03_jmp[ew->type]( ew ) ;
}

static void shl11_03_a_init( PLAYER_WORK *ew );
static void shl11_03_a_move( PLAYER_WORK *ew );
static void shl11_03_end( PLAYER_WORK *ew );
static void shl11_03_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl11_03_a_main( PLAYER_WORK *ew )
{
	static void ( *shl11_03_a_jmp[] )( PLAYER_WORK *ew ) = {
		shl11_03_a_init, shl11_03_a_move, shl11_03_end, shl11_03_free,
	} ;

	shl11_03_a_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl11_03_a_init( PLAYER_WORK *ew )
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
	ew->disp_flag = 0 ;
	shl11_03_ew_w->char_no = ew->master_w->char_no.w ;
}

static void shl11_03_a_fly( PLAYER_WORK *ew );
static void shl11_03_a_hit( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl11_03_a_move( PLAYER_WORK *ew )
{
	static void ( *shl11_03_a_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl11_03_a_fly, shl11_03_a_hit,
	} ;

	shl11_03_a_move_jmp[ew->routine_1]( ew ) ;
}

static Uint8 shl11_03_team_ck( PLAYER_WORK *ew );

/*==============================================================*/
/*			It waits for the animation frame that sends it		*/
/*			out													*/
/*==============================================================*/
static void shl11_03_a_fly( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( shl11_03_ew_w->char_no != pl_w->char_no.w ){
		shl11_03_end( ew ) ;
		return ;
	}
	if( shl11_03_team_ck( ew ) ){
		shl11_03_end( ew ) ;
		return ;
	}
	if( pl_w->cg_type ){
		ew->routine_1 ++ ;
		ew->disp_flag = 1 ;
		ew->prio = 12 ;
		ew->sub_prio = -( ew->pl_no * 4 ) - 1 ;
		ew->pos.x = pl_w->pos.x + ( ew->disp_dir ? DOT_X( 136.0f ) : -DOT_X( 136.0f ) ) ;
		ew->pos.y = pl_w->ground_y ;
		pls11_00_set2( ew, 18, 0 ) ;
		pls11_00_set2( ew, 18, 1 ) ;
		pl_char_set( ew, Char_Effect, 23 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl11_03_a_hit( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ) shl11_03_end( ew ) ;
}

static void shl11_03_b_init( PLAYER_WORK *ew );
static void shl11_03_b_move( PLAYER_WORK *ew );

/*==============================================================*/
static void shl11_03_b_main( PLAYER_WORK *ew )
{
	static void ( *shl11_03_b_jmp[] )( PLAYER_WORK *ew ) = {
		shl11_03_b_init, shl11_03_b_move, shl11_03_end, shl11_03_free,
	} ;

	shl11_03_b_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl11_03_b_init( PLAYER_WORK *ew )
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
	ew->disp_flag = 0 ;
	shl11_03_lw->tyrant_flag = 1 ;
	shl11_03_ew_w->char_no = ew->master_w->char_no.w ;
}

static void shl11_03_b_wait( PLAYER_WORK *ew );
static void shl11_03_b_reach( PLAYER_WORK *ew );
static void shl11_03_b_reach_end( PLAYER_WORK *ew );
static void shl11_03_b_catch( PLAYER_WORK *ew );
static void shl11_03_b_near_wait( PLAYER_WORK *ew );
static void shl11_03_b_near_hit( PLAYER_WORK *ew );
static void shl11_03_b_far_wait( PLAYER_WORK *ew );
static void shl11_03_b_far_hit( PLAYER_WORK *ew );
static void shl11_03_b_last_wait( PLAYER_WORK *ew );
static void shl11_03_b_last_hit( PLAYER_WORK *ew );
static void shl11_03_b_throw_wait( PLAYER_WORK *ew );
static void shl11_03_b_shot_wait( PLAYER_WORK *ew );
static void shl11_03_b_burst( PLAYER_WORK *ew );
static void shl11_03_b_fail_wait( PLAYER_WORK *ew );
static void shl11_03_b_fail_up( PLAYER_WORK *ew );
static void shl11_03_b_stop( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl11_03_b_move( PLAYER_WORK *ew )
{
	static void ( *shl11_03_b_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl11_03_b_wait, shl11_03_b_reach, shl11_03_b_reach_end, shl11_03_b_catch,
		shl11_03_b_near_wait, shl11_03_b_near_hit, shl11_03_b_far_wait, shl11_03_b_far_hit,
		shl11_03_b_last_wait, shl11_03_b_last_hit, shl11_03_b_throw_wait, shl11_03_b_shot_wait,
		shl11_03_b_burst, shl11_03_b_fail_wait, shl11_03_b_fail_up, shl11_03_b_stop,
	} ;

	shl11_03_b_move_jmp[ew->routine_1]( ew ) ;
}

static void shl11_03_out( PLAYER_WORK *ew );
static void shl11_03_held( PLAYER_WORK *ew );

/*==============================================================*/
/*			It waits for the animation frame that sends it		*/
/*			out													*/
/*==============================================================*/
static void shl11_03_b_wait( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( shl11_03_ew_w->char_no != pl_w->char_no.w ){
		shl11_03_out( ew ) ;
		return ;
	}
	if( shl11_03_team_ck( ew ) ){
		shl11_03_out( ew ) ;
		return ;
	}
	if( pl_w->cg_type ){
		ew->routine_1 ++ ;
		ew->disp_flag = 1 ;
		ew->prio = 12 ;
		ew->sub_prio = -( ew->pl_no * 4 ) + 1 ;
		ew->size_top = 32 ;
		ew->size_bottom = 192 ;
		ew->atck_flag = HIT_ON | HIT_SHOT ;
		ew->dm_flag = HIT_ON | HIT_SHOT ;
		ew->atck_no = pl11_at_tyrant ;
		HIT_RESET( ew ) ;
		ew->pos.x = pl_w->pos.x + ( ew->disp_dir ? DOT_X( 136.0f ) : -DOT_X( 136.0f ) ) ;
		ew->pos.y = pl_w->ground_y ;
		ew->timer0 = 0 ;
		pl_char_set( ew, Char_Effect, 24 ) ;
	}
}

/*==============================================================*/
/*			The reach: a hit that can be caught takes the		*/
/*			opponent											*/
/*==============================================================*/
static void shl11_03_b_reach( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *em ;

	if( pl_char_move( ew ) < 0 ){
		shl11_03_lw->tyrant_reach_st = -1 ;
		shl11_03_out( ew ) ;
		return ;
	}
	if( shl11_03_team_ck( ew ) ){
		ew->routine_1 ++ ;
		return ;
	}
	if( ew->hit_flag && !ew->timer0 ){
		ew->timer0 ++ ;
		if( pl_sp_catch_ck( ew ) && pl_w->routine_1 == PL_NORM && shl11_03_ew_w->char_no == pl_w->char_no.w ){
			ew->routine_1 += 2 ;
			pl_w->dm_flag = 0 ;
			pl_w->catch_ng_timer = 2 ;
			pl_w->push_ng_timer = 2 ;
			em = ew->em_w ;
			shl11_03_lw->tyrant_reach_st = 1 ;
			pl_w->catch_no = 0xc4 ;
			pl_ef_catch_set( ew, em ) ;
			pl_char_set( ew, Char_Effect, 25 ) ;
			ew->routine_2 = 0 ;
			ew->sub_type = 0 ;
			ew->dir = ew->disp_dir ^ 1 ;
			em->pos.x = ew->pos.x ;
			pl_char_set( em, Char_Damage, 5 ) ;
			return ;
		}
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl11_03_b_reach_end( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ) shl11_03_out( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl11_03_b_catch( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	shl11_03_held( ew ) ;
	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		pl_shadow_set( ew ) ;
		ew->be_flag = 1 ;
	}
}

/*--------------------------------------------------------------*/
static void shl11_03_b_near_wait( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *em = ew->catch_w ;

	pl_w->catch_flag = 1 ;
	shl11_03_held( ew ) ;
	if( shl11_03_team_ck( ew ) ){
		ew->routine_1 = 15 ;
		return ;
	}
	if( em->spd.y <= 0.0f && em->pos.y <= em->ground_y + DOT_Y( 128.0f ) ){
		ew->routine_1 ++ ;
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		ew->spd.x = em->pos.x + ( ew->disp_dir ? -DOT_X( 112.0f ) : DOT_X( 112.0f ) ) ;
		ew->spd.x = ( ew->spd.x - ew->pos.x ) / 16.0f ;
		ew->timer0 = 4 ;
		ew->timer1 = 3 ;
		pl_char_set( ew, Char_Effect, 26 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl11_03_b_near_hit( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *em = ew->catch_w ;
	VEC3 ofs ;

	pl_w->catch_flag = 1 ;
	if( shl11_03_team_ck( ew ) ){
		ew->routine_1 = 15 ;
		return ;
	}
	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		return ;
	}
	if( ew->cg_flag1 ){
		ew->cg_flag1 = 0 ;
		pls11_00_set2( ew, 12, 0 ) ;
	}
	shl11_03_held( ew ) ;
	if( !ew->cg_type ) return ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !-- ew->timer1 && ew->timer0 ){
		ew->routine_2 = 0 ;
		ew->timer0 -- ;
		ew->timer1 = 3 ;
		ew->sub_type = 1 ;
		catch_pos_set( ew, em ) ;
		em->atck_no = pl11_at_tyrant_hit ;
		catch_damage( ew ) ;
		ofs.x = -DOT_X( 128.0f ) ;
		ofs.y = DOT_Y( 88.0f ) ;
		ofs.z = 0.0f ;
		catch_hit_mark_set( ew, &ofs ) ;
		pl_se_req( ew, 6 ) ;
		combo_add( em, pl_w, 1 ) ;
		pl_char_set( em, Char_Damage, 5 ) ;
		shl11_03_held( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl11_03_b_far_wait( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *em = ew->catch_w ;

	pl_w->catch_flag = 1 ;
	if( shl11_03_team_ck( ew ) ){
		ew->routine_1 = 15 ;
		return ;
	}
	shl11_03_held( ew ) ;
	if( em->spd.y <= 0.0f && em->pos.y <= em->ground_y + DOT_Y( 128.0f ) ){
		ew->routine_1 ++ ;
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		ew->spd.x = em->pos.x + ( ew->disp_dir ? DOT_X( 112.0f ) : -DOT_X( 112.0f ) ) ;
		ew->spd.x = ( ew->spd.x - ew->pos.x ) / 16.0f ;
		ew->timer0 = 4 ;
		ew->timer1 = 3 ;
		shl11_03_lw->tyrant_hold_st = 1 ;
		pl_char_set( ew, Char_Effect, 27 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl11_03_b_far_hit( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *em = ew->catch_w ;
	VEC3 ofs ;

	pl_w->catch_flag = 1 ;
	if( shl11_03_team_ck( ew ) ){
		ew->routine_1 = 15 ;
		return ;
	}
	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		return ;
	}
	if( ew->cg_flag1 ){
		ew->cg_flag1 = 0 ;
		pls11_00_set2( ew, 12, 1 ) ;
	}
	shl11_03_held( ew ) ;
	if( !ew->cg_type ) return ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !-- ew->timer1 && ew->timer0 ){
		ew->routine_2 = 0 ;
		ew->timer0 -- ;
		ew->timer1 = 2 ;
		ew->sub_type = 2 ;
		catch_pos_set( ew, em ) ;
		em->atck_no = pl11_at_tyrant_hit ;
		catch_damage( ew ) ;
		ofs.x = 185.0f ;
		ofs.y = DOT_Y( 102.0f ) ;
		ofs.z = 0.0f ;
		catch_hit_mark_set( ew, &ofs ) ;
		pl_se_req( ew, 6 ) ;
		combo_add( em, pl_w, 1 ) ;
		pl_char_set( em, Char_Damage, 5 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl11_03_b_last_wait( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *em = ew->catch_w ;

	pl_w->catch_flag = 1 ;
	if( shl11_03_team_ck( ew ) ){
		ew->routine_1 = 15 ;
		return ;
	}
	shl11_03_held( ew ) ;
	if( em->spd.y <= 0.0f && em->pos.y <= em->ground_y + DOT_Y( 128.0f ) ){
		ew->routine_1 ++ ;
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		ew->spd.x = em->pos.x + ( ew->disp_dir ? -DOT_X( 112.0f ) : DOT_X( 112.0f ) ) ;
		ew->spd.x = ( ew->spd.x - ew->pos.x ) / 16.0f ;
		ew->timer0 = 3 ;
		ew->timer1 = 3 ;
		pl_char_set( ew, Char_Effect, 28 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl11_03_b_last_hit( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *em = ew->catch_w ;
	VEC3 ofs ;

	pl_w->catch_flag = 1 ;
	if( shl11_03_team_ck( ew ) ){
		ew->routine_1 = 15 ;
		return ;
	}
	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		pl_char_set( ew, Char_Effect, 29 ) ;
		shl11_03_lw->tyrant_hold_st = 2 ;
		ew->timer0 = 45 ;
		return ;
	}
	if( ew->cg_flag1 ){
		ew->cg_flag1 = 0 ;
		pls11_00_set2( ew, 12, 0 ) ;
	}
	shl11_03_held( ew ) ;
	if( !ew->cg_type ) return ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !-- ew->timer1 && ew->timer0 >= 0 ){
		if( !ew->timer0 ){
			ew->timer0 -- ;
			em->dm_hit_w = pl_w ;
			em->catch_type = 15 ;
			em->atck_no = pl11_at_tyrant_hit_last ;
			ofs.x = -DOT_X( 128.0f ) ;
			ofs.y = DOT_Y( 88.0f ) ;
			ofs.z = 0.0f ;
			catch_hit_mark_set( ew, &ofs ) ;
			pl_se_req( ew, 6 ) ;
			combo_add( em, pl_w, 1 ) ;
			em->pos.x = ew->pos.x ;
			ew->routine_2 = 0 ;
			ew->sub_type = 3 ;
			shl11_03_held( ew ) ;
		} else {
			ew->routine_2 = 0 ;
			ew->timer0 -- ;
			ew->timer1 = 3 ;
			ew->sub_type = 3 ;
			catch_pos_set( ew, em ) ;
			em->atck_no = pl11_at_tyrant_hit ;
			catch_damage( ew ) ;
			ofs.x = -DOT_X( 128.0f ) ;
			ofs.y = DOT_Y( 88.0f ) ;
			ofs.z = 0.0f ;
			catch_hit_mark_set( ew, &ofs ) ;
			pl_se_req( ew, 6 ) ;
			combo_add( em, pl_w, 1 ) ;
			pl_char_set( em, Char_Damage, 5 ) ;
		}
	}
}

static void shl11_03_fail( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl11_03_b_throw_wait( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	if( shl11_03_team_ck( ew ) ){
		ew->routine_1 = 15 ;
		return ;
	}
	if( shl11_03_lw->tyrant_throw_flag ){
		ew->routine_1 ++ ;
		ew->timer0 = 120 ;
		return ;
	}
	if( !-- ew->timer0 ) shl11_03_fail( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl11_03_b_shot_wait( PLAYER_WORK *ew )
{
	if( ew->hit_box_no ){
		ew->routine_1 ++ ;
		ew->disp_flag = 1 ;
		ew->timer0 = 60 ;
		ew->timer1 = 1 ;
		ew->atck_no = pl11_at_tyrant_finish ;
		HIT_RESET( ew ) ;
		pl_char_set( ew, Char_Effect, 31 ) ;
		return ;
	}
	hit_work_set( ew ) ;
	if( !-- ew->timer0 ) shl11_03_out( ew ) ;
}

/*==============================================================*/
/*			The hand breaks up into sparks						*/
/*==============================================================*/
static void shl11_03_b_burst( PLAYER_WORK *ew )
{
	VEC3 pos ;
	int i ;
	int x ;
	int y ;

	if( !-- ew->timer0 ){
		for( i = 0 ; i < 5 ; i ++ ){
			pos = ew->pos ;
			if( ( x = Rnd() & 0xff ) & 0x80 ){
				x &= 63 ;
				x = -x ;
			} else x &= 63 ;
			pos.x += DOT_X( x ) ;
			do {
				y = Rnd() & 127 ;
			} while( y < 16 ) ;
			pos.y += DOT_Y( y ) ;
			burst_set( ew, &pos, 1, 0x178 ) ;
		}
		shl11_03_out( ew ) ;
		return ;
	}
	ew->disp_flag = ( Sys->game_timer0 & 4 ) ? 0 : 1 ;
	if( !-- ew->timer1 ){
		ew->timer1 = Rnd() & 15 ;
		if( ew->timer1 < 8 ) ew->timer1 = 8 ;
		pos = ew->pos ;
		if( ( x = Rnd() & 0xff ) & 0x80 ){
			x &= 63 ;
			x = -x ;
		} else x &= 63 ;
		pos.x += DOT_X( x ) ;
		y = Rnd() & 127 ;
		pos.y += DOT_Y( y ) ;
		burst_set( ew, &pos, 1, 0xf9 ) ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl11_03_b_fail_wait( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	if( !ew->cg_type ){
		ew->routine_1 ++ ;
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		ew->spd.y = DOT_Y( 12.0f ) ;
		ew->acc.y = -0.033482142f ;
	}
}

/*--------------------------------------------------------------*/
static void shl11_03_b_fail_up( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !scr_in_y_ck( ew ) ) shl11_03_out( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl11_03_b_stop( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ) shl11_03_fail( ew ) ;
}

/*==============================================================*/
static Uint8 shl11_03_team_ck( PLAYER_WORK *ew )
{
	ew = ew->master_w ;
	if( ew->team_no ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static void shl11_03_fail( PLAYER_WORK *ew )
{
	ew->routine_1 = 13 ;
	shl11_03_lw->tyrant_hold_st = -1 ;
	pl_char_set( ew, Char_Effect, 30 ) ;
}

static void shl11_03_held_init( PLAYER_WORK *ew, PLAYER_WORK *em );
static void shl11_03_held_wait( PLAYER_WORK *ew, PLAYER_WORK *em );
static void shl11_03_held_up( PLAYER_WORK *ew, PLAYER_WORK *em );
static void shl11_03_held_fall( PLAYER_WORK *ew, PLAYER_WORK *em );

/*==============================================================*/
/*			What the opponent does in the hand, by routine_2	*/
/*==============================================================*/
static void shl11_03_held( PLAYER_WORK *ew )
{
	static void ( *shl11_03_held_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *em ) = {
		shl11_03_held_init, shl11_03_held_wait, shl11_03_held_up, shl11_03_held_fall,
	} ;

	shl11_03_held_jmp[ew->routine_2]( ew, ew->catch_w ) ;
}

/* his speed on being let go, by sub_type: x, y and the pull on y */
static Float shl11_03_held_tbl[][3] = {
	{ -DOT_X( 2.5f ), DOT_Y( 12.0f ), -DOT_Y( 0.375f ) },
	{ DOT_X( 8.0f ), DOT_Y( 12.0f ), -1.2053571f },
	{ DOT_X( 8.0f ), DOT_Y( 12.0f ), -1.2053571f },
	{ 5.0f, DOT_Y( 12.0f ), -DOT_Y( 0.125f ) },
} ;

/*==============================================================*/
static void shl11_03_held_init( PLAYER_WORK *ew, PLAYER_WORK *em )
{
	ew->routine_2 ++ ;
	em->spd.x = em->disp_dir ? shl11_03_held_tbl[ew->sub_type][0] : -shl11_03_held_tbl[ew->sub_type][0] ;
	em->acc.x = 0.0f ;
	em->spd.y = shl11_03_held_tbl[ew->sub_type][1] ;
	em->acc.y = shl11_03_held_tbl[ew->sub_type][2] ;
	ew->routine_3 = 3 ;
}

/*--------------------------------------------------------------*/
static void shl11_03_held_wait( PLAYER_WORK *ew, PLAYER_WORK *em )
{
	if( !-- ew->routine_3 ){
		ew->routine_2 ++ ;
		pl_char_set( em, Char_Damage, 16 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl11_03_held_up( PLAYER_WORK *ew, PLAYER_WORK *em )
{
	pl_char_move( em ) ;
	em->pos.x += em->spd.x ;
	em->spd.x += em->acc.x ;
	em->pos.y += em->spd.y ;
	em->spd.y += em->acc.y ;
	if( em->spd.y < 0.0f ){
		ew->routine_2 ++ ;
		pl_char_set( em, Char_Damage, 21 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl11_03_held_fall( PLAYER_WORK *ew, PLAYER_WORK *em )
{
	pl_char_move( em ) ;
	em->pos.x += em->spd.x ;
	em->spd.x += em->acc.x ;
	em->pos.y += em->spd.y ;
	em->spd.y += em->acc.y ;
	if( em->pos.y <= em->ground_y ) em->pos.y = em->ground_y ;
}

/*==============================================================*/
static void shl11_03_out( PLAYER_WORK *ew )
{
	shl11_03_lw->tyrant_flag = 0 ;
	shl11_03_end( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl11_03_end( PLAYER_WORK *ew )
{
	ew->routine_0 = 3 ;
	ew->disp_flag = 0 ;
	shl11_03_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl11_03_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl11_03.c" End of File	*/

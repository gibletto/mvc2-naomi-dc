/* Tron Bonne's effect 04 (character 10's shot 04): a claw that catches the opponent and holds him
   in a ring of four parts, while two Servbots jump in and drop on him. */
#include "player.h"
#include "ef_char.h"
#include "pl_char.h"
#include "pl10_def.h"

/* the effect's own work */
typedef struct SHL10_04_LOCAL_W {
	Sint32	bounce;		/* 000: a Servbot who rode the claw long enough bounces on landing */
	struct PLAYER_WORK *hold_w;	/* 004: the player the claw holds */
} SHL10_04_LOCAL_W;
#define SHL10_04_LW( w )	( ( SHL10_04_LOCAL_W * )&( w )->obj_flag )

/* the ring's four parts: their order among works, their direction and their animation */
typedef struct SHL10_04_RING {
	Sint8	sub_prio;
	Uint8	dir;
	Uint8	unk_2;
	Uint8	char_no;
} SHL10_04_RING;

static SHL10_04_RING shl10_04_ring_tbl[] = {
	{ -1, 28, 0, 24 }, { -1, 4, 0, 25 }, { -1, 20, 0, 26 }, { -1, 12, 0, 27 },
} ;

static void shl10_04_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl10_04_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl10_04_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
	}
	return ew ;
}

/*==============================================================*/
/*			A part of the effect hw: a Servbot (kind 2) or a	*/
/*			part of the ring									*/
/*==============================================================*/
static void *shl10_04_part_set( PLAYER_WORK *pl_w, PLAYER_WORK *hw, Uint8 kind, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl10_04_main ;
		ew->master_w = pl_w ;
		ew->ef_w = hw ;
		ew->type = kind ;
		ew->sub_no = no ;
	}
	return ew ;
}

static void shl10_04_init( PLAYER_WORK *ew );
static void shl10_04_move( PLAYER_WORK *ew );
static void shl10_04_end( PLAYER_WORK *ew );
static void shl10_04_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl10_04_main( PLAYER_WORK *ew )
{
	static void ( *shl10_04_jmp[] )( PLAYER_WORK *ew ) = {
		shl10_04_init, shl10_04_move, shl10_04_end, shl10_04_free,
	} ;

	shl10_04_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl10_04_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->routine_0 ++ ;
	ew->ef_no = 0x1004 ;
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
	shl10_04_move( ew ) ;
}

static void shl10_04_catch_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl10_04_ring_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl10_04_bot_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl10_04_move( PLAYER_WORK *ew )
{
	static void ( *shl10_04_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl10_04_catch_main, shl10_04_ring_main, shl10_04_bot_main,
	} ;

	shl10_04_move_jmp[ew->type]( ew, ew->master_w ) ;
}

static void shl10_04_catch_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl10_04_catch_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl10_04_catch_hit( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl10_04_catch_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The claw											*/
/*==============================================================*/
static void shl10_04_catch_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl10_04_catch_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl10_04_catch_init, shl10_04_catch_fly, shl10_04_catch_hit, shl10_04_catch_hold,
	} ;

	shl10_04_catch_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl10_04_catch_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	/* the claw's speed and its start from Tron, by strength */
	static struct { Float x, y ; } shl10_04_spd_tbl[] = {
		{ -DOT_X( 8.0f ), 0.0f }, { -10.0f, DOT_Y( 6.0f ) },
	} ;
	static struct { Sint16 x, y ; } shl10_04_ofs_tbl[] = {
		{ -72, 80 }, { -40, 96 },
	} ;
	Sint16 ofs ;
	Float spd ;

	ew->routine_1 ++ ;
	ew->disp_flag = 0 ;
	ew->prio = 8 ;
	ew->sub_prio = 0 ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->atck_no = pl10_at_beacon ;
	HIT_RESET( ew ) ;
	ew->atck_attr |= ATR_SHOT_HOLD ;
	ew->size_front = ew->size_back = 32 ;
	ofs = shl10_04_ofs_tbl[ew->atck_level].x ;
	if( ew->disp_dir ) ofs = -ofs ;
	ew->pos.x = pl_w->pos.x + 1.6666666f * ofs ;
	ew->pos.y = pl_w->pos.y + 2.142857f * shl10_04_ofs_tbl[ew->atck_level].y ;
	spd = shl10_04_spd_tbl[ew->atck_level].x ;
	if( ew->disp_dir ) spd = -spd ;
	ew->spd.x = spd ;
	ew->spd.y = shl10_04_spd_tbl[ew->atck_level].y ;
	ew->acc.x = ew->acc.y = 0.0f ;
	pl_char_set( ew, Char_Effect, 81 ) ;
	shl10_04_catch_fly( ew, pl_w ) ;
}

static void shl10_04_catch_pos( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The claw flies until it hits or leaves the			*/
/*			screen; a hit on a catchable opponent takes hold	*/
/*==============================================================*/
static void shl10_04_catch_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->routine_1 != PL_NORM ) goto end ;
	if( ew->hit_box_no ) goto end ;
	if( ew->hit_flag ){
		if( pl_sp_catch_ck( ew ) ){
			ew->routine_1 ++ ;
			ew->routine_2 = 0 ;
			shl10_04_catch_pos( ew, pl_w ) ;
			return ;
		}
	} else {
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		if( !scr_in_x_ck( ew ) ) goto end ;
		hit_work_set( ew ) ;
		return ;
	}
end :
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

static void shl10_04_hold_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Waits out the opponent's hit stop, then takes		*/
/*			hold of him											*/
/*==============================================================*/
static void shl10_04_catch_hit( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = ew->em_w ;

	shl10_04_hold_main( ew, pl_w ) ;
	if( !em->hit_stop ){
		ew->routine_1 ++ ;
		pl_char_set( em, Char_Dizzy, 0 ) ;
		pl_ef_catch_set( ew, em ) ;
		SHL10_04_LW( ew )->hold_w = pl_w->catch_w ;
		ew->timer0 = 180 ;
		hold_renda_set( pl_w->catch_w, 1 ) ;
		em->catch_type = 6 ;
		em->catch_no = pl_w->catch_no = 0xc3 ;
		pl_w->catch_flag = 1 ;
		pl_w->catch_grip_old = -1 ;
		if( em->pl_stance == STANCE_AIR ) em->spd.x = em->acc.x = em->spd.y = em->acc.y = 0.0f ;
	}
}

/*--------------------------------------------------------------*/
static void shl10_04_catch_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = SHL10_04_LW( ew )->hold_w ;

	if( ew->catch_pair.w->catch_pair.w == ew ){
		switch( em->routine_1 ){
		case 2:
			if( em->hit_box_no && em->routine_1 == PL_DAMAGE ) break ;
			shl10_04_catch_pos( ew, pl_w ) ;
			shl10_04_hold_main( ew, pl_w ) ;
			if( hold_renda_move_ck( em ) ) ew->timer0 -= 3 ;
			if( -- ew->timer0 < 0 ){
				em->catch_type = 0 ;
				em->auto_guard_timer = 8 ;
				if( !em->vital_new ) em->catch_type = 7 ;
				else if( em->pl_stance != STANCE_AIR ) pl_stand_ret( em ) ;
				else pl_sjump_ret( em ) ;
				break ;
			}
			goto hold ;
		}
	}
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	if( PL10_LW( pl_w )->claw_hop_cnt ){
		if( 0 ) PL10_LW( pl_w )->claw_hop_cnt -- ;
		PL10_LW( pl_w )->claw_hop_cnt -- ;
	}
	return ;
hold:
	PL10_LW( pl_w )->claw_hop_cnt = 1 ;
}

static void shl10_04_ring_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl10_04_ring_grow( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl10_04_hold_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl10_04_hold_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl10_04_hold_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl10_04_ring_set, shl10_04_ring_grow, shl10_04_hold_wait,
	} ;

	shl10_04_hold_jmp[ew->routine_2]( ew, pl_w ) ;
}

/*==============================================================*/
/*			The ring's four parts, which ride on the claw		*/
/*==============================================================*/
static void shl10_04_ring_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *rw ;
	int i ;

	ew->routine_2 ++ ;
	ew->disp_flag = 1 ;
	ew->timer1 = 10 ;
	ew->scale.x = 0.1f ;
	ew->scale.y = 0.1f ;
	for( i = 0 ; i < 4 ; i ++ ){
		if( ( rw = work_get( ew, W_SHL, LIST_BEHIND ) ) ){
			rw->work_func = shl10_04_main ;
			rw->master_w = pl_w ;
			rw->ef_w = ew ;
			rw->type = 1 ;
			rw->sub_no = i ;
		}
	}
	pl_voice_req( pl_w, 46 ) ;
	shl10_04_ring_grow( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl10_04_ring_grow( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->scale.x += 0.1f ;
	ew->scale.y += 0.1f ;
	if( !-- ew->timer1 ){
		ew->routine_2 ++ ;
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
		shl10_04_part_set( pl_w, ew, 2, 0 ) ;
		shl10_04_part_set( pl_w, ew, 2, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl10_04_hold_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
}

static void shl10_04_ring_init( PLAYER_WORK *ew );
static void shl10_04_ring_shrink( PLAYER_WORK *ew );
static void shl10_04_ring_out( PLAYER_WORK *ew );
static void shl10_04_ring_in( PLAYER_WORK *ew );

/*==============================================================*/
/*			A part of the ring, which swings in and out			*/
/*			about the claw										*/
/*==============================================================*/
static void shl10_04_ring_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl10_04_ring_jmp[] )( PLAYER_WORK *ew ) = {
		shl10_04_ring_init, shl10_04_ring_shrink, shl10_04_ring_out, shl10_04_ring_in,
	} ;

	PLAYER_WORK *bw = ew->before_w ;

	if( bw->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	shl10_04_ring_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void shl10_04_ring_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *hw = ew->ef_w ;
	SHL10_04_RING *dt ;

	ew->routine_1 ++ ;
	ew->timer0 = 10 ;
	ew->timer1 = 0xc80 ;
	ew->pos = hw->pos ;
	ew->scale.x = 1.5f ;
	ew->scale.y = 1.5f ;
	dt = &shl10_04_ring_tbl[ew->sub_no] ;
	ew->prio = 8 ;
	ew->sub_prio = dt->sub_prio ;
	ew->dir = dt->dir ;
	if( ew->disp_dir ) ew->dir = ( 32 - ew->dir ) & 31 ;
	ef_char_set( ew, 27, dt->char_no ) ;
}

/*--------------------------------------------------------------*/
static void shl10_04_ring_shrink( PLAYER_WORK *ew )
{
	PLAYER_WORK *hw = ew->ef_w ;

	ew->scale.x -= 0.1f ;
	ew->scale.y -= 0.1f ;
	ew->timer1 -= 200 ;
	ew->pos = hw->pos ;
	dir_move( ew, ew->timer1 ) ;
	if( !-- ew->timer0 ){
		ew->routine_1 ++ ;
		ew->scale.x = 0.5f ;
		ew->scale.y = 0.5f ;
		ew->timer0 = 6 ;
	}
}

/*--------------------------------------------------------------*/
static void shl10_04_ring_out( PLAYER_WORK *ew )
{
	PLAYER_WORK *hw = ew->ef_w ;

	ew->timer1 += 200 ;
	ew->pos = hw->pos ;
	dir_move( ew, ew->timer1 ) ;
	if( !-- ew->timer0 ){
		ew->routine_1 ++ ;
		ew->timer0 = 6 ;
	}
}

/*--------------------------------------------------------------*/
static void shl10_04_ring_in( PLAYER_WORK *ew )
{
	PLAYER_WORK *hw = ew->ef_w ;

	ew->timer1 -= 200 ;
	ew->pos = hw->pos ;
	dir_move( ew, ew->timer1 ) ;
	if( !-- ew->timer0 ){
		ew->routine_1 = 2 ;
		ew->timer0 = 6 ;
	}
}

static void shl10_04_bot_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl10_04_bot_jump( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl10_04_bot_ride( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl10_04_bot_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl10_04_bot_land( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl10_04_bot_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl10_04_bot_out( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A Servbot, who jumps in from the top of the			*/
/*			screen onto the claw								*/
/*==============================================================*/
static void shl10_04_bot_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl10_04_bot_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl10_04_bot_init, shl10_04_bot_jump, shl10_04_bot_ride, shl10_04_bot_fall,
		shl10_04_bot_land, shl10_04_bot_run, shl10_04_bot_out,
	} ;

	shl10_04_bot_jmp[ew->routine_1]( ew, pl_w ) ;
}

static int shl10_04_jump_cnt( PLAYER_WORK *ew, Float y );

/*==============================================================*/
static void shl10_04_bot_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *hw ;
	VEC2 tgt ;
	int cnt ;

	ew->routine_1 ++ ;
	ew->size_top = 16 ;
	ew->size_bottom = 16 ;
	ew->size_front = 24 ;
	ew->size_back = 24 ;
	ew->disp_dir = 0 ;
	ew->prio = 8 ;
	ew->sub_prio = -2 ;
	if( !ew->sub_no ) ew->pos.x = scr_work.scr_right + DOT_X( 32.0f ) ;
	else ew->pos.x = scr_work.scr_left + -DOT_X( 32.0f ) ;
	ew->pos.y = scr_work.scr_top + -DOT_Y( 64.0f ) ;
	hw = ew->ef_w ;
	if( !ew->sub_no ) tgt.x = hw->pos.x - ew->pos.x + DOT_X( 16.0f ) ;
	else tgt.x = hw->pos.x - ew->pos.x + -DOT_X( 16.0f ) ;
	tgt.y = hw->pos.y + -DOT_Y( 16.0f ) ;
	ew->spd.y = DOT_Y( 2.0f ) ;
	ew->acc.y = -DOT_Y( 0.375f ) ;
	cnt = shl10_04_jump_cnt( ew, tgt.y ) ;
	ew->spd.x = tgt.x / cnt ;
	ew->acc.x = 0.0f ;
	ew->timer0 = cnt ;
	pl_char_set( ew, Char_Effect, ew->sub_no + 71 ) ;
	ew->be_flag = 1 ;
	pl_shadow_set( ew ) ;
	if( ew->sub_no == 0 ) pl_voice_req( ew, 27 ) ;
	shl10_04_bot_jump( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl10_04_bot_jump( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *hw = ew->ef_w ;
	VEC2 d ;

	if( hw->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		ew->be_flag = 0 ;
		return ;
	}
	if( !ew->sub_no ) d.x = hw->pos.x - ew->pos.x + DOT_X( 16.0f ) ;
	else d.x = hw->pos.x - ew->pos.x + -DOT_X( 16.0f ) ;
	d.y = hw->pos.y - ew->pos.y + -DOT_Y( 16.0f ) ;
	ew->spd.x = d.x / ew->timer0 ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !-- ew->timer0 ){
		ew->routine_1 ++ ;
		ew->timer0 = 0 ;
		SHL10_04_LW( ew )->bounce = 0 ;
		pl_se_req( ew, 35 ) ;
		pl_char_set( ew, Char_Effect, ew->sub_no + 73 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl10_04_bot_ride( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *hw = ew->ef_w ;

	ew->timer0 ++ ;
	pl_char_move( ew ) ;
	if( !ew->sub_no ) ew->pos.x = hw->pos.x + DOT_X( 16.0f ) ;
	else ew->pos.x = hw->pos.x + -DOT_X( 16.0f ) ;
	ew->pos.y = hw->pos.y + -DOT_Y( 16.0f ) ;
	if( hw->routine_0 >= 2 ){
		ew->routine_1 ++ ;
		ew->disp_dir = ew->sub_no ;
		if( ew->timer0 >= 90 ) SHL10_04_LW( ew )->bounce = 1 ;
		ew->spd.x = ew->acc.x = ew->spd.y = 0.0f ;
		ew->acc.y = -DOT_Y( 0.375f ) ;
		pl_char_set( ew, Char_Effect, SHL10_04_LW( ew )->bounce + 75 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl10_04_bot_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float y ;

	pl_char_move( ew ) ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( SHL10_04_LW( ew )->bounce ) y = pl_w->ground_y + DOT_Y( 12.0f ) ;
	else y = pl_w->ground_y ;
	if( ew->pos.y <= y ){
		ew->routine_1 ++ ;
		ew->pos.y = y ;
		pl_char_set( ew, Char_Effect, SHL10_04_LW( ew )->bounce + 77 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl10_04_bot_land( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		ew->pos.y = pl_w->ground_y ;
		if( SHL10_04_LW( ew )->bounce && !ew->sub_no ) pl_voice_req( ew, 26 ) ;
		if( ew->pos.x >= pl_w->pos.x ){
			ew->disp_dir = 0 ;
			ew->spd.x = -DOT_X( 4.0f ) ;
		} else {
			ew->disp_dir = 1 ;
			ew->spd.x = DOT_X( 4.0f ) ;
		}
		ew->acc.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.y = 0.0f ;
		pl_char_set( ew, Char_Effect, SHL10_04_LW( ew )->bounce + 79 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl10_04_bot_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	if( !scr_in_x_ck( ew ) ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		ew->be_flag = 0 ;
		return ;
	}
	if( SHL10_04_LW( ew )->bounce ){
		if( ( ew->disp_dir == 0 && ew->pos.x <= pl_w->pos.x ) || ( ew->disp_dir != 0 && ew->pos.x >= pl_w->pos.x ) ){
			ew->routine_1 ++ ;
			pl_se_req( ew, 42 ) ;
			pl_char_set( ew, Char_Effect, 13 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void shl10_04_bot_out( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	if( !scr_in_x_ck( ew ) ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		ew->be_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl10_04_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl10_04_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			The frames of a jump before it falls back to y		*/
/*==============================================================*/
static int shl10_04_jump_cnt( PLAYER_WORK *ew, Float y )
{
	Float py = ew->pos.y ;
	Float spd = ew->spd.y ;
	int cnt = 0 ;

	do {
		py += spd ;
		spd += ew->acc.y ;
		cnt ++ ;
	} while( spd > 0.0f || py > y ) ;
	return cnt ;
}

/*==============================================================*/
/*			Holds the claw on the opponent						*/
/*==============================================================*/
static void shl10_04_catch_pos( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = ew->em_w ;

	ew->pos.x = em->pos.x ;
	ew->pos.y = em->pos.y + 2.142857f * ( em->size_top / 2 ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl10_04.c" End of File	*/

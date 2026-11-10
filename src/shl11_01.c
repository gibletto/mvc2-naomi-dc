/* Jill's effect 01 (character 11's shot 01): the zombie of Zombie Escape. Type 0 flies in,
   takes hold of whoever it touches and carries it off; type 1 follows it; type 2 and type 3 are the
   heavy version and its pieces. */
#include "player.h"
#include "ef_char.h"
#include "pl_char.h"
#include "pl11_def.h"

/* the effect's own work, from obj_flag */
typedef struct SHL11_01_LOCAL_W {
	Float	land_y;		/* 00: the height it keeps to */
	Uint8	unk_04;
	Uint8	hit_cnt;
	Uint8	end_flag;
	Uint8	fall_dir;
	Float	shake_x;		/* 08: where it was in x */
	Sint16	voice_timer;
	Uint8	stop_flag;
	Uint8	hit_off_flag;
} SHL11_01_LOCAL_W;

static PL11_LOCAL_W *shl11_01_lw ;	/* Jill's tlocal, set each frame by the main routine */
static SHL11_01_LOCAL_W *shl11_01_ew_w ;	/* the effect's own work */

static void shl11_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl11_01_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl11_01_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
		ew->ef_no = 0x1101 ;
	}
	return ew ;
}

/*==============================================================*/
static PLAYER_WORK *shl11_01_child_set( PLAYER_WORK *ew )
{
	PLAYER_WORK *cw ;

	if( ( cw = work_get( ew, W_SHL, LIST_BEHIND ) ) ){
		cw->work_func = shl11_01_main ;
		cw->master_w = ew->master_w ;
		cw->ef_w = ew ;
		ew->ef_w = cw ;
		cw->type = 1 ;
		cw->ef_no = 0x1101 ;
	}
	return cw ;
}

/*==============================================================*/
static PLAYER_WORK *shl11_01_part_set( PLAYER_WORK *ew, Uint8 no )
{
	PLAYER_WORK *cw ;

	if( ( cw = work_get( ew, W_SHL, LIST_BEHIND ) ) ){
		cw->work_func = shl11_01_main ;
		cw->master_w = ew->master_w ;
		cw->ef_w = ew ;
		cw->type = 3 ;
		cw->sub_type = no ;
		cw->ef_no = 0x1101 ;
	}
	return cw ;
}

static void shl11_01_a_main( PLAYER_WORK *ew );
static void shl11_01_b_main( PLAYER_WORK *ew );
static void shl11_01_c_main( PLAYER_WORK *ew );
static void shl11_01_d_main( PLAYER_WORK *ew );

/*==============================================================*/
static void shl11_01_main( PLAYER_WORK *ew )
{
	static void ( *shl11_01_jmp[] )( PLAYER_WORK *ew ) = {
		shl11_01_a_main, shl11_01_b_main, shl11_01_c_main, shl11_01_d_main,
	} ;

	shl11_01_lw = ( PL11_LOCAL_W * )&ew->master_w->tlocal[0] ;
	shl11_01_ew_w = ( SHL11_01_LOCAL_W * )&ew->obj_flag ;
	shl11_01_jmp[ew->type]( ew ) ;
}

static void shl11_01_a_init( PLAYER_WORK *ew );
static void shl11_01_a_move( PLAYER_WORK *ew );
static void shl11_01_end( PLAYER_WORK *ew );
static void shl11_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl11_01_a_main( PLAYER_WORK *ew )
{
	static void ( *shl11_01_a_jmp[] )( PLAYER_WORK *ew ) = {
		shl11_01_a_init, shl11_01_a_move, shl11_01_end, shl11_01_free,
	} ;

	shl11_01_a_jmp[ew->routine_0]( ew ) ;
}

static void shl11_01_leave( PLAYER_WORK *ew );

/*==============================================================*/
static void shl11_01_a_init( PLAYER_WORK *ew )
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
	ew->prio = 12 ;
	shl11_01_lw->zombie_shot_flag = 1 ;
	ew->atck_no = pl11_at_zombie ;
	HIT_RESET( ew ) ;
	ew->atck_flag = HIT_ON | HIT_PL_ONLY ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->atck_attr |= ATR_SHOT_HOLD ;
	ew->size_front = 64 ;
	ew->size_back = 64 ;
	ew->pos.x = ew->master_w->pl_dir ? scr_work.eye[0].x + -DOT_X( 256.0f ) : scr_work.eye[0].x + DOT_X( 256.0f ) ;
	ew->pos.y = shl11_01_ew_w->land_y = ew->master_w->ground_y + DOT_Y( 8.0f ) ;
	ew->timer0 = 600 ;
	shl11_01_ew_w->unk_04 = 0 ;
	shl11_01_ew_w->hit_cnt = 1 ;
	shl11_01_ew_w->end_flag = 0 ;
	shl11_01_ew_w->voice_timer = 180 ;
	shl11_01_ew_w->hit_off_flag = 0 ;
	shl11_01_child_set( ew ) ;
	pl_voice_req( ew, 48 ) ;
	shl11_01_leave( ew ) ;
}

static void shl11_01_a_seek( PLAYER_WORK *ew );
static void shl11_01_a_held( PLAYER_WORK *ew );
static void shl11_01_a_held_end( PLAYER_WORK *ew );
static void shl11_01_a_shake( PLAYER_WORK *ew );
static void shl11_01_a_fall( PLAYER_WORK *ew );
static void shl11_01_a_down( PLAYER_WORK *ew );
static void shl11_01_a_down_held( PLAYER_WORK *ew );
static void shl11_01_a_fade( PLAYER_WORK *ew );
static void shl11_01_a_down_fade( PLAYER_WORK *ew );
static void shl11_01_a_out( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl11_01_a_move( PLAYER_WORK *ew )
{
	static void ( *shl11_01_a_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl11_01_a_seek, shl11_01_a_held, shl11_01_a_held_end, shl11_01_a_shake, shl11_01_a_fall,
		shl11_01_a_down, shl11_01_a_down_held, shl11_01_a_fade, shl11_01_a_down_fade, shl11_01_a_out,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->team_no || ( !pl_w->routine_1 && pl_w->act_st == N_SUPER ) ) shl11_01_ew_w->stop_flag = 1 ;
	else shl11_01_ew_w->stop_flag = 0 ;
	if( pl_w->routine_1 == PL_CAUGHT ) shl11_01_ew_w->hit_off_flag = 1 ;
	ew->timer0 -- ;
	shl11_01_a_move_jmp[ew->routine_1]( ew ) ;
}

static void shl11_01_go( PLAYER_WORK *ew );
static void shl11_01_back( PLAYER_WORK *ew );
static void shl11_01_hold( PLAYER_WORK *ew, PLAYER_WORK *em );

/*==============================================================*/
static void shl11_01_a_seek( PLAYER_WORK *ew )
{
	PLAYER_WORK *em ;
	PLAYER_WORK *hitter ;

	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	if( shl11_01_ew_w->stop_flag ){
		shl11_01_ew_w->end_flag = 1 ;
		shl11_01_ew_w->fall_dir = ew->disp_dir ^ 1 ;
		pl_voice_req( ew, 48 ) ;
		shl11_01_go( ew ) ;
		return ;
	}
	if( ew->timer0 < 0 ){
		shl11_01_ew_w->end_flag = 1 ;
		pl_voice_req( ew, 48 ) ;
		shl11_01_go( ew ) ;
		return ;
	}
	if( shl11_01_ew_w->hit_off_flag ){
		ew->atck_flag = 0 ;
		ew->dm_flag = 0 ;
		ew->ef_w->atck_flag = 0 ;
		ew->ef_w->dm_flag = 0 ;
	}
	if( ew->hit_flag ){
		em = ew->em_w ;
		if( !pl_sp_catch_ck( ew ) ){
			shl11_01_ew_w->end_flag = 1 ;
			shl11_01_go( ew ) ;
			return ;
		}
		ew->routine_1 = 1 ;
		ew->prio = 0 ;
		ew->master_w->catch_no = 0xc3 ;
		pl_ef_catch_set( ew, em ) ;
		em->catch_type = 6 ;
		ew->timer1 = 128 ;
		hold_renda_set( em, 30 ) ;
		pl_char_set( ew, 15, 6 ) ;
		em->spd.x = 0.0f ;
		em->spd.y = 0.0f ;
		em->acc.x = 0.0f ;
		em->acc.y = 0.0f ;
		if( em->pl_stance == STANCE_AIR ) em->pos.y = em->ground_y ;
		if( PL_CK( ew->catch_w, PL_SERVBOT ) ) ew->catch_w->pos.y = ew->master_w->ground_y + DOT_Y( 60.0f ) ;
		else if( PL_CK( ew->catch_w, PL_ROLL ) ) ew->catch_w->pos.y = ew->master_w->ground_y + DOT_Y( 32.0f ) ;
		catch_em_pos_set1( ew, em ) ;
		shl11_01_hold( ew, em ) ;
		return ;
	}
	if( ew->hit_box_no || ew->ef_w->hit_box_no ){
		if( ew->hit_box_no ) hitter = ew->dm_hit_w ;
		else hitter = ew->ef_w->dm_hit_w ;
		catch_hitmark_no_se( ew ) ;
		pl_voice_req( ew, 44 ) ;
		shl11_01_ew_w->fall_dir = hitter->disp_dir ;
		if( !( hitter->pl_no == ew->pl_no && hitter->pl_type == PL_JILL ) ){
			if( -- shl11_01_ew_w->hit_cnt <= 0 || ( hitter->work_kind == W_SHL && hitter->ef_no == 0x1101 ) ){
				shl11_01_ew_w->end_flag = 1 ;
				pl_voice_req( ew, 48 ) ;
			}
			else {
				shl11_01_back( ew ) ;
				return ;
			}
		}
		ew->routine_3 = 2 ;
		shl11_01_go( ew ) ;
		return ;
	}
	if( !scr_in_x_ck( ew ) ){
		shl11_01_lw->zombie_shot_flag = 0 ;
		shl11_01_end( ew ) ;
		return ;
	}
	if( !-- shl11_01_ew_w->voice_timer ){
		shl11_01_ew_w->voice_timer = 180 ;
		pl_voice_req( ew, 49 ) ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl11_01_a_held( PLAYER_WORK *ew )
{
	PLAYER_WORK *em ;

	em = ew->catch_pair.w ;
	em->nml_catch_ng_timer = 1 ;
	if( ew->catch_pair.w->catch_pair.w != ew ){
		shl11_01_end( ew ) ;
		return ;
	}
	if( shl11_01_ew_w->stop_flag ) ew->timer1 = 1 ;
	if( em->routine_1 == PL_DAMAGE ) ew->timer1 = 1 ;
	if( -- ew->timer1 ){
		pl_char_move( ew ) ;
		catch_em_pos_set1( ew, em ) ;
		shl11_01_hold( ew, em ) ;
		if( !hold_renda_ck( em ) ) return ;
	}
	ew->prio = 12 ;
	em->catch_type = 0 ;
	if( em->routine_1 != PL_DAMAGE ){
		em->auto_guard_timer = 8 ;
		if( em->pl_stance == STANCE_AIR ) pl_jump_ck( em ) ;
		else pl_stand_ret( em ) ;
	}
	shl11_01_ew_w->fall_dir = ew->disp_dir ^ 1 ;
	shl11_01_ew_w->end_flag = 1 ;
	shl11_01_go( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl11_01_a_held_end( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 = 9 ;
		shl11_01_leave( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl11_01_a_shake( PLAYER_WORK *ew )
{
	switch( ew->timer1 & 3 ){
	case 0:
		ew->pos.x = shl11_01_ew_w->shake_x ;
		break ;
	case 1:
		ew->pos.x = shl11_01_ew_w->shake_x + -DOT_X( 2.0f ) ;
		break ;
	case 2:
		ew->pos.x = shl11_01_ew_w->shake_x ;
		break ;
	case 3:
		ew->pos.x = shl11_01_ew_w->shake_x + DOT_X( 2.0f ) ;
		break ;
	}
	if( -- ew->timer1 ) return ;
	ew->pos.x = shl11_01_ew_w->shake_x ;
	if( shl11_01_ew_w->end_flag ) ew->routine_1 = 9 ;
	else {
		ew->routine_1 = 0 ;
		shl11_01_ew_w->unk_04 = 1 ;
		ew->atck_no = pl11_at_zombie ;
		HIT_RESET( ew ) ;
		hit_work_set( ew ) ;
	}
	shl11_01_leave( ew ) ;
}

static void shl11_01_a_fall_move( PLAYER_WORK *ew );
static void shl11_01_a_fall_bound( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl11_01_a_fall( PLAYER_WORK *ew )
{
	static void ( *shl11_01_a_fall_jmp[] )( PLAYER_WORK *ew ) = {
		shl11_01_a_fall_move, shl11_01_a_fall_bound,
	} ;

	shl11_01_a_fall_jmp[ew->routine_2]( ew ) ;
}

/*==============================================================*/
static void shl11_01_a_fall_move( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y <= shl11_01_ew_w->land_y ){
		ew->routine_2 ++ ;
		ew->pos.y = shl11_01_ew_w->land_y ;
		ew->timer1 = 3 ;
		if( shl11_01_ew_w->fall_dir ) ew->spd.x = DOT_X( 2.0f ) ;
		else ew->spd.x = -DOT_X( 2.0f ) ;
		ew->acc.x = 0.0f ;
		ew->spd.y = DOT_Y( 2.0f ) ;
		ew->acc.y = -DOT_Y( 0.375f ) ;
		ew->char_no.b.no = ( ew->disp_dir ^ shl11_01_ew_w->fall_dir ) ? 13 : 15 ;
		pl_char_set( ew, Char_Effect, ew->char_no.b.no ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl11_01_a_fall_bound( PLAYER_WORK *ew )
{
	if( ew->timer1 && -- ew->timer1 ) return ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y <= shl11_01_ew_w->land_y ){
		if( shl11_01_ew_w->end_flag ){
			ew->routine_1 = 8 ;
			ew->alpha = 1.0f ;
		}
		else {
			ew->routine_1 = 5 ;
			ew->timer1 = 120 ;
			ew->atck_no = pl11_at_zombie_fall ;
			HIT_RESET( ew ) ;
			ew->char_no.b.no = ( ew->disp_dir ^ shl11_01_ew_w->fall_dir ) ? 16 : 17 ;
			pl_char_set( ew, Char_Effect, ew->char_no.b.no ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void shl11_01_a_down( PLAYER_WORK *ew )
{
	PLAYER_WORK *em ;

	pl_char_move( ew ) ;
	if( shl11_01_ew_w->stop_flag ){
		ew->routine_1 = 8 ;
		ew->alpha = 1.0f ;
		return ;
	}
	if( shl11_01_ew_w->hit_off_flag ){
		ew->atck_flag = 0 ;
		ew->dm_flag = 0 ;
		ew->ef_w->atck_flag = 0 ;
		ew->ef_w->dm_flag = 0 ;
	}
	if( !-- ew->timer1 ){
		ew->routine_1 = 8 ;
		ew->alpha = 1.0f ;
		return ;
	}
	if( ew->hit_flag ){
		em = ew->em_w ;
		if( !pl_sp_catch_ck( ew ) ) ew->routine_1 = 8 ;
		else {
			ew->routine_1 = 6 ;
			ew->master_w->catch_no = 0xc3 ;
			pl_ef_catch_set( ew, em ) ;
			ew->prio = 0 ;
			em->catch_type = 6 ;
			ew->timer1 = 128 ;
			hold_renda_set( em, 30 ) ;
			pl_char_set( ew, 15, 7 ) ;
			em->spd.x = 0.0f ;
			em->spd.y = 0.0f ;
			em->acc.x = 0.0f ;
			em->acc.y = 0.0f ;
			if( em->pl_stance == STANCE_AIR ) em->pos.y = em->ground_y ;
			catch_em_pos_set1( ew, em ) ;
			shl11_01_hold( ew, em ) ;
		}
		return ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl11_01_a_down_held( PLAYER_WORK *ew )
{
	PLAYER_WORK *em ;

	em = ew->catch_pair.w ;
	em->nml_catch_ng_timer = 1 ;
	if( ew->catch_pair.w->catch_pair.w != ew ){
		shl11_01_end( ew ) ;
		return ;
	}
	if( shl11_01_ew_w->stop_flag ) ew->timer1 = 1 ;
	if( em->routine_1 == PL_DAMAGE ) ew->timer1 = 1 ;
	if( -- ew->timer1 ){
		pl_char_move( ew ) ;
		catch_em_pos_set1( ew, em ) ;
		shl11_01_hold( ew, em ) ;
		if( !hold_renda_ck( em ) ) return ;
	}
	ew->routine_1 = 7 ;
	ew->prio = 12 ;
	em->catch_type = 0 ;
	if( em->routine_1 != PL_DAMAGE ){
		em->auto_guard_timer = 8 ;
		if( em->pl_stance == STANCE_AIR ) pl_jump_ret( em ) ;
		else pl_stand_ret( em ) ;
	}
	ew->alpha = 1.0f ;
	pl_char_set( ew, Char_Effect, 19 ) ;
}

/*--------------------------------------------------------------*/
static void shl11_01_a_fade( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	if( ( ew->alpha -= 0.05f ) <= 0.0f ) shl11_01_end( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl11_01_a_down_fade( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	if( ( ew->alpha -= 0.05f ) <= 0.0f ) shl11_01_end( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl11_01_a_out( PLAYER_WORK *ew )
{
	if( shl11_01_ew_w->stop_flag ){
		shl11_01_ew_w->end_flag = 1 ;
		shl11_01_ew_w->fall_dir = ew->disp_dir ^ 1 ;
		shl11_01_go( ew ) ;
		return ;
	}
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	if( !scr_in_x_ck( ew ) ){
		ew->routine_0 ++ ;
		shl11_01_end( ew ) ;
	}
}

/*==============================================================*/
static void shl11_01_leave( PLAYER_WORK *ew )
{
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	if( ew->disp_dir ) ew->spd.x = 2.5f ;
	else ew->spd.x = -2.5f ;
	pl_char_set( ew, Char_Effect, 9 ) ;
}

/*==============================================================*/
static void shl11_01_back( PLAYER_WORK *ew )
{
	ew->routine_1 = 3 ;
	ew->timer1 = 32 ;
	shl11_01_ew_w->shake_x = ew->pos.x ;
	ew->char_no.b.no = ( ew->disp_dir ^ shl11_01_ew_w->fall_dir ) ? 10 : 11 ;
	pl_char_set( ew, Char_Effect, ew->char_no.b.no ) ;
}

/*==============================================================*/
static void shl11_01_go( PLAYER_WORK *ew )
{
	ew->routine_1 = 4 ;
	ew->routine_2 = 0 ;
	if( shl11_01_ew_w->fall_dir ) ew->spd.x = 5.0f ;
	else ew->spd.x = -5.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = DOT_Y( 4.0f ) ;
	ew->acc.y = -DOT_Y( 0.375f ) ;
	ew->pos.y = shl11_01_ew_w->land_y ;
	ew->char_no.b.no = ( ew->disp_dir ^ shl11_01_ew_w->fall_dir ) ? 12 : 14 ;
	pl_char_set( ew, Char_Effect, ew->char_no.b.no ) ;
}

/*==============================================================*/
/*			The victim's pose and place follow the animation	*/
/*			frame's hold position								*/
/*==============================================================*/
static void shl11_01_hold( PLAYER_WORK *ew, PLAYER_WORK *em )
{
	if( PL_CK( ew->catch_w, PL_BB_HOOD ) && LO_BYTE( ew->cg_rival ) == 7 ){
		pl_char_set( em, Char_Caught, 11 ) ;
		ew->pos.x += ew->disp_dir ? -DOT_X( 20.0f ) : DOT_X( 20.0f ) ;
		ew->pos.y += DOT_Y( 16.0f ) ;
	}
	else if( PL_CK( ew->catch_w, PL_ROLL ) && LO_BYTE( ew->cg_rival ) == 7 ){
		em->rot[2] = 0 ;
		pl_char_set( em, Char_Caught, 18 ) ;
		ew->pos.x += ew->disp_dir ? 30.0f : -30.0f ;
		ew->pos.y += DOT_Y( 26.0f ) ;
	}
	else if( PL_CK( ew->catch_w, PL_MAGNETO ) && LO_BYTE( ew->cg_rival ) == 8 ){
		pl_char_set( em, Char_Caught, 2 ) ;
		ew->pos.y += -DOT_Y( 12.0f ) ;
	}
	else if( PL_CK( ew->catch_w, PL_MAGNETO ) && LO_BYTE( ew->cg_rival ) == 9 ){
		pl_char_set( em, Char_Caught, 13 ) ;
		ew->pos.x += ew->disp_dir ? -30.0f : 30.0f ;
		ew->pos.y += -DOT_Y( 23.0f ) ;
	}
}

static void shl11_01_b_init( PLAYER_WORK *ew );
static void shl11_01_b_move( PLAYER_WORK *ew );

/*==============================================================*/
static void shl11_01_b_main( PLAYER_WORK *ew )
{
	static void ( *shl11_01_b_jmp[] )( PLAYER_WORK *ew ) = {
		shl11_01_b_init, shl11_01_b_move, shl11_01_end, shl11_01_free,
	} ;

	shl11_01_b_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl11_01_b_init( PLAYER_WORK *ew )
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
	ew->hit_box_no = 0 ;
	ew->pl_no ^= 1 ;
	ew->atck_flag = 0 ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	shl11_01_b_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl11_01_b_move( PLAYER_WORK *ew )
{
	if( ew->ef_w->routine_0 > 1 ){
		shl11_01_end( ew ) ;
		return ;
	}
	ew->pos = ew->ef_w->pos ;
	CHAR_ANIM_COPY( ew, ew->ef_w ) ;
	ew->frame_box = ew->ef_w->frame_box ;
	if( ew->ef_w->routine_0 == 1 && ew->ef_w->routine_1 == 0 ) hit_work_set( ew ) ;
}

static void shl11_01_c_init( PLAYER_WORK *ew );
static void shl11_01_c_move( PLAYER_WORK *ew );

/*==============================================================*/
static void shl11_01_c_main( PLAYER_WORK *ew )
{
	static void ( *shl11_01_c_jmp[] )( PLAYER_WORK *ew ) = {
		shl11_01_c_init, shl11_01_c_move, shl11_01_end, shl11_01_free,
	} ;

	shl11_01_c_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl11_01_c_init( PLAYER_WORK *ew )
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
	ew->prio = 12 ;
	if( 1 ) ew->sub_prio = ew->pl_no * 4 + 4 ;
	else ew->sub_prio = ew->pl_no * 4 + 4 ;
	shl11_01_lw->zombie_shot_flag = 1 ;
	shl11_01_lw->zombie_land_flag = 0 ;
	ew->atck_no = pl11_at_zombie_h ;
	HIT_RESET( ew ) ;
	ew->atck_flag = HIT_ON | HIT_PL_ONLY ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	if( ew->pl_no ) ew->atck_attr = ATR_FIRE ;	/* both arms alike */
	else ew->atck_attr = ATR_FIRE ;
	ew->size_front = 64 ;
	ew->size_back = 64 ;
	ew->pos.x = ew->master_w->pl_dir ? scr_work.eye[0].x + -DOT_X( 256.0f ) : scr_work.eye[0].x + DOT_X( 256.0f ) ;
	ew->pos.y = shl11_01_ew_w->land_y = ew->master_w->ground_y + DOT_Y( 8.0f ) ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	if( ew->disp_dir ) ew->spd.x = 2.5f ;
	else ew->spd.x = -2.5f ;
	shl11_01_part_set( ew, 0 ) ;
	shl11_01_part_set( ew, 1 ) ;
	ew->dir = 0 ;
	ew->timer0 = 60 ;
	shl11_01_ew_w->voice_timer = 180 ;
	pl_char_set( ew, Char_Effect, 32 ) ;
	pl_voice_req( ew, 48 ) ;
}

static void shl11_01_c_fly( PLAYER_WORK *ew );
static void shl11_01_c_fall( PLAYER_WORK *ew );
static void shl11_01_c_wait( PLAYER_WORK *ew );
static void shl11_01_c_fade( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl11_01_c_move( PLAYER_WORK *ew )
{
	static void ( *shl11_01_c_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl11_01_c_fly, shl11_01_c_fall, shl11_01_c_wait, shl11_01_c_fade,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( ew->dir ){
		if( ew->timer0 & 2 ) ef_color_set( pl_w, 2, 2, 1 ) ;
		else pl_color_trans( pl_w, 2, 2, 1 ) ;
		if( !-- ew->timer0 ){
			ew->dir = 0 ;
			ew->timer0 = 60 ;
		}
	}
	else {
		pl_color_trans( pl_w, 2, 2, 1 ) ;
		if( !-- ew->timer0 ){
			ew->dir = 1 ;
			ew->timer0 = 10 ;
		}
	}
	shl11_01_c_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void shl11_01_c_fly( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->hit_flag || ew->hit_box_no ){
		if( ew->hit_box_no ){
			catch_hitmark_no_se( ew ) ;
			pl_voice_req( ew, 44 ) ;
			pl_voice_req( ew, 48 ) ;
		}
		ew->routine_1 ++ ;
		if( ew->disp_dir ) ew->spd.x = -5.0f ;
		else ew->spd.x = 5.0f ;
		ew->acc.x = 0.0f ;
		ew->spd.y = DOT_Y( 4.0f ) ;
		ew->acc.y = -DOT_Y( 0.375f ) ;
		ew->pos.y = shl11_01_ew_w->land_y ;
		pl_char_set( ew, Char_Effect, 12 ) ;
		return ;
	}
	if( !scr_in_x_ck( ew ) ){
		shl11_01_lw->zombie_shot_flag = 0 ;
		shl11_01_end( ew ) ;
		return ;
	}
	if( !-- shl11_01_ew_w->voice_timer ){
		shl11_01_ew_w->voice_timer = 180 ;
		pl_voice_req( ew, 49 ) ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl11_01_c_fall( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y <= shl11_01_ew_w->land_y ){
		ew->routine_1 ++ ;
		ew->pos.y = shl11_01_ew_w->land_y ;
		ew->timer1 = 3 ;
		if( ew->disp_dir ) ew->spd.x = -DOT_X( 2.0f ) ;
		else ew->spd.x = DOT_X( 2.0f ) ;
		ew->acc.x = 0.0f ;
		ew->spd.y = DOT_Y( 2.0f ) ;
		ew->acc.y = -DOT_Y( 0.375f ) ;
		pl_char_set( ew, Char_Effect, 13 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl11_01_c_wait( PLAYER_WORK *ew )
{
	if( ew->timer1 && -- ew->timer1 ) return ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y <= shl11_01_ew_w->land_y ){
		ew->routine_1 ++ ;
		ew->pos.y = shl11_01_ew_w->land_y ;
		ew->alpha = 1.0f ;
		shl11_01_lw->zombie_land_flag = 1 ;
	}
}

/*--------------------------------------------------------------*/
static void shl11_01_c_fade( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	if( ( ew->alpha -= 0.05f ) <= 0.0f ){
		shl11_01_lw->zombie_shot_flag = 0 ;
		shl11_01_end( ew ) ;
	}
}

static void shl11_01_d_init( PLAYER_WORK *ew );
static void shl11_01_d_move( PLAYER_WORK *ew );

/*==============================================================*/
static void shl11_01_d_main( PLAYER_WORK *ew )
{
	static void ( *shl11_01_d_jmp[] )( PLAYER_WORK *ew ) = {
		shl11_01_d_init, shl11_01_d_move, shl11_01_end, shl11_01_free,
	} ;

	shl11_01_d_jmp[ew->routine_0]( ew ) ;
}

static void shl11_01_d_pos( PLAYER_WORK *ew );

/*==============================================================*/
static void shl11_01_d_init( PLAYER_WORK *ew )
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
	ew->prio = 12 ;
	ew->sub_prio = ew->ef_w->sub_prio + ( ew->sub_type ? 1 : -1 ) ;
	ef_char_set( ew, 27, 24 ) ;
	shl11_01_d_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl11_01_d_move( PLAYER_WORK *ew )
{
	ef_char_move( ew ) ;
	shl11_01_d_pos( ew ) ;
	if( shl11_01_lw->zombie_shot_flag ){
		if( !ew->routine_1 ){
			if( shl11_01_lw->zombie_land_flag ){
				ew->routine_1 ++ ;
				ew->alpha = 1.0f ;
			}
		}
		else if( ( ew->alpha -= 0.05f ) <= 0.0f ) shl11_01_end( ew ) ;
	}
	else shl11_01_end( ew ) ;
}

/*==============================================================*/
static void shl11_01_d_pos( PLAYER_WORK *ew )
{
	Float dx ;
	static Float shl11_01_d_tbl[][2] = {
		{ -DOT_X( 10.0f ), DOT_Y( 58.0f ) }, { -DOT_X( 13.0f ), DOT_Y( 88.0f ) }, { -DOT_X( 10.0f ), DOT_Y( 58.0f ) },
		{ -DOT_X( 13.0f ), DOT_Y( 88.0f ) }, { -DOT_X( 10.0f ), DOT_Y( 58.0f ) }, { -DOT_X( 13.0f ), DOT_Y( 88.0f ) },
		{ -DOT_X( 20.0f ), DOT_Y( 68.0f ) }, { DOT_X( 7.0f ), DOT_Y( 81.0f ) }, { -35.0f, DOT_Y( 57.0f ) },
		{ -DOT_X( 16.0f ), DOT_Y( 82.0f ) }, { -20.0f, 75.0f }, { -DOT_X( 41.0f ), DOT_Y( 57.0f ) },
		{ -15.0f, 30.0f }, { -55.0f, DOT_Y( 25.0f ) }, { -DOT_X( 11.0f ), 30.0f },
		{ -DOT_X( 37.0f ), DOT_Y( 15.0f ) }, { -DOT_X( 11.0f ), 15.0f }, { -60.0f, DOT_Y( 13.0f ) },
	} ;

	ew->pos.x = ew->ef_w->pos.x ;
	ew->pos.y = ew->ef_w->pos.y ;
	dx = shl11_01_d_tbl[ew->ef_w->cg_flag1 * 2 + ew->sub_type][0] ;
	ew->pos.x += ew->disp_dir ? dx : -dx ;
	ew->pos.y += shl11_01_d_tbl[ew->ef_w->cg_flag1 * 2 + ew->sub_type][1] ;
}

/*--------------------------------------------------------------*/
static void shl11_01_end( PLAYER_WORK *ew )
{
	ew->routine_0 = 3 ;
	ew->disp_flag = 0 ;
	shl11_01_lw->zombie_shot_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl11_01_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl11_01.c" End of File	*/

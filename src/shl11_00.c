/* Jill's effect 00 (character 11's shot 00): the dog of Cerberus Escape (type 0, along the ground)
   and the crow of Crow Escape (type 1, from above). It comes to Jill, lands, and fades. */
#include "player.h"
#include "pl_char.h"
#include "pl11_def.h"

static PL11_LOCAL_W *shl11_00_lw ;	/* Jill's tlocal, set each frame by the main routine */
static Sint32 *shl11_00_ew_w ;	/* the effect's own work */

static void shl11_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl11_00_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl11_00_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
		ew->ef_no = 0x1100 ;
	}
	return ew ;
}

static void shl11_00_a_main( PLAYER_WORK *ew );
static void shl11_00_b_main( PLAYER_WORK *ew );

/*==============================================================*/
static void shl11_00_main( PLAYER_WORK *ew )
{
	static void ( *shl11_00_jmp[] )( PLAYER_WORK *ew ) = {
		shl11_00_a_main, shl11_00_b_main,
	} ;

	shl11_00_lw = ( PL11_LOCAL_W * )&ew->master_w->tlocal[0] ;
	shl11_00_ew_w = &ew->obj_flag ;
	shl11_00_jmp[ew->type]( ew ) ;
}

static void shl11_00_a_init( PLAYER_WORK *ew );
static void shl11_00_a_move( PLAYER_WORK *ew );
static void shl11_00_a_hit( PLAYER_WORK *ew );
static void shl11_00_free( PLAYER_WORK *ew );
static Uint8 shl11_00_near( PLAYER_WORK *ew, PLAYER_WORK *pl_w, Uint16 dist );

/*==============================================================*/
static void shl11_00_a_main( PLAYER_WORK *ew )
{
	static void ( *shl11_00_a_jmp[] )( PLAYER_WORK *ew ) = {
		shl11_00_a_init, shl11_00_a_move, shl11_00_a_hit, shl11_00_free,
	} ;

	shl11_00_a_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl11_00_a_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 1 ;
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
	ew->disp_dir = ew->master_w->disp_dir ;
	shl11_00_lw->cerberus_shot_flag = 1 ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->atck_no = pl11_at_cerberus ;
	HIT_RESET( ew ) ;
	ew->size_front = 64 ;
	ew->size_back = 64 ;
	ew->pos.x = ew->master_w->pl_dir ? scr_work.eye[0].x + -480.0f : scr_work.eye[0].x + 480.0f ;
	ew->pos.y = ew->master_w->ground_y ;
	ew->timer0 = 16 ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	if( ew->disp_dir ) ew->spd.x = 10.0f ;
	else ew->spd.x = -10.0f ;
	pl_shadow_set( ew ) ;
	ew->be_flag = 1 ;
	pl_char_set( ew, Char_Effect, 0 ) ;
}

static void shl11_00_a_fly( PLAYER_WORK *ew );
static void shl11_00_a_arc( PLAYER_WORK *ew );
static void shl11_00_a_land( PLAYER_WORK *ew );
static void shl11_00_a_stand( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl11_00_a_move( PLAYER_WORK *ew )
{
	static void ( *shl11_00_a_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl11_00_a_fly, shl11_00_a_arc, shl11_00_a_land, shl11_00_a_stand,
	} ;

	if( ( !ew->timer0 || !-- ew->timer0 ) && !scr_in_x_ck( ew ) ){
		ew->routine_0 = 3 ;
		shl11_00_lw->cerberus_shot_flag = 0 ;
		shl11_00_free( ew ) ;
		return ;
	}
	if( ew->hit_box_no ){
		ew->routine_0 ++ ;
		ew->routine_1 = 0 ;
		catch_hitmark_no_se( ew ) ;
		pl_voice_req( ew, 44 ) ;
		return ;
	}
	shl11_00_a_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void shl11_00_a_fly( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	if( !shl11_00_lw->cerberus_near_flag && shl11_00_near( ew, ew->master_w, 96 ) ){
		ew->routine_1 ++ ;
		shl11_00_lw->cerberus_near_flag = 1 ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl11_00_a_arc( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	if( !shl11_00_near( ew, ew->master_w, 96 ) ){
		ew->routine_1 ++ ;
		ew->spd.y = DOT_Y( 4.0f ) ;
		ew->acc.y = -DOT_Y( 0.25f ) ;
		pl_char_set( ew, Char_Effect, 1 ) ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl11_00_a_land( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	if( ew->pos.y <= ew->master_w->ground_y ){
		ew->routine_1 ++ ;
		ew->pos.y = ew->master_w->ground_y ;
		ew->spd.y = 0.0f ;
		ew->acc.y = 0.0f ;
		pl_char_set( ew, Char_Effect, 2 ) ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl11_00_a_stand( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	if( !ew->cg_type ) ew->routine_1 = 0 ;
	hit_work_set( ew ) ;
}

static void shl11_00_a_hit_init( PLAYER_WORK *ew );
static void shl11_00_a_hit_fall( PLAYER_WORK *ew );
static void shl11_00_a_hit_bound( PLAYER_WORK *ew );
static void shl11_00_a_hit_fade( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl11_00_a_hit( PLAYER_WORK *ew )
{
	static void ( *shl11_00_a_hit_jmp[] )( PLAYER_WORK *ew ) = {
		shl11_00_a_hit_init, shl11_00_a_hit_fall, shl11_00_a_hit_bound, shl11_00_a_hit_fade,
	} ;

	shl11_00_a_hit_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void shl11_00_a_hit_init( PLAYER_WORK *ew )
{
	ew->routine_1 ++ ;
	if( ew->disp_dir ) ew->spd.x = -1.6666666f ;
	else ew->spd.x = 1.6666666f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = DOT_Y( 6.0f ) ;
	ew->acc.y = -DOT_Y( 0.75f ) ;
	pl_char_set( ew, Char_Effect, 3 ) ;
	shl11_00_a_hit_fall( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl11_00_a_hit_fall( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	if( ew->pos.y <= ew->master_w->ground_y ){
		ew->routine_1 ++ ;
		ew->timer0 = 3 ;
		if( ew->disp_dir ) ew->spd.x = -1.6666666f ;
		else ew->spd.x = 1.6666666f ;
		ew->acc.x = 0.0f ;
		ew->spd.y = DOT_Y( 2.0f ) ;
		ew->acc.y = -DOT_Y( 0.5f ) ;
		pl_char_set( ew, Char_Effect, 4 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl11_00_a_hit_bound( PLAYER_WORK *ew )
{
	if( ew->timer0 && -- ew->timer0 ) return ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y <= ew->master_w->ground_y ){
		ew->routine_1 ++ ;
		ew->pos.y = ew->master_w->ground_y ;
		ew->alpha = 1.0f ;
	}
}

/*--------------------------------------------------------------*/
static void shl11_00_a_hit_fade( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	if( ( ew->alpha -= 0.05f ) <= 0.0f ){
		ew->routine_0 ++ ;
		shl11_00_lw->cerberus_shot_flag = 0 ;
		shl11_00_free( ew ) ;
	}
}

/*==============================================================*/
static Uint8 shl11_00_near( PLAYER_WORK *ew, PLAYER_WORK *pl_w, Uint16 dist )
{
	Float dx = fabs( ew->pos.x - pl_w->pos.x ) ;

	if( dx <= DOT_X( dist ) ) return 1 ;
	return 0 ;
}

static void shl11_00_b_init( PLAYER_WORK *ew );
static void shl11_00_b_move( PLAYER_WORK *ew );
static void shl11_00_b_hit( PLAYER_WORK *ew );

/*==============================================================*/
static void shl11_00_b_main( PLAYER_WORK *ew )
{
	static void ( *shl11_00_b_jmp[] )( PLAYER_WORK *ew ) = {
		shl11_00_b_init, shl11_00_b_move, shl11_00_b_hit, shl11_00_free,
	} ;

	shl11_00_b_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
/*			The unused 12-byte local is the frame its size		*/
/*			shows												*/
/*==============================================================*/
static void shl11_00_b_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	VEC3 unused ;

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
	shl11_00_lw->crow_shot_flag = 1 ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	if( pl_w->team_type == 3 ){
		ew->atck_no = pl11_at_crow_vc ;
		HIT_RESET( ew ) ;
	} else {
		ew->atck_no = pl11_at_crow ;
		HIT_RESET( ew ) ;
	}
	ew->size_front = 64 ;
	ew->size_back = 64 ;
	ew->size_top = 0x80 ;
	ew->size_bottom = 8 ;
	ew->pos.x = ew->disp_dir ? scr_work.eye[0].x + -DOT_X( 256.0f ) : scr_work.eye[0].x + DOT_X( 256.0f ) ;
	ew->pos.y = ew->master_w->pos.y + 480.0f ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	if( ew->disp_dir ) ew->spd.x = 10.0f ;
	else ew->spd.x = -10.0f ;
	ew->spd.y = -DOT_Y( 8.0f ) ;
	ew->acc.y = 0.4017857f ;
	ew->timer0 = 16 ;
	pl_shadow_set( ew ) ;
	ew->be_flag = 1 ;
	pl_char_set( ew, Char_Effect, 5 ) ;
}

static void shl11_00_b_fly( PLAYER_WORK *ew );
static void shl11_00_b_wait( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl11_00_b_move( PLAYER_WORK *ew )
{
	static void ( *shl11_00_b_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl11_00_b_fly, shl11_00_b_wait,
	} ;

	if( ( !ew->timer0 || !-- ew->timer0 ) && !scr_in_ck( ew ) ){
		ew->routine_0 = 3 ;
		shl11_00_lw->crow_shot_flag = 0 ;
		shl11_00_free( ew ) ;
		return ;
	}
	if( ew->hit_box_no ){
		ew->routine_0 ++ ;
		ew->routine_1 = 0 ;
		catch_hitmark_no_se( ew ) ;
		pl_voice_req( ew, 44 ) ;
		return ;
	}
	if( !shl11_00_lw->crow_near_flag && shl11_00_near( ew, ew->master_w, 64 ) ) shl11_00_lw->crow_near_flag = 1 ;
	shl11_00_b_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void shl11_00_b_fly( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->spd.y >= 0.0f ){
		ew->routine_1 ++ ;
		pl_char_set( ew, Char_Effect, 6 ) ;
		pls11_00_set2( ew, 17, 0 ) ;
		pls11_00_set2( ew, 17, 0 ) ;
		return ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl11_00_b_wait( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	hit_work_set( ew ) ;
}

static void shl11_00_b_hit_init( PLAYER_WORK *ew );
static void shl11_00_b_hit_fall( PLAYER_WORK *ew );
static void shl11_00_b_hit_fade( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl11_00_b_hit( PLAYER_WORK *ew )
{
	static void ( *shl11_00_b_hit_jmp[] )( PLAYER_WORK *ew ) = {
		shl11_00_b_hit_init, shl11_00_b_hit_fall, shl11_00_b_hit_fade,
	} ;

	shl11_00_b_hit_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void shl11_00_b_hit_init( PLAYER_WORK *ew )
{
	ew->routine_1 ++ ;
	if( ew->disp_dir ) ew->spd.x = DOT_X( 0.375f ) ;
	else ew->spd.x = -DOT_X( 0.375f ) ;
	ew->acc.x = 0.0f ;
	ew->spd.y = 2.142857f ;
	ew->acc.y = -DOT_Y( 0.375f ) ;
	pl_char_set( ew, Char_Effect, 7 ) ;
	pls11_00_set2( ew, 17, 1 ) ;
	pls11_00_set2( ew, 17, 1 ) ;
	shl11_00_b_hit_fall( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl11_00_b_hit_fall( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y <= ew->master_w->ground_y ){
		ew->routine_1 ++ ;
		ew->pos.y = ew->master_w->ground_y ;
		ew->alpha = 1.0f ;
		pl_char_set( ew, Char_Effect, 8 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl11_00_b_hit_fade( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	if( ( ew->alpha -= 0.05f ) <= 0.0f ){
		ew->routine_0 ++ ;
		shl11_00_lw->crow_shot_flag = 0 ;
		shl11_00_free( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl11_00_free( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl11_00.c" End of File	*/

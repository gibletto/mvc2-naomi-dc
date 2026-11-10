/* Strider's effect 02 (character 05's shot 02): the animals and the robot that run in from the
   screen's edge (types 0 to 2, 5 and 6), and the one that charges from Strider himself (type 4). */
#include "player.h"
#include "pl_char.h"
#include "pl05_def.h"


static Sint32 *lw;	/* the running work's, unused */
static PL05_LOCAL_W *pl_lw;	/* Strider's */

static void shl05_02_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl05_02_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl05_02_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->ef_no = 0x502 ;
	}
	return ew ;
}

/****************************************************************/
/*			Eight of type 2 and eight of type 1, numbered in	*/
/*			sub_type											*/
/****************************************************************/
void *shl05_02_legion_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	int i ;

	for( i = 0 ; i < 8 ; i ++ ){
		if( !( ew = work_get( 0, W_SHL, LIST_TOP ) ) ) break ;
		ew->work_func = shl05_02_main ;
		ew->master_w = pl_w ;
		ew->type = 2 ;
		ew->sub_type = i ;
		ew->ef_no = 0x502 ;
	}
	for( i = 0 ; i < 8 ; i ++ ){
		if( !( ew = work_get( 0, W_SHL, LIST_TOP ) ) ) break ;
		ew->work_func = shl05_02_main ;
		ew->master_w = pl_w ;
		ew->type = 1 ;
		ew->sub_type = i ;
		ew->ef_no = 0x502 ;
	}
	return ew ;
}

static void shl05_02_init( PLAYER_WORK *ew );
static void shl05_02_move( PLAYER_WORK *ew );
static void shl05_02_end( PLAYER_WORK *ew );
static void shl05_02_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl05_02_main( PLAYER_WORK *ew )
{
	static void ( *shl05_02_jmp[] )( PLAYER_WORK *ew ) = {
		shl05_02_init, shl05_02_move, shl05_02_end, shl05_02_free,
	} ;

	pl_lw = ( PL05_LOCAL_W * )&ew->master_w->tlocal[0] ;
	lw = &ew->obj_flag ;
	shl05_02_jmp[ew->routine_0]( ew ) ;
}

static void shl05_02_init0( PLAYER_WORK *ew );
static void shl05_02_init1( PLAYER_WORK *ew );
static void shl05_02_init2( PLAYER_WORK *ew );
static void shl05_02_init4( PLAYER_WORK *ew );
static void shl05_02_init5( PLAYER_WORK *ew );
static void shl05_02_init6( PLAYER_WORK *ew );

/*==============================================================*/
static void shl05_02_init( PLAYER_WORK *ew )
{
	static void ( *shl05_02_init_jmp[] )( PLAYER_WORK *ew ) = {
		shl05_02_init0, shl05_02_init1, shl05_02_init2, shl05_02_free,
		shl05_02_init4, shl05_02_init5, shl05_02_init6,
	} ;

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
	ew->disp_dir = ew->master_w->disp_dir ;
	shl05_02_init_jmp[ew->type]( ew ) ;
}

static void shl05_02_scr_pos( PLAYER_WORK *ew );

/*==============================================================*/
static void shl05_02_init0( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->disp_flag = 1 ;
	ew->prio = 11 ;
	ew->pos.y = pl_w->ground_y ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	if( pl_w->team_type == 3 ) ew->atck_no = pl05_at_vc_formation_a ;
	else ew->atck_no = pl05_at_formation_a_l ;
	HIT_RESET( ew ) ;
	shl05_02_scr_pos( ew ) ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.x = ew->disp_dir ? DOT_X( 8.0f ) : -DOT_X( 8.0f ) ;
	ew->acc.x = 0.0f ;
	pl_shadow_set( ew ) ;
	ew->be_flag = 1 ;
	pl_voice_req( ew, 31 ) ;
	pl_char_set( ew, Char_Effect, 16 ) ;
}

/*==============================================================*/
static void shl05_02_init1( PLAYER_WORK *ew )
{
	static Float shl05_02_y1[8] = {
		-DOT_Y( 16.0f ), -DOT_Y( 16.0f ), DOT_Y( 96.0f ), -DOT_Y( 16.0f ),
		-DOT_Y( 16.0f ), -DOT_Y( 16.0f ), DOT_Y( 64.0f ), DOT_Y( 64.0f ),
	} ;
	static Sint16 shl05_02_time1[8] = { 0, 0, 6, 0, 32, 12, 12, 0 } ;
	Float rnd ;

	ew->disp_flag = 0 ;
	ew->prio = 0 ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->atck_no = pl05_at_legion ;
	HIT_RESET( ew ) ;
	shl05_02_scr_pos( ew ) ;
	ew->spd.x = ew->disp_dir ? DOT_X( 4.0f ) : -DOT_X( 4.0f ) ;
	ew->acc.x = 0.0f ;
	ew->spd.y = DOT_Y( 3.0f ) ;
	ew->acc.y = -0.6696428f ;
	pl_shadow_set( ew ) ;
	ew->be_flag = 1 ;
	rnd = ( nlRand() & 0x3000 ) * 3.2697404e-05f ;
	ew->acc.y -= rnd ;
	ew->pos.y += shl05_02_y1[ew->sub_type] ;
	ew->timer0 = shl05_02_time1[ew->sub_type] ;
	pl_char_set( ew, Char_Effect, 17 ) ;
}

/*==============================================================*/
static void shl05_02_init2( PLAYER_WORK *ew )
{
	static Float shl05_02_y2[8] = {
		DOT_Y( 128.0f ), DOT_Y( 120.0f ), DOT_Y( 160.0f ), DOT_Y( 128.0f ),
		240.0f, DOT_Y( 144.0f ), 360.0f, DOT_Y( 152.0f ),
	} ;
	static Sint16 shl05_02_time2[8] = { 0, 8, 16, 24, 32, 40, 48, 56 } ;
	Float rnd ;

	ew->disp_flag = 0 ;
	ew->prio = 0 ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->atck_no = pl05_at_legion_2 ;
	HIT_RESET( ew ) ;
	shl05_02_scr_pos( ew ) ;
	ew->spd.x = ew->disp_dir ? 15.0f : -15.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = DOT_Y( 3.0f ) ;
	ew->acc.y = -0.6696428f ;
	rnd = ( nlRand() & 0x3000 ) * 3.2697404e-05f ;
	ew->acc.y -= rnd ;
	ew->pos.y += shl05_02_y2[ew->sub_type] ;
	ew->timer0 = shl05_02_time2[ew->sub_type] ;
	pl_char_set( ew, Char_Effect, 35 ) ;
}

/*==============================================================*/
/*			At the screen's edge behind the opponent			*/
/*==============================================================*/
static void shl05_02_scr_pos( PLAYER_WORK *ew )
{
	ew->pos.x = scr_work.eye[0].x ;
	ew->pos.x += ew->disp_dir ? -DOT_X( 256.0f ) : DOT_X( 256.0f ) ;
	ew->size_top = 64 ;
	ew->size_bottom = 16 ;
	ew->size_front = 96 ;
	ew->size_back = 96 ;
}

static void shl05_02_master_pos( PLAYER_WORK *ew );

/*==============================================================*/
static void shl05_02_init4( PLAYER_WORK *ew )
{
	ew->prio = 0 ;
	shl05_02_master_pos( ew ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	pl_lw->panther_go_flag = 0 ;
	pl_lw->panther_flag = -1 ;
	ew->size_top = 56 ;
	ew->size_bottom = 56 ;
	ew->size_front = 80 ;
	ew->size_back = 32 ;
	pl_char_set( ew, Char_Effect, 27 ) ;
}

/*==============================================================*/
/*			In front of Strider									*/
/*==============================================================*/
static void shl05_02_master_pos( PLAYER_WORK *ew )
{
	ew->disp_dir = ew->master_w->pl_dir ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.x += ew->disp_dir ? -DOT_X( 34.0f ) : DOT_X( 34.0f ) ;
	ew->pos.y += DOT_Y( 50.0f ) ;
}

/*==============================================================*/
static void shl05_02_init5( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	ew->prio = 0 ;
	shl05_02_scr_pos( ew ) ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->timer0 = 48 ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.x = ew->disp_dir ? 10.0f : -10.0f ;
	pl_shadow_set( ew ) ;
	ew->be_flag = 1 ;
	pl_char_set( ew, Char_Effect, 16 ) ;
}

/*==============================================================*/
static void shl05_02_init6( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->prio = 0 ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->atck_no = pl05_at_formation_a_h ;
	HIT_RESET( ew ) ;
	shl05_02_scr_pos( ew ) ;
	ew->pos.y = pl_w->ground_y + DOT_Y( 176.0f ) ;
	ew->spd.x = ew->disp_dir ? DOT_X( 8.5f ) : -DOT_X( 8.5f ) ;
	ew->acc.x = 0.0f ;
	ew->spd.y = -DOT_Y( 4.75f ) ;
	ew->acc.y = 0.3348214f ;
	pl_char_set( ew, Char_Effect, 35 ) ;
}

static void shl05_02_move0( PLAYER_WORK *ew );
static void shl05_02_move1( PLAYER_WORK *ew );
static void shl05_02_move2( PLAYER_WORK *ew );
static void shl05_02_move4( PLAYER_WORK *ew );
static void shl05_02_move5( PLAYER_WORK *ew );
static void shl05_02_move6( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl05_02_move( PLAYER_WORK *ew )
{
	static void ( *shl05_02_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl05_02_move0, shl05_02_move1, shl05_02_move2, shl05_02_free,
		shl05_02_move4, shl05_02_move5, shl05_02_move6,
	} ;

	shl05_02_move_jmp[ew->type]( ew ) ;
}

static void shl05_02_0_run( PLAYER_WORK *ew );
static void shl05_02_0_fly( PLAYER_WORK *ew );
static void shl05_02_0_blink( PLAYER_WORK *ew );

/*==============================================================*/
static void shl05_02_move0( PLAYER_WORK *ew )
{
	static void ( *shl05_02_0_jmp[] )( PLAYER_WORK *ew ) = {
		shl05_02_0_run, shl05_02_0_fly, shl05_02_0_blink,
	} ;

	shl05_02_0_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
/*			Running at the opponent								*/
/*==============================================================*/
static void shl05_02_0_run( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	pl_char_move( ew ) ;
	if( ew->hit_flag ){
		PLAYER_WORK *em_w = ew->em_w ;

		if( !em_w->work_kind ){
			ew->routine_1 ++ ;
			goto scr_ck ;
		}
		ew->routine_1 = 2 ;
		ew->timer0 = 24 ;
		pl_char_set( ew, Char_Effect, 17 ) ;
		return ;
	}
	if( ew->hit_box_no ){
		catch_hitmark( ew ) ;
		ew->routine_1 ++ ;
		pl_char_set( ew, Char_Effect, 21 ) ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		ew->spd.x = ew->disp_dir ? -15.0f : 15.0f ;
		ew->spd.y = DOT_Y( 2.0f ) ;
	}
scr_ck:
	if( !scr_in_x_ck( ew ) ){
		shl05_02_end( ew ) ;
		return ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl05_02_0_fly( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	if( !scr_in_x_ck( ew ) ){
		shl05_02_end( ew ) ;
		return ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl05_02_0_blink( PLAYER_WORK *ew )
{
	if( --ew->timer0 < 0 ){
		shl05_02_end( ew ) ;
		return ;
	}
	ew->disp_flag = Sys->game_timer0 & 1 ;
}

static void shl05_02_1_wait( PLAYER_WORK *ew );
static void shl05_02_1_land( PLAYER_WORK *ew );
static void shl05_02_1_jump( PLAYER_WORK *ew );
static void shl05_02_1_jump_land( PLAYER_WORK *ew );
static void shl05_02_1_run( PLAYER_WORK *ew );
static void shl05_02_1_blink( PLAYER_WORK *ew );
static void shl05_02_1_fly( PLAYER_WORK *ew );

/*==============================================================*/
static void shl05_02_move1( PLAYER_WORK *ew )
{
	static void ( *shl05_02_1_jmp[] )( PLAYER_WORK *ew ) = {
		shl05_02_1_wait, shl05_02_1_land, shl05_02_1_jump, shl05_02_1_jump_land,
		shl05_02_1_run, shl05_02_1_blink, shl05_02_1_fly,
	} ;

	shl05_02_1_jmp[ew->routine_1]( ew ) ;
}

static void shl05_02_catch( PLAYER_WORK *ew );
static void shl05_02_hit( PLAYER_WORK *ew );

/*==============================================================*/
static void shl05_02_1_wait( PLAYER_WORK *ew )
{
	if( ew->timer0 >= 0 ){
		if( ew->master_w->routine_1 ){
			shl05_02_end( ew ) ;
			return ;
		}
		ew->timer0 -- ;
		return ;
	}
	ew->disp_flag = 1 ;
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y < ew->master_w->ground_y ){
		ew->routine_1 ++ ;
		ew->pos.y = ew->master_w->ground_y ;
		pl_char_set( ew, Char_Effect, 18 ) ;
	}
	if( ew->hit_box_no ){
		shl05_02_catch( ew ) ;
		return ;
	}
	if( ew->hit_flag ) shl05_02_hit( ew ) ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl05_02_1_land( PLAYER_WORK *ew )
{
	static Float shl05_02_jump_spd[8][4] = {
		{ 20.0f, -0.0f, DOT_Y( 8.0f ), -DOT_Y( 0.375f ) },
		{ DOT_X( 14.0f ), -0.0f, DOT_Y( 4.0f ), -DOT_Y( 0.25f ) },
		{ 0.0f, -0.0f, 0.0f, -0.0f },
		{ 0.0f, -0.0f, 0.0f, -0.0f },
		{ 20.0f, -0.0f, DOT_Y( 8.0f ), -DOT_Y( 0.375f ) },
		{ DOT_X( 14.0f ), -0.0f, DOT_Y( 4.0f ), -DOT_Y( 0.25f ) },
		{ DOT_X( 14.0f ), -0.0f, DOT_Y( 4.0f ), -DOT_Y( 0.25f ) },
		{ 20.0f, -0.0f, DOT_Y( 8.0f ), -DOT_Y( 0.375f ) },
	} ;

	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		ew->spd.x = ew->disp_dir ? shl05_02_jump_spd[ew->sub_type][0] : -shl05_02_jump_spd[ew->sub_type][0] ;
		ew->acc.x = ew->disp_dir ? shl05_02_jump_spd[ew->sub_type][1] : -shl05_02_jump_spd[ew->sub_type][1] ;
		ew->spd.y = shl05_02_jump_spd[ew->sub_type][2] ;
		ew->acc.y = shl05_02_jump_spd[ew->sub_type][3] ;
		pl_voice_req( ew, 31 ) ;
		pl_char_set( ew, Char_Effect, 19 ) ;
	}
	if( ew->hit_box_no ){
		shl05_02_catch( ew ) ;
		return ;
	}
	if( ew->hit_flag ){
		shl05_02_hit( ew ) ;
		return ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl05_02_1_jump( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !scr_in_x_ck( ew ) ){
		shl05_02_end( ew ) ;
		return ;
	}
	if( ew->pos.y <= ew->master_w->ground_y ){
		ew->routine_1 ++ ;
		ew->pos.y = ew->master_w->ground_y ;
		pl_char_set( ew, Char_Effect, 18 ) ;
	}
	if( ew->hit_box_no ){
		shl05_02_catch( ew ) ;
		return ;
	}
	if( ew->hit_flag ){
		shl05_02_hit( ew ) ;
		return ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl05_02_1_jump_land( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		ew->spd.x = ew->disp_dir ? DOT_X( 8.0f ) : -DOT_X( 8.0f ) ;
		ew->acc.x = 0.0f ;
		pl_char_set( ew, Char_Effect, 16 ) ;
	}
	if( ew->hit_box_no ){
		shl05_02_catch( ew ) ;
		return ;
	}
	if( ew->hit_flag ){
		shl05_02_hit( ew ) ;
		return ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl05_02_1_run( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	if( !scr_in_x_ck( ew ) ){
		shl05_02_end( ew ) ;
		return ;
	}
	if( ew->hit_box_no ){
		shl05_02_catch( ew ) ;
		return ;
	}
	if( ew->hit_flag ){
		shl05_02_hit( ew ) ;
		return ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl05_02_1_blink( PLAYER_WORK *ew )
{
	if( --ew->timer0 < 0 ){
		shl05_02_end( ew ) ;
		return ;
	}
	ew->disp_flag = Sys->game_timer0 & 1 ;
}

/*--------------------------------------------------------------*/
static void shl05_02_1_fly( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	if( !scr_in_x_ck( ew ) ) shl05_02_end( ew ) ;
}

/*==============================================================*/
/*			Struck by an attack: thrown back					*/
/*==============================================================*/
static void shl05_02_catch( PLAYER_WORK *ew )
{
	catch_hitmark( ew ) ;
	ew->routine_1 = 6 ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.x = ew->disp_dir ? -15.0f : 15.0f ;
	ew->spd.y = DOT_Y( 2.0f ) ;
	pl_char_set( ew, Char_Effect, 21 ) ;
}

/*==============================================================*/
/*			It hit an effect: blinks out						*/
/*==============================================================*/
static void shl05_02_hit( PLAYER_WORK *ew )
{
	PLAYER_WORK *em_w = ew->em_w ;

	if( em_w->work_kind ){
		ew->routine_1 = 5 ;
		ew->timer0 = 24 ;
		pl_char_set( ew, Char_Effect, 17 ) ;
	}
}

static void shl05_02_2_wait( PLAYER_WORK *ew );
static void shl05_02_2_fly( PLAYER_WORK *ew );

/*==============================================================*/
static void shl05_02_move2( PLAYER_WORK *ew )
{
	static void ( *shl05_02_2_jmp[] )( PLAYER_WORK *ew ) = {
		shl05_02_2_wait, shl05_02_2_fly,
	} ;

	shl05_02_2_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void shl05_02_2_wait( PLAYER_WORK *ew )
{
	static Float shl05_02_fly_spd[8][2] = {
		{ -DOT_Y( 3.0f ), 0.4017857f }, { -DOT_Y( 2.125f ), DOT_Y( 0.125f ) },
		{ -DOT_Y( 2.0f ), 0.4017857f }, { -DOT_Y( 0.25f ), 0.0f },
		{ -2.142857f, DOT_Y( 0.25f ) }, { -DOT_Y( 2.0f ), 0.4017857f },
		{ -DOT_Y( 3.0f ), DOT_Y( 0.375f ) }, { -DOT_Y( 3.0f ), 0.13392857f },
	} ;

	if( ew->master_w->routine_1 ){
		shl05_02_end( ew ) ;
		return ;
	}
	if( --ew->timer0 < 0 ){
		ew->routine_1 ++ ;
		ew->disp_flag = 1 ;
		ew->spd.y = shl05_02_fly_spd[ew->sub_type][0] ;
		ew->acc.y = shl05_02_fly_spd[ew->sub_type][1] ;
	}
}

/*--------------------------------------------------------------*/
static void shl05_02_2_fly( PLAYER_WORK *ew )
{
	VEC3 pos ;

	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	if( !scr_in_x_ck( ew ) ){
		shl05_02_end( ew ) ;
		return ;
	}
	if( ew->hit_box_no ){
		pos = ew->pos ;
		pos.y += DOT_Y( 8.0f ) ;
		burst_set( ew, &pos, 1, 0xc9 ) ;
		shl05_02_end( ew ) ;
	}
	hit_work_set( ew ) ;
}

static void shl05_02_4_wait( PLAYER_WORK *ew );
static void shl05_02_4_fly( PLAYER_WORK *ew );

/*==============================================================*/
static void shl05_02_move4( PLAYER_WORK *ew )
{
	static void ( *shl05_02_4_jmp[] )( PLAYER_WORK *ew ) = {
		shl05_02_4_wait, shl05_02_4_fly,
	} ;

	shl05_02_4_jmp[ew->routine_1]( ew ) ;
}

static void shl05_02_4_end( PLAYER_WORK *ew );

/*==============================================================*/
static void shl05_02_4_wait( PLAYER_WORK *ew )
{
	shl05_02_master_pos( ew ) ;
	pl_char_move( ew ) ;
	if( pl_lw->panther_go_flag ){
		ew->routine_1 ++ ;
		pl_lw->panther_go_flag = 1 ;
		ew->atck_no = pl05_at_formation_b ;
		HIT_RESET( ew ) ;
		ew->spd.x = ew->disp_dir ? DOT_X( 9.125f ) : -DOT_X( 9.125f ) ;
		pl_char_set( ew, Char_Effect, 23 ) ;
		pl_voice_req( ew, 33 ) ;
		return ;
	}
	if( !ew->master_w->disp_flag ){
		shl05_02_4_end( ew ) ;
		return ;
	}
	if( !ew->master_w->vital_new ){
		shl05_02_4_end( ew ) ;
		return ;
	}
	if( !pl_lw->panther_flag ) shl05_02_4_end( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl05_02_4_fly( PLAYER_WORK *ew )
{
	VEC3 pos ;

	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !scr_in_x_ck( ew ) ){
		shl05_02_4_end( ew ) ;
		return ;
	}
	if( ew->hit_flag || ew->hit_box_no ){
		pos = ew->pos ;
		burst_set( ew, &pos, 1, 0xc9 ) ;
		shl05_02_4_end( ew ) ;
	}
	hit_work_set( ew ) ;
}

/*==============================================================*/
static void shl05_02_4_end( PLAYER_WORK *ew )
{
	pl_lw->panther_flag = 0 ;
	shl05_02_end( ew ) ;
}

static void shl05_02_5_run( PLAYER_WORK *ew );
static void shl05_02_5_go( PLAYER_WORK *ew );
static void shl05_02_carry( PLAYER_WORK *ew );

/*==============================================================*/
static void shl05_02_move5( PLAYER_WORK *ew )
{
	static void ( *shl05_02_5_jmp[] )( PLAYER_WORK *ew ) = {
		shl05_02_5_run, shl05_02_5_go,
	} ;

	shl05_02_5_jmp[ew->routine_1]( ew ) ;
	if( pl_lw->away_flag ) shl05_02_carry( ew ) ;
}

/*==============================================================*/
static void shl05_02_5_run( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	if( !--ew->timer0 ) ew->routine_1 ++ ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl05_02_5_go( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	pl_char_move( ew ) ;
	if( !scr_in_x_ck( ew ) ){
		shl05_02_end( ew ) ;
		return ;
	}
	hit_work_set( ew ) ;
}

/*==============================================================*/
/*			Strider rides on it									*/
/*==============================================================*/
static void shl05_02_carry( PLAYER_WORK *ew )
{
	static Float shl05_02_ride_y[6] = {
		DOT_X( 49.0f ), 75.0f, DOT_X( 40.0f ), DOT_X( 41.0f ), 70.0f, DOT_X( 46.0f ),
	} ;

	ew->master_w->pos.x = ew->pos.x + ( ew->disp_dir ? DOT_X( 16.0f ) : -DOT_X( 16.0f ) ) ;
	ew->master_w->pos.y = ew->pos.y + shl05_02_ride_y[ew->cg_type >> 1] ;
}

/*==============================================================*/
static void shl05_02_move6( PLAYER_WORK *ew )
{
	VEC3 pos ;

	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	if( !scr_in_x_ck( ew ) ){
		shl05_02_end( ew ) ;
		return ;
	}
	if( ew->hit_box_no ){
		pos = ew->pos ;
		pos.y += DOT_Y( 8.0f ) ;
		burst_set( ew, &pos, 1, 0xc9 ) ;
		shl05_02_end( ew ) ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl05_02_end( PLAYER_WORK *ew )
{
	ew->routine_0 = 3 ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl05_02_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl05_02.c" End of File	*/

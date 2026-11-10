/* Amingo's effect 00 (character 15's shot 00): the bite of his sp_bite (an assist). A body with two
   parts (types 1 and 2); the second part flies at the opponent and bites. */
#include "player.h"
#include "pl_char.h"
#include "pl15_def.h"

static void shl15_00_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl15_00_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl15_00_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
		ew->ef_no = 0x1500 ;
		ew->sub_no = pl_w->team_no ;
	}
	return ew ;
}

/*==============================================================*/
/*			A part of the body at pl_w							*/
/*==============================================================*/
static PLAYER_WORK *shl15_00_part_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( pl_w, W_SHL, LIST_BEHIND ) ) ){
		ew->work_func = shl15_00_main ;
		ew->master_w = pl_w->master_w ;
		ew->ef_w = pl_w ;
		ew->type = type ;
		ew->ef_no = 0x1500 ;
		ew->sub_no = pl_w->sub_no ;
	}
	return ew ;
}

static void shl15_00_body_main( PLAYER_WORK *ew );
static void shl15_00_dm_main( PLAYER_WORK *ew );
static void shl15_00_head_main( PLAYER_WORK *ew );

/*==============================================================*/
static void shl15_00_main( PLAYER_WORK *ew )
{
	static void ( *shl15_00_jmp[] )( PLAYER_WORK *ew ) = {
		shl15_00_body_main, shl15_00_dm_main, shl15_00_head_main,
	} ;

	shl15_00_jmp[ew->type]( ew ) ;
}

static void shl15_00_init( PLAYER_WORK *ew );
static void shl15_00_move( PLAYER_WORK *ew );
static void shl15_00_end( PLAYER_WORK *ew );
static void shl15_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl15_00_body_main( PLAYER_WORK *ew )
{
	static void ( *shl15_00_body_main_jmp[] )( PLAYER_WORK *ew ) = {
		shl15_00_init, shl15_00_move, shl15_00_end, shl15_00_free,
	} ;

	shl15_00_body_main_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
/*			The body: it goes at once if the pool is short		*/
/*==============================================================*/
static void shl15_00_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	if( work_rest_cnt < 3 ){
		ew->routine_0 = 2 ;
		return ;
	}
	shl15_00_part_set( ew, 2 ) ;
	shl15_00_part_set( ew, 1 ) ;
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
	ew->atck_no = pl15_at_bite ;
	HIT_RESET( ew ) ;
	ew->pl_no ^= 1 ;
	ew->atck_flag = HIT_ON | HIT_TOUCH | HIT_PL_ONLY ;
	ew->dm_flag = HIT_PL_ONLY ;
	ew->timer0 = 120 ;
	ew->pos = ew->master_w->pos ;
	if( ew->disp_dir ) ew->pos.x += DOT_X( 85.0f ) ;
	else ew->pos.x -= DOT_X( 85.0f ) ;
	ew->prio = 0 ;
	pl_char_set( ew, Char_Effect, 0 ) ;
	shl15_00_move( ew ) ;
}

static void shl15_00_wait( PLAYER_WORK *ew );
static void shl15_00_stay( PLAYER_WORK *ew );
static void shl15_00_back( PLAYER_WORK *ew );
static void shl15_00_over( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl15_00_move( PLAYER_WORK *ew )
{
	static void ( *shl15_00_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl15_00_wait, shl15_00_stay, shl15_00_back, shl15_00_over, shl15_00_over,
	} ;

	shl15_00_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
/*			1 once Amingo has changed place in his team (his	*/
/*			body goes)											*/
/*==============================================================*/
static Sint8 shl15_00_ck( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->team_no != ew->sub_no ){
		ew->routine_1 = 4 ;
		pl_char_set( ew, Char_Effect, 3 ) ;
		return 1 ;
	}
	return 0 ;
}

/*==============================================================*/
static void shl15_00_wait( PLAYER_WORK *ew )
{
	if( !shl15_00_ck( ew ) ){
		pl_char_move( ew ) ;
		ew->prio = 0 ;
		if( ew->cg_type ) ew->routine_1 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void shl15_00_stay( PLAYER_WORK *ew )
{
	if( !shl15_00_ck( ew ) ){
		if( ew->timer0 ) ew->timer0 -- ;
		if( ew->hit_flag ) ew->routine_1 ++ ;
		else if( ew->behind_w->hit_box_no || !ew->timer0 ){
			ew->routine_1 = 4 ;
			pl_char_set( ew, Char_Effect, 3 ) ;
		}
		else hit_work_set( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl15_00_back( PLAYER_WORK *ew )
{
	if( !shl15_00_ck( ew ) ){
		if( pl_char_move( ew ) < 0 ){
			ew->routine_1 ++ ;
			pl_char_set( ew, Char_Effect, 2 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void shl15_00_over( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
/*			The first part: it stays with the body, and its		*/
/*			boxes are the body's hit							*/
/*==============================================================*/
static void shl15_00_dm_main( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	if( !ew->routine_0 ){
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
		ew->atck_flag = 0 ;
		ew->dm_flag = HIT_ON | HIT_OBJ ;
	}
	pl_w = ew->master_w ;
	if( pl_w->team_no != ew->sub_no ){
		shl15_00_free( ew ) ;
		return ;
	}
	ew->pos = ew->ef_w->pos ;
	if( ew->ef_w->hit_flag || ew->hit_box_no || ew->ef_w->routine_1 > 1 || ew->ef_w->routine_0 > 1 ){
		if( ew->hit_box_no ) catch_hitmark( ew ) ;
		shl15_00_free( ew ) ;
		return ;
	}
	CHAR_ANIM_COPY( ew, ew->ef_w ) ;
	ew->frame_box = ew->ef_w->frame_box ;
	if( ew->ef_w->routine_0 == 1 && ew->ef_w->routine_1 == 1 ) hit_work_set( ew ) ;
}

static void shl15_00_head_init( PLAYER_WORK *ew );
static void shl15_00_head_move( PLAYER_WORK *ew );

/*==============================================================*/
static void shl15_00_head_main( PLAYER_WORK *ew )
{
	static void ( *shl15_00_head_main_jmp[] )( PLAYER_WORK *ew ) = {
		shl15_00_head_init, shl15_00_head_move, shl15_00_end, shl15_00_free,
	} ;

	shl15_00_head_main_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
/*			The second part: the biting head					*/
/*==============================================================*/
static void shl15_00_head_init( register PLAYER_WORK *ew )
{
	register PLAYER_WORK *pl_w = ew->master_w ;

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
	ew->atck_no = pl15_at_bite_2 ;
	HIT_RESET( ew ) ;
	ew->pl_no ^= 1 ;
	ew->atck_flag = HIT_ON | HIT_TOUCH | HIT_PL_ONLY ;
	ew->dm_flag = 0 ;
	ew->disp_flag = 0 ;
	ew->prio = 0 ;
	pl_char_set( ew, Char_Effect, 1 ) ;
	if( pl_w->pl_personal ) ef_color_set( ew, 1, 2, 1 ) ;
	else ef_color_set( ew, 1, 0, 1 ) ;
	shl15_00_head_move( ew ) ;
}

static void shl15_00_rise( PLAYER_WORK *ew );
static void shl15_00_fly( PLAYER_WORK *ew );
static void shl15_00_bite( PLAYER_WORK *ew );
static void shl15_00_bite_end( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl15_00_head_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	static void ( *shl15_00_head_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl15_00_rise, shl15_00_fly, shl15_00_bite, shl15_00_bite_end,
	} ;

	if( pl_w->team_no != ew->sub_no ){
		ew->routine_0 = 2 ;
		return ;
	}
	shl15_00_head_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void shl15_00_rise( PLAYER_WORK *ew )
{
	if( ew->ef_w->routine_1 >= 4 ) ew->routine_0 = 2 ;
	else if( ew->ef_w->cg_type ){
		ew->routine_1 ++ ;
		ew->disp_flag = 1 ;
		ew->pos = ew->ef_w->pos ;
		ew->pos.y = ew->ef_w->pos.y + DOT_Y( 43.0f ) ;
	}
	else ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl15_00_fly( PLAYER_WORK *ew )
{
	if( ew->ef_w->routine_1 >= 4 ) ew->routine_0 = 2 ;
	else {
		if( ew->ef_w->hit_flag ){
			if( ew->ef_w->cg_type == 9 ){
				ew->routine_1 ++ ;
				ew->routine_2 = 0 ;
				ew->em_w = ew->ef_w->em_w ;
			}
		}
	}
}

/*--------------------------------------------------------------*/
static void shl15_00_bite( PLAYER_WORK *ew )
{
	PL15_LOCAL_W *lw = ( PL15_LOCAL_W * )ew->master_w->tlocal ;
	PLAYER_WORK *em_w = ew->em_w ;

	if( !ew->routine_2 ){
		ew->routine_2 ++ ;
		ew->disp_flag = 1 ;
		ew->acc.x = 0 ;
		ew->acc.y = 0 ;
		ew->spd.z = em_w->pos.x ;
		ew->spd.x = ( ew->spd.z - ew->pos.x ) / 32.0f ;
		ew->spd.y = DOT_Y( 12.0f ) ;
		ew->acc.y = -( ew->spd.y / 16.0f ) ;
		ew->timer0 = 20 ;
	}
	else {
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		ew->timer0 -- ;
		if( ew->hit_flag ){
			ew->routine_1 ++ ;
			ew->routine_2 = 0 ;
			if( em_w->team_no ){
				ew->routine_1 = 4 ;
				ew->disp_flag = 0 ;
				ew->routine_0 = 2 ;
				return ;
			}
			if( ew->master_w->pl_personal ){
				def_plus_set( em_w, 360, 16 ) ;
				puff_ef_set( em_w, 1 ) ;
				ew->sub_type = 1 ;
				lw->unk_04 ++ ;
				lw->unk_04 &= 1 ;
				Timer_Gauge_Set( em_w, &em_w->def_plus_timer, 1 ) ;
			}
			else {
				vital_add_dmg( em_w, 20 ) ;
				puff_ef_set( em_w, 4 ) ;
				ew->sub_type = 0 ;
			}
			pl_se_req( ew, 42 ) ;
		}
		else if( em_w->ground_y > ew->pos.y ){
			ew->routine_1 ++ ;
			ew->routine_2 = 0 ;
		}
		else if( ew->spd.y <= 0 ) hit_work_set( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl15_00_bite_end( PLAYER_WORK *ew )
{
	if( !ew->routine_2 ){
		ew->routine_2 ++ ;
		pl_char_set( ew, Char_Effect, 16 ) ;
	}
	else {
		if( pl_char_move( ew ) < 0 ){
			ew->routine_0 ++ ;
			ew->disp_flag = 0 ;
		}
	}
}

/*--------------------------------------------------------------*/
static void shl15_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl15_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl15_00.c" End of File	*/

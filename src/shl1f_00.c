/* B.B. Hood's effect 00 (character 1f's shot 00): the things her heavy kicks throw, and the flame
   of Cheer & Fire. The thrown ones, three kinds, each move in their own way and end on a hit; the
   flame stays in front of her while she keeps its animation, and hits again and again. */
#include "player.h"
#include "pl_char.h"
#include "pl1f_def.h"

/* where the spark is placed from B.B. Hood, x then y for each type */
static Float shl1f_00_pos_ofs[2 * 2] = {
	-75.0f, DOT_Y( 88.0f ),
	-100.0f, DOT_Y( 50.0f ),
} ;

static void shl1f_00_main( PLAYER_WORK *ew );
static void shl1f_00_spark_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl1f_00_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl1f_00_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
		ew->ef_no = 0x1f00 ;
	}
	return ew ;
}

/****************************************************************/
PLAYER_WORK *shl1f_00_spark_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 dir )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl1f_00_spark_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
		ew->dir = dir ;
		ew->ef_no = 0x1f00 ;
		ew->obj_flag = pl_w->char_no.w ;
		ew->pl_type = pl_w->pl_type ;
	}
	return ew ;
}

/*==============================================================*/
/*			The spark: it lasts while B.B. Hood keeps the		*/
/*			animation it was made in							*/
/*==============================================================*/
static void shl1f_00_spark_main( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( !ew->routine_0 ){
		ew->routine_0 ++ ;
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
		ew->disp_flag = 1 ;
		ew->atck_flag = HIT_ON | HIT_SHOT ;
		ew->dm_flag = HIT_ON | HIT_SHOT ;
		if( pl_w->team_type == 8 ){
			ew->atck_no = pl1f_at_cheer_assist ;
			HIT_RESET( ew ) ;
			ew->timer1 = 15 ;
			ew->timer0 = 15 ;
		} else {
			if( !ew->type ){
				ew->atck_no = ew->atck_level + 50 ;
				HIT_RESET( ew ) ;
			} else {
				ew->atck_no = ew->atck_level + 72 ;
				HIT_RESET( ew ) ;
			}
			ew->timer1 = 4 ;
			if( !ew->atck_level ) ew->timer0 = 25 ;
			else ew->timer0 = 30 ;
		}
		pl_char_set( ew, Char_Effect, ew->type + 18 ) ;
		pl_se_req( pl_w, 75 ) ;
	}
	ew->pos.x = shl1f_00_pos_ofs[ew->type * 2] ;
	ew->pos.y = shl1f_00_pos_ofs[ew->type * 2 + 1] ;
	if( ew->disp_dir ) ew->pos.x = -ew->pos.x ;
	ew->pos.x += pl_w->pos.x ;
	ew->pos.y += pl_w->pos.y ;
	if( ew->obj_flag != pl_w->char_no.w ){
		work_free( ew ) ;
		return ;
	}
	if( ew->routine_0 == 1 ){
		if( ew->hit_box_no != 0 ){
			ew->routine_0 ++ ;
			pl_char_set( ew, Char_Effect, ew->type + 20 ) ;
			return ;
		}
		if( ew->hit_stop ){
			ew->hit_stop -- ;
			return ;
		}
		if( -- ew->timer1 < 0 ){
			ew->timer1 = 4 ;
			if( !ew->type ){
				ew->atck_no = ew->atck_level + 50 ;
				HIT_RESET( ew ) ;
			} else {
				ew->atck_no = ew->atck_level + 72 ;
				HIT_RESET( ew ) ;
			}
		}
		pl_char_move( ew ) ;
		if( -- ew->timer0 < 0 ){
			ew->timer0 = 0 ;
			ew->routine_0 ++ ;
			pl_char_set( ew, Char_Effect, ew->type + 20 ) ;
		} else hit_work_set( ew ) ;
	} else if( ew->routine_0 == 2 ){
		if( pl_char_move( ew ) < 0 ){
			ew->routine_0 ++ ;
			ew->disp_flag = 0 ;
			PL1F_LW( pl_w )->spark_end_flag = 1 ;
		}
	} else work_free( ew ) ;
}

static void shl1f_00_init( PLAYER_WORK *ew );
static void shl1f_00_move( PLAYER_WORK *ew );
static void shl1f_00_end( PLAYER_WORK *ew );
static void shl1f_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1f_00_main( PLAYER_WORK *ew )
{
	static void ( *shl1f_00_jmp[] )( PLAYER_WORK *ew ) = {
		shl1f_00_init, shl1f_00_move, shl1f_00_end, shl1f_00_free,
	} ;

	shl1f_00_jmp[ew->routine_0]( ew ) ;
}

/* where each kind starts from B.B. Hood, and its speed and acceleration, x then y */
static const Float shl1f_00_start[] = {
	-0.0f, DOT_Y( 32.0f ),
	-85.0f, DOT_Y( 37.0f ),
	-DOT_X( 16.0f ), DOT_Y( 39.0f ),
} ;
static const Float shl1f_00_spd[] = {
	0.0f, 0.0f, -DOT_Y( 2.0f ), -2.142857f,
	-5.0f, 0.052083332f, DOT_Y( 1.5f ), -DOT_Y( 0.25f ),
	-DOT_X( 16.0f ), -DOT_X( 2.0f ), DOT_Y( 39.0f ), -2.142857f,
} ;

/*==============================================================*/
static void shl1f_00_init( PLAYER_WORK *ew )
{
	static Uint8 shl1f_00_atck_no[] = { 5, 20, 17 } ;

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
	ew->disp_flag = 1 ;
	ew->sub_prio = -8 ;
	ew->dir = 0 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->pos.x = shl1f_00_start[ew->type * 2] ;
	ew->pos.y = shl1f_00_start[ew->type * 2 + 1] ;
	ew->spd.x = shl1f_00_spd[ew->type * 4] ;
	ew->acc.x = shl1f_00_spd[ew->type * 4 + 1] ;
	ew->spd.y = shl1f_00_spd[ew->type * 4 + 2] ;
	ew->acc.y = shl1f_00_spd[ew->type * 4 + 3] ;
	if( ew->disp_dir ){
		ew->pos.x = -ew->pos.x ;
		ew->spd.x = -ew->spd.x ;
		ew->acc.x = -ew->acc.x ;
	}
	ew->pos.x += ew->master_w->pos.x ;
	ew->pos.y += ew->master_w->pos.y ;
	ew->atck_no = shl1f_00_atck_no[ew->type] ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, Char_Effect, ew->type + 1 ) ;
	shl1f_00_move( ew ) ;
}

static void shl1f_00_move_00( PLAYER_WORK *ew );
static void shl1f_00_move_01( PLAYER_WORK *ew );
static void shl1f_00_move_02( PLAYER_WORK *ew );

/*==============================================================*/
/*			It flies by its kind, flashes out once B.B. Hood	*/
/*			has left the throw, and ends on a hit				*/
/*==============================================================*/
static void shl1f_00_move( PLAYER_WORK *ew )
{
	static void ( *shl1f_00_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl1f_00_move_00, shl1f_00_move_01, shl1f_00_move_02,
	} ;

	ew->prio = ew->master_w->prio ;
	if( !ew->routine_1 ){
		if( ew->hit_flag ){
			ew->routine_1 ++ ;
			pl_char_set( ew, Char_Effect, 10 ) ;
			return ;
		}
		if( ew->hit_box_no != 0 ){
			ew->routine_0 ++ ;
			ew->disp_flag = 0 ;
			return ;
		}
		shl1f_00_move_jmp[ew->type]( ew ) ;
		if( ew->dir ){
			ew->disp_flag = ew->dir & 1 ;
			if( !-- ew->dir ) ew->routine_0 ++ ;
		} else hit_work_set( ew ) ;
	} else {
		if( pl_char_move( ew ) < 0 ) ew->routine_0 ++ ;
	}
}

static void shl1f_00_move_00_fall( PLAYER_WORK *ew );
static void shl1f_00_move_00_wait( PLAYER_WORK *ew );
static void shl1f_00_move_00_jump( PLAYER_WORK *ew );
static void shl1f_00_move_00_bound( PLAYER_WORK *ew );
static void shl1f_00_move_00_slide( PLAYER_WORK *ew );
static void shl1f_00_move_hit_end( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1f_00_move_00( PLAYER_WORK *ew )
{
	static void ( *shl1f_00_move_00_jmp[] )( PLAYER_WORK *ew ) = {
		shl1f_00_move_00_fall, shl1f_00_move_00_wait, shl1f_00_move_00_jump, shl1f_00_move_00_bound,
		shl1f_00_move_00_slide, shl1f_00_move_hit_end,
	} ;

	shl1f_00_move_00_jmp[ew->routine_2]( ew ) ;
}

/*==============================================================*/
static void shl1f_00_move_00_fall( PLAYER_WORK *ew )
{
	if( ew->master_w->act_st != N_NML_ATCK ) ew->dir = 1 ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y < ew->master_w->ground_y ){
		ew->routine_2 ++ ;
		ew->pos.y = ew->master_w->ground_y ;
	}
}

/*--------------------------------------------------------------*/
static void shl1f_00_move_00_wait( PLAYER_WORK *ew )
{
	if( ew->cg_type ){
		ew->routine_2 ++ ;
		ew->cg_type = 0 ;
		ew->spd.x = -5.0f ;
		ew->acc.x = 0.052083332f ;
		ew->spd.y = DOT_Y( 1.5f ) ;
		ew->acc.y = -DOT_Y( 0.25f ) ;
		if( ew->disp_dir ){
			ew->spd.x = -ew->spd.x ;
			ew->acc.x = -ew->acc.x ;
		}
	}
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1f_00_move_00_jump( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y < ew->master_w->ground_y ){
		ew->routine_2 ++ ;
		ew->pos.y = ew->master_w->ground_y ;
		ew->spd.y = DOT_Y( 1.5f ) ;
		ew->acc.y = -DOT_Y( 0.25f ) ;
	}
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1f_00_move_00_bound( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y < ew->master_w->ground_y ){
		ew->routine_2 ++ ;
		ew->pos.y = ew->master_w->ground_y ;
	}
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1f_00_move_00_slide( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	if( ew->cg_type ){
		ew->routine_2 ++ ;
		ew->cg_type = 0 ;
	}
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1f_00_move_hit_end( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		pl_char_set( ew, Char_Effect, 10 ) ;
	}
}

static void shl1f_00_move_01_wait( PLAYER_WORK *ew );
static void shl1f_00_move_01_fly( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1f_00_move_01( PLAYER_WORK *ew )
{
	static void ( *shl1f_00_move_01_jmp[] )( PLAYER_WORK *ew ) = {
		shl1f_00_move_01_wait, shl1f_00_move_01_fly, shl1f_00_move_hit_end,
	} ;

	shl1f_00_move_01_jmp[ew->routine_2]( ew ) ;
}

/*==============================================================*/
static void shl1f_00_move_01_wait( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	if( ew->cg_type < 0 ){
		if( ew->master_w->act_st != N_NML_ATCK ) ew->dir = 1 ;
	}
	if( !ew->cg_type ) ew->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void shl1f_00_move_01_fly( PLAYER_WORK *ew )
{
	if( !ew->cg_type ) pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y < ew->master_w->ground_y ){
		ew->routine_2 ++ ;
		ew->pos.y = ew->master_w->ground_y ;
		pl_voice_req( ew->master_w, 32 ) ;
	}
}

static void shl1f_00_move_02_0( PLAYER_WORK *ew );
static void shl1f_00_move_02_1( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1f_00_move_02( PLAYER_WORK *ew )
{
	static void ( *shl1f_00_move_02_jmp[] )( PLAYER_WORK *ew ) = {
		shl1f_00_move_02_0, shl1f_00_move_02_1,
	} ;

	shl1f_00_move_02_jmp[ew->routine_2]( ew ) ;
}

/*==============================================================*/
static void shl1f_00_move_02_0( PLAYER_WORK *ew )
{
	if( ew->master_w->act_st != N_NML_ATCK ) ew->dir = 1 ;
	if( ew->cg_type ){
		ew->routine_2 ++ ;
		ew->acc.x *= 4.0f ;
		ew->acc.y *= 4.0f ;
	}
	ew->spd.x += ew->acc.x ;
	ew->spd.y += ew->acc.y ;
	ew->pos.x = ew->master_w->pos.x + ew->spd.x ;
	ew->pos.y = ew->master_w->pos.y + ew->spd.y ;
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1f_00_move_02_1( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		pl_char_set( ew, Char_Effect, 10 ) ;
		return ;
	}
	ew->spd.x += ew->acc.x ;
	ew->spd.y += ew->acc.y ;
	ew->pos.x = ew->master_w->pos.x + ew->spd.x ;
	ew->pos.y = ew->master_w->pos.y + ew->spd.y ;
}

/*--------------------------------------------------------------*/
static void shl1f_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl1f_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl1f_00.c" End of File	*/

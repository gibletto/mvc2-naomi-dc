/* B.B. Hood's effect 01 (character 1f's shot 01): the missile of Smile & Missile, six kinds: two
   strengths each of the standing one, the low one and the one fired from a jump. It flies until it
   hits, its time is up or it reaches the ground. */
#include "player.h"
#include "pl_char.h"
#include "pl1f_def.h"

static void shl1f_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl1f_01_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl1f_01_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
		ew->ef_no = 0x1f01 ;
		ew->obj_flag = pl_w->char_no.b.grp ;
		PL1F_LW( pl_w )->shot_flag = 1 ;
	}
	return ew ;
}

static void shl1f_01_init( PLAYER_WORK *ew );
static void shl1f_01_move( PLAYER_WORK *ew );
static void shl1f_01_end( PLAYER_WORK *ew );
static void shl1f_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1f_01_main( PLAYER_WORK *ew )
{
	static void ( *shl1f_01_jmp[] )( PLAYER_WORK *ew ) = {
		shl1f_01_init, shl1f_01_move, shl1f_01_end, shl1f_01_free,
	} ;

	shl1f_01_jmp[ew->routine_0]( ew ) ;
}

/* where each pair of kinds starts from B.B. Hood, x then y */
static const Float shl1f_01_start[] = {
	-50.0f, DOT_Y( 67.0f ),
	-DOT_X( 50.0f ), DOT_Y( 22.0f ),
	-DOT_X( 32.0f ), DOT_Y( 72.0f ),
} ;
/* the speed of each kind, x then y */
static const Float shl1f_01_spd[] = {
	-5.0f, 0.0f,
	-DOT_X( 5.0f ), 0.0f,
	-5.0f, 0.0f,
	-DOT_X( 5.0f ), 0.0f,
	-7.5f, -DOT_Y( 4.0f ),
	-12.5f, -DOT_Y( 4.0f ),
} ;

/*==============================================================*/
static void shl1f_01_init( PLAYER_WORK *ew )
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
	ew->disp_flag = 1 ;
	ew->sub_prio = -8 ;
	ew->timer0 = 96 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->pos.x = shl1f_01_start[ew->type & 6] ;
	ew->pos.y = shl1f_01_start[( ew->type & 6 ) + 1] ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.x = shl1f_01_spd[ew->type * 2] ;
	ew->spd.y = shl1f_01_spd[ew->type * 2 + 1] ;
	if( ew->disp_dir ){
		ew->pos.x = -ew->pos.x ;
		ew->spd.x = -ew->spd.x ;
		ew->acc.x = -ew->acc.x ;
	}
	ew->pos.x += ew->master_w->pos.x ;
	ew->pos.y += ew->master_w->pos.y ;
	ew->atck_no = ew->atck_level + 48 ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, Char_Effect, ew->type + 4 ) ;
	pls1f_00_set2( ew, 2, 0 ) ;
	shl1f_01_move( ew ) ;
}

/*==============================================================*/
/*			It ends on a hit, when its time is up, when B.B.	*/
/*			Hood starts a hyper combo, or on the ground			*/
/*==============================================================*/
static void shl1f_01_hit_end( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	PL1F_LW( pl_w )->shot_flag = 0 ;
	ew->routine_1 ++ ;
	pl_char_set( ew, Char_Effect, 10 ) ;
}

/*--------------------------------------------------------------*/
static void shl1f_01_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->prio = pl_w->prio ;
	if( !ew->routine_1 ){
		if( !scr_in_x_ck( ew ) ){
			ew->routine_0 = 2 ;
			PL1F_LW( pl_w )->shot_flag = 0 ;
			return ;
		}
		if( ew->hit_flag || !-- ew->timer0 || ew->hit_box_no || pl_w->char_no.b.grp == Char_Super ){
			shl1f_01_hit_end( ew ) ;
			return ;
		}
		if( ew->pos.y < ew->master_w->ground_y ){
			ew->pos.y = ew->master_w->ground_y ;
			shl1f_01_hit_end( ew ) ;
			return ;
		}
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		if( ew->cg_type ){
			ew->cg_type = 0 ;
			pls1f_00_set2( ew, 3, ew->type >> 1 ) ;
		}
		pl_char_move( ew ) ;
		hit_work_set( ew ) ;
	} else {
		if( pl_char_move( ew ) < 0 ) ew->routine_0 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void shl1f_01_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl1f_01_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl1f_01.c" End of File	*/

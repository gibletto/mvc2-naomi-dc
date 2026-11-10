/* Guile's effect 06 (character 02's shot 06): the crescent of a Somersault Kick, which flies up and
   forward from him until it hits or comes back down to his ground. */
#include "player.h"
#include "pl_char.h"
#include "pl02_def.h"

static void shl02_06_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl02_06_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl02_06_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->ef_no = 0x206 ;
	}
	return ew ;
}

static void shl02_06_init( PLAYER_WORK *ew );
static void shl02_06_move( PLAYER_WORK *ew );
static void shl02_06_end( PLAYER_WORK *ew );
static void shl02_06_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl02_06_main( PLAYER_WORK *ew )
{
	static void ( *shl02_06_jmp[] )( PLAYER_WORK *ew ) = {
		shl02_06_init, shl02_06_move, shl02_06_end, shl02_06_free,
	} ;

	shl02_06_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl02_06_init( PLAYER_WORK *ew )
{
	/* the speed, x and y, by strength */
	static struct { Float x, y ; } shl02_06_spd[] = {
		{ DOT_X( 4.0f ), DOT_Y( 4.5f ) }, { DOT_X( 8.0f ), DOT_Y( 8.5f ) },
	} ;

	PLAYER_WORK *pl_w = ew->master_w ;

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
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	if( ew->master_w->pl_dir ) ew->pos.x += 160.0f ;
	else ew->pos.x -= 160.0f ;
	ew->pos.y += DOT_Y( 64.0f ) ;
	ew->size_front = 64 ;
	ew->size_back = 64 ;
	ew->prio = 0 ;
	if( pl_w->team_type == 3 ){
		ew->atck_no = pl02_at_vc_smr_crescent ;
		HIT_RESET( ew ) ;
		pl_char_set( ew, 20, 2 ) ;
	} else if( ew->type == 1 ){
		ew->atck_no = pl02_at_strike_crescent ;
		HIT_RESET( ew ) ;
		pl_char_set( ew, 20, 3 ) ;
	} else if( ew->type == 2 ){
		ew->atck_no = pl02_at_crossfire_crescent ;
		HIT_RESET( ew ) ;
		pl_char_set( ew, 20, 2 ) ;
	} else {
		ew->atck_no = ew->atck_level ? 57 : 56 ;
		HIT_RESET( ew ) ;
		pl_char_set( ew, 20, 2 ) ;
	}
	if( ew->disp_dir ) ew->spd.x = shl02_06_spd[ew->atck_level].x ;
	else ew->spd.x = -shl02_06_spd[ew->atck_level].x ;
	ew->spd.y = shl02_06_spd[ew->atck_level].y ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	shl02_06_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl02_06_move( PLAYER_WORK *ew )
{
	if( ew->hit_box_no || !scr_in_x_ck( ew ) ){
		shl02_06_end( ew ) ;
		return ;
	}
	ew->prio = 0 ;
	if( ew->pos.y <= ew->master_w->ground_y ){
		shl02_06_end( ew ) ;
		return ;
	}
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	hit_work_set( ew ) ;
}

/*==============================================================*/
/*			The rest of the animation							*/
/*==============================================================*/
static void shl02_06_end( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ){
		ew->disp_flag = 0 ;
		ew->routine_0 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void shl02_06_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl02_06.c" End of File	*/

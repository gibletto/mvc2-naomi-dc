/* Guile's effect 04 (character 02's shot 04): a shot that slides out in front of him, slowing to
   a stop, and hits every twelve frames until its time is up. */
#include "player.h"
#include "pl_char.h"
#include "pl02_def.h"

static void shl02_04_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl02_04_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl02_04_main ;
		ew->master_w = pl_w ;
	}
	return ew ;
}

static void shl02_04_init( PLAYER_WORK *ew );
static void shl02_04_move( PLAYER_WORK *ew );
static void shl02_04_end( PLAYER_WORK *ew );
static void shl02_04_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl02_04_main( PLAYER_WORK *ew )
{
	static void ( *shl02_04_jmp[] )( PLAYER_WORK *ew ) = {
		shl02_04_init, shl02_04_move, shl02_04_end, shl02_04_free,
	} ;

	shl02_04_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl02_04_init( PLAYER_WORK *ew )
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
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->spd.x = DOT_X( 8.0f ) ;
	ew->acc.x = -DOT_X( 0.125f ) ;
	ew->timer0 = 92 ;
	ew->timer1 = 0 ;
	ew->pos.x = ew->master_w->pos.x ;
	if( !ew->disp_dir ){
		ew->spd.x = -ew->spd.x ;
		ew->acc.x = -ew->acc.x ;
	}
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.y += 120.0f ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->size_front = 64 ;
	ew->size_back = 64 ;
	ew->prio = 10 ;
	pl_char_set( ew, Char_Effect, 5 ) ;
	shl02_04_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl02_04_move( PLAYER_WORK *ew )
{
	if( ew->hit_box_no || ew->timer0 <= 0 ){
		shl02_04_end( ew ) ;
		return ;
	}
	if( !scr_in_x_ck( ew ) ){
		shl02_04_end( ew ) ;
		return ;
	}
	ew->spd.x += ew->acc.x ;
	ew->pos.x += ew->spd.x ;
	if( ew->disp_dir && ew->spd.x <= 0.0f ){
		ew->spd.x = 0.0f ;
		ew->acc.x = 0.0f ;
	}
	if( !ew->disp_dir && ew->spd.x >= 0.0f ){
		ew->spd.x = 0.0f ;
		ew->acc.x = 0.0f ;
	}
	ew->timer1 ++ ;
	if( ew->timer1 == 12 ){
		ew->atck_no = pl02_at_hurricane ;
		HIT_RESET( ew ) ;
		ew->timer1 = 0 ;
	}
	ew->timer0 -- ;
	hit_work_set( ew ) ;
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl02_04_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl02_04_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl02_04.c" End of File	*/

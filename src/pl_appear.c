/* The work that brings a fighter back into the fight after a KO: it follows him for a second, then
   starts his character's own entrance, or the common one, and blinks while that runs. */
#include "player.h"

static void pl_appear_main( PLAYER_WORK *ew );

/****************************************************************/
void pl_appear_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS_ST, LIST_TOP ) ) ){
		ew->work_func = pl_appear_main ;
		ew->master_w = pl_w ;
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
		ew->pos = pl_w->pos ;
		ew->pos.y = pl_w->ground_y ;
	}
}

static void pl_appear_init( PLAYER_WORK *ew );
static void pl_appear_move( PLAYER_WORK *ew );
static void pl_appear_end( PLAYER_WORK *ew );
static void pl_appear_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pl_appear_main( PLAYER_WORK *ew )
{
	static void ( *pl_appear_jmp[] )( PLAYER_WORK *ew ) = {
		pl_appear_init, pl_appear_move, pl_appear_end, pl_appear_free,
	} ;

	pl_appear_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pl_appear_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	BODY_SIZE_SET( ew, 0x80204040 ) ;
	ew->prio = 15 ;
	ew->timer0 = 60 ;
}

/*--------------------------------------------------------------*/
static void pl_appear_move( PLAYER_WORK *ew )
{
	VEC3 pos ;

	if( !ew->routine_1 ){
		if( -- ew->timer0 ){
			ew->disp_flag = scr_in_ck( ew ) ;
			return ;
		}
		ew->routine_1 ++ ;
		ew->timer0 = 60 ;
		if( ew->master_w->pl_type == PL_STRIDER ){
			pls05_01_set( ew->master_w ) ;
			ew->routine_0 ++ ;
			return ;
		}
		if( ew->master_w->pl_type == PL_MEGAMAN || ew->master_w->pl_type == PL_ROLL ){
			pls1c_06_start( ew->master_w ) ;
			ew->routine_0 ++ ;
			return ;
		}
		if( ew->master_w->armor_flag & ARMOR_HEAVY ){
			pos = ew->master_w->pos ;
			pos.y = ew->master_w->ground_y ;
			burst_set( ew->master_w, &pos, 1, 0x179 ) ;
			scr_work.quake_level = 3 ;
			scr_work.quake_flag = 1 ;
			ew->routine_0 ++ ;
			return ;
		}
		if( ew->master_w->pl_type == PL_SERVBOT ){
			pls3a_02_set( ew->master_w, 0 ) ;
			ew->routine_0 ++ ;
		}
	} else {
		if( !( ew->disp_flag = scr_in_ck( ew ) ) ){
			ew->routine_0 ++ ;
			return ;
		}
		if( !-- ew->timer0 ){
			ew->routine_0 ++ ;
			return ;
		}
		ew->disp_flag = ew->timer0 & 1 ;
	}
}

/*--------------------------------------------------------------*/
static void pl_appear_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pl_appear_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pl_appear.c" End of File	*/

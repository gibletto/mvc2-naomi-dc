/* Abyss's second form's hit mark (effect 01): it rides at the player's side while the player's animation
   runs, swells as it strikes (type picks the attack) and dies out with its own animation. */
#include "player.h"
#include "pl_char.h"
#include "pl19_def.h"

static void shl19_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl19_01_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl19_01_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
	}
	return ew ;
}

static void shl19_01_init( PLAYER_WORK *ew );
static void shl19_01_move( PLAYER_WORK *ew );
static void shl19_01_end( PLAYER_WORK *ew );
static void shl19_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl19_01_main( PLAYER_WORK *ew )
{
	static void ( *shl19_01_jmp[] )( PLAYER_WORK *ew ) = {
		shl19_01_init, shl19_01_move, shl19_01_end, shl19_01_free,
	} ;

	shl19_01_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl19_01_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->routine_0 ++ ;
	ew->ef_no = 0x1901 ;
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
	ew->prio = 9 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = ew->type + 55 ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, Char_Effect, 0 ) ;
	shl19_01_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl19_01_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->pos.x = pl_w->pos.x + ( !pl_w->disp_dir ? -DOT_X( 58.0f ) : DOT_X( 58.0f ) ) ;
	ew->pos.y = pl_w->pos.y + 180.0f ;
	if( !pl_w->cg_type ){
		ew->routine_0 ++ ;
		ew->scale.x = 1.0f ;
		pl_char_set( ew, Char_Effect, 1 ) ;
		return ;
	}
	pl_char_move( ew ) ;
	if( ew->cg_type ){
		if( ew->type ) ew->scale.x = 2.0f ;
		else ew->scale.x = 1.0f ;
	}
	if( ew->type && ew->cg_prm ){
		ew->cg_prm = 0 ;
		ew->atck_no = pl19_at_blast_h ;
		HIT_RESET( ew ) ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl19_01_end( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl19_01_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl19_01.c" End of File	*/

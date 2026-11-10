/* B.B. Hood's effect 02 (character 1f's shot 02): the four things that fly out beside her in her
   hyper combo, from where they start to off the screen. */
#include "player.h"
#include "pl_char.h"
#include "pl1f_def.h"

static void shl1f_02_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl1f_02_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl1f_02_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
		ew->ef_no = 0x1f02 ;
	}
	return ew ;
}

static void shl1f_02_init( PLAYER_WORK *ew );
static void shl1f_02_move( PLAYER_WORK *ew );
static void shl1f_02_end( PLAYER_WORK *ew );
static void shl1f_02_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1f_02_main( PLAYER_WORK *ew )
{
	static void ( *shl1f_02_jmp[] )( PLAYER_WORK *ew ) = {
		shl1f_02_init, shl1f_02_move, shl1f_02_end, shl1f_02_free,
	} ;

	shl1f_02_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl1f_02_init( PLAYER_WORK *ew )
{
	static Uint8 shl1f_02_atck_no[] = { 54, 55, 56, 68 } ;

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
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.x = DOT_X( 16.0f ) ;
	if( !ew->type ){
		ew->pos.x = DOT_X( 128.0f ) ;
		ew->pos.y = DOT_Y( 118.0f ) ;
	} else if( ew->type == 1 ){
		ew->pos.x = DOT_X( 154.0f ) ;
		ew->pos.y = DOT_Y( 113.0f ) ;
	} else {
		ew->pos.x = DOT_X( 56.0f ) ;
		ew->pos.y = DOT_Y( 62.0f ) ;
		ew->spd.x = DOT_X( 32.0f ) ;
	}
	if( !ew->disp_dir ){
		ew->pos.x = -ew->pos.x ;
		ew->spd.x = -ew->spd.x ;
	}
	ew->pos.x += ew->master_w->pos.x ;
	ew->pos.y += ew->master_w->pos.y ;
	ew->atck_no = shl1f_02_atck_no[ew->type] ;
	HIT_RESET( ew ) ;
	if( ew->type >= 2 ) pl_char_set( ew, Char_Effect, 16 ) ;
	else pl_char_set( ew, Char_Effect, ew->type + 11 ) ;
	shl1f_02_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1f_02_move( PLAYER_WORK *ew )
{
	if( vs_sys_work.slow_bit & ( 1 << vs_sys_work.slow_step ) ) return ;
	ew->prio = ew->master_w->prio ;
	if( ew->routine_1 ) return ;
	if( !scr_in_x_ck( ew ) ){
		ew->routine_0 = 2 ;
		return ;
	}
	if( ew->hit_flag ){
		ew->routine_0 ++ ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1f_02_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl1f_02_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl1f_02.c" End of File	*/

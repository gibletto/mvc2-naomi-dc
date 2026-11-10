/* M. Bison's effect 01 (character 26's shot 01): two works that ride beside the Knee Press
   Nightmare (sp_nightmare), one on each side, and strike while he flies, falls and slides. */
#include "player.h"
#include "pl_char.h"
#include "pl26_def.h"

/* where each work is from M. Bison, by type */
typedef struct SHL26_01_POS {
	Float	x, y;
} SHL26_01_POS;

static PL26_LOCAL_W *pl_lw;	/* M. Bison's */

static void shl26_01_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl26_01_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	Uint8 i ;

	for( i = 0 ; i < 2 ; i ++ ){
		if( ( ew = work_get( 0, W_SHL_ST, LIST_TOP ) ) ){
			ew->work_func = shl26_01_main ;
			ew->master_w = pl_w ;
			ew->type = i ;
			ew->ef_no = 0x2601 ;
		}
	}
	return ew ;
}

static void shl26_01_init( PLAYER_WORK *ew );
static void shl26_01_move( PLAYER_WORK *ew );
static void shl26_01_end( PLAYER_WORK *ew );
static void shl26_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl26_01_main( PLAYER_WORK *ew )
{
	static void ( *shl26_01_jmp[] )( PLAYER_WORK *ew ) = {
		shl26_01_init, shl26_01_move, shl26_01_end, shl26_01_free,
	} ;

	pl_lw = ( PL26_LOCAL_W * )&ew->master_w->tlocal[0] ;
	shl26_01_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl26_01_init( PLAYER_WORK *ew )
{
	static SHL26_01_POS shl26_01_pos[] = {
		{ DOT_X( 32.0f ), DOT_Y( 64.0f ) }, { DOT_X( 32.0f ), 0.0f },
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
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->disp_dir = ew->master_w->disp_dir ;
	ew->prio = 0 ;
	ew->pos.x += ew->disp_dir ? shl26_01_pos[ew->type].x : -shl26_01_pos[ew->type].x ;
	ew->pos.y += shl26_01_pos[ew->type].y ;
	ew->timer0 = ew->type ? 4 : 1 ;
	ew->hit_stop = 0 ;
	ew->hit_flag = 0 ;
	pl_char_set( ew, 21, 17 ) ;
}

static void shl26_01_start( PLAYER_WORK *ew );
static void shl26_01_wait( PLAYER_WORK *ew );
static void shl26_01_launch( PLAYER_WORK *ew );
static void shl26_01_fly( PLAYER_WORK *ew );
static void shl26_01_fall( PLAYER_WORK *ew );
static void shl26_01_land( PLAYER_WORK *ew );
static void shl26_01_slide( PLAYER_WORK *ew );

/*==============================================================*/
/*			Stands still during a hit stop or a stop of the		*/
/*			game; ends when M. Bison's state changes or the		*/
/*			work is hit											*/
/*==============================================================*/
static void shl26_01_move( PLAYER_WORK *ew )
{
	static void ( *shl26_01_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl26_01_start, shl26_01_wait, shl26_01_launch, shl26_01_fly,
		shl26_01_fall, shl26_01_land, shl26_01_slide,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( ew->routine_1 >= 1 ){
		if( ( 1 << ( ew->pl_no ^ 1 ) ) & vs_sys_work.game_stop ) return ;
		if( vs_sys_work.game_stop & ( 1 << ew->pl_no ) ) return ;
		if( pl_w->stop_move_flag ) return ;
	}
	ew->prio = 0 ;
	if( ew->master_w->routine_1 || ew->hit_box_no ){
		ew->routine_0 ++ ;
		ew->pos.y += DOT_Y( 48.0f ) ;
		pl_char_set( ew, Char_Effect, 4 ) ;
		return ;
	}
	ew->hit_stop = ew->master_w->hit_stop ;
	if( ew->hit_stop ){
		if( -- ew->hit_stop < 0 ) ew->hit_stop = 0 ;
		return ;
	}
	shl26_01_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void shl26_01_start( PLAYER_WORK *ew )
{
	if( ew->cg_type ) ew->cg_type = 0 ;
	ew->prio = 0 ;
	if( pl_char_move( ew ) < 0 ) ew->routine_1 ++ ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl26_01_wait( PLAYER_WORK *ew )
{
	if( !pl_lw->start_flag ) return ;
	if( ! -- ew->timer0 ){
		ew->routine_1 ++ ;
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		ew->atck_no = pl26_at_nightmare_side ;
		HIT_RESET( ew ) ;
		pl_char_set( ew, 21, 18 ) ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl26_01_launch( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	if( HI_BYTE( ew->cg_rival ) ){
		ew->routine_1 ++ ;
		ew->spd.x = ew->disp_dir ? DOT_X( 8.0f ) : -DOT_X( 8.0f ) ;
		ew->acc.x = ew->disp_dir ? -0.3125f : 0.3125f ;
		ew->spd.y = DOT_Y( 3.0f ) ;
		ew->acc.y = -DOT_Y( 0.375f ) ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl26_01_fly( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	if( ew->cg_type ){
		ew->cg_type = 0 ;
		ew->atck_no = pl26_at_nightmare_side ;
		HIT_RESET( ew ) ;
		ew->timer1 = 2 ;
	}
	if( ! -- ew->timer1 && ew->hit_flag ){
		ew->atck_no = pl26_at_nightmare_side ;
		HIT_RESET( ew ) ;
	}
	if( ew->pos.y <= ew->master_w->ground_y ){
		ew->routine_1 ++ ;
		ew->pos.y = ew->master_w->ground_y ;
		ew->atck_no = pl26_at_nightmare_side ;
		HIT_RESET( ew ) ;
		ew->spd.x = ew->disp_dir ? DOT_X( 13.0f ) : -DOT_X( 13.0f ) ;
		ew->acc.x = ew->disp_dir ? -0.3125f : 0.3125f ;
		ew->spd.y = DOT_Y( 3.0f ) ;
		ew->acc.y = -DOT_Y( 0.375f ) ;
		pl_char_set( ew, 21, 19 ) ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl26_01_fall( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	if( ew->cg_type ){
		ew->cg_type = 0 ;
		ew->atck_no = pl26_at_nightmare_side ;
		HIT_RESET( ew ) ;
		ew->timer1 = 2 ;
	}
	if( ! -- ew->timer1 && ew->hit_flag ){
		ew->atck_no = pl26_at_nightmare_side ;
		HIT_RESET( ew ) ;
	}
	if( ew->pos.y <= ew->master_w->ground_y ){
		ew->routine_1 ++ ;
		ew->pos.y = ew->master_w->ground_y ;
		pl_char_set( ew, 21, 20 ) ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl26_01_land( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	if( HI_BYTE( ew->cg_rival ) ){
		ew->routine_1 ++ ;
		HI_BYTE( ew->cg_rival ) = 0 ;
		ew->atck_no = pl26_at_nightmare_side ;
		HIT_RESET( ew ) ;
		ew->timer1 = 2 ;
		ew->spd.x = ew->disp_dir ? DOT_X( 14.0f ) : -DOT_X( 14.0f ) ;
		ew->acc.x = ew->disp_dir ? -DOT_X( 0.5f ) : DOT_X( 0.5f ) ;
		ew->spd.y = 0.0f ;
		ew->acc.y = 0.0f ;
		pl_se_req( ew, 41 ) ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl26_01_slide( PLAYER_WORK *ew )
{
	if( HI_BYTE( ew->cg_rival ) == 2 ){
		HI_BYTE( ew->cg_rival ) = 0 ;
		ew->spd.x = 0.0f ;
		ew->acc.x = 0.0f ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( pl_char_move( ew ) < 0 ){
		shl26_01_free( ew ) ;
		return ;
	}
	if( ! -- ew->timer1 && ew->hit_flag ){
		ew->atck_no = pl26_at_nightmare_crush ;
		HIT_RESET( ew ) ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl26_01_end( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) >= 0 ) hit_work_set( ew ) ;
	else shl26_01_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl26_01_free( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl26_01.c" End of File	*/

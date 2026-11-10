/* Wolverine's effect 02 (character 07's shot 02): a slash that hits while he holds his move, with
   four parts that ride on it and hit on the frames its animation marks. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl07_def.h"

static void shl07_02_main( PLAYER_WORK *ew );

/****************************************************************/
int shl07_02_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	int i ;

	if( work_rest_cnt <= 2 ) return 0 ;
	for( i = 0 ; i < 2 ; i ++ ){
		ew = work_get( 0, W_SHL, LIST_END ) ;
		ew->ef_no = 0x702 ;
		ew->type = 0 ;
		ew->sub_type = i ;
		ew->work_func = shl07_02_main ;
		ew->master_w = pl_w ;
	}
	return 1 ;
}

static void shl07_02_body( PLAYER_WORK *ew );
static void shl07_02_part( PLAYER_WORK *ew );

/*==============================================================*/
static void shl07_02_main( PLAYER_WORK *ew )
{
	static void ( *shl07_02_jmp[] )( PLAYER_WORK *ew ) = {
		shl07_02_body, shl07_02_part,
	} ;

	shl07_02_jmp[ew->type]( ew ) ;
}

static void shl07_02_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl07_02_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl07_02_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl07_02_part_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl07_02_body( PLAYER_WORK *ew )
{
	static void ( *shl07_02_body_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl07_02_init, shl07_02_move, shl07_02_free,
	} ;

	shl07_02_body_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void shl07_02_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
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
	ew->timer0 = 4 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->prio = ew->sub_type ? 12 : 11 ;
	ew->disp_dir = ew->sub_type ? 0 : 1 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y + DOT_Y( 40.0f ) ;
	ew->atck_no = pl07_at_fatal ;
	HIT_RESET( ew ) ;
	shl07_02_part_set( ew, pl_w ) ;
	pl_char_set( ew, Char_Effect, ew->type + 14 ) ;
}

/*--------------------------------------------------------------*/
static void shl07_02_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( vs_sys_work.slow_bit & 1 << vs_sys_work.slow_step ) return ;
	if( !pl_w->routine_1 && pl_w->act_st == N_SUPER ){
		ew->cg_type = 0 ;
		if( pl_char_move( ew ) >= 0 ){
			if( ew->cg_flag1 ){
				ew->atck_no = ew->cg_flag1 ;
				HIT_RESET( ew ) ;
				ew->cg_flag1 = 0 ;
			}
			goto hit ;
		}
		if( --ew->timer0 ) goto hit ;
	}
	ew->routine_0 = 2 ;
	ew->disp_flag = 0 ;
	pl_w->timer1 = 1 ;
	return ;
hit:
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl07_02_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_color_set( pl_w, PC_DEFAULT ) ;
	work_free( ew ) ;
}

/*==============================================================*/
/*			The four parts, each a work on the slash's			*/
/*			(ef_w)												*/
/*==============================================================*/
static void shl07_02_part_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *pw ;
	int i ;

	for( i = 0 ; i < 4 ; i ++ ){
		if( ( pw = work_get( ew, W_SHL, LIST_BEHIND ) ) ){
			pw->ef_no = 0x702 ;
			pw->type = 1 ;
			pw->sub_type = ew->sub_type ;
			pw->timer0 = i ;
			pw->work_func = shl07_02_main ;
			pw->master_w = pl_w ;
			pw->ef_w = ew ;
		} else break ;
	}
}

static void shl07_02_part_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl07_02_part_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl07_02_part_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl07_02_part( PLAYER_WORK *ew )
{
	static void ( *shl07_02_part_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl07_02_part_init, shl07_02_part_move, shl07_02_part_free,
	} ;

	shl07_02_part_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void shl07_02_part_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
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
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	ew->disp_dir = ew->sub_type ? 0 : 1 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->pos = ew->ef_w->pos ;
	pl_char_set( ew, Char_Effect, ew->timer0 + 16 ) ;
}

/*--------------------------------------------------------------*/
static void shl07_02_part_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	register PLAYER_WORK *pw = ew->ef_w ;

	if( vs_sys_work.slow_bit & 1 << vs_sys_work.slow_step ) return ;
	if( !pl_w->routine_1 && pl_w->act_st == N_SUPER && !pl_w->timer1 ){
		if( !pw->cg_type ) return ;
		ew->atck_no = ew->cg_flag1 ;
		HIT_RESET( ew ) ;
		hit_work_set( ew ) ;
		return ;
	}
	ew->routine_0 = 2 ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl07_02_part_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl07_02.c" End of File	*/

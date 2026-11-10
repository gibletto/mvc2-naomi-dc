/* The work that times the stop of the fight in a hyper combo's flash, a snap back or a KO: it holds
   the other side's players still, then gives the game back. */
#include "player.h"

extern void pl_hyper_stop_clr( PLAYER_WORK *pl_w );
extern void pl_win_team_set( void );

static void hyper_ef_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *hyper_ef_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get2( 0, W_PLS_ST, LIST_TOP ) ) ){
		ew->work_func = hyper_ef_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
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
		ew->disp_flag = 0 ;
	}
	return ew ;
}

static void hyper_ef_nml_main( PLAYER_WORK *ew );
static void hyper_ef_lvup_main( PLAYER_WORK *ew );
static void hyper_ef_main2( PLAYER_WORK *ew );
static void hyper_ef_ko_main( PLAYER_WORK *ew );
static void hyper_ef_ending_main( PLAYER_WORK *ew );

/*==============================================================*/
static void hyper_ef_main( PLAYER_WORK *ew )
{
	static void ( *hyper_ef_jmp[] )( PLAYER_WORK *ew ) = {
		hyper_ef_nml_main, hyper_ef_lvup_main, hyper_ef_main2, hyper_ef_ko_main,
		hyper_ef_nml_main, hyper_ef_nml_main, hyper_ef_nml_main, hyper_ef_ending_main,
	} ;

	hyper_ef_jmp[ew->type]( ew ) ;
}

static void hyper_ef_nml_init( PLAYER_WORK *ew );
static void hyper_ef_nml_move( PLAYER_WORK *ew );
static void hyper_ef_end( PLAYER_WORK *ew );
static void hyper_ef_free( PLAYER_WORK *ew );

/*==============================================================*/
static void hyper_ef_nml_main( PLAYER_WORK *ew )
{
	static void ( *hyper_ef_jmp0[] )( PLAYER_WORK *ew ) = {
		hyper_ef_nml_init, hyper_ef_nml_move, hyper_ef_end, hyper_ef_free,
	} ;

	hyper_ef_jmp0[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void hyper_ef_nml_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->timer0 = 44 ;
}

/*--------------------------------------------------------------*/
static void hyper_ef_nml_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *w ;
	Uint32 i ;

	if( !vs_sys_work.game_stop ){
		ew->routine_0 ++ ;
		return ;
	}
	if( -- ew->timer0 >= 0 ) return ;
	ew->routine_0 ++ ;
	for( i = 0 ; i < 3 ; i ++ ){
		w = vs_sys_work.team_w[ew->pl_no ^ 1][i] ;
		if( w->be_flag && w->routine_1 != PL_CAUGHT ) w->hit_stop = 10 ;
	}
	if( ew->type == 4 ){
		ew->master_w->nml_catch_ng_timer = 30 ;
		ew->master_w->catch_ng_timer = 30 ;
		ew->master_w->dm_flag = 0 ;
		pl_hyper_stop_clr( ew->master_w ) ;
		return ;
	}
	if( ew->type == 5 || !ew->type ){
		ew->master_w->nml_catch_ng_timer = 12 ;
		ew->master_w->catch_ng_timer = 12 ;
		ew->master_w->dm_flag = 0 ;
		pl_hyper_stop_clr( ew->master_w ) ;
		return ;
	}
	pl_hyper_stop_clr( ew->master_w ) ;
}

static void hyper_ef_lvup_init( PLAYER_WORK *ew );
static void hyper_ef_lvup_move( PLAYER_WORK *ew );

/*==============================================================*/
static void hyper_ef_lvup_main( PLAYER_WORK *ew )
{
	static void ( *hyper_ef_jmp1[] )( PLAYER_WORK *ew ) = {
		hyper_ef_lvup_init, hyper_ef_lvup_move, hyper_ef_end, hyper_ef_free,
	} ;

	hyper_ef_jmp1[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void hyper_ef_lvup_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->timer0 = 20 ;
}

/*--------------------------------------------------------------*/
static void hyper_ef_lvup_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *w ;
	Uint32 i ;

	if( !vs_sys_work.game_stop ){
		ew->routine_0 ++ ;
		return ;
	}
	if( !ew->routine_1 ){
		if( !ew->master_w->stop_move_flag ) ew->routine_1 ++ ;
		return ;
	}
	if( -- ew->timer0 >= 0 ) return ;
	ew->routine_0 ++ ;
	for( i = 0 ; i < 3 ; i ++ ){
		w = vs_sys_work.team_w[ew->pl_no][i] ;
		if( w->be_flag && w->routine_1 != PL_CAUGHT ) w->hit_stop = 1 ;
	}
	for( i = 0 ; i < 3 ; i ++ ){
		w = vs_sys_work.team_w[ew->pl_no ^ 1][i] ;
		if( w->be_flag && w->routine_1 == PL_DAMAGE ) w->hit_stop = 10 ;
	}
	ew->master_w->nml_catch_ng_timer = 12 ;
	ew->master_w->catch_ng_timer = 12 ;
	ew->master_w->dm_flag = 0 ;
	pl_hyper_stop_clr( ew->master_w ) ;
}

static void hyper_ef_init2( PLAYER_WORK *ew );
static void hyper_ef_move2( PLAYER_WORK *ew );

/*==============================================================*/
static void hyper_ef_main2( PLAYER_WORK *ew )
{
	static void ( *hyper_ef_jmp2[] )( PLAYER_WORK *ew ) = {
		hyper_ef_init2, hyper_ef_move2, hyper_ef_end, hyper_ef_free,
	} ;

	hyper_ef_jmp2[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void hyper_ef_init2( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->timer0 = 20 ;
	ew->timer1 = 20 ;
}

/*--------------------------------------------------------------*/
static void hyper_ef_move2( PLAYER_WORK *ew )
{
	PLAYER_WORK *w ;
	Uint32 i ;

	if( !vs_sys_work.game_stop ){
		ew->routine_0 ++ ;
		return ;
	}
	if( ew->timer0 ){
		if( !-- ew->timer0 ){
			scr_work.stop_x_mode = scr_work.x_mode ;
			scr_work.stop_y_mode = scr_work.y_mode ;
		}
		return ;
	}
	if( -- ew->timer1 >= 0 ) return ;
	ew->routine_0 ++ ;
	for( i = 0 ; i < 3 ; i ++ ){
		w = vs_sys_work.team_w[ew->pl_no ^ 1][i] ;
		if( w->be_flag && w->routine_1 != PL_CAUGHT ) w->hit_stop = 10 ;
	}
	pl_hyper_stop_clr( ew->master_w ) ;
}

static void hyper_ef_ko_init( PLAYER_WORK *ew );
static void hyper_ef_ko_move( PLAYER_WORK *ew );

/*==============================================================*/
static void hyper_ef_ko_main( PLAYER_WORK *ew )
{
	static void ( *hyper_ef_jmp3[] )( PLAYER_WORK *ew ) = {
		hyper_ef_ko_init, hyper_ef_ko_move, hyper_ef_end, hyper_ef_free,
	} ;

	hyper_ef_jmp3[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void hyper_ef_ko_init( PLAYER_WORK *ew )
{
	if( ew->master_w->vital_new || ew->master_w->team_no ){
		ew->routine_0 = 2 ;
		return ;
	}
	ew->timer0 = 64 ;
	vs_sys_w->slow_bit = SLOW_HALF ;
	ew->routine_0 ++ ;
	hyper_ef_ko_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void hyper_ef_ko_move( PLAYER_WORK *ew )
{
	if( -- ew->timer0 < 0 ){
		ew->routine_0 ++ ;
		vs_sys_w->slow_bit = 0 ;
	}
}

static void hyper_ef_ending_init( PLAYER_WORK *ew );
static void hyper_ef_ending_move( PLAYER_WORK *ew );

/*==============================================================*/
static void hyper_ef_ending_main( PLAYER_WORK *ew )
{
	static void ( *hyper_ef_jmp7[] )( PLAYER_WORK *ew ) = {
		hyper_ef_ending_init, hyper_ef_ending_move, hyper_ef_end, hyper_ef_free,
	} ;

	hyper_ef_jmp7[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void hyper_ef_ending_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->timer0 = 92 ;
	fade_set( -1, 32, FADE_OUT ) ;
}

/*--------------------------------------------------------------*/
static void hyper_ef_ending_move( PLAYER_WORK *ew )
{
	if( -- ew->timer0 < 0 ){
		ew->routine_0 ++ ;
		ew->timer0 = 48 ;
		fade_set( -1, 32, FADE_IN ) ;
		return ;
	}
	if( ew->timer0 == 40 ) pl_win_team_set() ;
}

/*--------------------------------------------------------------*/
static void hyper_ef_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void hyper_ef_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "hyper_ef.c" End of File	*/

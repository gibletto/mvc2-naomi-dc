/* The frame's run of the work lists after the hit check, slower while a hyper combo stops the
   game. */
#include "player.h"

extern void pl_hyper_stop_ck( PLAYER_WORK *pl_w );
extern void pl_hyper_stop_clr( PLAYER_WORK *pl_w );
extern void pl_work_move( void );
extern void stage_frame( void );
extern void pl_hit_work_set( void );
extern void bg_move( void );

static Uint8 game_stop_ck( void );

/****************************************************************/
void all_work_move( void )
{
	vs_sys_work.game_stop = game_stop_ck() ;
	pl_work_move() ;
	if( !vs_sys_work.game_stop ){
		bright_set( 1.0f ) ;
		if( !( vs_sys_work.slow_bit & 1 << vs_sys_work.slow_step ) ){
			work_list_move( W_PLS ) ;
			work_list_move( W_PLS_ST ) ;
			work_list_move( W_SHL ) ;
			work_list_move( W_SHL_ST ) ;
		}
	} else {
		bright_set( 0.5f ) ;
		if( !( vs_sys_work.slow_bit & 1 << vs_sys_work.slow_step ) ){
			work_list_move( W_PLS_ST ) ;
			work_list_move( W_SHL_ST ) ;
		}
	}
	if( !( vs_sys_work.slow_bit & 1 << vs_sys_work.slow_step ) ) scr_move() ;
	if( !vs_sys_work.game_stop ){
		work_list_move( W_SET ) ;
		work_list_move( W_BACK ) ;
		work_list_move( W_DM ) ;
		work_list_move( W_EF ) ;
		work_list_move( W_MARK ) ;
		work_list_move( W_FINISH ) ;
		stage_frame() ;
	} else {
		work_list_move( W_BACK ) ;
		work_list_move( W_MARK ) ;
		work_list_move( W_FINISH ) ;
	}
	pl_hit_work_set() ;
	bg_move() ;
	work_list_move( W_SYS ) ;
	work_list_move( W_FRONT ) ;
	fog_set() ;
}

/*==============================================================*/
/*			The hyper combo stops of both sides: a stop that	*/
/*			ended during the checks is cleared					*/
/*==============================================================*/
static Uint8 game_stop_ck( void )
{
	PLAYER_WORK *p1 = vs_sys_work.team_w[0][0] ;
	PLAYER_WORK *p2 = vs_sys_work.team_w[1][0] ;
	Uint8 stop = p1->hyper_stop_flag | p2->hyper_stop_flag ;
	Uint8 now ;

	if( stop & 1 << p1->pl_no ) pl_hyper_stop_ck( p1 ) ;
	if( stop & 1 << p2->pl_no ) pl_hyper_stop_ck( p2 ) ;
	now = p1->hyper_stop_flag | p2->hyper_stop_flag ;
	if( stop != now ){
		if( stop & 1 << p1->pl_no ) pl_hyper_stop_clr( p1 ) ;
		if( stop & 1 << p2->pl_no ) pl_hyper_stop_clr( p2 ) ;
	}
	return stop ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "all_move.c" End of File	*/

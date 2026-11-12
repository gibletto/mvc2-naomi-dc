/* What the select scene decides once the teams are chosen: each member's colour and the stage
  , and a debug print of the free texture memory. */
#include "player.h"
#include "select.h"

extern Sint8 sel_color_get( PLAYER_WORK *pl_w, PLAYER_WORK *tw, Sint16 sw ) ;

/* the stages, by the arcade route and the fight; random rows and columns in a versus game */
Sint8 stage_tbl[4][8] = {
	{ 0, 1, 2, 3, 4, 5, 6, 8 },
	{ 1, 3, 4, 6, 5, 2, 7, 8 },
	{ 6, 5, 0, 7, 1, 3, 2, 8 },
	{ 3, 6, 2, 4, 7, 1, 5, 8 },
} ;

#ifdef TARGET_DC
/* each stage's other look, once it is opened */
Sint8 stage_ex_tbl[9] = { 9, 10, 2, 12, 13, 14, 15, 16, 8 } ;
#endif

/****************************************************************/
/*			The colour of each of the side's three players,		*/
/*			from the button that chose it						*/
/****************************************************************/
void team_color_set( Sint8 side )
{
	PLAYER_WORK *pl_w = &pl_work[side] ;

	for( pl_w->timer1 = 0 ; pl_w->timer1 < 3 ; pl_w->timer1 ++ ){
		pl_work[pl_w->timer1 * 2 + side].load_color = sel_color_get( pl_w, &pl_work[pl_w->timer1 * 2 + side], 0x200 ) ;
	}
}

/*==============================================================*/
/*			The stage: random in a versus game, else the		*/
/*			next on the player's route							*/
/*==============================================================*/
static void stage_no_set( void )
{
	int col, row ;

	if( ( Sys->new_side | Sys->play_side ) == 3 ){
		col = nlRand() % 6 ;
		row = nlRand() % 4 ;
		stage_no = stage_tbl[row][col] ;
		Sys->boss_flag = 0 ;
	} else {
		if( ( Sys->new_side | Sys->play_side ) == 1 ) stage_no = stage_tbl[pl_work[0].route_no][Sys->stage_cnt] ;
		else stage_no = stage_tbl[pl_work[1].route_no][Sys->stage_cnt] ;
		if( stage_no == 8 ) Sys->boss_flag = 1 ;
		else Sys->boss_flag = 0 ;
	}
}

#ifdef TARGET_DC
/*==============================================================*/
/*			The stage, by the home game's mode: the versus's	*/
/*			is chosen, the training has its own, the network	*/
/*			game takes one both sides agree on; the others		*/
/*			show an opened stage's other look half the time		*/
/*==============================================================*/
static void mode_stage_no_set( void )
{
	int mode, r ;
	Sint8 max ;

	if( Sys->home_mode == HOME_VERSUS ) return ;
	if( Sys->home_mode == HOME_TRAINING ){
		stage_no = 11 ;
		return ;
	}
	if( Sys->home_mode == HOME_NETWORK ){
		stage_no = ( net_win_cnt + net_vs_win_cnt + net_win_add + net_vs_win_add ) & 7 ;
		return ;
	}
	stage_no_set() ;
	mode = Sys->home_mode ;
	if( ( !mode || mode == 7 ) && sel_w.stage_ex_cnt ){
		r = nlRand() ;
		if( r & 1 ){
			max = sel_w.stage_ex_cnt - 1 ;
			if( stage_no <= max ) stage_no = stage_ex_tbl[stage_no] ;
		}
	}
}
#endif

/****************************************************************/
void stage_decide( void )
{
#ifdef TARGET_DC
	mode_stage_no_set() ;
#else
	stage_no_set() ;
#endif
}

/****************************************************************/
/*			The free texture memory, for debugging				*/
/****************************************************************/
void tex_mem_disp( void )
{
	Uint32 mem ;

	mem = nlCommonTextureGetFreeMem() ;
#ifndef TARGET_DC
	mem -= 0x800000 ;	/* the board has twice the console's texture memory */
#endif
	STrPrint( 8, 20, 0, "%x/%x", mem, 0x400000 ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "sel_decide.c" End of File	*/

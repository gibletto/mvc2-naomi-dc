/* What the select scene decides once the teams are chosen: each member's colour and the stage
  , and a debug print of the free texture memory. */
#include "player.h"

extern Sint8 sel_color_get( PLAYER_WORK *pl_w, PLAYER_WORK *tw, Sint16 sw ) ;

/* the stages, by the arcade route and the fight; random rows and columns in a versus game */
Sint8 stage_tbl[4][8] = {
	{ 0, 1, 2, 3, 4, 5, 6, 8 },
	{ 1, 3, 4, 6, 5, 2, 7, 8 },
	{ 6, 5, 0, 7, 1, 3, 2, 8 },
	{ 3, 6, 2, 4, 7, 1, 5, 8 },
} ;

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

/****************************************************************/
void stage_decide( void )
{
	stage_no_set() ;
}

/****************************************************************/
/*			The free texture memory, for debugging				*/
/****************************************************************/
void tex_mem_disp( void )
{
	Uint32 mem ;

	mem = nlCommonTextureGetFreeMem() ;
	mem -= 0x800000 ;
	STrPrint( 8, 20, 0, "%x/%x", mem, 0x400000 ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "sel_decide.c" End of File	*/

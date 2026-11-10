/* Mega Man's player effect 02: the glow that follows Mega Man and Roll while a command is held
   (the buster's charge): it flashes the player's colours, and the longer it lasts the faster. */
#include "player.h"
#include "ef_char.h"
#include "pl_color.h"
#include "pl1c_def.h"

/* the effect's own work: the colour cycle it runs on the player */
typedef struct PLS1C_02_LOCAL_W {
	COLOR_STEP	*step ;		/* the next step */
	Uint8	time ;			/* the step's time left */
	Uint8	last ;			/* set on the cycle's last step */
	Uint16	wait ;			/* frames to the next gain in the charge */
	Uint8	helper_flag ;		/* set once the other effect is made */
} PLS1C_02_LOCAL_W ;
#define PLS1C_02_LW( w )	( ( PLS1C_02_LOCAL_W * )&( w )->obj_flag )

static void pls1c_02_main( PLAYER_WORK *ew );

/****************************************************************/
/*			Made with pls1c_02_set() in the character's			*/
/*			module												*/
/****************************************************************/
PLAYER_WORK *pls1c_02_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls1c_02_main ;
		ew->master_w = pl_w ;
	}
	return ew ;
}

static COLOR_STEP pls1c_02_hold_col[14] = {
	{ 4, 0, 2, 1, 1 },
	{ 4, 0, 3, 1, 1 },
	{ 4, 0, 4, 1, 1 },
	{ 4, 0, 5, 1, 1 },
	{ 4, 0, 6, 1, 1 },
	{ 4, 0, 7, 1, 1 },
	{ 4, 0, 8, 1, 1 },
	{ 4, 0, 9, 1, 1 },
	{ 4, 0, 8, 1, 1 },
	{ 4, 0, 7, 1, 1 },
	{ 4, 0, 6, 1, 1 },
	{ 4, 0, 5, 1, 1 },
	{ 4, 0, 4, 1, 1 },
	{ 4, 0, 3, 1, -13 },
} ;

static COLOR_STEP pls1c_02_charge_col[42] = {
	{ 4, 0, 38, 1, 1 },
	{ 4, 0, 39, 1, 1 },
	{ 4, 0, 40, 1, 1 },
	{ 4, 0, 41, 1, 1 },
	{ 4, 0, 42, 1, 1 },
	{ 4, 0, 43, 1, 1 },
	{ 4, 0, 44, 1, 1 },
	{ 4, 0, 45, 1, 1 },
	{ 4, 0, 44, 1, 1 },
	{ 4, 0, 43, 1, 1 },
	{ 4, 0, 42, 1, 1 },
	{ 4, 0, 41, 1, 1 },
	{ 4, 0, 40, 1, 1 },
	{ 4, 0, 39, 1, 1 },
	{ 4, 0, 38, 1, 1 },
	{ 3, 0, 39, 1, 1 },
	{ 3, 0, 40, 1, 1 },
	{ 3, 0, 41, 1, 1 },
	{ 3, 0, 42, 1, 1 },
	{ 3, 0, 43, 1, 1 },
	{ 3, 0, 44, 1, 1 },
	{ 3, 0, 45, 1, 1 },
	{ 3, 0, 44, 1, 1 },
	{ 3, 0, 43, 1, 1 },
	{ 3, 0, 42, 1, 1 },
	{ 3, 0, 41, 1, 1 },
	{ 3, 0, 40, 1, 1 },
	{ 3, 0, 39, 1, 1 },
	{ 2, 0, 38, 1, 1 },
	{ 2, 0, 39, 1, 1 },
	{ 2, 0, 40, 1, 1 },
	{ 2, 0, 41, 1, 1 },
	{ 2, 0, 42, 1, 1 },
	{ 2, 0, 43, 1, 1 },
	{ 2, 0, 44, 1, 1 },
	{ 2, 0, 45, 1, 1 },
	{ 2, 0, 44, 1, 1 },
	{ 2, 0, 43, 1, 1 },
	{ 2, 0, 42, 1, 1 },
	{ 2, 0, 41, 1, 1 },
	{ 2, 0, 40, 1, 1 },
	{ 2, 0, 39, 1, -13 },
} ;

static void pls1c_02_init( PLAYER_WORK *ew );
static void pls1c_02_move( PLAYER_WORK *ew );
static void pls1c_02_end( PLAYER_WORK *ew );
static void pls1c_02_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls1c_02_main( PLAYER_WORK *ew )
{
	static void ( *pls1c_02_jmp[] )( PLAYER_WORK *ew ) = {
		pls1c_02_init, pls1c_02_move, pls1c_02_end, pls1c_02_free,
	} ;

	pls1c_02_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls1c_02_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PL1C_LOCAL_W *lw = PL1C_LW( pl_w ) ;

	ew->routine_0 ++ ;
	ew->ef_no = 0x1c02 ;
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
	ew->sub_prio = -1 ;
	lw->item = 1 ;
	if( !PLS1C_02_LW( ew )->helper_flag ){
		lw->apart_flag = 1 ;
		if( pls1c_03_set( pl_w ) ){
			PLS1C_02_LW( ew )->helper_flag = 1 ;
		}
	}
	pls1c_02_move( ew ) ;
}

static void pls1c_02_off( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_02_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_02_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_02_charge( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void pls1c_02_move( PLAYER_WORK *ew )
{
	static void ( *pls1c_02_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_02_wait, pls1c_02_hold, pls1c_02_charge,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( !pl_w->vital_new ){
		pls1c_02_off( ew, pl_w ) ;
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	pls1c_02_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

static void pls1c_02_color_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w, COLOR_STEP *step );
static void pls1c_02_follow( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Waits for the command to start						*/
/*==============================================================*/
static void pls1c_02_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL1C_LOCAL_W *lw = PL1C_LW( pl_w ) ;

	if( !pl_w->com_w.step || pl_w->com_w.timer > 64 ){
		return ;
	}
	ew->routine_1 ++ ;
	lw->charge_flag = 1 ;
	pls1c_02_color_start( ew, pl_w, pls1c_02_hold_col ) ;
	ef_char_set( ew, 27, 6 ) ;
	pls1c_02_hold( ew, pl_w ) ;
}

/*==============================================================*/
/*			The command is held, and not yet done				*/
/*==============================================================*/
static void pls1c_02_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL1C_LOCAL_W *lw = PL1C_LW( pl_w ) ;

	if( !pl_w->com_w.step ){
		pls1c_02_off( ew, pl_w ) ;
		return ;
	}
	if( pl_w->com_w.timer ){
		pls1c_02_follow( ew, pl_w ) ;
		return ;
	}
	ew->routine_1 ++ ;
	pls1c_02_color_start( ew, pl_w, pls1c_02_charge_col ) ;
	ef_char_set( ew, 27, 7 ) ;
	lw->charge_lv = 2 ;
	PLS1C_02_LW( ew )->wait = 120 ;
	pl_voice_req( pl_w, 39 ) ;
	pls1c_02_charge( ew, pl_w ) ;
}

static void pls1c_02_flash( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The charge builds up								*/
/*==============================================================*/
static void pls1c_02_charge( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !pl_w->com_w.step ){
		pls1c_02_off( ew, pl_w ) ;
		return ;
	}
	pls1c_02_flash( ew, pl_w ) ;
	pls1c_02_follow( ew, pl_w ) ;
}

static void pls1c_02_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static int pls1c_02_color_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls1c_02_follow( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = pl_w->disp_flag ;
	ew->prio = pl_w->prio ;
	pls1c_02_pos_set( ew, pl_w ) ;
	pls1c_02_color_move( ew, pl_w ) ;
	ef_char_move( ew ) ;
}

/*==============================================================*/
/*			The command is over: back to the player's own		*/
/*			colours												*/
/*==============================================================*/
static void pls1c_02_off( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL1C_LOCAL_W *lw = PL1C_LW( pl_w ) ;

	ew->routine_1 = 0 ;
	ew->disp_flag = 0 ;
	lw->charge_flag = 0 ;
	pl_color_set( pl_w, 1 ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_02_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls1c_02_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
static void pls1c_02_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Sint16 pls1c_02_y_tbl[4] = { 42, 28, 62, 16 } ;

	if( !pl_w->disp_dir ){
		ew->pos.x = pl_w->pos.x + -DOT_X( 7.0f ) ;
	} else {
		ew->pos.x = pl_w->pos.x + DOT_X( 7.0f ) ;
	}
	ew->pos.y = pl_w->pos.y + DOT_Y( pls1c_02_y_tbl[pl_w->pl_stance] ) ;
}

/*==============================================================*/
static void pls1c_02_flash( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL1C_LOCAL_W *lw = PL1C_LW( pl_w ) ;

	if( --PLS1C_02_LW( ew )->wait == 0 ){
		PLS1C_02_LW( ew )->wait = 120 ;
		if( lw->charge_lv < 50 ){
			lw->charge_lv ++ ;
		}
	}
}

/*==============================================================*/
static void pls1c_02_color_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w, COLOR_STEP *step )
{
	PLS1C_02_LOCAL_W *lw = PLS1C_02_LW( ew ) ;
	int col ;

	do {
		lw->time = step->time ;
		col = step->col + pl_w->pl_color * 87 ;
		ef_color_set( pl_w, step->no, col, step->cnt ) ;
		if( step->next == 1 ) lw->last = 0 ;
		else lw->last = 1 ;
		step += step->next ;
	} while( lw->time == 0 ) ;
	lw->step = step ;
}

/*==============================================================*/
static int pls1c_02_color_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS1C_02_LOCAL_W *lw = PLS1C_02_LW( ew ) ;
	int ret = 1 ;

	if( !--lw->time ){
		if( lw->last ) ret = 0 ;
		pls1c_02_color_start( ew, pl_w, lw->step ) ;
	}
	return ret ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls1c_02.c" End of File	*/

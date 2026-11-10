/* Mega Man's player effect 04: the picture that brings Mega Man's and Roll's item when they
   call for it (sp_item_sel): it drops from the top of the screen, lands, turns to the player, runs
   at them and throws the item. routine_2 picks the stage, routine_3 the step in it. */
#include "player.h"
#include "pl_char.h"
#include "pl1c_def.h"

static void pls1c_04_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls1c_04_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;
	PL1C_LOCAL_W *lw ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls1c_04_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		lw = PL1C_LW( pl_w ) ;
		lw->item_ef_flag = 0xff ;
	}
	return ew ;
}

static void pls1c_04_init( PLAYER_WORK *ew );
static void pls1c_04_move( PLAYER_WORK *ew );
static void pls1c_04_end( PLAYER_WORK *ew );
static void pls1c_04_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls1c_04_main( PLAYER_WORK *ew )
{
	static void ( *pls1c_04_jmp[] )( PLAYER_WORK *ew ) = {
		pls1c_04_init, pls1c_04_move, pls1c_04_end, pls1c_04_free,
	} ;

	pls1c_04_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls1c_04_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
	ew->ef_no = 0x1c04 ;
	pl_w = ew->master_w ;
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
	ew->size_top = 24 ;
	ew->size_bottom = 24 ;
	ew->size_front = 16 ;
	ew->size_back = 16 ;
	ew->prio = 0 ;
	pls1c_04_move( ew ) ;
}

static void pls1c_04_drop( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_04_jump( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_04_turn( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_04_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_04_throw( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void pls1c_04_move( PLAYER_WORK *ew )
{
	static void ( *pls1c_04_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_04_drop, pls1c_04_jump, pls1c_04_turn, pls1c_04_run, pls1c_04_throw,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->act_st == N_SPECIAL ){
		if( pl_w->pl_sp_type == pl1c_sp_hyper_rock || pl_w->pl_sp_type == pl1c_sp_drill
		 || pl_w->pl_sp_type == pl1c_sp_beat || pl_w->pl_sp_type == pl1c_sp_13 ){
			PL1C_LOCAL_W *lw = PL1C_LW( pl_w ) ;

			lw->item_ef_flag = 0 ;
			ew->routine_0 ++ ;
			ew->routine_1 = ew->routine_2 = ew->routine_3 = 0 ;
			ew->disp_flag = 0 ;
			return ;
		}
	}
	ew->disp_flag = pl_w->disp_flag ;
	pls1c_04_move_jmp[ew->routine_2]( ew, pl_w ) ;
}

static void pls1c_04_drop_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_04_drop_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_04_drop_land( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Falls from above the screen							*/
/*==============================================================*/
static void pls1c_04_drop( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls1c_04_drop_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_04_drop_init, pls1c_04_drop_fall, pls1c_04_drop_land,
	} ;

	pls1c_04_drop_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls1c_04_drop_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float x_ofs_0 ;
	Float x_ofs_1 ;

	ew->routine_3 ++ ;
	ew->disp_dir = pl_w->disp_dir ^ 1 ;
	x_ofs_0 = 160.0f ;
	x_ofs_1 = -160.0f ;
	if( !pl_w->disp_dir ){
		ew->pos.x = pl_w->pos.x + x_ofs_1 ;
	} else {
		ew->pos.x = pl_w->pos.x + x_ofs_0 ;
	}
	ew->pos.y = scr_work.scr_top + DOT_Y( 32.0f ) ;
	ew->spd.x = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = -DOT_Y( 8.0f ) ;
	ew->acc.y = -DOT_Y( 0.375f ) ;
	if( !scr_in_x_ck( ew ) ){
		ew->disp_dir = 0 ;
		if( !pl_w->disp_dir ){
			ew->pos.x = pl_w->pos.x + x_ofs_0 ;
		} else {
			ew->pos.x = pl_w->pos.x + x_ofs_1 ;
		}
	}
	pl_voice_req( ew, 33 ) ;
	pl_char_set( ew, 25, 23 ) ;
	ew->be_flag = 1 ;
	pl_shadow_set( ew ) ;
	pls1c_04_drop_fall( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_04_drop_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( pl_w->ground_y < ew->pos.y ) return ;
	ew->routine_3 ++ ;
	ew->pos.y = pl_w->ground_y ;
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_04_drop_land( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_2 = 2 ;
		ew->routine_3 = 0 ;
	}
}

static void pls1c_04_jump_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_04_jump_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_04_jump_up( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Jumps up											*/
/*==============================================================*/
static void pls1c_04_jump( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls1c_04_jump_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_04_jump_init, pls1c_04_jump_start, pls1c_04_jump_up,
	} ;

	pls1c_04_jump_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls1c_04_jump_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->spd.x = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = DOT_Y( 8.0f ) ;
	ew->acc.y = 0.0f ;
	pl_char_set( ew, 25, 24 ) ;
	pls1c_04_jump_start( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_04_jump_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( ew->cg_type ) ew->routine_3 ++ ;
}

/*--------------------------------------------------------------*/
static void pls1c_04_jump_up( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL1C_LOCAL_W *lw = PL1C_LW( pl_w ) ;

	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !scr_in_y_ck( ew ) ){
		lw->item_ef_flag = 0 ;
		ew->routine_0 ++ ;
		ew->routine_1 = ew->routine_2 = ew->routine_3 = 0 ;
	}
}

static void pls1c_04_turn_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_04_turn_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Waits to turn to the player							*/
/*==============================================================*/
static void pls1c_04_turn( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls1c_04_turn_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_04_turn_init, pls1c_04_turn_move,
	} ;

	pls1c_04_turn_jmp[ew->routine_3]( ew, pl_w ) ;
}

static int pls1c_04_near_ck( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls1c_04_turn_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->timer0 = 30 ;
	ew->spd.x = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.y = 0.0f ;
	if( !( ew->pos.x < pl_w->pos.x ) ){
		ew->disp_dir = 0 ;
	} else {
		ew->disp_dir = 1 ;
	}
	pl_char_set( ew, 25, 20 ) ;
	pls1c_04_turn_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_04_turn_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		if( pls1c_04_near_ck( ew, pl_w ) ){
			ew->routine_2 = 4 ;
		} else {
			ew->routine_2 = 3 ;
		}
		ew->routine_3 = 0 ;
	}
}

static void pls1c_04_run_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_04_run_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Runs at the player									*/
/*==============================================================*/
static void pls1c_04_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls1c_04_run_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_04_run_init, pls1c_04_run_move,
	} ;

	pls1c_04_run_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls1c_04_run_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	if( !ew->disp_dir ){
		ew->spd.x = -DOT_X( 2.0f ) ;
	} else {
		ew->spd.x = DOT_X( 2.0f ) ;
	}
	ew->acc.x = 0.0f ;
	pl_char_set( ew, 25, 21 ) ;
	pls1c_04_run_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_04_run_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL1C_LOCAL_W *lw = PL1C_LW( pl_w ) ;
	int zero = 0 ;

	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	if( !scr_in_x_ck( ew ) ){
		lw->item_ef_flag = zero ;
		ew->routine_0 ++ ;
		ew->routine_1 = ew->routine_2 = ew->routine_3 = zero ;
		return ;
	}
	if( pls1c_04_near_ck( ew, pl_w ) ){
		ew->routine_2 = 4 ;
		ew->routine_3 = zero ;
	}
	pl_char_move( ew ) ;
}

static void pls1c_04_throw_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_04_throw_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_04_throw_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Throws: makes the item (pls1c_05)					*/
/*==============================================================*/
static void pls1c_04_throw( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls1c_04_throw_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_04_throw_init, pls1c_04_throw_move, pls1c_04_throw_end,
	} ;

	pls1c_04_throw_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls1c_04_throw_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->spd.x = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.y = 0.0f ;
	pl_char_set( ew, 25, 22 ) ;
	pls1c_04_throw_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_04_throw_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( ew->cg_type ){
		ew->routine_3 ++ ;
		pls1c_05_set( ew, ew->type ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls1c_04_throw_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_2 = 1 ;
		ew->routine_3 = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void pls1c_04_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls1c_04_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			Whether the picture is within 160 of the player		*/
/*==============================================================*/
static int pls1c_04_near_ck( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float dx = ew->pos.x - pl_w->pos.x ;

	if( dx >= -160.0f && dx <= 160.0f ) return 1 ;
	return 0 ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls1c_04.c" End of File	*/

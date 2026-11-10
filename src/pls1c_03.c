/* Mega Man's player effect 03: the helper that stays beside Mega Man or Roll in the round:
   it stands, walks, runs and joins the hyper combos as its routine_2 says, one routine for each
   action (17 of them), each with its own steps in routine_3. */
#include "player.h"
#include "pl_char.h"
#include "pl1c_def.h"

/* the effect's own work: the colour cycle it runs on the player, and the action to return to */
typedef struct PLS1C_03_LOCAL_W {
	COLOR_STEP	*step ;		/* the next step */
	Uint8	time ;			/* the step's time left */
	Uint8	last ;			/* set on the cycle's last step */
	Uint8	save_2 ;		/* routine_2 before the action changed */
	Uint8	save_3 ;		/* and routine_3 */
} PLS1C_03_LOCAL_W ;
#define PLS1C_03_LW( w )	( ( PLS1C_03_LOCAL_W * )&( w )->obj_flag )

/* the action changes: where it was is kept */
#define PLS1C_03_SAVE( w )	{ PLS1C_03_LW( w )->save_2 = ( w )->routine_2 ; PLS1C_03_LW( w )->save_3 = ( w )->routine_3 ; }

static void pls1c_03_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls1c_03_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS_ST, LIST_TOP ) ) ){
		ew->work_func = pls1c_03_main ;
		ew->master_w = pl_w ;
	}
	return ew ;
}

/* the colour cycle when the partner comes in */
static COLOR_STEP pls1c_03_col[6] = {
	{ 4, 1, 19, 1, 1 },
	{ 4, 1, 18, 1, 1 },
	{ 4, 1, 17, 1, 1 },
	{ 4, 1, 16, 1, 1 },
	{ 4, 1, 15, 1, 1 },
	{ 16, 1, 14, 1, 0 },
} ;

static void pls1c_03_init( PLAYER_WORK *ew );
static void pls1c_03_move( PLAYER_WORK *ew );
static void pls1c_03_end( PLAYER_WORK *ew );
static void pls1c_03_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls1c_03_main( PLAYER_WORK *ew )
{
	static void ( *pls1c_03_jmp[] )( PLAYER_WORK *ew ) = {
		pls1c_03_init, pls1c_03_move, pls1c_03_end, pls1c_03_free,
	} ;

	pls1c_03_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls1c_03_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	int zero = 0 ;

	if( Sys->play_mode == 0x80 ){
		ew->routine_0 = 2 ;
		ew->routine_1 = 0 ;
		return ;
	}
	ew->routine_0 ++ ;
	ew->ef_no = 0x1c03 ;
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
	ew->size_front = 32 ;
	ew->size_back = 32 ;
	ew->prio = 12 ;
	pl_char_set( ew, 25, 0 ) ;
	pl_shadow_set( ew ) ;
	ew->timer1 = 4 ;
	if( pl_w->team_no ){
		ew->routine_2 = 5 ;
		ew->be_flag = zero ;
		ew->disp_flag = zero ;
	}
	pls1c_03_move( ew ) ;
}

static void pls1c_03_appear( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_leave( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_stand( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_walk( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_hide( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_t6( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_t7( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_win_join( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_win( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_charge( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_turn( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_join( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_away( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_damage( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_t16( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void pls1c_03_move( PLAYER_WORK *ew )
{
	static void ( *pls1c_03_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_03_appear, pls1c_03_leave, pls1c_03_stand, pls1c_03_walk, pls1c_03_run, pls1c_03_hide,
		pls1c_03_t6, pls1c_03_t7, pls1c_03_win_join, pls1c_03_win, pls1c_03_charge, pls1c_03_turn,
		pls1c_03_join, pls1c_03_away, pls1c_03_join, pls1c_03_damage, pls1c_03_t16,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;
	Uint8 stop = vs_sys_work.game_stop ;

	if( stop & ( 1 << ( pl_w->pl_no ^ 1 ) ) ){
		return ;
	}
	if( stop && pl_w->act_st == N_SUPER && pl_w->pl_sp_type == 8 ){
		return ;
	}
	pls1c_03_move_jmp[ew->routine_2]( ew, pl_w ) ;
}

static void pls1c_03_appear_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_appear_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_appear_color( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_appear_land( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Comes in from above, to the player's side			*/
/*==============================================================*/
static void pls1c_03_appear( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls1c_03_appear_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_03_appear_init, pls1c_03_appear_fall, pls1c_03_appear_color, pls1c_03_appear_land,
	} ;

	pls1c_03_appear_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls1c_03_appear_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->be_flag = 1 ;
	ew->pos.x = pl_w->pos.x + ( !pl_w->disp_dir ? 80.0f : -80.0f ) ;
	ew->pos.y = pl_w->ground_y + DOT_Y( 256.0f ) ;
	ew->spd.x = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.y = -DOT_Y( 0.375f ) ;
	pl_char_set( ew, 25, 12 ) ;
	pls1c_03_appear_fall( ew, pl_w ) ;
}

void pls1c_03_color_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w, COLOR_STEP *step );
int pls1c_03_color_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void pls1c_03_appear_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( pl_w->ground_y < ew->pos.y ){
		return ;
	}
	ew->routine_3 ++ ;
	ew->pos.y = pl_w->ground_y ;
	ew->disp_dir = pl_w->disp_dir ;
	pls1c_03_color_start( ew, pl_w, pls1c_03_col ) ;
	pls1c_03_color_move( ew, pl_w ) ;
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_03_appear_color( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !pls1c_03_color_move( ew, pl_w ) ){
		ew->routine_3 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void pls1c_03_appear_land( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_2 = 2 ;
		ew->routine_3 = 0 ;
	}
}

/*==============================================================*/
/*			Waits for the player to come in						*/
/*==============================================================*/
static void pls1c_03_hide( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !pl_w->be_flag || pl_w->team_no ){
		return ;
	}
	ew->routine_2 = 2 ;
	ew->routine_3 = 0 ;
	ew->be_flag = 1 ;
	ew->disp_flag = 1 ;
	ew->pos.x = pl_w->pos.x + ( !pl_w->disp_dir ? 80.0f : -80.0f ) ;
	ew->pos.y = pl_w->ground_y ;
	pl_char_set( ew, 25, 0 ) ;
}

static void pls1c_03_leave_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_leave_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_leave_up( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Jumps out of the screen								*/
/*==============================================================*/
static void pls1c_03_leave( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls1c_03_leave_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_03_leave_init, pls1c_03_leave_start, pls1c_03_leave_up,
	} ;

	pls1c_03_leave_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls1c_03_leave_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->spd.x = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = DOT_Y( 8.0f ) ;
	ew->acc.y = 0.0f ;
	pl_char_set( ew, 25, 15 ) ;
	pls1c_03_leave_start( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_03_leave_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( ew->cg_type ){
		ew->routine_3 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void pls1c_03_leave_up( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !scr_in_y_ck( ew ) ){
		ew->routine_2 = 5 ;
		ew->routine_3 = 0 ;
		ew->be_flag = 0 ;
		ew->disp_flag = 0 ;
	}
}

static void pls1c_03_stand_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_stand_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Stands by the player, facing it						*/
/*==============================================================*/
static void pls1c_03_stand( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls1c_03_stand_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_03_stand_init, pls1c_03_stand_move,
	} ;

	pls1c_03_stand_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls1c_03_stand_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Uint8 dir ;

	ew->routine_3 ++ ;
	ew->spd.x = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = 0.0f ;
	dir = 0 ;
	ew->acc.y = 0.0f ;
	if( ew->pos.x < pl_w->pos.x ){
		dir = 1 ;
	}
	if( dir != ew->disp_dir ){
		PLS1C_03_SAVE( ew ) ;
		ew->routine_2 = 11 ;
		ew->routine_3 = 0 ;
		pls1c_03_turn( ew, pl_w ) ;
		return ;
	}
	if( ew->routine_2 != PLS1C_03_LW( ew )->save_2 ){
		pl_char_set( ew, 25, 0 ) ;
	}
	pls1c_03_stand_move( ew, pl_w ) ;
}

static void pls1c_03_next( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void pls1c_03_stand_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( !--ew->timer1 ){
		pls1c_03_next( ew, pl_w ) ;
	}
}

static void pls1c_03_turn_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_turn_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Turns round											*/
/*==============================================================*/
static void pls1c_03_turn( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls1c_03_turn_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_03_turn_init, pls1c_03_turn_move,
	} ;

	pls1c_03_turn_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls1c_03_turn_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->spd.x = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->disp_dir ^= 1 ;
	pl_char_set( ew, 25, 6 ) ;
	pls1c_03_turn_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_03_turn_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		PLS1C_03_SAVE( ew ) ;
		ew->routine_2 = 2 ;
		ew->routine_3 = 0 ;
	}
}

static void pls1c_03_t6_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_t6_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Waits, facing the player							*/
/*==============================================================*/
static void pls1c_03_t6( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls1c_03_t6_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_03_t6_init, pls1c_03_t6_move,
	} ;

	pls1c_03_t6_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls1c_03_t6_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->spd.x = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.y = 0.0f ;
	if( ew->pos.x < pl_w->pos.x ){
		ew->disp_dir = 1 ;
	} else {
		ew->disp_dir = 0 ;
	}
	if( ew->routine_2 != PLS1C_03_LW( ew )->save_2 ){
		pl_char_set( ew, 25, 3 ) ;
	}
	pls1c_03_t6_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_03_t6_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( !--ew->timer1 ){
		pls1c_03_next( ew, pl_w ) ;
	}
}

static void pls1c_03_t7_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_t7_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void pls1c_03_t7( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls1c_03_t7_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_03_t7_init, pls1c_03_t7_move,
	} ;

	pls1c_03_t7_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls1c_03_t7_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->spd.x = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.y = 0.0f ;
	if( ew->pos.x < pl_w->pos.x ){
		ew->disp_dir = 1 ;
	} else {
		ew->disp_dir = 0 ;
	}
	if( ew->routine_2 != PLS1C_03_LW( ew )->save_2 ){
		pl_char_set( ew, 25, 8 ) ;
	}
	pls1c_03_t7_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_03_t7_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( !--ew->timer1 ){
		pls1c_03_next( ew, pl_w ) ;
	}
}

static void pls1c_03_charge_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_charge_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void pls1c_03_charge( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls1c_03_charge_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_03_charge_init, pls1c_03_charge_move,
	} ;

	pls1c_03_charge_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
/*			Waits, facing the player's opponent					*/
/*==============================================================*/
static void pls1c_03_charge_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->spd.x = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.y = 0.0f ;
	if( ew->pos.x < pl_w->target_w->pos.x ){
		ew->disp_dir = 1 ;
	} else {
		ew->disp_dir = 0 ;
	}
	if( ew->routine_2 != PLS1C_03_LW( ew )->save_2 ){
		pl_char_set( ew, 25, 10 ) ;
	}
	pls1c_03_charge_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_03_charge_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( !--ew->timer1 ){
		pls1c_03_next( ew, pl_w ) ;
	}
}

static void pls1c_03_damage_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_damage_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void pls1c_03_damage( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls1c_03_damage_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_03_damage_init, pls1c_03_damage_move,
	} ;

	pls1c_03_damage_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls1c_03_damage_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->spd.x = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.y = 0.0f ;
	if( ew->pos.x < pl_w->pos.x ){
		ew->disp_dir = 1 ;
	} else {
		ew->disp_dir = 0 ;
	}
	if( ew->routine_2 != PLS1C_03_LW( ew )->save_2 ){
		pl_char_set( ew, 25, 9 ) ;
	}
	pls1c_03_damage_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_03_damage_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( !--ew->timer1 ){
		pls1c_03_next( ew, pl_w ) ;
	}
}

static void pls1c_03_walk_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_walk_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void pls1c_03_walk( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls1c_03_walk_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_03_walk_init, pls1c_03_walk_move,
	} ;

	pls1c_03_walk_jmp[ew->routine_3]( ew, pl_w ) ;
}

static Float pls1c_03_x_set( PLAYER_WORK *ew, Sint16 dist );
static void pls1c_03_run_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Walks to the player's side, backing off when it		*/
/*			is close											*/
/*==============================================================*/
static void pls1c_03_walk_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float d ;

	ew->routine_3 ++ ;
	d = ew->pos.x - pls1c_03_x_set( pl_w, 48 ) ;
	if( d >= 0 ){
		ew->disp_dir = 0 ;
	} else {
		d *= -1.0f ;
		ew->disp_dir = 1 ;
	}
	if( !( DOT_X( 56.0f ) > d ) ){
		ew->routine_2 = 4 ;
		ew->routine_3 = 0 ;
		pls1c_03_run_init( ew, pl_w ) ;
		return ;
	}
	if( pl_w->disp_dir == ew->disp_dir ){
		ew->spd.x = -DOT_X( 3.5f ) ;
	} else {
		ew->spd.x = -5.0f ;
	}
	ew->acc.x = 0.0f ;
	if( ew->disp_dir ){
		ew->spd.x *= -1.0f ;
	}
	if( ew->routine_2 != PLS1C_03_LW( ew )->save_2 ){
		pl_char_set( ew, 25, 4 ) ;
	}
	pls1c_03_walk_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_03_walk_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	if( !--ew->timer1 ){
		pls1c_03_next( ew, pl_w ) ;
	}
}

static void pls1c_03_run_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void pls1c_03_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls1c_03_run_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_03_run_init, pls1c_03_run_move,
	} ;

	pls1c_03_run_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
/*			Runs to the player's side							*/
/*==============================================================*/
static void pls1c_03_run_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	if( ew->pos.x - pls1c_03_x_set( pl_w, 48 ) >= 0 ){
		ew->disp_dir = 0 ;
		ew->spd.x = -DOT_X( 4.25f ) ;
	} else {
		ew->disp_dir = 1 ;
		ew->spd.x = DOT_X( 4.25f ) ;
	}
	ew->acc.x = 0.0f ;
	if( ew->routine_2 != PLS1C_03_LW( ew )->save_2 ){
		pl_char_set( ew, 25, 5 ) ;
	}
	pls1c_03_run_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_03_run_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	if( !--ew->timer1 ){
		pls1c_03_next( ew, pl_w ) ;
	}
}

static void pls1c_03_win_join_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_win_join_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_win_join_near( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_win_join_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_win_join_leave( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			In a win: runs up to the player's side				*/
/*==============================================================*/
static void pls1c_03_win_join( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls1c_03_win_join_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_03_win_join_init, pls1c_03_win_join_run, pls1c_03_win_join_near, pls1c_03_win_join_wait, pls1c_03_win_join_leave,
	} ;

	pls1c_03_win_join_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls1c_03_win_join_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Uint8 dir ;

	ew->routine_3 ++ ;
	ew->sub_prio = -1 ;
	dir = 1 ;
	if( ew->pos.x >= pl_w->pos.x ){
		ew->disp_dir = 0 ;
		ew->spd.x = -196608.0f ;
	} else {
		ew->disp_dir = 1 ;
		ew->spd.x = 196608.0f ;
	}
	ew->acc.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.y = 0.0f ;
	if( ew->disp_dir == pl_w->disp_dir ){
		pl_w->timer1 = dir ;
	}
	pl_char_set( ew, 25, 5 ) ;
	pls1c_03_win_join_run( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_03_win_join_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->prio = pl_w->prio ;
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	if( pl_w->act_st != N_WIN ){
		ew->routine_3 = 4 ;
		return ;
	}
	if( !ew->disp_dir ){
		if( ew->pos.x <= pl_w->pos.x + 80.0f ){
			ew->routine_3 ++ ;
			pl_w->timer0 = 0 ;
		}
	} else {
		if( pl_w->pos.x + -80.0f <= ew->pos.x ){
			ew->routine_3 ++ ;
			pl_w->timer0 = 0 ;
		}
	}
}

/*--------------------------------------------------------------*/
static void pls1c_03_win_join_near( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float k ;

	ew->prio = pl_w->prio ;
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	if( pl_w->act_st != N_WIN ){
		ew->routine_3 = 4 ;
		return ;
	}
	if( !ew->disp_dir ){
		k = DOT_X( 40.0f ) ;
		if( ew->pos.x > pl_w->pos.x + k ){
			return ;
		}
	} else {
		k = -DOT_X( 40.0f ) ;
		if( pl_w->pos.x + k > ew->pos.x ){
			return ;
		}
	}
	ew->routine_3 ++ ;
	ew->pos.x = pl_w->pos.x + k ;
	pl_char_set( ew, 25, 7 ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_03_win_join_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->prio = pl_w->prio ;
	if( pl_w->act_st != N_WIN ){
		pl_char_set( ew, 25, 5 ) ;
		ew->routine_3 = 4 ;
		return ;
	}
	if( pl_w->cg_type ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		ew->be_flag = 0 ;
	}
	pl_char_move( ew ) ;
	if( ew->cg_type ){
		ew->disp_dir = pl_w->disp_dir ;
		ew->pos.x = pl_w->pos.x ;
	}
}

/*--------------------------------------------------------------*/
static void pls1c_03_win_join_leave( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	if( !scr_in_x_ck( ew ) ){
		ew->routine_0 ++ ;
	}
}

static void pls1c_03_win_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_win_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_win_pose( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void pls1c_03_win( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls1c_03_win_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_03_win_init, pls1c_03_win_move, pls1c_03_win_pose,
	} ;

	if( pl_w->act_st != N_WIN ){
		pls1c_03_next( ew, pl_w ) ;
		return ;
	}
	pls1c_03_win_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls1c_03_win_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->spd.x = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.y = 0.0f ;
	if( pl_w->pos.x <= ew->pos.x ){
		ew->disp_dir = 0 ;
	} else {
		ew->disp_dir = 1 ;
	}
	pl_char_set( ew, 25, 14 ) ;
	pls1c_03_win_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_03_win_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( ew->cg_type ){
		ew->routine_3 ++ ;
		pls1c_00_set( ew, 51 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls1c_03_win_pose( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
}

static void pls1c_03_t16_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_t16_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void pls1c_03_t16( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls1c_03_t16_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_03_t16_init, pls1c_03_t16_move, pls1c_03_win_pose,
	} ;

	if( pl_w->act_st != N_WIN ){
		pls1c_03_next( ew, pl_w ) ;
		return ;
	}
	pls1c_03_t16_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls1c_03_t16_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->spd.x = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.y = 0.0f ;
	if( pl_w->pos.x <= ew->pos.x ){
		ew->disp_dir = 0 ;
	} else {
		ew->disp_dir = 1 ;
	}
	pl_char_set( ew, 25, 14 ) ;
	pls1c_03_t16_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_03_t16_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( ew->cg_type ){
		ew->routine_3 ++ ;
		pls1c_00_set( ew, 51 ) ;
	}
}

static void pls1c_03_join_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_join_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_join_ride( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_join_back( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Flies to the player and joins it (Rush Drill and	*/
/*			Hyper Mega Man)										*/
/*==============================================================*/
static void pls1c_03_join( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls1c_03_join_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_03_join_init, pls1c_03_join_fly, pls1c_03_join_ride, pls1c_03_join_back,
	} ;
	PL1C_LOCAL_W *lw = PL1C_LW( pl_w ) ;

	if( pl_w->routine_1 || pl_w->act_st != N_SUPER ){
		lw->join_sp = 0 ;
		lw->apart_flag = 1 ;
	}
	pls1c_03_join_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls1c_03_join_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->spd.x = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.y = 0.0f ;
	if( pl_w->pos.x <= ew->pos.x ){
		ew->disp_dir = 0 ;
	} else {
		ew->disp_dir = 1 ;
	}
	pl_char_set( ew, 25, 0 ) ;
	pls1c_03_join_fly( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_03_join_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL1C_LOCAL_W *lw = PL1C_LW( pl_w ) ;

	if( !lw->join_sp ){
		ew->pos.y = pl_w->ground_y ;
		pls1c_03_next( ew, pl_w ) ;
		return ;
	}
	if( lw->apart_flag ){
		ew->pos.x += ( pl_w->pos.x - ew->pos.x ) / 8.0f ;
		ew->pos.y += ( pl_w->pos.y - ew->pos.y ) / 8.0f ;
	} else {
		ew->routine_3 ++ ;
		ew->be_flag = 0 ;
		ew->disp_flag = 0 ;
		ew->be_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void pls1c_03_join_ride( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL1C_LOCAL_W *lw = PL1C_LW( pl_w ) ;

	if( !lw->join_sp || lw->apart_flag ){
		ew->routine_3 ++ ;
		ew->be_flag = 1 ;
		ew->disp_flag = 1 ;
		ew->disp_dir = pl_w->disp_dir ;
		ew->pos = pl_w->pos ;
	}
}

/*--------------------------------------------------------------*/
static void pls1c_03_join_back( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL1C_LOCAL_W *lw = PL1C_LW( pl_w ) ;

	if( !lw->join_sp ){
		ew->pos.y = pl_w->ground_y ;
		pls1c_03_next( ew, pl_w ) ;
		return ;
	}
	if( !ew->disp_dir ){
		ew->pos.x += ( pl_w->pos.x - ew->pos.x + 80.0f ) / 8.0f ;
	} else {
		ew->pos.x += ( pl_w->pos.x - ew->pos.x + -80.0f ) / 8.0f ;
	}
	ew->pos.y += ( pl_w->ground_y - ew->pos.y ) / 8.0f ;
}

static void pls1c_03_away_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_away_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_away_up( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_away_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_away_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_03_away_land( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Flies off, and comes down again when it is over		*/
/*			(Beat Plane)										*/
/*==============================================================*/
static void pls1c_03_away( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls1c_03_away_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_03_away_init, pls1c_03_away_start, pls1c_03_away_up,
		pls1c_03_away_wait, pls1c_03_away_fall, pls1c_03_away_land,
	} ;
	PL1C_LOCAL_W *lw = PL1C_LW( pl_w ) ;

	if( pl_w->routine_1 || pl_w->act_st != N_SUPER ){
		lw->join_sp = 0 ;
	}
	pls1c_03_away_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls1c_03_away_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->spd.x = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = DOT_Y( 8.0f ) ;
	ew->acc.y = 0.0f ;
	pl_char_set( ew, 25, 15 ) ;
	pls1c_03_away_up( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_03_away_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( ew->cg_type ){
		ew->routine_3 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void pls1c_03_away_up( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !scr_in_y_ck( ew ) ){
		ew->routine_3 ++ ;
		ew->be_flag = 0 ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void pls1c_03_away_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL1C_LOCAL_W *lw = PL1C_LW( pl_w ) ;
	if( lw->join_sp ){
		return ;
	}
	ew->routine_3 ++ ;
	lw->join_sp = 0 ;
	ew->be_flag = 1 ;
	ew->disp_flag = 1 ;
	ew->disp_dir = pl_w->disp_dir ;
	ew->pos.x = pl_w->pos.x + ( !pl_w->disp_dir ? 80.0f : -80.0f ) ;
	ew->pos.y = scr_work.scr_top + DOT_Y( 16.0f ) ;
	ew->spd.x = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = -DOT_Y( 8.0f ) ;
	ew->acc.y = -DOT_Y( 0.375f ) ;
	pl_char_set( ew, 25, 12 ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_03_away_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y > pl_w->ground_y ){
		return ;
	}
	ew->routine_3 ++ ;
	ew->pos.y = pl_w->ground_y ;
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_03_away_land( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		pls1c_03_next( ew, pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls1c_03_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	ew->be_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls1c_03_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
static int pls1c_03_bit_ck( Sint16 flag, Sint16 no )
{
	if( flag & ( 1 << no ) ){
		return 1 ;
	}
	return 0 ;
}

/*==============================================================*/
/*			The place dist steps in front of the work			*/
/*==============================================================*/
static Float pls1c_03_x_set( PLAYER_WORK *ew, Sint16 dist )
{
	Float x = DOT_X( dist ) ;

	if( !ew->disp_dir ){
		return ew->pos.x + x ;
	}
	return ew->pos.x - x ;
}

/*==============================================================*/
/*			Picks the next action								*/
/*==============================================================*/
static void pls1c_03_next( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Sint16 pls1c_03_next_tbl[4] = { 2, 2, 3, 3 } ;
	Uint8 no = 0 ;
	PL1C_LOCAL_W *lw = PL1C_LW( pl_w ) ;
	VEC2 d ;

	ew->timer1 = 4 ;
	if( pl_w->team_no || ( pls1c_03_bit_ck( vs_sys_work.team_hyper_flag, pl_w->pl_no ) && vs_sys_work.vs_st >= VS_END_WAIT ) ){
		PLS1C_03_SAVE( ew ) ;
		ew->routine_2 = 1 ;
		ew->routine_3 = 0 ;
		return ;
	}
	if( lw->join_sp ){
		switch( lw->join_sp ){
		case 2 :
			PLS1C_03_SAVE( ew ) ;
			ew->routine_2 = 12 ;
			ew->routine_3 = 0 ;
			return ;
		case 4 :
			PLS1C_03_SAVE( ew ) ;
			ew->routine_2 = 13 ;
			ew->routine_3 = 0 ;
			return ;
		case 6 :
		default :
			PLS1C_03_SAVE( ew ) ;
			ew->routine_2 = 14 ;
			ew->routine_3 = 0 ;
			return ;
		}
	}
	if( !scr_in_x_ck( ew ) ){
		PLS1C_03_SAVE( ew ) ;
		ew->routine_2 = 4 ;
		ew->routine_3 = 0 ;
	}
	else {
		if( pl_w->combo_dm_cnt >= 3 ) goto down ;
		if( lw->charge_flag ){
			PLS1C_03_SAVE( ew ) ;
			ew->routine_2 = 10 ;
			ew->routine_3 = 0 ;
			return ;
		}
		if( pl_w->act_st == N_WIN && pl_w->type >= 5 && pl_w->type != 10 ){
			switch( pl_w->type ){
			case 6 :
				PLS1C_03_SAVE( ew ) ;
				ew->routine_2 = 8 ;
				ew->routine_3 = 0 ;
				break ;
			case 9 :
				PLS1C_03_SAVE( ew ) ;
				ew->routine_2 = 16 ;
				ew->routine_3 = 0 ;
				break ;
			default :
				PLS1C_03_SAVE( ew ) ;
				ew->routine_2 = 9 ;
				ew->routine_3 = 0 ;
				break ;
			}
			return ;
		}
		if( pl_w->vital_new <= 0 ){
down:
			PLS1C_03_SAVE( ew ) ;
			ew->routine_2 = 15 ;
			ew->routine_3 = 0 ;
			return ;
		}
		d.x = pls1c_03_x_set( pl_w, 48 ) - ew->pos.x ;
		d.y = pl_w->pos.y ;
		if( d.y >= ew->pos.y + DOT_Y( 48.0f ) ) no = 1 ;
		if( d.x < 0.0f ) d.x *= -1.0f ;
		if( d.x >= DOT_X( 40.0f ) ) no += 2 ;
		PLS1C_03_SAVE( ew ) ;
		ew->routine_2 = pls1c_03_next_tbl[no] ;
		ew->routine_3 = 0 ;
	}
}

/****************************************************************/
void pls1c_03_color_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w, COLOR_STEP *step )
{
	PLS1C_03_LOCAL_W *lw = PLS1C_03_LW( ew ) ;
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

/****************************************************************/
int pls1c_03_color_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS1C_03_LOCAL_W *lw = PLS1C_03_LW( ew ) ;
	int ret = 1 ;

	if( !--lw->time ){
		if( lw->last ) ret = 0 ;
		pls1c_03_color_start( ew, pl_w, lw->step ) ;
	}
	return ret ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls1c_03.c" End of File	*/

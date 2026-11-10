/* Mega Man's player effect 07: the helpers and the other pictures of Mega Man's and Roll's
   hyper combos, wins and appearances, one routine for each kind (the type), its steps in routine_3. */
#include "player.h"
#include "pl_char.h"
#include "pl1c_def.h"

/* the effect's own work: the colour cycle it runs on the player */
typedef struct PLS1C_07_LOCAL_W {
	COLOR_STEP	*step ;		/* the next step */
	Uint8	time ;			/* the step's time left */
	Uint8	last ;			/* set on the cycle's last step */
} PLS1C_07_LOCAL_W ;
#define PLS1C_07_LW( w )	( ( PLS1C_07_LOCAL_W * )&( w )->obj_flag )

/* the colour cycle when the partner comes down */
static COLOR_STEP pls1c_07_col[6] = {
	{ 4, 1, 19, 1, 1 },
	{ 4, 1, 18, 1, 1 },
	{ 4, 1, 17, 1, 1 },
	{ 4, 1, 16, 1, 1 },
	{ 4, 1, 15, 1, 1 },
	{ 16, 1, 14, 1, 0 },
} ;

static void pls1c_07_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls1c_07_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls1c_07_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
	}
	return ew ;
}

/****************************************************************/
PLAYER_WORK *pls1c_07_set2( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS_ST, LIST_TOP ) ) ){
		ew->work_func = pls1c_07_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
	}
	return ew ;
}

static void pls1c_07_init( PLAYER_WORK *ew );
static void pls1c_07_move( PLAYER_WORK *ew );
static void pls1c_07_end( PLAYER_WORK *ew );
static void pls1c_07_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls1c_07_main( PLAYER_WORK *ew )
{
	static void ( *pls1c_07_jmp[] )( PLAYER_WORK *ew ) = {
		pls1c_07_init, pls1c_07_move, pls1c_07_end, pls1c_07_free,
	} ;

	pls1c_07_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls1c_07_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	pl_w = ew->master_w ;
	ew->routine_0 ++ ;
	ew->ef_no = 0x1c07 ;
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
	ew->be_flag = 1 ;
	ew->disp_flag = 1 ;
	ew->prio = 7 ;
	pls1c_07_move( ew ) ;
}

static void pls1c_07_beat( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_eddie( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_rush( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t5( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t6( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t7( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t8( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t9( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t10( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t11( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t12( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t13( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void pls1c_07_move( PLAYER_WORK *ew )
{
	static void ( *pls1c_07_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_07_beat, pls1c_07_beat, pls1c_07_eddie, pls1c_07_rush, pls1c_07_rush, pls1c_07_t5,
		pls1c_07_t6, pls1c_07_t7, pls1c_07_t8, pls1c_07_t9, pls1c_07_t10, pls1c_07_t11,
		pls1c_07_t12, pls1c_07_t13,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	pls1c_07_move_jmp[ew->type]( ew, pl_w ) ;
}

static void pls1c_07_beat_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_beat_in( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_beat_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_beat_join( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_beat_ride( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_beat_leave( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls1c_07_beat( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls1c_07_beat_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_07_beat_init, pls1c_07_beat_in, pls1c_07_beat_wait, pls1c_07_beat_join, pls1c_07_beat_ride,
		pls1c_07_beat_leave,
	} ;
	PL1C_LOCAL_W *lw = PL1C_LW( pl_w ) ;

	if( pl_w->routine_1 || pl_w->act_st != N_SUPER ){
		lw->join_sp = 0 ;
		lw->apart_flag = 1 ;
	}
	pls1c_07_beat_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
/*			Comes in from the screen's edge						*/
/*==============================================================*/
static void pls1c_07_beat_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->size_top = 16 ;
	ew->size_bottom = 16 ;
	ew->size_front = 16 ;
	ew->size_back = 16 ;
	ew->timer0 = 16 ;
	if( !( ew->disp_dir = pl_w->disp_dir ) ){
		ew->pos.x = scr_work.scr_right + 80.0f ;
		ew->spd.x = -DOT_X( 4.0f ) ;
	} else {
		ew->pos.x = scr_work.scr_left + -80.0f ;
		ew->spd.x = DOT_X( 4.0f ) ;
	}
	ew->pos.y = scr_work.scr_top + -DOT_Y( 96.0f ) ;
	ew->acc.x = 0 ;
	ew->spd.y = 0 ;
	ew->acc.y = 0 ;
	pl_char_set( ew, 25, 19 ) ;
	ew->be_flag = 1 ;
	pl_shadow_set( ew ) ;
	pls1c_07_beat_in( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_07_beat_in( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !-- ew->timer0 ){
		ew->routine_3 ++ ;
		ew->timer0 = 32 ;
	}
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_07_beat_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !( vs_sys_work.game_stop & ( 1 << ( pl_w->pl_no ^ 1 ) ) ) ){
		if( !-- ew->timer0 ){
			ew->routine_3 ++ ;
		}
		pl_char_move( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls1c_07_beat_join( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL1C_LOCAL_W *lw = PL1C_LW( pl_w ) ;

	if( !lw->join_sp ){
		ew->routine_3 = 5 ;
		ew->be_flag = 1 ;
		ew->disp_flag = 1 ;
		if( !ew->disp_dir ){
			ew->spd.x = -DOT_X( 4.0f ) ;
		} else {
			ew->spd.x = DOT_X( 4.0f ) ;
		}
		ew->acc.x = 0 ;
		ew->spd.y = DOT_Y( 2.0f ) ;
		ew->acc.y = 0 ;
	} else if( !lw->apart_flag ){
		ew->routine_3 ++ ;
		ew->be_flag = 0 ;
		ew->disp_flag = 0 ;
		return ;
	} else {
		ew->pos.x += ( pl_w->pos.x - ew->pos.x ) / 8.0f ;
		ew->pos.y += ( pl_w->pos.y - ew->pos.y + DOT_Y( 40.0f ) ) / 8.0f ;
	}
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_07_beat_ride( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL1C_LOCAL_W *lw = PL1C_LW( pl_w ) ;

	if( !lw->join_sp || lw->apart_flag ){
		ew->disp_dir = pl_w->disp_dir ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y + DOT_Y( 40.0f ) ;
		ew->routine_3 = 5 ;
		ew->be_flag = 1 ;
		ew->disp_flag = 1 ;
		if( !ew->disp_dir ){
			ew->spd.x = -DOT_X( 4.0f ) ;
		} else {
			ew->spd.x = DOT_X( 4.0f ) ;
		}
		ew->acc.x = 0 ;
		ew->spd.y = DOT_Y( 2.0f ) ;
		ew->acc.y = 0 ;
	}
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_07_beat_leave( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !scr_in_ck( ew ) ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	pl_char_move( ew ) ;
}

static void pls1c_07_eddie_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_eddie_in( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_eddie_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_eddie_join( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_eddie_ride( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_eddie_leave( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls1c_07_eddie( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls1c_07_eddie_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_07_eddie_init, pls1c_07_eddie_in, pls1c_07_eddie_wait, pls1c_07_eddie_join, pls1c_07_eddie_ride,
		pls1c_07_eddie_leave,
	} ;
	PL1C_LOCAL_W *lw = PL1C_LW( pl_w ) ;

	if( pl_w->routine_1 || pl_w->act_st != N_SUPER ){
		lw->join_sp = 0 ;
		lw->apart_flag = 1 ;
	}
	pls1c_07_eddie_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
/*			Comes in from the screen's edge, facing the			*/
/*			other way											*/
/*==============================================================*/
static void pls1c_07_eddie_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->size_top = 16 ;
	ew->size_bottom = 16 ;
	ew->size_front = 16 ;
	ew->size_back = 16 ;
	ew->timer0 = 16 ;
	if( !( ew->disp_dir = pl_w->disp_dir ^ 1 ) ){
		ew->pos.x = scr_work.scr_right + 80.0f ;
		ew->spd.x = -DOT_X( 4.0f ) ;
	} else {
		ew->pos.x = scr_work.scr_left + -80.0f ;
		ew->spd.x = DOT_X( 4.0f ) ;
	}
	ew->pos.y = scr_work.scr_top + -DOT_Y( 96.0f ) ;
	ew->acc.x = 0 ;
	ew->spd.y = 0 ;
	ew->acc.y = 0 ;
	pl_char_set( ew, 25, 20 ) ;
	ew->be_flag = 1 ;
	pl_shadow_set( ew ) ;
	pls1c_07_eddie_in( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_07_eddie_in( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !-- ew->timer0 ){
		ew->routine_3 ++ ;
		ew->timer0 = 32 ;
	}
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_07_eddie_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !( vs_sys_work.game_stop & ( 1 << ( pl_w->pl_no ^ 1 ) ) ) ){
		if( !-- ew->timer0 ){
			ew->routine_3 ++ ;
		}
		pl_char_move( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls1c_07_eddie_join( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL1C_LOCAL_W *lw = PL1C_LW( pl_w ) ;

	if( !lw->join_sp ){
		ew->routine_3 = 5 ;
		ew->be_flag = 1 ;
		ew->disp_flag = 1 ;
		if( !ew->disp_dir ){
			ew->spd.x = -DOT_X( 4.0f ) ;
		} else {
			ew->spd.x = DOT_X( 4.0f ) ;
		}
		ew->acc.x = 0 ;
		ew->spd.y = DOT_Y( 2.0f ) ;
		ew->acc.y = 0 ;
	} else if( !lw->apart_flag ){
		ew->routine_3 ++ ;
		ew->be_flag = 0 ;
		ew->disp_flag = 0 ;
		return ;
	} else {
		ew->pos.x += ( pl_w->pos.x - ew->pos.x ) / 8.0f ;
		ew->pos.y += ( pl_w->pos.y - ew->pos.y + DOT_Y( 40.0f ) ) / 8.0f ;
	}
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_07_eddie_ride( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL1C_LOCAL_W *lw = PL1C_LW( pl_w ) ;

	if( !lw->join_sp || lw->apart_flag ){
		ew->disp_dir = pl_w->disp_dir ^ 1 ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y + DOT_Y( 48.0f ) ;
		ew->routine_3 = 5 ;
		ew->be_flag = 1 ;
		ew->disp_flag = 1 ;
		if( !ew->disp_dir ){
			ew->spd.x = -DOT_X( 4.0f ) ;
		} else {
			ew->spd.x = DOT_X( 4.0f ) ;
		}
		ew->acc.x = 0 ;
		ew->spd.y = DOT_Y( 2.0f ) ;
		ew->acc.y = 0 ;
	}
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_07_eddie_leave( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !scr_in_ck( ew ) ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	pl_char_move( ew ) ;
}

static void pls1c_07_rush_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_rush_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_rush_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_rush_join( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_rush_ride( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_rush_out( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_rush_turn( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_rush_turn_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_rush_leave( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls1c_07_rush( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls1c_07_rush_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_07_rush_init, pls1c_07_rush_fall, pls1c_07_rush_wait, pls1c_07_rush_join, pls1c_07_rush_ride,
		pls1c_07_rush_out, pls1c_07_rush_turn, pls1c_07_rush_turn_end, pls1c_07_rush_leave,
	} ;
	PL1C_LOCAL_W *lw = PL1C_LW( pl_w ) ;

	if( pl_w->routine_1 || pl_w->act_st != N_SUPER ){
		lw->join_sp = 0 ;
		lw->apart_flag = 1 ;
	}
	pls1c_07_rush_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
/*			Drops in from above the screen's edge				*/
/*==============================================================*/
static void pls1c_07_rush_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->size_top = 32 ;
	ew->size_bottom = 32 ;
	ew->size_front = 32 ;
	ew->size_back = 32 ;
	if( !( ew->disp_dir = pl_w->disp_dir ) ){
		ew->pos.x = scr_work.scr_right + 80.0f ;
		ew->spd.x = -DOT_X( 4.0f ) ;
	} else {
		ew->pos.x = scr_work.scr_left + -80.0f ;
		ew->spd.x = DOT_X( 4.0f ) ;
	}
	ew->pos.y = pl_w->ground_y + DOT_Y( 32.0f ) ;
	ew->acc.x = 0 ;
	ew->spd.y = 2.142857f ;
	ew->acc.y = -DOT_Y( 0.375f ) ;
	pl_char_set( ew, 25, 16 ) ;
	ew->be_flag = 1 ;
	pl_shadow_set( ew ) ;
	pls1c_07_rush_fall( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_07_rush_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y <= pl_w->ground_y ){
		ew->routine_3 ++ ;
		ew->pos.y = pl_w->ground_y ;
		ew->timer0 = 32 ;
		pl_char_move( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls1c_07_rush_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !( vs_sys_work.game_stop & ( 1 << ( pl_w->pl_no ^ 1 ) ) ) ){
		if( !-- ew->timer0 ){
			ew->routine_3 ++ ;
		}
		pl_char_move( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls1c_07_rush_join( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL1C_LOCAL_W *lw = PL1C_LW( pl_w ) ;

	if( !lw->join_sp ){
		ew->routine_3 = 5 ;
		ew->be_flag = 1 ;
		ew->disp_flag = 1 ;
		ew->timer0 = 32 ;
		pl_char_set( ew, 25, 17 ) ;
	} else if( !lw->apart_flag ){
		ew->routine_3 ++ ;
		ew->be_flag = 0 ;
		ew->disp_flag = 0 ;
		return ;
	} else {
		ew->pos.x += ( pl_w->pos.x - ew->pos.x ) / 8.0f ;
		ew->pos.y += ( pl_w->pos.y - ew->pos.y ) / 8.0f ;
	}
}

/*--------------------------------------------------------------*/
static void pls1c_07_rush_ride( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL1C_LOCAL_W *lw = PL1C_LW( pl_w ) ;

	if( !lw->join_sp || lw->apart_flag ){
		ew->disp_dir = pl_w->disp_dir ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		ew->routine_3 = 5 ;
		ew->be_flag = 1 ;
		ew->disp_flag = 1 ;
		ew->timer0 = 32 ;
		pl_char_set( ew, 25, 17 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls1c_07_rush_out( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float x ;

	if( !-- ew->timer0 ){
		ew->routine_3 ++ ;
		ew->pos.y = pl_w->ground_y ;
		pl_char_move( ew ) ;
		return ;
	}
	if( !ew->disp_dir ) x = pl_w->pos.x + 80.0f ;
	else x = pl_w->pos.x + -80.0f ;
	ew->pos.x += ( x - ew->pos.x ) / 8.0f ;
	ew->pos.y += ( pl_w->ground_y - ew->pos.y ) / 8.0f ;
}

/*--------------------------------------------------------------*/
static void pls1c_07_rush_turn( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( ew->cg_type ){
		ew->routine_3 ++ ;
		ew->disp_dir ^= 1 ;
	}
}

/*--------------------------------------------------------------*/
static void pls1c_07_rush_turn_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_3 ++ ;
		if( !ew->disp_dir ){
			ew->spd.x = -DOT_X( 4.25f ) ;
		} else {
			ew->spd.x = DOT_X( 4.25f ) ;
		}
		ew->acc.x = 0 ;
		ew->spd.y = 0 ;
		ew->acc.y = 0 ;
		pl_char_set( ew, 25, 5 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls1c_07_rush_leave( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !scr_in_ck( ew ) ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	pl_char_move( ew ) ;
}

static void pls1c_07_t5_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t5_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t5_pose( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t5_leave( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls1c_07_t5( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls1c_07_t5_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_07_t5_init, pls1c_07_t5_run, pls1c_07_t5_pose, pls1c_07_t5_leave,
	} ;

	pls1c_07_t5_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
/*			Comes in at the player's height from the			*/
/*			screen's edge, facing the other way					*/
/*==============================================================*/
static void pls1c_07_t5_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->size_top = 16 ;
	ew->size_bottom = 16 ;
	ew->size_front = 16 ;
	ew->size_back = 16 ;
	if( !( ew->disp_dir = pl_w->disp_dir ^ 1 ) ){
		ew->pos.x = scr_work.scr_right + 80.0f ;
		ew->spd.x = -DOT_X( 4.0f ) ;
	} else {
		ew->pos.x = scr_work.scr_left + -80.0f ;
		ew->spd.x = DOT_X( 4.0f ) ;
	}
	ew->pos.y = pl_w->ground_y ;
	ew->acc.x = 0 ;
	ew->spd.y = 0 ;
	ew->acc.y = 0 ;
	pl_char_set( ew, 25, 21 ) ;
	ew->be_flag = 1 ;
	pl_shadow_set( ew ) ;
	pls1c_07_t5_run( ew, pl_w ) ;
}

/*==============================================================*/
/*			Runs in until it is at the player's side, as		*/
/*			long as the player is in the win					*/
/*==============================================================*/
static void pls1c_07_t5_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float x ;

	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( pl_w->act_st != N_WIN ){
		ew->routine_3 = 3 ;
		return ;
	}
	x = pl_w->pos.x + ( !ew->disp_dir ? 80.0f : -80.0f ) ;
	if( !ew->disp_dir && ew->pos.x <= x ){
		ew->routine_3 ++ ;
		ew->pos.x = x ;
		pl_w->timer0 = 0 ;
		pl_char_set( ew, 25, 25 ) ;
	} else if( ew->disp_dir && ew->pos.x >= x ){
		ew->routine_3 ++ ;
		ew->pos.x = x ;
		pl_w->timer0 = 0 ;
		pl_char_set( ew, 25, 25 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls1c_07_t5_pose( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_07_t5_leave( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !scr_in_ck( ew ) ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	pl_char_move( ew ) ;
}

static void pls1c_07_t6_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t6_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t6_near( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t6_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t6_leave( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls1c_07_t6( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls1c_07_t6_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_07_t6_init, pls1c_07_t6_run, pls1c_07_t6_near, pls1c_07_t6_wait, pls1c_07_t6_leave,
	} ;

	pls1c_07_t6_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls1c_07_t6_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->size_top = 16 ;
	ew->size_bottom = 16 ;
	ew->size_front = 16 ;
	ew->size_back = 16 ;
	ew->prio = 0 ;
	if( !( ew->disp_dir = pl_w->disp_dir ^ 1 ) ){
		ew->pos.x = scr_work.scr_right + 80.0f ;
		ew->spd.x = -DOT_X( 4.0f ) ;
	} else {
		ew->pos.x = scr_work.scr_left + -80.0f ;
		ew->spd.x = DOT_X( 4.0f ) ;
	}
	ew->pos.y = pl_w->ground_y ;
	ew->pos.y += DOT_Y( 2.0f ) ;
	ew->acc.x = 0 ;
	ew->spd.y = 0 ;
	ew->acc.y = 0 ;
	pl_char_set( ew, 25, 21 ) ;
	ew->be_flag = 1 ;
	pl_shadow_set( ew ) ;
	pls1c_07_t6_run( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_07_t6_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float x ;

	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( pl_w->act_st != N_WIN ){
		ew->routine_3 = 4 ;
		return ;
	}
	x = pl_w->pos.x + ( !ew->disp_dir ? 80.0f : -80.0f ) ;
	if( !ew->disp_dir && ew->pos.x <= x ){
		ew->routine_3 ++ ;
		pl_w->timer0 = 0 ;
	} else if( ew->disp_dir && ew->pos.x >= x ){
		ew->routine_3 ++ ;
		pl_w->timer0 = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void pls1c_07_t6_near( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float x ;

	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( pl_w->act_st != N_WIN ){
		ew->routine_3 = 4 ;
		return ;
	}
	x = pl_w->pos.x + ( !ew->disp_dir ? DOT_X( 31.0f ) : -DOT_X( 31.0f ) ) ;
	if( !ew->disp_dir && ew->pos.x <= x ){
		ew->routine_3 ++ ;
		ew->pos.x = x ;
		pl_w->timer0 = 0 ;
		pl_char_set( ew, 25, 20 ) ;
	} else if( ew->disp_dir && ew->pos.x >= x ){
		ew->routine_3 ++ ;
		ew->pos.x = x ;
		pl_w->timer0 = 0 ;
		pl_char_set( ew, 25, 20 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls1c_07_t6_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->act_st != N_WIN ){
		ew->routine_3 = 4 ;
		pl_char_set( ew, 25, 21 ) ;
		return ;
	}
	if( pl_w->cg_type ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_07_t6_leave( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !scr_in_ck( ew ) ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	pl_char_move( ew ) ;
}

static void pls1c_07_t7_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t7_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t7_pose( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t7_leave( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls1c_07_t7( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls1c_07_t7_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_07_t7_init, pls1c_07_t7_run, pls1c_07_t7_pose, pls1c_07_t7_leave,
	} ;

	pls1c_07_t7_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls1c_07_t7_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->size_top = 16 ;
	ew->size_bottom = 16 ;
	ew->size_front = 16 ;
	ew->size_back = 16 ;
	ew->sub_prio = -1 ;
	if( !( ew->disp_dir = pl_w->disp_dir ^ 1 ) ){
		ew->pos.x = scr_work.scr_right + 80.0f ;
		ew->spd.x = -DOT_X( 4.0f ) ;
	} else {
		ew->pos.x = scr_work.scr_left + -80.0f ;
		ew->spd.x = DOT_X( 4.0f ) ;
	}
	ew->pos.y = pl_w->ground_y ;
	ew->acc.x = 0 ;
	ew->spd.y = 0 ;
	ew->acc.y = 0 ;
	pl_char_set( ew, 25, 26 ) ;
	pls1c_00_set2( ew, ew, 52 ) ;
	ew->be_flag = 1 ;
	pl_shadow_set( ew ) ;
	pls1c_07_t7_run( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_07_t7_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float x ;

	ew->prio = pl_w->prio ;
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( pl_w->act_st != N_WIN ){
		ew->routine_3 = 3 ;
		return ;
	}
	x = pl_w->pos.x + ( !ew->disp_dir ? 90.0f : -90.0f ) ;
	if( !ew->disp_dir && ew->pos.x <= x ){
		ew->routine_3 ++ ;
		ew->pos = pl_w->pos ;
		ew->disp_dir ^= 1 ;
		pl_w->timer0 = 0 ;
		pl_char_set( ew, 25, 27 ) ;
	} else if( ew->disp_dir && ew->pos.x >= x ){
		ew->routine_3 ++ ;
		ew->pos = pl_w->pos ;
		ew->disp_dir ^= 1 ;
		pl_w->timer0 = 0 ;
		pl_char_set( ew, 25, 27 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls1c_07_t7_pose( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->prio = pl_w->prio ;
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_07_t7_leave( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !scr_in_ck( ew ) ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	pl_char_move( ew ) ;
}

static void pls1c_07_t8_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t8_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t8_color( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t8_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t8_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls1c_07_t8( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls1c_07_t8_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_07_t8_init, pls1c_07_t8_fall, pls1c_07_t8_color, pls1c_07_t8_move, pls1c_07_t8_end,
	} ;

	pls1c_07_t8_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
/*			Drops in beside the player							*/
/*==============================================================*/
static void pls1c_07_t8_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->pos.x = pl_w->pos.x + ( !( ew->disp_dir = pl_w->disp_dir ^ 1 ) ? 80.0f : -80.0f ) ;
	ew->pos.y = scr_work.scr_top + DOT_Y( 16.0f ) ;
	ew->spd.x = 0 ;
	ew->acc.x = 0 ;
	ew->spd.y = -DOT_Y( 2.0f ) ;
	ew->acc.y = -DOT_Y( 0.375f ) ;
	pl_char_set( ew, 25, 18 ) ;
	ew->be_flag = 1 ;
	pl_shadow_set( ew ) ;
	pls1c_07_t8_fall( ew, pl_w ) ;
}

static void pls1c_07_color_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w, COLOR_STEP *step );
static int pls1c_07_color_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void pls1c_07_t8_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y <= pl_w->ground_y ){
		ew->routine_3 ++ ;
		ew->pos.y = pl_w->ground_y ;
		pl_w->timer0 = 0 ;
		pls1c_07_color_start( ew, pl_w, pls1c_07_col ) ;
		pls1c_07_color_move( ew, pl_w ) ;
		pl_char_move( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls1c_07_t8_color( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !pls1c_07_color_move( ew, pl_w ) ){
		ew->routine_3 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void pls1c_07_t8_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( ew->cg_type ){
		ew->routine_3 ++ ;
		pl_w->timer0 = 0 ;
		pls1c_00_set2( ew, ew, 7 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls1c_07_t8_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
}

static void pls1c_07_t9_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t9_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t9_stop( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t9_float( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls1c_07_t9( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls1c_07_t9_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_07_t9_init, pls1c_07_t9_fall, pls1c_07_t9_stop, pls1c_07_t9_float,
	} ;

	pls1c_07_t9_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
/*			Comes down at the player's side with sparks			*/
/*			(four of the picture's other kind)					*/
/*==============================================================*/
static void pls1c_07_t9_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->prio = 12 ;
	ew->col_flag = 1 ;
	ew->col_no ++ ;
	ew->pos.x = pl_w->pos.x + ( !( ew->disp_dir = pl_w->disp_dir ^ 1 ) ? DOT_X( 80.0f ) : -DOT_X( 80.0f ) ) ;
	ew->pos.y = scr_work.scr_top + DOT_Y( 16.0f ) ;
	ew->spd.x = 0 ;
	ew->acc.x = 0 ;
	ew->spd.y = -DOT_Y( 4.0f ) ;
	ew->acc.y = 0 ;
	pl_char_set( ew, 22, 40 ) ;
	pls1c_00_set2( ew, ew, 53 ) ;
	pls1c_00_set2( ew, ew, 54 ) ;
	pls1c_00_set2( ew, ew, 55 ) ;
	pls1c_00_set2( ew, ew, 56 ) ;
	ew->be_flag = 1 ;
	pl_shadow_set( ew ) ;
	pls1c_07_t9_fall( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_07_t9_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float y ;

	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	y = pl_w->ground_y + DOT_Y( 24.0f ) ;
	if( ew->pos.y <= y ){
		ew->routine_3 ++ ;
		ew->timer0 = 8 ;
		ew->acc.y = 2.142857f ;
		pl_w->timer0 = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void pls1c_07_t9_stop( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !-- ew->timer0 ){
		ew->routine_3 ++ ;
		ew->dir = 48 ;
		ew->acc.z = ew->pos.y ;
	}
	pl_char_move( ew ) ;
}

/*==============================================================*/
/*			Swings up and down on a sine of dir					*/
/*==============================================================*/
static void pls1c_07_t9_float( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float s ;
	Sint16 rad ;

	ew->pos.y = ew->acc.z ;
	s = 204800.0f ;
	rad = ( ( 80 - ew->dir ) & 63 ) << 10 ;
	ew->pos.y += s * nlSin( rad ) * 1000.0f / 125000.0f / 256.0f * 2.142857f ;
	ew->dir ++ ;
	ew->dir &= 63 ;
	pl_char_move( ew ) ;
}

static void pls1c_07_t10_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t10_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t10_stop( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t10_pose( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t10_leave( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls1c_07_t10( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls1c_07_t10_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_07_t10_init, pls1c_07_t10_run, pls1c_07_t10_stop, pls1c_07_t10_pose, pls1c_07_t10_leave,
	} ;

	pls1c_07_t10_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls1c_07_t10_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->size_top = 16 ;
	ew->size_bottom = 16 ;
	ew->size_front = 24 ;
	ew->size_back = 24 ;
	ew->col_flag = 1 ;
	ew->col_no += 2 ;
	if( !( ew->disp_dir = pl_w->disp_dir ^ 1 ) ){
		ew->pos.x = scr_work.scr_right + 80.0f ;
		ew->spd.x = -DOT_X( 3.5f ) ;
	} else {
		ew->pos.x = scr_work.scr_left + -80.0f ;
		ew->spd.x = DOT_X( 3.5f ) ;
	}
	ew->pos.y = pl_w->ground_y ;
	ew->acc.x = 0 ;
	ew->spd.y = 0 ;
	ew->acc.y = 0 ;
	pl_char_set( ew, 25, 28 ) ;
	ew->be_flag = 1 ;
	pl_shadow_set( ew ) ;
	pls1c_07_t10_run( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_07_t10_run( register PLAYER_WORK *ew, register PLAYER_WORK *pl_w )
{
	Float x ;

	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( pl_w->act_st != N_WIN ){
		ew->routine_3 = 4 ;
		return ;
	}
	x = pl_w->pos.x + ( !ew->disp_dir ? 90.0f : -90.0f ) ;
	if( !ew->disp_dir && ew->pos.x <= x ){
		ew->routine_3 ++ ;
		ew->pos.x = x ;
		pl_char_set( ew, 25, 29 ) ;
	} else if( ew->disp_dir && ew->pos.x >= x ){
		ew->routine_3 ++ ;
		ew->pos.x = x ;
		pl_char_set( ew, 25, 29 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls1c_07_t10_stop( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_3 ++ ;
		pl_w->timer0 = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void pls1c_07_t10_pose( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_07_t10_leave( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !scr_in_ck( ew ) ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	pl_char_move( ew ) ;
}

static void pls1c_07_t11_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t11_ride( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t11_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls1c_07_t11( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls1c_07_t11_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_07_t11_init, pls1c_07_t11_ride, pls1c_07_t11_end,
	} ;

	pls1c_07_t11_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls1c_07_t11_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->sub_prio = 1 ;
	if( !( ew->disp_dir = pl_w->disp_dir ) ){
		ew->spd.x = -DOT_X( 4.0f ) ;
	} else {
		ew->spd.x = DOT_X( 4.0f ) ;
	}
	ew->acc.x = 0 ;
	ew->spd.y = 0 ;
	ew->acc.y = 0 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->ground_y ;
	pl_char_set( ew, 18, 1 ) ;
	ew->be_flag = 1 ;
	pl_shadow_set( ew ) ;
	pls1c_07_t11_ride( ew, pl_w ) ;
}

/*==============================================================*/
/*			Shakes the player to the step of its table			*/
/*==============================================================*/
static void pls1c_07_t11_ride( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Sint16 pls1c_07_shake[][2] = {
		{ 3, 5 }, { 2, 4 }, { 1, 3 }, { 0, 2 }, { 0, 4 }, { 0, 4 }, { 1, 5 }, { 2, 5 },
	} ;
	VEC2 v ;

	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !pl_w->cg_type ){
		ew->routine_3 ++ ;
		ew->timer0 = 30 ;
		return ;
	}
	ew->prio = pl_w->prio ;
	v.x = DOT_X( pls1c_07_shake[ew->cg_prm][0] ) ;
	v.y = DOT_Y( pls1c_07_shake[ew->cg_prm][1] ) ;
	if( ew->disp_dir ){
		v.x = -v.x ;
	}
	pl_w->pos.x = v.x + ew->pos.x ;
	pl_w->pos.y = v.y + ew->pos.y ;
}

/*--------------------------------------------------------------*/
static void pls1c_07_t11_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !-- ew->timer0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
}

static void pls1c_07_t12_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t12_ride( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t12_leave( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls1c_07_t12( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls1c_07_t12_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_07_t12_init, pls1c_07_t12_ride, pls1c_07_t12_leave,
	} ;

	pls1c_07_t12_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls1c_07_t12_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->sub_prio = -1 ;
	if( !( ew->disp_dir = pl_w->disp_dir ) ){
		ew->spd.x = -DOT_X( 4.0f ) ;
	} else {
		ew->spd.x = DOT_X( 4.0f ) ;
	}
	ew->acc.x = 0 ;
	ew->spd.y = 0 ;
	ew->acc.y = 0 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	pl_char_set( ew, 25, 19 ) ;
	ew->be_flag = 1 ;
	pl_shadow_set( ew ) ;
	pls1c_07_t12_ride( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_07_t12_ride( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	VEC2 v ;

	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !pl_w->cg_type ){
		ew->routine_3 ++ ;
		ew->timer0 = 50 ;
		if( !ew->disp_dir ){
			ew->acc.x = -0.104166664f ;
		} else {
			ew->acc.x = 0.104166664f ;
		}
		ew->acc.y = 0.1674107f ;
		return ;
	}
	ew->prio = pl_w->prio ;
	v.x = -DOT_X( 2.0f ) ;
	v.y = DOT_Y( 4.0f ) ;
	if( ew->disp_dir ){
		v.x = -v.x ;
	}
	pl_w->pos.x = v.x + ew->pos.x ;
	pl_w->pos.y = v.y + ew->pos.y ;
}

/*--------------------------------------------------------------*/
static void pls1c_07_t12_leave( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !-- ew->timer0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
}

static void pls1c_07_t13_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t13_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t13_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t13_land( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t13_turn( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t13_leave_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_07_t13_leave( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls1c_07_t13( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls1c_07_t13_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_07_t13_init, pls1c_07_t13_wait, pls1c_07_t13_fall, pls1c_07_t13_land, pls1c_07_t13_turn,
		pls1c_07_t13_turn, pls1c_07_t13_turn, pls1c_07_t13_turn, pls1c_07_t13_leave_init, pls1c_07_t13_leave,
	} ;

	pls1c_07_t13_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls1c_07_t13_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->timer0 = 70 ;
	ew->pos.x = pl_w->pos.x + ( !( ew->disp_dir = pl_w->disp_dir ) ? -DOT_X( 59.0f ) : DOT_X( 59.0f ) ) ;
	ew->pos.y = pl_w->pos.y + DOT_Y( 3.0f ) ;
	pl_char_set( ew, 25, 22 ) ;
	ew->be_flag = 1 ;
	pl_shadow_set( ew ) ;
	pls1c_07_t13_wait( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_07_t13_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	VEC3 v ;

	if( !-- ew->timer0 ){
		ew->routine_3 ++ ;
		if( !ew->disp_dir ){
			ew->spd.x = -DOT_X( 2.0f ) ;
		} else {
			ew->spd.x = DOT_X( 2.0f ) ;
		}
		ew->acc.x = 0 ;
		ew->spd.y = DOT_Y( 2.0f ) ;
		ew->acc.y = -DOT_Y( 0.375f ) ;
		v.x = 0 ;
		v.y = DOT_Y( 16.0f ) ;
		hit_mark_set( ew, &v, 0 ) ;
		pl_char_set( ew, 25, 20 ) ;
		return ;
	}
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_07_t13_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float y ;

	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	y = pl_w->ground_y + DOT_Y( 3.0f ) ;
	if( ew->pos.y <= y ){
		ew->routine_3 ++ ;
		ew->pos.y = y ;
		pl_char_set( ew, 18, 8 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls1c_07_t13_land( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_3 ++ ;
		ew->timer0 = 16 ;
		ew->disp_dir ^= 1 ;
		pl_char_set( ew, 25, 20 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls1c_07_t13_turn( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !-- ew->timer0 ){
		ew->routine_3 ++ ;
		ew->timer0 = 16 ;
		ew->disp_dir ^= 1 ;
	}
}

/*--------------------------------------------------------------*/
static void pls1c_07_t13_leave_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->timer0 = 48 ;
	if( !ew->disp_dir ){
		ew->spd.x = -DOT_X( 4.0f ) ;
	} else {
		ew->spd.x = DOT_X( 4.0f ) ;
	}
	ew->acc.x = 0 ;
	ew->spd.y = 0 ;
	ew->acc.y = 0 ;
	pl_char_set( ew, 25, 21 ) ;
	pls1c_07_t13_leave( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_07_t13_leave( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !-- ew->timer0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_07_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls1c_07_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
static void pls1c_07_color_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w, COLOR_STEP *step )
{
	PLS1C_07_LOCAL_W *lw = PLS1C_07_LW( ew ) ;
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
static int pls1c_07_color_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS1C_07_LOCAL_W *lw = PLS1C_07_LW( ew ) ;
	int ret = 1 ;

	if( !--lw->time ){
		if( lw->last ) ret = 0 ;
		pls1c_07_color_start( ew, pl_w, lw->step ) ;
	}
	return ret ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls1c_07.c" End of File	*/

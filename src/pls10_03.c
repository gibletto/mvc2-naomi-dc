/* Tron Bonne's player effect 03: a Servbot that runs on, fights beside her through nineteen
   steps of routine_2 (each with its own steps in routine_3) and runs off again. */
#include "player.h"
#include "pl_char.h"
#include "pl10_def.h"

/* the step it comes back to after a detour, in the effect's own work */
typedef struct PLS10_03_LOCAL_W {
	Sint32	save_2 ;
	Sint32	save_3 ;
} PLS10_03_LOCAL_W ;
#define PLS10_03_LW( w )	( ( PLS10_03_LOCAL_W * )&( w )->obj_flag )
#define PLS10_03_SAVE( w )	{ PLS10_03_LW( w )->save_2 = ( w )->routine_2 ; PLS10_03_LW( w )->save_3 = ( w )->routine_3 ; }

static void pls10_03_main( PLAYER_WORK *ew );
static void pls10_03_big( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static int pls10_03_bit_ck( Sint16 flag, Sint16 no );
static Float pls10_03_x_ofs( PLAYER_WORK *w, Sint16 n );

/****************************************************************/
PLAYER_WORK *pls10_03_set( PLAYER_WORK *pl_w, int no )
{
	PLAYER_WORK *ew ;
	PL10_LOCAL_W *lw = PL10_LW( pl_w ) ;

	if( ( ew = work_get( 0, W_PLS_ST, LIST_TOP ) ) ){
		ew->work_func = pls10_03_main ;
		ew->master_w = pl_w ;
		lw->servbot_flag = 1 ;
		lw->servbot_w = ew ;
	}
	return ew ;
}

static void pls10_03_init( PLAYER_WORK *ew );
static void pls10_03_move( PLAYER_WORK *ew );
static void pls10_03_end( PLAYER_WORK *ew );
static void pls10_03_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls10_03_main( PLAYER_WORK *ew )
{
	static void ( *pls10_03_jmp[] )( PLAYER_WORK *ew ) = {
		pls10_03_init, pls10_03_move, pls10_03_end, pls10_03_free,
	} ;

	pls10_03_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls10_03_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	if( Sys->play_mode == 0x80 ){
		ew->routine_0 = 2 ;
		ew->routine_1 = 0 ;
		return ;
	}
	ew->routine_0 ++ ;
	ew->ef_no = 0x1003 ;
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
	ew->prio = 12 ;
	pl_char_set( ew, Char_Effect, 0 ) ;
	pl_shadow_set( ew ) ;
	ew->timer1 = 4 ;
	if( pl_w->team_no ){
		ew->routine_2 = 5 ;
		ew->be_flag = 0 ;
		ew->disp_flag = 0 ;
	}
	pls10_03_move( ew ) ;
}

static void pls10_03_appear( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_leave( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_stand( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_walk( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_leave( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_stand( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_walk( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_change( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_enter( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_win( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_jump( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_chouhatsu( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_turn( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_lose( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_scene( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_s14( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_lunch( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_damage( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_king( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_claw( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*--------------------------------------------------------------*/
static void pls10_03_move( PLAYER_WORK *ew )
{
	static void ( *pls10_03_jmp_move[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls10_03_appear, pls10_03_leave, pls10_03_stand, pls10_03_walk,
		pls10_03_run, pls10_03_wait, pls10_03_change, pls10_03_enter,
		pls10_03_win, pls10_03_jump, pls10_03_chouhatsu, pls10_03_turn,
		pls10_03_lose, pls10_03_scene, pls10_03_s14, pls10_03_lunch,
		pls10_03_damage, pls10_03_king, pls10_03_claw,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( vs_sys_work.game_stop & ( 1 << ( pl_w->pl_no ^ 1 ) ) ) return ;
	if( vs_sys_work.game_stop && pl_w->act_st == N_SUPER && pl_w->pl_sp_type == 6 ) return ;
	pls10_03_jmp_move[ew->routine_2]( ew, pl_w ) ;
}

static void pls10_03_appear_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_appear_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_appear_jump( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_appear_land( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
/*			Drops in from above, lands, then turns toward		*/
/*			her													*/
/*==============================================================*/
static void pls10_03_appear( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls10_03_appear_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls10_03_appear_init, pls10_03_appear_wait, pls10_03_appear_jump, pls10_03_appear_land,
	} ;

	pls10_03_appear_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls10_03_appear_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->disp_flag = 0 ;
	ew->be_flag = 0 ;
	pl_char_set( ew, Char_Effect, 17 ) ;
}

/*--------------------------------------------------------------*/
static void pls10_03_appear_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_0 && pl_w->char_no.b.grp == Char_Appear && pl_w->cg_type ) return ;
	ew->routine_3 ++ ;
	ew->disp_flag = 1 ;
	ew->be_flag = 1 ;
	ew->prio = 0 ;
	ew->disp_dir = pl_w->disp_dir ^ 1 ;
	if( !pl_w->disp_dir ){
		ew->pos.x = pl_w->pos.x + -DOT_X( 22.0f ) ;
		ew->spd.x = DOT_X( 2.0f ) ;
	} else {
		ew->pos.x = pl_w->pos.x + DOT_X( 22.0f ) ;
		ew->spd.x = -DOT_X( 2.0f ) ;
	}
	ew->pos.y = pl_w->pos.y + DOT_Y( 30.0f ) ;
	ew->acc.x = 0 ;
	ew->spd.y = DOT_Y( 6.0f ) ;
	ew->acc.y = -DOT_Y( 0.375f ) ;
	pl_char_set( ew, Char_Effect, 17 ) ;
}

/*--------------------------------------------------------------*/
static void pls10_03_appear_jump( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y > pl_w->ground_y ) return ;
	ew->routine_3 ++ ;
	ew->prio = 12 ;
	ew->pos.y = pl_w->ground_y ;
	pl_char_set( ew, Char_Effect, 22 ) ;
}

/*--------------------------------------------------------------*/
static void pls10_03_appear_land( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		PLS10_03_SAVE( ew ) ;
		ew->routine_2 = 11 ;
		ew->routine_3 = 0 ;
		pls10_03_turn( ew, pl_w ) ;
	}
}

/*==============================================================*/
/*			Waits out of sight until her player is in the		*/
/*			fight, then comes on beside her						*/
/*==============================================================*/
static void pls10_03_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !pl_w->be_flag ) return ;
	if( !pl_w->team_no ){
		ew->be_flag = 1 ;
		ew->disp_flag = 1 ;
		if( !pl_w->disp_dir ) ew->pos.x = pl_w->pos.x + 80.0f ;
		else ew->pos.x = pl_w->pos.x + -80.0f ;
		ew->pos.y = pl_w->ground_y ;
		pl_char_set( ew, Char_Effect, 0 ) ;
		PLS10_03_SAVE( ew ) ;
		ew->routine_2 = 7 ;
		ew->routine_3 = 0 ;
		return ;
	}
	if( pl_w->team_type != 5 || pl_w->act_st != N_SUPER ) return ;
	ew->be_flag = 1 ;
	ew->disp_flag = 1 ;
	if( !pl_w->disp_dir ) ew->pos.x = pl_w->pos.x + 80.0f ;
	else ew->pos.x = pl_w->pos.x + -80.0f ;
	ew->pos.y = pl_w->ground_y ;
	pl_char_set( ew, Char_Effect, 0 ) ;
	switch( pl_w->pl_sp_type ){
	case 5 :
		PLS10_03_SAVE( ew ) ;
		ew->routine_2 = 15 ;
		ew->routine_3 = 0 ;
		break ;
	case 4 :
		PLS10_03_SAVE( ew ) ;
		ew->routine_2 = 17 ;
		ew->routine_3 = 0 ;
		break ;
	}
}

static void pls10_03_leave_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_leave_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
/*			Runs off the screen, keeping an eye on a hyper		*/
/*			combo of hers										*/
/*==============================================================*/
static void pls10_03_leave( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls10_03_leave_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls10_03_leave_init, pls10_03_leave_move,
	} ;

	pls10_03_leave_jmp[ew->routine_3]( ew, pl_w ) ;
	if( pl_w->act_st != N_SUPER ) return ;
	switch( pl_w->pl_sp_type ){
	case 5 :
		PLS10_03_SAVE( ew ) ;
		ew->routine_2 = 15 ;
		break ;
	case 4 :
		PLS10_03_SAVE( ew ) ;
		ew->routine_2 = 17 ;
		break ;
	default :
		return ;
	}
	ew->routine_3 = 0 ;
}

/*==============================================================*/
static void pls10_03_leave_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	if( !( ew->disp_dir = pl_w->pl_dir ^ 1 ) ) ew->spd.x = -10.0f ;
	else ew->spd.x = 10.0f ;
	ew->acc.x = 0 ;
	if( pl_w->vital_new > 0 ) pl_char_set( ew, Char_Effect, 13 ) ;
	else pl_char_set( ew, Char_Effect, 79 ) ;
}

/*--------------------------------------------------------------*/
static void pls10_03_leave_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	if( !scr_in_x_ck( ew ) ){
		ew->routine_2 = 5 ;
		ew->routine_3 = 0 ;
		ew->be_flag = 0 ;
		ew->disp_flag = 0 ;
	}
}

static void pls10_03_stand_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_stand_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*--------------------------------------------------------------*/
static void pls10_03_stand( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls10_03_stand_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls10_03_stand_init, pls10_03_stand_move,
	} ;

	pls10_03_stand_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls10_03_stand_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Uint8 dir = 0 ;

	ew->routine_3 ++ ;
	ew->spd.x = 0 ;
	ew->acc.x = 0 ;
	ew->spd.y = 0 ;
	ew->acc.y = 0 ;
	if( ew->pos.x < pl_w->pos.x ) dir = 1 ;
	if( dir != ew->disp_dir ){
		PLS10_03_SAVE( ew ) ;
		ew->routine_2 = 11 ;
		ew->routine_3 = 0 ;
		pls10_03_turn( ew, pl_w ) ;
		return ;
	}
	if( ew->routine_2 != PLS10_03_LW( ew )->save_2 ) pl_char_set( ew, Char_Effect, Rnd() % 3 ) ;
	pls10_03_stand_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls10_03_stand_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( -- ew->timer1 == 0 ) pls10_03_big( ew, pl_w ) ;
}

static void pls10_03_turn_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_turn_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*--------------------------------------------------------------*/
static void pls10_03_turn( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls10_03_turn_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls10_03_turn_init, pls10_03_turn_move,
	} ;

	pls10_03_turn_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls10_03_turn_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->spd.x = 0 ;
	ew->acc.x = 0 ;
	ew->spd.y = 0 ;
	ew->acc.y = 0 ;
	ew->disp_dir ^= 1 ;
	pl_char_set( ew, Char_Effect, 14 ) ;
	pls10_03_turn_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls10_03_turn_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		PLS10_03_SAVE( ew ) ;
		ew->routine_2 = 2 ;
		ew->routine_3 = 0 ;
	}
}

static void pls10_03_damage_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_damage_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_damage_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*--------------------------------------------------------------*/
static void pls10_03_damage( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls10_03_damage_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls10_03_damage_init, pls10_03_damage_move, pls10_03_damage_end,
	} ;

	pls10_03_damage_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls10_03_damage_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	if( ew->pos.x < pl_w->pos.x ) ew->disp_dir = 1 ;
	else ew->disp_dir = 0 ;
	pl_char_set( ew, Char_Effect, 16 ) ;
	pls10_03_damage_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls10_03_damage_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		if( pl_w->combo_dm_cnt < 3 ){
			ew->routine_3 ++ ;
			pl_char_set( ew, Char_Effect, 70 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void pls10_03_damage_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ) pls10_03_big( ew, pl_w ) ;
}

static void pls10_03_lunch_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_lunch_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_lunch_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_lunch_keep( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_lunch_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*--------------------------------------------------------------*/
static void pls10_03_lunch( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls10_03_lunch_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls10_03_lunch_init, pls10_03_lunch_run, pls10_03_lunch_wait, pls10_03_lunch_keep, pls10_03_lunch_end,
	} ;

	pls10_03_lunch_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls10_03_lunch_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->timer0 = 20 ;
	ew->spd.z = scr_work.scr_left + 320.0f ;
	ew->acc.z = pl_w->ground_y ;
	if( ew->pos.x >= ew->spd.z ) ew->disp_dir = 0 ;
	else ew->disp_dir = 1 ;
	pl_char_set( ew, Char_Effect, 13 ) ;
	pls10_03_lunch_run( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls10_03_lunch_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->act_st != N_SUPER || pl_w->pl_sp_type != 5 ){
		PLS10_03_SAVE( ew ) ;
		ew->routine_2 = 16 ;
		ew->routine_3 = 0 ;
		return ;
	}
	ew->pos.x += ( ew->spd.z - ew->pos.x ) / ew->timer0 ;
	ew->pos.y += ( ew->acc.z - ew->pos.y ) / ew->timer0 ;
	if( -- ew->timer0 == 0 ){
		ew->routine_3 ++ ;
		ew->disp_dir = pl_w->disp_dir ^ 1 ;
		pl_char_set( ew, Char_Effect, 64 ) ;
	}
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls10_03_lunch_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL10_LOCAL_W *lw = PL10_LW( pl_w ) ;

	if( pl_w->act_st != N_SUPER || pl_w->pl_sp_type != 5 ){
		PLS10_03_SAVE( ew ) ;
		ew->routine_2 = 16 ;
		ew->routine_3 = 0 ;
		return ;
	}
	pl_char_move( ew ) ;
	if( lw->run_cnt ){
		ew->routine_3 ++ ;
		ew->timer1 = 0 ;
		pl_char_set( ew, Char_Effect, 65 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls10_03_lunch_keep( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL10_LOCAL_W *lw = PL10_LW( pl_w ) ;

	pl_char_move( ew ) ;
	if( pl_w->act_st != N_SUPER || pl_w->pl_sp_type != 5 || ew->timer1 >= lw->run_cnt ){
		ew->routine_3 ++ ;
		pl_char_set( ew, Char_Effect, 66 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls10_03_lunch_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->act_st == N_SUPER && pl_w->pl_sp_type == 4 ){
		PLS10_03_SAVE( ew ) ;
		ew->routine_2 = 17 ;
		ew->routine_3 = 0 ;
		return ;
	}
	if( pl_char_move( ew ) < 0 ) pls10_03_big( ew, pl_w ) ;
}

static void pls10_03_king_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_king_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_king_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_king_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_king_grow( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_king_rush( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_king_shrink( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*--------------------------------------------------------------*/
static void pls10_03_king( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls10_03_king_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls10_03_king_init, pls10_03_king_run, pls10_03_king_wait, pls10_03_king_start, pls10_03_king_grow, pls10_03_king_rush, pls10_03_king_shrink,
	} ;

	pls10_03_king_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls10_03_king_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->timer0 = 40 ;
	ew->spd.z = pls10_03_x_ofs( pl_w, -64 ) ;
	ew->acc.z = pl_w->ground_y ;
	if( ew->pos.x >= ew->spd.z ) ew->disp_dir = 0 ;
	else ew->disp_dir = 1 ;
	pl_char_set( ew, Char_Effect, 13 ) ;
	pls10_03_king_run( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls10_03_king_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->act_st != N_SUPER || pl_w->pl_sp_type != 4 ){
		PLS10_03_SAVE( ew ) ;
		ew->routine_2 = 16 ;
		ew->routine_3 = 0 ;
		return ;
	}
	ew->pos.x += ( ew->spd.z - ew->pos.x ) / ew->timer0 ;
	ew->pos.y += ( ew->acc.z - ew->pos.y ) / ew->timer0 ;
	if( -- ew->timer0 == 0 ){
		ew->routine_3 ++ ;
		pl_char_set( ew, Char_Effect, 0 ) ;
	}
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls10_03_king_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->act_st != N_SUPER || pl_w->pl_sp_type != 4 ){
		PLS10_03_SAVE( ew ) ;
		ew->routine_2 = 16 ;
		ew->routine_3 = 0 ;
		return ;
	}
	pl_char_move( ew ) ;
	if( !pl_w->cg_type ) return ;
	if( vs_sys_work.game_stop ) return ;
	ew->routine_3 ++ ;
	ew->disp_dir = pl_w->disp_dir ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->atck_no = pl10_at_king ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, Char_Effect, 67 ) ;
}

/*--------------------------------------------------------------*/
static void pls10_03_king_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( ew->cg_type ) ew->routine_3 ++ ;
}

/*--------------------------------------------------------------*/
static void pls10_03_king_grow( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->scale.x += 0.03125f ;
	ew->scale.y += 0.03125f ;
	if( pl_char_move( ew ) < 0 ){
		ew->routine_3 ++ ;
		ew->timer0 = 0 ;
		ew->scale.x = 2.0f ;
		ew->scale.y = 2.0f ;
		if( !ew->disp_dir ) ew->spd.x = -DOT_X( 4.0f ) ;
		else ew->spd.x = DOT_X( 4.0f ) ;
		ew->acc.x = 0 ;
		pl_char_set( ew, Char_Effect, 68 ) ;
		ew->atck_no = pl10_at_king_2 ;
		HIT_RESET( ew ) ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls10_03_king_rush( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	if( !ew->disp_dir ){
		if( ew->spd.x < -DOT_X( 4.0f ) ){
			ew->spd.x = -DOT_X( 4.0f ) ;
			ew->acc.x = 0 ;
		}
	} else {
		if( ew->spd.x > DOT_X( 4.0f ) ){
			ew->spd.x = DOT_X( 4.0f ) ;
			ew->acc.x = 0 ;
		}
	}
	pl_char_move( ew ) ;
	if( !pl_w->cg_type || pl_w->act_st != N_SUPER || pl_w->pl_sp_type != 4 ){
		ew->routine_3 ++ ;
		pl_char_set( ew, Char_Effect, 69 ) ;
		return ;
	}
	if( ew->hit_flag || ew->hit_box_no ){
		if( !ew->timer0 ){
			ew->timer0 = 10 ;
			if( !ew->disp_dir ){
				ew->spd.x = 2.5f ;
				ew->acc.x = -DOT_X( 0.25f ) ;
			} else {
				ew->spd.x = -2.5f ;
				ew->acc.x = DOT_X( 0.25f ) ;
			}
		}
	}
	if( ew->timer0 ){
		if( -- ew->timer0 == 0 ){
			ew->atck_no = pl10_at_king_2 ;
			HIT_RESET( ew ) ;
		}
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls10_03_king_shrink( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->scale.x -= 0.035714287f ;
	ew->scale.y -= 0.035714287f ;
	if( ew->scale.x < 1.0f ){
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
	}
	if( pl_char_move( ew ) < 0 ){
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
		pls10_03_big( ew, pl_w ) ;
	}
}

static void pls10_03_chouhatsu_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_chouhatsu_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_chouhatsu_hop( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*--------------------------------------------------------------*/
static void pls10_03_chouhatsu( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls10_03_chouhatsu_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls10_03_chouhatsu_init, pls10_03_chouhatsu_move, pls10_03_chouhatsu_hop,
	} ;

	if( ew->pos.y == pl_w->ground_y && !( pl_w->act_st == N_SPECIAL && pl_w->pl_sp_type == 3 ) ){
		pls10_03_big( ew, pl_w ) ;
		return ;
	}
	pls10_03_chouhatsu_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls10_03_chouhatsu_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->spd.x = 0 ;
	ew->acc.x = 0 ;
	ew->spd.y = 0 ;
	ew->acc.y = 0 ;
	if( ew->pos.x < pl_w->pos.x ) ew->disp_dir = 1 ;
	else ew->disp_dir = 0 ;
	pl_voice_req( ew, 26 ) ;
	pl_char_set( ew, Char_Effect, 15 ) ;
	pls10_03_chouhatsu_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls10_03_chouhatsu_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( ew->cg_type ){
		ew->routine_3 ++ ;
		ew->spd.y = DOT_Y( 3.0f ) ;
		ew->acc.y = -DOT_Y( 0.375f ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls10_03_chouhatsu_hop( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( pl_w->ground_y < ew->pos.y ) return ;
	ew->routine_3 = 1 ;
	ew->pos.y = pl_w->ground_y ;
	pl_char_move( ew ) ;
}

static void pls10_03_claw_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_claw_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_claw_hop( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*--------------------------------------------------------------*/
static void pls10_03_claw( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls10_03_claw_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls10_03_claw_init, pls10_03_claw_move, pls10_03_claw_hop,
	} ;

	pls10_03_claw_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls10_03_claw_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	if( PL10_LW( pl_w )->claw_hop_cnt ) PL10_LW( pl_w )->claw_hop_cnt -- ;
	ew->spd.x = 0 ;
	ew->acc.x = 0 ;
	ew->spd.y = 0 ;
	ew->acc.y = 0 ;
	if( ew->pos.x < pl_w->pos.x ) ew->disp_dir = 1 ;
	else ew->disp_dir = 0 ;
	pl_char_set( ew, Char_Effect, 15 ) ;
	pls10_03_claw_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls10_03_claw_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( ew->cg_type ){
		ew->routine_3 ++ ;
		ew->spd.y = DOT_Y( 3.0f ) ;
		ew->acc.y = -DOT_Y( 0.375f ) ;
	}
	if( pl_w->act_st == N_SUPER ){
		switch( pl_w->pl_sp_type ){
		case 5 :
			PLS10_03_SAVE( ew ) ;
			ew->routine_2 = 15 ;
			break ;
		case 4 :
			PLS10_03_SAVE( ew ) ;
			ew->routine_2 = 17 ;
			break ;
		default :
			return ;
		}
		ew->routine_3 = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void pls10_03_claw_hop( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y > pl_w->ground_y ) return ;
	ew->pos.y = pl_w->ground_y ;
	pl_char_move( ew ) ;
	if( PL10_LW( pl_w )->claw_hop_cnt ){
		PL10_LW( pl_w )->claw_hop_cnt -- ;
		ew->routine_3 = 1 ;
		return ;
	}
	pls10_03_big( ew, pl_w ) ;
}

static void pls10_03_change_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_change_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*--------------------------------------------------------------*/
static void pls10_03_change( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls10_03_change_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls10_03_change_init, pls10_03_change_move,
	} ;

	if( !pl_w->team_no && ( pl_w->act_st != N_CHANGE || pl_w->change_type ) ){
		pls10_03_big( ew, pl_w ) ;
		return ;
	}
	pls10_03_change_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls10_03_change_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->disp_dir = pl_w->disp_dir ;
	pl_char_set( ew, Char_Effect, 25 ) ;
	pls10_03_change_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls10_03_change_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		PLS10_03_SAVE( ew ) ;
		ew->routine_2 = 1 ;
		ew->routine_3 = 0 ;
	}
}

static void pls10_03_enter_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_enter_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_enter_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_enter_stop( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
/*			Runs in from the screen's edge and stops by her		*/
/*==============================================================*/
static void pls10_03_enter( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls10_03_enter_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls10_03_enter_init, pls10_03_enter_start, pls10_03_enter_run, pls10_03_enter_stop,
	} ;

	if( pl_w->act_st == N_SUPER ){
		switch( pl_w->pl_sp_type ){
		case 5 :
			PLS10_03_SAVE( ew ) ;
			ew->routine_2 = 15 ;
			break ;
		case 4 :
			PLS10_03_SAVE( ew ) ;
			ew->routine_2 = 17 ;
			break ;
		default :
			return ;
		}
		ew->routine_3 = 0 ;
		return ;
	}
	pls10_03_enter_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls10_03_enter_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->timer0 = 30 ;
	ew->disp_dir = pl_w->disp_dir ;
	if( !scr_in_x_ck( ew ) ){
		if( !ew->disp_dir ){
			ew->pos.x = scr_work.scr_right + 80.0f ;
			ew->spd.x = -DOT_X( 4.75f ) ;
		} else {
			ew->pos.x = scr_work.scr_left + -80.0f ;
			ew->spd.x = DOT_X( 4.75f ) ;
		}
	}
	ew->acc.x = 0 ;
	ew->pos.y = pl_w->ground_y ;
	pl_char_set( ew, Char_Effect, 13 ) ;
	pls10_03_enter_start( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls10_03_enter_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	if( -- ew->timer0 == 0 ){
		ew->routine_3 ++ ;
		ew->timer0 = 300 ;
	}
}

/*--------------------------------------------------------------*/
static void pls10_03_enter_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float x ;

	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	x = pls10_03_x_ofs( pl_w, 56 ) ;
	if( -- ew->timer0 == 0 || ( !ew->disp_dir && ew->pos.x <= x ) || ( ew->disp_dir && ew->pos.x >= x ) ){
		ew->routine_3 ++ ;
		pl_char_set( ew, Char_Effect, 26 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls10_03_enter_stop( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ) pls10_03_big( ew, pl_w ) ;
}

static void pls10_03_s14_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_s14_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*--------------------------------------------------------------*/
static void pls10_03_s14( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls10_03_s14_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls10_03_s14_init, pls10_03_s14_move,
	} ;

	pls10_03_s14_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls10_03_s14_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->disp_dir = pl_w->disp_dir ;
	pl_char_set( ew, Char_Effect, 33 ) ;
	pls10_03_s14_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls10_03_s14_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ) pls10_03_big( ew, pl_w ) ;
}

static void pls10_03_win_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_win_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*--------------------------------------------------------------*/
static void pls10_03_win( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls10_03_win_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls10_03_win_init, pls10_03_win_move,
	} ;

	if( pl_w->act_st != N_WIN ){
		pls10_03_big( ew, pl_w ) ;
		return ;
	}
	pls10_03_win_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls10_03_win_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->disp_dir = pl_w->disp_dir ;
	pl_char_set( ew, Char_Effect, 27 ) ;
	pls10_03_win_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls10_03_win_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
}

static void pls10_03_scene_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_scene_walk_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_scene_walk( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_scene_stand_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_scene_stand( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_scene_turn( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_scene_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_scene_voice( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
/*			Walks to her and stands, turning as she turns		*/
/*==============================================================*/
static void pls10_03_scene( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls10_03_scene_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls10_03_scene_init, pls10_03_scene_walk_init, pls10_03_scene_walk, pls10_03_scene_stand_init, pls10_03_scene_stand, pls10_03_scene_turn, pls10_03_scene_run, pls10_03_scene_voice,
	} ;

	pls10_03_scene_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls10_03_scene_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float dx = ew->pos.x - pls10_03_x_ofs( pl_w, 56 ) ;

	if( dx < -DOT_X( 8.0f ) || dx > DOT_X( 8.0f ) ){
		ew->routine_3 = 1 ;
		pls10_03_scene_walk_init( ew, pl_w ) ;
	} else {
		ew->routine_3 = 3 ;
		pls10_03_scene_stand_init( ew, pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls10_03_scene_walk_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	if( ew->pos.x >= pls10_03_x_ofs( pl_w, 56 ) ){
		ew->disp_dir = 0 ;
		ew->spd.x = -DOT_X( 4.0f ) ;
	} else {
		ew->disp_dir = 1 ;
		ew->spd.x = DOT_X( 4.0f ) ;
	}
	ew->acc.x = 0 ;
	pl_char_set( ew, Char_Effect, 12 ) ;
	pls10_03_scene_walk( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls10_03_scene_walk( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float x ;

	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	x = pls10_03_x_ofs( pl_w, 56 ) ;
	if( ( ew->disp_dir == 0 && ew->pos.x <= x ) || ( ew->disp_dir != 0 && ew->pos.x >= x ) ){
		ew->routine_3 = 3 ;
		ew->pos.x = x ;
	}
	if( pl_w->cg_type ){
		ew->routine_3 = 3 ;
		ew->pos.x = x ;
	}
}

/*--------------------------------------------------------------*/
static void pls10_03_scene_stand_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->spd.x = 0 ;
	ew->spd.y = 0 ;
	ew->acc.x = 0 ;
	ew->acc.y = 0 ;
	ew->disp_dir = pl_w->disp_dir ;
	pl_char_set( ew, Char_Effect, 0 ) ;
	pls10_03_scene_stand( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls10_03_scene_stand( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float dx ;

	pl_char_move( ew ) ;
	if( pl_w->act_st != N_WIN ) return ;
	dx = ew->pos.x - pls10_03_x_ofs( pl_w, 56 ) ;
	if( dx < -DOT_X( 8.0f ) || dx > DOT_X( 8.0f ) ) ew->routine_3 = 1 ;
	if( pl_w->cg_type ){
		ew->routine_3 = 5 ;
		pl_char_set( ew, Char_Effect, 19 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls10_03_scene_turn( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_3 ++ ;
		ew->disp_dir ^= 1 ;
		if( !ew->disp_dir ) ew->spd.x = -DOT_X( 4.0f ) ;
		else ew->spd.x = DOT_X( 4.0f ) ;
		ew->acc.x = 0 ;
		ew->timer0 = 1 ;
		pl_char_set( ew, Char_Effect, 20 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls10_03_scene_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pls10_03_scene_voice( ew, pl_w ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	if( ( ew->disp_dir == 0 && ew->pos.x <= scr_work.scr_left + DOT_X( 28.0f ) ) || ( ew->disp_dir != 0 && ew->pos.x >= scr_work.scr_right + -DOT_X( 28.0f ) ) ){
		ew->routine_3 ++ ;
		pl_char_set( ew, Char_Effect, 21 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls10_03_scene_voice( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( -- ew->timer0 == 0 ){
		ew->timer0 = ( Rnd() & 31 ) + 60 ;
		pl_voice_req( ew, 26 ) ;
	}
}

static void pls10_03_lose_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_lose_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_lose_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*--------------------------------------------------------------*/
static void pls10_03_lose( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls10_03_lose_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls10_03_lose_init, pls10_03_lose_wait, pls10_03_lose_move,
	} ;

	pls10_03_lose_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls10_03_lose_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->spd.x = 0 ;
	ew->acc.x = 0 ;
	ew->spd.y = 0 ;
	ew->acc.y = 0 ;
	if( ew->pos.x < pl_w->pos.x ) ew->disp_dir = 1 ;
	else ew->disp_dir = 0 ;
	pl_char_set( ew, Char_Effect, 0 ) ;
	pls10_03_lose_wait( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls10_03_lose_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( pl_w->cg_type ) return ;
	ew->routine_3 ++ ;
	pl_char_set( ew, Char_Effect, 18 ) ;
}

/*--------------------------------------------------------------*/
static void pls10_03_lose_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( -- ew->timer1 == 0 ) pls10_03_big( ew, pl_w ) ;
}

static void pls10_03_walk_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_run_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_walk_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
/*			Walks to her										*/
/*==============================================================*/
static void pls10_03_walk( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls10_03_walk_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls10_03_walk_init, pls10_03_walk_move,
	} ;

	pls10_03_walk_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls10_03_walk_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float dx ;

	ew->routine_3 ++ ;
	dx = ew->pos.x - pls10_03_x_ofs( pl_w, 56 ) ;
	if( dx >= 0 ) ew->disp_dir = 0 ;
	else {
		dx *= -1 ;
		ew->disp_dir = 1 ;
	}
	if( dx >= DOT_X( 56.0f ) ){
		ew->routine_2 = 4 ;
		ew->routine_3 = 0 ;
		pls10_03_run_init( ew, pl_w ) ;
		return ;
	}
	if( pl_w->disp_dir == ew->disp_dir ) ew->spd.x = -DOT_X( 4.0f ) ;
	else ew->spd.x = -5.0f ;
	ew->acc.x = 0 ;
	if( ew->disp_dir ) ew->spd.x *= -1 ;
	if( ew->routine_2 != PLS10_03_LW( ew )->save_2 ) pl_char_set( ew, Char_Effect, 12 ) ;
	pls10_03_walk_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls10_03_walk_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	if( -- ew->timer1 == 0 ) pls10_03_big( ew, pl_w ) ;
}

static void pls10_03_run_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_run_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
/*			Runs to her											*/
/*==============================================================*/
static void pls10_03_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls10_03_run_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls10_03_run_init, pls10_03_run_move,
	} ;

	pls10_03_run_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls10_03_run_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	if( ew->pos.x - pls10_03_x_ofs( pl_w, 56 ) >= 0 ){
		ew->disp_dir = 0 ;
		ew->spd.x = -DOT_X( 4.75f ) ;
	} else {
		ew->disp_dir = 1 ;
		ew->spd.x = DOT_X( 4.75f ) ;
	}
	ew->acc.x = 0 ;
	if( ew->routine_2 != PLS10_03_LW( ew )->save_2 ) pl_char_set( ew, Char_Effect, 13 ) ;
	pls10_03_run_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls10_03_run_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	if( -- ew->timer1 == 0 ) pls10_03_big( ew, pl_w ) ;
}

static void pls10_03_jump_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_jump_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_jump_up( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_jump_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls10_03_jump_land( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
/*			Jumps as her player jumps, and lands				*/
/*==============================================================*/
static void pls10_03_jump( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls10_03_jump_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls10_03_jump_init, pls10_03_jump_start, pls10_03_jump_up, pls10_03_jump_fall, pls10_03_jump_land,
	} ;

	pls10_03_jump_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls10_03_jump_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	ew->disp_dir = pl_w->disp_dir ;
	pl_char_set( ew, Char_Effect, pl_w->jump_dir + 4 ) ;
	pls10_03_jump_start( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls10_03_jump_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->act_st == N_SUPER ){
		switch( pl_w->pl_sp_type ){
		case 5 :
			PLS10_03_SAVE( ew ) ;
			ew->routine_2 = 15 ;
			break ;
		case 4 :
			PLS10_03_SAVE( ew ) ;
			ew->routine_2 = 17 ;
			break ;
		default :
			return ;
		}
		ew->routine_3 = 0 ;
	} else {
		pl_char_move( ew ) ;
		if( !ew->cg_type ){
			ew->routine_3 ++ ;
			if( !pl_w->jump_dir ) ew->spd.x = -5.0f ;
			else if( pl_w->jump_dir == 1 ) ew->spd.x = 5.0f ;
			else ew->spd.x = 0 ;
			if( ew->disp_dir ) ew->spd.x = -ew->spd.x ;
			ew->acc.x = 0 ;
			ew->spd.y = DOT_Y( 16.0f ) ;
			ew->acc.y = -DOT_Y( 0.375f ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void pls10_03_jump_up( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( pl_w->spd.y <= 2.0f ){
		ew->routine_3 ++ ;
		pl_char_set( ew, Char_Effect, pl_w->jump_dir + 7 ) ;
	}
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls10_03_jump_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->spd.y <= -DOT_Y( 9.0f ) ) ew->acc.y = 0 ;
	if( ew->pos.y <= pl_w->ground_y ){
		ew->routine_3 ++ ;
		ew->pos.y = pl_w->ground_y ;
		pl_char_set( ew, Char_Effect, pl_w->jump_dir + 10 ) ;
	}
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls10_03_jump_land( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ) pls10_03_big( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls10_03_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	ew->be_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls10_03_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
static int pls10_03_bit_ck( Sint16 flag, Sint16 no )
{
	if( flag & ( 1 << no ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static Float pls10_03_x_ofs( PLAYER_WORK *w, Sint16 n )
{
	if( !w->disp_dir ) return w->pos.x + DOT_X( n ) ;
	return w->pos.x - n * 1.6666666f ;
}

/*==============================================================*/
/*			Decides what he does next, from what his player		*/
/*			is doing and where he is							*/
/*==============================================================*/
static void pls10_03_big( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Sint16 pls10_03_next_tbl[4] = { 2, 2, 3, 3 } ;
	Uint8 n = 0 ;
	VEC2 d ;

	ew->timer1 = 4 ;
	if( pl_w->act_st == N_SUPER && pl_w->pl_sp_type == 5 ){
		PLS10_03_SAVE( ew ) ;
		ew->routine_2 = 15 ;
		ew->routine_3 = 0 ;
		return ;
	}
	if( pl_w->act_st == N_SUPER && pl_w->pl_sp_type == 4 ){
		PLS10_03_SAVE( ew ) ;
		ew->routine_2 = 17 ;
		ew->routine_3 = 0 ;
		return ;
	}
	if( ( pl_w->team_no && !pl_w->team_type ) || ( pls10_03_bit_ck( vs_sys_work.team_hyper_flag, pl_w->pl_no ) && vs_sys_work.vs_st >= VS_END_WAIT ) ){
		PLS10_03_SAVE( ew ) ;
		ew->routine_2 = 1 ;
		ew->routine_3 = 0 ;
		return ;
	}
	if( pl_w->act_st == N_CHANGE && !pl_w->change_type ){
		PLS10_03_SAVE( ew ) ;
		ew->routine_2 = 6 ;
		ew->routine_3 = 0 ;
		return ;
	}
	if( !scr_in_x_ck( ew ) ){
		PLS10_03_SAVE( ew ) ;
		ew->routine_2 = 4 ;
		ew->routine_3 = 0 ;
		return ;
	}
	if( pl_w->combo_dm_cnt >= 3 ){
		PLS10_03_SAVE( ew ) ;
		ew->routine_2 = 16 ;
		ew->routine_3 = 0 ;
		return ;
	}
	if( pl_w->act_st == N_SPECIAL && pl_w->pl_sp_type == 3 ){
		PLS10_03_SAVE( ew ) ;
		ew->routine_2 = 10 ;
		ew->routine_3 = 0 ;
		return ;
	}
	if( pl_w->char_no.w == 0x1803 ){
		PLS10_03_SAVE( ew ) ;
		ew->routine_2 = 14 ;
		ew->routine_3 = 0 ;
		return ;
	}
	if( PL10_LW( pl_w )->claw_hop_cnt ){
		PLS10_03_SAVE( ew ) ;
		ew->routine_2 = 18 ;
		ew->routine_3 = 0 ;
		return ;
	}
	if( pl_w->act_st == N_WIN ){
		PLS10_03_SAVE( ew ) ;
		switch( pl_w->type ){
		case 0 :
		case 2 :
		case 5 :
		case 6 :
		case 7 :
			ew->routine_2 = 8 ;
			break ;
		case 8 :
		case 9 :
		case 10 :
			ew->routine_2 = 13 ;
			break ;
		case 1 :
		case 3 :
		case 4 :
			ew->routine_2 = 12 ;
			break ;
		}
		ew->routine_3 = 0 ;
		return ;
	}
	if( pl_w->vital_new <= 0 ){
		PLS10_03_SAVE( ew ) ;
		ew->routine_2 = 12 ;
		ew->routine_3 = 0 ;
		return ;
	}
	if( pl_w->act_st == N_SJUMP_AIR && pl_w->spd.y > DOT_Y( 8.0f ) ){
		PLS10_03_SAVE( ew ) ;
		ew->routine_2 = 9 ;
		ew->routine_3 = 0 ;
		return ;
	}
	d.x = pls10_03_x_ofs( pl_w, 56 ) - ew->pos.x ;
	d.y = pl_w->pos.y ;
	if( ew->pos.y + DOT_Y( 48.0f ) <= d.y ) n = 1 ;
	if( d.x < 0 ) d.x *= -1 ;
	if( d.x >= DOT_X( 40.0f ) ) n += 2 ;
	PLS10_03_SAVE( ew ) ;
	ew->routine_2 = pls10_03_next_tbl[n] ;
	ew->routine_3 = 0 ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls10_03.c" End of File	*/

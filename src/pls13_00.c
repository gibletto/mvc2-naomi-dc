/* Ruby Heart's player effect 00 (her pls13): the pieces that go with her moves, chosen by type:
   the ones on her as a flight turns, Sublimation's, her entrance's, and the scene of a win pose,
   whose pieces come in from the screen's edges. Each type has its own init and move; the move runs
   its steps by routine_1. */
#include "player.h"
#include "ef_char.h"

static void pls13_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls13_00_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls13_00_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
	}
	return ew ;
}

/****************************************************************/
/*			A piece that rides on the work pl_w, which is		*/
/*			another piece										*/
/****************************************************************/
PLAYER_WORK *pls13_00_set2( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls13_00_main ;
		ew->master_w = pl_w->master_w ;
		ew->ef_w = pl_w ;
		ew->type = no ;
	}
	return ew ;
}

static void pls13_00_init( PLAYER_WORK *ew );
static void pls13_00_move( PLAYER_WORK *ew );
static void pls13_00_end( PLAYER_WORK *ew );
static void pls13_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls13_00_main( PLAYER_WORK *ew )
{
	static void ( *pls13_00_jmp[] )( PLAYER_WORK *ew ) = {
		pls13_00_init, pls13_00_move, pls13_00_end, pls13_00_free,
	} ;

	pls13_00_jmp[ew->routine_0]( ew ) ;
}

static void pls13_00_turn_half_init( PLAYER_WORK *ew );
static void pls13_00_turn_init( PLAYER_WORK *ew );
static void pls13_00_magie_upper_init( PLAYER_WORK *ew );
static void pls13_00_magie_lower_init( PLAYER_WORK *ew );
static void pls13_00_sublimation_init( PLAYER_WORK *ew );
static void pls13_00_t7_init( PLAYER_WORK *ew );
static void pls13_00_t8_init( PLAYER_WORK *ew );
static void pls13_00_t9_init( PLAYER_WORK *ew );
static void pls13_00_t10_init( PLAYER_WORK *ew );
static void pls13_00_t11_init( PLAYER_WORK *ew );
static void pls13_00_t12_init( PLAYER_WORK *ew );
static void pls13_00_appear_init( PLAYER_WORK *ew );
static void pls13_00_t14_init( PLAYER_WORK *ew );
static void pls13_00_t15_init( PLAYER_WORK *ew );

/*==============================================================*/
static void pls13_00_init( PLAYER_WORK *ew )
{
	static void ( *pls13_00_init_jmp[] )( PLAYER_WORK *ew ) = {
		pls13_00_turn_half_init, pls13_00_turn_init, pls13_00_magie_upper_init, pls13_00_magie_lower_init,
		pls13_00_sublimation_init, pls13_00_sublimation_init, pls13_00_sublimation_init, pls13_00_t7_init,
		pls13_00_t8_init, pls13_00_t9_init, pls13_00_t10_init, pls13_00_t11_init,
		pls13_00_t12_init, pls13_00_appear_init, pls13_00_t14_init, pls13_00_t15_init,
	} ;

	pls13_00_init_jmp[ew->type]( ew ) ;
}

/*==============================================================*/
/*			T0: on her while a flight turns, half clear			*/
/*==============================================================*/
static void pls13_00_turn_half_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
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
	ew->alpha = 0.5f ;
	ew->disp_flag = 1 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ef_char_set( ew, 27, 0 ) ;
}

/*==============================================================*/
/*			T1: on her while a flight turns						*/
/*==============================================================*/
static void pls13_00_turn_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
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
	ew->disp_flag = 1 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ef_char_set( ew, 27, 1 ) ;
}

/*==============================================================*/
/*			T2: ahead of her, high up, for Tour De Magie		*/
/*==============================================================*/
static void pls13_00_magie_upper_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
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
	ew->disp_flag = 1 ;
	ew->prio = 0 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.x += ew->master_w->disp_dir ? 400.0f : -400.0f ;
	ew->pos.x += ew->master_w->disp_dir ? -DOT_X( 64.0f ) : DOT_X( 64.0f ) ;
	ew->pos.y += DOT_Y( 136.0f ) ;
	if( ew->master_w->disp_dir ) ef_char_set( ew, 27, 2 ) ;
	else ef_char_set( ew, 27, 3 ) ;
}

/*==============================================================*/
/*			T3: ahead of her, low, for Tour De Magie			*/
/*==============================================================*/
static void pls13_00_magie_lower_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
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
	ew->disp_flag = 1 ;
	ew->prio = 0 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->pos.x += ew->master_w->disp_dir ? 400.0f : -400.0f ;
	ew->pos.x += ew->master_w->disp_dir ? -DOT_X( 64.0f ) : DOT_X( 64.0f ) ;
	ew->pos.y += DOT_Y( 24.0f ) ;
	if( pl_w->disp_dir ) ef_char_set( ew, 27, 5 ) ;
	else ef_char_set( ew, 27, 6 ) ;
}

/*==============================================================*/
/*			T4: Sublimation's piece, ahead of her by the		*/
/*			strength and kept on the screen						*/
/*==============================================================*/
static void pls13_00_sublimation_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

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
	ew->disp_flag = 1 ;
	ew->prio = 9 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	if( !ew->master_w->atck_level ) ew->pos.x += ew->disp_dir ? DOT_X( 112.0f ) : -DOT_X( 112.0f ) ;
	else ew->pos.x += ew->disp_dir ? DOT_X( 224.0f ) : -DOT_X( 224.0f ) ;
	if( ew->disp_dir ){
		if( ew->pos.x > scr_work.scr_right ) ew->pos.x = scr_work.scr_right - 80.0f ;
	} else {
		if( ew->pos.x < scr_work.scr_left ) ew->pos.x = scr_work.scr_left + 80.0f ;
	}
	ew->timer0 = 40 ;
	ef_char_set( ew, 27, 4 ) ;
}

/*==============================================================*/
/*			T7: a piece that comes in from the edge of the		*/
/*			screen												*/
/*==============================================================*/
static void pls13_00_t7_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	Float x = DOT_X( 32.0f ) ;

	ew->routine_0 ++ ;
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
	ew->disp_flag = 1 ;
	ew->prio = 12 ;
	ew->disp_dir = ew->master_w->disp_dir ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->pos.x = ew->disp_dir ? scr_work.scr_left - x : scr_work.scr_right + x ;
	ew->pos.y += DOT_Y( 24.0f ) ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.x = ew->disp_dir ? 29.999998f : -29.999998f ;
	ew->timer0 = 8 ;
	ef_color_set( pl_w, 4, 4, 1 ) ;
	pl_shadow_set( ew ) ;
	ew->be_flag = 1 ;
	ef_char_set( ew, 27, 8 ) ;
}

/*==============================================================*/
/*			T8: a piece that comes in from the edge of the		*/
/*			screen												*/
/*==============================================================*/
static void pls13_00_t8_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	Float x = -DOT_X( 22.0f ) ;

	ew->routine_0 ++ ;
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
	ew->disp_flag = 1 ;
	ew->prio = 13 ;
	ew->disp_dir = ew->master_w->disp_dir ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->pos.x = ew->disp_dir ? scr_work.scr_left - x : scr_work.scr_right + x ;
	ew->pos.y = scr_work.scr_top + DOT_Y( 16.0f ) ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.x = ew->disp_dir ? DOT_X( 6.625f ) : -DOT_X( 6.625f ) ;
	ew->spd.y = 0.0f ;
	ew->acc.y = -DOT_Y( 0.375f ) ;
	ew->timer0 = 16 ;
	ef_color_set( pl_w, 2, 2, 1 ) ;
	pl_shadow_set( ew ) ;
	ew->be_flag = 1 ;
	ef_char_set( ew, 27, 11 ) ;
}

/*==============================================================*/
/*			T9: a piece that comes in from the edge of the		*/
/*			screen												*/
/*==============================================================*/
static void pls13_00_t9_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	Float x = DOT_X( 32.0f ) ;

	ew->routine_0 ++ ;
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
	ew->disp_flag = 1 ;
	ew->prio = 7 ;
	ew->disp_dir = ew->master_w->disp_dir ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->pos.x = ew->disp_dir ? scr_work.scr_left - x : scr_work.scr_right + x ;
	ew->pos.y -= DOT_Y( 8.0f ) ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.x = ew->disp_dir ? 26.333332f : -26.333332f ;
	ew->timer0 = 5 ;
	ef_color_set( pl_w, 2, 2, 1 ) ;
	pl_shadow_set( ew ) ;
	ew->be_flag = 1 ;
	ef_char_set( ew, 27, 7 ) ;
}

/*==============================================================*/
/*			T10: a piece along the screen's top					*/
/*==============================================================*/
static void pls13_00_t10_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
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
	ew->disp_flag = 1 ;
	ew->prio = 14 ;
	ew->disp_dir = ew->master_w->disp_dir ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->pos.x = ew->disp_dir ? scr_work.scr_left : scr_work.scr_right ;
	ew->pos.x += ew->disp_dir ? 104.99999f : -104.99999f ;
	ew->pos.y = scr_work.scr_top + DOT_Y( 16.0f ) ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->acc.y = -1.7678571f ;
	ef_color_set( pl_w, 3, 3, 1 ) ;
	pl_shadow_set( ew ) ;
	ew->be_flag = 1 ;
	ef_char_set( ew, 27, 10 ) ;
}

/*==============================================================*/
/*			T11: a piece along the screen's top					*/
/*==============================================================*/
static void pls13_00_t11_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
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
	ew->disp_flag = 1 ;
	ew->prio = 13 ;
	ew->disp_dir = ew->master_w->disp_dir ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->pos.x = ew->disp_dir ? scr_work.scr_left : scr_work.scr_right ;
	ew->pos.x += ew->disp_dir ? 104.99999f : -104.99999f ;
	ew->pos.y = scr_work.scr_top + DOT_Y( 16.0f ) ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->acc.y = -0.96428573f ;
	ef_color_set( pl_w, 2, 2, 1 ) ;
	ef_char_set( ew, 27, 9 ) ;
}

/*==============================================================*/
/*			T12: a piece along the screen's edge				*/
/*==============================================================*/
static void pls13_00_t12_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
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
	ew->disp_flag = 1 ;
	ew->prio = 15 ;
	ew->disp_dir = ew->master_w->disp_dir ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->pos.x = ew->disp_dir ? scr_work.scr_left : scr_work.scr_right ;
	ew->pos.x += ew->disp_dir ? DOT_X( 106.0f ) : -DOT_X( 106.0f ) ;
	ew->pos.y = ew->master_w->pos.y + DOT_X( 25.0f ) ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->alpha = 0.0f ;
	ew->timer0 = 30 ;
	ef_color_set( pl_w, 2, 2, 1 ) ;
	ef_char_set( ew, 27, 12 ) ;
}

/*==============================================================*/
/*			T13: her entrance's piece, behind her				*/
/*==============================================================*/
static void pls13_00_appear_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
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
	ew->disp_flag = 1 ;
	ew->prio = 14 ;
	ew->disp_dir = ew->master_w->disp_dir ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->pos.x += ew->disp_dir ? -DOT_X( 32.0f ) : DOT_X( 32.0f ) ;
	ew->pos.y += DOT_Y( 16.0f ) ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ef_char_set( ew, 27, 14 ) ;
}

/*==============================================================*/
/*			T14: a piece beside her								*/
/*==============================================================*/
static void pls13_00_t14_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
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
	ew->disp_flag = 1 ;
	ew->prio = 0 ;
	ew->disp_dir = ew->master_w->disp_dir ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->pos.x += ew->disp_dir ? DOT_X( 70.0f ) : -DOT_X( 70.0f ) ;
	ew->pos.y += DOT_Y( 6.0f ) ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ef_char_set( ew, 27, 14 ) ;
}

/*==============================================================*/
/*			T15: a piece beside her								*/
/*==============================================================*/
static void pls13_00_t15_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
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
	ew->disp_flag = 1 ;
	ew->prio = 0 ;
	ew->disp_dir = ew->master_w->disp_dir ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->pos.x += ew->disp_dir ? DOT_X( 16.0f ) : -DOT_X( 16.0f ) ;
	ew->pos.y += DOT_Y( 96.0f ) ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.x = ew->disp_dir ? DOT_X( 3.5f ) : -DOT_X( 3.5f ) ;
	ew->acc.x = ew->disp_dir ? DOT_X( 0.25f ) : -DOT_X( 0.25f ) ;
	ew->spd.y = 2.142857f ;
	ew->acc.y = DOT_Y( 0.125f ) ;
	ef_color_set( pl_w, 4, 8, 1 ) ;
	ef_char_set( ew, 27, 13 ) ;
}

static void pls13_00_turn_half_move( PLAYER_WORK *ew );
static void pls13_00_turn_move( PLAYER_WORK *ew );
static void pls13_00_magie_move( PLAYER_WORK *ew );
static void pls13_00_sublimation_move( PLAYER_WORK *ew );
static void pls13_00_t7_move( PLAYER_WORK *ew );
static void pls13_00_t8_move( PLAYER_WORK *ew );
static void pls13_00_t9_move( PLAYER_WORK *ew );
static void pls13_00_t10_move( PLAYER_WORK *ew );
static void pls13_00_t11_move( PLAYER_WORK *ew );
static void pls13_00_t12_move( PLAYER_WORK *ew );
static void pls13_00_appear_move( PLAYER_WORK *ew );
static void pls13_00_t14_move( PLAYER_WORK *ew );
static void pls13_00_t15_move( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls13_00_move( PLAYER_WORK *ew )
{
	static void ( *pls13_00_move_jmp[] )( PLAYER_WORK *ew ) = {
		pls13_00_turn_half_move, pls13_00_turn_move, pls13_00_magie_move, pls13_00_magie_move,
		pls13_00_sublimation_move, pls13_00_sublimation_move, pls13_00_sublimation_move, pls13_00_t7_move,
		pls13_00_t8_move, pls13_00_t9_move, pls13_00_t10_move, pls13_00_t11_move,
		pls13_00_t12_move, pls13_00_appear_move, pls13_00_t14_move, pls13_00_t15_move,
	} ;

	pls13_00_move_jmp[ew->type]( ew ) ;
}

/* type 0: follows her place until she is hit */
static void pls13_00_turn_half_follow( PLAYER_WORK *ew );

/*==============================================================*/
static void pls13_00_turn_half_move( PLAYER_WORK *ew )
{
	static void ( *pls13_00_turn_half_jmp[] )( PLAYER_WORK *ew ) = {
		pls13_00_turn_half_follow,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	pls13_00_turn_half_jmp[ew->routine_1]( ew ) ;
	if( pl_w->hit_box_no ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void pls13_00_turn_half_follow( PLAYER_WORK *ew )
{
	ef_char_move( ew ) ;
	if( ew->master_w->routine_2 != 3 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
}

/* type 1: the same */
static void pls13_00_turn_follow( PLAYER_WORK *ew );

/*==============================================================*/
static void pls13_00_turn_move( PLAYER_WORK *ew )
{
	static void ( *pls13_00_turn_jmp[] )( PLAYER_WORK *ew ) = {
		pls13_00_turn_follow,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	pls13_00_turn_jmp[ew->routine_1]( ew ) ;
	if( pl_w->hit_box_no ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void pls13_00_turn_follow( PLAYER_WORK *ew )
{
	ef_char_move( ew ) ;
	if( ew->master_w->routine_2 != 3 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
}

/* types 2 and 3: go when her Tour De Magie is done */
static void pls13_00_magie_pick_wait( PLAYER_WORK *ew );
static void pls13_00_magie_next_wait( PLAYER_WORK *ew );

/*==============================================================*/
static void pls13_00_magie_move( PLAYER_WORK *ew )
{
	static void ( *pls13_00_magie_jmp[] )( PLAYER_WORK *ew ) = {
		pls13_00_magie_pick_wait, pls13_00_magie_next_wait,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	pls13_00_magie_jmp[ew->routine_1]( ew ) ;
	if( pl_w->act_st != N_SUPER || pl_w->pl_sp_type != 9 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void pls13_00_magie_pick_wait( PLAYER_WORK *ew )
{
	ef_char_move( ew ) ;
	if( ew->master_w->routine_2 >= 5 ){
		if( ew->master_w->routine_2 != 10 ) ew->routine_0 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void pls13_00_magie_next_wait( PLAYER_WORK *ew )
{
	ef_char_move( ew ) ;
	if( ew->master_w->routine_2 == 4 ) ew->routine_0 ++ ;
}

/* types 4 to 6: a piece that stands for a while, then fades out */
static void pls13_00_sublimation_keep( PLAYER_WORK *ew );
static void pls13_00_sublimation_fade( PLAYER_WORK *ew );

/*==============================================================*/
static void pls13_00_sublimation_move( PLAYER_WORK *ew )
{
	static void ( *pls13_00_sublimation_jmp[] )( PLAYER_WORK *ew ) = {
		pls13_00_sublimation_keep, pls13_00_sublimation_fade,
	} ;

	PLAYER_WORK *pl_w = ew->master_w ;	/* unused */

	pls13_00_sublimation_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void pls13_00_sublimation_keep( PLAYER_WORK *ew )
{
	ef_char_move( ew ) ;
	if( !ew->timer0 -- ){
		ew->routine_1 ++ ;
		ew->timer0 = 8 ;
	}
}

/*--------------------------------------------------------------*/
static void pls13_00_sublimation_fade( PLAYER_WORK *ew )
{
	ef_char_move( ew ) ;
	ew->alpha -= 0.06f ;
	if( ew->alpha < 0.0f ) ew->alpha = 0.0f ;
	if( !ew->timer0 -- ) ew->routine_0 ++ ;
}

/* type 7: flies off, turning, then plays out its animation */
static void pls13_00_t7_fly( PLAYER_WORK *ew );
static void pls13_00_t7_stay( PLAYER_WORK *ew );

/*==============================================================*/
static void pls13_00_t7_move( PLAYER_WORK *ew )
{
	static void ( *pls13_00_t7_jmp[] )( PLAYER_WORK *ew ) = {
		pls13_00_t7_fly, pls13_00_t7_stay,
	} ;

	pls13_00_t7_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void pls13_00_t7_fly( PLAYER_WORK *ew )
{
	if( ew->timer0 -- < 0 ){
		ew->routine_1 ++ ;
		ew->pos.y = ew->master_w->pos.y ;
		ew->rot[2] = 0 ;
		ef_char_move( ew ) ;
	} else {
		ew->pos.x += ew->spd.x ;
		ew->rot[2] += 0x2000 ;
	}
}

/*--------------------------------------------------------------*/
static void pls13_00_t7_stay( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->act_st != N_WIN ){
		ew->routine_1 ++ ;
		return ;
	}
	if( ew->cg_end_flag >= 0 ) ef_char_move( ew ) ;
}

/* type 8: falls, lands with a sound and calls its pieces */
static void pls13_00_t8_fall( PLAYER_WORK *ew );
static void pls13_00_t8_stay( PLAYER_WORK *ew );

/*==============================================================*/
static void pls13_00_t8_move( PLAYER_WORK *ew )
{
	static void ( *pls13_00_t8_jmp[] )( PLAYER_WORK *ew ) = {
		pls13_00_t8_fall, pls13_00_t8_stay,
	} ;

	pls13_00_t8_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void pls13_00_t8_fall( PLAYER_WORK *ew )
{
	VEC3 pos ;

	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->timer0 -- > 0 ) ew->pos.x += ew->spd.x ;
	if( ew->master_w->pos.y + DOT_X( 8.0f ) > ew->pos.y ){
		ew->routine_1 ++ ;
		ew->pos.y = ew->master_w->pos.y + DOT_X( 8.0f ) ;
		pos.x = ew->pos.x ;
		pos.y = ew->pos.y ;
		pos.z = ew->pos.z ;
		pl_se_req( ew, 52 ) ;
		land_ef_set( &pos, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls13_00_t8_stay( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->act_st != N_WIN ){
		ew->routine_0 ++ ;
		return ;
	}
	if( ew->cg_type ){
		ew->cg_type = 0 ;
		pls13_00_set2( ew, 7 ) ;
	}
	if( ew->cg_end_flag >= 0 ) ef_char_move( ew ) ;
}

/* type 9: slides, then plays out its animation */
static void pls13_00_t9_slide( PLAYER_WORK *ew );
static void pls13_00_t9_stay( PLAYER_WORK *ew );

/*==============================================================*/
static void pls13_00_t9_move( PLAYER_WORK *ew )
{
	static void ( *pls13_00_t9_jmp[] )( PLAYER_WORK *ew ) = {
		pls13_00_t9_slide, pls13_00_t9_stay,
	} ;

	pls13_00_t9_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void pls13_00_t9_slide( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	if( ew->timer0 -- <= 0 ) ew->routine_1 ++ ;
}

/*--------------------------------------------------------------*/
static void pls13_00_t9_stay( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->act_st != N_WIN ){
		ew->routine_0 ++ ;
		return ;
	}
	if( ew->cg_end_flag >= 0 ) ef_char_move( ew ) ;
}

/* type 10: falls to her ground and calls its pieces by the animation's codes */
static void pls13_00_t10_fall( PLAYER_WORK *ew );
static void pls13_00_t10_stay( PLAYER_WORK *ew );

/*==============================================================*/
static void pls13_00_t10_move( PLAYER_WORK *ew )
{
	static void ( *pls13_00_t10_jmp[] )( PLAYER_WORK *ew ) = {
		pls13_00_t10_fall, pls13_00_t10_stay,
	} ;

	pls13_00_t10_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void pls13_00_t10_fall( PLAYER_WORK *ew )
{
	VEC3 pos ;

	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->timer0 -- > 0 ) ew->pos.x += ew->spd.x ;
	if( ew->master_w->pos.y > ew->pos.y ){
		ew->routine_1 ++ ;
		ew->pos.y = ew->master_w->pos.y ;
		pos.x = ew->pos.x ;
		pos.y = ew->pos.y ;
		pos.z = ew->pos.z ;
		pl_se_req( ew, 52 ) ;
		land_ef_set( &pos, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls13_00_t10_stay( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->act_st != N_WIN ){
		ew->routine_0 ++ ;
		return ;
	}
	if( ew->cg_type == 1 ){
		ew->cg_type = 0 ;
		pls13_00_set2( ew, 9 ) ;
		pls13_00_set2( ew, 8 ) ;
	}
	if( ew->cg_type == 2 ){
		ew->cg_type = 0 ;
		pls13_00_set2( ew, 11 ) ;
	}
	if( ew->cg_end_flag >= 0 ) ef_char_move( ew ) ;
}

/* type 11: falls, calls a piece when it is 240 over her ground */
static void pls13_00_t11_fall( PLAYER_WORK *ew );
static void pls13_00_t11_stay( PLAYER_WORK *ew );

/*==============================================================*/
static void pls13_00_t11_move( PLAYER_WORK *ew )
{
	static void ( *pls13_00_t11_jmp[] )( PLAYER_WORK *ew ) = {
		pls13_00_t11_fall, pls13_00_t11_stay,
	} ;

	pls13_00_t11_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void pls13_00_t11_fall( PLAYER_WORK *ew )
{
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->timer0 -- > 0 ) ew->pos.x += ew->spd.x ;
	if( ew->master_w->pos.y + 240.0f > ew->pos.y ){
		ew->routine_1 ++ ;
		pls13_00_set2( ew, 12 ) ;
		ew->pos.y = ew->master_w->pos.y ;
		ef_char_move( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls13_00_t11_stay( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->act_st != N_WIN ){
		ew->routine_0 ++ ;
		return ;
	}
	if( ew->cg_end_flag >= 0 ) ef_char_move( ew ) ;
}

/* type 12: waits, then fades in */
static void pls13_00_t12_wait( PLAYER_WORK *ew );
static void pls13_00_t12_fade_in( PLAYER_WORK *ew );

/*==============================================================*/
static void pls13_00_t12_move( PLAYER_WORK *ew )
{
	static void ( *pls13_00_t12_jmp[] )( PLAYER_WORK *ew ) = {
		pls13_00_t12_wait, pls13_00_t12_fade_in,
	} ;

	pls13_00_t12_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void pls13_00_t12_wait( PLAYER_WORK *ew )
{
	if( ew->timer0 -- <= 0 ){
		ew->routine_1 ++ ;
		pls13_00_t12_fade_in( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls13_00_t12_fade_in( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->act_st != N_WIN ){
		ew->routine_0 ++ ;
		return ;
	}
	ew->alpha += 0.03f ;
	if( ew->alpha > 1.0f ) ew->alpha = 1.0f ;
	if( ew->cg_end_flag >= 0 ) ef_char_move( ew ) ;
}

/* type 13: flies off when her animation gives the code */
static void pls13_00_appear_wait( PLAYER_WORK *ew );
static void pls13_00_appear_fly( PLAYER_WORK *ew );

/*==============================================================*/
static void pls13_00_appear_move( PLAYER_WORK *ew )
{
	static void ( *pls13_00_appear_jmp[] )( PLAYER_WORK *ew ) = {
		pls13_00_appear_wait, pls13_00_appear_fly,
	} ;

	PLAYER_WORK *pl_w = ew->master_w ;	/* unused */

	pls13_00_appear_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void pls13_00_appear_wait( PLAYER_WORK *ew )
{
	if( ew->master_w->cg_type == 1 ){
		ew->routine_1 ++ ;
		ew->spd.x = ew->disp_dir ? -DOT_X( 9.5f ) : DOT_X( 9.5f ) ;
		ew->acc.x = ew->disp_dir ? -0.3125f : 0.3125f ;
		ew->spd.y = DOT_Y( 3.0f ) ;
		ew->acc.y = -DOT_Y( 0.25f ) ;
		ew->timer0 = 30 ;
	}
}

/*--------------------------------------------------------------*/
static void pls13_00_appear_fly( PLAYER_WORK *ew )
{
	if( !ew->timer0 -- ) ew->routine_0 ++ ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
}

/* type 14: plays out its animation while she is in the win pose */
static void pls13_00_t14_stay( PLAYER_WORK *ew );

/*==============================================================*/
static void pls13_00_t14_move( PLAYER_WORK *ew )
{
	static void ( *pls13_00_t14_jmp[] )( PLAYER_WORK *ew ) = {
		pls13_00_t14_stay,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	pls13_00_t14_jmp[ew->routine_1]( ew ) ;
	if( pl_w->act_st != N_WIN ) ew->routine_0 ++ ;
}

/*==============================================================*/
static void pls13_00_t14_stay( PLAYER_WORK *ew )
{
	if( ew->cg_end_flag >= 0 ) ef_char_move( ew ) ;
}

/* type 15: waits for the animation's code, then flies on until its timer is out */
static void pls13_00_t15_wait( PLAYER_WORK *ew );
static void pls13_00_t15_fly( PLAYER_WORK *ew );

/*==============================================================*/
static void pls13_00_t15_move( PLAYER_WORK *ew )
{
	static void ( *pls13_00_t15_jmp[] )( PLAYER_WORK *ew ) = {
		pls13_00_t15_wait, pls13_00_t15_fly,
	} ;

	PLAYER_WORK *pl_w = ew->master_w ;	/* unused */

	pls13_00_t15_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void pls13_00_t15_wait( PLAYER_WORK *ew )
{
	if( ew->cg_type == 1 ){
		ew->routine_1 ++ ;
		ew->timer0 = 60 ;
	}
	ef_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls13_00_t15_fly( PLAYER_WORK *ew )
{
	if( !ew->timer0 -- ) ew->routine_0 ++ ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	ef_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls13_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls13_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls13_00.c" End of File	*/

/* Hayato's player effect 00 (character 12's player effect 00): ten pieces, chosen by type: the punch
   throw's, Shiden's slash, the bike he rides in on and its wheels, the piece that fades as he lands,
   and the figures of his win poses. Each type has its own init and move; the move runs its steps by
   routine_1. */
#include "player.h"
#include "ef_char.h"
#include "pl_color.h"

static void pls12_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls12_00_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls12_00_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
	}
	return ew ;
}

static void pls12_00_init( PLAYER_WORK *ew );
static void pls12_00_move( PLAYER_WORK *ew );
static void pls12_00_end( PLAYER_WORK *ew );
static void pls12_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls12_00_main( PLAYER_WORK *ew )
{
	static void ( *pls12_00_jmp[] )( PLAYER_WORK *ew ) = {
		pls12_00_init, pls12_00_move, pls12_00_end, pls12_00_free,
	} ;

	pls12_00_jmp[ew->routine_0]( ew ) ;
}

static void pls12_00_catch_init( PLAYER_WORK *ew );
static void pls12_00_shiden_init( PLAYER_WORK *ew );
static void pls12_00_bike_init( PLAYER_WORK *ew );
static void pls12_00_wheel_f_init( PLAYER_WORK *ew );
static void pls12_00_wheel_r_init( PLAYER_WORK *ew );
static void pls12_00_fade_init( PLAYER_WORK *ew );
static void pls12_00_t6_init( PLAYER_WORK *ew );
static void pls12_00_t7_init( PLAYER_WORK *ew );
static void pls12_00_t8_init( PLAYER_WORK *ew );
static void pls12_00_fade_win_init( PLAYER_WORK *ew );

/*==============================================================*/
static void pls12_00_init( PLAYER_WORK *ew )
{
	static void ( *pls12_00_init_jmp[] )( PLAYER_WORK *ew ) = {
		pls12_00_catch_init, pls12_00_shiden_init, pls12_00_bike_init, pls12_00_wheel_f_init, pls12_00_wheel_r_init,
		pls12_00_fade_init, pls12_00_t6_init, pls12_00_t7_init, pls12_00_t8_init, pls12_00_fade_win_init,
	} ;

	pls12_00_init_jmp[ew->type]( ew ) ;
}

/*==============================================================*/
/*			The punch throw's piece, on him						*/
/*==============================================================*/
static void pls12_00_catch_init( PLAYER_WORK *ew )
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
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ef_char_set( ew, 27, 16 ) ;
}

/*==============================================================*/
/*			Shiden's slash: a piece beside him that plays the	*/
/*			animation numbered by his timer0					*/
/*==============================================================*/
static void pls12_00_shiden_init( PLAYER_WORK *ew )
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
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->pos.x += ew->master_w->disp_dir ? -48 : 48 ;
	ef_char_set( ew, 27, ew->master_w->timer0 ) ;
}

/*==============================================================*/
/*			The bike: starts at his height and calls his voice	*/
/*==============================================================*/
static void pls12_00_bike_init( PLAYER_WORK *ew )
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
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->spd.y = ew->master_w->pos.y ;
	ew->timer0 = 120 ;
	pl_voice_req( ew->master_w, 38 ) ;
	ef_char_set( ew, 27, 9 ) ;
}

/*==============================================================*/
/*			The front wheel: 110 in front of him				*/
/*==============================================================*/
static void pls12_00_wheel_f_init( PLAYER_WORK *ew )
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
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->pos.x += ew->master_w->disp_dir ? 110.0f : -110.0f ;
	ew->pos.y -= DOT_Y( 8.0f ) ;
	ew->timer0 = 120 ;
	ef_char_set( ew, 27, 10 ) ;
}

/*==============================================================*/
/*			The rear wheel: the same piece, 175 behind him		*/
/*==============================================================*/
static void pls12_00_wheel_r_init( PLAYER_WORK *ew )
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
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->pos.x += ew->master_w->disp_dir ? -175.0f : 175.0f ;
	ew->pos.y -= DOT_Y( 8.0f ) ;
	ew->timer0 = 120 ;
	ef_char_set( ew, 27, 10 ) ;
}

/*==============================================================*/
/*			The fading piece: on him, a little smaller			*/
/*==============================================================*/
static void pls12_00_fade_init( PLAYER_WORK *ew )
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
	ew->alpha = 1.0f ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->scale.x = 0.8f ;
	ew->scale.y = 0.8f ;
	ef_char_set( ew, 27, 14 ) ;
}

/*==============================================================*/
/*			Type 6: a piece put out in front of him, with		*/
/*			his colour changed									*/
/*==============================================================*/
static void pls12_00_t6_init( PLAYER_WORK *ew )
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
	ew->prio = 7 ;
	ew->alpha = 1.0f ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->pos.x += ew->master_w->disp_dir ? 160.0f : -160.0f ;
	ew->scale.y = 1.0f ;
	ef_color_set( pl_w, 4, 4, 1 ) ;
	ef_char_set( ew, 27, 11 ) ;
	pl_shadow_set( ew ) ;
	ew->be_flag = 1 ;
}

/*==============================================================*/
/*			Type 7: a piece put out in front of him, with		*/
/*			two colours of his changed							*/
/*==============================================================*/
static void pls12_00_t7_init( PLAYER_WORK *ew )
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
	ew->prio = 7 ;
	ew->alpha = 1.0f ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->pos.x += ew->master_w->disp_dir ? 160.0f : -160.0f ;
	ew->scale.y = 1.0f ;
	ef_color_set( pl_w, 3, 3, 1 ) ;
	ef_color_set( pl_w, 5, 5, 1 ) ;
	ef_char_set( ew, 27, 12 ) ;
	pl_shadow_set( ew ) ;
	ew->be_flag = 1 ;
}

/*==============================================================*/
/*			Type 8: a piece put out in front of him, with		*/
/*			his colour changed									*/
/*==============================================================*/
static void pls12_00_t8_init( PLAYER_WORK *ew )
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
	ew->prio = 7 ;
	ew->alpha = 1.0f ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->pos.x += ew->master_w->disp_dir ? 160.0f : -160.0f ;
	ew->scale.y = 1.0f ;
	ef_color_set( pl_w, 6, 6, 1 ) ;
	ef_char_set( ew, 27, 13 ) ;
	pl_shadow_set( ew ) ;
	ew->be_flag = 1 ;
}

/*==============================================================*/
/*			The win poses' fading piece: put out in front of	*/
/*			him, a little smaller								*/
/*==============================================================*/
static void pls12_00_fade_win_init( PLAYER_WORK *ew )
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
	ew->alpha = 1.0f ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->pos.x += ew->master_w->disp_dir ? 160.0f : -160.0f ;
	ew->scale.x = 0.8f ;
	ew->scale.y = 0.8f ;
	ef_char_set( ew, 27, 14 ) ;
}

static void pls12_00_catch_move( PLAYER_WORK *ew );
static void pls12_00_shiden_move( PLAYER_WORK *ew );
static void pls12_00_bike_move( PLAYER_WORK *ew );
static void pls12_00_wheel_move( PLAYER_WORK *ew );
static void pls12_00_fade_move( PLAYER_WORK *ew );
static void pls12_00_t6_move( PLAYER_WORK *ew );
static void pls12_00_t7_move( PLAYER_WORK *ew );
static void pls12_00_t8_move( PLAYER_WORK *ew );

/*==============================================================*/
/*			The effect goes when Hayato is no longer the		*/
/*			character it rides on								*/
/*==============================================================*/
static void pls12_00_move( PLAYER_WORK *ew )
{
	static void ( *pls12_00_move_jmp[] )( PLAYER_WORK *ew ) = {
		pls12_00_catch_move, pls12_00_shiden_move, pls12_00_bike_move, pls12_00_wheel_move, pls12_00_wheel_move,
		pls12_00_fade_move, pls12_00_t6_move, pls12_00_t7_move, pls12_00_t8_move, pls12_00_fade_move,
	} ;

	if( ew->pl_type != ew->master_w->pl_type ){
		pls12_00_free( ew ) ;
		return ;
	}
	pls12_00_move_jmp[ew->type]( ew ) ;
}

/* the punch throw's piece: takes the animations his frames ask for, while he holds the opponent */
static void pls12_00_catch_char( PLAYER_WORK *ew );

/*==============================================================*/
static void pls12_00_catch_move( PLAYER_WORK *ew )
{
	static void ( *pls12_00_catch_jmp[] )( PLAYER_WORK *ew ) = {
		pls12_00_catch_char,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	pls12_00_catch_jmp[ew->routine_1]( ew ) ;
	if( pl_w->act_st != N_CATCH ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void pls12_00_catch_char( PLAYER_WORK *ew )
{
	if( ew->master_w->cg_type == 2 ) ef_char_set( ew, 27, 17 ) ;
	if( ew->master_w->cg_type == 3 ) ef_char_set( ew, 27, 18 ) ;
	if( ew->master_w->cg_type == 4 ) ef_char_set( ew, 27, 19 ) ;
	if( ew->master_w->cg_type == 5 ) ef_char_set( ew, 27, 20 ) ;
	if( ew->master_w->cg_type == 6 ) ef_char_set( ew, 27, 21 ) ;
	if( ew->master_w->cg_type == 7 ) ef_char_set( ew, 27, 21 ) ;
	if( ew->master_w->cg_type == 8 ) ef_char_set( ew, 27, 22 ) ;
	if( ew->master_w->cg_type == 9 ) ew->routine_0 ++ ;
}

/* Shiden's slash: plays its animation out */
static void pls12_00_shiden_play( PLAYER_WORK *ew );

/*==============================================================*/
static void pls12_00_shiden_move( PLAYER_WORK *ew )
{
	static void ( *pls12_00_shiden_jmp[] )( PLAYER_WORK *ew ) = {
		pls12_00_shiden_play,
	} ;

	pls12_00_shiden_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void pls12_00_shiden_play( PLAYER_WORK *ew )
{
	ef_char_move( ew ) ;
	if( ew->cg_end_flag < 0 ) ew->routine_0 ++ ;
}

/* the bike: slides along at the height it began at, shaking, and goes when its timer is out */
static void pls12_00_bike_run( PLAYER_WORK *ew );

/*==============================================================*/
static void pls12_00_bike_move( PLAYER_WORK *ew )
{
	static void ( *pls12_00_bike_jmp[] )( PLAYER_WORK *ew ) = {
		pls12_00_bike_run,
	} ;

	pls12_00_bike_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void pls12_00_bike_run( PLAYER_WORK *ew )
{
	ef_char_move( ew ) ;
	ew->pos.x += ew->master_w->disp_dir ? 18 : -18 ;
	ew->pos.y = ew->spd.y ;
	if( !( Sys->game_timer0 & 1 ) ) ew->pos.y += 3.0f ;
	if( !ew->timer0 -- ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/* the wheels: slide along, turn and go when the timer is out */
static void pls12_00_wheel_turn( PLAYER_WORK *ew );

/*==============================================================*/
static void pls12_00_wheel_move( PLAYER_WORK *ew )
{
	static void ( *pls12_00_wheel_jmp[] )( PLAYER_WORK *ew ) = {
		pls12_00_wheel_turn,
	} ;

	pls12_00_wheel_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void pls12_00_wheel_turn( PLAYER_WORK *ew )
{
	ef_char_move( ew ) ;
	ew->pos.x += ew->master_w->disp_dir ? 18 : -18 ;
	ew->rot[2] += 0x2000 ;
	if( !ew->timer0 -- ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/* the fading pieces: fade out and go at the end of the animation */
static void pls12_00_fade_out( PLAYER_WORK *ew );

/*==============================================================*/
static void pls12_00_fade_move( PLAYER_WORK *ew )
{
	static void ( *pls12_00_fade_jmp[] )( PLAYER_WORK *ew ) = {
		pls12_00_fade_out,
	} ;

	pls12_00_fade_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void pls12_00_fade_out( PLAYER_WORK *ew )
{
	ef_char_move( ew ) ;
	ew->alpha -= 0.02f ;
	if( ew->alpha < 0.0f ) ew->alpha = 0.0f ;
	if( ew->cg_end_flag < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/* types 6 to 8: play their animation while he is in his win pose, then give back his colour */
static void pls12_00_t6_keep( PLAYER_WORK *ew );

/*==============================================================*/
static void pls12_00_t6_move( PLAYER_WORK *ew )
{
	static void ( *pls12_00_t6_jmp[] )( PLAYER_WORK *ew ) = {
		pls12_00_t6_keep,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	pls12_00_t6_jmp[ew->routine_1]( ew ) ;
	if( pl_w->act_st != N_WIN ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		pl_color_set( pl_w, 0 ) ;
	}
}

/*==============================================================*/
static void pls12_00_t6_keep( PLAYER_WORK *ew )
{
	ef_char_move( ew ) ;
}

static void pls12_00_t7_keep( PLAYER_WORK *ew );

/*==============================================================*/
static void pls12_00_t7_move( PLAYER_WORK *ew )
{
	static void ( *pls12_00_t7_jmp[] )( PLAYER_WORK *ew ) = {
		pls12_00_t7_keep,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	pls12_00_t7_jmp[ew->routine_1]( ew ) ;
	if( pl_w->act_st != N_WIN ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		pl_color_set( pl_w, 0 ) ;
	}
}

/*==============================================================*/
static void pls12_00_t7_keep( PLAYER_WORK *ew )
{
	ef_char_move( ew ) ;
}

static void pls12_00_t8_wait( PLAYER_WORK *ew );
static void pls12_00_t8_keep( PLAYER_WORK *ew );

/*==============================================================*/
static void pls12_00_t8_move( PLAYER_WORK *ew )
{
	static void ( *pls12_00_t8_jmp[] )( PLAYER_WORK *ew ) = {
		pls12_00_t8_wait, pls12_00_t8_keep,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	pls12_00_t8_jmp[ew->routine_1]( ew ) ;
	if( pl_w->act_st != N_WIN ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		pl_color_set( pl_w, 0 ) ;
	}
}

/*==============================================================*/
static void pls12_00_t8_wait( PLAYER_WORK *ew )
{
	if( ew->cg_type == 1 ){
		ew->routine_1 ++ ;
		ew->cg_type = 0 ;
	}
	ef_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls12_00_t8_keep( PLAYER_WORK *ew )
{
	ef_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls12_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls12_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls12_00.c" End of File	*/

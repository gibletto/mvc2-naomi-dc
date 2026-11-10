/* Blackheart's effect 02 (character 35's shot 02): the fiends of his Armageddon. Four kinds,
   by type: the one that drops from over him to the ground where the opponent is, the two that fly
   in to his sides, the bats the dropped one puts out, and the blast at its place at the end. */
#include "player.h"
#include "pl_char_old.h"
#include "o_sub1_old.h"
#include "pl35_def.h"

/* the effect's own work, from obj_flag: where a side one waits beside its master */
typedef struct SHL35_02_LOCAL_W {
	Sint32	unk_00;
	Float	x;
	Float	y;
} SHL35_02_LOCAL_W;
#define SHL35_02_LW( w )	( ( SHL35_02_LOCAL_W * )&( w )->obj_flag )

static Uint8 shl35_02_no_0 ;
static Uint8 shl35_02_no_1 ;

static void shl35_02_main( PLAYER_WORK *ew );

/****************************************************************/
/* three of them at once, or 0 if the pool is short */
PLAYER_WORK *shl35_02_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( work_rest_cnt < 3 ) return 0 ;
	ew = work_get( 0, W_SHL, LIST_END ) ;
	ew->work_func = ( void * )shl35_02_main ;
	ew->master_w = pl_w ;
	ew->ef_w = pl_w ;
	ew->pl_type = pl_w->pl_type ;
	ew->ef_no = 0x3502 ;
	ew->type = 0 ;
	ew->sub_type = 0 ;
	ew = work_get( 0, W_SHL, LIST_END ) ;
	ew->work_func = ( void * )shl35_02_main ;
	ew->master_w = pl_w ;
	ew->ef_w = pl_w ;
	ew->pl_type = pl_w->pl_type ;
	ew->ef_no = 0x3502 ;
	ew->type = 1 ;
	ew->sub_type = 0 ;
	ew = work_get( 0, W_SHL, LIST_END ) ;
	ew->work_func = ( void * )shl35_02_main ;
	ew->master_w = pl_w ;
	ew->ef_w = pl_w ;
	ew->pl_type = pl_w->pl_type ;
	ew->ef_no = 0x3502 ;
	ew->type = 1 ;
	ew->sub_type = 1 ;
	return ew ;
}

/*==============================================================*/
/*			The blast, made by the dropped one at its place		*/
/*			when the fiends are to leave						*/
/*==============================================================*/
static PLAYER_WORK *shl35_02_blast_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( work_rest_cnt < 2 ) return 0 ;
	ew = work_get( 0, W_SHL, LIST_END ) ;
	ew->work_func = ( void * )shl35_02_main ;
	ew->master_w = pl_w ;
	ew->ef_w = pl_w->ef_w ;
	ew->pl_type = pl_w->pl_type ;
	ew->ef_no = 0x3502 ;
	ew->type = 3 ;
	ew->sub_type = 0 ;
	return ew ;
}

static void shl35_02_dummy( PLAYER_WORK *ew );
static void shl35_02_init( PLAYER_WORK *ew );
static void shl35_02_move( PLAYER_WORK *ew );
static void shl35_02_end( PLAYER_WORK *ew );
static void shl35_02_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl35_02_main( PLAYER_WORK *ew )
{
	static void ( *shl35_02_jmp[] )( PLAYER_WORK *ew ) = {
		shl35_02_init, shl35_02_move, shl35_02_end, shl35_02_free,
	} ;

	shl35_02_dummy( ew ) ;
	shl35_02_jmp[ew->routine_0]( ew ) ;
}

static void shl35_02_drop_init( PLAYER_WORK *ew );
static void shl35_02_side_init( PLAYER_WORK *ew );
static void shl35_02_bat_init( PLAYER_WORK *ew );
static void shl35_02_blast_init( PLAYER_WORK *ew );

/*==============================================================*/
static void shl35_02_init( PLAYER_WORK *ew )
{
	static void ( *shl35_02_init_jmp[] )( PLAYER_WORK *ew ) = {
		shl35_02_drop_init, shl35_02_side_init, shl35_02_bat_init, shl35_02_blast_init,
	} ;

	shl35_02_init_jmp[ew->type]( ew ) ;
	ew->routine_0 ++ ;
	ew->routine_1 = 0 ;
}

static void shl35_02_copy( PLAYER_WORK *ew );

/*==============================================================*/
/*			The dropped one: flies from his master at the		*/
/*			opponent											*/
/*==============================================================*/
static void shl35_02_drop_init( register PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->ef_w ;
	PLAYER_WORK *em_w = pl_w->target_w ;
	register Float y ;
	Float x, t ;

	shl35_02_copy( ew ) ;
	ew->prio = 11 ;
	ew->disp_flag = 0 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y + DOT_Y( 128.0f ) ;
	x = ( em_w->pos.x - pl_w->pos.x ) / 8.0f ;
	t = x / 2.0f ;
	x += t ;
	ew->spd.x = x ;
	ew->acc.x = 0.0f ;
	y = ( pl_w->ground_y - ew->pos.y ) / 8.0f ;
	t = y / 2.0f ;
	y += t ;
	ew->spd.y = y ;
	ew->acc.y = 0.0f ;
	ew->atck_no = pl35_at_armageddon_drop ;
	HIT_RESET( ew ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	pl_char_set( ew, Char_Effect, 22 ) ;
}

/*==============================================================*/
/*			The side ones: come in at the edge of the screen	*/
/*==============================================================*/
static void shl35_02_side_init( PLAYER_WORK *ew )
{
	SHL35_02_LOCAL_W *lw = SHL35_02_LW( ew ) ;
	PLAYER_WORK *pl_w = ew->ef_w ;

	shl35_02_copy( ew ) ;
	if( pl_w->pl_dir ) ew->pos.x = scr_work.scr_right + DOT_X( 32.0f ) ;
	else ew->pos.x = scr_work.scr_left - DOT_X( 32.0f ) ;
	ew->pos.y = scr_work.scr_top + DOT_Y( 64.0f ) ;
	ew->dir = Rnd() & 31 ;
	if( !ew->sub_type ){
		lw->x = -DOT_X( 32.0f ) ;
		lw->y = DOT_X( 104.0f ) ;
		ew->prio = 7 ;
		pl_char_set( ew, 18, 22 ) ;
	}
	else{
		lw->x = DOT_X( 32.0f ) ;
		lw->y = DOT_X( 104.0f ) ;
		ew->prio = 0 ;
		pl_char_set( ew, 18, 37 ) ;
	}
}

/*==============================================================*/
/*			The bats											*/
/*==============================================================*/
static void shl35_02_bat_init( register PLAYER_WORK *ew )
{
	static Uint8 shl35_02_anim_tbl[16] = { 22, 30, 37, 44, 57, 66, 22, 30, 37, 44, 57, 66, 22, 30, 37, 44 } ;
	static Sint32 shl35_02_no_tbl[16] = { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2 } ;
	PLAYER_WORK *pl_w ;
	Uint8 no ;

	shl35_02_copy( ew ) ;
	ew->disp_flag = 1 ;
	ew->routine_2 = 0 ;
	ew->prio = 11 ;
	pl_w = ew->master_w ;
	ew->pos.x = pl_w->pos.x + ( Sint32 )( Rnd() & 31 ) * 1.6666666f ;
	ew->pos.y = pl_w->pos.y + ( Sint32 )( Rnd() & 15 ) * 2.142857f ;
	ew->spd.y = DOT_Y( 4.0f ) ;
	ew->acc.y = DOT_Y( 0.125f ) ;
	ew->spd.x = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->timer0 = 10 ;
	if( Sys->game_timer0 & 3 ){
		ew->atck_no = pl35_at_armageddon_bat ;
		HIT_RESET( ew ) ;
	} else {
		ew->atck_no = pl35_at_armageddon_bat_2 ;
		HIT_RESET( ew ) ;
	}
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	no = Rnd() & 15 ;
	ew->sub_type = shl35_02_no_tbl[no] ;
	pl_char_set( ew, 18, shl35_02_anim_tbl[no] ) ;
}

/*==============================================================*/
/*			The blast, at the dropped one's place				*/
/*==============================================================*/
static void shl35_02_blast_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	shl35_02_copy( ew ) ;
	ew->prio = 11 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->scale.y = 2.4f ;
	pl_se_req( ew, 76 ) ;
	ew->atck_no = pl35_at_armageddon_blast ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, 21, 25 ) ;
}

static void shl35_02_drop_move( PLAYER_WORK *ew );
static void shl35_02_side_move( PLAYER_WORK *ew );
static void shl35_02_bat_move( PLAYER_WORK *ew );
static void shl35_02_blast_move( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl35_02_move( PLAYER_WORK *ew )
{
	static void ( *shl35_02_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl35_02_drop_move, shl35_02_side_move, shl35_02_bat_move, shl35_02_blast_move,
	} ;

	shl35_02_move_jmp[ew->type]( ew ) ;
}

static void shl35_02_drop_fall( PLAYER_WORK *ew );
static void shl35_02_drop_stay( PLAYER_WORK *ew );
static void shl35_02_drop_end( PLAYER_WORK *ew );

/*==============================================================*/
static void shl35_02_drop_move( PLAYER_WORK *ew )
{
	static void ( *shl35_02_drop_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl35_02_drop_fall, shl35_02_drop_stay, shl35_02_drop_end,
	} ;

	shl35_02_drop_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
/*			The dropped one flies down to the ground			*/
/*==============================================================*/
static void shl35_02_drop_fall( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->ef_w ;

	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	ew->disp_flag = 1 ;
	if( pl_w->ground_y < ew->pos.y ){
		pl_char_move( ew ) ;
		return ;
	}
	ew->routine_1 ++ ;
	ew->prio = 13 ;
	ew->pos.y = pl_w->ground_y ;
	pl_char_set( ew, 21, 23 ) ;
}

/*==============================================================*/
/*			And stays there while Blackheart is in the move,	*/
/*			putting out a bat every 8 frames; the blast is		*/
/*			made when the fiends are to leave					*/
/*==============================================================*/
static void shl35_02_drop_stay( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->ef_w ;
	PL35_LOCAL_W *pl_lw = ( PL35_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *nw ;

	if( pl_w->routine_1 || pl_w->act_st != N_SUPER || pl_w->pl_sp_type != 1 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	} else if( pl_lw->fiend_end_flag ){
		ew->routine_1 ++ ;
		ew->timer0 = 32 ;
		pl_char_set( ew, 21, 24 ) ;
		shl35_02_blast_set( ew ) ;
	} else {
		if( !( Sys->game_timer0 & 7 ) ){
			if( ( nw = work_get( 0, W_SHL, LIST_END ) ) ){
				nw->work_func = shl35_02_main ;
				nw->master_w = ew ;
				nw->ef_w = ew->ef_w ;
				nw->pl_type = pl_w->pl_type ;
				nw->type = 2 ;
				nw->sub_type = 0 ;
			}
		}
		hit_work_set( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl35_02_drop_end( PLAYER_WORK *ew )
{
	if( ! -- ew->timer0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

static void shl35_02_side_follow( PLAYER_WORK *ew );
static void shl35_02_side_stay( PLAYER_WORK *ew );
static void shl35_02_side_leave( PLAYER_WORK *ew );

/*==============================================================*/
static void shl35_02_side_move( PLAYER_WORK *ew )
{
	static void ( *shl35_02_side_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl35_02_side_follow, shl35_02_side_stay, shl35_02_side_leave,
	} ;
	PLAYER_WORK *pl_w ;

	if( ew->routine_1 != 2 ){
		pl_w = ew->ef_w ;
		if( pl_w->routine_1 || pl_w->pl_sp_type != 1 || pl_w->act_st != N_SUPER ){
			ew->routine_1 = 2 ;
			ew->spd.y = -DOT_Y( 2.0f ) ;
			ew->acc.y = DOT_Y( 0.25f ) ;
			ew->spd.x = 0.0f ;
			ew->acc.x = 0.0f ;
		}
	}
	shl35_02_side_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
/*			A side one flies to its place beside him, turning	*/
/*			towards it											*/
/*==============================================================*/
static void shl35_02_side_follow( PLAYER_WORK *ew )
{
	SHL35_02_LOCAL_W *lw = SHL35_02_LW( ew ) ;
	PLAYER_WORK *pl_w = ew->ef_w ;
	VEC3 pos ;

	pos.x = lw->x ;
	if( !pl_w->pl_dir ) pos.x = -pos.x ;
	pos.x += pl_w->pos.x ;
	pos.y = pl_w->pos.y + lw->y ;
	ew->dir = ( caldir_pos_256( &ew->pos, &pos ) + 4 ) >> 3 ;
	dir_spd_move( ew, 400 ) ;
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl35_02_side_stay( PLAYER_WORK *ew )
{
	SHL35_02_LOCAL_W *lw = SHL35_02_LW( ew ) ;
	PLAYER_WORK *pl_w = ew->ef_w ;
	Float x = lw->x ;

	if( !pl_w->pl_dir ) x = -x ;
	ew->pos.x = pl_w->pos.x + x ;
	ew->pos.y = pl_w->pos.y + lw->y ;
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl35_02_side_leave( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( scr_work.scr_top < ew->pos.y ){
		ew->routine_0 ++ ;
		ew->routine_1 = 0 ;
	}
	else pl_char_move( ew ) ;
}

static void shl35_02_bat_blink( PLAYER_WORK *ew );
static void shl35_02_bat_fly( PLAYER_WORK *ew );
static void shl35_02_bat_vanish( PLAYER_WORK *ew );
static void shl35_02_bat_mv_fast( PLAYER_WORK *ew );
static void shl35_02_bat_mv( PLAYER_WORK *ew );
static void shl35_02_bat_circle( PLAYER_WORK *ew );

/*==============================================================*/
static void shl35_02_bat_move( PLAYER_WORK *ew )
{
	static void ( *shl35_02_bat_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl35_02_bat_blink, shl35_02_bat_fly, shl35_02_bat_vanish,
	} ;

	shl35_02_bat_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
/*			The bats flicker in									*/
/*==============================================================*/
static void shl35_02_bat_blink( PLAYER_WORK *ew )
{
	if( -- ew->timer0 ){
		if( Sys->game_timer0 & 1 ) ew->disp_flag = 0 ;
		else ew->disp_flag = 1 ;
	}
	else{
		ew->routine_1 ++ ;
		ew->disp_flag = 1 ;
		shl35_02_bat_fly( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl35_02_bat_fly( PLAYER_WORK *ew )
{
	static void ( *shl35_02_bat_fly_jmp[] )( PLAYER_WORK *ew ) = {
		shl35_02_bat_mv_fast, shl35_02_bat_mv, shl35_02_bat_circle,
	} ;
	PLAYER_WORK *mw = ew->master_w ;
	PLAYER_WORK *pl_w = ew->ef_w ;
	PLAYER_WORK *em_w = pl_w->target_w ;
	PL35_LOCAL_W *pl_lw = ( PL35_LOCAL_W * )pl_w->tlocal ;

	if( mw->routine_0 != 1 ){
		ew->routine_1 ++ ;
		ew->timer0 = 3 ;
		shl35_02_bat_vanish( ew ) ;
	} else if( pl_lw->fiend_end_flag ){
		ew->routine_1 ++ ;
		ew->timer0 = 3 ;
		shl35_02_bat_vanish( ew ) ;
	} else {
		shl35_02_bat_fly_jmp[ew->sub_type]( ew ) ;
		if( !( Sys->game_timer0 & 31 ) ){
			if( Rnd() & 3 ){
				ew->atck_no = pl35_at_armageddon_bat ;
				goto hit ;
			} else ew->atck_no = pl35_at_armageddon_bat_2 ;
		hit :
			HIT_RESET( ew ) ;
		}
		pl_char_move( ew ) ;
		if( !em_w->hit_stop ) hit_work_set( ew ) ;
	}
}

/*==============================================================*/
static void shl35_02_bat_mv_fast( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	shl35_02_bat_mv( ew ) ;
}

static void shl35_02_bat_mv_init( PLAYER_WORK *ew );
static void shl35_02_bat_mv_rise( PLAYER_WORK *ew );
static void shl35_02_bat_mv_turn( PLAYER_WORK *ew );
static void shl35_02_bat_mv_fly( PLAYER_WORK *ew );

/*==============================================================*/
static void shl35_02_bat_mv( PLAYER_WORK *ew )
{
	static void ( *shl35_02_bat_mv_jmp[] )( PLAYER_WORK *ew ) = {
		shl35_02_bat_mv_init, shl35_02_bat_mv_rise, shl35_02_bat_mv_turn, shl35_02_bat_mv_fly,
	} ;

	shl35_02_bat_mv_jmp[ew->routine_2]( ew ) ;
}

/*==============================================================*/
static void shl35_02_bat_mv_init( PLAYER_WORK *ew )
{
	static Float shl35_02_x_tbl[8][2] = {
		{ 1.6666666f, 0.0f }, { DOT_X( 0.5f ), 0.0f }, { DOT_X( 0.25f ), 0.0f }, { 0.0f, 0.0f },
		{ 0.0f, 0.0f }, { -DOT_X( 0.25f ), 0.0f }, { -DOT_X( 0.5f ), 0.0f }, { -1.6666666f, 0.0f },
	} ;
	static Float shl35_02_y_tbl[8][2] = {
		{ DOT_Y( 8.0f ), -DOT_Y( 0.125f ) }, { 15.0f, -DOT_Y( 0.125f ) }, { DOT_Y( 6.5f ), -DOT_Y( 0.125f ) },
		{ DOT_Y( 5.0f ), -DOT_Y( 0.125f ) }, { DOT_Y( 8.0f ), -DOT_Y( 0.125f ) }, { DOT_Y( 6.0f ), -DOT_Y( 0.125f ) },
		{ DOT_Y( 8.0f ), -DOT_Y( 0.125f ) }, { DOT_Y( 10.0f ), -DOT_Y( 0.125f ) },
	} ;

	ew->routine_2 ++ ;
	shl35_02_no_0 = Rnd() & 7 ;
	ew->spd.x = shl35_02_x_tbl[shl35_02_no_0][0] ;
	ew->acc.x = shl35_02_x_tbl[shl35_02_no_0][1] ;
	shl35_02_no_0 = Rnd() & 7 ;
	ew->spd.y = shl35_02_y_tbl[shl35_02_no_0][0] ;
	ew->acc.y = shl35_02_y_tbl[shl35_02_no_0][1] ;
}

/*--------------------------------------------------------------*/
static void shl35_02_bat_mv_rise( PLAYER_WORK *ew )
{
	static Sint16 shl35_02_time_tbl[4] = { 5, 6, 8, 10 } ;

	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->spd.y < 0.0f ){
		ew->routine_2 ++ ;
		ew->timer0 = shl35_02_time_tbl[Rnd() & 3] ;
	}
}

/*--------------------------------------------------------------*/
static void shl35_02_bat_mv_turn( PLAYER_WORK *ew )
{
	static Float shl35_02_x_tbl[8][2] = {
		{ 5.0f, 0.0f }, { DOT_X( 2.5f ), 0.0f }, { DOT_X( 2.0f ), 0.0f }, { 0.0f, 0.0f },
		{ 0.0f, 0.0f }, { -DOT_X( 2.0f ), 0.0f }, { -DOT_X( 2.5f ), 0.0f }, { -5.0f, 0.0f },
	} ;
	static Float shl35_02_y_tbl[8][2] = {
		{ -DOT_Y( 2.0f ), DOT_Y( 0.25f ) }, { -DOT_Y( 2.0f ), DOT_Y( 0.25f ) }, { -DOT_Y( 2.0f ), DOT_Y( 0.5f ) },
		{ -DOT_Y( 2.0f ), DOT_Y( 0.625f ) }, { -DOT_Y( 4.0f ), DOT_Y( 0.75f ) }, { -DOT_Y( 4.0f ), 1.875f },
		{ -DOT_Y( 4.0f ), 2.142857f }, { -DOT_Y( 4.0f ), DOT_Y( 1.125f ) },
	} ;

	ew->routine_2 ++ ;
	shl35_02_no_1 = Rnd() & 7 ;
	ew->spd.x = shl35_02_x_tbl[shl35_02_no_1][0] ;
	ew->acc.x = shl35_02_x_tbl[shl35_02_no_1][1] ;
	shl35_02_no_1 = Rnd() & 7 ;
	ew->spd.y = shl35_02_y_tbl[shl35_02_no_1][0] ;
	ew->acc.y = shl35_02_y_tbl[shl35_02_no_1][1] ;
}

/*--------------------------------------------------------------*/
static void shl35_02_bat_mv_fly( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
}

static void shl35_02_bat_circle_init( PLAYER_WORK *ew );
static void shl35_02_bat_circle_move( PLAYER_WORK *ew );

/*==============================================================*/
static void shl35_02_bat_circle( PLAYER_WORK *ew )
{
	static void ( *shl35_02_bat_circle_jmp[] )( PLAYER_WORK *ew ) = {
		shl35_02_bat_circle_init, shl35_02_bat_circle_move,
	} ;

	shl35_02_bat_circle_jmp[ew->routine_2]( ew ) ;
}

/*==============================================================*/
static void shl35_02_bat_circle_init( PLAYER_WORK *ew )
{
	ew->routine_2 ++ ;
	ew->dir = Rnd() & 31 ;
	shl35_02_bat_circle_move( ew ) ;
}

/*==============================================================*/
static void shl35_02_bat_circle_move( PLAYER_WORK *ew )
{
	dir_spd_move( ew, 400 ) ;
	ew->dir = ++ ew->dir & 31 ;
}

/*--------------------------------------------------------------*/
static void shl35_02_bat_vanish( PLAYER_WORK *ew )
{
	if( -- ew->timer0 ){
		if( Sys->game_timer0 ) ew->disp_flag = 0 ;
		else{
			ew->disp_flag = 1 ;
			pl_char_move( ew ) ;
		}
	}
	else{
		ew->routine_0 ++ ;
		ew->routine_1 = 0 ;
	}
}

/*==============================================================*/
/*			The blast: hits at its place while the dropped		*/
/*			one is still there									*/
/*==============================================================*/
static void shl35_02_blast_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_0 != 1 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
	else{
		if( ew->sub_type ) ew->pos.y = pl_w->pos.y + DOT_Y( 232.0f ) ;
		pl_char_move( ew ) ;
		hit_work_set( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl35_02_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl35_02_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
static void shl35_02_copy( PLAYER_WORK *ew )
{
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->size_top = ew->size_bottom = 32 ;
	ew->size_front = ew->size_back = 36 ;
	ew->prio = 11 ;
}

/*==============================================================*/
static void shl35_02_dummy( PLAYER_WORK *ew )
{
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl35_02.c" End of File	*/

/* Tron Bonne's player effect 00: the marks that ride on her, by their animation (types 0 to 3 and
   13 to 17), and the two trails of works that follow one another along the ground (types 4 to 12). */
#include "player.h"
#include "ef_char.h"
#include "pl_char.h"

/* where each type's mark sits against the player, its draw priority and its animation */
typedef struct PLS10_00_TBL {
	Sint16	x ;
	Sint16	y ;
	Sint8	prio ;
	Uint8	no ;
} PLS10_00_TBL ;

static PLS10_00_TBL pls10_00_tbl[18] = {
	{ 0, 0, 13, 14 }, { 0, 0, 11, 15 }, { -80, 88, 8, 16 }, { -59, 119, 8, 17 },
	{ -88, 88, 11, 18 }, { 0, 0, 11, 19 }, { 0, 0, 11, 20 }, { -67, 119, 11, 21 },
	{ 0, 0, 11, 22 }, { 0, 0, 11, 23 }, { -88, 88, 11, 18 }, { 0, 0, 11, 19 },
	{ 0, 0, 11, 20 }, { -50, 64, 8, 34 }, { 32, 37, 11, 36 }, { 32, 49, 11, 36 },
	{ -50, 64, 8, 34 }, { 0, 0, 11, 15 },
} ;

/* the mark's place: the player's, moved by its table row (the table is in 3/5 and 7/15 pixels) */
#define PLS10_00_POS( ew, pl_w, tbl )	\
	if( !( ew )->disp_dir ) ( ew )->pos.x = ( pl_w )->pos.x + ( tbl )->x * 1.6666666f ;	\
	else ( ew )->pos.x = ( pl_w )->pos.x - ( tbl )->x * 1.6666666f ;	\
	( ew )->pos.y = ( pl_w )->pos.y + ( tbl )->y * 2.142857f

static void pls10_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls10_00_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls10_00_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
	}
	return ew ;
}

/****************************************************************/
PLAYER_WORK *pls10_00_lunch_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS_ST, LIST_TOP ) ) ){
		ew->work_func = pls10_00_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
	}
	return ew ;
}

static void pls10_00_init( PLAYER_WORK *ew );
static void pls10_00_move( PLAYER_WORK *ew );
static void pls10_00_end( PLAYER_WORK *ew );
static void pls10_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls10_00_main( PLAYER_WORK *ew )
{
	static void ( *pls10_00_jmp[] )( PLAYER_WORK *ew ) = {
		pls10_00_init, pls10_00_move, pls10_00_end, pls10_00_free,
	} ;

	pls10_00_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls10_00_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	PLS10_00_TBL *tbl ;

	pl_w = ew->master_w ;
	ew->routine_0 ++ ;
	ew->ef_no = 0x1000 ;
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
	tbl = &pls10_00_tbl[ew->type] ;
	ew->prio = tbl->prio ;
	PLS10_00_POS( ew, pl_w, tbl ) ;
	ef_char_set( ew, 27, tbl->no ) ;
	pls10_00_move( ew ) ;
}

static void pls10_00_scene( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls10_00_crouch_punch( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls10_00_upper( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls10_00_follow( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls10_00_beacon_head( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls10_00_beacon_part( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls10_00_beacon_last( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls10_00_beacon_up_head( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls10_00_beacon_up_part( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls10_00_beacon_up_last( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls10_00_lunch_head( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls10_00_lunch_part( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls10_00_lunch_last( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls10_00_stand_kick( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls10_00_strike( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls10_00_kick( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void pls10_00_move( PLAYER_WORK *ew )
{
	static void ( *pls10_00_jmp_type[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls10_00_scene, pls10_00_crouch_punch, pls10_00_follow, pls10_00_follow,
		pls10_00_beacon_head, pls10_00_beacon_part, pls10_00_beacon_last, pls10_00_beacon_up_head,
		pls10_00_beacon_up_part, pls10_00_beacon_up_last, pls10_00_lunch_head, pls10_00_lunch_part,
		pls10_00_lunch_last, pls10_00_stand_kick, pls10_00_strike, pls10_00_strike,
		pls10_00_kick, pls10_00_upper,
	} ;

	pls10_00_jmp_type[ew->type]( ew, ew->master_w ) ;
}

static void pls10_00_in( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls10_00_slide( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls10_00_anim( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls10_00_scene( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls10_00_scene_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls10_00_in, pls10_00_slide, pls10_00_anim,
	} ;

	pls10_00_scene_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
/*			Comes in from beyond the edge of the screen,		*/
/*			slowing as it crosses								*/
/*==============================================================*/
static void pls10_00_in( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_1 ++ ;
	if( !ew->disp_dir ) ew->pos.x = scr_work.scr_right + DOT_X( 112.0f ) ;
	else ew->pos.x = scr_work.scr_left + -DOT_X( 112.0f ) ;
	ew->pos.y = pl_w->ground_y ;
	if( !ew->disp_dir ){
		ew->spd.x = -DOT_X( 8.0f ) ;
		ew->acc.x = DOT_X( 0.25f ) ;
	}
	else {
		ew->spd.x = DOT_X( 8.0f ) ;
		ew->acc.x = -DOT_X( 0.25f ) ;
	}
	ew->spd.y = 0 ;
	ew->acc.y = 0 ;
	pls10_00_slide( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls10_00_slide( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ef_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	if( ew->spd.x * ew->acc.x > 0 ) ew->routine_1 ++ ;
}

/*--------------------------------------------------------------*/
static void pls10_00_anim( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ef_char_move( ew ) ;
}

/*==============================================================*/
/*			Shows the animation while the player does his		*/
/*			heavy normal										*/
/*==============================================================*/
static void pls10_00_crouch_punch( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS10_00_TBL *tbl ;

	if( !pl_w->cg_type || pl_w->act_st != N_NML_ATCK || pl_w->nml_no != 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	tbl = &pls10_00_tbl[ew->type] ;
	ef_char_set2( ew, 27, tbl->no, pl_w->cg_prm ) ;
}

/*==============================================================*/
static void pls10_00_upper( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS10_00_TBL *tbl ;

	if( !pl_w->cg_type || pl_w->act_st != N_CMD_ATCK ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	tbl = &pls10_00_tbl[ew->type] ;
	ef_char_set2( ew, 27, tbl->no, pl_w->cg_prm ) ;
}

/*==============================================================*/
static void pls10_00_follow( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS10_00_TBL *tbl ;

	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
	else {
		tbl = &pls10_00_tbl[ew->type] ;
		PLS10_00_POS( ew, pl_w, tbl ) ;
	}
}

/* the trails: each piece is made by the one before it and keeps its place behind it */
static Float pls10_00_edge_dist( PLAYER_WORK *ew );
static void pls10_00_beacon_head_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls10_00_beacon_head_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls10_00_beacon_head( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls10_00_beacon_head_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls10_00_beacon_head_init, pls10_00_beacon_head_move,
	} ;

	pls10_00_beacon_head_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls10_00_beacon_head_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *nw ;

	ew->routine_1 ++ ;
	ew->timer1 = 10 ;
	if( ( nw = work_get( ew, W_PLS, LIST_BEHIND ) ) ){
		nw->work_func = pls10_00_main ;
		nw->master_w = pl_w ;
		nw->timer1 = ew->timer1 - 1 ;
		if( pls10_00_edge_dist( ew ) >= 80.0f ) nw->type = 5 ;
		else nw->type = 6 ;
	}
	pls10_00_beacon_head_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls10_00_beacon_head_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->cg_prm && pl_w->act_st == N_SPECIAL && pl_w->pl_sp_type == 1 ) ef_char_move( ew ) ;
	else {
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

static void pls10_00_beacon_part_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls10_00_beacon_part_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls10_00_beacon_part( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls10_00_beacon_part_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls10_00_beacon_part_init, pls10_00_beacon_part_move,
	} ;

	pls10_00_beacon_part_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls10_00_beacon_part_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *bw = ew->before_w ;
	PLAYER_WORK *nw ;
	Float ofs ;

	ew->routine_1 ++ ;
	if( bw->type == 4 ) ofs = -DOT_X( 32.0f ) ;
	else ofs = -80.0f ;
	if( ew->disp_dir ) ofs = -ofs ;
	ew->pos.x = bw->pos.x + ofs ;
	ew->pos.y = bw->pos.y ;
	if( ( nw = work_get( ew, W_PLS, LIST_BEHIND ) ) ){
		nw->work_func = pls10_00_main ;
		nw->master_w = pl_w ;
		nw->timer1 = ew->timer1 - 1 ;
		if( ew->timer1 && pls10_00_edge_dist( ew ) >= DOT_X( 32.0f ) ) nw->type = 5 ;
		else nw->type = 6 ;
	}
	pls10_00_beacon_part_move( ew, pl_w ) ;
}

/*==============================================================*/
/*			Ends when the piece before it has ended				*/
/*==============================================================*/
static void pls10_00_beacon_part_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *bw = ew->before_w ;

	if( bw->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ef_char_move( ew ) ;
}

static void pls10_00_beacon_last_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls10_00_beacon_last( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls10_00_beacon_last_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls10_00_beacon_last_init, pls10_00_beacon_part_move,
	} ;

	pls10_00_beacon_last_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls10_00_beacon_last_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *bw = ew->before_w ;
	Float ofs ;

	ew->routine_1 ++ ;
	if( bw->type == 4 ) ofs = -DOT_X( 16.0f ) ;
	else ofs = -DOT_X( 32.0f ) ;
	if( ew->disp_dir ) ofs = -ofs ;
	ew->pos.x = bw->pos.x + ofs ;
	ew->pos.y = bw->pos.y ;
	pls10_00_beacon_part_move( ew, pl_w ) ;
}

static void pls10_00_beacon_up_head_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls10_00_beacon_up_head( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls10_00_beacon_up_head_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls10_00_beacon_up_head_init, pls10_00_beacon_head_move,
	} ;

	pls10_00_beacon_up_head_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls10_00_beacon_up_head_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *nw ;

	ew->routine_1 ++ ;
	ew->timer1 = 10 ;
	if( ( nw = work_get( ew, W_PLS, LIST_BEHIND ) ) ){
		nw->work_func = pls10_00_main ;
		nw->master_w = pl_w ;
		nw->timer1 = ew->timer1 - 1 ;
		if( pls10_00_edge_dist( ew ) >= DOT_X( 40.0f ) ) nw->type = 8 ;
		else nw->type = 9 ;
	}
	pls10_00_beacon_head_move( ew, pl_w ) ;
}

static void pls10_00_beacon_up_part_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls10_00_beacon_up_part( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls10_00_beacon_up_part_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls10_00_beacon_up_part_init, pls10_00_beacon_part_move,
	} ;

	pls10_00_beacon_up_part_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls10_00_beacon_up_part_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *bw = ew->before_w ;
	PLAYER_WORK *nw ;
	Float dx, dy ;

	ew->routine_1 ++ ;
	if( bw->type == 7 ){
		dx = -40.0f ;
		dy = DOT_Y( 32.0f ) ;
	}
	else {
		dx = -80.0f ;
		dy = DOT_Y( 48.0f ) ;
	}
	if( ew->disp_dir ) dx = -dx ;
	ew->pos.x = bw->pos.x + dx ;
	ew->pos.y = bw->pos.y + dy ;
	if( ( nw = work_get( ew, W_PLS, LIST_BEHIND ) ) ){
		nw->work_func = pls10_00_main ;
		nw->master_w = pl_w ;
		nw->timer1 = ew->timer1 - 1 ;
		if( ew->timer1 && pls10_00_edge_dist( ew ) >= DOT_X( 32.0f ) ) nw->type = 8 ;
		else nw->type = 9 ;
	}
	pls10_00_beacon_part_move( ew, pl_w ) ;
}

static void pls10_00_beacon_up_last_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls10_00_beacon_up_last( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls10_00_beacon_up_last_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls10_00_beacon_up_last_init, pls10_00_beacon_part_move,
	} ;

	pls10_00_beacon_up_last_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls10_00_beacon_up_last_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *bw = ew->before_w ;
	Float dx, dy ;

	ew->routine_1 ++ ;
	if( bw->type == 7 ){
		dx = -DOT_X( 2.0f ) ;
		dy = DOT_Y( 16.0f ) ;
	}
	else {
		dx = -40.0f ;
		dy = DOT_Y( 48.0f ) ;
	}
	if( ew->disp_dir ) dx = -dx ;
	ew->pos.x = bw->pos.x + dx ;
	ew->pos.y = bw->pos.y + dy ;
	pls10_00_beacon_part_move( ew, pl_w ) ;
}

/* the second trail runs while the game is not stopped */
static void pls10_00_lunch_head_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls10_00_lunch_head_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls10_00_lunch_head( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls10_00_lunch_head_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls10_00_lunch_head_init, pls10_00_lunch_head_move,
	} ;

	if( vs_sys_work.game_stop & 1 << ( pl_w->pl_no ^ 1 ) ) return ;
	pls10_00_lunch_head_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls10_00_lunch_head_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *nw ;

	ew->routine_1 ++ ;
	ew->timer1 = 10 ;
	if( ( nw = work_get( ew, W_PLS_ST, LIST_BEHIND ) ) ){
		nw->work_func = pls10_00_main ;
		nw->master_w = pl_w ;
		nw->timer1 = ew->timer1 - 1 ;
		if( pls10_00_edge_dist( ew ) >= 80.0f ) nw->type = 11 ;
		else nw->type = 12 ;
	}
	pls10_00_lunch_head_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls10_00_lunch_head_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->cg_prm && pl_w->act_st == N_SUPER && pl_w->pl_sp_type == 5 ) ef_char_move( ew ) ;
	else {
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

static void pls10_00_lunch_part_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls10_00_lunch_part_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls10_00_lunch_part( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls10_00_lunch_part_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls10_00_lunch_part_init, pls10_00_lunch_part_move,
	} ;

	if( vs_sys_work.game_stop & 1 << ( pl_w->pl_no ^ 1 ) ) return ;
	pls10_00_lunch_part_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls10_00_lunch_part_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *bw = ew->before_w ;
	PLAYER_WORK *nw ;
	Float ofs ;

	ew->routine_1 ++ ;
	if( bw->type == 10 ) ofs = -DOT_X( 32.0f ) ;
	else ofs = -80.0f ;
	if( ew->disp_dir ) ofs = -ofs ;
	ew->pos.x = bw->pos.x + ofs ;
	ew->pos.y = bw->pos.y ;
	if( ( nw = work_get( ew, W_PLS_ST, LIST_BEHIND ) ) ){
		nw->work_func = pls10_00_main ;
		nw->master_w = pl_w ;
		nw->timer1 = ew->timer1 - 1 ;
		if( ew->timer1 && pls10_00_edge_dist( ew ) >= DOT_X( 32.0f ) ) nw->type = 11 ;
		else nw->type = 12 ;
	}
	pls10_00_lunch_part_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls10_00_lunch_part_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *bw = ew->before_w ;

	if( bw->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ef_char_move( ew ) ;
}

static void pls10_00_lunch_last_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls10_00_lunch_last( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls10_00_lunch_last_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls10_00_lunch_last_init, pls10_00_lunch_part_move,
	} ;

	if( vs_sys_work.game_stop & 1 << ( pl_w->pl_no ^ 1 ) ) return ;
	pls10_00_lunch_last_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls10_00_lunch_last_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *bw = ew->before_w ;
	Float ofs ;

	ew->routine_1 ++ ;
	if( bw->type == 10 ) ofs = -DOT_X( 16.0f ) ;
	else ofs = -DOT_X( 32.0f ) ;
	if( ew->disp_dir ) ofs = -ofs ;
	ew->pos.x = bw->pos.x + ofs ;
	ew->pos.y = bw->pos.y ;
	pls10_00_lunch_part_move( ew, pl_w ) ;
}

static void pls10_00_kick_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls10_00_kick_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls10_00_kick_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls10_00_stand_kick( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls10_00_stand_kick_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls10_00_kick_init, pls10_00_kick_move, pls10_00_kick_end,
	} ;

	if( pl_w->act_st != N_NML_ATCK || pl_w->nml_no != 1 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	pls10_00_stand_kick_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls10_00_kick( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls10_00_kick_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls10_00_kick_init, pls10_00_kick_move, pls10_00_kick_end,
	} ;

	if( pl_w->act_st != N_SPECIAL || pl_w->pl_sp_type != 8 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	pls10_00_kick_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls10_00_kick_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_1 ++ ;
	ew->rot[2] = 0xf000 ;
	pls10_00_kick_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls10_00_kick_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS10_00_TBL *tbl ;

	ef_char_move( ew ) ;
	if( !pl_w->cg_type ){
		ew->routine_1 ++ ;
		ef_char_set( ew, 27, 35 ) ;
	}
	tbl = &pls10_00_tbl[ew->type] ;
	PLS10_00_POS( ew, pl_w, tbl ) ;
}

/*--------------------------------------------------------------*/
static void pls10_00_kick_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void pls10_00_strike( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS10_00_TBL *tbl ;

	tbl = &pls10_00_tbl[ew->type] ;
	PLS10_00_POS( ew, pl_w, tbl ) ;
	if( pl_w->cg_prm && pl_w->act_st == N_SPECIAL && pl_w->pl_sp_type == 0 ) ef_char_move( ew ) ;
	else {
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void pls10_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls10_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
static Float pls10_00_edge_dist( PLAYER_WORK *ew )
{
	if( !ew->disp_dir ) return ew->pos.x - scr_work.scr_left ;
	else return scr_work.scr_right - ew->pos.x ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls10_00.c" End of File	*/

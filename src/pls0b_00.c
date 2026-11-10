/* Captain America's effect 00 (his pls0b): the pieces that go with his moves, by type: the ones of
   his win poses, the pictures on his shield while it lies on the ground, the pieces of Charging
   Star, Hyper Stars & Stripes and Hyper Charging Star, and the flash where his thrown shield hits.
   A piece follows its master (or the work it rides on, ef_w). */
#include "player.h"
#include "ef_char.h"
#include "pl_char.h"
#include "pl0b_def.h"

/* the effect's own work, from obj_mtx */
typedef struct PLS0B_00_LOCAL_W {
	Uint16	char_no;	/* 000: the master's animation when this piece was put out */
} PLS0B_00_LOCAL_W;
#define PLS0B_00_LW( w )	( ( PLS0B_00_LOCAL_W * )&( w )->obj_mtx )

static void pls0b_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls0b_00_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b )
{
	PLAYER_WORK *ew ;
	PLS0B_00_LOCAL_W *lw ;

	if( ( ew = work_get( 0, W_PLS, LIST_END ) ) ){
		ew->work_func = pls0b_00_main ;
		ew->master_w = pl_w ;
		ew->type = a ;
		ew->sub_type = b ;
		ew->ef_no = 0xb00 ;
		ew->disp_flag = 0 ;
		lw = PLS0B_00_LW( ew ) ;
		lw->char_no = pl_w->char_no.w ;
	}
	return ew ;
}

/****************************************************************/
PLAYER_WORK *pls0b_00_set3( PLAYER_WORK *pl_w, Uint8 a, Uint8 b )
{
	PLAYER_WORK *ew ;
	PLS0B_00_LOCAL_W *lw ;

	if( ( ew = work_get( 0, W_PLS_ST, LIST_END ) ) ){
		ew->work_func = pls0b_00_main ;
		ew->master_w = pl_w ;
		ew->type = a ;
		ew->sub_type = b ;
		ew->ef_no = 0xb00 ;
		ew->disp_flag = 0 ;
		lw = PLS0B_00_LW( ew ) ;
		lw->char_no = pl_w->char_no.w ;
	}
	return ew ;
}

/****************************************************************/
/*			A piece that rides on the work pl_w, which is		*/
/*			his shield											*/
/****************************************************************/
PLAYER_WORK *pls0b_00_set2( PLAYER_WORK *pl_w, Uint8 a, Uint8 b )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_END ) ) ){
		ew->work_func = pls0b_00_main ;
		ew->master_w = pl_w->master_w ;
		ew->ef_w = pl_w ;
		ew->type = a ;
		ew->sub_type = b ;
		ew->ef_no = 0xb00 ;
		ew->disp_flag = 0 ;
	}
	return ew ;
}

static void pls0b_00_init( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw ) ;
static void pls0b_00_move( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw ) ;
static void pls0b_00_end( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw ) ;
static void pls0b_00_free( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw ) ;

/*==============================================================*/
static void pls0b_00_main( PLAYER_WORK *ew )
{
	static void ( *pls0b_00_jmp[] )( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw ) = {
		pls0b_00_init, pls0b_00_move, pls0b_00_end, pls0b_00_free,
	} ;

	PLAYER_WORK *pl_w = ew->master_w ;

	ew->prio = pl_w->prio ;
	pls0b_00_jmp[ew->routine_0]( ew, PLS0B_00_LW( ew ) ) ;
}

/*==============================================================*/
static void pls0b_00_win_join_init( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Float x ;

	ew->sub_prio = -2 ;
	ew->timer0 = 40 ;
	x = 800.0f ;
	ew->spd.x = -DOT_X( 2.0f ) ;
	ew->acc.x = -DOT_X( 0.125f ) ;
	ew->spd.y = -DOT_Y( 2.0f ) ;
	ew->acc.y = 0.13392857f ;
	if( !pl_w->disp_dir ){
		ew->spd.x = -ew->spd.x ;
		ew->acc.x = -ew->acc.x ;
		x = -800.0f ;
	}
	ew->pos.x = pl_w->pos.x + x ;
	ew->pos.y = pl_w->pos.y + 480.0f ;
	ew->disp_flag = 0 ;
	ew->routine_1 = 0 ;
	pl_char_set( ew, 19, 8 ) ;
}

/*==============================================================*/
static void pls0b_00_ground_init( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *ef_w = ew->ef_w ;

	ew->disp_flag = 0 ;
	ew->routine_1 = 0 ;
	ew->timer0 = 0 ;
	ew->disp_dir = ef_w->disp_dir ;
	ew->sub_prio = -2 ;
	ew->pos.x = ef_w->pos.x ;
	ew->pos.y = ef_w->pos.y ;
	pl_char_set( ew, Char_Effect, 27 ) ;
}

/* how fast the pieces that cross the screen in a win pose (type 2) go: x speed, its change,
   y speed, its change */
static Sint16 pls0b_00_win_cross_tbl[16][4] = {
	{ 896, 48, 768, 32 }, { 768, 64, 896, 32 }, { 768, 80, 512, 48 }, { 768, 96, 512, 48 },
	{ 1152, 48, 256, 64 }, { 1024, 64, 256, 64 }, { 1024, 80, 384, 96 }, { 1024, 96, 256, 96 },
	{ 384, 48, 1024, 32 }, { 384, 64, 640, 32 }, { 384, 80, 512, 48 }, { 512, 96, 512, 48 },
	{ 640, 48, 256, 64 }, { 512, 64, 256, 64 }, { 512, 80, 384, 96 }, { 512, 96, 256, 96 },
} ;

/*==============================================================*/
static void pls0b_00_win_cross_init( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Sint16 n ;

	ew->sub_prio = -2 ;
	n = nlRand() & 15 ;
	ew->spd.x = pls0b_00_win_cross_tbl[n][0] * 1.6666666f / 256.0f ;
	ew->acc.x = pls0b_00_win_cross_tbl[n][1] * 1.6666666f / 256.0f ;
	ew->spd.y = pls0b_00_win_cross_tbl[n][2] * 2.142857f / 256.0f ;
	ew->acc.y = pls0b_00_win_cross_tbl[n][3] * 2.142857f / 256.0f ;
	if( ew->disp_dir ){
		ew->pos.x = scr_work.scr_left + -DOT_X( 16.0f ) ;
	} else {
		ew->pos.x = scr_work.scr_right + DOT_X( 16.0f ) ;
		ew->spd.x = -ew->spd.x ;
		ew->acc.x = -ew->acc.x ;
	}
	ew->pos.y = pl_w->ground_y - ( ( nlRand() & 127 ) << 8 ) * 2.142857f / 256.0f + DOT_Y( 80.0f ) ;
	pl_char_set( ew, Char_Effect, 29 ) ;
}

/*==============================================================*/
static void pls0b_00_win_shield_init( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw )
{
	ew->disp_flag = 0 ;
	ew->routine_1 = 0 ;
	pl_char_set( ew, Char_Effect, 30 ) ;
}

typedef struct PLS0B_00_CHARGE_DASH {
	Sint16	x, y;
	Sint16	no;
} PLS0B_00_CHARGE_DASH;

static PLS0B_00_CHARGE_DASH pls0b_00_charge_dash_tbl[] = {
	{ 8192, 16384, 32 }, { 11008, 21760, 33 }, { -2048, 6656, 34 },
} ;

/*==============================================================*/
static void pls0b_00_charge_dash_init( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->disp_flag = 0 ;
	ew->routine_1 = 0 ;
	ew->spd.x = pls0b_00_charge_dash_tbl[ew->sub_type].x * 1.6666666f / 256.0f ;
	ew->spd.y = pls0b_00_charge_dash_tbl[ew->sub_type].y * 2.142857f / 256.0f ;
	ew->char_no.b.no = pls0b_00_charge_dash_tbl[ew->sub_type].no ;
	if( !pl_w->disp_dir ) ew->spd.x = -ew->spd.x ;
	ew->pos.x = pl_w->pos.x + ew->spd.x ;
	ew->pos.y = pl_w->pos.y + ew->spd.y ;
	pl_char_set( ew, Char_Effect, ew->char_no.b.no ) ;
}

/*==============================================================*/
static void pls0b_00_mark_init( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *nw ;

	ew->routine_1 = 0 ;
	ew->disp_flag = 0 ;
	if( !ew->sub_type ){
		ew->disp_dir = 0 ;
		if( !pl_w->pl_no ) ew->char_no.b.no = 35 ;
		else ew->char_no.b.no = 36 ;
		pl_char_set( ew, Char_Effect, ew->char_no.b.no ) ;
		if( ( nw = work_get( ew, W_PLS, LIST_BEHIND ) ) ){
			nw->work_func = pls0b_00_main ;
			nw->master_w = ew->master_w ;
			nw->ef_w = ew ;
			nw->type = 5 ;
			nw->sub_type = 1 ;
			nw->ef_no = 0xb00 ;
			nw->disp_flag = 0 ;
		}
	} else {
		if( !pl_w->pl_no ) ew->char_no.b.no = 37 ;
		else ew->char_no.b.no = 38 ;
		pl_char_set( ew, Char_Effect, ew->char_no.b.no ) ;
	}
}

/*==============================================================*/
static void pls0b_00_hstars_init( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw )
{
	ew->routine_1 = 0 ;
	ew->spd.x = DOT_X( 35.0f ) ;
	ew->spd.y = 285.0f ;
	pl_char_set( ew, Char_Effect, 40 ) ;
}

/*==============================================================*/
static void pls0b_00_t7_init( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw )
{
}

/*==============================================================*/
static void pls0b_00_charge_init( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->routine_1 = 0 ;
	ew->disp_flag = 0 ;
	ew->spd.x = DOT_X( 83.0f ) ;
	ew->spd.y = 150.0f ;
	if( !pl_w->disp_dir ) ew->spd.x = -ew->spd.x ;
	ew->pos.x = pl_w->pos.x + ew->spd.x ;
	ew->pos.y = pl_w->pos.y + ew->spd.y ;
	pl_char_set( ew, Char_Effect, 39 ) ;
}

/*==============================================================*/
static void pls0b_00_hcharge_init( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->routine_1 = 0 ;
	ew->disp_flag = 0 ;
	if( !ew->sub_type ) ew->char_no.b.no = 41 ;
	else ew->char_no.b.no = 42 ;
	pl_char_set( ew, Char_Effect, ew->char_no.b.no ) ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
}

static void pls0b_00_move( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw ) ;

/*==============================================================*/
static void pls0b_00_hit_init( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *ef_w = ew->ef_w ;

	ew->pos.x = ef_w->pos.x ;
	ew->pos.y = ef_w->pos.y ;
	if( !ew->sub_type || ew->sub_type >= 7 ){
		ew->disp_flag = 0 ;
		ew->routine_0 = 2 ;
		return ;
	}
	ef_char_set( ew, 27, ew->sub_type ) ;
	pls0b_00_move( ew, lw ) ;
}

/*==============================================================*/
static void pls0b_00_init( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw )
{
	static void ( *pls0b_00_init_jmp[] )( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw ) = {
		pls0b_00_win_join_init, pls0b_00_ground_init, pls0b_00_win_cross_init, pls0b_00_win_shield_init,
		pls0b_00_charge_dash_init, pls0b_00_mark_init, pls0b_00_hstars_init, pls0b_00_t7_init,
		pls0b_00_charge_init, pls0b_00_hcharge_init, pls0b_00_hit_init,
	} ;

	ew->routine_0 ++ ;
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
	pls0b_00_init_jmp[ew->type]( ew, lw ) ;
}

/*==============================================================*/
static void pls0b_00_win_join_wait( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw )
{
	if( -- ew->timer0 == 0 ){
		ew->routine_1 ++ ;
		ew->disp_flag = 1 ;
	}
}

/*--------------------------------------------------------------*/
static void pls0b_00_win_join_fly( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw )
{
	if( pl_char_move( ew ) < 0 ){
		ew->acc.y = 0.0f ;
		ew->routine_1 ++ ;
		pl_char_set( ew, 19, 9 ) ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
}

/*--------------------------------------------------------------*/
static void pls0b_00_win_join_glide( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		ew->acc.x = 0.26041666f ;
		if( !ew->disp_dir ) ew->acc.x = -ew->acc.x ;
		ew->timer0 = 48 ;
		pl_char_set( ew, 19, 10 ) ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
}

/*--------------------------------------------------------------*/
static void pls0b_00_win_join_brake( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( -- ew->timer0 == 0 ){
		ew->routine_1 ++ ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		ew->timer0 = 768 ;
		pl_char_set( ew, 19, 11 ) ;
	}
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
}

/*--------------------------------------------------------------*/
static void pls0b_00_win_join_keep( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw )
{
	if( -- ew->timer0 == 0 ) ew->routine_0 ++ ;
	pl_char_move( ew ) ;
}

/*==============================================================*/
static void pls0b_00_win_join_move( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw )
{
	static void ( *pls0b_00_win_join_jmp[] )( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw ) = {
		pls0b_00_win_join_wait, pls0b_00_win_join_fly, pls0b_00_win_join_glide, pls0b_00_win_join_brake,
		pls0b_00_win_join_keep,
	} ;

	pls0b_00_win_join_jmp[ew->routine_1]( ew, lw ) ;
}

/*==============================================================*/
/*			A piece on the shield takes the shield's place		*/
/*			and the way it faces								*/
/*==============================================================*/
static void pls0b_00_pos_set( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *ef_w = ew->ef_w ;

	ew->disp_dir = ef_w->disp_dir ;
	ew->sub_prio = -2 ;
	ew->pos.x = ef_w->pos.x ;
	ew->pos.y = ef_w->pos.y ;
}

static Uint8 pls0b_00_disp_ck( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw ) ;

/*==============================================================*/
static void pls0b_00_ground_wait( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw )
{
	ew->disp_flag = 0 ;
	if( ew->timer0 -- == 0 ){
		ew->routine_1 ++ ;
		ew->timer0 = 180 ;
		pls0b_00_pos_set( ew, lw ) ;
		pl_char_set( ew, Char_Effect, 27 ) ;
		pls0b_00_disp_ck( ew, lw ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls0b_00_ground_show( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw )
{
	pls0b_00_pos_set( ew, lw ) ;
	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 = 0 ;
		ew->disp_flag = 0 ;
		return ;
	}
	pls0b_00_disp_ck( ew, lw ) ;
}

/*==============================================================*/
static void pls0b_00_ground_move( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw )
{
	static void ( *pls0b_00_ground_jmp[] )( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw ) = {
		pls0b_00_ground_wait, pls0b_00_ground_show,
	} ;
	PLAYER_WORK *ef_w = ew->ef_w ;

	if( ef_w->routine_0 != 1 || ef_w->type != 1 || ef_w->sub_type ){
		ew->routine_0 ++ ;
		return ;
	}
	pls0b_00_ground_jmp[ew->routine_1]( ew, lw ) ;
}

/*==============================================================*/
static void pls0b_00_win_cross_move( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PL0B_LOCAL_W *pl_lw = PL0B_LW( pl_w ) ;
	Float x ;

	if( ew->pos.y > scr_work.scr_top + DOT_Y( 48.0f ) ){
		ew->routine_0 ++ ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	x = ew->pos.x - pl_w->pos.x ;
	if( x < 0.0f ) x = -x ;
	if( x >= 80.0f ) pl_lw->away_flag = 1 ;
	pl_char_move( ew ) ;
	pls0b_00_disp_ck( ew, lw ) ;
}

/*==============================================================*/
static void pls0b_00_win_shield_move( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *ef_w = ew->ef_w ;
	PLAYER_WORK *pl_w = ew->master_w ;

	switch( ef_w->routine_0 ){
	case 1:
		if( ew->sub_type && !ef_w->type ){
			switch( ef_w->sub_type ){
			case 2:
				if( !pl_w->cg_type ) break ;
				ew->prio = ef_w->prio ;
				ew->sub_prio = ef_w->sub_prio - 2 ;
				ew->pos.x = ef_w->pos.x ;
				ew->pos.y = ef_w->pos.y ;
				ew->disp_flag = 1 ;
				if( pl_char_move( ew ) >= 0 ) return ;
				break ;
			}
		}
	}
	ew->disp_flag = 0 ;
	ew->routine_0 ++ ;
}

/*==============================================================*/
static void pls0b_00_charge_dash_move( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->disp_flag = 0 ;
	if( pl_w->char_no.w != lw->char_no ){
		ew->routine_0 ++ ;
		return ;
	}
	if( pl_w->cg_type ){
		ew->disp_flag = 1 ;
		ew->disp_dir = pl_w->disp_dir ;
		ew->sub_prio = -1 ;
		ew->pos.x = pl_w->pos.x + ew->spd.x ;
		ew->pos.y = pl_w->pos.y + ew->spd.y ;
		pl_char_move( ew ) ;
	}
}

/*==============================================================*/
/*			The mark (type 5) puts itself 24 dots over the		*/
/*			shield it rides on									*/
/*==============================================================*/
static void pls0b_00_mark_pos_set( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *ef_w = ew->ef_w ;

	ew->sub_prio = -2 ;
	ew->pos.x = ef_w->pos.x ;
	ew->pos.y = ef_w->pos.y + DOT_Y( 24.0f ) ;
}

/*==============================================================*/
static void pls0b_00_mark_move( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *ef_w = ew->ef_w ;

	ew->disp_flag = 0 ;
	if( ef_w->routine_0 != 1 ){
		ew->routine_0 ++ ;
		return ;
	}
	if( !ew->sub_type ){
		if( ef_w->type != 1 || ef_w->sub_type != 0 ){
			ew->routine_0 ++ ;
			return ;
		}
		if( ef_w->char_no.w != 0x1718 ){
			ew->routine_0 ++ ;
			return ;
		}
		pls0b_00_mark_pos_set( ew, lw ) ;
	} else {
		ew->sub_prio = -1 ;
		ew->pos.x = ef_w->pos.x ;
		ew->pos.y = ef_w->pos.y + DOT_Y( 10.0f ) ;
	}
	pl_char_move( ew ) ;
	pls0b_00_disp_ck( ew, lw ) ;
}

/* where the piece of Hyper Stars & Stripes (type 6) sits from him, by his frame's code: x and y */
static Sint32 pls0b_00_hstars_tbl[][2] = {
	{ 0x400000, 0x500000 }, { 0x400000, 0x750000 }, { 0x230000, 0x850000 },
} ;

/*==============================================================*/
static void pls0b_00_hstars_move( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Sint8 n ;
	Sint8 k ;

	ew->disp_flag = 0 ;
	if( pl_w->char_no.w != lw->char_no ){
		ew->routine_0 ++ ;
		return ;
	}
	n = pl_w->cg_type & 0xf0 ;
	if( n ){
		ew->disp_flag = 1 ;
		k = n / 16 - 1 ;
		ew->spd.x = pls0b_00_hstars_tbl[k][0] * 1.6666666f / 65536.0f ;
		ew->spd.y = pls0b_00_hstars_tbl[k][1] * 2.142857f / 65536.0f ;
		if( !pl_w->disp_dir ) ew->spd.x = -ew->spd.x ;
		ew->pos.x = pl_w->pos.x + ew->spd.x ;
		ew->pos.y = pl_w->pos.y + ew->spd.y ;
		ew->disp_dir = pl_w->disp_dir ;
		ew->sub_prio = -2 ;
		pl_char_move( ew ) ;
	}
}

/*==============================================================*/
static void pls0b_00_t7_move( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw )
{
}

/*==============================================================*/
static void pls0b_00_charge_move( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->disp_flag = 0 ;
	if( pl_w->char_no.w != lw->char_no ){
		ew->routine_0 ++ ;
		return ;
	}
	if( pl_w->cg_type ){
		ew->disp_dir = pl_w->disp_dir ;
		ew->sub_prio = -2 ;
		ew->pos.x = pl_w->pos.x + ew->spd.x ;
		ew->pos.y = pl_w->pos.y + ew->spd.y ;
		pl_char_move( ew ) ;
		if( !ew->cg_prm ) ew->disp_flag = 1 ;
	}
}

/*==============================================================*/
static void pls0b_00_hcharge_follow( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PL0B_LOCAL_W *pl_lw = PL0B_LW( pl_w ) ;

	ew->disp_flag = 0 ;
	if( pl_w->char_no.w != lw->char_no ){
		if( ew->sub_type ){
			ew->routine_0 ++ ;
			return ;
		}
		ew->routine_1 ++ ;
		ew->disp_flag = 1 ;
		pl_char_set( ew, Char_Effect, 43 ) ;
		return ;
	}
	if( pl_w->cg_type ){
		ew->disp_dir = pl_w->disp_dir ;
		ew->sub_prio = -2 ;
		if( ew->sub_type ) ew->sub_prio -= 3 ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		if( !vs_sys_work.game_stop ) pl_char_move( ew ) ;
		if( !ew->cg_prm && ( !ew->sub_type || !pl_lw->shield_st ) ) ew->disp_flag = 1 ;
	}
}

/*--------------------------------------------------------------*/
static void pls0b_00_hcharge_end( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_char_move( ew ) < 0 ){
		ew->disp_flag = 0 ;
		ew->routine_0 ++ ;
		return ;
	}
	ew->disp_dir = pl_w->disp_dir ;
	ew->sub_prio = -2 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
}

/*==============================================================*/
static void pls0b_00_hcharge_move( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw )
{
	static void ( *pls0b_00_hcharge_jmp[] )( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw ) = {
		pls0b_00_hcharge_follow, pls0b_00_hcharge_end,
	} ;

	pls0b_00_hcharge_jmp[ew->routine_1]( ew, lw ) ;
}

/*==============================================================*/
static void pls0b_00_hit_move( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw )
{
	PLAYER_WORK *ef_w = ew->ef_w ;

	ew->prio = ef_w->prio ;
	ew->sub_prio = ef_w->sub_prio - 4 ;
	ew->disp_flag = 1 ;
	if( ef_char_move( ew ) < 0 ){
		ew->disp_flag = 0 ;
		ew->routine_0 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void pls0b_00_move( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw )
{
	static void ( *pls0b_00_move_jmp[] )( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw ) = {
		pls0b_00_win_join_move, pls0b_00_ground_move, pls0b_00_win_cross_move, pls0b_00_win_shield_move,
		pls0b_00_charge_dash_move, pls0b_00_mark_move, pls0b_00_hstars_move, pls0b_00_t7_move,
		pls0b_00_charge_move, pls0b_00_hcharge_move, pls0b_00_hit_move,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->pl_type != ew->pl_type ){
		ew->routine_0 ++ ;
		return ;
	}
	pls0b_00_move_jmp[ew->type]( ew, lw ) ;
}

/*--------------------------------------------------------------*/
static void pls0b_00_end( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls0b_00_free( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			Whether the piece is on the screen: draws it if		*/
/*			so													*/
/*==============================================================*/
static Uint8 pls0b_00_disp_ck( PLAYER_WORK *ew, PLS0B_00_LOCAL_W *lw )
{
	if( ew->pos.x < scr_work.scr_left ) ew->disp_flag = 0 ;
	else if( ew->pos.x > scr_work.scr_right ) ew->disp_flag = 0 ;
	else ew->disp_flag = 1 ;
	return ew->disp_flag ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls0b_00.c" End of File	*/

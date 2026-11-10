/* Sentinel's effect 00 (character 34's shot 00): the Rocket Punch. A fist (type 0) flies out from him
   with a trail of six links behind it (type 1); dir 0 is the punch that strikes and comes back, dir 1
   the one that takes hold of the opponent. */
#define PL_CHAR_SET2_OLD
#include "player.h"
#include "pl_char.h"
#include "pl34_def.h"

/* the effect's own work, at obj_mtx: the fist keeps the links' count, a link its neighbours */
typedef struct SHL34_00_LOCAL_W {
	PLAYER_WORK	*prev;
	PLAYER_WORK	*next;
	Float	ofs[2];		/* where it rests from Sentinel */
	VEC2	mov;		/* how far it has moved from there */
	Sint16	cnt;		/* the links that the fist's reach needs */
} SHL34_00_LOCAL_W;
#define SHL34_00_LW( w )	( ( SHL34_00_LOCAL_W * )&( w )->obj_mtx )

/* the reach of the fist, by Sentinel's atck_level: along it, and across */
typedef struct SHL34_00_REACH {
	Sint16	x, y;
} SHL34_00_REACH;

static SHL34_00_REACH shl34_00_reach_tbl[3] = { { 0x3800, -0x1700 }, { 0x3F00, 0 }, { 0x3800, 0x1700 } } ;

static void shl34_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl34_00_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl34_00_main ;
		ew->master_w = pl_w ;
		ew->type = 0 ;
		ew->sub_no = no ;
		ew->dir = 0 ;
		ew->ef_no = 0x3400 ;
	}
	return ew ;
}

/****************************************************************/
PLAYER_WORK *shl34_00_set_1( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl34_00_main ;
		ew->master_w = pl_w ;
		ew->type = 0 ;
		ew->sub_no = no ;
		ew->dir = 1 ;
		ew->ef_no = 0x3400 ;
	}
	return ew ;
}

/*==============================================================*/
static PLAYER_WORK *shl34_00_trail_set( PLAYER_WORK *ew )
{
	PLAYER_WORK *first, *tw ;
	SHL34_00_LOCAL_W *lw, *elw ;
	Sint16 n = 5 ;
	int i ;

	if( ( first = work_get( ew, W_SHL, LIST_BEHIND ) ) ){
		first->work_func = shl34_00_main ;
		first->master_w = ew->master_w ;
		first->ef_w = ew ;
		first->dir = ew->dir ;
		first->sub_no = 0 ;
		first->type = 1 ;
		first->ef_no = 0x3400 ;
		first->timer1 = n -- ;
		lw = SHL34_00_LW( first ) ;
		elw = SHL34_00_LW( ew ) ;
		lw->prev = ew ;
		lw->next = first ;
		elw->next = first ;
	}
	for( i = 0 ; i < 5 ; i ++ ){
		if( ( tw = work_get( ew, W_SHL, LIST_BEHIND ) ) ){
			tw->work_func = shl34_00_main ;
			tw->master_w = ew->master_w ;
			tw->ef_w = ew ;
			tw->dir = ew->dir ;
			tw->sub_no = 1 ;
			tw->type = 1 ;
			tw->ef_no = 0x3400 ;
			tw->timer1 = n -- ;
			elw = SHL34_00_LW( ew ) ;
			lw = SHL34_00_LW( first ) ;
			lw->prev = ew ;
			lw->next = elw->next ;
		}
	}
	return tw ;
}

static void shl34_00_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl34_00_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl34_00_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl34_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl34_00_main( PLAYER_WORK *ew )
{
	static void ( *shl34_00_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl34_00_init, shl34_00_move, shl34_00_end, shl34_00_free,
	} ;

	shl34_00_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

static void shl34_00_punch_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl34_00_catch_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl34_00_link_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl34_00_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	BODY_SIZE_SET( ew, 0x20202020 ) ;
	if( !ew->dir ){
		if( !ew->type ) shl34_00_punch_init( ew, pl_w ) ;
		else shl34_00_link_init( ew, pl_w ) ;
	} else {
		if( !ew->type ) shl34_00_catch_init( ew, pl_w ) ;
		else shl34_00_link_init( ew, pl_w ) ;
	}
}

/* where the fist rests from him, its speed out and its slowing, 16.16 each, by sub_no */
typedef struct SHL34_00_FIST {
	Sint32	x, y, spd, acc;
} SHL34_00_FIST;

/*==============================================================*/
static void shl34_00_punch_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static SHL34_00_FIST fist_tbl[6] = {
		{ 0x690000, 0x440000, 0xA0000, 0x6000 },
		{ 0x820000, 0x5E0000, 0xE0000, 0x8000 },
		{ 0x780000, 0x7C0000, 0xA0000, 0x6000 },
		{ 0x5B0000, 0x560000, 0xA0000, 0x6000 },
		{ 0x610000, 0x690000, 0xE0000, 0x8000 },
		{ 0x5D0000, 0x810000, 0xA0000, 0x6000 },
	} ;
	PL34_LOCAL_W *tl = ( PL34_LOCAL_W * )pl_w->tlocal ;
	SHL34_00_LOCAL_W *lw = SHL34_00_LW( ew ) ;

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
	switch( ew->sub_no ){
	case 0:
		ew->sub_type = 9 ;
		if( pl_w->team_type == 3 ){
			ew->atck_no = pl34_at_rocket ;
			HIT_RESET( ew ) ;
			break ;
		}
		ew->atck_no = pl34_at_rocket_l ;
		HIT_RESET( ew ) ;
		break ;
	case 1:
		ew->sub_type = 5 ;
		ew->atck_no = pl34_at_rocket ;
		HIT_RESET( ew ) ;
		break ;
	case 2:
		ew->sub_type = 13 ;
		ew->atck_no = pl34_at_rocket2 ;
		HIT_RESET( ew ) ;
		break ;
	case 3:
		ew->sub_type = 9 ;
		ew->atck_no = pl34_at_jrocket_l ;
		HIT_RESET( ew ) ;
		break ;
	case 4:
		ew->sub_type = 5 ;
		ew->atck_no = pl34_at_jrocket ;
		HIT_RESET( ew ) ;
		break ;
	case 5:
		ew->sub_type = 13 ;
		ew->atck_no = pl34_at_jrocket2 ;
		HIT_RESET( ew ) ;
		break ;
	}
	if( !pl_w->disp_dir ){
		lw->ofs[0] = -( fist_tbl[ew->sub_no].x * 1.6666666f / 65536.0f ) ;
		lw->ofs[1] = fist_tbl[ew->sub_no].y * 2.142857f / 65536.0f ;
		ew->spd.x = -( fist_tbl[ew->sub_no].spd * 1.6666666f / 65536.0f ) ;
		ew->acc.x = -( fist_tbl[ew->sub_no].acc * 1.6666666f / 65536.0f ) ;
	} else {
		lw->ofs[0] = fist_tbl[ew->sub_no].x * 1.6666666f / 65536.0f ;
		lw->ofs[1] = fist_tbl[ew->sub_no].y * 2.142857f / 65536.0f ;
		ew->spd.x = fist_tbl[ew->sub_no].spd * 1.6666666f / 65536.0f ;
		ew->acc.x = fist_tbl[ew->sub_no].acc * 1.6666666f / 65536.0f ;
	}
	ew->pos.x = pl_w->pos.x + lw->ofs[0] ;
	ew->pos.y = pl_w->pos.y + lw->ofs[1] ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_OWNER ;
	pl_char_set( ew, 21, ew->sub_type ) ;
	ew->sub_prio = -2 ;
	hit_work_set( ew ) ;
	if( !shl34_00_trail_set( ew ) ){
		ew->routine_0 = 3 ;
		ew->disp_flag = 0 ;
		tl->fist_end_flag = 1 ;
	}
}

/*==============================================================*/
static void shl34_00_link_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static SHL34_00_REACH link_tbl[6] = {
		{ 0x6000, 0x4900 }, { 0x7B00, 0x5F00 }, { 0x7300, 0x7A00 },
		{ 0x5700, 0x5A00 }, { 0x5C00, 0x6A00 }, { 0x5700, 0x7F00 },
	} ;
	PLAYER_WORK *ef_w = ew->ef_w ;
	SHL34_00_LOCAL_W *lw = SHL34_00_LW( ew ) ;

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
	ew->disp_flag = 0 ;
	ew->sub_prio = ef_w->sub_prio + 1 ;
	if( ew == ef_w->behind_w ){
		lw->ofs[0] = link_tbl[ef_w->sub_no].x * 1.6666666f / 256.0f ;
		lw->ofs[1] = link_tbl[ef_w->sub_no].y * 2.142857f / 256.0f ;
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		ew->spd.x = shl34_00_reach_tbl[pl_w->atck_level].x * 1.6666666f / 256.0f ;
		ew->spd.y = shl34_00_reach_tbl[pl_w->atck_level].y * 2.142857f / 256.0f ;
		if( !pl_w->disp_dir ){
			ew->spd.x = -ew->spd.x ;
			lw->ofs[0] = -lw->ofs[0] ;
		}
	}
	switch( ef_w->sub_no ){
	case 0:
		ew->sub_type = 8 ;
		break ;
	case 1:
		ew->sub_type = 4 ;
		break ;
	case 2:
		ew->sub_type = 12 ;
		break ;
	case 3:
		ew->sub_type = 8 ;
		break ;
	case 4:
		ew->sub_type = 4 ;
		break ;
	case 5:
		ew->sub_type = 12 ;
		break ;
	}
	pl_char_set( ew, 21, ew->sub_type ) ;
}

/*==============================================================*/
static void shl34_00_catch_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static SHL34_00_FIST fist_tbl[6] = {
		{ 0x690000, 0x440000, 0xA0000, 0x6000 },
		{ 0x820000, 0x5E0000, 0xE0000, 0x8000 },
		{ 0x780000, 0x7C0000, 0xA0000, 0x6000 },
		{ 0x5B0000, 0x560000, 0xA0000, 0x6000 },
		{ 0x610000, 0x690000, 0xE0000, 0x8000 },
		{ 0x5D0000, 0x810000, 0xA0000, 0x6000 },
	} ;
	PL34_LOCAL_W *tl = ( PL34_LOCAL_W * )pl_w->tlocal ;
	SHL34_00_LOCAL_W *lw = SHL34_00_LW( ew ) ;

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
	switch( ew->sub_no ){
	case 0:
		ew->sub_type = 9 ;
		break ;
	case 1:
		ew->sub_type = 5 ;
		break ;
	case 2:
		ew->sub_type = 13 ;
		break ;
	case 3:
		ew->sub_type = 9 ;
		break ;
	case 4:
		ew->sub_type = 5 ;
		break ;
	case 5:
		ew->sub_type = 13 ;
		break ;
	}
	if( !pl_w->disp_dir ){
		lw->ofs[0] = -( fist_tbl[ew->sub_no].x * 1.6666666f / 65536.0f ) ;
		lw->ofs[1] = fist_tbl[ew->sub_no].y * 2.142857f / 65536.0f ;
		ew->spd.x = -( fist_tbl[ew->sub_no].spd * 1.6666666f / 65536.0f ) ;
		ew->acc.x = -( fist_tbl[ew->sub_no].acc * 1.6666666f / 65536.0f ) ;
	} else {
		lw->ofs[0] = fist_tbl[ew->sub_no].x * 1.6666666f / 65536.0f ;
		lw->ofs[1] = fist_tbl[ew->sub_no].y * 2.142857f / 65536.0f ;
		ew->spd.x = fist_tbl[ew->sub_no].spd * 1.6666666f / 65536.0f ;
		ew->acc.x = fist_tbl[ew->sub_no].acc * 1.6666666f / 65536.0f ;
	}
	ew->pos.x = pl_w->pos.x + lw->ofs[0] ;
	ew->pos.y = pl_w->pos.y + lw->ofs[1] ;
	pl_char_set( ew, 21, ew->sub_type ) ;
	ew->sub_prio = -2 ;
	if( !shl34_00_trail_set( ew ) ){
		ew->routine_0 = 3 ;
		ew->disp_flag = 0 ;
		tl->fist_end_flag = 1 ;
	}
}

static void shl34_00_spd_add( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl34_00_follow( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl34_00_char_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static int shl34_00_scr_ck( PLAYER_WORK *ew, Uint8 no );

/*==============================================================*/
/*			The fist flies out until it hits, leaves the		*/
/*			screen, or the buttons turn it back					*/
/*==============================================================*/
static void shl34_00_punch_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w ;

	shl34_00_spd_add( ew, pl_w ) ;
	if( ew->hit_flag ){
		em_w = ew->em_w ;
		ew->timer0 = 3 ;
		ew->routine_1 ++ ;
		shl34_00_follow( ew, pl_w ) ;
		if( !em_w->work_kind ){
			scr_work.quake_level = 1 ;
			scr_work.quake_flag = 1 ;
			pl_se_req( pl_w, 48 ) ;
		}
		return ;
	}
	if( shl34_00_scr_ck( ew, 0 ) || ( ( pl_w->pl_sw_now & KEY_PUNCH ) && ( pl_w->pl_sw_new & KEY_BACK ) ) ){
		ew->timer0 = 3 ;
		ew->routine_1 ++ ;
	}
	shl34_00_follow( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl34_00_punch_turn( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Sint32 spd_tbl[6][2] = {
		{ 0xE0000, 0x8000 }, { 0xA0000, 0x6000 }, { 0xA0000, 0x6000 },
		{ 0xE0000, 0x8000 }, { 0xA0000, 0x6000 }, { 0xA0000, 0x6000 },
	} ;

	if( !ew->timer0 -- ){
		ew->spd.x = spd_tbl[ew->sub_no][0] * 1.6666666f / 65536.0f ;
		ew->acc.x = spd_tbl[ew->sub_no][1] * 1.6666666f / 65536.0f ;
		if( ew->disp_dir ){
			ew->spd.x = -ew->spd.x ;
			ew->acc.x = -ew->acc.x ;
		}
		ew->atck_flag = 0 ;
		ew->routine_1 ++ ;
	}
	shl34_00_follow( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl34_00_punch_back( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL34_00_LOCAL_W *lw = SHL34_00_LW( ew ) ;
	PL34_LOCAL_W *tl = ( PL34_LOCAL_W * )pl_w->tlocal ;
	Float x, dx ;

	x = pl_w->pos.x + lw->ofs[0] ;
	dx = ew->pos.x - x ;
	if( !ew->disp_dir ) dx = -dx ;
	if( dx <= 0.0f ){
		ew->pos.x = pl_w->pos.x + lw->ofs[0] ;
		ew->pos.y = pl_w->pos.y + lw->ofs[1] ;
		lw->mov.x = 0.0f ;
		ew->atck_flag = 0 ;
		ew->timer0 = 3 ;
		tl->fist_end_flag = 1 ;
		ew->routine_1 ++ ;
		shl34_00_char_move( ew, pl_w ) ;
	} else {
		shl34_00_spd_add( ew, pl_w ) ;
		shl34_00_follow( ew, pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl34_00_punch_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->timer0 -- ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	shl34_00_follow( ew, pl_w ) ;
}

/*==============================================================*/
static void shl34_00_link( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ef_w = ew->ef_w ;
	SHL34_00_LOCAL_W *lw = SHL34_00_LW( ef_w ) ;

	if( ef_w->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( ew->timer1 <= lw->cnt ) ew->disp_flag = scr_in_y_ck( ew ) ;
}

/*==============================================================*/
static void shl34_00_catch_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w = pl_w->catch_w ;
	VEC3 pos ;

	if( em_w->wall_flag ){
		ew->timer0 = 3 ;
		ew->routine_1 ++ ;
		em_w->dm_hit_w = pl_w ;
		em_w->catch_type = 1 ;
		em_w->pl_stance = STANCE_AIR ;
		em_w->atck_no = pl34_at_throw ;
		pl_w->atck_no = pl34_at_throw ;
		pos.x = em_w->pos.x ;
		pos.y = ew->pos.y ;
		burst_set( em_w, &pos, 1, 0x89 ) ;
		pl_se_req( ew, 73 ) ;
	} else {
		shl34_00_spd_add( ew, pl_w ) ;
	}
	shl34_00_follow( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl34_00_catch_turn( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Sint32 spd_tbl[6][2] = {
		{ 0xE0000, 0x8000 }, { 0xA0000, 0x6000 }, { 0xA0000, 0x6000 },
		{ 0xE0000, 0x8000 }, { 0xA0000, 0x6000 }, { 0xA0000, 0x6000 },
	} ;

	if( !ew->timer0 -- ){
		ew->spd.x = spd_tbl[ew->sub_no][0] * 1.6666666f / 65536.0f ;
		ew->acc.x = spd_tbl[ew->sub_no][1] * 1.6666666f / 65536.0f ;
		if( ew->disp_dir ){
			ew->spd.x = -ew->spd.x ;
			ew->acc.x = -ew->acc.x ;
		}
		ew->routine_1 ++ ;
	}
	shl34_00_follow( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl34_00_catch_back( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL34_00_LOCAL_W *lw = SHL34_00_LW( ew ) ;
	PL34_LOCAL_W *tl = ( PL34_LOCAL_W * )pl_w->tlocal ;
	Float x, dx ;

	x = pl_w->pos.x + lw->ofs[0] ;
	dx = ew->pos.x - x ;
	if( !ew->disp_dir ) dx = -dx ;
	if( dx <= 0.0f ){
		ew->pos.x = pl_w->pos.x + lw->ofs[0] ;
		ew->pos.y = pl_w->pos.y + lw->ofs[1] ;
		lw->mov.x = 0.0f ;
		ew->timer0 = 3 ;
		tl->fist_end_flag = 1 ;
		ew->routine_1 ++ ;
		shl34_00_char_move( ew, pl_w ) ;
	} else {
		shl34_00_spd_add( ew, pl_w ) ;
		shl34_00_follow( ew, pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl34_00_catch_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->timer0 -- ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	shl34_00_follow( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl34_00_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl34_00_punch_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl34_00_punch_fly, shl34_00_punch_turn, shl34_00_punch_back, shl34_00_punch_wait,
	} ;
	static void ( *shl34_00_link_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl34_00_link,
	} ;
	static void ( *shl34_00_catch_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl34_00_catch_fly, shl34_00_catch_turn, shl34_00_catch_back, shl34_00_catch_wait,
	} ;
	PL34_LOCAL_W *tl = ( PL34_LOCAL_W * )pl_w->tlocal ;

	ew->prio = pl_w->prio ;
	if( !ew->dir ){
		if( !ew->type ){
			if( !pl_w->routine_1 && pl_w->act_st == N_SPECIAL ){
				shl34_00_punch_jmp[ew->routine_1]( ew, pl_w ) ;
			} else {
				ew->routine_0 ++ ;
				ew->disp_flag = 0 ;
				tl->fist_end_flag = 1 ;
				return ;
			}
		} else {
			if( pl_w->routine_1 || pl_w->act_st != N_SPECIAL ){
				ew->routine_0 ++ ;
				ew->disp_flag = 0 ;
				return ;
			}
			ew->disp_flag = 0 ;
			shl34_00_link_jmp[ew->routine_1]( ew, pl_w ) ;
		}
	} else {
		if( !ew->type ){
			if( pl_w->routine_1 != PL_CATCH || pl_w->act_st != N_CATCH ){
				ew->routine_0 ++ ;
				ew->disp_flag = 0 ;
				tl->fist_end_flag = 1 ;
				return ;
			} else {
				shl34_00_catch_jmp[ew->routine_1]( ew, pl_w ) ;
			}
		} else {
			if( pl_w->routine_1 != PL_CATCH || pl_w->act_st != N_CATCH ){
				ew->routine_0 ++ ;
				ew->disp_flag = 0 ;
				return ;
			} else {
				ew->disp_flag = 0 ;
				shl34_00_link_jmp[ew->routine_1]( ew, pl_w ) ;
			}
		}
	}
}

/*--------------------------------------------------------------*/
static void shl34_00_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl34_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
static void shl34_00_spd_add( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL34_00_LOCAL_W *lw = SHL34_00_LW( ew ) ;

	ew->spd.x += ew->acc.x ;
	ew->spd.y += ew->acc.y ;
	lw->mov.x = lw->mov.x + ew->spd.x ;
	lw->mov.y = lw->mov.y + ew->spd.y ;
}

/*==============================================================*/
static void shl34_00_follow( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL34_00_LOCAL_W *lw = SHL34_00_LW( ew ) ;

	ew->pos.x = pl_w->pos.x + lw->ofs[0] + lw->mov.x ;
	ew->pos.y = pl_w->pos.y + lw->ofs[1] ;
	shl34_00_char_move( ew, pl_w ) ;
}

static void shl34_00_trail_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl34_00_char_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->dir ) hit_work_set( ew ) ;
	pl_char_move( ew ) ;
	ew->disp_flag = scr_in_y_ck( ew ) ;
	shl34_00_trail_move( ew, pl_w ) ;
}

static int shl34_00_floor_ck( PLAYER_WORK *ew );

/*==============================================================*/
static int shl34_00_scr_ck( PLAYER_WORK *ew, Uint8 no )
{
	static Float width_tbl[2] = { 64.0f, 32.0f } ;

	if( ew->pos.x - width_tbl[no] < scr_work.scr_left ) return 1 ;
	if( ew->pos.x + width_tbl[no] > scr_work.scr_right ) return 1 ;
	return shl34_00_floor_ck( ew ) ;
}

/*==============================================================*/
static int shl34_00_floor_ck( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->ground_y > ew->pos.y + -DOT_Y( 16.0f ) ){
		scr_work.quake_level = 1 ;
		scr_work.quake_flag = 1 ;
		pl_se_req( pl_w, 48 ) ;
		return 1 ;
	}
	return 0 ;
}

/*==============================================================*/
static void shl34_00_trail_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Sint16 arc_tbl[2][64] = { {
		0, 0, 0, 0, -0x100, -0x100, -0x200, -0x200,
		-0x300, -0x300, -0x400, -0x400, -0x500, -0x500, -0x600, -0x600,
		-0x700, -0x700, -0x700, -0x700, -0x800, -0x800, -0x800, -0x900,
		-0xA00, -0xA00, -0xA00, -0xB00, -0xB00, -0xB00, -0xC00, -0xC00,
		-0xD00, -0xE00, -0xE00, -0xE00, -0xF00, -0xF00, -0x1000, -0x1000,
		-0x1000, -0x1100, -0x1100, -0x1200, -0x1200, -0x1200, -0x1300, -0x1400,
		-0x1400, -0x1400, -0x1500, -0x1500, -0x1500, -0x1600, -0x1600, -0x1700,
		-0x1700, -0x1700, -0x1800, -0x1900, -0x1900, -0x1A00, -0x1B00, -0x1B00,
	}, {
		0, 0, 0, 0, 0x100, 0x100, 0x200, 0x200,
		0x300, 0x300, 0x400, 0x400, 0x500, 0x500, 0x600, 0x600,
		0x700, 0x700, 0x700, 0x700, 0x800, 0x800, 0x800, 0x900,
		0xA00, 0xA00, 0xA00, 0xB00, 0xB00, 0xB00, 0xC00, 0xC00,
		0xD00, 0xE00, 0xE00, 0xE00, 0xF00, 0xF00, 0x1000, 0x1000,
		0x1000, 0x1100, 0x1100, 0x1200, 0x1200, 0x1200, 0x1300, 0x1400,
		0x1400, 0x1400, 0x1500, 0x1500, 0x1500, 0x1600, 0x1600, 0x1700,
		0x1700, 0x1700, 0x1800, 0x1900, 0x1900, 0x1A00, 0x1B00, 0x1B00,
	} } ;
	SHL34_00_LOCAL_W *lw = SHL34_00_LW( ew ) ;
	SHL34_00_LOCAL_W *lw1, *elw = SHL34_00_LW( ew ) ;
	PLAYER_WORK *tw = ew, *first, *em_w ;
	Float dist, f20, fx, fy ;
	Sint16 len, step, rest ;
	int i ;
	Sint16 cnt ;

	dist = ew->pos.x - pl_w->pos.x - lw->ofs[0] ;
	if( 0 ) elw = elw + 1 ;
	if( !pl_w->disp_dir ) dist = -dist ;
	if( dist < 0.0f ) return ;
	f20 = shl34_00_reach_tbl[pl_w->atck_level].x * 1.6666666f / 256.0f ;
	len = dist / 1.6666666f ;
	step = f20 / 1.6666666f ;
	cnt = len / step ;
	lw->cnt = cnt ;
	if( !pl_w->atck_level ){
		rest = len % step ;
		ew->pos.y = shl34_00_reach_tbl[pl_w->atck_level].y * 2.142857f / 256.0f * cnt + arc_tbl[0][rest & 63] * 2.142857f / 256.0f + pl_w->pos.y + lw->ofs[1] ;
	} else if( pl_w->atck_level == 2 ){
		rest = len % step ;
		ew->pos.y = shl34_00_reach_tbl[pl_w->atck_level].y * 2.142857f / 256.0f * cnt + arc_tbl[1][rest & 63] * 2.142857f / 256.0f + pl_w->pos.y + lw->ofs[1] ;
	}
	if( ew->dir == 1 && !ew->type && ew->routine_1 < 1 ){
		em_w = pl_w->catch_w ;
		if( !em_w->wall_flag && em_w && em_w->vital_new ) catch_pos_set( ew, em_w ) ;
	}
	tw = tw->behind_w ;
	first = tw ;
	lw1 = SHL34_00_LW( tw ) ;
	fx = pl_w->pos.x + lw1->ofs[0] ;
	fy = pl_w->pos.y + lw1->ofs[1] ;
	tw->pos.x = fx ;
	tw->pos.y = fy ;
	pl_char_move( first ) ;
	if( elw->cnt - 1 < 0 ) return ;
	i = 0 ;
	while( i < elw->cnt ){
		tw = tw->behind_w ;
		if( !tw ) return ;
		fx += first->spd.x ;
		tw->pos.x = fx ;
		fy += first->spd.y ;
		tw->pos.y = fy ;
		if( first->cg_type != tw->cg_type ) pl_char_set2( tw, 21, tw->sub_type, first->cg_type ) ;
		i ++ ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl34_00.c" End of File	*/

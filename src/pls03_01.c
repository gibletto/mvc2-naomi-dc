/* Morrigan's effect 01 (character 03's pls01): the grip of her kick throw. The first work holds the
   opponent (type 0), then puts out a second that flies out beside her and back (type 1). */
#include "player.h"
#include "pl_char.h"
#include "pl03_def.h"

static void pls03_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls03_01_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_END ) ) ){
		ew->work_func = pls03_01_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = 0 ;
		ew->sub_type = 0 ;
		ew->ef_no = 0x301 ;
	}
	return ew ;
}

/*==============================================================*/
/*			The second work, on the first						*/
/*==============================================================*/
static PLAYER_WORK *pls03_01_next_set( PLAYER_WORK *ow )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( ow, W_PLS, LIST_BEFORE ) ) ){
		ew->work_func = pls03_01_main ;
		ew->master_w = ow->master_w ;
		ew->ef_w = ow ;
		ew->pl_type = ow->pl_type ;
		ew->type = 1 ;
		ew->sub_type = ow->sub_type + 1 ;
		ew->ef_no = 0x301 ;
	}
	return ew ;
}

static void pls03_01_move_a( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_01_move_b( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls03_01_main( PLAYER_WORK *ew )
{
	static void ( *pls03_01_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls03_01_move_a, pls03_01_move_b,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->prio = pl_w->prio ;
	pls03_01_jmp[ew->type]( ew, pl_w ) ;
}

static void pls03_01_a_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_01_a_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls03_01_move_a( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls03_01_a_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls03_01_a_init, pls03_01_a_move, pls03_01_end, pls03_01_free,
	} ;

	pls03_01_a_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls03_01_a_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
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
	ew->sub_prio = 4 ;
	pl_char_set( ew, 15, 6 ) ;
	if( ew->disp_dir ) ew->pos.x = pl_w->pos.x + DOT_X( 104.0f ) ;
	else ew->pos.x = pl_w->pos.x - DOT_X( 104.0f ) ;
	ew->pos.y = pl_w->pos.y + DOT_Y( 67.0f ) ;
}

static void pls03_01_leave( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The opponent is held where the grip is, until he	*/
/*			breaks free											*/
/*==============================================================*/
static void pls03_01_a_catch( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->catch_w ;

	if( !em->wall_flag ){
		ew->routine_1 ++ ;
		if( pls03_01_next_set( ew ) ){
			catch_pos_set( ew, em ) ;
			return ;
		}
	}
	ew->sub_type |= 0x80 ;
	ew->routine_1 = 2 ;
	pls03_01_leave( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls03_01_a_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->catch_w ;
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	if( em->wall_flag || ew->sub_type >= 0x80 ){
		ew->routine_1 ++ ;
		return ;
	}
	if( !lw->leave_flag ) catch_pos_set( ew, em ) ;
}

/*--------------------------------------------------------------*/
static void pls03_01_a_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ew->sub_type ){
		ew->routine_0 ++ ;
		pl_w->routine_2 = 2 ;
	}
}

/*--------------------------------------------------------------*/
static void pls03_01_a_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls03_01_a_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls03_01_a_catch, pls03_01_a_hold, pls03_01_a_wait,
	} ;

	pls03_01_a_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

static void pls03_01_b_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_01_b_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls03_01_move_b( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls03_01_b_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls03_01_b_init, pls03_01_b_move, pls03_01_end, pls03_01_free,
	} ;

	pls03_01_b_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls03_01_b_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
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
	ew->sub_prio = 2 ;
	pl_char_set( ew, 20, 3 ) ;
	ew->spd.x = DOT_X( 32.0f ) ;
	ew->acc.x = 0.0f ;
	ew->pos.x = DOT_X( 104.0f ) ;
	if( ew->disp_dir ) ew->pos.x = pl_w->pos.x + ew->pos.x ;
	else {
		ew->spd.x = -ew->spd.x ;
		ew->pos.x = pl_w->pos.x - ew->pos.x ;
	}
	ew->pos.y = pl_w->pos.y + DOT_Y( 67.0f ) ;
}

/*==============================================================*/
/*			It flies out, the first work following its end		*/
/*==============================================================*/
static void pls03_01_b_out( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ow = ew->ef_w ;
	int done ;
	int d ;

	ew->pos.x += ew->spd.x ;
	if( ew->spd.x > 0.0f ) ew->acc.x += ew->spd.x ;
	else ew->acc.x -= ew->spd.x ;
	done = 0 ;
	while( !done ){
		d = 0 ;
		if( ow->sub_type ) d = 0x200000 ;
		if( !ew->disp_dir ) d = -d ;
		ow->pos.x = ew->pos.x + d * 1.6666666f / 65536.0f ;
		if( ew->acc.x < DOT_X( 32.0f ) ) done = 1 ;
		else if( ew->acc.x == DOT_X( 32.0f ) ){
			done = 1 ;
			ew->routine_1 ++ ;
		}
		else {
			ew->acc.x = DOT_X( 32.0f ) ;
			d = 0x880000 ;
			if( !ew->disp_dir ) d = -0x880000 ;
			ew->pos.x = pl_w->pos.x + d * 1.6666666f / 65536.0f ;
		}
	}
}

/*--------------------------------------------------------------*/
static void pls03_01_b_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->catch_w ;
	PLAYER_WORK *tw = em->catch_pair.w ;

	if( !em->wall_flag && tw->routine_1 != 2 ){
		ew->routine_1 ++ ;
		if( pls03_01_next_set( ew ) ) return ;
	}
	ew->sub_type |= 0x80 ;
	ew->routine_1 = 3 ;
	pls03_01_leave( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls03_01_b_follow( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ow = ew->ef_w ;
	Sint16 d ;

	if( ew->sub_type >= 0x80 ) ew->routine_1 ++ ;
	d = 0 ;
	if( ow->sub_type ) d = 0x2000 ;
	if( !ow->disp_dir ) d = -d ;
	ow->pos.x = ew->pos.x + d * 1.6666666f / 256.0f ;
}

/*==============================================================*/
/*			It comes back in, and the first work with it		*/
/*==============================================================*/
static void pls03_01_b_back( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ow = ew->ef_w ;
	Sint16 d ;

	ew->pos.x -= ew->spd.x ;
	if( ew->spd.x > 0.0f ) ew->acc.x -= ew->spd.x ;
	else ew->acc.x += ew->spd.x ;
	if( ew->acc.x < 0.0f ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		ow->sub_type |= 0x80 ;
		return ;
	}
	d = 0 ;
	if( ow->sub_type ) d = 0x2000 ;
	if( !ow->disp_dir ) d = -d ;
	ow->pos.x = ew->pos.x + d * 1.6666666f / 256.0f ;
}

/*--------------------------------------------------------------*/
static void pls03_01_b_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls03_01_b_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls03_01_b_out, pls03_01_b_hold, pls03_01_b_follow, pls03_01_b_back,
	} ;

	pls03_01_b_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls03_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls03_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			The opponent is let go: Morrigan has him thrown		*/
/*==============================================================*/
static void pls03_01_leave( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->catch_w ;
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;
	VEC3 pos ;

	lw->leave_flag = -1 ;
	pl_se_req( ew, 73 ) ;
	em->dm_hit_w = pl_w ;
	em->catch_type = 1 ;
	em->pl_stance = STANCE_AIR ;
	em->atck_no = pl03_at_throw_k ;
	pos.x = em->pos.x ;
	pos.y = ew->pos.y ;
	burst_set( em, &pos, 1, 0x89 ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls03_01.c" End of File	*/

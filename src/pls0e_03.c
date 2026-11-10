/* Venom's player effect 03: the web rope of his Web Throw. A row of pieces is made at once;
   the first one runs the rope, turns it by a table of angles and puts each piece at the end of the
   one before it, and holds the opponent at the last one. */
#include "player.h"
#include "pl0e_def.h"

/* where each piece's two ends are, by its direction (0 to 31) */
Sint16 pls0e_03_end_tbl[32][4] = {
	{ 0, 6, 0, -6 }, { 1, 6, -1, -6 }, { 3, 6, -3, -6 }, { 5, 6, -4, -5 },
	{ 6, 5, -5, -4 }, { 7, 3, -7, -3 }, { 8, 2, -7, -2 }, { 8, 1, -7, -1 },
	{ 8, 0, -7, 0 }, { 8, -1, -7, 1 }, { 8, -2, -7, 2 }, { 7, -3, -7, 3 },
	{ 6, -5, -5, 4 }, { 5, -6, -4, 5 }, { 3, -6, -3, 6 }, { 1, -6, -1, 6 },
	{ 0, -6, 0, 6 }, { -1, -6, 1, 6 }, { -3, -6, 3, 6 }, { -5, -6, 4, 5 },
	{ -6, -5, 5, 4 }, { -7, -3, 7, 3 }, { -8, -2, 7, 2 }, { -8, -1, 7, 1 },
	{ -8, 0, 7, 0 }, { -8, 1, 7, -1 }, { -8, 2, 7, -2 }, { -7, 3, 7, -3 },
	{ -6, 5, 5, -4 }, { -5, 6, 4, -5 }, { -3, 6, 3, -6 }, { -1, 6, 1, -6 },
} ;

/* the last piece's tip, an x and a y for each eighth of a turn */
static Sint16 pls0e_03_tip_tbl[16] = {
	0, -6, -6, -5, -8, 0, -6, 5, 0, 6, 6, 5, 8, 0, 6, -5,
} ;

/* the animation of each piece, by its number */
Uint8 pls0e_03_char_tbl[16] = { 2, 1, 0, 2, 1, 2, 0, 1, 1, 0, 2, 0, 1, 2, 1, 0 } ;

/* a turn of the rope: the angle it ends at (256 to a turn), the way it turns, and the frames it takes */
typedef Sint16 PLS0E_03_KEY[4];

/* the work's place in its table of turns, kept where an effect keeps the work it rides on */
#define PLS0E_03_KEY_W( ew )	( *( Sint16 *** )&( ew )->ef_w )

static PLS0E_03_KEY pls0e_03_key_0[] = {
	{ 0, 1, 6, 0 }, { 224, -1, 6, 0 }, { 192, -1, 4, 0 }, { 248, 1, 16, 0 }, { 248, 1, 4, 0 },
	{ 0, 1, 4, 0 }, { 48, 1, 6, 0 }, { 8, -1, 16, 0 }, { 8, -1, 4, 0 }, { 0, -1, 6, 0 },
} ;

static Sint16 *pls0e_03_keys_0[] = {
	pls0e_03_key_0[0], pls0e_03_key_0[1], pls0e_03_key_0[2], pls0e_03_key_0[3], pls0e_03_key_0[4],
	pls0e_03_key_0[5], pls0e_03_key_0[6], pls0e_03_key_0[7], pls0e_03_key_0[8], pls0e_03_key_0[9],
	0,
} ;

static PLS0E_03_KEY pls0e_03_key_1[] = {
	{ 0, 1, 6, 0 }, { 192, -1, 4, 0 }, { 248, 1, 12, 0 }, { 248, 1, 4, 0 }, { 0, 1, 3, 0 },
	{ 48, 1, 4, 0 }, { 8, -1, 12, 0 }, { 8, -1, 4, 0 }, { 240, -1, 6, 0 },
} ;

static Sint16 *pls0e_03_keys_1[] = {
	pls0e_03_key_1[0], pls0e_03_key_1[1], pls0e_03_key_1[2], pls0e_03_key_1[3], pls0e_03_key_1[4],
	pls0e_03_key_1[5], pls0e_03_key_1[6], pls0e_03_key_1[7], pls0e_03_key_1[8],
	0,
} ;

/* the turns of each type */
static Sint16 **pls0e_03_keys_tbl[] = { pls0e_03_keys_0, pls0e_03_keys_1 } ;

static void pls0e_03_main( PLAYER_WORK *ew );

/****************************************************************/
int pls0e_03_set( PLAYER_WORK *pl_w, int n, int type, int no )
{
	PLAYER_WORK *ew ;
	int i ;

	if( work_rest_cnt <= n ) return 0 ;
	if( n > 12 ) n = 12 ;
	for( i = 0 ; i < n ; i ++ ){
		if( ( ew = work_get( 0, W_PLS, LIST_END ) ) ){
			ew->ef_no = 0xe03 ;
			ew->sub_no = i ;
			ew->type = type ;
			ew->sub_type = no ;
			ew->timer1 = n ;
			ew->work_func = pls0e_03_main ;
			ew->master_w = pl_w ;
		}
		if( no ) no -- ;
	}
	return i ;
}

static void pls0e_03_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0e_03_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0e_03_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Only the first piece runs; the rest are moved by	*/
/*			it													*/
/*==============================================================*/
static void pls0e_03_main( PLAYER_WORK *ew )
{
	static void ( *pls0e_03_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0e_03_init, pls0e_03_move, pls0e_03_free,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( ew->sub_no ) return ;
	if( pl_w->routine_1 || pl_w->act_st != N_SPECIAL || pl_w->pl_sp_type != 2 ) ew->routine_0 = 2 ;
	pls0e_03_jmp[ew->routine_0]( ew, pl_w ) ;
}

static void pls0e_03_make( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static int pls0e_03_key_next( PLAYER_WORK *ew );
static int pls0e_03_turn( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0e_03_place( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0e_03_anim( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls0e_03_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0E_LOCAL_W *lw = ( PL0E_LOCAL_W * )pl_w->tlocal ;

	pls0e_03_make( ew, pl_w ) ;
	ew->routine_0 ++ ;
	ew->pos.x = pl_w->pos.x + lw->hand_x ;
	ew->pos.y = pl_w->pos.y + lw->hand_y ;
	PLS0E_03_KEY_W( ew ) = pls0e_03_keys_tbl[ew->type] ;
	pls0e_03_key_next( ew ) ;
	pls0e_03_turn( ew, pl_w ) ;
	pls0e_03_place( ew, pl_w ) ;
	pls0e_03_anim( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls0e_03_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0E_LOCAL_W *lw = ( PL0E_LOCAL_W * )pl_w->tlocal ;

	if( vs_sys_work.slow_bit & ( 1 << vs_sys_work.slow_step ) ) return ;
	pls0e_03_turn( ew, pl_w ) ;
	pls0e_03_place( ew, pl_w ) ;
	pls0e_03_anim( ew, pl_w ) ;
	if( lw->line_st == 2 || lw->line_st == -1 ){
		ew->routine_0 = 2 ;
		pls0e_03_free( ew, pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls0e_03_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *w ;
	int i, n = ew->timer1 ;

	for( i = 1 ; i < n ; i ++ ){
		w = ew->behind_w ;
		w->disp_flag = 0 ;
		work_free( w ) ;
	}
	work_free( ew ) ;
}

/*==============================================================*/
/*			The pieces take their place in the fight, a copy	*/
/*			of him each											*/
/*==============================================================*/
static void pls0e_03_make( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *top ;
	int i, n = ew->timer1 ;

	for( i = 0 ; i < n ; i ++ ){
		top = ew ;
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
		ew->disp_flag = ew->sub_type ? 1 : 0 ;
		ew->prio = pl_w->prio ;
		ew->sub_prio = 8 ;
		ew->pos = pl_w->pos ;
		ew = ew->behind_w ;
	}
	pl_catch_pass( pl_w, top ) ;
	top->disp_flag = 1 ;
	top->catch_grip_old = -1 ;
}

/* each piece takes the animation its number gives, at a frame of its own; the last one catches */

/*==============================================================*/
static void pls0e_03_anim( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	int i, no, on = 0, n = ew->timer1 ;

	for( i = 0 ; i < n - 1 ; i ++ ){
		no = pls0e_03_char_tbl[ew->sub_no] + 85 ;
		pl_char_set2( ew, Char_Effect, no, ew->dir ) ;
		if( !ew->disp_flag && !on ){
			ew->disp_flag = 1 ;
			on = 1 ;
		}
		ew = ew->behind_w ;
	}
	if( 0 ) ew->sub_no = i ;
	pl_char_set2( ew, 15, 5, ( ( ew->dir + 2 ) & 31 ) >> 2 ) ;
	catch_pos_set( ew, ew->catch_w ) ;
}

/*==============================================================*/
/*			Each piece goes to the end of the one before it,	*/
/*			the first to his hand								*/
/*==============================================================*/
static void pls0e_03_place( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0E_LOCAL_W *lw = ( PL0E_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *w ;
	Sint16 *a ;
	Float x0, y0, x1, y1 ;
	int i, n ;

	a = &pls0e_03_end_tbl[ew->dir][2] ;
	x0 = *a ++ * 1.6666666f ;
	y0 = *a * 2.142857f ;
	if( ew->disp_dir ) x0 = -x0 ;
	ew->pos.x = pl_w->pos.x + lw->hand_x - x0 ;
	ew->pos.y = pl_w->pos.y + lw->hand_y - y0 ;
	n = ew->timer1 ;
	for( i = 1 ; i < n - 1 ; i ++ ){
		w = ew->behind_w ;
		if( !w->disp_flag ){
			x0 = x1 = 0 ;
			y0 = y1 = 0 ;
		}
		else {
			a = pls0e_03_end_tbl[ew->dir] ;
			x0 = *a ++ * 1.6666666f ;
			y0 = *a * 2.142857f ;
			a = &pls0e_03_end_tbl[w->dir][2] ;
			x1 = *a ++ * 1.6666666f ;
			y1 = *a * 2.142857f ;
			if( ew->disp_dir ){
				x0 = -x0 ;
				x1 = -x1 ;
			}
		}
		w->pos.x = ew->pos.x + x0 - x1 ;
		w->pos.y = ew->pos.y + y0 - y1 ;
		ew = w ;
	}
	w = ew->behind_w ;
	a = pls0e_03_end_tbl[ew->dir] ;
	x0 = *a ++ * 1.6666666f ;
	y0 = *a * 2.142857f ;
	a = &pls0e_03_tip_tbl[( ( ( w->dir + 2 ) & 31 ) >> 2 ) * 2] ;
	x1 = *a ++ * 1.6666666f ;
	y1 = *a * 2.142857f ;
	if( ew->disp_dir ){
		x0 = -x0 ;
		x1 = -x1 ;
	}
	w->pos.x = ew->pos.x + x0 - x1 ;
	w->pos.y = ew->pos.y + y0 - y1 ;
}

/*==============================================================*/
/*			The rope turns a step: each piece's angle			*/
/*			follows from the one before it						*/
/*==============================================================*/
static int pls0e_03_turn( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *w ;
	int i, n, ret = 1 ;

	ew->dir = pl_w->dir ;
	if( ew->timer0 ){
		ew->timer0 -- ;
		n = ew->timer1 ;
		for( i = 1 ; i < n ; i ++ ){
			w = ew->behind_w ;
			w->spd.y += w->spd.z ;
			w->acc.y = ew->acc.y + w->spd.y ;
			if( w->acc.y < 0 ) w->acc.y += 256.0f ;
			if( !( 256.0f > w->acc.y ) ) w->acc.y -= 256.0f ;
			w->dir = ( ( ( Uint8 )( int )w->acc.y + 4 ) >> 3 ) + pl_w->dir & 31 ;
			ew = w ;
		}
	}
	else ret = pls0e_03_key_next( ew ) ;
	return ret ;
}

/*==============================================================*/
/*			The next turn of the table: the rope's angle it		*/
/*			ends at, and the speed that gives each piece		*/
/*==============================================================*/
static int pls0e_03_key_next( PLAYER_WORK *ew )
{
	Sint16 **kp, *key ;
	PLAYER_WORK *w ;
	int i, n, ret ;

	kp = PLS0E_03_KEY_W( ew ) ;
	key = *kp ++ ;
	ret = 1 ;
	if( !key ){
		kp -= 2 ;
		key = *kp ++ ;
		ret = 0 ;
	}
	PLS0E_03_KEY_W( ew ) = kp ;
	ew->acc.y = ew->acc.x ;
	ew->acc.x = *key ++ ;
	ew->acc.z = ( ew->acc.x - ew->acc.y ) * *key ;
	if( ew->acc.z < 0 ) ew->acc.z += 256.0f ;
	ew->acc.z *= *key ++ ;
	ew->acc.z /= 12.0f ;
	ew->timer0 = *key ;
	ew->acc.y = 0 ;
	n = ew->timer1 ;
	for( i = 1 ; i < n ; i ++ ){
		w = ew->behind_w ;
		w->acc.y = ew->acc.y + w->spd.y ;
		w->acc.z = ew->acc.z ;
		w->timer0 = ew->timer0 ;
		w->spd.z = ew->acc.z / ew->timer0 ;
		if( w->acc.y < 0 ) w->acc.y += 256.0f ;
		if( !( 256.0f > w->acc.y ) ) w->acc.y -= 256.0f ;
		ew = w ;
	}
	return ret ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls0e_03.c" End of File	*/

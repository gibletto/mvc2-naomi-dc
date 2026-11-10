/* Spider-Man's player effect 02: the web he throws, a chain of up to twelve links that turn
   about his hand (ew->acc and spd hold each link's turn) and end where he holds the opponent. */
#include "player.h"
#include "pl0c_def.h"

/* where a link's two ends are, by the way it points (32 ways): the end near the player x and y,
   then the end near the next link */
Sint16 pls0c_02_ofs[32][4] = {
	0, 6, 0, -6,
	1, 6, -1, -6,
	3, 6, -3, -6,
	5, 6, -4, -5,
	6, 5, -5, -4,
	7, 3, -7, -3,
	8, 2, -7, -2,
	8, 1, -7, -1,
	8, 0, -7, 0,
	8, -1, -7, 1,
	8, -2, -7, 2,
	7, -3, -7, 3,
	6, -5, -5, 4,
	5, -6, -4, 5,
	3, -6, -3, 6,
	1, -6, -1, 6,
	0, -6, 0, 6,
	-1, -6, 1, 6,
	-3, -6, 3, 6,
	-5, -6, 4, 5,
	-6, -5, 5, 4,
	-7, -3, 7, 3,
	-8, -2, 7, 2,
	-8, -1, 7, 1,
	-8, 0, 7, 0,
	-8, 1, 7, -1,
	-8, 2, 7, -2,
	-7, 3, 7, -3,
	-6, 5, 5, -4,
	-5, 6, 4, -5,
	-3, 6, 3, -6,
	-1, 6, 1, -6,
} ;

/* where the last link's end is, by the way the link before points (8 ways) */
Sint16 pls0c_02_end_ofs[8][2] = {
	0, -6,
	-6, -5,
	-8, 0,
	-6, 5,
	0, 6,
	6, 5,
	8, 0,
	6, -5,
} ;

/* the animation of each link, by its place in the chain */
Uint8 pls0c_02_char_tbl[16] = {
	2, 1, 0, 2,
	1, 2, 0, 1,
	1, 0, 2, 0,
	1, 2, 1, 0,
} ;

/* the web's turn, step by step: the angle (0 to 255), its direction, the frames it takes */
static Sint16 pls0c_02_key_a[5][4] = {
	0, 1, 10, 0,
	224, -1, 5, 0,
	240, 1, 5, 0,
	248, 1, 6, 0,
	0, 1, 4, 0,
} ;
static Sint16 *pls0c_02_list_a[] = {
	pls0c_02_key_a[0], pls0c_02_key_a[1], pls0c_02_key_a[2], pls0c_02_key_a[3], pls0c_02_key_a[4], 0,
} ;
static Sint16 pls0c_02_key_b[5][4] = {
	0, 1, 8, 0,
	40, 1, 6, 0,
	64, 1, 4, 0,
	40, -1, 6, 0,
	0, -1, 6, 0,
} ;
static Sint16 *pls0c_02_list_b[] = {
	pls0c_02_key_b[0], pls0c_02_key_b[1], pls0c_02_key_b[4], 0,
} ;
static Sint16 **pls0c_02_list_tbl[] = { pls0c_02_list_a, pls0c_02_list_b } ;

static void pls0c_02_main( PLAYER_WORK *ew );

/****************************************************************/
int pls0c_02_set( PLAYER_WORK *pl_w, int no, int lvl, int frame )
{
	PLAYER_WORK *ew ;
	int cnt ;

	if( work_rest_cnt <= no ) return 0 ;
	if( no > 12 ) no = 12 ;
	for( cnt = 0 ; cnt < no ; cnt ++ ){
		if( ( ew = work_get( 0, W_PLS, LIST_END ) ) ){
			ew->ef_no = 0xc02 ;
			ew->sub_no = cnt ;
			ew->type = lvl ;
			ew->sub_type = frame ;
			ew->timer1 = no ;
			ew->work_func = pls0c_02_main ;
			ew->master_w = pl_w ;
		}
		if( frame ) frame -- ;
	}
	return cnt ;
}

static void pls0c_02_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0c_02_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0c_02_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls0c_02_main( PLAYER_WORK *ew )
{
	static void ( *pls0c_02_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0c_02_init, pls0c_02_move, pls0c_02_free,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( !ew->sub_no ) pls0c_02_jmp[ew->routine_0]( ew, pl_w ) ;
}

static void pls0c_02_chain_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0c_02_char_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0c_02_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls0c_02_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0C_LOCAL_W *lw = ( PL0C_LOCAL_W * )pl_w->tlocal ;

	pls0c_02_chain_set( ew, pl_w ) ;
	ew->routine_0 ++ ;
	ew->pos.x = pl_w->pos.x + lw->hand_x ;
	ew->pos.y = pl_w->pos.y + lw->hand_y ;
	PLS0C_02_KEY( ew ) = pls0c_02_list_tbl[ew->type] ;
	pls0c_02_key_next( ew ) ;
	pls0c_02_spin( ew, pl_w ) ;
	pls0c_02_pos_set( ew, pl_w ) ;
	pls0c_02_char_set( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls0c_02_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	int s ;
	PL0C_LOCAL_W *lw = ( PL0C_LOCAL_W * )pl_w->tlocal ;

	if( vs_sys_work.slow_bit & ( 1 << vs_sys_work.slow_step ) ) return ;
	if( pl_w->pl_sp_type != pl0c_sp_webthrow ){
		ew->routine_0 = 2 ;
		pls0c_02_free( ew, pl_w ) ;
		return ;
	}
	if( ( s = lw->line_st ) == 2 || s == -1 ){
		ew->routine_0 = 2 ;
		pls0c_02_free( ew, pl_w ) ;
		return ;
	}
	ew->prio = pl_w->prio ;
	ew->sub_prio = 8 ;
	pls0c_02_spin( ew, pl_w ) ;
	pls0c_02_pos_set( ew, pl_w ) ;
	pls0c_02_char_set( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls0c_02_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *w ;
	int n = ew->timer1 ;
	int i ;

	for( i = 1 ; i < n ; i ++ ){
		w = ew->behind_w ;
		w->disp_flag = 0 ;
		work_free( w ) ;
	}
	work_free( ew ) ;
}

/*==============================================================*/
/*			Every link takes the player's look; the last one	*/
/*			holds the opponent									*/
/*==============================================================*/
static void pls0c_02_chain_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *tail ;
	int n = ew->timer1 ;
	int i ;

	for( i = 0 ; i < n ; i ++ ){
		tail = ew ;
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
		ew->prio = pl_w->prio ;
		ew->sub_prio = 8 ;
		ew->pos = pl_w->pos ;
		ew->disp_flag = ew->sub_type ? 1 : 0 ;
		ew = ew->behind_w ;
	}
	pl_catch_pass( pl_w, tail ) ;
	tail->disp_flag = 1 ;
	tail->catch_grip_old = -1 ;
	tail->prio = tail->catch_w->prio ;
	tail->sub_prio = -2 ;
}

/* the animation of each link, and the end of the chain that holds the opponent */

/*==============================================================*/
static void pls0c_02_char_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	int anm ;
	int i ;
	int shown = 0 ;
	int n = ew->timer1 ;

	for( i = 0 ; i < n - 1 ; i ++ ){
		anm = pls0c_02_char_tbl[ew->sub_no] + 16 ;
		pl_char_set2( ew, Char_Effect, anm, ew->dir ) ;
		if( !ew->disp_flag && !shown ){
			ew->disp_flag = 1 ;
			shown = 1 ;
		}
		ew = ew->behind_w ;
	}
	if( 0 ) ew->sub_no = i ;
	pl_char_set2( ew, 15, 10, ( ( ew->dir + 2 ) & 31 ) >> 2 ) ;
	catch_pos_set( ew, ew->catch_w ) ;
}

/*==============================================================*/
/*			Each link's place, from the one before				*/
/*==============================================================*/
static void pls0c_02_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0C_LOCAL_W *lw = ( PL0C_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *next ;
	Sint16 *ofs = &pls0c_02_ofs[ew->dir][2] ;
	float ax, ay, bx, by ;
	int n ;
	int i ;

	ax = *ofs ++ * 1.6666666f ;
	ay = *ofs * 2.142857f ;
	if( ew->disp_dir ) ax = -ax ;
	ew->pos.x = pl_w->pos.x + lw->hand_x - ax ;
	ew->pos.y = pl_w->pos.y + lw->hand_y - ay ;
	n = ew->timer1 ;
	for( i = 1 ; i < n - 1 ; i ++ ){
		next = ew->behind_w ;
		if( !next->disp_flag ){
			ax = ay = 0.0f ;
			bx = by = 0.0f ;
		}
		else{
			ofs = pls0c_02_ofs[ew->dir] ;
			ax = *ofs ++ * 1.6666666f ;
			ay = *ofs * 2.142857f ;
			ofs = &pls0c_02_ofs[next->dir][2] ;
			bx = *ofs ++ * 1.6666666f ;
			by = *ofs * 2.142857f ;
			if( ew->disp_dir ){
				ax = -ax ;
				bx = -bx ;
			}
		}
		next->pos.x = ew->pos.x + ax - bx ;
		next->pos.y = ew->pos.y + ay - by ;
		ew = next ;
	}
	next = ew->behind_w ;
	ofs = pls0c_02_ofs[ew->dir] ;
	ax = *ofs ++ * 1.6666666f ;
	ay = *ofs * 2.142857f ;
	ofs = &pls0c_02_end_ofs[0][( ( ( next->dir + 2 ) & 31 ) >> 2 ) * 2] ;
	bx = *ofs ++ * 1.6666666f ;
	by = *ofs * 2.142857f ;
	if( ew->disp_dir ){
		ax = -ax ;
		bx = -bx ;
	}
	next->pos.x = ew->pos.x + ax - bx ;
	next->pos.y = ew->pos.y + ay - by ;
}

/****************************************************************/
/*			The links turn: each one's angle follows the one	*/
/*			before by its own phase								*/
/****************************************************************/
int pls0c_02_spin( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *w ;
	int more = 1 ;
	int n ;
	int i ;

	ew->dir = pl_w->dir ;
	if( ew->timer0 ){
		ew->timer0 -- ;
		n = ew->timer1 ;
		for( i = 1 ; i < n ; i ++ ){
			w = ew->behind_w ;
			w->spd.y += w->spd.z ;
			w->acc.y = ew->acc.y + w->spd.y ;
			if( w->acc.y < 0.0f ) w->acc.y += 256.0f ;
			if( w->acc.y >= 256.0f ) w->acc.y -= 256.0f ;
			w->dir = ( pl_w->dir + ( ( ( Uint8 )w->acc.y + 4 ) >> 3 ) ) & 31 ;
			ew = w ;
		}
	}
	else more = pls0c_02_key_next( ew ) ;
	return more ;
}

/****************************************************************/
/*			Goes on to the next step of the web's turn:			*/
/*			returns 0 at the last one							*/
/****************************************************************/
int pls0c_02_key_next( PLAYER_WORK *ew )
{
	Sint16 **list = PLS0C_02_KEY( ew ) ;
	Sint16 *key ;
	PLAYER_WORK *w ;
	int more = 1 ;
	int n ;
	int i ;

	key = *list ++ ;
	if( !key ){
		list -= 2 ;
		key = *list ++ ;
		more = 0 ;
	}
	PLS0C_02_KEY( ew ) = list ;
	ew->acc.y = ew->acc.x ;
	ew->acc.x = *key ++ ;
	ew->acc.z = ( ew->acc.x - ew->acc.y ) * *key ;
	if( ew->acc.z < 0.0f ) ew->acc.z += 256.0f ;
	ew->acc.z *= *key ++ ;
	ew->acc.z /= 12.0f ;
	ew->timer0 = *key ;
	ew->acc.y = 0.0f ;
	n = ew->timer1 ;
	for( i = 1 ; i < n ; i ++ ){
		w = ew->behind_w ;
		w->acc.y = ew->acc.y + w->spd.y ;
		w->acc.z = ew->acc.z ;
		w->timer0 = ew->timer0 ;
		w->spd.z = ew->acc.z / ew->timer0 ;
		if( w->acc.y < 0.0f ) w->acc.y += 256.0f ;
		if( w->acc.y >= 256.0f ) w->acc.y -= 256.0f ;
		ew = w ;
	}
	return more ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls0c_02.c" End of File	*/

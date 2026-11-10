/* Spider-Man's player effect 05: the web of his hyper combo's throw, a chain of up to twelve
   links from his hand to the opponent it holds. It turns with the keys of the web throw, pls0c_02. */
#include "player.h"
#include "pl0c_def.h"

extern Sint16 pls0c_02_ofs[] ;	/* pls0c_02.c: a web link's two ends, x and y, four words a direction */

/* pls0c_02's tables of the links' ends, four and two to a link */
extern Sint16 pls0c_02_end_ofs[] ;

/* the web's turn, step by step: the angle (0 to 255), its direction, the frames it takes */
static Sint16 pls0c_05_key[2][4] = {
	0, 1, 20, 0,
	48, 1, 6, 0,
} ;
static Sint16 *pls0c_05_list[] = {
	pls0c_05_key[0], pls0c_05_key[1], 0,
} ;
static Sint16 **pls0c_05_list_tbl[] = { pls0c_05_list, pls0c_05_list } ;

static void pls0c_05_main( PLAYER_WORK *ew );

/****************************************************************/
int pls0c_05_set( PLAYER_WORK *pl_w, int no, int type )
{
	PLAYER_WORK *ew ;
	int cnt ;

	if( work_rest_cnt <= no ) return 0 ;
	if( no > 12 ) no = 12 ;
	for( cnt = 0 ; cnt < no ; cnt ++ ){
		if( ( ew = work_get( 0, W_PLS, LIST_END ) ) ){
			ew->ef_no = 0xc05 ;
			ew->sub_no = cnt ;
			ew->type = type ;
			ew->timer1 = no ;
			ew->work_func = pls0c_05_main ;
			ew->master_w = pl_w ;
		}
	}
	return cnt ;
}

static void pls0c_05_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0c_05_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0c_05_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls0c_05_main( PLAYER_WORK *ew )
{
	static void ( *pls0c_05_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0c_05_init, pls0c_05_move, pls0c_05_free,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;
	PL0C_LOCAL_W *lw = ( PL0C_LOCAL_W * )pl_w->tlocal ;

	if( !ew->sub_no ){
		if( pl_w->pl_sp_type != pl0c_sp_hwebthrow || pl_w->routine_1 || pl_w->act_st != N_SUPER || lw->hweb_st == -1 )
			ew->routine_0 = 2 ;
		pls0c_05_jmp[ew->routine_0]( ew, pl_w ) ;
	}
}

static void pls0c_05_chain_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0c_05_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0c_05_char_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls0c_05_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0C_LOCAL_W *lw = ( PL0C_LOCAL_W * )pl_w->tlocal ;

	pls0c_05_chain_set( ew, pl_w ) ;
	ew->routine_0 ++ ;
	ew->pos.x = pl_w->pos.x + lw->hand_x ;
	ew->pos.y = pl_w->pos.y + lw->hand_y ;
	PLS0C_02_KEY( ew ) = pls0c_05_list_tbl[ew->type] ;
	pls0c_02_key_next( ew ) ;
	pls0c_05_pos_set( ew, pl_w ) ;
	pls0c_05_char_set( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls0c_05_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0C_LOCAL_W *lw = ( PL0C_LOCAL_W * )pl_w->tlocal ;
	int line ;

	if( vs_sys_work.slow_bit & ( 1 << vs_sys_work.slow_step ) ) return ;
	pls0c_02_spin( ew, pl_w ) ;
	pls0c_05_pos_set( ew, pl_w ) ;
	pls0c_05_char_set( ew, pl_w ) ;
	line = lw->hweb_st ;
	if( line == 2 || line == -1 ) ew->routine_0 = 2 ;
}

/*--------------------------------------------------------------*/
static void pls0c_05_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
static void pls0c_05_chain_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
		ew->prio = 7 ;
		ew->pos = pl_w->pos ;
		ew = ew->behind_w ;
	}
	pl_catch_pass( pl_w, tail ) ;
	tail->catch_grip_old = -1 ;
}

/*==============================================================*/
/*			The animation of each link, and the end of the		*/
/*			chain that holds the opponent						*/
/*==============================================================*/
static void pls0c_05_char_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	int anim ;
	int n = ew->timer1 ;
	int i ;

	for( i = 0 ; i < n - 1 ; i ++ ){
		anim = pls0c_02_char_tbl[ew->sub_no] + 16 ;
		pl_char_set2( ew, Char_Effect, anim, ew->dir ) ;
		ew = ew->behind_w ;
	}
	pl_char_set2( ew, 15, 10, ( ( ew->dir + 2 ) & 31 ) >> 2 ) ;
	catch_pos_set( ew, ew->catch_w ) ;
}

/*==============================================================*/
/*			Each link's place, from the one before				*/
/*==============================================================*/
static void pls0c_05_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0C_LOCAL_W *lw ;
	PLAYER_WORK *next ;
	Sint16 *a ;
	float ax, ay, bx, by ;
	int n ;
	int i ;

	a = &pls0c_02_ofs[ew->dir * 4 + 2] ;
	ax = *a ++ * 1.6666666f ;
	lw = ( PL0C_LOCAL_W * )pl_w->tlocal ;
	ay = *a * 2.142857f ;
	if( ew->disp_dir ) ax = -ax ;
	ew->pos.x = pl_w->pos.x + lw->hand_x - ax ;
	ew->pos.y = pl_w->pos.y + lw->hand_y - ay ;
	n = ew->timer1 ;
	for( i = 1 ; i < n - 1 ; i ++ ){
		next = ew->behind_w ;
		a = &pls0c_02_ofs[ew->dir * 4] ;
		ax = *a ++ * 1.6666666f ;
		ay = *a * 2.142857f ;
		a = &pls0c_02_ofs[next->dir * 4 + 2] ;
		bx = *a ++ * 1.6666666f ;
		by = *a * 2.142857f ;
		if( ew->disp_dir ){
			ax = -ax ;
			bx = -bx ;
		}
		next->pos.x = ew->pos.x + ax - bx ;
		next->pos.y = ew->pos.y + ay - by ;
		ew = next ;
	}
	next = ew->behind_w ;
	a = &pls0c_02_ofs[ew->dir * 4] ;
	ax = *a ++ * 1.6666666f ;
	ay = *a * 2.142857f ;
	a = &pls0c_02_end_ofs[( ( ( next->dir + 2 ) & 31 ) >> 2 ) * 2] ;
	bx = *a ++ * 1.6666666f ;
	by = *a * 2.142857f ;
	if( ew->disp_dir ){
		ax = -ax ;
		bx = -bx ;
	}
	next->pos.x = ew->pos.x + ax - bx ;
	next->pos.y = ew->pos.y + ay - by ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls0c_05.c" End of File	*/

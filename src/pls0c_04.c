/* Spider-Man's player effect 04: the web he swings on, a chain of up to twelve links from his
   hand to where it holds (ew->ef_w is the last link). It grows link by link, pulls him along and
   lets go. It turns with the same keys as the web throw, pls0c_02. */
#include "player.h"
#include "pl_char.h"
#include "o_sub1.h"
#include "pl0c_def.h"

extern Sint16 pls0c_02_ofs[] ;	/* pls0c_02.c: a web link's two ends, x and y, four words a direction */

/* pls0c_02's table of the links' ends, four to a link */

/* the web's turn, step by step: the angle (0 to 255), its direction, the frames it takes */
static Sint16 pls0c_04_key[2][4] = {
	0, 1, 2, 0,
	192, -1, 6, 0,
} ;
static Sint16 *pls0c_04_list[] = {
	pls0c_04_key[0], pls0c_04_key[1], 0,
} ;

/* where the web leaves his hand: forward and back (nothing reads it) */
static Sint16 pls0c_04_hand_ofs[4][2] = {
	6, 5,
	6, 5,
	-6, 5,
	-6, 5,
} ;

static void pls0c_04_main( PLAYER_WORK *ew );

/****************************************************************/
int pls0c_04_set( PLAYER_WORK *pl_w, int no, int type )
{
	PLAYER_WORK *ew ;
	PLAYER_WORK *first ;
	int cnt ;

	if( work_rest_cnt <= no ) return 0 ;
	if( no > 12 ) no = 12 ;
	for( cnt = 0 ; cnt < no ; cnt ++ ){
		if( ( ew = work_get( 0, W_PLS, LIST_END ) ) ){
			ew->ef_no = 0xc04 ;
			ew->sub_no = cnt ;
			ew->type = type ;
			ew->timer1 = no ;
			ew->work_func = pls0c_04_main ;
			ew->master_w = pl_w ;
			if( !cnt ) first = ew ;
		}
	}
	first->ef_w = ew ;
	return cnt ;
}

static void pls0c_04_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0c_04_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0c_04_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls0c_04_main( PLAYER_WORK *ew )
{
	static void ( *pls0c_04_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0c_04_init, pls0c_04_move, pls0c_04_free,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( !ew->sub_no ) pls0c_04_jmp[ew->routine_0]( ew, pl_w ) ;
}

static void pls0c_04_chain_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0c_04_show( PLAYER_WORK *ew, PLAYER_WORK *pl_w, int mode );
static void pls0c_04_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0c_04_char_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls0c_04_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pls0c_04_chain_set( ew, pl_w ) ;
	ew->routine_0 ++ ;
	PLS0C_02_KEY( ew ) = pls0c_04_list ;
	pls0c_02_key_next( ew ) ;
	pls0c_02_spin( ew, pl_w ) ;
	pls0c_04_show( ew, pl_w, 0 ) ;
	pls0c_04_pos_set( ew, pl_w ) ;
	pls0c_04_char_set( ew, pl_w ) ;
}

static void pls0c_04_aim( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0c_04_back_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void pls0c_04_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0C_LOCAL_W *lw = ( PL0C_LOCAL_W * )pl_w->tlocal ;

	if( pl_w->routine_1 || pl_w->act_st != N_SPECIAL || pl_w->char_no.b.grp != Char_Special ){
		ew->routine_0 = 2 ;
		pls0c_04_free( ew, pl_w ) ;
		return ;
	}
	if( vs_sys_work.slow_bit & ( 1 << vs_sys_work.slow_step ) ) return ;
	switch( ew->routine_1 ){
	case 0:
		pls0c_04_show( ew, pl_w, 0 ) ;
		pls0c_04_pos_set( ew, pl_w ) ;
		pls0c_04_char_set( ew, pl_w ) ;
		if( lw->swing_st ) ew->routine_1 ++ ;
		break ;
	case 1:
		pls0c_04_aim( ew, pl_w ) ;
		pls0c_04_char_set( ew, pl_w ) ;
		if( lw->swing_st != 1 ){
			ew->routine_1 ++ ;
			ew->sub_type = 16 ;
		}
		break ;
	case 2:
		if( -- ew->sub_type == 0 ){
			ew->routine_0 = 2 ;
			ew->disp_flag = 0 ;
			pls0c_04_show( ew, pl_w, 2 ) ;
			return ;
		}
		pls0c_04_show( ew, pl_w, 1 ) ;
		pls0c_02_spin( ew, pl_w ) ;
		pls0c_04_back_set( ew, pl_w ) ;
		pls0c_04_char_set( ew, pl_w ) ;
		break ;
	}
}

/*--------------------------------------------------------------*/
static void pls0c_04_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
/*			Every link takes the player's look					*/
/*==============================================================*/
static void pls0c_04_chain_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	int n = ew->timer1 ;
	int i ;

	for( i = 0 ; i < n ; i ++ ){
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
		ew->prio = pl_w->prio ;
		ew->sub_prio = 8 ;
		ew->pos = pl_w->pos ;
		ew = ew->behind_w ;
	}
}

/*==============================================================*/
/*			Mode 0: shows the links one a frame, and tells		*/
/*			the player when all are out; 1: blinks them; 2:		*/
/*			hides them											*/
/*==============================================================*/
static void pls0c_04_show( PLAYER_WORK *ew, PLAYER_WORK *pl_w, int mode )
{
	PL0C_LOCAL_W *lw = ( PL0C_LOCAL_W * )pl_w->tlocal ;
	int n = ew->timer1 ;
	int i ;

	for( i = 0 ; i < n ; i ++ ){
		if( mode == 0 ){
			if( !ew->disp_flag ) break ;
		}
		else if( mode == 1 ) ew->disp_flag = Sys->game_timer0 & 1 ;
		else ew->disp_flag = 0 ;
		ew = ew->behind_w ;
	}
	if( mode == 0 ){
		if( i < n ) ew->disp_flag = 1 ;
		else lw->swing_st = 1 ;
	}
}

/*==============================================================*/
/*			The animation of each link, and the tip				*/
/*==============================================================*/
static void pls0c_04_char_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0C_LOCAL_W *lw = ( PL0C_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *next ;
	int anim ;
	int no ;
	int n = ew->timer1 ;
	int i ;

	for( i = 1 ; i < n ; i ++ ){
		next = ew->behind_w ;
		if( !next->disp_flag ) break ;
		anim = pls0c_02_char_tbl[ew->sub_no] + 16 ;
		pl_char_set2( ew, Char_Effect, anim, ew->dir ) ;
		ew = next ;
	}
	no = 19 ;
	if( lw->swing_st >= 1 ) no ++ ;
	pl_char_set( ew, Char_Effect, no ) ;
}

/*==============================================================*/
/*			Each link's place, from the one before				*/
/*==============================================================*/
static void pls0c_04_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0C_LOCAL_W *lw = ( PL0C_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *next ;
	Sint16 *a ;
	float ax, ay, bx, by ;
	int n ;
	int i ;

	a = &pls0c_02_ofs[ew->dir * 4 + 2] ;
	ax = *a ++ * 1.6666666f ;
	ay = *a * 2.142857f ;
	if( ew->disp_dir ) ax = -ax ;
	ew->pos.x = pl_w->pos.x + lw->hand_x - ax ;
	ew->pos.y = pl_w->pos.y + lw->hand_y - ay ;
	n = ew->timer1 ;
	for( i = 1 ; i < n ; i ++ ){
		next = ew->behind_w ;
		if( !next->disp_flag ) break ;
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
}

/*==============================================================*/
/*			The same from the last link back to the first,		*/
/*			which pulls the player along						*/
/*==============================================================*/
static void pls0c_04_back_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0C_LOCAL_W *lw = ( PL0C_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *w ;
	Sint16 *a ;
	float ax, ay, bx, by ;
	int n ;
	int i ;

	ew = ew->ef_w ;
	by = 4 * 2.142857f ;
	w = ew->before_w ;
	a = &pls0c_02_ofs[w->dir * 4] ;
	bx = 0.0f ;
	ax = *a ++ * 1.6666666f ;
	ay = *a * 2.142857f ;
	if( ew->disp_dir ){
		ax = -ax ;
		bx = -bx ;
	}
	w->pos.x = ew->pos.x + bx - ax ;
	w->pos.y = ew->pos.y + by - ay ;
	n = ew->timer1 ;
	for( i = 1 ; i < n - 1 ; i ++ ){
		ew = w ;
		w = ew->before_w ;
		a = &pls0c_02_ofs[ew->dir * 4 + 2] ;
		bx = *a ++ * 1.6666666f ;
		by = *a * 2.142857f ;
		a = &pls0c_02_ofs[w->dir * 4] ;
		ax = *a ++ * 1.6666666f ;
		ay = *a * 2.142857f ;
		if( ew->disp_dir ){
			bx = -bx ;
			ax = -ax ;
		}
		w->pos.x = ew->pos.x + bx - ax ;
		w->pos.y = ew->pos.y + by - ay ;
	}
	if( lw->swing_st == 1 ){
		pl_w->pos.x = w->pos.x - lw->hand_x ;
		pl_w->pos.y = w->pos.y - lw->hand_y ;
	}
}

/*==============================================================*/
/*			The links point at his hand, from the last one		*/
/*			back, and he is put where the first one ends		*/
/*==============================================================*/
static void pls0c_04_aim( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0C_LOCAL_W *lw = ( PL0C_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *prev ;
	VEC3 hand ;
	Uint8 dir ;
	float ax, ay, bx, by ;
	int n ;
	int i ;

	ew = ew->ef_w ;
	hand.x = pl_w->pos.x + lw->hand_x ;
	hand.y = pl_w->pos.y + lw->hand_y ;
	dir = caldir_pos_256( &hand, &ew->pos ) ;
	if( pl_w->pl_dir ) dir = -dir ;
	ew->dir = ( Uint8 )( dir + 4 ) >> 3 ;
	n = ew->timer1 ;
	for( i = 1 ; i < n ; i ++ ){
		prev = ew->before_w ;
		dir = caldir_pos_256( &hand, &ew->pos ) ;
		if( pl_w->pl_dir ) dir = -dir ;
		prev->dir = ( Uint8 )( dir + 4 ) >> 3 ;
		if( i == 1 ){
			ax = 0.0f ;
			ay = 4 * 2.142857f ;
		}
		else{
			Sint16 *a = &pls0c_02_ofs[ew->dir * 4 + 2] ;

			ax = *a ++ * 1.6666666f ;
			ay = *a * 2.142857f ;
		}
		{
			Sint16 *a = &pls0c_02_ofs[prev->dir * 4] ;

			bx = *a ++ * 1.6666666f ;
			by = *a * 2.142857f ;
		}
		if( prev->disp_dir ){
			ax = -ax ;
			bx = -bx ;
		}
		prev->pos.x = ew->pos.x + ax - bx ;
		prev->pos.y = ew->pos.y + ay - by ;
		ew = prev ;
	}
	{
		Sint16 *a = &pls0c_02_ofs[ew->dir * 4 + 2] ;

		ax = *a ++ * 1.6666666f ;
		ay = *a * 2.142857f ;
	}
	if( ew->disp_dir ) ax = -ax ;
	pl_w->pos.x = ew->pos.x + ax - lw->hand_x ;
	pl_w->pos.y = ew->pos.y + ay - lw->hand_y ;
	pl_w->dir = ew->dir ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls0c_04.c" End of File	*/

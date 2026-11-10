/* Venom's player effect 05: the web of his swing (pl0e_web_swing), a chain of up to twelve links
   from his hand to where it holds (ew->ef_w is the last link), which drags him along until he lets
   go. Its links are the rope pieces of pls0e_03. */
#include "player.h"
#include "pl_char.h"
#include "o_sub1.h"
#include "pl0e_def.h"

/* pls0e_03's tables of the pieces' ends (four to a piece) and animations */
extern Sint16 pls0e_03_end_tbl[] ;
extern Uint8 pls0e_03_char_tbl[16] ;

/* the work's place in its table of turns, in the field the effect has no other use for */
#define PLS0E_05_KEY( ew )	( *( Sint16 *** )&( ew )->em_w )

/* the web's turn, step by step: the angle (0 to 255), its direction, the frames it takes */
static Sint16 pls0e_05_key[2][4] = {
	0, 1, 1, 0,
	208, -1, 16, 0,
} ;
static Sint16 *pls0e_05_list[] = {
	pls0e_05_key[0], pls0e_05_key[1], 0,
} ;

/* where the web leaves his hand: forward and back (nothing reads it) */
static Sint16 pls0e_05_hand_ofs[4][2] = {
	6, 5,
	6, 5,
	-6, 5,
	-6, 5,
} ;

static void pls0e_05_main( PLAYER_WORK *ew );
static int pls0e_05_turn( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static int pls0e_05_key_next( PLAYER_WORK *ew );

/****************************************************************/
int pls0e_05_set( PLAYER_WORK *pl_w, int no, int type )
{
	PLAYER_WORK *ew ;
	PLAYER_WORK *first ;
	int cnt ;

	if( work_rest_cnt <= no ) return 0 ;
	if( no > 12 ) no = 12 ;
	for( cnt = 0 ; cnt < no ; cnt ++ ){
		if( ( ew = work_get( 0, W_PLS, LIST_END ) ) ){
			ew->ef_no = 0xe05 ;
			ew->sub_no = cnt ;
			ew->type = type ;
			ew->timer1 = no ;
			ew->work_func = pls0e_05_main ;
			ew->master_w = pl_w ;
			if( !cnt ) first = ew ;
		}
	}
	first->ef_w = ew ;
	return cnt ;
}

static void pls0e_05_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0e_05_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0e_05_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls0e_05_main( PLAYER_WORK *ew )
{
	static void ( *pls0e_05_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0e_05_init, pls0e_05_move, pls0e_05_free,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( !ew->sub_no ) pls0e_05_jmp[ew->routine_0]( ew, pl_w ) ;
}

static void pls0e_05_chain_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0e_05_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0e_05_char_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls0e_05_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pls0e_05_chain_set( ew, pl_w ) ;
	ew->routine_0 ++ ;
	ew->routine_1 = 0 ;
	PLS0E_05_KEY( ew ) = pls0e_05_list ;
	pls0e_05_key_next( ew ) ;
	pls0e_05_turn( ew, pl_w ) ;
	pls0e_05_pos_set( ew, pl_w ) ;
	pls0e_05_char_set( ew, pl_w ) ;
}

static void pls0e_05_aim( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0e_05_back_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0e_05_blink( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void pls0e_05_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0E_LOCAL_W *lw = ( PL0E_LOCAL_W * )pl_w->tlocal ;

	if( pl_w->routine_1 || pl_w->act_st != N_AIR_DASH || pl_w->char_no.b.grp != Char_Extra || pl_w->char_no.b.no != 3 ){
		ew->routine_0 = 2 ;
		pls0e_05_free( ew, pl_w ) ;
		return ;
	}
	if( vs_sys_work.slow_bit & ( 1 << vs_sys_work.slow_step ) ) return ;
	if( !ew->routine_1 ){
		pls0e_05_aim( ew, pl_w ) ;
		pls0e_05_char_set( ew, pl_w ) ;
		if( lw->swing_st == -1 ){
			ew->routine_1 ++ ;
			ew->sub_type = 10 ;
		}
	}
	else if( -- ew->sub_type == 0 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		pls0e_05_free( ew, pl_w ) ;
	}
	else{
		pls0e_05_blink( ew, pl_w ) ;
		pls0e_05_turn( ew, pl_w ) ;
		pls0e_05_back_set( ew, pl_w ) ;
		pls0e_05_char_set( ew, pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls0e_05_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
static void pls0e_05_chain_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
		ew->prio = pl_w->prio ;
		ew->sub_prio = 8 ;
		ew->pos = pl_w->pos ;
		ew = ew->behind_w ;
	}
}

/*==============================================================*/
/*			The links blink while the web lets go				*/
/*==============================================================*/
static void pls0e_05_blink( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	int n = ew->timer1 ;
	int i ;

	for( i = 0 ; i < n ; i ++ ){
		ew->disp_flag = Sys->game_timer0 & 1 ;
		ew = ew->behind_w ;
	}
}

/*==============================================================*/
/*			The animation of each link, and the tip				*/
/*==============================================================*/
static void pls0e_05_char_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	int anim ;
	int n = ew->timer1 ;
	int i ;

	for( i = 0 ; i < n - 1 ; i ++ ){
		anim = pls0e_03_char_tbl[ew->sub_no] + 85 ;
		pl_char_set2( ew, Char_Effect, anim, ew->dir ) ;
		ew = ew->behind_w ;
	}
	pl_char_set( ew, Char_Effect, 43 ) ;
}

/*==============================================================*/
/*			Each link's place, from the one before				*/
/*==============================================================*/
static void pls0e_05_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0E_LOCAL_W *lw = ( PL0E_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *next ;
	Sint16 *a ;
	float ay, ax, by, bx ;
	int n ;
	int i ;

	a = &pls0e_03_end_tbl[ew->dir * 4 + 2] ;
	ax = *a ++ * 1.6666666f ;
	ay = *a * 2.142857f ;
	if( ew->disp_dir ) ax = -ax ;
	ew->pos.x = pl_w->pos.x + lw->hand_x - ax ;
	ew->pos.y = pl_w->pos.y + lw->hand_y - ay ;
	n = ew->timer1 ;
	for( i = 1 ; i < n ; i ++ ){
		next = ew->behind_w ;
		a = &pls0e_03_end_tbl[ew->dir * 4] ;
		ax = *a ++ * 1.6666666f ;
		ay = *a * 2.142857f ;
		a = &pls0e_03_end_tbl[next->dir * 4 + 2] ;
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
static void pls0e_05_back_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0E_LOCAL_W *lw = ( PL0E_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *w ;
	Sint16 *a ;
	float ay, ax, by, bx ;
	int n ;
	int i ;

	ew = ew->ef_w ;
	by = -2 * 2.142857f ;
	w = ew->before_w ;
	a = &pls0e_03_end_tbl[w->dir * 4] ;
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
		a = &pls0e_03_end_tbl[ew->dir * 4 + 2] ;
		bx = *a ++ * 1.6666666f ;
		by = *a * 2.142857f ;
		a = &pls0e_03_end_tbl[w->dir * 4] ;
		ax = *a ++ * 1.6666666f ;
		ay = *a * 2.142857f ;
		if( ew->disp_dir ){
			bx = -bx ;
			ax = -ax ;
		}
		w->pos.x = ew->pos.x + bx - ax ;
		w->pos.y = ew->pos.y + by - ay ;
	}
	if( !lw->swing_st ){
		pl_w->pos.x = w->pos.x - lw->hand_x ;
		pl_w->pos.y = w->pos.y - lw->hand_y ;
	}
}

/*==============================================================*/
/*			The links point at his hand, from the last one		*/
/*			back, and he is put where the first one ends		*/
/*==============================================================*/
static void pls0e_05_aim( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0E_LOCAL_W *lw = ( PL0E_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *w = ew->ef_w ;
	PLAYER_WORK *prev ;
	VEC3 hand ;
	Uint8 dir ;
	float ay, ax, by, bx ;
	int n ;
	int i ;

	hand.x = pl_w->pos.x + lw->hand_x ;
	hand.y = pl_w->pos.y + lw->hand_y ;
	dir = caldir_pos_256( &hand, &w->pos ) ;
	if( pl_w->pl_dir ) dir = -dir ;
	w->dir = ( Uint8 )( dir + 4 ) >> 3 ;
	n = w->timer1 ;
	for( i = 1 ; i < n ; i ++ ){
		prev = w->before_w ;
		dir = caldir_pos_256( &hand, &w->pos ) ;
		if( pl_w->pl_dir ) dir = -dir ;
		prev->dir = ( Uint8 )( dir + 4 ) >> 3 ;
		if( i == 1 ){
			ax = 0.0f ;
			ay = -2 * 2.142857f ;
		}
		else{
			Sint16 *a = &pls0e_03_end_tbl[w->dir * 4 + 2] ;

			ax = *a ++ * 1.6666666f ;
			ay = *a * 2.142857f ;
		}
		{
			Sint16 *a = &pls0e_03_end_tbl[prev->dir * 4] ;

			bx = *a ++ * 1.6666666f ;
			by = *a * 2.142857f ;
		}
		if( prev->disp_dir ){
			ax = -ax ;
			bx = -bx ;
		}
		prev->pos.x = w->pos.x + ax - bx ;
		prev->pos.y = w->pos.y + ay - by ;
		w = prev ;
	}
	{
		Sint16 *a = &pls0e_03_end_tbl[w->dir * 4 + 2] ;

		ax = *a ++ * 1.6666666f ;
		ay = *a * 2.142857f ;
	}
	if( w->disp_dir ) ax = -ax ;
	pl_w->pos.x = w->pos.x + ax - lw->hand_x ;
	pl_w->pos.y = w->pos.y + ay - lw->hand_y ;
	pl_w->dir = w->dir ;
}

/*==============================================================*/
/*			The links turn										*/
/*==============================================================*/
static int pls0e_05_turn( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	else ret = pls0e_05_key_next( ew ) ;
	return ret ;
}

/*==============================================================*/
/*			The next turn of the table: the rope's angle it		*/
/*			ends at, and the speed that gives each piece		*/
/*==============================================================*/
static int pls0e_05_key_next( PLAYER_WORK *ew )
{
	Sint16 **kp, *key ;
	PLAYER_WORK *w ;
	int i, n, ret ;

	kp = PLS0E_05_KEY( ew ) ;
	key = *kp ++ ;
	ret = 1 ;
	if( !key ){
		kp -= 2 ;
		key = *kp ++ ;
		ret = 0 ;
	}
	PLS0E_05_KEY( ew ) = kp ;
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
/* "pls0e_05.c" End of File	*/

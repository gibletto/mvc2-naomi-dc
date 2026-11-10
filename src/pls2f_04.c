/* Silver Samurai's player effect 04: the element sparks around him. Kind 0 follows the element he
   has on and, each beat, makes a kind 1 spark that stands off his place by a record of his
   animation's table (ef_pos_tbl) and blinks. */
#include "player.h"
#include "ef_char.h"
#include "pl2f_def.h"

/* the effect's own work */
typedef struct PLS2F_04_LOCAL_W {
	PLAYER_WORK	*owner;		/* 000: his work, the same as its master for kind 0 */
	Sint32	unk_04;			/* 004: kind 1: his animation's code, kept */
} PLS2F_04_LOCAL_W;
#define PLS2F_04_LW( w )	( ( PLS2F_04_LOCAL_W * )&( w )->obj_mtx.m[0][0] )

/* where a spark stands from him, in pixels: x then y, six to an animation */
typedef struct PLS2F_04_POS {
	Sint16	x;
	Sint16	y;
} PLS2F_04_POS;

static void pls2f_04_main( PLAYER_WORK *ew );

/****************************************************************/
/*			Sets the element sparks (their kind 0)				*/
/****************************************************************/
void *pls2f_04_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls2f_04_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x2f04 ;
		ew->type = 0 ;
		PLS2F_04_LW( ew )->owner = pl_w ;
	}
	return ew ;
}

/*==============================================================*/
static PLAYER_WORK *pls2f_04_mark_set( PLAYER_WORK *ef_w, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	Sint16 *pos ;
	Sint16 x, y ;
	Uint8 front ;

	if( ( ew = work_get( ef_w, W_PLS, LIST_BEHIND ) ) ){
		ew->work_func = pls2f_04_main ;
		ew->master_w = ef_w ;
		ew->ef_no = 0x2f04 ;
		ew->type = 1 ;
		ew->sub_type = 0 ;
		ew->dir = EF_WK( ef_w )->work[2] ;
		front = 0 ;
		if( pl_w->cg_atck_flag & CG_ATCK_ALL ) front = 1 ;
		ew->sub_no = front ;
		PLS2F_04_LW( ew )->owner = pl_w ;
		pos = ( Sint16 * )pl_w->ef_pos_tbl ;
		pos += ( pl_w->cg_rival * 6 + EF_WK( ef_w )->work[2] ) * 2 ;
		x = *pos ++ ;
		y = *pos ;
		if( PL2F_LW( pl_w )->elem == 3 ){
			x += 8 ;
			y += 8 ;
		}
		if( pl_w->disp_dir ) x = -x ;
		ew->pos.x = pl_w->pos.x - x * 1.6666666f ;
		ew->pos.y = pl_w->pos.y + DOT_Y( y ) ;
	}
	return ew ;
}

/****************************************************************/
/*			Nothing calls it, but it is global					*/
/****************************************************************/
void pls2f_04_step( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ){
		work_free( ew ) ;
	}
}

static void pls2f_04_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2f_04_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2f_04_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2f_04_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2f_04_main( PLAYER_WORK *ew )
{
	static void ( *pls2f_04_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2f_04_init, pls2f_04_move, pls2f_04_wait, pls2f_04_free,
	} ;

	pls2f_04_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/* the animation of a kind 1 spark, by the element */
static Uint8 pls2f_04_ef_tbl[4] = { 3, 1, 3, 5 } ;

/*==============================================================*/
static void pls2f_04_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->disp_flag = 0 ;
	ew->sub_prio = -1 ;
	if( !ew->type ){
		EF_WK( ew )->work[3] = PL2F_LW( pl_w )->elem ;
		ew->timer0 = 0 ;
		EF_WK( ew )->work[2] = 5 ;
	}
	else {
		PLS2F_04_LW( ew )->unk_04 = 0 ;
		ew->prio = 8 ;
		ew->disp_dir ^= 1 ;
		ef_char_set( ew, 27, pls2f_04_ef_tbl[EF_WK( pl_w )->work[3]] ) ;
	}
	pls2f_04_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls2f_04_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Sint16 tm_tbl[8] = { 0, 0, 8, 32, 8, 8, 8, 32 } ;
	PLAYER_WORK *own_w = PLS2F_04_LW( ew )->owner ;
	Uint8 no ;

	if( !own_w->disp_flag ){
		ew->disp_flag = 0 ;
		return ;
	}
	if( !ew->type ){
		if( !PL2F_LW( pl_w )->elem_flag ) goto free ;
		if( !PL2F_LW( pl_w )->elem_timer ) goto free ;
		if( EF_WK( ew )->work[3] != PL2F_LW( pl_w )->elem ) goto free ;
		if( pl_w->cg_rival <= 0 ) return ;
		ew->disp_dir = pl_w->disp_dir ;
		if( -- ew->timer0 >= 0 ) return ;
		if( ++ EF_WK( ew )->work[2] >= 6 ) EF_WK( ew )->work[2] = 0 ;
		no = 0 ;
		if( !pl_w->act_st ) no = 2 ;
		if( pl_w->act_st == N_WALK ) no = 4 ;
		if( pl_w->act_st == N_CROUCHING ) no = 6 ;
		if( EF_WK( ew )->work[2] == 5 ) no ++ ;
		ew->timer0 = tm_tbl[no] ;
		if( EF_WK( ew )->work[3] != 1 || EF_WK( ew )->work[2] == 1 || EF_WK( ew )->work[2] == 5 ){
			pls2f_04_mark_set( ew, pl_w ) ;
		}
		return ;
	}
	else {
		if( ef_char_move( ew ) >= 0 ){
			ew->disp_flag = ( Sys->game_timer0 + ew->dir ) & 1 ;
			if( ew->disp_flag ){
				ew->prio = 8 ;
				if( !ew->sub_no ) ew->prio = own_w->prio ;
				if( EF_WK( pl_w )->work[3] == 1 ){
					ew->disp_dir = Rnd() & 1 ;
					PLS2F_04_LW( ew )->unk_04 = own_w->cg_atck_flag ;
				}
			}
			return ;
		}
	}
free :
	pls2f_04_free( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls2f_04_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
}

/*--------------------------------------------------------------*/
static void pls2f_04_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls2f_04.c" End of File	*/

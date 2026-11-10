/* Morrigan's effect 00 (character 03's pls00): the pieces of her Finishing Shower. Fifteen flashes
   (type 0) fly out to a place by her and follow her animation; two more (type 1) turn round her
   and put out its shots (shl03_01) as their animation says. */
#include "player.h"
#include "ef_char.h"
#include "pl03_def.h"

extern int abs( int x ) ;

/* the effect's own work, from obj_mtx */
typedef struct PLS03_00_LOCAL_W {
	Float	x;		/* 000: where it is from Morrigan */
	Float	y;		/* 004 */
	Float	to_x;		/* 008: where it flies to */
	Float	to_y;		/* 00c */
	Uint16	char_no;	/* 010: Morrigan's animation when it was put out */
	Uint8	unk_12[2];
	Sint32	ang;		/* 014: the way it flies */
	Sint32	to_ang;		/* 018: the way it turns to */
	Sint8	pow;		/* 01c: its speed, as a power of two */
} PLS03_00_LOCAL_W;
#define PLS03_00_LW( w )	( ( PLS03_00_LOCAL_W * )&( w )->obj_mtx )

static void pls03_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls03_00_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	int i ;

	for( i = 0 ; i < 15 ; i ++ ){
		if( ( ew = work_get( 0, W_PLS_ST, LIST_END ) ) ){
			ew->work_func = pls03_00_main ;
			ew->master_w = pl_w ;
			ew->pl_type = pl_w->pl_type ;
			ew->type = 0 ;
			ew->sub_type = i ;
			ew->ef_no = 0x300 ;
		}
	}
	return ew ;
}

/****************************************************************/
PLAYER_WORK *pls03_00_set2( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	int i ;

	for( i = 0 ; i < 2 ; i ++ ){
		if( ( ew = work_get( 0, W_PLS_ST, LIST_END ) ) ){
			ew->work_func = pls03_00_main ;
			ew->master_w = pl_w ;
			ew->pl_type = pl_w->pl_type ;
			ew->type = 1 ;
			ew->sub_type = i ;
			ew->ef_no = 0x300 ;
		}
	}
	return ew ;
}

static void pls03_00_move_a( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_00_move_b( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls03_00_main( PLAYER_WORK *ew )
{
	static void ( *pls03_00_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls03_00_move_a, pls03_00_move_b,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->prio = pl_w->prio ;
	pls03_00_jmp[ew->type]( ew, pl_w ) ;
}

static void pls03_00_a_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_00_a_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_00_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls03_00_move_a( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls03_00_a_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls03_00_a_init, pls03_00_a_move, pls03_00_end, pls03_00_free,
	} ;

	pls03_00_a_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls03_00_a_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Sint16 pls03_00_to[15][2] = {
	{ 0x1000, 0x2000 }, { 0x2000, 0x2000 }, { 0x3000, 0x2000 }, { 0x2000, 0x3000 }, { 0x3000, 0x3000 },
	{ 0x4000, 0x3000 }, { 0x0, 0x4000 }, { 0x1000, 0x4000 }, { 0x2000, 0x4000 }, { -0x4000, 0x3000 },
	{ -0x5000, 0x3000 }, { -0x1000, 0x7800 }, { 0x1000, 0x7800 }, { -0x1000, 0x1800 },
	{ 0x1000, 0x1800 },
	} ;
	static Sint16 pls03_00_spd[15][3] = {
	{ 0x0, 0x1000, 0x4000 }, { 0x1000, 0x1000, 0x4000 }, { 0x2000, 0x1000, 0x4000 },
	{ 0x1000, 0x2000, 0x4000 }, { 0x2000, 0x2000, 0x4000 }, { 0x3000, 0x2000, 0x4000 },
	{ -0x1000, 0x3000, 0x4000 }, { 0x0, 0x3000, 0x4000 }, { 0x1000, 0x3000, 0x4000 },
	{ -0x3000, 0x2000, 0x4000 }, { -0x4000, 0x2000, 0x4000 }, { -0x1000, 0x6800, 0x4000 },
	{ 0x1000, 0x6800, 0x4000 }, { -0x1000, 0x1000, -0x4000 }, { 0x1000, 0x1000, -0x4000 },
	} ;
	static Sint32 pls03_00_col[] = { 0x32, 0x35, 0x38, 0x3b, 0x3e, 0x41 } ;
	PLS03_00_LOCAL_W *lw = PLS03_00_LW( ew ) ;

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
	lw->char_no = pl_w->char_no.w ;
	lw->pow = 3 ;
	ew->timer0 = 4 ;
	ew->timer1 = 8 ;
	ew->sub_prio = -2 ;
	ew->char_no.b.no = 9 ;
	ef_char_set( ew, 27, ew->char_no.b.no ) ;
	if( !ew->sub_type ) ef_color_set( pl_w, 2, pls03_00_col[pl_w->pl_color], 1 ) ;
	ew->pos.x = pl_w->pos.x ;
	lw->x = 0.0f ;
	ew->pos.y = pl_w->pos.y ;
	lw->y = 0.0f ;
	ew->acc.y = ew->acc.x = 0.0f ;
	lw->to_x = pls03_00_to[ew->sub_type][0] * 1.6666666f / 256.0f ;
	lw->to_y = pls03_00_to[ew->sub_type][1] * 2.142857f / 256.0f ;
	ew->spd.x = pls03_00_spd[ew->sub_type][0] * 1.6666666f / 256.0f ;
	ew->spd.y = pls03_00_spd[ew->sub_type][1] * 2.142857f / 256.0f ;
	lw->ang = pls03_00_spd[ew->sub_type][2] ;
	if( !ew->disp_dir ){
		lw->to_x = -lw->to_x ;
		ew->spd.x = -ew->spd.x ;
	}
	lw->x += ew->spd.x ;
	lw->y = lw->y + ew->spd.y ;
	ew->pos.x = lw->x + pl_w->pos.x ;
	ew->pos.y = lw->y + pl_w->pos.y ;
}

/*==============================================================*/
/*			It turns the way of its place, by the short way		*/
/*			round												*/
/*==============================================================*/
static void pls03_00_turn( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS03_00_LOCAL_W *lw = PLS03_00_LW( ew ) ;
	Float dx = pl_w->pos.x + lw->to_x - ew->pos.x ;
	Float dy = pl_w->pos.y + lw->to_y - ew->pos.y ;

	lw->to_ang = nlArcTan2( dy, dx ) ;
	if( abs( lw->to_ang - lw->ang ) >= 0x400 ){
		if( lw->to_ang - lw->ang > 0 ) lw->ang -= 0x400 ;
		else lw->ang += 0x400 ;
	}
}

/*==============================================================*/
static void pls03_00_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS03_00_LOCAL_W *lw = PLS03_00_LW( ew ) ;

	ew->spd.x = nlCos( lw->ang ) * nlPow( 2.0f, lw->pow ) / 2.0f ;
	ew->spd.y = nlSin( lw->ang ) * nlPow( 2.0f, lw->pow ) / 2.0f ;
	lw->x += ew->spd.x ;
	lw->y = lw->y + ew->spd.y ;
	ew->pos.x = lw->x + pl_w->pos.x ;
	ew->pos.y = lw->y + pl_w->pos.y ;
}

/*==============================================================*/
static void pls03_00_a_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Sint16 pls03_00_time[] = { 0x10, 0x10, 0x0c, 0x0a } ;
	PLS03_00_LOCAL_W *lw = PLS03_00_LW( ew ) ;

	if( lw->char_no != pl_w->char_no.w ){
		ew->routine_1 ++ ;
		lw->pow = 0 ;
		pls03_00_turn( ew, pl_w ) ;
		pls03_00_fly( ew, pl_w ) ;
		ef_char_move( ew ) ;
		return ;
	}
	if( !-- ew->timer0 ){
		if( !-- ew->timer1 ){
			ew->timer1 = pls03_00_time[lw->pow] ;
			if( lw->pow ){
				lw->pow -- ;
				ew->timer0 = 4 ;
			}
			else ew->timer0 = 1 ;
		}
		else ew->timer0 = 1 ;
		pls03_00_turn( ew, pl_w ) ;
	}
	pls03_00_fly( ew, pl_w ) ;
	ef_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls03_00_a_follow( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Sint32 pls03_00_col2[] = { 0x30, 0x33, 0x36, 0x39, 0x3c, 0x3f } ;

	if( pl_w->cg_flag1 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ef_color_set( pl_w, 2, pls03_00_col2[pl_w->pl_color] + pl_w->cg_prm, 1 ) ;
	pls03_00_turn( ew, pl_w ) ;
	pls03_00_fly( ew, pl_w ) ;
	ef_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls03_00_a_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls03_00_a_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls03_00_a_fly, pls03_00_a_follow,
	} ;

	if( pl_w->routine_1 || pl_w->act_st != N_SUPER ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	pls03_00_a_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

static void pls03_00_b_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_00_b_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls03_00_move_b( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls03_00_b_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls03_00_b_init, pls03_00_b_move, pls03_00_end, pls03_00_free,
	} ;

	pls03_00_b_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls03_00_b_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS03_00_LOCAL_W *lw = PLS03_00_LW( ew ) ;

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
	lw->char_no = pl_w->char_no.w ;
	lw->pow = 8 ;
	ew->timer0 = 180 ;
	ew->timer1 = 1 ;
	ew->sub_prio = -4 ;
	ew->char_no.b.no = 10 ;
	ef_char_set( ew, 27, ew->char_no.b.no ) ;
	if( !ew->sub_type ) lw->ang = 0x4000 ;
	else lw->ang = 0xc000 ;
	if( ew->disp_dir ) lw->to_ang = -0x400 ;
	else lw->to_ang = 0x400 ;
	ew->spd.x = nlCos( lw->ang ) * nlPow( 2.0f, lw->pow ) / 2.0f ;
	ew->spd.y = nlSin( lw->ang ) * nlPow( 2.0f, lw->pow ) / 2.0f ;
	ew->pos.x = pl_w->pos.x + ew->spd.x ;
	ew->pos.y = pl_w->pos.y + ew->spd.y + DOT_Y( 64.0f ) ;
}

/*==============================================================*/
static void pls03_00_b_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS03_00_LOCAL_W *lw = PLS03_00_LW( ew ) ;

	if( lw->char_no != pl_w->char_no.w ){
		lw->char_no = pl_w->char_no.w ;
		ew->routine_1 ++ ;
		ew->timer0 = 1 ;
		return ;
	}
	if( pl_w->routine_2 <= 2 && ew->cg_prm == 2 ) ef_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls03_00_b_turn( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS03_00_LOCAL_W *lw = PLS03_00_LW( ew ) ;
	PL03_LOCAL_W *lw03 = ( PL03_LOCAL_W * )pl_w->tlocal ;

	if( lw->char_no != pl_w->char_no.w ){
		ew->routine_1 ++ ;
		ew->timer0 = 8 ;
		return ;
	}
	if( ew->cg_type ){
		ew->cg_type = 0 ;
		shl03_01_ef_set( ew ) ;
	}
	if( !( vs_sys_work.slow_bit & 1 << vs_sys_work.slow_step ) ){
		if( !vs_sys_work.game_stop ){
			ef_char_move( ew ) ;
			if( ew->timer1 ){
				ew->timer1 -- ;
				return ;
			}
			if( !ew->cg_prm ){
				ew->timer1 = 1 ;
				lw->ang += lw->to_ang ;
				if( lw03->gacha_flag ) lw->ang += lw->to_ang ;
			}
		}
		ew->spd.x = nlCos( lw->ang ) * nlPow( 2.0f, lw->pow ) / 2.0f ;
		ew->spd.y = nlSin( lw->ang ) * nlPow( 2.0f, lw->pow ) / 2.0f ;
		ew->pos.x = pl_w->pos.x + ew->spd.x ;
		ew->pos.y = pl_w->pos.y + ew->spd.y + DOT_Y( 64.0f ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls03_00_b_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !( ew->timer0 -- ) ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void pls03_00_b_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls03_00_b_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls03_00_b_wait, pls03_00_b_turn, pls03_00_b_end,
	} ;

	if( pl_w->routine_1 || ( pl_w->act_st != N_SUPER && !vs_sys_work.game_stop ) ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	pls03_00_b_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls03_00_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls03_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls03_00.c" End of File	*/

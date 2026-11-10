/* Anakaris's effect 00 (character 04's player effect 00): the small pieces of his moves,
   by type: the rings that circle a Royal Judgement or Pharaoh Magic shot, Mummy Drop's two
   pieces in front of him and behind, and those that show the frames his animation asks for. */
#include "player.h"
#include "ef_char.h"
#include "pl_char.h"

/* where the ring sits each step: x, y, the draw priority and the animation frame */
static Sint16 pls04_00_judgement_ring_tbl[] = {
	-27, 1, 12, 6, -24, 1, 12, 22,
	-22, 2, 12, 7, -18, 2, 12, 23,
	-14, 3, 12, 8, -9, 4, 12, 24,
	-4, 4, 12, 9, 0, 4, 12, 25,
	4, 4, 12, 10, 9, 4, 12, 26,
	14, 3, 12, 11, 18, 2, 12, 27,
	22, 2, 12, 13, 24, 1, 12, 29,
	27, 1, 12, 14, 28, 0, 12, 30,
	27, -1, 10, 15, 25, -2, 10, 31,
	23, -3, 10, 0, 19, -3, 10, 16,
	15, -4, 10, 0, 10, -5, 10, 16,
	5, -5, 10, 1, 0, -5, 10, 17,
	-5, -5, 10, 2, -10, -5, 10, 18,
	-15, -4, 10, 3, -19, -3, 10, 19,
	-23, -3, 10, 4, -25, -2, 10, 20,
	-27, -1, 10, 5, -28, 0, 10, 21,
} ;

static Sint16 pls04_00_magic_ring_tbl[] = {
	-16, -27, 12, 0, -17, -24, 12, 0,
	-17, -22, 12, 1, -16, -18, 12, 1,
	-14, -14, 12, 1, -13, -9, 12, 2,
	-10, -4, 12, 2, -8, 0, 12, 2,
	-4, 4, 12, 3, -1, 9, 12, 3,
	2, 14, 12, 3, 6, 18, 12, 4,
	9, 22, 12, 4, 11, 24, 12, 4,
	14, 27, 12, 5, 16, 28, 12, 5,
	16, 27, 10, 0, 17, 25, 10, 0,
	17, 23, 10, 1, 16, 19, 10, 1,
	14, 15, 10, 1, 13, 10, 10, 2,
	10, 5, 10, 2, 8, 0, 10, 2,
	4, -5, 10, 3, 1, -10, 10, 3,
	-2, -15, 10, 3, -6, -19, 10, 4,
	-9, -23, 10, 4, -11, -25, 10, 4,
	-14, -27, 10, 5, -16, -28, 10, 5,
} ;

static void pls04_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls04_00_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 dir )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls04_00_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
		ew->dir = dir ;
		ew->ef_no = 0x400 ;
	}
	return ew ;
}

/****************************************************************/
PLAYER_WORK *pls04_00_set2( PLAYER_WORK *pl_w, Uint8 type, Uint8 dir )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls04_00_main ;
		ew->master_w = pl_w->master_w ;
		ew->ef_w = pl_w ;
		ew->type = type ;
		ew->dir = dir ;
		ew->ef_no = 0x400 ;
	}
	return ew ;
}

static void pls04_00_init( PLAYER_WORK *ew );
static void pls04_00_move( PLAYER_WORK *ew );
static void pls04_00_end( PLAYER_WORK *ew );
static void pls04_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls04_00_main( PLAYER_WORK *ew )
{
	static void ( *pls04_00_jmp[] )( PLAYER_WORK *ew ) = {
		pls04_00_init, pls04_00_move, pls04_00_end, pls04_00_free,
	} ;

	pls04_00_jmp[ew->routine_0]( ew ) ;
}

static void pls04_00_judgement_init( PLAYER_WORK *ew );
static void pls04_00_mummy_init( PLAYER_WORK *ew );
static void pls04_00_magic_init( PLAYER_WORK *ew );
static void pls04_00_frame_init( PLAYER_WORK *ew );
static void pls04_00_catch_init( PLAYER_WORK *ew );

/*==============================================================*/
static void pls04_00_init( PLAYER_WORK *ew )
{
	static void ( *pls04_00_init_jmp[] )( PLAYER_WORK *ew ) = {
		pls04_00_judgement_init, pls04_00_mummy_init, pls04_00_magic_init, pls04_00_frame_init, pls04_00_catch_init,
	} ;
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
	ew->disp_flag = 1 ;
	pl_w = ew->master_w ;
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
	pls04_00_init_jmp[ew->type]( ew ) ;
	pls04_00_move( ew ) ;
}

/*==============================================================*/
/*			Up in front of Anakaris, unseen until it flies		*/
/*==============================================================*/
static void pls04_00_judgement_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->pos.x = DOT_X( 64.0f ) ;
	ew->pos.y = DOT_Y( 96.0f ) ;
	if( !pl_w->disp_dir ) ew->pos.x = -ew->pos.x ;
	ew->pos.x += pl_w->pos.x ;
	ew->pos.y += pl_w->pos.y ;
	ew->prio = 10 ;
	ew->disp_flag = 0 ;
}

/*==============================================================*/
static void pls04_00_mummy_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->pos = pl_w->pos ;
	ew->pos.x += pl_w->spd.z ;
	if( !ew->dir ){
		ew->prio = 10 ;
		ef_char_set( ew, 27, ew->atck_level + 6 ) ;
	}
	else{
		ew->prio = 12 ;
		ef_char_set( ew, 27, ew->atck_level + 9 ) ;
	}
}

/*==============================================================*/
static void pls04_00_magic_init( PLAYER_WORK *ew )
{
}

/*==============================================================*/
static void pls04_00_frame_init( PLAYER_WORK *ew )
{
	ew->prio = 13 ;
	ew->timer0 = 0 ;
	ew->disp_flag = 0 ;
}

/*==============================================================*/
static void pls04_00_catch_init( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	ew->prio = 11 ;
}

static void pls04_00_judgement_move( PLAYER_WORK *ew );
static void pls04_00_mummy_move( PLAYER_WORK *ew );
static void pls04_00_magic_move( PLAYER_WORK *ew );
static void pls04_00_frame_move( PLAYER_WORK *ew );
static void pls04_00_catch_move( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls04_00_move( register PLAYER_WORK *ew )
{
	static void ( *pls04_00_move_jmp[] )( PLAYER_WORK *ew ) = {
		pls04_00_judgement_move, pls04_00_mummy_move, pls04_00_magic_move, pls04_00_frame_move, pls04_00_catch_move,
	} ;

	pls04_00_move_jmp[ew->type]( ew ) ;
}

static void pls04_00_judgement_wait( PLAYER_WORK *ew );
static void pls04_00_judgement_fly( PLAYER_WORK *ew );
static void pls04_00_judgement_ring( PLAYER_WORK *ew );
static void pls04_00_judgement_end( PLAYER_WORK *ew );

/*==============================================================*/
static void pls04_00_judgement_move( PLAYER_WORK *ew )
{
	static void ( *pls04_00_judgement_move_jmp[] )( PLAYER_WORK *ew ) = {
		pls04_00_judgement_wait, pls04_00_judgement_fly, pls04_00_judgement_ring, pls04_00_judgement_end,
	} ;

	pls04_00_judgement_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
/*			Waits its turn, then flies toward the shot for		*/
/*			6 frames											*/
/*==============================================================*/
static void pls04_00_judgement_wait( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->ef_w ;

	if( pl_w->routine_0 == 2 || pl_w->routine_1 ){
		ew->routine_1 = 3 ;
		ef_char_set( ew, 27, 4 ) ;
		return ;
	}
	if( -- ew->timer0 ) return ;
	ew->routine_1 ++ ;
	ew->disp_flag = 1 ;
	ew->timer0 = 6 ;
	ew->acc.x = 0 ;
	ew->acc.y = 0 ;
	ew->spd.x = ( pl_w->pos.x - ew->pos.x ) / 8.0f ;
	ew->spd.y = ( pl_w->pos.y - ew->pos.y ) / 8.0f ;
	ef_char_set( ew, 27, 3 ) ;
}

/*--------------------------------------------------------------*/
static void pls04_00_judgement_fly( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->ef_w ;

	if( pl_w->routine_0 == 2 || pl_w->routine_1 ){
		ew->routine_1 = 3 ;
		ef_char_set( ew, 27, 4 ) ;
		return ;
	}
	if( -- ew->timer0 ){
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		ef_char_move( ew ) ;
		return ;
	}
	ew->routine_1 ++ ;
	ew->dir = 96 ;
}

/*==============================================================*/
/*			Circles the shot by the table						*/
/*==============================================================*/
static void pls04_00_judgement_ring( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->ef_w ;

	if( pl_w->routine_0 == 2 || pl_w->routine_1 ){
		ew->routine_1 = 3 ;
		ef_char_set( ew, 27, 4 ) ;
		return ;
	}
	ew->dir = ( ew->dir + 4 ) & 124 ;
	ew->pos.x = DOT_X( pls04_00_judgement_ring_tbl[ew->dir] ) ;
	ew->pos.y = DOT_Y( pls04_00_judgement_ring_tbl[ew->dir + 1] ) ;
	ew->prio = pls04_00_judgement_ring_tbl[ew->dir + 2] ;
	if( pl_w->disp_dir ) ew->pos.x = -ew->pos.x ;
	ew->pos.x += pl_w->pos.x ;
	ew->pos.y += pl_w->pos.y ;
	ef_char_set2( ew, 27, 5, pls04_00_judgement_ring_tbl[ew->dir + 3] ) ;
}

/*--------------------------------------------------------------*/
static void pls04_00_judgement_end( PLAYER_WORK *ew )
{
	if( ef_char_move( ew ) < 0 ) ew->routine_0 = 2 ;
}

static void pls04_00_mummy_set( PLAYER_WORK *ew );
static void pls04_00_mummy_nop0( PLAYER_WORK *ew );
static void pls04_00_mummy_nop1( PLAYER_WORK *ew );
static void pls04_00_mummy_nop2( PLAYER_WORK *ew );

/*==============================================================*/
/*			Follows him while his animation asks for it			*/
/*			(code 36)											*/
/*==============================================================*/
static void pls04_00_mummy_move( PLAYER_WORK *ew )
{
	static void ( *pls04_00_mummy_move_jmp[] )( PLAYER_WORK *ew ) = {
		pls04_00_mummy_set, pls04_00_mummy_nop0, pls04_00_mummy_nop1, pls04_00_mummy_nop2,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( HI_BYTE( pl_w->cg_rival ) != 36 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
	}
	else{
		ef_char_move( ew ) ;
		ew->pos = pl_w->pos ;
		ew->pos.x += pl_w->spd.z ;
		pls04_00_mummy_move_jmp[ew->routine_1]( ew ) ;
	}
}

/*==============================================================*/
static void pls04_00_mummy_set( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->dir == 1 ){
		ew->routine_1 ++ ;
		if( !ew->dir ) ef_char_set( ew, 27, 8 ) ;
		else ef_char_set( ew, 27, 11 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls04_00_mummy_nop0( PLAYER_WORK *ew )
{
}

/*--------------------------------------------------------------*/
static void pls04_00_mummy_nop1( PLAYER_WORK *ew )
{
}

/*--------------------------------------------------------------*/
static void pls04_00_mummy_nop2( PLAYER_WORK *ew )
{
}

static void pls04_00_magic_ring( PLAYER_WORK *ew );
static void pls04_00_magic_end( PLAYER_WORK *ew );

/*==============================================================*/
static void pls04_00_magic_move( PLAYER_WORK *ew )
{
	static void ( *pls04_00_magic_move_jmp[] )( PLAYER_WORK *ew ) = {
		pls04_00_magic_ring, pls04_00_magic_end,
	} ;

	pls04_00_magic_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
/*			Circles a Pharaoh Magic shot by its own table		*/
/*==============================================================*/
static void pls04_00_magic_ring( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->ef_w ;

	if( pl_w->routine_0 == 2 || pl_w->routine_1 ){
		ew->routine_1 = 1 ;
		ef_char_set( ew, 27, 4 ) ;
		return ;
	}
	ew->dir = ( ew->dir + 4 ) & 124 ;
	ew->pos.x = DOT_X( pls04_00_magic_ring_tbl[ew->dir] ) ;
	ew->pos.y = DOT_Y( pls04_00_magic_ring_tbl[ew->dir + 1] ) ;
	ew->prio = pls04_00_magic_ring_tbl[ew->dir + 2] ;
	if( pl_w->disp_dir ) ew->pos.x = -ew->pos.x ;
	ew->pos.x += pl_w->pos.x ;
	ew->pos.y += pl_w->pos.y ;
	ef_char_set2( ew, 27, 5, pls04_00_magic_ring_tbl[ew->dir + 3] ) ;
}

/*--------------------------------------------------------------*/
static void pls04_00_magic_end( PLAYER_WORK *ew )
{
	if( ef_char_move( ew ) < 0 ) ew->routine_0 = 2 ;
}

/*==============================================================*/
/*			Shows the animation frame his own animation asks	*/
/*			for													*/
/*==============================================================*/
static void pls04_00_frame_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Sint8 no = HI_BYTE( pl_w->cg_rival ) ;

	if( no ){
		ew->disp_flag = 1 ;
		ew->pos = pl_w->pos ;
		ew->disp_dir = pl_w->disp_dir ;
		ew->alpha = pl_w->alpha ;
		ew->scale.x = pl_w->scale.x ;
		ew->scale.y = pl_w->scale.y ;
		if( no < 0 ){
			no &= 127 ;
			ew->pos.y = pl_w->ground_y ;
		}
		if( no != ew->timer0 ){
			ef_char_set2( ew, 27, ew->dir + 1, no ) ;
			ew->timer0 = no ;
		}
	}
	else{
		ew->disp_flag = 0 ;
		ew->timer0 = 0 ;
	}
}

/*==============================================================*/
static void pls04_00_catch_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Uint8 no ;

	if( !LO_BYTE( pl_w->cg_rival ) ){
		ew->routine_0 = 2 ;
		return ;
	}
	ew->disp_flag = 0 ;
	ew->pos = pl_w->pos ;
	if( ( no = pl_w->cg_type >> 6 ) ){
		ew->disp_flag = 1 ;
		ef_char_set( ew, 27, ( no & 3 ) + 11 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls04_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls04_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls04_00.c" End of File	*/

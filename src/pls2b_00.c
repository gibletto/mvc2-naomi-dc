/* Sabretooth's player effect 00: the small piece of a claw swing (shl2b_00) that flies off from
   it, by the animation the claw is showing (0 to 2). */
#include "player.h"
#include "pl_char.h"
#include "pl2b_def.h"

/* where it starts from the claw, by the claw's animation: x and y in pixels */
static Sint16 pls2b_00_pos_tbl[] = {
	-21, 84, -12, 93, -22, 76,
} ;

/* its speed and its pull, x then y, in 16.16 fixed point */
static Sint32 pls2b_00_spd_tbl[4] = {
	0x30000, 0, 0x80000, -0x10000,
} ;

static void pls2b_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls2b_00_set( PLAYER_WORK *ef_w, PLAYER_WORK *pl_w, int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_END ) ) ){
		ew->ef_no = 0x2b00 ;
		ew->type = no ;
		ew->work_func = pls2b_00_main ;
		ew->ef_w = ef_w ;
		ew->master_w = pl_w ;
	}
	return ew ;
}

static void pls2b_00_init( PLAYER_WORK *ew );
static void pls2b_00_move( PLAYER_WORK *ew );
static void pls2b_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls2b_00_main( PLAYER_WORK *ew )
{
	static void ( *pls2b_00_jmp[] )( PLAYER_WORK *ew ) = {
		pls2b_00_init, pls2b_00_move, pls2b_00_free,
	} ;

	pls2b_00_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls2b_00_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *ef_w = ew->ef_w ;
	int x, y ;
	int i ;
	Sint32 *spd ;

	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ef_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ef_w->pl_no ;
	ew->pl_type = ef_w->pl_type ;
	ew->scale.x = ef_w->scale.x ;
	ew->scale.y = ef_w->scale.y ;
	ew->atck_level = ef_w->atck_level ;
	ew->work_no = ef_w->work_no ;
	ew->col_cnt = ef_w->col_cnt ;
	ew->scale = ef_w->scale ;
	ew->prio = ef_w->prio ;
	ew->prio = 0 ;
	ew->disp_dir = ef_w->disp_dir ;
	ew->size_top = 16 ;
	ew->size_bottom = 16 ;
	ew->size_front = 16 ;
	ew->size_back = 16 ;
	ew->pos = ef_w->pos ;
	i = ew->type * 2 ;
	x = pls2b_00_pos_tbl[i] ;
	y = pls2b_00_pos_tbl[i + 1] ;
	spd = pls2b_00_spd_tbl ;
	ew->spd.x = *spd ++ * 1.6666666f / 65536.0f ;
	ew->acc.x = *spd ++ * 1.6666666f / 65536.0f ;
	ew->spd.y = *spd ++ * 2.142857f / 65536.0f ;
	ew->acc.y = *spd * 2.142857f / 65536.0f ;
	if( ew->disp_dir ){
		x = -x ;
		ew->spd.x = -ew->spd.x ;
		ew->acc.x = -ew->acc.x ;
	}
	ew->pos.x += DOT_X( x ) ;
	ew->pos.y += DOT_Y( y ) ;
	pl_char_set( ew, Char_Effect, 10 ) ;
}

/*--------------------------------------------------------------*/
static void pls2b_00_move( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	if( !scr_in_ck( ew ) ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void pls2b_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls2b_00.c" End of File	*/

/* Venom's player effect 00: the three marks of his Venom Fang, each at its own place beside him and
   with a picture by the code his animation holds (type 0), and a mark on the ground at his place,
   run once, where a leap of his starts or lands (type 1). */
#include "player.h"
#include "pl_char.h"

/* the animation each code shows, four marks a code */
static Uint8 pls0e_00_char_tbl[6 * 4] = {
	0, 0, 0, 0,  6, 7, 8, 0,  6, 7, 8, 0,
	0, 1, 2, 0,  3, 4, 2, 0,  3, 4, 5, 0,
} ;

/* where each mark goes from him, an x and a y for each of four marks, by the code less 1 */
static Sint16 pls0e_00_ofs_tbl[5][4 * 2] = {
	{ -38, 89,  -4, 64,  11, 18,  -32, 0 },
	{ -30, 84,  9, 62,  26, 32,  -32, 0 },
	{ -18, 93,  26, 84,  70, 67,  -32, 0 },
	{ -40, 96,  17, 102,  73, 94,  -32, 0 },
	{ -36, 103,  6, 132,  50, 148,  -32, 0 },
} ;

static void pls0e_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls0e_00_set( PLAYER_WORK *pl_w, int type, int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_END ) ) ){
		ew->ef_no = 0xe00 ;
		ew->type = type ;
		ew->sub_type = no ;
		ew->work_func = pls0e_00_main ;
		ew->master_w = pl_w ;
		ew->timer0 = pl_w->char_no.b.grp ;
	}
	return ew ;
}

static void pls0e_00_fang( PLAYER_WORK *ew );
static void pls0e_00_ground( PLAYER_WORK *ew );

/*==============================================================*/
static void pls0e_00_main( PLAYER_WORK *ew )
{
	static void ( *pls0e_00_jmp[] )( PLAYER_WORK *ew ) = {
		pls0e_00_fang, pls0e_00_ground,
	} ;

	pls0e_00_jmp[ew->type]( ew ) ;
}

static void pls0e_00_fang_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0e_00_fang_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0e_00_fang_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls0e_00_fang( PLAYER_WORK *ew )
{
	static void ( *pls0e_00_fang_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0e_00_fang_init, pls0e_00_fang_move, pls0e_00_fang_free,
	} ;

	pls0e_00_fang_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void pls0e_00_fang_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->dir = 0xff ;
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
	ew->acc.x = 0 ;
	ew->acc.y = 0 ;
	ew->prio = pl_w->prio ;
	ew->sub_prio = -8 ;
	if( !ew->sub_type ){
		pls0e_00_set( pl_w, 0, 1 ) ;
		pls0e_00_set( pl_w, 0, 2 ) ;
	}
	pls0e_00_fang_move( ew, pl_w ) ;
}

/*==============================================================*/
/*			Follow him while his animation holds a code,		*/
/*			showing the mark the code and its number pick		*/
/*==============================================================*/
static void pls0e_00_fang_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	int code, i ;
	Sint16 *ofs ;

	if( pl_w->routine_1 != PL_NORM || pl_w->act_st != N_SPECIAL || pl_w->pl_sp_type != 0 || pl_w->char_no.b.grp != ew->timer0 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( vs_sys_work.slow_bit & ( 1 << vs_sys_work.slow_step ) ) return ;
	ew->prio = pl_w->prio ;
	ew->sub_prio = -8 ;
	code = pl_w->cg_prm ;
	if( code ){
		ew->disp_flag = 1 ;
		if( code != ew->dir ){
			ew->dir = code ;
			i = ( code & 15 ) * 4 + ew->sub_type ;
			pl_char_set( ew, Char_Effect, pls0e_00_char_tbl[i] ) ;
			ofs = pls0e_00_ofs_tbl[0] ;
			ofs += ( ( ew->dir & 15 ) - 1 ) * 8 + ew->sub_type * 2 ;
			ew->spd.x = *ofs ++ * 1.6666666f ;
			ew->spd.y = *ofs * 2.142857f ;
			if( pl_w->pl_dir ) ew->spd.x = -ew->spd.x ;
		}
		ew->pos = pl_w->pos ;
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		pl_char_move( ew ) ;
	}
	else ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls0e_00_fang_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

static void pls0e_00_ground_init( PLAYER_WORK *ew );
static void pls0e_00_ground_move( PLAYER_WORK *ew );
static void pls0e_00_ground_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls0e_00_ground( PLAYER_WORK *ew )
{
	static void ( *pls0e_00_ground_jmp[] )( PLAYER_WORK *ew ) = {
		pls0e_00_ground_init, pls0e_00_ground_move, pls0e_00_ground_free,
	} ;

	pls0e_00_ground_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls0e_00_ground_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

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
	ew->prio = 0 ;
	ew->pos = pl_w->pos ;
	pl_char_set( ew, Char_Effect, 9 ) ;
}

/*--------------------------------------------------------------*/
static void pls0e_00_ground_move( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void pls0e_00_ground_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls0e_00.c" End of File	*/

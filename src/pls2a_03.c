/* Storm's player effect 03: two copies of her sprite (types 0 and 1) that follow her and
   play the animation of group 27 that her frame code asks for, from a table of their own. */
#include "player.h"
#include "ef_char.h"

/* the animations, by frame code */
static Uint8 pls2a_03_tbl0[] = { 0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38 } ;
static Uint8 pls2a_03_tbl1[14] = { 0, 39, 40, 41, 42, 43, 44 } ;

/* the effect's own work */
typedef struct PLS2A_03_LOCAL_W {
	Uint8	*tbl;	/* the table of its type */
} PLS2A_03_LOCAL_W;
#define PLS2A_03_LW( w )	( ( PLS2A_03_LOCAL_W * )&( w )->obj_flag )

static void pls2a_03_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls2a_03_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS_ST, LIST_TOP ) ) ){
		ew->work_func = pls2a_03_main ;
		ew->master_w = pl_w ;
		ew->type = 0 ;
		ew->ef_no = 0x2a03 ;
	}
	if( ( ew = work_get( 0, W_PLS_ST, LIST_TOP ) ) ){
		ew->work_func = pls2a_03_main ;
		ew->master_w = pl_w ;
		ew->type = 1 ;
		ew->ef_no = 0x2a03 ;
	}
	return ew ;
}

static void pls2a_03_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2a_03_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2a_03_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2a_03_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2a_03_main( PLAYER_WORK *ew )
{
	static void ( *pls2a_03_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2a_03_init, pls2a_03_move, pls2a_03_end, pls2a_03_free,
	} ;

	pls2a_03_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void pls2a_03_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Uint8 *tbl ;
	int lv ;

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
	ew->pos = pl_w->pos ;
	ew->timer0 = -1 ;
	tbl = pls2a_03_tbl0 ;
	lv = 2 ;
	if( ew->type ){
		tbl = pls2a_03_tbl1 ;
		lv = 1 ;
	}
	PLS2A_03_LW( ew )->tbl = tbl ;
	ew->sub_prio = lv ;
	pls2a_03_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls2a_03_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Sint16 no ;
	Sint16 ef ;
	Sint16 ofs ;
	Uint8 *tbl ;

	if( ew->pl_type != pl_w->pl_type ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->disp_flag = 0 ;
	if( !pl_w->disp_flag ) return ;
	ew->pos = pl_w->pos ;
	ew->disp_dir = pl_w->disp_dir ;
	ew->prio = pl_w->prio ;
	if( ( no = HI_BYTE( pl_w->cg_rival ) ) > 13 ){
		ew->timer0 = -1 ;
		return ;
	}
	if( ew->timer0 != no ){
		ew->timer0 = no ;
		tbl = PLS2A_03_LW( ew )->tbl ;
		if( !( ef = tbl[no] ) ){
			ew->timer0 = -1 ;
			return ;
		}
		ef_char_set( ew, 27, ef ) ;
	} else if( !vs_sys_work.unk_44 && !( vs_sys_work.game_stop & ( 1 << ( ew->pl_no ^ 1 ) ) ) ){
		ef_char_move( ew ) ;
	}
	ew->disp_flag = 1 ;
	if( !ew->type && no == 13 ){
		ofs = 10 ;
		if( ew->disp_dir ) ofs = -10 ;
		ew->pos.x += DOT_X( ofs ) ;
		ew->pos.y += DOT_Y( 6.0f ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls2a_03_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls2a_03_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls2a_03.c" End of File	*/

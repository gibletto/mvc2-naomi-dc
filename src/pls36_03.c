/* Thanos's player effect 03: the glows and sparks on him that follow his animation: they
   colour him by the frame, ride on his place, or circle him and fade. */
#include "player.h"
#include "ef_char.h"
#include "pl36_def.h"

/* the effect's own work, from obj_flag: the animation of the player it was made in */
typedef struct PLS36_03_LOCAL_W {
	Sint32	char_no;
} PLS36_03_LOCAL_W;
#define PLS36_03_LW( w )	( ( PLS36_03_LOCAL_W * )&( w )->obj_flag )

static void pls36_03_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls36_03_set( PLAYER_WORK *pl_w, Uint16 no )
{
	PLAYER_WORK *ew ;
	Uint8 list ;

	list = 3 ;
	if( no >= 3 ) list = 4 ;
	if( ( ew = work_get( 0, list, LIST_TOP ) ) ){
		ew->work_func = pls36_03_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = no ;
		ew->ef_no = 0x3603 ;
		PLS36_03_LW( ew )->char_no = pl_w->char_no.w ;
		ew->dir = 0 ;
	}
	return ew ;
}

static void pls36_03_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls36_03_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls36_03_nop( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls36_03_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls36_03_main( PLAYER_WORK *ew )
{
	static void ( *pls36_03_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls36_03_init, pls36_03_move, pls36_03_nop, pls36_03_free,
	} ;

	pls36_03_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/* the animation each type shows */
static Uint8 pls36_03_char_tbl[9] = { 14, 13, 16, 17, 18, 19, 20, 21, 22 } ;

/*==============================================================*/
static void pls36_03_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
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
	ew->routine_0 ++ ;
	ew->sub_prio = -1 ;
	ew->prio = 8 ;
	ew->pos = pl_w->pos ;
	ef_char_set( ew, 27, pls36_03_char_tbl[ew->type] ) ;
	pls36_03_move( ew, pl_w ) ;
}

/* the colour each frame code of the animation gives the player, -1 for his own */
static Sint16 pls36_03_col_tbl[7] = { 0, 0, 1, 5, -1, -1, 0 } ;


/*--------------------------------------------------------------*/
static void pls36_03_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float x ;

	if( ew->type >= 3 ) goto circle ;
	if( ew->type != 2 ){
		if( ew->type != 1 ){
			if( ef_char_move( ew ) < 0 ) pls36_03_free( ew, pl_w ) ;
		} else {
			if( PLS36_03_LW( ew )->char_no != pl_w->char_no.w ) pls36_03_free( ew, pl_w ) ;
			else {
				ef_char_move( ew ) ;
				if( !ew->cg_prm ) return ;
				if( pls36_03_col_tbl[ew->cg_prm] < 0 ) pl_color_trans( pl_w, 1, 0, 1 ) ;
				else ef_color_set( pl_w, 1, pls36_03_col_tbl[ew->cg_prm], 1 ) ;
				ew->cg_prm = 0 ;
			}
		}
		return ;
	}
	if( PLS36_03_LW( ew )->char_no != pl_w->char_no.w ){
		pls36_03_free( ew, pl_w ) ;
		return ;
	}
	ew->disp_flag = 0 ;
	if( !pl_w->cg_flag1 ) return ;
	ew->disp_flag = 1 ;
	ew->prio = pl_w->prio ;
	ew->pos = pl_w->pos ;
	if( ef_char_move( ew ) >= 0 ){
		if( 1 ) ;
	} else pls36_03_free( ew, pl_w ) ;
	return ;

circle :
	if( PLS36_03_LW( ew )->char_no != pl_w->char_no.w ){
		pls36_03_free( ew, pl_w ) ;
		return ;
	}
	if( !ew->routine_1 ){
		ew->routine_1 ++ ;
		ew->timer0 = 60 ;
		ew->timer1 = 0 ;
	}
	ew->timer1 += 120 ;
	x = 40.0f ;
	if( pl_w->disp_dir ) x = -40.0f ;
	ew->pos.x = pl_w->pos.x + x ;
	ew->pos.y = pl_w->pos.y + DOT_Y( 160.0f ) ;
	dir_move( ew, ew->timer1 ) ;
	ew->dir += 9 ;
	ew->dir &= 31 ;
	if( -- ew->timer0 <= 0 ) pls36_03_free( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls36_03_nop( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
}

/*--------------------------------------------------------------*/
static void pls36_03_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls36_03.c" End of File	*/

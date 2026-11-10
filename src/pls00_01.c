/* Ryu's player effect 01: the pair of works (type 0 and 1, kind 3) that his Shinkuu Hadouken puts beside him,
   which follow him at an offset and stay while his animation is one of two. */
#include "player.h"
#include "pl_char.h"

/* the effect's own work, from obj_flag: the offset from the master, which both of the pair read through
   one pointer, set by the main routine */
typedef struct PLS00_01_LOCAL_W {
	Float	x, y;
} PLS00_01_LOCAL_W;
#define PLS00_01_LW( w )	( ( PLS00_01_LOCAL_W * )&( w )->obj_flag )

static Float *pls00_01_ofs_p ;

static void pls00_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls00_01_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls00_01_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 1 ;
		ew->type = no ;
		ew->sub_type = 0 ;
	}
	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls00_01_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 1 ;
		ew->type = no ;
		ew->sub_type = 1 ;
	}
	return ew ;
}

static void pls00_01_init( PLAYER_WORK *ew ) ;
static void pls00_01_move( PLAYER_WORK *ew ) ;
static void pls00_01_end( PLAYER_WORK *ew ) ;
static void pls00_01_free( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void pls00_01_main( PLAYER_WORK *ew )
{
	static void ( *pls00_01_jmp[] )( PLAYER_WORK *ew ) = {
		pls00_01_init, pls00_01_move, pls00_01_end, pls00_01_free,
	} ;

	pls00_01_ofs_p = &PLS00_01_LW( ew )->x ;
	pls00_01_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
/*			Beside the master, at the offset					*/
/*==============================================================*/
static void pls00_01_pos_set( PLAYER_WORK *ew )
{
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	if( ew->disp_dir ) ew->pos.x += pls00_01_ofs_p[0] ;
	else ew->pos.x -= pls00_01_ofs_p[0] ;
	ew->pos.y += pls00_01_ofs_p[1] ;
}

/*==============================================================*/
static void pls00_01_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->disp_flag = 1 ;
	ew->prio = ew->sub_type ? 0 : 11 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->timer0 = 0 ;
	pls00_01_ofs_p[0] = ew->type ? -30.0f : 0.0f ;
	pls00_01_ofs_p[1] = ew->type ? DOT_Y( 16.0f ) : 0.0f ;
	pls00_01_pos_set( ew ) ;
	pl_char_set( ew, Char_Effect, ew->sub_type << 1 ) ;
}

/*--------------------------------------------------------------*/
static void pls00_01_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	pls00_01_pos_set( ew ) ;
	pl_w = ew->master_w ;
	if( HI_BYTE( pl_w->cg_rival ) != 5 && HI_BYTE( pl_w->cg_rival ) != 8 ){
		pls00_01_end( ew ) ;
		return ;
	}
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls00_01_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	pls00_01_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls00_01_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls00_01.c" End of File	*/

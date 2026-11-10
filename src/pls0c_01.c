/* Spider-Man's player effect 01: a copy of the player that shows on his position while his
   animation frame asks for it, and takes up the animation the frame names. */
#include "player.h"

typedef struct PLS0C_01_LOCAL_W {
	Uint8	on ;
} PLS0C_01_LOCAL_W ;
#define PLS0C_01_LW( w )	( ( PLS0C_01_LOCAL_W * )&( w )->obj_flag )

static void pls0c_01_main( PLAYER_WORK *ew );

/****************************************************************/
void *pls0c_01_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_END ) ) ){
		ew->ef_no = 0xc01 ;
		ew->work_func = pls0c_01_main ;
		ew->master_w = pl_w ;
	}
	return ew ;
}

static void pls0c_01_init( PLAYER_WORK *ew, PLS0C_01_LOCAL_W *lw, PLAYER_WORK *pl_w );
static void pls0c_01_move( PLAYER_WORK *ew, PLS0C_01_LOCAL_W *lw, PLAYER_WORK *pl_w );
static void pls0c_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls0c_01_main( PLAYER_WORK *ew )
{
	static void ( *pls0c_01_jmp[] )( PLAYER_WORK *ew, PLS0C_01_LOCAL_W *lw, PLAYER_WORK *pl_w ) = {
		pls0c_01_init, pls0c_01_move, pls0c_01_free,
	} ;

	pls0c_01_jmp[ew->routine_0]( ew, PLS0C_01_LW( ew ), ew->master_w ) ;
}

/*==============================================================*/
static void pls0c_01_init( PLAYER_WORK *ew, PLS0C_01_LOCAL_W *lw, PLAYER_WORK *pl_w )
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
	lw->on = 0 ;
}

/*--------------------------------------------------------------*/
static void pls0c_01_move( PLAYER_WORK *ew, PLS0C_01_LOCAL_W *lw, PLAYER_WORK *pl_w )
{
	int no ;

	ew->prio = pl_w->prio ;
	ew->sub_prio = -4 ;
	ew->disp_dir = pl_w->disp_dir ;
	if( ( no = HI_BYTE( pl_w->cg_rival ) ) ){
		ew->disp_flag = 1 ;
		ew->pos = pl_w->pos ;
		lw->on = 1 ;
		pl_char_set2( ew, Char_Effect, no + 38, pl_w->cg_type ) ;
	}
	else if( lw->on ){
		if( pl_char_move( ew ) < 0 ){
			ew->disp_flag = 0 ;
			lw->on = 0 ;
		}
	}
}

/*--------------------------------------------------------------*/
static void pls0c_01_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls0c_01.c" End of File	*/

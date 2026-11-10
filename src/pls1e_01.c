/* Akuma's player effect 01: two copies of his sprite (sub_type 0 and 1) that follow him, offset by the work's own x and y. */
#include "player.h"
#include "pl_char_old.h"

/* the effect's own work, from obj_flag: the offset from him, read through one pointer set by main */
typedef struct PLS1E_01_LOCAL_W {
	Float	x ;
	Float	y ;
} PLS1E_01_LOCAL_W;
#define PLS1E_01_LW( w )	( ( PLS1E_01_LOCAL_W * )&( w )->obj_flag )

static PLS1E_01_LOCAL_W *pls1e_01_lw_p ;

static void pls1e_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls1e_01_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls1e_01_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x1e01 ;
		ew->sub_type = 0 ;
	}
	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls1e_01_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x1e01 ;
		ew->sub_type = 1 ;
	}
	return ew ;
}

static void pls1e_01_init( PLAYER_WORK *ew );
static void pls1e_01_move( PLAYER_WORK *ew );
static void pls1e_01_end( PLAYER_WORK *ew );
static void pls1e_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls1e_01_main( PLAYER_WORK *ew )
{
	static void ( *pls1e_01_jmp[] )( PLAYER_WORK *ew ) = {
		pls1e_01_init, pls1e_01_move, pls1e_01_end, pls1e_01_free,
	} ;

	pls1e_01_lw_p = PLS1E_01_LW( ew ) ;
	pls1e_01_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls1e_01_follow( PLAYER_WORK *ew )
{
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	if( ew->disp_dir ) ew->pos.x += pls1e_01_lw_p->x ;
	else ew->pos.x -= pls1e_01_lw_p->x ;
	ew->pos.y += pls1e_01_lw_p->y ;
}

/*==============================================================*/
static void pls1e_01_init( PLAYER_WORK *ew )
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
	pls1e_01_lw_p->x = 0.0f ;
	pls1e_01_lw_p->y = 0.0f ;
	pls1e_01_follow( ew ) ;
	pl_char_set( ew, Char_Effect, ew->sub_type * 2 ) ;
}

/*--------------------------------------------------------------*/
static void pls1e_01_move( PLAYER_WORK *ew )
{
	pls1e_01_follow( ew ) ;
	if( HI_BYTE( ew->master_w->cg_rival ) != 3 ){
		pls1e_01_end( ew ) ;
		return ;
	}
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls1e_01_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	pls1e_01_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls1e_01_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls1e_01.c" End of File	*/

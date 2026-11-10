/* Amingo's player effect 00: four kinds of overlay that ride on him and show an animation of his. */
#include "player.h"
#include "pl_char.h"
#include "pl15_def.h"

static void pls15_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls15_00_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls15_00_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->ef_no = 0x1500 ;
	}
	return ew ;
}

static void pls15_00_t0_main( PLAYER_WORK *ew );
static void pls15_00_t1_main( PLAYER_WORK *ew );
static void pls15_00_t2_main( PLAYER_WORK *ew );
static void pls15_00_t3_main( PLAYER_WORK *ew );

/*==============================================================*/
static void pls15_00_main( PLAYER_WORK *ew )
{
	static void ( *pls15_00_jmp[] )( PLAYER_WORK *ew ) = {
		pls15_00_t0_main, pls15_00_t1_main, pls15_00_t2_main, pls15_00_t3_main,
	} ;

	pls15_00_jmp[ew->type]( ew ) ;
}

static void pls15_00_t0_init( PLAYER_WORK *ew );
static void pls15_00_t0_move( PLAYER_WORK *ew );
static void pls15_00_end( PLAYER_WORK *ew );
static void pls15_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls15_00_t0_main( PLAYER_WORK *ew )
{
	static void ( *pls15_00_t0_jmp[] )( PLAYER_WORK *ew ) = {
		pls15_00_t0_init, pls15_00_t0_move, pls15_00_end, pls15_00_free,
	} ;

	pls15_00_t0_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls15_00_t0_init( PLAYER_WORK *ew )
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
	ew->prio = 0 ;
	if( ew->sub_type ) pl_char_set2( ew, 21, 16, 6 ) ;
	else pl_char_set2( ew, 21, 10, 6 ) ;
}

/*--------------------------------------------------------------*/
static void pls15_00_t0_move( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

static void pls15_00_t1_init( PLAYER_WORK *ew );
static void pls15_00_t1_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls15_00_t1_main( PLAYER_WORK *ew )
{
	static void ( *pls15_00_t1_jmp[] )( PLAYER_WORK *ew ) = {
		pls15_00_t1_init, pls15_00_t1_move, pls15_00_end, pls15_00_free,
	} ;

	pls15_00_t1_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls15_00_t1_init( PLAYER_WORK *ew )
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
	ew->prio = 7 ;
	pl_char_set( ew, 21, 11 ) ;
}

/*==============================================================*/
static void pls15_00_t1_move( PLAYER_WORK *ew )
{
	PL15_LOCAL_W *lw = ( PL15_LOCAL_W * )ew->master_w->tlocal ;

	switch( ew->routine_1 ){
	case 0:
		pl_char_move( ew ) ;
		if( ew->cg_type ){
			ew->routine_1 ++ ;
			pls15_01_ef_set( ew, 2 ) ;
		}
		break ;
	case 1:
		if( lw->midori_land_flag ){
			ew->routine_1 ++ ;
			ew->pos.x = ew->master_w->pos.x ;
		} else if( ew->master_w->act_st != N_SPECIAL || ew->master_w->routine_1 ){
			ew->routine_1 = 3 ;
			pl_char_set( ew, 21, 13 ) ;
		}
		break ;
	case 2:
		if( ew->master_w->act_st != N_SPECIAL || ew->master_w->routine_1 ){
			ew->routine_1 = 3 ;
			pl_char_set( ew, 21, 13 ) ;
			return ;
		}
		if( ew->master_w->char_no.w == 0x150b ){
			ew->routine_0 ++ ;
			ew->disp_flag = 0 ;
		}
		break ;
	case 3:
		if( pl_char_move( ew ) < 0 ){
			ew->routine_0 ++ ;
			ew->disp_flag = 0 ;
		}
		break ;
	}
}

/*==============================================================*/
static void pls15_00_t2_main( PLAYER_WORK *ew )
{
	Uint8 no ;

	ew->disp_flag = 0 ;
	if( ew->routine_0 == 0 ){
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
		ew->work_no = ew->master_w->catch_w->work_no ;
		ew->prio = 7 ;
		ew->sub_type = 0xff ;
	}
	ew->pos = ew->master_w->pos ;
	no = ew->master_w->cg_prm ;
	if( !no || ew->master_w->char_no.b.grp != Char_Catch ){
		pls15_00_free( ew ) ;
		return ;
	}
	ew->disp_flag = 1 ;
	if( no != ew->sub_type ){
		ew->sub_type = no ;
		ef_char_set2( ew, 27, 0, no - 1 ) ;
	}
}

static void pls15_00_t3_init( PLAYER_WORK *ew );
static void pls15_00_t3_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls15_00_t3_main( PLAYER_WORK *ew )
{
	static void ( *pls15_00_t3_jmp[] )( PLAYER_WORK *ew ) = {
		pls15_00_t3_init, pls15_00_t3_move, pls15_00_end, pls15_00_free,
	} ;

	pls15_00_t3_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls15_00_t3_init( PLAYER_WORK *ew )
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
	ew->prio = 0 ;
	pl_char_set2( ew, 21, 10, 6 ) ;
	if( ew->cg_prm ) ew->rot[2] = -0x1000 ;
}

/*--------------------------------------------------------------*/
static void pls15_00_t3_move( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	} else if( ew->cg_prm ) ew->rot[2] = -0x1000 ;
	else ew->rot[2] = 0 ;
}

/*--------------------------------------------------------------*/
static void pls15_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls15_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls15_00.c" End of File	*/

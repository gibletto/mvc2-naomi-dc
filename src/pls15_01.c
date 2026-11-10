/* Amingo's player effect 01: the shapes that show beside him or on the effect he made, by type (0 to 8). */
#include "player.h"
#include "ef_char.h"
#include "pl15_def.h"

/* where each of types 3 to 7 sits from his position: x and y */
const Float pls15_01_ofs_tbl[5][2] = {
	{ DOT_X( 106.0f ), DOT_Y( 72.0f ) }, { DOT_X( 38.0f ), DOT_Y( 75.0f ) }, { DOT_X( 44.0f ), DOT_Y( 6.0f ) }, { 205.0f, DOT_Y( 23.0f ) }, { DOT_X( 101.0f ), DOT_Y( 33.0f ) }
} ;

static void pls15_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls15_01_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls15_01_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x1501 ;
	}
	return ew ;
}

/****************************************************************/
PLAYER_WORK *pls15_01_ef_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls15_01_main ;
		ew->master_w = pl_w ;
		ew->ef_w = pl_w->master_w ;
		ew->type = no ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x1501 ;
	}
	return ew ;
}

static void pls15_01_t0_main( PLAYER_WORK *ew );
static void pls15_01_t1_main( PLAYER_WORK *ew );
static void pls15_01_t2_main( PLAYER_WORK *ew );
static void pls15_01_t3_main( PLAYER_WORK *ew );
static void pls15_01_t8_main( PLAYER_WORK *ew );

/*==============================================================*/
static void pls15_01_main( PLAYER_WORK *ew )
{
	static void ( *pls15_01_jmp[] )( PLAYER_WORK *ew ) = {
		pls15_01_t0_main, pls15_01_t1_main, pls15_01_t2_main,
		pls15_01_t3_main, pls15_01_t3_main, pls15_01_t3_main, pls15_01_t3_main, pls15_01_t3_main,
		pls15_01_t8_main,
	} ;

	pls15_01_jmp[ew->type]( ew ) ;
}

static void pls15_01_end( PLAYER_WORK *ew );
static void pls15_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls15_01_t0_main( PLAYER_WORK *ew )
{
	pls15_01_free( ew ) ;
}

/*==============================================================*/
static void pls15_01_t1_main( PLAYER_WORK *ew )
{
	switch( ew->routine_0 ){
	case 0:
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
		ef_char_set( ew, 27, 2 ) ;
		ew->pos = ew->master_w->pos ;
		break ;
	case 1:
		if( ef_char_move( ew ) < 0 ) pls15_01_end( ew ) ;
		break ;
	case 2:
		pls15_01_free( ew ) ;
		break ;
	}
}

/*==============================================================*/
static void pls15_01_t2_main( PLAYER_WORK *ew )
{
	PL15_LOCAL_W *lw ;

	switch( ew->routine_0 ){
	case 0:
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
		ef_char_set( ew, 27, 4 ) ;
		ew->pos = ew->master_w->pos ;
		ew->prio = 0 ;
		break ;
	case 1:
		lw = ( PL15_LOCAL_W * )ew->ef_w->tlocal ;
		if( lw->midori_land_flag || ew->ef_w->act_st != N_SPECIAL || ew->ef_w->routine_1 ) pls15_01_end( ew ) ;
		break ;
	case 2:
		pls15_01_free( ew ) ;
		break ;
	}
}

/*==============================================================*/
static void pls15_01_t3_main( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PL15_LOCAL_W *lw = ( PL15_LOCAL_W * )pl_w->tlocal ;
	Uint8 no = ew->type - 3 ;

	switch( ew->routine_0 ){
	case 0:
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
		ef_char_set( ew, 27, no + 5 ) ;
		ew->prio = 0 ;
		ew->pos = pl_w->pos ;
		if( ew->disp_dir ) ew->pos.x -= pls15_01_ofs_tbl[no][0] ;
		else ew->pos.x += pls15_01_ofs_tbl[no][0] ;
		ew->pos.y += pls15_01_ofs_tbl[no][1] ;
		break ;
	case 1:
		if( lw->kaze_end_flag || pl_w->act_st != N_SPECIAL || pl_w->routine_1 ) pls15_01_end( ew ) ;
		else if( ef_char_move( ew ) < 0 ) ew->routine_0 = 0 ;
		break ;
	case 2:
		pls15_01_free( ew ) ;
		break ;
	}
}

/*==============================================================*/
static void pls15_01_t8_main( PLAYER_WORK *ew )
{
	switch( ew->routine_0 ){
	case 0:
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
		ef_char_set( ew, 27, 10 ) ;
		ew->pos = ew->master_w->pos ;
		ew->prio = 0 ;
		break ;
	case 1:
		if( ef_char_move( ew ) < 0 ) pls15_01_end( ew ) ;
		break ;
	case 2:
		pls15_01_free( ew ) ;
		break ;
	}
}

/*==============================================================*/
static void pls15_01_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*==============================================================*/
static void pls15_01_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls15_01.c" End of File	*/

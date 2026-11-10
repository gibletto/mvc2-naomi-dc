/* Anakaris's effect 01 (character 04's player effect 01): the two pictures of a win pose,
   one drawn over the other beside the opposing point character. Each plays one animation and
   stays until it is done and Anakaris has left the win. */
#include "player.h"
#include "ef_char.h"

static void pls04_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls04_01_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls04_01_main ;
		ew->master_w = pl_w ;
		ew->type = 0 ;
		ew->sub_type = 0 ;
		ew->ef_no = 0x401 ;
	}
	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls04_01_main ;
		ew->master_w = pl_w ;
		ew->type = 0 ;
		ew->sub_type = 1 ;
		ew->ef_no = 0x401 ;
	}
	return ew ;
}

static void pls04_01_init( PLAYER_WORK *ew );
static void pls04_01_move( PLAYER_WORK *ew );
static void pls04_01_end( PLAYER_WORK *ew );
static void pls04_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls04_01_main( PLAYER_WORK *ew )
{
	static void ( *pls04_01_jmp[] )( PLAYER_WORK *ew ) = {
		pls04_01_init, pls04_01_move, pls04_01_end, pls04_01_free,
	} ;

	pls04_01_jmp[ew->routine_0]( ew ) ;
}

static void pls04_01_init0( PLAYER_WORK *ew );
static void pls04_01_init1( PLAYER_WORK *ew );

/*==============================================================*/
static void pls04_01_init( PLAYER_WORK *ew )
{
	static void ( *pls04_01_init_jmp[] )( PLAYER_WORK *ew ) = {
		pls04_01_init0, pls04_01_init1,
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
	if( !ew->sub_type ) ew->prio = 11 ;
	else ew->prio = 12 ;
	pls04_01_init_jmp[ew->type]( ew ) ;
	pls04_01_move( ew ) ;
}

/*==============================================================*/
/*			Beside the opposing point character					*/
/*==============================================================*/
static void pls04_01_init0( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *em_w = pl_w->target_w ;
	Float ofs = DOT_X( 128.0f ) ;

	if( pl_w->disp_dir ) ofs = -DOT_X( 128.0f ) ;
	ew->pos.x = em_w->pos.x + ofs ;
	ew->pos.y = em_w->pos.y ;
	ew->disp_dir = pl_w->disp_dir ;
	ef_char_set( ew, 27, ew->sub_type * 2 + 16 ) ;
}

/*==============================================================*/
static void pls04_01_init1( PLAYER_WORK *ew )
{
}

/*--------------------------------------------------------------*/
static void pls04_01_move( PLAYER_WORK *ew )
{
	if( !ew->routine_1 ){
		ef_char_move( ew ) ;
		if( ew->cg_prm ){
			PLAYER_WORK *pl_w = ew->master_w ;

			if( pl_w->act_st != N_WIN ) ew->routine_1 ++ ;
		}
	}
	else if( ef_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void pls04_01_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls04_01_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls04_01.c" End of File	*/

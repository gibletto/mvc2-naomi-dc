/* Marrow's player effect 01: three overlays on her that show the animation her cg_rival asks for. */
#include "player.h"
#include "ef_char.h"
#include "pl16_def.h"

static void pls16_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls16_01_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls16_01_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x1601 ;
		ew->type = no ;
	}
	return ew ;
}

static void pls16_01_init( PLAYER_WORK *ew );
static void pls16_01_move( PLAYER_WORK *ew );
static void pls16_01_end( PLAYER_WORK *ew );
static void pls16_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls16_01_main( PLAYER_WORK *ew )
{
	static void ( *pls16_01_jmp[] )( PLAYER_WORK *ew ) = {
		pls16_01_init, pls16_01_move, pls16_01_end, pls16_01_free,
	} ;

	pls16_01_jmp[ew->routine_0]( ew ) ;
}

static void pls16_01_t0_init( PLAYER_WORK *ew );
static void pls16_01_t1_init( PLAYER_WORK *ew );
static void pls16_01_t2_init( PLAYER_WORK *ew );

/*==============================================================*/
static void pls16_01_init( PLAYER_WORK *ew )
{
	static void ( *pls16_01_init_jmp[] )( PLAYER_WORK *ew ) = {
		pls16_01_t0_init, pls16_01_t1_init, pls16_01_t2_init,
	} ;

	pls16_01_init_jmp[ew->type]( ew ) ;
}

static void pls16_01_t0_move( PLAYER_WORK *ew );
static void pls16_01_t1_move( PLAYER_WORK *ew );
static void pls16_01_t2_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls16_01_t0_init( PLAYER_WORK *ew )
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
	ew->prio = 0 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ef_char_set( ew, 27, 0 ) ;
	pls16_01_t0_move( ew ) ;
}

/*==============================================================*/
static void pls16_01_t1_init( PLAYER_WORK *ew )
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
	ew->prio = 0 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	pls16_01_t1_move( ew ) ;
}

/*==============================================================*/
static void pls16_01_t2_init( PLAYER_WORK *ew )
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
	ew->prio = 11 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	pls16_01_t2_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls16_01_move( PLAYER_WORK *ew )
{
	static void ( *pls16_01_move_jmp[] )( PLAYER_WORK *ew ) = {
		pls16_01_t0_move, pls16_01_t1_move, pls16_01_t2_move,
	} ;

	pls16_01_move_jmp[ew->type]( ew ) ;
}

/*==============================================================*/
static void pls16_01_t0_move( PLAYER_WORK *ew )
{
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	if( ef_char_move( ew ) < 0 ) pls16_01_end( ew ) ;
}

/*==============================================================*/
static void pls16_01_t1_move( PLAYER_WORK *ew )
{
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ef_char_set( ew, 27, 1 ) ;
	for(;;){
		if( HI_BYTE( ew->master_w->cg_rival ) == ew->cg_prm ){
			ew->disp_flag = 1 ;
			break ;
		}
		if( ef_char_move( ew ) < 0 ){
			pls16_01_end( ew ) ;
			break ;
		}
	}
}

/*==============================================================*/
static void pls16_01_t2_move( PLAYER_WORK *ew )
{
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	if( !ew->master_w->hit_flag ) ef_char_set( ew, 27, 2 ) ;
	else ef_char_set( ew, 27, 3 ) ;
	for(;;){
		if( HI_BYTE( ew->master_w->cg_rival ) == ew->cg_prm ){
			ew->disp_flag = 1 ;
			break ;
		}
		if( ef_char_move( ew ) < 0 ){
			pls16_01_end( ew ) ;
			break ;
		}
	}
}

/*--------------------------------------------------------------*/
static void pls16_01_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	pls16_01_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls16_01_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls16_01.c" End of File	*/

/* Marrow's player effect 06: the effect of her Stinger Bones, which waits for her animation to
   raise cg_prm and then shows its three shapes in turn, one for each time she does. */
#include "player.h"
#include "ef_char.h"
#include "pl16_def.h"

/* the effect's own work, from obj_flag: the animation of hers that it follows, read through one pointer
   set by the main routine */
typedef struct PLS16_06_LOCAL_W {
	Uint16	char_no ;
} PLS16_06_LOCAL_W;
#define PLS16_06_LW( w )	( ( PLS16_06_LOCAL_W * )&( w )->obj_flag )

static Uint16 *pls16_06_char_no_p ;

static void pls16_06_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls16_06_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls16_06_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x1601 ;
	}
	return ew ;
}

static void pls16_06_init( PLAYER_WORK *ew );
static void pls16_06_wait( PLAYER_WORK *ew );
static void pls16_06_step0( PLAYER_WORK *ew );
static void pls16_06_step1( PLAYER_WORK *ew );
static void pls16_06_last( PLAYER_WORK *ew );
static void pls16_06_end( PLAYER_WORK *ew );
static void pls16_06_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls16_06_main( PLAYER_WORK *ew )
{
	static void ( *pls16_06_jmp[] )( PLAYER_WORK *ew ) = {
		pls16_06_init, pls16_06_wait, pls16_06_step0, pls16_06_step1,
		pls16_06_last, pls16_06_end, pls16_06_free,
	} ;

	pls16_06_char_no_p = &PLS16_06_LW( ew )->char_no ;
	pls16_06_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls16_06_init( PLAYER_WORK *ew )
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
	ew->disp_flag = 0 ;
	ew->prio = 7 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	*pls16_06_char_no_p = ew->master_w->char_no.w ;
}

/*--------------------------------------------------------------*/
static void pls16_06_wait( PLAYER_WORK *ew )
{
	if( *pls16_06_char_no_p != ew->master_w->char_no.w ){
		pls16_06_end( ew ) ;
		return ;
	}
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	if( ew->master_w->cg_prm == 1 ){
		ew->routine_0 ++ ;
		ew->master_w->cg_prm = 0 ;
		ef_char_set( ew, 27, 13 ) ;
		ew->disp_flag = 1 ;
	}
}

/*--------------------------------------------------------------*/
static void pls16_06_step0( PLAYER_WORK *ew )
{
	if( *pls16_06_char_no_p != ew->master_w->char_no.w ){
		pls16_06_end( ew ) ;
		return ;
	}
	if( ef_char_move( ew ) < 0 ) ew->disp_flag = 0 ;
	ew->prio = 0 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	if( ew->master_w->cg_prm == 1 ){
		ew->routine_0 ++ ;
		ew->master_w->cg_prm = 0 ;
		ef_char_set( ew, 27, 14 ) ;
		ew->disp_flag = 1 ;
	}
}

/*--------------------------------------------------------------*/
static void pls16_06_step1( PLAYER_WORK *ew )
{
	if( *pls16_06_char_no_p != ew->master_w->char_no.w ){
		pls16_06_end( ew ) ;
		return ;
	}
	if( ef_char_move( ew ) < 0 ) ew->disp_flag = 0 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	if( ew->master_w->cg_prm == 1 ){
		ew->routine_0 ++ ;
		ew->master_w->cg_prm = 0 ;
		ef_char_set( ew, 27, 15 ) ;
		ew->disp_flag = 1 ;
	}
}

/*--------------------------------------------------------------*/
static void pls16_06_last( PLAYER_WORK *ew )
{
	if( *pls16_06_char_no_p != ew->master_w->char_no.w ){
		pls16_06_end( ew ) ;
		return ;
	}
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	if( ef_char_move( ew ) < 0 ) pls16_06_end( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls16_06_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	pls16_06_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls16_06_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls16_06.c" End of File	*/

/* Marrow's player effect 03: the shape that stays on her while she keeps one animation and shows the
   frame her cg_flag1 asks for, one frame behind (type 0 and 1 differ in the animation). */
#include "player.h"
#include "pl_char.h"
#include "pl16_def.h"

/* the effect's own work, from obj_flag: the animation of hers that it follows, read through one pointer
   set by the main routine */
typedef struct PLS16_03_LOCAL_W {
	Uint16	char_no ;
} PLS16_03_LOCAL_W;
#define PLS16_03_LW( w )	( ( PLS16_03_LOCAL_W * )&( w )->obj_flag )

static Uint16 *pls16_03_char_no_p ;

static void pls16_03_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls16_03_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls16_03_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x1603 ;
		ew->type = no ;
	}
	return ew ;
}

static void pls16_03_init( PLAYER_WORK *ew );
static void pls16_03_move( PLAYER_WORK *ew );
static void pls16_03_end( PLAYER_WORK *ew );
static void pls16_03_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls16_03_main( PLAYER_WORK *ew )
{
	static void ( *pls16_03_jmp[] )( PLAYER_WORK *ew ) = {
		pls16_03_init, pls16_03_move, pls16_03_end, pls16_03_free,
	} ;

	pls16_03_char_no_p = &PLS16_03_LW( ew )->char_no ;
	pls16_03_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls16_03_init( PLAYER_WORK *ew )
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
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	*pls16_03_char_no_p = ew->master_w->char_no.w ;
}

static void pls16_03_nml_p_move( PLAYER_WORK *ew );
static void pls16_03_jump_p_move( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls16_03_move( PLAYER_WORK *ew )
{
	static void ( *pls16_03_move_jmp[] )( PLAYER_WORK *ew ) = {
		pls16_03_nml_p_move, pls16_03_jump_p_move,
	} ;

	pls16_03_move_jmp[ew->type]( ew ) ;
}

/*==============================================================*/
static void pls16_03_nml_p_move( PLAYER_WORK *ew )
{
	if( *pls16_03_char_no_p != ew->master_w->char_no.w ){
		pls16_03_end( ew ) ;
		return ;
	}
	ef_char_set2( ew, 27, 7, ( Sint8 )ew->master_w->cg_flag1 - 1 ) ;
	if( ( Sint8 )ew->master_w->cg_flag1 - 1 < 0 ){
		pls16_03_end( ew ) ;
		return ;
	}
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
}

/*==============================================================*/
static void pls16_03_jump_p_move( PLAYER_WORK *ew )
{
	if( *pls16_03_char_no_p != ew->master_w->char_no.w ){
		pls16_03_end( ew ) ;
		return ;
	}
	ef_char_set2( ew, 27, 8, ( Sint8 )ew->master_w->cg_flag1 - 1 ) ;
	if( ( Sint8 )ew->master_w->cg_flag1 - 1 < 0 ){
		pls16_03_end( ew ) ;
		return ;
	}
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
}

/*--------------------------------------------------------------*/
static void pls16_03_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	pls16_03_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls16_03_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls16_03.c" End of File	*/

/* Cable's player effect 01: a copy of his sprite that follows him and shows his animation while it lasts. */
#include "player.h"
#include "ef_char.h"
#include "pl17_def.h"

/* the effect's own work, from obj_flag: the animation of his that it follows, read through one pointer
   set by the setter */
typedef struct PLS17_01_LOCAL_W {
	Uint16	char_no ;
} PLS17_01_LOCAL_W;
#define PLS17_01_LW( w )	( ( PLS17_01_LOCAL_W * )&( w )->obj_flag )

static Uint16 *pls17_01_char_no_p ;

static void pls17_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls17_01_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls17_01_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->disp_dir = ew->master_w->disp_dir ;
		ew->ef_no = 0x1700 ;
		pls17_01_char_no_p = &PLS17_01_LW( ew )->char_no ;
		*pls17_01_char_no_p = ew->master_w->char_no.w ;
		return ew ;
	}
}

static void pls17_01_init( PLAYER_WORK *ew );
static void pls17_01_move( PLAYER_WORK *ew );
static void pls17_01_end( PLAYER_WORK *ew );
static void pls17_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls17_01_main( PLAYER_WORK *ew )
{
	static void ( *pls17_01_jmp[] )( PLAYER_WORK *ew ) = {
		pls17_01_init, pls17_01_move, pls17_01_end, pls17_01_free,
	} ;

	pls17_01_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls17_01_init( PLAYER_WORK *ew )
{
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
	ew->timer0 = ew->master_w->timer1 ;
	ew->alpha = 0.5f ;
	ew->pos = pl_w->pos ;
	ew->prio = pl_w->prio ;
	ew->sub_prio -= 2 ;
	if( ew->type ) ef_char_set( ew, 23, 9 ) ;
	else ef_char_set( ew, 23, 8 ) ;
}

/*--------------------------------------------------------------*/
static void pls17_01_move( PLAYER_WORK *ew )
{
	ew->prio = ew->master_w->prio ;
	ew->pos = ew->master_w->pos ;
	if( ew->pl_type != ew->master_w->pl_type ){
		pls17_01_free( ew ) ;
		return ;
	}
	if( *pls17_01_char_no_p != ew->master_w->char_no.w ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
	}
	pl_char_move( ew ) ;
	if( ew->cg_type ) ew->alpha = 0.5f ;
	else ew->alpha = 1.0f ;
}

/*--------------------------------------------------------------*/
static void pls17_01_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls17_01_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls17_01.c" End of File	*/

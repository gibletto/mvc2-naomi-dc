/* Marrow's player effect 07: a piece that is thrown up from her side and falls back to the ground,
   going out as soon as its animation or the box it carries is spent. */
#include "player.h"
#include "ef_char.h"
#include "pl16_def.h"

/* the effect's own work, from obj_flag: the animation of hers that it follows, read through one pointer
   set by the main routine */
typedef struct PLS16_07_LOCAL_W {
	Uint16	char_no ;
} PLS16_07_LOCAL_W;
#define PLS16_07_LW( w )	( ( PLS16_07_LOCAL_W * )&( w )->obj_flag )

static Uint16 *pls16_07_char_no_p ;

static void pls16_07_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls16_07_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls16_07_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x1607 ;
	}
	return ew ;
}

static void pls16_07_init( PLAYER_WORK *ew );
static void pls16_07_move( PLAYER_WORK *ew );
static void pls16_07_end( PLAYER_WORK *ew );
static void pls16_07_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls16_07_main( PLAYER_WORK *ew )
{
	static void ( *pls16_07_jmp[] )( PLAYER_WORK *ew ) = {
		pls16_07_init, pls16_07_move, pls16_07_end, pls16_07_free,
	} ;

	pls16_07_char_no_p = &PLS16_07_LW( ew )->char_no ;
	pls16_07_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls16_07_init( PLAYER_WORK *ew )
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
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.x += ew->disp_dir ? 65.0f : -65.0f ;
	ew->pos.y += DOT_Y( 108.0f ) ;
	ew->spd.x = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = 2.142857f ;
	ew->acc.y = -DOT_Y( 0.25f ) ;
	ef_char_set( ew, 27, 18 ) ;
	*pls16_07_char_no_p = ew->master_w->char_no.w ;
}

/*--------------------------------------------------------------*/
static void pls16_07_move( PLAYER_WORK *ew )
{
	if( ew->hit_box_no ){
		pls16_07_end( ew ) ;
		return ;
	}
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.x += ew->disp_dir ? 65.0f : -65.0f ;
	if( ef_char_move( ew ) < 0 ) pls16_07_end( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls16_07_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	pls16_07_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls16_07_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls16_07.c" End of File	*/

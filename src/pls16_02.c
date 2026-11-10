/* Marrow's player effect 02: the mark that stays at her side while she keeps one animation (type 0)
   and the one that drops from it (type 1). */
#include "player.h"
#include "ef_char.h"
#include "pl16_def.h"

/* the effect's own work, from obj_flag: the animation of hers that it follows, read through one pointer
   set by the main routine */
typedef struct PLS16_02_LOCAL_W {
	Uint16	char_no ;
} PLS16_02_LOCAL_W;
#define PLS16_02_LW( w )	( ( PLS16_02_LOCAL_W * )&( w )->obj_flag )

static Uint16 *pls16_02_char_no_p ;

static void pls16_02_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls16_02_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls16_02_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x1602 ;
		ew->type = no ;
	}
	return ew ;
}

static void pls16_02_init( PLAYER_WORK *ew );
static void pls16_02_move( PLAYER_WORK *ew );
static void pls16_02_end( PLAYER_WORK *ew );
static void pls16_02_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls16_02_main( PLAYER_WORK *ew )
{
	static void ( *pls16_02_jmp[] )( PLAYER_WORK *ew ) = {
		pls16_02_init, pls16_02_move, pls16_02_end, pls16_02_free,
	} ;

	pls16_02_char_no_p = &PLS16_02_LW( ew )->char_no ;
	pls16_02_jmp[ew->routine_0]( ew ) ;
}

static void pls16_02_dive_init( PLAYER_WORK *ew );
static void pls16_02_drop_init( PLAYER_WORK *ew );

/*==============================================================*/
static void pls16_02_init( PLAYER_WORK *ew )
{
	static void ( *pls16_02_init_jmp[] )( PLAYER_WORK *ew ) = {
		pls16_02_dive_init, pls16_02_drop_init,
	} ;

	pls16_02_init_jmp[ew->type]( ew ) ;
}

static void pls16_02_dive_move( PLAYER_WORK *ew );
static void pls16_02_drop_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls16_02_dive_init( PLAYER_WORK *ew )
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
	if( ew->atck_level ){
		ew->pos.x += ew->disp_dir ? 35.0f : -35.0f ;
		ew->pos.y += DOT_Y( 29.0f ) ;
		ew->rot[2] = 0x1c00 ;
	} else {
		ew->pos.x += ew->disp_dir ? 60.0f : -60.0f ;
		ew->pos.y += DOT_Y( 23.0f ) ;
		ew->rot[2] = 0x2400 ;
	}
	ef_char_set( ew, 27, 9 ) ;
	*pls16_02_char_no_p = ew->master_w->char_no.w ;
	ew->alpha = 0.5f ;
	pls16_02_dive_move( ew ) ;
}

/*==============================================================*/
static void pls16_02_drop_init( PLAYER_WORK *ew )
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
	ew->spd.x = ew->disp_dir ? 1.6666666f : -1.6666666f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.y = -DOT_Y( 0.25f ) ;
	ef_char_set( ew, 27, 10 ) ;
	pls16_02_drop_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls16_02_move( PLAYER_WORK *ew )
{
	static void ( *pls16_02_move_jmp[] )( PLAYER_WORK *ew ) = {
		pls16_02_dive_move, pls16_02_drop_move,
	} ;

	pls16_02_move_jmp[ew->type]( ew ) ;
}

/*==============================================================*/
static void pls16_02_dive_move( PLAYER_WORK *ew )
{
	if( *pls16_02_char_no_p != ew->master_w->char_no.w ){
		pls16_02_end( ew ) ;
		return ;
	}
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	if( ew->atck_level ){
		ew->pos.x += ew->disp_dir ? DOT_X( 25.0f ) : -DOT_X( 25.0f ) ;
		ew->pos.y += DOT_Y( 29.0f ) ;
	} else {
		ew->pos.x += ew->disp_dir ? 60.0f : -60.0f ;
		ew->pos.y += DOT_Y( 23.0f ) ;
	}
	ef_char_move( ew ) ;
}

/*==============================================================*/
static void pls16_02_drop_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y <= pl_w->ground_y ) ew->pos.y = pl_w->ground_y ;
	if( ef_char_move( ew ) < 0 ) pls16_02_end( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls16_02_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	pls16_02_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls16_02_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls16_02.c" End of File	*/

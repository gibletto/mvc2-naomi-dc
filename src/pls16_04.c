/* Marrow's player effect 04: a mark that stays at her place while she keeps one animation of her
   and then fades out. */
#include "player.h"
#include "ef_char.h"
#include "pl16_def.h"

/* the effect's own work, from obj_flag: the animation of hers that it follows, read through one pointer
   set by the main routine */
typedef struct PLS16_04_LOCAL_W {
	Uint16	char_no ;
} PLS16_04_LOCAL_W;
#define PLS16_04_LW( w )	( ( PLS16_04_LOCAL_W * )&( w )->obj_flag )

static Uint16 *pls16_04_char_no_p ;

static void pls16_04_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls16_04_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS_ST, LIST_TOP ) ) ){
		ew->work_func = pls16_04_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x1604 ;
	}
	return ew ;
}

static void pls16_04_init( PLAYER_WORK *ew );
static void pls16_04_move( PLAYER_WORK *ew );
static void pls16_04_fade( PLAYER_WORK *ew );
static void pls16_04_end( PLAYER_WORK *ew );
static void pls16_04_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls16_04_main( PLAYER_WORK *ew )
{
	static void ( *pls16_04_jmp[] )( PLAYER_WORK *ew ) = {
		pls16_04_init, pls16_04_move, pls16_04_fade, pls16_04_end, pls16_04_free,
	} ;

	pls16_04_char_no_p = &PLS16_04_LW( ew )->char_no ;
	pls16_04_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls16_04_init( PLAYER_WORK *ew )
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
	ef_char_set( ew, 27, 12 ) ;
	*pls16_04_char_no_p = ew->master_w->char_no.w ;
}

/*--------------------------------------------------------------*/
static void pls16_04_move( PLAYER_WORK *ew )
{
	if( *pls16_04_char_no_p != ew->master_w->char_no.w ){
		pls16_04_end( ew ) ;
		return ;
	}
	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->timer0 = 30 ;
		return ;
	}
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
}

/*--------------------------------------------------------------*/
static void pls16_04_fade( PLAYER_WORK *ew )
{
	if( --ew->timer0 > 0 ) return ;
	ew->alpha -= 0.012f ;
	if( ew->alpha <= 0.0f ) pls16_04_end( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls16_04_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	pls16_04_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls16_04_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls16_04.c" End of File	*/

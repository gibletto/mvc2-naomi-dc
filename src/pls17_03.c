/* Cable's player effect 03: four copies of his sprite by type, that blink, show or fade while an animation of his lasts. */
#include "player.h"
#include "ef_char.h"
#include "pl_char_old.h"
#include "pl17_def.h"

/* the effect's own work, from obj_flag: the animation of his that it follows, read through one pointer
   set by the setter */
typedef struct PLS17_03_LOCAL_W {
	Uint16	char_no ;
} PLS17_03_LOCAL_W;
#define PLS17_03_LW( w )	( ( PLS17_03_LOCAL_W * )&( w )->obj_flag )

static Uint16 *pls17_03_char_no_p ;

static void pls17_03_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls17_03_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls17_03_main ;
		ew->master_w = pl_w ;
		ew->disp_dir = ew->master_w->disp_dir ;
		ew->ef_no = 0x1700 ;
		pls17_03_char_no_p = &PLS17_03_LW( ew )->char_no ;
		*pls17_03_char_no_p = ew->master_w->char_no.w ;
		ew->sub_no = no ;
		return ew ;
	}
}

static void pls17_03_main0( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls17_03_main1( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls17_03_main2( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls17_03_main3( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls17_03_main( PLAYER_WORK *ew )
{
	static void ( *pls17_03_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls17_03_main0, pls17_03_main1, pls17_03_main2, pls17_03_main3,
	} ;

	pls17_03_jmp[ew->sub_no]( ew, ew->master_w ) ;
}

static void pls17_03_init0( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls17_03_move0( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls17_03_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls17_03_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls17_03_main0( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls17_03_jmp0[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls17_03_init0, pls17_03_move0, pls17_03_end, pls17_03_free,
	} ;

	pls17_03_jmp0[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls17_03_init0( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 1 ;
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
	ew->prio = 0 ;
	ew->pos = pl_w->pos ;
	ef_char_set( ew, 27, 2 ) ;
}

/*--------------------------------------------------------------*/
static void pls17_03_move0( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_1 ++ ;
	if( ew->routine_1 % 2 ) ew->alpha = 1.0f ;
	else ew->alpha = 0.8f ;
	ew->pos = pl_w->pos ;
	if( ew->pl_type != pl_w->pl_type ){
		pls17_03_free( ew, pl_w ) ;
		return ;
	}
	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
	}
	else if( ew->cg_type == 2 ) ew->prio = pl_w->prio ;
}

static void pls17_03_init1( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls17_03_move1( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls17_03_main1( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls17_03_jmp1[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls17_03_init1, pls17_03_move1, pls17_03_end, pls17_03_free,
	} ;

	pls17_03_jmp1[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls17_03_init1( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 1 ;
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
	ew->prio = 0 ;
	ew->pos = pl_w->pos ;
	ef_char_set( ew, 27, 4 ) ;
}

/*--------------------------------------------------------------*/
static void pls17_03_move1( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos = pl_w->pos ;
	if( ew->pl_type != pl_w->pl_type ){
		pls17_03_free( ew, pl_w ) ;
		return ;
	}
	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
	}
}

static void pls17_03_init2( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls17_03_move2( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls17_03_main2( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls17_03_jmp2[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls17_03_init2, pls17_03_move2, pls17_03_end, pls17_03_free,
	} ;

	pls17_03_jmp2[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls17_03_init2( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 1 ;
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
	ew->prio = 0 ;
	ew->pos = pl_w->pos ;
	ef_char_set( ew, 27, 5 ) ;
	ew->alpha = 1.0f ;
}

/*--------------------------------------------------------------*/
static void pls17_03_move2( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos = pl_w->pos ;
	if( ew->pl_type != pl_w->pl_type ){
		pls17_03_free( ew, pl_w ) ;
		return ;
	}
	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
	}
	ew->alpha -= 0.03125f ;
}

static void pls17_03_init3( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls17_03_move3( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls17_03_main3( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls17_03_jmp3[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls17_03_init3, pls17_03_move3, pls17_03_end, pls17_03_free,
	} ;

	pls17_03_jmp3[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls17_03_init3( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 1 ;
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
	ew->prio = 0 ;
	ew->pos = pl_w->pos ;
	ef_char_set( ew, 27, 3 ) ;
}

/*==============================================================*/
/*			The animation number is passed to ef_char_set2		*/
/*			as if it took an int								*/
/*==============================================================*/
static void pls17_03_move3( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos = pl_w->pos ;
	if( ew->pl_type != pl_w->pl_type ){
		pls17_03_free( ew, pl_w ) ;
		return ;
	}
	if( pl_w->char_no.w != 0xf01 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
	}
	if( HI_BYTE( pl_w->cg_rival ) ){
		if( HI_BYTE( pl_w->cg_rival ) == 16 ){
			ew->disp_flag = 0 ;
			HI_BYTE( pl_w->cg_rival ) = 0 ;
		} else {
			ew->disp_flag = 1 ;
			ef_char_set2( ew, 27, ew->char_no.b.no, HI_BYTE( pl_w->cg_rival ) - 1 ) ;
			HI_BYTE( pl_w->cg_rival ) = 0 ;
		}
	}
}

/*--------------------------------------------------------------*/
static void pls17_03_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls17_03_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls17_03.c" End of File	*/

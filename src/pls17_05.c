/* Cable's player effect 05: two copies of his sprite, by type, that follow him while he is in one of his moves. */
#include "player.h"
#include "ef_char.h"
#include "pl_char.h"
#include "pl17_def.h"

/* the effect's own work, from obj_flag: the animation of his that it follows, read through one pointer
   set by the setter */
typedef struct PLS17_05_LOCAL_W {
	Uint16	char_no ;
} PLS17_05_LOCAL_W;
#define PLS17_05_LW( w )	( ( PLS17_05_LOCAL_W * )&( w )->obj_flag )

static Uint16 *pls17_05_char_no_p ;

static void pls17_05_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls17_05_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls17_05_main ;
		ew->master_w = pl_w ;
		ew->disp_dir = ew->master_w->disp_dir ;
		ew->ef_no = 0x1700 ;
		pls17_05_char_no_p = &PLS17_05_LW( ew )->char_no ;
		*pls17_05_char_no_p = ew->master_w->char_no.w ;
		ew->sub_no = no ;
		return ew ;
	}
}

static void pls17_05_main0( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls17_05_main1( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls17_05_main( PLAYER_WORK *ew )
{
	static void ( *pls17_05_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls17_05_main0, pls17_05_main1,
	} ;

	pls17_05_jmp[ew->sub_no]( ew, ew->master_w ) ;
}

static void pls17_05_init0( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls17_05_move0( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls17_05_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls17_05_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls17_05_main0( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls17_05_jmp0[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls17_05_init0, pls17_05_move0, pls17_05_end, pls17_05_free,
	} ;

	pls17_05_jmp0[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void pls17_05_init0( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 = 1 ;
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
	ef_char_set( ew, 27, 0 ) ;
}

static Uint8 pls17_05_ck( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls17_05_move0( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos = pl_w->pos ;
	if( ew->pl_type != pl_w->pl_type || HI_BYTE( pl_w->cg_rival ) == 1 ){
		ew->disp_flag = 0 ;
		pls17_05_free( ew, pl_w ) ;
		return ;
	}
	if( pls17_05_ck( ew ) ){
		ew->disp_flag = 0 ;
		pls17_05_free( ew, pl_w ) ;
		return ;
	}
	ef_char_move( ew ) ;
}

static void pls17_05_init1( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls17_05_move1( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls17_05_main1( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls17_05_jmp1[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls17_05_init1, pls17_05_move1, pls17_05_end, pls17_05_free,
	} ;

	pls17_05_jmp1[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void pls17_05_init1( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 = 1 ;
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
	ef_char_set( ew, 27, 1 ) ;
}

/*--------------------------------------------------------------*/
static void pls17_05_move1( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ew->pl_type != pl_w->pl_type ){
		pls17_05_free( ew, pl_w ) ;
		return ;
	}
	if( pls17_05_ck( ew ) ){
		ew->disp_flag = 0 ;
		pls17_05_free( ew, pl_w ) ;
		return ;
	}
	if( ew->cg_type == 0 ) ew->pos = pl_w->pos ;
	if( HI_BYTE( pl_w->cg_rival ) ){
		if( HI_BYTE( pl_w->cg_rival ) == 9 ){
			ew->disp_flag = 0 ;
			pls17_05_free( ew, pl_w ) ;
			return ;
		}
		ef_char_set2( ew, 27, 1, HI_BYTE( pl_w->cg_rival ) - 1 ) ;
		HI_BYTE( pl_w->cg_rival ) = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void pls17_05_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls17_05_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
static Uint8 pls17_05_ck( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->char_no.b.grp != Char_Special || pl_w->act_st != N_SPECIAL || pl_w->routine_1 ) return 1 ;
	return 0 ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls17_05.c" End of File	*/

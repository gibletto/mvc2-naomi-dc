/* Cable's player effects 00: three overlays that copy his sprite and show it while an animation of his lasts. */
#include "player.h"
#include "ef_char.h"
#include "pl_char.h"
#include "pl17_def.h"

/* the effect's own work, from obj_flag: the animation of his it follows, and the one it shows */
typedef struct PLS17_00_LOCAL_W {
	Uint16	char_no ;
	Uint8	anim_no ;
} PLS17_00_LOCAL_W;
#define PLS17_00_LW( w )	( ( PLS17_00_LOCAL_W * )&( w )->obj_flag )

static void pls17_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls17_00_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls17_00_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->disp_dir = ew->master_w->disp_dir ;
		ew->ef_no = 0x1700 ;
		PLS17_00_LW( ew )->char_no = ew->master_w->char_no.w ;
		return ew ;
	}
}

static void pls17_00_init( PLAYER_WORK *ew );
static void pls17_00_move( PLAYER_WORK *ew );
static void pls17_00_end( PLAYER_WORK *ew );
static void pls17_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls17_00_main( PLAYER_WORK *ew )
{
	static void ( *pls17_00_jmp[] )( PLAYER_WORK *ew ) = {
		pls17_00_init, pls17_00_move, pls17_00_end, pls17_00_free,
	} ;

	pls17_00_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls17_00_init( PLAYER_WORK *ew )
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
	ew->prio = 10 ;
	if( ew->type ) ef_char_set( ew, 23, 4 ) ;
	else ef_char_set( ew, 23, 3 ) ;
}

/*--------------------------------------------------------------*/
static void pls17_00_move( PLAYER_WORK *ew )
{
	ew->pos = ew->master_w->pos ;
	if( ew->pl_type != ew->master_w->pl_type ){
		pls17_00_free( ew ) ;
		return ;
	}
	if( PLS17_00_LW( ew )->char_no != ew->master_w->char_no.w ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
	}
	pl_char_move( ew ) ;
	if( ew->cg_type ) ew->alpha = 0.5f ;
	else ew->alpha = 1.0f ;
}

/*--------------------------------------------------------------*/
static void pls17_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls17_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

static void pls17_00_main2( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls17_00_set2( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls17_00_main2 ;
		ew->master_w = pl_w ;
		ew->disp_dir = ew->master_w->disp_dir ;
		ew->ef_no = 0x1700 ;
		ew->type = no ;
		PLS17_00_LW( ew )->char_no = ew->master_w->char_no.w ;
		return ew ;
	}
}

static Uint8 pls17_00_ck( PLAYER_WORK *ew );
static void pls17_00_init2( PLAYER_WORK *ew );
static void pls17_00_move2( PLAYER_WORK *ew );

/*==============================================================*/
static void pls17_00_main2( PLAYER_WORK *ew )
{
	static void ( *pls17_00_jmp2[] )( PLAYER_WORK *ew ) = {
		pls17_00_init2, pls17_00_move2, pls17_00_end, pls17_00_free,
	} ;

	if( pls17_00_ck( ew ) ){
		pls17_00_free( ew ) ;
		return ;
	}
	pls17_00_jmp2[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls17_00_init2( PLAYER_WORK *ew )
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
	ew->pos = pl_w->pos ;
	ew->prio = pl_w->prio ;
	ew->sub_prio = -8 ;
	if( ew->type ) ef_char_set( ew, 27, 20 ) ;
	else ef_char_set( ew, 27, 17 ) ;
	pls17_00_move2( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls17_00_move2( PLAYER_WORK *ew )
{
	if( ew->pl_type != ew->master_w->pl_type ){
		pls17_00_free( ew ) ;
		return ;
	}
	ew->prio = ew->master_w->prio ;
	ew->pos = ew->master_w->pos ;
	if( HI_BYTE( ew->master_w->cg_rival ) == 9 ){
		pls17_00_free( ew ) ;
		return ;
	}
	if( HI_BYTE( ew->master_w->cg_rival ) ){
		ef_char_set2( ew, 27, ew->char_no.b.no, HI_BYTE( ew->master_w->cg_rival ) - 1 ) ;
		HI_BYTE( ew->master_w->cg_rival ) = 0 ;
	}
}

static void pls17_00_main3( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls17_00_set3( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS_ST, LIST_TOP ) ) ){
		ew->work_func = pls17_00_main3 ;
		ew->master_w = pl_w ;
		ew->disp_dir = ew->master_w->disp_dir ;
		ew->ef_no = 0x1700 ;
		ew->type = no ;
		PLS17_00_LW( ew )->char_no = ew->master_w->char_no.w ;
		return ew ;
	}
}

static void pls17_00_init3( PLAYER_WORK *ew );
static void pls17_00_move3( PLAYER_WORK *ew );

/*==============================================================*/
static void pls17_00_main3( PLAYER_WORK *ew )
{
	static void ( *pls17_00_jmp3[] )( PLAYER_WORK *ew ) = {
		pls17_00_init3, pls17_00_move3, pls17_00_end, pls17_00_free,
	} ;

	pls17_00_jmp3[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls17_00_init3( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
	ew->routine_1 = 0 ;
	ew->disp_flag = 0 ;
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
	ew->pos = pl_w->pos ;
	ew->prio = pl_w->prio ;
	ew->sub_prio = -1 ;
	ew->dir = 0 ;
	if( !ew->type ) PLS17_00_LW( ew )->anim_no = 23 ;
	else PLS17_00_LW( ew )->anim_no = 26 ;
	pls17_00_move3( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls17_00_move3( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->prio = ew->master_w->prio ;
	ew->pos = ew->master_w->pos ;
	if( ew->pl_type != ew->master_w->pl_type ){
		pls17_00_free( ew ) ;
		return ;
	}
	if( ew->master_w->char_no.b.grp != Char_Super || ( ew->master_w->cg_flag1 & 0x80 ) ){
		work_free( ew ) ;
	}
	else if( ew->master_w->cg_flag1 != ew->dir ){
		ew->dir = ew->master_w->cg_flag1 ;
		ew->disp_flag = 1 ;
		if( pl_w->team_no && ew->cg_type ) ew->disp_flag = 0 ;
		ef_char_set2( ew, 27, PLS17_00_LW( ew )->anim_no, ew->dir - 1 ) ;
	}
}

/*==============================================================*/
static Uint8 pls17_00_ck( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->pl_sp_type != 4 || pl_w->char_no.b.grp != Char_Special || pl_w->act_st != N_SPECIAL || pl_w->routine_1 ) return 1 ;
	return 0 ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls17_00.c" End of File	*/

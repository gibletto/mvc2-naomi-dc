/* Cable's player effect 06: copies of his sprite that fade in and out around him, and the sparks that fly off him. */
#include "player.h"
#include "ef_char.h"
#include "pl_char.h"
#include "pl17_def.h"

#define DEG_ANG( d )	( ( Sint32 )( ( d ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

/* the effect's own work, from obj_flag: the animation of his it follows */
typedef struct PLS17_06_LOCAL_W {
	Uint32	char_no ;
} PLS17_06_LOCAL_W;
#define PLS17_06_LW( w )	( ( PLS17_06_LOCAL_W * )&( w )->obj_flag )

static void pls17_06_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls17_06_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls17_06_main ;
		ew->master_w = pl_w ;
		ew->disp_dir = ew->master_w->disp_dir ;
		ew->ef_no = 0x1706 ;
		PLS17_06_LW( ew )->char_no = ew->master_w->char_no.w ;
		ew->sub_no = no ;
		return ew ;
	}
}

static void pls17_06_main0( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls17_06_main1( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls17_06_main( PLAYER_WORK *ew )
{
	static void ( *pls17_06_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls17_06_main0, pls17_06_main0, pls17_06_main0, pls17_06_main1,
		pls17_06_main1, pls17_06_main1, pls17_06_main1, pls17_06_main0,
	} ;

	pls17_06_jmp[ew->sub_no]( ew, ew->master_w ) ;
}

static void pls17_06_init0( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls17_06_move0( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls17_06_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls17_06_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls17_06_main0( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls17_06_jmp0[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls17_06_init0, pls17_06_move0, pls17_06_end, pls17_06_free,
	} ;

	pls17_06_jmp0[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void pls17_06_init0( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->prio = pl_w->prio ;
	ew->sub_prio = -1 ;
	ew->pos = pl_w->pos ;
	ew->alpha = 0.0f ;
	ew->timer0 = 0 ;
	switch( ew->sub_no ){
	case 7:
	case 0:
		ef_char_set( ew, 27, 11 ) ;
		break ;
	case 1:
		ef_char_set( ew, 27, 12 ) ;
		break ;
	case 2:
		ef_char_set( ew, 27, 13 ) ;
		break ;
	}
}

/*--------------------------------------------------------------*/
static void pls17_06_move0( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ew->pl_type != pl_w->pl_type ){
		pls17_06_free( ew, pl_w ) ;
		return ;
	}
	if( ew->sub_no == 7 ){
		if( ew->master_w->master_w->routine_1 == 3 ){
			ew->routine_0 = 2 ;
			ew->disp_flag = 0 ;
			pls17_06_free( ew, pl_w ) ;
		}
	}
	else if( PLS17_06_LW( ew )->char_no != ew->master_w->char_no.w ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		pls17_06_free( ew, pl_w ) ;
	}
	ew->prio = pl_w->prio ;
	ew->timer0 ++ ;
	if( ew->sub_no != 7 ){
		ew->pos = pl_w->pos ;
		if( ew->timer0 < 2 ) ew->alpha += 0.35f ;
		else if( ew->timer0 < 4 ) ew->alpha = 0.7f ;
		else ew->alpha -= 0.04375f ;
	}
	else {
		ew->pos = pl_w->pos ;
		if( ew->timer0 < 2 ) ew->alpha += 0.25f ;
		else if( ew->timer0 < 4 ) ew->alpha = 0.5f ;
		else ew->alpha -= 0.03125f ;
	}
	if( HI_BYTE( pl_w->cg_rival ) == 3 ){
		ew->routine_0 = 3 ;
		ew->disp_flag = 0 ;
		pls17_06_free( ew, pl_w ) ;
		return ;
	}
	if( HI_BYTE( pl_w->cg_rival ) ){
		ef_char_set2( ew, 27, ew->char_no.b.no, HI_BYTE( pl_w->cg_rival ) - 1 ) ;
		HI_BYTE( pl_w->cg_rival ) = 0 ;
	}
}

static void pls17_06_init1( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls17_06_move1( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls17_06_main1( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls17_06_jmp1[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls17_06_init1, pls17_06_move1, pls17_06_end, pls17_06_free,
	} ;

	pls17_06_jmp1[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void pls17_06_init1( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->prio = 7 ;
	ew->pos = pl_w->pos ;
	ew->timer0 = 30 ;
	switch( ew->sub_no ){
	case 3:
		ew->pos.x += ew->disp_dir ? 65.0f : -65.0f ;
		ew->pos.y += 248.0f ;
		break ;
	case 4:
		ew->pos.x += ew->disp_dir ? 13.0f : -13.0f ;
		ew->pos.y += 258.0f ;
		break ;
	case 5:
		ew->pos.x += ew->disp_dir ? -13.0f : 13.0f ;
		ew->pos.y += 275.0f ;
		break ;
	case 6:
		ew->pos.x += ew->disp_dir ? 65.0f : -65.0f ;
		ew->pos.y += 248.0f ;
		ew->alpha = 0.7f ;
		break ;
	}
	ew->spd.x = ew->disp_dir ? -20.0f : 20.0f ;
	ew->acc.x = ew->disp_dir ? 0.2f : -0.2f ;
	ew->spd.y = 10.0f ;
	ew->acc.y = -1.0f ;
	ew->work1 = 120 ;
	if( ew->sub_no == 6 ) ew->alpha = 0.4f ;
	ef_char_set( ew, 27, 10 ) ;
}

/*--------------------------------------------------------------*/
static void pls17_06_move1( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	ew->rot[2] += DEG_ANG( -ew->work1 ) ;
	if( ew->pl_type != pl_w->pl_type ){
		pls17_06_free( ew, pl_w ) ;
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		return ;
	}
	if( -- ew->timer0 == 0 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		return ;
	}
	ef_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls17_06_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls17_06_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls17_06.c" End of File	*/

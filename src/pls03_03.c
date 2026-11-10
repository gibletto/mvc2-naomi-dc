/* Morrigan's effect 03 (character 03's pls03): a piece that comes in beside her, in three kinds:
   when she appears (type 0), in her taunt (type 1) and in one of her win poses (type 2). Each has
   its own steps. */
#include "player.h"
#include "pl_char.h"
#include "pl03_def.h"

static void pls03_03_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls03_03_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_END ) ) ){
		ew->work_func = pls03_03_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = type ;
		ew->ef_no = 0x303 ;
	}
	return ew ;
}

static void pls03_03_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_03_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_03_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_03_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls03_03_main( PLAYER_WORK *ew )
{
	static void ( *pls03_03_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls03_03_init, pls03_03_move, pls03_03_end, pls03_03_free,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->prio = pl_w->prio ;
	pls03_03_jmp[ew->routine_0]( ew, pl_w ) ;
}

static void pls03_03_appear_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_03_chouhatsu_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_03_win_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls03_03_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls03_03_init_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls03_03_appear_init, pls03_03_chouhatsu_init, pls03_03_win_init,
	} ;

	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
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
	pls03_03_init_jmp[ew->type]( ew, pl_w ) ;
	pl_shadow_set( ew ) ;
	ew->be_flag = 1 ;
}

/*==============================================================*/
static void pls03_03_appear_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 1 ;
	ew->pos.x = pl_w->pos.x + ( ew->pl_no ? -DOT_X( 64.0f ) : DOT_X( 64.0f ) ) ;
	ew->pos.y = pl_w->pos.y + DOT_Y( 256.0f ) ;
	BODY_SIZE_SET( ew, 0x20202424 ) ;
	ew->spd.y = -DOT_Y( 6.0f ) ;
	ew->acc.y = 0.13392857f ;
	pl_char_set( ew, 18, 3 ) ;
}

/*==============================================================*/
static void pls03_03_chouhatsu_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->disp_dir ) ew->pos.x = pl_w->pos.x - DOT_X( 56.0f ) ;
	else ew->pos.x = pl_w->pos.x + DOT_X( 56.0f ) ;
	ew->pos.y = pl_w->ground_y ;
	ew->spd.x = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = DOT_Y( 2.0f ) ;
	ew->acc.y = -DOT_Y( 0.25f ) ;
	BODY_SIZE_SET( ew, 0x60003030 ) ;
	pl_char_set( ew, 20, 6 ) ;
}

/*==============================================================*/
static void pls03_03_win_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ew->disp_dir ) ew->pos.x = pl_w->pos.x + DOT_X( 64.0f ) ;
	else ew->pos.x = pl_w->pos.x - DOT_X( 64.0f ) ;
	ew->pos.y = pl_w->pos.y + DOT_Y( 192.0f ) ;
	BODY_SIZE_SET( ew, 0x20202424 ) ;
	ew->spd.y = -DOT_Y( 6.0f ) ;
	ew->acc.y = 0.13392857f ;
	pl_char_set( ew, 19, 6 ) ;
}

static void pls03_03_appear_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_03_chouhatsu_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_03_win_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void pls03_03_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls03_03_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls03_03_appear_move, pls03_03_chouhatsu_move, pls03_03_win_move,
	} ;

	pls03_03_move_jmp[ew->type]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls03_03_appear_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y > pl_w->ground_y ){
		pl_char_move( ew ) ;
		return ;
	}
	ew->pos.y = pl_w->ground_y ;
	ew->routine_1 ++ ;
	pl_char_set( ew, 18, 4 ) ;
}

/*--------------------------------------------------------------*/
static void pls03_03_appear_land( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		ew->timer0 = 32 ;
		if( ew->pl_no ) ew->spd.x = DOT_X( 2.0f ) ;
		else ew->spd.x = -DOT_X( 2.0f ) ;
		ew->acc.x = 0.0f ;
	}
}

/*--------------------------------------------------------------*/
static void pls03_03_appear_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->routine_2 < 2 ) return ;
	ew->routine_1 ++ ;
	pl_char_set( ew, 18, 5 ) ;
}

/*--------------------------------------------------------------*/
static void pls03_03_appear_off( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Sint32 pls03_03_col[] = { 0x42, 0x4b, 0x54, 0x5d, 0x66, 0x6f } ;

	pl_char_move( ew ) ;
	if( ew->cg_prm ){
		ew->cg_prm = 0 ;
		ef_color_set( pl_w, 1, pls03_03_col[pl_w->pl_color] + ew->cg_flag1, 1 ) ;
		if( !( -- ew->timer0 ) ) ew->routine_0 ++ ;
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
	}
}

/*==============================================================*/
static void pls03_03_appear_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls03_03_appear_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls03_03_appear_fall, pls03_03_appear_land, pls03_03_appear_wait, pls03_03_appear_off,
	} ;

	ew->sub_prio = 4 ;
	pls03_03_appear_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls03_03_chouhatsu_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( ew->cg_prm ){
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		if( ew->pos.y < pl_w->ground_y ){
			ew->pos.y = pl_w->ground_y ;
			ew->routine_1 ++ ;
			pl_char_set( ew, 20, 7 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void pls03_03_chouhatsu_land( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) >= 0 ) return ;
	ew->routine_1 ++ ;
	ew->spd.x = -DOT_X( 8.0f ) ;
	ew->acc.x = 0.0f ;
	ew->spd.y = DOT_Y( 4.0f ) ;
	ew->acc.y = 0.0f ;
	if( !ew->disp_dir ) ew->spd.x = -ew->spd.x ;
	pl_char_set( ew, 20, 8 ) ;
}

/*--------------------------------------------------------------*/
static void pls03_03_chouhatsu_off( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	if( !scr_in_ck( ew ) ){
		ew->disp_flag = 0 ;
		ew->routine_0 ++ ;
	}
}

/*==============================================================*/
static void pls03_03_chouhatsu_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls03_03_chouhatsu_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls03_03_chouhatsu_fall, pls03_03_chouhatsu_land, pls03_03_chouhatsu_off,
	} ;

	ew->sub_prio = -4 ;
	pls03_03_chouhatsu_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls03_03_win_rise( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( !ew->cg_type ) ew->routine_1 ++ ;
	ew->pos.y -= DOT_Y( 5.0f ) ;
}

/*--------------------------------------------------------------*/
static void pls03_03_win_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
}

/*==============================================================*/
static void pls03_03_win_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls03_03_win_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls03_03_win_rise, pls03_03_win_wait,
	} ;

	ew->sub_prio = 4 ;
	pls03_03_win_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls03_03_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls03_03_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls03_03.c" End of File	*/

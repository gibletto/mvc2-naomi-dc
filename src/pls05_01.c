/* Strider's player effect 01: a burst made of a main part (type 0), which stays at the
   player and, as its animation reaches a mark, throws out the pieces: two of type 1, two of
   type 2 and three pairs of type 3. The pieces ride out from it and are freed when they leave the
   screen. */
#include "player.h"
#include "pl_char.h"

static void pls05_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls05_01_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls05_01_main ;
		ew->master_w = pl_w ;
		ew->type = 0 ;
		ew->ef_no = 0x501 ;
	}
	return ew ;
}

/*==============================================================*/
/*			The pieces: the type, which of the table's			*/
/*			places and the side									*/
/*==============================================================*/
static PLAYER_WORK *pls05_01_part_set( PLAYER_WORK *ew )
{
	PLAYER_WORK *pw ;
	int i ;

	if( ( pw = work_get( 0, W_PLS, LIST_TOP ) ) ){
		pw->work_func = pls05_01_main ;
		pw->master_w = ew->master_w ;
		pw->ef_w = ew ;
		pw->type = 1 ;
		pw->sub_type = 0 ;
		pw->dir = 0 ;
		pw->ef_no = 0x501 ;
	}
	if( ( pw = work_get( 0, W_PLS, LIST_TOP ) ) ){
		pw->work_func = pls05_01_main ;
		pw->master_w = ew->master_w ;
		pw->ef_w = ew ;
		pw->type = 1 ;
		pw->sub_type = 1 ;
		pw->dir = 1 ;
		pw->ef_no = 0x501 ;
	}
	if( ( pw = work_get( 0, W_PLS, LIST_TOP ) ) ){
		pw->work_func = pls05_01_main ;
		pw->master_w = ew->master_w ;
		pw->ef_w = ew ;
		pw->type = 2 ;
		pw->sub_type = 0 ;
		pw->dir = 0 ;
		pw->ef_no = 0x501 ;
	}
	if( ( pw = work_get( 0, W_PLS, LIST_TOP ) ) ){
		pw->work_func = pls05_01_main ;
		pw->master_w = ew->master_w ;
		pw->ef_w = ew ;
		pw->type = 2 ;
		pw->sub_type = 0 ;
		pw->dir = 1 ;
		pw->ef_no = 0x501 ;
	}
	i = 0 ;
	while( i < 3 ){
		if( ( pw = work_get( 0, W_PLS, LIST_TOP ) ) ){
			pw->work_func = pls05_01_main ;
			pw->master_w = ew->master_w ;
			pw->ef_w = ew ;
			pw->type = 3 ;
			pw->sub_type = i ;
			pw->dir = 0 ;
			pw->ef_no = 0x501 ;
		}
		if( ( pw = work_get( 0, W_PLS, LIST_TOP ) ) ){
			pw->work_func = pls05_01_main ;
			pw->master_w = ew->master_w ;
			pw->ef_w = ew ;
			pw->type = 3 ;
			pw->sub_type = i ;
			pw->dir = 1 ;
			pw->ef_no = 0x501 ;
		}
		i ++ ;
	}
	return pw ;
}

static void pls05_01_init( PLAYER_WORK *ew );
static void pls05_01_move( PLAYER_WORK *ew );
static void pls05_01_end( PLAYER_WORK *ew );
static void pls05_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls05_01_main( PLAYER_WORK *ew )
{
	static void ( *pls05_01_jmp[] )( PLAYER_WORK *ew ) = {
		pls05_01_init, pls05_01_move, pls05_01_end, pls05_01_free,
	} ;

	pls05_01_jmp[ew->routine_0]( ew ) ;
}

static void pls05_01_init0( PLAYER_WORK *ew );
static void pls05_01_init1( PLAYER_WORK *ew );
static void pls05_01_init2( PLAYER_WORK *ew );
static void pls05_01_init3( PLAYER_WORK *ew );

/*==============================================================*/
static void pls05_01_init( PLAYER_WORK *ew )
{
	static void ( *pls05_01_init_jmp[] )( PLAYER_WORK *ew ) = {
		pls05_01_init0, pls05_01_init1, pls05_01_init2, pls05_01_init3,
	} ;

	ew->routine_0 ++ ;
	pls05_01_init_jmp[ew->type]( ew ) ;
}

/*==============================================================*/
static void pls05_01_init0( PLAYER_WORK *ew )
{
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
	pl_char_set( ew, Char_Effect, 12 ) ;
}

/*==============================================================*/
static void pls05_01_init1( PLAYER_WORK *ew )
{
	CHAR_PART_COPY( ew, ew->ef_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->ef_w->pl_no ;
	ew->pl_type = ew->ef_w->pl_type ;
	ew->scale.x = ew->ef_w->scale.x ;
	ew->scale.y = ew->ef_w->scale.y ;
	ew->atck_level = ew->ef_w->atck_level ;
	ew->work_no = ew->ef_w->work_no ;
	ew->col_cnt = ew->ef_w->col_cnt ;
	ew->scale = ew->ef_w->scale ;
	ew->prio = ew->ef_w->prio ;
	ew->disp_dir = ew->dir ;
	ew->prio = 12 ;
	ew->pos.x = ew->ef_w->pos.x + ( ew->disp_dir ? -DOT_X( 80.0f ) : DOT_X( 80.0f ) ) ;
	ew->pos.y = ew->ef_w->pos.y ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.x = ew->disp_dir ? -15.0f : 15.0f ;
	ew->size_top = 16 ;
	ew->size_bottom = 16 ;
	ew->size_front = 48 ;
	ew->size_back = 48 ;
	pl_char_set( ew, Char_Effect, 13 ) ;
}

/*==============================================================*/
static void pls05_01_init2( PLAYER_WORK *ew )
{
	static struct { Float x, spd ; } pls05_01_tbl2[] = {
		{ DOT_X( 80.0f ), DOT_X( 7.0f ) },
		{ DOT_X( 64.0f ), 10.0f },
	} ;

	CHAR_PART_COPY( ew, ew->ef_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->ef_w->pl_no ;
	ew->pl_type = ew->ef_w->pl_type ;
	ew->scale.x = ew->ef_w->scale.x ;
	ew->scale.y = ew->ef_w->scale.y ;
	ew->atck_level = ew->ef_w->atck_level ;
	ew->work_no = ew->ef_w->work_no ;
	ew->col_cnt = ew->ef_w->col_cnt ;
	ew->scale = ew->ef_w->scale ;
	ew->prio = ew->ef_w->prio ;
	ew->disp_dir = ew->dir ;
	ew->prio = 12 ;
	ew->pos.x = ew->ef_w->pos.x + ( ew->disp_dir ? pls05_01_tbl2[ew->sub_type].x : -pls05_01_tbl2[ew->sub_type].x ) ;
	ew->pos.y = ew->ef_w->pos.y ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.x = ew->disp_dir ? pls05_01_tbl2[ew->sub_type].spd : -pls05_01_tbl2[ew->sub_type].spd ;
	ew->size_top = 16 ;
	ew->size_bottom = 16 ;
	ew->size_front = 48 ;
	ew->size_back = 48 ;
	pl_char_set( ew, Char_Effect, 14 ) ;
}

/*==============================================================*/
static void pls05_01_init3( PLAYER_WORK *ew )
{
	static struct { Float x, y, spd ; } pls05_01_tbl3[] = {
		{ -DOT_X( 16.0f ), DOT_Y( 48.0f ), -DOT_X( 4.0f ) },
		{ -DOT_X( 32.0f ), DOT_Y( 24.0f ), -DOT_X( 5.0f ) },
		{ -DOT_X( 32.0f ), DOT_Y( 24.0f ), -DOT_X( 5.0f ) },
		{ -DOT_X( 16.0f ), DOT_Y( 48.0f ), -DOT_X( 4.0f ) },
	} ;

	CHAR_PART_COPY( ew, ew->ef_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->ef_w->pl_no ;
	ew->pl_type = ew->ef_w->pl_type ;
	ew->scale.x = ew->ef_w->scale.x ;
	ew->scale.y = ew->ef_w->scale.y ;
	ew->atck_level = ew->ef_w->atck_level ;
	ew->work_no = ew->ef_w->work_no ;
	ew->col_cnt = ew->ef_w->col_cnt ;
	ew->scale = ew->ef_w->scale ;
	ew->prio = ew->ef_w->prio ;
	ew->disp_dir = ew->dir ;
	ew->prio = 12 ;
	ew->pos.x = ew->ef_w->pos.x + ( ew->disp_dir ? -pls05_01_tbl3[ew->sub_type].x : pls05_01_tbl3[ew->sub_type].x ) ;
	ew->pos.y = ew->ef_w->pos.y + pls05_01_tbl3[ew->sub_type].y ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.x = ew->disp_dir ? pls05_01_tbl3[ew->sub_type].spd : -pls05_01_tbl3[ew->sub_type].spd ;
	ew->size_top = 16 ;
	ew->size_bottom = 16 ;
	ew->size_front = 48 ;
	ew->size_back = 48 ;
	pl_char_set( ew, Char_Effect, 15 ) ;
}

static void pls05_01_move0( PLAYER_WORK *ew );
static void pls05_01_move1( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls05_01_move( PLAYER_WORK *ew )
{
	static void ( *pls05_01_move_jmp[] )( PLAYER_WORK *ew ) = {
		pls05_01_move0, pls05_01_move1, pls05_01_move1, pls05_01_move1,
	} ;

	pls05_01_move_jmp[ew->type]( ew ) ;
}

/*==============================================================*/
/*			The main part throws the pieces out when its		*/
/*			animation asks										*/
/*==============================================================*/
static void pls05_01_move0( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ){
		pls05_01_end( ew ) ;
		return ;
	}
	if( ew->cg_type ){
		ew->cg_type = 0 ;
		pls05_01_part_set( ew ) ;
		pl_voice_req( ew, 32 ) ;
	}
}

/*==============================================================*/
static void pls05_01_move1( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	pl_char_move( ew ) ;
	if( !scr_in_x_ck( ew ) ) pls05_01_end( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls05_01_end( PLAYER_WORK *ew )
{
	ew->routine_0 = 3 ;
	ew->disp_flag = 0 ;
	pls05_01_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls05_01_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls05_01.c" End of File	*/

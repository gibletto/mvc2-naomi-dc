/* Abyss's second form (character 19) player effects, 16 kinds by type: the sparks where he is hit, and the
   marks that ride on him or on the screen while he does his specials, each placed by pls19_00_tbl. */
#include "player.h"
#include "ef_char.h"
#include "pl_char.h"
#include "pl19_def.h"

/* a kind's place beside its master (the x and y in 3/5 pixels), its draw priority and its animation */
typedef struct PLS19_00_TBL {
	Sint16	x;
	Sint16	y;
	Sint8	prio;
	Uint8	no;
} PLS19_00_TBL;

static PLS19_00_TBL pls19_00_tbl[] = {
	{ 0, 0, 8, 0 }, { 0, 0, 8, 0 }, { 0, 0, 8, 0 }, { -41, 83, 0, 2 },
	{ -41, 83, 0, 3 }, { -41, 83, 0, 4 }, { -41, 83, 0, 5 }, { 39, 127, 7, 6 },
	{ 112, 101, 7, 6 }, { 169, 65, 7, 6 }, { 171, 39, 0, 6 }, { 38, 85, 0, 6 },
	{ 10, 112, 0, 7 }, { 107, 52, 0, 7 }, { 0, 0, 0, 11 }, { 0, 0, 0, 12 },
} ;

static void pls19_00_main( PLAYER_WORK *ew );

/****************************************************************/
/*			A spark at the place pos where he took a hit, at	*/
/*			most five at once									*/
/****************************************************************/
PLAYER_WORK *pls19_00_set2( PLAYER_WORK *pl_w, VEC3 *pos, Uint8 no )
{
	PLAYER_WORK *ew ;
	PL19_LOCAL_W *lw = PL19_LW( pl_w ) ;

	if( lw->ef_cnt >= 5 ) return 0 ;
	if( ( ew = work_get( 0, W_PLS_ST, LIST_TOP ) ) ){
		ew->work_func = pls19_00_main ;
		ew->master_w = pl_w ;
		ew->spd = *pos ;
		ew->type = no ;
		lw->ef_cnt ++ ;
	}
	return ew ;
}

/****************************************************************/
PLAYER_WORK *pls19_00_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls19_00_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
	}
	return ew ;
}

static void pls19_00_init( PLAYER_WORK *ew );
static void pls19_00_type( PLAYER_WORK *ew );
static void pls19_00_end( PLAYER_WORK *ew );
static void pls19_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls19_00_main( PLAYER_WORK *ew )
{
	static void ( *pls19_00_jmp[] )( PLAYER_WORK *ew ) = {
		pls19_00_init, pls19_00_type, pls19_00_end, pls19_00_free,
	} ;

	pls19_00_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls19_00_init( PLAYER_WORK *ew )
{
	PLS19_00_TBL *tbl ;
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->routine_0 ++ ;
	ew->ef_no = 0x1900 ;
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
	tbl = &pls19_00_tbl[ew->type] ;
	ew->prio = tbl->prio ;
	if( !ew->disp_dir ) ew->pos.x += DOT_X( tbl->x ) ;
	else ew->pos.x += -DOT_X( tbl->x ) ;
	ew->pos.y += DOT_Y( tbl->y ) ;
	ef_char_set( ew, 27, tbl->no ) ;
	pls19_00_type( ew ) ;
}

static void pls19_00_hit( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls19_00_beam_ready( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls19_00_beam_flash( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls19_00_beam_keep( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls19_00_storm( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls19_00_edge_l( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls19_00_edge_r( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void pls19_00_type( PLAYER_WORK *ew )
{
	static void ( *pls19_00_type_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls19_00_hit, pls19_00_hit, pls19_00_hit, pls19_00_beam_ready,
		pls19_00_beam_flash, pls19_00_beam_flash, pls19_00_beam_keep, pls19_00_storm,
		pls19_00_storm, pls19_00_storm, pls19_00_storm, pls19_00_storm,
		pls19_00_storm, pls19_00_storm, pls19_00_edge_l, pls19_00_edge_r,
	} ;

	pls19_00_type_jmp[ew->type]( ew, ew->master_w ) ;
}

static void pls19_00_hit_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls19_00_hit_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A spark at the place of a hit, in one of three		*/
/*			sizes												*/
/*==============================================================*/
static void pls19_00_hit( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls19_00_hit_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls19_00_hit_init, pls19_00_hit_move,
	} ;

	pls19_00_hit_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls19_00_hit_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Float pls19_00_scale_tbl[][2] = {
		{ 0.8f, 0.8f }, { 1.0f, 1.0f }, { 1.2f, 1.2f },
	} ;

	ew->routine_1 ++ ;
	ew->pos = ew->spd ;
	ew->scale.x = pls19_00_scale_tbl[ew->type][0] ;
	ew->scale.y = pls19_00_scale_tbl[ew->type][1] ;
	pls19_00_hit_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls19_00_hit_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL19_LOCAL_W *lw ;

	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		lw = PL19_LW( pl_w ) ;
		if( lw->ef_cnt ) lw->ef_cnt -- ;
	}
}

/*==============================================================*/
static void pls19_00_beam_ready( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS19_00_TBL *tbl ;
	int no ;

	if( pl_w->act_st != N_SPECIAL || pl_w->pl_sp_type != 2 || !pl_w->cg_prm ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
	else{
		no = pl_w->char_no.w ;
		if( no != 0x1506 && no != 0x1507 ){
			ew->routine_0 ++ ;
			ew->disp_flag = 0 ;
		}
		else{
			ef_char_move( ew ) ;
			tbl = &pls19_00_tbl[ew->type] ;
			if( !( ew->disp_dir = pl_w->disp_dir ) ) ew->pos.x = pl_w->pos.x + DOT_X( tbl->x ) ;
			else ew->pos.x = pl_w->pos.x - tbl->x * 1.6666666f ;
			ew->pos.y = pl_w->pos.y + DOT_Y( tbl->y ) ;
		}
	}
}

static void pls19_00_beam_flash_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls19_00_beam_flash_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls19_00_beam_flash( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	int no ;
	static void ( *pls19_00_beam_flash_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls19_00_beam_flash_init, pls19_00_beam_flash_move,
	} ;

	if( pl_w->act_st == N_SPECIAL && pl_w->pl_sp_type == 2 ){
		no = pl_w->char_no.w ;
		if( no != 0x1506 && no != 0x1507 ){
			ew->routine_0 ++ ;
			ew->disp_flag = 0 ;
			return ;
		}
	}
	else{
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	pls19_00_beam_flash_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls19_00_beam_flash_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_1 ++ ;
	ew->timer0 = 10 ;
	ew->scale.x = ew->scale.y = ew->alpha = 1.0f ;
	pls19_00_beam_flash_move( ew, pl_w ) ;
}

/*==============================================================*/
/*			Grows and fades while it follows him				*/
/*==============================================================*/
static void pls19_00_beam_flash_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS19_00_TBL *tbl ;

	ew->scale.x += 0.4f ;
	ew->scale.y += 0.25f ;
	ew->alpha += ( 0.0f - ew->alpha ) / ew->timer0 ;
	if( --ew->timer0 == 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	tbl = &pls19_00_tbl[ew->type] ;
	if( !( ew->disp_dir = pl_w->disp_dir ) ) ew->pos.x = pl_w->pos.x + DOT_X( tbl->x ) ;
	else ew->pos.x = pl_w->pos.x - tbl->x * 1.6666666f ;
	ew->pos.y = pl_w->pos.y + DOT_Y( tbl->y ) ;
}

/*==============================================================*/
static void pls19_00_beam_keep( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS19_00_TBL *tbl ;
	int no ;

	if( pl_w->act_st != N_SPECIAL || pl_w->pl_sp_type != 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
	else{
		no = pl_w->char_no.w ;
		if( no != 0x1506 && no != 0x1507 ){
			ew->routine_0 ++ ;
			ew->disp_flag = 0 ;
		}
		else{
			tbl = &pls19_00_tbl[ew->type] ;
			ef_char_set2( ew, 27, tbl->no, pl_w->cg_prm & 15 ) ;
			if( !( ew->disp_dir = pl_w->disp_dir ) ) ew->pos.x = pl_w->pos.x + DOT_X( tbl->x ) ;
			else ew->pos.x = pl_w->pos.x - tbl->x * 1.6666666f ;
			ew->pos.y = pl_w->pos.y + DOT_Y( tbl->y ) ;
		}
	}
}

/*==============================================================*/
/*			The eight marks that ride on him through his		*/
/*			anim 0x1504, each blinking now and then				*/
/*==============================================================*/
static void pls19_00_storm( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS19_00_TBL *tbl ;

	if( pl_w->act_st != N_SUPER || pl_w->pl_sp_type != 1 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( pl_w->char_no.w != 0x1504 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	tbl = &pls19_00_tbl[ew->type] ;
	if( !( ew->disp_dir = pl_w->disp_dir ) ) ew->pos.x = pl_w->pos.x + DOT_X( tbl->x ) ;
	else ew->pos.x = pl_w->pos.x - tbl->x * 1.6666666f ;
	ew->pos.y = pl_w->pos.y + DOT_Y( tbl->y ) ;
	if( ew->routine_2 == 0 ){
		if( !( Rnd() & 31 ) ) ew->routine_2 ++ ;
	}
	if( ew->routine_2 ){
		if( ef_char_move( ew ) < 0 ) ew->routine_2 = 0 ;
	}
}

/*==============================================================*/
/*			A mark at the screen's left edge while he lands		*/
/*==============================================================*/
static void pls19_00_edge_l( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->act_st != N_WIN ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->disp_flag = 1 ;
	ew->disp_dir = 1 ;
	ew->scale.x = 4.0f ;
	ew->scale.y = 0.8f ;
	ew->alpha = 0.75f ;
	ew->pos.x = scr_work.scr_left + -DOT_X( 64.0f ) ;
	ew->pos.y = pl_w->ground_y + 0.0f ;
	ef_char_move( ew ) ;
}

/*==============================================================*/
/*			And its twin at the right edge						*/
/*==============================================================*/
static void pls19_00_edge_r( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->act_st != N_WIN ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->disp_flag = 1 ;
	ew->disp_dir = 0 ;
	ew->scale.x = 3.5f ;
	ew->scale.y = 0.8f ;
	ew->alpha = 0.75f ;
	ew->pos.x = scr_work.scr_right + DOT_X( 32.0f ) ;
	ew->pos.y = pl_w->ground_y + 0.0f ;
	ef_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls19_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls19_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls19_00.c" End of File	*/

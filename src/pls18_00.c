/* Abyss's (character 18) player effects, 22 kinds by type: marks and pieces that ride on him or on the
   effect before them while he does a special move or super, each placed by pls18_00_tbl. */
#include "player.h"
#include "ef_char.h"
#include "pl_char.h"
#include "pl18_def.h"

/* a kind's place beside its master (the x and y in 3/5 pixels), its draw priority and its animation */
typedef struct PLS18_00_TBL {
	Sint16	x;
	Sint16	y;
	Sint8	prio;
	Uint8	no;
} PLS18_00_TBL;

static PLS18_00_TBL pls18_00_tbl[] = {
	{ 0, 0, 0, 0 }, { -40, 48, 0, 1 }, { 0, 0, 0, 4 }, { 0, 0, 0, 5 },
	{ -99, 93, 0, 6 }, { 0, 0, 0, 7 }, { 0, 0, 15, 9 }, { -36, 88, 7, 1 },
	{ 24, 112, 7, 1 }, { 0, 0, 0, 14 }, { 0, 0, 0, 10 }, { -32, 100, 0, 11 },
	{ -32, 100, 0, 12 }, { -64, 128, 0, 13 }, { 0, 0, 7, 16 }, { 0, 0, 0, 42 },
	{ 0, 0, 0, 43 }, { 85, 212, 10, 44 }, { 0, 0, 12, 45 }, { -136, 0, 10, 45 },
	{ 0, 0, 0, 47 }, { 0, 0, 0, 48 },
} ;

/* the places of the marks (types 15 and 20) on him, by their sub_no */
static Sint16 pls18_00_pos_tbl[][2] = {
	{ 16, 141 }, { 19, 107 }, { 16, 84 }, { -19, 31 }, { -24, 55 }, { 37, 25 }, { 25, 53 }, { -22, 73 },
	{ 20, 76 }, { 0, 154 }, { 33, 115 }, { -43, 112 }, { -33, 133 }, { 5, 142 },
} ;

static void pls18_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls18_00_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls18_00_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
	}
	return ew ;
}

/****************************************************************/
PLAYER_WORK *pls18_00_pillar_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS_ST, LIST_TOP ) ) ){
		ew->work_func = pls18_00_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
	}
	return ew ;
}

/****************************************************************/
PLAYER_WORK *pls18_00_idle_set( PLAYER_WORK *pl_w, PLAYER_WORK *ef_w, Uint8 type, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls18_00_main ;
		ew->master_w = pl_w ;
		ew->ef_w = ef_w ;
		ew->type = type ;
		ew->sub_no = no ;
	}
	return ew ;
}

static void pls18_00_init( PLAYER_WORK *ew );
static void pls18_00_type( PLAYER_WORK *ew );
static void pls18_00_end( PLAYER_WORK *ew );
static void pls18_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls18_00_main( PLAYER_WORK *ew )
{
	static void ( *pls18_00_jmp[] )( PLAYER_WORK *ew ) = {
		pls18_00_init, pls18_00_type, pls18_00_end, pls18_00_free,
	} ;

	pls18_00_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls18_00_init( PLAYER_WORK *ew )
{
	PLS18_00_TBL *tbl ;
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->routine_0 ++ ;
	ew->ef_no = 0x1800 ;
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
	tbl = &pls18_00_tbl[ew->type] ;
	ew->prio = tbl->prio ;
	if( !ew->disp_dir ) ew->pos.x = pl_w->pos.x + DOT_X( tbl->x ) ;
	else ew->pos.x = pl_w->pos.x - tbl->x * 1.6666666f ;
	ew->pos.y = pl_w->pos.y + DOT_Y( tbl->y ) ;
	ef_char_set( ew, 27, tbl->no ) ;
	pls18_00_type( ew ) ;
}

static void pls18_00_ball( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls18_00_idle( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls18_00_sweep( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls18_00_t3( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls18_00_rush( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls18_00_rush_frame( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls18_00_change( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls18_00_idle_fix( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls18_00_t9( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls18_00_pillar( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls18_00_copy( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls18_00_mark( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls18_00_t16( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls18_00_punch( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls18_00_punch_part( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls18_00_t19( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls18_00_pillar_frame( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void pls18_00_type( PLAYER_WORK *ew )
{
	static void ( *pls18_00_type_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls18_00_ball, pls18_00_idle, pls18_00_sweep, pls18_00_t3,
		pls18_00_rush, pls18_00_rush_frame, pls18_00_change, pls18_00_idle_fix,
		pls18_00_idle_fix, pls18_00_t9, pls18_00_pillar, pls18_00_pillar,
		pls18_00_pillar, pls18_00_pillar, pls18_00_copy, pls18_00_mark,
		pls18_00_t16, pls18_00_punch, pls18_00_punch_part, pls18_00_t19,
		pls18_00_mark, pls18_00_pillar_frame,
	} ;

	pls18_00_type_jmp[ew->type]( ew, ew->master_w ) ;
}

/*==============================================================*/
/*			The one of his anim 0x1502 that follows its			*/
/*			frames												*/
/*==============================================================*/
static void pls18_00_ball( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS18_00_TBL *tbl ;

	if( pl_w->act_st != N_SPECIAL || pl_w->pl_sp_type != 2 || pl_w->char_no.w != 0x1502 || pl_w->cg_prm == 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
	else{
		tbl = &pls18_00_tbl[ew->type] ;
		if( !( ew->disp_dir = pl_w->disp_dir ) ) ew->pos.x = pl_w->pos.x + DOT_X( tbl->x ) ;
		else ew->pos.x = pl_w->pos.x - tbl->x * 1.6666666f ;
		ew->pos.y = pl_w->pos.y + DOT_Y( tbl->y ) ;
		ef_char_set2( ew, 27, tbl->no, pl_w->cg_prm & 15 ) ;
	}
}

static void pls18_00_idle_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls18_00_in( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls18_00_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls18_00_out( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The pieces that rise, wait and fade beside him,		*/
/*			one of three animations picked at random			*/
/*==============================================================*/
static void pls18_00_idle( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls18_00_idle_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls18_00_idle_init, pls18_00_in, pls18_00_wait, pls18_00_out,
	} ;

	pls18_00_idle_jmp[ew->routine_1]( ew, pl_w ) ;
	if( !( ew->disp_dir = pl_w->disp_dir ) ) ew->pos.x = pl_w->pos.x + ew->spd.x ;
	else ew->pos.x = pl_w->pos.x - ew->spd.x ;
	ew->pos.y = pl_w->pos.y + ew->spd.y ;
	ew->prio = pl_w->prio ;
}

/*==============================================================*/
static void pls18_00_idle_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS18_00_TBL *tbl ;

	ew->routine_1 ++ ;
	ew->sub_prio = -1 ;
	ew->col_flag = -1 ;
	ew->col_no = col_no_tbl[pl_w->work_no] ;
	ew->timer0 = ( Rnd() & 31 ) + 60 ;
	ew->alpha = 0.0f ;
	ew->spd.z = 0.0125f ;
	tbl = &pls18_00_tbl[ew->type] ;
	ew->spd.x = ( tbl->x + ( Sint16 )( Rnd() & 63 ) ) * 1.6666666f ;
	ew->spd.y = ( tbl->y + ( Sint16 )( Rnd() & 63 ) ) * 2.142857f ;
	tbl = &pls18_00_tbl[ew->type] ;
	ef_char_set2( ew, 27, tbl->no + Rnd() % 3, Rnd() % 10 ) ;
	pls18_00_in( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls18_00_in( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ef_char_move( ew ) ;
	if( pl_w->act_st ){
		ew->routine_1 = 3 ;
		ew->spd.z = 0.05f ;
		return ;
	}
	ew->alpha += ew->spd.z ;
	if( ew->alpha >= 0.3f ){
		ew->routine_1 ++ ;
		ew->alpha = 0.3f ;
	}
}

/*--------------------------------------------------------------*/
static void pls18_00_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ef_char_move( ew ) ;
	if( pl_w->act_st ){
		ew->spd.z = 0.05f ;
		ew->routine_1 ++ ;
	}
	else if( --ew->timer0 == 0 ){
		ew->spd.z = 0.0125f ;
		ew->routine_1 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void pls18_00_out( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ef_char_move( ew ) ;
	ew->alpha -= ew->spd.z ;
	if( ew->alpha <= 0.0f ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		if( ew->ef_w->work_kind == W_PLS && ew->ef_w->ef_no == 0x1802 ){
			ew->ef_w->sub_no &= ~ew->sub_no ;
			if( ew->type == 7 ) ew->ef_w->sub_no &= 0xfe ;
			else if( ew->type == 8 ) ew->ef_w->sub_no &= 0xfe ;
		}
	}
}

static void pls18_00_idle_fix_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls18_00_idle_fix( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls18_00_idle_fix_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls18_00_idle_fix_init, pls18_00_in, pls18_00_wait, pls18_00_out,
	} ;

	pls18_00_idle_fix_jmp[ew->routine_1]( ew, pl_w ) ;
	if( !( ew->disp_dir = pl_w->disp_dir ) ) ew->pos.x = pl_w->pos.x + ew->spd.x ;
	else ew->pos.x = pl_w->pos.x - ew->spd.x ;
	ew->pos.y = pl_w->pos.y + ew->spd.y ;
	ew->prio = pl_w->prio ;
}

/*==============================================================*/
static void pls18_00_idle_fix_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS18_00_TBL *tbl ;

	ew->routine_1 ++ ;
	ew->sub_prio = 1 ;
	ew->col_flag = -1 ;
	ew->col_no = col_no_tbl[pl_w->work_no] ;
	ew->timer0 = ( Rnd() & 31 ) + 60 ;
	ew->alpha = 0.0f ;
	ew->spd.z = 0.0125f ;
	tbl = &pls18_00_tbl[ew->type] ;
	ew->spd.x = DOT_X( tbl->x ) ;
	ew->spd.y = DOT_Y( tbl->y ) ;
	tbl = &pls18_00_tbl[ew->type] ;
	ef_char_set2( ew, 27, tbl->no + Rnd() % 3, Rnd() % 10 ) ;
	pls18_00_in( ew, pl_w ) ;
}

static void pls18_00_sweep_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls18_00_sweep_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls18_00_sweep_last( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls18_00_sweep( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls18_00_sweep_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls18_00_sweep_init, pls18_00_sweep_move, pls18_00_sweep_last,
	} ;

	if( pl_w->act_st != N_SPECIAL || pl_w->pl_sp_type != 5 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	pls18_00_sweep_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls18_00_sweep_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_1 ++ ;
	pls18_00_sweep_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls18_00_sweep_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS18_00_TBL *tbl ;

	ew->disp_dir = pl_w->disp_dir ;
	ew->pos = pl_w->pos ;
	if( !pl_w->cg_type ){
		ew->routine_1 ++ ;
		return ;
	}
	tbl = &pls18_00_tbl[ew->type] ;
	ef_char_set2( ew, 27, tbl->no, pl_w->cg_prm ) ;
}

/*--------------------------------------------------------------*/
static void pls18_00_sweep_last( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_dir = pl_w->disp_dir ;
	ew->pos = pl_w->pos ;
	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void pls18_00_t3( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static struct { Sint16 x ; Sint16 y ; Sint16 ang ; } pls18_00_t3_tbl[] = {
		{ -63, 203, 0 }, { -87, 168, 0xa00 }, { -112, 100, 0x4000 }, { -112, 102, 0x4000 },
	} ;

	if( !pl_w->cg_type || pl_w->act_st != N_WIN ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
	else{
		ef_char_move( ew ) ;
		if( !( ew->disp_dir = pl_w->disp_dir ) ) ew->pos.x = pl_w->pos.x + DOT_X( pls18_00_t3_tbl[pl_w->cg_prm].x ) ;
		else ew->pos.x = pl_w->pos.x - pls18_00_t3_tbl[pl_w->cg_prm].x * 1.6666666f ;
		ew->pos.y = pl_w->pos.y + DOT_Y( pls18_00_t3_tbl[pl_w->cg_prm].y ) ;
		ew->rot[2] = pls18_00_t3_tbl[pl_w->cg_prm].ang ;
		if( pl_w->cg_prm >= 2 ) ew->prio = 7 ;
	}
}

/*==============================================================*/
static void pls18_00_rush( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS18_00_TBL *tbl ;

	if( pl_w->act_st != N_SPECIAL || pl_w->pl_sp_type != 6 || pl_w->char_no.w != 0x1507 || !pl_w->cg_prm ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->scale.y = 1.35f ;
	tbl = &pls18_00_tbl[ew->type] ;
	if( !( ew->disp_dir = pl_w->disp_dir ) ) ew->pos.x = pl_w->pos.x + DOT_X( tbl->x ) ;
	else ew->pos.x = pl_w->pos.x - tbl->x * 1.6666666f ;
	ew->pos.y = pl_w->pos.y + DOT_Y( tbl->y ) ;
	ew->prio = pl_w->prio ;
	ew->sub_prio = -2 ;
	ef_char_move( ew ) ;
}

/*==============================================================*/
static void pls18_00_rush_frame( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS18_00_TBL *tbl ;

	if( pl_w->act_st != N_SPECIAL || pl_w->pl_sp_type != 6 || pl_w->char_no.w != 0x1507 || !pl_w->cg_prm ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->alpha = 0.5f ;
	ew->disp_dir = pl_w->disp_dir ;
	ew->pos = pl_w->pos ;
	ew->prio = pl_w->prio ;
	ew->sub_prio = -1 ;
	tbl = &pls18_00_tbl[ew->type] ;
	ef_char_set2( ew, 27, tbl->no, pl_w->cg_prm & 15 ) ;
}

static void pls18_00_change_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls18_00_change_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls18_00_change_time( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls18_00_change_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls18_00_change( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls18_00_change_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls18_00_change_init, pls18_00_change_wait, pls18_00_change_time, pls18_00_change_move,
	} ;

	pls18_00_change_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls18_00_change_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_1 ++ ;
	ew->disp_flag = 1 ;
	pls18_00_change_wait( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls18_00_change_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( vs_sys_work.vs_st >= VS_END_WAIT ) return ;
	ew->routine_1 ++ ;
	ew->timer0 = 300 ;
}

/*--------------------------------------------------------------*/
static void pls18_00_change_time( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( --ew->timer0 == 0 ){
		ew->routine_1 ++ ;
		pls18_00_change_move( ew, pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls18_00_change_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void pls18_00_t9( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS18_00_TBL *tbl ;

	if( !pl_w->cg_type || pl_w->routine_0 > 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( ew->routine_1 == 0 ){
		ew->routine_1 ++ ;
		ew->alpha = 1.0f ;
		ew->timer0 = 10 ;
	}
	else{
		ew->alpha += ( 0.0f - ew->alpha ) / ew->timer0 ;
		if( --ew->timer0 == 0 ){
			ew->routine_0 ++ ;
			ew->disp_flag = 0 ;
			return ;
		}
	}
	ew->disp_dir = pl_w->disp_dir ;
	ew->pos = pl_w->pos ;
	tbl = &pls18_00_tbl[ew->type] ;
	ef_char_set2( ew, 27, tbl->no, pl_w->cg_prm ) ;
}

/*==============================================================*/
static void pls18_00_pillar( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS18_00_TBL *tbl ;

	if( pl_w->act_st != N_SUPER || pl_w->pl_sp_type != 3 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( vs_sys_work.game_stop & ( 1 << ( pl_w->pl_no ^ 1 ) ) ) return ;
	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	tbl = &pls18_00_tbl[ew->type] ;
	if( !( ew->disp_dir = pl_w->disp_dir ) ) ew->pos.x = pl_w->pos.x + DOT_X( tbl->x ) ;
	else ew->pos.x = pl_w->pos.x - tbl->x * 1.6666666f ;
	ew->pos.y = pl_w->pos.y + DOT_Y( tbl->y ) ;
}

static void pls18_00_copy_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls18_00_copy_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls18_00_copy_fade( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls18_00_copy( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls18_00_copy_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls18_00_copy_init, pls18_00_copy_wait, pls18_00_copy_fade,
	} ;

	pls18_00_copy_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
/*			A copy of the effect it rides on, drawn just		*/
/*			behind it											*/
/*==============================================================*/
static void pls18_00_copy_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *bw = ew->ef_w ;

	ew->routine_1 ++ ;
	ew->disp_flag = 1 ;
	ew->alpha = 1.0f ;
	ew->timer0 = 5 ;
	ew->sub_prio = bw->sub_prio - 1 ;
	ef_char_set( ew, 27, bw->char_no.b.no ) ;
	pls18_00_copy_wait( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls18_00_copy_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( --ew->timer0 == 0 ){
		ew->routine_1 ++ ;
		ew->timer0 = 10 ;
	}
}

/*--------------------------------------------------------------*/
static void pls18_00_copy_fade( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->alpha -= 0.1f ;
	if( ew->alpha <= 0.0f ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

static void pls18_00_mark_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls18_00_mark_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls18_00_mark( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls18_00_mark_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls18_00_mark_init, pls18_00_mark_move,
	} ;

	pls18_00_mark_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls18_00_mark_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_1 ++ ;
	ew->disp_flag = 1 ;
	ew->alpha = 1.0f ;
	if( ew->disp_dir == 0 ) ew->pos.x = pl_w->pos.x + DOT_X( pls18_00_pos_tbl[ew->sub_no][0] ) ;
	else ew->pos.x = pl_w->pos.x - pls18_00_pos_tbl[ew->sub_no][0] * 1.6666666f ;
	ew->pos.y = pl_w->pos.y + DOT_Y( pls18_00_pos_tbl[ew->sub_no][1] ) ;
	pls18_00_mark_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls18_00_mark_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void pls18_00_t16( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->cg_prm != 1 || pl_w->act_st != N_WIN ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->disp_dir = pl_w->disp_dir ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
}

static void pls18_00_punch_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls18_00_punch_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls18_00_punch( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls18_00_punch_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls18_00_punch_init, pls18_00_punch_move,
	} ;

	pls18_00_punch_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls18_00_punch_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *cw ;

	ew->routine_1 ++ ;
	ew->rot[2] = 0x2600 ;
	if( ( cw = work_get( ew, W_PLS, LIST_BEHIND ) ) ){
		cw->work_func = pls18_00_main ;
		cw->master_w = ew ;
		cw->type = 18 ;
	}
	pls18_00_punch_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls18_00_punch_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
/*			Rides on the punch's piece (type 17)				*/
/*==============================================================*/
static void pls18_00_punch_part( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS18_00_TBL *tbl ;

	if( pl_w->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( ( Sint8 )( ew->disp_flag = pl_w->cg_type ) ){
		ew->rot[2] = pl_w->rot[2] ;
		tbl = &pls18_00_tbl[ew->type] ;
		ef_char_set2( ew, 27, tbl->no, pl_w->cg_prm ) ;
	}
}

static void pls18_00_t19_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls18_00_t19_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls18_00_t19( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls18_00_t19_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls18_00_t19_init, pls18_00_t19_move,
	} ;

	pls18_00_t19_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls18_00_t19_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_1 ++ ;
	ew->pos.y = pl_w->master_w->ground_y ;
	ew->scale.y = 1.5f ;
	pls18_00_t19_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls18_00_t19_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void pls18_00_pillar_frame( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS18_00_TBL *tbl ;

	if( pl_w->act_st != N_SUPER || pl_w->pl_sp_type != 3 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( pl_w->char_no.w != 0x1503 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->alpha = 0.5f ;
	ew->disp_flag = pl_w->cg_prm ? 1 : 0 ;
	tbl = &pls18_00_tbl[ew->type] ;
	ef_char_set2( ew, 27, tbl->no, pl_w->cg_prm & 15 ) ;
	if( !( ew->disp_dir = pl_w->disp_dir ) ) ew->pos.x = pl_w->pos.x + DOT_X( tbl->x ) ;
	else ew->pos.x = pl_w->pos.x - tbl->x * 1.6666666f ;
	ew->pos.y = pl_w->pos.y + DOT_Y( tbl->y ) ;
}

/*--------------------------------------------------------------*/
static void pls18_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls18_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls18_00.c" End of File	*/

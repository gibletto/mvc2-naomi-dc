/* Abyss's effect 02 (character 18's shot 02): the two fists of pl18_sp_punch (types 0 and 1,
   which follow his animation and, at the right frame, make) the shock marks (types 2 and 3). */
#include "player.h"
#include "pl_char.h"
#include "pl18_def.h"

/* one fist or mark: where it is made from Abyss, its draw priority, its animation and its attack */
typedef struct SHL18_02_TBL {
	Sint16	x;
	Sint16	y;
	Uint8	prio;
	Uint8	unk_5;
	Uint8	char_no;
	Uint8	atck_no;
} SHL18_02_TBL;

/* where the fist is, by the frame of Abyss's animation */
typedef struct SHL18_02_POS {
	Sint16	x;
	Sint16	y;
	Sint16	rot;
} SHL18_02_POS;

static SHL18_02_TBL shl18_02_tbl[] = {
	{ 40, 176, 7, 0, 0, 48 },
	{ 40, 176, 7, 0, 0, 48 },
	{ -136, 0, 10, 0, 7, 60 },
	{ -136, 0, 10, 0, 7, 60 },
} ;

static void shl18_02_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl18_02_set( PLAYER_WORK *pl_w, PLAYER_WORK *em, Uint8 level )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl18_02_main ;
		ew->master_w = pl_w ;
		ew->ef_w = em ;
		ew->type = level ;
	}
	return ew ;
}

static void shl18_02_init( PLAYER_WORK *ew );
static void shl18_02_move( PLAYER_WORK *ew );
static void shl18_02_end( PLAYER_WORK *ew );
static void shl18_02_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl18_02_main( PLAYER_WORK *ew )
{
	static void ( *shl18_02_jmp[] )( PLAYER_WORK *ew ) = {
		shl18_02_init, shl18_02_move, shl18_02_end, shl18_02_free,
	} ;

	shl18_02_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl18_02_init( PLAYER_WORK *ew )
{
	const SHL18_02_TBL *tbl ;
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->routine_0 ++ ;
	ew->ef_no = 0x1802 ;
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
	ew->disp_flag = 1 ;
	tbl = &shl18_02_tbl[ew->type] ;
	ew->prio = tbl->prio ;
	if( ew->type == 0 || ew->type == 1 ) pl_w = ew->master_w ;
	else pl_w = ew->ef_w ;
	ew->pos = pl_w->pos ;
	if( !pl_w->disp_dir ) ew->pos.x += 1.6666666f * tbl->x ;
	else ew->pos.x += - ( 1.6666666f * tbl->x ) ;
	ew->pos.y += 2.142857f * tbl->y ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = tbl->atck_no ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, Char_Effect, tbl->char_no ) ;
	shl18_02_move( ew ) ;
}

static void shl18_02_fist( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl18_02_mark( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl18_02_move( PLAYER_WORK *ew )
{
	static void ( *shl18_02_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl18_02_fist, shl18_02_fist, shl18_02_mark, shl18_02_mark,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	shl18_02_move_jmp[ew->type]( ew, pl_w ) ;
}

static void shl18_02_fist_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl18_02_fist_hit( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl18_02_fist_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl18_02_fist( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl18_02_fist_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl18_02_fist_move, shl18_02_fist_hit, shl18_02_fist_end,
	} ;

	shl18_02_fist_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
/*			It rides Abyss's animation through his special		*/
/*			move, and sets the shock off at its frame			*/
/*==============================================================*/
static void shl18_02_fist_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static SHL18_02_POS shl18_02_pos_tbl[] = {
		{ 52, 177, 0x8800 },
		{ 53, 176, 0x8800 },
		{ 10, 181, 0x9400 },
		{ -102, 114, 0xa200 },
	} ;

	if( !pl_w->cg_type || pl_w->act_st != N_SPECIAL || pl_w->pl_sp_type ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( !( ew->disp_dir = pl_w->disp_dir ) ) ew->pos.x = pl_w->pos.x + 1.6666666f * shl18_02_pos_tbl[pl_w->cg_prm].x ;
	else ew->pos.x = pl_w->pos.x - 1.6666666f * shl18_02_pos_tbl[pl_w->cg_prm].x ;
	ew->pos.y = pl_w->pos.y + 2.142857f * shl18_02_pos_tbl[pl_w->cg_prm].y ;
	ew->rot[2] = shl18_02_pos_tbl[pl_w->cg_prm].rot ;
	if( pl_w->cg_type == 2 ){
		ew->routine_1 ++ ;
		ew->prio = 11 ;
		pls18_00_set( pl_w, 17 ) ;
		shl18_02_fist_hit( ew, pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl18_02_fist_hit( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( ew->cg_type ){
		ew->routine_1 ++ ;
		shl18_02_set( pl_w, ew, pl_w->atck_level + 2 ) ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl18_02_fist_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

static void shl18_02_mark_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl18_02_mark_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl18_02_mark( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl18_02_mark_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl18_02_mark_init, shl18_02_mark_move,
	} ;

	shl18_02_mark_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl18_02_mark_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_1 ++ ;
	ew->pos.y = pl_w->ground_y ;
	ew->scale.y = 1.5f ;
	shl18_02_mark_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl18_02_mark_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl18_02_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl18_02_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl18_02.c" End of File	*/

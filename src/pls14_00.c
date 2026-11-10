/* SonSon's player effect 00 (character 14's player effect 00): a copy of her sprite that rides on
   her during some of her animations (groups 5, 6, 11, 15 and 24) and follows them step by step. */
#include "player.h"
#include "ef_char.h"

/* where a piece of a kind rides on her, and its animation */
typedef struct PLS14_00_POS {
	Sint16	x ;
	Sint16	y ;
	Uint8	no ;
	Uint8	unk_05 ;
} PLS14_00_POS ;

/* where it moves to for her animation's code, and how it is turned */
typedef struct PLS14_00_OFS {
	Sint16	x ;
	Sint16	y ;
	Sint16	rot ;
} PLS14_00_OFS ;

const PLS14_00_POS pls14_00_pos_tbl[6] = {
	{ 21, 0, 0, 0 }, { 23, 0, 1, 0 }, { 25, 0, 2, 0 },
	{ 106, 137, 3, 0 }, { 23, 0, 0, 0 }, { 36, 0, 1, 0 }
} ;
const PLS14_00_OFS pls14_00_ofs_tbl[6] = {
	{ 30, 68, 0x6800 }, { 53, 102, 0x2800 }, { -20, 205, 0x2800 },
	{ -20, 205, 0x2800 }, { 0, 171, 0x2800 }, { 6, 154, 0x2800 }
} ;

/* the effect's own work: the animation it began with */
typedef struct PLS14_00_LOCAL_W {
	CHAR_NO	char_no ;
} PLS14_00_LOCAL_W ;
#define PLS14_00_LW( w )	( ( PLS14_00_LOCAL_W * )&( w )->obj_flag )

static void pls14_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls14_00_set( PLAYER_WORK *pl_w, int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls14_00_main ;
		ew->master_w = pl_w ;
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
		ew->ef_no = 0x1400 ;
		ew->size_top = ew->size_bottom = 32 ;
		ew->size_front = ew->size_back = 36 ;
		ew->prio = 0 ;
		ew->routine_0 = 0 ;
		ew->disp_flag = 0 ;
	}
	return ew ;
}

static void pls14_00_init( PLAYER_WORK *ew );
static void pls14_00_move( PLAYER_WORK *ew );
static void pls14_00_end( PLAYER_WORK *ew );
static void pls14_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls14_00_main( PLAYER_WORK *ew )
{
	static void ( *pls14_00_jmp[] )( PLAYER_WORK *ew ) = {
		pls14_00_init, pls14_00_move, pls14_00_end, pls14_00_free,
	} ;

	pls14_00_jmp[ew->routine_0]( ew ) ;
}

static void pls14_00_pos_set( PLAYER_WORK *pl_w, PLAYER_WORK *ew );
static void pls14_00_debug( PLAYER_WORK *ew );

/*==============================================================*/
static void pls14_00_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	PLS14_00_LOCAL_W *lw ;

	lw = PLS14_00_LW( ew ) ;
	pl_w = ew->master_w ;
	ew->rot[2] = 0 ;
	switch( pl_w->char_no.b.grp ){
	case 5:
		lw->char_no.w = pl_w->char_no.w ;
		lw->char_no.b.grp = pl_w->char_no.b.grp ;
		lw->char_no.b.no = pl_w->char_no.b.no ;
		ew->routine_0 ++ ;
		ew->routine_1 = 0 ;
		ew->disp_flag = 0 ;
		if( pl_w->char_no.b.no >= 4 ){
			ew->sub_type = 1 ;
		} else {
			ew->sub_type = 0 ;
		}
		break ;
	case 6:
		ew->sub_type = 2 ;
		lw->char_no.w = pl_w->char_no.w ;
		lw->char_no.b.grp = pl_w->char_no.b.grp ;
		lw->char_no.b.no = pl_w->char_no.b.no ;
		ew->routine_0 ++ ;
		ew->routine_1 = 0 ;
		ew->disp_flag = 0 ;
		break ;
	case 11:
		if( pl_w->char_no.b.no == 2 ){
			ew->sub_type = 3 ;
			lw->char_no.w = pl_w->char_no.w ;
			ew->routine_0 ++ ;
			ew->routine_1 = 3 ;
			ew->routine_2 = 0 ;
			ew->disp_flag = 0 ;
		}
		break ;
	case 24:
		if( pl_w->char_no.b.no == 1 ){
			ew->sub_type = 4 ;
			lw->char_no.w = pl_w->char_no.w ;
			lw->char_no.b.grp = pl_w->char_no.b.grp ;
			lw->char_no.b.no = pl_w->char_no.b.no ;
			ew->routine_0 ++ ;
			ew->routine_1 = 1 ;
			ew->disp_flag = 1 ;
			pls14_00_pos_set( pl_w, ew ) ;
			ef_char_set( ew, 27, pls14_00_pos_tbl[ew->sub_type].no ) ;
		}
		break ;
	case 15:
		if( pl_w->char_no.b.no == 5 ){
			ew->sub_type = 5 ;
			lw->char_no.w = pl_w->char_no.w ;
			lw->char_no.b.grp = pl_w->char_no.b.grp ;
			lw->char_no.b.no = pl_w->char_no.b.no ;
			ew->routine_0 ++ ;
			ew->routine_1 = 4 ;
		}
		break ;
	}
	pls14_00_debug( ew ) ;
}

static void pls14_00_guard( PLAYER_WORK *ew );
static void pls14_00_change( PLAYER_WORK *ew );
static void pls14_00_move2( PLAYER_WORK *ew );
static void pls14_00_jump_p( PLAYER_WORK *ew );
static void pls14_00_catch( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls14_00_move( PLAYER_WORK *ew )
{
	static void ( *pls14_00_move_jmp[] )( PLAYER_WORK *ew ) = {
		pls14_00_guard, pls14_00_change, pls14_00_move2, pls14_00_jump_p, pls14_00_catch,
	} ;

	pls14_00_move_jmp[ew->routine_1]( ew ) ;
	pls14_00_debug( ew ) ;
}

static void pls14_00_guard_wait( PLAYER_WORK *ew );
static void pls14_00_guard_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls14_00_guard( PLAYER_WORK *ew )
{
	static void ( *pls14_00_guard_jmp[] )( PLAYER_WORK *ew ) = {
		pls14_00_guard_wait, pls14_00_guard_move,
	} ;

	pls14_00_guard_jmp[ew->routine_2]( ew ) ;
}

/*==============================================================*/
static void pls14_00_guard_wait( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	PLS14_00_LOCAL_W *lw ;

	pl_w = ew->master_w ;
	lw = PLS14_00_LW( ew ) ;
	if( pl_w->cg_type ){
		ew->routine_2 ++ ;
		ew->disp_flag = 1 ;
		pls14_00_pos_set( pl_w, ew ) ;
		ef_char_set( ew, 27, pls14_00_pos_tbl[ew->sub_type].no ) ;
	}
	if( lw->char_no.w != pl_w->char_no.w ){
		ew->routine_2 = 0 ;
		ew->routine_1 = 0 ;
		ew->routine_0 = 0 ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void pls14_00_guard_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	PLS14_00_LOCAL_W *lw ;

	lw = PLS14_00_LW( ew ) ;
	pl_w = ew->master_w ;
	if( lw->char_no.b.no != pl_w->char_no.b.no || !pl_w->cg_type ){
		ew->routine_2 = 0 ;
		ew->routine_1 = 0 ;
		ew->routine_0 = 0 ;
		ew->disp_flag = 0 ;
		return ;
	}
	pls14_00_pos_set( pl_w, ew ) ;
	ew->prio = pl_w->prio ;
	ew->sub_prio = -1 ;
	if( !pl_w->hit_stop ){
		ef_char_move( ew ) ;
	}
	if( ew->cg_type ){
		ew->cg_type = 0 ;
		pls14_01_set( pl_w, ew->sub_type + 6, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls14_00_change( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	PLS14_00_LOCAL_W *lw ;

	lw = PLS14_00_LW( ew ) ;
	pl_w = ew->master_w ;
	if( lw->char_no.w != pl_w->char_no.w ){
		ew->routine_2 = 0 ;
		ew->routine_1 = 0 ;
		ew->routine_0 = 0 ;
		ew->disp_flag = 0 ;
		return ;
	}
	pls14_00_pos_set( pl_w, ew ) ;
	ew->prio = pl_w->prio ;
	ew->sub_prio = -1 ;
	ef_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls14_00_move2( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	PLS14_00_LOCAL_W *lw ;

	lw = PLS14_00_LW( ew ) ;
	pl_w = ew->master_w ;
	if( lw->char_no.w != pl_w->char_no.w ){
		ew->routine_1 = 0 ;
		ew->routine_0 = 0 ;
		ew->disp_flag = 0 ;
	}
}

static void pls14_00_jump_p_wait( PLAYER_WORK *ew );
static void pls14_00_jump_p_move( PLAYER_WORK *ew );
static void pls14_00_jump_p_end( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls14_00_jump_p( PLAYER_WORK *ew )
{
	static void ( *pls14_00_jump_p_jmp[] )( PLAYER_WORK *ew ) = {
		pls14_00_jump_p_wait, pls14_00_jump_p_move, pls14_00_jump_p_end,
	} ;

	pls14_00_jump_p_jmp[ew->routine_2]( ew ) ;
}

/*==============================================================*/
static void pls14_00_jump_p_wait( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	PLS14_00_LOCAL_W *lw ;

	pl_w = ew->master_w ;
	lw = PLS14_00_LW( ew ) ;
	if( pl_w->cg_type ){
		ew->routine_2 ++ ;
		ew->disp_flag = 1 ;
		pls14_00_pos_set( pl_w, ew ) ;
		ef_char_set( ew, 27, pls14_00_pos_tbl[ew->sub_type].no ) ;
	}
	if( lw->char_no.w != pl_w->char_no.w ){
		ew->routine_2 = 0 ;
		ew->routine_1 = 0 ;
		ew->routine_0 = 0 ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void pls14_00_jump_p_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	PLS14_00_LOCAL_W *lw ;

	lw = PLS14_00_LW( ew ) ;
	pl_w = ew->master_w ;
	if( lw->char_no.w != pl_w->char_no.w ){
		ew->routine_0 = ew->routine_1 = ew->routine_2 = 0 ;
		ew->disp_flag = 0 ;
	} else {
		pls14_00_pos_set( pl_w, ew ) ;
		ew->prio = 0 ;
		switch( pl_w->cg_type ){
		case 0:
		default:
			ew->routine_2 ++ ;
			ew->disp_flag = 0 ;
			break ;
		case 1:
			if( !pl_w->hit_stop ){
				ef_char_move( ew ) ;
			}
			break ;
		case 2:
			break ;
		}
	}
}

/*--------------------------------------------------------------*/
static void pls14_00_jump_p_end( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	PLS14_00_LOCAL_W *lw ;

	lw = PLS14_00_LW( ew ) ;
	pl_w = ew->master_w ;
	if( lw->char_no.w != pl_w->char_no.w ){
		ew->routine_2 = 0 ;
		ew->routine_1 = 0 ;
		ew->routine_0 = 0 ;
		ew->disp_flag = 0 ;
	}
}

static void pls14_00_catch_wait( PLAYER_WORK *ew );
static void pls14_00_catch_move( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls14_00_catch( PLAYER_WORK *ew )
{
	static void ( *pls14_00_catch_jmp[] )( PLAYER_WORK *ew ) = {
		pls14_00_catch_wait, pls14_00_catch_move,
	} ;
	PLAYER_WORK *pl_w ;
	PLS14_00_LOCAL_W *lw ;

	pl_w = ew->master_w ;
	lw = PLS14_00_LW( ew ) ;
	if( lw->char_no.b.grp != pl_w->char_no.b.grp ){
		ew->routine_2 = 0 ;
		ew->routine_1 = 0 ;
		ew->routine_0 = 0 ;
		ew->disp_flag = 0 ;
		return ;
	}
	pls14_00_catch_jmp[ew->routine_2]( ew ) ;
}

/*==============================================================*/
static void pls14_00_catch_wait( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	pl_w = ew->master_w ;
	if( pl_w->cg_type ){
		ew->routine_2 ++ ;
		ew->disp_flag = 1 ;
		pls14_00_pos_set( pl_w, ew ) ;
		ef_char_set( ew, 27, pl_w->cg_type + 35 ) ;
		pls14_00_pos_set( pl_w, ew ) ;
		ew->pos.x += pl_w->disp_dir ? pls14_00_ofs_tbl[pl_w->cg_type - 1].x : -pls14_00_ofs_tbl[pl_w->cg_type - 1].x ;
		ew->pos.y += pls14_00_ofs_tbl[pl_w->cg_type - 1].y ;
		ew->rot[2] = pls14_00_ofs_tbl[pl_w->cg_type - 1].rot ;
		ew->prio = pl_w->prio ;
		ew->sub_prio = -1 ;
	}
}

/*--------------------------------------------------------------*/
static void pls14_00_catch_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	pl_w = ew->master_w ;
	if( !pl_w->cg_type ){
		ew->routine_2 = 0 ;
		ew->routine_1 = 0 ;
		ew->routine_0 = 0 ;
		ew->disp_flag = 0 ;
		ew->rot[2] = 0 ;
	} else {
		ef_char_set( ew, 27, pl_w->cg_type + 35 ) ;
		pls14_00_pos_set( pl_w, ew ) ;
		ew->pos.x += pl_w->disp_dir ? pls14_00_ofs_tbl[pl_w->cg_type - 1].x : -pls14_00_ofs_tbl[pl_w->cg_type - 1].x ;
		ew->pos.y += pls14_00_ofs_tbl[pl_w->cg_type - 1].y ;
		ew->rot[2] = pls14_00_ofs_tbl[pl_w->cg_type - 1].rot ;
		if( pl_w->cg_type > 2 ){
			ew->prio = pl_w->prio ;
			ew->sub_prio = 1 ;
		} else {
			ew->sub_prio = -1 ;
		}
	}
}

/*--------------------------------------------------------------*/
static void pls14_00_end( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	pls14_00_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls14_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			Put the piece at his place, shifted by its			*/
/*			kind's offset										*/
/*==============================================================*/
static void pls14_00_pos_set( PLAYER_WORK *pl_w, PLAYER_WORK *ew )
{
	Float ofs ;

	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.y += pls14_00_pos_tbl[ew->sub_type].y ;
	ofs = pls14_00_pos_tbl[ew->sub_type].x ;
	if( !( ew->disp_dir = pl_w->disp_dir ) ){
		ofs = -ofs ;
	}
	ew->pos.x += ofs ;
}

/*==============================================================*/
static void pls14_00_debug( PLAYER_WORK *ew )
{
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls14_00.c" End of File	*/

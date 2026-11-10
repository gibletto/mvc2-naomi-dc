/* Colossus's player effect 00, six kinds by type: a piece at one of four places on him, now and then
   as he stands (0), the heavy punch's piece in front of him (1), the pairs that step with the Giant
   Swing's mashing (2 and 3), the kick throw's piece, which shows the frame his animation asks for (4),
   and one that only shakes the screen (5). */
#include "player.h"
#include "ef_char_old.h"
#include "pl_char.h"
#include "pl32_def.h"

/* the effect's own work, from obj_flag */
typedef struct PLS32_00_LOCAL_W {
	Uint16	char_no;	/* 00: his animation when it was made; the piece ends when it changes */
	Sint8	cnt;		/* 02: the step in the animation table */
	Sint8	timer;		/* 03: frames to the next step */
} PLS32_00_LOCAL_W;
#define PLS32_00_LW( w )	( ( PLS32_00_LOCAL_W * )&( w )->obj_flag )

/* type 0: its four places on him, x and y */
static Sint16 pls32_00_ofs_tbl[] = {
	-15, 111, -36, 54, 25, 89, 8, 49,
} ;

/* type 0: its animation, one of eight */
static Sint16 pls32_00_anim_tbl[8] = { 1, 2, 3, 4, 5, 6, 1, 2 } ;

/* types 2 and 3: frames to each step, by how hard the throw is mashed */
static Sint8 pls32_00_wait_tbl[8] = { 1, 1, 1, 1, 0, 0, 0, 0 } ;

static void pls32_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls32_00_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 kind )
{
	PLAYER_WORK *ew ;
	PLS32_00_LOCAL_W *lw ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls32_00_main ;
		ew->ef_no = 0x3200 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = type ;
		ew->sub_type = kind ;
		lw = PLS32_00_LW( ew ) ;
		lw->char_no = pl_w->char_no.w ;
	}
	return ew ;
}

static void pls32_00_idle( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls32_00_punch( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls32_00_mash( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls32_00_nml_k( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls32_00_quake( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls32_00_main( PLAYER_WORK *ew )
{
	static void ( *pls32_00_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls32_00_idle, pls32_00_punch, pls32_00_mash, pls32_00_mash, pls32_00_nml_k, pls32_00_quake,
	} ;

	pls32_00_jmp[ew->type]( ew, ew->master_w ) ;
}

static void pls32_00_next( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls32_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

static void pls32_00_idle_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls32_00_idle_play( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls32_00_idle_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls32_00_idle_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Uint8 i ;
	Sint16 x ;
	Uint8 no ;

	ew->routine_0 ++ ;
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
	ew->sub_prio = -1 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	i = Rnd() & 3 ;
	x = DOT_X( pls32_00_ofs_tbl[i * 2] ) ;
	if( ew->disp_dir ) x = -x ;
	ew->spd.x = x ;
	ew->spd.y += DOT_Y( pls32_00_ofs_tbl[i * 2 + 1] ) ;
	no = Rnd() & 7 ;
	ef_char_set( ew, 27, pls32_00_anim_tbl[no] ) ;
	pls32_00_idle_move( ew, pl_w ) ;
}

/*==============================================================*/
static void pls32_00_idle_play( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.x += ew->spd.x ;
	ew->pos.y += ew->spd.y ;
	if( ef_char_move( ew ) < 0 ){
		ew->disp_flag = 0 ;
		ew->routine_0 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void pls32_00_idle_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls32_00_idle_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls32_00_idle_play,
	} ;

	ew->prio = pl_w->prio ;
	if( pl_w->char_no.b.grp ){
		ew->routine_0 ++ ;
		return ;
	}
	pls32_00_idle_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls32_00_idle( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls32_00_idle_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls32_00_idle_init, pls32_00_idle_move, pls32_00_next, pls32_00_free,
	} ;

	pls32_00_idle_jmp[ew->routine_0]( ew, pl_w ) ;
}

static void pls32_00_punch_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls32_00_punch_play( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls32_00_punch_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls32_00_punch_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
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
	ew->sub_prio = -1 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	ew->spd.x = -DOT_X( 154.0f ) ;
	if( ew->disp_dir ) ew->spd.x = -ew->spd.x ;
	ew->spd.y = DOT_Y( 72.0f ) ;
	ef_char_set( ew, 27, 0 ) ;
	pls32_00_punch_move( ew, pl_w ) ;
}

/*==============================================================*/
static void pls32_00_punch_play( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.x += ew->spd.x ;
	ew->pos.y += ew->spd.y ;
	if( ef_char_move( ew ) < 0 ){
		ew->disp_flag = 0 ;
		ew->routine_0 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void pls32_00_punch_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls32_00_punch_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls32_00_punch_play,
	} ;
	PLS32_00_LOCAL_W *lw = PLS32_00_LW( ew ) ;

	ew->prio = pl_w->prio ;
	if( pl_w->char_no.w != lw->char_no ){
		ew->routine_0 ++ ;
		return ;
	}
	pls32_00_punch_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls32_00_punch( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls32_00_punch_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls32_00_punch_init, pls32_00_punch_move, pls32_00_next, pls32_00_free,
	} ;

	pls32_00_punch_jmp[ew->routine_0]( ew, pl_w ) ;
}

static void pls32_00_mash_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls32_00_mash_follow( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls32_00_mash_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/* this module saw ef_char_set with an int animation number (ef_char_old.h) */
static void pls32_00_mash_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS32_00_LOCAL_W *lw = PLS32_00_LW( ew ) ;

	ew->routine_0 ++ ;
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
	ew->scale.x = 1.5f ;
	ew->alpha = 0.5f ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	if( !ew->sub_type ){
		ew->char_no.b.no = 7 ;
	} else {
		ew->char_no.b.no = 8 ;
	}
	ef_char_set( ew, 27, ew->char_no.b.no ) ;
	ew->spd.x = 0.0f ;
	if( ew->disp_dir ) ew->spd.x = -ew->spd.x ;
	if( ew->type == 2 ){
		ew->spd.y = DOT_Y( 88.0f ) ;
	} else {
		ew->spd.y = DOT_Y( 107.0f ) ;
		ew->rot[2] = 0xa00 ;
	}
	lw->cnt = 0 ;
	lw->timer = 3 ;
	pls32_00_mash_move( ew, pl_w ) ;
}

/*==============================================================*/
static void pls32_00_mash_follow( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->hit_stop ){
		ew->disp_flag = 0 ;
	} else {
		ew->disp_flag = 1 ;
	}
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.x += ew->spd.x ;
	ew->pos.y += ew->spd.y ;
}

/*--------------------------------------------------------------*/
static void pls32_00_mash_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls32_00_mash_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls32_00_mash_follow,
	} ;
	PL32_LOCAL_W *pl_lw = ( PL32_LOCAL_W * )&pl_w->tlocal[0] ;
	PLS32_00_LOCAL_W *lw = PLS32_00_LW( ew ) ;

	if( !lw->timer -- ){
		lw->cnt ++ ;
		if( lw->cnt > 5 ) lw->cnt = 0 ;
		lw->timer = pls32_00_wait_tbl[pl_lw->mash_lv] ;
		if( !ew->sub_type ){
			ew->char_no.b.no = 7 ;
		} else {
			ew->char_no.b.no = 8 ;
		}
		ef_char_set2( ew, 27, ew->char_no.b.no, lw->cnt ) ;
	}
	if( !ew->sub_type ){
		ew->prio = 0 ;
	} else {
		ew->prio = 7 ;
	}
	if( pl_w->char_no.w != lw->char_no ){
		ew->routine_0 ++ ;
		return ;
	}
	pls32_00_mash_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls32_00_mash( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls32_00_mash_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls32_00_mash_init, pls32_00_mash_move, pls32_00_next, pls32_00_free,
	} ;

	pls32_00_mash_jmp[ew->routine_0]( ew, pl_w ) ;
}

static void pls32_00_nml_k_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls32_00_nml_k_frame( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls32_00_nml_k_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls32_00_nml_k_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
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
	ew->disp_flag = 0 ;
	ew->sub_prio = 1 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	pl_char_set( ew, 15, 5 ) ;
	pls32_00_nml_k_move( ew, pl_w ) ;
}

/*==============================================================*/
static void pls32_00_nml_k_frame( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	if( !pl_w->cg_prm ){
		ew->disp_flag = 0 ;
	} else {
		ew->disp_flag = 1 ;
		pl_char_set2( ew, 15, 5, pl_w->cg_prm - 16 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls32_00_nml_k_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls32_00_nml_k_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls32_00_nml_k_frame,
	} ;
	PLS32_00_LOCAL_W *lw = PLS32_00_LW( ew ) ;

	ew->prio = pl_w->prio ;
	if( pl_w->char_no.w != lw->char_no ){
		ew->routine_0 ++ ;
		return ;
	}
	pls32_00_nml_k_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls32_00_nml_k( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls32_00_nml_k_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls32_00_nml_k_init, pls32_00_nml_k_move, pls32_00_next, pls32_00_free,
	} ;

	pls32_00_nml_k_jmp[ew->routine_0]( ew, pl_w ) ;
}

static void pls32_00_quake_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls32_00_quake_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls32_00_quake_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
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
	ew->disp_flag = 0 ;
	scr_work.quake_level = 3 ;
	scr_work.quake_flag = 1 ;
	ew->routine_0 = 2 ;
}

/*--------------------------------------------------------------*/
static void pls32_00_quake_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
}

/*==============================================================*/
static void pls32_00_quake( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls32_00_quake_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls32_00_quake_init, pls32_00_quake_move, pls32_00_next, pls32_00_free,
	} ;

	pls32_00_quake_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls32_00_next( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls32_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls32_00.c" End of File	*/

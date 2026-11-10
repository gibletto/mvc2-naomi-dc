/* Hulk's player effect 00: the afterimages that follow his body. Each type is one kind of
   mark, taking the master's place and picture while his animation lasts (types 0 and 1 face as
   he does, 2, 3, 6, 7 and 8 face the other way, 4 and 5 show set animations). */
#include "player.h"
#include "pl_char.h"

/* the animation the master showed when the effect was made, in the work's own word */
#define PLS0D_00_ANIM( ew )	( ( Uint16 * )&( ew )->obj_flag )

/* the work takes the master's look */
#define PLS0D_00_LOOK( ew, pl_w ) \
	CHAR_PART_COPY( ew, pl_w ) ; \
	( ew )->disp_flag = 1 ; \
	( ew )->pl_no = ( pl_w )->pl_no ; \
	( ew )->pl_type = ( pl_w )->pl_type ; \
	( ew )->scale.x = ( pl_w )->scale.x ; \
	( ew )->scale.y = ( pl_w )->scale.y ; \
	( ew )->atck_level = ( pl_w )->atck_level ; \
	( ew )->work_no = ( pl_w )->work_no ; \
	( ew )->col_cnt = ( pl_w )->col_cnt ; \
	( ew )->scale = ( pl_w )->scale

static void pls0d_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls0d_00_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 no )
{
	PLAYER_WORK *ew ;
	Uint16 *anim ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls0d_00_main ;
		ew->ef_no = 0xd00 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = type ;
		ew->sub_type = no ;
		anim = PLS0D_00_ANIM( ew ) ;
		*anim = pl_w->char_no.w ;
	}
	return ew ;
}

static void pls0d_00_a_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0d_00_b_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0d_00_c_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0d_00_d_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls0d_00_main( PLAYER_WORK *ew )
{
	static void ( *pls0d_00_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0d_00_a_main, pls0d_00_a_main, pls0d_00_b_main, pls0d_00_b_main, pls0d_00_c_main,
		pls0d_00_d_main, pls0d_00_b_main, pls0d_00_b_main, pls0d_00_b_main,
	} ;

	pls0d_00_jmp[ew->type]( ew, ew->master_w ) ;
}

static void pls0d_00_a_sync( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls0d_00_a_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	PLS0D_00_LOOK( ew, pl_w ) ;
	ew->prio = pl_w->prio ;
	ew->disp_flag = 1 ;
	ew->pos.z = pl_w->pos.z ;
	if( !ew->sub_type ){
		ew->char_no.b.no = 0 ;
		ew->sub_prio = 1 ;
	}
	else{
		ew->char_no.b.no = 1 ;
		ew->sub_prio = -1 ;
	}
	pl_char_set( ew, Char_Effect, ew->char_no.b.no ) ;
	pls0d_00_a_sync( ew, pl_w ) ;
}

/*==============================================================*/
static void pls0d_00_a_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void pls0d_00_a_sync( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls0d_00_a_sync_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0d_00_a_move,
	} ;
	Uint16 *anim = PLS0D_00_ANIM( ew ) ;

	if( pl_w->char_no.w != *anim ){
		ew->routine_0 ++ ;
		return ;
	}
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->prio = pl_w->prio ;
	pls0d_00_a_sync_jmp[ew->routine_1]( ew, pl_w ) ;
}

static void pls0d_00_next( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0d_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls0d_00_a_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls0d_00_a_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0d_00_a_init, pls0d_00_a_sync, pls0d_00_next, pls0d_00_free,
	} ;

	pls0d_00_a_jmp[ew->routine_0]( ew, pl_w ) ;
}

static void pls0d_00_b_sync( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls0d_00_b_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	PLS0D_00_LOOK( ew, pl_w ) ;
	ew->prio = pl_w->prio ;
	ew->disp_flag = 1 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	if( !ew->sub_type ){
		ew->char_no.b.no = 2 ;
		ew->sub_prio = -1 ;
	}
	else{
		ew->char_no.b.no = 3 ;
		ew->sub_prio = 1 ;
	}
	pl_char_set( ew, Char_Effect, ew->char_no.b.no ) ;
	ew->disp_dir ^= 1 ;
	pls0d_00_b_sync( ew, pl_w ) ;
}

/*==============================================================*/
static void pls0d_00_b_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void pls0d_00_b_sync( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls0d_00_b_sync_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0d_00_b_move,
	} ;

	ew->prio = pl_w->prio ;
	pls0d_00_b_sync_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls0d_00_b_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls0d_00_b_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0d_00_b_init, pls0d_00_b_sync, pls0d_00_next, pls0d_00_free,
	} ;

	pls0d_00_b_jmp[ew->routine_0]( ew, pl_w ) ;
}

static void pls0d_00_c_sync( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls0d_00_c_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	PLS0D_00_LOOK( ew, pl_w ) ;
	ew->prio = pl_w->prio ;
	ew->disp_flag = 1 ;
	ew->sub_prio = 1 ;
	ew->pos.z = pl_w->pos.z ;
	pl_char_set( ew, Char_Effect, 4 ) ;
	pls0d_00_c_sync( ew, pl_w ) ;
}

/*==============================================================*/
static void pls0d_00_c_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void pls0d_00_c_sync( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls0d_00_c_sync_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0d_00_c_move,
	} ;
	Uint16 *anim = PLS0D_00_ANIM( ew ) ;

	if( pl_w->char_no.w != *anim ){
		ew->routine_0 ++ ;
		return ;
	}
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->prio = 7 ;
	pls0d_00_c_sync_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls0d_00_c_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls0d_00_c_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0d_00_c_init, pls0d_00_c_sync, pls0d_00_next, pls0d_00_free,
	} ;

	pls0d_00_c_jmp[ew->routine_0]( ew, pl_w ) ;
}

static void pls0d_00_d_sync( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls0d_00_d_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	/* whether the mark is turned round, and its animation, by type */
	static Uint8 pls0d_00_d_tbl[4][2] = {
		{ 0, 5 }, { 0, 6 }, { 0, 7 }, { 1, 7 },
	} ;

	ew->routine_0 ++ ;
	PLS0D_00_LOOK( ew, pl_w ) ;
	ew->prio = pl_w->prio ;
	ew->disp_flag = 1 ;
	ew->sub_prio = -1 ;
	ew->pos.z = pl_w->pos.z ;
	if( pls0d_00_d_tbl[ew->sub_type][0] ) ew->disp_dir ^= 1 ;
	pl_char_set( ew, Char_Effect, pls0d_00_d_tbl[ew->sub_type][1] ) ;
	pls0d_00_d_sync( ew, pl_w ) ;
}

/*==============================================================*/
static void pls0d_00_d_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls0d_00_d_sync( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls0d_00_d_sync_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0d_00_d_move,
	} ;
	Uint16 *anim = PLS0D_00_ANIM( ew ) ;

	if( pl_w->char_no.w != *anim ){
		ew->routine_0 ++ ;
		return ;
	}
	ew->prio = pl_w->prio ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	pls0d_00_d_sync_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls0d_00_d_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls0d_00_d_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0d_00_d_init, pls0d_00_d_sync, pls0d_00_next, pls0d_00_free,
	} ;

	pls0d_00_d_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
/*			The mark is over: it is hidden, and freed on the	*/
/*			next frame											*/
/*==============================================================*/
static void pls0d_00_next( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls0d_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls0d_00.c" End of File	*/

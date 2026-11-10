/* Doctor Doom's player effect 00: the marks of his moves that fly off him. Type 0 sits at a side
   of him and follows his animation, type 1 flies off to a side and type 2 drops to the ground
   after a wait. */
#include "player.h"
#include "ef_char.h"
#include "pl_char.h"

/* the work takes the master's look */
#define PLS0F_00_LOOK( ew, pl_w ) \
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

/* type 0: the mark's place beside him, by its number */
static Sint16 pls0f_00_ofs_a[2] = { 64, -64 } ;

/* type 1: how fast the mark flies off, x then y, by its number */
static Sint16 pls0f_00_spd_b[4] = { -77, 143, -32, 143 } ;

/* type 2: the mark's place beside him, the order it is drawn in and how far above the ground it
   comes to rest, by its number */
static Sint16 pls0f_00_ofs_c[4] = { -48, -96, -144, -192 } ;
static Uint8 pls0f_00_prio_c[4] = { 1, 2, 1, 0 } ;
static Sint16 pls0f_00_drop_c[4] = { 16, 24, 16, 0 } ;

static void pls0f_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls0f_00_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls0f_00_main ;
		ew->ef_no = 0xf00 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = type ;
		ew->sub_type = no ;
	}
	return ew ;
}

static void pls0f_00_a_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0f_00_b_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0f_00_c_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls0f_00_main( PLAYER_WORK *ew )
{
	static void ( *pls0f_00_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0f_00_a_main, pls0f_00_b_main, pls0f_00_c_main,
	} ;

	pls0f_00_jmp[ew->type]( ew, ew->master_w ) ;
}

static void pls0f_00_a_sync( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls0f_00_a_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	PLS0F_00_LOOK( ew, pl_w ) ;
	ew->prio = pl_w->prio ;
	ew->disp_flag = 1 ;
	ew->sub_prio = -1 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	ew->disp_dir = ew->sub_type ;
	ew->pos.x += DOT_X( pls0f_00_ofs_a[ew->sub_type] ) ;
	ef_char_set( ew, 27, 0 ) ;
	pls0f_00_a_sync( ew, pl_w ) ;
}

/*==============================================================*/
static void pls0f_00_a_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ef_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls0f_00_a_sync( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls0f_00_a_sync_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0f_00_a_move,
	} ;

	ew->prio = pl_w->prio ;
	if( pl_w->char_no.b.grp != Char_Win ){
		ew->routine_0 ++ ;
		return ;
	}
	pls0f_00_a_sync_jmp[ew->routine_1]( ew, pl_w ) ;
}

static void pls0f_00_next( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0f_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls0f_00_a_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls0f_00_a_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0f_00_a_init, pls0f_00_a_sync, pls0f_00_next, pls0f_00_free,
	} ;

	pls0f_00_a_jmp[ew->routine_0]( ew, pl_w ) ;
}

static void pls0f_00_b_sync( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls0f_00_b_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	PLS0F_00_LOOK( ew, pl_w ) ;
	ew->prio = pl_w->prio ;
	ew->disp_flag = 1 ;
	ew->sub_prio = -1 ;
	ew->pos.z = pl_w->pos.z ;
	ew->spd.x = DOT_X( pls0f_00_spd_b[ew->sub_type * 2] ) ;
	ew->spd.y = DOT_Y( pls0f_00_spd_b[ew->sub_type * 2 + 1] ) ;
	if( ew->disp_dir ) ew->spd.x = -ew->spd.x ;
	pl_char_set( ew, Char_Effect, 17 ) ;
	pls0f_00_b_sync( ew, pl_w ) ;
}

/*==============================================================*/
static void pls0f_00_b_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void pls0f_00_b_sync( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls0f_00_b_sync_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0f_00_b_move,
	} ;

	ew->pos.x = pl_w->pos.x + ew->spd.x ;
	ew->pos.y = pl_w->pos.y + ew->spd.y ;
	pls0f_00_b_sync_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls0f_00_b_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls0f_00_b_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0f_00_b_init, pls0f_00_b_sync, pls0f_00_next, pls0f_00_free,
	} ;

	pls0f_00_b_jmp[ew->routine_0]( ew, pl_w ) ;
}

static void pls0f_00_c_sync( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls0f_00_c_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	PLS0F_00_LOOK( ew, pl_w ) ;
	ew->prio = pl_w->prio ;
	ew->disp_flag = 0 ;
	ew->sub_prio = pls0f_00_prio_c[ew->sub_type] ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = scr_work.scr_top ;
	ew->pos.z = pl_w->pos.z ;
	if( !ew->disp_dir ) ew->pos.x += DOT_X( pls0f_00_ofs_c[ew->sub_type] ) ;
	else ew->pos.x -= DOT_X( pls0f_00_ofs_c[ew->sub_type] ) ;
	ew->spd.y = -DOT_Y( 6.0f ) ;
	ew->acc.y = -DOT_Y( 0.375f ) ;
	pl_char_set( ew, 19, 7 ) ;
	ew->timer0 = ew->sub_type * 8 ;
	pls0f_00_c_sync( ew, pl_w ) ;
}

/*==============================================================*/
/*			Waits, then shows the mark							*/
/*==============================================================*/
static void pls0f_00_c_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ew->timer0 -- == 0 ){
		ew->disp_flag = 1 ;
		ew->routine_1 ++ ;
	}
}

/*==============================================================*/
/*			Falls to the ground, and stays there				*/
/*==============================================================*/
static void pls0f_00_c_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( pl_w->ground_y + pls0f_00_drop_c[ew->sub_type] > ew->pos.y ){
		ew->pos.y = pl_w->ground_y + pls0f_00_drop_c[ew->sub_type] ;
		ew->routine_1 ++ ;
		pl_char_set( ew, 19, ew->sub_type + 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls0f_00_c_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls0f_00_c_sync( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls0f_00_c_sync_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0f_00_c_wait, pls0f_00_c_fall, pls0f_00_c_move,
	} ;

	ew->prio = 13 ;
	if( pl_w->char_no.b.grp != Char_Win ){
		ew->routine_0 ++ ;
		return ;
	}
	pls0f_00_c_sync_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls0f_00_c_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls0f_00_c_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0f_00_c_init, pls0f_00_c_sync, pls0f_00_next, pls0f_00_free,
	} ;

	pls0f_00_c_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
/*			The mark is over: it is hidden, and freed on the	*/
/*			next frame											*/
/*==============================================================*/
static void pls0f_00_next( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls0f_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls0f_00.c" End of File	*/

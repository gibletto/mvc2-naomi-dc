/* Omega Red's player effect 00: the pieces that ride on the player in one of his moves,
   shown at spots from a table, by the frame of his animation. */
#include "player.h"
#include "ef_char.h"
#include "pl_char.h"
#include "pl30_def.h"

typedef struct PLS30_00_REC {
	Sint16	anim;		/* the animation to show, 0 or less for none */
	Sint16	x;
	Sint16	y;
	Sint16	flag;		/* bit 0 turns it about, bit 1 flips it */
} PLS30_00_REC;

static PLS30_00_REC pls30_00_tbl[2][9] = {
	{
		{ 0, 32, 4, 2 }, { 1, 36, 4, 2 }, { 2, 36, 4, 2 },
		{ 3, -63, 29, 1 }, { 4, -63, 29, 1 }, { 5, -63, 29, 1 },
		{ 6, -78, 80, 0 }, { 7, -78, 80, 0 }, { 8, -78, 80, 0 },
	},
	{
		{ -1, 0, 0, 0 }, { -1, 0, 0, 0 }, { -1, 0, 0, 0 },
		{ 0, 4, 53, 0 }, { 1, 4, 53, 0 }, { 2, 4, 53, 0 },
		{ -1, 0, 0, 0 }, { -1, 0, 0, 0 }, { -1, 0, 0, 0 },
	},
} ;

static void pls30_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls30_00_set( PLAYER_WORK *pl_w, PLAYER_WORK *w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_END ) ) ){
		ew->work_func = pls30_00_main ;
		ew->master_w = pl_w ;
		ew->ef_w = w ;
		ew->type = type ;
		ew->ef_no = 0x3000 ;
	}
	return ew ;
}

static void pls30_00_drain( PLAYER_WORK *ew );
static void pls30_00_ground( PLAYER_WORK *ew );

/*==============================================================*/
static void pls30_00_main( PLAYER_WORK *ew )
{
	static void ( *pls30_00_jmp[] )( PLAYER_WORK *ew ) = {
		pls30_00_drain, pls30_00_drain, pls30_00_ground,
	} ;

	pls30_00_jmp[ew->type]( ew ) ;
}

static void pls30_00_init( PLAYER_WORK *ew );
static void pls30_00_move( PLAYER_WORK *ew );
static void pls30_00_next( PLAYER_WORK *ew );
static void pls30_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls30_00_drain( PLAYER_WORK *ew )
{
	static void ( *pls30_00_drain_jmp[] )( PLAYER_WORK *ew ) = {
		pls30_00_init, pls30_00_move, pls30_00_next, pls30_00_free,
	} ;

	pls30_00_drain_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls30_00_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
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
	ew->timer0 = 0 ;
	ew->sub_prio = -1 ;
	pls30_00_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls30_00_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLS30_00_REC *rec ;
	Float x, y ;
	Sint16 *fp ;

	ew->disp_flag = 0 ;
	if( pl_w->act_st != N_SPECIAL || ( pl_w->routine_2 != 9 && pl_w->routine_2 != 10 ) ){
		pls30_00_next( ew ) ;
		return ;
	}
	{
		ew->prio = pl_w->prio ;
		ew->pos = pl_w->pos ;
		rec = &pls30_00_tbl[ew->type][ew->timer0] ;
		ew->timer0 = ( ew->timer0 + 1 ) % 9 ;
		if( rec->anim > 0 ){
			ef_char_set2( ew, 27, 10, rec->anim ) ;
			x = DOT_X( rec->x ) ;
			if( pl_w->disp_dir ) x = -x ;
			ew->pos.x += x ;
			y = rec->y * DOT_Y( 1.0f ) ;
			ew->pos.y += y ;
			if( pl_w->pl_stance == STANCE_AIR ) ew->pos.y += DOT_Y( 16.0f ) ;
			if( *( fp = &rec->flag ) & 1 ) ew->disp_dir ^= 1 ;
			if( *fp & 2 ) ew->scale.y = -ew->scale.y ;
			ew->disp_flag = 1 ;
		}
	}
}

/*==============================================================*/
static void pls30_00_ground( PLAYER_WORK *ew )
{
	if( !ew->routine_0 ){
		ew->routine_0 ++ ;
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
		ef_char_set( ew, 27, 11 ) ;
		ew->pos = ew->ef_w->pos ;
		ew->pos.y = ew->master_w->ground_y ;
	} else if( ef_char_move( ew ) < 0 ){
		pls30_00_free( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls30_00_next( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls30_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls30_00.c" End of File	*/

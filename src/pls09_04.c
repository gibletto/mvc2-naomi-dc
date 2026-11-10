/* Iceman's player effect 04: the aura that shows while his animation holds the same frame
   code, changing to each new code that his animation passes on. */
#define PL_CHAR_SET2_BYTE
#include "player.h"
#include "pl_char_old.h"

static void pls09_04_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls09_04_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls09_04_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x904 ;
		ew->timer0 = pl_w->char_no.w ;
	}
	return ew ;
}

static void pls09_04_init( PLAYER_WORK *ew );
static void pls09_04_move( PLAYER_WORK *ew );
static void pls09_04_end( PLAYER_WORK *ew );
static void pls09_04_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls09_04_main( PLAYER_WORK *ew )
{
	static void ( *pls09_04_jmp[] )( PLAYER_WORK *ew ) = {
		pls09_04_init, pls09_04_move, pls09_04_end, pls09_04_free,
	} ;

	pls09_04_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
/*			A new code in the low four bits of his				*/
/*			animation's cg_type changes the aura's animation	*/
/*==============================================================*/
static void pls09_04_char_set( PLAYER_WORK *ew )
{
	int no = ew->master_w->cg_type ;

	if( !( no & 15 ) ) return ;
	if( no == ew->sub_type ) return ;
	ew->sub_type = ew->master_w->cg_type ;
	pl_char_set2( ew, Char_Effect, 8, ( ew->sub_type - 1 ) & 15 ) ;
}

/*==============================================================*/
static void pls09_04_init( PLAYER_WORK *ew )
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
	ew->disp_flag = 1 ;
	ew->prio = 15 ;
	ew->sub_type = 0xff ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	pls09_04_char_set( ew ) ;
	pls09_04_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls09_04_move( PLAYER_WORK *ew )
{
	pls09_04_char_set( ew ) ;
	if( !ew->master_w->cg_type || ew->master_w->char_no.w != ew->timer0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void pls09_04_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	pls09_04_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls09_04_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls09_04.c" End of File	*/

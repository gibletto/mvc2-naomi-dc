/* Spider-Man's player effect 00: eight marks that ride on a player, each at its own offset from
   the player and in its own animation, by type; the setter returns how many it made. */
#include "player.h"
#include "pl_char.h"

/* the offset of each mark from the player: x and y, then the same when mirrored */
static Sint16 pls0c_00_ofs0[] = {
	0, 24, 5, 14, 0, 48, 0, 36,
	0, 74, 0, 61, 20, 99, -8, 86,
} ;
static Sint16 pls0c_00_ofs1[] = {
	0, 36, 0, 22, -19, 64, 0, 50,
	-19, 92, 5, 78, 0, 120, 0, 106,
} ;
static Sint16 *pls0c_00_ofs_tbl[] = { pls0c_00_ofs0, pls0c_00_ofs1 } ;

static void pls0c_00_main( PLAYER_WORK *ew );

/****************************************************************/
int pls0c_00_set( PLAYER_WORK *pl_w, int no )
{
	PLAYER_WORK *ew ;
	int i = 0 ;
	int cnt = 0 ;

	for( i = 0 ; i < 8 ; i ++ ){
		if( ( ew = work_get( 0, W_PLS, LIST_END ) ) ){
			ew->work_func = pls0c_00_main ;
			ew->ef_no = 0xc00 ;
			ew->type = no ;
			ew->sub_type = i >> 1 ;
			if( i & 1 ) ew->sub_type |= 0x80 ;
			ew->ef_w = pl_w ;
			ew->master_w = pl_w->master_w ;
			cnt ++ ;
		}
	}
	return cnt ;
}

static void pls0c_00_init( PLAYER_WORK *ew );
static void pls0c_00_move( PLAYER_WORK *ew );
static void pls0c_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls0c_00_main( PLAYER_WORK *ew )
{
	static void ( *pls0c_00_jmp[] )( PLAYER_WORK *ew ) = {
		pls0c_00_init, pls0c_00_move, pls0c_00_free,
	} ;

	PLAYER_WORK *pl_w = ew->ef_w ;

	pls0c_00_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls0c_00_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->ef_w ;
	Sint16 *ofs ;
	int anim ;

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
	ew->routine_0 ++ ;
	ew->prio = pl_w->prio ;
	ew->sub_prio = pl_w->sub_prio - 4 ;
	ofs = pls0c_00_ofs_tbl[ew->type] ;
	anim = 8 ;
	if( ew->sub_type & 0x80 ){
		anim = 12 ;
		ofs += 2 ;
		ew->sub_type &= 0x7f ;
	}
	ofs += ew->sub_type * 4 ;
	ew->pos = pl_w->pos ;
	ew->pos.x += *ofs ++ * 1.6666666f ;
	ew->pos.y += *ofs * 2.142857f ;
	pl_char_set( ew, Char_Effect, anim + ew->type * 2 ) ;
}

/*--------------------------------------------------------------*/
static void pls0c_00_move( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void pls0c_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls0c_00.c" End of File	*/

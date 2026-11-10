/* Silver Samurai's player effect 05: the sparks that fly from the opponent when his element hit
   lands. Kind 0 sits on the opponent and shows its animation until the hit is over; kind 1 makes
   a kind 2 spark for each beat; a kind 2 spark flies off by pls2f_05_sx_tbl and pls2f_05_sy_tbl. */
#include "player.h"
#include "ef_char.h"
#include "pl2f_def.h"

/* the effect's own work, from obj_flag */
typedef struct PLS2F_05_LOCAL_W {
	Sint32	char_no;		/* 000: his animation when it was made */
	Sint32	atck_no;		/* 004: his attack */
	PLAYER_WORK	*tgt_w;		/* 008: the opponent it sits on */
} PLS2F_05_LOCAL_W;
#define PLS2F_05_LW( w )	( ( PLS2F_05_LOCAL_W * )&( w )->obj_flag )

static void pls2f_05_main( PLAYER_WORK *ew );
static void pls2f_05_main2( PLAYER_WORK *ew );

/****************************************************************/
/*			Sets a spark of kind no: 0 the sitting				*/
/*			spark, 1 the maker, 2 a flying spark				*/
/****************************************************************/
void *pls2f_05_set( PLAYER_WORK *pl_w, int no )
{
	PLAYER_WORK *ew ;
	void ( *func )( PLAYER_WORK *ew ) ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		func = pls2f_05_main ;
		if( no ) func = pls2f_05_main2 ;
		ew->work_func = func ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x2f05 ;
		PLS2F_05_LW( ew )->char_no = pl_w->char_no.w ;
		PLS2F_05_LW( ew )->atck_no = pl_w->atck_no ;
		ew->type = no ;
		ew->sub_type = no + 255 ;
		PLS2F_05_LW( ew )->tgt_w = pl_w->em_w ;
	}
	return ew ;
}

static void pls2f_05_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2f_05_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2f_05_next( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2f_05_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2f_05_main( PLAYER_WORK *ew )
{
	static void ( *pls2f_05_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2f_05_init, pls2f_05_move, pls2f_05_next, pls2f_05_free,
	} ;

	pls2f_05_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void pls2f_05_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->sub_prio = -1 ;
	pls2f_05_move( ew, pl_w ) ;
}

static void pls2f_05_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2f_05_keep( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2f_05_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void pls2f_05_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls2f_05_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2f_05_wait, pls2f_05_keep, pls2f_05_end,
	} ;

	pls2f_05_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
/*			Waits for his hit on the opponent, then shows up	*/
/*			on him												*/
/*==============================================================*/
static void pls2f_05_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w ;

	if( PLS2F_05_LW( ew )->char_no != pl_w->char_no.w ){
		pls2f_05_free( ew, pl_w ) ;
		return ;
	}
	em_w = pl_w->em_w ;
	if( pl_w->hit_flag && em_w->routine_1 == PL_DAMAGE && pl_w == em_w->dm_hit_w &&em_w->dm_atck_no == PLS2F_05_LW( ew )->atck_no ){
		ew->routine_1 ++ ;
		ew->disp_flag = 1 ;
		ew->prio = em_w->prio ;
		ew->timer0 = ( Rnd() & 63 ) + 120 ;
		ew->acc.x = ( ( int )( Rnd() & 63 ) - 32 ) * 1.6666666f ;
		ew->acc.y = ( ( int )Rnd() % em_w->size_top ) * 2.142857f ;
		PLS2F_05_LW( ew )->tgt_w = em_w ;
		ef_char_set( ew, 27, ( Sys->game_timer0 & 3 ) + 6 ) ;
		pls2f_05_keep( ew, pl_w ) ;
	}
}

/*==============================================================*/
/*			Stays on the opponent while he is held, then		*/
/*			plays its end										*/
/*==============================================================*/
static void pls2f_05_keep( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *tgt_w ;
	Float ofs ;
	Uint8 elem ;

	ef_char_move( ew ) ;
	tgt_w = PLS2F_05_LW( ew )->tgt_w ;
	ofs = ew->acc.x ;
	if( tgt_w->disp_dir ) ofs = -ofs ;
	ew->pos.x = tgt_w->pos.x + ofs ;
	ofs = ew->acc.y ;
	if( tgt_w->pl_stance == STANCE_CROUCH ) ofs /= 2.0f ;
	if( tgt_w->pl_stance == STANCE_DOWN ) ofs /= 4.0f ;
	ew->pos.y = tgt_w->pos.y + ofs ;
	ew->prio = tgt_w->prio ;
	if( tgt_w->dm_no == 9 ){
		elem = tgt_w->dm_elem ;
		if( elem == 1 || !elem ){
			if( tgt_w->dm_no != 3 && -- ew->timer0 > 0 ) return ;
		}
	}
	ew->routine_1 ++ ;
	ef_char_set( ew, 27, ew->char_no.b.no + 4 ) ;
}

/*--------------------------------------------------------------*/
static void pls2f_05_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

static void pls2f_05_init2( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2f_05_move2( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2f_05_main2( PLAYER_WORK *ew )
{
	static void ( *pls2f_05_jmp2[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2f_05_init2, pls2f_05_move2, pls2f_05_next, pls2f_05_free,
	} ;

	pls2f_05_jmp2[ew->routine_0]( ew, ew->master_w ) ;
}

/* how far a flying spark's speed reaches, by the beats left */
static Sint8 pls2f_05_sx_tbl[18] = { 5, 3, 1, -1, -3, -5, 4, 2, 0, -2, -4, 6, 4, 2, 0, -2, -4, -6 } ;
static Sint8 pls2f_05_sy_tbl[18] = { 3, 3, 3, 3, 3, 3, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0 } ;

/*==============================================================*/
static void pls2f_05_init2( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	if( !ew->sub_type ){
		ew->disp_flag = 0 ;
		ew->timer0 = 0 ;
		ew->timer1 = 17 ;
	}
	else {
		ew->disp_flag = 1 ;
		ew->sub_prio = -1 ;
		ef_char_set( ew, 27, ( Rnd() & 3 ) + 6 ) ;
	}
	pls2f_05_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls2f_05_move2( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *tgt_w = PLS2F_05_LW( ew )->tgt_w ;
	PLAYER_WORK *sp_w ;
	Sint32 ofs ;
	Uint8 elem ;

	if( !ew->sub_type ){
		if( tgt_w->routine_1 == PL_DAMAGE && tgt_w->dm_no == 9 && ew->timer1 >= 0 ){
			if( !( sp_w = pls2f_05_set( pl_w, 2 ) ) ) return ;
			sp_w->acc.x = ( ( ( tgt_w->size_front + tgt_w->size_back ) >> 4 ) * pls2f_05_sx_tbl[ew->timer1] + ( int )( Rnd() & 2 ) - 1 ) * 1.6666666f ;
			sp_w->acc.y = ( ( tgt_w->size_top >> 5 ) * pls2f_05_sy_tbl[ew->timer1] + ( int )( Rnd() & 2 ) - 1 ) * 2.142857f ;
			ew->timer1 -- ;
			return ;
		}
	}
	else if( !ew->routine_1 ){
		ofs = ew->acc.x ;
		if( tgt_w->disp_dir ) ofs = -ofs ;
		ew->pos.x = tgt_w->pos.x + ofs ;
		ew->pos.y = tgt_w->pos.y + ew->acc.y ;
		ew->prio = tgt_w->prio ;
		ef_char_move( ew ) ;
		if( tgt_w->routine_1 == PL_DAMAGE ){
			elem = tgt_w->dm_elem ;
			if( ( elem == 1 || !elem ) && tgt_w->dm_no == 9 ) return ;
		}
		ew->routine_1 ++ ;
		ef_char_set( ew, 27, ew->char_no.b.no + 4 ) ;
		return ;
	}
	else if( ef_char_move( ew ) >= 0 && ef_char_move( ew ) >= 0 ){
		return ;
	}
	pls2f_05_free( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls2f_05_next( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls2f_05_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls2f_05.c" End of File	*/

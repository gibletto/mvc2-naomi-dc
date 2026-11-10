/* Captain Commando's player effect 00: a script of effects (type 0) that makes the small pieces of
   his entrance, the types that fall into place, follow him or fly off. */
#include "player.h"
#include "pl_char.h"

/* the script's place in the effect, which keeps it in ef_w until it ends */
#define PLS38_00_SCRIPT( w )	( *( Sint16 ** )&( w )->ef_w )

/* the script: how many pieces, the wait before the first, then each one's number and the wait
   after it */
static Sint16 pls38_00_script[] = {
	5, 4, 0x0100, 8, 0x0101, 4, 0x0102, 0x10, 0x0103, 0x14, 0x0400, 0,
} ;

/* where a falling piece lands, the frames to its first spark and between sparks, and its animation */
typedef struct PLS38_00_POS {
	Float	x, y ;
	Sint8	wait, spark_wait ;
	Sint16	no ;
} PLS38_00_POS ;

static PLS38_00_POS pls38_00_pos_tbl[4] = {
	{ 55.0f, DOT_Y( 62.0f ), 2, 2, 2 },
	{ -DOT_X( 44.0f ), 150.0f, 4, 4, 3 },
	{ 80.0f, 30.0f, 6, 3, 2 },
	{ -DOT_X( 46.0f ), DOT_Y( 13.0f ), 3, 6, 3 },
} ;

static void pls38_00_main( PLAYER_WORK *ew );

/****************************************************************/
/*			Sets the script of his entrance's pieces			*/
/****************************************************************/
PLAYER_WORK *pls38_00_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls38_00_main ;
		ew->type = ew->sub_type = 0 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x3800 ;
	}
	return ew ;
}

/*==============================================================*/
/*			A piece of the script's: it follows the effect		*/
/*			that made it										*/
/*==============================================================*/
static PLAYER_WORK *pls38_00_child_set( PLAYER_WORK *pl_w, Uint16 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( pl_w, W_PLS, LIST_BEHIND ) ) ){
		ew->work_func = pls38_00_main ;
		ew->type = no >> 8 ;
		ew->sub_type = no ;
		ew->master_w = pl_w->master_w ;
		ew->ef_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x3800 ;
	}
	return pl_w ;
}

static void pls38_00_script_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls38_00_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls38_00_ride( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls38_00_keep( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls38_00_last( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls38_00_trail( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls38_00_main( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	static void ( *pls38_00_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls38_00_script_main, pls38_00_fall, pls38_00_ride, pls38_00_keep, pls38_00_last, pls38_00_trail,
	} ;

	if( ew->routine_0 < 2 ) pls38_00_jmp[ew->type]( ew, pl_w ) ;
	else if( ew->routine_0 == 2 ) ew->routine_0 = 3 ;
	else work_free( ew ) ;
}

/*==============================================================*/
/*			The script's player: makes its pieces one after		*/
/*			the other											*/
/*==============================================================*/
static void pls38_00_script_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Sint16 *p ;

	if( !ew->routine_0 ){
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
		p = pls38_00_script ;
		if( !( ew->timer1 = *p ++ ) ){
			work_free( ew ) ;
			return ;
		}
		ew->timer0 = *p ++ ;
		PLS38_00_SCRIPT( ew ) = p ;
	}
	if( !ew->routine_1 ){
		if( --ew->timer0 < 0 ){
			p = PLS38_00_SCRIPT( ew ) ;
			pls38_00_child_set( ew, *p ++ ) ;
			ew->timer0 = *p ++ ;
			PLS38_00_SCRIPT( ew ) = p ;
			if( --ew->timer1 <= 0 ) ew->routine_1 ++ ;
		}
	} else ew->routine_0 = 2 ;
}

/*==============================================================*/
/*			A piece that falls from above him to its			*/
/*			place, bursting into more when it lands				*/
/*==============================================================*/
static void pls38_00_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Uint16 pls38_00_burst_tbl[8] = {
		1, 0x0300, 1, 0x0301, 2, 0x0302, 1, 0x0304,
	} ;
	PLS38_00_POS *pos = &pls38_00_pos_tbl[ew->sub_type] ;
	Uint16 n, i ;
	Sint16 no ;
	Float x ;

	if( !ew->routine_0 ){
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
		ew->sub_prio = -8 ;
		ew->spd.x = 0 ;
		ew->spd.y = 0 ;
		ew->acc.x = 0 ;
		ew->acc.y = 0 ;
		ew->spd.y = -DOT_Y( 16.0f ) ;
		ew->acc.y = -DOT_Y( 0.5f ) ;
		pl_char_set( ew, 18, pos->no ) ;
		pls38_00_child_set( ew, ew->sub_type | 0x200 ) ;
		ew->timer0 = pos->wait ;
		ew->timer1 = pos->spark_wait ;
		if( ew->disp_dir ) x = -pos->x ;
		else x = pos->x ;
		ew->pos.x = pl_w->pos.x + x ;
		ew->pos.y = pl_w->pos.y + DOT_Y( 288.0f ) ;
	}
	ew->prio = pl_w->prio ;
	if( !ew->routine_1 ){
		if( --ew->timer0 < 0 ){
			ew->timer0 = ew->timer1 ;
			if( Rnd() & 1 ) pls38_00_child_set( ew, 0x500 ) ;
		}
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		if( pos->y > ew->pos.y ){
			ew->pos.y = pos->y ;
			ew->routine_1 ++ ;
			n = pls38_00_burst_tbl[ew->sub_type * 2] ;
			no = pls38_00_burst_tbl[ew->sub_type * 2 + 1] ;
			for( i = 0 ; i < n ; i ++, no ++ ) pls38_00_child_set( ew, no ) ;
		}
	} else if( pl_char_move( ew ) < 0 ) work_free( ew ) ;
}

/*==============================================================*/
/*			A copy of the piece it rides on, until its			*/
/*			animation ends										*/
/*==============================================================*/
static void pls38_00_ride( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ef_w ;

	if( !ew->routine_0 ){
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
		pl_char_set( ew, 18, 4 ) ;
	}
	ef_w = ew->ef_w ;
	ew->pos.x = ef_w->pos.x ;
	ew->pos.y = ef_w->pos.y ;
	ew->prio = ef_w->prio ;
	if( ef_w->cg_type ){
		ef_w->cg_type = 0 ;
		work_free( ew ) ;
	}
}

/*==============================================================*/
/*			A piece at his place that goes while his			*/
/*			animation stays										*/
/*==============================================================*/
static void pls38_00_keep( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->routine_0 ){
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
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		ew->timer0 = pl_w->char_no.w ;
		ew->sub_prio = -8 ;
		pl_char_set( ew, 18, ew->sub_type + 7 ) ;
		pl_voice_req( pl_w, 31 ) ;
	}
	ew->prio = pl_w->prio ;
	if( ew->timer0 != pl_w->char_no.w ) work_free( ew ) ;
}

/*==============================================================*/
/*			A piece at his place that makes his entrance go on	*/
/*			once its animation has a frame code					*/
/*==============================================================*/
static void pls38_00_last( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->routine_0 ){
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
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		ew->sub_prio = -8 ;
		pl_char_set( ew, 18, 12 ) ;
	}
	ew->prio = pl_w->prio ;
	if( pl_char_move( ew ) < 0 ) work_free( ew ) ;
	else if( ew->cg_type ){
		ew->cg_type = 0 ;
		pl_w->routine_2 = 3 ;
	}
}

/*==============================================================*/
/*			A spark that rises from the piece it rides on		*/
/*			and fades											*/
/*==============================================================*/
static void pls38_00_trail( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ef_w ;

	if( !ew->routine_0 ){
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
		ef_w = ew->ef_w ;
		ew->pos.x = ef_w->pos.x ;
		ew->pos.y = ef_w->pos.y ;
		ew->timer0 = 32 ;
		pl_char_set( ew, 18, 6 ) ;
		ew->spd.x = 0 ;
		ew->acc.x = 0 ;
		ew->spd.y = -2.142857f ;
		ew->acc.y = -DOT_Y( 0.25f ) ;
	}
	pl_char_move( ew ) ;
	if( --ew->timer0 < 0 ) ew->routine_0 = 2 ;
	else {
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls38_00.c" End of File	*/

/* Iceman's player effect 00: nine puffs that wait their turn, then drift out from his
   position and fade. */
#include "player.h"
#include "pl_char.h"

static Sint16 pls09_00_x_tbl[8] = { -24, -8, 8, 24, -24, -10, -10, 24 } ;

/* by type - 4: the speed and acceleration along x */
static Float pls09_00_spd_tbl[8] = {
	-1.6666666f, 0.052083332f, -DOT_X( 0.5f ), 0.052083332f,
	1.6666666f, -0.052083332f, DOT_X( 0.5f ), -0.052083332f,
} ;

static void pls09_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls09_00_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	Sint8 i ;

	for( i = 0 ; i <= 8 ; i ++ ){
		if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
			ew->work_func = pls09_00_main ;
			ew->master_w = pl_w ;
			ew->ef_no = 0x900 ;
			ew->type = i ;
			ew->sub_type = 7 ;
			ew->timer0 = 32 ;
			ew->timer1 = 118 ;
		}
	}
	return ew ;
}

static void pls09_00_init( PLAYER_WORK *ew );
static void pls09_00_move( PLAYER_WORK *ew );
static void pls09_00_end( PLAYER_WORK *ew );
static void pls09_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls09_00_main( PLAYER_WORK *ew )
{
	static void ( *pls09_00_jmp[] )( PLAYER_WORK *ew ) = {
		pls09_00_init, pls09_00_move, pls09_00_end, pls09_00_free,
	} ;

	pls09_00_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls09_00_init( PLAYER_WORK *ew )
{
	if( -- ew->timer0 > 0 ) return ;
	if( !( -- ew->sub_type > 0 ) ){
		ew->routine_0 = 2 ;
		pls09_00_end( ew ) ;
		return ;
	}
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
	ew->prio = 0 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	if( ew->type & 4 ){
		pl_char_set( ew, Char_Effect, 5 ) ;
		ew->spd.x = pls09_00_spd_tbl[( ew->type - 4 ) * 2] ;
		ew->acc.x = pls09_00_spd_tbl[( ew->type - 4 ) * 2 + 1] ;
		ew->spd.y = DOT_Y( 6.0f ) ;
		ew->acc.y = -DOT_Y( 0.125f ) ;
	} else {
		pl_char_set( ew, Char_Effect, 4 ) ;
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
	}
	ew->pos.x += DOT_X( pls09_00_x_tbl[ew->type] ) ;
	ew->pos.y += DOT_Y( ew->timer1 ) ;
	if( !ew->type ) pl_voice_req( ew, 10 ) ;
	pls09_00_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls09_00_move( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 = 0 ;
		ew->disp_flag = 0 ;
		ew->timer1 -= 19 ;
	}
}

/*--------------------------------------------------------------*/
static void pls09_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	pls09_00_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls09_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls09_00.c" End of File	*/

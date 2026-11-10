/* Mega Man's player effect 06: sixteen pictures that burst out from Mega Man or Roll when
   they are beaten, four each way (type is the direction, and sets the speed). */
#include "player.h"
#include "ef_char.h"
#include "pl1c_def.h"

static PLAYER_WORK *pls1c_06_set( PLAYER_WORK *pl_w, Uint8 no );

/****************************************************************/
void pls1c_06_start( PLAYER_WORK *pl_w )
{
	pls1c_06_set( pl_w, 3 ) ;
	pls1c_06_set( pl_w, 7 ) ;
	pls1c_06_set( pl_w, 11 ) ;
	pls1c_06_set( pl_w, 15 ) ;
	pls1c_06_set( pl_w, 1 ) ;
	pls1c_06_set( pl_w, 5 ) ;
	pls1c_06_set( pl_w, 9 ) ;
	pls1c_06_set( pl_w, 13 ) ;
	pls1c_06_set( pl_w, 2 ) ;
	pls1c_06_set( pl_w, 6 ) ;
	pls1c_06_set( pl_w, 10 ) ;
	pls1c_06_set( pl_w, 14 ) ;
	pls1c_06_set( pl_w, 0 ) ;
	pls1c_06_set( pl_w, 4 ) ;
	pls1c_06_set( pl_w, 8 ) ;
	pls1c_06_set( pl_w, 12 ) ;
	pl_voice_req( pl_w, 32 ) ;
	ef_color_set( pl_w, 2, pl_w->pl_color * 87 + 1, 1 ) ;
}

static void pls1c_06_main( PLAYER_WORK *ew );

/*==============================================================*/
static PLAYER_WORK *pls1c_06_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls1c_06_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->dir = ( no << 1 ) & 31 ;
		ew->pos.x = pl_w->pos.x ;
		if( pl_w->char_no.w == 0x1305 ){
			ew->pos.y = pl_w->pos.y + 90.0f ;
		} else {
			ew->pos.y = pl_w->pos.y + DOT_Y( 8.0f ) ;
		}
		dir_move( ew, 800 ) ;
	}
	return ew ;
}

static void pls1c_06_init( PLAYER_WORK *ew );
static void pls1c_06_move( PLAYER_WORK *ew );
static void pls1c_06_end( PLAYER_WORK *ew );
static void pls1c_06_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls1c_06_main( PLAYER_WORK *ew )
{
	static void ( *pls1c_06_jmp[] )( PLAYER_WORK *ew ) = {
		pls1c_06_init, pls1c_06_move, pls1c_06_end, pls1c_06_free,
	} ;

	pls1c_06_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls1c_06_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
	ew->ef_no = 0x1c06 ;
	pl_w = ew->master_w ;
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
	ew->size_top = 32 ;
	ew->size_bottom = 32 ;
	ew->size_front = 32 ;
	ew->size_back = 32 ;
	ew->timer0 = 120 ;
	ew->prio = 0 ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	if( ew->type & 1 ){
		dir_spd_move( ew, 100 ) ;
	} else {
		dir_spd_move( ew, 200 ) ;
	}
	ef_char_set( ew, 27, 16 ) ;
	pls1c_06_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_06_move( PLAYER_WORK *ew )
{
	if( --ew->timer0 == 0 || !scr_in_ck( ew ) ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	ef_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_06_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls1c_06_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls1c_06.c" End of File	*/

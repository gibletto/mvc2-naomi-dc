/* Storm's effect 03 (character 2a's shot 03): the hail of her Hail Storm. Two drops start at
   the top of the screen at a place from a table and fall, slanting away, until they hit. */
#include "player.h"
#include "pl_char.h"

static void shl2a_03_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl2a_03_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	Sint32 i ;

	for( i = 2 ; i > 0 ; i -- ){
		if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
			ew->work_func = shl2a_03_main ;
			ew->master_w = pl_w ;
			ew->ef_no = 0x2a03 ;
			ew->pl_type = pl_w->pl_type ;
		}
	}
	return ew ;
}

static void shl2a_03_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2a_03_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2a_03_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl2a_03_main( PLAYER_WORK *ew )
{
	static void ( *shl2a_03_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2a_03_init, shl2a_03_move, shl2a_03_free, shl2a_03_free,
	} ;

	shl2a_03_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void shl2a_03_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	/* where along the top of the screen a drop starts, from the screen's left edge */
	static Uint16 x_tbl[32] = {
		0, 16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240,
		256, 272, 288, 304, 320, 336, 352, 368, 384, 144, 160, 176, 192, 208, 224, 240,
	} ;
	Sint32 acc_x, spd_x ;
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
	ew->prio = 0 ;
	ew->atck_flag = HIT_ON ;
	ew->dm_flag = 0 ;
	ew->pos.x = scr_work.scr_left + DOT_X( x_tbl[Rnd() & 31] ) ;
	ew->pos.y = scr_work.scr_top ;
	spd_x = 11 ;
	acc_x = 0 ;
	if( !ew->disp_dir ){
		spd_x = -11 ;
		acc_x = 0 ;
	}
	ew->spd.x = spd_x ;
	ew->spd.y = -( ( int )( Rnd() & 31 ) * 2.142857f ) - DOT_Y( 6.0f ) ;
	ew->acc.x = acc_x ;
	ew->acc.y = 0.0f ;
	ew->timer0 = 100 ;
	pl_char_set( ew, Char_Effect, Rnd() & 3 ) ;
	no = 90 ;
	if( Rnd() & 3 ) no = 95 ;
	ew->atck_no = no ;
	HIT_RESET( ew ) ;
	ew->size_top = ew->size_bottom = ew->size_front = ew->size_back = 64 ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl2a_03_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ew->pl_type != pl_w->pl_type ){
	} else if( !-- ew->timer0 ){
	} else if( ew->hit_flag ){
	} else if( !scr_in_x_ck( ew ) ){
	} else {
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		pl_char_move( ew ) ;
		hit_work_set( ew ) ;
		return ;
	}
	shl2a_03_free( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl2a_03_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl2a_03.c" End of File	*/

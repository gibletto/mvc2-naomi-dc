/* Psylocke's player effect 01: the sparks that scatter from her position, by type (0 the
   small ones, 1 the others, thrown up and against her speed). */
#include "player.h"
#include "ef_char.h"

static void pls08_01_main( PLAYER_WORK *ew );

/****************************************************************/
void *pls08_01_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls08_01_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->ef_no = 0x801 ;
	}
	return ew ;
}

static void pls08_01_init( PLAYER_WORK *ew );
static void pls08_01_move( PLAYER_WORK *ew );
static void pls08_01_end( PLAYER_WORK *ew );
static void pls08_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls08_01_main( PLAYER_WORK *ew )
{
	static void ( *pls08_01_jmp[] )( PLAYER_WORK *ew ) = {
		pls08_01_init, pls08_01_move, pls08_01_end, pls08_01_free,
	} ;

	pls08_01_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls08_01_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	int rnd ;

	ew->routine_0 ++ ;
	pl_w = ew->master_w ;
	CHAR_PART_COPY( ew, pl_w ) ;
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
	ew->pos = pl_w->pos ;
	rnd = nlRand() ;
	if( ew->type ){
		ew->prio = 0 ;
		ew->pos.y = ew->pos.y + ( ( rnd & 63 ) + 24 ) * 2.142857f ;
		rnd = nlRand() ;
		ew->pos.x += ( rnd & 31 ) * 1.6666666f ;
		ew->spd.x = -pl_w->spd.x ;
		ew->acc.x = 0.0f ;
		ew->spd.y = -pl_w->spd.y ;
		ew->acc.y = 0.0f ;
	}
	else{
		ew->pos.y += ( ( rnd & 31 ) - 16 ) * 2.142857f ;
		ew->spd.x = pl_w->spd.x / 8.0f ;
		ew->acc.x = 0.0f ;
		ew->spd.y = pl_w->spd.y / 8.0f ;
		ew->acc.y = 0.0f ;
	}
	if( Sys->game_timer0 & 1 ) ef_char_set( ew, 27, 0 ) ;
	else ef_char_set( ew, 27, 1 ) ;
	pls08_01_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls08_01_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->pl_color = pl_w->pl_color ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ef_char_move( ew ) < 0 ){
		ew->disp_flag = 0 ;
		ew->routine_0 = 3 ;
	}
}

/*--------------------------------------------------------------*/
static void pls08_01_end( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	pls08_01_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls08_01_free( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls08_01.c" End of File	*/

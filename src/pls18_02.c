/* Abyss's (character 18) player effect of the third kind: a watcher that, while he stands idle, now and then
   sets off pieces of pls18_00 (types 1, 7 and 8) and keeps track of which are still alive in sub_no. */
#include "player.h"
#include "pl18_def.h"

static void pls18_02_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls18_02_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls18_02_main ;
		ew->master_w = pl_w ;
	}
	return ew ;
}

/*==============================================================*/
/*			A piece of type 1, or one of types 7 and 8 where	*/
/*			their bits in sub_no allow							*/
/*==============================================================*/
static PLAYER_WORK *pls18_02_ef_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *pw ;
	Uint8 bit ;
	Uint8 type ;

	if( ( ew->sub_no & 3 ) == 3 || ( Rnd() & 3 ) ){
		pw = pls18_00_idle_set( pl_w, ew, 1, no ) ;
	}
	else{
		if( Rnd() & 1 ){
			if( !( ew->sub_no & 1 ) ){
				type = 7 ;
				bit = 1 ;
			}
			else{
				type = 8 ;
				bit = 2 ;
			}
		}
		else{
			if( !( ew->sub_no & 2 ) ){
				type = 8 ;
				bit = 2 ;
			}
			else{
				type = 7 ;
				bit = 1 ;
			}
		}
		if( ( pw = pls18_00_idle_set( pl_w, ew, type, no ) ) ) ew->sub_no |= bit ;
	}
	return pw ;
}

static void pls18_02_init( PLAYER_WORK *ew );
static void pls18_02_type( PLAYER_WORK *ew );
static void pls18_02_end( PLAYER_WORK *ew );
static void pls18_02_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls18_02_main( PLAYER_WORK *ew )
{
	static void ( *pls18_02_jmp[] )( PLAYER_WORK *ew ) = {
		pls18_02_init, pls18_02_type, pls18_02_end, pls18_02_free,
	} ;

	pls18_02_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls18_02_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
	ew->ef_no = 0x1802 ;
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
	ew->disp_flag = 0 ;
	pls18_02_type( ew ) ;
}

static void pls18_02_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls18_02_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void pls18_02_type( PLAYER_WORK *ew )
{
	static void ( *pls18_02_type_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls18_02_wait, pls18_02_move,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_0 >= 2 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	pls18_02_type_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls18_02_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_0 || pl_w->act_st ) return ;
	ew->routine_1 ++ ;
	ew->timer0 = ( Rnd() & 7 ) + 8 ;
	ew->sub_no = 0 ;
	pls18_02_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls18_02_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->act_st ){
		ew->routine_1 = 0 ;
		return ;
	}
	if( !( ew->sub_no & 4 ) && pls18_02_ef_set( ew, pl_w, 4 ) ) ew->sub_no |= 4 ;
	if( !( ew->sub_no & 8 ) && !ew->timer0 && pls18_02_ef_set( ew, pl_w, 8 ) ) ew->sub_no |= 8 ;
	if( ew->timer0 ) ew->timer0 -- ;
}

/*--------------------------------------------------------------*/
static void pls18_02_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void pls18_02_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls18_02.c" End of File	*/

/* Venom's Venom Web (character 0e's shot 01): four parts set together; part 0 hits and catches the
   enemy, and they all go when Venom leaves the move. */
#include "player.h"
#include "pl_char.h"
#include "pl0e_def.h"

static void shl0e_01_main( PLAYER_WORK *ew );

/****************************************************************/
int shl0e_01_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	int i ;

	if( work_rest_cnt <= 4 ) return 0 ;
	for( i = 0 ; i < 4 ; i ++ ){
		if( ( ew = work_get( 0, W_SHL, LIST_END ) ) ){
			ew->ef_no = 0x0e01 ;
			ew->type = i ;
			ew->work_func = shl0e_01_main ;
			ew->master_w = pl_w ;
		}
	}
	return i ;
}

static void shl0e_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0e_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0e_01_back( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0e_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl0e_01_main( PLAYER_WORK *ew )
{
	static void ( *shl0e_01_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0e_01_init, shl0e_01_move, shl0e_01_back, shl0e_01_free,
	} ;

	shl0e_01_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void shl0e_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
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
	ew->atck_flag = HIT_ON | HIT_PL_ONLY ;
	ew->dm_flag = HIT_ON | HIT_PL_ONLY ;
	ew->prio = 12 ;
	ew->sub_type = 0 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y + DOT_Y( 80.0f ) ;
	if( !ew->type ){
		ew->atck_no = pl0e_at_web ;
		HIT_RESET( ew ) ;
	}
	hit_work_set( ew ) ;
	pl_char_set( ew, Char_Effect, ew->type + 10 ) ;
}

static void shl0e_01_catch( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl0e_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0E_LOCAL_W *lw = ( PL0E_LOCAL_W * )pl_w->tlocal ;

	if( pl_w->routine_1 || pl_w->act_st != N_SUPER || pl_w->pl_sp_type != pl0e_sp_web ){
		ew->routine_0 = 3 ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( !ew->type ){
		if( !ew->sub_type && ew->hit_flag ){
			shl0e_01_catch( ew, pl_w ) ;
			return ;
		}
		if( pl_char_move( ew ) < 0 ){
			if( lw->web_line_st && lw->web_web_flag ) return ;
			ew->routine_0 ++ ;
			lw->web_web_flag = 0 ;
			pl_char_set( ew, Char_Effect, ew->type + 14 ) ;
		}
		hit_work_set( ew ) ;
	} else {
		pl_char_move( ew ) ;
		if( !lw->web_web_flag ){
			ew->routine_0 ++ ;
			pl_char_set( ew, Char_Effect, ew->type + 14 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void shl0e_01_back( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0E_LOCAL_W *lw = ( PL0E_LOCAL_W * )&pl_w->tlocal[0] ;

	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 = 3 ;
		ew->disp_flag = 0 ;
		lw->web_line_st = -1 ;
	}
}

/*--------------------------------------------------------------*/
static void shl0e_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

static PLAYER_WORK *shl0e_01_catch_ck( PLAYER_WORK *ew );

/*==============================================================*/
static void shl0e_01_catch( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0E_LOCAL_W *lw = ( PL0E_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *em ;

	ew->sub_type = 1 ;
	if( ( em = shl0e_01_catch_ck( ew ) ) ){
		lw->web_line_st = 1 ;
		em->pl_stance = STANCE_STAND ;
		pl_char_set( em, Char_Damage, 2 ) ;
		pl_w->catch_no = 0xc4 ;
		pl_w->catch_grip_old = -1 ;
		pl_ef_catch_set( ew, em ) ;
		em->catch_type = 12 ;
		pls0e_04_set( pl_w, em, 0 ) ;
		lw->web_em_w = em ;
	}
}

/*==============================================================*/
static PLAYER_WORK *shl0e_01_catch_ck( PLAYER_WORK *ew )
{
	PLAYER_WORK *em ;

	if( !pl_sp_catch_ck( ew ) ) return 0 ;
	em = ew->em_w ;
	if( em->dm_atck_no != pl0e_at_web ) return 0 ;
	return em ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl0e_01.c" End of File	*/

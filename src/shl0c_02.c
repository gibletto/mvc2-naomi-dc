/* The web of Spider-Man's Ultimate Web Throw (character 0c's shot 02): an unseen grab in front of him
   that catches the opponent for the hyper combo, or misses. */
#include "player.h"
#include "pl_char.h"
#include "pl0c_def.h"

static void shl0c_02_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl0c_02_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl0c_02_main ;
		ew->ef_no = 0x0c02 ;
		ew->master_w = pl_w ;
	}
	return ew ;
}

static void shl0c_02_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0c_02_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0c_02_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl0c_02_main( PLAYER_WORK *ew )
{
	static void ( *shl0c_02_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0c_02_init, shl0c_02_move, shl0c_02_free,
	} ;

	shl0c_02_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void shl0c_02_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float ofs ;

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
	ew->dm_flag = HIT_ON ;
	ofs = -DOT_X( 50.0f ) ;
	if( pl_w->pl_dir ) ofs = DOT_X( 50.0f ) ;
	ew->pos.x = pl_w->pos.x + ofs ;
	ew->pos.y = pl_w->pos.y + 90.0f ;
	ew->atck_no = pl0c_at_hwebthrow ;
	HIT_RESET( ew ) ;
	hit_work_set( ew ) ;
	pl_char_set( ew, Char_Effect, 23 ) ;
}

/*--------------------------------------------------------------*/
static void shl0c_02_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0C_LOCAL_W *lw = ( PL0C_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *em ;
	Float ofs ;

	if( !ew->routine_1 ){
		ofs = -DOT_X( 50.0f ) ;
		if( pl_w->pl_dir ) ofs = DOT_X( 50.0f ) ;
		ew->pos.x = pl_w->pos.x + ofs ;
		ew->pos.y = pl_w->pos.y + 90.0f ;
		if( ew->hit_stop ){
			ew->hit_stop -- ;
			return ;
		}
		if( pl_sp_catch_ck( ew ) ){
			em = ew->em_w ;
			if( em->dm_atck_no == pl0c_at_hwebthrow ){
				ew->routine_1 ++ ;
				pl_w->catch_flag = 1 ;
				pl_w->catch_ng_timer = 32 ;
				pl_w->dm_flag = 0 ;
				pl_w->catch_no = 0xc5 ;
				pl_w->catch_grip_old = -1 ;
				lw->hweb_st = 1 ;
				em->wall_ng_timer = 2 ;
				pl_ef_catch_set( ew, em ) ;
				pl_char_set( ew, Char_Effect, 24 ) ;
				return ;
			}
		} else if( pl_char_move( ew ) >= 0 ){
			hit_work_set( ew ) ;
			return ;
		}
		ew->routine_1 ++ ;
		lw->hweb_st = -1 ;
		pl_char_set( ew, Char_Effect, 24 ) ;
	} else if( pl_char_move( ew ) < 0 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl0c_02_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl0c_02.c" End of File	*/

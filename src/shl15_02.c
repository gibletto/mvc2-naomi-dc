/* Amingo's effect 02 (character 15's shot 02): the grabbing arm of Shokubutsu no Okite (sp_okite).
   It stays out while his animation does, seizes the opponent it touches and holds him to Amingo. */
#include "player.h"
#include "pl_char.h"
#include "pl15_def.h"

static void shl15_02_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl15_02_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl15_02_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
		ew->ef_no = 0x1502 ;
		ew->timer1 = pl_w->char_no.w ;
	}
	return ew ;
}

static void shl15_02_init( PLAYER_WORK *ew );
static void shl15_02_move( PLAYER_WORK *ew );
static void shl15_02_end( PLAYER_WORK *ew );
static void shl15_02_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl15_02_main( PLAYER_WORK *ew )
{
	static void ( *shl15_02_jmp[] )( PLAYER_WORK *ew ) = {
		shl15_02_init, shl15_02_move, shl15_02_end, shl15_02_free,
	} ;

	shl15_02_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl15_02_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	Float x ;

	ew->routine_0 ++ ;
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
	ew->pos = pl_w->pos ;
	x = -DOT_X( 64.0f ) ;
	if( pl_w->disp_dir ) x = DOT_X( 64.0f ) ;
	ew->pos.x += x ;
	ew->atck_flag = HIT_ON | HIT_PL_ONLY ;
	ew->dm_flag = 0 ;
	ew->atck_no = pl15_at_okite_reach ;
	HIT_RESET( ew ) ;
	ew->size_back = 32 ;
	ew->size_front = 32 ;
	ew->prio = 0 ;
	pl_char_set( ew, Char_Effect, 14 ) ;
}

static void shl15_02_out( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl15_02_reach( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl15_02_back( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl15_02_catch( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl15_02_move( PLAYER_WORK *ew )
{
	static void ( *shl15_02_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl15_02_out, shl15_02_reach, shl15_02_back, shl15_02_catch,
	} ;

	shl15_02_move_jmp[ew->routine_1]( ew, ew->master_w ) ;
}

/*==============================================================*/
/*			It goes if Amingo has left the animation it was		*/
/*			sent in												*/
/*==============================================================*/
static void shl15_02_out( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ( Sint16 )pl_w->char_no.w != ew->timer1 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	pl_char_move( ew ) ;
	if( ew->cg_type ){
		ew->routine_1 ++ ;
		ew->spd.x = 0 ;
		ew->spd.y = 0 ;
		ew->acc.x = 0 ;
		ew->acc.y = 0 ;
		ew->spd.x = -DOT_X( 2.5f ) ;
		if( ew->disp_dir ) ew->spd.x = -ew->spd.x ;
	}
}

/*==============================================================*/
/*			It reaches out and may seize the opponent			*/
/*==============================================================*/
static void shl15_02_reach( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w ;

	if( ( Sint16 )pl_w->char_no.w != ew->timer1 ){
		if( pl_w->char_no.b.grp == Char_Super ) ew->routine_1 = 2 ;
		else {
			ew->routine_0 = 2 ;
			ew->disp_flag = 0 ;
		}
		return ;
	}
	if( !scr_in_x_ck( ew ) ) pl_w->timer0 = 0 ;
	if( ew->hit_flag ){
		if( pl_sp_catch_ck( ew ) ){
			ew->routine_1 = 3 ;
			ew->routine_2 = 0 ;
			pl_char_set( ew, 15, 2 ) ;
			ew->type = 0xff ;
			pl_w->catch_no = 0xc1 ;
			pl_w->dm_flag = 0 ;
			pl_w->catch_ng_timer = 3 ;
			pl_w->catch_flag = 1 ;
			pl_w->routine_2 = 3 ;
			pl_w->routine_3 = 0 ;
			em_w = ew->em_w ;
			pl_ef_catch_set( ew, em_w ) ;
			catch_pos_set( ew, em_w ) ;
		}
		else {
			ew->routine_1 = 2 ;
			pl_w->timer0 = 0 ;
		}
	}
	else {
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		pl_char_move( ew ) ;
		hit_work_set( ew ) ;
	}
}

/*==============================================================*/
/*			It draws back										*/
/*==============================================================*/
static void shl15_02_back( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->routine_2 ){
		ew->routine_2 ++ ;
		pl_char_set( ew, Char_Effect, 15 ) ;
	}
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

static void shl15_02_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl15_02_hold_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl15_02_catch( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl15_02_catch_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl15_02_hold, shl15_02_hold_end,
	} ;

	shl15_02_catch_jmp[ew->routine_2]( ew, pl_w ) ;
}

/*==============================================================*/
/*			It holds the opponent where Amingo's animation		*/
/*			says												*/
/*==============================================================*/
static void shl15_02_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type != ew->type ){
		ew->type = pl_w->cg_type ;
		pl_char_set2( ew, 15, 2, ew->type - 1 ) ;
	}
	catch_pos_set( ew, ew->catch_pair.w ) ;
	if( ew->cg_type || pl_w->act_st != N_SUPER ) ew->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void shl15_02_hold_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl15_02_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl15_02_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl15_02.c" End of File	*/

/* War Machine's effect 04 (character 2e's shot 04): the beam of Iron Man's Proton Cannon
   (shl33_04), which nothing of War Machine's sets. Type 0 is the head at the cannon's mouth, which
   lays six parts in a row and then keeps firing them while he is in Char_Super; type 1 is a part,
   which flies out from it. */
#include "player.h"
#include "pl_char.h"
#include "pl2e_def.h"

static void shl2e_04_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl2e_04_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl2e_04_main ;
		ew->ef_no = 0x2e04 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = a ;
		ew->sub_type = b ;
	}
	return ew ;
}

/*==============================================================*/
/*			A part that rides on the effect hw: its place,		*/
/*			and the turn of its attack							*/
/*==============================================================*/
static void *shl2e_04_part_set( PLAYER_WORK *hw, Uint8 a, Uint8 b, Sint16 c, Uint8 d )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_END ) ) ){
		ew->work_func = shl2e_04_main ;
		ew->ef_no = 0x2e04 ;
		ew->master_w = hw->master_w ;
		ew->pl_type = hw->pl_type ;
		ew->ef_w = hw ;
		ew->type = a ;
		ew->sub_type = b ;
		ew->spd.x = c ;
		ew->timer1 = d ;
	}
	return ew ;
}

static void shl2e_04_head_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_04_part_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl2e_04_main( PLAYER_WORK *ew )
{
	static void ( *shl2e_04_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2e_04_head_main, shl2e_04_part_main,
	} ;

	shl2e_04_jmp[ew->type]( ew, ew->master_w ) ;
}

static void shl2e_04_head_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_04_head_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_04_head_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_04_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl2e_04_head_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl2e_04_head_main_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2e_04_head_init, shl2e_04_head_move, shl2e_04_head_end, shl2e_04_free,
	} ;

	shl2e_04_head_main_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl2e_04_head_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->disp_flag = 1 ;
	ew->sub_prio = -2 ;
	ew->atck_no = pl2e_at_proton_beam ;
	HIT_RESET( ew ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	pl_char_set( ew, Char_Effect, 19 ) ;
	ew->sub_no = 0 ;
	ew->timer0 = 4 ;
	shl2e_04_head_move( ew, pl_w ) ;
}

static void shl2e_04_open( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_04_fire( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_04_close( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl2e_04_head_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl2e_04_head_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2e_04_open, shl2e_04_fire, shl2e_04_close,
	} ;

	if( pl_w->char_no.b.grp != Char_Super || pl_w->routine_1 ){
		ew->routine_0 ++ ;
		shl2e_04_head_end( ew, pl_w ) ;
		return ;
	}
	ew->prio = pl_w->prio ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	if( !ew->disp_dir ) ew->pos.x += -DOT_X( 107.0f ) ;
	else ew->pos.x += DOT_X( 107.0f ) ;
	ew->pos.y += DOT_Y( 95.0f ) ;
	if( vs_sys_work.slow_bit & 1 << vs_sys_work.slow_step ) return ;
	shl2e_04_head_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl2e_04_open( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Sint16 x ;
	int i ;

	pl_char_move( ew ) ;
	if( !-- ew->timer0 ){
		ew->routine_1 ++ ;
		ew->sub_type = 7 ;
		ew->timer1 = 0 ;
		x = -170 ;
		for( i = 6 ; i > 0 ; i -- ){
			shl2e_04_part_set( ew, 1, ew->sub_type, x, ew->timer1 ) ;
			x -= 50.0f ;
			ew->timer1 ++ ;
			ew->timer1 &= 3 ;
		}
		ew->sub_type = 0 ;
		pl_se_req( ew, 72 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl2e_04_fire( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL2E_LOCAL_W *lw = ( PL2E_LOCAL_W * )&pl_w->tlocal[0] ;

	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
	if( ew->cg_type ){
		ew->cg_type = 0 ;
		ew->atck_no = pl2e_at_proton_beam ;
		HIT_RESET( ew ) ;
	}
	shl2e_04_part_set( ew, 1, ew->sub_type, -170, ew->timer1 ) ;
	ew->sub_type ++ ;
	ew->sub_type &= 7 ;
	ew->timer1 ++ ;
	ew->timer1 &= 3 ;
	if( lw->proton_end_flag ){
		ew->routine_1 ++ ;
		pl_char_set( ew, Char_Effect, 20 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl2e_04_close( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		shl2e_04_head_end( ew, pl_w ) ;
	}
}

static void shl2e_04_part_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_04_part_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_04_part_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl2e_04_part_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl2e_04_part_main_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2e_04_part_init, shl2e_04_part_move, shl2e_04_part_end, shl2e_04_free,
	} ;

	shl2e_04_part_main_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl2e_04_part_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *hw = ew->ef_w ;

	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, hw ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = hw->pl_no ;
	ew->pl_type = hw->pl_type ;
	ew->scale.x = hw->scale.x ;
	ew->scale.y = hw->scale.y ;
	ew->atck_level = hw->atck_level ;
	ew->work_no = hw->work_no ;
	ew->col_cnt = hw->col_cnt ;
	ew->scale = hw->scale ;
	ew->prio = hw->prio ;
	ew->disp_flag = 1 ;
	ew->sub_prio = -2 ;
	if( !ew->timer1 ){
		ew->atck_flag = HIT_ON | HIT_SHOT ;
		hw->sub_no ++ ;
		hw->sub_no &= 3 ;
		ew->atck_no = hw->sub_no + 55 ;
		HIT_RESET( ew ) ;
		ew->dm_flag = HIT_ON | HIT_SHOT ;
	}
	ew->size_front = ew->size_back = 26 ;
	pl_char_set( ew, Char_Effect, ew->sub_type + 21 ) ;
	shl2e_04_part_move( ew, pl_w ) ;
}

static void shl2e_04_beam( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_04_beam_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl2e_04_part_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl2e_04_part_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2e_04_beam, shl2e_04_beam_end,
	} ;

	if( pl_w->char_no.b.grp != Char_Super || pl_w->routine_1 ){
		ew->routine_0 ++ ;
		shl2e_04_part_end( ew, pl_w ) ;
		return ;
	}
	ew->prio = pl_w->prio ;
	if( vs_sys_work.slow_bit & 1 << vs_sys_work.slow_step ) return ;
	shl2e_04_part_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

static void shl2e_04_pos_set( PLAYER_WORK *ew, PLAYER_WORK *hw );

/*==============================================================*/
static void shl2e_04_beam( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL2E_LOCAL_W *lw = ( PL2E_LOCAL_W * )&pl_w->tlocal[0] ;

	shl2e_04_pos_set( ew, ew->ef_w ) ;
	pl_char_move( ew ) ;
	if( !ew->timer1 ) hit_work_set( ew ) ;
	if( lw->proton_end_flag ){
		ew->routine_1 ++ ;
		pl_char_set( ew, Char_Effect, 29 ) ;
	}
	if( !scr_in_x_ck( ew ) ){
		ew->routine_0 ++ ;
		shl2e_04_part_end( ew, pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl2e_04_beam_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		shl2e_04_part_end( ew, pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl2e_04_head_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl2e_04_part_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl2e_04_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*			The part keeps its place beside the one it rides	*/
/*			on, and falls back by spd.x							*/
/*==============================================================*/
static void shl2e_04_pos_set( PLAYER_WORK *ew, PLAYER_WORK *hw )
{
	ew->pos.x = hw->pos.x ;
	ew->pos.y = hw->pos.y ;
	ew->pos.z = hw->pos.z ;
	if( !ew->disp_dir ) ew->pos.x += ew->spd.x ;
	else ew->pos.x -= ew->spd.x ;
	ew->spd.x -= 50.0f ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl2e_04.c" End of File	*/

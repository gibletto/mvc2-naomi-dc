/* War Machine's effect 00 (character 2e's shot 00): the beam of his crouching heavy punch, while he
   is in Char_Crouch_P. Type 0 is the flash where the beam starts, which puts out its parts; types 1
   and 2 are the parts, each riding on the one before. */
#include "player.h"
#include "pl_char.h"
#include "pl2e_def.h"

/* where the flash stands from War Machine, x and y by sub_type (crouching heavy punch: 2) */
static Sint16 shl2e_00_pos_tbl[] = {
	-23, 89, -37, 98, -50, 57,
} ;

static void shl2e_00_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl2e_00_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl2e_00_main ;
		ew->ef_no = 0x2e00 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = a ;
		ew->sub_type = b ;
	}
	return ew ;
}

/*==============================================================*/
/*			A part that rides on the effect hw					*/
/*==============================================================*/
static void *shl2e_00_part_set( PLAYER_WORK *hw, Uint8 a, Uint8 b, Uint8 c )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_END ) ) ){
		ew->work_func = shl2e_00_main ;
		ew->ef_no = 0x2e00 ;
		ew->master_w = hw->master_w ;
		ew->ef_w = hw ;
		ew->pl_type = hw->pl_type ;
		ew->type = a ;
		ew->sub_type = b ;
		ew->sub_no = c ;
	}
	return ew ;
}

static void shl2e_00_head_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_00_part_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl2e_00_main( PLAYER_WORK *ew )
{
	static void ( *shl2e_00_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2e_00_head_main, shl2e_00_part_main, shl2e_00_part_main,
	} ;

	shl2e_00_jmp[ew->type]( ew, ew->master_w ) ;
}

static void shl2e_00_head_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_00_head_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_00_head_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl2e_00_head_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl2e_00_head_main_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2e_00_head_init, shl2e_00_head_move, shl2e_00_head_end, shl2e_00_free,
	} ;

	shl2e_00_head_main_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl2e_00_head_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->atck_no = pl2e_at_crouch_hp ;
	HIT_RESET( ew ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	pl_char_set( ew, Char_Effect, 5 ) ;
	ew->timer0 = 1 ;
	shl2e_00_head_move( ew, pl_w ) ;
}

static void shl2e_00_open( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_00_fire( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_00_close( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl2e_00_head_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl2e_00_head_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2e_00_open, shl2e_00_fire, shl2e_00_close,
	} ;

	ew->prio = pl_w->prio ;
	if( pl_w->char_no.b.grp != Char_Crouch_P ){
		ew->routine_0 ++ ;
		shl2e_00_head_end( ew, pl_w ) ;
		return ;
	}
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	if( !ew->disp_dir ) ew->pos.x += DOT_X( shl2e_00_pos_tbl[ew->sub_type * 2] ) ;
	else ew->pos.x -= DOT_X( shl2e_00_pos_tbl[ew->sub_type * 2] ) ;
	ew->pos.y += DOT_Y( shl2e_00_pos_tbl[ew->sub_type * 2 + 1] ) ;
	shl2e_00_head_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl2e_00_open( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	shl2e_00_part_set( ew, 2, 0, 0 ) ;
	ew->routine_1 ++ ;
}

/*--------------------------------------------------------------*/
static void shl2e_00_fire( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL2E_LOCAL_W *lw = ( PL2E_LOCAL_W * )&pl_w->tlocal[0] ;

	pl_char_move( ew ) ;
	ew->timer0 ++ ;
	shl2e_00_part_set( ew, 1, ew->timer0 & 1, ew->timer0 & 3 ) ;
	if( lw->beam_end_flag ){
		ew->routine_1 ++ ;
		pl_char_set( ew, Char_Effect, 6 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl2e_00_close( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		shl2e_00_head_end( ew, pl_w ) ;
	}
}

static void shl2e_00_part_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_00_part_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_00_part_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl2e_00_part_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl2e_00_part_main_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2e_00_part_init, shl2e_00_part_move, shl2e_00_part_end, shl2e_00_free,
	} ;

	shl2e_00_part_main_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl2e_00_part_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->atck_no = pl2e_at_crouch_hp_2 ;
	HIT_RESET( ew ) ;
	if( !ew->sub_no ) ew->atck_flag = HIT_ON | HIT_SHOT ;
	else ew->atck_flag = HIT_ON | HIT_TOUCH ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->size_front = ew->size_back = 26 ;
	ew->pos.x = hw->pos.x ;
	ew->pos.y = hw->pos.y ;
	ew->pos.z = hw->pos.z ;
	if( !ew->disp_dir ) ew->pos.x += -DOT_X( 32.0f ) ;
	else ew->pos.x += DOT_X( 32.0f ) ;
	if( !ew->disp_dir ) ew->acc.x = -50.0f ;
	else ew->acc.x = 50.0f ;
	ew->spd.x = 0 ;
	if( ew->type == 2 ) pl_char_set( ew, Char_Effect, 0 ) ;
	else if( !ew->sub_type ) pl_char_set( ew, Char_Effect, 2 ) ;
	else pl_char_set( ew, Char_Effect, 3 ) ;
	shl2e_00_part_move( ew, pl_w ) ;
}

static void shl2e_00_beam_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_00_beam_tail( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl2e_00_part_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl2e_00_part_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2e_00_beam_move, shl2e_00_beam_tail,
	} ;

	ew->prio = pl_w->prio ;
	if( pl_w->char_no.b.grp != Char_Crouch_P ){
		ew->routine_0 ++ ;
		shl2e_00_part_end( ew, pl_w ) ;
		return ;
	}
	shl2e_00_part_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

static void shl2e_00_pos_set( PLAYER_WORK *ew, PLAYER_WORK *hw );

/*==============================================================*/
static void shl2e_00_beam_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL2E_LOCAL_W *lw = ( PL2E_LOCAL_W * )&pl_w->tlocal[0] ;
	PLAYER_WORK *hw = ew->ef_w ;

	ew->spd.x += ew->acc.x ;
	shl2e_00_pos_set( ew, hw ) ;
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
	if( lw->beam_end_flag ){
		ew->routine_1 ++ ;
		if( ew->type == 2 ) pl_char_set( ew, Char_Effect, 1 ) ;
		else pl_char_set( ew, Char_Effect, 4 ) ;
		return ;
	}
	if( !scr_in_x_ck( ew ) ){
		ew->routine_0 ++ ;
		shl2e_00_part_end( ew, pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl2e_00_beam_tail( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	shl2e_00_pos_set( ew, ew->ef_w ) ;
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		shl2e_00_part_end( ew, pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl2e_00_head_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	pl_voice_req( ew, 43 ) ;
}

/*--------------------------------------------------------------*/
static void shl2e_00_part_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl2e_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*			The part keeps its place beside the one it rides	*/
/*			on, and moves on by spd.x							*/
/*==============================================================*/
static void shl2e_00_pos_set( PLAYER_WORK *ew, PLAYER_WORK *hw )
{
	ew->pos.x = hw->pos.x ;
	ew->pos.y = hw->pos.y ;
	ew->pos.z = hw->pos.z ;
	if( ew->type == 2 ){
		if( !ew->disp_dir ) ew->pos.x += -DOT_X( 16.0f ) ;
		else ew->pos.x += DOT_X( 16.0f ) ;
	} else {
		if( !ew->disp_dir ) ew->pos.x += -DOT_X( 32.0f ) ;
		else ew->pos.x += DOT_X( 32.0f ) ;
	}
	ew->pos.x += ew->spd.x ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl2e_00.c" End of File	*/

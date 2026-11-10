/* Iron Man's effect 00 (character 33's shot 00): the Unibeam, on the ground (sub_type 0) and in the
   air (sub_type 1). Type 0 is the flash at his chest, which puts out the beam's parts; types 1 and
   2 are the parts, each riding on the one before and flying out ahead. */
#include "player.h"
#include "pl_char.h"
#include "pl33_def.h"

/* where the flash sits from Iron Man, x and y, by the shot's sub_type */
static Sint16 shl33_00_ofs_tbl[] = {
	-12, 80, -23, 82,
} ;

static void shl33_00_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl33_00_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl33_00_main ;
		ew->ef_no = 0x3300 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = a ;
		ew->sub_type = b ;
	}
	return ew ;
}

/*==============================================================*/
/*			A work that rides on the effect hw					*/
/*==============================================================*/
static void *shl33_00_part_set( PLAYER_WORK *hw, Uint8 a, Uint8 b, Uint8 c )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_END ) ) ){
		ew->work_func = shl33_00_main ;
		ew->ef_no = 0x3300 ;
		ew->master_w = hw->master_w ;
		ew->ef_w = hw ;
		ew->pl_type = hw->pl_type ;
		ew->type = a ;
		ew->sub_type = b ;
		ew->sub_no = c ;
	}
	return ew ;
}

static void shl33_00_head_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl33_00_part_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl33_00_main( PLAYER_WORK *ew )
{
	static void ( *shl33_00_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl33_00_head_main, shl33_00_part_main, shl33_00_part_main,
	} ;

	shl33_00_jmp[ew->type]( ew, ew->master_w ) ;
}

static void shl33_00_head_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl33_00_head_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl33_00_head_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl33_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl33_00_head_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl33_00_head_main_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl33_00_head_init, shl33_00_head_move, shl33_00_head_end, shl33_00_free,
	} ;

	shl33_00_head_main_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl33_00_head_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->atck_no = ew->atck_level + 53 ;
	HIT_RESET( ew ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	pl_char_set( ew, Char_Effect, 5 ) ;
	ew->timer0 = 1 ;
	pl_voice_req( ew, 35 ) ;
	shl33_00_head_move( ew, pl_w ) ;
}

static void shl33_00_open( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl33_00_fire( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl33_00_close( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			It lasts while Iron Man is in the beam's			*/
/*			animation											*/
/*==============================================================*/
static void shl33_00_head_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl33_00_head_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl33_00_open, shl33_00_fire, shl33_00_close,
	} ;

	ew->prio = pl_w->prio ;
	if( pl_w->char_no.b.grp != Char_Special || pl_w->routine_1 ){
		ew->routine_0 ++ ;
		shl33_00_head_end( ew, pl_w ) ;
		return ;
	}
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	if( !ew->disp_dir ) ew->pos.x += DOT_X( shl33_00_ofs_tbl[ew->sub_type * 2] ) ;
	else ew->pos.x -= DOT_X( shl33_00_ofs_tbl[ew->sub_type * 2] ) ;
	ew->pos.y += DOT_Y( shl33_00_ofs_tbl[ew->sub_type * 2 + 1] ) ;
	shl33_00_head_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl33_00_open( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	shl33_00_part_set( ew, 2, 0, 0 ) ;
	ew->routine_1 ++ ;
}

/*--------------------------------------------------------------*/
static void shl33_00_fire( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL33_LOCAL_W *lw = ( PL33_LOCAL_W * )&pl_w->tlocal[0] ;

	pl_char_move( ew ) ;
	ew->timer0 ++ ;
	shl33_00_part_set( ew, 1, ew->timer0 & 1, ew->timer0 & 3 ) ;
	if( lw->beam_end_flag ){
		ew->routine_1 ++ ;
		pl_char_set( ew, Char_Effect, 6 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl33_00_close( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		shl33_00_head_end( ew, pl_w ) ;
	}
}

static void shl33_00_part_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl33_00_part_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl33_00_part_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl33_00_part_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl33_00_part_main_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl33_00_part_init, shl33_00_part_move, shl33_00_part_end, shl33_00_free,
	} ;

	shl33_00_part_main_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl33_00_part_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->atck_no = ew->atck_level + 48 ;
	HIT_RESET( ew ) ;
	if( !ew->sub_no ) ew->atck_flag = HIT_ON | HIT_SHOT ;
	else ew->atck_flag = HIT_ON | HIT_TOUCH ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->size_front = ew->size_back = 13 ;
	ew->pos.x = hw->pos.x ;
	ew->pos.y = hw->pos.y ;
	ew->pos.z = hw->pos.z ;
	if( !ew->disp_dir ) ew->pos.x += -DOT_X( 32.0f ) ;
	else ew->pos.x += DOT_X( 32.0f ) ;
	if( !ew->disp_dir ) ew->acc.x = -50.0f ;
	else ew->acc.x = 50.0f ;
	ew->spd.x = 0.0f ;
	if( ew->type == 2 ) pl_char_set( ew, Char_Effect, 0 ) ;
	else if( !ew->sub_type ) pl_char_set( ew, Char_Effect, 2 ) ;
	else pl_char_set( ew, Char_Effect, 3 ) ;
	shl33_00_part_move( ew, pl_w ) ;
}

static void shl33_00_beam_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl33_00_beam_tail( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl33_00_part_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl33_00_part_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl33_00_beam_move, shl33_00_beam_tail,
	} ;

	ew->prio = pl_w->prio ;
	if( pl_w->char_no.b.grp != Char_Special || pl_w->routine_1 ){
		ew->routine_0 ++ ;
		shl33_00_part_end( ew, pl_w ) ;
		return ;
	}
	shl33_00_part_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

static void shl33_00_pos_set( PLAYER_WORK *ew, PLAYER_WORK *hw );

/*==============================================================*/
static void shl33_00_beam_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL33_LOCAL_W *lw = ( PL33_LOCAL_W * )&pl_w->tlocal[0] ;
	PLAYER_WORK *ef = ew->ef_w ;

	ew->spd.x += ew->acc.x ;
	shl33_00_pos_set( ew, ef ) ;
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
		shl33_00_part_end( ew, pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl33_00_beam_tail( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	shl33_00_pos_set( ew, ew->ef_w ) ;
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		shl33_00_part_end( ew, pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl33_00_head_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	pl_voice_req( ew, 43 ) ;
}

/*--------------------------------------------------------------*/
static void shl33_00_part_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl33_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
static void shl33_00_pos_set( PLAYER_WORK *ew, PLAYER_WORK *hw )
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
/* "shl33_00.c" End of File	*/

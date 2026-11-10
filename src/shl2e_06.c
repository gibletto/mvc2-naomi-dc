/* War Machine's effect 06 (character 2e's shot 06): the flash at the mouth of his Proton Cannon
   while he is in Char_Super (type 0), which keeps throwing out shots (type 1) until the firing is
   over. */
#include "player.h"
#include "pl_char.h"
#include "pl2e_def.h"

/* where a shot starts above or below the burst, by a random 0 to 15 */
static Sint16 shl2e_06_y_tbl[] = {
	20, 24, 16, 12, 4, 0, 8, -4, -16, -8, -32, -24, -36, -40, -44, -48,
} ;

static void shl2e_06_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl2e_06_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl2e_06_main ;
		ew->ef_no = 0x2e06 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = a ;
		ew->sub_type = b ;
	}
	return ew ;
}

/*==============================================================*/
/*			A shot that rides on the effect hw					*/
/*==============================================================*/
static void *shl2e_06_part_set( PLAYER_WORK *hw, Uint8 a, Uint8 b )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_END ) ) ){
		ew->work_func = shl2e_06_main ;
		ew->ef_no = 0x2e06 ;
		ew->master_w = hw->master_w ;
		ew->pl_type = hw->pl_type ;
		ew->ef_w = hw ;
		ew->type = a ;
		ew->sub_type = b ;
	}
	return ew ;
}

static void shl2e_06_head_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_06_part_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl2e_06_main( PLAYER_WORK *ew )
{
	static void ( *shl2e_06_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2e_06_head_main, shl2e_06_part_main,
	} ;

	shl2e_06_jmp[ew->type]( ew, ew->master_w ) ;
}

static void shl2e_06_head_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_06_head_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_06_head_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_06_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl2e_06_head_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl2e_06_head_main_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2e_06_head_init, shl2e_06_head_move, shl2e_06_head_end, shl2e_06_free,
	} ;

	shl2e_06_head_main_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl2e_06_head_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->disp_flag = 0 ;
	ew->sub_prio = -2 ;
	ew->timer0 = 4 ;
	shl2e_06_head_move( ew, pl_w ) ;
}

static void shl2e_06_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_06_fire( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl2e_06_head_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl2e_06_head_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2e_06_wait, shl2e_06_fire,
	} ;

	if( pl_w->char_no.b.grp != Char_Super || pl_w->routine_1 ){
		ew->routine_0 ++ ;
		shl2e_06_head_end( ew, pl_w ) ;
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
	shl2e_06_head_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl2e_06_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !-- ew->timer0 ){
		ew->routine_1 ++ ;
		ew->sub_type = 0 ;
		ew->timer0 = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl2e_06_fire( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL2E_LOCAL_W *lw = ( PL2E_LOCAL_W * )&pl_w->tlocal[0] ;

	if( !ew->timer0 -- ){
		ew->timer0 = 2 ;
		shl2e_06_part_set( ew, 1, ew->sub_type ) ;
		ew->sub_type ++ ;
		ew->sub_type &= 1 ;
	}
	if( lw->proton_end_flag ){
		ew->routine_0 ++ ;
		shl2e_06_head_end( ew, pl_w ) ;
	}
}

static void shl2e_06_part_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_06_part_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_06_part_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl2e_06_part_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl2e_06_part_main_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2e_06_part_init, shl2e_06_part_move, shl2e_06_part_end, shl2e_06_free,
	} ;

	shl2e_06_part_main_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl2e_06_part_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *hw = ew->ef_w ;
	Sint16 no ;

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
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	if( !ew->sub_type ){
		ew->atck_no = pl2e_at_proton_shot ;
		HIT_RESET( ew ) ;
	} else {
		ew->atck_no = pl2e_at_proton_shot2 ;
		HIT_RESET( ew ) ;
	}
	ew->size_front = ew->size_back = 26 ;
	ew->pos.x = hw->pos.x ;
	ew->pos.y = hw->pos.y ;
	ew->pos.z = hw->pos.z ;
	no = Rnd() & 15 ;
	ew->pos.y += DOT_Y( shl2e_06_y_tbl[no] ) ;
	ew->spd.x = -DOT_X( 14.0f ) ;
	ew->acc.x = -DOT_X( 0.125f ) ;
	if( ew->disp_dir ){
		ew->spd.x = -ew->spd.x ;
		ew->acc.x = -ew->acc.x ;
	}
	pl_char_set( ew, Char_Effect, 41 ) ;
	shl2e_06_part_move( ew, pl_w ) ;
}

static void shl2e_06_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_06_burst( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl2e_06_part_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl2e_06_part_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2e_06_fly, shl2e_06_burst,
	} ;

	ew->prio = pl_w->prio ;
	shl2e_06_part_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl2e_06_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	pl_char_move( ew ) ;
	if( ew->hit_box_no ){
		ew->routine_1 ++ ;
		return ;
	}
	hit_work_set( ew ) ;
	if( ew->hit_flag ){
		ew->routine_1 ++ ;
		return ;
	}
	if( !scr_in_x_ck( ew ) ){
		ew->routine_0 ++ ;
		shl2e_06_part_end( ew, pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl2e_06_burst( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	burst_set( ew, &ew->pos, 1, 8 ) ;
	pl_se_req( ew, 73 ) ;
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl2e_06_head_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl2e_06_part_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl2e_06_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl2e_06.c" End of File	*/

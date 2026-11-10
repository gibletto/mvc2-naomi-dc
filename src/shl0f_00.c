/* Doctor Doom's Electric Cage (character 0f's shot 00): it flies while Doom holds the pose, and when it
   strikes a player it holds him with a hit every 8 frames, and a last one. */
#include "player.h"
#include "pl_char.h"
#include "pl0f_def.h"

static void shl0f_00_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl0f_00_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl0f_00_main ;
		ew->ef_no = 0x0f00 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = a ;
		ew->sub_type = b ;
	}
	return ew ;
}

static void shl0f_00_0_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl0f_00_main( PLAYER_WORK *ew )
{
	static void ( *shl0f_00_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0f_00_0_main,
	} ;

	shl0f_00_jmp[ew->type]( ew, ew->master_w ) ;
}

static void shl0f_00_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0f_00_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0f_00_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0f_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl0f_00_0_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0f_00_0_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0f_00_init, shl0f_00_move, shl0f_00_end, shl0f_00_free,
	} ;

	shl0f_00_0_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl0f_00_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->sub_prio = 1 ;
	ew->prio = 0 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	if( !ew->disp_dir ){
		ew->pos.x += -DOT_X( 112.0f ) ;
	} else {
		ew->pos.x -= -DOT_X( 112.0f ) ;
	}
	ew->pos.y += DOT_Y( 80.0f ) ;
	ew->size_front = ew->size_back = 93 ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.x = -DOT_X( 8.0f ) ;
	if( ew->disp_dir ) ew->spd.x = -ew->spd.x ;
	ew->atck_no = pl0f_at_cage ;
	HIT_RESET( ew ) ;
	ew->atck_no |= 16 ;
	ew->atck_flag = HIT_ON | HIT_SHOT | HIT_OWNER ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	pl_char_set( ew, Char_Effect, 0 ) ;
	shl0f_00_move( ew, pl_w ) ;
}

static void shl0f_00_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0f_00_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl0f_00_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0f_00_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0f_00_fly, shl0f_00_hold,
	} ;

	if( pl_w->char_no.b.grp != Char_Super ){
		ew->routine_0 ++ ;
		return ;
	}
	if( !scr_in_x_ck( ew ) ){
		ew->routine_0 ++ ;
		return ;
	}
	shl0f_00_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl0f_00_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
	if( ew->hit_flag ){
		ew->atck_no = pl0f_at_cage ;
		HIT_RESET( ew ) ;
		em = ew->em_w ;
		if( em->work_kind ) return ;
		ew->routine_1 ++ ;
		pl_char_set( ew, Char_Effect, 1 ) ;
		ew->timer0 = 90 ;
	}
}

/*--------------------------------------------------------------*/
static void shl0f_00_hold( register PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	Sint16 dx, dx_em ;

	pl_char_move( ew ) ;
	if( !ew->cg_type ){
		ew->prio = 10 ;
	} else {
		ew->prio = 13 ;
	}
	hit_work_set( ew ) ;
	if( !( ew->timer0 & 7 ) ){
		em = pl_w->target_w ;
		dx = pl_w->pos.x - ew->pos.x ;
		if( dx < 0 ) dx = -dx ;
		dx_em = pl_w->pos.x - em->pos.x ;
		if( dx_em < 0 ) dx_em = -dx_em ;
		if( dx < dx_em ){
			ew->atck_no = pl0f_at_cage ;
			HIT_RESET( ew ) ;
			ew->atck_attr |= ATR_BACK_POS ;
		} else {
			ew->atck_no = pl0f_at_cage ;
			HIT_RESET( ew ) ;
		}
	}
	if( !ew->timer0 -- ){
		ew->atck_no = pl0f_at_cage_last ;
		HIT_RESET( ew ) ;
		ew->routine_0 ++ ;
	}
}

static void shl0f_00_end_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0f_00_end_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl0f_00_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0f_00_end_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0f_00_end_set, shl0f_00_end_wait,
	} ;

	shl0f_00_end_jmp[ew->routine_2]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl0f_00_end_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0F_LOCAL_W *lw = PL0F_LW( pl_w ) ;

	ew->routine_2 ++ ;
	lw->cage_end_flag = 1 ;
	pl_char_set( ew, Char_Effect, 2 ) ;
	shl0f_00_end_wait( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl0f_00_end_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl0f_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl0f_00.c" End of File	*/

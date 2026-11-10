/* Doctor Doom's Sphere Flame (character 0f's shot 01): a hit that rises from above him while he holds the
   pose, and when it strikes, slows and hits every 8 frames. */
#include "player.h"
#include "pl_char.h"
#include "pl0f_def.h"

static void shl0f_01_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl0f_01_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl0f_01_main ;
		ew->ef_no = 0x0f01 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = a ;
		ew->sub_type = b ;
	}
	return ew ;
}

static void shl0f_01_0_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl0f_01_main( PLAYER_WORK *ew )
{
	static void ( *shl0f_01_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0f_01_0_main,
	} ;

	shl0f_01_jmp[ew->type]( ew, ew->master_w ) ;
}

static void shl0f_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0f_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0f_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0f_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl0f_01_0_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0f_01_0_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0f_01_init, shl0f_01_move, shl0f_01_end, shl0f_01_free,
	} ;

	shl0f_01_0_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl0f_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	if( !ew->disp_dir ){
		ew->pos.x += -120.0f ;
	} else {
		ew->pos.x -= -120.0f ;
	}
	ew->pos.y += DOT_Y( 176.0f ) ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.y = DOT_Y( 8.0f ) ;
	ew->atck_no = pl0f_at_flame ;
	HIT_RESET( ew ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT | HIT_OWNER ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	pl_char_set( ew, Char_Effect, 3 ) ;
	shl0f_01_move( ew, pl_w ) ;
}

static void shl0f_01_rise( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0f_01_hit( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl0f_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0f_01_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0f_01_rise, shl0f_01_hit,
	} ;

	if( pl_w->char_no.b.grp != Char_Super || ew->pos.y > pl_w->ground_y + DOT_Y( 512.0f ) ){
		ew->routine_0 ++ ;
		return ;
	}
	shl0f_01_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl0f_01_rise( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
	if( ew->hit_flag ){
		ew->routine_1 ++ ;
		ew->spd.y = DOT_Y( 2.0f ) ;
		ew->timer0 = 120 ;
	}
}

/*--------------------------------------------------------------*/
static void shl0f_01_hit( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
	if( !( ew->timer0 & 7 ) ){
		ew->atck_no = pl0f_at_flame ;
		HIT_RESET( ew ) ;
	}
	if( ew->timer0 == 8 ){
		ew->atck_no = pl0f_at_flame_last ;
		HIT_RESET( ew ) ;
	}
	if( !ew->timer0 -- ){
		ew->routine_0 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void shl0f_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0F_LOCAL_W *lw = PL0F_LW( pl_w ) ;

	lw->flame_end_flag = 1 ;
	ew->disp_flag = 0 ;
	ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void shl0f_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl0f_01.c" End of File	*/

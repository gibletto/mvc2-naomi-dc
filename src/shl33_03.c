/* Iron Man's effect 03 (character 33's shot 03): the cannon of his Proton Cannon, while he is in
   Char_Super. Type 0 appears 144 dots above his feet, runs its animation (then he may fire) and
   comes down to 99 dots, hitting on the way; type 1, set when the firing is over, starts there and
   goes up while its animation runs. */
#include "player.h"
#include "pl_char.h"
#include "pl33_def.h"

static void shl33_03_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl33_03_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL_ST, LIST_TOP ) ) ){
		ew->work_func = shl33_03_main ;
		ew->ef_no = 0x3303 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = a ;
		ew->sub_type = b ;
	}
	return ew ;
}

static void shl33_03_down_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl33_03_up_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl33_03_main( PLAYER_WORK *ew )
{
	static void ( *shl33_03_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl33_03_down_main, shl33_03_up_main,
	} ;

	shl33_03_jmp[ew->type]( ew, ew->master_w ) ;
}

static void shl33_03_down_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl33_03_down_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl33_03_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl33_03_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl33_03_down_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl33_03_down_main_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl33_03_down_init, shl33_03_down_move, shl33_03_end, shl33_03_free,
	} ;

	shl33_03_down_main_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl33_03_down_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->sub_prio = -1 ;
	ew->sub_type = 0 ;
	ew->atck_no = pl33_at_proton ;
	HIT_RESET( ew ) ;
	ew->atck_flag = HIT_ON | HIT_OWNER ;
	ew->dm_flag = HIT_ON | HIT_OWNER ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	ew->pos.y += DOT_Y( 144.0f ) ;
	pl_char_set( ew, Char_Effect, 24 ) ;
	shl33_03_down_move( ew, pl_w ) ;
}

static void shl33_03_drop( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl33_03_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			It lasts while Iron Man is in the animation			*/
/*			group Char_Super									*/
/*==============================================================*/
static void shl33_03_down_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl33_03_down_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl33_03_drop, shl33_03_fall,
	} ;

	if( vs_sys_work.game_stop & ( 1 << ( ew->pl_no ^ 1 ) ) ){
	} else if( pl_w->char_no.b.grp != Char_Super || pl_w->routine_1 ){
		shl33_03_free( ew, pl_w ) ;
	} else {
		ew->prio = pl_w->prio ;
		if( !ew->sub_type ) ew->sub_prio = -1 ;
		else ew->sub_prio = 1 ;
		ew->pos.x = pl_w->pos.x ;
		if( pl_w->hit_stop ){
		} else shl33_03_down_move_jmp[ew->routine_1]( ew, pl_w ) ;
	}
}

/*==============================================================*/
static void shl33_03_drop( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL33_LOCAL_W *lw = ( PL33_LOCAL_W * )&pl_w->tlocal[0] ;

	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		lw->ready_flag = 1 ;
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		ew->acc.y = -DOT_Y( 0.25f ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl33_03_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type ) ew->sub_type = 1 ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	hit_work_set( ew ) ;
	if( ew->pos.y <= pl_w->ground_y + DOT_Y( 99.0f ) ){
		ew->routine_1 ++ ;
		shl33_03_end( ew, pl_w ) ;
	}
}

static void shl33_03_up_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl33_03_up_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl33_03_up_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl33_03_up_main_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl33_03_up_init, shl33_03_up_move, shl33_03_end, shl33_03_free,
	} ;

	shl33_03_up_main_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl33_03_up_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->sub_prio = -1 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	ew->pos.y += DOT_Y( 99.0f ) ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->acc.y = 0.01674107f ;
	pl_char_set( ew, Char_Effect, 25 ) ;
	shl33_03_up_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl33_03_up_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->prio = pl_w->prio ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		shl33_03_end( ew, pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl33_03_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl33_03_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl33_03.c" End of File	*/

/* Magneto's effect 01 (character 2c's shot 01): the sparks that fly off him, at random, and then
   drift away. */
#include "player.h"
#include "pl_char.h"
#include "pl2c_def.h"

static void shl2c_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl2c_01_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl2c_01_main ;
		ew->type = type ;
		ew->sub_type = 0 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		ew->ef_no = 0x2c01 ;
	}
	return ew ;
}

static void shl2c_01_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2c_01_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2c_01_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl2c_01_main( PLAYER_WORK *ew )
{
	static void ( *shl2c_01_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2c_01_start, shl2c_01_wait, shl2c_01_fly,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;
	Sint16 n ;

	if( ew->routine_0 >= 2 ){
		if( !ew->type ) PL2C_LW( pl_w )->shot_flag = 0 ;
		work_free( ew ) ;
	}
	else if( !ew->routine_0 ){
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
		ew->atck_flag = HIT_ON | HIT_SHOT ;
		ew->dm_flag = HIT_ON | HIT_SHOT ;
		ew->pos.x = pl_w->pos.x + DOT_X( 8.0f ) ;
		ew->pos.y = pl_w->pos.y + DOT_Y( 80.0f ) ;
		n = ( Rnd() & 31 ) + 32 ;
		if( n & 1 ) n = -n ;
		ew->pos.x += DOT_X( n ) ;
		n = Rnd() & 31 ;
		if( n & 1 ) n = -n ;
		ew->pos.y += DOT_Y( n ) ;
		ew->size_top = ew->size_bottom = ew->size_front = ew->size_back = 48 ;
		ew->hit_flag = 0 ;
		if( ew->sub_type = Rnd() & 3 ) ew->sub_prio = -8 ;
		else ew->sub_prio = 8 ;
		pl_char_set( ew, 21, 7 ) ;
	}
	else shl2c_01_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl2c_01_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		pl_char_set( ew, 21, ew->sub_type + 8 ) ;
	}
}

/*==============================================================*/
static void shl2c_01_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		if( Sys->game_timer0 & 3 ){
			ew->atck_no = pl2c_at_tempest_2 ;
			HIT_RESET( ew ) ;
		} else {
			ew->atck_no = pl2c_at_tempest ;
			HIT_RESET( ew ) ;
		}
		ew->dir = Rnd() & 15 ;
		if( !ew->disp_dir ) ew->dir += 16 ;
		if( !ew->type ) ew->obj_flag = ( Rnd() & 3 ) * 100 + 1000 ;
		else ew->obj_flag = ( Rnd() & 3 ) * 100 + 1800 ;
	}
}

/*--------------------------------------------------------------*/
static void shl2c_01_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->hit_box_no ){
		dir_spd_move( ew, ew->obj_flag ) ;
		pl_char_move( ew ) ;
		if( scr_in_ck( ew ) ) hit_work_set( ew ) ;
		else work_free( ew ) ;
	}
	else work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl2c_01.c" End of File	*/

/* Dhalsim's effect 00 (character 25's shot 00): the yoga fireball he spits. It flies straight until it
   hits or leaves the screen, and tells him it is gone through his local work. */
#include "player.h"
#include "pl_char.h"
#include "pl25_def.h"

static void shl25_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl25_00_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl25_00_main ;
		ew->master_w = pl_w ;
	}
	return ew ;
}

static void shl25_00_init( PLAYER_WORK *ew );
static void shl25_00_move( PLAYER_WORK *ew );
static void shl25_00_hit( PLAYER_WORK *ew );
static void shl25_00_end( PLAYER_WORK *ew );
static void shl25_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl25_00_main( PLAYER_WORK *ew )
{
	static void ( *shl25_00_jmp[] )( PLAYER_WORK *ew ) = {
		shl25_00_init, shl25_00_move, shl25_00_hit, shl25_00_end, shl25_00_free,
	} ;

	shl25_00_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl25_00_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PL25_LOCAL_W *pl_lw = PL25_LW( pl_w ) ;

	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->size_front = 64 ;
	ew->size_back = 64 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->prio = 11 ;
	ew->disp_flag = 1 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.y += DOT_Y( 54.0f ) ;
	ew->pos.z = ew->master_w->pos.z ;
	if( ew->disp_dir ) ew->pos.x += 120.0f ;
	else ew->pos.x -= 120.0f ;
	if( !ew->atck_level ) ew->spd.x = -DOT_X( 2.5f ) ;
	else ew->spd.x = -7.5f ;
	if( ew->disp_dir ) ew->spd.x = -ew->spd.x ;
	pl_lw->shot_flag = 1 ;
	pl_char_set( ew, 20, 0 ) ;
	ew->atck_no = pl25_at_fire ;
	HIT_RESET( ew ) ;
	shl25_00_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl25_00_move( PLAYER_WORK *ew )
{
	if( !scr_in_x_ck( ew ) ){
		shl25_00_end( ew ) ;
		return ;
	}
	if( ew->hit_flag || ew->hit_box_no ){
		ew->routine_0 ++ ;
		pl_char_set( ew, 20, 1 ) ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	hit_work_set( ew ) ;
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl25_00_hit( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ) shl25_00_end( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl25_00_end( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PL25_LOCAL_W *pl_lw = PL25_LW( pl_w ) ;

	pl_lw->shot_flag = 0 ;
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl25_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl25_00.c" End of File	*/
